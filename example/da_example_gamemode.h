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

#ifndef INCLUDE_DAEXAMPLE_GAMEMODE
#define INCLUDE_DAEXAMPLE_GAMODE

#include "da_event.h" //For DAEventClass definition.
#include "da_gamemode.h" //For DAGameModeClass definition.

//Game modes inherit from DAGameModeClass, and if you want any events you can inherit from DAEventClass too.
class DAExampleGameModeClass : public DAEventClass, public DAGameModeClass {
	virtual void Init(); //Called each time this game mode is loaded. Note that the game mode is unloaded and reloaded every map.

	//The events needed to implement this game mode.
	virtual int Character_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const SoldierGameObjDef *Item);
	virtual int PowerUp_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const PowerUpGameObjDef *Item);
	virtual bool Add_Weapon_Request_Event(cPlayer *Player,const WeaponDefinitionClass *Weapon);
	virtual bool Damage_Request_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type);
	virtual void Object_Created_Event(GameObject *obj);
	virtual void Timer_Expired(int Number,unsigned int Data);
	bool Example4_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
};

#endif