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

class MDB_Base_Defense_Popup_No_VTOL : public ScriptImpClass {
public:
	int State,LastSeen,ReturnTime;
	unsigned int LastAttack;
	float MaxDis;
	bool AdjustInf;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	public: void Register_Auto_Save_Variables();
};

class MDB_Base_Defense_Popup_No_VTOL_No_Stealth : public MDB_Base_Defense_Popup_No_VTOL {
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
};

class MDB_Base_Defense_Popup_VTOL_Only : public ScriptImpClass {
	int State,LastSeen,ReturnTime;
	unsigned int LastAttack;
	float MaxDis;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	public: void Register_Auto_Save_Variables();
};

class MDB_Base_Defense_Popup : public ScriptImpClass {
	int State,LastSeen,ReturnTime;
	unsigned int LastAttack;
	float MaxDis;
	bool AdjustInf;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	public: void Register_Auto_Save_Variables();
};

class MDB_Base_Defense : public ScriptImpClass {
	Vector3 V[4];
	float MaxDis;
	bool AdjustInf;
	int LastSeen;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class MDB_Base_Defense_No_VTOL : public ScriptImpClass {
	Vector3 V[4];
	float MaxDis;
	bool AdjustInf;
	int LastSeen;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class MDB_Base_Defense_VTOL_Only : public ScriptImpClass {
	Vector3 V[4];
	int LastSeen;
	float MaxDis;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};
