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
#ifndef TT_INCLUDE_SCRIPTZONEGAMEOBJDEF_H
#define TT_INCLUDE_SCRIPTZONEGAMEOBJDEF_H
#include "ScriptableGameObjDef.h"
namespace ZoneConstants
{
	enum ZoneType {
		TYPE_DEFAULT = 0,
		TYPE_CTF,
		TYPE_VEHICLE_CONSTRUCTION,
		TYPE_VEHICLE_REPAIR,
		TYPE_TIBERIUM_FIELD,
		TYPE_BEACON,
		TYPE_GDI_TIB_FIELD,
		TYPE_NOD_TIB_FIELD,
		TYPE_NAVAL_SPAWN,
	};
};
class ScriptZoneGameObjDef : public ScriptableGameObjDef {
public:
	ScriptZoneGameObjDef();
	~ScriptZoneGameObjDef() {};
	const PersistFactoryClass &Get_Factory() const;
	bool Save(ChunkSaveClass &csave);
	bool Load(ChunkLoadClass &cload);
	uint32 Get_Class_ID() const;
	PersistClass *Create() const;
	bool Is_Valid_Config(StringClass& oError) {return true;}
	const Vector3 &Get_Color() const {return Color;}
	ZoneConstants::ZoneType Get_Type() const {return ZoneType;}
	int Get_Preset_ID() const {return PresetID;}
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE (ScriptZoneGameObjDef, ScriptableGameObjDef);
//protected: //DA
	ZoneConstants::ZoneType ZoneType;
	Vector3 Color;
	bool unk;
	bool CheckStarsOnly;
	bool IsEnvironmentZone;
	bool CheckC4;
	int PresetID;

	StringClass ModelName;

	friend class ScriptZoneGameObj;
	friend class PresetDump;
};

#endif