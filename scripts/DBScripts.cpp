#include "General.h"
#include "engine_tt.h"
#include "engine_def.h"
#include "VehicleGameObjDef.h"
#include "BeaconGameObjDef.h"
#include "TeamPurchaseSettingsDefClass.h"
#include "PurchaseSettingsDefClass.h"
#include "PowerupGameObj.h"
#include "PowerupGameObjDef.h"
#include "SoldierGameObj.h"
#include "VehicleGameObj.h"
#include "BuildingGameObj.h"
#include "CommandLineParser.h"
#include "WeaponClass.h"
#include "BeaconGameObj.h"
#include "weaponmgr.h"
#include "GameObjManager.h"
#include "MoveablePhysClass.h"
#include "DB_General.h"

#include "engine_tdb.h"
#include "engine_obj.h"
#include "engine_script.h"
#include "engine_player.h"
#include "engine_phys.h"
#include "engine_game.h"
#include "engine_obj2.h"
#include "engine_dmg.h"

class DB_Captureable_Silo : public ScriptImpClass {
	bool Play_Damage;
	bool Play_Killed;
	bool Upgraded;
	const char *Building;
	float amount;
	int UpgradeID;
void Created(GameObject *obj)
{
	Upgraded=false;
	Building = Get_Translated_Preset_Name(obj);
	Play_Damage = false;
	Play_Killed = false;
	Set_Object_Type(obj, Get_Int_Parameter("Team"));
	
	amount = Get_Float_Parameter("Amount");
	
	UpgradeID=Get_Int_Parameter("UpgradeID");

	if(!UpgradeID)
		UpgradeID=1;

	Attach_Script_Once(obj,"DB_Research_Listener","");
	if(UpgradeComplete.Exists(UpgradeID))
	{
		amount = Get_Float_Parameter("UpgradeAmount");
		Upgraded=true;
	}

	if(Get_Object_Type(obj)!=-2)
	{
		Attach_Script_Once_V(obj,"dp88_buildingScripts_functionMoneyTrickle","%f",amount);
		Play_Killed = true;
		Play_Damage = true;
	}
}

	void Custom(GameObject *obj, int message, int param, GameObject *sender)
	{
		if (!Upgraded &&  message == Research_Technology::UPGRADE_INFANTRY_ARMOR_CUSTOM && param==UpgradeID)
		{
			Upgraded=true;
			amount=Get_Float_Parameter("UpgradeAmount");
			if(Is_Script_Attached(obj,"dp88_buildingScripts_functionMoneyTrickle"))
			{
				Remove_Script(obj,"dp88_buildingScripts_functionMoneyTrickle");
				Attach_Script_V(obj,"dp88_buildingScripts_functionMoneyTrickle","%f",amount);
			}

		}
	}

	void Detach(GameObject *obj)
	{
		if (Exe != 4)
		{
			delete[] this->Building;
			this->Building = 0;
		}
	}

	void Damaged(GameObject *obj, GameObject *damager, float damage)
	{

		if (damage < 0.0f)
		{
			if (Get_Object_Type(obj) == -2)
			{
				if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj))
				{
					Set_Object_Type(obj,Get_Object_Type(damager));
					if (Get_Object_Type(obj) == 0)
					{
						Send_Message_Team(0,255,0,0,StringClass::getFormattedString("Nod has captured the %s!", Building));
							Attach_Script_Once_V(obj,"dp88_buildingScripts_functionMoneyTrickle","%f",amount);
					}
					else if (Get_Object_Type(obj) == 1)
					{
						Send_Message_Team(1,255,201,0,StringClass::getFormattedString("GDI has captured the %s!", Building));
							Attach_Script_Once_V(obj,"dp88_buildingScripts_functionMoneyTrickle","%f",amount);
					}
					Play_Killed = true;
					Play_Damage = true;
				}
			}
		}

		if (damage > Commands->Get_Health(obj))
		{
			if (Get_Object_Type(obj) == 0)
			{
				if (Play_Killed)
				{
					Send_Message(255,255,255,StringClass::getFormattedString("The %s was destroyed by %s (%s)", Building, Get_Player_Name(damager), Get_Team_Name(Get_Object_Type(damager))));
					Play_Killed = false;
					Play_Damage = false;
					Remove_Script(obj,"dp88_buildingScripts_functionMoneyTrickle");
				}
			}
			else if (Get_Object_Type(obj) == 1)
			{
				if (Play_Killed)
				{
					Send_Message(255,255,255,StringClass::getFormattedString("The %s was destroyed by %s (%s)", Building, Get_Player_Name(damager), Get_Team_Name(Get_Object_Type(damager))));
					Play_Killed = false;
					Play_Damage = false;
					Remove_Script(obj,"dp88_buildingScripts_functionMoneyTrickle");
				}
			}
			Set_Object_Type(obj,-2);
			Commands->Set_Health(obj,1.0f);
		}
	}
};

class DB_Helipad_Captureable : public ScriptImpClass {
	bool Play_Damage;
	bool Play_Killed;
	const char *Building;
void Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,5.0f,333);
	Building = Get_Translated_Preset_Name(obj);
	Play_Damage = false;
	Play_Killed = false;
	Commands->Set_Animation_Frame(obj,Get_Model(obj),0);
	Set_Object_Type(obj, Get_Int_Parameter("Team"));

	if(Get_Object_Type(obj)!=-2)
	{
		Play_Killed = true;
		Play_Damage = true;
	}

	if(!obj->As_VehicleGameObj())
		Attach_Script_Once(obj,"SH_CustomCollisionGroup","15");
}

	void Custom(GameObject* obj, int type, int param, GameObject* sender)
	{
		if(type == 826455)
		{
			if(!Has_Timer(obj,this,123))
			{
				if(param == 0)
				{
						Commands->Set_Animation_Frame(obj,Get_Model(obj),2);
				}
				else if(param == 1)
				{
						Commands->Set_Animation_Frame(obj,Get_Model(obj),4);
				}
			}
			else
				Stop_Timer2(obj,this,123);
			Commands->Start_Timer(obj,this,2.1f,123);
		}
	}

	void Timer_Expired(GameObject *obj, int number)
	{
		if(number == 123)
		{
			if(Get_Object_Type(obj) == 0)
				Commands->Set_Animation_Frame(obj,Get_Model(obj),1);
			else if(Get_Object_Type(obj) == 1)
				Commands->Set_Animation_Frame(obj,Get_Model(obj),3);
			else
				Commands->Set_Animation_Frame(obj,Get_Model(obj),0);
		}
	}

	void Damaged(GameObject *obj, GameObject *damager, float damage)
	{
		if (damage > Commands->Get_Health(obj))
		{
			if (Get_Object_Type(obj) == 0)
			{
				if (Play_Killed)
				{
					Send_Message(255,255,255,StringClass::getFormattedString("The %s was destroyed by %s (%s)", Building,  Get_Player_Name(damager), Get_Team_Name(Get_Object_Type(damager))));
					Play_Killed = false;
					Play_Damage = false;
					Commands->Set_Animation_Frame(obj,Get_Model(obj),0);
				}
			}
			else if (Get_Object_Type(obj) == 1)
			{
				if (Play_Killed)
				{
					Send_Message(255,255,255,StringClass::getFormattedString("The %s was destroyed by %s (%s)", Building, Get_Player_Name(damager), Get_Team_Name(Get_Object_Type(damager))));
					Play_Killed = false;
					Play_Damage = false;
					Commands->Set_Animation_Frame(obj,Get_Model(obj),0);
				}
			}
			Set_Object_Type(obj,-2);
			Commands->Set_Health(obj,1.0f);

		}

		if (damage < 0.0f)
		{
			if (Get_Object_Type(obj) == -2)
			{
				if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj))
				{
					Set_Object_Type(obj,Get_Object_Type(damager));
					if (Get_Object_Type(obj) == 0)
					{
						Send_Message_Team(0,255,0,0,StringClass::getFormattedString("Nod has captured the %s!",Building));
						Commands->Set_Animation_Frame(obj,Get_Model(obj),1);
					}
					else if (Get_Object_Type(obj) == 1)
					{
						Send_Message_Team(1,255,201,0,StringClass::getFormattedString("GDI has captured the %s!",Building));
						Commands->Set_Animation_Frame(obj,Get_Model(obj),3);
					}
					Play_Killed = true;
					Play_Damage = true;
				}
			}
		}
	}
	
	void Detach(GameObject *obj)
	{
		if (Exe != 4)
		{
			delete[] this->Building;
			this->Building = 0;
		}
	}
};


class DB_Capturable_repairpad : public ScriptImpClass {
	bool Play_Damage;
	bool Play_Killed;
	const char *Building;
	int zoneid;
void Created(GameObject *obj)
{
	zoneid = Get_Int_Parameter("RepairZoneID");
	Commands->Start_Timer(obj,this,5.0f,333);
	Play_Damage = false;
	Play_Killed = false;
	Commands->Set_Animation_Frame(obj,Get_Model(obj),0);
	Set_Object_Type(obj, Get_Int_Parameter("Team"));
	if(Get_Object_Type(obj)!=-2)
	{
		Commands->Set_Health(obj, Commands->Get_Max_Health(obj));
		Commands->Set_Shield_Strength(obj, Commands->Get_Max_Shield_Strength(obj));
		Play_Killed = true;
		Play_Damage = true;
		Attach_Script_Once_V(Commands->Find_Object(zoneid), "JFW_Sell_Zone", "%i",Get_Object_Type(obj));
	}


	if(!obj->As_VehicleGameObj())
		Attach_Script_Once(obj,"SH_CustomCollisionGroup","15");

	
	Building = Get_Translated_Preset_Name(obj);
}

	void Custom(GameObject* obj, int type, int param, GameObject* sender)
	{
		if(type == 826455)
		{
			if(!Has_Timer(obj,this,123))
			{
				if(param == 0)
				{
						Commands->Set_Animation_Frame(obj,Get_Model(obj),2);
				}
				else if(param == 1)
				{
						Commands->Set_Animation_Frame(obj,Get_Model(obj),4);
				}
			}
			else
				Stop_Timer2(obj,this,123);
			Commands->Start_Timer(obj,this,1.1f,123);
		}
	}

	void Timer_Expired(GameObject *obj, int number)
	{
		if(number == 123)
		{
			if(Get_Object_Type(obj) == 0)
				Commands->Set_Animation_Frame(obj,Get_Model(obj),1);
			else if(Get_Object_Type(obj) == 1)
				Commands->Set_Animation_Frame(obj,Get_Model(obj),3);
			else
				Commands->Set_Animation_Frame(obj,Get_Model(obj),0);
		}
	}

	void Damaged(GameObject *obj, GameObject *damager, float damage)
	{
		if (damage > Commands->Get_Health(obj))
		{
			if (Get_Object_Type(obj) == 0)
			{
				if (Play_Killed)
				{
					Send_Message(255,255,255,StringClass::getFormattedString("The %s was destroyed by %s (%s)", Building, Get_Player_Name(damager), Get_Team_Name(Get_Object_Type(damager))));
					Play_Killed = false;
					Play_Damage = false;
					Remove_Script(Commands->Find_Object(zoneid), "JFW_Sell_Zone");
					Commands->Set_Animation_Frame(obj,Get_Model(obj),0);
				}
			}
			else if (Get_Object_Type(obj) == 1)
			{
				if (Play_Killed)
				{
					Send_Message(255,255,255,StringClass::getFormattedString("The %s was destroyed by %s (%s)", Building, Get_Player_Name(damager), Get_Team_Name(Get_Object_Type(damager))));
					Play_Killed = false;
					Play_Damage = false;
					Remove_Script(Commands->Find_Object(zoneid), "JFW_Sell_Zone");
					Commands->Set_Animation_Frame(obj,Get_Model(obj),0);
				}
			}
			Set_Object_Type(obj,-2);
			Commands->Set_Health(obj,1.0f);

		}

		if (damage < 0.0f)
		{
			if (Get_Object_Type(obj) == -2)
			{
				if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj))
				{
					Set_Object_Type(obj,Get_Object_Type(damager));
					if (Get_Object_Type(obj) == 0)
					{
						Send_Message_Team(0,255,0,0,StringClass::getFormattedString("Nod has captured the %s!", Building));
						Attach_Script_Once(Commands->Find_Object(zoneid), "JFW_Sell_Zone", "0");
						Commands->Set_Animation_Frame(obj,Get_Model(obj),1);
					}
					else if (Get_Object_Type(obj) == 1)
					{
						Send_Message_Team(1,255,201,0,StringClass::getFormattedString("GDI has captured the %s!", Building));
						Attach_Script_Once(Commands->Find_Object(zoneid), "JFW_Sell_Zone", "1");
						Commands->Set_Animation_Frame(obj,Get_Model(obj),3);
					}
					Play_Killed = true;
					Play_Damage = true;
				}
			}
		}
	}

	
	void Detach(GameObject *obj)
	{
		if (Exe != 4)
		{
			delete[] this->Building;
			this->Building = 0;
		}
	}
};

class DB_Capturable_object : public ScriptImpClass {
	bool Play_Damage;
	bool Play_Killed;
	const char *Building;
void Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,5.0f,333);
	Set_Object_Type(obj, Get_Int_Parameter("Owner"));
	Building = Get_Translated_Preset_Name(obj);
	Play_Damage = false;
	Play_Killed = false;

	if(Get_Object_Type(obj)!=-2)
	{
		Play_Killed = true;
		Play_Damage = true;
	}

	if(!obj->As_VehicleGameObj())
		Attach_Script_Once(obj,"SH_CustomCollisionGroup","15");
}
	void Damaged(GameObject *obj, GameObject *damager, float damage)
	{
		if (damage > Commands->Get_Health(obj))
		{
			if (Get_Object_Type(obj) == 0)
			{
				if (Play_Killed)
				{
					if(Commands->Is_A_Star(damager))
					{
						Send_Message(255,255,255,StringClass::getFormattedString("The Nod %s was destroyed by %s (%s)", Building, Get_Player_Name(damager), Get_Team_Name(Get_Object_Type(damager))));
						//DALogManager::Write_Log("_INFO","The Nod %s was destroyed by %s (%s)", Building, Get_Player_Name(damager), Get_Team_Name(Get_Object_Type(damager)));
					}
					else
					{
						Send_Message(255,255,255,StringClass::getFormattedString("The Nod %s was destroyed by a %s (%s)", Building, Get_Translated_Preset_Name(damager), Get_Team_Name(Get_Object_Type(damager))));
						//DALogManager::Write_Log("_INFO","The Nod %s was destroyed by a %s", Building, DATranslationManager::Translate_With_Team_Name(damager));
					}
					Play_Killed = false;
					Play_Damage = false;
					Commands->Action_Reset(obj,200);
				}
			}
			else if (Get_Object_Type(obj) == 1)
			{
				if (Play_Killed)
				{
					if(Commands->Is_A_Star(damager))
					{
						Send_Message(255,255,255,StringClass::getFormattedString("The GDI %s was destroyed by %s (%s)", Building, Get_Player_Name(damager), Get_Team_Name(Get_Object_Type(damager))));
						//DALogManager::Write_Log("_INFO","The GDI %s was destroyed by %s (%s)", Building, Get_Player_Name(damager), Get_Team_Name(Get_Object_Type(damager)));
					}
					else
					{
						Send_Message(255,255,255,StringClass::getFormattedString("The GDI %s was destroyed by a %s (%s)", Building, Get_Translated_Preset_Name(damager), Get_Team_Name(Get_Object_Type(damager))));
						//DALogManager::Write_Log("_INFO","The GDI %s was destroyed by a %s", Building, DATranslationManager::Translate_With_Team_Name(damager));
					}
					Play_Killed = false;
					Play_Damage = false;
					Commands->Action_Reset(obj,200);
				}
			}
			Set_Object_Type(obj,-2);
			Commands->Set_Health(obj,1.0f);

		}

		if (damage < 0.0f)
		{
			if (Get_Object_Type(obj) == -2)
			{
				if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj))
				{
					Set_Object_Type(obj,Get_Object_Type(damager));
					if (Get_Object_Type(obj) == 0)
					{
						Send_Message_Team(0,255,0,0,StringClass::getFormattedString("Nod has captured the %s!",Building));
						//DALogManager::Write_Log("_INFO","Nod has captured the %s!",Building);
					}
					else if (Get_Object_Type(obj) == 1)
					{
						Send_Message_Team(1,255,201,0,StringClass::getFormattedString("GDI has captured the %s!",Building));
						//DALogManager::Write_Log("_INFO","GDI has captured the %s!",Building);
					}
					Play_Killed = true;
					Play_Damage = true;
				}
			}
		}
	}

	
	void Detach(GameObject *obj)
	{
		if (Exe != 4)
		{
			delete[] this->Building;
			this->Building = 0;
		}
	}
};


class DB_Powerup_Buy_Poke_Sound : public ScriptImpClass {
	bool Do_Poke_Stuff;
void DB_Powerup_Buy_Poke_Sound::Created(GameObject *obj)
{
	Do_Poke_Stuff = true; 
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
}
void DB_Powerup_Buy_Poke_Sound::Poked(GameObject *obj,GameObject *poker)
{
	if (Do_Poke_Stuff)
	{
		Do_Poke_Stuff = false; Commands->Start_Timer(obj,this,5.0f,12345);
		if (!Is_Building_Dead(Commands->Find_Object(Get_Int_Parameter("BuildingID"))))
		{
			const char *preset;
			int x;
			int cost;
			x = Get_Int_Parameter("Team");
			if (Get_Object_Type(poker) != x)
			{
				Send_Message_Player(poker,255,255,255,"Access Denied!");
				Create_2D_WAV_Sound_Player(poker,Get_Parameter("SoundDenied"));
				return;
			}
			preset = Get_Parameter("Preset");
			cost = Get_Int_Parameter("Cost");
			if (cost <= Commands->Get_Money(poker))
			{
				cost = -cost;
				Commands->Give_Money(poker,(float)cost,0);
				Grant_Powerup(poker,preset);
				Create_2D_WAV_Sound_Player(poker,Get_Parameter("SoundGranted"));
				Send_Message_Player(poker,255,255,255,"Purchase request granted!");
			}
			else
			{
				Create_2D_WAV_Sound_Player(poker,Get_Parameter("SoundDenied"));
				Send_Message_Player(poker,255,255,255,"Access Denied! Insufficient Funds!");
			}
		}
		else
		{
			const char *Building = Get_Translated_Preset_Name(Commands->Find_Object(Get_Int_Parameter("BuildingID")));
			Send_Message_Player(poker,255,255,255,StringClass::getFormattedString("Sorry the %s is dead. Purchase denied.",Building));
			Create_2D_WAV_Sound_Player(poker,Get_Parameter("SoundDenied"));
		}
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
	}
}
void DB_Powerup_Buy_Poke_Sound::Timer_Expired(GameObject *obj, int number)
{
	if (number == 12345)
	{
		Do_Poke_Stuff = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}
};


void  PlayInsufficientFunds(GameObject *obj)
{
	if(Get_Player_Type(obj)==0)
		Create_2D_WAV_Sound_Player (obj, "m00evan_dsgn0024i1evan_snd.wav");
	if(Get_Player_Type(obj)==1)
		Create_2D_WAV_Sound_Player (obj, "m00evag_dsgn0028i1evag_snd.wav");
}

class DB_capturable_Helipad_Terminal : public ScriptImpClass {
	bool Do_Poke_Stuff;
	ReferencerClass building;
void  DB_capturable_Helipad_Terminal::Created(GameObject *obj)
{
	Do_Poke_Stuff = true; 
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	Commands->Set_Animation_Frame(obj,Get_Model(obj),0);
	Commands->Start_Timer(obj,this,0.5f,1);
	building = Commands->Find_Object(Get_Int_Parameter("BuildingID"));
}
void  DB_capturable_Helipad_Terminal::Poked(GameObject *obj,GameObject *poker)
{
	if (Do_Poke_Stuff)
	{
		Do_Poke_Stuff = false; Commands->Start_Timer(obj,this,5.0f,12345);
		if (Get_Object_Type(building)==Get_Object_Type(poker))
		{
			const char *preset;
			float cost;
			int spawnlocation;
			Vector3 location;
			bool powered=true;
			cost=Get_Float_Parameter("Cost");
			preset = Get_Parameter("Preset");
			spawnlocation = Get_Int_Parameter("SpawnLocation");
			location = Commands->Get_Position(Commands->Find_Object(spawnlocation));
			if(!Is_Base_Powered(Get_Object_Type(poker)))
			{
				powered=false;
				cost=cost*1.5f;
			}
			if (cost <= Commands->Get_Money(poker))
			{
				cost = -cost;
				Commands->Give_Money(poker,(float)cost,0);
				GameObject *Purchase = Commands->Create_Object(preset, location);
				if(Purchase)
					Commands->Set_Facing(Purchase, Commands->Get_Facing(Commands->Find_Object(spawnlocation)));
				if(Purchase && Purchase->As_VehicleGameObj())
				{
					Set_Object_Type(Purchase,Get_Object_Type(poker));
					Purchase->As_VehicleGameObj()->Lock_Vehicle(poker,45.0f);
				}
				Send_Message_Team(Get_Object_Type(poker),160,160,255,StringClass::getFormattedString("Building: %s - %s",Get_Translated_Definition_Name(preset),Get_Player_Name(poker)));
				Send_Message_Player(poker,178,178,178,"Purchase request granted!");
				Stop_Timer2(obj,this,1);
				DB_capturable_Helipad_Terminal::Timer_Expired(obj, 1);
			}
			else
			{
				PlayInsufficientFunds(poker);
				if(!powered)
					Send_Message_Player(poker,255,255,255,"Access Denied! Insufficient Funds! (Power Down - Cost x 1.5)");
				else
					Send_Message_Player(poker,255,255,255,"Access Denied! Insufficient Funds!");
			}
		}
		else
		{
			const char *Building = Get_Translated_Preset_Name(building);
			Send_Message_Player(poker,255,255,255,StringClass::getFormattedString("Sorry, the %s is not captured. Purchase denied.",Building));

		}
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
	}
}
void  DB_capturable_Helipad_Terminal::Timer_Expired(GameObject *obj, int number)
{
	if(number == 1)
	{
		int team = Get_Object_Type(building);
		int frame = 0;
		if(team == 1)
		{
			if(Do_Poke_Stuff)
			{
				if(Is_Base_Powered(1))
					frame=7;
				else
					frame=5;
			}
			else
			{
				if(Is_Base_Powered(1))
					frame=8;
				else
					frame=6;
			}
			if(Get_Object_Type(obj) != -2)
			{
				Set_Object_Type(obj,1);
			}
		}
		else if (team == 0)
		{
			if(Do_Poke_Stuff)
			{
				if(Is_Base_Powered(0))
					frame=3;
				else
					frame=1;
			}
			else
			{
				if(Is_Base_Powered(0))
					frame=4;
				else
					frame=2;
			}
			if(Get_Object_Type(obj) != 0)
			{
				Set_Object_Type(obj,0);
			}
		}
		else
		{
			frame=0;
			if(Get_Object_Type(obj) != -2)
			{
				Set_Object_Type(obj,-2);
			}
		}
		if(Get_Animation_Frame(obj) != frame)
		{
			Commands->Set_Animation_Frame(obj,Get_Model(obj),frame);
		}
		Commands->Start_Timer(obj,this,0.5f,1);
	}

	else if (number == 12345)
	{
		Do_Poke_Stuff = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}


}
};

class DB_capturable_WF_Terminal : public ScriptImpClass {
	bool Do_Poke_Stuff;
void  DB_capturable_WF_Terminal::Created(GameObject *obj)
{
	Do_Poke_Stuff = true; 
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
}
void  DB_capturable_WF_Terminal::Poked(GameObject *obj,GameObject *poker)
{
	if (Do_Poke_Stuff)
	{
		Do_Poke_Stuff = false; Commands->Start_Timer(obj,this,5.0f,12345);
		if (Get_Object_Type(Commands->Find_Object(Get_Int_Parameter("BuildingID")))==Get_Object_Type(poker))
		{
			const char *preset;
			int cost;
			int spawnlocation;
			Vector3 location;
			cost=Get_Int_Parameter("Cost");
			preset = Get_Parameter("Preset");
			spawnlocation = Get_Int_Parameter("SpawnLocation");
			location = Commands->Get_Position(Commands->Find_Object(spawnlocation));
			if (cost <= Commands->Get_Money(poker))
			{
				cost = -cost;
				Commands->Give_Money(poker,(float)cost,0);
				GameObject *vehicle = Commands->Create_Object(preset, location);
				if(vehicle)
				Commands->Set_Facing(vehicle, Commands->Get_Facing(Commands->Find_Object(spawnlocation)));

				if(vehicle->As_VehicleGameObj())
				{
					Set_Object_Type(vehicle,Get_Object_Type(poker));
					vehicle->As_VehicleGameObj()->Lock_Vehicle(poker,45.0f);
					//if(!vehicle->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass()->Can_Teleport(vehicle->As_PhysicalGameObj()->Get_Transform()))
						//Fix_Stuck_Object(vehicle->As_VehicleGameObj(),15);
				}
				Send_Message_Player(poker,255,255,255,"Purchase request granted!");
			}
			else
			{
				PlayInsufficientFunds(poker);
				Send_Message_Player(poker,255,255,255,"Access Denied! Insufficient Funds!");
			}
		}
		else
		{
			const char *Building = Get_Translated_Preset_Name(Commands->Find_Object(Get_Int_Parameter("BuildingID")));
			Send_Message_Player(poker,255,255,255,StringClass::getFormattedString("Sorry the %s is not captured. Purchase denied.",Building));
		}
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
	}
}
void  DB_capturable_WF_Terminal::Timer_Expired(GameObject *obj, int number)
{
	if (number == 12345)
	{
		Do_Poke_Stuff = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}
};


class DB_capturable_helipadzone_reload : public ScriptImpClass {
	void DB_capturable_helipadzone_reload::Entered(GameObject *obj, GameObject *enterer)
{
	if(Commands->Find_Object(Get_Int_Parameter("BuildingID")))
		if (Get_Object_Type(enterer) == Get_Object_Type(Commands->Find_Object(Get_Int_Parameter("BuildingID"))) && enterer->As_VehicleGameObj() && Is_VTOL(enterer))
		{
			if(!Has_Timer(obj,this,enterer->Get_ID()))
				Commands->Start_Timer(obj, this, 1, Commands->Get_ID(enterer));
			Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("BuildingID")),826455,Get_Object_Type(enterer),0);
		}
}

void DB_capturable_helipadzone_reload::Timer_Expired(GameObject *obj, int number)
{
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if ((distance <= 10.0) && (Get_Vehicle_Driver(Commands->Find_Object(number)))) //reloads vehicle up to 10m from zone center
	{
		
		
		GameObject *Aircraft = Commands->Find_Object(number);
		if(Aircraft && Is_VTOL(Aircraft))
			{
				Commands->Give_PowerUp(Commands->Find_Object(number),Get_Parameter("ReloadPreset"),false);
				//WeaponClass *weapon = Aircraft->As_VehicleGameObj()->Get_Weapon_Bag()->Get_Weapon();
				if(Get_Vehicle_Driver(Aircraft))
				{
					Create_2D_WAV_Sound_Player(Get_Vehicle_Driver(Aircraft),"powerup_ammo.wav");
				}
				/*
				if(weapon)
				{
					weapon->Set_Clip_Rounds(weapon->Get_Definition()->ClipSize);
				}
				*/
				Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("BuildingID")),826455,Get_Object_Type(Aircraft),0);
			}

		Commands->Start_Timer(obj, this, 2, number);
	}
}


};


class DB_capturable_Repairzone : public ScriptImpClass {
void DB_capturable_Repairzone::Entered(GameObject *obj,GameObject *enterer)
{
	int Player_Type = Get_Object_Type(Commands->Find_Object(Get_Int_Parameter("BuildingID")));
	if (CheckPlayerType(enterer,Player_Type) || Commands->Find_Object(Get_Int_Parameter("BuildingID"))==enterer || Player_Type==-2)
	{
		return;
	}
	if (enterer->As_VehicleGameObj() && !enterer->As_VehicleGameObj()->Is_Turret())
	{
		if(Get_Vehicle_Driver(enterer))
		{
			if(Player_Type==0)
				Create_2D_WAV_Sound_Player(Get_Vehicle_Driver(enterer),"m00evan_dsgn0015i1evan_snd.wav");
			else
				Create_2D_WAV_Sound_Player(Get_Vehicle_Driver(enterer),"m00evag_dsgn0018i1evag_snd.wav");
			unsigned int Red = 0,Blue = 0,Green = 0;
			Get_Team_Color(Get_Object_Type(Get_Vehicle_Driver(enterer)),&Red,&Green,&Blue);
			Send_Message_Player(Get_Vehicle_Driver(enterer),Red,Green,Blue,"Repairing...");
		}
		if(Get_Hitpoints(enterer)!=Get_Max_Hitpoints(enterer))
			Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("BuildingID")),826455,Player_Type,0);
		if(!Has_Timer(obj,this,enterer->Get_ID()))
			Commands->Start_Timer(obj,this,1.0,Commands->Get_ID(enterer));
	}
}

void DB_capturable_Repairzone::Timer_Expired(GameObject *obj,int number)
{
		Vector3 pos1;
		Vector3 pos2;
		float distance;
		GameObject *repairunit = Commands->Find_Object(number);
		if(repairunit)
		{
			pos1 = Commands->Get_Position(obj);
			pos2 = Commands->Get_Position(repairunit);
			distance = Commands->Get_Distance(pos1,pos2);
			int Player_Type = Get_Object_Type(Commands->Find_Object(Get_Int_Parameter("BuildingID")));
			if (distance <= 10 && Player_Type!=-2)
			{
				if(Get_Hitpoints(repairunit)!=Get_Max_Hitpoints(repairunit))
					Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("BuildingID")),826455,Player_Type,0);	
				Commands->Apply_Damage(repairunit,-50,"None",NULL);
				Commands->Start_Timer(obj,this,1.0,number);
			}
		}
	}
};

class DB_Capturable_Infantry_Terminal : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	int InfantryFactoryID;
	int Team;
	bool Allowpoke;
	float Cost;
	const char *Preset;
};


void DB_Capturable_Infantry_Terminal::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	InfantryFactoryID = Get_Int_Parameter("InfantryFactoryID");
	Cost = Get_Float_Parameter("Cost");
	Preset = Get_Parameter("Preset");
	Team = Get_Object_Type(Commands->Find_Object(InfantryFactoryID));
	Allowpoke = true;
	Set_Object_Type(obj, Team);
	Commands->Start_Timer(obj, this, 10.0f, 102031);//set the team of the console to that of the associated helipad
}
void DB_Capturable_Infantry_Terminal::Poked(GameObject *obj, GameObject *poker)
{
	if (Allowpoke)
	{
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
		Team = Get_Object_Type(Commands->Find_Object(InfantryFactoryID));//request the helipad team again and check it against poker
		Allowpoke = false; Commands->Start_Timer(obj, this, 3.0f, 102030);//No poking for 5s
		if (Get_Object_Type(poker) == Team)
		{
			if (Is_Base_Powered(Team))
			{
				if (Commands->Get_Money(poker) >= Cost)
				{
					Commands->Give_Money(poker, -Cost, false);
					//DALogManager::Write_Log("_PURCHASE","%s - %s",Get_Player_Name(poker),DATranslationManager::Translate(Preset));
					Send_Message_Player(poker,255,255,255,"Purchase request granted!");
					Change_Character(poker, Preset);
				}
				else
				{
					if (Team == 0)
					{
						Create_2D_Wave_Sound_Dialog_Player(poker, "m00evan_dsgn0024i1evan_snd.wav");
					}
					else if (Team == 1)
					{
						Create_2D_Wave_Sound_Dialog_Player(poker, "m00evag_dsgn0028i1evag_snd.wav");
					}
					Send_Message_Player(poker, 255, 255, 255, "Insufficient Funds");
				}
			}
			else if (!Is_Base_Powered(Team))
			{
				if (Commands->Get_Money(poker) >= Cost * 1.5f)
				{
					Commands->Give_Money(poker, -Cost * 1.5f, false);
					//DALogManager::Write_Log("_PURCHASE","%s - %s",Get_Player_Name(poker),DATranslationManager::Translate(Preset));
					Send_Message_Player(poker,255,255,255,"Purchase request granted!");
					Change_Character(poker, Preset);
				}
				else
				{
					if (Team == 0)
					{
						Create_2D_Wave_Sound_Dialog_Player(poker, "m00evan_dsgn0024i1evan_snd.wav");
					}
					else if (Team == 1)
					{
						Create_2D_Wave_Sound_Dialog_Player(poker, "m00evag_dsgn0028i1evag_snd.wav");
					}
					Send_Message_Player(poker, 255, 255, 255, "Insufficient Funds: Power is Down = 1.5x Cost");
				}
			}
		}
		else
		{
			const char *Building = Get_Translated_Preset_Name(Commands->Find_Object(InfantryFactoryID));
			Send_Message_Player(poker,255,255,255,StringClass::getFormattedString("Sorry, the %s is not captured. Purchase denied.",Building));
		}
	}
}
void DB_Capturable_Infantry_Terminal::Timer_Expired(GameObject *obj, int number)
{
	if (number == 102030)
	{
		Allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
	else if (number == 102031)
	{
		Team = Get_Object_Type(Commands->Find_Object(InfantryFactoryID));
		Set_Object_Type(obj, Team);
		Commands->Start_Timer(obj, this, 10.0f, 102031);//re-set the team of the console to that of the associated helipad
	}
}
ScriptRegistrant<DB_Capturable_Infantry_Terminal> DB_Capturable_Infantry_Terminal_Registrant("DB_Capturable_Infantry_Terminal", "InfantryFactoryID=0:int,Preset=bla:string,Cost=10000:float");

class DB_Capturable_Powerup_Terminal : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	int CheckObjectID;
	int Team;
	bool Allowpoke;
	float Cost;
	const char *Preset;
};

void DB_Capturable_Powerup_Terminal::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	CheckObjectID = Get_Int_Parameter("CheckObjectID");
	Cost = Get_Float_Parameter("Cost");
	Preset = Get_Parameter("Powerup");
	Team = Get_Object_Type(Commands->Find_Object(CheckObjectID));
	Allowpoke = true;
	Set_Object_Type(obj, Team);
	Commands->Start_Timer(obj, this, 10.0f, 102031);//set the team of the console to that of the associated helipad
}
void DB_Capturable_Powerup_Terminal::Poked(GameObject *obj, GameObject *poker)
{
	if (Allowpoke)
	{
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
		Team = Get_Object_Type(Commands->Find_Object(CheckObjectID));//request the helipad team again and check it against poker
		Allowpoke = false; Commands->Start_Timer(obj, this, 3.0f, 102030);//No poking for 5s
		if (Get_Object_Type(poker) == Team)
		{
			if (Is_Base_Powered(Team))
			{
				if (Commands->Get_Money(poker) >= Cost)
				{
					Commands->Give_Money(poker, -Cost, false);
					Grant_Powerup(poker, Preset);
				}
				else
				{
					if (Team == 0)
					{
						Create_2D_WAV_Sound_Player(poker, "m00evan_dsgn0024i1evan_snd.wav");
					}
					else if (Team == 1)
					{
						Create_2D_WAV_Sound_Player(poker, "m00evag_dsgn0028i1evag_snd.wav");
					}
					Send_Message_Player(poker, 255, 255, 255, "Insufficient Funds");
				}
			}
			else if (!Is_Base_Powered(Team))
			{
				if (Commands->Get_Money(poker) >= Cost * 1.5f)
				{
					Commands->Give_Money(poker, -Cost * 1.5f, false);
					Grant_Powerup(poker, Preset);
				}
				else
				{
					if (Team == 0)
					{
						Create_2D_WAV_Sound_Player(poker, "m00evan_dsgn0024i1evan_snd.wav");
					}
					else if (Team == 1)
					{
						Create_2D_WAV_Sound_Player(poker, "m00evag_dsgn0028i1evag_snd.wav");
					}
					Send_Message_Player(poker, 255, 255, 255, "Insufficient Funds: Power is Down = Double Cost");
				}
			}
		}
		else
		{
			const char *Building = Get_Translated_Preset_Name(Commands->Find_Object(CheckObjectID));
			Send_Message_Player(poker,255,255,255,StringClass::getFormattedString("Sorry, the %s is not captured. Purchase denied.",Building));
		}
	}
}
void DB_Capturable_Powerup_Terminal::Timer_Expired(GameObject *obj, int number)
{
	if (number == 102030)
	{
		Allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
	else if (number == 102031)
	{
		Team = Get_Object_Type(Commands->Find_Object(CheckObjectID));
		Set_Object_Type(obj, Team);
		Commands->Start_Timer(obj, this, 10.0f, 102031);//re-set the team of the console to that of the associated helipad
	}
}

ScriptRegistrant<DB_Capturable_Powerup_Terminal> DB_Capturable_Powerup_Terminal_Registrant("DB_Capturable_Powerup_Terminal", "CheckObjectID=0:int,Powerup=bla:string,Cost=10000:float");


class Nod_Turret_DeathSound : public ScriptImpClass {
	int team;
	void Nod_Turret_DeathSound::Created(GameObject *obj)
	{
		team=Get_Object_Type(obj);
		
	}

	void Nod_Turret_DeathSound::Killed(GameObject *obj, GameObject *killer)
	{
		team=Get_Object_Type(obj);
		if(team==0)
			Create_2D_WAV_Sound_Team("m00bntu_kill0001i1evan_snd.wav",team);
		else if(team==1)
			Create_2D_WAV_Sound_Team("m00bgtu_kill0002i1evag_snd.wav",team);
	}

};


class GDI_Guard_Tower_DeathSound : public ScriptImpClass {
	int team;
	void  GDI_Guard_Tower_DeathSound::Created(GameObject *obj)
	{
		team=Get_Object_Type(obj);
		
	}

	void  GDI_Guard_Tower_DeathSound::Killed(GameObject *obj, GameObject *killer)
	{
		team=Get_Object_Type(obj);
		if(team==1)
			Create_2D_WAV_Sound_Team("m00bggt_kill0001i1evag_snd.wav",team);
		else if(team==0)
			Create_2D_WAV_Sound_Team("m00bngt_kill0001i1evan_snd.wav",team);
	}
};

class Nod_SamSite_DeathSound : public ScriptImpClass {
	int team;
	void Nod_SamSite_DeathSound::Created(GameObject *obj)
	{
		team=Get_Object_Type(obj);
		
	}

	void Nod_SamSite_DeathSound::Killed(GameObject *obj, GameObject *killer)
	{
		team=Get_Object_Type(obj);
		if(team==0)
			Create_2D_WAV_Sound_Team("m00bnss_kill0001i1evan_snd.wav",team);
		else if(team==1)
			Create_2D_WAV_Sound_Team("m00bgss_kill0002i1evag_snd.wav",team);
	}

};

class DB_Power_Plant_fix : public ScriptImpClass {
	int team;
	void DB_Power_Plant_fix::Created(GameObject *obj)
	{
		team=Get_Object_Type(obj);
		
	}

	void DB_Power_Plant_fix::Killed(GameObject *obj, GameObject *killer)
	{
			Commands->Start_Timer(obj,this,3,8);
	}

	void DB_Power_Plant_fix::Timer_Expired(GameObject *obj,int number)
	{
		if(obj->As_BuildingGameObj()->Get_Player_Type()==0)
		{
			if(!Is_Base_Powered(0))
			{
				Create_2D_WAV_Sound_Team("m00evag_dsgn0065i1evag_snd2.wav",1);
				Create_2D_WAV_Sound_Team("m00evan_dsgn0069i1evan_snd2.wav",0);
			}
		}
		else if(obj->As_BuildingGameObj()->Get_Player_Type()==1)
		{
			if(!Is_Base_Powered(1))
			{
				Create_2D_WAV_Sound_Team("m00evag_dsgn0064i1evag_snd2.wav",1);
				Create_2D_WAV_Sound_Team("m00evan_dsgn0068i1evan_snd2.wav",0);
			}
		}
	}
};

class DB_Beacon_fix : public ScriptImpClass {

	int beacontimerID;
	void DB_Beacon_fix::Created(GameObject *obj)
	{
		
		
	}

	void DB_Beacon_fix::Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if( type == CUSTOM_EVENT_BEACON_DEPLOYED && obj->As_PhysicalGameObj() && obj->As_PhysicalGameObj()->As_BeaconGameObj())
		{
			float det_time = obj->As_PhysicalGameObj()->As_BeaconGameObj()->Get_Detonate_Time();
			//Console_InputF("msg beacon deployed: %f seconds",det_time);
			Vector3 droploc = Commands->Get_Position(obj);
			GameObject *beacontimer = Commands->Create_Object("Invisible_Object",droploc);
			beacontimerID = beacontimer->Get_ID();
			Attach_Script_Once_V(beacontimer,"DB_Beacon_fix2","%i,%f",obj->Get_ID(),det_time);
		}
	}

	void DB_Beacon_fix::Destroyed(GameObject *obj)
	{
		if(beacontimerID)
			Commands->Destroy_Object(Commands->Find_Object(beacontimerID));
	}
};

class DB_Beacon_fix2 : public ScriptImpClass {

	int BeaconID;
	int CountDown;
	void DB_Beacon_fix2::Created(GameObject *obj)
	{
		float DetTime = Get_Float_Parameter("DetTime");
		BeaconID = Get_Int_Parameter("BeaconID");
		GameObject *beacon = Commands->Find_Object(BeaconID);
		if(beacon && beacon->As_PhysicalGameObj()->As_BeaconGameObj())
		{
			int IsNuke = beacon->As_PhysicalGameObj()->As_BeaconGameObj()->Get_Definition().Is_Nuke();
			if(DetTime>52)
			{
				Commands->Start_Timer(obj,this,DetTime-(float)67,3575);  // countdown initiated
				if(IsNuke)
				{
					if(DetTime>62)
					{
						Commands->Start_Timer(obj,this,DetTime-(float)52.5,3585);  // Nuclear Strike in 50 seconds
						if(DetTime>72)
						{
							Commands->Start_Timer(obj,this,DetTime-(float)62.5,3583);  // Nuclear Strike in 60 seconds
						}
					}
					Commands->Start_Timer(obj,this,DetTime-(float)42.5,3587);  // Nuclear Strike in 40 seconds
					Commands->Start_Timer(obj,this,DetTime-(float)32.5,3589);  // Nuclear Strike in 30 seconds
				}
				else 
				{
					if(DetTime>62)
					{
						Commands->Start_Timer(obj,this,DetTime-(float)52.5,3586);  // Nuclear Strike in 50 seconds
						if(DetTime>72)
						{
							Commands->Start_Timer(obj,this,DetTime-(float)62.5,3584);  // Nuclear Strike in 60 seconds
						}
					}
					Commands->Start_Timer(obj,this,DetTime-(float)42.5,3588);  // Ion Cannon Strike in 40 seconds
					Commands->Start_Timer(obj,this,DetTime-(float)32.5,3590);  // Ion Cannon Strike in 30 seconds
				}
				Commands->Start_Timer(obj,this,DetTime-25,3579);
				Commands->Start_Timer(obj,this,DetTime-20,3580);
				Commands->Start_Timer(obj,this,DetTime-15,3581);
			}

			else if(DetTime>25)
			{
				Commands->Start_Timer(obj,this,DetTime-(float)37,3575); // countdown initiated
				Commands->Start_Timer(obj,this,DetTime-(float)33.9,3574); // you have GDI
				Commands->Start_Timer(obj,this,DetTime-(float)33.8,3576); // you have Nod
				Commands->Start_Timer(obj,this,DetTime-33,3577); // 30
				Commands->Start_Timer(obj,this,DetTime-(float)32.4,3578); // Seconds to reach minimum safe distance
				Commands->Start_Timer(obj,this,DetTime-25,3579);
				Commands->Start_Timer(obj,this,DetTime-20,3580);
				Commands->Start_Timer(obj,this,DetTime-15,3581);
			}
			else if(DetTime>10)
			{
				Commands->Start_Timer(obj,this,DetTime-(float)14.9,3574); // you have GDI
				Commands->Start_Timer(obj,this,DetTime-(float)14.8,3576); // you have Nod
				Commands->Start_Timer(obj,this,DetTime-14,3582); // 10
				Commands->Start_Timer(obj,this,DetTime-(float)13.4,3578); // Seconds to reach minimum safe distance
			}
			//Commands->Start_Timer(obj,this,DetTime-10,3582);
			CountDown = 1;
		}
	}

	void Timer_Expired(GameObject *obj,int number)
	{
		if(number==3575)
		{
			//Console_Input("msg Beacon will detonate in 31 Seconds");
			Create_2D_WAV_Sound_Team_Dialog("m00evan_dsgn0102i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team_Dialog("m00evag_dsgn0096i1evag_snd.wav",1);
		}

		if(number==3574)
		{
			//Console_Input("msg Beacon will detonate in 31 Seconds");
			Create_2D_WAV_Sound_Team_Dialog("m00evag_dsgn0097i1evag_snd.wav",1);
		}

		if(number==3576)
		{
			//Console_Input("msg Beacon will detonate in 31 Seconds");
			Create_2D_WAV_Sound_Team_Dialog("m00evan_dsgn0103i1evan_snd.wav",0);
		}

		if(number==3577)
		{
			//Console_Input("msg Beacon will detonate in 30 Seconds");
			Create_2D_WAV_Sound_Team_Dialog("m00evan_dsgn0079i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team_Dialog("m00evag_dsgn0073i1evag_snd.wav",1);
		}

		if(number==3578)
		{
			//Console_Input("msg Beacon will detonate in 29 Seconds");
			Create_2D_WAV_Sound_Team_Dialog("m00evan_dsgn0104i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team_Dialog("m00evag_dsgn0098i1evag_snd.wav",1);
		}

		if(number==3579)
		{
			//Console_Input("msg Beacon will detonate in 25 Seconds");
			Create_2D_WAV_Sound_Team_Dialog("m00evan_dsgn0080i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team_Dialog("m00evag_dsgn0074i1evag_snd.wav",1);
		}

		if(number==3580)
		{
			//Console_Input("msg Beacon will detonate in 20 Seconds");
			Create_2D_WAV_Sound_Team_Dialog("m00evan_dsgn0081i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team_Dialog("m00evag_dsgn0075i1evag_snd.wav",1);
		}

		if(number==3581)
		{
			//Console_Input("msg Beacon will detonate in 15 Seconds");
			Create_2D_WAV_Sound_Team_Dialog("m00evan_dsgn0082i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team_Dialog("m00evag_dsgn0076i1evag_snd.wav",1);
		}

		if(number==3582)
		{
			//Console_InputF("msg Beacon will detonate in %i Seconds",11-CountDown);
			StringClass SoundNod;
			StringClass SoundGDI;
			SoundNod.Format("m00evan_dsgn00%ii1evan_snd.wav",82+CountDown);
			SoundGDI.Format("m00evag_dsgn00%ii1evag_snd.wav",76+CountDown);
			Create_2D_WAV_Sound_Team_Dialog(SoundNod,0);
			Create_2D_WAV_Sound_Team_Dialog(SoundGDI,1);
			//CountDown++;
			//if(CountDown<=11)
				//Commands->Start_Timer(obj,this,1,3582);
			//else
				//Commands->Destroy_Object(obj);
		}

		if(number==3583)
		{
			//Console_Input("msg Beacon will detonate in 60 Seconds");
			Create_2D_WAV_Sound_Team_Dialog("mxxdsgn_dsgn0033i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team_Dialog("mxxdsgn_dsgn0035i1evag_snd.wav",1);
		}
		
		if(number==3584)
		{
			//Console_Input("msg Beacon will detonate in 60 Seconds");
			Create_2D_WAV_Sound_Team_Dialog("mxxdsgn_dsgn0023i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team_Dialog("mxxdsgn_dsgn0024i1evag_snd.wav",1);
		}

		if(number==3585)
		{
			//Console_Input("msg Beacon will detonate in 50 Seconds");
			Create_2D_WAV_Sound_Team_Dialog("mxxdsgn_dsgn0032i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team_Dialog("mxxdsgn_dsgn0034i1evag_snd.wav",1);
		}
		
		if(number==3586)
		{
			//Console_Input("msg Beacon will detonate in 50 Seconds");
			Create_2D_WAV_Sound_Team_Dialog("mxxdsgn_dsgn0022i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team_Dialog("mxxdsgn_dsgn0023i1evag_snd.wav",1);
		}

		if(number==3587)
		{
			//Console_Input("msg Beacon will detonate in 40 Seconds");
			Create_2D_WAV_Sound_Team_Dialog("mxxdsgn_dsgn0031i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team_Dialog("mxxdsgn_dsgn0033i1evag_snd.wav",1);
		}
		
		if(number==3588)
		{
			//Console_Input("msg Beacon will detonate in 40 Seconds");
			Create_2D_WAV_Sound_Team_Dialog("mxxdsgn_dsgn0021i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team_Dialog("mxxdsgn_dsgn0022i1evag_snd.wav",1);
		}

		if(number==3589)
		{
			//Console_Input("msg Beacon will detonate in 30 Seconds");
			Create_2D_WAV_Sound_Team_Dialog("mxxdsgn_dsgn0030i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team_Dialog("mxxdsgn_dsgn0032i1evag_snd.wav",1);
		}
		
		if(number==3590)
		{
			//Console_Input("msg Beacon will detonate in 30 Seconds");
			Create_2D_WAV_Sound_Team_Dialog("mxxdsgn_dsgn0020i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team_Dialog("mxxdsgn_dsgn0021i1evag_snd.wav",1);
		}
	}

};

class DB_Enter_Teleport_Random : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter)
	{

		int random = Commands->Get_Random_Int(1,5);
		{
	
			if (random == 1)
			{
				int x = Get_Int_Parameter("Object_ID1");
				if (x)
				{
					GameObject *gotoObject = Commands->Find_Object(x);
					Vector3 gotoLocation = Commands->Get_Position(gotoObject);
					Commands->Set_Position(enter,gotoLocation);
				}
			}
			else if (random == 2)
			{
				int x = Get_Int_Parameter("Object_ID2");
				if (x)
				{
					GameObject *gotoObject = Commands->Find_Object(x);
					Vector3 gotoLocation = Commands->Get_Position(gotoObject);
					Commands->Set_Position(enter,gotoLocation);
				}
			}
			else if (random == 3)
			{
				int x = Get_Int_Parameter("Object_ID3");
				if (x)
				{
					GameObject *gotoObject = Commands->Find_Object(x);
					Vector3 gotoLocation = Commands->Get_Position(gotoObject);
					Commands->Set_Position(enter,gotoLocation);
				}
			}
			else if (random == 4)
			{
				int x = Get_Int_Parameter("Object_ID4");
				if (x)
				{
					GameObject *gotoObject = Commands->Find_Object(x);
					Vector3 gotoLocation = Commands->Get_Position(gotoObject);
					Commands->Set_Position(enter,gotoLocation);
				}
			}
			else
			{
				int x = Get_Int_Parameter("Object_ID5");
				if (x)
				{
					GameObject *gotoObject = Commands->Find_Object(x);
					Vector3 gotoLocation = Commands->Get_Position(gotoObject);
					Commands->Set_Position(enter,gotoLocation);
				}
			}
		}
	}
};

class DB_Face_Forward: public ScriptImpClass {
	Vector3 DefaultPos;
	float Duration;
	void DB_Face_Forward::Created(GameObject *obj) 	{
		Duration = Get_Float_Parameter("Duration");
		Vector3 pos = Commands->Get_Bone_Position(obj,"barrel");
		Vector3 target_direction = obj->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Transform( obj->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Index("Barrel")).Get_X_Vector();
		DefaultPos = pos + target_direction * 100;

		if(stristr(Commands->Get_Preset_Name(obj),"Sam") || stristr(Commands->Get_Preset_Name(obj),"AA_Gun"))
		{
			DefaultPos.Z+=36;
		}
		ActionParamsStruct var;
		var.Priority=10;
		var.Set_Face_Location(DefaultPos,0,Duration);
		Commands->Action_Face_Location(obj,var);
	}

	void DB_Face_Forward::Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if(type==8765)
		{
			GameObject *target = Commands->Find_Object(param);
			if(obj->As_VehicleGameObj() && obj->As_VehicleGameObj()->Is_Turret())
			{
				ActionParamsStruct var;
				var.Priority=71;
				var.Set_Attack(target,1000,0,true);
				Commands->Action_Attack(obj,var);
			}
		}

		else if(type==5432)
		{
			ActionParamsStruct var;
			var.Priority=71;
			var.Set_Face_Location(DefaultPos,0,Duration);
			Commands->Action_Face_Location(obj,var);
		}

		else if(type==7654)
		{
			GameObject *target = Commands->Find_Object(param);
			if(target)
			{
				if(Get_Object_Type(target)!=Get_Object_Type(obj))
				{

				Vector3 TargetPos = Commands->Get_Position(target);
				ActionParamsStruct var;
				var.Priority=71;
				var.Set_Face_Location(TargetPos,0,Duration);
				Commands->Action_Face_Location(obj,var);
				}
			}
		}
	}
};

ScriptRegistrant<DB_Face_Forward> DB_Face_Forward_Registrant("DB_Face_Forward", "Duration=1:float");
ScriptRegistrant<DB_Captureable_Silo>DB_Captureable_Silo_Registrant("DB_Captureable_Silo","Team=-2:int,Amount=2:float,UpgradeAmount=3:float,UpgradeID=0:int");
ScriptRegistrant<DB_Helipad_Captureable> DB_Capturable_Helipad_Registrant("DB_Capturable_Helipad","Team=-2:int,ReloadZoneID=0:int");
ScriptRegistrant<DB_Powerup_Buy_Poke_Sound> DB_Powerup_Buy_Poke_Sound_Registrant("DB_Powerup_Buy_Poke_Sound", "Team=0:int,BuildingID=0:int,Preset=bla:string,Cost=10000:float,SoundGranted=bla:string,SoundDenied=bla:string");
ScriptRegistrant<DB_capturable_Helipad_Terminal> DB_capturable_Helipad_Terminal_Registrant("DB_capturable_Helipad_Terminal", "BuildingID=0:int,Preset=bla:string,Cost=10000:float,SpawnLocation=0:int");
ScriptRegistrant<DB_capturable_WF_Terminal> DB_capturable_WF_Terminal_Registrant("DB_capturable_WF_Terminal", "BuildingID=0:int,Preset=bla:string,Cost=10000:float,SpawnLocation=0:int,WaypathID=0:int");
ScriptRegistrant<DB_capturable_helipadzone_reload> DB_capturable_helipad_zone_reload_Registrant("DB_capturable_helipadzone_reload", "BuildingID=0:int,ReloadPreset=bla:string");
ScriptRegistrant<DB_Capturable_repairpad> DB_capturable_repairpad_Registrant("DB_capturable_repairpad", "Team=-2:int,RepairZoneID=0:int");
ScriptRegistrant<DB_Capturable_object> DB_capturable_object_Registrant("DB_capturable_object", "Owner=-2:int");
ScriptRegistrant<DB_Capturable_object> DB_Capturable_Object_2_Registrant("DB_Capturable_Object_2", "Owner=0:int,Audio_Captured_GDI_GDI=asdf:string,Audio_Captured_GDI_Nod=asdf:string,Audio_Captured_Nod_GDI=asdf:string,Audio_Captured_Nod_Nod=asdf:string,Audio_Killed_GDI_GDI=asdf:string,Audio_Killed_GDI_Nod=asdf:string,Audio_Killed_Nod_GDI=asdf:string,Audio_Killed_Nod_Nod=asdf:string");
ScriptRegistrant<DB_capturable_Repairzone> DB_capturable_Repairzone_Registrant("DB_capturable_Repairzone", "BuildingID=0:int");
ScriptRegistrant<DB_capturable_Repairzone> DB_capturable_Helipad_Repairzone_Registrant("DB_capturable_Helipad_Repairzone", "BuildingID=0:int");
ScriptRegistrant<Nod_Turret_DeathSound> Nod_Turret_DeathSound_Registrant("Nod_Turret_DeathSound", "");
ScriptRegistrant<GDI_Guard_Tower_DeathSound> GDI_Guard_Tower_DeathSound_Registrant("GDI_Guard_Tower_DeathSound", "");
ScriptRegistrant<Nod_SamSite_DeathSound> Nod_SamSite_DeathSound_Registrant("Nod_SamSite_DeathSound", "");
ScriptRegistrant<DB_Power_Plant_fix> DB_Power_Plant_fix_Registrant("DB_Power_Plant_fix", "");
ScriptRegistrant<DB_Beacon_fix> DB_Beacon_fix_Registrant("DB_Beacon_fix", "");
ScriptRegistrant<DB_Beacon_fix2> DB_Beacon_fix2_Registrant("DB_Beacon_fix2", "BeaconID=0:int,DetTime:0:float");
ScriptRegistrant<DB_Enter_Teleport_Random> DB_Enter_Teleport_Random_Registrant("DB_Enter_Teleport_Random", "Object_ID1=1:int,Object_ID2=1:int,Object_ID3=1:int,Object_ID4=1:int,Object_ID5=1:int");