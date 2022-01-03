/*	Renegade Scripts.dll
    Dragonade Disable List
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
#include "da_disable.h"
#include "da_settings.h"
#include "da_vehicle.h"
#include "BaseControllerClass.h"
#include "RefineryGameObj.h"
#include "GameObjManager.h"
#include "PurchaseSettingsDefClass.h"
#include "PowerupGameObjDef.h"

DynamicVectorClass<const DefinitionClass*> DADisableListManager::DisableList;
HashTemplateClass<unsigned int,DACharacterDisableListStruct> DADisableListManager::CharacterDisableList;

void DADisableListManager::Init() {
	static DADisableListManager Instance;
	Instance.Register_Event(DAEvent::SETTINGSLOADED,INT_MAX-1);
	Instance.Register_Event(DAEvent::LEVELLOADED,INT_MAX-1);
	Instance.Register_Event(DAEvent::CHARACTERPURCHASEREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEPURCHASEREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::POWERUPPURCHASEREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::POWERUPGRANTREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::ADDWEAPONREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEENTRYREQUEST,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::ALL,INT_MAX);
}

bool DADisableListManager::Is_Preset_Disabled(GameObject *obj) {
	if (obj) {
		return (DisableList.ID(&obj->Get_Definition()) != -1);
	}
	return false;
}

bool DADisableListManager::Is_Preset_Disabled(const DefinitionClass *Def) {
	return (DisableList.ID(Def) != -1);
}

bool DADisableListManager::Is_Preset_Disabled(const char *Preset) {
	return (DisableList.ID(Find_Named_Definition(Preset)) != -1);
}

bool DADisableListManager::Is_Preset_Disabled(unsigned int DefID) {
	return (DisableList.ID(Find_Definition(DefID)) != -1);
}

bool DADisableListManager::Is_Preset_Disabled_For_Character(const DefinitionClass *Character,const DefinitionClass *Def) {
	DACharacterDisableListStruct *List = CharacterDisableList.Get((unsigned int)Character);
	if (List) {
		return (List->DisableList.ID(Def) != -1);
	}
	return false;
}

bool DADisableListManager::Is_Preset_Disabled_For_Character(const DefinitionClass *Character,const char *Preset) {
	return Is_Preset_Disabled_For_Character(Character,Find_Named_Definition(Preset));
}

bool DADisableListManager::Is_Preset_Disabled_For_Character(const DefinitionClass *Character,unsigned int DefID) {
	return Is_Preset_Disabled_For_Character(Character,Find_Definition(DefID));
}

bool DADisableListManager::Can_Character_Enter_Vehicles(const DefinitionClass *Character) {
	DACharacterDisableListStruct *List = CharacterDisableList.Get((unsigned int)Character);
	if (List) {
		return !List->EnterVehicles;
	}
	return true;
}

bool DADisableListManager::Can_Character_Drive_Vehicles(const DefinitionClass *Character) {
	DACharacterDisableListStruct *List = CharacterDisableList.Get((unsigned int)Character);
	if (List) {
		return !List->DriveVehicles;
	}
	return true;
}

bool DADisableListManager::Can_Character_Steal_Vehicles(const DefinitionClass *Character) {
	DACharacterDisableListStruct *List = CharacterDisableList.Get((unsigned int)Character);
	if (List) {
		return !List->StealVehicles;
	}
	return true;
}

void DADisableListManager::Settings_Loaded_Event() {
	//Disable list
	DisableList.Delete_All();
	INISection *Section = DASettingsManager::Get_Section("Disable_List");
	if (Section) {
		for (INIEntry *i = Section->EntryList.First();i && i->Is_Valid();i = i->Next()) {
			if (!_stricmp(i->Value,"1") || !_stricmp(i->Value,"true")) {
				DefinitionClass *Def = Find_Named_Definition(i->Entry);
				if (Def) {
					Hide_Preset_By_Name(0,Def->Get_Name(),true);
					Hide_Preset_By_Name(1,Def->Get_Name(),true);
					DisableList.Add(Def);
				}
			}
		}
	}
	if (DASettingsManager::Get_Bool("Disable_List","Beacons",false)) {
		for (PowerUpGameObjDef *PowerUpDef = (PowerUpGameObjDef*)DefinitionMgrClass::Get_First(CID_PowerUp);PowerUpDef;PowerUpDef = (PowerUpGameObjDef*)DefinitionMgrClass::Get_Next(PowerUpDef,CID_PowerUp)) {
			if (PowerUpDef->GrantWeapon) {
				WeaponDefinitionClass *WeaponDef = (WeaponDefinitionClass*)Find_Definition(PowerUpDef->GrantWeaponID);
				if (WeaponDef && WeaponDef->Style == STYLE_BEACON) {
					Hide_Preset_By_Name(0,PowerUpDef->Get_Name(),true);
					Hide_Preset_By_Name(1,PowerUpDef->Get_Name(),true);
					DisableList.Add(PowerUpDef);
				}
			}
		}
		for (WeaponDefinitionClass *WeaponDef = (WeaponDefinitionClass*)DefinitionMgrClass::Get_First(CID_Weapon);WeaponDef;WeaponDef = (WeaponDefinitionClass*)DefinitionMgrClass::Get_Next(WeaponDef,CID_Weapon)) {
			if (WeaponDef->Style == STYLE_BEACON) {
				DisableList.Add(WeaponDef);
			}
		}
	}
	if (DASettingsManager::Get_Bool("Disable_List","C4",false)) {
		for (PowerUpGameObjDef *PowerUpDef = (PowerUpGameObjDef*)DefinitionMgrClass::Get_First(CID_PowerUp);PowerUpDef;PowerUpDef = (PowerUpGameObjDef*)DefinitionMgrClass::Get_Next(PowerUpDef,CID_PowerUp)) {
			if (PowerUpDef->GrantWeapon) {
				WeaponDefinitionClass *WeaponDef = (WeaponDefinitionClass*)Find_Definition(PowerUpDef->GrantWeaponID);
				if (WeaponDef && WeaponDef->Style == STYLE_C4) {
					Hide_Preset_By_Name(0,PowerUpDef->Get_Name(),true);
					Hide_Preset_By_Name(1,PowerUpDef->Get_Name(),true);
					DisableList.Add(PowerUpDef);
				}
			}
		}
		for (WeaponDefinitionClass *WeaponDef = (WeaponDefinitionClass*)DefinitionMgrClass::Get_First(CID_Weapon);WeaponDef;WeaponDef = (WeaponDefinitionClass*)DefinitionMgrClass::Get_Next(WeaponDef,CID_Weapon)) {
			if (WeaponDef->Style == STYLE_C4) {
				DisableList.Add(WeaponDef);
			}
		}
	}
	if (DASettingsManager::Get_Bool("Disable_List","Vehicles",false)) {
		for (VehicleGameObjDef *VehicleDef = (VehicleGameObjDef*)DefinitionMgrClass::Get_First(CID_Vehicle);VehicleDef;VehicleDef = (VehicleGameObjDef*)DefinitionMgrClass::Get_Next(VehicleDef,CID_Vehicle)) {
			if (VehicleDef->Get_Type() != VEHICLE_TYPE_TURRET && VehicleDef->Get_ID() != (unsigned int)Get_Harvester_Preset_ID(0) && VehicleDef->Get_ID() != (unsigned int)Get_Harvester_Preset_ID(1)) {
				Hide_Preset_By_Name(0,VehicleDef->Get_Name(),true);
				Hide_Preset_By_Name(1,VehicleDef->Get_Name(),true);
				DisableList.Add(VehicleDef);
			}
		}
		Set_Vehicle_Limit(0);
	}
	if (DASettingsManager::Get_Bool("Disable_List","Harvesters",false)) {
		for (RefineryGameObjDef *Ref = (RefineryGameObjDef*)DefinitionMgrClass::Get_First(CID_Refinery);Ref;Ref = (RefineryGameObjDef*)DefinitionMgrClass::Get_Next(Ref,CID_Refinery)) {
			Ref->HarvesterDefID = 0;
		}
	}
	if (DASettingsManager::Get_Bool("Disable_List","Refills",false)) {
		Register_Event(DAEvent::REFILL,INT_MAX);
	}
	else {
		Unregister_Event(DAEvent::REFILL);
	}

	//Per-character disable list
	CharacterDisableList.Remove_All();
	for (DefinitionClass *Def = DefinitionMgrClass::Get_First(CID_Soldier);Def;Def = DefinitionMgrClass::Get_Next(Def,CID_Soldier)) {
		StringClass SectionName;
		SectionName.Format("%s_Disable_List",Def->Get_Name());
		INISection *Section = DASettingsManager::Get_Section(SectionName);
		if (Section) {
			DACharacterDisableListStruct Disable;
			for (INIEntry *i = Section->EntryList.First();i && i->Is_Valid();i = i->Next()) {
				if (!_stricmp(i->Value,"1") || !_stricmp(i->Value,"true")) {
					DefinitionClass *Def = Find_Named_Definition(i->Entry);
					if (Def && (Def->Get_Class_ID() == CID_PowerUp || Def->Get_Class_ID() == CID_Weapon)) {
						Disable.DisableList.Add(Def);
					}
				}
			}
			if (DASettingsManager::Get_Bool(SectionName,"Beacons",false)) {
				for (PowerUpGameObjDef *PowerUpDef = (PowerUpGameObjDef*)DefinitionMgrClass::Get_First(CID_PowerUp);PowerUpDef;PowerUpDef = (PowerUpGameObjDef*)DefinitionMgrClass::Get_Next(PowerUpDef,CID_PowerUp)) {
					if (PowerUpDef->GrantWeapon) {
						WeaponDefinitionClass *WeaponDef = (WeaponDefinitionClass*)Find_Definition(PowerUpDef->GrantWeaponID);
						if (WeaponDef && WeaponDef->Style == STYLE_BEACON) {
							Disable.DisableList.Add(PowerUpDef);
						}
					}
				}
				for (WeaponDefinitionClass *WeaponDef = (WeaponDefinitionClass*)DefinitionMgrClass::Get_First(CID_Weapon);WeaponDef;WeaponDef = (WeaponDefinitionClass*)DefinitionMgrClass::Get_Next(WeaponDef,CID_Weapon)) {
					if (WeaponDef->Style == STYLE_BEACON) {
						Disable.DisableList.Add(WeaponDef);
					}
				}
			}
			if (DASettingsManager::Get_Bool(SectionName,"C4",false)) {
				for (PowerUpGameObjDef *PowerUpDef = (PowerUpGameObjDef*)DefinitionMgrClass::Get_First(CID_PowerUp);PowerUpDef;PowerUpDef = (PowerUpGameObjDef*)DefinitionMgrClass::Get_Next(PowerUpDef,CID_PowerUp)) {
					if (PowerUpDef->GrantWeapon) {
						WeaponDefinitionClass *WeaponDef = (WeaponDefinitionClass*)Find_Definition(PowerUpDef->GrantWeaponID);
						if (WeaponDef && WeaponDef->Style == STYLE_C4) {
							Disable.DisableList.Add(PowerUpDef);
						}
					}
				}
				for (WeaponDefinitionClass *WeaponDef = (WeaponDefinitionClass*)DefinitionMgrClass::Get_First(CID_Weapon);WeaponDef;WeaponDef = (WeaponDefinitionClass*)DefinitionMgrClass::Get_Next(WeaponDef,CID_Weapon)) {
					if (WeaponDef->Style == STYLE_C4) {
						Disable.DisableList.Add(WeaponDef);
					}
				}
			}
			Disable.EnterVehicles = DASettingsManager::Get_Bool(SectionName,"EnterVehicles",false);
			Disable.DriveVehicles = DASettingsManager::Get_Bool(SectionName,"DriveVehicles",false);
			Disable.StealVehicles = DASettingsManager::Get_Bool(SectionName,"StealVehicles",false);
			CharacterDisableList.Insert((unsigned int)Def,Disable);
		}
	}
}

void DADisableListManager::Level_Loaded_Event() { //Buildings can only be deleted on level load.
	if (DASettingsManager::Get_Bool("Disable_List","Buildings",false)) {
		BaseControllerClass *Base = BaseControllerClass::Find_Base(0);
		for (int i = Base->Get_Building_List().Count()-1;i >= 0;i--) {
			Base->Get_Building_List()[i]->Set_Delete_Pending();
			Base->Get_Building_List()[i]->Clear_Object_Dirty_Bits();
			Base->Get_Building_List().Delete(i);
		}
		Base = BaseControllerClass::Find_Base(1);
		for (int i = Base->Get_Building_List().Count()-1;i >= 0;i--) {
			Base->Get_Building_List()[i]->Set_Delete_Pending();
			Base->Get_Building_List()[i]->Clear_Object_Dirty_Bits();
			Base->Get_Building_List().Delete(i);
		}
		for (SLNode<VehicleGameObj> *z = GameObjManager::VehicleGameObjList.Head();z;z = z->Next()) {
			if (z->Data()->Is_Turret()) {
				z->Data()->Set_Delete_Pending();
				z->Data()->Clear_Object_Dirty_Bits();
			}
		}
	}
	if (DASettingsManager::Get_Bool("Disable_List","BaseDefenses",false)) {
		Disable_Building_Type(0,BuildingConstants::TYPE_BASE_DEFENSE);
		Disable_Building_Type(1,BuildingConstants::TYPE_BASE_DEFENSE);
		for (SLNode<VehicleGameObj> *z = GameObjManager::VehicleGameObjList.Head();z;z = z->Next()) {
			if (z->Data()->Is_Turret()) {
				z->Data()->Set_Delete_Pending();
				z->Data()->Clear_Object_Dirty_Bits();
			}
		}
	}
	if (DASettingsManager::Get_Bool("Disable_List","PowerPlants",false)) {
		Disable_Building_Type(0,BuildingConstants::TYPE_POWER_PLANT);
		Disable_Building_Type(1,BuildingConstants::TYPE_POWER_PLANT);
	}
	if (DASettingsManager::Get_Bool("Disable_List","Refineries",false)) {
		Disable_Building_Type(0,BuildingConstants::TYPE_REFINERY);
		Disable_Building_Type(1,BuildingConstants::TYPE_REFINERY);
	}
	if (DASettingsManager::Get_Bool("Disable_List","SoldierFactories",false)) {
		Disable_Building_Type(0,BuildingConstants::TYPE_SOLDIER_FACTORY);
		Disable_Building_Type(1,BuildingConstants::TYPE_SOLDIER_FACTORY);
	}
	if (DASettingsManager::Get_Bool("Disable_List","VehicleFactories",false)) {
		Disable_Building_Type(0,BuildingConstants::TYPE_VEHICLE_FACTORY);
		Disable_Building_Type(1,BuildingConstants::TYPE_VEHICLE_FACTORY);
	}
	if (DASettingsManager::Get_Bool("Disable_List","Helipads",false)) {
		Disable_Building_Type(0,BuildingConstants::TYPE_HELIPAD);
		Disable_Building_Type(1,BuildingConstants::TYPE_HELIPAD);
	}
	if (DASettingsManager::Get_Bool("Disable_List","NavalFactories",false)) {
		Disable_Building_Type(0,BuildingConstants::TYPE_NAVAL_FACTORY);
		Disable_Building_Type(1,BuildingConstants::TYPE_NAVAL_FACTORY);
	}
	if (DASettingsManager::Get_Bool("Disable_List","RepairPads",false)) {
		Disable_Building_Type(0,BuildingConstants::TYPE_REPAIR_BAY);
		Disable_Building_Type(1,BuildingConstants::TYPE_REPAIR_BAY);
	}
	if (DASettingsManager::Get_Bool("Disable_List","CommCenters",false)) {
		Disable_Building_Type(0,BuildingConstants::TYPE_COM_CENTER);
		Disable_Building_Type(1,BuildingConstants::TYPE_COM_CENTER);
	}
	if (DASettingsManager::Get_Bool("Disable_List","ConYards",false)) {
		Disable_Building_Type(0,BuildingConstants::TYPE_CONYARD);
		Disable_Building_Type(1,BuildingConstants::TYPE_CONYARD);
	}
	if (DASettingsManager::Get_Bool("Disable_List","TechCenters",false)) {
		Disable_Building_Type(0,BuildingConstants::TYPE_TECH_CENTER);
		Disable_Building_Type(1,BuildingConstants::TYPE_TECH_CENTER);
	}
}

void DADisableListManager::Object_Created_Event(GameObject *obj) {
	if (Is_Preset_Disabled(obj)) {
		obj->Set_Delete_Pending();
		obj->Clear_Object_Dirty_Bits();
	}
	else if (Is_Powerup(obj)) {
		if (Is_Preset_Disabled(((const PowerUpGameObjDef&)obj->Get_Definition()).GrantWeaponID)) {
			obj->Set_Delete_Pending();
			obj->Clear_Object_Dirty_Bits();
		}
	}
}

int DADisableListManager::Character_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const SoldierGameObjDef *Item) {
	if (Is_Preset_Disabled(Item)) {
		return 4;
	}
	return -1;
}

int DADisableListManager::Vehicle_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const VehicleGameObjDef *Item) {
	if (Is_Preset_Disabled(Item)) {
		return 4;
	}
	return -1;
}

int DADisableListManager::PowerUp_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const PowerUpGameObjDef *Item) {	
	if (Is_Preset_Disabled(Item) || Is_Preset_Disabled_For_Character(&Player->Get_GameObj()->Get_Definition(),Item)) {
		return 4;
	}
	return -1;
}

bool DADisableListManager::PowerUp_Grant_Request_Event(cPlayer *Player,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) {
	if (Is_Preset_Disabled(PowerUp) || Is_Preset_Disabled_For_Character(&Player->Get_GameObj()->Get_Definition(),PowerUp)) {
		return false;
	}
	return true;
}

bool DADisableListManager::Add_Weapon_Request_Event(cPlayer *Player,const WeaponDefinitionClass *Weapon) {
	if (Is_Preset_Disabled(Weapon) || Is_Preset_Disabled_For_Character(&Player->Get_GameObj()->Get_Definition(),Weapon)) {
		return false;
	}
	return true;
}

bool DADisableListManager::Refill_Event(cPlayer *Player) {
	return false;
}

bool DADisableListManager::Vehicle_Entry_Request_Event(VehicleGameObj *Vehicle,cPlayer *Player,int &Seat) {
	if (!Can_Character_Enter_Vehicles(&Player->Get_GameObj()->Get_Definition())) {
		DA::Page_Player(Player,"Your current character cannot enter vehicles.");
		return false;
	}
	else if (Seat == 0 && !Can_Character_Drive_Vehicles(&Player->Get_GameObj()->Get_Definition())) {
		DA::Page_Player(Player,"Your current character cannot drive vehicles.");
		Seat = Find_Empty_Vehicle_Seat(Vehicle,false);
		return true;
	}
	else {
		int Team = DAVehicleManager::Get_Team(Vehicle);
		if (Team != -2 && Team != Player->Get_Player_Type() && !Can_Character_Steal_Vehicles(&Player->Get_GameObj()->Get_Definition())) {
			DA::Page_Player(Player,"Your current character cannot steal vehicles.");
			return false;
		}
	}
	return true;
}