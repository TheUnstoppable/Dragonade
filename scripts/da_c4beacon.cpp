/*	Renegade Scripts.dll
    Dragonade C4 and Beacon Manager
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#include "General.h"
#include "engine.h"
#include "engine_da.h"
#include "da.h"
#include "da_c4beacon.h"
#include "da_chatcommand.h"
#include "da_translation.h"
#include "da_log.h"
#include "da_settings.h"
#include "GameObjManager.h"
#include "weaponmgr.h"

void DAC4BeaconManager::Init() {
	static DAC4BeaconManager Instance;
	Instance.Register_Event(DAEvent::SETTINGSLOADED,INT_MAX);
	Instance.Register_Event(DAEvent::BEACONDEPLOY,INT_MAX);
	Instance.Register_Event(DAEvent::BEACONDETONATE,INT_MAX);
	Instance.Register_Event(DAEvent::C4DETONATE,INT_MAX);
	Instance.Register_Event(DAEvent::POWERUPPURCHASEREQUEST,INT_MIN);
	Instance.Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::BEACON);
	Instance.Register_Object_Event(DAObjectEvent::POKE,DAObjectEvent::C4 | DAObjectEvent::BEACON,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::KILLRECEIVED,DAObjectEvent::C4 | DAObjectEvent::BEACON,INT_MAX);
}

void DAC4BeaconManager::Settings_Loaded_Event() {
	Console_InputF("mlimit %d",DASettingsManager::Get_Int("C4Limit",Get_Mine_Limit()));
	BlockFakeBeacons = DASettingsManager::Get_Bool("BlockFakeBeacons",false);
}

void DAC4BeaconManager::Object_Created_Event(GameObject *obj) {
	BeaconGameObj *Beacon = (BeaconGameObj*)obj;
	if (Commands->Is_A_Star(Beacon->Get_Owner())) {
		BuildingGameObj *Building = Get_Closest_Building(Beacon->Get_Position(),!Beacon->Get_Player_Type());
		if (Building) {
			bool Ped = Beacon->Is_In_Enemy_Base();
			ExplosionDefinitionClass *Explosion = (ExplosionDefinitionClass*)Find_Definition(Beacon->Get_Definition().ExplosionObjDef);
			if (Explosion) {
				float Distance = 0.0f;
				float FakeDistance = 0.0f;
				Building->Find_Closest_Poly(Beacon->Get_Position(),&Distance);
				PhysicalGameObj *FakeBuilding = Get_Closest_Fake_Building(Beacon->Get_Position(),!Beacon->Get_Player_Type());
				if (FakeBuilding) {
					FakeDistance = Commands->Get_Distance(FakeBuilding->Get_Position(),Beacon->Get_Position());
				}
				if (BlockFakeBeacons && (!The_Cnc_Game()->BeaconPlacementEndsGame || !Ped) && Distance > Explosion->DamageRadius*Explosion->DamageRadius && FakeDistance > Explosion->DamageRadius) {
					WeaponClass *Weapon = Beacon->Get_Owner()->Get_Weapon_Bag()->Find_Weapon(Beacon->Get_WeaponDef());
					if (Weapon) { //Refund the ammo used to plant this beacon.
						Weapon->Set_Clip_Rounds(Weapon->Get_Clip_Rounds()+Weapon->PrimaryAmmoDefinition->SprayBulletCost);
					}
					Beacon->Set_Delete_Pending();
					DA::Page_Player(Beacon->Get_Owner(),"Beacons may only be deployed where they would damage an enemy building.");
				}
				else if (Ped && !The_Cnc_Game()->BeaconPlacementEndsGame && Distance > Explosion->DamageRadius*Explosion->DamageRadius) {
					DA::Page_Player(Beacon->Get_Owner(),"Sure you want to deploy that here? Pedestal beacons are disabled on this server.");
				}
			}
		}
	}
}

void DAC4BeaconManager::Beacon_Deploy_Event(BeaconGameObj *Beacon) {
	if (Commands->Is_A_Star(Beacon->Get_Owner())) {
		if (The_Cnc_Game()->BeaconPlacementEndsGame && Beacon->Is_In_Enemy_Base()) {
			DA::Team_Player_Message(Beacon->Get_Owner(),"Defend my beacon on the pedestal!");
		}
		else {
			BuildingGameObj *Building = Get_Closest_Building(Beacon->Get_Position(),!Beacon->Get_Player_Type());
			if (Building) {
				ExplosionDefinitionClass *Explosion = (ExplosionDefinitionClass*)Find_Definition(Beacon->Get_Definition().ExplosionObjDef);
				float Distance = 0.0f;
				Building->Find_Closest_Poly(Beacon->Get_Position(),&Distance);
				if (Distance <= Explosion->DamageRadius*Explosion->DamageRadius) {
					DA::Team_Player_Message(Beacon->Get_Owner(),"Defend my beacon at the %s!",DATranslationManager::Translate(Building));
				}
				else {
					PhysicalGameObj *FakeBuilding = Get_Closest_Fake_Building(Beacon->Get_Position(),!Beacon->Get_Player_Type());
					if (FakeBuilding && Commands->Get_Distance(FakeBuilding->Get_Position(),Beacon->Get_Position()) <= Explosion->DamageRadius) {
						DA::Team_Player_Message(Beacon->Get_Owner(),"Defend my beacon at the %s!",DATranslationManager::Translate(FakeBuilding));
					}
					else {
						DA::Team_Player_Message(Beacon->Get_Owner(),"The beacon is a fake.");
					}
				}
			}
		}
		DALogManager::Write_Log("_BEACON","%ls deployed %s.",Beacon->Get_Owner()->Get_Player()->Get_Name(),a_or_an_Prepend(DATranslationManager::Translate(Beacon)));
	}
	else {
		DALogManager::Write_Log("_BEACON", "%s was deployed",A_Or_An_Prepend(DATranslationManager::Translate(Beacon)));
	}
}

void DAC4BeaconManager::Beacon_Detonate_Event(BeaconGameObj *Beacon) {
	if (Commands->Is_A_Star(Beacon->Get_Owner())) {
		DALogManager::Write_Log("_BEACON","%ls %s has detonated.",Make_Possessive(Beacon->Get_Owner()->Get_Player()->Get_Name()),DATranslationManager::Translate(Beacon));
	}
	else {
		DALogManager::Write_Log("_BEACON", "%s has detonated.",A_Or_An_Prepend(DATranslationManager::Translate(Beacon)));
	}
}

void DAC4BeaconManager::C4_Detonate_Event(C4GameObj *C4) {
	if (Commands->Is_A_Star(C4->Get_Owner())) {
		DALogManager::Write_Log("_C4","%ls %s has detonated (Attached to: %s)",Make_Possessive(C4->Get_Owner()->Get_Player()->Get_Name()),DATranslationManager::Translate(C4),C4->Get_Stuck_Object()?DATranslationManager::Translate(C4->Get_Stuck_Object()):"None");
	}
	else {
		DALogManager::Write_Log("_C4", "%s has detonated (Attached to: %s)",A_Or_An_Prepend(DATranslationManager::Translate(C4)), C4->Get_Stuck_Object() ? DATranslationManager::Translate(C4->Get_Stuck_Object()) : "None");
	}
}

void DAC4BeaconManager::Poke_Event(cPlayer *Player,PhysicalGameObj *obj) {
	if (obj->As_BeaconGameObj()) {
		if (((BeaconGameObj*)obj)->Get_Owner()) {
			DA::Page_Player(Player,"The owner of this beacon is %ls.",((BeaconGameObj*)obj)->Get_Owner()->Get_Player()->Get_Name());
		}
		else {
			DA::Page_Player(Player,"This beacon has no owner.");
		}
	}
	else {
		if (((C4GameObj*)obj)->Get_Owner()) {
			DA::Page_Player(Player,"The owner of this C4 is %ls.",((C4GameObj*)obj)->Get_Owner()->Get_Player()->Get_Name());
		}
		else {
			DA::Page_Player(Player,"This C4 has no owner.");
		}
	}
}

void DAC4BeaconManager::Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	if (((PhysicalGameObj*)Victim)->As_BeaconGameObj()) {
		BeaconGameObj *Beacon = (BeaconGameObj*)Victim;
		if (Commands->Is_A_Star(Beacon->Get_Owner())) {
			if (Is_Player(Killer)) {
				DALogManager::Write_Log("_BEACON","%ls disarmed %ls %s.",((SoldierGameObj*)Killer)->Get_Player()->Get_Name(),Make_Possessive(Beacon->Get_Owner()->Get_Player()->Get_Name()),DATranslationManager::Translate(Beacon));
			}
			else {
				DALogManager::Write_Log("_BEACON","%ls %s was disarmed.",Make_Possessive(Beacon->Get_Owner()->Get_Player()->Get_Name()),DATranslationManager::Translate(Beacon));
			}
		}
		else {
			if (Is_Player(Killer)) {
				DALogManager::Write_Log("_BEACON", "%ls disarmed %s.", ((SoldierGameObj*)Killer)->Get_Player()->Get_Name(),a_or_an_Prepend(DATranslationManager::Translate(Beacon)));
			}
			else {
				DALogManager::Write_Log("_BEACON", "%s was disarmed.",A_Or_An_Prepend(DATranslationManager::Translate(Beacon)));
			}
		}
	}
	else {
		C4GameObj *C4 = (C4GameObj*)Victim;
		if (Commands->Is_A_Star(C4->Get_Owner())) {
			if (Is_Player(Killer)) {
				DALogManager::Write_Log("_C4","%ls disarmed %ls %s (Attached to: %s)",((SoldierGameObj*)Killer)->Get_Player()->Get_Name(),Make_Possessive(C4->Get_Owner()->Get_Player()->Get_Name()),DATranslationManager::Translate(C4),C4->Get_Stuck_Object()?DATranslationManager::Translate(C4->Get_Stuck_Object()):"None");
			}
			else {
				DALogManager::Write_Log("_C4","%ls %s was disarmed (Attached to: %s)",Make_Possessive(C4->Get_Owner()->Get_Player()->Get_Name()),DATranslationManager::Translate(C4),C4->Get_Stuck_Object()?DATranslationManager::Translate(C4->Get_Stuck_Object()):"None");
			}
		}
		else {
			if (Is_Player(Killer)) {
				DALogManager::Write_Log("_C4", "%ls disarmed %s (Attached to: %s)", ((SoldierGameObj*)Killer)->Get_Player()->Get_Name(), a_or_an_Prepend(DATranslationManager::Translate(C4)), C4->Get_Stuck_Object() ? DATranslationManager::Translate(C4->Get_Stuck_Object()) : "None");
			}
			else {
				DALogManager::Write_Log("_C4", "%s was disarmed (Attached to: %s)", A_Or_An_Prepend(DATranslationManager::Translate(C4)), C4->Get_Stuck_Object() ? DATranslationManager::Translate(C4->Get_Stuck_Object()) : "None");
			}
		}
	}
}

//Default powerup purchase handler.
int DAC4BeaconManager::PowerUp_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const PowerUpGameObjDef *Item) {
	if (Player->Get_Money() >= Cost) {
		if (Item->Grant(Player->Get_GameObj())) {
			Player->Purchase_Item((int)Cost);
			return 0;
		}
		else {
			return 4;
		}
	}
	return 2;
}



class DAC4ChatCommandClass: public DAChatCommandClass {
	bool Activate(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
		int Remote = 0,Prox = 0;
		int Team = Player->Get_Player_Type();
		for (SLNode<C4GameObj> *x = GameObjManager::C4GameObjList.Head();x;x = x->Next()) {
			if (x->Data()->Get_Player_Type() == Team) {
				if (x->Data()->Get_Ammo_Def()->AmmoType == 3) {
					++Prox;
				}
				else if (x->Data()->Get_Ammo_Def()->AmmoType == 1) {
					++Remote;
				}
			}
		}
		DA::Page_Player(Player,"Remote: %d - Proximity: %d - Total: %d - Limit: %d",Remote,Prox,Remote+Prox,Get_Mine_Limit());
		return false;
	}
};
Register_Simple_Chat_Command(DAC4ChatCommandClass,"!c4|!mine|!mines|!mlimit|!minelimit|C4Count");