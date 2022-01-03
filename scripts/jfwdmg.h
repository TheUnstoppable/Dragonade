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

// -------------------------------------------------------------------------------------------------

/*!
*\ brief Spawns a new object when an object is killed
* \author jonwil
*
* When the object this script is attached to is killed it will spawn a new object at the location it
* was killed. This is useful for spawning a destroyed version of an object.
*
* \param Drop_Object
*   Name of the preset to be created when the object is killed
* \param Drop_Height
*   Height above the location where the object was killed to spawn the new object, or 0 to spawn it
*   at the same height as the object was killed. Can also be negative. Helps to prevent issues with
*   terrain clipping when the destroyed object has a different origin point
*
* \note
*   The object the script is attached to must be killed through damage applied to it for this script
*   to work. If the object is simply destroyed (such as by JFW_Destroy_Self_Timer) no object will be
*   spawned.
*/
class JFW_Spawn_Object_Death : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Destroys a building by applying damage after a fixed period of time
* \author jonwil
*
* This script will apply 10,000 damage using the <b>Explosive</b> warhead to the object it is
* attached to after a specified number of seconds has elapsed, which is sufficient to destroy
* any standard building in Renegade.
*
* \note
*   Although the script is intended for use with buildings it will actually work on any damagable
*   game object
*
* \pre
*   Requires a warhead called <b>Explosive</b> to be defined in armor.ini. This is already present
*   in stock versions of Renegade but may not exist in total conversion modifications.
*
* \param Time
*   Number of seconds to wait before triggering
* \param TimerNum
*   Internal ID to use for the timer to avoid clashing with any other timers on the same object
*/
class JFW_Timer_Destroy_Building : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Destroys an object by applying damage after a fixed period of time
* \author jonwil
*
* This script will apply a specified amount of damage with a specified warhead to the object it is
* attached to after a specified number of seconds has elapsed.
*
* \note
*   Although the script name implies it is purely for destroying objects it can also be used to
*   apply a fixed amount of damage with a particular warhead to an object if the specified amount of
*   damage is insufficient to kill the object.
*
* \param Time
*   Number of seconds to wait before triggering
* \param TimerNum
*   Internal ID to use for the timer to avoid clashing with any other timers on the same object
* \param Amount
*   Amount of damage to apply to the object the script is attached to
* \param Warhead
*   Warhead to use when applying damage, typically <b>Death</b> is a good choice here
*/
class JFW_Timer_Destroy_Object : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Creates an explosion when an object is killed
* \author jonwil
*
* When the object this script is attached to is killed it will create an explosion at the location
* it was killed.
*
* \param Explosion
*   Preset name of the explosion to be created
*
* \note
*   The object the script is attached to must be killed through damage applied to it for this script
*   to work. If the object is simply destroyed (such as by JFW_Destroy_Self_Timer) no explosion will
*   be created.
*/
class JFW_Blow_Up_On_Death : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Grants extra points when an object is killed
* \author jonwil
*
* When the object this script is attached to is killed by the specified team it will grant points to
* the killer and, optionally, every other player on the killers team.
* 
* \param Player_Type
*   Team the killer must be on to recieve points (0 = Nod, 1 = GDI, 2 = Either)
* \param Points
*   Number of points to give when this object is killed
* \param Whole_Team
*   0 to grant points to the killer only, 1 to give points to their whole team
*/
class JFW_Give_Points_On_Death : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

// -------------------------------------------------------------------------------------------------

class JFW_Armour_Regen : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Health_Regen : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Death_Destroy_Object : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_Spawn_Object_Death_Enable : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	public: void Register_Auto_Save_Variables();
};

class JFW_Spawn_Object_Death_Random : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_Spawn_Object_Death_Enable_Random : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	public: void Register_Auto_Save_Variables();
};

class JFW_Engineer_Target : public ScriptImpClass {
	float health;
	float shieldstrength;
	int repairid;
	public: void Register_Auto_Save_Variables();
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Destroyed(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Engineer_Repair : public ScriptImpClass {
	bool active;
	int repairid;
	int targetid;
	int priority;
	public: void Register_Auto_Save_Variables();
	void Created(GameObject *obj);
	void Sound_Heard(GameObject *obj,const CombatSound & sound);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
};

class JFW_Invulnerability_Timer : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Death_Destroy_Object_Delay : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Set_Health_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Add_Health_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Set_Max_Health_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Add_Max_Health_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Set_Shield_Strength_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Add_Shield_Strength_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Set_Max_Shield_Strength_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Add_Max_Shield_Strength_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Set_Health_On_Custom_Sender : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Add_Health_On_Custom_Sender : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Set_Max_Health_On_Custom_Sender : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Add_Max_Health_On_Custom_Sender : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Set_Shield_Strength_On_Custom_Sender : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Add_Shield_Strength_On_Custom_Sender : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Set_Max_Shield_Strength_On_Custom_Sender : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Add_Max_Shield_Strength_On_Custom_Sender : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Regenerate_Health_Conditional : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Building_Damage : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Building_Damage_Scale : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Blow_Up_On_Death_Driver : public ScriptImpClass {
	int DriverID;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Random_DriverDeath : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Building_Damage_Percentage : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Animation_Frame_Damage : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	public: void Register_Auto_Save_Variables();
};

class JFW_Damage_Do_Damage : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	public: void Register_Auto_Save_Variables();
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Invulnerable_On_Create : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Destroys an object after a fixed period of time
* \author jonwil
*
* This script will destroy the object it is attached to after a specified number of seconds has
* elapsed.
*
* \warning
*   Because the object is destroyed (by using ScriptCommands::Destroy_Object()) rather than killed
*   by damage the GameObjObserverClass::Killed() callback will not be trigger for any other scripts
*   attached to the object, which may cause unexpected behaviour. If you need to ensure the callback
*   is executed consider using JFW_Timer_Destroy_Object instead.
*
* \param Time
*   Number of seconds to wait before triggering
* \param TimerNum
*   Internal ID to use for the timer to avoid clashing with any other timers on the same object
*/
class JFW_Destroy_Self_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Change_Model_Health : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	public: void Register_Auto_Save_Variables();
};

class JFW_Change_Model_Health2 : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	public: void Register_Auto_Save_Variables();
};

class JFW_Damage_Animation : public ScriptImpClass {
	bool enable;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Spawn_Object_Death_Weapon : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_Spawn_Object_Death_Team : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_Pilot_Repair : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Damage_Occupants_Death : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_Engineer_Target_2 : public ScriptImpClass {
	float health;
	float shieldstrength;
	int repairid;
	public: void Register_Auto_Save_Variables();
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Destroyed(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Engineer_Repair_2 : public ScriptImpClass {
	bool active;
	int repairid;
	int targetid;
	int priority;
	public: void Register_Auto_Save_Variables();
	void Created(GameObject *obj);
	void Sound_Heard(GameObject *obj,const CombatSound & sound);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
};

struct KillMessage {
	unsigned int PresetID;
	unsigned int StringID;
	bool SendDriverCustom;
};

class JFW_Kill_Message_Display : public ScriptImpClass {
	KillMessage *messages;
	int count;
	int KillerTeam;
	unsigned int KillerID;
	unsigned int KillerPlayerID;
	unsigned int KilledID;
	unsigned int KilledPlayerID;
	bool KillerIsMSBot;
	bool KilledIsMSBot;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
public:
	JFW_Kill_Message_Display() {messages = 0;}
	~JFW_Kill_Message_Display();
};

class JFW_Kill_Message : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_C4_Explode : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Beacon_Building : public ScriptImpClass {
	int pos;
	void Created(GameObject *obj);
	static int PowerupPurchaseHook(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data);
public:
	JFW_Beacon_Building() {pos = 0;}
	~JFW_Beacon_Building();
};

class JFW_Spawn_Object_Repair : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float amount);
};
