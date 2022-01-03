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
#ifndef SCRIPTS_INCLUDE__ENGINE_PLAYER_H
#define SCRIPTS_INCLUDE__ENGINE_PLAYER_H

#include "scripts.h"

class cPlayer;
class WideStringClass;

GameObject SCRIPTS_API *Get_GameObj(int PlayerID); //convert a player ID into a GameObject
long SCRIPTS_API Get_Player_ID(GameObject *obj); //convert a GameObject into a player ID
const char SCRIPTS_API *Get_Player_Name(GameObject *obj); //converts a GameObject into a player name
const char SCRIPTS_API *Get_Player_Name_By_ID(int PlayerID); //gets the player name from a player ID. Will return NULL if that player doesnt exist.
void SCRIPTS_API Change_Team(GameObject *obj,int Team); //changes the team of a player given their GameObject and also kills the player so they respwan,passing anything other than 0 = Nod,1 = GDI will crash
void SCRIPTS_API Change_Team_By_ID(int PlayerID,int Team); //changes the team of a player given their ID and also kills the player so they respwan,passing anything other than 0 = Nod,1 = GDI will crash
int SCRIPTS_API Get_Player_Count(); //gets the count of how many players there are
int SCRIPTS_API Get_Team_Player_Count(int Team); //gets the count of players for a given team,0 = Nod,1 = GDI)
int SCRIPTS_API Get_Team(int PlayerID); //get the team of a player
int SCRIPTS_API Get_Rank(int PlayerID); //get the rank of a player
int SCRIPTS_API Get_Kills(int PlayerID); //get the kills of a player
int SCRIPTS_API Get_Deaths(int PlayerID); //get the deaths of a player
float SCRIPTS_API Get_Score(int PlayerID); //get the score of a player
float SCRIPTS_API Get_Money(int PlayerID); //get the money of a player
float SCRIPTS_API Get_Kill_To_Death_Ratio(int PlayerID); //get the kill/death ratio of a player
GameObject SCRIPTS_API *Get_Part_Name(const char *name1); //Will return the player with this string as part of their name if there is exactly one player with it
int SCRIPTS_API Get_Part_Names(const char *name1); //Will return the count of players with this string as part of their name
void SCRIPTS_API Get_Team_Color(unsigned int team, unsigned int *red, unsigned int *green, unsigned int *blue); //get color for a team
void SCRIPTS_API Get_Player_Color(int PlayerID, unsigned int *red, unsigned int *green, unsigned int *blue); //get color for a player
bool SCRIPTS_API Purchase_Item(GameObject *obj,int cost); //Pass a soldier object & a cost to deduct that much money from the soldier. Returns true if the transaction suceeded, false otherwise
void SCRIPTS_API Set_Ladder_Points(int PlayerID,int amount); //Set the ladder points of a player
void SCRIPTS_API Set_Rung(int PlayerID,int amount); //Set the rung of a player
void SCRIPTS_API Set_Money(int PlayerID,float amount); //Set the money of a player
void SCRIPTS_API Set_Score(int PlayerID,float amount); //Set the score of a player
GameObject SCRIPTS_API *Find_First_Player(int Team); //finds the first player of Team,0 = Nod,1 = GDI,2 = either
bool SCRIPTS_API Change_Player_Team(GameObject *obj,bool Retain_Score,bool Retain_Money,bool Show_Host_Message); //changes the players team
int SCRIPTS_API Tally_Team_Size(int team); //gets the team size for a team
float SCRIPTS_API Get_Team_Score(int team); //gets the score for a team (same as Game_Info uses)
void SCRIPTS_API Change_Team_2(GameObject *obj,int Team); //changes the team of a player given their GameObject without killing the player,passing anything other than 0 = Nod,1 = GDI will crash
int SCRIPTS_API Get_Player_Type(GameObject *obj); //Get the player type of a player from the cPlayer
const wchar_t SCRIPTS_API *Get_Wide_Player_Name(GameObject *obj); //Converts a GameObject into a player name
Vector3 SCRIPTS_API Get_Color_For_Team(int teamId); //Get the color values for a team
SCRIPTS_API int Get_Player_Team(int PlayerID); //Get the team type of a player
SCRIPTS_API cPlayer *Find_Player(int playerId); //Find a player by ID
SCRIPTS_API cPlayer *Find_Player(const WideStringClass& playerName); //find a player by name
SCRIPTS_API int Get_Player_ID_By_Name(const char *Name); //Get the ID of a player given a name
SCRIPTS_API bool Is_Active(int ID); //Is a player active
SCRIPTS_API void Set_Kills(int ID,int kills); //Set kills for a player
SCRIPTS_API void Set_Deaths(int ID,int deaths); //Set deaths for a player
SCRIPTS_API void Set_Team_Score(int ID,float score); //Set score for a team
SCRIPTS_API void ChangeTeamDeaths(int teamId,int amount);
#endif
