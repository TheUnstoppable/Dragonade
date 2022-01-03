/*	Renegade Scripts.dll
    Dragonade Log Manager
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
#include "da_log.h"
#include "da_settings.h"
#include "engine_da.h"

//Based on code by danpaul88 from SSGM 4.0.

SOCKET DALogManager::Socket = 0;
DynamicVectorClass<DALogManager::Connection*> DALogManager::Connections;

void DALogManager::Init() {
	short Port = (short)DASettingsManager::Get_Int("LogPort",0);
	if (Port && !Socket) {
		WSADATA wsaData;
#pragma warning(suppress:6031)
		WSAStartup(MAKEWORD(1,1), &wsaData);
		Socket = (SOCKET)socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		sockaddr_in SockAddr;
		SockAddr.sin_family = AF_INET;
		SockAddr.sin_addr.s_addr = INADDR_ANY;
		SockAddr.sin_port = htons(Port);
		u_long i = 1;
		ioctlsocket(Socket,FIONBIO,&i);
		bind(Socket,(sockaddr *)&SockAddr,sizeof(SockAddr));
		listen(Socket,3);
		static DALogManager Instance;
		Instance.Register_Event(DAEvent::THINK);
		Instance.Register_Event(DAEvent::CONSOLEOUTPUT);
		Instance.Register_Event(DAEvent::RENLOG);
	}
}

void DALogManager::Shutdown() {
	for (int i = 0;i < Connections.Count();i++) {
		closesocket(Connections[i]->Socket);
	}
	if (Socket) {
		closesocket(Socket);
		Socket =  0;
	}
}

void DALogManager::Think() {
	SOCKET Client = accept(Socket,NULL,NULL);
	if (Client != INVALID_SOCKET) {
		Connection *c = new Connection;
		c->BufferFilled = 0;
		c->Socket = Client;
		c->Buffer[_countof(c->Buffer)-1] = '\0';
		Connections.Add(c);
	}
	for (int i = 0; i < Connections.Count(); ++i) {
		int Receive = recv(Connections[i]->Socket, Connections[i]->Buffer + Connections[i]->BufferFilled, _countof(Connections[i]->Buffer)-1 - Connections[i]->BufferFilled, 0);
		if (Receive == 0 || (Receive == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)) {
			closesocket(Connections[i]->Socket);
			delete Connections[i];
			Connections.Delete(i);
		}
		else if (Receive > 0) {
			char *LineStartPos = Connections[i]->Buffer;
			char *EndPos = Connections[i]->Buffer + Connections[i]->BufferFilled+Receive;
			for (;;) {
				char* LineEndPos = (char*)memchr(LineStartPos,'\n',EndPos-LineStartPos);
				if (!LineEndPos) break;
				*LineEndPos = '\0';
				if (LineEndPos-LineStartPos > 0 && *(LineEndPos-1) == '\r') *(LineEndPos-1) = '\0';
				Console_Input(LineStartPos);
				LineStartPos = LineEndPos+1;
			}
			Connections[i]->BufferFilled = EndPos-LineStartPos;
			memmove(Connections[i]->Buffer,LineStartPos,EndPos-LineStartPos);
		}
	}
}

void DALogManager::Write_Log(const char *Header,const char *Format,...) {
	char Buffer[526];
	char Time[64];
	memset(Time,0,sizeof(Time));
	Time[0] = 0x3F;
	GetTimeFormat(LOCALE_SYSTEM_DEFAULT,TIME_FORCE24HOURFORMAT,0,"'['HH':'mm':'ss']'",Time,0xFF);
	int Len = sprintf(Buffer,"000%s %s ",Time,Header);
	va_list arg_list;
	va_start(arg_list,Format);
	vsnprintf(Buffer+Len,525-Len,Format,arg_list);
	va_end(arg_list);
	DAEventManager::DA_Log_Event(Header,Buffer+Len);
	Send(Buffer);
	Buffer[2] = '1';
	Send(Buffer);
}

void DALogManager::Write_GameLog(const char *Format,...) {
	char Buffer[526];
	char Time[64];
	memset(Time,0,sizeof(Time));
	Time[0] = 0x3F;
	GetTimeFormat(LOCALE_SYSTEM_DEFAULT,TIME_FORCE24HOURFORMAT,0,"'['HH':'mm':'ss']'",Time,0xFF);
	int Len = sprintf(Buffer,"001%s ",Time);
	va_list arg_list;
	va_start(arg_list,Format);
	vsnprintf(Buffer+Len,525-Len,Format,arg_list);
	va_end(arg_list);
	Send(Buffer);
}

void DALogManager::Ren_Log_Event(const char *Output) {
	char Buffer[526];
	char Time[64];
	memset(Time,0,sizeof(Time));
	Time[0] = 0x3F;
	GetTimeFormat(LOCALE_SYSTEM_DEFAULT,TIME_FORCE24HOURFORMAT,0,"'['HH':'mm':'ss']'",Time,0xFF);
	sprintf(Buffer,"002%s %s",Time,Output);
	Send(Buffer);
}

void DALogManager::Console_Output_Event(const char *Output) {
	char Buffer[526];
	sprintf(Buffer,"003%s",Output);
	Send(Buffer);
}

void DALogManager::Send(const char *Data) {
	for (int i = 0;i < Connections.Count();i++) {
		send(Connections[i]->Socket,Data,strlen(Data)+1,0);
	}
}