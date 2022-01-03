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

#ifndef INCLUDE_DARADIO
#define INCLUDE_DARADIO

#include "da_event.h"
#include "da_gamefeature.h"
#include "da_player.h"
#include "engine_game.h"

class DAExtraRadioCommandsPlayerDataClass : public DAPlayerDataClass {
public:
	void Set_Last_Damage(GameObject *obj) {
		LastDamage = obj;
	}
	GameObject *Get_Last_Damage() {
		return LastDamage;
	}
	
private:
	virtual void Init() {
		LastDamage = 0;
	}
	ReferencerClass LastDamage;
};

class DAExtraRadioCommandsGameFeatureClass : public DAEventClass, public DAGameFeatureClass, public DAPlayerDataManagerClass<DAExtraRadioCommandsPlayerDataClass> {
	virtual void Init();
	virtual void Damage_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type);
	void Radio1_Key_Hook(cPlayer *Player); //Repair the <building>!
	void Radio2_Key_Hook(cPlayer *Player); //Enemy beacon detected at the <building>!
	void Radio3_Key_Hook(cPlayer *Player); //Defend the pedestal!
	void Radio4_Key_Hook(cPlayer *Player); //Cease fire.
	void Radio5_Key_Hook(cPlayer *Player); //Focus fire on <player>!
	void Radio6_Key_Hook(cPlayer *Player); //Enemy <type> spotted!
	void Radio7_Key_Hook(cPlayer *Player); //Get that crate/weapon/backpack.
	void Radio8_Key_Hook(cPlayer *Player); //Thanks.
	void Radio9_Key_Hook(cPlayer *Player); //Sorry.
	void Radio10_Key_Hook(cPlayer *Player); //Wait up!
	void Radio11_Key_Hook(cPlayer *Player); //Incoming enemy <type>!
	void Radio12_Key_Hook(cPlayer *Player); //Requesting more <vehicle> for a <vehicle> rush.
	void Radio13_Key_Hook(cPlayer *Player); //I'm going for a refill.
	void Radio14_Key_Hook(cPlayer *Player); //Watch out! There's C4 on you!
	void Radio15_Key_Hook(cPlayer *Player); //Behind us!
	void Radio16_Key_Hook(cPlayer *Player); //I'm taking fire!
	void Radio17_Key_Hook(cPlayer *Player); //Stay in the vehicle!
	void Radio18_Key_Hook(cPlayer *Player); //I'm mining the <building>.
	void Radio19_Key_Hook(cPlayer *Player); //The <building> needs mining.
};

#endif