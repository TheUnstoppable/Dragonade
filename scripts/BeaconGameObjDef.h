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
#ifndef TT_INCLUDE_BEACONGAMEOBJDEF_H
#define TT_INCLUDE_BEACONGAMEOBJDEF_H
#include "SimpleGameObjDef.h"
class BeaconGameObjDef :
	public SimpleGameObjDef
{
public:
	BeaconGameObjDef();
	~BeaconGameObjDef() {};
	const PersistFactoryClass& Get_Factory() const;
	bool Save(ChunkSaveClass& chunks);
	bool Load(ChunkLoadClass& chunks);
	bool Load_Variables(ChunkLoadClass& chunks);
	uint32 Get_Class_ID() const;
	PersistClass* Create() const;
	int Is_Nuke() const {return IsNuke;}
	bool Requires_Silo() const {return RequiresSilo;}
	int Get_Explosion_Obj_Def() const {return ExplosionObjDef;}
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE( BeaconGameObjDef, SimpleGameObjDef );
//private: //DA
	float BroadcastToAllTime; // 00AC
	float ArmTime; // 00B0
	float DisarmTime; // 00B4
	float PreDetonateCinematicDelay; // 00B8
	float DetonateTime; // 00BC
	float PostDetonateTime; // 00C0
	int ArmedSoundDefID; // 00C4
	int DisarmingTextID; // 00C8
	int DisarmedTextID; // 00CC
	int ArmingTextID; // 00D0
	int ArmingInterruptedTextID; // 00D4
	int DisarmingInterruptedTextID; // 00D8
	int PreDetonateCinematicObjDef; // 00DC
	int PostDetonateCinematicObjDef; // 00E0
	int ExplosionObjDef; // 00E4
	int IsNuke; // 00E8
	StringClass ArmingAnimationName; // 00EC
	bool RequiresSilo;
	friend class BeaconGameObj;
	friend class PresetDump;
}; // 00F0

#endif