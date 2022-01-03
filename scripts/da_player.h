/*	Renegade Scripts.dll
    Dragonade Player Manager
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAPLAYER
#define INCLUDE_DAPLAYER

#include "scripts.h"
#include "da_event.h"
#include "cPlayer.h"
#include "SoldierGameObj.h"
#include "VehicleGameObj.h"
#include "BeaconGameObj.h"
#include "BeaconGameObjDef.h"
#include "C4GameObj.h"
#include "C4GameObjDef.h"
#include "weaponmgr.h"

class DAPlayerDataFactoryClass;
class DAPlayerObserverClass;
class DAPlayerDataClass;
class cPlayer;

namespace DAPlayerFlags {
	enum Flag {
		PERSISTLEAVE = 0x1,
		PERSISTMAP = 0x2,
		THINK = 0x4,
		LEGACYTAG = 0x8
	};
};
inline DAPlayerFlags::Flag operator|(DAPlayerFlags::Flag First,DAPlayerFlags::Flag Second) {
	return (DAPlayerFlags::Flag)(+First | +Second);
}
inline DAPlayerFlags::Flag operator|=(DAPlayerFlags::Flag &First,DAPlayerFlags::Flag Second) {
	return First = (DAPlayerFlags::Flag)(+First | +Second);
}
inline DAPlayerFlags::Flag operator&=(DAPlayerFlags::Flag &First,DAPlayerFlags::Flag Second) {
	return First = (DAPlayerFlags::Flag)(+First & +Second);
}
inline DAPlayerFlags::Flag operator~(DAPlayerFlags::Flag First) {
	return (DAPlayerFlags::Flag)(~+First);
}

struct DAPlayerObserverTimerStruct {
	unsigned long StartTime;
	unsigned long Duration;
	bool Repeat;
	int Number;
	unsigned int Data;
};

class DA_API DAPlayerClass {
public:
	inline cPlayer *Get_Owner() {
		return Owner;
	}
	inline int Get_ID() {
		return Get_Owner()->Get_Id();
	}
	inline const WideStringClass &Get_Name() {
		return Get_Owner()->Get_Name();
	}
	inline SoldierGameObj *Get_GameObj() {
		return Get_Owner()->Get_GameObj();
	}
	inline int Get_Team() {
		return Get_Owner()->Get_Player_Type();
	}
	void Add_Observer(DAPlayerObserverClass *Observer);
	void Remove_Observer(const char *Name);
	DAPlayerObserverClass *Find_Observer(const char *Name);
	DAPlayerDataClass *Find_Data(const DAPlayerDataFactoryClass *Factory);
	void Update_Tag();
	void Add_Tag(const char *Tag,int Position = 0,DAPlayerFlags::Flag Flags = (DAPlayerFlags::Flag)0);
	void Remove_Tag(const char *Tag);
	void Remove_Tags_With_Flag(DAPlayerFlags::Flag Flag);
	const DynamicVectorClass<DAPlayerObserverClass*> &Get_Observers();
	void Set_Access_Level(DAAccessLevel::Level Level);
	DAAccessLevel::Level Get_Access_Level();
	void Mute(bool mute);
	bool Is_Muted();
	void Beacon_Lock(bool lock);
	bool Is_Beacon_Locked();
	void C4_Lock(bool lock);
	bool Is_C4_Locked();
	void Set_Character_Discount(float Discount);
	void Inc_Character_Discount(float Discount);
	float Get_Character_Discount();
	void Set_Vehicle_Discount(float Discount);
	void Inc_Vehicle_Discount(float Discount);
	float Get_Vehicle_Discount();
	void Set_PowerUp_Discount(float Discount);
	void Inc_PowerUp_Discount(float Discount);
	float Get_PowerUp_Discount();
	unsigned long Get_Time_Since_Last_Tib_Damage();
	void Reset_Last_Tib_Damage_Time();
	bool Is_Loaded();
	void Set_Loaded(bool Load);
	const StringClass &Get_Serial();
	void Set_Version(float Ver);
	float Get_Version();
	unsigned int Get_Revision();
	bool Is_TT_Client();
	bool Is_Scripts_Client();
	bool Is_Stock_Client();
	void Set_Server_Damage(bool Damage);
	bool Use_Server_Damage();
	bool Is_Spawning();
	unsigned long Get_Creation_Time();
	void Reset_Creation_Time();
	void Set_Needs_Team(bool Needs);
	bool Get_Needs_Team();
	
	void Join();
	void Leave();
	void Level_Loaded();
	void Player_Loaded();
	void Name_Change();
	bool Chat(TextMessageEnum Type,const wchar_t *Message,int ReceiverID);
	bool Chat_Command(TextMessageEnum Type,const StringClass &Command,const DATokenClass &Text,int ReceiverID);
	bool Radio(int PlayerType,int AnnouncementID,int IconID,AnnouncementEnum AnnouncementType);
	bool Key_Hook(const StringClass &Key);
	void Team_Change();
	int Character_Purchase_Request(float &Cost,const SoldierGameObjDef *Item);
	int Vehicle_Purchase_Request(float &Cost,const VehicleGameObjDef *Item);
	int PowerUp_Purchase_Request(float &Cost,const PowerUpGameObjDef *Item);
	int Custom_Purchase_Request(float &Cost,unsigned int ID);
	void Character_Purchase(float Cost,const SoldierGameObjDef *Item);
	void Vehicle_Purchase(float Cost,const VehicleGameObjDef *Item);
	void PowerUp_Purchase(float Cost,const PowerUpGameObjDef *Item);
	void Custom_Purchase(float Cost,unsigned int ID);
	bool Vehicle_Entry_Request(VehicleGameObj *Vehicle,int &Seat);
	void Vehicle_Enter(VehicleGameObj *Vehicle,int Seat);
	void Vehicle_Exit(VehicleGameObj *Vehicle,int Seat);
	bool PowerUp_Grant_Request(const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj);
	void PowerUp_Grant(const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj);
	bool Add_Weapon_Request(const WeaponDefinitionClass *Weapon);
	void Add_Weapon(WeaponClass *Weapon);
	void Remove_Weapon(WeaponClass *Weapon);
	void Clear_Weapons();
	bool C4_Detonate_Request(C4GameObj *C4);
	void C4_Detonate(C4GameObj *C4);
	void Change_Character(const SoldierGameObjDef *Soldier);

	void Created();
	void Destroyed();
	bool Damage_Dealt_Request(DamageableGameObj *Victim,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type);
	bool Damage_Received_Request(ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type);
	void Damage_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	void Damage_Received(ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	void Kill_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	void Kill_Received(ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	void Custom(GameObject *Sender,int Type,int Param);
	void Poked(cPlayer *Player);

	inline void Think();

private:
	struct DAPlayerTagStruct {
		void Set_Flags(DAPlayerFlags::Flag Flag);
		bool Has_Flag(DAPlayerFlags::Flag Flag);
		StringClass Tag;
		DAPlayerFlags::Flag Flags;
		int Position;
	};
	
	void Add_Data(DAPlayerDataClass *data);
	void Remove_Data(const DAPlayerDataFactoryClass *Parent);
	DAPlayerClass();
	DAPlayerClass(cPlayer *Player);
	~DAPlayerClass();
	cPlayer *Owner;
	DAAccessLevel::Level AccessLevel;
	bool Muted;
	bool C4Locked;
	bool BeaconLocked;
	unsigned int VehicleLimit;
	unsigned int C4Limit;
	float CharacterDiscount;
	float VehicleDiscount;
	float PowerUpDiscount;
	unsigned long LastTibDamageTime;
	bool Loaded;
	bool ServerDamage;
	unsigned long CreationTime;
	bool NeedsTeam;
	StringClass Serial;
	float Version;
	unsigned int Revision;
	DynamicVectorClass<DAPlayerObserverClass*> Observers;
	DynamicVectorClass<DAPlayerDataClass*> Data;
	DynamicVectorClass<DAPlayerTagStruct*> Tags;
	friend class DAPlayerManager;
};

typedef bool (DAPlayerObserverClass::*DAPOCC)(const DATokenClass &Text,TextMessageEnum ChatType);
struct DAPlayerObserverChatCommandStruct {
	DynamicVectorClass<StringClass> Triggers;
	DAPOCC Func;
	DAAccessLevel::Level AccessLevel;
	int Parameters;
	DAChatType::Type ChatType;
};

typedef void (DAPlayerObserverClass::*DAPOKH)(void);
struct DAPlayerObserverKeyHookStruct {
	DAPOKH Func;
	DynamicVectorClass<StringClass> Triggers;
};

class DA_API DAPlayerObserverClass abstract {
public:
	DAPlayerObserverClass() {
		Set_Flags((DAPlayerFlags::Flag)0);
		DeletePending = false;
	}
	inline cPlayer *Get_Owner() {
		return Owner;
	}
	inline void Set_Owner(cPlayer *Player) {
		Owner = Player;
	}
	inline DAPlayerFlags::Flag Get_Flags() {
		return Flags;
	}
	inline void Set_Flags(DAPlayerFlags::Flag Flag) {
		Flags = Flag;
	}
	inline void Add_Flag(DAPlayerFlags::Flag Flag) {
		Flags |= Flag;
	}
	inline void Remove_Flag(DAPlayerFlags::Flag Flag) {
		Flags &= ~Flag;
	}
	inline bool Has_Flag(DAPlayerFlags::Flag Flag) {
		return (Flags & Flag) == Flag;
	}
	inline int Get_ID() {
		return Get_Owner()->Get_Id();
	}
	inline const WideStringClass &Get_Name() {
		return Get_Owner()->Get_Name();
	}
	inline SoldierGameObj *Get_GameObj() {
		return Get_Owner()->Get_GameObj();
	}
	inline int Get_Team() {
		return Get_Owner()->Get_Player_Type();
	}
	
	inline DynamicVectorClass<DAPlayerObserverTimerStruct*> &Get_Timers() { 
		return Timers;
	}
	void Start_Timer(int Number,float Duration,bool Repeat = false,unsigned int Data = 0);
	void Stop_Timer(int Number,unsigned int Data = 0);
	bool Is_Timer(int Number,unsigned int Data = 0);
	void Clear_Timers();
	
	void Register_Chat_Command(DAPOCC FuncPtr,const char *Triggers,int Parameters = 0,DAAccessLevel::Level AccessLevel = DAAccessLevel::NONE,DAChatType::Type ChatType = DAChatType::ALL);
	void Unregister_Chat_Command(const char *Trigger);
	void Clear_Chat_Commands();
	inline const DynamicVectorClass<DAPlayerObserverChatCommandStruct*> &Get_Chat_Commands() {
		return ChatCommands;
	}

	void Register_Key_Hook(DAPOKH FuncPtr,const char *Triggers);
	void Unregister_Key_Hook(const char *Trigger);
	void Clear_Key_Hooks();
	inline const DynamicVectorClass<DAPlayerObserverKeyHookStruct*> &Get_Key_Hooks() {
		return KeyHooks;
	}

	inline void Set_Delete_Pending() {
		DeletePending = true;
	}
	inline bool Is_Delete_Pending() {
		return DeletePending;
	}
	
	virtual ~DAPlayerObserverClass() {
		Clear_Timers();
		Clear_Chat_Commands();
		Clear_Key_Hooks();
	}

	virtual const char *Get_Observer_Name() = 0;
	
	virtual void Init() { } //Called when the observer is attached to a player.

	virtual void Join() { } //Called when the player rejoins after leaving.
	virtual void Leave() { } //Called when the player leaves the game.
	virtual void Level_Loaded() { } //Called when a new level is loaded.
	virtual void Player_Loaded() { } //Called after the player has loaded the map and spawned.
	virtual void Name_Change() { } //Called after the player's name is changed for fake color messaging.
	virtual void Team_Change() { } //Called when the player changes teams.
	virtual int Character_Purchase_Request(float &Cost,const SoldierGameObjDef *Item) { return -1; } //Called when the player tries to buy a character.
	virtual int Vehicle_Purchase_Request(float &Cost,const VehicleGameObjDef *Item) { return -1; } //Called when the player tries to buy a vehicle.
	virtual int PowerUp_Purchase_Request(float &Cost,const PowerUpGameObjDef *Item) { return -1; } //Called when the player tries to buy a powerup.
	virtual int Custom_Purchase_Request(float &Cost,unsigned int ID) { return -1; } //Called when the player tries to buy a custom object.
	virtual void Character_Purchase(float Cost,const SoldierGameObjDef *Item) { } //Called after the player has bought a character.
	virtual void Vehicle_Purchase(float Cost,const VehicleGameObjDef *Item) { } //Called after the player has bought a vehicle.
	virtual void PowerUp_Purchase(float Cost,const PowerUpGameObjDef *Item) { } //Called after the player has bought a powerup.
	virtual void Custom_Purchase(float Cost,unsigned int ID) { } //Called after the player has bought a custom object.
	virtual bool Vehicle_Entry_Request(VehicleGameObj *Vehicle,int &Seat) { return true; } //Called when tries to enter a vehicle.
	virtual void Vehicle_Enter(VehicleGameObj *Vehicle,int Seat) { } //Called when the player enters a vehicle.
	virtual void Vehicle_Exit(VehicleGameObj *Vehicle,int Seat) { } //Called when the player exits a vehicle.
	virtual bool PowerUp_Grant_Request(const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) { return true; } //Called when something tries to grant a powerup to the player.
	virtual void PowerUp_Grant(const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) { } //Called when the player is granted a powerup.
	virtual bool Add_Weapon_Request(const WeaponDefinitionClass *Weapon) { return true; } //Called when something tries to grant the player a weapon.
	virtual void Add_Weapon(WeaponClass *Weapon) { } //Called when the player is granted a weapon.
	virtual void Remove_Weapon(WeaponClass *Weapon) { } //Called when a weapon is removed from the player.
	virtual void Clear_Weapons() { } //Called when the player's weapons are cleared.
	virtual bool C4_Detonate_Request(C4GameObj *C4) { return true; } //Called when the player tries to trigger a proximity C4.
	virtual void C4_Detonate(C4GameObj *C4) { } //Called when the player triggers a proximity C4.
	virtual void Change_Character(const SoldierGameObjDef *Soldier) { } //Called before the player changes characters.

	virtual void Created() { } //Called when the player changes to a new gameobject.
	virtual void Destroyed() { } //Called when the player's gameobject is destroyed.
	virtual bool Damage_Dealt_Request(DamageableGameObj *Victim,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type) { return true; } //Called when the player tries to damage an object. Damage can be changed by modifying the OffenseObjectClass.
	virtual bool Damage_Received_Request(ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type) { return true; } //Called when an object tries to damage the player. Damage can be changed by modifying the OffenseObjectClass.
	virtual void Damage_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) { } //Called when the player damages an object.
	virtual void Damage_Received(ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) { } //Called when an object damages the player.
	virtual void Kill_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) { } //Called when the player kills an object.
	virtual void Kill_Received(ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) { } //Called when an object kills the player.
	virtual void Custom(GameObject *Sender,int Type,int Param) { } //Called when the player receives a custom.
	virtual void Poked(cPlayer *Player) { } //Called when the player is poked.
	
	virtual void Timer_Expired(int Number,unsigned int Data) { } //Callback for Start_Timer.
	virtual void Think() { } //Called on each frame. Requires THINK flag.
	
private:
	cPlayer *Owner;
	DAPlayerFlags::Flag Flags;
	DynamicVectorClass<DAPlayerObserverTimerStruct*> Timers;
	DynamicVectorClass<DAPlayerObserverChatCommandStruct*> ChatCommands;
	DynamicVectorClass<DAPlayerObserverKeyHookStruct*> KeyHooks;
	bool DeletePending;
};

class DA_API DAPlayerDataFactoryClass abstract {
public:
	virtual ~DAPlayerDataFactoryClass() { }
	virtual DAPlayerDataClass *Create_Data() = 0;
};

class DA_API DAPlayerDataClass abstract {
public:
	virtual ~DAPlayerDataClass() { }
	inline cPlayer *Get_Owner() {
		return Owner;
	}
	inline void Set_Owner(cPlayer *Player) {
		Owner = Player;
	}
	inline const DAPlayerDataFactoryClass *Get_Factory() {
		return Factory;
	}
	inline void Set_Factory(const DAPlayerDataFactoryClass *Fac) {
		Factory = Fac;
	}
	inline int Get_ID() {
		return Get_Owner()->Get_Id();
	}
	inline const WideStringClass &Get_Name() {
		return Get_Owner()->Get_Name();
	}
	inline SoldierGameObj *Get_GameObj() {
		return Get_Owner()->Get_GameObj();
	}
	inline int Get_Team() {
		return Get_Owner()->Get_Player_Type();
	}
	virtual void Init() { };
	virtual void Clear_Level() { };
	virtual void Clear_Session() { };
	
private:
	cPlayer *Owner;
	const DAPlayerDataFactoryClass *Factory;
};

template <class T> class DAPlayerDataManagerClass : public DAPlayerDataFactoryClass {
public:
	virtual ~DAPlayerDataManagerClass() {
		DAPlayerManager::Remove_Data_Factory(this);
	}
	inline T *Get_Player_Data(int ID) const {
		cPlayer *Player = Find_Player(ID);
		if (Player) {
			return static_cast<T*>(Player->Get_DA_Player()->Find_Data(this));
		}
		return 0;
	}
	inline T *Get_Player_Data(GameObject *obj) const {
		if (Is_Player(obj)) {
			return static_cast<T*>(((SoldierGameObj*)obj)->Get_Player()->Get_DA_Player()->Find_Data(this));
		}
		return 0;
	}
	inline T *Get_Player_Data(cPlayer *Player) const {
		return static_cast<T*>(Player->Get_DA_Player()->Find_Data(this));
	}
	inline T *Get_Player_Data(DAPlayerClass *Player) const {
		return static_cast<T*>(Player->Find_Data(this));
	}

protected:
	inline void Init() {
		DAPlayerManager::Add_Data_Factory(this);
	}
	
private:
	virtual DAPlayerDataClass *Create_Data() {
		DAPlayerDataClass *Data = new T;
		Data->Set_Factory(this);
		return Data;
	}
};

class DAPlayerManager : public DAEventClass {
public:
	static void Init();
	static void Shutdown();
	DA_API static void Add_Data_Factory(DAPlayerDataFactoryClass *Factory);
	DA_API static void Remove_Data_Factory(DAPlayerDataFactoryClass *Factory);
	DA_API static bool Get_Disable_Kill_Messages();
	DA_API static bool Get_Disable_Kill_Counter();
	DA_API static bool Get_Disable_Team_Kill_Counter();
	DA_API static bool Get_Disable_Death_Counter();
	DA_API static bool Get_Disable_Team_Death_Counter();
	DA_API static bool Get_Disable_Damage_Points();
	DA_API static bool Get_Disable_Death_Points();
	DA_API static bool Get_Disable_Team_Score_Counter();
	DA_API static float Get_Credits_Multiplier();
	DA_API static void Set_Disable_Kill_Messages(bool Enable);
	DA_API static void Set_Disable_Kill_Counter(bool Disable);
	DA_API static void Set_Disable_Team_Kill_Counter(bool Disable);
	DA_API static void Set_Disable_Death_Counter(bool Disable);
	DA_API static void Set_Disable_Team_Death_Counter(bool Disable);
	DA_API static void Set_Disable_Damage_Points(bool Disable);
	DA_API static void Set_Disable_Death_Points(bool Disable);
	DA_API static void Set_Disable_Team_Score_Counter(bool Disable);
	DA_API static void Set_Credits_Multiplier(float Multiplier);
	
private:
	static inline bool Check_Player(DAPlayerClass *DAPlayer);
	virtual bool Chat_Event(cPlayer *Player,TextMessageEnum Type,const wchar_t *Message,int ReceiverID);
	virtual bool Radio_Event(cPlayer *Player,int PlayerType,int AnnouncementID,int IconID,AnnouncementEnum AnnouncementType);
	virtual void Level_Loaded_Event();
	virtual void Settings_Loaded_Event();
	virtual ConnectionAcceptanceFilter::STATUS Connection_Request_Event(ConnectionRequest &Request,WideStringClass &RefusalMessage);
	virtual void Player_Join_Event(cPlayer *Player);
	virtual void Player_Leave_Event(cPlayer *Player);
	virtual void Player_Loaded_Event(cPlayer *Player);
	virtual void Name_Change_Event(cPlayer *Player);
	virtual int Character_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const SoldierGameObjDef *Item);
	virtual int Vehicle_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const VehicleGameObjDef *Item);
	virtual int PowerUp_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const PowerUpGameObjDef *Item);
	virtual int Custom_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,unsigned int ID);
	virtual void Character_Purchase_Event(cPlayer *Player,float Cost,const SoldierGameObjDef *Item);
	virtual void Vehicle_Purchase_Event(cPlayer *Player,float Cost,const VehicleGameObjDef *Item);
	virtual void PowerUp_Purchase_Event(cPlayer *Player,float Cost,const PowerUpGameObjDef *Item);
	virtual void Custom_Purchase_Event(cPlayer *Player,float Cost,unsigned int ID);
	virtual void Team_Change_Event(cPlayer *Player);
	virtual bool Suicide_Event(cPlayer *Player);
	virtual bool Vehicle_Entry_Request_Event(VehicleGameObj *Vehicle,cPlayer *Player,int &Seat);
	virtual void Vehicle_Enter_Event(VehicleGameObj *Vehicle,cPlayer *Player,int Seat);
	virtual void Vehicle_Exit_Event(VehicleGameObj *Vehicle,cPlayer *Player,int Seat);
	virtual bool PowerUp_Grant_Request_Event(cPlayer *Player,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj);
	virtual void PowerUp_Grant_Event(cPlayer *Player,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj);
	virtual bool Add_Weapon_Request_Event(cPlayer *Player,const WeaponDefinitionClass *Weapon);
	virtual void Add_Weapon_Event(cPlayer *Player,WeaponClass *Weapon);
	virtual void Remove_Weapon_Event(cPlayer *Player,WeaponClass *Weapon);
	virtual void Clear_Weapons_Event(cPlayer *Player);
	virtual bool C4_Detonate_Request_Event(C4GameObj *C4,SmartGameObj *Triggerer);
	virtual void C4_Detonate_Event(C4GameObj *C4,SmartGameObj *Triggerer);
	virtual void Change_Character_Event(cPlayer *Player,const SoldierGameObjDef *Soldier);
	virtual void Think();
	
	virtual void Object_Created_Event(GameObject *obj);
	virtual void Object_Destroyed_Event(GameObject *obj);
	virtual bool Damage_Request_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type);
	virtual void Damage_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	virtual void Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	virtual void Custom_Event(GameObject *obj,int Type,int Param,GameObject *Sender);
	virtual void Poke_Event(cPlayer *Player,PhysicalGameObj *obj);
	
	static DynamicVectorClass<DAPlayerDataFactoryClass*> DataFactories;
	static DynamicVectorClass<DAPlayerClass*> Players;

	//Settings
	WideStringClass DisallowedNickFirstCharacters;
	WideStringClass DisallowedNickCharacters;
	DynamicVectorClass<WideStringClass> DisallowedNicks;
	unsigned int ForceTT;
	unsigned int TTRevision;
	static bool DisableKillMessages;
	static bool DisableKillCounter;
	static bool DisableTeamKillCounter;
	static bool DisableDeathCounter;
	static bool DisableTeamDeathCounter;
	static bool DisableDamagePoints;
	static bool DisableDeathPoints;
	static bool DisableTeamScoreCounter;
	static float CreditsMultiplier;
};

#endif