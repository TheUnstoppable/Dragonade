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
#ifndef TT_INCLUDE__DECORATIONPHYSCLASS_H
#define TT_INCLUDE__DECORATIONPHYSCLASS_H

#include "DynamicPhysClass.h"
#include "DecorationPhysDefClass.h"

class DecorationPhysClass : public DynamicPhysClass
{
public:
	DecorationPhysClass(void);
	virtual DecorationPhysClass *	As_DecorationPhysClass(void)							{ return this; }
	void									Init(const DecorationPhysDefClass & def);
	virtual void						Timestep(float dt)										{ } 
	virtual void						Set_Model(RenderObjClass * model);
	virtual bool						Cast_Ray(PhysRayCollisionTestClass & raytest);
	virtual bool						Cast_AABox(PhysAABoxCollisionTestClass & boxtest);
	virtual bool						Cast_OBBox(PhysOBBoxCollisionTestClass & boxtest);
	virtual bool						Intersection_Test(PhysAABoxIntersectionTestClass & test);
	virtual bool						Intersection_Test(PhysOBBoxIntersectionTestClass & test);
	virtual const AABoxClass &		Get_Bounding_Box(void) const;
	virtual const Matrix3D &		Get_Transform(void) const;
	virtual void						Set_Transform(const Matrix3D & m);
	virtual void						Get_Shadow_Blob_Box(AABoxClass * set_obj_space_box);
	virtual const PersistFactoryClass &	Get_Factory (void) const;
	virtual bool								Save (ChunkSaveClass &csave);
	virtual bool								Load (ChunkLoadClass &cload);		
	virtual void								On_Post_Load(void);
private:
	DecorationPhysClass(const DecorationPhysClass &);
	DecorationPhysClass & operator = (const DecorationPhysClass &);
	AABoxClass							ObjSpaceWorldBox;
};


#endif
