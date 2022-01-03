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
class SUR_Spawn_Delay : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

//Game Type
class SUR_Generator_Zone : public ScriptImpClass {
	int energy;
	int curEmitter;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Entered(GameObject *obj,GameObject *enterer);
	public: void Register_Auto_Save_Variables();
};

class SUR_MiniGen : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *killer);
};

class SUR_NHB : public ScriptImpClass {
	int vpadAttackers;
	int GDIGenAttackers;
	int NodGenAttackers;
	int heliAttackers;
	int jobRequests;
	int Targets[11];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class SUR_NHB_Soldier : public ScriptImpClass {
	int command;
	Vector3 oldPos;
	bool atDes;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class SUR_Obj_Destroyer : public ScriptImpClass {
	void Death(GameObject *obj,GameObject *killer);
};

class SUR_New_PT : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class SUR_New_PT_Reciever : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class SUR_APC_AI : public ScriptImpClass {
	int aiCount;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class SUR_Orca_AI : public ScriptImpClass {
	void Created(GameObject *obj);
	void Death(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
};

class SUR_Chinook_AI : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class SUR_Dep_Turret : public ScriptImpClass {
	int id1;
	int id2;
	int id3;
	int objtype;
	void Created(GameObject *obj);
	void EnemySeen(GameObject *obj,GameObject *enemy);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class SUR_Timed_Death : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class SUR_GrantPowerup_OnEntry : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};
