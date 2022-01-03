/*	Renegade Scripts.dll
    Dragonade Translation Manager
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
#include "engine.h"
#include "engine_da.h"
#include "da.h"
#include "da_translation.h"
#include "da_settings.h"
#include "da_string.h"
#include "DefinitionMgrClass.h"
#include "WeaponClass.h"

HashTemplateClass<unsigned int,StringClass> DATranslationManager::Translations;

void DATranslationManager::Init() {
	static DATranslationManager Instance;
	Instance.Register_Event(DAEvent::SETTINGSLOADED,INT_MAX);
}

void DATranslationManager::Settings_Loaded_Event() {
	Translations.Remove_All();
	INISection *Section = DASettingsManager::Get_Section("Translations");
	if (Section) {
		for (INIEntry *i = Section->EntryList.First();i && i->Is_Valid();i = i->Next()) {
			const DefinitionClass *Def = Find_Named_Definition(i->Entry);
			if (Def) {
				Translations.Insert((unsigned int)Def,i->Value);
			}
		}
	}
	for (PowerUpGameObjDef *PowerUpDef = (PowerUpGameObjDef*)DefinitionMgrClass::Get_First(CID_PowerUp);PowerUpDef;PowerUpDef = (PowerUpGameObjDef*)DefinitionMgrClass::Get_Next(PowerUpDef,CID_PowerUp)) {
		if (PowerUpDef->GrantWeapon) {
			WeaponDefinitionClass *WeapDef = (WeaponDefinitionClass*)Find_Definition(PowerUpDef->GrantWeaponID);
			if (WeapDef) {
				PowerUpDef->Set_Translated_Name_ID(WeapDef->IconNameID); //Map the translated name of the weapon to the powerup that grants it. This way we don't have to manually define the translation for each powerup.
			}
		}
	}
}

const char *DATranslationManager::Translate(GameObject *obj) {
	if (!obj || !obj->As_DamageableGameObj()) {
		return "None";
	}
	if (Is_C4(obj)) {
		int Mode = Get_C4_Mode(obj);
		if (Mode == 1) {
			return "Remote C4";
		}
		else if (Mode == 2) {
			return "Timed C4";
		}
		else if (Mode == 3) {
			return "Proximity C4";
		}
	}
	const DamageableGameObjDef *Def = (DamageableGameObjDef*)&obj->Get_Definition();
	if (Translations.Get((unsigned int)Def)) {
		return Translations.Get((unsigned int)Def)->Peek_Buffer();
	}
	const char *Trans = TranslateDBClass::Get_English_String(Def->Get_Translated_Name_ID());
	if (Trans) {
		return Trans;
	}
	return Def->Get_Name();
}

const char *DATranslationManager::Translate(WeaponClass *Weap) {
	if (!Weap) {
		return "No weapon";
	}
	const WeaponDefinitionClass *Def = Weap->Get_Definition();
	if (Translations.Get((unsigned int)Def)) {
		return Translations.Get((unsigned int)Def)->Peek_Buffer();
	}
	const char *Trans = TranslateDBClass::Get_English_String(Def->IconNameID);
	if (Trans) {
		return Trans;
	}
	return Def->Get_Name();
}

const char *DATranslationManager::Translate(const DefinitionClass *Def) {
	if (Def->Get_Class_ID() == CID_Weapon) {
		if (Translations.Get((unsigned int)Def)) {
			return Translations.Get((unsigned int)Def)->Peek_Buffer();
		}
		const char *Trans = TranslateDBClass::Get_English_String(((WeaponDefinitionClass*)Def)->IconNameID);
		if (Trans) {
			return Trans;
		}
	}
	else {
		if (Translations.Get((unsigned int)Def)) {
			return Translations.Get((unsigned int)Def)->Peek_Buffer();
		}
		const char *Trans = TranslateDBClass::Get_English_String(((DamageableGameObjDef*)Def)->Get_Translated_Name_ID());
		if (Trans) {
			return Trans;
		}
	}
	return Def->Get_Name();
}
const char *DATranslationManager::Translate(const char *Preset) {
	const DefinitionClass *Def = Find_Named_Definition(Preset);
	if (!Def) {
		return "None";
	}
	if (Def->Get_Class_ID() == CID_Weapon) {
		if (Translations.Get((unsigned int)Def)) {
			return Translations.Get((unsigned int)Def)->Peek_Buffer();
		}
		const char *Trans = TranslateDBClass::Get_English_String(((WeaponDefinitionClass*)Def)->IconNameID);
		if (Trans) {
			return Trans;
		}
	}
	else {
		if (Translations.Get((unsigned int)Def)) {
			return Translations.Get((unsigned int)Def)->Peek_Buffer();
		}
		const char *Trans = TranslateDBClass::Get_English_String(((DamageableGameObjDef*)Def)->Get_Translated_Name_ID());
		if (Trans) {
			return Trans;
		}
	}
	return Def->Get_Name();
}

const char *DATranslationManager::Translate(unsigned int DefID) {
	const DefinitionClass *Def = Find_Definition(DefID);
	if (!Def) {
		return "None";
	}
	if (Def->Get_Class_ID() == CID_Weapon) {
		if (Translations.Get((unsigned int)Def)) {
			return Translations.Get((unsigned int)Def)->Peek_Buffer();
		}
		const char *Trans = TranslateDBClass::Get_English_String(((WeaponDefinitionClass*)Def)->IconNameID);
		if (Trans) {
			return Trans;
		}
	}
	else {
		if (Translations.Get((unsigned int)Def)) {
			return Translations.Get((unsigned int)Def)->Peek_Buffer();
		}
		const char *Trans = TranslateDBClass::Get_English_String(((DamageableGameObjDef*)Def)->Get_Translated_Name_ID());
		if (Trans) {
			return Trans;
		}
	}
	return Def->Get_Name();
}


StringClass DATranslationManager::Translate_With_Team_Name(GameObject *obj) {
	const char *Translation = Translate(obj);
	StringClass Team = Get_Wide_Team_Name(Get_Object_Type(obj));
	if (stristr(Translation,Team)) {
		return Translation;
	}
	return StringFormat("%s %s",Team,Translation);
}

const char *DATranslationManager::Translate_Weapon(GameObject *obj) {
	if (!obj || !obj->As_PhysicalGameObj() || !obj->As_PhysicalGameObj()->As_ArmedGameObj()) {
		return "None";
	}
	return Translate(((ArmedGameObj*)obj)->Get_Weapon());
}

StringClass DATranslationManager::Translate_Soldier(GameObject *obj) {
	if (!obj) {
		return "None";
	}
	if (obj->As_SoldierGameObj()) {
		if (((SoldierGameObj*)obj)->Get_Vehicle()) {
			return Translate(((SoldierGameObj*)obj)->Get_Vehicle());
		}
		else {
			return StringFormat("%s/%s",Translate(obj),Translate(((SoldierGameObj*)obj)->Get_Weapon()));
		}
	}
	return Translate(obj);
}
