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
#ifndef TT_INCLUDE__MATERIALEFFECTCLASS_H
#define TT_INCLUDE__MATERIALEFFECTCLASS_H
#include "engine_vector.h"
class PhysClass;
class RenderInfoClass;
class	MaterialPassClass;
class MaterialEffectClass : public MultiListObjectClass, public RefCountClass
{
public:
	MaterialEffectClass(void);
	virtual ~MaterialEffectClass(void);
	void					Enable_Auto_Remove(bool onoff)			{ AutoRemoveEnabled = onoff; }
	bool					Is_Auto_Remove_Enabled(void)				{ return AutoRemoveEnabled; }
	void					Enable_Suppress_Shadows(bool onoff)		{ SuppressShadows = onoff; }
	bool					Are_Shadows_Suppressed(void)				{ return SuppressShadows; }
	virtual void		Timestep(float dt)							{ }
	virtual void		Render_Push(RenderInfoClass & rinfo,PhysClass * obj)		= 0;
	virtual void		Render_Pop(RenderInfoClass & rinfo)								= 0;
	static void			Timestep_All_Effects(float dt);
private:
	bool					AutoRemoveEnabled;
	bool					SuppressShadows;
#ifndef TTLE_EXPORTS
#ifndef DDBEDIT
	static MultiListClass<MaterialEffectClass> AllocatedEffects;
#endif
#endif
}; // size: 20, RH7: 44

#endif
