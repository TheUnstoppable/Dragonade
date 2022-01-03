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
#ifndef TT_INCLUDE__SCENECLASS_H
#define TT_INCLUDE__SCENECLASS_H



#include "scripts.h"
#include "engine_common.h"
#include "engine_vector.h"
#include "RenderObjClass.h"

class SceneIterator {
public:
	virtual ~SceneIterator() {};
	virtual void First() = 0;
	virtual void Next() = 0;
	virtual bool Is_Done() = 0;
	virtual RenderObjClass *Current_Item() = 0;
};
class RenderInfoClass;
class SceneClass : public RefCountClass
{
public:
   enum RegType {
      ON_FRAME_UPDATE = 0x0,
      LIGHT = 0x1,
      RELEASE = 0x2,
   };
   enum PolyRenderType {
      POINT = 0x0,
      LINE = 0x1,
      FILL = 0x2,
   };
   enum ExtraPassPolyRenderType {
      EXTRA_PASS_DISABLE = 0x0,
      EXTRA_PASS_LINE = 0x1,
      EXTRA_PASS_CLEAR_LINE = 0x2,
   };
private:
   Vector3 AmbientLight; //  8  32
   PolyRenderType PolyRenderMode; // 20 44
   ExtraPassPolyRenderType ExtraPassPolyRenderMode; // 24 48
   bool    FogEnabled; // 28 52
   Vector3 FogColor; // 32 56
   float   FogStart; // 44 68
   float   FogEnd; // 48 72


public:
   virtual ~SceneClass();
   virtual void Add_Render_Object(RenderObjClass* obj);
   virtual void Remove_Render_Object(RenderObjClass* obj);
   virtual SceneIterator *Create_Iterator(bool onlyvisible) = 0;
   virtual void Destroy_Iterator(SceneIterator* it) = 0;
   virtual void Set_Ambient_Light(const Vector3& color);
   virtual const Vector3& Get_Ambient_Light();
   virtual void Set_Fog_Enable(bool set);
   virtual bool Get_Fog_Enable();
   virtual void Set_Fog_Color(const Vector3& color);
   virtual Vector3& Get_Fog_Color();
   virtual void Set_Fog_Range(float start,float end);
   virtual void Get_Fog_Range(float* start,float* end);
   virtual void Register(class RenderObjClass* obj, RegType for_what) = 0;
   virtual void Unregister(class RenderObjClass* obj, RegType for_what) = 0;
   virtual float Compute_Point_Visibility(RenderInfoClass& rinfo,const Vector3& point);
   virtual void Save(ChunkSaveClass& csave);
   virtual void Load(ChunkLoadClass& cload);
   virtual void Customized_Render(RenderInfoClass& rinfo) = 0;
   virtual void Pre_Render_Processing(RenderInfoClass& rinfo);
   virtual void Post_Render_Processing(RenderInfoClass& rinfo);
   void Render(RenderInfoClass& rinfo);
   SceneClass();
   PolyRenderType Get_Polygon_Mode()
   {
	   return PolyRenderMode;
   }
};


#endif
