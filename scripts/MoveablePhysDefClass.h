/*	Renegade Scripts.dll
	Copyright 2013 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#ifndef TT_INCLUDE__MOVEABLEPHYSDEFCLASS_H
#define TT_INCLUDE__MOVEABLEPHYSDEFCLASS_H

#include "DynamicPhysDefClass.h"


class MoveablePhysDefClass : public DynamicPhysDefClass
{
public:
	MoveablePhysDefClass(void);
	virtual const char *						Get_Type_Name(void)			{ return "MoveablePhysDef"; }
	virtual bool								Is_Type(const char *);
	virtual bool								Save(ChunkSaveClass &csave);
	virtual bool								Load(ChunkLoadClass &cload);
	float											Get_Mass(void)					{ return Mass; }
	float											Get_Grav_Scale(void)			{ return GravScale; }
	float											Get_Elasticity(void) { return Elasticity; }
	void											Set_Mass(float new_mass)	{ Mass = new_mass; }
	void											Set_Grav_Scale(float new_g){ GravScale = new_g; }
	void											Set_Elasticity(float new_e) { Elasticity = new_e; }
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE(MoveablePhysDefClass,DynamicPhysDefClass);
//protected: //DA
	float				Mass;
	float				GravScale;
	float				Elasticity;
	enum { 
		CINEMATIC_COLLISION_NONE = 0, 
		CINEMATIC_COLLISION_STOP, 
		CINEMATIC_COLLISION_PUSH, 
		CINEMATIC_COLLISION_KILL 
	};
	int				CinematicCollisionMode;
	friend class MoveablePhysClass;
	friend class PresetDump;
}; // 36

#endif
