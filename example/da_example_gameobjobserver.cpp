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

#include "general.h"
#include "engine.h"
#include "engine_da.h"
#include "da.h"
#include "da_example_gameobjobserver.h"
#include "da_vehicle.h"

/*
GameObjObserverClass is the base of ScriptImpClass. It provides the various events, such as Custom and Destroyed, while the ScriptImpClass provides
the script parameters and means to create the observer. Scripts were never really intended for what we've used them for, though. They were intended
to be attached to objects in leveledit, not by other scripts or code. This makes them somewhat hard to work with if you need to create them directly
or pass complex parameters. For this reason scripts are no longer used anywhere in DA. In most cases a DAEventClass is used to implement a feature
that would have previously been implemented through a script attached to every object.

However, there are still a few places where script functionality is useful, such as crate effects or the vehicle ownership system. 
For this we have DAGameObjObserverClass.
It's an improved version of GameObjObserverClass that can be attached to an object similarly to ScriptImpClass. 
It includes extra events, better timer functionality, and an easier method of attaching to objects.

The following is used to attach a DAGameObjObserverClass:

obj->Add_Observer(new DAGameObjObserverClass);

See da_gameobj.h for the full list of events.
*/

//This replaces the Created and Attach events. Note that the gameobject is not passed to the events added by DA since you can get it with Get_Owner().
void DAExampleGameObjObserverClass::Init() {
	Start_Timer(1,5.0f); //Timers are created like this instead of with Commands->Start_Timer.
	Stop_Timer(1); //We also have this now.
}

//Use the destructor in place of Detach.
DAExampleGameObjObserverClass::~DAExampleGameObjObserverClass() {
}

//Same as stock event.
void DAExampleGameObjObserverClass::Destroyed(GameObject *obj) {
}

//Same as stock event.
void DAExampleGameObjObserverClass::Timer_Expired(GameObject *obj,int Number) {
}

//This allows you to block players from getting in the vehicle the observer is attached to, or change their seat.
bool DAExampleGameObjObserverClass::Vehicle_Entry_Request(cPlayer *Player,int &Seat) {
	if (Player->Get_Player_Type() != DAVehicleManager::Get_Team(Get_Owner())) { //Prevent the enemy from stealing this vehicle.
		return false;
	}
	return true;
}

//This allows you to block or change the damage done to the object the observer is attached to.
bool DAExampleGameObjObserverClass::Damage_Received_Request(ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type) {
	Damage *= 0.9f; //%10 damage reduction.
	return true;
}

//Called when the object the observer is attached to kills another object.
void DAExampleGameObjObserverClass::Kill_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
}
