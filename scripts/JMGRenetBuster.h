/*	Renegade Renet Busters Scripts by Jerad Gray
	Copyright 2007 Jerad Gray, Jonathan Wilson

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#include "direct.h"
bool IsRenegadeServer = true;
#define PI 3.14159265f
#define PI180 PI/180
#define MaxGamePlayerCount 8
bool PlayerRequestingALife[MaxGamePlayerCount] = {false};
#define MaxNumberOfHighYieldWarheads 2
#define PLAYERDRONE_ID 1000000091
#define CARGOSHIP_ID 1000000074
#define CARGOSHIPTURRET_ID 1000000077
#define MaxHyperdriveCharge 5

// Perk Types

#define PERKCOUNT 126
#define PERKCATEGORIES 11
enum PerkTypeCategories{INVALID = -1,PerkPurchase = 0,PerkJob = 1,PerkSwimming = 2,PerkCharacterHealth = 3,PerkVehicle = 4,PerkCombat = 5,PerkDeath = 6,PerkMisc = 7,PerkWeapon = 8,PerkSpawnCharacter = 9,PerkUnusable = 999};

struct PerkSystemPerkTypeNode
{
	int PerkID;
	int CategoryPerkID;
	PerkTypeCategories Category;
	char Name[256];
	char Description[512];
	unsigned int UnlockCost;
	bool MultiplyUnlockCost;
	bool ResetOnFail;
	bool RequiresOrder;
	int MaxPerkUse;
	void AddDescription(const char *String)
	{
		if (strlen(String) <= 256)
			sprintf(Description,"%s",String);
		else
		{
			char msg[512];
			sprintf(msg,"msg Perk %d's Description is too long!",PerkID);
			Console_Input(msg);
		}
	}
};

PerkSystemPerkTypeNode PerkSystemPerkTypeNodeList[PERKCOUNT];

class PerkSystemPerkTypeList
{
private:
	void Reset()
	{
		USEABLEPERKCOUNT = 0;
		for (int x = 0;x < PERKCATEGORIES;x++)
			PerkTypeCategoryCounts[x] = 0;
		for (int x = 0;x < PERKCOUNT;x++)
			PerkSystemPerkTypeNodeList[x].Category = INVALID;
	}
	int PerkTypeCategoryCounts[PERKCATEGORIES];
public:
	static int USEABLEPERKCOUNT;
	PerkSystemPerkTypeList()
	{
		Reset();
	}
	void AddAPerkType(int PerkID,const char *Name,unsigned int UnlockCost,bool MultiplyUnlockCost,bool ResetOnFail,bool RequiresOrder,PerkTypeCategories Category,int MaxPerkUse,const char *Description)
	{
		if (PerkSystemPerkTypeNodeList[PerkID].Category != INVALID)
		{
			char errormsg[220];
			sprintf(errormsg,"msg ADDAPERKTYPE ERROR: PerkID %d has already been assigned!",PerkID);
			Console_Input(errormsg);
			Commands->Create_2D_WAV_Sound("yo1.wav");
		}
		USEABLEPERKCOUNT++;
		PerkSystemPerkTypeNodeList[PerkID].PerkID = PerkID;
		sprintf(PerkSystemPerkTypeNodeList[PerkID].Name,"%s",Name);
		PerkSystemPerkTypeNodeList[PerkID].AddDescription(Description);
		PerkSystemPerkTypeNodeList[PerkID].UnlockCost = UnlockCost;
		PerkSystemPerkTypeNodeList[PerkID].MultiplyUnlockCost = MultiplyUnlockCost;
		PerkSystemPerkTypeNodeList[PerkID].ResetOnFail = ResetOnFail;
		PerkSystemPerkTypeNodeList[PerkID].RequiresOrder = RequiresOrder;
		PerkSystemPerkTypeNodeList[PerkID].Category = Category;
		PerkSystemPerkTypeNodeList[PerkID].MaxPerkUse = MaxPerkUse;
	}
	void Clean_Up()
	{
		Reset();
	}
	void Finalize_Perks()
	{
		for (int x = 0;x < PERKCOUNT;x++)
			PerkSystemPerkTypeNodeList[x].CategoryPerkID = (PerkTypeCategoryCounts[PerkSystemPerkTypeNodeList[x].Category] = (PerkTypeCategoryCounts[PerkSystemPerkTypeNodeList[x].Category]+1));
	}
};
int PerkSystemPerkTypeList::USEABLEPERKCOUNT = 0;
// Player Perks

struct PlayerPerkDataNode
{// A single perk for a player
	bool HasPerk;
	unsigned int PerkProgress;
	PlayerPerkDataNode()
	{
		PerkProgress = 0;
		HasPerk = false;
	};
};

struct PlayerPerksDataNode
{// All of the players perks
	char PlayerName[128];
	int UnlockedPerks;
	PlayerPerkDataNode PlayerPerkList[PERKCOUNT];
	PlayerPerksDataNode *next;
	PlayerPerksDataNode()
	{
		UnlockedPerks = 0;
		for (int x = 0;x < PERKCOUNT;x++)
			PlayerPerkList[x] = PlayerPerkDataNode();
	};
};

PlayerPerksDataNode *PlayerPerkListNodes[128] = {NULL};

class PlayerPerksNodeControlSystem
{
private:
	bool HasLoaded;
	PlayerPerksDataNode *PlayerPerksDataNodeList;
	PlayerPerksDataNode *AddAPlayerPerkNode(PlayerPerksDataNode *Node,const char *PlayerName,PlayerPerkDataNode PPerkList[])
	{
		sprintf(Node->PlayerName,"%s",PlayerName);
		for (int x = 0;x < PERKCOUNT;x++)
		{
			Node->PlayerPerkList[x].HasPerk = PPerkList[x].HasPerk;
			Node->PlayerPerkList[x].PerkProgress = PPerkList[x].PerkProgress;
		}
		Node->UnlockedPerks = 0;
		Node->next = NULL;
		return Node;
	}
	PlayerPerksDataNode *GetPlayerPerkList(const char *PlayerName)
	{// Used to find if a player's perk list already exists, if not it creates a new node for the player's name
		if (!PlayerPerksDataNodeList)
		{
			PlayerPerkDataNode PPerkList[PERKCOUNT] = {PlayerPerkDataNode()};
			PlayerPerksDataNodeList = AddAPlayerPerkNode(new PlayerPerksDataNode,PlayerName,PPerkList);
			return PlayerPerksDataNodeList;
		}
		PlayerPerksDataNode *Current = PlayerPerksDataNodeList;
		while (Current)
		{
			if (!_stricmp(Current->PlayerName,PlayerName))
				return Current;
			if (!Current->next)
			{
				PlayerPerkDataNode PPerkList[PERKCOUNT] = {PlayerPerkDataNode()};
				Current->next = AddAPlayerPerkNode(new PlayerPerksDataNode,PlayerName,PPerkList);
				return Current->next;
			}
			Current = Current->next;
		}
		return NULL;
	}
	void AddPlayerToPerkList(const char *PlayerName,PlayerPerkDataNode PPerkList[])
	{// Used when loading the player perk list control file
		if (!PlayerPerksDataNodeList)
		{
			PlayerPerksDataNodeList = AddAPlayerPerkNode(new PlayerPerksDataNode,PlayerName,PPerkList);
			return;
		}
		PlayerPerksDataNode *Current = PlayerPerksDataNodeList;
		while (Current)
		{
			if (!_stricmp(Current->PlayerName,PlayerName))
				return;
			if (!Current->next)
			{
				Current->next = AddAPlayerPerkNode(new PlayerPerksDataNode,PlayerName,PPerkList);
				return;
			}
			Current = Current->next;
		}
		return;
	}
public:
	PlayerPerksNodeControlSystem()
	{
		HasLoaded = false;
		for (int x = 0;x < 128;x++)
			PlayerPerkListNodes[x] = NULL;
		PlayerPerksDataNodeList = NULL;
	};
	void FindPlayerPerkList(GameObject *obj,int DisplayMessage)
	{// Searches for the player's perk list, when it finds it it pulls it into the player ID if it isn't already, then it reloads the player's selected perks
		if (!Commands->Is_A_Star(obj))
			return;
		int PlayerID = JmgUtility::JMG_Get_Player_ID(obj);
		const char *PlayerName = Get_Player_Name(obj);
		if (!PlayerPerkListNodes[PlayerID] || _stricmp(PlayerPerkListNodes[PlayerID]->PlayerName,PlayerName))
			PlayerPerkListNodes[PlayerID] = GetPlayerPerkList(PlayerName);
	}
	bool IncreasePlayerPerkUnlockAmount(GameObject *obj,int PerkID,unsigned int IncreaseAmount)
	{
		if (!Commands->Is_A_Star(obj))
			return false;
		int PlayerID = JmgUtility::JMG_Get_Player_ID(obj);
		if (!PlayerPerkListNodes[PlayerID])
			return false;
		if (PlayerPerkListNodes[PlayerID]->PlayerPerkList[PerkID].HasPerk)
			return false;
		if (PerkSystemPerkTypeNodeList[PerkID].RequiresOrder)
		{
			if ((PlayerPerkListNodes[PlayerID]->PlayerPerkList[PerkID].PerkProgress + 1) == IncreaseAmount)
				PlayerPerkListNodes[PlayerID]->PlayerPerkList[PerkID].PerkProgress = IncreaseAmount;
			else if (PerkSystemPerkTypeNodeList[PerkID].ResetOnFail)
				PlayerPerkListNodes[PlayerID]->PlayerPerkList[PerkID].PerkProgress = 0;
			else
				PlayerPerkListNodes[PlayerID]->PlayerPerkList[PerkID].PerkProgress = IncreaseAmount;
		}
		else if (PerkSystemPerkTypeNodeList[PerkID].MultiplyUnlockCost)
			if (!PlayerPerkListNodes[PlayerID]->PlayerPerkList[PerkID].PerkProgress)
				PlayerPerkListNodes[PlayerID]->PlayerPerkList[PerkID].PerkProgress = IncreaseAmount; 
			else
				PlayerPerkListNodes[PlayerID]->PlayerPerkList[PerkID].PerkProgress *= IncreaseAmount;
		else
			PlayerPerkListNodes[PlayerID]->PlayerPerkList[PerkID].PerkProgress += IncreaseAmount;
		if ((PlayerPerkListNodes[PlayerID]->PlayerPerkList[PerkID].PerkProgress >= PerkSystemPerkTypeNodeList[PerkID].UnlockCost && !PerkSystemPerkTypeNodeList[PerkID].ResetOnFail) || PlayerPerkListNodes[PlayerID]->PlayerPerkList[PerkID].PerkProgress == PerkSystemPerkTypeNodeList[PerkID].UnlockCost)
		{
			GameObject *Player = Get_GameObj(PlayerID);
			if (Player)
			{
				char perkmsg[512];
				if (PerkSystemPerkTypeNodeList[PerkID].Category != PerkUnusable)
				{
					sprintf(perkmsg,"You have unlocked the %s perk!",PerkSystemPerkTypeNodeList[PerkID].Name);
					JmgUtility::DisplayChatMessage(Player,230,230,0,perkmsg);
				}
				JmgUtility::DisplayChatMessage(Player,230,230,0,PerkSystemPerkTypeNodeList[PerkID].Description);
				Create_2D_WAV_Sound_Player(Player,"sysmsg.wav");
			}
			PlayerPerkListNodes[PlayerID]->PlayerPerkList[PerkID].PerkProgress = PerkSystemPerkTypeNodeList[PerkID].UnlockCost;
			PlayerPerkListNodes[PlayerID]->PlayerPerkList[PerkID].HasPerk = true;
			PlayerPerkListNodes[PlayerID]->UnlockedPerks++;
		}
		else if (PlayerPerkListNodes[PlayerID]->PlayerPerkList[PerkID].PerkProgress > PerkSystemPerkTypeNodeList[PerkID].UnlockCost && PerkSystemPerkTypeNodeList[PerkID].ResetOnFail)
		{
			PlayerPerkListNodes[PlayerID]->PlayerPerkList[PerkID].PerkProgress = 0;
			return false;
		}
		return true;
	}
	void ResetPerkProgress(int PerkID)
	{
		PlayerPerksDataNode *Current = PlayerPerksDataNodeList;
		while (Current)
		{
			if (Current->PlayerPerkList[PerkID].HasPerk)
			{
				Current->UnlockedPerks--;
				Current->PlayerPerkList[PerkID].HasPerk = false;
			}
			Current->PlayerPerkList[PerkID].PerkProgress = 0;
			Current = Current->next;
		}
	}
	int GetPlayerPerkUnlockAmount(GameObject *obj,int PerkID)
	{
		if (!Commands->Is_A_Star(obj))
			return 0;
		int PlayerID = JmgUtility::JMG_Get_Player_ID(obj);
		if (!PlayerPerkListNodes[PlayerID])
			return 0;
		return PlayerPerkListNodes[PlayerID]->PlayerPerkList[PerkID].PerkProgress;
	}
	int GetPlayerUnlockedPerkCountByCategory(GameObject *obj,int Category)
	{
		if (!Commands->Is_A_Star(obj))
			return 0;
		int PlayerID = JmgUtility::JMG_Get_Player_ID(obj);
		if (!PlayerPerkListNodes[PlayerID])
			return 0;
		int Count = 0;
		for (int x = 0;x < PERKCOUNT;x++)
			if (PerkSystemPerkTypeNodeList[x].Category == Category)
				if (PlayerPerkListNodes[PlayerID]->PlayerPerkList[x].HasPerk)
					Count++;
		return Count;
	}
	bool CheckForAnyUnlockedPerks(GameObject *obj)
	{
		if (!Commands->Is_A_Star(obj))
			return false;
		int PlayerID = JmgUtility::JMG_Get_Player_ID(obj);
		if (!PlayerPerkListNodes[PlayerID])
			return false;
		for (int x = 0;x < PERKCOUNT;x++)
			if (PerkSystemPerkTypeNodeList[x].Category != INVALID)
				if (PlayerPerkListNodes[PlayerID]->PlayerPerkList[x].HasPerk)
					return true;
		return false;
	}
	bool CheckIfPlayerHasPerkUnlocked(GameObject *obj,int PerkID)
	{
		if (!Commands->Is_A_Star(obj))
			return false;
		int PlayerID = JmgUtility::JMG_Get_Player_ID(obj);
		if (!PlayerPerkListNodes[PlayerID])
			return false;
		if (PlayerPerkListNodes[PlayerID]->PlayerPerkList[PerkID].HasPerk)
			return true;
		return false;
	}
	void ClearPerksList()
	{
		HasLoaded = false;
		for (int x = 0;x < 128;x++)
			PlayerPerkListNodes[x] = NULL;
		PlayerPerksDataNode *Current = PlayerPerksDataNodeList,*Temp = NULL;
		while (Current)
		{	
			Temp = Current;
			Current = Current->next;
			delete Temp;
		}
		PlayerPerksDataNodeList = NULL;
	}
	void SaveData()
	{
		if (!HasLoaded)
		{
			Console_Input("msg PlayerPerksNodeControlSystem ERROR: Saved blocked!");
			return;
		}
		char filePath[256], tempPath[256];
		sprintf(filePath,"%sSave",Get_File_Path());
		_mkdir(filePath);
		sprintf(tempPath,"%sSave\\BetaRp2PlayerPerks.tmp",Get_File_Path());
		FILE *SavePerks = fopen(tempPath,"w");
		if (!SavePerks)
			return;
		PlayerPerksDataNode *Current = PlayerPerksDataNodeList;
		while (Current)
		{
			char EncryptString[2048];
			sprintf(EncryptString,"");
			for (int x = 0;x < PERKCOUNT;x++)
				sprintf(EncryptString,"%s%d %d ",EncryptString,Current->PlayerPerkList[x].PerkProgress,Current->PlayerPerkList[x].PerkProgress);
			fprintf(SavePerks,"%s\n%s",JmgUtility::Rp2Encrypt(Current->PlayerName,25,5),JmgUtility::Rp2Encrypt2(EncryptString,Current->PlayerName[0],Current->PlayerName[1]));
			if (Current->next)
				fprintf(SavePerks,"\n");
			Current = Current->next;
		}
		fclose(SavePerks);
		sprintf(tempPath,"%sSave\\BetaRp2PlayerPerks.tmp",Get_File_Path());
		sprintf(filePath,"%sSave\\BetaRp2PlayerPerks.Rp2",Get_File_Path());
		remove(filePath);
		rename(tempPath,filePath);
		sprintf(tempPath,"%sSave\\BetaRp2PlayerPerks.txt",Get_File_Path());
		FILE *SavePerks2 = fopen(tempPath,"w");
		Current = PlayerPerksDataNodeList;
		while (Current)
		{
			char EncryptString[2048];
			sprintf(EncryptString,"");
			for (int x = 0;x < PERKCOUNT;x++)
				sprintf(EncryptString,"%s%.2f ",EncryptString,Current->PlayerPerkList[x].HasPerk ? 100.00 : ((float)Current->PlayerPerkList[x].PerkProgress/PerkSystemPerkTypeNodeList[x].UnlockCost*100.0f));
			fprintf(SavePerks2,"%s\n%s\n",Current->PlayerName,EncryptString);
			Current = Current->next;
		}
		fclose(SavePerks2);
	}
	void LoadData()
	{
		char loadPath[256];
		sprintf(loadPath,"%sSave\\BetaRp2PlayerPerks.Rp2",Get_File_Path());
		FILE *LoadPerks = fopen(loadPath,"r");
		if (LoadPerks)
		{
			while (!feof(LoadPerks))
			{
				char PlayerName[256];
				fgets(PlayerName,256,LoadPerks);
				int Length = strlen(PlayerName);
				if (Length <= 0)
				{
					Console_Input("msg PlayerPerksNodeControlSystem::LoadData ERROR: Length is less than 1");
					char DecryptString[2048];
					fgets(DecryptString,2048,LoadPerks);
					continue;
				}
				PlayerName[Length-1] = '\0';
				PlayerPerksDataNode *Current = GetPlayerPerkList(JmgUtility::Rp2Decrypt(PlayerName,25,5));
				char DecryptString[2048];
				fgets(DecryptString,2048,LoadPerks);
				char DecryptedString[2048];
				sprintf(DecryptedString,"%s",JmgUtility::Rp2Decrypt(DecryptString,Current->PlayerName[0],Current->PlayerName[1]));
				PlayerPerkDataNode LA[PERKCOUNT] = {PlayerPerkDataNode()};
				PlayerPerkDataNode LA2[PERKCOUNT] = {PlayerPerkDataNode()};
				sscanf(DecryptedString,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d 0",&LA[0].PerkProgress,&LA2[0].PerkProgress,&LA[1].PerkProgress,&LA2[1].PerkProgress,&LA[2].PerkProgress,&LA2[2].PerkProgress,&LA[3].PerkProgress,&LA2[3].PerkProgress,&LA[4].PerkProgress,&LA2[4].PerkProgress,&LA[5].PerkProgress,&LA2[5].PerkProgress,&LA[6].PerkProgress,&LA2[6].PerkProgress,&LA[7].PerkProgress,&LA2[7].PerkProgress,&LA[8].PerkProgress,&LA2[8].PerkProgress,&LA[9].PerkProgress,&LA2[9].PerkProgress,&LA[10].PerkProgress,&LA2[10].PerkProgress,&LA[11].PerkProgress,&LA2[11].PerkProgress,&LA[12].PerkProgress,&LA2[12].PerkProgress,&LA[13].PerkProgress,&LA2[13].PerkProgress,&LA[14].PerkProgress,&LA2[14].PerkProgress,&LA[15].PerkProgress,&LA2[15].PerkProgress,&LA[16].PerkProgress,&LA2[16].PerkProgress,&LA[17].PerkProgress,&LA2[17].PerkProgress,&LA[18].PerkProgress,&LA2[18].PerkProgress,&LA[19].PerkProgress,&LA2[19].PerkProgress,&LA[20].PerkProgress,&LA2[20].PerkProgress,&LA[21].PerkProgress,&LA2[21].PerkProgress,&LA[22].PerkProgress,&LA2[22].PerkProgress,&LA[23].PerkProgress,&LA2[23].PerkProgress,&LA[24].PerkProgress,&LA2[24].PerkProgress,&LA[25].PerkProgress,&LA2[25].PerkProgress,&LA[26].PerkProgress,&LA2[26].PerkProgress,&LA[27].PerkProgress,&LA2[27].PerkProgress,&LA[28].PerkProgress,&LA2[28].PerkProgress,&LA[29].PerkProgress,&LA2[29].PerkProgress,&LA[30].PerkProgress,&LA2[30].PerkProgress,&LA[31].PerkProgress,&LA2[31].PerkProgress,&LA[32].PerkProgress,&LA2[32].PerkProgress,&LA[33].PerkProgress,&LA2[33].PerkProgress,&LA[34].PerkProgress,&LA2[34].PerkProgress,&LA[35].PerkProgress,&LA2[35].PerkProgress,&LA[36].PerkProgress,&LA2[36].PerkProgress,&LA[37].PerkProgress,&LA2[37].PerkProgress,&LA[38].PerkProgress,&LA2[38].PerkProgress,&LA[39].PerkProgress,&LA2[39].PerkProgress,&LA[40].PerkProgress,&LA2[40].PerkProgress,&LA[41].PerkProgress,&LA2[41].PerkProgress,&LA[42].PerkProgress,&LA2[42].PerkProgress,&LA[43].PerkProgress,&LA2[43].PerkProgress,&LA[44].PerkProgress,&LA2[44].PerkProgress,&LA[45].PerkProgress,&LA2[45].PerkProgress,&LA[46].PerkProgress,&LA2[46].PerkProgress,&LA[47].PerkProgress,&LA2[47].PerkProgress,&LA[48].PerkProgress,&LA2[48].PerkProgress,&LA[49].PerkProgress,&LA2[49].PerkProgress,&LA[50].PerkProgress,&LA2[50].PerkProgress,&LA[51].PerkProgress,&LA2[51].PerkProgress,&LA[52].PerkProgress,&LA2[52].PerkProgress,&LA[53].PerkProgress,&LA2[53].PerkProgress,&LA[54].PerkProgress,&LA2[54].PerkProgress,&LA[55].PerkProgress,&LA2[55].PerkProgress,&LA[56].PerkProgress,&LA2[56].PerkProgress,&LA[57].PerkProgress,&LA2[57].PerkProgress,&LA[58].PerkProgress,&LA2[58].PerkProgress,&LA[59].PerkProgress,&LA2[59].PerkProgress,&LA[60].PerkProgress,&LA2[60].PerkProgress,&LA[61].PerkProgress,&LA2[61].PerkProgress,&LA[62].PerkProgress,&LA2[62].PerkProgress,&LA[63].PerkProgress,&LA2[63].PerkProgress,&LA[64].PerkProgress,&LA2[64].PerkProgress,&LA[65].PerkProgress,&LA2[65].PerkProgress,&LA[66].PerkProgress,&LA2[66].PerkProgress,&LA[67].PerkProgress,&LA2[67].PerkProgress,&LA[68].PerkProgress,&LA2[68].PerkProgress,&LA[69].PerkProgress,&LA2[69].PerkProgress,&LA[70].PerkProgress,&LA2[70].PerkProgress,&LA[71].PerkProgress,&LA2[71].PerkProgress,&LA[72].PerkProgress,&LA2[72].PerkProgress,&LA[73].PerkProgress,&LA2[73].PerkProgress,&LA[74].PerkProgress,&LA2[74].PerkProgress,&LA[75].PerkProgress,&LA2[75].PerkProgress,&LA[76].PerkProgress,&LA2[76].PerkProgress,&LA[77].PerkProgress,&LA2[77].PerkProgress,&LA[78].PerkProgress,&LA2[78].PerkProgress,&LA[79].PerkProgress,&LA2[79].PerkProgress,&LA[80].PerkProgress,&LA2[80].PerkProgress,&LA[81].PerkProgress,&LA2[81].PerkProgress,&LA[82].PerkProgress,&LA2[82].PerkProgress,&LA[83].PerkProgress,&LA2[83].PerkProgress,&LA[84].PerkProgress,&LA2[84].PerkProgress,&LA[85].PerkProgress,&LA2[85].PerkProgress,&LA[86].PerkProgress,&LA2[86].PerkProgress,&LA[87].PerkProgress,&LA2[87].PerkProgress,&LA[88].PerkProgress,&LA2[88].PerkProgress,&LA[89].PerkProgress,&LA2[89].PerkProgress,&LA[90].PerkProgress,&LA2[90].PerkProgress,&LA[91].PerkProgress,&LA2[91].PerkProgress,&LA[92].PerkProgress,&LA2[92].PerkProgress,&LA[93].PerkProgress,&LA2[93].PerkProgress,&LA[94].PerkProgress,&LA2[94].PerkProgress,&LA[95].PerkProgress,&LA2[95].PerkProgress,&LA[96].PerkProgress,&LA2[96].PerkProgress,&LA[97].PerkProgress,&LA2[97].PerkProgress,&LA[98].PerkProgress,&LA2[98].PerkProgress,&LA[99].PerkProgress,&LA2[99].PerkProgress,&LA[100].PerkProgress,&LA2[100].PerkProgress,&LA[101].PerkProgress,&LA2[101].PerkProgress,&LA[102].PerkProgress,&LA2[102].PerkProgress,&LA[103].PerkProgress,&LA2[103].PerkProgress,&LA[104].PerkProgress,&LA2[104].PerkProgress,&LA[105].PerkProgress,&LA2[105].PerkProgress,&LA[106].PerkProgress,&LA2[106].PerkProgress,&LA[107].PerkProgress,&LA2[107].PerkProgress,&LA[108].PerkProgress,&LA2[108].PerkProgress,&LA[109].PerkProgress,&LA2[109].PerkProgress,&LA[110].PerkProgress,&LA2[110].PerkProgress,&LA[111].PerkProgress,&LA2[111].PerkProgress,&LA[112].PerkProgress,&LA2[112].PerkProgress,&LA[113].PerkProgress,&LA2[113].PerkProgress,&LA[114].PerkProgress,&LA2[114].PerkProgress,&LA[115].PerkProgress,&LA2[115].PerkProgress,&LA[116].PerkProgress,&LA2[116].PerkProgress,&LA[117].PerkProgress,&LA2[117].PerkProgress,&LA[118].PerkProgress,&LA2[118].PerkProgress,&LA[119].PerkProgress,&LA2[119].PerkProgress,&LA[120].PerkProgress,&LA2[120].PerkProgress,&LA[121].PerkProgress,&LA2[121].PerkProgress,&LA[122].PerkProgress,&LA2[122].PerkProgress,&LA[123].PerkProgress,&LA2[123].PerkProgress,&LA[124].PerkProgress,&LA2[124].PerkProgress,&LA[125].PerkProgress,&LA2[125].PerkProgress);
				for (int x = 0;x < PERKCOUNT;x++)
				{
					if (LA2[x].PerkProgress == LA[x].PerkProgress)
						Current->PlayerPerkList[x].PerkProgress = LA[x].PerkProgress;
					else if (PerkSystemPerkTypeNodeList[x].ResetOnFail)
						Current->PlayerPerkList[x].PerkProgress = 0;
					if (PerkSystemPerkTypeNodeList[x].UnlockCost == LA[x].PerkProgress)
					{
						Current->PlayerPerkList[x].HasPerk = true;
						if (PerkSystemPerkTypeNodeList[x].PerkID)
							Current->UnlockedPerks++;
					}
					if (PerkSystemPerkTypeNodeList[x].UnlockCost < Current->PlayerPerkList[x].PerkProgress)
						Current->PlayerPerkList[x].PerkProgress = PerkSystemPerkTypeNodeList[x].UnlockCost;
				}
			}
			fclose(LoadPerks);
		}
		HasLoaded = true;
	}
};

PerkSystemPerkTypeList PerkTypeSystem = PerkSystemPerkTypeList();
PlayerPerksNodeControlSystem PlayerPerkSystemControl = PlayerPerksNodeControlSystem();
bool CheckIfPlayerHasPerkUnlocked(GameObject *obj,int PerkID)
{
	if (PlayerPerkSystemControl.CheckIfPlayerHasPerkUnlocked(obj,PerkID))
		return true;
	return false;
}
bool PerkSystemIncreasePlayerPerkUnlockAmount(GameObject *obj,int PerkID,unsigned int IncreaseAmount)
{
	return PlayerPerkSystemControl.IncreasePlayerPerkUnlockAmount(obj,PerkID,IncreaseAmount);
}
void AddPerkTypes()
{
	PerkTypeSystem.Clean_Up();
	for (int x = 0;x < 105;x++)
		if (x != 61 && x != 63)
			PerkTypeSystem.AddAPerkType(x,"N/A",1000000,false,false,false,PerkUnusable,0,"N/A");
	PerkTypeSystem.AddAPerkType(61,"To The Moon and Beyond",26,false,false,true,PerkWeapon,1,"Your reward for defending Earth!");
	PerkTypeSystem.AddAPerkType(63,"Gamer",10,false,false,false,PerkWeapon,1,"Buying Floppy Disks gives you Golden Floppy Disks instead.");
	PerkTypeSystem.AddAPerkType(105,"Extra Life Ability",5,false,false,false,PerkUnusable,0,"You have unlocked the Extra Life ability.");
	PerkTypeSystem.AddAPerkType(106,"Hyperdrive Ability",10,false,false,false,PerkUnusable,0,"You have unlocked the Hyperdrive ability.");
	PerkTypeSystem.AddAPerkType(107,"Double Points Ability",25,false,false,false,PerkUnusable,0,"You have unlocked the Double Points ability.");
	PerkTypeSystem.AddAPerkType(108,"Rapid Energy Regen Ability",25,false,false,false,PerkUnusable,0,"You have unlocked the Rapid Energy Regen ability.");
	PerkTypeSystem.AddAPerkType(109,"Weapons Upgrade Ability",25,false,false,false,PerkUnusable,0,"You have unlocked the Weapons Upgrade ability.");
	PerkTypeSystem.AddAPerkType(110,"Drone Ability",50,false,false,false,PerkUnusable,0,"You have unlocked the Drone ability.");
	PerkTypeSystem.AddAPerkType(111,"High Yield Warhead Ability",50,false,false,false,PerkUnusable,0,"You have unlocked the High Yield Warhead ability.");
	PerkTypeSystem.AddAPerkType(112,"SOS Beacon Ability",25,false,false,false,PerkUnusable,0,"You have unlocked the SOS Beacon ability.");
	PerkTypeSystem.AddAPerkType(113,"Drone Swarm Ability",25,false,false,false,PerkUnusable,0,"You have unlocked the Drone Swarm ability.");
	for (int x = 114;x < PERKCOUNT;x++)
		PerkTypeSystem.AddAPerkType(x,"N/A",1000000,false,false,false,PerkUnusable,0,"N/A");

	PerkTypeSystem.Finalize_Perks();
	PerkSystemPerkTypeList::USEABLEPERKCOUNT = 0;
	for (int x = 0;x < PERKCOUNT;x++)
		if (PerkSystemPerkTypeNodeList[x].PerkID)
			PerkSystemPerkTypeList::USEABLEPERKCOUNT++;
}


struct RenCometBustersScoreSystem
{
	int difficulty;
public:
	#define RCMTBHIGHSCORELISTCOUNT 61
	struct CMTBHighScoresNode
	{
		char PlayerName[256];
		unsigned long PlayTime;
		unsigned long RoundsPlayed;
		unsigned long Deaths;
		unsigned long CometSmall;
		unsigned long CometMedium;
		unsigned long CometLarge;
		unsigned long CometHuge;
		unsigned long TheMoon;
		unsigned long UFO;
		unsigned long Mine;
		unsigned long OtherPlayers;
		unsigned long HighestScore;
		unsigned long TotalScore;
		unsigned int HighestLevel;
		unsigned long TotalComets;
		unsigned long HEWPowerups;
		unsigned long HEWUsed;
		unsigned long RoundsFired;
		unsigned long TeamHighestLevel;
		unsigned long GrantedOtherPlayersLives;
		unsigned long ReceivedOtherPlayersLives;
		unsigned long CargoShip;
		unsigned long NewLivesAcquired;
		unsigned long ObjectsRammed;
		unsigned long SOSBeacons;
		unsigned long PowerPowerups;
		unsigned long ExtraLifePowerups;
		unsigned long UsedSOSBeacons;
		unsigned long PointPowerups;
		unsigned long PowerupPoints;
		unsigned long PickedUpDrone;
		unsigned long UsedDrones;
		unsigned long DroneKills;
		unsigned long DronesKilled;
		unsigned long DronePoints;
		unsigned long RegenPowerups;
		unsigned long DoublePointsPowerups;
		unsigned long DoublePointsPoints;
		unsigned long DeerPowerups;
		unsigned long TimeCloaked;
		unsigned long ShipCloakedCount;
		unsigned long TimeShielded;
		unsigned long ShieldRaisedCount;
		unsigned long RanOutOfPowerCount;
		unsigned long CargoShipScore;
		unsigned long CargoShipKills;
		unsigned long JumpsUsed;
		unsigned long PickedUpDroneSwarm;
		unsigned long UsedDronesSwarm;
		unsigned long TimeDisrupted;
		unsigned long UpgadedWeaponPowerups;
		unsigned long UpgadedWeaponTime;
		unsigned long HighestCleanScore;
		unsigned long HighestCleanTeamScore;
		unsigned int HighestCleanLevel;
		unsigned int HighestCleanTeamLevel;
		unsigned long HighestTeamScore;
		unsigned long LevelsPlayedWithPerks;
		unsigned long LevelsPlayedWithOutPerks;
		unsigned long LevelsPlayedWithPerksAsATeam;
		unsigned long LevelsPlayedWithOutPerksAsATeam;
		JmgUtility::GenericDateTime LastPlayTime;
		CMTBHighScoresNode *next;
		CMTBHighScoresNode(void)
		{
			sprintf(PlayerName,"\0");
			PlayTime = 0;
			RoundsPlayed = 0;
			Deaths = 0;
			CometSmall = 0;
			CometMedium = 0;
			CometLarge = 0;
			CometHuge = 0;
			TheMoon = 0;
			UFO = 0;
			Mine = 0;
			OtherPlayers = 0;
			HighestScore = 0;
			TotalScore = 0;
			HighestLevel = 0;
			TotalComets = 0;
			HEWPowerups = 0;
			HEWUsed = 0;
			RoundsFired = 0;
			TeamHighestLevel = 0;
			GrantedOtherPlayersLives = 0;
			ReceivedOtherPlayersLives = 0;
			CargoShip = 0;
			NewLivesAcquired = 0;
			ObjectsRammed = 0;
			SOSBeacons = 0;
			PowerPowerups = 0;
			ExtraLifePowerups = 0;
			UsedSOSBeacons = 0;
			PointPowerups = 0;
			PowerupPoints = 0;
			PickedUpDrone = 0;
			UsedDrones = 0;
			DroneKills = 0;
			DronesKilled = 0;
			DronePoints = 0;
			RegenPowerups = 0;
			DoublePointsPowerups = 0;
			DoublePointsPoints = 0;
			DeerPowerups = 0;
			TimeCloaked = 0;
			ShipCloakedCount = 0;
			TimeShielded = 0;
			ShieldRaisedCount = 0;
			RanOutOfPowerCount = 0;
			CargoShipScore = 0;
			CargoShipKills = 0;
			JumpsUsed = 0;
			PickedUpDroneSwarm = 0;
			UsedDronesSwarm = 0;
			TimeDisrupted = 0;
			UpgadedWeaponPowerups = 0;
			UpgadedWeaponTime = 0;
			HighestCleanScore = 0;
			HighestCleanTeamScore = 0;
			HighestCleanLevel = 0;
			HighestCleanTeamLevel = 0;
			HighestTeamScore = 0;
			LevelsPlayedWithPerks = 0;
			LevelsPlayedWithOutPerks = 0;
			LevelsPlayedWithPerksAsATeam = 0;
			LevelsPlayedWithOutPerksAsATeam = 0;
			LastPlayTime = JmgUtility::GenericDateTime();
			next = NULL;
		}
	};
private:
	CMTBHighScoresNode *CMTBHighScoresNodeList;
	CMTBHighScoresNode *CMTBHighScoresNodeEmptyNode;
	CMTBHighScoresNode *CMTBPlayerHighScoreNodes[128];
	struct PlayersInGameNode
	{
		char PlayerName[256];
		PlayersInGameNode *next;
	};
	PlayersInGameNode *PlayersInGameNodeList;
	void AddPlayerToInGameList(CMTBHighScoresNode *Node)
	{
		if (!Node)
			return;
		if (!PlayersInGameNodeList)
		{
			PlayersInGameNodeList = new PlayersInGameNode;
			sprintf(PlayersInGameNodeList->PlayerName,"%s",Node->PlayerName);
			PlayersInGameNodeList->next = NULL;
			Node->RoundsPlayed++;
			return;
		}
		PlayersInGameNode *Current = PlayersInGameNodeList;
		while (Current)
		{
			if (!_stricmp(Current->PlayerName,Node->PlayerName))
				return;
			if (!Current->next)
			{
				Current->next = new PlayersInGameNode;
				sprintf(Current->next->PlayerName,"%s",Node->PlayerName);
				Current->next->next = NULL;
				Node->RoundsPlayed++;
				return;
			}
			Current = Current->next;
		}
	}

	void LoadHighestScores()
	{
		if (!CMTBHighScoresNodeList)
			return;
		for (int x = 0;x < RCMTBHIGHSCORELISTCOUNT;x++)
		{
			CMTBHighScoresNode *Current = CMTBHighScoresNodeList,*Best = NULL;
			while (Current)
			{
				Best = CompareHighScoreNodes(Best,Current,x);
				Current = Current->next;
			}
			sprintf(HighScoreHolderName[x],"%s",Best->PlayerName);
		}
	}

	CMTBHighScoresNode *FindOrAddPlayerMAHighScoreNode(const char *PlayerName)
	{
		if (!CMTBHighScoresNodeList)
		{
			CMTBHighScoresNodeList = new CMTBHighScoresNode();
			sprintf(CMTBHighScoresNodeList->PlayerName,"%s",PlayerName);
			return CMTBHighScoresNodeList;
		}
		CMTBHighScoresNode *Current = CMTBHighScoresNodeList;
		while (Current)
		{
			if (!_stricmp(Current->PlayerName,PlayerName))
				return Current;
			if (!Current->next)
			{
				Current->next = new CMTBHighScoresNode();
				Current = Current->next;
				sprintf(Current->PlayerName,"%s",PlayerName);
				return Current;
			}
			Current = Current->next;
		}
		return NULL;
	}

public:
	RenCometBustersScoreSystem()
	{
		CMTBHighScoresNodeList = NULL;
		CMTBHighScoresNodeEmptyNode = NULL;
		for (int x = 0;x < 128;x++)
			CMTBPlayerHighScoreNodes[x] = NULL;
		PlayersInGameNodeList = NULL;
	};

	char HighScoreHolderName[RCMTBHIGHSCORELISTCOUNT][128];
	CMTBHighScoresNode *Get_Current_Player_Score_Node(int PlayerID)
	{
		const char *PlayerName = Get_Player_Name_By_ID(PlayerID);
		if (!PlayerName)
		{
			if (!CMTBHighScoresNodeEmptyNode)
				CMTBHighScoresNodeEmptyNode = new CMTBHighScoresNode();
			return CMTBHighScoresNodeEmptyNode;
		}
		if (!CMTBPlayerHighScoreNodes[PlayerID])
		{
			CMTBPlayerHighScoreNodes[PlayerID] = FindOrAddPlayerMAHighScoreNode(PlayerName);
			AddPlayerToInGameList(CMTBPlayerHighScoreNodes[PlayerID]);
		}
		else if (_stricmp(CMTBPlayerHighScoreNodes[PlayerID]->PlayerName,PlayerName))
		{
			CMTBPlayerHighScoreNodes[PlayerID] = FindOrAddPlayerMAHighScoreNode(PlayerName);
			AddPlayerToInGameList(CMTBPlayerHighScoreNodes[PlayerID]);
		}
		return CMTBPlayerHighScoreNodes[PlayerID];
	}

	void Cleanup()
	{
		if (CMTBHighScoresNodeEmptyNode)
		{
			delete CMTBHighScoresNodeEmptyNode;
			CMTBHighScoresNodeEmptyNode = NULL;
		}
		for (int x = 0;x < 128;x++)
		{
			if (CMTBPlayerHighScoreNodes[x])
				CMTBPlayerHighScoreNodes[x] = NULL;
		}
		if (CMTBHighScoresNodeList)
		{
			CMTBHighScoresNode *temp,*die;
			temp = CMTBHighScoresNodeList;
			while (temp != NULL)
			{
				die = temp;
				temp = temp->next;
				delete die;
			}
			CMTBHighScoresNodeList = NULL;
		}
		if (PlayersInGameNodeList)
		{
			PlayersInGameNode *temp,*die;
			temp = PlayersInGameNodeList;
			while (temp != NULL)
			{
				die = temp;
				temp = temp->next;
				delete die;
			}
			PlayersInGameNodeList = NULL;
		}
	}

	void SaveData()
	{
		FILE *SaveScores;
		FILE *SaveScores2;
		char FileName[256];
		char FileNameTmp[256];
		char SavePath[256];
		sprintf(SavePath, "%sSave\\", Get_File_Path());
		sprintf(FileNameTmp,"%sBetaCMBTPlayerRecords.tmp",SavePath);
		SaveScores = fopen(FileNameTmp,"w");
		switch (difficulty)
		{
		case 0:sprintf(FileName,"%sBetaCMBTPlayerRecordsEasy.txt",SavePath);break;
		case 1:sprintf(FileName,"%sBetaCMBTPlayerRecords.txt",SavePath);break;
		case 2:sprintf(FileName,"%sBetaCMBTPlayerRecordsHard.txt",SavePath);break;
		case 3:sprintf(FileName,"%sBetaCMBTPlayerRecordsDoomsday.txt",SavePath);break;
		};
		SaveScores2 = fopen(FileName,"w");
		CMTBHighScoresNode *Current = CMTBHighScoresNodeList;
		if (!SaveScores || !SaveScores2)
		{
			char debug[220];
			sprintf(debug,"msg RenCometBustersScoreSystem ERROR: Could not open save files %sBetaCMBTPlayerRecords.tmp and %sCMBTPlayerRecords.txt for writting!",SavePath,SavePath);
			Console_Input(debug);
			if (SaveScores)
				fclose(SaveScores);
			if (SaveScores2)
				fclose(SaveScores2);
			return;
		}
		while (Current)
		{
			//CalculateKillToDeathRatio(Current);
			char EncryptString[2048];
 			sprintf(EncryptString,"%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d %lu %lu %lu %lu %lu %d %d %d %d %d %d %lu 0",Current->PlayTime,Current->RoundsPlayed,Current->Deaths,Current->CometSmall,Current->CometMedium,Current->CometLarge,Current->CometHuge,Current->TheMoon,Current->UFO,Current->Mine,Current->OtherPlayers,Current->HighestScore,Current->TotalScore,Current->HighestLevel,Current->TotalComets,Current->HEWPowerups,Current->HEWUsed,Current->RoundsFired,Current->TeamHighestLevel,Current->GrantedOtherPlayersLives,Current->ReceivedOtherPlayersLives,Current->CargoShip,Current->NewLivesAcquired,Current->ObjectsRammed,Current->SOSBeacons,Current->PowerPowerups,Current->ExtraLifePowerups,Current->UsedSOSBeacons,Current->PointPowerups,Current->PowerupPoints,Current->PickedUpDrone,Current->UsedDrones,Current->DroneKills,Current->DronesKilled,Current->DronePoints,Current->RegenPowerups,Current->DoublePointsPowerups,Current->DoublePointsPoints,Current->DeerPowerups,Current->TimeCloaked,Current->ShipCloakedCount,Current->TimeShielded,Current->ShieldRaisedCount,Current->RanOutOfPowerCount,Current->CargoShipScore,Current->CargoShipKills,Current->JumpsUsed,Current->PickedUpDroneSwarm,Current->UsedDronesSwarm,Current->TimeDisrupted,Current->UpgadedWeaponPowerups,Current->UpgadedWeaponTime,Current->HighestCleanScore,Current->HighestCleanTeamScore,Current->HighestCleanLevel,Current->HighestCleanTeamLevel,Current->HighestTeamScore,Current->LevelsPlayedWithPerks,Current->LevelsPlayedWithOutPerks,Current->LevelsPlayedWithPerksAsATeam,Current->LevelsPlayedWithOutPerksAsATeam,Current->LastPlayTime.day,Current->LastPlayTime.month,Current->LastPlayTime.year,Current->LastPlayTime.second,Current->LastPlayTime.minute,Current->LastPlayTime.hour,Current->LastPlayTime.lTime);
			fprintf(SaveScores2,"%s\n%s\n",Current->PlayerName,EncryptString);
			fprintf(SaveScores,"%s\n%s",JmgUtility::Rp2Encrypt(Current->PlayerName,25,5),JmgUtility::Rp2Encrypt2(EncryptString,Current->PlayerName[0],Current->PlayerName[1]));
			if (Current->next)
				fprintf(SaveScores,"\n");
			Current = Current->next;
		}
		fclose(SaveScores);
		fclose(SaveScores2);
		switch (difficulty)
		{
		case 0:sprintf(FileName,"%sBetaCMBTPlayerRecordsEasy.Rp2",SavePath);break;
		case 1:sprintf(FileName,"%sBetaCMBTPlayerRecords.Rp2",SavePath);break;
		case 2:sprintf(FileName,"%sBetaCMBTPlayerRecordsHard.Rp2",SavePath);break;
		case 3:sprintf(FileName,"%sBetaCMBTPlayerRecordsDoomsday.Rp2",SavePath);break;
		};
		remove(FileName);
		sprintf(FileNameTmp,"%sBetaCMBTPlayerRecords.tmp",SavePath);
		rename(FileNameTmp,FileName);
	}

	void LoadData(int setDifficulty)
	{
		difficulty = setDifficulty;
		Cleanup();
		char PlayerName[256];
		FILE *LoadScores;
		char FileName[256];
		char SavePath[256];
		sprintf(SavePath, "%sSave\\", Get_File_Path());
		switch (difficulty)
		{
		case 0:sprintf(FileName,"%sBetaCMBTPlayerRecordsEasy.Rp2",SavePath);break;
		case 1:sprintf(FileName,"%sBetaCMBTPlayerRecords.Rp2",SavePath);break;
		case 2:sprintf(FileName,"%sBetaCMBTPlayerRecordsHard.Rp2",SavePath);break;
		case 3:sprintf(FileName,"%sBetaCMBTPlayerRecordsDoomsday.Rp2",SavePath);break;
		}
		LoadScores = fopen(FileName,"r");
		if (LoadScores)
		{
			while (!feof(LoadScores))
			{
				fgets(PlayerName,256,LoadScores);
				int Length = strlen(PlayerName);
				if (Length <= 0)
				{
					Console_Input("msg MutantAssaultHighScoreSystem::LoadData ERROR: Length is less than 1");
					continue;
				}
				PlayerName[Length-1] = '\0';
				CMTBHighScoresNode *Current = FindOrAddPlayerMAHighScoreNode(JmgUtility::Rp2Decrypt(PlayerName,25,5));
				char DecryptString[2048];
				fgets(DecryptString,2048,LoadScores);
				sscanf(JmgUtility::Rp2Decrypt(DecryptString,Current->PlayerName[0],Current->PlayerName[1]),"%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d %lu %lu %lu %lu %lu %d %d %d %d %d %d %lu",&Current->PlayTime,&Current->RoundsPlayed,&Current->Deaths,&Current->CometSmall,&Current->CometMedium,&Current->CometLarge,&Current->CometHuge,&Current->TheMoon,&Current->UFO,&Current->Mine,&Current->OtherPlayers,&Current->HighestScore,&Current->TotalScore,&Current->HighestLevel,&Current->TotalComets,&Current->HEWPowerups,&Current->HEWUsed,&Current->RoundsFired,&Current->TeamHighestLevel,&Current->GrantedOtherPlayersLives,&Current->ReceivedOtherPlayersLives,&Current->CargoShip,&Current->NewLivesAcquired,&Current->ObjectsRammed,&Current->SOSBeacons,&Current->PowerPowerups,&Current->ExtraLifePowerups,&Current->UsedSOSBeacons,&Current->PointPowerups,&Current->PowerupPoints,&Current->PickedUpDrone,&Current->UsedDrones,&Current->DroneKills,&Current->DronesKilled,&Current->DronePoints,&Current->RegenPowerups,&Current->DoublePointsPowerups,&Current->DoublePointsPoints,&Current->DeerPowerups,&Current->TimeCloaked,&Current->ShipCloakedCount,&Current->TimeShielded,&Current->ShieldRaisedCount,&Current->RanOutOfPowerCount,&Current->CargoShipScore,&Current->CargoShipKills,&Current->JumpsUsed,&Current->PickedUpDroneSwarm,&Current->UsedDronesSwarm,&Current->TimeDisrupted,&Current->UpgadedWeaponPowerups,&Current->UpgadedWeaponTime,&Current->HighestCleanScore,&Current->HighestCleanTeamScore,&Current->HighestCleanLevel,&Current->HighestCleanTeamLevel,&Current->HighestTeamScore,&Current->LevelsPlayedWithPerks,&Current->LevelsPlayedWithOutPerks,&Current->LevelsPlayedWithPerksAsATeam,&Current->LevelsPlayedWithOutPerksAsATeam,&Current->LastPlayTime.day,&Current->LastPlayTime.month,&Current->LastPlayTime.year,&Current->LastPlayTime.second,&Current->LastPlayTime.minute,&Current->LastPlayTime.hour,&Current->LastPlayTime.lTime);
			}
			fclose(LoadScores);	
		}
		LoadHighestScores();
	}

	CMTBHighScoresNode *CompareHighScoreNodes(CMTBHighScoresNode *High,CMTBHighScoresNode *Current,int CompVal)
	{
		if (!High)
			return Current;
		switch (CompVal)
		{
		case 0:
			return (High->RoundsPlayed >= Current->RoundsPlayed ? High : Current);
		case 1:
			return (High->Deaths >= Current->Deaths ? High : Current);
		case 2:
			return (High->CometSmall >= Current->CometSmall ? High : Current);
		case 3:
			return (High->CometMedium >= Current->CometMedium ? High : Current);
		case 4:
			return (High->CometLarge >= Current->CometLarge ? High : Current);
		case 5:
			return (High->CometHuge >= Current->CometHuge ? High : Current);
		case 6:
			return (High->TheMoon >= Current->TheMoon ? High : Current);
		case 7:
			return (High->UFO >= Current->UFO ? High : Current);
		case 8:
			return (High->Mine >= Current->Mine ? High : Current);
		case 9:
			return (High->OtherPlayers >= Current->OtherPlayers ? High : Current);
		case 10:
			return (High->HighestScore >= Current->HighestScore ? High : Current);
		case 11:
			return (High->TotalScore >= Current->TotalScore ? High : Current);
		case 12:
			return (High->HighestLevel >= Current->HighestLevel ? High : Current);
		case 13:
			return (High->TotalComets >= Current->TotalComets ? High : Current);
		case 14:
			return (High->HEWPowerups >= Current->HEWPowerups ? High : Current);
		case 15:
			return (High->HEWUsed >= Current->HEWUsed ? High : Current);
		case 16:
			return (High->RoundsFired >= Current->RoundsFired ? High : Current);
		case 17:
			return (High->TeamHighestLevel >= Current->TeamHighestLevel ? High : Current);
		case 18:
			return (High->GrantedOtherPlayersLives >= Current->GrantedOtherPlayersLives ? High : Current);
		case 19:
			return (High->ReceivedOtherPlayersLives >= Current->ReceivedOtherPlayersLives ? High : Current);
		case 20:
			return (High->CargoShip >= Current->CargoShip ? High : Current);
		case 21:
			return (High->NewLivesAcquired >= Current->NewLivesAcquired ? High : Current);
		case 22:
			return (High->ObjectsRammed >= Current->ObjectsRammed ? High : Current);
		case 23:
			return (High->SOSBeacons >= Current->SOSBeacons ? High : Current);
		case 24:
			return (High->PowerPowerups >= Current->PowerPowerups ? High : Current);
		case 25:
			return (High->ExtraLifePowerups >= Current->ExtraLifePowerups ? High : Current);
		case 26:
			return (High->UsedSOSBeacons >= Current->UsedSOSBeacons ? High : Current);
		case 27:
			return (High->PointPowerups >= Current->PointPowerups ? High : Current);
		case 28:
			return (High->PowerupPoints >= Current->PowerupPoints ? High : Current);
		case 29:
			return (High->PickedUpDrone >= Current->PickedUpDrone ? High : Current);
		case 30:
			return (High->UsedDrones >= Current->UsedDrones ? High : Current);
		case 31:
			return (High->DroneKills >= Current->DroneKills ? High : Current);
		case 32:
			return (High->DronesKilled >= Current->DronesKilled ? High : Current);
		case 33:
			return (High->DronePoints >= Current->DronePoints ? High : Current);
		case 34:
			return (High->RegenPowerups >= Current->RegenPowerups ? High : Current);
		case 35:
			return (High->DoublePointsPowerups >= Current->DoublePointsPowerups ? High : Current);
		case 36:
			return (High->DoublePointsPoints >= Current->DoublePointsPoints ? High : Current);
		case 37:
			return (High->DeerPowerups >= Current->DeerPowerups ? High : Current);
		case 38:
			return (High->TimeCloaked >= Current->TimeCloaked ? High : Current);
		case 39:
			return (High->ShipCloakedCount >= Current->ShipCloakedCount ? High : Current);
		case 40:
			return (High->TimeShielded >= Current->TimeShielded ? High : Current);
		case 41:
			return (High->ShieldRaisedCount >= Current->ShieldRaisedCount ? High : Current);
		case 42:
			return (High->RanOutOfPowerCount >= Current->RanOutOfPowerCount ? High : Current);
		case 43:
			return (High->CargoShipScore >= Current->CargoShipScore ? High : Current);
		case 44:
			return (High->CargoShipKills >= Current->CargoShipKills ? High : Current);
		case 45:
			return (High->JumpsUsed >= Current->JumpsUsed ? High : Current);
		case 46:
			return (High->PickedUpDroneSwarm >= Current->PickedUpDroneSwarm ? High : Current);
		case 47:
			return (High->UsedDronesSwarm >= Current->UsedDronesSwarm ? High : Current);
		case 48:
			return (High->TimeDisrupted >= Current->TimeDisrupted ? High : Current);
		case 49:
			return (High->UpgadedWeaponPowerups >= Current->UpgadedWeaponPowerups ? High : Current);
		case 50:
			return (High->UpgadedWeaponTime >= Current->UpgadedWeaponTime ? High : Current);
		case 51:
			return (High->HighestCleanScore >= Current->HighestCleanScore ? High : Current);
		case 52:
			return (High->HighestCleanTeamScore >= Current->HighestCleanTeamScore ? High : Current);
		case 53:
			return (High->HighestCleanLevel >= Current->HighestCleanLevel ? High : Current);
		case 54:
			return (High->HighestCleanTeamLevel >= Current->HighestCleanTeamLevel ? High : Current);
		case 55:
			return (High->HighestTeamScore >= Current->HighestTeamScore ? High : Current);
		case 56:
			return (High->LevelsPlayedWithPerks >= Current->LevelsPlayedWithPerks ? High : Current);
		case 57:
			return (High->LevelsPlayedWithOutPerks >= Current->LevelsPlayedWithOutPerks ? High : Current);
		case 58:
			return (High->LevelsPlayedWithPerksAsATeam >= Current->LevelsPlayedWithPerksAsATeam ? High : Current);
		case 59:
			return (High->LevelsPlayedWithOutPerksAsATeam >= Current->LevelsPlayedWithOutPerksAsATeam ? High : Current);
		default:
			return (High->PlayTime >= Current->PlayTime ? High : Current);
		}
	}

	bool ReturnHighScoreNodeValue(CMTBHighScoresNode *Node,int CompVal)
	{
		if (!Node)
			return false;
		switch (CompVal)
		{
		case 0:
			return Node->RoundsPlayed ? true : false;
		case 1:
			return Node->Deaths ? true : false;
		case 2:
			return Node->CometSmall ? true : false;
		case 3:
			return Node->CometMedium ? true : false;
		case 4:
			return Node->CometLarge ? true : false;
		case 5:
			return Node->CometHuge ? true : false;
		case 6:
			return Node->TheMoon ? true : false;
		case 7:
			return Node->UFO ? true : false;
		case 8:
			return Node->Mine ? true : false;
		case 9:
			return Node->OtherPlayers ? true : false;
		case 10:
			return Node->HighestScore ? true : false;
		case 11:
			return Node->TotalScore ? true : false;
		case 12:
			return Node->HighestLevel ? true : false;
		case 13:
			return Node->TotalComets ? true : false;
		case 14:
			return Node->HEWPowerups ? true : false;
		case 15:
			return Node->HEWUsed ? true : false;
		case 16:
			return Node->RoundsFired ? true : false;
		case 17:
			return Node->TeamHighestLevel ? true : false;
		case 18:
			return Node->GrantedOtherPlayersLives ? true : false;
		case 19:
			return Node->ReceivedOtherPlayersLives ? true : false;
		case 20:
			return Node->CargoShip ? true : false;
		case 21:
			return Node->NewLivesAcquired ? true : false;
		case 22:
			return Node->ObjectsRammed ? true : false;
		case 23:
			return Node->SOSBeacons ? true : false;
		case 24:
			return Node->PowerPowerups ? true : false;
		case 25:
			return Node->ExtraLifePowerups ? true : false;
		case 26:
			return Node->UsedSOSBeacons ? true : false;
		case 27:
			return Node->PointPowerups ? true : false;
		case 28:
			return Node->PowerupPoints ? true : false;
		case 29:
			return Node->PickedUpDrone ? true : false;
		case 30:
			return Node->UsedDrones ? true : false;
		case 31:
			return Node->DroneKills ? true : false;
		case 32:
			return Node->DronesKilled ? true : false;
		case 33:
			return Node->DronePoints ? true : false;
		case 34:
			return Node->RegenPowerups ? true : false;
		case 35:
			return Node->DoublePointsPowerups ? true : false;
		case 36:
			return Node->DoublePointsPoints ? true : false;
		case 37:
			return Node->DeerPowerups ? true : false;
		case 38:
			return Node->TimeCloaked ? true : false;
		case 39:
			return Node->ShipCloakedCount ? true : false;
		case 40:
			return Node->TimeShielded ? true : false;
		case 41:
			return Node->ShieldRaisedCount ? true : false;
		case 42:
			return Node->RanOutOfPowerCount ? true : false;
		case 43:
			return Node->CargoShipScore ? true : false;
		case 44:
			return Node->CargoShipKills ? true : false;
		case 45:
			return Node->JumpsUsed ? true : false;
		case 46:
			return Node->PickedUpDroneSwarm ? true : false;
		case 47:
			return Node->UsedDronesSwarm ? true : false;
		case 48:
			return Node->TimeDisrupted ? true : false;
		case 49:
			return Node->UpgadedWeaponPowerups ? true : false;
		case 50:
			return Node->UpgadedWeaponTime ? true : false;
		case 51:
			return Node->HighestCleanScore ? true : false;
		case 52:
			return Node->HighestCleanTeamScore ? true : false;
		case 53:
			return Node->HighestCleanLevel ? true : false;
		case 54:
			return Node->HighestCleanTeamLevel ? true : false;
		case 55:
			return Node->HighestTeamScore ? true : false;
		case 56:
			return Node->LevelsPlayedWithPerks ? true : false;
		case 57:
			return Node->LevelsPlayedWithOutPerks ? true : false;
		case 58:
			return Node->LevelsPlayedWithPerksAsATeam ? true : false;
		case 59:
			return Node->LevelsPlayedWithOutPerksAsATeam ? true : false;
		default:
			return Node->PlayTime ? true : false;
		}
	}

	char *ReturnScore(CMTBHighScoresNode *High,int CompVal)
	{
		static char RetChar[200];
		if (!High)
			return " ";
		switch (CompVal)
		{
		case 0:
			sprintf(RetChar,"Server Record: %s has played %lu rounds of Renegade Comet Busters.",High->PlayerName,High->RoundsPlayed);return RetChar;
		case 1:
			sprintf(RetChar,"Server Record: %s has lost %lu ships playing Renegade Comet Busters.",High->PlayerName,High->Deaths);return RetChar;
		case 2:
			sprintf(RetChar,"Server Record: %s has destroyed %lu small comets.",High->PlayerName,High->CometSmall);return RetChar;
		case 3:
			sprintf(RetChar,"Server Record: %s has destroyed %lu medium comets.",High->PlayerName,High->CometMedium);return RetChar;
		case 4:
			sprintf(RetChar,"Server Record: %s has destroyed %lu large comets.",High->PlayerName,High->CometLarge);return RetChar;
		case 5:
			sprintf(RetChar,"Server Record: %s has destroyed %lu huge comets.",High->PlayerName,High->CometHuge);return RetChar;
		case 6:
			sprintf(RetChar,"Server Record: %s has destroyed the moon %lu times.",High->PlayerName,High->TheMoon);return RetChar;
		case 7:
			sprintf(RetChar,"Server Record: %s has destroyed %lu UFOs.",High->PlayerName,High->UFO);return RetChar;
		case 8:
			sprintf(RetChar,"Server Record: %s has destroyed %lu Mines.",High->PlayerName,High->Mine);return RetChar;
		case 9:
			sprintf(RetChar,"Server Record: %s has killed %lu other players.",High->PlayerName,High->OtherPlayers);return RetChar;
		case 10:
			sprintf(RetChar,"Server Record: %s has scored %lu points playing Renegade Comet Busters in a single round.",High->PlayerName,High->HighestScore);return RetChar;
		case 11:
			sprintf(RetChar,"Server Record: %s has scored a total of %lu points playing Renegade Comet Busters.",High->PlayerName,High->TotalScore);return RetChar;
		case 12:
			sprintf(RetChar,"Server Record: %s has made it to level %lu in Renegade Comet Busters.",High->PlayerName,High->HighestLevel);return RetChar;
		case 13:
			sprintf(RetChar,"Server Record: %s has destroyed %lu comets total.",High->PlayerName,High->TotalComets);return RetChar;
		case 14:
			sprintf(RetChar,"Server Record: %s has acquired %lu High Yield Warhead powerups.",High->PlayerName,High->HEWPowerups);return RetChar;
		case 15:
			sprintf(RetChar,"Server Record: %s has used %lu High Yield Warheads.",High->PlayerName,High->HEWUsed);return RetChar;
		case 16:
			sprintf(RetChar,"Server Record: %s has fired a total of %lu rounds.",High->PlayerName,High->RoundsFired);return RetChar;
		case 17:
			sprintf(RetChar,"Server Record: %s has made it to level %lu through teamwork.",High->PlayerName,High->TeamHighestLevel);return RetChar;
		case 18:
			sprintf(RetChar,"Server Record: %s has given %lu lives to other players.",High->PlayerName,High->GrantedOtherPlayersLives);return RetChar;
		case 19:
			sprintf(RetChar,"Server Record: %s has received %lu lives from other players.",High->PlayerName,High->ReceivedOtherPlayersLives);return RetChar;
		case 20:
			sprintf(RetChar,"Server Record: %s has destroyed %lu cargo ships.",High->PlayerName,High->CargoShip);return RetChar;
		case 21:
			sprintf(RetChar,"Server Record: %s has acquired %lu extra lives.",High->PlayerName,High->NewLivesAcquired);return RetChar;
		case 22:
			sprintf(RetChar,"Server Record: %s has rammed %lu objects.",High->PlayerName,High->ObjectsRammed);return RetChar;
		case 23:
			sprintf(RetChar,"Server Record: %s has acquired %lu SOS Beacon powerups.",High->PlayerName,High->SOSBeacons);return RetChar;
		case 24:
			sprintf(RetChar,"Server Record: %s has acquired %lu power cell powerups.",High->PlayerName,High->PowerPowerups);return RetChar;
		case 25:
			sprintf(RetChar,"Server Record: %s has acquired %lu extra life powerups.",High->PlayerName,High->ExtraLifePowerups);return RetChar;
		case 26:
			sprintf(RetChar,"Server Record: %s has deployed %lu SOS Beacons.",High->PlayerName,High->UsedSOSBeacons);return RetChar;
		case 27:
			sprintf(RetChar,"Server Record: %s has acquired %lu points powerups.",High->PlayerName,High->PointPowerups);return RetChar;
		case 28:
			sprintf(RetChar,"Server Record: %s has %lu points from powerups.",High->PlayerName,High->PowerupPoints);return RetChar;
		case 29:
			sprintf(RetChar,"Server Record: %s has acquired %lu Drones.",High->PlayerName,High->PickedUpDrone);return RetChar;
		case 30:
			sprintf(RetChar,"Server Record: %s has launched %lu Drones.",High->PlayerName,High->UsedDrones);return RetChar;
		case 31:
			sprintf(RetChar,"Server Record: %s has destroyed %lu objects with Drones.",High->PlayerName,High->DroneKills);return RetChar;
		case 32:
			sprintf(RetChar,"Server Record: %s has destroyed %lu Drones.",High->PlayerName,High->DronesKilled);return RetChar;
		case 33:
			sprintf(RetChar,"Server Record: %s has scored %lu points with Drones.",High->PlayerName,High->DronePoints);return RetChar;
		case 34:
			sprintf(RetChar,"Server Record: %s has acquired %lu power regen powerups.",High->PlayerName,High->RegenPowerups);return RetChar;
		case 35:
			sprintf(RetChar,"Server Record: %s has acquired %lu double points powerups.",High->PlayerName,High->DoublePointsPowerups);return RetChar;
		case 36:
			sprintf(RetChar,"Server Record: %s has scored %lu points from double points powerups.",High->PlayerName,High->DoublePointsPoints);return RetChar;
		case 37:
			sprintf(RetChar,"Server Record: %s has acquired %lu hyperspace recharges.",High->PlayerName,High->DeerPowerups);return RetChar;
		case 38:
			sprintf(RetChar,"Server Record: %s has spent %.0lf minutes cloaked.",High->PlayerName,High->TimeCloaked/60.0f);return RetChar;
		case 39:
			sprintf(RetChar,"Server Record: %s has cloaked %lu times.",High->PlayerName,High->ShipCloakedCount);return RetChar;
		case 40:
			sprintf(RetChar,"Server Record: %s has spent %.0lf minutes with shields up.",High->PlayerName,High->TimeShielded/60.0f);return RetChar;
		case 41:
			sprintf(RetChar,"Server Record: %s has raised their shield %lu times.",High->PlayerName,High->ShieldRaisedCount);return RetChar;
		case 42:
			sprintf(RetChar,"Server Record: %s has ran out of power %lu times.",High->PlayerName,High->RanOutOfPowerCount);return RetChar;
		case 43:
			sprintf(RetChar,"Server Record: %s has scored %lu points by calling in Galatic Cargo Ships.",High->PlayerName,High->CargoShipScore);return RetChar;
		case 44:
			sprintf(RetChar,"Server Record: %s has destroyed %lu objects with Galatic Cargo Ships.",High->PlayerName,High->CargoShipKills);return RetChar;
		case 45:
			sprintf(RetChar,"Server Record: %s jumped out of danger %lu times.",High->PlayerName,High->JumpsUsed);return RetChar;
		case 46:
			sprintf(RetChar,"Server Record: %s has acquired %lu Drones Swarm Transmission Codes.",High->PlayerName,High->PickedUpDroneSwarm);return RetChar;
		case 47:
			sprintf(RetChar,"Server Record: %s has used %lu Drones Swarm Transmission Codes.",High->PlayerName,High->UsedDronesSwarm);return RetChar;
		case 48:
			sprintf(RetChar,"Server Record: %s has used the disrupter for %.0lf minutes.",High->PlayerName,High->TimeDisrupted/60.0f);return RetChar;
		case 49:
			sprintf(RetChar,"Server Record: %s has used the disrupter for %lu minutes.",High->PlayerName,High->UpgadedWeaponPowerups);return RetChar;
		case 50:
			sprintf(RetChar,"Server Record: %s has acquired %lu weapon upgrade powerups.",High->PlayerName,High->UpgadedWeaponPowerups);return RetChar;
		case 51:
			sprintf(RetChar,"Server Record: %s has gained a total score of %lu without any special abilities.",High->PlayerName,High->HighestCleanScore);return RetChar;
		case 52:
			sprintf(RetChar,"Server Record: %s has acquired a team score of %lu without any special abilities.",High->PlayerName,High->HighestCleanTeamScore);return RetChar;
		case 53:
			sprintf(RetChar,"Server Record: %s has made it to level %lu without any special abilities.",High->PlayerName,High->HighestCleanLevel);return RetChar;
		case 54:
			sprintf(RetChar,"Server Record: %s has made it to level %lu with teamwork and without any special abilities.",High->PlayerName,High->HighestCleanTeamLevel);return RetChar;
		case 55:
			sprintf(RetChar,"Server Record: %s has acquired a team score of %lu.",High->PlayerName,High->HighestTeamScore);return RetChar;
		case 56:
			sprintf(RetChar,"Server Record: %s has played %lu levels with special abilities.",High->PlayerName,High->LevelsPlayedWithPerks);return RetChar;
		case 57:
			sprintf(RetChar,"Server Record: %s has played %lu levels without special abilities.",High->PlayerName,High->LevelsPlayedWithOutPerks);return RetChar;
		case 58:
			sprintf(RetChar,"Server Record: %s has played %lu levels with a team using perks.",High->PlayerName,High->LevelsPlayedWithPerksAsATeam);return RetChar;
		case 59:
			sprintf(RetChar,"Server Record: %s has played %lu levels with a team not using perks.",High->PlayerName,High->LevelsPlayedWithOutPerksAsATeam);return RetChar;
		default:
			sprintf(RetChar,"Server Record: %s has played Renegade Comet Busters for %.0lf minutes on this server.",High->PlayerName,High->PlayTime/60.0f);
			return RetChar;
		}
	}

	void StateHighScore()
	{
		int maxRetry = 0;
		int LastHighScoreList = 1;
		int Random = Commands->Get_Random_Int(0,RCMTBHIGHSCORELISTCOUNT+1);
MAStartOfHighScoreSelectProcess:
		if (maxRetry > 10000)
			return;
		CMTBHighScoresNode *Current = CMTBHighScoresNodeList,*Best = NULL;
		while (Current)
		{
			Best = CompareHighScoreNodes(Best,Current,Random);
			Current = Current->next;
		}
		if (!ReturnHighScoreNodeValue(Best,Random))
		{
			if (LastHighScoreList <= 5)
			{
				LastHighScoreList++;
				if (Random+1 < RCMTBHIGHSCORELISTCOUNT)
					Random++;
				else
					Random = 1;
			}
			else
			{
				LastHighScoreList = 0;
				Random = 0;
			}
			maxRetry++;
			goto MAStartOfHighScoreSelectProcess;
		}
		sprintf(HighScoreHolderName[Random],"%s",Best->PlayerName);
		char DisplayMessage[128];
		sprintf(DisplayMessage,"%s",ReturnScore(Best,Random));
		for (int x = 1;x < 128;x++)
		{
			GameObject *Player = Get_GameObj(x);
			if (Player)
				JmgUtility::DisplayChatMessage(Player,6,145,148,DisplayMessage);
		}
	}
};

RenCometBustersScoreSystem RenCometBustersScoreControl = RenCometBustersScoreSystem();

enum ObjectType{NormalPlayerShip=0,UFO=1,Asteroid=2,Mine=3,TheMoon=4,PlayerShield=5,UFOBoss=6,PlayerCloak=7,PlayerSuper=8,CMTPowerup=9,CargoShip=10,MineBoss=11,PlayerDrone=12,PlayerDroneShield=13,PlayerShipJumpable=14,PlayerShipJumping=15,PlayerShipDisrupter=16};
struct TypeObject
{
	ReferencerClass obj;
	float Size;
	float RealSize;
	int parentId;
	ObjectType Type;
	TypeObject(GameObject *_obj,float _SquaredSize,float _RealSize,ObjectType _Type)
	{
		obj = _obj;
		Size = _SquaredSize;
		RealSize = _RealSize;
		Type = _Type;
		parentId = 0;
	};
	TypeObject(GameObject *obj,float Size,float RealSize,ObjectType Type,int parentId)
	{
		this->obj = obj;
		this->Size = Size;
		this->RealSize = RealSize;
		this->Type = Type;
		this->parentId = parentId;
	};
};

struct AnObject
{
	ReferencerClass Object;
	float Size;
	float RealSize;
	int ObjectID;
	ObjectType Type;
	int parentId;
	int AllowReplaceTime;
	struct AnObject *next;
	AnObject(GameObject *obj,float Size,float RealSize,ObjectType Type,int parentId)
	{
		this->AllowReplaceTime = -1;
		this->Object = obj;
		this->ObjectID = Commands->Get_ID(obj);
		this->Size = Size;
		this->RealSize = RealSize;
		this->Type = Type;
		this->AllowReplaceTime = 0;
		this->parentId = parentId;
		this->next = NULL;
	};
};

struct RenCometBustersPlayerNode
{
	bool isReady;
	int NumberOfLives;
	ReferencerClass PlayerShip;
	ReferencerClass ReplacementShip;
	ReferencerClass SpectatorObject;
	int GamePlayerID;
	int RespawnTime;
	char PlayerModel[32];
	float Score;
	Vector3 DeathLocation;
	Vector3 OriginalLocation;
	int FakeSoldierID;
	char OriginalSkin[128];
	char OriginalArmor[128];
	int PlayerID;
	int HighestLevel;
	int SuperBombs;
	float LastNewLifeScore;
	unsigned short NotInGameTime;
	bool HasSOSBeacon;
	int Drones[2];
	bool DronePowerup;
	bool DroneSwarmPowerup;
	float DroneScore;
	int CargoWarnedCount;
	int CargoTotalWarnedCount;
	int CargoWarnDecreaseDelay;
	int DoublePointsTime;
	int CargoShipIDs[7];
	float CargoScore;
	int HyperspaceDriveCharge;
	int SelectedPerk;
	bool SecondSpace;
	char PlayerName[128];
	int kickoutOfSpecTime;
	int endGamePadding;
	Vector3 specCreateSpot;
	AnObject *ShipObject;
	RenCometBustersPlayerNode()
	{
		isReady = false;
		NumberOfLives = 5;
		PlayerShip = NULL;
		ReplacementShip = NULL;
		SpectatorObject = NULL;
		GamePlayerID = 0;
		RespawnTime = 30;
		Score = 0;
		DeathLocation = Vector3(0.0f,0.0f,0.0f);
		OriginalLocation = Vector3(0.0f,0.0f,0.0f);
		FakeSoldierID = 0;
		PlayerID = 0;
		HighestLevel = 0;
		SuperBombs = 0;
		LastNewLifeScore = 0;
		NotInGameTime = 0;
		HasSOSBeacon = false;
		DronePowerup = false;
		DroneSwarmPowerup = false;
		Drones[0] = Drones[1] = 0;
		CargoWarnedCount = 0;
		CargoTotalWarnedCount = 0;
		CargoWarnDecreaseDelay = 0;
		DoublePointsTime = 0;
		CargoScore = 0.0f;
		HyperspaceDriveCharge = 0;
		SelectedPerk = 0;
		SecondSpace = false;
		ShipObject = NULL;
		sprintf(PlayerName,"");
		for (int x = 0;x < 7;x++)
			CargoShipIDs[x] = 0;
		sprintf(PlayerModel,"\0");
		sprintf(OriginalSkin,"\0");
		sprintf(OriginalArmor,"\0");
		kickoutOfSpecTime = 0;
		endGamePadding = 0;
		specCreateSpot = Vector3();
	};
};

struct RenCometBustersPlayerNode MiniGamePlayerControlSystem[MaxGamePlayerCount] = {RenCometBustersPlayerNode()};

class RenCometBustersGameObjects
{
private:
	static int PrioritizeTargets[17];
public:
	struct AnObject *ObjectsList;
	RenCometBustersGameObjects()
	{
		ObjectsList = NULL;
	};
	~RenCometBustersGameObjects()
	{
		if (ObjectsList)
		{
			AnObject *temp,*die;
			temp = ObjectsList;
			while (temp != NULL)
			{
				die = temp;
				temp = temp->next;
				delete die;
			}
			ObjectsList = NULL;
		}
	};
	RenCometBustersGameObjects &operator += (TypeObject obj)
	{
		if (!obj.obj)
			return *this;
		AnObject *Current = ObjectsList;
		if (!ObjectsList)
		{
			ObjectsList = new AnObject(obj.obj,obj.Size,obj.RealSize,obj.Type,obj.parentId);
			return *this;
		}
		while (Current)
		{
			if (Current->AllowReplaceTime == 30)
			{
				Current->AllowReplaceTime = -1;
				Current->Object = obj.obj;
				Current->ObjectID = Commands->Get_ID(obj.obj);
				Current->Size = obj.Size;
				Current->RealSize = obj.RealSize;
				Current->Type = obj.Type;
				Current->AllowReplaceTime = 0;
				Current->parentId = obj.parentId;
				return *this;
			}
			if (!Current->AllowReplaceTime && Current->Object == obj.obj)
			{
				Console_Input("msg already in list!");
				return *this;
			}
			if (!Current->next)
			{
				Current->next = new AnObject(obj.obj,obj.Size,obj.RealSize,obj.Type,obj.parentId);
				return *this;
			}
			Current = Current->next;
		}
		return *this;
	};
	RenCometBustersGameObjects &operator -= (GameObject *object)
	{
		if (!ObjectsList || !object)
			return *this;
		AnObject *Current = ObjectsList;
		while (Current)
		{
			if (Current->Object == object)
			{
				Current->AllowReplaceTime = 1;
			}
			Current = Current->next;
		}
		return *this;
	};
	AnObject *FindObject(GameObject *object)
	{
		if (!ObjectsList || !object)
			return NULL;
		AnObject *Current = ObjectsList;
		while (Current)
		{
			if (Current->Object == object)
				return Current;
			Current = Current->next;
		}
		return NULL;
	};
	AnObject *FindObjectByID(int objectID)
	{
		if (!ObjectsList || !objectID)
			return NULL;
		AnObject *Current = ObjectsList;
		while (Current)
		{
			if (Current->ObjectID == objectID)
				return Current;
			Current = Current->next;
		}
		return NULL;
	};
	void SpecialApplyDamage(GameObject *obj,float damage,const char *warhead,GameObject *damager)
	{
		GameObject *driver = Get_Vehicle_Driver(obj);
		int originalDriverTeam = driver ? Get_Player_Type(driver) : 1;
		Commands->Set_Player_Type(obj,-2);
		Commands->Set_Player_Type(driver,-2);
		Commands->Apply_Damage(obj,damage,warhead,damager);
		if (driver)
			Commands->Set_Player_Type(driver,originalDriverTeam);
	}
	bool DoesCollide(AnObject *Obj,AnObject *OtherObj)
	{
		if (!Obj || !OtherObj || Obj->AllowReplaceTime || OtherObj->AllowReplaceTime)
			return false;
		ReferencerClass Hitter = Obj->Object;
		ReferencerClass Other = OtherObj->Object;
		if (Obj->Type == PlayerDroneShield && OtherObj->Type == PlayerDroneShield)
			return false;
		if (OtherObj->Type == PlayerShipJumping || Obj->Type == PlayerShipJumping)
			return false;
		if (Obj->Type == CargoShip && OtherObj->Type == TheMoon)
		{
			if (JmgUtility::SimpleDistance(Commands->Get_Position(Hitter),Commands->Get_Position(Other)) <= Obj->Size+OtherObj->Size-200.0f)
			{
				Commands->Apply_Damage(Hitter,99999.9f,"BlamoKiller",Other);
				return true;
			}
			return false;
		}
		if (JmgUtility::SimpleDistance(Commands->Get_Position(Hitter),Commands->Get_Position(Other)) <= Obj->Size+OtherObj->Size)
		{
			if (Obj->Type == CargoShip && OtherObj->Type == CargoShip && Obj->parentId != OtherObj->parentId)
			{
				Commands->Apply_Damage(Hitter,99999.9f,"BlamoKiller",Other);
				Commands->Apply_Damage(Other,99999.9f,"BlamoKiller",Hitter);
				return true;
			}
			if (Obj->Type == TheMoon && OtherObj->Type == CargoShip)
				return false;
			if (OtherObj->Type == PlayerShipJumpable || Obj->Type == PlayerShipJumpable)
			{
				if (OtherObj->Type == PlayerShipJumpable && Commands->Get_Random(0.0f,1.0f) < 0.5f)
					Commands->Send_Custom_Event(Obj->Object,OtherObj->Object,77454831,0,0.0);
				else if (Obj->Type == PlayerShipJumpable)
					Commands->Send_Custom_Event(OtherObj->Object,Obj->Object,77454831,0,0);
				else if (OtherObj->Type == PlayerShipJumpable)
					Commands->Send_Custom_Event(Obj->Object,OtherObj->Object,77454831,0,0.0);
				return false;
			}
			if (Obj->Type == TheMoon || Obj->Type == CargoShip)
			{
				if (OtherObj->Type == PlayerShipJumpable)
				{
					Commands->Send_Custom_Event(Obj->Object,OtherObj->Object,77454831,0,0);
					return false;
				}
				if (OtherObj->Type == Mine || OtherObj->Type == MineBoss || OtherObj->Type == TheMoon)
					return false;
				if (OtherObj->Type == PlayerDroneShield)
				{
					Commands->Apply_Damage(Other,99999.9f,"BlamoKiller",Hitter);
					return true;
				}
				if (OtherObj->Type != Asteroid)
					Commands->Apply_Damage(Hitter,1.0f,"none",Other);
				if (OtherObj->Type != PlayerShield && OtherObj->Type != PlayerSuper)
					Commands->Apply_Damage(Other,99999.9f,"BlamoKiller",Hitter);
				else
				{
					Commands->Send_Custom_Event(Other,Other,7043453,25,0.0f);
					if (Commands->Get_Health(Hitter))
						return false;
				}
			}
			else
			{
				if (OtherObj->Type == PlayerShield && Obj->Type == PlayerShield)
				{
					Vector3 Pos = Commands->Get_Position(Obj->Object);
					Vector3 OtherPos = Commands->Get_Position(OtherObj->Object);
					Pos.X -= OtherPos.X;
					Pos.Y -= OtherPos.Y;
					float TempRotation = atan2(Pos.Y,Pos.X) * 180 / PI;
					Pos = Commands->Get_Position(Obj->Object);
					Pos.X += cos(TempRotation*PI180)*0.025f;
					Pos.Y += sin(TempRotation*PI180)*0.025f;
					TempRotation = JmgUtility::MathClampDegrees(TempRotation-180.0f);
					OtherPos.X += cos(TempRotation*PI180)*0.025f;
					OtherPos.Y += sin(TempRotation*PI180)*0.025f;
					GameObject *Lockdown = Commands->Create_Object("Daves Arrow",Pos);
					Commands->Set_Facing(Lockdown,Commands->Get_Facing(Obj->Object));
					Commands->Attach_To_Object_Bone(Obj->Object,Lockdown,"origin");
					Commands->Attach_To_Object_Bone(Obj->Object,Lockdown,"origin");
					Commands->Destroy_Object(Lockdown);
					Lockdown = Commands->Create_Object("Daves Arrow",OtherPos);
					Commands->Set_Facing(Lockdown,Commands->Get_Facing(OtherObj->Object));
					Commands->Attach_To_Object_Bone(OtherObj->Object,Lockdown,"origin");
					Commands->Attach_To_Object_Bone(OtherObj->Object,Lockdown,"origin");
					Commands->Destroy_Object(Lockdown);
					Commands->Send_Custom_Event(Hitter,Hitter,7043453,2142,0.0f);
					Commands->Send_Custom_Event(Other,Other,7043453,2142,0.0f);
					return false;
				}
				if (OtherObj->Type == Mine || OtherObj->Type == MineBoss)
				{
					Commands->Create_Explosion("Explosion_Drone",Commands->Get_Position(OtherObj->Object),0);
					Commands->Destroy_Object(OtherObj->Object);
					if (Obj->Type == PlayerShield || Obj->Type == PlayerDroneShield)
						Commands->Send_Custom_Event(Hitter,Hitter,7043453,10,0.0f);
				}
				if (Obj->Type == Mine || Obj->Type == MineBoss)
				{
					Commands->Create_Explosion("Explosion_Drone",Commands->Get_Position(Obj->Object),0);
					Commands->Destroy_Object(Obj->Object);
					if (OtherObj->Type == PlayerShield || OtherObj->Type == PlayerDroneShield)
						Commands->Send_Custom_Event(Other,Other,7043453,10,0.0f);
				}
				if (OtherObj->Type != PlayerShield && OtherObj->Type != PlayerSuper && OtherObj->Type != PlayerDroneShield && OtherObj->Type != TheMoon && OtherObj->Type != CargoShip)
					SpecialApplyDamage(Other,99999.9f,"BlamoKiller",Hitter);
				else
					if (Obj->Type == NormalPlayerShip || Obj->Type == PlayerCloak || Obj->Type == PlayerShipJumpable || Obj->Type == PlayerShipDisrupter)
						Commands->Send_Custom_Event(Other,Other,7043453,33,0.0f);
					else
						Commands->Send_Custom_Event(Other,Other,7043453,5,0.0f);
				if (Obj->Type != PlayerShield && Obj->Type != PlayerSuper && Obj->Type != PlayerDroneShield && OtherObj->Type != TheMoon)
					SpecialApplyDamage(Hitter,99999.9f,"BlamoKiller",Other);
				else
					if (OtherObj->Type == NormalPlayerShip || OtherObj->Type == PlayerCloak || OtherObj->Type == PlayerShipJumpable  || OtherObj->Type == PlayerShipDisrupter)
						Commands->Send_Custom_Event(Hitter,Hitter,7043453,33,0.0f);
					else
						Commands->Send_Custom_Event(Hitter,Hitter,7043453,5,0.0f);
			}
			return true;
		}
		return false;
	}
	bool PickupPowerup(AnObject *Obj,AnObject *OtherObj)
	{
		if (!Obj || !OtherObj || Obj->AllowReplaceTime || OtherObj->AllowReplaceTime)
			return true;
		ReferencerClass Hitter = Obj->Object;
		ReferencerClass Other = OtherObj->Object;
		if (JmgUtility::SimpleDistance(Commands->Get_Position(Hitter),Commands->Get_Position(Other)) <= Obj->Size+OtherObj->Size)
		{
			Commands->Apply_Damage(Hitter,99999.9f,"BlamoKiller",Other);
			return true;
		}
		return false;
	}
	void UpdateCollisions()
	{
		AnObject *Current = ObjectsList;
		while (Current)
		{
			if (Current->AllowReplaceTime)
			{
				if (Current->AllowReplaceTime < 30 && Current->AllowReplaceTime != -1)
					Current->AllowReplaceTime++;
			}
			Current = Current->next;
		}
CollisionCheckStart:
		Current = ObjectsList;
UpdateCollisions:
		while (Current)
		{
			if (!Current->AllowReplaceTime)
			{
				if (Current->Type == CargoShip || Current->Type == TheMoon || Current->Type == NormalPlayerShip || Current->Type == PlayerShield || Current->Type == PlayerCloak || Current->Type == PlayerSuper || Current->Type == PlayerShipJumpable || Current->Type == PlayerShipDisrupter)
				{
					AnObject *OtherObjs = ObjectsList;
					while (OtherObjs)
					{
						if (Current->Type == CargoShip && OtherObjs->Type == CargoShip && Current->parentId != OtherObjs->parentId)
							DoesCollide(Current,OtherObjs);
						else if (Current->Type == CargoShip && OtherObjs->Type == TheMoon)
							DoesCollide(Current,OtherObjs);
						else if (!OtherObjs->AllowReplaceTime && (OtherObjs->Type != PlayerDrone && OtherObjs->Type != PlayerDroneShield) || (Current->Type == CargoShip || Current->Type == TheMoon))
							if (Current != OtherObjs && OtherObjs->Type != TheMoon && OtherObjs->Type != CargoShip && OtherObjs->Type != CMTPowerup)
								if (DoesCollide(Current,OtherObjs))
									goto CollisionCheckStart;
						OtherObjs = OtherObjs->next;
					}
				}
				else if (Current->Type == PlayerDrone || Current->Type == PlayerDroneShield)
				{
					AnObject *OtherObjs = ObjectsList;
					while (OtherObjs)
					{
						if (!OtherObjs->AllowReplaceTime && (Current != OtherObjs && OtherObjs->Type != NormalPlayerShip && OtherObjs->Type != PlayerShipJumpable && OtherObjs->Type != PlayerShield && OtherObjs->Type != PlayerCloak && OtherObjs->Type != PlayerSuper && OtherObjs->Type != CMTPowerup && OtherObjs->Type != PlayerShipDisrupter))
							if (DoesCollide(Current,OtherObjs))
							{
								Current = Current->next;
								goto UpdateCollisions;
							}
						OtherObjs = OtherObjs->next;
					}
				}
				else if (Current->Type == UFO || Current->Type == UFOBoss)
				{
					AnObject *OtherObjs = ObjectsList;
					while (OtherObjs)
					{
						if (!OtherObjs->AllowReplaceTime && (OtherObjs->Type == Mine || OtherObjs->Type == MineBoss))
							if (DoesCollide(Current,OtherObjs))
								goto CollisionCheckStart;
						OtherObjs = OtherObjs->next;
					}
				}
				else if (Current->Type == CMTPowerup)
				{
					AnObject *OtherObjs = ObjectsList;
					while (OtherObjs)
					{
						if (!OtherObjs->AllowReplaceTime && (OtherObjs->Type == NormalPlayerShip || OtherObjs->Type == PlayerShield || OtherObjs->Type == PlayerCloak || OtherObjs->Type == PlayerSuper || OtherObjs->Type == PlayerShipJumpable || OtherObjs->Type == PlayerShipDisrupter))
							if (PickupPowerup(Current,OtherObjs))
								goto CollisionCheckStart;
						OtherObjs = OtherObjs->next;
					}
				}
			}
			Current = Current->next;
		}
	}
	float FindNearestCollidableObject(GameObject *Obj,const Vector3 &Position)
	{
		if (!ObjectsList)
			return 0.0f;
		float MinDist = 0;
		AnObject *Current = ObjectsList,*Closest = NULL;
		while (Current)
		{
			if (!Current->AllowReplaceTime && Obj != Current->Object && Current->Type != CMTPowerup && Current->Type != PlayerDrone && Current->Type != PlayerDroneShield)
			{
				float Dist = JmgUtility::SimpleDistance(Position,Commands->Get_Position(Current->Object))-Current->Size;
				if (!Closest)
				{
					Closest = Current;
					MinDist = Dist;
				}
				else if (Dist < MinDist)
				{
					Closest = Current;
					MinDist = Dist;
				}
			}
			Current = Current->next;
		}
		return MinDist;
	}
	int AllowShipSpawn(int PlayerNumber,Vector3 SpawnSpots[MaxGamePlayerCount])
	{
		float Distance[MaxGamePlayerCount];
		for (int x = 0;x < MaxGamePlayerCount;x++)
			Distance[x] = 9999999.0f;
		AnObject *Current = ObjectsList;
		while (Current)
		{
			if (!Current->AllowReplaceTime)
			{
				float Temp = JmgUtility::SimpleDistance(SpawnSpots[PlayerNumber],Commands->Get_Position(Current->Object));
				if (Current->Type == TheMoon && Temp < 2626.5625f)
				{
					Distance[PlayerNumber] = 0;
					break;
				}
				if (Current->Object && Temp < Distance[PlayerNumber])
					Distance[PlayerNumber] = Temp;
			}
			Current = Current->next;
		}
		if (Distance[PlayerNumber] >= 625)
			return PlayerNumber;
		for (int x = 0;x < MaxGamePlayerCount;x++)
		{
			Current = ObjectsList;
			if (PlayerNumber == x)
				continue;
			while (Current)
			{
				if (!Current->AllowReplaceTime)
				{
					float Temp = JmgUtility::SimpleDistance(SpawnSpots[x],Commands->Get_Position(Current->Object));
					if (Current->Type == TheMoon && Temp < 2626.5625f)
					{
						Distance[x] = 0;
						break;
					}
					if (Temp < Distance[x])
						Distance[x] = Temp;
				}
				Current = Current->next;
			}
			if (Distance[x] >= 625)
				return x;
		}
		return MaxGamePlayerCount;
	}
	int CountType(ObjectType Type)
	{
		int Count = 0;
		AnObject *Current = ObjectsList;
		while (Current)
		{
			if (Current->AllowReplaceTime <= 0 && Current->Type == Type)
				Count++;
			Current = Current->next;
		}
		return Count;
	}
	bool CheckIfLevelComplete()
	{
		AnObject *Current = ObjectsList;
		while (Current)
		{
			if (Current->AllowReplaceTime <= 0 && (Current->Type == Asteroid || Current->Type == UFOBoss || Current->Type == TheMoon || Current->Type == MineBoss))
				return false;
			Current = Current->next;
		}
		return true;
	}
	void ClearAllObjects()
	{
		AnObject *Current = ObjectsList,*Temp;
		ObjectsList = NULL;
		while (Current)
		{
			Temp = Current;
			if (!Current->AllowReplaceTime)
				Commands->Apply_Damage(Current->Object,99999.99f,"BlamoKiller",0);
			Current = Current->next;
			delete Temp;
		}
		if (ObjectsList)
			ClearAllObjects();
	}
	void GrantScoreToKiller(GameObject *killer,float Score,ObjectType Type,int Size = 0)
	{
		if (!killer || !Score)
			return;
		GameObject *Driver = Get_Vehicle_Driver(killer);
		if (!Commands->Is_A_Star(Driver) && !Commands->Is_A_Star(killer))
		{
			int PresetID = Commands->Get_Preset_ID(killer);
			if (PresetID == PLAYERDRONE_ID)
			{// Killer was a player's drone
				int ID = Commands->Get_ID(killer);
				Score *= 0.1f;
				for (int x = 0;x < MaxGamePlayerCount;x++)
					for (int y = 0;y < 2;y++)
						if (MiniGamePlayerControlSystem[x].Drones[y] == ID)
						{
							MiniGamePlayerControlSystem[x].DroneScore += Score;
							RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->DroneKills++;
							killer = Commands->Find_Object(MiniGamePlayerControlSystem[x].GamePlayerID);
							x = MaxGamePlayerCount+1;
							break;
						}
			}
			else if (PresetID == CARGOSHIPTURRET_ID)
			{// Killer was a Cargo Ships Turret
				int ID = Commands->Get_ID(killer);
				for (int x = 0;x < MaxGamePlayerCount;x++)
					for (int y = 0;y < 6;y++)
						if (MiniGamePlayerControlSystem[x].CargoShipIDs[y] == ID)
						{
							Score *= 0.05f;
							MiniGamePlayerControlSystem[x].CargoScore += Score;
							RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->CargoShipKills++;
							killer = Commands->Find_Object(MiniGamePlayerControlSystem[x].GamePlayerID);
							x = MaxGamePlayerCount+1;
							break;
						}
			}
			else if (PresetID == CARGOSHIP_ID)
			{// Killer was a Cargo Ship
				int ID = Commands->Get_ID(killer);
				for (int x = 0;x < MaxGamePlayerCount;x++)
					if (MiniGamePlayerControlSystem[x].CargoShipIDs[6] == ID)
					{
						Score *= 0.025f;
						MiniGamePlayerControlSystem[x].CargoScore += Score;
						RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->CargoShipKills++;
						killer = Commands->Find_Object(MiniGamePlayerControlSystem[x].GamePlayerID);
						break;
					}
			}
		}
		for (int x = 0;x < MaxGamePlayerCount;x++)
		{
			GameObject *Player = Commands->Find_Object(MiniGamePlayerControlSystem[x].GamePlayerID);
			if (!Player)
				continue;
			if (Player == killer)
			{
				if (MiniGamePlayerControlSystem[x].DoublePointsTime)
				{
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(Get_Player_ID(Player))->DoublePointsPoints += (int)Score;
					Score *= 2;
				}
				MiniGamePlayerControlSystem[x].Score += Score;
				Commands->Give_Points(Player,Score,false);
AddKilledObjctToPlayersScoreListSwitch:
				switch (Type)
				{
				case NormalPlayerShip:
				case PlayerShield:
				case PlayerCloak:
				case PlayerSuper:
				case PlayerShipJumpable:
				case PlayerShipDisrupter:
				case PlayerShipJumping:
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(Get_Player_ID(Player))->OtherPlayers++;break;
				case UFO:case UFOBoss:
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(Get_Player_ID(Player))->UFO++;break;
				case Mine:case MineBoss:
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(Get_Player_ID(Player))->Mine++;break;
				case Asteroid:
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(Get_Player_ID(Player))->TotalComets++;
					switch (Size)
					{
					case 0: RenCometBustersScoreControl.Get_Current_Player_Score_Node(Get_Player_ID(Player))->CometSmall++;break;
					case 1: RenCometBustersScoreControl.Get_Current_Player_Score_Node(Get_Player_ID(Player))->CometMedium++;break;
					case 2: RenCometBustersScoreControl.Get_Current_Player_Score_Node(Get_Player_ID(Player))->CometLarge++;break;
					default: RenCometBustersScoreControl.Get_Current_Player_Score_Node(Get_Player_ID(Player))->CometHuge++;break;
					}break;
				case TheMoon:
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(Get_Player_ID(Player))->TheMoon++;break;
				case CargoShip:
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(Get_Player_ID(Player))->CargoShip++;break;
				case PlayerDrone: case PlayerDroneShield:
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(Get_Player_ID(Player))->DronesKilled++;break;
				case CMTPowerup:break;
				}
				return;
			}
			if (Player == Driver)
			{// shield or ram kill
				if (MiniGamePlayerControlSystem[x].DoublePointsTime)
				{
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(Get_Player_ID(Player))->DoublePointsPoints += (int)Score;
					Score *= 2;
				}
				Score *= 0.5f;
				MiniGamePlayerControlSystem[x].Score += Score;
				RenCometBustersScoreControl.Get_Current_Player_Score_Node(Get_Player_ID(Player))->ObjectsRammed++;
				Commands->Give_Points(Player,Score,false);
				goto AddKilledObjctToPlayersScoreListSwitch;
			}
		}
	}
	AnObject *FindMineTarget(GameObject *obj,int *MinDistance)
	{
		Vector3 MyPos = Commands->Get_Position(obj);
		float NearestDistance = 9999999.9f;
		AnObject *Target = NULL;
		ObjectType TargetType = NormalPlayerShip;
		AnObject *Current = ObjectsList;
		while (Current)
		{
			if (!Current->AllowReplaceTime && Current->Object != obj && (Current->Type != PlayerCloak && Current->Type != Asteroid && Current->Type != TheMoon && Current->Type != CMTPowerup))
			{
				float TempDistance = JmgUtility::SimpleDistance(Commands->Get_Position(Current->Object),MyPos);
				if (TempDistance <= 22500)
				{
					if (!Target)
					{	
						Target = Current;
						TargetType = Current->Type;
						NearestDistance = TempDistance;
					}
					else
					{
						int TempPriority = PrioritizeTargets[Current->Type];
						if (TempPriority > PrioritizeTargets[TargetType])
						{
							Target = Current;
							TargetType = Current->Type;
							NearestDistance = TempDistance;
						}
						else if (TempPriority == PrioritizeTargets[TargetType])
						{
							if ((Current->Type == Mine || Current->Type == MineBoss) && TempDistance > NearestDistance)
							{
								Target = Current;
								TargetType = Current->Type;
								NearestDistance = TempDistance;
							}
							else if ((Current->Type != Mine && Current->Type != MineBoss) && TempDistance < NearestDistance)
							{
								Target = Current;
								TargetType = Current->Type;
								NearestDistance = TempDistance;
							}
						}
					}
				}
			}
			Current = Current->next;
		}
		if (TargetType == Mine || TargetType == MineBoss)
			*MinDistance = Commands->Get_Random_Int(2,6);
		if (Target)
			return Target;
		return NULL;
	}
	void KillInRange(GameObject *Player,const Vector3 &Position,float MinRange,float MaxRange)
	{
KillInRangeCheckStart:
		AnObject *Current = ObjectsList;
		while (Current)
		{
			if (!Current->AllowReplaceTime && (Current->Type == Asteroid || Current->Type == UFO || Current->Type == UFOBoss || Current->Type == PlayerDrone || Current->Type == Mine || Current->Type == MineBoss || Current->Type == PlayerCloak || Current->Type == NormalPlayerShip || Current->Type == PlayerShipJumpable || Current->Type == PlayerShipDisrupter))
			{
				float Dist = JmgUtility::SimpleDistance(Position,Commands->Get_Position(Current->Object));
				if (Dist <= MaxRange && Dist >= MinRange)
				{
					if (_stricmp(Get_Skin(Current->Object),"Blamo"))
					{
						SpecialApplyDamage(Current->Object,1.0f,"None",Player);
						goto KillInRangeCheckStart;
					}
				}
			}
			Current = Current->next;
		}
	}
	GameObject *FindObjectInRange(GameObject *Obj,const Vector3 &Position,float Range)
	{
		float MinDist = 0;
		AnObject *Current = ObjectsList,*Closest = NULL;
		while (Current)
		{
			if (!Current->AllowReplaceTime && Obj != Current->Object)
			{
				float Dist = JmgUtility::SimpleDistance(Position,Commands->Get_Position(Current->Object))-Current->Size;
				if (!Closest)
				{
					Closest = Current;
					MinDist = Dist;
				}
				else if (MinDist > Dist)
				{
					Closest = Current;
					MinDist = Dist;
				}
			}
			Current = Current->next;
		}
		if (!Closest || Range < MinDist)
			return NULL;
		return Closest->Object;
	}
	int FindDroneAvoidObjectInRange(GameObject *Obj,Vector3 *Position,float Range,float *_MinDist)
	{
		if (!ObjectsList)
			return 0;
		int ReturnID = 0;
		float MinDist = 0;
		AnObject *Current = ObjectsList;
		while (Current)
		{
			int TempID = Current->ObjectID;
			ReferencerClass Target = Current->Object;
			if (Target && !Current->AllowReplaceTime && (Obj != Target && Current->Type != PlayerDrone && Current->Type != PlayerDroneShield && Current->Type != CMTPowerup && Current->Type != NormalPlayerShip && Current->Type != PlayerShield && Current->Type != PlayerCloak && Current->Type != PlayerSuper && Current->Type != PlayerShipJumpable && Current->Type != PlayerShipDisrupter))
			{
				float Dist = JmgUtility::SimpleDistance(*Position,Commands->Get_Position(Target))-Current->Size;
				if (!ReturnID)
				{
					ReturnID = TempID;
					MinDist = Dist;
				}
				else if (MinDist > Dist)
				{
					ReturnID = TempID;
					MinDist = Dist;
				}
			}
			Current = Current->next;
		}
		*_MinDist = MinDist;
		if (Range < MinDist)
			return 0;
		return ReturnID;
	}
	GameObject *CheckForDroneTarget(GameObject *Obj,const Vector3 &Position,float *TargetSize)
	{
		if (!ObjectsList)
			return NULL;
		float MinDist = 0;
		AnObject *Current = ObjectsList,*Closest = NULL;
		while (Current)
		{
			if (!Current->AllowReplaceTime && Obj != Current->Object && Current->Type != TheMoon && Current->Type != CMTPowerup && Current->Type != CargoShip && Current->Type != PlayerDrone && Current->Type != PlayerDroneShield && Current->Type != NormalPlayerShip && Current->Type != PlayerShield && Current->Type != PlayerCloak && Current->Type != PlayerSuper && Current->Type != PlayerShipJumpable && Current->Type != PlayerShipDisrupter)
			{
				float Dist = JmgUtility::SimpleDistance(Position,Commands->Get_Position(Current->Object))-Current->Size;
				if (!Closest)
				{
					Closest = Current;
					MinDist = Dist;
				}
				else if (MinDist > Dist)
				{
					Closest = Current;
					MinDist = Dist;
				}
			}
			Current = Current->next;
		}
		if (!Closest || 2500 < MinDist)
			return NULL;
		*TargetSize = Closest->RealSize;
		return Closest->Object;
	}
};
int RenCometBustersGameObjects::PrioritizeTargets[17] = {2,2,0,1,0,3,2,0,2,0,0,1,1,3,2,0,2};

Vector3 Random_Map_Position()
{
	int Random = Commands->Get_Random_Int(0,2);
	return Vector3((!Random ? (Commands->Get_Random(0.0,1.0f) < 0.5 ? 1 : -1)*Commands->Get_Random(75.0f,80.0f) : Commands->Get_Random(-80.0f,80.0f)),(Random ? (Commands->Get_Random(0.0,1.0f) < 0.5 ? 1 : -1)*Commands->Get_Random(75.0f,80.0f) : Commands->Get_Random(-80.0f,80.0f)),0.5);
}

class JMG_CMTB_Main_Game_Control : public ScriptImpClass {
	enum MusicType{MusicNone = 0,MusicGameOver = 1,MusicNormal = 2,MusicBoss = 3};
	int NextLevelDelay;
	int UFOSpawnTime;
	unsigned int BabySit;
	MusicType CurrentMusic;
	int LastPlayerCount;
	int MaxPlayerCount;
	int LastCometCount;
	Vector3 SpawnLocations[MaxGamePlayerCount];
	unsigned int PlayerSOSBeaconDealy;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
	void Create_Player_Ship(GameObject *obj,int PlayerNumber);
	void DisableGun(GameObject *obj);
	void FadeMusic(const char *music);
public:
	static int MoonID;
	static int MoonHolderID;
	static bool GameInProgress;
	static unsigned int CMTBLevel;
	static unsigned int PlayerSOSBeaconID;
	static unsigned int DroneSwarmCount;
	static bool EndGameSwitchEnabled;
	static float DifficultyMultiplier;
	static float DifficultyMultiplierMine;
	static float DifficultyMultiplierInv;
};
int JMG_CMTB_Main_Game_Control::MoonID = 0;
int JMG_CMTB_Main_Game_Control::MoonHolderID = 0;
bool JMG_CMTB_Main_Game_Control::GameInProgress = false;
unsigned int JMG_CMTB_Main_Game_Control::CMTBLevel = 0;
unsigned int JMG_CMTB_Main_Game_Control::PlayerSOSBeaconID = 0;
unsigned int JMG_CMTB_Main_Game_Control::DroneSwarmCount = 0;
bool JMG_CMTB_Main_Game_Control::EndGameSwitchEnabled = true;
float JMG_CMTB_Main_Game_Control::DifficultyMultiplier = 1.0f;
float JMG_CMTB_Main_Game_Control::DifficultyMultiplierMine = 1.0f;
float JMG_CMTB_Main_Game_Control::DifficultyMultiplierInv = 1.0f;

class JMG_CMTB_UFO_Random_Fire_Control : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj, GameObject *damager);
	void Destroyed(GameObject *obj);
};

class JMG_CMTB_Ship_Control_Script : public ScriptImpClass {
	bool prepingHyperjump;
	int EngineDisabled;
	Vector3 OldPos;
	int PortalInID;
	int PortalOutID;
	int Power;
	int LastBulletCount;
	int ForfeitLives;
	AnObject *MyObject;
	char Animation[32];
	float ShieldLightUp;
	bool OverPoweredWeapons;
	unsigned short ChangingWeapons;
	unsigned int RapidPowerRegen;
	unsigned int ExtraShotActive;
	bool RemoveExtraShot;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj, GameObject *damager);
	void Destroyed(GameObject *obj);
	void ShieldOff(GameObject *obj);
	void CloakOff(GameObject *obj);
	bool DrainPower(GameObject *obj,int Drain = 1);
	void ReturnToNormalState(GameObject *obj);
};

class JMG_CMTB_Comet_Script : public ScriptImpClass {
	void Created(GameObject *obj);
	void Killed(GameObject *obj, GameObject *damager);
	void Destroyed(GameObject *obj);
};

class JMG_CMTB_Zone_Realign_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_CMTB_Spectator_Object : public ScriptImpClass {
	int machineNumber;
	int RequestLifeDelay;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void SpecialAbilitiesList(GameObject *player);
	void ResetCurrentAbility(GameObject *player);
};

class JMG_CMTB_Arcade_Machine : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
};

class JMG_CMTB_Fake_Player_Attach_Script : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj, GameObject *damager);
};

class JMG_CMTB_Renegade_Code : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_CMTB_Motion_Mine : public ScriptImpClass {
	AnObject *LastTargetObject;
	Vector3 lastTargetSpot;
	bool movingToLastSeenSpot;
	bool NewRandom;
	float Speed;
	int FollowObjectID;
	Vector3 LastPosition;
	int ArriveDistance;
	Vector3 TargetPos;
	unsigned int NewTarget;
	int StuckCount;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj, GameObject *damager);
	void Destroyed(GameObject *obj);
	float Clamp_Value(float Value,float Min,float Max);
};

class JMG_CMTB_The_Moon_Script : public ScriptImpClass {
	bool SpawnPoints[16];
	float lastHealth;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj, GameObject *damager);
	void Destroyed(GameObject *obj);
	bool EnableSpawnPositions(GameObject *obj);
};

class JMG_CMTB_Ship_Super_Weapon : public ScriptImpClass {
	float LastDistance;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_CMTB_Powerup_Script : public ScriptImpClass {
	int TimeRemaining;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj, GameObject *damager);
	void Destroyed(GameObject *obj);
};

class JMG_CMTB_Cargo_Ship_Script : public ScriptImpClass {
	int ShipCollideID[7];
	int AnimationControllerID;
	int Turrets[6];
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj, GameObject *damager);
	void Destroyed(GameObject *obj);
	void CreateShipCollision(GameObject *obj,int index,const char *boneName,float size);
};

class JMG_CMTB_Cargo_Ship_Turret : public ScriptImpClass {
	float MissAmount;
	bool PrimaryFire;
	int HuntingPlayer;
	float NearestDistance;
	int EnemyID;
	bool ReaimRequired;
	Vector3 LastPosition[3];
	int GiveUpTime;
	int FireWeapons;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
	float SafeDiv(float Number1,float Number2);
	void EndAttack(GameObject *obj);
	bool CheckIfInTargetableRange(GameObject *obj,const Vector3 &Pos);
};

class JMG_CMTB_Player_Drone : public ScriptImpClass {
	enum AI_State{DroneIdle=0,DroneGuarding=1,DroneAvoiding=2,DroneAttacking=3,DroneReturning=4};
	int AIStatePriorities[5];
	bool ShieldOverride;
	AnObject *MyObject;
	AI_State MyState;
	AI_State LastState;
	Vector3 LastPosition;
	Vector3 TargetPos;
	int TargetID;
	int LastTargetID;
	int StuckCount;
	int NewTargetTime;
	int AvoidObjectID;
	int StateTimeOutTime;
	int SearchTimeDelay;
	int Power;
	float AvoidDegree;
	float AvoidDistance;
	bool PrimaryFire;
	bool LastPrimaryFire;
	float FollowAngle;
	int ExtremeStuckCount;
	float TargetSize;
	float CurrentSpeed;
	float ShieldLightUp;
	char myAnimation[32];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj, GameObject *damager);
	void Destroyed(GameObject *obj);
	float Clamp_Value(float Value,float Min,float Max);
	bool ChangeState(AI_State NewState);
	void Drone_Action_Complete(GameObject *obj);
	void AI_Update(GameObject *obj,bool Reset = false);
	void Drone_Attack(GameObject *obj);
	void Position_At_Player(GameObject *obj);
	void ShieldOff(GameObject *obj);
	void ShieldOn(GameObject *obj);
public:
	JMG_CMTB_Player_Drone()
	{
		MyObject = NULL;
	}
};

class JMG_Create_Ship_On_Poke : public ScriptImpClass {
	int ShipID;
	int YourID;
	const char *armor;
	int BotID;
	int cango;
	int ViewingBoxID;
	const char *Your3DModel;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	//void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_Advanced_Bounce_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Ship_Random_Hyperspace : public ScriptImpClass {
	const char *OriginalModel;
	int Has_Recived_Message;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_Random_Music : public ScriptImpClass {
	int songnumber;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};


class JMG_Ship_Weapon : public ScriptImpClass {
	int newtotal;
	int Ammo;
	int cangainammo;
	int Currentammo;
	int ammocheck;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_Return_Custom_When_Shot : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_Cronie_AI : public ScriptImpClass {
	int target;
	void Created(GameObject *obj);
	//void Action_Complete(GameObject *obj,int action,ActionCompleteReason reason);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Kill_Self_And_Sender_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Ship_Shield_Control_Script : public ScriptImpClass {
	int ison;
	int ObjID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Killed(GameObject *obj, GameObject *damager);
	//void Timer_Expired(GameObject *obj,int number);
};

class JMG_Kill_Self_And_Sender_On_Custom_Ship : public ScriptImpClass {
	int canbekilled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Ship_Shield_Script : public ScriptImpClass {
	const char *ShieldModel;
	int LastSenderID;
	int ReccentlyDamaged;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_Renetbusters_Object : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_New_Ship_Weapon_Control_System : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_Comet_Control_Script : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj, GameObject *damager);
};

class JMG_Attach_And_Remove : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};


class JMG_User_Auto_Enter_Ship : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_Apply_Damage_On_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_CMTB_Vehicle_Face_Turret : public ScriptImpClass {
	int davesArrowId;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Destroyed(GameObject *obj);
};

class JMG_CMTB_Poke_End_Map : public ScriptImpClass {
	int time;
	int startDelay;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	void FlipSwitch(GameObject *obj,float frame);
};

class JMG_CMTB_Poke_Change_Difficulty : public ScriptImpClass {
	int difficulty;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
};

class JMG_CMTB_Cargo_Ship_Child_Script : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *damager);
};