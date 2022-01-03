/*	Renegade Scripts.dll
    Dragonade Settings Manager
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
#include "engine_game.h"
#include "engine_da.h"
#include "da.h"
#include "da_settings.h"
#include "da_event.h"
#include "da_game.h"
#include "da_log.h"

DynamicVectorClass<DASettingsClass*> DASettingsManager::Settings;

DASettingsClass::DASettingsClass() {
	INI = 0;
	LastModTime = 0;
}

DASettingsClass::DASettingsClass(const char *Name) {
	INI = 0;
	LastModTime = 0;
	StringClass Temp = Name;
	Temp.ToLower();
	RawFileClass File(Temp);
	Set_INI(File);
}

DASettingsClass::DASettingsClass(INIClass *That) {
	INI = 0;
	LastModTime = 0;
	RawFileClass File(That->Get_File_Name());
	Set_INI(File);
}

DASettingsClass::DASettingsClass(const DASettingsClass &That) {
	INI = 0;
	LastModTime = 0;
	RawFileClass File(That.Get_File_Name());
	Set_INI(File);
}

const INIClass *DASettingsClass::Get_INI() const {
	return INI;
}

const char *DASettingsClass::Get_File_Name() const {
	return INI->Get_File_Name();
}

DASettingsClass::~DASettingsClass() {
	delete INI;
	INI = 0;
}

void DASettingsClass::Set_INI(RawFileClass &File) {
	INIClass *Temp = INI;
	INI = new INIClass(File);
	delete Temp;
}

void DASettingsClass::Reload() {
	RawFileClass File;
	File.Open(Get_File_Name(),1);
	if (File.Get_Date_Time() != LastModTime) {
		LastModTime = File.Get_Date_Time();
		Console_Output("New settings detected and loaded from \"%s\"!\n",Get_File_Name());
		DALogManager::Write_Log("_GENERAL","New settings detected and loaded from \"%s\"!",Get_File_Name());
	}
	Set_INI(File);
}

void DASettingsClass::Reload_Silent() {
	RawFileClass File(Get_File_Name());
	Set_INI(File);
}

int DASettingsClass::Get_Int(const char *Entry,int Default) const {
	return Get_Int("General",DAGameManager::Get_Map(),Entry,Default);
}

int DASettingsClass::Get_Int(const char *Section,const char *Entry,int Default) const {
	return Get_Int(Section,StringFormat("%s_%s",DAGameManager::Get_Map(),Section),Entry,Default);
}

int DASettingsClass::Get_Int(const char *Section1,const char *Section2,const char *Entry,int Default) const {
	return INI->Get_Int(Section2,Entry,INI->Get_Int(Section1,Entry,Default));
}

float DASettingsClass::Get_Float(const char *Entry,float Default) const {
	return Get_Float("General",DAGameManager::Get_Map(),Entry,Default);
}

float DASettingsClass::Get_Float(const char *Section,const char *Entry,float Default) const {
	return Get_Float(Section,StringFormat("%s_%s",DAGameManager::Get_Map(),Section),Entry,Default);
}

float DASettingsClass::Get_Float(const char *Section1,const char *Section2,const char *Entry,float Default) const {
	return INI->Get_Float(Section2,Entry,INI->Get_Float(Section1,Entry,Default));
}

bool DASettingsClass::Get_Bool(const char *Entry,bool Default) const {
	return Get_Bool("General",DAGameManager::Get_Map(),Entry,Default);
}

bool DASettingsClass::Get_Bool(const char *Section,const char *Entry,bool Default) const {
	return Get_Bool(Section,StringFormat("%s_%s",DAGameManager::Get_Map(),Section),Entry,Default);
}

bool DASettingsClass::Get_Bool(const char *Section1,const char *Section2,const char *Entry,bool Default) const {
	return INI->Get_Bool(Section2,Entry,INI->Get_Bool(Section1,Entry,Default));
}

StringClass &DASettingsClass::Get_String(StringClass &Buffer,const char *Entry,const char *Default) const {
	return Get_String(Buffer,"General",DAGameManager::Get_Map(),Entry,Default);
}

StringClass &DASettingsClass::Get_String(StringClass &Buffer,const char *Section,const char *Entry,const char *Default) const {
	return Get_String(Buffer,Section,StringFormat("%s_%s",DAGameManager::Get_Map(),Section),Entry,Default);
}

StringClass &DASettingsClass::Get_String(StringClass &Buffer,const char *Section1,const char *Section2,const char *Entry,const char *Default) const {
	return INI->Get_String(Buffer,Section2,Entry,INI->Get_String(Buffer,Section1,Entry,Default));
}

void DASettingsClass::Get_Vector3(Vector3 &Buffer,const char *Entry,const Vector3 &Default) const {
	Get_Vector3(Buffer,"General",DAGameManager::Get_Map(),Entry,Default);
}

void DASettingsClass::Get_Vector3(Vector3 &Buffer,const char *Section,const char *Entry,const Vector3 &Default) const {
	Get_Vector3(Buffer,Section,StringFormat("%s_%s",DAGameManager::Get_Map(),Section),Entry,Default);
}

void DASettingsClass::Get_Vector3(Vector3 &Buffer,const char *Section1,const char *Section2,const char *Entry,const Vector3 &Default) const {
	StringClass String;
	String.Format("%s_X",Entry);
	if (INI->Find_Entry(Section2,String)) {
		Buffer.X = INI->Get_Float(Section2,String,Default.X);
		String.Format("%s_Y",Entry);
		Buffer.Y = INI->Get_Float(Section2,String,Default.Y);
		String.Format("%s_Z",Entry);
		Buffer.Z = INI->Get_Float(Section2,String,Default.Z);
	}
	else {
		Buffer.X = INI->Get_Float(Section1,String,Default.X);
		String.Format("%s_Y",Entry);
		Buffer.Y = INI->Get_Float(Section1,String,Default.Y);
		String.Format("%s_Z",Entry);
		Buffer.Z = INI->Get_Float(Section1,String,Default.Z);
	}
}

INISection *DASettingsClass::Get_Section(const char *Section) const {
	if (INISection *Sect = INI->Find_Section(StringFormat("%s_%s",DAGameManager::Get_Map(),Section))) {
		return Sect;
	}
	return INI->Find_Section(Section);
}



void DASettingsManager::Shutdown() {
	for (int i = 0;i < Settings.Count();i++) {
		delete Settings[i];
	}
	Settings.Delete_All();
}

void DASettingsManager::Reload() {
	for (int i = 0;i < Settings.Count();i++) {
		Settings[i]->Reload(); //Reload all settings files.
	}
	DAEventManager::Settings_Loaded_Event(); //Then trigger events.
}

void DASettingsManager::Reload_Silent() {
	for (int i = 0;i < Settings.Count();i++) {
		Settings[i]->Reload_Silent(); //Reload all files without triggering events.
	}
}

void DASettingsManager::Add_Settings(const char *Name) {
	for (int i = 0;i < Settings.Count();i++) { //Check if we already have this settings file so we don't add duplicates.
		if (!_stricmp(Settings[i]->Get_File_Name(),Name)) {
			Settings[i]->Reload_Silent();
			return;
		}
	}
	DASettingsClass *Temp = new DASettingsClass(Name);
	Settings.Add(Temp);
}

void DASettingsManager::Remove_Settings(const char *Name) {
	for (int i = 0;i < Settings.Count();i++) {
		if (!_stricmp(Settings[i]->Get_File_Name(),Name)) {
			delete Settings[i];
			Settings.Delete(i);
			return;
		}
	}
}

void DASettingsManager::Remove_Settings(int Position) {
	delete Settings[Position];
	Settings.Delete(Position);
}

const DASettingsClass *DASettingsManager::Get_Settings(const char *Name) {
	for (int i = 0;i < Settings.Count();i++) {
		if (!strcmp(Settings[i]->Get_File_Name(),Name)) {
			return Settings[i];
		}
	}
	return 0;
}

const DASettingsClass *DASettingsManager::Get_Settings(int Position) {
	return Settings[Position];
}

int DASettingsManager::Get_Settings_Count() {
	return Settings.Count();
}

int DASettingsManager::Get_Int(const char *Entry,int Default) {
	for (int i = 0;i < Settings.Count();i++) {
		Default = Settings[i]->Get_Int(Entry,Default);
	}
	return Default;
}

int DASettingsManager::Get_Int(const char *Section,const char *Entry,int Default) {
	for (int i = 0;i < Settings.Count();i++) {
		Default = Settings[i]->Get_Int(Section,Entry,Default);
	}
	return Default;
}

int DASettingsManager::Get_Int(const char *Section1,const char *Section2,const char *Entry,int Default) {
	for (int i = 0;i < Settings.Count();i++) {
		Default = Settings[i]->Get_Int(Section1,Section2,Entry,Default);
	}
	return Default;
}

float DASettingsManager::Get_Float(const char *Entry,float Default) {
	for (int i = 0;i < Settings.Count();i++) {
		Default = Settings[i]->Get_Float(Entry,Default);
	}
	return Default;
}

float DASettingsManager::Get_Float(const char *Section,const char *Entry,float Default) {
	for (int i = 0;i < Settings.Count();i++) {
		Default = Settings[i]->Get_Float(Section,Entry,Default);
	}
	return Default;
}

float DASettingsManager::Get_Float(const char *Section1,const char *Section2,const char *Entry,float Default) {
	for (int i = 0;i < Settings.Count();i++) {
		Default = Settings[i]->Get_Float(Section1,Section2,Entry,Default);
	}
	return Default;
}

bool DASettingsManager::Get_Bool(const char *Entry,bool Default) {
	for (int i = 0;i < Settings.Count();i++) {
		Default = Settings[i]->Get_Bool(Entry,Default);
	}
	return Default;
}

bool DASettingsManager::Get_Bool(const char *Section,const char *Entry,bool Default) {
	for (int i = 0;i < Settings.Count();i++) {
		Default = Settings[i]->Get_Bool(Section,Entry,Default);
	}
	return Default;
}

bool DASettingsManager::Get_Bool(const char *Section1,const char *Section2,const char *Entry,bool Default) {
	for (int i = 0;i < Settings.Count();i++) {
		Default = Settings[i]->Get_Bool(Section1,Section2,Entry,Default);
	}
	return Default;
}

StringClass &DASettingsManager::Get_String(StringClass &Buffer,const char *Entry,const char *Default) {
	Buffer = Default;
	for (int i = 0;i < Settings.Count();i++) {
		Settings[i]->Get_String(Buffer,Entry,Buffer);
	}
	return Buffer;
}

StringClass &DASettingsManager::Get_String(StringClass &Buffer,const char *Section,const char *Entry,const char *Default) {
	Buffer = Default;
	for (int i = 0;i < Settings.Count();i++) {
		Settings[i]->Get_String(Buffer,Section,Entry,Buffer);
	}
	return Buffer;
}

StringClass &DASettingsManager::Get_String(StringClass &Buffer,const char *Section1,const char *Section2,const char *Entry,const char *Default) {
	Buffer = Default;
	for (int i = 0;i < Settings.Count();i++) {
		Settings[i]->Get_String(Buffer,Section1,Section2,Entry,Buffer);
	}
	return Buffer;
}

void DASettingsManager::Get_Vector3(Vector3 &Buffer,const char *Entry,const Vector3 &Default) {
	Buffer = Default;
	for (int i = 0;i < Settings.Count();i++) {
		Settings[i]->Get_Vector3(Buffer,Entry,Buffer);
	}
}

void DASettingsManager::Get_Vector3(Vector3 &Buffer,const char *Section,const char *Entry,const Vector3 &Default) {
	Buffer = Default;
	for (int i = 0;i < Settings.Count();i++) {
		Settings[i]->Get_Vector3(Buffer,Section,Entry,Buffer);
	}
}

void DASettingsManager::Get_Vector3(Vector3 &Buffer,const char *Section1,const char *Section2,const char *Entry,const Vector3 &Default) {
	Buffer = Default;
	for (int i = 0;i < Settings.Count();i++) {
		Settings[i]->Get_Vector3(Buffer,Section1,Section2,Entry,Buffer);
	}
}

INISection *DASettingsManager::Get_Section(const char *Section) {
	for (int i = Settings.Count()-1;i >= 0;i--) {
		if (INISection *Sect = Settings[i]->Get_Section(Section)) {
			return Sect;
		}
	}
	return 0;
}



class DAReloadConsoleFunctionClass : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "reload"; }
	const char *Get_Alias() { return "rehash"; }
	const char *Get_Help() { return "RELOAD - Reload da.ini and gamemode.ini."; }
	void Activate(const char *ArgumentsString) {
		DASettingsManager::Reload();
	}
};
Register_Console_Function(DAReloadConsoleFunctionClass);