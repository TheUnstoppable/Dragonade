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

class RA_Repair_Controller : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class RA_Repair_Script : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class RA_Infantry_Spy : public ScriptImpClass {
	void Created(GameObject *obj);
	void Detach(GameObject *obj);
};

class RA_Infantry_NotSpy : public ScriptImpClass {
	void Created(GameObject *obj);
};

class RA_Helipad_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class RA_MineLayer : public ScriptImpClass {
	int driverID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int mesage,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	public: void Register_Auto_Save_Variables();
};

class RA_Mine : public ScriptImpClass {
	bool AP_mine;
	bool AT_mine;
	int ownerID;
	int team;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int mesage,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Destroyed(GameObject *obj);
	public: void Register_Auto_Save_Variables();
};

class RA_Mine_Manager : public ScriptImpClass {
	int all_mines;
	int sov_mines;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class RA_ObjectDeath_OnCreate : public ScriptImpClass {
	void Created(GameObject *obj);
};

class RA_Naval_Yard : public ScriptImpClass {
	int team_count;
	bool active;
	bool ready;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class RA_Naval_PT_Pokable : public ScriptImpClass {
	int storedID;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class RA_Naval_Unit : public ScriptImpClass {
	void Destroyed(GameObject *obj);
};

class RA_Naval_Zone : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class RA_Damaged_Credits : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float amount);
};

class RA_GameStart_Detector : public ScriptImpClass {
	void Created(GameObject *obj);
	void Killed (GameObject *obj,GameObject *killer);
	void Timer_Expired (GameObject *obj,int number);
};


/*!
* \brief Driver Death Script
*
* This script kills the object it is attached to after 0.5 seconds and is generally attached by
* another script to kill a vehicle driver once the parent vehicle has died (since doing so on the
* same engine frame would cause a crash). It could also be attached on a custom message by using
* a script such as JFW_Attach_Script_Custom
*
* \pre
*   A warhead of type "Death" must exist in armor.ini which is capable of damaging the object this
*   script is attached to.
*/
class RA_DriverDeath : public ScriptImpClass
{
  void Created(GameObject *obj);
  void Timer_Expired(GameObject *obj,int number);
};

class RA_Conyard_Repair : public ScriptImpClass {
	bool Enabled;
	int objID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int timer_id);
	public: void Register_Auto_Save_Variables();
};

class RA_ObjectDeath_OnDeath : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class RA_Demolition_Truck_Improved :public ScriptImpClass {
	int driverID;
	bool triggered;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Killed(GameObject *obj,GameObject *killer);
	public: void Register_Auto_Save_Variables();
};

class RA_CTF_Zone:public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int timer_id);
	void Created(GameObject *obj);
	public: void Register_Auto_Save_Variables();
	int NumCaptured;
	bool GotFlag;
	unsigned int DecorFlag;
	unsigned int FlagId;
	unsigned int PlayerId;
	bool DecorFlagPresent;
};

class RA_MAD_Tank_Improved : public ScriptImpClass {
	int driverID;
	bool deployed;
	int newobjID;
	int ownerID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	void Destroyed(GameObject *obj);
	public: void Register_Auto_Save_Variables();
};

class RA_Conyard_Controller_Improved : public ScriptImpClass {
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class RA_Conyard_Controller_Improved_2 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class RA_Building_DeathSound : public ScriptImpClass {
	bool halfdead;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float amount);
	void Killed(GameObject *obj, GameObject *killer);
	public: void Register_Auto_Save_Variables();
};

class RA_Base_Defense_Simple : public ScriptImpClass {
private:
	unsigned long targetID;
	bool attacking;
	int current_priority;
	float attack_timer;
	int loop_count;
	void Attack(GameObject *obj, GameObject *target); 
	GameObject* Select_Target(GameObject *obj, GameObject *target);
	GameObject* Set_Target(GameObject *target);
	GameObject* Tie_Breaker(GameObject *obj, GameObject *new_target, GameObject *old_target);
	bool Adjust_For_Infantry(GameObject *target);
public:
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj, GameObject *enemy);
	void Damaged(GameObject *obj, GameObject *damager, float amount);
	void Timer_Expired(GameObject *obj, int number);
	void Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason);
	public: void Register_Auto_Save_Variables();
};

class RA_Base_Defense_Powered : public ScriptImpClass {
private:
	unsigned long targetID;
	bool attacking;
	int current_priority;
	float attack_timer;
	int loop_count;
	void Attack(GameObject *obj, GameObject *target); 
	GameObject* Select_Target(GameObject *obj, GameObject *target);
	GameObject* Set_Target(GameObject *target);
	GameObject* Tie_Breaker(GameObject *obj, GameObject *new_target, GameObject *old_target);
	bool Adjust_For_Infantry(GameObject *target);
public:
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj, GameObject *enemy);
	void Damaged(GameObject *obj, GameObject *damager, float amount);
	void Timer_Expired(GameObject *obj, int number);
	void Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason);
	public: void Register_Auto_Save_Variables();
};

class RA_Base_Defense_Chargeup : public ScriptImpClass {
private:
	unsigned long targetID;
	unsigned long damageID;
	unsigned long damagerID;
	bool attacking;
	int current_priority;
	float attack_timer;
	int loop_count;
	void Attack(GameObject *obj, GameObject *target); 
	void Attack_Charged(GameObject *obj, GameObject *target); 
	GameObject* Select_Target(GameObject *obj, GameObject *target);
	GameObject* Set_Target(GameObject *target);
	GameObject* Tie_Breaker(GameObject *obj, GameObject *new_target, GameObject *old_target);
	bool Adjust_For_Infantry(GameObject *target);
public:
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj, GameObject *enemy);
	void Damaged(GameObject *obj, GameObject *damager, float amount);
	void Timer_Expired(GameObject *obj, int number);
	void Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	public: void Register_Auto_Save_Variables();
};

class RA_Vehicle_Regen : public ScriptImpClass {
	bool healing;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float amount);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
};

class RA_Thief : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class RA_Thief_Improved : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class RA_Credit_Theft_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class RA_Credit_Theft_Zone_Timer : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj, int number);
};

class RA_DestroyNearest_OnDeath : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *killer);
};

class RA_Ore_Truck : public ScriptImpClass {
	int scoops;
	float value;
	bool harvesting;
	int field;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
};

class RA_Ore_Truck_2 : public ScriptImpClass {
	int scoops;
	float value;
	bool harvesting;
	int field;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
};

class RA_Ore_Field : public ScriptImpClass { 
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
};
	
class RA_Ore_Delivery_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class RA_Vehicle_Team_Set : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class RA_Vehicle_Team_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class RA_Visible_Driver : public ScriptImpClass {
	unsigned long modelID;
	unsigned long driverID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	void Killed(GameObject *obj,GameObject *killer);
	void Destroyed(GameObject *obj);
	public: void Register_Auto_Save_Variables();
};

class RA_Vision_Control : public ScriptImpClass {
	bool allowed;
	bool underwater;
	bool driving;
	bool blackout;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class RA_Vision_Control_2 : public ScriptImpClass {
	bool allowed;
	bool underwater;
	bool driving;
	bool blackout;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class RA_Fog_Level_Settings : public ScriptImpClass {
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
};

class RA_Missile_Beacon : public ScriptImpClass {
	void Created(GameObject *obj); //send message to team's RA_Missile_Control
};

class RA_Beacon_Terminal : public ScriptImpClass {
	bool active; //true if beacon is available
	bool reset; //true when spy pokes terminal
	void Created(GameObject *obj); //Add Beacon_Timer
	void Custom(GameObject *obj, int type, int param, GameObject *sender); 
	void Poked(GameObject *obj, GameObject *poker); //check if poker is friend or enemy spy
	void Timer_Expired(GameObject *obj, int number); //Slight time delay after spy disable, before timer script re-added
	public: void Register_Auto_Save_Variables();
};

class RA_Beacon_Terminal_2 : public ScriptImpClass {
	bool active; //true if beacon is available
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int type, int param, GameObject *sender); 
	void Poked(GameObject *obj, GameObject *poker);
	void Damaged(GameObject *obj, GameObject *damager, float amount);
	void Timer_Expired(GameObject *obj, int number);
};

class RA_Beacon_Timer : public ScriptImpClass { 
	//acts as timer for Beacon_Terminal
	//This script will be removed & replaced if a spy pokes the terminal
	//This should reset the timer
	void Created(GameObject *obj); //Start Timer
	void Timer_Expired(GameObject *obj, int number); //Send message to Terminal and remove self
};

class RA_Missile_Control : public ScriptImpClass {
	bool launching; //true when animating
	bool alive; //false when building dies
	void Created(GameObject *obj); //initialize variables
	void Custom(GameObject *obj, int type, int param, GameObject *sender); 
	void Animation_Complete(GameObject *obj,const char *animation_name);
	void Timer_Expired(GameObject *obj, int number); //Time delay to kill beacons
	public: void Register_Auto_Save_Variables();
};

class RA_Missile_Controller : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *killer); //inform Missile_Control and Beacon_Terminal
};

class RA_Demolition_Truck_Retarded :public ScriptImpClass {
	int driverID;
	bool triggered;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Killed(GameObject *obj,GameObject *killer);
	public: void Register_Auto_Save_Variables();
};

class RA_MAD_Tank_Devolved : public JFW_Key_Hook_Base {
	bool deployed;
	int newobjID;
	int ownerID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	void Destroyed(GameObject *obj);
	void KeyHook();
	public: void Register_Auto_Save_Variables();
};

class RA_Superweapon_Beacon : public ScriptImpClass {
	void Created(GameObject *obj);
};
