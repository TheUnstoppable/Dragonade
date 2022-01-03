/*	Renegade Scripts.dll
    Dragonade Gamelog
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#include "General.h"
#include "engine.h"
#include "engine_da.h"
#include "da.h"
#include "da_gamelog.h"
#include "da_translation.h"
#include "da_log.h"

void DAGameLogGameFeatureClass::Init() {
	Register_Event(DAEvent::LEVELLOADED);
	Register_Event(DAEvent::GAMEOVER);
	Register_Event(DAEvent::VEHICLEENTER);
	Register_Event(DAEvent::VEHICLEEXIT);
	Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::SOLDIER | DAObjectEvent::VEHICLE | DAObjectEvent::C4 | DAObjectEvent::BEACON | DAObjectEvent::BUILDING);
	Register_Object_Event(DAObjectEvent::DESTROYED,DAObjectEvent::SOLDIER | DAObjectEvent::VEHICLE | DAObjectEvent::C4 | DAObjectEvent::BEACON | DAObjectEvent::BUILDING);
	Register_Object_Event(DAObjectEvent::DAMAGERECEIVED,DAObjectEvent::SOLDIER | DAObjectEvent::VEHICLE | DAObjectEvent::C4 | DAObjectEvent::BEACON | DAObjectEvent::BUILDING);
	Register_Object_Event(DAObjectEvent::KILLRECEIVED,DAObjectEvent::SOLDIER | DAObjectEvent::VEHICLE | DAObjectEvent::C4 | DAObjectEvent::BEACON | DAObjectEvent::BUILDING);
}

void DAGameLogGameFeatureClass::Level_Loaded_Event() {
	DALogManager::Write_GameLog("2.03;%s",The_Game()->Get_Map_Name());
	DALogManager::Write_GameLog("CONFIG;%u;%ls",The_Game()->Get_Time_Limit_Minutes(),The_Game()->Get_Game_Title());
}

void DAGameLogGameFeatureClass::Game_Over_Event() {
	StringClass WinType;
	if (The_Game()->WinType == 0) {
		WinType = "server shutdown/gameover";
	}
	else if (The_Game()->WinType == 1) {
		WinType = "score limit reached";
	}
	else if (The_Game()->WinType == 2) {
		WinType = "high score when time limit expired";
	}
	else if (The_Game()->WinType == 3) {
		WinType = "building destruction";
	}
	else if (The_Game()->WinType == 4) {
		WinType = "pedestal beacon";
	}
	else {
		WinType = "unknown";
	}
	DALogManager::Write_GameLog("WIN;%ls;%s;%d;%d",Get_Wide_Team_Name(The_Game()->Get_Winner_ID()),WinType,(int)Get_Team_Score(0),(int)Get_Team_Score(1));
}

void DAGameLogGameFeatureClass::Vehicle_Enter_Event(VehicleGameObj *Vehicle,cPlayer *Player,int Seat) {
	SoldierGameObj *Soldier = Player->Get_GameObj();
	Vector3	VehiclePos;
	Vector3	SoldierPos;
	Vehicle->Get_Position(&VehiclePos);
	Soldier->Get_Position(&SoldierPos);
	DALogManager::Write_GameLog("ENTER;%d;%s;%d;%d;%d;%d;%s;%d;%d;%d",Vehicle->Get_ID(),Vehicle->Get_Definition().Get_Name(),(int)VehiclePos.Y,(int)VehiclePos.X,(int)VehiclePos.Z,Soldier->Get_ID(),Soldier->Get_Definition().Get_Name(),(int)SoldierPos.Y,(int)SoldierPos.Y,(int)SoldierPos.Y);
}

void DAGameLogGameFeatureClass::Vehicle_Exit_Event(VehicleGameObj *Vehicle,cPlayer *Player,int Seat) {
	SoldierGameObj *Soldier = Player->Get_GameObj();
	Vector3	VehiclePos;
	Vector3	SoldierPos;
	Vehicle->Get_Position(&VehiclePos);
	Soldier->Get_Position(&SoldierPos);
	DALogManager::Write_GameLog("EXIT;%d;%s;%d;%d;%d;%d;%s;%d;%d;%d",Vehicle->Get_ID(),Vehicle->Get_Definition().Get_Name(),(int)VehiclePos.Y,(int)VehiclePos.X,(int)VehiclePos.Z,Soldier->Get_ID(),Soldier->Get_Definition().Get_Name(),(int)SoldierPos.Y,(int)SoldierPos.Y,(int)SoldierPos.Y);
}

void DAGameLogGameFeatureClass::Object_Created_Event(GameObject *obj) {
	Vector3 Pos;
	obj->Get_Position(&Pos);
	if (obj->As_SoldierGameObj()) {
		DALogManager::Write_GameLog("CREATED;SOLDIER;%d;%s;%d;%d;%d;%d;%d;%d;%d;%ls",obj->Get_ID(),obj->Get_Definition().Get_Name(),(int)Pos.Y,(int)Pos.X,(int)Pos.Z,(int)Commands->Get_Facing(obj),(int)Commands->Get_Max_Health(obj),(int)Commands->Get_Max_Shield_Strength(obj),Get_Object_Type(obj),Get_Wide_Player_Name(obj));
	}
	else if (Is_C4(obj)) {
		DALogManager::Write_GameLog("CREATED;OBJECT;%d;%s;%d;%d;%d;%d;%d;%d;%d;%d",obj->Get_ID(),obj->Get_Definition().Get_Name(),(int)Pos.Y,(int)Pos.X,(int)Pos.Z,(int)Commands->Get_Facing(obj),(int)Commands->Get_Max_Health(obj),(int)Commands->Get_Max_Shield_Strength(obj),Get_Object_Type(obj),Commands->Get_ID(Get_C4_Planter(obj)));
	}
	else if (Is_Beacon(obj)) {
		DALogManager::Write_GameLog("CREATED;OBJECT;%d;%s;%d;%d;%d;%d;%d;%d;%d;%d",obj->Get_ID(),obj->Get_Definition().Get_Name(),(int)Pos.Y,(int)Pos.X,(int)Pos.Z,(int)Commands->Get_Facing(obj),(int)Commands->Get_Max_Health(obj),(int)Commands->Get_Max_Shield_Strength(obj),Get_Object_Type(obj),Commands->Get_ID(Get_Beacon_Planter(obj)));
	}
	else {
		StringClass ObjectType;
		if (obj->As_VehicleGameObj()) {
			if (((VehicleGameObj*)obj)->Is_Turret()) {
				ObjectType = "BUILDING";
			}
			else if (!((VehicleGameObj*)obj)->Peek_Physical_Object()->As_MoveablePhysClass()) {
				return;
			}
			else {
				ObjectType = "VEHICLE";
			}
		}
		else {
			ObjectType = "BUILDING";
		}
		DALogManager::Write_GameLog("CREATED;%s;%d;%s;%d;%d;%d;%d;%d;%d;%d",ObjectType,obj->Get_ID(),obj->Get_Definition().Get_Name(),(int)Pos.Y,(int)Pos.X,(int)Pos.Z,(int)Commands->Get_Facing(obj),(int)Commands->Get_Max_Health(obj),(int)Commands->Get_Max_Shield_Strength(obj),Get_Object_Type(obj));
	}
}

void DAGameLogGameFeatureClass::Damage_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type){
	if (Damage) {
		Vector3 VictimPos;
		Vector3 DamagerPos;
		Victim->Get_Position(&VictimPos);
		if (Damager) {
			Damager->Get_Position(&DamagerPos);
		}
		else {
			DamagerPos.X = 0.0f;
			DamagerPos.Y = 0.0f;
			DamagerPos.Z = 0.0f;
		}
		StringClass ObjectType;
		if (Victim->As_SoldierGameObj()) {
			ObjectType = "SOLDIER";
		}
		else if (Is_C4(Victim) || Is_Beacon(Victim)) {
			ObjectType = "OBJECT";
		}
		else if (Victim->As_VehicleGameObj()) {
			if (((VehicleGameObj*)Victim)->Get_Definition().Get_Type() == VEHICLE_TYPE_TURRET) {
				ObjectType = "BUILDING";
			}
			else if (!((VehicleGameObj*)Victim)->Peek_Physical_Object()->As_MoveablePhysClass()) {
				return;
			}
			else {
				ObjectType = "VEHICLE";
			}
		}
		else {
			ObjectType = "BUILDING";
		}
		DALogManager::Write_GameLog("DAMAGED;%s;%d;%s;%d;%d;%d;%d;%d;%s;%d;%d;%d;%d;%f;%d;%d;%d",ObjectType,Victim->Get_ID(),Victim->Get_Definition().Get_Name(),(int)VictimPos.Y,(int)VictimPos.X,(int)VictimPos.Z,(int)Commands->Get_Facing(Victim),Damager?Damager->Get_ID():0,Damager?Damager->Get_Definition().Get_Name():"",(int)DamagerPos.Y,(int)DamagerPos.X,(int)DamagerPos.Z,(int)Commands->Get_Facing(Damager),Damage,(int)Victim->Get_Defense_Object()->Get_Health(),(int)Victim->Get_Defense_Object()->Get_Shield_Strength(),(int)Commands->Get_Points(Damager));
	}
}

void DAGameLogGameFeatureClass::Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	Vector3 VictimPos;
	Vector3 KillerPos;
	Victim->Get_Position(&VictimPos);
	if (Killer) {
		Killer->Get_Position(&KillerPos);
	}
	else {
		KillerPos.X = 0.0f;
		KillerPos.Y = 0.0f;
		KillerPos.Z = 0.0f;
	}
	StringClass ObjectType;
	StringClass Translation;
	if (Victim->As_SoldierGameObj()) {
		ObjectType = "SOLDIER";
		Translation = DATranslationManager::Translate(Victim);
	}
	else if (Is_C4(Victim) || Is_Beacon(Victim)) {
		ObjectType = "OBJECT";
		Translation = DATranslationManager::Translate(Victim);
	}
	else if (Victim->As_VehicleGameObj()) {
		if (((VehicleGameObj*)Victim)->Get_Definition().Get_Type() == VEHICLE_TYPE_TURRET) {
			ObjectType = "BUILDING";
			Translation = DATranslationManager::Translate_With_Team_Name(Victim);
		}
		else if (!((VehicleGameObj*)Victim)->Peek_Physical_Object()->As_MoveablePhysClass()) {
			return;
		}
		else {
			ObjectType = "VEHICLE";
			Translation = DATranslationManager::Translate(Victim);
		}
	}
	else {
		ObjectType = "BUILDING";
		Translation = DATranslationManager::Translate_With_Team_Name(Victim);
	}
	DALogManager::Write_GameLog("KILLED;%s;%d;%s;%d;%d;%d;%d;%d;%s;%d;%d;%d;%d;%s;%s;%s",ObjectType,Victim->Get_ID(),Victim->Get_Definition().Get_Name(),(int)VictimPos.Y,(int)VictimPos.X,(int)VictimPos.Z,(int)Commands->Get_Facing(Victim),Killer?Killer->Get_ID():0,Killer?Killer->Get_Definition().Get_Name():"",(int)KillerPos.Y,(int)KillerPos.X,(int)KillerPos.Z,(int)Commands->Get_Facing(Killer),Get_Current_Weapon(Killer),Translation,DATranslationManager::Translate(Killer));
}

void DAGameLogGameFeatureClass::Object_Destroyed_Event(GameObject *obj) {
	Vector3 Pos;
	obj->Get_Position(&Pos);
	StringClass ObjectType;
	if (obj->As_SoldierGameObj()) {
		ObjectType = "SOLDIER";
	}
	else if (Is_C4(obj) || Is_Beacon(obj)) {
		ObjectType = "OBJECT";
	}
	else if (obj->As_VehicleGameObj()) {
		if (((VehicleGameObj*)obj)->Get_Definition().Get_Type() == VEHICLE_TYPE_TURRET) {
			ObjectType = "BUILDING";
		}
		else if (!((VehicleGameObj*)obj)->Peek_Physical_Object()->As_MoveablePhysClass()) {
			return;
		}
		else {
			ObjectType = "VEHICLE";
		}
	}
	else {
		ObjectType = "BUILDING";
	}
	DALogManager::Write_GameLog("DESTROYED;%s;%d;%s;%d;%d;%d",ObjectType,obj->Get_ID(),obj->Get_Definition().Get_Name(),(int)Pos.Y,(int)Pos.X,(int)Pos.Z);
}

Register_Game_Feature(DAGameLogGameFeatureClass,0,"EnableGameLog",0);