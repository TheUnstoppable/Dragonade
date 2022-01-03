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
#pragma once
#include "PurchaseSettingsDefClass.h"
#include "TeamPurchaseSettingsDefClass.h"

class MDB_Weapon_Scope : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
	int CurrScope;
};

class MDB_Weapon_Scope_Global : public JFW_Object_Created_Hook_Base {
	void Created(GameObject *obj);
	void ObjectCreateHook(GameObject *obj);
	char Params[10];
};

class MDB_Change_Spawn_Char_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class MDB_Change_Spawn_Char_Custom : public ScriptImpClass {
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
};

class MDB_ConYard : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	public: void Register_Auto_Save_Variables();
	int PMode;
	float Time,Amount;
	bool Self;
};

class MDB_Send_Custom_Enemy_Seen : public ScriptImpClass {
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
};

class MDB_Water_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class MDB_Water_Unit : public ScriptImpClass {
};

class MDB_Vehicle_Limit : public ScriptImpClass {
	void Created(GameObject *obj);
};

class MDB_Mine_Limit : public ScriptImpClass {
	void Created(GameObject *obj);
};

class MDB_Unit_Limit : public ScriptImpClass {
	bool Free;
	bool Disabled;
	unsigned int Position;
	PurchaseSettingsDefClass::TYPE Type;
	PurchaseSettingsDefClass::TEAM Team;
	unsigned int Definitions[4];
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	public: void Register_Auto_Save_Variables();
	void ReEnable();
};

class MDB_Send_Custom_On_Key : public JFW_Key_Hook_Base {
	void Created(GameObject *obj);
	void KeyHook();
};

class MDB_Remote_Controlled_Vehicle : public ScriptImpClass {
	int BotID;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	public: void Register_Auto_Save_Variables();
};


class MDB_Remote_Controlled_Vehicle_Bot : public ScriptImpClass {
	int ID,VehID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Killed(GameObject *obj,GameObject *killer);
	public: void Register_Auto_Save_Variables();
};

class MDB_Sidebar_Key : public ScriptImpClass {
};

class MDB_Set_Ammo_Granted_Weapon_On_Pickup : public ScriptImpClass {
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
};

class MDB_Set_Ammo_Current_Weapon_On_Pickup : public ScriptImpClass {
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
};

class MDB_Set_Ammo_On_Pickup : public ScriptImpClass {
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
};
