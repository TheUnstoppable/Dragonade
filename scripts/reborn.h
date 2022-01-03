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

class Reborn_Game_Manager : public ScriptImpClass {
	int MMK2lim;
	int Cyborglim;
	void Created(GameObject *obj);
	public: void Register_Auto_Save_Variables();
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class Reborn_Cyborg : public ScriptImpClass {
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class Reborn_MMK2 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class Reborn_Deployable_Vehicle : public ScriptImpClass {
	int pilotID;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class Reborn_Deploy_Animation : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class Reborn_MMK2_Turret_Spawn : public ScriptImpClass {
	int turID;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	public: void Register_Auto_Save_Variables();
};

class Reborn_Diggable_Vehicle : public ScriptImpClass {
	int pilotID;
	int ug;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	public: void Register_Auto_Save_Variables();
};

class Reborn_Diggable_Vehicle_Animated : public ScriptImpClass {
	int pilotID;
	int ug;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
	public: void Register_Auto_Save_Variables();
};

class Reborn_PlaySound_OnCreate : public ScriptImpClass {
	void Created(GameObject *obj);
};

class Reborn_Deployable_Vehicle_Player : public JFW_Key_Hook_Base {
	void Created(GameObject *obj);
	void KeyHook();
};

class Reborn_IsDeployableMech : public ScriptImpClass {
	int PilotID,mode,Mode,LastDir;
	bool enabled,play,forward,backward;
	float xpos;
	float ypos;
	float zpos;
	void Created(GameObject *obj);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	public: void Register_Auto_Save_Variables();
};

class Reborn_IsDeployableTank : public ScriptImpClass {
	int PilotID,mode;
	void Created(GameObject *obj);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
	public: void Register_Auto_Save_Variables();
};

class Reborn_IsDeployableTank_2 : public ScriptImpClass {
	int PilotID,mode;
	void Created(GameObject *obj);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
	public: void Register_Auto_Save_Variables();
};

class Reborn_IsDeployableTank_3 : public ScriptImpClass {
	int PilotID,mode;
	void Created(GameObject *obj);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
	public: void Register_Auto_Save_Variables();
};

class Reborn_IsDeployableTank_4 : public ScriptImpClass {
	int PilotID, mode;
	void Created(GameObject *obj);
	void Animation_Complete(GameObject *obj, const char *animation_name);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	void Destroyed(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
public: void Register_Auto_Save_Variables();
};

class Reborn_IsMech : public ScriptImpClass {
	bool enabled,forward,backward,play;
	float xpos;
	float ypos;
	float zpos;
	int Mode,LastDir;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	public: void Register_Auto_Save_Variables();
};
