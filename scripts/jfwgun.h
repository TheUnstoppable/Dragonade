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
class JFW_Building_Gun : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Weapon : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Building_Gun_Secondary : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Weapon_Secondary : public ScriptImpClass {
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_No_Aircraft : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Aircraft_Only : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Weapon_No_Aircraft : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Building_Gun_Weapon_Aircraft_Only : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Building_Gun_No_Aircraft_Secondary : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Aircraft_Only_Secondary : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Weapon_No_Aircraft_Secondary : public ScriptImpClass {
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Weapon_Aircraft_Only_Secondary : public ScriptImpClass {
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Weapon : public ScriptImpClass {
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Secondary : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Weapon_Secondary : public ScriptImpClass {
	bool primary;
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_No_Aircraft : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Aircraft_Only : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Weapon_No_Aircraft : public ScriptImpClass {
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Weapon_Aircraft_Only : public ScriptImpClass {
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_No_Aircraft_Secondary : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Aircraft_Only_Secondary : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Weapon_No_Aircraft_Secondary : public ScriptImpClass {
	bool primary;
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Weapon_Aircraft_Only_Secondary : public ScriptImpClass {
	bool primary;
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_No_VTOL : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_VTOL_Only : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Weapon_No_VTOL : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Building_Gun_Weapon_VTOL_Only : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Building_Gun_No_VTOL_Secondary : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_VTOL_Only_Secondary : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Weapon_No_VTOL_Secondary : public ScriptImpClass {
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Weapon_VTOL_Only_Secondary : public ScriptImpClass {
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_No_VTOL : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_VTOL_Only : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Weapon_No_VTOL : public ScriptImpClass {
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Weapon_VTOL_Only : public ScriptImpClass {
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_No_VTOL_Secondary : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_VTOL_Only_Secondary : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Weapon_No_VTOL_Secondary : public ScriptImpClass {
	bool primary;
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Weapon_VTOL_Only_Secondary : public ScriptImpClass {
	bool primary;
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound_Weapon : public ScriptImpClass {
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound_Secondary : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound_Weapon_Secondary : public ScriptImpClass {
	bool primary;
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound_No_Aircraft : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound_Aircraft_Only : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft : public ScriptImpClass {
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only : public ScriptImpClass {
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound_No_Aircraft_Secondary : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound_Aircraft_Only_Secondary : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound_Weapon_No_Aircraft_Secondary : public ScriptImpClass {
	bool primary;
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound_Weapon_Aircraft_Only_Secondary : public ScriptImpClass {
	bool primary;
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound_No_VTOL : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound_VTOL_Only : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL : public ScriptImpClass {
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only : public ScriptImpClass {
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound_No_VTOL_Secondary : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound_VTOL_Only_Secondary : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound_Weapon_No_VTOL_Secondary : public ScriptImpClass {
	bool primary;
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Gun_Animated_Sound_Weapon_VTOL_Only_Secondary : public ScriptImpClass {
	bool primary;
	int effectid;
	int attackid;
	bool enemyseen;
	bool lastframe;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};
