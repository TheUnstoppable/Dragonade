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
#ifndef TT_INCLUDE__PROTOTYPECLASS_H
#define TT_INCLUDE__PROTOTYPECLASS_H
#include "engine_io.h"
class RenderObjClass;
class PrototypeClass {
public:
	PrototypeClass* NextHash;
	PrototypeClass() : NextHash(0)
	{
	}
	virtual ~PrototypeClass()
	{
	}
	virtual const char *Get_Name() const = 0;
	virtual int Get_Class_ID() const = 0;
	virtual RenderObjClass *Create() = 0;
};

class PrototypeLoaderClass
{
public:
	virtual int Chunk_Type() = 0;
	virtual PrototypeClass* Load_W3D(ChunkLoadClass &chunk_load) = 0;
};

#endif
