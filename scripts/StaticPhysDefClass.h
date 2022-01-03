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
#ifndef TT_INCLUDE__STATICPHYSDEFCLASS_H
#define TT_INCLUDE__STATICPHYSDEFCLASS_H

#include "PhysDefClass.h"


class StaticPhysDefClass : public PhysDefClass
{
public:
	StaticPhysDefClass(void);
	virtual uint32								Get_Class_ID (void) const;
	virtual PersistClass *					Create(void) const;
	virtual const char *						Get_Type_Name(void);
	virtual bool								Is_Type(const char *);
	virtual const PersistFactoryClass &	Get_Factory (void) const;
	virtual bool								Save(ChunkSaveClass &csave);
	virtual bool								Load(ChunkLoadClass &cload);
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE(StaticPhysDefClass,PhysDefClass);
protected:
	bool											IsNonOccluder;
	char											CollisionGroup;
	char											VisibilityMode;
	friend class StaticPhysClass;
	friend class PresetDump;
}; // RH7: 40

#endif
