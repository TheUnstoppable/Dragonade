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
#include "da_example_playerobserver.h"

/*
Player observers are similar to gameobject observers except that they attach to a player, not their gameobject. This means they can persist
through death, multiple levels, or the player leaving the game.
You do not need to register events. All events are automatically called.
Player observers have a higher priority than DAEventClasses, meaning that all events are triggered first for a player observer and then for the event
classes. This allows player observers to temporarily overwrite events, such as key hooks.
See da_player.h for the full list of events.
*/

void DAExamplePlayerObserverClass::Init() {
	//An example of the above note on priority. This will overwrite the vehicle ownership system's use of the key hook "VehStatus" for this player.
	Register_Key_Hook((DAPOKH)&DAExamplePlayerObserverClass::Refill_Key_Hook,"VehStatus");

	//This makes the observer persist through the player leaving and rejoining the game.
	//You can also use PERSISTMAP to make it persist through multiple maps.
	Add_Flag(DAPlayerFlags::PERSISTLEAVE);
}

void DAExamplePlayerObserverClass::Refill_Key_Hook() {
	Grant_Refill(Get_GameObj());
}

int DAExamplePlayerObserverClass::Character_Purchase_Request(float &Cost,const SoldierGameObjDef *Item) {
	Cost *= 0.9f; //10% discount on characters.
	return -1;
}

int DAExamplePlayerObserverClass::Vehicle_Purchase_Request(float &Cost,const VehicleGameObjDef *Item) {
	Cost *= 0.8f; //20% discount on vehicles.
	return -1;
}

int DAExamplePlayerObserverClass::PowerUp_Purchase_Request(float &Cost,const PowerUpGameObjDef *Item) {
	Cost *= 0.7f; //30% discount on powerups.
	return -1;
}

bool DAExamplePlayerObserverClass::Vehicle_Entry_Request(VehicleGameObj *Vehicle,int &Seat) {
	//This player can only enter the passenger seat of vehicles. If it's a one seater then the player is prohibited from entering.
	if (Seat == 0) {
		Seat = Find_Empty_Vehicle_Seat(Vehicle,false);
	}
	return true;
}

void DAExamplePlayerObserverClass::Vehicle_Enter(VehicleGameObj *Vehicle,int Seat) {
	//This player increases the health of any vehicle they enter by 10%
	Vehicle->Get_Defense_Object()->Set_Shield_Strength_Max(Vehicle->Get_Defense_Object()->Get_Shield_Strength_Max()*1.1f);
	Vehicle->Get_Defense_Object()->Set_Shield_Strength(Vehicle->Get_Defense_Object()->Get_Shield_Strength_Max());
}

void DAExamplePlayerObserverClass::Vehicle_Exit(VehicleGameObj *Vehicle,int Seat) {
	//Remove the increase when they exit.
	Vehicle->Get_Defense_Object()->Set_Shield_Strength_Max(Vehicle->Get_Definition().Get_DefenseObjectDef().ShieldStrengthMax);
}

bool DAExamplePlayerObserverClass::C4_Detonate_Request(C4GameObj *C4) {
	return false; //Don't trigger proximity C4.
}

//This is called each time the player receives a new gameobject.
void DAExamplePlayerObserverClass::Created() {
	//Another 10% health increase.
	Get_GameObj()->Get_Defense_Object()->Set_Health_Max(Get_GameObj()->Get_Defense_Object()->Get_Health_Max()*1.1f);
	Get_GameObj()->Get_Defense_Object()->Set_Health(Get_GameObj()->Get_Defense_Object()->Get_Health_Max());
}

//This is called when the player damages another object.
bool DAExamplePlayerObserverClass::Damage_Dealt_Request(DamageableGameObj *Victim,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type) {
	if (Type == DADamageType::SPLASH) {
		return false; //This player deals no splash damage.
	}
	return true;
}

//This is called when the player is damaged.
bool DAExamplePlayerObserverClass::Damage_Received_Request(ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type) {
	if (Type == DADamageType::SQUISH || Type == DADamageType::FALL) {
		return false; //This player cannot be squished and takes no fall damage.
	}
	return true;
}

//This is called when the player kills another object.
void DAExamplePlayerObserverClass::Kill_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	//Anything this player kills explodes.
	Commands->Create_Explosion("Explosion_Shell_Artillery",Commands->Get_Position(Victim),Get_GameObj());
}

//This is called when the player is killed.
void DAExamplePlayerObserverClass::Kill_Received(ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	//Anyone who kills this player gets a refill.
	Grant_Refill(Killer);
}
