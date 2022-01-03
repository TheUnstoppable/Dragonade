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
#ifndef TT_INCLUDE_C4GAMEOBJDEF_H
#define TT_INCLUDE_C4GAMEOBJDEF_H
#include "SimpleGameObjDef.h"
class C4GameObjDef : public SimpleGameObjDef {
public:
	C4GameObjDef();
	~C4GameObjDef() {};
	const PersistFactoryClass &Get_Factory() const;
	bool Save(ChunkSaveClass &csave);
	bool Load(ChunkLoadClass &cload);
	uint32 Get_Class_ID() const;
	PersistClass *Create() const;
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE( C4GameObjDef, SimpleGameObjDef );
private:
	float ThrowVelocity;
	friend class C4GameObj;
	friend class PresetDump;
};

#endif