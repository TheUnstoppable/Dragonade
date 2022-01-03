/*	Renegade Scripts.dll
    Dragonade Game Object Manager
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAGAMEOBJ
#define INCLUDE_DAGAMEOBJ

#include "da_event.h"

class DA_API DAGameObjObserverClass : public GameObjObserverClass {
public:
	//Stock
	virtual ~DAGameObjObserverClass() { }
	virtual void Destroyed(GameObject *obj) { }
	virtual void Custom(GameObject *obj,int Message,int Param,GameObject *Sender) { }
	virtual void Poked(GameObject *obj,GameObject *Poker) { }
	virtual void Entered(GameObject *obj,GameObject *Enterer) { }
	virtual void Exited(GameObject *obj,GameObject *Exiter) { }
	virtual const char *Get_Name() = 0;
	virtual void Sound_Heard(GameObject *obj,const CombatSound &Sound) { }
	virtual void Enemy_Seen(GameObject *obj,GameObject *Enemy) { }
	virtual void Action_Complete(GameObject *obj,int ActionID,ActionCompleteReason CompleteReason) { }
	virtual void Timer_Expired(GameObject *obj,int Number) { }
	virtual void Animation_Complete(GameObject *obj,const char *Name) { }

	//Added in DA
	virtual void Init() { } //Called when the observer is attached to an object.
	virtual bool Vehicle_Entry_Request(cPlayer *Player,int &Seat) { return true; } //Called when a soldier tries to enter the vehicle this observer is attached to. Return bool to block entry or change Seat to change the seat the soldier will get in.
	virtual bool Vehicle_Entry_Request(VehicleGameObj *Vehicle,int &Seat) { return true; } //Called when the soldier this observer is attached to tries to enter a vehicle. Return bool to block entry or change Seat to change the seat the soldier will get in.
	virtual void Vehicle_Enter(cPlayer *Player,int Seat) { } //Called when a soldier enters the vehicle this observer is attached to.
	virtual void Vehicle_Enter(VehicleGameObj *Vehicle,int Seat) { } //Called when the soldier this observer is attached to enters a vehicle.
	virtual void Vehicle_Exit(cPlayer *Player,int Seat) { } //Called when a soldier exits the vehicle this observer is attached to.
	virtual void Vehicle_Exit(VehicleGameObj *Vehicle,int Seat) { } //Called when the soldier this observer is attached to exits a vehicle.
	virtual bool Vehicle_Flip() { return true; } //Called when the vehicle this observer is attached to tries to flip. Return bool to block the flip.
	virtual bool Damage_Dealt_Request(DamageableGameObj *Victim,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type) { return true; } //Called when the object this observer is attached to damages another object. Damage can be changed by modifying the OffenseObjectClass.
	virtual bool Damage_Received_Request(ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type) { return true; } //Called when the object this observer is attached to is damaged. Damage can be changed by modifying the OffenseObjectClass.
	virtual void Damage_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) { } //Called when the object this observer is attached to damages another object.
	virtual void Damage_Received(ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) { } //Called when the object this observer is attached to is damaged.
	virtual void Kill_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) { } //Called when the object this observer is attached to kills another object.
	virtual void Kill_Received(ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) { } //Called when the object this observer is attached to is killed.
	virtual bool PowerUp_Grant_Request(cPlayer *Player) { return true; } //Called when a soldier attempts to pickup the powerup this observer is attached to. Return false to block the pickup.
	virtual bool PowerUp_Grant_Request(const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) { return true; } //Called when the soldier this observer is attached to attempts to pickup a powerup. Return false to block the pickup.
	virtual void PowerUp_Grant(cPlayer *Player) { } //Called when a soldier picks up the powerup this observer is attached to.
	virtual void PowerUp_Grant(const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) { } //Called when the soldier this observer is attached to picks up a powerup.
	virtual bool Add_Weapon_Request(const WeaponDefinitionClass *Weapon) { return true; } //Called when something tries to grant the soldier this observer is attached to a weapon.
	virtual void Add_Weapon(WeaponClass *Weapon) { } //Called when the soldier this observer is attached to is granted a weapon.
	virtual void Remove_Weapon(WeaponClass *Weapon) { } //Called when a weapon is removed from the soldier this observer is attached to.
	virtual void Clear_Weapons() { } //Called when the soldier this observer is attached to's weapons are cleared.
	inline GameObject *Get_Owner() {
		return _Owner;
	}
	void Start_Timer(int Number,float Duration);
	void Stop_Timer(int Number);
	bool Is_Timer(int Number);
	void Set_Delete_Pending();

private:
	__declspec(deprecated("Replaced by Kill_Received.")) virtual void Killed(GameObject *obj,GameObject *Killer) { }
	__declspec(deprecated("Replaced by Damage_Received.")) virtual void Damaged(GameObject *obj,GameObject *Damager,float Damage) { }
	__declspec(deprecated("Replaced by Init.")) virtual void Created(GameObject *obj) { }
	__declspec(deprecated("Replaced by Get_Owner")) virtual GameObject *Owner();
	__declspec(deprecated("Reserved for internal use. Use Init instead.")) virtual void Attach(GameObject *obj);
	__declspec(deprecated("Reserved for internal use. Use destructor instead.")) virtual void Detach(GameObject *obj);
	ReferencerClass _Owner;
};

class DAGameObjManager : public DAEventClass {
public:
	static void Init();
	static inline bool Is_DAGameObjObserverClass(GameObjObserverClass *Observer);
	DA_API static void Set_Observer_Delete_Pending(DAGameObjObserverClass *Observer);
	DA_API static void Set_GameObj_Delete_Pending(GameObject *obj);
	DA_API static void Set_GameObj_TT_Only(GameObject *obj);
	DA_API static void Set_GameObj_Stock_Only(GameObject *obj);
	DA_API static void Set_GameObj_Invisible(GameObject *obj);
	DA_API static void Set_GameObj_Invisible_No_Delete(GameObject *obj);

private:
	virtual void Think();
	virtual void Player_Loaded_Event(cPlayer *Player);
	virtual bool Vehicle_Entry_Request_Event(VehicleGameObj *Vehicle,cPlayer *Player,int &Seat);
	virtual void Vehicle_Enter_Event(VehicleGameObj *Vehicle,cPlayer *Player,int Seat);
	virtual void Vehicle_Exit_Event(VehicleGameObj *Vehicle,cPlayer *Player,int Seat);
	virtual bool Vehicle_Flip_Event(VehicleGameObj *Vehicle);
	virtual bool Damage_Request_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type);
	virtual void Damage_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	virtual void Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	virtual bool PowerUp_Grant_Request_Event(cPlayer *Player,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj);
	virtual void PowerUp_Grant_Event(cPlayer *Player,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj);
	virtual bool Add_Weapon_Request_Event(cPlayer *Player,const WeaponDefinitionClass *Weapon);
	virtual void Add_Weapon_Event(cPlayer *Player,WeaponClass *Weapon);
	virtual void Remove_Weapon_Event(cPlayer *Player,WeaponClass *Weapon);
	virtual void Clear_Weapons_Event(cPlayer *Player);
	virtual void Object_Destroyed_Event(GameObject *obj);

	static DynamicVectorClass<DAGameObjObserverClass*> ObserversDeletePending;
	static DynamicVectorClass<GameObject*> GameObjsDeletePending;
	static DynamicVectorClass<GameObject*> TTGameObjs;
	static DynamicVectorClass<GameObject*> StockGameObjs;
	static DynamicVectorClass<GameObject*> InvisibleGameObjs;
};

#endif