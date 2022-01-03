/*	Renegade Scripts.dll
    Dragonade Example Plugin
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
#include "engine.h"
#include "engine_da.h"
#include "da.h"
#include "da_example.h"
#include "da_settings.h"
#include "da_translation.h"
#include "da_vehicle.h"
#include "GameObjManager.h"

/*
The following are examples on how to use some of the systems in DA:

Event System - da_example.cpp, da_example_gamemode.cpp/h, and da_example_gamefeature.cpp/h
Game Mode System - da_example_gamemode.cpp/h
Game Feature System - da_example_gamefeature.cpp/h
Player Observers - da_example_playerobserver.cpp/h and da_example_gamemode.cpp/h
Player Data Manager - da_example_gamefeature.cpp/h
Improved GameObject Observers - da_example_gameobjobserver.cpp/h and da_example_gamefeature.cpp/h
Chat Commands and Keyhook - da_example_chatkeyhook.cpp/h, da_example_gamefeature.cpp/h, and da_example_gamemode.cpp/h
Console Functions - da_example.cpp/h
Crates - da_example_crates.cpp
Settings - da_example.cpp/h and da_example_gamefeature.cpp/h

These systems are designed to allow features to be decentralized. No longer do you have to manually initialize everything from one place,
nor do you need a central settings struct that holds all settings. Instead game features and game modes are registered using a registrant
and automatically loaded when enabled, and automatically unloaded when disabled.

Note that DA has no plugin interface. Instead you just use the various systems and managers to implement things in the exact same way DA itself does.
*/


/*
DA's core is divided into various manager classes, each one handling a different type of object or aspect of the game. 
Generally, managers implement the following types of things:
-Essential functionality that is used to implement other features.
-Non-essential functionality that is centralized so there aren't multiple implementions of it in various game features.
-Settings too small, or that would not be appropriate, to make a game feature out of.
-Log messages.
-Default events.
-Console commands.



DAEventManager: da_event.cpp/h
-Handles event system.
-Installs hooks.
-Interacted with through the DAEventClass, not directly.

DASettingsManager: da_settings.cpp/h
-Loads settings files.
-Provides various functions to retrieve settings.
-Force settings reload by calling Reload.
-Files can be added/removed from the settings chain by Add_Settings/Remove_Settings.

DAGameObjManager: da_gameobj.cpp/h
-Handles calling the extra events added in DAGameObjManager.
-Functions Set_GameObj_TT_Only, Set_GameObj_Stock_Only, and Set_GameObj_Invisible to control gameobject visibilty for different clients.
-Overloads Commands->Enable_Stealth to make it work for non-TT clients on soldiers.

DAPlayerManager: da_player.cpp/h
-Creates and manages each player's DAPlayerClass.
-Default connection request and suicide events.
-Implements DisallowedNickCharacters, DisallowedNickFirstCharacters, and DisallowedNicks settings.
-Controls stock kill messages, and kill and death counters.
-Writes purchase log messages.
-Not interacted with directly.
DAPlayerClass: da_player.cpp/h
-Handles player observers.
-Handles player data.
-Implements C4 locking, beacon locking, muting, and tags.
-Holds player data used by the managers.
-Call cPlayer::Get_DA_Player to get that player's DAPlayerClass.

DAGameManager: da_game.cpp/h
-Selects game mode.
-Loads/unloads game features.
-Adds da.ini and gamemode.ini to the settings chain.
-Writes level loaded and gameover log message.
-Functions provided to get current game mode or a list of game features.

DALogManager: da_log.cpp/h
-Handles TCP log connection to bots.
-Provides functions Write_Log and Write_GameLog to send log messages.

DATranslationManager: da_translate.cpp/h
-Loads translation DB from da.ini.
-Provides functions Translate, Translate_With_Team_Name, Translate_Weapon, and Translate_Soldier to translate object names.

DADamageLog: da_damage.cpp/h
-Records all damage done to an object in the last 30 seconds.
-Includes functions to turn this damage log into a damage table, get the highest damager, get total damage done by a team, etc.

DATeamManager: da_team.cpp/h
-Handles team remixing, rebalancing, and swapping.
-Implements RebalanceTime, RemixFrequency, SwapChance, EnableRebalance, EnableFreeTeamChanging, and ForceTeam settings.
-Default team change request event.

DAVehicleManager: da_vehicle.cpp/h
-Loads VehicleLimit setting.
-Implements per-player and per-team vehicle limits(NYI).
-Checks vehicle limit and vehicle factory status for vehicle purchases.
-Default vehicle purchase event.
-Functions Get_Team, Get_Last_Driver, and Get_Last_Player provided to get extra information about a vehicle.
-Writes vehicle kill log messages.

DASoldierManager: da_soldier.cpp/h
-Loads spawn character settings.
-Implements Grant_Weapons setting.
-Default character purchase event.
-Writes player and bot kill log messages.
-Not interacted with directly.

DAC4BeaconManager: da_c4beacon.cpp/h
-Handles all beacon and C4 log messages.
-Sends beacon deploy message to team.
-Implements per-player and per-team C4 limits(NYI).
-Not interacted with directly.

DABuildingManager: da_building.cpp/h
-Plays building sounds.
-Writes building log messages.
-Not interacted with directly.

DAChatCommandManager: da_chatcommand.cpp/h
-Holds global and event chat commands.
-Parses chat and calls chat commands.
-Interacted with through DAEventClass, DAPlayerObserverClass, and DAChatCommandRegistrant.

DAPluginManager: da_plugin.cpp/h
-Loads plugins.
-Not interacted with directly.

DADisableListManager: da_disable.cpp/h
-Implements various disable list settings.
-Functions Is_Preset_Disabled and Is_Preset_Disabled_For_Character provided to check disable list.

DACrateManager: da_cratemanager.cpp/h
-Static part holds crate registrants, crate modifier registrants, and adds da_crates.ini to the settings chain.
-Virtual part implements the Crates Game Feature.
*/


/*
At the heart of DA is the Event System. The DAEventClass includes over 60 different events divided into a few different types.
Managers, game features, and game modes all make use of this class to implement their functionality.

Event Type:
1. DAEvent. These are events that can happen either to players, or the game in general. They are registered with Register_Event.
2. DAObjectEvent. These can involve multiple types of objects. The second parameter determines what type of objects will trigger the event.
   Registered with Register_Object_Event.

Event Format:
1. Returns bool/int. These allow you to block or somehow affect the event. If suffixed with "Request" then it will be paired with a similarly named
event that triggers after the event is completed. Examples of this would be the chat, purchase, and damage events.
2. Void. These trigger after the event is complete and cannot be blocked or modified. Examples of this are the player leave and level loaded events.

DAEventClasses have timer functionality and can have chat commands or key hooks directly bound to them.

See da_event.h for the full list of events, or da_example_gamemode.cpp/h and da_example_gamefeature.cpp/h for more examples.
*/



DAExampleEventClass::DAExampleEventClass() {
	Register_Event(DAEvent::DALOG);
	Register_Event(DAEvent::REFILL,INT_MAX); //Using a priority of INT_MAX insures we can block these event before something else implements them.
	Register_Event(DAEvent::SUICIDE,INT_MAX);
	Register_Event(DAEvent::TEAMCHANGEREQUEST,INT_MAX);
}

void DAExampleEventClass::DA_Log_Event(const char *Header,const char *Output) {
	Console_Output("DALog: %s %s\n",Header,Output); //Output the DA log to the console.
}

bool DAExampleEventClass::Refill_Event(cPlayer *Player) {
	return false; //Block all refills.
}


bool DAExampleEventClass::Suicide_Event(cPlayer *Player) {
	return false; //Block suicides.
}

bool DAExampleEventClass::Team_Change_Request_Event(cPlayer *Player) {
	return false; //Block team change requests. This will catch both the team change button and the rtc command.
}


//If you're making something simple that doesn't need an enable setting, or you just don't want to use the game feature or game mode systems, this is how to do it.
DAExampleEventClass DAExampleEvent1; //Will be initialized when the dll is loaded.
DAExampleEventClass *DAExampleEvent2 = 0; //Another method. Created using new below.

extern "C" {

//These can be left out when using method 1.
_declspec(dllexport) void Plugin_Init() {
	DAExampleEvent2 = new DAExampleEventClass;
	DASettingsManager::Add_Settings("da_example_settings.ini"); //Add file to the settings chain. DASettingsManager will now load settings from this file when used.
}

_declspec(dllexport) void Plugin_Shutdown() {
	delete DAExampleEvent2;
}

}



class DAExampleConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "example"; }
	const char *Get_Alias() { return "examplealias"; }
	const char *Get_Help() { return "EXAMPLE <playerid> - Provides an example."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			DA::Page_Player(Player,"1 2 3");
		}
	}
};
Register_Console_Function(DAExampleConsoleFunctionClass); //This macro can be used to add the console function instead of doing it manually.