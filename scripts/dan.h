/*	Renegade Scripts.dll
	Copyright 2017 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#pragma once
class DAN_CnC_Crate : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	// There isn't really much need for more than GameObject *obj in the following:
	void Points(GameObject *obj,int RandNum);	// Give/Take Points
	void DeathCrate(GameObject *obj);			// Death Crate
	void StealthSuit(GameObject *obj);			// Enables Stealth
	void DeHealth(GameObject *obj);			// Sets Health to 1
	void DeArmour(GameObject *obj);			// Sets Armour to 1
	void FullHealth(GameObject *obj);			// Sets Health and Armour to full
	void Money(GameObject *obj,int RandNum);	// Give/Take Money
	void SpyCrate(GameObject *obj);			// Base Defenses will ignore player
	void RandomWeapon(GameObject *obj);		// Gives a Random Weapon
	void Theif(GameObject *obj);				// Takes all player's money
	int VehBlock;
	public: void Register_Auto_Save_Variables();
};

class DAN_Drop_Wreckage_On_Death : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};
class DAN_Wreckage_Rebuildable : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
};
