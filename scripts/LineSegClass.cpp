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
#include "general.h"

#include "LineSegClass.h"


Vector3 LineSegClass::Find_Point_Closest_To(const Vector3 &pos) const
{
	float val1 = ((pos.X - P0.X) * Dir.X) + ((pos.Y - P0.Y) * Dir.Y) + ((pos.Z - P0.Z) * Dir.Z);
	if (val1 > 0.0f)
	{
		if (val1 < Length)
		{
			return (Dir * val1) + P0;
		}
		return P1;
	}
	return P0;
}

void LineSegClass::Set(const LineSegClass& object, const Matrix3D& transform)
{
    Matrix3D::Transform_Vector(transform, object.P0, &P0);
    Matrix3D::Transform_Vector(transform, object.P1, &P1);
    DP = P1 - P0;
    Matrix3D::Rotate_Vector(transform, object.Dir, &Dir);
	Length = object.Length;
}

bool LineSegClass::Find_Intersection(const LineSegClass &other_line, Vector3 *p1, float *fraction1, Vector3 *p2, float *fraction2) const
{
	Vector3 cross1 = Vector3::Cross_Product(Dir,other_line.Dir);
	Vector3 cross2 = Vector3::Cross_Product(other_line.P0 - P0,other_line.Dir);
	float len = cross1.Length2();
	Vector3 cross3 = Vector3::Cross_Product(other_line.Dir,Dir);
	float len2 = cross3.Length();
	Vector3 cross4 = Vector3::Cross_Product(P0 - other_line.P0,Dir);
	if (len == 0 || len2 == 0)
	{
		return false;
	}
	else
	{
		float f1 = (cross1 * cross2) / len;
		float f2 = (cross3 * cross4) / len2;
		*p1 = Dir * f1 + P0;
		*p2 = other_line.Dir * f2 + other_line.P0;
		if (fraction1)
		{
			*fraction1 = f1 / Length;
		}
		if (fraction2)
		{
			*fraction2 = f2 / Length;
		}
		return true;
	}
}
