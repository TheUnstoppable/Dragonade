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
#include "nh.h"

void NH_Spawn_Object_On_Poke::Created(GameObject *obj)
{
	spawn = 1;
	Commands->Enable_HUD_Pokable_Indicator(obj,1);
}

void NH_Spawn_Object_On_Poke::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (!spawn)
		return;

	const char *build;
	Vector3 position;
	Vector3 spawner;
	build = Get_Parameter("PresetName");
	position = Commands->Get_Position(obj);
	spawner = Get_Vector3_Parameter("Add_Vectors");

	position.X = spawner.X + position.X;
	position.Y = spawner.Y + position.Y;
	position.Z = spawner.Z + position.Z;
	if (!CheckPlayerType(damager,0))
	{
		Commands->Create_Object(build,position);
		spawn = 0;
		Commands->Start_Timer(obj,this,5.00,1);
	}
}

void NH_Spawn_Object_On_Poke::Timer_Expired(GameObject *obj,int number)
{
	spawn = 1;
	Commands->Start_Timer(obj,this,5.00,1);
}

void NH_Spawn_Object_On_Poke::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	spawn = 0;
	Commands->Apply_Damage(obj,10000,"BlamoKiller",0);
}

void NH_SetTeam_OnLeave::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		Commands->Set_Player_Type(obj,Commands->Get_Player_Type(sender));
	}
}

void NH_Deploy_Building_On_Poke::Death(GameObject *obj,GameObject *killer)
{
	const char *build;
	Vector3 position;
	Vector3 spawner;
	build = Get_Parameter("PresetName");
	position = Commands->Get_Position(obj);
	spawner = Get_Vector3_Parameter("Add_Vectors");
	position.X = spawner.X + position.X;
	position.Y = spawner.Y + position.Y;
	position.Z = spawner.Z + position.Z;
	Commands->Set_Facing(Commands->Create_Object(build,position),Commands->Get_Facing(obj));
}

void NH_Create_Buy_Panels::Created(GameObject *obj)
{
	const char *build;
	Vector3 position;
	Vector3 spawner;

	build = Get_Parameter("Panel_1_Name");
	spawner = Commands->Get_Position(obj);
	position = Get_Vector3_Parameter("Panel_1_Position");
	spawner.X = spawner.X + position.X;
	spawner.Y = spawner.Y + position.Y;
	spawner.Z = spawner.Z + position.Z;
	panel1 = Commands->Get_ID(Commands->Create_Object(build,spawner));

	build = Get_Parameter("Panel_2_Name");
	spawner = Commands->Get_Position(obj);
	position = Get_Vector3_Parameter("Panel_2_Position");
	spawner.X = spawner.X + position.X;
	spawner.Y = spawner.Y + position.Y;
	spawner.Z = spawner.Z + position.Z;
	panel2 = Commands->Get_ID(Commands->Create_Object(build,spawner));

	build = Get_Parameter("Panel_3_Name");
	spawner = Commands->Get_Position(obj);
	position = Get_Vector3_Parameter("Panel_3_Position");
	spawner.X = spawner.X + position.X;
	spawner.Y = spawner.Y + position.Y;
	spawner.Z = spawner.Z + position.Z;
	panel3 = Commands->Get_ID(Commands->Create_Object(build,spawner));

	build = Get_Parameter("Panel_4_Name");
	spawner = Commands->Get_Position(obj);
	position = Get_Vector3_Parameter("Panel_4_Position");
	spawner.X = spawner.X + position.X;
	spawner.Y = spawner.Y + position.Y;
	spawner.Z = spawner.Z + position.Z;
	panel4 = Commands->Get_ID(Commands->Create_Object(build,spawner));
}

void NH_Create_Buy_Panels::Death(GameObject *obj,GameObject *killer)
{
	int msg;
	int prm;
	msg = 2223;
	prm = 0;
	Commands->Send_Custom_Event(obj,Commands->Find_Object(panel1),msg,prm,0);
	Commands->Send_Custom_Event(obj,Commands->Find_Object(panel2),msg,prm,0);
	Commands->Send_Custom_Event(obj,Commands->Find_Object(panel3),msg,prm,0);
	Commands->Send_Custom_Event(obj,Commands->Find_Object(panel4),msg,prm,0);
}

void NH_Create_Buy_Panels::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&panel1,4,1);
	Auto_Save_Variable(&panel1,4,2);
	Auto_Save_Variable(&panel1,4,3);
	Auto_Save_Variable(&panel1,4,4);
}

ScriptRegistrant<NH_SetTeam_OnLeave> NH_SetTeam_OnLeave_Registrant("NH_SetTeam_OnLeave","");
ScriptRegistrant<NH_Spawn_Object_On_Poke> NH_Spawn_Object_On_Poke_Registrant("NH_Spawn_Object_On_Poke","PresetName:string,Add_Vectors:vector3");
ScriptRegistrant<NH_Deploy_Building_On_Poke> NH_Deploy_Building_On_Poke_Registrant("NH_Deploy_Building_On_Poke","PresetName:string,Add_Vectors:vector3");
ScriptRegistrant<NH_Create_Buy_Panels> NH_Create_Buy_Panels_Registrant("NH_Create_Buy_Panels","Panel_1_Name:string,Panel_1_Position:vector3,Panel_2_Name:string,Panel_2_Position:vector3,Panel_3_Name:string,Panel_3_Position:vector3,Panel_4_Name:string,Panel_4_Position:vector3");
