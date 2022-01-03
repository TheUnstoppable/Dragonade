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

#ifndef INCLUDE_ENGINEDA
#define INCLUDE_ENGINEDA

#include "SimpleGameObj.h"
#include "Definition.h"
#include "DefinitionMgrClass.h"
#include "TranslateDBClass.h"
#include "VehicleFactoryGameObj.h"
#include "PhysicalGameObj.h"
#include "BuildingGameObj.h"
#include "SmartGameObj.h"
#include "SoldierGameObj.h"
#include "SoldierGameObjDef.h"
#include "PowerupGameObjDef.h"
#include "weaponmgr.h"
#include "cPlayer.h"
#include "cTeam.h"
#include "engine_tt.h"

template <typename A, typename B> class PairClass {
public:
	A First;
	B Second;
	inline PairClass() { }
	inline PairClass(const A &first,const B &second) {
		First = first;
		Second = second;
	}
	inline PairClass(const PairClass &That) {
		this = that;
	}
	inline PairClass &operator=(const PairClass &That) {
		First = That.First;
		Second = That.Second;
		return *this;
	}
	inline bool operator==(const PairClass &That) {
		return (First == That.First && Second == That.Second);
	}
	inline bool operator!=(const PairClass &That) {
		return (First != That.First || Second != That.Second);
	}
};

#define STYLE_C4 0
#define STYLE_BEACON 6

DA_API void Destroy_All_Objects_With_Script(const char *Script);
DA_API bool Has_Beacon(ArmedGameObj *obj);

DA_API float Get_Distance_To_Closest_Beacon(const Vector3 &Position,int Team);
DA_API float Get_Distance_To_Closest_Building(const Vector3 &Position,int Team);
DA_API float Get_Distance_To_Closest_PCT(const Vector3 &Position,int Team);
DA_API float Get_Distance_To_Closest_Player(const Vector3 &Position,int Team = 2);
DA_API BuildingGameObj *Get_Closest_Building(const Vector3 &Position,int Team);
DA_API PhysicalGameObj *Get_Closest_Fake_Building(const Vector3 &Position,int Team);
DA_API BuildingGameObj *Get_Random_Building(int Team);

DA_API void Send_Custom_Event_Vehicle_Occupants(GameObject *obj,GameObject *Sender,int Message,int Param,float Delay,bool Driver);
DA_API void Disarm_Closest_Beacon(const Vector3 &pos,int Team);
DA_API void Disarm_All_Beacons_Team(int Team);
DA_API void Disarm_All_Beacons_Player(GameObject *Player);
DA_API void Play_Animation_All_Players(const char *Anim);
DA_API StringClass Get_String_Player_Name(GameObject *obj);
DA_API StringClass Get_String_Player_Name_By_ID(int ID);
DA_API const wchar_t *Get_Wide_Player_Name_By_ID(int ID);
DA_API StringClass Get_IP_Address_String(int PlayerID);
DA_API int Find_Empty_Vehicle_Seat(VehicleGameObj *obj,bool AllowDriver = true);
DA_API void Change_Team_3(cPlayer *Player,int Team);
DA_API void Change_Team_4(cPlayer *Player,int Team);
DA_API void Change_Team_5(cPlayer *Player,int Team);
DA_API cPlayer *Match_Player(cPlayer *Player,const StringClass &Nick,bool TeamOnly = false,bool AllowSelf = false);
DA_API void Damage_Objects_Half_No_Kill();
DA_API void Attach_Script_To_Definition(const char *Preset,const char *Script,const char *Params);
DA_API void Attach_Script_To_Definitions(unsigned long CID,const char *Script,const char *Params);
DA_API void Remove_Script_From_Definition(const char *Preset, const char *Script);
DA_API void Remove_Script_From_Definitions(unsigned long CID, const char *Script);
DA_API void Set_Object_Type_Preset(const char *Preset,int Team);
DA_API cTeam *Find_Team(int team);
DA_API void Give_Credits_Team(int Team,float Credits);
DA_API void Give_Points_Players_In_Range(const Vector3 &Position,float Range,float Points,bool GiveMoney = false);
DA_API void Give_Points_Players_In_Range_Team(int Team,const Vector3 &Position,float Range,float Points,bool GiveMoney = false);
DA_API StringClass Encrypt_Apgar(const char *Password);
DA_API int Get_Player_Team(int PlayerID);
DA_API void Set_Facing_Player(GameObject *obj,float Facing);
DA_API bool Overlap_Test(GameObject *obj,const Vector3 &Position,bool CheckDynOnly = false);
DA_API void Console_InputF(const char *Format,...);
inline bool Is_Simple(GameObject *obj) {
	if (!obj || !obj->As_PhysicalGameObj()) {
		return false;
	}
	return !!((PhysicalGameObj*)obj)->As_SimpleGameObj();
}
inline bool Is_Powerup(GameObject *obj) {
	if (!obj || !obj->As_PhysicalGameObj()) {
		return false;
	}
	return !!((PhysicalGameObj*)obj)->As_PowerUpGameObj();
}
inline bool Is_C4(GameObject *obj) {
	if (!obj || !obj->As_PhysicalGameObj()) {
		return false;
	}
	return !!((PhysicalGameObj*)obj)->As_C4GameObj();
}
inline bool Is_Beacon(GameObject *obj) {
	if (!obj || !obj->As_PhysicalGameObj()) {
		return false;
	}
	return !!((PhysicalGameObj*)obj)->As_BeaconGameObj();
}
inline bool Is_Armed(GameObject *obj) {
	if (!obj || !obj->As_PhysicalGameObj()) {
		return false;
	}
	return !!((PhysicalGameObj*)obj)->As_ArmedGameObj();
}
inline bool Is_PowerPlant(GameObject *obj) {
	if (!obj || !obj->As_BuildingGameObj()) {
		return false;
	}
	return !!((BuildingGameObj*)obj)->As_PowerPlantGameObj();
}
inline bool Is_VehicleFactory(GameObject *obj) {
	if (!obj || !obj->As_BuildingGameObj()) {
		return false;
	}
	return !!((BuildingGameObj*)obj)->As_VehicleFactoryGameObj();
}
inline bool Is_Refinery(GameObject *obj) {
	if (!obj || !obj->As_BuildingGameObj()) {
		return false;
	}
	return !!((BuildingGameObj*)obj)->As_RefineryGameObj();
}
inline bool Is_SoldierFactory(GameObject *obj) {
	if (!obj || !obj->As_BuildingGameObj()) {
		return false;
	}
	return !!((BuildingGameObj*)obj)->As_SoldierFactoryGameObj();
}
inline bool Is_DecorationPhys(GameObject *obj) {
	if (!obj || !obj->As_PhysicalGameObj()) {
		return false;
	}
	return !!(((PhysicalGameObj*)obj)->Peek_Physical_Object()->Get_Definition()->Get_Class_ID() == 0x9000);
}
inline bool Is_Player(GameObject *obj) {
	return (obj && obj->As_SoldierGameObj() && ((SoldierGameObj*)obj)->Get_Player_Data());
}

DA_API bool Is_Stealth_Enabled2(GameObject *obj);
DA_API bool Is_Stealth_Unit(GameObject *obj);
DA_API unsigned int Get_Ground_Vehicle_Count(int Team);
DA_API unsigned int Get_Air_Vehicle_Count(int Team);
DA_API unsigned int Get_Naval_Vehicle_Count(int Team);

DA_API void Set_Object_Dirty_Bit_For_Version_Greater_Than(NetworkObjectClass *obj,float Version,NetworkObjectClass::DIRTY_BIT Bit,bool OnOff);
DA_API void Set_Object_Dirty_Bit_For_Version_Less_Than(NetworkObjectClass *obj,float Version,NetworkObjectClass::DIRTY_BIT Bit,bool OnOff);
DA_API void Set_Object_Dirty_Bit_For_Team_Version_Greater_Than(NetworkObjectClass *obj,int Team,float Version,NetworkObjectClass::DIRTY_BIT Bit,bool OnOff);
DA_API void Set_Object_Dirty_Bit_For_Team_Version_Less_Than(NetworkObjectClass *obj,int Team,float Version,NetworkObjectClass::DIRTY_BIT Bit,bool OnOff);
DA_API int Setup_Send_Message_Fake(const char *NewNick,int ID = 0);
DA_API int Setup_Send_Message_Team_Fake(const char *NewNick,int Team,int ID = 0);
DA_API void Restore_Send_Message_Fake();

DA_API void Send_Announcement(const char *StringID);
DA_API void Send_Announcement_Version_Less_Than(const char *StringID,float Version);
DA_API void Send_Announcement_Player(int ID,const char *StringID);
DA_API void Send_Announcement_Player_Version_Less_Than(int ID,const char *StringID,float Version);
DA_API void Send_Announcement_Team(int Team,const char *StringID);
DA_API void Send_Announcement_Team_Version_Less_Than(int Team,const char *StringID,float Version);

DA_API void Send_Message_Team_With_Team_Color(int Team,const char *Msg);
DA_API void Send_Message_Player_By_ID(int ID,unsigned int Red,int unsigned Green,int unsigned Blue,const char *Message);
DA_API void Create_2D_WAV_Sound_Player_By_ID(int ID,const char *Sound);

DA_API PhysicalGameObj *Create_Object(const char *Preset,const Vector3 &Position);
DA_API PhysicalGameObj *Create_Object(const char *Preset,const Matrix3D &Transform);
DA_API PhysicalGameObj *Create_Object(int Preset,const Vector3 &Position);
DA_API PhysicalGameObj *Create_Object(int Preset,const Matrix3D &Transform);
DA_API PhysicalGameObj *Create_Object(const DefinitionClass *Def,const Vector3 &Position);
DA_API PhysicalGameObj *Create_Object(const DefinitionClass *Def,const Matrix3D &Transform);

DA_API SoldierGameObj *Create_Commando(cPlayer *Player,const char *Preset,const Vector3 &Position);
DA_API SoldierGameObj *Create_Commando(cPlayer *Player,const char *Preset,const Matrix3D &Transform);
DA_API SoldierGameObj *Create_Commando(cPlayer *Player,int Preset,const Vector3 &Position);
DA_API SoldierGameObj *Create_Commando(cPlayer *Player,int Preset,const Matrix3D &Transform);
DA_API SoldierGameObj *Create_Commando(cPlayer *Player,const DefinitionClass *Def,const Vector3 &Position);
DA_API SoldierGameObj *Create_Commando(cPlayer *Player,const DefinitionClass *Def,const Matrix3D &Transform);

DA_API void Disable_Building_Type(int Team,BuildingConstants::BuildingType Type);
DA_API unsigned int Get_Hash(const char *String);
DA_API bool Is_Starting_Weapon(const SoldierGameObjDef *Soldier,const PowerUpGameObjDef *PowerUp);
DA_API bool Is_Starting_Weapon(SoldierGameObj *Soldier,const PowerUpGameObjDef *PowerUp);

DA_API void Fix_Stuck_Objects(const Vector3 &Position,float CheckRange,float Range,bool DestroyUnfixable = true);
DA_API bool Fix_Stuck_Object(PhysicalGameObj *obj,float Range);

DA_API StringClass Clean_Model_Name(StringClass Model);
DA_API StringClass Get_Weapon_PowerUp_Model(const WeaponDefinitionClass *Weapon);

DA_API void Add_Console_Function(ConsoleFunctionClass *Func);

DA_API int Get_Building_Count(int Team,bool Destroyed);

DA_API void Enable_HUD(bool Enable);
DA_API void Enable_HUD_Player_By_ID(int ID,bool Enable);

DA_API void Set_Fog_Enable_Player_By_ID(int ID,bool Enable);
DA_API void Set_Fog_Range_Player_By_ID(int ID,float StartDistance,float EndDistance,float Transition);

DA_API bool Exit_Vehicle(SoldierGameObj *Soldier);

DA_API void Reverse_Damage(GameObject *obj,float Amount);

DA_API void Set_Emote_Icon(int ID,const char *Model,int Team);
class SpawnerClass;
extern REF_DECL(DynamicVectorClass<SpawnerClass*>,SpawnerList);
extern REF_DECL(float, TimeScale);

#endif