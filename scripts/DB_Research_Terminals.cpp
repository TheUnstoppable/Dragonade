/*	Renegade Scripts.dll
	Copyright 2016 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#include "general.h"
#include "scripts.h"
#include "VehicleGameObj.h"
#include "engine.h"
#include "engine_game.h"
#include "engine_tt.h"
#include "GameObjManager.h"
#include "DB_Research_Terminals.h"
#include "DB_General.h"
#include "dp88_custom_timer_defines.h"
              
#include "SoldierGameObjDef.h"
#include "PowerupGameObjDef.h"
#include "BuildingGameObj.h"

void Set_Model_Custom(GameObject *obj,const char *Model_Name)
{
	if(obj)
	{
		Commands->Set_Model(obj,Model_Name);
		Commands->Send_Custom_Event(obj,obj,REATTACH_BONES_CUSTOM,0,0);
	}
}

void DB_Research_Listener::Created(GameObject *obj)
{
	//dummy script to mark that this object should recieve research customs
}
ScriptRegistrant<DB_Research_Listener> DB_Research_Listener_Registrant("DB_Research_Listener", "");


void DB_Research_Infantry_Armor_Terminal::Created(GameObject *obj)
{
	Vector3 pos = Commands->Get_Position(obj);
	pos.Z+=0.01f;
	Commands->Set_Position(obj,pos);
	UpgradeName = Get_Translated_Preset_Name(obj);
	Allow_Poke = true;
	Upgrading_In_Progress = false;
	Terminal_Team = Get_Int_Parameter("Terminal_Team");
	Set_Object_Type(obj, Terminal_Team);
	Upgrade_Completed = false;
	UpgradeID = Get_Int_Parameter("UpgradeID");
	if(!UpgradeID)
		UpgradeID=1;
	Upgrade_In_Progress_Text = Get_Parameter("Upgrade_In_Progress_Text");
	Upgrade_In_Progress_Sound = Get_Parameter("Upgrade_In_Progress_Sound");
	Upgrade_Money_Added_Sound = Get_Parameter("Upgrade_Money_Added_Sound");
	Upgrade_Completed_Sound = Get_Parameter("Upgrade_Completed_Sound");
	Upgrade_Insufficient_Funds_Sound = Get_Parameter("Upgrade_Insufficient_Funds_Sound");
	Upgrade_Total_Cost = Get_Float_Parameter("Upgrade_Total_Cost");
	Upgrade_Current_Funds = 0.0f;
	Upgrade_Add_Funds_Cost = Get_Float_Parameter("Upgrade_Add_Funds_Cost");
	Upgrade_Access_Denied_Sound = Get_Parameter("Upgrade_Access_Denied_Sound");
	Upgrade_Complete_Text = Get_Parameter("Upgrade_Complete_Text");
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	Commands->Start_Timer(obj,this,0.0f,5678);
}


void DB_Research_Infantry_Armor_Terminal::Poked(GameObject *obj, GameObject *poker)
{
	if (Allow_Poke)
	{
		if (Get_Object_Type(poker) == Terminal_Team)
		{
			if (!Upgrade_Completed)
			{
				if (!Upgrading_In_Progress)
				{
					if (!Is_Building_Dead(Find_Building_By_Type(Terminal_Team,BuildingConstants::TYPE_COM_CENTER)))
					{
						if (Commands->Get_Money(poker) >= Upgrade_Add_Funds_Cost*InsertScale)
						{
							if (Is_Base_Powered(Terminal_Team))
							{
								if (Upgrade_Current_Funds < Upgrade_Total_Cost)
								{
									Upgrade_Current_Funds += (Upgrade_Add_Funds_Cost*InsertScale)/CostScale;
									Commands->Give_Money(poker, -Upgrade_Add_Funds_Cost*InsertScale, false);
									Create_2D_Wave_Sound_Dialog_Player(poker, Upgrade_Money_Added_Sound);
									Send_Message_Team(Terminal_Team,175,175,255,StringClass::getFormattedString("%s added %g to the %s Fund. Status: %g/%g",Get_Player_Name(poker),Upgrade_Add_Funds_Cost*InsertScale,Get_Translated_Preset_Name(obj),Upgrade_Current_Funds*CostScale,Upgrade_Total_Cost*CostScale));
									if (Upgrade_Current_Funds >= Upgrade_Total_Cost)
									{
										Upgrading_In_Progress = true;
										Commands->Start_Timer(obj,this,Research_Technology::UPGRADE_INFANTRY_ARMOR_TIME,Research_Technology::UPGRADE_INFANTRY_ARMOR_TIMER_NUMBER);
										Send_Message_Team(Terminal_Team,125,125,255,Upgrade_In_Progress_Text);
										Create_2D_WAV_Sound_Team(Upgrade_In_Progress_Sound, Terminal_Team);
									}
								}
							}
							else //if power is down
							{
								if (Upgrade_Current_Funds < (Upgrade_Total_Cost) )
								{
									Upgrade_Current_Funds += (Upgrade_Add_Funds_Cost*InsertScale)/CostScale;
									Commands->Give_Money(poker, -Upgrade_Add_Funds_Cost*InsertScale, false);
									Create_2D_Wave_Sound_Dialog_Player(poker, Upgrade_Money_Added_Sound);
									Send_Message_Team(Terminal_Team,175,175,255,StringClass::getFormattedString("%s added %g to the %s Fund. Status: %g/%g",Get_Player_Name(poker),Upgrade_Add_Funds_Cost*InsertScale,Get_Translated_Preset_Name(obj),Upgrade_Current_Funds*CostScale,Upgrade_Total_Cost*CostScale));
									if (Upgrade_Current_Funds >= (Upgrade_Total_Cost ))
									{
										Upgrading_In_Progress = true;
										Commands->Start_Timer(obj,this,Research_Technology::UPGRADE_INFANTRY_ARMOR_TIME,Research_Technology::UPGRADE_INFANTRY_ARMOR_TIMER_NUMBER);
										if(Upgrade_In_Progress_Text)
										{
											Send_Message_Team(Terminal_Team,125,125,255,Upgrade_In_Progress_Text);
										}
										Create_2D_WAV_Sound_Team(Upgrade_In_Progress_Sound, Terminal_Team);
									}
								}						
							}
						}
						else
						{
							Send_Message_Player(poker,255,255,255,"Insufficient Funds!");
							Create_2D_Wave_Sound_Dialog_Player(poker, Upgrade_Insufficient_Funds_Sound);
						}
					}
					else
					{
						Create_2D_Wave_Sound_Dialog_Player(poker, Upgrade_Access_Denied_Sound);
						Send_Message_Player(poker,255,255,255,StringClass::getFormattedString("Sorry but your Communications Center is dead, researching of the Upgrade %s is not possible!",Get_Translated_Preset_Name(obj)));
					}
				}
				else
				{
					Create_2D_Wave_Sound_Dialog_Player(poker, Upgrade_In_Progress_Sound);
					Send_Message_Player(poker,255,255,255,StringClass::getFormattedString("Sorry the %s's research is already in progress!",Get_Translated_Preset_Name(obj)));
				}
			}
			else
			{
				Create_2D_Wave_Sound_Dialog_Player(poker, Upgrade_Access_Denied_Sound);
				Send_Message_Player(poker,255,255,255,"Access Denied: This upgrade has already been completed!");
			}
		}
		else
		{
			Create_2D_Wave_Sound_Dialog_Player(poker, Upgrade_Access_Denied_Sound);
			Send_Message_Player(poker,255,255,255,"Access Denied: You are on the wrong team!");
		}
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
		Allow_Poke = false; Commands->Start_Timer(obj,this,0.25f,700); //Allow poking the terminal after 2.5 seconds again
	}
}
void DB_Research_Infantry_Armor_Terminal::Timer_Expired(GameObject *obj, int number)
{
	if (number == 700)
	{
		Allow_Poke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
	else if (number == Research_Technology::UPGRADE_INFANTRY_ARMOR_TIMER_NUMBER)
	{
		//if (!Is_Building_Dead(Find_Building_By_Type(Terminal_Team,BuildingConstants::TYPE_COM_CENTER)))
		{
			Create_2D_WAV_Sound_Team(Upgrade_Completed_Sound, Terminal_Team);
			Send_Message_Team(Terminal_Team,100,100,255,Upgrade_Complete_Text);
			Send_Custom_Event_To_Objects_With_Script(obj,"DB_Research_Listener",Research_Technology::UPGRADE_INFANTRY_ARMOR_CUSTOM,UpgradeID,0);
			Upgrade_Completed = true;
			Upgrading_In_Progress = false;
			UpgradeComplete.Insert(UpgradeID,UpgradeID);
			//DALogManager::Write_Log("_INFO","%s %s completed. (ID: %i)",Get_Team_Name(Terminal_Team),Get_Translated_Preset_Name(obj),UpgradeID);
		}
	}

		else if (number == 5678)
	{

		if(Upgrade_Completed)
		{
			Commands->Set_Animation_Frame(obj,Get_Model(obj),9);
		}
		else if(Is_Building_Dead(Find_Com_Center(Terminal_Team)))
		{
			Commands->Set_Animation_Frame(obj,Get_Model(obj),0);
		}
		else if(Upgrade_Current_Funds / Upgrade_Total_Cost >= 1.0)
		{
			Commands->Set_Animation_Frame(obj,Get_Model(obj),7);
		}
		else if(Upgrade_Current_Funds / Upgrade_Total_Cost >= 0.8)
		{
			Commands->Set_Animation_Frame(obj,Get_Model(obj),5);
		}
		else if(Upgrade_Current_Funds / Upgrade_Total_Cost >= 0.6)
		{
			Commands->Set_Animation_Frame(obj,Get_Model(obj),4);
		}
		else if(Upgrade_Current_Funds / Upgrade_Total_Cost >= 0.4)
		{
			Commands->Set_Animation_Frame(obj,Get_Model(obj),3);
		}
		else if(Upgrade_Current_Funds / Upgrade_Total_Cost >= 0.2)
		{
			Commands->Set_Animation_Frame(obj,Get_Model(obj),2);
		}
		else if(Upgrade_Current_Funds / Upgrade_Total_Cost >= 0.0)
		{
			Commands->Set_Animation_Frame(obj,Get_Model(obj),1);
		}
		Commands->Start_Timer(obj,this,0.5f,5678);
	}
}

void DB_Research_Infantry_Armor_Terminal::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if(message == 456987)
	{
		StringClass TempName;
		TempName = StringClass::getFormattedString("$ %g - %s",Upgrade_Total_Cost*CostScale,UpgradeName);
		if(obj->As_DamageableGameObj() && sender)
			Change_String_Player(sender,obj->As_DamageableGameObj()->Get_Translated_Name_ID(),TempName);
		//Force_Position_Update(obj);
	}

	else if(message == 456986)
	{
		StringClass TempName;
		TempName = StringClass::getFormattedString("$ %g - %s",Upgrade_Total_Cost*CostScale,UpgradeName);
		for(SLNode<SoldierGameObj> *z = GameObjManager::StarGameObjList.Head();z;z = z->Next())
			Change_String_Player(z->Data(),obj->As_DamageableGameObj()->Get_Translated_Name_ID(),TempName);
	}

	else if(message == 456985)
	{
		//Force_Position_Update(obj);
	}

}

void DB_Research_Infantry_Armor_Terminal::Detach(GameObject *obj)
{
	UpgradeComplete.Remove(UpgradeID);
	CharacterID.Remove_All();
	/*
	delete [] UpgradeName;
	delete [] Upgrade_In_Progress_Sound;
	delete [] Upgrade_Money_Added_Sound;
	delete [] Upgrade_Completed_Sound;
	delete [] Upgrade_Insufficient_Funds_Sound;
	delete [] Upgrade_Access_Denied_Sound;
	delete [] Upgrade_Complete_Text;
	delete [] Upgrade_In_Progress_Text;
	*/
}
ScriptRegistrant<DB_Research_Infantry_Armor_Terminal> DB_Research_Infantry_Armor_Terminal_Registrant("DB_Research_Infantry_Armor_Terminal", "Terminal_Team=0:int,Upgrade_In_Progress_Sound=snd:string,Upgrade_In_Progress_Text=txt:string,Upgrade_Money_Added_Sound=snd:string,Upgrade_Access_Denied_Sound=snd:string,Upgrade_Completed_Sound=snd:string,Upgrade_Complete_Text=txt:string,Upgrade_Insufficient_Funds_Sound=snd:string,Upgrade_Total_Cost=0:float,Upgrade_Add_Funds_Cost=0:float,UpgradeID=1:int");

void DB_Rebuild_Building_Terminal::Created(GameObject *obj)
{
	UpgradeName = Get_Translated_Preset_Name(obj);
	Allow_Poke = true;
	Upgrading_In_Progress = false;
	Terminal_Team = Get_Int_Parameter("Terminal_Team");
	Set_Object_Type(obj, Terminal_Team);
	Upgrade_Completed = false;
	BuildingType = Get_Int_Parameter("BuildingType");
	Upgrade_In_Progress_Text = Get_Parameter("Upgrade_In_Progress_Text");
	Upgrade_In_Progress_Sound = Get_Parameter("Upgrade_In_Progress_Sound");
	Upgrade_Money_Added_Sound = Get_Parameter("Upgrade_Money_Added_Sound");
	Upgrade_Completed_Sound = Get_Parameter("Upgrade_Completed_Sound");
	Upgrade_Insufficient_Funds_Sound = Get_Parameter("Upgrade_Insufficient_Funds_Sound");
	Upgrade_Total_Cost = Get_Float_Parameter("Upgrade_Total_Cost");
	Upgrade_Current_Funds = 0.0f;
	Upgrade_Add_Funds_Cost = Get_Float_Parameter("Upgrade_Add_Funds_Cost");
	Upgrade_Access_Denied_Sound = Get_Parameter("Upgrade_Access_Denied_Sound");
	Upgrade_Complete_Text = Get_Parameter("Upgrade_Complete_Text");
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	Commands->Start_Timer(obj,this,0.0f,5678);
}
void DB_Rebuild_Building_Terminal::Poked(GameObject *obj, GameObject *poker)
{
	if (Allow_Poke)
	{
		if (Get_Object_Type(poker) == Terminal_Team)
		{
			if (!Upgrade_Completed)
			{
				if (!Upgrading_In_Progress)
				{
					if(!Is_Building_Dead(Find_Construction_Yard(Terminal_Team)))
					{
						if (Is_Building_Dead(Find_Building_By_Type(Terminal_Team,BuildingType)))
						{
							if (Commands->Get_Money(poker) >= Upgrade_Add_Funds_Cost*InsertScale)
							{
								if (Is_Base_Powered(Terminal_Team))
								{
									if (Upgrade_Current_Funds < Upgrade_Total_Cost)
									{
										Upgrade_Current_Funds += (Upgrade_Add_Funds_Cost*InsertScale)/CostScale;
										Commands->Give_Money(poker, -Upgrade_Add_Funds_Cost*InsertScale, false);
										Create_2D_Wave_Sound_Dialog_Player(poker, Upgrade_Money_Added_Sound);
										Send_Message_Team(Terminal_Team,150,255,150,StringClass::getFormattedString("%s added %g to the %s Fund. Status: %g/%g",Get_Player_Name(poker),Upgrade_Add_Funds_Cost*InsertScale,Get_Translated_Preset_Name(obj),Upgrade_Current_Funds*CostScale,Upgrade_Total_Cost*CostScale));
										if (Upgrade_Current_Funds >= Upgrade_Total_Cost)
										{
											Upgrading_In_Progress = true;
											Commands->Start_Timer(obj,this,Research_Technology::UPGRADE_INFANTRY_ARMOR_TIME,Research_Technology::UPGRADE_INFANTRY_ARMOR_TIMER_NUMBER);
											Send_Message_Team(Terminal_Team,100,255,100,Upgrade_In_Progress_Text);
											Create_2D_WAV_Sound_Team_Dialog(Upgrade_In_Progress_Sound, Terminal_Team);
										}
									}
								}
								else //if power is down
								{
									if (Upgrade_Current_Funds < (Upgrade_Total_Cost) )
									{
										Upgrade_Current_Funds += (Upgrade_Add_Funds_Cost*InsertScale)/CostScale;
										Commands->Give_Money(poker, -Upgrade_Add_Funds_Cost*InsertScale, false);
										Create_2D_Wave_Sound_Dialog_Player(poker, Upgrade_Money_Added_Sound);
										Send_Message_Team(Terminal_Team,150,255,150,StringClass::getFormattedString("%s added %g to the %s Fund. Status: %g/%g",Get_Player_Name(poker),Upgrade_Add_Funds_Cost*InsertScale,Get_Translated_Preset_Name(obj),Upgrade_Current_Funds*CostScale,Upgrade_Total_Cost*CostScale));
										if (Upgrade_Current_Funds >= (Upgrade_Total_Cost ))
										{
											Upgrading_In_Progress = true;
											Commands->Start_Timer(obj,this,Research_Technology::UPGRADE_INFANTRY_ARMOR_TIME,Research_Technology::UPGRADE_INFANTRY_ARMOR_TIMER_NUMBER);
											if(Upgrade_In_Progress_Text)
											{
												Send_Message_Team(Terminal_Team,100,255,100,Upgrade_In_Progress_Text);
											}
											Create_2D_WAV_Sound_Team_Dialog(Upgrade_In_Progress_Sound, Terminal_Team);
										}
									}						
								}
							}
							else
							{
								Send_Message_Player(poker,255,255,255,"Insufficient Funds!");
								Create_2D_Wave_Sound_Dialog_Player(poker, Upgrade_Insufficient_Funds_Sound);
							}
						}
						else
						{
							Create_2D_Wave_Sound_Dialog_Player(poker, Upgrade_Access_Denied_Sound);
							Send_Message_Player(poker,255,255,255,StringClass::getFormattedString("Structure is not Destroyed!",Get_Translated_Preset_Name(obj)));
						}
					}
					else
					{
						Create_2D_Wave_Sound_Dialog_Player(poker, Upgrade_Access_Denied_Sound);
						Send_Message_Player(poker,255,255,255,StringClass::getFormattedString("Access Denied: Construction Yard is Destroyed!",Get_Translated_Preset_Name(obj)));
					}
				}
				else
				{
					Create_2D_Wave_Sound_Dialog_Player(poker, Upgrade_Access_Denied_Sound);
					Send_Message_Player(poker,255,255,255,StringClass::getFormattedString("Building Construction is already in progress!",Get_Translated_Preset_Name(obj)));
				}
			}
			else
			{
				Create_2D_Wave_Sound_Dialog_Player(poker, Upgrade_Access_Denied_Sound);
				Send_Message_Player(poker,255,255,255,"Access Denied: Building Construction has already been completed!");
			}
		}
		else
		{
			Create_2D_Wave_Sound_Dialog_Player(poker, Upgrade_Access_Denied_Sound);
			Send_Message_Player(poker,255,255,255,"Access Denied: You are on the wrong team!");
		}
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
		Allow_Poke = false; Commands->Start_Timer(obj,this,0.25f,700); //Allow poking the terminal after 2.5 seconds again
	}
}
void DB_Rebuild_Building_Terminal::Timer_Expired(GameObject *obj, int number)
{
	if (number == 700)
	{
		Allow_Poke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
	else if (number == Research_Technology::UPGRADE_INFANTRY_ARMOR_TIMER_NUMBER)
	{
		{
			Create_2D_WAV_Sound_Team(Upgrade_Completed_Sound, Terminal_Team);
			Send_Message_Team(Terminal_Team,50,255,50,Upgrade_Complete_Text);
			
			if(Is_Building_Dead(Find_Building_By_Type(Terminal_Team,BuildingType)))
			{
				Find_Building_By_Type(Terminal_Team,BuildingType)->As_BuildingGameObj()->On_Revived();
				Upgrade_Current_Funds=0;
				Commands->Set_Animation_Frame(obj,Get_Model(obj),9);
				//DALogManager::Write_Log("_INFO","%s %s Rebuilt.",Get_Team_Name(Terminal_Team),Get_Translated_Preset_Name(Find_Building_By_Type(Terminal_Team,BuildingType)));
			}
			Upgrading_In_Progress = false;
		}
	}

	else if (number == 5678)
	{
		if(Is_Building_Dead(Find_Construction_Yard(Terminal_Team)))
		{
			Commands->Set_Animation_Frame(obj,Get_Model(obj),0);
		}
		else if(!Is_Building_Dead(Find_Building_By_Type(Terminal_Team,BuildingType)))
		{
			Commands->Set_Animation_Frame(obj,Get_Model(obj),9);
		}
		else if(Upgrade_Current_Funds / Upgrade_Total_Cost >= 1.0)
		{
			Commands->Set_Animation_Frame(obj,Get_Model(obj),7);
		}
		else if(Upgrade_Current_Funds / Upgrade_Total_Cost >= 0.8)
		{
			Commands->Set_Animation_Frame(obj,Get_Model(obj),5);
		}
		else if(Upgrade_Current_Funds / Upgrade_Total_Cost >= 0.6)
		{
			Commands->Set_Animation_Frame(obj,Get_Model(obj),4);
		}
		else if(Upgrade_Current_Funds / Upgrade_Total_Cost >= 0.4)
		{
			Commands->Set_Animation_Frame(obj,Get_Model(obj),3);
		}
		else if(Upgrade_Current_Funds / Upgrade_Total_Cost >= 0.2)
		{
			Commands->Set_Animation_Frame(obj,Get_Model(obj),2);
		}
		else if(Upgrade_Current_Funds / Upgrade_Total_Cost >= 0.0)
		{
			Commands->Set_Animation_Frame(obj,Get_Model(obj),1);
		}
		Commands->Start_Timer(obj,this,0.5f,5678);
	}
}

void DB_Rebuild_Building_Terminal::Detach(GameObject *obj)
{
	/*
	delete [] UpgradeName;
	delete [] Upgrade_In_Progress_Sound;
	delete [] Upgrade_Money_Added_Sound;
	delete [] Upgrade_Completed_Sound;
	delete [] Upgrade_Insufficient_Funds_Sound;
	delete [] Upgrade_Access_Denied_Sound;
	delete [] Upgrade_Complete_Text;
	delete [] Upgrade_In_Progress_Text;
	*/
}

void DB_Rebuild_Building_Terminal::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if(message == 456987)
	{
		StringClass TempName;
		TempName = StringClass::getFormattedString("%g - %s",Upgrade_Total_Cost*CostScale,UpgradeName);
		if(obj->As_DamageableGameObj() && sender)
			Change_String_Player(sender,obj->As_DamageableGameObj()->Get_Translated_Name_ID(),TempName);
	}

	else if(message == 456986)
	{
		StringClass TempName;

		TempName = StringClass::getFormattedString("%g - %s",Upgrade_Total_Cost*CostScale,UpgradeName);
		for(SLNode<SoldierGameObj> *z = GameObjManager::StarGameObjList.Head();z;z = z->Next())
			Change_String_Player(z->Data(),obj->As_DamageableGameObj()->Get_Translated_Name_ID(),TempName);
	}
}

ScriptRegistrant<DB_Rebuild_Building_Terminal> DB_Rebuild_Building_Terminal_Registrant("DB_Rebuild_Building_Terminal", "Terminal_Team=0:int,Upgrade_In_Progress_Sound=snd:string,Upgrade_In_Progress_Text=txt:string,Upgrade_Money_Added_Sound=snd:string,Upgrade_Access_Denied_Sound=snd:string,Upgrade_Completed_Sound=snd:string,Upgrade_Complete_Text=txt:string,Upgrade_Insufficient_Funds_Sound=snd:string,Upgrade_Total_Cost=0:float,Upgrade_Add_Funds_Cost=0:float,BuildingType=0:int");



void DB_Research_Infantry_Armor_Ability::Created(GameObject *obj)
{
	UpgradeID = Get_Int_Parameter("UpgradeID");
	Upgraded=false;
	if(!UpgradeID)
		UpgradeID=1;
	Attach_Script_Once(obj,"DB_Research_Listener","");
	if(UpgradeComplete.Get(UpgradeID,0))
	{
		Upgraded=true;
		float MaxAP = Commands->Get_Max_Shield_Strength(obj);
		float NewMaxAP = MaxAP + Get_Float_Parameter("Extra_Armor_Amount");
		float CurAP = Commands->Get_Shield_Strength(obj);
		float NewAP = CurAP + Get_Float_Parameter("Extra_Armor_Amount");
		Set_Max_Shield_Strength(obj, NewMaxAP);
		Commands->Set_Shield_Strength(obj, NewAP);
	}
}
void DB_Research_Infantry_Armor_Ability::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (!Upgraded &&  message == Research_Technology::UPGRADE_INFANTRY_ARMOR_CUSTOM && param==UpgradeID)
	{
		Upgraded=true;
		float MaxAP = Commands->Get_Max_Shield_Strength(obj);
		float NewMaxAP = MaxAP + Get_Float_Parameter("Extra_Armor_Amount");
		float CurAP = Commands->Get_Shield_Strength(obj);
		float NewAP = CurAP + Get_Float_Parameter("Extra_Armor_Amount");
		Set_Max_Shield_Strength(obj, NewMaxAP);
		Commands->Set_Shield_Strength(obj, NewAP);
		Create_2D_Wave_Sound_Dialog_Player(obj, Get_Parameter("Unit_Upgraded_Sound"));
		Send_Message_Player(obj,255,255,255,"Unit Upgraded!");
	}
}
ScriptRegistrant<DB_Research_Infantry_Armor_Ability> DB_Research_Infantry_Armor_Ability_Registrant("DB_Research_Infantry_Armor_Ability", "Extra_Armor_Amount=0:float,Unit_Upgraded_Sound=bla:string,UpgradeID=1:int");


void DB_Research_Infantry_Hitpoints_Ability::Created(GameObject *obj)
{
	UpgradeID = Get_Int_Parameter("UpgradeID");
	Upgraded=false;
	if(!UpgradeID)
		UpgradeID=1;
	Attach_Script_Once(obj,"DB_Research_Listener","");
	if(UpgradeComplete.Get(UpgradeID,0))
	{
		Upgraded=true;
		const char *model = Get_Parameter("Model");
		float MaxHP = Commands->Get_Max_Health(obj);
		float NewMaxHP = MaxHP + Get_Float_Parameter("Extra_Health_Amount");
		float CurHP = Commands->Get_Health(obj);
		float NewHP = CurHP + Get_Float_Parameter("Extra_Health_Amount");
		float MaxAP = Commands->Get_Max_Shield_Strength(obj);
		float NewMaxAP = MaxAP + Get_Float_Parameter("Extra_Armor_Amount");
		float CurAP = Commands->Get_Shield_Strength(obj);
		float NewAP = CurAP + Get_Float_Parameter("Extra_Armor_Amount");
		Set_Max_Health(obj, NewMaxHP);
		Commands->Set_Health(obj, NewHP);
		Set_Max_Shield_Strength(obj, NewMaxAP);
		Commands->Set_Shield_Strength(obj, NewAP);
		if(strcmp(model,"") != 0)
			Set_Model_Custom(obj,model);
	}
}
void DB_Research_Infantry_Hitpoints_Ability::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (!Upgraded &&  message == Research_Technology::UPGRADE_INFANTRY_ARMOR_CUSTOM && param==UpgradeID)
	{
		Upgraded=true;
		const char *model = Get_Parameter("Model");
		float MaxHP = Commands->Get_Max_Health(obj);
		float NewMaxHP = MaxHP + Get_Float_Parameter("Extra_Health_Amount");
		float CurHP = Commands->Get_Health(obj);
		float NewHP = CurHP + Get_Float_Parameter("Extra_Health_Amount");
		float MaxAP = Commands->Get_Max_Shield_Strength(obj);
		float NewMaxAP = MaxAP + Get_Float_Parameter("Extra_Armor_Amount");
		float CurAP = Commands->Get_Shield_Strength(obj);
		float NewAP = CurAP + Get_Float_Parameter("Extra_Armor_Amount");
		Set_Max_Health(obj, NewMaxHP);
		Commands->Set_Health(obj, NewHP);
		Set_Max_Shield_Strength(obj, NewMaxAP);
		Commands->Set_Shield_Strength(obj, NewAP);
		if(strcmp(model,"") != 0)
			Set_Model_Custom(obj,model);
		Create_2D_Wave_Sound_Dialog_Player(obj, Get_Parameter("Unit_Upgraded_Sound"));
		Send_Message_Player(obj,255,255,255,"Unit Upgraded!");
	}
}
ScriptRegistrant<DB_Research_Infantry_Hitpoints_Ability> DB_Research_Infantry_Hitpoints_Ability_Registrant("DB_Research_Infantry_Hitpoints_Ability","Extra_Health_Amount=0:float,Extra_Armor_Amount=0:float,Model:string,Unit_Upgraded_Sound=bla:string,UpgradeID=1:int");



void DB_Research_Weapon_Upgrade::Created(GameObject *obj)
{
	UpgradeID = Get_Int_Parameter("UpgradeID");
	Secondary = Get_Int_Parameter("Secondary_Weapon");
	RemoveWeapon = Get_Parameter("Remove_Weapon_Preset");
	Upgraded=false;
	Attach_Script_Once(obj,"DB_Research_Listener","");
	if(UpgradeComplete.Get(UpgradeID,0))
	{
		Upgraded=true;
		const char *PowerUpPreset = Get_Parameter("PowerupPreset");
		const char *model = Get_Parameter("Model");
		if(Find_Named_Definition(PowerUpPreset))
		{
			if(Get_Powerup_Weapon(PowerUpPreset))
			{
				Grant_Powerup(obj,PowerUpPreset);
				if(!Secondary)
				{
					if(Has_Weapon(obj,Get_Powerup_Weapon(PowerUpPreset)))
						Commands->Select_Weapon(obj,Get_Powerup_Weapon(PowerUpPreset));
				}
				if(strcmp(RemoveWeapon,"") != 0)
				{
					if(Has_Weapon(obj,RemoveWeapon))
						Remove_Weapon(obj,RemoveWeapon);
				}
			
				if(!Secondary && obj->As_SoldierGameObj() && !CharacterID.Exists(obj->Get_Definition().Get_ID()))
				{
					CharacterID.Insert(obj->Get_Definition().Get_ID(),1);
					SoldierGameObjDef *def = (SoldierGameObjDef *)Find_Definition(obj->Get_Definition().Get_ID());
					if(def)
					{
						DefinitionClass *powerupdef = Find_Named_Definition(PowerUpPreset);
						PowerUpGameObjDef *c = (PowerUpGameObjDef *)powerupdef;
						if (c)
						{
							 //def->WeaponDefID=c->Get_Grant_Weapon_ID();
						}
					}
					//DALootGameFeature->Settings_Loaded_Event();
					//DAEventManager::Settings_Loaded_Event();
				}	
			
			}
			//Remove_Weapon(obj,Get_Weapon(obj,0));
			if(strcmp(model,"") != 0)
				Set_Model_Custom(obj,model);
		}
		else
		{
			Console_Input(StringClass::getFormattedString("msg invalid weapon preset: %s - %s",Get_Translated_Preset_Name(obj),PowerUpPreset));
		}
	}
	
}
void DB_Research_Weapon_Upgrade::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (!Upgraded && message == Research_Technology::UPGRADE_INFANTRY_ARMOR_CUSTOM && param==UpgradeID)
	{
		Upgraded=true;
		const char *PowerUpPreset = Get_Parameter("PowerupPreset");
		const char *model = Get_Parameter("Model");
		if(Find_Named_Definition(PowerUpPreset))
		{
			if(Get_Powerup_Weapon(PowerUpPreset))
			{
				if(Commands->Is_A_Star(obj))
				{
					//Find_Player(Get_Player_ID(obj))->Get_DA_Player()->Reset_Creation_Time();
				}
				Grant_Powerup(obj,PowerUpPreset);
				if(!Secondary)
				{
					if(Has_Weapon(obj,Get_Powerup_Weapon(PowerUpPreset)))
						Commands->Select_Weapon(obj,Get_Powerup_Weapon(PowerUpPreset));
				}
				if(strcmp(RemoveWeapon,"") != 0)
				{
					if(Has_Weapon(obj,RemoveWeapon))
						Remove_Weapon(obj,RemoveWeapon);
				}


				if(!Secondary && obj->As_SoldierGameObj() && !CharacterID.Exists(obj->Get_Definition().Get_ID()))
				{
					CharacterID.Insert(obj->Get_Definition().Get_ID(),1);
					SoldierGameObjDef *def = (SoldierGameObjDef *)Find_Definition(obj->Get_Definition().Get_ID());
					if(def)
					{
						DefinitionClass *powerupdef = Find_Named_Definition(PowerUpPreset);
						PowerUpGameObjDef *c = (PowerUpGameObjDef *)powerupdef;
						if (c)
						{
							 //def->WeaponDefID=c->Get_Grant_Weapon_ID();
						}
					}
					//DALootGameFeature->Settings_Loaded_Event();
					//DAEventManager::Settings_Loaded_Event();
				}	
			
			}
		}
		else
		{
			Console_Input(StringClass::getFormattedString("msg invalid weapon preset: %s - %s",Get_Translated_Preset_Name(obj),PowerUpPreset));
		}
		
		if(strcmp(model,"") != 0)
			Set_Model_Custom(obj,model);
		Create_2D_Wave_Sound_Dialog_Player(obj, Get_Parameter("Unit_Upgraded_Sound"));
		Send_Message_Player(obj,255,255,255,"Unit Upgraded!");
	}
}
ScriptRegistrant<DB_Research_Weapon_Upgrade> DB_Research_Weapon_Upgrade_Registrant("DB_Research_Weapon_Upgrade", "PowerupPreset:string,Model:string,Unit_Upgraded_Sound=bla:string,UpgradeID=1:int,Remove_Weapon_Preset=0:string,Secondary_Weapon=0:int");


void DB_Research_Armor_Model::Created(GameObject *obj)
{
	UpgradeID = Get_Int_Parameter("UpgradeID");
	Upgraded=false;
	Attach_Script_Once(obj,"DB_Research_Listener","");
	if(UpgradeComplete.Get(UpgradeID,0))
	{
		Upgraded=true;
		float HealthShield = Get_Float_Parameter("HealthShieldBonus");
		const char *model = Get_Parameter("Model");
		
		float MaxAP = Commands->Get_Max_Shield_Strength(obj);
		float NewMaxAP = MaxAP + HealthShield;
		float CurAP = Commands->Get_Shield_Strength(obj);
		float NewAP = CurAP + HealthShield;

		float MaxHP = Commands->Get_Max_Health(obj);
		float NewMaxHP = MaxHP + HealthShield;
		float CurHP = Commands->Get_Health(obj);
		float NewHP = CurHP + HealthShield;

		Set_Max_Shield_Strength(obj, NewMaxAP);
		Commands->Set_Shield_Strength(obj, NewAP);
		Set_Max_Health(obj, NewMaxHP);
		Commands->Set_Health(obj, NewHP);

		if(strcmp(model,"") != 0)
			Set_Model_Custom(obj,model);
	
	}
}
void DB_Research_Armor_Model::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (!Upgraded && message == Research_Technology::UPGRADE_INFANTRY_ARMOR_CUSTOM && param==UpgradeID)
	{
		Upgraded=true;
		float HealthShield = Get_Float_Parameter("HealthShieldBonus");
		const char *model = Get_Parameter("Model");
		
		float MaxAP = Commands->Get_Max_Shield_Strength(obj);
		float NewMaxAP = MaxAP + HealthShield;
		float CurAP = Commands->Get_Shield_Strength(obj);
		float NewAP = CurAP + HealthShield;

		float MaxHP = Commands->Get_Max_Health(obj);
		float NewMaxHP = MaxHP + HealthShield;
		float CurHP = Commands->Get_Health(obj);
		float NewHP = CurHP + HealthShield;

		Set_Max_Shield_Strength(obj, NewMaxAP);
		Commands->Set_Shield_Strength(obj, NewAP);
		Set_Max_Health(obj, NewMaxHP);
		Commands->Set_Health(obj, NewHP);


		if(strcmp(model,"") != 0)
			Set_Model_Custom(obj,model);
		Create_2D_Wave_Sound_Dialog_Player(obj, Get_Parameter("Unit_Upgraded_Sound"));
		Send_Message_Player(obj,255,255,255,"Unit Upgraded!");
	}
}
ScriptRegistrant<DB_Research_Armor_Model> DB_Research_Armor_Model_Registrant("DB_Research_Armor_Model", "HealthShieldBonus:float,Model:string,Unit_Upgraded_Sound=bla:string,UpgradeID=1:int");


void DB_Research_Credit_Tick::Created(GameObject *obj)
{
	float TickRate = Get_Float_Parameter("TickRate");
	UpgradeID = Get_Int_Parameter("UpgradeID");
	Upgraded=false;
	Attach_Script_Once(obj,"DB_Research_Listener","");
	if(UpgradeComplete.Get(UpgradeID,0))
	{
		Upgraded=true;
		const char *model = Get_Parameter("Model");
		Attach_Script_V(obj,"dp88_buildingScripts_functionMoneyTrickle","%f",TickRate);
		if(strcmp(model,"") != 0)
			Set_Model_Custom(obj,model);
	}
}
void DB_Research_Credit_Tick::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (!Upgraded && message == Research_Technology::UPGRADE_INFANTRY_ARMOR_CUSTOM && param==UpgradeID)
	{
		Upgraded=true;
		float TickRate = Get_Float_Parameter("TickRate");
		const char *model = Get_Parameter("Model");
		Attach_Script_V(obj,"dp88_buildingScripts_functionMoneyTrickle","%f",TickRate);
		if(strcmp(model,"") != 0)
			Set_Model_Custom(obj,model);
		Create_2D_Wave_Sound_Dialog_Player(obj, Get_Parameter("Unit_Upgraded_Sound"));
		Send_Message_Player(obj,255,255,255,"Unit Upgraded!");

	}
}
ScriptRegistrant<DB_Research_Credit_Tick> DB_Research_Credit_Tick_Registrant("DB_Research_Credit_Tick", "TickRate:float,Model:string,Unit_Upgraded_Sound=bla:string,UpgradeID=1:int");

void DB_Tiberium_Field::Created(GameObject *obj)
{
	Zonetype = Get_Int_Parameter("Zonetype"); //0=green 1=blue
}
void DB_Tiberium_Field::Entered(GameObject *obj, GameObject *enterer)
{
	if (Is_Script_Attached(enterer,"DB_Tiberium_Harvester"))
	{
		if (Zonetype == 0) //green
			Commands->Send_Custom_Event(obj, enterer, CUSTOM_EVENT_GREENTIB_ENTERED, 0, 0);//tell harv a green zone entered 
		else if (Zonetype == 1) //blue
			Commands->Send_Custom_Event(obj, enterer, CUSTOM_EVENT_BLUETIB_ENTERED, 0, 0);//tell harv a blue zone entered
		else if (Zonetype == 2) //red
			Commands->Send_Custom_Event(obj, enterer, CUSTOM_EVENT_REDTIB_ENTERED, 0, 0);//tell harv a red zone entered
	}
}
void DB_Tiberium_Field::Exited(GameObject *obj, GameObject *exiter)
{
	if (Is_Script_Attached(exiter,"DB_Tiberium_Harvester"))
		Commands->Send_Custom_Event(obj, exiter, CUSTOM_EVENT_HARVY_LEFT_TIBFIELD, 0, 0);//tell harv left zone
}

void DB_Tiberium_Deposit::Entered(GameObject *obj, GameObject *enterer)
{
	if (Get_Object_Type(enterer) == Get_Int_Parameter("Team"))
	{
		if (Is_Script_Attached(enterer,"DB_Tiberium_Harvester"))
			Commands->Send_Custom_Event(obj,enterer, CUSTOM_EVENT_HARVY_DEPOSITING, 0, 0);
	}
}

ScriptRegistrant<DB_Tiberium_Field> DB_Tiberium_Field_Registrant("DB_Tiberium_Field","Zonetype=0:int");
ScriptRegistrant<DB_Tiberium_Deposit> DB_Tiberium_Deposit_Registrant("DB_Tiberium_Deposit","Team=0:int");

void DB_DeployableTank_Upgradeable::Created(GameObject *obj)
{
	//mode: 0=walk;1=deploy;2=deployed;3=redeploy
	mode = 0;
	PilotID = 0;
	model = Get_Parameter("Model_Name");
	UpgradeID = Get_Int_Parameter("UpgradeID");
	Upgraded=false;
	if(!UpgradeID)
		UpgradeID=1;
	Attach_Script_Once(obj,"DB_Research_Listener","");
	if(UpgradeComplete.Get(UpgradeID,0))
	{
		model = Get_Parameter("Upgraded_Model_Name");
		Set_Model_Custom(obj,model);
		float MaxHP = Commands->Get_Max_Health(obj);
		float NewMaxHP = MaxHP + Get_Float_Parameter("Extra_Health_Amount");
		float CurHP = Commands->Get_Health(obj);
		float NewHP = CurHP + Get_Float_Parameter("Extra_Health_Amount");
		float MaxAP = Commands->Get_Max_Shield_Strength(obj);
		float NewMaxAP = MaxAP + Get_Float_Parameter("Extra_Armor_Amount");
		float CurAP = Commands->Get_Shield_Strength(obj);
		float NewAP = CurAP + Get_Float_Parameter("Extra_Armor_Amount");
		Set_Max_Health(obj, NewMaxHP);
		Commands->Set_Health(obj, NewHP);
		Set_Max_Shield_Strength(obj, NewMaxAP);
		Commands->Set_Shield_Strength(obj, NewAP);
	}
}

void DB_DeployableTank_Upgradeable::KeyHook()
{
	Commands->Send_Custom_Event(Owner(),Owner(),923572385,0,0);
}

void DB_DeployableTank_Upgradeable::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (!Upgraded &&  type == Research_Technology::UPGRADE_INFANTRY_ARMOR_CUSTOM && param==UpgradeID)
	{
		Upgraded=true;
		model = Get_Parameter("Upgraded_Model_Name");
		if(mode == 0)
		{
			Set_Model_Custom(obj,model);
		}
		else if(mode == 1)
		{
			char deployedmodel[512];
			sprintf(deployedmodel,"%s_d",model);
			float frame = Get_Animation_Frame(obj);
			char deployanim[512];
			Set_Model_Custom(obj,deployedmodel);
			sprintf(deployanim,"%s_d.%s_d",model,model);
			Commands->Set_Animation(obj,deployanim,false,0,frame,-1,0);
		}
		else if(mode == 2)
		{
			char deployedmodel[512];
			sprintf(deployedmodel,"%s_dd",model);
			Set_Model_Custom(obj,deployedmodel);
		}
		else if(mode == 3)
		{
			char deployedmodel[512];
			sprintf(deployedmodel,"%s_d",model);
			float frame = Get_Animation_Frame(obj);
			char deployanim[512];
			Set_Model_Custom(obj,deployedmodel);
			sprintf(deployanim,"%s_d.%s_d",model,model);
			Commands->Set_Animation(obj,deployanim,false,0,frame,0,0);
		}
		float MaxHP = Commands->Get_Max_Health(obj);
		float NewMaxHP = MaxHP + Get_Float_Parameter("Extra_Health_Amount");
		float CurHP = Commands->Get_Health(obj);
		float NewHP = CurHP + Get_Float_Parameter("Extra_Health_Amount");
		float MaxAP = Commands->Get_Max_Shield_Strength(obj);
		float NewMaxAP = MaxAP + Get_Float_Parameter("Extra_Armor_Amount");
		float CurAP = Commands->Get_Shield_Strength(obj);
		float NewAP = CurAP + Get_Float_Parameter("Extra_Armor_Amount");
		Set_Max_Health(obj, NewMaxHP);
		Commands->Set_Health(obj, NewHP);
		Set_Max_Shield_Strength(obj, NewMaxAP);
		Commands->Set_Shield_Strength(obj, NewAP);
		Create_2D_Wave_Sound_Dialog_Player(obj, Get_Parameter("Unit_Upgraded_Sound"));
		Send_Message_Player(obj,255,255,255,"Unit Upgraded!");
	}
	else if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!PilotID)
		{
			PilotID = Commands->Get_ID(sender);
			char params[50];
			sprintf(params,"IKDeploy,%d,923572385",Commands->Get_ID(obj));
			Commands->Attach_Script(sender,"Reborn_Deployable_Vehicle_Player",params);
		}
	}
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (PilotID == Commands->Get_ID(sender))
		{
			PilotID = 0;
			Remove_Script(sender,"Reborn_Deployable_Vehicle_Player");
		}
	}
	else if (type == 923572385)
	{
		if (!obj->As_VehicleGameObj()->Can_Drive())
		{
			return;
		}
		char deployanim[512];
		char dmodel[512];
		sprintf(deployanim,"%s_d.%s_d",model,model);
		sprintf(dmodel,"%s_d",model);
		//mode: 0=walk;1=deploy;2=deployed;3=redeploy
		if (!mode)
		{
			if (!obj->As_VehicleGameObj()->Is_Immovable())
			{
				Commands->Send_Custom_Event(obj,obj,DEPLOY_BEGIN_CUSTOM,0,0);
				obj->As_VehicleGameObj()->Set_Immovable(true);
				mode = 1;
				Set_Model_Custom(obj,dmodel);
				Commands->Set_Shield_Type(obj,Get_Parameter("Armor"));
				Commands->Set_Animation(obj,deployanim,false,0,0,-1,0);
				Commands->Clear_Weapons(obj);
				Commands->Give_PowerUp(obj,Get_Parameter("Weapon_Powerup_Name"),false);
				Commands->Select_Weapon(obj,Get_Parameter("Weapon_Name"));
				//Commands->Control_Enable(sender,false);
				obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(false);
				Commands->Create_Sound(Get_Parameter("DeploySound"),Commands->Get_Position(obj),obj);
				Commands->Enable_Vehicle_Transitions(obj,false);
			}
		}
		//mode: 0=walk;1=deploy;2=deployed;3=redeploy
		else if (mode == 2)
		{
			Commands->Send_Custom_Event(obj,obj,UNDEPLOY_BEGIN_CUSTOM,0,0);
			mode = 3;
			Set_Model_Custom(obj,dmodel);
			Commands->Set_Animation_Frame(obj,deployanim,(int)Get_Float_Parameter("Last_Deploy_Frame"));
			Update_Network_Object(obj);
			Commands->Set_Animation(obj,deployanim,false,0,Get_Float_Parameter("Last_Deploy_Frame"),0,0);
			Commands->Clear_Weapons(obj);
			Commands->Give_PowerUp(obj,Get_Parameter("Weapon_Powerup_Name2"),false);
			Commands->Select_Weapon(obj,Get_Parameter("Weapon_Name2"));
			//Commands->Control_Enable(sender,false);
			obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(false);
			Commands->Create_Sound(Get_Parameter("UndeploySound"),Commands->Get_Position(obj),obj);
			Commands->Enable_Vehicle_Transitions(obj,false);
		}
	}
}

void DB_DeployableTank_Upgradeable::Animation_Complete(GameObject *obj,const char *animation_name)
{
	char deployedmodel[512];
	sprintf(deployedmodel,"%s_dd",model);
	char deployanim[512];
	sprintf(deployanim,"%s_d.%s_d",model,model);
	//tank deploy
	//mode: 0=walk;1=deploy;2=deployed;3=redeploy
	if (!_stricmp(animation_name,deployanim) && (mode == 1))
	{
		mode = 2;
		Set_Model_Custom(obj,deployedmodel);
		obj->As_PhysicalGameObj()->Clear_Animation();
		Commands->Control_Enable(Commands->Find_Object(PilotID),true);
		obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(true);
		Commands->Enable_Engine(obj,false);
		Commands->Enable_Vehicle_Transitions(obj,true);
		Commands->Enable_Innate_Conversations(obj,true);
		Commands->Send_Custom_Event(obj,obj,DEPLOY_COMPLETE_CUSTOM,0,0);
		Commands->Send_Custom_Event(obj,obj,CUSTOM_DEPLOY_STATE_CHANGED,1,0);
		return;
	}
	//tank redeploy
	//mode: 0=walk;1=deploy;2=deployed;3=redeploy
	if (!_stricmp(animation_name,deployanim) && (mode == 3))
	{
		obj->As_VehicleGameObj()->Set_Immovable(false);
		mode = 0;
		Set_Model_Custom(obj,model);
		obj->As_PhysicalGameObj()->Clear_Animation();
		Commands->Set_Shield_Type(obj,Get_Parameter("Armor2"));
		Commands->Control_Enable(Commands->Find_Object(PilotID),true);
		obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(true);
		Commands->Enable_Hibernation(obj,false);
		Commands->Enable_Innate_Conversations(obj,true);
		Commands->Enable_Vehicle_Transitions(obj,true);
		Commands->Enable_Engine(obj,true);
		Commands->Send_Custom_Event(obj,obj,UNDEPLOY_COMPLETE_CUSTOM,0,0);
		Commands->Send_Custom_Event(obj,obj,CUSTOM_DEPLOY_STATE_CHANGED,0,0);
		return;
	}
}

void DB_DeployableTank_Upgradeable::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&PilotID,4,6);
	Auto_Save_Variable(&mode,4,7);
	Auto_Save_Variable(&Upgraded,1,8);
	Auto_Save_Variable(&model,16,9);
}

void DB_DeployableTank_Upgradeable::Destroyed(GameObject *obj)
{
	Remove_Script(Commands->Find_Object(PilotID),"Reborn_Deployable_Vehicle_Player");
	Commands->Control_Enable(Commands->Find_Object(PilotID),true);
}

ScriptRegistrant<DB_DeployableTank_Upgradeable> DB_DeployableTank_Upgradeable_Registrant("DB_DeployableTank_Upgradeable","Model_Name=v_nod_art:string,Last_Deploy_Frame=0.00:float,Weapon_Name:string,Weapon_Powerup_Name:string,Armor:string,Weapon_Name2:string,Weapon_Powerup_Name2:string,Armor2:string,DeploySound:string,UndeploySound:string,Extra_Health_Amount=0:float,Extra_Armor_Amount=0:float,Upgraded_Model_Name=v_nod_art:string,UpgradeID=1:int");