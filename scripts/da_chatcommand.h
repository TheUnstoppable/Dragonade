/*	Renegade Scripts.dll
    Dragonade Chat Command Manager
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DACHATCOMMAND
#define INCLUDE_DACHATCOMMAND

#include "da_event.h"
#include "da_token.h"
#include "engine_ttdef.h"

class DAChatCommandClass abstract {
public:
	DynamicVectorClass<StringClass> Triggers;
	DAAccessLevel::Level AccessLevel;
	DAChatType::Type ChatType;
	int Parameters;
	virtual bool Activate(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) = 0;
	virtual ~DAChatCommandClass() { }
};

struct DAEventChatCommandStruct {
	DynamicVectorClass<StringClass> Triggers;
	DAAccessLevel::Level AccessLevel;
	DAChatType::Type ChatType;
	int Parameters;
	DAEventClass *Base;
	DAECC Func;
};

class DAKeyHookClass abstract {
public:
	DynamicVectorClass<StringClass> Triggers;
	virtual void Activate(cPlayer *Player) = 0;
	virtual ~DAKeyHookClass() { }
};

struct DAEventKeyHookStruct {
	DynamicVectorClass<StringClass> Triggers;
	DAEventClass *Base;
	DAEKH Func;
};

class DAChatCommandManager : public DAEventClass {
public:
	static void Init();
	static void Shutdown();
	virtual bool Chat_Command_Event(cPlayer *Player,TextMessageEnum Type,const StringClass &Command,const DATokenClass &Text,int ReceiverID);
	virtual bool Key_Hook_Event(cPlayer *Player,const StringClass &Key);
	DA_API static void Register_Chat_Command(DAChatCommandClass *Base,const char *Triggers,int Parameters = 0,DAAccessLevel::Level AccessLevel = DAAccessLevel::NONE,DAChatType::Type ChatType = DAChatType::ALL);
	DA_API static void Register_Event_Chat_Command(DAEventClass *Base,DAECC Func,const char *Triggers,int Parameters = 0,DAAccessLevel::Level AccessLevel = DAAccessLevel::NONE,DAChatType::Type ChatType = DAChatType::ALL);
	DA_API static void Register_Key_Hook(DAKeyHookClass *Base,const char *Triggers);
	DA_API static void Register_Event_Key_Hook(DAEventClass *Base,DAEKH Func,const char *Triggers);
	DA_API static void Unregister_Chat_Command(const char *Trigger);
	DA_API static void Unregister_Event_Chat_Command(DAEventClass *Base,const char *Trigger);
	DA_API static void Unregister_Key_Hook(const char *Trigger);
	DA_API static void Unregister_Event_Key_Hook(DAEventClass *Base,const char *Trigger);
	DA_API static void Clear_Event_Chat_Commands(DAEventClass *Base);
	DA_API static void Clear_Event_Key_Hooks(DAEventClass *Base);

private:
	static DynamicVectorClass<DAChatCommandClass*> ChatCommands;
	static DynamicVectorClass<DAEventChatCommandStruct*> EventChatCommands;
	static DynamicVectorClass<DAKeyHookClass*> KeyHooks;
	static DynamicVectorClass<DAEventKeyHookStruct*> EventKeyHooks;
	static DATokenClass NullToken;
};

template <class T> class DAChatCommandRegistrant {
public:
	DAChatCommandRegistrant(const char *Triggers,int Parameters = 0,DAAccessLevel::Level AccessLevel = DAAccessLevel::NONE,DAChatType::Type ChatType = DAChatType::ALL) {
		DAChatCommandManager::Register_Chat_Command(new T,Triggers,Parameters,AccessLevel,ChatType);
	}
};
#define Register_Simple_Chat_Command(ClassName,Triggers) DAChatCommandRegistrant<ClassName> ClassName##Registrant(Triggers); //This has to be named as such so it doesn't break the various functions named Register_Chat_Command.
#define Register_Full_Chat_Command(ClassName,Triggers,Parameters,AccessLevel,ChatType) DAChatCommandRegistrant<ClassName> ClassName##Registrant(Triggers,Parameters,AccessLevel,ChatType);


template <class T> class DAKeyHookRegistrant {
public:
	DAKeyHookRegistrant(const char *Triggers) {
		DAChatCommandManager::Register_Key_Hook(new T,Triggers);
	}
};
#define Register_Simple_Key_Hook(ClassName,Triggers) DAKeyHookRegistrant<ClassName> ClassName##Registrant(Triggers); //This has to be named as such so it doesn't break the various functions named Register_Key_Hook.

#endif