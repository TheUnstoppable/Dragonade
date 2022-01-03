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

#ifndef INCLUDE_DAEXAMPLE_GAMEFEATURE
#define INCLUDE_DAEXAMPLE_GAMEFEATURE

#include "da_event.h" //For DAEventClass definition.
#include "da_gamefeature.h" //For DAGameFeatureClass definition.
#include "da_player.h" //DAPlayerDataClass and DAPlayerDataManagerClass definitions.

//To use the player data manager you create a class that inherits from DAPlayerDataClass and use that class with DAPlayerDataManagerClass. See DAExampleGameFeatueClass.
class DAExamplePlayerDataClass : public DAPlayerDataClass {
public:
	virtual void Init() { //Called when the class is created for a new player.
		Purchased = false;
	}
	virtual void Clear_Level() { //Called when a new map is loaded.
		Purchased = false;
	}
	virtual void Clear_Session() { //Called when the player leaves the game.
	}
	bool Purchased;
};

//Game features inherit from DAGameFeatureClass, and if you want any events you can inherit from DAEventClass too.
//We also inherit from DAPlayerDataManagerClass<DAExamplePlayerDataClass> to use the player data manager.
class DAExampleGameFeatueClass : public DAEventClass, public DAGameFeatureClass, public DAPlayerDataManagerClass<DAExamplePlayerDataClass> {
	virtual void Init(); //Called when this game feature is first loaded.

	//The events needed to implement this game feature.
	virtual bool Team_Change_Request_Event(cPlayer *Player);
	virtual bool Damage_Request_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type);
	virtual void Timer_Expired(int Number,unsigned int Data);
	bool Example3_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);

	float NodDamageScale;
	float GDIDamageScale;
};

#endif