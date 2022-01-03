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
#ifndef TT_INCLUDE_COMCENTERGAMEOBJDEF_H
#define TT_INCLUDE_COMCENTERGAMEOBJDEF_H
#include "BuildingGameObjDef.h"
class ComCenterGameObjDef : public BuildingGameObjDef
{
public:
	ComCenterGameObjDef (void);
	~ComCenterGameObjDef (void);
	uint32								Get_Class_ID (void) const;
	PersistClass *						Create (void) const;
	bool									Save (ChunkSaveClass &csave);
	bool									Load (ChunkLoadClass &cload);
	const PersistFactoryClass &	Get_Factory (void) const;
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE (ComCenterGameObjDef, BuildingGameObjDef);
protected:
	void					Load_Variables (ChunkLoadClass &cload);
private:
	int unk1;
	int unk2;
	int unk3;
	friend class PresetDump;
};
#endif
