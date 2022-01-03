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

#ifndef INCLUDE_DATRANSLATION
#define INCLUDE_DATRANSLATION

#include "da_event.h"
#include "HashTemplateClass.h"

class DATranslationManager : public DAEventClass {
public:
	static void Init();
	DA_API static const char *Translate(GameObject *obj); //Returns the translated name of a GameObject, Weapon, or Preset.
	DA_API static const char *Translate(WeaponClass *Weap); //da.ini translation > strings.tdb translation > preset name
	DA_API static const char *Translate(const DefinitionClass *Def);
	DA_API static const char *Translate(const char *Preset);
	DA_API static const char *Translate(unsigned int DefID);
	DA_API static StringClass Translate_With_Team_Name(GameObject *obj); //Returns the translated name and team of a GameObject.
	DA_API static const char *Translate_Weapon(GameObject *obj); //Returns the translated name of the current weapon of a GameObject.
	DA_API static StringClass Translate_Soldier(GameObject *obj); //Returns the translated name and translated weapon, or the translated vehicle, of a SoldierGameObj.

private:
	virtual void Settings_Loaded_Event();
	
	static HashTemplateClass<unsigned int,StringClass> Translations; //DefinitionClass*,Translation
};

#endif