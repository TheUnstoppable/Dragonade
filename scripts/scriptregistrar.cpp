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

#include "scripts.h"
ScriptFactory *ScriptRegistrar::mScriptFactories = NULL;

void ScriptRegistrar::UnregisterScript(ScriptFactory* factory)
{
	ScriptFactory* previous = NULL;
	ScriptFactory* current = mScriptFactories;
	while (current != NULL)
	{
		ScriptFactory* next = current->GetNext();
		if (current == factory)
		{
			if (previous == NULL)
			{
				mScriptFactories = next;
			}
			else
			{
				previous->SetNext(next);
			}
		}
		previous = current;
		current = next;
	}
}

void ScriptRegistrar::RegisterScript(ScriptFactory* factory)
{
	if (factory != NULL)
	{
		factory->SetNext(mScriptFactories);
		mScriptFactories = factory;
	}
}

ScriptFactory* ScriptRegistrar::GetScriptFactory(const char* name)
{
	if (name != NULL)
	{
		ScriptFactory* factory = mScriptFactories;
		while (factory != NULL)
		{
			if (_stricmp(factory->GetName(), name) == 0)
			{
				return factory;
			}
			factory = factory->GetNext();
		}
	}
	return NULL;
}

ScriptFactory* ScriptRegistrar::GetScriptFactory(int index)
{
	int count = 0;
	ScriptFactory* factory = mScriptFactories;
	while (factory != NULL)
	{
		if (count == index)
		{
			return factory;
		}
		count++;
		factory = factory->GetNext();
	}
	return NULL;
}

int ScriptRegistrar::Count()
{
	int count = 0;
	ScriptFactory* factory = mScriptFactories;
	while (factory != NULL)
	{
		count++;
		factory = factory->GetNext();
	}
	return count;
}

ScriptImpClass* ScriptRegistrar::CreateScript(const char* scriptName)
{
	if (scriptName != NULL)
	{
		ScriptFactory* factory = mScriptFactories;
		while (factory != NULL)
		{
			if (_stricmp(factory->GetName(), scriptName) == 0)
			{
				return factory->Create();
			}
			factory = factory->GetNext();
		}
	}
	return NULL;
}
