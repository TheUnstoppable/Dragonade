/*	Renegade Scripts.dll
    Dragonade Taunts Game Feature
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DATAUNT
#define INCLUDE_DATAUNT

#include "da_event.h"
#include "da_gamefeature.h"


class DATauntsGameFeatureClass : public DAEventClass, public DAGameFeatureClass {
	virtual void Init();
	bool Taunt_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	void Taunt1_Key_Hook(cPlayer *Player);
	void Taunt2_Key_Hook(cPlayer *Player);
	void Taunt3_Key_Hook(cPlayer *Player);
	void Taunt4_Key_Hook(cPlayer *Player);
	void Taunt5_Key_Hook(cPlayer *Player);
	void Taunt6_Key_Hook(cPlayer *Player);
	void Taunt7_Key_Hook(cPlayer *Player);
	void Taunt8_Key_Hook(cPlayer *Player);
	void Taunt9_Key_Hook(cPlayer *Player);
	void Taunt0_Key_Hook(cPlayer *Player);
	void TauntS_Key_Hook(cPlayer *Player);
};

#endif