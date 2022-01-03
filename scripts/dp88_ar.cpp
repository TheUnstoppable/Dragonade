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
#include "dp88_ar.h"
#include "ms.h"
#include "Definition.h"
#include "PurchaseSettingsDefClass.h"
#include "SoldierGameObj.h"
#include "VehicleGameObj.h"
#include "VehicleGameObjDef.h"
#include "MoveablePhysClass.h"
#include "SList.h"
#include "OffenseObjectClass.h"
#include "WeaponBagClass.h"

/*------------------------
Game Controller
--------------------------*/

dp88_AR_GameController::dp88_AR_GameController()
{
  created = false;

  team0_countryID = 0;
  team1_countryID = 0;

  mirageTank_disguisePresets[0] = 0;
  mirageTank_disguisePresets[1] = 0;
  mirageTank_disguisePresets[2] = 0;
}

// -------------------------------------------------------------------------------------------------

dp88_AR_GameController::~dp88_AR_GameController()
{}

void dp88_AR_GameController::Created( GameObject *obj )
{
  dp88_Camo_Controller::Created(obj);

	//Console_Output ( "Created dp88_AR_GameController\n" );
	created = true;
	
	// Check we have not disabled all soviet countries
	if ( Get_Int_Parameter ( "enableCountry_Russia" ) == 0
		&& Get_Int_Parameter ( "enableCountry_Cuba" ) == 0
		&& Get_Int_Parameter ( "enableCountry_Iraq" ) == 0
		&& Get_Int_Parameter ( "enableCountry_Libya" ) == 0 )
	{
		Console_Input ( "msg GAME CONTROLLER ERROR: All Soviet countries are disabled, defaulting to Russia!" );
		team0_countryID = 1;
	}

	// Check we have not disabled all allied countries
	if ( Get_Int_Parameter ( "enableCountry_America" ) == 0
		&& Get_Int_Parameter ( "enableCountry_France" ) == 0
		&& Get_Int_Parameter ( "enableCountry_Germany" ) == 0
		&& Get_Int_Parameter ( "enableCountry_GreatBritain" ) == 0
		&& Get_Int_Parameter ( "enableCountry_Korea" ) == 0 )
	{
		Console_Input ( "msg GAME CONTROLLER ERROR: All Allied countries are disabled, defaulting to America!" );
		team1_countryID = 1;
	}
	
	// Loop around until we get a non-disabled country ID for Soviets
	while ( team0_countryID == 0 )		
	{
		team0_countryID = (short)Commands->Get_Random_Int ( 1, 5 ); // Soviet, 1 to 4

		if ( team0_countryID == 1 && Get_Int_Parameter ( "enableCountry_Russia" ) == 0 )
			team0_countryID = 0;
		
		else if ( team0_countryID == 2 && Get_Int_Parameter ( "enableCountry_Cuba" ) == 0 )
			team0_countryID = 0;
		
		else if ( team0_countryID == 3 && Get_Int_Parameter ( "enableCountry_Iraq" ) == 0 )
			team0_countryID = 0;
		
		else if ( team0_countryID == 4 && Get_Int_Parameter ( "enableCountry_Libya" ) == 0 )
			team0_countryID = 0;
	}

	// Loop around until we get a non-disabled country ID for Allies
	while ( team1_countryID == 0 )		
	{
		team1_countryID = (short)Commands->Get_Random_Int ( 1, 5 ); // Allies, 1 to 5

		if ( team1_countryID == 1 && Get_Int_Parameter ( "enableCountry_America" ) == 0 )
			team1_countryID = 0;
		
		else if ( team1_countryID == 2 && Get_Int_Parameter ( "enableCountry_France" ) == 0 )
			team1_countryID = 0;
		
		else if ( team1_countryID == 3 && Get_Int_Parameter ( "enableCountry_Germany" ) == 0 )
			team1_countryID = 0;
		
		else if ( team1_countryID == 4 && Get_Int_Parameter ( "enableCountry_GreatBritain" ) == 0 )
			team1_countryID = 0;

		else if ( team1_countryID == 5 && Get_Int_Parameter ( "enableCountry_Korea" ) == 0 )
			team1_countryID = 0;
	}

	// Output Soviet country name and cmsg the team what their special ability is
	StringClass countriesMessage ( "The Soviets are playing as ", true );
	StringClass sovietCountryMessage ( true );
	StringClass alliedCountryMessage ( true );
	switch ( team0_countryID )
	{
		case 1:
			countriesMessage += "Russia";
			sovietCountryMessage = "As the Russians you can use the powerful Tesla Tank to wipe out those allied scum!";
			break;
		case 2:
			countriesMessage += "Cuba";
			sovietCountryMessage = "As the Cubans you can use the insane but deadly Terrorist to bomb your enemies!";
			break;
		case 3:
			countriesMessage += "Iraq";
			sovietCountryMessage = "As the Iraqis you can use the deadly Desolator to irradiate the allied scum!";
			break;
		case 4:
			countriesMessage += "Libya";
			sovietCountryMessage = "As the Libiyans you can use the deadly Demolition Truck to destroy enemy forces!";
			break;
	}


	// Output Allied country name and cmsg the team what their special ability is
	countriesMessage += " and the Allies are playing as ";
	switch ( team1_countryID )
	{
		case 1:
			countriesMessage += "America";
			alliedCountryMessage = "As the Americans you can use paradrops to surprise the Soviet forces!";
			break;
		case 2:
			countriesMessage += "France";
			alliedCountryMessage = "As the French your base is well defended by the powerful Grand Cannon";
			break;
		case 3:
			countriesMessage += "Germany";
			alliedCountryMessage = "As the Germans you can use the Tank Destroyer to easily wipe out Soviet tanks.";
			break;
		case 4:
			countriesMessage += "Great Britain";
			alliedCountryMessage = "As the British you can use snipers to pick off enemy infantry from distance";
			break;
		case 5:
			countriesMessage += "Korea";
			alliedCountryMessage = "As the Koreans you can use Black Eagles to rain death from the skies!";
			break;
	}

	// Send public countries message and team country messages
	Send_Message ( 255,255,255, countriesMessage.Peek_Buffer() );
	Send_Message_Team( 0, 255,69,0, sovietCountryMessage.Peek_Buffer() );
	Send_Message_Team( 1, 30,114,255, alliedCountryMessage.Peek_Buffer() );


	// Get mirage tank disguise preset names
	const char* mtDisguise = Get_Parameter( "MirageTank_disguisePreset_1" );
	if ( strcmp ( mtDisguise, "null" ) != 0 && Is_Valid_Preset ( mtDisguise ) )
	{
		mirageTank_disguisePresets[0] = new char[strlen(mtDisguise)+1];
		strcpy ( mirageTank_disguisePresets[0], mtDisguise );
	}

	mtDisguise = Get_Parameter( "MirageTank_disguisePreset_2" );
	if ( strcmp ( mtDisguise, "null" ) != 0 && Is_Valid_Preset ( mtDisguise ) )
	{
		mirageTank_disguisePresets[1] = new char[strlen(mtDisguise)+1];
		strcpy ( mirageTank_disguisePresets[1], mtDisguise );
	}

	mtDisguise = Get_Parameter( "MirageTank_disguisePreset_3" );
	if ( strcmp ( mtDisguise, "null" ) != 0 && Is_Valid_Preset ( mtDisguise ) )
	{
		mirageTank_disguisePresets[2] = new char[strlen(mtDisguise)+1];
		strcpy ( mirageTank_disguisePresets[2], mtDisguise );
	}
}


void dp88_AR_GameController::Custom ( GameObject *obj, int type, int param, GameObject *sender )
{
}


void dp88_AR_GameController::Timer_Expired( GameObject *obj, int number )
{
}


void dp88_AR_GameController::Destroyed ( GameObject* obj )
{
	// Clean up memory for Mirage Tank disguises
	if ( mirageTank_disguisePresets[0] != NULL )
	{
		delete [] mirageTank_disguisePresets[0];
		mirageTank_disguisePresets[0] = NULL;
	}
	if ( mirageTank_disguisePresets[1] != NULL )
	{
		delete [] mirageTank_disguisePresets[1];
		mirageTank_disguisePresets[1] = NULL;
	}
	if ( mirageTank_disguisePresets[2] != NULL )
	{
		delete [] mirageTank_disguisePresets[2];
		mirageTank_disguisePresets[2] = NULL;
	}
}




/*------------------------
Default script for all vehicles in AR
--------------------------*/

void dp88_AR_Vehicle::Created(GameObject *obj)
{
	//Console_Output ( "Created dp88_AR_Vehicle\n" );
	pilotID = 0;
	attackingTerrorDroneID = 0;
	
	dead = false;
}


void dp88_AR_Vehicle::Killed( GameObject *obj, GameObject *killer )
{
	/*dead = true;


	// If we have a pilot make sure they know they are no longer a pilot
	if ( pilotID != 0 )
	{
		Commands->Send_Custom_Event( obj,Commands->Find_Object(pilotID),CUSTOM_PILOTED_VEHICLE_ID,0,0 );
		pilotID = 0;
	}

	// If a Terror Drone is attacking us let them know we died...
	if ( attackingTerrorDroneID != 0 )
	{
		Commands->Send_Custom_Event( obj,Commands->Find_Object(attackingTerrorDroneID),CUSTOM_TD_TARGET_DIED,0,0 );
		attackingTerrorDroneID = 0;
	}*/
}



void dp88_AR_Vehicle::Custom( GameObject *obj, int type, int param, GameObject *sender )
{/*
	// Look for vehicle entry
	if ( type == CUSTOM_EVENT_VEHICLE_ENTERED )
	{
		if ( pilotID == 0 )
		{
			pilotID = Commands->Get_ID(sender);

			// Send a custom to the driver to make their scripts work properly in
			// vehicle mode, and give them our ID. They will report their veterancy level
			// in return, if nessicary.
			Commands->Send_Custom_Event( obj, sender, CUSTOM_PILOTED_VEHICLE_ID, Commands->Get_ID(obj), 0 );
		}
	}


	// Look for vehicle exit
	else if ( type == CUSTOM_EVENT_VEHICLE_EXITED )
	{
		if ( Commands->Get_ID(sender) == pilotID )
		{
			pilotID = 0;

			// Send a custom to the driver to let them know they got out
			Commands->Send_Custom_Event( obj, sender, CUSTOM_PILOTED_VEHICLE_ID, 0, 0 );
		}
	}
	
	
	// Look for a message from the attacking terror drone telling us it died
	else if ( type == CUSTOM_TD_TD_DIED )
	{
		attackingTerrorDroneID = 0;
	}*/
}


void dp88_AR_Vehicle::Timer_Expired( GameObject *obj, int number )
{
	// Terror Drone damage timer
	/*if ( number == TIMER_TD_DO_DAMAGE )
	{
		if ( attackingTerrorDroneID != 0 && !dead )
		{
			Commands->Apply_Damage( obj, 40.0, "Steel", Get_Vehicle_Driver ( Commands->Find_Object( attackingTerrorDroneID ) ) );
			Commands->Start_Timer( obj, this, 1.0f, TIMER_TD_DO_DAMAGE );
		}
	}*/
}


void dp88_AR_Vehicle::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	// If damage is repairing, and we are being attacked by a TD, kill the TD
	/*if ( amount < 0 && attackingTerrorDroneID != 0 )
		Commands->Send_Custom_Event( obj, Commands->Find_Object(attackingTerrorDroneID), CUSTOM_TD_TARGET_REPAIRED, 1, 0 );

	
	// If the damage is from the Terror Drone gun set the attacker and start anim
	// Can only be attacked by one TD at a time, and only by opposite team
	const static char *TD_Comparison = "Terror Drone";
	if ( strstr( Commands->Get_Preset_Name( Get_Vehicle(damager) ), TD_Comparison ) != NULL
		&& ( Get_Object_Type(damager) != Get_Object_Type(obj) )
		&& attackingTerrorDroneID == 0
		&& !dead )
	{
		attackingTerrorDroneID = Commands->Get_ID( Get_Vehicle(damager) );
		Commands->Send_Custom_Event( obj, Get_Vehicle(damager), CUSTOM_TD_TARGET_ID, 1, 0 );
		Commands->Set_Animation( obj, Get_Parameter("TD_attack_animName"), false, 0, Get_Float_Parameter("TD_attack_firstFrame"), Get_Float_Parameter("TD_attack_lastFrame"), false );
		Commands->Start_Timer( obj, this, 0.1f, TIMER_TD_DO_DAMAGE );
	}*/
}


void dp88_AR_Vehicle::Animation_Complete ( GameObject *obj, const char *animation_name )
{
	// Restart TD Attack Anim
	/*if ( attackingTerrorDroneID != 0 )
		Commands->Set_Animation( obj, Get_Parameter("TD_attack_animName"), false, 0, Get_Float_Parameter("TD_attack_firstFrame"), Get_Float_Parameter("TD_attack_lastFrame"), false );
		*/
}








/*------------------------
Deployable Infantry scripts
--------------------------*/
void dp88_AR_Deployable_Infantry::Created( GameObject *obj )
{
	//Console_Output ( "Created dp88_AR_Deployable_Infantry\n" );
	objectID = Commands->Get_ID ( obj );
	deployed = false;
	undeployedWeapon = NULL;
	deployedObjectId = NULL;
	lastDeploy = 0;
	currentVetLevel = 0;
	
	// Check deployedObjectPreset is a valid preset name
	if ( !Is_Valid_Preset(Get_Parameter ("deployedObjectPreset")) )
	{
		Send_Message(255,255,255,"dp88_AR_Deployable_Infantry :: Script aborting due to invalid deployedObjectPreset");
		Destroy_Script();
		return;
	}
	
	
	// Validate the weapon powerups
	hasRookieWeaponPowerup = (strcmp ( Get_Parameter( "deployedWeaponPowerup" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter( "deployedWeaponPowerup" ) )) ? true : false; 
	hasVeteranWeaponPowerup = (strcmp ( Get_Parameter( "deployedWeaponPowerup_veteran" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter( "deployedWeaponPowerup_veteran" ) )) ? true : false; 
	hasEliteWeaponPowerup = (strcmp ( Get_Parameter( "deployedWeaponPowerup_elite" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter( "deployedWeaponPowerup_elite" ) )) ? true : false; 
	
	// Store undeployed skin & armour types
	strcpy_s ( undeployedSkinType, sizeof( undeployedSkinType ), Get_Skin ( obj ) );
	strcpy_s ( undeployedArmourType, sizeof( undeployedArmourType ), Get_Shield_Type ( obj ) );


	// Get cannot deploy string and sound IDs
	int soundId = 0;
	cannotDeployStringId = (Is_Valid_String_ID(Get_Int_Parameter("cannotDeployStringId"))) ? Get_Int_Parameter("cannotDeployStringId") : 0;
	if ( cannotDeployStringId && (soundId = Get_String_Sound_ID(cannotDeployStringId)) != 0 && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000 )
		cannotDeploySoundId = soundId;

	// Install keyhook
	if ( obj->As_SoldierGameObj() && Get_Player_ID ( obj ) >= 0 )
		InstallHook( Get_Parameter("deployKeyhook"), obj );
}


void dp88_AR_Deployable_Infantry::KeyHook()
{
	// Find object
	GameObject* obj = Commands->Find_Object ( objectID );
	if ( !obj || Get_Vehicle(obj) || obj->As_SoldierGameObj()->Is_On_Ladder() || obj->As_SoldierGameObj()->Is_In_Elevator() )
		return;

	if ( deployed )
	{
		// If deployed for less than undeployTime seconds dont allow undeploy
    float undeployTime = Get_Float_Parameter("undeployTime");
    if ( time(NULL) - lastDeploy < undeployTime )
    {
      StringClass str(true);
      str.Format("You cannot undeploy yet, you must wait at least %.*f seconds after deploying to undeploy.", (((int)undeployTime)==undeployTime)?0:2, undeployTime);
      Send_Message_Player(obj, DP88_RGB_WARNING_MSG, str);
      return;
    }

    Undeploy(obj);
	}
	else
	{
		// If deployed for less than deployTime seconds dont allow deploy
    float deployTime = Get_Float_Parameter("deployTime");
    if ( time(NULL) - lastDeploy < deployTime )
    {
      StringClass str(true);
      str.Format("You cannot deploy yet, you must wait at least %.*f seconds after undeploying to deploy again.",(((int)deployTime)==deployTime)?0:2, deployTime);
      Send_Message_Player(obj, DP88_RGB_WARNING_MSG, str);
      return;
    }

		// Get velocity of deployer
		Vector3 velocity = Get_Velocity(obj);

		// Find closest other armed object
		GameObject* closest = Get_Closest_Armed_Object_To_Object(obj);

		// Check if we can deploy
		if ( fabs(velocity.X) > 0.001 || fabs(velocity.Y) > 0.001 || fabs(velocity.Z) > 0.001
			|| (closest && Commands->Get_Distance(Commands->Get_Position(obj), Commands->Get_Position(closest)) < Get_Float_Parameter("deployedObjectSpaceRequired") ) )
		{
			const char *str = Get_Translated_String(cannotDeployStringId);
			Send_Message_Player(obj,153,204,25,str);
			delete[] str;
			if ( cannotDeploySoundId )
				Create_2D_Sound_Player(obj, Get_Definition_Name(cannotDeploySoundId));
		}
		else
		{
			Deploy(obj);
		}
	}
}


void dp88_AR_Deployable_Infantry::Killed( GameObject *obj, GameObject *killer )
{
	RemoveHook();
	if ( deployedObjectId != NULL )
		Commands->Destroy_Object ( Commands->Find_Object ( deployedObjectId ) );
	deployedObjectId = NULL;
}


void dp88_AR_Deployable_Infantry::Destroyed( GameObject *obj )
{
	if ( deployedObjectId != NULL )
		Commands->Destroy_Object ( Commands->Find_Object ( deployedObjectId ) );
	deployedObjectId = NULL;
}


void dp88_AR_Deployable_Infantry::Custom ( GameObject *obj, int type, int param, GameObject *sender )
{
	// We have been promoted, apply new weapon and/or armour types
	if ( sender == obj && type == CUSTOM_VETERANCY_PROMOTED )
	{
		// Remember old vet level
		int oldVetLevel = currentVetLevel;
		
		// Save new level
		currentVetLevel = param;
		
		/* Save new defaults for skin and armour type, unless we are deployed and
		they match the values we set for the old vet level, which implies they
		didn't recieve a new armour type on promotion */
		if ( !deployed || strcmp(GetArmourType(oldVetLevel),Get_Skin(obj)) != 0 )
			strcpy_s ( undeployedSkinType, sizeof( undeployedSkinType ), Get_Skin ( obj ) );
		if ( !deployed || strcmp(GetArmourType(oldVetLevel),Get_Shield_Type(obj)) != 0 )
			strcpy_s ( undeployedArmourType, sizeof( undeployedArmourType ), Get_Shield_Type ( obj ) );
		
		// Set new armour / shield types and weapon if deployed
		if ( deployed )
		{
			const char* armourType = GetArmourType(currentVetLevel);
			if ( armourType )
			{
				Commands->Set_Shield_Type(obj, armourType);
				Set_Skin(obj, armourType);
			}


			// Grant new weapon if necessary
			bool grantWeapon = ((currentVetLevel == 2 && hasEliteWeaponPowerup )||( oldVetLevel == 0 && hasVeteranWeaponPowerup)) ? true : false;
			if ( grantWeapon )
			{
				// Do we need to remove an old deployed weapon?
				if ( (oldVetLevel == 1 && hasVeteranWeaponPowerup) || hasRookieWeaponPowerup )
				{
					// Remove old weapon
					const char* oldWeaponPowerup = GetWeaponPowerup(oldVetLevel);
					Remove_Weapon ( obj, Get_Powerup_Weapon ( oldWeaponPowerup ) );
				}

				// Grant and select new weapon
				const char* powerup = GetWeaponPowerup(currentVetLevel);
				Commands->Give_PowerUp( obj, powerup, true );
				Commands->Select_Weapon ( obj, Get_Powerup_Weapon ( powerup ) );
			}
		}
	}
}


// TEMPORARY - CHECK IF GI MOVES AWAY FROM SANDBAGS
void dp88_AR_Deployable_Infantry::Timer_Expired ( GameObject* obj, int number )
{
  if ( deployed && number == 154785 )
  {
    GameObject* deployedObject = Commands->Find_Object(deployedObjectId);
    float distance = Commands->Get_Distance(Commands->Get_Position(deployedObject),Commands->Get_Position(obj));

    // If more than 3m away, undeploy
    if ( deployedObject != NULL && distance > 1.5f )
    {
      Undeploy(obj);

      // Remove script to punish abusers
      Send_Message_Player(obj, DP88_RGB_ERROR_MSG, StringClass::getFormattedString("Deployment abuse detected, disabling deploy script... (distance from deployment: %.2fm)", distance));
      RemoveHook();
      Destroy_Script();
      return;
    }

    Commands->Start_Timer(obj,this,2.0f,154785);
  }
}


// Get armour type to set for the given veterancy level
const char* dp88_AR_Deployable_Infantry::GetArmourType ( int vetLevel )
{
  if ( vetLevel == 2 && strcmp ( Get_Parameter( "deployedArmourType_elite" ), "null" ) != 0 )
    return Get_Parameter ( "deployedArmourType_elite" );
  else if ( vetLevel >= 1 && strcmp ( Get_Parameter( "deployedArmourType_veteran" ), "null" ) != 0 )
    return Get_Parameter ( "deployedArmourType_veteran" );
  else if ( strcmp ( Get_Parameter( "deployedArmourType" ), "null" ) != 0 )
    return Get_Parameter ( "deployedArmourType" );
  return NULL;
}


// Get weapon powerup to grant for the given veterancy level
const char* dp88_AR_Deployable_Infantry::GetWeaponPowerup ( int vetLevel )
{
  if ( vetLevel == 2 && hasEliteWeaponPowerup )
    return Get_Parameter("deployedWeaponPowerup_elite");
  else if ( vetLevel >= 1 && hasVeteranWeaponPowerup )
    return Get_Parameter("deployedWeaponPowerup_veteran");
  else if ( hasRookieWeaponPowerup )
    return Get_Parameter("deployedWeaponPowerup");
  return NULL;
}

void dp88_AR_Deployable_Infantry::Deploy(GameObject* obj)
{
  if (deployed)
    return;

  lastDeploy = time(NULL);
  deployed = true;

  // Create the defensive structure
  GameObject* deployedObject = Commands->Create_Object ( Get_Parameter ( "deployedObjectPreset" ), Commands->Get_Position ( obj ) );
  deployedObjectId = Commands->Get_ID ( deployedObject );

  // Grant deployed weapon and select it for use
  const char* powerup = GetWeaponPowerup(currentVetLevel);
  if ( powerup )
  {
    Commands->Give_PowerUp( obj, powerup, true );
    Commands->Select_Weapon ( obj, Get_Powerup_Weapon ( powerup ) );
  }

  // Set armour & skin types
  const char* armourType = GetArmourType(currentVetLevel);
  if ( armourType )
  {
    Commands->Set_Shield_Type(obj, armourType);
    Set_Skin(obj, armourType);
  }

  // Disable loiters and vehicle entry
  if ( SoldierGameObj* sObj = obj->As_SoldierGameObj() )  // Should always be true, but safety first...
  {
    m_bCanDrive = sObj->Can_Drive_Vehicles(); // Save value for when we undeploy...
    sObj->Set_Can_Drive_Vehicles(false);
    Commands->Set_Loiters_Allowed(obj,false);
  }
  const char *sound = Get_Parameter("deploySound");
  if (sound[0])
  {
	  Commands->Create_Sound(sound, Commands->Get_Position(obj), obj);
  }

  // TEMP - Start timer to check position for script abuse
  Commands->Start_Timer(obj,this,2.0f,154785);
}

void dp88_AR_Deployable_Infantry::Undeploy(GameObject* obj)
{
  if (!deployed)
    return;

  lastDeploy = time(NULL);
  deployed = false;

  // Destroy the defensive structure
  if (NULL != deployedObjectId)
  {
    Commands->Destroy_Object(Commands->Find_Object(deployedObjectId));
    
    // Wake up any objects inside the box to prevent abusing the physics engine to
    // "walk on air" by moving forward, undeploying and redeploying (and repeating)
    // without waking the physics engine to check for terrain under the soldier
    Vector3 pos = Commands->Get_Position(obj);
    Vector3 extent(10.0f, 10.0f, 10.0f);
    OBBoxClass boundingBox(pos, extent);
    Wake_Up_Objects_In_OBBox(boundingBox);
  }

  // Remove deployed weapon
  const char* powerup = GetWeaponPowerup(currentVetLevel);
  if (powerup)
    Remove_Weapon(obj, Get_Powerup_Weapon(powerup));

  // Re-enable loiters and vehicle entry
  if (SoldierGameObj* sObj = obj->As_SoldierGameObj())  // Should always be true, but safety first...
  {
    sObj->Set_Can_Drive_Vehicles(m_bCanDrive);
    Commands->Set_Loiters_Allowed(obj,true);
  }
  const char *sound = Get_Parameter("undeploySound");
  if (sound[0])
  {
	  Commands->Create_Sound(sound, Commands->Get_Position(obj), obj);
  }

  // Reset armour & skin types
  Set_Skin(obj, undeployedSkinType);
  Commands->Set_Shield_Type(obj, undeployedArmourType);
}




/*------------------------
Chrono Legionnaire Scripts
--------------------------*/

void dp88_AR_CLEG::Created(GameObject *obj)
{
	//Console_Output ( "Created dp88_AR_CLEG\n" );
	phasing = false;
	phasingBack = false;
	timeRemaining = 180;
	zPosDropObj = 0;
	dropObjID = 0;
}

void dp88_AR_CLEG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if ( Commands->Get_ID ( damager ) == Commands->Get_ID ( obj ) && amount == 0.0 )
	{
		Vector3 position = Commands->Get_Position ( obj );

		// If damage = 0 and damager is ourselves then phase to the top plane, or phase
		// back if we are already there.
		if ( !phasing )
		{
			position.Z = 90;
			Commands->Set_Position ( obj, position );
			timeRemaining = 180;

			// Start the clock ticking
			Commands->Start_Timer ( obj, this, 1.0f, TIMER_CLEG_PHASEDEATH );
			phasing = true;
		}
		else if ( dropObjID == 0 )
		{
			position.Z -= 5.0;
			// Time to phase back down again, create the object and start the timer
			// to wait for it to hit the floor
			phasingBack = true;
			GameObject *dropper;
			dropper = Commands->Create_Object( "CLEG_DropObj", position );
			dropObjID = Commands->Get_ID(dropper);
			zPosDropObj = 0;
			Commands->Start_Timer ( obj, this, 0.2f, TIMER_CLEG_CHECKDROPOBJ );
			Send_Message_Player( obj, 153, 204, 25, "Phasing in...." );
		}
	}
}

void dp88_AR_CLEG::Killed(GameObject *obj, GameObject *killer)
{
	if ( dropObjID != 0 )
		Commands->Destroy_Object( Commands->Find_Object( dropObjID ) );
}

void dp88_AR_CLEG::Timer_Expired( GameObject *obj, int number )
{
	if ( number == TIMER_CLEG_CHECKDROPOBJ && dropObjID != 0 )
	{
		Vector3 position = Commands->Get_Position( Commands->Find_Object( dropObjID ) );
		
		if ( (int)zPosDropObj == (int)position.Z )
		{
			Commands->Destroy_Object( Commands->Find_Object( dropObjID ) );
			position.Z += 1;
			Commands->Set_Position( obj, position );
			phasing = false;
			phasingBack = false;
			dropObjID = 0;
			zPosDropObj = 0;
		}
		else
		{
			zPosDropObj = (int)position.Z;
			Commands->Start_Timer( obj, this, 0.2f, TIMER_CLEG_CHECKDROPOBJ );
		}
	}

	else if ( number == TIMER_CLEG_PHASEDEATH )
	{
		if ( phasing == true )
		{
			Commands->Start_Timer ( obj, this, 1.0f, TIMER_CLEG_PHASEDEATH );
			timeRemaining -= 1;
		}

		if ( timeRemaining == 0 && phasing == true && phasingBack != true )
		{
			char deathMSG[128];
			sprintf ( deathMSG, "You have been phased out too long and have died..." );
			Send_Message_Player( obj, 153, 204, 25, deathMSG );
			
			Commands->Apply_Damage( obj, 500.0, "Death", obj );
		}
		else if ( phasing == true && timeRemaining % 20 == 0 || timeRemaining == 10 || timeRemaining == 5 )
		{
			char deathTimer[128];
			sprintf ( deathTimer, "WARNING: You have %d seconds to phase back before you die...", timeRemaining );
			Send_Message_Player( obj, 153, 204, 25, deathTimer );
		}
	}
}

void dp88_AR_CLEG_target::Created(GameObject *obj)
{
	currentResistance = Get_Int_Parameter("resistance")*10;
	lastResistanceCheck = 0;
	effectObjectId = 0;
}

void dp88_AR_CLEG_target::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	const static char *Comparison = "CLEG_Weapon";
	if ( damager
		&& Get_Current_Weapon(damager)
		&& strstr( Get_Current_Weapon(damager), Comparison ) != NULL
		&& ( !Get_Vehicle(damager) )								// Check they are not in a vehicle, as current_weapon will not show this
		&& ( Get_Object_Type(damager) != Get_Object_Type(obj) )		// Check they are not on the same team
		&& currentResistance > 0 )									// Check if we have any CLEG resistance remaining
	{
		// If this is the first damage we have taken in this CLEG attack setup
		// all variables
		if ( lastResistanceCheck == 0 )
		{
			lastResistanceCheck = currentResistance;

			// Start timer to see if we can be released
			Commands->Start_Timer( obj, this, 1.0f, TIMER_CLEG_CHECKRELEASETARGET );

			// Create instance of CLEG effect preset (if applicable)
			const char* effectPreset = Get_Parameter("clegEffectPreset");
			if ( Is_Valid_Preset(effectPreset) )
			{
				GameObject *effectObject = Commands->Create_Object ( effectPreset, Commands->Get_Position(obj) );
				if ( effectObject )
					effectObjectId = Commands->Get_ID ( effectObject );
			}

			// Disable controls for target
			Commands->Control_Enable( obj, false );
		}

		// Reduce remaining resistance
		currentResistance--;
		if ( currentResistance == 0 )
		{
			Commands->Apply_Damage( obj, 5000.0, "Death", damager );

			// Make target 'vanish'
			Commands->Set_Model( obj, "null" );
		}
	}
}

void dp88_AR_CLEG_target::Killed(GameObject *obj, GameObject *killer)
{
	/* Destroy effect object if it exists */
	if ( effectObjectId != 0 )
	{
		GameObject *effectObject = Commands->Find_Object(effectObjectId);
		if ( effectObject )
			Commands->Destroy_Object(effectObject);
		effectObjectId = 0;
	}
}

void dp88_AR_CLEG_target::Timer_Expired( GameObject *obj, int number )
{
	if ( number == TIMER_CLEG_CHECKRELEASETARGET )
	{
		/* If resistance since last check has not changed then CLEG has stopped attacking
		us so free the unit, reset its resistance and destroy any effect object */
		if ( lastResistanceCheck == currentResistance )
		{
			// Reset resistance
			currentResistance = Get_Int_Parameter("resistance")*10;
			lastResistanceCheck = 0;
			
			// Re-enable controls
			Commands->Control_Enable( obj, true );

			/* Destroy effect object if it exists */
			if ( effectObjectId != 0 )
			{
				GameObject *effectObject = Commands->Find_Object(effectObjectId);
				if ( effectObject )
					Commands->Destroy_Object(effectObject);
				effectObjectId = 0;
			}
		}

		/* Otherwise we are still being attacked, update the last check variable and restart
		the timer... */
		else if ( currentResistance > 0 )
		{
			// If we are still being targetted by CLEG(s) start the timer again
			lastResistanceCheck = currentResistance;
			Commands->Start_Timer( obj, this, 1.0f, TIMER_CLEG_CHECKRELEASETARGET );
		}
	}
}








/*------------------------
AR Miner Script - Base Class

This base class is used by both dp88_AR_Chrono_Miner and dp88_AR_War_Miner
and implements common functionality used by both scripts
--------------------------*/

void dp88_Ore_Miner::Created ( GameObject *obj )
{
  m_aiState       = MINER_AISTATE_IDLE;
  m_oreMined      = 0;
  m_oreValue      = 0;

  m_bUseAI        = ( Get_Int_Parameter ( "Use_AI" ) == 1 ) ? true : false;
  m_resourceName  = Get_Parameter("Resource_Name");
  m_oreCapacity   = Get_Int_Parameter("Ore_Capacity");
  m_oreMiningTime = Get_Float_Parameter("Ore_Mining_Time");
  m_oreDumpTime   = Get_Float_Parameter("Ore_Dump_Time");

  m_oreFieldId    = 0;
  m_oreFieldRand  = 0;

  // Load animation settings
  m_animations[MINER_ANIM_IDLE]     = Get_Parameter("Idle_Animation");
  m_animations[MINER_ANIM_MINING]   = Get_Parameter("Mining_Animation");
  m_animations[MINER_ANIM_DUMPING]  = Get_Parameter("Dump_Animation");
  m_animSounds[MINER_ANIM_IDLE]     = NULL;
  m_animSounds[MINER_ANIM_MINING]   = Get_Parameter("Mining_Sound");
  m_animSounds[MINER_ANIM_DUMPING]  = Get_Parameter("Dump_Sound");

  for ( int i = MINER_ANIM_IDLE; i <= MINER_ANIM_DUMPING; ++i )
  {
    if (m_animations[i] != NULL && strlen(m_animations[i]) <= 0)
      m_animations[i] = NULL;
    if (m_animSounds[i] != NULL && strlen(m_animSounds[i]) <= 0)
      m_animSounds[i] = NULL;
  }

  // Set the initial animation state
  UpdateAnimation(obj, MINER_ANIM_IDLE);

  /* For AI miners send a message to ourselves to start searching for an ore field - note the delay
  * which is required to prevent the default harvester AI taking over */
  if ( m_bUseAI )
    Commands->Send_Custom_Event( obj, obj, CUSTOM_MINER_AI_SEARCH_FOR_ORE, 1, (float)Get_Int_Parameter("AI_Init_Delay") );
}

// -------------------------------------------------------------------------------------------------

void dp88_Ore_Miner::Custom ( GameObject *obj, int type, int param, GameObject *sender )
{
  // Message from an ore field indicating we have entered it
  if ( type == CUSTOM_MINER_ENTERED_ORE_FIELD )
  {
    EnteredOreField(obj, sender);
  }


  // Message from an ore field indicating we have left it
  else if ( type == CUSTOM_MINER_EXITED_ORE_FIELD && Commands->Get_ID(sender) == m_oreFieldId )
  {
    ExitedOreField(obj, sender);
  }


  // Message to ourselves to mine another ore load
  else if ( type == CUSTOM_MINER_INCREASE_ORE_LOAD && param == m_oreFieldRand )
  {
    GameObject* pOreField = Commands->Find_Object(m_oreFieldId);
    dp88_Ore_Field* pOreFieldScript = (!pOreField) ? NULL : (dp88_Ore_Field *)(Find_Script_On_Object(pOreField,"dp88_Ore_Field"));

    // Unless we are already full increase our load level and the value of our load
    if ( m_oreMined < m_oreCapacity && pOreFieldScript )
    {
      if ( pOreFieldScript->RemoveOre(1) == 1 )
      {
        // Play the mining sound
        if ( m_animSounds[MINER_ANIM_MINING] != NULL )
          Commands->Create_Sound(m_animSounds[MINER_ANIM_MINING],Commands->Get_Position(obj),obj);

        m_oreMined++;
        m_oreValue += pOreFieldScript->GetOreValue();
      }
    }

    // If we are still not full start send a delayed custom to increase it again
    if ( m_oreMined < m_oreCapacity && pOreFieldScript && (pOreFieldScript->IsInfinite() || pOreFieldScript->NumOreUnits() > 0) )
      Commands->Send_Custom_Event ( obj, obj, CUSTOM_MINER_INCREASE_ORE_LOAD, m_oreFieldRand, m_oreMiningTime );


    // Otherwise we are full of ore... or the ore field is depleted...
    else
    {
      UpdateAnimation(obj, MINER_ANIM_IDLE);

      // If using the AI start driving to the refinery
      if ( m_bUseAI )
        ReturnToRefinery(obj);

      // Or, if we are a player driven miner, tell the driver we are full
      else if ( Get_Vehicle_Driver(obj) != NULL )
      {
        if ( m_oreMined < m_oreCapacity )
        {
          StringClass str;
          str.Format("The %s field is depleted, find another %s field or dock at the Refinery to process the %s you have collected so far...",m_resourceName,m_resourceName,m_resourceName);
          Send_Message_Player ( Get_Vehicle_Driver(obj), DP88_RGB_GENERAL_MSG, str);
        }
        else
        {
          StringClass str;
          str.Format("Fully loaded with %s, dock at the Refinery to process the %s into credits",m_resourceName,m_resourceName);
          Send_Message_Player ( Get_Vehicle_Driver(obj), DP88_RGB_GENERAL_MSG,str);
        }
      }
    }
  }


  // Message from the ore dump zone notifying us that we have entered it. If
  // we have ore to unload then immobilize the vehicle and begin unloading it
  else if ( type == CUSTOM_MINER_ENTERED_DUMP_ZONE && m_oreMined > 0 )
  {
    // Inform driver we are unloading
    if ( Get_Vehicle_Driver(obj) != NULL )
    {
      StringClass str;
      str.Format("Unloading %s, please stand by...", m_resourceName);
      Send_Message_Player ( Get_Vehicle_Driver(obj), DP88_RGB_GENERAL_MSG, str);
    }

    // Set AI state
    if ( m_bUseAI )
      m_aiState = MINER_AISTATE_UNLOADING_ORE;

    // Send a timed event to notify us when the dump is complete and then call the docking event
    Commands->Send_Custom_Event(obj, obj, CUSTOM_MINER_UNLOAD_ORE_COMPLETE, 0, m_oreDumpTime);
    DockedAtRefinery(obj);
  }


  // Message to ourselves to indicate ore unloading is complete, grant money
  // to the team and set off to collect some more ore
  else if ( type == CUSTOM_MINER_UNLOAD_ORE_COMPLETE )
  {
    // Inform the driver that we have finished unloading
    if ( Get_Vehicle_Driver(obj) != NULL )
    {
      StringClass message(true);
      StringClass s = m_resourceName;
      s[0] = (char)toupper(s[0]);
      message.Format ("%s unloaded successfully, you have earned %d credits for each player and %d points for yourself",s.Peek_Buffer(), m_oreValue, m_oreValue/10 );
      Send_Message_Player ( Get_Vehicle_Driver(obj), DP88_RGB_GENERAL_MSG, message );

      Commands->Give_Points(Get_Vehicle_Driver(obj),(float)m_oreValue/10.0f,false);
    }

    // Grant money to team and reset ore load level
    Commands->Give_Money ( obj, (float)m_oreValue, true );
	MS_AccessHelper::Give_Bot_Credits(Get_Object_Type(obj), (float)m_oreValue);
    m_oreMined = 0;
    m_oreValue = 0;

    // Call UndockedFromRefinery
    UndockedFromRefinery(obj);
  }


  // AI message to search for an ore field...
  else if ( type == CUSTOM_MINER_AI_SEARCH_FOR_ORE )
  {
    DriveToOreField(obj);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_Ore_Miner::Action_Complete ( GameObject *obj, int action_id, ActionCompleteReason complete_reason )
{
  // If the completed action was RETURN_TO_REFINERY then set the AI state to
  // docking and call DockAtRefinery();
  if ( action_id == MINER_ACTIONID_RETURN_TO_REFINERY )
  {
    //Console_Output ( "dp88_Ore_Miner: Arrived at refinery... start docking\n" );
    m_aiState = MINER_AISTATE_DOCK_AT_REFINERY;
    DockAtRefinery(obj);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_Ore_Miner::DriveToOreField ( GameObject *obj )
{
  m_aiState = MINER_AISTATE_SEARCH_FOR_ORE;

  GameObject* bestField = NULL;
  double bestFieldScore = 0.0;

  // \todo Check number of units available in an ore field and find one with a sufficient quantity - 
  //       ideally 1.5x our capacity to avoid it being depleted before we get there!
  SList<GameObject> oreFields;
  Find_All_Objects_With_Script("dp88_Ore_Field", oreFields);

  for ( SLNode<GameObject>* oreFieldNode = oreFields.Head(); oreFieldNode != NULL; oreFieldNode = oreFieldNode->Next() )
  {
    if (GameObject* oreField = oreFieldNode->Data())
    {
      dp88_Ore_Field* pOreFieldScript = (dp88_Ore_Field *)(Find_Script_On_Object(oreField,"dp88_Ore_Field"));
      if (!pOreFieldScript || !pOreFieldScript->IsSuitableForAI())
        continue;

      int availableUnits = min(m_oreCapacity, (pOreFieldScript->IsInfinite()?m_oreCapacity:(int)pOreFieldScript->NumOreUnits()));
      if (0 == availableUnits)
        continue;

      Vector3 fieldPosition = Commands->Get_Position(oreField);
      double fieldScore = (availableUnits*pOreFieldScript->GetOreValue()) / (0.5*Commands->Get_Distance(Commands->Get_Position(oreField), Commands->Get_Position(obj)));

      if (fieldScore > bestFieldScore || NULL == bestField)
      {
        bestField = oreField;
        bestFieldScore = fieldScore;
      }
    }
  }

  if (NULL == bestField)
  {
    //Console_Output ( "dp88_Ore_Miner: No ore fields available..." );

    // No ore fields are available at the moment... send ourselves a message to have another look in 5 seconds...
    Commands->Send_Custom_Event(obj, obj, CUSTOM_MINER_AI_SEARCH_FOR_ORE, 1, (float)5.0f);
    return;
  }

  //Console_Output ( "dp88_Ore_Miner: Driving to location: %.2f, %.2f, %.2f\n", position.X, position.Y, position.Z );

  /* Setup parameters and get going! */
  m_aiState = MINER_AISTATE_DRIVE_TO_ORE;
  ActionParamsStruct params;
  params.Set_Basic(this, 100.0f, MINER_ACTIONID_DRIVE_TO_ORE);
  params.Set_Movement(Commands->Get_Position(bestField), 1.0f, 1.0f);
  params.MovePathfind = true;
  params.ShutdownEngineOnArrival = true;
  params.AttackActive = false;
  Commands->Action_Goto(obj, params);
}

// -------------------------------------------------------------------------------------------------

void dp88_Ore_Miner::EnteredOreField ( GameObject *obj, GameObject* oreField )
{
  // Ignore this if we are already full
  if ( m_oreMined >= m_oreCapacity )
    return;

  // Check if this is a valid ore field
  dp88_Ore_Field* pOreFieldScript = (dp88_Ore_Field *)(Find_Script_On_Object(oreField,"dp88_Ore_Field"));
  if (!pOreFieldScript || (m_bUseAI && !pOreFieldScript->IsSuitableForAI()))
    return;

  // Check this field isn't empty (we might have just driven into it on the way to another one, or
  // some thief might have stolen the resources!)
  if (!pOreFieldScript->IsInfinite() && 0 == pOreFieldScript->NumOreUnits())
  {
    if (m_bUseAI)
      DriveToOreField(obj);

    return;
  }

  // Set AI state, notify driver or abort if neither AI controlled nor player driven...
  if (m_bUseAI)
  {
    m_aiState = MINER_AISTATE_COLLECTING_ORE;
    Commands->Action_Reset ( obj, 101.0f );
  }
  else if (NULL != Get_Vehicle_Driver(obj))
  {
    StringClass str;
    str.Format("Collecting %s...",m_resourceName);
    Send_Message_Player ( Get_Vehicle_Driver(obj), DP88_RGB_GENERAL_MSG, str );
  }
  else
    return;

  // Save the ore field ID and generate a random integer to identify this trip into the ore field
  // to prevent glitching the timers by entering and exiting constantly
  m_oreFieldId    = Commands->Get_ID(oreField);
  m_oreFieldRand  = Commands->Get_Random_Int(2,10240);

  // Send delayed message to increase ore load
  Commands->Send_Custom_Event ( obj, obj, CUSTOM_MINER_INCREASE_ORE_LOAD, m_oreFieldRand, m_oreMiningTime );

  UpdateAnimation(obj, MINER_ANIM_MINING);
}

// -------------------------------------------------------------------------------------------------

void dp88_Ore_Miner::ExitedOreField ( GameObject *obj, GameObject* oreField )
{
  // Reset ore field parameters
  m_oreFieldId    = 0;
  m_oreFieldRand  = 0;

  // Stop the mining animation
  UpdateAnimation(obj, MINER_ANIM_IDLE);

  // If this is an AI miner and our state is still collecting ore then we were probably shoved out
  // of the ore field by some bully in a vehicle so drive back in to finish mining
  if ( m_bUseAI && m_aiState == MINER_AISTATE_COLLECTING_ORE )
    DriveToOreField(obj);
}

// -------------------------------------------------------------------------------------------------

void dp88_Ore_Miner::ReturnToRefinery ( GameObject *obj )
{
  /* Find and drive to the refinery */
  m_aiState = MINER_AISTATE_RETURN_TO_REFINERY;
  GameObject *refinery = Find_Refinery(Commands->Get_Player_Type(obj));
  if ( refinery != NULL )
  {
    GameObject* zone = Find_Closest_Object_With_Script("dp88_Ore_Dump_Zone", Commands->Get_Position(refinery));
    if ( zone != NULL )
    {
      Vector3 position = Commands->Get_Position(zone);
      //Console_Output ( "dp88_Ore_Miner: Driving to location: %.2f, %.2f, %.2f\n", position.X, position.Y, position.Z );

      /* Setup parameters and get going! */
      ActionParamsStruct params;
      params.Set_Basic( this, 100.0f, MINER_ACTIONID_RETURN_TO_REFINERY );
      params.Set_Movement ( position, 1.0f, 25.0f );
      params.MovePathfind = true;
      params.ShutdownEngineOnArrival = true;
      params.AttackActive = false;
      Commands->Action_Goto( obj, params );
    }
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_Ore_Miner::DockAtRefinery ( GameObject *obj )
{
  // Reset current action
  Commands->Action_Reset(obj, 101.0f);

  /* Find and drive to the refinery unloading area */
  GameObject* zone = Find_Closest_Object_With_Script("dp88_Ore_Dump_Zone", Commands->Get_Position(obj));
  if ( zone != NULL )
  {
    Vector3 position = Commands->Get_Position(zone);
    //Console_Output ( "dp88_Ore_Miner: Docking at location: %.2f, %.2f, %.2f\n", position.X, position.Y, position.Z );

    /* Setup parameters and get going! */
    ActionParamsStruct params;
    params.Set_Basic( this, 100.0f, MINER_ACTIONID_DOCK_AT_REFINERY );
    params.Set_Movement ( position, 1.0f, 1.0f );
    params.MoveBackup = true;
    params.ShutdownEngineOnArrival = true;
    params.MovePathfind = true;
    Commands->Action_Goto(obj, params);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_Ore_Miner::DockedAtRefinery ( GameObject *obj )
{
  // If we are using AI then reset the action now that we have arrived
  if (m_bUseAI)
    Commands->Action_Reset ( obj, 101.0f );

  UpdateAnimation(obj, MINER_ANIM_DUMPING);

  // Immobilize the vehicle and disable engine sounds
  if ( obj->As_VehicleGameObj() )
  {
    obj->As_VehicleGameObj()->Set_Immovable(true);
    Commands->Enable_Engine ( obj, false );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_Ore_Miner::UndockedFromRefinery ( GameObject *obj )
{
  // Un-immobilize the vehicle and enable engine sounds
  if ( obj->As_VehicleGameObj() )
  {
    obj->As_VehicleGameObj()->Set_Immovable(false);
    Commands->Enable_Engine ( obj, true );
  }

  // If using the AI then set the AI state and start driving to the ore field
  if ( m_bUseAI )
  {
    DriveToOreField(obj);
  }

  UpdateAnimation(obj, MINER_ANIM_IDLE);
}

// -------------------------------------------------------------------------------------------------

void dp88_Ore_Miner::UpdateAnimation ( GameObject* pObj, MINER_ANIMID animId )
{
  pObj->As_PhysicalGameObj()->Clear_Animation();

  if ( animId < countof(m_animations) && m_animations[animId] != NULL )
  {
    bool bLooping = (animId == MINER_ANIM_MINING) ? true : false;
    Commands->Set_Animation(pObj,m_animations[animId],bLooping,NULL,0,-1,false);
  }

  if (animId < countof(m_animSounds) && m_animSounds[animId] != NULL)
    Commands->Create_Sound(m_animSounds[animId],Commands->Get_Position(pObj),pObj);
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_Ore_Miner> dp88_Ore_Miner_Registrant(
  "dp88_Ore_Miner",
  "Use_AI=1:int,"
  "Ore_Capacity=10:int,"
  "Ore_Mining_Time=2.0:float,"
  "Ore_Dump_Time=8.0:float,"
  "AI_Init_Delay=10:int,"
  "Dump_Animation:string,"
  "Dump_Sound:string,"
  "Mining_Animation:String,"
  "Mining_Sound:string,"
  "Idle_Animation:string,"
  "Resource_Name=ore:string"
);







/*------------------------
Chrono Miner Scripts (AI and non-AI)
--------------------------*/

void dp88_AR_Chrono_Miner::Created( GameObject *obj )
{
  dp88_Ore_Miner::Created(obj);
  objectId = Commands->Get_ID(obj);
  driverId = NULL;
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Chrono_Miner::Damaged( GameObject *obj, GameObject *damager, float amount )
{
  // If AI miner health drops below the emergency chronoshift health threshold and we are driving to
  // the ore field or collecting ore then begin an emergency chronoshift
  if ( m_bUseAI && Commands->Get_Health(obj) < (Commands->Get_Max_Health(obj)*(Get_Float_Parameter("emergencyChronoshiftHealthThreshold")/100.0f))
    && (m_aiState == MINER_AISTATE_COLLECTING_ORE && m_oreMined > 0) )
  {
    // Attempt to start a chronoshift - if it fails don't bother with anything else, driving away
    // won't help...
    Start_Chronoshift(obj);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Chrono_Miner::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
  // Look for vehicle entry
  if ( type == CUSTOM_EVENT_VEHICLE_ENTERED && driverId == NULL )
  {
    driverId = Commands->Get_ID(sender);
    InstallHook( Get_Parameter("chronoshiftKeyhook"), sender );
  }


  // Look for vehicle exit
  else if ( type == CUSTOM_EVENT_VEHICLE_EXITED && Commands->Get_ID(sender) == driverId )
  {
    driverId = NULL;
    RemoveHook();
  }
  
  
  // AI miner failed to chronoshift back to the refinery, if we are still more than 150m
  // from the target then try again
  else if ( type == CUSTOM_CHRONO_MINER_RETRY_CHRONOSHIFT && m_aiState == MINER_AISTATE_RETURN_TO_REFINERY )
  {
    GameObject *refinery = Find_Refinery(Commands->Get_Player_Type(obj));
    if ( refinery != NULL && Commands->Get_Distance(Commands->Get_Position(refinery),Commands->Get_Position(obj)) > 150.0f )
    {
      // Try to chronoshift and, on failure, set timer to try again in 5 seconds
      if ( !Start_Chronoshift(obj) )
        Commands->Send_Custom_Event(obj,obj,CUSTOM_CHRONO_MINER_RETRY_CHRONOSHIFT,0,5.0f);
    }
  }
  
  
  // Time to perform a chronoshift!
  else if ( type == CUSTOM_CHRONO_MINER_DO_CHRONOSHIFT )
  {
    Do_Chronoshift(obj,param);
  }


  // Otherwise pass the message on to the base class
  else
    dp88_Ore_Miner::Custom ( obj, type, param, sender );
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Chrono_Miner::KeyHook()
{
  // Find object
  GameObject* obj = Commands->Find_Object ( objectId );
  if ( !obj )
    return;

  if ( m_aiState != CMINER_AISTATE_CHRONOSHIFTING )
  {
    if ( !Start_Chronoshift(obj) )
      Send_Message_Player ( Get_Vehicle_Driver(obj), 153, 204, 25, "Unable to chronoshift, all target zones are unavailable..." );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Chrono_Miner::ReturnToRefinery ( GameObject *obj )
{
  // Attempt to chronoshift to the refinery
  if ( !Start_Chronoshift(obj) )
  {
    // Can't chronoshift... drive there instead!
    dp88_Ore_Miner::ReturnToRefinery(obj);
    Send_Message_Team ( Get_Object_Type(obj), 153, 204, 25, "The AI Chronominer was unable to chronoshift, please clear the area around the refinery" );

    // Send a delayed custom to retry the chronoshift in 5 seconds
    Commands->Send_Custom_Event(obj,obj,CUSTOM_CHRONO_MINER_RETRY_CHRONOSHIFT,0,5.0f);
  }
}

// -------------------------------------------------------------------------------------------------

bool dp88_AR_Chrono_Miner::Start_Chronoshift( GameObject *obj )
{
  // If we are currently chronoshifting then bail out
  if ( m_aiState == CMINER_AISTATE_CHRONOSHIFTING )
    return false;

  /* Find a zone to chronoshift in to */
  GameObject *refinery = Find_Refinery(Commands->Get_Player_Type(obj));
  if ( refinery != NULL )
  {
    // Check the refinery is not dead - if it is then the target script zones will
    // all be gone, so we have nowhere to go...
    if ( Commands->Get_Health(refinery) == 0 )
      return false;

    // Define the maximum distance we will shift from the refinery - this prevents us
    // from going to the enemy refinery
    const float maxDist = 50.0f;
    Vector3 refineryPos = Commands->Get_Position(refinery);

    SList<GameObject> chronoZones;
    Find_All_Objects_With_Script_By_Distance ( "dp88_AR_Chrono_Miner_Chronozone", chronoZones, refineryPos );

    for ( SLNode<GameObject>* x = chronoZones.Head(); x != NULL; x = x->Next() )
    {
      GameObject* zone = x->Data();
      dp88_AR_Chrono_Miner_Chronozone *chronozone_script = NULL;

      if ( !zone )
        continue;

      // OK, got a candidate zone, can we chronoshift here?
      Vector3 zonePos = Commands->Get_Position(zone);
      if (Vector3::Distance_Squared(zonePos, refineryPos) > maxDist * maxDist || !CanChronoshiftToLocation(obj, zonePos) )
        continue;

      // Is this zone in use for another chronoshift operation? If so then we cannot use it
      chronozone_script = (dp88_AR_Chrono_Miner_Chronozone*)Find_Script_On_Object(zone, "dp88_AR_Chrono_Miner_Chronozone");
      if ( !chronozone_script || (chronozone_script->chronominer_id != NULL
                    && Commands->Find_Object(chronozone_script->chronominer_id)
                    && Commands->Get_Health(Commands->Find_Object(chronozone_script->chronominer_id)) > 0) )
      {
        continue;
      }


      // OK, got ourselves a target zone, lock the zone to our ID and set up for chronoshift...
      chronozone_script->chronominer_id = Commands->Get_ID(obj);

      // NB: We use the AI state flag to determine if we are currently in the middle of
      // a chronoshift for player driven miners too
      m_aiState = CMINER_AISTATE_CHRONOSHIFTING;
    
      // Immobilise the vehicle
      if ( obj->As_VehicleGameObj() )
        obj->As_VehicleGameObj()->Set_Immovable(true);

      // Send a delayed custom to perform the chronoshift
      Commands->Send_Custom_Event ( obj, obj, CUSTOM_CHRONO_MINER_DO_CHRONOSHIFT, Commands->Get_ID(zone), Get_Float_Parameter("chronoshift_time") );


      /* If we have an out effect preset to spawn then spawn it at the origin */
      if ( Is_Valid_Preset(Get_Parameter("chronoshift_out_effect_preset")) )
      {
        // Create effect object
        GameObject* effectObject = Commands->Create_Object ( Get_Parameter("chronoshift_out_effect_preset"), Commands->Get_Position(obj) );

        // Attach script to clean up effect
        StringClass params(true);
        params.Format ( "%.2f,%d", Get_Float_Parameter("chronoshift_out_effect_time"), 983142 );
        Commands->Attach_Script ( effectObject, "JFW_Destroy_Self_Timer", params.Peek_Buffer() );
      }


      /* If we have an in effect preset to spawn then spawn it at the destination */
      if ( Is_Valid_Preset(Get_Parameter("chronoshift_in_effect_preset")) )
      {
        // Create effect object
        GameObject* effectObject = Commands->Create_Object ( Get_Parameter("chronoshift_in_effect_preset"), Commands->Get_Position ( zone ) );
      
        // Attach script to clean up effect
        StringClass params(true);
        params.Format ( "%.2f,%d", Get_Float_Parameter("chronoshift_in_effect_time"), 983142 );
        Commands->Attach_Script ( effectObject, "JFW_Destroy_Self_Timer", params.Peek_Buffer() );
      }

      return true;
    }
  }

  return false;
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Chrono_Miner::Do_Chronoshift( GameObject *obj, int target_zone_id )
{
  // Unimmobilise the vehicle
  if ( obj->As_VehicleGameObj() )
    obj->As_VehicleGameObj()->Set_Immovable(false);

  /* Get the target chronoshift zone */
  GameObject *zone = Commands->Find_Object(target_zone_id);
  if ( zone != NULL )
  {
    // NB: We use the AI state flag to determine if we are currently in the middle
    // of a chronoshift for player driven miners too
    m_aiState = MINER_AISTATE_IDLE;

    // Get a reference to the chronozone script and check if the chronominer_id
    // matches ours. If so zero it and continue, otherwise bail out...
    dp88_AR_Chrono_Miner_Chronozone *chronozone_script = (dp88_AR_Chrono_Miner_Chronozone*)Find_Script_On_Object(zone, "dp88_AR_Chrono_Miner_Chronozone");
    if ( !chronozone_script || chronozone_script->chronominer_id != Commands->Get_ID(obj) )
    {
      // Have another go at a chronoshift...
      ReturnToRefinery(obj);
      return;
    }

    chronozone_script->chronominer_id = 0;

    // Chronoshift to position of zone (resetting rotation in the process)
    Vector3 zonePos = Commands->Get_Position(zone);
    if ( CanChronoshiftToLocation(obj, zonePos) )
      Set_Transform(obj, Matrix3D(zonePos) );
    else
    {
      // Oh noes! Some dipstick has driven into the chronoshift zone... try again
      ReturnToRefinery(obj);
      return;
    }
  }


  /* If using AI start driving to refinery now */
  if ( m_bUseAI )
  {
    if ( m_oreMined > 0 )
    {
      dp88_Ore_Miner::ReturnToRefinery(obj);
    }

    // No ore collected... guess we must have chronoshifted away from an attack
    // so lets set off towards the ore field again... what a waste of time!
    else
    {
      DriveToOreField(obj);
    }
  }
}

// -------------------------------------------------------------------------------------------------

bool dp88_AR_Chrono_Miner::CanChronoshiftToLocation ( GameObject* obj, Vector3& location )
{
  // Get physical game object and moveable phys class references
  MoveablePhysClass* mphys = ( obj->As_PhysicalGameObj() ) ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;

  // Can we move to this position without getting stuck?
  if (mphys)
  {
	return mphys->Can_Teleport( Matrix3D(location) );
  }
  return false;
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_AR_Chrono_Miner> dp88_AR_Chrono_Miner_Registrant(
  "dp88_AR_Chrono_Miner",
  "Use_AI=1:int,"
  "chronoshift_time=2.5:float,"
  "chronoshift_out_effect_preset:string,"
  "chronoshift_out_effect_time:float,"
  "chronoshift_in_effect_preset:string,"
  "chronoshift_in_effect_time:float,"
  "chronoshiftKeyhook=VDeploy:string,"
  "Ore_Capacity=5:int,"
  "Ore_Mining_Time=1.00:float,"
  "Ore_Dump_Time=4.0:float,"
  "emergencyChronoshiftHealthThreshold=30.0:float,"
  "AI_Init_Delay=10:int,"
  "Dump_Animation:string,"
  "Dump_Sound:string,"
  "Mining_Animation:String,"
  "Mining_Sound:string,"
  "Idle_Animation:string"
);








/*------------------------
Chronoshift Zone Controller
--------------------------*/

void dp88_AR_Chrono_Miner_Chronozone::Created ( GameObject *obj )
{
  // If the game controller does not exist then bail out
  GameObject *gameController = Find_Object_With_Script("dp88_ar_gameController");
  if ( !gameController )
  {
    Console_Output ( "dp88_AR_Chrono_Miner_Chronozone - Unable to find Game Controller, unable to continue. Destroying script...\n" );
    Destroy_Script();
    return;
  }

  chronominer_id = NULL;
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_AR_Chrono_Miner_Chronozone> dp88_AR_Chrono_Miner_Chronozone_Registrant(
  "dp88_AR_Chrono_Miner_Chronozone",
  ""
);








/*------------------------
Ore Field
--------------------------*/

void dp88_Ore_Field::Created ( GameObject* pObj )
{
  m_myObjId = Commands->Get_ID(pObj);
  m_pZoneObserver = NULL;

  m_oreValue = Get_Int_Parameter("Ore_Value");
  m_oreCapacity = Get_Int_Parameter("Ore_Capacity");
  m_nOreUnits = Get_Int_Parameter("Ore_Units");

  if ( m_nOreUnits > m_oreCapacity )
    m_nOreUnits =  m_oreCapacity;


  m_strAnimation = Get_Parameter("Animation_Name");
  if (strlen(m_strAnimation) <= 0 || !pObj->As_PhysicalGameObj())
    m_strAnimation = NULL;
  else
  {
    m_nAnimationFullFrame = Get_Int_Parameter("Animation_Full_Frame");
    m_nAnimationEmptyFrame = Get_Int_Parameter("Animation_Empty_Frame");
    UpdateAnimationFrame();
  }


  m_zoneSizeFull = Get_Vector3_Parameter("Zone_Size");
  if ( m_strAnimation )
  {
    m_zoneStepX = Get_Float_Parameter("Zone_Anim_Step_X");
    m_zoneStepY = Get_Float_Parameter("Zone_Anim_Step_Y");
  }


  m_bAiIgnore = (Get_Parameter_Count() >= 10) ? Get_Bool_Parameter("AI_Ignore") : false;

  if (!pObj->As_ScriptZoneGameObj())
  {
    // Create the miner script zone
    Matrix3 rotation(true);
    rotation.Rotate_Z(DEG2RAD(Commands->Get_Facing(pObj)));

    // Define the bounding box and create the zone
    OBBoxClass zoneBoundingBox ( Commands->Get_Position(pObj), m_zoneSizeFull, rotation );
    if ( GameObject* pMinerZone = Create_Zone("Script_Zone_All",zoneBoundingBox) )
    {
      m_minerZoneId = Commands->Get_ID(pMinerZone);

      // Attach observer to the script zone
      m_pZoneObserver = new dp88_Ore_Field_Observer(this);
      pMinerZone->Add_Observer(m_pZoneObserver);

      return;
    }
  }
  else
  {
    m_minerZoneId = Commands->Get_ID(pObj);
    return;
  }

  m_pZoneObserver = NULL;
  Console_Output ( "[%d:%s:%s] Critical Error: Unable to create the miner script zone. Destroying script...\n", Commands->Get_ID(pObj), Commands->Get_Preset_Name(pObj), this->Get_Name() );
  Destroy_Script();
}

// -------------------------------------------------------------------------------------------------

void dp88_Ore_Field::Detach ( GameObject* pObj )
{
  ScriptImpClass::Detach(pObj);

  if ( m_pZoneObserver != NULL )
  {
    if ( GameObject* pMinerZone = Commands->Find_Object(m_minerZoneId) )
      pMinerZone->Remove_Observer(m_pZoneObserver);
    delete m_pZoneObserver;
    m_pZoneObserver = NULL;
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_Ore_Field::Entered ( GameObject* pZoneObj, GameObject* pEnterer )
{
  if ( pZoneObj == Commands->Find_Object(m_minerZoneId) )
  {
    GameObject* pObj = Commands->Find_Object(m_myObjId);

    Commands->Send_Custom_Event( pObj, pEnterer, CUSTOM_MINER_ENTERED_ORE_FIELD, 0, 0 );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_Ore_Field::Exited ( GameObject* pZoneObj, GameObject* pExiter )
{
  if ( pZoneObj == Commands->Find_Object(m_minerZoneId) )
  {
    GameObject* pObj = Commands->Find_Object(m_myObjId);

    Commands->Send_Custom_Event( pObj, pExiter, CUSTOM_MINER_EXITED_ORE_FIELD, 0, 0 );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_Ore_Field::AddOre ( unsigned int nUnits )
{
  if ( m_oreCapacity != 0 )
  {
    m_nOreUnits += min(nUnits,m_oreCapacity-m_nOreUnits);
    UpdateAnimationFrame();
  }
}

// -------------------------------------------------------------------------------------------------

unsigned int dp88_Ore_Field::RemoveOre ( unsigned int nUnits )
{
  if ( m_oreCapacity != 0 )
  {
    nUnits = min(nUnits,m_nOreUnits);
    m_nOreUnits -= nUnits;
    UpdateAnimationFrame();
  }
  return nUnits;
}

// -------------------------------------------------------------------------------------------------

void dp88_Ore_Field::UpdateAnimationFrame()
{
  if ( GameObject* pObj = Commands->Find_Object(m_myObjId) )
    UpdateAnimationFrame(pObj);
}

// -------------------------------------------------------------------------------------------------

void dp88_Ore_Field::UpdateAnimationFrame( GameObject* pObj )
{
  if ( m_oreCapacity != 0 && m_strAnimation != NULL )
  {
    int frame = m_nAnimationFullFrame - (int)ceil((m_nAnimationFullFrame-m_nAnimationEmptyFrame)*((float)m_nOreUnits/m_oreCapacity));
    Commands->Set_Animation_Frame(pObj, m_strAnimation, frame);
  }
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_Ore_Field> dp88_Ore_Field_Registrant(
  "dp88_Ore_Field",
  "Ore_Value:int,"
  "Ore_Capacity:int,"
  "Ore_Units:int,"
  "Animation_Name:string,"
  "Animation_Full_Frame:int,"
  "Animation_Empty_Frame:int,"
  "Zone_Size:vector3,"
  "Zone_Anim_Step_X:float,"
  "Zone_Anim_Step_Y:float,"
  "AI_Ignore=0:int"
);








/*------------------------
Ore Extractor
--------------------------*/

void dp88_Ore_Extractor::Created ( GameObject* pObj )
{
  if ( GameObject* pOreField = Find_Closest_Object_With_Script("dp88_Ore_Field", Commands->Get_Position(pObj)) )
  {
    m_oreFieldId = Commands->Get_ID(pOreField);

    m_nOreUnits = Get_Int_Parameter("Ore_Units");
    m_interval = Get_Int_Parameter("Extraction_Interval");
    m_strAnimation = Get_Parameter("Extraction_Animation");
    if ( strlen(m_strAnimation) <= 0 )
      m_strAnimation = NULL;

    Commands->Start_Timer(pObj, this, (float)m_interval, TIMER_OREMINING_EXTRACTOR );
    return;
  }


  Console_Output ( "[%d:%s:%s] Critical Error: Unable to locate an ore field zone. Destroying script...\n", Commands->Get_ID(pObj), Commands->Get_Preset_Name(pObj), this->Get_Name() );
  Destroy_Script();
}

// -------------------------------------------------------------------------------------------------

void dp88_Ore_Extractor::Timer_Expired ( GameObject* pObj, int number )
{
  if ( number == TIMER_OREMINING_EXTRACTOR )
  {
    if ( m_strAnimation )
      Commands->Set_Animation ( pObj, m_strAnimation, false, NULL, 0, -1, false );
    else
      Animation_Complete(pObj, NULL);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_Ore_Extractor::Animation_Complete ( GameObject* pObj, const char* animationName )
{
  if ( (m_strAnimation == NULL && animationName == NULL)
    || (m_strAnimation != NULL && animationName != NULL && 0 == _stricmp(m_strAnimation,animationName)) )
  {
    // Populate ore field with additional ore
    GameObject* pOreField = Commands->Find_Object(m_oreFieldId);
    if ( !pOreField )
	{
      Destroy_Script();
	  return;
	}

    dp88_Ore_Field* pOreFieldScript = (dp88_Ore_Field *)(Find_Script_On_Object(pOreField, "dp88_Ore_Field"));
    if ( !pOreFieldScript )
	{
      Destroy_Script();
	  return;
	}

    pOreFieldScript->AddOre(m_nOreUnits);

    // Set timer for next extraction
    Commands->Start_Timer(pObj, this, (float)m_interval, TIMER_OREMINING_EXTRACTOR );
  }
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_Ore_Extractor> dp88_Ore_Extractor_Registrant(
  "dp88_Ore_Extractor",
  "Ore_Units:int,"
  "Extraction_Interval:int,"
  "Extraction_Animation:string"
);








/*------------------------
Ore Deposit Zone Controller
--------------------------*/

void dp88_Ore_Dump_Zone::Entered( GameObject *obj, GameObject *enterer )
{
  if ( Get_Object_Type(enterer) == Get_Int_Parameter( "Team" ) )
    Commands->Send_Custom_Event( obj, enterer, CUSTOM_MINER_ENTERED_DUMP_ZONE, Get_Int_Parameter( "Team" ), 0 );
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_Ore_Dump_Zone> dp88_Ore_Dump_Zone_Registrant(
  "dp88_Ore_Dump_Zone",
  "Team=0:int"
);








/*------------------------
Aircraft Landing Zone Scripts
--------------------------*/

// Landing Zone
void dp88_Aircraft_LandingZone::Entered( GameObject *obj, GameObject *enterer )
{
  Commands->Send_Custom_Event( obj, enterer, CUSTOM_TRANSITION_VTOL_LAND_ZONE, 1, 0 );
}

void dp88_Aircraft_LandingZone::Exited( GameObject *obj, GameObject *exiter )
{
  Commands->Send_Custom_Event( obj, exiter, CUSTOM_TRANSITION_VTOL_LAND_ZONE, 0, 0 );
}


// Landing Zone - Aircraft
void dp88_Aircraft_LandingZone_Aircraft::Created ( GameObject *obj )
{
  driverID = 0;
  landingZoneCount = 0;
  if (Get_Int_Parameter("require_landing_zone") >= 1)
  {
	  Commands->Enable_Vehicle_Transitions(obj,false);
  }
}

void dp88_Aircraft_LandingZone_Aircraft::Killed ( GameObject *obj, GameObject* killer )
{
  // We can't simply kill the pilot because things go horribly wrong... instead use the script
  // JFW_Timer_Destroy_Object to kill them as soon as possible.
  if ( driverID != 0 && landingZoneCount == 0 && Get_Int_Parameter("require_landing_zone") >= 1 )
  {
    if ( GameObject* driver = Commands->Find_Object(driverID) )
      Commands->Attach_Script ( driver, "JFW_Timer_Destroy_Object", "1.0,547859,5000.0,Death" );
  }
}

void dp88_Aircraft_LandingZone_Aircraft::Custom ( GameObject *obj, int type, int param, GameObject *sender )
{
  if ( type == CUSTOM_TRANSITION_VTOL_LAND_ZONE && param == 1 )
  {
    landingZoneCount++;

    // Play landing animation if this is the first zone we have entered
    if ( landingZoneCount == 1 )
	{
	  if (Get_Int_Parameter("require_landing_zone") >= 1)
	  {
		  Commands->Enable_Vehicle_Transitions(obj,true);
	  }
      Commands->Set_Animation( obj,Get_Parameter("landing_anim_name"), false, 0, Get_Float_Parameter("landing_anim_first_frame"), Get_Float_Parameter("landing_anim_last_frame"), false );
	}
  }

  else if ( type == CUSTOM_TRANSITION_VTOL_LAND_ZONE && param == 0 )
  {
    landingZoneCount--;

    // Play take off animation if this is the last zone we were in (landing anim in reverse...)
    if ( landingZoneCount == 0 )
	{
	  if (Get_Int_Parameter("require_landing_zone") >= 1)
	  {
		  Commands->Enable_Vehicle_Transitions(obj,false);
	  }
      Commands->Set_Animation( obj, Get_Parameter("landing_anim_name"), false, 0, Get_Float_Parameter("landing_anim_last_frame"), Get_Float_Parameter("landing_anim_first_frame"), false );
	}
  }

  else if ( type == CUSTOM_EVENT_VEHICLE_ENTERED && driverID == NULL )
    driverID = Commands->Get_ID(sender);

  else if ( type == CUSTOM_EVENT_VEHICLE_EXITED && Commands->Get_ID(sender) == driverID )
  {
    driverID = NULL;

    // If the driver exited outside of a landing zone then kablooey!
    if ( Get_Int_Parameter("require_landing_zone") >= 1 && landingZoneCount == 0 )
    {
      Commands->Apply_Damage(sender, 10000.0f, "Death", obj );
      Commands->Apply_Damage(obj, 10000.0f, "Death", obj );
    }
  }
}








/*------------------------
Terror Drone Script
--------------------------*/

/*void dp88_AR_TerrorDrone::Created ( GameObject *obj )
{
	//Console_Output ( "Created dp88_AR_TerrorDrone\n" );
	pilotID = 0;
	targetID = 0;
	consoleID = 0;

	// Report our creation to the game controller to pass on to the console
	Commands->Send_Custom_Event ( obj, Find_Object_With_Script("dp88_AR_GameController"), CUSTOM_TD_REPORTCREATION, Commands->Get_ID( obj ), 1 );

	// Remember default model
	strcpy_s ( defaultModel, sizeof(defaultModel), Get_Model ( obj ) );
}

void dp88_AR_TerrorDrone::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Look for vehicle entry
	if ( type == CUSTOM_EVENT_VEHICLE_ENTERED )
	{
		if ( pilotID == 0 )
			pilotID = Commands->Get_ID(sender);
	}

	// Look for vehicle exit
	else if ( type == CUSTOM_EVENT_VEHICLE_EXITED )
	{
		if ( Commands->Get_ID(sender) == pilotID )
		{
			pilotID = 0;

			// Report driver exit to our console
			Commands->Send_Custom_Event( obj, Commands->Find_Object( consoleID ), CUSTOM_TD_DRIVEREXIT, 1, 0 );
		}
	}

	// Look for console reporting
	else if ( type == CUSTOM_TD_CONSOLEID && consoleID == 0 )
	{
		consoleID = Commands->Get_ID ( sender );
	}

	// A target reporting we have successfully latched on to them
	else if ( type == CUSTOM_TD_TARGET_ID )
	{
		targetID = Commands->Get_ID( sender );
		Commands->Set_Model ( obj, "dumbox" );
		Commands->Set_Position ( obj, Commands->Get_Position ( sender ) );
		Commands->Attach_To_Object_Bone ( obj, sender, "worldbox" );
		Commands->Disable_All_Collisions ( obj );
	}

	// A target reporting they have died
	else if ( type == CUSTOM_TD_TARGET_DIED )
	{
		targetID = 0;
		Vector3 newPosition = Commands->Get_Position ( obj );
		newPosition.Z += 0.5;
		Commands->Set_Position ( obj, newPosition );
		Commands->Enable_Collisions ( obj );
		Commands->Set_Model ( obj, defaultModel );
	}

	// A target reporting they have been repaired (kills terror drone)
	else if ( type == CUSTOM_TD_TARGET_REPAIRED )
	{
		Commands->Apply_Damage ( obj, 5000.00, "Death", sender );
	}
}


void dp88_AR_TerrorDrone::Killed ( GameObject *obj, GameObject *killer )
{
	if ( targetID )
		Commands->Send_Custom_Event( obj, Commands->Find_Object( targetID ), CUSTOM_TD_TD_DIED, 1, 0 );
	targetID = 0;

	// Report death to our console
	Commands->Send_Custom_Event( obj, Commands->Find_Object( consoleID ), CUSTOM_TD_TD_DIED, 1, 0 );
}*/




// -------------------------------------------------------------------------------------------------
// Remote Control Console Script
// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::Created ( GameObject *obj )
{
  //Console_Output ( "[%d:%s:%s] Created - remoteControlID %d\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Int_Parameter("remoteControlID") );

  vehicleID         = NULL;
  pilotID           = NULL;
  m_pilotDummyID    = NULL;
  m_bEnabled        = true;
  m_nChargeTime     = Get_Int_Parameter ( "chargeTime" );

  m_pLoopedAnimCtrl = new LoopedAnimationController(obj);

  // Start charge tick timer if necessary and set initial animation frames
  if ( m_nChargeTime > 0 )
  {
    Commands->Start_Timer( obj, this, 1.0f, TIMER_REMOTECONTROL_CHARGETICK);
  }

  UpdateAnimation(obj);
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::Detach ( GameObject* obj )
{
	ScriptImpClass::Detach(obj);
	if (m_pLoopedAnimCtrl)
	{
		delete m_pLoopedAnimCtrl;
	}
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::Poked ( GameObject *obj, GameObject *poker )
{
	//Console_Output ( "[%d:%s:%s] Poked\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );

	// Check the team of the poker
	int team = Get_Int_Parameter("team");
	if ( team != 2 && Get_Object_Type(poker) != team )
	{
		//Console_Output ( "[%d:%s:%s] Console access denied\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
		// Play access denied sound
		Create_2D_Sound_Player(poker, Get_Parameter("accessDeniedSound") );
		return;
	}

	// Check if the terminal is offline
	if ( !m_bEnabled )
	{
		//Console_Output ( "[%d:%s:%s] Console is offline\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
		// Play console offline sound
		Create_2D_Sound_Player(poker, Get_Parameter("consoleOfflineSound") );
		return;
	}

	// Check if the terminal is charging
	if ( m_nChargeTime > 0 )
	{
		//Console_Output ( "[%d:%s:%s] Console still charging\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
		// Play console charging sound
		Create_2D_Sound_Player(poker, Get_Parameter("consoleChargingSound") );
		return;
	}

	// If no vehicle currently exists then create one
	if ( pilotID == NULL && vehicleID == NULL )
	{
		//Console_Output ( "[%d:%s:%s] BUYING\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
		// Try to purchase the unit, will fail if we don't have enough money
		int cost = Get_Int_Parameter ( "cost" );
		if ( !Purchase_Item(poker, cost ) )
		{
			//Console_Output ( "[%d:%s:%s] NO NOOLAH\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
			// Play insufficient funds denied sound
			Create_2D_Sound_Player(poker, Get_Parameter("insufficientFundsSound") );
			return;
		}

		// OK, we have paid for the unit, lets proceed... first glue the driver in place...
		pilotID = Commands->Get_ID( poker );
		Commands->Control_Enable ( poker, false );

		UpdateAnimation(obj);

		// ...Then create the vehicle...
		//Console_Output ( "[%d:%s:%s] Creating remote control vehicle %s\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Parameter ( "vehiclePreset" ) );
		if (!Create_Vehicle( Get_Parameter ( "vehiclePreset" ), 0.0, Commands->Find_Object( pilotID ), Get_Object_Type( Commands->Find_Object(pilotID) ) ))
		{
			Timer_Expired(obj, TIMER_REMOTECONTROL_TIMEOUT);
		}
		else
		{
			// And wait for confirmation... start a timer in the event that we don't ever get this...
			Commands->Start_Timer ( obj, this, 5.0, TIMER_REMOTECONTROL_TIMEOUT );
		}
	}

	// OK, a vehicle already exists, does it have a pilot? If not then take control of it
	else if ( pilotID == NULL )
	{
		//Console_Output ( "[%d:%s:%s] ENTER EXISTING\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
		// Enable transitions and wait until they are active to put driver in
		GameObject* vehicle = Commands->Find_Object(vehicleID);
		if ( vehicle )
		{
			// Stop the pilot from walking away in the meantime
			pilotID = Commands->Get_ID( poker );
			Commands->Control_Enable ( poker, false );

			// Disable pokeable indicator
			UpdateAnimation(obj);

			Commands->Enable_Vehicle_Transitions( vehicle, true );
			Commands->Start_Timer ( obj, this, 0.5, TIMER_REMOTECONTROL_DRIVERENTER );
		}
		else
			vehicleID = NULL;	// This should never actually happen... but just in case...
	}
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	//Console_Output ( "[%d:%s:%s] Custom %d - %d\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), type, param );
	if ( type == CUSTOM_REMOTECONTROL_CREATED && param == Get_Int_Parameter("remoteControlID") && vehicleID == NULL && pilotID != NULL )
	{
		//Console_Output ( "[%d:%s:%s] Sending console ID to unit\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );

		// Inform the remote control unit that this is it's console (we do this to prevent multiple
		// consoles trying to control the same unit if two are created close together)
		Commands->Send_Custom_Event ( obj, sender, CUSTOM_REMOTECONTROL_CONSOLEID, 0, 0.0 );

		// Wait for the remote control unit to accept this as it's console - it will only accept the
		// first response it recieves, so we may not hear back from it...
	}

	else if ( type == CUSTOM_REMOTECONTROL_CONSOLEACK )
	{
		//Console_Output ( "[%d:%s:%s] Recieved ACK from unit, establishing control\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );

		// The remote control unit has accepted us as it's control console
		vehicleID = Commands->Get_ID(sender);
		
		// Put our pilot in the drivers seat
		Commands->Enable_Vehicle_Transitions( sender, true );
		Commands->Start_Timer ( obj, this, 0.5, TIMER_REMOTECONTROL_DRIVERENTER );
	}

	else if ( type == CUSTOM_REMOTECONTROL_DRIVEREXIT )
	{
    DestroyDummy();
    HandleDriverExit ( obj, Commands->Find_Object(pilotID), sender );
    pilotID = NULL;
	}

	else if ( type == CUSTOM_REMOTECONTROL_DESTROYED )
	{
		//Console_Output ( "[%d:%s:%s] Remote control unit was destroyed\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );

		vehicleID = NULL;

		// Start timer to return the pilot (if we do it immediately the game will crash...)
		if (NULL != pilotID)
			Commands->Start_Timer ( obj, this, 0.5, TIMER_REMOTECONTROL_DRIVEREXIT );

		// Reset charge time
		m_nChargeTime	= Get_Int_Parameter("chargeTime");

		// Start charge tick timer if necessary and set initial animation frames
		if (m_bEnabled && m_nChargeTime > 0)
		{
			Commands->Start_Timer( obj, this, 1.0f, TIMER_REMOTECONTROL_CHARGETICK);
		}
    
    UpdateAnimation(obj);
	}

	else if ( type == CUSTOM_REMOTECONTROL_DISABLED )
	{
		SetEnabled(obj, false);
	}

	else if ( type == CUSTOM_REMOTECONTROL_ENABLED )
	{
		SetEnabled(obj, true);
	}
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::Timer_Expired ( GameObject *obj, int number )
{
	if ( number == TIMER_REMOTECONTROL_TIMEOUT && vehicleID == 0 )
	{
		//Console_Output ( "[%d:%s:%s] Purchase timed out, refunding money and freeing pilot\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );

		GameObject* pilot = Commands->Find_Object(pilotID);
		if ( pilot )
		{
			Commands->Control_Enable(pilot, true);
			Commands->Give_Money(pilot, (float)(Get_Int_Parameter("cost")), 0);
		}
		pilotID = NULL;

    UpdateAnimation(obj);
	}


	else if ( number == TIMER_REMOTECONTROL_DRIVERENTER )
	{
		//Console_Output ( "[%d:%s:%s] Transitioning driver into vehicle\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
		GameObject* vehicle = Commands->Find_Object(vehicleID);
		GameObject* pilot = Commands->Find_Object(pilotID);
		if (pilot)
		{
			HandleDriverEnter(obj, pilot, vehicle);
		}
		else	// This should never actually happen... but just in case...
		{
			Commands->Enable_Vehicle_Transitions(vehicle, false);
			pilotID = NULL;
			UpdateAnimation(obj);
		}
	}


	// This can be called for the vehicle being destroyed OR the console becoming disabled
	else if ( number == TIMER_REMOTECONTROL_DRIVEREXIT )
	{
		DestroyDummy();
		HandleDriverExit ( obj, Commands->Find_Object(pilotID), Commands->Find_Object(vehicleID) );
		pilotID = NULL;
	}


	// Count down tick for charge time
	else if ( number == TIMER_REMOTECONTROL_CHARGETICK )
	{
		if ( m_bEnabled )
		{
			m_nChargeTime--;
			if ( m_nChargeTime > 0 )
				Commands->Start_Timer( obj, this, 1.0f, TIMER_REMOTECONTROL_CHARGETICK);
			else
			{
        UpdateAnimation(obj);
			}
		}
	}
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::SetEnabled(GameObject* obj, bool state)
{
  if ( state == m_bEnabled )
    return;

  m_bEnabled = state;

  // If we are no longer enabled evict the driver
  if (!m_bEnabled && NULL != pilotID)
  {
    GameObject* pilot = Commands->Find_Object(pilotID);
    if (NULL != pilot)
    {
      Soldier_Transition_Vehicle(pilot);
      Create_2D_Sound_Player(pilot, Get_Parameter("consoleOfflineSound"));

      // Force a network update
      Update_Network_Object(pilot);

      // Can't do this instantly or the game crashes. Yay!
      Commands->Start_Timer(obj, this, 0.5, TIMER_REMOTECONTROL_DRIVEREXIT);
    }
  }

  // Start charging for next use (if applicable)
  if (m_nChargeTime > 0)
  {
    Commands->Start_Timer( obj, this, 1.0f, TIMER_REMOTECONTROL_CHARGETICK);
  }

  UpdateAnimation(obj);
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::HandleDriverEnter(GameObject* obj, GameObject* pilot, GameObject* vehicle)
{
  if (NULL != pilot && NULL != vehicle)
  {
    // Create pilot dummy...
    CreateDummy(pilot, Commands->Get_Position(pilot), Commands->Get_Facing(pilot));

    Commands->Control_Enable(pilot, true);

    // Make driver invisible so you don't see them flash on screen when exiting a drone
    Commands->Set_Is_Rendered(pilot, false);
    
    // Set the pilot to the uncollidable group so they can't be killed before they are teleported home
    PhysicalGameObj *physPilot = pilot->As_PhysicalGameObj();
    if (NULL != physPilot)
    {
      m_pilotCachedCollisionGroup = physPilot->Peek_Physical_Object()->Get_Collision_Group();
      physPilot->Peek_Physical_Object()->Set_Collision_Group(UNCOLLIDEABLE_GROUP);
    }

    // Update the pilot object and then force them into the vehicle
    Update_Network_Object(pilot);
    Force_Vehicle_Entry(pilot, vehicle);

    // Play control established sound
    Create_2D_Sound_Player(pilot, Get_Parameter("connectionEstablishedSound"));

    UpdateAnimation(obj);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::HandleDriverExit(GameObject* obj, GameObject* pilot, GameObject* vehicle)
{
  if (pilot)
  {
    Commands->Set_Position ( pilot, pilotDummyPos );

    // Make driver visible and collidable again
    Commands->Set_Is_Rendered ( pilot, true );

    PhysicalGameObj *physPilot = pilot->As_PhysicalGameObj();
    if (NULL != physPilot)
    {
      physPilot->Peek_Physical_Object()->Set_Collision_Group(m_pilotCachedCollisionGroup);
    }

    Update_Network_Object(pilot);
  }

	// If the vehicle is still alive set it to its idle state
	if ( vehicle )
	{
		// Set team to correct setting (so it gives points for being damaged)
		Set_Object_Type ( vehicle, Get_Int_Parameter("team") );

		// Disable transitions on remote vehicle
		Commands->Enable_Vehicle_Transitions( vehicle, false );
	}

  // Start charging for next use (if applicable)
  if (m_nChargeTime > 0)
  {
    Commands->Start_Timer( obj, this, 1.0f, TIMER_REMOTECONTROL_CHARGETICK);
  }

  UpdateAnimation(obj);
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::CreateDummy ( GameObject* pilot, Vector3 position, float facing )
{
  GameObject* dummy = Commands->Create_Object ( "Invisible_Object_2", position );
  if ( dummy )
  {
    m_pilotDummyID = Commands->Get_ID(dummy);
    Commands->Set_Facing(dummy, facing);
    pilotDummyPos = position;   // Used to put the pilot back in his original location

    // Clone health/armour
    Set_Skin(dummy, Get_Skin(pilot) );
    Commands->Set_Shield_Type(dummy, Get_Shield_Type(pilot) );
    Set_Max_Health(dummy, Commands->Get_Max_Health(pilot) );
    Set_Max_Shield_Strength(dummy, Commands->Get_Max_Shield_Strength(pilot) );
    Commands->Set_Health(dummy, Commands->Get_Health(pilot) );
    Commands->Set_Shield_Strength(dummy, Commands->Get_Shield_Strength(pilot) );
    Set_Object_Type(dummy, Get_Object_Type(pilot));

    // Link dummy and pilot health and armour
    char pilotIdString[12];
    sprintf ( pilotIdString, "%d", Commands->Get_ID(pilot) );
    Attach_Script_Once ( dummy, "dp88_linkHealth", pilotIdString );

    // Clone player model onto dummy and set a pose
    Commands->Set_Model(dummy, Get_Model(pilot));
    Commands->Set_Animation(dummy, "S_A_HUMAN.H_A_A0A0", true, NULL, 0, 0, false);
    
    // Setup soldier collision mode on the dummy
    PhysicalGameObj *physDummy = dummy->As_PhysicalGameObj();
    if (NULL != physDummy)
      physDummy->Peek_Physical_Object()->Set_Collision_Group(SOLDIER_COLLISION_GROUP);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::DestroyDummy()
{
  if (NULL != m_pilotDummyID)
  {
    GameObject* dummy = Commands->Find_Object(m_pilotDummyID);
    m_pilotDummyID = NULL;

    // Destroy the dummy object rather than killing it - this prevents dp88_linkHealth from also
    // killing the pilot
    if ( dummy )
      Commands->Destroy_Object(dummy);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::UpdateAnimation(GameObject* obj)
{
  bool bPokable = false;

  if (!m_bEnabled)
    m_pLoopedAnimCtrl->PlayAnimation(Get_Parameter("animationName"), Get_Int_Parameter("animFrameDisabled1"), Get_Int_Parameter("animFrameDisabled2"));

  else if (NULL != pilotID)
    m_pLoopedAnimCtrl->PlayAnimation(Get_Parameter("animationName"), Get_Int_Parameter("animFrameActive1"), Get_Int_Parameter("animFrameActive2"));

  else if (NULL != vehicleID)
  {
    m_pLoopedAnimCtrl->PlayAnimation(Get_Parameter("animationName"), Get_Int_Parameter("animFrameIdle1"), Get_Int_Parameter("animFrameIdle2"));
    bPokable = true;
  }

  else if (m_nChargeTime > 0)
    m_pLoopedAnimCtrl->PlayAnimation(Get_Parameter("animationName"), Get_Int_Parameter("animFrameCharging1"), Get_Int_Parameter("animFrameCharging2"));

  else
  {
    m_pLoopedAnimCtrl->PlayAnimation(Get_Parameter("animationName"), Get_Int_Parameter("animFrameAvailable1"), Get_Int_Parameter("animFrameAvailable2"));
    bPokable = true;
  }

  Commands->Enable_HUD_Pokable_Indicator(obj, bPokable);
}




// -------------------------------------------------------------------------------------------------
// Remote Control Vehicle Script
// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlVehicle::Created ( GameObject *obj )
{
	//Console_Output ( "[%d:%s:%s] Created - remoteControlID %d\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Int_Parameter("remoteControlID") );

	consoleID = NULL;
	pilotID = NULL;

	Commands->Enable_Vehicle_Transitions(obj, false);

	// Notify all consoles of our creation and disable transitions by default
	Send_Custom_Event_To_Objects_With_Script(obj, "dp88_RemoteControlConsole", CUSTOM_REMOTECONTROL_CREATED, Get_Int_Parameter("remoteControlID"), 0 );
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlVehicle::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Look for vehicle entry
	if ( type == CUSTOM_EVENT_VEHICLE_ENTERED )
	{
		if ( pilotID == 0 )
			pilotID = Commands->Get_ID(sender);
	}

	// Look for vehicle exit
	else if ( type == CUSTOM_EVENT_VEHICLE_EXITED )
	{
		if ( Commands->Get_ID(sender) == pilotID )
		{
			pilotID = 0;

			// Report driver exit to our console
			if ( consoleID != NULL )
				Commands->Send_Custom_Event( obj, Commands->Find_Object( consoleID ), CUSTOM_REMOTECONTROL_DRIVEREXIT, 1, 0 );
		}
	}

	// Look for console reporting and, if we don't already have a console, send an ACK
	// to accept them as our controller
	else if ( type == CUSTOM_REMOTECONTROL_CONSOLEID && consoleID == 0 )
	{
		//Console_Output ( "[%d:%s:%s] Got console, sending ACK\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
		consoleID = Commands->Get_ID ( sender );
		Commands->Send_Custom_Event(obj, sender, CUSTOM_REMOTECONTROL_CONSOLEACK, 1 ,0);
	}
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlVehicle::Killed ( GameObject* obj, GameObject* killer )
{
	if ( consoleID != NULL )
		Commands->Send_Custom_Event( obj, Commands->Find_Object( consoleID ), CUSTOM_REMOTECONTROL_DESTROYED, 1, 0 );
}




/*------------------------
Demo Truck Scripts
--------------------------*/

void dp88_AR_DemoTruck::Created( GameObject *obj )
{
	//Console_Output ( "Created dp88_AR_DemoTruck\n" );
	pilotID = 0;
	canDetonate = true;
}

void dp88_AR_DemoTruck::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Look for vehicle entry
	if ( type == CUSTOM_EVENT_VEHICLE_ENTERED )
	{
		if ( pilotID == 0 )
			pilotID = Commands->Get_ID(sender);
	}

	// Look for vehicle exit
	else if ( type == CUSTOM_EVENT_VEHICLE_EXITED )
	{
		if ( Commands->Get_ID(sender) == pilotID && canDetonate )
			pilotID = 0;
	}
}

void dp88_AR_DemoTruck::Killed( GameObject *obj, GameObject *killer )
{
	Detonate( obj );
}

void dp88_AR_DemoTruck::Damaged ( GameObject *obj, GameObject *damager, float amount )
{
	// If we damaged ourselves thats the trigger to detonate
	if ( Commands->Get_ID ( damager ) == Commands->Get_ID ( Get_Vehicle_Driver ( obj ) ) && amount == 0.0 )
		Detonate( obj );
}

void dp88_AR_DemoTruck::Detonate( GameObject *obj )
{
	if ( canDetonate )
	{
		canDetonate = false;

		// Create the explosion
		if ( pilotID != 0 )
		{
			Commands->Create_Explosion ( Get_Parameter ( "explosionPreset" ), 
				Commands->Get_Position ( Commands->Find_Object( pilotID ) ), 
				Commands->Find_Object( pilotID )
			);

			Ranged_Scale_Damage_To_Buildings( 
				Get_Float_Parameter ( "buildingDamageStrength" ), 
				Get_Parameter ( "buildingDamageWarhead" ), 
				Commands->Get_Position( Commands->Find_Object( pilotID ) ), 
				Get_Float_Parameter ( "buildingDamageRange" ), 
				Commands->Find_Object( pilotID )
			);
		}
		
		else
		{
			Commands->Create_Explosion ( Get_Parameter ( "explosionPreset" ), 
				Commands->Get_Position ( obj ), 
				obj
			);


			Ranged_Scale_Damage_To_Buildings( 
				Get_Float_Parameter ( "buildingDamageStrength" ), 
				Get_Parameter ( "buildingDamageWarhead" ), 
				Commands->Get_Position( obj ), 
				Get_Float_Parameter ( "buildingDamageRange" ), 
				obj
			);
		}

		// Create cinematic if nessicary
		if ( strcmp ( Get_Parameter( "cinematicPreset" ), "null" ) != 0 )
			Commands->Create_Object ( Get_Parameter ( "cinematicPreset" ), Commands->Get_Position ( obj ) );

		// Kill driver and truck - can't kill driver instantly as truck protects them
		Commands->Apply_Damage ( obj, 5000.00, "Death", obj );
		if ( pilotID != 0 )
			Commands->Attach_Script( Commands->Find_Object( pilotID ),"RA_DriverDeath", "0" );
	}
}



/*------------------------
Paradrop Scripts
--------------------------*/

void dp88_AR_paradrop_Console::Created ( GameObject *obj )
{
	//Console_Output ( "Created dp88_AR_paradrop_Console\n" );
	last_triggered = 0;
}

void dp88_AR_paradrop_Console::Poked ( GameObject *obj, GameObject *poker )
{
	// Deny use to members of the wrong team (unless team == 2 (ie: anyone))
	if ( Get_Int_Parameter("team") != 2 && Get_Object_Type(poker) != Get_Int_Parameter("team"))
	{
		Send_Message_Player(obj, 153, 204, 25, "Invalid Security Clearance\n");
		return;
	}

	if ( time(NULL)-last_triggered < 180 )
	{
		Send_Message_Player(obj, 153, 204, 25, "This can only be used once every 3 minutes\n");
		return;
	}

	last_triggered = (int)time(NULL);

	// TEMP
	Vector3 pos;
	pos.X = -40.0f;
	pos.Y = 110.0f;
	pos.Z = 40.0f;

	// Change poker into paradrop unit and teleport to position
	Commands->Set_Position(poker,pos);
	Change_Character(poker,"Allied_GI_Paradrop");
	
	// Spawn 3 bots to go with me :)
	for ( int i = 0; i < 3; i++ )
	{
		pos.X += 2;
		pos.Y += 2;
		Commands->Create_Object ( "Allied_GI_Paradrop", pos );
	}
}

// -------------------------------------------------------------------------------------------------

// Registrar goes here...
//








void dp88_AR_Paradrop::Created( GameObject* pObj )
{
  if ( !pObj->As_SoldierGameObj() )
  {
    Console_Output ( "[%d:%s:%s] Critical Error: This script is only compatible with soldier game objects. Destroying script...\n", Commands->Get_ID(pObj), Commands->Get_Preset_Name(pObj), this->Get_Name() );
    Destroy_Script();
    return;
  }

  //Console_Output ( "Created dp88_AR_paradrop\n" );
  earth_warhead = ArmorWarheadManager::Get_Warhead_Type("Earth");
  m_nParachuteModel = 0;

  const char* parachute_model = Get_Parameter("Parachute_Model");
  if ( strlen(parachute_model) > 0 )
  {
    GameObject* pParachute = Commands->Create_Object("Invisible_Object",Commands->Get_Position(pObj));
    Commands->Set_Model ( pParachute, parachute_model );
    Commands->Attach_To_Object_Bone(pParachute, pObj, Get_Parameter("Parachute_Bone"));
    m_nParachuteModel = Commands->Get_ID(pParachute);
  }

  Vector3 velocity = Get_Velocity(pObj);
  m_fallRate = velocity.Z;
  Commands->Start_Timer(pObj, this, 1.0f, TIMER_PARADROP_CHECKFALLRATE );

  if ( strlen(Get_Parameter("Animation")) > 0 )
  {
    m_pAnimController = new LoopedAnimationController(pObj);
    m_pAnimController->PlayAnimation ( Get_Parameter("Animation"), Get_Int_Parameter("Animation_First_Frame"), Get_Int_Parameter("Animation_Last_Frame"), (Get_Int_Parameter("Animation_Looped")==1) );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Paradrop::Damaged( GameObject* pObj, GameObject* pDamager, float amount )
{
  if ( Get_Damage_Warhead() == earth_warhead )
  {
    // Repair the falling damage. Note that, in stock Renegade and most mods, falling damage ignores
    // armour, so we can't use Apply_Damage to heal the damage, it has to be done manually
    float health = Commands->Get_Health(pObj);
    float max = Commands->Get_Max_Health(pObj);
    health += amount;
    if ( health > max )
    {
      amount -= abs(max-health);
      health = max;
    }
    else
      amount = 0.0f;
    Commands->Set_Health(pObj, health);

    // Apply any left-over repairs to the armour (to support mods where armour takes falling damage)
    if ( amount > 0 )
    {
      health = Commands->Get_Shield_Strength(pObj);
      max = Commands->Get_Max_Shield_Strength(pObj);
      health += amount;
      if ( health > max )
        health = max;
      Commands->Set_Shield_Strength(pObj, health);
    }

    Landed(pObj);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Paradrop::Killed( GameObject* pObj, GameObject* pKilled )
{
  // Despawn the parachute model
  if ( m_nParachuteModel != 0 )
    Commands->Destroy_Object(Commands->Find_Object(m_nParachuteModel));
  m_nParachuteModel = 0;

  // Prevent anything else happening, such as Timer_Expired
  Destroy_Script();
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Paradrop::Timer_Expired( GameObject* pObj, int number )
{
  if ( number == TIMER_PARADROP_CHECKFALLRATE )
  {
    Vector3 velocity = Get_Velocity(pObj);
    if ( velocity.Z > m_fallRate )
      Landed(pObj);
    else
    {
      m_fallRate = velocity.Z;
      Commands->Start_Timer(pObj, this, 1.0f, TIMER_PARADROP_CHECKFALLRATE );
    }
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Paradrop::Detach(GameObject* obj)
{
  ScriptImpClass::Detach(obj);
  delete m_pAnimController;
  m_pAnimController = NULL;
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Paradrop::Landed ( GameObject* pObj )
{
  // Despawn the parachute model
  if ( m_nParachuteModel != 0 )
    Commands->Destroy_Object(Commands->Find_Object(m_nParachuteModel));
  m_nParachuteModel = 0;


  // Swap to new infantry preset
  const char* infantry_preset = Get_Parameter("Infantry_Preset");
  if ( strlen(infantry_preset) > 0 && Is_Valid_Preset(infantry_preset) )
  {
    float health = Commands->Get_Health(pObj);
    float armour = Commands->Get_Shield_Strength(pObj);
    Change_Character(pObj, infantry_preset );
    Commands->Set_Health(pObj, health);
    Commands->Set_Shield_Strength(pObj, armour);
  }

  // Prevent anything else happening, such as Timer_Expired
  Destroy_Script();
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_AR_Paradrop> dp88_AR_Paradrop_Registrant(
  "dp88_AR_Paradrop",
  "Infantry_Preset:string,"
  "Parachute_Model:string,"
  "Parachute_Bone:string,"
  "Animation:string,"
  "Animation_First_Frame:int,"
  "Animation_Last_Frame:int,"
  "Animation_Looped:int"
);







/*------------------------
Prism Tower AI
--------------------------*/

// Static variable initialisation
int dp88_AR_Prism_Tower::prismTowerCount = 0;
int dp88_AR_Prism_Tower::prismTowerIds[MAX_TOWERS] = { 0 };
dp88_AR_Prism_Tower* dp88_AR_Prism_Tower::prismTowerScripts[MAX_TOWERS] = { NULL };

// Static function - register tower
void dp88_AR_Prism_Tower::registerTower(int towerId, dp88_AR_Prism_Tower* script)
{
	// Find an empty slot for this tower
	for ( int i = 0; i < MAX_TOWERS; ++i )
	{
		if ( prismTowerIds[i] == 0 )
		{
			prismTowerIds[i] = towerId;
			prismTowerScripts[i] = script;

			// Recalculate tower map
			calculateTowerMap();
			break;
		}
	}
}

// Static function - deregister tower
void dp88_AR_Prism_Tower::deregisterTower ( int towerId )
{
	// Find this tower in the list and remove it from the array
	for ( int i = 0; i < MAX_TOWERS; ++i )
	{
		if ( prismTowerIds[i] == towerId )
		{
			prismTowerIds[i] = 0;
			clearTowerMap(i);
			prismTowerScripts[i] = NULL;

			// Recalculate tower map
			calculateTowerMap();
			break;
		}
	}
}

// Static function - clear tower map for a given index
void dp88_AR_Prism_Tower::clearTowerMap ( int idx )
{
	if ( idx < MAX_TOWERS )
	{
		prismTowerScripts[idx]->adjacentTowerCount = 0;
		if ( prismTowerScripts[idx]->adjacentTowers != NULL )
		{
			delete [] prismTowerScripts[idx]->adjacentTowers;
			prismTowerScripts[idx]->adjacentTowers = NULL;
		}
	}
}

// Static function - calculate tower map
void dp88_AR_Prism_Tower::calculateTowerMap()
{
	// Count number of registered towers and reset their adjacent tower indexes
	int numTowers = 0;
	for ( int i = 0; i < MAX_TOWERS; ++i )
	{
		if ( prismTowerIds[i] > 0 )
		{
			numTowers++;
			clearTowerMap(i);
		}
	}


	// Build array of all possible connections between registered towers - maximum
	// number of nodes is n(n-1)/2 (IE: Complete graph), but unless the towers are
	// very close together we will probably use less than this
	int maxConnections = (int)(numTowers*((numTowers-1.0f)/2.0f));
	int* towerConnections = new int[maxConnections*3];
	memset(towerConnections,0,sizeof(int)*maxConnections*3);


	// Populate array
	int connectionCount = 0;
	for ( int i = 0; i < MAX_TOWERS; ++i )
	{
		if ( prismTowerIds[i] > 0 )
		{
			GameObject* t1 = Commands->Find_Object(prismTowerIds[i]);
			for ( int j = i+1; j < MAX_TOWERS; ++j )
			{
				if ( prismTowerIds[j] > 0 )
				{
					GameObject* t2 = Commands->Find_Object(prismTowerIds[j]);
					
					// If these towers can assist each other then add them to the connections list
					if (CanAssistTower(t1,t2, ((dp88_AR_Prism_Tower*)prismTowerScripts[i])->primary_maxRange))
					{
					  towerConnections[connectionCount*3] = i;
						towerConnections[(connectionCount*3)+1] = j;
						towerConnections[(connectionCount*3)+2] = (int)Commands->Get_Distance(Commands->Get_Position(t1),Commands->Get_Position(t2));
						connectionCount++;
					}
				}
			}
		}
	}


	// Now sort the entries in descending order and add a fourth field to
	// allow the connections to be marked as 'bad' during the reverse
	// delete algorithm
	int* sortedTowerConnections = new int[connectionCount*4];
	memset(sortedTowerConnections,0,sizeof(int)*connectionCount*4);


	// Copy connections to the new array one by one, starting with the largest
	// value and ending with the smallest... yes, this is probably a horribly
	// inefficient way of doing this but it only occasionally and it works...
	for ( int i = 0; i < connectionCount; ++i )
	{
		int maxDist = 0;
		int maxDistIndex = -1;
		for ( int j = 0; j < connectionCount; ++j )
		{
			if ( towerConnections[j*3] != -1 && towerConnections[(j*3)+2] > maxDist )
			{
				maxDist = towerConnections[(j*3)+2];
				maxDistIndex = j;
			}
		}

		// Copy highest index and set it's first tower ID to 0 so we skip past
		// it in subsequent iterations
		if ( maxDistIndex != -1 )
		{
			sortedTowerConnections[i*4] = towerConnections[maxDistIndex*3];
			sortedTowerConnections[(i*4)+1] = towerConnections[(maxDistIndex*3)+1];
			sortedTowerConnections[(i*4)+2] = towerConnections[(maxDistIndex*3)+2];
			sortedTowerConnections[(i*4)+3] = 0;	// Not a 'bad' connection

			// Remove the copied item from the sort
			towerConnections[maxDistIndex*3] = -1;
		}
	}

	// Get rid of tower connections
	delete [] towerConnections;


	// OK, we have a sorted list of connections, now iterate through it and
	// for each connection see if there is another path between the towers it
	// links. If there is then get rid of this connection
	for ( int i = 0; i < connectionCount; ++i )
	{
		// Mark the connection as bad
		sortedTowerConnections[(i*4)+3] = 1;

		// Check to see if another path exists between these towers - if it
		// does not then this path is required for the minimum spanning tree
		// so restore it as a 'good' path
		if ( !calculateTowerMapPathSearch(sortedTowerConnections,connectionCount,sortedTowerConnections[i*4],sortedTowerConnections[(i*4)+1]) )
			sortedTowerConnections[(i*4)+3] = 0;
	}


	// OK, we now have a minimum spanning tree... update each tower with
	// a list of their neighbours
	for ( int i = 0; i < connectionCount; ++i )
	{
		// Ignore 'bad' connections
		if ( sortedTowerConnections[(i*4)+3] != 1 )
		{
			int tower1idx = sortedTowerConnections[i*4];
			int tower1id = prismTowerIds[tower1idx];
			dp88_AR_Prism_Tower* tower1script = prismTowerScripts[tower1idx];
			int tower2idx = sortedTowerConnections[(i*4)+1];
			int tower2id = prismTowerIds[tower2idx];
			dp88_AR_Prism_Tower* tower2script = prismTowerScripts[tower2idx];


			// Add tower2 to tower1's adjacent towers list
			int* oldAdjacentTowers = tower1script->adjacentTowers;
			tower1script->adjacentTowers = new int[++tower1script->adjacentTowerCount];

			// Copy existing data
			for ( int j = 0; j < tower1script->adjacentTowerCount-1; ++j )
				tower1script->adjacentTowers[j] = oldAdjacentTowers[j];

			// Delete old array
			if ( oldAdjacentTowers != NULL )
				delete [] oldAdjacentTowers;

			// Add new adjacent tower
			tower1script->adjacentTowers[tower1script->adjacentTowerCount-1] = tower2id;




			// Add tower1 to tower2's adjacent towers list
			oldAdjacentTowers = tower2script->adjacentTowers;
			tower2script->adjacentTowers = new int[++tower2script->adjacentTowerCount];

			// Copy existing data
			for ( int j = 0; j < tower2script->adjacentTowerCount-1; ++j )
				tower2script->adjacentTowers[j] = oldAdjacentTowers[j];

			// Delete old array
			if ( oldAdjacentTowers != NULL )
				delete [] oldAdjacentTowers;

			// Add new adjacent tower
			tower2script->adjacentTowers[tower2script->adjacentTowerCount-1] = tower1id;
		}
	}

	// Get rid of sorted tower connections
	delete [] sortedTowerConnections;
}


bool dp88_AR_Prism_Tower::calculateTowerMapPathSearch(int* sortedConnections, int numConnections, int tower1, int tower2)
{
	// Try to find a path between these towers
	for ( int i = 0; i < numConnections; ++i )
	{
		// Ignore 'bad' connections
		if ( sortedConnections[(i*4)+3] != 1 )
		{
			// If this is a connection between the towers we are searching for then
			// return success
			if ( (sortedConnections[i*4] == tower1 && sortedConnections[(i*4)+1] == tower2)
				|| (sortedConnections[i*4] == tower2 && sortedConnections[(i*4)+1] == tower1) )
				return true;

      bool bFound = false;

      // Temporarily mark this connection as "bad" to prevent infinite recursion
      sortedConnections[(i*4)+3] = 1;

      // If either of the two towers in this connection matches the desired tower then check for a
      // path between the other desired tower and the third party tower it is connected to...
      if ( sortedConnections[i*4] == tower1 )
        bFound = calculateTowerMapPathSearch(sortedConnections,numConnections,tower2,sortedConnections[(i*4)+1]);
      else if ( sortedConnections[(i*4)+1] == tower1 )
        bFound = calculateTowerMapPathSearch(sortedConnections,numConnections,tower2,sortedConnections[i*4]);
      else if ( sortedConnections[i*4] == tower2 )
        bFound = calculateTowerMapPathSearch(sortedConnections,numConnections,tower1,sortedConnections[(i*4)+1]);
      else if ( sortedConnections[(i*4)+1] == tower2 )
        bFound = calculateTowerMapPathSearch(sortedConnections,numConnections,tower1,sortedConnections[i*4]);

      // Restore this connection as "good"
      sortedConnections[(i*4)+3] = 0;

      if ( bFound )
        return true;
		}
	}

	// Nothing found...
	return false;
}


bool dp88_AR_Prism_Tower::CanAssistTower(GameObject* tower1, GameObject* tower2, int maxRange)
{
  int distance = (int)Commands->Get_Distance(Commands->Get_Position(tower1),Commands->Get_Position(tower2));
  if (distance > maxRange)
    return false;
    
  // In weapons range so lets do a collision test to check for obstructions
/*  CastResultStruct res;
  LineSegClass ray(Commands->Get_Bone_Position(tower1,"muzzlea0"),GetAssistAimPoint(tower2));
  PhysRayCollisionTestClass coltest(ray, &res, BULLET_COLLISION_GROUP);
  PhysicsSceneClass::Get_Instance()->Cast_Ray(coltest,false);
  
  if (NULL != coltest.CollidedPhysObj)
  {
    // todo: verify the collided object == tower2
    // todo: what if we hit the source tower on the way out? Do we need to figure out some math
    //       to offset the beam origin to slightly beyond the bounds of the origin? Or maybe
    //       set the turret rotation to aim at the tower2?
	// use PhysClass::Inc_Ignore_Counter and PhysClass::Dec_Ignore_Counter here to make the ray casting code ignore the object you want it to ignore
  }*/
  
  return true;
}








void dp88_AR_Prism_Tower::Created( GameObject *obj )
{
  loadSettings(obj, false, false);
  Init(obj);

  // Initialise member variables
  isAssistingTower = false;
  adjacentTowerCount = 0;
  adjacentTowers = NULL;

  // Register the tower
  registerTower(Commands->Get_ID(obj),this);
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Prism_Tower::Damaged ( GameObject *obj, GameObject *damager, float amount )
{
  // If the damager is a prism tower then we have been charged, add 1 to ammo count
  if ( Is_Script_Attached ( damager, "dp88_AR_Prism_Tower" ) )
  {
    /* Increase ammo count by 1 */
    if ( Get_Current_Weapon(obj) != NULL )
      Set_Current_Bullets(obj,Get_Current_Bullets(obj)+1);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Prism_Tower::Killed ( GameObject *obj, GameObject *killer )
{
  // Notify adjacent towers to stop assisting us if they were
  for ( int i = 0; i < adjacentTowerCount; i++ )
  {
    if (Commands->Find_Object(adjacentTowers[i]) != obj)
    {
      Commands->Send_Custom_Event(obj,Commands->Find_Object(adjacentTowers[i]), CUSTOM_PRISMTOWER_STOP_CHARGING,0,0.0f);
    }
  }

  // Deregister this tower
  deregisterTower(Commands->Get_ID(obj));
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Prism_Tower::Destroyed ( GameObject *obj )
{
  // Deregister this tower
  deregisterTower(Commands->Get_ID(obj));
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Prism_Tower::Custom ( GameObject *obj, int type, int param, GameObject *sender )
{
  // If we recieve a stop charging message from the tower we are currently
  // charging then we should stop all actions
  if ( type == CUSTOM_PRISMTOWER_STOP_CHARGING && isAssistingTower && sender == m_target )
    StopAssisting(obj);


  // If we recieve a request charging message from another tower then we
  // should check to see if their target has higher priority than our
  // current target (if any) and, if so, start charging them
  else if ( type == CUSTOM_PRISMTOWER_REQUEST_CHARGING )
  {
    // Are we idle? If so then there's no reason not to simply start charging immediatly
    if ( !m_target )
      StartAssisting(obj, sender, (float)param);

    // Is this request from the tower we are currently charging? If so then update the last seen
    // time and priority and forward the assistance request to any other adjacent towers
    else if ( isAssistingTower && m_target == sender )
    {
      targetLastSeen = (int)time(NULL);
      targetPriority = (float)param;
      SendAssistanceRequests(obj);
      return;
    }

    // Is the priority of the request greater than the priority of our current target (either
    // another tower or an enemy)? If so then stop what we are doing and charge the requester
    else if ( param > targetPriority )
      StartAssisting(obj, sender, (float)param );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Prism_Tower::Timer_Expired ( GameObject *obj, int number )
{
  // Piggy back our assistance polling and charge refilling on the existing think timer
  if ( number == TIMER_AI_THINK )
  {
    /* Send out assistance requests to ensure other towers don't time out on the 'last seen' check
    *
    * \todo
    *   Can this screw up the timings between charging towers and the attacking tower? Need to think
    *   this through sometime and possibly do some experimentation...
    */
    if ( m_target )
      SendAssistanceRequests(obj);

    /* Refill a single unit of charge if depleted */
    if ( Get_Current_Clip_Bullets(obj) == 0 )
      Set_Current_Clip_Bullets(obj,1);

    /* If current bullets > 1 and no enemy seen recently then additional charge is lost */
    if ( Get_Current_Bullets(obj) > 1 && !m_target )
      Set_Current_Bullets(obj,1);
  }


  // Pass the timer onto the base class to be handled
  dp88_AI_ChargedTurret::Timer_Expired(obj, number);
}

// -------------------------------------------------------------------------------------------------

float dp88_AR_Prism_Tower::getPriority( GameObject *obj, GameObject *target )
{
  // If the target is the tower we are currently charging then return
  // the priority of that charging sequence
  if ( m_target == target && isAssistingTower )
    return targetPriority;

  // Otherwise run the normal priority calculation
  return dp88_AI_ChargedTurret::getPriority(obj, target);
}

// -------------------------------------------------------------------------------------------------

bool dp88_AR_Prism_Tower::checkTeam( GameObject *obj, GameObject *target )
{
  // Return true for the tower we are charging, even though it is on the same team as us
  if ( m_target == target && isAssistingTower )
    return true;

  // Otherwise run the normal check team function
  return dp88_AI_ChargedTurret::checkTeam(obj, target);
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Prism_Tower::attackTarget ( GameObject* obj, GameObject* target, bool primary )	
{
  // Call base classes attack routine
  dp88_AI_ChargedTurret::attackTarget(obj, target, primary);

  // Send notifications for assistance
  SendAssistanceRequests(obj);
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Prism_Tower::attackLocation ( GameObject* obj, Vector3 location, bool primary )
{
  // Call base classes attack routine
  dp88_AI_ChargedTurret::attackLocation(obj, location, primary);

  // Send notifications for assistance
  SendAssistanceRequests(obj);
}

// -------------------------------------------------------------------------------------------------


void dp88_AR_Prism_Tower::stopAttacking ( GameObject* obj )
{
  // Was the target a tower we were charging? If so this means they must have timed out on the last
  // seen and are therefore probably dead...
  if ( isAssistingTower )
    StopAssisting(obj);

  // Otherwise we were attacking a normal target, send out end assistance notifications to all
  // adjacent towers and call base class
  else
  {
    SendEndAssistanceNotifications(obj);
    dp88_AI_ChargedTurret::stopAttacking(obj);
  }
}

// -------------------------------------------------------------------------------------------------

/* Start charging the specified tower */
void dp88_AR_Prism_Tower::StartAssisting(GameObject* obj, GameObject* tower, float priority)
{
  // Set our new target ID and priority
  m_target = tower;
  targetPriority = priority;
  targetLastSeen = (int)time(NULL);
  isAssistingTower = true;

  // Work out attack position
  Vector3 chargePos = GetAssistAimPoint(tower);

  // Start charging the tower - this will also trigger assistance notifications to be sent out
  attackLocation ( obj, chargePos, true );
}

// -------------------------------------------------------------------------------------------------

/* Stop charging the specified tower */
void dp88_AR_Prism_Tower::StopAssisting(GameObject* obj)
{
  if ( isAssistingTower )
  {
    m_target = nullptr;
    targetPriority = 0;
    isAssistingTower = false;
    Commands->Action_Reset(obj, 101.0f);
  }

  // Send end assistance notifications
  SendEndAssistanceNotifications(obj);
}

// -------------------------------------------------------------------------------------------------

Vector3 dp88_AR_Prism_Tower::GetAssistAimPoint(GameObject* pTargetTower)
{
  Vector3 target = Commands->Get_Position(pTargetTower);
  Vector3 height = Commands->Get_Bone_Position(pTargetTower,"muzzlea0");
  target.Z = height.Z;
  return target;
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Prism_Tower::SendAssistanceRequests(GameObject* obj)
{
  // Send out assistance requests to all adjacent towers except the one we are charging, if any
  for ( int i = 0; i < adjacentTowerCount; ++i )
  {
    if ( adjacentTowers[i] != m_target->Get_ID() && Commands->Find_Object(adjacentTowers[i]) != obj)
      Commands->Send_Custom_Event(obj, Commands->Find_Object(adjacentTowers[i]), CUSTOM_PRISMTOWER_REQUEST_CHARGING, (int)targetPriority, 0.0f );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Prism_Tower::SendEndAssistanceNotifications(GameObject* obj)
{
  // Send out end assistance notifications to all adjacent towers
  for ( int i = 0; i < adjacentTowerCount; ++i )
  {
    if (Commands->Find_Object(adjacentTowers[i]) != obj)
    {
      Commands->Send_Custom_Event(obj, Commands->Find_Object(adjacentTowers[i]), CUSTOM_PRISMTOWER_STOP_CHARGING, 0, 0.0f );
    }
  }
}

// -------------------------------------------------------------------------------------------------




/*------------------------
Health Link Script

Maintains the same health & armour between two objects, for example
the AI warminer and its gun turret
--------------------------*/

void dp88_linkHealth::Created ( GameObject* obj )
{
	parentObjID = Get_Int_Parameter ( "parentObjectId" );
	GameObject* parent = Commands->Find_Object(parentObjID);
	if ( !parent )
	{
		Destroy_Script();
		return;
	}

	// Set initial values for lastHealth / lastArmour
	lastHealth = Commands->Get_Health ( parent );
	lastArmour = Commands->Get_Shield_Strength ( parent );

	// Set our max / current health / armour to match our parent
	Set_Max_Health ( obj, Commands->Get_Max_Health(parent) );
	Commands->Set_Health ( obj, lastHealth );
	Set_Max_Shield_Strength ( obj, Commands->Get_Max_Shield_Strength(parent) );
	Commands->Set_Shield_Strength ( obj, lastArmour );

	// Start timer
	Commands->Start_Timer ( obj, this, 0.25, TIMER_LINKHEALTH );
}

void dp88_linkHealth::Timer_Expired ( GameObject* obj, int number )
{
	equaliseHealth( obj );

	// Restart timer if still alive
	if ( lastHealth + lastArmour > 0.0f )
		Commands->Start_Timer ( obj, this, 0.25, TIMER_LINKHEALTH );
}

void dp88_linkHealth::Killed ( GameObject* obj, GameObject *killer )
{
	// Destroy parent object if its still alive
	GameObject* parent = Commands->Find_Object ( parentObjID );
	if ( parent )
	{
    // If the parent is a soldier in a vehicle we can't simply kill them because things go horribly wrong. Instead
    // eject them from a vehicle and attach JFW_Timer_Destroy_Object to kill them as soon as possible.
    if ( parent->As_SoldierGameObj() && parent->As_SoldierGameObj()->Is_In_Vehicle() )
    {
      Soldier_Transition_Vehicle ( parent );
      Commands->Attach_Script ( parent, "JFW_Timer_Destroy_Object", "1.0,547859,5000.0,Death" );
      Destroy_Script();   // Prevent equaliseHealth() getting called and trying to kill the driver
    }
		else
		    Commands->Apply_Damage ( parent, 5000.00, "None", NULL );
	}
}

void dp88_linkHealth::equaliseHealth ( GameObject* obj )
{
  GameObject* parent = Commands->Find_Object(parentObjID);
  if (!parent)
  {
    Commands->Apply_Damage(obj, 5000.00, "Death", obj);
    return;
  }

  /* Work out difference in our health and armour since last check */
  float difference = ((Commands->Get_Health(obj)-lastHealth)+(Commands->Get_Shield_Strength(obj)-lastArmour));

	/* Apply damage to parent using 'None' warhead (this should be 1-1 ratio against all skin and
  shield types. Special case scenario: if the target is a soldier in a vehicle we need to use a
  special function to override the check that prevents damage to soldiers in a vehicle */
  if (0.0f != difference)
  {
    if ( parent->As_SoldierGameObj() && parent->As_SoldierGameObj()->Is_In_Vehicle() )
    {
      OffenseObjectClass offense(difference*-1.0f, ArmorWarheadManager::Get_Warhead_Type("None"), NULL);
      parent->As_SoldierGameObj()->Apply_Damage_IgnoreVehicleCheck ( offense, 1, -1 );
    }
    else
      Commands->Apply_Damage ( parent, difference*-1.0f, "None", NULL );
  }

  /* Update our health/armour from the parents new health/armour */
  Commands->Set_Health ( obj, Commands->Get_Health(parent) );
  Commands->Set_Shield_Strength ( obj, Commands->Get_Shield_Strength(parent) );

  /* Set lastHealth / lastArmour */
  lastHealth = Commands->Get_Health(obj);
  lastArmour = Commands->Get_Shield_Strength(obj);

  /* Are we dead? */
  if ( lastHealth <= 0.0f )
  {
    Commands->Set_Health ( obj, 1.0f );
    Commands->Apply_Damage ( obj, 5000.00f, "Death", obj );
    Commands->Set_Health ( parent, 1.0f );
    Commands->Apply_Damage ( parent, 5000.00f, "Death", NULL );
  }

  /* Otherwise apply 0 damage to ourselves to trigger any on-damage events
  that need to run, such as dp88_damageAnimation() */
  Commands->Apply_Damage ( obj, 0.0, "None", obj );
}






/*------------------------
Script Registrants
--------------------------*/

// Game Controller
ScriptRegistrant<dp88_AR_GameController> dp88_AR_GameController_Registrant( "dp88_AR_GameController", "enableCountry_Russia=1:int,enableCountry_Cuba=1:int,enableCountry_Iraq=1:int,enableCountry_Libya=1:int,enableCountry_America=1:int,enableCountry_France=1:int,enableCountry_Germany=1:int,enableCountry_GreatBritain=1:int,enableCountry_Korea=1:int,MirageTank_disguisePreset_1=mt_tree:string,MirageTank_disguisePreset_2=null:string,MirageTank_disguisePreset_3=null:string,Camouflage=u:string" );

// Unit scripts
//ScriptRegistrant<dp88_AR_Vehicle> dp88_AR_Vehicle_Registrant( "dp88_AR_Vehicle", "TD_attack_animName=modelfile.animfile:string,TD_attack_firstFrame=0.0:float,TD_attack_lastFrame=30.0:float,CLEG_Resistance=10:int" );

// Deployable Infantry
ScriptRegistrant<dp88_AR_Deployable_Infantry> dp88_AR_Deployable_Infantry_Registrant("dp88_AR_Deployable_Infantry", "deployedObjectPreset=null:string,deployedObjectSpaceRequired=6:float,deployAnimation=obj.obj:string,deployTime=4:float,undeployAnimation=obj.obj:string,undeployTime=4:float,deployedWeaponPowerup=null:string,deployedWeaponPowerup_veteran=null:string,deployedWeaponPowerup_elite=null:string,cannotDeployStringId=0:int,deployKeyhook=IDeploy:string,deployedArmourType=null:string,deployedArmourType_veteran=null:string,deployedArmourType_elite=null:string,deploySound:string,undeploySound:string");

// CLEG
ScriptRegistrant<dp88_AR_CLEG> dp88_AR_CLEG_Registrant("dp88_AR_CLEG","");

// CLEG resistance script
ScriptRegistrant<dp88_AR_CLEG_target> dp88_AR_CLEG_target_Registrant( "dp88_AR_CLEG_target", "resistance=20:int,clegEffectPreset=null:string" );



// Aircraft Landing Zone
ScriptRegistrant<dp88_Aircraft_LandingZone> dp88_Aircraft_LandingZone_Registrant("dp88_Aircraft_LandingZone","");
ScriptRegistrant<dp88_Aircraft_LandingZone_Aircraft> dp88_Aircraft_LandingZone_Aircraft_Registrant("dp88_Aircraft_LandingZone_Aircraft","landing_anim_name=modelfile.animfile:string,landing_anim_first_frame=0.0:float,landing_anim_last_frame=25.0:float,require_landing_zone=1:int");


// Terror Drone
/*ScriptRegistrant<dp88_AR_TerrorDrone> dp88_AR_TerrorDrone_Registrant("dp88_AR_TerrorDrone","");*/

// Remote Control Scripts
ScriptRegistrant<dp88_RemoteControlConsole> dp88_RemoteControlConsole_Registrant("dp88_RemoteControlConsole","remoteControlID:int,vehiclePreset:string,cost:int,team:int,chargeTime=0:int,accessDeniedSound:string,consoleOfflineSound:string,consoleChargingSound:string,insufficientFundsSound:string,connectionEstablishedSound:string,animationName:string,animFrameAvailable1=0:int,animFrameAvailable2=0:int,animFrameCharging1=0:int,animFrameCharging2=0:int,animFrameActive1=0:int,animFrameActive2=0:int,animFrameIdle1=0:int,animFrameIdle2=0:int,animFrameDisabled1=0:int,animFrameDisabled2=0:int");
ScriptRegistrant<dp88_RemoteControlVehicle> dp88_RemoteControlVehicle_Registrant("dp88_RemoteControlVehicle","remoteControlID:int");


// Demo Truck
ScriptRegistrant<dp88_AR_DemoTruck> dp88_AR_DemoTruck_Registrant("dp88_AR_DemoTruck","explosionPreset=Explosion_NukeBeacon:string,cinematicPreset=null:string,buildingDamageRange=50.0:float,buildingDamageWarhead=Nuke:string,buildingDamageStrength=2200.00:float");


// Paradrop scripts
ScriptRegistrant<dp88_AR_paradrop_Console> dp88_AR_paradrop_Console_Registrant("dp88_AR_paradrop_Console","team=1:int");


// Prism Tower script
ScriptRegistrant<dp88_AR_Prism_Tower> dp88_AR_Prism_Tower_Registrant("dp88_AR_Prism_Tower","Priority_Infantry=1.0:float,Splash_Infantry=0:int,Priority_Light_Vehicle=5.0:float,Priority_Heavy_Vehicle=7.0:float,Priority_VTOL=0.0:float,Min_Attack_Range=0:int,Max_Attack_Range=150:int,Animation_Model:string,Animation_Model_Bone:string,Animation:string,Animation_Idle_Start_Frame:int,Animation_Idle_End_Frame:int,Animation_Unpowered_Start_Frame:int,Animation_Unpowered_End_Frame:int,Animation_Charge_Start_Frame:int,Animation_Charge_End_Frame:int,Charge_Sound:string,Modifier_Distance=0.25:float,Modifier_Target_Damage=0.1:float,Modifier_Target_Value=0.25:float,Requires_Power=1:int,Debug=0:int,Detects_Stealth=1:int");

// Health link script
ScriptRegistrant<dp88_linkHealth> dp88_linkHealth_Registrant("dp88_linkHealth","parentObjectId=0:int");
