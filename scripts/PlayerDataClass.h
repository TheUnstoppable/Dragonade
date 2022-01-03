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
#ifndef TT_INCLUDE_PLAYERDATACLASS_H
#define TT_INCLUDE_PLAYERDATACLASS_H
#include "DataSafe.h"
#include "ReferencerClass.h"
#include "engine_vector.h"
#include "SmartGameObj.h"
class DamageableGameObj;
class BitStreamClass;
class PlayerDataClass
{
//protected: //DA
public: //DA
	safe_float Score; // 0004
	safe_float Money; // 0008
	ReferencerClass Owner; // 000C
	float PunishTimer; // 001C
	float GameTime; // 0020
	float SessionTime; // 0024
	int EnemiesKilled; // 0028
	int AlliesKilled; // 002C
	int ShotsFired; // 0030
	int HeadShots; // 0034
	int TorsoShots; // 0038
	int ArmShots; // 003C
	int LegShots; // 0040
	int CrotchShots; // 0044
	float FinalHealth; // 0048
	int PowerupsCollected; // 004C
	int VehiclesDestroyed; // 0050
	float VehicleTime; // 0054
	int KillsFromVehicle; // 0058
	int Squishes; // 005C
	float CreditGrant; // 0060
	int BuildingDestroyed; // 0064
	int HeadHit; // 0068
	int TorsoHit; // 006C
	int ArmHit; // 0070
	int LegHit; // 0074
	int CrotchHit; // 0078
	int WeaponFired; // 007C
	SimpleDynVecClass<int> WeaponsFiredID; // 0080
	SimpleDynVecClass<int> WeaponsFiredCount; // 0090

public:
	PlayerDataClass();
	~PlayerDataClass();
	virtual void Reset_Player();
	virtual void Set_Score(float score);
	virtual void Increment_Score(float score);
	virtual void Set_Money(float money);
	virtual void Increment_Money(float money);
	virtual bool Purchase_Item(int money);
	bool Purchase_Item(float money); //DA
	virtual void Apply_Damage_Points(float points,DamageableGameObj *damager);
	virtual void Apply_Death_Points(float points,DamageableGameObj *damager);
	void Export_Occasional(BitStreamClass &BitStream);
	void Import_Occasional(BitStreamClass &BitStream);
	void Stats_Reset();
	void Stats_Add_Game_Time(float time) { GameTime += time; SessionTime += time; }
	void Stats_Add_Enemy_Killed() {EnemiesKilled++;}
	void Stats_Add_Ally_Killed() {AlliesKilled++;}
	void Stats_Add_Shot_Fired() {ShotsFired++;}
	void Stats_Add_Head_Shot() {HeadShots++;}
	void Stats_Add_Torso_Shot() {TorsoShots++;}
	void Stats_Add_Arm_Shot() {ArmShots++;}
	void Stats_Add_Leg_Shot() {LegShots++;}
	void Stats_Add_Crotch_Shot() {CrotchShots++;}
	void Stats_Set_Final_Health(float health) { FinalHealth = health; }
	void Stats_Add_Powerup() {PowerupsCollected++;}
	void Stats_Add_Vehicle_Destroyed() {VehiclesDestroyed++;}
	void Stats_Add_Vehicle_Time(float time) { VehicleTime += time; }
	void Stats_Add_Kill_From_Vehicle() {KillsFromVehicle++;}
	void Stats_Add_Squish() {Squishes++;}
	void Stats_Add_Credit_Grant(float grant) {CreditGrant += grant;}
	void Stats_Add_Building_Destroyed() {BuildingDestroyed++;}
	void Stats_Add_Torso_Hit() {TorsoHit++;}
	void Stats_Add_Head_Hit() {HeadHit++;}
	void Stats_Add_Arm_Hit() {ArmHit++;}
	void Stats_Add_Leg_Hit() {LegHit++;}
	void Stats_Add_Crotch_Hit() {CrotchHit++;}
	void Stats_Add_Weapon_Fired(int weapon);
	SmartGameObj *Get_GameObj() {return (SmartGameObj *)(ScriptableGameObj *)Owner;}
	void Set_GameObj(SmartGameObj *obj) {Owner = obj;}
	float Get_Score() const {return Score;}
	float Get_Money() const {return Money;}
	float Get_Session_Time() const {return SessionTime;}
	float Get_Punish_Timer() {return PunishTimer;}
	void Inc_Punish_Timer(float time) {PunishTimer += time;}

}; // 00A0

#endif