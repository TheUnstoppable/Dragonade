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
#include "da_example_gamefeature.h"
#include "da_example_gameobjobserver.h"
#include "da_settings.h"

/*
Each game feature is controlled by a setting defined in its registrant. On level load all game features are checked and automatically 
loaded/unloaded as necessary.
If your game feature needs to store settings they should be included in its class definition, not global variables.
*/
void DAExampleGameFeatueClass::Init() {
	//Initialize the player data manager. From this point all data management will be done automatically.
	DAPlayerDataManagerClass<DAExamplePlayerDataClass>::Init();

	//Register our events.
	Register_Event(DAEvent::REFILL,INT_MAX); //The second parameter here is the priority. We want to make sure these trigger and block the event before anything else.
	Register_Event(DAEvent::TEAMCHANGEREQUEST,INT_MAX);
	Register_Object_Event(DAObjectEvent::DAMAGEDEALTREQUEST,DAObjectEvent::PLAYER); //Only triggers when a player is damaging an object.

	//Chat commands can be bound directly to a DAEventClass. This allows chat commands to be automatically loaded and unloaded along with game modes and game features.
	//The somewhat ugly format is a limitation of the C++ language.
	Register_Chat_Command((DAECC)&DAExampleGameFeatueClass::Example3_Chat_Command,"!example3",0,DAAccessLevel::NONE,DAChatType::TEAM | DAChatType::PRIVATE);

	//DAEventClass includes a timer system. Timers can have multiple data parameters and can be set to repeat.
	Start_Timer(1,5.0f,true); //Triggers every 5 seconds repeating.


	//The settings manager automatically loads settings from all files in the settings chain.
	//Be default da.ini, da_crates.ini, and gamemode.ini are in the settings chain with the following priority.
	//gamemode.ini map specific > gamemode.ini general > da_crates.ini map specific > da_crates.ini general > da.ini map specific > da.ini general
	NodDamageScale = DASettingsManager::Get_Float("NodDamageScale",1.1f); //By default Nod takes 10% more damage.
	GDIDamageScale = DASettingsManager::Get_Float("GDIDamageScale",0.8f); //By default GDI takes 20% less damage.
}

bool DAExampleGameFeatueClass::Team_Change_Request_Event(cPlayer *Player) {
	return false; //Block team change requests.
}

bool DAExampleGameFeatueClass::Damage_Request_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type) {
	if (Victim->Get_Player_Type() == 0) {
		Damage *= NodDamageScale; //Scale damage based on loaded setting.
	}
	else if (Victim->Get_Player_Type() == 1) {
		Damage *= GDIDamageScale;
	}
	return true;
}

void DAExampleGameFeatueClass::Timer_Expired(int Number,unsigned int Data) {
	Give_Credits_Team(0,100.0f); //All players get 100 credits every 5 seconds.
	Give_Credits_Team(1,100.0f);
}

bool DAExampleGameFeatueClass::Example3_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	//Allow players to buy the following gameobject observer. See da_example_gameobjobserver.cpp/h
	VehicleGameObj *Vehicle = Player->Get_GameObj()->Get_Vehicle();
	if (Vehicle) {
		if (!Get_Player_Data(Player)->Purchased) { //Limit one per customer.
			if (Player->Purchase_Item(2000)) {
				Vehicle->Add_Observer(new DAExampleGameObjObserverClass);
				Get_Player_Data(Player)->Purchased = true;
			}
		}
	}
	return false;
}

/*
This is the game feature registrant. It registers the game feature with the game feature manager.
The first parameter is the name. It's used in the loaded message and listed in GameSpy. You can leave this blank if you don't want the game feature to be listed on GameSpy or have a loaded message.
The second parameter is the setting name. That's what you use in the settings file to enable or disabled the game feature. You can leave this blank to have the game feature always be enabled.
The third optional parameter is the "required data." This works the same way as in the game mode and defines some data that has to exist for the game feature to work. 
One example would be "Crate1_X", spawn locations for crate spawns. If this data is not defined the game feature will be automatically unloaded for that map.
*/
Register_Game_Feature(DAExampleGameFeatueClass,"Example Game Feature","EnableExample",0);
/*
Register_Game_Feature is a macro that produces the following code:
DAGameFeatureRegistrant<DAExampleGameFeatueClass> DAExampleGameFeatueClassRegistrant("Example Game Feature","EnableExample",0);

It's possible to overwrite existing game features by simply registering your game feature with the same name.
The following would replace the Taunts game feature with our example game feature
Register_Game_Feature(DAExampleGameFeatueClass,"Taunts","EnableTaunts",0);
*/