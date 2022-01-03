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
#include "engine_common.h"
#include "engine_vector.h"
#include "engine_threading.h"
#include "engine_string.h"
#include "WeaponMgr.h"
#include "DefinitionMgrClass.h"
#include "WeaponManager.h"



const WeaponDefinitionClass *WeaponManager::Find_Weapon_Definition( const char *name )
{
	return (const WeaponDefinitionClass *)DefinitionMgrClass::Find_Typed_Definition( name, 45057, true);
}
const WeaponDefinitionClass *WeaponManager::Find_Weapon_Definition( int id )
{
	return (const WeaponDefinitionClass *)DefinitionMgrClass::Find_Definition( id, true);
}
const AmmoDefinitionClass *WeaponManager::Find_Ammo_Definition( const char *name )
{
	return (const AmmoDefinitionClass *)DefinitionMgrClass::Find_Typed_Definition( name, 45058, true);
}
const AmmoDefinitionClass *WeaponManager::Find_Ammo_Definition( int id )
{
	return (const AmmoDefinitionClass *)DefinitionMgrClass::Find_Definition( id, true);
}
