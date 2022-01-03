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

#include "PowerUpGameObjDef.h"
#include "PowerUpGameObj.h"
#include "scripts.h"
#include "BuildingGameObj.h"
#include "GameObjManager.h"
#include "wwmath.h"
#include "VehicleGameObj.h"
#include "SoldierGameObj.h"
#include "engine_def.h"
#include "RefineryGameObj.h"
#include "ScriptZoneGameObj.h"
#include "MoveablePhysClass.h"
#include "SmartGameObjDef.h"
#include "VehicleFactoryGameObj.h"
#include "VehicleFactoryGameObjDef.h"
#include "TransitionGameObjDef.h"
#include "AirFactoryGameObjDef.h"
#include "AirFactoryGameObj.h"
#include "NavalFactoryGameObjDef.h"
#include "NavalFactoryGameObj.h"
#include "AirStripGameObjDef.h"
#include "AirStripGameObj.h"
#include "BeaconGameObjDef.h"
#include "BeaconGameObj.h"
#include "C4GameObjDef.h"
#include "C4GameObj.h"
#include "CinematicGameObjDef.h"
#include "CinematicGameObj.h"
#include "ComCenterGameObjDef.h"
#include "ComCenterGameObj.h"
#include "ConstructionYardGameObjDef.h"
#include "ConstructionYardGameObj.h"
#include "DamageZoneGameObjDef.h"
#include "DamageZoneGameObj.h"
#include "PowerPlantGameObjDef.h"
#include "PowerPlantGameObj.h"
#include "RepairBayGameObjDef.h"
#include "RepairBayGameObj.h"
#include "SamSiteGameObjDef.h"
#include "SamSiteGameObj.h"
#include "SoldierFactoryGameObjDef.h"
#include "SoldierFactoryGameObj.h"
#include "SuperweaponGameObjDef.h"
#include "SuperweaponGameObj.h"
#include "SpecialEffectsGameObjDef.h"
#include "SpecialEffectsGameObj.h"
#include "TransitionGameObjDef.h"
#include "TransitionGameObj.h"
#include "WarFactoryGameObjDef.h"
#include "WarFactoryGameObj.h"
#include "Iterator.h"
#include "BuildingAggregateClass.h"
#include "BuildingAggregateDefClass.h"

SCRIPTS_API bool BuildingAggregateClass::Is_MCT(void) 
{
	return Get_BuildingAggregateDef()->IsMCT;
}

SCRIPTS_API const AirFactoryGameObjDef &
AirFactoryGameObj::Get_Definition (void) const
{
	return (const AirFactoryGameObjDef &)BaseGameObj::Get_Definition ();
}
SCRIPTS_API  const NavalFactoryGameObjDef &
NavalFactoryGameObj::Get_Definition (void) const
{
	return (const NavalFactoryGameObjDef &)BaseGameObj::Get_Definition ();
}
SCRIPTS_API  const AirStripGameObjDef & AirStripGameObj::Get_Definition( void ) const
{
	return (const AirStripGameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API  const ArmedGameObjDef & ArmedGameObj::Get_Definition( void ) const
{
	return (const ArmedGameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API const BeaconGameObjDef & BeaconGameObj::Get_Definition( void ) const
{
	return (const BeaconGameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API const C4GameObjDef & C4GameObj::Get_Definition( void ) const
{
	return (const C4GameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API const CinematicGameObjDef & CinematicGameObj::Get_Definition( void ) const
{
	return (const CinematicGameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API const ComCenterGameObjDef &
ComCenterGameObj::Get_Definition (void) const
{
	return (const ComCenterGameObjDef &)BaseGameObj::Get_Definition ();
}
SCRIPTS_API const ConstructionYardGameObjDef &
ConstructionYardGameObj::Get_Definition (void) const
{
	return (const ConstructionYardGameObjDef &)BaseGameObj::Get_Definition ();
}
SCRIPTS_API const DamageZoneGameObjDef & DamageZoneGameObj::Get_Definition( void ) const
{
	return (const DamageZoneGameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API const PowerPlantGameObjDef &
PowerPlantGameObj::Get_Definition (void) const
{
	return (const PowerPlantGameObjDef &)BaseGameObj::Get_Definition ();
}
SCRIPTS_API const RepairBayGameObjDef &
RepairBayGameObj::Get_Definition (void) const
{
	return (const RepairBayGameObjDef &)BaseGameObj::Get_Definition ();
}
SCRIPTS_API const SAMSiteGameObjDef & SAMSiteGameObj::Get_Definition( void ) const
{
	return (const SAMSiteGameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API const SoldierGameObjDef & SoldierGameObj::Get_Definition( void ) const
{
	return (const SoldierGameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API const SoldierFactoryGameObjDef &
SoldierFactoryGameObj::Get_Definition (void) const
{
	return (const SoldierFactoryGameObjDef &)BaseGameObj::Get_Definition ();
}
SCRIPTS_API const SuperweaponGameObjDef &
SuperweaponGameObj::Get_Definition (void) const
{
	return (const SuperweaponGameObjDef &)BaseGameObj::Get_Definition ();
}
SCRIPTS_API const SpecialEffectsGameObjDef & SpecialEffectsGameObj::Get_Definition( void ) const
{
	return (const SpecialEffectsGameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API const TransitionGameObjDef & TransitionGameObj::Get_Definition( void ) const
{
	return (const TransitionGameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API const WarFactoryGameObjDef &
WarFactoryGameObj::Get_Definition (void) const
{
	return (const WarFactoryGameObjDef &)BaseGameObj::Get_Definition ();
}

SCRIPTS_API const VehicleFactoryGameObjDef &
VehicleFactoryGameObj::Get_Definition (void) const
{
	return (const VehicleFactoryGameObjDef &)BaseGameObj::Get_Definition ();
}

SCRIPTS_API const ScriptZoneGameObjDef & ScriptZoneGameObj::Get_Definition( void ) const
{
	return (const ScriptZoneGameObjDef &)BaseGameObj::Get_Definition();
}

unsigned char NetworkObjectClass::Get_Object_Dirty_Bits(int clientId)
{
	return ClientStatus[clientId];
}

bool NetworkObjectClass::Get_Object_Dirty_Bit(int clientId, DIRTY_BIT level)
{
	return (ClientStatus[clientId] & level) == level;
}

void DefenseObjectClass::Set_Health_Max(float health)
{
	HealthMax = WWMath::Clamp(health, 0, MAX_MAX_HEALTH_NEW);
	if (HealthMax < Health)
	{
		Health = HealthMax;
	}
	Mark_Owner_Dirty();
}

void	DefenseObjectClass::Set_Shield_Type( ArmorType type )	
{ 
	ShieldType = type; 
	Mark_Owner_Dirty();
}

float DefenseObjectClass::Get_Health() const
{
	return this->Health;
}



float DefenseObjectClass::Get_Health_Max() const
{
	return this->HealthMax;
}


float DefenseObjectClass::Get_Shield_Strength() const
{
	return this->ShieldStrength;
}



float DefenseObjectClass::Get_Shield_Strength_Max() const
{
	return this->ShieldStrengthMax;
}

void DefenseObjectClass::Set_Shield_Strength_Max(float str)
{
	ShieldStrengthMax = WWMath::Clamp(str, 0, MAX_MAX_SHIELD_STRENGTH_NEW);
	if (ShieldStrengthMax < ShieldStrength)
	{
		ShieldStrength = ShieldStrengthMax;
	}
	Mark_Owner_Dirty();
}



void DefenseObjectClass::Mark_Owner_Dirty()
{
	if (Get_Owner())
	{
		Get_Owner()->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_OCCASIONAL, true);
	}
}

SCRIPTS_API REF_DEF3(SList<BuildingGameObj>, GameObjManager::BuildingGameObjList, 0x00856FE8, 0x008561D0, 0x0085BED0);
SCRIPTS_API REF_DEF3(SList<BaseGameObj>, GameObjManager::GameObjList, 0x00856FF8, 0x008561E0, 0x0085BEE0);
SCRIPTS_API REF_DEF3(SList<SoldierGameObj>, GameObjManager::StarGameObjList, 0x00856FC8, 0x008561B0, 0x0085BEB0);
SCRIPTS_API REF_DEF3(SList<SmartGameObj>, GameObjManager::SmartGameObjList, 0x00856FD8, 0x008561C0, 0x0085BEC0);

SCRIPTS_API const DamageableGameObjDef & DamageableGameObj::Get_Definition( void ) const 
{
	return (const DamageableGameObjDef &)BaseGameObj::Get_Definition();
}

SCRIPTS_API void ScriptableGameObj::Insert_Observer( GameObjObserverClass * observer )
{
	observer->Attach( this );
	Observers.Add( observer );
}

SCRIPTS_API void ScriptableGameObj::Remove_Observer( GameObjObserverClass * observer )
{
	Observers.Delete( observer );
	observer->Detach( this );
}

SCRIPTS_API GameObjObserverClass *ScriptableGameObj::Find_Observer(const char *Name)
{
	for (int i = 0;i < Observers.Count();i++)
	{
		if (!_stricmp(Observers[i]->Get_Name(),Name)) 
		{
			return Observers[i];
		}
	}
	return 0;
}

SCRIPTS_API const ScriptableGameObjDef & ScriptableGameObj::Get_Definition( void ) const
{
	return (const ScriptableGameObjDef &)BaseGameObj::Get_Definition();
}

SCRIPTS_API const BuildingGameObjDef & BuildingGameObj::Get_Definition( void ) const
{
	return (const BuildingGameObjDef &)BaseGameObj::Get_Definition();
}

SCRIPTS_API const RefineryGameObjDef & RefineryGameObj::Get_Definition( void ) const
{
	return (const RefineryGameObjDef &)BaseGameObj::Get_Definition();
}

int SCRIPTS_API Get_Object_Type(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	DamageableGameObj *o2 = obj->As_DamageableGameObj();
	if (!o2)
	{
		return 0;
	}
	return o2->Get_Player_Type();
}

void SCRIPTS_API Set_Object_Type(GameObject *obj,int type)
{
	if (!obj)
	{
		return;
	}
	DamageableGameObj *o2 = obj->As_DamageableGameObj();
	if (!o2)
	{
		return;
	}
	o2->Set_Player_Type(type);
}

GameObject SCRIPTS_API *Find_Smart_Object_By_Team(int Team)
{
	SLNode<SmartGameObj> *x = GameObjManager::SmartGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			if (Get_Object_Type(o) == Team)
			{
				return o;
			}
		}
		x = x->Next();
	}
	return 0;
}

GameObject SCRIPTS_API *Find_Object_By_Team(int Team)
{
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if (Get_Object_Type(o2) == Team)
			{
				return o2;
			}
		}
		x = x->Next();
	}
	return 0;
}

GameObject SCRIPTS_API *Find_Non_Player_Object_By_Team(int Team)
{
	SLNode<SmartGameObj> *x = GameObjManager::SmartGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			if ((Get_Object_Type(o) == Team) && (!Commands->Is_A_Star(o)))
			{
				return o;
			}
		}
		x = x->Next();
	}
	return 0;
}

GameObject SCRIPTS_API *Find_Object_By_Preset(int Team,const char *Preset_Name)
{
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			const DefinitionClass *d = &o2->Get_Definition();
			if (!_stricmp(d->Get_Name(),Preset_Name))
			{
				if ((Get_Object_Type(o2) == Team) || (Team == 2))
				{
					return o2;
				}
			}
		}
		x = x->Next();
	}
	return 0;
}

GameObject SCRIPTS_API *Find_Closest_Non_Building_Object_By_Team(int Team,Vector3 position)
{
	float closestdist_sq = FLT_MAX;
	GameObject *closest = 0;
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if ((Get_Object_Type(o2) == Team) && !o2->As_BuildingGameObj())
			{
				Vector3 pos = Commands->Get_Position(o2);
				float dist_sq = Vector3::Distance_Squared(pos, position);
				if (dist_sq < closestdist_sq)
				{
					closestdist_sq = dist_sq;
					closest = o2;
				}
			}
		}
		x = x->Next();
	}
	return closest;
}

GameObject SCRIPTS_API *Find_Closest_Preset_By_Team(int Team,const Vector3 &pos,const char *Preset)
{
	float closestdist_sq = FLT_MAX;
	GameObject *closest = 0;
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if ((Get_Object_Type(o2) == Team || Team == 2) && !_stricmp(Commands->Get_Preset_Name(o2),Preset))
			{
				float dist_sq = Vector3::Distance_Squared(Commands->Get_Position(o2), pos);
				if (dist_sq < closestdist_sq)
				{
					closestdist_sq = dist_sq;
					closest = o2;
				}
			}
		}
		x = x->Next();
	}
	return closest;
}

GameObject SCRIPTS_API *Find_Random_Preset_By_Team(int Team,const char *Preset)
{
	SimpleDynVecClass<GameObject*> ObjList(0);
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if ((Get_Object_Type(o2) == Team || Team == 2) && !_stricmp(Commands->Get_Preset_Name(o2),Preset))
			{
				ObjList.Add(o2);
			}
		}
		x = x->Next();
	}
	int len = ObjList.Count();
	if (len > 1)
	{
		return ObjList[Commands->Get_Random_Int(0,len)];
	}
	else if (!len)
	{
		return 0;
	}
	return ObjList[0];
}

int SCRIPTS_API Get_Object_Count(int Team,const char *Preset)
{
	int count = 0;
	SLNode<SmartGameObj> *x = GameObjManager::SmartGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			if ((Get_Object_Type(o) == Team || Team == 2) && !_stricmp(Commands->Get_Preset_Name(o),Preset))
			{
				count++;
			}
		}
		x = x->Next();
	}
	return count;
}

SimpleDynVecClass<GameObject*> SCRIPTS_API *Get_All_Objects_By_Preset(int Team,const char *Preset)
{
	SimpleDynVecClass<GameObject*> *ObjList = new SimpleDynVecClass<GameObject*>;
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if ((Get_Object_Type(o2) == Team || Team == 2) && !_stricmp(Commands->Get_Preset_Name(o2),Preset))
			{
				ObjList->Add(o2);
			}
		}
		x = x->Next();
	}
	return ObjList;
}

GameObject SCRIPTS_API *Find_Closest_Building(const Vector3& position)
{
	GameObject* bestBuilding = 0;
	float bestDistance = FLT_MAX;
	TT_FOREACH(building, GameObjManager::BuildingGameObjList)
	{
		Vector3 offset = Commands->Get_Position(building) - position;
		float distance = offset.Length2();
		if (distance < bestDistance)
		{
			bestBuilding = building;
			bestDistance = distance;
		}
	}
	return bestBuilding;
}

GameObject SCRIPTS_API *Find_Nearest_Preset(Vector3 position, const char *preset)
{
	GameObject *object = 0;
	float min_dist_sq = FLT_MAX;
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if (!_stricmp(Commands->Get_Preset_Name(o2),preset))
			{
				Vector3 pos;
				o2->Get_Position(&pos);
				float dist_sq = Vector3::Distance_Squared(position, pos);
				if (dist_sq < min_dist_sq)
				{
					object = o2;
					min_dist_sq = dist_sq;
				}
			}
		}
		x = x->Next();
	}
	return object;
}

GameObject SCRIPTS_API *Find_Random_Preset(const char* preset, int min, int max)
{
	int count = Commands->Get_Random_Int(min, max);
	GameObject *object = 0;
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (count > 0)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if (!_stricmp(Commands->Get_Preset_Name(o2),preset))
			{
				object = o2;
				count--;
			}
		}
		x = x->Next();
		if (!x)
		{
			if (object)
			{
				x = GameObjManager::GameObjList.Head();
			}
			else
			{
				return object;
			}
		}
	}
	return object;
}

void SCRIPTS_API Send_Custom_To_Team_Buildings(int Team,GameObject *sender,int type,int param,float delay)
{
	if (!sender)
	{
		return;
	}
	SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			if (Get_Object_Type(o) == Team)
			{
				int ReceiverID = Commands->Get_ID(o);
				int SenderID = Commands->Get_ID(sender);
				if (SenderID != ReceiverID)
				{
					Commands->Send_Custom_Event(sender,o,type,param,delay);
				}
			}
		}
		x = x->Next();
	}
}

void SCRIPTS_API Send_Custom_To_Team_Preset(int Team,const char *PresetName,GameObject *sender,int type,int param,float delay)
{
	if (!sender)
	{
		return;
	}
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if (Get_Object_Type(o2) == Team)
			{
				if (!_stricmp(Commands->Get_Preset_Name(o2),PresetName))
				{
					Commands->Send_Custom_Event(sender,o2,type,param,delay);
				}
			}
		}
		x = x->Next();
	}
}

void SCRIPTS_API Send_Custom_All_Objects_Area(int type,const Vector3 &Position,float Distance,GameObject *sender,int team)
{
	if (!sender)
	{
		return;
	}
	Vector3 TestPosition = Position;
	TestPosition.Z = 0;
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x) 
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			Vector3 ObjPosition = Commands->Get_Position(o2);
			ObjPosition.Z = 0;
			if (Vector3::Distance_Squared(ObjPosition, TestPosition) <= Distance * Distance)
			{
				if ((Get_Object_Type(o2) == team) || (team == 2))
				{
					Commands->Send_Custom_Event(sender,o2,type,0,0);
				}
			}
		}
		x = x->Next();
	}
}

void SCRIPTS_API Send_Custom_All_Objects(int type,GameObject *sender,int team)
{
	if (!sender)
	{
		return;
	}
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x) 
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if ((Get_Object_Type(o2) == team) || (team == 2))
			{
				Commands->Send_Custom_Event(sender,o2,type,0,0);
			}
		}
		x = x->Next();
	}
}

void SCRIPTS_API Send_Custom_Event_To_Object(GameObject *sender,const char *Preset,int type,int param,float delay)
{
	if (!sender)
	{
		return;
	}
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			const DefinitionClass *d = &o2->Get_Definition();
			if (!_stricmp(d->Get_Name(),Preset))
			{
				Commands->Send_Custom_Event(sender,o2,type,param,delay);
			}
		}
		x = x->Next();
	}
}

bool SCRIPTS_API Is_Unit_In_Range(const char *preset,float range,Vector3 location,int team,bool empty)
{
	SLNode<SmartGameObj> *x = GameObjManager::SmartGameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if (Get_Object_Type(o2) == team && !_stricmp(Commands->Get_Preset_Name(o2),preset))
			{
				Vector3 pos = Commands->Get_Position(o2);
				pos.Z = 0;
				location.Z = 0;
				if (Vector3::Distance_Squared(pos, location) <= range * range)
				{
					if (o2->As_VehicleGameObj() && !empty)
					{
						if (o2->As_VehicleGameObj()->Get_Occupant_Count() == 0)
						{
							return false;
						}
						else
						{
							return true;
						}
					}
					else
					{
						return true;
					}
				}
			}
		}
		x = x->Next();
	}
	return false;
}

bool SCRIPTS_API Get_Is_Powerup_Persistant(GameObject *obj)
{
	if (obj->As_PhysicalGameObj())
	{
		PowerUpGameObj *o = obj->As_PhysicalGameObj()->As_PowerUpGameObj();
		if (!o)
		{
			return 0;
		}
		return o->Get_Definition().Is_Persistant();
	}
	return 0;
}

bool SCRIPTS_API Get_Powerup_Always_Allow_Grant(GameObject *obj)
{
	if (obj->As_PhysicalGameObj())
	{
		PowerUpGameObj *o = obj->As_PhysicalGameObj()->As_PowerUpGameObj();
		if (!o)
		{
			return 0;
		}
		return o->Get_Definition().Is_Always_Allow_Grant();
	}
	return 0;
}

int SCRIPTS_API Get_Powerup_Grant_Sound(GameObject *obj)
{
	if (obj->As_PhysicalGameObj())
	{
		PowerUpGameObj *o = obj->As_PhysicalGameObj()->As_PowerUpGameObj();
		if (!o)
		{
			return 0;
		}
		return o->Get_Definition().Get_Grant_Sound();
	}
	return 0;
}


Vector3 SCRIPTS_API Get_Velocity( GameObject* obj )
{
	Vector3 velocity ( 0.0f, 0.0f, 0.0f );

	if ( obj && obj->As_PhysicalGameObj() )
	{
		MoveablePhysClass* moveable = obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass();
		if (moveable)
			moveable->Get_Velocity(&velocity);
	}

	return velocity;
}


void SCRIPTS_API Set_Velocity( GameObject* obj, Vector3 velocity )
{
	if ( obj && obj->As_PhysicalGameObj() )
	{
		MoveablePhysClass* moveable = obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass();
		if (moveable)
			moveable->Set_Velocity(velocity);
	}
}


Matrix3D SCRIPTS_API Get_Transform( GameObject* obj )
{
	Matrix3D transform(true);

	if ( obj && obj->As_PhysicalGameObj() )
	{
		MoveablePhysClass* moveable = obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass();
		if (moveable)
			transform = moveable->Get_Transform();
	}

	return transform;
}


void SCRIPTS_API Set_Transform( GameObject* obj, Matrix3D transform )
{
	if ( obj && obj->As_PhysicalGameObj() )
	{
		MoveablePhysClass* moveable = obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass();
		if (moveable)
			moveable->Set_Transform(transform);
	}
}


GameObject SCRIPTS_API *Get_Closest_Armed_Object_To_Object( GameObject* obj, int team )
{
	float closestdist_sq = FLT_MAX;
	GameObject *closest = 0;
	Vector3 position = Commands->Get_Position(obj);
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o && o != obj)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if ((team == 2 || Get_Object_Type(o2) == team) && o2->As_PhysicalGameObj() && o2->As_PhysicalGameObj()->As_ArmedGameObj())
			{
				Vector3 pos = Commands->Get_Position(o2);
				float dist_sq = Vector3::Distance_Squared(pos, position);
				if (dist_sq < closestdist_sq)
				{
					closestdist_sq = dist_sq;
					closest = o2;
				}
			}
		}
		x = x->Next();
	}
	return closest;
}

void PhysicalGameObj::Set_Transform(const Matrix3D& mat)
{
	PhysObj->Set_Transform(mat);
}

const Matrix3D &PhysicalGameObj::Get_Transform() const
{
	return PhysObj->Get_Transform();
}

void PhysicalGameObj::Set_Position(const Vector3& position)
{
	PhysObj->Set_Position(position);
}

float PhysicalGameObj::Get_Facing() const
{
	return PhysObj->Get_Facing();
}

SCRIPTS_API bool DamageableGameObj::Is_Teammate(DamageableGameObj * p_obj)
{
	return (p_obj && ((p_obj == this) || (Is_Team_Player() && Get_Player_Type() == p_obj->Get_Player_Type())));
}

int SCRIPTS_API VehicleGameObj::Get_Occupant_Count(void)
{
	int count = 0;
	for ( int i = 0; i < SeatOccupants.Length(); i++ ) {
		if ( SeatOccupants[i] != NULL ) {
			count++;
		}
	}
	return count;
}

VehicleGameObj* SoldierGameObj::Get_Profile_Vehicle( void )
{
	if (( Get_State() == HumanStateClass::IN_VEHICLE ) && Vehicle )
	{
		return Vehicle;
	}
	if (( Get_State() == HumanStateClass::TRANSITION ) && ( TransitionCompletionData != NULL ))
	{
		if ( TransitionCompletionData->Type == TransitionDataClass::VEHICLE_ENTER )
		{
			return (VehicleGameObj *)TransitionCompletionData->Vehicle.Get_Ptr();
		}
	}
	return NULL;
}

SCRIPTS_API bool DamageableGameObj::Is_Team_Player(void) 
{
	int player_type = Get_Player_Type();
	return player_type == PLAYERTYPE_NOD || player_type == PLAYERTYPE_GDI;
}

SCRIPTS_API bool DamageableGameObj::Is_Enemy(DamageableGameObj *obj)
{
	bool enemy = false;
	if (obj != this && obj != NULL)
	{
		int sTeamID1 = this->Get_Player_Type();
		int sTeamID2 = obj->Get_Player_Type();
		if ( sTeamID1 != -2 && sTeamID2 != -2 && sTeamID1 != -4 && sTeamID2 != -4 )
		{
			if ( sTeamID1 != -1 && sTeamID2 != -1 )
			{
				enemy = sTeamID1 != sTeamID2;
			}
			else
			{
				enemy = true;
			}
		}
		else
		{
			enemy = false;
		}
	}
	return enemy;
}

SCRIPTS_API AnimControlClass *PhysicalGameObj::Get_Anim_Control()
{
	return AnimControl;
}

void PhysicalGameObj::Get_Position(Vector3 *Position) const
{
	*Position = Peek_Physical_Object()->Get_Transform().Get_Translation();
}

SCRIPTS_API  const PowerUpGameObjDef & PowerUpGameObj::Get_Definition( void ) const
{
	return (const PowerUpGameObjDef &)BaseGameObj::Get_Definition();
}

SCRIPTS_API  const PhysicalGameObjDef & PhysicalGameObj::Get_Definition( void ) const
{
	return (const PhysicalGameObjDef &)BaseGameObj::Get_Definition();
}

SCRIPTS_API bool Verify_Preset_Limit(int team, int instance_count, uint32* preset_ids, int preset_id_count, GameObject* ignore)
{
	for (SLNode<SmartGameObj>* node = GameObjManager::SmartGameObjList.Head(); node; node = node->Next())
	{
		SmartGameObj* object = node->Data();
		if (object != ignore && (object->Get_Player_Type() == team || team == 2))
		{
			for (int i = 0; i < preset_id_count; ++i)
			{
				if ((uint32)object->Get_Definition().Get_ID() == preset_ids[i] && --instance_count == 0) return true;
			}
		}
	}
	return false;
}

void SCRIPTS_API Set_Powerup_Always_Allow_Grant(GameObject *obj,bool Grant)
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
	PowerUpGameObj *o3 = o2->As_PowerUpGameObj();
	if (!o3)
	{
		return;
	}
	((PowerUpGameObjDef&)o3->Get_Definition()).AlwaysAllowGrant = Grant;
}

void SCRIPTS_API PhysicalGameObj::Set_Collision_Group(int group)
{
	PhysObj->Set_Collision_Group((Collision_Group_Type)group);
}

void SCRIPTS_API VehicleGameObj::Set_Immovable(bool b)
{
	Peek_Physical_Object()->Set_Immovable(b);
	Set_Object_Dirty_Bit(NetworkObjectClass::BIT_FREQUENT,true);
	if (Peek_Physical_Object()->As_MoveablePhysClass())
	{
		Peek_Physical_Object()->As_MoveablePhysClass()->Set_Velocity(Vector3(0,0,0));
	}
}

SCRIPTS_API const SmartGameObjDef & SmartGameObj::Get_Definition( void ) const
{
	return (const SmartGameObjDef &)BaseGameObj::Get_Definition();
}
