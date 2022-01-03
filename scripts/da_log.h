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

#ifndef INCLUDE_DALOG
#define INCLUDE_DALOG

#include "da_event.h"
#include "winsock2.h"

class DALogManager : public DAEventClass {
public:
	static void Init();
	static void Shutdown();
	DA_API static void Write_Log(const char *Header,const char *Format,...);
	DA_API static void Write_GameLog(const char *Format,...);
	DA_API static void Send(const char *Data);
	
private:
	virtual void Think();
	virtual void Ren_Log_Event(const char *Output);
	virtual void Console_Output_Event(const char *Output);
	struct Connection {
		SOCKET Socket;
		char Buffer[512];
		int BufferFilled;
	};
	static SOCKET Socket;
	static DynamicVectorClass<Connection*> Connections;
};

#endif