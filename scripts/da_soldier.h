/*	Renegade Scripts.dll
    Dragonade Soldier Manager
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DASOLDIER
#define INCLUDE_DASOLDIER

#include "da_event.h"

class DASoldierManager : public DAEventClass {
public:
	static void Init();
	DA_API static HashTemplateClass<unsigned int,DynamicVectorClass<const WeaponDefinitionClass*>>  &Get_Exclusive_Weapons();
	DA_API static HashTemplateClass<unsigned int,const WeaponDefinitionClass*> &Get_Replace_Weapons();
	DA_API static HashTemplateClass<unsigned int,DynamicVectorClass<const WeaponDefinitionClass*>>  &Get_Remove_Weapons();

private:
	virtual void Settings_Loaded_Event();
	virtual void Object_Created_Event(GameObject *obj);
	virtual void Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	virtual int Character_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const SoldierGameObjDef *Item);

	//Settings
	HashTemplateClass<unsigned int,DynamicVectorClass<const WeaponDefinitionClass*>> GrantWeapons; //SoldierGameObjDef*,WeaponDefinitionClass*
	static HashTemplateClass<unsigned int,DynamicVectorClass<const WeaponDefinitionClass*>> ExclusiveWeapons; //WeaponDefinitionClass*,WeaponDefinitionClass*
	static HashTemplateClass<unsigned int,const WeaponDefinitionClass*> ReplaceWeapons; //WeaponDefinitionClass*,WeaponDefinitionClass*
	static HashTemplateClass<unsigned int,DynamicVectorClass<const WeaponDefinitionClass*>> RemoveWeapons; //WeaponDefinitionClass*,WeaponDefinitionClass*
};

#endif
