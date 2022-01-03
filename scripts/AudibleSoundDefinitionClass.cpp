/*	Renegade Scripts.dll
	Copyright 2013 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#include "General.h"
#include "AudibleSoundDefinitionClass.h"
#include "SimpleDefinitionFactoryClass.h"
#include "SimplePersistFactoryClass.h"
#ifndef DDBEDIT
#ifndef TDBEDIT
#include "AudibleSoundClass.h"
#include "Sound3DClass.h"
#endif
#endif
#ifdef PARAM_EDITING_ON
#include "ParameterImpl.h"
#endif
void forcelinkaudiblesound()
{
}
SimplePersistFactoryClass<AudibleSoundDefinitionClass, 196608>	_AudibleSoundDefPersistFactory;
DECLARE_DEFINITION_FACTORY(AudibleSoundDefinitionClass, 20480, "Sound") _SoundDefFactory;

AudibleSoundDefinitionClass::AudibleSoundDefinitionClass() : m_Priority(0.5), m_Volume(1), m_VolumeRandomizer(0), m_Pan(0.5), m_LoopCount(1), m_VirtualChannel(0), m_DropoffRadius(40), m_MaxVolRadius(20), m_Is3DSound(true), m_Type(1), m_StartOffset(0), m_PitchFactor(1), m_PitchFactorRandomizer(0), m_LogicalType(0), m_LogicalNotifDelay(2.0), m_LogicalDropoffRadius(-1.0), m_CreateLogicalSound(false), m_SphereColor(0,0.75,0.75)
{
#ifdef PARAM_EDITING_ON
	NAMED_EDITABLE_PARAM(AudibleSoundDefinitionClass,ParameterClass::TYPE_SOUND_FILENAME,m_Filename,"Filename");
	INT_EDITABLE_PARAM(AudibleSoundDefinitionClass,m_VirtualChannel,0,100);
	NAMED_EDITABLE_PARAM(AudibleSoundDefinitionClass,ParameterClass::TYPE_FLOAT,m_DropoffRadius,"Drop-off Radius");
	NAMED_EDITABLE_PARAM(AudibleSoundDefinitionClass,ParameterClass::TYPE_FLOAT,m_MaxVolRadius,"Max-Vol Radius");
	NAMED_EDITABLE_PARAM(AudibleSoundDefinitionClass,ParameterClass::TYPE_BOOL,m_Is3DSound,"Is 3D Sound");
	INT_EDITABLE_PARAM(AudibleSoundDefinitionClass,m_LoopCount,0,1000000);
	FLOAT_EDITABLE_PARAM(AudibleSoundDefinitionClass,m_Volume,0,1.0);
	FLOAT_UNITS_PARAM(AudibleSoundDefinitionClass,m_VolumeRandomizer,0,1.0,"Volume Random (+/-)");
	FLOAT_EDITABLE_PARAM(AudibleSoundDefinitionClass,m_Pan,0,1.0);
	FLOAT_EDITABLE_PARAM(AudibleSoundDefinitionClass,m_Priority,0,1.0);
	EnumParameterClass * type = new EnumParameterClass(&m_Type);		
	type->Set_Name ("m_Type");
	type->Add_Value("Sound Effect",1);
	type->Add_Value("Music",0);
	type->Add_Value("Dialog",2);
	type->Add_Value("Cinematic",3);
	GENERIC_EDITABLE_PARAM( AudibleSoundDefinitionClass, type);
	FLOAT_EDITABLE_PARAM(AudibleSoundDefinitionClass,m_StartOffset,0,1.0);
	FLOAT_EDITABLE_PARAM(AudibleSoundDefinitionClass,m_PitchFactor,0,1.0);
	FLOAT_UNITS_PARAM(AudibleSoundDefinitionClass,m_PitchFactorRandomizer,0,1.0,"Pitch Factor Random (+/-)");
	NAMED_EDITABLE_PARAM(AudibleSoundDefinitionClass,ParameterClass::TYPE_STRING,m_DisplayText,"Display Text");
	NAMED_EDITABLE_PARAM(AudibleSoundDefinitionClass,ParameterClass::TYPE_BOOL,m_CreateLogicalSound,"Create Logical Sound");
	NAMED_EDITABLE_PARAM(AudibleSoundDefinitionClass,ParameterClass::TYPE_FLOAT,m_LogicalDropoffRadius,"Logical Drop-off Radius");
	NAMED_EDITABLE_PARAM(AudibleSoundDefinitionClass,ParameterClass::TYPE_FLOAT,m_LogicalNotifDelay,"Logical Notif Delay");
	EnumParameterClass * logical = new EnumParameterClass(&m_LogicalType);		
	logical->Set_Name ("Logical Type");
	logical->Add_Value("None",0);
	logical->Add_Value("Old Weapon",1);
	logical->Add_Value("Footsteps",2);
	logical->Add_Value("Vehicle",3);
	logical->Add_Value("Gunshot",4);
	logical->Add_Value("Bullet Hit",5);
	logical->Add_Value("Designer01",6);
	logical->Add_Value("Designer02",7);
	logical->Add_Value("Designer03",8);
	logical->Add_Value("Designer04",9);
	logical->Add_Value("Designer05",10);
	logical->Add_Value("Designer06",11);
	logical->Add_Value("Designer07",12);
	logical->Add_Value("Designer08",13);
	logical->Add_Value("Designer09",14);
	GENERIC_EDITABLE_PARAM( AudibleSoundDefinitionClass, logical);
	NAMED_EDITABLE_PARAM(AudibleSoundDefinitionClass,ParameterClass::TYPE_COLOR,m_SphereColor,"Sphere Color");
#endif
}

AudibleSoundDefinitionClass::~AudibleSoundDefinitionClass()
{
}

uint32 AudibleSoundDefinitionClass::Get_Class_ID() const
{
	return 0x5000;
}

void AudibleSoundDefinitionClass::Initialize_From_Sound(AudibleSoundClass *sound)
{
#ifndef DDBEDIT
#ifndef TDBEDIT
	if (sound)
	{
		Sound3DClass *s3d = sound->As_Sound3DClass();
		m_LogicalDropoffRadius = -1;
		m_LogicalNotifDelay = 2;
		m_LogicalType = 0;
		m_CreateLogicalSound = false;
		m_Pan = 0.5;
		m_DisplayText = "";
		m_Filename = sound->Get_Filename();
		m_DropoffRadius = sound->Get_DropOff_Radius();
		m_Priority = sound->Get_Priority();
		m_Is3DSound = s3d != 0;
		m_Type = sound->Get_Type();
		m_LoopCount = sound->Get_Loop_Count();
		m_Volume = sound->Get_Volume();
		m_StartOffset = sound->Get_Start_Offset();
		m_PitchFactor = sound->Get_Pitch_Factor();
		m_VirtualChannel = sound->Get_Virtual_Channel();
		if (s3d)
		{
			m_MaxVolRadius = s3d->Get_Max_Vol_Radius();
		}
	}
#endif
#endif
}

const PersistFactoryClass &AudibleSoundDefinitionClass::Get_Factory() const
{
	return _AudibleSoundDefPersistFactory;
}

bool AudibleSoundDefinitionClass::Save(ChunkSaveClass &csave)
{
	csave.Begin_Chunk(0x100);
	bool b = Save_Variables(csave);
	csave.End_Chunk();
	csave.Begin_Chunk(0x200);
	bool b2 = DefinitionClass::Save(csave);
	csave.End_Chunk();
	return b && b2;
}

bool AudibleSoundDefinitionClass::Load(ChunkLoadClass &cload)
{
	bool b = false;
	bool b2 = false;
	while (cload.Open_Chunk())
	{
		switch(cload.Cur_Chunk_ID()) 
		{
		case 0x100:
			b = Load_Variables(cload);
			break;
		case 0x200:
			b2 = DefinitionClass::Load(cload);
			break;
		}
		cload.Close_Chunk();
	}
	return b && b2;
}

bool AudibleSoundDefinitionClass::Save_Variables(ChunkSaveClass &csave)
{
	WRITE_MICRO_CHUNK(csave,3,m_Priority);
	WRITE_MICRO_CHUNK(csave,4,m_Volume);
	WRITE_MICRO_CHUNK(csave,5,m_Pan);
	WRITE_MICRO_CHUNK(csave,6,m_LoopCount);
	WRITE_MICRO_CHUNK(csave,7,m_DropoffRadius);
	WRITE_MICRO_CHUNK(csave,8,m_MaxVolRadius);
	WRITE_MICRO_CHUNK(csave,9,m_Type);
	WRITE_MICRO_CHUNK(csave,10,m_Is3DSound);
	WRITE_MICRO_CHUNK_WWSTRING(csave,11,m_Filename);
	WRITE_MICRO_CHUNK_WWSTRING(csave,12,m_DisplayText);
	WRITE_MICRO_CHUNK(csave,18,m_StartOffset);
	WRITE_MICRO_CHUNK(csave,19,m_PitchFactor);
	WRITE_MICRO_CHUNK(csave,20,m_PitchFactorRandomizer);
	WRITE_MICRO_CHUNK(csave,21,m_VolumeRandomizer);
	WRITE_MICRO_CHUNK(csave,22,m_VirtualChannel);
	WRITE_MICRO_CHUNK(csave,13,m_LogicalType);
	WRITE_MICRO_CHUNK(csave,14,m_LogicalNotifDelay);
	WRITE_MICRO_CHUNK(csave,15,m_CreateLogicalSound);
	WRITE_MICRO_CHUNK(csave,16,m_LogicalDropoffRadius);
	WRITE_MICRO_CHUNK(csave,17,m_SphereColor);
	return true;
}

bool AudibleSoundDefinitionClass::Load_Variables(ChunkLoadClass &cload)
{
	while (cload.Open_Micro_Chunk())
	{
		switch(cload.Cur_Micro_Chunk_ID())
		{
			READ_MICRO_CHUNK(cload,3,m_Priority);
			READ_MICRO_CHUNK(cload,4,m_Volume);
			READ_MICRO_CHUNK(cload,5,m_Pan);
			READ_MICRO_CHUNK(cload,6,m_LoopCount);
			READ_MICRO_CHUNK(cload,7,m_DropoffRadius);
			READ_MICRO_CHUNK(cload,8,m_MaxVolRadius);
			READ_MICRO_CHUNK(cload,9,m_Type);
			READ_MICRO_CHUNK(cload,10,m_Is3DSound);
			READ_MICRO_CHUNK_WWSTRING(cload,11,m_Filename);
			READ_MICRO_CHUNK_WWSTRING(cload,12,m_DisplayText);
			READ_MICRO_CHUNK(cload,18,m_StartOffset);
			READ_MICRO_CHUNK(cload,19,m_PitchFactor);
			READ_MICRO_CHUNK(cload,20,m_PitchFactorRandomizer);
			READ_MICRO_CHUNK(cload,21,m_VolumeRandomizer);
			READ_MICRO_CHUNK(cload,22,m_VirtualChannel);
			READ_MICRO_CHUNK(cload,13,m_LogicalType);
			READ_MICRO_CHUNK(cload,14,m_LogicalNotifDelay);
			READ_MICRO_CHUNK(cload,15,m_CreateLogicalSound);
			READ_MICRO_CHUNK(cload,16,m_LogicalDropoffRadius);
			READ_MICRO_CHUNK(cload,17,m_SphereColor);
		}
		cload.Close_Micro_Chunk();	
	}
	return true;
}

#if (TDBEDIT) || (DDBEDIT)
PersistClass *AudibleSoundDefinitionClass::Create() const
{
	return 0;
}
AudibleSoundClass *AudibleSoundDefinitionClass::Create_Sound(int type) const
{
	return 0;
}

LogicalSoundClass *AudibleSoundDefinitionClass::Create_Logical() const
{
	return 0;
}
#endif

const AudibleSoundDefinitionClass &AudibleSoundDefinitionClass::operator =(const AudibleSoundDefinitionClass &that)
{
	Set_Post_Load_Registered(that.Is_Post_Load_Registered());
	m_DefinitionMgrLink = that.m_DefinitionMgrLink;
	Set_Name(that.Get_Name());
	Set_ID(that.Get_ID());
	Set_User_Data(that.Get_User_Data());
	Enable_Save(that.Is_Save_Enabled());
	m_Priority = that.m_Priority;
	m_Volume = that.m_Volume;
	m_VolumeRandomizer = that.m_VolumeRandomizer;
	m_Pan = that.m_Pan;
	m_LoopCount = that.m_LoopCount;
	m_VirtualChannel = that.m_VirtualChannel;
	m_DropoffRadius = that.m_DropoffRadius;
	m_MaxVolRadius = that.m_MaxVolRadius;
	m_Is3DSound = that.m_Is3DSound;
	m_Filename = that.m_Filename;
	m_Type = that.m_Type;
	m_DisplayText = that.m_DisplayText;
	m_StartOffset = that.m_StartOffset;
	m_PitchFactor = that.m_PitchFactor;
	m_PitchFactorRandomizer = that.m_PitchFactorRandomizer;
	m_LogicalType = that.m_LogicalType;
	m_LogicalNotifDelay = that.m_LogicalNotifDelay;
	m_LogicalDropoffRadius = that.m_LogicalDropoffRadius;
	m_CreateLogicalSound = that.m_CreateLogicalSound;
	m_SphereColor = that.m_SphereColor;
	return *this;
}

#ifdef DDBEDIT
void AudibleSoundDefinitionClass::Dump(FileClass &file)
{
	for (int i = 0;i < this->Get_Parameter_Count();i++)
	{
		ParameterClass *param = this->Lock_Parameter(i);
		param->Dump(file);
		this->Unlock_Parameter(i);
	}
}
#endif
