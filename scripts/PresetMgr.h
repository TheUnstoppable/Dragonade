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
#ifndef TT_INCLUDE_PRESETMGR_H
#define TT_INCLUDE_PRESETMGR_H
#include "Persist.h"
#include "engine_string.h"
#include "engine_vector.h"
#include "SaveLoadSubSystemClass.h"
#include "engine_io.h"
class DefinitionClass;
class NodeClass;
class PresetClass;
class PresetMgrClass : public SaveLoadSubSystemClass
{
public:
	PresetClass *_PresetListHead;
	bool PresetsDirty;
	bool ImmedCheckin;
	virtual ~PresetMgrClass();
	virtual uint32 Chunk_ID() const;
	virtual bool Contains_Data() const;
	virtual bool Save(ChunkSaveClass& oSave);
	virtual bool Load(ChunkLoadClass& oLoad);
	virtual const char *Name() const;
	static PresetClass *Find_Preset(uint32 id);
	static void Destroy_Presets();
	static void Add_Preset(PresetClass *preset);
	static PresetClass *Find_First_Preset(uint32 ClassID, int MatchType, bool FollowTwiddlers);
	static PresetClass *Find_Next_Preset(PresetClass *preset,uint32 ClassID, int MatchType, bool FollowTwiddlers);
	static bool Match_Class_ID(uint32 classid, int MatchType, bool FollowTwiddlers, PresetClass *preset);
};
class PresetClass : public PersistClass
{
	PresetClass *m_NextPreset;
	PresetClass *m_PrevPreset;
	PresetClass *parent;
	DefinitionClass *definition;
	DynamicVectorClass<StringClass> dependencies;
	StringClass Comments;
	bool IsTemp;
	bool IsGood;
	uint32 ParentID;
	uint32 m_DefinitionID;
	DynamicVectorClass<NodeClass *> Nodes;
	DynamicVectorClass<uint32> ChildIDs;
public:
	PresetClass();
	virtual ~PresetClass();
	virtual void On_Post_Load(void);
	virtual const PersistFactoryClass &	Get_Factory(void) const;
	virtual bool Save(ChunkSaveClass &csave);
	virtual bool Load(ChunkLoadClass &cload);
	void Remove_ID(uint32 id);
	void Add_ID_To_List(uint32 id);
	PresetClass *Next()
	{
		return m_NextPreset;
	}
	PresetClass *Prev()
	{
		return m_PrevPreset;
	}
	void Set_Next(PresetClass *preset)
	{
		m_NextPreset = preset;
	}
	void Set_Prev(PresetClass *preset)
	{
		m_PrevPreset = preset;
	}
	bool Is_Good()
	{
		return IsGood;
	}
	PresetClass *Get_Child(int index)
	{
		if (index >= 0 && index < ChildIDs.Count())
		{
			return PresetMgrClass::Find_Preset(ChildIDs[index]);
		}
		return 0;
	}
	int Get_Child_Count()
	{
		return ChildIDs.Count();
	}
	DefinitionClass *Get_Definition()
	{
		return definition;
	}
	PresetClass *Get_Parent()
	{
		return parent;
	}
	uint32 Get_Definition_ID()
	{
		return m_DefinitionID;
	}
	void Set_Definition_ID(uint32 id)
	{
		m_DefinitionID = id;
	}
	bool Is_Temp()
	{
		return IsTemp;
	}
};

#if (DDBEDIT) || (TDBEDIT) || (W3DVIEWER)
extern PresetMgrClass _ThePresetMgr;
#else
extern REF_DECL(PresetMgrClass, _ThePresetMgr);
#endif
#endif
