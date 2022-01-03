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

#include "general.h"
#include "engine.h"
#include "engine_da.h"
#include "da.h"
#include "da_ban.h"
#include "da_log.h"
#include "da_player.h"
#include "RawFileClass.h"

DABanListClass *DABanManager::BanList;
DABanListClass *DABanManager::BanExceptionList;
DABanListClass *DABanManager::ForceTTList;
DABanListClass *DABanManager::ForceTTExceptionList;

/********************************************************************************************************************************/

DABanEntryClass::DABanEntryClass(const char *Name,const char *IP,const char *Serial,const char *Reason) {
	this->Name = Name;
	this->IP = IP;
	this->Serial = Serial;
	this->Reason = Reason;
}

bool DABanEntryClass::Matches_Name(const char *Name) {
	return (this->Name == Name);
}

bool DABanEntryClass::Matches_IP(const char *IP) {
	if (!IP) {
		return false;
	}
	const char *BanIP = this->IP.Peek_Buffer();
	for (;;) {
		if (*IP == '\0') {
			return (*BanIP == '\0' || *BanIP == '*');
		}
		else if (*BanIP == '\0') {
			return (*IP == '\0');
		}
		else if (*BanIP == '*') {
			return true;
		}
		else if (*IP != *BanIP) {
			return false;
		}
		IP++;
		BanIP++;
	}
}

bool DABanEntryClass::Matches_Serial(const char *Serial) {
	return (this->Serial == Serial);
}

const StringClass &DABanEntryClass::Get_Name() {
	return Name;
}

const StringClass &DABanEntryClass::Get_IP() {
	return IP;
}

const StringClass &DABanEntryClass::Get_Serial() {
	return Serial;
}

const StringClass &DABanEntryClass::Get_Reason() {
	return Reason;
}

/********************************************************************************************************************************/

DABanListClass::DABanListClass(const char *Name,const char *FileName) {
	this->Name = Name;
	this->FileName.Format("%s%s",Get_File_Path(),FileName);
	this->LastModTime = 0;
	Load();
}

DABanListClass::~DABanListClass() {
	for (int i = 0;i < List.Count();i++) {
		delete List[i];
	}
	List.Delete_All();
}

void DABanListClass::Load() {
	TextFileClass File;
	if (File.Open(FileName,1) && File.Get_Date_Time() != LastModTime) {
		for (int i = 0;i < List.Count();i++) {
			delete List[i];
		}
		List.Delete_All();
		LastModTime = File.Get_Date_Time();
		StringClass Buffer;
		while (File.Read_Line(Buffer)) {
			DATokenParserClass Parser(Buffer,'\t');
			const char *Name = Parser.Get_String();
			const char *IP = Parser.Get_String();
			const char *Serial = Parser.Get_String();
			if (Name && IP && Serial) {
				List.Add(new DABanEntryClass(Name,IP,Serial,Parser.Get_String()));
			}
		}
	}
}

void DABanListClass::Save() {
	TextFileClass File;
	if (File.Open(FileName,2)) {
		StringClass Buffer;
		for (int i = 0;i < List.Count();i++) {
			Buffer.Format("%s\t%s\t%s\t%s",List[i]->Get_Name(),List[i]->Get_IP(),List[i]->Get_Serial(),List[i]->Get_Reason());
			File.Write_Line(Buffer);
		}
		LastModTime = File.Get_Date_Time();
	}
}

void DABanListClass::Save(DABanEntryClass *Entry) {
	TextFileClass File;
	if (File.Open(FileName,3)) {
		File.Seek(File.Size(),0);
		StringClass Buffer;
		Buffer.Format("%s\t%s\t%s\t%s",Entry->Get_Name(),Entry->Get_IP(),Entry->Get_Serial(),Entry->Get_Reason());
		File.Write_Line(Buffer);
		File.Close();
		File.Open(1);
		LastModTime = File.Get_Date_Time();
	}
}

DABanEntryClass *DABanListClass::Add_Entry(const char *Name,const char *IP,const char *Serial,const char *Reason) {
	DABanEntryClass *Entry = new DABanEntryClass(Name,IP,Serial,Reason);
	List.Add(Entry);
	Save(Entry);
	StringClass String;
	String.Format("Added %s %s(%s)(%s)(%s)",this->Name,Name,IP,Serial,Reason);
	Console_Output("%s\n",String);
	DALogManager::Write_Log("_BAN","%s",String);
	return Entry;
}

void DABanListClass::Internal_Remove_Entry(int Position) {
	StringClass String;
	String.Format("Removed %s %s(%s)(%s)(%s)",this->Name,List[Position]->Get_Name(),List[Position]->Get_IP(),List[Position]->Get_Serial(),List[Position]->Get_Reason());
	Console_Output("%s\n",String);
	DALogManager::Write_Log("_BAN","%s",String);
	delete List[Position];
	List.Delete(Position);
}

bool DABanListClass::Remove_Entry(const char *Name,const char *IP,const char *Serial) {
	Load();
	bool Removed = false;
	for (int i = List.Count()-1;i >= 0;i--) {
		if (List[i]->Matches_Name(Name) || List[i]->Matches_IP(IP) || List[i]->Matches_Serial(Serial)) {
			Internal_Remove_Entry(i);
			Removed = true;
		}
	}
	if (Removed) {
		Save();
	}
	return Removed;
}

bool DABanListClass::Remove_Entry_Match_All(const char *Name,const char *IP,const char *Serial) {
	Load();
	bool Removed = false;
	for (int i = List.Count()-1;i >= 0;i--) {
		if (List[i]->Matches_Name(Name) && List[i]->Matches_IP(IP) && List[i]->Matches_Serial(Serial)) {
			Internal_Remove_Entry(i);
			Removed = true;
		}
	}
	if (Removed) {
		Save();
	}
	return Removed;
}

DABanEntryClass *DABanListClass::Find_Entry(const char *Name,const char *IP,const char *Serial) {
	Load();
	for (int i = 0;i < List.Count();i++) {
		if (List[i]->Matches_Name(Name) || List[i]->Matches_IP(IP) || List[i]->Matches_Serial(Serial)) {
			return List[i];
		}
	}
	return 0;
}

DABanEntryClass *DABanListClass::Find_Entry_Match_All(const char *Name,const char *IP,const char *Serial) {
	Load();
	for (int i = 0;i < List.Count();i++) {
		if (List[i]->Matches_Name(Name) && List[i]->Matches_IP(IP) && List[i]->Matches_Serial(Serial)) {
			return List[i];
		}
	}
	return 0;
}

void DABanListClass::Internal_List_Entry(int Position) {
	StringClass Reason;
	if (!List[Position]->Get_Reason().Is_Empty()) {
		Reason.Format("- %s",List[Position]->Get_Reason());;
	}
	Console_Output("%s %s %s %s\n",List[Position]->Get_Name(),List[Position]->Get_IP(),List[Position]->Get_Serial(),Reason);
	DALogManager::Write_Log("_BAN","%s %s %s %s",List[Position]->Get_Name(),List[Position]->Get_IP(),List[Position]->Get_Serial(),Reason);
}

void DABanListClass::List_Entries() {
	Load();
	StringClass String;
	String.Format("%s List:",Get_Name());
	Console_Output("%s\n",String);
	DALogManager::Write_Log("_BAN","%s",String);
	for (int i = 0;i < List.Count();i++) {
		Internal_List_Entry(i);
	}
}

void DABanListClass::List_Entries(const char *Name,const char *IP,const char *Serial) {
	Load();
	StringClass String;
	String.Format("%s List:",Get_Name());
	Console_Output("%s\n",String);
	DALogManager::Write_Log("_BAN","%s",String);
	for (int i = 0;i < List.Count();i++) {
		if (List[i]->Matches_Name(Name) || List[i]->Matches_IP(IP) || List[i]->Matches_Serial(Serial)) {
			Internal_List_Entry(i);
		}
	}
}

void DABanListClass::List_Entries_Match_All(const char *Name,const char *IP,const char *Serial) {
	Load();
	StringClass String;
	String.Format("%s List:",Get_Name());
	Console_Output("%s\n",String);
	DALogManager::Write_Log("_BAN","%s",String);
	for (int i = 0;i < List.Count();i++) {
		if (List[i]->Matches_Name(Name) && List[i]->Matches_IP(IP) && List[i]->Matches_Serial(Serial)) {
			Internal_List_Entry(i);
		}
	}
}

int DABanListClass::Get_Count() {
	return List.Count();
}

DABanEntryClass *DABanListClass::Peek_Entry(int Position) {
	return List[Position];
}

const StringClass &DABanListClass::Get_Name() {
	return Name;
}

/********************************************************************************************************************************/

class KickConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "kick"; }
	const char *Get_Help() { return "KICK <playerid> [reason] - Kicks the given player from the server."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		int ID;
		Parser.Get_Int(ID);
		if (Find_Player(ID)) {
			if (Parser.Get_Remaining_String()) {
				Evict_Client(ID,WideStringFormat(L"You have been kicked from the server: %hs",Parser.Get_Remaining_String()));
			}
			else {
				Evict_Client(ID,"You have been kicked from the server.");
			}
		}
	}
};
Register_Console_Function(KickConsoleFunctionClass);

class BanConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "ban"; }
	const char *Get_Help() { return "BAN <playerid> [reason] - Kicks the given player from the server and bans their nick, IP, and serial."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		int ID;
		Parser.Get_Int(ID);
		cPlayer *Player = Find_Player(ID);
		if (Player) {
			StringClass Serial = Player->Get_DA_Player()->Get_Serial();
			if (Serial == "00000000000000000000000000000000") { //Don't ban the blank serial.
				Serial = "0";
			}
			DABanManager::Get_Ban_List()->Add_Entry(StringClass(Player->Get_Name()),Get_IP_Address_String(ID),Serial,Parser.Get_Remaining_String());
			if (Parser.Get_Remaining_String()) {
				Evict_Client(ID,WideStringFormat(L"You have been banned from the server: %hs",Parser.Get_Remaining_String()));
			}
			else {
				Evict_Client(ID,"You have been banned from the server.");
			}
		}
	}
};
Register_Console_Function(BanConsoleFunctionClass);

class AddBanConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "addban"; }
	const char *Get_Help() { return "ADDBAN <name> <IP> <serial> [reason] - Adds a ban entry."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		const char *Name = Parser.Get_String();
		const char *IP = Parser.Get_String();
		const char *Serial = Parser.Get_String();
		if (Name && IP && Serial) {
			DABanManager::Get_Ban_List()->Add_Entry(Name,IP,Serial,Parser.Get_Remaining_String());
		}
	}
};
Register_Console_Function(AddBanConsoleFunctionClass);

class RemoveBanConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "removeban"; }
	const char *Get_Help() { return "REMOVEBAN <name/IP/serial> / REMOVEBAN <name> <ip> <serial> - Removes any ban that matches the specified name, IP, OR serial, or name, IP, AND serial."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		const char *Name = Parser.Get_String();
		const char *IP = Parser.Get_String();
		const char *Serial = Parser.Get_String();
		if (Name) {
			if (IP && Serial) {
				DABanManager::Get_Ban_List()->Remove_Entry_Match_All(Name,IP,Serial);
			}
			else {
				DABanManager::Get_Ban_List()->Remove_Entry(Name,Name,Name);
			}
		}
	}
};
Register_Console_Function(RemoveBanConsoleFunctionClass);

class ListBanConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "listban"; }
	const char *Get_Help() { return "LISTBAN / LISTBAN <name/IP/serial> / LISTBAN <name> <IP> <serial> - Lists all bans, or any ban that matches the specified name, IP, OR serial, or name, IP, AND serial."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		const char *Name = Parser.Get_String();
		const char *IP = Parser.Get_String();
		const char *Serial = Parser.Get_String();
		if (Name) {
			if (IP && Serial) {
				DABanManager::Get_Ban_List()->List_Entries_Match_All(Name,IP,Serial);
			}
			else {
				DABanManager::Get_Ban_List()->List_Entries(Name,Name,Name);
			}
		}
		else {
			DABanManager::Get_Ban_List()->List_Entries();
		}
	}
};
Register_Console_Function(ListBanConsoleFunctionClass);

class AddBanExceptionConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "addbanexception"; }
	const char *Get_Help() { return "ADDBANEXCEPTION <name> <IP> <serial> [reason] - Adds a ban exception."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		const char *Name = Parser.Get_String();
		const char *IP = Parser.Get_String();
		const char *Serial = Parser.Get_String();
		if (Name && IP && Serial) {
			DABanManager::Get_Ban_Exception_List()->Add_Entry(Name,IP,Serial,Parser.Get_Remaining_String());
		}
	}
};
Register_Console_Function(AddBanExceptionConsoleFunctionClass);

class RemoveBanExceptionConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "removebanexception"; }
	const char *Get_Help() { return "REMOVEBANEXCEPTION <name/IP/serial> / REMOVEBANEXCEPTION <name> <ip> <serial> - Removes any ban exception that matches the specified name, IP, OR serial, or name, IP, AND serial."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		const char *Name = Parser.Get_String();
		const char *IP = Parser.Get_String();
		const char *Serial = Parser.Get_String();
		if (Name) {
			if (IP && Serial) {
				DABanManager::Get_Ban_Exception_List()->Remove_Entry_Match_All(Name,IP,Serial);
			}
			else {
				DABanManager::Get_Ban_Exception_List()->Remove_Entry(Name,Name,Name);
			}
		}
	}
};
Register_Console_Function(RemoveBanExceptionConsoleFunctionClass);

class ListBanExceptionConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "listbanexception"; }
	const char *Get_Help() { return "LISTBANEXCEPTION / LISTBANEXCEPTION <name/IP/serial> / LISTBANEXCEPTION <name> <IP> <serial> - Lists all ban exceptions, or any ban exception that matches the specified name, IP, OR serial, or name, IP, AND serial."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		const char *Name = Parser.Get_String();
		const char *IP = Parser.Get_String();
		const char *Serial = Parser.Get_String();
		if (Name) {
			if (IP && Serial) {
				DABanManager::Get_Ban_Exception_List()->List_Entries_Match_All(Name,IP,Serial);
			}
			else {
				DABanManager::Get_Ban_Exception_List()->List_Entries(Name,Name,Name);
			}
		}
		else {
			DABanManager::Get_Ban_Exception_List()->List_Entries();
		}
	}
};
Register_Console_Function(ListBanExceptionConsoleFunctionClass);

class ForceTTConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "forceTT"; }
	const char *Get_Help() { return "FORCETT <playerid> [reason] - Forces a player to run TT."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		int ID;
		Parser.Get_Int(ID);
		cPlayer *Player = Find_Player(ID);
		if (Player) {
			StringClass Serial = Player->Get_DA_Player()->Get_Serial();
			if (Serial == "00000000000000000000000000000000") { //Don't forceTT the blank serial.
				Serial = "0";
			}
			DABanManager::Get_ForceTT_List()->Add_Entry(StringClass(Player->Get_Name()),Get_IP_Address_String(ID),Serial,Parser.Get_Remaining_String());
			if (!Player->Get_DA_Player()->Is_TT_Client()) {
				if (Parser.Get_Remaining_String()) {
					Evict_Client(ID,WideStringFormat(L"You are required to run the Tiberian Technologies community patch to play on this server. You can download it at http://www.tiberiantechnologies.org/. Reason: %hs",Parser.Get_Remaining_String()));
				}
				else {
					Evict_Client(ID,"You are required to run the Tiberian Technologies community patch to play on this server. You can download it at http://www.tiberiantechnologies.org/.");
				}
			}
		}
	}
};
Register_Console_Function(ForceTTConsoleFunctionClass);

class AddForceTTConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "addforcett"; }
	const char *Get_Help() { return "ADDFORCETT <name> <IP> <serial> [reason] - Adds a forceTT entry."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		const char *Name = Parser.Get_String();
		const char *IP = Parser.Get_String();
		const char *Serial = Parser.Get_String();
		if (Name && IP && Serial) {
			DABanManager::Get_ForceTT_List()->Add_Entry(Name,IP,Serial,Parser.Get_Remaining_String());
		}
	}
};
Register_Console_Function(AddForceTTConsoleFunctionClass);

class RemoveForceTTConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "removeforcett"; }
	const char *Get_Help() { return "REMOVEFORCETT <name/ip/serial> / REMOVEFORCETT <name> <ip> <serial> - Removes any forceTT that matches the specified name, IP, OR serial, or name, IP, AND serial."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		const char *Name = Parser.Get_String();
		const char *IP = Parser.Get_String();
		const char *Serial = Parser.Get_String();
		if (Name) {
			if (IP && Serial) {
				DABanManager::Get_ForceTT_List()->Remove_Entry_Match_All(Name,IP,Serial);
			}
			else {
				DABanManager::Get_ForceTT_List()->Remove_Entry(Name,Name,Name);
			}
		}
	}
};
Register_Console_Function(RemoveForceTTConsoleFunctionClass);

class ListForceTTConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "listforcett"; }
	const char *Get_Help() { return "LISTFORCETT / LISTFORCETT <name/IP/serial> / LISTFORCETT <name> <IP> <serial> - Lists all forceTTs, or any forceTT that matches the specified name, IP, OR serial, or name, IP, AND serial."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		const char *Name = Parser.Get_String();
		const char *IP = Parser.Get_String();
		const char *Serial = Parser.Get_String();
		if (Name) {
			if (IP && Serial) {
				DABanManager::Get_ForceTT_List()->List_Entries_Match_All(Name,IP,Serial);
			}
			else {
				DABanManager::Get_ForceTT_List()->List_Entries(Name,Name,Name);
			}
		}
		else {
			DABanManager::Get_ForceTT_List()->List_Entries();
		}
	}
};
Register_Console_Function(ListForceTTConsoleFunctionClass);

class AddForceTTExceptionConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "addforcettexception"; }
	const char *Get_Help() { return "ADDFORCETTEXCEPTION <name> <IP> <serial> [reason] - Adds a forceTT exception."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		const char *Name = Parser.Get_String();
		const char *IP = Parser.Get_String();
		const char *Serial = Parser.Get_String();
		if (Name && IP && Serial) {
			DABanManager::Get_ForceTT_Exception_List()->Add_Entry(Name,IP,Serial,Parser.Get_Remaining_String());
		}
	}
};
Register_Console_Function(AddForceTTExceptionConsoleFunctionClass);

class RemoveForceTTExceptionConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "removeforcettexception"; }
	const char *Get_Help() { return "REMOVEFORCETTEXCEPTION <name/ip/serial> / REMOVEFORCETTEXCEPTION <name> <ip> <serial> - Removes any forceTT exception that matches the specified name, IP, OR serial, or name, IP, AND serial."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		const char *Name = Parser.Get_String();
		const char *IP = Parser.Get_String();
		const char *Serial = Parser.Get_String();
		if (Name) {
			if (IP && Serial) {
				DABanManager::Get_ForceTT_Exception_List()->Remove_Entry_Match_All(Name,IP,Serial);
			}
			else {
				DABanManager::Get_ForceTT_Exception_List()->Remove_Entry(Name,Name,Name);
			}
		}
	}
};
Register_Console_Function(RemoveForceTTExceptionConsoleFunctionClass);

class ListForceTTExceptionConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "listforcettexception"; }
	const char *Get_Help() { return "LISTFORCETTEXCEPTION / LISTFORCETTEXCEPTION <name/IP/serial> / LISTFORCETTEXCEPTION <name> <IP> <serial> - Lists all forceTT exceptions, or any forceTT exception that matches the specified name, IP, OR serial, or name, IP, AND serial."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		const char *Name = Parser.Get_String();
		const char *IP = Parser.Get_String();
		const char *Serial = Parser.Get_String();
		if (Name) {
			if (IP && Serial) {
				DABanManager::Get_ForceTT_Exception_List()->List_Entries_Match_All(Name,IP,Serial);
			}
			else {
				DABanManager::Get_ForceTT_Exception_List()->List_Entries(Name,Name,Name);
			}
		}
		else {
			DABanManager::Get_ForceTT_Exception_List()->List_Entries();
		}
	}
};
Register_Console_Function(ListForceTTExceptionConsoleFunctionClass);

class RehashBanListConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "rehash_ban_list"; }
	const char *Get_Help() { return "Depreciated"; }
	void Activate(const char *ArgumentsString) { }
};
Register_Console_Function(RehashBanListConsoleFunctionClass);

/********************************************************************************************************************************/

void DABanManager::Init() {
	BanList = new DABanListClass("Ban","banlist.tsv");
	BanExceptionList = new DABanListClass("Ban Exception","banexceptionlist.tsv");
	ForceTTList = new DABanListClass("ForceTT","forcettlist.tsv");
	ForceTTExceptionList = new DABanListClass("ForceTT Exception","forcettexceptionlist.tsv");

	static DABanManager Instance;
	Instance.Register_Event(DAEvent::CONNECTIONREQUEST,INT_MAX);
}

void DABanManager::Shutdown() {
	delete ForceTTExceptionList;
	delete ForceTTList;
	delete BanExceptionList;
	delete BanList;
}

DABanListClass *DABanManager::Get_Ban_List() {
	return BanList;
}

DABanListClass *DABanManager::Get_Ban_Exception_List() {
	return BanExceptionList;
}

DABanListClass *DABanManager::Get_ForceTT_List() {
	return ForceTTList;
}

DABanListClass *DABanManager::Get_ForceTT_Exception_List() {
	return ForceTTExceptionList;
}

ConnectionAcceptanceFilter::STATUS DABanManager::Connection_Request_Event(ConnectionRequest &Request,WideStringClass &RefusalMessage) {
	StringClass Name = Request.clientName;
	StringClass IP;
	IP.Format("%d.%d.%d.%d",0xFF & Request.clientAddress.sin_addr.s_addr,(0xFF00 & Request.clientAddress.sin_addr.s_addr) >> 8,(0xFF0000 & Request.clientAddress.sin_addr.s_addr) >> 16,((0xFF000000 & Request.clientAddress.sin_addr.s_addr) >> 24));
	DABanEntryClass *Entry = BanList->Find_Entry(Name,IP,Request.clientSerialHash);  //Lookup any bans matching this player.
	if (Entry) { //Ban matched.
		DABanEntryClass *Exception = BanExceptionList->Find_Entry(Name,IP,Request.clientSerialHash);  //Lookup any ban exceptions matching this player.
		if (Exception) { //Exception matched.
			StringClass String;
			String.Format("Player %s(%s)(%s)(%.1f)(%u) matches Ban %s(%s)(%s)(%s) but is allowed by Ban Exception %s(%s)(%s)(%s).",Name,IP,Request.clientSerialHash,Request.clientVersion,Request.clientRevisionNumber,Entry->Get_Name(),Entry->Get_IP(),Entry->Get_Serial(),Entry->Get_Reason(),Exception->Get_Name(),Exception->Get_IP(),Exception->Get_Serial(),Exception->Get_Reason());
			Console_Output("%s\n",String);
			DALogManager::Write_Log("_BAN","%s",String);
		}
		else {
			StringClass Reason;
			if (!Entry->Get_Reason().Is_Empty()) {
				Reason.Format(": %s",Entry->Get_Reason());
			}
			else {
				Reason = ".";
			}
			StringClass String;
			String.Format("Player %s(%s)(%s)(%.1f)(%u) matches Ban %s(%s)(%s)(%s).",Name,IP,Request.clientSerialHash,Request.clientVersion,Request.clientRevisionNumber,Entry->Get_Name(),Entry->Get_IP(),Entry->Get_Serial(),Entry->Get_Reason());
			Console_Output("%s\n",String);
			DALogManager::Write_Log("_BAN","%s",String);
			RefusalMessage.Format(L"You are banned from this server%hs",Reason);
			return ConnectionAcceptanceFilter::STATUS_REFUSING;
		}
	}
	if (Request.clientVersion < 4.0f) {
		Entry = ForceTTList->Find_Entry(Name,IP,Request.clientSerialHash); //Lookup any forceTTs matching this player.
		if (Entry) { //ForceTT matched.
			DABanEntryClass *Exception = ForceTTExceptionList->Find_Entry(Name,IP,Request.clientSerialHash);  //Lookup any forceTT exceptions matching this player.
			if (Exception) { //Exception matched.
				StringClass String;
				String.Format("Player %s(%s)(%s)(%.1f)(%u) matches ForceTT %s(%s)(%s)(%s) but is allowed by ForceTT Exception %s(%s)(%s)(%s).",Name,IP,Request.clientSerialHash,Request.clientVersion,Request.clientRevisionNumber,Entry->Get_Name(),Entry->Get_IP(),Entry->Get_Serial(),Entry->Get_Reason(),Exception->Get_Name(),Exception->Get_IP(),Exception->Get_Serial(),Exception->Get_Reason());
				Console_Output("%s\n",String);
				DALogManager::Write_Log("_BAN","%s",String);
			}
			else {
				StringClass String;
				String.Format("Player %s(%s)(%s)(%.1f)(%u) matches ForceTT %s(%s)(%s)(%s).",Name,IP,Request.clientSerialHash,Request.clientVersion,Request.clientRevisionNumber,Entry->Get_Name(),Entry->Get_IP(),Entry->Get_Serial(),Entry->Get_Reason());
				Console_Output("%s\n",String);
				DALogManager::Write_Log("_BAN","%s",String);
				RefusalMessage.Format(L"You are required to run the Tiberian Technologies community patch to play on this server. Download it at http://www.tiberiantechnologies.org/.");
				return ConnectionAcceptanceFilter::STATUS_REFUSING;
			}
		}
	}
	return ConnectionAcceptanceFilter::STATUS_ACCEPTING;
}
