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
#include "da_chatcommand.h"

/*
There are three different ways a chat command or keyhook can be created in DA:

1. Bound to a DAPlayerObserverClass. See da_example_playerobserver.cpp/h.
2. Bound to a DAEventClass. See da_example_gamefeature.cpp/h and da_example_gamemode.cpp/h.
3. Globally using DAChatCommandClass or DAKeyHookClass. See below.

The above are listed in order of priority. If there is a DAPlayerObserverClass and DAChatCommandClass both bound to "!c4" then the DAEventClass will be the one triggered when the command is used.

When a chat command, of any kind, is registered it takes the following parameters:

Triggers: All commands, separated by |, that you want to trigger the chat command. You may also list key hooks here.
Parameters: How many parameters, i.e. words that follow the initial command, that the console command requires. Players will receive an error if they try to use it with too few.
Access Level: The moderation access level a player needs to use the command. A player's access level can be set with the "setaccesslevel" console function.
Chat Type: The type of chat the command can be used in. Can be set to all, a single type, or multiple types.

Key hooks are simpler and only require a list of triggers.
*/

//Inherit from DAChatCommandClass.
class DAExample1ChatCommandClass : public DAChatCommandClass {
	virtual bool Activate(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
		DA::Host_Message("Triggered by %ls in %d with %d parameters",Player->Get_Name(),ChatType,Text.Size());
		return false; //Block the chat command from appearing in chat.
	}
};
/*
Here's the registrant. This is the simple registrant and only takes the triggers. All the other settings will use the defaults.
This will be triggerable by typing either "!example1" or "!ex1" in any chat type or using the "Taunt1" key hook.
However, since DAEventClasses take priority over DAChatCommandClass the "Taunt1" key hook will not work when the Taunts Game Feature is enabled.
*/
Register_Simple_Chat_Command(DAExample1ChatCommandClass,"!example1|!ex1|Taunt1");



class DAExample2ChatCommandClass : public DAChatCommandClass {
	virtual bool Activate(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
		DA::Host_Message("Triggered by %ls with parameters %s",Player->Get_Name(),Text[0]);
		return true; //Allow this one to appear in chat.
	}
};
/*
This time we'll use the full registrant and define some of the more advanced settings.
This command requires atleast 1 parameters, can be triggered by a moderator or above, and has to be used in team or private chat.
*/
Register_Full_Chat_Command(DAExample2ChatCommandClass,"!example2|!ex2",1,DAAccessLevel::FULLMODERATOR,DAChatType::TEAM | DAChatType::PRIVATE);



//Inherit from DAKeyHookClass.
class DAExampleKeyHookClass : public DAKeyHookClass {
	virtual void Activate(cPlayer *Player) {
		DA::Page_Player(Player,"This can be triggered by %d different keyhooks",Triggers.Count());
	}
};
/*
Registrant only needs a list of triggers.
There is no full version of this registrant. It is just named as such for technical reasons.
*/
Register_Simple_Key_Hook(DAExampleKeyHookClass,"ToggleNumPad|VehStatus|VetInfo");
