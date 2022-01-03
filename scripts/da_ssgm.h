/*	Renegade Scripts.dll
    Dragonade Legacy SSGM Interface
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DASSGM
#define INCLUDE_DASSGM

#include "da_event.h"
#include "engine_tt.h"

class DA_API SSGMGameLog {
public:
	static void Init(unsigned short port) { }
	static void Log_Message(const char *message,const char *category);
	static void Log_Gamelog(const char *format,...);
	static void Log_RenLog(const char *message);
	static void Log_Custom(int id,const char *format,...);
	static void Send_Console(const char *message);
	static void Shutdown();
	static void Think();
	static void Send(const char *data);
};

SCRIPTS_API void Log_Killed_Message(GameObject* obj, GameObject* killer, const char* objectType);

enum EventType {
	EVENT_GLOBAL_INI = 0,
	EVENT_MAP_INI,
	EVENT_CHAT_HOOK,
	EVENT_OBJECT_CREATE_HOOK,
	EVENT_LOAD_LEVEL_HOOK,
	EVENT_GAME_OVER_HOOK,
	EVENT_PLAYER_JOIN_HOOK,
	EVENT_PLAYER_LEAVE_HOOK,
	EVENT_REFILL_HOOK,
	EVENT_POWERUP_PURCHASE_HOOK,
	EVENT_VEHICLE_PURCHASE_HOOK,
	EVENT_CHARACTER_PURCHASE_HOOK,
	EVENT_THINK_HOOK,
	EVENT_RADIO_HOOK,
	EVENT_STOCK_DAMAGE_HOOK,
	EVENT_TT_DAMAGE_HOOK,
	EVENT_PRE_LOAD_LEVEL_HOOK,
	EVENT_COUNT
};

class Plugin { //This is the legacy interface for SSGM plugins. When making a new plugin you should base it on DAEventClass instead.
public:
	virtual ~Plugin() {};
	virtual float Get_Version() { return 0.0f; }
	virtual void OnLoadGlobalINISettings(INIClass *SSGMIni) { };
	virtual void OnFreeData() { };
	virtual void OnLoadMapINISettings(INIClass *SSGMIni) { };
	virtual void OnFreeMapData() { };
	virtual bool OnChat(int PlayerID,TextMessageEnum Type,const wchar_t *Message,int recieverID) { return true; };
	virtual void OnObjectCreate(void *data,GameObject *obj) { };
	virtual void OnLoadLevel() { };
	virtual void OnGameOver() { };
	virtual void OnPlayerJoin(int PlayerID,const char *PlayerName) { };
	virtual void OnPlayerLeave(int PlayerID) { };
	virtual bool OnRefill(GameObject *purchaser) { return true; };
	virtual int OnPowerupPurchase(BaseControllerClass *base,GameObject *Purchaser,unsigned int cost,unsigned int preset,const char *data) { return -1; };
	virtual int OnVehiclePurchase(BaseControllerClass *base,GameObject *Purchaser,unsigned int cost,unsigned int preset,const char *data) { return -1; };
	virtual int OnCharacterPurchase(BaseControllerClass *base,GameObject *Purchaser,unsigned int cost,unsigned int preset,const char *data) { return -1; };
	virtual void OnThink() { };
	virtual bool OnRadioCommand(int PlayerType, int PlayerID, int AnnouncementID, int IconID, AnnouncementEnum AnnouncementType) { return true; }
	virtual bool OnStockDamage(PhysicalGameObj* damager, PhysicalGameObj* target, float damage, uint warheadId) { return true; }
	virtual bool OnTtDamage(PhysicalGameObj* damager, PhysicalGameObj* target, const AmmoDefinitionClass* ammo, const char* bone) { return true; }
	virtual void OnPreLoadLevel() { };
};

class DASSGMPluginManager : public DAEventClass {
public:
	static void Init();

	static DynamicVectorClass<Plugin*> Events[EVENT_COUNT];
	
private:
	virtual bool Chat_Event(cPlayer *Player,TextMessageEnum Type,const wchar_t *Message,int ReceiverID);
	virtual bool Radio_Event(cPlayer *Player,int PlayerType,int AnnouncementID,int IconID,AnnouncementEnum AnnouncementType);
	virtual void Player_Join_Event(cPlayer *Player);
	virtual void Player_Leave_Event(cPlayer *Player);
	virtual void Level_Loaded_Event();
	virtual void Settings_Loaded_Event();
	virtual void Game_Over_Event();
	virtual int Character_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const SoldierGameObjDef *Item);
	virtual int Vehicle_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const VehicleGameObjDef *Item);
	virtual int PowerUp_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const PowerUpGameObjDef *Item);
	virtual bool Refill_Event(cPlayer *Player);
	virtual bool Stock_Client_Damage_Request_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float Damage, uint Warhead);
	virtual bool TT_Client_Damage_Request_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,const AmmoDefinitionClass *Ammo,const char *Bone);
	virtual void Think();
	virtual void Object_Created_Event(GameObject *obj);

	static DynamicVectorClass<ConsoleOutputHook> ConsoleOutputHooks;
	static DynamicVectorClass<HostHook> HostHooks;
};

#endif