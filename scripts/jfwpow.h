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
class JFW_Spawn_Timer : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Warp_On_Pickup_Team : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Refill_Powerup : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Vehicle_Powerup : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Soldier_Powerup : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Vehicle_Powerup_Preset : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Powerup_Send_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Powerup_Send_Custom_Team : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Give_Powerup_Create : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Clear_Money_Create : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Change_Character_Powerup : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Change_Character_Powerup_2 : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Give_Money : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};
