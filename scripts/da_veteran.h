/*	Renegade Scripts.dll
	Dragonade Veteran Game Feature
	Copyright 2022 Unstoppable, Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAVETERAN
#define INCLUDE_DAVETERAN

#include "da_gamefeature.h"
#include "da_event.h"
#include "da_player.h"
#include "HashTemplateClass.h"

// Common macros with Worth and Repair Multiplier
#define VETWORTH_STARSOLDIER "Star"
#define VETWORTH_BOTSOLDIER "Bot"
#define VETWORTH_REMOTEC4 "RemoteC4"
#define VETWORTH_TIMEDC4 "TimedC4"
#define VETWORTH_PROXC4 "ProximityC4"
#define VETWORTH_BEACON "Beacon"
#define VETWORTH_HARVESTER "Harvester"
#define VETWORTH_TURRET "Turret"
#define VETWORTH_VEHICLE "Vehicle"
#define VETWORTH_POWERPLANT "PowerPlant"
#define VETWORTH_REFINERY "TiberiumRefinery"
#define VETWORTH_SOLDIERFACTORY "SoldierFactory"
#define VETWORTH_VEHICLEFACTORY "VehicleFactory"
#define VETWORTH_BASEDEFENSE "BaseDefense"
#define VETWORTH_COMCENTER "ComCenter"
#define VETWORTH_CONYARD "ConYard"
#define VETWORTH_REPAIRBAY "RepairBay"
#define VETWORTH_SHRINE "Shrine"
#define VETWORTH_HELIPAD "Helipad"
#define VETWORTH_TECHCENTER "TechCenter"
#define VETWORTH_NAVALFACTORY "NavalFactory"
#define VETWORTH_WEAK_BUILDING "WeakBuilding"
#define VETWORTH_HEAVY_BUILDING "HeavyBuilding"
// Specific to Worth
#define VETWORTH_REPAIR "Repair"
#define VETWORTH_DAMAGE "Damage"
#define VETWORTH_STEAL "Steal"

// Misc stuff
#define VETCOLOR 144,244,44 
#define HALF(value) ((value - 1.f) * .5f + 1.f) // .5f * value + .5f

struct DAVeteranLevel
{
	int Index;
	
	StringClass Name;
	StringClass PublicSound_GDI;
	StringClass PublicSound_Nod;
	float RequiredPoints;

	float InfantryHealthIncrease;
	float InfantryRegen;
	float InfantryRegenInterval;
	
	float VehicleHealthIncrease;
	float VehicleRegen;
	float VehicleRegenInterval;
	
	float PurchaseDiscount;
};

class DA_API DAVeteranPlayerDataObserverClass : public DAPlayerObserverClass {
private:
	virtual void Init();
	virtual void Created();
	virtual void Vehicle_Enter(VehicleGameObj* Vehicle, int Seat);
	virtual void Vehicle_Exit(VehicleGameObj* Vehicle, int Seat);
	virtual void Timer_Expired(int Number, unsigned int Data);
	virtual void Custom(GameObject* Sender, int Type, int Param);
	virtual void Think();

public:
	const char* Get_Observer_Name() override { return "DAVeteranPlayerDataObserverClass"; }

	int Get_Level() { return Level; }
	float Get_Points() { return Points; }
	float Get_Repair_Points() { return RepairPoints; }
	float Get_Damage_Points() { return DamagePoints; }
	void Set_Level(int level) { Level = level; }
	void Set_Points(float points) { Points = points; }
	void Set_Repair_Points(float points) { RepairPoints = points; }
	void Set_Damage_Points(float points) { DamagePoints = points; }
	void Add_Points(float points) { Points += points; }
	void Add_Repair_Points(float points) { RepairPoints += points; }
	void Add_Damage_Points(float points) { DamagePoints += points; }

	DAVeteranLevel* Get_Player_Level();

private:
	int Level;
	float Points;
	float RepairPoints;
	float DamagePoints;

	float SoldierOriginalHealth;
	float SoldierOriginalArmor;
	float VehicleOriginalHealth;
	float VehicleOriginalArmor;

	float SoldierCalculatedHealth;
	float SoldierCalculatedArmor;
	float VehicleCalculatedHealth;
	float VehicleCalculatedArmor;

	inline bool Is_In_Vehicle() {
		return Get_Owner() && Get_Owner()->Get_GameObj() && Get_Owner()->Get_GameObj()->Get_Vehicle() && Get_Owner()->Get_GameObj()->Get_Vehicle()->Get_Driver() == Get_Owner()->Get_GameObj();
	}
};


class DA_API DAVeteranManagerClass : public DAEventClass, public DAGameFeatureClass {
protected:
	virtual void Init();
	virtual ~DAVeteranManagerClass();
	virtual void Level_Loaded_Event();
	virtual void Settings_Loaded_Event();
	virtual void Player_Join_Event(cPlayer* Player);
	virtual void Vehicle_Enter_Event(VehicleGameObj* Vehicle, cPlayer* Player, int Seat);
	virtual void Damage_Event(DamageableGameObj* Victim, ArmedGameObj* Damager, float Damage, unsigned Warhead, float Scale, DADamageType::Type Type);
	virtual void Kill_Event(DamageableGameObj* Victim, ArmedGameObj* Killer, float Damage, unsigned Warhead, float Scale, DADamageType::Type Type);

	bool Veteran_Chat_Command(cPlayer* Player, const DATokenClass& Text, TextMessageEnum ChatType);

	DAVeteranPlayerDataObserverClass* Get_Player_Data(cPlayer* Player);
	DAVeteranPlayerDataObserverClass* Get_Player_Data(GameObject* obj);

public:
	static void Static_Init();

	DAVeteranLevel* Get_Level_Data(int level);
	float Get_Worth(StringClass name);
	float Get_Worth(GameObject* obj);
	float Get_Points(GameObject* obj);
	float Get_Damage_Multiplier(StringClass name);
	float Get_Damage_Multiplier(GameObject* obj);
	void Give_Veteran_Points(GameObject* obj, float amount);
	StringClass Get_Veteran_Type(GameObject* obj);
	void Check_Promotions(GameObject* obj);

	int Get_Player_Level(cPlayer* Player) {
		if (DAVeteranPlayerDataObserverClass* Data = Get_Player_Data(Player)) {
			return Data->Get_Level();
		}
		else {
			return -1;
		}
	}

	int Get_Player_Level(GameObject* obj) {
		if (DAVeteranPlayerDataObserverClass* Data = Get_Player_Data(obj)) {
			return Data->Get_Level();
		}
		else {
			return -1;
		}
	}

	DAVeteranLevel* Get_Player_Level_Data(cPlayer* Player) {
		if (DAVeteranPlayerDataObserverClass* Data = Get_Player_Data(Player)) {
			return Get_Level_Data(Data->Get_Level());
		}
		else {
			return 0;
		}
	}

	DAVeteranLevel* Get_Player_Level_Data(GameObject* obj) {
		if (DAVeteranPlayerDataObserverClass* Data = Get_Player_Data(obj)) {
			return Get_Level_Data(Data->Get_Level());
		}
		else {
			return 0;
		}
	}

	bool Get_Label_Player_Rank() {
		return LabelPlayerRank;
	}

	int Get_Levels() {
		return Levels.Count();
	}

private:
	DynamicVectorClass<DAVeteranLevel*> Levels;
	HashTemplateClass<StringClass, float> Worth;
	HashTemplateClass<StringClass, float> RepairMultiplier;
	bool LabelPlayerRank;
	float RepairPointsRequiredForPoints;
	float DamagePointsRequiredForPoints;

	void Clear_Levels() {
		for(int i = 0; i < Levels.Count(); ++i) {
			delete Levels[i];
		}
		Levels.Clear();
	}
};
#endif