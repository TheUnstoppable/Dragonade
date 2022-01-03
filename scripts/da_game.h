/*	Renegade Scripts.dll
    Dragonade Game Manager
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAGAME
#define INCLUDE_DAGAME

#include "da_event.h"
#include "da_gamemode.h"
#include "da_gamefeature.h"

class DAGameManager : public DAEventClass {
public:
	static void Init();
	static void Shutdown();
	DA_API static DAGameModeFactoryClass *Get_Game_Mode();
	DA_API static const char *Get_Game_Mode_Long_Name();
	DA_API static const char *Get_Game_Mode_Short_Name();
	DA_API static bool Is_Game_Mode(const char *Name);
	DA_API static DAGameModeFactoryClass *Find_Game_Mode(const char *Name);
	DA_API static DAGameFeatureFactoryClass *Find_Game_Feature(const char *Name);
	DA_API static void Add_Game_Mode(DAGameModeFactoryClass *Factory);
	DA_API static void Add_Game_Feature(DAGameFeatureFactoryClass* Factory);
	DA_API static const DynamicVectorClass<DAGameFeatureFactoryClass*> &Get_Game_Features();
	DA_API static bool Is_Shutdown_Pending();
	DA_API static const StringClass &Get_Map();

private:
	virtual void Game_Over_Event();
	virtual void Level_Loaded_Event();
	virtual void Settings_Loaded_Event();
	virtual void Player_Loaded_Event(cPlayer *Player);

	static DAGameModeFactoryClass *GameMode;
	static DynamicVectorClass<DAGameModeFactoryClass*> GameModes;
	static bool ShowGameModeTitle;
	static DynamicVectorClass<DAGameFeatureFactoryClass*> GameFeatures;
	static bool FirstMap;
	static bool ShutdownPending;
	static StringClass Map;
};


#endif