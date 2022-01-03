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

#include "LoopedAnimationController.h"


// Define building child types
#define BUILDING_CHILD_TYPE_GENERIC				    0
#define BUILDING_CHILD_TYPE_MINOR_WEAKPOINT		1
#define BUILDING_CHILD_TYPE_MAJOR_WEAKPOINT		2
#define BUILDING_CHILD_TYPE_CAPTUREPOINT		  3


/*------------------------
Building Controller Script
--------------------------*/

class dp88_buildingScripts_controller : public ScriptImpClass
{
  // Struct to contain data for building children
  struct ChildData
  {
    int objectId;
    int type;
    bool destroyed;

    ChildData* operator= (ChildData* cd )
    {
      objectId = cd->objectId;
      type = cd->type;
      destroyed = false;
    }
  };

  void Created( GameObject *obj );
  void Destroyed( GameObject *obj );
  void Killed( GameObject *obj, GameObject *killer );
  void Custom( GameObject *obj, int type, int param, GameObject *sender );

  // Array of data for child objects belonging to this building controller
  int numChildren;
  ChildData* children;

  /*!
  * Find the position of the given object ID in the children array
  * \return Array index of child, or -1 if not found
  */
  int findChild ( int objectId );

  // Is the building online?
  bool buildingOnline;

  // String IDs
  unsigned long team0_buildingOfflineStringId;
  unsigned long team1_buildingOfflineStringId;
  unsigned long team0_buildingOnlineStringId;
  unsigned long team1_buildingOnlineStringId;
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief dp88 Building Scripts - Base Class
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* This is the base class for all scripts in the dp88_BuildingScripts set except the controller. It
* handles location of the parent building (where necessary) and parsing all of the custom messages
* from the parent building.
*/
class dp88_buildingScripts_baseClass : public ScriptImpClass
{
public:
  dp88_buildingScripts_baseClass();

protected:
  /*! This function should be called in the Created() function of any derived script which is placed
  on an object other than the one with the dp88_buildingScripts_controller script attached.
  If no parent is found this will call Destroy_Script() and return false */
  GameObject* RegisterWithParent( GameObject* obj, const char* preset_name, int type );


  /*! Handles custom messages from the parent building, should not be overridden by derived classes,
  which should instead override the new OnCustom() stub function if they need to do their own custom
  processing */
  void Custom( GameObject *obj, int type, int param, GameObject *sender );

  /*! This function is called by Custom() for any custom it has not handled itself */
  virtual void OnCustom( GameObject* obj, int type, int param, GameObject *sender ) {};


  /*! Event function called when the building enters the "offline" state */
  virtual void OnBuildingOffline( GameObject* obj ) {};

  /*! Event function called when the building enters the "online" state */
  virtual void OnBuildingOnline( GameObject* obj ) {};

  /*! Event function called when the building has been captured by a team or become neutral */
  virtual void OnBuildingCaptured( GameObject* obj, int team ) {};

  /*! Event function called when the building has been destroyed */
  virtual void OnBuildingDestroyed( GameObject* obj ) {};


  /*! ID of the building controller we belong to */
  int m_parentId;
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief dp88 Building Scripts - Component Base Class
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* An extension of dp88_buildingScripts_objectBaseClass which adds extra support for building
* components such as weakpoints and purchase terminals.
*/
class dp88_buildingScripts_componentBaseClass : public dp88_buildingScripts_baseClass
{
public:
  dp88_buildingScripts_componentBaseClass();

protected:
  /*! Cleans up animation controller */
  void Detach( GameObject* obj );

  /*! Called by Detach() to allow derived classes to use the event without overriding Detach() */
  virtual void OnDetach( GameObject* obj ) {};


  /*! Plays the specified animation, creating a new animation controller if one does not exist. If
  the animation name is NULL, a zero length string or invalid any existing animation is stopped */
  void PlayAnimation( GameObject* obj, const char* animation, int nStartFrame, int nEndFrame, bool bLooped );


  /*! Controller for the weakpoint animation */
  LoopedAnimationController* m_pAnimController;
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief dp88 Building Scripts - Building Component
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_dp88BuildingScripts
*
* Attach this script to any object whose health should be linked with the health of the parent
* building. This can be used to create areas of the building which have different skin/shield types,
* such as repair nodes which allow faster repairing or weak areas which cause more damage.
*
* \pre
*   There must be a warhead of type "Death" defined in armor.ini which can cause damage to the
*   object this script is attached to otherwise it will not be destroyed when the parent building
*   gets killed.
*
* \param Building_Controller_Preset
*   Name of the preset to which the controller script for this building is attached. Note that if
*   there are multiple copies of this preset on the map the script will use the closest instance
*   as its parent.
* \param Animation_Name
*   Optional. The name of an animation to play on this component
* \param Animation_Start_Frame
*   Optional. The first frame number to play within the named animation
* \param Animation_End_Frame
*   Optional. The last frame number to play within the named animation
* \param Animation_Looped
*   Optional. Whether to loop the animation. 1 to enable, 0 to disable
* \param Dead_Preset
*   Optional. The name of a preset to spawn in place of this component when it has been destroyed
*
* \note
*   If you want a more complex animation with multiple damage levels you can leave the animation
*   parameters in this script blank and use a script such as dp88_damageAnimation to provide more
*   control over the component animation.
*/
class dp88_buildingScripts_component : public dp88_buildingScripts_componentBaseClass
{
protected:
  void Created( GameObject *obj );
  void OnBuildingDestroyed( GameObject *obj );
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief dp88 Building Scripts - Major Weakpoint
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_dp88BuildingScripts
*
* Attach this script to a building part which should function as a major weakpoint, which can be
* attacked by an enemy to destroy the building. If a building has multiple major weakpoints they
* must all be destroyed before the parent building is destroyed.
*
* \pre
*   There must be a warhead of type "Death" defined in armor.ini which can cause damage to the
*   object this script is attached to otherwise it will not be destroyed when the parent building
*   gets killed.
*
* \param Building_Controller_Preset
*   Name of the preset to which the controller script for this building is attached. Note that if
*   there are multiple copies of this preset on the map the script will use the closest instance
*   as its parent.
* \param Animation_Name
*   Optional. The name of an animation to play on this weakpoint
* \param Animation_Start_Frame
*   Optional. The first frame number to play within the named animation
* \param Animation_End_Frame
*   Optional. The last frame number to play within the named animation
* \param Animation_Looped
*   Optional. Whether to loop the animation. 1 to enable, 0 to disable
* \param Dead_Preset
*   Optional. The name of a preset to spawn in place of this weakpoint when it has been destroyed
*
* \note
*   If you want a more complex animation with multiple damage levels you can leave the animation
*   parameters in this script blank and use a script such as dp88_damageAnimation to provide more
*   control over the weakpoint animation.
*/
class dp88_buildingScripts_majorWeakpoint : public dp88_buildingScripts_componentBaseClass
{
protected:
  void Created( GameObject *obj );
  void Killed( GameObject *obj, GameObject *killer );
  void OnBuildingDestroyed( GameObject *obj );
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief dp88 Building Scripts - Minor Weakpoint
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_dp88BuildingScripts
*
* Attach this script to a building part which should function as a minor weakpoint, which can be
* attacked by an enemy to disable the building until the weakpoint is repaired. If a building has
* multiple minor weakpoints they must all be destroyed before the building is disabled and, once
* disabled, they must all be fully repaired before the building is enabled again.
*
* The exact effects of disabling the building are determined by the building function scripts which
* are attached to the object with the dp88_buildingScripts_controller attached.
*
* \pre
*   The weakpoint is considered destroyed if the shield strength is 0 and the object should be
*   indestructable to allow it to be repaired (if desired). The recommended way to implement this is
*   to give the preset a health value of 1 with a "blamo" skin type. The shield strength alone will
*   then function as the weakpoints total hitpoints.
*
* \param Building_Controller_Preset
*   Name of the preset to which the controller script for this building is attached. Note that if
*   there are multiple copies of this preset on the map the script will use the closest instance
*   as its parent.
* \param Animation_Name
*   Optional. The name of an animation to play on this weakpoint in the "alive" state
* \param Animation_Start_Frame
*   Optional. The first frame number to play within the named animation for the "alive" state
* \param Animation_End_Frame
*   Optional. The last frame number to play within the named animation for the "alive" state
* \param Animation_Looped
*   Optional. Whether to loop the animation in the "alive" state. 1 to enable, 0 to disable
* \param Destroyed_Animation_Name
*   Optional. The name of an animation to play on this weakpoint in the "destroyed" state
* \param Destroyed_Animation_Start_Frame
*   Optional. The first frame number to play within the named animation for the "destroyed" state
* \param Destroyed_Animation_End_Frame
*   Optional. The last frame number to play within the named animation for the "destroyed" state
* \param Destroyed_Animation_Looped
*   Optional. Whether to loop the animation in the "destroyed" state. 1 to enable, 0 to disable
*
* \note
*   The "alive" and "destroyed" animations can both be contained in the same animation if desired
*/
class dp88_buildingScripts_minorWeakpoint : public dp88_buildingScripts_componentBaseClass
{
protected:
  void Created( GameObject *obj );
  void Damaged ( GameObject *obj, GameObject *damager, float amount );
  void Killed( GameObject *obj, GameObject *killer );
  void OnBuildingDestroyed( GameObject *obj );

  void UpdateAnimation( GameObject* pSelf );

  /*! Tracks whether this weakpoint has been destroyed, so we can trigger the repaired event
  upon reaching 100% health again */
  bool m_bIsDestroyed;
};

// -------------------------------------------------------------------------------------------------



/*!
* \brief Building function - Produce Infantry
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_dp88BuildingScripts
*
* \todo document script
*/
class dp88_buildingScripts_functionProduceInfantry : public dp88_buildingScripts_baseClass
{
protected:
  void OnBuildingOffline( GameObject* obj );
  void OnBuildingOnline( GameObject* obj );
};




/*!
* \brief Building function - Produce Vehicles
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_dp88BuildingScripts
*
* \todo document script
*/
class dp88_buildingScripts_functionProduceVehicles : public dp88_buildingScripts_baseClass
{
protected:
  void OnBuildingOffline( GameObject* obj );
  void OnBuildingOnline( GameObject* obj );
};


/*!
* \brief Building function - Produce Aircraft
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_dp88BuildingScripts
*
* \todo document script
*/
class dp88_buildingScripts_functionProduceAircraft : public dp88_buildingScripts_baseClass
{
protected:
  void OnBuildingOffline( GameObject* obj );
  void OnBuildingOnline( GameObject* obj );
};

/*!
* \brief Building function - Produce Naval Units
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_dp88BuildingScripts
*
* \todo document script
*/
class dp88_buildingScripts_functionProduceNavalUnits : public dp88_buildingScripts_baseClass
{
protected:
  void OnBuildingOffline( GameObject* obj );
  void OnBuildingOnline( GameObject* obj );
};

/*!
* \brief Building function - Provide Power
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_dp88BuildingScripts
*
* \todo document script
*/
class dp88_buildingScripts_functionProvidePower : public dp88_buildingScripts_baseClass
{
protected:
  void OnBuildingOffline( GameObject* obj );
  void OnBuildingOnline( GameObject* obj );
};

/*!
* \brief Building function - Provide Radar
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_dp88BuildingScripts scripts_radar
*
* \todo document script
*/
class dp88_buildingScripts_functionProvideRadar : public dp88_buildingScripts_baseClass
{
protected:
  void OnBuildingOffline( GameObject* obj );
  void OnBuildingOnline( GameObject* obj );
};

/*!
* \brief Building function - Provide Harvester
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_dp88BuildingScripts
*
* \todo document script
*/
class dp88_buildingScripts_functionProvideHarvester : public dp88_buildingScripts_baseClass
{
protected:
  void OnBuildingOffline( GameObject* obj );
  void OnBuildingOnline( GameObject* obj );
};


// Money trickle function - gives the owning team X credits per second whilst alive
class dp88_buildingScripts_functionMoneyTrickle : public dp88_buildingScripts_baseClass
{
public:
  dp88_buildingScripts_functionMoneyTrickle() : isTimerRunning(false) {}

  void Created ( GameObject* obj );
  void Timer_Expired ( GameObject* obj, int number );
  void OnBuildingCaptured ( GameObject* obj, int team );

private:
  bool isTimerRunning;
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Building function - Grant Money On Capture
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_dp88BuildingScripts
*
* Grants a fixed amount of money to a team when they capture this building. If the building is not
* neutral when this script is attached the current team will immediately get money. Can either give
* money each time the building is captured or only the first time it is captured. This script should
* be attached to the same object as the dp88_buildingScripts_controller script for the building.
*
* \param credits
*   The amount of credits to be given
* \param onceOnly
*   Only give credits the first time the building is captured
*/
class dp88_buildingScripts_functionMoneyGrant : public dp88_buildingScripts_baseClass
{
  void Created ( GameObject* obj );
  void OnBuildingCaptured ( GameObject* obj, int team );
  
  /*! \name Cached Script Parameters */
  /*! @{ */
  float m_credits;
  /*! @} */
};

// -------------------------------------------------------------------------------------------------

// Spawn team zone - spawns a script zone depending on which team currently owns
// the building
class dp88_buildingScripts_functionSpawnTeamZone : public dp88_buildingScripts_baseClass
{
public:
  dp88_buildingScripts_functionSpawnTeamZone() : zoneId(NULL) {}

	void Created ( GameObject* obj );
	void OnBuildingOffline( GameObject* obj );
	void OnBuildingOnline ( GameObject* obj );
	void OnBuildingCaptured ( GameObject* obj, int team );
	void Destroyed ( GameObject* obj );

private:
	// Functions to create and destroy the script zone
	void createZone(GameObject* obj);
	void destroyZone(GameObject* obj);

	// ID of the zone we have created
	int zoneId;
};




/*!
* \brief Building Function - Spawn Zone
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_dp88BuildingScripts
*
* Spawns a script zone at the location of the object this script is attached to and, optionally,
* destroys the zone when the parent building is offline and/or destroyed. This can be used to proxy
* script zones into the correct locations using Daves Arrow type presets with this script.
*
* \param Building_Controller_Preset
*   Name of the preset to which the controller script for this building is attached. Note that if
*   there are multiple copies of this preset on the map the script will use the closest instance
*   as its parent.
* \param scriptZonePreset
*   Name of the script zone preset to be spawned
* \param scriptZoneSize
*   The size of the script zone in the + and - directions on each axis, so a size of 1x1x2 will
*   create a 2x2x4 script zone centered on the object this script is attached to
* \param destroyZoneWhenDisabled
*   Whether to destroy the spawned script zone when the parent building is offline. It will be
*   created again when the building comes back online. 1 to enable, 0 to disable
* \param destroyZoneWhenDestroyed
*   Whether to destroy the spawned script zone when the parent building is destroyed. 1 to enable,
*   0 to disable
*/
class dp88_buildingScripts_functionSpawnZone : public dp88_buildingScripts_baseClass
{
public:
  dp88_buildingScripts_functionSpawnZone() : zoneId(NULL) {}

  void Created( GameObject *obj );
  void OnBuildingOffline( GameObject* obj );
  void OnBuildingOnline ( GameObject* obj );
  void OnBuildingDestroyed ( GameObject* obj );

  // Functions to create and destroy the script zone
  void createZone(GameObject* obj);
  void destroyZone(GameObject* obj);

  // ID of the zone we have created
  int zoneId;
};




/*!
* \brief dp88 Building Scripts - Capture Point
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_dp88BuildingScripts
*
* \todo document script
*/
class dp88_buildingScripts_capturePoint : public dp88_buildingScripts_baseClass
{
protected:
  void Created( GameObject *obj );
  void Damaged ( GameObject *obj, GameObject *damager, float amount );
  void OnBuildingCaptured ( GameObject* obj, int team );
  void OnBuildingDestroyed( GameObject *obj );
  void Timer_Expired( GameObject *obj, int number );

  void UpdateAnimationFrame( GameObject* obj );

  int currentTeam;

  /*! \name Cached Script Parameters */
  /*! @{ */
  int m_nAnimTransitionFrames;
  /*! @} */
};



/*------------------------
Spawn Building Part script
--------------------------*/

class dp88_buildingScripts_spawnPart : public dp88_buildingScripts_baseClass
{
	void Created ( GameObject* obj );
	void Timer_Expired( GameObject *obj, int number );
	void OnBuildingCaptured ( GameObject* obj, int team );
	void Destroyed ( GameObject* obj );

	int partObjectId;
};
