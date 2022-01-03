/*	Renegade Scripts.dll
    Dragonade Team Manager
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DATEAM
#define INCLUDE_DATEAM

#include "da_event.h"

class DATeamManager : public DAEventClass {
public:
	static void Init();
	DA_API static void Set_Force_Team(int Team);
	DA_API static int Get_Force_Team();
	DA_API static bool Is_Free_Team_Changing_Enabled();
	DA_API static void Remix();
	DA_API static void Rebalance();
	DA_API static void Swap();

private:
	virtual void Settings_Loaded_Event();
	virtual void Level_Loaded_Event();
	virtual void Game_Over_Event();
	virtual void Player_Join_Event(cPlayer *Player);
	virtual void Player_Leave_Event(cPlayer *Player);
	virtual bool Team_Change_Request_Event(cPlayer *Player);
	virtual void Timer_Expired(int Number,unsigned int Data);
	static int Winner;

	//Settings
	static bool EnableFreeTeamChanging;
	static int ForceTeam;
	unsigned int RebalanceTime;
	int RemixFrequency;
	int SwapChance;
	int RemixCount;
	bool EnableRebalance;
	int RebalanceScoreLimit;
};


#endif