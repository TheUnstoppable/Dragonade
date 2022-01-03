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
#include "general.h"
#include "PresetMgr.h"
#include "SimplePersistFactoryClass.h"
#include "DefinitionMgrClass.h"
#include "Definition.h"
#include "TwiddlerClass.h"
SimplePersistFactoryClass<PresetClass,327687> _PresetClassFactory;
PresetMgrClass _ThePresetMgr;
void forcelinkpresetmgr()
{
}
PresetClass::PresetClass() : m_NextPreset(0), m_PrevPreset(0), parent(0), definition(0), IsTemp(false), IsGood(true), ParentID(0), m_DefinitionID(0)
{
}

PresetClass::~PresetClass()
{
	PresetClass *parent = PresetMgrClass::Find_Preset(ParentID);
	if (parent)
	{
		parent->Remove_ID(m_DefinitionID);
	}
	DefinitionClass *def = DefinitionMgrClass::Find_Definition(m_DefinitionID,false);
	if (def)
	{
		if ((PresetClass *)def->Get_User_Data() == this)
		{
			def->Set_User_Data(0);
		}
	}
	//free_nodes goes here
}

void PresetClass::On_Post_Load(void)
{
	if (ParentID)
	{
		PresetClass *preset = PresetMgrClass::Find_Preset(ParentID);
		parent = preset;
		if (preset)
		{
			if (definition)
			{
				m_DefinitionID = definition->Get_ID();
				parent->Add_ID_To_List(m_DefinitionID);
			}
		}
	}
}

const PersistFactoryClass &	PresetClass::Get_Factory(void) const
{
	return _PresetClassFactory;
}

bool PresetClass::Save(ChunkSaveClass &csave)
{
	WRITE_WWSTRING_CHUNK(csave,257,Comments);
	csave.Begin_Chunk(256);
	if (definition)
	{
		m_DefinitionID = definition->Get_ID();
		WRITE_MICRO_CHUNK(csave,1,m_DefinitionID);
	}
	WRITE_MICRO_CHUNK(csave,2,IsTemp);
	for (int i = 0;i < dependencies.Count();i++)
	{
		WRITE_MICRO_CHUNK_WWSTRING(csave,8,dependencies[i]);
	}
	PresetClass *preset = this;
	WRITE_MICRO_CHUNK(csave,5,preset);
	if (parent)
	{
		DefinitionClass *def = parent->definition;
		if (def)
		{
			parent->m_DefinitionID = def->Get_ID();
		}
		ParentID = parent->m_DefinitionID;
		WRITE_MICRO_CHUNK(csave,7,ParentID);
	}
	csave.End_Chunk();
	return true;
}

bool PresetClass::Load(ChunkLoadClass &cload)
{
	StringClass str;
	PresetClass *old_this_ptr = 0;
	while (cload.Open_Chunk())
	{
		switch(cload.Cur_Chunk_ID())
		{
			READ_WWSTRING_CHUNK(cload,257,Comments);
			case 256:
				while (cload.Open_Micro_Chunk ())
				{
					switch (cload.Cur_Micro_Chunk_ID ())
					{
						READ_MICRO_CHUNK(cload,1,m_DefinitionID);
						READ_MICRO_CHUNK(cload,2,IsTemp);
						READ_MICRO_CHUNK_WWSTRING(cload,3,Comments);
						READ_MICRO_CHUNK(cload,4,parent);
						READ_MICRO_CHUNK(cload,5,old_this_ptr);
						READ_MICRO_CHUNK(cload,7,ParentID);
						case 8:
						cload.Read(str.Get_Buffer(cload.Cur_Micro_Chunk_Length()),cload.Cur_Micro_Chunk_Length());
						dependencies.Add(str);
					}
					cload.Close_Micro_Chunk();
				}
				if (definition)
				{
					m_DefinitionID = definition->Get_ID();
				}
				if (ParentID == m_DefinitionID)
				{
					IsGood = false;
				}
				SaveLoadSystemClass::Register_Pointer(old_this_ptr,this);
				if (parent)
				{
					SaveLoadSystemClass::Request_Pointer_Remap((void**)&parent);
				}
				definition = DefinitionMgrClass::Find_Definition(m_DefinitionID,false);
				if (definition)
				{
					definition->Set_User_Data((uint32)this);
				}
				break;
			default:
				break;
		}
		cload.Close_Chunk();
	}
	SaveLoadSystemClass::Register_Post_Load_Callback(this);
	return true;
}

void PresetClass::Remove_ID(uint32 id)
{
	ChildIDs.DeleteObj(id);
}

void PresetClass::Add_ID_To_List(uint32 id)
{
	if (definition)
	{
		m_DefinitionID = definition->Get_ID();
	}
	if (id != m_DefinitionID)
	{
		if (ChildIDs.ID(id) == -1)
		{
			ChildIDs.Add(id);
		}
	}
}

PresetMgrClass::~PresetMgrClass()
{
	Destroy_Presets();
}

uint32 PresetMgrClass::Chunk_ID() const
{
	return 327688;
}

bool PresetMgrClass::Contains_Data() const
{
	return _ThePresetMgr._PresetListHead != 0;
}

bool PresetMgrClass::Save(ChunkSaveClass& csave)
{
	csave.Begin_Chunk(257);
	PresetClass *preset = _ThePresetMgr._PresetListHead;
	if (preset)
	{
		do
		{
			csave.Begin_Chunk(preset->Get_Factory().Chunk_ID());
			preset->Get_Factory().Save(csave,preset);
			csave.End_Chunk();
			preset = preset->Next();
		}
		while (preset);
	}
	csave.End_Chunk();
	csave.Begin_Chunk(258);
	//save nodes
	csave.End_Chunk();
	return true;
}

bool PresetMgrClass::Load(ChunkLoadClass& cload)
{
	while (cload.Open_Chunk())
	{
		switch(cload.Cur_Chunk_ID())
		{
			case 257:
				while (cload.Open_Chunk())
				{
					PersistFactoryClass *factory = SaveLoadSystemClass::Find_Persist_Factory(cload.Cur_Chunk_ID());
					if (factory)
					{
						PresetClass *preset = (PresetClass *)factory->Load(cload);
						if (preset)
						{
							if (preset->Is_Good())
							{
								PresetMgrClass::Add_Preset(preset);
							}
						}
					}
					cload.Close_Chunk();
				}
				break;
			case 258:
				//load nodes
				break;
			default:
				break;
		}
		cload.Close_Chunk();
	}
	return true;
}

const char *PresetMgrClass::Name() const
{
	return "PresetMgrClass";
}

PresetClass *PresetMgrClass::Find_Preset(uint32 id)
{
	DefinitionClass *def = DefinitionMgrClass::Find_Definition(id,0);
	if (def)
	{
		return (PresetClass *)def->Get_User_Data();
	}
	else
	{
		return 0;
	}
}

void PresetMgrClass::Destroy_Presets()
{
	if (_ThePresetMgr._PresetListHead)
	{
		PresetClass *preset = _ThePresetMgr._PresetListHead;
		PresetClass *next;
		do
		{
			next = preset->Next();
			if (preset)
			{
				delete[] preset;
			}
			preset = next;
		} while (next);
	}
	_ThePresetMgr._PresetListHead = 0;
}

void PresetMgrClass::Add_Preset(PresetClass *preset)
{
	if (!preset->Next())
	{
		if (!preset->Prev())
		{
			preset->Set_Next(_ThePresetMgr._PresetListHead);
			if (_ThePresetMgr._PresetListHead)
			{
				_ThePresetMgr._PresetListHead->Set_Prev(preset);
			}
			_ThePresetMgr._PresetListHead = preset;
		}
	}
}

PresetClass *PresetMgrClass::Find_First_Preset(uint32 ClassID, int MatchType, bool FollowTwiddlers)
{
	PresetClass *preset = _ThePresetMgr._PresetListHead;
	PresetClass *result = 0;
	if (_ThePresetMgr._PresetListHead)
	{
		while (!Match_Class_ID(ClassID, MatchType, FollowTwiddlers, preset))
		{
			preset = preset->Next();
			if (!preset)
			{
				return 0;
			}
		}
		result = preset;
	}
	return result;
}

PresetClass *PresetMgrClass::Find_Next_Preset(PresetClass *preset,uint32 ClassID, int MatchType, bool FollowTwiddlers)
{
	PresetClass *next = preset->Next();
	PresetClass *result = 0;
	if (next)
	{
		while (!Match_Class_ID(ClassID, MatchType, FollowTwiddlers, next))
		{
			next = next->Next();
			if (!next)
			{
				return 0;
			}
		}
		result = next;
	}
	return result;
}

const int	DEF_CLASSID_START			= 0x00001000;
const int	DEF_CLASSID_RANGE			= 0x00001000;
uint32 SuperClassID_From_ClassID (uint32 class_id)
{
	int delta			= class_id - DEF_CLASSID_START;
	int num_ranges		= delta / DEF_CLASSID_RANGE;
	return DEF_CLASSID_START + (num_ranges * DEF_CLASSID_RANGE);
}

bool PresetMgrClass::Match_Class_ID(uint32 classid, int MatchType, bool FollowTwiddlers, PresetClass *preset)
{
	if (!preset->Get_Definition())
	{
		return false;
	}
	uint32 classid2 = preset->Get_Definition()->Get_Class_ID();
	if (FollowTwiddlers)
	{
		if (classid2 == 57344)
		{
			classid2 = ((TwiddlerClass *)(preset->Get_Definition()))->Get_Indirect_Class_ID();
		}
	}
	if (MatchType == 1)
	{
		if (classid2 == classid)
		{
			return true;
		}
	}
	else if (MatchType == 2)
	{
		if (SuperClassID_From_ClassID(classid2) == classid)
		{
			return true;
		}
	}
	return false;
}
