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
#ifndef TT_INCLUDE__STEALTHEFFECTCLASS_H
#define TT_INCLUDE__STEALTHEFFECTCLASS_H

#include "MaterialEffectClass.h"
#include "Vector2.h"
class MatrixMapperClass;
class MaterialPassClass;
class ChunkSaveClass;
class ChunkLoadClass;
class StealthEffectClass : public MaterialEffectClass
{
public:
	StealthEffectClass(void);
	~StealthEffectClass(void);
	virtual void		Timestep(float dt);
	virtual void		Render_Push(RenderInfoClass & rinfo,PhysClass * obj);
	virtual void		Render_Pop(RenderInfoClass & rinfo);
	void					Enable_Stealth(bool onoff);
	void					Set_Friendly(bool onoff);
	void					Set_Broken(bool onoff);
	bool					Is_Stealth_Enabled(void)					{ return IsStealthEnabled; }
	bool					Is_Friendly(void)								{ return IsFriendly; }
	bool					Is_Broken(void)								{ return IsBroken; }
	bool					Is_Stealthed(void) const					{ return CurrentFraction > 0.5f; }
	void					Set_Fade_Distance(float d)					{ FadeDistance = d; }
	float					Get_Fade_Distance(void) const				{ return FadeDistance; }
	void					Set_Current_State( float percent )		{ CurrentFraction = percent; }
	void					Set_Target_State( float percent )		{ TargetFraction = percent; }
	void					Damage_Occured(void);
	bool					Save(ChunkSaveClass & csave);
	bool					Load(ChunkLoadClass & cload);

	static const char*		Get_Stealth_Shader_Name()						{ return StealthShaderName; };
	static void				Set_Stealth_Shader_Name(const char* new_name);
protected:
	void					Update_Target_Fraction(void);
	bool					IsStealthEnabled; //20
	bool					IsFriendly; //21
	bool					IsBroken; //22
	float					FadeDistance; //24
	float					CurrentFraction; //28
	float					TargetFraction; //32
	Vector2					UVRate; //36
	bool					RenderBaseMaterial; //44
	bool					RenderStealthMaterial; //45
	float					IntensityScale; //48
	Vector2					UVOffset; //52
	MatrixMapperClass *		Mapper; //60
	MaterialPassClass *		MaterialPass; //64

	static char	StealthShaderName[64];
};


#endif
