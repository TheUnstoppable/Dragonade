/*	Renegade Scripts.dll
    RenCorner Sounds Game Feature
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
#include "engine.h"
#include "engine_da.h"
#include "da.h"
#include "da_settings.h"
#include "da_chatsounds.h"
#include "cScTextObj.h"


class DAMuteSoundsConsoleFunctionClass : public ConsoleFunctionClass {
public:
	DAMuteSoundsConsoleFunctionClass(DAChatSoundsGameFeatureClass *Instance) {
		this->Instance = Instance;
	}
	const char *Get_Name() { return "mutesounds"; }
	const char *Get_Help() { return "MUTESOUNDS <playerid> - Prevent a player from using sound commands."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Instance->Get_Player_Data(Player)->Mute = true;
		}
	}
	DAChatSoundsGameFeatureClass *Instance;
};

class DAUnMuteSoundsConsoleFunctionClass : public ConsoleFunctionClass {
public:
	DAUnMuteSoundsConsoleFunctionClass(DAChatSoundsGameFeatureClass *Instance) {
		this->Instance = Instance;
	}
	const char *Get_Name() { return "unmutesounds"; }
	const char *Get_Help() { return "UNMUTESOUNDS <playerid> - Re-allow a player to use sound commands."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Instance->Get_Player_Data(Player)->Mute = false;
		}
	}
	DAChatSoundsGameFeatureClass *Instance;
};

void DAChatSoundsGameFeatureClass::Init() {
	DAPlayerDataManagerClass<DAChatSoundsPlayerDataClass>::Init();
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Event(DAEvent::CHAT);
	Register_Chat_Command((DAECC)&DAChatSoundsGameFeatureClass::EnableSounds_Chat_Command,"!enablesounds|!enablesound");
	Register_Chat_Command((DAECC)&DAChatSoundsGameFeatureClass::DisableSounds_Chat_Command,"!disablesounds|!disablesound");
	Register_Chat_Command((DAECC)&DAChatSoundsGameFeatureClass::Sounds_Chat_Command,"!sounds|!sound");
	Add_Console_Function(new DAMuteSoundsConsoleFunctionClass(this));
	Add_Console_Function(new DAUnMuteSoundsConsoleFunctionClass(this));
	LastUsed = 0;
}

DAChatSoundsGameFeatureClass::~DAChatSoundsGameFeatureClass() {
	for (int i = 0;i < ConsoleFunctionList.Count();i++) {
		if (!_stricmp(ConsoleFunctionList[i]->Get_Name(),"mutesounds") || !_stricmp(ConsoleFunctionList[i]->Get_Name(),"unmutesounds")) {
			delete ConsoleFunctionList[i];
			ConsoleFunctionList.Delete(i);
			i--;
		}
	}
}

void DAChatSoundsGameFeatureClass::Settings_Loaded_Event() {
	Sounds.Remove_All();
	INISection *Section = DASettingsManager::Get_Section("Chat_Sounds");
	if (Section) {
		for (INIEntry *i = Section->EntryList.First();i && i->Is_Valid();i = i->Next()) {
			if (i->Entry && i->Value) {
				Sounds.Insert(i->Entry,i->Value);
			}
		}
	}
	Delay = (unsigned int)DASettingsManager::Get_Int("ChatSoundDelay",5)*1000;
}

bool DAChatSoundsGameFeatureClass::EnableSounds_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	Get_Player_Data(Player)->Enable = true;
	DA::Page_Player(Player,"You will now hear sound commands. To disable them type \"!disablesounds\".");
	return true;
}

bool DAChatSoundsGameFeatureClass::DisableSounds_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	Get_Player_Data(Player)->Enable = false;
	DA::Page_Player(Player,"You will no longer hear sound commands. To enable them type \"!enablesounds\".");
	return true;
}

bool DAChatSoundsGameFeatureClass::Sounds_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	StringClass Send;
	for (HashTemplateIterator<StringClass,StringClass> it = Sounds;it;++it) {
		StringClass Buffer;
		Buffer.Format("%s, ",it.getKey());
		if (Send.Get_Length() + Buffer.Get_Length() > 200) {
			Send.TruncateRight(3);
			DA::Page_Player(Player,"%s",Send);
			Send = Buffer;
		}
		else {
			Send += Buffer;
		}
	}
	if (!Send.Is_Empty()) {
		Send.TruncateRight(3);
		DA::Page_Player(Player,"%s",Send);
	}
	DA::Page_Player(Player,"You can also use the \"!enablesounds\" and \"!disablesounds\" commands to turn sounds on or off for yourself.");
	return true;
}

bool DAChatSoundsGameFeatureClass::Chat_Event(cPlayer *Player,TextMessageEnum Type,const wchar_t *Message,int ReceiverID) {
	StringClass *Sound = Sounds.Get(Message);
	if (Sound) {
		if (GetTickCount()-LastUsed < Delay) {
			//DA::Page_Player(Player,"A sound command may only be used once every %u second(s). You must wait %u second(s) before using one again.",Delay/1000,(Delay-(GetTickCount()-LastUsed))/1000);
			return true;
		}
		cScTextObj *Text = Send_Client_Text(WideStringFormat(L"j\n95\n%hs\n,",*Sound),TEXT_MESSAGE_PUBLIC,false,-2,-1,false,false);
		DAChatSoundsPlayerDataClass *Data = Get_Player_Data(Player);
		if (Data->Mute || Type == TEXT_MESSAGE_PRIVATE) {
			if (Data->Enable) {
				Text->Set_Object_Dirty_Bit(Player->Get_Id(),NetworkObjectClass::BIT_CREATION,true);
			}
		}
		else if (Type == TEXT_MESSAGE_TEAM) {
			LastUsed = GetTickCount();
			for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
				if (z->Data()->Is_Active() && Get_Player_Data(z->Data())->Enable && z->Data()->Get_Player_Type() == Player->Get_Player_Type()) {
					Text->Set_Object_Dirty_Bit(z->Data()->Get_Id(),NetworkObjectClass::BIT_CREATION,true);
				}
			}
		}
		else {
			LastUsed = GetTickCount();
			for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
				if (z->Data()->Is_Active() && Get_Player_Data(z->Data())->Enable) {
					Text->Set_Object_Dirty_Bit(z->Data()->Get_Id(),NetworkObjectClass::BIT_CREATION,true);
				}
			}
		}
	}
	return true;
}

Register_Game_Feature(DAChatSoundsGameFeatureClass,"Chat Sounds","EnableChatSounds",0);

