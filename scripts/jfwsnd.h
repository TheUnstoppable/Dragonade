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
class JFW_2D_Sound_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_2D_Sound_Timer_Random : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_3D_Sound_Timer_Random : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_2D_Sound_Timer_Health : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_2D_Sound_Damage : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	public: void Register_Auto_Save_Variables();
};

class JFW_3D_Sound_Damage : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	public: void Register_Auto_Save_Variables();
};

class JFW_2D_Sound_Damage_Range : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	public: void Register_Auto_Save_Variables();
};

class JFW_2D_Sound_Damage_Range_Team : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	public: void Register_Auto_Save_Variables();
};

class JFW_2D_Sound_Damage_Range_Team_String : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	public: void Register_Auto_Save_Variables();
};

class JFW_3D_Sound_Damage_Range : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	public: void Register_Auto_Save_Variables();
};

class JFW_Set_Background_Music_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Fade_Background_Music_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Stop_Background_Music_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Set_Background_Music_On_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Fade_Background_Music_On_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Stop_Background_Music_On_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_3D_Sound_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_2D_Sound_Startup : public ScriptImpClass {
	void Created(GameObject *obj);
};

struct TimeRemainingSound {
	char *sound;
	int minutes;
	bool played;
};

class JFW_Time_Remaining_Sounds : public ScriptImpClass {
	TimeRemainingSound *Sounds;
	int SoundCount;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
public:
	JFW_Time_Remaining_Sounds();
	~JFW_Time_Remaining_Sounds();
};

class JFW_Time_Remaining_Sounds_2 : public ScriptImpClass {
	TimeRemainingSound *Sounds;
	int SoundCount;
	int team;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
public:
	JFW_Time_Remaining_Sounds_2();
	~JFW_Time_Remaining_Sounds_2();
};

class JFW_2D_Sound_Death_Team : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_Vehicle_Full_Sound : public ScriptImpClass {
	bool full;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_C4_Sound : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_C4_Sound_2 : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Repair_Complete_Sound : public ScriptImpClass {
	bool damaged;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float amount);
};
