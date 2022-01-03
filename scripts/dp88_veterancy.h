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

#include "scripts.h"
#include "engine.h"

// -------------------------------------------------------------------------------------------------

struct dp88_Veterancy_Settings
{
  public:
    dp88_Veterancy_Settings();

    void Reset();

    static dp88_Veterancy_Settings& GetInstance()
    {
      return ms_instance;
    }

    float valueMultiplier[3];    //!< Value multiplier for veteran and elite levels
    float carryOverPercentI;     //!< Percentage of infantry points carried over when switching infantry
    float carryOverPercentV;     //!< Percentage of vehicle points carried over when switching infantry
    int boostPrice;              //!< Price to buy a single veterancy point, or 0 if boost isn't enabled

private:
    static dp88_Veterancy_Settings ms_instance;
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Veterancy - Controller
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_veterancy
*
* An optional controller script for the veterancy system, if an instance of this object exists on a
* map it can alter the behaviour of dp88_veterancyUnit and other scripts in the veterancy family. If
* there is no instance of this script on a map the other scripts will use the default values instead.
*
* \param[in] Veteran_Value_Multiplier_Percent
*   The percentage increase applied to the veterancy value of a unit at the veteran level, causing
*   enemies to earn more points when attacking the unit. Default: 10
* \param[in] Elite_Value_Multiplier_Percent
*   The percentage increase applied to the veterancy value of a unit at the elite level, causing
*   enemies to earn more points when attacking the unit. Does not stack with the veteran level
*   increase. Default: 25
* \param[in] Change_Character_Carry_Over_Percent_Inf
*   The percentage of infantry veterancy points that are carried over when a player buys a new
*   infantry unit at a purchase terminal. Default: 60
* \param[in] Change_Character_Carry_Over_Percent_Veh
*   The percentage of vehicle veterancy points that are carried over when a player buys a new
*   infantry unit at a purchase terminal. Default: 60
* \param[in] Boost_Price
*   The price to buy 1 veterancy point in the boost system, which adds an additional keyhook for
*   players to spend money to reach the next level. Boost is disabled if this is 0. Default: 0
*/
class dp88_Veterancy_Controller : public ScriptImpClass
{
  public:
    void Created(GameObject* obj);
    void Destroyed(GameObject* obj);
};

// -------------------------------------------------------------------------------------------------

/* Script to handle veterancy on any type of object, either player or AI controlled */
class dp88_veterancyUnit : public MultiKeyHookScriptImpClass
{
  public:
    void Created ( GameObject *obj );
    void Damaged( GameObject *obj, GameObject *damager, float amount );
    void Killed ( GameObject *obj, GameObject *killer );
    void Destroyed ( GameObject *obj );
    void Detach(GameObject* obj);
    void Custom ( GameObject* obj, int type, int param, GameObject* sender );
    void Timer_Expired( GameObject *obj, int number );
    void KeyHook(const char* logicalKey);


    // Recieve veterancy points
    void recieveVeterancyPoints ( float points );

  private:
    /****************
    Variables
    *****************/

    // Our data
    int objectId;
    int currentLevel;
    float infantryVeterancyPoints, vehicleVeterancyPoints;
    int infantryVeteranRequirement, infantryEliteRequirement;
    int vehicleVeteranRequirement, vehicleEliteRequirement;
    int chevronObjId, promotionChevronObjId;
    bool chevronVisible;

    /* Flags to mark whether we have upgraded weapons for veteran or rookie levels (saves lots of
    string comparisions and preset validation later on) */
    bool hasVeteranWeaponPowerup, hasEliteWeaponPowerup;

    /* Original values for weapons / skin / armour type (req. for vehicles only) */
    char rookieWeapon[128], rookieSkinType[128], rookieShieldType[128];

    // ID for vehicle pilot (vehicles only)
    int pilotId;

    // Marker to indicate if we are dead or not, sometimes the promotion
    // functions can get called after the unit is dead, so we need a quick
    // and easy way to check this and abort promotion if we are dead
    bool deregistered;


    // Static arrays of pointers to all veterancy units
    static dp88_veterancyUnit* playerData[128];
    static dp88_veterancyUnit* AIUnitData[256];

    // Logical key hooks
    static const char ShowVeterancyPointsHookName[];
    static const char BoostVeterancyHookName[];

    /****************
    Functions
    *****************/

    /*! Called when this unit has been killed or destroyed to deregister from the static array */
    void Deregister(GameObject* obj);

    /*! Grant x veterancy points to the specified unit */
    void grantVeterancyPoints ( GameObject* obj, float points );

    /*! Get a pointer to the dp88_veterancyUnit class object for the specified GameObject */
    dp88_veterancyUnit* getVeterancyData ( GameObject* obj );

    /*! Handle promotion events */
    void promoteToVeteran();
    void promoteToElite();

    /*! Handle demotion event - Mainly for vehicles when their drivers get out */
    void demoteToRookie();

    /*! Chevron scripts */
    void createChevrons();
    void clearChevrons();

    /****************
    Struct to carry-over points when purchasing new infantry (this is required since Destroyed()
    is now called correctly in this instance so we can't rely on that mechanism for carry-over)
    *****************/

    struct PointsCarryOverData
    {
      long playerId;
      unsigned int gameTime;
      float infantryVeterancyPoints, vehicleVeterancyPoints;

      PointsCarryOverData() : playerId(-1) {}
    };
    
    // Only need one instance, carry-over should happen immediately after Destroyed() is called
    static PointsCarryOverData carryOverData;
};

// -------------------------------------------------------------------------------------------------

/*! Script to grant veterancy points to whatever it is attached to, also works for crates */
class dp88_veterancyGrantPoints : public ScriptImpClass
{
  void Created ( GameObject *obj );
  void Custom ( GameObject *obj, int type, int param, GameObject *sender );
};

// -------------------------------------------------------------------------------------------------

// Script to link veterancy points of two objects together
class dp88_linkVetPoints : public ScriptImpClass
{
  void Created ( GameObject *obj );
  void Timer_Expired ( GameObject *obj, int number );
  void equalisePoints( GameObject* obj );

  int parentObjID;
  float lastInfantryPoints;
  float lastVehiclePoints;
};

// -------------------------------------------------------------------------------------------------

// Script to grant extra health / armour upon promotion
class dp88_veterancyPromotionHealthArmourIncrease : public ScriptImpClass
{
  void Created( GameObject *obj );
  void Custom( GameObject *obj, int type, int param, GameObject *sender );

private:
  int m_veterancyLevel;
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Veterancy - Grant Weapon
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* This is a companion script to dp88_veterancyUnit which grants an additional weapon to a unit upon
* spawning and replaces the weapon with a (presumably) more powerful version when the unit gets
* promoted. Alternatively it can grant an additional weapon to a unit when it reaches veteran or
* elite level, without granting anything to a rookie unit.
*
* Similar to the m00_GrantPowerup_Created stock script, with the additional veterancy logic added.
*
* \note
*   Whilst this script is designed to be used for granting additional weapons there's no technical
*   reason it can't also be used to grant other powerup types. If non-weapon powerups are granted
*   their effects will not be removed when the next veterancy level is reached.
*
* \param powerupPreset
*   The name of a powerup preset to grant to the unit when it is spawned at rookie level
* \param powerupPreset_veteran
*   The name of a powerup preset to grant to the unit when it is promoted to veteran level
* \param powerupPreset_elite
*   The name of a powerup preset to grant to the unit when it is promoted to elite level
*/
class dp88_veterancyGrantPowerup : public ScriptImpClass
{
  char weaponName[64];
  char oldWeapon[64];

  void Created( GameObject *obj );
  void Custom( GameObject *obj, int type, int param, GameObject *sender );

private:
  int veterancyLevel;
};