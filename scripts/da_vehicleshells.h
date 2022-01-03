/*	Renegade Scripts.dll
    Dragonade Vehicle Shells Game Feature
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAVEHICLESHELLS
#define INCLUDE_DAVEHICLESHELLS

#include "da_event.h"
#include "da_gamefeature.h"
#include "da_gameobj.h"
#include "HashTemplateClass.h"

struct DAVehicleShellStruct {
	VehicleGameObjDef *Def;
	StringClass Model;
	StringClass Explosion;
	float ZOffset;
};

class DAVehicleShellObserverClass : public DAGameObjObserverClass {
public:
	DAVehicleShellObserverClass(DAVehicleShellStruct *ShellDef,VehicleGameObj *Vehicle);
	virtual void Damage_Received(ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	virtual void Kill_Received(ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	virtual void Timer_Expired(GameObject *obj,int Number);
	~DAVehicleShellObserverClass();
	const char *Get_Name() {
		return "DAVehicleShellObserverClass";
	}
	
private:
	cPlayer *VOGFOwner;
	cPlayer *VMOwner;
	int Team;
	const VehicleGameObjDef *VehicleDef;
	DAVehicleShellStruct *ShellDef;
	Vector3 LastPos;
	float StartZ;
	ReferencerClass Shadow;
};

class DAVehicleShellShadowObserverClass : public DAGameObjObserverClass {
public:
	DAVehicleShellShadowObserverClass(GameObject *Shell);
	virtual bool Damage_Received_Request(ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type);
	virtual void Damage_Received(ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	const char *Get_Name() {
		return "DAVehicleShellShadowObserverClass";
	}

private:
	ReferencerClass Shell;
};

class DAVehicleShellsGameFeatureClass : public DAEventClass, public DAGameFeatureClass {
public:
	inline DAVehicleShellStruct *Get_Shell(VehicleGameObj *Vehicle) {
		return Shells.Get((unsigned int)&Vehicle->Get_Definition());
	}

private:
	virtual void Init();
	virtual void Settings_Loaded_Event();
	virtual void Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);

	HashTemplateClass<unsigned int,DAVehicleShellStruct> Shells;
};

#endif