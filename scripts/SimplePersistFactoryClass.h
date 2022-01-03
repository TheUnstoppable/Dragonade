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
#ifndef TT_INCLUDE__SIMPLEPERSISTFACTORYCLASS_H
#define TT_INCLUDE__SIMPLEPERSISTFACTORYCLASS_H

#include "PersistFactoryClass.h"
#include "SaveLoadSystemClass.h"

template <class T, unsigned long I>
class SimplePersistFactoryClass : public PersistFactoryClass
{
public:
	virtual uint32 Chunk_ID () const
	{
		return I;
	}
	PersistClass* Load(ChunkLoadClass& xLoad) const
	{
		T *newobj = new T;
		void *pointer;
		xLoad.Open_Chunk();
		xLoad.Read(&pointer,4);
		xLoad.Close_Chunk();
		xLoad.Open_Chunk();
		newobj->Load(xLoad);
		xLoad.Close_Chunk();
		SaveLoadSystemClass::Register_Pointer(pointer,newobj);
		return newobj;
	}
	void Save(ChunkSaveClass& xSave, PersistClass* persist) const
	{
		xSave.Begin_Chunk(0x100100);
		xSave.Write(&persist,4);
		xSave.End_Chunk();
		xSave.Begin_Chunk(0x100101);
		persist->Save(xSave);
		xSave.End_Chunk();
	}
};


#endif
