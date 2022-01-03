/*	Renegade Scripts.dll
    Dragonade Ban Manager
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DABAN
#define INCLUDE_DABAN

#include "da_event.h"

class DA_API DABanEntryClass {
public:
	DABanEntryClass(const char *Name,const char *IP,const char *Serial,const char *Reason);
	bool Matches_Name(const char *Name);
	bool Matches_IP(const char *IP);
	bool Matches_Serial(const char *Serial);
	const StringClass &Get_Name();
	const StringClass &Get_IP();
	const StringClass &Get_Serial();
	const StringClass &Get_Reason();

private:
	StringClass Name;
	StringClass IP;
	StringClass Serial;
	StringClass Reason;
};

class DA_API DABanListClass {
public:
	DABanListClass(const char *Name,const char *FileName);
	~DABanListClass();
	void Load();
	void Save();
	void Save(DABanEntryClass *Entry);
	DABanEntryClass *Add_Entry(const char *Name,const char *IP,const char *Serial,const char *Reason);
	bool Remove_Entry(const char *Name,const char *IP,const char *Serial);
	bool Remove_Entry_Match_All(const char *Name,const char *IP,const char *Serial);
	DABanEntryClass *Find_Entry(const char *Name,const char *IP,const char *Serial);
	DABanEntryClass *Find_Entry_Match_All(const char *Name,const char *IP,const char *Serial);
	void List_Entries();
	void List_Entries(const char *Name,const char *IP,const char *Serial);
	void List_Entries_Match_All(const char *Name,const char *IP,const char *Serial);
	int Get_Count();
	DABanEntryClass *Peek_Entry(int Position);
	const StringClass &Get_Name();

private:
	void Internal_List_Entry(int Position);
	void Internal_Remove_Entry(int Position);

	StringClass Name;
	StringClass FileName;
	unsigned int LastModTime;
	DynamicVectorClass<DABanEntryClass*> List;
};

class DABanManager : public DAEventClass {
public:
	static void Init();
	static void Shutdown();
	DA_API static DABanListClass *Get_Ban_List();
	DA_API static DABanListClass *Get_Ban_Exception_List();
	DA_API static DABanListClass *Get_ForceTT_List();
	DA_API static DABanListClass *Get_ForceTT_Exception_List();

private:
	virtual ConnectionAcceptanceFilter::STATUS Connection_Request_Event(ConnectionRequest &Request,WideStringClass &RefusalMessage);
	static DABanListClass *BanList;
	static DABanListClass *BanExceptionList;
	static DABanListClass *ForceTTList;
	static DABanListClass *ForceTTExceptionList;
};

#endif
