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
#ifndef SCRIPTS_INCLUDE__ENGINE_OBJ_H
#define SCRIPTS_INCLUDE__ENGINE_OBJ_H
#include "scripts.h"
#include "engine_vector.h"
#include "Matrix3D.h"

//type/class ID values for various things
#define CID_Beacon 0x3016
#define CID_Building 0xD001 //only covers generic buildings,not the special stuff like PowerPlant or Refinery
#define CID_C4 0x3006
#define CID_ConstructionYard 0xD00A
#define CID_PowerPlant 0xD003
#define CID_PowerUp 0x3003
#define CID_Refinery 0xD002
#define CID_RepairBay 0xD009
#define CID_SamSite 0x3007
#define CID_ScriptZone 0x300E
#define CID_Simple 0x3004
#define CID_Soldier 0x3001
#define CID_SoldierFactory 0xD004
#define CID_SpecialEffects 0x3013
#define CID_Vehicle 0x3010
#define CID_VehicleFactory 0xD005 //for WarFactory or Airstrip,use the relavent values
#define CID_WarFactory 0xD007
#define CID_Airstrip 0xD006
#define CID_Cinematic 0x3011
#define CID_ComCenter 0xD008
#define CID_Weapon 0xB001
#define CID_Ammo 0xB002
#define CID_Explosion 0xB003
#define CID_Spawner 0x300D

int SCRIPTS_API Get_Object_Type(GameObject *obj); //get the team of an object,0 = Nod,1 = GDI,2 = neutral,works on buildings
void SCRIPTS_API Set_Object_Type(GameObject *obj,int type); //set the team of an object,0 = Nod,1 = GDI,2 = neutral,works on buildings
void SCRIPTS_API Get_Object_Color(GameObject *obj, unsigned int *red, unsigned int *green, unsigned int *blue); //get color for a player
GameObject SCRIPTS_API *Find_Smart_Object_By_Team(int Team); //will find the first SmartGameObj (i.e. soldier or vechicle) for the team,0 = Nod,1 = GDI
GameObject SCRIPTS_API *Find_Object_By_Team(int Team); //will find the first object for the team,0 = Nod,1 = GDI
GameObject SCRIPTS_API *Find_Non_Player_Object_By_Team(int Team); //will find the first non player object for the team,0 = Nod,1 = GDI
GameObject SCRIPTS_API *Find_Object_By_Preset(int Team,const char *Preset_Name); //will find the first object of team matching preset,0 = Nod,1 = GDI,2 = either
GameObject SCRIPTS_API *Find_Closest_Non_Building_Object_By_Team(int Team,Vector3 position); //find the closest non building object to position
GameObject SCRIPTS_API *Find_Closest_Preset_By_Team(int Team,const Vector3 &pos,const char *Preset); //Find the closest object of this preset to this position
GameObject SCRIPTS_API *Find_Random_Preset_By_Team(int Team,const char *Preset); //Find a random object on this team with this preset
int SCRIPTS_API Get_Object_Count(int Team,const char *Preset); //Get the number of objects that exist with this preset
SimpleDynVecClass<GameObject*> SCRIPTS_API *Get_All_Objects_By_Preset(int Team,const char *Preset); //Returns an array of all objects of this preset
GameObject SCRIPTS_API *Find_Nearest_Preset(Vector3 position, const char* preset); //find the nearest preset
GameObject SCRIPTS_API *Find_Random_Preset(const char* preset, int min, int max); //find a random preset
void SCRIPTS_API Send_Custom_To_Team_Buildings(int Team,GameObject *sender,int type,int param,float delay); //send custom to team buildings
void SCRIPTS_API Send_Custom_To_Team_Preset(int Team,const char *PresetName,GameObject *sender,int type,int param,float delay); //send custom to team preset
void SCRIPTS_API Send_Custom_All_Objects_Area(int type,const Vector3 &Position,float Distance,GameObject *sender,int team); //sends a custom to all objects in a given area
void SCRIPTS_API Send_Custom_All_Objects(int type,GameObject *sender,int team); //sends a custom to all objects
void SCRIPTS_API Send_Custom_Event_To_Object(GameObject *sender,const char *Preset,int type,int param,float delay); //this is like Send_Custom_Event except it sends to all objects of the passed in preset
bool SCRIPTS_API Is_Unit_In_Range(const char *preset,float range,Vector3 location,int team,bool empty); //Is the given unit type in range of a location
bool SCRIPTS_API Get_Is_Powerup_Persistant(GameObject *obj); //Returns if this powerup is persistant
bool SCRIPTS_API Get_Powerup_Always_Allow_Grant(GameObject *obj); //Returns if this powerup is set to always allows grant
void SCRIPTS_API Set_Powerup_Always_Allow_Grant(GameObject *obj,bool Grant); //Change if this powerup is set to always allows grant
int SCRIPTS_API Get_Powerup_Grant_Sound(GameObject *obj); //Returns the sound that is played when this powerup is picked up
void SCRIPTS_API Grant_Powerup(GameObject *obj,const char *Preset_Name); //grants a powerup
GameObject SCRIPTS_API *Find_Closest_Building(const Vector3& position); // Find the closest building
Vector3 SCRIPTS_API Get_Velocity( GameObject* obj ); // Get the current velocity of a PhysicalGameObject
void SCRIPTS_API Set_Velocity( GameObject* obj, Vector3 velocity ); // Set the velocity of a PhysicalGameObject
Matrix3D SCRIPTS_API Get_Transform( GameObject* obj ); // Get the current transform of a PhysicalGameObject
void SCRIPTS_API Set_Transform( GameObject* obj, Matrix3D transform ); // Set the transform of a PhysicalGameObject
GameObject SCRIPTS_API * Get_Closest_Armed_Object_To_Object( GameObject* obj, int team = 2 ); // Get the closest armed (vehicle or infantry) object to the given object, optionally filtered by team (0=Nod,1=GDI,2=any)

SCRIPTS_API bool Verify_Preset_Limit(int team, int instance_count, uint32* preset_ids, int preset_id_count, GameObject* ignore = nullptr); // returns true if limit is reached
#endif
