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
#ifndef TT_INCLUDE__STATICANIMPHYSDEFCLASS_H
#define TT_INCLUDE__STATICANIMPHYSDEFCLASS_H

#include "StaticPhysDefClass.h"
#include "ProjectorManagerClass.h"
#include "AnimCollisionManagerClass.h"
class StaticAnimPhysDefClass : public StaticPhysDefClass
{
public:
	StaticAnimPhysDefClass(void);
	virtual uint32								Get_Class_ID (void) const;
	virtual PersistClass *					Create(void) const;
	virtual const char *						Get_Type_Name(void)			{ return "StaticAnimPhysDef"; }
	virtual bool								Is_Type(const char *);
	virtual const PersistFactoryClass &	Get_Factory (void) const;
	virtual bool								Save(ChunkSaveClass &csave);
	virtual bool								Load(ChunkLoadClass &cload);
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE(StaticAnimPhysDefClass,StaticPhysDefClass);
	bool											Shadow_Dynamic_Objs(void)			{ return ShadowDynamicObjs; }
	bool											Shadow_Is_Additive(void)			{ return ShadowIsAdditive; }
	bool											Shadow_Ignores_Z_Rotation(void)	{ return ShadowIgnoresZRotation; }
	float											Shadow_NearZ(void)					{ return ShadowNearZ; }
	float											Shadow_FarZ(void)						{ return ShadowFarZ; }
	float											Shadow_Intensity(void)				{ return ShadowIntensity; }
	bool											Does_Collide_In_Pathfind(void)	{ return DoesCollideInPathfind; }
protected:
	bool											IsCosmetic;
	AnimCollisionManagerDefClass			AnimManagerDef;
	ProjectorManagerDefClass				ProjectorManagerDef;
	bool											ShadowDynamicObjs;
	bool											ShadowIsAdditive;
	bool											ShadowIgnoresZRotation;
	float											ShadowNearZ;
	float											ShadowFarZ;
	float											ShadowIntensity;
	bool											DoesCollideInPathfind;
	friend class StaticAnimPhysClass;
	friend class PresetDump;
}; // RH7: 116
#endif
