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
// Include files
#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "dp88_ar_unitScripts.h"
#include "SoldierGameObj.h"
#include "vehiclegameobj.h"
#include "vehiclegameobjdef.h"



/*------------------------
Rocketeer Script Functions
--------------------------*/

void dp88_AR_Rocketeer::Created ( GameObject* pObj )
{
  if ( m_nSoldierID != 0 )  // Triggered by the flight vehicle we are also listening too
    return;

  if ( !pObj->As_SoldierGameObj() )
  {
    Console_Output ( "[%d:%s:%s] Critical Error: This script is only compatible with soldier game objects. Destroying script...\n", Commands->Get_ID(pObj), Commands->Get_Preset_Name(pObj), this->Get_Name() );
    Destroy_Script();
    return;
  }

  if ( strlen(Get_Parameter("Flying_Preset")) <= 0 || !Is_Valid_Preset(Get_Parameter("Flying_Preset")) )
  {
    Console_Output ( "[%d:%s:%s] Critical Error: The specified flying preset, \"%s\", is not vaild. Destroying script...\n", Commands->Get_ID(pObj), Commands->Get_Preset_Name(pObj), this->Get_Name(), Get_Parameter("Flying_Preset") );
    Destroy_Script();
    return;
  }

  m_nSoldierID    = Commands->Get_ID(pObj);
  m_nVehicleID    = NULL;
  m_lastDeploy    = 0;
  m_bCanDrive     = (pObj->As_SoldierGameObj())->Can_Drive_Vehicles();

  m_minWalkTime   = Get_Int_Parameter("Minimum_Walk_Time");
  m_minFlightTime = Get_Int_Parameter("Minimum_Flight_Time");

  // Only attach keyhooks to players... but for AI units we might do something clever in future so
  // don't destroy the script if it's not an actual player
  if (Get_Player_ID(pObj) >= 0)
    InstallHook( Get_Parameter("Keyhook"), pObj );
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Rocketeer::Destroyed (GameObject* pObj)
{
  RemoveHook();
  
  if (NULL != m_nVehicleID)
  {
    if (GameObject* pVehicle = Commands->Find_Object(m_nVehicleID))
    {
      if (pVehicle->As_VehicleGameObj())
      {
		  int sound = pVehicle->As_VehicleGameObj()->Get_Definition().Get_Engine_Sound(ENGINE_SOUND_STATE_STOPPING);
		  const char *sound2 = Get_Definition_Name(sound);
		  Commands->Create_Sound(sound2, Commands->Get_Position(pVehicle), pVehicle);
      }
      Commands->Destroy_Object(pVehicle);
    }
    
    m_nVehicleID = NULL;
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Rocketeer::Custom( GameObject* pObj, int type, int param, GameObject* pSender )
{
  // Only interested in the flight vehicle being destroyed
  if ( m_nVehicleID == Commands->Get_ID(pSender) && type == CUSTOM_ROCKETEER_VEHICLEKILLED )
  {
    Commands->Attach_Script ( pObj, "RA_DriverDeath", "" );
    RemoveHook();
    m_nVehicleID = NULL;
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Rocketeer::Timer_Expired ( GameObject* pObj, int number )
{
  if ( number == TIMER_ROCKETEER_ENTERVEHICLE )
  {
    //Console_Output ( "[%d:%s:%s] Transitioning driver into vehicle\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
    if ( GameObject* pVehicle = Commands->Find_Object(m_nVehicleID) )
    {
      Force_Vehicle_Entry(pObj,pVehicle);
    }
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Rocketeer::KeyHook()
{
  // Find the soldier object and check they are alive and not on a ladder
  GameObject* pObj = Commands->Find_Object(m_nSoldierID);
  if ( !pObj || Commands->Get_Health(pObj) <= 0.0f || ((SoldierGameObj*)pObj)->Is_On_Ladder() )
    return;

  // If we are not currently flying then toggle flight mode on
  if (NULL == m_nVehicleID && !Get_Vehicle(pObj) )
  {
    if (time(NULL) - m_lastDeploy < m_minWalkTime)
    {
      StringClass message;
      message.Format("You must wait at least %d seconds before activating flying mode", m_minWalkTime );
      Send_Message_Player(pObj, DP88_RGB_WARNING_MSG, message);
    }
    else
      Toggle_Flight_Mode ( pObj->As_SoldierGameObj(), true );
  }

  // If we are currently flying then toggle flight mode off
  else if (NULL != m_nVehicleID)
  {
    if (time(NULL) - m_lastDeploy < m_minFlightTime)
    {
      StringClass message;
      message.Format("You must wait at least %d seconds before deactivating flying mode", m_minFlightTime );
      Send_Message_Player(pObj, DP88_RGB_WARNING_MSG, message);
    }
    else
      Toggle_Flight_Mode ( pObj->As_SoldierGameObj(), false );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Rocketeer::Toggle_Flight_Mode ( SoldierGameObj* pSoldier, bool bState )
{
  if ( !pSoldier )
    return;

  if ( bState && NULL == m_nVehicleID)
  {
    if ( !m_bCanDrive )
      (pSoldier->As_SoldierGameObj())->Set_Can_Drive_Vehicles(true);

    // Spawn flight vehicle and listen to its events (specifically the Killed event)
    Vector3 pos = Commands->Get_Position(pSoldier);
    pos.Z += 0.25;
    GameObject* pVehicle = Commands->Create_Object(Get_Parameter("Flying_Preset"),pos);
    Commands->Set_Facing(pVehicle, Commands->Get_Facing(pSoldier));
    m_nVehicleID = Commands->Get_ID(pVehicle);

    Attach_Script_V ( pVehicle, "JFW_Death_Send_Custom", "%d,%d,0", Commands->Get_ID(pSoldier), CUSTOM_ROCKETEER_VEHICLEKILLED );

    // Set the flight vehicles health/armour to match ours
    Set_Max_Health(pVehicle,Commands->Get_Max_Health(pSoldier));
    Set_Max_Shield_Strength(pVehicle,Commands->Get_Max_Shield_Strength(pSoldier));

    Commands->Set_Health(pVehicle,Commands->Get_Health(pSoldier));
    Commands->Set_Shield_Strength(pVehicle,Commands->Get_Shield_Strength(pSoldier));

    // Need to wait for an engine tick before transitioning the soldier into the vehicle
    Commands->Set_Is_Rendered(pSoldier,false);
    Commands->Start_Timer ( pSoldier, this, 0.5f, TIMER_ROCKETEER_ENTERVEHICLE );

    m_lastDeploy = time(NULL);
  }




  else if ( !bState && NULL != m_nVehicleID)
  {
    GameObject* pVehicle = Commands->Find_Object(m_nVehicleID);
    if ( pVehicle )
    {
      Commands->Set_Health(pSoldier,Commands->Get_Health(pVehicle));
      Commands->Set_Shield_Strength(pSoldier,Commands->Get_Shield_Strength(pVehicle));

	  if (pVehicle->As_VehicleGameObj())
	  {
		  int sound = pVehicle->As_VehicleGameObj()->Get_Definition().Get_Engine_Sound(ENGINE_SOUND_STATE_STOPPING);
		  const char *sound2 = Get_Definition_Name(sound);
		  Commands->Create_Sound(sound2, Commands->Get_Position(pVehicle), pVehicle);
	  }
	  Commands->Destroy_Object(pVehicle);
    }

    m_nVehicleID = NULL;
    m_lastDeploy = time(NULL);

    Commands->Set_Is_Rendered(pSoldier,true);

    if ( !m_bCanDrive )
      (pSoldier->As_SoldierGameObj())->Set_Can_Drive_Vehicles(false);
  }
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_AR_Rocketeer> dp88_AR_Rocketeer_Registrant(
  "dp88_AR_Rocketeer",
  "Flying_Preset:string,"
  "Keyhook=IDeploy:string,"
  "Minimum_Flight_Time=5:int,"
  "Minimum_Walk_Time=5:int"
);








/*------------------------
Mirage Tank Script Functions
--------------------------*/

void dp88_AR_MirageTank::Created(GameObject *obj)
{
  m_driverID = NULL;
  m_mirageID = NULL;
  m_lastActivity = time(NULL);

  // If the game controller does not exist then bail out
  GameObject *gameController = Find_Object_With_Script("dp88_ar_gameController");
  if ( !gameController )
  {
    Console_Output("dp88_AR_MirageTank - Unable to find Game Controller, unable to continue. Destroying script...\n");
    Destroy_Script();
    return;
  }

  m_controllerID = Commands->Get_ID(gameController);

  m_delay = Get_Int_Parameter("Mirage_Delay");
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_MirageTank::Damaged(GameObject* obj, GameObject* damager, float amount)
{
  m_lastActivity = time(NULL);

  if (NULL != m_mirageID)
    setHidden(obj, false);
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_MirageTank::Killed(GameObject* obj, GameObject* killer)
{
  if (NULL != m_mirageID)
    setHidden(obj, false);
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_MirageTank::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Look for vehicle entry
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (NULL == m_driverID)
		{
			m_driverID = Commands->Get_ID(sender);
      m_lastActivity = time(NULL);

			// Start timer to check for no movement (and create mirage)
			Commands->Start_Timer(obj, this, 0.5, TIMER_MIRAGE_CHECKMOVEMENT);
		}
	}

	// Look for vehicle exit
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (Commands->Get_ID(sender) == m_driverID)
		{
			m_driverID = NULL;
			setHidden(obj, false);
		}
	}
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_MirageTank::Timer_Expired(GameObject *obj, int number)
{
  if (number == TIMER_MIRAGE_CHECKMOVEMENT && NULL != m_driverID)
  {
    // Because stationary vehicles seem to 'drift' without moving we check that the difference is
    // very low to see if it is stationary. Also check if we are shooting currently. Apparently
    // vehicles drift a LOT, hence why we're using the relatively large value of 0.1...
    Vector3 speed = Get_Velocity(obj);
    if (fabs(speed.X) > 0.1 || fabs(speed.Y) > 0.1 || fabs(speed.Z) > 0.1 || Get_Current_Bullets(obj) == 0)
    {
      //Console_Output("dp88_AR_MirageTank::Timer_Expired - movement or shooting detected (%.4f, %.4f, %.4f, %d)\n", speed.X, speed.Y, speed.Z, Get_Current_Bullets(obj));
      m_lastActivity = time(NULL);

      if (NULL != m_mirageID)
        setHidden(obj, false);
    }

    else if (NULL == m_mirageID && time(NULL) - m_lastActivity > m_delay)
    {
      setHidden(obj, true);
    }

    Commands->Start_Timer(obj, this, 0.5, TIMER_MIRAGE_CHECKMOVEMENT);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_MirageTank::setHidden(GameObject *obj, bool hide)
{
  //Console_Output("dp88_AR_MirageTank::setHidden - %s\n", (hide)?"true":"false");

  if (hide && NULL == m_mirageID)
  {
    /* Get handle to game controller script */
    GameObject *gameController = Commands->Find_Object(m_controllerID);
    dp88_AR_GameController *gameControllerScript = (NULL != gameController) ? (dp88_AR_GameController*)Find_Script_On_Object(gameController, "dp88_ar_gameController") : NULL;
    if ( !gameControllerScript )
      return;

    // Check if there are any model names defined
    if ( gameControllerScript->mirageTank_disguisePresets[0] == 0
      && gameControllerScript->mirageTank_disguisePresets[1] == 0
      && gameControllerScript->mirageTank_disguisePresets[2] == 0 )
    {
      Console_Output ( "[%d:%s:%s] No valid Mirage Tank disguises have been set in the GameController, aborting script!", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
      Destroy_Script();
      return;
    }

    // Get a disguise number to use
    int disguiseID = -1;
    while ( disguiseID == -1 )
    {
      disguiseID = Commands->Get_Random_Int(0, 3); // 0 -> 2
      if (NULL == gameControllerScript->mirageTank_disguisePresets[disguiseID])
        disguiseID = -1;
    }

    // Create mirage
    Vector3 miragePosition = Commands->Get_Position(obj);
    miragePosition.Z -= 3;
    GameObject* mirage = Commands->Create_Object(gameControllerScript->mirageTank_disguisePresets[disguiseID], miragePosition);
    Commands->Disable_All_Collisions(mirage);
    m_mirageID = Commands->Get_ID(mirage);

    // Setup tank variables
    Commands->Enable_Stealth(obj, true);        // Disable targeting box
    Commands->Set_Is_Rendered(obj, false);      // Disables rendering
    Commands->Set_Is_Visible(obj, false);       // Prevents AI seeing tank
    Commands->Enable_Engine(obj, false);        // Disable engine sounds
  }


  else if (!hide && NULL != m_mirageID)
  {
    // Delete mirage
    GameObject* mirage = Commands->Find_Object(m_mirageID);
    if (NULL != mirage)
    {
      Commands->Destroy_Object(mirage);
      m_mirageID = NULL;
    }

    // Setup tank variables
	if (m_driverID)
	{
		Commands->Enable_Engine(obj, true);
	}
    Commands->Set_Is_Visible(obj, true);
    Commands->Set_Is_Rendered(obj, true);
    Commands->Enable_Stealth(obj, false);
  }
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_AR_MirageTank> dp88_AR_MirageTank_Registrant(
  "dp88_AR_MirageTank",
  "Mirage_Delay=2:int"
);




/*------------------------
IFV Scripts Functions
--------------------------*/

void dp88_AR_IFV::Created ( GameObject *obj )
{
	objectID = Commands->Get_ID ( obj );
	pilotID = NULL;
	isDeployed = false;
	isDeploying = false;
	veterancyLevel = 0;

	deployedAnimFrame = 0;
	deployedRookieWeaponPowerupId = NULL;
	deployedVeteranWeaponPowerupId = NULL;
	deployedEliteWeaponPowerupId = NULL;

	// Save default weapon information
	strcpy_s ( defaultWeapon, sizeof( defaultWeapon ), Get_Current_Weapon ( obj ) );
	strcpy_s ( currentDefaultWeapon, sizeof( currentDefaultWeapon ), Get_Current_Weapon ( obj ) );
	strcpy_s ( currentWeapon, sizeof( currentWeapon ), Get_Current_Weapon ( obj ) );

	// Set default animation frame
	Commands->Set_Animation_Frame ( obj, Get_Parameter ( "Turret_Frames_Animation" ), 1 );
	//Commands->Set_Animation ( obj, Get_Parameter ( "Turret_Frames_Animation" ), false, NULL, 1.0f, 1.0f, false );

	// Grant fake weapon for use during deployment
	//Commands->Give_PowerUp ( obj, "POW_Allied_IFV_None", false );
	nullWeaponGranted = false;
}


void dp88_AR_IFV::Destroyed ( GameObject *obj )
{
}


void dp88_AR_IFV::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Look for vehicle entry
	if ( type == CUSTOM_EVENT_VEHICLE_ENTERED )
	{
		if ( pilotID == NULL )
		{
			pilotID = Commands->Get_ID(sender);

			InstallHook( Get_Parameter("Keyhook"), sender );

			// Send a custom to the new driver to ask for model and powerup data
			Commands->Send_Custom_Event ( obj, sender, CUSTOM_IFV_REQUESTDRIVERDATA, 0, 0.0f );
		}


		/* If this is our first driver grant the Weapon_IFV_None weapon. Trying to do this here
		because for some reason the IFV can still fire while deploying on the FDS and I can only
		assume it's because the weapon cannot be granted at creation time on the FDS, unlike in
		LAN mode where it works perfectly well... */
		if ( nullWeaponGranted == false )
		{
			nullWeaponGranted = true;
			// Grant fake weapon for use during deployment
			Commands->Give_PowerUp ( obj, "POW_Allied_IFV_None", false );
		}
	}




	// Look for vehicle exit
	else if ( type == CUSTOM_EVENT_VEHICLE_EXITED )
	{
		if ( Commands->Get_ID(sender) == pilotID )
		{
			pilotID = NULL;

			RemoveHook();
		}


		// Are we deployed? If so then undeploy
		if ( isDeployed && !isDeploying )
			undeploy();

		/* Otherwise if we are deploying then disable transitions until
		deployment is complete and then undeploy */
		else if ( isDeployed )
			Commands->Enable_Vehicle_Transitions ( obj, false );


		// Reset driver data
		deployedAnimFrame = 0;
		deployedRookieWeaponPowerupId = NULL;
		deployedVeteranWeaponPowerupId = NULL;
		deployedEliteWeaponPowerupId = NULL;
	}




	// Look for a promotion (we will need to store new default weapon
	// if it changed, and possibly restore the deployed weapon)
	else if ( type == CUSTOM_VETERANCY_PROMOTED )
	{
		veterancyLevel = param;

		// If the promotion changed our weapon, then set that as our new default weapon
		if ( Get_Current_Weapon ( obj ) != NULL
			&& strcmp ( Get_Current_Weapon ( obj ), currentWeapon ) != 0 )
		{
			strcpy_s ( currentDefaultWeapon, sizeof(currentDefaultWeapon), Get_Current_Weapon ( obj ) );
			strcpy_s ( currentWeapon, sizeof(currentWeapon), Get_Current_Weapon ( obj ) );
		}


		// If we are deployed then we need to work out which weapon
		// preset we should currently have and set it if necessary
		if ( isDeployed )
		{
			int powerupID = NULL;

			if ( veterancyLevel == 2 && deployedEliteWeaponPowerupId != NULL )
				powerupID = deployedEliteWeaponPowerupId;

			else if ( veterancyLevel >= 1 && deployedVeteranWeaponPowerupId != NULL )
				powerupID = deployedVeteranWeaponPowerupId;

			else
				powerupID = deployedRookieWeaponPowerupId;


			if ( powerupID != NULL )
			{
				Commands->Give_PowerUp ( obj, Get_Definition_Name ( powerupID ), false );
				strcpy_s ( currentWeapon, sizeof(currentWeapon), Get_Powerup_Weapon ( Get_Definition_Name ( powerupID ) ) );
			}
		}


		/* If we are deploying then set weapon to none, otherwise double check
		it is set to the right weapon */
		if ( isDeploying )
			Commands->Select_Weapon ( obj, "Weapon_IFV_None" );
		else
			Commands->Select_Weapon ( obj, currentWeapon );
	}




	// Look for driver responses containing deployment data
	else if ( type == CUSTOM_IFV_DRIVER_ANIMFRAME )
		deployedAnimFrame = param;
	else if ( type == CUSTOM_IFV_DRIVER_ROOKIEPOWID && param > 0 && Is_Valid_Preset_ID ( param ) )
		deployedRookieWeaponPowerupId = param;
	else if ( type == CUSTOM_IFV_DRIVER_VETERANPOWID && param > 0 && Is_Valid_Preset_ID ( param ) )
		deployedVeteranWeaponPowerupId = param;
	else if ( type == CUSTOM_IFV_DRIVER_ELITEPOWID && param > 0 && Is_Valid_Preset_ID ( param ) )
		deployedEliteWeaponPowerupId = param;
}


void dp88_AR_IFV::Timer_Expired( GameObject *obj, int number )
{
	/* Timer to indicate (un)deployment is complete and set correct
	animation frame */
	if ( number == TIMER_IFV_DEPLOYCOMPLETE )
	{
		Create_2D_Sound_Player(Commands->Find_Object(pilotID),Get_Parameter("Sound"));
		isDeploying = false;

		if ( isDeployed )
		{
			Commands->Set_Animation_Frame ( obj, Get_Parameter ( "Turret_Frames_Animation" ), deployedAnimFrame );
			//Commands->Set_Animation ( obj, Get_Parameter ( "Turret_Frames_Animation" ), false, NULL, (float)deployedAnimFrame, (float)deployedAnimFrame, false );

			/* Has the pilot got out during deployment? If so then we
			need to undeploy */
			if ( pilotID == NULL )
			{
				undeploy();
				Commands->Enable_Vehicle_Transitions ( obj, true );
			}
		}

		else
		{
			Commands->Set_Animation_Frame ( obj, Get_Parameter ( "Turret_Frames_Animation" ), 1 );
			//Commands->Set_Animation ( obj, Get_Parameter ( "Turret_Frames_Animation" ), false, NULL, 1.0f, 1.0f, false );
		}
		

		// Select new weapon
		Commands->Select_Weapon( obj, currentWeapon );
	}
}


void dp88_AR_IFV::KeyHook()
{
	// Find object
	GameObject* obj = Commands->Find_Object ( objectID );
	if ( !obj )
		return;


	/* Return if no custom model is defined for the driver or if we
	are already in the process of (un)deploying */
	if ( deployedAnimFrame == 0 || isDeploying == true )
		return;

	// Undeploy
	if ( isDeployed )
		undeploy();

	// Deploy
	else
		deploy();
}


// Deploy function
void dp88_AR_IFV::deploy()
{
	// Find object
	GameObject* obj = Commands->Find_Object ( objectID );
	if ( !obj )
		return;
	
	// Pick a weapon powerup to grant
	int powerupID = NULL;

	if ( veterancyLevel == 2 && deployedEliteWeaponPowerupId != NULL )
		powerupID = deployedEliteWeaponPowerupId;

	else if ( veterancyLevel >= 1 && deployedVeteranWeaponPowerupId != NULL )
		powerupID = deployedVeteranWeaponPowerupId;

	else
		powerupID = deployedRookieWeaponPowerupId;


	// Grant any weapon powerups we found and set that as current weapon
	if ( powerupID != NULL )
	{
		Commands->Give_PowerUp ( obj, Get_Definition_Name ( powerupID ), false );
		strcpy_s ( currentWeapon, sizeof(currentWeapon), Get_Powerup_Weapon ( Get_Definition_Name ( powerupID ) ) );
	}


	// Disable weapon
	Commands->Select_Weapon ( obj, "Weapon_IFV_None" );


	// Set deploying anim frame
	Commands->Set_Animation_Frame ( obj, Get_Parameter ( "Turret_Frames_Animation" ), Get_Int_Parameter ( "Switching_Anim_Frame" ) );
	//Commands->Set_Animation ( obj, Get_Parameter ( "Turret_Frames_Animation" ), false, NULL, (float)Get_Int_Parameter ( "Switching_Anim_Frame" ), (float)Get_Int_Parameter ( "Switching_Anim_Frame" ), false );

	// Set flags and start timer to complete deployment
	isDeployed = true;
	isDeploying = true;
	Set_HUD_Help_Text_Player(Commands->Find_Object(pilotID),Get_Int_Parameter("String"),Get_Vector3_Parameter("Color"));
	Commands->Start_Timer ( obj, this, (float)Get_Int_Parameter ( "Switch_Time" ), TIMER_IFV_DEPLOYCOMPLETE );
}


// Undeploy function
void dp88_AR_IFV::undeploy()
{
	// Find object
	GameObject* obj = Commands->Find_Object ( objectID );
	if ( !obj )
		return;

	// Disable weapon and set the weapon to use after deploying
	Commands->Select_Weapon ( obj, "Weapon_IFV_None" );
	strcpy_s ( currentWeapon, sizeof(currentWeapon), currentDefaultWeapon );

	// Set deploying anim frame
	Commands->Set_Animation_Frame ( obj, Get_Parameter ( "Turret_Frames_Animation" ), Get_Int_Parameter ( "Switching_Anim_Frame" ) );
	//Commands->Set_Animation ( obj, Get_Parameter ( "Turret_Frames_Animation" ), false, NULL, (float)Get_Int_Parameter ( "Switching_Anim_Frame" ), (float)Get_Int_Parameter ( "Switching_Anim_Frame" ), false );

	// Set flags and start timer to complete deployment
	isDeployed = false;
	isDeploying = true;
	Set_HUD_Help_Text_Player(Commands->Find_Object(pilotID),Get_Int_Parameter("String"),Get_Vector3_Parameter("Color"));
	Commands->Start_Timer ( obj, this, (float)Get_Int_Parameter ( "Switch_Time" ), TIMER_IFV_DEPLOYCOMPLETE );
}





void dp88_AR_IFVDriver::Custom ( GameObject* obj, int type, int param, GameObject* sender )
{
	// If we entered an IFV we will recieve this to tell us to register our data
	if ( type == CUSTOM_IFV_REQUESTDRIVERDATA )
	{
		/* Send responses for our animation frame number, rookie weapon powerup ID,
		veteran weapon powerup ID and elite weapon powerup ID */
		Commands->Send_Custom_Event ( obj, sender, CUSTOM_IFV_DRIVER_ANIMFRAME, Get_Int_Parameter ( "IFV_ModelAnimFrame" ), 0.0f );
		
		if ( Is_Valid_Preset ( Get_Parameter ( "IFV_WeaponPowerup_Rookie" ) ) )
			Commands->Send_Custom_Event ( obj, sender, CUSTOM_IFV_DRIVER_ROOKIEPOWID, Get_Definition_ID ( Get_Parameter ( "IFV_WeaponPowerup_Rookie" ) ), 0.0f );
		if ( Is_Valid_Preset ( Get_Parameter ( "IFV_WeaponPowerup_Veteran" ) ) )
			Commands->Send_Custom_Event ( obj, sender, CUSTOM_IFV_DRIVER_VETERANPOWID, Get_Definition_ID ( Get_Parameter ( "IFV_WeaponPowerup_Veteran" ) ), 0.0f );
		if ( Is_Valid_Preset ( Get_Parameter ( "IFV_WeaponPowerup_Elite" ) ) )
			Commands->Send_Custom_Event ( obj, sender, CUSTOM_IFV_DRIVER_ELITEPOWID, Get_Definition_ID ( Get_Parameter ( "IFV_WeaponPowerup_Elite" ) ), 0.0f );
	}
}







/*------------------------
Tesla Coil Script
--------------------------*/

void dp88_AR_Tesla_Coil::Created( GameObject *obj )
{
  loadSettings(obj, false, false);  // No need to support secondary fire
  Init(obj);
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Tesla_Coil::Init ( GameObject* pSelf )
{
  dp88_AI_ChargedTurret::Init(pSelf);

  // Initialize our charge level to 0
  m_chargeLevel = 0;
  m_isSupercharged = false;
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Tesla_Coil::loadSettings ( GameObject* pSelf, bool loadSecondaryFireSettings, bool loadBuildingTargetSettings )
{
  dp88_AI_ChargedTurret::loadSettings(pSelf, loadSecondaryFireSettings, loadBuildingTargetSettings);

  // Load settings
  m_chargeWarheadID     = ArmorWarheadManager::Get_Warhead_Type(Get_Parameter("Charge_Warhead"));
  m_chargeDuration      = Get_Int_Parameter("Charge_Duration");
  m_chargesPowerOn      = Get_Int_Parameter("Charges_Power_On");
  m_chargesSupercharge  = Get_Int_Parameter("Charges_Supercharge");

  // Grant the supercharged weapon powerup preset and save the name of the default weapon preset
  if ( Is_Valid_Preset(Get_Parameter("Supercharged_Weapon_Powerup_Preset")) )
  {
    m_defaultWeapon       = Get_Current_Weapon(pSelf);
    m_defaultWeaponRange  = primary_maxRange;

    Commands->Give_PowerUp(pSelf, Get_Parameter("Supercharged_Weapon_Powerup_Preset"), false);
    m_superchargedWeapon      = Get_Powerup_Weapon(Get_Parameter("Supercharged_Weapon_Powerup_Preset"));
    m_superchargedWeaponRange = Get_Int_Parameter("Supercharged_Weapon_Range");
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Tesla_Coil::Damaged ( GameObject *obj, GameObject *damager, float amount )
{
  // If the damage was done with the right warhead then increase our charge level, unless the last
  // charge from this unit wss within 1 second of the current time - prevents double charges due to
  // explosion damage
  // \todo fix the 1 second limit
  if ( Get_Damage_Warhead() == m_chargeWarheadID /*&& chargerScript->m_lastChargeTime < time(NULL)-1*/ )
  {
    // Increase charge level
    m_chargeLevel++;

    // Set chargers last charge time
    //chargerScript->m_lastChargeTime = time(NULL);

    // Have we reached a sufficient level to become supercharged?
    int supercharge_chargesRequired = (dp88_AI_ChargedTurret::checkPowerState(obj)) ? m_chargesSupercharge : m_chargesPowerOn + m_chargesSupercharge;
    if ( !m_isSupercharged && m_chargeLevel >= supercharge_chargesRequired )
      setSuperchargedState(obj, true);

    // Start timer to decrement charge level
    Commands->Start_Timer(obj, this, (float)m_chargeDuration, TIMER_TESLACOIL_DECREMENT_CHARGE );
  }

  // Call base class damaged function
  dp88_AI_ChargedTurret::Damaged(obj, damager, amount);
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Tesla_Coil::Timer_Expired ( GameObject* obj, int number )
{
  if (number == TIMER_TESLACOIL_DECREMENT_CHARGE)
  {
    if ( m_chargeLevel > 0 )
      m_chargeLevel--;

    // Have we dropped out of supercharged status?
    int supercharge_chargesRequired = (dp88_AI_ChargedTurret::checkPowerState(obj)) ? m_chargesSupercharge : m_chargesPowerOn + m_chargesSupercharge;
    if ( m_isSupercharged && m_chargeLevel < supercharge_chargesRequired )
      setSuperchargedState(obj, false);
  }

  // Call base class timer expired
  dp88_AI_ChargedTurret::Timer_Expired(obj, number);
}

// -------------------------------------------------------------------------------------------------

// Returns true if the base is powered, if the defence does not require power, or
// if the defence has been charged sufficiently to provide base power
bool dp88_AR_Tesla_Coil::checkPowerState ( GameObject* obj )
{
  return ( dp88_AI_ChargedTurret::checkPowerState(obj) || m_chargeLevel >= Get_Int_Parameter("Charges_Power_On") );
}

// -------------------------------------------------------------------------------------------------

// Set the supercharged state
void dp88_AR_Tesla_Coil::setSuperchargedState(GameObject* obj, bool state)
{
  if ( !m_superchargedWeapon.Is_Empty() )
  {
    // If we are setting supercharged to true then switch weapons
    if ( state )
    {
      Commands->Select_Weapon(obj, m_superchargedWeapon);
      primary_maxRange = m_superchargedWeaponRange;
    }
    else
    {
      Commands->Select_Weapon(obj, m_defaultWeapon);
      primary_maxRange = m_defaultWeaponRange;
    }

    m_isSupercharged = state;
  }
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_AR_Tesla_Coil> dp88_AR_Tesla_Coil_Registrant(
  "dp88_AR_Tesla_Coil",
  "Priority_Infantry=1.0:float,"
  "Splash_Infantry=0:int,"
  "Priority_Light_Vehicle=5.0:float,"
  "Priority_Heavy_Vehicle=7.0:float,"
  "Priority_VTOL=5.0:float,"
  "Min_Attack_Range=0:int,"
  "Max_Attack_Range=80:int,"
  "Animation_Model:string,"
  "Animation_Model_Bone:string,"
  "Animation:string,"
  "Animation_Idle_Start_Frame:int,"
  "Animation_Idle_End_Frame:int,"
  "Animation_Unpowered_Start_Frame:int,"
  "Animation_Unpowered_End_Frame:int,"
  "Animation_Charge_Start_Frame:int,"
  "Animation_Charge_End_Frame:int,"
  "Charge_Sound:string,"
  "Charge_Warhead=Tesla:string,"
  "Charge_Duration=5:int,"
  "Charges_Supercharge=1:int,"
  "Charges_Power_On=2:int,"
  "Supercharged_Weapon_Powerup_Preset:string,"
  "Supercharged_Weapon_Range=100:int,"
  "Modifier_Distance=0.25:float,"
  "Modifier_Target_Damage=0.1:float,"
  "Modifier_Target_Value=0.05:float,"
  "Requires_Power=1:int,"
  "Debug=0:int,"
  "Detects_Stealth=1:int"
);

// IFV
ScriptRegistrant<dp88_AR_IFV> dp88_AR_IFV_Registrant( "dp88_AR_IFV", "Turret_Frames_Animation=v_all_ifv.v_all_ifv:string,Switch_Time=10:int,Switching_Anim_Frame=2:int,Keyhook=VDeploy:string,String:int,Color:vector3,Sound:string" );
ScriptRegistrant<dp88_AR_IFVDriver> dp88_AR_IFVDriver_Registrant( "dp88_AR_IFVDriver", "IFV_ModelAnimFrame=1:int,IFV_WeaponPowerup_Rookie=null:string,IFV_WeaponPowerup_Veteran=null:string,IFV_WeaponPowerup_Elite=null:string" );
