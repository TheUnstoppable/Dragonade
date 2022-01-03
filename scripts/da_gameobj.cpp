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

#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "engine_DA.h"
#include "da.h"
#include "da_gameobj.h"
#include "da_player.h"
#include "GameObjManager.h"

DynamicVectorClass<DAGameObjObserverClass*> DAGameObjManager::ObserversDeletePending;
DynamicVectorClass<GameObject*> DAGameObjManager::GameObjsDeletePending;
DynamicVectorClass<GameObject*> DAGameObjManager::TTGameObjs;
DynamicVectorClass<GameObject*> DAGameObjManager::StockGameObjs;
DynamicVectorClass<GameObject*> DAGameObjManager::InvisibleGameObjs;
REF_DEF2(sint32, NextID, 0x00813944, 0x00812B1C);

void DAGameObjObserverClass::Attach(GameObject *obj) {
	Set_ID(NextID);
	NextID++;
	_Owner = obj;
	Init();
}

void DAGameObjObserverClass::Detach(GameObject *obj) {
	delete this;
}

GameObject _declspec(naked) *DAGameObjObserverClass::Owner() {
	_asm {
		mov ecx, 1
		mov eax, 0
		retn
	}
}

void DAGameObjObserverClass::Start_Timer(int Number,float Duration) {
	Commands->Start_Timer(Get_Owner(),(ScriptClass *)this,Duration,Number);
}

void DAGameObjObserverClass::Stop_Timer(int Number) {
	Stop_Timer2(Get_Owner(),(ScriptClass *)this,Number);
}

bool DAGameObjObserverClass::Is_Timer(int Number) {
	return Has_Timer(Get_Owner(),(ScriptClass *)this,Number);
}

void DAGameObjObserverClass::Set_Delete_Pending() {
	DAGameObjManager::Set_Observer_Delete_Pending(this);
}

void (*Old_Enable_Stealth) (GameObject * object, bool onoff);

void Enable_Stealth(GameObject *obj,bool Enable) {
	if (obj->As_SmartGameObj() && ((SmartGameObj*)obj)->Is_Stealth_Enabled() != Enable) {
		Old_Enable_Stealth(obj,Enable);
		if (obj->As_SoldierGameObj()) {
			if (Enable) {
				const BaseGameObjDef *Stealth = (BaseGameObjDef*)Find_Named_Definition("CnC_Nod_FlameThrower_2SF");
				if (Stealth) {
					const BaseGameObjDef *DefSave = obj->Definition;
					obj->Definition = Stealth;
					for (SLNode<cPlayer>* z = Get_Player_List()->Head();z;z = z->Next()) {
						cPlayer *Player = z->Data();
						if (Player->Is_Alive_And_Kicking() && !Player->Get_DA_Player()->Is_TT_Client()) {
							obj->Set_Object_Dirty_Bit(Player->Get_Id(),NetworkObjectClass::BIT_RARE,true);
							Update_Network_Object_Player(obj,Player->Get_Id());
							obj->Set_Object_Dirty_Bit(Player->Get_Id(),NetworkObjectClass::BIT_RARE,true);
							Update_Network_Object_Player(obj,Player->Get_Id());
						}
					}
					obj->Definition = DefSave;
				}
			}
			else {
				obj->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
			}
		}
	}
}

void DAGameObjManager::Init() {
	static DAGameObjManager Instance;
	Instance.Register_Event(DAEvent::THINK,INT_MAX);
	Instance.Register_Event(DAEvent::PLAYERLOADED,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEENTRYREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEENTER,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEEXIT,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEFLIP,INT_MAX);
	Instance.Register_Event(DAEvent::POWERUPGRANTREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::POWERUPGRANT,INT_MAX);
	Instance.Register_Event(DAEvent::ADDWEAPONREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::ADDWEAPON,INT_MAX);
	Instance.Register_Event(DAEvent::REMOVEWEAPON,INT_MAX);
	Instance.Register_Event(DAEvent::CLEARWEAPONS,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::DAMAGERECEIVEDREQUEST,DAObjectEvent::ALL,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::DAMAGERECEIVED,DAObjectEvent::ALL,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::KILLRECEIVED,DAObjectEvent::ALL,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::DESTROYED,DAObjectEvent::ALL,INT_MAX);

	Old_Enable_Stealth = Commands->Enable_Stealth;
	Commands->Enable_Stealth = Enable_Stealth;
}

#pragma warning(disable: 4740)
bool DAGameObjManager::Is_DAGameObjObserverClass(GameObjObserverClass *Observer) {
	Observer->Owner();
	_asm {
		cmp ecx, 1
		jnz False
	}
	return true;

False:
	return false;
}

void DAGameObjManager::Set_Observer_Delete_Pending(DAGameObjObserverClass *Observer) {
	if (Observer->Get_Owner() && ObserversDeletePending.ID(Observer) == -1 && !Observer->Get_Owner()->Is_Delete_Pending()) {
		ObserversDeletePending.Add(Observer);
	}
}

//This is a workaround to a bug where you cannot destroy one object within the destroyed event of another.
void DAGameObjManager::Set_GameObj_Delete_Pending(GameObject *obj) {
	if (GameObjsDeletePending.ID(obj) == -1 && !obj->Is_Delete_Pending()) {
		GameObjsDeletePending.Add(obj);
	}
}

void DAGameObjManager::Set_GameObj_TT_Only(GameObject *obj) {
	if (TTGameObjs.ID(obj) == -1) {
		TTGameObjs.Add(obj);
		Update_Network_Object(obj);
		bool PendingSave = obj->Is_Delete_Pending();
		obj->Set_Is_Delete_Pending(true);
		for (SLNode<cPlayer>* z = Get_Player_List()->Head();z;z = z->Next()) {
			cPlayer *Player = z->Data();
			if (Player->Is_Alive_And_Kicking() && Player->Get_DA_Player()->Is_Stock_Client()) {
				Update_Network_Object_Player(obj,Player->Get_Id());
			}
		}
		obj->Set_Is_Delete_Pending(PendingSave);
	}
}

void DAGameObjManager::Set_GameObj_Stock_Only(GameObject *obj) {
	if (StockGameObjs.ID(obj) == -1) {
		StockGameObjs.Add(obj);
		Update_Network_Object(obj);
		bool PendingSave = obj->Is_Delete_Pending();
		obj->Set_Is_Delete_Pending(true);
		for (SLNode<cPlayer>* z = Get_Player_List()->Head();z;z = z->Next()) {
			cPlayer *Player = z->Data();
			if (Player->Is_Alive_And_Kicking() && Player->Get_DA_Player()->Is_TT_Client()) {
				Update_Network_Object_Player(obj,Player->Get_Id());
			}
		}
		obj->Set_Is_Delete_Pending(PendingSave);
	}
}

void DAGameObjManager::Set_GameObj_Invisible(GameObject *obj) {
	if (InvisibleGameObjs.ID(obj) == -1) {
		Set_GameObj_Invisible_No_Delete(obj);
		bool PendingSave = obj->Is_Delete_Pending();
		obj->Set_Is_Delete_Pending(true);
		Update_Network_Object(obj);
		obj->Set_Is_Delete_Pending(PendingSave);
	}
}

void DAGameObjManager::Set_GameObj_Invisible_No_Delete(GameObject *obj) {
	if (InvisibleGameObjs.ID(obj) == -1) {
		InvisibleGameObjs.Add(obj);
		obj->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_CREATION,false);
	}
}

void DAGameObjManager::Think() {
	for (int i = GameObjsDeletePending.Count()-1;i >= 0;i--) {
		GameObject *Temp = GameObjsDeletePending[i];
		GameObjsDeletePending.Delete(i);
		Temp->Set_Delete_Pending();
	}
	for (int i = 0;i < ObserversDeletePending.Count();i++) {
		if (ObserversDeletePending[i]->Get_Owner()) {
			ObserversDeletePending[i]->Get_Owner()->Remove_Observer(ObserversDeletePending[i]);
		}
	}
	ObserversDeletePending.Delete_All();
}

void DAGameObjManager::Player_Loaded_Event(cPlayer *Player) {
	for (int i = 0;i < InvisibleGameObjs.Count();i++) {
		InvisibleGameObjs[i]->Set_Object_Dirty_Bit(Player->Get_Id(),NetworkObjectClass::BIT_CREATION,false);
	}
	if (Player->Get_DA_Player()->Is_TT_Client()) {
		for (int i = 0;i < StockGameObjs.Count();i++) {
			StockGameObjs[i]->Set_Object_Dirty_Bit(Player->Get_Id(),NetworkObjectClass::BIT_CREATION,false);
		}
	}
	else {
		for (int i = 0;i < TTGameObjs.Count();i++) {
			TTGameObjs[i]->Set_Object_Dirty_Bit(Player->Get_Id(),NetworkObjectClass::BIT_CREATION,false);
		}
		for (SLNode<SoldierGameObj> *x = GameObjManager::SoldierGameObjList.Head();x;x = x->Next()) { 
			SoldierGameObj *Soldier = x->Data();
			if (Soldier->Is_Stealth_Enabled() && !Soldier->Get_Definition().Is_Stealthed()) {
				const BaseGameObjDef *Stealth = (BaseGameObjDef*)Find_Named_Definition("CnC_Nod_FlameThrower_2SF");
				if (Stealth) {
					const BaseGameObjDef *DefSave = Soldier->Definition;
					Soldier->Definition = Stealth;
					Update_Network_Object_Player(Soldier,Player->Get_Id());
					Soldier->Definition = DefSave;
				}
			}
		}
	}
}

bool DAGameObjManager::Vehicle_Entry_Request_Event(VehicleGameObj *Vehicle,cPlayer *Player,int &Seat) {
	for (int i = 0;i < Vehicle->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Vehicle->Get_Observers()[i])) {
			if (!((DAGameObjObserverClass*)Vehicle->Get_Observers()[i])->Vehicle_Entry_Request(Player,Seat)) {
				return false;
			}
		}
	}
	SoldierGameObj *Soldier = Player ? Player->Get_GameObj() : Vehicle->Get_Occupant(Seat);
	for (int i = 0;i < Soldier->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Soldier->Get_Observers()[i])) {
			if (!((DAGameObjObserverClass*)Soldier->Get_Observers()[i])->Vehicle_Entry_Request(Vehicle,Seat)) {
				return false;
			}
		}
	}
	return true;
}

void DAGameObjManager::Vehicle_Enter_Event(VehicleGameObj *Vehicle,cPlayer *Player,int Seat) {
	for (int i = 0;i < Vehicle->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Vehicle->Get_Observers()[i])) {
			((DAGameObjObserverClass*)Vehicle->Get_Observers()[i])->Vehicle_Enter(Player,Seat);
		}
	}
	SoldierGameObj *Soldier = Player ? Player->Get_GameObj() : Vehicle->Get_Occupant(Seat);
	for (int i = 0;i < Soldier->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Soldier->Get_Observers()[i])) {
			((DAGameObjObserverClass*)Soldier->Get_Observers()[i])->Vehicle_Enter(Vehicle,Seat);
		}
	}
}

void DAGameObjManager::Vehicle_Exit_Event(VehicleGameObj *Vehicle,cPlayer *Player,int Seat) {
	for (int i = 0;i < Vehicle->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Vehicle->Get_Observers()[i])) {
			((DAGameObjObserverClass*)Vehicle->Get_Observers()[i])->Vehicle_Exit(Player,Seat);
		}
	}
	SoldierGameObj *Soldier = Player ? Player->Get_GameObj() : Vehicle->Get_Occupant(Seat);
	for (int i = 0;i < Soldier->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Soldier->Get_Observers()[i])) {
			((DAGameObjObserverClass*)Soldier->Get_Observers()[i])->Vehicle_Exit(Vehicle,Seat);
		}
	}
}

bool DAGameObjManager::Vehicle_Flip_Event(VehicleGameObj *Vehicle) {
	for (int i = 0;i < Vehicle->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Vehicle->Get_Observers()[i])) {
			if (!((DAGameObjObserverClass*)Vehicle->Get_Observers()[i])->Vehicle_Flip()) {
				return false;
			}
		}
	}
	return true;
}

bool DAGameObjManager::Damage_Request_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type) {
	for (int i = 0;i < Victim->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Victim->Get_Observers()[i])) {
			if (!((DAGameObjObserverClass*)Victim->Get_Observers()[i])->Damage_Received_Request(Damager,Damage,Warhead,Scale,Type)) {
				return false;
			}
		}
	}
	if (Damager) {
		for (int i = 0;i < Damager->Get_Observers().Count();i++) {
			if (Is_DAGameObjObserverClass(Damager->Get_Observers()[i])) {
				if (!((DAGameObjObserverClass*)Damager->Get_Observers()[i])->Damage_Dealt_Request(Victim,Damage,Warhead,Scale,Type)) {
					return false;
				}
			}
		}
	}
	return true;
}

void DAGameObjManager::Damage_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	for (int i = 0;i < Victim->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Victim->Get_Observers()[i])) {
			((DAGameObjObserverClass*)Victim->Get_Observers()[i])->Damage_Received(Damager,Damage,Warhead,Scale,Type);
		}
	}
	if (Damager) {
		for (int i = 0;i < Damager->Get_Observers().Count();i++) {
			if (Is_DAGameObjObserverClass(Damager->Get_Observers()[i])) {
				((DAGameObjObserverClass*)Damager->Get_Observers()[i])->Damage_Dealt(Victim,Damage,Warhead,Scale,Type);
			}
		}
	}
}

void DAGameObjManager::Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	for (int i = 0;i < Victim->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Victim->Get_Observers()[i])) {
			((DAGameObjObserverClass*)Victim->Get_Observers()[i])->Kill_Received(Killer,Damage,Warhead,Scale,Type);
		}
	}
	if (Killer) {
		for (int i = 0;i < Killer->Get_Observers().Count();i++) {
			if (Is_DAGameObjObserverClass(Killer->Get_Observers()[i])) {
				((DAGameObjObserverClass*)Killer->Get_Observers()[i])->Kill_Dealt(Victim,Damage,Warhead,Scale,Type);
			}
		}
	}
}

bool DAGameObjManager::PowerUp_Grant_Request_Event(cPlayer *Player,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) {
	if (PowerUpObj) {
		for (int i = 0;i < PowerUpObj->Get_Observers().Count();i++) {
			if (Is_DAGameObjObserverClass(PowerUpObj->Get_Observers()[i])) {
				if (!((DAGameObjObserverClass*)PowerUpObj->Get_Observers()[i])->PowerUp_Grant_Request(Player)) {
					return false;
				}
			}
		}
	}
	SoldierGameObj *Soldier = Player->Get_GameObj();
	for (int i = 0;i < Soldier->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Soldier->Get_Observers()[i])) {
			if (!((DAGameObjObserverClass*)Soldier->Get_Observers()[i])->PowerUp_Grant_Request(PowerUp,PowerUpObj)) {
				return false;
			}
		}
	}
	return true;
}

void DAGameObjManager::PowerUp_Grant_Event(cPlayer *Player,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) {
	if (PowerUpObj) {
		for (int i = 0;i < PowerUpObj->Get_Observers().Count();i++) {
			if (Is_DAGameObjObserverClass(PowerUpObj->Get_Observers()[i])) {
				((DAGameObjObserverClass*)PowerUpObj->Get_Observers()[i])->PowerUp_Grant(Player);
			}
		}
	}
	SoldierGameObj *Soldier = Player->Get_GameObj();
	for (int i = 0;i < Soldier->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Soldier->Get_Observers()[i])) {
			((DAGameObjObserverClass*)Soldier->Get_Observers()[i])->PowerUp_Grant(PowerUp,PowerUpObj);
		}
	}
}

bool DAGameObjManager::Add_Weapon_Request_Event(cPlayer *Player,const WeaponDefinitionClass *Weapon) {
	SoldierGameObj *Soldier = Player->Get_GameObj();
	for (int i = 0;i < Soldier->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Soldier->Get_Observers()[i])) {
			if (!((DAGameObjObserverClass*)Soldier->Get_Observers()[i])->Add_Weapon_Request(Weapon)) {
				return false;
			}
		}
	}
	return true;
}

void DAGameObjManager::Add_Weapon_Event(cPlayer *Player,WeaponClass *Weapon) {
	SoldierGameObj *Soldier = Player->Get_GameObj();
	for (int i = 0;i < Soldier->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Soldier->Get_Observers()[i])) {
			((DAGameObjObserverClass*)Soldier->Get_Observers()[i])->Add_Weapon(Weapon);
		}
	}
}

void DAGameObjManager::Remove_Weapon_Event(cPlayer *Player,WeaponClass *Weapon) {
	SoldierGameObj *Soldier = Player->Get_GameObj();
	for (int i = 0;i < Soldier->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Soldier->Get_Observers()[i])) {
			((DAGameObjObserverClass*)Soldier->Get_Observers()[i])->Remove_Weapon(Weapon);
		}
	}
}

void DAGameObjManager::Clear_Weapons_Event(cPlayer *Player) {
	SoldierGameObj *Soldier = Player->Get_GameObj();
	for (int i = 0;i < Soldier->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Soldier->Get_Observers()[i])) {
			((DAGameObjObserverClass*)Soldier->Get_Observers()[i])->Clear_Weapons();
		}
	}
}

void DAGameObjManager::Object_Destroyed_Event(GameObject *obj) {
	for (int i = ObserversDeletePending.Count()-1;i >= 0;i--) {
		if (ObserversDeletePending[i]->Get_Owner() == obj) {
			ObserversDeletePending.Delete(i);
		}
	}
	GameObjsDeletePending.DeleteObj(obj);
	TTGameObjs.DeleteObj(obj);
	StockGameObjs.DeleteObj(obj);
	InvisibleGameObjs.DeleteObj(obj);
}
