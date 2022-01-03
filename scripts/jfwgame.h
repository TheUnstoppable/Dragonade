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
class JFW_Team_DM_Zone : public ScriptImpClass {
	bool triggered;
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Exited(GameObject *obj,GameObject *exiter);
	public: void Register_Auto_Save_Variables();
};

class JFW_Team_DM_Controller : public ScriptImpClass {
	int deaths;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Team_DM_Attatched : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Assault_Powerplant_Controller : public ScriptImpClass {
	bool inzone;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	public: void Register_Auto_Save_Variables();
};

class JFW_Resource_Collector : public ScriptImpClass {
	int cash;
	int ID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Resource_Refinery : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Resource_Field : public ScriptImpClass {
	int ID;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Domination_Zone : public ScriptImpClass {
	unsigned int currentmodelid;
	bool enabled;
	int owner;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Domination_Controler : public ScriptImpClass {
	int gdizonecount;
	int nodzonecount;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Iron_Curtain_Zone : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Chrono_Controler_Zone : public ScriptImpClass {
	bool enabled;
	bool activating;
	float xpos;
	float ypos;
	float zpos;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Drone_Damage : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Conyard : public ScriptImpClass {
	bool disabled;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Conyard_Vehicle_Buildings : public ScriptImpClass {
	bool disabled;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Flying_Infantry : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Flying_Infantry_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Flying_Infantry_Powerup : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Flying_Infantry_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Flying_Infantry_Exit : public ScriptImpClass {
	void Exited(GameObject *obj,GameObject *exiter);
};

class JFW_Team_DM_Controller_2 : public ScriptImpClass {
	int deaths;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Vehicle_Thief : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float amount);
};

class JFW_Vehicle_Thief_2 : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float amount);
};

class JFW_Domination_Controler_End_Game : public ScriptImpClass {
	int gdizonecount;
	int nodzonecount;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Carryable_Vehicle : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_Carry_Control : public ScriptImpClass {
	int carryall1;
	int carryall2;
	int carryall3;
	int carryall4;
	int carryall5;
	int carryall6;
	int carryall7;
	int carryall8;
	int carryall9;
	int carryall10;
	int carryall11;
	int carryall12;
	int carryall13;
	int carryall14;
	int carryall15;
	int carryall16;
	int carryall17;
	int carryall18;
	int carryall19;
	int carryall20;
	int carrycount;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_CarryAll : public ScriptImpClass {
	bool carrying;
	int id;
	int pilotID;
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Killed(GameObject *obj,GameObject *killer);
	public: void Register_Auto_Save_Variables();
};

class JFW_Sensor_Array : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Conyard_Radius : public ScriptImpClass {
	bool disabled;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Chrono_Harvester_Logic : public ScriptImpClass {
	int HarvyID;
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Chrono_Harvester_Kill : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_Chrono_Harvester_Spawn_Kill : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_Chrono_Harvester_Attached : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
	bool End_Timer;
	float facing;
	int CrystalNumber;
	int PathNumber;
	int Crystal1,Crystal2,Crystal3,Crystal4,Crystal5,Crystal6,Crystal7,Crystal8,Crystal9,Crystal10;
};

class JFW_Conyard_Turrets : public ScriptImpClass {
	bool disabled;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Radar_Jammer : public ScriptImpClass {
	bool radaron;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Radar_Jammer_Sound : public ScriptImpClass {
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Sonar_Pulse : public ScriptImpClass {
	bool reset;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Global_Stealth_Controller : public ScriptImpClass {
	Gap_ListNode *ListHead;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	void Destroyed(GameObject *obj);
	void Add_Generator(unsigned long ObjectID, float Range, int Team);
	void Remove_Generator(unsigned long ID);
	void Update_Generator(unsigned long ID, bool Active, int Team);
	void Update_Generator(unsigned long ID, bool Active);
};

class JFW_Stealth_Generator_Building : public ScriptImpClass {
	unsigned long sizeID;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Killed(GameObject *obj, GameObject *killer);
};

class JFW_Stealth_Generator_Vehicle : public ScriptImpClass {
	unsigned long sizeID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	void Destroyed(GameObject *obj);
};

class JFW_Stealth_Generator_Vehicle_Deploy : public ScriptImpClass {
	bool deploy;
	unsigned long sizeID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	void Destroyed(GameObject *obj);
};

class JFW_Stealth_Generator_Ignored : public ScriptImpClass {
};

class JFW_Resource_Collector_2 : public ScriptImpClass {
	bool HasOre;
	int cash;
	int ID;
	int PlayerID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Resource_Collector_3 : public ScriptImpClass {
	bool HasOre;
	int cash;
	int ID;
	int PlayerID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Low_Power_Message : public ScriptImpClass {
	bool lowpower;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Low_Power_Sound : public ScriptImpClass {
	bool lowpower;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Message_Send_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Message_Send_Zone_Team : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Message_Send_Zone_Player : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Message_Send_Death : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *killer);
};

class JFW_Message_Send_Death_Team : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *killer);
};

class JFW_Message_Send_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Message_Send_Powerup : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Message_Send_Custom_Team : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Spy_Switch_Team : public ScriptImpClass {
	void Created(GameObject *obj);
	void Killed(GameObject *obj, GameObject *killer);
	void Destroyed(GameObject *obj);
	void Detach(GameObject *obj);
};

class JFW_Spy_Vehicle_Ignore : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	int spycount;
	int nonspycount;
};

class JFW_Spy_Vehicle_Ignore_New : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	int spycount;
	int nonspycount;
};

class JFW_Message_Send_Zone_Player_Vehicle : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Water_Level : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Submarine : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj, GameObject *killer);
	float waterlevel;
	bool submerged;
	bool occupied;
	int driverID;
};

class JFW_Reaper_Web : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	int WebID;
};

class JFW_Toxic_Grenade : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	bool active;
};

class JFW_Hijacker_Vehicle : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *killer);
	int HijackerID;
};

class JFW_Hijacker_Vehicle_2 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *killer);
	int HijackerID;
	bool jacking;
};

class JFW_Building_Preset_Disable : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	unsigned int count;
};
