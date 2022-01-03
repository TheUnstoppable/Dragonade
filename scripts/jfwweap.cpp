/*  Renegade Scripts.dll
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
#include "jfwweap.h"
#include "VehicleGameObj.h"

void JFW_Nod_Turret::Created(GameObject *obj)
{
	Vector3 pos,pos1,pos2,pos3;
	GameObject *object;
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	pos = Commands->Get_Position(obj);
	pos1.X = pos.X - 10;
	pos1.Y = pos.Y - 10;
	pos1.Z = pos.Z + 2;
	pos2.X = pos.X + 10;
	pos2.Y = pos.Y;
	pos2.Z = pos.Z + 2;
	pos3.X = pos.X + 10;
	pos3.Y = pos.Y - 10;
	pos3.Z = pos.Z + 2;
	object = Commands->Create_Object("Invisible_Object",pos1);
	if (object)
		id1 = Commands->Get_ID(object);
	object = Commands->Create_Object("Invisible_Object",pos2);
	if (object)
		id2 = Commands->Get_ID(object);
	object = Commands->Create_Object("Invisible_Object",pos3);
	if (object)
		id3 = Commands->Get_ID(object);
	Commands->Start_Timer(obj,this,10,1);
}

void JFW_Nod_Turret::Killed(GameObject *obj,GameObject *killer)
{
	Vector3 position;
	float facing;
	GameObject *object;
	position = Commands->Get_Position(obj);
	facing = Commands->Get_Facing(obj);
	object = Commands->Create_Object("Nod_Turret_Destroyed",position);
	Commands->Set_Facing(object,facing);
}

void JFW_Nod_Turret::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	Vector3 v = Commands->Get_Position(enemy);
	ActionParamsStruct params;
	params.Set_Basic(this,100,2);
	v.Z += 0.5;
	params.Set_Attack(v,300.0,0.0,true);
	params.AttackCheckBlocked = false;
	Commands->Action_Attack(obj,params);
	Commands->Start_Timer(obj,this,2,2);
}

void JFW_Nod_Turret::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	if (action_id == 2)
	{
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Nod_Turret::Timer_Expired(GameObject *obj,int number)
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

void JFW_Nod_Turret::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,4,1);
	Auto_Save_Variable(&id2,4,2);
	Auto_Save_Variable(&id3,4,3);
}

void JFW_Nod_Obelisk_CnC::Created(GameObject *obj)
{
	weaponid = 0;
	Vector3 v;
	GameObject *object;
	v = Commands->Get_Position(obj);
	v.Z += 45;
	int x;
	int controllerid;
	object = Commands->Create_Object("Nod_Obelisk",v);
	if (object)
	{
		Commands->Attach_Script(object,"JFW_Obelisk_Weapon_CnC","0");
		Commands->Start_Timer(obj,this,1.0,1);
		weaponid = Commands->Get_ID(object);
		x = Commands->Get_ID(object);
		controllerid = Get_Int_Parameter("Controller_ID");
		object = Commands->Find_Object(controllerid);
		Commands->Send_Custom_Event(obj,object,9033,x,0);
	}
}

void JFW_Nod_Obelisk_CnC::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	object = Commands->Find_Object(weaponid);
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,3,0,0);
	}
}

void JFW_Nod_Obelisk_CnC::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	GameObject *object;
	if ((type == 1) && (param == 1))
	{
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			Commands->Destroy_Object(object);
		}
	}
}

void JFW_Nod_Obelisk_CnC::Timer_Expired(GameObject *obj,int number)
{
	float health;
	bool power;
	GameObject *object;
	if (number == 1)
	{
		health = Commands->Get_Health(obj);
		power = Commands->Get_Building_Power(obj);
		object = Commands->Find_Object(weaponid);
		if (object)
		{
			if ((!power) || (!health))
			{
				Commands->Send_Custom_Event(obj,object,1,0,0);
			}
			else
			{
				Commands->Send_Custom_Event(obj,object,1,1,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}

void JFW_Nod_Obelisk_CnC::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&weaponid,4,1);
}

void JFW_Obelisk_Weapon_CnC::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
	Commands->Set_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj));
}

void JFW_Obelisk_Weapon_CnC::Created(GameObject *obj)
{
	Vector3 v;
	GameObject *object;
	attack = false;
	range = 150.0f;
	effectid = 0;
	attackid = 0;
	EnemySeen = true;
	Commands->Set_Player_Type(obj,0);
	Commands->Set_Is_Rendered(obj,false);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	v = Commands->Get_Position(obj);
	object = Commands->Create_Object("Obelisk Effect",v);
	if (object)
	{
		effectid = Commands->Get_ID(object);
		Commands->Set_Animation_Frame(object,"OBL_POWERUP.OBL_POWERUP",0);
	}
}

void JFW_Obelisk_Weapon_CnC::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	Vector3 pos,pos2;
	float distance;
	GameObject *object;
	switch(type)
	{
	case 1:
	if (!param)
	{
		Commands->Enable_Enemy_Seen(obj,false);
		EnemySeen = false;
		ActionParamsStruct params;
		params.Set_Basic(this,100,0);
		params.Set_Attack(obj,0,0,true);
		Commands->Action_Attack(obj,params);
		Commands->Action_Reset(obj,100);
		if (effectid)
		{
			object = Commands->Find_Object(effectid);
			if (object)
			{
				Commands->Set_Animation_Frame(object,"OBL_POWERUP.OBL_POWERUP",0);
			}
		}
	}
	else
	{
		Commands->Enable_Enemy_Seen(obj,true);
		EnemySeen = true;
	}
	break;
	case 2:
		if (!attack && EnemySeen)
		{
			object = Commands->Find_Object(param);
			if (object)
			{
				pos = Commands->Get_Position(obj);
				pos2 = Commands->Get_Position(object);
				distance = Commands->Get_Distance(pos,pos2);
				pos.Z = 0;
				pos2.Z = 0;
				if ((Commands->Get_Distance(pos,pos2) > 15) && (range >= distance))
				{
					attackid = param;
					EnemySeen = false;
					Commands->Start_Timer(obj,this,2.0,1);
					object = Commands->Find_Object(effectid);
					if (object)
					{
						Commands->Set_Animation_Frame(object,"OBL_POWERUP.OBL_POWERUP",1);
					}
					pos2.Z -= 20;
					Commands->Create_Sound("Obelisk_Warm_Up",pos2,obj);
				}
				else
				{
					if (effectid)
					{
						object = Commands->Find_Object(effectid);
						if (object)
						{
							Commands->Set_Animation_Frame(object,"OBL_POWERUP.OBL_POWERUP",0);
						}
					}
					Commands->Action_Reset(obj,100);
				}
			}
		}
	break;
	case 3:
		attack = true;
		if (effectid)
		{
			object = Commands->Find_Object(effectid);
			if (object)
			{
				Commands->Set_Animation_Frame(object,"OBL_POWERUP.OBL_POWERUP",0);
			}
		}
		Commands->Action_Reset(obj,100);
	break;
	}
}

void JFW_Obelisk_Weapon_CnC::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	int id;
	if (!attack)
	{
		id = Commands->Get_ID(enemy);
		Commands->Send_Custom_Event(obj,obj,2,id,0);
	}
}

void JFW_Obelisk_Weapon_CnC::Timer_Expired(GameObject *obj,int number)
{
	GameObject *object;
	Vector3 pos,pos2;
	float distance;
	if (!attack)
	{
		if (number == 1)
		{
			object = Commands->Find_Object(attackid);
			if (object)
			{
				pos = Commands->Get_Position(obj);
				pos2 = Commands->Get_Position(object);
				distance = Commands->Get_Distance(pos,pos2);
				pos.Z = 0;
				pos2.Z = 0;
				if ((Commands->Get_Distance(pos,pos2) > 15) && (range >= distance))
				{
					ActionParamsStruct params;
					params.Set_Basic(this,100,0);
					params.Set_Attack(object,range,0,true);
					Commands->Action_Attack(obj,params);
					attackid = 0;
					Commands->Start_Timer(obj,this,2.0,2);
				}
				else
				{
					if (effectid)
					{
						object = Commands->Find_Object(effectid);
						if (object)
						{
							Commands->Set_Animation_Frame(object,"OBL_POWERUP.OBL_POWERUP",0);
						}
					}
					Commands->Action_Reset(obj,100);
					EnemySeen = true;
				}
			}
			else
			{
				if (effectid)
				{
					object = Commands->Find_Object(effectid);
					if (object)
					{
						Commands->Set_Animation_Frame(object,"OBL_POWERUP.OBL_POWERUP",0);
					}
				}
				Commands->Action_Reset(obj,100);
				EnemySeen = true;
			}
		}
		else
		{
			if (number == 2)
			{
				if (effectid)
				{
					object = Commands->Find_Object(effectid);
					if (object)
					{
						Commands->Set_Animation_Frame(object,"OBL_POWERUP.OBL_POWERUP",0);
					}
				}
				EnemySeen = true;
			}
		}
	}
}

void JFW_Obelisk_Weapon_CnC::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&EnemySeen,1,1);
	Auto_Save_Variable(&attackid,4,2);
	Auto_Save_Variable(&effectid,4,3);
	Auto_Save_Variable(&range,4,4);
	Auto_Save_Variable(&attack,1,5);
}

void JFW_Advanced_Guard_Tower::Created(GameObject *obj)
{
	Vector3 v,missilepos,gun1pos,gun2pos,gun3pos,gun4pos;
	GameObject *object;
	enable = true;
	Commands->Enable_Hibernation(obj,false);
	missileid = 0;
	gun1id = 0;
	gun2id = 0;
	gun3id = 0;
	gun4id = 0;
	v = Commands->Get_Position(obj);
	missilepos.X = v.X;
	missilepos.Y = v.Y;
	missilepos.Z = v.Z + 6.0f;
	gun1pos.X = v.X + 5.1789999f;
	gun1pos.Y = v.Y - 4.2389998f;
	gun1pos.Z = v.Z - 9.0f;
	gun2pos.X = v.X + 5.1609998f;
	gun2pos.Y = v.Y + 3.2720001f;
	gun2pos.Z = v.Z - 9.0f;
	gun3pos.X = v.X - 4.4910002f;
	gun3pos.Y = v.Y + 3.257f;
	gun3pos.Z = v.Z - 9.0f;
	gun4pos.X = v.X - 5.1329999f;
	gun4pos.Y = v.Y - 4.3660002f;
	gun4pos.Z = v.Z - 9.0f;
	object = Commands->Create_Object("GDI_AGT",missilepos);
	if (object)
	{
		Commands->Attach_Script(object,"JFW_Advanced_Guard_Tower_Missile","0");
		missileid = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("GDI_Ceiling_Gun_AGT",gun1pos);
	if (object)
	{
		Commands->Attach_Script(object,"JFW_Advanced_Guard_Tower_Gun","0");
		gun1id = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("GDI_Ceiling_Gun_AGT",gun2pos);
	if (object)
	{
		Commands->Attach_Script(object,"JFW_Advanced_Guard_Tower_Gun","0");
		gun2id = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("GDI_Ceiling_Gun_AGT",gun3pos);
	if (object)
	{
		Commands->Attach_Script(object,"JFW_Advanced_Guard_Tower_Gun","0");
		gun3id = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("GDI_Ceiling_Gun_AGT",gun4pos);
	if (object)
	{
		Commands->Attach_Script(object,"JFW_Advanced_Guard_Tower_Gun","0");
		gun4id = Commands->Get_ID(object);
	}
	Commands->Start_Timer(obj,this,1.0,1);
	Commands->Start_Timer(obj,this,1.0,2);
}

void JFW_Advanced_Guard_Tower::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *gun1obj,*gun2obj,*gun3obj,*gun4obj;
	gun1obj = Commands->Find_Object(gun1id);
	gun2obj = Commands->Find_Object(gun2id);
	gun3obj = Commands->Find_Object(gun3id);
	gun4obj = Commands->Find_Object(gun4id);
	if (gun1obj)
	{
		Commands->Send_Custom_Event(obj,gun1obj,3,0,0);
	}
	if (gun2obj)
	{
		Commands->Send_Custom_Event(obj,gun2obj,3,0,0);
	}
	if (gun3obj)
	{
		Commands->Send_Custom_Event(obj,gun3obj,3,0,0);
	}
	if (gun4obj)
	{
		Commands->Send_Custom_Event(obj,gun4obj,3,0,0);
	}
}

void JFW_Advanced_Guard_Tower::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	GameObject *gun1obj,*gun2obj,*gun3obj,*gun4obj;
	gun1obj = Commands->Find_Object(gun1id);
	gun2obj = Commands->Find_Object(gun2id);
	gun3obj = Commands->Find_Object(gun3id);
	gun4obj = Commands->Find_Object(gun4id);
	switch (type)
	{
	case 0:
		enable = false;
		if (gun1obj)
		{
			Commands->Send_Custom_Event(obj,gun1obj,2,0,0);
		}
		if (gun2obj)
		{
			Commands->Send_Custom_Event(obj,gun2obj,2,0,0);
		}
		if (gun3obj)
		{
			Commands->Send_Custom_Event(obj,gun3obj,2,0,0);
		}
		if (gun4obj)
		{
			Commands->Send_Custom_Event(obj,gun4obj,2,0,0);
		}
		break;
	case 1:
		enable = true;
		if (gun1obj)
		{
			Commands->Send_Custom_Event(obj,gun1obj,2,1,0);
		}
		if (gun2obj)
		{
			Commands->Send_Custom_Event(obj,gun2obj,2,1,0);
		}
		if (gun3obj)
		{
			Commands->Send_Custom_Event(obj,gun3obj,2,1,0);
		}
		if (gun4obj)
		{
			Commands->Send_Custom_Event(obj,gun4obj,2,1,0);
		}
		break;
	}
}

void JFW_Advanced_Guard_Tower::Timer_Expired(GameObject *obj,int number)
{
	GameObject *gun1obj,*gun2obj,*gun3obj,*gun4obj;
	gun1obj = Commands->Find_Object(gun1id);
	gun2obj = Commands->Find_Object(gun2id);
	gun3obj = Commands->Find_Object(gun3id);
	gun4obj = Commands->Find_Object(gun4id);
	switch (number)
	{
	case 1:
		if (gun1obj)
		{
			Commands->Send_Custom_Event(obj,gun1obj,1,missileid,0);
		}
		if (gun2obj)
		{
			Commands->Send_Custom_Event(obj,gun2obj,1,missileid,0);
		}
		if (gun3obj)
		{
			Commands->Send_Custom_Event(obj,gun3obj,1,missileid,0);
		}
		if (gun4obj)
		{
			Commands->Send_Custom_Event(obj,gun4obj,1,missileid,0);
		}
		break;
	case 2:
		if ((Commands->Get_Building_Power(obj)) && (Commands->Get_Health(obj)))
		{
			if (enable)
			{
				if (gun1obj)
				{
					Commands->Send_Custom_Event(obj,gun1obj,2,1,0);
				}
				if (gun2obj)
				{
					Commands->Send_Custom_Event(obj,gun2obj,2,1,0);
				}
				if (gun3obj)
				{
					Commands->Send_Custom_Event(obj,gun3obj,2,1,0);
				}
				if (gun4obj)
				{
					Commands->Send_Custom_Event(obj,gun4obj,2,1,0);
				}
			}
		}
		else
		{
			if (gun1obj)
			{
				Commands->Send_Custom_Event(obj,gun1obj,2,0,0);
			}
			if (gun2obj)
			{
				Commands->Send_Custom_Event(obj,gun2obj,2,0,0);
			}
			if (gun3obj)
			{
				Commands->Send_Custom_Event(obj,gun3obj,2,0,0);
			}
			if (gun4obj)
			{
				Commands->Send_Custom_Event(obj,gun4obj,2,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,2);
		break;
	}
}

void JFW_Advanced_Guard_Tower::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&missileid,4,1);
	Auto_Save_Variable(&gun1id,4,2);
	Auto_Save_Variable(&gun2id,4,3);
	Auto_Save_Variable(&gun3id,4,4);
	Auto_Save_Variable(&gun4id,4,5);
	Auto_Save_Variable(&enable,1,6);
}

void JFW_Advanced_Guard_Tower_2::Created(GameObject *obj)
{
	Vector3 v,missilepos,gun1pos,gun2pos,gun3pos,gun4pos;
	Vector3 missileofs = Get_Vector3_Parameter("MissileOffset");
	Vector3 gun1ofs = Get_Vector3_Parameter("Gun1Offset");
	Vector3 gun2ofs = Get_Vector3_Parameter("Gun2Offset");
	Vector3 gun3ofs = Get_Vector3_Parameter("Gun3Offset");
	Vector3 gun4ofs = Get_Vector3_Parameter("Gun4Offset");
	GameObject *object;
	enable = true;
	Commands->Enable_Hibernation(obj,false);
	missileid = 0;
	gun1id = 0;
	gun2id = 0;
	gun3id = 0;
	gun4id = 0;
	v = Commands->Get_Position(obj);
	missilepos.X = v.X + missileofs.X;
	missilepos.Y = v.Y + missileofs.Y;
	missilepos.Z = v.Z + missileofs.Z;
	gun1pos.X = v.X + gun1ofs.X;
	gun1pos.Y = v.Y + gun1ofs.Y;
	gun1pos.Z = v.Z + gun1ofs.Z;
	gun2pos.X = v.X + gun2ofs.X;
	gun2pos.Y = v.Y + gun2ofs.Y;
	gun2pos.Z = v.Z + gun2ofs.Z;
	gun3pos.X = v.X + gun3ofs.X;
	gun3pos.Y = v.Y + gun3ofs.Y;
	gun3pos.Z = v.Z + gun3ofs.Z;
	gun4pos.X = v.X + gun4ofs.X;
	gun4pos.Y = v.Y + gun4ofs.Y;
	gun4pos.Z = v.Z + gun4ofs.Z;
	object = Commands->Create_Object("GDI_AGT",missilepos);
	if (object)
	{
		Commands->Attach_Script(object,"JFW_Advanced_Guard_Tower_Missile","0");
		missileid = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("GDI_Ceiling_Gun_AGT",gun1pos);
	if (object)
	{
		Commands->Attach_Script(object,"JFW_Advanced_Guard_Tower_Gun","0");
		gun1id = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("GDI_Ceiling_Gun_AGT",gun2pos);
	if (object)
	{
		Commands->Attach_Script(object,"JFW_Advanced_Guard_Tower_Gun","0");
		gun2id = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("GDI_Ceiling_Gun_AGT",gun3pos);
	if (object)
	{
		Commands->Attach_Script(object,"JFW_Advanced_Guard_Tower_Gun","0");
		gun3id = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("GDI_Ceiling_Gun_AGT",gun4pos);
	if (object)
	{
		Commands->Attach_Script(object,"JFW_Advanced_Guard_Tower_Gun","0");
		gun4id = Commands->Get_ID(object);
	}
	Commands->Start_Timer(obj,this,1.0,1);
	Commands->Start_Timer(obj,this,1.0,2);
}

void JFW_Advanced_Guard_Tower_2::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *gun1obj,*gun2obj,*gun3obj,*gun4obj;
	gun1obj = Commands->Find_Object(gun1id);
	gun2obj = Commands->Find_Object(gun2id);
	gun3obj = Commands->Find_Object(gun3id);
	gun4obj = Commands->Find_Object(gun4id);
	if (gun1obj)
	{
		Commands->Send_Custom_Event(obj,gun1obj,3,0,0);
	}
	if (gun2obj)
	{
		Commands->Send_Custom_Event(obj,gun2obj,3,0,0);
	}
	if (gun3obj)
	{
		Commands->Send_Custom_Event(obj,gun3obj,3,0,0);
	}
	if (gun4obj)
	{
		Commands->Send_Custom_Event(obj,gun4obj,3,0,0);
	}
}

void JFW_Advanced_Guard_Tower_2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	GameObject *gun1obj,*gun2obj,*gun3obj,*gun4obj;
	gun1obj = Commands->Find_Object(gun1id);
	gun2obj = Commands->Find_Object(gun2id);
	gun3obj = Commands->Find_Object(gun3id);
	gun4obj = Commands->Find_Object(gun4id);
	switch (type)
	{
	case 0:
		enable = false;
		if (gun1obj)
		{
			Commands->Send_Custom_Event(obj,gun1obj,2,0,0);
		}
		if (gun2obj)
		{
			Commands->Send_Custom_Event(obj,gun2obj,2,0,0);
		}
		if (gun3obj)
		{
			Commands->Send_Custom_Event(obj,gun3obj,2,0,0);
		}
		if (gun4obj)
		{
			Commands->Send_Custom_Event(obj,gun4obj,2,0,0);
		}
		break;
	case 1:
		enable = true;
		if (gun1obj)
		{
			Commands->Send_Custom_Event(obj,gun1obj,2,1,0);
		}
		if (gun2obj)
		{
			Commands->Send_Custom_Event(obj,gun2obj,2,1,0);
		}
		if (gun3obj)
		{
			Commands->Send_Custom_Event(obj,gun3obj,2,1,0);
		}
		if (gun4obj)
		{
			Commands->Send_Custom_Event(obj,gun4obj,2,1,0);
		}
		break;
	}
}

void JFW_Advanced_Guard_Tower_2::Timer_Expired(GameObject *obj,int number)
{
	GameObject *gun1obj,*gun2obj,*gun3obj,*gun4obj;
	gun1obj = Commands->Find_Object(gun1id);
	gun2obj = Commands->Find_Object(gun2id);
	gun3obj = Commands->Find_Object(gun3id);
	gun4obj = Commands->Find_Object(gun4id);
	switch (number)
	{
	case 1:
		if (gun1obj)
		{
			Commands->Send_Custom_Event(obj,gun1obj,1,missileid,0);
		}
		if (gun2obj)
		{
			Commands->Send_Custom_Event(obj,gun2obj,1,missileid,0);
		}
		if (gun3obj)
		{
			Commands->Send_Custom_Event(obj,gun3obj,1,missileid,0);
		}
		if (gun4obj)
		{
			Commands->Send_Custom_Event(obj,gun4obj,1,missileid,0);
		}
		break;
	case 2:
		if ((Commands->Get_Building_Power(obj)) && (Commands->Get_Health(obj)))
		{
			if (enable)
			{
				if (gun1obj)
				{
					Commands->Send_Custom_Event(obj,gun1obj,2,1,0);
				}
				if (gun2obj)
				{
					Commands->Send_Custom_Event(obj,gun2obj,2,1,0);
				}
				if (gun3obj)
				{
					Commands->Send_Custom_Event(obj,gun3obj,2,1,0);
				}
				if (gun4obj)
				{
					Commands->Send_Custom_Event(obj,gun4obj,2,1,0);
				}
			}
		}
		else
		{
			if (gun1obj)
			{
				Commands->Send_Custom_Event(obj,gun1obj,2,0,0);
			}
			if (gun2obj)
			{
				Commands->Send_Custom_Event(obj,gun2obj,2,0,0);
			}
			if (gun3obj)
			{
				Commands->Send_Custom_Event(obj,gun3obj,2,0,0);
			}
			if (gun4obj)
			{
				Commands->Send_Custom_Event(obj,gun4obj,2,0,0);
			}
		}
		Commands->Start_Timer(obj,this,1.0,2);
		break;
	}
}

void JFW_Advanced_Guard_Tower_2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&missileid,4,1);
	Auto_Save_Variable(&gun1id,4,2);
	Auto_Save_Variable(&gun2id,4,3);
	Auto_Save_Variable(&gun3id,4,4);
	Auto_Save_Variable(&gun4id,4,5);
	Auto_Save_Variable(&enable,1,6);
}

void JFW_Advanced_Guard_Tower_Missile::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
	Commands->Set_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj));
}

void JFW_Advanced_Guard_Tower_Missile::Created(GameObject *obj)
{
	Commands->Set_Is_Rendered(obj,false);
	Commands->Enable_Hibernation(obj,false);
	firing = false;
}

void JFW_Advanced_Guard_Tower_Missile::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	GameObject *object;
	Vector3 pos,pos2;
	if (type == 1)
	{
		object = Commands->Find_Object(param);
		if (object)
		{
			pos = Commands->Get_Position(obj);
			pos2 = Commands->Get_Position(object);
			if ((Commands->Get_Distance(pos,pos2) > 30) && (!firing))
			{
				firing = true;
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				params.Set_Attack(object,300,0,true);
				params.AttackCheckBlocked = false;
				Commands->Action_Attack(obj,params);
				Commands->Start_Timer(obj,this,1.0,1);
			}
		}
	}
}

void JFW_Advanced_Guard_Tower_Missile::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Action_Reset(obj,100);
		firing = false;
	}
}

void JFW_Advanced_Guard_Tower_Gun::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
	Commands->Set_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj));
}

void JFW_Advanced_Guard_Tower_Gun::Created(GameObject *obj)
{
	Commands->Set_Shield_Type(obj,"Blamo");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	id = 0;
	disable = false;
}

void JFW_Advanced_Guard_Tower_Gun::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	switch (type)
	{
	case 1:
		id = param;
	break;
	case 2:
		if (!param)
		{
			Commands->Enable_Enemy_Seen(obj,false);
		}
		else
		{
			Commands->Enable_Enemy_Seen(obj,true);
		}
	break;
	case 3:
		disable = true;
		Commands->Action_Reset(obj,100);
	break;
	}
}

void JFW_Advanced_Guard_Tower_Gun::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *object;
	Vector3 pos,pos2;
	if (!disable)
	{
		object = Commands->Find_Object(id);
		if (object)
		{
			Commands->Send_Custom_Event(obj,object,1,Commands->Get_ID(enemy),0);
		}
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(enemy);
		if (Commands->Get_Distance(pos,pos2) > 20)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,300,0,true);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			Commands->Start_Timer(obj,this,10,1);
		}
	}
}

void JFW_Advanced_Guard_Tower_Gun::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Action_Reset(obj,100);
	}
}

void JFW_Advanced_Guard_Tower_Gun::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id,4,1);
	Auto_Save_Variable(&disable,1,2);
}

void JFW_Guard_Duty_Helper::Created(GameObject *obj)
{
	Commands->Set_Is_Rendered(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Guard_Duty_Helper::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	int id;
	int helpermsg;
	int seenid;
	GameObject *object;
	id = Get_Int_Parameter("ID");
	helpermsg = Get_Int_Parameter("Helper_Msg");
	object = Commands->Find_Object(id);
	seenid = Commands->Get_ID(enemy);
	Commands->Send_Custom_Event(obj,object,helpermsg,seenid,0);
}

void JFW_Guard_Duty::Created(GameObject *obj)
{
	const char *helperobj;
	const char *helperbone;
	float range;
	int dummyobj;
	int helpermsg;
	int waypathid;
	char param[100];
	GameObject *object;
	helperobj = Get_Parameter("Helper_Obj");
	helperbone = Get_Parameter("Helper_Bone");
	range = Get_Float_Parameter("Range");
	dummyobj = Get_Int_Parameter("Dummy_Object");
	helpermsg = Get_Int_Parameter("Helper_Msg");
	waypathid = Get_Int_Parameter("Waypathid");
	object = Commands->Create_Object_At_Bone(obj,helperobj,helperbone);
	Commands->Attach_To_Object_Bone(object,obj,helperbone);
	helperid = Commands->Get_ID(obj);
	sprintf(param,"%d,%d",helperid,helpermsg);
	Commands->Attach_Script(object,"JFW_Guard_Duty_Helper",param);
	ActionParamsStruct params;
	object = Commands->Find_Object(dummyobj);
	params.MoveArrivedDistance = 100;
	params.Set_Basic(this,100,777);
	params.Set_Attack(object,range,0.5,true);
	params.WaypathID = waypathid;
	params.WaypathSplined = true;
	params.AttackActive = false;
	params.AttackCheckBlocked = false;
	Commands->Action_Attack(obj,params);
}

void JFW_Guard_Duty::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	float range;
	int dummyobj;
	int waypathid;
	GameObject *object;
	range = Get_Float_Parameter("Range");
	dummyobj = Get_Int_Parameter("Dummy_Object");
	waypathid = Get_Int_Parameter("Waypathid");
	ActionParamsStruct params;
	if (action_id == 777)
	{
		object = Commands->Find_Object(dummyobj);
		params.MoveArrivedDistance = 100;
		params.Set_Basic(this,100,777);
		params.Set_Attack(object,range,0.5,true);
		params.WaypathID = waypathid;
		params.WaypathSplined = true;
		params.AttackActive = false;
		params.AttackCheckBlocked = false;
		Commands->Action_Attack(obj,params);
	}
}

void JFW_Guard_Duty::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	float range;
	int helpermsg;
	int waypathid;
	float attacktime;
	GameObject *object;
	range = Get_Float_Parameter("Range");
	helpermsg = Get_Int_Parameter("Helper_Msg");
	waypathid = Get_Int_Parameter("Waypathid");
	attacktime = Get_Float_Parameter("Attack_Time");
	if (type == helpermsg)
	{
		object = Commands->Find_Object(param);
		ActionParamsStruct params;
		params.Set_Basic(this,100,777);
		params.MoveArrivedDistance = 100;
		params.Set_Movement(0,0.1f,range);
		params.Set_Attack(object,range,0.5,true);
		params.WaypathID = waypathid;
		params.WaypathSplined = true;
		params.AttackActive = true;
		params.AttackCheckBlocked = false;
		Commands->Modify_Action(obj,777,params,true,true);
		Commands->Start_Timer(obj,this,attacktime,1);
		object = Commands->Find_Object(helperid);
		Commands->Enable_Enemy_Seen(object,false);
	}
}

void JFW_Guard_Duty::Timer_Expired(GameObject *obj,int number)
{
	int dummyobj;
	float range;
	int waypathid;
	float waittime;
	ActionParamsStruct params;
	GameObject *object;
	range = Get_Float_Parameter("Range");
	dummyobj = Get_Int_Parameter("Dummy_Object");
	waypathid = Get_Int_Parameter("Waypathid");
	waittime = Get_Float_Parameter("Wait_Time");
	switch (number)
	{
	case 1:
		object = Commands->Find_Object(dummyobj);
		params.MoveArrivedDistance = 100;
		params.Set_Basic(this,100,777);
		params.Set_Attack(object,range,0.5,true);
		params.WaypathID = waypathid;
		params.WaypathSplined = true;
		params.AttackActive = false;
		params.AttackCheckBlocked = false;
		Commands->Modify_Action(obj,777,params,true,true);
		Commands->Start_Timer(obj,this,waittime,2);
	break;
	case 2:
		object = Commands->Find_Object(helperid);
		Commands->Enable_Enemy_Seen(object,true);
	break;
	}
}

void JFW_Guard_Duty::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&helperid,4,1);
}

void JFW_Hunt_Attack::Created(GameObject *obj)
{
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Enable_Hibernation(obj,false);
}

void JFW_Hunt_Attack::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	float range;
	range = Get_Float_Parameter("Attack_Range");
	ActionParamsStruct params;
	params.MoveArrivedDistance = 100;
	params.Set_Basic(this,100,1);
	params.Set_Attack(enemy,range,0,true);
	Commands->Action_Attack(obj,params);
}

void JFW_Hunt_Attack::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	Commands->Action_Reset(obj,100);
}

void JFW_Hunt_Blow_Up::Created(GameObject *obj)
{
	firsttime = true;
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Enable_Hibernation(obj,false);
}

void JFW_Hunt_Blow_Up::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	float speed;
	float distance;
	speed = Get_Float_Parameter("Speed");
	distance = Get_Float_Parameter("Distance");
	ActionParamsStruct params;
	params.MoveArrivedDistance = 100;
	params.Set_Basic(this,100,777);
	params.Set_Movement(enemy,speed,distance);
	params.MoveFollow = true;
	Commands->Action_Goto(obj,params);
}

void JFW_Hunt_Blow_Up::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	Vector3 v;
	if ((action_id == 777) && (!firsttime))
	{
		Commands->Action_Reset(obj,100);
		v = Commands->Get_Position(obj);
		Commands->Create_Explosion(Get_Parameter("Explosion"),v,0);
	}
	else
	{
		firsttime = false;
	}
}

void JFW_Hunt_Blow_Up::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&firsttime,4,1);
}

void JFW_Clear_Weapons::Entered(GameObject *obj,GameObject *enterer)
{
	Commands->Clear_Weapons(enterer);
}

void JFW_Weapon_Change_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Commands->Clear_Weapons(obj);
		Commands->Give_PowerUp(obj,Get_Parameter("Powerup"),false);
		Commands->Select_Weapon(obj,Get_Parameter("Weapon"));
	}
}

void JFW_Weapon_Change_On_Custom_Sender::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Commands->Clear_Weapons(sender);
		Commands->Give_PowerUp(sender,Get_Parameter("Powerup"),false);
		Commands->Select_Weapon(sender,Get_Parameter("Weapon"));
	}
}

void JFW_Weapon_Change_On_Custom_Multiple::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Commands->Clear_Weapons(obj);
		const char *powerup;
		powerup = Get_Parameter("Powerup1");
		if (!_stricmp(powerup,"0"))
		{
			powerup = 0;
		}
		Commands->Give_PowerUp(obj,powerup,false);
		powerup = Get_Parameter("Powerup2");
		if (!_stricmp(powerup,"0"))
		{
			powerup = 0;
		}
		Commands->Give_PowerUp(obj,powerup,false);
		powerup = Get_Parameter("Powerup3");
		if (!_stricmp(powerup,"0"))
		{
			powerup = 0;
		}
		Commands->Give_PowerUp(obj,powerup,false);
		powerup = Get_Parameter("Powerup4");
		if (!_stricmp(powerup,"0"))
		{
			powerup = 0;
		}
		Commands->Give_PowerUp(obj,powerup,false);
		powerup = Get_Parameter("Powerup5");
		if (!_stricmp(powerup,"0"))
		{
			powerup = 0;
		}
		Commands->Give_PowerUp(obj,powerup,false);
		Commands->Select_Weapon(obj,Get_Parameter("Weapon"));
	}
}

void JFW_Weapon_Change_On_Custom_Multiple_Sender::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Commands->Clear_Weapons(sender);
		const char *powerup;
		powerup = Get_Parameter("Powerup1");
		if (!_stricmp(powerup,"0"))
		{
			powerup = 0;
		}
		Commands->Give_PowerUp(sender,powerup,false);
		powerup = Get_Parameter("Powerup2");
		if (!_stricmp(powerup,"0"))
		{
			powerup = 0;
		}
		Commands->Give_PowerUp(sender,powerup,false);
		powerup = Get_Parameter("Powerup3");
		if (!_stricmp(powerup,"0"))
		{
			powerup = 0;
		}
		Commands->Give_PowerUp(sender,powerup,false);
		powerup = Get_Parameter("Powerup4");
		if (!_stricmp(powerup,"0"))
		{
			powerup = 0;
		}
		Commands->Give_PowerUp(sender,powerup,false);
		powerup = Get_Parameter("Powerup5");
		if (!_stricmp(powerup,"0"))
		{
			powerup = 0;
		}
		Commands->Give_PowerUp(sender,powerup,false);
		Commands->Select_Weapon(sender,Get_Parameter("Weapon"));
	}
}

void JFW_Clear_Weapons_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Commands->Clear_Weapons(obj);
	}
}

void JFW_Zone_Weapon::Entered(GameObject *obj,GameObject *enterer)
{
	const char *preset;
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	preset = Get_Parameter("Weapon_Name");
	Commands->Give_PowerUp(enterer,preset,true);
}

void JFW_Vehicle_Weapon_Preset::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if ((!_stricmp(Commands->Get_Preset_Name(sender),Get_Parameter("Preset"))) && (type == CUSTOM_EVENT_VEHICLE_ENTERED))
	{
		Commands->Clear_Weapons(obj);
		Commands->Give_PowerUp(obj,Get_Parameter("Powerup"),false);
		Commands->Select_Weapon(obj,Get_Parameter("Weapon"));
	}
}

void JFW_Vehicle_Weapon_Weapon::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if ((Has_Weapon(sender,Get_Parameter("CharWeapon"))) && (type == CUSTOM_EVENT_VEHICLE_ENTERED))
	{
		Commands->Clear_Weapons(obj);
		Commands->Give_PowerUp(obj,Get_Parameter("Powerup"),false);
		Commands->Select_Weapon(obj,Get_Parameter("Weapon"));
	}
}

void JFW_Clear_Weapons_Create::Created(GameObject *obj)
{
	Commands->Clear_Weapons(obj);
}

void JFW_Fire_Weapon_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Custom"))
	{
		Commands->Trigger_Weapon(obj,true,Get_Vector3_Parameter("Target"),true);
	}
}

void JFW_Vehicle_Weapon_Switcher::Created(GameObject *obj)
{
	switching = false;
	newweap = false;
	pilotID = 0;
	new_weapon = 0;
	old_weapon = 0;
	Commands->Give_PowerUp(obj,Get_Parameter("Powerup"),false);
	new_weapon = Get_Powerup_Weapon(Get_Parameter("PowerUp"));
	hookid = 0;
	k = 0;
}

void JFW_Vehicle_Weapon_Switcher::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!pilotID)
		{
			InstallHook(Get_Parameter("Keyhook"),sender);
			pilotID = Commands->Get_ID(sender);
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (pilotID == Commands->Get_ID(sender))
		{
			RemoveHook();
			pilotID = 0;
		}
	}
}

void JFW_Vehicle_Weapon_Switcher::Killed(GameObject *obj,GameObject *killer)
{
	RemoveHook();
}

void JFW_Vehicle_Weapon_Switcher::Timer_Expired(GameObject *obj,int number)
{
	switching = false;
	obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(true);
	if (newweap)
	{
		SAFE_DELETE_ARRAY(old_weapon);
		old_weapon = newstr(Get_Current_Weapon(obj));
		Commands->Select_Weapon(obj,new_weapon);
	}
	else
	{
		Commands->Select_Weapon(obj,old_weapon);
		SAFE_DELETE_ARRAY(old_weapon);
		old_weapon = 0;
	}
}

void JFW_Vehicle_Weapon_Switcher::KeyHook()
{
	if (switching)
	{
		return;
	}
	VehicleGameObj *obj = Owner()->As_VehicleGameObj();
	obj->Set_Scripts_Can_Fire(false);
	switching = true;
	if (newweap)
	{
		newweap = false;
		Commands->Create_Sound(Get_Parameter("OldSound"),Commands->Get_Position(obj),obj);
	}
	else
	{
		newweap = true;
		Commands->Create_Sound(Get_Parameter("NewSound"),Commands->Get_Position(obj),obj);
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("SwitchTime"),1);
}

void JFW_Char_Weapon_Switcher::Created(GameObject *obj)
{
	LastSwitch = 0;
	Weapon2 = true;
	InTimer = false;
	InstallHook(Get_Parameter("Keyhook"),obj);
}

void JFW_Char_Weapon_Switcher::KeyHook()
{
	if (Get_Vehicle(Owner()))
	{
		return;
	}
	if (time(NULL) - LastSwitch < Get_Float_Parameter("SwitchTime"))
	{
		StringClass str;
		str.Format("You cannot switch weapons yet, you must wait at least %f seconds after switching to switch again.",Get_Float_Parameter("SwitchTime"));
		Send_Message_Player(Owner(),153,204,25,str);
		return;
	}
	if (InTimer)
	{
		return;
	}
	LastSwitch = time(NULL);
	if (Weapon2)
	{
		Remove_Weapon(Owner(),Get_Powerup_Weapon(Get_Parameter("WeaponPowerup1")));
	}
	else
	{
		Remove_Weapon(Owner(),Get_Powerup_Weapon(Get_Parameter("WeaponPowerup2")));
	}
	InTimer = true;
	Commands->Start_Timer(Owner(),this,Get_Float_Parameter("SwitchTime"),1);
}

void JFW_Char_Weapon_Switcher::Timer_Expired(GameObject *obj,int number)
{
	if (Weapon2)
	{
		Commands->Give_PowerUp(obj,Get_Parameter("WeaponPowerup2"),false);
		Commands->Select_Weapon(obj,Get_Powerup_Weapon(Get_Parameter("WeaponPowerup2")));
	}
	else
	{
		Commands->Give_PowerUp(obj,Get_Parameter("WeaponPowerup1"),false);
		Commands->Select_Weapon(obj,Get_Powerup_Weapon(Get_Parameter("WeaponPowerup1")));
	}
	Weapon2 = !Weapon2;
	InTimer = false;
}

ScriptRegistrant<JFW_Nod_Turret> JFW_Nod_Turret_Registrant("JFW_Nod_Turret","");
ScriptRegistrant<JFW_Nod_Obelisk_CnC> JFW_Nod_Obelisk_CnC_Registrant("JFW_Nod_Obelisk_CnC","Controller_ID=0:int");
ScriptRegistrant<JFW_Obelisk_Weapon_CnC> JFW_Obelisk_Weapon_CnC_Registrant("JFW_Obelisk_Weapon_CnC","");
ScriptRegistrant<JFW_Advanced_Guard_Tower> JFW_Advanced_Guard_Tower_Registrant("JFW_Advanced_Guard_Tower","");
ScriptRegistrant<JFW_Advanced_Guard_Tower_2> JFW_Advanced_Guard_Tower_2_Registrant("JFW_Advanced_Guard_Tower_2","Gun1Offset:vector3,Gun2Offset:vector3,Gun3Offset:vector3,Gun4Offset:vector3,MissileOffset:vector3");
ScriptRegistrant<JFW_Advanced_Guard_Tower_Missile> JFW_Advanced_Guard_Tower_Missile_Registrant("JFW_Advanced_Guard_Tower_Missile","");
ScriptRegistrant<JFW_Advanced_Guard_Tower_Gun> JFW_Advanced_Guard_Tower_Gun_Registrant("JFW_Advanced_Guard_Tower_Gun","");
ScriptRegistrant<JFW_Guard_Duty_Helper> JFW_Guard_Duty_Helper_Registrant("JFW_Guard_Duty_Helper","ID:int,Helper_Msg:int");
ScriptRegistrant<JFW_Guard_Duty> JFW_Guard_Duty_Registrant("JFW_Guard_Duty","Helper_Obj:string,Helper_Bone:string,Range:float,Dummy_Object:int,Helper_Msg:int,Waypathid:int,Attack_Time:float,Wait_Time:float");
ScriptRegistrant<JFW_Hunt_Attack> JFW_Hunt_Attack_Registrant("JFW_Hunt_Attack","Attack_Range:float");
ScriptRegistrant<JFW_Hunt_Blow_Up> JFW_Hunt_Blow_Up_Registrant("JFW_Hunt_Blow_Up","Speed:float,Distance:float,Explosion:string");
ScriptRegistrant<JFW_Clear_Weapons> JFW_Clear_Weapons_Registrant("JFW_Clear_Weapons","");
ScriptRegistrant<JFW_Zone_Weapon> JFW_Zone_Weapon_Registrant("JFW_Zone_Weapon","Weapon_Name:string,Player_Type:int");
ScriptRegistrant<JFW_Weapon_Change_On_Custom> JFW_Weapon_Change_On_Custom_Registrant("JFW_Weapon_Change_On_Custom","Message:int,Powerup:string,Weapon:string");
ScriptRegistrant<JFW_Weapon_Change_On_Custom_Sender> JFW_Weapon_Change_On_Custom_Sender_Registrant("JFW_Weapon_Change_On_Custom_Sender","Message:int,Powerup:string,Weapon:string");
ScriptRegistrant<JFW_Clear_Weapons_On_Custom> JFW_Clear_Weapons_On_Custom_Registrant("JFW_Clear_Weapons_On_Custom","Message:int,Powerup:string,Weapon:string");
ScriptRegistrant<JFW_Vehicle_Weapon_Preset> JFW_Vehicle_Weapon_Preset_Registrant("JFW_Vehicle_Weapon_Preset","Powerup:string,Weapon:string,Preset:string");
ScriptRegistrant<JFW_Vehicle_Weapon_Weapon> JFW_Vehicle_Weapon_Weapon_Registrant("JFW_Vehicle_Weapon_Weapon","Powerup:string,Weapon:string,CharWeapon:string");
ScriptRegistrant<JFW_Clear_Weapons_Create> JFW_Clear_Weapons_Create_Registrant("JFW_Clear_Weapons_Create","");
ScriptRegistrant<JFW_Weapon_Change_On_Custom_Multiple> JFW_Weapon_Change_On_Custom_Multiple_Registrant("JFW_Weapon_Change_On_Custom_Multiple","Message:int,Powerup1:string,Powerup2:string,Powerup3:string,Powerup4:string,Powerup5:string,Weapon:string");
ScriptRegistrant<JFW_Weapon_Change_On_Custom_Multiple_Sender> JFW_Weapon_Change_On_Custom_Multiple_Sender_Registrant("JFW_Weapon_Change_On_Custom_Multiple_Sender","Message:int,Powerup1:string,Powerup2:string,Powerup3:string,Powerup4:string,Powerup5:string,Weapon:string");
ScriptRegistrant<JFW_Fire_Weapon_Custom> JFW_Fire_Weapon_Custom_Registrant("JFW_Fire_Weapon_Custom","Target:vector3,Custom:int");
ScriptRegistrant<JFW_Vehicle_Weapon_Switcher> JFW_Vehicle_Weapon_Switcher_Registrant("JFW_Vehicle_Weapon_Switcher","Powerup:string,Keyhook=VDeploy:string,SwitchTime:float,OldSound:string,NewSound:string");
ScriptRegistrant<JFW_Char_Weapon_Switcher> JFW_Char_Weapon_Switcher_Registrant("JFW_Char_Weapon_Switcher","SwitchTime:float,WeaponPowerup1:string,WeaponPowerup2:string,Keyhook:string");
