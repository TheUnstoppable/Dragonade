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

#ifndef INCLUDE_DARTC
#define INCLUDE_DARTC

#include "da_event.h"
#include "da_gamefeature.h"
#include "da_player.h"


struct DARTCStruct {
	DARTCStruct() {
		Player = 0;
		Timeout = The_Game()->GameDuration_Seconds;
	}
	DARTCStruct(cPlayer *player) {
		Player = player;
		Timeout = The_Game()->GameDuration_Seconds;
	}
	bool operator ==(const DARTCStruct &Struct) {
		return false;
	}
	bool operator !=(const DARTCStruct &Struct) {
		return false;
	}
	cPlayer *Player;
	unsigned int Timeout;
};

class DARTCPlayerDataClass : public DAPlayerDataClass {
public:
	virtual void Init() {
		CanRTC = true;
	}
	virtual void Clear_Level() {
		CanRTC = true;
	}
	bool CanRTC;
};

class DARTCGameFeatureClass : public DAEventClass, private DAPlayerDataManagerClass<DARTCPlayerDataClass>, public DAGameFeatureClass {
	virtual void Init();
	virtual void Settings_Loaded_Event();
	virtual bool Chat_Command_Event(cPlayer *Player,TextMessageEnum Type,const StringClass &Command,const DATokenClass &Text,int ReceiverID);
	virtual void Player_Leave_Event(cPlayer *Player);
	virtual void Think();
	virtual bool Team_Change_Request_Event(cPlayer *Player);
	virtual void Team_Change_Event(cPlayer *Player);

	bool RTC_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);

	void Remove_RTC(cPlayer *Player);
	inline void Clear_RTC() {
		Queue[0].Delete_All();
		Queue[1].Delete_All();
	}
	bool Is_RTC(cPlayer *Player);
	inline bool Get_Can_RTC(cPlayer *Player) {
		return Get_Player_Data(Player)->CanRTC;
	}
	inline void Set_Can_RTC(cPlayer *Player,bool Can) {
		Get_Player_Data(Player)->CanRTC = Can;
	}

	DynamicVectorClass<DARTCStruct> Queue[2];

	//Settings
	DATokenClass DisablingCommands;
	int ScoreLimit;
};

#endif