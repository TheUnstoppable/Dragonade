/*	Renegade Scripts.dll
    Dragonade Advanced Kill Messages Game Feature
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#include "General.h"
#include "da.h"
#include "da_killmessages.h"
#include "da_settings.h"
#include "da_player.h"

void DAAdvancedKillMessagesGameFeatureClass::Init() {
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Event(DAEvent::DALOG);
}

void DAAdvancedKillMessagesGameFeatureClass::Settings_Loaded_Event() {
	EnablePlayer = DASettingsManager::Get_Bool("EnablePlayerKillMessages",true);
	EnableVehicle = DASettingsManager::Get_Bool("EnableVehicleKillMessages",true);
	EnableBuilding = DASettingsManager::Get_Bool("EnableBuildingKillMessages",true);
	EnableBot = DASettingsManager::Get_Bool("EnableBotKillMessages",true);
	DAPlayerManager::Set_Disable_Kill_Messages(EnablePlayer);
}

//This just displays the kill messages ingame. Actual messages are made by the soldier, vehicle, and building managers.
void DAAdvancedKillMessagesGameFeatureClass::DA_Log_Event(const char *Header,const char *Output) {
	if ((EnablePlayer && !strcmp(Header,"_PLAYERKILL")) || (EnableVehicle && (!strcmp(Header,"_VEHKILL") || !strcmp(Header,"_HARVKILL"))) || (EnableBuilding && !strcmp(Header,"_BUILDINGKILL")) || (EnableBot && !strcmp(Header,"_BOTKILL"))) {
		int Color = 2; //First character of the message is the color to use.
		if (Output[0] == '-') { //Negative team, need to advance Output an extra character.
			Output += 3;
		}
		else {
			Color = Output[0]-48; //Turn the value of the character into an int.
			Output += 2; //Advance output past the color number.
			if (Color < 0 || Color > 2) { //Anything besides 0 and 1 is white.
				Color = 2;
			}
		}
		DA::Color_Message_With_Team_Color(Color,"%s",Output);
	}
}

Register_Game_Feature(DAAdvancedKillMessagesGameFeatureClass,"Advanced Kill Messages","EnableAdvancedKillMessages",0);