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

#include "dp88_ar.h"

/*!
* \brief Apocalypse Rising Rocketeer
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \arscript
*
* Attached to an infantry unit, this script registers a keyhook that allows the player to toggle
* flying mode on or off. When flying mode is enabled a flying vehicle preset is spawned and the
* infantry unit is automatically transitioned into it. Pressing the keyhook again will cause the
* infantry unit to exit the vehicle, which will then be destroyed.
*/
class dp88_AR_Rocketeer : public JFW_Key_Hook_Base
{
public:
  dp88_AR_Rocketeer() : m_nSoldierID(0) {};

protected:
  void Created ( GameObject* pObj );
  void Destroyed (GameObject* pObj);
  void Custom ( GameObject* pObj, int type, int param, GameObject* pSender );
  void Timer_Expired ( GameObject* pObj, int number );
  void KeyHook();

  void Toggle_Flight_Mode ( class SoldierGameObj* pSoldier, bool bState );

  int m_nSoldierID;     //!< ID of the infantry unit, since KeyHook doesn't pass an object pointer
  int m_nVehicleID;     //!< ID of the flight vehicle, if it is currently in use
  time_t m_lastDeploy;  //!< Timestamp of the last deploy event (to restrict usage)
  bool m_bCanDrive;     //!< Cached value of SoldierGameObj->Can_Drive_Vehicles()

  /*! \name Cached Script Parameters */
  /*! @{ */
  int m_minWalkTime;
  int m_minFlightTime;
  /*! @} */
};




/*------------------------
Mirage Tank Script
--------------------------*/

/*!
* \brief Apocalypse Rising Mirage Tank
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \arscript
*
* Script for the Mirage Tank in Apocalypse Rising, controls the appearance and disappearance of the
* tree disguise based on whether the unit is moving, firing or taking damage
*
* \param[in] Mirage_Delay
*   The length of time, in seconds, before the mirage disguise becomes active when the tank is not
*   moving, firing or taking damage
*/
class dp88_AR_MirageTank : public ScriptImpClass
{
	void Created( GameObject *obj );
	void Damaged( GameObject *obj, GameObject *damager, float amount );
	void Killed( GameObject *obj, GameObject *killer );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );
	void Timer_Expired( GameObject *obj, int number );

	void setHidden ( GameObject *obj, bool hidden );

	int m_driverID;         //!< ID of our driver
  int m_mirageID;         //!< ID of the mirage disguising us
  int m_controllerID;     //!< ID of the game controller

  /*! \name Cached Script Parameters */
  /*! @{ */
  int m_delay;                //!< Delay before the mirage disguise activates
  time_t m_lastActivity;      //!< The time of the last activity that would interrupt mirage mode (moving, shooting, damage)
  /*! @} */
};




/*------------------------
IFV Scripts
--------------------------*/

class dp88_AR_IFV : public JFW_Key_Hook_Base
{
	void Created ( GameObject* obj );
	void Destroyed ( GameObject *obj );
	void Custom ( GameObject* obj, int type, int param, GameObject* sender );
	void Timer_Expired ( GameObject* obj, int number );
	void KeyHook();

	/* General variables. ID of this object, ID of the pilot, current
	veterancy level. */
	int objectID, pilotID, veterancyLevel;

	/* Flags to indicate deployment status and if it's currently deploying */
	bool isDeployed, isDeploying;

	int deployedAnimFrame;					// Animation frame for deployed mode
	int deployedRookieWeaponPowerupId;		// Powerup ID for rookie deployed weapon
	int deployedVeteranWeaponPowerupId;		// Powerup ID for veteran deployed weapon
	int deployedEliteWeaponPowerupId;		// Powerup ID for elite deployed weapon


	char defaultWeapon[64];				// Original default model
	char currentDefaultWeapon[64];		// Current default model
	char currentWeapon[64];				// Current model

	

	// Deploy / undeploy functions
	void deploy();
	void undeploy();

	bool nullWeaponGranted; // test to see if this works around stupid FDS bug
};


// IFV driver script
class dp88_AR_IFVDriver : public ScriptImpClass
{
	void Custom ( GameObject* obj, int type, int param, GameObject* sender );
};





/*!
* \brief Apocalypse Rising Tesla Coil
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_basedefences
* \arscript
*
* Based on my standard charged turret AI, this adds additional logic to allow the defence to be
* both powered up in the absence of base power and supercharged to deal additional damage by
* switching to a second weapon preset (which should be setup to deal more damage than the normal
* weapon preset).
*
* The Tesla Coil can be powered up and supercharged by shooting it with a specific warhead defined
* in the script parameters. Each time the coil is hit by a weapon of that warhead will add 1 to the
* charge level. Note that each unit can only charge the defence at most once a second and multiple
* units may work together to increase the charge level.
*
* For more information about the charge up logic see the documentation for dp88_AI_ChargedTurret
*
* \note
*   This script does not currently support the use of secondary fire, therefore the parameters
*   relevant to secondary fire are not present.
*
* \param Priority_Infantry
*   Base targetting priority for infantry targets, or 0 to ignore infantry
* \param Splash_Infantry
*   Determines if we should try to damage infantry with splash instead of hitting them directly.
*   This is useful for slow / inaccurate weapons which do splash damage: 1 to enable, 0 to disable
* \param Priority_Light_Vehicle
*   Base targetting priority for light vehicle targets, or 0 to ignore light vehicles
* \param Priority_Heavy_Vehicle
*   Base targetting priority for heavy vehicle targets, or 0 to ignore heavy vehicles
* \param Priority_VTOL
*   Base targetting priority for flying targets, or 0 to ignore flying targets
* \param Min_Attack_Range
*   Minimum distance at which the turret can engage enemies when using primary fire
* \param Max_Attack_Range
*   Maximum distance at which the turret can engage enemies when using primary fire
* \param Animation_Model
*   Optional name of a W3D model file to spawn and attach to the turret upon which the charge
*   animations defined in this script will be applied. This allows the turret itself to be running
*   a different set of animations, such as damage states via dp88_damageAnimation
* \param Animation_Model_Bone
*   If you specify an Animation_Model above then you must also specify the a bone on the parent
*   to which that model should be attached. This allows the animation model to be moved by the
*   animation set executing on the turret.
* \param Animation
*   Name of the animation which contains the frames for the various charge states
* \param Animation_Idle_Start_Frame
*   First frame number for the idle animation, which will loop continually whilst the turret is
*   powered and not currently attacking a target
* \param Animation_Idle_End_Frame
*   Final frame number for the idle animation
* \param Animation_Unpowered_Start_Frame
*   First frame number for the unpowered animation, which will loop continually whilst the turret
*   is in the unpowered state. This is not used if Requires_Power is disabled (see below).
* \param Animation_Unpowered_End_Frame
*   Final frame number for the unpowered animation
* \param Animation_Charge_Start_Frame
*   First frame number for the charging animation, which will be played once each time the turret
*   reloads its weapon. Once this animation completes the turret will fire again, therefore the
*   length of this animation must be greater than the weapon reload time.
* \param Animation_Charge_End_Frame
*   Final frame number for the charging animation
* \param Charge_Sound
*   Optional sound effect to be played each time the turret starts charging up
* \param Charge_Warhead
*   The name of a warhead in armor.ini which can be used to charge up this tesla coil
* \param Charge_Duration
*   How long, in seconds, a charge from a unit lasts
*   will last before it expires.
* \param Charges_Supercharge
*   The charge level required to supercharge the defence, causing it to switch to the supercharged
*   weapon preset. Note that if base power is disabled the number of charges required is this value
*   plus the value of the Charges_Power_On parameter (see below)
* \param Charges_Power_On
*   The charge level required to provide power to the defence when the base power is offline. If
*   this charge level is reached the defence will operate as normal until the charge level drops
*   below the required threshold again
* \param Supercharged_Weapon_Powerup_Preset
*   The name of a powerup which grants the weapon preset to be used when the defence is in
*   supercharged mode
* \param Supercharged_Weapon_Range
*   The maximum attack range to use when the defence is in supercharged mode, this overrides the
*   Max_Attack_Range parameter to allow the defence weapon to target enemies at a greater distance
*   whilst it is using the supercharged weapon
* \param Modifier_Distance
*   Priority modification to apply based on distance to target. Higher values will favour targets
*   which are closer to the turret, good for less accurate weapons
* \param Modifier_Target_Damage
*   Priority modification to apply based on damage a target has already sustained. Higher values
*   will favour targets which have already been damaged in combat, picking them off first
* \param Modifier_Target_Value
*   Priority modification to apply based on the value of the target. Higher values will favour
*   targets with a higher purchase cost, good for hard hitting weapons
* \param Requires_Power
*   Specify whether this turret requires base power to operate: 1 to require power, 0 to ignore
* \param Debug
*   Specify whether to produce a debug logfile about the turrets targetting decisions, this is
*   useful for fine tuning your base priorities and modifiers: 1 to enable, 0 to disable
* \param Detects_Stealth
*   Determine whether this turret can detect stealthed enemies or not: 1 to enable, 0 to disable
*
* \note
*   Vehicles are always classified as light vehicles unless they have the dp88_AI_heavyVehicleMarker
*   script attached to them.
*
* \warning
*   Never leave the Debug parameter enabled when releasing your mod, it will clog up everyones
*   machines with useless text files everywhere... people usually don't like that :D
*/
class dp88_AR_Tesla_Coil : public dp88_AI_ChargedTurret
{
  /* -----
  Events
  ----- */

  void Created ( GameObject *obj );
  void Damaged ( GameObject *obj, GameObject *damager, float amount );
  void Timer_Expired ( GameObject *obj, int number );

  // Custom AI initialisation script overloads
  virtual void Init( GameObject *obj );
  virtual void loadSettings( GameObject *obj, bool loadSecondaryFireSettings = true, bool loadBuildingTargetSettings = false );

  // Overload the checkPowerState function to return true when sufficient
  // charge level exists to provide power
  virtual bool checkPowerState(GameObject* obj);

  private:
  /* -----
  Functions
  ----- */

  void setSuperchargedState(GameObject* obj, bool state);

  /* -----
  Variables
  ----- */

  int m_chargeLevel;
  bool m_isSupercharged;
  StringClass m_defaultWeapon;
  int m_defaultWeaponRange;
  StringClass m_superchargedWeapon;
  int m_superchargedWeaponRange;

  // Settings
  unsigned int m_chargeWarheadID;
  int m_chargeDuration;
  int m_chargesPowerOn;
  int m_chargesSupercharge;
};
