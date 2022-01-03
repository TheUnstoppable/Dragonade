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
#include "General.h"
#include "DefinitionFactoryMgrClass.h"
#include "DefinitionFactoryClass.h"
#pragma warning(disable: 4073) //warning C4073: initializers put in library initialization area - That's EXACTLY why I put that pragma in...
#pragma init_seg(lib) // Move this files static initializers up a level
#pragma warning(default: 4073)
#if (TDBEDIT || DDBEDIT || W3DVIEWER)
DefinitionFactoryClass *DefinitionFactoryMgrClass::_FactoryListHead;
#else
REF_DEF3(DefinitionFactoryClass *, DefinitionFactoryMgrClass::_FactoryListHead, 0x00830450, 0x0082F638, 0x008B5DF4);
#endif
void DefinitionFactoryMgrClass::Register_Factory(DefinitionFactoryClass* factory)
{
	Link_Factory(factory);
}

void DefinitionFactoryMgrClass::Unregister_Factory(DefinitionFactoryClass* factory)
{
	Unlink_Factory(factory);
}

void DefinitionFactoryMgrClass::Link_Factory(DefinitionFactoryClass *factory)
{
	DefinitionFactoryClass *fac = _FactoryListHead;
	factory->m_NextFactory = _FactoryListHead;
	if (fac)
	{
		fac->m_PrevFactory = factory;
	}
	_FactoryListHead = factory;
}

void DefinitionFactoryMgrClass::Unlink_Factory(DefinitionFactoryClass *factory)
{
	if (factory->m_PrevFactory)
	{
		factory->m_PrevFactory->m_NextFactory = factory->m_NextFactory;
	}
	else
	{
		_FactoryListHead = factory->m_NextFactory;
	}
	if (factory->m_NextFactory)
	{
		factory->m_NextFactory->m_PrevFactory = factory->m_PrevFactory;
	}
	factory->m_NextFactory = 0;
	factory->m_PrevFactory = 0;
}

DefinitionFactoryClass *DefinitionFactoryMgrClass::Find_Factory(uint32 id)
{
	for (DefinitionFactoryClass *fac = _FactoryListHead;fac;fac = fac->m_NextFactory)
	{
		if (fac->Get_Class_ID() == id)
		{
			return fac;
		}
	}
	return 0;
}

DefinitionFactoryClass *DefinitionFactoryMgrClass::Get_First(uint32 id)
{
	for (DefinitionFactoryClass *i = _FactoryListHead;i;i = i->m_NextFactory)
	{
		uint32 clsid = i->Get_Class_ID();
		uint32 clsid2 = clsid - 0x1000;
		if (clsid - 0x1000 < 0)
		{
			clsid2 = clsid - 1;
		}
		if ((clsid2 & 0xFFFFF000) + 0x1000 == id)
		{
			return i;
		}
	}
	return 0;
}

DefinitionFactoryClass *DefinitionFactoryMgrClass::Get_Next(DefinitionFactoryClass *factory,uint32 id)
{
	for (DefinitionFactoryClass *i = factory->m_NextFactory;i;i = i->m_NextFactory)
	{
		uint32 clsid = i->Get_Class_ID();
		uint32 clsid2 = clsid - 0x1000;
		if (clsid - 0x1000 < 0)
		{
			clsid2 = clsid - 1;
		}
		if ((clsid2 & 0xFFFFF000) + 0x1000 == id)
		{
			return i;
		}
	}
	return 0;
}
