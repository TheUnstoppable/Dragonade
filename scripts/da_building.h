/*	Renegade Scripts.dll
    Dragonade Building Manager
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DABUILDING
#define INCLUDE_DABUILDING

#include "da_event.h"
#include "da_gameobj.h"

class DABuildingObserverClass : public DAGameObjObserverClass {
	virtual void Init();
	virtual void Timer_Expired(GameObject *obj,int Number);
	virtual void Damage_Received(ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	virtual void Kill_Received(ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	virtual void Custom(GameObject *obj, int Message, int Param, GameObject *Sender);
	virtual const char *Get_Name() {
		return "DABuildingObserverClass";
	}

	bool DestructionMessage;
	bool RepairMessage;
	bool AllowRepairMessage;
};

class DABuildingManager : public DAEventClass {
public:
	static void Init();

private:
	virtual void Settings_Loaded_Event();
	virtual void Object_Created_Event(GameObject *obj);
	virtual void Timer_Expired(int Number,unsigned int Data);
	virtual void Damage_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	virtual void Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);

	bool UnderAttackMessage[2];
};

#endif