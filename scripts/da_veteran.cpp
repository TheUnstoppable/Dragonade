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

#include "General.h"
#include "da.h"
#include "da_settings.h"
#include "da_veteran.h"
#include "da_damagelog.h"
#include "da_vehicle.h"
#include "engine_da.h"
#include "engine_dmg.h"
#include "engine_game.h"
#include "engine_obj.h"
#include "engine_player.h"
#include "engine_weap.h"

DAVeteranLevel* DAVeteranManagerClass::Get_Level_Data(int level) {
	for (int i = 0; i < Get_Levels(); ++i) {
		if (Levels[i]->Index == level) {
			return Levels[i];
		}
	}

	return 0;
}

float DAVeteranManagerClass::Get_Worth(StringClass name) {
	float* worthScore = Worth.Get(name);
	return worthScore ? *worthScore : 0.f;
}

float DAVeteranManagerClass::Get_Worth(GameObject* obj) {
	float* worthScore;
	if (worthScore = Worth.Get(Commands->Get_Preset_Name(obj))) {
		return *worthScore;
	} else if (worthScore = Worth.Get(Get_Veteran_Type(obj))) {
		return *worthScore;
	} else {
		return 0.f;
	}
}

float DAVeteranManagerClass::Get_Points(GameObject* obj) {
	if (Commands->Is_A_Star(obj)) {
		if (obj->As_SoldierGameObj()) {
			return Get_Player_Data(((SoldierGameObj*)obj)->Get_Player())->Get_Points();
		} else if (obj->As_VehicleGameObj()) {
			return Get_Player_Data(((VehicleGameObj*)obj)->Get_Driver()->Get_Player())->Get_Points();
		} 
	}
	return 0.f;
}

float DAVeteranManagerClass::Get_Damage_Multiplier(StringClass name) {
	float* multiplier = RepairMultiplier.Get(name);
	return multiplier ? *multiplier : 1.f;
}

float DAVeteranManagerClass::Get_Damage_Multiplier(GameObject* obj) {
	float* multiplier;
	if (multiplier = RepairMultiplier.Get(Commands->Get_Preset_Name(obj))) {
		return *multiplier;
	} else if (multiplier = RepairMultiplier.Get(Get_Veteran_Type(obj))) {
		return *multiplier;
	} else {
		return 1.f;
	}
}

void DAVeteranManagerClass::Give_Veteran_Points(GameObject* obj, float amount) {
	if (amount == 0.f)
		return;

	if (Commands->Is_A_Star(obj)) {
		if (obj->As_SoldierGameObj()) {
			Get_Player_Data(obj)->Add_Points(amount);
		} else if (obj->As_VehicleGameObj()) {
			Get_Player_Data(((VehicleGameObj*)obj)->Get_Driver())->Add_Points(amount);
		}

		DA::Private_Color_Message(obj, VETCOLOR, "%s%s%.2f veteran point(s)", DA::Get_Message_Prefix(), amount < 0.f ? "-" : "+", amount);
	}
}

void DAVeteranManagerClass::Check_Promotions(GameObject* obj) {
	DAVeteranPlayerDataObserverClass* PlayerData = Get_Player_Data(obj);

	if (RepairPointsRequiredForPoints > 0.f) {
		if (PlayerData->Get_Repair_Points() >= RepairPointsRequiredForPoints) {
			Give_Veteran_Points(obj, Get_Worth("Repair"));
			PlayerData->Add_Repair_Points(RepairPointsRequiredForPoints * -1.f);
		}
	}

	if (DamagePointsRequiredForPoints > 0.f) {
		if (PlayerData->Get_Damage_Points() >= DamagePointsRequiredForPoints) {
			Give_Veteran_Points(obj, Get_Worth("Damage"));
			PlayerData->Add_Damage_Points(DamagePointsRequiredForPoints * -1.f);
		}
	}

	float Points = PlayerData->Get_Points();
	int TotalLevels = Get_Levels();
	int PlayerLevel = PlayerData->Get_Level();
	int NextLevel = PlayerLevel + 1;
	DAVeteranLevel* PromoteLevel = 0;
	while (NextLevel <= TotalLevels - 1) {
		if (DAVeteranLevel* Next = Get_Level_Data(NextLevel)) {
			if (Points >= Next->RequiredPoints) {
				PromoteLevel = Next;
				NextLevel++;
			} else break;
		}
	}

	if (PromoteLevel) {
		PlayerData->Set_Level(PromoteLevel->Index);
		Commands->Send_Custom_Event(0, obj, 47864234, 1, 0.f);
		DAPlayerClass* DAPlayer = ((SoldierGameObj*)obj)->Get_DA_Player();
		if (LabelPlayerRank) {
			DAPlayer->Remove_Tag(Get_Level_Data(PlayerLevel)->Name);
			DAPlayer->Add_Tag(PromoteLevel->Name, INT_MAX, DAPlayerFlags::PERSISTLEAVE);
		}
		DAPlayer->Set_Character_Discount(PromoteLevel->PurchaseDiscount);
		DAPlayer->Set_Vehicle_Discount(PromoteLevel->PurchaseDiscount);
		DAPlayer->Set_PowerUp_Discount(PromoteLevel->PurchaseDiscount);
		DA::Host_Message("%s has been promoted to the rank of \"%s\".", Get_Player_Name(obj), PromoteLevel->Name);
		DA::Private_Color_Message(obj, VETCOLOR, "%sCongratulations, %s! You've been promoted to the rank of \"%s\".", DA::Get_Message_Prefix(), Get_Player_Name(obj), PromoteLevel->Name);
		DA::Create_2D_Sound(Get_Object_Type(obj) ? PromoteLevel->PublicSound_GDI : PromoteLevel->PublicSound_Nod);
	}
}

StringClass DAVeteranManagerClass::Get_Veteran_Type(GameObject* obj) {
	if (Is_C4(obj)) {
		if (Get_C4_Mode(obj) == 1) {
			return VETWORTH_REMOTEC4;
		} else if (Get_C4_Mode(obj) == 2) {
			return VETWORTH_TIMEDC4;
		} else if (Get_C4_Mode(obj) == 3) {
			return VETWORTH_PROXC4;
		}
	} else if (Is_Beacon(obj)) {
		return VETWORTH_BEACON;
	} else if (BuildingGameObj* Building = obj->As_BuildingGameObj()) {
		const BuildingGameObjDef& Definition = Building->Get_Definition();
		if (Definition.Type == TYPE_POWER_PLANT) {
			return VETWORTH_POWERPLANT;
		} else if (Definition.Type == TYPE_REFINERY) {
			return VETWORTH_REFINERY;
		} else if (Definition.Type == TYPE_SOLDIER_FACTORY) {
			return VETWORTH_SOLDIERFACTORY;
		} else if (Definition.Type == TYPE_VEHICLE_FACTORY) {
			return VETWORTH_VEHICLEFACTORY;
		} else if (Definition.Type == TYPE_BASE_DEFENSE) {
			return VETWORTH_BASEDEFENSE;
		} else if (Definition.Type == TYPE_COM_CENTER) {
			return VETWORTH_COMCENTER;
		} else if (Definition.Type == TYPE_CONYARD) {
			return VETWORTH_CONYARD;
		} else if (Definition.Type == TYPE_REPAIR_BAY) {
			return VETWORTH_REPAIRBAY;
		} else if (Definition.Type == TYPE_SHRINE) {
			return VETWORTH_SHRINE;
		} else if (Definition.Type == TYPE_HELIPAD) {
			return VETWORTH_HELIPAD;
		} else if (Definition.Type == TYPE_TECH_CENTER) {
			return VETWORTH_TECHCENTER;
		} else if (Definition.Type == TYPE_NAVAL_FACTORY) {
			return VETWORTH_NAVALFACTORY;
		} else if (!strcmp(ArmorWarheadManager::Get_Armor_Name(Building->Get_Defense_Object()->Get_Shield_Type()),"CNCStructureHeavy") || !strcmp(ArmorWarheadManager::Get_Armor_Name(Building->Get_Defense_Object()->Get_Skin()), "CNCStructureHeavy")) {
			return VETWORTH_HEAVY_BUILDING;
		} else {
			return VETWORTH_WEAK_BUILDING;
		}
	} else if (VehicleGameObj* Vehicle = obj->As_VehicleGameObj()) {
		if (Is_Harvester(Vehicle)) {
			return VETWORTH_HARVESTER;
		} else if (Vehicle->Is_Turret()) {
			return VETWORTH_TURRET;
		} else {
			return VETWORTH_VEHICLE;
		}
	} else if (SoldierGameObj* Soldier = obj->As_SoldierGameObj()) {
		if (Commands->Is_A_Star(Soldier)) {
			return VETWORTH_STARSOLDIER;
		} else {
			return VETWORTH_BOTSOLDIER;
		}
	}

	return Commands->Get_Preset_Name(obj);
}

DAVeteranPlayerDataObserverClass* DAVeteranManagerClass::Get_Player_Data(cPlayer* Player) {
	return (DAVeteranPlayerDataObserverClass*)Player->Get_DA_Player()->Find_Observer("DAVeteranPlayerDataObserverClass");
}

DAVeteranPlayerDataObserverClass* DAVeteranManagerClass::Get_Player_Data(GameObject* obj) {
	if (obj->As_VehicleGameObj()) {
		return Get_Player_Data(((VehicleGameObj*)obj)->Get_Driver()->Get_Player());
	} else if (obj->As_SoldierGameObj()) {
		return Get_Player_Data(((SoldierGameObj*)obj)->Get_Player());
	} else {
		return 0;
	}
}

void DAVeteranManagerClass::Static_Init() {
	DASettingsManager::Add_Settings("da_veteran.ini");
}

void DAVeteranManagerClass::Init() {
	LabelPlayerRank = false;
	RepairPointsRequiredForPoints = 0.f;
	DamagePointsRequiredForPoints = 0.f;

	Register_Event(DAEvent::LEVELLOADED, INT_MAX);
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Event(DAEvent::VEHICLEENTER);
	Register_Event(DAEvent::PLAYERJOIN, INT_MAX);
	Register_Object_Event(DAObjectEvent::DAMAGEDEALT, DAObjectEvent::PLAYER);
	Register_Object_Event(DAObjectEvent::KILLRECEIVED, DAObjectEvent::ALL);
	Register_Object_Event(DAObjectEvent::CREATED, DAObjectEvent::PLAYER);
	Register_Chat_Command((DAECC)&DAVeteranManagerClass::Veteran_Chat_Command, "!veteran|!vet|!vets|!vetinfo|!vi|!vetstatus|!vetstats|!vs|!vetpoints|!vetpoint|!vp|!v|!xp|!exp|VetInfo", 0, DAAccessLevel::NONE, DAChatType::ALL);
}

DAVeteranManagerClass::~DAVeteranManagerClass() {
	for (SLNode<cPlayer>* z = Get_Player_List()->Head(); z; z = z->Next()) {
		if (DAPlayerObserverClass* Observer = z->Data()->Get_DA_Player()->Find_Observer("DAVeteranPlayerDataObserverClass")) {
			Observer->Set_Delete_Pending();
		}
	}

	Clear_Levels();
}

void DAVeteranManagerClass::Level_Loaded_Event() {
	for (SLNode<cPlayer>* Node = Get_Player_List()->Head(); Node; Node = Node->Next()) {
		cPlayer* Player = Node->Data();
		DAVeteranPlayerDataObserverClass* Data = Get_Player_Data(Player);

		if (Data) {
			if (LabelPlayerRank) {
				Player->Get_DA_Player()->Remove_Tag(Get_Level_Data(Data->Get_Level())->Name);
				Player->Get_DA_Player()->Add_Tag(Get_Level_Data(0)->Name, INT_MAX, DAPlayerFlags::PERSISTLEAVE);
			}
			Data->Set_Level(0);
			Data->Set_Points(0.f);
			Data->Set_Damage_Points(0.f);
			Data->Set_Repair_Points(0.f);
		} else {
			Player->Get_DA_Player()->Add_Observer(new DAVeteranPlayerDataObserverClass);
			if (LabelPlayerRank) {
				Player->Get_DA_Player()->Add_Tag(Get_Level_Data(0)->Name, INT_MAX, DAPlayerFlags::PERSISTLEAVE);
			}
		}
	}
}

void DAVeteranManagerClass::Settings_Loaded_Event() {
	int Index = 0;
	Clear_Levels();

	LabelPlayerRank = DASettingsManager::Get_Bool("LabelPlayerRank", false);
	RepairPointsRequiredForPoints = DASettingsManager::Get_Float("RepairPointsRequiredForPoints", 0.f);
	DamagePointsRequiredForPoints = DASettingsManager::Get_Float("DamagePointsRequiredForPoints", 0.f);
	
	INISection* VetLevels = DASettingsManager::Get_Section("VeteranLevels");
	for(INIEntry *Entry = VetLevels->EntryList.First(); Entry && Entry->Is_Valid(); Entry = Entry->Next()) {
		if (!strcmp(Entry->Value, "1")) {
			StringClass SectionName = StringFormat("%s_VetLevel", Entry->Entry);
			if (DASettingsManager::Get_Section(SectionName)) {
				DAVeteranLevel* Level = new DAVeteranLevel;
				
				Level->Name = Entry->Entry;
				DASettingsManager::Get_String(Level->PublicSound_GDI, SectionName, "GDIPromoteSound", "");
				DASettingsManager::Get_String(Level->PublicSound_Nod, SectionName, "NodPromoteSound", "");

				Level->Index = Index;
				Level->RequiredPoints = DASettingsManager::Get_Float(SectionName, "RequiredPoints", FLT_MAX);
				Level->InfantryHealthIncrease = (DASettingsManager::Get_Float(SectionName, "InfantryHealthIncrease", 0.f) / 100.f) + 1.f;
				Level->InfantryRegen = DASettingsManager::Get_Float(SectionName, "InfantryRegen", 0.f);
				Level->InfantryRegenInterval = DASettingsManager::Get_Float(SectionName, "InfantryRegenInterval", 0.f);
				Level->VehicleHealthIncrease = (DASettingsManager::Get_Float(SectionName, "VehicleHealthIncrease", 0.f) / 100.f) + 1.f;
				Level->VehicleRegen = DASettingsManager::Get_Float(SectionName, "VehicleRegen", 0.f);
				Level->VehicleRegenInterval = DASettingsManager::Get_Float(SectionName, "VehicleRegenInterval", 0.f);
				Level->PurchaseDiscount = 1.f - (DASettingsManager::Get_Float(SectionName, "PurchaseDiscount", 0.f) / 100.f);

				Levels.Add(Level);
				++Index;
			}
		}
	}

	Worth.Remove_All();
	INISection* VetWorths = DASettingsManager::Get_Section("VeteranWorths");
	for (INIEntry* Entry = VetWorths->EntryList.First(); Entry && Entry->Is_Valid(); Entry = Entry->Next()) {
		Worth.Insert(Entry->Entry, DASettingsManager::Get_Float("VeteranWorths", Entry->Entry, 0.f));
	}

	RepairMultiplier.Remove_All();
	INISection* VetRepairMultipliers = DASettingsManager::Get_Section("VeteranDamageMultipliers");
	for (INIEntry* Entry = VetRepairMultipliers->EntryList.First(); Entry && Entry->Is_Valid(); Entry = Entry->Next()) {
		RepairMultiplier.Insert(Entry->Entry, DASettingsManager::Get_Float("VeteranDamageMultipliers", Entry->Entry, 0.f));
	}

	// It is not safe to continue with current player data because we can't know what changed in configuration. So we reset them.
	for(SLNode<cPlayer> *Node = Get_Player_List()->Head(); Node; Node = Node->Next()) {
		cPlayer* Player = Node->Data();
		DAVeteranPlayerDataObserverClass* Data = Get_Player_Data(Player);
		if (LabelPlayerRank) {
			Player->Get_DA_Player()->Remove_Tag(Get_Level_Data(Data->Get_Level())->Name);
			Player->Get_DA_Player()->Add_Tag(Get_Level_Data(0)->Name, INT_MAX, DAPlayerFlags::PERSISTLEAVE);
		}
		Data->Set_Level(0);
		Data->Set_Points(0.f);
		Data->Set_Damage_Points(0.f);
		Data->Set_Repair_Points(0.f);
		
		Commands->Send_Custom_Event(0, Player->Get_GameObj(), 47864234, 2, 0.f);
		Commands->Send_Custom_Event(0, Player->Get_GameObj(), 47864234, 3, .1f);
		Commands->Send_Custom_Event(0, Player->Get_GameObj(), 47864234, 4, .1f);
	}
}

void DAVeteranManagerClass::Player_Join_Event(cPlayer* Player) {
	if (!Player->Get_DA_Player()->Find_Observer("DAVeteranPlayerDataObserverClass")) {
		Player->Get_DA_Player()->Add_Observer(new DAVeteranPlayerDataObserverClass);
		if (LabelPlayerRank) {
			Player->Get_DA_Player()->Add_Tag(Get_Level_Data(0)->Name, INT_MAX, DAPlayerFlags::PERSISTLEAVE);
		}
	}
}

void DAVeteranManagerClass::Vehicle_Enter_Event(VehicleGameObj* Vehicle, cPlayer* Player, int Seat) {
	if (PTTEAM(Player->Get_Player_Type()) == DAVehicleManager::Get_Vehicle_Data(Vehicle)->Get_Team()) {
		Give_Veteran_Points(Player->Get_GameObj(), Get_Worth("Steal"));
		Check_Promotions(Player->Get_GameObj());
	}
}

void DAVeteranManagerClass::Damage_Event(DamageableGameObj* Victim, ArmedGameObj* Damager, float Damage, unsigned Warhead, float Scale, DADamageType::Type Type) {
	if (Damage < 0.f && RepairPointsRequiredForPoints > 0.f && Get_Object_Type(Victim) == Get_Object_Type(Damager)) {
		Get_Player_Data(Damager)->Add_Repair_Points(WWMath::Fabs(Damage) * Get_Damage_Multiplier(Victim));
		Check_Promotions(Damager);
	} else if (Damage > 0.f && DamagePointsRequiredForPoints > 0.f && PTTEAM(Get_Object_Type(Damager)) == Get_Object_Type(Victim)) {
		Get_Player_Data(Damager)->Add_Damage_Points(Damage * Get_Damage_Multiplier(Victim));
		Check_Promotions(Damager);
	}
}

void DAVeteranManagerClass::Kill_Event(DamageableGameObj* Victim, ArmedGameObj* Killer, float Damage, unsigned Warhead, float Scale, DADamageType::Type Type) {
	float Worth = Get_Worth(Victim);
	if (Worth > 0.f) {
		if (Killer && Is_Beacon(GetExplosionObj()) && PTTEAM(Get_Object_Type(Victim)) == Get_Object_Type(Killer)) {
			Give_Veteran_Points(Killer, Worth);
			Check_Promotions(Killer);
		}
		else {
			DamageableGameObj* ReferenceObj = Victim;
			if (Victim->As_VehicleGameObj()) {
				SoldierGameObj* VehOwner = 0;
				if (cPlayer* Owner = DAVehicleManager::Get_Vehicle_Owner(Victim)) {
					VehOwner = Owner->Get_GameObj();
				}
				else {
					VehOwner = ((VehicleGameObj*)Victim)->Get_Owner();
				}
				if (VehOwner) {
					ReferenceObj = VehOwner;
				}
			}
			DynamicVectorClass<DADamageTableStruct> Damagers;
			DADamageLog::Get_Damagers_By_Percent_Other_Team(Damagers, Victim, Get_Object_Type(Victim), 0.0f, 0.0f);
			for (int i = 0; i < Damagers.Count(); i++) {
				SoldierGameObj* Damager = Damagers[i].Player->Get_GameObj();
				if (ReferenceObj->Is_Enemy(Damager)) {
					Give_Veteran_Points(Damager, Worth * Damagers[i].Damage);
					Check_Promotions(Damager);
				}
			}
		}
	}
}

bool DAVeteranManagerClass::Veteran_Chat_Command(cPlayer* Player, const DATokenClass& Text, TextMessageEnum ChatType) {
	SoldierGameObj* Soldier = Player->Get_GameObj();
	if (DAVeteranPlayerDataObserverClass* PlayerData = Get_Player_Data(Player)) {
		DA::Private_Color_Message(Player, VETCOLOR, "%sYou have %.2f veteran point(s), and you are ranked as \"%s\".", DA::Get_Message_Prefix(), Get_Points(Soldier), Get_Level_Data(PlayerData->Get_Level())->Name);
		if (PlayerData->Get_Level() + 1 <= Get_Levels() - 1) {
			DAVeteranLevel* Next = Get_Level_Data(PlayerData->Get_Level() + 1);
			DA::Private_Color_Message(Player, VETCOLOR, "%sYou need %.2f more veteran point(s) for next rank \"%s\".", DA::Get_Message_Prefix(), Next->RequiredPoints - Get_Points(Soldier), Next->Name);
		}
	}
	return false;
}

Register_Game_Feature(DAVeteranManagerClass, "Veteran", "EnableVeteran", 0);

DAVeteranLevel* DAVeteranPlayerDataObserverClass::Get_Player_Level() {
	return ((DAVeteranManagerClass*)DAVeteranManagerClassRegistrant.Get_Instance())->Get_Level_Data(Get_Level());
}

void DAVeteranPlayerDataObserverClass::Init() {
	Level = 0;
	Points = 0.f;
	RepairPoints = 0.f;
	DamagePoints = 0.f;
	
	Set_Flags(DAPlayerFlags::PERSISTLEAVE | DAPlayerFlags::PERSISTMAP | DAPlayerFlags::THINK);
	DAPlayerClass* DAPlayer = Get_Owner()->Get_DA_Player();
	DAPlayer->Set_Character_Discount(Get_Player_Level()->PurchaseDiscount);
	DAPlayer->Set_Vehicle_Discount(Get_Player_Level()->PurchaseDiscount);
	DAPlayer->Set_PowerUp_Discount(Get_Player_Level()->PurchaseDiscount);
	
	if (Get_Owner()->Get_GameObj()) {
		SoldierOriginalHealth = Commands->Get_Max_Health(Get_Owner()->Get_GameObj());
		SoldierOriginalArmor = Commands->Get_Max_Shield_Strength(Get_Owner()->Get_GameObj());
		if (Is_In_Vehicle()) {
			VehicleOriginalHealth = Commands->Get_Max_Health(Get_Owner()->Get_GameObj()->Get_Vehicle());
			VehicleOriginalArmor = Commands->Get_Max_Shield_Strength(Get_Owner()->Get_GameObj()->Get_Vehicle());
		}
	}
}

void DAVeteranPlayerDataObserverClass::Created() {
	SoldierGameObj* Soldier = Get_Owner()->Get_GameObj();
	SoldierOriginalHealth = Commands->Get_Max_Health(Soldier);
	SoldierOriginalArmor = Commands->Get_Max_Shield_Strength(Soldier);
	SoldierCalculatedHealth = Commands->Get_Max_Health(Soldier) * HALF(Get_Player_Level()->InfantryHealthIncrease);
	SoldierCalculatedArmor = Commands->Get_Max_Shield_Strength(Soldier) * HALF(Get_Player_Level()->InfantryHealthIncrease);
	Set_Max_Health(Soldier, SoldierCalculatedHealth);
	Set_Max_Shield_Strength(Soldier, SoldierCalculatedArmor);

	if (Get_Player_Level()->InfantryRegen > 0.f) {
		Stop_Timer(123123);
		Start_Timer(123123, Get_Player_Level()->InfantryRegenInterval, false, 0);
	}
}

void DAVeteranPlayerDataObserverClass::Vehicle_Enter(VehicleGameObj* Vehicle, int Seat) {
	if (Seat == 0) {
		VehicleOriginalHealth = Commands->Get_Max_Health(Vehicle);
		VehicleOriginalArmor = Commands->Get_Max_Shield_Strength(Vehicle);
		VehicleCalculatedHealth = Commands->Get_Max_Health(Vehicle) * HALF(Get_Player_Level()->VehicleHealthIncrease);
		VehicleCalculatedArmor = Commands->Get_Max_Shield_Strength(Vehicle) * HALF(Get_Player_Level()->VehicleHealthIncrease);

		Set_Max_Health_Without_Healing(Vehicle, VehicleCalculatedHealth);
		Set_Max_Shield_Strength_Without_Healing(Vehicle, VehicleCalculatedArmor);

		//Scale health and armor
		Commands->Set_Health(Vehicle, (Commands->Get_Health(Vehicle) * Commands->Get_Max_Health(Vehicle)) / VehicleOriginalHealth);
		Commands->Set_Shield_Strength(Vehicle, (Commands->Get_Shield_Strength(Vehicle) * Commands->Get_Max_Shield_Strength(Vehicle)) / VehicleOriginalArmor);

		if (Get_Player_Level()->VehicleRegen > 0.f) {
			Stop_Timer(123123);
			Start_Timer(123123, Get_Player_Level()->VehicleRegenInterval, false, 0);
		}
	}
}

void DAVeteranPlayerDataObserverClass::Vehicle_Exit(VehicleGameObj* Vehicle, int Seat) {
	if (Seat == 0) {
		//Scale health and armor
		Commands->Set_Health(Vehicle, (Commands->Get_Health(Vehicle) * VehicleOriginalHealth) / Commands->Get_Max_Health(Vehicle));
		Commands->Set_Shield_Strength(Vehicle, (Commands->Get_Shield_Strength(Vehicle) * VehicleOriginalArmor) / Commands->Get_Max_Shield_Strength(Vehicle));
		
		Set_Max_Health_Without_Healing(Vehicle, VehicleOriginalHealth);
		Set_Max_Shield_Strength_Without_Healing(Vehicle, VehicleOriginalArmor);
		VehicleOriginalHealth = VehicleOriginalArmor = VehicleCalculatedHealth = VehicleCalculatedArmor = -1;

		if (Get_Player_Level()->InfantryRegen > 0.f) {
			Stop_Timer(123123);
			Start_Timer(123123, Get_Player_Level()->InfantryRegenInterval, false, 0);
		}
	}
}

void DAVeteranPlayerDataObserverClass::Timer_Expired(int Number, unsigned int Data) {
	if (Number == 123123) {
		DAVeteranLevel* Level = Get_Player_Level();
		if ((Is_In_Vehicle() ? Level->VehicleRegen : Level->InfantryRegen) > 0.f) {
			if (Is_In_Vehicle()) {
				if (VehicleGameObj* Vehicle = Get_Owner()->Get_GameObj()->Get_Vehicle()) {
					Commands->Apply_Damage(Vehicle, -Level->VehicleRegen, "None", 0);
				}
			} else {
				if (Get_Owner()->Get_GameObj() && !Get_Owner()->Get_GameObj()->Is_Dead()) {
					Commands->Apply_Damage(Get_Owner()->Get_GameObj(), -Level->InfantryRegen, "None", 0);
				}
			}
			Start_Timer(123123, Is_In_Vehicle() ? Level->VehicleRegenInterval : Level->InfantryRegenInterval, false, 0);
		}
	}
}

void DAVeteranPlayerDataObserverClass::Custom(GameObject* Sender, int Type, int Param) {
	if (Type == 47864234) { // Base messaging event.
		if (Param == 1) { // Leveled up.
			Custom(Sender, Type, 3);
			
			SoldierGameObj* Soldier = Get_Owner()->Get_GameObj();

			SoldierCalculatedHealth = SoldierOriginalHealth * HALF(Get_Player_Level()->InfantryHealthIncrease);
			SoldierCalculatedArmor = SoldierOriginalArmor * HALF(Get_Player_Level()->InfantryHealthIncrease);

			Set_Max_Health_Without_Healing(Soldier, SoldierCalculatedHealth);
			Set_Max_Shield_Strength_Without_Healing(Soldier, SoldierCalculatedArmor);

			// Scale health and armor
			Commands->Set_Health(Soldier, (Commands->Get_Health(Soldier) * Commands->Get_Max_Health(Soldier)) / SoldierOriginalHealth);
			Commands->Set_Shield_Strength(Soldier, (Commands->Get_Shield_Strength(Soldier) * Commands->Get_Max_Shield_Strength(Soldier)) / SoldierOriginalArmor);

			if (Is_In_Vehicle()) {
				VehicleGameObj* Vehicle = Soldier->Get_Vehicle();

				VehicleCalculatedHealth = VehicleOriginalHealth * HALF(Get_Player_Level()->VehicleHealthIncrease);
				VehicleCalculatedArmor = VehicleOriginalArmor * HALF(Get_Player_Level()->VehicleHealthIncrease);

				Set_Max_Health_Without_Healing(Vehicle, VehicleCalculatedHealth);
				Set_Max_Shield_Strength_Without_Healing(Vehicle, VehicleCalculatedArmor);

				// Scale health and armor
				Commands->Set_Health(Vehicle, (Commands->Get_Health(Vehicle) * Commands->Get_Max_Health(Vehicle)) / VehicleOriginalHealth);
				Commands->Set_Shield_Strength(Vehicle, (Commands->Get_Shield_Strength(Vehicle) * Commands->Get_Max_Shield_Strength(Vehicle)) / VehicleOriginalArmor);
			}
			Custom(Sender, Type, 2);
		} else if (Param == 2) { // Reset timer.
			if (Is_In_Vehicle()) {
				if (Get_Player_Level()->VehicleRegen > 0.f) {
					Stop_Timer(123123);
					Start_Timer(123123, Get_Player_Level()->VehicleRegenInterval, false, 0);
				}
			} else {
				if (Get_Player_Level()->InfantryRegen > 0.f) {
					Stop_Timer(123123);
					Start_Timer(123123, Get_Player_Level()->InfantryRegenInterval, false, 0);
				}
			}
		} else if (Param == 3) { // Restore back the old health & armor of soldier.
			SoldierGameObj* Soldier = Get_Owner()->Get_GameObj();

			//Scale health and armor
			Commands->Set_Health(Soldier, (Commands->Get_Health(Soldier) * SoldierOriginalHealth) / Commands->Get_Max_Health(Soldier));
			Commands->Set_Shield_Strength(Soldier, (Commands->Get_Shield_Strength(Soldier) * SoldierOriginalArmor) / Commands->Get_Max_Shield_Strength(Soldier));

			Set_Max_Health_Without_Healing(Soldier, SoldierOriginalHealth);
			Set_Max_Shield_Strength_Without_Healing(Soldier, SoldierOriginalArmor);
		} else if (Param == 4) { // Restore back the old health & armor of vehicle.
			if (Is_In_Vehicle()) {
				VehicleGameObj* Vehicle = Get_Owner()->Get_GameObj()->Get_Vehicle();

				//Scale health and armor
				Commands->Set_Health(Vehicle, (Commands->Get_Health(Vehicle) * VehicleOriginalHealth) / Commands->Get_Max_Health(Vehicle));
				Commands->Set_Shield_Strength(Vehicle, (Commands->Get_Shield_Strength(Vehicle) * VehicleOriginalArmor) / Commands->Get_Max_Shield_Strength(Vehicle));

				Set_Max_Health_Without_Healing(Vehicle, VehicleOriginalHealth);
				Set_Max_Shield_Strength_Without_Healing(Vehicle, VehicleOriginalArmor);
			}
		}
	} else if (Type == 558223) { // Edit veteran points.
		Add_Points(*(float*)&Param);
		((DAVeteranManagerClass*)DAVeteranManagerClassRegistrant.Get_Instance())->Check_Promotions(Get_Owner()->Get_GameObj());
	}
}

void DAVeteranPlayerDataObserverClass::Think() {
	if (Is_In_Vehicle()) {
		VehicleGameObj* Vehicle = Get_Owner()->Get_GameObj()->Get_Vehicle();
		if (Commands->Get_Max_Health(Vehicle) - VehicleCalculatedHealth > WWMATH_EPSILON) {
			VehicleCalculatedHealth = Commands->Get_Max_Health(Vehicle);
			VehicleOriginalHealth = VehicleCalculatedHealth / HALF(Get_Player_Level()->VehicleHealthIncrease);
		}
		if (Commands->Get_Max_Shield_Strength(Vehicle) - VehicleCalculatedArmor > WWMATH_EPSILON) {
			VehicleCalculatedArmor = Commands->Get_Max_Shield_Strength(Vehicle);
			VehicleOriginalArmor = VehicleCalculatedArmor / HALF(Get_Player_Level()->VehicleHealthIncrease);
		}
	}

	SoldierGameObj* Soldier = Get_Owner()->Get_GameObj();
	if (Commands->Get_Max_Health(Soldier) - SoldierCalculatedHealth > WWMATH_EPSILON) {
		SoldierCalculatedHealth = Commands->Get_Max_Health(Soldier);
		SoldierOriginalHealth = SoldierCalculatedHealth / HALF(Get_Player_Level()->InfantryHealthIncrease);
	}
	if (Commands->Get_Max_Shield_Strength(Soldier) - SoldierCalculatedArmor > WWMATH_EPSILON) {
		SoldierCalculatedArmor = Commands->Get_Max_Shield_Strength(Soldier);
		SoldierOriginalArmor = SoldierCalculatedArmor / HALF(Get_Player_Level()->InfantryHealthIncrease);
	}
}
