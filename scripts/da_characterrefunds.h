/*	Renegade Scripts.dll
    Dragonade Character Refunds Game Feature
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DACHARACTERREFUNDS
#define INCLUDE_DACHARACTERREFUNDS

#include "da_event.h"
#include "da_gamefeature.h"
#include "da_player.h"

class DACharacterRefundsPlayerDataClass : public DAPlayerDataClass {
public:
	virtual void Init() {
		Cost = 0;
		Def = 0;
	}
	float Cost;
	const SoldierGameObjDef *Def;
};

class DACharacterRefundsGameFeatureClass : public DAEventClass, public DAGameFeatureClass, public DAPlayerDataManagerClass<DACharacterRefundsPlayerDataClass> {
	virtual void Init();
	virtual void Settings_Loaded_Event();
	virtual void Character_Purchase_Event(cPlayer *Player,float Cost,const SoldierGameObjDef *Item);
	virtual void Object_Destroyed_Event(GameObject *obj);
	float Percent;
};

#endif
	