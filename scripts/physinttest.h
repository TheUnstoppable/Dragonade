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
#ifndef TT_INCLUDE_PHYSINTTEST_H
#define TT_INCLUDE_PHYSINTTEST_H
#include "inttest.h"
#include "MeshClass.h"
class PhysAABoxIntersectionTestClass : public AABoxIntersectionTestClass
{
public:
	PhysAABoxIntersectionTestClass(const AABoxClass & box,int col_group,int col_type,MultiListClass<PhysClass> * result_list = NULL) :
		AABoxIntersectionTestClass(box,col_type),
		CollisionGroup(col_group),
		IntersectedObjects(result_list),
		CheckStaticObjs(true),
		CheckDynamicObjs(true)
	{
	}
	void							Add_Intersected_Object(PhysClass * obj) { if (IntersectedObjects) IntersectedObjects->Add(obj); }
public:
	int							CollisionGroup;
	bool							CheckStaticObjs;
	bool							CheckDynamicObjs;
private:
	MultiListClass<PhysClass> *	IntersectedObjects;
	PhysAABoxIntersectionTestClass(const PhysAABoxIntersectionTestClass & );
	PhysAABoxIntersectionTestClass & operator = (const PhysAABoxIntersectionTestClass & );
};
class PhysOBBoxIntersectionTestClass : public OBBoxIntersectionTestClass
{
public:
	PhysOBBoxIntersectionTestClass(const OBBoxClass & box,int col_group,int col_type,MultiListClass<PhysClass> * result_list = NULL) :
		OBBoxIntersectionTestClass(box,col_type),
		CollisionGroup(col_group),
		IntersectedObjects(result_list),
		CheckStaticObjs(true),
		CheckDynamicObjs(true)
	{
	}
	void							Add_Intersected_Object(PhysClass * obj) { if (IntersectedObjects) IntersectedObjects->Add(obj); }
public:
	int							CollisionGroup;
	bool							CheckStaticObjs;
	bool							CheckDynamicObjs;
private:
	MultiListClass<PhysClass> *	IntersectedObjects;
	PhysOBBoxIntersectionTestClass(const PhysOBBoxIntersectionTestClass & );
	PhysOBBoxIntersectionTestClass & operator = (const PhysOBBoxIntersectionTestClass & );
};
class PhysMeshIntersectionTestClass : public IntersectionTestClass
{
public:
	PhysMeshIntersectionTestClass::PhysMeshIntersectionTestClass(MeshClass * mesh,int col_group,int col_type,MultiListClass<PhysClass> * result_list) :
		IntersectionTestClass(col_type),
		Mesh(NULL),
		CollisionGroup(col_group),
		IntersectedObjects(result_list),
		CheckStaticObjs(true),
		CheckDynamicObjs(true)
	{
		REF_PTR_SET(Mesh,mesh);
		if (mesh)
		{
			BoundingBox = Mesh->Get_Bounding_Box();
		}
	}
	~PhysMeshIntersectionTestClass(void)
	{
		REF_PTR_RELEASE(Mesh);
	}
	bool							Cull(const Vector3 & min,const Vector3 & max);
	bool							Cull(const AABoxClass & box);
	void							Add_Intersected_Object(PhysClass * obj) { if (IntersectedObjects) IntersectedObjects->Add(obj); }
public:
	MeshClass *					Mesh;
	AABoxClass					BoundingBox;
	int							CollisionGroup;
	bool							CheckStaticObjs;
	bool							CheckDynamicObjs;
private:
	MultiListClass<PhysClass> *	IntersectedObjects;
private:
	PhysMeshIntersectionTestClass(const PhysMeshIntersectionTestClass & );
	PhysMeshIntersectionTestClass & operator = (const PhysMeshIntersectionTestClass & );
};

inline bool PhysMeshIntersectionTestClass::Cull(const Vector3 & cull_min,const Vector3 & cull_max)
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
inline bool PhysMeshIntersectionTestClass::Cull(const AABoxClass & cull_box)
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
#endif
