/*	Renegade Scripts.dll
    RenCorner Sounds Game Feature
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAChatSounds
#define INCLUDE_DAChatSounds

#include "da_event.h"
#include "da_gamefeature.h"
#include "da_player.h"
#include "HashTemplateClass.h"
#include "HashTemplateIterator.h"

class DAChatSoundsPlayerDataClass : public DAPlayerDataClass {
public:
	virtual void Init() {
		Enable = true;
		Mute = false;
	}
	bool Enable;
	bool Mute;
};

class DAChatSoundsGameFeatureClass : public DAEventClass, public DAGameFeatureClass, public DAPlayerDataManagerClass<DAChatSoundsPlayerDataClass> {
	virtual void Init();
	virtual ~DAChatSoundsGameFeatureClass();
	virtual void Settings_Loaded_Event();
	bool EnableSounds_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	bool DisableSounds_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	bool Sounds_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	virtual bool Chat_Event(cPlayer *Player,TextMessageEnum Type,const wchar_t *Message,int ReceiverID);

	HashTemplateClass<StringClass,StringClass> Sounds;
	unsigned long LastUsed;
	unsigned int Delay;
};

#endif