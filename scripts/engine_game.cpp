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

#include "engine_game.h"
#include "engine_obj.h"
#include "RefineryGameObj.h"
#include "RefineryGameObjDef.h"
#include "cGameType.h"
#include "slist.h"
#include "GameObjManager.h"
#include "engine_def.h"
#include "SimpleGameObj.h"
#include "CombatManager.h"
#include "VehicleFactoryGameObj.h"
#include "VehicleFactoryGameObjDef.h"
#include "HarvesterClass.h"
#include "DefinitionMgrClass.h"
REF_DEF2(bool, CombatManager::IAmServer, 0x00855EC8, 0x008550B0);
RENEGADE_FUNCTION
void *Game_Mode_Find(const char *)
AT2(0x004372B0,0x00437350);
RENEGADE_FUNCTION
cGameData SCRIPTS_API *The_Game()
AT2(0x00477CA0,0x00477370);
RENEGADE_FUNCTION
BaseControllerClass *BaseControllerClass::Find_Base(int Team)
AT2(0x006EF790,0x006EED50);
#ifndef TT_EXPORTS
RENEGADE_FUNCTION
void BaseControllerClass::Power_Base(bool Power)
AT2(0x006EF310,0x006EE8D0);
#endif
VehicleGameObj *BaseControllerClass::Get_Harvester_Vehicle()
{
	BuildingGameObj* refinery = Find_Building(BuildingConstants::TYPE_REFINERY);
	return refinery && refinery->As_RefineryGameObj() ? refinery->As_RefineryGameObj()->Get_Harvester_Vehicle() : nullptr;
}
void BaseControllerClass::Set_Can_Generate_Soldiers(bool CanGenerate)
{
	CanGenerateSoldiers = CanGenerate;
	Set_Object_Dirty_Bit(NetworkObjectClass::BIT_OCCASIONAL,true);
}
void BaseControllerClass::Set_Can_Generate_Vehicles(bool CanGenerate)
{
	CanGenerateVehicles = CanGenerate;
	Set_Object_Dirty_Bit(NetworkObjectClass::BIT_OCCASIONAL,true);
}
void BaseControllerClass::Set_Base_Destroyed(bool newBaseDestroyed)
{
	BaseDestroyed = newBaseDestroyed;
	Set_Object_Dirty_Bit(NetworkObjectClass::BIT_OCCASIONAL,true);
}
RENEGADE_FUNCTION
void BaseControllerClass::Destroy_Base()
AT2(0x006EF840,0x006EEE00);
void BaseControllerClass::Set_Beacon_Destroyed_Base(bool BeaconDestroyed)
{
	BeaconDestroyedBase = BeaconDestroyed;
	Set_Object_Dirty_Bit(NetworkObjectClass::BIT_OCCASIONAL,true);
}
RENEGADE_FUNCTION
BuildingGameObj *BaseControllerClass::Find_Building(unsigned int Type)
AT2(0x006EF6D0,0x006EEC90);
RENEGADE_FUNCTION
cGameDataSkirmish SCRIPTS_API *The_Skirmish_Game()
AT2(0x00477CC0,0x00477390);
RENEGADE_FUNCTION
cGameDataCnc SCRIPTS_API *The_Cnc_Game()
AT2(0x00477CD0,0x004773A0);
RENEGADE_FUNCTION
cGameDataSinglePlayer SCRIPTS_API *The_Single_Player_Game()
AT2(0x00477CB0,0x00477380);

bool AllMapsAreFlying = false;

void BaseControllerClass::Check_Vehicle_Factory()
{
	if (CombatManager::I_Am_Server())
	{
		for (int i = 0; i < BuildingList.Count(); ++i)
		{
			BuildingGameObj* building = BuildingList[i];
			if (building->As_VehicleFactoryGameObj() || building->As_AirFactoryGameObj() || building->As_NavalFactoryGameObj())
			{
				if (!building->Is_Destroyed())
				{
					Set_Can_Generate_Vehicles(true);
					return;
				}
			}
		}
		Set_Can_Generate_Vehicles(false);
	}
}

void BaseControllerClass::Check_Radar()
{
	if (CombatManager::I_Am_Server())
	{
		bool radarEnabled = true;
		for (int i = 0; i < BuildingList.Count(); ++i)
		{
			const BuildingGameObj* building = BuildingList[i];
			if (building->Get_Definition().Get_Type() == BuildingConstants::TYPE_COM_CENTER)
			{
				radarEnabled &= !building->Is_Destroyed(); // We need all communications centres to be online for the radar to work.
			}
		}
		Enable_Radar(radarEnabled);
	}
}


REF_DEF2(int, cGameData::HostedGameNumber, 0x0082F120, 0x0082E308);
REF_DEF2(WideStringClass, cGameData::WinText, 0x0082F108, 0x0082E2F0);
REF_DEF2(Vector3, PrivateMessageColor, 0x0082FC20, 0x0082EE08);
REF_DEF2(Vector3, PublicMessageColor, 0x0082FBA0, 0x0082ED88);
REF_DEF2(Vector3, GDIHouseColor, 0x0085CAA8, 0x0085BC90);
REF_DEF2(Vector3, NodHouseColor, 0x0085CA98, 0x0085BC80);
REF_DEF2(bool, IsMapVTOL, 0x0085E31C, 0x0085D4F4);
REF_DEF2(bool, IsListed, 0x0082FD94, 0x0082EF7C);
REF_DEF2(unsigned int, GameMode, 0x0081F5F0, 0x0081E7D8);

void SCRIPTS_API Power_Base(int team,bool powered)
{
	BaseControllerClass *c = BaseControllerClass::Find_Base(team);
	if (c)
	{
		c->Power_Base(powered);
	}
}

void SCRIPTS_API Set_Can_Generate_Soldiers(int team,bool cangenerate)
{
	BaseControllerClass *c = BaseControllerClass::Find_Base(team);
	if (c)
	{
		c->Set_Can_Generate_Soldiers(cangenerate);
	}
}

void SCRIPTS_API Set_Can_Generate_Vehicles(int team,bool cangenerate)
{
	BaseControllerClass *c = BaseControllerClass::Find_Base(team);
	if (c)
	{
		c->Set_Can_Generate_Vehicles(cangenerate);
	}
}

void SCRIPTS_API Destroy_Base(int team)
{
	BaseControllerClass *c = BaseControllerClass::Find_Base(team);
	if (c)
	{
		c->Destroy_Base();
	}
}

void SCRIPTS_API Beacon_Destroyed_Base(int team, bool destroyed)
{
	BaseControllerClass *c = BaseControllerClass::Find_Base(team);
	if (c)
	{
		c->Set_Beacon_Destroyed_Base(destroyed);
	}
}

void SCRIPTS_API Enable_Base_Radar(int team, bool enable)
{
	BaseControllerClass *c = BaseControllerClass::Find_Base(team);
	if (c)
	{
		c->Enable_Radar(enable);
	}
}

bool SCRIPTS_API Is_Harvester(GameObject *obj)
{
	if (!obj)
	{
		return false;
	}
	if (Find_Harvester(0) == obj || Find_Harvester(1) == obj)
	{
		return true;
	}
	return false;
}

bool SCRIPTS_API Is_Radar_Enabled(int team)
{
	BaseControllerClass *c = BaseControllerClass::Find_Base(team);
	if (c)
	{
		return c->Is_Radar_Enabled();
	}
	return false;
}

int SCRIPTS_API Building_Type(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	BuildingGameObj *o = obj->As_BuildingGameObj();
	if (!o)
	{
		return 0;
	}
	const BuildingGameObjDef &d = o->Get_Definition();
	return d.Get_Type();
}

bool SCRIPTS_API Is_Building_Dead(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	BuildingGameObj *o = obj->As_BuildingGameObj();
	if (!o)
	{
		return 0;
	}
	return o->Is_Destroyed();
}

GameObject SCRIPTS_API *Find_Building_By_Type(int team,int type)
{
	BaseControllerClass *c = BaseControllerClass::Find_Base(team);
	if (c)
	{
		return c->Find_Building(type);
	}
	return 0;
}

GameObject SCRIPTS_API *Find_Base_Defense(int team)
{
	BaseControllerClass *c = BaseControllerClass::Find_Base(team);
	if (c)
	{
		return c->Find_Building(BuildingConstants::TYPE_BASE_DEFENSE);
	}
	return 0;
}

bool SCRIPTS_API Is_Map_Flying()
{
	return IsMapVTOL || AllMapsAreFlying;
}

GameObject SCRIPTS_API *Create_Building(const char *preset,const Vector3 & Position)
{
	DefinitionClass *d = Find_Named_Definition(preset);
	if(!d)
	{
		return 0;
	}
	GameObject *obj = (GameObject *)d->Create();
	
	if (obj)
	{
		BuildingGameObj *c = obj->As_BuildingGameObj();
		if (c)
		{
			c->Set_Position(Position);
			c->Collect_Building_Components();
			c->Start_Observers();
			return c;
		}
	}
	return 0;
}

void RefineryGameObj::Destroy_Harvester()
{
	if (Get_Harvester_Vehicle())
	{
		Commands->Apply_Damage(Get_Harvester_Vehicle(),99999,"Death",0);
	}
}

VehicleGameObj *
RefineryGameObj::Get_Harvester_Vehicle (void)
{
	if (HarvesterVehicle.Get_Ptr()) 
	{
		return HarvesterVehicle->As_VehicleGameObj();
	}
	return 0;
}

GameObject SCRIPTS_API *Find_Harvester(int team)
{
	BaseControllerClass *c = BaseControllerClass::Find_Base(team);
	GameObject *o = 0;
	if (c)
	{
		o = c->Get_Harvester_Vehicle();
	}
	return o;
}

bool SCRIPTS_API Is_Base_Powered(int team)
{
	BaseControllerClass *c = BaseControllerClass::Find_Base(team);
	if (c)
	{
		return c->Is_Base_Powered();
	}
	return false;
}

bool SCRIPTS_API Can_Generate_Vehicles(int team)
{
	BaseControllerClass *c = BaseControllerClass::Find_Base(team);
	if (c)
	{
		return c->Can_Generate_Vehicles();
	}
	return false;
}

bool SCRIPTS_API Can_Generate_Soldiers(int team)
{
	if ( BaseControllerClass *c = BaseControllerClass::Find_Base(team) )
		return c->Can_Generate_Soldiers();
	return false;
}

int SCRIPTS_API Get_Building_Count_Team(int Team)
{
	SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();
	int Count = 0;
	while (x)
	{
		GameObject *o = x->Data();
		if (o && Commands->Get_Health(o) > 0 && (Get_Object_Type(o) == Team || Team == 2))
		{
			Count++;
		}
		x = x->Next();
	}
	return Count;
}

GameObject SCRIPTS_API *Find_Building_By_Class_ID(int Team,unsigned long Class_ID)
{
	SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			const DefinitionClass *d = &o->Get_Definition();
			if (d->Get_Class_ID() == Class_ID)
			{
				if ((Get_Object_Type(o) == Team) || (Team == 2))
				{
					return o;
				}
			}
		}
		x = x->Next();
	}
	return 0;
}

GameObject SCRIPTS_API *Find_Power_Plant(int Team)
{
	return Find_Building_By_Type(Team,BuildingConstants::TYPE_POWER_PLANT);
}

GameObject SCRIPTS_API *Find_Construction_Yard(int Team)
{
	return Find_Building_By_Type(Team,BuildingConstants::TYPE_CONYARD);
}

GameObject SCRIPTS_API *Find_Refinery(int Team)
{
	return Find_Building_By_Type(Team,BuildingConstants::TYPE_REFINERY);
}

GameObject SCRIPTS_API *Find_Repair_Bay(int Team)
{
	return Find_Building_By_Type(Team,BuildingConstants::TYPE_REPAIR_BAY);
}

GameObject SCRIPTS_API *Find_Soldier_Factory(int Team)
{
	return Find_Building_By_Type(Team,BuildingConstants::TYPE_SOLDIER_FACTORY);
}

GameObject SCRIPTS_API *Find_Vehicle_Factory(int Team)
{
	return Find_Building_By_Type(Team,BuildingConstants::TYPE_VEHICLE_FACTORY);
}

GameObject SCRIPTS_API *Find_Shrine(int Team)
{
	return Find_Building_By_Type(Team,BuildingConstants::TYPE_SHRINE);
}

GameObject SCRIPTS_API *Find_Helipad(int Team)
{
	return Find_Building_By_Type(Team,BuildingConstants::TYPE_HELIPAD);
}

GameObject SCRIPTS_API *Find_Tech_Center(int Team)
{
	return Find_Building_By_Type(Team,BuildingConstants::TYPE_TECH_CENTER);
}

GameObject SCRIPTS_API *Find_Naval_Factory(int Team)
{
	return Find_Building_By_Type(Team,BuildingConstants::TYPE_NAVAL_FACTORY);
}

GameObject SCRIPTS_API *Find_Special(int Team)
{
	return Find_Building_By_Type(Team,BuildingConstants::TYPE_SPECIAL);
}

GameObject SCRIPTS_API *Find_Com_Center(int Team)
{
	return Find_Building_By_Type(Team,BuildingConstants::TYPE_COM_CENTER);
}

GameObject SCRIPTS_API *Find_Building_By_Team(int Team)
{
	SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();
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

GameObject SCRIPTS_API *Find_Building_By_Preset(int Team,const char *Preset_Name)
{
	SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			const DefinitionClass *d = &o->Get_Definition();
			if (!_stricmp(d->Get_Name(),Preset_Name))
			{
				if ((Get_Object_Type(o) == Team) || (Team == 2))
				{
					return o;
				}
			}
		}
		x = x->Next();
	}
	return 0;
}

void SCRIPTS_API Restore_Building(GameObject* obj) 
{
	if (!obj) return;
	BuildingGameObj *Building = obj->As_BuildingGameObj();
	if (Building)
	{
		Building->On_Revived();
	}
}

bool SCRIPTS_API Is_Gameplay_Permitted()
{
	return The_Game()->Is_Gameplay_Permitted();
}

bool SCRIPTS_API Is_Dedicated()
{
	return Exe == 1;
}

unsigned int SCRIPTS_API Get_Current_Game_Mode()
{
	if (cGameType::GameType == 1)
	{
		return 1;
	}
	if (cGameType::GameType == 2)
	{
		return 2;
	}
	if (cGameType::GameType == 3)
	{
		if (GameMode == 1)
		{
			return 5;
		}
		if (GameMode == 2)
		{
			return 3;
		}
		if (GameMode == 3)
		{
			return 4;
		}
		if (Game_Mode_Find("WOL"))
		{
			return 3;
		}
		if (Game_Mode_Find("LAN"))
		{
			if (IsListed)
			{
				return 4;
			}
			else
			{
				return 5;
			}
		}
	}
	return 0;
}

void SCRIPTS_API Get_Private_Message_Color(unsigned int *red, unsigned int *green, unsigned int *blue)
{
	*red = (unsigned int)(PrivateMessageColor.X*255);
	*green = (unsigned int)(PrivateMessageColor.Y*255);
	*blue = (unsigned int)(PrivateMessageColor.Z*255);
}

void SCRIPTS_API Get_Public_Message_Color(unsigned int *red, unsigned int *green, unsigned int *blue)
{
	*red = (unsigned int)(PublicMessageColor.X*255);
	*green = (unsigned int)(PublicMessageColor.Y*255);
	*blue = (unsigned int)(PublicMessageColor.Z*255);
}

int SCRIPTS_API Get_Harvester_Preset_ID(int Team)
{
	if (!Find_Refinery(Team))
	{
		return 0;
	}
	BuildingGameObj *Ref = Find_Refinery(Team)->As_BuildingGameObj();
	if (!Ref || !Ref->As_RefineryGameObj())
	{
		return 0;
	}
	return Ref->As_RefineryGameObj()->Get_Harvester_Def_ID();
}

bool SCRIPTS_API Is_Harvester_Preset(GameObject *obj)
{
	int ID = Commands->Get_Preset_ID(obj);
	if (Get_Harvester_Preset_ID(0) == ID || Get_Harvester_Preset_ID(1) == ID)
	{
		return true;
	}
	return false;
}

const SimpleGameObjDef SCRIPTS_API & SimpleGameObj::Get_Definition( void ) const
{
	return (const SimpleGameObjDef &)BaseGameObj::Get_Definition();
}

bool SCRIPTS_API Vehicle_Preset_Is_Air(int preset)
{
	DefinitionClass *def = DefinitionMgrClass::Find_Definition(preset,true);
	if (!def)
	{
		return false;
	}
	if (def->Get_Class_ID() == 12304)
	{
		VehicleGameObjDef *def2 = (VehicleGameObjDef *)def;
		if (def2->Get_Type() == VEHICLE_TYPE_FLYING && !def2->Get_Build_At_Ground())
		{
			return true;
		}
	}
	return false;
}
bool SCRIPTS_API Vehicle_Preset_Is_Naval(int preset)
{
	DefinitionClass *def = DefinitionMgrClass::Find_Definition(preset,true);
	if (!def)
	{
		return false;
	}
	if (def->Get_Class_ID() == 12304)
	{
		VehicleGameObjDef *def2 = (VehicleGameObjDef *)def;
		if (def2->Get_Type() == VEHICLE_TYPE_BOAT || def2->Get_Type() == VEHICLE_TYPE_SUB)
		{
			return true;
		}
	}
	return false;
}

Vector3 SCRIPTS_API Get_Base_Center(int Team)
{
		Vector3 ReturnPos(0.0f,0.0f,0.0f);
		BaseControllerClass *Base = BaseControllerClass::Find_Base(Team);
		if (Base)
		{
				int Num = 0;
				for (Num = 0;Num < Base->Get_Building_List().Count();Num++)
				{
						ReturnPos += Commands->Get_Position(Base->Get_Building_List()[Num]);
				}
				if (Num)
				{
						ReturnPos /= (float)Num;
				}
		}
		return ReturnPos;
}

BuildingAggregateClass *BuildingGameObj::Find_MCT()
{
	for (auto it = Aggregates.Iterator(); it; ++it)
	{
		if (it->Is_MCT())
		{
			return it.Peek_Obj();
		}
	}
	return 0;
}

void SCRIPTS_API Seconds_To_Hms(float secs, int &hours, int &minutes, int &seconds)
{
	int i1 = (int) (secs * 0.00027777778f);
	hours = i1;
	float f1 = secs - (float) (3600 * (int) i1);
	int i2 = (int) (f1 * 0.016666668f);
	minutes = i2;
	seconds = (int) (f1 - (float) (60 * (int) i2));
}
