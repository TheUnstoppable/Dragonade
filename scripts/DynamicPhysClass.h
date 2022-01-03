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
#ifndef TT_INCLUDE__DYNAMICPHYSCLASS_H
#define TT_INCLUDE__DYNAMICPHYSCLASS_H

#include "PhysClass.h"
#include "DynamicPhysDefClass.h"


class DynamicPhysClass : public PhysClass
{
public:
	DynamicPhysClass(void);
	~DynamicPhysClass(void);
	virtual DynamicPhysClass *	As_DynamicPhysClass(void)									{ return this; }
	void Init(const DynamicPhysDefClass & definition);
	virtual void								Set_Model(RenderObjClass * model);
	void											Update_Visibility_Status(void);
	virtual int									Get_Vis_Object_ID(void);
	virtual bool								Is_Simulation_Disabled(void)				{ return _DisableDynamicPhysSimulation; }
	virtual bool								Is_Rendering_Disabled(void)				{ return _DisableDynamicPhysRendering || PhysClass::Is_Rendering_Disabled(); }
	static void									Disable_All_Simulation(bool onoff)		{ _DisableDynamicPhysSimulation = onoff; }
	static void									Disable_All_Rendering(bool onoff)		{ _DisableDynamicPhysRendering = onoff; }
	virtual bool								Save (ChunkSaveClass &csave);
	virtual bool								Load (ChunkLoadClass &cload);
	virtual void								On_Post_Load(void);
protected:
	void											Internal_Update_Visibility_Status(void);
	bool											DirtyVisObjectID; // 0078  0078  0094  0090
	int											VisNodeID; // 007C  007C  0098  0094
	unsigned int								VisStatusLastUpdated; // 0080  0080  009C  0098
private:
	static bool _DisableDynamicPhysSimulation;
	static bool _DisableDynamicPhysRendering;
	DynamicPhysClass(const DynamicPhysClass &);
	DynamicPhysClass & operator = (const DynamicPhysClass &);
}; // 0084  0084  00A0  009C

#endif
