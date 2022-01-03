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
#ifndef TT_INCLUDE__PROJECTORMANAGERCLASS_H
#define TT_INCLUDE__PROJECTORMANAGERCLASS_H

class ProjectorManagerDefClass;
class RenderObjClass;
class PhysTexProjectClass;
class TexProjectClass;
#include "engine_string.h"
class ProjectorManagerClass
{
public:
	ProjectorManagerClass(void);
	~ProjectorManagerClass(void);
	enum 
	{
		IS_ANIMATED = 0x00000001,
	};
	void Init(const ProjectorManagerDefClass & def,RenderObjClass * model);
	void Update_From_Model(RenderObjClass * model);
	void Set_Flag(int flag,bool onoff) {(onoff ? Flags |= flag : Flags &= ~flag);}
	bool Get_Flag(int flag) {return ((Flags & flag) == flag);}
protected:
	void Free(void);
	uint16 Flags;
	uint16 ProjectorBoneIndex;
	TexProjectClass *Projector;
};
class ChunkSaveClass;
class ChunkLoadClass;
class ProjectorManagerDefClass
{
public:
	ProjectorManagerDefClass(void);
	~ProjectorManagerDefClass(void);
	void Validate_Parameters(void);
	bool Save(ChunkSaveClass &csave);
	bool Load(ChunkLoadClass &cload);
public:
	bool											IsEnabled; //0
	bool											IsPerspective; //1
	bool											IsAdditive; //2
	bool											IsAnimated; //3
	float											OrthoWidth; //4
	float											OrthoHeight; //8
	float											HorizontalFOV; //12
	float											VerticalFOV; //16
	float											NearZ; //20
	float											FarZ; //24
	float											Intensity; //28
	StringClass									TextureName; //32
	StringClass									BoneName; //36
};
#define	PROJECTORMANAGERDEF_EDITABLE_PARAMS( class_name , member_name )																						\
   PARAM_SEPARATOR( class_name, "Texture Projector Settings");																										\
	NAMED_EDITABLE_PARAM( class_name , ParameterClass::TYPE_BOOL,		member_name ## .IsEnabled, "EnableProjector");								\
	NAMED_EDITABLE_PARAM( class_name , ParameterClass::TYPE_BOOL,		member_name ## .IsPerspective, "Perspective Projection" );				\
	NAMED_EDITABLE_PARAM( class_name , ParameterClass::TYPE_BOOL,		member_name ## .IsAdditive, "Projector Is Additive" );					\
	NAMED_EDITABLE_PARAM( class_name , ParameterClass::TYPE_BOOL,		member_name ## .IsAnimated, "Projector Is Animated" );					\
	NAMED_FLOAT_UNITS_PARAM( class_name , member_name ## .OrthoWidth , 0.01f , 1000.0f, "meters","Ortho Width" );									\
	NAMED_FLOAT_UNITS_PARAM( class_name , member_name ## .OrthoHeight , 0.01f , 1000.0f, "meters","Ortho Height" );								\
	NAMED_ANGLE_EDITABLE_PARAM( class_name , member_name ## .HorizontalFOV, DEG_TO_RADF(0.01f), DEG_TO_RADF(89.99f), "Horizontal FOV" );	\
	NAMED_ANGLE_EDITABLE_PARAM( class_name , member_name ## .VerticalFOV, DEG_TO_RADF(0.01f), DEG_TO_RADF(89.99f), "Vertical FOV" );			\
	NAMED_FLOAT_UNITS_PARAM( class_name , member_name ## .NearZ , 0.01f , 1000.0f, "meters","NearZ" );													\
	NAMED_FLOAT_UNITS_PARAM( class_name , member_name ## .FarZ , 0.01f , 1000.0f, "meters","FarZ" );													\
	NAMED_FLOAT_UNITS_PARAM( class_name , member_name ## .Intensity , 0.01f , 1.0f, "","Intensity" );													\
	FILENAME_PARAM ( class_name , member_name ## .TextureName, "Texture files", ".tga");																	\
	NAMED_EDITABLE_PARAM( class_name , ParameterClass::TYPE_STRING,	member_name ## .BoneName, "BoneName" );		
#endif
