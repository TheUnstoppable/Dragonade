/*	Renegade Scripts.dll
    Dragonade Example Plugin
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAEXAMPLE_GAMEOBJOBSERVER
#define INCLUDE_DAEXAMPLE_GAMEOBJOBSERVER

#include "da_gameobj.h" //For DAGameObjObserverClass definition.

//Inherit from DAGameObjObserverClass.
class DAExampleGameObjObserverClass : public DAGameObjObserverClass {
	virtual void Init();
	~DAExampleGameObjObserverClass();
	virtual void Destroyed(GameObject *obj);
	virtual void Timer_Expired(GameObject *obj,int Number);
	virtual bool Vehicle_Entry_Request(cPlayer *Player,int &Seat);
	virtual bool Damage_Received_Request(ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type);
	virtual void Kill_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	const char *Get_Name() { return "DAExampleObserverClass"; } //This is required.
};

#endif