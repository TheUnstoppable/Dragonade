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

#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "engine_DA.h"
#include "da.h"
#include "da_damagelog.h"
#include "da_vehicle.h"
#include "GameObjManager.h"

static const char *ObserverName = "DADamageLogObserverClass";

void DADamageLogObserverClass::Init() {
	Start_Timer(1,30.0f);
}

void DADamageLogObserverClass::Damage_Received(ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	if (Damager != Get_Owner() && Damage) {
		DADamageEventStruct *Event = new DADamageEventStruct;
		Event->Damage = Damage;
		Event->Time = The_Game()->Get_Game_Duration_S();
		if (Is_Player(Damager)) {
			Event->Player = ((SoldierGameObj*)Damager)->Get_Player();
		}
		else {
			Event->Player = 0;
		}
		DamageEvents.Add_Tail(Event);
	}
}

void DADamageLogObserverClass::Timer_Expired(GameObject *obj,int Number) {
	for (SLNode<DADamageEventStruct> *z = DamageEvents.Head();z;) {
		if (The_Game()->Get_Game_Duration_S()-z->Data()->Time > 30) {
			z = z->Next();
			delete DamageEvents.Remove_Head();
		}
		else {
			break;
		}
	}
	Start_Timer(1,30.0f);
}

void DADamageLogObserverClass::Vehicle_Enter(cPlayer *Player,int Seat) {
	if (Player->Get_Player_Type() != DAVehicleManager::Get_Team(Get_Owner())) {
		Clear_Damage();
	}
}

void DADamageLogObserverClass::Remove_Damage(cPlayer *Player) {
	for (SLNode<DADamageEventStruct> *z = DamageEvents.Head();z;) {
		if (z->Data()->Player == Player) {
			DADamageEventStruct *Event = z->Data();
			z = z->Next();
			DamageEvents.Remove(Event);
			delete Event;
		}
		else {
			z = z->Next();
		}
	}
}

void DADamageLogObserverClass::Clear_Damage() {
	for (SLNode<DADamageEventStruct> *z = DamageEvents.Head();z;z = z->Next()) {
		delete z->Data();
	}
	DamageEvents.Remove_All();
}

DADamageLogObserverClass::~DADamageLogObserverClass() {
	Clear_Damage();
}

const SList<DADamageEventStruct> *DADamageLogObserverClass::Get_Damage_Events() {
	return &DamageEvents;
}

float DADamageLogObserverClass::Compile_Damage_Table_Team(DADamageTableStruct *DamageTable,int Team) {
	float TotalDamage = 0.0f;
	for (SLNode<DADamageEventStruct> *z = DamageEvents.Head();z;z = z->Next()) {
		if (The_Game()->Get_Game_Duration_S()-z->Data()->Time <= 30 && z->Data()->Damage > 0.0f) {
			cPlayer *Player = z->Data()->Player;
			if (Player) {
				if (Player->Get_Player_Type() == Team || Team == 2) {
					DamageTable[Player->Get_Id()].Player = Player;
					DamageTable[Player->Get_Id()].Damage += z->Data()->Damage;
				}
			}
			else {
				DamageTable[0].Damage += z->Data()->Damage;
			}
			TotalDamage += z->Data()->Damage;
		}
	}
	return TotalDamage;
}

float DADamageLogObserverClass::Compile_Damage_Table_Other_Team(DADamageTableStruct *DamageTable,int Team) {
	float TotalDamage = 0.0f;
	for (SLNode<DADamageEventStruct> *z = DamageEvents.Head();z;z = z->Next()) {
		if (The_Game()->Get_Game_Duration_S()-z->Data()->Time <= 30 && z->Data()->Damage > 0.0f) {
			cPlayer *Player = z->Data()->Player;
			if (Player) {
				if (Player->Get_Player_Type() != Team) {
					DamageTable[Player->Get_Id()].Player = Player;
					DamageTable[Player->Get_Id()].Damage += z->Data()->Damage;
				}
			}
			else {
				DamageTable[0].Damage += z->Data()->Damage;
			}
			TotalDamage += z->Data()->Damage;
		}
	}
	return TotalDamage;
}

void DADamageLogObserverClass::Get_Damagers_By_Percent_Team(DynamicVectorClass<DADamageTableStruct> &Buffer,int Team,float MinimumPercentTotal,float MinimumPercentHighest) {
	Buffer.Delete_All();
	Buffer.Resize(The_Game()->Get_Current_Players());
	DADamageTableStruct DamageTable[128];
	float TotalDamage = Compile_Damage_Table_Team(DamageTable,Team);
	float ModifiedTotalDamage = 0.0f;
	float HighestDamage = 0.0f;
	float TotalDamagePercent = TotalDamage*MinimumPercentTotal;
	for (int i = 0;i < 127;i++) {
		if (DamageTable[i].Damage > 0.0f && DamageTable[i].Damage >= TotalDamagePercent) {
			ModifiedTotalDamage += DamageTable[i].Damage;
			if (DamageTable[i].Damage > HighestDamage) {
				HighestDamage = DamageTable[i].Damage;
			}
		}
	}
	float HighestDamagePercent = HighestDamage*MinimumPercentHighest;
	for (int i = 1;i < 127;i++) {
		if (DamageTable[i].Damage > 0.0f && DamageTable[i].Damage >= HighestDamagePercent) {
			Buffer.Add(DADamageTableStruct(DamageTable[i].Player,DamageTable[i].Damage/ModifiedTotalDamage));
		}
	}
}

void DADamageLogObserverClass::Get_Damagers_By_Percent_Other_Team(DynamicVectorClass<DADamageTableStruct> &Buffer,int Team,float MinimumPercentTotal,float MinimumPercentHighest) {
	Buffer.Delete_All();
	Buffer.Resize(The_Game()->Get_Current_Players());
	DADamageTableStruct DamageTable[128];
	float TotalDamage = Compile_Damage_Table_Other_Team(DamageTable,Team);
	float ModifiedTotalDamage = 0.0f;
	float HighestDamage = 0.0f;
	float TotalDamagePercent = TotalDamage*MinimumPercentTotal;
	for (int i = 0;i < 127;i++) {
		if (DamageTable[i].Damage > 0.0f && DamageTable[i].Damage >= TotalDamagePercent) {
			ModifiedTotalDamage += DamageTable[i].Damage;
			if (DamageTable[i].Damage > HighestDamage) {
				HighestDamage = DamageTable[i].Damage;
			}
		}
	}
	float HighestDamagePercent = HighestDamage*MinimumPercentHighest;
	for (int i = 1;i < 127;i++) {
		if (DamageTable[i].Damage > 0.0f && DamageTable[i].Damage >= HighestDamagePercent) {
			Buffer.Add(DADamageTableStruct(DamageTable[i].Player,DamageTable[i].Damage/ModifiedTotalDamage));
		}
	}
}

cPlayer *DADamageLogObserverClass::Get_Highest_Damager_Team(int Team) {
	DADamageTableStruct DamageTable[128];
	Compile_Damage_Table_Team(DamageTable,Team);
	cPlayer *HighestDamager = 0;
	float HighestDamage = 0.0f;
	for (int i = 1;i < 127;i++) {
		if (DamageTable[i].Damage > HighestDamage) {
			if (DamageTable[i].Player->Get_Player_Type() == Team || Team == 2) {
				HighestDamager = DamageTable[i].Player;
				HighestDamage = DamageTable[i].Damage;
			}
		}
	}
	return HighestDamager;
}

cPlayer *DADamageLogObserverClass::Get_Highest_Damager_Other_Team(int Team) {
	DADamageTableStruct DamageTable[128];
	Compile_Damage_Table_Team(DamageTable,Team);
	cPlayer *HighestDamager = 0;
	float HighestDamage = 0.0f;
	for (int i = 1;i < 127;i++) {
		if (DamageTable[i].Damage > HighestDamage) {
			if (DamageTable[i].Player->Get_Player_Type() != Team) {
				HighestDamager = DamageTable[i].Player;
				HighestDamage = DamageTable[i].Damage;
			}
		}
	}
	return HighestDamager;
}

float DADamageLogObserverClass::Get_Percent_Team_Damage(int Team) {
	float TotalDamage = 0.0f;
	float TeamDamage = 0.0f;
	for (SLNode<DADamageEventStruct> *z = DamageEvents.Head();z;z = z->Next()) {
		if (The_Game()->Get_Game_Duration_S()-z->Data()->Time <= 30 && z->Data()->Damage > 0.0f) {
			cPlayer *Player = z->Data()->Player;
			if (Player) {
				if (Player->Get_Player_Type() == Team || Team == 2) {
					TeamDamage += z->Data()->Damage;
				}
			}
			TotalDamage += z->Data()->Damage;
		}
	}
	if (!TeamDamage) {
		return 0.0f;
	}
	return TeamDamage/TotalDamage;
}

float DADamageLogObserverClass::Get_Percent_Other_Team_Damage(int Team) {
	float TotalDamage = 0.0f;
	float TeamDamage = 0.0f;
	for (SLNode<DADamageEventStruct> *z = DamageEvents.Head();z;z = z->Next()) {
		if (The_Game()->Get_Game_Duration_S()-z->Data()->Time <= 30 && z->Data()->Damage > 0.0f) {
			cPlayer *Player = z->Data()->Player;
			if (!Player || Player->Get_Player_Type() != Team) {
				TeamDamage += z->Data()->Damage;
			}
			TotalDamage += z->Data()->Damage;
		}
	}
	if (!TeamDamage) {
		return 0.0f;
	}
	return TeamDamage/TotalDamage;
}

float DADamageLogObserverClass::Get_Percent_Player_Damage(cPlayer *Player) {
	float TotalDamage = 0.0f;
	float PlayerDamage = 0.0f;
	for (SLNode<DADamageEventStruct> *z = DamageEvents.Head();z;z = z->Next()) {
		if (The_Game()->Get_Game_Duration_S()-z->Data()->Time <= 30 && z->Data()->Damage > 0.0f) {
			if (z->Data()->Player == Player) {
				PlayerDamage += z->Data()->Damage;
			}
			TotalDamage += z->Data()->Damage;
		}
	}
	if (!PlayerDamage) {
		return 0.0f;
	}
	return PlayerDamage/TotalDamage;
}

const DADamageEventStruct *DADamageLogObserverClass::Get_Last_Damage_Event() {
	const DADamageEventStruct *Return = 0;
	for (SLNode<DADamageEventStruct> *z = DamageEvents.Head();z;z = z->Next()) {
		if (z->Data()->Damage > 0.0f) {
			Return = z->Data();
		}
	}
	return Return;
}

float DADamageLogObserverClass::Compile_Repair_Table_Team(DADamageTableStruct *DamageTable,int Team) {
	float TotalDamage = 0.0f;
	for (SLNode<DADamageEventStruct> *z = DamageEvents.Head();z;z = z->Next()) {
		if (The_Game()->Get_Game_Duration_S()-z->Data()->Time <= 30 && z->Data()->Damage < 0.0f) {
			cPlayer *Player = z->Data()->Player;
			if (Player) {
				if (Player->Get_Player_Type() == Team || Team == 2) {
					DamageTable[Player->Get_Id()].Player = Player;
					DamageTable[Player->Get_Id()].Damage -= z->Data()->Damage;
				}
			}
			else {
				DamageTable[0].Damage -= z->Data()->Damage;
			}
			TotalDamage -= z->Data()->Damage;
		}
	}
	return TotalDamage;
}

float DADamageLogObserverClass::Compile_Repair_Table_Other_Team(DADamageTableStruct *DamageTable,int Team) {
	float TotalDamage = 0.0f;
	for (SLNode<DADamageEventStruct> *z = DamageEvents.Head();z;z = z->Next()) {
		if (The_Game()->Get_Game_Duration_S()-z->Data()->Time <= 30 && z->Data()->Damage < 0.0f) {
			cPlayer *Player = z->Data()->Player;
			if (Player) {
				if (Player->Get_Player_Type() != Team) {
					DamageTable[Player->Get_Id()].Player = Player;
					DamageTable[Player->Get_Id()].Damage -= z->Data()->Damage;
				}
			}
			else {
				DamageTable[0].Damage -= z->Data()->Damage;
			}
			TotalDamage -= z->Data()->Damage;
		}
	}
	return TotalDamage;
}

void DADamageLogObserverClass::Get_Repairers_By_Percent_Team(DynamicVectorClass<DADamageTableStruct> &Buffer,int Team,float MinimumPercentTotal,float MinimumPercentHighest) {
	Buffer.Delete_All();
	Buffer.Resize(The_Game()->Get_Current_Players());
	DADamageTableStruct DamageTable[128];
	float TotalDamage = Compile_Repair_Table_Team(DamageTable,Team);
	float ModifiedTotalDamage = 0.0f;
	float HighestDamage = 0.0f;
	float TotalDamagePercent = TotalDamage*MinimumPercentTotal;
	for (int i = 0;i < 127;i++) {
		if (DamageTable[i].Damage > 0.0f && DamageTable[i].Damage >= TotalDamagePercent) {
			ModifiedTotalDamage += DamageTable[i].Damage;
			if (DamageTable[i].Damage > HighestDamage) {
				HighestDamage = DamageTable[i].Damage;
			}
		}
	}
	float HighestDamagePercent = HighestDamage*MinimumPercentHighest;
	for (int i = 1;i < 127;i++) {
		if (DamageTable[i].Damage > 0 && DamageTable[i].Damage >= HighestDamagePercent) {
			Buffer.Add(DADamageTableStruct(DamageTable[i].Player,DamageTable[i].Damage/ModifiedTotalDamage));
		}
	}
}

void DADamageLogObserverClass::Get_Repairers_By_Percent_Other_Team(DynamicVectorClass<DADamageTableStruct> &Buffer,int Team,float MinimumPercentTotal,float MinimumPercentHighest) {
	Buffer.Delete_All();
	Buffer.Resize(The_Game()->Get_Current_Players());
	DADamageTableStruct DamageTable[128];
	float TotalDamage = Compile_Repair_Table_Other_Team(DamageTable,Team);
	float ModifiedTotalDamage = 0.0f;
	float HighestDamage = 0.0f;
	float TotalDamagePercent = TotalDamage*MinimumPercentTotal;
	for (int i = 0;i < 127;i++) {
		if (DamageTable[i].Damage > 0.0f && DamageTable[i].Damage >= TotalDamagePercent) {
			ModifiedTotalDamage += DamageTable[i].Damage;
			if (DamageTable[i].Damage > HighestDamage) {
				HighestDamage = DamageTable[i].Damage;
			}
		}
	}
	float HighestDamagePercent = HighestDamage*MinimumPercentHighest;
	for (int i = 1;i < 127;i++) {
		if (DamageTable[i].Damage > 0 && DamageTable[i].Damage >= HighestDamagePercent) {
			Buffer.Add(DADamageTableStruct(DamageTable[i].Player,DamageTable[i].Damage/ModifiedTotalDamage));
		}
	}
}

cPlayer *DADamageLogObserverClass::Get_Highest_Repairer_Team(int Team) {
	DADamageTableStruct DamageTable[128];
	Compile_Repair_Table_Team(DamageTable,Team);
	cPlayer *HighestDamager = 0;
	float HighestDamage = 0.0f;
	for (int i = 1;i < 127;i++) {
		if (DamageTable[i].Damage > HighestDamage) {
			if (DamageTable[i].Player->Get_Player_Type() == Team || Team == 2) {
				HighestDamager = DamageTable[i].Player;
				HighestDamage = DamageTable[i].Damage;
			}
		}
	}
	return HighestDamager;
}

cPlayer *DADamageLogObserverClass::Get_Highest_Repairer_Other_Team(int Team) {
	DADamageTableStruct DamageTable[128];
	Compile_Repair_Table_Team(DamageTable,Team);
	cPlayer *HighestDamager = 0;
	float HighestDamage = 0.0f;
	for (int i = 1;i < 127;i++) {
		if (DamageTable[i].Damage > HighestDamage) {
			if (DamageTable[i].Player->Get_Player_Type() != Team) {
				HighestDamager = DamageTable[i].Player;
				HighestDamage = DamageTable[i].Damage;
			}
		}
	}
	return HighestDamager;
}

float DADamageLogObserverClass::Get_Percent_Team_Repairs(int Team) {
	float TotalDamage = 0.0f;
	float TeamDamage = 0.0f;
	for (SLNode<DADamageEventStruct> *z = DamageEvents.Head();z;z = z->Next()) {
		if (The_Game()->Get_Game_Duration_S()-z->Data()->Time <= 30 && z->Data()->Damage < 0.0f) {
			cPlayer *Player = z->Data()->Player;
			if (Player) {
				if (Player->Get_Player_Type() == Team || Team == 2) {
					TeamDamage -= z->Data()->Damage;
				}
			}
			TotalDamage -= z->Data()->Damage;
		}
	}
	if (!TeamDamage) {
		return 0.0f;
	}
	return TeamDamage/TotalDamage;
}

float DADamageLogObserverClass::Get_Percent_Other_Team_Repairs(int Team) {
	float TotalDamage = 0.0f;
	float TeamDamage = 0.0f;
	for (SLNode<DADamageEventStruct> *z = DamageEvents.Head();z;z = z->Next()) {
		if (The_Game()->Get_Game_Duration_S()-z->Data()->Time <= 30 && z->Data()->Damage < 0.0f) {
			cPlayer *Player = z->Data()->Player;
			if (!Player || Player->Get_Player_Type() != Team) {
				TeamDamage -= z->Data()->Damage;
			}
			TotalDamage -= z->Data()->Damage;
		}
	}
	if (!TeamDamage) {
		return 0.0f;
	}
	return TeamDamage/TotalDamage;
}

float DADamageLogObserverClass::Get_Percent_Player_Repairs(cPlayer *Player) {
	float TotalDamage = 0.0f;
	float PlayerDamage = 0.0f;
	for (SLNode<DADamageEventStruct> *z = DamageEvents.Head();z;z = z->Next()) {
		if (The_Game()->Get_Game_Duration_S()-z->Data()->Time <= 30 && z->Data()->Damage < 0.0f) {
			if (z->Data()->Player == Player) {
				PlayerDamage -= z->Data()->Damage;
			}
			TotalDamage -= z->Data()->Damage;
		}
	}
	if (!PlayerDamage) {
		return 0.0f;
	}
	return PlayerDamage/TotalDamage;
}

const DADamageEventStruct *DADamageLogObserverClass::Get_Last_Repair_Event() {
	const DADamageEventStruct *Return = 0;
	for (SLNode<DADamageEventStruct> *z = DamageEvents.Head();z;z = z->Next()) {
		if (z->Data()->Damage < 0.0f) {
			Return = z->Data();
		}
	}
	return Return;
}

const char *DADamageLogObserverClass::Get_Name() {
	return ObserverName;
}

void DADamageLog::Init() {
	static DADamageLog Instance;
	Instance.Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::DAMAGEABLE,INT_MAX);
	Instance.Register_Event(DAEvent::PLAYERLEAVE,INT_MAX);
	Instance.Register_Event(DAEvent::TEAMCHANGE,INT_MAX);
}

DADamageLogObserverClass *DADamageLog::Get_Damage_Log(GameObject *obj) {
	const SimpleDynVecClass<GameObjObserverClass*> &Observers = obj->Get_Observers();
	for (int i = 0;i < Observers.Count();i++) {
		if (Observers[i]->Get_Name() == ObserverName) {
			return (DADamageLogObserverClass*)Observers[i];
		}
	}
	return 0;
}

const SList<DADamageEventStruct> *DADamageLog::Get_Damage_Events(GameObject *obj) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		return Log->Get_Damage_Events();
	}
	return 0;
}

float DADamageLog::Compile_Damage_Table_Team(DADamageTableStruct *DamageTable,GameObject *obj,int Team) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		return Log->Compile_Damage_Table_Team(DamageTable,Team);
	}
	return 0.0f;
}

float DADamageLog::Compile_Damage_Table_Other_Team(DADamageTableStruct *DamageTable,GameObject *obj,int Team) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		return Log->Compile_Damage_Table_Other_Team(DamageTable,Team);
	}
	return 0.0f;
}

void DADamageLog::Get_Damagers_By_Percent_Team(DynamicVectorClass<DADamageTableStruct> &Buffer,GameObject *obj,int Team,float MinimumPercentTotal,float MinimumPercentHighest) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		Log->Get_Damagers_By_Percent_Team(Buffer,Team,MinimumPercentTotal,MinimumPercentHighest);
	}
}

void DADamageLog::Get_Damagers_By_Percent_Other_Team(DynamicVectorClass<DADamageTableStruct> &Buffer,GameObject *obj,int Team,float MinimumPercentTotal,float MinimumPercentHighest) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		Log->Get_Damagers_By_Percent_Other_Team(Buffer,Team,MinimumPercentTotal,MinimumPercentHighest);
	}
}

cPlayer *DADamageLog::Get_Highest_Damager_Team(GameObject *obj,int Team) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		return Log->Get_Highest_Damager_Team(Team);
	}
	return 0;
}

cPlayer *DADamageLog::Get_Highest_Damager_Other_Team(GameObject *obj,int Team) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		return Log->Get_Highest_Damager_Other_Team(Team);
	}
	return 0;
}

float DADamageLog::Get_Percent_Team_Damage(GameObject *obj,int Team) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		return Log->Get_Percent_Team_Damage(Team);
	}
	return 0;
}

float DADamageLog::Get_Percent_Other_Team_Damage(GameObject *obj,int Team) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		return Log->Get_Percent_Other_Team_Damage(Team);
	}
	return 0;
}

float DADamageLog::Get_Percent_Player_Damage(GameObject *obj,cPlayer *Player) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		return Log->Get_Percent_Player_Damage(Player);
	}
	return 0;
}

const DADamageEventStruct *DADamageLog::Get_Last_Damage_Event(GameObject *obj) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		return Log->Get_Last_Damage_Event();
	}
	return 0;
}

float DADamageLog::Compile_Repair_Table_Team(DADamageTableStruct *DamageTable,GameObject *obj,int Team) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		return Log->Compile_Repair_Table_Team(DamageTable,Team);
	}
	return 0.0f;
}

float DADamageLog::Compile_Repair_Table_Other_Team(DADamageTableStruct *DamageTable,GameObject *obj,int Team) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		return Log->Compile_Repair_Table_Other_Team(DamageTable,Team);
	}
	return 0.0f;
}

void DADamageLog::Get_Repairers_By_Percent_Team(DynamicVectorClass<DADamageTableStruct> &Buffer,GameObject *obj,int Team,float MinimumPercentTotal,float MinimumPercentHighest) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		Log->Get_Repairers_By_Percent_Team(Buffer,Team,MinimumPercentTotal,MinimumPercentHighest);
	}
}

void DADamageLog::Get_Repairers_By_Percent_Other_Team(DynamicVectorClass<DADamageTableStruct> &Buffer,GameObject *obj,int Team,float MinimumPercentTotal,float MinimumPercentHighest) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		Log->Get_Repairers_By_Percent_Other_Team(Buffer,Team,MinimumPercentTotal,MinimumPercentHighest);
	}
}

cPlayer *DADamageLog::Get_Highest_Repairer_Team(GameObject *obj,int Team) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		return Log->Get_Highest_Repairer_Team(Team);
	}
	return 0;
}

cPlayer *DADamageLog::Get_Highest_Repairer_Other_Team(GameObject *obj,int Team) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		return Log->Get_Highest_Repairer_Other_Team(Team);
	}
	return 0;
}

float DADamageLog::Get_Percent_Team_Repairs(GameObject *obj,int Team) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		return Log->Get_Percent_Team_Repairs(Team);
	}
	return 0;
}

float DADamageLog::Get_Percent_Other_Team_Repairs(GameObject *obj,int Team) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		return Log->Get_Percent_Other_Team_Repairs(Team);
	}
	return 0;
}

float DADamageLog::Get_Percent_Player_Repairs(GameObject *obj,cPlayer *Player) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		return Log->Get_Percent_Player_Repairs(Player);
	}
	return 0;
}

const DADamageEventStruct *DADamageLog::Get_Last_Repair_Event(GameObject *obj) {
	DADamageLogObserverClass *Log = Get_Damage_Log(obj);
	if (Log) {
		return Log->Get_Last_Repair_Event();
	}
	return 0;
}

void DADamageLog::Object_Created_Event(GameObject *obj) {
	obj->Add_Observer(new DADamageLogObserverClass);
}

void DADamageLog::Player_Leave_Event(cPlayer *Player) {
	for (SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();x;x = x->Next()) {
		if (x->Data()->As_ScriptableGameObj() && ((ScriptableGameObj*)x->Data())->As_DamageableGameObj()) {
			const SimpleDynVecClass<GameObjObserverClass*> &Observers = ((ScriptableGameObj*)x->Data())->Get_Observers();
			for (int i = 0;i < Observers.Count();i++) {
				if (Observers[i]->Get_Name() == ObserverName) {
					((DADamageLogObserverClass*)Observers[i])->Remove_Damage(Player);
					break;
				}
			}
		}
	}
}

void DADamageLog::Team_Change_Event(cPlayer *Player) {
	Player_Leave_Event(Player);
}
