/*	Renegade Scripts.dll
    Dragonade
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
#include "cScTextObj.h"
#include "da.h"
#include "da_plugin.h"
#include "da_log.h"
#include "da_game.h"
#include "da_gameobj.h"
#include "da_team.h"
#include "da_chatcommand.h"
#include "da_settings.h"
#include "da_player.h"
#include "da_disable.h"
#include "da_translation.h"
#include "da_vehicle.h"
#include "da_soldier.h"
#include "da_c4beacon.h"
#include "da_building.h"
#include "da_damagelog.h"
#include "da_cratemanager.h"
#include "da_veteran.h"
#include "da_ban.h"
#include <random>

#pragma warning(disable: 4073)
#pragma init_seg(lib)

StringClass DA::MessagePrefix;
DynamicVectorClass<ConsoleFunctionClass*> DA::ConsoleFunctions;

std::mt19937 RNG;

/*
Settings/Level Loaded Order:

DAEventManager::Level_Loaded - Call level loaded events.
DAPlayerManager::Level_Loaded - Create/destroy player data.
DAGameManager::Level_Loaded - Select game mode.
DASettingsManager::Reload - Reload settings files.
DAEventManager::Setting_Loaded - Call settings loaded events.
DAPlayerManager::Settings_Loaded - Load player settings.
DAGameManager::Settings_Loaded - Load/unload game features.
DATranslationManager::Settings_Loaded - Load translation DB.
DATeamManager::Settings_Loaded - Load team management settings.
DAVehicleManager::Settings_Loaded - Load vehicle settings.
DASoldierManager::Settings_Loaded - Load soldier settings.
Any other settings loaded events
DATeamManager::Level_Loaded - Remix, rebalance, and/or swap teams
Any other level loaded events with INT_MAX priority
DADisableListManager::Level_Loaded (INT_MAX-1) - Load disable list
Any other level loaded events
*/

DA_API DA_gdv Get_Dragonade_Version;
DA_API DA_gdr Get_Dragonade_Revision;

const char *DA::Get_Version() {
	return "1.11.0";
}

void DA::Init() {
	RNG.seed((unsigned long)time(0));
	Commands->Get_Random_Int = Get_Random_Int;
	Commands->Get_Random = Get_Random_Float;

	HINSTANCE Handle = LoadLibrary("da.dll");
	if (!Handle) {
		MessageBox(NULL, "da.dll was not found. Exiting FDS.", "Error", MB_OK | MB_ICONEXCLAMATION | MB_TOPMOST);
		ExitProcess(HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND));
	}

	Get_Dragonade_Version = (DA_gdv)GetProcAddress(Handle, "Get_Dragonade_Version");
	Get_Dragonade_Revision = (DA_gdr)GetProcAddress(Handle, "Get_Dragonade_Revision");

	DAEventManager::Init(); //These must be initialized in this order.
	DADisableListManager::Init(); //Block disabled presets before any other event classes can hear about them.
	DAGameObjManager::Init();
	DAPlayerManager::Init(); //Handle player data before anything is loaded that might need it.
	DAGameManager::Init(); //Triggers reload of settings after the game mode is selected.
	DALogManager::Init();
	DATranslationManager::Init();
	DADamageLog::Init();
	DATeamManager::Init();
	DAVehicleManager::Init();
	DASoldierManager::Init();
	DAC4BeaconManager::Init();
	DABuildingManager::Init();
	DACrateManager::Static_Init();
	DABanManager::Init();
	DAChatCommandManager::Init();
	DAVeteranManagerClass::Static_Init();
	DAPluginManager::Init();

	for (int i = 0;i < ConsoleFunctions.Count();i++) { //Add any console functions using the registrant to the main list.
		Add_Console_Function(ConsoleFunctions[i]);
	}
	Sort_Function_List();
	Verbose_Help_File();

	DASettingsManager::Get_String(MessagePrefix,"MessagePrefix",0);
	if (!MessagePrefix.Is_Empty()) {
		MessagePrefix += " ";
	}
}

void DA::Shutdown() {
	DABanManager::Shutdown();
	DAPluginManager::Shutdown();
	DAChatCommandManager::Shutdown();
	DALogManager::Shutdown();
	DAGameManager::Shutdown();
	DAPlayerManager::Shutdown();
	DASettingsManager::Shutdown();
}

int Get_Random_Int(int Min,int Max) {
	std::uniform_int_distribution<int> Dis(Min,Max-1);
	return Dis(RNG);
}

float Get_Random_Float(float Min,float Max) {
	std::uniform_real_distribution<float> Dis(Min,Max);
	return Dis(RNG);
}

bool Get_Random_Bool() {
	return !!(RNG() % 2);
}

void DA::Host_Message(const char *Format,...) {
	char Message[256];
	Format_String_Prefix(Message);
	if (DAEventManager::Host_Chat_Event(-1,TEXT_MESSAGE_PUBLIC,Message)) {
		Send_Client_Text(WideStringClass(Message),TEXT_MESSAGE_PUBLIC,false,-1,-1,true,true);
		Commands->Create_2D_WAV_Sound("public_message.wav");
	}
}

void DA::Team_Host_Message(int Team,const char *Format,...) {
	char Message[256];
	Format_String_Prefix(Message);
	cScTextObj *Text = Send_Client_Text(WideStringClass(Message),TEXT_MESSAGE_PUBLIC,false,-1,-1,false,false);
	Set_Object_Dirty_Bit_For_Team_Version_Less_Than(Text,Team,2.6f,NetworkObjectClass::BIT_CREATION,true);
	Send_Message_Team_With_Team_Color(Team,StringFormat("Host: %s",Message));
	Create_2D_WAV_Sound_Team("public_message.wav",Team);
}

void DA::Private_Host_Message(cPlayer *Player,const char *Format,...) {
	char Message[256];
	Format_String_Prefix(Message);
	if (!Player->Get_DA_Player()->Is_Scripts_Client()) {
		cScTextObj *Text = Send_Client_Text(WideStringClass(Message),TEXT_MESSAGE_PUBLIC,false,-1,-1,false,false);
		Text->Set_Object_Dirty_Bits(Player->Get_Id(),NetworkObjectClass::BIT_CREATION);
	}
	else {
		Send_Message_Player_By_ID(Player->Get_Id(),COLORLIGHTBLUE,StringFormat("Host: %s",Message));
	}
	Create_2D_WAV_Sound_Player_By_ID(Player->Get_Id(),"public_message.wav");
}

void DA::Private_Host_Message(int Player,const char *Format,...) {
	cPlayer *P = Find_Player(Player);
	if (P) {
		char Message[256];
		Format_String(Message);
		Private_Host_Message(P,"%s",Message);
	}
}

void DA::Private_Host_Message(GameObject *Player,const char *Format,...) {
	if (Is_Player(Player)) {
		cPlayer *P = ((SoldierGameObj*)Player)->Get_Player();
		char Message[256];
		Format_String(Message);
		Private_Host_Message(P,"%s",Message);
	}
}

void DA::Admin_Message(const char *Format,...) {
	char Message[256];
	Format_String(Message);
	if (DAEventManager::Host_Chat_Event(-1,TEXT_MESSAGE_PUBLIC,Message)) {
		Send_Client_Text(WideStringClass(Message),TEXT_MESSAGE_PUBLIC,true,-1,-1,true,true);
	}
}

void DA::Team_Admin_Message(int Team,const char *Format,...) {
	char Message[256];
	Format_String(Message);
	cScTextObj *Text = Send_Client_Text(WideStringClass(Message),TEXT_MESSAGE_PUBLIC,true,-1,-1,false,false);
	Text->Set_Dirty_Bit_For_Team(NetworkObjectClass::BIT_CREATION,Team);
}

void DA::Private_Admin_Message(cPlayer *Player,const char *Format,...) {
	char Message[256];
	Format_String(Message);
	cScTextObj *Text = Send_Client_Text(WideStringClass(Message),TEXT_MESSAGE_PUBLIC,true,-1,-1,false,false);
	Text->Set_Object_Dirty_Bits(Player->Get_Id(),NetworkObjectClass::BIT_CREATION);
}

void DA::Private_Admin_Message(int Player,const char *Format,...) {
	char Message[256];
	Format_String(Message);
	cScTextObj *Text = Send_Client_Text(WideStringClass(Message),TEXT_MESSAGE_PUBLIC,true,-1,-1,false,false);
	Text->Set_Object_Dirty_Bits(Player,NetworkObjectClass::BIT_CREATION);
}

void DA::Private_Admin_Message(GameObject *Player,const char *Format,...) {
	int ID = Get_Player_ID(Player);
	if (ID == -1) {
		return;
	}
	char Message[256];
	Format_String(Message);
	cScTextObj *Text = Send_Client_Text(WideStringClass(Message),TEXT_MESSAGE_PUBLIC,true,-1,-1,false,false);
	Text->Set_Object_Dirty_Bits(ID,NetworkObjectClass::BIT_CREATION);
}

void DA::Player_Message(cPlayer *Player,const char *Format,...) {
	char Message[256];
	Format_String(Message);
	Send_Client_Text(WideStringClass(Message),TEXT_MESSAGE_PUBLIC,false,Player->Get_Id(),-1,true,false);
}

void DA::Player_Message(int Player,const char *Format,...) {
	char Message[256];
	Format_String(Message);
	Send_Client_Text(WideStringClass(Message),TEXT_MESSAGE_PUBLIC,false,Player,-1,true,false);
}

void DA::Player_Message(GameObject *Player,const char *Format,...) {
	int ID = Get_Player_ID(Player);
	if (ID == -1) {
		return;
	}
	char Message[256];
	Format_String(Message);
	Send_Client_Text(WideStringClass(Message),TEXT_MESSAGE_PUBLIC,false,ID,-1,true,false);
}

void DA::Team_Player_Message(cPlayer *Player,const char *Format,...) {
	char Message[256];
	Format_String(Message);
	Send_Client_Text(WideStringClass(Message),TEXT_MESSAGE_TEAM,false,Player->Get_Id(),-1,true,false);
}

void DA::Team_Player_Message(int Player,const char *Format,...) {
	char Message[256];
	Format_String(Message);
	Send_Client_Text(WideStringClass(Message),TEXT_MESSAGE_TEAM,false,Player,-1,true,false);
}

void DA::Team_Player_Message(GameObject *Player,const char *Format,...) {
	int ID = Get_Player_ID(Player);
	if (ID == -1) {
		return;
	}
	char Message[256];
	Format_String(Message);
	Send_Client_Text(WideStringClass(Message),TEXT_MESSAGE_TEAM,false,ID,-1,true,false);
}

void DA::Page_Team(int Team,const char *Format,...) {
	char Message[256];
	Format_String(Message);
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		cPlayer *Player = z->Data();
		if (Player->Is_Active() && Player->Get_Player_Type() == Team) {
			Page_Player(Player->Get_Id(),"%s",Message);
		}
	}
}

void DA::Page_Team_Except(int Team,cPlayer *Except,const char *Format,...) {
	char Message[256];
	Format_String(Message);
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		cPlayer *Player = z->Data();
		if (Player->Is_Active() && Player->Get_Player_Type() == Team && Player != Except) {
			Page_Player(Player,"%s",Message);
		}
	}
}

void DA::Page_Player(cPlayer *Player,const char *Format,...) {
	int ID = Player->Get_Id();
	char Message[256];
	Format_String_Prefix(Message);
	if (!Player->Get_DA_Player()->Is_Scripts_Client()) {
		Send_Client_Text(WideStringClass(Message),TEXT_MESSAGE_PRIVATE,false,-1,ID,true,true);
	}
	else {
		Send_Message_Player_By_ID(ID,COLORLIGHTBLUE,StringFormat("Host: %s",Message));
		Create_2D_WAV_Sound_Player_By_ID(ID,"yo1.wav");
	}
}

void DA::Page_Player(int Player,const char *Format,...) {
	cPlayer *P = Find_Player(Player);
	if (P) {
		char Message[256];
		Format_String(Message);
		Page_Player(P,"%s",Message);
	}
}

void DA::Page_Player(GameObject *Player,const char *Format,...) {
	if (Is_Player(Player)) {
		cPlayer *P = ((SoldierGameObj*)Player)->Get_Player();
		char Message[256];
		Format_String(Message);
		Page_Player(P,"%s",Message);
	}
}

void DA::Color_Message(unsigned int Red,int unsigned Green,int unsigned Blue,const char *Format,...) {
	char Message[256];
	Format_String(Message);
	Send_Message(Red,Green,Blue,Message);
	cScTextObj *ChatEvent = 0;
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		cPlayer *Player = z->Data();
		if (Player->Is_Active() && !Player->Get_DA_Player()->Is_Scripts_Client()) {
			if (!ChatEvent) {
				ChatEvent = Send_Client_Text(L" ",TEXT_MESSAGE_PUBLIC,false,Setup_Send_Message_Fake(Message),-1,false,false);
			}
			ChatEvent->Set_Object_Dirty_Bits(Player->Get_Id(),NetworkObjectClass::BIT_CREATION);
		}
	}
	if (ChatEvent) {
		Update_Network_Object(ChatEvent);
		Restore_Send_Message_Fake();
	}
}

void DA::Color_Message_With_Team_Color(int Team,const char *Format,...) {
	char Message[256];
	Format_String(Message);
	if (Team != 0 && Team != 1) {
		Color_Message(COLORWHITE,"%s",Message);
	}
	else {
		unsigned int Red = 0,Green = 0,Blue = 0;
		Get_Team_Color(Team,&Red,&Green,&Blue);
		Send_Message(Red,Green,Blue,Message);
		cScTextObj *ChatEvent = 0;
		for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
			cPlayer *Player = z->Data();
			if (Player->Is_Active() && !Player->Get_DA_Player()->Is_Scripts_Client()) {
				if (!ChatEvent) {
					ChatEvent = Send_Client_Text(L" ",TEXT_MESSAGE_PUBLIC,false,0,-1,false,false);
					int Sender = Setup_Send_Message_Team_Fake(Message,Team);
					if (Sender) {
						ChatEvent->senderId = Sender;
						ChatEvent->type = TEXT_MESSAGE_TEAM;
					}
					else {
						ChatEvent->senderId = Setup_Send_Message_Fake(Message);
						ChatEvent->type = TEXT_MESSAGE_PUBLIC;
					}
				}
				ChatEvent->Set_Object_Dirty_Bits(Player->Get_Id(),NetworkObjectClass::BIT_CREATION);
			}
		}
		if (ChatEvent) {
			Update_Network_Object(ChatEvent);
			Restore_Send_Message_Fake();
		}
	}
}

void DA::Team_Color_Message(int Team,unsigned int Red,int unsigned Green,int unsigned Blue,const char *Format,...) {
	char Message[256];
	Format_String(Message);
	Send_Message_Team(Team,Red,Green,Blue,Message);
	cScTextObj *ChatEvent = 0;
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		cPlayer *Player = z->Data();
		if (Player->Get_Player_Type() == Team && Player->Is_Active() && !Player->Get_DA_Player()->Is_Scripts_Client()) {
			if (!ChatEvent) {
				ChatEvent = Send_Client_Text(L" ",TEXT_MESSAGE_PUBLIC,false,Setup_Send_Message_Fake(Message),-1,false,false);
			}
			ChatEvent->Set_Object_Dirty_Bits(Player->Get_Id(),NetworkObjectClass::BIT_CREATION);
		}
	}
	if (ChatEvent) {
		Update_Network_Object(ChatEvent);
		Restore_Send_Message_Fake();
	}
}

void DA::Team_Color_Message_With_Team_Color(int Team,const char *Format,...) {
	char Message[256];
	Format_String(Message);
	unsigned int Red = 0,Green = 0,Blue = 0;
	Get_Team_Color(Team,&Red,&Green,&Blue);
	cScTextObj *ChatEvent = 0;
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		cPlayer *Player = z->Data();
		if (Player->Get_Player_Type() == Team && Player->Is_Active()) {
			if (!Player->Get_DA_Player()->Is_Scripts_Client()) {
				if (!ChatEvent) {
					ChatEvent = Send_Client_Text(L" ",TEXT_MESSAGE_PUBLIC,false,0,-1,false,false);
					int Sender = Setup_Send_Message_Team_Fake(Message,Team);
					if (Sender) {
						ChatEvent->senderId = Sender;
						ChatEvent->type = TEXT_MESSAGE_TEAM;
					}
					else {
						ChatEvent->senderId = Setup_Send_Message_Fake(Message);
						ChatEvent->type = TEXT_MESSAGE_PUBLIC;
					}
				}
				ChatEvent->Set_Object_Dirty_Bits(Player->Get_Id(),NetworkObjectClass::BIT_CREATION);
			}
			else {
				Send_Message_Player_By_ID(Player->Get_Id(),Red,Green,Blue,Message);
			}
		}
	}
	if (ChatEvent) {
		Update_Network_Object(ChatEvent);
		Restore_Send_Message_Fake();
	}
}

void DA::Private_Color_Message(cPlayer *Player,unsigned int Red,unsigned int Green,unsigned int Blue,const char *Format,...) {
	int ID = Player->Get_Id();
	char Message[256];
	Format_String(Message);
	if (!Player->Get_DA_Player()->Is_Scripts_Client()) {
		cScTextObj *ChatEvent = Send_Client_Text(L" ",TEXT_MESSAGE_PUBLIC,false,Setup_Send_Message_Fake(Message,ID),-1,false,false);
		ChatEvent->Set_Object_Dirty_Bits(ID,NetworkObjectClass::BIT_CREATION);
		Update_Network_Object(ChatEvent);
		Restore_Send_Message_Fake();
	}
	else {
		Send_Message_Player_By_ID(ID,Red,Green,Blue,Message);
	}
}

void DA::Private_Color_Message(int Player,unsigned int Red,unsigned int Green,unsigned int Blue,const char *Format,...) {
	cPlayer *P = Find_Player(Player);
	if (P) {
		char Message[256];
		Format_String(Message);
		Private_Color_Message(P,Red,Green,Blue,"%s",Message);
	}
}

void DA::Private_Color_Message(GameObject *Player,unsigned int Red,unsigned int Green,unsigned int Blue,const char *Format,...) {
	if (Is_Player(Player)) {
		cPlayer *P = ((SoldierGameObj*)Player)->Get_Player();
		char Message[256];
		Format_String(Message);
		Private_Color_Message(P,Red,Green,Blue,"%s",Message);
	}
}

void DA::Private_Color_Message_With_Team_Color(cPlayer *Player,int Team,const char *Format,...) {
	int ID = Player->Get_Id();
	char Message[256];
	Format_String(Message);
	if (Team != 0 && Team != 1) {
		Private_Color_Message(ID,COLORWHITE,"%s",Message);
	}
	else {
		unsigned int Red = 0,Green = 0,Blue = 0;
		Get_Team_Color(Team,&Red,&Green,&Blue);
		if (!Player->Get_DA_Player()->Is_Scripts_Client()) {
			cScTextObj *ChatEvent = Send_Client_Text(L" ",TEXT_MESSAGE_PUBLIC,false,0,-1,false,false);
			int Sender = Setup_Send_Message_Team_Fake(Message,Team,ID);
			if (Sender) {
				ChatEvent->senderId = Sender;
				ChatEvent->type = TEXT_MESSAGE_TEAM;
			}
			else {
				ChatEvent->senderId = Setup_Send_Message_Fake(Message,ID);
				ChatEvent->type = TEXT_MESSAGE_PUBLIC;
			}
			ChatEvent->Set_Object_Dirty_Bits(ID,NetworkObjectClass::BIT_CREATION);
			Update_Network_Object(ChatEvent);
			Restore_Send_Message_Fake();
		}
		else {
			Send_Message_Player_By_ID(ID,Red,Green,Blue,Message);
		}
	}
}

void DA::Private_Color_Message_With_Team_Color(int Player,int Team,const char *Format,...) {
	cPlayer *P = Find_Player(Player);
	if (P) {
		char Message[256];
		Format_String(Message);
		Private_Color_Message_With_Team_Color(P,Team,"%s",Message);
	}
}

void DA::Private_Color_Message_With_Team_Color(GameObject *Player,int Team,const char *Format,...) {
	if (Is_Player(Player)) {
		cPlayer *P = ((SoldierGameObj*)Player)->Get_Player();
		char Message[256];
		Format_String(Message);
		Private_Color_Message_With_Team_Color(P,Team,"%s",Message);
	}
}

void DA::Create_2D_Sound(const char *Sound) {
	Commands->Create_2D_WAV_Sound(Sound);
	StringClass String(Sound);
	String.TruncateRight(8);
	Send_Announcement_Version_Less_Than(StringFormat("IDS_%s_TXT",String),2.6f);
}

void DA::Create_2D_Sound_Team(int Team,const char *Sound) {
	Create_2D_WAV_Sound_Team(Sound,Team);
	StringClass String(Sound);
	String.TruncateRight(8);
	Send_Announcement_Team_Version_Less_Than(Team,StringFormat("IDS_%s_TXT",String),2.6f);
}

void DA::Create_2D_Sound_Player(cPlayer *Player,const char *Sound) {
	Create_2D_WAV_Sound_Player_By_ID(Player->Get_Id(),Sound);
	StringClass String(Sound);
	String.TruncateRight(8);
	Send_Announcement_Player_Version_Less_Than(Player->Get_Id(),StringFormat("IDS_%s_TXT",String),2.6f);
}

void DA::Create_2D_Sound_Player(int Player,const char *Sound) {
	Create_2D_WAV_Sound_Player_By_ID(Player,Sound);
	StringClass String(Sound);
	String.TruncateRight(8);
	Send_Announcement_Player_Version_Less_Than(Player,StringFormat("IDS_%s_TXT",String),2.6f);
}

void DA::Create_2D_Sound_Player(GameObject *Player,const char *Sound) {
	int ID = Get_Player_ID(Player);
	if (ID != -1) {
		Create_2D_WAV_Sound_Player_By_ID(ID,Sound);
		StringClass String(Sound);
		String.TruncateRight(8);
		Send_Announcement_Player_Version_Less_Than(ID,StringFormat("IDS_%s_TXT",String),2.6f);
	}
}

void DA::HUD_Message(unsigned int Red, int unsigned Green, int unsigned Blue, const char *Format, ...) {
	char Message[256];
	Format_String(Message);
	WideStringClass Send;
	Send.Format(L"j\n90\n8269\n%hs\n%f\n%f\n%f\n,", Message, Red/255.0f, Green/255.0f, Blue/255.0f);
	Send_Client_Text(Send, TEXT_MESSAGE_PUBLIC, false, -2, -1, true, true);
}

void DA::Team_HUD_Message(int Team, unsigned int Red, int unsigned Green, int unsigned Blue, const char *Format, ...) {
	char Message[256];
	Format_String(Message);
	WideStringClass Send;
	Send.Format(L"j\n90\n8269\n%hs\n%f\n%f\n%f\n,", Message, Red/255.0f, Green/255.0f, Blue/255.0f);
	cScTextObj *Text = Send_Client_Text(Send, TEXT_MESSAGE_PUBLIC, false, -2, -1,false,false);
	Text->Set_Dirty_Bit_For_Team(NetworkObjectClass::BIT_CREATION, Team);
}

void DA::Private_HUD_Message(cPlayer *Player, unsigned int Red, unsigned int Green, unsigned int Blue, const char *Format, ...) {
	char Message[256];
	Format_String(Message);
	WideStringClass Send;
	Send.Format(L"j\n90\n8269\n%hs\n%f\n%f\n%f\n,", Message, Red/255.0f, Green/255.0f, Blue/255.0f);
	Send_Client_Text(Send, TEXT_MESSAGE_PRIVATE, false, -2, Player->Get_Id(), true,true);
}

void DA::Private_HUD_Message(int Player, unsigned int Red, unsigned int Green, unsigned int Blue, const char *Format, ...) {
	cPlayer *P = Find_Player(Player);
	if (P) {
		char Message[256];
		Format_String(Message);
		Private_HUD_Message(P, Red, Green, Blue, "%s", Message);
	}
}

void DA::Private_HUD_Message(GameObject *Player, unsigned int Red, unsigned int Green, unsigned int Blue, const char *Format, ...) {
	if (Is_Player(Player)) {
		cPlayer *P = ((SoldierGameObj*)Player)->Get_Player();
		char Message[256];
		Format_String(Message);
		Private_HUD_Message(P, Red, Green, Blue, "%s", Message);
	}
}



void DebugMsg(const char *Format,...) {
	char Buffer[526];
	va_list arg_list;
	va_start(arg_list,Format);
	_vsnprintf(Buffer,526,Format,arg_list);
	va_end(arg_list);
	FILE *file = fopen("Debug.txt","a");
	fprintf(file,"%s\n",Buffer);
	fclose(file);
}



class DAMsgConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "message"; }
	const char *Get_Alias() { return "msg"; }
	const char *Get_Help() { return "MESSAGE <message> - Sends a chat message to all players."; }
	void Activate(const char *ArgumentsString) {
		if (ArgumentsString && strlen(ArgumentsString)) {
			if (DAEventManager::Host_Chat_Event(-1,TEXT_MESSAGE_PUBLIC,ArgumentsString)) {
				Send_Client_Text(WideStringClass(ArgumentsString),TEXT_MESSAGE_PUBLIC,false,-1,-1,true,true);
				Commands->Create_2D_WAV_Sound("public_message.wav");
			}
		}
	}
};
Register_Console_Function(DAMsgConsoleFunctionClass);

class DATHMsgConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "teammessage"; }
	const char *Get_Alias() { return "thmsg"; }
	const char *Get_Help() { return "TEAMMESSAGE <team> <message> - Sends a chat message to all players on the specified team."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		int Team;
		if (Parser.Get_Int(Team) && Parser.Get_Remaining_String() && (Team == 0 || Team == 1)) {
			if (DAEventManager::Host_Chat_Event(Team,TEXT_MESSAGE_TEAM,Parser.Get_Remaining_String())) {
				cScTextObj *Text = Send_Client_Text(Parser.Get_Remaining_String(),TEXT_MESSAGE_PUBLIC,false,-1,-1,false,false);
				Set_Object_Dirty_Bit_For_Team_Version_Less_Than(Text,Team,2.6f,NetworkObjectClass::BIT_CREATION,true);
				Send_Message_Team_With_Team_Color(Team,StringFormat("Host: %s",Parser.Get_Remaining_String()));
				Create_2D_WAV_Sound_Team("public_message.wav",Team);
			}
		}
	}
};
Register_Console_Function(DATHMsgConsoleFunctionClass);

class DAAMsgConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "admin_message"; }
	const char *Get_Alias() { return "amsg"; }
	const char *Get_Help() { return "ADMIN_MESSAGE <message> - Sends a popup message to all players."; }
	void Activate(const char *ArgumentsString) {
		if (ArgumentsString && strlen(ArgumentsString)) {
			if (DAEventManager::Host_Chat_Event(-1,TEXT_MESSAGE_PUBLIC,ArgumentsString)) {
				Send_Client_Text(WideStringClass(ArgumentsString),TEXT_MESSAGE_PUBLIC,true,-1,-1,true,true);
			}
		}
	}
};
Register_Console_Function(DAAMsgConsoleFunctionClass);



class DAPPageConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char* Get_Name() { return "ppage"; }
	const char* Get_Help() { return "PPAGE <playerid> <message> - Sends a page to a specific player."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Text(ArgumentsString,' ');
		int ID = 0;
		Text.Get_Int(ID);
		cPlayer *Player = Find_Player(ID);
		if (Player && Text.Get_Remaining_String()) {
			if (!Player->Get_DA_Player()->Is_Scripts_Client()) {
				Send_Client_Text(Text.Get_Remaining_String(),TEXT_MESSAGE_PRIVATE,false,-1,ID,true,true);
			}
			else {
				Send_Message_Player_By_ID(ID,COLORLIGHTBLUE,StringFormat("Host: %s",Text.Get_Remaining_String()));
				Create_2D_WAV_Sound_Player_By_ID(ID,"yo1.wav");
			}
		}
	}
};
Register_Console_Function(DAPPageConsoleFunctionClass);

class DATPageConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char* Get_Name() { return "tpage"; }
	const char* Get_Help() { return "TPAGE <team> <message> - Sends a page to every player on the given team. Host only."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Text(ArgumentsString,' ');
		int Team = 0;
		if (Text.Get_Int(Team) && Text.Get_Remaining_String()) {
			for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
				cPlayer *Player = z->Data();
				if (Player->Is_Active() && Player->Get_Player_Type() == Team) {
					if (!Player->Get_DA_Player()->Is_Scripts_Client()) {
						Send_Client_Text(Text.Get_Remaining_String(),TEXT_MESSAGE_PRIVATE,false,-1,Player->Get_Id(),true,true);
					}
					else {
						DA::Private_Color_Message(Player,COLORLIGHTBLUE,StringFormat("Host: %s",Text.Get_Remaining_String()));
						Create_2D_WAV_Sound_Player_By_ID(Player->Get_Id(),"yo1.wav");
					}
				}
			}
		}
	}
};
Register_Console_Function(DATPageConsoleFunctionClass);



class DACMsgConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char* Get_Name() { return "cmsg"; }
	const char* Get_Help() { return "CMSG <red>,<green>,<blue> <message> - Displays a colored message in the info box of all players. Host only."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Text(ArgumentsString,' ');
		const char *Colors = Text.Get_String();
		const char *Message = Text.Get_Remaining_String();
		if (Colors && Message) {
			unsigned int Red,Green,Blue;
			DATokenParserClass ColorParser(Colors,',');
			if (ColorParser.Get_UInt(Red) && ColorParser.Get_UInt(Green) && ColorParser.Get_UInt(Blue)) {
				unsigned int TeamRed,TeamGreen,TeamBlue;
				Get_Team_Color(0,&TeamRed,&TeamGreen,&TeamBlue);
				if (Red == TeamRed && Green == TeamGreen && Blue == TeamBlue) {
					DA::Color_Message_With_Team_Color(0,"%s",Message);
				}
				else {
					Get_Team_Color(1,&TeamRed,&TeamGreen,&TeamBlue);
					if (Red == TeamRed && Green == TeamGreen && Blue == TeamBlue) {
						DA::Color_Message_With_Team_Color(1,"%s",Message);
					}
					else {
						DA::Color_Message(Red,Green,Blue,"%s",Message);
					}
				}
			}
		}
	}
};
Register_Console_Function(DACMsgConsoleFunctionClass);

class DACMsgTConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char* Get_Name() { return "cmsgt"; }
	const char* Get_Help() { return "CMSGT <team> <red>,<green>,<blue> <message> - Displays a colored message in the info box of a team. Host only."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Text(ArgumentsString,' ');
		int Team;
		Text.Get_Int(Team);
		const char *Colors = Text.Get_String();
		const char *Message = Text.Get_Remaining_String();
		if (Colors && Message) {
			unsigned int Red,Green,Blue;
			DATokenParserClass ColorParser(Colors,',');
			if (ColorParser.Get_UInt(Red) && ColorParser.Get_UInt(Green) && ColorParser.Get_UInt(Blue)) {
				unsigned int TeamRed,TeamGreen,TeamBlue;
				Get_Team_Color(Team,&TeamRed,&TeamGreen,&TeamBlue);
				if (Red == TeamRed && Green == TeamGreen && Blue == TeamBlue) {
					DA::Team_Color_Message_With_Team_Color(Team,"%s",Message);
				}
				else {
					DA::Team_Color_Message(Team,Red,Green,Blue,"%s",Message);
				}
			}
		}
	}
};
Register_Console_Function(DACMsgTConsoleFunctionClass);

class DACMsgPConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char* Get_Name() { return "cmsgp"; }
	const char* Get_Help() { return "CMSGP <player> <red>,<green>,<blue> <message> - Displays a colored message in the info box of a player. Host only."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Text(ArgumentsString,' ');
		int ID;
		Text.Get_Int(ID);
		const char *Colors = Text.Get_String();
		const char *Message = Text.Get_Remaining_String();
		if (Colors && Message) {
			unsigned int Red,Green,Blue;
			DATokenParserClass ColorParser(Colors,',');
			if (ColorParser.Get_UInt(Red) && ColorParser.Get_UInt(Green) && ColorParser.Get_UInt(Blue)) {
				unsigned int TeamRed,TeamGreen,TeamBlue;
				Get_Team_Color(0,&TeamRed,&TeamGreen,&TeamBlue);
				if (Red == TeamRed && Green == TeamGreen && Blue == TeamBlue) {
					DA::Private_Color_Message_With_Team_Color(ID,0,"%s",Message);
				}
				else {
					Get_Team_Color(1,&TeamRed,&TeamGreen,&TeamBlue);
					if (Red == TeamRed && Green == TeamGreen && Blue == TeamBlue) {
						DA::Private_Color_Message_With_Team_Color(ID,1,"%s",Message);
					}
					else {
						DA::Private_Color_Message(ID,Red,Green,Blue,"%s",Message);
					}
				}
			}
		}
	}
};
Register_Console_Function(DACMsgPConsoleFunctionClass);



class DAHUDMsgConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char* Get_Name() { return "hudmsg"; }
	const char* Get_Help() { return "HUDMSG <red>,<green>,<blue> <message> - Displays a colored message on the HUD of all players. Host only."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Text(ArgumentsString, ' ');
		const char *Colors = Text.Get_String();
		const char *Message = Text.Get_Remaining_String();
		if (Colors && Message) {
			unsigned int Red, Green, Blue;
			DATokenParserClass ColorParser(Colors, ',');
			if (ColorParser.Get_UInt(Red) && ColorParser.Get_UInt(Green) && ColorParser.Get_UInt(Blue)) {
				DA::HUD_Message(Red, Green, Blue, "%s", Message);
			}
		}
	}
};
Register_Console_Function(DAHUDMsgConsoleFunctionClass);

class DAHUDMsgTConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char* Get_Name() { return "hudmsgt"; }
	const char* Get_Help() { return "HUDMSGT <team> <red>,<green>,<blue> <message> - Displays a colored message on the HUD of a team. Host only."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Text(ArgumentsString, ' ');
		int Team;
		Text.Get_Int(Team);
		const char *Colors = Text.Get_String();
		const char *Message = Text.Get_Remaining_String();
		if (Colors && Message) {
			unsigned int Red, Green, Blue;
			DATokenParserClass ColorParser(Colors, ',');
			if (ColorParser.Get_UInt(Red) && ColorParser.Get_UInt(Green) && ColorParser.Get_UInt(Blue)) {
				DA::Team_HUD_Message(Team, Red, Green, Blue, "%s", Message);
			}
		}
	}
};
Register_Console_Function(DAHUDMsgTConsoleFunctionClass);

class DAHUDMsgPConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char* Get_Name() { return "hudmsgp"; }
	const char* Get_Help() { return "HUDMSGP <player> <red>,<green>,<blue> <message> - Displays a colored message on the HUD of a player. Host only."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Text(ArgumentsString, ' ');
		int ID;
		Text.Get_Int(ID);
		const char *Colors = Text.Get_String();
		const char *Message = Text.Get_Remaining_String();
		if (Colors && Message) {
			unsigned int Red, Green, Blue;
			DATokenParserClass ColorParser(Colors, ',');
			if (ColorParser.Get_UInt(Red) && ColorParser.Get_UInt(Green) && ColorParser.Get_UInt(Blue)) {
				DA::Private_HUD_Message(ID, Red, Green, Blue, "%s", Message);
			}
		}
	}
};
Register_Console_Function(DAHUDMsgPConsoleFunctionClass);
