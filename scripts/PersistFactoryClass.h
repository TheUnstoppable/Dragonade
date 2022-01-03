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
#ifndef __PERSISTFACTORYCLASS_H__
#define __PERSISTFACTORYCLASS_H__
#include "scripts.h"
#include "engine_common.h"
#include "engine_vector.h"
#include "engine_io.h"
#include "engine_threading.h"
#include "engine_string.h"
class PersistClass;
class PersistFactoryClass
{
public:
	PersistFactoryClass* NextFactory;
	PersistFactoryClass();
	virtual ~PersistFactoryClass();
	virtual uint32 Chunk_ID() const = 0;
	virtual PersistClass* Load     (ChunkLoadClass& xLoad) const = 0;
	virtual void          Save     (ChunkSaveClass& xSave, PersistClass*) const = 0;
};


#endif
