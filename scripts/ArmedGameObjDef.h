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
#ifndef TT_INCLUDE_ARMEDGAMEOBJDEF_H
#define TT_INCLUDE_ARMEDGAMEOBJDEF_H
#include "PhysicalGameObjDef.h"
class ArmedGameObjDef : public PhysicalGameObjDef
{
public:
	ArmedGameObjDef( void );
	virtual bool								Save( ChunkSaveClass &csave );
	virtual bool								Load( ChunkLoadClass &cload );
	int											Get_Weapon_Def_ID() const { return WeaponDefID; }
	int											Get_Secondary_Weapon_Def_ID() const { return SecondaryWeaponDefID; }
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE(ArmedGameObjDef,PhysicalGameObjDef);
//protected: //DA
	//All of these floats are in radians (although leveledit shows degrees)
	float WeaponTiltRate; // 00A8
	float WeaponTiltMin; // 00AC
	float WeaponTiltMax; // 00B0
	float WeaponTurnRate; // 00B4
	float WeaponTurnMin; // 00B8
	float WeaponTurnMax; // 00BC
	float WeaponError; // 00C0
  	int											WeaponDefID;
  	int											SecondaryWeaponDefID;
  	safe_int										WeaponRounds;
	friend	class								ArmedGameObj;
	friend class PresetDump;
}; // 00D0

#endif