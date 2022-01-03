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
void ObjectCreateHookBaseHook(void *data,GameObject *obj);

void KeyHookBaseHook(void *data);

class JFW_Jetpack : public JFW_Key_Hook_Base {
	bool enabled;
	bool time;
	void Detach(GameObject *obj);
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void KeyHook();
};

class JFW_Jetpack_Timer : public JFW_Key_Hook_Base {
	bool enabled;
	bool time;
	void Detach(GameObject *obj);
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void KeyHook();
};

class JFW_Limited_Jetpack_Timer : public JFW_Key_Hook_Base {
	bool enabled;
	bool timeon;
	bool timeoff;
	void Detach(GameObject *obj);
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void KeyHook();
};

class JFW_Jetpack_Model : public JFW_Key_Hook_Base {
	bool enabled;
	bool time;
	void Detach(GameObject *obj);
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void KeyHook();
};

class JFW_Attach_Script_Preset_Created : public JFW_Object_Created_Hook_Base {
	void ObjectCreateHook(GameObject *obj);
};

class JFW_Attach_Script_Type_Created : public JFW_Object_Created_Hook_Base {
	void ObjectCreateHook(GameObject *obj);
};

class JFW_Attach_Script_Player_Created : public JFW_Object_Created_Hook_Base {
	void Created(GameObject *obj);
	void ObjectCreateHook(GameObject *obj);
};

class JFW_Attach_Script_Vehicle_Created : public JFW_Object_Created_Hook_Base {
	void ObjectCreateHook(GameObject *obj);
};

class JFW_Dplbl_Vhcls_Keyboard : public JFW_Key_Hook_Base {
	int pilotID;
	bool CanDestroyAnim;
	void Created(GameObject *obj);
	public: void Register_Auto_Save_Variables();
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void KeyHook();
};

class JFW_Underground_Logic : public JFW_Key_Hook_Base {
	bool underground;
	bool enable;
	int pilotID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void KeyHook();
	void Killed(GameObject *obj,GameObject *killer);
	public: void Register_Auto_Save_Variables();
};

class JFW_Suicide_Bomber : public JFW_Key_Hook_Base {
	bool enabled;
	void Created(GameObject *obj);
	void KeyHook();
	public: void Register_Auto_Save_Variables();
};

class JFW_Sidebar_Key_2 : public ScriptImpClass {
};

class JFW_Jetpack_Timer_No_Fly : public JFW_Key_Hook_Base {
	bool enabled;
	bool time;
	bool time2;
	bool nofly;
	void Detach(GameObject *obj);
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void KeyHook();
};

class JFW_Limited_Jetpack_Timer_No_Fly : public JFW_Key_Hook_Base {
	bool enabled;
	bool timeon;
	bool timeoff;
	bool nofly;
	void Detach(GameObject *obj);
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void KeyHook();
};
