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
#include "TDBCategoryClass.h"
#include "SimplePersistFactoryClass.h"
SimplePersistFactoryClass<TDBCategoryClass,589826> _TDBCategoryPersistFactory;

TDBCategoryClass::TDBCategoryClass() : ID(0)
{
}

TDBCategoryClass::TDBCategoryClass(const TDBCategoryClass &src) : ID(0)
{
	*this = src;
}

TDBCategoryClass::~TDBCategoryClass()
{
}

const TDBCategoryClass &TDBCategoryClass::operator= (const TDBCategoryClass &src)
{
	ID = src.ID;
	Name = src.Name;
	return *this;
}

const PersistFactoryClass &TDBCategoryClass::Get_Factory() const
{
	return _TDBCategoryPersistFactory;
}

bool TDBCategoryClass::Save(ChunkSaveClass &csave)
{
	csave.Begin_Chunk(0x11221023);
	csave.End_Chunk();
	csave.Begin_Chunk(0x11221022);
	Save_Variables(csave);
	csave.End_Chunk();
	return true;
}

bool TDBCategoryClass::Load(ChunkLoadClass &cload)
{
	while (cload.Open_Chunk())
	{
		switch(cload.Cur_Chunk_ID()) 
		{
			case 0x11221022:
				Load_Variables(cload);
				break;
		}
		cload.Close_Chunk();
	}
	return true;
}

void TDBCategoryClass::Save_Variables(ChunkSaveClass &csave)
{
	WRITE_MICRO_CHUNK(csave,1,ID);
	WRITE_MICRO_CHUNK_WWSTRING(csave,2,Name);
}

void TDBCategoryClass::Load_Variables(ChunkLoadClass &cload)
{
	while (cload.Open_Micro_Chunk ())
	{
		switch (cload.Cur_Micro_Chunk_ID ())
		{
			READ_MICRO_CHUNK (cload, 1,	ID);
			READ_MICRO_CHUNK_WWSTRING (cload, 2, Name);
			default:
				break;
		}
		cload.Close_Micro_Chunk();
	}
}
