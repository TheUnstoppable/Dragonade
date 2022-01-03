/*	Renegade Scripts.dll
	Copyright 2017 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#include "General.h"
#include "TDBObjClass.h"
#include "SimplePersistFactoryClass.h"
#include "TranslateDBClass.h"
SimplePersistFactoryClass<TDBObjClass,589825> _TranslateObjPersistFactory;

TDBObjClass::TDBObjClass() : ID(0), SoundID(0xFFFFFFFF), CategoryID(0)
{
}

TDBObjClass::TDBObjClass(const TDBObjClass &src) : ID(0), SoundID(0xFFFFFFFF), CategoryID(0)
{
	*this = src;
}

TDBObjClass::~TDBObjClass()
{
}

const TDBObjClass &TDBObjClass::operator= (const TDBObjClass &src)
{
	TranslatedStrings = src.TranslatedStrings;
	EnglishString = src.EnglishString;
	IDDesc = src.IDDesc;
	ID = src.ID;
	SoundID = src.SoundID;
	AnimationName = src.AnimationName;
	CategoryID = src.CategoryID;
	return *this;
}

const PersistFactoryClass &TDBObjClass::Get_Factory() const
{
	return _TranslateObjPersistFactory;
}

bool TDBObjClass::Save(ChunkSaveClass &csave)
{
	csave.Begin_Chunk(0x6141109);
	csave.End_Chunk();
	csave.Begin_Chunk(0x6141108);
	Save_Variables(csave);
	csave.End_Chunk();
	WRITE_WWSTRING_CHUNK(csave,0x614110A,EnglishString);
	if (TranslateDBClass::Is_Single_Language_Export_Enabled())
	{
		int lang = TranslateDBClass::Get_Current_Language();
		if (lang > TranslatedStrings.Count())
		{
			lang = 0;
		}
		WRITE_WIDESTRING_CHUNK(csave,0x614110B,TranslatedStrings[lang]);
		return true;
	}
	else
	{
		for (int i = 0;i < TranslatedStrings.Count();i++)
		{
			WRITE_WIDESTRING_CHUNK(csave,0x614110B,TranslatedStrings[i]);
		}
		return true;
	}
}

bool TDBObjClass::Load(ChunkLoadClass &cload)
{
	while (cload.Open_Chunk())
	{
		switch(cload.Cur_Chunk_ID()) 
		{
			case 0x6141108:
				Load_Variables(cload);
				break;
			READ_WWSTRING_CHUNK(cload,0x614110A,EnglishString);
			case 0x614110B:
				WideStringClass string;
				cload.Read(string.Get_Buffer((cload.Cur_Chunk_Length() + 1) / 2),cload.Cur_Chunk_Length());
				TranslatedStrings.Add(string);
				break;
		}
		cload.Close_Chunk();
	}
	if (TranslatedStrings.Count() == 0)
	{
		TranslatedStrings.Add(WideStringClass(EnglishString));
	}
	return true;
}

void TDBObjClass::Save_Variables(ChunkSaveClass &csave)
{
	WRITE_MICRO_CHUNK(csave,1,ID);
	WRITE_MICRO_CHUNK(csave,5,SoundID);
	WRITE_MICRO_CHUNK(csave,7,CategoryID);
	WRITE_MICRO_CHUNK_WWSTRING(csave,2,IDDesc);
	WRITE_MICRO_CHUNK_WWSTRING(csave,6,AnimationName);
}

void TDBObjClass::Load_Variables(ChunkLoadClass &cload)
{
	while (cload.Open_Micro_Chunk ())
	{
		switch (cload.Cur_Micro_Chunk_ID ())
		{
			READ_MICRO_CHUNK(cload,1,ID);
			READ_MICRO_CHUNK(cload,5,SoundID);
			READ_MICRO_CHUNK(cload,7,CategoryID);
			READ_MICRO_CHUNK_WWSTRING(cload,2,IDDesc);
			READ_MICRO_CHUNK_WWSTRING(cload,4,EnglishString);
			READ_MICRO_CHUNK_WWSTRING(cload,6,AnimationName);
			case 3:
			{
				WideStringClass str;
				cload.Read(str.Get_Buffer((cload.Cur_Micro_Chunk_Length() + 1) * 2),cload.Cur_Micro_Chunk_Length());
				TranslatedStrings.Add(str);
			}
			default:
				break;
		}
		cload.Close_Micro_Chunk();
	}
}

void TDBObjClass::Set_English_String(const char *string)
{
	EnglishString = string;
	while (TranslatedStrings.Count() <= 0)
	{
		WideStringClass str("");
		TranslatedStrings.Add(str);
	}
	TranslatedStrings[0].Convert_From(string);
}

void TDBObjClass::Set_ID(uint32 ID)
{
// keeping most of this here because I can't figure out why it's being done
// 	if (!TranslateDBClass::Find_Object(ID))
// 	{
// 		TranslateDBClass::Clear_Object(this->ID - 1000);
		this->ID = ID;
// 		TranslateDBClass::Add_Object(this);
// 	}
}

void TDBObjClass::Set_ID_Desc(const char *description)
{
	IDDesc = description;
}

const WideStringClass &TDBObjClass::Get_String()
{
	return Get_String(TranslateDBClass::Get_Current_Language());
}

const WideStringClass &TDBObjClass::Get_String(uint32 language)
{
	int lang = (int)language < TranslatedStrings.Count() ? language : 0;
	return TranslatedStrings[lang];
}

void TDBObjClass::Set_String(uint32 language, const wchar_t *string)
{
	while (TranslatedStrings.Count() <= (int)language)
	{
		WideStringClass str("");
		TranslatedStrings.Add(str);
	}
	if (string)
	{
		TranslatedStrings[language] = string;
	}
	if (!language)
	{
		EnglishString.Copy_Wide(string);
	}
}

bool TDBObjClass::Contains_Translation(uint32 language)
{
	if ((int)language < TranslatedStrings.Count())
	{
		return TranslatedStrings[language].Get_Length() > 0;
	}
	return false;
}
