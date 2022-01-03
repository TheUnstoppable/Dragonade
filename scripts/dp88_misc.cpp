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
#include "general.h"
#include "dp88_misc.h"
#include "dp88_custom_timer_defines.h"
#include "definition.h"
#include "VehicleGameObj.h"
#include "SoldierGameObj.h"
#include "SoldierGameObjDef.h"
#include "GameObjManager.h"
#include "ScriptZoneGameObj.h"
#include "RenderObjClass.h"
#include "cPlayer.h"
#include "cTeam.h"


/*------------------------
Random weather script
--------------------------*/

void dp88_randomWeather::Created ( GameObject* obj )
{
	// Start timer
	float updateInterval = (float)Commands->Get_Random_Int ( Get_Int_Parameter ( "update_interval_min" ), Get_Int_Parameter ( "update_interval_max" )+1 );
	Commands->Start_Timer ( obj, this, updateInterval, TIMER_RANDOMWEATHER );
}


void dp88_randomWeather::Timer_Expired ( GameObject* obj, int number )
{
	// Decide if we should change the weather at all
	if ( Commands->Get_Random_Int ( 0,101 ) <= Get_Int_Parameter ( "probability_change" ) )
	{
		// Time to change the weather!


		/********************
		Fog section (start / end / time)
		********************/
		float fog_start = (float)Commands->Get_Random_Int ( Get_Int_Parameter ( "fog_start_distance_min" ), Get_Int_Parameter ( "fog_start_distance_max" )+1 );
		float fog_end = (float)Commands->Get_Random_Int ( Get_Int_Parameter ( "fog_end_distance_min" ), Get_Int_Parameter ( "fog_end_distance_max" )+1 );
		
		// Check start <= end
		if ( fog_start > fog_end )
			fog_end = fog_start;

		Commands->Set_Fog_Range ( fog_start, fog_end, 2.0f );






		/********************
		Precipitation section (density / time / unknown)
		********************/
		int precipitation_type = Commands->Get_Random_Int ( 1, 101 );		// 1 to 100
		float precipitation_density = (float)(Commands->Get_Random_Int ( (int)(Get_Float_Parameter ( "precipitation_density_min" ) *100.0f), (int)((Get_Float_Parameter ( "precipitation_density_max" ) *100.0f)+1.0f) ) / 100.0f );

		
		if ( precipitation_density > 0.0f && Get_Int_Parameter ( "precipitation_probability_snow" ) >= precipitation_type )
		{
			// Set snow
			Commands->Set_Snow(precipitation_density, 2.0f, true);
			Commands->Set_Rain(0.0f, 2.0f, true);
			Commands->Set_Ash(0.0f, 2.0f, true);
		}
		else if ( precipitation_density > 0.0f && Get_Int_Parameter ( "precipitation_probability_snow" ) + Get_Int_Parameter ( "precipitation_probability_rain" ) >= precipitation_type )
		{
			// Set rain
			Commands->Set_Snow(0.0f, 2.0f, true);
			Commands->Set_Rain(precipitation_density, 2.0f, true);
			Commands->Set_Ash(0.0f, 2.0f, true);
		}
		else if ( precipitation_density > 0.0f && Get_Int_Parameter ( "precipitation_probability_snow" ) + Get_Int_Parameter ( "precipitation_probability_rain" )+ Get_Int_Parameter ( "precipitation_probability_volcanic_ash" ) >= precipitation_type )
		{
			// Set volcanic ash
			Commands->Set_Snow(0.0f, 2.0f, true);
			Commands->Set_Rain(0.0f, 2.0f, true);
			Commands->Set_Ash(precipitation_density, 2.0f, true);
		}
		else
		{
			// Set none
			Commands->Set_Rain(0.0f, 2.0f, true);
			Commands->Set_Snow(0.0f, 2.0f, true);
			Commands->Set_Ash(0.0f, 2.0f, true);
		}






		/********************
		Lightning section (intensity / start / end / heading / distribution / time)
		********************/
		int lightning_type = Commands->Get_Random_Int ( 1, 101 );		// 1 to 100
		float lightning_intensity = (float)(Commands->Get_Random_Int ( (int)(Get_Float_Parameter ( "lighting_intensity_min" ) *100.0f), (int)((Get_Float_Parameter ( "lighting_intensity_max" ) *100.0f)+1.0f) ) / 100.0f );
		float lightning_start = (float)(Commands->Get_Random_Int ( (int)(Get_Float_Parameter ( "lighting_start_distance_min" ) *100.0f), (int)((Get_Float_Parameter ( "lighting_start_distance_max" ) *100.0f)+1.0f) ) / 100.0f );
		float lightning_end = (float)(Commands->Get_Random_Int ( (int)(Get_Float_Parameter ( "lighting_end_distance_min" ) *100.0f), (int)((Get_Float_Parameter ( "lighting_end_distance_max" ) *100.0f)+1.0f) ) / 100.0f );
		float lightning_heading = (float)Commands->Get_Random_Int ( Get_Int_Parameter ( "lighting_heading_min" ), Get_Int_Parameter ( "lighting_heading_max" ) +1 );
		float lightning_distribution = (float)(Commands->Get_Random_Int ( (int)(Get_Float_Parameter ( "lighting_distribution_min" ) *100.0f), (int)((Get_Float_Parameter ( "lighting_distribution_max" ) *100.0f)+1.0f) ) / 100.0f );

		// Check ranges of variables
		if ( lightning_intensity > 1.0f )			lightning_intensity = 1.0f;
		if ( lightning_start > 1.0f )				lightning_start = 1.0f;
		if ( lightning_end > 1.0f )					lightning_end = 1.0f;
		if ( lightning_distribution > 1.0f )		lightning_distribution = 1.0f;

		// Check start <= end
		if ( lightning_start > lightning_end )
			lightning_end = lightning_start;


		if ( Get_Int_Parameter ( "lighting_probability_normal" ) >= lightning_type )
		{
			// Set lightning normal
			Commands->Set_Lightning ( lightning_intensity, lightning_start, lightning_end, lightning_heading, lightning_distribution, 2.0f );
			Commands->Set_War_Blitz ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f );
		}

		else if ( Get_Int_Parameter ( "lighting_probability_normal" ) + Get_Int_Parameter ( "lighting_probability_war_blitz" ) >= lightning_type )
		{
			// Set lightning war blitz
			Commands->Set_Lightning ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f );
			Commands->Set_War_Blitz ( lightning_intensity, lightning_start, lightning_end, lightning_heading, lightning_distribution, 2.0f );
		}

		else
		{
			// Set none
			Commands->Set_Lightning ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f );
			Commands->Set_War_Blitz ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f );
		}
		





		/********************
		Wind section (heading / strength / variability / time)
		********************/
		float wind_heading = (float)Commands->Get_Random_Int ( Get_Int_Parameter ( "wind_heading_min" ), Get_Int_Parameter ( "wind_heading_max" ) +1 );
		float wind_strength = (float)(Commands->Get_Random_Int ( (int)(Get_Float_Parameter ( "wind_speed_min" ) *100.0f), (int)((Get_Float_Parameter ( "wind_speed_max" ) *100.0f)+1.0f) ) / 100.0f );
		float wind_variability = (float)(Commands->Get_Random_Int ( (int)(Get_Float_Parameter ( "wind_variability_min" ) *100.0f), (int)((Get_Float_Parameter ( "wind_variability_max" ) *100.0f)+1.0f) ) / 100.0f );

		// Check ranges of variables
		if ( wind_variability > 1.0f )				wind_variability = 1.0f;
		
		Commands->Set_Wind ( wind_heading, wind_strength, wind_variability, 2.0f );
	}

	// Restart timer
	float updateInterval = (float)Commands->Get_Random_Int ( Get_Int_Parameter ( "update_interval_min" ), Get_Int_Parameter ( "update_interval_max" )+1 );
	Commands->Start_Timer ( obj, this, updateInterval, TIMER_RANDOMWEATHER );
}

ScriptRegistrant<dp88_randomWeather> dp88_randomWeather_Registrant(
	"dp88_randomWeather",
	"update_interval_min=20:int,"
	"update_interval_max=50:int,"
	"probability_change=20:int,"
	"fog_start_distance_min=20:int,"
	"fog_start_distance_max=40:int,"
	"fog_end_distance_min=80:int,"
	"fog_end_distance_max=100:int,"
	"precipitation_probability_snow=0:int,"
	"precipitation_probability_rain=70:int,"
	"precipitation_probability_volcanic_ash=0:int,"
	"precipitation_density_min=0.0:float,"
	"precipitation_density_max=20.0:float,"
	"lighting_probability_normal=20:int,"
	"lighting_probability_war_blitz=5:int,"
	"lighting_intensity_min=0.2:float,"
	"lighting_intensity_max=0.8:float,"
	"lighting_start_distance_min=0.2:float,"
	"lighting_start_distance_max=0.8:float,"
	"lighting_end_distance_min=0.4:float,"
	"lighting_end_distance_max=1.0:float,"
	"lighting_heading_min=0:int,"
	"lighting_heading_max=359:int,"
	"lighting_distribution_min=0.1:float,"
	"lighting_distribution_max=1.0:float,"
	"wind_heading_min=0:int,"
	"wind_heading_max=359:int,"
	"wind_speed_min=0.0:float,"
	"wind_speed_max=8.0:float,"
	"wind_variability_min=0.0:float,"
	"wind_variability_max=1.0:float"
);










/*------------------------
Unit sounds script
--------------------------*/

void dp88_unitSounds::Created ( GameObject* obj )
{
	Update_Network_Object(obj);
	healAmount = 0;
	if ( strcmp ( Get_Parameter ( "createdSound" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter ( "createdSound" ) ) )
		Commands->Create_3D_Sound_At_Bone( Get_Parameter ( "createdSound" ), obj, "origin" );


	if ( strcmp ( Get_Parameter ( "damagedSound" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter ( "damagedSound" ) ) )
		damagedEnabled = true;
	else
		damagedEnabled = false;


	if ( strcmp ( Get_Parameter ( "heavilyDamagedSound" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter ( "heavilyDamagedSound" ) ) )
		heavilyDamagedEnabled = true;
	else
		heavilyDamagedEnabled = false;


	if ( strcmp ( Get_Parameter ( "healedSound" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter ( "healedSound" ) ) )
		healedEnabled = true;
	else
		healedEnabled = false;


	if ( strcmp ( Get_Parameter ( "fallingDamageSound" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter ( "fallingDamageSound" ) ) )
		fallingDamageEnabled = true;
	else
		fallingDamageEnabled = false;
}

void dp88_unitSounds::Damaged( GameObject *obj, GameObject *damager, float amount )
{
	// Bail out if damager == null or damager == obj
	if ( damager == NULL || Commands->Get_ID ( damager ) == Commands->Get_ID ( obj ) )
		return;

	if ( amount > 0 && heavilyDamagedEnabled && amount > Get_Int_Parameter ( "heavilyDamagedSoundMinDamageAmount" ) )
	{
		if ( Commands->Get_Random_Int ( 0,101 ) <= Get_Int_Parameter ( "heavilyDamagedSoundProbability" ) )
		{
			heavilyDamagedEnabled = false;
			Commands->Create_3D_Sound_At_Bone( Get_Parameter ( "heavilyDamagedSound" ), obj, "origin" );
			Commands->Start_Timer ( obj, this, (float)Get_Int_Parameter( "heavilyDamagedSoundMinInterval" ), 2 );
		}
	}


	else if ( amount > 0 && damagedEnabled )
	{
		if ( Commands->Get_Random_Int ( 0,101 ) <= Get_Int_Parameter ( "damagedSoundProbability" ) )
		{
			damagedEnabled = false;
			Commands->Create_3D_Sound_At_Bone( Get_Parameter ( "damagedSound" ), obj, "origin" );
			Commands->Start_Timer ( obj, this, (float)Get_Int_Parameter( "damagedSoundMinInterval" ), 1 );
		}
	}


	else if ( amount < 0 && healedEnabled )
	{
		healAmount += (int)-amount;
		
		// Send delayed custom to decrement heal amount
		Commands->Send_Custom_Event ( obj, obj, CUSTOM_UNITSOUNDS_HEALDECREMENT, (int)-amount, (float)Get_Int_Parameter ( "healedSoundCheckTime" ) );

		if ( healAmount >= Get_Int_Parameter ( "healedSoundMinHealAmount" ) )
		{
			healedEnabled = false;
			Commands->Create_3D_Sound_At_Bone( Get_Parameter ( "healedSound" ), obj, "origin" );
			Commands->Start_Timer ( obj, this, (float)Get_Int_Parameter( "healedSoundMinInterval" ), 3 );
		}
	}
}

void dp88_unitSounds::Killed ( GameObject *obj, GameObject *killer )
{
	if ( strcmp ( Get_Parameter ( "killedSound" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter ( "killedSound" ) ) )
		Commands->Create_3D_Sound_At_Bone( Get_Parameter ( "killedSound" ), obj, "origin" );
}

void dp88_unitSounds::Custom ( GameObject* obj, int type, int param, GameObject* sender )
{
	if ( type == CUSTOM_EVENT_FALLING_DAMAGE && fallingDamageEnabled )
	{
		fallingDamageEnabled = false;
		Commands->Create_3D_Sound_At_Bone( Get_Parameter ( "fallingDamageSound" ), obj, "origin" );
		Commands->Start_Timer ( obj, this, (float)Get_Int_Parameter( "fallingDamageSoundMinInterval" ), 4 );
	}

	else if ( type == CUSTOM_UNITSOUNDS_HEALDECREMENT )
		healAmount -= param;
}

void dp88_unitSounds::Timer_Expired ( GameObject* obj, int number )
{
	if ( number == 1 )
		damagedEnabled = true;
	else if ( number == 2 )
		heavilyDamagedEnabled = true;
	else if ( number == 3 )
		healedEnabled = true;
	else if ( number == 4 )
		fallingDamageEnabled = true;
}


ScriptRegistrant<dp88_unitSounds> dp88_unitSounds_Registrant(
	"dp88_unitSounds",
	"createdSound=null:string,"
	"damagedSound=null:string,"
	"damagedSoundProbability=15:int,"
	"damagedSoundMinInterval=5:int,"
	"heavilyDamagedSound=null:string,"
	"heavilyDamagedSoundMinDamageAmount=40:int,"
	"heavilyDamagedSoundProbability=15:int,"
	"heavilyDamagedSoundMinInterval=5:int,"
	"healedSound=null:string,"
	"healedSoundMinHealAmount=45:int,"
	"healedSoundCheckTime=15:int,"
	"healedSoundMinInterval=30:int,"
	"fallingDamageSound=null:string,"
	"fallingDamageSoundMinInterval=5:int,"
	"killedSound=null:string"
);










/*------------------------
Unique Infantry Controller
--------------------------*/

void dp88_uniqueInfantry_controller::Created ( GameObject* obj )
{
	infantry_preset = new char[strlen(Get_Parameter("infantry_preset"))+1];
	strcpy ( infantry_preset, Get_Parameter("infantry_preset") );
	team_id = Get_Int_Parameter("team_id");
	max_count = Get_Int_Parameter("max_count");
	percent_chance = Get_Int_Parameter("percent_chance");

	// Die if infantry_preset is not valid
	if ( !Is_Valid_Preset(infantry_preset) )
	{
		Console_Output("dp88_uniqueInfantry_controller: %s is not a valid infantry preset! Destroying script...\n", infantry_preset );
		Destroy_Script();
	}
	else
	{
		hookid = 0;
		InstallHook();
	}
}

void dp88_uniqueInfantry_controller::Destroyed ( GameObject* obj )
{
	if ( infantry_preset != NULL )
		delete [] infantry_preset;
}

void dp88_uniqueInfantry_controller::ObjectCreateHook ( GameObject *obj )
{
		if ( Commands->Is_A_Star(obj)
		&& Get_Object_Type(obj) == team_id
		&& Get_Object_Count(team_id,infantry_preset) < max_count
		&& Commands->Get_Random_Int(0,100) < percent_chance )
	{
		Change_Character(obj,infantry_preset);
	}
}


ScriptRegistrant<dp88_uniqueInfantry_controller> dp88_uniqueInfantry_controller_Registrant(
	"dp88_uniqueInfantry_controller",
	"infantry_preset:string,"
	"team_id:int,"
	"max_count=1:int,"
	"percent_chance=20:int"
);










// -------------------------------------------------------------------------------------------------
// Damage animation script
// -------------------------------------------------------------------------------------------------

void dp88_damageAnimation::Created ( GameObject* obj )
{
  loopAnimation = (Get_Int_Parameter ( "loopAnimation" ) == 1);
  m_pLoopedAnimCtrl = new LoopedAnimationController(obj);   // Can also do single-iteration loops :) 

  /* Load settings */
  damageLevelBoundaries[0] = 100;
  damageLevelStartFrames[0] = Get_Int_Parameter("healthy_startFrame");
  damageLevelEndFrames[0] = Get_Int_Parameter("healthy_endFrame");
  damageLevelLowPowerStartFrames[0] = Get_Int_Parameter("healthy_startFrame_lowPower");
  damageLevelLowPowerEndFrames[0] = Get_Int_Parameter("healthy_endFrame_lowPower");

  damageLevelBoundaries[1] = Get_Int_Parameter("damageBoundary1");
  damageLevelStartFrames[1] = Get_Int_Parameter("damageBoundary1_startFrame");
  damageLevelEndFrames[1] = Get_Int_Parameter("damageBoundary1_endFrame");
  damageLevelLowPowerStartFrames[1] = Get_Int_Parameter("damageBoundary1_startFrame_lowPower");
  damageLevelLowPowerEndFrames[1] = Get_Int_Parameter("damageBoundary1_endFrame_lowPower");

  damageLevelBoundaries[2] = Get_Int_Parameter("damageBoundary2");
  damageLevelStartFrames[2] = Get_Int_Parameter("damageBoundary2_startFrame");
  damageLevelEndFrames[2] = Get_Int_Parameter("damageBoundary2_endFrame");
  damageLevelLowPowerStartFrames[2] = Get_Int_Parameter("damageBoundary2_startFrame_lowPower");
  damageLevelLowPowerEndFrames[2] = Get_Int_Parameter("damageBoundary2_endFrame_lowPower");

  damageLevelBoundaries[3] = Get_Int_Parameter("damageBoundary3");
  damageLevelStartFrames[3] = Get_Int_Parameter("damageBoundary3_startFrame");
  damageLevelEndFrames[3] = Get_Int_Parameter("damageBoundary3_endFrame");
  damageLevelLowPowerStartFrames[3] = Get_Int_Parameter("damageBoundary3_startFrame_lowPower");
  damageLevelLowPowerEndFrames[3] = Get_Int_Parameter("damageBoundary3_endFrame_lowPower");

  damageLevelBoundaries[4] = Get_Int_Parameter("damageBoundary4");
  damageLevelStartFrames[4] = Get_Int_Parameter("damageBoundary4_startFrame");
  damageLevelEndFrames[4] = Get_Int_Parameter("damageBoundary4_endFrame");
  damageLevelLowPowerStartFrames[4] = Get_Int_Parameter("damageBoundary4_startFrame_lowPower");
  damageLevelLowPowerEndFrames[4] = Get_Int_Parameter("damageBoundary4_endFrame_lowPower");

  damageLevelBoundaries[5] = Get_Int_Parameter("damageBoundary5");
  damageLevelStartFrames[5] = Get_Int_Parameter("damageBoundary5_startFrame");
  damageLevelEndFrames[5] = Get_Int_Parameter("damageBoundary5_endFrame");
  damageLevelLowPowerStartFrames[5] = Get_Int_Parameter("damageBoundary5_startFrame_lowPower");
  damageLevelLowPowerEndFrames[5] = Get_Int_Parameter("damageBoundary5_endFrame_lowPower");

  m_bUseDestroyedMode = (Get_Parameter_Count() < 33) ? false : 0 != Get_Int_Parameter("useDestroyedMode");
  m_bIsDestroyed = false;

  /* Set default level */
  currentDamageLevel = 0;
  basePowerState = Is_Base_Powered(Get_Object_Type(obj));
  SetDamageAnimation(obj);

  /* If any of the damage levels have a different set of frames for the low power state
  then start the timer to check power state regularly */
  for ( int i = 0; i < 6; i++ )
  {
    if ( damageLevelBoundaries[i] >= 0 && damageLevelLowPowerStartFrames[i] >= 0
      && (damageLevelLowPowerStartFrames[i] != damageLevelStartFrames[i]
        || damageLevelLowPowerEndFrames[i] != damageLevelEndFrames[i])
      )
    {
      Commands->Start_Timer(obj, this, 1.0f, TIMER_CHECK_BASE_POWER_STATE );
      break;
    }
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_damageAnimation::Detach ( GameObject* obj )
{
  ScriptImpClass::Detach(obj);
  if (m_pLoopedAnimCtrl)
  {
    delete m_pLoopedAnimCtrl;
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_damageAnimation::Damaged( GameObject *obj, GameObject *damager, float amount )
{
  if ( amount >= 0 )
  {
    while ( currentDamageLevel < 5 && damageLevelBoundaries[currentDamageLevel+1] >= 0
      && ((Get_Hitpoints(obj)/Get_Max_Hitpoints(obj))*100.0f) < damageLevelBoundaries[currentDamageLevel+1] )
    {
      currentDamageLevel++;
      basePowerState = Is_Base_Powered(Get_Object_Type(obj));
      SetDamageAnimation(obj);

      if ( m_bUseDestroyedMode && (currentDamageLevel == 5 || damageLevelBoundaries[currentDamageLevel+1] <= 0) )
        m_bIsDestroyed = true;
    }
  }

  // Repairs make the damage level go backwards, unless we're fully destroyed and not at 100% health
  if ( amount <= 0 && (!m_bIsDestroyed || Get_Hitpoints(obj) == Get_Max_Hitpoints(obj)) )
  {
    while ( currentDamageLevel > 0
      && ((Get_Hitpoints(obj)/Get_Max_Hitpoints(obj))*100.0f) > damageLevelBoundaries[currentDamageLevel] )
    {
      currentDamageLevel--;
      basePowerState = Is_Base_Powered(Get_Object_Type(obj));
      SetDamageAnimation(obj);
    }
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_damageAnimation::Timer_Expired ( GameObject* obj, int number )
{
  if ( number == TIMER_CHECK_BASE_POWER_STATE )
  {
    if ( Is_Base_Powered(Get_Object_Type(obj)) != basePowerState )
    {
      basePowerState = !basePowerState;
      SetDamageAnimation(obj);
    }

    // Restart timer
    Commands->Start_Timer(obj, this, 1.0f, TIMER_CHECK_BASE_POWER_STATE );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_damageAnimation::SetDamageAnimation ( GameObject* obj )
{
  int wakeup_param = Get_Parameter_Index("wakeObjects");
  bool WakeObjects = wakeup_param < Get_Parameter_Count() ? Get_Bool_Parameter(wakeup_param) : false;
  if (WakeObjects)
  {
    Wake_Up_Objects_In_Box(obj,"WAKEBOX");
  }
  if ( basePowerState || damageLevelLowPowerStartFrames[currentDamageLevel] < 0 )
    m_pLoopedAnimCtrl->PlayAnimation ( Get_Parameter("animationName"), damageLevelStartFrames[currentDamageLevel], damageLevelEndFrames[currentDamageLevel], (loopAnimation)?0:1 );
  else
    m_pLoopedAnimCtrl->PlayAnimation ( Get_Parameter("animationName"), damageLevelLowPowerStartFrames[currentDamageLevel], damageLevelLowPowerEndFrames[currentDamageLevel], (loopAnimation)?0:1 );
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_damageAnimation> dp88_damageAnimation_Registrant(
  "dp88_damageAnimation",
  "animationName=object.object:string,"
  "loopAnimation=1:int,"
  "healthy_startFrame=0:int,"
  "healthy_endFrame=0:int,"
  "healthy_startFrame_lowPower=-1:int,"
  "healthy_endFrame_lowPower=-1:int,"
  "damageBoundary1=-1:int,"
  "damageBoundary1_startFrame=0:int,"
  "damageBoundary1_endFrame=0:int,"
  "damageBoundary1_startFrame_lowPower=-1:int,"
  "damageBoundary1_endFrame_lowPower=-1:int,"
  "damageBoundary2=-1:int,"
  "damageBoundary2_startFrame=0:int,"
  "damageBoundary2_endFrame=0:int,"
  "damageBoundary2_startFrame_lowPower=-1:int,"
  "damageBoundary2_endFrame_lowPower=-1:int,"
  "damageBoundary3=-1:int,"
  "damageBoundary3_startFrame=0:int,"
  "damageBoundary3_endFrame=0:int,"
  "damageBoundary3_startFrame_lowPower=-1:int,"
  "damageBoundary3_endFrame_lowPower=-1:int,"
  "damageBoundary4=-1:int,"
  "damageBoundary4_startFrame=0:int,"
  "damageBoundary4_endFrame=0:int,"
  "damageBoundary4_startFrame_lowPower=-1:int,"
  "damageBoundary4_endFrame_lowPower=-1:int,"
  "damageBoundary5=-1:int,"
  "damageBoundary5_startFrame=0:int,"
  "damageBoundary5_endFrame=0:int,"
  "damageBoundary5_startFrame_lowPower=-1:int,"
  "damageBoundary5_endFrame_lowPower=-1:int,"
  "wakeObjects=0:int,"
  "useDestroyedMode=0:int"
);

// -------------------------------------------------------------------------------------------------










/*------------------------
Object Annoucements script
--------------------------*/

void dp88_objectAnnouncements::Created ( GameObject* obj )
{
	lastAnnouncementTime = 0;
	minimumAnnounceInterval = Get_Int_Parameter("minimumAnnounceInterval");

	/* By default we set all sound IDs to 0 */
	team0_underAttackSoundId = 0;
	team1_underAttackSoundId = 0;
	team0_destroyedSoundId = 0;
	team1_destroyedSoundId = 0;

	/* Get string IDs and for each one determine if it has a valid sound
	attached to it */
	unsigned long soundId;
	team0_underAttackStringId = (Is_Valid_String_ID(Get_Int_Parameter("team0_underAttackStringId"))) ? Get_Int_Parameter("team0_underAttackStringId") : 0;
	if ( team0_underAttackStringId && (soundId = Get_String_Sound_ID(team0_underAttackStringId)) != 0 && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000 )
		team0_underAttackSoundId = soundId;

	team1_underAttackStringId = (Is_Valid_String_ID(Get_Int_Parameter("team1_underAttackStringId"))) ? Get_Int_Parameter("team1_underAttackStringId") : 0;
	if ( team1_underAttackStringId && (soundId = Get_String_Sound_ID(team1_underAttackStringId)) != 0 && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000 )
		team1_underAttackSoundId = soundId;

	team0_destroyedStringId = (Is_Valid_String_ID(Get_Int_Parameter("team0_destroyedStringId"))) ? Get_Int_Parameter("team0_destroyedStringId") : 0;
	if ( team0_destroyedStringId && (soundId = Get_String_Sound_ID(team0_destroyedStringId)) != 0 && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000 )
		team0_destroyedSoundId = soundId;

	team1_destroyedStringId = (Is_Valid_String_ID(Get_Int_Parameter("team1_destroyedStringId"))) ? Get_Int_Parameter("team1_destroyedStringId") : 0;
	if ( team1_destroyedStringId && (soundId = Get_String_Sound_ID(team1_destroyedStringId)) != 0 && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000 )
		team1_destroyedSoundId = soundId;
}


void dp88_objectAnnouncements::Damaged( GameObject *obj, GameObject *damager, float amount )
{
	/* If damage has been done and it is more than minimumAnnounceInterval seconds since the last
	announcement was made then trigger the under attack announcement */
	if ( amount > 0 && time(NULL) - lastAnnouncementTime > minimumAnnounceInterval )
	{
    // Don't trigger above 95% health to avoid lots of annoying announcements when it takes a tiny
    // bit of damage from an explosion
    if ( 0.95f < (Commands->Get_Health(obj)+Commands->Get_Shield_Strength(obj))/(Commands->Get_Max_Health(obj)+Commands->Get_Max_Shield_Strength(obj)) )
      return;

		lastAnnouncementTime = time(NULL);

		/* Send messages to teams */
		if ( team0_underAttackStringId )
		{
			const char *str = Get_Translated_String(team0_underAttackStringId);
			Send_Message_Team(0,255,255,255,str);
			delete[] str;
			if ( team0_underAttackSoundId )
				Create_2D_Sound_Team(Get_Definition_Name(team0_underAttackSoundId),0);
		}

		if ( team1_underAttackStringId )
		{
			const char *str = Get_Translated_String(team1_underAttackStringId);
			Send_Message_Team(1,255,255,255,str);
			delete[] str;
			if ( team1_underAttackSoundId )
				Create_2D_Sound_Team(Get_Definition_Name(team1_underAttackSoundId),1);
		}
	}
}


void dp88_objectAnnouncements::Killed ( GameObject *obj, GameObject *killer )
{
	/* Send messages to teams */
	if ( team0_destroyedStringId )
	{
		const char *str = Get_Translated_String(team0_destroyedStringId);
		Send_Message_Team(0,255,255,255,str);
		delete[] str;
		if ( team0_destroyedSoundId )
			Create_2D_Sound_Team(Get_Definition_Name(team0_destroyedSoundId),0);
	}
		
	if ( team1_destroyedStringId )
	{
		const char *str = Get_Translated_String(team1_destroyedStringId);
		Send_Message_Team(1,255,255,255,str);
		delete[] str;
		if ( team1_destroyedSoundId )
			Create_2D_Sound_Team(Get_Definition_Name(team1_destroyedSoundId),1);
	}
}


ScriptRegistrant<dp88_objectAnnouncements> dp88_objectAnnouncements_Registrant(
	"dp88_objectAnnouncements",
	"minimumAnnounceInterval=20:int,"
	"team0_underAttackStringId=0:int,"
	"team1_underAttackStringId=0:int,"
	"team0_destroyedStringId=0:int,"
	"team1_destroyedStringId=0:int,"
);










/*------------------------
Chrono Tank Script
--------------------------*/

void dp88_chronoTank::Created ( GameObject* obj )
{
	state = CT_STATE_CHARGED;
	object_id = Commands->Get_ID(obj);
	driver_id = NULL;
	shifted_vehicle_object_id = NULL;

	// Can only be used on vehicles
	if ( !obj->As_VehicleGameObj() )
	{
		Console_Output ( "[%d:%s:%s] Critical Error: This script can only be used on vehicles! Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
		Destroy_Script();
    return;
	}

	// Check the shifted vehicle preset is valid
	if ( !Is_Valid_Preset(Get_Parameter("shifted_vehicle_preset")) )
	{
		Console_Output ( "[%d:%s:%s] Critical Error: A vehicle preset with the name '%s' was not found. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Parameter("shifted_vehicle_preset") );
		Destroy_Script();
	}
}


void dp88_chronoTank::Killed ( GameObject *obj, GameObject *killer )
{
	if ( shifted_vehicle_object_id != NULL )
	{
		Commands->Destroy_Object(Commands->Find_Object(shifted_vehicle_object_id));
		shifted_vehicle_object_id = NULL;
	}
}


void dp88_chronoTank::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Look for vehicle entry
	if ( type == CUSTOM_EVENT_VEHICLE_ENTERED && driver_id == NULL )
	{
		driver_id = Commands->Get_ID(sender);
		InstallHook( Get_Parameter("shift_keyhook"), sender );
	}


	// Look for vehicle exit (ignore when leaving due to chronoshift)
	else if ( type == CUSTOM_EVENT_VEHICLE_EXITED && Commands->Get_ID(sender) == driver_id && state != CT_STATE_SHIFTED_OUT_PENDING_CLEANUP_1 )
	{
		driver_id = NULL;
		RemoveHook();
	}
}


void dp88_chronoTank::Timer_Expired( GameObject *obj, int number )
{
	// Timer to advance to the next stage of the chronoshift process
	if ( number == TIMER_CHRONOTANK_ADVANCE_STATE )
	{
		// Chronoshift device has recharged, set state and send message to driver
		if ( state == CT_STATE_CHARGING )
		{
			state = CT_STATE_CHARGED;

			if ( driver_id != NULL )
				Send_Message_Player(Commands->Find_Object(driver_id),255,255,255,"The chronoshift device is now fully charged and ready for use");
		}


		// Shift out sequence has completed
		else if ( state == CT_STATE_SHIFTING_OUT )
			Shift_Out_Complete(obj);


		// Shift out sequence is ready for cleanup
		else if ( state == CT_STATE_SHIFTED_OUT_PENDING_CLEANUP_1 || state == CT_STATE_SHIFTED_OUT_PENDING_CLEANUP_2 )
			Shift_Out_Cleanup(obj);


		// Chronoshift time has expired
		else if ( state == CT_STATE_SHIFTED_OUT )
			Shift_In_Begin(obj);


		// Shift in sequence has completed
		else if ( state == CT_STATE_SHIFTING_IN )
			Shift_In_Complete(obj);


		// Shift in sequence is ready for cleanup
		else if ( state == CT_STATE_SHIFTED_IN_PENDING_CLEANUP_1 || state == CT_STATE_SHIFTED_IN_PENDING_CLEANUP_2 )
			Shift_In_Cleanup(obj);
	}


	// Timer to apply random interference actions
	else if ( number == TIMER_CHRONOTANK_RANDOM_INTERFERENCE && shifted_vehicle_object_id != NULL && state == CT_STATE_SHIFTED_OUT )
	{
		// Get shifted vehicle
		GameObject* shifted_vehicle = Commands->Find_Object(shifted_vehicle_object_id);


		// Alter heading
		if ( Get_Int_Parameter("shift_heading_fluctuation_degrees_max") > 0 )
		{
			float heading = Commands->Get_Facing (shifted_vehicle);
			Console_Output ( "Heading was %.2f\n", heading );
			heading += Commands->Get_Random_Int(Get_Int_Parameter("shift_heading_fluctuation_degrees_max")*-1,Get_Int_Parameter("shift_heading_fluctuation_degrees_max"));
			heading = (heading<0) ? heading+360 : ((heading>360) ? heading-360 : heading);
			Console_Output ( "Heading is now %.2f\n", heading );
			Commands->Set_Facing(shifted_vehicle, heading);
		}


		// Alter velocity - move x% of velocity from a random axis to another random axis
		float velocity_fluctuation_percent_max = Get_Float_Parameter("shift_velocity_fluctuation_percent_max");
		if (velocity_fluctuation_percent_max > 0 )
		{
			Vector3 velocity = Get_Velocity (shifted_vehicle);
			Console_Output ( "Velocity was %.2f, %.2f, %.2f\n", velocity.X, velocity.Y, velocity.Z );

			float avgVelocity = (abs(velocity.X) + abs(velocity.Y) + abs(velocity.Z))/3.0f;
			float percentChange = Commands->Get_Random(0.0f,velocity_fluctuation_percent_max);
			float velocityChange = avgVelocity * (percentChange/100.0f);

			// Pick a random axis to apply the changes to
			int subAxis = Commands->Get_Random_Int(1,3);
			int plusAxis = Commands->Get_Random_Int(1,3);
			while (plusAxis == subAxis)
				plusAxis = Commands->Get_Random_Int(1,3);
			Console_Output ( "Subtracting %.2f%% velocity (%.2f) from axis %d\n", percentChange, velocityChange*-1.0f, subAxis );
			Console_Output ( "Adding %.2f%% velocity (%.2f) to axis %d\n", percentChange, velocityChange, plusAxis );

			// Apply the velocity changes
			if ( plusAxis == 1 )
				velocity.X -= velocityChange;
			else if ( plusAxis == 2 )
				velocity.Y -= velocityChange;
			else if ( plusAxis == 3 )
				velocity.Z -= velocityChange;
			
			if ( subAxis == 1 )
				velocity.X += velocityChange;
			else if ( subAxis == 2 )
				velocity.Y += velocityChange;
			else if ( subAxis == 3 )
				velocity.Z += velocityChange;

			// Set new velocity
			Console_Output ( "Velocity is now %.2f, %.2f, %.2f\n", velocity.X, velocity.Y, velocity.Z );
			Set_Velocity(shifted_vehicle, velocity);
		}

		// Start timer again for the next set of interference actions
		Commands->Start_Timer(obj,this,1.00f,TIMER_CHRONOTANK_RANDOM_INTERFERENCE);
	}
}


void dp88_chronoTank::KeyHook()
{
	GameObject* obj = Commands->Find_Object(object_id);
	if ( state == CT_STATE_CHARGED )
		Shift_Out_Begin(obj);
	else if ( state == CT_STATE_CHARGING )
		Send_Message_Player(Commands->Find_Object(driver_id),255,255,255,"The chronoshift device has not finished recharging yet");
}


void dp88_chronoTank::Shift_Out_Begin(GameObject* obj)
{
	if ( state != CT_STATE_CHARGED )
		return;

	// Set new state
	state = CT_STATE_SHIFTING_OUT;

	// Lock the vehicle in it's current location and prevent the driver getting out
	obj->As_VehicleGameObj()->Set_Immovable(true);
	Commands->Enable_Vehicle_Transitions(obj,false);

	// Create the shifted out vehicle in a safe location (pre-creation reduces the transition delay)
	GameObject* shifted_vehicle = Commands->Create_Object ( Get_Parameter("shifted_vehicle_preset"), Vector3(0.0f,0.0f,500.0f) );
	shifted_vehicle_object_id = Commands->Get_ID(shifted_vehicle);

	// Start timer to advance to next stage (shift out complete)
	Commands->Start_Timer(obj,this,Get_Float_Parameter("shift_out_time"),TIMER_CHRONOTANK_ADVANCE_STATE);

	/* Create shift out effect if the preset provided is valid */
	if ( Is_Valid_Preset(Get_Parameter("shift_out_effect_preset")) )
	{
		// Create effect object
		GameObject* effect_object = Commands->Create_Object ( Get_Parameter("shift_out_effect_preset"), Commands->Get_Position(obj) );

		// Attach script to clean up effect
		StringClass params(true);
		params.Format ( "%.2f,%d", Get_Float_Parameter("shift_out_effect_time"), 983142 );
		Commands->Attach_Script ( effect_object, "JFW_Destroy_Self_Timer", params.Peek_Buffer() );
	}
}


void dp88_chronoTank::Shift_Out_Complete(GameObject* obj)
{
	// Enable vehicle transitions to allow the driver out
	Commands->Enable_Vehicle_Transitions(obj,true);

	// Transition the driver out of this vehicle
	Soldier_Transition_Vehicle(Commands->Find_Object(driver_id));

	// Set state and start timer to advance to next state (cleanup step 1) on next engine tick
	state = CT_STATE_SHIFTED_OUT_PENDING_CLEANUP_1;
	Commands->Start_Timer(obj,this,5.2f,TIMER_CHRONOTANK_ADVANCE_STATE);
}


void dp88_chronoTank::Shift_Out_Cleanup(GameObject* obj)
{
	// Cleanup step 1
	if ( state == CT_STATE_SHIFTED_OUT_PENDING_CLEANUP_1 )
	{
		// Get shifted vehicle
		GameObject* shifted_vehicle = Commands->Find_Object(shifted_vehicle_object_id);

    // Make the shifted vehicle invisible to base defences
    Commands->Set_Is_Visible(shifted_vehicle,false);

		// Move the shifted vehicle to this location
		Set_Transform(shifted_vehicle, Get_Transform(obj) );

		// Move the driver to where the shifted vehicle is
		Commands->Set_Position(Commands->Find_Object(driver_id),Commands->Get_Position(obj));

		// Move this vehicle to a safe place out of harms way and make it invisible
		Commands->Set_Position(obj,Vector3(0.0f,0.0f,500.0f));
		Commands->Set_Is_Rendered ( obj, false );

		// Set state and start timer to advance to next state (cleanup step 2) on next engine tick
		state = CT_STATE_SHIFTED_OUT_PENDING_CLEANUP_2;
		Commands->Start_Timer(obj,this,5.4f,TIMER_CHRONOTANK_ADVANCE_STATE);
	}

	// Cleanup step 2
	else
	{
		// Get shifted vehicle
		GameObject* shifted_vehicle = Commands->Find_Object(shifted_vehicle_object_id);

		// Move the driver to where the shifted vehicle is (make sure they don't walk off...)
		Commands->Set_Position(Commands->Find_Object(driver_id),Commands->Get_Position(shifted_vehicle));

		// Transition the driver into the shifted vehicle
		ActionParamsStruct params;
		params.Set_Basic((long)0,100,39);
		GameObject* driver = Commands->Find_Object(driver_id);
		if ( driver )
			Commands->Action_Enter_Exit(driver,params);

		// Set state and start timer to advance to next stage (shift in)
		state = CT_STATE_SHIFTED_OUT;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("shift_duration"),TIMER_CHRONOTANK_ADVANCE_STATE);

		// Begin timer for random interference actions
		Commands->Start_Timer(obj,this,1.5f,TIMER_CHRONOTANK_RANDOM_INTERFERENCE);
	}
}


void dp88_chronoTank::Shift_In_Begin(GameObject* obj)
{
	// Set new state
	state = CT_STATE_SHIFTING_IN;

	// Get shifted vehicle
	GameObject* shifted_vehicle = Commands->Find_Object(shifted_vehicle_object_id);

	// Lock the shifted vehicle in position
	shifted_vehicle->As_VehicleGameObj()->Set_Immovable(true);

	// Start timer to advance to next stage (shift in complete)
	Commands->Start_Timer(obj,this,Get_Float_Parameter("shift_in_time"),TIMER_CHRONOTANK_ADVANCE_STATE);

	/* Create shift in effect if the preset provided is valid */
	if ( Is_Valid_Preset(Get_Parameter("shift_in_effect_preset")) )
	{
		// Create effect object
		GameObject* effect_object = Commands->Create_Object ( Get_Parameter("shift_in_effect_preset"), Commands->Get_Position(shifted_vehicle) );

		// Attach script to clean up effect
		StringClass params(true);
		params.Format ( "%.2f,%d", Get_Float_Parameter("shift_in_effect_time"), 983142 );
		Commands->Attach_Script ( effect_object, "JFW_Destroy_Self_Timer", params.Peek_Buffer() );
	}
}


void dp88_chronoTank::Shift_In_Complete(GameObject* obj)
{
	// Get shifted vehicle
	GameObject* shifted_vehicle = Commands->Find_Object(shifted_vehicle_object_id);

	// Move this vehicle to the location of the shifted vehicle
	Set_Transform(obj, Get_Transform(shifted_vehicle) );

	// Make this vehicle visible again
	Commands->Set_Is_Rendered(obj, true);

	// Destroy the shifted vehicle
	Commands->Destroy_Object(Commands->Find_Object(shifted_vehicle_object_id));
	shifted_vehicle_object_id = NULL;

	// Set state
	state = CT_STATE_SHIFTED_IN_PENDING_CLEANUP_1;

	// Start timer to advance to next state (cleanup) on next engine tick
	Commands->Start_Timer(obj,this,5.01f,TIMER_CHRONOTANK_ADVANCE_STATE);
}


void dp88_chronoTank::Shift_In_Cleanup(GameObject* obj)
{
	// Cleanup step 1
	if ( state == CT_STATE_SHIFTED_IN_PENDING_CLEANUP_1 )
	{
		// Move driver to location of this vehicle
		Commands->Set_Position(Commands->Find_Object(driver_id),Commands->Get_Position(obj));

		// Set state and start timer to advance to next state (cleanup step 2) on next engine tick
		state = CT_STATE_SHIFTED_IN_PENDING_CLEANUP_2;
		Commands->Start_Timer(obj,this,5.4f,TIMER_CHRONOTANK_ADVANCE_STATE);
	}

	// Cleanup step 2
	else
	{
		// Transition the driver into this vehicle
		ActionParamsStruct params;
		params.Set_Basic((long)0,100,39);
		GameObject* driver = Commands->Find_Object(driver_id);
		if ( driver )
			Commands->Action_Enter_Exit(driver,params);

		// Allow this vehicle to be mobile again
		obj->As_VehicleGameObj()->Set_Immovable(false);

		// Set state and start timer to advance to next stage (charged)
		state = CT_STATE_CHARGING;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("recharge_time"),TIMER_CHRONOTANK_ADVANCE_STATE);
	}
}


ScriptRegistrant<dp88_chronoTank> dp88_chronoTank_Registrant(
	"dp88_chronoTank",
	"shift_keyhook:string,"
	"shift_duration:int,"
	"recharge_time:int,"
	"shifted_vehicle_preset:string,"
	"shift_out_time:float,"
	"shift_out_effect_preset:string,"
	"shift_out_effect_time:float,"
	"shift_in_time:float,"
	"shift_in_effect_preset:string,"
	"shift_in_effect_time:float,"
	"shift_heading_fluctuation_degrees_max=10:int,"
	"shift_velocity_fluctuation_percent_max=10:float"
);










/*------------------------
Spawn Vehicle Part script
--------------------------*/

void dp88_spawnVehiclePart::Created ( GameObject* obj )
{
	partObjectId = NULL;

	/* Start a timer to spawn the vehicle part. We can't do this immediatly because
  it seems to cause issues with double initializations of scripts... */
	Commands->Start_Timer ( obj, this, 0.01f, TIMER_SPAWNVEHICLEPART );
}


void dp88_spawnVehiclePart::Timer_Expired( GameObject *obj, int number )
{
	/* Part spawn timer */
	if ( number == TIMER_SPAWNVEHICLEPART && partObjectId == NULL )
	{
		/* Spawn the vehicle part and get it's ID */
		GameObject* part = Commands->Create_Object_At_Bone ( obj, Get_Parameter("Vehicle_Part_Preset"), Get_Parameter("Spawn_At_Bone") );
		Commands->Attach_To_Object_Bone ( part, obj, Get_Parameter ( "Spawn_At_Bone" ) );
		partObjectId = Commands->Get_ID(part);

		/* Unless this is a decorative part start the timer again to attach the health and
		veterancy link scripts. We don't do this immediately as it causes a lovely crash... */
		if ( Get_Int_Parameter("IsDecorative") == 0 )
		  Commands->Start_Timer ( obj, this, 0.01f, TIMER_SPAWNVEHICLEPART );
	}


	/* Link health & veterancy script attach timer */
	else if ( number == TIMER_SPAWNVEHICLEPART )
	{
		GameObject* part = Commands->Find_Object(partObjectId);
		if ( part )
		{
			Attach_Script_Once_V ( part, "dp88_linkHealth", "%d", Commands->Get_ID(obj) );
			Attach_Script_Once_V ( part, "dp88_linkVetPoints", "%d", Commands->Get_ID(obj) );
		}
	}
}


void dp88_spawnVehiclePart::Killed ( GameObject* obj, GameObject *killer )
{
	if ( partObjectId != NULL && Commands->Find_Object(partObjectId) )
		Commands->Destroy_Object ( Commands->Find_Object (partObjectId) );
	partObjectId = NULL;
}


void dp88_spawnVehiclePart::Destroyed ( GameObject* obj )
{
	if ( partObjectId != NULL && Commands->Find_Object(partObjectId) )
		Commands->Destroy_Object ( Commands->Find_Object (partObjectId) );
	partObjectId = NULL;
}


ScriptRegistrant<dp88_spawnVehiclePart> dp88_spawnVehiclePart_Registrant(
	"dp88_spawnVehiclePart",
	"Vehicle_Part_Preset:string,"
	"Spawn_At_Bone:string,"
	"IsDecorative=0:int"
);








/*------------------------
Custom Points script
--------------------------*/

void dp88_customPoints::Created ( GameObject* obj )
{
  // Preload values since converting strings to floats every time we take
  // damage is very inefficient
  m_killPoints		= Get_Float_Parameter("killPoints");
  m_damagePoints	= Get_Float_Parameter("damagePoints");
  m_repairPoints	= Get_Float_Parameter("repairPoints");
  m_killMoney		= Get_Float_Parameter("killMoney");
  m_damageMoney		= Get_Float_Parameter("damageMoney");
  m_repairMoney		= Get_Float_Parameter("repairMoney");
}

// -------------------------------------------------------------------------------------------------

void dp88_customPoints::Damaged ( GameObject *obj, GameObject *damager, float amount )
{
  // Abort if there is no damager, or the unit damaged itself, or the damager is not
  // a star
  if ( !damager || damager == obj || !Commands->Is_A_Star(damager) )
    return;

  /* Damaged or repaired? Then convert amount into it's absolute value for
  calculating points granted */
  bool bRepair = (amount < 0);
  amount = abs(amount);
  float points = ((bRepair) ? m_repairPoints : m_damagePoints) * amount;
  float money = ((bRepair) ? m_repairMoney : m_damageMoney) * amount;

  // Is this player an APB spy?
  bool bSpy = Is_Spy(damager);

  // Repaired other team or damaged own team? That would be negative points and no
  // money for you then... unless your spy, then the logic is inverted...
  if ( bSpy ^ ((Get_Object_Type(obj) == Get_Object_Type(damager)) != bRepair) )
  {
    points *= -1.0f;
    money = 0.0f;
  }

  // Give the damager their points and money
  GivePointsAndMoney ( damager, points, money );
}

// -------------------------------------------------------------------------------------------------

void dp88_customPoints::Killed ( GameObject* obj, GameObject *killer )
{
  // Abort if there is no killer, or the unit killed itself, or the killer is not
  // a star
  if ( !killer || killer == obj || !Commands->Is_A_Star(killer) )
    return;

  // Killed own teammate? That would be negative points and no cash for you then...
  if ( Get_Object_Type(obj) == Get_Object_Type(killer) )
  {
    GivePointsAndMoney ( killer, m_killPoints*-1.0f, 0.0f );
  }

  else
  {
    // Give the killer their points and money
    GivePointsAndMoney ( killer, m_killPoints, m_killMoney );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_customPoints::GivePointsAndMoney ( GameObject* obj, float points, float money )
{
  int playerId = Get_Player_ID(obj);
  if ( playerId >= 0 )
  {
    if ( cPlayer* player = Find_Player(playerId) )
    {
      player->Set_Score(player->Get_Score()+points);
      player->Increment_Money(money);
    }
  }

  int team = Get_Object_Type(obj);
  Set_Team_Score(team, Get_Team_Score(team) + points);
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_customPoints> dp88_customPoints_Registrant(
  "dp88_customPoints",
  "killPoints:float,"
  "damagePoints:float,"
  "repairPoints:float,"
  "killMoney:float,"
  "damageMoney:float,"
  "repairMoney:float"
);








/*------------------------
Conquest Controller
--------------------------*/

void dp88_conquestController_ObjectCreateHook(void *data,GameObject *obj)
{
  Set_Death_Points(obj, 0);
  Set_Damage_Points(obj, 0);
}

// -------------------------------------------------------------------------------------------------

void dp88_conquestController::Created ( GameObject* pSelf )
{
  m_scoringMode = (unsigned char)Get_Int_Parameter("ScoringMode");

  m_targets[0] = Get_Int_Parameter("TargetScoreTeam0");
  m_targets[1] = Get_Int_Parameter("TargetScoreTeam1");

  m_points[0] = ( m_scoringMode == MODE_DEFAULT ) ? 0 : m_targets[0];
  m_points[1] = ( m_scoringMode == MODE_DEFAULT ) ? 0 : m_targets[1];

  // Do game intro after 30 seconds, giving people time to load up
  Commands->Start_Timer( pSelf, this, 30.0f, TIMER_CONQUEST_DOINTRO );

  // First set scores for all existing objects
  SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
  while (x)
  {
    ScriptableGameObj* o = x->Data()->As_ScriptableGameObj();
    Set_Death_Points(o, 0);
    Set_Damage_Points(o, 0);
    x = x->Next();
  }

  // And now hook all new objects
  ObjectCreateHookStruct* h = new ObjectCreateHookStruct();
  h->data = this;
  h->hook = dp88_conquestController_ObjectCreateHook;
  m_objectCreateHookId = AddObjectCreateHook(h);
}

// -------------------------------------------------------------------------------------------------

void dp88_conquestController::Destroyed(GameObject* pSelf)
{
  RemoveObjectCreateHook(m_objectCreateHookId);
}

// -------------------------------------------------------------------------------------------------

void dp88_conquestController::Timer_Expired ( GameObject* pSelf, int number )
{
  if ( number == TIMER_CONQUEST_DOINTRO )
  {
    // Show intro
    int stringId = Get_String_ID_By_Desc(Get_Parameter("IntroString"));
    if ( stringId != 0 )
    {
      DisplayMessage(StringClass(Get_Translated_String(stringId)));
    }

    // Start scores timer if interval > 0
    int scoreInterval = Get_Int_Parameter("ScoreInterval");
    if ( scoreInterval > 0 )
      Commands->Start_Timer( pSelf, this, (float)scoreInterval, TIMER_CONQUEST_DOSCORES );
  }


  else if ( number == TIMER_CONQUEST_DOSCORES )
  {
    // Show scores
    int winningTeam = GetWinningTeamID();
    int stringId = 0;
    if ( 2 != winningTeam )
      stringId = Get_String_ID_By_Desc(Get_Parameter(((0==winningTeam)?"ScoreStringTeam0":"ScoreStringTeam1")));

    if ( 0 == stringId )
      stringId = Get_String_ID_By_Desc(Get_Parameter("ScoreString"));
    
    if ( stringId != 0 )
    {
      DisplayMessage(StringClass(Get_Translated_String(stringId)));
    }
    else
    {
      if ( m_scoringMode == MODE_DEDUCTION )
        DisplayMessage(StringClass("Scores are currently: %TEAM0%: %TEAM0SCORE%, %TEAM1%: %TEAM1SCORE%"));
      else
        DisplayMessage(StringClass("Scores are currently: %TEAM0%: (%TEAM0SCORE%/%TEAM0TARGET%), %TEAM1%: (%TEAM1SCORE%/%TEAM1TARGET%)"));
    }

    Commands->Start_Timer( pSelf, this, (float)Get_Int_Parameter("ScoreInterval"), TIMER_CONQUEST_DOSCORES );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_conquestController::Custom ( GameObject* pSelf, int type, int param, GameObject* pSender )
{
  if ( type == CUSTOM_CONQUEST_GIVEPOINTS_T0 || type == CUSTOM_CONQUEST_GIVEPOINTS_T1 )
  {
    int team = (type==CUSTOM_CONQUEST_GIVEPOINTS_T0) ? 0 : 1;
    int otherteam = (team==0) ? 1 : 0;

    // In normal scoring mode grant <param> points to the earning team
    if ( m_scoringMode == MODE_DEFAULT )
    {
      m_points[team] += param;

      // Have we won?
      if ( m_points[team] >= m_targets[team] )
      {
        BaseControllerClass::Find_Base(otherteam)->Set_Base_Destroyed(true);
      }
    }

    // In deduction scoring mode deduct <param> points from the opposing team
    else if ( m_scoringMode == MODE_DEDUCTION )
    {
      m_points[otherteam] -= param;

      // Have they lost?
      if ( m_points[otherteam] < 0 )
      {
        BaseControllerClass::Find_Base(otherteam)->Set_Base_Destroyed(true);
      }
    }

    // Update the scores
    Set_Team_Score(0, (float)m_points[0]);
    Set_Team_Score(1, (float)m_points[1]);
  }
}

// -------------------------------------------------------------------------------------------------

int dp88_conquestController::GetWinningTeamID()
{
  if ( m_scoringMode == MODE_DEFAULT )
  {
    // In default mode the "winner" is the team with the least points left to earn
    int team0_togo = m_targets[0]-m_points[0];
    int team1_togo = m_targets[1]-m_points[1];

    if ( team0_togo == team1_togo )
      return 2;

    return ( team1_togo < team0_togo ) ? 1 : 0;
  }

  else if ( m_scoringMode == MODE_DEDUCTION )
  {
    // In deduction mode the "winner" is the team with the most points remaining
    if ( m_points[0] == m_points[1] )
      return 2;

    return ( m_points[0] > m_points[1] ) ? 0 : 1;
  }

  return 2;
}

// -------------------------------------------------------------------------------------------------

int dp88_conquestController::GetLosingTeamID()
{
  return (GetWinningTeamID() == 0) ? 1 : 0;
}

// -------------------------------------------------------------------------------------------------

void dp88_conquestController::DisplayMessage(StringClass message)
{
  message.Replace("%TEAM0%", Get_Team_Name(0), false);
  message.Replace("%TEAM1%", Get_Team_Name(1), false);

  int winningTeam = GetWinningTeamID();
  message.Replace("%WINNINGTEAM%", ((2==winningTeam) ? "Neither" : Get_Team_Name(winningTeam)), false);
  message.Replace("%LOSINGTEAM%", ((2==winningTeam) ? "Neither" : Get_Team_Name(GetLosingTeamID())), false);

  message.Replace("%TEAM0TARGET%", StringClass::getFormattedString("%d",m_targets[0]), false);
  message.Replace("%TEAM1TARGET%", StringClass::getFormattedString("%d",m_targets[1]), false);

  message.Replace("%TEAM0SCORE%", StringClass::getFormattedString("%d",m_points[0]), false);
  message.Replace("%TEAM1SCORE%", StringClass::getFormattedString("%d",m_points[1]), false);

  if ( m_scoringMode == MODE_DEFAULT )
  {
    message.Replace("%TEAM0SCORENEEDED%", StringClass::getFormattedString("%d",m_targets[0]-m_points[0]), false);
    message.Replace("%TEAM1SCORENEEDED%", StringClass::getFormattedString("%d",m_targets[1]-m_points[1]), false);
  }
  else if ( m_scoringMode == MODE_DEDUCTION )
  {
    message.Replace("%TEAM0SCORENEEDED%", StringClass::getFormattedString("%d",m_points[1]), false);
    message.Replace("%TEAM1SCORENEEDED%", StringClass::getFormattedString("%d",m_points[0]), false);
  }

  Send_Message(50,200,50,message);
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_conquestController> dp88_conquestController_Registrant(
  "dp88_conquestController",
  "ScoringMode=0:int,"
  "TargetScoreTeam0=100:int,"
  "TargetScoreTeam1=100:int,"
  "IntroString:string,"
  "ScoreInterval=60:int,"
  "ScoreString:string,"
  "ScoreStringTeam0:string,"
  "ScoreStringTeam1:string"
);










/*------------------------
Conquest Control Zone
--------------------------*/

void dp88_conquestControlZone::Created ( GameObject* pObj )
{
  // Find the controller
  GameObject* pController = Find_Object_With_Script("dp88_conquestController");
  if ( !pController )
  {
    Console_Output ( "[%d:%s:%s] Critical Error: Conquest controller object not found. Destroying script...\n", Commands->Get_ID(pObj), Commands->Get_Preset_Name(pObj), this->Get_Name() );
    Destroy_Script();
    return;
  }
  m_controllerID = Commands->Get_ID(pController);

  // Validate zone preset
  if ( !Is_Valid_Preset(Get_Parameter("ZonePreset")) )
  {
    Console_Output ( "[%d:%s:%s] Critical Error: A script zone preset with the name '%s' was not found. Destroying script...\n", Commands->Get_ID(pObj), Commands->Get_Preset_Name(pObj), this->Get_Name(), Get_Parameter("ZonePreset") );
    Destroy_Script();
    return;
  }

  // Determine the size and rotation for the control zone
  Vector3 position;
  Vector3::Add ( Commands->Get_Position(pObj), Get_Vector3_Parameter("ZoneOffset"), &position );
  Vector3 size = Get_Vector3_Parameter("ZoneSize");
  Matrix3 rotation(true);
  rotation.Rotate_Z(DEG2RAD(Commands->Get_Facing(pObj)));

  // Define the bounding box and create the zone
  OBBoxClass zoneBoundingBox ( position, size, rotation );
  GameObject* pZone = Create_Zone(Get_Parameter("ZonePreset"), zoneBoundingBox );
  m_zoneID = Commands->Get_ID(pZone);

  // Load settings
  m_captureTime             = Get_Int_Parameter("CaptureTime");
  m_bAllowMajorityCapture   = (Get_Int_Parameter("AllowMajorityCapture") > 0);
  m_multiCaptureCap         = Get_Int_Parameter("MultiCaptureCap");
  m_multiCaptureMultiplier  = Get_Float_Parameter("MultiCaptureMultiplier");
  m_nAnimTransitionFrames   = Get_Int_Parameter("CaptureAnimTransitionFrames");

  if ( m_captureTime <= 0 || m_multiCaptureCap <= 0 || m_multiCaptureMultiplier <= 0.0f || m_nAnimTransitionFrames < 0 )
  {
    Console_Output ( "[%d:%s:%s] Critical Error: Invaild script parameters. Destroying script...\n", Commands->Get_ID(pObj), Commands->Get_Preset_Name(pObj), this->Get_Name() );
    Destroy_Script();
    return;
  }
  
  // Read AI objective priorities
  m_aiPriority_soldier      = Get_Int_Parameter("AIObjective_Priority_Soldier");
  m_aiPriority_lvehicle     = Get_Int_Parameter("AIObjective_Priority_Light_Vehicle");
  m_aiPriority_hvehicle     = Get_Int_Parameter("AIObjective_Priority_Heavy_Vehicle");
  m_aiPriority_aircraft     = Get_Int_Parameter("AIObjective_Priority_Aircraft");

  // Determine starting team and, if not neutral, start the tick timer to grant points per tick
  int team = (unsigned char)Get_Object_Type(pObj);
  if (0 == team || 1 == team)
  {
    ChangeOwner(pObj, team);
    Commands->Start_Timer( pObj, this, (float)Get_Int_Parameter("TickInterval"), TIMER_CONQUEST_TICK );
    m_bTickRunning = true;
  }
  else
  {
    // Force correct neutral team
    ChangeOwner(pObj, 2);
    m_bTickRunning = false;
  }

  // Start the think timer to track players in the zone and update ownership accordingly
  Commands->Start_Timer( pObj, this, 1.0f, TIMER_CONQUEST_THINK );
}

// -------------------------------------------------------------------------------------------------

void dp88_conquestControlZone::Timer_Expired ( GameObject* pObj, int number )
{
  ScriptZoneGameObj* pZone = (ScriptZoneGameObj*)Commands->Find_Object(m_zoneID);
  if ( !pZone || !((GameObject*)pZone)->As_ScriptZoneGameObj() )   // Sanity checking
    return;

  if ( number == TIMER_CONQUEST_THINK )
  {
    // Count the objects inside the zone - not we don't simply use Get_Object_Count_In_Zone because
    // we don't want to count vehicles *and* their drivers... but we do want to allow AI units to
    // contribute. Also ignore dead units...
    int nPlayers[2] = {0};
    for (SLNode<SmartGameObj> *z = GameObjManager::SmartGameObjList.Head();z;z = z->Next())
    {
      SmartGameObj* o = z->Data();
      int team = Get_Object_Type(o);
      if ( Commands->Get_Health(o) == 0 || team < 0 || team > 2 || !pZone->Inside_Me(o) )
        continue;

      // Only AI-enabled vehicles count
      if ( o->As_VehicleGameObj() && !((VehicleGameObj*)o)->Get_Action()->Is_Acting() )
        continue;

      nPlayers[team]++;
    }

    // Check which team currently controls the area inside the zone (and can therefore capture it)
    int capturingTeam = -1;
    if ( nPlayers[0] != nPlayers[1] )
    {
      if ( nPlayers[0] > nPlayers[1] && (nPlayers[1] == 0 || m_bAllowMajorityCapture) )
        capturingTeam = 0;
      else if ( nPlayers[1] > nPlayers[0] && (nPlayers[0] == 0 || m_bAllowMajorityCapture) )
        capturingTeam = 1;
    }

    // Increment the capture progress for the team in control of the area inside the zone
    if ( capturingTeam != -1 )
      IncrementCaptureProgress ( pObj, capturingTeam, nPlayers[capturingTeam]-nPlayers[(capturingTeam==0)?1:0] );


    // Otherwise revert to previous state if partially captured and nobody is in the zone (don't
    // trigger this if there are players in the zone but they don't currently qualify to capture it,
    // avoids losing capture progress whilst fending off enemy attackers)
    else if ( (nPlayers[0] == 0 && nPlayers[1] == 0) && m_captureState != 0.0f && abs(m_captureState) != m_captureTime )
    {
      int team = Get_Object_Type(pObj);
      int targetState = (team==2) ? 0 : ((team==0) ? (m_captureTime*-1) : m_captureTime);

      if ( targetState > m_captureState )
      {
        m_captureState += 1.0f;
        if ( m_captureState > targetState )
          m_captureState = (float)targetState;
      }
      else
      {
        m_captureState -= 1.0f;
        if ( m_captureState < targetState )
          m_captureState = (float)targetState;
      }

      UpdateAnimationFrame(pObj);
    }


    // Restart timer
    Commands->Start_Timer( pObj, this, 1.0f, TIMER_CONQUEST_THINK );
  }




  else if ( number == TIMER_CONQUEST_TICK )
  {
    int team = Get_Object_Type(pObj);
    if ( team != 2 )
    {
      GameObject* pController = Commands->Find_Object(m_controllerID);
      if ( !pController )
        return;

      // Grant points to owning team
      int custom = (team==0) ? CUSTOM_CONQUEST_GIVEPOINTS_T0 : CUSTOM_CONQUEST_GIVEPOINTS_T1;
      Commands->Send_Custom_Event ( pObj, pController, custom, Get_Int_Parameter("TickPoints"), 0.0f);

      Commands->Start_Timer( pObj, this, (float)Get_Int_Parameter("TickInterval"), TIMER_CONQUEST_TICK );
    }
    else
      m_bTickRunning = false;
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_conquestControlZone::IncrementCaptureProgress( GameObject* pObj, int team, int nPlayers )
{
  // If the team already fully controls the zone then don't do anything
  int targetState = (team==0) ? (m_captureTime*-1) : m_captureTime;
  if ( m_captureState == targetState || nPlayers == 0 )
    return;

  // Apply multi capture cap and work out the additional progress to be applied
  nPlayers = min(nPlayers,m_multiCaptureCap);
  float progress = 1.0f + ((nPlayers-1)*m_multiCaptureMultiplier);
  float oldCaptureState = m_captureState;

  // Apply progress
  if ( team == 0 )
  {
    m_captureState -= progress;
    if ( m_captureState < targetState )
      m_captureState = (float)targetState;
  }
  else
  {
    m_captureState += progress;
    if ( m_captureState > targetState )
      m_captureState = (float)targetState;
  }


  // Have we taken control of the zone?
  if ( m_captureState == targetState && Get_Object_Type(pObj) != team )
  {
    // Send control taken message
    ChangeOwner(pObj, team);

    int stringId = Get_String_ID_By_Desc(Get_Parameter("CapturedString"));
    if ( stringId != 0 )
    {
      DisplayMessage(pObj, StringClass(Get_Translated_String(stringId)));
    }
    else
    {
      DisplayMessage(pObj, StringClass("A conquest zone has been captured by %OWNINGTEAM%"));
    }

    // \todo Give capture points

    if ( !m_bTickRunning )
      Commands->Start_Timer( pObj, this, (float)Get_Int_Parameter("TickInterval"), TIMER_CONQUEST_TICK );
  }


  // Has the zone become neutral?
  else if ( (m_captureState == 0.0f || (team == 0 && m_captureState < 0.0f) || (team == 1 && m_captureState > 0.0f))
    && Get_Object_Type(pObj) != 2 )
  {
    // Send neutral message
    ChangeOwner(pObj, 2);

    int stringId = Get_String_ID_By_Desc(Get_Parameter("NeutralString"));
    if ( stringId != 0 )
    {
      DisplayMessage(pObj, StringClass(Get_Translated_String(stringId)));
    }
    else
    {
      DisplayMessage(pObj, StringClass("A conquest zone has become neutral"));
    }
  }


  // Display warning message to owning team if it has just started being captured
  if ( oldCaptureState == targetState*-1 )
  {
    int stringId = Get_String_ID_By_Desc(Get_Parameter("CapturingString"));
    if ( stringId != 0 )
    {
      DisplayMessage(pObj, StringClass(Get_Translated_String(stringId)), (team==0)?1:0);
    }
  }

  UpdateAnimationFrame(pObj);
}

// -------------------------------------------------------------------------------------------------

void dp88_conquestControlZone::UpdateAnimationFrame( GameObject* pObj )
{
  int animFrame = 1+m_nAnimTransitionFrames;  // Neutral frame
  int teamFrames = (int)floor((1+m_nAnimTransitionFrames) * (abs(m_captureState)/m_captureTime));
  animFrame += (m_captureState<0) ? (teamFrames*-1) : teamFrames;
  Commands->Set_Animation_Frame(pObj, Get_Parameter("CaptureAnim"), animFrame);
}

// -------------------------------------------------------------------------------------------------

void dp88_conquestControlZone::ChangeOwner(GameObject* pObj, int team)
{
  // Get parameters for objectives
  bool bAddObjectives = (0 < m_aiPriority_soldier || 0 < m_aiPriority_lvehicle || 0 < m_aiPriority_hvehicle || 0 < m_aiPriority_aircraft);
  Vector3 size = Get_Vector3_Parameter("ZoneSize");
  int distance = (int)(min(size.X, size.Y)*0.75f);
  
  // Remove old objective scripts
  if (bAddObjectives)
  {
    Remove_Script(pObj,"dp88_AI_Objective");
  }
  
  if (0 == team || 1 == team)
  {
    Set_Object_Type(pObj, team);
    m_captureState = (float)((team==0) ? (m_captureTime*-1) : m_captureTime);
    
    if (bAddObjectives)
    {
      Attach_Script_V(pObj, "dp88_AI_Objective", "%d,2,%d,%d,%d,%d,%d", team, distance, m_aiPriority_soldier, m_aiPriority_lvehicle, m_aiPriority_hvehicle, m_aiPriority_aircraft );          // Defensive
      Attach_Script_V(pObj, "dp88_AI_Objective", "%d,1,%d,%d,%d,%d,%d", (team==0)?1:0, distance, m_aiPriority_soldier, m_aiPriority_lvehicle, m_aiPriority_hvehicle, m_aiPriority_aircraft ); // Offensive
    }
  }
  
  else
  {
    Set_Object_Type(pObj, 2);
    m_captureState = 0.0f;
    
    if (bAddObjectives)
    {
      Attach_Script_V(pObj, "dp88_AI_Objective", "0,1,%d,%d,%d,%d,%d", distance, m_aiPriority_soldier, m_aiPriority_lvehicle, m_aiPriority_hvehicle, m_aiPriority_aircraft );          // Defensive
      Attach_Script_V(pObj, "dp88_AI_Objective", "1,1,%d,%d,%d,%d,%d", distance, m_aiPriority_soldier, m_aiPriority_lvehicle, m_aiPriority_hvehicle, m_aiPriority_aircraft ); // Offensive
    }
  }

  UpdateAnimationFrame(pObj);
}

// -------------------------------------------------------------------------------------------------

void dp88_conquestControlZone::DisplayMessage(GameObject* pObj, StringClass message, int sendtoteam)
{
  int team = Get_Object_Type(pObj);
  message.Replace("%OWNINGTEAM%", ((team==2)?"Neither":Get_Team_Name(team)), false);
  message.Replace("%OTHERTEAM%", ((team==2)?"Neither":Get_Team_Name((team==0)?1:0)), false);

  message.Replace("%CAPTUREPOINTS%", StringClass::getFormattedString("%d",Get_Int_Parameter("CapturePoints")), false);
  message.Replace("%TICKPOINTS%", StringClass::getFormattedString("%d",Get_Int_Parameter("TickPoints")), false);
  message.Replace("%TICKINTERVAL%", StringClass::getFormattedString("%d",Get_Int_Parameter("TickInterval")), false);

  message.Replace("%ZONENAME%", Get_Translated_Preset_Name(pObj), false);

  if (2 == sendtoteam)
    Send_Message(50,200,50,message);
  else
    Send_Message_Team(team, 50,200,50,message);
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_conquestControlZone> dp88_conquestControlZone_Registrant(
  "dp88_conquestControlZone",
  "ZoneSize:vector3,"
  "ZoneOffset:vector3,"
  "ZonePreset=Script_Zone_All:string,"
  "CapturePoints=5:int,"
  "CapturePointsFirstTeamOnly=1:int,"
  "TickPoints=1:int,"
  "TickInterval=3:int,"
  "CaptureTime=20:int,"
  "MultiCaptureCap=3:int,"
  "MultiCaptureMultiplier=0.5:float,"
  "AllowMajorityCapture=1:int,"
  "CaptureAnim:string,"
  "CaptureAnimTransitionFrames=0:int"
  "CapturingString:string,"
  "NeutralString:string,"
  "CapturedString:string,"
  "AIObjective_Priority_Soldier=0:int,"
  "AIObjective_Priority_Light_Vehicle=0:int,"
  "AIObjective_Priority_Heavy_Vehicle=0:int,"
  "AIObjective_Priority_Aircraft=0:int"
);










/*------------------------
Conquest Control Object
--------------------------*/

void dp88_conquestControlObject::Created ( GameObject* pObj )
{
  // Find the controller
  GameObject* pController = Find_Object_With_Script("dp88_conquestController");
  if ( !pController )
  {
    Console_Output ( "[%d:%s:%s] Critical Error: Conquest controller object not found. Destroying script...\n", Commands->Get_ID(pObj), Commands->Get_Preset_Name(pObj), this->Get_Name() );
    Destroy_Script();
    return;
  }
  m_controllerID = Commands->Get_ID(pController);

  // Always run the tick timer since we don't track the ownership state in a think timer
  Commands->Start_Timer( pObj, this, (float)Get_Int_Parameter("TickInterval"), TIMER_CONQUEST_TICK );
}

// -------------------------------------------------------------------------------------------------

void dp88_conquestControlObject::Timer_Expired ( GameObject* pObj, int number )
{
  if ( number == TIMER_CONQUEST_TICK )
  {
    int team = Get_Object_Type(pObj);
    if ( team == 0 || team == 1 )
    {
      GameObject* pController = Commands->Find_Object(m_controllerID);
      if ( !pController )
        return;

      // Grant points to owning team
      int custom = (team==0) ? CUSTOM_CONQUEST_GIVEPOINTS_T0 : CUSTOM_CONQUEST_GIVEPOINTS_T1;
      Commands->Send_Custom_Event ( pObj, pController, custom, Get_Int_Parameter("TickPoints"), 0.0f);
    }

    // Always run the tick timer since we don't track the ownership state in a think timer
    Commands->Start_Timer( pObj, this, (float)Get_Int_Parameter("TickInterval"), TIMER_CONQUEST_TICK );
  }
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_conquestControlObject> dp88_conquestControlObject_Registrant(
  "dp88_conquestControlObject",
  "TickPoints=1:int,"
  "TickInterval=3:int"
);










/*------------------------
Radar Invisibility
--------------------------*/

void dp88_radarInvisibility::Created ( GameObject* pObj )
{
  PhysicalGameObj* phys = pObj->As_PhysicalGameObj();
  if ( !phys )
  {
    Console_Output ( "[%d:%s:%s] Critical Error: This script is only compatible with physical game objects. Destroying script...\n", Commands->Get_ID(pObj), Commands->Get_Preset_Name(pObj), this->Get_Name() );
    Destroy_Script();
    return;
  }

  // Disable the radar blip for this object
  m_originalRadarMode = phys->Get_Radar_Blip_Shape_Type();
  phys->Set_Radar_Blip_Shape_Type(RADAR_BLIP_SHAPE_NONE);

  // If this is a vehicle then setup the passenger data structures
  if ( VehicleGameObj* pVehicle = pObj->As_VehicleGameObj() )
  {
    m_nSeats = pVehicle->Get_Definition().Get_Seat_Count();
    m_pPassengerIds = new int[m_nSeats];
    m_pPassengerRadarModes = new int[m_nSeats];
    for ( int i = 0; i < m_nSeats; ++i )
      m_pPassengerIds[i] = NULL;
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_radarInvisibility::Custom ( GameObject* pObj, int type, int param, GameObject* pSender )
{
  if ( type == CUSTOM_EVENT_VEHICLE_ENTERED )
  {
    HidePassengerBlip(pObj->As_VehicleGameObj(), pSender->As_SoldierGameObj());
  }

  else if ( type == CUSTOM_EVENT_VEHICLE_EXITED )
  {
    RestorePassengerBlip(pSender->As_SoldierGameObj());
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_radarInvisibility::HidePassengerBlip ( VehicleGameObj* pVehicle, SoldierGameObj* pSoldier )
{
  if ( !pVehicle || !pSoldier || !m_pPassengerIds || !pSoldier->As_PhysicalGameObj() )
    return;

  for ( int i = 0; i < m_nSeats; ++i )
  {
    if ( pVehicle->Get_Occupant(i) == pSoldier )
    {
      m_pPassengerIds[i] = Commands->Get_ID(pSoldier);
      m_pPassengerRadarModes[i] = pSoldier->As_PhysicalGameObj()->Get_Radar_Blip_Shape_Type();
      pSoldier->As_PhysicalGameObj()->Set_Radar_Blip_Shape_Type(RADAR_BLIP_SHAPE_NONE);
    }
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_radarInvisibility::RestorePassengerBlip ( SoldierGameObj* pSoldier )
{
  if ( !pSoldier || !m_pPassengerIds || !pSoldier->As_PhysicalGameObj() )
    return;

  int soldierId = Commands->Get_ID(pSoldier);

  for ( int i = 0; i < m_nSeats; ++i )
  {
    if ( m_pPassengerIds[i] == soldierId )
    {
      pSoldier->As_PhysicalGameObj()->Set_Radar_Blip_Shape_Type(m_pPassengerRadarModes[i]);
      m_pPassengerIds[i] = NULL;
    }
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_radarInvisibility::Detach ( GameObject* pObj )
{
  if ( Exe != EXE_LEVELEDIT )   // Don't trigger this code in LevelEdit
  {
    if ( PhysicalGameObj* phys = pObj->As_PhysicalGameObj() )
      phys->Set_Radar_Blip_Shape_Type(m_originalRadarMode);

    for ( int i = 0; i < m_nSeats; ++i )
    {
      if ( m_pPassengerIds[i] != NULL )
      {
        if ( GameObject* pSoldier = Commands->Find_Object(m_pPassengerIds[i]) )
          RestorePassengerBlip( pSoldier->As_SoldierGameObj() );
      }
    }
  }

  delete [] m_pPassengerIds;
  delete [] m_pPassengerRadarModes;
  
  // Prevent a crash if Detach is somehow called twice (can apparently happen?!?)
  m_pPassengerIds = NULL;
  m_pPassengerRadarModes = NULL;
  m_nSeats = 0;

  ScriptImpClass::Detach(pObj);
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_radarInvisibility> dp88_radarInvisibility_Registrant(
  "dp88_radarInvisibility",
  ""
);








void dp88_turretSound::Created ( GameObject* pObj )
{
  if ( VehicleGameObj* vObj = pObj->As_VehicleGameObj() )
  {
    m_nMinDifferential = Get_Float_Parameter("Min_Differential_Rad");
    m_lastFacing = Get_Turret_Facing(vObj->Peek_Model());
    m_nSoundId = -1;
    Commands->Start_Timer(pObj, this, 0.5f, TIMER_TURRETSOUND );
  }
  else
  {
    Console_Output ( "[%d:%s:%s] Critical Error: This script is only compatible with vehicle game objects. Destroying script...\n", Commands->Get_ID(pObj), Commands->Get_Preset_Name(pObj), this->Get_Name() );
    Destroy_Script();
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_turretSound::Timer_Expired ( GameObject* pObj, int number )
{
  if ( number == TIMER_TURRETSOUND )
  {
    if ( VehicleGameObj* vObj = pObj->As_VehicleGameObj() )
    {
      float newFacing = Get_Turret_Facing(vObj->Peek_Model());

      // Check if we are rotating - ignore tiny rotation amounts
      bool bRotating = ( abs(newFacing-m_lastFacing) > m_nMinDifferential );

      if ( m_nSoundId == -1 && bRotating)
        Play_Sound(pObj);

      else if ( m_nSoundId != -1 && !bRotating )
        Stop_Sound(pObj);

      m_lastFacing = newFacing;
    }

    // Restart timer to check for rotation
    Commands->Start_Timer(pObj, this, 0.5f, TIMER_TURRETSOUND );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_turretSound::Custom ( GameObject* pObj, int type, int param, GameObject* pSender )
{
  if (type == CUSTOM_EVENT_SOUND_ENDED && param == m_nSoundId)
  {
    // We will allow the timer to stop the sound if necessary, since this might trigger
    // on the same engine tick, thus checking our facing against the previous timer
    // facing could produce a false-positive for "stopped rotating"
    Play_Sound(pObj);
  }
}

// -------------------------------------------------------------------------------------------------

float dp88_turretSound::Get_Turret_Facing ( RenderObjClass* pRenderObj )
{
  if ( pRenderObj )
  {
    Matrix3D vehicleTransform = pRenderObj->Get_Transform();
    Matrix3D transform = pRenderObj->Get_Bone_Transform("turret");
    return abs(vehicleTransform.Get_Z_Rotation()-transform.Get_Z_Rotation());
  }

  return 0.0f;
}

// -------------------------------------------------------------------------------------------------

void dp88_turretSound::Play_Sound ( GameObject* pObj )
{
  m_nSoundId = Commands->Create_3D_Sound_At_Bone(Get_Parameter("Sound_Preset"), pObj, "turret");
  Commands->Monitor_Sound(pObj, m_nSoundId);
}

// -------------------------------------------------------------------------------------------------

void dp88_turretSound::Stop_Sound ( GameObject* pObj )
{
  Commands->Stop_Sound(m_nSoundId,true);
  m_nSoundId = -1;
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_turretSound> dp88_turretSound_Registrant(
  "dp88_turretSound",
  "Sound_Preset:string,"
  "Min_Differential_Rad=0.25:float"
);










// -------------------------------------------------------------------------------------------------
// Teleportation script
// -------------------------------------------------------------------------------------------------

void dp88_teleport::Created ( GameObject* pObj )
{
  m_nObjectID = Get_Int_Parameter("ObjectID");    // Don't check object ID right now, it might be created later
  m_bUseBone = strlen(Get_Parameter("Bone")) > 0;
  m_offset = Get_Vector3_Parameter("Offset");

  m_bTeleportInfantry = (Get_Int_Parameter("Teleport_Infantry") == 1);
  m_bTeleportVehicles = (Get_Int_Parameter("Teleport_Vehicles") == 1);
}

// -------------------------------------------------------------------------------------------------

void dp88_teleport::Entered ( GameObject* pObj, GameObject* pEnterer )
{
  if ( (m_bTeleportInfantry && pEnterer->As_SoldierGameObj()) || (m_bTeleportVehicles && pEnterer->As_VehicleGameObj()) )
  {
    Vector3 location = m_offset;

    if ( m_nObjectID != 0 )
    {
      GameObject* pTarget = Commands->Find_Object(m_nObjectID);
      if ( !pTarget )
        return;

      location = ( m_bUseBone ) ? Commands->Get_Bone_Position(pTarget,Get_Parameter("bone")) : Commands->Get_Position(pTarget);
      location.X += m_offset.X;
      location.Y += m_offset.Y;
      location.Z += m_offset.Z;
    }

    Commands->Set_Position(pEnterer, location);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_teleport::Custom ( GameObject* pObj, int type, int param, GameObject* pSender )
{
  if ( type == CUSTOM_EVENT_POWERUP_GRANTED )
    Entered ( pObj, pSender );
}

// -------------------------------------------------------------------------------------------------

void dp88_teleport::Poked ( GameObject* pObj, GameObject* pPoker )
{
  Entered ( pObj, pPoker );
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_teleport> dp88_teleport_Registrant(
  "dp88_teleport",
  "ObjectID=0:int,"
  "Bone:string,"
  "Offset:vector3,"
  "Teleport_Infantry=1:int,"
  "Teleport_Vehicles=1:int"
);










// -------------------------------------------------------------------------------------------------
// Ammo Animation script
// -------------------------------------------------------------------------------------------------

void dp88_Ammo_Animation::Created ( GameObject* pObj )
{
  m_strAnimation = Get_Parameter("Animation");
  m_nFrames = Get_Int_Parameter("Animation_Frames");
  m_bUseTotal = (Get_Int_Parameter("Use_Total_Ammo")!=0);

  if ( m_strAnimation == NULL || strlen(m_strAnimation) == 0 )
  {
    Console_Output ( "[%d:%s:%s] Critical Error: No animation has been specified! Destroying script...\n", Commands->Get_ID(pObj), Commands->Get_Preset_Name(pObj), this->Get_Name() );
    Destroy_Script();
    return;
  }

  if ( Get_Current_Max_Bullets(pObj) <= 0 || (m_bUseTotal && Get_Current_Clip_Max_Bullets(pObj) <= 0) )
  {
    Console_Output ( "[%d:%s:%s] Critical Error: This script cannot be used on a weapon with infinite animation! Destroying script...\n", Commands->Get_ID(pObj), Commands->Get_Preset_Name(pObj), this->Get_Name() );
    Destroy_Script();
    return;
  }

  // If an animation model is specified then create it now...
  if ( strlen ( Get_Parameter("Animation_Model") ) > 0 )
  {
    if ( GameObject* pAmmoAnimObj = Commands->Create_Object ( "Invisible_Object", Commands->Get_Position(pObj) ) )
    {
      m_ammoAnimObjId = Commands->Get_ID(pAmmoAnimObj);
      Commands->Set_Model ( pAmmoAnimObj, Get_Parameter("Animation_Model") );
      Commands->Attach_To_Object_Bone ( pAmmoAnimObj, pObj, Get_Parameter("Animation_Model_Bone") );
    }
  }

  m_nBullets = -1;    // Animation frame update on first timer tick (unless infinite ammo)
  Commands->Start_Timer(pObj, this, 0.1f, TIMER_AMMO_ANIMATION);
}

// -------------------------------------------------------------------------------------------------

void dp88_Ammo_Animation::Timer_Expired ( GameObject* pObj, int number )
{
  if ( number == TIMER_AMMO_ANIMATION )
  {
    int nBullets = (m_bUseTotal) ? Get_Current_Total_Bullets(pObj) : Get_Current_Bullets(pObj);
    if ( nBullets != m_nBullets )
    {
      m_nBullets = nBullets;
      UpdateAnimationFrame(pObj);
    }

    Commands->Start_Timer(pObj, this, 0.1f, TIMER_AMMO_ANIMATION);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_Ammo_Animation::UpdateAnimationFrame ( GameObject* pObj )
{
  GameObject* pAnimObj = ( m_ammoAnimObjId != NULL ) ? Commands->Find_Object(m_ammoAnimObjId) : pObj;
  int maxBullets = (m_bUseTotal) ? Get_Current_Total_Max_Bullets(pObj) : Get_Current_Max_Bullets(pObj);
  int frame = (int)floor(((m_nFrames-1)*(1.0f-(float)m_nBullets/maxBullets)));
  Commands->Set_Animation_Frame(pAnimObj, m_strAnimation, frame);
  Update_Network_Object(pObj);
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_Ammo_Animation> dp88_Ammo_Animation_Registrant(
  "dp88_Ammo_Animation",
  "Animation:string,"
  "Animation_Frames:int,"
  "Use_Total_Ammo:int,"
  "Animation_Model:string,"
  "Animation_Model_Bone:string"
);










// -------------------------------------------------------------------------------------------------
// Camo Controller
// -------------------------------------------------------------------------------------------------

void dp88_Camo_Controller::Created ( GameObject* pObj )
{
  m_camoVariant = Get_Parameter("Camouflage");
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_Camo_Controller> dp88_Camo_Controller_Registrant(
  "dp88_Camo_Controller",
  "Camouflage:string"
);










// -------------------------------------------------------------------------------------------------
// Camo Object
// -------------------------------------------------------------------------------------------------

void dp88_Camo_Object::Created ( GameObject* pObj )
{
  if ( GameObject* pController = Find_Object_With_Script("dp88_Camo_Controller") )
  {
    if ( dp88_Camo_Controller* pControllerScript = (dp88_Camo_Controller*)Find_Script_On_Object(pController, "dp88_Camo_Controller") )
    {
      if ( strcmp(Get_Parameter("Camouflage"),pControllerScript->GetCamoVariant()) == 0 )
        Commands->Set_Model(pObj, Get_Parameter("Model"));
    }
  }

  Destroy_Script();
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_Camo_Object> dp88_Camo_Object_Registrant(
  "dp88_Camo_Object",
  "Camouflage:string,"
  "Model:string"
);










// -------------------------------------------------------------------------------------------------
// Create Object with Cinematic
// -------------------------------------------------------------------------------------------------

// Copied from sh_factory.cpp - these should really move into a header...
enum
{
    CINEMATIC_SET_SLOT  = 10000,
    CINEMATIC_START     = 99000
};

// -------------------------------------------------------------------------------------------------

void dp88_Create_Object_Cinematic::Created ( GameObject* pObj )
{
  if ( 0 == Get_Int_Parameter("Trigger_Custom") )
  {
    Create_Object(pObj, pObj);
    Destroy_Script();
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_Create_Object_Cinematic::Custom ( GameObject* pObj, int type, int param, GameObject* pSender )
{
  if ( type == Get_Int_Parameter("Trigger_Custom") )
  {
    GameObject* pOwner = (param >0 ) ? Commands->Find_Object(param) : NULL;
    if ( NULL == pOwner )
      pOwner = pSender;

    Create_Object(pObj, pOwner);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_Create_Object_Cinematic::Create_Object ( GameObject* pobj, GameObject* pOwner )
{
  // Mostly cloned from SH_CinematicVehicleFactoryBypass::PurchaseMonitor()
  Vector3 position = Commands->Get_Position(pOwner);
  position += Get_Vector3_Parameter("Offset");
  float facing = Commands->Get_Facing(pOwner);

  // create cinematic
  char cinematic_params[128];
  sprintf_s(cinematic_params, "%s,1", Get_Parameter("Cinematic"));

  auto cinematic = Commands->Create_Object("Invisible_Object", position);
  Commands->Set_Facing(cinematic, facing);
  Commands->Attach_Script(cinematic, "JFW_Cinematic", cinematic_params);

  // create vehicle at delivery site
  GameObject* vehicle = Commands->Create_Object(Get_Parameter("Preset"), position);
  Commands->Set_Facing(vehicle, facing);
  Commands->Attach_Script(vehicle, "SH_CinematicVehicleDeliveryCallback", "");

  // load vehicle and purchaser into cinematic, start it
  Commands->Send_Custom_Event(pOwner, cinematic, CINEMATIC_SET_SLOT + 0, Commands->Get_ID(vehicle), 0);
  Commands->Send_Custom_Event(pOwner, cinematic, CINEMATIC_SET_SLOT + 1, Commands->Get_ID(pOwner), 0);
  Commands->Send_Custom_Event(pOwner, cinematic, CINEMATIC_START, 0, 0);
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_Create_Object_Cinematic> dp88_Create_Object_Cinematic_Registrant(
  "dp88_Create_Object_Cinematic",
  "Preset:string,"
  "Cinematic:string,"
  "Trigger_Custom:int,"
  "Offset:vector3"
);










// -------------------------------------------------------------------------------------------------
// Set Team On Custom
// -------------------------------------------------------------------------------------------------

void dp88_Set_Team_On_Custom::Custom ( GameObject* pObj, int type, int param, GameObject* pSender )
{
  if ( type == Get_Int_Parameter("Message") )
  {
    if ( param >= 0 && param <= 2 && param != Get_Object_Type(pObj) )
    {
      if ( Find_Script_On_Object(pObj, "dp88_buildingScripts_controller") != NULL )
      {
        Commands->Send_Custom_Event(pObj, pObj, CUSTOM_BUILDINGSCRIPTS_BUILDINGCAPTURED, param, 0.0f);
      }
      else
      {
        Set_Object_Type(pObj, param);
      }
    }
  }
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_Set_Team_On_Custom> dp88_Set_Team_On_Custom_Registrant(
  "dp88_Set_Team_On_Custom",
  "Message:int"
);










// -------------------------------------------------------------------------------------------------
// Health regeneration script
// -------------------------------------------------------------------------------------------------

void dp88_RegenerateHitpoints::Created(GameObject *obj)
{
  m_mode = (Get_Parameter_Count() >= 8) ? Get_Int_Parameter("Mode") : MODE_HEALTH + MODE_ARMOUR;
  m_amount = Get_Float_Parameter("Hitpoints");
  m_maxPercent = Get_Int_Parameter("Max_Percent")/100.0f;
  m_interval = (float)Get_Int_Parameter("Interval");

  m_lastDamage = time(NULL);
  m_lastDamageTimeout = Get_Int_Parameter("Damage_Timeout");

  m_warheadId = -1;

  const char* warhead = Get_Parameter("Warhead");
  if (Is_Valid_Warhead_Type(warhead))
    m_warheadId = ArmorWarheadManager::Get_Warhead_Type(warhead);

  Commands->Start_Timer(obj, this, m_interval, TIMER_MISC_TICK);
}

// -------------------------------------------------------------------------------------------------

bool dp88_RegenerateHitpoints::IsModeEnabled(int mode)
{
  return (m_mode & mode) == mode;
}

// -------------------------------------------------------------------------------------------------

void dp88_RegenerateHitpoints::Timer_Expired(GameObject *obj, int number)
{
  // Only process this if we are not dead and are not inside the combat timeout
  if (Commands->Get_Health(obj) > 0.0f && m_amount > 0 && m_lastDamageTimeout <= time(NULL)-m_lastDamage )
  {
    // Repair self
    RegenObject(obj, IsModeEnabled(MODE_HEALTH), IsModeEnabled(MODE_ARMOUR));

    // Handle soldier-specific modes (piloted / occupied vehicle)
    if (obj->As_SoldierGameObj())
    {
      GameObject* vehicle = Get_Vehicle(obj);

      if (NULL != vehicle)
      {
        GameObject* driver = Get_Vehicle_Driver(vehicle);
        bool bIsPilot = NULL != driver && driver->Get_ID() == obj->Get_ID();

        // Repair piloted vehicle
        if (bIsPilot)
        {
          RegenObject(vehicle, IsModeEnabled(MODE_PILOTED_HEALTH), IsModeEnabled(MODE_PILOTED_ARMOUR));
        }

        // Repair occupied vehicle
        else
        {
          RegenObject(vehicle, IsModeEnabled(MODE_OCCUPIED_HEALTH), IsModeEnabled(MODE_OCCUPIED_ARMOUR));
        }
      }
    }

    // Handle vehicle-specific modes (driver / passengers)
    if (obj->As_VehicleGameObj())
    {
      // Repair driver
      RegenObject(Get_Vehicle_Driver(obj), IsModeEnabled(MODE_DRIVER_HEALTH), IsModeEnabled(MODE_DRIVER_ARMOUR));
      
      // Repair passengers
      bool bPassengersHealth = IsModeEnabled(MODE_PASSENGERS_HEALTH);
      bool bPassengersArmour = IsModeEnabled(MODE_PASSENGERS_ARMOUR);

      if (bPassengersHealth || bPassengersArmour)
      {
        int seats = Get_Vehicle_Seat_Count(obj);
        for (int i = 1; i < seats; ++i)
        {
          RegenObject(Get_Vehicle_Occupant(obj, i), bPassengersHealth, bPassengersArmour);
        }
      }
    }
  }

  Commands->Start_Timer(obj, this, m_interval, TIMER_MISC_TICK);
}

// -------------------------------------------------------------------------------------------------

void dp88_RegenerateHitpoints::RegenObject(GameObject* obj, bool bHealth, bool bArmour)
{
  if (NULL == obj || !bHealth && !bArmour)
    return;

  float hitpoints = (bHealth ? Commands->Get_Health(obj) : 0.0f) + (bArmour ? Commands->Get_Shield_Strength(obj) : 0.0f);
  float maxHitpoints = (bHealth ? Commands->Get_Max_Health(obj) : 0.0f) + (bArmour ? Commands->Get_Max_Shield_Strength(obj) : 0.0f);

  if (hitpoints < maxHitpoints)
  {
    float regenCap = (maxHitpoints*m_maxPercent);
    float amount = min(m_amount, regenCap);

    if (hitpoints < regenCap)
    {
      if (m_warheadId != -1)
      {
        Commands->Apply_Damage(obj, amount, ArmorWarheadManager::Get_Warhead_Name(m_warheadId), NULL);
      }
      else
      {
        ApplyNonWarheadRepairs(obj, amount, bHealth, bArmour);
      }

      // Trigger update for built-in engine damage bones
      if (obj->As_VehicleGameObj())
      {
        obj->As_VehicleGameObj()->Damage_Meshes_Update();
      }
    }
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_RegenerateHitpoints::ApplyNonWarheadRepairs(GameObject* obj, float amount, bool bHealth, bool bArmour)
{
  if (bHealth)
  {
    float health = Commands->Get_Health(obj);
    float hrepair = min(amount, Commands->Get_Max_Health(obj) - health);
    Commands->Set_Health(obj, health + hrepair);
    amount -= hrepair;
  }

  if (bArmour)
  {
    float armour = Commands->Get_Shield_Strength(obj);
    float arepair = min(amount, Commands->Get_Max_Shield_Strength(obj) - armour);
    Commands->Set_Shield_Strength(obj, armour + arepair);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_RegenerateHitpoints::Damaged(GameObject *obj, GameObject *damager, float amount)
{
  if (amount > 0.0f)
    m_lastDamage = time(NULL);
}

// -------------------------------------------------------------------------------------------------

void dp88_RegenerateHitpoints::Custom ( GameObject* pObj, int type, int param, GameObject* pSender )
{
  if ( type == CUSTOM_VETERANCY_PROMOTED )
  {
    if (2==param)
      m_amount = Get_Float_Parameter("Hitpoints_Elite");
    else if (1==param)
      m_amount = Get_Float_Parameter("Hitpoints_Veteran");
    else
      m_amount = Get_Float_Parameter("Hitpoints");
  }
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_RegenerateHitpoints> dp88_RegenerateHitpoints_Registrant(
  "dp88_RegenerateHitpoints",
  "Hitpoints:float,"
  "Interval=1:int,"
  "Warhead:string,"
  "Max_Percent=100:int,"
  "Damage_Timeout=0:int,"
  "Hitpoints_Veteran:float,"
  "Hitpoints_Elite:float,"
  "Mode=3:int"
);











/*------------------------
Spawn Vehicle Part script
--------------------------*/

void dp88_CreateAttachedObject::Created(GameObject* obj)
{
  m_spawnedObjectId = NULL;
  m_hasDriver = NULL != Get_Vehicle_Driver(obj);  // In theory, might not be attached at vehicle creation :) 

  // Spawn the new object
  GameObject* spawnedObject = Commands->Create_Object_At_Bone(obj, Get_Parameter("Preset"), Get_Parameter("Bone"));
  if (NULL == spawnedObject)
  {
    Console_Output("[%d:%s:%s] Critical Error: Failed to create an instance of the preset %s. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Parameter("Preset"));
    Destroy_Script();
    return;
  }

  // Attach it to the parent
  Commands->Attach_To_Object_Bone(spawnedObject, obj, Get_Parameter("Bone"));
  m_spawnedObjectId = Commands->Get_ID(spawnedObject);

  // Configure linking features (health & veterancy)
  if (1 == Get_Int_Parameter("EnableHealthLink"))
  {
    Attach_Script_Once_V(spawnedObject, "dp88_linkHealth", "%d", Commands->Get_ID(obj));
  }
  if (1 == Get_Int_Parameter("EnableVeterancyLink"))
  {
    Attach_Script_Once_V(spawnedObject, "dp88_linkVetPoints", "%d", Commands->Get_ID(obj));
  }

  // Configure AI visibility
  if (0 == Get_Int_Parameter("VisibleToAI"))
  {
    Commands->Set_Is_Visible(spawnedObject, false);
  }
    
  // If driver customrs are enabled, send the appropriate custom based on whether we have a driver
  int driverCustom = (m_hasDriver) ? Get_Int_Parameter("DriverEnterCustom") : Get_Int_Parameter("DriverExitCustom");
  if (0 != driverCustom)
  {
    Commands->Send_Custom_Event(obj, spawnedObject, driverCustom, 0, 0);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_CreateAttachedObject::Killed(GameObject* obj, GameObject *killer)
{
  if (NULL != m_spawnedObjectId)
  {
    GameObject* spawnedObject = Commands->Find_Object(m_spawnedObjectId);
    if (NULL != spawnedObject)
    {
      Commands->Destroy_Object(spawnedObject);
    }
  }

  m_spawnedObjectId = NULL;
}

// -------------------------------------------------------------------------------------------------

void dp88_CreateAttachedObject::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
  if (type == CUSTOM_EVENT_VEHICLE_ENTERED || type == CUSTOM_EVENT_VEHICLE_EXITED)
  {
    GameObject* spawnedObject = Commands->Find_Object(m_spawnedObjectId);
    if (NULL == spawnedObject)
    {
      return;
    }

    // Always sync the spawned objects team with our own
    Commands->Set_Player_Type(spawnedObject, Commands->Get_Player_Type(obj));

    // Decide which custom, if any, to send
    bool hasDriver = NULL != Get_Vehicle_Driver(obj);
    if (hasDriver != m_hasDriver)
    {
      m_hasDriver = hasDriver;

      int driverCustom = (m_hasDriver) ? Get_Int_Parameter("DriverEnterCustom") : Get_Int_Parameter("DriverExitCustom");
      if (0 != driverCustom)
      {
        Commands->Send_Custom_Event(obj, spawnedObject, driverCustom, 0, 0);
      }
    }
  }
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_CreateAttachedObject> dp88_CreateAttachedObject_Registrant(
  "dp88_CreateAttachedObject",
  "Preset:string,"
  "Bone:string,"
  "EnableHealthLink=1:int,"
  "EnableVeterancyLink=0:int,"
  "VisibleToAI=0:int,"
  "DriverEnterCustom=0:int,"
  "DriverExitCustom=0:int"
);

/*------------------------
Custom Points Percentage script
--------------------------*/

void dp88_customPointsPercentage::Created ( GameObject* obj )
{
  // Preload values since converting strings to floats every time we take
  // damage is very inefficient
  m_killPoints		= Get_Float_Parameter("killPoints") / 100;
  m_damagePoints	= Get_Float_Parameter("damagePoints") / 100;
  m_repairPoints	= Get_Float_Parameter("repairPoints") / 100;
  m_killMoney		= Get_Float_Parameter("killMoney") / 100;
  m_damageMoney		= Get_Float_Parameter("damageMoney") / 100;
  m_repairMoney		= Get_Float_Parameter("repairMoney") / 100;
}

// -------------------------------------------------------------------------------------------------

void dp88_customPointsPercentage::Damaged ( GameObject *obj, GameObject *damager, float amount )
{
  // Abort if there is no damager, or the unit damaged itself, or the damager is not
  // a star
  if ( !damager || damager == obj || !Commands->Is_A_Star(damager) )
    return;

  /* Damaged or repaired? Then convert amount into it's absolute value for
  calculating points granted */
  bool bRepair = (amount < 0);
  float points = ((bRepair) ? m_repairPoints : m_damagePoints) * amount;
  float money = ((bRepair) ? m_repairMoney : m_damageMoney) * amount;

  // Give the damager their points and money
  GivePointsAndMoney ( damager, points, money );
}

// -------------------------------------------------------------------------------------------------

void dp88_customPointsPercentage::Killed ( GameObject* obj, GameObject *killer )
{
  // Abort if there is no killer, or the unit killed itself, or the killer is not
  // a star
  if ( !killer || killer == obj || !Commands->Is_A_Star(killer) )
    return;

   // Give the killer their points and money
   GivePointsAndMoney ( killer, m_killPoints, m_killMoney );
}

// -------------------------------------------------------------------------------------------------

void dp88_customPointsPercentage::GivePointsAndMoney ( GameObject* obj, float points, float money )
{
  float pointsMultiplier = 1.f;
  SmartGameObj *smart = obj->As_SmartGameObj();
  if (smart->Is_Teammate(Owner()->As_DamageableGameObj()))
  {
	  pointsMultiplier = -pointsMultiplier;
  }
  else if (!smart->Is_Enemy(Owner()->As_DamageableGameObj()))
  {
	  pointsMultiplier = 0.f;
  }
  points *= pointsMultiplier;
  money *= pointsMultiplier;
  int playerId = Get_Player_ID(obj);
  if ( playerId >= 0 )
  {
    if ( cPlayer* player = Find_Player(playerId) )
    {
      player->Set_Score(player->Get_Score()+points);
      if (money > 0)
      {
        player->Increment_Money(money);
      }
    }
  }

  int team = Get_Object_Type(obj);
  Set_Team_Score(team, Get_Team_Score(team) + points);
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_customPointsPercentage> dp88_customPointsPercentage_Registrant(
  "dp88_customPointsPercentage",
  "killPoints:float,"
  "damagePoints:float,"
  "repairPoints:float,"
  "killMoney:float,"
  "damageMoney:float,"
  "repairMoney:float"
);
