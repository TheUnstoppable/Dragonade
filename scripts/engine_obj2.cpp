/*	Renegade Scripts.dll
	Copyright 2013 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#include "general.h"
#include "SoldierGameObj.h"
#include "SoldierGameObjDef.h"
#include "VehicleGameObj.h"
#include "VehicleGameObjDef.h"
#include "VehicleFactoryGameObj.h"
#include "ScriptZoneGameObj.h"
#include "ScriptZoneGameObjDef.h"
#include "GameObjManager.h"
#include "engine_obj.h"
#include "engine_game.h"
#include "engine_def.h"
#include "PhysicsSceneClass.h"
#include "AirFactoryGameObj.h"
#include "AirFactoryGameObjDef.h"
#include "VehicleFactoryGameObjDef.h"
#include "NavalFactoryGameObj.h"
#include "StealthEffectClass.h"

RENEGADE_FUNCTION
GameObject *Create_Library_Object(const char *preset)
AT2(0x006C6420,0x006C5CC0);
ScriptZoneGameObj *ScriptZoneGameObj::Find_Closest_Zone(Vector3 &Location,ZoneConstants::ZoneType Type)
{
	float closest = FLT_MAX;
	ScriptZoneGameObj *closestzone = 0;
	for (SLNode<ScriptZoneGameObj>* node = GameObjManager::ScriptZoneGameObjList.Head(); node; node = node->Next())
	{
		ScriptZoneGameObj *zone = node->Data();
		if (zone->Get_Definition().Get_Type() == Type)
		{
			float distance = Vector3::Distance_Squared(zone->Get_Bounding_Box().Center,Location);
			if (distance < closest)
			{
				closest = distance;
				closestzone = zone;
			}
		}
	}
	return closestzone;
}

bool ScriptZoneGameObj::Inside_Me(PhysicalGameObj *obj)
{
    if (obj && obj->Peek_Physical_Object())
    {
        Vector3 v;
        obj->Get_Position(&v);
		return Point_In_Zone(v);
    }
    else
    {
        return false;
    }
}

RENEGADE_FUNCTION
void GrantSupplies(GameObject *obj)
AT2(0x00470AC0,0x00470230);
GameObject SCRIPTS_API *Find_Closest_Zone(Vector3 &location,unsigned int type)
{
	return ScriptZoneGameObj::Find_Closest_Zone(location,(ZoneConstants::ZoneType)type);
}

GameObject SCRIPTS_API *Get_Vehicle(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	SoldierGameObj *o = obj->As_SoldierGameObj();
	if (o)
	{
		GameObject *vehicle = o->Get_Vehicle();
		if (vehicle && vehicle->As_VehicleGameObj())
		{
			return vehicle;
		}
		return 0;
	}
	return 0;
}

void SCRIPTS_API Grant_Refill(GameObject *obj)
{
	if (!obj)
	{
		return;
	}
	GameObject *o = obj->As_SoldierGameObj();
	if (o)
	{
		GrantSupplies(obj);
	}
}

bool SCRIPTS_API Change_Character(GameObject *obj,const char *Preset_Name)
{
	if (!obj || Get_Vehicle(obj))
	{
		return false;
	}
	DefinitionClass *def = Find_Named_Definition(Preset_Name);
	if (!def)
	{
		return false;
	}
	if (def->Get_Class_ID() != 0x3001)
	{
		return false;
	}
	SoldierGameObj *o = obj->As_SoldierGameObj();
	if (!o)
	{
		return false;
	}
	o->Re_Init(*(SoldierGameObjDef *)def);
	o->Post_Re_Init();
	return true;
}

bool SCRIPTS_API Is_Available_For_Purchase(GameObject *factory)
{
	if (factory->As_BuildingGameObj() && factory->As_BuildingGameObj()->As_VehicleFactoryGameObj())
	{
		return factory->As_BuildingGameObj()->As_VehicleFactoryGameObj()->Is_Available_For_Purchase();
	}
	else if (factory->As_BuildingGameObj() && factory->As_BuildingGameObj()->As_AirFactoryGameObj())
	{
		return factory->As_BuildingGameObj()->As_AirFactoryGameObj()->Is_Available_For_Purchase();
	}
	else if (factory->As_BuildingGameObj() && factory->As_BuildingGameObj()->As_NavalFactoryGameObj())
	{
		return factory->As_BuildingGameObj()->As_NavalFactoryGameObj()->Is_Available_For_Purchase();
	}
	else
	{
		return false;
	}
}

void SCRIPTS_API Toggle_Fly_Mode(GameObject *obj)
{
	if (!obj)
	{
		return;
	}
	SoldierGameObj *o = obj->As_SoldierGameObj();
	if (o)
	{
		o->Toggle_Fly_Mode();
		Commands->Enable_Collisions(o);
	}
}

int SCRIPTS_API Get_Vehicle_Occupant_Count(GameObject *obj)
{
	if (!obj || !obj->As_VehicleGameObj())
	{
		return 0;
	}
	VehicleGameObj *o = obj->As_VehicleGameObj();
	return o->Get_Occupant_Count();
}

GameObject SCRIPTS_API *Get_Vehicle_Occupant(GameObject *obj,int seat)
{
	if (!obj || !obj->As_VehicleGameObj())
	{
		return 0;
	}
	VehicleGameObj *o = obj->As_VehicleGameObj();
	if (seat >= o->Get_Definition().Get_Seat_Count())
	{
		return 0;
	}
	return o->Get_Occupant(seat);
}

GameObject SCRIPTS_API *Get_Vehicle_Driver(GameObject *obj)
{
	if (!obj || !obj->As_VehicleGameObj())
	{
		return 0;
	}
	VehicleGameObj *o = obj->As_VehicleGameObj();
	return o->Get_Driver();
}

GameObject SCRIPTS_API *Get_Vehicle_Gunner_Pos(GameObject *obj)
{
	if (!obj || !obj->As_VehicleGameObj())
	{
		return 0;
	}
	VehicleGameObj *o = obj->As_VehicleGameObj();
	return o->Get_Gunner();
}

GameObject SCRIPTS_API *Get_Vehicle_Gunner(GameObject *obj)
{
	if (!obj || !obj->As_VehicleGameObj())
	{
		return 0;
	}
	VehicleGameObj *o = obj->As_VehicleGameObj();
	return o->Get_Actual_Gunner();
}

void SCRIPTS_API Force_Occupant_Exit(GameObject *obj,int seat)
{
	if (!obj || !obj->As_VehicleGameObj())
	{
		return;
	}
	VehicleGameObj *o = obj->As_VehicleGameObj();
	if (seat >= o->Get_Definition().Get_Seat_Count())
	{
		return;
	}
	if (o->Get_Occupant(seat))
	{
		ActionParamsStruct params;
		params.Set_Basic((long)0,100,39);
		Commands->Action_Enter_Exit(o->Get_Occupant(seat),params);
	}
}

void SCRIPTS_API Force_Occupants_Exit(GameObject *obj)
{
	if (!obj || !obj->As_VehicleGameObj())
	{
		return;
	}
	VehicleGameObj *o = obj->As_VehicleGameObj();
	int x = o->Get_Definition().Get_Seat_Count();
	ActionParamsStruct params;
	params.Set_Basic((long)0,100,39);
	for (int i = 0;i < x;i++)
	{
		if (o->Get_Occupant(i))
		{
			Commands->Action_Enter_Exit(o->Get_Occupant(i),params);
		}
	}
}

void SCRIPTS_API Soldier_Transition_Vehicle(GameObject *obj)
{
	if (!obj || !obj->As_SoldierGameObj())
	{
		return;
	}
	ActionParamsStruct params;
	params.Set_Basic((long)0,100,39);
	Commands->Action_Enter_Exit(obj,params);
}

void SCRIPTS_API Force_Occupants_Exit_Team(GameObject *obj,int team)
{
	if (!obj || !obj->As_VehicleGameObj())
	{
		return;
	}
	VehicleGameObj *o = obj->As_VehicleGameObj();
	int x = o->Get_Definition().Get_Seat_Count();
	ActionParamsStruct params;
	params.Set_Basic((long)0,100,39);
	for (int i = 0;i < x;i++)
	{
		if ((o->Get_Occupant(i)) && (Commands->Get_Player_Type(o->Get_Occupant(i)) != team))
		{
			Commands->Action_Enter_Exit(o->Get_Occupant(i),params);
		}
	}
}

GameObject SCRIPTS_API *Get_Vehicle_Return(GameObject *obj)
{
	if (!obj)
	{
		return obj;
	}
	SoldierGameObj *o = obj->As_SoldierGameObj();
	if (o)
	{
		GameObject *vehicle = o->Get_Vehicle();
		if (vehicle && vehicle->As_VehicleGameObj())
		{
			return vehicle;
		}
		return obj;
	}
	return obj;
}

bool SCRIPTS_API Get_Fly_Mode(GameObject *obj)
{
	if (!obj)
	{
		return false;
	}
	SoldierGameObj *o = obj->As_SoldierGameObj();
	if (!o)
	{
		return false;
	}
	return o->Get_Fly_Mode();
}

bool SCRIPTS_API Is_Stealth(GameObject *obj)
{
	if (!obj)
	{
		return false;
	}
	SmartGameObj *o = obj->As_SmartGameObj();
	if (!o)
	{
		return false;
	}
	return o->Is_Stealthed();
}

int SCRIPTS_API Get_Vehicle_Seat_Count(GameObject *obj)
{
	if (!obj || !obj->As_VehicleGameObj())
	{
		return 0;
	}
	VehicleGameObj *o = obj->As_VehicleGameObj();
	return o->Get_Definition().Get_Seat_Count();
}

unsigned int SCRIPTS_API Get_Vehicle_Mode(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	GameObject *o = obj->As_VehicleGameObj();
	if (!o)
	{
		obj = Get_Vehicle(obj);
		if (!obj)
		{
			return 0;
		}
	}
	const VehicleGameObjDef *d = &obj->As_VehicleGameObj()->Get_Definition();
	return (int)d->Get_Type();
}

bool SCRIPTS_API Is_VTOL(GameObject *obj)
{
	return (Get_Vehicle_Mode(obj) == VEHICLE_TYPE_FLYING) || Get_Fly_Mode(obj);
}

GameObject SCRIPTS_API *Get_Vehicle_Owner(GameObject *obj)
{	
	if (!obj)
	{
		return 0;
	}
	VehicleGameObj *o = obj->As_VehicleGameObj();
	return o->Get_Lock_Owner();
}

unsigned int SCRIPTS_API Get_Vehicle_Definition_Mode(const char *preset)
{
	VehicleGameObjDef *d = (VehicleGameObjDef *)Find_Named_Definition(preset);
	if (!d || d->Get_Class_ID() != 0x3010) return 5;
	return (int)d->Get_Type();
}

OBBoxClass SCRIPTS_API *Get_Zone_Box(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	ScriptZoneGameObj *o = obj->As_ScriptZoneGameObj();
	if (!o)
	{
		return 0;
	}
	return &o->Get_Bounding_Box();
}

bool SCRIPTS_API IsInsideZone(GameObject *zone,GameObject *obj)
{
	if (!obj)
	{
		return false;
	}
	if (!zone)
	{
		return false;
	}
	GameObject *o = obj->As_SmartGameObj();
	ScriptZoneGameObj *o2 = zone->As_ScriptZoneGameObj();
	if ((o) && (o2))
	{
		Vector3 v;
		((PhysicalGameObj *)o)->Get_Position(&v);
		return o2->Point_In_Zone(v);
	}
	return false;
}

unsigned int SCRIPTS_API Get_Vehicle_Definition_Mode_By_ID(unsigned long ID)
{
	VehicleGameObjDef *d = (VehicleGameObjDef *)Find_Definition(ID);
	if (!d || d->Get_Class_ID() != 0x3010) return 5;
	return (int)d->Get_Type();
}

unsigned int SCRIPTS_API Get_Zone_Type(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	ScriptZoneGameObj *o = obj->As_ScriptZoneGameObj();
	if (!o)
	{
		return 0;
	}
	const ScriptZoneGameObjDef *d = &o->Get_Definition();
	return d->Get_Type();
}

void SCRIPTS_API Set_Zone_Box(GameObject *obj,OBBoxClass &box)
{
	if (!obj)
	{
		return;
	}
	ScriptZoneGameObj *o = obj->As_ScriptZoneGameObj();
	if (!o)
	{
		return;
	}
	o->Set_Bounding_Box(box);
}

GameObject SCRIPTS_API *Create_Zone(const char *preset,OBBoxClass &box)
{
	GameObject *obj = Create_Library_Object(preset);
	Set_Zone_Box(obj,box);
	obj->Start_Observers();
	return obj;
}

void SCRIPTS_API Set_Vehicle_Gunner(GameObject *obj,int seat)
{
	if (!obj || !obj->As_VehicleGameObj())
	{
		return;
	}
	VehicleGameObj *o = obj->As_VehicleGameObj();
	if (seat <= 1 || seat > o->Get_Definition().Get_Seat_Count())
	{
		return;
	}
	if (o->Get_Occupant(seat) && o->Get_Occupant(1))
	{
		SoldierGameObj *obj2 = o->Get_Occupant(1);
		o->Set_Occupant(1,o->Get_Occupant(seat));
		o->Set_Occupant(seat,obj2);
	}
}

SCRIPTS_API bool SmartGameObj::Is_Stealthed() const
{
	if (StealthEffect)
	{
		if (StealthEffect->Is_Stealthed())
		{
			return true;
		}
	}
	return false;
}

SCRIPTS_API bool SmartGameObj::Is_Human_Controlled()
{
	return (this->ControlOwner >= 0);
}

SoldierGameObj * VehicleGameObj::Get_Driver(void)
{
	if ( SeatOccupants.Length() > DRIVER_SEAT ) {
		return SeatOccupants[DRIVER_SEAT];
	}
	return NULL;
}

SoldierGameObj * VehicleGameObj::Get_Gunner(void)
{
	if ( SeatOccupants.Length() > GUNNER_SEAT ) {
		return SeatOccupants[GUNNER_SEAT];
	}
	return NULL;
}

SoldierGameObj * VehicleGameObj::Get_Actual_Gunner(void)
{
	SoldierGameObj * gunner = Get_Gunner();
	if ( gunner && !Get_Driver_Is_Gunner() ) {
		return gunner;
	}
	return Get_Driver();
}

SCRIPTS_API const VehicleGameObjDef & VehicleGameObj::Get_Definition( void ) const
{
	return (const VehicleGameObjDef &)BaseGameObj::Get_Definition();
}

// 0x006802B0, 0x0067FB50, 0x, 0x0810D93A
SCRIPTS_API SmartGameObj* GameObjManager::Find_SmartGameObj(int networkId)
{
	if (!networkId)
		return 0;
	for (SLNode<SmartGameObj>* node = GameObjManager::SmartGameObjList.Head(); node; node = node->Next())
	{
		SmartGameObj* object = (SmartGameObj *)node->Data();
		if (object->Get_Network_ID() != networkId)
			continue;
		if (object->Is_Delete_Pending())
			return 0;
		return object;
	}
	return 0;
}

SCRIPTS_API bool SmartGameObj::Has_Player()
{
	return ControlOwner != SERVER_CONTROL_OWNER;
}

bool SmartGameObj::Is_Stealth_Enabled()
{
	return StealthEnabled;
}

SCRIPTS_API bool SmartGameObj::Is_Obj_Visible(PhysicalGameObj* object)
{
	const Vector3 bullseyePosition = object->Get_Bullseye_Position();
	const Matrix3D lookTransform = Get_Look_Transform();
	
	const Vector3 lookPosition = lookTransform.Get_Translation();
	const Vector3 tranformedBullseyeOffset = lookTransform.Inverse_Rotate_Vector(bullseyePosition - lookPosition);
	
	if (tranformedBullseyeOffset.Length() >= Get_Definition().SightRange * GlobalSightRangeScale)
		return false;
	
	const Vector3 xytranformedBullseyeOffset = Vector3(tranformedBullseyeOffset.X, tranformedBullseyeOffset.Y, 0);
	const Vector3 xyBullseyeDirection = Normalize(xytranformedBullseyeOffset);
	const float angle = WWMath::Fast_Acos(xyBullseyeDirection.X);
	
	if (angle >= .5 * Get_Definition().SightArc)
		return false;
	
	CastResultStruct castResult;
	PhysRayCollisionTestClass collisionTest(LineSegClass(lookPosition, bullseyePosition), &castResult, BULLET_COLLISION_GROUP);
	
	Peek_Physical_Object()->Inc_Ignore_Counter();
	PhysicsSceneClass::Get_Instance()->Cast_Ray(collisionTest, false);
	Peek_Physical_Object()->Dec_Ignore_Counter();
	
	return
		castResult.Fraction == 1.f ||
		collisionTest.CollidedPhysObj == object->Peek_Physical_Object();
}

SCRIPTS_API bool SmartGameObj::Is_Splash_Possible(PhysicalGameObj* object)
{
	Vector3 bullseyePosition = object->Get_Position();
	const Matrix3D lookTransform = Get_Look_Transform();
	const Vector3 lookPosition = lookTransform.Get_Translation();
	

	
	CastResultStruct castResult;
	PhysRayCollisionTestClass collisionTest(LineSegClass(bullseyePosition,lookPosition), &castResult, BULLET_COLLISION_GROUP);
	
	object->Peek_Physical_Object()->Inc_Ignore_Counter();
	PhysicsSceneClass::Get_Instance()->Cast_Ray(collisionTest, false);
	object->Peek_Physical_Object()->Dec_Ignore_Counter();
	
	return
		castResult.Fraction == 1.f ||
		collisionTest.CollidedPhysObj == Peek_Physical_Object();
}

bool SCRIPTS_API ActionClass::Is_Acting( void )
{
	return ( ActionCode != NULL );
}

bool SCRIPTS_API Is_Spy(GameObject *obj)
{
	if (obj && obj->As_SoldierGameObj())
	{
		return obj->As_SoldierGameObj()->Get_Definition().Is_Spy();
	}
	return false;
}

bool SCRIPTS_API Is_Unsquishable(GameObject *obj)
{
	if (obj && obj->As_SoldierGameObj())
	{
		return obj->As_SoldierGameObj()->Get_Definition().Is_Unsquishable();
	}
	return true;
}

SCRIPTS_API bool Is_Stealth_Enabled(GameObject *obj)
{
	if (obj && obj->As_SoldierGameObj())
	{
		return obj->As_SoldierGameObj()->Is_Stealth_Enabled();
	}
	return false;
}

SCRIPTS_API int Get_Object_Count_In_Zone(GameObject *obj,int Team)
{
		if (!obj)
		{
				return 0;
		}
		ScriptZoneGameObj *Zone = obj->As_ScriptZoneGameObj();
		if (!Zone)
		{
				return 0;
		}
		int Return = 0;
		for (SLNode<SmartGameObj> *z = GameObjManager::SmartGameObjList.Head();z;z = z->Next())
		{
				if ((Get_Object_Type(z->Data()) == Team || Team == 2) && Zone->Inside_Me(z->Data()))
				{
						Return++;
				}
		}
		return Return;
}

SCRIPTS_API int Get_Player_Count_In_Zone(GameObject *obj,int Team)
{
		if (!obj)
		{
				return 0;
		}
		ScriptZoneGameObj *Zone = obj->As_ScriptZoneGameObj();
		if (!Zone)
		{
				return 0;
		}
		int Return = 0;
		for (SLNode<SoldierGameObj> *z = GameObjManager::StarGameObjList.Head();z;z = z->Next())
		{
				if ((Get_Object_Type(z->Data()) == Team || Team == 2) && Zone->Inside_Me(z->Data()))
				{
						Return++;
				}
		}
		return Return;
}

SCRIPTS_API GameObject* Create_Object_Attach_To_Object(GameObject* host, const char* preset, const char* bone = NULL)
{
  GameObject* obj = Commands->Create_Object_At_Bone(host, preset, (NULL != bone) ? bone : "origin");
  if ( NULL != obj )
  {
    Commands->Set_Facing(obj, Commands->Get_Facing(host));
    Commands->Attach_To_Object_Bone(obj, host, (NULL != bone) ? bone : "origin");
  }

  return obj;
}

int SCRIPTS_API Get_Occupant_Seat(GameObject *vehicle,GameObject *occupant)
{
	if (!vehicle || !vehicle->As_VehicleGameObj())
	{
		return -1;
	}
	if (!occupant || !occupant->As_SoldierGameObj())
	{
		return -1;
	}
	return vehicle->As_VehicleGameObj()->Find_Seat(occupant->As_SoldierGameObj());
}

int	VehicleGameObj::Find_Seat( SoldierGameObj * occupant )
{
	for ( int i = 0; i < SeatOccupants.Length(); i++ )
	{
		if ( SeatOccupants[i] == occupant )
		{
			return i;
		}
	}
	return -1;
}

REF_DEF2(float, SmartGameObj::GlobalSightRangeScale, 0x00811E64, 0x0081103C);
