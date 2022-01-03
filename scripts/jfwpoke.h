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
class JFW_Poke_Send_Custom : public ScriptImpClass {
	int active;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Poked(GameObject *obj,GameObject *poker);
	public: void Register_Auto_Save_Variables();
};

class JFW_Pokeable_Item : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Preset_Buy_Poke : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Character_Buy_Poke : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Refill_Buy_Poke : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Powerup_Buy_Poke : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Vehicle_Buy_Poke : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Group_Purchase_Poke : public ScriptImpClass {
	float cash;
	bool enabled;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	public: void Register_Auto_Save_Variables();
};

class JFW_Gate_Poke : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Poke_Send_Custom_2 : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Poke_Send_Custom_Preset : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Poke_Send_Custom_Keycard : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Poke_Send_Custom_Cost : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Poke_Play_2D_Sound : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Poke_Play_3D_Sound : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Powerup_Buy_Poke_Timer : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Poked(GameObject *obj,GameObject *poker);
	public: void Register_Auto_Save_Variables();
};

class JFW_PCT : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Poke_Play_2D_Sound_Team : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Poke_Play_3D_Sound_Team : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Poke_Play_2D_Sound_Player : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Poke_Play_3D_Sound_Player : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Look_At_Location_Poke : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Look_At_Object_Poke : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Weapon_Buy_Poke : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Preset_Buy_Poke_Timer : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Poked(GameObject *obj,GameObject *poker);
	public: void Register_Auto_Save_Variables();
};

class JFW_Preset_Buy_Poke_Custom : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Poked(GameObject *obj,GameObject *poker);
	public: void Register_Auto_Save_Variables();
};

class JFW_Slot_Machine : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Slot_Machine_2 : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Switch_Door : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Switch_Lock_Door : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Toggle_Door : public ScriptImpClass {
	bool open;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	public: void Register_Auto_Save_Variables();
};

class JFW_Toggle_Lock_Door : public ScriptImpClass {
	bool open;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	public: void Register_Auto_Save_Variables();
};

class JFW_Switch_Door_Team : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Toggle_Door_Team : public ScriptImpClass {
	bool open;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	public: void Register_Auto_Save_Variables();
};

class JFW_Preset_Buy_Poke_Sound : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Character_Buy_Poke_Sound : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Refill_Buy_Poke_Sound : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Powerup_Buy_Poke_Sound : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Vehicle_Buy_Poke_Sound : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Powerup_Buy_Poke_Timer_Sound : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Poked(GameObject *obj,GameObject *poker);
	public: void Register_Auto_Save_Variables();
};

class JFW_Weapon_Buy_Poke_Sound : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Preset_Buy_Poke_Timer_Sound : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Poked(GameObject *obj,GameObject *poker);
	public: void Register_Auto_Save_Variables();
};

class JFW_Group_Purchase_Poke_Sound : public ScriptImpClass {
	float cash;
	bool enabled;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	public: void Register_Auto_Save_Variables();
};

class JFW_Preset_Buy_Poke_Custom_Sound : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Poked(GameObject *obj,GameObject *poker);
	public: void Register_Auto_Save_Variables();
};

class JFW_Switch_Door_2 : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Switch_Lock_Door_2 : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Toggle_Door_2 : public ScriptImpClass {
	bool open;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	public: void Register_Auto_Save_Variables();
};

class JFW_Toggle_Lock_Door_2 : public ScriptImpClass {
	bool open;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	public: void Register_Auto_Save_Variables();
};

class JFW_Switch_Door_Team_2 : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Toggle_Door_Team_2 : public ScriptImpClass {
	bool open;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	public: void Register_Auto_Save_Variables();
};

class JFW_Lock_Sound : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JFW_Poke_Send_Custom_Toggle : public ScriptImpClass {
	bool toggle;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	public: void Register_Auto_Save_Variables();
};

class JFW_Poke_Play_2D_Sound_Timer : public ScriptImpClass {
	bool active;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	public: void Register_Auto_Save_Variables();
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Poke_Play_3D_Sound_Timer : public ScriptImpClass {
	bool active;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	public: void Register_Auto_Save_Variables();
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Sidebar_PT : public ScriptImpClass {
};

class JFW_Escort_Poke : public ScriptImpClass {
	int follower;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
};
