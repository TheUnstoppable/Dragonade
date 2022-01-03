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
#ifndef TT_INCLUDE__REFINERYGAMEOBJDEF_H
#define TT_INCLUDE__REFINERYGAMEOBJDEF_H

#include "BuildingGameObjDef.h"


class RefineryGameObjDef : public BuildingGameObjDef
{
public:
	RefineryGameObjDef (void);
	~RefineryGameObjDef (void);
	uint32								Get_Class_ID (void) const;
	PersistClass *						Create (void) const;
	float								Get_Funds_Gathered() const { return FundsGathered; }
	float								Get_Funds_Distributed_Per_Sec() const { return FundsDistributedPerSec; }
	bool									Save (ChunkSaveClass &csave);
	bool									Load (ChunkLoadClass &cload);
	const PersistFactoryClass &	Get_Factory (void) const;
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE (RefineryGameObjDef, BuildingGameObjDef);
//protected: //DA
	void					Load_Variables (ChunkLoadClass &cload);
	float			UnloadTime;
	float			FundsGathered;	
	float			FundsDistributedPerSec;
	int			HarvesterDefID;
	friend class RefineryGameObj;
	friend class PresetDump;
};

#endif
