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
#ifndef TT_INCLUDE__STATICPHYSCLASS_H
#define TT_INCLUDE__STATICPHYSCLASS_H

#include "PhysClass.h"
#include "PhysIntTest.h"
#include "PhysColTest.h"
class StaticPhysDefClass;
class StaticPhysClass : public PhysClass
{
public:
	StaticPhysClass(void);
	~StaticPhysClass(void);
	virtual StaticPhysClass *	As_StaticPhysClass(void)									{ return this; }
	StaticPhysDefClass *			Get_StaticPhysDef(void)										{ return (StaticPhysDefClass *)Definition; }
	void								Init(const StaticPhysDefClass & def);
	virtual bool					Needs_Timestep(void)											{ return false; }
	virtual void					Timestep(float dt)											{ }; 
	virtual void					Set_Model(RenderObjClass * model);
	virtual void					Render_Vis_Meshes(RenderInfoClass & rinfo);
	virtual bool					Cast_Ray(PhysRayCollisionTestClass & raytest)
	{
		if (Model->Cast_Ray(raytest))
		{
			raytest.CollidedPhysObj = this;
			return true;
		}
		else
		{
			return false;
		}
	}
	virtual bool					Cast_AABox(PhysAABoxCollisionTestClass & boxtest)
	{
		if (Model->Cast_AABox(boxtest))
		{
			boxtest.CollidedPhysObj = this;
			return true;
		}
		else
		{
			return false;
		}
	}
	virtual bool					Cast_OBBox(PhysOBBoxCollisionTestClass & boxtest)
	{
		if (Model->Cast_OBBox(boxtest))
		{
			boxtest.CollidedPhysObj = this;
			return true;
		}
		else
		{
			return false;
		}
	}
	virtual bool					Intersection_Test(PhysAABoxIntersectionTestClass & test)
	{
		if (Model->Intersect_AABox(test))
		{
			test.Add_Intersected_Object(this);
			return true;
		}
		return false;
	}
	virtual bool					Intersection_Test(PhysOBBoxIntersectionTestClass & test)
	{
		if (Model->Intersect_OBBox(test))
		{
			test.Add_Intersected_Object(this);
			return true;
		}
		return false;
	}
	virtual bool					Intersection_Test(PhysMeshIntersectionTestClass & test)
	{
		return false;
	}
	bool								Intersects(const OBBoxClass & obbox)
	{
		CastResultStruct result;
		PhysOBBoxCollisionTestClass boxtest(obbox,Vector3(0,0,0),&result,TERRAIN_COLLISION_GROUP,COLLISION_TYPE_ALL);
		Cast_OBBox(boxtest);
		return result.StartBad;
	}
	virtual const AABoxClass & Get_Bounding_Box(void) const;
	virtual const Matrix3D &	Get_Transform(void) const;
	virtual void					Set_Transform(const Matrix3D & m);
	int								Is_Occluder(void);
	bool								Is_Model_Pre_Lit(void);
	bool								Is_Model_User_Lit(void);
	void								Set_Vis_Sector_ID(int new_id);	
	int								Get_Vis_Sector_ID(void) const { return VisSectorID; }
	bool								Is_Vis_Sector(RenderObjClass * model = NULL) const;
	virtual bool					Is_Simulation_Disabled(void)				{ return _DisableStaticPhysSimulation; }
	virtual bool					Is_Rendering_Disabled(void)				{ return _DisableStaticPhysRendering || PhysClass::Is_Rendering_Disabled(); }
	static void						Disable_All_Simulation(bool onoff)		{ _DisableStaticPhysSimulation = onoff; }
	static void						Disable_All_Rendering(bool onoff)		{ _DisableStaticPhysRendering = onoff; }
	static bool						Is_All_Simulation_Disabled(void)			{ return _DisableStaticPhysSimulation; }
	static bool						Is_All_Rendering_Disabled(void)			{ return _DisableStaticPhysRendering; }
	virtual bool								Has_Dynamic_State(void)							{ return false; }
	virtual void								Save_State(ChunkSaveClass & csave)			{ }
	virtual void								Load_State(ChunkLoadClass & cload)			{ }
	virtual const PersistFactoryClass &	Get_Factory (void) const;
	virtual bool								Save (ChunkSaveClass &csave);
	virtual bool								Load (ChunkLoadClass &cload);
	virtual void								On_Post_Load(void);
	float	Compute_Vis_Mesh_Ram(RenderObjClass * model = NULL);
protected:
	void								Update_Cached_Model_Parameters(void);
	virtual void					Update_Sun_Status(void);
protected:
	int								VisSectorID;		// set if this static object contains a vis sector mesh (-1 if not)
private:
	static bool						_DisableStaticPhysSimulation;
	static bool						_DisableStaticPhysRendering;
	StaticPhysClass(const StaticPhysClass &);
	StaticPhysClass & operator = (const StaticPhysClass &);
}; // 007C  007C  0098  0094


#endif
