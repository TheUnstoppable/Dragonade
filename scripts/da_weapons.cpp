/*	Renegade Scripts.dll
    RenCorner Purchasable Weapons Game Feature
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
#include "engine_da.h"
#include "da.h"
#include "da_disable.h"
#include "da_settings.h"
#include "da_translation.h"
#include "da_player.h"
#include "da_log.h"
#include "da_weapons.h"

void DAWeaponsGameFeatureClass::Init() {
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Event(DAEvent::CHATCOMMAND);
	Register_Chat_Command((DAECC)&DAWeaponsGameFeatureClass::Weapons_Chat_Command,"!weapons|!weapon|!weaps|!weap|!weps|!wep|!buy");
}

void DAWeaponsGameFeatureClass::Settings_Loaded_Event() {
	Weapons.Delete_All();
	INISection *Section = DASettingsManager::Get_Section("Purchasable_Weapons");
	if (Section) {
		for (INIEntry *i = Section->EntryList.First();i && i->Is_Valid();i = i->Next()) {
			DATokenParserClass EntryParser(i->Entry,'|');
			DATokenParserClass ValueParser(i->Value,'|');
			WeaponStruct Weapon;
			while (char *Token = EntryParser.Get_String()) {
				Weapon.Triggers.Add(Token);
			}
			Weapon.Weapon[0] = (WeaponDefinitionClass*)Find_Named_Definition(ValueParser.Get_String());
			if (Weapon.Weapon[0] && Weapon.Weapon[0]->Get_Class_ID() != CID_Weapon) {
				Weapon.Weapon[0] = 0;
			}
			Weapon.Weapon[1] = (WeaponDefinitionClass*)Find_Named_Definition(ValueParser.Get_String());
			if (Weapon.Weapon[1] && Weapon.Weapon[1]->Get_Class_ID() != CID_Weapon) {
				Weapon.Weapon[1] = 0;
			}
			if ((Weapon.Weapon[0] || Weapon.Weapon[1]) && ValueParser.Get_Int(Weapon.Cost)) {
				Weapons.Add(Weapon);
			}
		}
	}
}

bool DAWeaponsGameFeatureClass::Weapons_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	int Team = Player->Get_Player_Type();
	if (Team != 0 && Team != 1) {
		return true;
	}

	bool PP = BaseControllerClass::Find_Base(Team)->Is_Base_Powered();
	float Discount = Player->Get_DA_Player()->Get_PowerUp_Discount();

	StringClass Send;
	for (int i = 0;i < Weapons.Count();i++) {
		if (Weapons[i].Weapon[Team]) {
			int Cost = (int)Round((float)Weapons[i].Cost*Discount);
			if (!PP) {
				Cost *= 2;
			}
			StringClass Buffer;
			Buffer.Format("%s: %s, %d - ",Weapons[i].Triggers[0],DATranslationManager::Translate(Weapons[i].Weapon[Team]),Cost);
			if (Send.Get_Length() + Buffer.Get_Length() > 220) {
				Send.TruncateRight(3);
				DA::Private_Color_Message(Player,COLORGRAY,"%s",Send);
				Send = Buffer;
			}
			else {
				Send += Buffer;
			}
		}
	}
	if (!Send.Is_Empty()) {
		Send.TruncateRight(3);
		DA::Private_Color_Message(Player,COLORGRAY,"%s",Send);
	}
	return true;
}

bool DAWeaponsGameFeatureClass::Chat_Command_Event(cPlayer *Player,TextMessageEnum Type,const StringClass &Command,const DATokenClass &Text,int ReceiverID) {
	for (int i = 0;i < Weapons.Count();i++) {
		if (Weapons[i].Triggers.ID(Command) != -1) {
			int Team = Player->Get_Player_Type();
			if ((Team != 0 && Team != 1) || !Weapons[i].Weapon[Team]) {
				DA::Private_Color_Message(Player,COLORGRAY,"Your team cannot purchase that weapon.");
				return false;
			}
			else if (Get_Distance_To_Closest_PCT(Player->Get_GameObj()->Get_Position(),Team) > 10.0f) {
				DA::Private_Color_Message(Player,COLORGRAY,"You must be at a Purchase Terminal to use this command.");
				return false;
			}
			else {
				WeaponDefinitionClass *WeaponDef = Weapons[i].Weapon[Team];
				if (Player->Get_GameObj()->Get_Weapon_Bag()->Find_Weapon(WeaponDef)) {
					DA::Private_Color_Message(Player,COLORGRAY,"You already have that weapon.");
					return false;
				}
				else {
					int Cost = (int)Round((float)Weapons[i].Cost*Player->Get_DA_Player()->Get_PowerUp_Discount());
					if (!BaseControllerClass::Find_Base(Team)->Is_Base_Powered()) {
						Cost *= 2;
					}
					if (Player->Get_Money() < Cost) {
						DA::Private_Color_Message(Player,COLORGRAY,"Insufficient funds. %s currently costs %d credits.",A_Or_An_Prepend(DATranslationManager::Translate(WeaponDef)),Cost);
						if (Team == 0) {
							Create_2D_WAV_Sound_Player_By_ID(Player->Get_Id(),"M00EVAN_DSGN0024I1EVAN_snd.wav");
						}
						else {
							Create_2D_WAV_Sound_Player_By_ID(Player->Get_Id(),"M00EVAG_DSGN0028I1EVAG_snd.wav");
						}
						return false;
					}
					else {
						if (Player->Get_GameObj()->Get_Weapon_Bag()->Add_Weapon(WeaponDef,999,true)) {
							Player->Purchase_Item(Cost);
							Create_2D_WAV_Sound_Player_By_ID(Player->Get_Id(),"M00GBMG_SECX0002I1GBMG_snd.wav");
							DALogManager::Write_Log("_PURCHASE","%ls - %s",Player->Get_Name(),DATranslationManager::Translate(WeaponDef));
							DA::Private_Color_Message(Player,COLORGRAY,"You have purchased %s for %d credits.",a_or_an_Prepend(DATranslationManager::Translate(WeaponDef)),Cost);
						}
						else {
							DA::Private_Color_Message(Player,COLORGRAY,"Your current character cannot use that weapon.");
						}
					}
				}
			}
		}
	}
	return true;
}

Register_Game_Feature(DAWeaponsGameFeatureClass,"Purchasable Weapons","EnablePurchasableWeapons",0);