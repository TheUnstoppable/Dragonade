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
#ifndef SCRIPTS_INCLUDE__ENGINE_DMG_H
#define SCRIPTS_INCLUDE__ENGINE_DMG_H
void SCRIPTS_API Damage_All_Buildings_By_Team(int Team,float Damage,const char *Warhead,GameObject *Damager); //damages all buildings of the specified team,the Damager is the object that will be passed to the Damaged callback for the buildings and also the one that will get the points,0 = Nod,1 = GDI,2 = both
void SCRIPTS_API Repair_All_Buildings_By_Team(int Team,int ConstructionYardID,float Health); //repairs all buildings for the team except the passed in ConstructionYardID,0 = Nod,1 = GDI
void SCRIPTS_API Repair_All_Static_Vehicles_By_Team(int Team,int type); //sends a custom to all vechicles of the team with the DecorationPhys physics type (i.e. all base defense vechicles etc),0 = Nod,1 = GDI,Message is the message to send. Use with JFW_Repair_On_Custom on the vechicles you want repaired to do the actual repairing
void SCRIPTS_API Set_Max_Health(GameObject *obj,float health); //set the max health of obj
void SCRIPTS_API Set_Max_Health_Without_Healing(GameObject *obj,float health); //set the max health of obj without setting the actual health to maximum
void SCRIPTS_API Set_Max_Shield_Strength(GameObject *obj,float shieldstrength); //set the max shield strength of obj
void SCRIPTS_API Set_Max_Shield_Strength_Without_Healing(GameObject *obj,float shieldstrength); //set the max shield strength of obj without setting the actual health to maximum
const char SCRIPTS_API *Get_Shield_Type(GameObject *obj); //gets the shield type of obj
const char SCRIPTS_API *Get_Skin(GameObject *obj); //gets the skin type of obj
void SCRIPTS_API Set_Skin(GameObject *obj,const char *Skintype); //sets the skin type of obj
void SCRIPTS_API Damage_All_Vehicles_Area(float Damage,const char *Warhead,const Vector3 &Position,float Distance,GameObject *Host,GameObject *Damager); //damages all vechicles in a given area except the host
void SCRIPTS_API Damage_All_Objects_Area(float Damage,const char *Warhead,const Vector3 &Position,float Distance,GameObject *Host,GameObject *Damager); //damages all objects in a given area except the host
void SCRIPTS_API Damage_All_Objects_Area_By_Team(float Damage,const char *Warhead,const Vector3 &Position,float Distance,GameObject *Damager,bool Soldiers,bool Vehicles,int Team); //damage all smart objects in a given area by team
//these only do damage if the building is close enough
void SCRIPTS_API Ranged_Damage_To_Buildings(float Damage,const char *Warhead,Vector3 Location,float DamageRadius,GameObject *Damager); //damage buildings
void SCRIPTS_API Ranged_Scale_Damage_To_Buildings(float Damage,const char *Warhead,Vector3 Location,float DamageRadius,GameObject *Damager); //damage buildings based on a scale factor
void SCRIPTS_API Ranged_Percentage_Damage_To_Buildings(float Percentage,const char *Warhead,Vector3 Location,float DamageRadius,GameObject *Damager); //damage buildings based on a percentage factor
void SCRIPTS_API Ranged_Damage_To_Buildings_Team(int Team,float Damage,const char *Warhead,Vector3 Location,float DamageRadius,GameObject *Damager); //damage buildings per team
void SCRIPTS_API Ranged_Scale_Damage_To_Buildings_Team(int Team,float Damage,const char *Warhead,Vector3 Location,float DamageRadius,GameObject *Damager); //damage buildings per team based on a scale factor
void SCRIPTS_API Ranged_Percentage_Damage_To_Buildings_Team(int Team,float Percentage,const char *Warhead,Vector3 Location,float DamageRadius,GameObject *Damager); //damage buildings per team based on a percentage factor
void SCRIPTS_API Damage_Objects_Half(); //damage all objects by half
void SCRIPTS_API Ranged_Percentage_Damage_To_Vehicles(float Percentage,const char *Warhead,Vector3 Location,float DamageRadius,GameObject *Damager); //damage vehicles based on a percentage factor
void SCRIPTS_API Set_Damage_Points(GameObject *obj,float points); //Set the damage points for an object
void SCRIPTS_API Set_Death_Points(GameObject *obj,float points); //Set the death points for an object
void SCRIPTS_API Repair_All_Buildings_By_Team_Radius(int Team,int ConstructionYardID,float Health,float Radius); //repairs all buildings in the specified radius around the object represented by ConstructionYardID for the team except the passed in ConstructionYardID,0 = Nod,1 = GDI
void SCRIPTS_API Repair_All_Turrets_By_Team(int team,float health); //repairs all vehicles that have mode = turret
void SCRIPTS_API Damage_All_Objects_In_Zone(GameObject *Zone,float Damage,const char *Warhead,GameObject *Damager); //Damages all objects in this zone
float SCRIPTS_API Get_Damage_Points(GameObject *obj); //get the damage points for an object
float SCRIPTS_API Get_Death_Points(GameObject *obj); //get the death points for an object
void SCRIPTS_API Kill_Occupants(GameObject *obj); //kill all the occupants of a vehicle
void SCRIPTS_API Damage_Occupants(GameObject *obj,float Damage,const char *Warhead); //Damage all the occupants of a vehicle
void SCRIPTS_API Ranged_Variable_Percent_Vehicle_Damage(float EnemyPercentage, float FriendPecentage, bool ForceFriendly, const char *Warhead,Vector3 Location,float DamageRadius,GameObject *Damager); //damage vehicles based on a percentage factor

/*!
* \brief Get maximum hitpoints
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* Returns the maximum hitpoints for the specified object, rather than having to manually add up the
* maximum health and shield values
*
* \param[in] obj
*   Object to get the maximum hitpoints of
* \returns
*   The combined maximum health and shield strength of the object
*/
float SCRIPTS_API Get_Max_Hitpoints ( GameObject* obj );

/*!
* \brief Get current hitpoints
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* Returns the current hitpoints for the specified object, rather than having to manually add up the
* current health and shield values
*
* \param[in] obj
*   Object to get the maximum hitpoints of
* \returns
*   The combined current health and shield strength of the object
*/
float SCRIPTS_API Get_Hitpoints ( GameObject* obj );

/*!
* \brief Check armour type exists
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* Checks if the named armor type exists in armor.ini. This improves on the Get_Armor_Type function
* in ArmorWarheadManager by removing the ambiguity between 0 == not found and 0 == first armor type.
*/
bool SCRIPTS_API Is_Valid_Armor_Type(const char* name);

/*!
* \brief Check armour type exists
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* Checks if the named warhead exists in armor.ini. This improves on the Get_Warhead_Type function
* in ArmorWarheadManager by removing the ambiguity between 0 == not found and 0 == first warhead.
*/
bool SCRIPTS_API Is_Valid_Warhead_Type(const char* name);

#endif
