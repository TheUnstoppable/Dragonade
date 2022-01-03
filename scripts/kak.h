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
class KAK_Warp_On_Pickup : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class KAK_Precipitation_Create : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class KAK_Precipitation_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	void Timer_Expired(GameObject *obj,int number);
};

class KAK_Wind_Create : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class KAK_Wind_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	void Timer_Expired(GameObject *obj,int number);
};

class KAK_Clouds_Create : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class KAK_Clouds_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	void Timer_Expired(GameObject *obj,int number);
};

class KAK_Lightning_Create : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class KAK_Lightning_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	void Timer_Expired(GameObject *obj,int number);
};

class KAK_Flowing_Weather : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
	float Range,CurrentClouds,CurrentPrecipitation,CurrentLightning,CurrentWind,CurrentWindHeading;
};

class KAK_Give_Powerup_On_Pickup : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class KAK_Select_Weapon_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
};

class KAK_Select_Weapon_On_Pickup : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class KAK_Harvester_Logic : public ScriptImpClass {
	int HarvyID;
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class KAK_Harvester_Kill : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class KAK_Harvester_Spawn_Kill : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class KAK_Harvester_Attached : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
	bool End_Timer;
	float facing;
	int CrystalNumber;
	int PathNumber;
	int Crystal1,Crystal2,Crystal3,Crystal4,Crystal5,Crystal6,Crystal7,Crystal8,Crystal9,Crystal10;
};

class KAK_Prevent_Kill : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Killed(GameObject *obj,GameObject *killer);
};

class KAK_Convert_Visceroid : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *killer);
};

class KAK_Freeze_Object : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
	float elapsedtime;
	float xpos;
	float ypos;
	float zpos;
};

class KAK_Regen_Custom : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};
