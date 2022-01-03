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
#ifndef TT_INCLUDE__STATICANIMPHYSCLASS_H
#define TT_INCLUDE__STATICANIMPHYSCLASS_H



#include "AnimCollisionManagerClass.h"
#include "ProjectorManagerClass.h"
#include "StaticPhysClass.h"



class TexProjectClass;
class StaticAnimPhysDefClass;



class StaticAnimPhysClass : public StaticPhysClass
{
public:
	StaticAnimPhysClass(void);
	~StaticAnimPhysClass(void);
	virtual StaticAnimPhysClass *			As_StaticAnimPhysClass(void)								{ return this; }
	void											Init(const StaticAnimPhysDefClass & def);
	virtual void								Set_Model(RenderObjClass * model);
	virtual void								Vis_Render(SpecialRenderInfoClass & rinfo);
	StaticAnimPhysDefClass *				Get_StaticAnimPhysDef(void)								{ return (StaticAnimPhysDefClass *)Definition; }
	virtual bool								Needs_Timestep(void);
	virtual void								Timestep(float dt); 
	virtual bool								Has_Dynamic_State(void)										{ return true; }
	virtual void								Save_State(ChunkSaveClass & csave);
	virtual void								Load_State(ChunkLoadClass & cload);
	virtual bool								Internal_Link_Rider(PhysClass * rider);
	virtual bool								Internal_Unlink_Rider(PhysClass * rider);
	AnimCollisionManagerClass &			Get_Animation_Manager(void)								{ return AnimManager; }
	void											Set_Shadow(TexProjectClass * shadow);
	virtual bool								Is_Casting_Shadow(void)										{ return (ShadowProjector != NULL); }
	void											Debug_Display_Shadow(const Vector2 & v0,const Vector2 & v1);
	virtual const PersistFactoryClass &	Get_Factory(void) const;
	virtual bool								Save(ChunkSaveClass &csave);
	virtual bool								Load(ChunkLoadClass &cload);
	virtual void								On_Post_Load(void);
protected:
	void											Update_Cached_Model_Parameters(void);
	virtual void								Update_Sun_Status(void);
protected:
	ProjectorManagerClass					ProjectorManager;
	AnimCollisionManagerClass				AnimManager;
	TexProjectClass *							ShadowProjector;
private:
	StaticAnimPhysClass(const StaticPhysClass &);
	StaticAnimPhysClass & operator = (const StaticPhysClass &);
}; // 00E4  00E4  0100  00FC

#endif
