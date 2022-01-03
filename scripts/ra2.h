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
class Ra2Ven_MirageTank : public ScriptImpClass {
	int pilotID;
	float xpos;
	float ypos;
	float zpos;
	bool IsHided;
	void Created(GameObject *obj);
	public: void Register_Auto_Save_Variables();
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
};

class Ra2Ven_VehicleFalling : public ScriptImpClass {
	bool IsFalling;
	float xpos;
	float ypos;
	float zpos;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class Ra2Ven_DemoTruck : public ScriptImpClass {
	int pilotID;
	bool CanExplode;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	public: void Register_Auto_Save_Variables();
};

class Ra2Ven_Deployable_Vehicle : public ScriptImpClass {
	int pilotID;
	bool CanDestroyAnim;
	void Created(GameObject *obj);
	public: void Register_Auto_Save_Variables();
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
};

class Ra2Ven_Deploy_Animation : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class Ra2Ven_RandomTree : public ScriptImpClass {
	void Created(GameObject *obj);
};

class Ra2Ven_TurretSound : public ScriptImpClass {
	int TurretId;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	public: void Register_Auto_Save_Variables();
};

class Ra2Ven_TurretSoundObj : public ScriptImpClass {
	int LastOrientation;
	bool CanPlay;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class Ra2Ven_OccupentWeapon : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class Ra2Ven_FireAnimation : public ScriptImpClass {
	float InitDistance;
	bool CanPlayAnim;
	int	AnimMode;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	public: void Register_Auto_Save_Variables();
};

class Ra2Ven_FireAnimation2 : public ScriptImpClass {
	float InitDistance;
	bool CanPlayAnim;
	int CurAnim;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	public: void Register_Auto_Save_Variables();
};

class Ra2Ven_Dplbl_Vhcls_Cursor : public ScriptImpClass {
	int pilotID;
	bool CanDestroyAnim;
	bool IsOnDeploy;
	void Created(GameObject *obj);
	public: void Register_Auto_Save_Variables();
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
};
