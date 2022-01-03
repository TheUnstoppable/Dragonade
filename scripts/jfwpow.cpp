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
#include "jfwpow.h"

void JFW_Spawn_Timer::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("SpawnCustom"))
	{
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
	}
}

void JFW_Spawn_Timer::Timer_Expired(GameObject *obj,int number)
{
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("TriggerID")),Get_Int_Parameter("TriggerCustom"),0,0);
	Commands->Create_Object(Get_Parameter("SpawnPreset"),Get_Vector3_Parameter("SpawnLocation"));
}

void JFW_Warp_On_Pickup_Team::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		int x;
		x = Get_Int_Parameter("Player_Type");
		if (CheckPlayerType(sender,x))
		{
			return;
		}
		Vector3 WarpPosition = Get_Vector3_Parameter("Position");
		Commands->Set_Position(sender,WarpPosition);
	}
}

void JFW_Refill_Powerup::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Grant_Refill(sender);
	}
}

void JFW_Vehicle_Powerup::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	GameObject *o = Get_Vehicle(sender);
	if (o)
	{
		Commands->Send_Custom_Event(obj,o,Get_Int_Parameter("Message"),0,0);
	}
}

void JFW_Soldier_Powerup::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	Commands->Send_Custom_Event(obj,sender,Get_Int_Parameter("Message"),0,0);
}

void JFW_Vehicle_Powerup_Preset::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	GameObject *o = Get_Vehicle(sender);
	if ((o) && (!_stricmp(Commands->Get_Preset_Name(o),Get_Parameter("Preset"))))
	{
		Commands->Send_Custom_Event(obj,o,Get_Int_Parameter("Message"),0,0);
	}
}

void JFW_Powerup_Send_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message"),0,0);
	}
}

void JFW_Powerup_Send_Custom_Team::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		if (!Get_Object_Type(sender))
		{
			Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("NodID")),Get_Int_Parameter("NodMessage"),0,0);
		}
		else
		{
			Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("GDIID")),Get_Int_Parameter("GDIMessage"),0,0);
		}
	}
}

void JFW_Give_Powerup_Create::Created(GameObject *obj)
{
	Commands->Give_PowerUp(obj,Get_Parameter("Powerup"),false);
	Destroy_Script();
}

void JFW_Change_Character_Powerup::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,Get_Parameter("Preset"));
	}
}

void JFW_Change_Character_Powerup_2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		if (Commands->Get_Player_Type(sender) == 0)
		{
			Change_Character(sender,Get_Parameter("Nod_Preset"));
		}
		else if (Commands->Get_Player_Type(sender) == 1)
		{
			Change_Character(sender,Get_Parameter("GDI_Preset"));
		}
	}
}

void JFW_Clear_Money_Create::Created(GameObject *obj)
{
	Commands->Give_Money(obj,-Commands->Get_Money(obj),false);
	Destroy_Script();
}

void JFW_Give_Money::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	int amount;
	bool team;
	msg = Get_Int_Parameter("Message");
	if (msg == type)
	{
		team = Get_Bool_Parameter("Entire_Team");
		amount = Get_Int_Parameter("Amount");
		Commands->Give_Money(sender,(float)amount,team);
	}
}

ScriptRegistrant<JFW_Give_Money> JFW_Give_Money_Registrant("JFW_Give_Money","Message:int,Entire_Team:int,Amount:int");
ScriptRegistrant<JFW_Give_Powerup_Create> JFW_Give_Powerup_Create_Registrant("JFW_Give_Powerup_Create","Powerup:string");
ScriptRegistrant<JFW_Clear_Money_Create> JFW_Clear_Money_Create_Registrant("JFW_Clear_Money_Create","");
ScriptRegistrant<JFW_Spawn_Timer> JFW_Spawn_Timer_Registrant("JFW_Spawn_Timer","Time:float,TimerNum:int,SpawnCustom:int,TriggerCustom:int,TriggerID:int,SpawnPreset:string,SpawnLocation:vector3");
ScriptRegistrant<JFW_Warp_On_Pickup_Team> JFW_Warp_On_Pickup_Team_Registrant("JFW_Warp_On_Pickup_Team","Position:vector3,Player_Type:int");
ScriptRegistrant<JFW_Refill_Powerup> JFW_Refill_Powerup_Registrant("JFW_Refill_Powerup","");
ScriptRegistrant<JFW_Vehicle_Powerup> JFW_Vehicle_Powerup_Registrant("JFW_Vehicle_Powerup","Message:int");
ScriptRegistrant<JFW_Vehicle_Powerup_Preset> JFW_Vehicle_Powerup_Preset_Registrant("JFW_Vehicle_Powerup_Preset","Message:int,Preset:string");
ScriptRegistrant<JFW_Powerup_Send_Custom> JFW_Powerup_Send_Custom_Registrant("JFW_Powerup_Send_Custom","ID:int,Message:int");
ScriptRegistrant<JFW_Powerup_Send_Custom_Team> JFW_Powerup_Send_Custom_Team_Registrant("JFW_Powerup_Send_Custom_Team","GDIID:int,GDIMessage:int,NodID:int,NodMessage:int");
ScriptRegistrant<JFW_Change_Character_Powerup> JFW_Change_Character_Powerup_Registrant("JFW_Change_Character_Powerup","Preset:string");
ScriptRegistrant<JFW_Change_Character_Powerup_2> JFW_Change_Character_Powerup_2_Registrant("JFW_Change_Character_Powerup_2","GDI_Preset:string,Nod_Preset:string");
ScriptRegistrant<JFW_Soldier_Powerup> JFW_Soldier_Powerup_Registrant("JFW_Soldier_Powerup","Message:int");
