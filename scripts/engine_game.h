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
#ifndef SCRIPTS_INCLUDE__ENGINE_GAME_H
#define SCRIPTS_INCLUDE__ENGINE_GAME_H
#include "GameData.h"

/*!
* \ingroup api_teamcontrol
* Enable or disable base power for a team, this correctly handles all the relavent stuff including base defences and purchase time/cost changes.
*
* \param[in] team
*   ID of the team to set power state for
*
* \param[in] powered
*   whether to enable or disable power
*
*/
void SCRIPTS_API Power_Base(int team,bool powered);

/*!
* \ingroup api_teamcontrol
* Enable or disable advanced infantry purchasability for a team
*
* \param[in] team
*   ID of the team to set advanced infantry purchasability for
*
* \param[in] cangenerate
*   whether to enable or disable generation
*
*/
void SCRIPTS_API Set_Can_Generate_Soldiers(int team,bool cangenerate);

/*!
* \ingroup api_teamcontrol
* Enable or disable vehicle purchasability for a team
*
* \param[in] team
*   ID of the team to set vehicle purchasability for
*
* \param[in] cangenerate
*   whether to enable or disable generation
*
*/
void SCRIPTS_API Set_Can_Generate_Vehicles(int team,bool cangenerate);

/*!
* \ingroup api_teamcontrol
* Destroy the base of a team
*
* \param[in] team
*   ID of the team to destroy the base of
*
*/
void SCRIPTS_API Destroy_Base(int team);

/*!
* \ingroup api_teamcontrol
* Set or clear the "beacon destroyed base" flag for a team
*
* \param[in] team
*   ID of the team to change the "beacon destroyed base" flag on
*
* \param[in] destroyed
*   whether to enable or disable the "beacon destroyed base" flag
*
*/
void SCRIPTS_API Beacon_Destroyed_Base(int team, bool destroyed);

/*!
* \ingroup api_teamcontrol
* Enable or disable radar for a team
*
* \param[in] team
*   ID of the team to set radar for
*
* \param[in] enable
*   whether to enable or disable radar
*
*/
void SCRIPTS_API Enable_Base_Radar(int team, bool enable);

/*!
* Check if an object is an AI harvester
*
* \param[in] obj
*   object to check
*
* \return
*   True if the object is an AI harvester, false otherwise
*/
bool SCRIPTS_API Is_Harvester(GameObject *obj);

/*!
* \ingroup api_teamcontrol
* Check if a teams base radar is enabled
*
* \param[in] team
*   ID of the team to check the radar of
*
* \return
*   True if the specified teams radar is enabled, false otherwise
*/
bool SCRIPTS_API Is_Radar_Enabled(int team);

/*!
* Get the building type for a building
*
* \param[in] obj
*   object to get type of
*
* \return
*   building type of the building
*/
int SCRIPTS_API Building_Type(GameObject *obj);

/*!
* Check if a building is dead
*
* \param[in] obj
*   object to check
*
* \return
*   True if the building is dead, false otherwise
*/
bool SCRIPTS_API Is_Building_Dead(GameObject *obj);

/*!
* \ingroup api_objects_find
* Find the first building of a given building type for this team
*
* \param[in] team
*   ID of the team to find the building for
*
* \param[in] type
*   type to find
*
* \return
*   First building for this team of this type (if any)
*/
GameObject SCRIPTS_API *Find_Building_By_Type(int team,int type);

/*!
* \ingroup api_objects_find
* Find the first base defense building for this team (if any)
*
* \param[in] Team
*   ID of the team to find the base defense building for
*
* \return
*   First base defense building for this team (if any)
*/
GameObject SCRIPTS_API *Find_Base_Defense(int team);

/*!
* Check if the current map is a flying map
*
* \return
*   True if the map is a flying time map
*/
bool SCRIPTS_API Is_Map_Flying();

GameObject SCRIPTS_API *Create_Building(const char *preset,const Vector3 & Position); //Create a building controller

/*!
* \ingroup api_objects_find
* Find the AI harvester for a team (if any)
*
* \param[in] team
*   ID of the team to find the harvester for
*
* \return
*   AI harvester object
*/
GameObject SCRIPTS_API *Find_Harvester(int team);

/*!
* \ingroup api_teamcontrol
* Check if a teams base is powered
*
* \param[in] team
*   ID of the team to check the base power of
*
* \return
*   True if the specified teams base is powered, false otherwise
*/
bool SCRIPTS_API Is_Base_Powered(int team);

/*!
* \ingroup api_teamcontrol
* Check if a team can currently purchase vehicles
*
* \param[in] team
*   ID of the team to check vehicle purchasability for
*
* \return
*   True if the specified team can purchase vehicles, false otherwise
*/
bool SCRIPTS_API Can_Generate_Vehicles(int team);

/*!
* \ingroup api_teamcontrol
* Check if a team can currently purchase advanced infantry characters
*
* \param[in] team
*   ID of the team to check advanced infantry purchasability for
*
* \return
*   True if the specified team can purchase advanced infantry characters, false otherwise
*/
bool SCRIPTS_API Can_Generate_Soldiers(int team);

/*!
* Get the current cGameData
*
* \return
*   current cGameData
*/
cGameData SCRIPTS_API *The_Game();

/*!
* Get the current cGameDataSkirmish, if any
*
* \return
*   current cGameDataSirmish, if any
*/
cGameDataSkirmish SCRIPTS_API *The_Skirmish_Game();

/*!
* Get the current cGameDataCnc, if any
*
* \return
*   current cGameDataCnc, if any
*/
cGameDataCnc SCRIPTS_API *The_Cnc_Game();

/*!
* Get the current cGameDataSinglePlayer, if any
*
* \return
*   current cGameDataSinglePlayer, if any
*/
cGameDataSinglePlayer SCRIPTS_API *The_Single_Player_Game();

/*!
* \ingroup api_teamcontrol
* Get the count of how many buildings a given team has
*
* \param[in] Team
*   ID of the team to count the buildings of
*
* \return
*   count of the buildings for that team
*/
int SCRIPTS_API Get_Building_Count_Team(int Team);

/*!
* \ingroup api_objects_find
* Find the first building for this team
*
* \param[in] Team
*   ID of the team to find the building for
*
* \return
*   First building for this team
*/
GameObject SCRIPTS_API *Find_Building_By_Team(int Team);

/*!
* \ingroup api_objects_find
* Find the first building of a given preset type for this team
*
* \param[in] Team
*   ID of the team to find the building for
*
* \param[in] Preset_Name
*   preset name to find
*
* \return
*   First building for this team of this preset (if any)
*/
GameObject SCRIPTS_API *Find_Building_By_Preset(int Team,const char *Preset_Name);

/*!
* \ingroup api_objects_find
* Find the first power plant for this team (if any)
*
* \param[in] Team
*   ID of the team to find the power plant for
*
* \return
*   First power plant for this team (if any)
*/
GameObject SCRIPTS_API *Find_Power_Plant(int Team);

/*!
* \ingroup api_objects_find
* Find the first construction yard for this team (if any)
*
* \param[in] Team
*   ID of the team to find the construction yard for
*
* \return
*   First construction yard for this team (if any)
*/
GameObject SCRIPTS_API *Find_Construction_Yard(int Team);

/*!
* \ingroup api_objects_find
* Find the first refinery for this team (if any)
*
* \param[in] Team
*   ID of the team to find the refinery for
*
* \return
*   First refinery for this team (if any)
*/
GameObject SCRIPTS_API *Find_Refinery(int Team);

/*!
* \ingroup api_objects_find
* Find the first repair bay for this team (if any)
*
* \param[in] Team
*   ID of the team to find the repair bay for
*
* \return
*   First repair bay for this team (if any)
*/
GameObject SCRIPTS_API *Find_Repair_Bay(int Team);

/*!
* \ingroup api_objects_find
* Find the first soldier factory for this team (if any)
*
* \param[in] Team
*   ID of the team to find the soldier factory for
*
* \return
*   First soldier factory for this team (if any)
*/
GameObject SCRIPTS_API *Find_Soldier_Factory(int Team);

/*!
* \ingroup api_objects_find
* Find the first vehicle factory for this team (if any)
*
* \param[in] Team
*   ID of the team to find the vehicle factory for
*
* \return
*   First vehicle factory for this team (if any)
*/
GameObject SCRIPTS_API *Find_Vehicle_Factory(int Team);

/*!
* \ingroup api_objects_find
* Find the first com center for this team (if any)
*
* \param[in] Team
*   ID of the team to find the com center for
*
* \return
*   First com center for this team (if any)
*/
GameObject SCRIPTS_API *Find_Com_Center(int Team);

/*!
* \ingroup api_objects_find
* Find the first special building for this team (if any)
*
* \param[in] Team
*   ID of the team to find the special building for
*
* \return
*   First special building for this team (if any)
*/
GameObject SCRIPTS_API *Find_Special(int Team);

/*!
* \ingroup api_objects_find
* Find the first naval factory for this team (if any)
*
* \param[in] Team
*   ID of the team to find the naval factory for
*
* \return
*   First naval factory for this team (if any)
*/
GameObject SCRIPTS_API *Find_Naval_Factory(int Team);

/*!
* \ingroup api_objects_find
* Find the first tech center for this team (if any)
*
* \param[in] Team
*   ID of the team to find the tech center for
*
* \return
*   First tech center for this team (if any)
*/
GameObject SCRIPTS_API *Find_Tech_Center(int Team);

/*!
* \ingroup api_objects_find
* Find the first helipad for this team (if any)
*
* \param[in] Team
*   ID of the team to find the helipad for
*
* \return
*   First helipad for this team (if any)
*/
GameObject SCRIPTS_API *Find_Helipad(int Team);

/*!
* \ingroup api_objects_find
* Find the first shrine for this team (if any)
*
* \param[in] Team
*   ID of the team to find the shrine for
*
* \return
*   First shrine for this team (if any)
*/
GameObject SCRIPTS_API *Find_Shrine(int Team);

void SCRIPTS_API Restore_Building(GameObject* obj); //restores a building to life

/*!
* Check if gameplay is permitted
*
* \return
*   True if gameplay is permitted
*/
bool SCRIPTS_API Is_Gameplay_Permitted();

/*!
* Check if running on the FDS
*
* \return
*   True if running on the FDS
*/
bool SCRIPTS_API Is_Dedicated();

/*!
* Get current game mode, 0 = error, 1 = single player, 2 = skirmish, 3 = WOL, 4 = GameSpy, 5 = LAN.
* May not always differentiate correctly between WOL, Gamespy and LAN, especially when running as a non-dedicated server.
*
* \return
*   Current game mode
*/
unsigned int SCRIPTS_API Get_Current_Game_Mode();

void SCRIPTS_API Get_Private_Message_Color(unsigned int *red, unsigned int *green, unsigned int *blue); //get color for private messages
void SCRIPTS_API Get_Public_Message_Color(unsigned int *red, unsigned int *green, unsigned int *blue); //get color for public messages

/*!
* Get the AI harvester preset ID of a specific team
*
* \param[in] team
*   ID of the team to get the harvester preset ID of
*
* \return
*   ID of the AI harvester preset for this team
*/
int SCRIPTS_API Get_Harvester_Preset_ID(int Team);

/*!
* Check if a preset matches the AI harvester preset set in the refinery
*
* \param[in] preset
*   ID of the preset to check
*
* \return
*   True if the preset matches the AI harvester preset set in the refinery
*/
bool SCRIPTS_API Is_Harvester_Preset(GameObject *obj);

/*!
* Check if a preset is an air unit
*
* \param[in] preset
*   ID of the preset to check
*
* \return
*   True if the preset is an air unit (i.e. will be built at the air factory when its built)
*/
bool SCRIPTS_API Vehicle_Preset_Is_Air(int preset);

/*!
* Check if a preset is a naval unit
*
* \param[in] preset
*   ID of the preset to check
*
* \return
*   True if the preset is a naval unit (i.e. will be built at the naval factory when its built)
*/
bool SCRIPTS_API Vehicle_Preset_Is_Naval(int preset);

/*!
* \ingroup api_teamcontrol
* Find the center point of a base
*
* \param[in] team
*   ID of the team to use
*
* \return
*   Center of that teams base
*/
Vector3 SCRIPTS_API Get_Base_Center(int Team);

void SCRIPTS_API Seconds_To_Hms(float secs, int &hours, int &minutes, int &seconds);
#endif
