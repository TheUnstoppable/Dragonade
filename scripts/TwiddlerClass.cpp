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
#include "TwiddlerClass.h"
#include "SimplePersistFactoryClass.h"
#include "SimpleDefinitionFactoryClass.h"
#include "engine_io.h"
#ifdef PARAM_EDITING_ON
#include "parameterimpl.h"
#endif
#include "SysTimeClass.h"
#include "RandomClass.h"
#include "DefinitionMgrClass.h"
void forcelinktwiddler()
{
}
SimplePersistFactoryClass<TwiddlerClass, 258> _TwiddlerPersistFactory;
DECLARE_DEFINITION_FACTORY(TwiddlerClass,57344,"Twiddler") _TwiddlerFactory;
TwiddlerClass::TwiddlerClass() : m_IndirectClassID(0)
{
#ifdef PARAM_EDITING_ON
	CLASSID_DEFIDLIST_PARAM(TwiddlerClass,m_DefinitionList,0,m_IndirectClassID,"Preset List");
#endif
}


TwiddlerClass::~TwiddlerClass()
{
}

DefinitionClass *TwiddlerClass::Twiddle() const
{
	if (m_DefinitionList.Count() > 0)
	{
		RandomClass random(TIMEGETTIME());
		int pos = random(0,m_DefinitionList.Count() - 1);
		return DefinitionMgrClass::Find_Definition(m_DefinitionList[pos],true);
	}
	return 0;
}

PersistClass *TwiddlerClass::Create() const
{
	DefinitionClass *def = Twiddle();
	if (def)
	{
		return def->Create();
	}
	return 0;
}

const PersistFactoryClass &TwiddlerClass::Get_Factory() const
{
	return _TwiddlerPersistFactory;
}

bool TwiddlerClass::Save(ChunkSaveClass &csave)
{
	csave.Begin_Chunk(256);
	bool b = Save_Variables(csave);
	csave.End_Chunk();
	csave.Begin_Chunk(512);
	bool b2 = DefinitionClass::Save(csave);
	csave.End_Chunk();
	return b && b2;
}

bool TwiddlerClass::Load(ChunkLoadClass &cload)
{
	bool b = false;
	bool b2 = false;
	while (cload.Open_Chunk())
	{
		switch (cload.Cur_Chunk_ID())
		{
			case 256:
				b = Load_Variables(cload);
				break;
			case 512:
				b2 = DefinitionClass::Load(cload);
				break;
			default:
				break;
		}
		cload.Close_Chunk();
	}
	return b && b2;
}

bool TwiddlerClass::Save_Variables(ChunkSaveClass &csave)
{
	WRITE_MICRO_CHUNK(csave,2,m_IndirectClassID);
	for (int i = 0;i < m_DefinitionList.Count();i++)
	{
		int id = m_DefinitionList[i];
		WRITE_MICRO_CHUNK(csave,1,id);
	}
	return true;
}

bool TwiddlerClass::Load_Variables(ChunkLoadClass &cload)
{
	m_DefinitionList.Delete_All();
	while (cload.Open_Micro_Chunk())
	{
		switch (cload.Cur_Micro_Chunk_ID())
		{
		case 1:
			{
				int id;
				cload.Read(&id,sizeof(id));
				m_DefinitionList.Add(id);
			}
			break;
			READ_MICRO_CHUNK(cload,2,m_IndirectClassID);
			default:
				break;
		}
		cload.Close_Micro_Chunk();
	}
	return true;
}

#ifdef DDBEDIT
void TwiddlerClass::Dump(FileClass &file)
{
	for (int i = 0;i < this->Get_Parameter_Count();i++)
	{
		ParameterClass *param = this->Lock_Parameter(i);
		param->Dump(file);
		this->Unlock_Parameter(i);
	}
}
#endif
