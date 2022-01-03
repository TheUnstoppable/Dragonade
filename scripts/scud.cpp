/*	Renegade Scripts.dll
	Copyright 2013 Tiberian Technologies

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
#include "scud.h"

void SCUD_Turret_Spawn::Created(GameObject *obj)
{
	Vector3 sp;
	GameObject *object;
	sp.X = 0;
	sp.Y = 0;
	sp.Z = 0;
	object = Commands->Create_Object(Get_Parameter("Turret_Preset"),sp);
	Commands->Attach_To_Object_Bone(object,obj,Get_Parameter("Bone_Name"));
	turID = Commands->Get_ID(object);
}

void SCUD_Turret_Spawn::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&turID,4,1);
}

void SCUD_Turret_Spawn::Killed(GameObject *obj,GameObject *killer)
{
	Vector3 tmb;
	tmb = Commands->Get_Bone_Position(obj,Get_Parameter("Bone_Name"));
	Commands->Create_Explosion(Get_Parameter("Explosion_Preset"),tmb,0);
}

void SCUD_Harvester_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if ((enabled != false) && (Get_Int_Parameter("Harvester_Preset_ID") == Commands->Get_Preset_ID(enterer)))
	{
		enabled = false;
		Commands->Give_Money(enterer,Get_Float_Parameter("Credits"),true);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Next_Harvester_Time"),1);
	}
	if ((enabled != false) && (Get_Int_Parameter("LvlHarvester_Preset_ID") == Commands->Get_Preset_ID(enterer)))
	{
		enabled = false;
		Commands->Give_Money(enterer,Get_Float_Parameter("Credits"),true);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Next_Harvester_Time"),1);
	}
	if ((enabled != false) && (Get_Int_Parameter("Thief_Preset_ID") == Commands->Get_Preset_ID(enterer)))
	{
		enabled = false;
		Commands->Give_Money(enterer,Get_Float_Parameter("Thief_Credits"),true);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Thief_Time"),1);
	}
}

void SCUD_Harvester_Zone::Timer_Expired(GameObject *obj,int number)
{
	enabled = true;
}

void SCUD_Harvester_Zone::Created(GameObject *obj)
{
	enabled = true;
}

void SCUD_Harvester_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void SCUD_Custom_Follow_Waypath::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	int waypathid;
	float speed;
	waypathid = Get_Int_Parameter("WaypathID");
	speed = Get_Float_Parameter("Speed");
	msg = Get_Int_Parameter("Message");
	if (msg == type)
	{
		ActionParamsStruct params;
		params.MoveArrivedDistance = 100;
		params.Set_Basic(this,100,777);
		params.Set_Movement(0,speed,10);
		params.WaypathID = waypathid;
		params.WaypathSplined = true;
		params.AttackActive = false;
		params.AttackCheckBlocked = false;
		Commands->Action_Goto(obj,params);
	}
}

void SCUD_Custom_Follow_Waypath::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	int waypathid;
	float speed;
	waypathid = Get_Int_Parameter("WaypathID");
	speed = Get_Float_Parameter("Speed");
	ActionParamsStruct params;
	params.MoveArrivedDistance = 100;
	params.Set_Basic(this,100,777);
	params.Set_Movement(0,speed,10);
	params.WaypathID = waypathid;
	params.WaypathSplined = true;
	params.AttackActive = false;
	params.AttackCheckBlocked = false;
	Commands->Action_Goto(obj,params);
}

void SCUD_Follow_Waypath_Until_Custom::Created(GameObject *obj)
{
	int waypathid;
	float speed;
	waypathid = Get_Int_Parameter("WaypathID");
	speed = Get_Float_Parameter("Speed");
	ActionParamsStruct params;
	params.MoveArrivedDistance = 100;
	params.Set_Basic(this,100,777);
	params.Set_Movement(0,speed,10);
	params.WaypathID = waypathid;
	params.WaypathSplined = true;
	params.AttackActive = false;
	params.AttackCheckBlocked = false;
	Commands->Action_Goto(obj,params);
	disable = false;
}

void SCUD_Follow_Waypath_Until_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Message");
	if (msg == type)
	{
		disable = true;
	}
}

void SCUD_Follow_Waypath_Until_Custom::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	if (!disable)
	{
		int waypathid;
		float speed;
		waypathid = Get_Int_Parameter("WaypathID");
		speed = Get_Float_Parameter("Speed");
		ActionParamsStruct params;
		params.MoveArrivedDistance = 100;
		params.Set_Basic(this,100,777);
		params.Set_Movement(0,speed,10);
		params.WaypathID = waypathid;
		params.WaypathSplined = true;
		params.AttackActive = false;
		params.AttackCheckBlocked = false;
		Commands->Action_Goto(obj,params);
	}
}

void SCUD_Follow_Waypath_Until_Custom::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&disable,1,1);
}

void SCUD_Poke_Preset_Buy::Created(GameObject *obj)
{
	active = 1;
	Commands->Enable_HUD_Pokable_Indicator(obj,1);
}

void SCUD_Poke_Preset_Buy::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	active = 1;
	Commands->Enable_HUD_Pokable_Indicator(obj,1);
}

void SCUD_Poke_Preset_Buy::Poked(GameObject *obj,GameObject *poker)
{
	const char *preset;
	int x;
	int cost;
	Vector3 spawn_position;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	preset = Get_Parameter("Preset_Name");
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(poker))
	{
		cost = -cost;
		spawn_position = Get_Vector3_Parameter("location");
		Commands->Give_Money(poker,(float)cost,0);
		Commands->Create_Object(preset,spawn_position);
	}
}

void SCUD_Poke_Preset_Buy::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&active,1,1);
}

void SCUD_Preset_Buy::Created(GameObject *obj)
{
	active = 1;
	Commands->Enable_HUD_Pokable_Indicator(obj,1);
}

void SCUD_Preset_Buy::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	const char *preset;
	GameObject *npre;
	int x;
	int cost;
	float facing;
	Vector3 spawn_position;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(sender,x))
	{
		return;
	}
	cost = Get_Int_Parameter("Cost");
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (cost <= Commands->Get_Money(sender))
		{
			cost = -cost;
			preset = Get_Parameter("Preset_Name");
			spawn_position = Get_Vector3_Parameter("location");
			Commands->Give_Money(sender,(float)cost,0);
			npre = Commands->Create_Object(preset,spawn_position);
			facing = Get_Float_Parameter("Rotation");
			Commands->Set_Facing(npre,facing);
		}
	}
}

void SCUD_Preset_Buy::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&active,1,1);
}

void SCUD_CustomSend_Preset_Buy::Created(GameObject *obj)
{
	active = 1;
	Commands->Enable_HUD_Pokable_Indicator(obj,1);
}

void SCUD_CustomSend_Preset_Buy::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	const char *preset;
	GameObject *npre;
	int x;
	int cost;
	float facing;
	Vector3 spawn_position;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(sender,x))
	{
		return;
	}
	cost = Get_Int_Parameter("Cost");
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("SendID")),Get_Int_Parameter("SendMessage"),Get_Int_Parameter("SendParam"),0);
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (cost <= Commands->Get_Money(sender))
		{
			cost = -cost;
			preset = Get_Parameter("Preset_Name");
			spawn_position = Get_Vector3_Parameter("location");
			Commands->Give_Money(sender,(float)cost,0);
			npre = Commands->Create_Object(preset,spawn_position);
			facing = Get_Float_Parameter("Rotation");
			Commands->Set_Facing(npre,facing);
		}
	}
}

void SCUD_CustomSend_Preset_Buy::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&active,1,1);
}

void SCUD_CustomSend_PresetBuy_FollowWaypath::Created(GameObject *obj)
{
	active = 1;
	Commands->Enable_HUD_Pokable_Indicator(obj,1);
}

void SCUD_CustomSend_PresetBuy_FollowWaypath::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	const char *preset;
	GameObject *npre;
	int x;
	int cost;
	float facing;
	Vector3 spawn_position;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(sender,x))
	{
		return;
	}
	cost = Get_Int_Parameter("Cost");
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("SendID")),Get_Int_Parameter("SendMessage"),Get_Int_Parameter("SendParam"),0);
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (cost <= Commands->Get_Money(sender))
		{
			cost = -cost;
			preset = Get_Parameter("Preset_Name");
			spawn_position = Get_Vector3_Parameter("location");
			Commands->Give_Money(sender,(float)cost,0);
			npre = Commands->Create_Object(preset,spawn_position);
			facing = Get_Float_Parameter("Rotation");
			Commands->Set_Facing(npre,facing);
			if (Get_Int_Parameter("Waypath_enabled") == 1)
			{
				int waypathid;
				float speed;
				waypathid = Get_Int_Parameter("WaypathID");
				speed = Get_Float_Parameter("Speed");
				ActionParamsStruct params;
				params.MoveArrivedDistance = 100;
				params.Set_Basic(this,100,777);
				params.Set_Movement(0,speed,10);
				params.WaypathID = waypathid;
				params.WaypathSplined = true;
				params.AttackActive = false;
				params.AttackCheckBlocked = false;
				Commands->Action_Goto(npre,params);
			}
		}
	}
}

void SCUD_CustomSend_PresetBuy_FollowWaypath::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&active,1,1);
}

void SCUD_CustomSend_PresetBuy_GeneralPresets_FollowWaypath::Created(GameObject *obj)
{
	active = 1;
	Commands->Enable_HUD_Pokable_Indicator(obj,1);
}

void SCUD_CustomSend_PresetBuy_GeneralPresets_FollowWaypath::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	const char *preset;
	const char *preset1;
	const char *preset2;
	const char *preset3;
	GameObject *npre;
	GameObject *npre1;
	GameObject *npre2;
	GameObject *npre3;
	int x;
	int cost;
	int cost1;
	int cost2;
	int cost3;
	float facing;
	Vector3 spawn_position;
	int senderid;
	int generalid1;
	int generalid2;
	int generalid3;
	senderid = Commands->Get_Preset_ID(sender);
	generalid1 = Get_Int_Parameter("General1_Preset_ID");
	generalid2 = Get_Int_Parameter("General2_Preset_ID");
	generalid3 = Get_Int_Parameter("General3_Preset_ID");
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(sender,x))
	{
		return;
	}
	cost = Get_Int_Parameter("Cost");
	cost1 = Get_Int_Parameter("General1_Preset_Cost");
	cost2 = Get_Int_Parameter("General2_Preset_Cost");
	cost3 = Get_Int_Parameter("General3_Preset_Cost");
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("SendID")),Get_Int_Parameter("SendMessage"),Get_Int_Parameter("SendParam"),0);
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if ((senderid != generalid1) && (senderid != generalid2) && (senderid != generalid3))
		{
			if (cost <= Commands->Get_Money(sender))
			{
				cost = -cost;
				preset = Get_Parameter("Preset_Name");
				spawn_position = Get_Vector3_Parameter("location");
				Commands->Give_Money(sender,(float)cost,0);
				npre = Commands->Create_Object(preset,spawn_position);
				facing = Get_Float_Parameter("Rotation");
				Commands->Set_Facing(npre,facing);
				if (Get_Int_Parameter("Waypath_enabled") == 1)
				{
					int waypathid;
					float speed;
					waypathid = Get_Int_Parameter("WaypathID");
					speed = Get_Float_Parameter("Speed");
					ActionParamsStruct params;
					params.MoveArrivedDistance = 100;
					params.Set_Basic(this,100,777);
					params.Set_Movement(0,speed,10);
					params.WaypathID = waypathid;
					params.WaypathSplined = true;
					params.AttackActive = false;
					params.AttackCheckBlocked = false;
					Commands->Action_Goto(npre,params);
				}
			}
		}
		if (senderid == generalid1)
		{
			if (cost1 <= Commands->Get_Money(sender))
			{
				cost1 = -cost1;
				preset1 = Get_Parameter("General1_Preset_Name");
				spawn_position = Get_Vector3_Parameter("location");
				Commands->Give_Money(sender,(float)cost1,0);
				npre1 = Commands->Create_Object(preset1,spawn_position);
				facing = Get_Float_Parameter("Rotation");
				Commands->Set_Facing(npre1,facing);
				if (Get_Int_Parameter("Waypath_enabled") == 1)
				{
					int waypathid;
					float speed;
					waypathid = Get_Int_Parameter("WaypathID");
					speed = Get_Float_Parameter("Speed");
					ActionParamsStruct params;
					params.MoveArrivedDistance = 100;
					params.Set_Basic(this,100,777);
					params.Set_Movement(0,speed,10);
					params.WaypathID = waypathid;
					params.WaypathSplined = true;
					params.AttackActive = false;
					params.AttackCheckBlocked = false;
					Commands->Action_Goto(npre1,params);
				}
			}
		}
		if (senderid == generalid2)
		{
			if (cost2 <= Commands->Get_Money(sender))
			{
				cost2 = -cost2;
				preset2 = Get_Parameter("General2_Preset_Name");
				spawn_position = Get_Vector3_Parameter("location");
				Commands->Give_Money(sender,(float)cost2,0);
				npre2 = Commands->Create_Object(preset2,spawn_position);
				facing = Get_Float_Parameter("Rotation");
				Commands->Set_Facing(npre2,facing);
				if (Get_Int_Parameter("Waypath_enabled") == 1)
				{
					int waypathid;
					float speed;
					waypathid = Get_Int_Parameter("WaypathID");
					speed = Get_Float_Parameter("Speed");
					ActionParamsStruct params;
					params.MoveArrivedDistance = 100;
					params.Set_Basic(this,100,777);
					params.Set_Movement(0,speed,10);
					params.WaypathID = waypathid;
					params.WaypathSplined = true;
					params.AttackActive = false;
					params.AttackCheckBlocked = false;
					Commands->Action_Goto(npre2,params);
				}
			}
		}
		if (senderid == generalid3)
		{
			if (cost3 <= Commands->Get_Money(sender))
			{
				cost3 = -cost3;
				preset3 = Get_Parameter("General3_Preset_Name");
				spawn_position = Get_Vector3_Parameter("location");
				Commands->Give_Money(sender,(float)cost3,0);
				npre3 = Commands->Create_Object(preset3,spawn_position);
				facing = Get_Float_Parameter("Rotation");
				Commands->Set_Facing(npre3,facing);
				if (Get_Int_Parameter("Waypath_enabled") == 1)
				{
					int waypathid;
					float speed;
					waypathid = Get_Int_Parameter("WaypathID");
					speed = Get_Float_Parameter("Speed");
					ActionParamsStruct params;
					params.MoveArrivedDistance = 100;
					params.Set_Basic(this,100,777);
					params.Set_Movement(0,speed,10);
					params.WaypathID = waypathid;
					params.WaypathSplined = true;
					params.AttackActive = false;
					params.AttackCheckBlocked = false;
					Commands->Action_Goto(npre3,params);
				}
			}
		}

	}
}

void SCUD_CustomSend_PresetBuy_GeneralPresets_FollowWaypath::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&active,1,1);
}

void SCUD_Deployable_Vehicle::Created(GameObject *obj)
{
	pilotID = 0;
}

void SCUD_Deployable_Vehicle::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!pilotID)
		{
			pilotID = Commands->Get_ID(sender);
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (pilotID == Commands->Get_ID(sender))
		{
			pilotID = 0;
		}
	}
}

void SCUD_Deployable_Vehicle::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	const char *preset;
	GameObject *npre;
	Vector3 pos;
	float health;
	float facing;
	if ((damager == Commands->Find_Object(pilotID)) && (damager))
	{
		preset = Get_Parameter("Animated_Preset");
		pos = Commands->Get_Position(obj);
		health = Commands->Get_Health(obj);
		npre = Commands->Create_Object(preset,pos);
		Commands->Set_Health(npre,health);
		health = Commands->Get_Shield_Strength(obj);
		Commands->Set_Shield_Strength(npre,health);
		facing = Commands->Get_Facing(obj);
		Commands->Set_Facing(npre,facing);
 		preset = Get_Parameter("Explosion_Preset");
		Commands->Create_Explosion(preset,pos,0);
	}
}

void SCUD_Deployable_Vehicle::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&pilotID,4,1);
}

void SCUD_Deploy_Animation::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time_Till_Death") ,1);
}

void SCUD_Deploy_Animation::Timer_Expired(GameObject *obj,int number)
{
	const char *preset;
	GameObject *npre;
	Vector3 pos;
	float health;
	float facing;
	preset = Get_Parameter("Deployed_Tank_Preset");
	pos = Commands->Get_Position(obj);
	health = Commands->Get_Health(obj);
	npre = Commands->Create_Object(preset,pos);
	Commands->Set_Health(npre,health);
	health = Commands->Get_Shield_Strength(obj);
	Commands->Set_Shield_Strength(npre,health);
	facing = Commands->Get_Facing(obj);
	Commands->Set_Facing(npre,facing);
 	preset = Get_Parameter("Explosion_Preset");
	Commands->Create_Explosion(preset,pos,0);
}

void SCUD_DeathZoneOnCustom::Entered(GameObject *obj,GameObject *enterer)
{
	visitor = Commands->Get_ID(enterer);
}

void SCUD_DeathZoneOnCustom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	const char *exp;
	msg = Get_Int_Parameter("Message");
	exp = Get_Parameter("Explosion");
	if (type == msg)
	{
		Commands->Create_Explosion(exp,Commands->Get_Position(Commands->Find_Object(visitor)),0);
	}
}

void SCUD_DeathZoneOnCustom::Exited(GameObject *obj,GameObject *exiter)
{
	visitor = 0;
}

void SCUD_Harvester_Control::Created(GameObject *obj)
{
	USHarvLim = 0;
	CHHarvLim = 0;
	uslimreacht = 0;
	chlimreacht = 0;
}

void SCUD_Harvester_Control::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&USHarvLim,4,1);
	Auto_Save_Variable(&CHHarvLim,4,1);
	Auto_Save_Variable(&uslimreacht,4,1);
	Auto_Save_Variable(&chlimreacht,4,1);
}

void SCUD_Harvester_Control::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	GameObject *send;
	int msg;
	int sparam;
	int usterminal;
	int chterminal;
	int uslimit;
	int chlimit;
	int uscashmsg;
	int chcashmsg;
	usterminal = Get_Int_Parameter("US_Harvester_Terminal_ID");
	chterminal = Get_Int_Parameter("CH_Harvester_Terminal_ID");
	uslimit = Get_Int_Parameter("USA_Harvester_Limit");
	chlimit = Get_Int_Parameter("China_Harvester_Limit");
	uscashmsg = Get_Int_Parameter("USA_Harvester_Cost");
	chcashmsg = Get_Int_Parameter ("China_Harvester_Cost");
	msg = 3054;
	sparam = 0;
	send = sender;
	if (uslimreacht == 1)
	{
		if (type == 1919)
		{
			Commands->Give_Money(Commands->Find_Object(param),(float)uscashmsg,0);
		}
	}
	if (chlimreacht == 1)
	{
		if (type == 1920)
		{
			Commands->Give_Money(Commands->Find_Object(param),(float)chcashmsg,0);
		}
	}
	if (USHarvLim < uslimit)
	{
		uslimreacht = 0;
	}
	if (CHHarvLim < chlimit)
	{
		chlimreacht = 0;
	}
	if (USHarvLim >= uslimit)
	{
		uslimreacht = 1;
	}
	if (CHHarvLim >= chlimit)
	{
		chlimreacht = 1;
	}
	if (type == 1015)
	{
		USHarvLim++;
		if (USHarvLim >= uslimit)
		{
			Commands->Send_Custom_Event(obj,sender,msg,sparam,0);
		}
	}
	if (type == 1016)
	{
		USHarvLim--;
		if (USHarvLim < uslimit)
		{
		}
	}
	
	if (type == 1025)
	{
		CHHarvLim++;
		if (CHHarvLim >= chlimit)
			Commands->Send_Custom_Event(obj,sender,msg,sparam,0);
		}
	if (type == 1026)
	{
		CHHarvLim--;
		if (CHHarvLim < chlimit)
		{
		}
	}
}

void SCUD_USHarvester::Created(GameObject *obj)
{
	GameObject *send;
	int msg;
	int sparam;
	HarvCtrlID = Get_Int_Parameter("Harvester_Control_ID");
	msg = 1015;
	sparam = 0;
	send = Commands->Find_Object(HarvCtrlID);
	Commands->Send_Custom_Event(obj,send,msg,sparam,0);
}

void SCUD_USHarvester::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *send;
	int msg;
	int sparam;
	msg = 1016;
	sparam = 0;
	send = Commands->Find_Object(HarvCtrlID);
	Commands->Send_Custom_Event(obj,send,msg,sparam,0);
}

void SCUD_USHarvester::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	Vector3 v;
	v = Commands->Get_Position(obj);
	if (type == 3054)
	{
		Commands->Create_Explosion(Get_Parameter("Explosion_Preset_Name"),v,0);
	}
}

void SCUD_CHHarvester::Created(GameObject *obj)
{
	GameObject *send;
	int msg;
	int sparam;
	HarvCtrlID = Get_Int_Parameter("Harvester_Control_ID");
	msg = 1025;
	sparam = 0;
	send = Commands->Find_Object(HarvCtrlID);
	Commands->Send_Custom_Event(obj,send,msg,sparam,0);
}

void SCUD_CHHarvester::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	Vector3 v;
	v = Commands->Get_Position(obj);
	if (type == 3054)
	{
		Commands->Create_Explosion(Get_Parameter("Explosion_Preset_Name"),v,0);
	}
}

void SCUD_CHHarvester::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *send;
	int msg;
	int sparam;
	msg = 1026;
	sparam = 0;
	send = Commands->Find_Object(HarvCtrlID);
	Commands->Send_Custom_Event(obj,send,msg,sparam,0);
}

void SCUD_USHarvester_Terminal::Created(GameObject *obj)
{
	active = 1;
	Commands->Enable_HUD_Pokable_Indicator(obj,1);
	visitorID = 0;
	
}

void SCUD_USHarvester_Terminal::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	const char *preset;
	GameObject *npre;
	int x;
	int cost;
	float facing;
	Vector3 spawn_position;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(sender,x))
	{
		return;
	}
	cost = Get_Int_Parameter("Cost");
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("SendID")),Get_Int_Parameter("SendMessage"),Get_Int_Parameter("SendParam"),0);
		visitorID = Commands->Get_ID(sender);
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (cost <= Commands->Get_Money(sender))
		{
			cost = -cost;
			preset = Get_Parameter("Preset_Name");
			spawn_position = Get_Vector3_Parameter("location");
			Commands->Give_Money(sender,(float)cost,0);
			npre = Commands->Create_Object(preset,spawn_position);
			facing = Get_Float_Parameter("Rotation");
			Commands->Set_Facing(npre,facing);
			Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("Harvester_Control_ID")),1919,visitorID,0);
			if (Get_Int_Parameter("Waypath_enabled") == 1)
			{
				int waypathid;
				float speed;
				waypathid = Get_Int_Parameter("WaypathID");
				speed = Get_Float_Parameter("Speed");
				ActionParamsStruct params;
				params.MoveArrivedDistance = 100;
				params.Set_Basic(this,100,777);
				params.Set_Movement(0,speed,10);
				params.WaypathID = waypathid;
				params.WaypathSplined = true;
				params.AttackActive = false;
				params.AttackCheckBlocked = false;
				Commands->Action_Goto(npre,params);
			}
		}
	}
}

void SCUD_USHarvester_Terminal::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&active,1,1);
}

void SCUD_CHHarvester_Terminal::Created(GameObject *obj)
{
	active = 1;
	Commands->Enable_HUD_Pokable_Indicator(obj,1);
	visitorID = 0;
}

void SCUD_CHHarvester_Terminal::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	const char *preset;
	GameObject *npre;
	int x;
	int cost;
	float facing;
	Vector3 spawn_position;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(sender,x))
	{
		return;
	}
	cost = Get_Int_Parameter("Cost");
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("SendID")),Get_Int_Parameter("SendMessage"),Get_Int_Parameter("SendParam"),0);
		visitorID = Commands->Get_ID(sender);
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (cost <= Commands->Get_Money(sender))
		{
			cost = -cost;
			preset = Get_Parameter("Preset_Name");
			spawn_position = Get_Vector3_Parameter("location");
			Commands->Give_Money(sender,(float)cost,0);
			npre = Commands->Create_Object(preset,spawn_position);
			facing = Get_Float_Parameter("Rotation");
			Commands->Set_Facing(npre,facing);
			Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("Harvester_Control_ID")),1920,visitorID,0);
			if (Get_Int_Parameter("Waypath_enabled") == 1)
			{
				int waypathid;
				float speed;
				waypathid = Get_Int_Parameter("WaypathID");
				speed = Get_Float_Parameter("Speed");
				ActionParamsStruct params;
				params.MoveArrivedDistance = 100;
				params.Set_Basic(this,100,777);
				params.Set_Movement(0,speed,10);
				params.WaypathID = waypathid;
				params.WaypathSplined = true;
				params.AttackActive = false;
				params.AttackCheckBlocked = false;
				Commands->Action_Goto(npre,params);
			}
		}
	}
}

void SCUD_CHHarvester_Terminal::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&active,1,1);
}

void SCUD_SuperWeapon::Created(GameObject *obj)
{
	counter=0;
}

void SCUD_SuperWeapon::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	counter++;
	if (counter == Get_Int_Parameter("SuperWeaponAmmo"))
	{
		Commands->Send_Custom_Event(obj,obj,5432,0,Get_Float_Parameter("FireTimeSinceFirstShot"));
	}
}

void SCUD_SuperWeapon::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 5432)
	{
		Commands->Destroy_Object(obj);
	}
}

void SCUD_Set_Player_Type_OnCustom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	int pltype;
	pltype = Get_Int_Parameter("Player_Type");
	if (pltype == 2)
	{
		pltype = Commands->Get_Player_Type(sender);
	}
	msg = Get_Int_Parameter("Message");
	if (type == msg)
	{
		Commands->Set_Player_Type(obj,pltype);
	}
}

void SCUD_DozerBuildTurret::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!PilotID)
		{
			PilotID = Commands->Get_ID(sender);
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (PilotID == Commands->Get_ID(sender))
		{
			PilotID = 0;
		}
	}
}

void SCUD_DozerBuildTurret::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	const char *preset;
	GameObject *npre;
	int cost;
	float facing;
	Vector3 spawn_position;
	cost = Get_Int_Parameter("Cost");
	if ((damager == Commands->Find_Object(PilotID)) && (damager))
	{
		if (cost <= Commands->Get_Money(damager))
		{
			cost = -cost;
			preset = Get_Parameter("Turret_PresetName");
			spawn_position = Commands->Get_Position(damager);
			spawn_position.Z = spawn_position.Z - 2;
			Commands->Give_Money(damager,(float)cost,0);
			Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("Controller_ID")),1920,PilotID,0);
			npre = Commands->Create_Object(preset,spawn_position);
			facing = Commands->Get_Facing(damager);
			Commands->Set_Facing(npre,facing);
		}
	}
}

void SCUD_TurretBuildup::Created(GameObject *obj)
{
	int msg;
	int param;
	
	msg = 1010;
	param = 0;
	Commands->Send_Custom_Event(obj,obj,msg,param,Get_Float_Parameter("Buildup_Time"));
}

void SCUD_TurretBuildup::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	GameObject *turret;
	const char *preset;
	if (type == 1010)
	{
		preset = Get_Parameter("Turret_PresetName");
		turret = Commands->Create_Object(preset,Commands->Get_Position(obj));
		Commands->Set_Facing(turret,Commands->Get_Facing(obj));
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<SCUD_Turret_Spawn> SCUD_Turret_Spawn_Registrant("SCUD_Turret_Spawn","Turret_Preset:string,Bone_Name=BoneName:string,Explosion_Preset:string");
ScriptRegistrant<SCUD_Harvester_Zone> SCUD_Harvester_Zone_Registrant("SCUD_Harvester_Zone","Credits=1000.00:float,Next_Harvester_Time=5.00:float,Harvester_Preset_ID:int,LvlHarvester_Preset_ID:int,Thief_Credits=500.00:float,Thief_Time=5.00:float,Thief_Preset_ID:int");
ScriptRegistrant<SCUD_Custom_Follow_Waypath> SCUD_Custom_Follow_Waypath_Registrant("SCUD_Custom_Follow_Waypath","WaypathID:int,Speed:float,Message:int");
ScriptRegistrant<SCUD_Follow_Waypath_Until_Custom> SCUD_Follow_Waypath_Until_Custom_Registrant("SCUD_Follow_Waypath_Until_Custom","WaypathID:int,Speed:float,Message:int");
ScriptRegistrant<SCUD_Poke_Preset_Buy> SCUD_Poke_Preset_Buy_Registrant("SCUD_Poke_Preset_Buy","Unknown:float,Preset_Name:string,Cost:int,location:vector3,Player_Type:int");
ScriptRegistrant<SCUD_Preset_Buy> SCUD_Preset_Buy_Registrant("SCUD_Preset_Buy","Unknown:float,Preset_Name:string,Cost:int,location:vector3,Player_Type:int,Rotation:float");
ScriptRegistrant<SCUD_CustomSend_Preset_Buy> SCUD_CustomSend_Preset_Buy_Registrant("SCUD_CustomSend_Preset_Buy","Unknown:float,SendID:int,SendMessage:int,SendParam:int,Preset_Name:string,Cost:int,location:vector3,Player_Type:int,Rotation:float");
ScriptRegistrant<SCUD_CustomSend_PresetBuy_FollowWaypath> SCUD_CustomSend_PresetBuy_FollowWaypath_Registrant("SCUD_CustomSend_PresetBuy_FollowWaypath","Unknown:float,SendID:int,SendMessage:int,SendParam:int,Preset_Name:string,Cost:int,location:vector3,Player_Type:int,Rotation:float,Waypath_enabled:int,WaypathID:int,Speed:float");
ScriptRegistrant<SCUD_CustomSend_PresetBuy_GeneralPresets_FollowWaypath> SCUD_CustomSend_PresetBuy_GeneralPresets_FollowWaypath_Registrant("SCUD_CustomSend_PresetBuy_GeneralPresets_FollowWaypath","Unknown:float,SendID:int,SendMessage:int,SendParam:int,Preset_Name:string,Cost:int,location:vector3,Player_Type:int,Rotation:float,General1_Preset_ID:int,General2_Preset_ID:int,General3_Preset_ID:int,General1_Preset_Name:string,General2_Preset_Name:string,General3_Preset_Name:string,General1_Preset_Cost:int,General2_Preset_Cost:int,General3_Preset_Cost:int,Waypath_enabled:int,WaypathID:int,Speed:float");
ScriptRegistrant<SCUD_Deployable_Vehicle> SCUD_Deployable_Vehicle_Registrant("SCUD_Deployable_Vehicle","Animated_Preset:string,Explosion_Preset:string");
ScriptRegistrant<SCUD_Deploy_Animation> SCUD_Deploy_Animation_Registrant("SCUD_Deploy_Animation","Time_Till_Death:float,Deployed_Tank_Preset:string,Explosion_Preset:string");
ScriptRegistrant<SCUD_DeathZoneOnCustom> SCUD_DeathZoneOnCustom_Registrant("SCUD_DeathZoneOnCustom","Message:int,Explosion:string");
ScriptRegistrant<SCUD_Harvester_Control> SCUD_Harvester_Control_Registrant("SCUD_Harvester_Control","USA_Harvester_Limit:int,China_Harvester_Limit:int,US_Harvester_Terminal_ID:int,CH_Harvester_Terminal_ID:int,USA_Harvester_Cost:int,China_Harvester_Cost:int");
ScriptRegistrant<SCUD_USHarvester> SCUD_USHarvester_Registrant("SCUD_USHarvester","Harvester_Control_ID:int,Explosion_Preset_Name:string");
ScriptRegistrant<SCUD_CHHarvester> SCUD_CHHarvester_Registrant("SCUD_CHHarvester","Harvester_Control_ID:int,Explosion_Preset_Name:string");
ScriptRegistrant<SCUD_USHarvester_Terminal> SCUD_USHarvester_Terminal_Registrant("SCUD_USHarvester_Terminal","Unknown:float,SendID:int,SendMessage:int,SendParam:int,Preset_Name:string,Cost:int,location:vector3,Player_Type:int,Rotation:float,Waypath_enabled:int,WaypathID:int,Speed:float,Harvester_Control_ID:int");
ScriptRegistrant<SCUD_CHHarvester_Terminal> SCUD_CHHarvester_Terminal_Registrant("SCUD_CHHarvester_Terminal","Unknown:float,SendID:int,SendMessage:int,SendParam:int,Preset_Name:string,Cost:int,location:vector3,Player_Type:int,Rotation:float,Waypath_enabled:int,WaypathID:int,Speed:float,Harvester_Control_ID:int");
ScriptRegistrant<SCUD_Set_Player_Type_OnCustom> SCUD_Set_Player_Type_OnCustom_Registrant("SCUD_Set_Player_Type_OnCustom","Message:int,Player_Type:int");
ScriptRegistrant<SCUD_SuperWeapon> SCUD_SuperWeapon_Registrant("SCUD_SuperWeapon","SuperWeaponAmmo:int,FireTimeSinceFirstShot:float");
ScriptRegistrant<SCUD_DozerBuildTurret> SCUD_DozerBuildTurret_Registrant("SCUD_DozerBuildTurret","Turret_PresetName:string,Cost:int,Controller_ID:int");
ScriptRegistrant<SCUD_TurretBuildup> SCUD_TurretBuildup_Registrant("SCUD_TurretBuildup","Buildup_Time:float,Turret_PresetName:string");
