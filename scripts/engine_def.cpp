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
#include "Definition.h"
#include "scripts.h"
#include "ScriptableGameObj.h"
#include "engine_obj.h"
#include "PhysicalGameObj.h"
#include "PhysClass.h"
#include "DefinitionMgrClass.h"
#include "BaseGameObjDef.h"
#include "ScriptableGameObjDef.h"

DefinitionClass SCRIPTS_API *Find_Definition(unsigned long id)
{
	return DefinitionMgrClass::Find_Definition(id,true);
}

const char SCRIPTS_API *Get_Definition_Name(unsigned long id)
{
	if (!id)
	{
		return "none";
	}
	DefinitionClass *d = DefinitionMgrClass::Find_Definition(id,1);
	if (!d)
	{
		return "none";
	}
	return d->Get_Name();
}

DefinitionClass SCRIPTS_API *Find_Named_Definition(const char *name)
{
	return DefinitionMgrClass::Find_Named_Definition(name,1);
}

int SCRIPTS_API Get_Definition_ID(const char *name)
{
	if (!name)
	{
		return 0;
	}
	DefinitionClass *d = Find_Named_Definition(name);
	if (!d)
	{
		return 0;
	}
	return d->Get_ID();
}

DefinitionClass SCRIPTS_API *Get_Phys_Definition(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	PhysicalGameObj *o = obj->As_PhysicalGameObj();
	if (!o)
	{
		return 0;
	}
	return (DefinitionClass *)o->Peek_Physical_Object()->Get_Definition();
}

unsigned long SCRIPTS_API Get_Definition_Class_ID(const char *name)
{
	if (!name)
	{
		return 0;
	}
	DefinitionClass *d = Find_Named_Definition(name);
	if (!d)
	{
		return 0;
	}
	return d->Get_Class_ID();
}

bool SCRIPTS_API Is_Valid_Preset_ID(unsigned long ID)
{
	if (Find_Definition(ID))
	{
		return true;
	}
	return false;
}

bool SCRIPTS_API Is_Valid_Preset(const char *Preset)
{
	if (Find_Named_Definition(Preset))
	{
		return true;
	}
	return false;
}
