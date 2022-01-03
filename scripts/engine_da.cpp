/*	Renegade Scripts.dll
    Dragonade Engine Functions
	Copyright 2017 Whitedragon, Tiberian Technologies

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
#include "engine_DA.h"
#include "da.h"
#include "da_hook.h"
#include "da_player.h"
#include "da_translation.h"
#include "TeamPurchaseSettingsDefClass.h"
#include "PurchaseSettingsDefClass.h"
#include "WeaponBagClass.h"
#include "WeaponClass.h"
#include "datasafe.h"
#include "GameObjManager.h"
#include "VehicleFactoryGameObj.h"
#include "VehicleFactoryGameObjDef.h"
#include "VehicleGameObj.h"
#include "VehicleGameObjDef.h"
#include "SoldierGameObj.h"
#include "SoldierGameObjDef.h"
#include "RefineryGameObj.h"
#include "RefineryGameObjDef.h"
#include "PowerupGameObj.h"
#include "PowerupGameObjDef.h"
#include "DefinitionMgrClass.h"
#include "TranslateDBClass.h"
#include "TDBObjClass.h"
#include "SmartGameObj.h"
#include "SmartGameObjDef.h"
#include "ScriptZoneGameObj.h"
#include "ScriptZoneGameObjDef.h"
#include "MoveablePhysClass.h"
#include "C4GameObj.h"
#include "C4GameObjDef.h"
#include "weaponmgr.h"
#include "PhysicsSceneClass.h"
#include "SoldierFactoryGameObj.h"
#include "SoldierFactoryGameObjDef.h"
#include "RawFileClass.h"
#include "BeaconGameObj.h"
#include "BeaconGameObjDef.h"
#include "OffenseObjectClass.h"
#include "TransitionInstanceClass.h"
#include "cScTextObj.h"
#include "SCAnnouncement.h"
#include "AirFactoryGameObj.h"
#include "NavalFactoryGameObj.h"
#include "SpawnerClass.h"
#include "CombatManager.h"
#include "cPlayer.h"

RENEGADE_FUNCTION
void VehicleGameObj::Add_Occupant(SoldierGameObj*,int)
AT2(0,0x0067AB70);

REF_DEF2(DynamicVectorClass<SpawnerClass*>,SpawnerList,0,0x008564A8);

RENEGADE_FUNCTION
WeaponClass *WeaponBagClass::Add_Weapon(const WeaponDefinitionClass *,int,bool) 
AT2(0,0x006F3000);
RENEGADE_FUNCTION
void WeaponBagClass::Remove_Weapon(int) 
AT2(0,0x006F2EF0);
RENEGADE_FUNCTION
void WeaponBagClass::Clear_Weapons() 
AT2(0,0x006F2F50);
RENEGADE_FUNCTION
void WeaponBagClass::Select_Index(int)
AT2(0,0x006F36C0);
RENEGADE_FUNCTION
void WeaponBagClass::Select_Next() 
AT2(0,0x006F3260);
RENEGADE_FUNCTION
void WeaponBagClass::Select_Prev() 
AT2(0,0x006F32F0);

RENEGADE_FUNCTION
void PhysicalGameObj::Attach_To_Object_Bone(PhysicalGameObj *,const char *)
AT2(0,0x006A5270);

REF_DEF2(bool,CombatManager::FriendlyFirePermitted,0,0x008550E4);
REF_DEF2(bool,CombatManager::BeaconPlacementEndsGame,0,0x008550E5);
REF_DEF2(bool,BuildingGameObj::CanRepairBuildings,0,0x00810474);
REF_DEF2(float, TimeScale, 0x00811E60, 0x00811038);

RENEGADE_FUNCTION
bool cGameData::Set_Max_Players(int)
AT2(0,0x00472080);

RENEGADE_FUNCTION
bool BeaconGameObj::Is_In_Enemy_Base()
AT2(0,0x00709A90);

void Destroy_All_Objects_With_Script(const char *Script) {
	for (SLNode<BaseGameObj> *z = GameObjManager::GameObjList.Head();z;z = z->Next()) {
		if (z->Data()->As_ScriptableGameObj()) {
			if (Is_Script_Attached((ScriptableGameObj*)z->Data(),Script)) {
				z->Data()->Set_Delete_Pending();
			}
		}
	}
}

bool Has_Beacon(ArmedGameObj *obj) {
	WeaponBagClass *Bag = obj->Get_Weapon_Bag();
	for (int i = 1;i < Bag->Get_Count();i++) {
		if (Bag->Peek_Weapon(i)->Get_Definition()->Style == STYLE_BEACON && Bag->Peek_Weapon(i)->Get_Total_Rounds()) {
			return true;
		}
	}
	return false;
}

float Get_Distance_To_Closest_Building(const Vector3 &Position,int Team) {
	float ClosestDist = FLT_MAX;
	for (SLNode<BuildingGameObj> *z = GameObjManager::BuildingGameObjList.Head();z;z = z->Next()) {
		if ((z->Data()->Get_Player_Type() == Team || Team == 2) && !z->Data()->Is_Destroyed()) {
			float Dist = 0.0f;
			z->Data()->Find_Closest_Poly(Position,&Dist);
			if (Dist < ClosestDist) {
				ClosestDist = Dist;
			}
		}
	}
	return ClosestDist;
}

float Get_Distance_To_Closest_Beacon(const Vector3 &Position,int Team) {
	float ClosestDist = FLT_MAX;
	for (SLNode<BeaconGameObj> *z = GameObjManager::BeaconGameObjList.Head();z;z = z->Next()) {
		if (z->Data()->Get_Player_Type() == Team || Team == 2) {
			float Dist = Commands->Get_Distance(Position,Commands->Get_Position(z->Data()));
			if (Dist < ClosestDist) {
				ClosestDist = Dist;
			}
		}
	}
	return ClosestDist;
}

float Get_Distance_To_Closest_PCT(const Vector3 &Position,int Team) {
	float ClosestDist = FLT_MAX;
	for (SLNode<BaseGameObj> *z = GameObjManager::GameObjList.Head();z;z = z->Next()) {
		if (z->Data()->As_PhysicalGameObj() && ((PhysicalGameObj*)z->Data())->As_SimpleGameObj()) {
			if ((Team == 0 && ((SimpleGameObj*)z->Data())->Get_Definition().Get_Player_Terminal_Type() == 1) || (Team == 1 && ((SimpleGameObj*)z->Data())->Get_Definition().Get_Player_Terminal_Type() == 0) || Team == 2) { //Stupid backwards teams.
				float Dist = Commands->Get_Distance(Position,((PhysicalGameObj*)z->Data())->Get_Position());
				if (Dist < ClosestDist) {
					ClosestDist = Dist;
				}
			}
		}
	}
	return ClosestDist;
}

BuildingGameObj *Get_Closest_Building(const Vector3 &Position,int Team) {
	float ClosestDist = FLT_MAX;
	BuildingGameObj *Closest = 0;
	for (SLNode<BuildingGameObj> *z = GameObjManager::BuildingGameObjList.Head();z;z = z->Next()) {
		if ((z->Data()->Get_Player_Type() == Team || Team == 2) && !z->Data()->Is_Destroyed()) {
			float Dist = 0.0f;
			z->Data()->Find_Closest_Poly(Position,&Dist);
			if (Dist < ClosestDist) {
				ClosestDist = Dist;
				Closest = z->Data();
			}
		}
	}
	return Closest;
}

PhysicalGameObj *Get_Closest_Fake_Building(const Vector3 &Position,int Team) {
	float ClosestDist = FLT_MAX;
	PhysicalGameObj *Closest = 0;
	for (SLNode<BaseGameObj> *z = GameObjManager::GameObjList.Head();z;z = z->Next()) {
		if (z->Data()->As_PhysicalGameObj()) {
			PhysicalGameObj *Phys = (PhysicalGameObj*)z->Data();
			if (Phys->Get_Definition().Get_Encyclopedia_Type() == 3 && (Phys->Get_Player_Type() == Team || Team == 2) && Phys->Get_Defense_Object()->Get_Health() && Phys->Peek_Physical_Object()->Peek_Model()) {
				float Dist = Commands->Get_Distance(Position,Phys->Get_Position());
				if (Dist < ClosestDist) {
					ClosestDist = Dist;
					Closest = Phys;
				}
			}
		}
	}
	return Closest;
}

BuildingGameObj *Get_Random_Building(int Team) {
	BaseControllerClass *Base = BaseControllerClass::Find_Base(Team);
	if (!Base || !Base->Get_Building_List().Count() || Base->Is_Base_Destroyed()) {
		return 0;
	}
	BuildingGameObj *Return = Base->Get_Building_List()[Get_Random_Int(0,Base->Get_Building_List().Count())];
	if (Return->Is_Destroyed()) {
		if (Get_Random_Bool()) {
			for (int i = 0;i < Base->Get_Building_List().Count();i++) {
				if (!Base->Get_Building_List()[i]->Is_Destroyed()) {
					return Base->Get_Building_List()[i];
				}
			}
		}
		else {
			for (int i = Base->Get_Building_List().Count()-1;i >= 0;i--) {
				if (!Base->Get_Building_List()[i]->Is_Destroyed()) {
					return Base->Get_Building_List()[i];
				}
			}
		}
		return 0;
	}
	return Return;
}

void Send_Custom_Event_Vehicle_Occupants(GameObject *obj,GameObject *Sender,int Message,int Param,float Delay,bool Driver) {
	VehicleGameObj *Vehicle = obj->As_VehicleGameObj();
	if (!Vehicle) {
		return;
	}

	int x = Vehicle->Get_Definition().Get_Seat_Count();
	for (int i = Driver?0:1;i < x;i++) {
		if (Vehicle->Get_Occupant(i)) {
			Commands->Send_Custom_Event(Sender,Vehicle->Get_Occupant(i),Message,Param,Delay);
		}
	}
}

void Disarm_Closest_Beacon(const Vector3 &Position,int Team) {
	float ClosestDist = FLT_MAX;
	BeaconGameObj *Closest = 0;
	for (SLNode<BeaconGameObj> *z = GameObjManager::BeaconGameObjList.Head();z;z = z->Next()) {
		if (z->Data()->Get_Player_Type() == Team || Team == 2) {
			float Dist = Commands->Get_Distance(Position,Commands->Get_Position(z->Data()));
			if (Dist < ClosestDist) {
				ClosestDist = Dist;
				Closest = z->Data();
			}
		}
	}
	if (Closest) {
		OffenseObjectClass offense(0);
		Closest->Completely_Damaged(offense);
	}
}

void Disarm_All_Beacons_Team(int Team) {
	for (SLNode<BeaconGameObj> *z = GameObjManager::BeaconGameObjList.Head();z;z = z->Next()) {
		if (z->Data()->Get_Player_Type() == Team || Team == 2) {
			OffenseObjectClass offense(0);
			z->Data()->Completely_Damaged(offense);
		}
	}
}

void Disarm_All_Beacons_Player(GameObject *Player) {
	for (SLNode<BeaconGameObj> *z = GameObjManager::BeaconGameObjList.Head();z;z = z->Next()) {
		if (z->Data()->Get_Owner() == Player) {
			OffenseObjectClass offense(0);
			z->Data()->Completely_Damaged(offense);
		}
	}
}

void Play_Animation_All_Players(const char *Anim) {
	for (SLNode<SoldierGameObj> *z = GameObjManager::StarGameObjList.Head();z;z = z->Next()) {
		Commands->Set_Animation(z->Data(),Anim,false,0,0.0f,-1.0f,false);
	}
}

StringClass Get_String_Player_Name(GameObject *obj) {
	if (!obj || !obj->As_SoldierGameObj()) {
		return "None";
	}
	if (!((SoldierGameObj*)obj)->Get_Player()) {
		return "None";
	}
	return ((SoldierGameObj*)obj)->Get_Player()->Get_Name().Peek_Buffer();
}

StringClass Get_String_Player_Name_By_ID(int ID) {
	cPlayer *x = Find_Player(ID);
	if (!x) {
		return "None";
	}
	return x->Get_Name().Peek_Buffer();
}

const wchar_t *Get_Wide_Player_Name_By_ID(int ID) {
	cPlayer *x = Find_Player(ID);
	if (!x) {
		return L"None";
	}
	return x->PlayerName;
}

StringClass Get_IP_Address_String(int PlayerID) {
	const sockaddr_in *addr = Get_IP(PlayerID);
	int ip1 = addr->sin_addr.s_addr&0x000000FF;
	int ip2 = (addr->sin_addr.s_addr&0x0000FF00)>>8;
	int ip3 = (addr->sin_addr.s_addr&0x00FF0000)>>16;
	int ip4 = (addr->sin_addr.s_addr&0xFF000000)>>24;
	return StringFormat("%d.%d.%d.%d",ip1,ip2,ip3,ip4);
}


int Find_Empty_Vehicle_Seat(VehicleGameObj *obj,bool AllowDriver) {
	int x = obj->Get_Definition().Get_Seat_Count();
	for (int i = (AllowDriver?0:1);i < x;i++) {
		if (!obj->Get_Occupant(i)) {
			return i;
		}
	}
	return -1;
}

ScriptImpClass *Get_Script_By_Name(GameObject *obj,const char *Script) {
	if (!obj->As_ScriptableGameObj()) {
		return 0;
	}
	const SimpleDynVecClass<GameObjObserverClass *> *observers = &obj->Get_Observers();
	for (int i = 0;i < observers->Count();i++) {
		if (!_stricmp((*observers)[i]->Get_Name(),Script)) {
			return (ScriptImpClass*)(*observers)[i];
		}
	}
	return 0;
}

void Change_Team_3(cPlayer *Player,int Team) {
	if (Player->Get_Money() > (float)The_Cnc_Game()->StartingCredits) { //Allow players to keep their starting credits when switching teams.
		Give_Credits_Team(Player->Get_Player_Type(),(Player->Get_Money()-(float)The_Cnc_Game()->StartingCredits)/(Get_Team_Player_Count(Player->Get_Player_Type())-1)); //Distribute anything above the starting credits to their teammates.
	}
	Player->Set_Money((float)The_Cnc_Game()->StartingCredits);
	Disarm_All_C4_Beacons(Player->Get_Id());
	Player->Set_Player_Type(Team);
	Player->Destroy_GameObj();
}

void Change_Team_4(cPlayer *Player,int Team) {
	Player->Set_Money((float)The_Cnc_Game()->StartingCredits);
	Disarm_All_C4_Beacons(Player->Get_Id());
	Player->Set_Player_Type(Team);
	Player->Destroy_GameObj();
}

void Change_Team_5(cPlayer *Player,int Team) {
	Player->Set_Score(0);
	Player->Set_Kills(0);
	Player->Set_Money((float)The_Cnc_Game()->StartingCredits);
	Disarm_All_C4_Beacons(Player->Get_Id());
	Player->Set_Player_Type(Team);
	Player->Destroy_GameObj();
	Player->Set_Deaths(0);
}

cPlayer *Match_Player(cPlayer *Player,const StringClass &Nick,bool TeamOnly,bool AllowSelf) {
	cPlayer *Return = 0;
	int Team = Player?Player->Get_Player_Type():2;
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		if (z->Data()->Is_Active() && (AllowSelf || Player != z->Data())) {
			if (!TeamOnly || Team == z->Data()->Get_Player_Type() || Team == 2) {
				if (!_stricmp(StringClass(z->Data()->Get_Name().Peek_Buffer()),Nick)) {
					return z->Data();
				}
				if (stristr(StringClass(z->Data()->Get_Name().Peek_Buffer()),Nick)) {
					if (Return) {
						if (Player) {
							DA::Page_Player(Player,"Multiple players were found matching \"%s\". Please supply a more unique wildcard.",Nick);
						}
						return 0;
					}
					else {
						Return = z->Data();
					}
				}
			}
		}
	}
	if (!Return) {
		if (Player) {
			DA::Page_Player(Player,"No players were found matching \"%s\".",Nick);
		}
		return 0;
	}
	return Return;
}

void Damage_Objects_Half_No_Kill() {
	for (SLNode<SmartGameObj> *z = GameObjManager::SmartGameObjList.Head();z;z = z->Next()) {
		DefenseObjectClass *Defense = z->Data()->Get_Defense_Object();
		float MaxHealth = Defense->Get_Health_Max() + Defense->Get_Shield_Strength_Max();
		float Health = Defense->Get_Health() + Defense->Get_Shield_Strength();
		if ((MaxHealth / 2) >= Health) {
			Commands->Apply_Damage(z->Data(),Health-1.0f,"None",0);
		}
		else {
			Commands->Apply_Damage(z->Data(),(MaxHealth / 2),"None",0);
		}
	}
}

void Attach_Script_To_Definition(const char *Preset,const char *Script,const char *Params) {
	ScriptableGameObjDef *Def = (ScriptableGameObjDef*)Find_Named_Definition(Preset);
	if (Def) {
		Def->ScriptNameList.Add(Script);
		Def->ScriptParameterList.Add(Params);
	}
}

void Attach_Script_To_Definitions(unsigned long CID,const char *Script,const char *Params) {
	for (ScriptableGameObjDef *Def = (ScriptableGameObjDef*)DefinitionMgrClass::Get_First(CID,DefinitionMgrClass::ID_CLASS);Def;Def = (ScriptableGameObjDef*)DefinitionMgrClass::Get_Next(Def,CID,DefinitionMgrClass::ID_CLASS)) {
		Def->ScriptNameList.Add(Script);
		Def->ScriptParameterList.Add(Params);
	}
}

void Remove_Script_From_Definition(const char *Preset, const char *Script) {
	ScriptableGameObjDef *Def = (ScriptableGameObjDef*)Find_Named_Definition(Preset);
	if (Def) {
		for (int i = Def->ScriptNameList.Count() - 1; i >= 0; i--) {
			if (!_stricmp(Def->ScriptNameList[i], Script)) {
				Def->ScriptNameList.Delete(i);
				Def->ScriptParameterList.Delete(i);
			}
		}
	}
}

void Remove_Script_From_Definitions(unsigned long CID, const char *Script) {
	for (ScriptableGameObjDef *Def = (ScriptableGameObjDef*)DefinitionMgrClass::Get_First(CID, DefinitionMgrClass::ID_CLASS); Def; Def = (ScriptableGameObjDef*)DefinitionMgrClass::Get_Next(Def, CID, DefinitionMgrClass::ID_CLASS)) {
		for (int i = Def->ScriptNameList.Count() - 1; i >= 0; i--) {
			if (!_stricmp(Def->ScriptNameList[i], Script)) {
				Def->ScriptNameList.Delete(i);
				Def->ScriptParameterList.Delete(i);
			}
		}
	}
}


void TDBObjClass::Set_English_String (const char *string) {
	EnglishString = string;
}

void Set_Object_Type_Preset(const char *Preset,int Team) {
	for (SLNode<BaseGameObj> *z = GameObjManager::GameObjList.Head();z;z = z->Next()) {
		if (!_stricmp(Commands->Get_Preset_Name(z->Data()->As_ScriptableGameObj()),Preset)) {
			Set_Object_Type(z->Data()->As_ScriptableGameObj(),Team);
		}
	}
} 

GameObject *Get_Closest_Vehicle(GameObject *obj,float MaxDistance) {
	if (!obj || !Commands->Get_ID(obj)) {
		return 0;
	}
	GameObject *Closest = 0;
	float ClosestDistance = 99999.0f;
	Vector3 Pos = Commands->Get_Position(obj);
	for (SLNode<SmartGameObj> *z = GameObjManager::SmartGameObjList.Head();z;z = z->Next()) {
		GameObject *o = z->Data();
		if (o && o->As_VehicleGameObj()) {
			float Distance = Commands->Get_Distance(Pos,Commands->Get_Position(o));
			if (Distance < ClosestDistance) {
				ClosestDistance = Distance;
				Closest = o;
			}
		}
	}
	if (ClosestDistance > MaxDistance) {
		return 0;
	}
	return Closest;
}

float Get_Distance_To_Closest_Player(const Vector3 &Position,int Team) {
	float Return = 9999.0f;
	for (SLNode<SoldierGameObj> *z = GameObjManager::StarGameObjList.Head();z;z = z->Next()) {
		float Dis = Commands->Get_Distance(Commands->Get_Position(z->Data()),Position);
		if (Dis < Return) {
			Return = Dis;
		}
	}
	return Return;
} 

void Give_Credits_Team(int Team,float Credits) {
	for (SLNode<SoldierGameObj> *z = GameObjManager::StarGameObjList.Head();z;z = z->Next()) {
		SoldierGameObj *Soldier = z->Data();
		if (Soldier->Get_Player_Type() == Team) {
			Soldier->Get_Player()->Increment_Money(Credits);
		}
	}
}

const char *PhysClass::Get_Name() {
	return Name;
}

void Give_Points_Players_In_Range(const Vector3 &Position,float Range,float Points,bool GiveMoney) {
	for (SLNode<SoldierGameObj> *z = GameObjManager::StarGameObjList.Head();z;z = z->Next()) {
		SoldierGameObj *Soldier = z->Data();
		if (Commands->Get_Distance(Soldier->Get_Position(),Position) <= Range) {
			Soldier->Get_Player()->Increment_Score(Points);
			if (!GiveMoney) {
				Soldier->Get_Player()->Increment_Money(Points * DAPlayerManager::Get_Credits_Multiplier() * -1.0f);
			}
		}
	}
}

void Give_Points_Players_In_Range_Team(int Team,const Vector3 &Position,float Range,float Points,bool GiveMoney) {
	for (SLNode<SoldierGameObj> *z = GameObjManager::StarGameObjList.Head();z;z = z->Next()) {
		SoldierGameObj *Soldier = z->Data();
		if (Soldier->Get_Player_Type() == Team && Commands->Get_Distance(Soldier->Get_Position(),Position) <= Range) {
			Soldier->Get_Player()->Increment_Score(Points);
			if (!GiveMoney) {
				Soldier->Get_Player()->Increment_Money(Points * DAPlayerManager::Get_Credits_Multiplier() * -1.0f);
			}
		}
	}
}

StringClass Encrypt_Apgar(const char *Password) {
	int Len = strlen(Password);
	char Lookup[70] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";
	StringClass Return;
	for (int i = 0;i < Len;i++) {
		char Left = Password[i];
		char Right = ((i > 0) ? Password[Len - i] : 0);
		char x = ((Left & 1) > 0 ? (Left << 1) & Right : Left ^ Right);
		Return += Lookup[x & 63]; 
	}
	return Return;
}

void Set_Facing_Player(GameObject *obj,float Facing) {
	float Angle = Facing*WWMATH_PI/180.0f;
	Vector3 Position = Commands->Get_Position(obj);
	Position.X += cos(Angle)*2.0f;
	Position.Y += sin(Angle)*2.0f;
	Position.Z += 4.0f;
	Force_Camera_Look_Player(obj,Position);
}

bool Overlap_Test(GameObject *obj,const Vector3 &Position,bool CheckDynOnly) {
	if (!obj || !obj->As_PhysicalGameObj()) {
		return false;
	}
	MoveablePhysClass *Phys = obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass();
	if (!Phys || (!Phys->As_RigidBodyClass() && !Phys->As_Phys3Class())) {
		return false;
	}
	Matrix3D Transform = Phys->Get_Transform();
	Transform.Set_Translation(Position);
	return !Phys->Can_Teleport(Transform,CheckDynOnly);
}

void Console_InputF(const char *Format,...) {
	va_list arg_list;
	va_start(arg_list,Format);
	char Buffer[256];
	vsnprintf(Buffer,256,Format,arg_list);
	Console_Input(Buffer);
	va_end(arg_list);
}

bool Is_Stealth_Unit(GameObject *obj) {
	if (!obj || !obj->As_SmartGameObj()) {
		return false;
	}
	return obj->As_SmartGameObj()->Get_Definition().Is_Stealthed();
}

void Set_Object_Dirty_Bit_For_Version_Greater_Than(NetworkObjectClass *obj,float Version,NetworkObjectClass::DIRTY_BIT Bit,bool OnOff) {
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		if (z->Data()->Get_DA_Player()->Get_Version() >= Version) {
			obj->Set_Object_Dirty_Bit(z->Data()->Get_Id(),Bit,OnOff);
		}
	}
}

void Set_Object_Dirty_Bit_For_Version_Less_Than(NetworkObjectClass *obj,float Version,NetworkObjectClass::DIRTY_BIT Bit,bool OnOff) {
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		if (z->Data()->Get_DA_Player()->Get_Version() < Version) {
			obj->Set_Object_Dirty_Bit(z->Data()->Get_Id(),Bit,OnOff);
		}
	}
}

void Set_Object_Dirty_Bit_For_Team_Version_Greater_Than(NetworkObjectClass *obj,int Team,float Version,NetworkObjectClass::DIRTY_BIT Bit,bool OnOff) {
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		if (z->Data()->Get_DA_Player()->Get_Version() >= Version && z->Data()->Get_Player_Type() == Team) {
			obj->Set_Object_Dirty_Bit(z->Data()->Get_Id(),Bit,OnOff);
		}
	}
}

void Set_Object_Dirty_Bit_For_Team_Version_Less_Than(NetworkObjectClass *obj,int Team,float Version,NetworkObjectClass::DIRTY_BIT Bit,bool OnOff) {
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		if (z->Data()->Get_DA_Player()->Get_Version() < Version && z->Data()->Get_Player_Type() == Team) {
			obj->Set_Object_Dirty_Bit(z->Data()->Get_Id(),Bit,OnOff);
		}
	}
}

static WideStringClass NickSave;
static cPlayer *NickSavePlayer = 0;

int Setup_Send_Message_Fake(const char *NewNick,int ID) {
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		cPlayer *Player = z->Data();
		if (Player->Is_Active()) {
			NickSave = Player->Get_Name();
			NickSavePlayer = Player;
			Player->PlayerName = NewNick;
			if (ID) {
				Player->Set_Object_Dirty_Bit(ID,NetworkObjectClass::BIT_CREATION,true);
				//Send_Object_Update(Player,ID);
				Update_Network_Object(Player);
			}
			else {
				//Player->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_CREATION,true);
				Set_Object_Dirty_Bit_For_Version_Less_Than(Player,2.6f,NetworkObjectClass::BIT_CREATION,true);
				Update_Network_Object(Player);
			}
			return Player->Get_Id();
		}
	}
	return 0;
}

int Setup_Send_Message_Team_Fake(const char *NewNick,int Team,int ID) {
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		cPlayer *Player = z->Data();
		if (Player->Is_Active() && Player->Get_Player_Type() == Team) {
			NickSave = Player->Get_Name();
			NickSavePlayer = Player;
			Player->PlayerName = NewNick;
			if (ID) {
				Player->Set_Object_Dirty_Bit(ID,NetworkObjectClass::BIT_CREATION,true);
				//Send_Object_Update(Player,ID);
				Update_Network_Object(Player);
			}
			else {
				//Player->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_CREATION,true);
				Set_Object_Dirty_Bit_For_Version_Less_Than(Player,2.6f,NetworkObjectClass::BIT_CREATION,true);
				Update_Network_Object(Player);
			}
			return Player->Get_Id();
		}
	}
	return 0;
}

void Restore_Send_Message_Fake() {
	if (NickSavePlayer) {
		NickSavePlayer->PlayerName = NickSave;
		//NickSavePlayer->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_CREATION,true);
		Set_Object_Dirty_Bit_For_Version_Less_Than(NickSavePlayer,2.6f,NetworkObjectClass::BIT_CREATION,true);
		Update_Network_Object(NickSavePlayer);
		DAEventManager::Name_Change_Event(NickSavePlayer);
		NickSavePlayer = 0;
	}
}

void Send_Announcement(const char *StringID) {
	TDBObjClass *TDBO = TranslateDBClass::Find_Object(StringID);
	if (TDBO) {
		Send_Client_Announcement(0,-1,TDBO->Get_ID(),ANNOUNCE_PUBLIC,0,true,true);
	}
}

void Send_Announcement_Version_Less_Than(const char *StringID,float Version) {
	TDBObjClass *TDBO = TranslateDBClass::Find_Object(StringID);
	if (TDBO) {
		SCAnnouncement *RadioEvent = Send_Client_Announcement(0,-1,TDBO->Get_ID(),ANNOUNCE_PUBLIC,0,false,false);
		for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
			cPlayer *Player = z->Data();
			if (Player->Is_Active() && Player->Get_DA_Player()->Get_Version() < Version) {
				RadioEvent->Set_Object_Dirty_Bits(Player->Get_Id(),NetworkObjectClass::BIT_CREATION);
			}
		}
	}
}

void Send_Announcement_Player(int ID,const char *StringID) {
	TDBObjClass *TDBO = TranslateDBClass::Find_Object(StringID);
	if (TDBO) {
		SCAnnouncement *RadioEvent = Send_Client_Announcement(0,-1,TDBO->Get_ID(),ANNOUNCE_PUBLIC,0,false,false);
		RadioEvent->Set_Object_Dirty_Bits(ID,NetworkObjectClass::BIT_CREATION);
	}
}

void Send_Announcement_Player_Version_Less_Than(int ID,const char *StringID,float Version) {
	if (Get_Client_Version(ID) < Version) {
		TDBObjClass *TDBO = TranslateDBClass::Find_Object(StringID);
		if (TDBO) {
			SCAnnouncement *RadioEvent = Send_Client_Announcement(0,-1,TDBO->Get_ID(),ANNOUNCE_PUBLIC,0,false,false);
			RadioEvent->Set_Object_Dirty_Bits(ID,NetworkObjectClass::BIT_CREATION);
		}
	}
}

void Send_Announcement_Team(int Team,const char *StringID) {
	TDBObjClass *TDBO = TranslateDBClass::Find_Object(StringID);
	if (TDBO) {
		SCAnnouncement *RadioEvent = Send_Client_Announcement(0,-1,TDBO->Get_ID(),ANNOUNCE_PUBLIC,0,false,false);
		for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
			cPlayer *Player = z->Data();
			if (Player->Is_Active() && Player->Get_Player_Type() == Team) {
				RadioEvent->Set_Object_Dirty_Bits(Player->Get_Id(),NetworkObjectClass::BIT_CREATION);
			}
		}
	}
}

void Send_Announcement_Team_Version_Less_Than(int Team,const char *StringID,float Version) {
	TDBObjClass *TDBO = TranslateDBClass::Find_Object(StringID);
	if (TDBO) {
		SCAnnouncement *RadioEvent = Send_Client_Announcement(0,-1,TDBO->Get_ID(),ANNOUNCE_PUBLIC,0,false,false);
		for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
			cPlayer *Player = z->Data();
			if (Player->Is_Active() && Player->Get_Player_Type() == Team && Player->Get_DA_Player()->Get_Version() < Version) {
				RadioEvent->Set_Object_Dirty_Bits(Player->Get_Id(),NetworkObjectClass::BIT_CREATION);
			}
		}
	}
}

void Send_Message_Team_With_Team_Color(int Team,const char *Msg) {
	unsigned int Red = 0,Blue = 0,Green = 0;
	Get_Team_Color(Team,&Red,&Green,&Blue);
	Send_Message_Team(Team,Red,Green,Blue,Msg);
}

void Send_Message_Player_By_ID(int ID,unsigned int Red,int unsigned Green,int unsigned Blue,const char *Message) {
	WideStringClass Send;
	Send.Format(L"j\n44\n%u\n%u\n%u\n%hs\n,",Red,Green,Blue,Message);
	Send_Client_Text(Send,TEXT_MESSAGE_PRIVATE,false,-2,ID,true,true);
}

void Create_2D_WAV_Sound_Player_By_ID(int ID,const char *Sound) {
	WideStringClass Send;
	Send.Format(L"j\n10\n%hs\n,",Sound);
	Send_Client_Text(Send,TEXT_MESSAGE_PRIVATE,false,-2,ID,true,true);
}

PhysicalGameObj *Create_Object(const char *Preset,const Vector3 &Position) {
	DefinitionClass *Def = Find_Named_Definition(Preset);
	if (Def) {
		return Create_Object(Def,Position);
	}
	return 0;
}

PhysicalGameObj *Create_Object(const char *Preset,const Matrix3D &Transform) {
	DefinitionClass *Def = Find_Named_Definition(Preset);
	if (Def) {
		return Create_Object(Def,Transform);
	}
	return 0;
}

PhysicalGameObj *Create_Object(int Preset,const Vector3 &Position) {
	DefinitionClass *Def = Find_Definition(Preset);
	if (Def) {
		return Create_Object(Def,Position);
	}
	return 0;
}

PhysicalGameObj *Create_Object(int Preset,const Matrix3D &Transform) {
	DefinitionClass *Def = Find_Definition(Preset);
	if (Def) {
		return Create_Object(Def,Transform);
	}
	return 0;
}

PhysicalGameObj *Create_Object(const DefinitionClass *Def,const Vector3 &Position) {
	PhysicalGameObj *obj = (PhysicalGameObj*)Def->Create();
	if (obj->As_PhysicalGameObj()) {
		obj->Set_Position(Position);
		obj->Start_Observers();
		return obj;
	}
	else {
		obj->Set_Delete_Pending();
		return 0;
	}
}

PhysicalGameObj *Create_Object(const DefinitionClass *Def,const Matrix3D &Transform) {
	PhysicalGameObj *obj = (PhysicalGameObj*)Def->Create();
	if (obj->As_PhysicalGameObj()) {
		obj->Set_Transform(Transform);
		obj->Start_Observers();
		return obj;
	}
	else {
		obj->Set_Delete_Pending();
		return 0;
	}
}

SoldierGameObj *Create_Commando(cPlayer *Player,const char *Preset,const Vector3 &Position) {
	DefinitionClass *Def = Find_Named_Definition(Preset);
	if (Def) {
		return Create_Commando(Player,Def,Position);
	}
	return 0;
}

SoldierGameObj *Create_Commando(cPlayer *Player,const char *Preset,const Matrix3D &Transform) {
	DefinitionClass *Def = Find_Named_Definition(Preset);
	if (Def) {
		return Create_Commando(Player,Def,Transform);
	}
	return 0;
}

SoldierGameObj *Create_Commando(cPlayer *Player,int Preset,const Vector3 &Position) {
	DefinitionClass *Def = Find_Definition(Preset);
	if (Def) {
		return Create_Commando(Player,Def,Position);
	}
	return 0;
}

SoldierGameObj *Create_Commando(cPlayer *Player,int Preset,const Matrix3D &Transform) {
	DefinitionClass *Def = Find_Definition(Preset);
	if (Def) {
		return Create_Commando(Player,Def,Transform);
	}
	return 0;
}

SoldierGameObj *Create_Commando(cPlayer *Player,const DefinitionClass *Def,const Vector3 &Position) {
	if (!Def || Def->Get_Class_ID() != CID_Soldier) {
		return 0;
	}
	SoldierGameObj *obj = (SoldierGameObj*)Def->Create();
	Player->Owner = obj;
	obj->Set_Control_Owner(Player->Get_Id());
	obj->Set_Player_Data(Player);
	obj->Set_Player_Type(Player->Get_Player_Type());
	obj->Set_Position(Position);
	obj->Start_Observers();
	return obj;
}

SoldierGameObj *Create_Commando(cPlayer *Player,const DefinitionClass *Def,const Matrix3D &Transform) {
	if (!Def || Def->Get_Class_ID() != CID_Soldier) {
		return 0;
	}
	SoldierGameObj *obj = (SoldierGameObj*)Def->Create();
	Player->Owner = obj;
	obj->Set_Control_Owner(Player->Get_Id());
	obj->Set_Player_Data(Player);
	obj->Set_Player_Type(Player->Get_Player_Type());
	obj->Set_Transform(Transform);
	obj->Start_Observers();
	return obj;
}

//This function should only be called from a pre level loaded event. Calling it anywhere else will cause clients to crash.
void Disable_Building_Type(int Team,BuildingConstants::BuildingType Type) {
	BaseControllerClass *Base = BaseControllerClass::Find_Base(Team);
	for (int i = Base->Get_Building_List().Count()-1;i >= 0;i--) {
		if (Base->Get_Building_List()[i]->Get_Definition().Get_Type() == Type) {
			Base->Get_Building_List()[i]->Set_Delete_Pending();
			Base->Get_Building_List()[i]->Clear_Object_Dirty_Bits();
			Base->Get_Building_List().Delete(i);
		}
	}
}

unsigned int Get_Hash(const char *String) {
	int Len = strlen(String);
	if (Len >= 8) {
		return HashTemplateKeyClass<uint>::Get_Hash_Value((uint&)String[Len - 8]);
	}
	else {
		unsigned int Hash = 0;
		for (int i = 0; i < Len; i++) {
			Hash += String[i] + Hash * 37;
		}
		return Hash;
	}
}

bool Is_Starting_Weapon(const SoldierGameObjDef *Soldier,const PowerUpGameObjDef *PowerUp) {
	if (!PowerUp || !Soldier) {
		return false;
	}
	if (Soldier->WeaponDefID == PowerUp->GrantWeaponID || Soldier->SecondaryWeaponDefID == PowerUp->GrantWeaponID) {
		return true;
	}
	for (int i = 0;i < Soldier->ScriptNameList.Count();i++) {
		if (Soldier->ScriptNameList[i] == "M00_GrantPowerup_Created") {
			if (Soldier->ScriptParameterList[i] == PowerUp->Get_Name()) {
				return true;
			}
		}
	}
	return false;
}

bool Is_Starting_Weapon(SoldierGameObj *Soldier,const PowerUpGameObjDef *PowerUp) {
	if (!Soldier) {
		return false;
	}
	return Is_Starting_Weapon(&Soldier->Get_Definition(),PowerUp);
}

void WeaponBagClass::Remove_Weapon(const WeaponDefinitionClass *Weapon) {
	 int Pos = Get_Weapon_Position(Weapon);
	 if (Pos) {
		Remove_Weapon(Pos);
	}
}

void WeaponBagClass::Remove_Weapon(const char *Weapon) {
	int Pos = Get_Weapon_Position(Weapon);
	 if (Pos) {
		Remove_Weapon(Pos);
	}
}

void WeaponBagClass::Remove_Weapon(unsigned int Weapon) {
	int Pos = Get_Weapon_Position(Weapon);
	 if (Pos) {
		Remove_Weapon(Pos);
	}
}

WeaponClass *WeaponBagClass::Add_Weapon(const char *Weapon,int Rounds,bool Owned) {
	WeaponDefinitionClass *Def = (WeaponDefinitionClass*)DefinitionMgrClass::Find_Named_Definition(Weapon,true);
	if (Def && Def->Get_Class_ID() == CID_Weapon) {
		return Add_Weapon(Def,Rounds,Owned);
	}
	return 0;
}

WeaponClass *WeaponBagClass::Add_Weapon(int Weapon,int Rounds,bool Owned) {
	WeaponDefinitionClass *Def = (WeaponDefinitionClass*)DefinitionMgrClass::Find_Definition(Weapon,true);
	if (Def && Def->Get_Class_ID() == CID_Weapon) {
		return Add_Weapon(Def,Rounds,Owned);
	}
	return 0;
}

void WeaponBagClass::Select_Weapon_Name(const char *Weapon) {
   int Pos = Get_Weapon_Position(Weapon);
   if (Pos) {
	   Select_Index(Pos);
   }
}

void WeaponBagClass::Select_Weapon_ID(int ID) {
   int Pos = Get_Weapon_Position(ID);
   if (Pos) {
	   Select_Index(Pos);
   }
}

void WeaponBagClass::Select_Weapon(WeaponClass *Weapon) {
   int Pos = Get_Weapon_Position(Weapon);
   if (Pos) {
	   Select_Index(Pos);
   }
}

void WeaponBagClass::Deselect() {
   Select_Index(0);
}

WeaponClass *WeaponBagClass::Find_Weapon(const WeaponDefinitionClass *WeaponDef) {
	for (int x = 1;x < WeaponList.Count();x++) {
		if (WeaponList[x]->Get_Definition() == WeaponDef) {
			return WeaponList[x];
		}
	}
	return 0;
}

WeaponClass *WeaponBagClass::Find_Weapon(const char *Weapon) {
	for (int x = 1;x < WeaponList.Count();x++) {
		if (!_stricmp(WeaponList[x]->Get_Definition()->Get_Name(),Weapon)) {
			return WeaponList[x];
		}
	}
	return 0;
}

WeaponClass *WeaponBagClass::Find_Weapon(unsigned int WeaponDefID) {
	for (int x = 1;x < WeaponList.Count();x++) {
		if (WeaponList[x]->Get_Definition()->Get_ID() == WeaponDefID) {
			return WeaponList[x];
		}
	}
	return 0;
}

int WeaponBagClass::Get_Weapon_Position(const WeaponDefinitionClass *WeaponDef) {
	for (int x = 1;x < WeaponList.Count();x++) {
		if (WeaponList[x]->Get_Definition() == WeaponDef) {
			return x;
		}
	}
	return 0;
}

int WeaponBagClass::Get_Weapon_Position(const char *Weapon) {
	for (int x = 1;x < WeaponList.Count();x++) {
		if (!_stricmp(WeaponList[x]->Get_Definition()->Get_Name(),Weapon)) {
			return x;
		}
	}
	return 0;
}

int WeaponBagClass::Get_Weapon_Position(unsigned int WeaponDefID) {
	for (int x = 1;x < WeaponList.Count();x++) {
		if (WeaponList[x]->Get_Definition()->Get_ID() == WeaponDefID) {
			return x;
		}
	}
	return 0;
}

int WeaponBagClass::Get_Weapon_Position(WeaponClass *Weapon) {
	for (int x = 1;x < WeaponList.Count();x++) {
		if (WeaponList[x] == Weapon) {
			return x;
		}
	}
	return 0;
}

void Fix_Stuck_Objects(const Vector3 &Position,float CheckRange,float Range,bool DestroyUnfixable) {
	for (SLNode<SmartGameObj> *x = GameObjManager::SmartGameObjList.Head();x;x = x->Next()) {
		PhysicalGameObj *obj = x->Data();
		if (obj->Peek_Physical_Object()->As_MoveablePhysClass() && Commands->Get_Distance(Position,obj->Get_Position()) <= CheckRange && !((MoveablePhysClass*)obj->Peek_Physical_Object())->Can_Teleport(obj->Get_Transform()) && !obj->Is_Attached_To_An_Object() && (!obj->As_SoldierGameObj() || !((SoldierGameObj*)obj)->Get_Vehicle())) {
			if (!Fix_Stuck_Object(obj,Range) && DestroyUnfixable) {
				Commands->Apply_Damage(obj,99999.0f,"None",0);
			}
		}
	}
}

bool Fix_Stuck_Object(PhysicalGameObj *obj,float Range) {
	MoveablePhysClass *Phys = (MoveablePhysClass*)obj->Peek_Physical_Object();
	Collision_Group_Type CollisionSave = Phys->Get_Collision_Group();
	float MinDistance = FLT_MAX;
	Matrix3D MinTransform;
	Matrix3D OldTransform = Phys->Get_Transform();
	for (int i = 0;i < 50;i++) { //First try to find a new position without changing the Z axis.
		Matrix3D NewTransform = OldTransform;
		NewTransform.Set_X_Translation(OldTransform.Get_Translation().X+Get_Random_Float(Range*-1.0f,Range));
		NewTransform.Set_Y_Translation(OldTransform.Get_Translation().Y+Get_Random_Float(Range*-1.0f,Range));
		float Distance = Commands->Get_Distance(NewTransform.Get_Translation(),OldTransform.Get_Translation());
		if (Phys->Can_Teleport(NewTransform)) { //Don't use this position if it collides with something.
			if (Distance < MinDistance) { //Use closest positon.
				Phys->Set_Collision_Group(TERRAIN_COLLISION_GROUP); //Need to change the collision group so the ray can collide with the object.
				CastResultStruct CastResult;
				PhysRayCollisionTestClass CollisionTest(LineSegClass(NewTransform.Get_Translation(),OldTransform.Get_Translation()),&CastResult,TERRAIN_ONLY_COLLISION_GROUP); //We only want to collide with the object and terrain, not other objects.
				PhysicsSceneClass::Get_Instance()->Cast_Ray(CollisionTest,false);
				Phys->Set_Collision_Group(CollisionSave); //Restore old collision group.
				if (CollisionTest.CollidedPhysObj == Phys) { //Only use this position if it can see the original position. Prevents teleporting through walls.
					MinDistance = Distance;
					MinTransform = NewTransform;
				}
			}
		}
	}
	if (MinDistance != FLT_MAX) {
		obj->Set_Transform(MinTransform);
		return true;
	}
	for (int i = 0;i < 50;i++) { //Shit's getting serious. Start changing the Z axis too.
		Matrix3D NewTransform = OldTransform;
		NewTransform.Set_X_Translation(OldTransform.Get_Translation().X+Get_Random_Float(Range*-1.0f,Range));
		NewTransform.Set_Y_Translation(OldTransform.Get_Translation().Y+Get_Random_Float(Range*-1.0f,Range));
		NewTransform.Set_Z_Translation(OldTransform.Get_Translation().Z+Get_Random_Float(0.0f,Range));
		float Distance = Commands->Get_Distance(NewTransform.Get_Translation(),OldTransform.Get_Translation());
		if (Phys->Can_Teleport(NewTransform)) { //Don't use this position if it collides with something.
			if (Distance < MinDistance) { //Use closest positon.
				Phys->Set_Collision_Group(TERRAIN_COLLISION_GROUP); //Need to change the collision group so the ray can collide with the object.
				CastResultStruct CastResult;
				PhysRayCollisionTestClass CollisionTest(LineSegClass(NewTransform.Get_Translation(),OldTransform.Get_Translation()),&CastResult,TERRAIN_ONLY_COLLISION_GROUP); //We only want to collide with the object and terrain, not other objects.
				PhysicsSceneClass::Get_Instance()->Cast_Ray(CollisionTest,false);
				Phys->Set_Collision_Group(CollisionSave); //Restore old collision group.
				if (CollisionTest.CollidedPhysObj == Phys) { //Only use this position if it can see the original position. Prevents teleporting through walls.
					MinDistance = Distance;
					MinTransform = NewTransform;
				}
			}
		}
	}
	if (MinDistance == FLT_MAX) { //Couldn't find viable new position.
		return false;
	}
	else {
		obj->Set_Transform(MinTransform);
		return true;
	}
}

StringClass Clean_Model_Name(StringClass Model) {
	Model.cropTo(Model.Get_Length()-4);
	for (int i = Model.Get_Length();i >= 0;i--) {
		if (Model[i] == '\\') {
			Model.cropFrom(i+1);
			break;
		}
	}
	return Model;
}

StringClass Get_Weapon_PowerUp_Model(const WeaponDefinitionClass *Weapon) {
	StringClass Model = Clean_Model_Name(Weapon->Model);
	if (!Model.Is_Empty()) {
		Model[0] = 'p';
	}
	return Model;
}

void Add_Console_Function(ConsoleFunctionClass *Func) {
	if (Func->Get_Name()) {
		Delete_Console_Function(Func->Get_Name());
	}
	if (Func->Get_Alias()) {
		Delete_Console_Function(Func->Get_Alias());
	}
	ConsoleFunctionList.Add(Func);
	Sort_Function_List();
	Verbose_Help_File();
}

int Get_Building_Count(int Team,bool Destroyed) {
	int Return = 0;
	BaseControllerClass *Base = BaseControllerClass::Find_Base(Team);
	if (Base) {
		for (int i = 0;i < Base->Get_Building_List().Count();i++) {
			if (Base->Get_Building_List()[i]->Is_Destroyed() == Destroyed) {
				Return++;
			}
		}
	}
	return Return;
}

void Enable_HUD(bool Enable) {
	WideStringClass Send;
	Send.Format(L"j\n29\n%d\n",Enable);
	Send_Client_Text(Send,TEXT_MESSAGE_PUBLIC,false,-2,-1,true,true);
}

void Enable_HUD_Player_By_ID(int ID,bool Enable) {
	WideStringClass Send;
	Send.Format(L"j\n29\n%d\n",Enable);
	Send_Client_Text(Send,TEXT_MESSAGE_PRIVATE,false,-2,ID,true,true);
}

void Set_Fog_Enable_Player_By_ID(int ID,bool Enable) {
	WideStringClass Send;
	Send.Format(L"j\n2\n%d\n",Enable);
	Send_Client_Text(Send,TEXT_MESSAGE_PRIVATE,false,-2,ID,true,true);
}

void Set_Fog_Range_Player_By_ID(int ID,float StartDistance,float EndDistance,float Transition) {
	WideStringClass Send;
	Send.Format(L"j\n3\n%f\n%f\n%f\n",StartDistance,EndDistance,Transition);
	Send_Client_Text(Send,TEXT_MESSAGE_PRIVATE,false,-2,ID,true,true);
}

bool Exit_Vehicle(SoldierGameObj *Soldier) {
	if (Is_Player(Soldier) && Soldier->Get_Vehicle()) {
		VehicleGameObj *Vehicle = Soldier->Get_Vehicle();
		Soldier->Get_Control().Set_Action_Trigger(true);
		Soldier->Apply_Control();
		Update_Network_Object(Vehicle);
		Update_Network_Object(Soldier);
		return (!Soldier->Get_Vehicle() && Vehicle->Find_Seat(Soldier) == -1);
	}
	return false;
}

void ScriptableGameObj::Remove_Observer(const char *Name) {
	for (int i = Observers.Count()-1;i >= 0;i--) {
		if (!_stricmp(Observers[i]->Get_Name(),Name)) {
			Observers[i]->Detach(this);
			Observers.Delete(i,true);
		}
	}
}

void DefenseObjectClass::Set_Health(float health) {
	if (health > HealthMax) {
		Health = HealthMax;
	}
	else {
		Health = health;
	}
	Mark_Owner_Dirty();
}

void DefenseObjectClass::Add_Health(float add_health) {
	Health += add_health;
	if (Health > HealthMax) {
		Health = HealthMax;
	}
	Mark_Owner_Dirty();
}

void DefenseObjectClass::Set_Shield_Strength(float str) {
	if (str > ShieldStrengthMax) {
		ShieldStrength = ShieldStrengthMax;
	}
	else {
		ShieldStrength = str;
	}
	Mark_Owner_Dirty();
}

void DefenseObjectClass::Add_Shield_Strength(float str) {
	ShieldStrength += str;
	if (ShieldStrength > ShieldStrengthMax) {
		ShieldStrength = ShieldStrengthMax;
	}
	Mark_Owner_Dirty();
}

int VehicleGameObjDef::Get_Damage_Report(int Team) const {
	if (Team == 0) {
		return NodDamageReportID;
	}
	else if (Team == 1) {
		return GDIDamageReportID;
	}
	return 0;
}

int VehicleGameObjDef::Get_Destroy_Report(int Team) const {
	if (Team == 0) {
		return NodDestroyReportID;
	}
	else if (Team == 1) {
		return GDIDestroyReportID;
	}
	return 0;
}

int BuildingGameObjDef::Get_Damage_Report(int Team) const {
	if (Team == 0) {
		return NodDamageReportID;
	}
	else if (Team == 1) {
		return GDIDamageReportID;
	}
	return 0;
}

int BuildingGameObjDef::Get_Destroy_Report(int Team) const {
	if (Team == 0) {
		return NodDestroyReportID;
	}
	else if (Team == 1) {
		return GDIDestroyReportID;
	}
	return 0;
}


void WeaponClass::Set_Clip_Rounds(int Rounds) {
	ClipRounds = Rounds;
}

void WeaponClass::Set_Inventory_Rounds(int Rounds) {
	InventoryRounds = Rounds;
}

void cGameData::Set_Time_Limit_Minutes(int Minutes) {
	TimeLimit_Minutes = Minutes;
}

void SmartGameObj::Set_Player_Data(PlayerDataClass *Data) {
	PlayerData = Data;
}

uint DA_API Send_Object_Update(NetworkObjectClass *Object, int ID) {
	Update_Network_Object_Player(Object,ID);
	return 0;
}

void DA_API Update_Game_Settings(int ID) {
	Update_Game_Options(ID);
}

void Reverse_Damage(GameObject *obj,float Amount) {
	float Health = Commands->Get_Health(obj);
	float Armor = Commands->Get_Shield_Strength(obj);
	if (Health != Commands->Get_Max_Health(obj)) {
		Health += Amount;
		float OverFlow = Health-Commands->Get_Max_Health(obj);
		if (OverFlow > 0) {
			Armor += OverFlow;
			Commands->Set_Shield_Strength(obj,Armor);
		}
		Commands->Set_Health(obj,Health);
	}
	else {
		Commands->Set_Shield_Strength(obj,Armor+Amount);
	}
}

void cGameData::Set_Intermission_Time_Seconds(int time) {
	IntermissionTime_Seconds = time;
}

void Set_Emote_Icon(int ID,const char *Model,int Team) {
	WideStringClass Send;
	Send.Format(L"j\n36\n%d\n%hs\n",ID,Model);
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		if (z->Data()->Is_Active() && (Team == 2 || Team == z->Data()->Get_Player_Type())) {
			Send_Client_Text(Send,TEXT_MESSAGE_PRIVATE,false,-2,z->Data()->Get_Id(),true,true);
		}
	}
}

unsigned int Get_Ground_Vehicle_Count(int Team) {
	GameObject *factory = Find_Vehicle_Factory(Team);
	if (factory && factory->As_BuildingGameObj() && factory->As_BuildingGameObj()->As_VehicleFactoryGameObj())
	{
		return factory->As_BuildingGameObj()->As_VehicleFactoryGameObj()->Get_Team_Vehicle_Count();
	}
	return 0;
};
unsigned int Get_Air_Vehicle_Count(int Team) {
	GameObject *factory = Find_Helipad(Team);
	if (factory && factory->As_BuildingGameObj() && factory->As_BuildingGameObj()->As_AirFactoryGameObj())
	{
		return factory->As_BuildingGameObj()->As_AirFactoryGameObj()->Get_Team_Vehicle_Count();
	}
	return 0;
};
unsigned int Get_Naval_Vehicle_Count(int Team) {
	GameObject *factory = Find_Naval_Factory(Team);
	if (factory && factory->As_BuildingGameObj() && factory->As_BuildingGameObj()->As_NavalFactoryGameObj())
	{
		return factory->As_BuildingGameObj()->As_NavalFactoryGameObj()->Get_Team_Vehicle_Count();
	}
	return 0;
};

const SpawnerDefClass* SpawnerClass::Get_Definition() const
{
	return this->definition;
}

Matrix3D SpawnerClass::Get_TM() const
{
	return this->transform;
}

bool CombatManager::Is_Gameplay_Permitted()
{
	if (NetworkHandler)
		return IsGameplayPermitted;
	else
		return true;
}
REF_DEF2(CombatNetworkHandlerClass*, CombatManager::NetworkHandler, 0x00855ED4, 0x008550BC);
REF_DEF2(bool, CombatManager::IsGameplayPermitted, 0x00855EFE, 0x008550E6);
