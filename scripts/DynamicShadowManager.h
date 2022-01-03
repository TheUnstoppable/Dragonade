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
#ifndef TT_INCLUDE_DYNAMICSHADOWMANAGER_H
#define TT_INCLUDE_DYNAMICSHADOWMANAGER_H
class PhysClass;
class DynTexProjectClass;
#include "Vector3.h"
class DynamicShadowManagerClass
{
public:
	DynamicShadowManagerClass(PhysClass & parent);
	~DynamicShadowManagerClass(void);
	void								Update_Shadow(void);
	void								Set_Shadow_Planes(float znear = -1.0f,float zfar = -1.0f);
	void								Get_Shadow_Planes(float * set_znear,float * set_zfar);
	void								Enable_Force_Use_Blob_Box(bool onoff) { ForceUseBlobBox = onoff; }
	bool								Is_Force_Use_Blob_Box_Enabled(void) { return ForceUseBlobBox; }
	void								Set_Blob_Box_Projection_Scale(const Vector3 & scl) { BlobBoxProjectionScale = scl; }
	const Vector3 &				Get_Blob_Box_Projection_Scale(void) { return BlobBoxProjectionScale; }
	bool								Is_Casting_Shadow(void) { return (Shadow != NULL); }
protected:
	void								Allocate_Shadow(void);
	void								Release_Shadow(void);

	PhysClass &						Parent; // 0000
	DynTexProjectClass *			Shadow; // 0004
	float								ShadowNearZ; // 0008
	float								ShadowFarZ; // 000C
	bool								ForceUseBlobBox; // 0010
	Vector3							BlobBoxProjectionScale; // 0014
private:
	DynamicShadowManagerClass & operator = (const DynamicShadowManagerClass &);
}; // 0020
inline void DynamicShadowManagerClass::Set_Shadow_Planes(float znear,float zfar)
{
	ShadowNearZ = znear;
	ShadowFarZ = zfar;
}
inline void DynamicShadowManagerClass::Get_Shadow_Planes(float * set_znear,float * set_zfar)
{
	*set_znear = ShadowNearZ;
	*set_zfar = ShadowFarZ;
}

#endif