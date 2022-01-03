/*	Renegade Scripts.dll
    Dragonade Vehicle Ownership Game Feature
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAVEHICLEOWNERSHIP
#define INCLUDE_DAVEHICLEOWNERSHIP

#include "da_event.h"
#include "da_gamefeature.h"
#include "da_gameobj.h"

namespace DAVehicleOwnershipState {
	enum State {
		BOUND,
		LOCKED,
		SELLING,
		FREE
	};
};

class DAVehicleOwnershipObserverClass : public DAGameObjObserverClass {
public:
	DA_API DAVehicleOwnershipObserverClass(VehicleGameObj *Vehicle,cPlayer *Player);
	DA_API ~DAVehicleOwnershipObserverClass();
	DA_API void Create_Icons();
	DA_API void Destroy_Icons();
	DA_API void Lock();
	DA_API void Unlock();
	inline bool Is_Locked() {
		return (State == DAVehicleOwnershipState::LOCKED);
	}
	DA_API void Sell();
	inline bool Is_Selling() {
		return (State == DAVehicleOwnershipState::SELLING);
	}
	DA_API void Free();
	inline bool Is_Free() {
		return (State == DAVehicleOwnershipState::FREE);
	}
	DA_API void Unbind();
	inline VehicleGameObj *Get_Vehicle() {
		return (VehicleGameObj*)Get_Owner();
	}
	inline cPlayer *Get_Vehicle_Owner() {
		return VehicleOwner;
	}
	inline int Get_Team() {
		if (VehicleOwner) {
			return VehicleOwner->Get_Player_Type();
		}
		return INT_MAX;
	}
	virtual const char *Get_Name();

private:
	void Update_Icons();
	void Set_State(DAVehicleOwnershipState::State state);
	virtual void Vehicle_Enter(cPlayer *Player,int Seat);
	virtual bool Vehicle_Entry_Request(cPlayer *Player,int &Seat);
	virtual bool Damage_Received_Request(ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type);
	virtual void Destroyed(GameObject *obj);
	virtual void Timer_Expired(GameObject *obj,int Number);
	
	cPlayer *VehicleOwner;
	DAVehicleOwnershipState::State State;
	ReferencerClass Icon[2];
};

class DAVehicleOwnershipGameFeatureClass : public DAEventClass, public DAGameFeatureClass {
public:
	DA_API DAVehicleOwnershipObserverClass *Bind_Vehicle(VehicleGameObj *Vehicle,cPlayer *Player);
	DA_API bool Unbind_Vehicle(VehicleGameObj *Vehicle);
	DA_API bool Unbind_Vehicle(cPlayer *Player);
	DA_API bool Lock_Vehicle(VehicleGameObj *Vehicle);
	DA_API bool Lock_Vehicle(cPlayer *Player);
	DA_API bool Unlock_Vehicle(VehicleGameObj *Vehicle);
	DA_API bool Unlock_Vehicle(cPlayer *Player);
	cPlayer *Get_Vehicle_Owner(GameObject *Vehicle) {
		DAVehicleOwnershipObserverClass *Data = Get_Vehicle_Data(Vehicle);
		if (Data) {
			return Data->Get_Vehicle_Owner();
		}
		return 0;
	}
	VehicleGameObj *Get_Vehicle(cPlayer *Player) {
		DAVehicleOwnershipObserverClass *Data = Get_Vehicle_Data(Player);
		if (Data) {
			return Data->Get_Vehicle();
		}
		return 0;
	}
	DA_API DAVehicleOwnershipObserverClass *Get_Vehicle_Data(GameObject *Vehicle);
	DA_API DAVehicleOwnershipObserverClass *Get_Vehicle_Data(cPlayer *Player);
	DA_API DAVehicleOwnershipObserverClass *Get_Vehicle_Data(SoldierGameObj *Soldier);
	inline float Get_Sell_Percent() {
		return SellPercent;
	}

private:
	virtual void Init();
	virtual void Settings_Loaded_Event();
	~DAVehicleOwnershipGameFeatureClass();
	bool Bind_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	bool Unbind_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	bool Lock_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	bool Unlock_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	bool BL_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	bool VKick_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	bool VStatus_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	bool Free_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	bool SellVeh_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);

	virtual void Vehicle_Enter_Event(VehicleGameObj *Vehicle,cPlayer *Player,int Seat);
	virtual void Player_Leave_Event(cPlayer *Player);
	virtual void Team_Change_Event(cPlayer *Player);
	virtual void Object_Destroyed_Event(GameObject *obj);
	virtual void Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);

	float SellPercent;
};

extern DA_API DAGameFeatureRegistrant<DAVehicleOwnershipGameFeatureClass> DAVehicleOwnershipGameFeatureClassRegistrant;
#define DAVehicleOwnershipGameFeature ((DAVehicleOwnershipGameFeatureClass*)DAVehicleOwnershipGameFeatureClassRegistrant.Get_Instance())

#endif