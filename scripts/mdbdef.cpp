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
#include "mdbdef.h"
#include "SmartGameObj.h"
#include "VehicleGameObjDef.h"

void MDB_Base_Defense_Popup_No_VTOL::Created(GameObject *obj)
{
	State = 1;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	obj->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	LastAttack = The_Game()->Get_Game_Duration_S();
	LastSeen = 0;
	MaxDis = Get_Float_Parameter("MaxAttackDistance");
	ReturnTime = Get_Int_Parameter("ReturnTime");
	AdjustInf = Get_Bool_Parameter("Adjust_Aim_For_Infantry");
	Commands->Start_Timer(obj,this,3.0f,2);
}

void MDB_Base_Defense_Popup_No_VTOL::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 9813199)
	{
		GameObject *o = Get_Vehicle(sender);
		if (o)
		{
			sender = o;
		}
		if (!Is_VTOL(sender))
		{
			if (Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(sender)) <= MaxDis)
			{
				if (State == 1)
				{
					State = 2;
					Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
					obj->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
					if (_stricmp(Get_Parameter("Sound"),"0"))
					{
						Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(obj),obj);
					}
					LastSeen = Commands->Get_ID(sender);
					LastAttack = The_Game()->Get_Game_Duration_S();
				}
			}
		}
	}
}

void MDB_Base_Defense_Popup_No_VTOL::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!Is_VTOL(enemy) && State == 3)
	{
		if (Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(enemy)) <= MaxDis)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			if (AdjustInf && enemy->As_SoldierGameObj())
			{
				params.Set_Attack(Commands->Get_Bone_Position(enemy,"c R Foot"),MaxDis,0.0,true);
				params.AttackCheckBlocked = true;
			}
			else
			{
				params.Set_Attack(enemy,MaxDis,0.0,true);
				params.AttackCheckBlocked = true;
			}
			LastSeen = Commands->Get_ID(enemy);
			Commands->Start_Timer(obj,this,5.0f,3);
			Commands->Action_Attack(obj,params);
			LastAttack = The_Game()->Get_Game_Duration_S();
			Commands->Start_Timer(obj,this,(float)ReturnTime,1);
		}
		else
		{
			Commands->Start_Timer(obj,this,(float)ReturnTime,1);
		}
	}
}

void MDB_Base_Defense_Popup_No_VTOL::Animation_Complete(GameObject *obj,const char *animation_name)
{
	if (State == 2)
	{
		State = 3;
		Enemy_Seen(obj,Commands->Find_Object(LastSeen));
	}
	else if (State == 4)
	{
		State = 1;
	}
}

void MDB_Base_Defense_Popup_No_VTOL::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (State == 3)
		{
			if ((int)(The_Game()->Get_Game_Duration_S() - LastAttack)+1 >= Get_Int_Parameter("ReturnTime"))
			{
				Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
				obj->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
				if (_stricmp(Get_Parameter("Sound"),"0"))
				{
					Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(obj),obj);
				}
				Commands->Action_Reset(obj,100);
				State = 4;
			}
		}
	}
	else if (number == 2)
	{
		char params[40];
		sprintf(params,"9813199,%d",Commands->Get_ID(obj));
		Commands->Attach_Script(Commands->Create_Object(Get_Parameter("Sensor_Preset"),Commands->Get_Bone_Position(obj,Get_Parameter("Sensor_Creation_Bone"))),"MDB_Send_Custom_Enemy_Seen",params);
	}
	else if (number == 3)
	{
		if (!Commands->Find_Object(LastSeen) || !obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(LastSeen)->As_PhysicalGameObj()))
		{
			Commands->Action_Reset(obj,100);
		}
	}
}

void MDB_Base_Defense_Popup_No_VTOL::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&State,4,1);
	Auto_Save_Variable(&LastSeen,4,2);
	Auto_Save_Variable(&LastAttack,4,3);
	Auto_Save_Variable(&ReturnTime,4,4);
	Auto_Save_Variable(&MaxDis,4,5);
	Auto_Save_Variable(&AdjustInf,1,6);
}

void MDB_Base_Defense_Popup_No_VTOL_No_Stealth::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!Is_VTOL(enemy) && State == 3)
	{
		SmartGameObj* obj1 = enemy->As_SmartGameObj();
		if(!obj1 
		|| !obj1->Is_Stealthed() 
		|| Commands->Get_Distance(Commands->Get_Position(obj), Commands->Get_Position(enemy)) <= Get_Float_Parameter("MaxStealthAttackDistance"))
		{
			if (Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(enemy)) <= MaxDis)
			{
				ActionParamsStruct params;
				params.Set_Basic(this,100,1);
				if (AdjustInf && enemy->As_SoldierGameObj())
				{
					params.Set_Attack(Commands->Get_Bone_Position(enemy,"c R Foot"),MaxDis,0.0,true);
					params.AttackCheckBlocked = true;
				}
				else
				{
					params.Set_Attack(enemy,MaxDis,0.0,true);
					params.AttackCheckBlocked = true;
				}
				LastSeen = Commands->Get_ID(enemy);
				Commands->Start_Timer(obj,this,5.0f,3);
				Commands->Action_Attack(obj,params);
				LastAttack = The_Game()->Get_Game_Duration_S();
				Commands->Start_Timer(obj,this,(float)ReturnTime,1);
			}	
			else
			{
				Commands->Start_Timer(obj,this,(float)ReturnTime,1);
			}
		}
	}
}

void MDB_Base_Defense_Popup_VTOL_Only::Created(GameObject *obj)
{
	State = 1;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	obj->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	LastAttack = The_Game()->Get_Game_Duration_S();
	LastSeen = 0;
	MaxDis = Get_Float_Parameter("MaxAttackDistance");
	ReturnTime = Get_Int_Parameter("ReturnTime");
	Commands->Start_Timer(obj,this,3.0f,2);
}

void MDB_Base_Defense_Popup_VTOL_Only::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 9813199)
	{
		GameObject *o = Get_Vehicle(sender);
		if (o)
		{
			sender = o;
		}
		if (Is_VTOL(sender))
		{
			if (Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(sender)) <= MaxDis)
			{
				if (State == 1)
				{
					State = 2;
					Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
					obj->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
					if (_stricmp(Get_Parameter("Sound"),"0"))
					{
						Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(obj),obj);
					}
					LastSeen = Commands->Get_ID(sender);
					LastAttack = The_Game()->Get_Game_Duration_S();
				}
			}
		}
	}
}

void MDB_Base_Defense_Popup_VTOL_Only::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (Is_VTOL(enemy) && State == 3)
	{
		if (Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(enemy)) <= MaxDis)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.Set_Attack(enemy,MaxDis,0.0,true);
			params.AttackCheckBlocked = true;
			Commands->Action_Attack(obj,params);
			LastAttack = The_Game()->Get_Game_Duration_S();
			Commands->Start_Timer(obj,this,(float)ReturnTime,1);
			LastSeen = Commands->Get_ID(enemy);
			Commands->Start_Timer(obj,this,5.0f,3);
		}
		else
		{
			Commands->Start_Timer(obj,this,(float)ReturnTime,1);
		}
	}
}

void MDB_Base_Defense_Popup_VTOL_Only::Animation_Complete(GameObject *obj,const char *animation_name)
{
	if (State == 2)
	{
		State = 3;
		Enemy_Seen(obj,Commands->Find_Object(LastSeen));
	}
	else if (State == 4)
	{
		State = 1;
	}
}

void MDB_Base_Defense_Popup_VTOL_Only::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (State == 3)
		{
			if ((int)(The_Game()->Get_Game_Duration_S() - LastAttack)+1 >= Get_Int_Parameter("ReturnTime"))
			{
				Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
				obj->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
				if (_stricmp(Get_Parameter("Sound"),"0"))
				{
					Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(obj),obj);
				}
				Commands->Action_Reset(obj,100);
				State = 4;
			}
		}
	}
	else if (number == 2)
	{
		char params[40];
		sprintf(params,"9813199,%d",Commands->Get_ID(obj));
		Commands->Attach_Script(Commands->Create_Object(Get_Parameter("Sensor_Preset"),Commands->Get_Bone_Position(obj,Get_Parameter("Sensor_Creation_Bone"))),"MDB_Send_Custom_Enemy_Seen",params);
	}
	else if (number == 3)
	{
		if (!Commands->Find_Object(LastSeen) || !obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(LastSeen)->As_PhysicalGameObj()))
		{
			Commands->Action_Reset(obj,100);
		}
	}
}

void MDB_Base_Defense_Popup_VTOL_Only::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&State,4,1);
	Auto_Save_Variable(&LastSeen,4,2);
	Auto_Save_Variable(&LastAttack,4,3);
	Auto_Save_Variable(&ReturnTime,4,4);
	Auto_Save_Variable(&MaxDis,4,5);
}

void MDB_Base_Defense_Popup::Created(GameObject *obj)
{
	State = 1;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,0,false);
	obj->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	LastAttack = The_Game()->Get_Game_Duration_S();
	LastSeen = 0;
	MaxDis = Get_Float_Parameter("MaxAttackDistance");
	ReturnTime = Get_Int_Parameter("ReturnTime");
	AdjustInf = Get_Bool_Parameter("Adjust_Aim_For_Infantry");
	Commands->Start_Timer(obj,this,3.0f,2);
}

void MDB_Base_Defense_Popup::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 9813199)
	{
		GameObject *o = Get_Vehicle(sender);
		if (o)
		{
			sender = o;
		}
		if (Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(sender)) <= MaxDis)
		{
			if (State == 1)
			{
				State = 2;
				Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,Get_Float_Parameter("LastFrame"),false);
				obj->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
				if (_stricmp(Get_Parameter("Sound"),"0"))
				{
					Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(obj),obj);
				}
				LastSeen = Commands->Get_ID(sender);
				LastAttack = The_Game()->Get_Game_Duration_S();
			}
		}
	}
}

void MDB_Base_Defense_Popup::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (State == 3)
	{
		if (Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(enemy)) <= MaxDis)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			if (AdjustInf && enemy->As_SoldierGameObj())
			{
				params.Set_Attack(Commands->Get_Bone_Position(enemy,"c R Foot"),MaxDis,0.0,true);
				params.AttackCheckBlocked = true;
			}
			else
			{
				params.Set_Attack(enemy,MaxDis,0.0,true);
				params.AttackCheckBlocked = true;
			}
			LastSeen = Commands->Get_ID(enemy);
			Commands->Start_Timer(obj,this,5.0f,3);
			Commands->Action_Attack(obj,params);
			LastAttack = The_Game()->Get_Game_Duration_S();
			Commands->Start_Timer(obj,this,(float)ReturnTime,1);
		}
		else
		{
			Commands->Start_Timer(obj,this,(float)ReturnTime,1);
		}
	}
}

void MDB_Base_Defense_Popup::Animation_Complete(GameObject *obj,const char *animation_name)
{
	if (State == 2)
	{
		State = 3;
		Enemy_Seen(obj,Commands->Find_Object(LastSeen));
	}
	else if (State == 4)
	{
		State = 1;
	}
}

void MDB_Base_Defense_Popup::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (State == 3)
		{
			if ((int)(The_Game()->Get_Game_Duration_S() - LastAttack)+1 >= Get_Int_Parameter("ReturnTime"))
			{
				Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0,false);
				obj->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
				if (_stricmp(Get_Parameter("Sound"),"0"))
				{
					Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(obj),obj);
				}
				Commands->Action_Reset(obj,100);
				State = 4;
			}
		}
	}
	else if (number == 2)
	{
		char params[40];
		sprintf(params,"9813199,%d",Commands->Get_ID(obj));
		Commands->Attach_Script(Commands->Create_Object(Get_Parameter("Sensor_Preset"),Commands->Get_Bone_Position(obj,Get_Parameter("Sensor_Creation_Bone"))),"MDB_Send_Custom_Enemy_Seen",params);
	}
	else if (number == 3)
	{
		if (!Commands->Find_Object(LastSeen) || !obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(LastSeen)->As_PhysicalGameObj()))
		{
			Commands->Action_Reset(obj,100);
		}
	}
}

void MDB_Base_Defense_Popup::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&State,4,1);
	Auto_Save_Variable(&LastSeen,4,2);
	Auto_Save_Variable(&LastAttack,4,3);
	Auto_Save_Variable(&ReturnTime,4,4);
	Auto_Save_Variable(&MaxDis,4,5);
	Auto_Save_Variable(&AdjustInf,1,6);
}

void MDB_Base_Defense::Created(GameObject *obj)
{
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Vector3 pos = Commands->Get_Position(obj);
	V[0].X = pos.X - 10;
	V[0].Y = pos.Y;
	V[0].Z = pos.Z + 2;
	V[1].X = pos.X + 10;
	V[1].Y = pos.Y;
	V[1].Z = pos.Z + 2;
	V[2].X = pos.X;
	V[2].Y = pos.Y - 10;
	V[2].Z = pos.Z + 2;
	V[3].X = pos.X;
	V[3].Y = pos.Y + 10;
	V[3].Z = pos.Z + 2;
	ActionParamsStruct var;
	var.Set_Basic(this,1,2);
	var.Set_Attack(V[Commands->Get_Random_Int(0,4)],0.0,0.0,true);
	MaxDis = Get_Float_Parameter("MaxAttackDistance");
	LastSeen = 0;
	AdjustInf = Get_Bool_Parameter("Adjust_Aim_For_Infantry");
	Commands->Start_Timer(obj,this,Commands->Get_Random(5.0f,15.0f),1);
}

void MDB_Base_Defense::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	ActionParamsStruct params;
	params.Set_Basic(this,100,1);
	if (AdjustInf && enemy->As_SoldierGameObj())
	{
		params.Set_Attack(Commands->Get_Bone_Position(enemy,"c R Foot"),MaxDis,0.0,true);
		params.AttackCheckBlocked = true;
	}
	else
	{
		params.Set_Attack(enemy,MaxDis,0.0,true);
		params.AttackCheckBlocked = true;
	}
	LastSeen = Commands->Get_ID(enemy);
	Commands->Start_Timer(obj,this,1.0f,2);
	Commands->Action_Attack(obj,params);
}

void MDB_Base_Defense::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		ActionParamsStruct var;
		var.Set_Basic(this,1,2);
		var.Set_Attack(V[Commands->Get_Random_Int(0,4)],0.0,0.0,true);
		Commands->Action_Attack(obj,var);
		Commands->Start_Timer(obj,this,Commands->Get_Random(5.0f,15.0f),1);
	}
	else if (number == 2)
	{
		if (!Commands->Find_Object(LastSeen) || !obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(LastSeen)->As_PhysicalGameObj()))
		{
			Commands->Action_Reset(obj,100);
		}
	}
}

void MDB_Base_Defense::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&MaxDis,4,1);
	Auto_Save_Variable(&AdjustInf,1,2);
	Auto_Save_Variable(&LastSeen,4,3);
	Auto_Save_Variable(&V[0].X,4,4);
	Auto_Save_Variable(&V[0].Y,4,5);
	Auto_Save_Variable(&V[0].Z,4,6);
	Auto_Save_Variable(&V[1].X,4,7);
	Auto_Save_Variable(&V[1].Y,4,8);
	Auto_Save_Variable(&V[1].Z,4,9);
	Auto_Save_Variable(&V[2].X,4,10);
	Auto_Save_Variable(&V[2].Y,4,11);
	Auto_Save_Variable(&V[2].Z,4,12);
	Auto_Save_Variable(&V[3].X,4,13);
	Auto_Save_Variable(&V[3].Y,4,14);
	Auto_Save_Variable(&V[3].Z,4,15);
}

void MDB_Base_Defense_No_VTOL::Created(GameObject *obj)
{
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Vector3 pos = Commands->Get_Position(obj);
	V[0].X = pos.X - 10;
	V[0].Y = pos.Y;
	V[0].Z = pos.Z + 2;
	V[1].X = pos.X + 10;
	V[1].Y = pos.Y;
	V[1].Z = pos.Z + 2;
	V[2].X = pos.X;
	V[2].Y = pos.Y - 10;
	V[2].Z = pos.Z + 2;
	V[3].X = pos.X;
	V[3].Y = pos.Y + 10;
	V[3].Z = pos.Z + 2;
	ActionParamsStruct var;
	var.Set_Basic(this,1,2);
	var.Set_Attack(V[Commands->Get_Random_Int(0,4)],0.0,0.0,true);
	MaxDis = Get_Float_Parameter("MaxAttackDistance");
	LastSeen = 0;
	AdjustInf = Get_Bool_Parameter("Adjust_Aim_For_Infantry");
	Commands->Start_Timer(obj,this,Commands->Get_Random(5.0f,15.0f),1);
}

void MDB_Base_Defense_No_VTOL::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (!Is_VTOL(enemy))
	{
		ActionParamsStruct params;
		params.Set_Basic(this,100,1);
		if (AdjustInf && enemy->As_SoldierGameObj())
		{
			params.Set_Attack(Commands->Get_Bone_Position(enemy,"c R Foot"),MaxDis,0.0,true);
			params.AttackCheckBlocked = true;
		}
		else
		{
			params.Set_Attack(enemy,MaxDis,0.0,true);
			params.AttackCheckBlocked = true;
		}
		LastSeen = Commands->Get_ID(enemy);
		Commands->Start_Timer(obj,this,1.0f,2);
		Commands->Action_Attack(obj,params);
	}
}

void MDB_Base_Defense_No_VTOL::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		ActionParamsStruct var;
		var.Set_Basic(this,1,2);
		var.Set_Attack(V[Commands->Get_Random_Int(0,4)],0.0,0.0,true);
		Commands->Action_Attack(obj,var);
		Commands->Start_Timer(obj,this,Commands->Get_Random(5.0f,15.0f),1);
	}
	else if (number == 2)
	{
		if (!Commands->Find_Object(LastSeen) || !obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(LastSeen)->As_PhysicalGameObj()))
		{
			Commands->Action_Reset(obj,100);
		}
	}
}

void MDB_Base_Defense_No_VTOL::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&MaxDis,4,1);
	Auto_Save_Variable(&AdjustInf,1,2);
	Auto_Save_Variable(&LastSeen,4,3);
	Auto_Save_Variable(&V[0].X,4,4);
	Auto_Save_Variable(&V[0].Y,4,5);
	Auto_Save_Variable(&V[0].Z,4,6);
	Auto_Save_Variable(&V[1].X,4,7);
	Auto_Save_Variable(&V[1].Y,4,8);
	Auto_Save_Variable(&V[1].Z,4,9);
	Auto_Save_Variable(&V[2].X,4,10);
	Auto_Save_Variable(&V[2].Y,4,11);
	Auto_Save_Variable(&V[2].Z,4,12);
	Auto_Save_Variable(&V[3].X,4,13);
	Auto_Save_Variable(&V[3].Y,4,14);
	Auto_Save_Variable(&V[3].Z,4,15);
}

void MDB_Base_Defense_VTOL_Only::Created(GameObject *obj)
{
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Vector3 pos = Commands->Get_Position(obj);
	V[0].X = pos.X - 10;
	V[0].Y = pos.Y;
	V[0].Z = pos.Z + 5;
	V[1].X = pos.X + 10;
	V[1].Y = pos.Y;
	V[1].Z = pos.Z + 5;
	V[2].X = pos.X;
	V[2].Y = pos.Y - 10;
	V[2].Z = pos.Z + 5;
	V[3].X = pos.X;
	V[3].Y = pos.Y + 10;
	V[3].Z = pos.Z + 5;
	ActionParamsStruct var;
	var.Set_Basic(this,1,2);
	var.Set_Attack(V[Commands->Get_Random_Int(0,4)],0.0,0.0,true);
	MaxDis = Get_Float_Parameter("MaxAttackDistance");
	Commands->Start_Timer(obj,this,Commands->Get_Random(5.0f,15.0f),1);
}

void MDB_Base_Defense_VTOL_Only::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *o = Get_Vehicle(enemy);
	if (o)
	{
		enemy = o;
	}
	if (Is_VTOL(enemy))
	{
		ActionParamsStruct params;
		params.Set_Basic(this,100,1);
		params.Set_Attack(enemy,MaxDis,0.0,true);
		params.AttackCheckBlocked = true;
		Commands->Action_Attack(obj,params);
		LastSeen = Commands->Get_ID(enemy);
		Commands->Start_Timer(obj,this,1.0f,2);
	}
}

void MDB_Base_Defense_VTOL_Only::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		ActionParamsStruct var;
		var.Set_Basic(this,1,2);
		var.Set_Attack(V[Commands->Get_Random_Int(0,4)],0.0,0.0,true);
		Commands->Action_Attack(obj,var);
		Commands->Start_Timer(obj,this,Commands->Get_Random(5.0f,15.0f),1);
	}
	else if (number == 2)
	{
		if (!Commands->Find_Object(LastSeen) || !obj->As_SmartGameObj()->Is_Obj_Visible(Commands->Find_Object(LastSeen)->As_PhysicalGameObj()))
		{
			Commands->Action_Reset(obj,100);
		}
	}
}

void MDB_Base_Defense_VTOL_Only::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&MaxDis,4,1);
	Auto_Save_Variable(&V[0].X,4,2);
	Auto_Save_Variable(&V[0].Y,4,3);
	Auto_Save_Variable(&V[0].Z,4,4);
	Auto_Save_Variable(&V[1].X,4,5);
	Auto_Save_Variable(&V[1].Y,4,6);
	Auto_Save_Variable(&V[1].Z,4,7);
	Auto_Save_Variable(&V[2].X,4,8);
	Auto_Save_Variable(&V[2].Y,4,9);
	Auto_Save_Variable(&V[2].Z,4,10);
	Auto_Save_Variable(&V[3].X,4,11);
	Auto_Save_Variable(&V[3].Y,4,12);
	Auto_Save_Variable(&V[3].Z,4,13);
}

ScriptRegistrant<MDB_Base_Defense_Popup_No_VTOL> MDB_Base_Defense_Popup_No_VTOL_Registrant("MDB_Base_Defense_Popup_No_VTOL","MaxAttackDistance=10.0:float,ReturnTime=10:int,Animation=0:string,LastFrame=-1.0:float,Sensor_Preset=0:string,Sensor_Creation_Bone=0:string,Sound=0:string,Adjust_Aim_For_Infantry=1:int");
ScriptRegistrant<MDB_Base_Defense_Popup_No_VTOL_No_Stealth> MDB_Base_Defense_Popup_No_VTOL_No_Stealth_Registrant("MDB_Base_Defense_Popup_No_VTOL_No_Stealth","MaxAttackDistance=10.0:float,ReturnTime=10:int,Animation=0:string,LastFrame=-1.0:float,Sensor_Preset=0:string,Sensor_Creation_Bone=0:string,Sound=0:string,Adjust_Aim_For_Infantry=1:int,MaxStealthAttackDistance=0.0:float");
ScriptRegistrant<MDB_Base_Defense_Popup_VTOL_Only> MDB_Base_Defense_Popup_VTOL_Only_Registrant("MDB_Base_Defense_Popup_VTOL_Only","MaxAttackDistance=10.0:float,ReturnTime=10:int,Animation=0:string,LastFrame=-1.0:float,Sensor_Preset=0:string,Sensor_Creation_Bone=0:string,Sound=0:string");
ScriptRegistrant<MDB_Base_Defense_Popup> MDB_Base_Defense_Popup_Registrant("MDB_Base_Defense_Popup","MaxAttackDistance=10.0:float,ReturnTime=10:int,Animation=0:string,LastFrame=-1.0:float,Sensor_Preset=0:string,Sensor_Creation_Bone=0:string,Sound=0:string,Adjust_Aim_For_Infantry=1:int");
ScriptRegistrant<MDB_Base_Defense> MDB_Base_Defense_Registrant("MDB_Base_Defense","MaxAttackDistance=10.0:float,Adjust_Aim_For_Infantry=1:int");
ScriptRegistrant<MDB_Base_Defense_No_VTOL> MDB_Base_Defense_No_VTOL_Registrant("MDB_Base_Defense_No_VTOL","MaxAttackDistance=10.0:float,Adjust_Aim_For_Infantry=1:int");
ScriptRegistrant<MDB_Base_Defense_VTOL_Only> MDB_Base_Defense_VTOL_Only_Registrant("MDB_Base_Defense_VTOL_Only","MaxAttackDistance=10.0:float,Adjust_Aim_For_Infantry=1:int");
