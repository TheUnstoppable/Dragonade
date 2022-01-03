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

#include "dp88_customAI.h"
#include "dp88_custom_timer_defines.h"
#include "dp88_misc.h"

// -------------------------------------------------------------------------------------------------

/*!
* \brief Apocalypse Rising Game Controller
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \arscript
*
* The game controller script for Apocalypse Rising, this should be attached to an editor only object
* on every map. It is currently responsible for the following functionality;
*
* - Map specific Mirage Tank disguises, up to 3 variants per map
* - Random country selection for each team from available countries on the current map
*
* In future the country selection code will have additional logic to spawn country-specific objects
* such as the French Grand Cannon at predetermined markers on the map or enable country specific
* units for purchase. Currently the country selection has no actual effect on the game.
*
* \note
*   If no country choices are available for one or both teams the controller will choose from all
*   available countries at random as a failsafe mechanism.
*
* \param enableCountry_Russia
*   Determines if the controller can select Russia as the Soviet country on this map. 1 to enable,
*   0 to disable.
* \param enableCountry_Cuba
*   Determines if the controller can select Cuba as the Soviet country on this map. 1 to enable,
*   0 to disable.
* \param enableCountry_Iraq
*   Determines if the controller can select Iraq as the Soviet country on this map. 1 to enable,
*   0 to disable.
* \param enableCountry_Libya
*   Determines if the controller can select Libya as the Soviet country on this map. 1 to enable,
*   0 to disable.
* \param enableCountry_America
*   Determines if the controller can select America as the Allied country on this map. 1 to enable,
*   0 to disable.
* \param enableCountry_France
*   Determines if the controller can select France as the Allied country on this map. 1 to enable,
*   0 to disable.
* \param enableCountry_Germany
*   Determines if the controller can select Germany as the Allied country on this map. 1 to enable,
*   0 to disable.
* \param enableCountry_GreatBritain
*   Determines if the controller can select Great Britain as the Allied country on this map. 1 to
*   enable, 0 to disable.
* \param enableCountry_Korea
*   Determines if the controller can select Korea as the Allied country on this map. 1 to enable,
*   0 to disable.
* \param MirageTank_disguisePreset_1
*   The name of the first disguise preset for the Mirage Tank on this map.
* \param MirageTank_disguisePreset_2
*   Optional. The name of the second disguise preset for the Mirage Tank on this map.
* \param MirageTank_disguisePreset_3
*   Optional. The name of the third disguise preset for the Mirage Tank on this map.
* \param Camouflage
*   The ID of the camouflage variant to use for this map
*/

class dp88_AR_GameController : public dp88_Camo_Controller
{
public:
  // Events
  void Created( GameObject *obj );
  void Custom( GameObject *obj, int type, int param, GameObject *sender );
  void Timer_Expired( GameObject *obj, int number );
  void Destroyed( GameObject *obj );

  dp88_AR_GameController();
  ~dp88_AR_GameController();

protected:
  bool created;

  short team0_countryID;    // Country ID for Soviets
  short team1_countryID;    // Country ID for Allies

public:
  // Mirage Tank
  char* mirageTank_disguisePresets[3];
};

// -------------------------------------------------------------------------------------------------

/*!
* \deprecated - Early AR development script, keeping it temporarily for terror drone code
*/
class dp88_AR_Vehicle : public ScriptImpClass
{
  // Variables
  int pilotID;						// Pilots GameObject ID
  int attackingTerrorDroneID;			// ID of any Terror Drones attacking this vehicle
  bool dead;							// Set to true when we die

  // Events
  void Custom(GameObject *obj,int type,int param,GameObject *sender);
  void Created(GameObject *obj);
  void Timer_Expired( GameObject *obj,int number );
  void Killed(GameObject *obj, GameObject *killer);
  void Damaged(GameObject *obj, GameObject *damager, float amount);
  void Animation_Complete ( GameObject *obj, const char *animation_name );
};




/*------------------------
Deployable Infantry script
  Allows an infantry unit to 'deploy', which involves switching to
  a more powerful weapon and creating a defensive structure of some
  kind at the players location.
--------------------------*/

class dp88_AR_Deployable_Infantry : public JFW_Key_Hook_Base
{
public:
	// Events
	void Created( GameObject *obj );
	void Killed( GameObject *obj, GameObject *killer );
	void Destroyed ( GameObject* obj );
	void KeyHook();
	void Custom( GameObject *obj, int type, int param, GameObject *sender );
	void Timer_Expired( GameObject *obj,int number );		// TEMPORARY

	// Get armour type to set for the given veterancy level
	const char* GetArmourType ( int vetLevel );

	// Get weapon powerup to grant for the given veterancy level
	 const char* GetWeaponPowerup ( int vetLevel );

protected:
  void Deploy(GameObject* obj);

  void Undeploy(GameObject* obj);

	// Variables
	int objectID;
	bool deployed;
	char* undeployedWeapon;
	int deployedObjectId;

	int cannotDeployStringId;
	int cannotDeploySoundId;
	time_t lastDeploy;	// Timestamp of last deploy keyhook event (prevent constant spamming)

	/* Current veterancy level */
	int currentVetLevel;

	/* Flags to mark which veterancy levels have deployed weapon powerups */
	bool hasRookieWeaponPowerup, hasVeteranWeaponPowerup, hasEliteWeaponPowerup;

	/* Original values for Skin & Armour types */
	char undeployedSkinType[128], undeployedArmourType[128];

  /*! Cached value of SoldierGameObj::Can_Drive_Vehicles() prior to deploying */
  bool m_bCanDrive;
};




/*------------------------
Chrono Legionnaire
--------------------------*/

class dp88_AR_CLEG : public ScriptImpClass {
	bool phasing;
	bool phasingBack;
	int zPosDropObj;
	short timeRemaining;
	int dropObjID;

	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float amount);
	void Killed(GameObject *obj, GameObject *killer);
	void Timer_Expired( GameObject *obj,int number );
};

class dp88_AR_CLEG_target : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float amount);
	void Killed(GameObject *obj, GameObject *killer);
	void Timer_Expired( GameObject *obj,int number );

	int currentResistance;
	int lastResistanceCheck;
	int effectObjectId;
};




// -------------------------------------------------------------------------------------------------

/*!
* \brief Ore Miner
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* \todo Update this documentation to remove references to AR specific logic - now a general purpose
*       script for use with dp88_Ore_Field and dp88_Ore_Dump_Zone
*
* This class contains the basic logic for the ore miners used in Apocalypse Rising and supports both
* AI and player controlled miners. A miner can collect ore from any ore fields defined by a script
* zone with the dp88_AR_Ore_Field_Zone script attached to it, which determines the value of each ore
* 'unit' in that field, thus implicitly supporting gem fields with the same scripts.
*
* The miner can hold up to a configurable number of ore units, each of which takes a configurable
* amount of time to mine from the ore field. Mining is achieved by remaining inside an ore field
* whilst ore units are 'collected'.
*
* Miners can deposit ore at any time as long as they have at least one unit mined, with the total
* value of the deposit being based on the number of units collected and the value of each unit. Ore
* deposits are achieved by entering a script zone with the dp88_Ore_Dump_Zone attached to it
* with the team parameter matching that of the miner.
*
* When the AI is enabled the miner will use the pathfind grid to locate the nearest ore field and
* drive to it to begin mining. Once it has collected a full load of ore it will locate the nearest
* ore deposit zone and drive there to unload it's ore. This will be repeated ad-infinitum until the
* miner is destroyed. If you have problems with the miner getting stuck on terrain or other objects
* you should place pathfind blockers around the problematic areas and re-generate pathfind data to
* make it route around those locations.
*
* \param Use_AI
*   Specifies that this miner should use its AI to mine autonomously. 1 to enable AI, 0 otherwise
* \param Ore_Capacity
*   Maximum number of ore units this miner can store
* \param Ore_Mining_Time
*   The amount of time, in seconds, it takes to mine one ore unit from an ore field
* \param Ore_Dump_Time
*   The amount of time, in seconds, it takes to unload all ore units at an ore deposit zone
* \param AI_Init_Delay
*   If the miner AI is enabled this specifies the initial delay before starting the first AI action,
*   which is required to avoid the original Renegade Harvester AI overriding this one.
* \param Dump_Animation
*   An optional animation to be played when depositing ore at a refinery
* \param Dump_Sound
*   An optional sound effect to be played when depositing ore at a refinery
* \param Mining_Animation
*   An optional animation to be played in a loop whilst mining in an ore field
* \param Mining_Sound
*   An optional sound effect to be played each time the ore load level increases
* \param Idle_Animation
*   An optional animation to be played in a loop when the miner is not mining or docked
* \param Resource_Name
*   The name of hte resource field (defaults to ore if none is specified)
*
* \note
*   Because this script is designed to act as a base class for both types of AR miner it is not
*   named after either teams miner and does not appear as an available script in LevelEdit. However,
*   because there is no additional logic required for the Soviet War Miner there isn't actually a
*   derived script for that at this time, thus the dp88_AR_War_Miner script in LevelEdit is really
*   an instance of this base class script.
*/
class dp88_Ore_Miner : public JFW_Key_Hook_Base   // Inherit from keyhook base for chrono miner
{
protected:
  int m_aiState;          //!< Current AI state

  int m_oreMined;         //!< Current number of mined ore loads onboard
  int m_oreValue;         //!< Cumulative value of all ore loads onboard

  int m_oreFieldId;       //!< ID of the ore field we are currently mining ore from
  int m_oreFieldRand;     //!< Used to prevent glitching by entering/exiting an ore field rapidly. 0 = not in ore field

  /*! \name Cached Script Parameters */
  /*! @{ */
  bool m_bUseAI;          //!< Is AI enabled?
  int m_oreCapacity;      //!< Maximum number of mined ore units onboard
  float m_oreMiningTime;  //!< Time required to mine one ore unit
  float m_oreDumpTime;    //!< Time required to dump ore at a refinery
  const char* m_resourceName; //!< Resource name for strings>

  const char* m_animations[3];
  const char* m_animSounds[3];
  /*! @} */

public:
  // Default event handlers for the AI miners
  virtual void Created ( GameObject *obj );
  virtual void Custom ( GameObject *obj, int type, int param, GameObject *sender );
  virtual void Action_Complete( GameObject *obj, int action_id, ActionCompleteReason complete_reason );
  virtual	void KeyHook() {};

protected:
  // These functions implement standard drive to ore field, harvest some ore, return to refinery,
  // dock and unload ore functionlity for an AI miner. They can be overloaded to provide custom
  // functionality in derived classes.
  virtual void DriveToOreField ( GameObject *obj );
  virtual void EnteredOreField ( GameObject *obj, GameObject* oreField );
  virtual void ExitedOreField ( GameObject *obj, GameObject* oreField );
  virtual void ReturnToRefinery ( GameObject *obj );
  virtual void DockAtRefinery ( GameObject *obj );
  virtual void DockedAtRefinery ( GameObject *obj );
  virtual void UndockedFromRefinery ( GameObject *obj );

  // AI states for the miner
  enum MINER_AISTATES
  {
    MINER_AISTATE_IDLE,
    MINER_AISTATE_SEARCH_FOR_ORE,
    MINER_AISTATE_DRIVE_TO_ORE,
    MINER_AISTATE_COLLECTING_ORE,
    MINER_AISTATE_RETURN_TO_REFINERY,
    MINER_AISTATE_DOCK_AT_REFINERY,
    MINER_AISTATE_UNLOADING_ORE
  };

  enum MINER_ACTIONIDS
  {
    MINER_ACTIONID_DRIVE_TO_ORE,
    MINER_ACTIONID_RETURN_TO_REFINERY,
    MINER_ACTIONID_DOCK_AT_REFINERY
  };

  enum MINER_ANIMID
  {
    MINER_ANIM_IDLE,
    MINER_ANIM_MINING,
    MINER_ANIM_DUMPING
  };

  void UpdateAnimation ( GameObject* pObj, MINER_ANIMID animId );
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief AR Chrono Miner
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \arscript
*
* \todo Write Documentation
*/
class dp88_AR_Chrono_Miner : public dp88_Ore_Miner
{
protected:
  int objectId;     //!< ID of the object we are running on, since Keyhook() does not include the GameObject...
  int driverId;     //!< ID of our current driver, or NULL

public:
  void Created ( GameObject *obj );
  void Damaged ( GameObject *obj, GameObject *damager, float amount );
  void Custom ( GameObject *obj, int type, int param, GameObject *sender );
  void KeyHook();

private:
  void ReturnToRefinery ( GameObject *obj );
  bool Start_Chronoshift( GameObject *obj );
  void Do_Chronoshift( GameObject *obj, int target_zone_id );
  void Complete_Chronoshift( GameObject *obj );

  bool CanChronoshiftToLocation ( GameObject* obj, Vector3& location );

  enum CMINER_AISTATES
  {
    CMINER_AISTATE_CHRONOSHIFTING = MINER_AISTATE_UNLOADING_ORE+1
  };
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief AR Chrono Miner Chronoshift Zone
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \arscript
*
* \todo Write Documentation
*/
class dp88_AR_Chrono_Miner_Chronozone : public ScriptImpClass
{
  void Created( GameObject *obj );

public:
  // Set by dp88_AR_Chrono_Miner when it is shifting to this zone, used
  // to prevent multiple miners trying to shift to the same place
  int chronominer_id;
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Ore Field
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* Attach this script to an object which represents an ore (or gem) field on the map, or to a Daves
* arrow if you do not want animation support for growing / shrinking the field. A script zone will
* be created at the center of this object (it's local 0,0,0 position) which defines the area miners
* must enter to be considered 'inside' the ore field. This zone can grow and shrink with the ore
* field animation if desired.
*
* If you attach this to a script zone directly, it will use that zone instead of spawning a new zone.
*
* Used on it's own this script can either create an ore field with infinite capacity or one which is
* gradually depleted until it is empty. If you want the ore field to re-grow you must add one or
* more objects with the dp88_Ore_Extractor script attached.
*
* To visually show the current size of an ore field you can use an animation on the object this
* script is attached to which will animate from the full frame to an empty frame as the field is
* depleted. If an ore extractor is present it will also animate in reverse as the field is refilled.
*
* \pre
*   A preset with the name "Script_Zone_All" must exist to allow the miner zone to be created. This
*   preset should have CheckStarsOnly disabled to allow AI miners to use this ore field
*
* \param Ore_Value
*   The value of each unit of 'ore' mined from this field. Multiply this by the capacity of a miner
*   to get the total credits per dump from a fully loaded miner.
* \param Ore_Capacity
*   The maximum capacity of this ore field, in units. Set this to 0 to create an infinite ore field
*   which never shrinks or grows.
* \param Ore_Units
*   If Ore_Capacity is not 0 this defines the number of units in the ore field when it is created
* \param Animation_Name
*   The name of an animation containing the frames to show ore capacity
* \param Animation_Full_Frame
*   The frame number for the ore field when it is at maximum capacity
* \param Animation_Empty_Frame
*   The frame number for the ore field when it is empty
* \param Zone_Size
*   The size of the zone which miners must enter to mine ore from this field. This size is used when
*   the field is at maximum capacity. When the zone is empty the zone will despawn.
* \param Zone_Anim_Step_X
*   How much to shrink the miner zone in the X axis by for each frame in the animation
* \param Zone_Anim_Step_Y
*   How much to shrink the miner zone in the Y axis by for each frame in the animation
* \param AI_Ignore
*   The name of the resource field (defaults to ore if none is specified)
*/
class dp88_Ore_Field : public ScriptImpClass
{
public:
  dp88_Ore_Field() : m_pZoneObserver(NULL) {}
  void Created ( GameObject* pObj );
  void Detach ( GameObject* pObj );
  void Entered ( GameObject* pZoneObj, GameObject* pEnterer );
  void Exited ( GameObject* pZoneObj, GameObject* pExiter );

  /*!
  * Add one or more units of ore to this field, if there is additional capacity available
  *
  * \param[in] nUnits
  *   Number of ore units to be added
  */
  void AddOre ( unsigned int nUnits );

  /*!
  * Attempt to remove one or more units of ore from this field, if they are available
  *
  * \param[in] nUnits
  *   Number of ore units to try and remove
  *
  * \return Number of units actually removed, which will be <= nUnits
  */
  unsigned int RemoveOre ( unsigned int nUnits = 1 );

  /*!
  * Check the number of ore units currently available in this field
  *
  * \return Number of ore units available in this field, or 0 if this field has infinite capacity
  */
  unsigned int NumOreUnits() { return m_nOreUnits; }

  /*! Get the value of the ore in this field */
  unsigned int GetOreValue() { return m_oreValue; }

  /*! Check if this ore field is infinite */
  bool IsInfinite() { return m_oreCapacity == 0; }

  /*! Check if the AI miners can use this field */
  bool IsSuitableForAI() { return !m_bAiIgnore; }


protected:
  void UpdateAnimationFrame();
  void UpdateAnimationFrame( GameObject* pObj );


  unsigned int m_myObjId;     //!< My own GameObject ID, used by the zone observer callbacks
  unsigned int m_nOreUnits;
  int m_minerZoneId;

  /*! \name Cached Script Parameters */
  /*! @{ */
  unsigned int m_oreValue;
  unsigned int m_oreCapacity;

  const char* m_strAnimation;
  unsigned int m_nAnimationFullFrame;
  unsigned int m_nAnimationEmptyFrame;

  Vector3 m_zoneSizeFull;
  float m_zoneStepX;
  float m_zoneStepY;

  bool m_bAiIgnore;
  /*! @} */


  /*! Class to observe the script zone - don't need an extra script for this */
  class dp88_Ore_Field_Observer : public GameObjObserverClass
  {
  public:
    dp88_Ore_Field_Observer(dp88_Ore_Field* pParent)
      { m_pParent = pParent; }

  protected:
    dp88_Ore_Field* m_pParent;

    // These are observer events we actually use
    void Entered( GameObject* pObj, GameObject* pEnterer )  { m_pParent->Entered(pObj, pEnterer); }
    void Exited( GameObject* pObj, GameObject* pExiter )    { m_pParent->Exited(pObj, pExiter); }

  private:
    // These are observer events we don't use but have to provide an implementation for to compile
    void Attach ( GameObject* pObj ) {};
    void Detach ( GameObject* pObj ) {};
    void Animation_Complete ( GameObject *pObj, const char *animation_name ) {};
    void Created(GameObject* pObj) {};
    void Destroyed(GameObject* pObj) {};
    void Killed(GameObject* pObj,GameObject* pKiller) {};
    void Damaged(GameObject* pObj,GameObject* pDamager,float amount) {};
    void Custom(GameObject* pObj,int type,int param,GameObject* pSender) {};
    void Sound_Heard(GameObject* pObj,const CombatSound & sound) {};
    void Enemy_Seen(GameObject* pObj,GameObject* pEnemy) {};
    void Action_Complete(GameObject* pObj,int action_id,ActionCompleteReason complete_reason) {};
    void Timer_Expired(GameObject* pObj,int number) {};
    void Poked(GameObject* pObj,GameObject* pPoker) {};


    // We also need to provide an implementation for Get_Name to compile
    const char* Get_Name() { return "dp88_Ore_Field_Observer"; }
  };

  dp88_Ore_Field_Observer* m_pZoneObserver;
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Ore Extractor
* \author Daniel Paul (danpaul88@yahoo.co.uk)

* Attach this script to an object which is inside an ore field created with dp88_Ore_Field to allow
* the object to refill the ore field.
*
* \note
*   If you do not make the object this is attached to indestructable players can strategically
*   destroy ore extractors close to the enemy base to force their miners to venture further out into
*   the map to find ore. You should have at least one indestructable extractor or infinite ore field
*   on a map if you want to guarantee credit income for the entire match...
*
* \param Ore_Units
*   The number of ore units produced per extraction
* \param Extraction_Interval
*   The number of seconds between each ore extraction, not including the time required to play the
*   extraction animation if one is used
* \param Extraction_Animation
*   An animation to be played when ore extraction is taking place - the ore will be created once the
*   animation is completed
*/
class dp88_Ore_Extractor : public ScriptImpClass
{
public:
  void Created ( GameObject* pObj );
  void Timer_Expired ( GameObject* pObj, int number );
  void Animation_Complete ( GameObject* pObj, const char* animationName );

protected:
  int m_oreFieldId;

  /*! \name Cached Script Parameters */
  /*! @{ */
  unsigned int m_nOreUnits;
  unsigned int m_interval;
  const char* m_strAnimation;
  /*! @} */
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Ore Dump Zone
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* \todo Write Documentation
*/
class dp88_Ore_Dump_Zone : public ScriptImpClass
{
public:
  void Entered( GameObject *obj, GameObject *enterer );
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Aircraft Landing Zone
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* Attach this script to a zone to designate the zone as an aircraft landing area. When a game object
* enters or exits the zone it will be sent a custom message, which can be used by other scripts to
* determine if the aircraft is currently in a landing area.
*
* \see dp88_Aircraft_LandingZone_Aircraft
*/
class dp88_Aircraft_LandingZone : public ScriptImpClass
{
  void Entered( GameObject *obj, GameObject *enterer );
  void Exited ( GameObject *obj, GameObject *exiter );
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Aircraft Landing Zone Animation
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* Plays an animation on the attached object whenever it enters or leaves an aircraft landing zone,
* designated by the presence of the dp88_Aircraft_LandingZone script. Upon entering a landing zone
* the landing animation will be played and upon exiting a landing zone the same animation will be
* played in reverse. This can be used to create a landing gear animation for aircraft.
*
* Optionally the script can also enforce the pilot only exiting the aircraft whilst inside a landing
* zone if the require_landing_zone parameter is set to 1. This will cause the vehicle and pilot to
* both be killed if the pilot attempts to leave the vehicle outside of a landing zone.
*
* \note
*   This script supports overlapping landing zones and will only play the reverse animation once the
*   aircraft has exited all landing zones.
*
* \param landing_anim_name
*   Name of the animation to play upon entering a landing zone (and reverse upon leaving)
* \param landing_anim_first_frame
*   First frame number of the landing animation to play
* \param landing_anim_last_frame
*   Last frame number of the landing animation to play
* \param require_landing_zone
*   Whether to enforce the pilot only exiting inside a landing zone. 1 to enable, 0 to disable
*
* \note
*   If <i>require_landing_zone</i> is enabled the pilot will be killed if the aircraft is destroyed
*   outside of a landing zone
*
* \pre
*   To enforce the pilot exiting the aircraft only inside a landing zone this script requires a
*   warhead named "Death" to be defined in the armor.ini file and for that warhead to be capable of
*   inflicting damage upon both the vehicle and pilot.
*/
class dp88_Aircraft_LandingZone_Aircraft : public ScriptImpClass
{
  void Created ( GameObject *obj );
  void Killed ( GameObject *obj, GameObject* killed );
  void Custom ( GameObject *obj, int type, int param, GameObject *sender );

private:
  int driverID;
  unsigned int landingZoneCount;  // Since we may be in more than one at a time
};

// -------------------------------------------------------------------------------------------------



/*------------------------
Terror Drone Script
--------------------------*/

/*class dp88_AR_TerrorDrone : public ScriptImpClass {
	void Created( GameObject *obj );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );
	void Killed( GameObject *obj, GameObject *killer );

	int targetID;
	int pilotID;
	int consoleID;
	char defaultModel[32];
};*/

// -------------------------------------------------------------------------------------------------

/*!
* \brief Remote Control Vehicle Console
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* This script is attached to consoles to allow players to take remote control of a drone vehicle
* which is spawned by the console on demand. The vehicle preset which is spawned must have the
* dp88_RemoteControlVehicle script attached with a matching remoteControlID parameter to work
* correctly
*
* \pre
*   A preset with the name "Invisible_Object_2" must exist to allow the dummy character to be
*   created. You can create this by making a copy of the stock Invisible_Object preset and unticking
*    the "IsHiddenObject" parameter to enable it to be damagable.
*
* \param remoteControlID
*   Shared ID used by the console and drones to enable them to be linked correctly - each vehicle
*   preset should use a unique ID, but multiple consoles can share that vehicles ID
* \param vehiclePreset
*   Name of the vehicle preset to spawn as the drone
* \param cost
*   Cost of purchasing a drone when non exists
* \param team
*   ID of the team permitted to use this console (0: Nod, 1: GDI, 2: Both)
* \param chargeTime
*   Minimum time, in seconds, for the console to charge up before the purchase of a drone is
*   permitted and to recharge between drone purchases. Set to 0 to disable charging.
* \param accessDeniedSound
*   2D sound to play for a player of the wrong team who tries to use the console
* \param consoleOfflineSound
*   2D sound to play for a player trying to use a disabled console
* \param consoleChargingSound
*   2D sound to play for a player trying to use a console which is charging
* \param insufficientFundsSound
*   2D sound to play for a player trying to purchase a drone with insufficient credits
* \param connectionEstablishedSound
*   2D sound to play for a player when they successful take control of a drone
* \param animationName
*   Name of the animation for the state frames
* \param animFrameAvailable1
*   First frame of the animation to play when the console is available (drone ready to purchase)
* \param animFrameAvailable2
*   Last frame of the animation to play when the console is available (drone ready to purchase)
* \param animFrameCharging1
*   First frame of the animation to play when the console is charging
* \param animFrameCharging2
*   Last frame of the animation to play when the console is charging
* \param animFrameActive1
*   First frame of the animation to play when the console is in use
* \param animFrameActive2
*   Last frame of the animation to play when the console is in use
* \param animFrameIdle1
*   First frame of the animation to play when the console is available (drone idle in field)
* \param animFrameIdle2
*   Last frame of the animation to play when the console is available (drone idle in field)
* \param animFrameDisabled1
*   First frame of the animation to play when the console is disabled
* \param animFrameDisabled2
*   Last frame of the animation to play when the console is disabled
*/
class dp88_RemoteControlConsole : public ScriptImpClass
{
  void Created( GameObject *obj );
  void Detach ( GameObject* obj );
  void Poked ( GameObject *obj, GameObject *poker );
  void Custom( GameObject *obj, int type, int param, GameObject *sender );
  void Timer_Expired( GameObject *obj, int number );
  void SetEnabled ( GameObject* obj, bool state );

  int vehicleID;
  int pilotID;
  Collision_Group_Type m_pilotCachedCollisionGroup;
  int m_pilotDummyID;
  Vector3 pilotDummyPos;
  bool m_bEnabled;
  int m_nChargeTime;
  
  void HandleDriverEnter(GameObject* obj, GameObject* pilot, GameObject* vehicle);
  void HandleDriverExit(GameObject* obj, GameObject* pilot, GameObject* vehicle);

  void CreateDummy ( GameObject* pilot, Vector3 position, float facing );
  void DestroyDummy();

  void UpdateAnimation(GameObject* obj);

protected:
  LoopedAnimationController* m_pLoopedAnimCtrl;
public:
  dp88_RemoteControlConsole() : m_pLoopedAnimCtrl(0) {}
};

// -------------------------------------------------------------------------------------------------

/*------------------------
Remote Control Vehicle script
--------------------------*/

class dp88_RemoteControlVehicle : public ScriptImpClass
{
	void Created( GameObject *obj );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );
	void Killed( GameObject *obj, GameObject *killer );

	int consoleID;
	int pilotID;
};



/*------------------------
Demo Truck Scripts
--------------------------*/

class dp88_AR_DemoTruck : public ScriptImpClass {

	// Events
	void Created( GameObject *obj );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );
	void Killed( GameObject *obj, GameObject *killer );
	void Damaged ( GameObject *obj, GameObject *damager, float amount );

	// Custom functions
	void dp88_AR_DemoTruck::Detonate( GameObject *obj );

	// Variables
	int pilotID;
	bool canDetonate;
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief AR Paradrop Console
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \arscript
*
* A paradrop console script for AR, this is currently in active development and subject to change.
*/
class dp88_AR_paradrop_Console : public ScriptImpClass {
	void Created( GameObject *obj );
	void Poked ( GameObject *obj, GameObject *poker );

	int last_triggered;
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief AR Paradrop
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \arscript
*
* This script can be attached to an infantry unit which can be paradropped, allowing it to survive
* the impact with the ground and optionally, upon hitting the ground, switch to a new character
* preset.
*
* \pre
*   A preset with the name "Invisible_Object" must exist to allow the parachute model to be created
*
* \note
*   This script determines an infantry unit has hit the ground when it takes damage from an "Earth"
*   warhead OR it's Z velocity decreases, which is checked at an interval of 1 second.
*
* \param Infantry_Preset
*   Optional. The name of an infantry preset to switch to when the unit hits the ground
* \param Parachute_Model
*   Optional. The name of a W3D model to spawn on the infantry unit whilst it is falling. This will
*   be removed when the unit hits the ground
* \param Parachute_Bone
*   Optional. The name of a bone on the infantry unit to attach the parachute model to, if any
* \param Animation
*   Optional. The name of an animation to play on the infantry unit whilst it is falling
* \param Animation_First_Frame
*   The first frame of the falling animation to play
* \param Animation_Last_Frame
*   The last frame of the falling animation to play
* \param Animation_Looped
*   Whether to loop the falling animation
*/
class dp88_AR_Paradrop : public ScriptImpClass
{
public:
  dp88_AR_Paradrop() : m_pAnimController(0) {};
protected:
  void Created( GameObject* pObj );
  void Damaged( GameObject* pObj, GameObject* pDamager, float amount );
  void Killed( GameObject* pObj, GameObject* pKilled );
  void Timer_Expired ( GameObject* pObj, int number );
  void Detach(GameObject* obj);

  void Landed ( GameObject* pObj );

  unsigned int earth_warhead;       //!< ID of the "Earth" warhead
  int m_nParachuteModel;            //!< ID of the spawned parachute model, if any
  float m_fallRate;                 //!< Falling speed at last check

  LoopedAnimationController* m_pAnimController;
};

// -------------------------------------------------------------------------------------------------





// -------------------------------------------------------------------------------------------------

/*!
* \brief Apocalypse Rising Prism Tower
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_basedefences
* \arscript
*
* This script implements the prism tower logic for Apocalypse Rising on top of the generic charged
* turret implementation provided by dp88_AI_ChargedTurret.
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
* \todo
*   Decide on a mechanism to ensure each firing sequence fires one bullet per assisting tower,
*   regardless of chain length. Can possibly overload dp88_AI_ChargedTurret::StartDischarging() to
*   introduce a 0.1 second delay for each level of assisting tower depth to ensure the charging
*   shot arrives before it fires its own attacking (or chain charging) shot.
*/
class dp88_AR_Prism_Tower : public dp88_AI_ChargedTurret
{
  /* -----
  Static variables
  ----- */

  static const int MAX_TOWERS = 20;
  static int prismTowerCount;
  static int prismTowerIds[MAX_TOWERS];
  static dp88_AR_Prism_Tower* prismTowerScripts[MAX_TOWERS];

  /* -----
  Static functions
  ----- */
  static void registerTower(int towerId, dp88_AR_Prism_Tower* script);
  static void deregisterTower(int towerId);
  static void clearTowerMap(int idx);
  static void calculateTowerMap();
  static bool calculateTowerMapPathSearch(int* sortedConnections, int numConnections, int tower1, int tower2);
  
  /*!
  * Checks if tower1 can assist tower2 by checking they are in range of each other and that there is
  * a clear line of sight between the towers
  */
  static bool CanAssistTower(GameObject* tower1, GameObject* tower2, int maxRange);

  /* -----
  Variables
  ----- */

  // Is our current target a tower we are charging?
  bool isAssistingTower;

  // Towers adjacent to us in the tower map
  int adjacentTowerCount;
  int* adjacentTowers;


  /* -----
  Events
  ----- */

  void Created ( GameObject *obj );
  void Damaged ( GameObject *obj, GameObject *damager, float amount );
  void Killed ( GameObject *obj, GameObject *killer );
  void Destroyed ( GameObject *obj );
  void Custom ( GameObject *obj, int type, int param, GameObject *sender );
  void Timer_Expired ( GameObject *obj, int number );

  /* -----
  Functions
  ----- */

  // Override priority calculation with our own taking into account
  // the fact that the target might be a tower we are charging
  virtual float getPriority( GameObject *obj, GameObject *target );

  /*!
  * Overrides the team check mechanism to return true if the specified target is the tower we are
  * currently charging, otherwise the base class target validation logic will cancel the 'attack'
  */
  virtual bool checkTeam( GameObject *obj, GameObject *target );

  // Customised attack functionality
  virtual void attackTarget ( GameObject* obj, GameObject* target, bool primary );
  virtual void attackLocation ( GameObject* obj, Vector3 location, bool primary );
  virtual void stopAttacking ( GameObject* obj );

  /* Functions to start and stop charging of another tower */
  void StartAssisting(GameObject* obj, GameObject* tower, float priority);
  void StopAssisting(GameObject* obj);
  
  /*!
  * Get the position to aim at when charging another prism tower, this uses the height of the
  * other towers muzzle bone and the X,Y components of its position to aim at the right place
  *
  * \param[in] pTargetTower
  *   Pointer to the GameObject of the tower to get an assist aiming point for
  *   
  */
  static Vector3 GetAssistAimPoint(GameObject* pTargetTower);

  /*!
  * Send assistance requests to all neighbouring prism towers, except the one we are currently
  * assisting, if any
  */
  void SendAssistanceRequests(GameObject* obj);

  /*!
  * Send end assistance notifications to all neighnouring prism towers
  */
  void SendEndAssistanceNotifications(GameObject* obj);
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Object health linking script
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* This is a simple script that links together the health and armour states of two different objects,
* keeping them in constant sync. This is useful when an ingame entity is made up of two seperate
* game objects, such as the AI controlled War Miner in Apocalypse Rising which spawns a seperate
* gun turret to allow the logic for the turret and the miner to execute on seperate objects and
* avoid conflicts between the attack and move-to actions.
*
* The script works on a parent/child basis, whereby the object this script is attached to is the
* child and the parent is the object whose ID is specified in the script parameters. Upon creation
* the script will inherit the maximum and current values of the parents health and armour states,
* however it will maintain it's own skin and armour types, thus allowing for a child object to be
* either more or less resistant to damage than it's parent, allowing the creation of strong or weak
* points in the overall entities defences.
*
* This script is usually attached to dynamically created objects by other scripts and it's unlikely
* to be attached to anything directly via LevelEdit.
*
* \param parentObjectId
*   ID of the parent game object with which this objects health and armour will be linked
*
* \note
*   You can link multiple child objects to a single parent object to create complex entities which
*   consist of many parts, but you should always ensure the same object is designated as the parent
*   to avoid conflicts between the scripts.
*/
class dp88_linkHealth : public ScriptImpClass
{
  void Created ( GameObject *obj );
  void Timer_Expired ( GameObject *obj, int number );
  void Killed ( GameObject *obj, GameObject *killer );
  void equaliseHealth( GameObject* obj );

  int parentObjID;
  float lastHealth;
  float lastArmour;
};

// -------------------------------------------------------------------------------------------------






/*
void printDebug ( const char debugline[], int objID )
{
	FILE *f = fopen("dp88_debug.txt","at");
	fprintf(f,"[%d:%s] %s\n",objID,Commands->Get_Preset_Name(Commands->Find_Object(objID)),debugline);
	fclose(f);
}
*/
