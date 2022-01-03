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
#include "slist.h"
#include "VehicleGameObj.h"
#include "GameObjManager.h"
#include "engine_obj.h"
#include "engine_obj2.h"
#include "engine_phys.h"
#include "ArmorWarheadManager.h"
#include "BuildingGameObj.h"
#include "VehicleGameObjDef.h"
#ifndef TT_EXPORTS
RENEGADE_FUNCTION
int ArmorWarheadManager::Get_Num_Armor_Types()
AT3(0x00687730, 0x00686FD0, 0x00519180);

RENEGADE_FUNCTION
int ArmorWarheadManager::Get_Num_Warhead_Types()
AT3(0x00687740, 0x00686FE0, 0x00519190);

RENEGADE_FUNCTION
const char *ArmorWarheadManager::Get_Armor_Name(ArmorType)
AT3(0x006877D0, 0x00687070, 0x00519240);

RENEGADE_FUNCTION
ArmorType ArmorWarheadManager::Get_Armor_Type(const char*)
AT3(0x00687750, 0x00686FF0, 0x005191A0);

RENEGADE_FUNCTION
WarheadType ArmorWarheadManager::Get_Warhead_Type(const char*)
AT3(0x00687790, 0x00687030, 0x005191F0);

RENEGADE_FUNCTION
const char *ArmorWarheadManager::Get_Warhead_Name(WarheadType)
AT3(0x006877E0, 0x00687080, 0x005192A0);

RENEGADE_FUNCTION
float ArmorWarheadManager::Get_Damage_Multiplier(ArmorType, WarheadType)
AT3(0x006877F0, 0x00687090, 0x00519300);

RENEGADE_FUNCTION
ArmorWarheadManager::SpecialDamageType	ArmorWarheadManager::Get_Special_Damage_Type( WarheadType type )
AT3(0x00687E60, 0x00687700, 0x005194E0);

RENEGADE_FUNCTION
float	ArmorWarheadManager::Get_Shield_Absorbsion( ArmorType armor, WarheadType warhead )
AT3(0x00687AE0,0x00687380,0x005193A0);

RENEGADE_FUNCTION
bool				ArmorWarheadManager::Is_Armor_Soft( ArmorType armor )
AT3(0x00687DD0,0x00687670,0x00519440);

RENEGADE_FUNCTION
float	ArmorWarheadManager::Get_Special_Damage_Probability( WarheadType type )
AT3(0x00687E70,0x00687710,0x00519540);

RENEGADE_FUNCTION
WarheadType ArmorWarheadManager::Get_Special_Damage_Warhead( SpecialDamageType type )
AT3(0x00687E80,0x00687720,0x005195A0);

RENEGADE_FUNCTION
float	ArmorWarheadManager::Get_Special_Damage_Duration( SpecialDamageType type )
AT3(0x00687E90,0x00687730,0x005195F0);

RENEGADE_FUNCTION
float	ArmorWarheadManager::Get_Special_Damage_Scale( SpecialDamageType type )
AT3(0x00687EA0,0x00687740,0x00519640);

RENEGADE_FUNCTION
const char *	ArmorWarheadManager::Get_Special_Damage_Explosion( SpecialDamageType type )
AT3(0x00687EB0,0x00687750,0x00519690);

RENEGADE_FUNCTION
float	ArmorWarheadManager::Get_Visceroid_Probability( WarheadType type )
AT3(0x00687EC0,0x00687760,0x005196E0);

RENEGADE_FUNCTION
bool	ArmorWarheadManager::Is_Skin_Impervious( SpecialDamageType type, ArmorType skin )
AT3(0x00687ED0,0x00687770,0x00519740);

#endif
#ifndef TTLE_EXPORTS
#include "Engine_Game.h"
void SCRIPTS_API Damage_All_Buildings_By_Team(int Team,float Damage,const char *Warhead,GameObject *Damager)
{
	SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			if ((Get_Object_Type(o) == Team) || (Team == 2))
			{
				Commands->Apply_Damage(o,Damage,Warhead,Damager);
			}
		}
		x = x->Next();
	}
}

void SCRIPTS_API Repair_All_Buildings_By_Team(int Team,int ConstructionYardID,float Health)
{
	SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			if ((Get_Object_Type(o) == Team) && (Commands->Get_ID(o) != ConstructionYardID))
			{
				float amount = Commands->Get_Health(o);
				if (amount>0)
				{
					Commands->Set_Health(o,(amount+Health));
				}
			}
		}
		x = x->Next();
	}
}

void SCRIPTS_API Repair_All_Static_Vehicles_By_Team(int Team,int type)
{
	SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();
	while (x)
	{
		VehicleGameObj *o = x->Data();
		if (o)
		{
			if (Get_Object_Type(o) == Team && o->Peek_Physical_Object() && o->Peek_Physical_Object()->As_DecorationPhysClass())
			{
				Commands->Send_Custom_Event(o,o,type,0,0);
			}
		}
		x = x->Next();
	}
}

void SCRIPTS_API Set_Max_Health(GameObject *obj,float health)
{
	if (!obj)
	{
		return;
	}
	DamageableGameObj *o = obj->As_DamageableGameObj();
	if (!o)
	{
		return;
	}
	o->Get_Defense_Object()->Set_Health_Max(health);
	Commands->Set_Health(obj,health);
}

void SCRIPTS_API Set_Max_Health_Without_Healing(GameObject *obj,float health)
{
	if (!obj)
	{
		return;
	}
	DamageableGameObj *o = obj->As_DamageableGameObj();
	if (!o)
	{
		return;
	}
	o->Get_Defense_Object()->Set_Health_Max(health);
}

void SCRIPTS_API Set_Max_Shield_Strength(GameObject *obj,float shieldstrength)
{
	if (!obj)
	{
		return;
	}
	DamageableGameObj *o = obj->As_DamageableGameObj();
	if (!o)
	{
		return;
	}
	o->Get_Defense_Object()->Set_Shield_Strength_Max(shieldstrength);
	Commands->Set_Shield_Strength(obj,shieldstrength);
}

void SCRIPTS_API Set_Max_Shield_Strength_Without_Healing(GameObject *obj,float shieldstrength)
{
	if (!obj)
	{
		return;
	}
	DamageableGameObj *o = obj->As_DamageableGameObj();
	if (!o)
	{
		return;
	}
	o->Get_Defense_Object()->Set_Shield_Strength_Max(shieldstrength);
}

const char SCRIPTS_API *Get_Shield_Type(GameObject *obj)
{
	if (!obj)
	{
		return "";
	}
	DamageableGameObj *o = obj->As_DamageableGameObj();
	if (!o)
	{
		return "";
	}
	return ArmorWarheadManager::Get_Armor_Name(o->Get_Defense_Object()->Get_Shield_Type());
}

const char SCRIPTS_API *Get_Skin(GameObject *obj)
{
	if (!obj)
	{
		return "";
	}
	DamageableGameObj *o = obj->As_DamageableGameObj();
	if (!o)
	{
		return "";
	}
	return ArmorWarheadManager::Get_Armor_Name(o->Get_Defense_Object()->Get_Skin());
}

void SCRIPTS_API Set_Skin(GameObject *obj,const char *Skintype)
{
	if (!obj)
	{
		return;
	}
	DamageableGameObj *o = obj->As_DamageableGameObj();
	if (!o)
	{
		return;
	}
	unsigned int skin = ArmorWarheadManager::Get_Armor_Type(Skintype);
	o->Get_Defense_Object()->Set_Skin(skin);
	o->Get_Defense_Object()->Mark_Owner_Dirty();
}

void SCRIPTS_API Damage_All_Vehicles_Area(float Damage,const char *Warhead,const Vector3 &Position,float Distance,GameObject *Host,GameObject *Damager)
{
	if (!Host)
	{
		return;
	}
	Vector3 TestPosition = Position;
	TestPosition.Z = 0;
	SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();
	while (x)
	{
		GameObject *obj = x->Data();
		if (obj) 
		{
			Vector3 ObjPosition = Commands->Get_Position(obj);

			ObjPosition.Z = 0;
			if ((Vector3::Distance_Squared(ObjPosition, TestPosition) <= Distance * Distance) && (Commands->Get_ID(obj) != Commands->Get_ID(Host)))
			{
				Commands->Apply_Damage(obj,Damage,Warhead,Damager);
			}
		}
		x = x->Next();
	}
}

void SCRIPTS_API Damage_All_Objects_Area(float Damage,const char *Warhead,const Vector3 &Position,float Distance,GameObject *Host,GameObject *Damager) 
{
	if (!Host)
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
			if ((Vector3::Distance_Squared(ObjPosition, TestPosition) <= Distance * Distance) && (Commands->Get_ID(o2) != Commands->Get_ID(Host)))
			{
				Commands->Apply_Damage(o2,Damage,Warhead,Damager);
			}
		}
		x = x->Next();
	}
}

void SCRIPTS_API Damage_All_Objects_Area_By_Team(float Damage,const char *Warhead,const Vector3 &Position,float Distance,GameObject *Damager,bool Soldiers,bool Vehicles,int Team) 
{
	SLNode<SmartGameObj> *x = GameObjManager::SmartGameObjList.Head();
	while (x) 
	{
		SmartGameObj *o = x->Data();
		if (o)
		{
			if (o->Get_Player_Type() == Team || Team == 2)
			{
				if (o->As_SoldierGameObj() && Soldiers || o->As_VehicleGameObj() && Vehicles)
				{
					Vector3 ObjPosition = Commands->Get_Position(o);
					Vector3 TestPosition = Position;
					if (Vector3::Distance_Squared(ObjPosition, TestPosition) <= Distance * Distance)
					{
						Commands->Apply_Damage(o,Damage,Warhead,Damager);
					}
				}
			}
		}
		x = x->Next();
	}
}

void SCRIPTS_API Ranged_Damage_To_Buildings(float Damage,const char *Warhead,Vector3 Location,float DamageRadius,GameObject *Damager)
{
	if (DamageRadius > 0)
	{
		SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();
		while (x)
		{
			GameObject *o = x->Data();
			if (o)
			{
				float health = Commands->Get_Health(o);
				if (health > 0)
				{
					Vector3 pos1 = Commands->Get_Position(o);
					Vector3 pos2 = Location;

					if (Vector3::Distance_Squared(pos1, pos2) <= DamageRadius * DamageRadius)
					{
						Commands->Apply_Damage(o,Damage,Warhead,Damager);
					}
				}
			}
			x = x->Next();
		}
	}
}

void SCRIPTS_API Ranged_Scale_Damage_To_Buildings(float Damage,const char *Warhead,Vector3 Location,float DamageRadius,GameObject *Damager)
{
	if (DamageRadius > 0)
	{
		float Ratio = Damage/DamageRadius;

		SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();
		while (x)
		{
			GameObject *o = x->Data();
			if (o)
			{
				float health = Commands->Get_Health(o);
				if (health > 0)
				{
					Vector3 pos1 = Commands->Get_Position(o);
					Vector3 pos2 = Location;

					float Distance = Commands->Get_Distance(pos1,pos2);

					if (Distance <= DamageRadius)
					{
						float scaledamage = (Damage - (Distance*(Ratio)));
						Commands->Apply_Damage(o,scaledamage,Warhead,Damager);
					}
				}
			}
			x = x->Next();
		}
	}
}

void SCRIPTS_API Ranged_Percentage_Damage_To_Buildings(float Percentage,const char *Warhead,Vector3 Location,float DamageRadius,GameObject *Damager)
{
	if (DamageRadius > 0)
	{
		SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();
		while (x)
		{
			GameObject *o = x->Data();
			if (o)
			{
				float health = Commands->Get_Health(o);
				if (health > 0)
				{
					Vector3 pos1 = Commands->Get_Position(o);
					Vector3 pos2 = Location;

					if (Vector3::Distance_Squared(pos1, pos2) <= DamageRadius * DamageRadius)
					{
						float Max_Health = Commands->Get_Max_Health(o);
						float Damage = Max_Health*Percentage;
						Commands->Apply_Damage(o,Damage,Warhead,Damager);
					}
				}
			}
			x = x->Next();
		}
	}
}

void SCRIPTS_API Ranged_Damage_To_Buildings_Team(int Team,float Damage,const char *Warhead,Vector3 Location,float DamageRadius,GameObject *Damager)
{
	if (DamageRadius > 0)
	{
		SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();
		while (x)
		{
			GameObject *o = x->Data();
			if (o)
			{
				if ((Get_Object_Type(o) == Team) || (Team == 2))
				{
					float health = Commands->Get_Health(o);
					if (health > 0)
					{
						Vector3 pos1 = Commands->Get_Position(o);
						Vector3 pos2 = Location;
						if (Vector3::Distance_Squared(pos1, pos2) <= DamageRadius * DamageRadius)
						{
							Commands->Apply_Damage(o,Damage,Warhead,Damager);
						}
					}
				}
			}
			x = x->Next();
		}
	}
}

void SCRIPTS_API Ranged_Scale_Damage_To_Buildings_Team(int Team,float Damage,const char *Warhead,Vector3 Location,float DamageRadius,GameObject *Damager)
{
	if (DamageRadius > 0)
	{
		float Ratio = Damage/DamageRadius;

		SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();
		while (x)
		{
			GameObject *o = x->Data();
			if (o)
			{
				if ((Get_Object_Type(o) == Team) || (Team == 2))
				{
					float health = Commands->Get_Health(o);
					if (health > 0)
					{
						Vector3 pos1 = Commands->Get_Position(o);
						Vector3 pos2 = Location;
						float Distance = Commands->Get_Distance(pos1,pos2);
						if (Distance <= DamageRadius)
						{
							float scaledamage = (Damage - (Distance*(Ratio)));
							Commands->Apply_Damage(o,scaledamage,Warhead,Damager);
						}
					}
				}
			}
			x = x->Next();
		}
	}
}

void SCRIPTS_API Ranged_Percentage_Damage_To_Buildings_Team(int Team,float Percentage,const char *Warhead,Vector3 Location,float DamageRadius,GameObject *Damager)
{
	if (DamageRadius > 0)
	{
		SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();
		while (x)
		{
			GameObject *o = x->Data();
			if (o)
			{
				if ((Get_Object_Type(o) == Team) || (Team == 2))
				{
					float health = Commands->Get_Health(o);
					if (health > 0)
					{
						Vector3 pos1 = Commands->Get_Position(o);
						Vector3 pos2 = Location;
						if (Vector3::Distance_Squared(pos1, pos2) <= DamageRadius * DamageRadius)
						{
							float Max_Health = Commands->Get_Max_Health(o);
							float Damage = Max_Health*Percentage;
							Commands->Apply_Damage(o,Damage,Warhead,Damager);
						}
					}
				}
			}
			x = x->Next();
		}
	}
}

void SCRIPTS_API Damage_Objects_Half()
{
	SLNode<SmartGameObj> *z = GameObjManager::SmartGameObjList.Head();
	while (z)
	{
		GameObject *obj = z->Data();
		if (obj)
		{
			float Health = Commands->Get_Max_Health(obj) + Commands->Get_Max_Shield_Strength(obj);
			Commands->Apply_Damage(obj,(Health / 2),"None",0);
		}
		z = z->Next();
	}
}

void SCRIPTS_API Ranged_Percentage_Damage_To_Vehicles(float Percentage,const char *Warhead,Vector3 Location,float DamageRadius,GameObject *Damager)
{
	if (DamageRadius > 0)
	{
		SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();
		while (x)
		{
			GameObject *o = x->Data();
			if (o)
			{
				float health = Commands->Get_Health(o);
				if (health > 0)
				{
					Vector3 pos1 = Commands->Get_Position(o);
					Vector3 pos2 = Location;
					float Distance = Vector3::Distance_Squared(pos1, pos2);
					if (Distance <= DamageRadius * DamageRadius)
					{
						float Max_Health = Commands->Get_Max_Health(o);
						float Max_Shield = Commands->Get_Max_Shield_Strength(o);
						float Damage = (Max_Health + Max_Shield) * Percentage;
						Commands->Apply_Damage(o,Damage,Warhead,Damager);
					}
				}
			}
			x = x->Next();
		}
	}
}

void SCRIPTS_API Set_Damage_Points(GameObject *obj,float points)
{
	if (!obj)
	{
		return;
	}
	DamageableGameObj *o = obj->As_DamageableGameObj();
	if (!o)
	{
		return;
	}
	o->Get_Defense_Object()->Set_Damage_Points(points);
}

void SCRIPTS_API Set_Death_Points(GameObject *obj,float points)
{
	if (!obj)
	{
		return;
	}
	DamageableGameObj *o = obj->As_DamageableGameObj();
	if (!o)
	{
		return;
	}
	o->Get_Defense_Object()->Set_Death_Points(points);
}

void SCRIPTS_API Repair_All_Buildings_By_Team_Radius(int Team,int ConstructionYardID,float Health,float Radius)
{
	Vector3 Location = Commands->Get_Position(Commands->Find_Object(ConstructionYardID));
	SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			if ((Get_Object_Type(o) == Team) && (Commands->Get_ID(o) != ConstructionYardID))
			{
				float amount = Commands->Get_Health(o);
				if (amount>0)
				{
					Vector3 pos1 = Commands->Get_Position(o);
					Vector3 pos2 = Location;
					float Distance = Vector3::Distance_Squared(pos1, pos2);
					if (Distance <= Radius * Radius)
					{
						Commands->Set_Health(o,(amount+Health));
					}
				}
			}
		}
		x = x->Next();
	}
}

void SCRIPTS_API Repair_All_Turrets_By_Team(int team,float health)
{
	SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			if (Get_Object_Type(o) == team && (Get_Vehicle_Mode(o) == VEHICLE_TYPE_TURRET))
			{
				float amount = Commands->Get_Health(o);
				if (amount>0)
				{
					Commands->Set_Health(o,(amount+health));
				}
				amount = Commands->Get_Shield_Strength(o);
				if (amount>0)
				{
					Commands->Set_Shield_Strength(o,(amount+health));
				}
			}
		}
		x = x->Next();
	}
}

void SCRIPTS_API Damage_All_Objects_In_Zone(GameObject *Zone,float Damage,const char *Warhead,GameObject *Damager)
{
	if (!Zone)
	{
		return;
	}
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
			if (IsInsideZone(Zone,o2))
			{
				Commands->Apply_Damage(o2,Damage,Warhead,Damager);
			}
		}
		x = x->Next();
	}
}

float SCRIPTS_API Get_Damage_Points(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	DamageableGameObj *o = obj->As_DamageableGameObj();
	if (!o)
	{
		return 0;
	}
	return o->Get_Defense_Object()->Get_Damage_Points();
}

float SCRIPTS_API Get_Death_Points(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	DamageableGameObj *o = obj->As_DamageableGameObj();
	if (!o)
	{
		return 0;
	}
	return o->Get_Defense_Object()->Get_Death_Points();
}

void SCRIPTS_API Damage_Occupants(GameObject *obj,float Damage,const char *Warhead)
{
	if (!obj || !obj->As_VehicleGameObj())
	{
		return;
	}
	VehicleGameObj *o = obj->As_VehicleGameObj();
	int x = o->Get_Definition().Get_Seat_Count();
	for (int i = 0;i < x;i++)
	{
		if (o->Get_Occupant(i))
		{
			Commands->Apply_Damage(o->Get_Occupant(i),Damage,Warhead,0);
		}
	}
}

void SCRIPTS_API Kill_Occupants(GameObject *obj)
{
	if (!obj || !obj->As_VehicleGameObj())
	{
		return;
	}
	VehicleGameObj *o = obj->As_VehicleGameObj();
	int x = o->Get_Definition().Get_Seat_Count();
	for (int i = 0;i < x;i++)
	{
		if (o->Get_Occupant(i))
		{
			Commands->Apply_Damage((GameObject *)o->Get_Occupant(i),99999,"Death",0);
		}
	}
}

void SCRIPTS_API Ranged_Variable_Percent_Vehicle_Damage(float EnemyPercentage, float FriendPercentage, bool ForceFriendly, const char *Warhead,Vector3 Location,float DamageRadius,GameObject *Damager)
{
	//This function will apply a different damage amount to friendly vehicles than it
	//does to all other vehicles. It can also force friendly fire
	if (DamageRadius > 0)
	{
		SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();
		while (x)
		{
			GameObject *o = x->Data();
			if (o)
			{
				float health = Commands->Get_Health(o);
				if (health > 0)
				{
					Vector3 pos1 = Commands->Get_Position(o);
					Vector3 pos2 = Location;
					float Distance = Vector3::Distance_Squared(pos1, pos2);
					if (Distance <= DamageRadius * DamageRadius)
					{
						float Max_Health = Commands->Get_Max_Health(o);
						float Max_Shield = Commands->Get_Max_Shield_Strength(o);
						if(Commands->Get_Player_Type(Damager) == Commands->Get_Player_Type(o)) 
						{
							float Damage = (Max_Health + Max_Shield) * FriendPercentage;
							if ((Get_Vehicle_Mode(o) != VEHICLE_TYPE_TURRET))
							{
								if (ForceFriendly || The_Game()->Is_Friendly_Fire_Permitted())
								{
									Commands->Apply_Damage(o,Damage,Warhead,0);
								}
								else
								{
									Commands->Apply_Damage(o,Damage,Warhead,Damager);
								}
							}
						}
						else if (Get_Vehicle_Mode(o) != VEHICLE_TYPE_TURRET)
						{
							float Damage = (Max_Health + Max_Shield) * EnemyPercentage;
							Commands->Apply_Damage(o,Damage,Warhead,Damager);
						}
					}
				}
			}
			x = x->Next();
		}
		SLNode<BaseGameObj> *x2 = GameObjManager::GameObjList.Head();
		while (x2)
		{
			BaseGameObj *ox = x2->Data();
			PhysicalGameObj *o = 0;
			if (ox)
			{
				o = ox->As_PhysicalGameObj();
			}
			if (o)
			{
				if (o->Get_Definition().Get_Encyclopedia_Type() == 3)
				{
					float health = Commands->Get_Health(o);
					if (health > 0)
					{
						Vector3 pos1 = Commands->Get_Position(o);
						Vector3 pos2 = Location;
						float Distance = Vector3::Distance_Squared(pos1, pos2);
						if (Distance <= DamageRadius * DamageRadius)
						{
							float Max_Health = Commands->Get_Max_Health(o);
							float Max_Shield = Commands->Get_Max_Shield_Strength(o);
							if (Commands->Get_Player_Type(Damager) == Commands->Get_Player_Type(o))
							{
								float Damage = (Max_Health + Max_Shield) * FriendPercentage;
								if (ForceFriendly || The_Game()->Is_Friendly_Fire_Permitted())
								{
									Commands->Apply_Damage(o, Damage, Warhead, 0);
								}
								else
								{
									Commands->Apply_Damage(o, Damage, Warhead, Damager);
								}
							}
							else
							{
								float Damage = (Max_Health + Max_Shield) * EnemyPercentage;
								Commands->Apply_Damage(o, Damage, Warhead, Damager);
							}
						}
					}
				}
			}
			x2 = x2->Next();
		}
	}
}

float SCRIPTS_API Get_Max_Hitpoints ( GameObject* obj )
{
  return Commands->Get_Max_Health(obj) + Commands->Get_Max_Shield_Strength(obj);
}

float SCRIPTS_API Get_Hitpoints ( GameObject* obj )
{
  return Commands->Get_Health(obj) + Commands->Get_Shield_Strength(obj);
}

bool SCRIPTS_API Is_Valid_Armor_Type(const char* name)
{
  // Note: This really belongs in ArmorWarheadManager but I couldn't figure out how to get scripts.dll
  // to link to it since ArmorWarheadManager is compiled in bandtest.dll. Feel free to move it into there
  // and improve it to simply iterate the array property...
  unsigned int id = ArmorWarheadManager::Get_Armor_Type(name);

  // 0 could mean armor index 0 OR not found, disambiguate by re-checking against the name for ID 0
  return (id != 0 || _stricmp(name, ArmorWarheadManager::Get_Armor_Name(0)) == 0);
}

bool SCRIPTS_API Is_Valid_Warhead_Type(const char* name)
{
  // Note: This really belongs in ArmorWarheadManager but I couldn't figure out how to get scripts.dll
  // to link to it since ArmorWarheadManager is compiled in bandtest.dll. Feel free to move it into there
  // and improve it to simply iterate the array property...
  unsigned int id = ArmorWarheadManager::Get_Warhead_Type(name);
  
  // 0 could mean warhead index 0 OR not found, disambiguate by re-checking against the name for ID 0
  return (id != 0 || _stricmp(name, ArmorWarheadManager::Get_Warhead_Name(0)) == 0);
}

#endif
