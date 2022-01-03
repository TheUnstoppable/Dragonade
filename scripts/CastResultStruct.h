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
#ifndef TT_INCLUDE__CASTRESULTSTRUCT_H
#define TT_INCLUDE__CASTRESULTSTRUCT_H
#include "Vector3.h"

struct CastResultStruct
{
	CastResultStruct(void)	{ Reset(); }
	void		Reset(void)		{ StartBad = false; Fraction = 1.0f; Normal.Set(0,0,0); SurfaceType = 0; ComputeContactPoint = false; ContactPoint.Set(0,0,0); }
	bool		StartBad;		// was the inital configuration interpenetrating something?
	float		Fraction;		// fraction of the move up until collision
	Vector3	Normal;			// surface normal at the collision point
	uint32	SurfaceType;	// surface type of polygon at collision point (see W3D_SURFACE_TYPES in w3d_file.h)
	bool		ComputeContactPoint;		// This signals the collision code to compute the point of collision
	Vector3	ContactPoint;				// This will be set to the point of collision if ComputeContactPoint is true
}; // 40

#endif
