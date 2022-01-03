/*	Renegade Scripts.dll
    Dragonade Loot Game Feature
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
#include "da_loot.h"
#include "da_settings.h"
#include "da_translation.h"
#include "da_damagelog.h"
#include "da_gameobjobservers.h"
#include "WeaponBagClass.h"
#include "WeaponClass.h"

void DALootPlayerDataClass::Add_Weapon(WeaponClass *Weapon) {
	if (Locker.ID(Weapon->Get_Definition()) == -1) {
		Locker.Add(Weapon->Get_Definition());
	}
}

void DALootPlayerDataClass::Add_Weapon(const WeaponDefinitionClass *Weapon) {
	if (Locker.ID(Weapon) == -1) {
		Locker.Add(Weapon);
	}
}

void DALootPlayerDataClass::Remove_Weapon(WeaponClass *Weapon) {
	Locker.DeleteObj(Weapon->Get_Definition());
}

void DALootPlayerDataClass::Remove_Weapon(const WeaponDefinitionClass *Weapon) {
	Locker.DeleteObj(Weapon);
}

bool DALootPlayerDataClass::Has_Weapon(WeaponClass *Weapon) {
	return (Locker.ID(Weapon->Get_Definition()) != -1);
}

bool DALootPlayerDataClass::Has_Weapon(const WeaponDefinitionClass *Weapon) {
	return (Locker.ID(Weapon) != -1);
}

void DALootPowerUpClass::Set_Expire_Time(float Time) {
	Stop_Timer(1);
	Stop_Timer(2);
	if (Time > 1.0f) {
		Start_Timer(1,Time-1.0f);
		Start_Timer(2,Time);
	}
}

//Create list of all players that damaged the dropper.
void DALootPowerUpClass::Init_Damagers(float Range,float ExpireTime) {
	if (!Get_Dropper()) {
		return;
	}
	SoldierGameObj *Soldier = Get_Dropper()->Get_GameObj();
	if (!Soldier) {
		return;
	}
	Init_Damagers(Range,ExpireTime,Soldier);
}

void DALootPowerUpClass::Init_Damagers(float Range,float ExpireTime,SoldierGameObj *Soldier) {
	if (!Soldier) {
		return;
	}
	if (ExpireTime > 0.0f) {
		Start_Timer(3,ExpireTime);
	}
	DADamageTableStruct DamageTable[128];
	DADamageLog::Compile_Damage_Table_Other_Team(DamageTable,Soldier,Soldier->Get_Player_Type());
	Vector3 Position;
	Soldier->Get_Position(&Position);
	for (int i = 1;i < 128;i++) {
		if (DamageTable[i].Player && DamageTable[i].Player->Get_GameObj()) {
			if (Commands->Get_Distance(DamageTable[i].Player->Get_GameObj()->Get_Position(),Position) <= Range) {
				Add_Damager(DamageTable[i].Player);
			}
		}
	}
}

void DALootPowerUpClass::Add_Damager(cPlayer *Player) {
	Damagers.Add(Player);
}

//Only allow players in the damager list to pickup the powerup. If there are no damagers allow anyone.
bool DALootPowerUpClass::PowerUp_Grant_Request(cPlayer *Player) {
	if (Player != Dropper) {
		for (int i = Damagers.Count()-1;i >= 0;i--) {
			if (!Damagers[i]) {
				Damagers.Delete(i);
			}
		}
		if (!Dropper || !Damagers.Count() || Damagers.ID(Player) != -1 || Player->Get_Player_Type() == Dropper->Get_Player_Type()) {
			return true;
		}
	}
	return false;
}

//Play sound and destroy powerup.
void DALootPowerUpClass::PowerUp_Grant(cPlayer *Player) {
	const PowerUpGameObjDef *Pow = &((PowerUpGameObj*)Get_Owner())->Get_Definition();
	if (!Pow->Get_Grant_Sound()) {
		Create_2D_WAV_Sound_Player(Player->Get_GameObj(), "powerup_ammo.wav");
	}
	Get_Owner()->Set_Delete_Pending();
	StringClass HUD;
	if (Pow->GrantHealthMax) {
		HUD = "Augmented Health";
	}
	else if (Pow->GrantShieldStrengthMax) {
		HUD = "Augmented Armor";
	}
	else if (Pow->GrantHealth) {
		HUD = "Health";
	}
	else if (Pow->GrantShieldStrength) {
		HUD = "Armor";
	}
	else if (Pow->GrantWeaponClips) {
		HUD = "Ammo";
	}
	else if (Pow->GrantWeapon && Pow->GrantWeaponID) {
		HUD = DATranslationManager::Translate(Pow->GrantWeaponID);
	}
	if (!HUD.Is_Empty()) {
		DA::Private_HUD_Message(Player, COLORGREEN, "%s", HUD);
	}
}

//Timeout and clear damagers list.
void DALootPowerUpClass::Timer_Expired(GameObject *obj,int Number) {
	if (Number == 1) {
		Commands->Create_Object("Spawner Created Special Effect",Commands->Get_Position(obj));
	}
	else if (Number == 2) {
		Commands->Destroy_Object(obj);
	}
	else if (Number == 3) {
		Damagers.Delete_All();
	}
}

void DALootPowerUpClass::Create_Icon() {
	Destroy_Icon();
	Icon = Create_Object("Invisible_Object",Vector3());
	((PhysicalGameObj*)Icon.Get_Ptr())->Set_Collision_Group(UNCOLLIDEABLE_GROUP);
}

void DALootPowerUpClass::Set_Icon(const WeaponDefinitionClass *Weapon) {
	PhysicalGameObj *IconPhys = (PhysicalGameObj*)Icon.Get_Ptr();
	if (IconPhys) {
		Vector3 Position;
		Get_Owner()->Get_Position(&Position);
		StringClass PowerUpModel = Get_Weapon_PowerUp_Model(Weapon);
		FileClass *File = Get_Data_File(PowerUpModel+".w3d");
		if (File->Is_Available()) { //Use powerup model for spinning animation.
			Position.Z += 0.75f;
			if (Position != IconPhys->Get_Position()) {
				IconPhys->Set_Position(Position);
				IconPhys->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_CREATION,true);
			}
			Commands->Set_Model(IconPhys,PowerUpModel);
			Commands->Set_Animation(IconPhys,StringFormat("%s.%s",PowerUpModel,PowerUpModel),true,0,0,-1.0f,false);
		}
		else { //Use weapon model if there is no powerup model.
			Position.Z += 1.15f;
			if (Position != IconPhys->Get_Position()) {
				IconPhys->Set_Position(Position);
				IconPhys->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_CREATION,true);
			}
			Commands->Set_Model(IconPhys,Clean_Model_Name(Weapon->Model));
			Commands->Set_Animation(IconPhys,0,true,0,0,-1.0f,false);
		}
		Close_Data_File(File);
	}
}

void DALootPowerUpClass::Destroy_Icon() {
	if (Icon) {
		DAGameObjManager::Set_GameObj_Delete_Pending(Icon);
	}
}

DALootPowerUpClass::~DALootPowerUpClass() {
	Destroy_Icon();
}



void DALootBackpackClass::Init() {
	Start_Timer(4,0.75f);
	Get_Owner()->Get_Position(&LastPos);
}

void DALootBackpackClass::Add_Weapon(const WeaponDefinitionClass *Weapon,int Rounds) {
	WeaponStruct Struct;
	Struct.Weapon = Weapon;
	Struct.Rounds = Rounds;
	if (Weapons.ID(Struct) == -1) {
		Weapons.Add(Struct);
	}
}

//Grant players the weapons in the backpack.
void DALootBackpackClass::PowerUp_Grant(cPlayer *Player) {
	WeaponBagClass *Bag = Player->Get_GameObj()->Get_Weapon_Bag();
	bool PickedUp = false;
	bool CycleIcon = false;
	for (int i = Weapons.Count()-1;i >= 0;i--) {
		if (Bag->Add_Weapon(Weapons[i].Weapon,Weapons[i].Rounds,true)) { //Make sure they need and can receive the weapon before we remove it from the backpack.
			if (!PickedUp) {
				DA::Private_HUD_Message(Player, COLORGREEN, "%s",DATranslationManager::Translate(Weapons[i].Weapon));
			}
			Weapons.Delete(i);
			PickedUp = true;
			if (i == IconIndex) {
				CycleIcon = true;
			}
		}
	}
	if (PickedUp) { //Only play sound if they actually got a weapon out of it.
		Create_2D_WAV_Sound_Player(Player->Get_GameObj(),"powerup_ammo.wav");
		if (CycleIcon && Icon) { //Immediately cycle icon so it doesn't show a weapon that isn't in the backpack anymore.
			Stop_Timer(5);
			Timer_Expired(Get_Owner(),5);
		}
	}
	if (!Weapons.Count()) { //Destroy backpack when empty.
		Get_Owner()->Set_Delete_Pending();
	}
}

void DALootBackpackClass::Timer_Expired(GameObject *obj,int Number) {
	if (Number == 4) { //Create icon.
		if (Weapons.Count()) {
			Vector3 Position;
			Get_Owner()->Get_Position(&Position);
			if (Position == LastPos) { //Wait until the powerup has stopped falling before creating the icon.
				Create_Icon();
				Set_Icon(Weapons[0].Weapon);
				IconIndex = 0;
				Start_Timer(5,2.5f);
			}
			else {
				LastPos = Position;
				Start_Timer(4,0.1f);
			}
		}
		else { //Destroy backpack if no weapons were added to it.
			Get_Owner()->Set_Delete_Pending();
		}
	}
	else if (Number == 5) { //Cycle icon.
		if (Weapons.Count()) {
			IconIndex++;
			if (IconIndex >= Weapons.Count()) {
				IconIndex = 0;
			}
			Set_Icon(Weapons[IconIndex].Weapon);
		}
		Start_Timer(5,2.5f);
	}
	else if (Number == 6) { //Clear dropper.
		Dropper = 0;
	}
	else {
		DALootPowerUpClass::Timer_Expired(obj,Number);
	}
}



void DALootDNAClass::Init() {
	Get_Owner()->Get_Position(&LastPos);
	Start_Timer(4,1.0f);
}

bool DALootDNAClass::PowerUp_Grant_Request(cPlayer *Player) {
	if (Player->Get_GameObj()->Get_Vehicle()) { //Can't change a player's character while they're in a vehicle, so prevent them from picking it up.
		return false;
	}
	return DALootPowerUpClass::PowerUp_Grant_Request(Player);
}

void DALootDNAClass::PowerUp_Grant(cPlayer *Player) {
	SoldierGameObj *Soldier = Player->Get_GameObj();
	Create_2D_WAV_Sound_Player(Soldier,"powerup_ammo.wav");
	Soldier->Re_Init(*Character);
	Soldier->Post_Re_Init();
	Get_Owner()->Set_Delete_Pending();
	DA::Private_HUD_Message(Player, COLORLIGHTBLUE, "%s",DATranslationManager::Translate(Character));
}

void DALootDNAClass::Timer_Expired(GameObject *obj,int Number) {
	if (Number == 4) { //Create icon.
		Vector3 Position;
		Get_Owner()->Get_Position(&Position);
		if (Position == LastPos) {
			const WeaponDefinitionClass *Weapon = (WeaponDefinitionClass*)Find_Definition(Character->WeaponDefID);
			if (Weapon) {
				Create_Icon();
				Set_Icon(Weapon);
			}
		}
		else {
			LastPos = Position;
			Start_Timer(4,0.1f);
		}
	}
	else {
		DALootPowerUpClass::Timer_Expired(obj,Number);
	}
}



const PowerUpGameObjDef *DALootGameFeatureClass::Get_Character_PowerUp_Drop(SoldierGameObj *Soldier) {
	if (!Soldier) {
		return 0;
	}
	const DynamicVectorClass<const PowerUpGameObjDef*> &PowerUps = CharacterPowerUps.Get((unsigned int)&Soldier->Get_Definition(),DefaultPowerUps);
	if (!PowerUps.Count()) {
		return 0;
	}
	return PowerUps[Get_Random_Int(0,PowerUps.Count())];
}

const PowerUpGameObjDef *DALootGameFeatureClass::Get_Character_PowerUp_Drop(const SoldierGameObjDef *Soldier) {
	const DynamicVectorClass<const PowerUpGameObjDef*> &PowerUps = CharacterPowerUps.Get((unsigned int)Soldier,DefaultPowerUps);
	if (!PowerUps.Count()) {
		return 0;
	}
	return PowerUps[Get_Random_Int(0,PowerUps.Count())];
}

DALootPowerUpClass *DALootGameFeatureClass::Create_PowerUp(SoldierGameObj *Soldier) {
	const PowerUpGameObjDef *PowerUpDef = Get_Character_PowerUp_Drop(Soldier);
	if (!PowerUpDef) {
		return 0;
	}
	DALootPowerUpClass *PowerUp = new DALootPowerUpClass(Soldier);
	Create_Object(PowerUpDef,Soldier->Get_Transform())->Add_Observer(PowerUp);
	PowerUp->Set_Expire_Time(ExpireTime);
	return PowerUp;
}

DALootPowerUpClass *DALootGameFeatureClass::Create_PowerUp(const Vector3 &Position,const SoldierGameObjDef *Soldier) {
	const PowerUpGameObjDef *PowerUpDef = Get_Character_PowerUp_Drop(Soldier);
	if (!PowerUpDef) {
		return 0;
	}
	DALootPowerUpClass *PowerUp = new DALootPowerUpClass;
	Create_Object(PowerUpDef,Position)->Add_Observer(PowerUp);
	PowerUp->Set_Expire_Time(ExpireTime);
	return PowerUp;
}

const WeaponDefinitionClass *DALootGameFeatureClass::Get_Character_Weapon_Drop(SoldierGameObj *Soldier) {
	if (!Soldier) {
		return 0;
	}
	return CharacterWeapons.Get((unsigned int)&Soldier->Get_Definition(),0);
}

const WeaponDefinitionClass *DALootGameFeatureClass::Get_Character_Weapon_Drop(const SoldierGameObjDef *Soldier) {
	return CharacterWeapons.Get((unsigned int)Soldier,0);
}

bool DALootGameFeatureClass::Is_Droppable(WeaponClass *Weapon) {
	return (UndroppableWeapons.ID(Weapon->Get_Definition()) == -1);
}

bool DALootGameFeatureClass::Is_Droppable(const WeaponDefinitionClass *Weapon) {
	return (UndroppableWeapons.ID(Weapon) == -1);
}

DALootBackpackClass *DALootGameFeatureClass::Create_Backpack(SoldierGameObj *Soldier) {
	DALootBackpackClass *Backpack = new DALootBackpackClass(Soldier);
	PhysicalGameObj *BackpackObj = Create_Object(BasePowerUpDef,Soldier->Get_Transform());
	BackpackObj->Add_Observer(Backpack);
	Commands->Set_Model(BackpackObj,WeaponModel);
	Commands->Set_Animation(BackpackObj,StringFormat("%s.%s",WeaponModel,WeaponModel),true,0,0,-1.0f,false);
	Backpack->Set_Expire_Time(ExpireTime);
	return Backpack;
}

DALootBackpackClass *DALootGameFeatureClass::Create_Backpack(const Vector3 &Position) {
	DALootBackpackClass *Backpack = new DALootBackpackClass;
	PhysicalGameObj *BackpackObj = Create_Object(BasePowerUpDef,Position);
	BackpackObj->Add_Observer(Backpack);
	Commands->Set_Model(BackpackObj,WeaponModel);
	Commands->Set_Animation(BackpackObj,StringFormat("%s.%s",WeaponModel,WeaponModel),true,0,0,-1.0f,false);
	Backpack->Set_Expire_Time(ExpireTime);
	return Backpack;
}

DALootDNAClass *DALootGameFeatureClass::Create_DNA(SoldierGameObj *Soldier) {
	DALootDNAClass *DNA = new DALootDNAClass(Soldier);
	PhysicalGameObj *DNAObj = Create_Object(BasePowerUpDef,Soldier->Get_Transform());
	DNAObj->Add_Observer(DNA);
	Commands->Set_Model(DNAObj,DNAModel);
	Commands->Set_Animation(DNAObj,StringFormat("%s.%s",DNAModel,DNAModel),true,0,0,-1.0f,false);
	DNA->Set_Expire_Time(ExpireTime);
	return DNA;
}

DALootDNAClass *DALootGameFeatureClass::Create_DNA(const Vector3 &Position,const SoldierGameObjDef *Character) {
	DALootDNAClass *DNA = new DALootDNAClass(Character);
	PhysicalGameObj *DNAObj = Create_Object(BasePowerUpDef,Position);
	DNAObj->Add_Observer(DNA);
	Commands->Set_Model(DNAObj,DNAModel);
	Commands->Set_Animation(DNAObj,StringFormat("%s.%s",DNAModel,DNAModel),true,0,0,-1.0f,false);
	DNA->Set_Expire_Time(ExpireTime);
	return DNA;
}

void DALootGameFeatureClass::Init() {
	DAPlayerDataManagerClass<DALootPlayerDataClass>::Init();
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Event(DAEvent::ADDWEAPON);
	Register_Event(DAEvent::REMOVEWEAPON);
	Register_Event(DAEvent::CLEARWEAPONS);
	Register_Event(DAEvent::CHANGECHARACTER);
	Register_Object_Event(DAObjectEvent::DESTROYED,DAObjectEvent::PLAYER);
}

void DALootGameFeatureClass::Settings_Loaded_Event() {
	//Expiration time and damager settings.
	ExpireTime = DASettingsManager::Get_Float("Loot","ExpireTime",20.0f);
	DropCommandExpireTime = DASettingsManager::Get_Float("Loot","DropCommandExpireTime",60.0f);
	DamagersOnlyTime = DASettingsManager::Get_Float("Loot","DamagersOnlyTime",5.0f);
	DamagersOnlyDistance = DASettingsManager::Get_Float("Loot","DamagersOnlyDistance",10.0f);

	//Default drop odds.
	DropOdds.PowerUp = DASettingsManager::Get_Int("Loot","PowerUpOdds",0);
	DropOdds.Weapon = DASettingsManager::Get_Int("Loot","WeaponOdds",100);
	DropOdds.DNA = DASettingsManager::Get_Int("Loot","DNAOdds",0);
	DropOdds.Total = DropOdds.PowerUp+DropOdds.Weapon+DropOdds.DNA;

	//Drop command.
	Unregister_Chat_Command("!drop");
	if (DASettingsManager::Get_Bool("Loot","EnableDropCommand",true)) {
		Register_Chat_Command((DAECC)&DALootGameFeatureClass::Drop_Chat_Command,"!drop|!wdrop|!weapdrop|!weapondrop|!dropweap|!dropweapon|!rweapon|!removeweapon|WeapDrop");
	}

	//Carry over.
	Unregister_Object_Event(DAObjectEvent::CREATED);
	if (DASettingsManager::Get_Bool("Loot","EnableWeaponCarryOver",true)) {
		Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::PLAYER);
	}

	//Drop multiple weapons.
	EnableMultiWeaponDrop = DASettingsManager::Get_Bool("Loot","EnableMultiWeaponDrop",true);

	//Models
	DASettingsManager::Get_String(WeaponModel,"Loot","WeaponModel","p_backpack");
	DASettingsManager::Get_String(DNAModel,"Loot","DNAModel","p_tnanites");

	//Per-character drop odds.
	CharacterDropOdds.Remove_All();
	INISection *Section = DASettingsManager::Get_Section("Loot");
	if (Section) {
		for (INIEntry *i = Section->EntryList.First();i && i->Is_Valid();i = i->Next()) {
			DefinitionClass *Def = Find_Named_Definition(i->Entry);
			if (Def && Def->Get_Class_ID() == CID_Soldier) {
				DATokenParserClass Parser(i->Value,'|');
				DropOddsStruct Odds;
				char *Token = Parser.Get_String();
				if (Token) { //<powerup %>|<weapon %>|<DNA %>
					if (Is_Numeric(Token)) {
						Odds.PowerUp = atoi(Token);
					}
					else {
						Odds.PowerUp = DropOdds.PowerUp; //Use default value if not a number.
					}
					Token = Parser.Get_String();
					if (Token) {
						if (Is_Numeric(Token)) {
							Odds.Weapon = atoi(Token);
						}
						else {
							Odds.Weapon = DropOdds.Weapon;
						}
						Token = Parser.Get_String();
						if (Token) {
							if (Is_Numeric(Token)) {
								Odds.DNA = atoi(Token);
							}
							else {
								Odds.DNA = DropOdds.DNA;
							}
							Odds.Total = Odds.PowerUp+Odds.Weapon+Odds.DNA;
							CharacterDropOdds.Insert((unsigned int)Def,Odds);
						}
					}
				}
			}
		}
	}

	//Default powerup drops.
	DefaultPowerUps.Delete_All();
	StringClass Default;
	DASettingsManager::Get_String(Default,"Loot_PowerUps","Default",0);
	DATokenParserClass Parser(Default,'|');
	while (char *Token = Parser.Get_String()) {
		const PowerUpGameObjDef *PowerUpDef = (PowerUpGameObjDef*)Find_Named_Definition(Token);
		if (PowerUpDef && PowerUpDef->Get_Class_ID() == CID_PowerUp) {
			DefaultPowerUps.Add(PowerUpDef);
		}
	}

	//Per-character powerup drops.
	CharacterPowerUps.Remove_All();
	Section = DASettingsManager::Get_Section("Loot_PowerUps");
	if (Section) {
		for (INIEntry *i = Section->EntryList.First();i && i->Is_Valid();i = i->Next()) {
			DefinitionClass *Def = Find_Named_Definition(i->Entry);
			if (Def && Def->Get_Class_ID() == CID_Soldier) {
				DATokenParserClass Parser(i->Value,'|');
				DynamicVectorClass<const PowerUpGameObjDef *> PowerUps;
				while (char *Token = Parser.Get_String()) {
					const PowerUpGameObjDef *PowerUpDef = (PowerUpGameObjDef*)Find_Named_Definition(Token);
					if (PowerUpDef && PowerUpDef->Get_Class_ID() == CID_PowerUp) {
						PowerUps.Add(PowerUpDef);
					}
				}
				if (PowerUps.Count()) {
					CharacterPowerUps.Insert((unsigned int)Def,PowerUps);
				}
			}
		}
	}

	//Undroppable weapons and per-character weapon drops.
	UndroppableWeapons.Delete_All();
	CharacterWeapons.Remove_All();
	Section = DASettingsManager::Get_Section("Loot_Weapons");
	if (Section) {
		for (INIEntry *i = Section->EntryList.First();i && i->Is_Valid();i = i->Next()) {
			DefinitionClass *Def = Find_Named_Definition(i->Entry);
			if (Def) {
				if (Def->Get_Class_ID() == CID_Weapon) { //Undroppable weapons
					if (!_stricmp(i->Value,"1") || !_stricmp(i->Value,"true")) {
						UndroppableWeapons.Add((WeaponDefinitionClass*)Def);
					}
				}
				else if (Def->Get_Class_ID() == CID_Soldier) { //Soldier -> Weapon overrides
					const WeaponDefinitionClass *WeaponDef = (WeaponDefinitionClass*)Find_Named_Definition(i->Value);
					if (WeaponDef && WeaponDef->Get_Class_ID() == CID_Weapon) {
						CharacterWeapons.Insert((unsigned int)Def,WeaponDef);
					}
				}
			}
		}
	}
	for (SoldierGameObjDef *SoldierDef = (SoldierGameObjDef*)DefinitionMgrClass::Get_First(CID_Soldier);SoldierDef;SoldierDef = (SoldierGameObjDef*)DefinitionMgrClass::Get_Next(SoldierDef,CID_Soldier)) {
		const WeaponDefinitionClass *WeaponDef = (WeaponDefinitionClass*)Find_Definition(SoldierDef->WeaponDefID);
		if (WeaponDef) {
			if (!CharacterWeapons.Exists((unsigned int)SoldierDef)) { //Default Soldier -> Weapon
				CharacterWeapons.Insert((unsigned int)SoldierDef,WeaponDef);
			}
		}
	}

	BasePowerUpDef = (PowerUpGameObjDef*)Find_Named_Definition("Soldier PowerUps");
}

void DALootGameFeatureClass::Object_Created_Event(GameObject *obj) {
	DALootPlayerDataClass *Player = Get_Player_Data(((SoldierGameObj*)obj)->Get_Player());
	WeaponBagClass *Bag = ((SoldierGameObj*)obj)->Get_Weapon_Bag();
	for (int i = 0;i < Player->Locker.Count();i++) { //Keep weapons when switching characters.
		Bag->Add_Weapon(Player->Locker[i],999);
	}
}

void DALootGameFeatureClass::Object_Destroyed_Event(GameObject *obj) {
	if (!The_Game()->Is_Game_Over() && The_Game()->Is_Gameplay_Permitted()) {
		SoldierGameObj *Soldier = (SoldierGameObj*)obj;
		DAPlayerClass *Player = Soldier->Get_DA_Player();
		if (!Player->Is_Spawning()) {
			DALootPlayerDataClass *PlayerData = Get_Player_Data(Player);
			DropOddsStruct Odds = CharacterDropOdds.Get((unsigned int)&Soldier->Get_Definition(),DropOdds);
			if (!Soldier->Is_Visible()) {
				Odds.Total -= Odds.DNA;
				Odds.DNA = 0;
			}
		Reselect:
			int Rand = Get_Random_Int(1,Odds.Total+1);
			int Total = 0;
			if (Odds.PowerUp && Rand <= (Total+=Odds.PowerUp)) { //Drop powerup
				DALootPowerUpClass *PowerUp = Create_PowerUp(Soldier);
				if (PowerUp) {
					if (DamagersOnlyTime >= 0.0f) {
						PowerUp->Init_Damagers(DamagersOnlyDistance,DamagersOnlyTime);
					}
				}	
				else { //This character doesn't drop any powerups, remove chance to drop powerups and reselect.
					Odds.Total -= Odds.PowerUp;
					Odds.PowerUp = 0;
					goto Reselect;
				}
			}
			else if (Odds.Weapon && Rand <= (Total+=Odds.Weapon)) { //Drop backpack
				DALootBackpackClass *Backpack = Create_Backpack(Soldier);
				const WeaponDefinitionClass *MainDrop = Get_Character_Weapon_Drop(Soldier);
				WeaponBagClass *Bag = Soldier->Get_Weapon_Bag();
				bool MainDropFound = false;
				if (EnableMultiWeaponDrop) { //Drop all picked up weapons.
					for (int i = Bag->Get_Count()-1;i >= 1;i--) {
						WeaponClass *Weapon = Bag->Peek_Weapon(i);
						const WeaponDefinitionClass *WeaponDef = Weapon->Get_Definition();
						if (Weapon->Get_Total_Rounds() || (WeaponDef->Style != STYLE_C4 && WeaponDef->Style != STYLE_BEACON)) { //Don't drop empty C4 or beacons.
							if (PlayerData->Has_Weapon(WeaponDef)) {
								Backpack->Add_Weapon(WeaponDef,Weapon->Get_Total_Rounds());
							}
							else if (WeaponDef == MainDrop) {
								Backpack->Add_Weapon(WeaponDef,Weapon->Get_Total_Rounds());
								MainDropFound = true;
							}
						}
					}
				}
				else if (MainDrop) { //Only drop main weapon.
					WeaponClass *Weapon = Bag->Find_Weapon(MainDrop);
					if (Weapon) {
						Backpack->Add_Weapon(MainDrop,Weapon->Get_Total_Rounds());
						MainDropFound = true;
					}
				}
					
				if (MainDrop) {
					if (!MainDropFound) { //Try to find a match by translation ID.
						int IconID = MainDrop->IconNameID;
						for (int i = Bag->Get_Count()-1;i >= 1;i--) {
							WeaponClass *Weapon = Bag->Peek_Weapon(i);
							if (Weapon->Get_Definition()->IconNameID == IconID && (Weapon->Get_Total_Rounds() || (MainDrop->Style != STYLE_C4 && MainDrop->Style != STYLE_BEACON))) {
								Backpack->Add_Weapon(MainDrop,Weapon->Get_Total_Rounds());
								MainDropFound = true;
								break;
							}
						}
					}
					if (!MainDropFound) { //Try to find a match by ammo ID.
						unsigned int AmmoDefID = MainDrop->PrimaryAmmoDefID;
						for (int i = Bag->Get_Count()-1;i >= 1;i--) {
							WeaponClass *Weapon = Bag->Peek_Weapon(i);
							if (Weapon->PrimaryAmmoDefinition->Get_ID() == AmmoDefID && (Weapon->Get_Total_Rounds() || (MainDrop->Style != STYLE_C4 && MainDrop->Style != STYLE_BEACON))) {
								Backpack->Add_Weapon(MainDrop,Weapon->Get_Total_Rounds());
								MainDropFound = true;
								break;
							}
						}
					}
					if (!MainDropFound) { //Just going to have to drop a full one then.
						Backpack->Add_Weapon(MainDrop,MainDrop->ClipSize+MainDrop->MaxInventoryRounds);
					}
				}
				if (!Backpack->Get_Weapon_Count()) { //No weapons to drop, remove chance to drop weapons and reselect.
					Backpack->Get_Owner()->Set_Delete_Pending();
					Odds.Total -= Odds.Weapon;
					Odds.Weapon = 0;
					goto Reselect;
				}
				if (DamagersOnlyTime >= 0.0f) {
					Backpack->Init_Damagers(DamagersOnlyDistance,DamagersOnlyTime);
				}
			}
			else if (Odds.DNA && Rand <= (Total+=Odds.DNA)) { //Drop DNA
				DALootDNAClass *DNA = Create_DNA(Soldier);
				if (DamagersOnlyTime >= 0.0f) {
					DNA->Init_Damagers(DamagersOnlyDistance,DamagersOnlyTime);
				}
			}
			PlayerData->Clear_Weapons();
		}
	}
}

void DALootGameFeatureClass::Add_Weapon_Event(cPlayer *Player,WeaponClass *Weapon) {
	if (!Player->Get_DA_Player()->Is_Spawning() && Is_Droppable(Weapon)) { //Don't add undroppable or starting weapons to the locker.
		Get_Player_Data(Player)->Add_Weapon(Weapon);
	}
}

void DALootGameFeatureClass::Remove_Weapon_Event(cPlayer *Player,WeaponClass *Weapon) {
	Get_Player_Data(Player)->Remove_Weapon(Weapon);
}

void DALootGameFeatureClass::Clear_Weapons_Event(cPlayer *Player) {
	Get_Player_Data(Player)->Clear_Weapons();
}

bool DALootGameFeatureClass::Drop_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	DALootPlayerDataClass *PlayerData = Get_Player_Data(Player);
	if (!PlayerData->Get_Weapon_Count() || !Player->Get_GameObj() || Player->Get_GameObj()->Get_Defense_Object()->Get_Health() == 0.0f) {
		DA::Private_Color_Message(Player,COLORWHITE,"You have no weapons to drop.");
	}
	else {
		WeaponBagClass *Bag = Player->Get_GameObj()->Get_Weapon_Bag();
		if (Text.Size()) {
			if (Text[0] == "all") {
				DALootBackpackClass *Backpack = Create_Backpack(Player->Get_GameObj());
				for (int i = Bag->Get_Count()-1;i >= 1;i--) {
					WeaponClass *Weapon = Bag->Peek_Weapon(i);
					const WeaponDefinitionClass *WeaponDef = Weapon->Get_Definition();
					if (PlayerData->Has_Weapon(WeaponDef) && (Weapon->Get_Total_Rounds() || (WeaponDef->Style != STYLE_C4 && WeaponDef->Style != STYLE_BEACON))) {
						Backpack->Add_Weapon(WeaponDef,Weapon->Get_Total_Rounds());
						Bag->Remove_Weapon(i);
					}
				}
				if (!Backpack->Get_Weapon_Count()) {
					Backpack->Get_Owner()->Set_Delete_Pending();
					DA::Private_Color_Message(Player,COLORWHITE,"You have no weapons to drop.");
				}
				else {
					DA::Private_Color_Message(Player,COLORWHITE,"You have dropped all your weapons.");
					Backpack->Set_Expire_Time(DropCommandExpireTime);
					Backpack->Start_Timer(6,5.0f);
				}
			}
			else {
				for (int i = Bag->Get_Count()-1;i >= 1;i--) {
					WeaponClass *Weapon = Bag->Peek_Weapon(i);
					const WeaponDefinitionClass *WeaponDef = Weapon->Get_Definition();
					const char *Translation = DATranslationManager::Translate(WeaponDef);
					if (PlayerData->Has_Weapon(WeaponDef) && stristr(Translation,Text[0]) && (Weapon->Get_Total_Rounds() || (WeaponDef->Style != STYLE_C4 && WeaponDef->Style != STYLE_BEACON))) { //Only let them drop weapons they've picked up, not started with.
						DA::Private_Color_Message(Player,COLORWHITE,"You have dropped your %s.",Translation);
						DALootBackpackClass *Backpack = Create_Backpack(Player->Get_GameObj());
						Backpack->Add_Weapon(WeaponDef,Weapon->Get_Total_Rounds());
						Backpack->Set_Expire_Time(DropCommandExpireTime);
						Backpack->Start_Timer(6,5.0f);
						Bag->Remove_Weapon(i);
						return true;
					}
				}
				DA::Private_Color_Message(Player,COLORWHITE,"You cannot drop that weapon.");
			}
		}
		else if (Bag->Get_Index()) {
			WeaponClass *Weapon = Bag->Get_Weapon();
			const WeaponDefinitionClass *WeaponDef = Weapon->Get_Definition();
			if (PlayerData->Has_Weapon(WeaponDef) && (Weapon->Get_Total_Rounds() || (WeaponDef->Style != STYLE_C4 && WeaponDef->Style != STYLE_BEACON))) { //Only let them drop weapons they've picked up, not started with.
				DA::Private_Color_Message(Player,COLORWHITE,"You have dropped your %s.",DATranslationManager::Translate(Weapon));
				DALootBackpackClass *Backpack = Create_Backpack(Player->Get_GameObj());
				Backpack->Add_Weapon(WeaponDef,Weapon->Get_Total_Rounds());
				Backpack->Set_Expire_Time(DropCommandExpireTime);
				Backpack->Start_Timer(6,5.0f);
				Bag->Select_Next();
				Bag->Remove_Weapon(WeaponDef);
				return true;
			}
			DA::Private_Color_Message(Player,COLORWHITE,"You cannot drop that weapon.");
		}
	}
	return true;
}

DA_API Register_Game_Feature(DALootGameFeatureClass,"Loot","EnableLoot",0);
