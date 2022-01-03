/*	Renegade Scripts.dll
    Dragonade C4 and Beacon Manager
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAC4BEACON
#define INCLUDE_DAC4BEACON

#include "da_event.h"

class DAC4BeaconManager : public DAEventClass {
public:
	static void Init();

private:
	virtual void Settings_Loaded_Event();
	virtual void Object_Created_Event(GameObject *obj);
	virtual void Beacon_Deploy_Event(BeaconGameObj *Beacon);
	virtual void Beacon_Detonate_Event(BeaconGameObj *Beacon);
	virtual void C4_Detonate_Event(C4GameObj *C4);
	virtual void Poke_Event(cPlayer *Player,PhysicalGameObj *obj);
	virtual void Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	virtual int PowerUp_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const PowerUpGameObjDef *Item);
	
	bool BlockFakeBeacons;
};

#endif