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


class Iran_Beaconing_Prevention_Zone : public ScriptImpClass {
	// This is an experimental script that might crash the FDS, don't use this.
	void Entered(GameObject *obj,GameObject *enter);
	void Timer_Expired(GameObject *obj,int number);
	void Remove_Weapon_Safely(GameObject *obj,const char *WeaponName);
};

// Generic kill zone scripts, based on the JFW_ damage zone scripts written by jonwil
class Iran_Vehicle_All_Kill_Zone: public ScriptImpClass {
	int saved;
	void Entered(GameObject *obj,GameObject *enter);
	void Timer_Expired(GameObject *obj,int number);
};

class Iran_All_Kill_Zone: public ScriptImpClass {
	int saved;
	void Entered(GameObject *obj,GameObject *enter);
	void Timer_Expired(GameObject *obj,int number);
};

class Iran_Star_Kill_Zone: public ScriptImpClass {
	int saved;
	void Entered(GameObject *obj,GameObject *enter);
	void Timer_Expired(GameObject *obj,int number);
};

class Iran_Vehicle_Ground_Kill_Zone: public ScriptImpClass {
	int saved;
	void Entered(GameObject *obj,GameObject *enter);
	void Timer_Expired(GameObject *obj,int number);
};

class Iran_Vehicle_Flying_Kill_Zone: public ScriptImpClass {
	int saved;
	void Entered(GameObject *obj,GameObject *enter);
	void Timer_Expired(GameObject *obj,int number);
};

// Destroys the object it's attached to and crates an invisible laser fence blocker
// at the same position and facing
class Iran_Invisible_Blocker : public ScriptImpClass {
	void Created(GameObject *obj);
};

// Same as above but shows the laser fence, for placing purposes 
class Iran_Invisible_Blocker_Visible : public ScriptImpClass {
	void Created(GameObject *obj);
};