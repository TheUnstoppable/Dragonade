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
#ifndef TT_INCLUDE__LIGHTPHYSCLASS_H
#define TT_INCLUDE__LIGHTPHYSCLASS_H

#include "DynamicPhysClass.h"
#include "DecorationPhysClass.h"
class LightPhysClass : public DecorationPhysClass
{
public:
	LightPhysClass(bool auto_allocate_light = false);
	virtual LightPhysClass *				As_LightPhysClass(void)		{ return this; }
	void											Set_Model(RenderObjClass * model);
	void											Set_Vis_Sector_ID(int new_id)		{ VisSectorID = new_id; }	
	int											Get_Vis_Sector_ID(void) const		{ return VisSectorID; }
	void											Set_Group_ID(int new_id)			{ GroupID = new_id; }
	int											Get_Group_ID(void) const			{ return GroupID; }
	int											Is_Vis_Object_Visible(int vis_object_id);
	virtual const PersistFactoryClass &	Get_Factory (void) const;
	virtual bool								Save (ChunkSaveClass &csave);
	virtual bool								Load (ChunkLoadClass &cload);
	virtual void								On_Post_Load(void);
private:
	int											VisSectorID;			// static lights have a pvs.
	int											GroupID;					// group id, used by external code
	LightPhysClass(const LightPhysClass &);
	LightPhysClass & operator = (const LightPhysClass &);
};

#endif
