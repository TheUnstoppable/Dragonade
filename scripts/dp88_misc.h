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
#include "LoopedAnimationController.h"



/*------------------------
Random weather script
--------------------------*/

/*!
* \brief Weather Randomizer
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_weather
*
* This script can be used to create randomised weather conditions on a map which change periodically
* during the game. The range of weather conditions that are applied can be controlled via the script
* parameters to ensure inappropriate types of weather are not used (such as snow in the desert).
*
* You should only have one instance of this script active at any given time, generally it should be
* attached to a map controller object such as a Daves Arrow preset.
*
* \param update_interval_min
*   The minimum time, in seconds, between weather update checks. Note that unless probability_change
*   is set to 100 an update check does not necessarily cause new weather conditions to occur
* \param update_interval_max
*   The maximum time, in seconds, between weather update checks. Note that unless probability_change
*   is set to 100 an update check does not necessarily cause new weather conditions to occur
* \param probability_change
*   The percentage probability that new weather conditions will be generated when an update check
*   occurs
* \param fog_start_distance_min
*   The minimum starting fog distance that can be applied
* \param fog_start_distance_max
*   The maximum starting fog distance that can be applied
* \param fog_end_distance_min
*   The minimum end of fog distance that can be applied
* \param fog_end_distance_max
*   The maximum end of fog distance that can be applied
* \param precipitation_probability_snow
*   The percentage probability of snow occuring
* \param precipitation_probability_rain
*   The percentage probability of rain occuring
* \param precipitation_probability_volcanic_ash
*   The percentage probability of volcanic ash occuring
* \param precipitation_density_min
*   The minimum precipitation density that can be applied
* \param precipitation_density_max
*   The maximum precipitation density that can be applied
* \param lighting_probability_normal
*   The percentage probability of normal lightning occuring
* \param lighting_probability_war_blitz
*   The percentage probability of a war blitz occuring
* \param lighting_intensity_min
*   The minimum intensity of lightning that can be applied
* \param lighting_intensity_max
*   The maximum intensity of lightning that can be applied
* \param lighting_start_distance_min
*   The minimum starting distance for lightning that can be applied
* \param lighting_start_distance_max
*   The maximum starting distance for lightning that can be applied
* \param lighting_end_distance_min
*   The minimum end distance for lightning that can be applied
* \param lighting_end_distance_max
*   The maximum end distance for lightning that can be applied
* \param lighting_heading_min
*   The minimum lightning heading that can be applied
* \param lighting_heading_max
*   The maximum lightning heading that can be applied
* \param lighting_distribution_min
*   The minimum lightning distribution that can be applied
* \param lighting_distribution_max
*   The maximum lightning distribution that can be applied
* \param wind_heading_min
*   The minimum wind heading that can be applied
* \param wind_heading_max
*   The maximum wind heading that can be applied
* \param wind_speed_min
*   The minimum wind speed that can be applied
* \param wind_speed_max
*   The maximum wind speed that can be applied
* \param wind_variability_min
*   The minimum wind speed variability that can be applied
* \param wind_variability_max
*   The maximum wind speed variability that can be applied
*
* \note
*   The probability of clear conditions (no rain, snow or volcanic ash) is the 100% minus the
*   combined probabilities of rain, snow and volcanic ash that are configured in the script. For
*   example, if precipitation_probability_snow is 5, precipitation_probability_rain is 15 and
*   precipitation_probability_volcanic_ash is 1 the probability of clear conditions will be 100 -
*   5 - 15 - 1 = 79%
*/
class dp88_randomWeather : public ScriptImpClass
{
	void Created ( GameObject* obj );
	void Timer_Expired ( GameObject* obj, int number );
};





/*------------------------
Unit sounds script
--------------------------*/

class dp88_unitSounds : public ScriptImpClass
{
	void Created ( GameObject* obj );
	void Damaged( GameObject *obj, GameObject *damager, float amount );
	void Killed ( GameObject *obj, GameObject *killer );
	void Timer_Expired ( GameObject* obj, int number );
	void Custom ( GameObject* obj, int type, int param, GameObject* sender );

	// Enabled states
	bool damagedEnabled;
	bool heavilyDamagedEnabled;
	bool healedEnabled;
	bool fallingDamageEnabled;

	// Counters
	int healAmount;
};



/*------------------------
Unique Infantry Controller
--------------------------*/

class dp88_uniqueInfantry_controller : public JFW_Object_Created_Hook_Base
{
	void Created ( GameObject* obj );
	void ObjectCreateHook ( GameObject* obj );
	void Destroyed ( GameObject* obj );

	char* infantry_preset;
	int team_id;
	int max_count;
	int percent_chance;
};



/*!
* \brief Damage animation script
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* This is a generic damage animation script that allows you to choose the health thresholds at which
* different damage states are applied, rather than being tied into the 25%, 50%, 75% etc permitted
* by the damage bones mechanism.
*
* You can create up to six damage states using this script, with both powered and unpowered versions
* of each state, however if you don't require all six states you can leave the extras unconfigured.
*
* The first state is considered to be the 'healthy' state and is used when the object is above the
* first damage boundary. Each subsequent state is activated when the health of the object drops
* below the specified boundary, which is a percentage of total health. So, for instance, if you set
* boundary 1 to 70 and boundary 2 to 30 your object will animate as follows;
*
* Health 71% - 100%: Healthy animation
* Health 31% - 70%: Damage animation 1
* Health 1% - 0%: Damage animation 2
*
* To use the WakeupObjects parameter, create an obbox object in max/gmax and name it WAKEBOX. Then
* set WakeupObjects to 1. Any time the object changes damage states, any objects in the WAKEBOX box
* will be forced awake so they can run their physics simulation (e.g. falling through newly-existing
* holes in the mesh
*
* \param animationName
*   Name of the animation containing all the damage state frames
* \param loopAnimation
*   True if the damage animation should be played in a loop, false otherwise
* \param healthy_startFrame
*   First frame number for the healthy state animation
* \param healthy_endFrame
*   Last frame number for the healthy state animation
* \param healthy_startFrame_lowPower
*   First frame number for the healthy state animation when power is offline, or -1 to use the same
*   value as healthy_startFrame
* \param healthy_endFrame_lowPower
*   Last frame number for the healthy state animation when power is offline, or -1 to use the same
*   value as healthy_endFrame
* \param damageBoundary1
*   Damage percent boundary for the first damage state
* \param damageBoundary1_startFrame
*   First frame number for the first damage state animation
* \param damageBoundary1_endFrame
*   Last frame number for the first damage state animation
* \param damageBoundary1_startFrame_lowPower
*   First frame number for the first damage state animation when power is offline, or -1 to use the
*   same value as damageBoundary1_startFrame
* \param damageBoundary1_endFrame_lowPower
*   Last frame number for the first damage state animation when power is offline, or -1 to use the
*   same value as damageBoundary1_endFrame
* \param damageBoundary2
*   Damage percent boundary for the second damage state, or -1 to disable this state
* \param damageBoundary2_startFrame
*   second frame number for the second damage state animation
* \param damageBoundary2_endFrame
*   Last frame number for the second damage state animation
* \param damageBoundary2_startFrame_lowPower
*   second frame number for the second damage state animation when power is offline, or -1 to use
*   the same value as damageBoundary2_startFrame
* \param damageBoundary2_endFrame_lowPower
*   Last frame number for the second damage state animation when power is offline, or -1 to use the
*   same value as damageBoundary2_endFrame
* \param damageBoundary3
*   Damage percent boundary for the third damage state, or -1 to disable this state
* \param damageBoundary3_startFrame
*   third frame number for the third damage state animation
* \param damageBoundary3_endFrame
*   Last frame number for the third damage state animation
* \param damageBoundary3_startFrame_lowPower
*   third frame number for the third damage state animation when power is offline, or -1 to use
*   the same value as damageBoundary3_startFrame
* \param damageBoundary3_endFrame_lowPower
*   Last frame number for the third damage state animation when power is offline, or -1 to use the
*   same value as damageBoundary3_endFrame
* \param damageBoundary4
*   Damage percent boundary for the fourth damage state, or -1 to disable this state
* \param damageBoundary4_startFrame
*   fourth frame number for the fourth damage state animation
* \param damageBoundary4_endFrame
*   Last frame number for the fourth damage state animation
* \param damageBoundary4_startFrame_lowPower
*   fourth frame number for the fourth damage state animation when power is offline, or -1 to use
*   the same value as damageBoundary4_startFrame
* \param damageBoundary4_endFrame_lowPower
*   Last frame number for the fourth damage state animation when power is offline, or -1 to use the
*   same value as damageBoundary4_endFrame
* \param damageBoundary5
*   Damage percent boundary for the fifth damage state, or -1 to disable this state
* \param damageBoundary5_startFrame
*   fifth frame number for the fifth damage state animation
* \param damageBoundary5_endFrame
*   Last frame number for the fifth damage state animation
* \param damageBoundary5_startFrame_lowPower
*   fifth frame number for the fifth damage state animation when power is offline, or -1 to use
*   the same value as damageBoundary5_startFrame
* \param damageBoundary5_endFrame_lowPower
*   Last frame number for the fifth damage state animation when power is offline, or -1 to use the
*   same value as damageBoundary5_endFrame
* \param wakeObjects
*   Whether to wake up objects that might be on top of this object
* \param useDestroyedMode
*   Set this to 1 to cause the last damage state to be a "destroyed" state, which means that the
*   animation will not change again until it is repaired to 100% health
*/

class dp88_damageAnimation : public ScriptImpClass
{
  void Created ( GameObject* obj );
  void Detach ( GameObject* obj );
  void Damaged( GameObject *obj, GameObject *damager, float amount );
  void Timer_Expired ( GameObject* obj, int number );

  void SetDamageAnimation ( GameObject* obj );

  int currentDamageLevel;
  bool basePowerState;
  bool m_bIsDestroyed;

  /*! \name Cached Script Parameters */
  /*! @{ */
  int damageLevelBoundaries[6];
  int damageLevelStartFrames[6];
  int damageLevelEndFrames[6];
  int damageLevelLowPowerStartFrames[6];
  int damageLevelLowPowerEndFrames[6];
  bool loopAnimation;
  bool m_bUseDestroyedMode;
  /*! @} */
protected:
  LoopedAnimationController* m_pLoopedAnimCtrl;
public:
  dp88_damageAnimation() : m_pLoopedAnimCtrl(0) {}
};



/*------------------------
Object Annoucements script
--------------------------*/

class dp88_objectAnnouncements : public ScriptImpClass
{
	void Created ( GameObject* obj );
	void Damaged ( GameObject *obj, GameObject *damager, float amount );
	void Killed ( GameObject *obj, GameObject *killer );

	time_t lastAnnouncementTime;
	int minimumAnnounceInterval;

	unsigned long team0_underAttackStringId;
	unsigned long team1_underAttackStringId;
	unsigned long team0_destroyedStringId;
	unsigned long team1_destroyedStringId;

	unsigned long team0_underAttackSoundId;
	unsigned long team1_underAttackSoundId;
	unsigned long team0_destroyedSoundId;
	unsigned long team1_destroyedSoundId;
};





/*------------------------
Chrono Tank Script
--------------------------*/

class dp88_chronoTank : public JFW_Key_Hook_Base
{
	void Created ( GameObject* obj );
	void Killed ( GameObject *obj, GameObject *killer );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );
	void Timer_Expired ( GameObject* obj, int number );
	void KeyHook();

	void Shift_Out_Begin(GameObject* obj);
	void Shift_Out_Complete(GameObject* obj);
	void Shift_Out_Cleanup(GameObject* obj);
	void Shift_In_Begin(GameObject* obj);
	void Shift_In_Complete(GameObject* obj);
	void Shift_In_Cleanup(GameObject* obj);

	int state;
	int object_id;		// For KeyHook (obj is not provided)
	int driver_id;
	int shifted_vehicle_object_id;

private:
	// Define states
	enum CT_STATES
	{
		CT_STATE_CHARGING,
		CT_STATE_CHARGED,
		CT_STATE_SHIFTING_OUT,
		CT_STATE_SHIFTED_OUT_PENDING_CLEANUP_1,
		CT_STATE_SHIFTED_OUT_PENDING_CLEANUP_2,
		CT_STATE_SHIFTED_OUT,
		CT_STATE_SHIFTING_IN,
		CT_STATE_SHIFTED_IN_PENDING_CLEANUP_1,
		CT_STATE_SHIFTED_IN_PENDING_CLEANUP_2
	};
};



/*------------------------
Spawn Vehicle Part script
--------------------------*/

class dp88_spawnVehiclePart : public ScriptImpClass
{
	void Created ( GameObject* obj );
	void Timer_Expired( GameObject *obj, int number );
	void Killed ( GameObject* obj, GameObject *killer );
	void Destroyed( GameObject* obj );

	int partObjectId;
};



/*!
* \brief Custom Unit Points & Credits
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts
*
* This script provides finer control over the points and credits awarded for damaging or repairing
* an object, as opposed to the values in an LevelEdit preset which award equal amounts of score and
* credits.
*
* \note
*   You should set the points value of the object to 0 in the LevelEdit preset settings or both
*   values will be applied
*
* \param killPoints
*   Number of points to award for killing the object
* \param damagePoints
*   Number of points to award per hitpoint of damage done to the object
* \param repairPoints
*   Number of points to award per hitpoint repaired on object
* \param killMoney
*   Amount of credits to award for killing the object
* \param damageMoney
*   Amount of credits to award per hitpoint of damage done to the object
* \param repairMoney
*   Amount of credits to award per hitpoint repaired on object
*/
class dp88_customPoints : public ScriptImpClass
{
protected:
  void Created ( GameObject* obj );
  void Damaged ( GameObject *obj, GameObject *damager, float amount );
  void Killed ( GameObject* obj, GameObject *killer );

  void GivePointsAndMoney ( GameObject* obj, float points, float money );

  /*! \name Cached Script Parameters */
  /*! @{ */
  float m_killPoints;
  float m_damagePoints;
  float m_repairPoints;
  float m_killMoney;
  float m_damageMoney;
  float m_repairMoney;
  /*! @} */
};

class dp88_customPointsPercentage : public ScriptImpClass
{
protected:
	void Created(GameObject* obj);
	void Damaged(GameObject *obj, GameObject *damager, float amount);
	void Killed(GameObject* obj, GameObject *killer);

	void GivePointsAndMoney(GameObject* obj, float points, float money);

	float m_killPoints;
	float m_damagePoints;
	float m_repairPoints;
	float m_killMoney;
	float m_damageMoney;
	float m_repairMoney;
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Conquest Mode - Controller
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_conquest
*
* This script controls conquest mode and must be placed on exactly one object on any map where you
* want to enable conquest mode, such as a Daves Arrow. It stores the points totals for each team
* and continually sends messages to the server with the current scores.
*
* The controller allows you to choose from one of two scoring systems, which basically work the same
* way but in different directions. The default scoring system is points accumulation mode, where
* each team starts with zero points and the points they earn are added to their total. The first
* team to reach the target score wins the game.
*
* Alternatively you can enable points deducation mode, where each team starts with a set number of
* points and the points they score are removed from the other teams total. The first team to have
* their score reduced to zero loses the game.
*
* \pre
*   Requires the base destruction victory mode to be enabled in the server configuration otherwise
*   it will be unable to end the game when a team achieves their victory condition
*
* \param ScoringMode
*    Specifies which scoring system to use. 0 = default, 1 = deduction mode
* \param TargetScoreTeam0
*    This is the number of points team 0 (Nod / Soviet) need to win the game or, if deduction mode
*    is enabled, the number of points team 0 starts with
* \param TargetScoreTeam1
*    This is the number of points team 1 (GDI / Allies) need to win the game or, if deduction mode
*    is enabled, the number of points team 1 starts with
* \param IntroString
*    The name of an entry in the strings database containing a message to be displayed to players at
*    the start of the map which can be used to explain how to win. It will also be sent to players
*    when they join mid game. See the note below for a list of macros you can use in this string
* \param ScoreInterval
*    Specifies how often, in seconds, the controller should send a message to the server with the
*    current scores. Set this to 0 if you do not want the scores to be shown.
* \param ScoreString
*    The name of strings database entry containing the message to be sent to the server with the
*    current scores. If this is blank a generic message will be shown. See the note below for a list
*    of macros you can use in this string
* \param ScoreStringTeam0
*    Optional. The name of a strings database entry containing an alternative score string to be
*    used if team 0 are currently winning. If this is blank the value of ScoreString will be used
*    instead. See the note below for a list of macros you can use in this string
* \param ScoreStringTeam1
*    Optional. The name of a strings database entry containing an alternative score string to be
*    used if team 1 are currently winning. If this is blank the value of ScoreString will be used
*    instead. See the note below for a list of macros you can use in this string
*
* \note
*   The following macros can be used in strings database entries;\n
*   \n
*   <table>
*    <tr><th>Macro</th>                     <th>Value</th></tr>
*    <tr><td>%%TEAM0%</td>                  <td>Translated name of team 0</td></tr>
*    <tr><td>%%TEAM1%</td>                  <td>Translated name of team 1</td></tr>
*    <tr><td>%%WINNINGTEAM%</td>            <td>Translated name of winning team, or 'Neither' if the points are level</td></tr>
*    <tr><td>%%LOSINGTEAM%</td>             <td>Translated name of losing team, or 'Neither' if the points are level</td></tr>
*    <tr><td>%%TEAM0TARGET%</td>            <td>Target score for team 0 (or starting score in deduction mode)</td></tr>
*    <tr><td>%%TEAM1TARGET%</td>            <td>Target score for team 1 (or starting score in deduction mode)</td></tr>
*    <tr><td>%%TEAM0SCORE%</td>             <td>Current score for team 0</td></tr>
*    <tr><td>%%TEAM1SCORE%</td>             <td>Current score for team 1</td></tr>
*    <tr><td>%%TEAM0SCORENEEDED%</td>       <td>Points required before team 0 will win</td></tr>
*    <tr><td>%%TEAM1SCORENEEDED%</td>       <td>Points required before team 1 will win</td></tr>
*   </table>
*/
class dp88_conquestController : public ScriptImpClass
{
  void Created ( GameObject* pSelf );
  void Destroyed ( GameObject* pSelf );
  void Timer_Expired ( GameObject* pSelf, int number );
  void Custom ( GameObject* pSelf, int type, int param, GameObject* pSender );

  int GetWinningTeamID();
  int GetLosingTeamID();

  void DisplayMessage(StringClass message);

protected:
  /*!
  * \name Scoring Modes
  * Scoring mode definitions, see the class description for an explaination of these modes
  */
  ///@{
  static const unsigned char MODE_DEFAULT   = 0;  /*!< Default, points accumulation mode */
  static const unsigned char MODE_DEDUCTION = 1;  /*!< Points deduction mode */
  ///@}

  unsigned char m_scoringMode;  /*!< The scoring mode being used */
  int m_points[2];              /*!< Current points total for each team */
  int m_targets[2];             /*!< Target for each team, or starting points in deduction mode */

private:
  int m_objectCreateHookId;
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Conquest Mode - Control Zone
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_conquest
*
* Attach this script to an object to create a conquest control zone around or near that object. A
* control zone can be taken over by a team if one or more players from that team are standing in the
* zone with no enemy players. A zone can grant conquest points to a team upon it's initial capture
* and continually for as long as they control the zone. Requires a controller object to be placed
* on the map with the dp88_conquestController script attached to it before this script can work.
*
* If the zone is currently neutral it takes CaptureTime seconds for one player to take control of
* the zone for their team. If the zone is currently owned by the enemy team it takes CaptureTime
* seconds for one player to make the zone neutral, after which they can take control of the zone as
* normal.
*
* Additional units can contribute to the capturing process to reduce the time required to take
* control of the zone, up to MultiCaptureCap. Any additional players in the zone beyond the maximum
* cap will not provide any additional boost to the capture speed. Note that the cap is the total
* number of units, including the first unit to enter the zone, so a cap of 1 will disable the
* ability to boost capture speeds.
*
* Each additional player within the cap, beyond the first player, will reduce the capture time by
* (CaptureTime*MultiCaptureMultiplier) seconds, thus with a multipler of 0.5 each additional player
* will reduce the capture time by half of the original time.
*
* If AllowMajorityCapture is enabled the zone can be captured whilst there are enemy players in the
* zone by the team which has the most players in the zone. If combined with a MultiCaptureCap >1
* each enemy player in the zone nullifies the effect of one of the majority teams players, reducing
* the capture speed boost. Thus, with 5 players on the majority team and 3 players on the minority
* team in the zone the capture speed will be 50% of the original time, rather than the 20% it would
* be if there were no enemy team players in the zone (assuming a MultiCaptureMultiplier of 1.0).
*
* An animation can be played on the object upon which the script is attached to indicate the current
* state of the control zone, which consists of a single frame for each of the three fixed states
* (team0 controlled, neutral, team1 controlled) and optionally a series of intermediate transition
* frames which will be used to indicate how close the zone is to being captured or returned to its
* neutral state. You could attach other animated objects to the animation using bones if a continual
* animation is required, such as a waving flag that moves up and down a flagpole during capture.
*
* If the zone is partially captured and there is no longer anybody in the zone it will slowly revert
* to its previous state, losing 1 second of capture progress per second.
*
* \param ZoneSize
*   The size of the capture zone to be created
* \param ZoneOffset
*   Optional. The offset from the object location the zone should be centered upon
* \param ZonePreset
*   The preset to use for the capture zone, this does not require any scripts to be attached
* \param CapturePoints
*   The number of points to give to a team when they take control of the zone from neutral. If the
*   control point spawns as non-neutral no points will be given to the initial team
* \param CapturePointsFirstTeamOnly
*   Determines if CapturePoints are every time a team takes control of a zone or if they are only
*   given the first time the zone is taken over from neutral. 1 to enable, 0 to disable
* \param TickPoints
*   Conquest points to give to the team which currently owns the zone every 'TickInterval' seconds
* \param TickInterval
*   Tick interval, in seconds, at which to give conquest points to the zones current owner
* \param CaptureTime
*   Time, in seconds, required to take control of the zone from neutral or restore the zone to the
*   neutral state from enemy control
* \param MultiCaptureCap
*   The maximum number of units that can contribute to speeding up the capture process. If set to 1
*   or 0 multi capture will be disabled. See the class description for more information
* \param MultiCaptureMultiplier
*   The multipler to be applied to the capture speed by each additional unit, up to the specified
*   cap, where 1.0 gives a 100% boost to the capture speed and 0.5 gives a 50% boost. See the class
*   description for more information 
* \param AllowMajorityCapture
*   Allow a zone to be captured by the team with more players in the zone, even if enemy players are
*   also in the zone. See the class description for more information. 1 to enable, 0 to disable
* \param CaptureAnim
*   Optional. The name of an animation to play on the object to indicate capture progress. This
*   animation should be in the following order: (team0 controlled) -> (team0<->neutral transition)
*   -> (neutral) -> (neutral<->team1 transition) -> (team1 controlled). The controlled and neutral
*   states should be a single frame. The transitions are optional, but if present must contain the
*   same number of frames
* \param CaptureAnimTransitionFrames
*   The number of transition frames between the controlled and neutral states in the CaptureAnim, or
*   zero if there are no transition frames
* \param CapturingString
*   Optional. The name of a strings database entry containing a message to be displayed to the team
*   which currently controls the zone when the enemy team are capturing it. See the note below for a
*   list of macros you can use in this string
* \param NeutralString
*   Optional. The name of a strings database entry containing a message to be displayed to all
*   players when a capture zone becomes neutral. See the note below for a list of macros you can use
*   in this string
* \param CapturedString
*   Optional. The name of a strings database entry containing a message to be displayed to all
*   players when a team takes control of a capture zone. See the note below for a list of macros you
*   can use in this string
* \param AIObjective_Priority_Soldier
*   Optional. AI objective priority for soldiers. See notes below for more details.
* \param AIObjective_Priority_Light_Vehicle
*   Optional. AI objective priority for light vehicles. See notes below for more details.
* \param AIObjective_Priority_Heavy_Vehicle
*   Optional. AI objective priority for heavy vehicles. See notes below for more details.
* \param AIObjective_Priority_Aircraft
*   Optional. AI objective priority for aircraft. See notes below for more details.
*
* \note
*   This script can be used in conjunction with danpaul88's custom AI scripts by setting this zone
*   as an AI objective for both teams, as an offensive objective for teams that do not own the zone
*   and as a defensive objective for teams that do own the zone. The objective priorities can be
*   configured using the <i>AIObjectivePriority_*</i> parameters. See \ref dp88_AI_Objective for
*   more info.
*
* \note
*   The following macros can be used in strings database entries;\n
*   \n
*   <table>
*    <tr><th>Macro</th>                     <th>Value</th></tr>
*    <tr><td>%%OWNINGTEAM%</td>             <td>Translated name of the team which controls (or controlled) the zone</td></tr>
*    <tr><td>%%OTHERTEAM%</td>              <td>Translated name of the other team</td></tr>
*    <tr><td>%%CAPTUREPOINTS%</td>          <td>The value of CapturePoints</td></tr>
*    <tr><td>%%TICKPOINTS%</td>             <td>The value of TickPoints</td></tr>
*    <tr><td>%%TICKINTERVAL%</td>           <td>The value of TickInterval</td></tr>
*    <tr><td>%%ZONENAME%</td>               <td>The translated name of the object this script is attached to</td></tr>
*   </table>
*/
class dp88_conquestControlZone : public ScriptImpClass
{
public:
  void Created( GameObject* pObj );
  void Timer_Expired( GameObject* pObj, int number );

protected:
  void IncrementCaptureProgress( GameObject* pObj, int team, int nPlayers );
  void UpdateAnimationFrame( GameObject* pObj );

  /*!
  * This function is called when the zone has been captured or made neutral
  */
  void ChangeOwner(GameObject* pObj, int team);

  void DisplayMessage(GameObject* pObj, StringClass message, int sendtoteam = 2);

  int m_controllerID;       /*!< ID of the GameObject with the controller script attached */
  int m_zoneID;             /*!< ID of the control zone GameObject */
  float m_captureState;     /*!< Current capture progress. 0 = neutral, negative = team0, positive = team1 */

  bool m_bTickRunning;      /*!< True if the tick timer is currently running, false otherwise */

  /*! \name Cached Script Parameters */
  /*! @{ */
  int m_captureTime;
  int m_multiCaptureCap;
  float m_multiCaptureMultiplier;
  bool m_bAllowMajorityCapture;
  int m_nAnimTransitionFrames;
  
  int m_aiPriority_soldier;
  int m_aiPriority_lvehicle;
  int m_aiPriority_hvehicle;
  int m_aiPriority_aircraft;
  /*! @} */
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Conquest Mode - Control Object
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_conquest
*
* Attach this script to an object to generate conquest points for the team which owns the object.
* This script does not include any mechanism for changing the owner of the object and therefore can
* be used on something the enemy must destroy to reduce the opponents points income or combined with
* additional scripts to allow the object to change team, for example;
*
*   A capturable building using the \ref scripts_dp88BuildingScripts "dp88_buildingScripts" set of
*   scripts with a capture point object.
*
* \param TickPoints
*   Conquest points to give to the team which currently owns the object every 'TickInterval' seconds
* \param TickInterval
*   Tick interval, in seconds, at which to give conquest points to the objects current owner
*/
class dp88_conquestControlObject : public ScriptImpClass
{
public:
  void Created( GameObject* pObj );
  void Timer_Expired( GameObject* pObj, int number );

protected:
  int m_controllerID;       /*!< ID of the GameObject with the controller script attached */
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Radar Invisibility
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_radar
*
* Attach this script to an object to make it invisible to the radar - when the script is detached it
* will restore the original radar state. If this script is attached to a vehicle it will also make
* passengers radar-invisible until they leave the vehicle again.
*/
class dp88_radarInvisibility : public ScriptImpClass
{
public:
  /*! Constructor to prevent crashes in LE due to uninitialised variables */
  dp88_radarInvisibility() : m_pPassengerIds(0), m_pPassengerRadarModes(0), m_nSeats(0) {};

protected:
  void Created ( GameObject* pObj );
  void Custom ( GameObject* pObj, int type, int param, GameObject* pSender );
  void Detach ( GameObject* pObj );

  void HidePassengerBlip ( class VehicleGameObj* pVehicle, class SoldierGameObj* pSoldier );
  void RestorePassengerBlip ( class SoldierGameObj* pSoldier );

  int m_originalRadarMode;      //!< The original radar mode of this object
  int* m_pPassengerIds;         //!< For vehicles, the ID of each passenger
  int* m_pPassengerRadarModes;  //!< For vehicles, the original radar mode of each passenger
  int m_nSeats;                 //!< For vehicles, cache the number of seats (for Detach() to use)
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Turret Rotation Sound Effect
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_sound
*
* This script plays a 3d sound at a vehicles turret bone when that bone is being rotated and stops
* the sound when the rotation stops. The sound will be looped whilst the turret is being rotated.
*
* \note
*   This script uses the difference between the vehicle rotation and the turret bone rotation to
*   determine if the turret is rotating. This means simply aiming in one direction and spinning on
*   the spot will cause the sound to be played, since the turret is rotating relative to the vehicle
*
* \param Sound_Preset
*   The name of a 3D sound preset to be played whilst the turret is rotating
* \param Min_Differential_Rad
*   The minimum difference in the turret rotation, in radians, to be considered as "rotating", this
*   helps to filter out tiny movements caused by driving along uneven terrain.
*/
class dp88_turretSound : public ScriptImpClass
{
protected:
  void Created ( GameObject* pObj );
  void Timer_Expired ( GameObject* pObj, int number );
  void Custom ( GameObject* pObj, int type, int param, GameObject* pSender );

  float Get_Turret_Facing ( class RenderObjClass* pRenderObj );
  void Play_Sound ( GameObject* pObj );
  void Stop_Sound ( GameObject* pObj );

  float m_lastFacing;
  int m_nSoundId;

  /*! \name Cached Script Parameters */
  /*! @{ */
  float m_nMinDifferential;
  /*! @} */
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Yet Another Teleport Script
* \author Daniel Paul
* \ingroup scripts
*
* The teleport script to end all teleport scripts, this is designed to cover almost any scenario for
* teleporting units to a location or another object. It can be attached to either a script zone, a
* pokeable object or a powerup for maximum flexibility.
*
* \param ObjectID
*   The ID of an object to teleport to, or 0 to teleport to a location instead. If a target object
*   with the specified ID is not found the script will not teleport the unit.
* \param Bone
*   The bone on the object to teleport to, or 0 to teleport to the objects origin. Not applicable
*   if ObjectID is set to 0.
* \param Offset
*   If teleporting to an object (or one of it's bones), apply this offset to the object location.
*   Otherwise this is used as the absolute position to teleport to (ie: an offset from 0,0,0)
* \param Teleport_Infantry
*   Set this to 1 if infantry should be teleported by this script, 0 if it should ignore infantry
* \param Teleport_Vehicles
*   Set this to 1 if vehicles should be teleported by this script, 0 if it should ignore vehicles
*
* \note
*   If you only want to teleport players and not AI controlled units use a script zone and tick the
*   <i>CheckStarsOnly</i> box in LevelEdit.
*/
class dp88_teleport : public ScriptImpClass
{
protected:
  void Created ( GameObject* pObj );
  void Entered ( GameObject* pObj, GameObject* pEnterer );
  void Custom ( GameObject* pObj, int type, int param, GameObject* pSender );
  void Poked ( GameObject* pObj, GameObject* pPoker );

  /*! \name Cached Script Parameters */
  /*! @{ */
  int m_nObjectID;
  bool m_bUseBone;
  Vector3 m_offset;

  bool m_bTeleportInfantry;
  bool m_bTeleportVehicles;
  /*! @} */
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Vehicle Ammo Animation
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* This script plays an animation on a vehicle based on the currently loaded ammunition, allowing a
* visual representation of ammo being used and, if desired, more than two firing locations, which is
* achieved by moving the muzzlea0 bone as part of the ammo animation. Note that since scripts are
* executed server side latency can affect the speed at which the animation updates, therefore this
* is not recommended for use with high ROF weapons.
*
* \param Animation
*   The name of the animation to use, where frame 0 is the first frame used when the weapon is fully
*   loaded and <i>Animation_Frames</i> is the final frame with no ammunition remaining
* \param Animation_Frames
*   The number of frames to use from the specified animation
* \param Use_Total_Ammo
*   Specify whether the animation should be based on the total ammunition the object has or just
*   those currently loaded into their gun. Set to 0 to count loaded ammo only or 1 to count all
* \param Animation_Model
*   Optional name of a W3D model file to spawn and attach to the object upon which the animation
*   defined in this script will be applied. This allows the host object to have other animations
*   applied, such as damage states via dp88_damageAnimation
* \param Animation_Model_Bone
*   If you specify an Animation_Model above then you must also specify the a bone on the parent to
*   which that model should be attached. This also allows the animation model to be moved by the
*   animation set executing on the parent.
*/
class dp88_Ammo_Animation : public ScriptImpClass
{
public:
  void Created ( GameObject* pObj );
  void Timer_Expired ( GameObject* pObj, int number );

protected:
  void UpdateAnimationFrame ( GameObject* pObj );

  int m_nBullets;

  /*! ID of the animation object we have created for ourselves */
  int m_ammoAnimObjId;

  /*! \name Cached Script Parameters */
  /*! @{ */
  const char* m_strAnimation;
  int m_nFrames;
  bool m_bUseTotal;
  /*! @} */
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Camo Controller
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* This script is the controller for dp88_Camo_Object and defines which camouflage varient should be
* used on the current map. Only one instance of this controller should exist on a map or you may
* get unexpected results. Typically you will create a new Daves Arrow type preset and attach this
* script to that preset to place on your map.
*
* Each object that is created with one or more copies of dp88_Camo_Object attached will check the
* variant specified in each against the one in this script and, if one matches, will update it's
* model to the appropriate one for that camo type.
*
* \param Camouflage
*   The ID of the camouflage variant to use for this map
*/
class dp88_Camo_Controller : public ScriptImpClass
{
public:
  virtual void Created ( GameObject* pObj );

  const char* GetCamoVariant() const { return m_camoVariant; };

protected:
  const char* m_camoVariant;
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Camo Object
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* Attach one or more copies of this script to an object to specify alternative camouflage models to
* use in different environments. Each copy of the script defines one camouflage varient and a model
* filename to use in that environment. If the map contains an instance of dp88_Camo_Controller which
* matches the camouflage variant specified in the script the model will be updated to match.
*
* If there is no dp88_Camo_Controller script instance on a map or its variant does not match any of
* the variants defined in any instances of this script on the object the default model will be used.
*
* \param Camouflage
*   The ID of the camouflage variant this script specifies a model for
* \param Model
*   The filename of the model to use if the <i>Camouflage</i> value matches the controllers value,
*   this should include the .w3d extension
*/
class dp88_Camo_Object : public ScriptImpClass
{
public:
  virtual void Created ( GameObject* pObj );
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Create Object With Cinematic
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_cinematic
*
* Based on SH_CinematicVehicleFactoryBypass this script spawns an object and then attaches it to a
* cinematic. This could be used to spawn vehicles and have them dropped off via a cinematic script
* for example. The benefit of these scripts over spawning objects directly in cinematics is improved
* engine performance since cinematic spawned objects incur more physics engine checks than necessary
*
* This script can either trigger on creation or on a custom trigger. When using a custom trigger it
* can be triggered multiple times if necessary.
*
* The object created will be attached to the cinematic in slot 0 whilst the "owner" will be added in
* slot 1. The definition of the owner is either an object ID specified in the parameter of the
* triggering custom or, if this is 0 or not a valid game object, the sender of the custom, which may
* be the object this script is attached to.
*
* \preset Preset
*   The preset of the object to be created
* \param Cinematic
*   The cinematic script to be played when the object has been created
* \param Trigger_Custom
*   ID of the custom message to listen for or 0 to trigger on creation instead
* \param Offset
*   The offset from the location of the object this script is attached to to create the cinematic at
*
* \note
*   Your cinematic must send the custom message 43000 to the object at an appropriate time to allow
*   it to start taking damage. If you do not send this custom the object will be permanently immune
*   from all damage.
*/
class dp88_Create_Object_Cinematic: public ScriptImpClass
{
public:
  void Created ( GameObject* pObj );
  void Custom ( GameObject* pObj, int type, int param, GameObject* pSender );

protected:
  void Create_Object ( GameObject* pobj, GameObject* pOwner );
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Set Team On Custom
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_customconsumers
*
* Changes the team of the object this script is attached to upon receiving a custom message, where
* the message parameter contains the new team ID for the object which can be either 0 (Nod), 1 (GDI)
* or 2 (Neutral)
*
* This can be combined with JFW_Startup_Custom_Self to set the team of an object when it is created
* without needing to create duplicate copies of a preset for each team (by attaching the scripts to
* a placed object in LE rather than its preset)
*
* \param Message
*   The ID of the custom message to listen for
*
* \note
*   This is compatible with capturable buildings using the \ref scripts_dp88BuildingScripts "dp88_buildingScripts"
*   set of scripts
*/
class dp88_Set_Team_On_Custom : public ScriptImpClass
{
public:
  void Custom ( GameObject* pObj, int type, int param, GameObject* pSender );
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Health Regeneration
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* Attach this script to a unit to enable it to regenerate health and armour hitpoints, up to a
* configurable % of its maximum hitpoints. Additionally it can also be configured to only repair
* hitpoints when not in combat, forcing the player to escape from battle or defeat their opponent
* before they begin regenerating their hitpoints.
*
* This script is compatible with the \ref dp88_veterancyUnit script and includes settings for
* alternative regeneration rates at higher veterancy levels, although it also works as a standalone
* script.
*
* \param Hitpoints
*   The amount of hitpoints to repair each interval
* \param Interval
*   The number of seconds between each repair interval
* \param Warhead
*   The warhead to use when applying repair damage. If this is not set it will apply raw repairs
*   without any warhead scaling. Note that the warhead must have a negative rating vs the unit skin
*   and armour types otherwise the unit will actually take damage every interval
* \param Max_Percent
*   The maximum percentage of the units total hitpoints that can be repaired, set this to 100 to
*   allow the unit to be repaired completely
* \param Damage_Timeout
*   The number of seconds to wait after the last damage event before regeneration is allowed. Set
*   this to 0 to allow regeneration whilst in combat
* \param Hitpoints_Veteran
*   The amount of hitpoints to repair when the unit is promoted to veteran. Only applicable when
*   used alongside the \ref dp88_veterancyUnit script 
* \param Hitpoints_Elite
*   The amount of hitpoints to repair when the unit is promoted to veteran. Only applicable when
*   used alongside the \ref dp88_veterancyUnit script
* \param Mode
*   Added in 4.2, this is a bitflag which controls which hitpoints the script should heal. See the
*   table below for the available modes.
*
* \note
*   The following mode flags are available. To combine modes add the flag values together, for
*   example: Mode 3 would regenerate hitpoints for health (1) and armour (2);\n
*   \n
*   <table>
*    <tr><th>Mode</th>      <th>Target</th></tr>
*    <tr><td>1</td>         <td>Health of this object</td></tr>
*    <tr><td>2</td>         <td>Armour of this object</td></tr>
*    <tr><td>4</td>         <td>Health of the vehicle this object is driving (only if attached to a soldier)</td></tr>
*    <tr><td>8</td>         <td>Armour of the vehicle this object is driving (only if attached to a soldier)</td></tr>
*    <tr><td>16</td>        <td>Health of the vehicle this object is a passenger in (only if attached to a soldier)</td></tr>
*    <tr><td>32</td>        <td>Armour of the vehicle this object is a passenger in (only if attached to a soldier)</td></tr>
*    <tr><td>64</td>        <td>Health of the driver of this object (only if attached to a vehicle)</td></tr>
*    <tr><td>128</td>       <td>Armour of the driver of this object (only if attached to a vehicle)</td></tr>
*    <tr><td>256</td>       <td>Health of the passengers of this object (only if attached to a vehicle)</td></tr>
*    <tr><td>512</td>       <td>Armour of the passengers of this object (only if attached to a vehicle)</td></tr>
*   </table>
*
* \note
*   The maximum percentage cap is based upon the current mode. If configured to heal both the
*   health and armour of a target the cap is the percentage of the combined health and armour
*   hitpoints, otherwise it is the percentage of whichever hitpoints it is configured to repair.
*   \n
*   For example, given an object with 100 health and 100 armour and a cap of 75% the following caps
*   would apply;
*   \n
*   <table>
*    <tr><th>Mode</th>      <th>Cap</th></tr>
*    <tr><td>1</td>         <td>Capped at 75 health, ignores armour</td></tr>
*    <tr><td>2</td>         <td>Capped at 75 armour, ignores health</td></tr>
*    <tr><td>3 (1+2)</td>   <td>Capped at 150 hitpoints (health+armour)</td></tr>
*   </table>
*
* \warning
*   When using a warhead with this script it is possible that healing may occur outside the target
*   parameters due to armor.ini settings. If you want to heal only health or armour for a target it
*   is recommended that you either use no warhead or use a warhead which only affects the hitpoints
*   you want to heal.
*/
class dp88_RegenerateHitpoints : public ScriptImpClass
{
protected:
  const static int MODE_HEALTH                = 1;
  const static int MODE_ARMOUR                = 2;
  const static int MODE_PILOTED_HEALTH        = 4;
  const static int MODE_PILOTED_ARMOUR        = 8;
  const static int MODE_OCCUPIED_HEALTH       = 16;
  const static int MODE_OCCUPIED_ARMOUR       = 32;
  const static int MODE_DRIVER_HEALTH         = 64;
  const static int MODE_DRIVER_ARMOUR         = 128;
  const static int MODE_PASSENGERS_HEALTH     = 256;
  const static int MODE_PASSENGERS_ARMOUR     = 512;

public:
  void Created( GameObject *obj );
  void Timer_Expired( GameObject *obj, int number );
  void Damaged ( GameObject *obj, GameObject *damager, float amount );
  void Custom( GameObject *obj, int type, int param, GameObject *sender );

protected:
  bool IsModeEnabled(int mode);
  void RegenObject(GameObject* obj, bool bHealth, bool bArmour);
  void ApplyNonWarheadRepairs(GameObject* obj, float amount, bool bHealth, bool bArmour);

private:
  /*! Time of the last non-repair damage event */
  time_t m_lastDamage;
  
  /*! \name Cached Script Parameters */
  /*! @{ */
  int m_mode;
  float m_amount;
  float m_interval;
  float m_maxPercent;
  int m_warheadId;
  int m_lastDamageTimeout;
  /*! @} */
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Create Attached Object
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* Spawns a new object and attaches it to this object at a specific bone. This script incorporates
* features from several other similar scripts together into a single script for the first time to
* allow them to be used in any combination.
*
* <h3>Supported Features</h3>
*    - <b>Health Link</b><br/>
*        Synchronize the health of the spawned object with the parent object, in effect creating a
*        shared health bar between the objects. Multiple attached objects can share health in this
*        way to create multi-part objects with a single set of hitpoints
*    - <b>Veterancy Link</b><br/>
*        Works the same way as the Health Link feature but for veterancy points awarded. Only
*        applicable when used alongside the \ref dp88_veterancyUnit script
*    - <b>Enter/Exit Customs</b><br/>
*        Sends a custom message to the spawned object when a driver enters or exits the vehicle.
*        Only applicable when this script is attached to a vehicle.
*
* \param Preset
*   The name of the preset to spawn an instance of
* \param Bone
*   The name of the bone to attach the spawned object to
* \param EnableHealthLink
*   Whether to enable the health linking feature. 1 to enable, 0 to disable
* \param EnableVeterancyLink
*   Whether to enable the veterancy linking feature. 1 to enable, 0 to disable
* \param VisibleToAI
*   Whether the spawned object should be visible to the AI, allowing them to target it independently
*   of this object - generally this should be disabled. 1 to make it visible, 0 to disable
* \param DriverEnterCustom
*   Custom message to send to the spawned object when a driver enters the object
* \param DriverExitCustom
*   Custom message to send to the spawned object when a driver leaves the object
*/
class dp88_CreateAttachedObject : public ScriptImpClass
{
  void Created(GameObject* obj);
  void Killed(GameObject* obj, GameObject *killer);
  void Custom(GameObject *obj, int type, int param, GameObject *sender);

  int m_spawnedObjectId;
  bool m_hasDriver;
};

// -------------------------------------------------------------------------------------------------
// Dynamic spawn system
// -------------------------------------------------------------------------------------------------

/*!
* \brief Dynamic Spawner - Controller
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_dynamicSpawner
*
* The controller script for danpaul88's dynamic spawner system, this allows players for one or all
* teams to use dynamic spawn points which can be created, moved and destroyed during the course of a
* match. Read the documentation carefully!
*
* This script must be combined with instances of any of the other scripts in the \ref scripts_dynamicSpawn
* "Dynamic Spawners" set of scripts or it will have no effect.
*
* This script provides the ability to stack the teams and even split players between more than two
* teams, however implementors should be aware that it does not fundamentally alter the engine and
* the following caveats apply to teams beyond the original two;
*
*   - They cannot have building controllers
*   - They cannot win the game
*   - They will not have a team points score
*   - They may be able to kill each other with friendly fire
*
* You may also find that other scripts behave erratically when using additional teams and could even
* result in crashing the game. This aspect of the controller is highly experimental and not generally
* recommended for use outside well controlled environments.
*
* The controller can attempt to stack the teams by moving newly joined players onto a different team
* to the default one chosen by the engine. By default it will not do this, however if you wish to set
* up assault / defence type scenarios you may want to bias the players to one side or the other.
*
* When a player respawns the controller will search all of the available dynamic spawner instances for
* their team and picks one based on a weighted randomiser. The spawner script itself then takes over
* and moves them to the location of the spawner, optionally applying special effects such as spawning
* them inside a newly created vehicle.
*
* \param Number_Of_Teams
*   The number of teams that should play in the match
* \param Team_Stacking
*   An optional parameter, if this is not blank the script expects to find a space seperated list of
*   team weightings, with 1 number per team specified in <i>Number_Of_Teams</i>. If no value is provided
*   and <i>Number_Of_Teams</i> is > 2 it will distribute players evenly between all teams
*/
class dp88_DynamicSpawner_Controller : ScriptImpClass
{};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Dynamic Spawner - Simple Spawner
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_dynamicSpawner
*
* A basic spawner for use in the \ref scripts_dynamicSpawn "Dynamic Spawners" set of scripts, this
* simply indicates a location at which a player can spawn and, optionally, specifies a preset or a
* twiddler to change what soldier the player spawns as.
*
* \param Weighting
*   The weighting for this spawner in the controller, higher numbers will favour this spawner over
*   other available spawners. This should be a value between 1 and 10
* \param Team
*   The 0 based index of the team which can use this spawner
* \param Infantry_Preset
*   Optional, specifies an infantry preset to spawn the player. You can use a twiddler here to
*   randomise the infantry unit the player spawns with.
* \param Exclusive
*   Optional, if you set this to 1 the spawner will be disabled until the player who last spawned
*   at it dies. This is useful for creating single-instance special units or restricting the number
*   of players in an area
*/
class dp88_DynamicSpawner_Spawner : ScriptImpClass
{};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Dynamic Spawner - Vehicle Spawner
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_dynamicSpawner
*
* A spawner for use in the \ref scripts_dynamicSpawn "Dynamic Spawners" set of scripts, this spawner
* allows the player to spawn inside a vehicle which will be created for them at the location of this
* spawner.
*
* You can optionally have the vehicle attached to a cinematic upon creation, allowing for a delivery
* animation or other special features. The vehicle will be immune to damage until the cinematic sends
* a custom to enable damage events. The vehicle will be attached to the cinematic in slot 0.
*
* \param Weighting
*   The weighting for this spawner in the controller, higher numbers will favour this spawner over
*   other available spawners. This should be a value between 1 and 10
* \param Team
*   The 0 based index of the team which can use this spawner
* \param Vehicle_Preset
*   Specifies the vehicle preset to spawn for the player. You can use a twiddler here to randomise
*   the vehicle the player spawns with.
* \param Infantry_Preset
*   Optional, specifies an infantry preset to spawn the player. You can use a twiddler here to
*   randomise the infantry unit the player spawns with.
* \param Exclusive
*   Optional, if you set this to 1 the spawner will be disabled until the player who last spawned
*   at it dies. This is useful for creating single-instance special units or restricting the number
*   of players in an area
* \param Cinematic
*   Optional, specifies the name of a Cinematic which will be used to deliver the vehicle
*
* \warning
*   When using a cinematic you must ensure it sends the custom message 43000 to the vehicle at an
*   appropriate time to allow it to start taking damage. If you do not send this custom the vehicle
*   will permanently be immune to all damage. 
*/
class dp88_DynamicSpawner_VehicleSpawner : dp88_DynamicSpawner_Spawner
{};
