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
#ifndef TT_INCLUDE_PHYSCOLTEST_H
#define TT_INCLUDE_PHYSCOLTEST_H
#include "PhysDefClass.h"
#include "coltype.h"
#include "coltest.h"
class PhysClass;
class PhysRayCollisionTestClass : public RayCollisionTestClass
{
public:
	PhysRayCollisionTestClass(const LineSegClass& ray, CastResultStruct* res, Collision_Group_Type group, Collision_Type type = COLLISION_TYPE_PROJECTILE) :
		RayCollisionTestClass(ray, res, type, false), 
		CollidedPhysObj(NULL),
		CollisionGroup(group),
		CheckStaticObjs(true),
		CheckDynamicObjs(true)
	{
	}
	PhysClass *		CollidedPhysObj;
	Collision_Group_Type CollisionGroup;
	bool				CheckStaticObjs;
	bool				CheckDynamicObjs;
private:
	PhysRayCollisionTestClass(const PhysRayCollisionTestClass & );
	PhysRayCollisionTestClass & operator = (const PhysRayCollisionTestClass & );
};

class PhysAABoxCollisionTestClass : public AABoxCollisionTestClass
{
public:
	PhysAABoxCollisionTestClass(const AABoxClass & aabox,const Vector3 & move,CastResultStruct * res,Collision_Group_Type group,Collision_Type type = COLLISION_TYPE_PHYSICAL) : 
		AABoxCollisionTestClass(aabox,move,res,type), 
		CollidedPhysObj(NULL),
		CollisionGroup(group),
		CheckStaticObjs(true),
		CheckDynamicObjs(true)
	{
	}
	PhysClass *		CollidedPhysObj; // 0048
	Collision_Group_Type CollisionGroup; // 004C
	bool				CheckStaticObjs; // 0050
	bool				CheckDynamicObjs;  // 0051
private:
	PhysAABoxCollisionTestClass(const PhysAABoxCollisionTestClass & );
	PhysAABoxCollisionTestClass & operator = (const PhysAABoxCollisionTestClass & );
}; // 0054

class PhysOBBoxCollisionTestClass : public OBBoxCollisionTestClass
{
public:

    PhysOBBoxCollisionTestClass(const OBBoxClass& box, const Vector3& move, CastResultStruct* res, Collision_Group_Type group, Collision_Type type = COLLISION_TYPE_PHYSICAL) :
        OBBoxCollisionTestClass(box, move, res, type),
        CollidedPhysObj(NULL),
        CollisionGroup(group),
        CheckStaticObjs(true),
        CheckDynamicObjs(true),
        ForceCheckModel(false)
    {
    }

    PhysClass*              CollidedPhysObj;
    Collision_Group_Type    CollisionGroup;
    bool                    CheckStaticObjs;
    bool                    CheckDynamicObjs;
    bool                    ForceCheckModel;

private:

    PhysOBBoxCollisionTestClass(const PhysOBBoxCollisionTestClass&);
    PhysOBBoxCollisionTestClass& operator = (const PhysOBBoxCollisionTestClass&);
};

#endif
