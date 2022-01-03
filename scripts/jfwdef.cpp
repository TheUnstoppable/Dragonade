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
#include "jfwdef.h"
#include "SmartGameObj.h"
#include "VehicleGameObjDef.h"

void JFW_User_Controllable_Base_Defence::Created(GameObject *obj)
{
	Vector3 pos,pos1,pos2,pos3;
	GameObject *object;
	objtype = Commands->Get_Player_Type(obj);
	player = false;
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	pos = Commands->Get_Position(obj);
	pos1.X = pos.X - 5;
	pos1.Y = pos.Y + 8.66025f;
	pos1.Z = pos.Z + 2;
	pos2.X = pos.X + 10;
	pos2.Y = pos.Y;
	pos2.Z = pos.Z + 2;
	pos3.X = pos.X - 5;
	pos3.Y = pos.Y - 8.66025f;
	pos3.Z = pos.Z + 2;
	object = Commands->Create_Object("Invisible_Object",pos1);
	if (object)
	{
		id1 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos2);
	if (object)
	{
		id2 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos3);
	if (object)
	{
		id3 = Commands->Get_ID(object);
	}
	Commands->Start_Timer(obj,this,10,1);
}

void JFW_User_Controllable_Base_Defence::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	ActionParamsStruct params;
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		player = false;
		Commands->Set_Player_Type(obj,objtype);
		Commands->Action_Reset(obj,100);
	}
	else
	{
		if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
		{
			player = true;
			params.Set_Basic(this,100,3);
			Commands->Action_Follow_Input(obj,params);
		}
	}
}

void JFW_User_Controllable_Base_Defence::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	ActionParamsStruct params;
	Vector3 pos,pos2;
	float distance;
	float minattack,maxattack;
	float attacktimer;
	if (!player)
	{
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		distance = Commands->Get_Distance(pos2,pos);
		minattack = Get_Float_Parameter("MinAttackDistance");
		if (distance >= minattack)
		{
			maxattack = Get_Float_Parameter("MaxAttackDistance");
			params.Set_Basic(this,100,2);
			params.Set_Attack(enemy,maxattack,0.0,true);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			attacktimer = Get_Float_Parameter("AttackTimer");
			Commands->Start_Timer(obj,this,attacktimer,2);
		}
	}
}

void JFW_User_Controllable_Base_Defence::Timer_Expired(GameObject *obj,int number)
{
	ActionParamsStruct var;
	GameObject *object;
	float f;
	switch (number)
	{
	case 1:
		f = Commands->Get_Random(0.0f,2.9999f);
		if (!player)
		{
			switch (__min(int(f),2))
			{
			case 0:
				object = Commands->Find_Object(id1);
				if (object)
				{
					var.Set_Basic(this,70,1);
					var.Set_Attack(object,0.0,0.0,true);
					Commands->Action_Attack(obj,var);
				}
				break;
			case 1:
				object = Commands->Find_Object(id2);
				if (object)
				{
					var.Set_Basic(this,70,1);
					var.Set_Attack(object,0.0,0.0,true);
					Commands->Action_Attack(obj,var);
				}
				break;
			default:
				object = Commands->Find_Object(id3);
				if (object)
				{
					var.Set_Basic(this,70,1);
					var.Set_Attack(object,0.0,0.0,true);
					Commands->Action_Attack(obj,var);
				}
			}
		}
		Commands->Start_Timer(obj,this,10,1);
		break;
	case 2:
		Commands->Action_Reset(obj,100);
		break;
	}
}

void JFW_User_Controllable_Base_Defence::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,4,1);
	Auto_Save_Variable(&id2,4,2);
	Auto_Save_Variable(&id3,4,3);
	Auto_Save_Variable(&objtype,4,4);
	Auto_Save_Variable(&player,1,5);
}

void JFW_Base_Defence::Created(GameObject *obj)
{
	Vector3 pos,pos1,pos2,pos3;
	GameObject *object;
	objtype = Commands->Get_Player_Type(obj);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	pos = Commands->Get_Position(obj);
	pos1.X = pos.X - 5;
	pos1.Y = pos.Y + 8.66025f;
	pos1.Z = pos.Z + 2;
	pos2.X = pos.X + 10;
	pos2.Y = pos.Y;
	pos2.Z = pos.Z + 2;
	pos3.X = pos.X - 5;
	pos3.Y = pos.Y - 8.66025f;
	pos3.Z = pos.Z + 2;
	object = Commands->Create_Object("Invisible_Object",pos1);
	if (object)
	{
		id1 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos2);
	if (object)
	{
		id2 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos3);
	if (object)
	{
		id3 = Commands->Get_ID(object);
	}
	Commands->Start_Timer(obj,this,10,1);
}

void JFW_Base_Defence::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	ActionParamsStruct params;
	Vector3 pos,pos2;
	float distance;
	float minattack,maxattack;
	float attacktimer;
	pos = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(enemy);
	distance = Commands->Get_Distance(pos2,pos);
	minattack = Get_Float_Parameter("MinAttackDistance");
	if (distance >= minattack)
	{
		maxattack = Get_Float_Parameter("MaxAttackDistance");
		params.Set_Basic(this,100,2);
		params.Set_Attack(enemy,maxattack,0.0,true);
		params.AttackCheckBlocked = false;
		Commands->Action_Attack(obj,params);
		attacktimer = Get_Float_Parameter("AttackTimer");
		Stop_Timer2(obj,this,2);
		Commands->Start_Timer(obj,this,attacktimer,2);
	}
}

void JFW_Base_Defence::Timer_Expired(GameObject *obj,int number)
{
	ActionParamsStruct var;
	GameObject *object;
	float f;
	switch (number)
	{
	case 1:
		f = Commands->Get_Random(0.0f,2.9999f);
		switch (__min(int(f),2))
		{
		case 0:
			object = Commands->Find_Object(id1);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		case 1:
			object = Commands->Find_Object(id2);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		default:
			object = Commands->Find_Object(id3);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
		}
	Commands->Start_Timer(obj,this,10,1);
	break;
	case 2:
		Commands->Action_Reset(obj,100);
		break;
	}
}

void JFW_Base_Defence::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,4,1);
	Auto_Save_Variable(&id2,4,2);
	Auto_Save_Variable(&id3,4,3);
	Auto_Save_Variable(&objtype,4,4);
}

void JFW_Base_Defence_No_Aircraft::Created(GameObject *obj)
{
	Vector3 pos,pos1,pos2,pos3;
	GameObject *object;
	objtype = Commands->Get_Player_Type(obj);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	pos = Commands->Get_Position(obj);
	pos1.X = pos.X - 5;
	pos1.Y = pos.Y + 8.66025f;
	pos1.Z = pos.Z + 2;
	pos2.X = pos.X + 10;
	pos2.Y = pos.Y;
	pos2.Z = pos.Z + 2;
	pos3.X = pos.X - 5;
	pos3.Y = pos.Y - 8.66025f;
	pos3.Z = pos.Z + 2;
	object = Commands->Create_Object("Invisible_Object",pos1);
	if (object)
	{
		id1 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos2);
	if (object)
	{
		id2 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos3);
	if (object)
	{
		id3 = Commands->Get_ID(object);
	}
	Commands->Start_Timer(obj,this,10,1);
}

void JFW_Base_Defence_No_Aircraft::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		ActionParamsStruct params;
		Vector3 pos,pos2;
		float distance;
		float minattack,maxattack;
		float attacktimer;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		distance = Commands->Get_Distance(pos2,pos);
		minattack = Get_Float_Parameter("MinAttackDistance");
		if (distance >= minattack)
		{
			maxattack = Get_Float_Parameter("MaxAttackDistance");
			params.Set_Basic(this,100,2);
			params.Set_Attack(enemy,maxattack,0.0,true);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			attacktimer = Get_Float_Parameter("AttackTimer");
			Stop_Timer2(obj,this,2);
			Commands->Start_Timer(obj,this,attacktimer,2);
		}
	}
}

void JFW_Base_Defence_No_Aircraft::Timer_Expired(GameObject *obj,int number)
{
	ActionParamsStruct var;
	GameObject *object;
	float f;
	switch (number)
	{
	case 1:
		f = Commands->Get_Random(0.0f,2.9999f);
		switch (__min(int(f),2))
		{
		case 0:
			object = Commands->Find_Object(id1);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		case 1:
			object = Commands->Find_Object(id2);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		default:
			object = Commands->Find_Object(id3);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
		}
	Commands->Start_Timer(obj,this,10,1);
	break;
	case 2:
		Commands->Action_Reset(obj,100);
		break;
	}
}

void JFW_Base_Defence_No_Aircraft::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,4,1);
	Auto_Save_Variable(&id2,4,2);
	Auto_Save_Variable(&id3,4,3);
	Auto_Save_Variable(&objtype,4,4);
}

void JFW_Base_Defence_Aircraft_Only::Created(GameObject *obj)
{
	Vector3 pos,pos1,pos2,pos3;
	GameObject *object;
	objtype = Commands->Get_Player_Type(obj);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	pos = Commands->Get_Position(obj);
	pos1.X = pos.X - 5;
	pos1.Y = pos.Y + 8.66025f;
	pos1.Z = pos.Z + 2;
	pos2.X = pos.X + 10;
	pos2.Y = pos.Y;
	pos2.Z = pos.Z + 2;
	pos3.X = pos.X - 5;
	pos3.Y = pos.Y - 8.66025f;
	pos3.Z = pos.Z + 2;
	object = Commands->Create_Object("Invisible_Object",pos1);
	if (object)
	{
		id1 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos2);
	if (object)
	{
		id2 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos3);
	if (object)
	{
		id3 = Commands->Get_ID(object);
	}
	Commands->Start_Timer(obj,this,10,1);
}

void JFW_Base_Defence_Aircraft_Only::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		ActionParamsStruct params;
		Vector3 pos,pos2;
		float distance;
		float minattack,maxattack;
		float attacktimer;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		distance = Commands->Get_Distance(pos2,pos);
		minattack = Get_Float_Parameter("MinAttackDistance");
		if (distance >= minattack)
		{
			maxattack = Get_Float_Parameter("MaxAttackDistance");
			params.Set_Basic(this,100,2);
			params.Set_Attack(enemy,maxattack,0.0,true);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			attacktimer = Get_Float_Parameter("AttackTimer");
			Stop_Timer2(obj,this,2);
			Commands->Start_Timer(obj,this,attacktimer,2);
		}
	}
}

void JFW_Base_Defence_Aircraft_Only::Timer_Expired(GameObject *obj,int number)
{
	ActionParamsStruct var;
	GameObject *object;
	float f;
	switch (number)
	{
	case 1:
		f = Commands->Get_Random(0.0f,2.9999f);
		switch (__min(int(f),2))
		{
		case 0:
			object = Commands->Find_Object(id1);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		case 1:
			object = Commands->Find_Object(id2);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		default:
			object = Commands->Find_Object(id3);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
		}
	Commands->Start_Timer(obj,this,10,1);
	break;
	case 2:
		Commands->Action_Reset(obj,100);
		break;
	}
}

void JFW_Base_Defence_Aircraft_Only::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,4,1);
	Auto_Save_Variable(&id2,4,2);
	Auto_Save_Variable(&id3,4,3);
	Auto_Save_Variable(&objtype,4,4);
}

void JFW_Base_Defence_Secondary::Created(GameObject *obj)
{
	Vector3 pos,pos1,pos2,pos3;
	GameObject *object;
	primary = true;
	objtype = Commands->Get_Player_Type(obj);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	pos = Commands->Get_Position(obj);
	pos1.X = pos.X - 5;
	pos1.Y = pos.Y + 8.66025f;
	pos1.Z = pos.Z + 2;
	pos2.X = pos.X + 10;
	pos2.Y = pos.Y;
	pos2.Z = pos.Z + 2;
	pos3.X = pos.X - 5;
	pos3.Y = pos.Y - 8.66025f;
	pos3.Z = pos.Z + 2;
	object = Commands->Create_Object("Invisible_Object",pos1);
	if (object)
	{
		id1 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos2);
	if (object)
	{
		id2 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos3);
	if (object)
	{
		id3 = Commands->Get_ID(object);
	}
	Commands->Start_Timer(obj,this,10,1);
}

void JFW_Base_Defence_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	ActionParamsStruct params;
	Vector3 pos,pos2;
	float distance;
	float minattack,maxattack;
	float attacktimer;
	pos = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(enemy);
	distance = Commands->Get_Distance(pos2,pos);
	minattack = Get_Float_Parameter("MinAttackDistance");
	if (distance >= minattack)
	{
		maxattack = Get_Float_Parameter("MaxAttackDistance");
		params.Set_Basic(this,100,2);
		params.Set_Attack(enemy,maxattack,0.0,primary);
		params.AttackCheckBlocked = false;
		Commands->Action_Attack(obj,params);
		attacktimer = Get_Float_Parameter("AttackTimer");
		Stop_Timer2(obj,this,2);
		Commands->Start_Timer(obj,this,attacktimer,2);
		if (primary)
		{
			primary = false;
		}
		else
		{
			primary = true;
		}
	}
}

void JFW_Base_Defence_Secondary::Timer_Expired(GameObject *obj,int number)
{
	ActionParamsStruct var;
	GameObject *object;
	float f;
	switch (number)
	{
	case 1:
		f = Commands->Get_Random(0.0f,2.9999f);
		switch (__min(int(f),2))
		{
		case 0:
			object = Commands->Find_Object(id1);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		case 1:
			object = Commands->Find_Object(id2);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		default:
			object = Commands->Find_Object(id3);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
		}
	Commands->Start_Timer(obj,this,10,1);
	break;
	case 2:
		Commands->Action_Reset(obj,100);
		break;
	}
}

void JFW_Base_Defence_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,4,1);
	Auto_Save_Variable(&id2,4,2);
	Auto_Save_Variable(&id3,4,3);
	Auto_Save_Variable(&objtype,4,4);
	Auto_Save_Variable(&primary,5,1);
}

void JFW_Base_Defence_No_Aircraft_Secondary::Created(GameObject *obj)
{
	Vector3 pos,pos1,pos2,pos3;
	GameObject *object;
	primary = true;
	objtype = Commands->Get_Player_Type(obj);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	pos = Commands->Get_Position(obj);
	pos1.X = pos.X - 5;
	pos1.Y = pos.Y + 8.66025f;
	pos1.Z = pos.Z + 2;
	pos2.X = pos.X + 10;
	pos2.Y = pos.Y;
	pos2.Z = pos.Z + 2;
	pos3.X = pos.X - 5;
	pos3.Y = pos.Y - 8.66025f;
	pos3.Z = pos.Z + 2;
	object = Commands->Create_Object("Invisible_Object",pos1);
	if (object)
	{
		id1 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos2);
	if (object)
	{
		id2 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos3);
	if (object)
	{
		id3 = Commands->Get_ID(object);
	}
	Commands->Start_Timer(obj,this,10,1);
}

void JFW_Base_Defence_No_Aircraft_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		ActionParamsStruct params;
		Vector3 pos,pos2;
		float distance;
		float minattack,maxattack;
		float attacktimer;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		distance = Commands->Get_Distance(pos2,pos);
		minattack = Get_Float_Parameter("MinAttackDistance");
		if (distance >= minattack)
		{
			maxattack = Get_Float_Parameter("MaxAttackDistance");
			params.Set_Basic(this,100,2);
			params.Set_Attack(enemy,maxattack,0.0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			attacktimer = Get_Float_Parameter("AttackTimer");
			Stop_Timer2(obj,this,2);
			Commands->Start_Timer(obj,this,attacktimer,2);
			if (primary)
			{
				primary = false;
			}
			else
			{
				primary = true;
			}
		}
	}
}

void JFW_Base_Defence_No_Aircraft_Secondary::Timer_Expired(GameObject *obj,int number)
{
	ActionParamsStruct var;
	GameObject *object;
	float f;
	switch (number)
	{
	case 1:
		f = Commands->Get_Random(0.0f,2.9999f);
		switch (__min(int(f),2))
		{
		case 0:
			object = Commands->Find_Object(id1);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		case 1:
			object = Commands->Find_Object(id2);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		default:
			object = Commands->Find_Object(id3);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
		}
	Commands->Start_Timer(obj,this,10,1);
	break;
	case 2:
		Commands->Action_Reset(obj,100);
		break;
	}
}

void JFW_Base_Defence_No_Aircraft_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,4,1);
	Auto_Save_Variable(&id2,4,2);
	Auto_Save_Variable(&id3,4,3);
	Auto_Save_Variable(&objtype,4,4);
	Auto_Save_Variable(&primary,5,1);
}

void JFW_Base_Defence_Aircraft_Only_Secondary::Created(GameObject *obj)
{
	Vector3 pos,pos1,pos2,pos3;
	GameObject *object;
	primary = true;
	objtype = Commands->Get_Player_Type(obj);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	pos = Commands->Get_Position(obj);
	pos1.X = pos.X - 5;
	pos1.Y = pos.Y + 8.66025f;
	pos1.Z = pos.Z + 2;
	pos2.X = pos.X + 10;
	pos2.Y = pos.Y;
	pos2.Z = pos.Z + 2;
	pos3.X = pos.X - 5;
	pos3.Y = pos.Y - 8.66025f;
	pos3.Z = pos.Z + 2;
	object = Commands->Create_Object("Invisible_Object",pos1);
	if (object)
	{
		id1 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos2);
	if (object)
	{
		id2 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos3);
	if (object)
	{
		id3 = Commands->Get_ID(object);
	}
	Commands->Start_Timer(obj,this,10,1);
}

void JFW_Base_Defence_Aircraft_Only_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		ActionParamsStruct params;
		Vector3 pos,pos2;
		float distance;
		float minattack,maxattack;
		float attacktimer;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		distance = Commands->Get_Distance(pos2,pos);
		minattack = Get_Float_Parameter("MinAttackDistance");
		if (distance >= minattack)
		{
			maxattack = Get_Float_Parameter("MaxAttackDistance");
			params.Set_Basic(this,100,2);
			params.Set_Attack(enemy,maxattack,0.0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			attacktimer = Get_Float_Parameter("AttackTimer");
			Stop_Timer2(obj,this,2);
			Commands->Start_Timer(obj,this,attacktimer,2);
			if (primary)
			{
				primary = false;
			}
			else
			{
				primary = true;
			}
		}
	}
}

void JFW_Base_Defence_Aircraft_Only_Secondary::Timer_Expired(GameObject *obj,int number)
{
	ActionParamsStruct var;
	GameObject *object;
	float f;
	switch (number)
	{
	case 1:
		f = Commands->Get_Random(0.0f,2.9999f);
		switch (__min(int(f),2))
		{
		case 0:
			object = Commands->Find_Object(id1);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		case 1:
			object = Commands->Find_Object(id2);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		default:
			object = Commands->Find_Object(id3);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
		}
	Commands->Start_Timer(obj,this,10,1);
	break;
	case 2:
		Commands->Action_Reset(obj,100);
		break;
	}
}

void JFW_Base_Defence_Aircraft_Only_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,4,1);
	Auto_Save_Variable(&id2,4,2);
	Auto_Save_Variable(&id3,4,3);
	Auto_Save_Variable(&objtype,4,4);
	Auto_Save_Variable(&primary,5,1);
}

void JFW_Base_Defence_Animated::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!popup)
	{
		popup = true;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
		Stop_Timer2(obj,this,1);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
	}
	else if (attack)
	{
		ActionParamsStruct params;
		params.Set_Basic(this,100,1);
		params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,true);
		params.AttackCheckBlocked = false;
		Commands->Action_Attack(obj,params);
	}
}

void JFW_Base_Defence_Animated::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&attack,2,1);
}

void JFW_Base_Defence_Animated_No_Aircraft::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_No_Aircraft::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,true);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
		}
	}
}

void JFW_Base_Defence_Animated_No_Aircraft::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_No_Aircraft::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&attack,2,1);
}

void JFW_Base_Defence_Animated_Aircraft_Only::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Aircraft_Only::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,true);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
		}
	}
}

void JFW_Base_Defence_Animated_Aircraft_Only::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Aircraft_Only::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&attack,2,1);
}

void JFW_Base_Defence_Animated_Secondary::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!popup)
	{
		popup = true;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
		Stop_Timer2(obj,this,1);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
	}
	else if (attack)
	{
		ActionParamsStruct params;
		params.Set_Basic(this,100,1);
		params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
		params.AttackCheckBlocked = false;
		Commands->Action_Attack(obj,params);
		if (primary)
		{
			primary = false;
		}
		else
		{
			primary = true;
		}
	}
}

void JFW_Base_Defence_Animated_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_Animated_No_Aircraft_Secondary::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_No_Aircraft_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			if (primary)
			{
				primary = false;
			}
			else
			{
				primary = true;
			}
		}
	}
}

void JFW_Base_Defence_Animated_No_Aircraft_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_No_Aircraft_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_Animated_Aircraft_Only_Secondary::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Aircraft_Only_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			if (primary)
			{	
				primary = false;
			}
			else
			{
				primary = true;
			}
		}
	}
}

void JFW_Base_Defence_Animated_Aircraft_Only_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Aircraft_Only_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_No_VTOL::Created(GameObject *obj)
{
	Vector3 pos,pos1,pos2,pos3;
	GameObject *object;
	objtype = Commands->Get_Player_Type(obj);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	pos = Commands->Get_Position(obj);
	pos1.X = pos.X - 5;
	pos1.Y = pos.Y + 8.66025f;
	pos1.Z = pos.Z + 2;
	pos2.X = pos.X + 10;
	pos2.Y = pos.Y;
	pos2.Z = pos.Z + 2;
	pos3.X = pos.X - 5;
	pos3.Y = pos.Y - 8.66025f;
	pos3.Z = pos.Z + 2;
	object = Commands->Create_Object("Invisible_Object",pos1);
	if (object)
	{
		id1 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos2);
	if (object)
	{
		id2 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos3);
	if (object)
	{
		id3 = Commands->Get_ID(object);
	}
	Commands->Start_Timer(obj,this,10,1);
}

void JFW_Base_Defence_No_VTOL::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!Is_VTOL(enemy))
	{
		ActionParamsStruct params;
		Vector3 pos,pos2;
		float distance;
		float minattack,maxattack;
		float attacktimer;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		distance = Commands->Get_Distance(pos2,pos);
		minattack = Get_Float_Parameter("MinAttackDistance");
		if (distance >= minattack)
		{
			maxattack = Get_Float_Parameter("MaxAttackDistance");
			params.Set_Basic(this,100,2);
			params.Set_Attack(enemy,maxattack,0,true);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			attacktimer = Get_Float_Parameter("AttackTimer");
			Stop_Timer2(obj,this,2);
			Commands->Start_Timer(obj,this,attacktimer,2);
		}
	}
}

void JFW_Base_Defence_No_VTOL::Timer_Expired(GameObject *obj,int number)
{
	ActionParamsStruct var;
	GameObject *object;
	float f;
	switch (number)
	{
	case 1:
		f = Commands->Get_Random(0.0f,2.9999f);
		switch (__min(int(f),2))
		{
		case 0:
			object = Commands->Find_Object(id1);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		case 1:
			object = Commands->Find_Object(id2);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		default:
			object = Commands->Find_Object(id3);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
		}
	Commands->Start_Timer(obj,this,10,1);
	break;
	case 2:
		Commands->Action_Reset(obj,100);
		break;
	}
}

void JFW_Base_Defence_No_VTOL::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,4,1);
	Auto_Save_Variable(&id2,4,2);
	Auto_Save_Variable(&id3,4,3);
	Auto_Save_Variable(&objtype,4,4);
}

void JFW_Base_Defence_No_VTOL_No_Stealth::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!Is_VTOL(enemy))
	{
		SmartGameObj* obj1 = enemy->As_SmartGameObj();
		if(!obj1 
		|| !obj1->Is_Stealthed() 
		|| Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(enemy)) <= Get_Float_Parameter("MaxStealthAttackDistance"))
		{
			ActionParamsStruct params;
			Vector3 pos,pos2;
			float distance;
			float minattack,maxattack;
			float attacktimer;
			pos = Commands->Get_Position(obj);
			pos2 = Commands->Get_Position(enemy);
			distance = Commands->Get_Distance(pos2,pos);
			minattack = Get_Float_Parameter("MinAttackDistance");
			if (distance >= minattack)
			{
				maxattack = Get_Float_Parameter("MaxAttackDistance");
				params.Set_Basic(this,100,2);
				params.Set_Attack(enemy,maxattack,0,true);
				params.AttackCheckBlocked = false;
				Commands->Action_Attack(obj,params);
				attacktimer = Get_Float_Parameter("AttackTimer");
				Stop_Timer2(obj,this,2);
				Commands->Start_Timer(obj,this,attacktimer,2);
			}
		}
	}
}

void JFW_Base_Defence_VTOL_Only::Created(GameObject *obj)
{
	Vector3 pos,pos1,pos2,pos3;
	GameObject *object;
	objtype = Commands->Get_Player_Type(obj);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	pos = Commands->Get_Position(obj);
	pos1.X = pos.X - 5;
	pos1.Y = pos.Y + 8.66025f;
	pos1.Z = pos.Z + 2;
	pos2.X = pos.X + 10;
	pos2.Y = pos.Y;
	pos2.Z = pos.Z + 2;
	pos3.X = pos.X - 5;
	pos3.Y = pos.Y - 8.66025f;
	pos3.Z = pos.Z + 2;
	object = Commands->Create_Object("Invisible_Object",pos1);
	if (object)
	{
		id1 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos2);
	if (object)
	{
		id2 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos3);
	if (object)
	{
		id3 = Commands->Get_ID(object);
	}
	Commands->Start_Timer(obj,this,10,1);
}

void JFW_Base_Defence_VTOL_Only::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (Is_VTOL(enemy))
	{
		ActionParamsStruct params;
		Vector3 pos,pos2;
		float distance;
		float minattack,maxattack;
		float attacktimer;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		distance = Commands->Get_Distance(pos2,pos);
		minattack = Get_Float_Parameter("MinAttackDistance");
		if (distance >= minattack)
		{
			maxattack = Get_Float_Parameter("MaxAttackDistance");
			params.Set_Basic(this,100,2);
			params.Set_Attack(enemy,maxattack,0,true);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			attacktimer = Get_Float_Parameter("AttackTimer");
			Stop_Timer2(obj,this,2);
			Commands->Start_Timer(obj,this,attacktimer,2);
		}
	}
}

void JFW_Base_Defence_VTOL_Only::Timer_Expired(GameObject *obj,int number)
{
	ActionParamsStruct var;
	GameObject *object;
	float f;
	switch (number)
	{
	case 1:
		f = Commands->Get_Random(0.0f,2.9999f);
		switch (__min(int(f),2))
		{
		case 0:
			object = Commands->Find_Object(id1);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		case 1:
			object = Commands->Find_Object(id2);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		default:
			object = Commands->Find_Object(id3);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
		}
	Commands->Start_Timer(obj,this,10,1);
	break;
	case 2:
		Commands->Action_Reset(obj,100);
		break;
	}
}

void JFW_Base_Defence_VTOL_Only::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,4,1);
	Auto_Save_Variable(&id2,4,2);
	Auto_Save_Variable(&id3,4,3);
	Auto_Save_Variable(&objtype,4,4);
}

void JFW_Base_Defence_No_VTOL_Secondary::Created(GameObject *obj)
{
	Vector3 pos,pos1,pos2,pos3;
	GameObject *object;
	primary = true;
	objtype = Commands->Get_Player_Type(obj);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	pos = Commands->Get_Position(obj);
	pos1.X = pos.X - 5;
	pos1.Y = pos.Y + 8.66025f;
	pos1.Z = pos.Z + 2;
	pos2.X = pos.X + 10;
	pos2.Y = pos.Y;
	pos2.Z = pos.Z + 2;
	pos3.X = pos.X - 5;
	pos3.Y = pos.Y - 8.66025f;
	pos3.Z = pos.Z + 2;
	object = Commands->Create_Object("Invisible_Object",pos1);
	if (object)
	{
		id1 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos2);
	if (object)
	{
		id2 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos3);
	if (object)
	{
		id3 = Commands->Get_ID(object);
	}
	Commands->Start_Timer(obj,this,10,1);
}

void JFW_Base_Defence_No_VTOL_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!Is_VTOL(enemy))
	{
		ActionParamsStruct params;
		Vector3 pos,pos2;
		float distance;
		float minattack,maxattack;
		float attacktimer;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		distance = Commands->Get_Distance(pos2,pos);
		minattack = Get_Float_Parameter("MinAttackDistance");
		if (distance >= minattack)
		{
			maxattack = Get_Float_Parameter("MaxAttackDistance");
			params.Set_Basic(this,100,2);
			params.Set_Attack(enemy,maxattack,0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			attacktimer = Get_Float_Parameter("AttackTimer");
			Stop_Timer2(obj,this,2);
			Commands->Start_Timer(obj,this,attacktimer,2);
			if (primary)
			{
				primary = false;
			}
			else
			{
				primary = true;
			}
		}
	}
}

void JFW_Base_Defence_No_VTOL_Secondary::Timer_Expired(GameObject *obj,int number)
{
	ActionParamsStruct var;
	GameObject *object;
	float f;
	switch (number)
	{
	case 1:
		f = Commands->Get_Random(0.0f,2.9999f);
		switch (__min(int(f),2))
		{
		case 0:
			object = Commands->Find_Object(id1);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		case 1:
			object = Commands->Find_Object(id2);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		default:
			object = Commands->Find_Object(id3);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
		}
	Commands->Start_Timer(obj,this,10,1);
	break;
	case 2:
		Commands->Action_Reset(obj,100);
		break;
	}
}

void JFW_Base_Defence_No_VTOL_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,4,1);
	Auto_Save_Variable(&id2,4,2);
	Auto_Save_Variable(&id3,4,3);
	Auto_Save_Variable(&objtype,4,4);
	Auto_Save_Variable(&primary,5,1);
}

void JFW_Base_Defence_VTOL_Only_Secondary::Created(GameObject *obj)
{
	Vector3 pos,pos1,pos2,pos3;
	GameObject *object;
	primary = true;
	objtype = Commands->Get_Player_Type(obj);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	pos = Commands->Get_Position(obj);
	pos1.X = pos.X - 5;
	pos1.Y = pos.Y + 8.66025f;
	pos1.Z = pos.Z + 2;
	pos2.X = pos.X + 10;
	pos2.Y = pos.Y;
	pos2.Z = pos.Z + 2;
	pos3.X = pos.X - 5;
	pos3.Y = pos.Y - 8.66025f;
	pos3.Z = pos.Z + 2;
	object = Commands->Create_Object("Invisible_Object",pos1);
	if (object)
	{
		id1 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos2);
	if (object)
	{
		id2 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos3);
	if (object)
	{
		id3 = Commands->Get_ID(object);
	}
	Commands->Start_Timer(obj,this,10,1);
}

void JFW_Base_Defence_VTOL_Only_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (Is_VTOL(enemy))
	{
		ActionParamsStruct params;
		Vector3 pos,pos2;
		float distance;
		float minattack,maxattack;
		float attacktimer;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		distance = Commands->Get_Distance(pos2,pos);
		minattack = Get_Float_Parameter("MinAttackDistance");
		if (distance >= minattack)
		{
			maxattack = Get_Float_Parameter("MaxAttackDistance");
			params.Set_Basic(this,100,2);
			params.Set_Attack(enemy,maxattack,0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			attacktimer = Get_Float_Parameter("AttackTimer");
			Stop_Timer2(obj,this,2);
			Commands->Start_Timer(obj,this,attacktimer,2);
			if (primary)
			{
				primary = false;
			}
			else
			{
				primary = true;
			}
		}
	}
}

void JFW_Base_Defence_VTOL_Only_Secondary::Timer_Expired(GameObject *obj,int number)
{
	ActionParamsStruct var;
	GameObject *object;
	float f;
	switch (number)
	{
	case 1:
		f = Commands->Get_Random(0.0f,2.9999f);
		switch (__min(int(f),2))
		{
		case 0:
			object = Commands->Find_Object(id1);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		case 1:
			object = Commands->Find_Object(id2);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		default:
			object = Commands->Find_Object(id3);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
		}
	Commands->Start_Timer(obj,this,10,1);
	break;
	case 2:
		Commands->Action_Reset(obj,100);
		break;
	}
}

void JFW_Base_Defence_VTOL_Only_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,4,1);
	Auto_Save_Variable(&id2,4,2);
	Auto_Save_Variable(&id3,4,3);
	Auto_Save_Variable(&objtype,4,4);
	Auto_Save_Variable(&primary,5,1);
}

void JFW_Base_Defence_Animated_No_VTOL::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_No_VTOL::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!Is_VTOL(enemy))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,true);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
		}
	}
}

void JFW_Base_Defence_Animated_No_VTOL::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_No_VTOL::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&attack,2,1);
}

void JFW_Base_Defence_Animated_VTOL_Only::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_VTOL_Only::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (Is_VTOL(enemy))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,true);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
		}
	}
}

void JFW_Base_Defence_Animated_VTOL_Only::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_VTOL_Only::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&attack,2,1);
}

void JFW_Base_Defence_Animated_No_VTOL_Secondary::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_No_VTOL_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!Is_VTOL(enemy))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			if (primary)
			{
				primary = false;
			}
			else
			{
				primary = true;
			}
		}
	}
}

void JFW_Base_Defence_Animated_No_VTOL_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_No_VTOL_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_Animated_VTOL_Only_Secondary::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_VTOL_Only_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (Is_VTOL(enemy))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			if (primary)
			{	
				primary = false;
			}
			else
			{
				primary = true;
			}
		}
	}
}

void JFW_Base_Defence_Animated_VTOL_Only_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_VTOL_Only_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_Animated_Sound::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Sound::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!popup)
	{
		popup = true;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
		Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(enemy),obj);
		Stop_Timer2(obj,this,1);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
	}
	else if (attack)
	{
		ActionParamsStruct params;
		params.Set_Basic(this,100,1);
		params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,true);
		params.AttackCheckBlocked = false;
		Commands->Action_Attack(obj,params);
	}
}

void JFW_Base_Defence_Animated_Sound::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Sound::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&attack,2,1);
}

void JFW_Base_Defence_Animated_Sound_No_Aircraft::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Sound_No_Aircraft::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(enemy),obj);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,true);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
		}
	}
}

void JFW_Base_Defence_Animated_Sound_No_Aircraft::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Sound_No_Aircraft::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&attack,2,1);
}

void JFW_Base_Defence_Animated_Sound_Aircraft_Only::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Sound_Aircraft_Only::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(enemy),obj);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,true);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
		}
	}
}

void JFW_Base_Defence_Animated_Sound_Aircraft_Only::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Sound_Aircraft_Only::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&attack,2,1);
}

void JFW_Base_Defence_Animated_Sound_Secondary::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Sound_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!popup)
	{
		popup = true;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
		Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(enemy),obj);
		Stop_Timer2(obj,this,1);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
	}
	else if (attack)
	{
		ActionParamsStruct params;
		params.Set_Basic(this,100,1);
		params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
		params.AttackCheckBlocked = false;
		Commands->Action_Attack(obj,params);
		if (primary)
		{
			primary = false;
		}
		else
		{
			primary = true;
		}
	}
}

void JFW_Base_Defence_Animated_Sound_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Sound_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_Animated_Sound_No_Aircraft_Secondary::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Sound_No_Aircraft_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(enemy),obj);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			if (primary)
			{
				primary = false;
			}
			else
			{
				primary = true;
			}
		}
	}
}

void JFW_Base_Defence_Animated_Sound_No_Aircraft_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Sound_No_Aircraft_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_Animated_Sound_Aircraft_Only_Secondary::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Sound_Aircraft_Only_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(enemy),obj);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			if (primary)
			{	
				primary = false;
			}
			else
			{
				primary = true;
			}
		}
	}
}

void JFW_Base_Defence_Animated_Sound_Aircraft_Only_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Sound_Aircraft_Only_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_Animated_Sound_No_VTOL::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Sound_No_VTOL::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!Is_VTOL(enemy))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(enemy),obj);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,true);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
		}
	}
}

void JFW_Base_Defence_Animated_Sound_No_VTOL::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Sound_No_VTOL::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&attack,2,1);
}

void JFW_Base_Defence_Animated_Sound_VTOL_Only::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Sound_VTOL_Only::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (Is_VTOL(enemy))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(enemy),obj);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,true);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
		}
	}
}

void JFW_Base_Defence_Animated_Sound_VTOL_Only::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Sound_VTOL_Only::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&attack,2,1);
}

void JFW_Base_Defence_Animated_Sound_No_VTOL_Secondary::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Sound_No_VTOL_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!Is_VTOL(enemy))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(enemy),obj);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			if (primary)
			{
				primary = false;
			}
			else
			{
				primary = true;
			}
		}
	}
}

void JFW_Base_Defence_Animated_Sound_No_VTOL_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Sound_No_VTOL_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_Animated_Sound_VTOL_Only_Secondary::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Sound_VTOL_Only_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (Is_VTOL(enemy))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(enemy),obj);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			if (primary)
			{	
				primary = false;
			}
			else
			{
				primary = true;
			}
		}
	}
}

void JFW_Base_Defence_Animated_Sound_VTOL_Only_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Sound_VTOL_Only_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_Swap::Created(GameObject *obj)
{
	Vector3 pos,pos1,pos2,pos3;
	GameObject *object;
	primary = true;
	objtype = Commands->Get_Player_Type(obj);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	pos = Commands->Get_Position(obj);
	pos1.X = pos.X - 5;
	pos1.Y = pos.Y + 8.66025f;
	pos1.Z = pos.Z + 2;
	pos2.X = pos.X + 10;
	pos2.Y = pos.Y;
	pos2.Z = pos.Z + 2;
	pos3.X = pos.X - 5;
	pos3.Y = pos.Y - 8.66025f;
	pos3.Z = pos.Z + 2;
	object = Commands->Create_Object("Invisible_Object",pos1);
	if (object)
	{
		id1 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos2);
	if (object)
	{
		id2 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos3);
	if (object)
	{
		id3 = Commands->Get_ID(object);
	}
	Commands->Start_Timer(obj,this,10,1);
}

void JFW_Base_Defence_Swap::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	ActionParamsStruct params;
	Vector3 pos,pos2;
	float distance;
	float minattack,maxattack;
	float attacktimer;
	pos = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(enemy);
	distance = Commands->Get_Distance(pos2,pos);
	minattack = Get_Float_Parameter("MinAttackDistance");
	if (distance >= minattack)
	{
		maxattack = Get_Float_Parameter("MaxAttackDistance");
		params.Set_Basic(this,100,2);
		params.Set_Attack(enemy,maxattack,0.0,primary);
		params.AttackCheckBlocked = false;
		Commands->Action_Attack(obj,params);
		attacktimer = Get_Float_Parameter("AttackTimer");
		Stop_Timer2(obj,this,2);
		Commands->Start_Timer(obj,this,attacktimer,2);
	}
}

void JFW_Base_Defence_Swap::Timer_Expired(GameObject *obj,int number)
{
	ActionParamsStruct var;
	GameObject *object;
	float f;
	switch (number)
	{
	case 1:
		f = Commands->Get_Random(0.0f,2.9999f);
		switch (__min(int(f),2))
		{
		case 0:
			object = Commands->Find_Object(id1);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		case 1:
			object = Commands->Find_Object(id2);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		default:
			object = Commands->Find_Object(id3);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
		}
	Commands->Start_Timer(obj,this,10,1);
	break;
	case 2:
		Commands->Action_Reset(obj,100);
		break;
	}
}

void JFW_Base_Defence_Swap::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,4,1);
	Auto_Save_Variable(&id2,4,2);
	Auto_Save_Variable(&id3,4,3);
	Auto_Save_Variable(&objtype,4,4);
	Auto_Save_Variable(&primary,5,1);
}

void JFW_Base_Defence_No_Aircraft_Swap::Created(GameObject *obj)
{
	Vector3 pos,pos1,pos2,pos3;
	GameObject *object;
	primary = true;
	objtype = Commands->Get_Player_Type(obj);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	pos = Commands->Get_Position(obj);
	pos1.X = pos.X - 5;
	pos1.Y = pos.Y + 8.66025f;
	pos1.Z = pos.Z + 2;
	pos2.X = pos.X + 10;
	pos2.Y = pos.Y;
	pos2.Z = pos.Z + 2;
	pos3.X = pos.X - 5;
	pos3.Y = pos.Y - 8.66025f;
	pos3.Z = pos.Z + 2;
	object = Commands->Create_Object("Invisible_Object",pos1);
	if (object)
	{
		id1 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos2);
	if (object)
	{
		id2 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos3);
	if (object)
	{
		id3 = Commands->Get_ID(object);
	}
	Commands->Start_Timer(obj,this,10,1);
}

void JFW_Base_Defence_No_Aircraft_Swap::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		ActionParamsStruct params;
		Vector3 pos,pos2;
		float distance;
		float minattack,maxattack;
		float attacktimer;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		distance = Commands->Get_Distance(pos2,pos);
		minattack = Get_Float_Parameter("MinAttackDistance");
		if (distance >= minattack)
		{
			maxattack = Get_Float_Parameter("MaxAttackDistance");
			params.Set_Basic(this,100,2);
			params.Set_Attack(enemy,maxattack,0.0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			attacktimer = Get_Float_Parameter("AttackTimer");
			Commands->Start_Timer(obj,this,attacktimer,2);
		}
	}
}

void JFW_Base_Defence_No_Aircraft_Swap::Timer_Expired(GameObject *obj,int number)
{
	ActionParamsStruct var;
	GameObject *object;
	float f;
	switch (number)
	{
	case 1:
		f = Commands->Get_Random(0.0f,2.9999f);
		switch (__min(int(f),2))
		{
		case 0:
			object = Commands->Find_Object(id1);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		case 1:
			object = Commands->Find_Object(id2);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		default:
			object = Commands->Find_Object(id3);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
		}
	Commands->Start_Timer(obj,this,10,1);
	break;
	case 2:
		Commands->Action_Reset(obj,100);
		break;
	}
}

void JFW_Base_Defence_No_Aircraft_Swap::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,4,1);
	Auto_Save_Variable(&id2,4,2);
	Auto_Save_Variable(&id3,4,3);
	Auto_Save_Variable(&objtype,4,4);
	Auto_Save_Variable(&primary,5,1);
}

void JFW_Base_Defence_Aircraft_Only_Swap::Created(GameObject *obj)
{
	Vector3 pos,pos1,pos2,pos3;
	GameObject *object;
	primary = true;
	objtype = Commands->Get_Player_Type(obj);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	pos = Commands->Get_Position(obj);
	pos1.X = pos.X - 5;
	pos1.Y = pos.Y + 8.66025f;
	pos1.Z = pos.Z + 2;
	pos2.X = pos.X + 10;
	pos2.Y = pos.Y;
	pos2.Z = pos.Z + 2;
	pos3.X = pos.X - 5;
	pos3.Y = pos.Y - 8.66025f;
	pos3.Z = pos.Z + 2;
	object = Commands->Create_Object("Invisible_Object",pos1);
	if (object)
	{
		id1 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos2);
	if (object)
	{
		id2 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos3);
	if (object)
	{
		id3 = Commands->Get_ID(object);
	}
	Commands->Start_Timer(obj,this,10,1);
}

void JFW_Base_Defence_Aircraft_Only_Swap::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		ActionParamsStruct params;
		Vector3 pos,pos2;
		float distance;
		float minattack,maxattack;
		float attacktimer;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		distance = Commands->Get_Distance(pos2,pos);
		minattack = Get_Float_Parameter("MinAttackDistance");
		if (distance >= minattack)
		{
			maxattack = Get_Float_Parameter("MaxAttackDistance");
			params.Set_Basic(this,100,2);
			params.Set_Attack(enemy,maxattack,0.0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			attacktimer = Get_Float_Parameter("AttackTimer");
			Commands->Start_Timer(obj,this,attacktimer,2);
		}
	}
}

void JFW_Base_Defence_Aircraft_Only_Swap::Timer_Expired(GameObject *obj,int number)
{
	ActionParamsStruct var;
	GameObject *object;
	float f;
	switch (number)
	{
	case 1:
		f = Commands->Get_Random(0.0f,2.9999f);
		switch (__min(int(f),2))
		{
		case 0:
			object = Commands->Find_Object(id1);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		case 1:
			object = Commands->Find_Object(id2);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		default:
			object = Commands->Find_Object(id3);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
		}
	Commands->Start_Timer(obj,this,10,1);
	break;
	case 2:
		Commands->Action_Reset(obj,100);
		break;
	}
}

void JFW_Base_Defence_Aircraft_Only_Swap::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,4,1);
	Auto_Save_Variable(&id2,4,2);
	Auto_Save_Variable(&id3,4,3);
	Auto_Save_Variable(&objtype,4,4);
	Auto_Save_Variable(&primary,5,1);
}

void JFW_Base_Defence_Animated_Swap::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Swap::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!popup)
	{
		popup = true;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
		Stop_Timer2(obj,this,1);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
	}
	else if (attack)
	{
		ActionParamsStruct params;
		params.Set_Basic(this,100,1);
		params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
		params.AttackCheckBlocked = false;
		Commands->Action_Attack(obj,params);
	}
}

void JFW_Base_Defence_Animated_Swap::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Swap::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_Animated_No_Aircraft_Swap::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_No_Aircraft_Swap::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
		}
	}
}

void JFW_Base_Defence_Animated_No_Aircraft_Swap::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_No_Aircraft_Swap::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_Animated_Aircraft_Only_Swap::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Aircraft_Only_Swap::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
		}
	}
}

void JFW_Base_Defence_Animated_Aircraft_Only_Swap::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Aircraft_Only_Swap::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_No_VTOL_Swap::Created(GameObject *obj)
{
	Vector3 pos,pos1,pos2,pos3;
	GameObject *object;
	primary = true;
	objtype = Commands->Get_Player_Type(obj);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	pos = Commands->Get_Position(obj);
	pos1.X = pos.X - 5;
	pos1.Y = pos.Y + 8.66025f;
	pos1.Z = pos.Z + 2;
	pos2.X = pos.X + 10;
	pos2.Y = pos.Y;
	pos2.Z = pos.Z + 2;
	pos3.X = pos.X - 5;
	pos3.Y = pos.Y - 8.66025f;
	pos3.Z = pos.Z + 2;
	object = Commands->Create_Object("Invisible_Object",pos1);
	if (object)
	{
		id1 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos2);
	if (object)
	{
		id2 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos3);
	if (object)
	{
		id3 = Commands->Get_ID(object);
	}
	Commands->Start_Timer(obj,this,10,1);
}

void JFW_Base_Defence_No_VTOL_Swap::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!Is_VTOL(enemy))
	{
		ActionParamsStruct params;
		Vector3 pos,pos2;
		float distance;
		float minattack,maxattack;
		float attacktimer;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		distance = Commands->Get_Distance(pos2,pos);
		minattack = Get_Float_Parameter("MinAttackDistance");
		if (distance >= minattack)
		{
			maxattack = Get_Float_Parameter("MaxAttackDistance");
			params.Set_Basic(this,100,2);
			params.Set_Attack(enemy,maxattack,0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			attacktimer = Get_Float_Parameter("AttackTimer");
			Stop_Timer2(obj,this,2);
			Commands->Start_Timer(obj,this,attacktimer,2);
		}
	}
}

void JFW_Base_Defence_No_VTOL_Swap::Timer_Expired(GameObject *obj,int number)
{
	ActionParamsStruct var;
	GameObject *object;
	float f;
	switch (number)
	{
	case 1:
		f = Commands->Get_Random(0.0f,2.9999f);
		switch (__min(int(f),2))
		{
		case 0:
			object = Commands->Find_Object(id1);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		case 1:
			object = Commands->Find_Object(id2);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		default:
			object = Commands->Find_Object(id3);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
		}
	Commands->Start_Timer(obj,this,10,1);
	break;
	case 2:
		Commands->Action_Reset(obj,100);
		break;
	}
}

void JFW_Base_Defence_No_VTOL_Swap::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,4,1);
	Auto_Save_Variable(&id2,4,2);
	Auto_Save_Variable(&id3,4,3);
	Auto_Save_Variable(&objtype,4,4);
	Auto_Save_Variable(&primary,5,1);
}

void JFW_Base_Defence_VTOL_Only_Swap::Created(GameObject *obj)
{
	Vector3 pos,pos1,pos2,pos3;
	GameObject *object;
	primary = true;
	objtype = Commands->Get_Player_Type(obj);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	pos = Commands->Get_Position(obj);
	pos1.X = pos.X - 5;
	pos1.Y = pos.Y + 8.66025f;
	pos1.Z = pos.Z + 2;
	pos2.X = pos.X + 10;
	pos2.Y = pos.Y;
	pos2.Z = pos.Z + 2;
	pos3.X = pos.X - 5;
	pos3.Y = pos.Y - 8.66025f;
	pos3.Z = pos.Z + 2;
	object = Commands->Create_Object("Invisible_Object",pos1);
	if (object)
	{
		id1 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos2);
	if (object)
	{
		id2 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object",pos3);
	if (object)
	{
		id3 = Commands->Get_ID(object);
	}
	Commands->Start_Timer(obj,this,10,1);
}

void JFW_Base_Defence_VTOL_Only_Swap::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (Is_VTOL(enemy))
	{
		ActionParamsStruct params;
		Vector3 pos,pos2;
		float distance;
		float minattack,maxattack;
		float attacktimer;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		distance = Commands->Get_Distance(pos2,pos);
		minattack = Get_Float_Parameter("MinAttackDistance");
		if (distance >= minattack)
		{
			maxattack = Get_Float_Parameter("MaxAttackDistance");
			params.Set_Basic(this,100,2);
			params.Set_Attack(enemy,maxattack,0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			attacktimer = Get_Float_Parameter("AttackTimer");
			Stop_Timer2(obj,this,2);
			Commands->Start_Timer(obj,this,attacktimer,2);
		}
	}
}

void JFW_Base_Defence_VTOL_Only_Swap::Timer_Expired(GameObject *obj,int number)
{
	ActionParamsStruct var;
	GameObject *object;
	float f;
	switch (number)
	{
	case 1:
		f = Commands->Get_Random(0.0f,2.9999f);
		switch (__min(int(f),2))
		{
		case 0:
			object = Commands->Find_Object(id1);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		case 1:
			object = Commands->Find_Object(id2);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		default:
			object = Commands->Find_Object(id3);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0.0,0.0,true);
				Commands->Action_Attack(obj,var);
			}
		}
	Commands->Start_Timer(obj,this,10,1);
	break;
	case 2:
		Commands->Action_Reset(obj,100);
		break;
	}
}

void JFW_Base_Defence_VTOL_Only_Swap::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,4,1);
	Auto_Save_Variable(&id2,4,2);
	Auto_Save_Variable(&id3,4,3);
	Auto_Save_Variable(&objtype,4,4);
	Auto_Save_Variable(&primary,5,1);
}

void JFW_Base_Defence_Animated_No_VTOL_Swap::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_No_VTOL_Swap::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!Is_VTOL(enemy))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
		}
	}
}

void JFW_Base_Defence_Animated_No_VTOL_Swap::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_No_VTOL_Swap::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_Animated_VTOL_Only_Swap::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_VTOL_Only_Swap::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (Is_VTOL(enemy))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
		}
	}
}

void JFW_Base_Defence_Animated_VTOL_Only_Swap::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_VTOL_Only_Swap::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_Animated_Sound_Swap::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Sound_Swap::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!popup)
	{
		popup = true;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
		Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(enemy),obj);
		Stop_Timer2(obj,this,1);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
	}
	else if (attack)
	{
		ActionParamsStruct params;
		params.Set_Basic(this,100,1);
		params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
		params.AttackCheckBlocked = false;
		Commands->Action_Attack(obj,params);
	}
}

void JFW_Base_Defence_Animated_Sound_Swap::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Sound_Swap::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_Animated_Sound_No_Aircraft_Swap::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Sound_No_Aircraft_Swap::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(enemy),obj);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
		}
	}
}

void JFW_Base_Defence_Animated_Sound_No_Aircraft_Swap::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Sound_No_Aircraft_Swap::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_Animated_Sound_Aircraft_Only_Swap::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Sound_Aircraft_Only_Swap::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(enemy),obj);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
		}
	}
}

void JFW_Base_Defence_Animated_Sound_Aircraft_Only_Swap::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Sound_Aircraft_Only_Swap::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_Animated_Sound_No_VTOL_Swap::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Sound_No_VTOL_Swap::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!Is_VTOL(enemy))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(enemy),obj);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
		}
	}
}

void JFW_Base_Defence_Animated_Sound_No_VTOL_Swap::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Sound_No_VTOL_Swap::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_Animated_Sound_VTOL_Only_Swap::Created(GameObject *obj)
{
	popup = false;
	attack = false;
	primary = true;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Base_Defence_Animated_Sound_VTOL_Only_Swap::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (Is_VTOL(enemy))
	{
		if (!popup)
		{
			popup = true;
			Commands->Start_Timer(obj,this,Get_Float_Parameter("PopupTime"),2);
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
			Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(enemy),obj);
			Stop_Timer2(obj,this,1);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("AttackTimer"),1);
		}
		else if (attack)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,Get_Float_Parameter("MaxAttackDistance"),0,primary);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
		}
	}
}

void JFW_Base_Defence_Animated_Sound_VTOL_Only_Swap::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		popup = false;
		attack = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
		Commands->Action_Reset(obj,100);
	}
	if (number == 2)
	{
		attack = true;
	}
}

void JFW_Base_Defence_Animated_Sound_VTOL_Only_Swap::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&popup,1,1);
	Auto_Save_Variable(&primary,1,2);
	Auto_Save_Variable(&attack,3,1);
}

void JFW_Base_Defence_Swap::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("SwapMessage"))
	{
		if (primary)
		{
			primary = false;
		}
		else
		{
			primary = true;
		}
	}
}

void JFW_Base_Defence_No_Aircraft_Swap::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("SwapMessage"))
	{
		if (primary)
		{
			primary = false;
		}
		else
		{
			primary = true;
		}
	}
}

void JFW_Base_Defence_Aircraft_Only_Swap::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("SwapMessage"))
	{
		if (primary)
		{
			primary = false;
		}
		else
		{
			primary = true;
		}
	}
}

void JFW_Base_Defence_Animated_Swap::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("SwapMessage"))
	{
		if (primary)
		{
			primary = false;
		}
		else
		{
			primary = true;
		}
	}
}

void JFW_Base_Defence_Animated_No_Aircraft_Swap::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("SwapMessage"))
	{
		if (primary)
		{
			primary = false;
		}
		else
		{
			primary = true;
		}
	}
}

void JFW_Base_Defence_Animated_Aircraft_Only_Swap::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("SwapMessage"))
	{
		if (primary)
		{
			primary = false;
		}
		else
		{
			primary = true;
		}
	}
}

void JFW_Base_Defence_No_VTOL_Swap::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("SwapMessage"))
	{
		if (primary)
		{
			primary = false;
		}
		else
		{
			primary = true;
		}
	}
}

void JFW_Base_Defence_VTOL_Only_Swap::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("SwapMessage"))
	{
		if (primary)
		{
			primary = false;
		}
		else
		{
			primary = true;
		}
	}
}

void JFW_Base_Defence_Animated_No_VTOL_Swap::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("SwapMessage"))
	{
		if (primary)
		{
			primary = false;
		}
		else
		{
			primary = true;
		}
	}
}

void JFW_Base_Defence_Animated_VTOL_Only_Swap::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("SwapMessage"))
	{
		if (primary)
		{
			primary = false;
		}
		else
		{
			primary = true;
		}
	}
}

void JFW_Base_Defence_Animated_Sound_Swap::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("SwapMessage"))
	{
		if (primary)
		{
			primary = false;
		}
		else
		{
			primary = true;
		}
	}
}

void JFW_Base_Defence_Animated_Sound_No_Aircraft_Swap::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("SwapMessage"))
	{
		if (primary)
		{
			primary = false;
		}
		else
		{
			primary = true;
		}
	}
}

void JFW_Base_Defence_Animated_Sound_Aircraft_Only_Swap::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("SwapMessage"))
	{
		if (primary)
		{
			primary = false;
		}
		else
		{
			primary = true;
		}
	}
}

void JFW_Base_Defence_Animated_Sound_No_VTOL_Swap::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("SwapMessage"))
	{
		if (primary)
		{
			primary = false;
		}
		else
		{
			primary = true;
		}
	}
}

void JFW_Base_Defence_Animated_Sound_VTOL_Only_Swap::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("SwapMessage"))
	{
		if (primary)
		{
			primary = false;
		}
		else
		{
			primary = true;
		}
	}
}

ScriptRegistrant<JFW_User_Controllable_Base_Defence> JFW_User_Controllable_Base_Defence_Registrant("JFW_User_Controllable_Base_Defence","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:float");
ScriptRegistrant<JFW_Base_Defence> JFW_Base_Defence_Registrant("JFW_Base_Defence","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:float");
ScriptRegistrant<JFW_Base_Defence> M00_Base_Defense_Registrant("M00_Base_Defense","MinAttackDistance=0:int,MaxAttackDistance=300:int,AttackTimer=10:int");
ScriptRegistrant<JFW_Base_Defence_No_Aircraft> JFW_Base_Defence_No_Aircraft_Registrant("JFW_Base_Defence_No_Aircraft","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string");
ScriptRegistrant<JFW_Base_Defence_Aircraft_Only> JFW_Base_Defence_Aircraft_Only_Registrant("JFW_Base_Defence_Aircraft_Only","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string");
ScriptRegistrant<JFW_Base_Defence_Secondary> JFW_Base_Defence_Secondary_Registrant("JFW_Base_Defence_Secondary","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:float");
ScriptRegistrant<JFW_Base_Defence_No_Aircraft_Secondary> JFW_Base_Defence_No_Aircraft_Secondary_Registrant("JFW_Base_Defence_No_Aircraft_Secondary","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string");
ScriptRegistrant<JFW_Base_Defence_Aircraft_Only_Secondary> JFW_Base_Defence_Aircraft_Only_Secondary_Registrant("JFW_Base_Defence_Aircraft_Only_Secondary","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string");
ScriptRegistrant<JFW_Base_Defence_Animated> JFW_Base_Defence_Animated_Registrant("JFW_Base_Defence_Animated","MaxAttackDistance:float,AttackTimer:float,Animation:string,LastFrame:float,PopupTime:float");
ScriptRegistrant<JFW_Base_Defence_Animated_No_Aircraft> JFW_Base_Defence_Animated_No_Aircraft_Registrant("JFW_Base_Defence_Animated_No_Aircraft","MaxAttackDistance:float,AttackTimer:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,Animation:string,LastFrame:float,PopupTime:float");
ScriptRegistrant<JFW_Base_Defence_Animated_Aircraft_Only> JFW_Base_Defence_Animated_Aircraft_Only_Registrant("JFW_Base_Defence_Animated_Aircraft_Only","MaxAttackDistance:float,AttackTimer:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,Animation:string,LastFrame:float,PopupTime:float");
ScriptRegistrant<JFW_Base_Defence_Animated_Secondary> JFW_Base_Defence_Animated_Secondary_Registrant("JFW_Base_Defence_Animated_Secondary","MaxAttackDistance:float,AttackTimer:float,Animation:string,LastFrame:float,PopupTime:float");
ScriptRegistrant<JFW_Base_Defence_Animated_No_Aircraft_Secondary> JFW_Base_Defence_Animated_No_Aircraft_Secondary_Registrant("JFW_Base_Defence_Animated_No_Aircraft_Secondary","MaxAttackDistance:float,AttackTimer:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,Animation:string,LastFrame:float,PopupTime:float");
ScriptRegistrant<JFW_Base_Defence_Animated_Aircraft_Only_Secondary> JFW_Base_Defence_Animated_Aircraft_Only_Secondary_Registrant("JFW_Base_Defence_Animated_Aircraft_Only_Secondary","MaxAttackDistance:float,AttackTimer:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,Animation:string,LastFrame:float,PopupTime:float");
ScriptRegistrant<JFW_Base_Defence_No_VTOL> JFW_Base_Defence_No_VTOL_Registrant("JFW_Base_Defence_No_VTOL","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:float");
ScriptRegistrant<JFW_Base_Defence_No_VTOL_No_Stealth> JFW_Base_Defence_No_VTOL_No_Stealth_Registrant("JFW_Base_Defence_No_VTOL_No_Stealth","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:float,MaxStealthAttackDistance=0.0:float");
ScriptRegistrant<JFW_Base_Defence_VTOL_Only> JFW_Base_Defence_VTOL_Only_Registrant("JFW_Base_Defence_VTOL_Only","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:float");
ScriptRegistrant<JFW_Base_Defence_No_VTOL_Secondary> JFW_Base_Defence_No_VTOL_Secondary_Registrant("JFW_Base_Defence_No_VTOL_Secondary","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:float");
ScriptRegistrant<JFW_Base_Defence_VTOL_Only_Secondary> JFW_Base_Defence_VTOL_Only_Secondary_Registrant("JFW_Base_Defence_VTOL_Only_Secondary","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:float");
ScriptRegistrant<JFW_Base_Defence_Animated_No_VTOL> JFW_Base_Defence_Animated_No_VTOL_Registrant("JFW_Base_Defence_Animated_No_VTOL","MaxAttackDistance:float,AttackTimer:float,Animation:string,LastFrame:float,PopupTime:float");
ScriptRegistrant<JFW_Base_Defence_Animated_VTOL_Only> JFW_Base_Defence_Animated_VTOL_Only_Registrant("JFW_Base_Defence_Animated_VTOL_Only","MaxAttackDistance:float,AttackTimer:float,Animation:string,LastFrame:float,PopupTime:float");
ScriptRegistrant<JFW_Base_Defence_Animated_No_VTOL_Secondary> JFW_Base_Defence_Animated_No_VTOL_Secondary_Registrant("JFW_Base_Defence_Animated_No_VTOL_Secondary","MaxAttackDistance:float,AttackTimer:float,Animation:string,LastFrame:float,PopupTime:float");
ScriptRegistrant<JFW_Base_Defence_Animated_VTOL_Only_Secondary> JFW_Base_Defence_Animated_VTOL_Only_Secondary_Registrant("JFW_Base_Defence_Animated_VTOL_Only_Secondary","MaxAttackDistance:float,AttackTimer:float,Animation:string,LastFrame:float,PopupTime:float");
ScriptRegistrant<JFW_Base_Defence_Animated_Sound> JFW_Base_Defence_Animated_Sound_Registrant("JFW_Base_Defence_Animated_Sound","MaxAttackDistance:float,AttackTimer:float,Animation:string,LastFrame:float,PopupTime:float,Sound:string");
ScriptRegistrant<JFW_Base_Defence_Animated_Sound_No_Aircraft> JFW_Base_Defence_Animated_Sound_No_Aircraft_Registrant("JFW_Base_Defence_Animated_Sound_No_Aircraft","MaxAttackDistance:float,AttackTimer:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,Animation:string,LastFrame:float,PopupTime:float,Sound:string");
ScriptRegistrant<JFW_Base_Defence_Animated_Sound_Aircraft_Only> JFW_Base_Defence_Animated_Sound_Aircraft_Only_Registrant("JFW_Base_Defence_Animated_Sound_Aircraft_Only","MaxAttackDistance:float,AttackTimer:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,Animation:string,LastFrame:float,PopupTime:float,Sound:string");
ScriptRegistrant<JFW_Base_Defence_Animated_Sound_Secondary> JFW_Base_Defence_Animated_Sound_Secondary_Registrant("JFW_Base_Defence_Animated_Sound_Secondary","MaxAttackDistance:float,AttackTimer:float,Animation:string,LastFrame:float,PopupTime:float,Sound:string");
ScriptRegistrant<JFW_Base_Defence_Animated_Sound_No_Aircraft_Secondary> JFW_Base_Defence_Animated_Sound_No_Aircraft_Secondary_Registrant("JFW_Base_Defence_Animated_Sound_No_Aircraft_Secondary","MaxAttackDistance:float,AttackTimer:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,Animation:string,LastFrame:float,PopupTime:float,Sound:string");
ScriptRegistrant<JFW_Base_Defence_Animated_Sound_Aircraft_Only_Secondary> JFW_Base_Defence_Animated_Sound_Aircraft_Only_Secondary_Registrant("JFW_Base_Defence_Animated_Sound_Aircraft_Only_Secondary","MaxAttackDistance:float,AttackTimer:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,Animation:string,LastFrame:float,PopupTime:float,Sound:string");
ScriptRegistrant<JFW_Base_Defence_Animated_Sound_No_VTOL> JFW_Base_Defence_Animated_Sound_No_VTOL_Registrant("JFW_Base_Defence_Animated_Sound_No_VTOL","MaxAttackDistance:float,AttackTimer:float,Animation:string,LastFrame:float,PopupTime:float,Sound:string");
ScriptRegistrant<JFW_Base_Defence_Animated_Sound_VTOL_Only> JFW_Base_Defence_Animated_Sound_VTOL_Only_Registrant("JFW_Base_Defence_Animated_Sound_VTOL_Only","MaxAttackDistance:float,AttackTimer:float,Animation:string,LastFrame:float,PopupTime:float,Sound:string");
ScriptRegistrant<JFW_Base_Defence_Animated_Sound_No_VTOL_Secondary> JFW_Base_Defence_Animated_Sound_No_VTOL_Secondary_Registrant("JFW_Base_Defence_Animated_Sound_No_VTOL_Secondary","MaxAttackDistance:float,AttackTimer:float,Animation:string,LastFrame:float,PopupTime:float,Sound:string");
ScriptRegistrant<JFW_Base_Defence_Animated_Sound_VTOL_Only_Secondary> JFW_Base_Defence_Animated_Sound_VTOL_Only_Secondary_Registrant("JFW_Base_Defence_Animated_Sound_VTOL_Only_Secondary","MaxAttackDistance:float,AttackTimer:float,Animation:string,LastFrame:float,PopupTime:float,Sound:string");
ScriptRegistrant<JFW_Base_Defence_Swap> JFW_Base_Defence_Swap_Registrant("JFW_Base_Defence_Swap","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:float,SwapMessage:int");
ScriptRegistrant<JFW_Base_Defence_No_Aircraft_Swap> JFW_Base_Defence_No_Aircraft_Swap_Registrant("JFW_Base_Defence_No_Aircraft_Swap","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,SwapMessage:int");
ScriptRegistrant<JFW_Base_Defence_Aircraft_Only_Swap> JFW_Base_Defence_Aircraft_Only_Swap_Registrant("JFW_Base_Defence_Aircraft_Only_Swap","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,SwapMessage:int");
ScriptRegistrant<JFW_Base_Defence_Animated_Swap> JFW_Base_Defence_Animated_Swap_Registrant("JFW_Base_Defence_Animated_Swap","MaxAttackDistance:float,AttackTimer:float,Animation:string,LastFrame:float,PopupTime:float,SwapMessage:int");
ScriptRegistrant<JFW_Base_Defence_Animated_No_Aircraft_Swap> JFW_Base_Defence_Animated_No_Aircraft_Swap_Registrant("JFW_Base_Defence_Animated_No_Aircraft_Swap","MaxAttackDistance:float,AttackTimer:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,Animation:string,LastFrame:float,PopupTime:float,SwapMessage:int");
ScriptRegistrant<JFW_Base_Defence_Animated_Aircraft_Only_Swap> JFW_Base_Defence_Animated_Aircraft_Only_Swap_Registrant("JFW_Base_Defence_Animated_Aircraft_Only_Swap","MaxAttackDistance:float,AttackTimer:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,Animation:string,LastFrame:float,PopupTime:float,SwapMessage:int");
ScriptRegistrant<JFW_Base_Defence_No_VTOL_Swap> JFW_Base_Defence_No_VTOL_Swap_Registrant("JFW_Base_Defence_No_VTOL_Swap","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:float,SwapMessage:int");
ScriptRegistrant<JFW_Base_Defence_VTOL_Only_Swap> JFW_Base_Defence_VTOL_Only_Swap_Registrant("JFW_Base_Defence_VTOL_Only_Swap","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:float,SwapMessage:int");
ScriptRegistrant<JFW_Base_Defence_Animated_No_VTOL_Swap> JFW_Base_Defence_Animated_No_VTOL_Swap_Registrant("JFW_Base_Defence_Animated_No_VTOL_Swap","MaxAttackDistance:float,AttackTimer:float,Animation:string,LastFrame:float,PopupTime:float,SwapMessage:int");
ScriptRegistrant<JFW_Base_Defence_Animated_VTOL_Only_Swap> JFW_Base_Defence_Animated_VTOL_Only_Swap_Registrant("JFW_Base_Defence_Animated_VTOL_Only_Swap","MaxAttackDistance:float,AttackTimer:float,Animation:string,LastFrame:float,PopupTime:float,SwapMessage:int");
ScriptRegistrant<JFW_Base_Defence_Animated_Sound_Swap> JFW_Base_Defence_Animated_Sound_Swap_Registrant("JFW_Base_Defence_Animated_Sound_Swap","MaxAttackDistance:float,AttackTimer:float,Animation:string,LastFrame:float,PopupTime:float,Sound:string,SwapMessage:int");
ScriptRegistrant<JFW_Base_Defence_Animated_Sound_No_Aircraft_Swap> JFW_Base_Defence_Animated_Sound_No_Aircraft_Swap_Registrant("JFW_Base_Defence_Animated_Sound_No_Aircraft_Swap","MaxAttackDistance:float,AttackTimer:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,Animation:string,LastFrame:float,PopupTime:float,Sound:string,SwapMessage:int");
ScriptRegistrant<JFW_Base_Defence_Animated_Sound_Aircraft_Only_Swap> JFW_Base_Defence_Animated_Sound_Aircraft_Only_Swap_Registrant("JFW_Base_Defence_Animated_Sound_Aircraft_Only_Swap","MaxAttackDistance:float,AttackTimer:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,Animation:string,LastFrame:float,PopupTime:float,Sound:string,SwapMessage:int");
ScriptRegistrant<JFW_Base_Defence_Animated_Sound_No_VTOL_Swap> JFW_Base_Defence_Animated_Sound_No_VTOL_Swap_Registrant("JFW_Base_Defence_Animated_Sound_No_VTOL_Swap","MaxAttackDistance:float,AttackTimer:float,Animation:string,LastFrame:float,PopupTime:float,Sound:string,SwapMessage:int");
ScriptRegistrant<JFW_Base_Defence_Animated_Sound_VTOL_Only_Swap> JFW_Base_Defence_Animated_Sound_VTOL_Only_Swap_Registrant("JFW_Base_Defence_Animated_Sound_VTOL_Only_Swap","MaxAttackDistance:float,AttackTimer:float,Animation:string,LastFrame:float,PopupTime:float,Sound:string,SwapMessage:int");
