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
#include "kamuix.h"

void Kamuix_PAMSG_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{	
		return;
	}
	if (!Commands->Is_A_Star(enterer))
	{
		return;
	}
	char message[1000];
	sprintf(message,"pamsg %d %s",Get_Player_ID(enterer),Get_Parameter("Message"));
	Console_Input(message);
}

void Kamuix_Send_FDS_MSG_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	char message[100];
	sprintf(message,"%s",Get_Parameter("Message"));
	Console_Input(message);
}

void Kamuix_Team_Change_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	if (!Commands->Is_A_Star(enterer))
	{
		return;
	}
	char message[1000];
	sprintf(message,"team2 %d %s",Get_Player_ID(enterer),Get_Parameter("Team"));
	Console_Input(message);
}

void Kamuix_Announce_Preset_Buy::Entered(GameObject *obj,GameObject *enterer)
{
	const char *preset;
	int x;
	int cost;
	Vector3 spawn_position;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	char message[1000];
	preset = Get_Parameter("Preset_Name");
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(enterer))
	{
		cost = -cost;
		spawn_position = Get_Vector3_Parameter("location");
		Commands->Give_Money(enterer,(float)cost,0);
		Commands->Create_Object(preset,spawn_position);
		const char *c = Get_Player_Name(enterer);
		sprintf(message,"Msg %s Has Purchased A %s",c,Get_Parameter("DisplayName"));
		delete[] c;
		Console_Input(message);
	}
	else
	{
		sprintf(message,"ppage %d You do not have enouph money to purchase this!!",Get_Player_ID(enterer));
		Console_Input(message);
	}
}

void Kamuix_Death_Announce::Killed(GameObject *obj,GameObject *killer)
{
	char message[1000];
	const char *str = Get_Player_Name(killer);
	sprintf(message,"msg Player %s has destroyed the %s",str,Get_Parameter("DestroyedName"));
	delete[] str;
	Console_Input(message);
}

void Kamuix_Kill_Change::Killed(GameObject *obj,GameObject *killer)
{
	if (Get_Object_Type(obj) == 1)
	{
		char message[1000];
		sprintf(message,"Team2 %d 0",Get_Player_ID(obj));
		Console_Input(message);
	}
	if (Get_Object_Type(obj) == 0)
	{
		char message[1000];
		sprintf(message,"Team2 %d %d",Get_Player_ID(obj),Get_Int_Parameter("Team")); //Mismatch between const char* and %d printf type, don't do it again.
		Console_Input(message);
	}
}

void Kamuix_Death_Team_Win::Killed(GameObject *obj,GameObject *killer)
{
	char message[1000];
	sprintf(message,"win %d",Get_Int_Parameter("Team"));
	Console_Input(message);
}
void Kamuix_Damaged_Send_MsgFds::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	char message[1000];
	sprintf(message,"%s",Get_Parameter("FDSMessage"));
	Console_Input(message);
}

void Kamuix_Death_Send_MsgFds::Killed(GameObject *obj,GameObject *killer)
{
	char message[1000];
	sprintf(message,"%s",Get_Parameter("FDSMessage"));
	Console_Input(message);
}

void Kamuix_Player_Announce_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	char message[1000];
	const char *c = Get_Player_Name(enterer);
	sprintf(message,"msg %s %s",c,Get_Parameter("Message"));
	delete[] c;
	Console_Input(message);
}

void Kamuix_Ban_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int pltype;
	pltype = Get_Int_Parameter("Player_Type");
	char message[1000];
	sprintf(message,"ban %d",Get_Player_ID(enterer));
	Console_Input(message);
}
void Kamuix_Kick_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int pltype;
	pltype = Get_Int_Parameter("Player_Type");
	char message[1000];
	sprintf(message,"kick %d",Get_Player_ID(enterer));
	Console_Input(message);
}
void Zone_Kamuix_Destroy_Object::Entered(GameObject *obj,GameObject *enterer)
{
	Commands->Destroy_Object(Commands->Find_Object(Get_Int_Parameter("ID")));
}

void Kamuix_Set_Type::Created(GameObject *obj)
{
	Commands->Set_Player_Type(obj,Get_Int_Parameter("PlayerType"));
}

void Kamuix_Revive_Building_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	Commands->Set_Health(Commands->Find_Object(Get_Int_Parameter("ID")),500);
}

void Kamuix_Set_Model::Created(GameObject *obj)
{
	Commands->Set_Model(obj,Get_Parameter("Model"));
}

void Kamuix_Set_Building_Health_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (Commands->Get_Player_Type(enterer) == x)
	{
		Commands->Set_Health(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Float_Parameter("Health"));
	}
}

void Kamuix_Death_Destroy_Building::Killed(GameObject *obj,GameObject *enterer)
{
	int id;
	GameObject *object;
	id = Get_Int_Parameter("Id");
	object = Commands->Find_Object(id);
	Commands->Apply_Damage(object,10000,"Explosive",0);
}

void Kamuix_Zone_Destroy_Building::Entered(GameObject *obj,GameObject *enterer)
{
	int id;
	GameObject *ob;
	id = Get_Int_Parameter("Id");
	ob = Commands->Find_Object(id);
	Commands->Apply_Damage(ob,10000,"Explosive",0);
}

void Kamuix_Control_Spawner::Killed(GameObject *obj,GameObject *enterer)
{
	Commands->Enable_Spawner(Get_Int_Parameter("ID"),Get_Bool_Parameter("Enable"));
}

void Kamuix_Rebuild_Structure_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	const char *preset;
	int x;
	char msg[1024];
	int cost;
	Vector3 spawn_position;
	preset = Get_Parameter("Preset");
	spawn_position = Get_Vector3_Parameter("Location");
	cost = Get_Int_Parameter("Cost");
	x = Get_Int_Parameter("Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	if (cost <= Commands->Get_Money(enterer))
	{
		cost = -cost;
		Commands->Give_Money(enterer,(float)cost,0);
		Commands->Set_Health(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Float_Parameter("Health"));
		Commands->Create_Object(preset,spawn_position);
		const char *c = Get_Player_Name(enterer);
		sprintf(msg,"msg Player %s has built a %s",c,Get_Parameter("Announcement"));
		delete[] c;
		Console_Input(msg);
		sprintf(msg,"SNDT %s 0",Get_Parameter("Teamsound"));
		Console_Input(msg);
	}
	if (cost >= Commands->Get_Money(enterer))
	{
		sprintf(msg,"ppage %d %s",Get_Player_ID(enterer),Get_Parameter("page"));
		Console_Input(msg);
		sprintf(msg,"pamsg %d %s",Get_Player_ID(enterer),Get_Parameter("Popmsg"));
		Console_Input(msg);
	}
}

ScriptRegistrant<Kamuix_PAMSG_Zone> Kamuix_PAMSG_Zone_Registrant("Kamuix_PAMSG_Zone","Player_Type:int,Message:string");
ScriptRegistrant<Kamuix_Send_FDS_MSG_Zone> Kamuix_Send_FDS_MSG_Zone_Registrant("Kamuix_Send_FDS_MSG_Zone","Message:string");
ScriptRegistrant<Kamuix_Team_Change_Zone> Kamuix_Team_Change_Zone_Registrant("Kamuix_Team_Change_Zone","Player_Type:int,Team:int");
ScriptRegistrant<Kamuix_Announce_Preset_Buy> Kamuix_Announce_Preset_Buy_Registrant("Kamuix_Announce_Preset_Buy","Preset_Name:string,Cost:int,location:vector3,Player_Type:int,DisplayName:String");
ScriptRegistrant<Kamuix_Death_Announce> Kamuix_Death_Announce_Registrant("Kamuix_Death_Announce","DestroyedName:string");
ScriptRegistrant<Kamuix_Damaged_Send_MsgFds> Kamuix_Damaged_Send_MsgFds_Registrant("Kamuix_Damaged_Send_MsgFds","FDSMessage:string");
ScriptRegistrant<Kamuix_Death_Send_MsgFds> Kamuix_Death_Send_MsgFds_Registrant("Kamuix_Death_Send_MsgFds","FDSMessage:string");
ScriptRegistrant<Kamuix_Death_Team_Win> Kamuix_Death_Team_Win_Registrant("Kamuix_Death_Team_Win","Team:int");
ScriptRegistrant<Kamuix_Kill_Change> Kamuix_Kill_Change_Registrant("Kamuix_Kill_Change","Team:int");
ScriptRegistrant<Kamuix_Player_Announce_Zone> Kamuix_Player_Announce_Zone_Registrant("Kamuix_Player_Announce_Zone","Message:string");
ScriptRegistrant<Kamuix_Ban_Zone> Kamuix_Player_Ban_Zone_Registrant("Kamuix_Ban_Zone","Player_type:int");
ScriptRegistrant<Kamuix_Kick_Zone> Kamuix_Player_Kick_Zone_Registrant("Kamuix_Kick_Zone","Player_type:int");
ScriptRegistrant<Zone_Kamuix_Destroy_Object> Zone_Kamuix_Destroy_Object_Registrant("Zone_Kamuix_Destroy_Object","ID:int");
ScriptRegistrant<Kamuix_Set_Type> Kamuix_Set_Type_Registrant("Kamuix_Set_Player_Type","PlayerType:int");
ScriptRegistrant<Kamuix_Revive_Building_Zone> Kamuix_Revive_Building_Zone_Registrant("Kamuix_Revive_Building","ID:int");
ScriptRegistrant<Kamuix_Set_Building_Health_Zone> Kamuix_Set_Building_Health_Zone_Registrant("Kamuix_Zone_Set_Health","Type:int,ID:int,Health:float");
ScriptRegistrant<Kamuix_Rebuild_Structure_Zone> Kamuix_Rebuild_Structure_Zone_Registrant("Kamuix_Rebuild_Structure_Zone","Location:vector3,Cost:int,Type:int,ID:int,Health:float,Preset:string,page:string,Popmsg:string,Announcement:string,Teamsound:string");
ScriptRegistrant<Kamuix_Death_Destroy_Building> Kamuix_Death_Destroy_Building_Registrant("Kamuix_Death_Destroy_Building","ID:int");
ScriptRegistrant<Kamuix_Control_Spawner> Kamuix_Control_Spawner_Registrant("Kamuix_Control_Spawner","ID:int,Enable:int");
ScriptRegistrant<Kamuix_Zone_Destroy_Building> Kamuix_Zone_Destroy_Building_Registrant("Kamuix_Zone_Destroy_Building","ID:int");
ScriptRegistrant<Kamuix_Set_Model> Kamuix_Set_Model_Registrant("Kamuix_Set_Model","Model:string");
