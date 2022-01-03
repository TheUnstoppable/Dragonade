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
#include "TwiddlerClass.h"
#include "DefinitionMgrClass.h"
#include "PersistFactoryClass.h"
#include "Definition.h"
#include "SaveLoadSystemClass.h"
#include "HashTemplateIterator.h"
#ifdef TT_EXPORTS
#include "AntiCheat.h"
#endif
DefinitionMgrClass _TheDefinitionMgr;
int DefinitionMgrClass::_DefinitionCount = 0;
DefinitionClass **DefinitionMgrClass::_SortedDefinitionArray = 0;
HashTemplateClass<StringClass,DynamicVectorClass<DefinitionClass *> *>* DefinitionMgrClass::DefinitionHash;
int DefinitionMgrClass::_MaxDefinitionCount = 0;

uint32 Get_Super_Class_ID(uint32 _class){
	int result;
	result = _class - 0x1000;
	return ((((result < 0) ? (_class-1) : result) >> 12) << 12) + 0x1000;
}

DefinitionClass* DefinitionMgrClass::Find_Definition(uint32 id, bool twiddle)
{
	DefinitionClass *definition = 0;
	int start = 0;
	int end = _DefinitionCount - 1;
	int current = end / 2;
	bool b = _DefinitionCount > 0;
	while (b)
	{
		if (_SortedDefinitionArray[current]->Get_ID() == id)
		{
			definition = _SortedDefinitionArray[current];
			b = 0;
		}
		else
		{
			if (end > start + 1)
			{
				if (id <= _SortedDefinitionArray[current]->Get_ID())
				{
					end = current;
					current -= (current - start) / 2;
				}
				else
				{
					start = current;
					current += (end - current) / 2;
				}
				continue;
			}
			b = 0;
			if (_SortedDefinitionArray[start]->Get_ID() == id )
			{
				definition = _SortedDefinitionArray[start];
				continue;
			}
			if (_SortedDefinitionArray[end]->Get_ID() == id )
			{
				definition = _SortedDefinitionArray[end];
				continue;
			}
		}
	}
	if (twiddle)
	{
		if (definition)
		{
			if (definition->Get_Class_ID() == 0xE000)
			{
				definition = ((TwiddlerClass *)definition)->Twiddle();
			}
		}
	}
	return definition;
}

void DefinitionMgrClass::Unregister_Definition(DefinitionClass *definition)
{
	if (definition)
	{
		if (definition->m_DefinitionMgrLink != -1)
		{
			for (int i = definition->m_DefinitionMgrLink;i < _DefinitionCount - 1;i++)
			{
				_SortedDefinitionArray[i] = _SortedDefinitionArray[i + 1];
				_SortedDefinitionArray[i]->m_DefinitionMgrLink = i;
			}
			_SortedDefinitionArray[_DefinitionCount - 1] = 0;
			definition->m_DefinitionMgrLink = -1;
			_DefinitionCount--;
		}
	}
}

void DefinitionMgrClass::Register_Definition(DefinitionClass *definition)
{
	if (definition)
	{
		if (definition->m_DefinitionMgrLink == -1)
		{
			if (definition->Get_ID())
			{
				Prepare_Definition_Array();
				unsigned long id = definition->Get_ID();
				int start = 0;
				int end = _DefinitionCount - 1;
				int current = end / 2;
				int count = _DefinitionCount;
				bool b = _DefinitionCount > 0;
				bool done = true;
				while (b)
				{
					if (_SortedDefinitionArray[current]->Get_ID() == id)
					{
						count = current;
						b = false;
						done = false;
					}
					else
					{
						if ( id <= _SortedDefinitionArray[current]->Get_ID())
						{
							end = current;
							current -= (current - start) / 2;
						}
						else
						{
							start = current;
							current += (end - current) / 2;
						}
						if (end <= start + 1)
						{
							if (_SortedDefinitionArray[end]->Get_ID() > id)
							{
								if (_SortedDefinitionArray[start]->Get_ID() > id)
								{
									count = start;
								}
								else
								{
									count = end;
								}
							}
							else
							{
								count = end + 1;
							}
							b = 0;
						}
					}
				}
				if (done)
				{
					for (current = _DefinitionCount - 1;current >= count;current--)
					{
						_SortedDefinitionArray[current + 1] = _SortedDefinitionArray[current];
						_SortedDefinitionArray[current + 1]->m_DefinitionMgrLink = current + 1;
					}
					definition->m_DefinitionMgrLink = count;
					_SortedDefinitionArray[count] = definition;
					_DefinitionCount++;
				}
			}
		}
	}
}

DefinitionClass* DefinitionMgrClass::Find_Named_Definition(const char* name, bool twiddle)
{
	DefinitionClass *definition = 0;
	for (int i = 0;i < _DefinitionCount;i++)
	{
		if (_SortedDefinitionArray[i])
		{
			if (!_stricmp(_SortedDefinitionArray[i]->Get_Name(),name))
			{
				definition = _SortedDefinitionArray[i];
				break;
			}
		}
	}
	if (twiddle)
	{
		if (definition)
		{
			if (definition->Get_Class_ID() == 0xE000)
			{
				definition = ((TwiddlerClass *)definition)->Twiddle();
			}
		}
	}
	return definition;
}
unsigned long Get_Definition_Type(unsigned long id)
{
	return ((id - 4096) / 4096 << 12) + 4096;
}
DefinitionClass* DefinitionMgrClass::Find_Typed_Definition(const char* name, uint32 class_id, bool twiddle)
{
	DefinitionClass *definition = 0;
	if (DefinitionHash)
	{
		StringClass str(name);
		_strlwr(str.Peek_Buffer());
		DynamicVectorClass<DefinitionClass *> *vector = DefinitionHash->Get(str, NULL);
		if (vector)
		{
			for (int i = 0;i < vector->Count();i++)
			{
				DefinitionClass *def = (*vector)[i];
				if (def && def->Get_Class_ID() != class_id)
				{
					if (Get_Definition_Type(def->Get_Class_ID()) != class_id)
					{
						if (!twiddle)
						{
							continue;
						}
						if (def->Get_Class_ID() != 0xE000)
						{
							continue;
						}
					}
				}
				definition = def;
			}
		}
		if (!definition)
		{
			for (int j = 0;j < _DefinitionCount;j++)
			{
				DefinitionClass *def = _SortedDefinitionArray[j];
				if (def)
				{
					if ((def->Get_Class_ID() == class_id) || (Get_Definition_Type(def->Get_Class_ID()) == class_id) || (twiddle && (def->Get_Class_ID() == 0xE000)))
					{
						if (!_strcmpi(def->Get_Name(),name))
						{
							definition = def;
							if (!vector)
							{
								vector = new DynamicVectorClass<DefinitionClass *>();
								DefinitionHash->Insert(str,vector);
							}
							vector->Add(definition);
							break;
						}
					}
				}
			}
		}
	}
	if (twiddle)
	{
		if (definition)
		{
			if (definition->Get_Class_ID() == 0xE000)
			{
				definition = ((TwiddlerClass *)definition)->Twiddle();
			}
		}
	}
	return definition;
}

void DefinitionMgrClass::Free_Definitions()
{
	if (DefinitionHash)
	{
		for (HashTemplateIterator<StringClass, DynamicVectorClass<DefinitionClass*>*> iter(*DefinitionHash); iter; ++iter)
			delete iter.getValue();
		
		DefinitionHash->Remove_All();
		delete DefinitionHash;
		DefinitionHash = 0;
	}
	for (int i = 0;i < _DefinitionCount;i++)
	{
		if (_SortedDefinitionArray[i])
		{
			delete _SortedDefinitionArray[i];
		}
	}
	if (_SortedDefinitionArray)
	{
		delete[] _SortedDefinitionArray;
	}
	_SortedDefinitionArray = 0;
	_MaxDefinitionCount = 0;
	_DefinitionCount = 0;
#ifdef TT_EXPORTS
	AntiCheatMgr::OnFreeDefinitions();
#endif
}

void DefinitionMgrClass::Prepare_Definition_Array()
{
	if (_DefinitionCount + 1 > _MaxDefinitionCount)
	{
		int newmaxdefcount = _MaxDefinitionCount + 1000;
		DefinitionClass **newarray = new DefinitionClass *[newmaxdefcount];
		memcpy(newarray,_SortedDefinitionArray,_DefinitionCount * sizeof(DefinitionClass *));
		if (_SortedDefinitionArray)
		{
			delete[] _SortedDefinitionArray;
		}
		_SortedDefinitionArray = newarray;
		_MaxDefinitionCount = newmaxdefcount;
	}
	if (!DefinitionHash)
	{
		DefinitionHash = new HashTemplateClass<StringClass,DynamicVectorClass<DefinitionClass *> *>();
	}
}

int DefinitionMgrClass::fnCompareDefinitionsCallback(const void* elem1, const void* elem2)
{
	DefinitionClass *d1 = *(DefinitionClass **)elem1;
	DefinitionClass *d2 = *(DefinitionClass **)elem2;
	int ret = 1;
	if (d1->Get_ID() <= d2->Get_ID())
	{
		ret = -(d1->Get_ID() < d2->Get_ID());
	}
	return ret;
}

unsigned long DefinitionMgrClass::Get_New_ID(uint32 class_id)
{
	for(int x = 0; x < _DefinitionCount; x++)
	{
		DefinitionClass *def = _SortedDefinitionArray[x];
		if (!def)
		{
			continue;
		}

		uint32 defId = def->Get_ID();
		if (defId >= 10000 * class_id - 40960000 && defId < 10000 * class_id - 40950000)
		{
			if (x == _DefinitionCount - 1)
			{
				return defId + 1;
			}

			DefinitionClass *nextDef = _SortedDefinitionArray[x + 1];
			if (nextDef && nextDef->Get_ID() > defId + 1)
			{
				return defId + 1;
			}
		}
	}

	return (10000 * class_id - 40959999);
}

DefinitionClass *DefinitionMgrClass::Get_First(uint32 id,ID_TYPE type)
{
	for (int i = 0;i < _DefinitionCount;i++)
	{
		if (_SortedDefinitionArray[i])
		{
			if (type == ID_SUPERCLASS && Get_Super_Class_ID(_SortedDefinitionArray[i]->Get_Class_ID()) == id)
				return _SortedDefinitionArray[i];
			else if (type == ID_CLASS && _SortedDefinitionArray[i]->Get_Class_ID() == id)
				return _SortedDefinitionArray[i];
		}
	}
	return 0;
}

DefinitionClass *DefinitionMgrClass::Get_Next(DefinitionClass* curr_def,uint32 id,ID_TYPE type)
{
	for (int i = curr_def->m_DefinitionMgrLink + 1;i < _DefinitionCount;i++)
	{
		if (_SortedDefinitionArray[i])
		{
		if (type == ID_SUPERCLASS && Get_Super_Class_ID(_SortedDefinitionArray[i]->Get_Class_ID()) == id)
			return _SortedDefinitionArray[i];
		else if (type == ID_CLASS && _SortedDefinitionArray[i]->Get_Class_ID() == id)
			return _SortedDefinitionArray[i];
		}
	}
	return 0;
}

DefinitionClass *DefinitionMgrClass::Get_Next(DefinitionClass* curr_def)
{
	if (curr_def->m_DefinitionMgrLink + 1 < _DefinitionCount)
	{
		return _SortedDefinitionArray[curr_def->m_DefinitionMgrLink + 1];
	}
	return 0;
}

DefinitionMgrClass::DefinitionMgrClass()
{
}

bool DefinitionMgrClass::Save_Objects(ChunkSaveClass& csave)
{
	for (int i = 0;i < _DefinitionCount;i++)
	{
		DefinitionClass *def = _SortedDefinitionArray[i];
		if (def)
		{
			if (def->m_SaveEnabled)
			{
				csave.Begin_Chunk(def->Get_Factory().Chunk_ID());
				def->Get_Factory().Save(csave,def);
				csave.End_Chunk();
			}
		}
	}
	return true;
}

bool DefinitionMgrClass::Load_Objects(ChunkLoadClass& cload)
{
	while (cload.Open_Chunk())
	{
		PersistFactoryClass *factory = SaveLoadSystemClass::Find_Persist_Factory(cload.Cur_Chunk_ID());
		if (factory)
		{
			DefinitionClass *def = (DefinitionClass *)factory->Load(cload);
			if (def)
			{
				Prepare_Definition_Array();
				_SortedDefinitionArray[_DefinitionCount++] = def;
			}
		}
		cload.Close_Chunk();
	}
	if (_DefinitionCount > 0)
	{
		qsort(_SortedDefinitionArray,_DefinitionCount,sizeof(DefinitionClass *),fnCompareDefinitionsCallback);
	}
	for (int i = 0;i < _DefinitionCount;i++)
	{
		_SortedDefinitionArray[i]->m_DefinitionMgrLink = i;
	}
	return true;
}

bool DefinitionMgrClass::Save_Variables(ChunkSaveClass& csave)
{
	return true;
}

bool DefinitionMgrClass::Load_Variables(ChunkLoadClass& cload)
{
	return true;
}

DefinitionMgrClass::~DefinitionMgrClass()
{
	Free_Definitions();
}

bool DefinitionMgrClass::Save(ChunkSaveClass& csave)
{
	csave.Begin_Chunk(256);
	Save_Variables(csave);
	csave.End_Chunk();
	csave.Begin_Chunk(257);
	bool b = (Save_Objects(csave) & 1) != 0;
	csave.End_Chunk();
	return b;
}

bool DefinitionMgrClass::Load(ChunkLoadClass& cload)
{
	bool b = true;
	while (cload.Open_Chunk())
	{
		switch (cload.Cur_Chunk_ID())
		{
		case 256:
			b = (b & Load_Variables(cload)) != 0;
			break;
		case 257:
			b = (b & Load_Objects(cload)) != 0;
			break;
		}
		cload.Close_Chunk();
	}
	return b;
}
