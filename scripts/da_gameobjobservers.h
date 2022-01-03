/*	Renegade Scripts.dll
    Dragonade Misc. Game Object Observers
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAGAMEOBJOBSERVERS
#define INCLUDE_DAGAMEOBJOBSERVERS

#include "da_gameobj.h"

//Makes an object invincible for ProtectTime.
class DA_API DATimedInvincibilityObserverClass : public DAGameObjObserverClass {
public:
	DATimedInvincibilityObserverClass(float ProtectTime);
	virtual void Init();
	virtual bool Damage_Received_Request(ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type);
	virtual void Timer_Expired(GameObject *obj,int Number);
	virtual const char *Get_Name() { 
		return "DATimedInvincibilityObserverClass"; 
	}

private:
	float ProtectTime;
};

//Attach an object to Host with a Z offset.
class DA_API DAAttachToObjectWithZOffsetObserverClass : public DAGameObjObserverClass {
public:
	DAAttachToObjectWithZOffsetObserverClass(GameObject *Host,float Offset,float SyncTime);
	virtual void Init();
	virtual void Timer_Expired(GameObject *obj,int Number);
	virtual const char *Get_Name() { 
		return "DAAttachToObjectWithZOffsetObserverClass"; 
	}

private:
	ReferencerClass Host;
	float Offset;
	float SyncTime;
};

//Clears a vehicle's death and damage points until it is entered.
class DA_API DANoPointsUntilEnteredObserverClass : public DAGameObjObserverClass {
	virtual void Init();
	virtual void Vehicle_Enter(cPlayer *Player,int Seat);
	virtual const char *Get_Name() { 
		return "DANoPointsUntilEnteredObserverClass"; 
	}

	float DeathPoints;
	float DamagePoints;
};

#endif