/*	Renegade Scripts.dll
    Dragonade Infinite Ammo Game Feature
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
#include "da.h"
#include "engine.h"
#include "engine_da.h"
#include "da_settings.h"
#include "da_infammo.h"
#include "WeaponClass.h"
#include "DefinitionMgrClass.h"

void DAInfiniteAmmoGameFeatureClass::Init() {
	Register_Event(DAEvent::LEVELLOADED);
	Register_Event(DAEvent::ADDWEAPON);
}

void DAInfiniteAmmoGameFeatureClass::Level_Loaded_Event() {
	for (WeaponDefinitionClass *Def = (WeaponDefinitionClass*)DefinitionMgrClass::Get_First(CID_Weapon);Def;Def = (WeaponDefinitionClass*)DefinitionMgrClass::Get_Next(Def,CID_Weapon)) {
		if (Def->Style != STYLE_C4 && Def->Style != STYLE_BEACON) {
			Def->MaxInventoryRounds = -1;
		}
	}
}

void DAInfiniteAmmoGameFeatureClass::Add_Weapon_Event(cPlayer *Player,WeaponClass *Weapon) {
	if (Weapon->Get_Definition()->Style != STYLE_C4 && Weapon->Get_Definition()->Style != STYLE_BEACON) {
		SoldierGameObj *Soldier = Player->Get_GameObj();
		Update_Network_Object(Player);
		Update_Network_Object(Soldier);
		Set_Position_Clip_Bullets(Soldier,Soldier->Get_Weapon_Bag()->Get_Weapon_Position(Weapon),-1);
	}
}

Register_Game_Feature(DAInfiniteAmmoGameFeatureClass,"Infinite Ammo","EnableInfiniteAmmo",0);





void DANoReloadPlayerObserverClass::Created() {
	Stop_Timer(1);
	Start_Timer(1,1.0f,true);
}

void DANoReloadPlayerObserverClass::Destroyed() {
	Stop_Timer(1);
}

void DANoReloadPlayerObserverClass::Timer_Expired(int Number,unsigned int Data) {
	if (Get_GameObj()) {
		WeaponClass *Weapon = Get_GameObj()->Get_Weapon();
		if (Weapon) {
			Set_Current_Bullets(Get_GameObj(),Weapon->Get_Definition()->ClipSize);
		}
	}
}

void DANoReloadGameFeatureClass::Init() {
	Register_Event(DAEvent::LEVELLOADED);
	Register_Event(DAEvent::PLAYERLOADED);
}

void DANoReloadGameFeatureClass::Level_Loaded_Event() {
	for (WeaponDefinitionClass *Def = (WeaponDefinitionClass*)DefinitionMgrClass::Get_First(CID_Weapon);Def;Def = (WeaponDefinitionClass*)DefinitionMgrClass::Get_Next(Def,CID_Weapon)) {
		if (Def->Style != STYLE_C4 && Def->Style != STYLE_BEACON) {
			AmmoDefinitionClass *AmmoDef = (AmmoDefinitionClass*)Find_Definition(Def->PrimaryAmmoDefID);
			if (AmmoDef) {
				AmmoDef->SprayBulletCost = 0;
			}
		}
	}
}

void DANoReloadGameFeatureClass::Player_Loaded_Event(cPlayer *Player) {
	if (Player->Get_DA_Player()->Is_TT_Client()) {
		Player->Get_DA_Player()->Add_Observer(new DANoReloadPlayerObserverClass);
	}
}

Register_Game_Feature(DANoReloadGameFeatureClass,"No Reload","EnableNoReload",0);