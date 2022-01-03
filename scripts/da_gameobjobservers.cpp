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

#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "engine_DA.h"
#include "da.h"
#include "da_gameobjobservers.h"


DATimedInvincibilityObserverClass::DATimedInvincibilityObserverClass(float ProtectTime) {
	this->ProtectTime = ProtectTime;
}

void DATimedInvincibilityObserverClass::Init() {
	Start_Timer(1,ProtectTime);
}

bool DATimedInvincibilityObserverClass::Damage_Received_Request(ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type) {
	if (Type != DADamageType::REPAIR) {
		return false;
	}
	return true;
}

void DATimedInvincibilityObserverClass::Timer_Expired(GameObject *obj,int Number) {
	Set_Delete_Pending();
}

/********************************************************************************************************************************/

DAAttachToObjectWithZOffsetObserverClass::DAAttachToObjectWithZOffsetObserverClass(GameObject *Host,float Offset,float SyncTime) {
	this->Host = Host;
	this->Offset = Offset;
	this->SyncTime = SyncTime;
}

void DAAttachToObjectWithZOffsetObserverClass::Init() {
	Timer_Expired(Get_Owner(),1);
}

void DAAttachToObjectWithZOffsetObserverClass::Timer_Expired(GameObject *obj,int Number) {
	if (Host) {
		Commands->Set_Position(Get_Owner(),((PhysicalGameObj*)Host.Get_Ptr())->Get_Position()+Vector3(0.0f,0.0f,Offset));
		Start_Timer(1,SyncTime);
	}
	else {
		Get_Owner()->Set_Delete_Pending();
	}
}

/********************************************************************************************************************************/

void DANoPointsUntilEnteredObserverClass::Init() {
	DefenseObjectClass *Defense = ((DamageableGameObj*)Get_Owner())->Get_Defense_Object();
	DeathPoints = Defense->Get_Death_Points();
	DamagePoints = Defense->Get_Damage_Points();
	Defense->Set_Death_Points(0.0f);
	Defense->Set_Damage_Points(0.0f);
}

void DANoPointsUntilEnteredObserverClass::Vehicle_Enter(cPlayer *Player,int Seat) {
	DefenseObjectClass *Defense = ((DamageableGameObj*)Get_Owner())->Get_Defense_Object();
	Defense->Set_Death_Points(DeathPoints);
	Defense->Set_Damage_Points(DamagePoints);
	Set_Delete_Pending();
}



