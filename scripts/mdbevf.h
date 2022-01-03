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

#define NOT_DISABLED 0
#define DISABLED_EVF 1 
#define DISABLED_UNIT 2
#define DISABLED_BOTH 3

#define VEHTYPE_GROUND 1
#define VEHTYPE_FLYING 2
#define VEHTYPE_NAVAL 3
#define VEHTYPE_SUPPORT 4

#include "PurchaseSettingsDefClass.h"
struct UnitInfo {
	UnitInfo(PurchaseSettingsDefClass::TYPE type,unsigned int pos,unsigned int preset,bool alternate) : Type(type), Pos(pos), Preset(preset), Alternate(alternate), Support(false)
	{
		DMode = NOT_DISABLED;
	}
	PurchaseSettingsDefClass::TYPE Type;
	unsigned int Pos;
	unsigned int Preset;
	unsigned int VehType;
	unsigned int DMode;
	bool Alternate;
	bool Support;
};

struct CreationPos {
	unsigned int Preset;
	GameObject *Pos;
};

class ExpVehFacClass {
	SimpleDynVecClass<UnitInfo*> Units;
	SimpleDynVecClass<CreationPos*> Naval_Creation_Pos_Per_Vehicle;
	int Last_Flying_Landing_Pos;
	int Last_Naval_Creation_Pos;
	unsigned int Team;
	int PHookID;
	int OHookID;
	ObjectCreateHookStruct *OHookStruct;
	bool Is_In_Vector(SimpleDynVecClass<unsigned int> &Vec, unsigned int Preset);
	Vector3 Get_Flying_Landing_Position();
	GameObject *Get_Naval_Creation_Position(unsigned int Preset);
	GameObject *Script;
	bool Using_Naval;

public:
	unsigned int Ground_Limit;
	unsigned int Flying_Limit;
	unsigned int Naval_Limit;
	unsigned int Support_Limit;
	unsigned int Ground_Amount;
	unsigned int Flying_Amount;
	unsigned int Naval_Amount;
	unsigned int Support_Amount;
	SimpleDynVecClass<GameObject*> *Naval_Creation_Positions;
	SimpleDynVecClass<GameObject*> *Flying_Landing_Positions;
	float Ground_Build_Time[2];
	float Flying_Build_Time[2];
	float Naval_Build_Time[2];
	bool Is_Ground_Building;
	bool Is_Flying_Building;
	bool Is_Naval_Building;
	bool Loaded;
	unsigned int LastFlyVehID;
	unsigned int LastFlyVehOwner;

	ExpVehFacClass(unsigned int team);
	~ExpVehFacClass();
	void Set_Ground_Currently_Building(bool building);
	void Set_Flying_Currently_Building(bool building);
	void Set_Naval_Currently_Building(bool building);
	void Set_Ground_Limit_Reached(bool Reached);
	void Set_Flying_Limit_Reached(bool Reached);
	void Set_Naval_Limit_Reached(bool Reached);
	void Set_Support_Limit_Reached(bool Reached);
	UnitInfo *Get_Unit_Info(unsigned int Preset);
	void Load(const char *NavalVehicles, const char *NavalCreationPresets,GameObject *script,const char *SupportVehicles = 0);
	void Clear();
	static int Purchase_Hook(BaseControllerClass *base, GameObject *purchaser, unsigned int cost, unsigned int preset,const char *data);
	static void Object_Hook(void *,GameObject *obj);
};

extern ExpVehFacClass *ExpVehFac[2];

class MDB_ExpVehFac_Helipad : public ScriptImpClass {
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	void Destroyed(GameObject *obj);
};

class MDB_ExpVehFac_Naval : public ScriptImpClass {
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	void Destroyed(GameObject *obj);
};

class MDB_ExpVehFac_Vehicle_Factory : public ScriptImpClass {
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	void Destroyed(GameObject *obj);
};

class MDB_ExpVehFac_Cinematic : public ScriptImpClass {
	int ID;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
};

class MDB_ExpVehFac_Cinematic_Vehicle : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float amount);
};

class MDB_ExpVehFac_Limit : public ScriptImpClass {
	int Team;
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
	public: void Register_Auto_Save_Variables();
};
