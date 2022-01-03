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
#ifndef TT_INCLUDE_CPLAYER_H
#define TT_INCLUDE_CPLAYER_H
#include "PlayerDataClass.h"
#include "NetworkObjectClass.h"
#include "engine_string.h"
#include "ScriptableGameObj.h" //DA

class SoldierGameObj; //DA
class DAPlayerClass; //DA

enum GAMESPY_AUTH_STATE_ENUM
{
	GAMESPY_AUTH_STATE_INITIAL,
	GAMESPY_AUTH_STATE_CHALLENGED,
	GAMESPY_AUTH_STATE_VALIDATING,
	GAMESPY_AUTH_STATE_ACCEPTED,
	GAMESPY_AUTH_STATE_REJECTING,
	GAMESPY_AUTH_STATE_REJECTED,
};


class cPlayer : public PlayerDataClass, public NetworkObjectClass {
public:
	cPlayer();
	~cPlayer();
	bool Load(ChunkLoadClass& chunkLoader);
	bool Save(ChunkSaveClass& chunkSaver);
	void Reset_Player();
	void Set_Score(float score);
	SCRIPTS_API void Increment_Score(float score); //DA
	void Set_Money(float money);
	void Increment_Money(float money);
	unsigned int Get_Network_Class_ID() const;
	void Import_Creation(BitStreamClass &BitStream);
	void Import_Rare(BitStreamClass &BitStream);
	void Import_Occasional(BitStreamClass &BitStream);
	void Export_Creation(BitStreamClass &BitStream);
	void Export_Rare(BitStreamClass &BitStream);
	void Export_Occasional(BitStreamClass &BitStream);
	void Delete();
	void SCRIPTS_API Set_Player_Type(int type);
	void Set_Ladder_Points(int amount);
	void Set_Rung(int amount);
	void Set_Is_Active(bool active);
	void Set_Name(const WideStringClass& name);
	void Set_Id(uint32 id);
	//void Set_Wol_ClanID(uint32 wolClanId); //DA
	void Set_Wol_Rank(sint32 wolRank);
	void Set_Is_In_Game(bool);
	void Set_Is_Waiting_For_Intermission(bool);
	void Set_Last_Update_Time_Ms(uint32 lastUpdate);
	void Set_Ip_Address(unsigned long ip);
	int Get_Ip_Address() const {return IpAddress;}
	void Reset_Join_Time();
	void Increment_Total_Time();
	void Init();
	Vector3 Get_Color() const;
	bool Get_Invulnerable() const {return Invulnerable;}
	bool Is_Team_Player();
	int Get_Ping();
	void Get_Player_String(int id,WideStringClass &str,bool b);
	SCRIPTS_API void Increment_Kills(); //DA
	SCRIPTS_API void Increment_Deaths(); //DA
	SCRIPTS_API void Decrement_Kills(); //DA
	SCRIPTS_API void Decrement_Deaths(); //DA
	SCRIPTS_API void Set_Kills(int kills);
	SCRIPTS_API void Set_Deaths(int deaths);
//private: //DA
	bool Invulnerable; // 1876
	WideStringClass PlayerName; // 1880
	int PlayerId; // 1884
	safe_int LadderPoints; // 1888
	safe_int Kills; // 1892
	safe_int Deaths; // 1896
	safe_int PlayerType; // 1900
	safe_int Rung; // 1904
	safe_int WolRank; // 1908
	short WolPoints; // 1912
	int DamageScaleFactor; // 1916
	int Ping; // 1920
	int JoinTime; // 1924
	int TotalTime; // 1928
	unsigned long IpAddress; // 1932
	int Fps; // 1936
	unsigned long LastUpdateTimeMs; // 1940
	int FastSortKey; // 1944
	int NumWolGames; // 1948
	bool IsWaitingForIntermission; // 1952; This is used for clients that joined during intermission time, these will be notified about when the game starts by cNetwork::Enabled_Waiting_Players.
	GAMESPY_AUTH_STATE_ENUM GameSpyAuthState; // 1956
	unsigned long GameSpyAuthStateEntryTimeMs; // 1960
	StringClass GameSpyChallengeString; // 1964
	StringClass GameSpyHashId; // 1968
	int GameSpyKickState; // 1972
	unsigned long GameSpyKickStateEntryTimeMs; // 1976
	bool IsInGame; // 1980
	bool IsActive; // 1981
	//unsigned long WolClanId; // 1984 //DA
public:
	int Get_Join_Time() const {return JoinTime;}
	int Get_Total_Time() const {return TotalTime;}
	void Set_Fps(int _fps) { Fps = _fps; }
	bool Get_Is_Active() const { return IsActive; }
	bool Is_Human() const { return PlayerId >= 0; }
	bool Get_Is_In_Game() const { return IsInGame; }
	int Get_Id() const { return PlayerId; }
	bool Get_Is_Waiting_For_Intermission() const { return IsWaitingForIntermission; }
	void Mark_As_Modified() { Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE, true); }
	bool Is_Active() {return IsActive;}
	const WideStringClass &Get_Name() const {return PlayerName;}
	int Get_Player_Type() const {return PlayerType;}
	int Get_Rung() const {return Rung;}
	int Get_Ladder_Points() const {return LadderPoints;}
	int Get_Kills() const {return Kills;}
	int Get_Deaths() const {return Deaths;}
	bool Is_Alive_And_Kicking() const { return PlayerId >= 0 && IsInGame && IsActive; }
	int Get_Damage_Scale_Factor() const {return DamageScaleFactor;}
	//int Get_WOL_ClanID() const {return WolClanId;} //DA
	short Get_WOL_Points() const {return WolPoints;}
	int Get_Num_WOL_Games() {return NumWolGames;}
	int Get_Fast_Sort_Key() {return FastSortKey;}
	void Set_Fast_Sort_Key(int key) {FastSortKey = key;}
	float Get_Kill_To_Death_Ratio() const
	{
		if (Deaths > 0)
		{
			return (float)Kills / (float)Deaths;
		}
		return -1;
	}
	
	void Set_GameSpy_Hash_Id(const StringClass& _GameSpyHashId) { GameSpyHashId = _GameSpyHashId; }
	void Set_GameSpy_Challenge_String(const StringClass& _GameSpyChallengeString) { GameSpyChallengeString = _GameSpyChallengeString; }
	GAMESPY_AUTH_STATE_ENUM Get_GameSpy_Auth_State() const { return GameSpyAuthState; }
	void Set_GameSpy_Auth_State(GAMESPY_AUTH_STATE_ENUM _GameSpyAuthState);

	DAPlayerClass *DAPlayer; //Replaces the unused WolClanId. This allows the functionality added in DAPlayerClass to be used directly through cPlayer. //DA
	inline void Set_DA_Player(DAPlayerClass *Player) { //DA
		DAPlayer = Player;
	}
	inline DAPlayerClass *Get_DA_Player() { //DA
		return DAPlayer;
	}
	inline SoldierGameObj *Get_GameObj() const {
		return (SoldierGameObj*)Owner.Get_Ptr();
	}
	inline bool Destroy_GameObj() {
		if (Owner) {
			Owner->Set_Delete_Pending();
			return true;
		}
		return false;
	}

	// Added by TT
	WideStringClass customTag;

	WideStringClass getNameLabel() const;
	bool isFriendly(const DamageableGameObj& object) const;
	bool isHostile(const DamageableGameObj& object) const;

private:
	SimpleDynVecClass<unsigned long> FloodCounter;

public:
	virtual void Increment_Flood_Counter();
	virtual void Decrement_Flood_Counter();
	virtual bool Is_Flooding();

};

#endif
