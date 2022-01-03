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
#ifndef TT_INCLUDE_BOXRENDEROBJCLASS_H
#define TT_INCLUDE_BOXRENDEROBJCLASS_H
#include "RenderObjClass.h"
#include "w3d.h"
#include "obboxclass.h"
#include "AABoxClass.h"
#include "prototypeclass.h"
class BoxRenderObjClass : public RenderObjClass
{
protected:
	static bool IsInitted;
	static int DisplayMask;
	char Name[32];
	Vector3 Color;
	Vector3 ObjSpaceCenter;
	Vector3 ObjSpaceExtent;
	float Opacity;
public:
	static void Init();
	static void Shutdown();
	static void Set_Box_Display_Mask(int mask);
	static int Get_Box_Display_Mask();
	BoxRenderObjClass(const BoxRenderObjClass& src);
	BoxRenderObjClass(const W3dBoxStruct& src);
	BoxRenderObjClass();
	BoxRenderObjClass& operator=(const BoxRenderObjClass& that);
	virtual const char*          Get_Name                          () const { return Name; }
	virtual void                 Set_Name                          (const char *name)
	{
		strcpy_s(Name,name);
	}
	virtual int                  Get_Num_Polys                     () const { return 0xC; }
	virtual void update_cached_box() = 0;
	void Set_Color(const Vector3 &color);
	void render_box(RenderInfoClass& rinfo,const Vector3 &center,const Vector3 &extent);
	void vis_render_box(RenderInfoClass& rinfo,const Vector3 &center,const Vector3 &extent);	
	void Set_Opacity(float opacity) {Opacity = opacity;}
	const Vector3 &Get_Local_Center() {return ObjSpaceCenter;}
	const Vector3 &Get_Local_Extent() {return ObjSpaceExtent;}
	void Set_Local_Center_Extent(const Vector3 &center,const Vector3 &extent)
	{
		ObjSpaceCenter = center;
		ObjSpaceExtent = extent;
		update_cached_box();
	}
};
class OBBoxRenderObjClass : public BoxRenderObjClass
{
public:
	OBBoxClass CachedBox;
	OBBoxRenderObjClass(const OBBoxRenderObjClass& src);
	OBBoxRenderObjClass(const W3dBoxStruct& src);
	OBBoxRenderObjClass();
	OBBoxRenderObjClass& operator=(const OBBoxRenderObjClass& that);
	OBBoxRenderObjClass(const OBBoxClass& src);
	virtual RenderObjClass*      Clone                             () const;
	virtual int                  Class_ID                          () const;
	virtual void                 Render                            (RenderInfoClass& rinfo);
	virtual void                 Special_Render                    (SpecialRenderInfoClass& rinfo);
	virtual void                 Set_Transform                     (const Matrix3D& tm);
	virtual void                 Set_Position                      (const Vector3& pos);
	virtual bool                 Cast_Ray                          (RayCollisionTestClass& col);
	virtual bool                 Cast_AABox                        (AABoxCollisionTestClass& col);
	virtual bool                 Cast_OBBox                        (OBBoxCollisionTestClass& col);
	virtual bool                 Intersect_AABox                   (AABoxIntersectionTestClass& col);
	virtual bool                 Intersect_OBBox                   (OBBoxIntersectionTestClass& col);
	virtual void                 Get_Obj_Space_Bounding_Sphere     (SphereClass& sphere) const;
	virtual void                 Get_Obj_Space_Bounding_Box        (AABoxClass& box) const;
	virtual void update_cached_box();
	OBBoxClass &Get_Box()
	{
		Validate_Transform();
		update_cached_box();
		return CachedBox;
	}
};
class AABoxRenderObjClass : public BoxRenderObjClass {
public:
	AABoxClass CachedBox;
	AABoxRenderObjClass(const AABoxRenderObjClass& src);
	AABoxRenderObjClass(const W3dBoxStruct& src);
	AABoxRenderObjClass();
	AABoxRenderObjClass& operator=(const AABoxRenderObjClass& that);
	AABoxRenderObjClass(const AABoxClass& src);
	virtual RenderObjClass*      Clone                             () const;
	virtual int                  Class_ID                          () const;
	virtual void                 Render                            (RenderInfoClass& rinfo);
	virtual void                 Special_Render                    (SpecialRenderInfoClass& rinfo);
	virtual void                 Set_Transform                     (const Matrix3D& tm);
	virtual void                 Set_Position                      (const Vector3& pos);
	virtual bool                 Cast_Ray                          (RayCollisionTestClass& col);
	virtual bool                 Cast_AABox                        (AABoxCollisionTestClass& col);
	virtual bool                 Cast_OBBox                        (OBBoxCollisionTestClass& col);
	virtual bool                 Intersect_AABox                   (AABoxIntersectionTestClass& col);
	virtual bool                 Intersect_OBBox                   (OBBoxIntersectionTestClass& col);
	virtual void                 Get_Obj_Space_Bounding_Sphere     (SphereClass& sphere) const;
	virtual void                 Get_Obj_Space_Bounding_Box        (AABoxClass& box) const;
	virtual void update_cached_box();
	AABoxClass &Get_Box()
	{
		Validate_Transform();
		update_cached_box();
		return CachedBox;
	}
};
class BoxLoaderClass : public PrototypeLoaderClass
{
public:
	virtual int Chunk_Type(void)
	{
		return 0x740;
	}
	virtual PrototypeClass *Load_W3D(ChunkLoadClass & cload);
};
extern BoxLoaderClass _BoxLoader;
#endif