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
#ifndef TT_INCLUDE_FRUSTUMCLASS_H
#define TT_INCLUDE_FRUSTUMCLASS_H
#include "Vector3.h"
#include "plane.h"
class FrustumClass
{
public:
	void Init(			const Matrix3D & camera,
							const Vector2 & vpmin,
							const Vector2 & vpmax,
							float znear,
							float zfar )
	{
		int i;
		CameraTransform = camera;
		if ((znear > 0.0f) && (zfar > 0.0f))
		{
			znear = -znear;
			zfar = -zfar;
		}
		Corners[0].Set(vpmin.X, vpmax.Y, 1.0);
		Corners[4] = Corners[0];
		Corners[0] *= znear;
		Corners[4] *= zfar;
		Corners[1].Set(vpmax.X, vpmax.Y, 1.0);
		Corners[5] = Corners[1];
		Corners[1] *= znear;
		Corners[5] *= zfar;
		Corners[2].Set(vpmin.X, vpmin.Y, 1.0);
		Corners[6] = Corners[2];
		Corners[2] *= znear;
		Corners[6] *= zfar;
		Corners[3].Set(vpmax.X, vpmin.Y, 1.0);
		Corners[7] = Corners[3];
		Corners[3] *= znear;
		Corners[7] *= zfar; 
		for (i = 0; i < 8; i++)
		{
			Matrix3D::Transform_Vector(CameraTransform, Corners[i], &(Corners[i]));
		}
		PlaneClass frustum_planes[6];
		Planes[0].Set(Corners[0], Corners[3], Corners[1]);
		Planes[1].Set(Corners[0], Corners[5], Corners[4]);
		Planes[2].Set(Corners[0], Corners[6], Corners[2]);
		Planes[3].Set(Corners[2], Corners[7], Corners[3]);
		Planes[4].Set(Corners[1], Corners[7], Corners[5]);
		Planes[5].Set(Corners[4], Corners[7], Corners[6]);
		BoundMin = BoundMax = Corners[0];
		for (i=1; i<8;i++)
		{
			if (Corners[i].X < BoundMin.X) BoundMin.X = Corners[i].X;
			if (Corners[i].X > BoundMax.X) BoundMax.X = Corners[i].X;
			if (Corners[i].Y < BoundMin.Y) BoundMin.Y = Corners[i].Y;
			if (Corners[i].Y > BoundMax.Y) BoundMax.Y = Corners[i].Y;
			if (Corners[i].Z < BoundMin.Z) BoundMin.Z = Corners[i].Z;
			if (Corners[i].Z > BoundMax.Z) BoundMax.Z = Corners[i].Z;
		}
	}
	const Vector3 &	Get_Bound_Min(void) const		{ return BoundMin; }
	const Vector3 &	Get_Bound_Max(void) const		{ return BoundMax; }
	Matrix3D CameraTransform; // 0000
	PlaneClass Planes[6]; // 0030
	Vector3 Corners[8]; // 0090
	Vector3 BoundMin; // 00F0
	Vector3 BoundMax; // 00FC
}; // 0108

#endif
