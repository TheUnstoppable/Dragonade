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
class JFW_Add_Objective : public ScriptImpClass {
	float timertime;
	int timernumber;
	bool repeat;
	void Entered(GameObject *obj,GameObject *enterer);
	void Poked(GameObject *obj,GameObject *poker);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Remove_Objective : public ScriptImpClass {
	float timertime;
	int timernumber;
	bool repeat;
	void Entered(GameObject *obj,GameObject *enterer);
	void Poked(GameObject *obj,GameObject *poker);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int timer_id);
	public: void Register_Auto_Save_Variables();
};

class JFW_Set_Objective_Status : public ScriptImpClass {
	float timertime;
	int timernumber;
	bool repeat;
	void Entered(GameObject *obj,GameObject *enterer);
	void Poked(GameObject *obj,GameObject *poker);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int timer_id);
	public: void Register_Auto_Save_Variables();
};

class JFW_Change_Objective_Type : public ScriptImpClass {
	float timertime;
	int timernumber;
	bool repeat;
	void Entered(GameObject *obj,GameObject *enterer);
	void Poked(GameObject *obj,GameObject *poker);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int timer_id);
	public: void Register_Auto_Save_Variables();
};

class JFW_Set_Objective_Radar_Blip : public ScriptImpClass {
	float timertime;
	int timernumber;
	bool repeat;
	void Entered(GameObject *obj,GameObject *enterer);
	void Poked(GameObject *obj,GameObject *poker);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int timer_id);
	public: void Register_Auto_Save_Variables();
};

class JFW_Set_Objective_Radar_Blip_Object : public ScriptImpClass {
	float timertime;
	int timernumber;
	bool repeat;
	void Entered(GameObject *obj,GameObject *enterer);
	void Poked(GameObject *obj,GameObject *poker);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int timer_id);
	public: void Register_Auto_Save_Variables();
};

class JFW_Set_Objective_HUD_Info : public ScriptImpClass {
	float timertime;
	int timernumber;
	bool repeat;
	void Entered(GameObject *obj,GameObject *enterer);
	void Poked(GameObject *obj,GameObject *poker);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int timer_id);
	public: void Register_Auto_Save_Variables();
};

class JFW_Set_Objective_HUD_Info_Position : public ScriptImpClass{
	float timertime;
	int timernumber;
	bool repeat;
	void Entered(GameObject *obj,GameObject *enterer);
	void Poked(GameObject *obj,GameObject *poker);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int timer_id);
	public: void Register_Auto_Save_Variables();
};

class JFW_Set_Num_Tertary_Objectives : public ScriptImpClass {
	float timertime;
	int timernumber;
	bool repeat;
	void Entered(GameObject *obj,GameObject *enterer);
	void Poked(GameObject *obj,GameObject *poker);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int timer_id);
	public: void Register_Auto_Save_Variables();
};

class JFW_Mission_Complete : public ScriptImpClass {
	float timertime;
	int timernumber;
	bool repeat;
	void Entered(GameObject *obj,GameObject *enterer);
	void Poked(GameObject *obj,GameObject *poker);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int timer_id);
	public: void Register_Auto_Save_Variables();
};

class JFW_Start_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Poked(GameObject *obj,GameObject *poker);
	void Entered(GameObject *obj,GameObject *enterer);
};
