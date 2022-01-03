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
#ifndef TT_INCLUDE__LINESEGCLASS_H
#define TT_INCLUDE__LINESEGCLASS_H

#include "Vector3.h"
#include "Matrix3D.h"

class SCRIPTS_API LineSegClass
{
public:
	LineSegClass(void)
	{
	}
	LineSegClass(const Vector3 & p0,const Vector3 & p1) : P0(p0), P1(p1)
	{
		recalculate();
	}
	LineSegClass(const Vector3& p0, const Vector3& dir, float l) : P0(p0), Dir(dir), Length(l)
	{
		DP = Dir * Length;
		P1 = P0 + DP;
	}
	LineSegClass(const LineSegClass & that,const Matrix3D & tm)
	{
		Set(that,tm);
	}
	void					Set(const Vector3 & p0,const Vector3 & p1)
	{
		P0 = p0;
		P1 = p1;
		recalculate();
	}
	void Set(const Vector3& p0, const Vector3& dir, float l)
	{
		DP = dir * l;
		P0 = p0;
		P1 = p0 + DP;
		Dir = dir;
		Length = l;
	}
	void					Set(const LineSegClass & that,const Matrix3D & tm);
	void					Set_Random(const Vector3 & min,const Vector3 & max);
	const Vector3 &	Get_P0() const
	{
		return P0;
	}
	const Vector3 &	Get_P1() const
	{
		return P1;
	}
	const Vector3 &	Get_DP() const
	{
		return DP;
	}
	const Vector3 &	Get_Dir() const
	{
		return Dir;
	}
	float					Get_Length() const
	{
		return Length;
	}
	void					Compute_Point(float t,Vector3 * set) const
	{
		Vector3::Add(P0,t*DP,set);
	}
	Vector3				Find_Point_Closest_To(const Vector3 &pos) const;
	bool					Find_Intersection (const LineSegClass &other_line, Vector3 *p1, float *fraction1, Vector3 *p2, float *fraction2) const;
protected:
	void					recalculate(void)
	{
		DP = P1 - P0;
		Dir = DP;
		Dir.Normalize();
		Length = DP.Length();
	}
	Vector3				P0;
	Vector3				P1;
	Vector3				DP;
	Vector3				Dir;
	float					Length;
}; // 0034


#endif
