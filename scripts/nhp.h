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
class NHP_Smart_Targetting : public ScriptImpClass {
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Created(GameObject *obj);
};

class NHP_Guard_Bot : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
};

class NHP_Construction_AI : public ScriptImpClass {
	int unitlim1;
	int unitlim2;
	int unitlim3;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class NHP_Sensor_Array_Zone : public ScriptImpClass {
	bool SensorIsAlive;
	int count;
	int soundId;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	public: void Register_Auto_Save_Variables();
};

class NHP_Sensorable_Object : public ScriptImpClass {
	bool insideSensorZone;
	int sensorZone;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};
