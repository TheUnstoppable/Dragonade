/*	Renegade Scripts.dll
    Dragonade Damage Log
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DADAMAGELOG
#define INCLUDE_DADAMAGELOG

#include "da_event.h"
#include "da_gameobj.h"

struct DADamageEventStruct {
	float Damage;
	cPlayer *Player;
	unsigned int Time;
};

struct DADamageTableStruct {
	DADamageTableStruct() {
		Player = 0;
		Damage = 0.0f;
	}
	DADamageTableStruct(cPlayer *player,float damage) {
		Player = player;
		Damage = damage;
	}
	bool operator!=(const DADamageTableStruct&) {
		return true;
	}
	bool operator==(const DADamageTableStruct&) {
		return false;
	}
	cPlayer *Player;
	float Damage;
};

class DADamageLogObserverClass : public DAGameObjObserverClass {
public:
	virtual void Init();
	virtual void Damage_Received(ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	virtual void Timer_Expired(GameObject *obj,int Number);
	virtual void Vehicle_Enter(cPlayer *Player,int Seat);
	DA_API void Remove_Damage(cPlayer *Player);
	DA_API void Clear_Damage();
	virtual ~DADamageLogObserverClass();

	DA_API const SList<DADamageEventStruct> *Get_Damage_Events();

	DA_API float Compile_Damage_Table_Team(DADamageTableStruct *DamageTable,int Team);
	DA_API float Compile_Damage_Table_Other_Team(DADamageTableStruct *DamageTable,int Team);
	DA_API void Get_Damagers_By_Percent_Team(DynamicVectorClass<DADamageTableStruct> &Buffer,int Team,float MinimumPercentTotal,float MinimumPercentHighest);
	DA_API void Get_Damagers_By_Percent_Other_Team(DynamicVectorClass<DADamageTableStruct> &Buffer,int Team,float MinimumPercentTotal,float MinimumPercentHighest);
	DA_API cPlayer *Get_Highest_Damager_Team(int Team);
	DA_API cPlayer *Get_Highest_Damager_Other_Team(int Team);
	DA_API float Get_Percent_Team_Damage(int Team);
	DA_API float Get_Percent_Other_Team_Damage(int Team);
	DA_API float Get_Percent_Player_Damage(cPlayer *Player);
	DA_API const DADamageEventStruct *Get_Last_Damage_Event();
	
	DA_API float Compile_Repair_Table_Team(DADamageTableStruct *DamageTable,int Team);
	DA_API float Compile_Repair_Table_Other_Team(DADamageTableStruct *DamageTable,int Team);
	DA_API void Get_Repairers_By_Percent_Team(DynamicVectorClass<DADamageTableStruct> &Buffer,int Team,float MinimumPercentTotal,float MinimumPercentHighest);
	DA_API void Get_Repairers_By_Percent_Other_Team(DynamicVectorClass<DADamageTableStruct> &Buffer,int Team,float MinimumPercentTotal,float MinimumPercentHighest);
	DA_API cPlayer *Get_Highest_Repairer_Team(int Team);
	DA_API cPlayer *Get_Highest_Repairer_Other_Team(int Team);
	DA_API float Get_Percent_Team_Repairs(int Team);
	DA_API float Get_Percent_Other_Team_Repairs(int Team);
	DA_API float Get_Percent_Player_Repairs(cPlayer *Player);
	DA_API const DADamageEventStruct *Get_Last_Repair_Event();
	
	virtual const char *Get_Name();
private:
	SList<DADamageEventStruct> DamageEvents;
};

class DADamageLog : public DAEventClass {
public:
	static void Init();

	DA_API static DADamageLogObserverClass *Get_Damage_Log(GameObject *obj);

	DA_API static const SList<DADamageEventStruct> *Get_Damage_Events(GameObject *obj);

	DA_API static float Compile_Damage_Table_Team(DADamageTableStruct *DamageTable,GameObject *obj,int Team);
	DA_API static float Compile_Damage_Table_Other_Team(DADamageTableStruct *DamageTable,GameObject *obj,int Team);
	DA_API static void Get_Damagers_By_Percent_Team(DynamicVectorClass<DADamageTableStruct> &Buffer,GameObject *obj,int Team,float MinimumPercentTotal,float MinimumPercentHighest);
	DA_API static void Get_Damagers_By_Percent_Other_Team(DynamicVectorClass<DADamageTableStruct> &Buffer,GameObject *obj,int Team,float MinimumPercentTotal,float MinimumPercentHighest);
	DA_API static cPlayer *Get_Highest_Damager_Team(GameObject *obj,int Team);
	DA_API static cPlayer *Get_Highest_Damager_Other_Team(GameObject *obj,int Team);
	DA_API static float Get_Percent_Team_Damage(GameObject *obj,int Team);
	DA_API static float Get_Percent_Other_Team_Damage(GameObject *obj,int Team);
	DA_API static float Get_Percent_Player_Damage(GameObject *obj,cPlayer *Player);
	DA_API static const DADamageEventStruct *Get_Last_Damage_Event(GameObject *obj);

	DA_API static float Compile_Repair_Table_Team(DADamageTableStruct *DamageTable,GameObject *obj,int Team);
	DA_API static float Compile_Repair_Table_Other_Team(DADamageTableStruct *DamageTable,GameObject *obj,int Team);
	DA_API static void Get_Repairers_By_Percent_Team(DynamicVectorClass<DADamageTableStruct> &Buffer,GameObject *obj,int Team,float MinimumPercentTotal,float MinimumPercentHighest);
	DA_API static void Get_Repairers_By_Percent_Other_Team(DynamicVectorClass<DADamageTableStruct> &Buffer,GameObject *obj,int Team,float MinimumPercentTotal,float MinimumPercentHighest);
	DA_API static cPlayer *Get_Highest_Repairer_Team(GameObject *obj,int Team);
	DA_API static cPlayer *Get_Highest_Repairer_Other_Team(GameObject *obj,int Team);
	DA_API static float Get_Percent_Team_Repairs(GameObject *obj,int Team);
	DA_API static float Get_Percent_Other_Team_Repairs(GameObject *obj,int Team);
	DA_API static float Get_Percent_Player_Repairs(GameObject *obj,cPlayer *Player);
	DA_API static const DADamageEventStruct *Get_Last_Repair_Event(GameObject *obj);

private:
	virtual void Object_Created_Event(GameObject *obj);
	virtual void Player_Leave_Event(cPlayer *Player);
	virtual void Team_Change_Event(cPlayer *Player);
};


#endif