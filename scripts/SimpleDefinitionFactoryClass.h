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
#ifndef TT_INCLUDE_SIMPLEDEFINITIONFACTORYCLASS_H
#define TT_INCLUDE_SIMPLEDEFINITIONFACTORYCLASS_H
#include "DefinitionFactoryClass.h"
#include "DefinitionFactoryMgrClass.h"
template <class T, uint32 I, const char *N>
class SimpleDefinitionFactoryClass : public DefinitionFactoryClass
{
public:
	bool IsDisplayed;
	SimpleDefinitionFactoryClass(bool is_displayed = true) : IsDisplayed(is_displayed)
	{
	}
	~SimpleDefinitionFactoryClass()
	{
	}
	DefinitionClass *Create() const
	{
		return new T;
	}
	const char *Get_Name() const
	{
		return N;
	}
	uint32 Get_Class_ID() const
	{
		return I;
	}
	bool Is_Displayed() const
	{
		return IsDisplayed;
	}
};
#define DECLARE_DEFINITION_FACTORY(definition,classid,string) \
	char definition##name[sizeof(string)] = string; \
	SimpleDefinitionFactoryClass<definition,classid,(const char *)definition##name>
#endif
