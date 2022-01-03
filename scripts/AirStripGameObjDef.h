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
#ifndef TT_INCLUDE__AIRSTRIPGAMEOBJDEF_H
#define TT_INCLUDE__AIRSTRIPGAMEOBJDEF_H



#include "VehicleFactoryGameObjDef.h"



class AirStripGameObjDef :
	public VehicleFactoryGameObjDef
{
public:
			AirStripGameObjDef();
	virtual ~AirStripGameObjDef();
	virtual const PersistFactoryClass& Get_Factory  () const;
	virtual bool                       Save         (ChunkSaveClass& chunkSaver);
	virtual bool                       Load         (ChunkLoadClass& chunkLoader);
	virtual uint32                     Get_Class_ID () const;
	virtual PersistClass*              Create       () const;
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE (AirStripGameObjDef, VehicleFactoryGameObjDef);
//private: //DA
	void					Load_Variables (ChunkLoadClass &cload);
	int DropCinematicDefId; // 00A8
	int CinematicSlotIndex; // 00AC
	float CinematicLengthToDropOff; // 00B0
	float CinematicLengthToVehicleDisplay; // 00B4
	bool HideVehicles;
	friend class AirStripGameObj;
	friend class PresetDump;
}; // 00B8



#endif