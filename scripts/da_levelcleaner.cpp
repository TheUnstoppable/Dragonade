/*	Renegade Scripts.dll
    Dragonade Singleplayer Level Cleaner
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
#include "da_levelcleaner.h"
#include "da_settings.h"
#include "da_gameobj.h"
#include "GameObjManager.h"
#include "SpawnerClass.h"

void DALevelCleaner::Clean_Level() {
	Destroy_SP_Buildings();
	Destroy_SP_Objects();
	Destroy_SP_Spawners();
}

void DALevelCleaner::Destroy_SP_Buildings() {
	for (SLNode<BuildingGameObj> *z = GameObjManager::BuildingGameObjList.Head();z;z = z->Next()) {
		BuildingGameObj *Building = z->Data();
		char *Token = newstr(Building->Get_Definition().Get_Name());
		StringClass Prefix = strtok(Token,"_");
		delete[] Token;
		if (Prefix == "SP" || Prefix == "Tut") {
			//Delete the building before its creation can be sent to the clients.
			BaseControllerClass::Find_Base(Building->Get_Player_Type())->Get_Building_List().DeleteObj(Building);
			Building->Set_Delete_Pending();
			Building->Clear_Object_Dirty_Bits();
		} 
	}
}

void DALevelCleaner::Destroy_SP_Objects() {
	for (SLNode<BaseGameObj> *z = GameObjManager::GameObjList.Head();z;z = z->Next()) {
		GameObject *obj = z->Data()->As_ScriptableGameObj();
		if (obj) {
			if (Is_Powerup(obj) || obj->As_VehicleGameObj()) { //Destroy all the powerups and vehicles.
				Commands->Destroy_Object(obj);
			}
			else if (!Is_Simple(obj) || obj->As_PhysicalGameObj()->As_SimpleGameObj()->Get_Definition().Get_Player_Terminal_Type() == -1) {
				//Destroy anything with a disallowed script, model, or preset. Don't destroy PTs though.
				if (Check_Scripts(obj) || Check_Model(obj) || Check_Preset(obj)) {
					Commands->Destroy_Object(obj);
				}
			}
		}
	}
}

void DALevelCleaner::Destroy_SP_Spawners() {
	for (int i = 0;i < SpawnerList.Count();i++) {
		SpawnerList[i]->Enable(false); //Disable all the armor/health spawners.
	}
}

void DALevelCleaner::Load_Blockers(const INIClass *INI) {
	for (int i = 1;;i++) {
		StringClass Header;
		Header.Format("Blocker%d",i);
		Vector3 Pos;
		StringClass Preset;
		StringClass Model;
		INI->Get_String(Preset,The_Game()->MapName,StringFormat("%s_Preset",Header));
		if (!Preset.Is_Empty()) {
			Pos.X = INI->Get_Float(The_Game()->MapName,StringFormat("%s_X",Header),0.0f);
			Pos.Y = INI->Get_Float(The_Game()->MapName,StringFormat("%s_Y",Header),0.0f);
			Pos.Z = INI->Get_Float(The_Game()->MapName,StringFormat("%s_Z",Header),0.0f);
			GameObject *obj = Commands->Create_Object(Preset,Pos);
			INI->Get_String(Model,The_Game()->MapName,StringFormat("%s_Model",Header));
			if (!Model.Is_Empty()) {
				Commands->Set_Model(obj,Model);
			}
			Commands->Set_Facing(obj,INI->Get_Float(The_Game()->MapName,StringFormat("%s_Facing",Header),0.0f));
			if (INI->Get_Bool(The_Game()->MapName,StringFormat("%s_Invisible",Header),false)) {
				DAGameObjManager::Set_GameObj_Invisible(obj);
			}
		}
		else {
			break;
		}
	}
}

bool DALevelCleaner::Check_Scripts(GameObject *obj) {
	for (int i = 0;i < obj->Get_Observers().Count();i++) {
		char *Token = newstr(obj->Get_Observers()[i]->Get_Name());
		StringClass Prefix = strtok(Token,"_");
		delete[] Token;
		 //These are the prefixes of the WW scripts used on the SP maps.
		if (Prefix == "MTU" || Prefix == "MX0" || Prefix == "M00" || Prefix == "M01" || Prefix == "M02" || Prefix == "M03" || Prefix == "M04"
			|| Prefix == "M05" || Prefix == "M06" || Prefix == "M07" || Prefix == "M08" || Prefix == "M09" || Prefix == "M10" || Prefix == "M11"
			|| Prefix == "RMV" || Prefix == "DAK" || Prefix == "DLS" || Prefix == "DME" || Prefix == "MDD" || Prefix == "DAY" || Prefix == "GTH" || Prefix == "PDS" || Prefix == "MSK") {
				return true;
		}
	}
	return false;
}

bool DALevelCleaner::Check_Model(GameObject *obj) {
	const char *Model = Get_Model(obj);
	if (!_stricmp(Model,"vehcol2m") || !_stricmp(Model,"dsp_container") || !_stricmp(Model,"small_dsapo") 
		|| !_stricmp(Model,"o_crate_lg") || !_stricmp(Model,"o_crate_md") || !_stricmp(Model,"o_crate_sm")
		|| !_stricmp(Model,"o_davesarrow")) {
			return true;
	}
	return false;
}

bool DALevelCleaner::Check_Preset(GameObject *obj) {
	const char *Preset = Commands->Get_Preset_Name(obj);
	if (!_stricmp(Preset,"Large_Blocker") || !_stricmp(Preset,"Small_Blocker") || !_stricmp(Preset,"Vehicle_Blocker")) {
		return true;
	}
	return false;
}