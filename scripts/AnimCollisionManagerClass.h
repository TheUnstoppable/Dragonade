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
#ifndef TT_INCLUDE__ANIMCOLLISIONMANAGERCLASS_H
#define TT_INCLUDE__ANIMCOLLISIONMANAGERCLASS_H

class PushRecordClass;
class PhysClass;
class AnimCollisionManagerDefClass;
class HAnimClass;
class ChunkSaveClass;
class ChunkLoadClass;
#include "RenderObjClass.h"
#include "Matrix3d.h"
#include "engine_vector.h"
#ifdef PARAM_EDITING_ON																														
#define	ANIMCOLLISIONMANAGERDEF_EDITABLE_PARAMS( class_name, member_name )												\
  	PARAM_SEPARATOR( class_name, "Animation Settings");																						\
	EnumParameterClass *collision_param = new EnumParameterClass(& member_name ## .CollisionMode);					\
	collision_param->Set_Name ("CollisionMode");																						\
	collision_param->Add_Value("NONE",AnimCollisionManagerClass::COLLIDE_NONE);											\
	collision_param->Add_Value("STOP",AnimCollisionManagerClass::COLLIDE_STOP);											\
	collision_param->Add_Value("PUSH",AnimCollisionManagerClass::COLLIDE_PUSH);											\
	collision_param->Add_Value("KILL",AnimCollisionManagerClass::COLLIDE_KILL);											\
	GENERIC_EDITABLE_PARAM( class_name , collision_param);																		\
	EnumParameterClass *anim_param = new EnumParameterClass(& member_name ## .AnimationMode);							\
	anim_param->Set_Name ("AnimationMode");																							\
	anim_param->Add_Value("LOOP",AnimCollisionManagerClass::ANIMATE_LOOP);													\
	anim_param->Add_Value("TARGET",AnimCollisionManagerClass::ANIMATE_TARGET);												\
	anim_param->Add_Value("MANUAL",AnimCollisionManagerClass::ANIMATE_MANUAL);												\
	GENERIC_EDITABLE_PARAM( class_name , anim_param);																				\
	NAMED_EDITABLE_PARAM( class_name, ParameterClass::TYPE_STRING, member_name ## .AnimationName,"AnimationName");
#else
#define	ANIMCOLLISIONMANAGERDEF_EDITABLE_PARAMS( class_name, member_name )															
#endif																																		
class AnimCollisionManagerDefClass
{
public:
	AnimCollisionManagerDefClass(void);
	~AnimCollisionManagerDefClass(void);
	void											Validate_Parameters(void);
	bool											Save(ChunkSaveClass &csave);
	bool											Load(ChunkLoadClass &cload);
public:
	int											CollisionMode;
	int											AnimationMode;
	StringClass									AnimationName;
};

class RiderManagerClass
{
public:
	RiderManagerClass(void);
	~RiderManagerClass(void);
	void						Link_Rider(PhysClass * obj);
	void						Unlink_Rider(PhysClass * obj);
	bool						Contains(PhysClass * obj);
	void						Move_Riders(const Matrix3D & delta,RenderObjClass * carrier_sub_obj = NULL);
	MultiListClass<PhysClass> * Get_Rider_List(void)	{ return &RiderList; }
protected:
	MultiListClass<PhysClass>	RiderList;
};
class AnimCollisionManagerClass
{
public:
	AnimCollisionManagerClass(PhysClass & parent);
	~AnimCollisionManagerClass(void);
	void											Init(const AnimCollisionManagerDefClass & def);
	void											Update_Cached_Model_Parameters(void);
	bool											Timestep(float dt);
	enum AnimModeType
	{
		ANIMATE_LOOP = 0,
		ANIMATE_TARGET,
		ANIMATE_MANUAL,
	};
	void											Set_Animation_Mode(AnimModeType mode);
	AnimModeType								Get_Animation_Mode(void);
	void											Set_Animation(const char * anim_name);
	HAnimClass *								Peek_Animation(void);
	void											Set_Target_Frame(float frame);
	void											Set_Target_Frame_End(void);
	float											Get_Target_Frame(void);
	bool											Is_At_Target(void);
	void											Set_Current_Frame(float frame);
	float											Get_Current_Frame(void);
	void											Set_Loop_Start(float frame0);
	void											Set_Loop_End(float frame1);
	float											Get_Loop_Start(void);
	float											Get_Loop_End(void);
	enum CollisionModeType
	{
		COLLIDE_NONE = 0,
		COLLIDE_STOP,
		COLLIDE_PUSH,
		COLLIDE_KILL,
	};
	void											Set_Collision_Mode(CollisionModeType mode);
	CollisionModeType							Get_Collision_Mode(void);
	bool											Link_Rider(PhysClass * rider)			{ RiderManager.Link_Rider(rider); return true; }
	bool											Unlink_Rider(PhysClass * rider)		{ RiderManager.Unlink_Rider(rider); return true; }
	bool											Save(ChunkSaveClass &csave);
	bool											Load(ChunkLoadClass &cload);
protected:
	class CollideableObjClass
	{
	public:
		CollideableObjClass(void);
		CollideableObjClass(RenderObjClass * collisionmesh);
		CollideableObjClass(const CollideableObjClass & that);
		~CollideableObjClass(void)
		{
			REF_PTR_RELEASE(CollisionMesh);
		}
		CollideableObjClass &				operator = (const CollideableObjClass & that);
		bool										operator == (const CollideableObjClass & that) const { return false; }
		bool										operator != (const CollideableObjClass & that) const { return true; }		
		void										Set_Collision_Object(RenderObjClass * mesh);
		RenderObjClass *						Peek_Collision_Object(void)						{ return CollisionMesh; }
		void										Cache_Start_Transform(void)						{ StartTransform = CollisionMesh->Get_Transform(); }
		void										Cache_End_Transform(void)							{ EndTransform = CollisionMesh->Get_Transform(); }
		Matrix3D &								Get_Start_Transform(void)							{ return StartTransform; }
		Matrix3D &								Get_End_Transform(void)								{ return EndTransform; }	
		void										Install_Start_Transform(void)						{ CollisionMesh->Set_Transform(StartTransform); }
		void										Install_End_Transform(void)						{ CollisionMesh->Set_Transform(EndTransform); }
		void										Intersect_Scene(int colgroup,MultiListClass<PhysClass> * list);
		int										Clear_Collision_Bits(void);
		void										Restore_Collision_Bits(int oldbits);
	protected:
		RenderObjClass *						CollisionMesh;
		Matrix3D									StartTransform;
		Matrix3D									EndTransform;
	};
	bool											Is_Collision_Model(RenderObjClass * model);
	int											Recursive_Count_Collision_Models(RenderObjClass * model);
	void											Recursive_Collect_Collision_Models(RenderObjClass * model);
	void											Revert_Animation_State(void);
	bool											Check_Collision(CollideableObjClass & collisionobj);
	bool											Is_Intersecting(void);
	bool											Push_Collided_Object(PhysClass * obj,const Matrix3D & delta_transform);
	void											Internal_Set_Animation(const char * anim_name);
	PhysClass &									Parent;
	AnimModeType								AnimationMode;		// current animation mode and parameters.
	float											TargetFrame;		
	float											LoopStart;
	float											LoopEnd;
	HAnimClass *								CurAnimation;		// current animation state.
	float											CurFrame;
	HAnimClass *								PrevAnimation;		// temporary variable used in timestep
	float											PrevFrame;			// temporary variable used in timestep
	RiderManagerClass							RiderManager;
	CollisionModeType							CollisionMode;
	DynamicVectorClass<CollideableObjClass>	CollisionMeshes;
	PushRecordClass *							PushList;
};
#endif
