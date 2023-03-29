/*	Renegade Scripts.dll
    Dragonade Building Annotations Game Feature
	Copyright 2022 Unstoppable, Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#include "General.h"
#include "engine_io.h"
#include "engine_obj.h"
#include "engine_player.h"
#include "da.h"
#include "da_settings.h"
#include "da_translation.h"
#include "da_buildingannotations.h"

const char DABuildingAnnotationsGameFeatureClass::Sounds[4][2][50] = {
		{
			"m00evan_dsgn0070i1evan_snd.wav",
			"m00evag_dsgn0066i1evag_snd.wav"
		},
		{
			"m00evan_dsgn0071i1evan_snd.wav",
			"m00evag_dsgn0067i1evag_snd.wav"
		},
		{
			"m00evan_dsgn0072i1evan_snd.wav",
			"m00evag_dsgn0068i1evag_snd.wav"
		},
		{
			"m00evan_dsgn0015i1evan_snd.wav",
			"m00evag_dsgn0018i1evag_snd.wav"
		}
};

void DABuildingAnnotationsGameFeatureClass::Init() {
	Enable75 = true;
	Enable50 = true;
	Enable25 = true;
	EnableRepairing = true;
	IncludeList = DynamicVectorClass<StringClass>();
	ExcludeList = DynamicVectorClass<StringClass>();
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Object_Event(DAObjectEvent::DAMAGERECEIVED, DAObjectEvent::ALL, INT_MAX);

	Announce75[0] = true; Announce75[1] = true;
	Announce50[0] = true; Announce50[1] = true;
	Announce25[0] = true; Announce25[1] = true;
	AnnounceRepairing[0] = true;
	AnnounceRepairing[1] = true;
}

void DABuildingAnnotationsGameFeatureClass::Settings_Loaded_Event() {
	Enable75 = DASettingsManager::Get_Bool("ReportPercent75", true);
	Enable50 = DASettingsManager::Get_Bool("ReportPercent50", true);
	Enable25 = DASettingsManager::Get_Bool("ReportPercent25", true);
	EnableRepairing = DASettingsManager::Get_Bool("ReportRepairing", true);
	IncludeList.Clear();
	INISection* IncludeSection = DASettingsManager::Get_Section("BuildingAnnotations_Include");
	if (IncludeSection) {
		for (INIEntry* Entry = IncludeSection->EntryList.First(); Entry && Entry->Is_Valid(); Entry = Entry->Next()) {
			if (!strcmp(Entry->Value, "1")) {
				IncludeList.Add(Entry->Entry);
			}
		}
	}
	ExcludeList.Clear();
	INISection* ExcludeSection = DASettingsManager::Get_Section("BuildingAnnotations_Exclude");
	if (ExcludeSection) {
		for (INIEntry* Entry = ExcludeSection->EntryList.First(); Entry && Entry->Is_Valid(); Entry = Entry->Next()) {
			if (!strcmp(Entry->Value, "1")) {
				ExcludeList.Add(Entry->Entry);
			}
		}
	}
}

void DABuildingAnnotationsGameFeatureClass::Damage_Event(DamageableGameObj* Victim, ArmedGameObj* Damager, float Damage, unsigned int Warhead, float Scale, DADamageType::Type Type) {
	StringClass Preset = Commands->Get_Preset_Name(Victim);
	if (ExcludeList.ID(Preset) == -1 && (Victim->As_BuildingGameObj() || IncludeList.ID(Preset) > -1)) {
		float Previous = Commands->Get_Health(Victim) + Commands->Get_Shield_Strength(Victim) + Damage;
		float Total = Commands->Get_Health(Victim) + Commands->Get_Shield_Strength(Victim);
		float Max = Commands->Get_Max_Health(Victim) + Commands->Get_Max_Shield_Strength(Victim);

		int Team = Get_Object_Type(Victim);

		if (Team == 0 || Team == 1) {
			if (Total > 0) {
				if (Damage > 0) {
					if (Previous / Max > 0.25f && Total / Max < 0.25f) {
						if (Enable25 && Announce25[Team]) {
							DA::Team_Color_Message_With_Team_Color(Team, "%s%s health at 25 percent.", DA::Get_Message_Prefix(), DATranslationManager::Translate(Victim));
							DA::Create_2D_Sound_Team(Team, Sounds[2][Team]);
							Announce25[Team] = false;
							Start_Timer(5555557, 30.0f, false, Team);
						}
					}
					else if (Previous / Max > 0.50f && Total / Max < 0.50f) {
						if (Enable50 && Announce50[Team]) {
							DA::Team_Color_Message_With_Team_Color(Team, "%s%s health at 50 percent.", DA::Get_Message_Prefix(), DATranslationManager::Translate(Victim));
							DA::Create_2D_Sound_Team(Team, Sounds[1][Team]);
							Announce50[Team] = false;
							Start_Timer(5555556, 30.0f, false, Team);
						}
					}
					else if (Previous / Max > 0.75f && Total / Max < 0.75f) {
						if (Enable75 && Announce75[Team]) {
							DA::Team_Color_Message_With_Team_Color(Team, "%s%s health at 75 percent.", DA::Get_Message_Prefix(), DATranslationManager::Translate(Victim));
							DA::Create_2D_Sound_Team(Team, Sounds[0][Team]);
							Announce75[Team] = false;
							Start_Timer(5555555, 30.0f, false, Team);
						}
					}
				}
				else if (Damage < 0) {
					if (Previous / Max < 0.25f && Total / Max > 0.25f) {
						if (Enable25 && Announce25[Team]) {
							DA::Team_Color_Message_With_Team_Color(Team, "%s%s health at 25 percent.", DA::Get_Message_Prefix(), DATranslationManager::Translate(Victim));
							DA::Create_2D_Sound_Team(Team, Sounds[2][Team]);
							Announce25[Team] = false;
							Start_Timer(5555557, 30.0f, false, Team);
						}
					}
					else if (Previous / Max < 0.50f && Total / Max > 0.50f) {
						if (Enable50 && Announce50[Team]) {
							DA::Team_Color_Message_With_Team_Color(Team, "%s%s health at 50 percent.", DA::Get_Message_Prefix(), DATranslationManager::Translate(Victim));
							DA::Create_2D_Sound_Team(Team, Sounds[1][Team]);
							Announce50[Team] = false;
							Start_Timer(5555556, 30.0f, false, Team);
						}
					}
					else if (Previous / Max < 0.75f && Total / Max > 0.75f) {
						if (Enable75 && Announce75[Team]) {
							DA::Team_Color_Message_With_Team_Color(Team, "%s%s health at 75 percent.", DA::Get_Message_Prefix(), DATranslationManager::Translate(Victim));
							DA::Create_2D_Sound_Team(Team, Sounds[0][Team]);
							Announce75[Team] = false;
							Start_Timer(5555555, 30.0f, false, Team);
						}
					}

					if (EnableRepairing && AnnounceRepairing[Team] && Get_Object_Type(Damager) == Team) {
						const char* PlayerName = Get_Player_Name(Damager);
						if (strcmp(PlayerName, "None")) {
							DA::Team_Color_Message_With_Team_Color(Team, "%s%s is repairing the %s.", DA::Get_Message_Prefix(), PlayerName, DATranslationManager::Translate(Victim));
							DA::Create_2D_Sound_Team(Team, Sounds[3][Team]);
							AnnounceRepairing[Team] = false;
							Start_Timer(5555558, 30.0f, false, Team);
						}
						delete[] PlayerName;
					}
				}
			}
		}
	}
}

void DABuildingAnnotationsGameFeatureClass::Timer_Expired(int Number, unsigned int Data) {
	if (Number == 5555555) {
		Announce75[Data] = true;
	}
	else if (Number == 5555556) {
		Announce50[Data] = true;
	}
	else if (Number == 5555557) {
		Announce25[Data] = true;
	}
	else if (Number == 5555558) {
		AnnounceRepairing[Data] = true;
	}
}

Register_Game_Feature(DABuildingAnnotationsGameFeatureClass, "Building Annotations", "EnableBuildingAnnotations", 0);