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

#include "general.h"
#include "da_hook.h"
#include "da_disable.h"
#include "da_translation.h"
#include "da_settings.h"
#include "da_ssgm.h"
#include "da_log.h"
#include "engine_da.h"
#include "engine_weap.h"
#include "engine_obj2.h"
#include "engine_player.h"

//These functions allow DA to load SSGM plugins.

DynamicVectorClass<Plugin*>  DASSGMPluginManager::Events[EVENT_COUNT];

void SSGMGameLog::Log_Message(const char *message,const char *category) {
	DALogManager::Write_Log(category,"%s",message);
}

void SSGMGameLog::Log_Gamelog(const char *format,...) {
	char Buffer[526];
	va_list arg_list;
	va_start(arg_list,format);
	vsnprintf(Buffer,526,format,arg_list);
	va_end(arg_list);
	DALogManager::Write_GameLog("%s",Buffer);
}

void SSGMGameLog::Log_RenLog(const char *message) {
}

void SSGMGameLog::Log_Custom(int id,const char *format,...) {
}

void SSGMGameLog::Send_Console(const char *message) {
}

void SSGMGameLog::Shutdown() {
}

void SSGMGameLog::Think() {
}

void SSGMGameLog::Send(const char *data) {
	DALogManager::Send(data);
}

DA_API bool InvincibleBuildings = false;
DA_API bool DisableBeacons = false;
DA_API bool DisableBaseDefenses = false;
DA_API bool DisablePowerPlants = false;
DA_API bool DisableRefineries = false;
DA_API bool DisableSoldierFactories = false;
DA_API bool DisableVehicleFactories = false;
DA_API bool DisableRepairPads = false;
DA_API bool DisableCommCenters = false;
DA_API bool FreePurchases = false;
DA_API bool DisableExtraWeapons = false;
DA_API bool BuildingDeathPages = false;
DA_API bool VehicleOwnership = true;
DA_API bool DisableConstructionYards = false;
DA_API bool DisableShrines = false;
DA_API bool DisableHelipads = false;
DA_API bool DisableSpecials = false;
DA_API bool DisableTechCenters = false;
DA_API bool DisableNavalFactories = false;
DA_API unsigned int RefillLimit = 0;
DA_API int WeatherType = 0;
DA_API int MapWeatherType = 0;
DA_API int ForceTeam = -1;
DA_API int MapForceTeam = -1;
DA_API bool ExtraKillMessages = 0;
DA_API float PowerupExpireTime = 10.f;
DA_API bool PlayPowerupSounds = 0;
DA_API float WreckDestroySelfTime = 0.0f;
DA_API bool PlayQuakeSounds = 0;
DA_API bool SBHCanPickupDropWeapons = 0;
DA_API bool CharactersDropDNA = 0;
DA_API bool ShowExtraMessages = 1;

DA_API bool IsPresetDisabled(int preset) {
	return DADisableListManager::Is_Preset_Disabled(preset);
}

DA_API const char *Get_Translated_Preset_Name_Part(GameObject *obj) {
	return newstr(DATranslationManager::Translate_With_Team_Name(obj));
}

DA_API const char *Get_Translated_Preset_Name_Ini(GameObject *obj) {
	return newstr(DATranslationManager::Translate(obj));
}

DA_API const char *Get_Translated_Definition_Name_Ini(const char *preset) {
	return newstr(DATranslationManager::Translate(preset));
}

DA_API const char *Get_Translated_Preset_Name_Ex(GameObject *obj) {
	return newstr(DATranslationManager::Translate_Soldier(obj));
}

DA_API void RegisterEvent(EventType Type,Plugin *Plugin) {
	if (DASSGMPluginManager::Events[Type].ID(Plugin) == -1) {
		DASSGMPluginManager::Events[Type].Add(Plugin);
	}
}

DA_API void UnregisterEvent(EventType Type,Plugin *Plugin) {
	DASSGMPluginManager::Events[Type].DeleteObj(Plugin);
}

void DASSGMPluginManager::Init() {
	static DASSGMPluginManager Instance;
	Instance.Register_Event(DAEvent::CHAT);
	Instance.Register_Event(DAEvent::RADIO);
	Instance.Register_Event(DAEvent::HOSTCHAT);
	Instance.Register_Event(DAEvent::PLAYERJOIN);
	Instance.Register_Event(DAEvent::PLAYERLEAVE);
	Instance.Register_Event(DAEvent::LEVELLOADED);
	Instance.Register_Event(DAEvent::SETTINGSLOADED);
	Instance.Register_Event(DAEvent::GAMEOVER);
	Instance.Register_Event(DAEvent::CONSOLEOUTPUT);
	Instance.Register_Event(DAEvent::CHARACTERPURCHASEREQUEST);
	Instance.Register_Event(DAEvent::VEHICLEPURCHASEREQUEST);
	Instance.Register_Event(DAEvent::POWERUPPURCHASEREQUEST);
	Instance.Register_Event(DAEvent::THINK);
	Instance.Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::ALL);
	Instance.Register_Object_Event(DAObjectEvent::STOCKCLIENTDAMAGEREQUEST,DAObjectEvent::ALL);
	Instance.Register_Object_Event(DAObjectEvent::TTCLIENTDAMAGEREQUEST,DAObjectEvent::ALL);
}
bool DASSGMPluginManager::Chat_Event(cPlayer *Player,TextMessageEnum Type,const wchar_t *Message,int ReceiverID) {
	for (int i = Events[EVENT_CHAT_HOOK].Count()-1;i >= 0;i--) {
		if (!Events[EVENT_CHAT_HOOK][i]->OnChat(Player->Get_Id(),Type,Message,ReceiverID)) {
			return false;
		}
	}
	return true;
}

bool DASSGMPluginManager::Radio_Event(cPlayer *Player,int PlayerType,int AnnouncementID,int IconID,AnnouncementEnum AnnouncementType) {
	for (int i = Events[EVENT_RADIO_HOOK].Count()-1;i >= 0;i--) {
		if (!Events[EVENT_RADIO_HOOK][i]->OnRadioCommand(PlayerType,Player->Get_Id(),AnnouncementID,IconID,AnnouncementType)) {
			return false;
		}
	}
	return true;
}

void DASSGMPluginManager::Player_Join_Event(cPlayer *Player) {
	for (int i = Events[EVENT_PLAYER_JOIN_HOOK].Count()-1;i >= 0;i--) {
		Events[EVENT_PLAYER_JOIN_HOOK][i]->OnPlayerJoin(Player->Get_Id(),StringClass(Player->Get_Name()));
	}
}

void DASSGMPluginManager::Player_Leave_Event(cPlayer *Player) {
	for (int i = Events[EVENT_PLAYER_LEAVE_HOOK].Count()-1;i >= 0;i--) {
		Events[EVENT_PLAYER_LEAVE_HOOK][i]->OnPlayerLeave(Player->Get_Id());
	}
}

void DASSGMPluginManager::Level_Loaded_Event() {
	for (int i = Events[EVENT_PRE_LOAD_LEVEL_HOOK].Count()-1;i >= 0;i--) {
		Events[EVENT_PRE_LOAD_LEVEL_HOOK][i]->OnPreLoadLevel();
	}
	for (int i = Events[EVENT_LOAD_LEVEL_HOOK].Count()-1;i >= 0;i--) {
		Events[EVENT_LOAD_LEVEL_HOOK][i]->OnLoadLevel();
	}
}

void DASSGMPluginManager::Settings_Loaded_Event() {
	for (int i = Events[EVENT_GLOBAL_INI].Count()-1;i >= 0;i--) {
		Events[EVENT_GLOBAL_INI][i]->OnFreeData();
		Events[EVENT_GLOBAL_INI][i]->OnLoadGlobalINISettings(const_cast<INIClass*>(DASettingsManager::Get_Settings(0)->Get_INI()));
	}
	for (int i = Events[EVENT_MAP_INI].Count()-1;i >= 0;i--) {
		Events[EVENT_MAP_INI][i]->OnFreeMapData();
		Events[EVENT_MAP_INI][i]->OnLoadMapINISettings(const_cast<INIClass*>(DASettingsManager::Get_Settings(0)->Get_INI()));
	}
}

void DASSGMPluginManager::Game_Over_Event() {
	for (int i = Events[EVENT_GAME_OVER_HOOK].Count()-1;i >= 0;i--) {
		Events[EVENT_GAME_OVER_HOOK][i]->OnGameOver();
	}
}

int DASSGMPluginManager::Character_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const SoldierGameObjDef *Item) {
	for (int i = Events[EVENT_CHARACTER_PURCHASE_HOOK].Count()-1;i >= 0;i--) {
		int Return = Events[EVENT_CHARACTER_PURCHASE_HOOK][i]->OnCharacterPurchase(Base,Player->Get_GameObj(),(unsigned int)Cost,Item->Get_ID(),0);
		if (Return != -1) {
			return Return;
		}
	}
	return -1;
}

int DASSGMPluginManager::Vehicle_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const VehicleGameObjDef *Item) {
	for (int i = Events[EVENT_VEHICLE_PURCHASE_HOOK].Count()-1;i >= 0;i--) {
		int Return = Events[EVENT_VEHICLE_PURCHASE_HOOK][i]->OnVehiclePurchase(Base,Player->Get_GameObj(),(unsigned int)Cost,Item->Get_ID(),0);
		if (Return != -1) {
			return Return;
		}
	}
	return -1;
}

int DASSGMPluginManager::PowerUp_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const PowerUpGameObjDef *Item) {
	for (int i = Events[EVENT_POWERUP_PURCHASE_HOOK].Count()-1;i >= 0;i--) {
		int Return = Events[EVENT_POWERUP_PURCHASE_HOOK][i]->OnPowerupPurchase(Base,Player->Get_GameObj(),(unsigned int)Cost,Item->Get_ID(),0);
		if (Return != -1) {
			return Return;
		}
	}
	return -1;
}

bool DASSGMPluginManager::Refill_Event(cPlayer *Player) {
	for (int i = Events[EVENT_REFILL_HOOK].Count()-1;i >= 0;i--) {
		if (!Events[EVENT_REFILL_HOOK][i]->OnRefill(Player->Get_GameObj())) {
			return false;
		}
	}
	return true;
}

bool DASSGMPluginManager::Stock_Client_Damage_Request_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float Damage, uint Warhead) {
	for (int i = Events[EVENT_STOCK_DAMAGE_HOOK].Count()-1;i >= 0;i--) {
		if (!Events[EVENT_STOCK_DAMAGE_HOOK][i]->OnStockDamage(Damager,(PhysicalGameObj*)Victim,Damage,Warhead)) {
			return false;
		}
	}
	return true;
}

bool DASSGMPluginManager::TT_Client_Damage_Request_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,const AmmoDefinitionClass *Ammo,const char *Bone) {
	for (int i = Events[EVENT_TT_DAMAGE_HOOK].Count()-1;i >= 0;i--) {
		if (!Events[EVENT_TT_DAMAGE_HOOK][i]->OnTtDamage(Damager,(PhysicalGameObj*)Victim,Ammo,Bone)) {
			return false;
		}
	}
	return true;
}

void DASSGMPluginManager::Think() {
	for (int i = Events[EVENT_THINK_HOOK].Count()-1;i >= 0;i--) {
		Events[EVENT_THINK_HOOK][i]->OnThink();
	}
}

void DASSGMPluginManager::Object_Created_Event(GameObject *obj) {
	for (int i = Events[EVENT_OBJECT_CREATE_HOOK].Count()-1;i >= 0;i--) {
		Events[EVENT_OBJECT_CREATE_HOOK][i]->OnObjectCreate(0,obj);
	}
}

SCRIPTS_API void Log_Killed_Message(GameObject* obj, GameObject* killer, const char* objectType)
{
	const char* killerPreset = Commands->Get_Preset_Name(killer);
	const char* killerWeapon = Get_Current_Weapon(killer);
	if (auto killerVehicle = Get_Vehicle(killer))
	{
		killerPreset = Commands->Get_Preset_Name(killerVehicle);
		killerWeapon = Get_Current_Weapon(killerVehicle);
	}

	auto killedObj = obj;
	if (auto killedVehicle = Get_Vehicle(obj))
	{
		killedObj = Get_Vehicle_Occupant(obj, 0);
	}

	Vector3	victimpos = Commands->Get_Position(obj);
	Vector3	damagerpos = Commands->Get_Position(killer);
	const char* str1 = Get_Translated_Preset_Name_Ex(obj);
	const char* str2 = Get_Translated_Preset_Name_Ex(killer);
	SSGMGameLog::Log_Gamelog("KILLED;%s;%d;%s;%d;%d;%d;%d;%d;%s;%d;%d;%d;%d;%s;%s;%s;%ls;%ls", objectType, Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), int(victimpos.Y), int(victimpos.X), int(victimpos.Z), int(Commands->Get_Facing(obj)), Commands->Get_ID(killer), killerPreset, int(damagerpos.Y), int(damagerpos.X), int(damagerpos.Z), int(Commands->Get_Facing(killer)), killerWeapon, str1, str2, Get_Wide_Player_Name(killer), Get_Wide_Player_Name(killedObj));
	delete[] str1;
	delete[] str2;
}