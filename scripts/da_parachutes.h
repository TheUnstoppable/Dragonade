/*	Renegade Scripts.dll
    Dragonade Parachutes Game Feature
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAPARACHUTES
#define INCLUDE_DAPARACHUTES

#include "da_event.h"
#include "da_gamefeature.h"
#include "da_gameobj.h"

class DAParachutesObserverClass : public DAGameObjObserverClass {
	virtual void Init();
	virtual const char *Get_Name() { return "DAParachutesObserverClass"; }
	virtual void Timer_Expired(GameObject *obj,int Number);
	virtual bool Vehicle_Entry_Request(VehicleGameObj *Vehicle,int &Seat);
	virtual void Kill_Received(ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	virtual ~DAParachutesObserverClass();
	ReferencerClass Parachute;
	Vector3 LastPos;
};

class DAParachutesGameFeatureClass : public DAEventClass, public DAGameFeatureClass {
	virtual void Init();
	virtual void Settings_Loaded_Event();
	virtual void Vehicle_Exit_Event(VehicleGameObj *Vehicle,cPlayer *Player,int Seat);
	bool Parachute_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);

	//Settings
	bool SingleSeat;
};

#endif