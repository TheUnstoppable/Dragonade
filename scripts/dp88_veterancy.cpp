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
#include "dp88_veterancy.h"
#include "dp88_custom_timer_defines.h"
#include "VehicleGameObjDef.h"
#include "ScriptableGameObj.h"
#include "GameData.h"

/******************************
Initialise static pointer lists to null
******************************/

dp88_Veterancy_Settings dp88_Veterancy_Settings::ms_instance;
dp88_veterancyUnit* dp88_veterancyUnit::playerData[128] = { NULL };
dp88_veterancyUnit* dp88_veterancyUnit::AIUnitData[256] = { NULL };
dp88_veterancyUnit::PointsCarryOverData dp88_veterancyUnit::carryOverData;// Set default values

const char dp88_veterancyUnit::ShowVeterancyPointsHookName[] = "ShowVeterancyPoints";
const char dp88_veterancyUnit::BoostVeterancyHookName[] = "BoostVeterancyLevel";




// -------------------------------------------------------------------------------------------------
// Veterancy - Settings Struct
// -------------------------------------------------------------------------------------------------

dp88_Veterancy_Settings::dp88_Veterancy_Settings()
{
  Reset();
}

// -------------------------------------------------------------------------------------------------

// Call on DLL initialisation to set default values
void dp88_Veterancy_Settings::Reset()
{
  valueMultiplier[0] = 1.0f;
  valueMultiplier[1] = 1.1f;
  valueMultiplier[2] = 1.25f;
  carryOverPercentI = 0.6f;
  carryOverPercentV = 0.6f;
  boostPrice = 0;
}




// -------------------------------------------------------------------------------------------------
// Veterancy - Controller
// -------------------------------------------------------------------------------------------------

void dp88_Veterancy_Controller::Created(GameObject* obj)
{
  dp88_Veterancy_Settings::GetInstance().valueMultiplier[1] = 1.0f + (Get_Int_Parameter("Veteran_Value_Multiplier_Percent")/100.0f);
  dp88_Veterancy_Settings::GetInstance().valueMultiplier[2] = 1.0f + (Get_Int_Parameter("Elite_Value_Multiplier_Percent")/100.0f);
  dp88_Veterancy_Settings::GetInstance().carryOverPercentI = Get_Int_Parameter("Change_Character_Carry_Over_Percent_Inf")/100.0f;
  dp88_Veterancy_Settings::GetInstance().carryOverPercentV = Get_Int_Parameter("Change_Character_Carry_Over_Percent_Veh")/100.0f;
  dp88_Veterancy_Settings::GetInstance().boostPrice = Get_Int_Parameter("Boost_Price");
}

// -------------------------------------------------------------------------------------------------

void dp88_Veterancy_Controller::Destroyed(GameObject* obj)
{
  dp88_Veterancy_Settings::GetInstance().Reset();
}

// -------------------------------------------------------------------------------------------------

// Script Registrant
ScriptRegistrant<dp88_Veterancy_Controller> dp88_Veterancy_Controller_Registrant (
  "dp88_Veterancy_Controller",
  "Veteran_Value_Multiplier_Percent=10:int,"
  "Elite_Value_Multiplier_Percent=25:int,"
  "Change_Character_Carry_Over_Percent_Inf=60:int,"
  "Change_Character_Carry_Over_Percent_Veh=60:int,"
  "Boost_Price=0:int"
);








// -------------------------------------------------------------------------------------------------
// Veterancy - Unit :: Event handler functions
// -------------------------------------------------------------------------------------------------

/* Created event :: Set all data variables to default values and
register with one of the static pointer arrays */
void dp88_veterancyUnit::Created ( GameObject* obj )
{
  objectId = Commands->Get_ID ( obj );

  // Set all values to 0 (default)
  currentLevel                = 0;
  infantryVeterancyPoints     = 0;
  vehicleVeterancyPoints      = 0;
  infantryVeteranRequirement  = 0;
  infantryEliteRequirement    = 0;
  vehicleVeteranRequirement   = 0;
  vehicleEliteRequirement     = 0;
  chevronObjId                = NULL;
  promotionChevronObjId       = NULL;
  chevronVisible              = true;

  rookieWeapon[0]             = '\0';
  rookieSkinType[0]           = '\0';
  rookieShieldType[0]         = '\0';

  hasVeteranWeaponPowerup     = (strlen(Get_Parameter("veteran_weaponPowerup")) > 0 && Is_Valid_Preset ( Get_Parameter( "veteran_weaponPowerup" ) )) ? true : false; 
  hasEliteWeaponPowerup       = (strlen(Get_Parameter("elite_weaponPowerup")) > 0 && Is_Valid_Preset ( Get_Parameter( "elite_weaponPowerup" ) )) ? true : false; 

  pilotId                     = NULL;
  deregistered                 = false;



  // Store default weapon / skin / armour types
  strcpy_s ( rookieSkinType, sizeof( rookieSkinType ), Get_Skin ( obj ) );
  strcpy_s ( rookieShieldType, sizeof( rookieShieldType ), Get_Shield_Type ( obj ) );

  if ( Get_Current_Weapon ( obj ) )
    strcpy_s ( rookieWeapon, sizeof( rookieWeapon ), Get_Current_Weapon ( obj ) );
  else
    strcpy_s ( rookieWeapon, sizeof( rookieWeapon ), "none" );



  // If unit is a soldier...
  if ( obj->As_SoldierGameObj() )
  {
    infantryVeteranRequirement = Get_Int_Parameter ( "veteran_pointsRequired" );
    infantryEliteRequirement = Get_Int_Parameter ( "elite_pointsRequired" );

    // If unit is a player...
    int playerId = Get_Player_ID ( obj );
    if ( playerId >= 0 )
    {
      /* Set player data array for this player ID to point to this object */
      playerData[playerId] = this;

      /* Keep 60% of the existing points (will be 0 unless player swapped character) */
      if ( carryOverData.playerId == playerId && The_Game()->Get_Game_Duration_S() == carryOverData.gameTime )
      {
        carryOverData.playerId = -1;
        carryOverData.gameTime = UINT_MAX;
        infantryVeterancyPoints = carryOverData.infantryVeterancyPoints;
        vehicleVeterancyPoints = carryOverData.vehicleVeterancyPoints;

        /* Trigger infantry promotions if applicable */
        if ( infantryVeterancyPoints >= infantryEliteRequirement && infantryEliteRequirement > 0 )
          promoteToElite();
        else if ( infantryVeterancyPoints >= infantryVeteranRequirement && infantryVeteranRequirement > 0 )
          promoteToVeteran();
      }

      // Install a keyhook for getting your current veterancy data
      InstallHook(ShowVeterancyPointsHookName, obj );
      if (dp88_Veterancy_Settings::GetInstance().boostPrice > 0 )
        InstallHook(BoostVeterancyHookName, obj );
    }


    // Else (if unit is a bot)
    else
    {
      /* Register AI Unit Data in first available position */
      for ( int i = 0; i < 256; ++i )
      {
        if ( AIUnitData[i] == NULL )
        {
          AIUnitData[i] = this;
          break;
        }
      }
    }
  }




  // Else if unit is a vehicle
  else if ( obj->As_VehicleGameObj() )
  {
    vehicleVeteranRequirement = Get_Int_Parameter ( "veteran_pointsRequired" );
    vehicleEliteRequirement = Get_Int_Parameter ( "elite_pointsRequired" );

    /* Register AI Unit Data in first available position (even if we are not
    actually an AI unit, it does not really matter)*/
    for ( int i = 0; i < 256; i++ )
    {
      if ( AIUnitData[i] == NULL )
      {
        AIUnitData[i] = this;
        break;
      }
    }
  }
}


/* Damaged event :: Grant veterancy points to the unit that damaged
us if they are registered as a veterancy unit */
void dp88_veterancyUnit::Damaged( GameObject *obj, GameObject *damager, float amount )
{
	// Bail out if damager == null or damager == obj
	if ( damager == NULL || Commands->Get_ID ( damager ) == Commands->Get_ID ( obj ) )
		return;

	// Calculate our veterancy points value
	float pointsValue = (float)Get_Int_Parameter("pointsValue") * dp88_Veterancy_Settings::GetInstance().valueMultiplier[currentLevel];

	// Work out our total hit points
	float totalHitPoints = Commands->Get_Max_Health(obj) + Commands->Get_Max_Shield_Strength ( obj );

	/* If damage was >0 then grant normal points, unless damager is on our team in which case we
	give nothing

	Points given: (pointsValue / total hit points) * damage */
	if ( amount > 0  && Get_Object_Type(damager) != Get_Object_Type(obj) && abs(Get_Object_Type(obj)) < 2 )
  {
		grantVeterancyPoints ( damager, (pointsValue/totalHitPoints)*amount );
  }

	/* Otherwise if damage is < 0 then it is repairs, grant veterancy points if the repairer is on
	the same team (and we are not an empty vehicle).

	Points given: 50% of ((pointsValue / total hit points) * damage repaired) */
	else if ( amount < 0 && Get_Object_Type(damager) == Get_Object_Type(obj)
		&& ( !obj->As_VehicleGameObj() || Get_Vehicle_Driver ( obj ) != NULL || Get_Vehicle_Mode(obj) == VEHICLE_TYPE_TURRET ) )
	{
		grantVeterancyPoints ( damager, ((pointsValue/totalHitPoints)*(amount*-1)) *0.50f );
	}
}


/* Killed event :: Grant veterancy points to the unit that killed
us if they are registered as a veterancy unit and deregister from
static pointer arrays */
void dp88_veterancyUnit::Killed( GameObject *obj, GameObject *killer )
{
  Deregister(obj);
}


/* Destroyed event :: Clean up any remaining chevrons and remove from
static pointer arrays if necessary. */
void dp88_veterancyUnit::Destroyed(GameObject *obj)
{
  // If we are not already deregistered that means we have some points to carry over
  if ( !deregistered && Get_Player_ID(obj) >= 0 )
  {
    carryOverData.playerId = Get_Player_ID(obj);
    carryOverData.gameTime = The_Game()->Get_Game_Duration_S();
    carryOverData.infantryVeterancyPoints = infantryVeterancyPoints * dp88_Veterancy_Settings::GetInstance().carryOverPercentI;
    carryOverData.vehicleVeterancyPoints = vehicleVeterancyPoints * dp88_Veterancy_Settings::GetInstance().carryOverPercentV;
  }

  Deregister(obj);
}


void dp88_veterancyUnit::Detach(GameObject* obj)
{
  if ( Exe != EXE_LEVELEDIT )
  {
    Deregister(obj);
  }

  MultiKeyHookScriptImpClass::Detach(obj);
}


void dp88_veterancyUnit::Deregister(GameObject* obj)
{
  if ( !deregistered )
  {
    deregistered = true;

    /* Clear chevrons */
    clearChevrons();

    /* Remove from static pointer arrays */
    if ( Get_Player_ID(obj) >= 0 )
    {
      playerData[Get_Player_ID(obj)] = NULL;

      // Remove keyhooks
      RemoveHook(ShowVeterancyPointsHookName, obj);
      RemoveHook(BoostVeterancyHookName, obj);
    }
    else
    {
      /* Find this unit and remove it */
      for ( int i = 0; i < 256; ++i )
      {
        if ( AIUnitData[i] == this )
        {
          AIUnitData[i] = NULL;
          break;
        }
      }
    }
  }
}


/* Custom event :: Process custom messages */
void dp88_veterancyUnit::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
  // Look for vehicle pilot entry
  if ( type == CUSTOM_EVENT_VEHICLE_ENTERED && pilotId == NULL )
  {
    pilotId = Commands->Get_ID ( sender );

    // Get veterancy data for the unit that entered
    dp88_veterancyUnit* vetUnit = getVeterancyData ( sender );
    if ( vetUnit )
    {
      // Reset veterancy to rookie
      demoteToRookie();

      // Apply promotions based upon driver veterancy points
      if ( vetUnit->vehicleVeterancyPoints >= vehicleEliteRequirement && vehicleEliteRequirement > 0 )
        promoteToElite();
      else if ( vetUnit->vehicleVeterancyPoints >= vehicleVeteranRequirement && vehicleVeteranRequirement > 0 )
        promoteToVeteran();

      // Set our drivers vehicle veterancy boundaries
      vetUnit->vehicleVeteranRequirement = vehicleVeteranRequirement;
      vetUnit->vehicleEliteRequirement = vehicleEliteRequirement;
    }
  }




  // Look for vehicle pilot exit
  else if ( type == CUSTOM_EVENT_VEHICLE_EXITED && pilotId != NULL && pilotId == Commands->Get_ID ( sender ) )
  {
    pilotId = NULL;

    // Reset veterancy to rookie
    demoteToRookie();

    // Apply promotions based upon our own veterancy points
    if ( vehicleVeterancyPoints >= vehicleEliteRequirement && vehicleEliteRequirement > 0 )
      promoteToElite();
    else if ( vehicleVeterancyPoints >= vehicleVeteranRequirement && vehicleVeteranRequirement > 0 )
      promoteToVeteran();

    // Get veterancy data for the unit that exited
    dp88_veterancyUnit* vetUnit = getVeterancyData ( sender );
    if ( vetUnit )
    {
      // Reset our old drivers vehicle veterancy boundaries
      vetUnit->vehicleVeteranRequirement = 0;
      vetUnit->vehicleEliteRequirement = 0;
    }
  }




  /* Look for internal message to remove old weapon following a promotion
  param = old veterancy level */
  else if ( type == CUSTOM_VETERANCY_REMOVE_OLD_WEAPON )
  {
    /* For whatever reason removing a non-selected weapon changes the
    selected weapon, so store the selected weapon here */
    char currentWeapon[128];
    strcpy_s ( currentWeapon, sizeof(currentWeapon), Get_Current_Weapon(obj) );
    
    if ( param == 1 && hasVeteranWeaponPowerup )
      Remove_Weapon ( obj, Get_Powerup_Weapon ( Get_Parameter ( "veteran_weaponPowerup" ) ) );
    else
      Remove_Weapon ( obj, rookieWeapon );

    /* Reselect current weapon (also triggers the back model of the old
    weapon to be removed so it doesn't crash on the next promotion) */
    Commands->Select_Weapon ( obj, currentWeapon );
  }
}


void dp88_veterancyUnit::Timer_Expired( GameObject *obj, int number )
{
  // Look for timer for clearing promotion chevrons
  if ( number == TIMER_VETERANCY_EXPIRE_PROMOTION_CHEVRON )
  {
    if ( promotionChevronObjId != NULL && Commands->Find_Object ( promotionChevronObjId ) != NULL )
    {
      Commands->Destroy_Object ( Commands->Find_Object ( promotionChevronObjId ) );
      promotionChevronObjId = NULL;
    }
  }

  else if ( number == TIMER_VETERANCY_CHEVRON_VISIBILITY_THINK )
  {
    GameObject* chevron = (NULL != chevronObjId) ? Commands->Find_Object(chevronObjId) : NULL;
    if ( NULL != chevron )
    {
      if ( chevronVisible )
      {
        if ( Is_Stealth(obj) || (obj->As_SoldierGameObj() && Get_Vehicle(obj)) )
          Commands->Set_Is_Rendered(chevron, (chevronVisible = false));
      }
      else
      {
        if ( !Is_Stealth(obj) && !(obj->As_SoldierGameObj() && Get_Vehicle(obj)) )
          Commands->Set_Is_Rendered(chevron, (chevronVisible = true));
      }

      Commands->Start_Timer(obj, this, 1.0f, TIMER_VETERANCY_CHEVRON_VISIBILITY_THINK);
    }
  }
}


void dp88_veterancyUnit::KeyHook(const char* logicalKey)
{
  // Get GameObject
  GameObject* obj = Commands->Find_Object ( objectId );
  if ( obj == NULL || deregistered )
    return;

  GameObject* vehicle = Get_Vehicle(obj);


  if (0 == strcmp(logicalKey, ShowVeterancyPointsHookName))
  {
    // Send infantry page
    StringClass message;
    const char *str = Get_Translated_Preset_Name (obj);
    if ( infantryVeteranRequirement > 0 )
      message.Format ( "You currently have %.2f infantry veterancy points. A %s requires %d points for Veteran status and %d points for Elite status.", infantryVeterancyPoints, str, infantryVeteranRequirement, infantryEliteRequirement );
    else
      message.Format ( "You currently have %.2f infantry veterancy points. A %s cannot be promoted", infantryVeterancyPoints, str );
    delete[] str;
    Send_Message_Player( obj,DP88_RGB_GENERAL_MSG,message );


    // Send vehicle page
    if (NULL != vehicle)
    {
      str = Get_Translated_Preset_Name (vehicle);
      if ( vehicleVeteranRequirement > 0 )
        message.Format ( "You currently have %.2f vehicle veterancy points. A %s requires %d points for Veteran status and %d points for Elite status.", vehicleVeterancyPoints, str, vehicleVeteranRequirement, vehicleEliteRequirement );
      else
        message.Format ( "You currently have %.2f vehicle veterancy points. A %s cannot be promoted", vehicleVeterancyPoints, str );
      delete[] str;
    }
    else
      message.Format ( "You currently have %.2f vehicle veterancy points.", vehicleVeterancyPoints );
    Send_Message_Player( obj,DP88_RGB_GENERAL_MSG,message );
  }




  else if(0 == strcmp(logicalKey, BoostVeterancyHookName))
  {
    int nextLevel = currentLevel+1;
    int requiredPoints = (nextLevel==1) ? infantryVeteranRequirement : infantryEliteRequirement;

    if (NULL != vehicle)
    {
      requiredPoints = 0;
      dp88_veterancyUnit* vetUnit = getVeterancyData(vehicle);
      if (NULL != vetUnit)
      {
        nextLevel = vetUnit->currentLevel+1;
        requiredPoints = (nextLevel==1) ? vehicleVeteranRequirement : vehicleEliteRequirement;
      }
    }

    if (2 < nextLevel || 0 == requiredPoints)
    {
      Send_Message_Player(obj, DP88_RGB_GENERAL_MSG, "Your current unit cannot be promoted any further");
      return;
    }
    
    float outstandingPoints = requiredPoints - (NULL != vehicle ? vehicleVeterancyPoints : infantryVeterancyPoints);
    float cost = dp88_Veterancy_Settings::GetInstance().boostPrice * outstandingPoints;
    if (Purchase_Item(obj, (int)ceil(cost)))
    {
      recieveVeterancyPoints(outstandingPoints+0.1f); // Add a bit more to "top it up" and avoid rounding errors leaving them just below the promotion boundary
      Send_Message_Player(obj, DP88_RGB_GENERAL_MSG, StringClass::getFormattedString("Your unit has been boosted to the next level at a cost of %.0f credits", cost));
    }
    else
    {
      Send_Message_Player(obj, DP88_RGB_GENERAL_MSG, StringClass::getFormattedString("You cannot afford to boost your unit at this time, you need at least %.0f credits", cost));
    }
  }
}




/******************************
Chevron functions
******************************/

/* Create chevrons */
void dp88_veterancyUnit::createChevrons()
{
  // Clear any existing chevrons
  bool startThinkTimer = (chevronObjId == NULL);
  clearChevrons();

  // Get GameObject
  GameObject* obj = Commands->Find_Object ( objectId );
  if ( obj == NULL || deregistered )
    return;

  GameObject* chevron = NULL;

  if ( obj->As_SoldierGameObj() )
  {
    // Create promoted chevron and record it's ID
    GameObject* promotionChevron = Create_Object_Attach_To_Object(obj, (2==currentLevel) ? "chev_inf_elite_promotion" : "chev_inf_veteran_promotion", "Worldbox");
    promotionChevronObjId = Commands->Get_ID( promotionChevron );

    // Start timer to destroy the promotion chevron
    Commands->Start_Timer( obj, this, 2.5f, TIMER_VETERANCY_EXPIRE_PROMOTION_CHEVRON );

    // Now create the main chevron
    chevron = Create_Object_Attach_To_Object(obj, (2==currentLevel) ? "chev_inf_elite" : "chev_inf_veteran", "Worldbox");
  }


  else if ( obj->As_VehicleGameObj() )
  {
    // Create promoted chevron and record it's ID
    GameObject* promotionChevron = Create_Object_Attach_To_Object(obj, (2==currentLevel) ? "chev_veh_elite_promotion" : "chev_veh_veteran_promotion", "Worldbox");
    promotionChevronObjId = Commands->Get_ID( promotionChevron );

    // Start timer to destroy the promotion chevron
    Commands->Start_Timer( obj, this, 2.5f, TIMER_VETERANCY_EXPIRE_PROMOTION_CHEVRON );

    // Now create the main chevron
    chevron = Create_Object_Attach_To_Object(obj, (2==currentLevel) ? "chev_veh_elite" : "chev_veh_veteran", "chevBone");
  }

  // Save the chevron ID, update its rendering state and start the think timer if required
  if ( NULL != chevron )
  {
    chevronObjId = Commands->Get_ID( chevron );
    Commands->Set_Is_Rendered(chevron, chevronVisible);

    if (startThinkTimer)
    {
      Commands->Start_Timer(obj, this, 1.0f, TIMER_VETERANCY_CHEVRON_VISIBILITY_THINK);
    }
  }
}


/* Clear chevrons */
void dp88_veterancyUnit::clearChevrons()
{
  if ( chevronObjId != NULL && Commands->Find_Object(chevronObjId) != NULL )
  {
    Commands->Destroy_Object( Commands->Find_Object(chevronObjId) );
    chevronObjId = NULL;
  }

  if ( promotionChevronObjId != NULL && Commands->Find_Object(promotionChevronObjId) != NULL )
  {
    Commands->Destroy_Object( Commands->Find_Object(promotionChevronObjId) );
    promotionChevronObjId = NULL;
  }
}




/******************************
Veterancy point functions
******************************/

/* Find veterancy unit object */
dp88_veterancyUnit* dp88_veterancyUnit::getVeterancyData( GameObject *obj )
{
	if (!obj)
	{
		return NULL;
	}
	if ( obj->As_SoldierGameObj() && Get_Player_ID ( obj ) != -1 )
		return playerData[Get_Player_ID ( obj )];
	else
	{
		/* Find this unit and return it */
		for ( int i = 0; i < 256; i++ )
		{
			if ( AIUnitData[i] != NULL && AIUnitData[i]->objectId == Commands->Get_ID ( obj ) )
				return AIUnitData[i];
		}
	}

	return NULL;
}


/* Grant veterancy points to specified unit */
void dp88_veterancyUnit::grantVeterancyPoints( GameObject *obj, float points )
{
	dp88_veterancyUnit* vetData = getVeterancyData ( obj );
	if ( vetData != NULL )
		vetData->recieveVeterancyPoints ( points );
}


/* Recieve veterancy points from another unit */
void dp88_veterancyUnit::recieveVeterancyPoints ( float points )
{
	// Get GameObject
	GameObject* obj = Commands->Find_Object ( objectId );
	if ( obj == NULL || deregistered )
		return;

	// If unit is a soldier and is not driving a vehicle give infantry veterancy points
	if ( obj->As_SoldierGameObj() && Get_Vehicle ( obj ) == NULL )
	{
		infantryVeterancyPoints += points;

		// Check for elite promotion
		if ( currentLevel < 2 && infantryVeterancyPoints >= infantryEliteRequirement && infantryEliteRequirement > 0 )
			promoteToElite();

		// Check for veteran promotion
		else if ( currentLevel < 1 && infantryVeterancyPoints >= infantryVeteranRequirement && infantryVeteranRequirement > 0 )
			promoteToVeteran();
	}


	// If unit is a vehicle or is driving a vehicle give vehicle veterancy points
	if ( obj->As_VehicleGameObj() || ( obj->As_SoldierGameObj() && Get_Vehicle ( obj ) != NULL ) )
	{
		vehicleVeterancyPoints += points;

		// Check for elite promotion
		if ( vehicleVeterancyPoints >= vehicleEliteRequirement && vehicleEliteRequirement > 0 )
		{
			// If unit is a vehicle promote self to elite
			if ( currentLevel < 2 && obj->As_VehicleGameObj() )
				promoteToElite();

			// Otherwise promote the vehicle this unit is driving to elite
			else
			{
				dp88_veterancyUnit* vetUnit = getVeterancyData ( Get_Vehicle ( obj ) );
				if ( vetUnit && vetUnit->currentLevel < 2 )
					vetUnit->promoteToElite();
			}
		}

		// Check for veteran promotion
		else if ( vehicleVeterancyPoints >= vehicleVeteranRequirement && vehicleVeteranRequirement > 0 )
		{
			// If unit is a vehicle promote self to veteran
			if ( currentLevel < 1 && obj->As_VehicleGameObj() )
				promoteToVeteran();

			// Otherwise promote the vehicle this unit is driving to veteran
			else
			{
				dp88_veterancyUnit* vetUnit = getVeterancyData ( Get_Vehicle ( obj ) );
				if ( vetUnit && vetUnit->currentLevel < 1 )
					vetUnit->promoteToVeteran();
			}
		}
	}
}

/* Promotion to veteran function */
void dp88_veterancyUnit::promoteToVeteran()
{
	// Get GameObject
	GameObject* obj = Commands->Find_Object ( objectId );
	if ( obj == NULL || deregistered )
		return;

	// Set veterancy level
	currentLevel = 1;

	// Create chevrons
	createChevrons ();

  // Send page and promotion sound if unit is a player
  if ( obj->As_SoldierGameObj() && Get_Player_ID ( obj ) >= 0 )
  {
    // Send page
    StringClass str(true);
    const char* playerName = Get_Player_Name( obj );
    str.Format("Congratulations %s, you have been promoted to veteran rank", playerName );
    delete [] playerName;
    Send_Message_Player( obj, DP88_RGB_GENERAL_MSG, str );

    // Play promotion sound
    Create_2D_WAV_Sound_Player(obj,"promotion.wav");
  }


  // Send page and promotion sound if unit driver is a player
  else if ( obj->As_VehicleGameObj() && Get_Vehicle_Driver(obj) != NULL && Get_Player_ID ( Get_Vehicle_Driver ( obj ) ) >= 0 )
  {
    GameObject* driver = Get_Vehicle_Driver(obj);

    // Send page
    StringClass str(true);
    const char* playerName = Get_Player_Name(driver);
    str.Format("Congratulations %s, your vehicle has been promoted to veteran rank", playerName );
    delete [] playerName;
    Send_Message_Player( driver, DP88_RGB_GENERAL_MSG, str );

    // Play promotion sound
    Create_2D_WAV_Sound_Player( driver, "promotion.wav" );
  }




	/* Apply new weapon if applicable */
	if ( hasVeteranWeaponPowerup )
	{
		/* Decide if we need to select this weapon once it is granted (if we
		are using the weapon it replaces then we select it */
		bool selectWeapon = ( strcmp ( Get_Current_Weapon ( obj ), rookieWeapon ) == 0 ) ? true : false;

		/* Grant new weapon */
		Commands->Give_PowerUp( obj, Get_Parameter( "veteran_weaponPowerup" ), true );
		if ( selectWeapon )
			Commands->Select_Weapon ( obj, Get_Powerup_Weapon ( Get_Parameter( "veteran_weaponPowerup" ) ) );

		/* If unit is an infantry unit remove old weapon - immediatly if not
		currently selected or after a 0.1 second delay otherwise */
		if ( obj->As_SoldierGameObj() )
		{
			if ( selectWeapon )
			{
				// Send custom after 0.1 seconds to clean up old weapon
				Commands->Send_Custom_Event ( obj, obj, CUSTOM_VETERANCY_REMOVE_OLD_WEAPON, 0, 0.1f );
			}

			// Remove immediately
			else
				Remove_Weapon ( obj, rookieWeapon );
		}
	}


  /* Set new skin/armour type if applicable */
  const char* armourType = Get_Parameter("veteran_shieldType");
  if ( strlen(armourType) > 0 && Is_Valid_Armor_Type(armourType) )
    Commands->Set_Shield_Type(obj, armourType);

  armourType = Get_Parameter("veteran_skinType");
  if ( strlen(armourType) > 0 && Is_Valid_Armor_Type(armourType) )
    Set_Skin(obj, armourType);



	/* Send custom event for other scripts to watch for (parameter 1 = veteran). Slight
	delay to ensure all scripts are created before it is sent (for when we are insta-
	promoted after buying a new character) */
	Commands->Send_Custom_Event ( obj, obj, CUSTOM_VETERANCY_PROMOTED, 1, 0.1f );
}


/* Promotion to elite function */
void dp88_veterancyUnit::promoteToElite()
{
  // Get GameObject
  GameObject* obj = Commands->Find_Object ( objectId );
  if ( obj == NULL || deregistered )
    return;

  // Set veterancy level
  int oldLevel = currentLevel;
  currentLevel = 2;

  // Create chevrons
  createChevrons();

  // Send page and promotion sound if unit is a player
  if ( obj->As_SoldierGameObj() && Get_Player_ID ( obj ) >= 0 )
  {
    // Send page
    StringClass str(true);
    const char* playerName = Get_Player_Name( obj );
    str.Format("Congratulations %s, you have been promoted to elite rank", playerName );
    delete [] playerName;
    Send_Message_Player( obj, DP88_RGB_GENERAL_MSG, str );

    // Play promotion sound
    Create_2D_WAV_Sound_Player(obj,"promotion.wav");
  }


  // Send page and promotion sound if unit driver is a player
  else if ( obj->As_VehicleGameObj() && Get_Vehicle_Driver(obj) != NULL && Get_Player_ID ( Get_Vehicle_Driver ( obj ) ) >= 0 )
  {
    GameObject* driver = Get_Vehicle_Driver(obj);

    // Send page
    StringClass str(true);
    const char* playerName = Get_Player_Name(driver);
    str.Format("Congratulations %s, your vehicle has been promoted to elite rank", playerName );
    delete [] playerName;
    Send_Message_Player( driver, DP88_RGB_GENERAL_MSG, str );

    // Play promotion sound
    Create_2D_WAV_Sound_Player( driver, "promotion.wav" );
  }




  /* Work out which new weapon to give them (if there is no elite version then
  give them the veteran version if they were promoted straight from rookie) */
  char weaponPowerup[64] = {'\0'};
  if ( hasEliteWeaponPowerup )
    strcpy_s ( weaponPowerup, sizeof(weaponPowerup), Get_Parameter ( "elite_weaponPowerup" ) );
  else if ( oldLevel == 0 && hasVeteranWeaponPowerup )
    strcpy_s ( weaponPowerup, sizeof(weaponPowerup), Get_Parameter ( "veteran_weaponPowerup" ) );


  /* Apply new weapon if applicable */
  if ( weaponPowerup[0] != '\0' && Is_Valid_Preset ( weaponPowerup ) )
  {
    bool selectWeapon = false;

    /* Decide if we need to select this weapon once it is granted (if we
    are using the weapon it replaces then we select it) */
    if ( Get_Current_Weapon(obj) == NULL || 
      ( oldLevel == 0 && strcmp ( Get_Current_Weapon ( obj ), rookieWeapon ) == 0 )
      || ( oldLevel == 1 &&
        ( hasVeteranWeaponPowerup && strcmp ( Get_Current_Weapon ( obj ), Get_Powerup_Weapon ( Get_Parameter ( "veteran_weaponPowerup" ) ) ) == 0 )
        || ( !hasVeteranWeaponPowerup && strcmp ( Get_Current_Weapon ( obj ), rookieWeapon ) == 0 ) ) )
    {
      selectWeapon = true;
    }


    /* Grant new weapon */
    Commands->Give_PowerUp( obj, weaponPowerup, true );
    if ( selectWeapon )
      Commands->Select_Weapon ( obj, Get_Powerup_Weapon ( weaponPowerup ) );

    /* If unit is an infantry unit remove old weapon - immediatly if not currently selected or after
    a 0.1 second delay otherwise */
    if ( obj->As_SoldierGameObj() )
    {
      if ( selectWeapon )
      {
        // Send custom after 0.1 seconds to clean up old weapon
        Commands->Send_Custom_Event ( obj, obj, CUSTOM_VETERANCY_REMOVE_OLD_WEAPON, oldLevel, 0.1f );
      }

      // Remove immediatly
      else
        Remove_Weapon ( obj, (hasVeteranWeaponPowerup) ? Get_Powerup_Weapon ( Get_Parameter ( "veteran_weaponPowerup" ) ) : rookieWeapon );
    }
  }


  /* Work out which new armour to give them (if there is no elite version then
  give them the veteran version if they were promoted straight from rookie) */
  const char* shieldType = Get_Parameter("elite_shieldType");
  if ( oldLevel == 0 && !Is_Valid_Armor_Type(shieldType))
    shieldType = Get_Parameter("veteran_shieldType");
  
  const char* skinType = Get_Parameter("elite_skinType");
  if ( oldLevel == 0 && !Is_Valid_Armor_Type(skinType))
    skinType = Get_Parameter("veteran_skinType");


  /* Set new skin/armour type if applicable */
  if ( strlen(shieldType) > 0 && Is_Valid_Armor_Type(shieldType) )
    Commands->Set_Shield_Type(obj, shieldType);
  
  if ( strlen(skinType) > 0 && Is_Valid_Armor_Type(skinType) )
    Set_Skin(obj, skinType);



  /* Send custom event for other scripts to watch for (parameter 2 = elite). Slight
  delay to ensure all scripts are created before it is sent (for when we are insta-
  promoted after buying a new character) */
  Commands->Send_Custom_Event ( obj, obj, CUSTOM_VETERANCY_PROMOTED, 2, 0.1f );
}


/* Demote unit to rookie - primaraily used for vehicles, will not work
fully on infantry as their original weapon is gone */
void dp88_veterancyUnit::demoteToRookie()
{
  // Get GameObject
  GameObject* obj = Commands->Find_Object ( objectId );
  if ( obj == NULL || deregistered )
    return;

  // Set rookie level
  currentLevel = 0;

  // Reset skin & armour types
  Set_Skin ( obj, rookieSkinType );
  Commands->Set_Shield_Type ( obj, rookieShieldType );

  /* Select default weapon (won't work on infantry as the weapon
  was taken off them */
  Commands->Select_Weapon ( obj, rookieWeapon );

  /* Get rid of chevrons */
  clearChevrons();



  /* Send custom event for other scripts to watch for (parameter 0 = rookie) */
  Commands->Send_Custom_Event ( obj, obj, CUSTOM_VETERANCY_PROMOTED, 0, 0 );
}




// Script Registrants
ScriptRegistrant<dp88_veterancyUnit> dp88_veterancyUnit_Registrant (
  "dp88_veterancyUnit",
  "pointsValue=0:int,"
  "veteran_pointsRequired=0:int,"
  "veteran_skinType:string,"
  "veteran_shieldType:string,"
  "veteran_weaponPowerup:string,"
  "elite_pointsRequired=0:int,"
  "elite_skinType:string,"
  "elite_shieldType:string,"
  "elite_weaponPowerup:string"
);








/* Script to grant veterancy points to whatever it is attached to */
void dp88_veterancyGrantPoints::Created ( GameObject *obj )
{
  // Wait for 1 second before taking action to prevent potential issues with this script being
  // loaded before the dp88_veterancyUnit script is loaded. Don't do this for powerups.
  if (NULL == obj->As_PhysicalGameObj() || NULL == obj->As_PhysicalGameObj()->As_PowerUpGameObj())
  {
    Commands->Send_Custom_Event(obj, obj, CUSTOM_VETERANCY_GRANT_POINTS_DELAY, 0, 1.0f);
  }
}

void dp88_veterancyGrantPoints::Custom ( GameObject *obj, int type, int param, GameObject *sender )
{
  if (CUSTOM_EVENT_POWERUP_GRANTED == type || CUSTOM_VETERANCY_GRANT_POINTS_DELAY == type)
  {
    int points = Commands->Get_Random_Int ( Get_Int_Parameter( "minPoints" ), Get_Int_Parameter( "maxPoints" ) );

    // Give the sender (either ourselves or, if we are attached to a crate, the person who picked
    // us up) some veterancy points
    dp88_veterancyUnit *veterancy_script = (dp88_veterancyUnit*)Find_Script_On_Object(sender, "dp88_veterancyUnit");
    if ( veterancy_script )
    {
      veterancy_script->recieveVeterancyPoints((float)points);

      if ( Get_Player_ID(obj) > 0 )
      {
        StringClass message;
        message.Format("You have been granted %d veterancy points!",points);
        Send_Message_Player(obj, 153, 204, 25, message);
      }
    }

    Destroy_Script();
  }
}

// Alternative name for the same script, they're functionally identical so no need to have multiple 
ScriptRegistrant<dp88_veterancyGrantPoints> dp88_veterancyCrate_Registrant (
  "dp88_veterancyCrate",
  "minPoints=1:int,"
  "maxPoints=20:int"
);

ScriptRegistrant<dp88_veterancyGrantPoints> dp88_veterancyGrantPoints_Registrant (
  "dp88_veterancyGrantPoints",
  "minPoints=1:int,"
  "maxPoints=20:int"
);








/*------------------------
Veterancy Points Link Script

Maintains the same veterancy points between two objects, for example
the AI warminer and its gun turret
--------------------------*/

void dp88_linkVetPoints::Created ( GameObject* obj )
{
	parentObjID = Get_Int_Parameter ( "parentObjectId" );
	GameObject* parent = Commands->Find_Object(parentObjID);
	if ( !parent )
	{
		Destroy_Script();
		return;
	}

  // Get veterancy script on parent and ourselves
  dp88_veterancyUnit *parentScript = (dp88_veterancyUnit*)Find_Script_On_Object(parent, "dp88_veterancyUnit");
  dp88_veterancyUnit *ourScript = (dp88_veterancyUnit*)Find_Script_On_Object(obj, "dp88_veterancyUnit");
	if ( !parentScript || !ourScript )
	{
		Destroy_Script();
		return;
	}

	// Set initial values for last points values
	//lastInfantryPoints = parentScript->
	//lastVehiclePoints = Commands->Get_Shield_Strength ( parent );

	// Set our max / current health / armour to match our parent
	//Set_Max_Health ( obj, Commands->Get_Max_Health(parent) );
	//Commands->Set_Health ( obj, lastHealth );
	//Set_Max_Shield_Strength ( obj, Commands->Get_Max_Shield_Strength(parent) );
	//Commands->Set_Shield_Strength ( obj, lastArmour );

	// Start timer
	Commands->Start_Timer ( obj, this, 0.25, TIMER_LINKHEALTH );
}

void dp88_linkVetPoints::Timer_Expired ( GameObject* obj, int number )
{
	equalisePoints( obj );

	// Restart timer if still alive
	if ( Commands->Get_Health(obj) > 0 )
		Commands->Start_Timer ( obj, this, 0.5, TIMER_LINKVETPOINTS );
}

void dp88_linkVetPoints::equalisePoints ( GameObject* obj )
{
	//GameObject* other = Commands->Find_Object ( otherObjID );

	// Compute difference in both objects (infantry points)
	//float diff_us = getInfantryVeterancyPoints(obj) - lastInfantryPoints;
	//float diff_other = getInfantryVeterancyPoints(other) - lastInfantryPoints;

	// Grant each object the points from the other object
	//if ( diff_other != 0.0f )
	//	grantInfantryVeterancyPoints ( obj, diff_other );
	//if ( diff_us != 0.0f )
	//	grantInfantryVeterancyPoints ( other, diff_us );

	//lastInfantryPoints += diff_us + diff_other;

	
	
	// Compute difference in both objects (vehicle points)
	//diff_us = getVehicleVeterancyPoints(obj) - lastVehiclePoints;
	//diff_other = getVehicleVeterancyPoints(other) - lastVehiclePoints;

	// Grant each object the points from the other object
	//if ( diff_other != 0.0f )
	//	grantVehicleVeterancyPoints ( obj, diff_other );
	//if ( diff_us != 0.0f )
	//	grantVehicleVeterancyPoints ( other, diff_us );

	//lastVehiclePoints += diff_us + diff_other;
}

ScriptRegistrant<dp88_linkVetPoints> dp88_linkVetPoints_Registrant(
	"dp88_linkVetPoints",
	"parentObjectId=0:int");








/*------------------------
Veterancy - Promotion health/armour boost
--------------------------*/

void dp88_veterancyPromotionHealthArmourIncrease::Created( GameObject *obj )
{
	// Set initial veterancy level
	m_veterancyLevel = 0;
}


void dp88_veterancyPromotionHealthArmourIncrease::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// If custom == CUSTOM_VETERANCY_PROMOTED update veterancyLevel
	if ( type == CUSTOM_VETERANCY_PROMOTED )
	{
		int oldVeterancy = m_veterancyLevel;
		m_veterancyLevel = param;

		int healthChange = 0;
		int armourChange = 0;

		// Handle promotions
		if ( oldVeterancy < m_veterancyLevel )
		{
			// Elite promotion
			if ( m_veterancyLevel == 2 )
			{
				healthChange += Get_Int_Parameter ( "eliteHealthIncrease" );
				armourChange += Get_Int_Parameter ( "eliteArmourIncrease" );
			}

			// Veteran promotion
			if ( oldVeterancy == 0 )	// Must be either veteran OR elite now, so apply veteran bonus
			{
				healthChange += Get_Int_Parameter ( "veteranHealthIncrease" );
				armourChange += Get_Int_Parameter ( "veteranArmourIncrease" );
			}
		}


		// And demotions (specifically vehicles)
		else
		{
			// Elite promotion
			if ( oldVeterancy == 2 )
			{
				healthChange -= Get_Int_Parameter ( "eliteHealthIncrease" );
				armourChange -= Get_Int_Parameter ( "eliteArmourIncrease" );
			}

			if ( m_veterancyLevel == 0 )	// Must have been either veteran OR elite, so remove veteran bonus
			{
				healthChange -= Get_Int_Parameter ( "veteranHealthIncrease" );
				armourChange -= Get_Int_Parameter ( "veteranArmourIncrease" );
			}
		}

		// Calculate new health/armour values - changing the max values might alter the
		// current values so we calculate this first to avoid problems with this
		int newHealth = (int)Commands->Get_Health(obj)+healthChange;
		int newArmour = (int)Commands->Get_Shield_Strength(obj)+armourChange;

		// Apply changes
		Set_Max_Health ( obj, Commands->Get_Max_Health(obj)+healthChange );
		Commands->Set_Health ( obj, (float)newHealth );
		Set_Max_Shield_Strength ( obj, Commands->Get_Max_Shield_Strength(obj)+armourChange );
		Commands->Set_Shield_Strength ( obj, (float)newArmour );
	}
}

ScriptRegistrant<dp88_veterancyPromotionHealthArmourIncrease> dp88_veterancyPromotionHealthArmourIncrease_Registrant(
	"dp88_veterancyPromotionHealthArmourIncrease",
	"veteranHealthIncrease=0:int,"
	"veteranArmourIncrease=0:int,"
	"eliteHealthIncrease=0:int,"
	"eliteArmourIncrease=0:int");








// -------------------------------------------------------------------------------------------------
// Veterancy - Grant Powerup
// -------------------------------------------------------------------------------------------------

void dp88_veterancyGrantPowerup::Created( GameObject *obj )
{
  //Console_Output ( "Created dp88_AR_grantDefaultWeapon\n" );
  veterancyLevel = 0;
  strcpy_s ( weaponName, sizeof(weaponName), "null" );
  strcpy_s ( oldWeapon, sizeof(oldWeapon), "null" );

  const char* powerupPreset = Get_Parameter ( "powerupPreset" );
  if ( Is_Valid_Preset(powerupPreset) )
  {
    Commands->Give_PowerUp ( obj, powerupPreset, true );
    if ( Get_Powerup_Weapon(powerupPreset) != NULL )
      strcpy_s ( weaponName, sizeof(weaponName), Get_Powerup_Weapon (powerupPreset) );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_veterancyGrantPowerup::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
  if ( type == CUSTOM_VETERANCY_PROMOTED )
  {
    // Work out if we need to grant them a new weapon
    char powerupPreset[64] = "null";
    switch ( param )
    {
      case 1:
        if ( strcmp ( Get_Parameter( "powerupPreset_veteran" ), "null" ) != 0 )
          strcpy_s ( powerupPreset, sizeof(powerupPreset), Get_Parameter ( "powerupPreset_veteran" ) );
        break;
      case 2:
        if ( strcmp ( Get_Parameter( "powerupPreset_elite" ), "null" ) != 0 )
          strcpy_s ( powerupPreset, sizeof(powerupPreset), Get_Parameter ( "powerupPreset_elite" ) );
        else if ( strcmp ( Get_Parameter( "powerupPreset_veteran" ), "null" ) != 0 )
          strcpy_s( powerupPreset, sizeof(powerupPreset), Get_Parameter ( "powerupPreset_veteran" ) );
        break;
    }

    // If we found a powerup to give them then grant it
    if ( strcmp ( powerupPreset, "null" ) != 0 && Is_Valid_Preset(powerupPreset) )
    {
      // Not a weapon powerup?
      if ( Get_Powerup_Weapon(powerupPreset) == NULL )
        Commands->Give_PowerUp( obj, powerupPreset, true );

      else
      {
        // Give new weapon, and select if if they were using that weapon
        const char* newWeapon = Get_Powerup_Weapon(powerupPreset);
        if ( strcmp ( newWeapon, weaponName ) == 0 )    // Same weapon? Nothing to do...
          return;

        if ( strcmp( Get_Current_Weapon(obj), weaponName ) == 0 )
        {
          Commands->Give_PowerUp( obj, powerupPreset, true );
          strcpy_s( oldWeapon, sizeof(oldWeapon), weaponName );
          Commands->Send_Custom_Event( obj, obj, CUSTOM_VETERANCY_REMOVE_OLD_WEAPON, 0, 0.1f);
          Commands->Select_Weapon( obj, newWeapon );
        }
        else
        {
          char currentWeapon[128];
          strcpy_s ( currentWeapon, sizeof(currentWeapon), Get_Current_Weapon(obj) );
          Commands->Give_PowerUp( obj, powerupPreset, true );
          Remove_Weapon( obj, weaponName );
          Commands->Select_Weapon( obj, currentWeapon );
        }

        // Set this weapon as their default for future reference
        strcpy_s ( weaponName, sizeof(weaponName), newWeapon );
      }
    }
  }


  else if ( type == CUSTOM_VETERANCY_REMOVE_OLD_WEAPON && strcmp ( oldWeapon, "null" ) != 0 )
  {
    /* For whatever reason removing a non-selected weapon changes the selected weapon, so store the
    selected weapon here so we can restore it afterwards */
    char currentWeapon[128];
    strcpy_s ( currentWeapon, sizeof(currentWeapon), Get_Current_Weapon(obj) );

    /* Make sure they don't have oldWeapon selected, otherwise crashy-fun-time will happen. If they
    do have it selected then swap it for their new weapon and fire the custom again */
    if ( strcmp ( currentWeapon, oldWeapon ) == 0 )
    {
      if ( strcmp ( oldWeapon, weaponName ) != 0 )  // Sanity check...
      {
        Commands->Select_Weapon ( obj, weaponName );
        Commands->Send_Custom_Event ( obj, obj, CUSTOM_VETERANCY_REMOVE_OLD_WEAPON, 0, 0.1f);
      }
    }
    else
    {
      Remove_Weapon ( obj, oldWeapon );
      strcpy_s ( oldWeapon, sizeof(oldWeapon), "null" );

      /* Reselect current weapon (also triggers the back model of the old weapon to be removed so it
      doesn't crash on the next promotion) */
      Commands->Select_Weapon ( obj, currentWeapon );
    }
  }
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_veterancyGrantPowerup> dp88_veterancyGrantPowerup_Registrant(
  "dp88_veterancyGrantPowerup",
  "powerupPreset=presetname:string,"
  "powerupPreset_veteran:string,"
  "powerupPreset_elite:string");

// Legacy registrant for existing AR presets
ScriptRegistrant<dp88_veterancyGrantPowerup> dp88_AR_grantDefaultWeapon_Registrant(
  "dp88_AR_grantDefaultWeapon",
  "powerupPreset=presetname:string,"
  "powerupPreset_veteran:string,"
  "powerupPreset_elite:string");
