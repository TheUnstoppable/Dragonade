/*	Renegade Scripts.dll
    Dragonade Vehicle Manager
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAVEHICLE
#define INCLUDE_DAVEHICLE

#include "da_event.h"
#include "da_gameobj.h"

class DAVehicleObserverClass : public DAGameObjObserverClass {
public:
	virtual void Init();
	virtual void Timer_Expired(GameObject *obj,int Number);
	virtual void Vehicle_Exit(cPlayer *Player,int Seat);

	DA_API cPlayer *Get_Vehicle_Owner();
	DA_API void Set_Vehicle_Owner(cPlayer *Player);
	DA_API int Get_Team();
	DA_API void Set_Team(int team);
	DA_API void Reset_Last_Exit_Time();
	DA_API unsigned int Get_Time_Since_Last_Exit();
	DA_API void Reset_Last_Theft_Time();
	DA_API unsigned int Get_Time_Since_Last_Theft();
	virtual const char *Get_Name();
	inline VehicleGameObj *Get_Vehicle() {
		return (VehicleGameObj*)Get_Owner();
	}
	
private:
	cPlayer *VehicleOwner;
	int Team;
	unsigned int LastExitTime;
	unsigned int LastTheftTime;
};

class DAAirDroppedVehicleObserverClass : public DAGameObjObserverClass {
	virtual void Init();
	virtual bool Damage_Received_Request(ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type);
	virtual void Timer_Expired(GameObject *obj,int Number);
	virtual void Vehicle_Enter(cPlayer *Player,int Seat);
	virtual const char *Get_Name() { return "DAAirDroppedVehicleObserverClass"; }
	int CollisionGroup;
	float LastZ;
};

class DAVehicleManager : public DAEventClass {
public:
	static void Init();
	DA_API static bool Check_Limit_For_Player(cPlayer *Player);
	DA_API static DAVehicleObserverClass *Get_Vehicle_Data(GameObject *obj);
	DA_API static cPlayer *Get_Vehicle_Owner(GameObject *obj);
	DA_API static int Get_Team(GameObject *obj);
	DA_API static void Air_Drop_Vehicle(int Team,VehicleGameObj *Vehicle,const Vector3 &Position,float Facing);
	DA_API static VehicleGameObj *Air_Drop_Vehicle(int Team,const VehicleGameObjDef *Vehicle,const Vector3 &Position,float Facing);
	DA_API static VehicleGameObj *Air_Drop_Vehicle(int Team,unsigned int Vehicle,const Vector3 &Position,float Facing);
	DA_API static VehicleGameObj *Air_Drop_Vehicle(int Team,const char *Vehicle,const Vector3 &Position,float Facing);
	
private:
	class DefaultPurchaseEvent : public DAEventClass {
		virtual int Vehicle_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const VehicleGameObjDef *Item);
	};
	virtual void Level_Loaded_Event();
	virtual void Settings_Loaded_Event();
	virtual void Object_Created_Event(GameObject *obj);
	virtual int Vehicle_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const VehicleGameObjDef *Item);
	virtual void Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	virtual bool Vehicle_Flip_Event(VehicleGameObj *Vehicle);
	virtual bool Vehicle_Entry_Request_Event(VehicleGameObj *Vehicle,cPlayer *Player,int &Seat);
	virtual void Vehicle_Enter_Event(VehicleGameObj *Vehicle,cPlayer *Player,int Seat);

	//Settings
	bool EnableTheftMessage;
};

#endif