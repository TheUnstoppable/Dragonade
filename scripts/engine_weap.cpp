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
#include "Iterator.h"
#include "BeaconGameObj.h"
#include "C4GameObj.h"
#include "WeaponBagClass.h"
#include "WeaponClass.h"
#include "WeaponMgr.h"
#include "slist.h"
#include "GameObjManager.h"
#include "SmartGameObj.h"
#include "PlayerDataClass.h"
#include "PowerupGameObjDef.h"
#include "engine_def.h"
#include "engine_obj.h"
#include "engine_player.h"
#include "SoldierGameObj.h"
#include "engine_weap.h"
#include "OffenseObjectClass.h"
#include <limits>

SoldierGameObj* BeaconGameObj::Get_Owner()
{
	if (!Owner)
		return NULL;
	
	PhysicalGameObj* physicalOwner = Owner->As_PhysicalGameObj();
	if (!physicalOwner)
		return NULL;
	
	return physicalOwner->As_SoldierGameObj();
}

// -------------------------------------------------------------------------------------------------

/*! Internal utility function to get the current weapon for an object, reducing code duplication */
WeaponClass* _Get_Current_Weapon ( GameObject* obj )
{
  if (!obj)
    return NULL;

  PhysicalGameObj* pPhysObjo2 = obj->As_PhysicalGameObj();
  if (!pPhysObjo2)
    return NULL;

  if ( ArmedGameObj* pArmedObj = pPhysObjo2->As_ArmedGameObj() )
  {
    WeaponBagClass* w = pArmedObj->Get_Weapon_Bag();
    if ((w->Get_Index()) && (w->Get_Index() < w->Get_Count()))
      return w->Peek_Weapon(w->Get_Index());
  }

  return NULL;
}

SCRIPTS_API int Get_Current_Bullets(GameObject *obj)
{
  if ( WeaponClass* wc = _Get_Current_Weapon(obj) )
    return wc->Get_Clip_Rounds();
  return 0;
}

SCRIPTS_API int Get_Current_Max_Bullets(GameObject *obj)
{
  if ( WeaponClass* wc = _Get_Current_Weapon(obj) )
    return wc->Get_Definition()->ClipSize;
  return 0;
}

SCRIPTS_API int Get_Current_Clip_Bullets(GameObject *obj)
{
  if ( WeaponClass* wc = _Get_Current_Weapon(obj) )
    return wc->Get_Inventory_Rounds();
  return 0;
}

SCRIPTS_API int Get_Current_Clip_Max_Bullets(GameObject *obj)
{
  if ( WeaponClass* wc = _Get_Current_Weapon(obj) )
    return wc->Get_Definition()->MaxInventoryRounds;
  return 0;
}

SCRIPTS_API int Get_Current_Total_Bullets(GameObject *obj)
{
  if ( WeaponClass* wc = _Get_Current_Weapon(obj) )
  {
    int bullets = wc->Get_Clip_Rounds();
    int clipbullets = wc->Get_Inventory_Rounds();

    if (clipbullets < 0)
      return -1;

    return bullets+clipbullets;
  }
  return 0;
}

SCRIPTS_API int Get_Current_Total_Max_Bullets(GameObject *obj)
{
  if ( WeaponClass* wc = _Get_Current_Weapon(obj) )
  {
    int bullets = wc->Get_Definition()->ClipSize;
    int clipbullets = wc->Get_Definition()->MaxInventoryRounds;

    if (clipbullets < 0)
      return -1;

    return bullets+clipbullets;
  }
  return 0;
}

// -------------------------------------------------------------------------------------------------

/*! Internal utility function to get the weapon in a specific position for an object, reducing code duplication */
WeaponClass* _Get_Position_Weapon ( GameObject* obj, int position )
{
  if (!obj)
    return NULL;

  PhysicalGameObj* pPhysObjo2 = obj->As_PhysicalGameObj();
  if (!pPhysObjo2)
    return NULL;

  if ( ArmedGameObj* pArmedObj = pPhysObjo2->As_ArmedGameObj() )
  {
    WeaponBagClass* w = pArmedObj->Get_Weapon_Bag();
    if ((position) && (position < w->Get_Count()))
      return w->Peek_Weapon(position);
  }

  return NULL;
}

SCRIPTS_API int Get_Position_Bullets(GameObject *obj,int position)
{
  if ( WeaponClass* wc = _Get_Position_Weapon(obj,position) )
    return wc->Get_Clip_Rounds();
  return 0;
}

SCRIPTS_API int Get_Position_Max_Bullets(GameObject *obj, int position)
{
  if ( WeaponClass* wc = _Get_Position_Weapon(obj,position) )
    return wc->Get_Definition()->ClipSize;
  return 0;
}

SCRIPTS_API int Get_Position_Clip_Bullets(GameObject *obj,int position)
{
  if ( WeaponClass* wc = _Get_Position_Weapon(obj,position) )
    return wc->Get_Inventory_Rounds();
  return 0;
}

SCRIPTS_API int Get_Position_Clip_Max_Bullets(GameObject *obj, int position)
{
  if ( WeaponClass* wc = _Get_Position_Weapon(obj,position) )
    return wc->Get_Definition()->MaxInventoryRounds;
  return 0;
}

SCRIPTS_API int Get_Position_Total_Bullets(GameObject *obj, int position)
{
  if ( WeaponClass* wc = _Get_Position_Weapon(obj,position) )
  {
    int bullets = wc->Get_Clip_Rounds();
    int clipbullets = wc->Get_Inventory_Rounds();

    if (clipbullets < 0)
      return -1;

    return bullets+clipbullets;
  }
  return 0;
}

SCRIPTS_API int Get_Position_Total_Max_Bullets(GameObject *obj,int position)
{
  if ( WeaponClass* wc = _Get_Position_Weapon(obj,position) )
  {
    int bullets = wc->Get_Definition()->ClipSize;
    int clipbullets = wc->Get_Definition()->MaxInventoryRounds;

    if (clipbullets < 0)
      return -1;

    return bullets+clipbullets;
  }
  return 0;
}

// -------------------------------------------------------------------------------------------------

/*! Internal utility function to get the named weapon preset for an object, reducing code duplication */
WeaponClass* _Get_Weapon ( GameObject* obj, const char* weapon )
{
  if (!obj)
    return NULL;

  PhysicalGameObj* pPhysObjo2 = obj->As_PhysicalGameObj();
  if (!pPhysObjo2)
    return NULL;

  if ( ArmedGameObj* pArmedObj = pPhysObjo2->As_ArmedGameObj() )
  {
    WeaponBagClass* w = pArmedObj->Get_Weapon_Bag();
    int x = w->Get_Count();
    for (int i = 0;i < x;i++)
    {
      if (w->Peek_Weapon(i))
      {
        if (!_stricmp(w->Peek_Weapon(i)->Get_Name(),weapon))
          return w->Peek_Weapon(i);
      }
    }
  }

  return NULL;
}

SCRIPTS_API int Get_Bullets(GameObject *obj,const char *weapon)
{
  if ( WeaponClass* wc = _Get_Weapon(obj,weapon) )
    return wc->Get_Clip_Rounds();
  return 0;
}

SCRIPTS_API int Get_Max_Bullets(GameObject *obj,const char *weapon)
{
  if ( WeaponClass* wc = _Get_Weapon(obj,weapon) )
    return wc->Get_Definition()->ClipSize;
  return 0;
}

SCRIPTS_API int Get_Clip_Bullets(GameObject *obj,const char *weapon)
{
  if ( WeaponClass* wc = _Get_Weapon(obj,weapon) )
    return wc->Get_Inventory_Rounds();
  return 0;
}

SCRIPTS_API int Get_Max_Clip_Bullets(GameObject *obj,const char *weapon)
{
  if ( WeaponClass* wc = _Get_Weapon(obj,weapon) )
    return wc->Get_Definition()->MaxInventoryRounds;
  return 0;
}

SCRIPTS_API int Get_Total_Bullets(GameObject *obj,const char *weapon)
{
  if ( WeaponClass* wc = _Get_Weapon(obj,weapon) )
  {
    int bullets = wc->Get_Clip_Rounds();
    int clipbullets = wc->Get_Inventory_Rounds();

    if (clipbullets < 0)
      return -1;

    return bullets+clipbullets;
  }
  return 0;
}

SCRIPTS_API int Get_Max_Total_Bullets(GameObject *obj,const char *weapon)
{
  if ( WeaponClass* wc = _Get_Weapon(obj,weapon) )
  {
    int bullets = wc->Get_Definition()->ClipSize;
    int clipbullets = wc->Get_Definition()->MaxInventoryRounds;

    if (clipbullets < 0)
      return -1;

    return bullets+clipbullets;
  }
  return 0;
}

// -------------------------------------------------------------------------------------------------

void C4GameObj::Restore_Owner()
{
	int at = AmmoDef->AmmoType;
	if (!Owner && Player && AmmoDef && (1 != at))
	{
		TT_FOREACH(object, GameObjManager::StarGameObjList)
		{
			if (object->Get_Player_Data() == Player)
			{
				Owner = object;
				break;
			}
		}

		/*
		if (!Owner)
		{
			Player = NULL;
			C4GameObj::Defuse(this);
		}
		*/
	}
}

SCRIPTS_API const char *Get_Powerup_Weapon(const char *Powerup)
{
  DefinitionClass *powerupdef = Find_Named_Definition(Powerup);
  PowerUpGameObjDef *c = (PowerUpGameObjDef *)powerupdef;
  if (c && c->Get_Grant_Weapon_ID())
  {
    if ( DefinitionClass *weapondef = Find_Definition(c->Get_Grant_Weapon_ID()) )
      return weapondef->Get_Name();
  }
  return "None";
}

SCRIPTS_API const AmmoDefinitionClass *Get_Weapon_Ammo_Definition(const char *weapon,bool PrimaryFire)
{
	WeaponDefinitionClass *d = (WeaponDefinitionClass *)Find_Named_Definition(weapon);
	if (d)
	{
		if (PrimaryFire)
		{
			unsigned long AmmoID = d->PrimaryAmmoDefID;
			AmmoDefinitionClass *AmmoDef = (AmmoDefinitionClass*)Find_Definition(AmmoID);
			return AmmoDef;
		}
		else
		{
			unsigned long AmmoID = d->SecondaryAmmoDefID;
			AmmoDefinitionClass *AmmoDef = (AmmoDefinitionClass*)Find_Definition(AmmoID);
			return AmmoDef;
		}
	}
	return 0;
}

SCRIPTS_API const WeaponDefinitionClass *Get_Current_Weapon_Definition(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	PhysicalGameObj *o2 = obj->As_PhysicalGameObj();
	if (!o2)
	{
		return 0;
	}
	ArmedGameObj *o3 = o2->As_ArmedGameObj();
	if (!o3)
	{
		return 0;
	}
	WeaponBagClass *w = o3->Get_Weapon_Bag();
	if ((w->Get_Index()) && (w->Get_Index() < w->Get_Count()))
	{
		return w->Peek_Weapon(w->Get_Index())->Get_Definition();
	}
	return 0;
}

SCRIPTS_API const WeaponDefinitionClass *Get_Weapon_Definition(const char *weapon)
{
	return (WeaponDefinitionClass *)Find_Named_Definition(weapon);
}

SCRIPTS_API const WeaponDefinitionClass *Get_Position_Weapon_Definition(GameObject *obj,int position)
{
	if (!obj)
	{
		return 0;
	}
	PhysicalGameObj *o2 = obj->As_PhysicalGameObj();
	if (!o2)
	{
		return 0;
	}
	ArmedGameObj *o3 = o2->As_ArmedGameObj();
	if (!o3)
	{
		return 0;
	}
	WeaponBagClass *w = o3->Get_Weapon_Bag();
	if ((position) && (position < w->Get_Count()))
	{
		return w->Peek_Weapon(position)->Get_Definition();
	}
	return 0;
}

SCRIPTS_API const ExplosionDefinitionClass *Get_Explosion(const char *explosion)
{
	return (ExplosionDefinitionClass *)Find_Named_Definition(explosion);
}

SCRIPTS_API const char *Get_Powerup_Weapon_By_Obj(GameObject *Powerup)
{
	if (!Powerup)
	{
		return "None";
	}
	const DefinitionClass *powerupdef = &Powerup->Get_Definition();
	PowerUpGameObjDef *c = (PowerUpGameObjDef *)powerupdef;
	if (!c)
	{
		return "None";
	}
	if (c->Get_Grant_Weapon_ID())
	{
		DefinitionClass *weapondef = Find_Definition(c->Get_Grant_Weapon_ID());
		return weapondef->Get_Name();
	}
	return "None";
}

SCRIPTS_API int Get_Current_Weapon_Style(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	PhysicalGameObj *o2 = obj->As_PhysicalGameObj();
	if (!o2)
	{
		return 0;
	}
	ArmedGameObj *o3 = o2->As_ArmedGameObj();
	if (!o3)
	{
		return 0;
	}
	WeaponBagClass *w = o3->Get_Weapon_Bag();
	if ((w->Get_Index()) && (w->Get_Index() < w->Get_Count()))
	{
		WeaponClass *wc = w->Peek_Weapon(w->Get_Index());
		return wc->Get_Definition()->Style;
	}
	return 0;
}

SCRIPTS_API int Get_Position_Weapon_Style(GameObject *obj, int position)
{
	if (!obj)
	{
		return 0;
	}
	PhysicalGameObj *o2 = obj->As_PhysicalGameObj();
	if (!o2)
	{
		return 0;
	}
	ArmedGameObj *o3 = o2->As_ArmedGameObj();
	if (!o3)
	{
		return 0;
	}
	WeaponBagClass *w = o3->Get_Weapon_Bag();
	if ((position) && (position < w->Get_Count()))
	{
		WeaponClass *wc = w->Peek_Weapon(position);
		return wc->Get_Definition()->Style;
	}
	return 0;
}

SCRIPTS_API int Get_Weapon_Style(GameObject *obj,const char *weapon)
{
	if (!obj)
	{
		return 0;
	}
	PhysicalGameObj *o2 = obj->As_PhysicalGameObj();
	if (!o2)
	{
		return 0;
	}
	ArmedGameObj *o3 = o2->As_ArmedGameObj();
	if (!o3)
	{
		return 0;
	}
	WeaponBagClass *w = o3->Get_Weapon_Bag();
	int x = w->Get_Count();
	for (int i = 0;i < x;i++)
	{
		if (w->Peek_Weapon(i))
		{
			if (!_stricmp(w->Peek_Weapon(i)->Get_Name(),weapon))
			{
				WeaponClass *wc = w->Peek_Weapon(i);
				return wc->Get_Definition()->Style;
			}
		}
	}
	return 0;
}

SCRIPTS_API void Disarm_Beacon(GameObject *obj)
{
	if (!obj)
	{
		return;
	}
	PhysicalGameObj *o2 = obj->As_PhysicalGameObj();
	if (!o2)
	{
		return;
	}
	BeaconGameObj *o3 = o2->As_BeaconGameObj();
	if (!o3)
	{
		return;
	}
	OffenseObjectClass offense(0);
	o3->Completely_Damaged(offense);
}

SCRIPTS_API GameObject *Get_Beacon_Planter(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	PhysicalGameObj *o2 = obj->As_PhysicalGameObj();
	if (!o2)
	{
		return 0;
	}
	BeaconGameObj *o3 = o2->As_BeaconGameObj();
	if (!o3)
	{
		return 0;
	}
	return o3->Get_Owner();
}

SCRIPTS_API void Disarm_Beacons(int PlayerID)
{
	SLNode<BeaconGameObj> *x = GameObjManager::BeaconGameObjList.Head();
	while (x)
	{
		GameObject *o = (GameObject *)x->Data();
		if (o)
		{
			if (Get_Player_ID(Get_Beacon_Planter(o)) == PlayerID)
			{
				Disarm_Beacon(o);
			}
		}
		x = x->Next();
	}
}


SCRIPTS_API BeaconGameObj* Find_Nearest_Beacon(const Vector3& position, int team)
{
	BeaconGameObj* beacon = nullptr;
	float min_distance = std::numeric_limits<float>::infinity();

	for (auto object_node = GameObjManager::BeaconGameObjList.Head(); object_node; object_node = object_node->Next())
	{
		BeaconGameObj* object = object_node->Data();
		if (team == 2 || team == object->Get_Player_Type())
		{
			Vector3 beacon_position;
			object->Get_Position(&beacon_position);

			float distance = (beacon_position - position).Length2();

			if (distance < min_distance)
			{
				beacon = object;
			}
		}
	}

	return beacon;
}

SCRIPTS_API void Disarm_C4(GameObject *obj)
{
	if (!obj)
	{
		return;
	}
	PhysicalGameObj *o2 = obj->As_PhysicalGameObj();
	if (!o2)
	{
		return;
	}
	C4GameObj *o3 = o2->As_C4GameObj();
	if (!o3)
	{
		return;
	}
	o3->Defuse();
}

SCRIPTS_API int Get_C4_Count(int team, int type /* = 0*/)
{
	int total = 0;

	for (auto c4_node = GameObjManager::C4GameObjList.Head(); c4_node; c4_node = c4_node->Next())
	{
		C4GameObj* c4 = c4_node->Data();
		const AmmoDefinitionClass* ammo = c4->Get_Ammo_Def();
		if ((type == 0 || type == ammo->AmmoType) && (team == 2 || team == c4->Get_Player_Type()))
		{
			++total;
		}
	}

	return total;
}

SCRIPTS_API void Defuse_Owned_C4(int player_id, int type /* = 0*/)
{
	for (auto c4_node = GameObjManager::C4GameObjList.Head(); c4_node; c4_node = c4_node->Next())
	{
		C4GameObj* c4 = c4_node->Data();

		const AmmoDefinitionClass* ammo = c4->Get_Ammo_Def();
		if (type == 0 || type == ammo->AmmoType)
		{
			c4->Restore_Owner();
			if (Get_Player_ID(c4->Get_Owner()) == player_id)
			{
				c4->Defuse();
			}
		}
	}
}

SCRIPTS_API void Defuse_Attached_C4(GameObject* attached_object)
{
	for (auto c4_node = GameObjManager::C4GameObjList.Head(); c4_node; c4_node = c4_node->Next())
	{
		C4GameObj* c4 = c4_node->Data();
		if (c4->Get_Stuck_Object() == attached_object)
		{
			c4->Defuse();
		}
	}
}




SCRIPTS_API void Disarm_All_C4_Beacons(int PlayerID)
{
	SLNode<BeaconGameObj> *x = GameObjManager::BeaconGameObjList.Head();
	while (x)
	{
		GameObject *o = (GameObject *)x->Data();
		if (o)
		{
			if (Get_Player_ID(Get_Beacon_Planter(o)) == PlayerID)
			{
				Disarm_Beacon(o);
			}
		}
		x = x->Next();
	}
	SLNode<C4GameObj> *x2 = GameObjManager::C4GameObjList.Head();
	while (x2)
	{
		GameObject *o = (GameObject *)x2->Data();
		if (o)
		{
			if (Get_Player_ID(Get_C4_Planter(o)) == PlayerID)
			{
				Disarm_C4(o);
			}
		}
		x2 = x2->Next();
	}
}

SCRIPTS_API const char *Get_Current_Weapon(GameObject *obj)
{
	if (!obj)
	{
		return "None";
	}
	PhysicalGameObj *o2 = obj->As_PhysicalGameObj();
	if (!o2)
	{
		return "None";
	}
	ArmedGameObj *o3 = o2->As_ArmedGameObj();
	if (!o3)
	{
		return "None";
	}
	WeaponBagClass *w = o3->Get_Weapon_Bag();
	if ((w->Get_Index()) && (w->Get_Index() < w->Get_Count()))
	{
		return w->Peek_Weapon(w->Get_Index())->Get_Name();
	}
	return "None";
}

SCRIPTS_API int Get_Weapon_Count(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	PhysicalGameObj *o2 = obj->As_PhysicalGameObj();
	if (!o2)
	{
		return 0;
	}
	ArmedGameObj *o3 = o2->As_ArmedGameObj();
	if (!o3)
	{
		return 0;
	}
	WeaponBagClass *w = o3->Get_Weapon_Bag();
	return w->Get_Count();
}

SCRIPTS_API const char *Get_Weapon(GameObject *obj,int position)
{
	if (!obj)
	{
		return "None";
	}
	PhysicalGameObj *o2 = obj->As_PhysicalGameObj();
	if (!o2)
	{
		return "None";
	}
	ArmedGameObj *o3 = o2->As_ArmedGameObj();
	if (!o3)
	{
		return "None";
	}
	WeaponBagClass *w = o3->Get_Weapon_Bag();
	if ((position) && (position < w->Get_Count()))
	{
		return (w->Peek_Weapon(position)->Get_Name());
	}
	return "None";
}

SCRIPTS_API bool Has_Weapon(GameObject *obj,const char *weapon)
{
	if (!obj)
	{
		return 0;
	}
	PhysicalGameObj *o2 = obj->As_PhysicalGameObj();
	if (!o2)
	{
		return 0;
	}
	ArmedGameObj *o3 = o2->As_ArmedGameObj();
	if (!o3)
	{
		return 0;
	}
	WeaponBagClass *w = o3->Get_Weapon_Bag();
	int x = w->Get_Count();
	for (int i = 0;i < x;i++)
	{
		if (w->Peek_Weapon(i))
		{
			if (!_stricmp(w->Peek_Weapon(i)->Get_Name(),weapon))
			{
				return true;
			}
		}
	}
	return false;
}

SCRIPTS_API GameObject *Find_Beacon(int Number,int Team)
{
	if (Number < 0)
	{
		Number = 0;
	}
	if (!Number)
	{
		return 0;
	}
	int Total = 0;
	GameObject *ReturnObj = 0;
	SLNode<BeaconGameObj> *x = GameObjManager::BeaconGameObjList.Head();
	while (x)
	{
		GameObject *o = (GameObject *)x->Data();
		if (o)
		{
			if (Team == 2)
			{
				Total++;
			}
			else if (Get_Object_Type(o) == Team)
			{
				Total++;
			}
			if (Total == Number) ReturnObj = o;
		}
		x = x->Next();
	}
	return ReturnObj;
}

SCRIPTS_API int Get_Beacon_Count(int Team)
{
	int Total = 0;
	SLNode<BeaconGameObj> *x = GameObjManager::BeaconGameObjList.Head();
	while (x)
	{
		GameObject *o = (GameObject *)x->Data();
		if (o)
		{
			if (Team == 2)
			{
				Total++;
			}
			else if (Get_Object_Type(o) == Team)
			{
				Total++;
			}
		}
		x = x->Next();
	}
	return Total;
}

SCRIPTS_API int Get_C4_Mode(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	PhysicalGameObj *o2 = obj->As_PhysicalGameObj();
	if (!o2)
	{
		return 0;
	}
	C4GameObj *o3 = o2->As_C4GameObj();
	if (!o3)
	{
		return 0;
	}
	return o3->Get_Ammo_Def()->AmmoType;
}

SCRIPTS_API GameObject *Get_C4_Planter(GameObject *obj)
{	
	if (!obj)
	{
		return 0;
	}
	PhysicalGameObj *o2 = obj->As_PhysicalGameObj();
	if (!o2)
	{
		return 0;
	}
	C4GameObj *o3 = o2->As_C4GameObj();
	if (!o3)
	{
		return 0;
	}
	o3->Restore_Owner();
	return (GameObject *)o3->Get_Owner();
}

SCRIPTS_API GameObject *Get_C4_Attached(GameObject *obj)
{	
	if (!obj)
	{
		return 0;
	}
	PhysicalGameObj *o2 = obj->As_PhysicalGameObj();
	if (!o2)
	{
		return 0;
	}
	C4GameObj *o3 = o2->As_C4GameObj();
	if (!o3)
	{
		return 0;
	}

	return (GameObject *)o3->Get_Stuck_Object();
}

SCRIPTS_API WeaponClass* ArmedGameObj::Get_Weapon()
{
	return WeaponBag->Get_Weapon();
}

SCRIPTS_API int	WeaponClass::Get_Total_Rounds()
{
	if (InventoryRounds < 0)
	{
		return -1;
	}
	return ClipRounds + InventoryRounds;
}

SCRIPTS_API float WeaponClass::Get_Range( void )
{
	return ( PrimaryAmmoDefinition != NULL ) ? PrimaryAmmoDefinition->Range : 0;
}

SCRIPTS_API bool WeaponClass::Is_Ammo_Maxed( void )
{
	if (InventoryRounds == -1) return true;
	if (((int)Definition->MaxInventoryRounds == 0) && (ClipRounds == 0)) return false;
	return (InventoryRounds == (int)Definition->MaxInventoryRounds);
}

SCRIPTS_API float WeaponClass::Get_Targeting_Range(void)
{
	if (PrimaryAmmoDefinition != NULL)
	{
		float range = PrimaryAmmoDefinition->TargetingRange;
		return range;
	}
	return 0;
}
