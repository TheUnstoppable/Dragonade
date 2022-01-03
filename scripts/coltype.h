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
#ifndef TT_INCLUDE_COLTYPE_H
#define TT_INCLUDE_COLTYPE_H
typedef enum
{
	COLLISION_TYPE_ALL = 0x01,
	COLLISION_TYPE_0 = 0x02,
	COLLISION_TYPE_1 = 0x04,
	COLLISION_TYPE_2 = 0x08,
	COLLISION_TYPE_3 = 0x10,
	COLLISION_TYPE_4 = 0x20,
	COLLISION_TYPE_5 = 0x40,
	COLLISION_TYPE_6 = 0x80,
	COLLISION_TYPE_PHYSICAL = COLLISION_TYPE_0,
	COLLISION_TYPE_PROJECTILE =	COLLISION_TYPE_1,
	COLLISION_TYPE_VIS = COLLISION_TYPE_2,
	COLLISION_TYPE_CAMERA = COLLISION_TYPE_3,
	COLLISION_TYPE_VEHICLE = COLLISION_TYPE_4,
	COLLISION_TYPE_USER1 = COLLISION_TYPE_5,
	COLLISION_TYPE_USER2 = COLLISION_TYPE_6,
} Collision_Type;
#endif
