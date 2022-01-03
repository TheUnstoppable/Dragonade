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

#ifndef INCLUDE_DASETTINGS
#define INCLUDE_DASETTINGS

#include "RawFileClass.h"
#include "engine_io.h"

class DA_API DASettingsClass {
public:
	DASettingsClass();
	DASettingsClass(const char *Name);
	DASettingsClass(INIClass *That);
	DASettingsClass(const DASettingsClass &That);
	const INIClass *Get_INI() const;
	const char *Get_File_Name() const;
	virtual ~DASettingsClass();

	void Reload(); //Reload from disk and display message if file has been modified.
	void Reload_Silent(); //Reload without displaying message.

	int Get_Int(const char *Entry,int Default) const; //Get an int from the "[General]" and "[<Current Map>]" sections.
	int Get_Int(const char *Section,const char *Entry,int Default) const; //Get an int from the "[<Section>]" and "[<Current Map>_<Section>]" sections.
	int Get_Int(const char *Section1,const char *Section2,const char *Entry,int Default) const; //Get an int from the "[<Section1>]" and "[<Section2>]" sections.
	float Get_Float(const char *Entry,float Default) const;
	float Get_Float(const char *Section,const char *Entry,float Default) const;
	float Get_Float(const char *Section1,const char *Section2,const char *Entry,float Default) const;
	bool Get_Bool(const char *Entry,bool Default) const;
	bool Get_Bool(const char *Section,const char *Entry,bool Default) const;
	bool Get_Bool(const char *Section1,const char *Section2,const char *Entry,bool Default) const;
	StringClass &Get_String(StringClass &Buffer,const char *Entry,const char *Default) const;
	StringClass &Get_String(StringClass &Buffer,const char *Section,const char *Entry,const char *Default) const;
	StringClass &Get_String(StringClass &Buffer,const char *Section1,const char *Section2,const char *Entry,const char *Default) const;
	void Get_Vector3(Vector3 &Buffer,const char *Entry,const Vector3 &Default) const;
	void Get_Vector3(Vector3 &Buffer,const char *Section,const char *Entry,const Vector3 &Default) const;
	void Get_Vector3(Vector3 &Buffer,const char *Section1,const char *Section2,const char *Entry,const Vector3 &Default) const;
	INISection *Get_Section(const char *Section) const; //Get the entire section "[<Section>]" or "[<Current Map>_<Section>]".

private:
	void Set_INI(RawFileClass &File);
	INIClass *INI;
	unsigned int LastModTime;
};

class DA_API DASettingsManager {
public:
	static void Shutdown();
	static void Reload(); //Reload all settings from disk, call setting loaded event, and display message if file has been modified.
	static void Reload_Silent(); //Reload all settings without calling settings loaded event or displaying message.
	
	static void Add_Settings(const char *Name); //Add a file to the settings chain.
	static void Remove_Settings(const char *Name); //Remove a file from the settings chain.
	static void Remove_Settings(int Position);
	static const DASettingsClass *Get_Settings(const char *Name); //Get the DASettingsClass for the given file.
	static const DASettingsClass *Get_Settings(int Position);
	static int Get_Settings_Count();

	static int Get_Int(const char *Entry,int Default); //Get an int from the "[General]" and "[<Current Map>]" sections of all files in the settings chain.
	static int Get_Int(const char *Section,const char *Entry,int Default); //Get an int from the "[<Section>]" and "[<Current Map>_<Section>]" sections of all files in the settings chain.
	static int Get_Int(const char *Section1,const char *Section2,const char *Entry,int Default); //Get an int from the "[<Section1>]" and "[<Section2>]" sections of all files in the settings chain.
	static float Get_Float(const char *Entry,float Default);
	static float Get_Float(const char *Section,const char *Entry,float Default);
	static float Get_Float(const char *Section1,const char *Section2,const char *Entry,float Default);
	static bool Get_Bool(const char *Entry,bool Default);
	static bool Get_Bool(const char *Section,const char *Entry,bool Default);
	static bool Get_Bool(const char *Section1,const char *Section2,const char *Entry,bool Default);
	static StringClass &Get_String(StringClass &Buffer,const char *Entry,const char *Default);
	static StringClass &Get_String(StringClass &Buffer,const char *Section,const char *Entry,const char *Default);
	static StringClass &Get_String(StringClass &Buffer,const char *Section1,const char *Section2,const char *Entry,const char *Default);
	static void Get_Vector3(Vector3 &Buffer,const char *Entry,const Vector3 &Default);
	static void Get_Vector3(Vector3 &Buffer,const char *Section,const char *Entry,const Vector3 &Default);
	static void Get_Vector3(Vector3 &Buffer,const char *Section1,const char *Section2,const char *Entry,const Vector3 &Default);
	static INISection *Get_Section(const char *Section);  //Get the entire section "[<Section>]" or "[<Current Map>_<Section>]" from all files in the settings chain.
	
private:
	static DynamicVectorClass<DASettingsClass*> Settings;
};

#endif