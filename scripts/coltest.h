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
#ifndef TT_INCLUDE_COLTEST_H
#define TT_INCLUDE_COLTEST_H
#include "coltype.h"
#include "castresultstruct.h"
#include "LineSegClass.h"
#include "AABoxClass.h"
#include "OBBoxClass.h"
class RenderObjClass;
class TriClass;
class SCRIPTS_API CollisionTestClass
{
public:
	CollisionTestClass(CastResultStruct * res,Collision_Type collision_type);
	CollisionTestClass(const CollisionTestClass & that);
public:
	CastResultStruct *			Result; // 0000
	Collision_Type								CollisionType; // 0004
	RenderObjClass *				CollidedRenderObj; // 0008	
}; // 000C

inline CollisionTestClass::CollisionTestClass(CastResultStruct * res,Collision_Type collision_type) :
	Result(res),
	CollisionType(collision_type),
	CollidedRenderObj(NULL)
{
}

inline CollisionTestClass::CollisionTestClass(const CollisionTestClass & that) :
	Result(that.Result),
	CollisionType(that.CollisionType),
	CollidedRenderObj(that.CollidedRenderObj)
{
}

class RayCollisionTestClass : public CollisionTestClass
{
public:
	LineSegClass     Ray;
	bool             IgnoreTranslucentMeshes;

	RayCollisionTestClass(const LineSegClass& ray, CastResultStruct* res, Collision_Type collision_type = COLLISION_TYPE_0, bool ignore_translucent_meshes = false);
	RayCollisionTestClass(const RayCollisionTestClass& raytest, const Matrix3D& tm);

	bool Cull(const Vector3& min,const Vector3& max);
	bool Cull(const AABoxClass& box);

	bool Cast_To_Triangle(const TriClass& tri);

private:
	RayCollisionTestClass(const RayCollisionTestClass &);
	RayCollisionTestClass & operator = (const RayCollisionTestClass &);
};

inline RayCollisionTestClass::RayCollisionTestClass(const LineSegClass& ray, CastResultStruct* res, Collision_Type collision_type, bool ignore_translucent_meshes) :
	CollisionTestClass(res, collision_type),
	Ray(ray),
	IgnoreTranslucentMeshes(ignore_translucent_meshes)
{
}

inline RayCollisionTestClass::RayCollisionTestClass(const RayCollisionTestClass& raytest, const Matrix3D& tm) :
	CollisionTestClass(raytest),
	Ray(raytest.Ray, tm),
	IgnoreTranslucentMeshes(raytest.IgnoreTranslucentMeshes)
{
}

class SCRIPTS_API AABoxCollisionTestClass : public CollisionTestClass
{
public:
	AABoxCollisionTestClass(const AABoxClass & aabox,const Vector3 & move,CastResultStruct * res,Collision_Type collision_type = COLLISION_TYPE_0);
	AABoxCollisionTestClass(const AABoxCollisionTestClass & that);
	enum ROTATION_TYPE 
	{
		ROTATE_NONE = 0,
		ROTATE_Z90,
		ROTATE_Z180,
		ROTATE_Z270
	};
	bool							Cull(const Vector3 & min,const Vector3 & max);
	bool							Cull(const AABoxClass & box);
	void							Translate(const Vector3 & translation);
	void							Rotate(ROTATION_TYPE rotation);
	void							Transform(const Matrix3D & tm);
public:
	AABoxClass					Box; // 000C
	Vector3						Move; // 0024
	Vector3						SweepMin; // 0030
	Vector3						SweepMax; // 003C
private:
	AABoxCollisionTestClass & operator = (const AABoxCollisionTestClass &);
}; // 0048

inline void AABoxCollisionTestClass::Translate(const Vector3 & translation)
{
	Box.Center += translation;
	SweepMin += translation;
	SweepMax += translation;
}

inline bool AABoxCollisionTestClass::Cull(const Vector3 & min,const Vector3 & max) 
{
	if ((SweepMin.X > max.X) || (SweepMax.X < min.X))
	{
		return true;
	}
	if ((SweepMin.Y > max.Y) || (SweepMax.Y < min.Y))
	{
		return true;
	}
	if ((SweepMin.Z > max.Z) || (SweepMax.Z < min.Z))
	{
		return true;
	}
	return false;
}

class SCRIPTS_API OBBoxCollisionTestClass : public CollisionTestClass
{
public:
	OBBoxCollisionTestClass(const OBBoxClass & obbox,const Vector3 & move,CastResultStruct * res,Collision_Type type = COLLISION_TYPE_0);
	OBBoxCollisionTestClass(const OBBoxCollisionTestClass & that);
	OBBoxCollisionTestClass(const OBBoxCollisionTestClass & that,const Matrix3D & tm);
	OBBoxCollisionTestClass(const AABoxCollisionTestClass & that,const Matrix3D & tm);
	bool Cull(const Vector3 & min,const Vector3 & max);
	bool Cull(const AABoxClass & box);
public:
	OBBoxClass	 				Box;
	Vector3						Move;
	Vector3						SweepMin;
	Vector3						SweepMax;
private:
	OBBoxCollisionTestClass & operator = (const OBBoxCollisionTestClass &);
};

inline bool OBBoxCollisionTestClass::Cull(const Vector3 & min,const Vector3 & max) 
{
	if ((SweepMin.X > max.X) || (SweepMax.X < min.X))
	{
		return true;
	}
	if ((SweepMin.Y > max.Y) || (SweepMax.Y < min.Y))
	{
		return true;
	}
	if ((SweepMin.Z > max.Z) || (SweepMax.Z < min.Z))
	{
		return true;
	}
	return false;
}

#endif
