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
#include "jfwgun.h"
#include "VehicleGameObjDef.h"
#include "PhysicalGameObj.h"
#include "SmartGameObj.h"

void JFW_Building_Gun::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Weapon",params);
	}
}

void JFW_Building_Gun::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
			else
			{
				Commands->Send_Custom_Event(obj,object,x,1,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Weapon::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
}

void JFW_Building_Gun_Weapon::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,param != 0);
	}
}

void JFW_Building_Gun_Weapon::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	float minrange;
	float maxrange;
	float range;
	minrange = Get_Float_Parameter("Min_Range");
	maxrange = Get_Float_Parameter("Max_Range");
	Vector3 pos,pos2;
	pos = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(enemy);
	range = Commands->Get_Distance(pos,pos2);
	pos.Z = 0;
	pos2.Z = 0;
	if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange))
	{
		ActionParamsStruct params;
		params.Set_Basic(this,100,1);
		params.Set_Attack(enemy,maxrange,0,true);
		Commands->Action_Attack(obj,params);
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Weapon::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_No_Aircraft::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("Preset1"),Get_Parameter("Preset2"),Get_Parameter("Preset3"),Get_Parameter("Preset4"),Get_Parameter("Preset5"),Get_Parameter("Preset6"),Get_Parameter("Preset7"),Get_Parameter("Preset8"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Weapon_No_Aircraft",params);
	}
}

void JFW_Building_Gun_No_Aircraft::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_No_Aircraft::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
			else
			{
				Commands->Send_Custom_Event(obj,object,x,1,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_No_Aircraft::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Weapon_No_Aircraft::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
}

void JFW_Building_Gun_Weapon_No_Aircraft::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		if (!param)
		{
			Commands->Enable_Enemy_Seen(obj,false);
		}
		else
		{
			Commands->Enable_Enemy_Seen(obj,true);
		}
	}
}

void JFW_Building_Gun_Weapon_No_Aircraft::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	float minrange;
	float maxrange;
	float range;
	if ((_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange))
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,maxrange,0,true);
			Commands->Action_Attack(obj,params);
			Commands->Start_Timer(obj,this,1.0,1);
		}
	}
}

void JFW_Building_Gun_Weapon_No_Aircraft::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Aircraft_Only::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("Preset1"),Get_Parameter("Preset2"),Get_Parameter("Preset3"),Get_Parameter("Preset4"),Get_Parameter("Preset5"),Get_Parameter("Preset6"),Get_Parameter("Preset7"),Get_Parameter("Preset8"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Weapon_Aircraft_Only",params);
	}
}

void JFW_Building_Gun_Aircraft_Only::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Aircraft_Only::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
			else
			{
				Commands->Send_Custom_Event(obj,object,x,1,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Aircraft_Only::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Weapon_Aircraft_Only::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
}

void JFW_Building_Gun_Weapon_Aircraft_Only::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		if (!param)
		{
			Commands->Enable_Enemy_Seen(obj,false);
		}
		else
		{
			Commands->Enable_Enemy_Seen(obj,true);
		}
	}
}

void JFW_Building_Gun_Weapon_Aircraft_Only::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	float minrange;
	float maxrange;
	float range;
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange))
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,maxrange,0,true);
			Commands->Action_Attack(obj,params);
			Commands->Start_Timer(obj,this,1.0,1);
		}
	}
}

void JFW_Building_Gun_Weapon_Aircraft_Only::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Secondary::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Weapon_Secondary",params);
	}
}

void JFW_Building_Gun_Secondary::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Secondary::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
			else
			{
				Commands->Send_Custom_Event(obj,object,x,1,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Weapon_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&primary,1,1);
}

void JFW_Building_Gun_Weapon_Secondary::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	primary = true;
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
}

void JFW_Building_Gun_Weapon_Secondary::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,param != 0);
	}
}

void JFW_Building_Gun_Weapon_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	float minrange;
	float maxrange;
	float range;
	minrange = Get_Float_Parameter("Min_Range");
	maxrange = Get_Float_Parameter("Max_Range");
	Vector3 pos,pos2;
	pos = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(enemy);
	range = Commands->Get_Distance(pos,pos2);
	pos.Z = 0;
	pos2.Z = 0;
	if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange))
	{
		ActionParamsStruct params;
		params.Set_Basic(this,100,1);
		params.Set_Attack(enemy,maxrange,0,primary);
		Commands->Action_Attack(obj,params);
		Commands->Start_Timer(obj,this,1.0,1);
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

void JFW_Building_Gun_Weapon_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_No_Aircraft_Secondary::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("Preset1"),Get_Parameter("Preset2"),Get_Parameter("Preset3"),Get_Parameter("Preset4"),Get_Parameter("Preset5"),Get_Parameter("Preset6"),Get_Parameter("Preset7"),Get_Parameter("Preset8"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Weapon_No_Aircraft_Secondary",params);
	}
}

void JFW_Building_Gun_No_Aircraft_Secondary::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_No_Aircraft_Secondary::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
			else
			{
				Commands->Send_Custom_Event(obj,object,x,1,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_No_Aircraft_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Weapon_No_Aircraft_Secondary::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	primary = true;
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible != 0);
}

void JFW_Building_Gun_Weapon_No_Aircraft_Secondary::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		if (!param)
		{
			Commands->Enable_Enemy_Seen(obj,false);
		}
		else
		{
			Commands->Enable_Enemy_Seen(obj,true);
		}
	}
}

void JFW_Building_Gun_Weapon_No_Aircraft_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	float minrange;
	float maxrange;
	float range;
	if ((_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange))
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,maxrange,0,primary);
			Commands->Action_Attack(obj,params);
			Commands->Start_Timer(obj,this,1.0,1);
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

void JFW_Building_Gun_Weapon_No_Aircraft_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Aircraft_Only_Secondary::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("Preset1"),Get_Parameter("Preset2"),Get_Parameter("Preset3"),Get_Parameter("Preset4"),Get_Parameter("Preset5"),Get_Parameter("Preset6"),Get_Parameter("Preset7"),Get_Parameter("Preset8"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Weapon_Aircraft_Only_Secondary",params);
	}
}

void JFW_Building_Gun_Aircraft_Only_Secondary::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Aircraft_Only_Secondary::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
			else
			{
				Commands->Send_Custom_Event(obj,object,x,1,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Aircraft_Only_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Weapon_Aircraft_Only_Secondary::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	primary = true;
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
}

void JFW_Building_Gun_Weapon_Aircraft_Only_Secondary::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		if (!param)
		{
			Commands->Enable_Enemy_Seen(obj,false);
		}
		else
		{
			Commands->Enable_Enemy_Seen(obj,true);
		}
	}
}

void JFW_Building_Gun_Weapon_Aircraft_Only_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	float minrange;
	float maxrange;
	float range;
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange))
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,maxrange,0,primary);
			Commands->Action_Attack(obj,params);
			Commands->Start_Timer(obj,this,1.0,1);
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

void JFW_Building_Gun_Weapon_Aircraft_Only_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Weapon_No_Aircraft_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&primary,1,1);
}

void JFW_Building_Gun_Weapon_Aircraft_Only_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&primary,1,1);
}

void JFW_Building_Gun_Animated::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Weapon",params);
	}
}

void JFW_Building_Gun_Animated::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Weapon::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	enemyseen = true;
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Weapon::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Weapon::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	float minrange;
	float maxrange;
	float range;
	minrange = Get_Float_Parameter("Min_Range");
	maxrange = Get_Float_Parameter("Max_Range");
	Vector3 pos,pos2;
	pos = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(enemy);
	range = Commands->Get_Distance(pos,pos2);
	pos.Z = 0;
	pos2.Z = 0;
	if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
	{
		enemyseen = false;
		Commands->Enable_Enemy_Seen(obj,false);
		attackid = Commands->Get_ID(enemy);
		if (!lastframe)
		{
			lastframe = true;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
		}
	}
}

void JFW_Building_Gun_Animated_Weapon::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,true);
				Commands->Action_Attack(obj,params);
			}
		}
	}
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_No_Aircraft::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("Preset1"),Get_Parameter("Preset2"),Get_Parameter("Preset3"),Get_Parameter("Preset4"),Get_Parameter("Preset5"),Get_Parameter("Preset6"),Get_Parameter("Preset7"),Get_Parameter("Preset8"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Weapon_No_Aircraft",params);
	}
}

void JFW_Building_Gun_Animated_No_Aircraft::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated_No_Aircraft::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated_No_Aircraft::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Weapon_No_Aircraft::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Weapon_No_Aircraft::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Weapon_No_Aircraft::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			enemyseen = false;
			Commands->Enable_Enemy_Seen(obj,false);
			attackid = Commands->Get_ID(enemy);
			if (!lastframe)
			{
				lastframe = true;
				Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
				Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
			}
		}
	}
}

void JFW_Building_Gun_Animated_Weapon_No_Aircraft::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,true);
				Commands->Action_Attack(obj,params);
			}
		}
	}
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_Aircraft_Only::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("Preset1"),Get_Parameter("Preset2"),Get_Parameter("Preset3"),Get_Parameter("Preset4"),Get_Parameter("Preset5"),Get_Parameter("Preset6"),Get_Parameter("Preset7"),Get_Parameter("Preset8"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Weapon_Aircraft_Only",params);
	}
}

void JFW_Building_Gun_Animated_Aircraft_Only::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated_Aircraft_Only::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated_Aircraft_Only::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Weapon_Aircraft_Only::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	enemyseen = true;
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Weapon_Aircraft_Only::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Weapon_Aircraft_Only::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			enemyseen = false;
			Commands->Enable_Enemy_Seen(obj,false);
			attackid = Commands->Get_ID(enemy);
			if (!lastframe)
			{
				lastframe = true;
				Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
				Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
			}
		}
	}
}

void JFW_Building_Gun_Animated_Weapon_Aircraft_Only::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,true);
				Commands->Action_Attack(obj,params);
			}
		}
	}
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_Secondary::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Weapon_Secondary",params);
	}
}

void JFW_Building_Gun_Animated_Secondary::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated_Secondary::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Weapon_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&primary,1,1);
	Auto_Save_Variable(&effectid,4,2);
	Auto_Save_Variable(&attackid,4,3);
	Auto_Save_Variable(&enemyseen,1,4);
	Auto_Save_Variable(&lastframe,1,5);
}

void JFW_Building_Gun_Animated_Weapon::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&effectid,4,1);
	Auto_Save_Variable(&attackid,4,2);
	Auto_Save_Variable(&enemyseen,1,3);
	Auto_Save_Variable(&lastframe,1,4);
}

void JFW_Building_Gun_Animated_Weapon_Secondary::Created(GameObject *obj)
{
	primary = true;
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	enemyseen = true;
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Weapon_Secondary::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Weapon_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	float minrange;
	float maxrange;
	float range;
	minrange = Get_Float_Parameter("Min_Range");
	maxrange = Get_Float_Parameter("Max_Range");
	Vector3 pos,pos2;
	pos = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(enemy);
	range = Commands->Get_Distance(pos,pos2);
	pos.Z = 0;
	pos2.Z = 0;
	if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
	{
		enemyseen = false;
		Commands->Enable_Enemy_Seen(obj,false);
		attackid = Commands->Get_ID(enemy);
		if (!lastframe)
		{
			lastframe = true;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
		}
	}
}

void JFW_Building_Gun_Animated_Weapon_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,primary);
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
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_No_Aircraft_Secondary::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("Preset1"),Get_Parameter("Preset2"),Get_Parameter("Preset3"),Get_Parameter("Preset4"),Get_Parameter("Preset5"),Get_Parameter("Preset6"),Get_Parameter("Preset7"),Get_Parameter("Preset8"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Weapon_No_Aircraft_Secondary",params);
	}
}

void JFW_Building_Gun_Animated_No_Aircraft_Secondary::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated_No_Aircraft_Secondary::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated_No_Aircraft_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Weapon_No_Aircraft_Secondary::Created(GameObject *obj)
{
	primary = true;
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	enemyseen = true;
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Weapon_No_Aircraft_Secondary::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Weapon_No_Aircraft_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			enemyseen = false;
			Commands->Enable_Enemy_Seen(obj,false);
			attackid = Commands->Get_ID(enemy);
			if (!lastframe)
			{
				lastframe = true;
				Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
				Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
			}
		}
	}
}

void JFW_Building_Gun_Animated_Weapon_No_Aircraft_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,primary);
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
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_Aircraft_Only_Secondary::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("Preset1"),Get_Parameter("Preset2"),Get_Parameter("Preset3"),Get_Parameter("Preset4"),Get_Parameter("Preset5"),Get_Parameter("Preset6"),Get_Parameter("Preset7"),Get_Parameter("Preset8"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Weapon_Aircraft_Only_Secondary",params);
	}
}

void JFW_Building_Gun_Animated_Aircraft_Only_Secondary::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated_Aircraft_Only_Secondary::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated_Aircraft_Only_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Weapon_Aircraft_Only_Secondary::Created(GameObject *obj)
{
	primary = true;
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	enemyseen = true;
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Weapon_Aircraft_Only_Secondary::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Weapon_Aircraft_Only_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			enemyseen = false;
			Commands->Enable_Enemy_Seen(obj,false);
			attackid = Commands->Get_ID(enemy);
			if (!lastframe)
			{
				lastframe = true;
				Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
				Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
			}
		}
	}
}

void JFW_Building_Gun_Animated_Weapon_Aircraft_Only_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,primary);
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
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_Weapon_No_Aircraft::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&effectid,4,1);
	Auto_Save_Variable(&attackid,4,2);
	Auto_Save_Variable(&enemyseen,1,3);
	Auto_Save_Variable(&lastframe,1,4);
}

void JFW_Building_Gun_Animated_Weapon_Aircraft_Only::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&effectid,4,1);
	Auto_Save_Variable(&attackid,4,2);
	Auto_Save_Variable(&enemyseen,1,3);
	Auto_Save_Variable(&lastframe,1,4);
}

void JFW_Building_Gun_Animated_Weapon_No_Aircraft_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&primary,1,1);
	Auto_Save_Variable(&effectid,4,2);
	Auto_Save_Variable(&attackid,4,3);
	Auto_Save_Variable(&enemyseen,1,4);
	Auto_Save_Variable(&lastframe,1,5);
}

void JFW_Building_Gun_Animated_Weapon_Aircraft_Only_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&primary,1,1);
	Auto_Save_Variable(&effectid,4,2);
	Auto_Save_Variable(&attackid,4,3);
	Auto_Save_Variable(&enemyseen,1,4);
	Auto_Save_Variable(&lastframe,1,5);
}

void JFW_Building_Gun_No_VTOL::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Weapon_No_VTOL",params);
	}
}

void JFW_Building_Gun_No_VTOL::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_No_VTOL::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
			else
			{
				Commands->Send_Custom_Event(obj,object,x,1,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_No_VTOL::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Weapon_No_VTOL::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
}

void JFW_Building_Gun_Weapon_No_VTOL::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		if (!param)
		{
			Commands->Enable_Enemy_Seen(obj,false);
		}
		else
		{
			Commands->Enable_Enemy_Seen(obj,true);
		}
	}
}

void JFW_Building_Gun_Weapon_No_VTOL::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	float minrange;
	float maxrange;
	float range;
	if (!Is_VTOL(enemy))
	{
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange))
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,maxrange,0,true);
			Commands->Action_Attack(obj,params);
			Commands->Start_Timer(obj,this,1.0,1);
		}
	}
}

void JFW_Building_Gun_Weapon_No_VTOL::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_VTOL_Only::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Weapon_VTOL_Only",params);
	}
}

void JFW_Building_Gun_VTOL_Only::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_VTOL_Only::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
			else
			{
				Commands->Send_Custom_Event(obj,object,x,1,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_VTOL_Only::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Weapon_VTOL_Only::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
}

void JFW_Building_Gun_Weapon_VTOL_Only::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		if (!param)
		{
			Commands->Enable_Enemy_Seen(obj,false);
		}
		else
		{
			Commands->Enable_Enemy_Seen(obj,true);
		}
	}
}

void JFW_Building_Gun_Weapon_VTOL_Only::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	float minrange;
	float maxrange;
	float range;
	if (Is_VTOL(enemy))
	{
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange))
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,maxrange,0,true);
			Commands->Action_Attack(obj,params);
			Commands->Start_Timer(obj,this,1.0,1);
		}
	}
}

void JFW_Building_Gun_Weapon_VTOL_Only::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_Sound::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"),Get_Parameter("Sound"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Sound_Weapon",params);
	}
}

void JFW_Building_Gun_Animated_Sound::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated_Sound::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated_Sound::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Sound_Weapon::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	enemyseen = true;
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	float minrange;
	float maxrange;
	float range;
	minrange = Get_Float_Parameter("Min_Range");
	maxrange = Get_Float_Parameter("Max_Range");
	Vector3 pos,pos2;
	pos = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(enemy);
	range = Commands->Get_Distance(pos,pos2);
	pos.Z = 0;
	pos2.Z = 0;
	if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
	{
		enemyseen = false;
		Commands->Enable_Enemy_Seen(obj,false);
		attackid = Commands->Get_ID(enemy);
		if (!lastframe)
		{
			lastframe = true;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
			Commands->Create_Sound(Get_Parameter("Sound"),pos2,obj);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
		}
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,true);
				Commands->Action_Attack(obj,params);
			}
		}
	}
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_Sound_No_Aircraft::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("Preset1"),Get_Parameter("Preset2"),Get_Parameter("Preset3"),Get_Parameter("Preset4"),Get_Parameter("Preset5"),Get_Parameter("Preset6"),Get_Parameter("Preset7"),Get_Parameter("Preset8"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"),Get_Parameter("Sound"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft",params);
	}
}

void JFW_Building_Gun_Animated_Sound_No_Aircraft::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated_Sound_No_Aircraft::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated_Sound_No_Aircraft::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			enemyseen = false;
			Commands->Enable_Enemy_Seen(obj,false);
			attackid = Commands->Get_ID(enemy);
			if (!lastframe)
			{
				lastframe = true;
				Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
				Commands->Create_Sound(Get_Parameter("Sound"),pos2,obj);
				Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
			}
		}
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,true);
				Commands->Action_Attack(obj,params);
			}
		}
	}
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_Sound_Aircraft_Only::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("Preset1"),Get_Parameter("Preset2"),Get_Parameter("Preset3"),Get_Parameter("Preset4"),Get_Parameter("Preset5"),Get_Parameter("Preset6"),Get_Parameter("Preset7"),Get_Parameter("Preset8"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"),Get_Parameter("Sound"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only",params);
	}
}

void JFW_Building_Gun_Animated_Sound_Aircraft_Only::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated_Sound_Aircraft_Only::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated_Sound_Aircraft_Only::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	enemyseen = true;
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			enemyseen = false;
			Commands->Enable_Enemy_Seen(obj,false);
			attackid = Commands->Get_ID(enemy);
			if (!lastframe)
			{
				lastframe = true;
				Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
				Commands->Create_Sound(Get_Parameter("Sound"),pos2,obj);
				Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
			}
		}
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,true);
				Commands->Action_Attack(obj,params);
			}
		}
	}
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_Sound_Secondary::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"),Get_Parameter("Sound"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Sound_Weapon_Secondary",params);
	}
}

void JFW_Building_Gun_Animated_Sound_Secondary::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated_Sound_Secondary::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated_Sound_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Sound_Weapon_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&primary,1,1);
	Auto_Save_Variable(&effectid,4,2);
	Auto_Save_Variable(&attackid,4,3);
	Auto_Save_Variable(&enemyseen,1,4);
	Auto_Save_Variable(&lastframe,1,5);
}

void JFW_Building_Gun_Animated_Sound_Weapon::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&effectid,4,1);
	Auto_Save_Variable(&attackid,4,2);
	Auto_Save_Variable(&enemyseen,1,3);
	Auto_Save_Variable(&lastframe,1,4);
}

void JFW_Building_Gun_Animated_Sound_Weapon_Secondary::Created(GameObject *obj)
{
	primary = true;
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	enemyseen = true;
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_Secondary::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	float minrange;
	float maxrange;
	float range;
	minrange = Get_Float_Parameter("Min_Range");
	maxrange = Get_Float_Parameter("Max_Range");
	Vector3 pos,pos2;
	pos = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(enemy);
	range = Commands->Get_Distance(pos,pos2);
	pos.Z = 0;
	pos2.Z = 0;
	if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
	{
		enemyseen = false;
		Commands->Enable_Enemy_Seen(obj,false);
		attackid = Commands->Get_ID(enemy);
		if (!lastframe)
		{
			lastframe = true;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
			Commands->Create_Sound(Get_Parameter("Sound"),pos2,obj);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
		}
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,primary);
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
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_Sound_No_Aircraft_Secondary::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("Preset1"),Get_Parameter("Preset2"),Get_Parameter("Preset3"),Get_Parameter("Preset4"),Get_Parameter("Preset5"),Get_Parameter("Preset6"),Get_Parameter("Preset7"),Get_Parameter("Preset8"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"),Get_Parameter("Sound"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft_Secondary",params);
	}
}

void JFW_Building_Gun_Animated_Sound_No_Aircraft_Secondary::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated_Sound_No_Aircraft_Secondary::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated_Sound_No_Aircraft_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft_Secondary::Created(GameObject *obj)
{
	primary = true;
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	enemyseen = true;
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft_Secondary::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) && (_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			enemyseen = false;
			Commands->Enable_Enemy_Seen(obj,false);
			attackid = Commands->Get_ID(enemy);
			if (!lastframe)
			{
				lastframe = true;
				Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
				Commands->Create_Sound(Get_Parameter("Sound"),pos2,obj);
				Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
			}
		}
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,primary);
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
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_Sound_Aircraft_Only_Secondary::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("Preset1"),Get_Parameter("Preset2"),Get_Parameter("Preset3"),Get_Parameter("Preset4"),Get_Parameter("Preset5"),Get_Parameter("Preset6"),Get_Parameter("Preset7"),Get_Parameter("Preset8"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"),Get_Parameter("Sound"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only_Secondary",params);
	}
}

void JFW_Building_Gun_Animated_Sound_Aircraft_Only_Secondary::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated_Sound_Aircraft_Only_Secondary::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated_Sound_Aircraft_Only_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only_Secondary::Created(GameObject *obj)
{
	primary = true;
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	enemyseen = true;
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only_Secondary::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if ((!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset1"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset2"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset3"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset4"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset5"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset6"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset7"))) || (!_stricmp(Commands->Get_Preset_Name(enemy),Get_Parameter("Preset8"))))
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			enemyseen = false;
			Commands->Enable_Enemy_Seen(obj,false);
			attackid = Commands->Get_ID(enemy);
			if (!lastframe)
			{
				lastframe = true;
				Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
				Commands->Create_Sound(Get_Parameter("Sound"),pos2,obj);
				Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
			}
		}
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,primary);
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
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
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&effectid,4,1);
	Auto_Save_Variable(&attackid,4,2);
	Auto_Save_Variable(&enemyseen,1,3);
	Auto_Save_Variable(&lastframe,1,4);
}

void JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&effectid,4,1);
	Auto_Save_Variable(&attackid,4,2);
	Auto_Save_Variable(&enemyseen,1,3);
	Auto_Save_Variable(&lastframe,1,4);
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&primary,1,1);
	Auto_Save_Variable(&effectid,4,2);
	Auto_Save_Variable(&attackid,4,3);
	Auto_Save_Variable(&enemyseen,1,4);
	Auto_Save_Variable(&lastframe,1,5);
}

void JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&primary,1,1);
	Auto_Save_Variable(&effectid,4,2);
	Auto_Save_Variable(&attackid,4,3);
	Auto_Save_Variable(&enemyseen,1,4);
	Auto_Save_Variable(&lastframe,1,5);
}

void JFW_Building_Gun_Animated_Sound_No_VTOL::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"),Get_Parameter("Sound"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL",params);
	}
}

void JFW_Building_Gun_Animated_Sound_No_VTOL::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated_Sound_No_VTOL::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated_Sound_No_VTOL::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!Is_VTOL(enemy))
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			enemyseen = false;
			Commands->Enable_Enemy_Seen(obj,false);
			attackid = Commands->Get_ID(enemy);
			if (!lastframe)
			{
				lastframe = true;
				Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
				Commands->Create_Sound(Get_Parameter("Sound"),pos2,obj);
				Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
			}
		}
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,true);
				Commands->Action_Attack(obj,params);
			}
		}
	}
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_Sound_VTOL_Only::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"),Get_Parameter("Sound"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only",params);
	}
}

void JFW_Building_Gun_Animated_Sound_VTOL_Only::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated_Sound_VTOL_Only::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated_Sound_VTOL_Only::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	enemyseen = true;
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (Is_VTOL(enemy))
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			enemyseen = false;
			Commands->Enable_Enemy_Seen(obj,false);
			attackid = Commands->Get_ID(enemy);
			if (!lastframe)
			{
				lastframe = true;
				Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
				Commands->Create_Sound(Get_Parameter("Sound"),pos2,obj);
				Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
			}
		}
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,true);
				Commands->Action_Attack(obj,params);
			}
		}
	}
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_Sound_No_VTOL_Secondary::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"),Get_Parameter("Sound"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL_Secondary",params);
	}
}

void JFW_Building_Gun_Animated_Sound_No_VTOL_Secondary::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated_Sound_No_VTOL_Secondary::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated_Sound_No_VTOL_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL_Secondary::Created(GameObject *obj)
{
	primary = true;
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	enemyseen = true;
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL_Secondary::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!Is_VTOL(enemy))
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			enemyseen = false;
			Commands->Enable_Enemy_Seen(obj,false);
			attackid = Commands->Get_ID(enemy);
			if (!lastframe)
			{
				lastframe = true;
				Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
				Commands->Create_Sound(Get_Parameter("Sound"),pos2,obj);
				Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
			}
		}
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,primary);
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
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_Sound_VTOL_Only_Secondary::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"),Get_Parameter("Sound"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only_Secondary",params);
	}
}

void JFW_Building_Gun_Animated_Sound_VTOL_Only_Secondary::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated_Sound_VTOL_Only_Secondary::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated_Sound_VTOL_Only_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only_Secondary::Created(GameObject *obj)
{
	primary = true;
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	enemyseen = true;
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only_Secondary::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!Is_VTOL(enemy))
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			enemyseen = false;
			Commands->Enable_Enemy_Seen(obj,false);
			attackid = Commands->Get_ID(enemy);
			if (!lastframe)
			{
				lastframe = true;
				Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
				Commands->Create_Sound(Get_Parameter("Sound"),pos2,obj);
				Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
			}
		}
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,primary);
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
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&effectid,4,1);
	Auto_Save_Variable(&attackid,4,2);
	Auto_Save_Variable(&enemyseen,1,3);
	Auto_Save_Variable(&lastframe,1,4);
}

void JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&effectid,4,1);
	Auto_Save_Variable(&attackid,4,2);
	Auto_Save_Variable(&enemyseen,1,3);
	Auto_Save_Variable(&lastframe,1,4);
}

void JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&primary,1,1);
	Auto_Save_Variable(&effectid,4,2);
	Auto_Save_Variable(&attackid,4,3);
	Auto_Save_Variable(&enemyseen,1,4);
	Auto_Save_Variable(&lastframe,1,5);
}

void JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&primary,1,1);
	Auto_Save_Variable(&effectid,4,2);
	Auto_Save_Variable(&attackid,4,3);
	Auto_Save_Variable(&enemyseen,1,4);
	Auto_Save_Variable(&lastframe,1,5);
}

void JFW_Building_Gun_No_VTOL_Secondary::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Weapon_No_VTOL_Secondary",params);
	}
}

void JFW_Building_Gun_No_VTOL_Secondary::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_No_VTOL_Secondary::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
			else
			{
				Commands->Send_Custom_Event(obj,object,x,1,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_No_VTOL_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Weapon_No_VTOL_Secondary::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	primary = true;
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
}

void JFW_Building_Gun_Weapon_No_VTOL_Secondary::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		if (!param)
		{
			Commands->Enable_Enemy_Seen(obj,false);
		}
		else
		{
			Commands->Enable_Enemy_Seen(obj,true);
		}
	}
}

void JFW_Building_Gun_Weapon_No_VTOL_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	float minrange;
	float maxrange;
	float range;
	if (!Is_VTOL(enemy))
	{
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange))
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,maxrange,0,primary);
			Commands->Action_Attack(obj,params);
			Commands->Start_Timer(obj,this,1.0,1);
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

void JFW_Building_Gun_Weapon_No_VTOL_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_VTOL_Only_Secondary::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Weapon_VTOL_Only_Secondary",params);
	}
}

void JFW_Building_Gun_VTOL_Only_Secondary::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_VTOL_Only_Secondary::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
			else
			{
				Commands->Send_Custom_Event(obj,object,x,1,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_VTOL_Only_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Weapon_VTOL_Only_Secondary::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	primary = true;
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
}

void JFW_Building_Gun_Weapon_VTOL_Only_Secondary::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		if (!param)
		{
			Commands->Enable_Enemy_Seen(obj,false);
		}
		else
		{
			Commands->Enable_Enemy_Seen(obj,true);
		}
	}
}

void JFW_Building_Gun_Weapon_VTOL_Only_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	float minrange;
	float maxrange;
	float range;
	if (Is_VTOL(enemy))
	{
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange))
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,maxrange,0,primary);
			Commands->Action_Attack(obj,params);
			Commands->Start_Timer(obj,this,1.0,1);
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

void JFW_Building_Gun_Weapon_VTOL_Only_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Weapon_No_VTOL_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&primary,1,1);
}

void JFW_Building_Gun_Weapon_VTOL_Only_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&primary,1,1);
}

void JFW_Building_Gun_Animated_No_VTOL::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Weapon_No_VTOL",params);
	}
}

void JFW_Building_Gun_Animated_No_VTOL::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated_No_VTOL::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated_No_VTOL::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Weapon_No_VTOL::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Weapon_No_VTOL::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Weapon_No_VTOL::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!Is_VTOL(enemy))
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			enemyseen = false;
			Commands->Enable_Enemy_Seen(obj,false);
			attackid = Commands->Get_ID(enemy);
			if (!lastframe)
			{
				lastframe = true;
				Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
				Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
			}
		}
	}
}

void JFW_Building_Gun_Animated_Weapon_No_VTOL::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,true);
				Commands->Action_Attack(obj,params);
			}
		}
	}
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_VTOL_Only::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Weapon_VTOL_Only",params);
	}
}

void JFW_Building_Gun_Animated_VTOL_Only::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated_VTOL_Only::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated_VTOL_Only::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Weapon_VTOL_Only::Created(GameObject *obj)
{
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	enemyseen = true;
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Weapon_VTOL_Only::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Weapon_VTOL_Only::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (Is_VTOL(enemy))
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			enemyseen = false;
			Commands->Enable_Enemy_Seen(obj,false);
			attackid = Commands->Get_ID(enemy);
			if (!lastframe)
			{
				lastframe = true;
				Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
				Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
			}
		}
	}
}

void JFW_Building_Gun_Animated_Weapon_VTOL_Only::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,true);
				Commands->Action_Attack(obj,params);
			}
		}
	}
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_No_VTOL_Secondary::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Weapon_No_VTOL_Secondary",params);
	}
}

void JFW_Building_Gun_Animated_No_VTOL_Secondary::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated_No_VTOL_Secondary::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated_No_VTOL_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Weapon_No_VTOL_Secondary::Created(GameObject *obj)
{
	primary = true;
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	enemyseen = true;
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Weapon_No_VTOL_Secondary::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Weapon_No_VTOL_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!Is_VTOL(enemy))
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			enemyseen = false;
			Commands->Enable_Enemy_Seen(obj,false);
			attackid = Commands->Get_ID(enemy);
			if (!lastframe)
			{
				lastframe = true;
				Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
				Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
			}
		}
	}
}

void JFW_Building_Gun_Animated_Weapon_No_VTOL_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,primary);
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
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_VTOL_Only_Secondary::Created(GameObject *obj)
{
	Vector3 v,v2;
	GameObject *object;
	const char *weapon;
	char params[1000];
	v = Commands->Get_Position(obj);
	v2 = Get_Vector3_Parameter("Positon");
	v.X += v2.X;
	v.Y += v2.Y;
	v.Z += v2.Z;
	weapon = Get_Parameter("Weapon");
	object = Commands->Create_Object(weapon,v);
	if (object)
	{
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		sprintf(params,"%s,%s,%s,%s,%s,%s,%s,%s",Get_Parameter("Disable_Custom"),Get_Parameter("Visible"),Get_Parameter("Min_Range"),Get_Parameter("Max_Range"),Get_Parameter("EffectPreset"),Get_Parameter("EffectAnimation"),Get_Parameter("EffectTime"),Get_Parameter("EffectFrame"));
		Commands->Attach_Script(object,"JFW_Building_Gun_Animated_Weapon_VTOL_Only_Secondary",params);
	}
}

void JFW_Building_Gun_Animated_VTOL_Only_Secondary::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,x,0,0);
	}
}

void JFW_Building_Gun_Animated_VTOL_Only_Secondary::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	int x;
	x = Get_Int_Parameter("Disable_Custom");
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,x,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Building_Gun_Animated_VTOL_Only_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Building_Gun_Animated_Weapon_VTOL_Only_Secondary::Created(GameObject *obj)
{
	primary = true;
	bool visible = Get_Bool_Parameter("Visible");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Set_Is_Rendered(obj,visible);
	enemyseen = true;
	attackid = 0;
	GameObject *effect = Commands->Create_Object(Get_Parameter("EffectPreset"),Commands->Get_Position(obj));
	if (effect)
	{
		effectid = Commands->Get_ID(effect);
		lastframe = false;
		Commands->Set_Animation(effect,Get_Parameter("EffectAnimation"),false,0,0,0,false);
	}
}

void JFW_Building_Gun_Animated_Weapon_VTOL_Only_Secondary::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Disable_Custom");
	if (type == msg)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		enemyseen = false;
	}
}

void JFW_Building_Gun_Animated_Weapon_VTOL_Only_Secondary::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!Is_VTOL(enemy))
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			enemyseen = false;
			Commands->Enable_Enemy_Seen(obj,false);
			attackid = Commands->Get_ID(enemy);
			if (!lastframe)
			{
				lastframe = true;
				Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,0,Get_Float_Parameter("EffectFrame"),false);
				Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),1);
			}
		}
	}
}

void JFW_Building_Gun_Animated_Weapon_VTOL_Only_Secondary::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float minrange;
		float maxrange;
		float range;
		minrange = Get_Float_Parameter("Min_Range");
		maxrange = Get_Float_Parameter("Max_Range");
		Vector3 pos,pos2;
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(Commands->Find_Object(attackid));
		range = Commands->Get_Distance(pos,pos2);
		pos.Z = 0;
		pos2.Z = 0;
		if (lastframe)
		{
			lastframe = false;
			Commands->Set_Animation(Commands->Find_Object(effectid),Get_Parameter("EffectAnimation"),false,0,Get_Float_Parameter("EffectFrame"),0,false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("EffectTime"),2);
		}
		if ((Commands->Get_Distance(pos,pos2) > minrange) && (range < maxrange)) 
		{
			if (obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(attackid)->As_PhysicalGameObj()))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(Commands->Find_Object(attackid),maxrange,0,primary);
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
	else if (number == 2)
	{
		enemyseen = true;
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Building_Gun_Animated_Weapon_No_VTOL::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&effectid,4,1);
	Auto_Save_Variable(&attackid,4,2);
	Auto_Save_Variable(&enemyseen,1,3);
	Auto_Save_Variable(&lastframe,1,4);
}

void JFW_Building_Gun_Animated_Weapon_VTOL_Only::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&effectid,4,1);
	Auto_Save_Variable(&attackid,4,2);
	Auto_Save_Variable(&enemyseen,1,3);
	Auto_Save_Variable(&lastframe,1,4);
}

void JFW_Building_Gun_Animated_Weapon_No_VTOL_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&primary,1,1);
	Auto_Save_Variable(&effectid,4,2);
	Auto_Save_Variable(&attackid,4,3);
	Auto_Save_Variable(&enemyseen,1,4);
	Auto_Save_Variable(&lastframe,1,5);
}

void JFW_Building_Gun_Animated_Weapon_VTOL_Only_Secondary::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&primary,1,1);
	Auto_Save_Variable(&effectid,4,2);
	Auto_Save_Variable(&attackid,4,3);
	Auto_Save_Variable(&enemyseen,1,4);
	Auto_Save_Variable(&lastframe,1,5);
}

ScriptRegistrant<JFW_Building_Gun> JFW_Building_Gun_Registrant("JFW_Building_Gun","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float");
ScriptRegistrant<JFW_Building_Gun_Weapon> JFW_Building_Gun_Weapon_Registrant("JFW_Building_Gun_Weapon","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float");
ScriptRegistrant<JFW_Building_Gun_No_Aircraft> JFW_Building_Gun_No_Aircraft_Registrant("JFW_Building_Gun_No_Aircraft","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string");
ScriptRegistrant<JFW_Building_Gun_Aircraft_Only> JFW_Building_Gun_Aircraft_Only_Registrant("JFW_Building_Gun_Aircraft_Only","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string");
ScriptRegistrant<JFW_Building_Gun_Weapon_No_Aircraft> JFW_Building_Gun_Weapon_No_Aircraft_Registrant("JFW_Building_Gun_Weapon_No_Aircraft","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string");
ScriptRegistrant<JFW_Building_Gun_Weapon_Aircraft_Only> JFW_Building_Gun_Weapon_Aircraft_Only_Registrant("JFW_Building_Gun_Weapon_Aircraft_Only","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string");
ScriptRegistrant<JFW_Building_Gun_Secondary> JFW_Building_Gun_Secondary_Registrant("JFW_Building_Gun_Secondary","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float");
ScriptRegistrant<JFW_Building_Gun_Weapon_Secondary> JFW_Building_Gun_Weapon_Secondary_Registrant("JFW_Building_Gun_Weapon_Secondary","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float");
ScriptRegistrant<JFW_Building_Gun_No_Aircraft_Secondary> JFW_Building_Gun_No_Aircraft_Secondary_Registrant("JFW_Building_Gun_No_Aircraft_Secondary","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string");
ScriptRegistrant<JFW_Building_Gun_Aircraft_Only_Secondary> JFW_Building_Gun_Aircraft_Only_Secondary_Registrant("JFW_Building_Gun_Aircraft_Only_Secondary","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string");
ScriptRegistrant<JFW_Building_Gun_Weapon_No_Aircraft_Secondary> JFW_Building_Gun_Weapon_No_Aircraft_Secondary_Registrant("JFW_Building_Gun_Weapon_No_Aircraft_Secondary","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string");
ScriptRegistrant<JFW_Building_Gun_Weapon_Aircraft_Only_Secondary> JFW_Building_Gun_Weapon_Aircraft_Only_Secondary_Registrant("JFW_Building_Gun_Weapon_Aircraft_Only_Secondary","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string");
ScriptRegistrant<JFW_Building_Gun_Animated> JFW_Building_Gun_Animated_Registrant("JFW_Building_Gun_Animated","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_Animated_Weapon> JFW_Building_Gun_Animated_Weapon_Registrant("JFW_Building_Gun_Animated_Weapon","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_Animated_No_Aircraft> JFW_Building_Gun_Animated_No_Aircraft_Registrant("JFW_Building_Gun_Animated_No_Aircraft","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_Animated_Aircraft_Only> JFW_Building_Gun_Animated_Aircraft_Only_Registrant("JFW_Building_Gun_Animated_Aircraft_Only","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_Animated_Weapon_No_Aircraft> JFW_Building_Gun_Animated_Weapon_No_Aircraft_Registrant("JFW_Building_Gun_Animated_Weapon_No_Aircraft","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_Animated_Weapon_Aircraft_Only> JFW_Building_Gun_Animated_Weapon_Aircraft_Only_Registrant("JFW_Building_Gun_Animated_Weapon_Aircraft_Only","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_Animated_Secondary> JFW_Building_Gun_Animated_Secondary_Registrant("JFW_Building_Gun_Animated_Secondary","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_Animated_Weapon_Secondary> JFW_Building_Gun_Animated_Weapon_Secondary_Registrant("JFW_Building_Gun_Animated_Weapon_Secondary","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_Animated_No_Aircraft_Secondary> JFW_Building_Gun_Animated_No_Aircraft_Secondary_Registrant("JFW_Building_Gun_Animated_No_Aircraft_Secondary","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_Animated_Aircraft_Only_Secondary> JFW_Building_Gun_Animated_Aircraft_Only_Secondary_Registrant("JFW_Building_Gun_Animated_Aircraft_Only_Secondary","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_Animated_Weapon_No_Aircraft_Secondary> JFW_Building_Gun_Animated_Weapon_No_Aircraft_Secondary_Registrant("JFW_Building_Gun_Animated_Weapon_No_Aircraft_Secondary","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_Animated_Weapon_Aircraft_Only_Secondary> JFW_Building_Gun_Animated_Weapon_Aircraft_Only_Secondary_Registrant("JFW_Building_Gun_Animated_Weapon_Aircraft_Only_Secondary","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_No_VTOL> JFW_Building_Gun_No_VTOL_Registrant("JFW_Building_Gun_No_VTOL","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float");
ScriptRegistrant<JFW_Building_Gun_Weapon_No_VTOL> JFW_Building_Gun_Weapon_No_VTOL_Registrant("JFW_Building_Gun_Weapon_No_VTOL","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float");
ScriptRegistrant<JFW_Building_Gun_No_VTOL_Secondary> JFW_Building_Gun_No_VTOL_Secondary_Registrant("JFW_Building_Gun_No_VTOL_Secondary","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float");
ScriptRegistrant<JFW_Building_Gun_Weapon_No_VTOL_Secondary> JFW_Building_Gun_Weapon_No_VTOL_Secondary_Registrant("JFW_Building_Gun_Weapon_No_VTOL_Secondary","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float");
ScriptRegistrant<JFW_Building_Gun_Animated_No_VTOL> JFW_Building_Gun_Animated_No_VTOL_Registrant("JFW_Building_Gun_Animated_No_VTOL","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_Animated_Weapon_No_VTOL> JFW_Building_Gun_Animated_Weapon_No_VTOL_Registrant("JFW_Building_Gun_Animated_Weapon_No_VTOL","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_Animated_No_VTOL_Secondary> JFW_Building_Gun_Animated_No_VTOL_Secondary_Registrant("JFW_Building_Gun_Animated_No_VTOL_Secondary","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_Animated_Weapon_No_VTOL_Secondary> JFW_Building_Gun_Animated_Weapon_No_VTOL_Secondary_Registrant("JFW_Building_Gun_Animated_Weapon_No_VTOL_Secondary","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_VTOL_Only> JFW_Building_Gun_VTOL_Only_Registrant("JFW_Building_Gun_VTOL_Only","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float");
ScriptRegistrant<JFW_Building_Gun_Weapon_VTOL_Only> JFW_Building_Gun_Weapon_VTOL_Only_Registrant("JFW_Building_Gun_Weapon_VTOL_Only","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float");
ScriptRegistrant<JFW_Building_Gun_VTOL_Only_Secondary> JFW_Building_Gun_VTOL_Only_Secondary_Registrant("JFW_Building_Gun_VTOL_Only_Secondary","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float");
ScriptRegistrant<JFW_Building_Gun_Weapon_VTOL_Only_Secondary> JFW_Building_Gun_Weapon_VTOL_Only_Secondary_Registrant("JFW_Building_Gun_Weapon_VTOL_Only_Secondary","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float");
ScriptRegistrant<JFW_Building_Gun_Animated_VTOL_Only> JFW_Building_Gun_Animated_VTOL_Only_Registrant("JFW_Building_Gun_Animated_VTOL_Only","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_Animated_Weapon_VTOL_Only> JFW_Building_Gun_Animated_Weapon_VTOL_Only_Registrant("JFW_Building_Gun_Animated_Weapon_VTOL_Only","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_Animated_VTOL_Only_Secondary> JFW_Building_Gun_Animated_VTOL_Only_Secondary_Registrant("JFW_Building_Gun_Animated_VTOL_Only_Secondary","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_Animated_Weapon_VTOL_Only_Secondary> JFW_Building_Gun_Animated_Weapon_VTOL_Only_Secondary_Registrant("JFW_Building_Gun_Animated_Weapon_VTOL_Only_Secondary","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound_No_VTOL> JFW_Building_Gun_Animated_Sound_No_VTOL_Registrant("JFW_Building_Gun_Animated_Sound_No_VTOL","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL> JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL_Registrant("JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound_No_VTOL_Secondary> JFW_Building_Gun_Animated_Sound_No_VTOL_Secondary_Registrant("JFW_Building_Gun_Animated_Sound_No_VTOL_Secondary","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL_Secondary> JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL_Secondary_Registrant("JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL_Secondary","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound_VTOL_Only> JFW_Building_Gun_Animated_Sound_VTOL_Only_Registrant("JFW_Building_Gun_Animated_Sound_VTOL_Only","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only> JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only_Registrant("JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound_VTOL_Only_Secondary> JFW_Building_Gun_Animated_Sound_VTOL_Only_Secondary_Registrant("JFW_Building_Gun_Animated_Sound_VTOL_Only_Secondary","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only_Secondary> JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only_Secondary_Registrant("JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only_Secondary","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound> JFW_Building_Gun_Animated_Sound_Registrant("JFW_Building_Gun_Animated_Sound","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound_Weapon> JFW_Building_Gun_Animated_Sound_Weapon_Registrant("JFW_Building_Gun_Animated_Sound_Weapon","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound_No_Aircraft> JFW_Building_Gun_Animated_Sound_No_Aircraft_Registrant("JFW_Building_Gun_Animated_Sound_No_Aircraft","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound_Aircraft_Only> JFW_Building_Gun_Animated_Sound_Aircraft_Only_Registrant("JFW_Building_Gun_Animated_Sound_Aircraft_Only","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft> JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft_Registrant("JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only> JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only_Registrant("JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound_Secondary> JFW_Building_Gun_Animated_Sound_Secondary_Registrant("JFW_Building_Gun_Animated_Sound_Secondary","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound_Weapon_Secondary> JFW_Building_Gun_Animated_Sound_Weapon_Secondary_Registrant("JFW_Building_Gun_Animated_Sound_Weapon_Secondary","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound_No_Aircraft_Secondary> JFW_Building_Gun_Animated_Sound_No_Aircraft_Secondary_Registrant("JFW_Building_Gun_Animated_Sound_No_Aircraft_Secondary","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound_Aircraft_Only_Secondary> JFW_Building_Gun_Animated_Sound_Aircraft_Only_Secondary_Registrant("JFW_Building_Gun_Animated_Sound_Aircraft_Only_Secondary","Disable_Custom:int,Positon:vector3,Weapon:string,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft_Secondary> JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft_Secondary_Registrant("JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft_Secondary","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
ScriptRegistrant<JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only_Secondary> JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only_Secondary_Registrant("JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only_Secondary","Disable_Custom:int,Visible:int,Min_Range:float,Max_Range:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string,EffectPreset:string,EffectAnimation:string,EffectTime:float,EffectFrame:float,Sound:string");
