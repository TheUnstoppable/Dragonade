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

#ifndef INCLUDE_DAINFAMMO
#define INCLUDE_DAINFAMMO

#include "scripts.h"
#include "da_event.h"
#include "da_gamefeature.h"
#include "da_player.h"

class DAInfiniteAmmoGameFeatureClass : public DAEventClass, public DAGameFeatureClass {
	virtual void Init();
	virtual void Level_Loaded_Event();
	virtual void Add_Weapon_Event(cPlayer *Player,WeaponClass *Weapon);
};

class DANoReloadPlayerObserverClass : public DAPlayerObserverClass {
	virtual void Created();
	virtual void Destroyed();
	virtual void Timer_Expired(int Number,unsigned int Data);
	virtual const char *Get_Observer_Name() {
		return "DANoReloadPlayerObserverClass";
	}
};

class DANoReloadGameFeatureClass : public DAEventClass, public DAGameFeatureClass {
	virtual void Init();
	virtual void Level_Loaded_Event();
	virtual void Player_Loaded_Event(cPlayer *Player);
};

#endif