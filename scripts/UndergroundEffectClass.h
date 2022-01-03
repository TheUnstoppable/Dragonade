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
#ifndef TT_INCLUDE__UNDERGROUNDEFFECTCLASS_H
#define TT_INCLUDE__UNDERGROUNDEFFECTCLASS_H

#include "MaterialEffectClass.h"
#include "Vector3.h"
class ChunkSaveClass;
class ChunkLoadClass;
class Vector3;
class MatrixMapperClass;

class UndergroundEffectClass : public MaterialEffectClass
{
public:
    UndergroundEffectClass(void);
    ~UndergroundEffectClass(void);

    virtual void            Timestep(float dt);
    virtual void            Render_Push(RenderInfoClass & rinfo,PhysClass * obj);
    virtual void            Render_Pop(RenderInfoClass & rinfo);

    void                    Set_Underground(bool onoff);
    void                    Set_Visible(bool onoff);
    bool                    Is_Underground() { return IsUnderground; }
    bool                    Is_Visible() { return IsVisible; }
    void                    Set_Fade_Distance(float d) { FadeDistance = d; }
    float                   Get_Fade_Distance() const { return FadeDistance; }
    void                    Set_Color(Vector3 &color) {Color = color;}
    Vector3 &               Get_Color() {return Color;}

    static const char*      Get_Underground_Shader_Name() { return UndergroundShaderName; };
    static void             Set_Underground_Shader_Name(const char* new_name);

protected:

    bool                    IsUnderground;
    bool                    WasUnderground;
    bool                    IsVisible;
    float                   FadeDistance;
    float                   CurrentFraction;
    bool                    RenderStealthMaterial;
    float                   IntensityScale;
    MaterialPassClass*      MaterialPass;
    MatrixMapperClass*      Mapper;
    Vector3                 Color;

    static char             UndergroundShaderName[64];


    void                    Update_Target_Fraction(void);
};

#endif
