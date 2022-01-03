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

#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "engine_DA.h"
#include "MoveablePhysClass.h"
#include "da.h"
#include "da_settings.h"
#include "da_parachutes.h"

void DAParachutesObserverClass::Init() {
	Get_Owner()->Get_Position(&LastPos);
	Start_Timer(1,0.1f);
}

void DAParachutesObserverClass::Timer_Expired(GameObject *obj,int Number) {
	HumanStateClass *State = ((SoldierGameObj*)Get_Owner())->Get_Human_State();
	Vector3 Pos;
	Get_Owner()->Get_Position(&Pos);
	if (Number == 1) { //Check if parachute should be deployed.
		if (State->Get_State() == HumanStateClass::AIRBORNE) { //Falling.
			if (State->Get_Jump_Transform().Get_Z_Translation() - Pos.Z >= 10) { //Deploy parachute after 10 meters.
				Matrix3D Transform = ((PhysicalGameObj*)Get_Owner())->Get_Transform();
				Transform.Rotate_Z(1.57f);
				Parachute = Create_Object("Soldier Powerups",Transform); //Powerups fall slower than infantry.
				Commands->Set_Model(Parachute,"X5D_Parachute");
				Commands->Attach_To_Object_Bone(Get_Owner(),Parachute,"Origin");
				Commands->Create_3D_WAV_Sound_At_Bone("parachute_open.wav",Get_Owner(),"Origin");
				Start_Timer(2,0.1f);
			}
			else {
				Start_Timer(1,0.1f);
			}
		}
		else {
			Set_Delete_Pending();
		}
	}
	else if (Number == 2) { //Check if landed.
		State->Set_Jump_Transform(((SoldierGameObj*)Get_Owner())->Get_Transform()); //Reset beginning of fall to current position to prevent fall damage.
		if (Pos.Z >= LastPos.Z || State->Get_State() != HumanStateClass::AIRBORNE) { //Landed.
			if (Parachute) {
				Parachute->Set_Delete_Pending();
				Commands->Create_3D_WAV_Sound_At_Bone("parachute_away.wav",Get_Owner(),"Origin");
			}
			Set_Delete_Pending();
		}
		else {
			LastPos = Pos;
			Start_Timer(2,0.1f);
		}
	}
}

bool DAParachutesObserverClass::Vehicle_Entry_Request(VehicleGameObj *Vehicle,int &Seat) {
	if (Parachute) { //Block vehicle entry if the parachute is deployed.
		return false;
	}
	Set_Delete_Pending();
	return true;
}

void DAParachutesObserverClass::Kill_Received(ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	Set_Delete_Pending();
}

DAParachutesObserverClass::~DAParachutesObserverClass() {
	if (Parachute) {
		DAGameObjManager::Set_GameObj_Delete_Pending(Parachute);
	}
}

void DAParachutesGameFeatureClass::Init() {
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Event(DAEvent::VEHICLEEXIT);
	Register_Chat_Command((DAECC)&DAParachutesGameFeatureClass::Parachute_Chat_Command,"!parachute|!para|!p");
}

void DAParachutesGameFeatureClass::Settings_Loaded_Event() {
	SingleSeat = DASettingsManager::Get_Bool("EnableSingleSeatParachutes",false);
}

void DAParachutesGameFeatureClass::Vehicle_Exit_Event(VehicleGameObj *Vehicle,cPlayer *Player,int Seat) {
	if (Vehicle->Get_Definition().Get_Type() == VEHICLE_TYPE_FLYING && (Vehicle->Get_Definition().Get_Seat_Count() > 1 || SingleSeat)) {
		Player->Get_GameObj()->Add_Observer(new DAParachutesObserverClass);
	}
}

bool DAParachutesGameFeatureClass::Parachute_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	DA::Page_Player(Player,"Your parachute will automatically deploy when you exit a %s. No command is needed.",SingleSeat?"flying vehicle":"multi-seat flying vehicle");
	return false;
}

Register_Game_Feature(DAParachutesGameFeatureClass,"Parachutes","EnableParachutes",0);