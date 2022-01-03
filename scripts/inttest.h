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
#ifndef TT_INCLUDE_INTTEST_H
#define TT_INCLUDE_INTTEST_H
#include "OBBoxClass.h"
class IntersectionTestClass
{
public:
	IntersectionTestClass(int collision_type) : CollisionType(collision_type)								{ }
	IntersectionTestClass(const IntersectionTestClass & that) :	CollisionType(that.CollisionType)	{ }
public:
	int								CollisionType; 
};
class AABoxIntersectionTestClass : public IntersectionTestClass
{
public:
	AABoxIntersectionTestClass(const AABoxClass & box,int collision_type) :
		IntersectionTestClass(collision_type),
		Box(box)
	{
	}
	AABoxIntersectionTestClass(const AABoxIntersectionTestClass & that) :
		IntersectionTestClass(that),
		Box(that.Box)
	{
	}
	bool								Cull(const Vector3 & cull_min,const Vector3 & cull_max);
	bool								Cull(const AABoxClass & cull_box);
	bool								Intersect_Triangle(const TriClass & tri);
public:
	AABoxClass						Box;
};
inline bool AABoxIntersectionTestClass::Cull(const Vector3 & cull_min,const Vector3 & cull_max)
{
	Vector3 box_min;
	Vector3::Subtract(Box.Center,Box.Extent,&box_min);
	Vector3 box_max;
	Vector3::Add(Box.Center,Box.Extent,&box_max);
	if ((box_min.X > cull_max.X) || (box_max.X < cull_min.X)) return true;
	if ((box_min.Y > cull_max.Y) || (box_max.Y < cull_min.Y)) return true;
	if ((box_min.Z > cull_max.Z) || (box_max.Z < cull_min.Z)) return true;
}
inline bool AABoxIntersectionTestClass::Cull(const AABoxClass & cull_box)
{
	Vector3 dc;
	Vector3 r;
	Vector3::Subtract(cull_box.Center,Box.Center,&dc);
	Vector3::Add(cull_box.Extent,Box.Extent,&r);
	if (WWMath::Fabs(dc.X) > r.X) return true;
	if (WWMath::Fabs(dc.Y) > r.Y) return true;
	if (WWMath::Fabs(dc.Z) > r.Z) return true;
}
class OBBoxIntersectionTestClass : public IntersectionTestClass
{
public:
	OBBoxIntersectionTestClass(const OBBoxClass & box,int collision_type);
	OBBoxIntersectionTestClass(const OBBoxIntersectionTestClass & that);
	OBBoxIntersectionTestClass(const OBBoxIntersectionTestClass & that,const Matrix3D & tm);
	OBBoxIntersectionTestClass(const AABoxIntersectionTestClass & that,const Matrix3D & tm);
	bool								Cull(const Vector3 & min,const Vector3 & max);
	bool								Cull(const AABoxClass & box);
	bool								Intersect_Triangle(const TriClass & tri);
protected:	
	void								update_bounding_box(void);
public:
	OBBoxClass						Box;
	AABoxClass						BoundingBox;
};
inline OBBoxIntersectionTestClass::OBBoxIntersectionTestClass(const OBBoxClass & box,int collision_type) :
	IntersectionTestClass(collision_type),
	Box(box)
{
	update_bounding_box();
}
inline OBBoxIntersectionTestClass::OBBoxIntersectionTestClass(const OBBoxIntersectionTestClass & that) :
	IntersectionTestClass(that),
	Box(that.Box)
{
	update_bounding_box();
}
inline OBBoxIntersectionTestClass::OBBoxIntersectionTestClass
(
	const OBBoxIntersectionTestClass & that,
	const Matrix3D & tm
) :
	IntersectionTestClass(that)
{
	OBBoxClass::Transform(tm,that.Box,&Box);
	update_bounding_box();
}
inline OBBoxIntersectionTestClass::OBBoxIntersectionTestClass
(
	const AABoxIntersectionTestClass & that,
	const Matrix3D & tm
) :
	IntersectionTestClass(that)
{
	Matrix3D::Transform_Vector(tm,that.Box.Center,&(Box.Center));
	Box.Extent = that.Box.Extent;
	Box.Basis = tm; // copies the 3x3 rotation portion of the transform
	update_bounding_box();
}
inline bool OBBoxIntersectionTestClass::Cull(const Vector3 & cull_min,const Vector3 & cull_max)
{
	Vector3 box_min;
	Vector3::Subtract(BoundingBox.Center,BoundingBox.Extent,&box_min);
	Vector3 box_max;
	Vector3::Add(BoundingBox.Center,BoundingBox.Extent,&box_max);
	if ((box_min.X > cull_max.X) || (box_max.X < cull_min.X)) return true;
	if ((box_min.Y > cull_max.Y) || (box_max.Y < cull_min.Y)) return true;
	if ((box_min.Z > cull_max.Z) || (box_max.Z < cull_min.Z)) return true;
	return false;
}
inline bool OBBoxIntersectionTestClass::Cull(const AABoxClass & cull_box)
{
	Vector3 dc;
	Vector3 r;
	Vector3::Subtract(cull_box.Center,BoundingBox.Center,&dc);
	Vector3::Add(cull_box.Extent,BoundingBox.Extent,&r);
	if (WWMath::Fabs(dc.X) > r.X) return true;
	if (WWMath::Fabs(dc.Y) > r.Y) return true;
	if (WWMath::Fabs(dc.Z) > r.Z) return true;
	return false;
}
inline void OBBoxIntersectionTestClass::update_bounding_box(void)
{
	BoundingBox.Center = Box.Center;
	Box.Basis.Rotate_AABox_Extent(Box.Extent,&BoundingBox.Extent);
}

#endif
