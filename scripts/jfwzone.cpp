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
#include "jfwzone.h"
#include "TeamPurchaseSettingsDefClass.h"
#include "PurchaseSettingsDefClass.h"
#include "ScriptableGameObj.h"
#include "ScriptZoneGameObj.h"
#include "ConstructionYardGameObj.h"
#include "VehicleGameObjDef.h"
#include "BuildingGameObjDef.h"
#include "dp88_custom_timer_defines.h"
#include "PhysicalGameObj.h"
#include "VehicleGameObj.h"

void JFW_3D_Sound_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int team;
	const char *c;
	Vector3 v;
	team = Get_Int_Parameter("Player_Type");
	c = Get_Parameter("Sound");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	v = Commands->Get_Position(enterer);
	Commands->Create_Sound(c,v,obj);
}

void JFW_Bounce_Zone_Entry::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	float amount;
	Vector3 pos;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	amount = Get_Float_Parameter("Amount");
	pos = Commands->Get_Position(enterer);
	pos.Z += amount;
	Commands->Set_Position(enterer,pos);
}

void JFW_Grant_Key_Zone_Entry::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	int key;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	key = Get_Int_Parameter("Key");
	Commands->Grant_Key(enterer,key,true);
}

void JFW_Zone_Send_Custom_Enable::Entered(GameObject *obj,GameObject *enterer)
{
	GameObject *object;
	int type;
	int ID;
	int param;
	int pltype;
	pltype = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,pltype))
	{
		return;
	}
	ID = Get_Int_Parameter("ID");
	type = Get_Int_Parameter("EnterMessage");
	param = Get_Int_Parameter("EnterParam");
	object = Commands->Find_Object(ID);
	if (enabled)
	{
		if (type)
		{
			if (!param)
			{
				param = Commands->Get_ID(enterer);
			}
			Commands->Send_Custom_Event(obj,object,type,param,0);
		}
	}
}

void JFW_Zone_Send_Custom_Enable::Exited(GameObject *obj,GameObject *exiter)
{
	GameObject *object;
	int type;
	int ID;
	int param;
	int pltype;
	pltype = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(exiter,pltype))
	{
		return;
	}
	ID = Get_Int_Parameter("ID");
	type = Get_Int_Parameter("ExitMessage");
	param = Get_Int_Parameter("ExitParam");
	object = Commands->Find_Object(ID);
	if (enabled)
	{
		if (type)
		{
			if (!param)
			{
				param = Commands->Get_ID(exiter);
			}
			Commands->Send_Custom_Event(obj,object,type,param,0);
		}
	}
}

void JFW_Zone_Send_Custom_Preset::Entered(GameObject *obj,GameObject *enterer)
{
	GameObject *object;
	int type;
	int ID;
	int param;
	const char *preset;
	const char *c;
	int pltype;
	pltype = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,pltype))
	{
		return;
	}
	ID = Get_Int_Parameter("ID");
	type = Get_Int_Parameter("EnterMessage");
	param = Get_Int_Parameter("EnterParam");
	object = Commands->Find_Object(ID);
	c = Get_Parameter("Preset_Name");
	preset = Commands->Get_Preset_Name(enterer);
	if (!_stricmp(preset,c))
	{
		if (type)
		{
			if (!param)
			{
				param = Commands->Get_ID(enterer);
			}
			Commands->Send_Custom_Event(obj,object,type,param,0);
		}
	}
}

void JFW_Zone_Send_Custom_Preset::Exited(GameObject *obj,GameObject *exiter)
{
	GameObject *object;
	int type;
	int ID;
	int param;
	const char *preset;
	const char *c;
	int pltype;
	pltype = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(exiter,pltype))
	{
		return;
	}
	ID = Get_Int_Parameter("ID");
	type = Get_Int_Parameter("ExitMessage");
	param = Get_Int_Parameter("ExitParam");
	object = Commands->Find_Object(ID);
	c = Get_Parameter("Preset_Name");
	preset = Commands->Get_Preset_Name(exiter);
	if (!_stricmp(preset,c))
	{
		if (type)
		{
			if (!param)
			{
				param = Commands->Get_ID(exiter);
			}
			Commands->Send_Custom_Event(obj,object,type,param,0);
		}
	}
}

void JFW_Zone_Send_Custom_Enable::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg,msg2;
	msg = Get_Int_Parameter("Enable_Message");
	msg2 = Get_Int_Parameter("Disable_Message");
	if (msg == type)
	{
		enabled = true;
	}
	if (msg2 == type)
	{
		enabled = false;
	}
}

void JFW_Zone_Send_Custom_Enable::Created(GameObject *obj)
{
	enabled = false;
}

void JFW_Zone_Send_Custom_Enable::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Zone_Send_Custom_Enable_Enter::Entered(GameObject *obj,GameObject *enterer)
{
	GameObject *object;
	int type;
	int param;
	int pltype;
	pltype = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,pltype))
	{
		return;
	}
	type = Get_Int_Parameter("EnterMessage");
	param = Get_Int_Parameter("EnterParam");
	object = enterer;
	if (enabled)
	{
		Commands->Send_Custom_Event(obj,object,type,param,0);
	}
}

void JFW_Zone_Send_Custom_Enable_Enter::Exited(GameObject *obj,GameObject *exiter)
{
	GameObject *object;
	int type;
	int param;
	int pltype;
	pltype = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(exiter,pltype))
	{
		return;
	}
	type = Get_Int_Parameter("ExitMessage");
	param = Get_Int_Parameter("ExitParam");
	object = exiter;
	if (enabled)
	{
		Commands->Send_Custom_Event(obj,object,type,param,0);
	}
}

void JFW_Zone_Send_Custom_Preset_Enter::Entered(GameObject *obj,GameObject *enterer)
{
	GameObject *object;
	int type;
	int param;
	const char *preset;
	const char *c;
	int pltype;
	pltype = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,pltype))
	{
		return;
	}
	type = Get_Int_Parameter("EnterMessage");
	param = Get_Int_Parameter("EnterParam");
	object = enterer;
	c = Get_Parameter("Preset_Name");
	preset = Commands->Get_Preset_Name(enterer);
	if (!_stricmp(preset,c))
	{
		Commands->Send_Custom_Event(obj,object,type,param,0);
	}
}

void JFW_Zone_Send_Custom_Preset_Enter::Exited(GameObject *obj,GameObject *exiter)
{
	GameObject *object;
	int type;
	int param;
	const char *preset;
	const char *c;
	int pltype;
	pltype = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(exiter,pltype))
	{
		return;
	}
	type = Get_Int_Parameter("ExitMessage");
	param = Get_Int_Parameter("ExitParam");
	object = exiter;
	c = Get_Parameter("Preset_Name");
	preset = Commands->Get_Preset_Name(exiter);
	if (!_stricmp(preset,c))
	{
		Commands->Send_Custom_Event(obj,object,type,param,0);
	}
}

void JFW_Zone_Send_Custom_Enable_Enter::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg,msg2;
	msg = Get_Int_Parameter("Enable_Message");
	msg2 = Get_Int_Parameter("Disable_Message");
	if (msg == type)
	{
		enabled = true;
	}
	if (msg2 == type)
	{
		enabled = false;
	}
}

void JFW_Zone_Send_Custom_Enable_Enter::Created(GameObject *obj)
{
	enabled = false;
}

void JFW_Zone_Send_Custom_Enable_Enter::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Send_Custom_Zone_Enter::Entered(GameObject *obj,GameObject *enterer)
{
	if (CheckPlayerType(enterer,Get_Int_Parameter("Team_ID")))
		return;
	GameObject *object = enterer;
	Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("EnterMessage"),Get_Int_Parameter("EnterParam"),0);
}

void JFW_Send_Custom_Zone_Enter::Exited(GameObject *obj,GameObject *exiter)
{
	if (CheckPlayerType(exiter,Get_Int_Parameter("Team_ID")))
		return;
	GameObject *object = exiter;
	Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("ExitMessage"),Get_Int_Parameter("ExitParam"),0);
}

void JFW_Gate_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int Player_Type = Get_Int_Parameter("Player_Type");
	int ID = Get_Int_Parameter("ID");
	const char *Animation = Get_Parameter("Animation");
	int TimerNum = Get_Int_Parameter("TimerNum");
	float Time = Get_Float_Parameter("Time");
	if (CheckPlayerType(enterer,Player_Type))
	{
		return;
	}
	Commands->Static_Anim_Phys_Goto_Last_Frame(ID,Animation);
	Commands->Start_Timer(obj,this,Time,TimerNum);
}

void JFW_Gate_Zone::Timer_Expired(GameObject *obj,int number)
{
	int ID = Get_Int_Parameter("ID");
	const char *Animation = Get_Parameter("Animation");
	int TimerNum = Get_Int_Parameter("TimerNum");
	if (TimerNum == number)
	{
		Commands->Static_Anim_Phys_Goto_Frame(ID,0.0,Animation);
	}
}

void JFW_Repair_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int Player_Type = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,Player_Type))
	{
		return;
	}
	if (enterer->As_VehicleGameObj() && enterer->As_VehicleGameObj()->Get_Definition().Can_Repair())
	{
		Commands->Start_Timer(obj,this,1.0,Commands->Get_ID(enterer));
	}
}

void JFW_Repair_Zone::Timer_Expired(GameObject *obj,int number)
{
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= Get_Float_Parameter("Repair_Distance"))
	{
		float Max_Health = Commands->Get_Max_Health(Commands->Find_Object(number));
		float Health = Commands->Get_Health(Commands->Find_Object(number));
		float Max_Shield_Strength = Commands->Get_Max_Shield_Strength(Commands->Find_Object(number));
		float Shield_Strength = Commands->Get_Shield_Strength(Commands->Find_Object(number));
		bool repair = false;
		if (Health < Max_Health)
		{
			Commands->Set_Health(Commands->Find_Object(number),Health + Get_Float_Parameter("Repair_Health"));
			repair = true;
		}
		if (Shield_Strength < Max_Shield_Strength)
		{
			Commands->Set_Shield_Strength(Commands->Find_Object(number),Shield_Strength + Get_Float_Parameter("Repair_Health"));
			repair = true;
		}
		if (repair && Commands->Find_Object(number)->As_VehicleGameObj())
		{
			Commands->Find_Object(number)->As_VehicleGameObj()->Damage_Meshes_Update(); //repair was done, update damage meshes
		}
		Commands->Start_Timer(obj,this,1.0,number);
	}
}

void JFW_Repair_Zone_Aircraft_Only::Entered(GameObject *obj,GameObject *enterer)
{
	int Player_Type = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,Player_Type))
	{
		return;
	}
	if ((!_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Preset1"))) || (!_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Preset2"))) || (!_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Preset3"))) || (!_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Preset4"))) || (!_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Preset5"))) || (!_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Preset6"))) || (!_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Preset7"))) || (!_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Preset8"))))
	{
		if (enterer->As_VehicleGameObj() && enterer->As_VehicleGameObj()->Get_Definition().Can_Repair())
		{
			Commands->Start_Timer(obj,this,1.0,Commands->Get_ID(enterer));
		}
	}
}

void JFW_Repair_Zone_Aircraft_Only::Timer_Expired(GameObject *obj,int number)
{
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= Get_Float_Parameter("Repair_Distance"))
	{
		float Max_Health = Commands->Get_Max_Health(Commands->Find_Object(number));
		float Health = Commands->Get_Health(Commands->Find_Object(number));
		float Max_Shield_Strength = Commands->Get_Max_Shield_Strength(Commands->Find_Object(number));
		float Shield_Strength = Commands->Get_Shield_Strength(Commands->Find_Object(number));
		bool repair = false;
		if (Health < Max_Health)
		{
			Commands->Set_Health(Commands->Find_Object(number),Health + Get_Float_Parameter("Repair_Health"));
			repair = true;
		}
		if (Shield_Strength < Max_Shield_Strength)
		{
			Commands->Set_Shield_Strength(Commands->Find_Object(number),Shield_Strength + Get_Float_Parameter("Repair_Health"));
			repair = true;
		}
		if (repair && Commands->Find_Object(number)->As_VehicleGameObj())
		{
			Commands->Find_Object(number)->As_VehicleGameObj()->Damage_Meshes_Update(); //repair was done, update damage meshes
		}
		Commands->Start_Timer(obj,this,1.0,number);
	}
}

void JFW_Repair_Zone_No_Aircraft::Entered(GameObject *obj,GameObject *enterer)
{
	int Player_Type = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,Player_Type))
	{
		return;
	}
	if ((_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Preset1"))) && (_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Preset2"))) && (_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Preset3"))) && (_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Preset4"))) && (_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Preset5"))) && (_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Preset6"))) && (_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Preset7"))) && (_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Preset8"))))
	{
		if (enterer->As_VehicleGameObj() && enterer->As_VehicleGameObj()->Get_Definition().Can_Repair())
		{
			Commands->Start_Timer(obj,this,1.0,Commands->Get_ID(enterer));
		}
	}
}

void JFW_Repair_Zone_No_Aircraft::Timer_Expired(GameObject *obj,int number)
{
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= Get_Float_Parameter("Repair_Distance"))
	{
		float Max_Health = Commands->Get_Max_Health(Commands->Find_Object(number));
		float Health = Commands->Get_Health(Commands->Find_Object(number));
		float Max_Shield_Strength = Commands->Get_Max_Shield_Strength(Commands->Find_Object(number));
		float Shield_Strength = Commands->Get_Shield_Strength(Commands->Find_Object(number));
		bool repair = false;
		if (Health < Max_Health)
		{
			Commands->Set_Health(Commands->Find_Object(number),Health + Get_Float_Parameter("Repair_Health"));
			repair = true;
		}
		if (Shield_Strength < Max_Shield_Strength)
		{
			Commands->Set_Shield_Strength(Commands->Find_Object(number),Shield_Strength + Get_Float_Parameter("Repair_Health"));
			repair = true;
		}
		if (repair && Commands->Find_Object(number)->As_VehicleGameObj())
		{
			Commands->Find_Object(number)->As_VehicleGameObj()->Damage_Meshes_Update(); //repair was done, update damage meshes
		}
		Commands->Start_Timer(obj,this,1.0,number);
	}
}

void JFW_Zone_Send_Custom_Not_Preset::Entered(GameObject *obj,GameObject *enterer)
{
	GameObject *object;
	int type;
	int ID;
	int param;
	const char *preset;
	const char *c;
	int pltype;
	pltype = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,pltype))
	{
		return;
	}
	ID = Get_Int_Parameter("ID");
	type = Get_Int_Parameter("EnterMessage");
	param = Get_Int_Parameter("EnterParam");
	object = Commands->Find_Object(ID);
	c = Get_Parameter("Preset_Name");
	preset = Commands->Get_Preset_Name(enterer);
	if (_stricmp(preset,c))
	{
		if (type)
		{
			if (!param)
			{
				param = Commands->Get_ID(enterer);
			}
			Commands->Send_Custom_Event(obj,object,type,param,0);
		}
	}
}

void JFW_Zone_Send_Custom_Not_Preset::Exited(GameObject *obj,GameObject *exiter)
{
	GameObject *object;
	int type;
	int ID;
	int param;
	const char *preset;
	const char *c;
	int pltype;
	pltype = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(exiter,pltype))
	{
		return;
	}
	ID = Get_Int_Parameter("ID");
	type = Get_Int_Parameter("ExitMessage");
	param = Get_Int_Parameter("ExitParam");
	object = Commands->Find_Object(ID);
	c = Get_Parameter("Preset_Name");
	preset = Commands->Get_Preset_Name(exiter);
	if (_stricmp(preset,c))
	{
		if (type)
		{
			if (!param)
			{
				param = Commands->Get_ID(exiter);
			}
			Commands->Send_Custom_Event(obj,object,type,param,0);
		}
	}
}

void JFW_Zone_Send_Custom_Keycard::Entered(GameObject *obj,GameObject *enterer)
{
	GameObject *object;
	int type;
	int ID;
	int param;
	int pltype;
	int key;
	pltype = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,pltype))
	{
		return;
	}
	ID = Get_Int_Parameter("ID");
	type = Get_Int_Parameter("EnterMessage");
	param = Get_Int_Parameter("EnterParam");
	object = Commands->Find_Object(ID);
	key = Get_Int_Parameter("Keycard_Number");
	if (Commands->Has_Key(enterer,key))
	{
		if (type)
		{
			if (!param)
			{
				param = Commands->Get_ID(enterer);
			}
			Commands->Send_Custom_Event(obj,object,type,param,0);
		}
	}
}

void JFW_Zone_Send_Custom_Keycard::Exited(GameObject *obj,GameObject *exiter)
{
	GameObject *object;
	int type;
	int ID;
	int param;
	int pltype;
	int key;
	pltype = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(exiter,pltype))
	{
		return;
	}
	ID = Get_Int_Parameter("ID");
	type = Get_Int_Parameter("ExitMessage");
	param = Get_Int_Parameter("ExitParam");
	object = Commands->Find_Object(ID);
	key = Get_Int_Parameter("Keycard_Number");
	if (Commands->Has_Key(exiter,key))
	{
		if (type)
		{
			if (!param)
			{
				param = Commands->Get_ID(exiter);
			}
			Commands->Send_Custom_Event(obj,object,type,param,0);
		}
	}
}

void JFW_Heal_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int Player_Type = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,Player_Type))
	{
		return;
	}
	if (enterer->As_SoldierGameObj())
	{
		Commands->Start_Timer(obj,this,1.0,Commands->Get_ID(enterer));
	}
}

void JFW_Heal_Zone::Timer_Expired(GameObject *obj,int number)
{
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= Get_Float_Parameter("Heal_Distance"))
	{
		float Max_Health = Commands->Get_Max_Health(Commands->Find_Object(number));
		float Health = Commands->Get_Health(Commands->Find_Object(number));
		float Max_Shield_Strength = Commands->Get_Max_Shield_Strength(Commands->Find_Object(number));
		float Shield_Strength = Commands->Get_Shield_Strength(Commands->Find_Object(number));
		if (Health < Max_Health)
		{
			Commands->Set_Health(Commands->Find_Object(number),Health + Get_Float_Parameter("Heal_Health"));
		}
		if (Shield_Strength < Max_Shield_Strength)
		{
			Commands->Set_Shield_Strength(Commands->Find_Object(number),Shield_Strength + Get_Float_Parameter("Heal_Health"));
		}
		Commands->Start_Timer(obj,this,1.0,number);
	}
}

void JFW_Heal_Zone_2::Entered(GameObject *obj,GameObject *enterer)
{
	int Player_Type = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,Player_Type))
	{
		return;
	}
	if (!_stricmp(Commands->Get_Preset_Name(obj),Get_Parameter("Preset_Name")))
	{
		Commands->Start_Timer(obj,this,1.0,Commands->Get_ID(enterer));
	}
}

void JFW_Heal_Zone_2::Timer_Expired(GameObject *obj,int number)
{
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= Get_Float_Parameter("Heal_Distance"))
	{
		float Max_Health = Commands->Get_Max_Health(Commands->Find_Object(number));
		float Health = Commands->Get_Health(Commands->Find_Object(number));
		float Max_Shield_Strength = Commands->Get_Max_Shield_Strength(Commands->Find_Object(number));
		float Shield_Strength = Commands->Get_Shield_Strength(Commands->Find_Object(number));
		if (Health < Max_Health)
		{
			Commands->Set_Health(Commands->Find_Object(number),Health + Get_Float_Parameter("Heal_Health"));
		}
		if (Shield_Strength < Max_Shield_Strength)
		{
			Commands->Set_Shield_Strength(Commands->Find_Object(number),Shield_Strength + Get_Float_Parameter("Heal_Health"));
		}
		Commands->Start_Timer(obj,this,1.0,number);
	}
}

void JFW_Disable_Transitions_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (enterer->As_VehicleGameObj())
	{
		Commands->Enable_Vehicle_Transitions(enterer,false);
	}
}

void JFW_Disable_Transitions_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (exiter->As_VehicleGameObj())
	{
		Commands->Enable_Vehicle_Transitions(exiter,true);
	}
}

void JFW_Bounce_Zone_Entry_All_Directions::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	Vector3 pos;
	float angle;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	pos = Commands->Get_Position(enterer);
	pos.Z += Get_Float_Parameter("ZAmount");
	angle = DEG_TO_RADF(Commands->Get_Facing(obj));
	pos.X += Get_Float_Parameter("Amount")*(float)cos(angle);
	pos.Y += Get_Float_Parameter("Amount")*(float)sin(angle);
	Commands->Set_Position(enterer,pos);
}

void JFW_Zone_Send_Custom_Cost::Entered(GameObject *obj,GameObject *enterer)
{
	GameObject *object;
	int type;
	int ID;
	int param;
	int pltype;
	int cost;
	pltype = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,pltype))
	{
		return;
	}
	ID = Get_Int_Parameter("ID");
	type = Get_Int_Parameter("Message");
	param = Get_Int_Parameter("Param");
	object = Commands->Find_Object(ID);
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(enterer))
	{
		cost = -cost;
		Commands->Give_Money(enterer,(float)cost,0);
		if (!param)
		{
			param = Commands->Get_ID(enterer);
		}
		Commands->Send_Custom_Event(obj,object,type,param,0);
	}
}

void JFW_Vechicle_Damage_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (enterer->As_VehicleGameObj())
	{
		int Player_Type = Get_Int_Parameter("Player_Type");
		if (CheckPlayerType(enterer,Player_Type))
		{
			return;
		}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Commands->Get_ID(enterer));
	}
}

void JFW_Vechicle_Damage_Zone::Timer_Expired(GameObject *obj,int number)
{
	float f;
	const char *w;
	float distance;
	Vector3 pos1;
	Vector3 pos2;
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= Get_Float_Parameter("Distance"))
	{
		w = Get_Parameter("Warhead");
		f = Get_Float_Parameter("Damage");
		Commands->Apply_Damage(Commands->Find_Object(number),f,w,0);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),number);
	}
}

void JFW_Weapon_Buy::Entered(GameObject *obj,GameObject *enterer)
{
	const char *preset;
	int x;
	int cost;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	preset = Get_Parameter("Weapon_Name");
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(enterer))
	{
		cost = -cost;
		Commands->Give_Money(enterer,(float)cost,0);
		Commands->Give_PowerUp(enterer,preset,true);
	}
}

void JFW_Preset_Buy::Entered(GameObject *obj,GameObject *enterer)
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
	preset = Get_Parameter("Preset_Name");
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(enterer))
	{
		cost = -cost;
		spawn_position = Get_Vector3_Parameter("location");
		Commands->Give_Money(enterer,(float)cost,0);
		Commands->Create_Object(preset,spawn_position);
	}
}

void JFW_Zone_Character::Entered(GameObject *obj,GameObject *enterer)
{
	const char *preset;
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	preset = Get_Parameter("Preset_Name");
	Change_Character(enterer,preset);
}

void JFW_Zone_Character_Swap::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	if (_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Old_Preset_Name")))
	{
		return;
	}
	const char *preset;
	preset = Get_Parameter("Preset_Name");
	Change_Character(enterer,preset);
}

void JFW_Teleport_Zone_Team::Entered(GameObject *obj,GameObject *enterer)
{
	Vector3 spawn_position;
	spawn_position = Get_Vector3_Parameter("Location");
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	int x = Get_Int_Parameter("Object_ID");
	if (x)
	{
		GameObject *gotoObject = Commands->Find_Object(x);
		Vector3 gotoLocation = Commands->Get_Position(gotoObject);
		Commands->Set_Position(enterer,gotoLocation);
	}
	else
	{
		Commands->Set_Position(enterer,spawn_position);
	}
}

void JFW_Repair_Zone_VTOL_Only::Entered(GameObject *obj,GameObject *enterer)
{
	int Player_Type = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,Player_Type))
	{
		return;
	}
	if (enterer->As_VehicleGameObj() && enterer->As_VehicleGameObj()->Get_Definition().Can_Repair())
	{
		if (Get_Vehicle_Mode(enterer) == VEHICLE_TYPE_FLYING)
		{
			Commands->Start_Timer(obj,this,1.0,Commands->Get_ID(enterer));
		}
	}
}

void JFW_Repair_Zone_VTOL_Only::Timer_Expired(GameObject *obj,int number)
{
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= Get_Float_Parameter("Repair_Distance"))
	{
		float Max_Health = Commands->Get_Max_Health(Commands->Find_Object(number));
		float Health = Commands->Get_Health(Commands->Find_Object(number));
		float Max_Shield_Strength = Commands->Get_Max_Shield_Strength(Commands->Find_Object(number));
		float Shield_Strength = Commands->Get_Shield_Strength(Commands->Find_Object(number));
		bool repair = false;
		if (Health < Max_Health)
		{
			Commands->Set_Health(Commands->Find_Object(number),Health + Get_Float_Parameter("Repair_Health"));
			repair = true;
		}
		if (Shield_Strength < Max_Shield_Strength)
		{
			Commands->Set_Shield_Strength(Commands->Find_Object(number),Shield_Strength + Get_Float_Parameter("Repair_Health"));
			repair = true;
		}
		if (repair && Commands->Find_Object(number)->As_VehicleGameObj())
		{
			Commands->Find_Object(number)->As_VehicleGameObj()->Damage_Meshes_Update(); //repair was done, update damage meshes
		}
		Commands->Start_Timer(obj,this,1.0,number);
	}
}

void JFW_Repair_Zone_No_VTOL::Entered(GameObject *obj,GameObject *enterer)
{
	int Player_Type = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,Player_Type))
	{
		return;
	}
	if (enterer->As_VehicleGameObj() && enterer->As_VehicleGameObj()->Get_Definition().Can_Repair())
	{
		if (Get_Vehicle_Mode(enterer) != VEHICLE_TYPE_FLYING)
		{
			Commands->Start_Timer(obj,this,1.0,Commands->Get_ID(enterer));
		}
	}
}

void JFW_Repair_Zone_No_VTOL::Timer_Expired(GameObject *obj,int number)
{
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= Get_Float_Parameter("Repair_Distance"))
	{
		float Max_Health = Commands->Get_Max_Health(Commands->Find_Object(number));
		float Health = Commands->Get_Health(Commands->Find_Object(number));
		float Max_Shield_Strength = Commands->Get_Max_Shield_Strength(Commands->Find_Object(number));
		float Shield_Strength = Commands->Get_Shield_Strength(Commands->Find_Object(number));
		bool repair = false;
		if (Health < Max_Health)
		{
			Commands->Set_Health(Commands->Find_Object(number),Health + Get_Float_Parameter("Repair_Health"));
			repair = true;
		}
		if (Shield_Strength < Max_Shield_Strength)
		{
			Commands->Set_Shield_Strength(Commands->Find_Object(number),Shield_Strength + Get_Float_Parameter("Repair_Health"));
			repair = true;
		}
		if (repair && Commands->Find_Object(number)->As_VehicleGameObj())
		{
			Commands->Find_Object(number)->As_VehicleGameObj()->Damage_Meshes_Update(); //repair was done, update damage meshes
		}
		Commands->Start_Timer(obj,this,1.0,number);
	}
}

void JFW_Refill_Buy::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	int cost;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(enterer))
	{
		cost = -cost;
		Commands->Give_Money(enterer,(float)cost,0);
		Grant_Refill(enterer);
	}
}

void JFW_Powerup_Buy::Entered(GameObject *obj,GameObject *enterer)
{
	const char *preset;
	int x;
	int cost;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	preset = Get_Parameter("Preset_Name");
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(enterer))
	{
		cost = -cost;
		Commands->Give_Money(enterer,(float)cost,0);
		Grant_Powerup(enterer,preset);
	}
}

void JFW_Vehicle_Buy::Entered(GameObject *obj,GameObject *enterer)
{
	const char *preset;
	int x;
	int cost;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	preset = Get_Parameter("Preset_Name");
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(enterer))
	{
		cost = -cost;
		Commands->Give_Money(enterer,(float)cost,0);
		Create_Vehicle(preset,5.0,enterer,x);
	}
}

void JFW_Zone_Vehicle::Entered(GameObject *obj,GameObject *enterer)
{
	const char *preset;
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	preset = Get_Parameter("Preset_Name");
	Create_Vehicle(preset,5.0,enterer,x);
}

void JFW_Zone_Powerup::Entered(GameObject *obj,GameObject *enterer)
{
	const char *preset;
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	preset = Get_Parameter("Preset_Name");
	Grant_Powerup(enterer,preset);
}

void JFW_Zone_Refill::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	Grant_Refill(enterer);
}

void JFW_Zone_Send_Custom_Team::Entered(GameObject *obj,GameObject *enterer)
{
	if (!Get_Object_Type(enterer))
	{
		GameObject *object = Commands->Find_Object(Get_Int_Parameter("NodID"));
		int type;
		int param;
		type = Get_Int_Parameter("NodEnterMessage");
		param = Get_Int_Parameter("NodEnterParam");
		if (type)
		{
			if (!param)
			{
				param = Commands->Get_ID(enterer);
			}
			Commands->Send_Custom_Event(obj,object,type,param,0);
		}
	}
	else
	{
		GameObject *object = Commands->Find_Object(Get_Int_Parameter("GDIID"));
		int type;
		int param;
		type = Get_Int_Parameter("GDIEnterMessage");
		param = Get_Int_Parameter("GDIEnterParam");
		if (type)
		{
			if (!param)
			{
				param = Commands->Get_ID(enterer);
			}
			Commands->Send_Custom_Event(obj,object,type,param,0);
		}
	}
}

void JFW_Zone_Send_Custom_Team::Exited(GameObject *obj,GameObject *exiter)
{
	if (!Get_Object_Type(exiter))
	{
		GameObject *object = Commands->Find_Object(Get_Int_Parameter("NodID"));
		int type;
		int param;
		type = Get_Int_Parameter("NodExitMessage");
		param = Get_Int_Parameter("NodExitParam");
		if (type)
		{
			if (!param)
			{
				param = Commands->Get_ID(exiter);
			}
			Commands->Send_Custom_Event(obj,object,type,param,0);
		}
	}
	else
	{
		GameObject *object = Commands->Find_Object(Get_Int_Parameter("GDIID"));
		int type;
		int param;
		type = Get_Int_Parameter("GDIExitMessage");
		param = Get_Int_Parameter("GDIExitParam");
		if (type)
		{
			if (!param)
			{
				param = Commands->Get_ID(exiter);
			}
			Commands->Send_Custom_Event(obj,object,type,param,0);
		}
	}
}

void JFW_Zone_Animation::Created(GameObject *obj)
{
	enabled = false;
	GameObject *object = Commands->Create_Object("Generic_Cinematic",Commands->Get_Position(obj));
	Commands->Set_Model(object,Get_Parameter("Model"));
	ID = Commands->Get_ID(object);
}

void JFW_Zone_Animation::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
	Auto_Save_Variable(&ID,4,2);
}

void JFW_Zone_Animation::Entered(GameObject *obj,GameObject *enterer)
{
	enabled = true;
	const char *subobject = Get_Parameter("UpSubobject");
	if (!_stricmp(subobject,"0"))
	{
		subobject = 0;
	}
	float firstframe = Get_Float_Parameter("UpFirstFrame");
	if (firstframe == -1)
	{
		firstframe = Get_Animation_Frame(Commands->Find_Object(ID));
	}
	Commands->Set_Animation(Commands->Find_Object(ID),Get_Parameter("UpAnimation"),false,subobject,firstframe,Get_Float_Parameter("UpLastFrame"),Get_Bool_Parameter("UpBlended"));
}

void JFW_Zone_Animation::Animation_Complete(GameObject *obj,const char *animation_name)
{
	if (!_stricmp(animation_name,Get_Parameter("UpAnimation")) || !_stricmp(animation_name,Get_Parameter("Animation")))
	{
		const char *subobject = Get_Parameter("Subobject");
		if (!_stricmp(subobject,"0"))
		{
			subobject = 0;
		}
		float firstframe = Get_Float_Parameter("FirstFrame");
		if (firstframe == -1)
		{
			firstframe = Get_Animation_Frame(Commands->Find_Object(ID));
		}
		obj->As_PhysicalGameObj()->Clear_Animation();
		Commands->Set_Animation(Commands->Find_Object(ID),Get_Parameter("Animation"),false,subobject,firstframe,Get_Float_Parameter("LastFrame"),Get_Bool_Parameter("Blended"));
	}
}

void JFW_Zone_Animation::Exited(GameObject *obj,GameObject *exiter)
{
	const char *subobject = Get_Parameter("DownSubobject");
	if (!_stricmp(subobject,"0"))
	{
		subobject = 0;
	}
	float firstframe = Get_Float_Parameter("DownFirstFrame");
	if (firstframe == -1)
	{
		firstframe = Get_Animation_Frame(Commands->Find_Object(ID));
	}
	Commands->Set_Animation(Commands->Find_Object(ID),Get_Parameter("DownAnimation"),false,subobject,firstframe,Get_Float_Parameter("DownLastFrame"),Get_Bool_Parameter("DownBlended"));
	enabled = false;
}

void JFW_Group_Purchase_Zone::Created(GameObject *obj)
{
	cash = 0;
	enabled = true;
}

void JFW_Group_Purchase_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (enabled)
	{
		float money = Commands->Get_Money(enterer);
		if (money >= Get_Float_Parameter("CashPerPerson"))
		{
			Commands->Give_Money(enterer,Get_Float_Parameter("CashPerPerson"),0);
			cash += Get_Float_Parameter("CashPerPerson");
		}
		if (cash >= Get_Float_Parameter("TotalCash"))
		{
			cash = 0;
			enabled = false;
			Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message"),0,0);
		}
	}
}

void JFW_Group_Purchase_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&cash,4,1);
	Auto_Save_Variable(&enabled,1,2);
}

void JFW_Zone_Timer::Created(GameObject *obj)
{
	count = 0;
	enabled = true;
}

void JFW_Zone_Timer::Entered(GameObject *obj,GameObject *enterer)
{
	count++;
	if (enabled)
	{
		if (count == 1)
		{
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
		}
	}
}

void JFW_Zone_Timer::Exited(GameObject *obj,GameObject *exiter)
{
	count--;
}

void JFW_Zone_Timer::Timer_Expired(GameObject *obj,int number)
{
	if (count >= Get_Int_Parameter("Count"))
	{
		enabled = false;
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message"),0,0);
	}
}

void JFW_Zone_Timer::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&count,4,1);
	Auto_Save_Variable(&enabled,1,2);
}

void JFW_Gate_Zone_2::Entered(GameObject *obj,GameObject *enterer)
{
	int Player_Type = Get_Int_Parameter("Player_Type");
	int ID = Get_Int_Parameter("ID");
	const char *Animation = Get_Parameter("Animation");
	int TimerNum = Get_Int_Parameter("TimerNum");
	float Time = Get_Float_Parameter("Time");
	if (CheckPlayerType(enterer,Player_Type))
	{
		return;
	}
	Commands->Set_Animation(Commands->Find_Object(ID),Animation,false,0,0,-1,false);
	Commands->Start_Timer(obj,this,Time,TimerNum);
}

void JFW_Gate_Zone_2::Timer_Expired(GameObject *obj,int number)
{
	int ID = Get_Int_Parameter("ID");
	const char *Animation = Get_Parameter("Animation");
	int TimerNum = Get_Int_Parameter("TimerNum");
	if (TimerNum == number)
	{
		Commands->Set_Animation(Commands->Find_Object(ID),Animation,false,0,Get_Animation_Frame(Commands->Find_Object(ID)),0,false);
	}
}

void JFW_Character_Buy::Entered(GameObject *obj,GameObject *enterer)
{
	const char *preset;
	int x;
	int cost;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	preset = Get_Parameter("Preset_Name");
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(enterer))
	{
		cost = -cost;
		Commands->Give_Money(enterer,(float)cost,0);
		Change_Character(enterer,preset);
	}
}

void JFW_Blow_Up_On_Enter::Entered(GameObject *obj,GameObject *enterer)
{
	const char *c;
	Vector3 v;
	c = Get_Parameter("Explosion");
	v = Commands->Get_Position(enterer);
	Commands->Create_Explosion(c,v,0);
}

void JFW_Blow_Up_On_Enter_Delay::Entered(GameObject *obj,GameObject *enterer)
{
	int Player_Type = Get_Int_Parameter("Player_Type");
	int TimerNum = Get_Int_Parameter("TimerNum");
	float Time = Get_Float_Parameter("Time");
	if (CheckPlayerType(enterer,Player_Type))
	{
		return;
	}
	Commands->Start_Timer(obj,this,Time,TimerNum);
}

void JFW_Blow_Up_On_Enter_Delay::Timer_Expired(GameObject *obj,int number)
{
	const char *c;
	Vector3 v;
	c = Get_Parameter("Explosion");
	v = Commands->Get_Position(obj);
	Commands->Create_Explosion(c,v,0);
}

void JFW_Apply_Damage_On_Enter::Entered(GameObject *obj,GameObject *enterer)
{
	float f;
	const char *w;
	w = Get_Parameter("Warhead");
	f = Get_Float_Parameter("Damage");
	Commands->Apply_Damage(enterer,f,w,0);
}

void JFW_Apply_Damage_On_Enter_Timer::Entered(GameObject *obj,GameObject *enterer)
{
	int Player_Type = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,Player_Type))
	{
		return;
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Commands->Get_ID(enterer));
}

void JFW_Apply_Damage_On_Enter_Timer::Timer_Expired(GameObject *obj,int number)
{
	float f;
	const char *w;
	float distance;
	Vector3 pos1;
	Vector3 pos2;
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= Get_Float_Parameter("Distance"))
	{
		w = Get_Parameter("Warhead");
		f = Get_Float_Parameter("Damage");
		Commands->Apply_Damage(Commands->Find_Object(number),f,w,0);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),number);
	}
}

void JFW_Apply_Damage_On_Enter_Timer::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ID,4,1);
}

void JFW_Blow_Up_On_Enter_Random::Entered(GameObject *obj,GameObject *enterer)
{
	int random = Commands->Get_Random_Int(0,100);
	if (random <= Get_Int_Parameter("Probobility"))
	{
		const char *c;
		Vector3 v;
		c = Get_Parameter("Explosion");
		v = Commands->Get_Position(enterer);
		Commands->Create_Explosion(c,v,0);
	}
}

void JFW_Create_Destroy_Object_On_Enter::Entered(GameObject *obj,GameObject *enterer)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	if (!ObjectID)
	{
		GameObject *object = Commands->Create_Object(Get_Parameter("Preset"),Get_Vector3_Parameter("Location"));
		Commands->Set_Facing(object,Get_Float_Parameter("Facing"));
		ObjectID = Commands->Get_ID(object);
	}
}

void JFW_Create_Destroy_Object_On_Enter::Exited(GameObject *obj,GameObject *exiter)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(exiter,team))
	{
		return;
	}
	if (ObjectID)
	{
		Commands->Destroy_Object(Commands->Find_Object(ObjectID));
		ObjectID = 0;
	}
}

void JFW_Create_Destroy_Object_On_Enter::Created(GameObject *obj)
{
	ObjectID = 0;
}

void JFW_Create_Destroy_Object_On_Enter::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ObjectID,4,1);
}

void JFW_Send_Driver_Custom_On_Enter::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		Commands->Send_Custom_Event(obj,sender,Get_Int_Parameter("EnterMessage"),0,0);
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		Commands->Send_Custom_Event(obj,sender,Get_Int_Parameter("ExitMessage"),0,0);
	}
}

void JFW_Fog_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (Get_Int_Parameter("OnEnter") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
		{
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		}
		else
		{
			bool fogenable = Get_Bool_Parameter("Fog_Enable");
			Commands->Set_Fog_Enable(fogenable);
			if (fogenable)
			{
				Commands->Set_Fog_Range(Get_Float_Parameter("Fog_Start_Distance"),Get_Float_Parameter("Fog_End_Distance"),1.0f);
			}
		}
	}
}

void JFW_Fog_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (Get_Int_Parameter("OnExit") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
		{
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		}
		else
		{
			bool fogenable = Get_Bool_Parameter("Fog_Enable");
			Commands->Set_Fog_Enable(fogenable);
			if (fogenable)
			{
				Commands->Set_Fog_Range(Get_Float_Parameter("Fog_Start_Distance"),Get_Float_Parameter("Fog_End_Distance"),1.0f);
			}
		}
	}
}

void JFW_Fog_Zone::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		bool fogenable = Get_Bool_Parameter("Fog_Enable");
		Commands->Set_Fog_Enable(fogenable);
		if (fogenable)
		{
			Commands->Set_Fog_Range(Get_Float_Parameter("Fog_Start_Distance"),Get_Float_Parameter("Fog_End_Distance"),1.0f);
		}
	}
}

void JFW_War_Blitz_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (Get_Int_Parameter("OnEnter") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
			Commands->Set_War_Blitz(Get_Float_Parameter("War_Blitz_Intensity"),Get_Float_Parameter("Start_Distance"),Get_Float_Parameter("End_Distance"),Get_Float_Parameter("War_Blitz_Heading"),Get_Float_Parameter("War_Blitz_Distribution"),1.0f);
	}
}

void JFW_War_Blitz_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (Get_Int_Parameter("OnExit") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
			Commands->Set_War_Blitz(Get_Float_Parameter("War_Blitz_Intensity"),Get_Float_Parameter("Start_Distance"),Get_Float_Parameter("End_Distance"),Get_Float_Parameter("War_Blitz_Heading"),Get_Float_Parameter("War_Blitz_Distribution"),1.0f);
	}
}

void JFW_War_Blitz_Zone::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
		Commands->Set_War_Blitz(Get_Float_Parameter("War_Blitz_Intensity"),Get_Float_Parameter("Start_Distance"),Get_Float_Parameter("End_Distance"),Get_Float_Parameter("War_Blitz_Heading"),Get_Float_Parameter("War_Blitz_Distribution"),1.0f);
}

void JFW_Heavy_Vehicle_Damage_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (enterer->As_VehicleGameObj())
	{
		int Player_Type = Get_Int_Parameter("Player_Type");
		if ((CheckPlayerType(enterer,Player_Type)) || (Get_Mass(enterer) < Get_Float_Parameter("Mass")))
		{
			return;
		}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Commands->Get_ID(enterer));
	}
}

void JFW_Heavy_Vehicle_Damage_Zone::Timer_Expired(GameObject *obj,int number)
{
	float damage;
	const char *w;
	float distance;
	Vector3 pos1;
	Vector3 pos2;
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= Get_Float_Parameter("Distance"))
	{
		w = Get_Parameter("Warhead");
		damage = Get_Float_Parameter("Damage");
		Commands->Apply_Damage(Commands->Find_Object(number),damage,w,0);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),number);
	}
}

void JFW_Light_Vehicle_Damage_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (enterer->As_VehicleGameObj())
	{
		int Player_Type = Get_Int_Parameter("Player_Type");
		if ((CheckPlayerType(enterer,Player_Type)) || (Get_Mass(enterer) > Get_Float_Parameter("Mass")))
		{
			return;
		}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Commands->Get_ID(enterer));
	}
}

void JFW_Light_Vehicle_Damage_Zone::Timer_Expired(GameObject *obj,int number)
{
	float f;
	const char *w;
	float distance;
	Vector3 pos1;
	Vector3 pos2;
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= Get_Float_Parameter("Distance"))
	{
		w = Get_Parameter("Warhead");
		f = Get_Float_Parameter("Damage");
		Commands->Apply_Damage(Commands->Find_Object(number),f,w,0);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),number);
	}
}

void JFW_2D_Sound_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int team;
	const char *c;
	team = Get_Int_Parameter("Player_Type");
	c = Get_Parameter("Sound");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	Commands->Create_2D_Sound(c);
}

void JFW_Disable_Radar_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	Enable_Radar_Player(enterer,false);
}

void JFW_Disable_Radar_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(exiter,x))
	{
		return;
	}
	Enable_Radar_Player(exiter,true);
}

void JFW_Zone_PCT::Entered(GameObject *obj,GameObject *enterer)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	if (!team)
	{
		Display_NOD_Player_Terminal_Player(enterer);
	}
	if (team == 1)
	{
		Display_GDI_Player_Terminal_Player(enterer);
	}
	if (team == 2)
	{
		if (!Commands->Get_Player_Type(enterer))
		{
			Display_NOD_Player_Terminal_Player(enterer);
		}
		if (Commands->Get_Player_Type(enterer) == 1)
		{
			Display_GDI_Player_Terminal_Player(enterer);
		}
	}
}

void JFW_3D_Sound_Team_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int team;
	const char *c;
	Vector3 v;
	team = Get_Int_Parameter("Player_Type");
	c = Get_Parameter("Sound");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	v = Commands->Get_Position(enterer);
	Create_Sound_Team(c,v,obj,team);
}

void JFW_3D_Sound_Player_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int team;
	const char *c;
	Vector3 v;
	team = Get_Int_Parameter("Player_Type");
	c = Get_Parameter("Sound");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	v = Commands->Get_Position(enterer);
	Create_Sound_Player(enterer,c,v,obj);
}

void JFW_2D_Sound_Team_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int team;
	const char *c;
	team = Get_Int_Parameter("Player_Type");
	c = Get_Parameter("Sound");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	Create_2D_Sound_Team(c,team);
}

void JFW_2D_Sound_Player_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int team;
	const char *c;
	team = Get_Int_Parameter("Player_Type");
	c = Get_Parameter("Sound");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	Create_2D_Sound_Player(enterer,c);
}

void JFW_Look_At_Location_Entry::Entered(GameObject *obj,GameObject *enterer)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	Force_Camera_Look_Player(enterer,Get_Vector3_Parameter("Location"));
}

void JFW_Look_At_Object_Entry::Entered(GameObject *obj,GameObject *enterer)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	Force_Camera_Look_Player(enterer,Commands->Get_Position(Commands->Find_Object(Get_Int_Parameter("ObjectID"))));
}

void JFW_Stealth_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	if (!_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Stealth1")))
	{
		return;
	}
	if (!_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Stealth2")))
	{
		return;
	}
	if (!_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Stealth3")))
	{
		return;
	}
	if (!_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Stealth4")))
	{
		return;
	}
	Commands->Enable_Stealth(enterer,true);
}

void JFW_Stealth_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(exiter,team))
	{
		return;
	}
	if (!_stricmp(Commands->Get_Preset_Name(exiter),Get_Parameter("Stealth1")))
	{
		return;
	}
	if (!_stricmp(Commands->Get_Preset_Name(exiter),Get_Parameter("Stealth2")))
	{
		return;
	}
	if (!_stricmp(Commands->Get_Preset_Name(exiter),Get_Parameter("Stealth3")))
	{
		return;
	}
	if (!_stricmp(Commands->Get_Preset_Name(exiter),Get_Parameter("Stealth4")))
	{
		return;
	}
	Commands->Enable_Stealth(exiter,false);
}

void JFW_Zone_Send_Custom_Multiple::Created(GameObject *obj)
{
	count = 0;
}

void JFW_Zone_Send_Custom_Multiple::Entered(GameObject *obj,GameObject *enterer)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	count++;
	if (count == Get_Int_Parameter("Count"))
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message"),0,0);
	}
}

void JFW_Zone_Send_Custom_Multiple::Exited(GameObject *obj,GameObject *exiter)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(exiter,team))
	{
		return;
	}
	count--;
}

void JFW_Zone_Send_Custom_Multiple::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&count,4,1);
}

void JFW_Zone_Send_Custom_Enter::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	Commands->Send_Custom_Event(obj,enterer,Get_Int_Parameter("Message"),0,0);
}

void JFW_Zone_Send_Custom_Exit::Exited(GameObject *obj,GameObject *exiter)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(exiter,x))
	{
		return;
	}
	Commands->Send_Custom_Event(obj,exiter,Get_Int_Parameter("Message"),0,0);
}

void JFW_Vehicle_Zone_Animation::Created(GameObject *obj)
{
	refcount = 0;
	GameObject *object = Commands->Create_Object("Generic_Cinematic",Commands->Get_Position(obj));
	Commands->Set_Model(object,Get_Parameter("Model"));
	ID = Commands->Get_ID(object);
}

void JFW_Vehicle_Zone_Animation::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&refcount,1,1);
	Auto_Save_Variable(&ID,4,2);
}

void JFW_Vehicle_Zone_Animation::Entered(GameObject *obj,GameObject *enterer)
{
	if (enterer->As_VehicleGameObj())
	{
		if (!refcount)
		{
			const char *subobject = Get_Parameter("UpSubobject");
			if (!_stricmp(subobject,"0"))
			{
				subobject = 0;
			}
			float firstframe = Get_Float_Parameter("UpFirstFrame");
			if (firstframe == -1)
			{
				firstframe = Get_Animation_Frame(Commands->Find_Object(ID));
			}
			Commands->Set_Animation(Commands->Find_Object(ID),Get_Parameter("UpAnimation"),false,subobject,firstframe,Get_Float_Parameter("UpLastFrame"),Get_Bool_Parameter("UpBlended"));
		}
		refcount++;
	}
}

void JFW_Vehicle_Zone_Animation::Animation_Complete(GameObject *obj,const char *animation_name)
{
	if (!_stricmp(animation_name,Get_Parameter("UpAnimation")) || !_stricmp(animation_name,Get_Parameter("Animation")))
	{
		const char *subobject = Get_Parameter("Subobject");
		if (!_stricmp(subobject,"0"))
		{
			subobject = 0;
		}
		float firstframe = Get_Float_Parameter("FirstFrame");
		if (firstframe == -1)
		{
			firstframe = Get_Animation_Frame(Commands->Find_Object(ID));
		}
		obj->As_PhysicalGameObj()->Clear_Animation();
		Commands->Set_Animation(Commands->Find_Object(ID),Get_Parameter("Animation"),false,subobject,firstframe,Get_Float_Parameter("LastFrame"),Get_Bool_Parameter("Blended"));
	}
}

void JFW_Vehicle_Zone_Animation::Exited(GameObject *obj,GameObject *exiter)
{
	if (exiter->As_VehicleGameObj())
	{
		refcount--;
		if (!refcount)
		{
			const char *subobject = Get_Parameter("DownSubobject");
			if (!_stricmp(subobject,"0"))
			{
				subobject = 0;
			}
			float firstframe = Get_Float_Parameter("DownFirstFrame");
			if (firstframe == -1)
			{
				firstframe = Get_Animation_Frame(Commands->Find_Object(ID));
			}
			Commands->Set_Animation(Commands->Find_Object(ID),Get_Parameter("DownAnimation"),false,subobject,firstframe,Get_Float_Parameter("DownLastFrame"),Get_Bool_Parameter("DownBlended"));
		}
	}
}

void JFW_Zone_Money_Preset::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	if (!_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Preset")))
	{
		if (ID != Commands->Get_ID(enterer))
		{
			ID = Commands->Get_ID(enterer);
			Commands->Give_Money(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Float_Parameter("Money"),true);
		}
	}
}

void JFW_Zone_Money_Preset::Exited(GameObject *obj,GameObject *exiter)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(exiter,x))
	{
		return;
	}
	if (!_stricmp(Commands->Get_Preset_Name(exiter),Get_Parameter("Preset")))
	{
		if (ID == Commands->Get_ID(exiter))
		{
			ID = 0;
		}
	}
}

void JFW_Zone_Money_Preset::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ID,1,4);
}

void JFW_PPAGE_Zone::Entered(GameObject *obj,GameObject *enterer)
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
	sprintf(message,"ppage %d %s",Get_Player_ID(enterer),Get_Parameter("Message"));
	Console_Input(message);
}

void JFW_MSG_Zone::Entered(GameObject *obj,GameObject *enterer)
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
	sprintf(message,"msg %d %s",Get_Player_ID(enterer),Get_Parameter("Message"));
	Console_Input(message);
}

void JFW_TMSG_Zone::Entered(GameObject *obj,GameObject *enterer)
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
	sprintf(message,"tmsg %d %s",Get_Player_ID(enterer),Get_Parameter("Message"));
	Console_Input(message);
}

void JFW_Teleport_Zone_Enable::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("EnableCustom"))
	{
		enabled = true;
	}
	if (type == Get_Int_Parameter("DisableCustom"))
	{
		enabled = false;
	}
}

void JFW_Teleport_Zone_Enable::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Teleport_Zone_Enable::Created(GameObject *obj)
{
	enabled = false;
}

void JFW_Teleport_Zone_Enable::Entered(GameObject *obj,GameObject *enterer)
{
	Vector3 spawn_position;
	spawn_position = Get_Vector3_Parameter("Location");
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	if (!enabled)
	{
		return;
	}
	int x = Get_Int_Parameter("Object_ID");
	if (x)
	{
		GameObject *gotoObject = Commands->Find_Object(x);
		Vector3 gotoLocation = Commands->Get_Position(gotoObject);
		Commands->Set_Position(enterer,gotoLocation);
	}
	else
	{
		Commands->Set_Position(enterer,spawn_position);
	}
}

void JFW_Radar_Spy_Zone::Created(GameObject *obj)
{
	reset = false;
}

void JFW_Radar_Spy_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (Get_Vehicle(enterer) || reset)
	{
		return;
	}
	if (!Is_Script_Attached(enterer,Get_Parameter("Spy_Script")))
	{
		return;
	}
	if (Find_Com_Center(Commands->Get_Player_Type(enterer)) && (Is_Building_Dead(Find_Com_Center(Commands->Get_Player_Type(enterer))) && (Is_Radar_Enabled(PTTEAM(Commands->Get_Player_Type(enterer))))))
	{
		Create_2D_Sound_Team(Get_Parameter("Sound"),Commands->Get_Player_Type(enterer));
		Enable_Base_Radar(Get_Object_Type(enterer),true);
		reset = true;
	}
}

void JFW_Radar_Spy_Zone_New::Created(GameObject *obj)
{
	reset = false;
}

void JFW_Radar_Spy_Zone_New::Entered(GameObject *obj,GameObject *enterer)
{
	if (Get_Vehicle(enterer) || reset)
	{
		return;
	}
	if (!Is_Spy(enterer))
	{
		return;
	}
	if (Find_Com_Center(Commands->Get_Player_Type(enterer)) && (Is_Building_Dead(Find_Com_Center(Commands->Get_Player_Type(enterer))) && (Is_Radar_Enabled(PTTEAM(Commands->Get_Player_Type(enterer))))))
	{
		Create_2D_Sound_Team(Get_Parameter("Sound"),Commands->Get_Player_Type(enterer));
		Enable_Base_Radar(Get_Object_Type(enterer),true);
		reset = true;
	}
}

void JFW_2D_Sound_Zone_Team::Entered(GameObject *obj,GameObject *enterer)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	int TimerNum = Get_Int_Parameter("TimerNum");
	float Time = Get_Float_Parameter("Time");
	Commands->Start_Timer(obj,this,Time,TimerNum);
}

void JFW_2D_Sound_Zone_Team::Timer_Expired(GameObject *obj,int number)
{
	int TimerNum = Get_Int_Parameter("TimerNum");
	if (TimerNum == number)
	{
		Create_2D_Sound_Team(Get_Parameter("Sound"),Get_Int_Parameter("Sound_Player_Type"));
	}
}

// -------------------------------------------------------------------------------------------------
// JFW_Repair_Zone_2

void JFW_Repair_Zone_2::Created(GameObject *obj)
{
	ID = 0;
	WaitForKeyhook = (Get_Parameter_Count()>=8) ? Get_Int_Parameter("WaitForKeyhook")==1 : true;
}

void JFW_Repair_Zone_2::Entered(GameObject *obj,GameObject *enterer)
{
	int Player_Type = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,Player_Type))
	{
		return;
	}
	if (!Commands->Find_Object(ID))
	{
		if ( WaitForKeyhook )
			RemoveHook();
		ID = 0;
	}
	if ((ID) && (ID != Commands->Get_ID(enterer)))
	{
		return;
	}
	if (enterer->As_VehicleGameObj() && enterer->As_VehicleGameObj()->Get_Definition().Can_Repair() && Get_Vehicle_Driver(enterer))
	{
		ID = Commands->Get_ID(enterer);

		if ( WaitForKeyhook )
		{
			Create_2D_Sound_Player(Get_Vehicle_Driver(enterer),Get_Parameter("Sound"));
			InstallHook("Repair",Get_Vehicle_Driver(enterer));
		}
		else
		{
			Commands->Start_Timer(Owner(),this,1.0,ID);
			Create_2D_Sound_Player(Get_Vehicle_Driver(Commands->Find_Object(ID)),Get_Parameter("RepairSound"));
		}
	}
}

void JFW_Repair_Zone_2::Timer_Expired(GameObject *obj,int number)
{
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	if (!Commands->Find_Object(ID))
	{
		ID = 0;
		return;
	}
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= Get_Float_Parameter("Repair_Distance"))
	{
		if (Get_Vehicle_Driver(Commands->Find_Object(number)))
		{
			float costDivisor = Get_Float_Parameter("Credits");
			float cost = (costDivisor==0) ? 0 : (Get_Cost(Commands->Get_Preset_Name(Commands->Find_Object(number))) / 2) / costDivisor;
			if (Commands->Get_Money(Get_Vehicle_Driver(Commands->Find_Object(number))) >= cost)
			{
				float Max_Health = Commands->Get_Max_Health(Commands->Find_Object(number));
				float Health = Commands->Get_Health(Commands->Find_Object(number));
				float Max_Shield_Strength = Commands->Get_Max_Shield_Strength(Commands->Find_Object(number));
				float Shield_Strength = Commands->Get_Shield_Strength(Commands->Find_Object(number));
				float cost_scale = 0.0f;
				if (Health < Max_Health)
				{
					Commands->Set_Health(Commands->Find_Object(number),Health + Max_Health / Get_Float_Parameter("Repair_Health"));
					cost_scale = 0.5f;
				}
				if (Shield_Strength < Max_Shield_Strength)
				{
					Commands->Set_Shield_Strength(Commands->Find_Object(number),Shield_Strength + Max_Shield_Strength / Get_Float_Parameter("Repair_Health"));
					cost_scale += 0.5f;
				}
				if (cost_scale > 0)
				{
					if (Commands->Find_Object(number)->As_VehicleGameObj())
					{
						Commands->Find_Object(number)->As_VehicleGameObj()->Damage_Meshes_Update(); //repair was done, update damage meshes
					}
					Commands->Give_Money(Get_Vehicle_Driver(Commands->Find_Object(number)),-cost * cost_scale,false);
					Commands->Start_Timer(obj,this,1.0,number);
					return;
				}
			}
		}
	}
	Create_2D_Sound_Player(Get_Vehicle_Driver(Commands->Find_Object(ID)),Get_Parameter("RepairStopSound"));
	ID = 0;
}

void JFW_Repair_Zone_2::Exited(GameObject *obj,GameObject *exiter)
{
	if (!Commands->Find_Object(ID) || ID == Commands->Get_ID(exiter))
	{
		if ( WaitForKeyhook )
			RemoveHook();
		ID = 0;
	}
}

void JFW_Repair_Zone_2::KeyHook()
{
	if (!Commands->Find_Object(ID))
	{
		RemoveHook();
		ID = 0;
	}
	if (Get_Vehicle_Driver(Commands->Find_Object(ID)))
	{
		Commands->Start_Timer(Owner(),this,1.0,ID);
		Create_2D_Sound_Player(Get_Vehicle_Driver(Commands->Find_Object(ID)),Get_Parameter("RepairSound"));
		RemoveHook();
	}
}

void JFW_Repair_Zone_2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ID,4,1);
}

// End JFW_Repair_Zone_2
// -------------------------------------------------------------------------------------------------

void JFW_Repair_Zone_No_Boats::Entered(GameObject *obj,GameObject *enterer)
{
	int Player_Type = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,Player_Type))
	{
		return;
	}
	if (!Commands->Find_Object(ID))
	{
		RemoveHook();
		ID = 0;
	}
	if ((ID) && (ID != Commands->Get_ID(enterer)))
	{
		return;
	}
	if (enterer->As_VehicleGameObj() && enterer->As_VehicleGameObj()->Get_Definition().Can_Repair() && Get_Vehicle_Driver(enterer) && Get_Vehicle_Mode(enterer) != VEHICLE_TYPE_BOAT && Get_Vehicle_Mode(enterer) != VEHICLE_TYPE_SUB)
	{
		Create_2D_Sound_Player(Get_Vehicle_Driver(enterer),Get_Parameter("Sound"));
		InstallHook("Repair",Get_Vehicle_Driver(enterer));
		ID = Commands->Get_ID(enterer);
	}
}

void JFW_Repair_Zone_No_Boats::Timer_Expired(GameObject *obj,int number)
{
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	if (!Commands->Find_Object(ID))
	{
		ID = 0;
		return;
	}
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= Get_Float_Parameter("Repair_Distance"))
	{
		if (Get_Vehicle_Driver(Commands->Find_Object(number)))
		{
			float cost = (Get_Cost(Commands->Get_Preset_Name(Commands->Find_Object(number))) / 2) / Get_Float_Parameter("Credits");
			if (Commands->Get_Money(Get_Vehicle_Driver(Commands->Find_Object(number))) >= cost)
			{
				float Max_Health = Commands->Get_Max_Health(Commands->Find_Object(number));
				float Health = Commands->Get_Health(Commands->Find_Object(number));
				float Max_Shield_Strength = Commands->Get_Max_Shield_Strength(Commands->Find_Object(number));
				float Shield_Strength = Commands->Get_Shield_Strength(Commands->Find_Object(number));
				float cost_scale = 0.0f;
				if (Health < Max_Health)
				{
					Commands->Set_Health(Commands->Find_Object(number),Health + Max_Health / Get_Float_Parameter("Repair_Health"));
					cost_scale = 0.5f;
				}
				if (Shield_Strength < Max_Shield_Strength)
				{
					Commands->Set_Shield_Strength(Commands->Find_Object(number),Shield_Strength + Max_Shield_Strength / Get_Float_Parameter("Repair_Health"));
					cost_scale += 0.5f;
				}
				if (cost_scale > 0)
				{
					if (Commands->Find_Object(number)->As_VehicleGameObj())
					{
						Commands->Find_Object(number)->As_VehicleGameObj()->Damage_Meshes_Update(); //repair was done, update damage meshes
					}
					Commands->Give_Money(Get_Vehicle_Driver(Commands->Find_Object(number)),-cost * cost_scale,false);
					Commands->Start_Timer(obj,this,1.0,number);
					return;
				}
			}
		}
	}
	Create_2D_Sound_Player(Get_Vehicle_Driver(Commands->Find_Object(ID)),Get_Parameter("RepairStopSound"));
	ID = 0;
}

void JFW_Repair_Zone_No_Boats::Exited(GameObject *obj,GameObject *exiter)
{
	if (!Commands->Find_Object(ID))
	{
		RemoveHook();
		ID = 0;
	}
	if (ID == Commands->Get_ID(exiter))
	{
		RemoveHook();
		ID = 0;
	}
}

void JFW_Repair_Zone_No_Boats::KeyHook()
{
	if (!Commands->Find_Object(ID))
	{
		RemoveHook();
		ID = 0;
	}
	if (Get_Vehicle_Driver(Commands->Find_Object(ID)))
	{
		Commands->Start_Timer(Owner(),this,1.0,ID);
		Create_2D_Sound_Player(Get_Vehicle_Driver(Commands->Find_Object(ID)),Get_Parameter("RepairSound"));
		RemoveHook();
	}
}

void JFW_Repair_Zone_No_Boats::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ID,4,1);
}

void JFW_Repair_Zone_No_Boats::Created(GameObject *obj)
{
	ID = 0;
}

void JFW_Repair_Zone_Boats::Entered(GameObject *obj,GameObject *enterer)
{
	int Player_Type = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,Player_Type))
	{
		return;
	}
	if (!Commands->Find_Object(ID))
	{
		RemoveHook();
		ID = 0;
	}
	if ((ID) && (ID != Commands->Get_ID(enterer)))
	{
		return;
	}
	if (enterer->As_VehicleGameObj() && enterer->As_VehicleGameObj()->Get_Definition().Can_Repair() && Get_Vehicle_Driver(enterer) && (Get_Vehicle_Mode(enterer) == VEHICLE_TYPE_BOAT || Get_Vehicle_Mode(enterer) == VEHICLE_TYPE_SUB))
	{
		Create_2D_Sound_Player(Get_Vehicle_Driver(enterer),Get_Parameter("Sound"));
		InstallHook("Repair",Get_Vehicle_Driver(enterer));
		ID = Commands->Get_ID(enterer);
	}
}

void JFW_Repair_Zone_Boats::Timer_Expired(GameObject *obj,int number)
{
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	if (!Commands->Find_Object(ID))
	{
		ID = 0;
		return;
	}
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= Get_Float_Parameter("Repair_Distance"))
	{
		if (Get_Vehicle_Driver(Commands->Find_Object(number)))
		{
			float cost = (Get_Cost(Commands->Get_Preset_Name(Commands->Find_Object(number))) / 2) / Get_Float_Parameter("Credits");
			if (Commands->Get_Money(Get_Vehicle_Driver(Commands->Find_Object(number))) >= cost)
			{
				float Max_Health = Commands->Get_Max_Health(Commands->Find_Object(number));
				float Health = Commands->Get_Health(Commands->Find_Object(number));
				float Max_Shield_Strength = Commands->Get_Max_Shield_Strength(Commands->Find_Object(number));
				float Shield_Strength = Commands->Get_Shield_Strength(Commands->Find_Object(number));
				float cost_scale = 0.0f;
				if (Health < Max_Health)
				{
					Commands->Set_Health(Commands->Find_Object(number),Health + Max_Health / Get_Float_Parameter("Repair_Health"));
					cost_scale = 0.5f;
				}
				if (Shield_Strength < Max_Shield_Strength)
				{
					Commands->Set_Shield_Strength(Commands->Find_Object(number),Shield_Strength + Max_Shield_Strength / Get_Float_Parameter("Repair_Health"));
					cost_scale += 0.5f;
				}
				if (cost_scale > 0)
				{
					if (Commands->Find_Object(number)->As_VehicleGameObj())
					{
						Commands->Find_Object(number)->As_VehicleGameObj()->Damage_Meshes_Update(); //repair was done, update damage meshes
					}
					Commands->Give_Money(Get_Vehicle_Driver(Commands->Find_Object(number)),-cost * cost_scale,false);
					Commands->Start_Timer(obj,this,1.0,number);
					return;
				}
			}
		}
	}
	Create_2D_Sound_Player(Get_Vehicle_Driver(Commands->Find_Object(ID)),Get_Parameter("RepairStopSound"));
	ID = 0;
}

void JFW_Repair_Zone_Boats::Exited(GameObject *obj,GameObject *exiter)
{
	if (!Commands->Find_Object(ID))
	{
		RemoveHook();
		ID = 0;
	}
	if (ID == Commands->Get_ID(exiter))
	{
		RemoveHook();
		ID = 0;
	}
}

void JFW_Repair_Zone_Boats::KeyHook()
{
	if (!Commands->Find_Object(ID))
	{
		RemoveHook();
		ID = 0;
	}
	if (Get_Vehicle_Driver(Commands->Find_Object(ID)))
	{
		Commands->Start_Timer(Owner(),this,1.0,ID);
		Create_2D_Sound_Player(Get_Vehicle_Driver(Commands->Find_Object(ID)),Get_Parameter("RepairSound"));
		RemoveHook();
	}
}

void JFW_Repair_Zone_Boats::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ID,4,1);
}

void JFW_Repair_Zone_Boats::Created(GameObject *obj)
{
	ID = 0;
}

void JFW_Sell_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int Player_Type = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,Player_Type))
	{
		return;
	}
	if (!Commands->Find_Object(ID))
	{
		RemoveHook();
		ID = 0;
	}
	if ((ID) && (ID != Commands->Get_ID(enterer)))
	{
		return;
	}
	if (enterer->As_VehicleGameObj() && enterer->As_VehicleGameObj()->Get_Definition().Can_Repair() && Get_Vehicle_Driver(enterer))
	{
		Create_2D_Sound_Player(Get_Vehicle_Driver(enterer),Get_Parameter("Sound"));
		InstallHook("Sell",Get_Vehicle_Driver(enterer));
		ID = Commands->Get_ID(enterer);
	}
}


void JFW_Sell_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (!Commands->Find_Object(ID))
	{
		RemoveHook();
		ID = 0;
	}
	if (ID == Commands->Get_ID(exiter))
	{
		RemoveHook();
		ID = 0;
	}
}

void JFW_Sell_Zone::KeyHook()
{
	if (!Commands->Find_Object(ID))
	{
		RemoveHook();
		ID = 0;
	}
	if (Commands->Find_Object(ID)->As_VehicleGameObj())
	{
		if (Get_Vehicle_Driver(Commands->Find_Object(ID)))
		{
			Create_2D_Sound_Player(Get_Vehicle_Driver(Commands->Find_Object(ID)),Get_Parameter("SellSound"));
			const char *preset = Commands->Get_Preset_Name(Commands->Find_Object(ID));
			float cost = ((float)Get_Cost(preset) / 2);
			Commands->Give_Money(Get_Vehicle_Driver(Commands->Find_Object(ID)),cost,false);
			Commands->Send_Custom_Event(Owner(),Commands->Find_Object(ID),CUSTOM_TRANSITION_VTOL_LAND_ZONE,1,0);	// Compatibility with dp88_Aircraft_LandingZone_Aircraft
			Force_Occupants_Exit(Commands->Find_Object(ID));
			Commands->Start_Timer(Owner(),this,1,ID);
			RemoveHook();
			ID = 0;
		}
	}
}

void JFW_Sell_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ID,4,1);
}

void JFW_Sell_Zone::Created(GameObject *obj)
{
	ID = 0;
}

void JFW_Sell_Zone::Timer_Expired(GameObject *obj,int number)
{
	Commands->Destroy_Object(Commands->Find_Object(number));
}

void JFW_Infantry_Force_Composition_Zone::Created(GameObject *obj)
{
	reset = false;
	count = 0;
	TeamPurchaseSettingsDefClass *teamdefs[2];
	teamdefs[0] = TeamPurchaseSettingsDefClass::Get_Definition(TeamPurchaseSettingsDefClass::TEAM_GDI);
	teamdefs[1] = TeamPurchaseSettingsDefClass::Get_Definition(TeamPurchaseSettingsDefClass::TEAM_NOD);
	PurchaseSettingsDefClass *defs[4];
	defs[0] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_CLASSES,PurchaseSettingsDefClass::TEAM_GDI);
	defs[1] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_CLASSES,PurchaseSettingsDefClass::TEAM_NOD);
	defs[2] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_SECRET_CLASSES,PurchaseSettingsDefClass::TEAM_GDI);
	defs[3] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_SECRET_CLASSES,PurchaseSettingsDefClass::TEAM_NOD);
	for (unsigned int d = 0;d < 2;d++)
	{
		for (unsigned int i = 0;i < 4;i++)
		{
			if (teamdefs[d]->Get_Enlisted_Definition(i))
			{
				count++;
			}
		}
	}
	for (unsigned int d = 0;d < 4;d++)
	{
		for (unsigned int i = 0;i < 10;i++)
		{
			if ((defs[d]) && (defs[d]->Get_Definition(i)))
			{
				count++;
				if (defs[d]->Get_Alt_Definition(i,0))
				{
					count++;
					if (defs[d]->Get_Alt_Definition(i,1))
					{
						count++;
						if (defs[d]->Get_Alt_Definition(i,2))
						{
							count++;
						}
					}
				}
			}
		}
	}
	IDs = new unsigned int[count];
	int pos = 0;
	for (unsigned int d = 0;d < 2;d++)
	{
		for (unsigned int i = 0;i < 4;i++)
		{
			if (teamdefs[d]->Get_Enlisted_Definition(i))
			{
				IDs[pos] = teamdefs[d]->Get_Enlisted_Definition(i);
				pos++;
			}
		}
	}
	for (unsigned int d = 0;d < 4;d++)
	{
		for (unsigned int i = 0;i < 10;i++)
		{
			if ((defs[d]) && (defs[d]->Get_Definition(i)))
			{
				IDs[pos] = defs[d]->Get_Definition(i);
				pos++;
				if (defs[d]->Get_Alt_Definition(i,0))
				{
					IDs[pos] = defs[d]->Get_Alt_Definition(i,0);
					pos++;
					if (defs[d]->Get_Alt_Definition(i,1))
					{
						IDs[pos] = defs[d]->Get_Alt_Definition(i,1);
						pos++;
						if (defs[d]->Get_Alt_Definition(i,2))
						{
							IDs[pos] = defs[d]->Get_Alt_Definition(i,2);
							pos++;
						}
					}
				}
			}
		}
	}
}

void JFW_Infantry_Force_Composition_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (Get_Vehicle(enterer) || reset)
	{
		return;
	}
	if (!Is_Script_Attached(enterer,Get_Parameter("Spy_Script")))
	{
		return;
	}
	int Player_Type = Commands->Get_Player_Type(enterer);
	int Count_Player_Type = PTTEAM(Player_Type);
	Create_2D_Sound_Team(Get_Parameter("Sound"),Player_Type);
	Send_Message_Team(Player_Type,255,255,255,"Current Enemy Infantry Strength:\n");
	reset = true;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("ResetTime"),1);
	for (unsigned int i = 0;i < count;i++)
	{
		const char *name = Get_Definition_Name(IDs[i]);
		unsigned int count = Get_Object_Count(Count_Player_Type,name);
		if (count)
		{
			char str[100];
			const char *str2 = Get_Translated_Definition_Name(name);
			sprintf(str,"%s: %u\n",str2,count);
			delete[] str2;
			Send_Message_Team(Player_Type,255,255,255,str);
		}
	}
}

void JFW_Infantry_Force_Composition_Zone::Timer_Expired(GameObject *obj,int number)
{
	reset = false;
}

JFW_Infantry_Force_Composition_Zone::~JFW_Infantry_Force_Composition_Zone()
{
	SAFE_DELETE_ARRAY(IDs);
}

void JFW_Vehicle_Force_Composition_Zone::Created(GameObject *obj)
{
	reset = false;
	count = 0;
	PurchaseSettingsDefClass *defs[8];
	defs[0] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_VEHICLES,PurchaseSettingsDefClass::TEAM_GDI);
	defs[1] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_VEHICLES,PurchaseSettingsDefClass::TEAM_NOD);
	defs[2] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_SECRET_VEHICLES,PurchaseSettingsDefClass::TEAM_GDI);
	defs[3] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_SECRET_VEHICLES,PurchaseSettingsDefClass::TEAM_NOD);
	defs[4] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_AIR,PurchaseSettingsDefClass::TEAM_GDI);
	defs[5] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_NAVAL,PurchaseSettingsDefClass::TEAM_NOD);
	defs[6] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_AIR,PurchaseSettingsDefClass::TEAM_GDI);
	defs[7] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_NAVAL,PurchaseSettingsDefClass::TEAM_NOD);
	for (unsigned int d = 0;d < 7;d++)
	{
		for (unsigned int i = 0;i < 10;i++)
		{
			if ((defs[d]) && (defs[d]->Get_Definition(i)))
			{
				count++;
				if (defs[d]->Get_Alt_Definition(i,0))
				{
					count++;
					if (defs[d]->Get_Alt_Definition(i,1))
					{
						count++;
						if (defs[d]->Get_Alt_Definition(i,2))
						{
							count++;
						}
					}
				}
			}
		}
	}
	IDs = new unsigned int[count];
	int pos = 0;
	for (unsigned int d = 0;d < 7;d++)
	{
		for (unsigned int i = 0;i < 10;i++)
		{
			if ((defs[d]) && (defs[d]->Get_Definition(i)))
			{
				IDs[pos] = defs[d]->Get_Definition(i);
				pos++;
				if (defs[d]->Get_Alt_Definition(i,0))
				{
					IDs[pos] = defs[d]->Get_Alt_Definition(i,0);
					pos++;
					if (defs[d]->Get_Alt_Definition(i,1))
					{
						IDs[pos] = defs[d]->Get_Alt_Definition(i,1);
						pos++;
						if (defs[d]->Get_Alt_Definition(i,2))
						{
							IDs[pos] = defs[d]->Get_Alt_Definition(i,2);
							pos++;
						}
					}
				}
			}
		}
	}
}

void JFW_Vehicle_Force_Composition_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (Get_Vehicle(enterer) || reset)
	{
		return;
	}
	if (!Is_Script_Attached(enterer,Get_Parameter("Spy_Script")))
	{
		return;
	}
	int Player_Type = Commands->Get_Player_Type(enterer);
	int Count_Player_Type = PTTEAM(Player_Type);
	Create_2D_Sound_Team(Get_Parameter("Sound"),Player_Type);
	Send_Message_Team(Player_Type,255,255,255,"Current Enemy Vehicle Strength:\n");
	reset = true;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("ResetTime"),1);
	for (unsigned int i = 0;i < count;i++)
	{
		const char *name = Get_Definition_Name(IDs[i]);
		unsigned int count = Get_Object_Count(Count_Player_Type,name);
		if (count)
		{
			char str[100];
			const char *str2 = Get_Translated_Definition_Name(name);
			sprintf(str,"%s: %u\n",str2,count);
			delete[] str2;
			Send_Message_Team(Player_Type,255,255,255,str);
		}
	}
}

void JFW_Vehicle_Force_Composition_Zone::Timer_Expired(GameObject *obj,int number)
{
	reset = false;
}

JFW_Vehicle_Force_Composition_Zone::~JFW_Vehicle_Force_Composition_Zone()
{
	SAFE_DELETE_ARRAY(IDs);
}

void JFW_Infantry_Force_Composition_Zone_New::Created(GameObject *obj)
{
	reset = false;
	count = 0;
	TeamPurchaseSettingsDefClass *teamdefs[2];
	teamdefs[0] = TeamPurchaseSettingsDefClass::Get_Definition(TeamPurchaseSettingsDefClass::TEAM_GDI);
	teamdefs[1] = TeamPurchaseSettingsDefClass::Get_Definition(TeamPurchaseSettingsDefClass::TEAM_NOD);
	PurchaseSettingsDefClass *defs[4];
	defs[0] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_CLASSES,PurchaseSettingsDefClass::TEAM_GDI);
	defs[1] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_CLASSES,PurchaseSettingsDefClass::TEAM_NOD);
	defs[2] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_SECRET_CLASSES,PurchaseSettingsDefClass::TEAM_GDI);
	defs[3] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_SECRET_CLASSES,PurchaseSettingsDefClass::TEAM_NOD);
	for (unsigned int d = 0;d < 2;d++)
	{
		for (unsigned int i = 0;i < 4;i++)
		{
			if (teamdefs[d]->Get_Enlisted_Definition(i))
			{
				count++;
			}
		}
	}
	for (unsigned int d = 0;d < 4;d++)
	{
		for (unsigned int i = 0;i < 10;i++)
		{
			if ((defs[d]) && (defs[d]->Get_Definition(i)))
			{
				count++;
				if (defs[d]->Get_Alt_Definition(i,0))
				{
					count++;
					if (defs[d]->Get_Alt_Definition(i,1))
					{
						count++;
						if (defs[d]->Get_Alt_Definition(i,2))
						{
							count++;
						}
					}
				}
			}
		}
	}
	IDs = new unsigned int[count];
	int pos = 0;
	for (unsigned int d = 0;d < 2;d++)
	{
		for (unsigned int i = 0;i < 4;i++)
		{
			if (teamdefs[d]->Get_Enlisted_Definition(i))
			{
				IDs[pos] = teamdefs[d]->Get_Enlisted_Definition(i);
				pos++;
			}
		}
	}
	for (unsigned int d = 0;d < 4;d++)
	{
		for (unsigned int i = 0;i < 10;i++)
		{
			if ((defs[d]) && (defs[d]->Get_Definition(i)))
			{
				IDs[pos] = defs[d]->Get_Definition(i);
				pos++;
				if (defs[d]->Get_Alt_Definition(i,0))
				{
					IDs[pos] = defs[d]->Get_Alt_Definition(i,0);
					pos++;
					if (defs[d]->Get_Alt_Definition(i,1))
					{
						IDs[pos] = defs[d]->Get_Alt_Definition(i,1);
						pos++;
						if (defs[d]->Get_Alt_Definition(i,2))
						{
							IDs[pos] = defs[d]->Get_Alt_Definition(i,2);
							pos++;
						}
					}
				}
			}
		}
	}
}

void JFW_Infantry_Force_Composition_Zone_New::Entered(GameObject *obj,GameObject *enterer)
{
	if (Get_Vehicle(enterer) || reset)
	{
		return;
	}
	if (!Is_Spy(enterer))
	{
		return;
	}
	int Player_Type = Commands->Get_Player_Type(enterer);
	int Count_Player_Type = PTTEAM(Player_Type);
	Create_2D_Sound_Team(Get_Parameter("Sound"),Player_Type);
	Send_Message_Team(Player_Type,255,255,255,"Current Enemy Infantry Strength:\n");
	reset = true;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("ResetTime"),1);
	for (unsigned int i = 0;i < count;i++)
	{
		const char *name = Get_Definition_Name(IDs[i]);
		unsigned int count = Get_Object_Count(Count_Player_Type,name);
		if (count)
		{
			char str[100];
			const char *str2 = Get_Translated_Definition_Name(name);
			sprintf(str,"%s: %u\n",str2,count);
			delete[] str2;
			Send_Message_Team(Player_Type,255,255,255,str);
		}
	}
}

void JFW_Infantry_Force_Composition_Zone_New::Timer_Expired(GameObject *obj,int number)
{
	reset = false;
}

JFW_Infantry_Force_Composition_Zone_New::~JFW_Infantry_Force_Composition_Zone_New()
{
	SAFE_DELETE_ARRAY(IDs);
}

void JFW_Vehicle_Force_Composition_Zone_New::Created(GameObject *obj)
{
	reset = false;
	count = 0;
	PurchaseSettingsDefClass *defs[8];
	defs[0] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_VEHICLES,PurchaseSettingsDefClass::TEAM_GDI);
	defs[1] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_VEHICLES,PurchaseSettingsDefClass::TEAM_NOD);
	defs[2] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_SECRET_VEHICLES,PurchaseSettingsDefClass::TEAM_GDI);
	defs[3] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_SECRET_VEHICLES,PurchaseSettingsDefClass::TEAM_NOD);
	defs[4] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_AIR,PurchaseSettingsDefClass::TEAM_GDI);
	defs[5] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_NAVAL,PurchaseSettingsDefClass::TEAM_NOD);
	defs[6] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_AIR,PurchaseSettingsDefClass::TEAM_GDI);
	defs[7] = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_NAVAL,PurchaseSettingsDefClass::TEAM_NOD);
	for (unsigned int d = 0;d < 7;d++)
	{
		for (unsigned int i = 0;i < 10;i++)
		{
			if ((defs[d]) && (defs[d]->Get_Definition(i)))
			{
				count++;
				if (defs[d]->Get_Alt_Definition(i,0))
				{
					count++;
					if (defs[d]->Get_Alt_Definition(i,1))
					{
						count++;
						if (defs[d]->Get_Alt_Definition(i,2))
						{
							count++;
						}
					}
				}
			}
		}
	}
	IDs = new unsigned int[count];
	int pos = 0;
	for (unsigned int d = 0;d < 7;d++)
	{
		for (unsigned int i = 0;i < 10;i++)
		{
			if ((defs[d]) && (defs[d]->Get_Definition(i)))
			{
				IDs[pos] = defs[d]->Get_Definition(i);
				pos++;
				if (defs[d]->Get_Alt_Definition(i,0))
				{
					IDs[pos] = defs[d]->Get_Alt_Definition(i,0);
					pos++;
					if (defs[d]->Get_Alt_Definition(i,1))
					{
						IDs[pos] = defs[d]->Get_Alt_Definition(i,1);
						pos++;
						if (defs[d]->Get_Alt_Definition(i,2))
						{
							IDs[pos] = defs[d]->Get_Alt_Definition(i,2);
							pos++;
						}
					}
				}
			}
		}
	}
}

void JFW_Vehicle_Force_Composition_Zone_New::Entered(GameObject *obj,GameObject *enterer)
{
	if (Get_Vehicle(enterer) || reset)
	{
		return;
	}
	if (!Is_Spy(enterer))
	{
		return;
	}
	int Player_Type = Commands->Get_Player_Type(enterer);
	int Count_Player_Type = PTTEAM(Player_Type);
	Create_2D_Sound_Team(Get_Parameter("Sound"),Player_Type);
	Send_Message_Team(Player_Type,255,255,255,"Current Enemy Vehicle Strength:\n");
	reset = true;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("ResetTime"),1);
	for (unsigned int i = 0;i < count;i++)
	{
		const char *name = Get_Definition_Name(IDs[i]);
		unsigned int count = Get_Object_Count(Count_Player_Type,name);
		if (count)
		{
			char str[100];
			const char *str2 = Get_Translated_Definition_Name(name);
			sprintf(str,"%s: %u\n",str2,count);
			delete[] str2;
			Send_Message_Team(Player_Type,255,255,255,str);
		}
	}
}

void JFW_Vehicle_Force_Composition_Zone_New::Timer_Expired(GameObject *obj,int number)
{
	reset = false;
}

JFW_Vehicle_Force_Composition_Zone_New::~JFW_Vehicle_Force_Composition_Zone_New()
{
	SAFE_DELETE_ARRAY(IDs);
}

void JFW_Resize_Zone::Created(GameObject *obj)
{
	ScriptZoneGameObj *object = ((ScriptableGameObj *)obj)->As_ScriptZoneGameObj();
	if (object)
	{
		Vector3 v = Get_Vector3_Parameter("Size");
		object->Get_Bounding_Box().Extent = v;
		float z = DEG_TO_RADF(Get_Float_Parameter("ZRotate"));
		object->Get_Bounding_Box().Basis.Rotate_Z(z);
	}
	Destroy_Script();
}

void JFW_Custom_Spawn_Zone::Created(GameObject *obj)
{
	zoneid = 0;
}

void JFW_Custom_Spawn_Zone::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("SpawnCustom"))
	{
		if (!zoneid)
		{
			Vector3 position = Get_Vector3_Parameter("Position");
			Vector3 size = Get_Vector3_Parameter("Size");
			Matrix3 rot;
			rot.Make_Identity();
			rot.Rotate_Z(DEG2RAD(Get_Float_Parameter("ZRotate")));
			OBBoxClass BoundingBox(position,size,rot);
			zoneid = Commands->Get_ID(Create_Zone(Get_Parameter("Preset"),BoundingBox));
		}
	}
	if (type == Get_Int_Parameter("DestroyCustom"))
	{
		if (zoneid)
		{
			Commands->Destroy_Object(Commands->Find_Object(zoneid));
			zoneid = 0;
		}
	}
}

void JFW_Spawn_Zone_Created::Created(GameObject *obj)
{
	zoneid = 0;
	Vector3 offset = Get_Vector3_Parameter("Position");
	Vector3 position = Commands->Get_Position(obj);
	position += offset;
	Vector3 size = Get_Vector3_Parameter("Size");
	Matrix3 rot;
	rot.Make_Identity();
	rot.Rotate_Z(DEG2RAD(Get_Float_Parameter("ZRotate")));
	OBBoxClass BoundingBox(position,size,rot);
	zoneid = Commands->Get_ID(Create_Zone(Get_Parameter("Preset"),BoundingBox));
}

void JFW_Spawn_Zone_Created::Killed(GameObject *obj,GameObject *killer)
{
	Commands->Destroy_Object(Commands->Find_Object(zoneid));
}

void JFW_Cash_Spy_Zone::Created(GameObject *obj)
{
	reset = false;
}

void JFW_Cash_Spy_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (Get_Vehicle(enterer) || reset)
	{
		return;
	}
	if (!Is_Script_Attached(enterer,Get_Parameter("Spy_Script")))
	{
		return;
	}
	int team = Commands->Get_Player_Type(enterer);
	char str[100];
	reset = true;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("ResetTime"),1);
	Create_2D_Sound_Team(Get_Parameter("Sound"),team);
	sprintf(str,"Current Enemy Credits: %.0f\n",Get_Team_Credits(PTTEAM(team)));
	Send_Message_Team(team,255,255,255,str);
}

void JFW_Cash_Spy_Zone::Timer_Expired(GameObject *obj,int number)
{
	reset = false;
}

void JFW_Power_Spy_Zone::Created(GameObject *obj)
{
	reset = false;
}

void JFW_Power_Spy_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (!Is_Script_Attached(enterer,Get_Parameter("Spy_Script")))
	{
		return;
	}
	team = Commands->Get_Player_Type(enterer);
	if (Is_Base_Powered(PTTEAM(team)) && !reset && Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_POWER_PLANT) && (!Is_Building_Dead(Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_POWER_PLANT))))
	{
		Create_2D_Sound_Team(Get_Parameter("Sound"),team);
		Power_Base(PTTEAM(team),false);
		GameObject *o = Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_COM_CENTER);
		if (o && !Is_Building_Dead(o))
		{
			Enable_Base_Radar(PTTEAM(team),false);
		}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Power_Time"),1);
	}
}

void JFW_Power_Spy_Zone::Timer_Expired(GameObject *obj,int number)
{
	if ((number == 1) && Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_POWER_PLANT) && (!Is_Building_Dead(Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_POWER_PLANT))))
	{
		Create_2D_Sound_Team(Get_Parameter("Sound2"),PTTEAM(team));
		Power_Base(PTTEAM(team),true);
		GameObject *o = Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_COM_CENTER);
		if (o && !Is_Building_Dead(o))
		{
			Enable_Base_Radar(PTTEAM(team),true);
		}
		reset = true;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Reset_Time"),2);
	}
	if (number == 2)
	{
		reset = false;
	}
}

void JFW_Cash_Spy_Zone_New::Created(GameObject *obj)
{
	reset = false;
}

void JFW_Cash_Spy_Zone_New::Entered(GameObject *obj,GameObject *enterer)
{
	if (Get_Vehicle(enterer) || reset)
	{
		return;
	}
	if (!Is_Spy(enterer))
	{
		return;
	}
	int team = Commands->Get_Player_Type(enterer);
	char str[100];
	reset = true;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("ResetTime"),1);
	Create_2D_Sound_Team(Get_Parameter("Sound"),team);
	sprintf(str,"Current Enemy Credits: %.0f\n",Get_Team_Credits(PTTEAM(team)));
	Send_Message_Team(team,255,255,255,str);
}

void JFW_Cash_Spy_Zone_New::Timer_Expired(GameObject *obj,int number)
{
	reset = false;
}

void JFW_Power_Spy_Zone_New::Created(GameObject *obj)
{
	reset = false;
}

void JFW_Power_Spy_Zone_New::Entered(GameObject *obj,GameObject *enterer)
{
	if (!Is_Spy(enterer))
	{
		return;
	}
	team = Commands->Get_Player_Type(enterer);
	if (Is_Base_Powered(PTTEAM(team)) && !reset && Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_POWER_PLANT) && (!Is_Building_Dead(Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_POWER_PLANT))))
	{
		Create_2D_Sound_Team(Get_Parameter("Sound"),team);
		Power_Base(PTTEAM(team),false);
		GameObject *o = Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_COM_CENTER);
		if (o && !Is_Building_Dead(o))
		{
			Enable_Base_Radar(PTTEAM(team),false);
		}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Power_Time"),1);
	}
}

void JFW_Power_Spy_Zone_New::Timer_Expired(GameObject *obj,int number)
{
	if ((number == 1) && Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_POWER_PLANT) && (!Is_Building_Dead(Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_POWER_PLANT))))
	{
		Create_2D_Sound_Team(Get_Parameter("Sound2"),PTTEAM(team));
		Power_Base(PTTEAM(team),true);
		GameObject *o = Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_COM_CENTER);
		if (o && !Is_Building_Dead(o))
		{
			Enable_Base_Radar(PTTEAM(team),true);
		}
		reset = true;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Reset_Time"),2);
	}
	if (number == 2)
	{
		reset = false;
	}
}

void JFW_Conyard_Spy_Zone::Created(GameObject *obj)
{
	reset = false;
	cydisabled = false;
}

void JFW_Conyard_Spy_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (!Is_Script_Attached(enterer,Get_Parameter("Spy_Script")))
	{
		return;
	}
	team = Commands->Get_Player_Type(enterer);
	if (!cydisabled && !reset && Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_CONYARD) && (!Is_Building_Dead(Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_CONYARD))))
	{
		Create_2D_Sound_Team(Get_Parameter("Sound"),team);
		cydisabled = true;
		Commands->Send_Custom_Event(obj,Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_CONYARD),3000,320023,0);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Power_Time"),1);
	}
}

void JFW_Conyard_Spy_Zone::Timer_Expired(GameObject *obj,int number)
{
	if ((number == 1) && Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_CONYARD) && (!Is_Building_Dead(Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_CONYARD))))
	{
		Create_2D_Sound_Team(Get_Parameter("Sound2"),PTTEAM(team));
		cydisabled = false;
		Commands->Send_Custom_Event(obj,Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_CONYARD),3000,230032,0);
		reset = true;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Reset_Time"),2);
	}
	if (number == 2)
	{
		reset = false;
	}
}

void JFW_Conyard_Spy_Zone_2::Created(GameObject *obj)
{
	reset = false;
	cydisabled = false;
}

void JFW_Conyard_Spy_Zone_2::Entered(GameObject *obj,GameObject *enterer)
{
	if (!Is_Script_Attached(enterer,Get_Parameter("Spy_Script")))
	{
		return;
	}
	team = Commands->Get_Player_Type(enterer);
	if (!cydisabled && !reset && Find_Construction_Yard(PTTEAM(team)) && (!Is_Building_Dead(Find_Construction_Yard(PTTEAM(team)))))
	{
		Create_2D_Sound_Team(Get_Parameter("Sound"),team);
		cydisabled = true;
		ConstructionYardGameObj *obj2 = (ConstructionYardGameObj *)Find_Construction_Yard(PTTEAM(team));
		obj2->Set_Spy(true);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Power_Time"),1);
	}
}

void JFW_Conyard_Spy_Zone_2::Timer_Expired(GameObject *obj,int number)
{
	if ((number == 1) && Find_Construction_Yard(PTTEAM(team)) && (!Is_Building_Dead(Find_Construction_Yard(PTTEAM(team)))))
	{
		Create_2D_Sound_Team(Get_Parameter("Sound2"),PTTEAM(team));
		cydisabled = false;
		ConstructionYardGameObj *obj2 = (ConstructionYardGameObj *)Find_Construction_Yard(PTTEAM(team));
		obj2->Set_Spy(false);
		reset = true;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Reset_Time"),2);
	}
	if (number == 2)
	{
		reset = false;
	}
}

void JFW_Conyard_Spy_Zone_2_New::Created(GameObject *obj)
{
	reset = false;
	cydisabled = false;
}

void JFW_Conyard_Spy_Zone_2_New::Entered(GameObject *obj,GameObject *enterer)
{
	if (!Is_Spy(enterer))
	{
		return;
	}
	team = Commands->Get_Player_Type(enterer);
	if (!cydisabled && !reset && Find_Construction_Yard(PTTEAM(team)) && (!Is_Building_Dead(Find_Construction_Yard(PTTEAM(team)))))
	{
		Create_2D_Sound_Team(Get_Parameter("Sound"),team);
		cydisabled = true;
		ConstructionYardGameObj *obj2 = (ConstructionYardGameObj *)Find_Construction_Yard(PTTEAM(team));
		obj2->Set_Spy(true);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Power_Time"),1);
	}
}

void JFW_Conyard_Spy_Zone_2_New::Timer_Expired(GameObject *obj,int number)
{
	if ((number == 1) && Find_Construction_Yard(PTTEAM(team)) && (!Is_Building_Dead(Find_Construction_Yard(PTTEAM(team)))))
	{
		Create_2D_Sound_Team(Get_Parameter("Sound2"),PTTEAM(team));
		cydisabled = false;
		ConstructionYardGameObj *obj2 = (ConstructionYardGameObj *)Find_Construction_Yard(PTTEAM(team));
		obj2->Set_Spy(false);
		reset = true;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Reset_Time"),2);
	}
	if (number == 2)
	{
		reset = false;
	}
}

void JFW_Destroy_Vehicle_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (!enterer->As_SmartGameObj())
	{
		return;
	}
	if (enterer->As_VehicleGameObj())
	{
		if (!Is_Script_Attached(enterer,"dp88_RemoteControlVehicle"))
		{
			Attach_Script_Occupants(enterer,"RA_DriverDeath", "0");
		}
		else
		{
			Attach_Script_Occupants(enterer,"JFW_Destroy_Vehicle_Zone_Marker", "0");
		}
	}
	if (!Is_Script_Attached(enterer,"JFW_Destroy_Vehicle_Zone_Marker"))
	{
		Commands->Apply_Damage(enterer,99999,"Death",0);
	}
	Remove_Script(enterer,"JFW_Destroy_Vehicle_Zone_Marker");
}

ScriptRegistrant<JFW_Look_At_Location_Entry> JFW_Look_At_Location_Entry_Registrant("JFW_Look_At_Location_Entry","Player_Type:int,Location:vector3");
ScriptRegistrant<JFW_Look_At_Object_Entry> JFW_Look_At_Object_Entry_Registrant("JFW_Look_At_Object_Entry","Player_Type:int,ObjectID:int");
ScriptRegistrant<JFW_Zone_PCT> JFW_Zone_PCT_Registrant("JFW_Zone_PCT","Player_Type:int");
ScriptRegistrant<JFW_3D_Sound_Team_Zone> JFW_3D_Sound_Team_Zone_Registrant("JFW_3D_Sound_Team_Zone","Player_Type:int,Sound:string");
ScriptRegistrant<JFW_3D_Sound_Player_Zone> JFW_3D_Sound_Player_Zone_Registrant("JFW_3D_Sound_Player_Zone","Player_Type:int,Sound:string");
ScriptRegistrant<JFW_2D_Sound_Team_Zone> JFW_2D_Sound_Team_Zone_Registrant("JFW_2D_Sound_Team_Zone","Player_Type:int,Sound:string");
ScriptRegistrant<JFW_2D_Sound_Player_Zone> JFW_2D_Sound_Player_Zone_Registrant("JFW_2D_Sound_Player_Zone","Player_Type:int,Sound:string");
ScriptRegistrant<JFW_Disable_Radar_Zone> JFW_Disable_Radar_Zone_Registrant("JFW_Disable_Radar_Zone","Player_Type:int");
ScriptRegistrant<JFW_Zone_Send_Custom_Multiple> JFW_Zone_Send_Custom_Multiple_Registrant("JFW_Zone_Send_Custom_Multiple","Player_Type:int,ID:int,Message:int,Count:int");
ScriptRegistrant<JFW_Zone_Send_Custom_Enter> JFW_Zone_Send_Custom_Enter_Registrant("JFW_Zone_Send_Custom_Enter","Player_Type:int,Message:int");
ScriptRegistrant<JFW_Zone_Send_Custom_Exit> JFW_Zone_Send_Custom_Exit_Registrant("JFW_Zone_Send_Custom_Exit","Player_Type:int,Message:int");
ScriptRegistrant<JFW_Vehicle_Zone_Animation> JFW_Vehicle_Zone_Animation_Registrant("JFW_Vehicle_Zone_Animation","Animation:string,Subobject:string,FirstFrame:float,LastFrame:float,Blended:int,UpAnimation:string,UpSubobject:string,UpFirstFrame:float,UpLastFrame:float,UpBlended:int,DownAnimation:string,DownSubobject:string,DownFirstFrame:float,DownLastFrame:float,DownBlended:int,Model:string");
ScriptRegistrant<JFW_Zone_Money_Preset> JFW_Zone_Money_Preset_Registrant("JFW_Zone_Money_Preset","Preset:string,Money:float,Player_Type:int,ID:int");
ScriptRegistrant<JFW_Fog_Zone> JFW_Fog_Zone_Registrant("JFW_Fog_Zone","Fog_Enable:int,Fog_Start_Distance=0.000:float,Fog_End_Distance=0.000:float,Delay=0.000:float,OnEnter=0:int,OnExit=0:int");
ScriptRegistrant<JFW_War_Blitz_Zone> JFW_War_Blitz_Zone_Registrant("JFW_War_Blitz_Zone","War_Blitz_Intensity=0.000:float,Start_Distance=0.000:float,End_Distance=1.000:float,War_Blitz_Heading=0.000:float,War_Blitz_Distribution=1.000:float,Delay=0.000:float,OnEnter=0:int,OnExit=0:int");
ScriptRegistrant<JFW_Create_Destroy_Object_On_Enter> JFW_Create_Destroy_Object_On_Enter_Registrant("JFW_Create_Destroy_Object_On_Enter","Location:vector3,Facing:float,Preset:string,Player_Type:int");
ScriptRegistrant<JFW_Apply_Damage_On_Enter_Timer> JFW_Apply_Damage_On_Enter_Timer_Registrant("JFW_Apply_Damage_On_Enter_Timer","Warhead:string,Damage:float,Time:float,Distance:float,Player_Type:int");
ScriptRegistrant<JFW_Blow_Up_On_Enter_Random> JFW_Blow_Up_On_Enter_Random_Registrant("JFW_Blow_Up_On_Enter_Random","Explosion:string,Probobility:int");
ScriptRegistrant<JFW_Blow_Up_On_Enter> JFW_Blow_Up_On_Enter_Registrant("JFW_Blow_Up_On_Enter","Explosion:string");
ScriptRegistrant<JFW_Apply_Damage_On_Enter> JFW_Apply_Damage_On_Enter_Registrant("JFW_Apply_Damage_On_Enter","Warhead:string,Damage:float");
ScriptRegistrant<JFW_Zone_Character> JFW_Zone_Character_Registrant("JFW_Zone_Character","Preset_Name:string,Player_Type:int");
ScriptRegistrant<JFW_Zone_Character_Swap> JFW_Zone_Character_Swap_Registrant("JFW_Zone_Character_Swap","Preset_Name:string,Old_Preset_Name:string,Player_Type:int");
ScriptRegistrant<JFW_Heavy_Vehicle_Damage_Zone> JFW_Heavy_Vehicle_Damage_Zone_Registrant("JFW_Heavy_Vehicle_Damage_Zone","Warhead:string,Damage:float,Time:float,Distance:float,Player_Type:int,Mass:float");
ScriptRegistrant<JFW_Light_Vehicle_Damage_Zone> JFW_Light_Vehicle_Damage_Zone_Registrant("JFW_Light_Vehicle_Damage_Zone","Warhead:string,Damage:float,Time:float,Distance:float,Player_Type:int,Mass:float");
ScriptRegistrant<JFW_2D_Sound_Zone> JFW_2D_Sound_Zone_Registrant("JFW_2D_Sound_Zone","Player_Type:int,Sound:string");
ScriptRegistrant<JFW_Stealth_Zone> JFW_Stealth_Zone_Registrant("JFW_Stealth_Zone","Player_Type:int,Stealth1:string,Stealth2:string,Stealth3:string,Stealth4:string");
ScriptRegistrant<JFW_Teleport_Zone_Team> JFW_Teleport_Zone_Team_Registrant("JFW_Teleport_Zone_Team","Location:vector3,Object_ID=0:int,Player_Type:int");
ScriptRegistrant<JFW_Character_Buy> JFW_Character_Buy_Registrant("JFW_Character_Buy","Preset_Name:string,Cost:int,Player_Type:int");
ScriptRegistrant<JFW_Preset_Buy> JFW_Preset_Buy_Registrant("JFW_Preset_Buy","Preset_Name:string,Cost:int,location:vector3,Player_Type:int");
ScriptRegistrant<JFW_Weapon_Buy> JFW_Weapon_Buy_Registrant("JFW_Weapon_Buy","Weapon_Name:string,Cost:int,Player_Type:int");
ScriptRegistrant<JFW_3D_Sound_Zone> JFW_3D_Sound_Zone_Registrant("JFW_3D_Sound_Zone","Player_Type:int,Sound:string");
ScriptRegistrant<JFW_Bounce_Zone_Entry> JFW_Bounce_Zone_Entry_Registrant("JFW_Bounce_Zone_Entry","Player_Type:int,Amount:float");
ScriptRegistrant<JFW_Grant_Key_Zone_Entry> JFW_Grant_Key_Zone_Entry_Registrant("JFW_Grant_Key_Zone_Entry","Player_Type:int,Key:int");
ScriptRegistrant<JFW_Zone_Send_Custom_Enable> JFW_Zone_Send_Custom_Enable_Registrant("JFW_Zone_Send_Custom_Enable","ID:int,EnterMessage:int,EnterParam:int,ExitMessage:int,ExitParam:int,Player_Type:int,Enable_Message:int,Disable_Message:int");
ScriptRegistrant<JFW_Zone_Send_Custom_Preset> JFW_Zone_Send_Custom_Preset_Registrant("JFW_Zone_Send_Custom_Preset","ID:int,EnterMessage:int,EnterParam:int,ExitMessage:int,ExitParam:int,Player_Type:int,Preset_Name:string");
ScriptRegistrant<JFW_Zone_Send_Custom_Enable_Enter> JFW_Zone_Send_Custom_Enable_Enter_Registrant("JFW_Zone_Send_Custom_Enable_Enter","EnterMessage:int,EnterParam:int,ExitMessage:int,ExitParam:int,Player_Type:int,Enable_Message:int,Disable_Message:int");
ScriptRegistrant<JFW_Zone_Send_Custom_Preset_Enter> JFW_Zone_Send_Custom_Preset_Enter_Registrant("JFW_Zone_Send_Custom_Preset_Enter","EnterMessage:int,EnterParam:int,ExitMessage:int,ExitParam:int,Player_Type:int,Preset_Name:string");
ScriptRegistrant<JFW_Send_Custom_Zone_Enter> JFW_Send_Custom_Zone_Enter_Registrant("JFW_Send_Custom_Zone_Enter","EnterMessage:int,EnterParam:int,ExitMessage:int,ExitParam:int,Team_ID:int");
ScriptRegistrant<JFW_Gate_Zone> JFW_Gate_Zone_Registrant("JFW_Gate_Zone","Player_Type:int,Time:float,TimerNum:int,ID:int,Animation:string");
ScriptRegistrant<JFW_Repair_Zone> JFW_Repair_Zone_Registrant("JFW_Repair_Zone","Player_Type:int,Repair_Distance:float,Repair_Health:float");
ScriptRegistrant<JFW_Repair_Zone_Aircraft_Only> JFW_Repair_Zone_Aircraft_Only_Registrant("JFW_Repair_Zone_Aircraft_Only","Player_Type:int,Repair_Distance:float,Repair_Health:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string");
ScriptRegistrant<JFW_Repair_Zone_No_Aircraft> JFW_Repair_Zone_No_Aircraft_Registrant("JFW_Repair_Zone_No_Aircraft","Player_Type:int,Repair_Distance:float,Repair_Health:float,Preset1:string,Preset2:string,Preset3:string,Preset4:string,Preset5:string,Preset6:string,Preset7:string,Preset8:string ");
ScriptRegistrant<JFW_Zone_Send_Custom_Not_Preset> JFW_Zone_Send_Custom_Not_Preset_Registrant("JFW_Zone_Send_Custom_Not_Preset","ID:int,EnterMessage:int,EnterParam:int,ExitMessage:int,ExitParam:int,Player_Type:int,Preset_Name:string");
ScriptRegistrant<JFW_Zone_Send_Custom_Keycard> JFW_Zone_Send_Custom_Keycard_Registrant("JFW_Zone_Send_Custom_Keycard","ID:int,EnterMessage:int,EnterParam:int,ExitMessage:int,ExitParam:int,Player_Type:int,Keycard_Number:int");
ScriptRegistrant<JFW_Heal_Zone> JFW_Heal_Zone_Registrant("JFW_Heal_Zone","Player_Type:int,Heal_Distance:float,Heal_Health:float");
ScriptRegistrant<JFW_Heal_Zone_2> JFW_Heal_Zone_2_Registrant("JFW_Heal_Zone_2","Player_Type:int,Heal_Distance:float,Heal_Health:float,Preset_Name:string");
ScriptRegistrant<JFW_Disable_Transitions_Zone> JFW_Disable_Transitions_Zone_Registrant("JFW_Disable_Transitions_Zone","");
ScriptRegistrant<JFW_Bounce_Zone_Entry_All_Directions> JFW_Bounce_Zone_Entry_All_Directions_Registrant("JFW_Bounce_Zone_Entry_All_Directions","Player_Type:int,ZAmount:float,Amount:float");
ScriptRegistrant<JFW_Zone_Send_Custom_Cost> JFW_Zone_Send_Custom_Cost_Registrant("JFW_Zone_Send_Custom_Cost","ID:int,Message:int,Param:int,Player_Type:int,Cost:int");
ScriptRegistrant<JFW_Vechicle_Damage_Zone> JFW_Vechicle_Damage_Zone_Registrant("JFW_Vechicle_Damage_Zone","Warhead:string,Damage:float,Time:float,Distance:float,Player_Type:int");
ScriptRegistrant<JFW_Refill_Buy> JFW_Refill_Buy_Registrant("JFW_Refill_Buy","Cost:int,Player_Type:int");
ScriptRegistrant<JFW_Vehicle_Buy> JFW_Vehicle_Buy_Registrant("JFW_Vehicle_Buy","Preset_Name:string,Cost:int,Player_Type:int");
ScriptRegistrant<JFW_Powerup_Buy> JFW_Powerup_Buy_Registrant("JFW_Powerup_Buy","Preset_Name:string,Cost:int,Player_Type:int");
ScriptRegistrant<JFW_Repair_Zone_No_VTOL> JFW_Repair_Zone_No_VTOL_Registrant("JFW_Repair_Zone_No_VTOL","Player_Type:int,Repair_Distance:float,Repair_Health:float");
ScriptRegistrant<JFW_Repair_Zone_VTOL_Only> JFW_Repair_Zone_VTOL_Only_Registrant("JFW_Repair_Zone_VTOL_Only","Player_Type:int,Repair_Distance:float,Repair_Health:float");
ScriptRegistrant<JFW_Zone_Vehicle> JFW_Zone_Vehicle_Registrant("JFW_Zone_Vehicle","Preset_Name:string,Player_Type:int");
ScriptRegistrant<JFW_Zone_Powerup> JFW_Zone_Powerup_Registrant("JFW_Zone_Powerup","Preset_Name:string,Player_Type:int");
ScriptRegistrant<JFW_Zone_Refill> JFW_Zone_Refill_Registrant("JFW_Zone_Refill","Player_Type:int");
ScriptRegistrant<JFW_Zone_Send_Custom_Team> JFW_Zone_Send_Custom_Team_Registrant("JFW_Zone_Send_Custom_Team","GDIID:int,GDIEnterMessage:int,GDIEnterParam:int,GDIExitMessage:int,GDIExitParam:int,NodID:int,NodEnterMessage:int,NodEnterParam:int,NodExitMessage:int,NodExitParam:int");
ScriptRegistrant<JFW_Zone_Animation> JFW_Zone_Animation_Registrant("JFW_Zone_Animation","Animation:string,Subobject:string,FirstFrame:float,LastFrame:float,Blended:int,UpAnimation:string,UpSubobject:string,UpFirstFrame:float,UpLastFrame:float,UpBlended:int,DownAnimation:string,DownSubobject:string,DownFirstFrame:float,DownLastFrame:float,DownBlended:int,Model:string");
ScriptRegistrant<JFW_Group_Purchase_Zone> JFW_Group_Purchase_Zone_Registrant("JFW_Group_Purchase_Zone","CashPerPerson:float,TotalCash:float,ID:int,Custom:int");
ScriptRegistrant<JFW_Zone_Timer> JFW_Zone_Timer_Registrant("JFW_Zone_Timer","Count:int,ID:int,Custom:int,Time:float,TimerNum:int");
ScriptRegistrant<JFW_Gate_Zone_2> JFW_Gate_Zone_2_Registrant("JFW_Gate_Zone_2","Player_Type:int,Time:float,TimerNum:int,ID:int,Animation:string");
ScriptRegistrant<JFW_PPAGE_Zone> JFW_PPAGE_Zone_Registrant("JFW_PPAGE_Zone","Player_Type:int,Message:string");
ScriptRegistrant<JFW_MSG_Zone> JFW_MSG_Zone_Registrant("JFW_MSG_Zone","Player_Type:int,Message:string");
ScriptRegistrant<JFW_TMSG_Zone> JFW_TMSG_Zone_Registrant("JFW_TMSG_Zone","Player_Type:int,Message:string");
ScriptRegistrant<JFW_Send_Driver_Custom_On_Enter> JFW_Send_Driver_Custom_On_Enter_Registrant("JFW_Send_Driver_Custom_On_Enter","EnterMessage:int,ExitMessage:int");
ScriptRegistrant<JFW_Teleport_Zone_Enable> JFW_Teleport_Zone_Enable_Registrant("JFW_Teleport_Zone_Enable","Location:vector3,Object_ID=0:int,Player_Type:int,EnableCustom:int,DisableCustom:int");
ScriptRegistrant<JFW_Radar_Spy_Zone> JFW_Radar_Spy_Zone_Registrant("JFW_Radar_Spy_Zone","Spy_Script:string,Sound:string");
ScriptRegistrant<JFW_Radar_Spy_Zone_New> JFW_Radar_Spy_Zone_New_Registrant("JFW_Radar_Spy_Zone_New","Sound:string");
ScriptRegistrant<JFW_2D_Sound_Zone_Team> JFW_2D_Sound_Zone_Team_Registrant("JFW_2D_Sound_Zone_Team","Player_Type:int,Sound:string,Time:float,TimerNum:int,Sound_Player_Type:int");
ScriptRegistrant<JFW_Repair_Zone_2> JFW_Repair_Zone_2_Registrant("JFW_Repair_Zone_2","Player_Type:int,Repair_Distance:float,Repair_Health:float,Sound:string,Credits:float,RepairSound:string,RepairStopSound:string,WaitForKeyhook=1:int");
ScriptRegistrant<JFW_Repair_Zone_No_Boats> JFW_Repair_Zone_No_Boats_Registrant("JFW_Repair_Zone_No_Boats","Player_Type:int,Repair_Distance:float,Repair_Health:float,Sound:string,Credits:float,RepairSound:string,RepairStopSound:string");
ScriptRegistrant<JFW_Repair_Zone_Boats> JFW_Repair_Zone_Boats_Registrant("JFW_Repair_Zone_Boats","Player_Type:int,Repair_Distance:float,Repair_Health:float,Sound:string,Credits:float,RepairSound:string,RepairStopSound:string");
ScriptRegistrant<JFW_Infantry_Force_Composition_Zone> JFW_Infantry_Force_Composition_Zone_Registrant("JFW_Infantry_Force_Composition_Zone","Spy_Script:string,Sound:string,ResetTime:float");
ScriptRegistrant<JFW_Vehicle_Force_Composition_Zone> JFW_Vehicle_Force_Composition_Zone_Registrant("JFW_Vehicle_Force_Composition_Zone","Spy_Script:string,Sound:string,ResetTime:float");
ScriptRegistrant<JFW_Infantry_Force_Composition_Zone_New> JFW_Infantry_Force_Composition_Zone_New_Registrant("JFW_Infantry_Force_Composition_Zone_New","Sound:string,ResetTime:float");
ScriptRegistrant<JFW_Vehicle_Force_Composition_Zone_New> JFW_Vehicle_Force_Composition_Zone_New_Registrant("JFW_Vehicle_Force_Composition_Zone_New","Sound:string,ResetTime:float");
ScriptRegistrant<JFW_Sell_Zone> JFW_Sell_Zone_Registrant("JFW_Sell_Zone","Player_Type:int,Sound:string,SellSound:string");
ScriptRegistrant<JFW_Resize_Zone> JFW_Resize_Zone_Registrant("JFW_Resize_Zone","Size:vector3,ZRotate:float");
ScriptRegistrant<JFW_Cash_Spy_Zone> JFW_Cash_Spy_Zone_Registrant("JFW_Cash_Spy_Zone","Spy_Script:string,Sound:string,ResetTime:float");
ScriptRegistrant<JFW_Power_Spy_Zone> JFW_Power_Spy_Zone_Registrant("JFW_Power_Spy_Zone","Spy_Script:string,Power_Time:float,Reset_Time:float,Sound:string,Sound2:string");
ScriptRegistrant<JFW_Cash_Spy_Zone_New> JFW_Cash_Spy_Zone_New_Registrant("JFW_Cash_Spy_Zone_New","Sound:string,ResetTime:float");
ScriptRegistrant<JFW_Power_Spy_Zone_New> JFW_Power_Spy_Zone_New_Registrant("JFW_Power_Spy_Zone_New","Power_Time:float,Reset_Time:float,Sound:string,Sound2:string");
ScriptRegistrant<JFW_Blow_Up_On_Enter_Delay> JFW_Blow_Up_On_Enter_Delay_Registrant("JFW_Blow_Up_On_Enter_Delay","Player_Type:int,Time:float,TimerNum:int,Explosion:string");
ScriptRegistrant<JFW_Conyard_Spy_Zone> JFW_Conyard_Spy_Zone_Registrant("JFW_Conyard_Spy_Zone","Spy_Script:string,Power_Time:float,Reset_Time:float,Sound:string,Sound2:string");
ScriptRegistrant<JFW_Conyard_Spy_Zone_2> JFW_Conyard_Spy_Zone_2_Registrant("JFW_Conyard_Spy_Zone_2","Spy_Script:string,Power_Time:float,Reset_Time:float,Sound:string,Sound2:string");
ScriptRegistrant<JFW_Conyard_Spy_Zone_2_New> JFW_Conyard_Spy_Zone_2_New_Registrant("JFW_Conyard_Spy_Zone_2_New","Power_Time:float,Reset_Time:float,Sound:string,Sound2:string");
ScriptRegistrant<JFW_Custom_Spawn_Zone> JFW_Custom_Spawn_Zone_Registrant("JFW_Custom_Spawn_Zone","Position:vector3,Size:vector3,ZRotate:float,SpawnCustom:int,DestroyCustom:int,Preset:string");
ScriptRegistrant<JFW_Spawn_Zone_Created> JFW_Spawn_Zone_Created_Registrant("JFW_Spawn_Zone_Created","Offset:vector3,Size:vector3,ZRotate:float,Preset:string");
ScriptRegistrant<JFW_Boat> JFW_Boat_Registrant("JFW_Boat","");
ScriptRegistrant<JFW_Destroy_Vehicle_Zone> JFW_Destroy_Vehicle_Zone_Registrant("JFW_Destroy_Vehicle_Zone","");
ScriptRegistrant<JFW_Destroy_Vehicle_Zone_Marker> JFW_Destroy_Vehicle_Zone_Marker_Registrant("JFW_Destroy_Vehicle_Zone_Marker","");
