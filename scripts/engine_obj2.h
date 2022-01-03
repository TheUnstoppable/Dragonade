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
#ifndef SCRIPTS_INCLUDE__ENGINE_OBJ2_H
#define SCRIPTS_INCLUDE__ENGINE_OBJ2_H
#include "scripts.h"
class OBBoxClass;
GameObject SCRIPTS_API *Get_Vehicle(GameObject *obj); //gets the vechicle that obj is driving
void SCRIPTS_API Grant_Refill(GameObject *obj); //triggers the same code as the refill button on the PT
bool SCRIPTS_API Change_Character(GameObject *obj,const char *Preset_Name); //will change the character of the passed in object to the passed in preset
bool SCRIPTS_API Create_Vehicle(const char *Preset_Name,float Delay,GameObject *Owner,int Team); //creates a vechicle,dont know what Delay is for,Owner is for the owner of the vechicle,Team says which sides factories to create it at
void SCRIPTS_API Toggle_Fly_Mode(GameObject *obj); //makes a soldier fly if they arent flying or not fly if they are
int SCRIPTS_API Get_Vehicle_Occupant_Count(GameObject *obj); //gets the count of occupants in a vehicle
//these next 3 may return NULL (e.g. if no-one is in the seat or if its not a vehicle)
GameObject SCRIPTS_API *Get_Vehicle_Occupant(GameObject *obj,int seat); //gets the occupant in the given seat of the vehicle
GameObject SCRIPTS_API *Get_Vehicle_Driver(GameObject *obj); //gets the driver of a vehicle
GameObject SCRIPTS_API *Get_Vehicle_Gunner(GameObject *obj); //gets the gunner of a vehicle,if there is only one person this should be the driver as well as the gunner
void SCRIPTS_API Force_Occupant_Exit(GameObject *obj,int seat); //kick the object in <seat> out of the vehicle,if anyone is in that seat
void SCRIPTS_API Force_Occupants_Exit(GameObject *obj); //kick all occupants out of the vehicle
GameObject SCRIPTS_API *Get_Vehicle_Return(GameObject *obj); //like Get_Vehicle but will return the soldier if its not inside a vehicle instead of NULL
bool SCRIPTS_API Is_Stealth(GameObject *obj); //is this object stealth
bool SCRIPTS_API Get_Fly_Mode(GameObject *obj); //is this infantry flying via Set_Fly_Mode
int SCRIPTS_API Get_Vehicle_Seat_Count(GameObject *obj); //get the seat count for a vehicle
void SCRIPTS_API Soldier_Transition_Vehicle(GameObject *obj); //makes the soldier exit the vehicle they are in (or if right next to a vehicle, get in)
unsigned int SCRIPTS_API Get_Vehicle_Mode(GameObject *obj); //Gets the mode of a vehicle
bool SCRIPTS_API Is_VTOL(GameObject *obj); //is this something that should be targeted as a VTOL
GameObject SCRIPTS_API *Get_Vehicle_Owner(GameObject *obj); //Gets the owner (if any) of a vehicle. Will not work if used within the first second after the ::Created event is called
void SCRIPTS_API Force_Occupants_Exit_Team(GameObject *obj,int team); //kick all occupants not of a given team out of the vehicle
unsigned int SCRIPTS_API Get_Vehicle_Definition_Mode(const char *preset); //get the mode of a vehicle given its preset name
GameObject SCRIPTS_API *Find_Closest_Zone(Vector3 &Location,unsigned int Type); //Find the closest zone
bool SCRIPTS_API IsInsideZone(GameObject *zone,GameObject *obj); //is <solder/vehicle> inside <zone>. Will now work if object is inside a zone when its created (e.g. spawns inside zone or zone is moved around them with Create_Zone or Set_Zone_Box)
unsigned int SCRIPTS_API Get_Vehicle_Definition_Mode_By_ID(unsigned long ID); //Get the mode of a vehicle given its preset ID
unsigned int SCRIPTS_API Get_Zone_Type(GameObject *obj); //Get the type of a script zone
OBBoxClass SCRIPTS_API *Get_Zone_Box(GameObject *obj); //Get the box (size/position) of a ScriptZoneGameObj
void SCRIPTS_API Set_Zone_Box(GameObject *obj,OBBoxClass &box); //Set the box (size/position) of a ScriptZoneGameObj
GameObject SCRIPTS_API *Create_Zone(const char *preset,OBBoxClass &box); //Create a script zone and set its box
bool SCRIPTS_API Is_Available_For_Purchase(GameObject *factory); //Is it possible to purchase a vehicle from this factory
GameObject SCRIPTS_API *Get_Vehicle_Gunner_Pos(GameObject *obj); //Get the vehicle gunner, returns zero if there is no gunner
void SCRIPTS_API Set_Vehicle_Gunner(GameObject *obj,int seat); //set gunner for this vehicle
bool SCRIPTS_API Is_Spy(GameObject *obj); //Is this soldier object a spy
SCRIPTS_API int Get_Player_Count_In_Zone(GameObject *obj,int Team); //Get the player count in a zone
SCRIPTS_API int Get_Object_Count_In_Zone(GameObject *obj,int Team); //Get the object count in a zone
SCRIPTS_API bool Is_Stealth_Enabled(GameObject *obj); //Is stealth enabled
int SCRIPTS_API Get_Occupant_Seat(GameObject *vehicle,GameObject *occupant); //returns the seat that the occupant is sitting in or -1 if they aren't in the vehicle
bool SCRIPTS_API Is_Unsquishable(GameObject *obj); //Is this soldier unsquishable

/*!
* \brief Create an object and attach it to a bone
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup api_objects
*
* Creates a new instance of the named preset and attaches it to a specific bone on the specified
* object, or the origin of the object if no bone is specified.
*
* \param[in] host
*   The host object to which the new object should be attached
* \param[in] preset
*   The name of the preset to create an instance of
* \param[in] bone
*   (Optional) The name of the bone on the host object to which the object should be attached
*/
SCRIPTS_API GameObject* Create_Object_Attach_To_Object(GameObject* host, const char* preset, const char* bone = NULL);
#endif
