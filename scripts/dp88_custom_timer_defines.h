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

#define DP88_RGB_GENERAL_MSG                      50,200,50     // Green
#define DP88_RGB_WARNING_MSG                      153,204,25    // Yellow
#define DP88_RGB_ERROR_MSG                        255,50,50     // Red

// -------------------------------------------------------------------------------------------------

#define DP88_CUSTOM                               0xDA000000
#define DP88_TIMER                                0xDB000000

// -------------------------------------------------------------------------------------------------
// Custom and timer groups
// -------------------------------------------------------------------------------------------------

#define MISC                                      0x00000000
#define VETERANCY                                 0x00010000
#define BUILDINGSCRIPTS                           0x00020000
#define CONQUEST                                  0x00030000
#define REMOTECONTROL                             0x00040000
#define OREMINING                                 0x00050000
#define AI                                        0x00060000

//#define AR_MINER                                  0x00A00000  // Avaiable for re-use
#define AR_IFV                                    0x00A10000

// -------------------------------------------------------------------------------------------------
// Misc group
// -------------------------------------------------------------------------------------------------

#define CUSTOM_TRANSITION_VTOL_LAND_ZONE          (DP88_CUSTOM|MISC|0x01)             //!< Sent by dp88_Aircraft_LandingZone to any GameObjects entering (param=1) or exiting (param=0) the zone
#define CUSTOM_UNITSOUNDS_HEALDECREMENT           (DP88_CUSTOM|MISC|0x02)             //!< Used by dp88_unitSounds to track the amount of healing a unit has recieved over a time period
#define CUSTOM_ROCKETEER_VEHICLEKILLED            (DP88_CUSTOM|MISC|0x03)             //!< Used by dp88_AR_Rocketeer to notify itself when the flight vehicle dies (via JFW_Death_Send_Custom)
#define CUSTOM_VEHICLE_DEPLOY                     (DP88_CUSTOM|MISC|0x04)             //!< Used by deployable vehicle scripts to notify other scripts of a change in the deployment state, 0 = undeployed, 1 = deploying, 2 = deployed, 3 = undeploying
#define CUSTOM_RADAR_JAM                          (DP88_CUSTOM|MISC|0x05)             //!< Used by new Radar Jammer scripts to switch radar off and on and also play jamming sounds, 0 = radar off, 1 = radar on
#define CUSTOM_DEPLOY_STATE_CHANGED               (DP88_CUSTOM|MISC|0x07)             //!< Used by deployable scripts to notify other scripts of a change in the deployment state, 0 = undeployed, 1 = deployed

#define TIMER_MISC_TICK                           (DP88_TIMER|MISC|0x01)              //!< Miscellaneous tick timer for generic use by scripts that implement on-tick events
#define TIMER_RANDOMWEATHER                       (DP88_TIMER|MISC|0x02)              //!< Used by dp88_randomWeather to do a weather update
#define TIMER_SPAWNVEHICLEPART                    (DP88_TIMER|MISC|0x03)              //!< Used by dp88_spawnVehiclePart
#define TIMER_MIRAGE_CHECKMOVEMENT                (DP88_TIMER|MISC|0x04)              //!< Used by dp88_AR_MirageTank to test for movement to toggle mirage mode
#define TIMER_TURRETSOUND                         (DP88_TIMER|MISC|0x05)              //!< Used by dp88_turretSound to test for turret rotation
#define TIMER_ROCKETEER_ENTERVEHICLE              (DP88_TIMER|MISC|0x06)              //!< Used by dp88_AR_Rocketeer to enter the flight vehicle
#define TIMER_PARADROP_CHECKFALLRATE              (DP88_TIMER|MISC|0x07)              //!< Used by dp88_AR_Paradrop to check if the falling speed drops (ie: landed)
#define TIMER_AMMO_ANIMATION                      (DP88_TIMER|MISC|0x08)              //!< Used by dp88_Ammo_Animation to check for changes in ammo count
#define TIMER_CHECK_BASE_POWER_STATE              (DP88_TIMER|MISC|0x09)              //!< Used by any scripts which need to check if a base is still powered

// -------------------------------------------------------------------------------------------------
// Veterancy group
// -------------------------------------------------------------------------------------------------

#define CUSTOM_VETERANCY_PROMOTED                 (DP88_CUSTOM|VETERANCY|0x01)        //!< Sent to self to notify other scripts about a promotion
#define CUSTOM_VETERANCY_REMOVE_OLD_WEAPON        (DP88_CUSTOM|VETERANCY|0x02)        //!< Remove infantry units previous weapon after a promotion upgraded it
#define CUSTOM_VETERANCY_GRANT_POINTS_DELAY       (DP88_CUSTOM|VETERANCY|0x03)        //!< Sent by dp88_veterancyGrantPoints to itself to delay the points grant until after game init

#define TIMER_VETERANCY_EXPIRE_PROMOTION_CHEVRON  (DP88_TIMER|VETERANCY|0x01)         //!< Timer to clear promotion chevron
#define TIMER_VETERANCY_CHEVRON_VISIBILITY_THINK  (DP88_TIMER|VETERANCY|0x02)         //!< Timer to update the visibility of promotion chevrons when stealthed or in a vehicle

// -------------------------------------------------------------------------------------------------
// Building scripts group
// -------------------------------------------------------------------------------------------------

#define CUSTOM_BUILDINGSCRIPTS_REGISTERCHILD      (DP88_CUSTOM|BUILDINGSCRIPTS|0x01)
#define CUSTOM_BUILDINGSCRIPTS_UNREGISTERCHILD    (DP88_CUSTOM|BUILDINGSCRIPTS|0x02)
#define CUSTOM_BUILDINGSCRIPTS_CHILDDESTROYED     (DP88_CUSTOM|BUILDINGSCRIPTS|0x03)  //!< Sent to the controller when a child is destroyed (minor / major WP)
#define CUSTOM_BUILDINGSCRIPTS_CHILDREPAIRED      (DP88_CUSTOM|BUILDINGSCRIPTS|0x04)  //!< Sent to the controller when a child is repaired (minor WP)
#define CUSTOM_BUILDINGSCRIPTS_BUILDINGDESTROYED  (DP88_CUSTOM|BUILDINGSCRIPTS|0x05)  //!< Sent by the controller to children when the building is destroyed
#define CUSTOM_BUILDINGSCRIPTS_BUILDINGOFFLINE    (DP88_CUSTOM|BUILDINGSCRIPTS|0x06)  //!< Sent by the controller to children when the building is offline
#define CUSTOM_BUILDINGSCRIPTS_BUILDINGONLINE     (DP88_CUSTOM|BUILDINGSCRIPTS|0x07)  //!< Sent by the controller to children when the building is online
#define CUSTOM_BUILDINGSCRIPTS_BUILDINGCAPTURED   (DP88_CUSTOM|BUILDINGSCRIPTS|0x08)  //!< Sent by the controller to children when the building has been captured or made neutral, or from a capture point to a building when it has been captured or made neutral

#define TIMER_BUILDINGSCRIPTS_CAPTUREPOINT        (DP88_TIMER|BUILDINGSCRIPTS|0x01)   //!< Runs on a building capture point to constantly move the health towards equilibrium
#define TIMER_BUILDINGSCRIPTS_TRICKLEMONEY        (DP88_TIMER|BUILDINGSCRIPTS|0x02)   //!< Runs on a trickle money building effect to grant money once per second

// -------------------------------------------------------------------------------------------------
// Conquest scripts group
// -------------------------------------------------------------------------------------------------

#define CUSTOM_CONQUEST_GIVEPOINTS_T0             (DP88_CUSTOM|CONQUEST|0x01)         //!< Sent to the dp88_conquestController to grant <param> points to team 0
#define CUSTOM_CONQUEST_GIVEPOINTS_T1             (DP88_CUSTOM|CONQUEST|0x02)         //!< Sent to the dp88_conquestController to grant <param> points to team 1

#define TIMER_CONQUEST_DOINTRO                    (DP88_TIMER|CONQUEST|0x01)          //!< Used by dp88_conquestController to trigger the intro message
#define TIMER_CONQUEST_DOSCORES                   (DP88_TIMER|CONQUEST|0x02)          //!< Used by dp88_conquestController to trigger the scores message
#define TIMER_CONQUEST_TICK                       (DP88_TIMER|CONQUEST|0x03)          //!< Tick event timer used by conquest scripts
#define TIMER_CONQUEST_THINK                      (DP88_TIMER|CONQUEST|0x04)          //!< Think event timer used by conquest scripts

// -------------------------------------------------------------------------------------------------
// Remote control group
// -------------------------------------------------------------------------------------------------

#define CUSTOM_REMOTECONTROL_CREATED              (DP88_CUSTOM|REMOTECONTROL|0x01)    //!< Sent by a remote controlled vehicle when it is created
#define CUSTOM_REMOTECONTROL_DRIVEREXIT           (DP88_CUSTOM|REMOTECONTROL|0x02)    //!< Sent by a remote controlled vehicle when it's driver exits
#define CUSTOM_REMOTECONTROL_DESTROYED            (DP88_CUSTOM|REMOTECONTROL|0x03)    //!< Sent by a remote controlled vehicle when it is destroyed
#define CUSTOM_REMOTECONTROL_CONSOLEID            (DP88_CUSTOM|REMOTECONTROL|0x04)
#define CUSTOM_REMOTECONTROL_CONSOLEACK           (DP88_CUSTOM|REMOTECONTROL|0x05)
#define CUSTOM_REMOTECONTROL_DISABLED             (DP88_CUSTOM|REMOTECONTROL|0x06)    //!< Send to a remote control console to disable it
#define CUSTOM_REMOTECONTROL_ENABLED              (DP88_CUSTOM|REMOTECONTROL|0x07)    //!< Send to a remote control console to enable it

#define TIMER_REMOTECONTROL_TIMEOUT               (DP88_TIMER|REMOTECONTROL|0x01)     //!< Timer to time out a remote vehicle purchase if we don't get confirmation within the deadline
#define TIMER_REMOTECONTROL_DRIVERENTER           (DP88_TIMER|REMOTECONTROL|0x02)     //!< Timer to place driver into remove vehicle once transitions are enabled
#define TIMER_REMOTECONTROL_DRIVEREXIT            (DP88_TIMER|REMOTECONTROL|0x03)     //!< Timer to return driver from a dead remote vehicle
#define TIMER_REMOTECONTROL_CHARGETICK            (DP88_TIMER|REMOTECONTROL|0x04)     //!< Timer to count down charge time

// -------------------------------------------------------------------------------------------------
// Ore Fields and Miners script group
// -------------------------------------------------------------------------------------------------

#define CUSTOM_MINER_ENTERED_ORE_FIELD            (DP88_CUSTOM|OREMINING|0x01)        //!< Sent by ore field zone to miners on entry
#define CUSTOM_MINER_EXITED_ORE_FIELD             (DP88_CUSTOM|OREMINING|0x02)        //!< Sent by ore field zone to miners on exit
#define CUSTOM_MINER_ENTERED_DUMP_ZONE            (DP88_CUSTOM|OREMINING|0x03)        //!< Sent by ore dump zone to miners on entry
#define CUSTOM_MINER_INCREASE_ORE_LOAD            (DP88_CUSTOM|OREMINING|0x04)        //!< Sent by miners to themselves on a delay to increase their ore load
#define CUSTOM_MINER_UNLOAD_ORE_COMPLETE          (DP88_CUSTOM|OREMINING|0x05)        //!< Sent by miners to themselves on a delay to finish unloading ore
#define CUSTOM_MINER_AI_SEARCH_FOR_ORE            (DP88_CUSTOM|OREMINING|0x06)        //!< Sent by AI miners to search for an ore field
#define CUSTOM_CHRONO_MINER_DO_CHRONOSHIFT        (DP88_CUSTOM|OREMINING|0x07)        //!< Sent by Chrono Miner to itself to trigger chronoshift
#define CUSTOM_CHRONO_MINER_RETRY_CHRONOSHIFT     (DP88_CUSTOM|OREMINING|0x08)        //!< Sent by Chrono Miner to itself to retry a failed chronoshift

#define TIMER_OREMINING_EXTRACTOR                 (DP88_TIMER|OREMINING|0x01)         //!< Timer for the ore extractor

// -------------------------------------------------------------------------------------------------
// Custom AI group
// -------------------------------------------------------------------------------------------------

#define CUSTOM_TURRETAI_ENEMYSEEN                 (DP88_CUSTOM|AI|0x01)               //!< Sent from dp88_AI_PopupTurret_Spotter to dp88_AI_PopupTurret
//#define CUSTOM_TURRETAI_ANIMATIONCOMPLETE         (DP88_CUSTOM|AI|0x02)               //!< Sent from dp88_AI_ChargeTurret_Animation to dp88_AI_ChargeTurret
#define CUSTOM_AI_DISABLEAI                       (DP88_CUSTOM|AI|0x03)               //!< Custom message that can be sent to a turret AI to disable it
#define CUSTOM_AI_ENABLEAI                        (DP88_CUSTOM|AI|0x04)               //!< Custom message that can be sent to a turret AI to enable it
#define CUSTOM_AI_RESET_ACTIONS                   (DP88_CUSTOM|AI|0x05)               //!< Custom message that can be sent to an AI to reset its current actions

#define TIMER_AI_THINK                            (DP88_TIMER|AI|0x01)                //!< Fires every second to process AI actions such as checking target status
#define TIMER_AI_CHARGE_PRERELOAD_COMPLETE        (DP88_TIMER|AI|0x02)                //!< Used by dp88_AI_ChargedTurret to indicate when pre-reloading is complete

// -------------------------------------------------------------------------------------------------
// AR IFV group
// -------------------------------------------------------------------------------------------------

#define CUSTOM_IFV_REQUESTDRIVERDATA              (DP88_CUSTOM|AR_IFV|0x01)           //!< Request IFV anim frame and powerup data from driver
#define CUSTOM_IFV_DRIVER_ANIMFRAME               (DP88_CUSTOM|AR_IFV|0x02)           //!< IFV deployed anim frame for driver
#define CUSTOM_IFV_DRIVER_ROOKIEPOWID             (DP88_CUSTOM|AR_IFV|0x03)           //!< IFV deployed rookie weapon POW id for driver
#define CUSTOM_IFV_DRIVER_VETERANPOWID            (DP88_CUSTOM|AR_IFV|0x04)           //!< IFV deployed veteran weapon POW id for driver
#define CUSTOM_IFV_DRIVER_ELITEPOWID              (DP88_CUSTOM|AR_IFV|0x05)           //!< IFV deployed elite weapon POW id for driver

#define TIMER_IFV_ALLOWDEPLOY                     (DP88_TIMER|AR_IFV|0x01)            //!< Timer to allow deployment again
#define TIMER_IFV_DEPLOYCOMPLETE                  (DP88_TIMER|AR_IFV|0x02)            //!< Timer to set new animation frame after deployment










/****************************
Unsorted customs and timers
****************************/

// Customs for Prism Tower script
#define CUSTOM_PRISMTOWER_REQUEST_CHARGING			1144060001
#define CUSTOM_PRISMTOWER_STOP_CHARGING				1144060002


// Custom for changing bot count on a map
#define CUSTOM_AI_CHANGE_BOTCOUNT					1144062001
#define CUSTOM_AI_CHANGE_BOTCOUNTTEAM				1144062002

#define TIMER_CLEG_PHASEDEATH					10055222
#define TIMER_CLEG_CHECKDROPOBJ					10055223
#define TIMER_CLEG_CHECKRELEASETARGET			10055224
#define TIMER_LINKHEALTH						10055235
#define TIMER_LINKVETPOINTS						10055236

// Timers for prism towers and tesla coils
#define TIMER_TESLACOIL_DECREMENT_CHARGE      2244040002

// Timers for dp88_chronoTank
#define TIMER_CHRONOTANK_ADVANCE_STATE        2244060001    //!< Timer to advance to the next state
#define TIMER_CHRONOTANK_RANDOM_INTERFERENCE  2244060002    //!< Timer to randomly alter velocity and heading during shifted out phase (unstable vortex)
