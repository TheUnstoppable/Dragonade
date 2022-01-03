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

#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "engine_da.h"
#include "da.h"
#include "da_player.h"
#include "da_translation.h"
#include "da_settings.h"
#include "da_log.h"
#include "da_chatcommand.h"
#include "da_vehicle.h"
#include "cScTextObj.h"
#include "SCAnnouncement.h"
#include "WeaponBagClass.h"
#include "WeaponClass.h"
#include "weaponmgr.h"
#include "MoveablePhysClass.h"
#include "ArmorWarheadManager.h"
#include "CombatManager.h"
#include "GameObjManager.h"

DynamicVectorClass<DAPlayerDataFactoryClass*> DAPlayerManager::DataFactories;
DynamicVectorClass<DAPlayerClass*> DAPlayerManager::Players;
bool DAPlayerManager::DisableKillMessages;
bool DAPlayerManager::DisableKillCounter;
bool DAPlayerManager::DisableTeamKillCounter;
bool DAPlayerManager::DisableDeathCounter;
bool DAPlayerManager::DisableTeamDeathCounter;
bool DAPlayerManager::DisableDamagePoints;
bool DAPlayerManager::DisableDeathPoints;
bool DAPlayerManager::DisableTeamScoreCounter;
float DAPlayerManager::CreditsMultiplier;


DAPlayerClass::DAPlayerClass(cPlayer *Player) {
	Player->Set_DA_Player(this);
	Owner = Player;
	Serial = Get_Client_Serial_Hash(Get_ID());
	Version = Get_Client_Version(Get_ID());
	Revision = Get_Client_Revision(Get_ID());
	AccessLevel = DAAccessLevel::NONE;
	Muted = false;
	C4Locked = false;
	BeaconLocked = false;
	VehicleLimit = Get_Vehicle_Limit();
	C4Limit = Get_Mine_Limit();
	CharacterDiscount = 1.0f;
	VehicleDiscount = 1.0f;
	PowerUpDiscount = 1.0f;
	Loaded = false;
	LastTibDamageTime = 0;
	ServerDamage = false;
	CreationTime = 0;
	NeedsTeam = false;
}

DAPlayerClass::~DAPlayerClass() {
	for (int i = 0;i < Observers.Count();i++) {
		delete Observers[i];
	}
	Observers.Delete_All();
	for (int i = 0;i < Data.Count();i++) {
		delete Data[i];
	}
	Data.Delete_All();
	for (int i = 0;i < Tags.Count();i++) {
		delete Tags[i];
	}
	Tags.Delete_All();
}

void DAPlayerObserverClass::Start_Timer(int Number,float Duration,bool Repeat,unsigned int Data) {
	DAPlayerObserverTimerStruct *Timer = new DAPlayerObserverTimerStruct;
	Timer->Number = Number;
	Timer->Repeat = Repeat;
	Timer->Data = Data;
	Timer->Duration = (unsigned long)(Duration*1000);
	Timer->StartTime = GetTickCount();
	Timers.Add(Timer);
}

void DAPlayerObserverClass::Stop_Timer(int Number,unsigned int Data) {
	for (int i = Timers.Count()-1;i >= 0;i--) {
		if (Timers[i]->Number == Number && (!Data || Timers[i]->Data == Data)) {
			Timers[i]->Number = -1;
		}
	}
}

bool DAPlayerObserverClass::Is_Timer(int Number,unsigned int Data) {
	for (int i = Timers.Count()-1;i >= 0;i--) {
		if (Timers[i]->Number == Number && (!Data || Timers[i]->Data == Data)) {
			return true;
		}
	}
	return false;
}


void DAPlayerObserverClass::Clear_Timers() {
	for (int i = Timers.Count()-1;i >= 0;i--) {
		delete Timers[i];
	}
	Timers.Delete_All();
}

void DAPlayerObserverClass::Register_Chat_Command(DAPOCC Func,const char *Triggers,int Parameters,DAAccessLevel::Level AccessLevel,DAChatType::Type ChatType) {
	DAPlayerObserverChatCommandStruct *Struct = new DAPlayerObserverChatCommandStruct;
	Struct->AccessLevel = AccessLevel;
	Struct->ChatType = ChatType;
	Struct->Parameters = Parameters;
	DATokenParserClass Parser(Triggers,'|');
	while (char *Token = Parser.Get_String()) {
		_strlwr(Token);
		Struct->Triggers.Add(Token);
	}
	Struct->Func = Func;
	ChatCommands.Add(Struct);
}

void DAPlayerObserverClass::Unregister_Chat_Command(const char *Trigger) {
	for (int i = ChatCommands.Count()-1;i >= 0;i--) {
		if (ChatCommands[i]->Triggers.ID(Trigger) != -1) {
			delete ChatCommands[i];
			ChatCommands.Delete(i);
		}
	}
}

void DAPlayerObserverClass::Clear_Chat_Commands() {
	for (int i = ChatCommands.Count()-1;i >= 0;i--) {
		delete ChatCommands[i];
	}
	ChatCommands.Delete_All();
}

void DAPlayerObserverClass::Register_Key_Hook(DAPOKH Func,const char *Triggers) {
	DAPlayerObserverKeyHookStruct *Struct = new DAPlayerObserverKeyHookStruct;
	DATokenParserClass Parser(Triggers,'|');
	while (char *Token = Parser.Get_String()) {
		_strlwr(Token);
		Struct->Triggers.Add(Token);
	}
	Struct->Func = Func;
	KeyHooks.Add(Struct);
}

void DAPlayerObserverClass::Unregister_Key_Hook(const char *Trigger) {
	for (int i = KeyHooks.Count()-1;i >= 0;i--) {
		if (KeyHooks[i]->Triggers.ID(Trigger) != -1) {
			delete KeyHooks[i];
			KeyHooks.Delete(i);
		}
	}
}

void DAPlayerObserverClass::Clear_Key_Hooks() {
	for (int i = KeyHooks.Count()-1;i >= 0;i--) {
		delete KeyHooks[i];
	}
	KeyHooks.Delete_All();
}




void DAPlayerClass::Add_Observer(DAPlayerObserverClass *Observer) {
	Observer->Set_Owner(Get_Owner());
	Observer->Init();
	if (Get_GameObj()) {
		Observer->Created();
	}
	Observers.Add(Observer);
}

void DAPlayerClass::Remove_Observer(const char *Name) {
	for (int i = 0;i < Observers.Count();i++) {
		if (!_stricmp(Observers[i]->Get_Observer_Name(),Name)) {
			Observers[i]->Set_Delete_Pending();
		}
	}
}

DAPlayerObserverClass *DAPlayerClass::Find_Observer(const char *Name) {
	for (int i = 0;i < Observers.Count();i++) {
		if (!_stricmp(Observers[i]->Get_Observer_Name(),Name)) {
			return Observers[i];
		}
	}
	return 0;
}

void DAPlayerClass::Add_Data(DAPlayerDataClass *data) {
	data->Set_Owner(Get_Owner());
	data->Init();
	Data.Add(data);
}

void DAPlayerClass::Remove_Data(const DAPlayerDataFactoryClass *Factory) {
	for (int i = Observers.Count()-1;i >= 0;i--) {
		if (Data[i]->Get_Factory() == Factory) {
			delete Data[i];
			Data.Delete(i);
		}
	}
}

DAPlayerDataClass *DAPlayerClass::Find_Data(const DAPlayerDataFactoryClass *Factory) {
	for (int i = 0;i < Data.Count();i++) {
		if (Data[i]->Get_Factory() == Factory) {
			return Data[i];
		}
	}
	return 0;
}

void DAPlayerClass::Update_Tag() {
	StringClass Tag;
	for (int i = 0;i < Tags.Count();i++) {
		Tag += Tags[i]->Tag;
		Tag += "\n";
	}
	Get_Owner()->customTag = Tag;
	Get_Owner()->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
}

void DAPlayerClass::Add_Tag(const char *Tag,int Position,DAPlayerFlags::Flag Flags) {
	DAPlayerTagStruct *NewTag = new DAPlayerTagStruct;
	NewTag->Tag = Tag;
	NewTag->Position = Position;
	NewTag->Flags = Flags;
	for (int i = 0;i < Tags.Count();i++) {
		if (Tags[i]->Position < Position) {
			Tags.Insert(i,NewTag);
			Update_Tag();
			return;
		}
	}
	Tags.Add(NewTag);
	Update_Tag();
}

void DAPlayerClass::Remove_Tag(const char *Tag) {
	for (int i = 0;i < Tags.Count();i++) {
		if (Tags[i]->Tag == Tag) {
			delete Tags[i];
			Tags.Delete(i);
			i--;
		}
	}
	Update_Tag();
}

void DAPlayerClass::Remove_Tags_With_Flag(DAPlayerFlags::Flag Flag) {
	for (int i = 0;i < Tags.Count();i++) {
		if (Tags[i]->Has_Flag(Flag)) {
			delete Tags[i];
			Tags.Delete(i);
			i--;
		}
	}
	Update_Tag();
}

const DynamicVectorClass<DAPlayerObserverClass*> &DAPlayerClass::Get_Observers() {
	return Observers;
}

void DAPlayerClass::Set_Access_Level(DAAccessLevel::Level Level) {
	AccessLevel = Level;
}

DAAccessLevel::Level DAPlayerClass::Get_Access_Level() {
	return AccessLevel;
}

void DAPlayerClass::Mute(bool mute) {
	Muted = mute;
}

bool DAPlayerClass::Is_Muted() {
	return Muted;
}

void DAPlayerClass::Beacon_Lock(bool lock) {
	BeaconLocked = lock;
	if (lock) {
		SoldierGameObj *Soldier = (SoldierGameObj*)Get_GameObj();
		if (Soldier) {
			WeaponBagClass *Bag = Soldier->Get_Weapon_Bag();
			for (int i = Bag->Get_Count()-1;i >= 1;i--) {
				if (Bag->Peek_Weapon(i)->Get_Definition()->Style == STYLE_BEACON) {
					Bag->Remove_Weapon(i);
				}
			}
		}
		DA::Page_Player(Get_ID(),"You have been beacon locked. You can no longer use beacons.");
	}
	else {
		DA::Page_Player(Get_ID(),"You are no longer beacon locked.");
	}
}

bool DAPlayerClass::Is_Beacon_Locked() {
	return BeaconLocked;
}

void DAPlayerClass::C4_Lock(bool lock) {
	C4Locked = lock;
	if (lock) {
		SoldierGameObj *Soldier = (SoldierGameObj*)Get_GameObj();
		if (Soldier) {
			WeaponBagClass *Bag = Soldier->Get_Weapon_Bag();
			for (int i = Bag->Get_Count()-1;i >= 1;i--) {
				if (Bag->Peek_Weapon(i)->Get_Definition()->Style == STYLE_C4) {
					if (Bag->Peek_Weapon(i)->PrimaryAmmoDefinition->AmmoType != 2) {
						Bag->Remove_Weapon(i);
					}
				}
			}
		}
		DA::Page_Player(Get_ID(),"You have been C4 locked. You can no longer use Remote or Proximity C4");
	}
	else {
		DA::Page_Player(Get_ID(),"You are no longer C4 locked.");
	}
}

 bool DAPlayerClass::Is_C4_Locked() {
	return C4Locked;
}

void DAPlayerClass::Set_Character_Discount(float Discount) {
	if (Discount > 1.0f) {
		CharacterDiscount = 1.0f;
	}
	else if (Discount < 0.0f) {
		CharacterDiscount = 0.0f;
	}
	else {
		CharacterDiscount = Discount;
	}
}

void DAPlayerClass::Inc_Character_Discount(float Discount) {
	Set_Character_Discount(CharacterDiscount - Discount);
}

float DAPlayerClass::Get_Character_Discount() {
	return CharacterDiscount;
}

void DAPlayerClass::Set_Vehicle_Discount(float Discount) {
	if (Discount > 1.0f) {
		VehicleDiscount = 1.0f;
	}
	else if (Discount < 0.0f) {
		VehicleDiscount = 0.0f;
	}
	else {
		VehicleDiscount = Discount;
	}
}

void DAPlayerClass::Inc_Vehicle_Discount(float Discount) {
	Set_Vehicle_Discount(VehicleDiscount - Discount);
}

float DAPlayerClass::Get_Vehicle_Discount() {
	return VehicleDiscount;
}

void DAPlayerClass::Set_PowerUp_Discount(float Discount) {
	if (Discount > 1.0f) {
		PowerUpDiscount = 1.0f;
	}
	else if (Discount < 0.0f) {
		PowerUpDiscount = 0.0f;
	}
	else {
		PowerUpDiscount = Discount;
	}
}

void DAPlayerClass::Inc_PowerUp_Discount(float Discount) {
	Set_PowerUp_Discount(VehicleDiscount - Discount);
}

float DAPlayerClass::Get_PowerUp_Discount() {
	return PowerUpDiscount;
}

unsigned long DAPlayerClass::Get_Time_Since_Last_Tib_Damage() {
	return GetTickCount()-LastTibDamageTime;
}

void DAPlayerClass::Reset_Last_Tib_Damage_Time() {
	LastTibDamageTime = GetTickCount();
}

bool DAPlayerClass::Is_Loaded() {
	return Loaded;
}

void DAPlayerClass::Set_Loaded(bool Load) {
	Loaded = Load;
}

const StringClass &DAPlayerClass::Get_Serial() {
	return Serial;
}

void DAPlayerClass::Set_Version(float Ver) {
	Version = Ver;
}

float DAPlayerClass::Get_Version() {
	return Version;
}

unsigned int DAPlayerClass::Get_Revision() {
	return Revision;
}

bool DAPlayerClass::Is_TT_Client() {
	return (Get_Version() >= 4.0f);
}

bool DAPlayerClass::Is_Scripts_Client() {
	return (Get_Version() >= 2.6f);
}

bool DAPlayerClass::Is_Stock_Client() {
	return (Get_Version() < 4.0f);
}

void DAPlayerClass::Set_Server_Damage(bool Damage) {
	ServerDamage = Damage;
}

bool DAPlayerClass::Use_Server_Damage() {
	return ServerDamage;
}

bool DAPlayerClass::Is_Spawning() {
	return (CreationTime == The_Game()->FrameCount);
}

unsigned long DAPlayerClass::Get_Creation_Time() {
	return CreationTime;
}

void DAPlayerClass::Reset_Creation_Time() {
	CreationTime = The_Game()->FrameCount;
}

void DAPlayerClass::Set_Needs_Team(bool Needs) {
	NeedsTeam = Needs;
}

bool DAPlayerClass::Get_Needs_Team() {
	return NeedsTeam;
}

void DAPlayerClass::Join() {
	Get_Owner()->Set_DA_Player(this);
	Serial = Get_Client_Serial_Hash(Get_ID());
	Version = Get_Client_Version(Get_ID());
	Revision = Get_Client_Revision(Get_ID());
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Join();
	}
	Loaded = false;
	ServerDamage = false;
}

void DAPlayerClass::Leave() {
	AccessLevel = DAAccessLevel::NONE;
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Leave();
		if (!Observers[i]->Has_Flag(DAPlayerFlags::PERSISTLEAVE)) {
			Observers[i]->Set_Delete_Pending();
		}
	}
	
	for (int i = 0;i < Data.Count();i++) {
		Data[i]->Clear_Session();
	}
	
	for (int i = 0;i < Tags.Count();i++) {
		if (!Tags[i]->Has_Flag(DAPlayerFlags::PERSISTLEAVE)) {
			delete Tags[i];
			Tags.Delete(i);
			i--;
		}
	}
	Update_Tag();
}

void DAPlayerClass::Level_Loaded() {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Level_Loaded();
		if (!Observers[i]->Has_Flag(DAPlayerFlags::PERSISTMAP)) {
			Observers[i]->Set_Delete_Pending();
		}
	}

	for (int i = 0;i < Data.Count();i++) {
		Data[i]->Clear_Level();
	}
	
	for (int i = 0;i < Tags.Count();i++) {
		if (!Tags[i]->Has_Flag(DAPlayerFlags::PERSISTMAP)) {
			delete Tags[i];
			Tags.Delete(i);
			i--;
		}
	}
	Update_Tag();
	CharacterDiscount = 1.0f;
	VehicleDiscount = 1.0f;
	PowerUpDiscount = 1.0f;
	Loaded = false;
}

void DAPlayerClass::Player_Loaded() {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Player_Loaded();
	}
}

void DAPlayerClass::Name_Change() {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Name_Change();
	}
}

bool DAPlayerClass::Chat(TextMessageEnum Type,const wchar_t *Message,int ReceiverID) {
	if (Is_Muted() && Type != TEXT_MESSAGE_PRIVATE) {
		cScTextObj *Text = Send_Client_Text(Message,Type,false,Get_ID(),ReceiverID,false,false); //Players will still see their own messages when muted.
		Text->Set_Object_Dirty_Bits(Get_ID(),NetworkObjectClass::BIT_CREATION);
		return false;
	}
	return true;
}

bool DAPlayerClass::Radio(int PlayerType,int AnnouncementID,int IconID,AnnouncementEnum AnnouncementType) {
	if (AnnouncementID < 8535 || AnnouncementID > 8564) { //Block invalid radio commands.
		return false;
	}
	else if (Is_Muted()) {
		SCAnnouncement *Radio = Send_Client_Announcement(PlayerType,Get_ID(),AnnouncementID,AnnouncementType,IconID,false,false);
		Radio->Set_Object_Dirty_Bits(Get_ID(),NetworkObjectClass::BIT_CREATION);
		return false;
	}
	return true;
}

void DAPlayerClass::Team_Change() {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Team_Change();
	}
}

int DAPlayerClass::Character_Purchase_Request(float &Cost,const SoldierGameObjDef *Item) {
	Cost *= Get_Character_Discount();
	for (int i = 0;i < Observers.Count();i++) {
		int Return = Observers[i]->Character_Purchase_Request(Cost,Item);
		if (Return != -1) {
			return Return;
		}
	}
	return -1;
}

int DAPlayerClass::Vehicle_Purchase_Request(float &Cost,const VehicleGameObjDef *Item) {
	Cost *= Get_Vehicle_Discount();
	for (int i = 0;i < Observers.Count();i++) {
		int Return = Observers[i]->Vehicle_Purchase_Request(Cost,Item);
		if (Return != -1) {
			return Return;
		}
	}
	return -1;
}

int DAPlayerClass::PowerUp_Purchase_Request(float &Cost,const PowerUpGameObjDef *Item) {
	Cost *= Get_PowerUp_Discount();
	if (Item->GrantWeapon) {
		WeaponDefinitionClass *WeapDef = (WeaponDefinitionClass*)Find_Definition(Item->GrantWeaponID);
		if (WeapDef) {
			if (Is_C4_Locked() && WeapDef->Style == STYLE_C4) {
				if (((AmmoDefinitionClass*)Find_Definition(WeapDef->PrimaryAmmoDefID))->AmmoType != 2) {
					return 4;
				}
			}
			else if (Is_Beacon_Locked() && WeapDef->Style == STYLE_BEACON) {
				return 4;
			}
		}
	}
	for (int i = 0;i < Observers.Count();i++) {
		int Return = Observers[i]->PowerUp_Purchase_Request(Cost,Item);
		if (Return != -1) {
			return Return;
		}
	}
	return -1;
}

int DAPlayerClass::Custom_Purchase_Request(float &Cost,unsigned int ID) {
	for (int i = 0;i < Observers.Count();i++) {
		int Return = Observers[i]->Custom_Purchase_Request(Cost,ID);
		if (Return != -1) {
			return Return;
		}
	}
	return -1;
}

void DAPlayerClass::Character_Purchase(float Cost,const SoldierGameObjDef *Item) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Character_Purchase(Cost,Item);
	}
}

void DAPlayerClass::Vehicle_Purchase(float Cost,const VehicleGameObjDef *Item) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Vehicle_Purchase(Cost,Item);
	}
}

void DAPlayerClass::PowerUp_Purchase(float Cost,const PowerUpGameObjDef *Item) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->PowerUp_Purchase(Cost,Item);
	}
}

void DAPlayerClass::Custom_Purchase(float Cost,unsigned int ID) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Custom_Purchase(Cost,ID);
	}
}

bool DAPlayerClass::Vehicle_Entry_Request(VehicleGameObj *Vehicle,int &Seat) {
	for (int i = 0;i < Observers.Count();i++) {
		if (!Observers[i]->Vehicle_Entry_Request(Vehicle,Seat)) {
			return false;
		}
	}
	return true;
}

void DAPlayerClass::Vehicle_Enter(VehicleGameObj *Vehicle,int Seat) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Vehicle_Enter(Vehicle,Seat);
	}
}

void DAPlayerClass::Vehicle_Exit(VehicleGameObj *Vehicle,int Seat) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Vehicle_Exit(Vehicle,Seat);
	}
}

bool DAPlayerClass::PowerUp_Grant_Request(const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) {
	if (PowerUp->GrantWeapon) {
		WeaponDefinitionClass *WeapDef = (WeaponDefinitionClass*)Find_Definition(PowerUp->GrantWeaponID);
		if (WeapDef) {
			if (Is_C4_Locked() && WeapDef->Style == STYLE_C4) {
				if (((AmmoDefinitionClass*)Find_Definition(WeapDef->PrimaryAmmoDefID))->AmmoType != 2) {
					return false;
				}
			}
			else if (Is_Beacon_Locked() && WeapDef->Style == STYLE_BEACON) {
				return false;
			}
		}
	}
	for (int i = 0;i < Observers.Count();i++) {
		if (!Observers[i]->PowerUp_Grant_Request(PowerUp,PowerUpObj)) {
			return false;
		}
	}
	return true;
}

void DAPlayerClass::PowerUp_Grant(const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->PowerUp_Grant(PowerUp,PowerUpObj);
	}
}

bool DAPlayerClass::Add_Weapon_Request(const WeaponDefinitionClass *Weapon) {
	if (Is_C4_Locked() && Weapon->Style == STYLE_C4) {
		if (((AmmoDefinitionClass*)Find_Definition(Weapon->PrimaryAmmoDefID))->AmmoType != 2) {
			return false;
		}
	}
	else if (Is_Beacon_Locked() && Weapon->Style == STYLE_BEACON) {
		return false;
	}
	for (int i = 0;i < Observers.Count();i++) {
		if (!Observers[i]->Add_Weapon_Request(Weapon)) {
			return false;
		}
	}
	return true;
}

void DAPlayerClass::Add_Weapon(WeaponClass *Weapon) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Add_Weapon(Weapon);
	}
}

void DAPlayerClass::Remove_Weapon(WeaponClass *Weapon) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Remove_Weapon(Weapon);
	}
}

void DAPlayerClass::Clear_Weapons() {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Clear_Weapons();
	}
}

bool DAPlayerClass::C4_Detonate_Request(C4GameObj *C4) {
	for (int i = 0;i < Observers.Count();i++) {
		if (!Observers[i]->C4_Detonate_Request(C4)) {
			return false;
		}
	}
	return true;
}

void DAPlayerClass::C4_Detonate(C4GameObj *C4) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->C4_Detonate(C4);
	}
}

void DAPlayerClass::Change_Character(const SoldierGameObjDef *Soldier) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Change_Character(Soldier);
	}
	Reset_Creation_Time();
}

void DAPlayerClass::Created() {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Created();
	}
	Reset_Creation_Time();
}

void DAPlayerClass::Destroyed() {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Destroyed();
	}
}

bool DAPlayerClass::Damage_Dealt_Request(DamageableGameObj *Victim,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type) {
	for (int i = 0;i < Observers.Count();i++) {
		if (!Observers[i]->Damage_Dealt_Request(Victim,Damage,Warhead,Scale,Type)) {
			return false;
		}
	}
	return true;
}

bool DAPlayerClass::Damage_Received_Request(ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type) {
	for (int i = 0;i < Observers.Count();i++) {
		if (!Observers[i]->Damage_Received_Request(Damager,Damage,Warhead,Scale,Type)) {
			return false;
		}
	}
	return true;
}

void DAPlayerClass::Damage_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Damage_Dealt(Victim,Damage,Warhead,Scale,Type);
	}
}

void DAPlayerClass::Damage_Received(ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Damage_Received(Damager,Damage,Warhead,Scale,Type);
	}
}

void DAPlayerClass::Kill_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Kill_Dealt(Victim,Damage,Warhead,Scale,Type);
	}
}

void DAPlayerClass::Kill_Received(ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Kill_Received(Killer,Damage,Warhead,Scale,Type);
	}
}

void DAPlayerClass::Custom(GameObject *Sender,int Type,int Param) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Custom(Sender,Type,Param);
	}
}

void DAPlayerClass::Poked(cPlayer *Player) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Poked(Player);
	}
}

void DAPlayerClass::Think() {
	for (int j = 0;j < Observers.Count();j++) {
		if (Observers[j]->Is_Delete_Pending()) {
			delete Observers[j];
			Observers.Delete(j);
			j--;
		}
		else {
			if (Observers[j]->Has_Flag(DAPlayerFlags::THINK)) {
				Observers[j]->Think();
			}
			DynamicVectorClass<DAPlayerObserverTimerStruct*> &Timers = Observers[j]->Get_Timers();
			for (int i = 0;i < Timers.Count();i++) {
				if (Timers[i]->Number == -1) {
					delete Timers[i];
					Timers.Delete(i);
					i--;
				}
				else if (GetTickCount()-Timers[i]->StartTime >= Timers[i]->Duration) {
					Observers[j]->Timer_Expired(Timers[i]->Number,Timers[i]->Data);
					if (Timers[i]->Repeat) {
						Timers[i]->StartTime = GetTickCount();
					}
					else {
						delete Timers[i];
						Timers.Delete(i);
						i--;
					}
				}
			}
		}
	}
}

void DAPlayerClass::DAPlayerTagStruct::Set_Flags(DAPlayerFlags::Flag Flag) {
	Flags = Flag;
}

bool DAPlayerClass::DAPlayerTagStruct::Has_Flag(DAPlayerFlags::Flag Flag) {
	return (Flags & Flag) == Flag;
}


void DAPlayerManager::Init() {
	static DAPlayerManager Instance;
	
	Instance.Register_Event(DAEvent::CHAT,INT_MAX);
	Instance.Register_Event(DAEvent::RADIO,INT_MAX);
	Instance.Register_Event(DAEvent::LEVELLOADED,INT_MAX);
	Instance.Register_Event(DAEvent::SETTINGSLOADED,INT_MAX);
	Instance.Register_Event(DAEvent::CONNECTIONREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::PLAYERJOIN,INT_MAX);
	Instance.Register_Event(DAEvent::PLAYERLEAVE,INT_MIN);
	Instance.Register_Event(DAEvent::PLAYERLOADED,INT_MAX);
	Instance.Register_Event(DAEvent::NAMECHANGE,INT_MAX);
	Instance.Register_Event(DAEvent::CHARACTERPURCHASEREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEPURCHASEREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::POWERUPPURCHASEREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEPURCHASE,INT_MAX);
	Instance.Register_Event(DAEvent::CHARACTERPURCHASE,INT_MAX);
	Instance.Register_Event(DAEvent::POWERUPPURCHASE,INT_MAX);
	Instance.Register_Event(DAEvent::TEAMCHANGE,INT_MAX);
	Instance.Register_Event(DAEvent::SUICIDE,INT_MIN);
	Instance.Register_Event(DAEvent::VEHICLEENTRYREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEENTER,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEEXIT,INT_MAX);
	Instance.Register_Event(DAEvent::POWERUPGRANTREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::POWERUPGRANT,INT_MAX);
	Instance.Register_Event(DAEvent::ADDWEAPONREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::ADDWEAPON,INT_MAX);
	Instance.Register_Event(DAEvent::REMOVEWEAPON,INT_MAX);
	Instance.Register_Event(DAEvent::CLEARWEAPONS,INT_MAX);
	Instance.Register_Event(DAEvent::C4DETONATEREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::C4DETONATE,INT_MAX);
	Instance.Register_Event(DAEvent::CHANGECHARACTER,INT_MAX);
	Instance.Register_Event(DAEvent::THINK,INT_MAX);
	
	Instance.Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::PLAYER,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::DESTROYED,DAObjectEvent::SOLDIER,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::DAMAGERECEIVEDREQUEST,DAObjectEvent::ALL,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::DAMAGERECEIVED,DAObjectEvent::ALL,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::KILLRECEIVED,DAObjectEvent::ALL,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::CUSTOM,DAObjectEvent::PLAYER,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::POKE,DAObjectEvent::PLAYER,INT_MAX);
}

void DAPlayerManager::Shutdown() {
	for (int i = 0;i < Players.Count();i++) {
		delete Players[i];
	}
	Players.Delete_All();
}

void DAPlayerManager::Add_Data_Factory(DAPlayerDataFactoryClass *Factory) {
	DataFactories.Add(Factory);
	for (int i = 0;i < Players.Count();i++) {
		Players[i]->Add_Data(Factory->Create_Data());
	}
}

void DAPlayerManager::Remove_Data_Factory(DAPlayerDataFactoryClass *Factory) {
	DataFactories.DeleteObj(Factory);
	for (int i = 0;i < Players.Count();i++) {
		Players[i]->Remove_Data(Factory);
	}
}

bool DAPlayerManager::Get_Disable_Kill_Messages() {
	return DisableKillMessages;
}

bool DAPlayerManager::Get_Disable_Kill_Counter() {
	return DisableKillCounter;
}

bool DAPlayerManager::Get_Disable_Team_Kill_Counter() {
	return DisableTeamKillCounter;
}

bool DAPlayerManager::Get_Disable_Death_Counter() {
	return DisableDeathCounter;
}

bool DAPlayerManager::Get_Disable_Team_Death_Counter() {
	return DisableTeamDeathCounter;
}

bool DAPlayerManager::Get_Disable_Damage_Points() {
	return DisableDamagePoints;
}

bool DAPlayerManager::Get_Disable_Death_Points() {
	return DisableDeathPoints;
}

bool DAPlayerManager::Get_Disable_Team_Score_Counter() {
	return DisableTeamScoreCounter;
}

float DAPlayerManager::Get_Credits_Multiplier() {
	return CreditsMultiplier;
}

void DAPlayerManager::Set_Disable_Kill_Messages(bool Enable) {
	DisableKillMessages = Enable;
}

void DAPlayerManager::Set_Disable_Kill_Counter(bool Disable) {
	DisableKillCounter = Disable;
}

void DAPlayerManager::Set_Disable_Team_Kill_Counter(bool Disable) {
	DisableTeamKillCounter = Disable;
}

void DAPlayerManager::Set_Disable_Death_Counter(bool Disable) {
	DisableDeathCounter = Disable;
}

void DAPlayerManager::Set_Disable_Team_Death_Counter(bool Disable) {
	DisableTeamDeathCounter = Disable;
}

void DAPlayerManager::Set_Disable_Damage_Points(bool Disable) {
	DisableDamagePoints = Disable;
}

void DAPlayerManager::Set_Disable_Death_Points(bool Disable) {
	DisableDeathPoints = Disable;
}

void DAPlayerManager::Set_Disable_Team_Score_Counter(bool Disable) {
	DisableTeamScoreCounter = Disable;
}

void DAPlayerManager::Set_Credits_Multiplier(float Multiplier) {
	CreditsMultiplier = Multiplier;
}

bool DAPlayerManager::Check_Player(DAPlayerClass *DAPlayer) {
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		if (z->Data() == DAPlayer->Get_Owner()) {
			return true;
		}
	}
	return false;
}

bool DAPlayerManager::Chat_Event(cPlayer *Player,TextMessageEnum Type,const wchar_t *Message,int ReceiverID) {
	return Player->Get_DA_Player()->Chat(Type,Message,ReceiverID);
}

bool DAPlayerManager::Radio_Event(cPlayer *Player,int PlayerType,int AnnouncementID,int IconID,AnnouncementEnum AnnouncementType) {
	return Player->Get_DA_Player()->Radio(PlayerType,AnnouncementID,IconID,AnnouncementType);
}

void DAPlayerManager::Level_Loaded_Event() {
	for (int i = 0;i < Players.Count();i++) {
		DAPlayerClass *DAPlayer = Players[i];
		if (Check_Player(DAPlayer)) { //Trigger loaded event if they're still ingame.
			DAPlayer->Level_Loaded();
		}
		else { //Delete player class if they're not.
			delete DAPlayer;
			Players.Delete(i);
			i--;
		}
	}
}

void DAPlayerManager::Settings_Loaded_Event() {
	//Disallowed nicks and characters
	StringClass Buffer;
	DASettingsManager::Get_String(Buffer,"DisallowedNickFirstCharacters"," ");
	DisallowedNickFirstCharacters = Buffer.Peek_Buffer();
	DASettingsManager::Get_String(Buffer,"DisallowedNickCharacters"," ");
	Buffer += ": ;";
	DisallowedNickCharacters = Buffer.Peek_Buffer();
	
	DisallowedNicks.Delete_All();
	DASettingsManager::Get_String(Buffer,"DisallowedNicks",0);
	DATokenParserClass Parser(Buffer,'|');
	while (const char *Token = Parser.Get_String()) {
		DisallowedNicks.Add(Token);
	}
	DisallowedNicks.Add(L"Host");
	DisallowedNicks.Add(The_Game()->Owner);
	WideStringClass MessagePrefix = DA::Get_Message_Prefix();
	if (!MessagePrefix.Is_Empty()) {
		for (int i = 0;i < MessagePrefix.Get_Length();i++) {
			if (MessagePrefix[i] == '(' || MessagePrefix[i] == ')' || MessagePrefix[i] == '[' || MessagePrefix[i] == ']' || MessagePrefix[i] == ':') {
				MessagePrefix.RemoveSubstring(i,1);
				i--;
			}
		}
		DisallowedNicks.Add(MessagePrefix);
	}

	//TT
	ForceTT = (unsigned int)DASettingsManager::Get_Int("ForceTT",0);
	TTRevision = (unsigned int)DASettingsManager::Get_Int("TTRevision",0);

	//Kill messages, score/kill/death counters, and points stuff.
	DisableKillMessages = DASettingsManager::Get_Bool("DisableKillMessages",false);
	DisableKillCounter = DASettingsManager::Get_Bool("DisableKillCounter",false);
	DisableTeamKillCounter = DASettingsManager::Get_Bool("DisableTeamKillCounter",false);
	DisableDeathCounter = DASettingsManager::Get_Bool("DisableDeathCounter",false);
	DisableTeamDeathCounter = DASettingsManager::Get_Bool("DisableTeamDeathCounter",false);
	DisableDamagePoints = DASettingsManager::Get_Bool("DisableDamagePoints",false);
	DisableDeathPoints = DASettingsManager::Get_Bool("DisableDeathPoints",false);
	DisableTeamScoreCounter = DASettingsManager::Get_Bool("DisableTeamScoreCounter",false);
	CreditsMultiplier = DASettingsManager::Get_Float("CreditsMultiplier",1.0f);
}

ConnectionAcceptanceFilter::STATUS DAPlayerManager::Connection_Request_Event(ConnectionRequest &Request,WideStringClass &RefusalMessage) {
	cGameData *TheGame = The_Game();
	
	CheckFirst:
	for (wchar_t *DisallowedPtr = DisallowedNickFirstCharacters.Peek_Buffer();*DisallowedPtr != '\0';DisallowedPtr++) {
		if (Request.clientName[0] == *DisallowedPtr) {
			Request.clientName.TruncateLeft(1);
			goto CheckFirst;
		}
	}
	for (int i = 0;i < Request.clientName.Get_Length();i++) {
		if (Request.clientName[i] < 33 || Request.clientName[i] > 126) {
			Request.clientName.RemoveSubstring(i,1);
			i--;
		}
		else {
			for (wchar_t *DisallowedPtr = DisallowedNickCharacters.Peek_Buffer();*DisallowedPtr != '\0';DisallowedPtr++) {
				if (Request.clientName[i] == *DisallowedPtr) {
					Request.clientName.RemoveSubstring(i,1);
					i--;
				}
			}
		}
	}
	if (Request.clientName.Get_Length() > 20) {
		Request.clientName.Erase(20,Request.clientName.Get_Length()-20);
	}
	if (DisallowedNicks.ID(Request.clientName) != -1) {
		RefusalMessage = L"Disallowed name.";
		return ConnectionAcceptanceFilter::STATUS_REFUSING;
	}
	else if (Request.clientName.Get_Length() <= 2) {
		RefusalMessage = L"Name too short.";
		return ConnectionAcceptanceFilter::STATUS_REFUSING;
	}
	else if (cPlayer *Player = Find_Player(Request.clientName)) {
		if (Request.clientAddress.sin_addr.s_addr == (unsigned int)Player->Get_Ip_Address() && Request.clientSerialHash == Player->Get_DA_Player()->Get_Serial()) { //Client reconnecting before their ghost has timed out.
			Evict_Client(Player->Get_Id(),L"Ghost");
		}
		else {
			RefusalMessage = L"Name already in use.";
			return ConnectionAcceptanceFilter::STATUS_REFUSING;
		}
	}

	if (TheGame->Is_Passworded() && Request.password.Compare(TheGame->Get_Password())) {
		RefusalMessage = L"Invalid password.";
		return ConnectionAcceptanceFilter::STATUS_REFUSING;
	}
	else if (Request.clientExeKey != TheGame->Get_Version_Number()) {
		RefusalMessage = L"Version mismatch.";
		return ConnectionAcceptanceFilter::STATUS_REFUSING;
	}	
	else if (TheGame->Get_Current_Players() > TheGame->Get_Max_Players()) {
		RefusalMessage = L"Game is full.";
		return ConnectionAcceptanceFilter::STATUS_REFUSING;
	}
	else if (Request.clientSerialHash.Get_Length() != 32) {
		RefusalMessage = L"Invalid serial.";
		return ConnectionAcceptanceFilter::STATUS_REFUSING;
	}
	else if (Request.clientVersion < 4.0f && ForceTT >= 2) {
		RefusalMessage = L"The Tiberian Technologies community patch is required to play on this server. You can download it at http://www.tiberiantechnologies.org/.";
		return ConnectionAcceptanceFilter::STATUS_REFUSING;
	}
	else if (Request.clientVersion >= 4.0f && Request.clientRevisionNumber < TTRevision) {
		RefusalMessage = L"Your version of Tiberian Technologies is outdated. Please launch your game through the TT launcher(game.exe) to automatically download the latest version.";
		return ConnectionAcceptanceFilter::STATUS_REFUSING;
	}
	return ConnectionAcceptanceFilter::STATUS_ACCEPTING;
}

void DAPlayerManager::Player_Join_Event(cPlayer *Player) {
	DAPlayerClass *DAPlayer = 0;
	for (int i = 0;i < Players.Count();i++) {
		if (Players[i]->Get_Owner() == Player) {
			Players[i]->Join(); //Trigger join event if they already exist.
			DAPlayer = Players[i];
			break;
		}
	}
	if (!DAPlayer) {
		DAPlayer = new DAPlayerClass(Player); //Create new player class if they don't.
		for (int i = 0;i < DataFactories.Count();i++) {
			DAPlayer->Add_Data(DataFactories[i]->Create_Data());
		}
		Players.Add(DAPlayer);
	}
	if (DAPlayer->Is_Stock_Client()) {
		if (ForceTT == 1) {
			DAPlayer->Set_Server_Damage(true);
		}
		StringClass Message("Download the Tiberian Technologies community patch at http://www.tiberiantechnologies.org/. It includes anti-cheat, automatic map downloading, and many more bug fixes and additions.");
		DA::Private_Admin_Message(Player,"%s",Message);
		DA::Page_Player(Player,"%s",Message);
	}

	//Send team kills, deaths, and score to new player.
	cTeam *Nod = Find_Team(0);
	cTeam *GDI = Find_Team(1);
	if (Nod) {
		Nod->Set_Object_Dirty_Bit(Player->Get_Id(),NetworkObjectClass::BIT_RARE,true);
		Nod->Set_Object_Dirty_Bit(Player->Get_Id(),NetworkObjectClass::BIT_OCCASIONAL,true);
	}
	if (GDI) {
		GDI->Set_Object_Dirty_Bit(Player->Get_Id(),NetworkObjectClass::BIT_RARE,true);
		GDI->Set_Object_Dirty_Bit(Player->Get_Id(),NetworkObjectClass::BIT_OCCASIONAL,true);
	}
}

void DAPlayerManager::Player_Leave_Event(cPlayer *Player) {
	Player->Get_DA_Player()->Leave();
}

void DAPlayerManager::Player_Loaded_Event(cPlayer *Player) {
	Player->Get_DA_Player()->Player_Loaded();
}

void DAPlayerManager::Name_Change_Event(cPlayer *Player) {
	Player->Get_DA_Player()->Name_Change();
}

int DAPlayerManager::Character_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const SoldierGameObjDef *Item) {
	if (!Player->Get_GameObj()->Get_Defense_Object()->Get_Health() || Player->Get_GameObj()->Get_Vehicle()) {
		return 3;
	}
	return Player->Get_DA_Player()->Character_Purchase_Request(Cost,Item);
}

int DAPlayerManager::Vehicle_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const VehicleGameObjDef *Item) {
	if (!Player->Get_GameObj()->Get_Defense_Object()->Get_Health()) { //Make it so players can't buy something if they died while in the PT screen.
		return 3;
	}
	return Player->Get_DA_Player()->Vehicle_Purchase_Request(Cost,Item);
}

int DAPlayerManager::PowerUp_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const PowerUpGameObjDef *Item) {
	if (!Player->Get_GameObj()->Get_Defense_Object()->Get_Health()) {
		return 3;
	}
	return Player->Get_DA_Player()->PowerUp_Purchase_Request(Cost,Item);
}

int DAPlayerManager::Custom_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,unsigned int ID) {
	if (!Player->Get_GameObj()->Get_Defense_Object()->Get_Health()) {
		return 3;
	}
	return Player->Get_DA_Player()->Custom_Purchase_Request(Cost,ID);
}

void DAPlayerManager::Character_Purchase_Event(cPlayer *Player,float Cost,const SoldierGameObjDef *Item) {
	DALogManager::Write_Log("_PURCHASE","%ls - %s",Player->Get_Name(),DATranslationManager::Translate(Item));
	Player->Get_DA_Player()->Character_Purchase(Cost,Item);
}

void DAPlayerManager::Vehicle_Purchase_Event(cPlayer *Player,float Cost,const VehicleGameObjDef *Item) {
	DALogManager::Write_Log("_PURCHASE","%ls - %s",Player->Get_Name(),DATranslationManager::Translate(Item));
	Player->Get_DA_Player()->Vehicle_Purchase(Cost,Item);
}

void DAPlayerManager::PowerUp_Purchase_Event(cPlayer *Player,float Cost,const PowerUpGameObjDef *Item) {
	DALogManager::Write_Log("_PURCHASE","%ls - %s",Player->Get_Name(),DATranslationManager::Translate(Item));
	Player->Get_DA_Player()->PowerUp_Purchase(Cost,Item);
}

void DAPlayerManager::Custom_Purchase_Event(cPlayer *Player,float Cost,unsigned int ID) {
	Player->Get_DA_Player()->Custom_Purchase(Cost,ID);
}

void DAPlayerManager::Team_Change_Event(cPlayer *Player) {
	Player->Get_DA_Player()->Team_Change();
}

bool DAPlayerManager::Suicide_Event(cPlayer *Player) { //Default suicide event.
	Player->Set_Money(0.0f);
	Player->Mark_As_Modified();
	Player->Destroy_GameObj();
	DA::Host_Message("%ls committed suicide.",Player->Get_Name());
	return false;
}

bool DAPlayerManager::Vehicle_Entry_Request_Event(VehicleGameObj* Vehicle, cPlayer* Player, int& Seat) {
	return Player->Get_DA_Player()->Vehicle_Entry_Request(Vehicle, Seat);
}

void DAPlayerManager::Vehicle_Enter_Event(VehicleGameObj* Vehicle, cPlayer* Player, int Seat) {
	Player->Get_DA_Player()->Vehicle_Enter(Vehicle, Seat);
}

void DAPlayerManager::Vehicle_Exit_Event(VehicleGameObj* Vehicle, cPlayer* Player, int Seat) {
	Player->Get_DA_Player()->Vehicle_Exit(Vehicle, Seat);
}

bool DAPlayerManager::PowerUp_Grant_Request_Event(cPlayer *Player,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) {
	if (!Player->Get_GameObj()->Get_Defense_Object()->Get_Health()) { //Fix bug where players can pickup powerups when they're dead.
		return false;
	}
	return Player->Get_DA_Player()->PowerUp_Grant_Request(PowerUp,PowerUpObj);
}

void DAPlayerManager::PowerUp_Grant_Event(cPlayer *Player,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) {
	Player->Get_DA_Player()->PowerUp_Grant(PowerUp,PowerUpObj);
}

bool DAPlayerManager::Add_Weapon_Request_Event(cPlayer *Player,const WeaponDefinitionClass *Weapon) {
	if (Player->Get_GameObj()->Get_Weapon_Bag()->Get_Count() >= 28) {
		return false;
	}
	return Player->Get_DA_Player()->Add_Weapon_Request(Weapon);
}

void DAPlayerManager::Add_Weapon_Event(cPlayer *Player,WeaponClass *Weapon) {
	if (Player->Get_GameObj()->Get_Weapon_Bag()->Get_Count() >= 28) {
		DA::Page_Player(Player,"You have reached the weapon limit of 28. To pick up additional weapons you will need to drop some of your current ones with the \"!drop\" command.");
	}
	Player->Get_DA_Player()->Add_Weapon(Weapon);
}

void DAPlayerManager::Remove_Weapon_Event(cPlayer *Player,WeaponClass *Weapon) {
	Player->Get_DA_Player()->Remove_Weapon(Weapon);
}

void DAPlayerManager::Clear_Weapons_Event(cPlayer *Player) {
	Player->Get_DA_Player()->Clear_Weapons();
}

bool DAPlayerManager::C4_Detonate_Request_Event(C4GameObj *C4,SmartGameObj *Triggerer) {
	if (Is_Player(Triggerer)) {
		return ((SoldierGameObj*)Triggerer)->Get_DA_Player()->C4_Detonate_Request(C4);
	}
	return true;
}

void DAPlayerManager::C4_Detonate_Event(C4GameObj *C4,SmartGameObj *Triggerer) {
	if (Is_Player(Triggerer)) {
		((SoldierGameObj*)Triggerer)->Get_DA_Player()->C4_Detonate(C4);
	}
}

void DAPlayerManager::Change_Character_Event(cPlayer *Player,const SoldierGameObjDef *Soldier) {
	Player->Get_DA_Player()->Change_Character(Soldier);
}

void DAPlayerManager::Think() {
	for (int i = 0;i < Players.Count();i++) {
		Players[i]->Think();
	}
}

void DAPlayerManager::Custom_Event(GameObject *obj,int Type,int Param,GameObject *Sender) {
	((SoldierGameObj*)obj)->Get_DA_Player()->Custom(Sender,Type,Param);
}

void DAPlayerManager::Poke_Event(cPlayer *Player,PhysicalGameObj *obj) {
	((SoldierGameObj*)obj)->Get_DA_Player()->Poked(Player);
}

void DAPlayerManager::Object_Created_Event(GameObject *obj) {
	((SoldierGameObj*)obj)->Get_DA_Player()->Created();
}

void DAPlayerManager::Object_Destroyed_Event(GameObject *obj) {
	cPlayer *Player = ((SoldierGameObj*)obj)->Get_Player();
	if (Player) {
		Player->Get_DA_Player()->Destroyed();
		if (!DisableDeathCounter && !Player->Get_DA_Player()->Is_Spawning() && ((Player->Is_Alive_And_Kicking() && ((SoldierGameObj*)obj)->Get_Player_Type() == Player->Get_Player_Type()) || !((SoldierGameObj*)obj)->Get_Defense_Object()->Get_Health())) { //Don't give a death if the player is leaving the game or changing teams, unless they're already dead.
			Player->Increment_Deaths();
		}
	}
	else if (!DisableTeamDeathCounter) { //Bot
		cTeam *Team = Find_Team(((SoldierGameObj*)obj)->Get_Player_Type());
		if (Team) {
			Team->Increment_Deaths();
		}
	}
}

bool DAPlayerManager::Damage_Request_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type) {
	if (Is_Player(Victim)) {
		return ((SoldierGameObj*)Victim)->Get_DA_Player()->Damage_Received_Request(Damager,Damage,Warhead,Scale,Type);
	}
	if (Is_Player(Damager)) {
		if (!((SoldierGameObj*)Damager)->Get_DA_Player()->Damage_Dealt_Request(Victim,Damage,Warhead,Scale,Type)) {
			return false;
		}
	}
	return true;
}

void DAPlayerManager::Damage_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type){
	if (Is_Player(Victim)) {
		((SoldierGameObj*)Victim)->Get_DA_Player()->Damage_Received(Damager,Damage,Warhead,Scale,Type);
	}
	if (Is_Player(Damager)) {
		((SoldierGameObj*)Damager)->Get_DA_Player()->Damage_Dealt(Victim,Damage,Warhead,Scale,Type);
	}
}

void DAPlayerManager::Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	if (Is_Player(Victim)) {
		((SoldierGameObj*)Victim)->Get_DA_Player()->Kill_Received(Killer,Damage,Warhead,Scale,Type);
	}
	if (Killer && Killer->As_SoldierGameObj()) {
		cPlayer *Player = ((SoldierGameObj*)Killer)->Get_Player();
		if (Player) {
			Player->Get_DA_Player()->Kill_Dealt(Victim,Damage,Warhead,Scale,Type);
			if (Victim->As_SoldierGameObj() && Killer != Victim) {
				if (!DisableKillCounter) {
					Player->Increment_Kills();
				}
				if (!DisableKillMessages && ((SoldierGameObj*)Victim)->Get_Player()) { //Send stock kill message.
					Send_Player_Kill_Message(Player->Get_Id(),((SoldierGameObj*)Victim)->Get_Player()->Get_Id());
				}
			}
		}
		else if (!DisableTeamKillCounter && Killer != Victim) { //Bot
			cTeam *Team = Find_Team(Killer->Get_Player_Type());
			if (Team) {
				Team->Increment_Kills();
			}
		}
	}
}

void cPlayer::Increment_Score(float Score) {
	if (Is_Gameplay_Permitted()) {
		this->Score += Score;
		if (!DAPlayerManager::Get_Disable_Team_Score_Counter()) {
			cTeam *Team = Find_Team(Get_Player_Type());
			if (Team) {
				Team->Increment_Score(Score);
			}
		}	
		if (Score > 0) {
			Money += Score * DAPlayerManager::Get_Credits_Multiplier();
		}
		Set_Object_Dirty_Bit(BIT_OCCASIONAL,true);
	}
}

void cPlayer::Increment_Kills() {
	if (Is_Gameplay_Permitted()) {
		Kills++;
		if (!DAPlayerManager::Get_Disable_Team_Kill_Counter()) {
			cTeam *Team = Find_Team(Get_Player_Type());
			if (Team) {
				Team->Increment_Kills();
			}
		}
		Set_Object_Dirty_Bit(BIT_OCCASIONAL,true);
	}
}

void cPlayer::Decrement_Kills() {
	if (Is_Gameplay_Permitted()) {
		Kills--;
		if (!DAPlayerManager::Get_Disable_Team_Kill_Counter()) {
			cTeam *Team = Find_Team(Get_Player_Type());
			if (Team) {
				Team->Decrement_Kills();
			}
		}
		Set_Object_Dirty_Bit(BIT_OCCASIONAL,true);
	}
}

void cPlayer::Increment_Deaths() {
	if (Is_Gameplay_Permitted()) {
		Deaths++;
		if (!DAPlayerManager::Get_Disable_Team_Death_Counter()) {
			cTeam *Team = Find_Team(Get_Player_Type());
			if (Team) {
				Team->Increment_Deaths();
			}
		}
		Set_Object_Dirty_Bit(BIT_OCCASIONAL,true);
	}
}

void cPlayer::Decrement_Deaths() {
	if (Is_Gameplay_Permitted()) {
		Deaths--;
		if (!DAPlayerManager::Get_Disable_Team_Death_Counter()) {
			cTeam *Team = Find_Team(Get_Player_Type());
			if (Team) {
				Team->Decrement_Deaths();
			}
		}
		Set_Object_Dirty_Bit(BIT_OCCASIONAL,true);
	}
}

bool PlayerDataClass::Purchase_Item(float money) {
	if (money > Money) {
		return false;
	}
	Set_Money(Money-money);
	return true;
}

class DALegacyTagConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "tag"; }
	const char *Get_Help() { return "TAG <playerid> <tag> - Legacy command to set the custom name tag on a player. Persists through leaving or multiple maps; defaults to position 0"; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		int ID = 0;
		Parser.Get_Int(ID);
		cPlayer *Player = Find_Player(ID);
		if (Player) {
			Player->Get_DA_Player()->Remove_Tags_With_Flag(DAPlayerFlags::LEGACYTAG);
			if (Parser.Get_Remaining_String()) {
				Player->Get_DA_Player()->Add_Tag(Parser.Get_Remaining_String(),0,DAPlayerFlags::LEGACYTAG | DAPlayerFlags::PERSISTLEAVE | DAPlayerFlags::PERSISTMAP);
			}
		}
	}
};
Register_Console_Function(DALegacyTagConsoleFunctionClass);

class DAAddTagConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "addtag"; }
	const char *Get_Help() { return "ADDTAG <playerid> <persist on leave> <persist on map> <position> <tag> - Add a custom name tag to a player. Persist on map/leave determines if this tag should be removed when the player leaves the game or a new map loads. Position determintes the tag's position relative to the player's other tags."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		int ID = 0;
		Parser.Get_Int(ID);
		const char *PersistLeave = Parser.Get_String();
		const char *PersistMap = Parser.Get_String();
		int Position;
		Parser.Get_Int(Position);
		const char *Tag = Parser.Get_Remaining_String();
		cPlayer *Player = Find_Player(ID);
		if (Player && Tag) {
			DAPlayerFlags::Flag Flags = (DAPlayerFlags::Flag)0;
			if (!_stricmp(PersistLeave,"1") || !_stricmp(PersistLeave,"true") || !_stricmp(PersistLeave,"yes")) {
				Flags |= DAPlayerFlags::PERSISTLEAVE;
			}
			if (!_stricmp(PersistMap,"1") || !_stricmp(PersistMap,"true") || !_stricmp(PersistMap,"yes")) {
				Flags |= DAPlayerFlags::PERSISTMAP;
			}
			Player->Get_DA_Player()->Add_Tag(Tag,Position,Flags);
		}
	}
};
Register_Console_Function(DAAddTagConsoleFunctionClass);

class DARemoveTagConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "removetag"; }
	const char *Get_Help() { return "REMOVETAG <playerid> <tag> - Removes the given tag from the player."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		int ID = 0;
		Parser.Get_Int(ID);
		const char *Tag = Parser.Get_Remaining_String();
		cPlayer *Player = Find_Player(ID);
		if (Player && Tag) {
			Player->Get_DA_Player()->Remove_Tag(Tag);
		}
	}
};
Register_Console_Function(DARemoveTagConsoleFunctionClass);

class DASetAccessLevelConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "setaccesslevel"; }
	const char *Get_Alias() { return "sal"; }
	const char *Get_Help() { return "SETACCESSLEVEL <playerid> <0-6> - Sets the given player's access level."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		int ID = 0;
		Parser.Get_Int(ID);
		int Level = 0;
		Parser.Get_Int(Level);
		cPlayer *Player = Find_Player(ID);
		if (Player) {
			Player->Get_DA_Player()->Set_Access_Level((DAAccessLevel::Level)Level);
		}
	}
};
Register_Console_Function(DASetAccessLevelConsoleFunctionClass);

class DAMuteConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "mute"; }
	const char *Get_Help() { return "MUTE <playerid> - Mute a player, preventing them from speaking in chat or using radio commands."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->Mute(true);
		}
	}
};
Register_Console_Function(DAMuteConsoleFunctionClass);

class DAUnMuteConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "unmute"; }
	const char *Get_Help() { return "UNMUTE <playerid> - Unmute a player."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->Mute(false);
		}
	}
};
Register_Console_Function(DAUnMuteConsoleFunctionClass);

class DAMuteToggleConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "mutetoggle"; }
	const char *Get_Help() { return "MUTETOGGLE <playerid> - Mute or unmute a player, preventing them from speaking in chat or using radio commands."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->Mute(!Player->Get_DA_Player()->Is_Muted());
		}
	}
};
Register_Console_Function(DAMuteToggleConsoleFunctionClass);

class DABeaconLockConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "beaconlock"; }
	const char *Get_Alias() { return "beaconmute"; }
	const char *Get_Help() { return "BEACONLOCK <playerid> - Prevent a player from deploying beacons."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->Beacon_Lock(true);
		}
	}
};
Register_Console_Function(DABeaconLockConsoleFunctionClass);

class DAUnBeaconLockConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "unbeaconlock"; }
	const char *Get_Alias() { return "unbeaconmute"; }
	const char *Get_Help() { return "UNBEACONLOCK <playerid> - Re-allow a player to deploy beacons."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->Beacon_Lock(false);
		}
	}
};
Register_Console_Function(DAUnBeaconLockConsoleFunctionClass);

class DABeaconLockToggleConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "beaconlocktoggle"; }
	const char *Get_Alias() { return "beaconmutetoggle"; }
	const char *Get_Help() { return "BEACONLOCKTOGGLE <playerid> - Prevent or re-allow a player from deploying beacons."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->Beacon_Lock(!Player->Get_DA_Player()->Is_Beacon_Locked());
		}
	}
};
Register_Console_Function(DABeaconLockToggleConsoleFunctionClass);

class DAC4LockConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "c4lock"; }
	const char *Get_Alias() { return "c4mute"; }
	const char *Get_Help() { return "C4LOCK <playerid> - Prevent a player from using remote and proximity C4."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->C4_Lock(true);
		}
	}
};
Register_Console_Function(DAC4LockConsoleFunctionClass);

class DAUnC4LockConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "unc4lock"; }
	const char *Get_Alias() { return "unc4mute"; }
	const char *Get_Help() { return "UNC4LOCK <playerid> - Re-allow a player to use remote and proximity C4."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->C4_Lock(false);
		}
	}
};
Register_Console_Function(DAUnC4LockConsoleFunctionClass);

class DAC4LockToggleConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "c4locktoggle"; }
	const char *Get_Alias() { return "c4mutetoggle"; }
	const char *Get_Help() { return "C4LOCKTOGGLE <playerid> - Prevent or re-allow a player from using remote and proximity C4."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->C4_Lock(!Player->Get_DA_Player()->Is_C4_Locked());
		}
	}
};
Register_Console_Function(DAC4LockToggleConsoleFunctionClass);

class SDEConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "sde"; }
	const char *Get_Help() { return "SDE <playerid> - Enable server damage extrapolation for the given player."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->Set_Server_Damage(true);
		}
	}
};
Register_Console_Function(SDEConsoleFunctionClass);

class UnSDEConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "unsde"; }
	const char *Get_Help() { return "UNSDE <playerid> - Disable server damage extrapolation for the given player."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->Set_Server_Damage(false);
		}
	}
};
Register_Console_Function(UnSDEConsoleFunctionClass);

class SDEToggleConsoleFunctionClass : public ConsoleFunctionClass {
	const char *Get_Name() { return "sdetoggle"; }
	const char *Get_Help() { return "SDETOGGLE <playerid> - Enable or disable server damage extrapolation for the given player."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->Set_Server_Damage(!Player->Get_DA_Player()->Use_Server_Damage());
		}
	}
};
Register_Console_Function(SDEToggleConsoleFunctionClass);

class MapCHConsoleFunctionClass : public ConsoleFunctionClass {
	const char* Get_Name() { return "mapch"; }
	const char* Get_Help() { return "MAPCH <playerid> <file> - Check if a given client has a given file. Host only."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		int ID;
		Parser.Get_Int(ID);
		if (Find_Player(ID) && Parser.Get_Remaining_String()) {
			Send_Client_Text(WideStringFormat(L"j\n71\n%d\n%hs\n",ID,Parser.Get_Remaining_String()),TEXT_MESSAGE_PRIVATE,true,-2,ID,true,true);
		}
	}
};
Register_Console_Function(MapCHConsoleFunctionClass);



class DAVoteYesKeyHookClass : public DAKeyHookClass {
	void Activate(cPlayer *Player) {
		if (DAEventManager::Chat_Event(Player,TEXT_MESSAGE_PUBLIC,L"!vote yes",-1)) {
			Console_Output("%ls: !vote yes\n",Player->Get_Name());
			Player->Increment_Flood_Counter();
			Player->Increment_Flood_Counter();
		}
	}
};
Register_Simple_Key_Hook(DAVoteYesKeyHookClass,"VoteYes");

class DAVoteNoKeyHookClass : public DAKeyHookClass {
	void Activate(cPlayer *Player) {
		if (DAEventManager::Chat_Event(Player,TEXT_MESSAGE_PUBLIC,L"!vote no",-1)) {
			Console_Output("%ls: !vote no\n",Player->Get_Name());
			Player->Increment_Flood_Counter();
			Player->Increment_Flood_Counter();
		}
	}
};
Register_Simple_Key_Hook(DAVoteNoKeyHookClass,"VoteNo");


class DAUnstuckObserverClass : public DAGameObjObserverClass {
	void Init() {
		Get_Owner()->Get_Position(&Position);
		Start_Timer(1,10.0f);
	}
	void Timer_Expired(GameObject *obj,int Number) {
		if (Commands->Get_Distance(Commands->Get_Position(Get_Owner()),Position) < 0.5f) {
			SoldierGameObj *Soldier = (SoldierGameObj*)Get_Owner();
			if (Soldier) {
				PhysicalGameObj *Phys = Soldier;
				if (Soldier->Get_Vehicle()) { 
					if (Soldier->Get_Vehicle()->Get_Driver() == Soldier) {
						Phys = Soldier->Get_Vehicle();
					}
					else {
						Set_Delete_Pending();
						return;
					}
				}
				if (!Phys->Is_Attached_To_An_Object() && Fix_Stuck_Object(Phys,10.0f)) {
					DA::Page_Player(Soldier,"You have been unstuck.");
				}
				else {
					DA::Page_Player(Soldier,"Unstuck failed. You can try again, or use the \"!killme\" command.");
				}
			}
		}
		Set_Delete_Pending();
	}
	const char *Get_Name() { return "DAUnstuckObserverClass"; }
	Vector3 Position;
};

class DAUnStuckChatCommandClass : public DAChatCommandClass {
	bool Activate(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
		if (ChatType != TEXT_MESSAGE_KEYHOOK && !Player->Get_GameObj()->Find_Observer("DAUnstuckObserverClass")) {
			Player->Get_GameObj()->Add_Observer(new DAUnstuckObserverClass);
			DA::Page_Player(Player,"You will be unstuck in 10 seconds. Moving will cancel this.");
		}
		return true;
	}
};
Register_Simple_Chat_Command(DAUnStuckChatCommandClass,"!unstuck|!unstick|!stuck|!stick");



class DAKillMeObserverClass : public DAGameObjObserverClass {
	void Init() {
		Get_Owner()->Get_Position(&Position);
		Start_Timer(1,10.0f);
	}
	void Timer_Expired(GameObject *obj,int Number) {
		if (Commands->Get_Distance(Commands->Get_Position(Get_Owner()),Position) < 0.5f) {
			Get_Owner()->Set_Delete_Pending();
		}
		Set_Delete_Pending();
	}
	const char *Get_Name() { return "DAKillMeObserverClass"; }
	Vector3 Position;
};

class DAKillMeChatCommandClass : public DAChatCommandClass {
	bool Activate(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
		if (ChatType != TEXT_MESSAGE_KEYHOOK && !Player->Get_GameObj()->Find_Observer("DAKillMeObserverClass")) {
			Player->Get_GameObj()->Add_Observer(new DAKillMeObserverClass);
			DA::Page_Player(Player,"You will be killed in 10 seconds. Moving will cancel this.");
		}
		return true;
	}
};
Register_Simple_Chat_Command(DAKillMeChatCommandClass,"!killme|!suicide");



class DADropChatCommandClass: public DAChatCommandClass {
	bool Activate(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
		WeaponBagClass *Bag = Player->Get_GameObj()->Get_Weapon_Bag();
		if (Bag->Get_Index()) {
			Bag->Remove_Weapon(Bag->Get_Index());
		}
		return true;
	}
};
Register_Simple_Chat_Command(DADropChatCommandClass,"!drop|!wdrop|!weapdrop|!weapondrop|!dropweap|!dropweapon|!rweapon|!removeweapon|WeapDrop");
