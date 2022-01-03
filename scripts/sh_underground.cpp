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
#include "MoveablePhysClass.h"
#include "VehicleGameObj.h"

// customs that are used by the underground script
enum
{
	SH_UNDERGROUND_LOCKSTATE = 420001, // set topmost lock state to param
	SH_UNDERGROUND_PUSHLOCKSTATE, // push param on new topmost lock state
	SH_UNDERGROUND_POPLOCKSTATE, // pop to old lock state
	SH_UNDERGROUND_SETSTATE, // attempt to set underground state to param
	SH_UNDERGROUND_TOGGLESTATE, // attempt to toggle underground state
	SH_UNDERGROUND_DETACH,
};

// UndergroundDigZone - Objects inside a zone with this script attached can enter and exit the underground state
class SH_UndergroundDigZone: public ScriptImpClass
{
public:

	void Entered(GameObject *zone, GameObject *object)
	{
		// push an "unlocked" state on the underground state stack
		Commands->Send_Custom_Event(zone, object, SH_UNDERGROUND_PUSHLOCKSTATE, 0, 0.0f);
	};

	void Exited(GameObject *zone, GameObject *object)
	{
		// pop what we pushed on the state stack
		Commands->Send_Custom_Event(zone, object, SH_UNDERGROUND_POPLOCKSTATE, 0, 0.0f);
	};
};

// UndergroundNoDigZone - Objects inside a zone with this script attached cannot enter and exit the underground state
class SH_UndergroundNoDigZone: public ScriptImpClass
{
public:

	void Entered(GameObject *zone, GameObject *object)
	{
		// push a "locked" state on the underground state stack
		Commands->Send_Custom_Event(zone, object, SH_UNDERGROUND_PUSHLOCKSTATE, 1, 0.0f);
	};

	void Exited(GameObject *zone, GameObject *object)
	{
		// pop what we pushed on the state stack
		Commands->Send_Custom_Event(zone, object, SH_UNDERGROUND_POPLOCKSTATE, 0, 0.0f);
	};
};

// UndergroundVehicle - Vehicles with this script attached can enter and exit the underground state
class SH_UndergroundVehicle : public ScriptImpClass 
{
protected:

	Collision_Group_Type	original_group;		// needed to undo underground state
	ArmorType				original_skin;		// ^
	ArmorType				original_armor;		// ^
	const char *            original_weapon;    // ^
	bool					underground;		// "am I currently underground?"
	bool					could_fire;			// "could I fire before I went underground?"
	bool					dig_pending;		// "is there a dig animation pending completion?"
    bool                    surface_pending;
	unsigned int			lock_state;			// stack of "can I set underground?"
	int						lock_state_index;	// index of topmost ^that
	time_t					lastTransition;		//!< Time of the last state transition
	int						cooldown;			//!< Length of cooldown timer, or 0 if no cooldown is used

	bool IsStateLocked()
	{
		// is the top-most state locked
		return (lock_state & (1 << lock_state_index)) != 0;
	};

	void SetStateLocked(bool onoff)
	{
		GameObject* owner = Owner();
		if (owner->As_VehicleGameObj())
		{
			if (onoff)
			{
				Vector3 v(0.9f,0,0);
				owner->As_VehicleGameObj()->Set_Color(v);
			}
			else
			{
				Vector3 v(1,1,1);
				owner->As_VehicleGameObj()->Set_Color(v);
			}
		}
		// set the top-most state
		if (onoff) lock_state |= (1 << lock_state_index);
		else lock_state &= ~(1 << lock_state_index);
	};

	void PushStateLocked(bool onoff)
	{
		// push a new state on
		++lock_state_index;

		// and set it to onoff
		SetStateLocked(onoff);
	};

	void PopStateLocked()
	{
		// pop off the topmost state
		--lock_state_index;
		
		SetStateLocked(IsStateLocked()); //use this to update the color
		// paranoia check
		TT_ASSERT("LockState popped too many times" && lock_state_index >= 0);
	};

	bool CheckCooldown(GameObject* requester)
	{
		if (cooldown > 0 && cooldown > time(NULL) - lastTransition)
		{
			if (-1 != Get_Player_ID(requester))
			{
				Send_Message_Player(requester, 255,50,50, StringClass::getFormattedString("You cannot %s for another %d seconds",
					(underground?"surface":"dig"), cooldown - (time(NULL) - lastTransition)));
			}

			return false;
		}

		return true;
	};

	void EnterUndergroundMode(bool force = false)
	{
		if (underground || dig_pending || surface_pending) return; // already entered or state locked and not forced
		if (IsStateLocked() && !force)
		{
			unsigned int red;
			unsigned int green;
			unsigned int blue;
			Get_Team_Color(Get_Player_Type(Owner()),&red,&green,&blue);
			Send_Message_Player(Get_Vehicle_Driver(Owner()),red,green,blue,Get_Parameter("DigMessage"));
			return;
		}

		// stash away the owner
		GameObject* owner = Owner();

		// pull out the physical object
		PhysicalGameObj* physical_object = owner->As_PhysicalGameObj();

		// only physical objects can play
		if (!physical_object) return;

		// pull out the movable phys
		MoveablePhysClass* movable_phys = physical_object->Peek_Physical_Object()->As_MoveablePhysClass();

		// only movable objects can play
		if (!movable_phys) return;

        // can't dig while EMP'd
		VehicleGameObj* vehicle = owner->As_VehicleGameObj();
		if (vehicle && !vehicle->Can_Drive()) return;

		// backup the original group for when we undig
		original_group = movable_phys->Get_Collision_Group();

		// go into transition mode
		movable_phys->Set_Collision_Group(UNDERGROUND_TRANSITION_COLLISION_GROUP);

		// check to see if we can dig
		const Matrix3D& new_tm = movable_phys->Get_Transform();
		if (!movable_phys->Can_Teleport(new_tm) && !force)
		{
			// we can't, so back to our original group and abort
			movable_phys->Set_Collision_Group(original_group);
			return;
		};

		// no weapon firing is allowed
		{
			if (vehicle)
			{
				could_fire = vehicle->Get_Scripts_Can_Fire();
				vehicle->Set_Scripts_Can_Fire(false);
			};
		};

		original_weapon = Get_Current_Weapon(owner);
		Commands->Select_Weapon(owner,0);

		// nobody gets in or out...
		Commands->Enable_Vehicle_Transitions(owner, false);
        Commands->Create_Explosion(Get_Parameter("DigExplosion"), Commands->Get_Position(owner), owner);

		// defuse attached C4
		Defuse_Attached_C4(owner);

		// start digging
		const char* animation = Get_Parameter("DigAnimation");
		
		if (*animation)
		{
			dig_pending = true;
            movable_phys->Set_Immovable(true);
			Commands->Set_Animation(owner, animation, false, 0, 0, -1, false);
		}
		else
		{
			CompleteDig(owner);
		}
	};

	void CompleteDig(GameObject* obj)
	{
		PhysicalGameObj* physical_object = obj->As_PhysicalGameObj();
		MoveablePhysClass* movable_phys = physical_object->Peek_Physical_Object()->As_MoveablePhysClass();

		// we aren't visible to AI
		Commands->Set_Is_Visible(obj, false);

		// become invulnerable
		original_skin = physical_object->Get_Defense_Object()->Get_Skin();
		original_armor = physical_object->Get_Defense_Object()->Get_Shield_Type();
		Set_Skin(obj, "Blamo");
		Commands->Set_Shield_Type(obj, "Blamo");

		// switch to the underground collision group
        movable_phys->Set_Immovable(false);
		movable_phys->Set_Collision_Group(UNDERGROUND_COLLISION_GROUP);

        // clear animation
        physical_object->Clear_Animation();

		// you are now underground, congrats
		underground = true;

		lastTransition = time(NULL);
	};

    void ExitUndergroundMode(bool force = false)
    {
        if (!underground || dig_pending || surface_pending) return; // already left or state locked and not forced
		if (IsStateLocked() && !force)
		{
			unsigned int red;
			unsigned int green;
			unsigned int blue;
			Get_Team_Color(Get_Player_Type(Owner()),&red,&green,&blue);
			Send_Message_Player(Get_Vehicle_Driver(Owner()),red,green,blue,Get_Parameter("SurfaceMessage"));
			return;
		}

        GameObject* owner = Owner();

        PhysicalGameObj* physical_object = owner->As_PhysicalGameObj();
        if (!physical_object) return;

        MoveablePhysClass* movable_phys = physical_object->Peek_Physical_Object()->As_MoveablePhysClass();
        if (!movable_phys) return;

        movable_phys->Set_Collision_Group(UNDERGROUND_TRANSITION_COLLISION_GROUP);

        // check to see if we can surface
        const Matrix3D& new_tm = movable_phys->Get_Transform();
        if (!movable_phys->Can_Teleport(new_tm) && !force)
        {
            // we can't, so back to underground we go
            movable_phys->Set_Collision_Group(UNDERGROUND_COLLISION_GROUP);
            return;
        };

        Commands->Set_Is_Visible(owner, true);
        Commands->Create_Explosion(Get_Parameter("SurfaceExplosion"), Commands->Get_Position(owner), owner);

        // no longer invulnerable
        physical_object->Get_Defense_Object()->Set_Skin(original_skin);
        physical_object->Get_Defense_Object()->Set_Shield_Type(original_armor);

        const char* animation = Get_Parameter("SurfaceAnimation");
        if (*animation)
        {
            surface_pending = true;
            movable_phys->Set_Immovable(true);
            Commands->Set_Animation(owner, animation, false, 0, 0, -1, false);
        }
        else
        {
            physical_object->Clear_Animation();
            CompleteSurface(owner);
        }
    }

    void CompleteSurface(GameObject* obj)
    {
        PhysicalGameObj* physical_object = obj->As_PhysicalGameObj();
        MoveablePhysClass* movable_phys = physical_object->Peek_Physical_Object()->As_MoveablePhysClass();

        // restore the original collision group
        movable_phys->Set_Immovable(false);
        movable_phys->Set_Collision_Group(original_group);

		Commands->Select_Weapon(obj,original_weapon);

        // weapon firing is now allowed
        {
            VehicleGameObj* vehicle = obj->As_VehicleGameObj();
            if (vehicle) vehicle->Set_Scripts_Can_Fire(could_fire);
        };

        Commands->Enable_Vehicle_Transitions(obj, true);

        // no longer mark us as underground
        underground = false;

        lastTransition = time(NULL);
    };

public:

  void Attach(GameObject* obj)
	{
		// base implementation
		ScriptImpClass::Attach(obj);

		// make sure we are in the default state for our attached object
		underground = false;
		dig_pending = false;
        surface_pending = false;
		lock_state = 0;
		lock_state_index = 0;
		lastTransition = 0;
		cooldown = (Get_Parameter_Count() >= 9) ? Get_Int_Parameter("Cooldown") : 0;

		// hook up any customizations
		SetStateLocked(Get_Int_Parameter("DefaultLockState") != 0);
	};

	void Detach(GameObject* obj)
	{
		if (Exe != 4) //make sure we dont try things in LE that may cause issues
		{
			// force exit of underground (don't want any objects stuck underground)
			ExitUndergroundMode(true);
		}

		// base implementation
		ScriptImpClass::Detach(obj);
	};

	void Custom(GameObject *obj, int type, int param, GameObject *sender)
	{
		switch(type)
		{
		case SH_UNDERGROUND_LOCKSTATE:
			// set the top-most lock state
			SetStateLocked(param != 0);
			break;
		case SH_UNDERGROUND_PUSHLOCKSTATE:
			// push a new top-most lock state
			PushStateLocked(param != 0);
			break;
		case SH_UNDERGROUND_POPLOCKSTATE:
			// pop to the old top-most lock state
			PopStateLocked();
			break;
		case SH_UNDERGROUND_SETSTATE:
			// attempt to set the current underground state
			if (param) EnterUndergroundMode();
			else ExitUndergroundMode();
			break;
		case SH_UNDERGROUND_TOGGLESTATE:
			if (CheckCooldown(sender))
			{
				// toggle states
				if (!underground) EnterUndergroundMode();
				else ExitUndergroundMode();
			}
			break;
		case SH_UNDERGROUND_DETACH:
			Destroy_Script();
			break;
		};
	};

    void Animation_Complete(GameObject *obj, const char *animation_name)
    {
        VehicleGameObj* vehicle = obj ? obj->As_VehicleGameObj() : NULL;
        if (!vehicle) return;

        if (dig_pending && !_stricmp(animation_name, Get_Parameter("DigAnimation")))
        {
            dig_pending = false;
            CompleteDig(obj);
        }
        else if (surface_pending && !_stricmp(animation_name, Get_Parameter("SurfaceAnimation")))
        {
            surface_pending = false;
            CompleteSurface(obj);
        }
    };
};

class SH_UndergroundKey: public JFW_Key_Hook_Base
{
public:
	void Created(GameObject *obj)
	{
		InstallHook(Get_Parameter("Key"),obj);
	}
	void KeyHook()
	{
		GameObject *owner = Owner();
		if (Get_Vehicle(owner) && Get_Vehicle_Driver(Get_Vehicle(owner)) == owner)
		{
			Commands->Send_Custom_Event(owner,Get_Vehicle(owner),SH_UNDERGROUND_TOGGLESTATE,0,0);
		}
	}
};

#define REGISTER_SCRIPT(name, params) ScriptRegistrant<name> name##Registrant(#name, params);

REGISTER_SCRIPT(SH_UndergroundDigZone, "");
REGISTER_SCRIPT(SH_UndergroundNoDigZone, "");
REGISTER_SCRIPT(SH_UndergroundVehicle, "DefaultLockState=0:int,DigAnimation:string,SurfaceAnimation:string,Velocity:float,DigExplosion:string,SurfaceExplosion:string,DigMessage:string,SurfaceMessage:string,Cooldown:int");
REGISTER_SCRIPT(SH_UndergroundKey, "Key=Key:string");
