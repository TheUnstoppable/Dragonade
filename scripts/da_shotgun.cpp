/*	Renegade Scripts.dll
    Dragonade Shotgun Wars Game Mode
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
#include "da_shotgun.h"

void DAShotgunGameModeClass::Init() {
	Register_Event(DAEvent::ADDWEAPONREQUEST,INT_MAX);
	Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::POWERUP | DAObjectEvent::BUILDING | DAObjectEvent::ARMED,INT_MAX);

	WeaponDefinitionClass *Shotgun = (WeaponDefinitionClass*)Find_Named_Definition("Weapon_Shotgun_Player");
	WeaponDefinitionClass *Repair = (WeaponDefinitionClass*)Find_Named_Definition("Weapon_RepairGun_Player");
	int ShotgunID = Shotgun->Get_ID();
	Shotgun->MaxInventoryRounds = -1;
	int RepairID = Repair->Get_ID();
	ArmorType LightArmor = ArmorWarheadManager::Get_Armor_Type("CNCVehicleLight");

	for (SoldierGameObjDef *Def = (SoldierGameObjDef*)DefinitionMgrClass::Get_First(CID_Soldier);Def;Def = (SoldierGameObjDef*)DefinitionMgrClass::Get_Next(Def,CID_Soldier)) {
		Def->WeaponDefID = ShotgunID;
		Def->SecondaryWeaponDefID = RepairID;
		Def->WeaponRounds = -1;
	}

	for (VehicleGameObjDef *Def = (VehicleGameObjDef*)DefinitionMgrClass::Get_First(CID_Vehicle); Def; Def = (VehicleGameObjDef*)DefinitionMgrClass::Get_Next(Def, CID_Vehicle)) {
		if (Def->Get_Type() != VehicleType::VEHICLE_TYPE_TURRET) {
			Def->WeaponDefID = ShotgunID;
			Def->WeaponRounds = -1;
		}
		if (Def->DefenseObjectDef.ShieldType != ArmorType(1)) {
			Def->DefenseObjectDef.ShieldType = LightArmor;
			Def->DefenseObjectDef.Skin = LightArmor;
		}
	}
}

bool DAShotgunGameModeClass::Add_Weapon_Request_Event(cPlayer *Player,const WeaponDefinitionClass *Weapon) {
	if (_stricmp(Weapon->Get_Name(),"Weapon_Shotgun_Player") && _stricmp(Weapon->Get_Name(), "Weapon_RepairGun_Player")) {
		return false;
	}
	return true;
}

void DAShotgunGameModeClass::Object_Created_Event(GameObject *obj) {
	if (BuildingGameObj *Building = obj->As_BuildingGameObj()) {
		Building->Get_Defense_Object()->Set_Skin(ArmorWarheadManager::Get_Armor_Type("CNCVehicleMedium"));
		Building->Get_Defense_Object()->Set_Shield_Type(ArmorWarheadManager::Get_Armor_Type("CNCVehicleMedium"));
		const_cast<BuildingGameObjDef&>(Building->Get_Definition()).MCTSkin = ArmorWarheadManager::Get_Armor_Type("CNCVehicleLight");
	}
	else if (VehicleGameObj *Vehicle = obj->As_VehicleGameObj()) {
		if (!_stricmp(Vehicle->Get_Weapon()->Get_Name(),"Weapon_Shotgun_Player")) {
			Update_Network_Object(Vehicle);
			Set_Position_Clip_Bullets(Vehicle,1,-1);
		}
		if (Vehicle->Get_Defense_Object()->Get_Shield_Type() != ArmorType(1)) {
			ArmorType LightArmor = ArmorWarheadManager::Get_Armor_Type("CNCVehicleLight");
			Vehicle->Get_Defense_Object()->Set_Shield_Type(LightArmor);
			Vehicle->Get_Defense_Object()->Set_Skin(LightArmor);
		}
	}
	else if (SoldierGameObj *Soldier = obj->As_SoldierGameObj()) {
		if (Soldier->Get_Player()) {
			Update_Network_Object(Soldier->Get_Player());
		}
		Update_Network_Object(Soldier);
		Set_Position_Clip_Bullets(Soldier,1,-1);
	}
	else if (((PowerUpGameObj*)obj)->Get_Definition().GrantWeapon) {
		obj->Set_Delete_Pending();
	}
}

Register_Game_Mode(DAShotgunGameModeClass,"Shotgun","Shotgun Wars",0);