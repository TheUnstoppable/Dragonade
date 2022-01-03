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
#ifndef TT_INCLUDE_OBBOXCLASS_H
#define TT_INCLUDE_OBBOXCLASS_H
#include "Matrix3.h"
#include "Vector3.h"
#include "Matrix3D.h"
class OBBoxClass
{
public:
	OBBoxClass(void)
	{
	}
	OBBoxClass(const OBBoxClass & that) : Basis(that.Basis),Center(that.Center),Extent(that.Extent)
	{
	}
	OBBoxClass(const Vector3 & center,const Vector3 & extent) : Basis(1),Center(center),Extent(extent)
	{
	}
	OBBoxClass(const Vector3 & center,const Vector3 & extent,const Matrix3 & basis) : Basis(basis),Center(center),Extent(extent)
	{
	}

	OBBoxClass(const Vector3& aabb_center,const Vector3& aabb_extent, const Matrix3D& transform): 
		Center(transform * aabb_center), 
		Extent(aabb_extent), 
		Basis(transform)
	{
	}

	OBBoxClass(const Vector3 * points, int num_points);
	bool operator== (const OBBoxClass &src)
	{
		return (Center == src.Center) && (Extent == src.Extent) && (Basis == src.Basis);
	}
	bool operator!= (const OBBoxClass &src)
	{
		return (Center != src.Center) || (Extent != src.Extent) && (Basis == src.Basis);
	}
	void		Init_From_Box_Points(Vector3 * points,int num_points);
	void		Init_Random(float min_extent = 0.5f,float max_extent = 1.0f);
	float		Project_To_Axis(const Vector3 & axis) const
	{
		float x = Extent[0] * Vector3::Dot_Product(axis,Vector3(Basis[0][0],Basis[1][0],Basis[2][0]));
		float y = Extent[1] * Vector3::Dot_Product(axis,Vector3(Basis[0][1],Basis[1][1],Basis[2][1]));
		float z = Extent[2] * Vector3::Dot_Product(axis,Vector3(Basis[0][2],Basis[1][2],Basis[2][2]));
		return (WWMath::Fabs(x) + WWMath::Fabs(y) + WWMath::Fabs(z));
	}
	float		Volume(void) const
	{
		return 2.0f*Extent.X * 2.0f*Extent.Y * 2.0f*Extent.Z;
	}
	void		Compute_Point(float params[3],Vector3 * set_point) const
	{
		Vector3 point = Extent;
		point.X *= params[0];
		point.Y *= params[1];
		point.Z *= params[2];
		Matrix3::Rotate_Vector(Basis,point,set_point);
		Vector3::Add(Center,*set_point,set_point);
	}
	void		Compute_Axis_Aligned_Extent(Vector3 * set_extent) const
	{
		set_extent->X =	WWMath::Fabs(Extent[0] * Basis[0][0]) + WWMath::Fabs(Extent[1] * Basis[0][1]) +	WWMath::Fabs(Extent[2] * Basis[0][2]);
		set_extent->Y =	WWMath::Fabs(Extent[0] * Basis[1][0]) + WWMath::Fabs(Extent[1] * Basis[1][1]) +	WWMath::Fabs(Extent[2] * Basis[1][2]);
		set_extent->Z =	WWMath::Fabs(Extent[0] * Basis[2][0]) + WWMath::Fabs(Extent[1] * Basis[2][1]) +	WWMath::Fabs(Extent[2] * Basis[2][2]);
	}
	Matrix3 Basis; // 0000: rotation matrix
	Vector3 Center; // 0024: centerpoint
	Vector3 Extent; // 0030: size (i.e. how far from the centerpoint in each direction does the box go)
	static void	Transform(const Matrix3D & tm,const OBBoxClass & in,OBBoxClass * out)
	{
		out->Extent = in.Extent;
		Matrix3D::Transform_Vector(tm,in.Center,&(out->Center));
		Matrix3::Multiply(tm,in.Basis,&(out->Basis));
	}

	float ComputeSquaredDistanceFromPoint(const Vector3& point)
	{
		Vector3 v = point - Center;
		float sq_dist = 0.0f;

		for (int i = 0; i < 3; ++i)
		{
			float d = Vector3::Dot_Product(v, Basis[i]);

			float excess;
			if (d < -Extent[i]) excess = d + Extent[i];
			else if (d > Extent[i]) excess = d - Extent[i];
			else excess = 0;

			sq_dist += excess * excess;
		}

		return sq_dist;
	}

}; // 003C

#endif
