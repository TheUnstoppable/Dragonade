/*	Renegade Scripts.dll
    Dragonade Building Manager
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
#include "da_building.h"
#include "da_log.h"
#include "da_settings.h"
#include "da_translation.h"
#include "GameObjManager.h"
#include "RefineryGameObj.h"
#include "SoldierFactoryGameObj.h"

static const char DestructionWarningText[2][BuildingConstants::TYPE_COUNT][50] = {
	{ //Nod Buildings
		"IDS_M00BNPP_HLTH0001I1EVAN_TXT", //Power Plant
		"IDS_M00BNHN_HLTH0001I1EVAN_TXT", //Hand of Nod
		"IDS_M00BNAF_HLTH0001I1EVAN_TXT", //Airstrip
		"IDS_M00BNTR_HLTH0001I1EVAN_TXT", //Refinery
		"IDS_M00bncc_hlth0001i1evan_TXT", //Comm. Center
		"IDS_M00bnrf_hlth0001i1evan_TXT", //Repair Bay	
		"IDS_M00bnsn_hlth0001i1evan_TXT", //Shrine
		"IDS_M00bnhp_hlth0001i1evan_TXT", //Helipad
		"IDS_M00bncy_hlth0001i1evan_TXT", //Conyard
		"IDS_M00BNOL_HLTH0001I1EVAN_TXT", //Obelisk
		"", //Tech Center
		"", //Naval Factory
		"", //Special
	},
	{ //GDI Buildings
		"IDS_M00BGPP_HLTH0001I1EVAG_TXT", //Power Plant
		"IDS_M00BGIB_HLTH0001I1EVAG_TXT", //Barracks
		"IDS_M00BGWF_HLTH0001I1EVAG_TXT", //Weapons Factory
		"IDS_M00BGTR_HLTH0001I1EVAG_TXT", //Refinery
		"IDS_M00bgcc_hlth0001i1evag_TXT", //Comm. Center
		"IDS_M00bgrf_hlth0001i1evag_TXT", //Repair Bay
		"", //Shrine
		"IDS_M00bghp_hlth0001i1evag_TXT", //Helipad
		"IDS_M00bgcy_hlth0001i1evag_TXT", //Conyard
		"IDS_M00BGAT_HLTH0001I1EVAG_TXT", //AGT
		0, //Tech Center
		0, //Naval Factory
		0 //Special
	}
};

//Annoyingly, the repaired and destruction imminent strings aren't setup properly; they're not linked to their respective sounds. So a seperate list of sounds is needed.
static const char DestructionWarningSounds[2][BuildingConstants::TYPE_COUNT][50] = {
	{ //Nod Buildings
		"M00BNPP_HLTH0001I1EVAN_SND.wav", //Power Plant
		"M00BNHN_HLTH0001I1EVAN_SND.wav", //Hand of Nod
		"M00BNAF_HLTH0001I1EVAN_SND.wav", //Airstrip
		"M00BNTR_HLTH0001I1EVAN_SND.wav", //Refinery
		"m00bncc_hlth0001i1evan_snd.wav", //Comm. Center
		"m00bnrf_hlth0001i1evan_snd.wav", //Repair Bay	
		"m00bnsn_hlth0001i1evan_snd.wav", //Shrine
		"m00bnhp_hlth0001i1evan_snd.wav", //Helipad
		"m00bncy_hlth0001i1evan_snd.wav", //Conyard
		"M00BNOL_HLTH0001I1EVAN_SND.wav", //Obelisk
		"", //Tech Center
		"", //Naval Factory
		"", //Special
	},
	{ //GDI Buildings
		"M00BGPP_HLTH0001I1EVAG_SND.wav", //Power Plant
		"M00BGIB_HLTH0001I1EVAG_SND.wav", //Barracks
		"M00BGWF_HLTH0001I1EVAG_SND.wav", //Weapons Factory
		"M00BGTR_HLTH0001I1EVAG_SND.wav", //Refinery
		"m00bgcc_hlth0001i1evag_snd.wav", //Comm. Center
		"m00bgrf_hlth0001i1evag_snd.wav", //Repair Bay
		"", //Shrine
		"m00bghp_hlth0001i1evag_snd.wav", //Helipad
		"m00bgcy_hlth0001i1evag_snd.wav", //Conyard
		"M00BGAT_HLTH0001I1EVAG_SND.wav", //AGT
		0, //Tech Center
		0, //Naval Factory
		0 //Special
	}
};

static const char RepairText[2][BuildingConstants::TYPE_COUNT][50] = {
	{ //Nod Buildings
		"IDS_M00BNPP_DSGN0010I1EVAN_TXT", //Power Plant
		"IDS_M00BNHN_DSGN0016I1EVAN_TXT", //Hand of Nod
		"IDS_M00BNAF_DSGN0010I1EVAN_TXT", //Airstrip
		"IDS_M00BNTR_DSGN0010I1EVAN_TXT", //Refinery
		"IDS_M00bncc_dsgn0010i1evan_TXT", //Comm. Center
		"IDS_M00bnrf_dsgn0002i1evan_TXT", //Repair Bay
		"IDS_M00bnsn_dsgn0014i1evan_TXT", //Shrine
		"IDS_M00bnhp_dsgn0002i1evan_TXT", //Helipad
		"IDS_M00bncy_dsgn0009i1evan_TXT", //Conyard
		"IDS_M00BNOL_DSGN0008I1EVAN_TXT", //Obelisk
		"", //Tech Center
		"", //Naval Factory
		"" //Special
	},
	{ //GDI Buildings
		"IDS_M00BGPP_DSGN0009I1EVAG_TXT", //Power Plant
		"IDS_M00BGIB_DSGN0008I1EVAG_TXT", //Barracks
		"IDS_M00BGWF_DSGN0009I1EVAG_TXT", //Weapons Factory
		"IDS_M00BGTR_DSGN0009I1EVAG_TXT", //Refinery
		"IDS_M00bgcc_dsgn0001i1evag_TXT", //Comm. Center
		"IDS_M00bgrf_dsgn0001i1evag_TXT", //Repair Bay
		"", //Shrine
		"IDS_M00bghp_dsgn0001i1evag_TXT", //Helipad
		"IDS_M00bgcy_dsgn0009i1evag_TXT", //Conyard
		"IDS_M00BGAT_DSGN0007I1EVAG_TXT", //AGT
		"", //Tech Center
		"", //Naval Factory
		"" //Special
	}
};

static const char RepairSounds[2][BuildingConstants::TYPE_COUNT][50] = {
	{ //Nod Buildings
		"M00BNPP_DSGN0010I1EVAN_SND.wav", //Power Plant
		"M00BNHN_DSGN0016I1EVAN_SND.wav", //Hand of Nod
		"M00BNAF_DSGN0010I1EVAN_SND.wav", //Airstrip
		"M00BNTR_DSGN0010I1EVAN_SND.wav", //Refinery
		"m00bncc_dsgn0010i1evan_snd.wav", //Comm. Center
		"m00bnrf_dsgn0002i1evan_snd.wav", //Repair Bay
		"m00bnsn_dsgn0014i1evan_snd.wav", //Shrine
		"m00bnhp_dsgn0002i1evan_snd.wav", //Helipad
		"m00bncy_dsgn0009i1evan_snd.wav", //Conyard
		"M00BNOL_DSGN0008I1EVAN_SND.wav", //Obelisk
		"", //Tech Center
		"", //Naval Factory
		"" //Special
	},
	{ //GDI Buildings
		"M00BGPP_DSGN0009I1EVAG_SND.wav", //Power Plant
		"M00BGIB_DSGN0008I1EVAG_SND.wav", //Barracks
		"M00BGWF_DSGN0009I1EVAG_SND.wav", //Weapons Factory
		"M00BGTR_DSGN0009I1EVAG_SND.wav", //Refinery
		"m00bgcc_dsgn0001i1evag_snd.wav", //Comm. Center
		"m00bgrf_dsgn0001i1evag_snd.wav", //Repair Bay
		"", //Shrine
		"m00bghp_dsgn0001i1evag_snd.wav", //Helipad
		"m00bgcy_dsgn0009i1evag_snd.wav", //Conyard
		"M00BGAT_DSGN0007I1EVAG_SND.wav", //AGT
		"", //Tech Center
		"", //Naval Factory
		"" //Special
	}
};

//The strings for these aren't linked to the proper sound, so it falls to us to play them.
static const char DestructionSounds[2][2][BuildingConstants::TYPE_COUNT][50] = {
	{ //Nod Buildings
		{ //Nod Team
			"", //Power Plant
			"", //Hand of Nod
			"", //Airstrip
			"", //Refinery
			"m00bncc_kill0001i1evan_snd.wav", //Comm. Center
			"m00bnrf_kill0001i1evan_snd.wav", //Repair Bay
			"", //Shrine
			"m00bnhp_kill0001i1evan_snd.wav", //Helipad
			"m00bncy_kill0001i1evan_snd.wav", //Conyard
			"", //Obelisk
			"", //Tech Center
			"", //Naval Factory
			"" //Special
		},
		{ //GDI Team
			"", //Power Plant
			"", //Hand of Nod
			"", //Airstrip
			"", //Refinery
			"m00bncc_kill0002i1evag_snd.wav", //Comm. Center
			"m00bnrf_kill0002i1evag_snd.wav", //Repair Bay
			"", //Shrine
			"m00bnhp_kill0002i1evag_snd.wav", //Helipad
			"m00bncy_kill0002i1evag_snd.wav", //Conyard
			"", //Obelisk
			"", //Tech Center
			"", //Naval Factory
			"" //Special
		}
	},
	{ //GDI Buildings
		{ //Nod Team
			"", //Power Plant
			"", //Barracks
			"", //Weapons Factory
			"", //Refinery
			"m00bgcc_kill0002i1evan_snd.wav", //Comm. Center
			"m00bgrf_kill0002i1evan_snd.wav", //Repair Bay
			"", //Shrine
			"m00bghp_kill0002i1evan_snd.wav", //Helipad
			"m00bgcy_kill0002i1evan_snd.wav", //Conyard
			"", //AGT
			"", //Tech Center
			"", //Naval Factory
			"" //Special
		},
		{ //GDI Team
			"", //Power Plant
			"", //Barracks
			"", //Weapons Factory
			"", //Refinery
			"m00bgcc_kill0001i1evag_snd.wav", //Comm. Center
			"m00bgrf_kill0001i1evag_snd.wav", //Repair Bay
			"", //Shrine
			"m00bghp_kill0001i1evag_snd.wav", //Helipad
			"m00bgcy_kill0001i1evag_snd.wav", //Conyard
			"", //AGT
			"", //Tech Center
			"", //Naval Factory
			"" //Special
		}
	}
};

static const char UnderAttackSounds[2][2][BuildingConstants::TYPE_COUNT][50] = {
	{ //Nod Buildings
		{ //Nod Team
			"", //Power Plant
			"", //Hand of Nod
			"", //Airstrip
			"", //Refinery
			"m00bncc_tdfe0001i1evan_snd.wav", //Comm. Center
			"m00bnrf_tdfe0001i1evan_snd.wav", //Repair Bay
			"", //Shrine
			"m00bnhp_tdfe0001i1evan_snd.wav", //Helipad
			"m00bncy_tdfe0001i1evan_snd.wav", //Conyard
			"", //Obelisk
			"", //Tech Center
			"", //Naval Factory
			"" //Special
		},
		{ //GDI Team
			"", //Power Plant
			"", //Hand of Nod
			"", //Airstrip
			"", //Refinery
			"m00bncc_tdfe0002i1evag_snd.wav", //Comm. Center
			"m00bnrf_tdfe0002i1evag_snd.wav", //Repair Bay
			"", //Shrine
			"m00bnhp_tdfe0002i1evag_snd.wav", //Helipad
			"m00bncy_tdfe0002i1evag_snd.wav", //Conyard
			"", //Obelisk
			"", //Tech Center
			"", //Naval Factory
			"" //Special
		}
	},
	{ //GDI Buildings
		{ //Nod Team
			"", //Power Plant
			"", //Barracks
			"", //Weapons Factory
			"", //Refinery
			"m00bgcc_tdfe0002i1evan_snd.wav", //Comm. Center
			"m00bgrf_tdfe0002i1evan_snd.wav", //Repair Bay
			"", //Shrine
			"m00bghp_tdfe0002i1evan_snd.wav", //Helipad
			"m00bgcy_tdfe0002i1evan_snd.wav", //Conyard
			"", //AGT
			"", //Tech Center
			"", //Naval Factory
			"" //Special
		},
		{ //GDI Team
			"", //Power Plant
			"", //Barracks
			"", //Weapons Factory
			"", //Refinery
			"m00bgcc_tdfe0001i1evag_snd.wav", //Comm. Center
			"m00bgrf_tdfe0001i1evag_snd.wav", //Repair Bay
			"", //Shrine
			"m00bghp_tdfe0001i1evag_snd.wav", //Helipad
			"m00bgcy_tdfe0001i1evag_snd.wav", //Conyard
			"", //AGT
			"", //Tech Center
			"", //Naval Factory
			"" //Special
		}
	}
};

void DABuildingObserverClass::Init() {
	DestructionMessage = true;
	RepairMessage = true;
	AllowRepairMessage = false;
}

void DABuildingObserverClass::Timer_Expired(GameObject *obj,int Number) {
	if (Number == 1) {
		DestructionMessage = true;
	}
	else if (Number == 2) {
		RepairMessage = true;
	}
}

void DABuildingObserverClass::Damage_Received(ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	if (!((BuildingGameObj*)Get_Owner())->Is_Destroyed()) {
		if (Damage > 0.0f) {
			if (((BuildingGameObj*)Get_Owner())->Get_Defense_Object()->Get_Health() <= ((BuildingGameObj*)Get_Owner())->Get_Defense_Object()->Get_Health_Max()*0.9f) {
				AllowRepairMessage = true;
				if (DestructionMessage && ((BuildingGameObj*)Get_Owner())->Get_Defense_Object()->Get_Health() && ((BuildingGameObj*)Get_Owner())->Get_Defense_Object()->Get_Health() <= ((BuildingGameObj*)Get_Owner())->Get_Defense_Object()->Get_Health_Max()*0.2f) {
					DestructionMessage = false;
					Start_Timer(1,30.0f);
					int Team = ((BuildingGameObj*)Get_Owner())->Get_Player_Type();
					BuildingConstants::BuildingType Type = ((BuildingGameObj*)Get_Owner())->Get_Definition().Get_Type();
					const SimpleDynVecClass<GameObjObserverClass *> &Observers = Get_Owner()->Get_Observers();
					for (int i = 0;i < Observers.Count();i++) {
						if (!_stricmp(Observers[i]->Get_Name(),"ConYard")) {
							Type = BuildingConstants::TYPE_CONYARD;
							break;
						}
					}
					Send_Announcement_Team(Team,DestructionWarningText[Team][Type]);
					Create_2D_WAV_Sound_Team(DestructionWarningSounds[Team][Type],Team);
				}
			}
		}
		else if (Damage < 0.0f) {
			if (RepairMessage && AllowRepairMessage && ((BuildingGameObj*)Get_Owner())->Get_Defense_Object()->Get_Health() == ((BuildingGameObj*)Get_Owner())->Get_Defense_Object()->Get_Health_Max()) {
				RepairMessage = false;
				AllowRepairMessage = false;
				Start_Timer(2,30.0f);
				int Team = ((BuildingGameObj*)Get_Owner())->Get_Player_Type();
				BuildingConstants::BuildingType Type = ((BuildingGameObj*)Get_Owner())->Get_Definition().Get_Type();
				const SimpleDynVecClass<GameObjObserverClass *> &Observers = Get_Owner()->Get_Observers();
				for (int i = 0;i < Observers.Count();i++) {
					if (!_stricmp(Observers[i]->Get_Name(),"ConYard")) {
						Type = BuildingConstants::TYPE_CONYARD;
						break;
					}
				}
				Send_Announcement_Team(Team,RepairText[Team][Type]);
				Create_2D_WAV_Sound_Team(RepairSounds[Team][Type],Team);
			}
		}
	}
}

void DABuildingObserverClass::Kill_Received(ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	int Team = ((BuildingGameObj*)Get_Owner())->Get_Player_Type();
	BuildingConstants::BuildingType BuildingType = ((BuildingGameObj*)Get_Owner())->Get_Definition().Get_Type();
	const SimpleDynVecClass<GameObjObserverClass *> &Observers = Get_Owner()->Get_Observers();
	for (int i = 0;i < Observers.Count();i++) {
		if (!_stricmp(Observers[i]->Get_Name(),"ConYard")) {
			BuildingType = BuildingConstants::TYPE_CONYARD;
			break;
		}
	}
	Create_2D_WAV_Sound_Team(DestructionSounds[Team][0][BuildingType],0);
	Create_2D_WAV_Sound_Team(DestructionSounds[Team][1][BuildingType],1);
	Stop_Timer(1);
}

void DABuildingObserverClass::Custom(GameObject *obj, int Message, int Param, GameObject *Sender) {
	if (Message == CUSTOM_EVENT_BUILDING_REVIVED) {
		DestructionMessage = true;
		DALogManager::Write_Log("_BUILDING", "%s revived.", DATranslationManager::Translate_With_Team_Name(obj));
	}
}

void DABuildingManager::Init() {
	static DABuildingManager Instance;
	Instance.Register_Event(DAEvent::SETTINGSLOADED);
	Instance.Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::BUILDING);
	Instance.Register_Object_Event(DAObjectEvent::DAMAGERECEIVED,DAObjectEvent::BUILDING | DAObjectEvent::VEHICLE);
	Instance.Register_Object_Event(DAObjectEvent::KILLRECEIVED,DAObjectEvent::BUILDING);
	Instance.UnderAttackMessage[0] = true;
	Instance.UnderAttackMessage[1] = true;
	Instance.Register_Event(DAEvent::PLAYERJOIN);
}

void DABuildingManager::Settings_Loaded_Event() {
	if (DASettingsManager::Get_Bool("EnableInvincibleBuildings",false)) {
		for (SLNode<BuildingGameObj> *z = GameObjManager::BuildingGameObjList.Head();z;z = z->Next()) {
			z->Data()->Get_Defense_Object()->Set_Can_Object_Die(false);
		}
		for (SLNode<VehicleGameObj> *z = GameObjManager::VehicleGameObjList.Head();z;z = z->Next()) {
			if (z->Data()->Is_Turret()) {
				z->Data()->Get_Defense_Object()->Set_Can_Object_Die(false);
			}
		}
	}
	float NodDump = DASettingsManager::Get_Float("NodRefineryDumpAmount",-1.0f);
	float NodTick = DASettingsManager::Get_Float("NodRefineryTickAmount",-1.0f);
	float GDIDump = DASettingsManager::Get_Float("GDIRefineryDumpAmount",-1.0f);
	float GDITick = DASettingsManager::Get_Float("GDIRefineryTickAmount",-1.0f);
	for (RefineryGameObjDef *Ref = (RefineryGameObjDef*)DefinitionMgrClass::Get_First(CID_Refinery);Ref;Ref = (RefineryGameObjDef*)DefinitionMgrClass::Get_Next(Ref,CID_Refinery)) {
		if (Ref->Get_Default_Player_Type() == 0) {
			if (NodDump != -1.0f) {
				Ref->FundsGathered = NodDump;
			}
			if (NodTick != -1.0f) {
				Ref->FundsDistributedPerSec = NodTick;
			}
		}
		else if (Ref->Get_Default_Player_Type() == 1) {
			if (GDIDump != -1.0f) {
				Ref->FundsGathered = GDIDump;
			}
			if (GDITick != -1.0f) {
				Ref->FundsDistributedPerSec = GDITick;
			}
		}
	}
}

void DABuildingManager::Object_Created_Event(GameObject *obj) {
	if (((BuildingGameObj*)obj)->Get_Player_Type() == 0 || ((BuildingGameObj*)obj)->Get_Player_Type() == 1) {
		obj->Add_Observer(new DABuildingObserverClass);
	}
}

void DABuildingManager::Timer_Expired(int Number,unsigned int Data) {
	UnderAttackMessage[Data] = true;
}

void DABuildingManager::Damage_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	int Team = Victim->Get_Player_Type();
	if (Damage > 0.0f && (Team == 0 || Team == 1) && UnderAttackMessage[Team] && Victim->Get_Defense_Object()->Get_Health()) {
		int Announcement = 0;
		if (Victim->As_BuildingGameObj()) {
			Announcement = ((BuildingGameObj*)Victim)->Get_Definition().Get_Damage_Report(!Victim->Get_Player_Type());
		}
		else {
			Announcement = ((VehicleGameObj*)Victim)->Get_Definition().Get_Damage_Report(!Victim->Get_Player_Type());
		}
		if (Announcement) {
			UnderAttackMessage[Team] = false;
			Start_Timer(1,30.0f,false,Team);
			DALogManager::Write_Log("_BUILDING","%s under attack.",DATranslationManager::Translate_With_Team_Name(Victim));
			/*
			BuildingConstants::BuildingType Type = ((BuildingGameObj*)Victim)->Get_Definition().Get_Type();
			const SimpleDynVecClass<GameObjObserverClass *> &Observers = Victim->Get_Observers();
			for (int i = 0;i < Observers.Count();i++) {
				if (!_stricmp(Observers[i]->Get_Name(),"ConYard")) {
					Type = BuildingConstants::TYPE_CONYARD;
					break;
				}
			}
			Create_2D_WAV_Sound_Team(UnderAttackSounds[Team][0][Type],0);
			Create_2D_WAV_Sound_Team(UnderAttackSounds[Team][1][Type],1);
			*/
		}
	}
}

void DABuildingManager::Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	StringClass Message;
	if (Killer) {
		if (Killer->As_SoldierGameObj()) { //Killed by player
			if (((SoldierGameObj*)Killer)->Get_Player()) {
				if (Type == DADamageType::EXPLOSION) {
					Message.Format("%d %ls destroyed the %s (%s VS. %s)",Killer->Get_Player_Type(),Get_Wide_Player_Name(Killer),DATranslationManager::Translate_With_Team_Name(Victim),DATranslationManager::Translate(GetExplosionObj()),DATranslationManager::Translate(Victim));
				}
				else {
					Message.Format("%d %ls destroyed the %s (%s VS. %s)",Killer->Get_Player_Type(),Get_Wide_Player_Name(Killer),DATranslationManager::Translate_With_Team_Name(Victim),DATranslationManager::Translate_Soldier(Killer),DATranslationManager::Translate(Victim));
				}
			}
			else { //Killed by bot
				if (Is_Smart_Bot(Killer)) {
					Message.Format("%d %ls destroyed the %s (%s VS. %s)", Killer->Get_Player_Type(), ((SoldierGameObj*)Killer)->Get_Bot_Tag(), DATranslationManager::Translate_With_Team_Name(Victim), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate(Victim));
				}
				else {
					Message.Format("%d %s destroyed the %s (%s VS. %s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Killer)), DATranslationManager::Translate_With_Team_Name(Victim), DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate(Victim));
				}
			}
		}
		else if (Killer->As_VehicleGameObj()) { //Killed by vehicle. Could be defense or AI vehicle.
			if (Killer->Get_Defense_Object()->Get_Shield_Type() == 1) {
				Message.Format("%d The %s destroyed the %s (%s VS. %s)",Killer->Get_Player_Type(),DATranslationManager::Translate_With_Team_Name(Killer),DATranslationManager::Translate_With_Team_Name(Victim),DATranslationManager::Translate(Killer),DATranslationManager::Translate(Victim));
			}
			else if (((VehicleGameObj*)Killer)->Is_Turret()) {
				Message.Format("%d %s destroyed the %s (%s VS. %s)",Killer->Get_Player_Type(),A_Or_An_Prepend(DATranslationManager::Translate_With_Team_Name(Killer)),DATranslationManager::Translate_With_Team_Name(Victim),DATranslationManager::Translate(Killer),DATranslationManager::Translate(Victim));
			}
			else if (Is_Smart_Bot(Killer)) {
				Message.Format("%d %ls destroyed the %s (%s VS. %s)", Killer->Get_Player_Type(), ((VehicleGameObj*)Killer)->Get_Driver()->Get_Bot_Tag(), DATranslationManager::Translate_With_Team_Name(Victim), DATranslationManager::Translate(Killer), DATranslationManager::Translate(Victim));
			}
			else {
				Message.Format("%d %s destroyed the %s (%s VS. %s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Killer)), DATranslationManager::Translate_With_Team_Name(Victim), DATranslationManager::Translate(Killer), DATranslationManager::Translate(Victim));
			}
		}
		else {
			Message.Format("%d The %s was destroyed.",Victim->Get_Player_Type(),DATranslationManager::Translate_With_Team_Name(Victim));
		}
	}
	else {
		Message.Format("%d The %s was destroyed.",Victim->Get_Player_Type(),DATranslationManager::Translate_With_Team_Name(Victim));
	}
	DALogManager::Write_Log("_BUILDINGKILL","%s",Message);
	DALogManager::Write_Log("_BUILDING","%s destroyed.",DATranslationManager::Translate_With_Team_Name(Victim));
	if (((BuildingGameObj*)Victim)->As_PowerPlantGameObj() && !Is_Base_Powered(Get_Object_Type(Victim))) {
		DALogManager::Write_Log("_BUILDING","%ls Base Power is off-line.",Get_Wide_Team_Name(Victim->Get_Player_Type()));
	}
}
