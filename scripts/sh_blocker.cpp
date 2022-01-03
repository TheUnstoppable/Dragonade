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
#include "engine.h"
#include "ScriptableGameObj.h"
#include "PhysicalGameObj.h"

// CustomCollisionGroup - When this script becomes attached to an object, it gives it a custom collision group. Upon detach, the original collision group is restored.
class SH_CustomCollisionGroup: public ScriptImpClass
{
protected:

	Collision_Group_Type original_group;		// needed to undo custom collision group on detach

public:

	void Created(GameObject* obj)
	{
		// pull out the physical_obj
		PhysicalGameObj* physical_obj = obj->As_PhysicalGameObj();

		// only physical objects can play
		if (!physical_obj) return;

		// pull out the phys
		PhysClass* phys = physical_obj->Peek_Physical_Object();

		// backup the original collision group
		original_group = phys->Get_Collision_Group();

		// tweak the current collision group
		phys->Set_Collision_Group((Collision_Group_Type)Get_Int_Parameter("CollisionGroup"));

		// inform netcode of the dirty things we've done with the phys
		physical_obj->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE, true);
	};

	void Detach(GameObject* obj)
	{
		// base implementation
		ScriptImpClass::Detach(obj);

		if (Exe != 4) //make sure we dont try things in LE that may cause issues
		{
			// pull out the physical_obj
			PhysicalGameObj* physical_obj = obj->As_PhysicalGameObj();
	
			// only physical objects can play
			if (!physical_obj) return;
	
			// pull out the phys
			PhysClass* phys = physical_obj->Peek_Physical_Object();
	
			// restore the original collision group
			phys->Set_Collision_Group(original_group);
	
			// inform netcode of the dirty things we've done with the phys
			physical_obj->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE, true);
		}
	};
};

#define REGISTER_SCRIPT(name, params) ScriptRegistrant<name> name##Registrant(#name, params);

REGISTER_SCRIPT(SH_CustomCollisionGroup, "CollisionGroup=0:int");