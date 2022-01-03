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
#ifndef TT_INCLUDE__MOVEABLEPHYSCLASS_H
#define TT_INCLUDE__MOVEABLEPHYSCLASS_H

#include "DynamicPhysClass.h"
#include "MoveablePhysDefClass.h"
#include "DynamicShadowManager.h"
#include "Matrix3.h"
class PhysControllerClass;
class TexProjectClass;
class DynTexProjectClass;
class MoveablePhysClass :
	public DynamicPhysClass
{
public:
	MoveablePhysClass(void);
	virtual ~MoveablePhysClass(void);
	void Init(const MoveablePhysDefClass & definition );
	virtual MoveablePhysClass * As_MoveablePhysClass(void)								{ return this; }
	const MoveablePhysDefClass * Get_MoveablePhysDef(void)								{ return (MoveablePhysDefClass*)Definition; }
	virtual void					Definition_Changed(void);
	virtual bool					Needs_Timestep(void)											{ return true; }
	virtual void					Post_Timestep_Process(void);
	virtual void					Set_Mass(float mass)											{ Mass = mass; MassInv = 1.0f / mass; }
	float								Get_Mass(void)													{ return Mass; }
	float								Get_Mass_Inv(void)											{ return MassInv; }
	virtual void					Set_Gravity_Multiplier(float grav)						{ GravScale = grav; }
	float								Get_Gravity_Multiplier(void) const						{ return GravScale; }
	virtual void					Set_Elasticity(float e)										{ Elasticity = e; }
	float								Get_Elasticity(void) const									{ return Elasticity; }
	virtual void					Get_Inertia_Inv(Matrix3 * set_I_inv);
	virtual bool					Can_Teleport(const Matrix3D &new_tm, bool check_dyn_only = false,MultiListClass<PhysClass> * result_list = NULL)	{ return false; }
	virtual bool					Can_Teleport_And_Stand(const Matrix3D &new_tm, Matrix3D *out)	{ *out = new_tm; return false; }
	virtual bool					Find_Teleport_Location(const Vector3 &start, float radius, Vector3 *out)			{ return false; }
	virtual bool					Can_Move_To(const Matrix3D &new_tm) { return false; }
	virtual bool					Cinematic_Move_To(const Matrix3D & new_tm);
	void								Set_Controller(PhysControllerClass * control)		{ Controller = control; }	
	PhysControllerClass *		Get_Controller(void)											{ return Controller; }
	virtual bool					Save (ChunkSaveClass &csave);
	virtual bool					Load (ChunkLoadClass &cload);
	virtual void					On_Post_Load (void);
	virtual void					Get_Velocity(Vector3 * set_vel) const					= 0;
	virtual void					Set_Velocity(const Vector3 & newvel)					{ }
	virtual void					Get_Shadow_Blob_Box(AABoxClass * set_obj_space_box);
	virtual bool					Is_Casting_Shadow(void)										{ return ShadowManager.Is_Casting_Shadow(); }
	virtual void					Link_To_Carrier(PhysClass * carrier,RenderObjClass * carrier_sub_obj = NULL);
	virtual PhysClass *			Peek_Carrier_Object(void); // TSS added 08-15-01
	virtual RenderObjClass *	Peek_Carrier_Sub_Object(void);
protected:
	float								Mass; // 0084  0084  00A0  009C
	float								MassInv; // 0088  0088  00A4  00A0
	float								GravScale; // 008C  008C  00A8  00A4
	float								Elasticity; // 0090  0090  00AC  00A8
	PhysControllerClass *		Controller; // 0094  0094  00B0  00AC
	PhysClass *						Carrier; // 0098  0098  00B4  00B0
	RenderObjClass *				CarrierSubObject; // 009C  009C  00B8  00B4
	DynamicShadowManagerClass	ShadowManager; // 00A0  00A0  00BC  00B8

	MoveablePhysClass(const MoveablePhysClass &);
	MoveablePhysClass & operator = (const MoveablePhysClass &);
}; // 00C0  00C0  00DC  00D8
inline void MoveablePhysClass::Get_Inertia_Inv(Matrix3 * set_I_inv)					
{ 
	set_I_inv->Make_Identity();
	(*set_I_inv)[0][0] = MassInv;
	(*set_I_inv)[1][1] = MassInv;
	(*set_I_inv)[2][2] = MassInv;
}

#endif
