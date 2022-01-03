/*	Renegade Scripts.dll
    Dragonade Domination Game Mode
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DASHOTGUN
#define INCLUDE_DASHOTGUN

#include "da_event.h"
#include "da_gamemode.h"

class DAShotgunGameModeClass : public DAEventClass, public DAGameModeClass {
	virtual void Init();
	virtual bool Add_Weapon_Request_Event(cPlayer *Player,const WeaponDefinitionClass *Weapon);
	virtual void Object_Created_Event(GameObject *obj);
};

#endif