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

ScriptFactory::ScriptFactory(const char* name, const char* param) : mNext(NULL)
{
	ScriptName = name;
	ParamDescription = param;
	ScriptRegistrar::RegisterScript(this);
}

ScriptFactory* ScriptFactory::GetNext() const
{
	return mNext;
}

const char *ScriptFactory::GetName()
{
	return ScriptName;
}

const char *ScriptFactory::GetParamDescription()
{
	return ParamDescription;
}

ScriptFactory::~ScriptFactory()
{
	ScriptRegistrar::UnregisterScript(this);
	ScriptName = NULL;
	ParamDescription = NULL;
}

void ScriptFactory::SetNext(ScriptFactory* link)
{
	if (mNext != NULL)
	{
		link->SetNext(mNext);
	}
	mNext = link;
}
