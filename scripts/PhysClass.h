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
#ifndef TT_INCLUDE__PHYSCLASS_H
#define TT_INCLUDE__PHYSCLASS_H

#include "CullableClass.h"
#include "engine_string.h"
#include "PhysObserverClass.h"
#include "RenderObjClass.h"
#include "MaterialEffectClass.h"
#include "PhysDefClass.h"
class CollisionEventClass;
class CombatPhysObserverClass;
class ElevatorPhysClass;
class HumanPhysClass;
class LightEnvironmentClass;
class MoveablePhysClass;
class Phys3Class;
class PhysAABoxCollisionTestClass;
class PhysAABoxIntersectionTestClass;
class PhysDefClass;
class PhysMeshIntersectionTestClass;
class PhysOBBoxCollisionTestClass;
class PhysOBBoxIntersectionTestClass;
class PhysRayCollisionTestClass;
class ProjectileClass;
class RenderInfoClass;
class RenderObjClass;
class RigidBodyClass;
class SpecialRenderInfoClass;
class StaticAnimPhysClass;
class VehiclePhysClass;
class DynamicPhysClass;
class MotorVehicleClass;
class WheeledVehicleClass;
class MotorcycleClass;
class TrackedVehicleClass;
class VTOLVehicleClass;
class StaticPhysClass;
class DamageableStaticPhysClass;
class DoorPhysClass;
class DecorationPhysClass;
class TimedDecorationPhysClass;
class DynamicAnimPhysClass;
class LightPhysClass;
class RenderObjPhysClass;
class AccessiblePhysClass;
class LightEnvironmentClass;
class Matrix3D;

class PhysClass : public CullableClass, public PersistClass, public MultiListObjectClass
{
public:
	PhysClass(void);
	virtual ~PhysClass(void);
	void		Init(const PhysDefClass & def);
	virtual void					 Definition_Changed(void)					{ }
	bool								Expire(void);
	virtual bool					Needs_Timestep(void)							{ return false; }
	virtual void					Timestep(float dt)							= 0; 
	virtual void					Post_Timestep_Process(void)				{ }
	virtual const Matrix3D &	Get_Transform(void) const					= 0;
	virtual void					Set_Transform(const Matrix3D & m)		= 0;
	void								Get_Position(Vector3 * set_pos) const	{ Get_Transform().Get_Translation(set_pos); }
	void								Set_Position(const Vector3 & pos)		{ Matrix3D tm = Get_Transform(); tm.Set_Translation(pos); Set_Transform(tm); }
	float								Get_Facing(void) const						{ return Get_Transform().Get_Z_Rotation(); }
	void								Set_Facing(float new_facing);
	virtual bool					Cast_Ray(PhysRayCollisionTestClass & raytest)		{ return false; }
	virtual bool					Cast_AABox(PhysAABoxCollisionTestClass & boxtest)	{ return false; }
	virtual bool					Cast_OBBox(PhysOBBoxCollisionTestClass & boxtest)	{ return false; }
	virtual bool					Intersection_Test(PhysAABoxIntersectionTestClass & test)		{ return false; }
	virtual bool					Intersection_Test(PhysOBBoxIntersectionTestClass & test)		{ return false; }
	virtual bool					Intersection_Test(PhysMeshIntersectionTestClass & test)		{ return false; }
	virtual void					Link_To_Carrier(PhysClass * carrier,RenderObjClass * carrier_sub_obj = NULL)	{ }
	virtual RenderObjClass *	Peek_Carrier_Sub_Object(void)											{ return NULL; }
	virtual bool					Push(const Vector3 & move)												{ return false; }
	virtual bool					Internal_Link_Rider(PhysClass * rider)								{ return false; }
	virtual bool					Internal_Unlink_Rider(PhysClass * rider)							{ return false; }
	void								Update_Cull_Box(void);
	virtual void					Set_Model(RenderObjClass * model);
	void								Set_Model_By_Name(const char * model_type_name);
	RenderObjClass *				Get_Model(void);
	RenderObjClass *	Peek_Model(void) { return Model; }
	void								Set_Name(const char * name);
	const char *					Get_Name(void);
	uint32							Get_ID(void) const											{ return InstanceID; }
	void								Set_ID(uint32 id)												{ InstanceID = id; }
	void								Set_Vis_Object_ID(int new_id)								{ VisObjectID = new_id; }
	virtual int						Get_Vis_Object_ID(void)										{ return VisObjectID; }
	virtual void					Render(RenderInfoClass & rinfo);
	virtual void					Vis_Render(SpecialRenderInfoClass & rinfo);
	void								Invalidate_Static_Lighting_Cache(void);
	LightEnvironmentClass *		Get_Static_Lighting_Environment(void);
	virtual void					Render_Vis_Meshes(RenderInfoClass & rinfo)			{ }
	virtual void					Get_Shadow_Blob_Box(AABoxClass * set_obj_space_box);
	virtual bool					Is_Casting_Shadow(void)										{ return false; }
	void								Add_Effect_To_Me(MaterialEffectClass * effect);
	void								Remove_Effect_From_Me(MaterialEffectClass * effect);
	bool								Do_Any_Effects_Suppress_Shadows(void);
	void								Set_Visibility_Mode(Visibility_Mode_Type type)			{ Flags &= ~VISIBILITY_MODE_MASK; Flags |= ((uint)type << VISIBILITY_MODE_SHIFT) & VISIBILITY_MODE_MASK; }
	Visibility_Mode_Type				Get_Visibility_Mode() const								{ return (Visibility_Mode_Type)((Flags & VISIBILITY_MODE_MASK) >> VISIBILITY_MODE_SHIFT); }
	void								Set_Collision_Group(Collision_Group_Type group)				{ Flags &= ~(uint)COLLISION_MASK; Flags |= (uint)group & COLLISION_MASK; }
	Collision_Group_Type				Get_Collision_Group(void) const							{ return (Collision_Group_Type)(Flags & COLLISION_MASK); }
	void								Inc_Ignore_Counter(void);
	void								Dec_Ignore_Counter(void);
	bool								Is_Ignore_Me(void) const									{ return ((Flags & IGNORE_MASK) > 0); }
	void								Set_Immovable(bool onoff)									{ Set_Flag(IMMOVABLE,onoff); }
	bool								Is_Immovable(void) const									{ return Get_Flag(IMMOVABLE); }
	void								Set_Disabled(bool onoff)									{ Set_Flag(DISABLED,onoff); }
	bool								Is_Disabled(void) const										{ return Get_Flag(DISABLED); }
	void								Enable_Debug_Display(bool onoff)							{ (onoff ? Flags |= DEBUGDISPLAY : Flags &= ~DEBUGDISPLAY); }
	bool								Is_Debug_Display_Enabled(void) const;
	void								Enable_User_Control(bool onoff)							{ Set_Flag(USERCONTROL,onoff); Set_Flag(ASLEEP,false); }
	bool								Is_User_Control_Enabled(void) const						{ return Get_Flag(USERCONTROL); }
	void								Enable_Shadow_Generation(bool onoff)					{ Set_Flag(CASTSHADOW,onoff); }
	bool								Is_Shadow_Generation_Enabled(void) const				{ return Get_Flag(CASTSHADOW); }
	void								Enable_Force_Projection_Shadow(bool onoff)			{ Set_Flag(FORCE_PROJECTION_SHADOW,onoff); }
	bool								Is_Force_Projection_Shadow_Enabled(void) const		{ return Get_Flag(FORCE_PROJECTION_SHADOW); }
	void								Enable_Dont_Save(bool onoff)								{ Set_Flag(DONT_SAVE,onoff); }
	bool								Is_Dont_Save_Enabled(void) const							{ return Get_Flag(DONT_SAVE); }
	bool								Is_Asleep(void) const										{ return ((Flags & ASLEEP) == ASLEEP); }
	void								Force_Awake(void)												{ Set_Flag(ASLEEP,false); }
	void								Enable_Is_World_Space_Mesh(bool onoff)					{ Set_Flag(IS_WS_MESH,onoff); }
	bool								Is_World_Space_Mesh(void)									{ return Get_Flag(IS_WS_MESH); }
	void								Enable_Is_Pre_Lit(bool onoff)								{ Set_Flag(IS_PRE_LIT,onoff); }
	bool								Is_Pre_Lit(void); //												{ return Get_Flag(IS_PRE_LIT) | Model->Get_F; }
	void								Enable_Is_In_The_Sun(bool onoff)							{ Set_Flag(IS_IN_THE_SUN,onoff); }
	bool								Is_In_The_Sun(void)											{ return Get_Flag(IS_IN_THE_SUN); }
	void								Enable_Is_State_Dirty(bool onoff)						{ Set_Flag(IS_STATE_DIRTY,onoff); }
	bool								Is_State_Dirty(void)											{ return Get_Flag(IS_STATE_DIRTY); }
	void								Hide(bool onoff)						{ Set_Flag(HIDDEN,onoff); }
	bool								Is_Hidden(void)											{ return Get_Flag(HIDDEN); }
	void								Enable_Objects_Simulation(bool onoff)					{ Set_Flag(SIMULATION_DISABLED,!onoff); }
	bool								Is_Objects_Simulation_Enabled(void) const				{ return !Get_Flag(SIMULATION_DISABLED); }
	bool								Is_Object_Simulating(void)									{ return Is_Objects_Simulation_Enabled() && !Is_Simulation_Disabled(); }
	void								Set_Observer(PhysObserverClass * o)						{ Observer = o; }
	PhysObserverClass *			Get_Observer(void)											{ return Observer; }
	CollisionReactionType		Collision_Occurred(CollisionEventClass & event);
	const PhysDefClass *				Get_Definition(void)										{ return Definition; }
	virtual DynamicPhysClass *		As_DynamicPhysClass(void)								{ return NULL; }
	virtual MoveablePhysClass *	As_MoveablePhysClass(void)								{ return NULL; }
	virtual Phys3Class *				As_Phys3Class(void)										{ return NULL; }
	virtual HumanPhysClass *		As_HumanPhysClass(void)									{ return NULL; }
	virtual RigidBodyClass *		As_RigidBodyClass(void)									{ return NULL; }
	virtual VehiclePhysClass *		As_VehiclePhysClass(void)								{ return NULL;	}
	virtual MotorVehicleClass *	As_MotorVehicleClass(void)								{ return NULL; }
	virtual WheeledVehicleClass *	As_WheeledVehicleClass(void)							{ return NULL;	}
	virtual MotorcycleClass *		As_MotorcycleClass(void)								{ return NULL; }
	virtual TrackedVehicleClass *	As_TrackedVehicleClass(void)							{ return NULL; }
	virtual VTOLVehicleClass *		As_VTOLVehicleClass(void)								{ return NULL; }
	virtual StaticPhysClass *		As_StaticPhysClass(void)								{ return NULL; }
	virtual StaticAnimPhysClass *	As_StaticAnimPhysClass(void)							{ return NULL; }
	virtual ElevatorPhysClass *	As_ElevatorPhysClass(void)								{ return NULL; }
	virtual DamageableStaticPhysClass *	As_DamageableStaticPhysClass (void)			{ return NULL; }
	virtual DoorPhysClass *			As_DoorPhysClass(void)									{ return NULL; }
	virtual DecorationPhysClass *	As_DecorationPhysClass(void)							{ return NULL; }
	virtual TimedDecorationPhysClass * As_TimedDecorationPhysClass(void)				{ return NULL; }
	virtual DynamicAnimPhysClass * As_DynamicAnimPhysClass(void)							{ return NULL; }
	virtual LightPhysClass *		As_LightPhysClass(void)									{ return NULL; }
	virtual RenderObjPhysClass *	As_RenderObjPhysClass(void)							{ return NULL; }
	virtual ProjectileClass *		As_ProjectileClass(void)								{ return NULL; }
	virtual AccessiblePhysClass *	As_AccessiblePhysClass(void)							{ return NULL; }
	virtual bool						Save (ChunkSaveClass &csave);
	virtual bool						Load (ChunkLoadClass &cload);
	virtual bool						Is_Simulation_Disabled(void)				{ return false; }
	virtual bool						Is_Rendering_Disabled(void);
	unsigned Get_Last_Visible_Frame() const { return LastVisibleFrame; }
	void Set_Last_Visible_Frame(unsigned frame) { LastVisibleFrame=frame; }
protected:
	bool									Get_Flag(unsigned int flag) const 					{ return ((Flags & flag) == flag); }
	void									Set_Flag(unsigned int flag,bool onoff)			 	{ (onoff ? Flags |= flag : Flags &= ~flag); }
	void									Push_Effects(RenderInfoClass & rinfo);
	void									Pop_Effects(RenderInfoClass & rinfo);
	virtual void						Update_Sun_Status(void);
	enum {
		COLLISION_MASK =			0x0000001F,		// bits for the collision group
		IMMOVABLE =					0x00000100,		// this object is immovable.
		DISABLED =					0x00000200,		// Some objects can be disabled (e.g. lights)
		DEBUGDISPLAY =				0x00000400,		// Render debugging aids (forces, impacts, etc)
		USERCONTROL =				0x00000800,		// Ignore physics, move according to controller directly
		CASTSHADOW =				0x00001000,		// Does this object cast a shadow?
		FORCE_PROJECTION_SHADOW =	0x00002000,		// When the shadow mode is BLOBS_PLUS, this object still uses a "proper" shadow
		DONT_SAVE =					0x00004000,		// Scene should never save this object (used for transient things like glass fragments)
		ASLEEP =					0x00008000,		// This object is not moving so its simulation was skipped
		IS_WS_MESH =				0x00010000,		// Enable the static-world-space-mesh rendering optimizations.
		IS_PRE_LIT =				0x00020000,		// Is this a light-mapped object that doesn't need static lights applied.
		IS_IN_THE_SUN =				0x00040000,		// Is this object illuminated by the sun?
		IS_STATE_DIRTY =			0x00080000,		// This object's state has changed. 
		STATIC_LIGHTING_DIRTY =		0x00100000,		// This object's static lighting cache is dirty
		FRICTION_DISABLED =			0x00200000,		// Friction is disabled for this object (vehicles disable body-friction when their wheels are in contact)
		SIMULATION_DISABLED =		0x00400000,		// Turn on/off simulation for this object
		HIDDEN =                    0x00800000,     // Turn off rendering for this object
		VISIBILITY_MODE_SHIFT =		24,				// shift count for the 'visibility mode'
		VISIBILITY_MODE_MASK =		0x03000000,		// mask for the 'visibility mode'
		IGNORE_SHIFT =				28,				// shift count for the 'ignore-me' counter
		IGNORE_MASK =				0xF0000000,		// mask for the 'ignore-me' counter
		DEFAULT_FLAGS =				0,
	};
protected:
	unsigned int					Flags; // 0038  0050
	RenderObjClass *				Model; // 003C  0054
	StringClass						Name; // 0040  0058
	uint32							InstanceID; // 0044  005C
	uint32							VisObjectID; // 0048  0060
	PhysObserverClass *			Observer; // 004C  0064
	const PhysDefClass *			Definition; // 0050  0068
	RefMultiListClass<MaterialEffectClass> MaterialEffectsOnMe; // 0054  006C
	LightEnvironmentClass *		StaticLightingCache; // 006C  006C  0088  0084
	unsigned	SunStatusLastUpdated; // 0070  0070  008C  0088
	unsigned LastVisibleFrame; // 0074  0074  0090  008C

	PhysClass(const PhysClass & src);
	PhysClass & operator = (const PhysClass & src);
}; // 0078  0078  0094  0090
inline void PhysClass::Inc_Ignore_Counter(void)		
{ 
	int count = (Flags & IGNORE_MASK) >> IGNORE_SHIFT;
	count++;
	Flags &= ~IGNORE_MASK;
	Flags |= (count << IGNORE_SHIFT);
}
inline void PhysClass::Dec_Ignore_Counter(void)
{
	int count = (Flags & IGNORE_MASK) >> IGNORE_SHIFT;
	count--;
	Flags &= ~IGNORE_MASK;
	Flags |= (count << IGNORE_SHIFT);
}
inline CollisionReactionType PhysClass::Collision_Occurred(CollisionEventClass & event)
{ 
	if (Observer) {
		return Observer->Collision_Occurred(event); 
	} else {
		return COLLISION_REACTION_DEFAULT;
	}
}
inline void PhysClass::Update_Cull_Box(void)
{
	if (Model) {
		Set_Cull_Box(Model->Get_Bounding_Box(),false);
	}
}
inline void PhysClass::Add_Effect_To_Me(MaterialEffectClass * effect)
{
	MaterialEffectsOnMe.Add(effect);
}
inline void PhysClass::Remove_Effect_From_Me(MaterialEffectClass * effect)
{
	MaterialEffectsOnMe.Remove(effect);
}
inline bool PhysClass::Is_Pre_Lit(void)
{
	if (Model) {
		return (Get_Flag(IS_PRE_LIT) | (Model->Has_User_Lighting() != 0)); 
	} else {
		return Get_Flag(IS_PRE_LIT);
	}
}

#endif
