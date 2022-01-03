/*	Renegade Scripts.dll
    Dragonade Taunts Game Feature
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
#include "engine.h"
#include "da.h"
#include "da_taunt.h"
#include "SoldierGameObj.h"

inline bool Allow_Taunt(SoldierGameObj *Soldier) {
	if (!Soldier) {
		return false;
	}

	return !(Soldier->Is_Airborne() || 
		Soldier->Get_Vehicle() || 
		Soldier->Get_Defense_Object()->Get_Health() == 0.0f || 
		!Soldier->Peek_Physical_Object()->As_HumanPhysClass() || 
		!Get_Data_File(StringFormat("%s.w3d", Get_Model(Soldier)))->Is_Available());
}

void DATauntsGameFeatureClass::Init() {
	Register_Chat_Command((DAECC)&DATauntsGameFeatureClass::Taunt_Chat_Command,"!taunt|!taunts");
	Register_Key_Hook((DAEKH)&DATauntsGameFeatureClass::Taunt1_Key_Hook,"Taunt1");
	Register_Key_Hook((DAEKH)&DATauntsGameFeatureClass::Taunt2_Key_Hook,"Taunt2");
	Register_Key_Hook((DAEKH)&DATauntsGameFeatureClass::Taunt3_Key_Hook,"Taunt3");
	Register_Key_Hook((DAEKH)&DATauntsGameFeatureClass::Taunt4_Key_Hook,"Taunt4");
	Register_Key_Hook((DAEKH)&DATauntsGameFeatureClass::Taunt5_Key_Hook,"Taunt5");
	Register_Key_Hook((DAEKH)&DATauntsGameFeatureClass::Taunt6_Key_Hook,"Taunt6");
	Register_Key_Hook((DAEKH)&DATauntsGameFeatureClass::Taunt7_Key_Hook,"Taunt7");
	Register_Key_Hook((DAEKH)&DATauntsGameFeatureClass::Taunt8_Key_Hook,"Taunt8");
	Register_Key_Hook((DAEKH)&DATauntsGameFeatureClass::Taunt9_Key_Hook,"Taunt9");
}

bool DATauntsGameFeatureClass::Taunt_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	if (Text[1].GetHash() == 2016144648) {
		TauntS_Key_Hook(Player);
	}
	else {
		int Num = 0;
		if (!Text.As_Int(1,Num)) {
			Num = Get_Random_Int(0,10);
		}
		if (Num == 0) {
			Taunt0_Key_Hook(Player);
		}
		else if (Num == 1) {
			Taunt1_Key_Hook(Player);
		}
		else if (Num == 2) {
			Taunt2_Key_Hook(Player);
		}
		else if (Num == 3) {
			Taunt3_Key_Hook(Player);
		}
		else if (Num == 4) {
			Taunt4_Key_Hook(Player);
		}
		else if (Num == 5) {
			Taunt5_Key_Hook(Player);
		}
		else if (Num == 6) {
			Taunt6_Key_Hook(Player);
		}
		else if (Num == 7) {
			Taunt7_Key_Hook(Player);
		}
		else if (Num == 8) {
			Taunt8_Key_Hook(Player);
		}
		else if (Num == 9) {
			Taunt9_Key_Hook(Player);
		}
	}
	return false;
}


void DATauntsGameFeatureClass::Taunt1_Key_Hook(cPlayer *Player) {
	SoldierGameObj *obj = Player->Get_GameObj();
	if (Allow_Taunt(obj)) {
		if (Get_Sex(obj) == 'A') {
			Commands->Set_Animation(obj,"h_a_a0a0_l12",false,0,0.0f,-1.0f,false);
		}
		else {
			Commands->Set_Animation(obj,"h_b_a0a0_05",false,0,0.0f,-1.0f,false);
		}
	}
}

void DATauntsGameFeatureClass::Taunt2_Key_Hook(cPlayer *Player) {
	SoldierGameObj *obj = Player->Get_GameObj();
	if (Allow_Taunt(obj)) {
		if (Get_Sex(obj) == 'A') {
			Commands->Set_Animation(obj,"H_A_a0a0_L22",false,0,0.0f,-1.0f,false);
		}
		else {
			Commands->Set_Animation(obj,"h_b_a0a0_06",false,0,0.0f,-1.0f,false);
		}
	}
}

void DATauntsGameFeatureClass::Taunt3_Key_Hook(cPlayer *Player) {
	SoldierGameObj *obj = Player->Get_GameObj();
	if (Allow_Taunt(obj)) {
		if (Get_Sex(obj) == 'A') {
			Commands->Set_Animation(obj,"H_A_a0a0_L23",false,0,0.0f,-1.0f,false);
		}
		else {
			Commands->Set_Animation(obj,"h_b_a0a0_08",false,0,0.0f,-1.0f,false);
		}
	}
}

void DATauntsGameFeatureClass::Taunt4_Key_Hook(cPlayer *Player) {
	SoldierGameObj *obj = Player->Get_GameObj();
	if (Allow_Taunt(obj)) {
		if (Get_Sex(obj) == 'A') {
			Commands->Set_Animation(obj,"H_A_a0a0_L24",false,0,0.0f,-1.0f,false);
		}
		else {
			Commands->Set_Animation(obj,"h_b_a0a0_12",false,0,0.0f,-1.0f,false);
		}
	}
}

void DATauntsGameFeatureClass::Taunt5_Key_Hook(cPlayer *Player) {
	SoldierGameObj *obj = Player->Get_GameObj();
	if (Allow_Taunt(obj)) {
		if (Get_Sex(obj) == 'A') {
			Commands->Set_Animation(obj,"H_A_a0a0_L25",false,0,0.0f,-1.0f,false);
		}
		else {
			Commands->Set_Animation(obj,"h_b_a0a0_13",false,0,0.0f,-1.0f,false);
		}
	}
}

void DATauntsGameFeatureClass::Taunt6_Key_Hook(cPlayer *Player) {
	SoldierGameObj *obj = Player->Get_GameObj();
	if (Allow_Taunt(obj)) {
		Commands->Set_Animation(obj,"H_A_a0a0_L58",false,0,0.0f,-1.0f,false);
	}
}

void DATauntsGameFeatureClass::Taunt7_Key_Hook(cPlayer *Player) {
	SoldierGameObj *obj = Player->Get_GameObj();
	if (Allow_Taunt(obj)) {
		Commands->Set_Animation(obj,"H_A_cresentkick",false,0,0.0f,-1.0f,false);
	}
}

void DATauntsGameFeatureClass::Taunt8_Key_Hook(cPlayer *Player) {
	SoldierGameObj *obj = Player->Get_GameObj();
	if (Allow_Taunt(obj)) {
		Commands->Set_Animation(obj,"H_A_sidekick",false,0,0.0f,-1.0f,false);
	}
}

void DATauntsGameFeatureClass::Taunt9_Key_Hook(cPlayer *Player) {
	SoldierGameObj *obj = Player->Get_GameObj();
	if (Allow_Taunt(obj)) {
		Commands->Set_Animation(obj,"H_A_punchcombo",false,0,0.0f,-1.0f,false);
	}
}

void DATauntsGameFeatureClass::Taunt0_Key_Hook(cPlayer *Player) {
	SoldierGameObj *obj = Player->Get_GameObj();
	if (Allow_Taunt(obj)) {
		Commands->Set_Animation(obj,"H_A_bodyslam",false,0,0.0f,-1.0f,false);
	}
}

void DATauntsGameFeatureClass::TauntS_Key_Hook(cPlayer *Player) {
	SoldierGameObj *obj = Player->Get_GameObj();
	if (Allow_Taunt(obj)) {
		Commands->Set_Animation(obj,"H_A_deathblast",false,0,0.0f,-1.0f,false);
	}
}

Register_Game_Feature(DATauntsGameFeatureClass,"Taunts","EnableTaunts",0);