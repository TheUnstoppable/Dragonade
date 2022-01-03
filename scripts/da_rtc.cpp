/*	Renegade Scripts.dll
    Dragonade Request Team Change Game Feature
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
#include "da_rtc.h"
#include "da_settings.h"
#include "da_team.h"

void DARTCGameFeatureClass::Init() {
	DAPlayerDataManagerClass<DARTCPlayerDataClass>::Init();
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Event(DAEvent::CHATCOMMAND);
	Register_Event(DAEvent::PLAYERLEAVE);
	Register_Event(DAEvent::TEAMCHANGEREQUEST);
	Register_Event(DAEvent::TEAMCHANGE);
	Register_Event(DAEvent::THINK);
	Register_Chat_Command((DAECC)&DARTCGameFeatureClass::RTC_Chat_Command,"!rtc|!tc|!swap|!switch|!change|!teamchange|!changeteam|!requestteamchange");
}

void DARTCGameFeatureClass::Settings_Loaded_Event() {
	StringClass Temp;
	DASettingsManager::Get_String(Temp,"RTCDisablingCommands",0);
	DisablingCommands.Build(Temp,0,'|');
	ScoreLimit = DASettingsManager::Get_Int("RTCScoreLimit",100);
}

bool DARTCGameFeatureClass::Chat_Command_Event(cPlayer *Player,TextMessageEnum Type,const StringClass &Command,const DATokenClass &Text,int ReceiverID) {
	if (DisablingCommands.Find(Command) != -1) {
		Set_Can_RTC(Player,false);
	}
	return true;
}

void DARTCGameFeatureClass::Player_Leave_Event(cPlayer *Player) {
	int Team = Player->Get_Player_Type();
	if (Team == 0 || Team == 1) {
		int OtherTeam = Team?0:1;

		Remove_RTC(Player);

		if (Tally_Team_Size(OtherTeam)-(Tally_Team_Size(Team)-1) > 1 && Queue[OtherTeam].Count()) { //If this player leaving made the teams uneven then accept an RTC.
			cPlayer *RTCPlayer = Queue[OtherTeam][0].Player;
			Queue[OtherTeam].Delete(0);
				
			Change_Team_3(RTCPlayer,Team);
			Set_Can_RTC(RTCPlayer,false);

			DA::Host_Message("%ls has changed teams.",RTCPlayer->Get_Name());
		}
	}
}

void DARTCGameFeatureClass::Think() {
	for (int i = Queue[0].Count()-1;i >= 0;i--) {
		if (The_Game()->GameDuration_Seconds-Queue[0][i].Timeout >= 30) {
			DA::Page_Player(Queue[0][i].Player,"Your team change request has timed out.");
			Queue[0].Delete(i);
		}
		else {
			break;
		}
	}
	for (int i = Queue[1].Count()-1;i >= 0;i--) {
		if (The_Game()->GameDuration_Seconds-Queue[1][i].Timeout >= 30) {
			DA::Page_Player(Queue[1][i].Player,"Your team change request has timed out.");
			Queue[1].Delete(i);
		}
		else {
			break;
		}
	}
}

void DARTCGameFeatureClass::Team_Change_Event(cPlayer *Player) {
	Remove_RTC(Player);
}

bool DARTCGameFeatureClass::Team_Change_Request_Event(cPlayer *Player) {
	int TeamCount[2];
	TeamCount[0] = Tally_Team_Size(0);
	TeamCount[1] = Tally_Team_Size(1);

	int Team = Player->Get_Player_Type();
	int OtherTeam = Team?0:1;

	if (DATeamManager::Get_Force_Team() != -1) {
		DA::Page_Player(Player,"You can not request a team change while force teaming is in effect.");
	}
	else if (Team != 0 && Team != 1) {
		DA::Page_Player(Player,"You can not request a team change while on the neutral team.");
	}
	else if (TeamCount[Team]-TeamCount[OtherTeam] > 1) { //If their team has 2+ more players let them change regardless.
		Change_Team_3(Player,OtherTeam);
		Set_Can_RTC(Player,false);
		if (Player->SessionTime >= 5.0f) { //Change silently if they just joined the game.
			DA::Host_Message("%ls has changed teams.",Player->Get_Name());
		}
	}
	else if (Is_RTC(Player)) {
		DA::Page_Player(Player,"You already have a team change request pending.");
	}
	else if (!Get_Can_RTC(Player) || (ScoreLimit && (int)Player->Get_Score() >= ScoreLimit)) {
		DA::Page_Player(Player,"You can not request a team change as you have already started playing.");
	}
	else if (Queue[OtherTeam].Count()) { //If there's someone in the queue swap with them.
		cPlayer *RTCPlayer = Queue[OtherTeam][0].Player;
		Queue[OtherTeam].Delete(0);
		
		Change_Team_3(Player,OtherTeam);
		Set_Can_RTC(Player,false);
		Change_Team_3(RTCPlayer,Team);
		Set_Can_RTC(RTCPlayer,false);
		
		DA::Page_Player(RTCPlayer,"%ls has agreed to swap teams with you.",Player->Get_Name());
		DA::Page_Player(Player,"%ls has agreed to swap teams with you.",RTCPlayer->Get_Name());
				
		DA::Host_Message("%ls and %ls have swapped teams.",RTCPlayer->Get_Name(),Player->Get_Name());
	}
	else { //Otherwise open a ticket for this player and announce it.
		Queue[Team].Add(Player);
		DA::Page_Player(Player,"Teams are currently balanced. You will only be allowed to change teams if a player on the other team agrees to take your place.");
		DA::Host_Message("%ls has requested to change teams. If you are on team %ls, and wish to switch to team %ls, type \"!rtc\".",Player->Get_Name(),Get_Wide_Team_Name(OtherTeam),Get_Wide_Team_Name(Team));
	}
	return false;
}

bool DARTCGameFeatureClass::RTC_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	DAEventManager::Team_Change_Request_Event(Player->Get_Id());
	return false;
}

void DARTCGameFeatureClass::Remove_RTC(cPlayer *Player) {
	for (int i = 0;i < Queue[0].Count();i++) {
		if (Queue[0][i].Player == Player) {
			Queue[0].Delete(i);
			break;
		}
	}

	for (int i = 0;i < Queue[1].Count();i++) {
		if (Queue[1][i].Player == Player) {
			Queue[1].Delete(i);
			break;
		}
	}
}

bool DARTCGameFeatureClass::Is_RTC(cPlayer *Player) {
	int Team = Player->Get_Player_Type();

	for (int i = 0;i < Queue[Team].Count();i++) {
		if (Queue[Team][i].Player == Player) {
			return true;
		}
	}
	return false;
}

Register_Game_Feature(DARTCGameFeatureClass,"Request Team Change","EnableRTC",0);