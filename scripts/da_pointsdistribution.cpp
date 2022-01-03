/*	Renegade Scripts.dll
    Dragonade Points Distribution Game Feature
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
#include "da_pointsdistribution.h"
#include "da_damagelog.h"
#include "da_vehicle.h"
#include "da_player.h"

void DAPointsDistributionGameFeatureClass::Init() {
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Object_Event(DAObjectEvent::KILLRECEIVED,DAObjectEvent::ALL);
}

void DAPointsDistributionGameFeatureClass::Settings_Loaded_Event() {
	DAPlayerManager::Set_Disable_Death_Points(true);
}

void DAPointsDistributionGameFeatureClass::Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	if (Is_Beacon(GetExplosionObj()) && Is_Player(Killer)) { //Give all points to the killer if killed by a beacon.
		((SoldierGameObj*)Killer)->Get_Player()->Increment_Score(Victim->Get_Defense_Object()->Get_Death_Points());
	}
	else {
		float Points = Victim->Get_Defense_Object()->Get_Death_Points();
		if (Points) {
			int Team = Victim->As_VehicleGameObj()?DAVehicleManager::Get_Team(Victim):Victim->Get_Player_Type();
			if (Team != -2) {
				DynamicVectorClass<DADamageTableStruct> Damagers;
				DADamageLog::Get_Damagers_By_Percent_Other_Team(Damagers,Victim,Team,0.0f,0.0f);
				for (int i = 0;i < Damagers.Count();i++) {
					Damagers[i].Player->Increment_Score(Damagers[i].Damage*Points); //Distribute points to all damagers.
				}
			}
		}
	}
}

Register_Game_Feature(DAPointsDistributionGameFeatureClass,"Points Distribution","EnablePointsDistribution",0);