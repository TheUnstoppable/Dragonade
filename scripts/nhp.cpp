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
#include "nhp.h"
#include "engine.h"
void NHP_Smart_Targetting::Created(GameObject *obj)
{
	Commands->Enable_Enemy_Seen(obj,true);
}

void NHP_Smart_Targetting::Enemy_Seen(GameObject *obj,GameObject *enemy)
{

	const char *c;
	const char *b;
	const char *d;
	c = Get_Parameter("Presets_1-4_Weapon");
	b = Get_Parameter("Presets_5-6_Weapon");
	d = Get_Parameter("Other_Weapon");

	if ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset_One"))) || ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset_Two")))) || ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset_Three")))) || ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset_Four")))))
	{
		Commands->Select_Weapon(obj,c);
	}
	else if ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset_Five"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset_Six"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset_Seven"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset_Eight"))))
	{
		Commands->Select_Weapon(obj,b);
	}
	else
	{
		Commands->Select_Weapon(obj,d);
	}

}

void NHP_Guard_Bot::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj,1);
}

void NHP_Guard_Bot::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	ActionParamsStruct actionThingy;
	float d;
	float s;
	actionThingy.Set_Basic(this,70,1);
	d = Get_Float_Parameter("Distance");
	s = Get_Float_Parameter("Speed");
	if (Commands->Get_Player_Type(obj) == Commands->Get_Player_Type(damager))
	{
		actionThingy.Set_Movement(damager,s,d);
		actionThingy.MoveFollow = true;
		Commands->Action_Goto(obj,actionThingy);
	}
}

void NHP_Construction_AI::Created(GameObject *obj)
{
	float timer;
	timer = Get_Float_Parameter("Time_Delay");
	Commands->Start_Timer(obj,this,timer,1);
	unitlim1 = 0;
	unitlim2 = 0;
	unitlim3 = 0;
}

void NHP_Construction_AI::Timer_Expired(GameObject *obj,int number)
{
	Vector3 spawnPos;
	ActionParamsStruct params;
	float timer;
	int limit;
	int id;
	const char *unit;
	GameObject *object;
	timer = Get_Float_Parameter("Time_Delay");
	spawnPos.X = Commands->Get_Position(obj).X + 5;
	spawnPos.Y = Commands->Get_Position(obj).Y;
	spawnPos.Z = Commands->Get_Position(obj).Z + 2;
	switch (number)
	{
	case 1:
	case 5:
			limit = Get_Int_Parameter("Money_Limit");
			if (unitlim1 < limit)
			{
				unitlim1++;
				unit = Get_Parameter("Money_Gen");
				object = Commands->Create_Object(unit,spawnPos);
				id = Get_Int_Parameter("Money_Waypath");
				params.MoveArrivedDistance = 100;
				params.Set_Basic(this,100,1);
				params.WaypathID = id;
				params.WaypathSplined = true;
				params.AttackActive = false;
				params.AttackCheckBlocked = false;
				Commands->Action_Goto(object,params);
				Commands->Start_Timer(obj,this,timer,2);
			}
		break;
	case 2:
			limit = Get_Int_Parameter("Guard_Limit");
			if (unitlim2 < limit)
			{
				unitlim2++;
				unit = Get_Parameter("Guard_Inf");
				object = Commands->Create_Object(unit,spawnPos);
				id = Get_Int_Parameter("Guard_Waypath");
				params.Set_Basic(this,100,1);
				params.WaypathID = id;
				params.WaypathSplined = true;
				params.AttackActive = false;
				params.AttackCheckBlocked = false;
				Commands->Action_Goto(object,params);
				Commands->Start_Timer(obj,this,timer,3);
			}
		break;
	case 3:
	case 4:
			limit = Get_Int_Parameter("Attack_Limit");
			if (unitlim3 < limit)
			{
				unitlim3++;
				unit = Get_Parameter("Attack_Unit");
				object = Commands->Create_Object(unit,spawnPos);
				id = Get_Int_Parameter("Attack_Waypath");
				params.Set_Basic(this,100,1);
				params.WaypathID = id;
				params.WaypathSplined = true;
				params.AttackActive = false;
				params.AttackCheckBlocked = false;
				Commands->Action_Goto(object,params);
				Commands->Start_Timer(obj,this,timer,4);
			}
		break;
	}
}

void NHP_Construction_AI::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&unitlim1,4,1);
	Auto_Save_Variable(&unitlim2,4,2);
	Auto_Save_Variable(&unitlim3,4,3);
}

//Sensor array zone
void NHP_Sensor_Array_Zone::Created(GameObject *obj)
{
	count = 0;
	SensorIsAlive = true;
	soundId = Commands->Create_Sound(Get_Parameter("Sound_Preset"),Get_Vector3_Parameter("Sound_Position"),obj);
	Commands->Stop_Sound(soundId,false);
}

void NHP_Sensor_Array_Zone::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 901192102)
	{
		if (param == 1)
			SensorIsAlive = true;
		else
			SensorIsAlive = false;
	}
	if (type == 20103020)
	{
		if (Get_Int_Parameter("Check_Team") == Commands->Get_Player_Type(sender))
			return;
		count--;
		if (count < 0)
			count = 0;
		if (!count)
			Commands->Stop_Sound(soundId,false);
	}
}

void NHP_Sensor_Array_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (!SensorIsAlive)
		return;
	if (Get_Int_Parameter("Check_Team") == Commands->Get_Player_Type(enterer))
		return;
	FILE* handle = fopen(Get_Parameter("Sensor_List_File"), "r");
	int detectCount = 0;
	char detectName[100];
	detectName[99] = 0;
	if (!handle)
	{
		Send_Message(255,255,255,"ERROR! Sensor can not find file for sensor list");
		return;
	}
#pragma warning(suppress: 6031) //warning C6031: return value ignored
	fscanf(handle,"%d", &detectCount);
	for (int i=0;i<=detectCount;i++)
	{
#pragma warning(suppress: 6031) //warning C6031: return value ignored
		fscanf(handle,"%99[^\n]", detectName);
		if (!_stricmp(Commands->Get_Preset_Name(enterer),detectName))
		{
			count++;
			if (count == 1)
			{
				Commands->Start_Sound(soundId);
			}
			//Tell the object that it has entered
			Commands->Send_Custom_Event(obj,enterer,20103020,0,0);
			fclose(handle);
			return;
		}
	}
	fclose(handle);
}

void NHP_Sensor_Array_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (!SensorIsAlive)
		return;
	if (Get_Int_Parameter("Check_Team") == Commands->Get_Player_Type(exiter))
		return;
	FILE* handle = fopen(Get_Parameter("Sensor_List_File"), "r");
	int detectCount = 0;
	char detectName[100];
	detectName[99] = 0;
	if (!handle)
	{
		Send_Message(255,255,255,"ERROR! Sensor can not find file for sensor list");
		return;
	}
#pragma warning(suppress: 6031) //warning C6031: return value ignored
	fscanf(handle,"%d", &detectCount);
	for (int i=0;i<=detectCount;i++)
	{
#pragma warning(suppress: 6031) //warning C6031: return value ignored
		fscanf(handle,"%99[^\n]", detectName);
		if (!_stricmp(Commands->Get_Preset_Name(exiter),detectName))
		{
			count--;
			if (count < 0)
				count = 0;
			if (!count)
			{
				Commands->Stop_Sound(soundId,false);
			}
			//Tell the object that it has left
			Commands->Send_Custom_Event(obj,exiter,20103020,1,0);
			fclose(handle);
			return;
		}
	}
	fclose(handle);
}

void NHP_Sensor_Array_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&SensorIsAlive,4,1);
	Auto_Save_Variable(&count,4,2);
	Auto_Save_Variable(&soundId,4,3);
}

//Sensorable object
void NHP_Sensorable_Object::Created(GameObject *obj)
{
	sensorZone = 0;
	insideSensorZone = false;
}

void NHP_Sensorable_Object::Killed(GameObject *obj,GameObject *killer)
{
	if (insideSensorZone)
		Commands->Send_Custom_Event(obj,Commands->Find_Object(sensorZone),20103020,1,0);
}

void NHP_Sensorable_Object::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 20103020)
	{
		if (!param)
		{
			insideSensorZone = true;
			sensorZone = Commands->Get_ID(sender);
		}
		else if (param == 1)
		{
			insideSensorZone = false;
			sensorZone = Commands->Get_ID(sender);
		}
	}
}

void NHP_Sensorable_Object::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&insideSensorZone,4,1);
	Auto_Save_Variable(&sensorZone,4,2);
}

ScriptRegistrant<NHP_Construction_AI> NHP_Construction_AI_Registrant("NHP_Construction_AI","Money_Gen:string,Guard_Inf:string,Attack_Unit:string,Money_Limit:int,Guard_Limit:int,Attack_Limit:int,Time_Delay:float,Money_Waypath:int,Guard_Waypath:int,Attack_Waypath:int");
ScriptRegistrant<NHP_Smart_Targetting> NHP_Smart_Targetting_Registrant("NHP_Smart_Targetting","Presets_1-4_Weapon:string,Presets_5-6_Weapon:string,Other_Weapon:string,Preset_One:string,Preset_Two:string,Preset_Three:string,Preset_Four:string,Preset_Five:string,Preset_Six:string,Preset_Seven:string,Preset_Eight:string");
ScriptRegistrant<NHP_Guard_Bot> NHP_Guard_Bot_Registrant("NHP_Guard_Bot","Distance:float,Speed:float");
ScriptRegistrant<NHP_Sensor_Array_Zone> NHP_Sensor_Array_Zone_Registrant("NHP_Sensor_Array_Zone","Sensor_List_File:string,Sound_Preset:string,Sound_Position:vector3,Check_Team:int");
ScriptRegistrant<NHP_Sensorable_Object> NHP_Sensorable_Object_Registrant("NHP_Sensorable_Object","");
