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
#ifndef SCRIPTS_INCLUDE__ENGINE_DEF_H
#define SCRIPTS_INCLUDE__ENGINE_DEF_H
#include "scripts.h"

class DefinitionClass;
const char SCRIPTS_API *Get_Definition_Name(unsigned long id); //Convert a definition/preset ID into a name
int SCRIPTS_API Get_Definition_ID(const char *name); //Convert a definition/preset name into an ID
DefinitionClass SCRIPTS_API *Find_Named_Definition(const char *name); //Find a DefinitionClass given its name
DefinitionClass SCRIPTS_API *Find_Definition(unsigned long id); //Find a DefinitionClass given its ID
unsigned long SCRIPTS_API Get_Definition_Class_ID(const char *name); //get the class ID of a definition (matches the #defines above)
DefinitionClass SCRIPTS_API *Get_Phys_Definition(GameObject *obj); //Get a DefinitionClass for the physics class given a PhysicalGameObj
bool SCRIPTS_API Is_Valid_Preset_ID(unsigned long ID); //Is this a valid preset id
bool SCRIPTS_API Is_Valid_Preset(const char *Preset); //Is this a valid preset name

#endif
