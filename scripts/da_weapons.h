/*	Renegade Scripts.dll
	Copyright 2017 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAWEAPONS
#define INCLUDE_DAWEAPONS

#include "da_event.h"
#include "da_gamefeature.h"
#include "HashTemplateClass.h"
#include "HashTemplateIterator.h"

class DAWeaponsGameFeatureClass : public DAEventClass, public DAGameFeatureClass {
	virtual void Init();
	virtual void Settings_Loaded_Event();
	bool Weapons_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	virtual bool Chat_Command_Event(cPlayer *Player,TextMessageEnum Type,const StringClass &Command,const DATokenClass &Text,int ReceiverID);

	struct WeaponStruct {
		bool operator==(const WeaponStruct &That) { return false; }
		bool operator!=(const WeaponStruct &That) { return false; }
		DynamicVectorClass<StringClass> Triggers;
		WeaponDefinitionClass *Weapon[2];
		int Cost;
	};
	
	DynamicVectorClass<WeaponStruct> Weapons;
};

#endif