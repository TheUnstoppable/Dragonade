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

#ifndef INCLUDE_DAEXAMPLE_PLAYEROBSERVER
#define INCLUDE_DAEXAMPLE_PLAYEROBSERVER

#include "da_player.h" //Included for DAPlayerObserverClass definition.

//Inherit from DAPlayerObserverClass.
class DAExamplePlayerObserverClass : public DAPlayerObserverClass {
	virtual void Init();
	void Refill_Key_Hook();
	virtual const char *Get_Observer_Name() { return "DAPlayerObserverExampleClass"; } //This is required.
	virtual int Character_Purchase_Request(float &Cost,const SoldierGameObjDef *Item);
	virtual int Vehicle_Purchase_Request(float &Cost,const VehicleGameObjDef *Item);
	virtual int PowerUp_Purchase_Request(float &Cost,const PowerUpGameObjDef *Item);
	virtual bool Vehicle_Entry_Request(VehicleGameObj *Vehicle,int &Seat);
	virtual void Vehicle_Enter(VehicleGameObj *Vehicle,int Seat);
	virtual void Vehicle_Exit(VehicleGameObj *Vehicle,int Seat);
	virtual bool C4_Detonate_Request(C4GameObj *C4);
	virtual void Created();
	virtual bool Damage_Dealt_Request(DamageableGameObj *Victim,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type);
	virtual bool Damage_Received_Request(ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type);
	virtual void Kill_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	virtual void Kill_Received(ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
};

#endif