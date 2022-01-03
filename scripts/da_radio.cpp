/*	Renegade Scripts.dll
    Dragonade Extra Radio Commands Game Feature
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
#include "scripts.h"
#include "da.h"
#include "engine.h"
#include "engine_da.h"
#include "da_radio.h"
#include "da_translation.h"
#include "GameObjManager.h"
#include "BuildingGameObj.h"
#include "VehicleGameObj.h"
#include "SCAnnouncement.h"

void DAExtraRadioCommandsGameFeatureClass::Init() {
	DAPlayerDataManagerClass<DAExtraRadioCommandsPlayerDataClass>::Init();
	Register_Object_Event(DAObjectEvent::DAMAGEDEALT,DAObjectEvent::PLAYER);
	Register_Key_Hook((DAEKH)&DAExtraRadioCommandsGameFeatureClass::Radio1_Key_Hook,"Radio1");
	Register_Key_Hook((DAEKH)&DAExtraRadioCommandsGameFeatureClass::Radio2_Key_Hook,"Radio2");
	Register_Key_Hook((DAEKH)&DAExtraRadioCommandsGameFeatureClass::Radio3_Key_Hook,"Radio3");
	Register_Key_Hook((DAEKH)&DAExtraRadioCommandsGameFeatureClass::Radio4_Key_Hook,"Radio4");
	Register_Key_Hook((DAEKH)&DAExtraRadioCommandsGameFeatureClass::Radio5_Key_Hook,"Radio5");
	Register_Key_Hook((DAEKH)&DAExtraRadioCommandsGameFeatureClass::Radio6_Key_Hook,"Radio6");
	Register_Key_Hook((DAEKH)&DAExtraRadioCommandsGameFeatureClass::Radio7_Key_Hook,"Radio7");
	Register_Key_Hook((DAEKH)&DAExtraRadioCommandsGameFeatureClass::Radio8_Key_Hook,"Radio8");
	Register_Key_Hook((DAEKH)&DAExtraRadioCommandsGameFeatureClass::Radio9_Key_Hook,"Radio9");
	Register_Key_Hook((DAEKH)&DAExtraRadioCommandsGameFeatureClass::Radio10_Key_Hook,"Radio10");
	Register_Key_Hook((DAEKH)&DAExtraRadioCommandsGameFeatureClass::Radio11_Key_Hook,"Radio11");
	Register_Key_Hook((DAEKH)&DAExtraRadioCommandsGameFeatureClass::Radio12_Key_Hook,"Radio12");
	Register_Key_Hook((DAEKH)&DAExtraRadioCommandsGameFeatureClass::Radio13_Key_Hook,"Radio13");
	Register_Key_Hook((DAEKH)&DAExtraRadioCommandsGameFeatureClass::Radio14_Key_Hook,"Radio14");
	Register_Key_Hook((DAEKH)&DAExtraRadioCommandsGameFeatureClass::Radio15_Key_Hook,"Radio15");
	Register_Key_Hook((DAEKH)&DAExtraRadioCommandsGameFeatureClass::Radio16_Key_Hook,"Radio16");
	Register_Key_Hook((DAEKH)&DAExtraRadioCommandsGameFeatureClass::Radio17_Key_Hook,"Radio17");
	Register_Key_Hook((DAEKH)&DAExtraRadioCommandsGameFeatureClass::Radio18_Key_Hook,"Radio18");
	Register_Key_Hook((DAEKH)&DAExtraRadioCommandsGameFeatureClass::Radio19_Key_Hook,"Radio19");
}

void DAExtraRadioCommandsGameFeatureClass::Damage_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type){
	if (Victim->Is_Enemy(Damager) && Damage > 0.0f) {
		Get_Player_Data(Damager)->Set_Last_Damage(Victim);
	}
}

void DAExtraRadioCommandsGameFeatureClass::Radio1_Key_Hook(cPlayer *Player) {
	if (!Player->Is_Flooding() && !Player->Get_DA_Player()->Is_Muted()) {
		int Team = Player->Get_Player_Type();
		float LowestPercentage = 1.0f;
		BuildingGameObj *LowestBuilding = 0;
		for (SLNode<BuildingGameObj> *z = GameObjManager::BuildingGameObjList.Head();z;z = z->Next()) {
			BuildingGameObj *Building = z->Data();
			if (Building->Get_Player_Type() == Team && !Building->Is_Destroyed()) {
				float Percentage = Building->Get_Defense_Object()->Get_Health() / Building->Get_Defense_Object()->Get_Health_Max();
				if (Percentage < LowestPercentage) {
					LowestPercentage = Percentage;
					LowestBuilding = Building;
				}
			}
		}
		if (!LowestBuilding) {
			DA::Private_Color_Message(Player,COLORWHITE,"All your buildings are at full health.");
		}
		else {
			DA::Team_Player_Message(Player,"Repair the %s!",DATranslationManager::Translate(LowestBuilding));
			Set_Emote_Icon(Player->Get_Id(),"o_em_cross.w3d",Team);
			Create_2D_WAV_Sound_Team("m00rado_dsgn0050i1gbmg_snd.wav",Team);
		}
	}
}

void DAExtraRadioCommandsGameFeatureClass::Radio2_Key_Hook(cPlayer *Player) {
	if (!Player->Is_Flooding() && !Player->Get_DA_Player()->Is_Muted()) {
		BuildingGameObj *ClosestBuilding = Get_Closest_Building(Player->Get_GameObj()->Get_Position(),Player->Get_Player_Type());
		if (ClosestBuilding) {
			if (ClosestBuilding->Find_Closest_Poly(Player->Get_GameObj()->Get_Position()) > 400.0f) {
				DA::Team_Player_Message(Player,"Enemy beacon detected in our base!");
				Set_Emote_Icon(Player->Get_Id(),"o_em_redarr.w3d",Player->Get_Player_Type());
			}
			else if (ClosestBuilding) {
				DA::Team_Player_Message(Player,"Enemy beacon detected at the %s!",DATranslationManager::Translate(ClosestBuilding));
				Set_Emote_Icon(Player->Get_Id(),"o_em_building.w3d",Player->Get_Player_Type());
			}
		}
		else {
			DA::Team_Player_Message(Player,"Enemy beacon detected in our base!");
			Set_Emote_Icon(Player->Get_Id(),"o_em_redarr.w3d",Player->Get_Player_Type());
		}
	}
}

void DAExtraRadioCommandsGameFeatureClass::Radio3_Key_Hook(cPlayer *Player) {
	if (!Player->Is_Flooding() && !Player->Get_DA_Player()->Is_Muted()) {
		DA::Team_Player_Message(Player,"Defend the pedestal!");
		Set_Emote_Icon(Player->Get_Id(),"o_em_redarr.w3d",Player->Get_Player_Type());
	}
}

void DAExtraRadioCommandsGameFeatureClass::Radio4_Key_Hook(cPlayer *Player) {
	if (!Player->Is_Flooding() && !Player->Get_DA_Player()->Is_Muted()) {
		DA::Team_Player_Message(Player,"Cease fire.");
		Set_Emote_Icon(Player->Get_Id(),"o_em_grnarr.w3d",Player->Get_Player_Type());
	}
}

void DAExtraRadioCommandsGameFeatureClass::Radio5_Key_Hook(cPlayer *Player) {
	if (!Player->Is_Flooding() && !Player->Get_DA_Player()->Is_Muted()) {
		GameObject *LastDamaged = Get_Player_Data(Player)->Get_Last_Damage();
		if (LastDamaged && Get_Object_Type(LastDamaged) != Player->Get_Player_Type()) {
			int ID = Player->Get_Id();
			if (Is_Player(LastDamaged)) {
				DA::Team_Player_Message(ID,"Focus fire on %ls (%s)!",Get_Wide_Player_Name(LastDamaged),DATranslationManager::Translate(Get_Vehicle_Return(LastDamaged)));
				Set_Emote_Icon(Player->Get_Id(),"o_em_redarr.w3d",Player->Get_Player_Type());
				if (!Is_Stealth_Enabled(LastDamaged)) {
					Set_Emote_Icon(Get_Player_ID(LastDamaged),"o_em_chevron.w3d",Player->Get_Player_Type());
				}
			}
			else if (VehicleGameObj *Vehicle = LastDamaged->As_VehicleGameObj()) {
				if (Is_Harvester_Preset(LastDamaged)) {
					DA::Team_Player_Message(ID,"Attack the %s!",DATranslationManager::Translate(LastDamaged));
					Set_Emote_Icon(Player->Get_Id(),"o_em_apc.w3d",Player->Get_Player_Type());
					Create_2D_WAV_Sound_Team("m00rado_dsgn0071i1gbmg_snd.wav",Player->Get_Player_Type());
				}
				else if (Vehicle->Is_Turret()) {
					DA::Team_Player_Message(ID,"Attack that %s!",DATranslationManager::Translate(LastDamaged));
					Set_Emote_Icon(Player->Get_Id(),"o_em_building.w3d",Player->Get_Player_Type());
					Create_2D_WAV_Sound_Team("m00rado_dsgn0070i1gbmg_snd.wav",Player->Get_Player_Type());
				}
				else if (!Vehicle->Get_Occupant_Count()) {
					DA::Team_Player_Message(ID,"Focus fire on that %s!",DATranslationManager::Translate(LastDamaged));
					Set_Emote_Icon(Player->Get_Id(),"o_em_apc.w3d",Player->Get_Player_Type());
					Create_2D_WAV_Sound_Team("m00rado_dsgn0053i1gbmg_snd.wav",Player->Get_Player_Type());
				}
				else {
					int x = Vehicle->Get_Definition().Get_Seat_Count();
					for (int i = 0;i < x;i++) {
						if (Vehicle->Get_Occupant(i)) {
							DA::Team_Player_Message(ID,"Focus fire on %ls (%s)!",Get_Wide_Player_Name(Vehicle->Get_Occupant(i)),DATranslationManager::Translate(LastDamaged));
							Set_Emote_Icon(Player->Get_Id(),"o_em_apc.w3d",Player->Get_Player_Type());
							if (!LastDamaged->As_VehicleGameObj()->Is_Stealth_Enabled()) {
								Set_Emote_Icon(Get_Player_ID(Vehicle->Get_Occupant(i)),"o_em_apc.w3d",Player->Get_Player_Type());
							}
							Create_2D_WAV_Sound_Team("m00rado_dsgn0053i1gbmg_snd.wav",Player->Get_Player_Type());
							break;
						}
					}
				}
			}
			else if (BuildingGameObj *Building = LastDamaged->As_BuildingGameObj()) {
				DA::Team_Player_Message(ID,"Attack the %s!",DATranslationManager::Translate(LastDamaged));
				Set_Emote_Icon(Player->Get_Id(),"o_em_building.w3d",Player->Get_Player_Type());
				if (Building->As_PowerPlantGameObj()) {
					Create_2D_WAV_Sound_Team("m00rado_dsgn0074i1gbmg_snd.wav",Player->Get_Player_Type());
				}
				else if (Building->As_RefineryGameObj()) {
					Create_2D_WAV_Sound_Team("m00rado_dsgn0073i1gbmg_snd.wav",Player->Get_Player_Type());
				}
				else if (Building->Get_Definition().Get_Type() == BuildingConstants::TYPE_BASE_DEFENSE) {
					Create_2D_WAV_Sound_Team("m00rado_dsgn0070i1gbmg_snd.wav",Player->Get_Player_Type());
				}
				else {
					Create_2D_WAV_Sound_Team("m00rado_dsgn0072i1gbmg_snd.wav",Player->Get_Player_Type());
				}
			}
			else if (Is_C4(LastDamaged)) {
				DA::Team_Player_Message(ID,"Disarm that C4!");
				Set_Emote_Icon(Player->Get_Id(),"o_em_cross.w3d",Player->Get_Player_Type());
			}
			else if (Is_Beacon(LastDamaged)) {
				DA::Team_Player_Message(ID,"Disarm that beacon!");
				Set_Emote_Icon(Player->Get_Id(),"o_em_cross.w3d",Player->Get_Player_Type());
			}
		}
	}
}

void DAExtraRadioCommandsGameFeatureClass::Radio6_Key_Hook(cPlayer *Player) {
	if (!Player->Is_Flooding() && !Player->Get_DA_Player()->Is_Muted()) {
		GameObject *LastDamaged = Get_Player_Data(Player)->Get_Last_Damage();
		if (LastDamaged && Get_Object_Type(LastDamaged) != Player->Get_Player_Type()) {
			if (!LastDamaged || (!LastDamaged->As_SoldierGameObj() && !LastDamaged->As_VehicleGameObj())) {
				SCAnnouncement* RadioEvent = Send_Client_Announcement(0, Player->Get_Id(), 8544, ANNOUNCE_TEAM, 0, false, false);
				for (SLNode<cPlayer>* z = Get_Player_List()->Head(); z; z = z->Next()) {
					cPlayer* p = z->Data();
					if (Player->Is_Active() && p->Get_Player_Type() == Player->Get_Player_Type()) {
						RadioEvent->Set_Object_Dirty_Bits(Player->Get_Id(), NetworkObjectClass::BIT_CREATION);
					}
				}
				
				Set_Emote_Icon(Player->Get_Id(),"o_em_redarr.w3d",Player->Get_Player_Type());
			}
			else {
				Create_2D_WAV_Sound_Team("m00rado_dsgn0059i1gbmg_snd.wav", Player->Get_Player_Type());
				DA::Team_Player_Message(Player,"Enemy %s spotted!",DATranslationManager::Translate(LastDamaged));
				if (LastDamaged->As_VehicleGameObj()) {
					Set_Emote_Icon(Player->Get_Id(),"o_em_apc.w3d",Player->Get_Player_Type());
					if (!LastDamaged->As_VehicleGameObj()->Is_Stealth_Enabled()) {
						Set_Emote_Icon(Get_Player_ID(Get_Vehicle_Occupant(LastDamaged,0)),"o_em_chevron.w3d",Player->Get_Player_Type());
					}
				}
				else {
					Set_Emote_Icon(Player->Get_Id(),"o_em_redarr.w3d",Player->Get_Player_Type());
					if (!Is_Stealth_Enabled(LastDamaged)) {
						Set_Emote_Icon(Get_Player_ID(LastDamaged),"o_em_chevron.w3d",Player->Get_Player_Type());
					}
				}
			}
		}
	}
}

void DAExtraRadioCommandsGameFeatureClass::Radio7_Key_Hook(cPlayer *Player) {
	if (!Player->Is_Flooding() && !Player->Get_DA_Player()->Is_Muted()) {
		float ClosestDistance = FLT_MAX;
		int ClosestType = 0;
		Vector3 Pos = Commands->Get_Position(Player->Get_GameObj());
		for (SLNode<BaseGameObj> *z = GameObjManager::GameObjList.Head();z;z = z->Next()) {
			GameObject *obj = z->Data()->As_ScriptableGameObj();
			if (Is_Powerup(obj)) {
				float Distance = Commands->Get_Distance(Pos,Commands->Get_Position(obj));
				if (Distance < ClosestDistance) {
					if (_stricmp(Get_Model(obj),"vehcol2m") == 0) {
						ClosestType = 1;
						ClosestDistance = Distance;
					}
					else if (_stricmp(Get_Model(obj),"p_backpack") == 0) {
						ClosestType = 2;
						ClosestDistance = Distance;
					}
					else if (_stricmp(Get_Model(obj),"p_ammregen") == 0) {
						ClosestType = 3;
						ClosestDistance = Distance;
					}
				}
			}
		}
		if (ClosestType == 1) {
			DA::Team_Player_Message(Player,"Get that crate.");
			Set_Emote_Icon(Player->Get_Id(),"o_em_chevron.w3d",Player->Get_Player_Type());
		}
		else if (ClosestType == 2) {
			DA::Team_Player_Message(Player,"Get that backpack.");
			Set_Emote_Icon(Player->Get_Id(),"o_em_chevron.w3d",Player->Get_Player_Type());
		}
		else if (ClosestType == 3) {
			DA::Team_Player_Message(Player,"Get that weapon.");
			Set_Emote_Icon(Player->Get_Id(),"o_em_chevron.w3d",Player->Get_Player_Type());
		}
	}
}

void DAExtraRadioCommandsGameFeatureClass::Radio8_Key_Hook(cPlayer *Player) {
	if (!Player->Is_Flooding() && !Player->Get_DA_Player()->Is_Muted()) {
		GameObject* Vehicle = Player->Get_GameObj() ? Player->Get_GameObj()->Get_Vehicle() : 0;
		if (!Vehicle) {
			DA::Private_Color_Message(Player->Get_Id(), COLORWHITE, "You are not in a vehicle.");
		}
		else {
			StringClass Translation = DATranslationManager::Translate(Vehicle);
			DA::Team_Player_Message(Player, "Requesting more %ss for %s rush.", Translation, a_or_an_Prepend(Translation));
			Set_Emote_Icon(Player->Get_Id(), "o_em_apc.w3d", Player->Get_Player_Type());
		}
	}
}

void DAExtraRadioCommandsGameFeatureClass::Radio9_Key_Hook(cPlayer *Player) {
	if (!Player->Is_Flooding() && !Player->Get_DA_Player()->Is_Muted()) {
		DA::Team_Player_Message(Player,"Sorry.");
		Set_Emote_Icon(Player->Get_Id(),"o_em_redarr.w3d",Player->Get_Player_Type());
	}
}

void DAExtraRadioCommandsGameFeatureClass::Radio10_Key_Hook(cPlayer *Player) {
	if (!Player->Is_Flooding() && !Player->Get_DA_Player()->Is_Muted()) {
		DA::Team_Player_Message(Player,"Wait up!");
		Set_Emote_Icon(Player->Get_Id(),"o_em_chevron.w3d",Player->Get_Player_Type());
	}
}

void DAExtraRadioCommandsGameFeatureClass::Radio11_Key_Hook(cPlayer *Player) {
	if (!Player->Is_Flooding() && !Player->Get_DA_Player()->Is_Muted()) {
		GameObject *LastDamaged = Get_Player_Data(Player)->Get_Last_Damage();
		Create_2D_WAV_Sound_Team("m00rado_dsgn0059i1gbmg_snd.wav",Player->Get_Player_Type());
		if (!LastDamaged || (!LastDamaged->As_SoldierGameObj() || !LastDamaged->As_VehicleGameObj()) || LastDamaged->As_VehicleGameObj()->Is_Turret()) {
			DA::Team_Player_Message(Player,"Incoming enemy unit!");
			Set_Emote_Icon(Player->Get_Id(),"o_em_redarr.w3d",Player->Get_Player_Type());
		}
		else {
			DA::Team_Player_Message(Player,"Incoming enemy %s!",DATranslationManager::Translate(LastDamaged));
			if (LastDamaged->As_VehicleGameObj()) {
				Set_Emote_Icon(Player->Get_Id(),"o_em_apc.w3d",Player->Get_Player_Type());
				if (!Is_Stealth_Enabled(LastDamaged)) {
					Set_Emote_Icon(Get_Player_ID(Get_Vehicle_Occupant(LastDamaged,0)),"o_em_chevron.w3d",Player->Get_Player_Type());
				}
			}
			else {
				Set_Emote_Icon(Player->Get_Id(),"o_em_redarr.w3d",Player->Get_Player_Type());
				if (!Is_Stealth_Enabled(LastDamaged)) {
					Set_Emote_Icon(Get_Player_ID(LastDamaged),"o_em_chevron.w3d",Player->Get_Player_Type());
				}
			}
		}
	}
}

void DAExtraRadioCommandsGameFeatureClass::Radio12_Key_Hook(cPlayer *Player) {
	if (!Player->Is_Flooding() && !Player->Get_DA_Player()->Is_Muted()) {
		GameObject* Vehicle = Player->Get_GameObj() ? Player->Get_GameObj()->Get_Vehicle() : 0;
		if (!Vehicle) {
			DA::Private_Color_Message(Player->Get_Id(), COLORWHITE, "You are not in a vehicle.");
		}
		else {
			StringClass Translation = DATranslationManager::Translate(Vehicle);
			DA::Team_Player_Message(Player, "Requesting more %ss for %s rush.", Translation, a_or_an_Prepend(Translation));
			Set_Emote_Icon(Player->Get_Id(), "o_em_apc.w3d", Player->Get_Player_Type());
		}
	}
}

void DAExtraRadioCommandsGameFeatureClass::Radio13_Key_Hook(cPlayer *Player) {
	if (!Player->Is_Flooding() && !Player->Get_DA_Player()->Is_Muted()) {
		DA::Team_Player_Message(Player,"I'm going for a refill.");
		Set_Emote_Icon(Player->Get_Id(),"o_em_chevron.w3d",Player->Get_Player_Type());
	}
}

void DAExtraRadioCommandsGameFeatureClass::Radio14_Key_Hook(cPlayer *Player) {
	if (!Player->Is_Flooding() && !Player->Get_DA_Player()->Is_Muted()) {
		DA::Team_Player_Message(Player,"Watch out! There's C4 on you!");
		Set_Emote_Icon(Player->Get_Id(),"o_em_redarr.w3d",Player->Get_Player_Type());
	}
}

void DAExtraRadioCommandsGameFeatureClass::Radio15_Key_Hook(cPlayer *Player) {
	if (!Player->Is_Flooding() && !Player->Get_DA_Player()->Is_Muted()) {
		DA::Team_Player_Message(Player,"Behind us!");
		Set_Emote_Icon(Player->Get_Id(),"o_em_redarr.w3d",Player->Get_Player_Type());
	}
}

void DAExtraRadioCommandsGameFeatureClass::Radio16_Key_Hook(cPlayer *Player) {
	if (!Player->Is_Flooding() && !Player->Get_DA_Player()->Is_Muted()) {
		DA::Team_Player_Message(Player,"I'm taking fire!");
		Set_Emote_Icon(Player->Get_Id(),"o_em_redarr.w3d",Player->Get_Player_Type());
	}
}

void DAExtraRadioCommandsGameFeatureClass::Radio17_Key_Hook(cPlayer *Player) {
	if (!Player->Is_Flooding() && !Player->Get_DA_Player()->Is_Muted()) {
		DA::Team_Player_Message(Player,"Stay in the vehicle.");
		Set_Emote_Icon(Player->Get_Id(),"o_em_apc.w3d",Player->Get_Player_Type());
	}
}

void DAExtraRadioCommandsGameFeatureClass::Radio18_Key_Hook(cPlayer *Player) {
	if (!Player->Is_Flooding() && !Player->Get_DA_Player()->Is_Muted()) {
		Vector3 Pos = Commands->Get_Position(Player->Get_GameObj());
		BuildingGameObj *Building = Get_Closest_Building(Pos,Player->Get_Player_Type());
		if (!Building || Building->Find_Closest_Poly(Pos) > 400.0f) {
			DA::Team_Player_Message(Player,"I'm mining the base.");
			Set_Emote_Icon(Player->Get_Id(),"o_em_grnarr.w3d",Player->Get_Player_Type());
		}
		else {
			DA::Team_Player_Message(Player,"I'm mining the %s.",DATranslationManager::Translate(Building));
			Set_Emote_Icon(Player->Get_Id(),"o_em_building.w3d",Player->Get_Player_Type());
		}
	}
}

void DAExtraRadioCommandsGameFeatureClass::Radio19_Key_Hook(cPlayer *Player) {
	if (!Player->Is_Flooding() && !Player->Get_DA_Player()->Is_Muted()) {
		Vector3 Pos = Commands->Get_Position(Player->Get_GameObj());
		BuildingGameObj *Building = Get_Closest_Building(Pos,Player->Get_Player_Type());
		if (!Building || Building->Find_Closest_Poly(Pos) > 400.0f) {
			DA::Team_Player_Message(Player,"The base needs mining.");
			Set_Emote_Icon(Player->Get_Id(),"o_em_redarr.w3d",Player->Get_Player_Type());
		}
		else {
			DA::Team_Player_Message(Player,"The %s needs mining.",DATranslationManager::Translate(Building));
			Set_Emote_Icon(Player->Get_Id(),"o_em_building.w3d",Player->Get_Player_Type());
		}
	}
}

Register_Game_Feature(DAExtraRadioCommandsGameFeatureClass,"Extra Radio Commands","EnableExtraRadioCommands",0);