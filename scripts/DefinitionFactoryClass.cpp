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
#include "DefinitionFactoryClass.h"
#include "DefinitionFactoryMgrClass.h"
#include "SimpleDefinitionFactoryClass.h"
DefinitionFactoryClass::DefinitionFactoryClass() : m_NextFactory(0), m_PrevFactory(0)
{
	DefinitionFactoryMgrClass::Register_Factory(this);
}

DefinitionFactoryClass::~DefinitionFactoryClass()
{
	DefinitionFactoryMgrClass::Unregister_Factory(this);
}
