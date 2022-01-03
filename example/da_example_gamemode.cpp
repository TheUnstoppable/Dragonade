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
#include "da_example_gamemode.h"
#include "da_example_playerobserver.h"
#include "GameObjManager.h"

/*
Each game mode has its own settings file in which all settings can be defined. This allows you to enable infinite ammo for only the Domination game mode, for exmaple.
The current game mode is controlled by the "GameMode" setting in da.ini. This can of course be set per map.
The following is our example game mode. It's a sort of vehicles only game mode.
*/
void DAExampleGameModeClass::Init() {
	//Here we register the events we want.

	//DAEvents are events that can happen either to players, or the game in general.
	Register_Event(DAEvent::CHARACTERPURCHASEREQUEST);
	Register_Event(DAEvent::POWERUPGRANTREQUEST);
	Register_Event(DAEvent::ADDWEAPONREQUEST);


	//DAObjectEvents can involve multiple types of objects. The second parameter determines what type of objects will trigger the event.

	//DAMAGERECEIVEDREQUEST means the object type applies to the object being damaged. DAMAGEDEALTREQUEST would make it apply to the object doing the damage, instead.
	Register_Object_Event(DAObjectEvent::DAMAGERECEIVEDREQUEST,DAObjectEvent::BUILDING); //This event will only trigger when buildings are damaged.

	Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::VEHICLE); //This event will only trigger when vehicles are created.

	//Chat commands can be bound directly to a DAEventClass. This allows chat commands to be automatically loaded and unloaded along with game modes and game features.
	//The somewhat ugly format is a limitation of the C++ language.
	Register_Chat_Command((DAECC)&DAExampleGameModeClass::Example4_Chat_Command,"!example4"); //Short version with defaults.
}

int DAExampleGameModeClass::Character_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const SoldierGameObjDef *Item) {
	return 4; //Since this is supposed to be vehicles only block all character purchases. 4 is "Item not in stock."
}

int DAExampleGameModeClass::PowerUp_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const PowerUpGameObjDef *Item) {
	return 4; //Since this is supposed to be vehicles only block all powerup purchases. 4 is "Item not in stock."
}

bool DAExampleGameModeClass::Add_Weapon_Request_Event(cPlayer *Player,const WeaponDefinitionClass *Weapon) {
	return false; //Block players from having any weapons.
}

bool DAExampleGameModeClass::Damage_Request_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type) {
	Damage *= 0.5f; //Since there's no way to repair in this game mode we will half all damage to buildings.
	return true;
}

void DAExampleGameModeClass::Object_Created_Event(GameObject *obj) {
	Start_Timer(1,0.1f,false,obj->Get_ID()); //Start a timer with the ID of the gameobject as the data.
}

void DAExampleGameModeClass::Timer_Expired(int Number,unsigned int Data) {
	VehicleGameObj *Vehicle = (VehicleGameObj*)GameObjManager::Find_SmartGameObj(Data); //Turn the ID back into a gameobject.
	if (Vehicle && Vehicle->Get_Lock_Owner()) { //Just in case.
		Vehicle->Add_Occupant((SoldierGameObj*)Vehicle->Get_Lock_Owner(),0); //Force the purchaser of the vehicle into it.
	}
}

bool DAExampleGameModeClass::Example4_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	//Allow players to buy the following player observer. See da_example_playerobserver.cpp/h
	if (!Player->Get_DA_Player()->Find_Observer("DAExamplePlayerObserverClass")) { //Don't want players to have multiple of this.
		if (Player->Purchase_Item(1000)) {
			Player->Get_DA_Player()->Add_Observer(new DAExamplePlayerObserverClass);
		}
	}
	return true;
}

/*
Here's our game mode registrant. It registers the game mode with the game mode manager.
The first parameter is the "short name." This is used for the GameMode setting in da.ini, the name of the game mode specific settings file,
the game mode setting on GameSpy, and appended to the server title if that is enabled.
The second parameter is the "long name." This is used in any messages involving this game mode. Such as the "Running in %s mode." message.
The third optional parameter is the "required setting." This is for if the game mode requires certain data to work on a map, such as flag locations in CTF. 
For that you might put "GDIFlag_X." Game modes such as sniping or infantry only inheritly work on all maps and don't need to make use of this setting.
*/
Register_Game_Mode(DAExampleGameModeClass,"Example","Super Fun Example",0);
/*
Register_Game_Mode is a macro that produces the following code:
DAGameModeRegistrant<DAExampleGameModeClass> DAExampleGameModeClassRegistrant("Example","Super Fun Example",0);

It's possible to overwrite existing game modes by registering your game mode with the same name.
The following would replace the Domination game mode with our example game mode.
Register_Game_Mode(DAExampleGameModeClass,"Domination","Domination",0);
*/