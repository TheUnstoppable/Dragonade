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
#ifndef TT_INCLUDE_DEFENSEOBJECTDEFCLASS_H
#define TT_INCLUDE_DEFENSEOBJECTDEFCLASS_H
#include "DataSafe.h"
class ChunkSaveClass;
class ChunkLoadClass;
typedef	safe_unsigned_int	SafeArmorType;
class DefenseObjectDefClass {
public:
	DefenseObjectDefClass(void);
	~DefenseObjectDefClass(void);
	bool Save(ChunkSaveClass &csave);
	bool Load(ChunkLoadClass &cload);
	safe_float Health; // 0000
	safe_float HealthMax; // 0004
	SafeArmorType Skin; // 0008
	safe_float ShieldStrength; // 000C
	safe_float ShieldStrengthMax; // 0010
	SafeArmorType ShieldType; // 0014
	safe_float DamagePoints; // 0018
	safe_float DeathPoints; // 001C
}; // 0020

#endif