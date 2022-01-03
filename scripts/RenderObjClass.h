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
#ifndef TT_INCLUDE__RENDEROBJCLASS_H
#define TT_INCLUDE__RENDEROBJCLASS_H

#include "Matrix3D.h"
#include "Vector3.h"
#include "AABoxClass.h"
#include "SphereClass.h"
#include "engine_vector.h"
#include "Persist.h"
#include "engine_string.h"

class AABoxClass;
class AABoxCollisionTestClass;
class AABoxIntersectionTestClass;
class CameraClass;
class DecalGeneratorClass;
class HAnimClass;
class HAnimComboClass;
class HTreeClass;
class IntersectionClass;
class IntersectionResultClass;
class OBBoxCollisionTestClass;
class OBBoxIntersectionTestClass;
class RayCollisionTestClass;
class RenderInfoClass;
class SceneClass;
class SpecialRenderInfoClass;
class SphereClass;
class MaterialInfoClass;
class RenderObjClass;

class RenderObjClass : public RefCountClass, public PersistClass, public MultiListObjectClass
{


protected:
   mutable unsigned long Bits; // 0018  0030
   Matrix3D        Transform; // 001C  0034
   mutable SphereClass     CachedBoundingSphere; // 004C  0064
   mutable AABoxClass      CachedBoundingBox; // 005C  0074
   float           NativeScreenSize; // 0074  008C
   mutable bool            IsTransformIdentity; // 0078  0090
   SceneClass*     Scene; // 007C  0094
   RenderObjClass* Container; // 0080  0098
   void*           User_Data; // 0084  009C


public:

   enum AnimMode {
      ANIM_MODE_MANUAL = 0,
      ANIM_MODE_LOOP,
      ANIM_MODE_ONCE,
   };
   enum {
      COLLISION_TYPE_MASK = 0xff,
	  IS_VISIBLE = 0x100,
	  IS_NOT_HIDDEN = 0x200,
	  IS_NOT_ANIMATION_HIDDEN = 0x400,
	  IS_FORCE_VISIBLE = 0x800,
	  BOUNDING_VOLUMES_VALID = 0x2000,
	  IS_TRANSLUCENT = 0x4000,
	  SUBOBJS_MATCH_LOD = 0x10000,
	  SUBOBJ_TRANSFORMS_DIRTY = 0x20000,
	  HAS_USER_LIGHTING = 0x40000,
	  IS_REALLY_VISIBLE = 0x700,
	  IS_NOT_HIDDEN_AT_ALL = 0x600,
	  DEFAULT_BITS = 0x601,
   };
	
	enum 
	{
		CLASSID_UNKNOWN = 0xFFFFFFFF,
		CLASSID_MESH = 0,
		CLASSID_HMODEL,
		CLASSID_DISTLOD,
		CLASSID_PREDLODGROUP,
		CLASSID_TILEMAP,
		CLASSID_IMAGE3D, // Obsolete
		CLASSID_LINE3D,
		CLASSID_BITMAP2D, // Obsolete
		CLASSID_CAMERA,
		CLASSID_DYNAMESH,
		CLASSID_DYNASCREENMESH,
		CLASSID_TEXTDRAW,
		CLASSID_FOG,
		CLASSID_LAYERFOG,		
		CLASSID_LIGHT,
		CLASSID_PARTICLEEMITTER,
		CLASSID_PARTICLEBUFFER,
		CLASSID_SCREENPOINTGROUP,
		CLASSID_VIEWPOINTGROUP,
		CLASSID_WORLDPOINTGROUP,
		CLASSID_TEXT2D,
		CLASSID_TEXT3D,
		CLASSID_NULL,
		CLASSID_COLLECTION,
		CLASSID_FLARE,
		CLASSID_HLOD,
		CLASSID_AABOX,
		CLASSID_OBBOX,
		CLASSID_SEGLINE,
		CLASSID_SPHERE,
		CLASSID_RING,
		CLASSID_BOUNDFOG,
		CLASSID_DAZZLE,
		CLASSID_SOUND,
		CLASSID_SEGLINETRAIL,
		CLASSID_LAND,
		CLASSID_RENEGADE_TERRAIN,
		// New classes!
		CLASSID_USERCLIPPLANECAMERA,
		CLASSID_PLANEREFLECTIONCAMERA,
		CLASSID_SCENEUI,
		CLASSID_DEBUGVECTOR,
		CLASSID_LAST					= 0x0000FFFF
	};

			RenderObjClass();
			RenderObjClass(const RenderObjClass& src);
			RenderObjClass& operator=(const RenderObjClass& that);
   virtual ~RenderObjClass() {}

   virtual RenderObjClass*      Clone                             () const = 0;
   virtual int                  Class_ID                          () const { return CLASSID_UNKNOWN; }
   virtual const char*          Get_Name                          () const { return "UNNAMED"; }
   virtual void                 Set_Name                          (const char*) { }
   virtual const char*          Get_Base_Model_Name               () const { return NULL; }
   virtual void                 Set_Base_Model_Name               (const char*) { }
   virtual int                  Get_Num_Polys                     () const { return 0; }
   virtual void                 Render                            (RenderInfoClass&) = 0;
   virtual void                 Special_Render                    (SpecialRenderInfoClass&) { }
   virtual void                 On_Frame_Update                   () { }
   virtual void                 Restart                           () { }
   virtual void                 Add                               (SceneClass*);
   virtual void                 Remove                            ();
   virtual SceneClass*          Get_Scene                         ();
   virtual SceneClass*          Peek_Scene                        () { return Scene; }
   virtual void                 Set_Container                     (RenderObjClass*);
   virtual RenderObjClass*      Get_Container                     () const;
   virtual void                 Validate_Transform                () const;
   virtual void                 Set_Transform                     (const Matrix3D&);
   virtual void                 Set_Position                      (const Vector3&);
   const Matrix3D &				Get_Transform(void) const;
   const Matrix3D &				Get_Transform(bool& is_transform_identity) const;
   const Matrix3D &				Get_Transform_No_Validity_Check(void) const;
   const Matrix3D &				Get_Transform_No_Validity_Check(bool& is_transform_identity) const;
   bool							Is_Transform_Identity() const;
   bool							Is_Transform_Identity_No_Validity_Check() const;
   Vector3						Get_Position(void) const;
   virtual void                 Notify_Added                      (SceneClass*);
   virtual void                 Notify_Removed                    (SceneClass*);
   virtual int                  Get_Num_Sub_Objects               () const { return 0; }
   virtual RenderObjClass*      Get_Sub_Object                    (int index) const { return NULL; }
   virtual int                  Add_Sub_Object                    (RenderObjClass*) { return 0; }
   virtual int                  Remove_Sub_Object                 (RenderObjClass*) { return 0; }
   virtual RenderObjClass*      Get_Sub_Object_By_Name            (const char* name) const;
   virtual int                  Get_Num_Sub_Objects_On_Bone       (int boneIndex) const { return 0; }
   virtual RenderObjClass*      Get_Sub_Object_On_Bone            (int index, int boneIndex) const { return NULL; }
   virtual int                  Get_Sub_Object_Bone_Index         (RenderObjClass* object) const { return 0; }
   virtual int                  Add_Sub_Object_To_Bone            (RenderObjClass*, int boneIndex) { return 0; }
   virtual int                  Add_Sub_Object_To_Bone            (RenderObjClass*, const char*);
   virtual int                  Remove_Sub_Objects_From_Bone      (const char* boneName);
   virtual void                 Update_Sub_Object_Transforms      ();
   virtual void                 Set_Animation                     () { }
   virtual void                 Set_Animation                     (HAnimClass*, float, int anim_mode = ANIM_MODE_MANUAL) { }
   virtual void                 Set_Animation                     (HAnimClass*, float, HAnimClass*, float, float) { }
   virtual void                 Set_Animation                     (HAnimComboClass*) { }
   virtual HAnimClass*          Peek_Animation                    () { return NULL; }
   virtual int                  Get_Num_Bones                     () { return 0; }
   virtual const char*          Get_Bone_Name                     (int boneIndex) { return NULL; }
   virtual int                  Get_Bone_Index                    (const char* boneName) { return 0; }
   virtual const Matrix3D&      Get_Bone_Transform                (const char* boneName) { return Get_Transform(); }
   virtual const Matrix3D&      Get_Bone_Transform                (int boneIndex) { return Get_Transform(); }
   virtual void                 Capture_Bone                      (int boneIndex) { }
   virtual void                 Release_Bone                      (int boneIndex) { }
   virtual bool                 Is_Bone_Captured                  (int boneIndex) const { return false; }
   virtual void                 Control_Bone                      (int boneIndex, const Matrix3D&, bool b = false) { }
   virtual const HTreeClass *	Get_HTree(void) const														{ return NULL; }
   virtual bool                 Cast_Ray                          (RayCollisionTestClass&) { return false; }
   virtual bool                 Cast_AABox                        (AABoxCollisionTestClass&) { return false; }
   virtual bool                 Cast_OBBox                        (OBBoxCollisionTestClass&) { return false; }
   virtual bool                 Intersect_AABox                   (AABoxIntersectionTestClass&) { return false; }
   virtual bool                 Intersect_OBBox                   (OBBoxIntersectionTestClass&) { return false; }
   virtual bool                 Intersect                         (IntersectionClass*, IntersectionResultClass*);
   virtual bool                 Intersect_Sphere                  (IntersectionClass*, IntersectionResultClass*);
   virtual bool                 Intersect_Sphere_Quick            (IntersectionClass*, IntersectionResultClass*);
   virtual const SphereClass&   Get_Bounding_Sphere               () const;
   virtual const AABoxClass&    Get_Bounding_Box                  () const;
   virtual void                 Get_Obj_Space_Bounding_Sphere     (SphereClass&) const;
   virtual void                 Get_Obj_Space_Bounding_Box        (AABoxClass&) const;
   virtual void                 Update_Obj_Space_Bounding_Volumes () { }
   static const float AT_MIN_LOD;
   static const float AT_MAX_LOD;
   virtual void                 Prepare_LOD                       (CameraClass&);
   virtual void                 Recalculate_Static_LOD_Factors    () { }
   virtual void                 Increment_LOD                     () { }
   virtual void                 Decrement_LOD                     () { }
   virtual float                Get_Cost                          () const;
   virtual float                Get_Value                         () const { return AT_MIN_LOD; }
   virtual float                Get_Post_Increment_Value          () const { return AT_MAX_LOD; }
   virtual void                 Set_LOD_Level                     (int) { }
   virtual int                  Get_LOD_Level                     () const { return 0; }
   virtual int                  Get_LOD_Count                     () const { return 1; }
   virtual void                 Set_LOD_Bias                      (float) { }
   virtual int                  Calculate_Cost_Value_Arrays       (float, float*, float*) const;
   virtual RenderObjClass *     Get_Current_LOD                   () { Add_Ref(); return this; }
   virtual bool					Build_Dependency_List (DynamicVectorClass<StringClass> &file_list, bool recursive=true);
   virtual bool					Build_Texture_List (DynamicVectorClass<StringClass> &texture_file_list, bool recursive=true);
   virtual void                 Create_Decal                      (DecalGeneratorClass*) { }
   virtual void                 Delete_Decal                      (uint32) { }
   virtual MaterialInfoClass*   Get_Material_Info                 () { return NULL; }
   virtual void                 Set_User_Data                     (void* value, bool) { User_Data = value; };
   virtual void*                Get_User_Data                     () { return User_Data; };
   virtual int                  Get_Num_Snap_Points               () { return 0; }
   virtual void                 Get_Snap_Point                    (int, Vector3*) { }
   virtual float                Get_Screen_Size                   (CameraClass&);
   virtual void                 Scale                             (float) { };
   virtual void                 Scale                             (float, float, float) { };
   virtual int                  Get_Sort_Level                    () const { return 0;}
   virtual void                 Set_Sort_Level                    (int) { }
   virtual int                  Is_Really_Visible                 () { return ((Bits & IS_REALLY_VISIBLE) == IS_REALLY_VISIBLE); }
   virtual int                  Is_Not_Hidden_At_All              () { return ((Bits & IS_NOT_HIDDEN_AT_ALL) == IS_NOT_HIDDEN_AT_ALL); }
   virtual int                  Is_Visible                        () const { return (Bits & IS_VISIBLE); }
   virtual void                 Set_Visible                       (int onoff) { if (onoff) { Bits |= IS_VISIBLE; } else { Bits &= ~IS_VISIBLE; } }
   virtual int                  Is_Hidden                         () const { return !(Bits & IS_NOT_HIDDEN); }
   virtual void                 Set_Hidden                        (int onoff) { if (onoff) { Bits &= ~IS_NOT_HIDDEN; } else { Bits |= IS_NOT_HIDDEN; } }
   virtual int                  Is_Animation_Hidden               () const { return !(Bits & IS_NOT_ANIMATION_HIDDEN); }
   virtual void                 Set_Animation_Hidden              (int onoff) { if (onoff) { Bits &= ~IS_NOT_ANIMATION_HIDDEN; } else { Bits |= IS_NOT_ANIMATION_HIDDEN; } }
   virtual int                  Is_Force_Visible                  () const { return Bits & IS_FORCE_VISIBLE; }
   virtual void                 Set_Force_Visible                 (int onoff) { if (onoff) { Bits |= IS_FORCE_VISIBLE; } else { Bits &= ~IS_FORCE_VISIBLE; } }
   virtual int                  Has_User_Lighting                 () const { return Bits & HAS_USER_LIGHTING; }
   virtual void                 Set_Has_User_Lighting             (bool onoff) { if (onoff) { Bits |= HAS_USER_LIGHTING; } else { Bits &= ~HAS_USER_LIGHTING; } }
   virtual int                  Is_Translucent                    () const { return Bits & IS_TRANSLUCENT; }
   virtual void                 Set_Translucent                   (int onoff) { if (onoff) { Bits |= IS_TRANSLUCENT; } else { Bits &= ~IS_TRANSLUCENT; } }
   virtual int                  Get_Collision_Type                () const { return (Bits & COLLISION_TYPE_MASK); }
   virtual void                 Set_Collision_Type                (int type) { Bits &= ~COLLISION_TYPE_MASK; Bits |= (type & COLLISION_TYPE_MASK) | 1; }
   virtual bool                 Is_Complete                       () { return false; }
   virtual bool                 Is_In_Scene                       () { return Scene != NULL; }
   virtual float                Get_Native_Screen_Size            () const { return NativeScreenSize; }
   virtual void                 Set_Native_Screen_Size            (float screensize) { NativeScreenSize = screensize; }
   virtual const PersistFactoryClass& Get_Factory                 () const;
   virtual bool                 Save                              (ChunkSaveClass& oSave);
   virtual bool                 Load                              (ChunkLoadClass& oLoad);
   virtual void                 Save_User_Lighting                (ChunkSaveClass&);
   virtual void                 Load_User_Lighting                (ChunkLoadClass&);
   virtual void					Add_Dependencies_To_List (DynamicVectorClass<StringClass> &file_list, bool textures_only = false);
   virtual void                 Update_Cached_Bounding_Volumes    () const;
   virtual void                 Update_Sub_Object_Bits            ();
   void							Set_Sub_Objects_Match_LOD(int onoff)									{ if (onoff) { Bits |= SUBOBJS_MATCH_LOD; } else { Bits &= ~SUBOBJS_MATCH_LOD; } }
   int							Is_Sub_Objects_Match_LOD_Enabled(void)									{ return Bits & SUBOBJS_MATCH_LOD; }
   void							Set_Sub_Object_Transforms_Dirty(bool onoff)							{ if (onoff) { Bits |= SUBOBJ_TRANSFORMS_DIRTY; } else { Bits &= ~SUBOBJ_TRANSFORMS_DIRTY; } }
   bool							Are_Sub_Object_Transforms_Dirty(void);
   bool							Bounding_Volumes_Valid(void) const;
   void							Invalidate_Cached_Bounding_Volumes(void) const						{ Bits &= ~BOUNDING_VOLUMES_VALID; }
   void							Validate_Cached_Bounding_Volumes(void)	const							{ Bits |= BOUNDING_VOLUMES_VALID; }
   void							Save_Sub_Object_User_Lighting(ChunkSaveClass & csave,RenderObjClass * sub_obj,int bone_index);
   void							Load_Sub_Object_User_Lighting(ChunkLoadClass & cload);
   void Set_User_Lighting_Flag(); //Set the HAS_USER_LIGHTING flag appropriatly for this renderobj, intended to be called during vertex solve.

   void             Register_For_Rendering(bool hint_static = false);

}; // 0088  00A0

TT_INLINE const SphereClass & RenderObjClass::Get_Bounding_Sphere(void) const
{
	if (!Bounding_Volumes_Valid()) {
		Update_Cached_Bounding_Volumes();
	} 
	return CachedBoundingSphere;
}
TT_INLINE const AABoxClass & RenderObjClass::Get_Bounding_Box(void) const
{
	if (!Bounding_Volumes_Valid()) {
		Update_Cached_Bounding_Volumes();
	}
	return CachedBoundingBox;
}
TT_INLINE float Bound_Degrees(float angle)
{
	while (angle > 359) angle -= 360;
	while (angle < 0) angle += 360;
	return angle;
}
TT_INLINE const Matrix3D & RenderObjClass::Get_Transform(void) const
{
	Validate_Transform();
	return Transform;
}
TT_INLINE const Matrix3D & RenderObjClass::Get_Transform(bool &is_transform_identity) const
{
	Validate_Transform();
	is_transform_identity=IsTransformIdentity;
	return Transform;
}
TT_INLINE bool RenderObjClass::Is_Transform_Identity() const
{
	Validate_Transform();
	return IsTransformIdentity;
}
TT_INLINE const Matrix3D & RenderObjClass::Get_Transform_No_Validity_Check(void) const
{
	return Transform;
}
TT_INLINE const Matrix3D & RenderObjClass::Get_Transform_No_Validity_Check(bool& is_transform_identity) const
{
	is_transform_identity=IsTransformIdentity;
	return Transform;
}
TT_INLINE bool RenderObjClass::Is_Transform_Identity_No_Validity_Check() const
{
	return IsTransformIdentity;
}

typedef RefMultiListClass<RenderObjClass>		RefRenderObjListClass;
typedef RefMultiListIterator<RenderObjClass>	RefRenderObjListIterator;

typedef MultiListClass<RenderObjClass>			NonRefRenderObjListClass;
typedef MultiListIterator<RenderObjClass>		NonRefRenderObjListIterator;

#endif
