/*	Renegade Scripts.dll
    Dragonade Soldier Manager
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
#include "da_soldier.h"
#include "da_settings.h"
#include "da_translation.h"
#include "da_log.h"
#include "TeamPurchaseSettingsDefClass.h"

HashTemplateClass<unsigned int, const WeaponDefinitionClass*> DASoldierManager::ReplaceWeapons;
HashTemplateClass<unsigned int, DynamicVectorClass<const WeaponDefinitionClass*>> DASoldierManager::ExclusiveWeapons;
HashTemplateClass<unsigned int, DynamicVectorClass<const WeaponDefinitionClass*>> DASoldierManager::RemoveWeapons;

void DASoldierManager::Init() {
	static DASoldierManager Instance;
	Instance.Register_Event(DAEvent::SETTINGSLOADED, INT_MAX);
	Instance.Register_Event(DAEvent::CHARACTERPURCHASEREQUEST, INT_MIN);
	Instance.Register_Event(DAEvent::ADDWEAPONREQUEST, INT_MAX);
	Instance.Register_Event(DAEvent::ADDWEAPON, INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::CREATED, DAObjectEvent::PLAYER, INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::DESTROYED, DAObjectEvent::PLAYER, INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::KILLRECEIVED, DAObjectEvent::SOLDIER, INT_MAX);
}

void DASoldierManager::Settings_Loaded_Event() {
	//Spawn characters
	StringClass Buffer;
	DASettingsManager::Get_String(Buffer, "NodSpawnCharacter", 0);
	if (!Buffer.Is_Empty()) {
		Set_Nod_Soldier_Name(Buffer);
	}
	DASettingsManager::Get_String(Buffer, "GDISpawnCharacter", 0);
	if (!Buffer.Is_Empty()) {
		Set_GDI_Soldier_Name(Buffer);
	}

	//Grant weapons
	GrantWeapons.Remove_All();
	INISection* Section = DASettingsManager::Get_Section("Grant_Weapons");
	if (Section) {
		for (INIEntry* i = Section->EntryList.First(); i && i->Is_Valid(); i = i->Next()) {
			DefinitionClass* Def = Find_Named_Definition(i->Entry);
			if (Def && Def->Get_Class_ID() == CID_Soldier) {
				DynamicVectorClass<const WeaponDefinitionClass*> Weapons;
				DATokenParserClass Parser(i->Value, '|');
				while (const char* Token = Parser.Get_String()) {
					DefinitionClass* Def2 = Find_Named_Definition(Token);
					if (Def2 && Def2->Get_Class_ID() == CID_Weapon) {
						Weapons.Add((const WeaponDefinitionClass*)Def2);
					}
				}
				if (Weapons.Count()) {
					GrantWeapons.Insert((unsigned int)Def, Weapons);
				}
			}
		}
	}

	//Exclusive weapons
	ExclusiveWeapons.Remove_All();
	Section = DASettingsManager::Get_Section("Exclusive_Weapons");
	if (Section) {
		for (INIEntry* i = Section->EntryList.First(); i && i->Is_Valid(); i = i->Next()) {
			DefinitionClass* Def = Find_Named_Definition(i->Entry);
			if (Def && Def->Get_Class_ID() == CID_Weapon) {
				DynamicVectorClass<const WeaponDefinitionClass*> Weapons;
				DATokenParserClass Parser(i->Value, '|');
				while (const char* Token = Parser.Get_String()) {
					DefinitionClass* Def2 = Find_Named_Definition(Token);
					if (Def2 && Def2->Get_Class_ID() == CID_Weapon) {
						Weapons.Add((const WeaponDefinitionClass*)Def2);
					}
				}
				if (Weapons.Count()) {
					ExclusiveWeapons.Insert((unsigned int)Def, Weapons);
				}
			}
		}
	}

	//Replace weapons
	ReplaceWeapons.Remove_All();
	Section = DASettingsManager::Get_Section("Replace_Weapons");
	if (Section) {
		for (INIEntry* i = Section->EntryList.First(); i && i->Is_Valid(); i = i->Next()) {
			DefinitionClass* OldWeaponDef = Find_Named_Definition(i->Entry);
			DefinitionClass* NewWeaponDef = Find_Named_Definition(i->Value);
			if (OldWeaponDef && OldWeaponDef->Get_Class_ID() == CID_Weapon && NewWeaponDef && NewWeaponDef->Get_Class_ID() == CID_Weapon) {
				ReplaceWeapons.Insert((unsigned int)OldWeaponDef, (WeaponDefinitionClass*)NewWeaponDef);
				for (PowerUpGameObjDef* PowerUpDef = (PowerUpGameObjDef*)DefinitionMgrClass::Get_First(CID_PowerUp); PowerUpDef; PowerUpDef = (PowerUpGameObjDef*)DefinitionMgrClass::Get_Next(PowerUpDef, CID_PowerUp)) {
					if ((unsigned int)PowerUpDef->GrantWeaponID == OldWeaponDef->Get_ID()) {
						PowerUpDef->GrantWeaponID = NewWeaponDef->Get_ID();
					}
				}
				for (SoldierGameObjDef* SoldierDef = (SoldierGameObjDef*)DefinitionMgrClass::Get_First(CID_Soldier); SoldierDef; SoldierDef = (SoldierGameObjDef*)DefinitionMgrClass::Get_Next(SoldierDef, CID_Soldier)) {
					if ((unsigned int)SoldierDef->WeaponDefID == OldWeaponDef->Get_ID()) {
						SoldierDef->WeaponDefID = NewWeaponDef->Get_ID();
					}
					if ((unsigned int)SoldierDef->SecondaryWeaponDefID == OldWeaponDef->Get_ID()) {
						SoldierDef->SecondaryWeaponDefID = NewWeaponDef->Get_ID();
					}
				}
			}
		}
	}

	//Remove weapons
	RemoveWeapons.Remove_All();
	Section = DASettingsManager::Get_Section("Remove_Weapons");
	if (Section) {
		for (INIEntry* i = Section->EntryList.First(); i && i->Is_Valid(); i = i->Next()) {
			DefinitionClass* Def = Find_Named_Definition(i->Entry);
			if (Def && Def->Get_Class_ID() == CID_Weapon) {
				DynamicVectorClass<const WeaponDefinitionClass*> Weapons;
				DATokenParserClass Parser(i->Value, '|');
				while (const char* Token = Parser.Get_String()) {
					DefinitionClass* Def2 = Find_Named_Definition(Token);
					if (Def2 && Def2->Get_Class_ID() == CID_Weapon) {
						Weapons.Add((const WeaponDefinitionClass*)Def2);
					}
				}
				if (Weapons.Count()) {
					RemoveWeapons.Insert((unsigned int)Def, Weapons);
				}
			}
		}
	}
}

void DASoldierManager::Object_Created_Event(GameObject* obj) {
	DynamicVectorClass<const WeaponDefinitionClass*>* Weapons = GrantWeapons.Get((unsigned int)&obj->Get_Definition());
	if (Weapons) {
		WeaponBagClass* Bag = ((SoldierGameObj*)obj)->Get_Weapon_Bag();
		for (int i = 0; i < Weapons->Count(); i++) {
			Bag->Add_Weapon(Weapons->operator[](i), 999, true);
		}
	}
}

void DASoldierManager::Kill_Event(DamageableGameObj* Victim, ArmedGameObj* Killer, float Damage, unsigned int Warhead, float Scale, DADamageType::Type Type) {
	StringClass Message;
	if (((SoldierGameObj*)Victim)->Get_Player()) { //Player
		if (!Killer) { //No killer
			if (Type == DADamageType::FALL) {
				Message.Format("%d %ls had a great fall (%s)", Victim->Get_Player_Type(), Get_Wide_Player_Name(Victim), DATranslationManager::Translate_Soldier(Victim));
			}
			else if (Type == DADamageType::TIBERIUM) {
				Message.Format("%d %ls took a stroll through the Tiberium (%s)", Victim->Get_Player_Type(), Get_Wide_Player_Name(Victim), DATranslationManager::Translate_Soldier(Victim));
			}
			else {
				Message.Format("%d %ls was killed (%s)", Victim->Get_Player_Type(), Get_Wide_Player_Name(Victim), DATranslationManager::Translate_Soldier(Victim));
			}
		}
		else if (Killer->As_VehicleGameObj()) { //Killed by vehicle. Could be harvester, defense, or AI vehicle.
			StringClass KillerTranslation;
			if (Killer->Get_Defense_Object()->Get_Shield_Type() == 1 || Is_Harvester_Preset(Killer)) {
				KillerTranslation = "The " + DATranslationManager::Translate_With_Team_Name(Killer);
			}
			else if (((VehicleGameObj*)Killer)->Is_Turret()) {
				KillerTranslation = A_Or_An_Prepend(DATranslationManager::Translate_With_Team_Name(Killer));
			}
			else if (Is_Smart_Bot(Killer)) {
				KillerTranslation = ((VehicleGameObj*)Killer)->Get_Driver()->Get_Bot_Tag();
			}
			else {
				KillerTranslation = A_Or_An_Prepend(DATranslationManager::Translate(Killer));
			}

			if (Type == DADamageType::SQUISH) {
				Message.Format("%d %s killed %ls SQUISH! (%s %VS. %s)", Killer->Get_Player_Type(), KillerTranslation, Get_Wide_Player_Name(Victim), DATranslationManager::Translate(Killer), DATranslationManager::Translate_Soldier(Victim));
			}
			else if (Type == DADamageType::HEADSHOT || Type == DADamageType::NECKSHOT) {
				Message.Format("%d %s killed %ls HEADSHOT! (%s VS. %s)", Killer->Get_Player_Type(), KillerTranslation, Get_Wide_Player_Name(Victim), DATranslationManager::Translate(Killer), DATranslationManager::Translate_Soldier(Victim));
			}
			else {
				Message.Format("%d %s killed %ls (%s VS. %s)", Killer->Get_Player_Type(), KillerTranslation, Get_Wide_Player_Name(Victim), DATranslationManager::Translate(Killer), DATranslationManager::Translate_Soldier(Victim));
			}
		}
		else if (Killer->As_SoldierGameObj()) { //Killed by soldier
			if (((SoldierGameObj*)Killer)->Get_Player()) { //Killed by player
				if (Victim == Killer) { //Suicide
					if (Type == DADamageType::EXPLOSION) {
						Message.Format("%d %ls killed themself (%s)", Victim->Get_Player_Type(), Get_Wide_Player_Name(Victim), DATranslationManager::Translate(GetExplosionObj()));
					}
					else {
						Message.Format("%d %ls killed themself (%s)", Victim->Get_Player_Type(), Get_Wide_Player_Name(Victim), DATranslationManager::Translate_Soldier(Victim));
					}
				}
				else {
					Create_2D_WAV_Sound_Player(Killer, "correction_3.wav");
					if (Type == DADamageType::SQUISH) {
						((SoldierGameObj*)Killer)->Get_Player()->Squishes++;
						Message.Format("%d %ls killed %ls SQUISH! (%s VS. %s)", Killer->Get_Player_Type(), Get_Wide_Player_Name(Killer), Get_Wide_Player_Name(Victim), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
					}
					else if (Type == DADamageType::HEADSHOT || Type == DADamageType::NECKSHOT) {
						Message.Format("%d %ls killed %ls HEADSHOT! (%s VS. %s)", Killer->Get_Player_Type(), Get_Wide_Player_Name(Killer), Get_Wide_Player_Name(Victim), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
					}
					else if (Type == DADamageType::EXPLOSION) {
						Message.Format("%d %ls killed %ls (%s VS. %s)", Killer->Get_Player_Type(), Get_Wide_Player_Name(Killer), Get_Wide_Player_Name(Victim), DATranslationManager::Translate(GetExplosionObj()), DATranslationManager::Translate_Soldier(Victim));
					}
					else {
						Message.Format("%d %ls killed %ls (%s VS. %s)", Killer->Get_Player_Type(), Get_Wide_Player_Name(Killer), Get_Wide_Player_Name(Victim), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
					}
				}
			}
			else { //Killed by bot
				if (Type == DADamageType::HEADSHOT || Type == DADamageType::NECKSHOT) {
					if (Is_Smart_Bot(Killer)) {//Smart bot kill
						Message.Format("%d %ls killed %ls HEADSHOT! (%s VS. %s)", Killer->Get_Player_Type(), ((SoldierGameObj*)Killer)->Get_Bot_Tag(), Get_Wide_Player_Name(Victim), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
					}
					else {
						Message.Format("%d %s killed %ls HEADSHOT! (%s VS. %s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Killer)), Get_Wide_Player_Name(Victim), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
					}
				}
				else if (Type == DADamageType::SQUISH) {
					if (Is_Smart_Bot(Killer)) {//Smart bot kill
						Message.Format("%d %ls killed %ls SQUISH! (%s VS. %s)", Killer->Get_Player_Type(), ((SoldierGameObj*)Killer)->Get_Bot_Tag(), Get_Wide_Player_Name(Victim), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
					}
					else {
						Message.Format("%d %s killed %ls SQUISH! (%s VS. %s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Killer)), Get_Wide_Player_Name(Victim), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
					}
				}
				else if (Type == DADamageType::EXPLOSION) {
					if (Is_Smart_Bot(Killer)) {//Smart bot kill
						Message.Format("%d %ls killed %ls (%s VS. %s)", Killer->Get_Player_Type(), ((SoldierGameObj*)Killer)->Get_Bot_Tag(), Get_Wide_Player_Name(Victim), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
					}
					else {
						Message.Format("%d %s killed %ls (%s VS. %s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Killer)), Get_Wide_Player_Name(Victim), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
					}
				}
				else {
					if (Is_Smart_Bot(Killer)) {//Smart bot kill
						Message.Format("%d %ls killed %ls (%s VS. %s)", Killer->Get_Player_Type(), ((SoldierGameObj*)Killer)->Get_Bot_Tag(), Get_Wide_Player_Name(Victim), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
					}
					else {
						Message.Format("%d %s killed %ls (%s VS. %s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Killer)), Get_Wide_Player_Name(Victim), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
					}
				}
			}
		}
		else { //Killed by something else
			Message.Format("%d %ls was killed (%s)", Victim->Get_Player_Type(), Get_Wide_Player_Name(Victim), DATranslationManager::Translate_Soldier(Victim));
		}

		DALogManager::Write_Log("_PLAYERKILL", "%s", Message);
	}
	else { //Bot
		if (!Killer) { //No killer
			if (Type == DADamageType::FALL) {
				if (Is_Smart_Bot(Victim)) {
					Message.Format("%d %ls had a great fall (%s)", Victim->Get_Player_Type(), ((SoldierGameObj*)Victim)->Get_Bot_Tag(), DATranslationManager::Translate_Soldier(Victim));
				}
				else {
					Message.Format("%d %s had a great fall (%s)", Victim->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Victim)), DATranslationManager::Translate_Soldier(Victim));
				}
			}
			else if (Type == DADamageType::TIBERIUM) {
				if (Is_Smart_Bot(Victim)) {
					Message.Format("%d %ls took a stroll through the Tiberium (%s)", Victim->Get_Player_Type(), ((SoldierGameObj*)Victim)->Get_Bot_Tag(), DATranslationManager::Translate_Soldier(Victim));
				}
				else {
					Message.Format("%d %s took a stroll through the Tiberium (%s)", Victim->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Victim)), DATranslationManager::Translate_Soldier(Victim));
				}
			}
			else {
				if (Is_Smart_Bot(Victim)) {
					Message.Format("%d %ls was killed (%s)", Victim->Get_Player_Type(), ((SoldierGameObj*)Victim)->Get_Bot_Tag(), DATranslationManager::Translate_Soldier(Victim));
				}
				else {
					Message.Format("%d %s was killed (%s)", Victim->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Victim)), DATranslationManager::Translate_Soldier(Victim));
				}
			}
		}
		else if (Killer->As_VehicleGameObj()) { //Killed by vehicle. Could be harvester, defense, or AI vehicle.
			StringClass KillerTranslation;
			if (Killer->Get_Defense_Object()->Get_Shield_Type() == 1 || Is_Harvester_Preset(Killer)) {
				KillerTranslation = "The " + DATranslationManager::Translate_With_Team_Name(Killer);
			}
			else if (((VehicleGameObj*)Killer)->Is_Turret()) {
				KillerTranslation = A_Or_An_Prepend(DATranslationManager::Translate_With_Team_Name(Killer));
			}
			else if (Is_Smart_Bot(Killer)) {
				KillerTranslation = ((VehicleGameObj*)Killer)->Get_Driver()->Get_Bot_Tag();
			}
			else {
				KillerTranslation = A_Or_An_Prepend(DATranslationManager::Translate(Killer));
			}

			if (Type == DADamageType::SQUISH) {
				if (Is_Smart_Bot(Victim)) { //Smart bot kill
					Message.Format("%d %s killed %ls SQUISH! (%s VS. %s)", Killer->Get_Player_Type(), KillerTranslation, ((SoldierGameObj*)Victim)->Get_Bot_Tag(), DATranslationManager::Translate(Killer), DATranslationManager::Translate_Soldier(Victim));
				}
				else {
					Message.Format("%d %s killed %s SQUISH! (%s VS. %s)", Killer->Get_Player_Type(), KillerTranslation, a_or_an_Prepend(DATranslationManager::Translate(Victim)), DATranslationManager::Translate(Killer), DATranslationManager::Translate_Soldier(Victim));
				}
			}
			else if (Type == DADamageType::HEADSHOT || Type == DADamageType::NECKSHOT) {
				if (Is_Smart_Bot(Victim)) { //Smart bot kill
					Message.Format("%d %s killed %ls HEADSHOT! (%s VS. %s)", Killer->Get_Player_Type(), KillerTranslation, ((SoldierGameObj*)Victim)->Get_Bot_Tag(), DATranslationManager::Translate(Killer), DATranslationManager::Translate_Soldier(Victim));
				}
				else {
					Message.Format("%d %s killed %s HEADSHOT! (%s VS. %s)", Killer->Get_Player_Type(), KillerTranslation, a_or_an_Prepend(DATranslationManager::Translate(Victim)), DATranslationManager::Translate(Killer), DATranslationManager::Translate_Soldier(Victim));
				}
			}
			else {
				if (Is_Smart_Bot(Victim)) { //Smart bot kill
					Message.Format("%d %s killed %ls (%s VS. %s)", Killer->Get_Player_Type(), KillerTranslation, ((SoldierGameObj*)Victim)->Get_Bot_Tag(), DATranslationManager::Translate(Killer), DATranslationManager::Translate_Soldier(Victim));
				}
				else {
					Message.Format("%d %s killed %s (%s VS. %s)", Killer->Get_Player_Type(), KillerTranslation, a_or_an_Prepend(DATranslationManager::Translate(Victim)), DATranslationManager::Translate(Killer), DATranslationManager::Translate_Soldier(Victim));
				}
			}
		}
		else if (Killer->As_SoldierGameObj()) { //Killed by soldier
			if (((SoldierGameObj*)Killer)->Get_Player()) { //Killed by player
				if (Type == DADamageType::SQUISH) {
					((SoldierGameObj*)Killer)->Get_Player()->Squishes++;
					if (Is_Smart_Bot(Victim)) { // Smart bot kill
						Message.Format("%d %ls killed %ls SQUISH! (%s VS. %s)", Killer->Get_Player_Type(), Get_Wide_Player_Name(Killer), ((SoldierGameObj*)Victim)->Get_Bot_Tag(), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
					}
					else {
						Message.Format("%d %ls killed %s SQUISH! (%s VS. %s)", Killer->Get_Player_Type(), Get_Wide_Player_Name(Killer), a_or_an_Prepend(DATranslationManager::Translate(Victim)), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
					}
				}
				else if (Type == DADamageType::HEADSHOT || Type == DADamageType::NECKSHOT) {
					if (Is_Smart_Bot(Victim)) { // Smart bot kill
						Message.Format("%d %ls killed %ls HEADSHOT! (%s VS. %s)", Killer->Get_Player_Type(), Get_Wide_Player_Name(Killer), ((SoldierGameObj*)Victim)->Get_Bot_Tag(), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
					}
					else {
						Message.Format("%d %ls killed %s HEADSHOT! (%s VS. %s)", Killer->Get_Player_Type(), Get_Wide_Player_Name(Killer), a_or_an_Prepend(DATranslationManager::Translate(Victim)), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
					}
				}
				else if (Type == DADamageType::EXPLOSION) {
					if (Is_Smart_Bot(Victim)) { // Smart bot kill
						Message.Format("%d %ls killed %ls (%s VS. %s)", Killer->Get_Player_Type(), Get_Wide_Player_Name(Killer), ((SoldierGameObj*)Victim)->Get_Bot_Tag(), DATranslationManager::Translate_Soldier(GetExplosionObj()), DATranslationManager::Translate_Soldier(Victim));
					}
					else {
						Message.Format("%d %ls killed %s (%s VS. %s)", Killer->Get_Player_Type(), Get_Wide_Player_Name(Killer), a_or_an_Prepend(DATranslationManager::Translate(Victim)), DATranslationManager::Translate_Soldier(GetExplosionObj()), DATranslationManager::Translate_Soldier(Victim));
					}
				}
				else {
					if (Is_Smart_Bot(Victim)) { // Smart bot kill
						Message.Format("%d %ls killed %ls (%s VS. %s)", Killer->Get_Player_Type(), Get_Wide_Player_Name(Killer), ((SoldierGameObj*)Victim)->Get_Bot_Tag(), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
					}
					else {
						Message.Format("%d %ls killed %s (%s VS. %s)", Killer->Get_Player_Type(), Get_Wide_Player_Name(Killer), a_or_an_Prepend(DATranslationManager::Translate(Victim)), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
					}
				}
			}
			else if (Victim == Killer) { //Suicide
				if (Is_Smart_Bot(Victim)) { // Smart bot kill
					Message.Format("%d %ls killed itself (%s)", Killer->Get_Player_Type(), ((SoldierGameObj*)Victim)->Get_Bot_Tag(), DATranslationManager::Translate_Soldier(Victim));
				}
				else {
					Message.Format("%d %s killed itself (%s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Victim)), DATranslationManager::Translate_Soldier(Victim));
				}
			}
			else { //Killed by bot
				if (Type == DADamageType::HEADSHOT || Type == DADamageType::NECKSHOT) {
					if (Is_Smart_Bot(Victim)) { // Smart bot kill
						if (Is_Smart_Bot(Killer)) { // Smart bot kill
							Message.Format("%d %ls killed %ls HEADSHOT! (%s VS. %s)", Killer->Get_Player_Type(), ((SoldierGameObj*)Killer)->Get_Bot_Tag(), ((SoldierGameObj*)Victim)->Get_Bot_Tag(), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
						}
						else {
							Message.Format("%d %s killed %ls HEADSHOT! (%s VS. %s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Killer)), ((SoldierGameObj*)Victim)->Get_Bot_Tag(), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
						}
					}
					else {
						if (Is_Smart_Bot(Killer)) { // Smart bot kill
							Message.Format("%d %ls killed %s HEADSHOT! (%s VS. %s)", Killer->Get_Player_Type(), ((SoldierGameObj*)Killer)->Get_Bot_Tag(), a_or_an_Prepend(DATranslationManager::Translate(Victim)), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
						}
						else {
							Message.Format("%d %s killed %s HEADSHOT! (%s VS. %s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Killer)), a_or_an_Prepend(DATranslationManager::Translate(Victim)), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
						}
					}
				}
				else if (Type == DADamageType::EXPLOSION) {
					if (Is_Smart_Bot(Victim)) { // Smart bot kill
						if (Is_Smart_Bot(Killer)) { // Smart bot kill
							Message.Format("%d %ls killed %ls (%s VS. %s)", Killer->Get_Player_Type(), ((SoldierGameObj*)Killer)->Get_Bot_Tag(), ((SoldierGameObj*)Victim)->Get_Bot_Tag(), DATranslationManager::Translate(GetExplosionObj()), DATranslationManager::Translate_Soldier(Victim));
						}
						else {
							Message.Format("%d %s killed %ls (%s VS. %s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Killer)), ((SoldierGameObj*)Victim)->Get_Bot_Tag(), DATranslationManager::Translate(GetExplosionObj()), DATranslationManager::Translate_Soldier(Victim));
						}
					}
					else {
						if (Is_Smart_Bot(Killer)) { // Smart bot kill
							Message.Format("%d %ls killed %s (%s VS. %s)", Killer->Get_Player_Type(), ((SoldierGameObj*)Killer)->Get_Bot_Tag(), a_or_an_Prepend(DATranslationManager::Translate(Victim)), DATranslationManager::Translate(GetExplosionObj()), DATranslationManager::Translate_Soldier(Victim));
						}
						else {
							Message.Format("%d %s killed %s (%s VS. %s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Killer)), a_or_an_Prepend(DATranslationManager::Translate(Victim)), DATranslationManager::Translate(GetExplosionObj()), DATranslationManager::Translate_Soldier(Victim));
						}
					}
				}
				else {
					if (Is_Smart_Bot(Victim)) { // Smart bot kill
						if (Is_Smart_Bot(Killer)) { // Smart bot kill
							Message.Format("%d %ls killed %ls (%s VS. %s)", Killer->Get_Player_Type(), ((SoldierGameObj*)Killer)->Get_Bot_Tag(), ((SoldierGameObj*)Victim)->Get_Bot_Tag(), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
						}
						else {
							Message.Format("%d %s killed %ls (%s VS. %s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Killer)), ((SoldierGameObj*)Victim)->Get_Bot_Tag(), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
						}
					}
					else {
						if (Is_Smart_Bot(Killer)) { // Smart bot kill
							Message.Format("%d %ls killed %s (%s VS. %s)", Killer->Get_Player_Type(), ((SoldierGameObj*)Killer)->Get_Bot_Tag(), a_or_an_Prepend(DATranslationManager::Translate(Victim)), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
						}
						else {
							Message.Format("%d %s killed %s (%s VS. %s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Killer)), a_or_an_Prepend(DATranslationManager::Translate(Victim)), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate_Soldier(Victim));
						}
					}
				}
			}
		}
		
		DALogManager::Write_Log("_BOTKILL", "%s", Message);
	}
}

//Default character purchase handler.
PurchaseStatus DASoldierManager::Character_Purchase_Request_Event(BaseControllerClass* Base, cPlayer* Player, float& Cost, const SoldierGameObjDef* Item) {
	if (Base->Can_Generate_Soldiers() || !Cost) {
		if (Player->Purchase_Item((int)Cost)) {
			Player->Get_GameObj()->Re_Init(*Item);
			Player->Get_GameObj()->Post_Re_Init();
			return PurchaseStatus_Granted;
		}
		return PurchaseStatus_InsufficientFunds;
	}
	return PurchaseStatus_FactoryUnavailable;
}

HashTemplateClass<unsigned int, DynamicVectorClass<const WeaponDefinitionClass*>>& DASoldierManager::Get_Exclusive_Weapons() {
	return ExclusiveWeapons;
}

HashTemplateClass<unsigned int, const WeaponDefinitionClass*>& DASoldierManager::Get_Replace_Weapons() {
	return ReplaceWeapons;
}

HashTemplateClass<unsigned int, DynamicVectorClass<const WeaponDefinitionClass*>>& DASoldierManager::Get_Remove_Weapons() {
	return RemoveWeapons;
}