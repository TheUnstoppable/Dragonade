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
class Neo_CreateObject_RandomTimer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class Neo_Vehicle_AI : public ScriptImpClass {
	unsigned int targetID;
	Vector3 position;
	bool rampaging;
	bool guarding;
	bool moving;
	bool hunting;
//	int move_priority;
//	int attack_priority;
	int face_priority;
	int current_priority;
	float guard_timer;
	float attack_timer;
	Vector3 mypos;
	int move_count;

	void Created(GameObject *obj);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Timer_Expired(GameObject *obj,int number);
	void Sound_Heard(GameObject *obj,const CombatSound & sound);
	public: void Register_Auto_Save_Variables();

	void Hunt(GameObject *obj,GameObject *target);
	void Guard(GameObject *obj,GameObject *target);
	GameObject *Select_Target(GameObject *obj,GameObject *target);
	GameObject *Set_Target(GameObject *target);
	GameObject *Get_Closest(GameObject *obj,GameObject *new_target,GameObject *old_target);
	bool Set_Ammo(GameObject *target);
	bool Set_Direction(GameObject *obj);

	/*********************************************/
	//May need a Rampage Timer
	//May need 'if (obj)' error checks in the Is_xxx engine.cpp stuff
};
