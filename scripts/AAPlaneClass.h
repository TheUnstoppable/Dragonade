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
#pragma once

#include "vector3.h"

class AAPlaneClass
{
public:

    enum AxisEnum
    {
        XNORMAL = 0,
        YNORMAL = 1,
        ZNORMAL = 2
    };

    AAPlaneClass() {}

    AAPlaneClass(AxisEnum normal,float dist) :
        Normal(normal),
        Dist(dist)
    {}

    void Set(AxisEnum normal, float dist)
    {
        Normal = normal;
        Dist = dist;
    }

    void Get_Normal(Vector3* normal) const
    {
        normal->Set(0,0,0);
        (*normal)[Normal] = 1.0f;
    }

    inline bool In_Front(const Vector3& point) const
    {
        return Dist > point[Normal];
    }

public:

    AxisEnum    Normal;
    float       Dist;
};
