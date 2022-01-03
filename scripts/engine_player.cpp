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

#include "scripts.h"
#include "SoldierGameObj.h"
#include "cPlayer.h"
#include "engine_player.h"
#include "slist.h"
#include "GameObjManager.h"
#include "engine_obj.h"
#include "engine_weap.h"
#include "engine_game.h"
#include "engine_tdb.h"
#include "SysTimeClass.h"
#include "cTeam.h"
#include "engine_tt.h"
#include "SCAnnouncement.h"
#include "CombatManager.h" //DA

RENEGADE_FUNCTION
int SCRIPTS_API Get_Player_Count()
AT2(0x00417040,0x00417040);
RENEGADE_FUNCTION
void cPlayer::Set_Ladder_Points(int amount)
AT2(0x0040FC90,0x0040FC90);
RENEGADE_FUNCTION
void cPlayer::Set_Rung(int amount)
AT2(0x00410790,0x00410790);
RENEGADE_FUNCTION
Vector3 cPlayer::Get_Color() const
AT2(0x0040E530,0x0040E530);
RENEGADE_FUNCTION
void cPlayer::Set_Wol_Rank(int rank)
AT2(0x004104D0,0x004104D0);
bool cPlayer::Is_Team_Player()
{
	return (unsigned (this->PlayerType) <= 1);
}
RENEGADE_FUNCTION
void cPlayer::Set_Is_Active(bool active)
AT2(0x00413100,0x00413100);

RENEGADE_FUNCTION
cPlayer *Find_Player(int id)
AT2(0x004157E0,0x004157E0);

RENEGADE_FUNCTION
cPlayer *Find_Player(const WideStringClass& playerName)
AT2(0x00415810,0x00415810);

RENEGADE_FUNCTION
cTeam *Find_Team(int team)
AT2(0x0041DA10,0x0041DA10);

RENEGADE_FUNCTION
int TallyTeamSize(int team)
AT2(0x004170D0,0x004170D0);

const char SCRIPTS_API *Get_Player_Name(GameObject *obj)
{
	if (!obj)
	{
		return newstr("None");
	}
	SoldierGameObj *o = obj->As_SoldierGameObj();
	if (!o)
	{
		return newstr("None");
	}
	if (!((cPlayer *)o->Get_Player_Data()) && o->Get_Bot_Tag().Is_Empty())
	{
		return newstr("None");
	}
	if (!o->Get_Bot_Tag().Is_Empty())
	{
		return WideCharToChar(o->Get_Bot_Tag());
	}
	return WideCharToChar(((cPlayer *)o->Get_Player_Data())->Get_Name());
}

const char SCRIPTS_API *Get_Player_Name_By_ID(int PlayerID)
{
	cPlayer *x = Find_Player(PlayerID);
	if (!x)
	{
		return newstr("None");
	}
	return WideCharToChar(x->Get_Name());
}

GameObject SCRIPTS_API *Get_GameObj
	(int clientId)
{
	cPlayer* player = Find_Player (clientId);
	if (!player)
		return 0;

	return player->Get_GameObj();
}

void SCRIPTS_API Change_Team_By_ID(int PlayerID,int Team)
{
	cPlayer *x = Find_Player(PlayerID);
	if (!x)
	{
		return;
	}
	x->Set_Player_Type(Team);
	GameObject *obj = Get_GameObj(PlayerID);
	Commands->Destroy_Object(obj);
}

void SCRIPTS_API Change_Team(GameObject *obj,int Team)
{
	if (!obj)
	{
		return;
	}
	SoldierGameObj *o = obj->As_SoldierGameObj();
	if (!o)
	{
		return;
	}
	if (!((cPlayer *)o->Get_Player_Data()))
	{
		return;
	}
	((cPlayer *)o->Get_Player_Data())->Set_Player_Type(Team);
	Commands->Destroy_Object(obj);
}

void SCRIPTS_API Change_Team_2(GameObject *obj,int Team)
{
	if (!obj)
	{
		return;
	}
	SoldierGameObj *o = obj->As_SoldierGameObj();
	if (!o)
	{
		return;
	}
	if (!((cPlayer *)o->Get_Player_Data()))
	{
		return;
	}
	((cPlayer *)o->Get_Player_Data())->Set_Player_Type(Team);
}

long SCRIPTS_API Get_Player_ID(GameObject *obj)
{
	if (!obj)
	{
		return -1;
	}
	SoldierGameObj *o = obj->As_SoldierGameObj();
	if (!o)
	{
		return -1;
	}
	if (!((cPlayer *)o->Get_Player_Data()))
	{
		return -1;
	}
	return ((cPlayer *)o->Get_Player_Data())->Get_Id();
}



int SCRIPTS_API Get_Team(int PlayerID)
{
	cPlayer *x = Find_Player(PlayerID);
	if (!x)
	{
		return 0;
	}
	return x->Get_Player_Type();
}

int SCRIPTS_API Get_Rank(int PlayerID)
{
	cPlayer *x = Find_Player(PlayerID);
	if (!x)
	{
		return 0;
	}
	return x->Get_Rung();
}

int SCRIPTS_API Get_Kills(int PlayerID)
{
	cPlayer *x = Find_Player(PlayerID);
	if (!x)
	{
		return 0;
	}
	return x->Get_Kills();
}

int SCRIPTS_API Get_Deaths(int PlayerID)
{
	cPlayer *x = Find_Player(PlayerID);
	if (!x)
	{
		return 0;
	}
	return x->Get_Deaths();
}

float SCRIPTS_API Get_Score(int PlayerID)
{
	cPlayer *x = Find_Player(PlayerID);
	if (!x)
	{
		return 0;
	}
	return x->Get_Score();
}

float SCRIPTS_API Get_Money(int PlayerID)
{
	cPlayer *x = Find_Player(PlayerID);
	if (!x)
	{
		return 0;
	}
	return x->Get_Money();
}

float SCRIPTS_API Get_Kill_To_Death_Ratio(int PlayerID)
{
	cPlayer *x = Find_Player(PlayerID);
	if (!x)
	{
		return 0;
	}
	return x->Get_Kill_To_Death_Ratio();
}

void SCRIPTS_API Get_Player_Color(int PlayerID, unsigned int *red, unsigned int *green, unsigned int *blue)
{
	Vector3 v;
	cPlayer *x = Find_Player(PlayerID);
	if (!x)
	{
		return;
	}
	v = x->Get_Color();
	*red = (unsigned int)(v.X*255);
	*green = (unsigned int)(v.Y*255);
	*blue = (unsigned int)(v.Z*255);
}

bool SCRIPTS_API Purchase_Item(GameObject *obj,int cost)
{
	if (!obj)
	{
		return false;
	}
	SoldierGameObj *o = obj->As_SoldierGameObj();
	if (!o)
	{
		return false;
	}
	if (!((cPlayer *)o->Get_Player_Data()))
	{
		return false;
	}
	return ((cPlayer *)o->Get_Player_Data())->Purchase_Item(cost);
}

void SCRIPTS_API Set_Money(int PlayerID,float amount)
{
	cPlayer *x = Find_Player(PlayerID);
	if (!x)
	{
		return;
	}
	x->Set_Money(amount);
}

void SCRIPTS_API Set_Score(int PlayerID,float amount)
{
	cPlayer *x = Find_Player(PlayerID);
	if (!x)
	{
		return;
	}
	x->Set_Score(amount);
}

void SCRIPTS_API Set_Ladder_Points(int PlayerID,int amount)
{
	cPlayer *x = Find_Player(PlayerID);
	if (!x)
	{
		return;
	}
	x->Set_Ladder_Points(amount);
}

void SCRIPTS_API Set_Rung(int PlayerID,int amount)
{
	cPlayer *x = Find_Player(PlayerID);
	if (!x)
	{
		return;
	}
	x->Set_Rung(amount);
}

int SCRIPTS_API Tally_Team_Size(int team)
{
	return TallyTeamSize(team);
}

float SCRIPTS_API Get_Team_Score(int team)
{
	cTeam *teamdata = Find_Team(team);
	if (teamdata)
	{
		return teamdata->Get_Score();
	}
	return 0;
}

void SCRIPTS_API Get_Team_Color(unsigned int team, unsigned int *red, unsigned int *green, unsigned int *blue)
{
	Vector3 v = Get_Color_For_Team(team);
	*red = (unsigned int)(v.X*255);
	*green = (unsigned int)(v.Y*255);
	*blue = (unsigned int)(v.Z*255);
}

GameObject SCRIPTS_API *Find_First_Player(int Team)
{
	SLNode<SoldierGameObj> *x = GameObjManager::StarGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			if (Commands->Is_A_Star(o))
			{
				if ((Get_Object_Type(o) == Team) || (Team == 2))
				{
					return o;
				}
			}
		}
		x = x->Next();
	}
	return 0;
}

bool SCRIPTS_API Change_Player_Team(GameObject *obj,bool Retain_Score,bool Retain_Money,bool Show_Host_Message)
{
	if (!obj)
	{
		return false;
	}
	if (!Commands->Is_A_Star(obj))
	{
		return false;
	}
	Disarm_All_C4_Beacons(Get_Player_ID(obj));
	float Score = Commands->Get_Points(obj);
	float Credits = Commands->Get_Money(obj);
	Commands->Give_Points(obj,(Score*-1),false);
	if (Score < 0)
	{
		Commands->Give_Money(obj,Score,false);
	}
	Commands->Give_Money(obj,(Credits*-1),false);
	if (!Commands->Get_Player_Type(obj))
	{
		Change_Team(obj,1);
	}
	else if (Commands->Get_Player_Type(obj) == 1)
	{
		Change_Team(obj,0);
	}
	if (Retain_Score)
	{
		Commands->Give_Points(obj,Score,false);
		if (Score > 0)
		{
			Commands->Give_Money(obj,(Score*-1),false);
		}
	}
	if (Retain_Money)
	{
		Commands->Give_Money(obj,Credits,false);
	}
	if (Show_Host_Message)
	{
		char outputmsg[100];
		const char *name = Get_Player_Name(obj);
		sprintf(outputmsg,"msg %s switched teams",name);
		delete[] name;
		Console_Input(outputmsg);
	}
	return true;
}

GameObject SCRIPTS_API *Get_Part_Name(const char *name1)
{
	SLNode<SoldierGameObj> *x = GameObjManager::StarGameObjList.Head();
	int count = 0;
	GameObject *current = 0;
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			const char *name = Get_Player_Name(o);
			if (stristr(name,name1))
			{
				current = o;
				count++;
			}
			delete[] name;
		}
		x = x->Next();
	}
	if ((count == 1) && (current) && (Commands->Get_ID(current)))
	{
		return current;
	}
	else
	{
		return 0;
	}
}

int SCRIPTS_API Get_Part_Names(const char *name1)
{
	SLNode<SoldierGameObj> *x = GameObjManager::StarGameObjList.Head();
	int count = 0;
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			const char *name = Get_Player_Name(o);
			if (stristr(name,name1))
			{
				count++;
			}
			delete[] name;
		}
		x = x->Next();
	}
	return count;
}

int SCRIPTS_API Get_Team_Player_Count(int Team)
{
	int Total = 0;
	SLNode<SoldierGameObj> *x = GameObjManager::StarGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o && Get_Object_Type(o) == Team)
		{
			Total++;
		}
		x = x->Next();
	}
	return Total;
}

int SCRIPTS_API Get_Player_Type(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	SoldierGameObj *o = obj->As_SoldierGameObj();
	if (!o)
	{
		return 0;
	}
	if (!((cPlayer *)o->Get_Player_Data()))
	{
		return 0;
	}
	return ((cPlayer *)o->Get_Player_Data())->Get_Player_Type();
}

const wchar_t SCRIPTS_API *Get_Wide_Player_Name(GameObject *obj)
{
	if (!obj)
	{
		return L"None";
	}
	SoldierGameObj *o = obj->As_SoldierGameObj();
	if (!o)
	{
		return L"None";
	}
	if (!((cPlayer *)o->Get_Player_Data()) && o->Get_Bot_Tag().Is_Empty())
	{
		return L"None";
	}
	if (!o->Get_Bot_Tag().Is_Empty())
	{
		return o->Get_Bot_Tag().Peek_Buffer();
	}
	return ((cPlayer *)o->Get_Player_Data())->Get_Name().Peek_Buffer();
}

void cPlayer::Set_Name(const WideStringClass& name)
{
	this->PlayerName = name;
	this->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
}

void cPlayer::Set_Id(uint32 id)
{
	this->PlayerId = id;
	this->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
}

/*void cPlayer::Set_Wol_ClanID(uint32 wolClanId) //DA
{
	this->WolClanId = wolClanId;
}*/

void cPlayer::Set_Is_Waiting_For_Intermission(bool waitingForIntermission)
{
	this->IsWaitingForIntermission = waitingForIntermission;
}

void cPlayer::Set_Last_Update_Time_Ms(uint32 lastUpdate)
{
	this->LastUpdateTimeMs = lastUpdate;
}

void cPlayer::Init()
{
	Set_Object_Dirty_Bit(NetworkObjectClass::BIT_CREATION, true);
}






WideStringClass cPlayer::getNameLabel() const
{
	WideStringClass nameLabel = PlayerName;
	
	if (_wcsicmp(PlayerName, The_Game()->Get_Mvp_Name()) == 0)
	{
		nameLabel += L"\n";
		nameLabel += Get_Wide_Translated_String(9034); // IDS_MP_MVP
		
		uint mvpCount = The_Game()->Get_Mvp_Count();
		if (mvpCount > 1)
		{
			WideStringClass mvpMultiplier;
			mvpMultiplier.Format(L" * %d", mvpCount);
			nameLabel += mvpMultiplier;
		}
	}

	if (!customTag.Is_Empty())
	{
		nameLabel += L"\n";
		nameLabel += customTag;
	}
	
	return nameLabel;
}





bool cPlayer::isFriendly
   (const DamageableGameObj& object) const
{
	int pt = PlayerType;
	return (object.Get_Player_Type() == pt);
}



bool cPlayer::isHostile
   (const DamageableGameObj& object) const
{
   int myTeamId = this->PlayerType;
   if (myTeamId < 0 || myTeamId > 1)
      return false;

   if (object.Get_Player_Type() < 0 || object.Get_Player_Type() > 1)
      return false;

   return (myTeamId != object.Get_Player_Type());
}



void cPlayer::Set_GameSpy_Auth_State(GAMESPY_AUTH_STATE_ENUM _GameSpyAuthState)
{
	if (_GameSpyAuthState != GameSpyAuthState)
	{
		GameSpyAuthState = _GameSpyAuthState;
		GameSpyAuthStateEntryTimeMs = TIMEGETTIME();
	}
}

SCRIPTS_API int Get_Player_Team(int PlayerID)
{
        cPlayer *x = Find_Player(PlayerID);
        if (!x)
		{
                return 0;
        }
		return x->Get_Player_Type();
}

SCRIPTS_API int Get_Player_ID_By_Name(const char *Name)
{
        cPlayer *Player = Find_Player(Name);
        if (Player)
		{
                return Player->Get_Id();
        }
        return 0;
}

SCRIPTS_API bool Is_Active(int ID)
{
        cPlayer *Player = Find_Player(ID);
        if (Player)
		{
                return Player->Get_Is_Active();
        }
        return false;
}

SCRIPTS_API void Set_Kills(int ID,int kills)
{
        cPlayer *Player = Find_Player(ID);
        if (Player)
		{
			Player->Set_Kills(kills);
		}
}

SCRIPTS_API void Set_Deaths(int ID,int deaths)
{
        cPlayer *Player = Find_Player(ID);
        if (Player)
		{
			Player->Set_Deaths(deaths);
		}
}

SCRIPTS_API void Set_Team_Score(int ID,float score)
{
	cTeam *teamdata = Find_Team(ID);
	if (teamdata)
	{
		teamdata->Set_Score(score);
	}
}

void cPlayer::Set_Kills(int kills)
{
	Kills = kills;
	Set_Object_Dirty_Bit(BIT_OCCASIONAL,true);
}

void cPlayer::Set_Deaths(int deaths)
{
	Deaths = deaths;
	Set_Object_Dirty_Bit(BIT_OCCASIONAL,true);
}

void cTeam::Set_Kills(int _kills) //DA
{
	kills = _kills;
	Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE, true);
}

void cTeam::Increment_Kills() //DA
{
	if (CombatManager::Is_Gameplay_Permitted())
	{
		kills++;
		Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE, true);
	}
}

void cTeam::Decrement_Kills() { //DA
	kills--;
	Set_Object_Dirty_Bit(BIT_RARE, true);
}

void cTeam::Set_Score(float _score)
{
	score = _score;
	Set_Object_Dirty_Bit(NetworkObjectClass::BIT_OCCASIONAL, true);
}

void cTeam::Set_Deaths(int _deaths)
{
	deaths = _deaths;
	Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE, true);
}

void cTeam::Increment_Deaths() //DA
{
	if (CombatManager::Is_Gameplay_Permitted())
	{
		deaths++;
		Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE, true);
	}
}

void cTeam::Decrement_Deaths() { //DA
	deaths--;
	Set_Object_Dirty_Bit(BIT_RARE, true);
}

void cTeam::Increment_Score(float amount) //DA
{
	if (CombatManager::Is_Gameplay_Permitted())
	{
		score += amount;
		Set_Object_Dirty_Bit(NetworkObjectClass::BIT_OCCASIONAL, true);
	}
}

float cTeam::Get_Kill_To_Death_Ratio() //DA
{
	return deaths == 0 ? -1.f : kills / deaths;
}

SCRIPTS_API void ChangeTeamDeaths(int teamId,int amount)
{
	cTeam *teamdata = Find_Team(teamId);
	if (teamdata)
		teamdata->Set_Deaths(teamdata->Get_Deaths()+amount);
}
