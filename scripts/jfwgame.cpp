/*	Renegade Scripts.dll
	Copyright 2017 Tiberian Technologies

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
#include "engine.h"
#include "jfwgame.h"
#include "TeamPurchaseSettingsDefClass.h"
#include "VehicleGameObj.h"
#include "BuildingGameObj.h"
#include "BuildingGameObjDef.h"
#include "WeaponClass.h"

void JFW_Team_DM_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	int x;
	int msg;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(exiter,x))
	{
		return;
	}
	msg = Get_Int_Parameter("Deactivate_Message");
	triggered = false;
	Commands->Send_Custom_Event(obj,exiter,msg,1,0);
	if (!triggered)
	{
		char param[100];
		sprintf(param,"%d,%d,%d",Get_Int_Parameter("Controller_ID"),Get_Int_Parameter("Deactivate_Message"),Get_Int_Parameter("Death_Message"));
		Commands->Attach_Script(exiter,"JFW_Team_DM_Attatched",param);
	}
}

void JFW_Team_DM_Zone::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Deactivate_Message");
	if (type == msg)
	{
		triggered = true;
	}
}

void JFW_Team_DM_Controller::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	int maxdeaths;
	int id;
	GameObject *tmpObj;
	float maxHealth;
	msg = Get_Int_Parameter("Death_Message");
	maxdeaths = Get_Int_Parameter("Deaths_To_Loose");
	if (type == msg)
	{
		deaths++;
	}
	if (deaths >= maxdeaths)
	{
		id = Get_Int_Parameter("Building_To_Destroy1");
		tmpObj = Commands->Find_Object(id);
		maxHealth = Commands->Get_Max_Health(tmpObj);
		Commands->Apply_Damage(tmpObj,10000,"Explosive",0);
		id = Get_Int_Parameter("Building_To_Destroy2");
		tmpObj = Commands->Find_Object(id);
		maxHealth = Commands->Get_Max_Health(tmpObj);
		Commands->Apply_Damage(tmpObj,10000,"Explosive",0);
		id = Get_Int_Parameter("Building_To_Destroy3");
		tmpObj = Commands->Find_Object(id);
		maxHealth = Commands->Get_Max_Health(tmpObj);
		Commands->Apply_Damage(tmpObj,10000,"Explosive",0);
		id = Get_Int_Parameter("Building_To_Destroy4");
		tmpObj = Commands->Find_Object(id);
		maxHealth = Commands->Get_Max_Health(tmpObj);
		Commands->Apply_Damage(tmpObj,10000,"Explosive",0);
		id = Get_Int_Parameter("Building_To_Destroy5");
		tmpObj = Commands->Find_Object(id);
		maxHealth = Commands->Get_Max_Health(tmpObj);
		Commands->Apply_Damage(tmpObj,10000,"Explosive",0);
	}
}

void JFW_Team_DM_Attatched::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Deactivate_Message");
	if (type == msg)
	{
		Commands->Send_Custom_Event(obj,sender,msg,1,0);
	}
}

void JFW_Team_DM_Controller::Created(GameObject *obj)
{
	deaths = 0;
}

void JFW_Team_DM_Attatched::Killed(GameObject *obj,GameObject *killer)
{
	int msg;
	int controller_id;
	GameObject *controller;
	msg = Get_Int_Parameter("Death_Message");
	controller_id = Get_Int_Parameter("Controller_ID");
	controller = Commands->Find_Object(controller_id);
	Commands->Send_Custom_Event(obj,controller,msg,1,0);
}

void JFW_Team_DM_Controller::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&deaths,4,1);
}

void JFW_Team_DM_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&triggered,1,1);
}

void JFW_Assault_Powerplant_Controller::Created(GameObject *obj)
{
	float timertime;
	int timernumber;
	int poweroff;
	int building;
	GameObject *object;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	poweroff = Get_Int_Parameter("Power_Off");
	Commands->Start_Timer(obj,this,timertime,timernumber);
	building = Get_Int_Parameter("Building1");
	object = Commands->Find_Object(building);
	Commands->Send_Custom_Event(obj,object,poweroff,0,0);
	building = Get_Int_Parameter("Building2");
	object = Commands->Find_Object(building);
	Commands->Send_Custom_Event(obj,object,poweroff,0,0);
	building = Get_Int_Parameter("Building3");
	object = Commands->Find_Object(building);
	Commands->Send_Custom_Event(obj,object,poweroff,0,0);
	building = Get_Int_Parameter("Building4");
	object = Commands->Find_Object(building);
	Commands->Send_Custom_Event(obj,object,poweroff,0,0);
	building = Get_Int_Parameter("Building5");
	object = Commands->Find_Object(building);
	Commands->Send_Custom_Event(obj,object,poweroff,0,0);
	building = Get_Int_Parameter("Building6");
	object = Commands->Find_Object(building);
	Commands->Send_Custom_Event(obj,object,poweroff,0,0);
	building = Get_Int_Parameter("Building7");
	object = Commands->Find_Object(building);
	Commands->Send_Custom_Event(obj,object,poweroff,0,0);
	building = Get_Int_Parameter("Building8");
	object = Commands->Find_Object(building);
	Commands->Send_Custom_Event(obj,object,poweroff,0,0);
	building = Get_Int_Parameter("Building9");
	object = Commands->Find_Object(building);
	Commands->Send_Custom_Event(obj,object,poweroff,0,0);
	building = Get_Int_Parameter("Building10");
	object = Commands->Find_Object(building);
	Commands->Send_Custom_Event(obj,object,poweroff,0,0);
	building = Get_Int_Parameter("Building11");
	object = Commands->Find_Object(building);
	Commands->Send_Custom_Event(obj,object,poweroff,0,0);
	building = Get_Int_Parameter("Building12");
	object = Commands->Find_Object(building);
	Commands->Send_Custom_Event(obj,object,poweroff,0,0);
	building = Get_Int_Parameter("Building13");
	object = Commands->Find_Object(building);
	Commands->Send_Custom_Event(obj,object,poweroff,0,0);
	building = Get_Int_Parameter("Building14");
	object = Commands->Find_Object(building);
	Commands->Send_Custom_Event(obj,object,poweroff,0,0);
	building = Get_Int_Parameter("Building15");
	object = Commands->Find_Object(building);
	Commands->Send_Custom_Event(obj,object,poweroff,0,0);
	inzone = false;
}

void JFW_Assault_Powerplant_Controller::Timer_Expired(GameObject *obj,int number)
{
	int timernumber;
	int poweroff;
	int building;
	GameObject *object;
	timernumber = Get_Int_Parameter("TimerNum");
	poweroff = Get_Int_Parameter("Power_Off");
	if ((!inzone) && (number == timernumber))
	{
		building = Get_Int_Parameter("Building1");
		object = Commands->Find_Object(building);
		Commands->Send_Custom_Event(obj,object,poweroff,0,0);
		building = Get_Int_Parameter("Building2");
		object = Commands->Find_Object(building);
		Commands->Send_Custom_Event(obj,object,poweroff,0,0);
		building = Get_Int_Parameter("Building3");
		object = Commands->Find_Object(building);
		Commands->Send_Custom_Event(obj,object,poweroff,0,0);
		building = Get_Int_Parameter("Building4");
		object = Commands->Find_Object(building);
		Commands->Send_Custom_Event(obj,object,poweroff,0,0);
		building = Get_Int_Parameter("Building5");
		object = Commands->Find_Object(building);
		Commands->Send_Custom_Event(obj,object,poweroff,0,0);
		building = Get_Int_Parameter("Building6");
		object = Commands->Find_Object(building);
		Commands->Send_Custom_Event(obj,object,poweroff,0,0);
		building = Get_Int_Parameter("Building7");
		object = Commands->Find_Object(building);
		Commands->Send_Custom_Event(obj,object,poweroff,0,0);
		building = Get_Int_Parameter("Building8");
		object = Commands->Find_Object(building);
		Commands->Send_Custom_Event(obj,object,poweroff,0,0);
		building = Get_Int_Parameter("Building9");
		object = Commands->Find_Object(building);
		Commands->Send_Custom_Event(obj,object,poweroff,0,0);
		building = Get_Int_Parameter("Building10");
		object = Commands->Find_Object(building);
		Commands->Send_Custom_Event(obj,object,poweroff,0,0);
		building = Get_Int_Parameter("Building11");
		object = Commands->Find_Object(building);
		Commands->Send_Custom_Event(obj,object,poweroff,0,0);
		building = Get_Int_Parameter("Building12");
		object = Commands->Find_Object(building);
		Commands->Send_Custom_Event(obj,object,poweroff,0,0);
		building = Get_Int_Parameter("Building13");
		object = Commands->Find_Object(building);
		Commands->Send_Custom_Event(obj,object,poweroff,0,0);
		building = Get_Int_Parameter("Building14");
		object = Commands->Find_Object(building);
		Commands->Send_Custom_Event(obj,object,poweroff,0,0);
		building = Get_Int_Parameter("Building15");
		object = Commands->Find_Object(building);
		Commands->Send_Custom_Event(obj,object,poweroff,0,0);
	}
}

void JFW_Assault_Powerplant_Controller::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	inzone = true;
}

void JFW_Assault_Powerplant_Controller::Exited(GameObject *obj,GameObject *exiter)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(exiter,x))
	{
		return;
	}
	inzone = false;
}

void JFW_Assault_Powerplant_Controller::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&inzone,1,1);
}

void JFW_Resource_Collector::Created(GameObject *obj)
{
	cash = 0;
	ID = 0;
}

void JFW_Resource_Collector::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int Collected_Message = Get_Int_Parameter("Collected_Mesasge");
	int Refinery_Message = Get_Int_Parameter("Refinery_Message");
	int Cash_Limit = Get_Int_Parameter("Cash_Limit");
	int Cash_Type = Get_Int_Parameter("Cash_Type");
	bool Entire_Team = Get_Bool_Parameter("Entire_Team");
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		int OldID;
		OldID = ID;
		ID = Commands->Get_ID(sender);
		if (!Cash_Type)
		{
			cash = 0;
		}
		if ((Cash_Type == 1) && (ID != OldID))
		{
			cash = 0;
		}
	}
	if (type == Collected_Message)
	{
		if (Cash_Type == 3)
		{
			cash = 0;
		}
		cash += param;
		if ((cash > Cash_Limit) && (Cash_Limit))
		{
			cash = Cash_Limit;
		}
	}
	if (type == Refinery_Message)
	{
		Commands->Give_Money(Commands->Find_Object(ID),(float)cash,Entire_Team);
		cash = 0;
	}
}

void JFW_Resource_Collector::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&cash,4,1);
	Auto_Save_Variable(&ID,4,2);
}

void JFW_Resource_Collector_2::Created(GameObject *obj)
{
	HasOre = false;
	cash = 0;
	ID = 0;
	PlayerID = 0;
}

void JFW_Resource_Collector_2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int team = Commands->Get_Player_Type(obj);
	int Collected_Message = Get_Int_Parameter("Collected_Mesasge");
	int Refinery_Message = Get_Int_Parameter("Refinery_Message");
	int Cash_Limit = Get_Int_Parameter("Cash_Limit");
	int Cash_Type = Get_Int_Parameter("Cash_Type");
	bool Entire_Team = Get_Bool_Parameter("Entire_Team");
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		int OldID;
		int OldPlayerID;
		OldID = ID;
		OldPlayerID = PlayerID;
		ID = Commands->Get_ID(sender);
		PlayerID = Get_Player_ID(sender);
		if (!Cash_Type)
		{
			cash = 0;
		}
		if ((Cash_Type == 1) && (ID != OldID) && (PlayerID != OldPlayerID))
		{
			cash = 0;
		}
	}
	if (type == Collected_Message)
	{
		HasOre = true;
		if (Cash_Type == 3)
		{
			cash = 0;
		}
		cash += param;
		if ((cash > Cash_Limit) && (Cash_Limit))
		{
			cash = Cash_Limit;
		}
	}
	if ((type == Refinery_Message) && (HasOre))
	{
		char DumpcMsg[240];
		if (team == 0)
		{
			if (cash == 700)
			{
				Commands->Give_Money(Commands->Find_Object(ID),(float)cash,Entire_Team);
				const char *c = Get_Player_Name_By_ID(PlayerID);
				sprintf(DumpcMsg,"cmsgt 0 9,193,18 %s dumped Ore, funding the Soviet Warmachine with 700 additional credits.",c);
				delete[] c;
				HasOre = false;
				cash = 0;
			}
			else if (cash == 1050)
			{
				Commands->Give_Money(Commands->Find_Object(ID),(float)cash,Entire_Team);
				const char *c = Get_Player_Name_By_ID(PlayerID);
				sprintf(DumpcMsg,"cmsgt 0 9,193,18 %s dumped Mixed Ore/Gems, funding the Soviet Warmachine with 1050 additional credits.",c);
				delete[] c;
				HasOre = false;
				cash = 0;
			}
			else if (cash == 1400)
			{
				Commands->Give_Money(Commands->Find_Object(ID),(float)cash,Entire_Team);
				const char *c = Get_Player_Name_By_ID(PlayerID);
				sprintf(DumpcMsg,"cmsgt 0 9,193,18 %s dumped Gems, funding the Soviet Warmachine with 1400 additional credits.",c);
				delete[] c;
				HasOre = false;
				cash = 0;
			}
			Console_Input("sndt 0 gsweep.wav");
			Console_Input(DumpcMsg);
		}
		else if (team == 1)
		{ 
			if (cash == 700)
			{
				Commands->Give_Money(Commands->Find_Object(ID),(float)cash,Entire_Team);
				const char *c = Get_Player_Name_By_ID(PlayerID);
				sprintf(DumpcMsg,"cmsgt 1 9,193,18 %s dumped Ore, funding the Allied Forces with 700 additional credits.",c);
				delete[] c;
				HasOre = false;
				cash = 0;
			}
			else if (cash == 1050)
			{
				Commands->Give_Money(Commands->Find_Object(ID),(float)cash,Entire_Team);
				const char *c = Get_Player_Name_By_ID(PlayerID);
				sprintf(DumpcMsg,"cmsgt 1 9,193,18 %s dumped Mixed Ore/Gems, funding the Allied Forces with 1050 additional credits.",c);
				delete[] c;
				HasOre = false;
				cash = 0;
			}
			else if (cash == 1400)
			{
				Commands->Give_Money(Commands->Find_Object(ID),(float)cash,Entire_Team);
				const char *c = Get_Player_Name_By_ID(PlayerID);
				sprintf(DumpcMsg,"cmsgt 1 9,193,18 %s dumped Gems, funding the Allied Forces with 1400 additional credits.",c);
				delete[] c;
				HasOre = false;
				cash = 0;
			}
			Console_Input("sndt 1 gsweep.wav");
			Console_Input(DumpcMsg);
		}
	}
}

void JFW_Resource_Collector_2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&cash,4,1);
	Auto_Save_Variable(&ID,4,2);
	Auto_Save_Variable(&PlayerID,4,3);
	Auto_Save_Variable(&HasOre,1,4);
}

void JFW_Resource_Collector_3::Created(GameObject *obj)
{
	HasOre = false;
	cash = 0;
	ID = 0;
	PlayerID = 0;
}

void JFW_Resource_Collector_3::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int team = Commands->Get_Player_Type(obj);
	int Collected_Message = Get_Int_Parameter("Collected_Mesasge");
	int Refinery_Message = Get_Int_Parameter("Refinery_Message");
	int Cash_Limit = Get_Int_Parameter("Cash_Limit");
	int Cash_Type = Get_Int_Parameter("Cash_Type");
	bool Entire_Team = Get_Bool_Parameter("Entire_Team");
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		int OldID;
		int OldPlayerID;
		OldID = ID;
		OldPlayerID = PlayerID;
		ID = Commands->Get_ID(sender);
		PlayerID = Get_Player_ID(sender);
		if (!Cash_Type)
		{
			cash = 0;
		}
		if ((Cash_Type == 1) && (ID != OldID) && (PlayerID != OldPlayerID))
		{
			cash = 0;
		}
	}
	if (type == Collected_Message)
	{
		HasOre = true;
		if (Cash_Type == 3)
		{
			cash = 0;
		}
		cash += param;
		if ((cash > Cash_Limit) && (Cash_Limit))
		{
			cash = Cash_Limit;
		}
	}
	if ((type == Refinery_Message) && (HasOre))
	{
		char DumpcMsg[240];
		if (team == 0)
		{
			if (cash == 700)
			{
				Commands->Give_Money(Commands->Find_Object(ID),(float)cash,Entire_Team);
				const char *c = Get_Player_Name_By_ID(PlayerID);
				sprintf(DumpcMsg,"cmsgt 0 9,193,18 %s dumped Tiberium, funding the Nod Warmachine with 700 additional credits.",c);
				delete[] c;
				HasOre = false;
				cash = 0;
			}
			else if (cash == 1050)
			{
				Commands->Give_Money(Commands->Find_Object(ID),(float)cash,Entire_Team);
				const char *c = Get_Player_Name_By_ID(PlayerID);
				sprintf(DumpcMsg,"cmsgt 0 9,193,18 %s dumped Mixed Tiberium/Blue Tiberium, funding the Nod Warmachine with 1050 additional credits.",c);
				delete[] c;
				HasOre = false;
				cash = 0;
			}
			else if (cash == 1400)
			{
				Commands->Give_Money(Commands->Find_Object(ID),(float)cash,Entire_Team);
				const char *c = Get_Player_Name_By_ID(PlayerID);
				sprintf(DumpcMsg,"cmsgt 0 9,193,18 %s dumped Blue Tiberium, funding the Nod Warmachine with 1400 additional credits.",c);
				delete[] c;
				HasOre = false;
				cash = 0;
			}
			Console_Input("sndt 0 gsweep.wav");
			Console_Input(DumpcMsg);
		}
		else if (team == 1)
		{ 
			if (cash == 700)
			{
				Commands->Give_Money(Commands->Find_Object(ID),(float)cash,Entire_Team);
				const char *c = Get_Player_Name_By_ID(PlayerID);
				sprintf(DumpcMsg,"cmsgt 1 9,193,18 %s dumped Tiberium, funding the GDI Forces with 700 additional credits.",c);
				delete[] c;
				HasOre = false;
				cash = 0;
			}
			else if (cash == 1050)
			{
				Commands->Give_Money(Commands->Find_Object(ID),(float)cash,Entire_Team);
				const char *c = Get_Player_Name_By_ID(PlayerID);
				sprintf(DumpcMsg,"cmsgt 1 9,193,18 %s dumped Mixed Tiberium/Blue Tiberium, funding the GDI Forces with 1050 additional credits.",c);
				delete[] c;
				HasOre = false;
				cash = 0;
			}
			else if (cash == 1400)
			{
				Commands->Give_Money(Commands->Find_Object(ID),(float)cash,Entire_Team);
				const char *c = Get_Player_Name_By_ID(PlayerID);
				sprintf(DumpcMsg,"cmsgt 1 9,193,18 %s dumped Blue Tiberium, funding the GDI Forces with 1400 additional credits.",c);
				delete[] c;
				HasOre = false;
				cash = 0;
			}
			Console_Input("sndt 1 gsweep.wav");
			Console_Input(DumpcMsg);
		}
	}
}

void JFW_Resource_Collector_3::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&cash,4,1);
	Auto_Save_Variable(&ID,4,2);
	Auto_Save_Variable(&PlayerID,4,3);
	Auto_Save_Variable(&HasOre,1,4);
}

void JFW_Resource_Refinery::Entered(GameObject *obj,GameObject *enterer)
{
	int Refinery_Message = Get_Int_Parameter("Refinery_Message");
	Commands->Send_Custom_Event(obj,enterer,Refinery_Message,0,0);
}

void JFW_Resource_Field::Created(GameObject *obj)
{
	ID = 0;
}

void JFW_Resource_Field::Entered(GameObject *obj,GameObject *enterer)
{
	const char *Nod_Preset = Get_Parameter("Nod_Preset");
	const char *Preset = Commands->Get_Preset_Name(enterer);
	const char *GDI_Preset = Get_Parameter("GDI_Preset");
	if (!ID)
	{
		if ((!_stricmp(Preset,Nod_Preset)) || (!_stricmp(Preset,GDI_Preset)))
		{
			ID = Commands->Get_ID(enterer);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
			Commands->Send_Custom_Event(obj,enterer,Get_Int_Parameter("StartHarvest"),0,0);
		}
	}
}

void JFW_Resource_Field::Exited(GameObject *obj,GameObject *exiter)
{
	if (Commands->Get_ID(exiter) == ID)
	{
		ID = 0;
		Commands->Send_Custom_Event(obj,exiter,Get_Int_Parameter("StopHarvest"),0,0);
	}
}

void JFW_Resource_Field::Timer_Expired(GameObject *obj,int number)
{
	int Cash = Get_Int_Parameter("Cash");
	int Collected_Message = Get_Int_Parameter("Collected_Message");
	Vector3 pos;
	float angle;
	GameObject *object;
	const char *Collected_Preset = Get_Parameter("Collected_Preset");
	float Collected_Preset_Distance = Get_Float_Parameter("Collected_Preset_Distance");
	if (ID)
	{
		object = Commands->Find_Object(ID);
		Commands->Send_Custom_Event(obj,object,Collected_Message,Cash,0);
		pos = Commands->Get_Position(object);
		angle = DEG_TO_RADF(Commands->Get_Facing(object));
		pos.X+=Collected_Preset_Distance*(float)cos(angle);
		pos.Y+=Collected_Preset_Distance*(float)sin(angle);
		Commands->Create_Object(Collected_Preset,pos);
		Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("StopHarvest"),0,0);
	}
}

void JFW_Resource_Field::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ID,4,1);
}

void JFW_Domination_Zone::Created(GameObject *obj)
{
	enabled = true;
	GameObject *object;
	float facing;
	object = Commands->Create_Object("Generic_Cinematic",Commands->Get_Position(obj));
	Commands->Enable_Hibernation(object,false);
	Commands->Add_To_Dirty_Cull_List(object);
	Commands->Set_Model(object,Get_Parameter("Neutral_Model"));
	facing = Commands->Get_Facing(obj);
	Commands->Set_Facing(object,facing);
	currentmodelid = Commands->Get_ID(object);
	owner = 2;
}

void JFW_Domination_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	float timertime;
	int timernumber;
	GameObject *object;
	float facing;
	int type;
	if (!enabled)
	{
		return;
	}
	if (Commands->Get_Player_Type(enterer) == owner)
	{
		return;
	}
	if (owner == 2)
	{
		type = Get_Int_Parameter("NeutralMessage");
	}
	else
	{
		type = Get_Int_Parameter("OwnedMessage");
	}
	Commands->Destroy_Object(Commands->Find_Object(currentmodelid));
	object = Commands->Create_Object("Generic_Cinematic",Commands->Get_Position(obj));
	Commands->Enable_Hibernation(object,false);
	Commands->Add_To_Dirty_Cull_List(object);
	if (!Commands->Get_Player_Type(enterer))
	{
		Commands->Set_Model(object,Get_Parameter("Nod_Model"));
		owner = 0;
	}
	if (Commands->Get_Player_Type(enterer) == 1)
	{
		Commands->Set_Model(object,Get_Parameter("GDI_Model"));
		owner = 1;
	}
	facing = Commands->Get_Facing(obj);
	Commands->Set_Facing(object,facing);
	currentmodelid = Commands->Get_ID(object);
	object = Commands->Find_Object(Get_Int_Parameter("Controler_ID"));
	Commands->Send_Custom_Event(obj,object,type,owner,0);
	enabled = false;
	timertime = Get_Float_Parameter("Capture_Delay_Time");
	timernumber = Get_Int_Parameter("TimerNum");
	if (timertime)
	{
		Commands->Start_Timer(obj,this,timertime,timernumber);
	}
	else
	{
		enabled = true;
	}
}

void JFW_Domination_Zone::Timer_Expired(GameObject *obj,int number)
{
	enabled = true;
}

void JFW_Domination_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&currentmodelid,4,1);
	Auto_Save_Variable(&enabled,1,2);
	Auto_Save_Variable(&owner,4,3);
}

void JFW_Domination_Controler::Created(GameObject *obj)
{
	float timertime;
	int timernumber;
	gdizonecount = 0;
	nodzonecount = 0;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_Domination_Controler::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int neutralmessage;
	int ownedmessage;
	neutralmessage = Get_Int_Parameter("NeutralMessage");
	ownedmessage = Get_Int_Parameter("OwnedMessage");
	if (type == ownedmessage)
	{
		if (!param)
		{
			gdizonecount--;
		}
		else
		{
			nodzonecount--;
		}
	}
	if ((type == neutralmessage) || (type == ownedmessage))
	{
		if (param == 1)
		{
			gdizonecount++;
		}
		else
		{
			nodzonecount++;
		}
	}
}

void JFW_Domination_Controler::Timer_Expired(GameObject *obj,int number)
{
	float timertime;
	int timernumber;
	float pointstogive;
	float pointstotake;
	GameObject *gdiobject;
	GameObject *nodobject;
	gdiobject = Commands->Find_Object(Get_Int_Parameter("GDIObjectID"));
	nodobject = Commands->Find_Object(Get_Int_Parameter("NodObjectID"));
	pointstogive = Get_Float_Parameter("PointsToGive");
	pointstotake = Get_Float_Parameter("PointsToTake");
	if (gdizonecount > 0)
	{
		if (pointstogive)
		{
			Commands->Give_Points(gdiobject,pointstogive*gdizonecount,true);
		}
		if (pointstotake)
		{
			Commands->Give_Points(nodobject,-(pointstotake*gdizonecount),true);
		}
	}
	if (nodzonecount > 0)
	{
		if (pointstogive)
		{
			Commands->Give_Points(nodobject,pointstogive*nodzonecount,true);
		}
		if (pointstotake)
		{
			Commands->Give_Points(gdiobject,-(pointstotake*nodzonecount),true);
		}
	}
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_Domination_Controler::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&gdizonecount,4,1);
	Auto_Save_Variable(&nodzonecount,4,2);
}

void JFW_Iron_Curtain_Zone::Created(GameObject *obj)
{
	enabled = false;
	float timertime;
	int timernumber;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
	Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OffID")),true);
	Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OnID")),false);
}

void JFW_Iron_Curtain_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	char params[1000];
	float timertime;
	int timernumber;
	const char *scriptname;
	Vector3 position;
	GameObject *object;
	float facing;
	int Player_Type = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,Player_Type)) 
	{
		return;
	}
	if (!enabled)
	{
		return;
	}
	if (enterer->As_VehicleGameObj())
	{
		enabled = false;
		timertime = Get_Float_Parameter("Time");
		timernumber = Get_Int_Parameter("TimerNum");
		Commands->Start_Timer(obj,this,timertime,timernumber);
		if (Get_Int_Parameter("CinematicPowerOff") == 1)
		{
			Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OnID")),false);
			Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OffID")),false);
		}
		scriptname = Get_Parameter("Animation2");
		position = Get_Vector3_Parameter("Animation2Loc");
		facing = Get_Float_Parameter("Animation2Facing");
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"Test_Cinematic",scriptname);
		Commands->Send_Custom_Event(obj,object,10000+Get_Int_Parameter("Animation2Slot"),Get_Int_Parameter("BuildingID"),0);
		sprintf(params,"%s,%s",Get_Parameter("Invunlerability_Time"),Get_Parameter("Invunlerability_TimerNum"));
		Commands->Attach_Script(enterer,"JFW_Invulnerability_Timer",params);
	}
}

void JFW_Iron_Curtain_Zone::Killed(GameObject *obj,GameObject *killer)
{
	if (enabled)
	{
		if (Get_Int_Parameter("CinematicPowerOff") == 1)
		{
			Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OnID")),false);
			Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OffID")),false);
		}
		const char *scriptname;
		Vector3 position;
		GameObject *object;
		float facing;
		scriptname = Get_Parameter("Animation3");
		position = Get_Vector3_Parameter("Animation3Loc");
		facing = Get_Float_Parameter("Animation3Facing");
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"Test_Cinematic",scriptname);
		Commands->Send_Custom_Event(obj,object,10000+Get_Int_Parameter("Animation3Slot"),Get_Int_Parameter("BuildingID"),0);
	}
}

void JFW_Iron_Curtain_Zone::Timer_Expired(GameObject *obj,int number)
{
	const char *scriptname;
	Vector3 position;
	GameObject *object;
	float facing;
	int num;
	num = Get_Int_Parameter("TimerNum");
	if (num == number)
	{
		if (Get_Int_Parameter("CinematicPowerOff") == 1)
		{
			Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OnID")),false);
			Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OffID")),false);
		}
		scriptname = Get_Parameter("Animation1");
		position = Get_Vector3_Parameter("Animation1Loc");
		facing = Get_Float_Parameter("Animation1Facing");
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"Test_Cinematic",scriptname);
		Commands->Send_Custom_Event(obj,object,10000+Get_Int_Parameter("Animation1Slot"),Get_Int_Parameter("BuildingID"),0);
		enabled = true;
	}
}

void JFW_Iron_Curtain_Zone::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	const char *scriptname;
	Vector3 position;
	GameObject *object;
	float facing;
	if (type == Get_Int_Parameter("Animation1Msg"))
	{
		Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OnID")),true);
		Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OffID")),false);
	}
	if (type == Get_Int_Parameter("Animation2Msg"))
	{
		scriptname = Get_Parameter("Animation3");
		position = Get_Vector3_Parameter("Animation3Loc");
		facing = Get_Float_Parameter("Animation3Facing");
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"Test_Cinematic",scriptname);
		Commands->Send_Custom_Event(obj,object,10000+Get_Int_Parameter("Animation3Slot"),Get_Int_Parameter("BuildingID"),0);
	}
	if (type == Get_Int_Parameter("Animation3Msg"))
	{
		Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OnID")),false);
		Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OffID")),true);
	}
}

void JFW_Iron_Curtain_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Chrono_Controler_Zone::Created(GameObject *obj)
{
	enabled = false;
	activating = false;
	xpos = 0;
	ypos = 0;
	zpos = 0;
	float timertime;
	int timernumber;
	timertime = Get_Float_Parameter("Spawner_Time");
	timernumber = Get_Int_Parameter("Spawner_TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
	Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OnID")),false);
	Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OffID")),true);
}

void JFW_Chrono_Controler_Zone::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	float timertime;
	int timernumber;
	int msg = Get_Int_Parameter("Message");
	const char *scriptname;
	Vector3 position;
	GameObject *object;
	float facing;
	if (type == msg)
	{
		Vector3 pos = Commands->Get_Position(sender);
		xpos = pos.X;
		ypos = pos.Y;
		zpos = pos.Z;
		activating = true;
		timertime = Get_Float_Parameter("Activate_Time");
		timernumber = Get_Int_Parameter("Activate_TimerNum");
		Commands->Start_Timer(obj,this,timertime,timernumber);
	}
	if (type == Get_Int_Parameter("DeathMessage"))
	{
		activating = false;
	}
	if (type == Get_Int_Parameter("Animation1Msg"))
	{
		Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OnID")),true);
		Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OffID")),false);
	}
	if (type == Get_Int_Parameter("Animation2Msg"))
	{
		scriptname = Get_Parameter("Animation3");
		position = Get_Vector3_Parameter("Animation3Loc");
		facing = Get_Float_Parameter("Animation3Facing");
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"Test_Cinematic",scriptname);
		Commands->Send_Custom_Event(obj,object,10000+Get_Int_Parameter("Animation3Slot"),Get_Int_Parameter("BuildingID"),0);
	}
	if (type == Get_Int_Parameter("Animation3Msg"))
	{
		Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OnID")),false);
		Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OffID")),true);
		timertime = Get_Float_Parameter("Spawner_Time");
		timernumber = Get_Int_Parameter("Spawner_TimerNum");
		Commands->Start_Timer(obj,this,timertime,timernumber);
	}
}

void JFW_Chrono_Controler_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	const char *scriptname;
	Vector3 position;
	GameObject *object;
	float facing;
	if (!enabled)
	{
		return;
	}
	if (enterer->As_VehicleGameObj())
	{
		enabled = false;
		if (Get_Int_Parameter("CinematicPowerOff") == 1)
		{
			Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OnID")),false);
			Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OffID")),false);
		}
		scriptname = Get_Parameter("Animation2");
		position = Get_Vector3_Parameter("Animation2Loc");
		facing = Get_Float_Parameter("Animation2Facing");
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"Test_Cinematic",scriptname);
		Commands->Send_Custom_Event(obj,object,10000+Get_Int_Parameter("Animation2Slot"),Get_Int_Parameter("BuildingID"),0);
		Vector3 pos;
		pos.X = xpos;
		pos.Y = ypos;
		pos.Z = zpos+Get_Float_Parameter("ZAdjust");
		Commands->Set_Position(enterer,pos);
	}
}

void JFW_Chrono_Controler_Zone::Timer_Expired(GameObject *obj,int number)
{
	const char *scriptname;
	Vector3 position;
	GameObject *object;
	float facing;
	int num;
	num = Get_Int_Parameter("Activate_TimerNum");
	if (num == number)
	{
		if (activating)
		{
			if (Get_Int_Parameter("CinematicPowerOff") == 1)
			{
				Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OnID")),false);
				Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OffID")),false);
			}
			scriptname = Get_Parameter("Animation1");
			position = Get_Vector3_Parameter("Animation1Loc");
			facing = Get_Float_Parameter("Animation1Facing");
			object = Commands->Create_Object("Invisible_Object",position);
			Commands->Set_Facing(object,facing);
			Commands->Attach_Script(object,"Test_Cinematic",scriptname);
			Commands->Send_Custom_Event(obj,object,10000+Get_Int_Parameter("Animation1Slot"),Get_Int_Parameter("BuildingID"),0);
			enabled = true;
			float timertime;
			int timernumber;
			timertime = Get_Float_Parameter("Chrono_Time");
			timernumber = Get_Int_Parameter("Chrono_TimerNum");
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
	num = Get_Int_Parameter("Spawner_TimerNum");
	if (num == number)
	{
		Commands->Create_Object(Get_Parameter("Spawner_Preset"),Get_Vector3_Parameter("Spawner_Location"));
	}
	num = Get_Int_Parameter("Chrono_TimerNum");
	if ((num == number) && (enabled))
	{
		enabled = false;
		if (Get_Int_Parameter("CinematicPowerOff") == 1)
		{
			Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OnID")),false);
			Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OffID")),false);
		}
		scriptname = Get_Parameter("Animation3");
		position = Get_Vector3_Parameter("Animation3Loc");
		facing = Get_Float_Parameter("Animation3Facing");
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"Test_Cinematic",scriptname);
		Commands->Send_Custom_Event(obj,object,10000+Get_Int_Parameter("Animation3Slot"),Get_Int_Parameter("BuildingID"),0);
	}
}

void JFW_Chrono_Controler_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
	Auto_Save_Variable(&xpos,4,2);
	Auto_Save_Variable(&ypos,4,3);
	Auto_Save_Variable(&zpos,4,4);
	Auto_Save_Variable(&activating,1,5);
}

void JFW_Chrono_Controler_Zone::Killed(GameObject *obj,GameObject *killer)
{
	if (enabled)
	{
		if (Get_Int_Parameter("CinematicPowerOff") == 1)
		{
			Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OnID")),false);
			Commands->Set_Is_Rendered(Commands->Find_Object(Get_Int_Parameter("OffID")),false);
		}
		const char *scriptname;
		Vector3 position;
		GameObject *object;
		float facing;
		scriptname = Get_Parameter("Animation3");
		position = Get_Vector3_Parameter("Animation3Loc");
		facing = Get_Float_Parameter("Animation3Facing");
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"Test_Cinematic",scriptname);
		Commands->Send_Custom_Event(obj,object,10000+Get_Int_Parameter("Animation3Slot"),Get_Int_Parameter("BuildingID"),0);
	}
}

void JFW_Conyard::Created(GameObject *obj)
{
	disabled = false;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Conyard::Killed(GameObject *obj,GameObject *killer)
{
	disabled = true;
}

void JFW_Conyard::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("DisableCustom"))
	{
		disabled = true;
	}
	else if (type == CUSTOM_EVENT_BUILDING_REVIVED)
	{
		disabled = false;
	}
}

void JFW_Conyard::Timer_Expired(GameObject *obj,int number)
{
	if (!disabled)
	{
		Repair_All_Buildings_By_Team(Get_Object_Type(obj),Commands->Get_ID(obj),Get_Float_Parameter("Health"));
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Conyard::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&disabled,1,1);
}

void JFW_Flying_Infantry::Created(GameObject *obj)
{
	Toggle_Fly_Mode(obj);
	Destroy_Script();
}

void JFW_Flying_Infantry_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Toggle_Fly_Mode(obj);
	}
}

void JFW_Flying_Infantry_Powerup::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Toggle_Fly_Mode(sender);
	}
}

void JFW_Flying_Infantry_Enter::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	Toggle_Fly_Mode(enterer);
}

void JFW_Flying_Infantry_Exit::Exited(GameObject *obj,GameObject *exiter)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(exiter,x))
	{
		return;
	}
	Toggle_Fly_Mode(exiter);
}

void JFW_Conyard_Vehicle_Buildings::Created(GameObject *obj)
{
	disabled = false;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Conyard_Vehicle_Buildings::Killed(GameObject *obj,GameObject *killer)
{
	disabled = true;
}

void JFW_Conyard_Vehicle_Buildings::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("DisableCustom"))
	{
		disabled = true;
	}
}

void JFW_Conyard_Vehicle_Buildings::Timer_Expired(GameObject *obj,int number)
{
	if (!disabled)
	{
		Repair_All_Buildings_By_Team(Get_Object_Type(obj),Commands->Get_ID(obj),Get_Float_Parameter("Health"));
		Repair_All_Static_Vehicles_By_Team(Get_Object_Type(obj),Get_Int_Parameter("Message"));
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
	}
}

void JFW_Conyard_Vehicle_Buildings::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&disabled,1,1);
}

void JFW_Drone_Damage::Created(GameObject *obj)
{
	enabled = false;
}

void JFW_Drone_Damage::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if ((!type) && (enabled))
	{
		float health = Commands->Get_Health(obj);
		Commands->Set_Health(obj,health-Get_Int_Parameter("Amount"));
		health = Commands->Get_Shield_Strength(obj);
		Commands->Set_Shield_Strength(obj,health-Get_Int_Parameter("Amount"));
		Commands->Send_Custom_Event(obj,obj,0,0,1.0);
	}
}

void JFW_Drone_Damage::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Drone_Damage::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	GameObject *o = Get_Vehicle(damager);
	if (o)
	{
		damager = o;
	}
	if ((amount < 0) && (Get_Int_Parameter("RepairStop") == 1))
	{
		enabled = false;
	}
	if (!_stricmp(Get_Parameter("Preset"),Commands->Get_Preset_Name(damager)))
	{
		enabled = true;
		Commands->Send_Custom_Event(obj,damager,Get_Int_Parameter("Custom"),0,0);
		Commands->Send_Custom_Event(obj,obj,0,0,0);
	}
}

void JFW_Team_DM_Controller_2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	int maxdeaths;
	msg = Get_Int_Parameter("Death_Message");
	maxdeaths = Get_Int_Parameter("Deaths_To_Loose");
	if (type == msg)
	{
		deaths++;
	}
	if (deaths >= maxdeaths)
	{
		Kill_All_Buildings_By_Team(Get_Int_Parameter("Team"));
	}
}

void JFW_Team_DM_Controller_2::Created(GameObject *obj)
{
	deaths = 0;
}

void JFW_Team_DM_Controller_2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&deaths,4,1);
}

void JFW_Vehicle_Thief::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(damager,team))
	{
		return;
	}
	if (_stricmp(Get_Current_Weapon(damager),Get_Parameter("Weapon")))
	{
		return;
	}
	Force_Occupants_Exit(obj);
}

void JFW_Vehicle_Thief_2::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(damager,team))
	{
		return;
	}
	if (_stricmp(Get_Current_Weapon(damager),Get_Parameter("Weapon")))
	{
		return;
	}
	Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(damager),damager);
	Force_Occupants_Exit(obj);
}

void JFW_Domination_Controler_End_Game::Created(GameObject *obj)
{
	float timertime;
	int timernumber;
	gdizonecount = 0;
	nodzonecount = 0;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_Domination_Controler_End_Game::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int neutralmessage;
	int ownedmessage;
	neutralmessage = Get_Int_Parameter("NeutralMessage");
	ownedmessage = Get_Int_Parameter("OwnedMessage");
	if (type == ownedmessage)
	{
		if (!param)
		{
			gdizonecount--;
		}
		else
		{
			nodzonecount--;
		}
	}
	if ((type == neutralmessage) || (type == ownedmessage))
	{
		if (param == 1)
		{
			gdizonecount++;
		}
		else
		{
			nodzonecount++;
		}
	}
	if (gdizonecount == Get_Int_Parameter("ZoneCount"))
	{
		Kill_All_Buildings_By_Team(0);
	}
	if (nodzonecount == Get_Int_Parameter("ZoneCount"))
	{
		Kill_All_Buildings_By_Team(1);
	}
}

void JFW_Domination_Controler_End_Game::Timer_Expired(GameObject *obj,int number)
{
	float timertime;
	int timernumber;
	float pointstogive;
	float pointstotake;
	GameObject *gdiobject;
	GameObject *nodobject;
	gdiobject = Commands->Find_Object(Get_Int_Parameter("GDIObjectID"));
	nodobject = Commands->Find_Object(Get_Int_Parameter("NodObjectID"));
	pointstogive = Get_Float_Parameter("PointsToGive");
	pointstotake = Get_Float_Parameter("PointsToTake");
	if (gdizonecount > 0)
	{
		if (pointstogive)
		{
			Commands->Give_Points(gdiobject,pointstogive*gdizonecount,true);
		}
		if (pointstotake)
		{
			Commands->Give_Points(nodobject,-(pointstotake*gdizonecount),true);
		}
	}
	if (nodzonecount > 0)
	{
		if (pointstogive)
		{
			Commands->Give_Points(nodobject,pointstogive*nodzonecount,true);
		}
		if (pointstotake)
		{
			Commands->Give_Points(gdiobject,-(pointstotake*nodzonecount),true);
		}
	}
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_Domination_Controler_End_Game::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&gdizonecount,4,1);
	Auto_Save_Variable(&nodzonecount,4,2);
}

void JFW_Carryable_Vehicle::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	int onlyempty;
	onlyempty = Get_Int_Parameter("Only_Empty");
	if ((onlyempty == 1 && !Get_Vehicle_Occupant_Count(obj)) || (!onlyempty))
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("Control_ID")),9876,Commands->Get_ID(damager),0);
	}
}

void JFW_Carryable_Vehicle::Killed(GameObject *obj,GameObject *killer)
{
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("Control_ID")),9875,Commands->Get_ID(killer),0);
}

void JFW_CarryAll::Created(GameObject *obj)
{
	pilotID = 0;
	carrying = false;
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("Control_ID")),9877,6,0);
}

void JFW_CarryAll::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if ((type == CUSTOM_EVENT_VEHICLE_ENTERED) && (!pilotID))
	{
		pilotID = Commands->Get_ID(sender);
	}
	if ((type == CUSTOM_EVENT_VEHICLE_EXITED) && (Commands->Get_ID(sender) == pilotID))
	{
		pilotID = 0;
	}
	if (type == 9876 && param == pilotID && !carrying)
	{
		carrying = true;
		id = Commands->Get_ID(sender);
		Commands->Set_Position(sender,Commands->Get_Bone_Position(obj,Get_Parameter("Bone_Name")));
		Commands->Attach_To_Object_Bone(sender,obj,Get_Parameter("Bone_Name"));
	}
	if (type == 9877 && param == 7)
	{
		if (pilotID)
		{
			Commands->Give_Money(Commands->Find_Object(pilotID),Get_Float_Parameter("Cost"),false);
		}
		Commands->Destroy_Object(obj);
	}
	if (type == 9875 && carrying && id == Commands->Get_ID(sender))
	{
		carrying = false;
	}
}

void JFW_CarryAll::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (carrying && damager == Commands->Find_Object(pilotID))
	{
		float facing = Commands->Get_Facing(Commands->Find_Object(id));
		float health = Commands->Get_Health(Commands->Find_Object(id));
		float shield = Commands->Get_Shield_Strength(Commands->Find_Object(id));
		const char *pn = Commands->Get_Preset_Name(Commands->Find_Object(id));
		Commands->Destroy_Object(Commands->Find_Object(id));
		GameObject *object;
		object = Commands->Create_Object(pn,Commands->Get_Bone_Position(obj,Get_Parameter("Bone_Name")));
		Commands->Set_Health(object,health);
		Commands->Set_Shield_Strength(object,shield);
		Commands->Set_Facing(object,facing);
		carrying = false;
	}
}

void JFW_CarryAll::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
	Auto_Save_Variable(&carrying,1,2);
	Auto_Save_Variable(&id,4,3);
	Auto_Save_Variable(&pilotID,4,4);
}

void JFW_CarryAll::Killed(GameObject *obj,GameObject *killer)
{
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("Control_ID")),9877,7,0);
}

void JFW_Carry_Control::Created(GameObject *obj)
{
	carryall1 = 0;
	carryall2 = 0;
	carryall3 = 0;
	carryall4 = 0;
	carryall5 = 0;
	carryall6 = 0;
	carryall7 = 0;
	carryall8 = 0;
	carryall9 = 0;
	carryall10 = 0;
	carryall11 = 0;
	carryall12 = 0;
	carryall13 = 0;
	carryall14 = 0;
	carryall15 = 0;
	carryall16 = 0;
	carryall17 = 0;
	carryall18 = 0;
	carryall19 = 0;
	carryall20 = 0;
	carrycount = 0;
}

void JFW_Carry_Control::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 9877 && param == 6)
	{
		carrycount +=1;
		if (carrycount > Get_Int_Parameter("Max_Carryalls"))
		{
			Commands->Send_Custom_Event(obj,sender,9877,7,0);
		}
		if (!carryall1 && Get_Int_Parameter("Max_Carryalls") >=1)
		{
			carryall1 = Commands->Get_ID(sender);
			return;
		}
		if (carryall1 && !carryall2 && Get_Int_Parameter("Max_Carryalls") >=2)
		{
			carryall2 = Commands->Get_ID(sender);
			return;
		}
		if (carryall1 && carryall2 && !carryall3 && Get_Int_Parameter("Max_Carryalls") >=3)
		{
			carryall3 = Commands->Get_ID(sender);
			return;
		}
		if (carryall1 && carryall2 && carryall3 && !carryall4 && Get_Int_Parameter("Max_Carryalls") >=4)
		{
			carryall4 = Commands->Get_ID(sender);
			return;
		}
		if (carryall1 && carryall2 && carryall3 && carryall4 && !carryall5 && Get_Int_Parameter("Max_Carryalls") >=5)
		{
			carryall5 = Commands->Get_ID(sender);
			return;
		}
		if (carryall1 && carryall2 && carryall3 && carryall4 && carryall5 && !carryall6 && Get_Int_Parameter("Max_Carryalls") >=6)
		{
			carryall6 = Commands->Get_ID(sender);
			return;
		}
		if (carryall1 && carryall2 && carryall3 && carryall4 && carryall5 && carryall6 && !carryall7 && Get_Int_Parameter("Max_Carryalls") >=7)
		{
			carryall7 = Commands->Get_ID(sender);
			return;
		}
		if (carryall1 && carryall2 && carryall3 && carryall4 && carryall5 && carryall6 && carryall7 && !carryall8 && Get_Int_Parameter("Max_Carryalls") >=8)
		{
			carryall8 = Commands->Get_ID(sender);
			return;
		}
		if (carryall1 && carryall2 && carryall3 && carryall4 && carryall5 && carryall6 && carryall7 && carryall8 && !carryall9 && Get_Int_Parameter("Max_Carryalls") >=9)
		{
			carryall9 = Commands->Get_ID(sender);
			return;
		}
		if (carryall1 && carryall2 && carryall3 && carryall4 && carryall5 && carryall6 && carryall7 && carryall8 && carryall9 && !carryall10 && Get_Int_Parameter("Max_Carryalls") >=10)
		{
			carryall10 = Commands->Get_ID(sender);
			return;
		}
		if (carryall1 && carryall2 && carryall3 && carryall4 && carryall5 && carryall6 && carryall7 && carryall8 && carryall9 && carryall10 && !carryall11 && Get_Int_Parameter("Max_Carryalls") >=11)
		{
			carryall11 = Commands->Get_ID(sender);
			return;
		}
		if (carryall1 && carryall2 && carryall3 && carryall4 && carryall5 && carryall6 && carryall7 && carryall8 && carryall9 && carryall10 && carryall11 && !carryall12 && Get_Int_Parameter("Max_Carryalls") >=12)
		{
			carryall12 = Commands->Get_ID(sender);
			return;
		}
		if (carryall1 && carryall2 && carryall3 && carryall4 && carryall5 && carryall6 && carryall7 && carryall8 && carryall9 && carryall10 && carryall11 && carryall12 && !carryall13 && Get_Int_Parameter("Max_Carryalls") >=13)
		{
			carryall13 = Commands->Get_ID(sender);
			return;
		}
		if (carryall1 && carryall2 && carryall3 && carryall4 && carryall5 && carryall6 && carryall7 && carryall8 && carryall9 && carryall10 && carryall11 && carryall12 && carryall13 && !carryall14 && Get_Int_Parameter("Max_Carryalls") >=14)
		{
			carryall14 = Commands->Get_ID(sender);
			return;
		}
		if (carryall1 && carryall2 && carryall3 && carryall4 && carryall5 && carryall6 && carryall7 && carryall8 && carryall9 && carryall10 && carryall11 && carryall12 && carryall13 && carryall14 && !carryall15 && Get_Int_Parameter("Max_Carryalls") >=15)
		{
			carryall15 = Commands->Get_ID(sender);
			return;
		}
		if (carryall1 && carryall2 && carryall3 && carryall4 && carryall5 && carryall6 && carryall7 && carryall8 && carryall9 && carryall10 && carryall11 && carryall12 && carryall13 && carryall14 && carryall15 && !carryall16 && Get_Int_Parameter("Max_Carryalls") >=16)
		{
			carryall16 = Commands->Get_ID(sender);
			return;
		}
		if (carryall1 && carryall2 && carryall3 && carryall4 && carryall5 && carryall6 && carryall7 && carryall8 && carryall9 && carryall10 && carryall11 && carryall12 && carryall13 && carryall14 && carryall15 && carryall16 && !carryall17 && Get_Int_Parameter("Max_Carryalls") >=17)
		{
			carryall17 = Commands->Get_ID(sender);
			return;
		}
		if (carryall1 && carryall2 && carryall3 && carryall4 && carryall5 && carryall6 && carryall7 && carryall8 && carryall9 && carryall10 && carryall11 && carryall12 && carryall13 && carryall14 && carryall15 && carryall16 && carryall17 && !carryall18 && Get_Int_Parameter("Max_Carryalls") >=18)
		{
			carryall18 = Commands->Get_ID(sender);
			return;
		}
		if (carryall1 && carryall2 && carryall3 && carryall4 && carryall5 && carryall6 && carryall7 && carryall8 && carryall9 && carryall10 && carryall11 && carryall12 && carryall13 && carryall14 && carryall15 && carryall16 && carryall17 && carryall18 && !carryall19 && Get_Int_Parameter("Max_Carryalls") >=19)
		{
			carryall19 = Commands->Get_ID(sender);
			return;
		}
		if (carryall1 && carryall2 && carryall3 && carryall4 && carryall5 && carryall6 && carryall7 && carryall8 && carryall9 && carryall10 && carryall11 && carryall12 && carryall13 && carryall14 && carryall15 && carryall16 && carryall17 && carryall18 && carryall19 && !carryall20 && Get_Int_Parameter("Max_Carryalls") >=20)
		{
			carryall20 = Commands->Get_ID(sender);
			return;
		}
	}
	if (type == 9877 && param == 7)
	{
		carrycount -= 1;
		if (carryall1 == Commands->Get_ID(sender))
		{
			carryall1 = 0;
		}
		if (carryall2 == Commands->Get_ID(sender))
		{
			carryall2 = 0;
		}
		if (carryall3 == Commands->Get_ID(sender))
		{
			carryall3 = 0;
		}
		if (carryall4 == Commands->Get_ID(sender))
		{
			carryall4 = 0;
		}
		if (carryall5 == Commands->Get_ID(sender))
		{
			carryall5 = 0;
		}
		if (carryall6 == Commands->Get_ID(sender))
		{
			carryall6 = 0;
		}
		if (carryall7 == Commands->Get_ID(sender))
		{
			carryall7 = 0;
		}
		if (carryall8 == Commands->Get_ID(sender))
		{
			carryall8 = 0;
		}
		if (carryall9 == Commands->Get_ID(sender))
		{
			carryall9 = 0;
		}
		if (carryall10 == Commands->Get_ID(sender))
		{
			carryall10 = 0;
		}
		if (carryall11 == Commands->Get_ID(sender))
		{
			carryall11 = 0;
		}
		if (carryall12 == Commands->Get_ID(sender))
		{
			carryall12 = 0;
		}
		if (carryall13 == Commands->Get_ID(sender))
		{
			carryall13 = 0;
		}
		if (carryall14 == Commands->Get_ID(sender))
		{
			carryall14 = 0;
		}
		if (carryall15 == Commands->Get_ID(sender))
		{
			carryall15 = 0;
		}
		if (carryall16 == Commands->Get_ID(sender))
		{
			carryall16 = 0;
		}
		if (carryall17 == Commands->Get_ID(sender))
		{
			carryall17 = 0;
		}
		if (carryall18 == Commands->Get_ID(sender))
		{
			carryall18 = 0;
		}
		if (carryall19 == Commands->Get_ID(sender))
		{
			carryall19 = 0;
		}
		if (carryall20 == Commands->Get_ID(sender))
		{
			carryall20 = 0;
		}
	}
	if (type == 9876)
	{
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall1),9876,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall2),9876,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall3),9876,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall4),9876,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall5),9876,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall6),9876,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall7),9876,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall8),9876,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall9),9876,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall10),9876,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall11),9876,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall12),9876,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall13),9876,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall14),9876,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall15),9876,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall16),9876,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall17),9876,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall18),9876,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall19),9876,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall20),9876,param,0);
		
	}
	if (type == 9875)
	{
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall1),9875,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall2),9875,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall3),9875,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall4),9875,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall5),9875,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall6),9875,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall7),9875,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall8),9875,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall9),9875,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall10),9875,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall11),9875,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall12),9875,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall13),9875,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall14),9875,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall15),9875,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall16),9875,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall17),9875,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall18),9875,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall19),9875,param,0);
		Commands->Send_Custom_Event(sender,Commands->Find_Object(carryall20),9875,param,0);
		
	}
}

void JFW_Carry_Control::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&carryall1,4,1);
	Auto_Save_Variable(&carryall2,4,2);
	Auto_Save_Variable(&carryall3,4,3);
	Auto_Save_Variable(&carryall4,4,4);
	Auto_Save_Variable(&carryall5,4,5);
	Auto_Save_Variable(&carryall6,4,6);
	Auto_Save_Variable(&carryall7,4,7);
	Auto_Save_Variable(&carryall8,4,8);
	Auto_Save_Variable(&carryall9,4,9);
	Auto_Save_Variable(&carryall10,4,10);
	Auto_Save_Variable(&carryall11,4,11);
	Auto_Save_Variable(&carryall12,4,12);
	Auto_Save_Variable(&carryall13,4,13);
	Auto_Save_Variable(&carryall14,4,14);
	Auto_Save_Variable(&carryall15,4,15);
	Auto_Save_Variable(&carryall16,4,16);
	Auto_Save_Variable(&carryall17,4,17);
	Auto_Save_Variable(&carryall18,4,18);
	Auto_Save_Variable(&carryall19,4,19);
	Auto_Save_Variable(&carryall20,4,20);
}

void JFW_Sensor_Array::Created(GameObject *obj) 
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Sensor_Array::Timer_Expired(GameObject *obj,int number)
{
	if (number == Get_Int_Parameter("TimerNum"))
	{
		Vector3 v;
		v.X = 0;
		v.Y = 0;
		v.Z = Get_Float_Parameter("IndicatorZOffset");
		Create_Effect_All_Stealthed_Objects_Area(Commands->Get_Position(obj),Get_Float_Parameter("Distance"),Get_Parameter("Indicator"),v,Get_Int_Parameter("Player_Type"));
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
	}
}

void JFW_Conyard_Radius::Created(GameObject *obj)
{
	disabled = false;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Conyard_Radius::Killed(GameObject *obj,GameObject *killer)
{
	disabled = true;
}

void JFW_Conyard_Radius::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("DisableCustom"))
	{
		disabled = true;
	}
}

void JFW_Conyard_Radius::Timer_Expired(GameObject *obj,int number)
{
	if (!disabled)
	{
		Repair_All_Buildings_By_Team_Radius(Get_Object_Type(obj),Commands->Get_ID(obj),Get_Float_Parameter("Health"),Get_Float_Parameter("Radius"));
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
	}
}

void JFW_Conyard_Radius::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&disabled,1,1);
}

void JFW_Chrono_Harvester_Logic::Created(GameObject *obj)
{
	enabled = true;
	HarvyID = 0;
	Commands->Send_Custom_Event(obj,obj,100001,0,0);
}

void JFW_Chrono_Harvester_Logic::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if ((type == 100001) && (enabled))
	{
		float facing = Commands->Get_Facing(obj);
		Commands->Create_Explosion(Get_Parameter("Explosion_Preset"),Commands->Get_Position(obj),0);
		GameObject *Harvy = Commands->Create_Object(Get_Parameter("Harvester_Preset"),Commands->Get_Position(obj));
		Commands->Set_Facing(Harvy,facing);
		Commands->Send_Custom_Event(obj,Harvy,100004,(int &)facing,0);
	}
	else if (type == 100002)
	{
		if (HarvyID)
		{
			Commands->Apply_Damage(Commands->Find_Object(HarvyID),99999,"Explosive",0);
			Commands->Destroy_Object(obj);
		}
	}
	else if (type == 100003)
	{
		HarvyID = param;
	}
	else if (type == 100005)
	{
		enabled = false;
	}
	else if (type == 100006)
	{
		HarvyID = 0;
	}
}

void JFW_Chrono_Harvester_Logic::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&HarvyID,4,1);
	Auto_Save_Variable(&enabled,1,2);
}

void JFW_Chrono_Harvester_Attached::Created(GameObject *obj)
{
	End_Timer = false;
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("LogicID")),100003,Commands->Get_ID(obj),0);
	if (Get_Int_Parameter("Number_Of_Paths") > 3) PathNumber = Commands->Get_Random_Int(1,4);
	else PathNumber = Commands->Get_Random_Int(1,(Get_Int_Parameter("Number_Of_Paths") + 1));
	Vector3 CrystalRelPos = Vector3(0,0,0);
	Vector3 CrystalPos;
	ActionParamsStruct HarvesterToField;
	HarvesterToField.MoveArrivedDistance = 100;
	HarvesterToField.Set_Basic(this,100,1);
	if (PathNumber == 1)
	{
		HarvesterToField.Set_Movement(0,1.0f,0.1f);
		HarvesterToField.WaypathID = Get_Int_Parameter("Waypath1_Dock2Field");
		HarvesterToField.WaypathSplined = true;
		HarvesterToField.AttackActive = false;
		HarvesterToField.AttackCheckBlocked = false;
		CrystalRelPos = Get_Vector3_Parameter("Waypath1_Tib_Zone");
	}
	else if (PathNumber == 2)
	{
		HarvesterToField.Set_Movement(0,1.0f,0.1f);
		HarvesterToField.WaypathID = Get_Int_Parameter("Waypath2_Dock2Field");
		HarvesterToField.WaypathSplined = true;
		HarvesterToField.AttackActive = false;
		HarvesterToField.AttackCheckBlocked = false;
		CrystalRelPos = Get_Vector3_Parameter("Waypath2_Tib_Zone");
	}
	else if (PathNumber == 3)
	{
		HarvesterToField.Set_Movement(0,1.0f,0.1f);
		HarvesterToField.WaypathID = Get_Int_Parameter("Waypath3_Dock2Field");
		HarvesterToField.WaypathSplined = true;
		HarvesterToField.AttackActive = false;
		HarvesterToField.AttackCheckBlocked = false;
		CrystalRelPos = Get_Vector3_Parameter("Waypath3_Tib_Zone");
	}
	if (_stricmp(Get_Parameter("Crystal_Preset"),"0"))
	{
		for (int x = 1;x <= 10;x++)
		{
			int XChange,YChange;
			CrystalPos = CrystalRelPos;
			XChange = Commands->Get_Random_Int(1,3);
			YChange = Commands->Get_Random_Int(1,3);
			if (XChange == 1) CrystalPos.X += Commands->Get_Random(0.0f,5.001f);
			else if (XChange == 2) CrystalPos.X -= Commands->Get_Random(0.0f,5.001f);
			if (YChange == 1) CrystalPos.Y += Commands->Get_Random(0.0f,5.001f);
			else if (YChange == 2) CrystalPos.Y -= Commands->Get_Random(0.0f,5.001f);
			if (x == 1)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal1 = Commands->Get_ID(Crystal);
			}
			else if (x == 2)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal2 = Commands->Get_ID(Crystal);
			}
			else if (x == 3)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal3 = Commands->Get_ID(Crystal);
			}
			else if (x == 4)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal4 = Commands->Get_ID(Crystal);
			}
			else if (x == 5)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal5 = Commands->Get_ID(Crystal);
			}
			else if (x == 6)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal6 = Commands->Get_ID(Crystal);
			}
			else if (x == 7)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal7 = Commands->Get_ID(Crystal);
			}
			else if (x == 8)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal8 = Commands->Get_ID(Crystal);
			}
			else if (x == 9)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal9 = Commands->Get_ID(Crystal);
			}
			else if (x == 10)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal10 = Commands->Get_ID(Crystal);
			}
		}
	}
	Commands->Action_Goto(obj,HarvesterToField);
	Commands->Start_Timer(obj,this,0.1f,5);
}

void JFW_Chrono_Harvester_Attached::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	if (action_id == 1)
	{
		if (!complete_reason) 
		{
			const char *harvest_animation = Get_Parameter("Harvest_Animation");
			if (_stricmp(harvest_animation,"0"))
			{
				Commands->Set_Animation(obj,harvest_animation,true,0,0.0f,-1.0f,false);
			}
		}
		Commands->Start_Timer(obj,this,(Get_Float_Parameter("Tiberium_Idle_Time") / 10),1);
		Commands->Start_Timer(obj,this,(Get_Float_Parameter("Tiberium_Idle_Time") / 10),4);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Tiberium_Idle_Time"),2);
	}
	else if (action_id == 2)
	{
		if (!complete_reason) 
		{
			const char *harvest_animation = Get_Parameter("Harvest_Animation");
			if (_stricmp(harvest_animation,"0"))
			{
				Commands->Set_Animation(obj,harvest_animation,true,0,0.0f,-1.0f,false);
			}
		}
		Commands->Start_Timer(obj,this,10.0f,1);
	}
	else if (action_id == 3)
	{
		ActionParamsStruct HarvesterDock;
		HarvesterDock.MoveArrivedDistance = 100;
		HarvesterDock.Set_Basic(this,100,4);
		HarvesterDock.Dock_Vehicle(Get_Vector3_Parameter("Dock_Location"),Commands->Get_Position(obj));
		HarvesterDock.Set_Movement(Get_Vector3_Parameter("Dock_Location"),1.0f,0.5f,false);
		Commands->Action_Dock(obj,HarvesterDock);
	}
	else if (action_id == 4)
	{
		if (!complete_reason) 
		{
			Commands->Set_Facing(obj,facing);
			const char *dock_animation = Get_Parameter("Dock_Animation");
			if (_stricmp(dock_animation,"0"))
			{
				Commands->Set_Animation(obj,dock_animation,false,0,0.0f,-1.0f,false);
			}
		}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Dock_Idle_Time"),3);
	}
}

void JFW_Chrono_Harvester_Attached::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 100004)
	{
		facing = (float &)param;
	}
}

void JFW_Chrono_Harvester_Attached::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && !End_Timer)
	{
		const char *harvest_animation = Get_Parameter("Harvest_Animation");
		if (_stricmp(harvest_animation,"0"))
		{
			Commands->Set_Animation(obj,harvest_animation,false,0,0.0f,-1.0f,false);
		}
		Vector3 Root_Position = Vector3(0,0,0);
		if (PathNumber == 1)
		{
			Root_Position = Get_Vector3_Parameter("Waypath1_Tib_Zone");
		}
		else if (PathNumber == 2)
		{
			Root_Position = Get_Vector3_Parameter("Waypath2_Tib_Zone");
		}
		else if (PathNumber == 3)
		{
			Root_Position = Get_Vector3_Parameter("Waypath3_Tib_Zone");
		}
		int X_Level = Commands->Get_Random_Int(1,3);
		if (X_Level == 1) Root_Position.X += Commands->Get_Random(0.0f,2.001f);
		else if (X_Level == 2) Root_Position.X -= Commands->Get_Random(0.0f,2.001f);
		int Y_Level = Commands->Get_Random_Int(1,3);
		if (Y_Level == 1) Root_Position.Y += Commands->Get_Random(0.0f,2.001f);
		else if (Y_Level == 2) Root_Position.Y -= Commands->Get_Random(0.0f,2.001f);
		ActionParamsStruct HarvesterTib;
		HarvesterTib.MoveArrivedDistance = 100;
		HarvesterTib.Set_Basic(this,100,2);
		HarvesterTib.Set_Movement(Root_Position,1.0f,0.1f,false);
		Commands->Action_Goto(obj,HarvesterTib);
	}
	else if (number == 2)
	{
		End_Timer = true;
		const char *harvest_animation = Get_Parameter("Harvest_Animation");
		if (_stricmp(harvest_animation,"0"))
		{
			Commands->Set_Animation(obj,harvest_animation,false,0,0.0f,-1.0f,false);
		}
		Commands->Set_Position(obj,Get_Vector3_Parameter("Chrono_Location"));
		ActionParamsStruct HarvesterReturn;
		HarvesterReturn.MoveArrivedDistance = 100;
		HarvesterReturn.Set_Basic(this,100,3);
		if (PathNumber == 1)
		{
			HarvesterReturn.Set_Movement(0,1.0f,0.1f);
			HarvesterReturn.WaypathID = Get_Int_Parameter("Waypath1_Field2Dock");
			HarvesterReturn.WaypathSplined = true;
			HarvesterReturn.AttackActive = false;
			HarvesterReturn.AttackCheckBlocked = false;
		}
		else if (PathNumber == 2)
		{
			HarvesterReturn.Set_Movement(0,1.0f,0.1f);
			HarvesterReturn.WaypathID = Get_Int_Parameter("Waypath2_Field2Dock");
			HarvesterReturn.WaypathSplined = true;
			HarvesterReturn.AttackActive = false;
			HarvesterReturn.AttackCheckBlocked = false;
		}
		else if (PathNumber == 3)
		{
			HarvesterReturn.Set_Movement(0,1.0f,0.1f);
			HarvesterReturn.WaypathID = Get_Int_Parameter("Waypath3_Field2Dock");
			HarvesterReturn.WaypathSplined = true;
			HarvesterReturn.AttackActive = false;
			HarvesterReturn.AttackCheckBlocked = false;
		}
		Commands->Action_Goto(obj,HarvesterReturn);
	}
	else if (number == 3)
	{
		End_Timer = false;
		Commands->Give_Money(obj,Get_Float_Parameter("Dock_Credits"),true);
		if (Get_Int_Parameter("Number_Of_Paths") > 3) PathNumber = Commands->Get_Random_Int(1,4);
		else PathNumber = Commands->Get_Random_Int(1,(Get_Int_Parameter("Number_Of_Paths") + 1));
		Vector3 CrystalRelPos = Vector3(0,0,0);
		Vector3 CrystalPos;
		ActionParamsStruct HarvesterReturnToField;
		HarvesterReturnToField.MoveArrivedDistance = 100;
		HarvesterReturnToField.Set_Basic(this,100,1);
		if (PathNumber == 1)
		{
			HarvesterReturnToField.Set_Movement(0,1.0f,0.1f);
			HarvesterReturnToField.WaypathID = Get_Int_Parameter("Waypath1_Dock2Field");
			HarvesterReturnToField.WaypathSplined = true;
			HarvesterReturnToField.AttackActive = false;
			HarvesterReturnToField.AttackCheckBlocked = false;
			CrystalRelPos = Get_Vector3_Parameter("Waypath1_Tib_Zone");
		}
		else if (PathNumber == 2)
		{
			HarvesterReturnToField.Set_Movement(0,1.0f,0.1f);
			HarvesterReturnToField.WaypathID = Get_Int_Parameter("Waypath2_Dock2Field");
			HarvesterReturnToField.WaypathSplined = true;
			HarvesterReturnToField.AttackActive = false;
			HarvesterReturnToField.AttackCheckBlocked = false;
			CrystalRelPos = Get_Vector3_Parameter("Waypath2_Tib_Zone");
		}
		else if (PathNumber == 3)
		{
			HarvesterReturnToField.Set_Movement(0,1.0f,0.1f);
			HarvesterReturnToField.WaypathID = Get_Int_Parameter("Waypath3_Dock2Field");
			HarvesterReturnToField.WaypathSplined = true;
			HarvesterReturnToField.AttackActive = false;
			HarvesterReturnToField.AttackCheckBlocked = false;
			CrystalRelPos = Get_Vector3_Parameter("Waypath3_Tib_Zone");
		}
		if (_stricmp(Get_Parameter("Crystal_Preset"),"0"))
		{
			for (int x = 1;x <= 10;x++)
			{
				int XChange,YChange;
				CrystalPos = CrystalRelPos;
				XChange = Commands->Get_Random_Int(1,3);
				YChange = Commands->Get_Random_Int(1,3);
				if (XChange == 1) CrystalPos.X += Commands->Get_Random(0.0f,5.001f);
				else if (XChange == 2) CrystalPos.X -= Commands->Get_Random(0.0f,5.001f);
				if (YChange == 1) CrystalPos.Y += Commands->Get_Random(0.0f,5.001f);
				else if (YChange == 2) CrystalPos.Y -= Commands->Get_Random(0.0f,5.001f);
				if (x == 1)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal1 = Commands->Get_ID(Crystal);
				}
				else if (x == 2)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal2 = Commands->Get_ID(Crystal);
				}
				else if (x == 3)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal3 = Commands->Get_ID(Crystal);
				}
				else if (x == 4)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal4 = Commands->Get_ID(Crystal);
				}
				else if (x == 5)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal5 = Commands->Get_ID(Crystal);
				}
				else if (x == 6)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal6 = Commands->Get_ID(Crystal);
				}
				else if (x == 7)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal7 = Commands->Get_ID(Crystal);
				}
				else if (x == 8)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal8 = Commands->Get_ID(Crystal);
				}
				else if (x == 9)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal9 = Commands->Get_ID(Crystal);
				}
				else if (x == 10)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal10 = Commands->Get_ID(Crystal);
				}
			}
		}
		Commands->Action_Goto(obj,HarvesterReturnToField);
	}
	else if (number == 4)
	{
		if (_stricmp(Get_Parameter("Crystal_Preset"),"0"))
		{
			if (CrystalNumber == 1)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal1));
			}
			else if (CrystalNumber == 2)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal2));
			}
			else if (CrystalNumber == 3)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal3));
			}
			else if (CrystalNumber == 4)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal4));
			}
			else if (CrystalNumber == 5)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal5));
			}
			else if (CrystalNumber == 6)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal6));
			}
			else if (CrystalNumber == 7)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal7));
			}
			else if (CrystalNumber == 8)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal8));
			}
			else if (CrystalNumber == 9)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal9));
			}
			else if (CrystalNumber == 10)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal10));
			}
			CrystalNumber++;
			if (CrystalNumber <= 10)
			{
				Commands->Start_Timer(obj,this,(Get_Float_Parameter("Tiberium_Idle_Time") / 10),4);
			}
			else
			{
				CrystalNumber = 1;
			}
		}
	}
	else if (number == 5)
	{
		CrystalNumber = 1;
	}
}

void JFW_Chrono_Harvester_Attached::Killed(GameObject *obj,GameObject *killer)
{
	if (Commands->Get_ID(Commands->Find_Object(Crystal1))) Commands->Destroy_Object(Commands->Find_Object(Crystal1));
	if (Commands->Get_ID(Commands->Find_Object(Crystal2))) Commands->Destroy_Object(Commands->Find_Object(Crystal2));
	if (Commands->Get_ID(Commands->Find_Object(Crystal3))) Commands->Destroy_Object(Commands->Find_Object(Crystal3));
	if (Commands->Get_ID(Commands->Find_Object(Crystal4))) Commands->Destroy_Object(Commands->Find_Object(Crystal4));
	if (Commands->Get_ID(Commands->Find_Object(Crystal5))) Commands->Destroy_Object(Commands->Find_Object(Crystal5));
	if (Commands->Get_ID(Commands->Find_Object(Crystal6))) Commands->Destroy_Object(Commands->Find_Object(Crystal6));
	if (Commands->Get_ID(Commands->Find_Object(Crystal7))) Commands->Destroy_Object(Commands->Find_Object(Crystal7));
	if (Commands->Get_ID(Commands->Find_Object(Crystal8))) Commands->Destroy_Object(Commands->Find_Object(Crystal8));
	if (Commands->Get_ID(Commands->Find_Object(Crystal9))) Commands->Destroy_Object(Commands->Find_Object(Crystal9));
	if (Commands->Get_ID(Commands->Find_Object(Crystal10))) Commands->Destroy_Object(Commands->Find_Object(Crystal10));
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("LogicID")),100006,0,0);
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("LogicID")),100001,0,Get_Float_Parameter("Harvester_Create_Idle_Time"));
}

void JFW_Chrono_Harvester_Attached::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&PathNumber,4,1);
	Auto_Save_Variable(&Crystal1,4,2);
	Auto_Save_Variable(&Crystal2,4,3);
	Auto_Save_Variable(&Crystal3,4,4);
	Auto_Save_Variable(&Crystal4,4,5);
	Auto_Save_Variable(&Crystal5,4,6);
	Auto_Save_Variable(&Crystal6,4,7);
	Auto_Save_Variable(&Crystal7,4,8);
	Auto_Save_Variable(&Crystal8,4,9);
	Auto_Save_Variable(&Crystal9,4,10);
	Auto_Save_Variable(&Crystal10,4,11);
	Auto_Save_Variable(&End_Timer,1,12);
	Auto_Save_Variable(&CrystalNumber,4,13);
	Auto_Save_Variable(&facing,4,14);
}

void JFW_Chrono_Harvester_Kill::Killed(GameObject *obj,GameObject *killer)
{
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("LogicID")),100002,0,0);
}

void JFW_Chrono_Harvester_Spawn_Kill::Killed(GameObject *obj,GameObject *killer)
{
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("LogicID")),100005,0,0);
}

void JFW_Conyard_Turrets::Created(GameObject *obj)
{
	disabled = false;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Conyard_Turrets::Killed(GameObject *obj,GameObject *killer)
{
	disabled = true;
}

void JFW_Conyard_Turrets::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("DisableCustom"))
	{
		disabled = true;
	}
	else if (type == CUSTOM_EVENT_BUILDING_REVIVED)
	{
		disabled = false;
	}
}

void JFW_Conyard_Turrets::Timer_Expired(GameObject *obj,int number)
{
	if (!disabled)
	{
		Repair_All_Buildings_By_Team(Get_Object_Type(obj),Commands->Get_ID(obj),Get_Float_Parameter("Health"));
		Repair_All_Turrets_By_Team(Get_Object_Type(obj),Get_Float_Parameter("Health"));
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Conyard_Turrets::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&disabled,1,1);
}

void JFW_Radar_Jammer::Created(GameObject *obj)
{
	radaron = true;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Radar_Jammer::Killed(GameObject *obj,GameObject *killer)
{
	Destroy_Script();
}

void JFW_Radar_Jammer::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("DisableCustom"))
	{
		Destroy_Script();
	}
}

void JFW_Radar_Jammer::Timer_Expired(GameObject *obj,int number)
{
	if (number == Get_Int_Parameter("TimerNum"))
	{
		if (Is_Base_Powered(Get_Object_Type(obj)))
		{
			Vector3 v = Commands->Get_Position(Find_Object_By_Preset(2,Get_Definition_Name(Get_Int_Parameter("CenterID"))));
			if (Is_Unit_In_Range(Get_Parameter("Preset"),Get_Float_Parameter("Range"),v,PTTEAM(Get_Object_Type(obj)),false))
			{
				if (radaron)
				{
					Create_2D_Sound_Team(Get_Parameter("StartSound1"),Get_Object_Type(obj));
					Create_2D_Sound_Team(Get_Parameter("StartSound2"),PTTEAM(Get_Object_Type(obj)));
					radaron = false;
					Enable_Base_Radar(Get_Object_Type(obj),false);
				}
			}
			else
			{
				if (!radaron)
				{
					Create_2D_Sound_Team(Get_Parameter("StopSound1"),Get_Object_Type(obj));
					Create_2D_Sound_Team(Get_Parameter("StopSound2"),PTTEAM(Get_Object_Type(obj)));
					radaron = true;
					Enable_Base_Radar(Get_Object_Type(obj),true);
				}
			}
		}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
	}
}

void JFW_Radar_Jammer_Sound::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Radar_Jammer_Sound::Killed(GameObject *obj,GameObject *killer)
{
	Destroy_Script();
}

void JFW_Radar_Jammer_Sound::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("DisableCustom"))
	{
		Destroy_Script();
	}
}

void JFW_Radar_Jammer_Sound::Timer_Expired(GameObject *obj,int number)
{
	if (number == Get_Int_Parameter("TimerNum"))
	{
		if (Is_Base_Powered(Get_Object_Type(obj)))
		{
			Vector3 v = Commands->Get_Position(Find_Object_By_Preset(2,Get_Definition_Name(Get_Int_Parameter("CenterID"))));
			if (Is_Unit_In_Range(Get_Parameter("Preset"),Get_Float_Parameter("Range"),v,PTTEAM(Get_Object_Type(obj)),false))
			{
				Create_2D_Sound_Team(Get_Parameter("Sound"),Get_Object_Type(obj));
			}
		}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
	}
}

void JFW_Sonar_Pulse::Created(GameObject *obj)
{
	reset = false;
}

void JFW_Sonar_Pulse::Entered(GameObject *obj,GameObject *enterer)
{
	if (Get_Vehicle(enterer) || reset)
	{
		return;
	}
	if (!Is_Script_Attached(enterer,Get_Parameter("Spy_Script")))
	{
		return;
	}
	reset = true;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
}

void JFW_Sonar_Pulse::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Create_2D_Sound(Get_Parameter("Sound"));
		Create_Effect_All_Of_Preset(Get_Parameter("Indicator"),Get_Parameter("Sub_Preset_1"),Get_Float_Parameter("ZAdjust"),false);
		Create_Effect_All_Of_Preset(Get_Parameter("Indicator"),Get_Parameter("Sub_Preset_2"),Get_Float_Parameter("ZAdjust"),false);
		Create_Effect_All_Of_Preset(Get_Parameter("WaterIndicator"),Get_Parameter("Sub_Preset_1"),Get_Float_Parameter("WaterZ"),true);
		Create_Effect_All_Of_Preset(Get_Parameter("WaterIndicator"),Get_Parameter("Sub_Preset_2"),Get_Float_Parameter("WaterZ"),true);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("ResetTime"),2);
	}
	else if (number == 2)
	{
		reset = false;
	}
}

void JFW_Global_Stealth_Controller::Created(GameObject *obj)
{
	ListHead = NULL;
	Commands->Start_Timer(obj, this, 5, Get_Int_Parameter("Timer_Number"));
}

void JFW_Global_Stealth_Controller::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	switch(type)
	{
	case 1111:
		Add_Generator(Commands->Get_ID(sender), (float)param, 1);
		break;
	case 1000:
		Add_Generator(Commands->Get_ID(sender), (float)param, 0);
		break;
	case 2222:
		switch(param)
		{
		case 0:
			Update_Generator(Commands->Get_ID(sender), false);
			break;
		case 1:
			Update_Generator(Commands->Get_ID(sender), true, Commands->Get_Player_Type(sender));
			break;
		case 2:
			Update_Generator(Commands->Get_ID(sender), false);
			break;
		case 3:
			Update_Generator(Commands->Get_ID(sender), true);
			break;
		}
		break;
	case 3333:
		Remove_Generator(Commands->Get_ID(sender));
		break;
	}
}

void JFW_Global_Stealth_Controller::Timer_Expired(GameObject *obj, int number)
{
	if (number == Get_Int_Parameter("Timer_Number"))
	{
		Ranged_Stealth_On_Team(ListHead);
		Commands->Start_Timer(obj, this, Get_Float_Parameter("Update_Delay"), Get_Int_Parameter("Timer_Number"));
	}
}

void JFW_Global_Stealth_Controller::Destroyed(GameObject *obj)
{
	Gap_ListNode *Current = ListHead;
	Gap_ListNode *Previous = NULL;
	while (Current != NULL)
	{
		Previous = Current;
		Current = Current->Next;
		Previous->Next = NULL;
		delete Previous;
		Previous = NULL;
	}
	ListHead = NULL;
}

void JFW_Global_Stealth_Controller::Add_Generator(unsigned long ObjectID, float Range, int Team)
{
	if (!ListHead)
	{
		ListHead = new Gap_ListNode;
		ListHead->Next = NULL;
		ListHead->unitID = ObjectID;
		ListHead->range = Range;
		ListHead->teamID = Team;
		ListHead->active = false;
	}
	else
	{
		Gap_ListNode *Current = ListHead;
		while (Current->Next != NULL)
		{
			if (Current->unitID == ObjectID)
			{
				Current->range = Range;
				Current->teamID = Team;
				Current->active = false;
				return;
			}
			Current = Current->Next;
		}
		Current->Next = new Gap_ListNode;
		Current = Current->Next;
		Current->Next = NULL;
		Current->unitID = ObjectID;
		Current->range = Range;
		Current->teamID = Team;
		Current->active = false;
	}
}

void JFW_Global_Stealth_Controller::Remove_Generator(unsigned long ID)
{
	if (ListHead)
	{
		Gap_ListNode *Previous = ListHead;
		do
		{
			if (ListHead->unitID == ID)
			{
				ListHead = ListHead->Next;
				delete Previous;
				Previous = ListHead;
				if (ListHead == NULL)
				{
					return;
				}
			}
		}
		while (ListHead->unitID == ID);
		Gap_ListNode *Current = ListHead->Next;
		while (Current)
		{
			if (Current->unitID == ID)
			{
				Previous->Next = Current->Next;
				Current->Next = NULL;
				delete Current;
				Current = Previous->Next;
			}
			else
			{
				Previous = Current;
				Current = Current->Next;
			}
		}
	}
}

void JFW_Global_Stealth_Controller::Update_Generator(unsigned long ID, bool Active, int Team)
{
	if (ListHead)
	{
		Gap_ListNode *Current = ListHead;
		while (Current)
		{
			if (Current->unitID == ID)
			{
				Current->active = Active;
				Current->teamID = Team;
			}
			Current = Current->Next;
		}
	}
}
	
void JFW_Global_Stealth_Controller::Update_Generator(unsigned long ID, bool Active)
{
	if (ListHead)
	{
		Gap_ListNode *Current = ListHead;
		while (Current)
		{
			if (Current->unitID == ID)
			{
				Current->active = Active;
			}
			Current = Current->Next;
		}
	}
}

void JFW_Stealth_Generator_Building::Created(GameObject *obj)
{
	GameObject *StealthControl = Find_Object_By_Preset(2, Get_Parameter("Stealth_Controller"));
	if (!Get_Int_Parameter("Team"))
	{
		Commands->Send_Custom_Event(obj, StealthControl, 1000, Get_Int_Parameter("Stealth_Range"), 2);
		Commands->Send_Custom_Event(obj, StealthControl, 2222, 3, 3);
	}
	else if (Get_Int_Parameter("Team") == 1)
	{
		Commands->Send_Custom_Event(obj, StealthControl, 1111, Get_Int_Parameter("Stealth_Range"), 2);
		Commands->Send_Custom_Event(obj, StealthControl, 2222, 3, 3);
	}
	sizeID = 0;
	Vector3 position = Commands->Get_Position(obj);
	GameObject *object2 = Commands->Create_Object(Get_Parameter("SizePreset"), position);
	Commands->Set_Player_Type(object2,Commands->Get_Player_Type(obj));
	sizeID = Commands->Get_ID(object2);
	Commands->Start_Timer(obj, this, 2, Get_Int_Parameter("Timer_Number"));
}

void JFW_Stealth_Generator_Building::Timer_Expired(GameObject *obj, int number)
{
	if (number == Get_Int_Parameter("Timer_Number"))
	{
		if (!(Commands->Get_Building_Power(obj)))
		{
			GameObject *StealthControl = Find_Object_By_Preset(2, Get_Parameter("Stealth_Controller"));
			Commands->Send_Custom_Event(obj, StealthControl, 2222, 2, 0);
			if (sizeID)
			{
				Commands->Destroy_Object(Commands->Find_Object(sizeID));
				sizeID = 0;
			}
		}
		Commands->Start_Timer(obj, this, 2, Get_Int_Parameter("Timer_Number"));
	}
}
	
void JFW_Stealth_Generator_Building::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *StealthControl = Find_Object_By_Preset(2, Get_Parameter("Stealth_Controller"));
	Commands->Send_Custom_Event(obj, StealthControl, 3333, 0, 0);
	if (sizeID)
	{
		Commands->Destroy_Object(Commands->Find_Object(sizeID));
		sizeID = 0;
	}
}

void JFW_Stealth_Generator_Vehicle::Created(GameObject *obj)
{
	sizeID = 0;
	GameObject *StealthControl = Find_Object_By_Preset(2, Get_Parameter("Stealth_Controller"));
	if (!Commands->Get_Player_Type(obj))
	{
		Commands->Send_Custom_Event(obj, StealthControl, 1000, Get_Int_Parameter("Stealth_Range"), 0.5);
	}
	else if (Commands->Get_Player_Type(obj) == 1)
	{
		Commands->Send_Custom_Event(obj, StealthControl, 1111, Get_Int_Parameter("Stealth_Range"), 0.5);
	}
}

void JFW_Stealth_Generator_Vehicle::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	GameObject *StealthControl = Find_Object_By_Preset(2, Get_Parameter("Stealth_Controller"));
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		Commands->Send_Custom_Event(obj, StealthControl, 2222, 1, 0);
		if (!sizeID)
		{
			Vector3 position = Commands->Get_Bone_Position(obj, "ROOTTRANSFORM");
			GameObject *object = Commands->Create_Object(Get_Parameter("SizePreset"), position);
			Commands->Set_Player_Type(object,Commands->Get_Player_Type(obj));
			Commands->Attach_To_Object_Bone(object, obj, "ROOTTRANSFORM");
			sizeID = Commands->Get_ID(object);
		}
	}
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		Commands->Send_Custom_Event(obj, StealthControl, 2222, 0, 0);
		if (sizeID)
		{
			Commands->Destroy_Object(Commands->Find_Object(sizeID));
			sizeID = 0;
		}
	}
}

void JFW_Stealth_Generator_Vehicle::Destroyed(GameObject *obj)
{
	GameObject *StealthControl = Find_Object_By_Preset(2, Get_Parameter("Stealth_Controller"));
	Commands->Send_Custom_Event(obj, StealthControl, 3333, 0, 0);
	if (sizeID)
	{
		Commands->Destroy_Object(Commands->Find_Object(sizeID));
		sizeID = 0;
	}
}


void JFW_Stealth_Generator_Vehicle_Deploy::Created(GameObject *obj)
{
	sizeID = 0;
	deploy = false;
	GameObject *StealthControl = Find_Object_By_Preset(2, Get_Parameter("Stealth_Controller"));
	if (!Commands->Get_Player_Type(obj))
	{
		Commands->Send_Custom_Event(obj, StealthControl, 1000, Get_Int_Parameter("Stealth_Range"), 0.5);
	}
	else if (Commands->Get_Player_Type(obj) == 1)
	{
		Commands->Send_Custom_Event(obj, StealthControl, 1111, Get_Int_Parameter("Stealth_Range"), 0.5);
	}
	Commands->Start_Timer(obj,this,1,1);
}

void JFW_Stealth_Generator_Vehicle_Deploy::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_EXITED && deploy && obj->As_VehicleGameObj()->Get_Occupant_Count() == 0)
	{
		Update_Network_Object(obj);
		Commands->Set_Player_Type(obj,Commands->Get_Player_Type(sender));
	}
}

void JFW_Stealth_Generator_Vehicle_Deploy::Timer_Expired(GameObject *obj,int number)
{
	Commands->Start_Timer(obj,this,1,1);
	GameObject *StealthControl = Find_Object_By_Preset(2, Get_Parameter("Stealth_Controller"));
	if (obj->As_VehicleGameObj()->Is_Immovable())
	{
		if (!deploy)
		{
			deploy = true;
			obj->As_VehicleGameObj()->Set_Lock_Team(obj->As_VehicleGameObj()->Get_Player_Type());
			Commands->Send_Custom_Event(obj, StealthControl, 2222, 1, 0);
			if (!sizeID)
			{
				Vector3 position = Commands->Get_Bone_Position(obj, "ROOTTRANSFORM");
				GameObject *object = Commands->Create_Object(Get_Parameter("SizePreset"), position);
				Commands->Set_Player_Type(object,Commands->Get_Player_Type(obj));
				Commands->Attach_To_Object_Bone(object, obj, "ROOTTRANSFORM");
				sizeID = Commands->Get_ID(object);
			}
		}
	}
	else
	{
		if (deploy)
		{
			deploy = false;
			obj->As_VehicleGameObj()->Set_Lock_Team(2);
			Commands->Send_Custom_Event(obj, StealthControl, 2222, 0, 0);
			if (sizeID)
			{
				Commands->Destroy_Object(Commands->Find_Object(sizeID));
				sizeID = 0;
			}
		}
	}
}

void JFW_Stealth_Generator_Vehicle_Deploy::Destroyed(GameObject *obj)
{
	GameObject *StealthControl = Find_Object_By_Preset(2, Get_Parameter("Stealth_Controller"));
	Commands->Send_Custom_Event(obj, StealthControl, 3333, 0, 0);
	if (sizeID)
	{
		Commands->Destroy_Object(Commands->Find_Object(sizeID));
		sizeID = 0;
	}
}

void JFW_Low_Power_Message::Created(GameObject *obj)
{
	lowpower = false;
	Commands->Start_Timer(obj,this,1.0,1);
}

void JFW_Low_Power_Message::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		bool power = Is_Base_Powered(Commands->Get_Player_Type(obj));
		if (power)
		{
			lowpower = false;
		}
		if (!power && !lowpower)
		{
			lowpower = true;
			Send_Message(255,255,255,Get_Parameter("Message"));
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Low_Power_Sound::Created(GameObject *obj)
{
	lowpower = false;
	Commands->Start_Timer(obj,this,1.0,1);
}

void JFW_Low_Power_Sound::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		bool power = Is_Base_Powered(Commands->Get_Player_Type(obj));
		if (power && lowpower)
		{
			lowpower = false;
			Commands->Create_Sound(Get_Parameter("UpSound"),Commands->Get_Position(obj),obj);
		}
		if (!power && !lowpower)
		{
			lowpower = true;
			Commands->Create_Sound(Get_Parameter("DownSound"),Commands->Get_Position(obj),obj);
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Message_Send_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	Commands->Create_2D_Sound(Get_Parameter("Sound"));
	Send_Message(Get_Int_Parameter("Red"),Get_Int_Parameter("Green"),Get_Int_Parameter("Blue"),Get_Parameter("Message"));
	if (Get_Int_Parameter("Delete"))
	{
		Destroy_Script();
	}
}

void JFW_Message_Send_Zone_Team::Entered(GameObject *obj,GameObject *enterer)
{
	int team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	Create_2D_Sound_Team(Get_Parameter("Sound"),team);
	Send_Message_Team(team,Get_Int_Parameter("Red"),Get_Int_Parameter("Green"),Get_Int_Parameter("Blue"),Get_Parameter("Message"));
	if (Get_Int_Parameter("Delete"))
	{
		Destroy_Script();
	}
}

void JFW_Message_Send_Zone_Player::Entered(GameObject *obj,GameObject *enterer)
{
	int team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	Create_2D_Sound_Player(enterer,Get_Parameter("Sound"));
	Send_Message_Player(enterer,Get_Int_Parameter("Red"),Get_Int_Parameter("Green"),Get_Int_Parameter("Blue"),Get_Parameter("Message"));
	if (Get_Int_Parameter("Delete"))
	{
		Destroy_Script();
	}
}

void JFW_Message_Send_Death::Killed(GameObject *obj, GameObject *killer)
{
	Commands->Create_2D_Sound(Get_Parameter("Sound"));
	Send_Message(Get_Int_Parameter("Red"),Get_Int_Parameter("Green"),Get_Int_Parameter("Blue"),Get_Parameter("Message"));
	Destroy_Script();
}

void JFW_Message_Send_Death_Team::Killed(GameObject *obj, GameObject *killer)
{
	int team = Get_Int_Parameter("Player_Type");
	Create_2D_Sound_Team(Get_Parameter("Sound"),team);
	Send_Message_Team(team,Get_Int_Parameter("Red"),Get_Int_Parameter("Green"),Get_Int_Parameter("Blue"),Get_Parameter("Message"));
	Destroy_Script();
}

void JFW_Message_Send_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Commands->Create_2D_Sound(Get_Parameter("Sound"));
		Send_Message(Get_Int_Parameter("Red"),Get_Int_Parameter("Green"),Get_Int_Parameter("Blue"),Get_Parameter("Display_Message"));
		if (Get_Int_Parameter("Delete"))
		{
			Destroy_Script();
		}
	}
}

void JFW_Message_Send_Powerup::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Create_2D_Sound_Player(sender,Get_Parameter("Sound"));
		Send_Message_Player(sender,Get_Int_Parameter("Red"),Get_Int_Parameter("Green"),Get_Int_Parameter("Blue"),Get_Parameter("Display_Message"));
	}
}

void JFW_Message_Send_Custom_Team::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		int team = Get_Int_Parameter("Player_Type");
		Create_2D_Sound_Team(Get_Parameter("Sound"),team);
		Send_Message_Team(team,Get_Int_Parameter("Red"),Get_Int_Parameter("Green"),Get_Int_Parameter("Blue"),Get_Parameter("Display_Message"));
		if (Get_Int_Parameter("Delete"))
		{
			Destroy_Script();
		}
	}
}

void JFW_Spy_Switch_Team::Created(GameObject *obj)
{
	Change_Team_2(obj,PTTEAM(Commands->Get_Player_Type(obj)));
	Commands->Set_Is_Visible(obj,false);
}

void JFW_Spy_Switch_Team::Killed(GameObject *obj, GameObject *killer)
{
	Change_Team_2(obj,Commands->Get_Player_Type(obj));
}

void JFW_Spy_Switch_Team::Destroyed(GameObject *obj)
{
	Change_Team_2(obj,Commands->Get_Player_Type(obj));
}

void JFW_Spy_Switch_Team::Detach(GameObject *obj)
{
	ScriptImpClass::Detach(obj);
	if (Exe != 4)
	{
		Change_Team_2(obj,Commands->Get_Player_Type(obj));
	}
}

void JFW_Spy_Vehicle_Ignore::Created(GameObject *obj)
{
	spycount = 0;
	nonspycount = 0;
}

void JFW_Spy_Vehicle_Ignore::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!Is_Script_Attached(sender,Get_Parameter("Spy_Script")))
		{
			nonspycount++;
		}
		else
		{
			spycount++;
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (!Is_Script_Attached(sender,Get_Parameter("Spy_Script")))
		{
			nonspycount--;
		}
		else
		{
			spycount--;
		}
	}
	if ((spycount) && (!nonspycount))
	{
		Commands->Set_Is_Visible(obj,false);
	}
	else
	{
		Commands->Set_Is_Visible(obj,true);
	}
}

void JFW_Spy_Vehicle_Ignore_New::Created(GameObject *obj)
{
	spycount = 0;
	nonspycount = 0;
}

void JFW_Spy_Vehicle_Ignore_New::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!Is_Spy(sender))
		{
			nonspycount++;
		}
		else
		{
			spycount++;
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (!Is_Spy(sender))
		{
			nonspycount--;
		}
		else
		{
			spycount--;
		}
	}
	if ((spycount) && (!nonspycount))
	{
		Commands->Set_Is_Visible(obj,false);
	}
	else
	{
		Commands->Set_Is_Visible(obj,true);
	}
}

void JFW_Message_Send_Zone_Player_Vehicle::Entered(GameObject *obj,GameObject *enterer)
{
	int team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	if (Get_Vehicle(enterer))
	{
		Create_2D_Sound_Player(enterer,Get_Parameter("Sound"));
		Send_Message_Player(enterer,Get_Int_Parameter("Red"),Get_Int_Parameter("Green"),Get_Int_Parameter("Blue"),Get_Parameter("Message"));
		if (Get_Int_Parameter("Delete"))
		{
			Destroy_Script();
		}
	}
}

void JFW_Water_Level::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Vector3 v = Commands->Get_Position(obj);
		float waterlevel = v.Z;
		Commands->Send_Custom_Event(obj,sender,type,*((int *)&waterlevel),0);
	}
}

void JFW_Submarine::Created(GameObject *obj)
{
	waterlevel = 0;
	submerged = false;
	driverID = 0;
	occupied = false;
	Commands->Send_Custom_Event(obj,Find_Object_With_Script("JFW_Water_Level"),Get_Int_Parameter("Message"),0,0);
}

void JFW_Submarine::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		waterlevel = *((float *)&param);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Ping_Time"),1);
		Commands->Start_Timer(obj,this,1,2);
	}
	else if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		occupied = true;
		if (!driverID)
		{
			driverID = Commands->Get_ID(sender);
		}
	}
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		occupied = false;
		if (submerged)
		{
			Commands->Apply_Damage(obj, 100, "Death", 0);
		}
		if (driverID == Commands->Get_ID(sender))
		{
			driverID = 0;
		}
	}
}

void JFW_Submarine::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (submerged)
		{
			Vector3 v = Commands->Get_Position(obj);
			if (v.Z < (waterlevel + Get_Float_Parameter("Surface_Z_Offset")))
			{
				Commands->Create_Sound(Get_Parameter("Ping_Sound"),Commands->Get_Position(obj),obj);
			}
		}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Ping_Time"),1);
	}
	if (number == 2)
	{
		if (submerged)
		{
			Vector3 v = Commands->Get_Position(obj);
			if (v.Z < (waterlevel + Get_Float_Parameter("Surface_Z_Offset")))
			{
			}
			else
			{
				Commands->Create_Sound(Get_Parameter("Surface_Sound"),Commands->Get_Position(obj),obj);
				Commands->Set_Shield_Type(obj,Get_Parameter("Surface_Armor"));
				Commands->Enable_Stealth(obj,false);
				if (Get_Int_Parameter("Block_Weapon"))
				{
					VehicleGameObj *veh = obj->As_VehicleGameObj();
					if (veh)
					{
						veh->Set_Scripts_Can_Fire(true);
					}
				}
				Commands->Send_Custom_Event(obj,Commands->Find_Object(driverID),987987,0,0);
				submerged = false;
			}
		}
		else
		{
			Vector3 v = Commands->Get_Position(obj);	
			if (v.Z < (waterlevel + Get_Float_Parameter("Dive_Z_Offset")))
			{
				Commands->Set_Shield_Type(obj,Get_Parameter("Submerge_Armor"));
				Commands->Enable_Stealth(obj,true);
				if (Get_Int_Parameter("Block_Weapon"))
				{
					VehicleGameObj *veh = obj->As_VehicleGameObj();
					if (veh)
					{
						veh->Set_Scripts_Can_Fire(false);
					}
				}
				if (!occupied)
				{
					Commands->Apply_Damage(obj, 100, "Death", 0);
				}
				Commands->Send_Custom_Event(obj,Commands->Find_Object(driverID),789789,0,0);
				submerged = true;
			}
		}
		Commands->Start_Timer(obj,this,1,2);
	}
}

void JFW_Submarine::Killed(GameObject *obj, GameObject *killer)
{
	if (submerged)
	{
		if (driverID) //do I have a driver ID
		{
			GameObject *driver = Commands->Find_Object(driverID);
			if (driver) //double checking validity
			{
				Commands->Attach_Script(driver, "RA_DriverDeath", "0");
			}
		}
	}
}

void JFW_Reaper_Web::Created(GameObject *obj)
{
	WebID = 0;
}

void JFW_Reaper_Web::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	ScriptableGameObj* damager_obj = Get_Vehicle_Return(damager);
	if (!damager_obj) return;
	unsigned int warhead = ArmorWarheadManager::Get_Warhead_Type(Get_Parameter("Warhead"));

	if (warhead == Get_Damage_Warhead())
	{
		if (WebID)
		{
			Commands->Destroy_Object(Commands->Find_Object(WebID));
			WebID = 0;
		}
		Vector3 pos;
		pos = Commands->Get_Position(obj);
		Commands->Set_Animation(obj,Get_Parameter("HumanAnimation"),false,0,0.0f,-1.0f,false);
		SoldierGameObj *o = obj->As_SoldierGameObj();
		o->Set_Freeze(true);
		float Facing = Commands->Get_Facing(obj);
		GameObject *web = Commands->Create_Object("Invisible_Object",pos);
		Commands->Set_Model(web,Get_Parameter("WebModel"));
		Commands->Set_Facing(web,Facing);
		StringClass s;
		s.Format("%s.%s",Get_Parameter("WebModel"),Get_Parameter("WebModel"));
		Commands->Set_Animation(web,s,false,0,0.0f,-1.0f,false);
		WebID = Commands->Get_ID(web);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("time"),1);
	}
}

void JFW_Reaper_Web::Killed(GameObject *obj,GameObject *killer)
{
	if (WebID)
	{
		Commands->Destroy_Object(Commands->Find_Object(WebID));
		WebID = 0;
	}
}

void JFW_Reaper_Web::Timer_Expired(GameObject *obj,int number)
{
	if (WebID)
	{
		SoldierGameObj *o = obj->As_SoldierGameObj();
		o->Set_Freeze(false);
		Commands->Destroy_Object(Commands->Find_Object(WebID));
		WebID = 0;
	}
}

void JFW_Toxic_Grenade::Created(GameObject *obj)
{
	active = false;
}

void JFW_Toxic_Grenade::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (amount < 0)
	{
		active = false;
		return;
	}
	ScriptableGameObj* damager_obj = Get_Vehicle_Return(damager);
	if (!damager_obj) return;
	const char* damager_weapon = Get_Current_Weapon(damager_obj);
	if (!damager_weapon) return;

	if (!_stricmp(damager_weapon,Get_Parameter("Weapon")))
	{
		active = true;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
	}
}

void JFW_Toxic_Grenade::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_REFILL)
	{
		active = false;
	}
}


void JFW_Toxic_Grenade::Timer_Expired(GameObject *obj,int number)
{
	if (active)
	{
		Commands->Apply_Damage(obj,Get_Float_Parameter("Damage"),Get_Parameter("Warhead"),0);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
	}
}

void JFW_Hijacker_Vehicle::Created(GameObject *obj)
{
	HijackerID = 0;
}

void JFW_Hijacker_Vehicle::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (damager && !HijackerID)
	{
		if (!_stricmp(Get_Current_Weapon(damager),Get_Parameter("Weapon")) && (!Get_Vehicle(damager)))
		{
			if (!damager->As_DamageableGameObj()->Is_Teammate(obj->As_DamageableGameObj()))
			{
				if (Get_Vehicle_Occupant_Count(obj) == 1 && !obj->As_VehicleGameObj()->Is_Immovable())
				{
					Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(damager),damager);
					Force_Occupants_Exit(obj);
					Commands->Start_Timer(obj,this,0.5,1);
					HijackerID = Commands->Get_ID(damager);
				}
			}
		}
	}
}

void JFW_Hijacker_Vehicle::Timer_Expired(GameObject *obj,int number)
{
	if (!Get_Vehicle_Occupant_Count(obj))
	{
		Soldier_Transition_Vehicle(Commands->Find_Object(HijackerID));
	}
}

void JFW_Hijacker_Vehicle::Killed(GameObject *obj,GameObject *killer)
{
	if (HijackerID)
	{
		Commands->Apply_Damage(Commands->Find_Object(HijackerID),Get_Float_Parameter("Damage"),Get_Parameter("Warhead"),0);
	}
}

void JFW_Hijacker_Vehicle_2::Created(GameObject *obj)
{
	HijackerID = 0;
	jacking = false;
}

void JFW_Hijacker_Vehicle_2::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (damager && !HijackerID && !jacking)
	{
		unsigned int warhead = ArmorWarheadManager::Get_Warhead_Type(Get_Parameter("Warhead"));
		if (warhead == Get_Damage_Warhead() && (!Get_Vehicle(damager)))
		{
			if (!damager->As_DamageableGameObj()->Is_Teammate(obj->As_DamageableGameObj()))
			{
				if (Get_Vehicle_Occupant_Count(obj) == 1)
				{
					GameObject *o = Get_Vehicle_Driver(obj);
					if (o && o->As_SmartGameObj() && o->As_SmartGameObj()->Is_Control_Enabled())
					{
						if (Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(damager)) <= Get_Float_Parameter("Distance"))
						{
							Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(damager),damager);
							Force_Occupants_Exit(obj);
							Commands->Start_Timer(obj,this,0.5,1);
							HijackerID = Commands->Get_ID(damager);
							damager->As_SoldierGameObj()->Set_Can_Drive_Vehicles(true);
							jacking = true;
						}
					}
				}
			}
		}
	}
}

void JFW_Hijacker_Vehicle_2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if ((type == CUSTOM_EVENT_VEHICLE_ENTERED) && (sender == Commands->Find_Object(HijackerID)))
	{
		if (Get_Int_Parameter("Disable_Transitions"))
		{
			Commands->Enable_Vehicle_Transitions(obj,false);
		}
		if (Get_Int_Parameter("Single_Hijack"))
		{
			Remove_Weapon(sender,Get_Parameter("Weapon"));
		}
	}
	if ((type == CUSTOM_EVENT_VEHICLE_EXITED) && (sender == Commands->Find_Object(HijackerID)))
	{
		if (!Get_Int_Parameter("Single_Hijack"))
		{
			sender->As_SoldierGameObj()->Set_Can_Drive_Vehicles(false);
		}
		HijackerID = 0;
	}
}

void JFW_Hijacker_Vehicle_2::Timer_Expired(GameObject *obj,int number)
{
	if (!Get_Vehicle_Occupant_Count(obj))
	{
		jacking = false;
		Force_Vehicle_Entry(Commands->Find_Object(HijackerID),obj);
	}
}

void JFW_Hijacker_Vehicle_2::Killed(GameObject *obj,GameObject *killer)
{
	if (Commands->Find_Object(HijackerID) && !Get_Int_Parameter("Single_Hijack"))
	{
		Commands->Find_Object(HijackerID)->As_SoldierGameObj()->Set_Can_Drive_Vehicles(false);
	}
}

void JFW_Building_Preset_Disable::Created(GameObject *obj)
{
	count = 0;
}
void JFW_Building_Preset_Disable::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Disable_Custom"))
	{
		if (count == 0)
		{
			if (obj->As_BuildingGameObj())
			{
				BuildingType typex = obj->As_BuildingGameObj()->Get_Definition().Get_Type();
				int team = obj->As_BuildingGameObj()->Get_Player_Type();
				Disable_All_Presets_By_Factory_Tech(typex,team,true);
			}
		}
		count++;
	}
	if (type == Get_Int_Parameter("Enable_Custom"))
	{
		count--;
		if (count == 0)
		{
			if (obj->As_BuildingGameObj())
			{
				BuildingType typex = obj->As_BuildingGameObj()->Get_Definition().Get_Type();
				int team = obj->As_BuildingGameObj()->Get_Player_Type();
				Disable_All_Presets_By_Factory_Tech(typex,team,false);
			}
		}
	}
}

void JFW_Building_Preset_Disable::Killed(GameObject *obj,GameObject *killer)
{
	Destroy_Script();
}

ScriptRegistrant<JFW_Domination_Controler_End_Game> JFW_Domination_Controler_End_Game_Registrant("JFW_Domination_Controler_End_Game","NeutralMessage:int,OwnedMessage:int,Time:float,TimerNum:int,PointsToGive:float,PointsToTake:float,GDIObjectID:int,NodObjectID:int,ZoneCount:int");
ScriptRegistrant<JFW_Carryable_Vehicle> JFW_Carryable_Vehicle_Registrant("JFW_Carryable_Vehicle","Only_Empty:int,Control_ID:int");
ScriptRegistrant<JFW_CarryAll> JFW_CarryAll_Registrant("JFW_CarryAll","Bone_Name:string,Cost:int,Control_ID:int");
ScriptRegistrant<JFW_Carry_Control> JFW_Carry_Control_Registrant("JFW_Carry_Control","Max_Carryalls:int");
ScriptRegistrant<JFW_Sensor_Array> JFW_Sensor_Array_Registrant("JFW_Sensor_Array","Distance:float,Indicator:string,IndicatorZOffset:float,Time:float,TimerNum:int,Player_Type:int");
ScriptRegistrant<JFW_Team_DM_Controller_2> JFW_Team_DM_Controller_2_Registrant("JFW_Team_DM_Controller_2","Deaths_To_Loose:int,Death_Message:int,Team:int");
ScriptRegistrant<JFW_Vehicle_Thief> JFW_Vehicle_Thief_Registrant("JFW_Vehicle_Thief","Weapon:string,Player_Type:int");
ScriptRegistrant<JFW_Vehicle_Thief_2> JFW_Vehicle_Thief_2_Registrant("JFW_Vehicle_Thief_2","Weapon:string,Player_Type:int,Sound:string");
ScriptRegistrant<JFW_Conyard> JFW_Conyard_Registrant("JFW_Conyard","Time:float,TimerNum:int,DisableCustom:int,Health:float");
ScriptRegistrant<JFW_Flying_Infantry> JFW_Flying_Infantry_Registrant("JFW_Flying_Infantry","");
ScriptRegistrant<JFW_Flying_Infantry_Custom> JFW_Flying_Infantry_Custom_Registrant("JFW_Flying_Infantry_Custom","Message:int");
ScriptRegistrant<JFW_Flying_Infantry_Powerup> JFW_Flying_Infantry_Powerup_Registrant("JFW_Flying_Infantry_Powerup","");
ScriptRegistrant<JFW_Flying_Infantry_Enter> JFW_Flying_Infantry_Enter_Registrant("JFW_Flying_Infantry_Enter","Player_Type:int");
ScriptRegistrant<JFW_Flying_Infantry_Exit> JFW_Flying_Infantry_Exit_Registrant("JFW_Flying_Infantry_Exit","Player_Type:int");
ScriptRegistrant<JFW_Drone_Damage> JFW_Drone_Damage_Registrant("JFW_Drone_Damage","RepairStop:int,Preset:string,Amount:int,Custom:int");
ScriptRegistrant<JFW_Conyard_Vehicle_Buildings> JFW_Conyard_Vehicle_Buildings_Registrant("JFW_Conyard_Vehicle_Buildings","Time:float,TimerNum:int,DisableCustom:int,Health:float,Message:int");
ScriptRegistrant<JFW_Domination_Zone> JFW_Domination_Zone_Registrant("JFW_Domination_Zone","Controler_ID:int,NeutralMessage:int,OwnedMessage:int,Nod_Model:string,GDI_Model:string,Neutral_Model:string,Capture_Delay_Time:float,TimerNum:int");
ScriptRegistrant<JFW_Domination_Controler> JFW_Domination_Controler_Registrant("JFW_Domination_Controler","NeutralMessage:int,OwnedMessage:int,Time:float,TimerNum:int,PointsToGive:float,PointsToTake:float,GDIObjectID:int,NodObjectID:int");
ScriptRegistrant<JFW_Iron_Curtain_Zone> JFW_Iron_Curtain_Zone_Registrant("JFW_Iron_Curtain_Zone","Time:float,TimerNum:int,Animation1:string,Animation1Loc:vector3,Animation1Facing:float,Animation1Slot:int,Animation2:string,Animation2Loc:vector3,Animation2Facing:float,Animation2Slot:int,Player_Type:int,Animation3:string,Animation3Loc:vector3,Animation3Facing:float,Animation3Slot:int,BuildingID:int,Invunlerability_Time:float,Invunlerability_TimerNum:int,Animation1Msg:int,Animation2Msg:int,Animation3Msg:int,OffID:int,OnID:int,CinematicPowerOff:int");
ScriptRegistrant<JFW_Chrono_Controler_Zone> JFW_Chrono_Controler_Zone_Registrant("JFW_Chrono_Controler_Zone","Spawner_Time:float,Spawner_TimerNum:int,Spawner_Preset:string,Spawner_Location:vector3,Chrono_Time:float,Chrono_TimerNum:int,Message:int,Animation1:string,Animation1Loc:vector3,Animation1Facing:float,Animation1Slot:int,Animation2:string,Animation2Loc:vector3,Animation2Facing:float,Animation2Slot:int,Animation3:string,Animation3Loc:vector3,Animation3Facing:float,Animation3Slot:int,BuildingID:int,ZAdjust:float,Animation1Msg:int,Animation2Msg:int,Animation3Msg:int,OffID:int,OnID:int,CinematicPowerOff:int,Activate_Time:float,Activate_TimerNum:int,DeathMessage:int");
ScriptRegistrant<JFW_Assault_Powerplant_Controller> JFW_Assault_Powerplant_Controller_Registrant("JFW_Assault_Powerplant_Controller","Time:float,TimerNum:int,Player_Type:int,Power_Off:int,Power_On:int,Building1:int,Building2:int,Building3:int,Building4:int,Building5:int,Building6:int,Building7:int,Building8:int,Building9:int,Building10:int,Building11:int,Building12:int,Building13:int,Building14:int,Building15:int");
ScriptRegistrant<JFW_Resource_Collector> JFW_Resource_Collector_Registrant("JFW_Resource_Collector","Collected_Mesasge:int,Refinery_Message:int,Cash_Limit:int,Cash_Type:int,Entire_Team:int");
ScriptRegistrant<JFW_Resource_Collector_2> JFW_Resource_Collector_2_Registrant("JFW_Resource_Collector_2","Collected_Mesasge:int,Refinery_Message:int,Cash_Limit:int,Cash_Type:int,Entire_Team:int");
ScriptRegistrant<JFW_Resource_Collector_3> JFW_Resource_Collector_3_Registrant("JFW_Resource_Collector_3","Collected_Mesasge:int,Refinery_Message:int,Cash_Limit:int,Cash_Type:int,Entire_Team:int");
ScriptRegistrant<JFW_Resource_Refinery> JFW_Resource_Refinery_Registrant("JFW_Resource_Refinery","Refinery_Message:int");
ScriptRegistrant<JFW_Resource_Field> JFW_Resource_Field_Registrant("JFW_Resource_Field","Cash:int,Time:float,TimerNum:int,Collected_Preset:string,Collected_Preset_Distance:float,Collected_Message:int,Nod_Preset:string,GDI_Preset:string,StartHarvest:int,StopHarvest:int");
ScriptRegistrant<JFW_Team_DM_Controller> JFW_Team_DM_Controller_Registrant("JFW_Team_DM_Controller","Deaths_To_Loose:int,Death_Message:int,Building_To_Destroy1:int,Building_To_Destroy2:int,Building_To_Destroy3:int,Building_To_Destroy4:int,Building_To_Destroy5:int");
ScriptRegistrant<JFW_Team_DM_Zone> JFW_Team_DM_Zone_Registrant("JFW_Team_DM_Zone","Player_Type:int,Controller_ID:int,Deactivate_Message:int,Death_Message:int");
ScriptRegistrant<JFW_Team_DM_Attatched> JFW_Team_DM_Attatched_Registrant("JFW_Team_DM_Attatched","Controller_ID:int,Deactivate_Message:int,Death_Message:int");
ScriptRegistrant<JFW_Conyard_Radius> JFW_Conyard_Radius_Registrant("JFW_Conyard_Radius","Time:float,TimerNum:int,DisableCustom:int,Health:float,Radius:float");
ScriptRegistrant<JFW_Chrono_Harvester_Logic> JFW_Chrono_Harvester_Logic_Registrant("JFW_Chrono_Harvester_Logic","Harvester_Preset:string,Explosion_Preset:string");
ScriptRegistrant<JFW_Chrono_Harvester_Attached> JFW_Chrono_Harvester_Attached_Registrant("JFW_Chrono_Harvester_Attached","LogicID=0:int,Number_Of_Paths=0:int,Waypath1_Dock2Field=0:int,Waypath1_Field2Dock=0:int,Waypath1_Tib_Zone:vector3,Waypath2_Dock2Field=0:int,Waypath2_Field2Dock=0:int,Waypath2_Tib_Zone:vector3,Waypath3_Dock2Field=0:int,Waypath3_Field2Dock=0:int,Waypath3_Tib_Zone:vector3,Harvest_Animation:string,Dock_Animation:string,Tiberium_Idle_Time=45.00:float,Dock_Location:vector3,Dock_Idle_Time=10.00:float,Dock_Credits=400.00:float,Harvester_Create_Idle_Time=5.00:float,Crystal_Preset:string,Chrono_Location:vector3");
ScriptRegistrant<JFW_Chrono_Harvester_Kill> JFW_Chrono_Harvester_Kill_Registrant("JFW_Chrono_Harvester_Kill","LogicID=0:int");
ScriptRegistrant<JFW_Chrono_Harvester_Spawn_Kill> JFW_Chrono_Harvester_Spawn_Kill_Registrant("JFW_Chrono_Harvester_Spawn_Kill","LogicID=0:int");
ScriptRegistrant<JFW_Conyard_Turrets> JFW_Conyard_Turrets_Registrant("JFW_Conyard_Turrets","Time:float,TimerNum:int,DisableCustom:int,Health:float");
ScriptRegistrant<JFW_Radar_Jammer> JFW_Radar_Jammer_Registrant("JFW_Radar_Jammer","Time:float,TimerNum:int,DisableCustom:int,Preset:string,CenterID:int,Range:float,StartSound1:string,StopSound1:string,StartSound2:string,StopSound2:string");
ScriptRegistrant<JFW_Radar_Jammer_Sound> JFW_Radar_Jammer_Sound_Registrant("JFW_Radar_Jammer_Sound","Time:float,TimerNum:int,DisableCustom:int,Preset:string,CenterID:int,Range:float,Sound:string");
ScriptRegistrant<JFW_Sonar_Pulse> JFW_Sonar_Pulse_Registrant("JFW_Sonar_Pulse","Spy_Script:string,Sub_Preset_1:string,Sub_Preset_2:string,Indicator:string,ZAdjust:float,WaterIndicator:string,WaterZ:float,Sound:string,Time:float,ResetTime:float");
ScriptRegistrant<JFW_Global_Stealth_Controller> JFW_Global_Stealth_Controller_Registrant("JFW_Global_Stealth_Controller","Update_Delay=2.0:float,Timer_Number=687:int");
ScriptRegistrant<JFW_Stealth_Generator_Building> JFW_Stealth_Generator_Building_Registrant("JFW_Stealth_Generator_Building","Timer_Number=876:int,Stealth_Range=50:int,Stealth_Controller:string,Team=1:int,SizePreset:string");
ScriptRegistrant<JFW_Stealth_Generator_Vehicle> JFW_Stealth_Generator_Vehicle_Registrant("JFW_Stealth_Generator_Vehicle","Stealth_Range=50:int,Stealth_Controller:string,SizePreset:string");
ScriptRegistrant<JFW_Stealth_Generator_Vehicle_Deploy> JFW_Stealth_Generator_Vehicle_Deploy_Registrant("JFW_Stealth_Generator_Vehicle_Deploy","Stealth_Range=50:int,Stealth_Controller:string,SizePreset:string");
ScriptRegistrant<JFW_Stealth_Generator_Ignored> JFW_Stealth_Generator_Ignored_Registrant("JFW_Stealth_Generator_Ignored","");
ScriptRegistrant<JFW_Low_Power_Message> JFW_Low_Power_Message_Registrant("JFW_Low_Power_Message","Message:string");
ScriptRegistrant<JFW_Low_Power_Sound> JFW_Low_Power_Sound_Registrant("JFW_Low_Power_Sound","DownSound:string,UpSound:string");
ScriptRegistrant<JFW_Message_Send_Zone> JFW_Message_Send_Zone_Registrant("JFW_Message_Send_Zone","Player_Type:int,Message:string,Red:int,Blue:int,Green:int,Sound:string,Delete:int");
ScriptRegistrant<JFW_Message_Send_Zone_Team> JFW_Message_Send_Zone_Team_Registrant("JFW_Message_Send_Zone_Team","Player_Type:int,Message:string,Red:int,Blue:int,Green:int,Sound:string,Delete:int");
ScriptRegistrant<JFW_Message_Send_Zone_Player> JFW_Message_Send_Zone_Player_Registrant("JFW_Message_Send_Zone_Player","Player_Type:int,Message:string,Red:int,Blue:int,Green:int,Sound:string,Delete:int");
ScriptRegistrant<JFW_Message_Send_Death> JFW_Message_Send_Death_Registrant("JFW_Message_Send_Death","Message:string,Red:int,Blue:int,Green:int,Sound:string");
ScriptRegistrant<JFW_Message_Send_Death_Team> JFW_Message_Send_Death_Team_Registrant("JFW_Message_Send_Death_Team","Player_Type:int,Message:string,Red:int,Blue:int,Green:int,Sound:string");
ScriptRegistrant<JFW_Message_Send_Custom> JFW_Message_Send_Custom_Registrant("JFW_Message_Send_Custom","Display_Message:string,Red:int,Blue:int,Green:int,Sound:string,Message:int,Delete:int");
ScriptRegistrant<JFW_Message_Send_Powerup> JFW_Message_Send_Powerup_Registrant("JFW_Message_Send_Powerup","Display_Message:string,Red:int,Blue:int,Green:int,Sound:string");
ScriptRegistrant<JFW_Message_Send_Custom_Team> JFW_Message_Send_Custom_Team_Registrant("JFW_Message_Send_Custom_Team","Player_Type:int,Display_Message:string,Red:int,Blue:int,Green:int,Sound:string,Message:int,Delete:int");
ScriptRegistrant<JFW_Spy_Switch_Team> JFW_Spy_Switch_Team_Registrant("JFW_Spy_Switch_Team","");
ScriptRegistrant<JFW_Spy_Vehicle_Ignore> JFW_Spy_Vehicle_Ignore_Registrant("JFW_Spy_Vehicle_Ignore","Spy_Script:string");
ScriptRegistrant<JFW_Spy_Vehicle_Ignore_New> JFW_Spy_Vehicle_Ignore_New_Registrant("JFW_Spy_Vehicle_Ignore_New","");
ScriptRegistrant<JFW_Message_Send_Zone_Player_Vehicle> JFW_Message_Send_Zone_Player_Vehicle_Registrant("JFW_Message_Send_Zone_Player_Vehicle","Player_Type:int,Message:string,Red:int,Blue:int,Green:int,Sound:string,Delete:int");
ScriptRegistrant<JFW_Water_Level> JFW_Water_Level_Registrant("JFW_Water_Level","Message:int");
ScriptRegistrant<JFW_Submarine> JFW_Submarine_Registrant("JFW_Submarine","Message:int,Submerge_Armor:string,Surface_Armor:string,Block_Weapon:int,Ping_Sound:string,Surface_Sound:string,Ping_Time:float,Surface_Z_Offset:float,Dive_Z_Offset:float,Powerup:string,Weapon:string");
ScriptRegistrant<JFW_Reaper_Web> JFW_Reaper_Web_Registrant("JFW_Reaper_Web","Warhead:string,Time:float,WebModel:string,HumanAnimation:string");
ScriptRegistrant<JFW_Toxic_Grenade> JFW_Toxic_Grenade_Registrant("JFW_Toxic_Grenade","Weapon:string,Time:float,Warhead:string,Damage:float");
ScriptRegistrant<JFW_Hijacker_Vehicle> JFW_Hijacker_Vehicle_Registrant("JFW_Hijacker_Vehicle","Weapon:string,Warhead:string,Damage:float,Sound:string");
ScriptRegistrant<JFW_Hijacker_Vehicle_2> JFW_Hijacker_Vehicle_2_Registrant("JFW_Hijacker_Vehicle_2","Warhead:string,Sound:string,Disable_Transitions:int,Single_Hijack:int,Distance:float,Weapon:string");
ScriptRegistrant<JFW_Building_Preset_Disable> JFW_Building_Preset_Disable_Registrant("JFW_Building_Preset_Disable","Disable_Custom:int,Enable_Custom:int");
