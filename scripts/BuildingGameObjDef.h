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
#ifndef TT_INCLUDE__BUILDINGGAMEOBJDEF_H
#define TT_INCLUDE__BUILDINGGAMEOBJDEF_H

#include "scripts.h"
#include "engine_common.h"
#include "engine_vector.h"
#include "engine_threading.h"
#include "engine_string.h"
#include "DamageableGameObjDef.h"
typedef	unsigned int	ArmorType;
namespace BuildingConstants
{
	typedef enum
	{
		TYPE_NONE					= -1,
		TYPE_POWER_PLANT,
		TYPE_SOLDIER_FACTORY,
		TYPE_VEHICLE_FACTORY,
		TYPE_REFINERY,
		TYPE_COM_CENTER,
		TYPE_REPAIR_BAY,		
		TYPE_SHRINE,
		TYPE_HELIPAD,
		TYPE_CONYARD,
		TYPE_BASE_DEFENSE,
		TYPE_TECH_CENTER,
		TYPE_NAVAL_FACTORY,
		TYPE_SPECIAL,
		TYPE_COUNT
	} BuildingType;

	typedef enum
	{
		LEGACY_TEAM_GDI						= 0,
		LEGACY_TEAM_NOD,
	} LegacyBuildingTeam;

	typedef enum
	{
		BASE_COUNT = 2,
	};
}
class BuildingGameObjDef : public DamageableGameObjDef
{
public:
	BuildingGameObjDef( void );
	virtual uint32								Get_Class_ID( void ) const;
	virtual PersistClass *						Create( void ) const ;
	virtual bool								Save( ChunkSaveClass &csave );
	virtual bool								Load( ChunkLoadClass &cload );
	virtual const PersistFactoryClass &	Get_Factory( void ) const;
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE( BuildingGameObjDef, DamageableGameObjDef );
	void										Set_Type (BuildingConstants::BuildingType type) { Type = type; }
	BuildingConstants::BuildingType				Get_Type (void) const	{ return Type; }
	const StringClass &							Get_Mesh_Prefix (void) const	{ return MeshPrefix; }
	int Get_Damage_Report(int team) const;
	int Get_Destroy_Report(int team) const;
//protected: //DA
	StringClass MeshPrefix; // 0084
	ArmorType MCTSkin; // 0088
	BuildingConstants::BuildingType Type; // 008C
	int GDIDamageReportID; // 0090
	int NodDamageReportID; // 0094
	int GDIDestroyReportID; // 0098
	int NodDestroyReportID; // 009C
	friend class BuildingGameObj;
	friend class PresetDump;
}; // 00A0


#endif
