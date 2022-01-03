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
#ifndef TT_INCLUDE_DAMAGEZONEGAMEOBJDEF_H
#define TT_INCLUDE_DAMAGEZONEGAMEOBJDEF_H
#include "BaseGameObjDef.h"
class DamageZoneGameObjDef : public BaseGameObjDef {
public:
	DamageZoneGameObjDef();
	~DamageZoneGameObjDef() {};
	const PersistFactoryClass &Get_Factory() const;
	bool Save(ChunkSaveClass &csave);
	bool Load(ChunkLoadClass &cload);
	uint32 Get_Class_ID() const;
	PersistClass *Create() const;
	Vector3 &Get_Color() {return Color;}
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE(DamageZoneGameObjDef, BaseGameObjDef);
//private: //DA
	float DamageRate;
	int DamageWarhead;
	Vector3 Color;
	friend class PresetDump;
	friend class DamageZoneGameObj;
};

#endif