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
#include "Definition.h"
#include "DefinitionMgrClass.h"
#include "engine_io.h"

void DefinitionClass::Set_ID(uint32 id)
{
	m_ID = id;
	if (m_DefinitionMgrLink != -1)
	{
		DefinitionMgrClass::Unregister_Definition(this);
		DefinitionMgrClass::Register_Definition(this);
	}
}

bool DefinitionClass::Save(ChunkSaveClass &csave)
{
	csave.Begin_Chunk(0x100);
	bool b = (Save_Variables(csave) & 1) != 0;
	csave.End_Chunk();
	return b;
}

bool DefinitionClass::Load(ChunkLoadClass &cload)
{
	while (cload.Open_Chunk())
	{
		if (cload.Cur_Chunk_ID() == 0x100)
		{
			Load_Variables(cload);
		}
		cload.Close_Chunk();
	}
	return true;
}

bool DefinitionClass::Save_Variables(ChunkSaveClass &csave)
{
	WRITE_MICRO_CHUNK(csave,1,m_ID);
	WRITE_MICRO_CHUNK_WWSTRING(csave,3,m_Name);
	return true;
}

bool DefinitionClass::Load_Variables(ChunkLoadClass &cload)
{
	while (cload.Open_Micro_Chunk())
	{
		switch (cload.Cur_Micro_Chunk_ID())
		{
			READ_MICRO_CHUNK(cload,1,m_ID);
			READ_MICRO_CHUNK_WWSTRING(cload,3,m_Name);
		}
		cload.Close_Micro_Chunk();
	}
	return true;
}

#ifdef DDBEDIT
void DefinitionClass::Dump(FileClass &file)
{
	for (int i = 0;i < this->Get_Parameter_Count();i++)
	{
		ParameterClass *param = this->Lock_Parameter(i);
		param->Dump(file);
		this->Unlock_Parameter(i);
	}
}

void DefinitionClass::DumpPhys(FileClass &phys)
{
}
#endif
