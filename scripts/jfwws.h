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
class JFW_Building_Explode_No_Damage : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_Viceroid_Innate : public ScriptImpClass {
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_No_Innate : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Credit_Trickle : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_PCT_Pokable : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Killed(GameObject *obj,GameObject *killer);
	void Created(GameObject *obj);
};

class JFW_Vehicle_Regen : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Disable_Transition : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Enable_Transition : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Permanent_No_Falling_Damage : public ScriptImpClass {
	float health;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Death_Powerup : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_GrantPowerup_Created : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_CnC_Crate : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Tiberium_Refinery : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_GrantMoney_Powerup : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_GrantScore_Powerup : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Disable_Physical_Collision : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Enable_Physical_Collision : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_DestroyedStateObject : public ScriptImpClass {
	void Destroyed(GameObject *obj);
};

class JFW_Play_Sound : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Engine_Sound : public ScriptImpClass {
	int sound;
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
	public: void Register_Auto_Save_Variables();
};

class JFW_Mobius_Script : public ScriptImpClass {
	unsigned int follower;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_HealthMedal_TextMessage : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_ArmorMedal_TextMessage : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_C130_Explosion : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_Send_Object_ID : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Ion_Cannon_Sound : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_NukeStrike_Anim : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_PlayAnimation_DestroyObject : public ScriptImpClass {
	void Created(GameObject *obj);
	void Animation_Complete(GameObject *obj,const char *animation_name);
};

class JFW_BuildingStateSoundSpeaker : public ScriptImpClass {
	unsigned int sound;
	bool destroyed;
	bool explode;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_BuildingStateSoundController : public ScriptImpClass {
	unsigned int speakerid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Play_Sound_Object_Bone : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Cinematic_Primary_Killed : public ScriptImpClass {
	bool killed;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Destroyed(GameObject *obj);
	public: void Register_Auto_Save_Variables();
};

class JFW_Disable_Loiter : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_InnateIsStationary : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Generic_Conv : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Disable_Hibernation : public ScriptImpClass {
	void Created(GameObject *obj);
};
