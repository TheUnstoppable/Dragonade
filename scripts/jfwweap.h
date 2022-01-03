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
class JFW_Nod_Turret : public ScriptImpClass {
	unsigned int id1;
	unsigned int id2;
	unsigned int id3;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Nod_Obelisk_CnC : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Obelisk_Weapon_CnC : public ScriptImpClass {
	bool EnemySeen;
	int attackid;
	int effectid;
	float range;
	bool attack;
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Advanced_Guard_Tower : public ScriptImpClass {
	int missileid;
	int gun1id;
	int gun2id;
	int gun3id;
	int gun4id;
	bool enable;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Advanced_Guard_Tower_2 : public ScriptImpClass {
	int missileid;
	int gun1id;
	int gun2id;
	int gun3id;
	int gun4id;
	bool enable;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Advanced_Guard_Tower_Missile : public ScriptImpClass {
	bool firing;
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Advanced_Guard_Tower_Gun : public ScriptImpClass {
	int id;
	bool disable;
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Guard_Duty_Helper : public ScriptImpClass {
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
};

class JFW_Guard_Duty : public ScriptImpClass {
	int helperid;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Hunt_Attack : public ScriptImpClass {
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
};

class JFW_Hunt_Blow_Up : public ScriptImpClass {
	bool firsttime;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
	public: void Register_Auto_Save_Variables();
};

/*!
* \brief Remove All Weapons On Zone Entry
* \author jonwil
* \ingroup scripts_weaponmanagement
*
* Removes all weapons from an object when it enters a zone this script is attached to
*/
class JFW_Clear_Weapons : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

/*!
* \brief Grant Weapon On Zone Entry
* \author jonwil
* \ingroup scripts_weaponmanagement
*
* Grants a weapon powerup to an object when it enters a zone this script is attached to. It can
* optionally be restricted to objects on a specific team.
*
* \note
*   Technically there is no reason this script cannot also be used to grant non-weapon powerups
*
* \param Weapon_Name
*   The name of a powerup preset which grants the weapon to be given to the object
* \param Player_Type
*   The player type (or, team) this script will trigger for. 0 = Nod, 1 = GDI, 2 = All
*/
class JFW_Zone_Weapon : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Weapon_Change_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Weapon_Change_On_Custom_Sender : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Weapon_Change_On_Custom_Multiple : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Weapon_Change_On_Custom_Multiple_Sender : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

/*!
* \brief Remove All Weapons On Custom
* \author jonwil
* \ingroup scripts_weaponmanagement
*
* Removes all weapons from the object the script is attached to when it recieves a specified custom
* message.
*
* \param Message
*   The ID of the custom message which will trigger this script
* \param Weapon
*   Redundant parameter
*/
class JFW_Clear_Weapons_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Vehicle_Weapon_Preset : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Vehicle_Weapon_Weapon : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

/*!
* \brief Remove All Weapons
* \author jonwil
* \ingroup scripts_weaponmanagement
*
* Removes all weapons from the object the script is attached to, this can either be done when the
* object spawns by adding this script to the preset or at any point during the game to remove all
* weapons the object has at that time.
*/
class JFW_Clear_Weapons_Create : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Fire_Weapon_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

/*!
* \brief Weapon Switch on Keyhook - Vehicle
* \author jonwil
* \ingroup scripts_weaponmanagement
*
* This script allows the driver of a vehicle to swap the weapon being used by the vehicle for an
* alternative one by pressing a key. Pressing the key again will restore the original weapon.
*
* \param Powerup
*   The name of a powerup preset which grants the alternative weapon
* \param Keyhook
*   The name of the keyhook binding to trigger the weapon swap
* \param SwitchTime
*   The amount of time the weapon swap takes, in seconds. During this time the player will be able
*   to fire either weapon.
* \param OldSound
*   The name of a sound preset to play when switching to the original vehicle weapon
* \param NewSound
*   The name of a sound preset to play when switching to the alternative weapon
*/
class JFW_Vehicle_Weapon_Switcher : public JFW_Key_Hook_Base {
public:
	bool switching;
	bool newweap;
	int pilotID;
	const char *new_weapon;
	const char *old_weapon;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	void KeyHook();
	JFW_Vehicle_Weapon_Switcher() {old_weapon = 0;}
	~JFW_Vehicle_Weapon_Switcher() {SAFE_DELETE_ARRAY(old_weapon);}
};

/*!
* \brief Weapon Switch on Keyhook - Infantry Character
* \author jonwil
* \ingroup scripts_weaponmanagement
*
* This script allows a player to swap one of their weapons for an alternative weapon by pressing a
* key. The original weapon will be removed from the player and they will be granted a powerup with
* the alternative weapon. Pressing the key again will remove the alternative weapon from them and
* grant them a powerup containing their original weapon.
*
* \note
*   This script will <b>not</b> grant the original weapon to the player, it should be given to them
*   either as their default weapon or by another script such as M00_GrantPowerup_Created.\n
*   \n
*   Because the weapons are removed from the player when swapping from one to the other the ammo
*   count is not preserved, so after swapping a weapon will have whatever amount of ammunition is
*   specified in the powerup that granted it.\n
*   \n
*   After the weapon switch has occurred the newly granted weapon will be selected as the active
*   weapon, regardless of whether the removed weapon was active when the swap key was pressed.
*
* \param SwitchTime
*   The amount of time the weapon swap takes, in seconds. During this time the player will not have
*   either their original weapon or the alternative weapon available to them.
* \param WeaponPowerup1
*   The name of a powerup preset which grants the players original weapon
* \param WeaponPowerup2
*   The name of a powerup preset which grants the alternative weapon
* \param Keyhook
*   The name of the keyhook binding to trigger the weapon swap
*/
class JFW_Char_Weapon_Switcher : public JFW_Key_Hook_Base {
	time_t LastSwitch;
	bool Weapon2;
	bool InTimer;
	void Created( GameObject *obj );
	void KeyHook();
	void Timer_Expired( GameObject *obj,int number );
};
