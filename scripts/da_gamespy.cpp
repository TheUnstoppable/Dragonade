/*	Renegade Scripts.dll
    Dragonade GameSpy Q&R
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
#include "engine_DA.h"
#include "da.h"
#include "da_gamespy.h"
#include "da_game.h"
#include "da_settings.h"
#include "da_translation.h"
#include "da_log.h"
#include "SysTimeClass.h"
#include "HashTemplateClass.h"
#include "GameDefinition.h"
#include "GameObjManager.h"

void DAGameSpyGameFeatureClass::Init() {
	StringClass IP = Long_To_IP(The_Game()->Get_Ip_Address());
	Port = (unsigned short)DASettingsManager::Get_Int("GameSpyQueryPort",DASettingsClass("server.ini").Get_INI()->Get_Int("Server","GameSpyQueryPort",25300));
	QueryID = 0;
	
	hostent *HostInfo = 0;
	sockaddr_in MasterAddress;
	unsigned short MasterPort = htons(27900);

	StringClass Masters;								 
	DASettingsManager::Get_String(Masters,"GameSpyMasterServers","renmaster.cncirc.net|renmaster-backup.cncirc.net|renmaster-backup2.cncirc.net");
	DATokenParserClass Parser(Masters,'|');
	while (char *Token = Parser.Get_String()) {
		HostInfo = gethostbyname(Token);
		if (HostInfo) {
			MasterAddress.sin_family = HostInfo->h_addrtype;
			memcpy((char*)&MasterAddress.sin_addr.s_addr,HostInfo->h_addr_list[0],HostInfo->h_length);
			MasterAddress.sin_port = MasterPort;
			MasterServers.Add(MasterAddress);
		}
	}

	if (!MasterServers.Count()) {
		const char *Error = "No valid GameSpy Master Servers found.";
		Console_Output("%s\n",Error);
		DALogManager::Write_Log("_ERROR","%s",Error);
	}

	ListenSocket = socket(AF_INET, SOCK_DGRAM,IPPROTO_IP);
	int vals = 0;
	setsockopt(ListenSocket,IPPROTO_IP,IP_DONTFRAGMENT,(const char*)&vals,sizeof(vals));
	u_long on = 1;
	ioctlsocket(ListenSocket, FIONBIO, &on);
	HostInfo = gethostbyname(IP);
	if (HostInfo) {
		sockaddr_in ServerAddress;
		ServerAddress.sin_family = AF_INET;
		memcpy((char*)&ServerAddress.sin_addr.s_addr,HostInfo->h_addr_list[0],HostInfo->h_length);
		ServerAddress.sin_port = htons(Port);
		bind(ListenSocket,(sockaddr *)&ServerAddress,sizeof(sockaddr_in));
		listen(ListenSocket,5);
		Register_Event(DAEvent::THINK);
		Register_Event(DAEvent::SETTINGSLOADED);
		Register_Event(DAEvent::LEVELLOADED);
		Register_Event(DAEvent::PLAYERJOIN);
		Register_Event(DAEvent::PLAYERLEAVE);
		Level_Loaded_Event();
		Timer_Expired(1,0);
		Start_Timer(1,60.0f,true);
	}
}

void DAGameSpyGameFeatureClass::Settings_Loaded_Event() {
	CustomRules.Delete_All();
	INISection *Section = DASettingsManager::Get_Section("GameSpyRules");
	if (Section) {
		for (INIEntry *i = Section->EntryList.First();i && i->Is_Valid();i = i->Next()) {
			CustomRules.Add(StringFormat("%s\\%s\\",i->Entry,i->Value));
		}
	}
}

void DAGameSpyGameFeatureClass::Level_Loaded_Event() {
	ShowPlayerCount = DASettingsManager::Get_Bool("GameSpyShowPlayerCount",false);
	Title = The_Game()->Get_Game_Title();
	if (ShowPlayerCount) {
		The_Game()->Get_Game_Title().Format(L"%s (%d/%d)",Title,The_Game()->Get_Current_Players(),The_Game()->Get_Max_Players());
	}
}

void DAGameSpyGameFeatureClass::Think() {
	sockaddr_in ClientAddress;
	static int SockSize = sizeof(sockaddr_in);
	char Buffer[256];
	memset(Buffer,0,256);
	int retrecv = recvfrom(ListenSocket,Buffer,255,0,(sockaddr*)&ClientAddress,&SockSize);
	if (retrecv > 0) {
		QueryID++;
		if (!strcmp(Buffer,"\\status\\")) {
			HashTemplateClass<StringClass,GameDefinition> Definitions;
			Get_Game_Definitions(Definitions);
			StringClass NextMap = Get_Map(Get_Current_Map_Index()+(The_Game()->Is_Intermission()?0:1));
			if (NextMap.Is_Empty()) {
				NextMap = Get_Map(0);
			}
			NextMap.ToUpper();
			StringClass Send;

			//Basic & Info
			Send.Format("\\gamename\\ccrenegade\\gamever\\838\\hostname\\%ls\\hostport\\%d\\mapname\\%s\\nextmap\\%s.mix\\gametype\\%s\\numplayers\\%d\\maxplayers\\%d\\timeleft\\%s\\timeelapsed\\%s\\timelimit\\%s\\",
				Title,
				The_Game()->Get_Port(),
				DAGameManager::Get_Map(),
				Definitions.Get(NextMap)->mapName,
				DAGameManager::Get_Game_Mode_Long_Name(),
				The_Game()->Get_Current_Players(),
				The_Game()->Get_Max_Players(),
				Format_Time((unsigned long)The_Game()->Get_Time_Remaining_Seconds()),
				Format_Time(The_Game()->Get_Game_Duration_S()),
				Format_Time(The_Game()->Get_Time_Limit_Minutes() * 60));
			
			//Custom rules
			for (int i = 0;i < CustomRules.Count();i++) {
				Send += CustomRules[i];
			}

			//Rules
			Send += StringFormat("SVER\\Dragonade/%s TT/%.1f-r%u\\CSVR\\1\\DED\\1\\DG\\%d\\password\\%d\\TC\\%d\\FF\\%d\\SC\\%d\\BDEG\\%d\\BPEG\\%d\\",
				DA::Get_Version(), GetTTVersion(), GetTTRevision(),
				The_Game()->Driver_Is_Always_Gunner() ? 1 : 0,
				The_Game()->Is_Passworded() ? 1 : 0,
				The_Game()->Is_Team_Changing_Allowed() || DAGameManager::Find_Game_Feature("Request Team Change") ? 1 : 0,
				The_Game()->Is_Friendly_Fire_Permitted() ? 1 : 0,
				The_Cnc_Game()->Get_Starting_Credits(),
				The_Cnc_Game()->Base_Destruction_Ends_Game() ? 1 : 0,
				The_Cnc_Game()->Beacon_Placement_Ends_Game() ? 1 : 0);

			//Game features
			const DynamicVectorClass<DAGameFeatureFactoryClass*> &GameFeatures = DAGameManager::Get_Game_Features();
			for (int i = 0;i < GameFeatures.Count();i++) {
				if (GameFeatures[i]->Get_Instance() && GameFeatures[i]->Get_Instance() != this && GameFeatures[i]->Get_Name()) {
					Send += GameFeatures[i]->Get_Name();
					Send += "\\1\\";
				}
			}

			//Basic
			Send += StringFormat("queryid\\%u.1",
				QueryID
			);

			//Send what we have so far.
			sendto(ListenSocket,Send,Send.Get_Length()+1,0,(sockaddr*)&ClientAddress,sizeof(sockaddr_in));

			Send = "";
			int SendCount = 2;

			//Nod team
			cTeam *Team = Find_Team(0);
			Send += StringFormat(
				"\\team_t0\\%ls\\score_t0\\%d\\kills_t0\\%d\\deaths_t0\\%d",
				Get_Wide_Team_Name(0),(int)Team->Get_Score(),Team->Get_Kills(),Team->Get_Deaths()
			);
	
			//GDI team
			Team = Find_Team(1);
			Send += StringFormat(
				"\\team_t1\\%ls\\score_t1\\%d\\kills_t1\\%d\\deaths_t1\\%d",
				Get_Wide_Team_Name(1),(int)Team->Get_Score(),Team->Get_Kills(),Team->Get_Deaths()
			);

			//Players
			for (SLNode<cPlayer>* z = Get_Player_List()->Head();z;z = z->Next()) {
				cPlayer *Player = z->Data();
				if (Send.Get_Length() > 1000) {
					Send += StringFormat("\\queryid\\%u.%d",QueryID,SendCount);
					sendto(ListenSocket,Send,Send.Get_Length()+1,0,(sockaddr *)&ClientAddress,sizeof(sockaddr_in));
					Send = "";
					SendCount++;
				}
				if (Player->Is_Active()) {
					Send += StringFormat(
						"\\player_%d\\%ls\\score_%d\\%d\\kills_%d\\%d\\deaths_%d\\%d\\time_%d\\%s\\ping_%d\\%d\\team_%d\\%d",
						Player->Get_Id(),Player->Get_Name(),Player->Get_Id(),(int)Player->Get_Score(),Player->Get_Id(),Player->Get_Kills(),Player->Get_Id(),Player->Get_Deaths(),Player->Get_Id(),Format_Time((TIMEGETTIME()-Player->JoinTime)/1000),Player->Get_Id(),Get_Ping(Player->Get_Id()),Player->Get_Id(),Player->Get_Player_Type()
					);
				}
			}
			if (!Send.Is_Empty()) {
				Send += StringFormat("\\queryid\\%u.%d",QueryID,SendCount);
				sendto(ListenSocket,Send,Send.Get_Length()+1,0,(sockaddr*)&ClientAddress,sizeof(sockaddr_in));
				SendCount++;
			}
			Send.Format("\\final\\\\queryid\\%u.%d",QueryID,SendCount);
			sendto(ListenSocket,Send,Send.Get_Length()+1,0,(sockaddr*)&ClientAddress,sizeof(sockaddr_in));
		}
		else if (!strcmp(Buffer, "\\info\\")) {
			StringClass Send;
			Send.Format(
				"\\hostname\\%ls\\hostport\\%d\\mapname\\%s\\gametype\\%s\\numplayers\\%i\\maxplayers\\%i\\final\\\\queryid\\%u.1",
				Title, The_Game()->Get_Port(), The_Game()->Get_Map_Name(), DAGameManager::Get_Game_Mode_Long_Name(), The_Game()->Get_Current_Players(), The_Game()->Get_Max_Players(), QueryID);
			sendto(ListenSocket, Send, Send.Get_Length() + 1, 0, (sockaddr*)&ClientAddress, sizeof(sockaddr_in));
		}
		else if (strstr(Buffer,"\\echo\\")) {
			StringClass Send;
			Send.Format("%s\\final\\\\queryid\\%u.1",Buffer,QueryID);
			sendto(ListenSocket,Send,Send.Get_Length()+ 1,0,(sockaddr*)&ClientAddress,sizeof(sockaddr_in));
		}
	}
}

void DAGameSpyGameFeatureClass::Timer_Expired(int Number,unsigned int Data) {
	StringClass Send;
	Send.Format("\\heartbeat\\%u\\gamename\\ccrenegade",Port);
	for (int i = 0;i < MasterServers.Count();i++) {
		sendto(ListenSocket,Send,Send.Get_Length()+1,0,(sockaddr*)&MasterServers[i],sizeof(sockaddr_in));
	}
}

void DAGameSpyGameFeatureClass::Player_Join_Event(cPlayer *Player) {
	if (ShowPlayerCount) {
		The_Game()->Get_Game_Title().Format(L"%s (%d/%d)",Title,The_Game()->Get_Current_Players(),The_Game()->Get_Max_Players());
		Update_Game_Options(-1); // Update game options as server title has changed.
	}
}

void DAGameSpyGameFeatureClass::Player_Leave_Event(cPlayer *Player) {
	if (ShowPlayerCount) {
		The_Game()->Get_Game_Title().Format(L"%s (%d/%d)",Title,The_Game()->Get_Current_Players()-1,The_Game()->Get_Max_Players());
		Update_Game_Options(-1); // Update game options as server title has changed.
	}
}

DAGameSpyGameFeatureClass::~DAGameSpyGameFeatureClass() { 
	StringClass Send;
	Send.Format("\\heartbeat\\%u\\gamename\\ccrenegade\\statechanged\\2",Port);
	for (int i = 0;i < MasterServers.Count();i++) {
		sendto(ListenSocket,Send,Send.Get_Length()+1,0,(sockaddr*)&MasterServers[i],sizeof(sockaddr_in));
	}
	closesocket(ListenSocket);
}

Register_Game_Feature(DAGameSpyGameFeatureClass,"GameSpy Q&R","EnableGameSpy",0);