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
/******************Missle Silo Script Declarations*******************/

class RA_Missile_Silo : public ScriptImpClass {
	bool alive; 
	bool launching;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	void Killed(GameObject *obj, GameObject *killer);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
};

class RA_Missile_Manager : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
};

class RA_Silo_Animation : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class RA_ABomb_Terminal : public ScriptImpClass {
	bool active;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
};

class RA_ABomb_Beacon : public ScriptImpClass {
	void Created(GameObject *obj);
};

/****************Weather Change Script Declarations******************/

class RA_CanyonRiver_Weather : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject * sender);
	void Timer_Expired(GameObject *obj, int number);
};

class RA_Metro_Weather : public ScriptImpClass {
	bool max;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
};

/******************Underwater Script Declarations********************/

class RA_Underwater_OnCustom : public ScriptImpClass {
	bool allowed;
	bool underwater;
	bool driving; 
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class RA_Underwater_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject * enter); 
	void Exited(GameObject *obj,GameObject * exit);
};

/*****************Miscellaneous Script Declarations******************/

class RA_Submarine : public ScriptImpClass {
	int driverID;
	bool underwater;
	bool occupied;
	int blockers[4];
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	void Killed(GameObject *obj, GameObject *killer);
	void Destroyed(GameObject *obj);
	public: void Register_Auto_Save_Variables();
};

class RA_Vehicle : public ScriptImpClass {
	int IDarray[6];
	void Created(GameObject * obj);
	void Custom(GameObject * obj,int type,int param,GameObject * sender);
	void Destroyed(GameObject *obj);
	void Killed(GameObject * obj,GameObject * killer);
	public: void Register_Auto_Save_Variables();
};
