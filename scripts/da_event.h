/*	Renegade Scripts.dll
    Dragonade Event Manager
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAEVENT
#define INCLUDE_DAEVENT

#include "scripts.h"
#include "engine_ttdef.h"
#include "BaseControllerClass.h"
#include "PhysicalGameObj.h"
#include "SimpleGameObj.h"
#include "SoldierGameObj.h"
#include "SoldierGameObjDef.h."
#include "VehicleGameObj.h"
#include "VehicleGameObjDef.h"
#include "VehicleFactoryGameObj.h"
#include "PowerupGameObj.h"
#include "PowerupGameObjDef.h"
#include "C4GameObj.h"
#include "C4GameObjDef.h"
#include "BeaconGameObj.h"
#include "BeaconGameObjDef.h"
#include "PhysicalGameObj.h"
#include "ScriptableGameObj.h"
#include "WeaponBagClass.h"
#include "WeaponClass.h"
#include "cPlayer.h"
#include "OffenseObjectClass.h"
#include "DefenseObjectClass.h"
#include "ConnectionRequest.h"
#include "ConnectionAcceptanceFilter.h"
#include "da_string.h"
#include "da_token.h"

class TransitionInstanceClass;
class DAEventManager;
class DAEventClass;
enum TextMessageEnum;

namespace DAChatType {
	enum Type {
		ALL = 0,
		PUBLIC = 0x1,
		TEAM = 0x2,
		PRIVATE = 0x4,
		TMSG = 0x8
	};
};
inline DAChatType::Type operator|(DAChatType::Type First,DAChatType::Type Second) {
	return (DAChatType::Type)(+First | +Second);
}

namespace DAAccessLevel {
	enum Level {
		NONE,
		VIP,
		TEMPMODERATOR,
		HALFMODERATOR,
		FULLMODERATOR,
		SUPERMODERATOR, //Generic name for Officer or Division Leader.
		ADMINISTRATOR
	};
};

namespace DAEvent {
	 enum Type {
		SETTINGSLOADED,
		CHAT,
		CHATCOMMAND,
		KEYHOOK,
		HOSTCHAT,
		RADIO,
		CONNECTIONREQUEST,
		PLAYERPREJOIN,
		PLAYERJOIN,
		PLAYERLEAVE,
		PLAYERLOADED,
		NAMECHANGE,
		LEVELLOADED,
		REMIX,
		REBALANCE,
		SWAP,
		GAMEOVER,
		CONSOLEOUTPUT,
		RENLOG,
		DALOG,
		VEHICLEPURCHASEREQUEST,
		CHARACTERPURCHASEREQUEST,
		POWERUPPURCHASEREQUEST,
		CUSTOMPURCHASEREQUEST,
		VEHICLEPURCHASE,
		CHARACTERPURCHASE,
		POWERUPPURCHASE,
		CUSTOMPURCHASE,
		REFILL,
		SUICIDE,
		TEAMCHANGEREQUEST,
		TEAMCHANGE,
		VEHICLEENTRYREQUEST,
		VEHICLEENTER,
		VEHICLEEXIT,
		POWERUPGRANTREQUEST,
		POWERUPGRANT,
		ADDWEAPONREQUEST,
		ADDWEAPON,
		REMOVEWEAPON,
		CLEARWEAPONS,
		BEACONDEPLOY,
		BEACONDETONATE,
		C4DETONATEREQUEST,
		C4DETONATE,
		CHANGECHARACTER,
		VEHICLEFLIP,
		REQUESTVEHICLE,
		THINK,
		MAX
	};
};

struct DAEventStruct {
	DAEventClass *Base;
	int Priority;
};

namespace DAObjectEvent {
	enum Type {
		CREATED,
		DESTROYED,
		KILLDEALT, //These allow you to choose whether ObjectType applies to the object being killed or the one doing the killing.
		KILLRECEIVED,
		STOCKCLIENTDAMAGEREQUEST,
		TTCLIENTDAMAGEREQUEST,
		DAMAGEDEALTREQUEST,
		DAMAGERECEIVEDREQUEST,
		DAMAGEDEALT,
		DAMAGERECEIVED,
		CUSTOM,
		POKE,
		ZONEENTER,
		ZONEEXIT,
		MAX
	};
	enum ObjectType {
		ALL = 0x1,
		PLAYER = 0x2,
		DAMAGEABLE = 0x4,
		SOLDIER = 0x8,
		VEHICLE = 0x10,
		C4 = 0x20,
		BEACON = 0x40,
		BUILDING = 0x80,
		ARMED = 0x160,
		POWERUP = 0x320,
		SIMPLE = 0x640
	};
};
inline DAObjectEvent::ObjectType operator|(DAObjectEvent::ObjectType First,DAObjectEvent::ObjectType Second) {
	return (DAObjectEvent::ObjectType)(+First | +Second);
}

struct DAObjectEventStruct {
	inline bool Has_Flag(DAObjectEvent::ObjectType Flag) {
		return (ObjectType & Flag) == Flag;
	}
	bool Check_Object_Type(GameObject *obj) {
		if (!obj) {
			return false;
		}
		else if (Has_Flag(DAObjectEvent::ALL)) {
			return true;
		}
		else if (obj->As_DamageableGameObj()) {
			if (Has_Flag(DAObjectEvent::DAMAGEABLE)) {
				return true;
			}
			else if (obj->As_BuildingGameObj()) {
				if (Has_Flag(DAObjectEvent::BUILDING)) {
					return true;
				}
			}
			else if (PhysicalGameObj *Physical = obj->As_PhysicalGameObj()) {
				if (Physical->As_ArmedGameObj()) {
					if (Has_Flag(DAObjectEvent::ARMED)) {
						return true;
					}
					else if (obj->As_SoldierGameObj()) {
						if (Has_Flag(DAObjectEvent::SOLDIER)) {
							return true;
						}
						else if (obj->As_SoldierGameObj()->Get_Player_Data() && Has_Flag(DAObjectEvent::PLAYER)) {
							return true;
						}
					}
					else if (obj->As_VehicleGameObj()) {
						if (Has_Flag(DAObjectEvent::VEHICLE)) {
							return true;
						}
					}
				}
				else if (SimpleGameObj *Simple = Physical->As_SimpleGameObj()) {
					if (Has_Flag(DAObjectEvent::SIMPLE)) {
						return true;
					}
					else if (Simple->As_BeaconGameObj()) {
						if (Has_Flag(DAObjectEvent::BEACON)) {
							return true;
						}
					}
					else if (Simple->As_C4GameObj()) {
						if (Has_Flag(DAObjectEvent::C4)) {
							return true;
						}
					}
					else if (Simple->As_PowerUpGameObj()) {
						if (Has_Flag(DAObjectEvent::POWERUP)) {
							return true;
						}
					}
				}
			}
		}
		return false;
	}
	bool operator==(const DAObjectEventStruct &That) {
		return (Base == That.Base && ObjectType == That.ObjectType);
	}
	bool operator!=(const DAObjectEventStruct &That) {
		return (Base != That.Base || ObjectType != That.ObjectType);
	}
	DAEventClass *Base;
	DAObjectEvent::ObjectType ObjectType;
	int Priority;
};

struct DAEventTimerStruct {
	DAEventClass *Base;
	unsigned long StartTime;
	unsigned long Duration;
	bool Repeat;
	int Number;
	unsigned int Data;
};

typedef bool (DAEventClass::*DAECC)(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType); //DA Event Chat Command
typedef void (DAEventClass::*DAEKH)(cPlayer *Player); //DA Event Key Hook

namespace DADamageType {
	enum Type {
		NONE,
		NORMAL,
		NECKSHOT,
		HEADSHOT,
		EXPLOSION,
		SPLASH,
		BURN,
		SQUISH,
		FALL,
		TIBERIUM,
		FLIP,
		REPAIR
	};
};

class DA_API DAEventManager {
public:
	static void Init();
	static void Settings_Loaded_Event();
	static bool Chat_Event(cPlayer *Player,TextMessageEnum Type,const wchar_t *Message,int ReceiverID);
	static bool Chat_Command_Event(cPlayer *Player,TextMessageEnum Type,const StringClass &Command,const DATokenClass &Text,int ReceiverID);
	static bool Key_Hook_Event(cPlayer *Player,const StringClass &Key);
	static bool Host_Chat_Event(int ID,TextMessageEnum Type,const char *Message);
	static bool Radio_Event(int PlayerType,int ID,int AnnouncementID,int IconID,AnnouncementEnum AnnouncementType);
	static void Player_Join_Event(int ID,const char *Name);
	static void Player_Leave_Event(int ID);
	static void Name_Change_Event(cPlayer *Player);
	static void Level_Loaded_Event();
	static void Remix_Event();
	static void Rebalance_Event();
	static void Swap_Event();
	static void Game_Over_Event();
	static void Console_Output_Event(const char *Output);
	static void Ren_Log_Event(const char *Output);
	static void DA_Log_Event(const char *Header,const char *Output);
	static int Character_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Soldier,unsigned int Cost,unsigned int Preset,const char *Data);
	static int Vehicle_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Soldier,unsigned int Cost,unsigned int Preset,const char *Data);
	static int PowerUp_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Soldier,unsigned int Cost,unsigned int Preset,const char *Data);
	static int Custom_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Soldier,unsigned int Cost,unsigned int Preset);
	static void Character_Purchase_Event(cPlayer *Player,float Cost,const SoldierGameObjDef *Def);
	static void Vehicle_Purchase_Event(cPlayer *Player,float Cost,const VehicleGameObjDef *Def);
	static void PowerUp_Purchase_Event(cPlayer *Player,float Cost,const PowerUpGameObjDef *Def);
	static void Custom_Purchase_Event(cPlayer *Player,float Cost,unsigned int ID);
	static bool Refill_Event(SoldierGameObj *Soldier);
	static void Suicide_Event(int ID);
	static void Team_Change_Request_Event(int ID);

	static bool Vehicle_Entry_Request_Event(VehicleGameObj *Vehicle,cPlayer *Player,int &Seat);
	static bool PowerUp_Grant_Request_Event(SmartGameObj *Grantee,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj);
	static void PowerUp_Grant_Event(SmartGameObj *Grantee,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj);
	static bool Add_Weapon_Request_Event(WeaponBagClass *Bag,const WeaponDefinitionClass *Weapon);
	static void Add_Weapon_Event(WeaponBagClass *Bag,WeaponClass *Weapon);
	static void Remove_Weapon_Event(WeaponBagClass *Bag,int Index);
	static void Clear_Weapons_Event(WeaponBagClass *Bag);
	static void Beacon_Set_State_Event(BeaconGameObj *Beacon);
	static bool C4_Detonate_Request_Event(C4GameObj *C4);
	static void C4_Detonate_Event(C4GameObj *C4);
	static void Soldier_Re_Init_Event(SoldierGameObj *Soldier,const SoldierGameObjDef *SoldierDef);
	static bool Request_Vehicle_Event(VehicleFactoryGameObj *Factory,unsigned int Vehicle,SoldierGameObj *Owner,float Delay);
	static void Think();

	static void Object_Created_Event(void *Data,GameObject *obj);
	static bool Stock_Client_Damage_Request_Event(PhysicalGameObj *Damager,PhysicalGameObj *Target,float Damage, uint Warhead);
	static bool TT_Client_Damage_Request_Event(PhysicalGameObj *Damager, PhysicalGameObj *Target,const AmmoDefinitionClass* Ammo,const char* Bone);
	static bool Damage_Request_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type);
	static void Damage_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	static void Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	static bool Vehicle_Flip_Event(VehicleGameObj *Vehicle);
	
	class DAEventObserverClass : public GameObjObserverClass {
		virtual void Detach(GameObject *obj);
		virtual void Custom(GameObject *obj,int Message,int Param,GameObject *Sender);
		virtual void Poked(GameObject *obj,GameObject *Poker);
		virtual void Entered(GameObject *obj,GameObject *Enterer);
		virtual void Exited(GameObject *obj,GameObject *Exiter);
		virtual void Destroyed(GameObject *obj);
		virtual const char *Get_Name() {
			return "DAEventObserverClass";
		}

		//Unused
		virtual void Created(GameObject *obj) { }
		virtual void Attach(GameObject *obj) { }
		virtual void Damaged(GameObject *obj,GameObject *Damager,float Damage) { }
		virtual void Killed(GameObject *obj,GameObject *Killer) { }
		virtual void Sound_Heard(GameObject *obj,const CombatSound &Sound) { }
		virtual void Enemy_Seen(GameObject *obj,GameObject *Enemy) { }
		virtual void Action_Complete(GameObject *obj,int ActionID,ActionCompleteReason CompleteReason) { }
		virtual void Timer_Expired(GameObject *obj,int Number) { }
		virtual void Animation_Complete(GameObject *obj,const char *Name) { }
	};

	class DAEventConnectionAcceptanceFilterClass : public ConnectionAcceptanceFilter {
		virtual void handleInitiation(const ConnectionRequest &Request);
		virtual void handleTermination(const ConnectionRequest &Request) { }
		virtual void handleCancellation(const ConnectionRequest &Request) { }
		virtual STATUS getStatus(const ConnectionRequest &Request,WideStringClass &RefusalMessage);
	};
	
	static void Register_Event(DAEventClass *Base,DAEvent::Type Type,int Priority);
	static void Unregister_Event(DAEventClass *Base,DAEvent::Type Type);
	static void Clear_Events(DAEventClass *Base);
	
	static void Register_Object_Event(DAEventClass *Base,DAObjectEvent::Type Type,DAObjectEvent::ObjectType ObjectType,int Priority);
	static void Unregister_Object_Event(DAEventClass *Base,DAObjectEvent::Type Type);
	static void Clear_Object_Events(DAEventClass *Base);
	
	static void Start_Timer(DAEventClass *Base,int Number,float Duration,bool Repeat,unsigned int Data);
	static void Stop_Timer(DAEventClass *Base,int Number,unsigned int Data);
	static bool Is_Timer(DAEventClass *Base,int Number,unsigned int Data);
	static void Clear_Timers(DAEventClass *Base);
	
private:
	static DynamicVectorClass<DAEventStruct*> Events[DAEvent::MAX];
	static DynamicVectorClass<DAObjectEventStruct*> ObjectEvents[DAObjectEvent::MAX];
	static DynamicVectorClass<DAEventTimerStruct*> Timers;
	static bool IsSoldierReInit;
};

class DA_API DAEventClass abstract {
public:
	inline void Register_Event(DAEvent::Type Type,int Priority = 0) {
		DAEventManager::Register_Event(this,Type,Priority);
	}
	inline void Unregister_Event(DAEvent::Type Type) {
		DAEventManager::Unregister_Event(this,Type);
	}
	inline void Clear_Events() {
		DAEventManager::Clear_Events(this);
	}
	inline void Register_Object_Event(DAObjectEvent::Type Type,DAObjectEvent::ObjectType ObjectType,int Priority = 0) {
		DAEventManager::Register_Object_Event(this,Type,ObjectType,Priority);
	}
	inline void Unregister_Object_Event(DAObjectEvent::Type Type) {
		DAEventManager::Unregister_Object_Event(this,Type);
	}
	inline void Clear_Object_Events() {
		DAEventManager::Clear_Object_Events(this);
	}
	inline void Start_Timer(int Number,float Duration,bool Repeat = false,unsigned int Data = 0) {
		DAEventManager::Start_Timer(this,Number,Duration,Repeat,Data);
	}
	inline void Stop_Timer(int Number,unsigned int Data = 0) {
		DAEventManager::Stop_Timer(this,Number,Data);
	}
	inline bool Is_Timer(int Number,unsigned int Data = 0) {
		return DAEventManager::Is_Timer(this,Number,Data);
	}
	inline void Clear_Timers() {
		DAEventManager::Clear_Timers(this);
	}
	void Register_Chat_Command(DAECC Func,const char *Triggers,int Parameters = 0,DAAccessLevel::Level AccessLevel = DAAccessLevel::NONE,DAChatType::Type ChatType = DAChatType::ALL);
	void Unregister_Chat_Command(const char *Trigger);
	void Clear_Chat_Commands();
	void Register_Key_Hook(DAEKH Func,const char *Triggers);
	void Unregister_Key_Hook(const char *Trigger);
	void Clear_Key_Hooks();
	
	virtual void Settings_Loaded_Event() { }
	virtual bool Chat_Event(cPlayer *Player,TextMessageEnum Type,const wchar_t *Message,int ReceiverID) { return true; }
	virtual bool Chat_Command_Event(cPlayer *Player,TextMessageEnum Type,const StringClass &Command,const DATokenClass &Text,int ReceiverID) { return true; }
	virtual bool Key_Hook_Event(cPlayer *Player,const StringClass &Key) { return true; }
	virtual bool Host_Chat_Event(int ID,TextMessageEnum Type,const char *Message) { return true; }
	virtual bool Radio_Event(cPlayer *Player,int PlayerType,int AnnouncementID,int IconID,AnnouncementEnum AnnouncementType) { return true; }
	virtual ConnectionAcceptanceFilter::STATUS Connection_Request_Event(ConnectionRequest &Request,WideStringClass &RefusalMessage) { return ConnectionAcceptanceFilter::STATUS_ACCEPTING; }
	virtual void Player_Pre_Join_Event(ConnectionRequest &Request) { }
	virtual void Player_Join_Event(cPlayer *Player) { }
	virtual void Player_Leave_Event(cPlayer *Player) { }
	virtual void Player_Loaded_Event(cPlayer *Player) { }
	virtual void Name_Change_Event(cPlayer *Player) { }
	virtual void Level_Loaded_Event() { }
	virtual void Remix_Event() { }
	virtual void Rebalance_Event() { }
	virtual void Swap_Event() { }
	virtual void Game_Over_Event() { }
	virtual void Console_Output_Event(const char *Output) { }
	virtual void Ren_Log_Event(const char *Output) { }
	virtual void DA_Log_Event(const char *Header,const char *Output) { }
	virtual int Character_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const SoldierGameObjDef *Item) { return -1; }
	virtual int Vehicle_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const VehicleGameObjDef *Item) { return -1; }
	virtual int PowerUp_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const PowerUpGameObjDef *Item) { return -1; }
	virtual int Custom_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,unsigned int ID) { return -1; }
	virtual void Character_Purchase_Event(cPlayer *Player,float Cost,const SoldierGameObjDef *Item) { }
	virtual void Vehicle_Purchase_Event(cPlayer *Player,float Cost,const VehicleGameObjDef *Item) { }
	virtual void PowerUp_Purchase_Event(cPlayer *Player,float Cost,const PowerUpGameObjDef *Item) { }
	virtual void Custom_Purchase_Event(cPlayer *Player,float Cost,unsigned int ID) { }
	virtual bool Refill_Event(cPlayer *Player) { return true; }
	virtual bool Suicide_Event(cPlayer *Player) { return true; }
	virtual bool Team_Change_Request_Event(cPlayer *Player) { return true; }
	virtual void Team_Change_Event(cPlayer *Player) { }
	virtual bool Vehicle_Entry_Request_Event(VehicleGameObj *Vehicle,cPlayer *Player,int &Seat) { return true; }
	virtual void Vehicle_Enter_Event(VehicleGameObj *Vehicle,cPlayer *Player,int Seat) { }
	virtual void Vehicle_Exit_Event(VehicleGameObj *Vehicle,cPlayer *Player,int Seat) { }
	virtual bool PowerUp_Grant_Request_Event(cPlayer *Player,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) { return true; }
	virtual void PowerUp_Grant_Event(cPlayer *Player,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) { }
	virtual bool Add_Weapon_Request_Event(cPlayer *Player,const WeaponDefinitionClass *Weapon) { return true; }
	virtual void Add_Weapon_Event(cPlayer *Player,WeaponClass *Weapon) { }
	virtual void Remove_Weapon_Event(cPlayer *Player,WeaponClass *Weapon) { }
	virtual void Clear_Weapons_Event(cPlayer *Player) { }
	virtual void Beacon_Deploy_Event(BeaconGameObj *Beacon) { }
	virtual void Beacon_Detonate_Event(BeaconGameObj *Beacon) { }
	virtual bool C4_Detonate_Request_Event(C4GameObj *C4,SmartGameObj *Triggerer) { return true; }
	virtual void C4_Detonate_Event(C4GameObj *C4) { }
	virtual void Change_Character_Event(cPlayer *Player,const SoldierGameObjDef *Soldier) { }
	virtual bool Vehicle_Flip_Event(VehicleGameObj *Vehicle) { return true; }
	virtual bool Request_Vehicle_Event(VehicleFactoryGameObj *Factory,const VehicleGameObjDef *Vehicle,cPlayer *Player,float Delay,SoldierGameObj *Owner) { return true; }
	virtual void Think() { }
	
	virtual void Object_Created_Event(GameObject *obj) { }
	virtual bool Stock_Client_Damage_Request_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float Damage,uint Warhead) { return true; }
	virtual bool TT_Client_Damage_Request_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,const AmmoDefinitionClass *Ammo,const char *Bone) { return true; }
	virtual bool Damage_Request_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type) { return true; }
	virtual void Damage_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type){ }
	virtual void Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) { }
	virtual void Custom_Event(GameObject *obj,int Type,int Param,GameObject *Sender) { }
	virtual void Poke_Event(cPlayer *Player,PhysicalGameObj *obj) { }
	virtual void Zone_Enter_Event(ScriptZoneGameObj *obj,PhysicalGameObj *Enterer) { }
	virtual void Zone_Exit_Event(ScriptZoneGameObj *obj,PhysicalGameObj *Exiter) { }
	virtual void Object_Destroyed_Event(GameObject *obj) { }
	
	virtual void Timer_Expired(int Number,unsigned int Data) { }

	virtual ~DAEventClass() {
		Clear_Events();
		Clear_Object_Events();
		Clear_Timers();
		Clear_Chat_Commands();
		Clear_Key_Hooks();
	}
};

template<class T> class DA_API DASingleton {
public:
	static inline T *Create_Instance() {
		if (!Instance) {
			Instance = new T;
		}
		return Instance;
	}
	static inline void Destroy_Instance() {
		if (Instance) {
			delete Instance;
			Instance = 0;
		}
	}
	static inline T *Get_Instance() {
		return Instance;
	}
private:
	static T *Instance;
};

#endif