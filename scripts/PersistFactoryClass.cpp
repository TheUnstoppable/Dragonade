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
#include "general.h"

#include "PersistFactoryClass.h"
#include "SaveLoadSystemClass.h"
#include "PointerRemapClass.h"
#pragma warning(disable: 4073) //warning C4073: initializers put in library initialization area - That's EXACTLY why I put that pragma in...
#pragma init_seg(lib) // Move this files static initializers up a level
#pragma warning(default: 4073)
PersistFactoryClass::PersistFactoryClass() : NextFactory(0)
{
	SaveLoadSystemClass::Register_Persist_Factory(this);
}

PersistFactoryClass::~PersistFactoryClass()
{
	SaveLoadSystemClass::Unregister_Persist_Factory(this);
}

SaveLoadSubSystemClass::SaveLoadSubSystemClass()
{
	NextSubSystem = 0;
	SaveLoadSystemClass::Register_Sub_System(this);
}

SaveLoadSubSystemClass::~SaveLoadSubSystemClass()
{
	SaveLoadSystemClass::Unregister_Sub_System(this);
}

bool SaveLoadSubSystemClass::Contains_Data() const
{
	return true;
}

void SaveLoadSystemClass::Register_Persist_Factory(PersistFactoryClass *factory)
{
	Link_Factory(factory);
}

void SaveLoadSystemClass::Unregister_Persist_Factory(PersistFactoryClass *factory)
{
	Unlink_Factory(factory);
}

void SaveLoadSystemClass::Link_Factory(PersistFactoryClass *factory)
{
	if (factory)
	{
		factory->NextFactory = _FactoryListHead;
		_FactoryListHead = factory;
	}
}

void SaveLoadSystemClass::Unlink_Factory(PersistFactoryClass *factory)
{
	PersistFactoryClass *fac;
	PersistFactoryClass *fac2 = 0;
	for (fac = _FactoryListHead;fac != factory;fac = fac->NextFactory)
	{
		fac2 = fac;
	}
	if (fac2)
	{
		fac2->NextFactory = factory->NextFactory;
	}
	else
	{
		_FactoryListHead = factory->NextFactory;
	}
	factory->NextFactory = 0;
}

PersistFactoryClass *SaveLoadSystemClass::Find_Persist_Factory(uint32 id)
{
	for (PersistFactoryClass *fac = _FactoryListHead;fac;fac = fac->NextFactory)
	{
		if (fac->Chunk_ID() == id)
		{
			return fac;
		}
	}
	return 0;
}

void SaveLoadSystemClass::Register_Sub_System(SaveLoadSubSystemClass *SubSystem)
{
	Link_Sub_System(SubSystem);
}

void SaveLoadSystemClass::Unregister_Sub_System(SaveLoadSubSystemClass *SubSystem)
{
	Unlink_Sub_System(SubSystem);
}

void SaveLoadSystemClass::Link_Sub_System(SaveLoadSubSystemClass *SubSystem)
{
	if (SubSystem)
	{
		SubSystem->NextSubSystem = _SubSystemListHead;
		_SubSystemListHead = SubSystem;
	}
}

void SaveLoadSystemClass::Unlink_Sub_System(SaveLoadSubSystemClass *SubSystem)
{
	SaveLoadSubSystemClass *fac;
	SaveLoadSubSystemClass *fac2 = 0;
	for (fac = _SubSystemListHead;fac != SubSystem;fac = fac->NextSubSystem)
	{
		fac2 = fac;
	}
	if (fac2)
	{
		fac2->NextSubSystem = SubSystem->NextSubSystem;
	}
	else
	{
		_SubSystemListHead = SubSystem->NextSubSystem;
	}
	SubSystem->NextSubSystem = 0;
}

SaveLoadSubSystemClass *SaveLoadSystemClass::Find_Sub_System(uint32 id)
{
	for (SaveLoadSubSystemClass *fac = _SubSystemListHead;fac;fac = fac->NextSubSystem)
	{
		if (fac->Chunk_ID() == id)
		{
			return fac;
		}
	}
	return 0;
}

#if (TDBEDIT) || (DDBEDIT) || (W3DVIEWER)
SaveLoadSubSystemClass *SaveLoadSystemClass::_SubSystemListHead;
PersistFactoryClass *SaveLoadSystemClass::_FactoryListHead;
PointerRemapClass SaveLoadSystemClass::PointerRemapper;
#else
REF_DEF3(PointerRemapClass, SaveLoadSystemClass::PointerRemapper, 0x00830398, 0x0082F580, 0x008B5D70);
REF_DEF3(PersistFactoryClass *, SaveLoadSystemClass::_FactoryListHead, 0x008303E8, 0x0082F5D0, 0x008B5DC0);
REF_DEF3(SaveLoadSubSystemClass *, SaveLoadSystemClass::_SubSystemListHead, 0x008303E4, 0x0082F5CC, 0x008B5DBC);
#endif

void SaveLoadSystemClass::Register_Pointer(void *ptr1, void *ptr2)
{
	PointerRemapper.Register_Pointer(ptr1,ptr2);
}

void SaveLoadSystemClass::Request_Pointer_Remap(void **ptr)
{
	PointerRemapper.Request_Pointer_Remap(ptr);
}

void SaveLoadSystemClass::Request_Ref_Counted_Pointer_Remap(RefCountClass **ptr)
{
	PointerRemapper.Request_Ref_Counted_Pointer_Remap(ptr);
}
