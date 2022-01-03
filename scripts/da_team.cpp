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

#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "engine_DA.h"
#include "da.h"
#include "da_team.h"
#include "da_settings.h"
#include "da_player.h"

int DATeamManager::Winner = -1;
bool DATeamManager::EnableFreeTeamChanging = false;
int DATeamManager::ForceTeam = -1;

#define Count_Team() for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) { cPlayer *Player = z->Data(); if (Player->Is_Active() && !Player->Get_DA_Player()->Get_Needs_Team()) { if (Player->Get_Player_Type() == 0) { TeamCount[0]++; } else if (Player->Get_Player_Type() == 1) { TeamCount[1]++; } } }

void DATeamManager::Init() {
	static DATeamManager Instance;
	Instance.Register_Event(DAEvent::SETTINGSLOADED,INT_MAX);
	Instance.Register_Event(DAEvent::LEVELLOADED,INT_MAX);
	Instance.Register_Event(DAEvent::GAMEOVER,INT_MAX);
	Instance.Register_Event(DAEvent::PLAYERJOIN,INT_MAX);
	Instance.Register_Event(DAEvent::TEAMCHANGEREQUEST);
}

void DATeamManager::Settings_Loaded_Event() {
	RebalanceTime = (unsigned int)DASettingsManager::Get_Int("RebalanceTime",10);
	RebalanceScoreLimit = DASettingsManager::Get_Int("RebalanceScoreLimit",100);
	RemixFrequency = DASettingsManager::Get_Int("RemixFrequency",1);
	SwapChance = DASettingsManager::Get_Int("SwapChance",50);
	EnableFreeTeamChanging = DASettingsManager::Get_Bool("EnableFreeTeamChanging",false);
	EnableRebalance = DASettingsManager::Get_Bool("EnableRebalance",true);
	if (EnableRebalance) {
		Register_Event(DAEvent::PLAYERLEAVE,INT_MAX);
	}
	else {
		Unregister_Event(DAEvent::PLAYERLEAVE);
	}
}

void DATeamManager::Level_Loaded_Event() {
	if (ForceTeam != -1) {
		Set_Force_Team(ForceTeam);
	}
	else if (RemixFrequency) {
		RemixCount++;
		if (RemixCount >= RemixFrequency) {
			Remix();
			RemixCount = 0;
		}
		else if (Get_Random_Int(1,101) <= SwapChance) {
			Swap();
		}
		Rebalance();
	}
	else if (Get_Random_Int(1,101) <= SwapChance) {
		Swap();
	}
	if (EnableRebalance) {
		Rebalance();
	}
	Winner = -1;
}

void DATeamManager::Game_Over_Event() {
	Winner = The_Game()->Get_Winner_ID();
}

void DATeamManager::Player_Join_Event(cPlayer *Player) {
	if (ForceTeam != -1) {
		Player->Set_Player_Type(ForceTeam);
	}
	else if (EnableRebalance && !EnableFreeTeamChanging && ((RebalanceScoreLimit >= 0 && Player->Get_Score() <= RebalanceScoreLimit && Player->Get_Score() >= (RebalanceScoreLimit*-1)) || The_Game()->Get_Game_Duration_S() <= RebalanceTime)) { //Check if this player imbalanced the teams by joining. Can be caused by a player using the team select screen on XWIS or rejoining after other players have joined and changed the team sizes.
		int TeamCount[2] = {0,0};
		Count_Team();
		if ((TeamCount[0]-TeamCount[1]) > 1) {
			if (Player->Get_Player_Type() == 0) {
				Change_Team_3(Player,1);
			}
		}
		else if ((TeamCount[1]-TeamCount[0]) > 1) {
			if (Player->Get_Player_Type() == 1) {
				Change_Team_3(Player,0);
			}
		}
	}
}

void DATeamManager::Player_Leave_Event(cPlayer *Player) {
	if (The_Game()->Get_Game_Duration_S() <= RebalanceTime) {
		int Team = Player->Get_Player_Type();
		if (Team == 0 || Team == 1) {
			int OtherTeam = Team?0:1;
			int TeamCount[2] = {0,0};
			Count_Team();
			TeamCount[Team]--;
			if (TeamCount[OtherTeam]-TeamCount[Team] > 1) { //Check if this player imbalanced the teams by leaving.
				if (!Is_Timer(1)) {
					Start_Timer(1,10.0f); //Give it 10 seconds to see if another player joins or leaves and fixes the imbalance.
				}
			}
		}
	}
}

bool DATeamManager::Team_Change_Request_Event(cPlayer *Player) {
	if (Get_Force_Team() != -1) {
		DA::Page_Player(Player,"You can not request a team change while force teaming is in effect.");
		return false;
	}
	else if (Is_Free_Team_Changing_Enabled()) {
		Change_Team_3(Player,Player->Get_Player_Type()?0:1);
		DA::Host_Message("%ls changed teams.",Player->Get_Name());
		return false;
	}
	return true;
}

void DATeamManager::Timer_Expired(int Number,unsigned int Data) {
	Rebalance();
}

void DATeamManager::Set_Force_Team(int Team) {
	if (Team == -1) {
		ForceTeam = -1;
		Remix();
	}
	else {
		ForceTeam = Team;
		for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
			Change_Team_4(z->Data(),Team);
		}
	}
}

int DATeamManager::Get_Force_Team() {
	return ForceTeam;
}

bool DATeamManager::Is_Free_Team_Changing_Enabled() {
	return EnableFreeTeamChanging;
}

void DATeamManager::Remix() {
	if (ForceTeam != -1) {
		return;
	}

	DA::Host_Message("Teams have been remixed.");
	
	DynamicVectorClass<cPlayer*> Winners;
	DynamicVectorClass<cPlayer*> Losers;

	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) { //First pass
		cPlayer *Player = z->Data();
		if (Player->Is_Active() && (Player->Get_Player_Type() == 0 || Player->Get_Player_Type() == 1)) { 
			if (Get_Random_Bool()) {
				if (Player->Get_Player_Type() == Winner) {
					Winners.Add(Player);
				}
				else {
					Losers.Add(Player);
				}
				Player->Get_DA_Player()->Set_Needs_Team(true);
			}
			else {
				Player->Get_DA_Player()->Set_Needs_Team(false);
			}
		}
	}
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) { //Second pass
		cPlayer *Player = z->Data();
		if (Player->Is_Active() && (Player->Get_Player_Type() == 0 || Player->Get_Player_Type() == 1) && !Player->Get_DA_Player()->Get_Needs_Team()) {
			if (Player->Get_Player_Type() == Winner) {
				Winners.Add(Player);
			}
			else {
				Losers.Add(Player);
			}
			Player->Get_DA_Player()->Set_Needs_Team(true);
		}
	}

	DAEventManager::Remix_Event(); //Trigger events.

	int TeamCount[2] = {0,0}; //Count team sizes.
	Count_Team();

	for (int i = 0;i < Winners.Count();i++) { //Team winners first so that each team will have half the winners...
		cPlayer *Player = Winners[i];
		if (Player->Is_Active() && Player->Get_DA_Player()->Get_Needs_Team()) {
			int RandTeam = Get_Random_Bool();
			if (TeamCount[RandTeam] <= TeamCount[RandTeam?0:1]) { //If the randomly chosen team has the same or less players we use it.
				Change_Team_4(Player,RandTeam);
				TeamCount[RandTeam]++;
				Player->Get_DA_Player()->Set_Needs_Team(false);
			}
			else { //Otherwise we use the other team to maintain balance.
				Change_Team_4(Player,RandTeam?0:1);
				TeamCount[RandTeam?0:1]++;
				Player->Get_DA_Player()->Set_Needs_Team(false);
			}
		}
	}
	for (int i = 0;i < Losers.Count();i++) { //And half the losers.
		cPlayer *Player = Losers[i];
		if (Player->Is_Active() && Player->Get_DA_Player()->Get_Needs_Team()) { //Only remix player if another event hasn't already handled them.
			int RandTeam = Get_Random_Bool();
			if (TeamCount[RandTeam] <= TeamCount[RandTeam?0:1]) {
				Change_Team_4(Player,RandTeam);
				TeamCount[RandTeam]++;
				Player->Get_DA_Player()->Set_Needs_Team(false);
			}
			else {
				Change_Team_4(Player,RandTeam?0:1);
				TeamCount[RandTeam?0:1]++;
				Player->Get_DA_Player()->Set_Needs_Team(false);
			}
		}
	}
}

void DATeamManager::Rebalance() {
	if (ForceTeam != -1) {
		return;
	}

	int TeamCount[2] = {0,0};
	Count_Team(); //Count team sizes.

	if (Diff(TeamCount[0],TeamCount[1]) > 1) { //Only trigger if the teams are actually imbalanced.
		DA::Host_Message("Teams have been rebalanced.");

		DAEventManager::Rebalance_Event(); //Trigger events.

		TeamCount[0] = 0;
		TeamCount[1] = 0;
		Count_Team(); //Re-count in case another event rebalanced already.

		if (Diff(TeamCount[0],TeamCount[1]) > 1) { //Still imbalanced.

			int OldTeam = -1;
			int NewTeam = -1;
			if (TeamCount[0] > TeamCount[1]) {
				OldTeam = 0;
				NewTeam = 1;
			}
			else {
				OldTeam = 1;
				NewTeam = 0;
			}
			while (TeamCount[OldTeam]-TeamCount[NewTeam] > 1) { //Go until teams are balanced.
				int Rand = Get_Random_Int(0,TeamCount[OldTeam]); //Select random player on team to change.
				int Count = 0;
				for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
					cPlayer *Player = z->Data();
					if (Player->Is_Active() && Player->Get_Player_Type() == OldTeam) {
						if (Count == Rand) { //Loop until we find that player.
							Change_Team_3(Player,NewTeam);
							TeamCount[OldTeam]--;
							TeamCount[NewTeam]++;
							break;
						}
						Count++;
					}
				}
			}
		}
	}
}

void DATeamManager::Swap() {
	if (ForceTeam != -1) {
		return;
	}

	DA::Host_Message("Teams have been swapped.");
	
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		cPlayer *Player = z->Data();
		if (Player->Is_Active() && (Player->Get_Player_Type() == 0 || Player->Get_Player_Type() == 1)) {
			Player->Get_DA_Player()->Set_Needs_Team(true);
		}
	}

	DAEventManager::Swap_Event(); //Trigger events.

	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		cPlayer *Player = z->Data();
		if (Player->Is_Active() && (Player->Get_Player_Type() == 0 || Player->Get_Player_Type() == 1) && Player->Get_DA_Player()->Get_Needs_Team()) { //Only swap if some other event hasn't handled this player already.
			Change_Team_4(Player,Player->Get_Player_Type()?0:1);
			Player->Get_DA_Player()->Set_Needs_Team(false);
		}
	}
}



class DARemixConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "remix"; }
	const char *Get_Help() { return "REMIX - Remix teams."; }
	void Activate(const char *ArgumentsString) {
		DATeamManager::Remix();
		DATeamManager::Rebalance();
	}
};
Register_Console_Function(DARemixConsoleFunctionClass);

class DARebalanceConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "rebalance"; }
	const char *Get_Help() { return "REBALANCE - Rebalance teams."; }
	void Activate(const char *ArgumentsString) {
		DATeamManager::Rebalance();
	}
};
Register_Console_Function(DARebalanceConsoleFunctionClass);

class DASwapConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "swap"; }
	const char *Get_Help() { return "SWAP - Swap teams."; }
	void Activate(const char *ArgumentsString) {
		DATeamManager::Swap();
	}
};
Register_Console_Function(DASwapConsoleFunctionClass);

class DAForceTeamConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "forceteam"; }
	const char *Get_Help() { return "FORCETEAM <team> - Force all players to switch to the given team."; }
	void Activate(const char *ArgumentsString) {
		DATeamManager::Set_Force_Team(atoi(ArgumentsString));
	}
};
Register_Console_Function(DAForceTeamConsoleFunctionClass);

class DATeam3ConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "team3"; }
	const char *Get_Help() { return "TEAM3 <playerid> - Swap a player's team, allowing them to keep their score, kills, deaths, and starting credits. Any credits over the starting amount are distributed amongst their former team."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Change_Team_3(Player,Player->Get_Player_Type()?0:1);
		}
	}
};
Register_Console_Function(DATeam3ConsoleFunctionClass);

class DATeam4ConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "team4"; }
	const char *Get_Help() { return "TEAM4 <playerid> - Swap a player's team, allowing them to keep their score, kills, deaths, and reseting their credits to the starting amount."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Change_Team_4(Player,Player->Get_Player_Type()?0:1);
		}
	}
};
Register_Console_Function(DATeam4ConsoleFunctionClass);

class DATeam5ConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "team5"; }
	const char *Get_Help() { return "TEAM5 <playerid> - Swap a player's team, reseting their score, kills and deaths to 0, and their credits to the starting amount."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Change_Team_5(Player,Player->Get_Player_Type()?0:1);
		}
	}
};
Register_Console_Function(DATeam5ConsoleFunctionClass);