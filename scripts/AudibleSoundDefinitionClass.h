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
#ifndef TT_INCLUDE__AUDIBLESOUNDDEFINITIONCLASS_H
#define TT_INCLUDE__AUDIBLESOUNDDEFINITIONCLASS_H



#include "definition.h"
#include "Vector3.h"


class LogicalSoundClass;
class AudibleSoundClass;



class AudibleSoundDefinitionClass :
	public DefinitionClass
{
public:
	virtual ~AudibleSoundDefinitionClass();
	virtual const PersistFactoryClass& Get_Factory() const;
	virtual bool Save(ChunkSaveClass&);
	virtual bool Load(ChunkLoadClass&);
	virtual uint32 Get_Class_ID() const;
	virtual PersistClass* Create() const;
	virtual AudibleSoundClass *Create_Sound(int type) const;
	virtual void Initialize_From_Sound(AudibleSoundClass*);
	virtual const StringClass& Get_Filename() const {return m_Filename;}
	virtual const StringClass& Get_Display_Text() const {return m_DisplayText;}
	virtual float Get_Max_Vol_Radius() const {return m_MaxVolRadius;}
	virtual float Get_DropOff_Radius() const {return m_DropoffRadius;}
	virtual const Vector3 &Get_Sphere_Color() const {return m_SphereColor;}
	virtual float Get_Volume() const {return m_Volume;}
	virtual float Get_Volume_Randomizer() const {return m_VolumeRandomizer;}
	virtual float Get_Start_Offset() const {return m_StartOffset;}
	virtual float Get_Pitch_Factor() const {return m_PitchFactor;}
	virtual float Get_Pitch_Factor_Randomizer() const {return m_PitchFactorRandomizer;}
	virtual int Get_Virtual_Channel() const {return m_VirtualChannel;}
	virtual void Set_Volume(float f) {m_Volume = f;}
	virtual void Set_Volume_Randomizer(float f) {m_VolumeRandomizer = f;}
	virtual void Set_Max_Vol_Radius(float f) {m_MaxVolRadius = f;}
	virtual void Set_DropOff_Radius(float f) {m_DropoffRadius = f;}
	virtual void Set_Start_Offset(float f) {m_StartOffset = f;}
	virtual void Set_Pitch_Factor(float f) {m_PitchFactor = f;}
	virtual void Set_Pitch_Factor_Randomizer(float f) {m_PitchFactorRandomizer = f;}
	virtual void Set_Virtual_Channel(int i) {m_VirtualChannel = i;}
	virtual LogicalSoundClass* Create_Logical() const;
	float Get_Logical_DropOff_Radius() const {return m_LogicalDropoffRadius;}
	int Get_Logical_Type() const {return m_LogicalType;}
	AudibleSoundDefinitionClass();
	bool Save_Variables(ChunkSaveClass&);
	bool Load_Variables(ChunkLoadClass&);
	const AudibleSoundDefinitionClass &operator=(AudibleSoundDefinitionClass const &that);
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE(AudibleSoundDefinitionClass,DefinitionClass);
private:
	float m_Priority; // 001C
	float m_Volume; // 0020
	float m_VolumeRandomizer; // 0024
	float m_Pan; // 0028
	int m_LoopCount; // 002C
	int m_VirtualChannel; // 0030
	float m_DropoffRadius; // 0034
	float m_MaxVolRadius; // 0038
	bool m_Is3DSound; // 003C
	StringClass m_Filename; // 0040
	int m_Type; // 0044
	StringClass m_DisplayText; // 0048
	float m_StartOffset; // 004C
	float m_PitchFactor; // 0050
	float m_PitchFactorRandomizer; // 0054
	int m_LogicalType; // 0058
	float m_LogicalNotifDelay; // 005C
	float m_LogicalDropoffRadius; // 0060
	bool m_CreateLogicalSound; // 0064
	Vector3 m_SphereColor; // 0068
	friend class PresetDump;
};



#endif