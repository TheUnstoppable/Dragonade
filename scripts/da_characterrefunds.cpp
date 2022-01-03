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

#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "engine_DA.h"
#include "da.h"
#include "da_characterrefunds.h"
#include "da_settings.h"

void DACharacterRefundsGameFeatureClass::Init() {
	DAPlayerDataManagerClass<DACharacterRefundsPlayerDataClass>::Init();
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Event(DAEvent::CHARACTERPURCHASE);
	Register_Object_Event(DAObjectEvent::DESTROYED,DAObjectEvent::PLAYER);
}

void DACharacterRefundsGameFeatureClass::Settings_Loaded_Event() {
	Percent = WWMath::Clamp(DASettingsManager::Get_Float("CharacterRefunds",0.0f),0.0f,1.0f);
}

void DACharacterRefundsGameFeatureClass::Character_Purchase_Event(cPlayer *Player,float Cost,const SoldierGameObjDef *Item) {
	DACharacterRefundsPlayerDataClass *Data = Get_Player_Data(Player);
	if (Data->Cost && &Player->Get_GameObj()->Get_Definition() == Data->Def) {
		Player->Increment_Money(Data->Cost*Percent);
	}
	Data->Cost = Cost;
	Data->Def = Item;
}

void DACharacterRefundsGameFeatureClass::Object_Destroyed_Event(GameObject *obj) {
	DAPlayerClass *Player = ((SoldierGameObj*)obj)->Get_DA_Player();
	if (!Player->Is_Spawning()) {
		Get_Player_Data(Player)->Cost = 0.0f;
	}
}

Register_Game_Feature(DACharacterRefundsGameFeatureClass,"Character Refunds","CharacterRefunds",0);