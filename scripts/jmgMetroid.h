/*	Renegade Roll Play 2 Scripts by Jerad Gray
	Scripts by Jerad Gray
	Copyright 2010 Jerad Gray

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#pragma once
#include "jmgUtility.h"
#include "direct.h"
#define PI 3.14159265f
#define PI180 PI/180

NewObjectiveSystem MetroidObjectiveSystemControl = NewObjectiveSystem(1,true,"mObjective","null","null");
NewObjectiveSystem MetroidRoomObjectives = NewObjectiveSystem(1,false,"mObjective","null","null");

bool PerkSystemIncreasePlayerPerkUnlockAmount(GameObject *obj,int PerkID,unsigned int IncreaseAmount);
void AddPerkTypes();
void SavePlayerPerkData(){};
void FindPlayerPerkList(GameObject *obj,bool Display){};
void ClearPlayerSelectedPerks(GameObject *obj){};
bool CheckIfPlayerHasPerkUnlocked(GameObject *obj,int PerkID);
void IncreasePlayerPerkSlotCount(GameObject *poker){};
struct MetroidScoreSystem
{
	bool hasLoaded;
	int selectRandomMatchingScore;
	bool saveInProgress;
	#define MetroidHIGHSCORELISTCOUNT 155
	char metroidSavePath[256];
public:
	struct MetroidHighScoresNode
	{
		char PlayerName[256];
		double PlayTime;
		double IdleTime;
		double Points;
		double MostPointsInARound;
		double HealthPackPoints;
		double RechargePoints;
		double DefenseGunPoints;
		double TeleportPoints;
		double ShieldUpgradePoints;
		double WeaponSpawnPoints;
		unsigned long Deaths;
		unsigned long DeathFromElectrifiedWater;
		unsigned long DeathFromSpikePit;
		unsigned long DeathFromFallingOffTheTrain;
		unsigned long DeathInLava;
		unsigned long DeathInRadioactivePit;
		unsigned long DeathFromReactorRadiation;
		unsigned long DeathInFire;
		unsigned long DeathFromMiniBoss;
		unsigned long KilledMiniBoss;
		unsigned long DeathFromDefenseCameras;
		unsigned long DeathFromAliens;
		unsigned long DeathFromRobots;
		unsigned long DeathFromTraitors;
		unsigned long DeathFromAttackCameras;
		unsigned long DeathFromGunEmplacements;
		unsigned long DeathFromShredderTurret;
		unsigned long DeathFromBansheeBoss;
		unsigned long DeathFromMineBoss;
		unsigned long DeathFromMineBossSquish;
		unsigned long DeathFromMineBossResources;
		unsigned long Kills;
		unsigned long Suicides;
		unsigned long KilledPlayers;
		unsigned long KilledDefenseCameras;
		unsigned long KilledAliens;
		unsigned long KilledRobots;
		unsigned long KilledTraitors;
		unsigned long KilledSecurityCameras;
		unsigned long KilledAttackCameras;
		unsigned long KilledGunEmplacements;
		unsigned long KilledTurretBoss;
		unsigned long KilledBansheeBoss;
		unsigned long KilledMineBoss;
		unsigned long Chapter1Completed;
		unsigned long Chapter2Completed;
		unsigned long Chapter3Completed;
		unsigned long Chapter4Completed;
		unsigned long Chapter5Completed;
		unsigned long Chapter6Completed;
		unsigned long Chapter7Completed;
		unsigned long Chapter8Completed;
		unsigned long Chapter9Completed;
		unsigned long Chapter10Completed;
		unsigned long TriggeredLockdown0;
		unsigned long TriggeredLockdown1;
		unsigned long TriggeredLockdown2;
		unsigned long TriggeredLockdown3;
		unsigned long TriggeredLockdown4;
		unsigned long TriggeredLockdown5;
		unsigned long TriggeredLockdown6;
		unsigned long TriggeredLockdown7;
		unsigned long TriggeredLockdown8;
		unsigned long TriggeredLockdown9;
		unsigned long PickedUpPlayerPlacedWeapon;
		unsigned long PickedUpLaser;
		unsigned long PickedUpShotgun;
		unsigned long PickedUpSniper;
		unsigned long PickedUpFlame;
		unsigned long PickedUpShockCoil;
		unsigned long PickedUpMachineGun;
		unsigned long PickedUpRoboticLaser;
		unsigned long PickedUpRoboticShotgun;
		unsigned long PickedUpRoboticSniper;
		unsigned long PickedUpRoboticFlame;
		unsigned long PickedUpRoboticShockCoil;
		unsigned long PickedUpRoboticMachineGun;
		unsigned long PickedUpGlitchGun;
		unsigned long PickedUpHealthPack;
		unsigned long PickedUpPlayerPlacedHealthPack;
		unsigned long PickedUpShieldUpgrade;
		unsigned long PickedUpPlayerShieldUpgrade;
		unsigned long PlacedMedPacks;
		unsigned long PlacedRechargers;
		unsigned long PlacedTurrets;
		unsigned long PlacedTeleports;
		unsigned long PlacedShieldUpgrades;
		unsigned long PlacedWeaponSpawners;
		unsigned long UsedPlayerPlacedTeleport;
		double PlayTimeAsClass0;
		double PlayTimeAsClass1;//Armor
		double PlayTimeAsClass2;//Speed
		double PlayTimeAsClass3;//FirePower
		double PlayTimeAsClass4;//Support
		double PlayTimeAsClass5;//Defense
		double PlayTimeAsClass6;//Supply
		double PlayTimeAsClass7;
		double PlayTimeAsClass8;
		double PlayTimeAsClass9;
		double PlayTimeAsClass10;//Cheat
		double PlayTimeAsClass11;//Training
		double PlayTimeInSpawnRoom;
		unsigned long CompletedPlayThroughs;
		unsigned long DeathFromPlayers;
		double LongestPlayTimeInARound;
		unsigned long SpawnedAsCharacter0;
		unsigned long SpawnedAsCharacter1;
		unsigned long SpawnedAsCharacter2;
		unsigned long SpawnedAsCharacter3;
		unsigned long SpawnedAsCharacter4;
		unsigned long SpawnedAsCharacter5;
		unsigned long SpawnedAsCharacter6;
		unsigned long SpawnedAsCharacter7;
		unsigned long SpawnedAsCharacter8;
		unsigned long SpawnedAsCharacter9;
		unsigned long KilledCrates;
		unsigned long PickedUpGrenadeLaunchers;
		unsigned long ElevatorButtonsPressed;
		unsigned long ElevatorLockdownButton;
		unsigned long KilledBrokenDoors;
		unsigned long GlitchModeFound;
		unsigned long GlitchModeCompleted;
		unsigned long FoundCheatGun;
		unsigned long OpenedFirstDoor;
		unsigned long GotKeycard;
		unsigned long KilledByRapids;
		unsigned long KilledByFall;
		unsigned long TurnedOffLaserFence;
		unsigned long BackLaserFence;
		unsigned long ReactorPowerRestored;
		unsigned long ComputerConsolesDeactivated;
		unsigned long CityShieldLowered;
		unsigned long ElectrifiedWaterTurnedOff;
		unsigned long TunnelEntryLowered;
		unsigned long KillsWithDefenseGun;
		unsigned long KillsWithCheatGun;
		unsigned long KillsWithFlameUpgrade;
		unsigned long KillsWithRoboticFlameUpgrade;
		unsigned long KillsWithGlitchGun;
		unsigned long KillsWithImperialist;
		unsigned long KillsWithRoboticImperialist;
		unsigned long KillsWithLaserShotgun;
		unsigned long KillsWithRoboticLaserShotgun;
		unsigned long KillsWithMachineGun;
		unsigned long KillsWithRoboticMachineGun;
		unsigned long KillsWithShockCoil;
		unsigned long KillsWithRoboticShockCoil;
		unsigned long KillsWithLaser;
		unsigned long KillsWithRoboticLaser;
		unsigned long KillsWithHyperdeath;
		unsigned long KillsWithSuperHyperdeath;
		unsigned long KillsWithKnife;
		unsigned long KillsWithGrenade;
		unsigned long KillsWithRoboticGrenade;
		unsigned long KillsWithUnknownWeapon; 
		JmgUtility::GenericDateTime LastPlayTime;

		

		MetroidHighScoresNode *next;
		float LastFacing;
		Vector3 LastPos;
		bool IsMoving;
		bool IsTurning;
		unsigned int IdleDelay;
		double pointsThisRound;
		double playTimeThisRound;
		MetroidHighScoresNode(void)
		{
			PlayTime = 0.0;
			IdleTime = 0.0;
			Points = 0.0;
			MostPointsInARound = 0.0;
			HealthPackPoints = 0.0;
			RechargePoints = 0.0;
			DefenseGunPoints = 0.0;
			TeleportPoints = 0.0;
			ShieldUpgradePoints = 0.0;
			WeaponSpawnPoints = 0.0;
			Deaths = 0;
			DeathFromElectrifiedWater = 0;
			DeathFromSpikePit = 0;
			DeathFromFallingOffTheTrain = 0;
			DeathInLava = 0;
			DeathInRadioactivePit = 0;
			DeathFromReactorRadiation = 0;
			DeathInFire = 0;
			DeathFromMiniBoss = 0;
			KilledMiniBoss = 0;
			DeathFromDefenseCameras = 0;
			DeathFromAliens = 0;
			DeathFromRobots = 0;
			DeathFromTraitors = 0;
			DeathFromAttackCameras = 0;
			DeathFromGunEmplacements = 0;
			DeathFromShredderTurret = 0;
			DeathFromBansheeBoss = 0;
			DeathFromMineBoss = 0;
			DeathFromMineBossSquish = 0;
			DeathFromMineBossResources = 0;
			Kills = 0;
			Suicides = 0;
			KilledPlayers = 0;
			KilledDefenseCameras = 0;
			KilledAliens = 0;
			KilledRobots = 0;
			KilledTraitors = 0;
			KilledSecurityCameras = 0;
			KilledAttackCameras = 0;
			KilledGunEmplacements = 0;
			KilledTurretBoss = 0;
			KilledBansheeBoss = 0;
			KilledMineBoss = 0;
			Chapter1Completed = 0;
			Chapter2Completed = 0;
			Chapter3Completed = 0;
			Chapter4Completed = 0;
			Chapter5Completed = 0;
			Chapter6Completed = 0;
			Chapter7Completed = 0;
			Chapter8Completed = 0;
			Chapter9Completed = 0;
			Chapter10Completed = 0;
			TriggeredLockdown0 = 0;
			TriggeredLockdown1 = 0;
			TriggeredLockdown2 = 0;
			TriggeredLockdown3 = 0;
			TriggeredLockdown4 = 0;
			TriggeredLockdown5 = 0;
			TriggeredLockdown6 = 0;
			TriggeredLockdown7 = 0;
			TriggeredLockdown8 = 0;
			TriggeredLockdown9 = 0;
			PickedUpPlayerPlacedWeapon = 0;
			PickedUpLaser = 0;
			PickedUpShotgun = 0;
			PickedUpSniper = 0;
			PickedUpFlame = 0;
			PickedUpShockCoil = 0;
			PickedUpMachineGun = 0;
			PickedUpRoboticLaser = 0;
			PickedUpRoboticShotgun = 0;
			PickedUpRoboticSniper = 0;
			PickedUpRoboticFlame = 0;
			PickedUpRoboticShockCoil = 0;
			PickedUpRoboticMachineGun = 0;
			PickedUpGlitchGun = 0;
			PickedUpHealthPack = 0;
			PickedUpPlayerPlacedHealthPack = 0;
			PickedUpShieldUpgrade = 0;
			PickedUpPlayerShieldUpgrade = 0;
			PlacedMedPacks = 0;
			PlacedRechargers = 0;	
			PlacedTurrets = 0;
			PlacedTeleports = 0;
			PlacedShieldUpgrades = 0;
			PlacedWeaponSpawners = 0;
			UsedPlayerPlacedTeleport = 0;
			PlayTimeAsClass0 = 0.0;
			PlayTimeAsClass1 = 0.0;
			PlayTimeAsClass2 = 0.0;
			PlayTimeAsClass3 = 0.0;
			PlayTimeAsClass4 = 0.0;
			PlayTimeAsClass5 = 0.0;
			PlayTimeAsClass6 = 0.0;
			PlayTimeAsClass7 = 0.0;
			PlayTimeAsClass8 = 0.0;
			PlayTimeAsClass9 = 0.0;
			PlayTimeAsClass10 = 0.0;
			PlayTimeAsClass11 = 0.0;
			PlayTimeInSpawnRoom = 0.0;
			CompletedPlayThroughs = 0;
			DeathFromPlayers = 0;
			LongestPlayTimeInARound = 0.0;
			SpawnedAsCharacter0 = 0;
			SpawnedAsCharacter1 = 0;
			SpawnedAsCharacter2 = 0;
			SpawnedAsCharacter3 = 0;
			SpawnedAsCharacter4 = 0;
			SpawnedAsCharacter5 = 0;
			SpawnedAsCharacter6 = 0;
			SpawnedAsCharacter7 = 0;
			SpawnedAsCharacter8 = 0;
			SpawnedAsCharacter9 = 0;
			KilledCrates = 0;
			PickedUpGrenadeLaunchers = 0;
			ElevatorButtonsPressed = 0;
			ElevatorLockdownButton = 0;
			KilledBrokenDoors = 0;
			GlitchModeFound = 0;
			GlitchModeCompleted = 0;
			FoundCheatGun = 0;
			OpenedFirstDoor = 0;
			GotKeycard = 0;
			KilledByRapids = 0;
			KilledByFall = 0;
			TurnedOffLaserFence = 0;
			BackLaserFence = 0;
			ReactorPowerRestored = 0;
			ComputerConsolesDeactivated = 0;
			CityShieldLowered = 0;
			ElectrifiedWaterTurnedOff = 0;
			TunnelEntryLowered = 0;
			KillsWithDefenseGun= 0;
			KillsWithCheatGun= 0;
			KillsWithFlameUpgrade= 0;
			KillsWithRoboticFlameUpgrade= 0;
			KillsWithGlitchGun= 0;
			KillsWithImperialist= 0;
			KillsWithRoboticImperialist= 0;
			KillsWithLaserShotgun= 0;
			KillsWithRoboticLaserShotgun= 0;
			KillsWithMachineGun= 0;
			KillsWithRoboticMachineGun= 0;
			KillsWithShockCoil= 0;
			KillsWithRoboticShockCoil= 0;
			KillsWithLaser= 0;
			KillsWithRoboticLaser= 0;
			KillsWithHyperdeath= 0;
			KillsWithSuperHyperdeath= 0;
			KillsWithKnife= 0;
			KillsWithGrenade= 0;
			KillsWithRoboticGrenade= 0;
			KillsWithUnknownWeapon = 0;
			LastPlayTime = JmgUtility::GenericDateTime();


			next = NULL;
			LastPos = Vector3(0.0f,0.0f,0.0f);
			IdleDelay = 0;
			pointsThisRound = 0.0;
			playTimeThisRound = 0.0;
		}
	};
	MetroidHighScoresNode *MetroidPlayerHighScoreNodes[128];
private:
	MetroidHighScoresNode *MetroidHighScoresNodeList;
	MetroidHighScoresNode *MetroidHighScoresNodeEmptyNode;
	void LoadHighestScores()
	{
		if (!MetroidHighScoresNodeList)
			return;
		for (int x = 0;x < MetroidHIGHSCORELISTCOUNT;x++)
		{
			MetroidHighScoresNode *Current = MetroidHighScoresNodeList,*Best = NULL;
			while (Current)
			{
				Best = CompareHighScoreNodes(Best,Current,x);
				Current = Current->next;
			}
		}
	}
	MetroidHighScoresNode *FindOrAddPlayerMetroidHighScoreNode(const char *PlayerName)
	{
		if (!MetroidHighScoresNodeList)
		{
			MetroidHighScoresNodeList = new MetroidHighScoresNode();
			sprintf(MetroidHighScoresNodeList->PlayerName,"%s",PlayerName);
			return MetroidHighScoresNodeList;
		}
		MetroidHighScoresNode *Current = MetroidHighScoresNodeList;
		while (Current)
		{
			if (!_stricmp(Current->PlayerName,PlayerName))
				return Current;
			if (!Current->next)
			{
				Current->next = new MetroidHighScoresNode();
				Current = Current->next;
				sprintf(Current->PlayerName,"%s",PlayerName);
				return Current;
			}
			Current = Current->next;
		}
		return NULL;
	}
public:
	MetroidScoreSystem()
	{
		saveInProgress = false;
		hasLoaded = false;
		if (Exe != EXE_LEVELEDIT)
		{
			sprintf(metroidSavePath, "%s\\Save\\", Get_File_Path());
		}
		MetroidHighScoresNodeList = NULL;
		MetroidHighScoresNodeEmptyNode = NULL;
		for (int x = 0;x < 128;x++)
			MetroidPlayerHighScoreNodes[x] = NULL;
	};
	MetroidHighScoresNode *Get_Current_Player_Score_Node(int PlayerID)
	{
		const char *PlayerName = Get_Player_Name_By_ID(PlayerID);
		if (!PlayerName)
		{
			if (!MetroidHighScoresNodeEmptyNode)
				MetroidHighScoresNodeEmptyNode = new MetroidHighScoresNode();
			return MetroidHighScoresNodeEmptyNode;
		}
		if (!MetroidPlayerHighScoreNodes[PlayerID])
		{
			MetroidPlayerHighScoreNodes[PlayerID] = FindOrAddPlayerMetroidHighScoreNode(PlayerName);
		}
		else if (_stricmp(MetroidPlayerHighScoreNodes[PlayerID]->PlayerName,PlayerName))
		{
			MetroidPlayerHighScoreNodes[PlayerID] = FindOrAddPlayerMetroidHighScoreNode(PlayerName);
		}
		return MetroidPlayerHighScoreNodes[PlayerID];
	}
	void Cleanup()
	{
		if (saveInProgress)
		{
			Console_Input("msg MetroidScoreSystem ERROR: clean list, save in progress!");
			return;
		}
		hasLoaded = false;
		if (MetroidHighScoresNodeEmptyNode)
		{
			delete MetroidHighScoresNodeEmptyNode;
			MetroidHighScoresNodeEmptyNode = NULL;
		}
		for (int x = 0;x < 128;x++)
		{
			if (MetroidPlayerHighScoreNodes[x])
				MetroidPlayerHighScoreNodes[x] = NULL;
		}
		if (MetroidHighScoresNodeList)
		{
			MetroidHighScoresNode *temp,*die;
			temp = MetroidHighScoresNodeList;
			while (temp != NULL)
			{
				die = temp;
				temp = temp->next;
				delete die;
			}
			MetroidHighScoresNodeList = NULL;
		}
	}
	void SaveData()
	{
		if (!hasLoaded)
		{
			Console_Input("msg MetroidScoreSystem ERROR: cannot save!");
			return;
		}
		if (saveInProgress)
		{
			Console_Input("msg MetroidScoreSystem ERROR: cannot save, save already in progress!");
			return;
		}
		saveInProgress = true;
		FILE *SaveScores;
		FILE *SaveScores2;
		char tempFileName[256],finalFileName[256],textFileName[256],textTmpFileName[256];
		_mkdir(metroidSavePath);
		sprintf(finalFileName,"%sBetaMetroidPlayerRecords.Rp2",metroidSavePath);
		sprintf(textFileName,"%sMetroidPlayerRecords.txt",metroidSavePath);
		sprintf(textTmpFileName,"%sMetroidPlayerRecordsTmp.txt",metroidSavePath);
		sprintf(tempFileName,"%sBetaMetroidPlayerRecords.tmp",metroidSavePath);
		SaveScores = fopen(tempFileName,"w");
		SaveScores2 = fopen(textTmpFileName,"w");
		MetroidHighScoresNode *Current = MetroidHighScoresNodeList;
		while (Current)
		{
			//CalculateKillToDeathRatio(Current);
			if (Current->MostPointsInARound < Current->pointsThisRound)
				Current->MostPointsInARound = Current->pointsThisRound;
			if (Current->LongestPlayTimeInARound < Current->playTimeThisRound)
				Current->LongestPlayTimeInARound = Current->playTimeThisRound;
			char EncryptString[4096];
 			sprintf(EncryptString,"%.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %lu %lu %.2lf %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d %d %d %d %d %lu 0",Current->PlayTime,Current->IdleTime,Current->Points,Current->MostPointsInARound,Current->HealthPackPoints,Current->RechargePoints,Current->DefenseGunPoints,Current->TeleportPoints,Current->ShieldUpgradePoints,Current->WeaponSpawnPoints,Current->Deaths,Current->DeathFromElectrifiedWater,Current->DeathFromSpikePit,Current->DeathFromFallingOffTheTrain,Current->DeathInLava,Current->DeathInRadioactivePit,Current->DeathFromReactorRadiation,Current->DeathInFire,Current->DeathFromMiniBoss,Current->KilledMiniBoss,Current->DeathFromDefenseCameras,Current->DeathFromAliens,Current->DeathFromRobots,Current->DeathFromTraitors,Current->DeathFromAttackCameras,Current->DeathFromGunEmplacements,Current->DeathFromShredderTurret,Current->DeathFromBansheeBoss,Current->DeathFromMineBoss,Current->DeathFromMineBossSquish,Current->DeathFromMineBossResources,Current->Kills,Current->Suicides,Current->KilledPlayers,Current->KilledDefenseCameras,Current->KilledAliens,Current->KilledRobots,Current->KilledTraitors,Current->KilledSecurityCameras,Current->KilledAttackCameras,Current->KilledGunEmplacements,Current->KilledTurretBoss,Current->KilledBansheeBoss,Current->KilledMineBoss,Current->Chapter1Completed,Current->Chapter2Completed,Current->Chapter3Completed,Current->Chapter4Completed,Current->Chapter5Completed,Current->Chapter6Completed,Current->Chapter7Completed,Current->Chapter8Completed,Current->Chapter9Completed,Current->Chapter10Completed,Current->TriggeredLockdown0,Current->TriggeredLockdown1,Current->TriggeredLockdown2,Current->TriggeredLockdown3,Current->TriggeredLockdown4,Current->TriggeredLockdown5,Current->TriggeredLockdown6,Current->TriggeredLockdown7,Current->TriggeredLockdown8,Current->TriggeredLockdown9,Current->PickedUpPlayerPlacedWeapon,Current->PickedUpLaser,Current->PickedUpShotgun,Current->PickedUpSniper,Current->PickedUpFlame,Current->PickedUpShockCoil,Current->PickedUpMachineGun,Current->PickedUpRoboticLaser,Current->PickedUpRoboticShotgun,Current->PickedUpRoboticSniper,Current->PickedUpRoboticFlame,Current->PickedUpRoboticShockCoil,Current->PickedUpRoboticMachineGun,Current->PickedUpGlitchGun,Current->PickedUpHealthPack,Current->PickedUpPlayerPlacedHealthPack,Current->PickedUpShieldUpgrade,Current->PickedUpPlayerShieldUpgrade,Current->PlacedMedPacks,Current->PlacedRechargers,Current->PlacedTurrets,Current->PlacedTeleports,Current->PlacedShieldUpgrades,Current->PlacedWeaponSpawners,Current->UsedPlayerPlacedTeleport,Current->PlayTimeAsClass0,Current->PlayTimeAsClass1,Current->PlayTimeAsClass2,Current->PlayTimeAsClass3,Current->PlayTimeAsClass4,Current->PlayTimeAsClass5,Current->PlayTimeAsClass6,Current->PlayTimeAsClass7,Current->PlayTimeAsClass8,Current->PlayTimeAsClass9,Current->PlayTimeAsClass10,Current->PlayTimeAsClass11,Current->PlayTimeInSpawnRoom,Current->CompletedPlayThroughs,Current->DeathFromPlayers,Current->LongestPlayTimeInARound,Current->SpawnedAsCharacter0,Current->SpawnedAsCharacter1,Current->SpawnedAsCharacter2,Current->SpawnedAsCharacter3,Current->SpawnedAsCharacter4,Current->SpawnedAsCharacter5,Current->SpawnedAsCharacter6,Current->SpawnedAsCharacter7,Current->SpawnedAsCharacter8,Current->SpawnedAsCharacter9,Current->KilledCrates,Current->PickedUpGrenadeLaunchers,Current->ElevatorButtonsPressed,Current->ElevatorLockdownButton,Current->KilledBrokenDoors,Current->GlitchModeFound,Current->GlitchModeCompleted,Current->FoundCheatGun,Current->OpenedFirstDoor,Current->GotKeycard,Current->KilledByRapids,Current->KilledByFall,Current->TurnedOffLaserFence,Current->BackLaserFence,Current->ReactorPowerRestored,Current->ComputerConsolesDeactivated,Current->CityShieldLowered,Current->ElectrifiedWaterTurnedOff,Current->TunnelEntryLowered,Current->KillsWithDefenseGun,Current->KillsWithCheatGun,Current->KillsWithFlameUpgrade,Current->KillsWithRoboticFlameUpgrade,Current->KillsWithGlitchGun,Current->KillsWithImperialist,Current->KillsWithRoboticImperialist,Current->KillsWithLaserShotgun,Current->KillsWithRoboticLaserShotgun,Current->KillsWithMachineGun,Current->KillsWithRoboticMachineGun,Current->KillsWithShockCoil,Current->KillsWithRoboticShockCoil,Current->KillsWithLaser,Current->KillsWithRoboticLaser,Current->KillsWithHyperdeath,Current->KillsWithSuperHyperdeath,Current->KillsWithKnife,Current->KillsWithGrenade,Current->KillsWithRoboticGrenade,Current->KillsWithUnknownWeapon,Current->LastPlayTime.day,Current->LastPlayTime.month,Current->LastPlayTime.year,Current->LastPlayTime.second,Current->LastPlayTime.minute,Current->LastPlayTime.hour,Current->LastPlayTime.lTime);
			fprintf(SaveScores2,"%s\n%s\n",Current->PlayerName,EncryptString);
			fprintf(SaveScores,"%s\n%s",JmgUtility::Rp2Encrypt(Current->PlayerName,25,5),JmgUtility::Rp2Encrypt2(EncryptString,Current->PlayerName[0],Current->PlayerName[1]));
			if (Current->next)
				fprintf(SaveScores,"\n");
			Current = Current->next;
		}
		fclose(SaveScores);
		fclose(SaveScores2);
		remove(finalFileName);
		rename(tempFileName,finalFileName);
		remove(textFileName);
		rename(textTmpFileName,textFileName);
		saveInProgress = false;
	}
	void LoadData()
	{
		hasLoaded = true;
		if (!MetroidHighScoresNodeEmptyNode)
			MetroidHighScoresNodeEmptyNode = new MetroidHighScoresNode();
		for (int x = 0;x < 128;x++)
			MetroidPlayerHighScoreNodes[x] = MetroidHighScoresNodeEmptyNode;
		FILE *LoadScores;
		char FileName[64];
		sprintf(FileName,"%sBetaMetroidPlayerRecords.Rp2",metroidSavePath);
		LoadScores = fopen(FileName,"r");
		if (!LoadScores)
			return;
		while (!feof(LoadScores))
		{
			char PlayerName[256];
			fgets(PlayerName,256,LoadScores);
			int Length = strlen(PlayerName);
			if (Length <= 0)
			{
				Console_Input("msg MutantAssaultHighScoreSystem::LoadData ERROR: Length is less than 1");
				continue;
			}
			PlayerName[Length-1] = '\0';
			MetroidHighScoresNode *Current = FindOrAddPlayerMetroidHighScoreNode(JmgUtility::Rp2Decrypt(PlayerName,25,5));
			char DecryptString[4096];
			fgets(DecryptString,4096,LoadScores);
			sscanf(JmgUtility::Rp2Decrypt(DecryptString,Current->PlayerName[0],Current->PlayerName[1]),"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lu %lu %lf %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d %d %d %d %d %lu ",&Current->PlayTime,&Current->IdleTime,&Current->Points,&Current->MostPointsInARound,&Current->HealthPackPoints,&Current->RechargePoints,&Current->DefenseGunPoints,&Current->TeleportPoints,&Current->ShieldUpgradePoints,&Current->WeaponSpawnPoints,&Current->Deaths,&Current->DeathFromElectrifiedWater,&Current->DeathFromSpikePit,&Current->DeathFromFallingOffTheTrain,&Current->DeathInLava,&Current->DeathInRadioactivePit,&Current->DeathFromReactorRadiation,&Current->DeathInFire,&Current->DeathFromMiniBoss,&Current->KilledMiniBoss,&Current->DeathFromDefenseCameras,&Current->DeathFromAliens,&Current->DeathFromRobots,&Current->DeathFromTraitors,&Current->DeathFromAttackCameras,&Current->DeathFromGunEmplacements,&Current->DeathFromShredderTurret,&Current->DeathFromBansheeBoss,&Current->DeathFromMineBoss,&Current->DeathFromMineBossSquish,&Current->DeathFromMineBossResources,&Current->Kills,&Current->Suicides,&Current->KilledPlayers,&Current->KilledDefenseCameras,&Current->KilledAliens,&Current->KilledRobots,&Current->KilledTraitors,&Current->KilledSecurityCameras,&Current->KilledAttackCameras,&Current->KilledGunEmplacements,&Current->KilledTurretBoss,&Current->KilledBansheeBoss,&Current->KilledMineBoss,&Current->Chapter1Completed,&Current->Chapter2Completed,&Current->Chapter3Completed,&Current->Chapter4Completed,&Current->Chapter5Completed,&Current->Chapter6Completed,&Current->Chapter7Completed,&Current->Chapter8Completed,&Current->Chapter9Completed,&Current->Chapter10Completed,&Current->TriggeredLockdown0,&Current->TriggeredLockdown1,&Current->TriggeredLockdown2,&Current->TriggeredLockdown3,&Current->TriggeredLockdown4,&Current->TriggeredLockdown5,&Current->TriggeredLockdown6,&Current->TriggeredLockdown7,&Current->TriggeredLockdown8,&Current->TriggeredLockdown9,&Current->PickedUpPlayerPlacedWeapon,&Current->PickedUpLaser,&Current->PickedUpShotgun,&Current->PickedUpSniper,&Current->PickedUpFlame,&Current->PickedUpShockCoil,&Current->PickedUpMachineGun,&Current->PickedUpRoboticLaser,&Current->PickedUpRoboticShotgun,&Current->PickedUpRoboticSniper,&Current->PickedUpRoboticFlame,&Current->PickedUpRoboticShockCoil,&Current->PickedUpRoboticMachineGun,&Current->PickedUpGlitchGun,&Current->PickedUpHealthPack,&Current->PickedUpPlayerPlacedHealthPack,&Current->PickedUpShieldUpgrade,&Current->PickedUpPlayerShieldUpgrade,&Current->PlacedMedPacks,&Current->PlacedRechargers,&Current->PlacedTurrets,&Current->PlacedTeleports,&Current->PlacedShieldUpgrades,&Current->PlacedWeaponSpawners,&Current->UsedPlayerPlacedTeleport,&Current->PlayTimeAsClass0,&Current->PlayTimeAsClass1,&Current->PlayTimeAsClass2,&Current->PlayTimeAsClass3,&Current->PlayTimeAsClass4,&Current->PlayTimeAsClass5,&Current->PlayTimeAsClass6,&Current->PlayTimeAsClass7,&Current->PlayTimeAsClass8,&Current->PlayTimeAsClass9,&Current->PlayTimeAsClass10,&Current->PlayTimeAsClass11,&Current->PlayTimeInSpawnRoom,&Current->CompletedPlayThroughs,&Current->DeathFromPlayers,&Current->LongestPlayTimeInARound,&Current->SpawnedAsCharacter0,&Current->SpawnedAsCharacter1,&Current->SpawnedAsCharacter2,&Current->SpawnedAsCharacter3,&Current->SpawnedAsCharacter4,&Current->SpawnedAsCharacter5,&Current->SpawnedAsCharacter6,&Current->SpawnedAsCharacter7,&Current->SpawnedAsCharacter8,&Current->SpawnedAsCharacter9,&Current->KilledCrates,&Current->PickedUpGrenadeLaunchers,&Current->ElevatorButtonsPressed,&Current->ElevatorLockdownButton,&Current->KilledBrokenDoors,&Current->GlitchModeFound,&Current->GlitchModeCompleted,&Current->FoundCheatGun,&Current->OpenedFirstDoor,&Current->GotKeycard,&Current->KilledByRapids,&Current->KilledByFall,&Current->TurnedOffLaserFence,&Current->BackLaserFence,&Current->ReactorPowerRestored,&Current->ComputerConsolesDeactivated,&Current->CityShieldLowered,&Current->ElectrifiedWaterTurnedOff,&Current->TunnelEntryLowered,&Current->KillsWithDefenseGun,&Current->KillsWithCheatGun,&Current->KillsWithFlameUpgrade,&Current->KillsWithRoboticFlameUpgrade,&Current->KillsWithGlitchGun,&Current->KillsWithImperialist,&Current->KillsWithRoboticImperialist,&Current->KillsWithLaserShotgun,&Current->KillsWithRoboticLaserShotgun,&Current->KillsWithMachineGun,&Current->KillsWithRoboticMachineGun,&Current->KillsWithShockCoil,&Current->KillsWithRoboticShockCoil,&Current->KillsWithLaser,&Current->KillsWithRoboticLaser,&Current->KillsWithHyperdeath,&Current->KillsWithSuperHyperdeath,&Current->KillsWithKnife,&Current->KillsWithGrenade,&Current->KillsWithRoboticGrenade,&Current->KillsWithUnknownWeapon,&Current->LastPlayTime.day,&Current->LastPlayTime.month,&Current->LastPlayTime.year,&Current->LastPlayTime.second,&Current->LastPlayTime.minute,&Current->LastPlayTime.hour,&Current->LastPlayTime.lTime);
		}
		fclose(LoadScores);	
		LoadHighestScores();
	}
	inline MetroidHighScoresNode *EveluateHighestScore(double highValue,double currentValue,MetroidHighScoresNode *High,MetroidHighScoresNode *Current)
	{
		if (highValue == currentValue)
		{
			selectRandomMatchingScore++;
			if (Commands->Get_Random(0.0f,(float)selectRandomMatchingScore) < 0.5)
				return Current;
		}
		else if (highValue < currentValue)
			selectRandomMatchingScore = 0;
		return (highValue >= currentValue ? High : Current);
	}
	inline MetroidHighScoresNode *EveluateHighestScore(unsigned long highValue,unsigned long currentValue,MetroidHighScoresNode *High,MetroidHighScoresNode *Current)
	{
		if (highValue == currentValue)
		{
			selectRandomMatchingScore++;
			if (Commands->Get_Random(0.0f,(float)selectRandomMatchingScore) < 0.5)
				return Current;
		}
		else if (highValue < currentValue)
			selectRandomMatchingScore = 0;
		return (highValue >= currentValue ? High : Current);
	}
	MetroidHighScoresNode *CompareHighScoreNodes(MetroidHighScoresNode *High,MetroidHighScoresNode *Current,int CompVal)
	{
		if (!High)
			return Current;
		switch (CompVal)
		{
			case 0:return EveluateHighestScore(High->PlayTime,Current->PlayTime,High,Current);
			case 1:return EveluateHighestScore(High->IdleTime,Current->IdleTime,High,Current);
			case 2:return EveluateHighestScore(High->Points,Current->Points,High,Current);
			case 3:return EveluateHighestScore(High->MostPointsInARound,Current->MostPointsInARound,High,Current);
			case 4:return EveluateHighestScore(High->HealthPackPoints,Current->HealthPackPoints,High,Current);
			case 5:return EveluateHighestScore(High->RechargePoints,Current->RechargePoints,High,Current);
			case 6:return EveluateHighestScore(High->DefenseGunPoints,Current->DefenseGunPoints,High,Current);
			case 7:return EveluateHighestScore(High->TeleportPoints,Current->TeleportPoints,High,Current);
			case 8:return EveluateHighestScore(High->ShieldUpgradePoints,Current->ShieldUpgradePoints,High,Current);
			case 9:return EveluateHighestScore(High->WeaponSpawnPoints,Current->WeaponSpawnPoints,High,Current);
			case 10:return EveluateHighestScore(High->Deaths,Current->Deaths,High,Current);
			case 11:return EveluateHighestScore(High->DeathFromElectrifiedWater,Current->DeathFromElectrifiedWater,High,Current);
			case 12:return EveluateHighestScore(High->DeathFromSpikePit,Current->DeathFromSpikePit,High,Current);
			case 13:return EveluateHighestScore(High->DeathFromFallingOffTheTrain,Current->DeathFromFallingOffTheTrain,High,Current);
			case 14:return EveluateHighestScore(High->DeathInLava,Current->DeathInLava,High,Current);
			case 15:return EveluateHighestScore(High->DeathInRadioactivePit,Current->DeathInRadioactivePit,High,Current);
			case 16:return EveluateHighestScore(High->DeathFromReactorRadiation,Current->DeathFromReactorRadiation,High,Current);
			case 17:return EveluateHighestScore(High->DeathInFire,Current->DeathInFire,High,Current);
			case 18:return EveluateHighestScore(High->DeathFromMiniBoss,Current->DeathFromMiniBoss,High,Current);
			case 19:return EveluateHighestScore(High->KilledMiniBoss,Current->KilledMiniBoss,High,Current);
			case 20:return EveluateHighestScore(High->DeathFromDefenseCameras,Current->DeathFromDefenseCameras,High,Current);
			case 21:return EveluateHighestScore(High->DeathFromAliens,Current->DeathFromAliens,High,Current);
			case 22:return EveluateHighestScore(High->DeathFromRobots,Current->DeathFromRobots,High,Current);
			case 23:return EveluateHighestScore(High->DeathFromTraitors,Current->DeathFromTraitors,High,Current);
			case 24:return EveluateHighestScore(High->DeathFromAttackCameras,Current->DeathFromAttackCameras,High,Current);
			case 25:return EveluateHighestScore(High->DeathFromGunEmplacements,Current->DeathFromGunEmplacements,High,Current);
			case 26:return EveluateHighestScore(High->DeathFromShredderTurret,Current->DeathFromShredderTurret,High,Current);
			case 27:return EveluateHighestScore(High->DeathFromBansheeBoss,Current->DeathFromBansheeBoss,High,Current);
			case 28:return EveluateHighestScore(High->DeathFromMineBoss,Current->DeathFromMineBoss,High,Current);
			case 29:return EveluateHighestScore(High->DeathFromMineBossSquish,Current->DeathFromMineBossSquish,High,Current);
			case 30:return EveluateHighestScore(High->DeathFromMineBossResources,Current->DeathFromMineBossResources,High,Current);
			case 31:return EveluateHighestScore(High->Kills,Current->Kills,High,Current);
			case 32:return EveluateHighestScore(High->Suicides,Current->Suicides,High,Current);
			case 33:return EveluateHighestScore(High->KilledPlayers,Current->KilledPlayers,High,Current);
			case 34:return EveluateHighestScore(High->KilledDefenseCameras,Current->KilledDefenseCameras,High,Current);
			case 35:return EveluateHighestScore(High->KilledAliens,Current->KilledAliens,High,Current);
			case 36:return EveluateHighestScore(High->KilledRobots,Current->KilledRobots,High,Current);
			case 37:return EveluateHighestScore(High->KilledTraitors,Current->KilledTraitors,High,Current);
			case 38:return EveluateHighestScore(High->KilledSecurityCameras,Current->KilledSecurityCameras,High,Current);
			case 39:return EveluateHighestScore(High->KilledAttackCameras,Current->KilledAttackCameras,High,Current);
			case 40:return EveluateHighestScore(High->KilledGunEmplacements,Current->KilledGunEmplacements,High,Current);
			case 41:return EveluateHighestScore(High->KilledTurretBoss,Current->KilledTurretBoss,High,Current);
			case 42:return EveluateHighestScore(High->KilledBansheeBoss,Current->KilledBansheeBoss,High,Current);
			case 43:return EveluateHighestScore(High->KilledMineBoss,Current->KilledMineBoss,High,Current);
			case 44:return EveluateHighestScore(High->Chapter1Completed,Current->Chapter1Completed,High,Current);
			case 45:return EveluateHighestScore(High->Chapter2Completed,Current->Chapter2Completed,High,Current);
			case 46:return EveluateHighestScore(High->Chapter3Completed,Current->Chapter3Completed,High,Current);
			case 47:return EveluateHighestScore(High->Chapter4Completed,Current->Chapter4Completed,High,Current);
			case 48:return EveluateHighestScore(High->Chapter5Completed,Current->Chapter5Completed,High,Current);
			case 49:return EveluateHighestScore(High->Chapter6Completed,Current->Chapter6Completed,High,Current);
			case 50:return EveluateHighestScore(High->Chapter7Completed,Current->Chapter7Completed,High,Current);
			case 51:return EveluateHighestScore(High->Chapter8Completed,Current->Chapter8Completed,High,Current);
			case 52:return EveluateHighestScore(High->Chapter9Completed,Current->Chapter9Completed,High,Current);
			case 53:return EveluateHighestScore(High->Chapter10Completed,Current->Chapter10Completed,High,Current);
			case 54:return EveluateHighestScore(High->TriggeredLockdown0,Current->TriggeredLockdown0,High,Current);
			case 55:return EveluateHighestScore(High->TriggeredLockdown1,Current->TriggeredLockdown1,High,Current);
			case 56:return EveluateHighestScore(High->TriggeredLockdown2,Current->TriggeredLockdown2,High,Current);
			case 57:return EveluateHighestScore(High->TriggeredLockdown3,Current->TriggeredLockdown3,High,Current);
			case 58:return EveluateHighestScore(High->TriggeredLockdown4,Current->TriggeredLockdown4,High,Current);
			case 59:return EveluateHighestScore(High->TriggeredLockdown5,Current->TriggeredLockdown5,High,Current);
			case 60:return EveluateHighestScore(High->TriggeredLockdown6,Current->TriggeredLockdown6,High,Current);
			case 61:return EveluateHighestScore(High->TriggeredLockdown7,Current->TriggeredLockdown7,High,Current);
			case 62:return EveluateHighestScore(High->TriggeredLockdown8,Current->TriggeredLockdown8,High,Current);
			case 63:return EveluateHighestScore(High->TriggeredLockdown9,Current->TriggeredLockdown9,High,Current);
			case 64:return EveluateHighestScore(High->PickedUpPlayerPlacedWeapon,Current->PickedUpPlayerPlacedWeapon,High,Current);
			case 65:return EveluateHighestScore(High->PickedUpLaser,Current->PickedUpLaser,High,Current);
			case 66:return EveluateHighestScore(High->PickedUpShotgun,Current->PickedUpShotgun,High,Current);
			case 67:return EveluateHighestScore(High->PickedUpSniper,Current->PickedUpSniper,High,Current);
			case 68:return EveluateHighestScore(High->PickedUpFlame,Current->PickedUpFlame,High,Current);
			case 69:return EveluateHighestScore(High->PickedUpShockCoil,Current->PickedUpShockCoil,High,Current);
			case 70:return EveluateHighestScore(High->PickedUpMachineGun,Current->PickedUpMachineGun,High,Current);
			case 71:return EveluateHighestScore(High->PickedUpRoboticLaser,Current->PickedUpRoboticLaser,High,Current);
			case 72:return EveluateHighestScore(High->PickedUpRoboticShotgun,Current->PickedUpRoboticShotgun,High,Current);
			case 73:return EveluateHighestScore(High->PickedUpRoboticSniper,Current->PickedUpRoboticSniper,High,Current);
			case 74:return EveluateHighestScore(High->PickedUpRoboticFlame,Current->PickedUpRoboticFlame,High,Current);
			case 75:return EveluateHighestScore(High->PickedUpRoboticShockCoil,Current->PickedUpRoboticShockCoil,High,Current);
			case 76:return EveluateHighestScore(High->PickedUpRoboticMachineGun,Current->PickedUpRoboticMachineGun,High,Current);
			case 77:return EveluateHighestScore(High->PickedUpGlitchGun,Current->PickedUpGlitchGun,High,Current);
			case 78:return EveluateHighestScore(High->PickedUpHealthPack,Current->PickedUpHealthPack,High,Current);
			case 79:return EveluateHighestScore(High->PickedUpPlayerPlacedHealthPack,Current->PickedUpPlayerPlacedHealthPack,High,Current);
			case 80:return EveluateHighestScore(High->PickedUpShieldUpgrade,Current->PickedUpShieldUpgrade,High,Current);
			case 81:return EveluateHighestScore(High->PickedUpPlayerShieldUpgrade,Current->PickedUpPlayerShieldUpgrade,High,Current);
			case 82:return EveluateHighestScore(High->PlacedMedPacks,Current->PlacedMedPacks,High,Current);
			case 83:return EveluateHighestScore(High->PlacedRechargers,Current->PlacedRechargers,High,Current);
			case 84:return EveluateHighestScore(High->PlacedTurrets,Current->PlacedTurrets,High,Current);
			case 85:return EveluateHighestScore(High->PlacedTeleports,Current->PlacedTeleports,High,Current);
			case 86:return EveluateHighestScore(High->PlacedShieldUpgrades,Current->PlacedShieldUpgrades,High,Current);
			case 87:return EveluateHighestScore(High->PlacedWeaponSpawners,Current->PlacedWeaponSpawners,High,Current);
			case 88:return EveluateHighestScore(High->UsedPlayerPlacedTeleport,Current->UsedPlayerPlacedTeleport,High,Current);
			case 89:return EveluateHighestScore(High->PlayTimeAsClass0,Current->PlayTimeAsClass0,High,Current);
			case 90:return EveluateHighestScore(High->PlayTimeAsClass1,Current->PlayTimeAsClass1,High,Current);
			case 91:return EveluateHighestScore(High->PlayTimeAsClass2,Current->PlayTimeAsClass2,High,Current);
			case 92:return EveluateHighestScore(High->PlayTimeAsClass3,Current->PlayTimeAsClass3,High,Current);
			case 93:return EveluateHighestScore(High->PlayTimeAsClass4,Current->PlayTimeAsClass4,High,Current);
			case 94:return EveluateHighestScore(High->PlayTimeAsClass5,Current->PlayTimeAsClass5,High,Current);
			case 95:return EveluateHighestScore(High->PlayTimeAsClass6,Current->PlayTimeAsClass6,High,Current);
			case 96:return EveluateHighestScore(High->PlayTimeAsClass7,Current->PlayTimeAsClass7,High,Current);
			case 97:return EveluateHighestScore(High->PlayTimeAsClass8,Current->PlayTimeAsClass8,High,Current);
			case 98:return EveluateHighestScore(High->PlayTimeAsClass9,Current->PlayTimeAsClass9,High,Current);
			case 99:return EveluateHighestScore(High->PlayTimeAsClass10,Current->PlayTimeAsClass10,High,Current);
			case 100:return EveluateHighestScore(High->PlayTimeAsClass11,Current->PlayTimeAsClass11,High,Current);
			case 101:return EveluateHighestScore(High->CompletedPlayThroughs,Current->CompletedPlayThroughs,High,Current);
			case 102:return EveluateHighestScore(High->DeathFromPlayers,Current->DeathFromPlayers,High,Current); 
			case 103:return EveluateHighestScore(High->LongestPlayTimeInARound,Current->LongestPlayTimeInARound,High,Current);
			case 104:return EveluateHighestScore(High->SpawnedAsCharacter0,Current->SpawnedAsCharacter0,High,Current);
			case 105:return EveluateHighestScore(High->SpawnedAsCharacter1,Current->SpawnedAsCharacter1,High,Current);
			case 106:return EveluateHighestScore(High->SpawnedAsCharacter2,Current->SpawnedAsCharacter2,High,Current);
			case 107:return EveluateHighestScore(High->SpawnedAsCharacter3,Current->SpawnedAsCharacter3,High,Current);
			case 108:return EveluateHighestScore(High->SpawnedAsCharacter4,Current->SpawnedAsCharacter4,High,Current);
			case 109:return EveluateHighestScore(High->SpawnedAsCharacter5,Current->SpawnedAsCharacter5,High,Current);
			case 110:return EveluateHighestScore(High->SpawnedAsCharacter6,Current->SpawnedAsCharacter6,High,Current);
			case 111:return EveluateHighestScore(High->SpawnedAsCharacter7,Current->SpawnedAsCharacter7,High,Current);
			case 112:return EveluateHighestScore(High->SpawnedAsCharacter8,Current->SpawnedAsCharacter8,High,Current);
			case 113:return EveluateHighestScore(High->SpawnedAsCharacter9,Current->SpawnedAsCharacter9,High,Current);
			case 114:return EveluateHighestScore(High->KilledCrates,Current->KilledCrates,High,Current);
			case 115:return EveluateHighestScore(High->PickedUpGrenadeLaunchers,Current->PickedUpGrenadeLaunchers,High,Current);
			case 116:return EveluateHighestScore(High->ElevatorButtonsPressed,Current->ElevatorButtonsPressed,High,Current);
			case 117:return EveluateHighestScore(High->ElevatorLockdownButton,Current->ElevatorLockdownButton,High,Current);
			case 118:return EveluateHighestScore(High->KilledBrokenDoors,Current->KilledBrokenDoors,High,Current);
			case 119:return EveluateHighestScore(High->GlitchModeFound,Current->GlitchModeFound,High,Current);
			case 120:return EveluateHighestScore(High->GlitchModeCompleted,Current->GlitchModeCompleted,High,Current);
			case 121:return EveluateHighestScore(High->FoundCheatGun,Current->FoundCheatGun,High,Current);
			case 122:return EveluateHighestScore(High->OpenedFirstDoor,Current->OpenedFirstDoor,High,Current);
			case 123:return EveluateHighestScore(High->GotKeycard,Current->GotKeycard,High,Current);
			case 124:return EveluateHighestScore(High->KilledByRapids,Current->KilledByRapids,High,Current);
			case 125:return EveluateHighestScore(High->KilledByFall,Current->KilledByFall,High,Current);
			case 126:return EveluateHighestScore(High->TurnedOffLaserFence,Current->TurnedOffLaserFence,High,Current);
			case 127:return EveluateHighestScore(High->BackLaserFence,Current->BackLaserFence,High,Current);
			case 128:return EveluateHighestScore(High->ReactorPowerRestored,Current->ReactorPowerRestored,High,Current);
			case 129:return EveluateHighestScore(High->ComputerConsolesDeactivated,Current->ComputerConsolesDeactivated,High,Current);
			case 130:return EveluateHighestScore(High->CityShieldLowered,Current->CityShieldLowered,High,Current);
			case 131:return EveluateHighestScore(High->ElectrifiedWaterTurnedOff,Current->ElectrifiedWaterTurnedOff,High,Current);
			case 132:return EveluateHighestScore(High->TunnelEntryLowered,Current->TunnelEntryLowered,High,Current);
			case 133:return EveluateHighestScore(High->KillsWithDefenseGun,Current->KillsWithDefenseGun,High,Current);
			case 134:return EveluateHighestScore(High->KillsWithCheatGun,Current->KillsWithCheatGun,High,Current);
			case 135:return EveluateHighestScore(High->KillsWithFlameUpgrade,Current->KillsWithFlameUpgrade,High,Current);
			case 136:return EveluateHighestScore(High->KillsWithRoboticFlameUpgrade,Current->KillsWithRoboticFlameUpgrade,High,Current);
			case 137:return EveluateHighestScore(High->KillsWithGlitchGun,Current->KillsWithGlitchGun,High,Current);
			case 138:return EveluateHighestScore(High->KillsWithImperialist,Current->KillsWithImperialist,High,Current);
			case 139:return EveluateHighestScore(High->KillsWithRoboticImperialist,Current->KillsWithRoboticImperialist,High,Current);
			case 140:return EveluateHighestScore(High->KillsWithLaserShotgun,Current->KillsWithLaserShotgun,High,Current);
			case 141:return EveluateHighestScore(High->KillsWithRoboticLaserShotgun,Current->KillsWithRoboticLaserShotgun,High,Current);
			case 142:return EveluateHighestScore(High->KillsWithMachineGun,Current->KillsWithMachineGun,High,Current);
			case 143:return EveluateHighestScore(High->KillsWithRoboticMachineGun,Current->KillsWithRoboticMachineGun,High,Current);
			case 144:return EveluateHighestScore(High->KillsWithShockCoil,Current->KillsWithShockCoil,High,Current);
			case 145:return EveluateHighestScore(High->KillsWithRoboticShockCoil,Current->KillsWithRoboticShockCoil,High,Current);
			case 146:return EveluateHighestScore(High->KillsWithLaser,Current->KillsWithLaser,High,Current);
			case 147:return EveluateHighestScore(High->KillsWithRoboticLaser,Current->KillsWithRoboticLaser,High,Current);
			case 148:return EveluateHighestScore(High->KillsWithHyperdeath,Current->KillsWithHyperdeath,High,Current);
			case 149:return EveluateHighestScore(High->KillsWithSuperHyperdeath,Current->KillsWithSuperHyperdeath,High,Current);
			case 150:return EveluateHighestScore(High->KillsWithKnife,Current->KillsWithKnife,High,Current);
			case 151:return EveluateHighestScore(High->KillsWithGrenade,Current->KillsWithGrenade,High,Current);
			case 152:return EveluateHighestScore(High->KillsWithRoboticGrenade,Current->KillsWithRoboticGrenade,High,Current);
			case 153:return EveluateHighestScore(High->KillsWithUnknownWeapon,Current->KillsWithUnknownWeapon,High,Current);
			default:return EveluateHighestScore(High->PlayTimeInSpawnRoom,Current->PlayTimeInSpawnRoom,High,Current);
		}
	}
	bool ReturnHighScoreNodeValue(MetroidHighScoresNode *Node,int CompVal)
	{
		if (!Node)
			return false;
		switch (CompVal)
		{
			case 0:return Node->PlayTime ? true : false;
			case 1:return Node->IdleTime ? true : false;
			case 2:return Node->Points ? true : false;
			case 3:return Node->MostPointsInARound ? true : false;
			case 4:return Node->HealthPackPoints ? true : false;
			case 5:return Node->RechargePoints ? true : false;
			case 6:return Node->DefenseGunPoints ? true : false;
			case 7:return Node->TeleportPoints ? true : false;
			case 8:return Node->ShieldUpgradePoints ? true : false;
			case 9:return Node->WeaponSpawnPoints ? true : false;
			case 10:return Node->Deaths ? true : false;
			case 11:return Node->DeathFromElectrifiedWater ? true : false;
			case 12:return Node->DeathFromSpikePit ? true : false;
			case 13:return Node->DeathFromFallingOffTheTrain ? true : false;
			case 14:return Node->DeathInLava ? true : false;
			case 15:return Node->DeathInRadioactivePit ? true : false;
			case 16:return Node->DeathFromReactorRadiation ? true : false;
			case 17:return Node->DeathInFire ? true : false;
			case 18:return Node->DeathFromMiniBoss ? true : false;
			case 19:return Node->KilledMiniBoss ? true : false;
			case 20:return Node->DeathFromDefenseCameras ? true : false;
			case 21:return Node->DeathFromAliens ? true : false;
			case 22:return Node->DeathFromRobots ? true : false;
			case 23:return Node->DeathFromTraitors ? true : false;
			case 24:return Node->DeathFromAttackCameras ? true : false;
			case 25:return Node->DeathFromGunEmplacements ? true : false;
			case 26:return Node->DeathFromShredderTurret ? true : false;
			case 27:return Node->DeathFromBansheeBoss ? true : false;
			case 28:return Node->DeathFromMineBoss ? true : false;
			case 29:return Node->DeathFromMineBossSquish ? true : false;
			case 30:return Node->DeathFromMineBossResources ? true : false;
			case 31:return Node->Kills ? true : false;
			case 32:return Node->Suicides ? true : false;
			case 33:return Node->KilledPlayers ? true : false;
			case 34:return Node->KilledDefenseCameras ? true : false;
			case 35:return Node->KilledAliens ? true : false;
			case 36:return Node->KilledRobots ? true : false;
			case 37:return Node->KilledTraitors ? true : false;
			case 38:return Node->KilledSecurityCameras ? true : false;
			case 39:return Node->KilledAttackCameras ? true : false;
			case 40:return Node->KilledGunEmplacements ? true : false;
			case 41:return Node->KilledTurretBoss ? true : false;
			case 42:return Node->KilledBansheeBoss ? true : false;
			case 43:return Node->KilledMineBoss ? true : false;
			case 44:return Node->Chapter1Completed ? true : false;
			case 45:return Node->Chapter2Completed ? true : false;
			case 46:return Node->Chapter3Completed ? true : false;
			case 47:return Node->Chapter4Completed ? true : false;
			case 48:return Node->Chapter5Completed ? true : false;
			case 49:return Node->Chapter6Completed ? true : false;
			case 50:return Node->Chapter7Completed ? true : false;
			case 51:return Node->Chapter8Completed ? true : false;
			case 52:return Node->Chapter9Completed ? true : false;
			case 53:return Node->Chapter10Completed ? true : false;
			case 54:return Node->TriggeredLockdown0 ? true : false;
			case 55:return Node->TriggeredLockdown1 ? true : false;
			case 56:return Node->TriggeredLockdown2 ? true : false;
			case 57:return Node->TriggeredLockdown3 ? true : false;
			case 58:return Node->TriggeredLockdown4 ? true : false;
			case 59:return Node->TriggeredLockdown5 ? true : false;
			case 60:return Node->TriggeredLockdown6 ? true : false;
			case 61:return Node->TriggeredLockdown7 ? true : false;
			case 62:return Node->TriggeredLockdown8 ? true : false;
			case 63:return Node->TriggeredLockdown9 ? true : false;
			case 64:return Node->PickedUpPlayerPlacedWeapon ? true : false;
			case 65:return Node->PickedUpLaser ? true : false;
			case 66:return Node->PickedUpShotgun ? true : false;
			case 67:return Node->PickedUpSniper ? true : false;
			case 68:return Node->PickedUpFlame ? true : false;
			case 69:return Node->PickedUpShockCoil ? true : false;
			case 70:return Node->PickedUpMachineGun ? true : false;
			case 71:return Node->PickedUpRoboticLaser ? true : false;
			case 72:return Node->PickedUpRoboticShotgun ? true : false;
			case 73:return Node->PickedUpRoboticSniper ? true : false;
			case 74:return Node->PickedUpRoboticFlame ? true : false;
			case 75:return Node->PickedUpRoboticShockCoil ? true : false;
			case 76:return Node->PickedUpRoboticMachineGun ? true : false;
			case 77:return Node->PickedUpGlitchGun ? true : false;
			case 78:return Node->PickedUpHealthPack ? true : false;
			case 79:return Node->PickedUpPlayerPlacedHealthPack ? true : false;
			case 80:return Node->PickedUpShieldUpgrade ? true : false;
			case 81:return Node->PickedUpPlayerShieldUpgrade ? true : false;
			case 82:return Node->PlacedMedPacks ? true : false;
			case 83:return Node->PlacedRechargers ? true : false;
			case 84:return Node->PlacedTurrets ? true : false;
			case 85:return Node->PlacedTeleports ? true : false;
			case 86:return Node->PlacedShieldUpgrades ? true : false;
			case 87:return Node->PlacedWeaponSpawners ? true : false;
			case 88:return Node->UsedPlayerPlacedTeleport ? true : false;
			case 89:return Node->PlayTimeAsClass0 ? true : false;
			case 90:return Node->PlayTimeAsClass1 ? true : false;
			case 91:return Node->PlayTimeAsClass2 ? true : false;
			case 92:return Node->PlayTimeAsClass3 ? true : false;
			case 93:return Node->PlayTimeAsClass4 ? true : false;
			case 94:return Node->PlayTimeAsClass5 ? true : false;
			case 95:return Node->PlayTimeAsClass6 ? true : false;
			case 96:return Node->PlayTimeAsClass7 ? true : false;
			case 97:return Node->PlayTimeAsClass8 ? true : false;
			case 98:return Node->PlayTimeAsClass9 ? true : false;
			case 99:return Node->PlayTimeAsClass10 ? true : false;
			case 100:return Node->PlayTimeAsClass11 ? true : false;
			case 101:return Node->CompletedPlayThroughs ? true : false;
			case 102:return Node->DeathFromPlayers ? true : false;
			case 103:return Node->LongestPlayTimeInARound ? true : false;
			case 104:return Node->SpawnedAsCharacter0 ? true : false;
			case 105:return Node->SpawnedAsCharacter1 ? true : false;
			case 106:return Node->SpawnedAsCharacter2 ? true : false;
			case 107:return Node->SpawnedAsCharacter3 ? true : false;
			case 108:return Node->SpawnedAsCharacter4 ? true : false;
			case 109:return Node->SpawnedAsCharacter5 ? true : false;
			case 110:return Node->SpawnedAsCharacter6 ? true : false;
			case 111:return Node->SpawnedAsCharacter7 ? true : false;
			case 112:return Node->SpawnedAsCharacter8 ? true : false;
			case 113:return Node->SpawnedAsCharacter9 ? true : false;
			case 114:return Node->KilledCrates ? true : false;
			case 115:return Node->PickedUpGrenadeLaunchers ? true : false;
			case 116:return Node->ElevatorButtonsPressed ? true : false;
			case 117:return Node->ElevatorLockdownButton ? true : false;
			case 118:return Node->KilledBrokenDoors ? true : false;
			case 119:return Node->GlitchModeFound ? true : false;
			case 120:return Node->GlitchModeCompleted ? true : false;
			case 121:return Node->FoundCheatGun ? true : false;
			case 122:return Node->OpenedFirstDoor ? true : false;
			case 123:return Node->FoundCheatGun ? true : false;
			case 124:return Node->KilledByRapids ? true : false;
			case 125:return Node->KilledByFall ? true : false;
			case 126:return Node->TurnedOffLaserFence ? true : false;
			case 127:return Node->BackLaserFence ? true : false;
			case 128:return Node->ReactorPowerRestored ? true : false;
			case 129:return Node->ComputerConsolesDeactivated ? true : false;
			case 130:return Node->CityShieldLowered ? true : false;
			case 131:return Node->ElectrifiedWaterTurnedOff ? true : false;
			case 132:return Node->TunnelEntryLowered ? true : false;
			case 133:return Node->KillsWithDefenseGun ? true : false;
			case 134:return Node->KillsWithCheatGun ? true : false;
			case 135:return Node->KillsWithFlameUpgrade ? true : false;
			case 136:return Node->KillsWithRoboticFlameUpgrade ? true : false;
			case 137:return Node->KillsWithGlitchGun ? true : false;
			case 138:return Node->KillsWithImperialist ? true : false;
			case 139:return Node->KillsWithRoboticImperialist ? true : false;
			case 140:return Node->KillsWithLaserShotgun ? true : false;
			case 141:return Node->KillsWithRoboticLaserShotgun ? true : false;
			case 142:return Node->KillsWithMachineGun ? true : false;
			case 143:return Node->KillsWithRoboticMachineGun ? true : false;
			case 144:return Node->KillsWithShockCoil ? true : false;
			case 145:return Node->KillsWithRoboticShockCoil ? true : false;
			case 146:return Node->KillsWithLaser ? true : false;
			case 147:return Node->KillsWithRoboticLaser ? true : false;
			case 148:return Node->KillsWithHyperdeath ? true : false;
			case 149:return Node->KillsWithSuperHyperdeath ? true : false;
			case 150:return Node->KillsWithKnife ? true : false;
			case 151:return Node->KillsWithGrenade ? true : false;
			case 152:return Node->KillsWithRoboticGrenade ? true : false;
			case 153:return Node->KillsWithUnknownWeapon ? true : false;
			default:return Node->PlayTimeInSpawnRoom ? true : false;
		}
	}
	char *ReturnScore(MetroidHighScoresNode *High,int CompVal)
	{
		static char RetChar[200];
		if (!High)
			return " ";
		switch (CompVal)
		{
			case 0:sprintf(RetChar,"Server Record: %s has played for %.0lf minutes.",High->PlayerName,High->PlayTime/60.0f);return RetChar;
			case 1:sprintf(RetChar,"Server Record: %s been ilde for %.0lf minutes.",High->PlayerName,High->IdleTime/60.0f);return RetChar;
			case 2:sprintf(RetChar,"Server Record: %s has scored a total of %.0lf points.",High->PlayerName,High->Points);return RetChar;
			case 3:sprintf(RetChar,"Server Record: %s has socred %.0lf points in a single round.",High->PlayerName,High->MostPointsInARound);return RetChar;
			case 4:sprintf(RetChar,"Server Record: %s has socred %.0lf points from placed Health Packs.",High->PlayerName,High->HealthPackPoints);return RetChar;
			case 5:sprintf(RetChar,"Server Record: %s has socred %.0lf points Recharging Shields.",High->PlayerName,High->RechargePoints);return RetChar;
			case 6:sprintf(RetChar,"Server Record: %s has scored %.0lf points via Defense Guns.",High->PlayerName,High->DefenseGunPoints);return RetChar;
			case 7:sprintf(RetChar,"Server Record: %s has socred %.0lf points from Telepad use.",High->PlayerName,High->TeleportPoints);return RetChar;
			case 8:sprintf(RetChar,"Server Record: %s has scored %.0lf points by handing out Shield Upgrades.",High->PlayerName,High->ShieldUpgradePoints);return RetChar;
			case 9:sprintf(RetChar,"Server Record: %s has scored %.0lf points from placed Weapon Spawners.",High->PlayerName,High->WeaponSpawnPoints);return RetChar;
			case 10:sprintf(RetChar,"Server Record: %s has been killed a total of %lu times.",High->PlayerName,High->Deaths);return RetChar;
			case 11:sprintf(RetChar,"Server Record: %s has fallen into Electrified Water %lu times.",High->PlayerName,High->DeathFromElectrifiedWater);return RetChar;
			case 12:sprintf(RetChar,"Server Record: %s has fallen into Spike Pits %lu times.",High->PlayerName,High->DeathFromSpikePit);return RetChar;
			case 13:sprintf(RetChar,"Server Record: %s has fallen off the Train %lu times.",High->PlayerName,High->DeathFromFallingOffTheTrain);return RetChar;
			case 14:sprintf(RetChar,"Server Record: %s has died in Lava %lu times.",High->PlayerName,High->DeathInLava);return RetChar;
			case 15:sprintf(RetChar,"Server Record: %s has died in the Radioactive Pit %lu times.",High->PlayerName,High->DeathInRadioactivePit);return RetChar;
			case 16:sprintf(RetChar,"Server Record: %s has died from radiation from the Reactor %lu times.",High->PlayerName,High->DeathFromReactorRadiation);return RetChar;
			case 17:sprintf(RetChar,"Server Record: %s has died in Fire %lu times.",High->PlayerName,High->DeathInFire);return RetChar;
			case 18:sprintf(RetChar,"Server Record: %s has been killed by the Boss Alien %lu times.",High->PlayerName,High->DeathFromMiniBoss);return RetChar;
			case 19:sprintf(RetChar,"Server Record: %s has killed the Boss Alien %lu times.",High->PlayerName,High->KilledMiniBoss);return RetChar;
			case 20:sprintf(RetChar,"Server Record: %s has been killed by Defense Turrets %lu times.",High->PlayerName,High->DeathFromDefenseCameras);return RetChar;
			case 21:sprintf(RetChar,"Server Record: %s has been killed by Aliens %lu times.",High->PlayerName,High->DeathFromAliens);return RetChar;
			case 22:sprintf(RetChar,"Server Record: %s has been killed by Robots %lu times.",High->PlayerName,High->DeathFromRobots);return RetChar;
			case 23:sprintf(RetChar,"Server Record: %s has been killed by Traitors %lu times.",High->PlayerName,High->DeathFromTraitors);return RetChar;
			case 24:sprintf(RetChar,"Server Record: %s has been killed by Attack Cameras %lu times.",High->PlayerName,High->DeathFromAttackCameras);return RetChar;
			case 25:sprintf(RetChar,"Server Record: %s has been killed by Gun Emplacements %lu times.",High->PlayerName,High->DeathFromGunEmplacements);return RetChar;
			case 26:sprintf(RetChar,"Server Record: %s has been killed by the Shredder Turret %lu times.",High->PlayerName,High->DeathFromShredderTurret);return RetChar;
			case 27:sprintf(RetChar,"Server Record: %s has been killed by the Alien Strike Craft %lu times.",High->PlayerName,High->DeathFromBansheeBoss);return RetChar;
			case 28:sprintf(RetChar,"Server Record: %s has been killed by the Mine Equipment %lu times.",High->PlayerName,High->DeathFromMineBoss);return RetChar;
			case 29:sprintf(RetChar,"Server Record: %s has been squished by the Mine Equipment %lu times.",High->PlayerName,High->DeathFromMineBossSquish);return RetChar;
			case 30:sprintf(RetChar,"Server Record: %s has been turned to resources by the Mine Equipment %lu times.",High->PlayerName,High->DeathFromMineBossResources);return RetChar;
			case 31:sprintf(RetChar,"Server Record: %s has killed %lu enemies.",High->PlayerName,High->Kills);return RetChar;
			case 32:sprintf(RetChar,"Server Record: %s has killed theirself %lu times.",High->PlayerName,High->Suicides);return RetChar;
			case 33:sprintf(RetChar,"Server Record: %s has killed other players %lu times.",High->PlayerName,High->KilledPlayers);return RetChar;
			case 34:sprintf(RetChar,"Server Record: %s has destroyed %lu Defense Cameras.",High->PlayerName,High->KilledDefenseCameras);return RetChar;
			case 35:sprintf(RetChar,"Server Record: %s has killed %lu Aliens.",High->PlayerName,High->KilledAliens);return RetChar;
			case 36:sprintf(RetChar,"Server Record: %s has destroyed %lu Robots.",High->PlayerName,High->KilledRobots);return RetChar;
			case 37:sprintf(RetChar,"Server Record: %s has killed %lu Traitors.",High->PlayerName,High->KilledTraitors);return RetChar;
			case 38:sprintf(RetChar,"Server Record: %s has destroyed %lu Security Cameras.",High->PlayerName,High->KilledSecurityCameras);return RetChar;
			case 39:sprintf(RetChar,"Server Record: %s has destroyed %lu Attack Cameras.",High->PlayerName,High->KilledAttackCameras);return RetChar;
			case 40:sprintf(RetChar,"Server Record: %s has destroyed %lu Gun Emplacements.",High->PlayerName,High->KilledGunEmplacements);return RetChar;
			case 41:sprintf(RetChar,"Server Record: %s has destroyed the Shredder Turret %lu times.",High->PlayerName,High->KilledTurretBoss);return RetChar;
			case 42:sprintf(RetChar,"Server Record: %s has destroyed the Alien Attack Craft %lu times.",High->PlayerName,High->KilledBansheeBoss);return RetChar;
			case 43:sprintf(RetChar,"Server Record: %s has destroyed the Mine Equipment %lu times.",High->PlayerName,High->KilledMineBoss);return RetChar;
			case 44:sprintf(RetChar,"Server Record: %s has completed Chapter 1 %lu times.",High->PlayerName,High->Chapter1Completed);return RetChar;
			case 45:sprintf(RetChar,"Server Record: %s has completed Chapter 2 %lu times.",High->PlayerName,High->Chapter2Completed);return RetChar;
			case 46:sprintf(RetChar,"Server Record: %s has completed Chapter 3 %lu times.",High->PlayerName,High->Chapter3Completed);return RetChar;
			case 47:sprintf(RetChar,"Server Record: %s has completed Chapter 4 %lu times.",High->PlayerName,High->Chapter4Completed);return RetChar;
			case 48:sprintf(RetChar,"Server Record: %s has completed Chapter 5 %lu times.",High->PlayerName,High->Chapter5Completed);return RetChar;
			case 49:sprintf(RetChar,"Server Record: %s has completed Chapter 6 %lu times.",High->PlayerName,High->Chapter6Completed);return RetChar;
			case 50:sprintf(RetChar,"Server Record: %s has completed Chapter 7 %lu times.",High->PlayerName,High->Chapter7Completed);return RetChar;
			case 51:sprintf(RetChar,"Server Record: %s has completed Chapter 8 %lu times",High->PlayerName,High->Chapter8Completed);return RetChar;
			case 52:sprintf(RetChar,"Server Record: %s has completed Chapter 9 %lu times",High->PlayerName,High->Chapter9Completed);return RetChar;
			case 53:sprintf(RetChar,"Server Record: %s has completed Chapter 10 %lu times",High->PlayerName,High->Chapter10Completed);return RetChar;
			case 54:sprintf(RetChar,"Server Record: %s has triggered Lockdown Zone 1 %lu times.",High->PlayerName,High->TriggeredLockdown0);return RetChar;
			case 55:sprintf(RetChar,"Server Record: %s has triggered Lockdown Zone 2 %lu times.",High->PlayerName,High->TriggeredLockdown1);return RetChar;
			case 56:sprintf(RetChar,"Server Record: %s has triggered Lockdown Zone 3 %lu times.",High->PlayerName,High->TriggeredLockdown2);return RetChar;
			case 57:sprintf(RetChar,"Server Record: %s has triggered Lockdown Zone 4 %lu times.",High->PlayerName,High->TriggeredLockdown3);return RetChar;
			case 58:sprintf(RetChar,"Server Record: %s has triggered Lockdown Zone 5 %lu times.",High->PlayerName,High->TriggeredLockdown4);return RetChar;
			case 59:sprintf(RetChar,"Server Record: %s has triggered Lockdown Zone 6 %lu times.",High->PlayerName,High->TriggeredLockdown5);return RetChar;
			case 60:sprintf(RetChar,"Server Record: %s has triggered Lockdown Zone 7 %lu times.",High->PlayerName,High->TriggeredLockdown6);return RetChar;
			case 61:sprintf(RetChar,"Server Record: %s has triggered Lockdown Zone 8 %lu times.",High->PlayerName,High->TriggeredLockdown7);return RetChar;
			case 62:sprintf(RetChar,"Server Record: %s has triggered Lockdown Zone 9 %lu times.",High->PlayerName,High->TriggeredLockdown8);return RetChar;
			case 63:sprintf(RetChar,"Server Record: %s has triggered Lockdown Zone 10 %lu times.",High->PlayerName,High->TriggeredLockdown9);return RetChar;
			case 64:sprintf(RetChar,"Server Record: %s has acquired %lu weapons placed by other players.",High->PlayerName,High->PickedUpPlayerPlacedWeapon);return RetChar;
			case 65:sprintf(RetChar,"Server Record: %s has acquired %lu Laser Gun upgrades.",High->PlayerName,High->PickedUpLaser);return RetChar;
			case 66:sprintf(RetChar,"Server Record: %s has acquired %lu Laser Shotgun upgrades.",High->PlayerName,High->PickedUpShotgun);return RetChar;
			case 67:sprintf(RetChar,"Server Record: %s has acquired %lu Imperialist upgrades.",High->PlayerName,High->PickedUpSniper);return RetChar;
			case 68:sprintf(RetChar,"Server Record: %s has acquired %lu Flame upgrades.",High->PlayerName,High->PickedUpFlame);return RetChar;
			case 69:sprintf(RetChar,"Server Record: %s has acquired %lu Shock Coil upgrades.",High->PlayerName,High->PickedUpShockCoil);return RetChar;
			case 70:sprintf(RetChar,"Server Record: %s has acquired %lu Machinegun upgrades.",High->PlayerName,High->PickedUpMachineGun);return RetChar;
			case 71:sprintf(RetChar,"Server Record: %s has acquired %lu Robotic Laser Gun upgrades.",High->PlayerName,High->PickedUpRoboticLaser);return RetChar;
			case 72:sprintf(RetChar,"Server Record: %s has acquired %lu Robotic Laser Shotgun upgrades.",High->PlayerName,High->PickedUpRoboticShotgun);return RetChar;
			case 73:sprintf(RetChar,"Server Record: %s has acquired %lu Robotic Imperialist upgrades.",High->PlayerName,High->PickedUpRoboticSniper);return RetChar;
			case 74:sprintf(RetChar,"Server Record: %s has acquired %lu Robotic Flame upgrades.",High->PlayerName,High->PickedUpRoboticFlame);return RetChar;
			case 75:sprintf(RetChar,"Server Record: %s has acquired %lu Robotic Shock Coil upgrades.",High->PlayerName,High->PickedUpRoboticShockCoil);return RetChar;
			case 76:sprintf(RetChar,"Server Record: %s has acquired %lu Robotic Machinegun upgrades.",High->PlayerName,High->PickedUpRoboticMachineGun);return RetChar;
			case 77:sprintf(RetChar,"Server Record: %s has acquired %lu Glitch Guns.",High->PlayerName,High->PickedUpGlitchGun);return RetChar;
			case 78:sprintf(RetChar,"Server Record: %s has acquired %lu Health Packs.",High->PlayerName,High->PickedUpHealthPack);return RetChar;
			case 79:sprintf(RetChar,"Server Record: %s has acquired %lu player placed Health Packs.",High->PlayerName,High->PickedUpPlayerPlacedHealthPack);return RetChar;
			case 80:sprintf(RetChar,"Server Record: %s has acquired %lu Shield Upgrades.",High->PlayerName,High->PickedUpShieldUpgrade);return RetChar;
			case 81:sprintf(RetChar,"Server Record: %s has acquired %lu player placed Shield Upgrades.",High->PlayerName,High->PickedUpPlayerShieldUpgrade);return RetChar;
			case 82:sprintf(RetChar,"Server Record: %s has placed %lu Med Packs.",High->PlayerName,High->PlacedMedPacks);return RetChar;
			case 83:sprintf(RetChar,"Server Record: %s has placed %lu Shield Rechargers.",High->PlayerName,High->PlacedRechargers);return RetChar;
			case 84:sprintf(RetChar,"Server Record: %s has placed %lu Defense Turrets.",High->PlayerName,High->PlacedTurrets);return RetChar;
			case 85:sprintf(RetChar,"Server Record: %s has placed %lu Telepads.",High->PlayerName,High->PlacedTeleports);return RetChar;
			case 86:sprintf(RetChar,"Server Record: %s has placed %lu Shield Upgrades.",High->PlayerName,High->PlacedShieldUpgrades);return RetChar;
			case 87:sprintf(RetChar,"Server Record: %s has placed %lu Weapon Spawners.",High->PlayerName,High->PlacedWeaponSpawners);return RetChar;
			case 88:sprintf(RetChar,"Server Record: %s has used %lu player placed Telepads.",High->PlayerName,High->UsedPlayerPlacedTeleport);return RetChar;
			case 89:sprintf(RetChar,"Server Record: %s has played as Armor Mode for %.0lf mintues.",High->PlayerName,High->PlayTimeAsClass0/60.0);return RetChar;
			case 90:sprintf(RetChar,"Server Record: %s has played as Speed Mode for %.0lf mintues.",High->PlayerName,High->PlayTimeAsClass1/60.0);return RetChar;
			case 91:sprintf(RetChar,"Server Record: %s has played as Firepower Mode for %.0lf mintues.",High->PlayerName,High->PlayTimeAsClass2/60.0);return RetChar;
			case 92:sprintf(RetChar,"Server Record: %s has played as Support Mode for %.0lf mintues.",High->PlayerName,High->PlayTimeAsClass3/60.0);return RetChar;
			case 93:sprintf(RetChar,"Server Record: %s has played as Defense Mode for %.0lf mintues.",High->PlayerName,High->PlayTimeAsClass4/60.0);return RetChar;
			case 94:sprintf(RetChar,"Server Record: %s has played as  Mode for %.0lf mintues.",High->PlayerName,High->PlayTimeAsClass5/60.0);return RetChar;
			case 95:sprintf(RetChar,"Server Record: %s has played as Supply Mode for %.0lf mintues.",High->PlayerName,High->PlayTimeAsClass6/60.0);return RetChar;
			case 96:sprintf(RetChar,"Server Record: %s has played as N/A Mode for %.0lf mintues.",High->PlayerName,High->PlayTimeAsClass7/60.0);return RetChar;
			case 97:sprintf(RetChar,"Server Record: %s has played as N/A Mode for %.0lf mintues.",High->PlayerName,High->PlayTimeAsClass8/60.0);return RetChar;
			case 98:sprintf(RetChar,"Server Record: %s has played as Cheat Mode for %.0lf mintues.",High->PlayerName,High->PlayTimeAsClass9/60.0);return RetChar;
			case 99:sprintf(RetChar,"Server Record: %s has played as Training Mode for %.0lf mintues.",High->PlayerName,High->PlayTimeAsClass10/60.0);return RetChar;
			case 100:sprintf(RetChar,"Server Record: %s has played as N/A Mode for %.0lf mintues.",High->PlayerName,High->PlayTimeAsClass11/60.0);return RetChar;
			case 101:sprintf(RetChar,"Server Record: %s has completed the story %lu times.",High->PlayerName,High->CompletedPlayThroughs);return RetChar;
			case 102:sprintf(RetChar,"Server Record: %s has been killed by friendly fire %lu times.",High->PlayerName,High->DeathFromPlayers);return RetChar;
			case 103:sprintf(RetChar,"Server Record: %s has spent %.0lf minutes in a single game.",High->PlayerName,High->LongestPlayTimeInARound/60.0);return RetChar;
			case 104:sprintf(RetChar,"Server Record: %s has spawned as Armor Mode %lu times.",High->PlayerName,High->SpawnedAsCharacter0);return RetChar;
			case 105:sprintf(RetChar,"Server Record: %s has spawned as Speed Mode %lu times.",High->PlayerName,High->SpawnedAsCharacter1);return RetChar;
			case 106:sprintf(RetChar,"Server Record: %s has spawned as Firepower Mode %lu times.",High->PlayerName,High->SpawnedAsCharacter2);return RetChar;
			case 107:sprintf(RetChar,"Server Record: %s has spawned as Support Mode %lu times.",High->PlayerName,High->SpawnedAsCharacter3);return RetChar;
			case 108:sprintf(RetChar,"Server Record: %s has spawned as Defense Mode %lu times.",High->PlayerName,High->SpawnedAsCharacter4);return RetChar;
			case 109:sprintf(RetChar,"Server Record: %s has spawned as Supply Mode %lu times.",High->PlayerName,High->SpawnedAsCharacter5);return RetChar;
			case 110:sprintf(RetChar,"Server Record: %s has spawned as N/A Mode %lu times.",High->PlayerName,High->SpawnedAsCharacter6);return RetChar;
			case 111:sprintf(RetChar,"Server Record: %s has spawned as N/A Mode %lu times.",High->PlayerName,High->SpawnedAsCharacter7);return RetChar;
			case 112:sprintf(RetChar,"Server Record: %s has spawned as N/A Mode %lu times.",High->PlayerName,High->SpawnedAsCharacter8);return RetChar;
			case 113:sprintf(RetChar,"Server Record: %s has spawned as Cheat Mode %lu times.",High->PlayerName,High->SpawnedAsCharacter9);return RetChar;
			case 114:sprintf(RetChar,"Server Record: %s has destroyed %lu crates.",High->PlayerName,High->KilledCrates);return RetChar;
			case 115:sprintf(RetChar,"Server Record: %s has acquired %lu Grenade Launcher upgrades.",High->PlayerName,High->PickedUpGrenadeLaunchers);return RetChar;
			case 116:sprintf(RetChar,"Server Record: %s has activated %lu elevator buttons.",High->PlayerName,High->ElevatorButtonsPressed);return RetChar;
			case 117:sprintf(RetChar,"Server Record: %s has deactivated %lu drill security lockouts.",High->PlayerName,High->ElevatorLockdownButton);return RetChar;
			case 118:sprintf(RetChar,"Server Record: %s has destroyed the jammed doors %lu times.",High->PlayerName,High->KilledBrokenDoors);return RetChar;
			case 119:sprintf(RetChar,"Server Record: %s has found the glitch %lu times.",High->PlayerName,High->GlitchModeFound);return RetChar;
			case 120:sprintf(RetChar,"Server Record: %s has escaped the glitch %lu times.",High->PlayerName,High->GlitchModeCompleted);return RetChar;
			case 121:sprintf(RetChar,"Server Record: %s has acquired The Cheat Gun %lu times.",High->PlayerName,High->FoundCheatGun);return RetChar;
			case 122:sprintf(RetChar,"Server Record: %s has opened the security room door %lu times.",High->PlayerName,High->OpenedFirstDoor);return RetChar;
			case 123:sprintf(RetChar,"Server Record: %s has acquired %lu keycards.",High->PlayerName,High->GotKeycard);return RetChar;
			case 124:sprintf(RetChar,"Server Record: %s has fallen into the rapids %lu times.",High->PlayerName,High->KilledByRapids);return RetChar;
			case 125:sprintf(RetChar,"Server Record: %s has fallen off the cliff %lu times.",High->PlayerName,High->KilledByFall);return RetChar;
			case 126:sprintf(RetChar,"Server Record: %s has turned off the laser fence %lu times.",High->PlayerName,High->TurnedOffLaserFence);return RetChar;
			case 127:sprintf(RetChar,"Server Record: %s has turned off the back laser fence %lu times.",High->PlayerName,High->BackLaserFence);return RetChar;
			case 128:sprintf(RetChar,"Server Record: %s has restored the reactor power %lu times.",High->PlayerName,High->ReactorPowerRestored);return RetChar;
			case 129:sprintf(RetChar,"Server Record: %s has deactivated %lu computer consoles.",High->PlayerName,High->ComputerConsolesDeactivated);return RetChar;
			case 130:sprintf(RetChar,"Server Record: %s has lowered the city laser fence %lu times.",High->PlayerName,High->CityShieldLowered);return RetChar;
			case 131:sprintf(RetChar,"Server Record: %s has turned off the electrified water %lu times.",High->PlayerName,High->ElectrifiedWaterTurnedOff);return RetChar;
			case 132:sprintf(RetChar,"Server Record: %s has turned off the laser fence blocking the building %lu times.",High->PlayerName,High->TunnelEntryLowered);return RetChar;
			case 133:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Deployable Defense Turret.",High->PlayerName,High->KillsWithDefenseGun);return RetChar;
			case 134:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Cheating Gun.",High->PlayerName,High->KillsWithCheatGun);return RetChar;
			case 135:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Flame Upgrade.",High->PlayerName,High->KillsWithFlameUpgrade);return RetChar;
			case 136:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Robotic Flame Upgrade.",High->PlayerName,High->KillsWithRoboticFlameUpgrade);return RetChar;
			case 137:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Glitch Gun.",High->PlayerName,High->KillsWithGlitchGun);return RetChar;
			case 138:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Imperialist.",High->PlayerName,High->KillsWithImperialist);return RetChar;
			case 139:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Robotic Imperalist.",High->PlayerName,High->KillsWithRoboticImperialist);return RetChar;
			case 140:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Laser Shotgun.",High->PlayerName,High->KillsWithLaserShotgun);return RetChar;
			case 141:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Robotic Laser Shotgun.",High->PlayerName,High->KillsWithRoboticLaserShotgun);return RetChar;
			case 142:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Machinegun.",High->PlayerName,High->KillsWithMachineGun);return RetChar;
			case 143:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Robotic Machinegun.",High->PlayerName,High->KillsWithRoboticMachineGun);return RetChar;
			case 144:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Shock Coil.",High->PlayerName,High->KillsWithShockCoil);return RetChar;
			case 145:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Robotic Shock Coil.",High->PlayerName,High->KillsWithRoboticShockCoil);return RetChar;
			case 146:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Laser.",High->PlayerName,High->KillsWithLaser);return RetChar;
			case 147:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Robotic Laser.",High->PlayerName,High->KillsWithRoboticLaser);return RetChar;
			case 148:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Kinetic Cannon.",High->PlayerName,High->KillsWithHyperdeath);return RetChar;
			case 149:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Energy Beam.",High->PlayerName,High->KillsWithSuperHyperdeath);return RetChar;
			case 150:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Knife.",High->PlayerName,High->KillsWithKnife);return RetChar;
			case 151:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Grenade Launcher.",High->PlayerName,High->KillsWithGrenade);return RetChar;
			case 152:sprintf(RetChar,"Server Record: %s has killed %ld enemies with the Robotic Grenade Launcher.",High->PlayerName,High->KillsWithRoboticGrenade);return RetChar;
			case 153:sprintf(RetChar,"Server Record: %s has killed %ld enemies with an Unknown Device.",High->PlayerName,High->KillsWithUnknownWeapon);return RetChar;
			default:sprintf(RetChar,"Server Record: %s has spent %.0lf minutes in the spawn room.",High->PlayerName,High->PlayTimeInSpawnRoom/60.0);return RetChar;
		} 
	}
	void StateHighScore()
	{
		int LastHighScoreList = 1;
		int Random = Commands->Get_Random_Int(0,MetroidHIGHSCORELISTCOUNT+1);
		int attempts = 0;
MetroidStartOfHighScoreSelectProcess:
		selectRandomMatchingScore = 0;
		MetroidHighScoresNode *Current = MetroidHighScoresNodeList,*Best = NULL;
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
				if (Random+1 < MetroidHIGHSCORELISTCOUNT)
					Random++;
				else
					Random = 1;
			}
			else
			{
				LastHighScoreList = 0;
				Random = 0;
			}
			attempts++;
			if (attempts < 100)
				goto MetroidStartOfHighScoreSelectProcess;
			else
				return;
		}
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

MetroidScoreSystem MetroidScoreControl = MetroidScoreSystem();

void SaveMetroidHighScores()
{
	MetroidScoreControl.SaveData();
}
char *MetroidStringLibrary(int StringID)
{
	switch (StringID)
	{
	//case :return "";
	case 0:return "Checkpoint 1 Unlocked!";
	case 1:return "Checkpoint 2 Unlocked!";
	case 2:return "Checkpoint 3 Unlocked!";
	case 3:return "Checkpoint 4 Unlocked!";
	case 4:return "Checkpoint 5 Unlocked!";
	case 5:return "Final checkpoint unlocked!";
	case 6:return "Laser Wall Deactivated.";
	case 7:return "The back gate has been unlocked.";
	case 8:return "Power has been shut off.";
	case 9:return "Emergency power restored.";
	case 10:return "Power failure! Please re activate power!";
	case 11:return "Shield lowered.";
	case 12:return "Lockdown ended. Gate has been lowered.";
	case 13:return "Proceed to the mining facility.";
	case 14:return "Deactivate the lockdown and clear all security alerts.";
	case 15:return "All-clear, reactivating operations.";
	case 16:return "Dactivate the facility's shielding via the 3 control panels.";
	case 17:return "Mining Equipment Destroyed!";
	case 18:return "Quarantine lifted, laser gate powered off.";
	case 19:return "Eliminate the hazardous lifeform to end the quarantine.";
	case 20:return "Kill the guards to cancel the lockdown.";
	case 21:return "?????";
	case 22:return "Turret destroyed lockdown averted.";
	case 23:return "Lockdown canceled, laser gate powered off.";
	case 24:return "Mash the action key to hack the terminal.";
	case 25:return "Good work!";
	default:return "HUD STRING NOT FOUND!";
	}
}

void Set_Shader_Number_Vector(GameObject *Player,int eventid,Vector4 parameter)
{
	if (eventid == -1)
		return;
	if (parameter.X == 4920036.0f)
	{// Metroid HUD Text
		char text[220];
		sprintf_s(text,"%s",MetroidStringLibrary(eventid));
		JmgUtility::DisplayChatMessage(Player,(int)(parameter.Y*255),(int)(parameter.Z*255),(int)(parameter.W*255),text);
	}
	if (parameter.X == 4920037.0f)
	{// Metroid Disarm HUD text
		char textString[256];
		switch (eventid)
		{
		case 99:sprintf_s(textString,"Control panel deactivated (%.0f/3)",parameter.Y);	Set_HUD_Help_Text_Player_Text(Player,7233,textString,Vector3(0.0,1.0,0.0));break;
		case 999:sprintf_s(textString,"Incorrect code! Counter measures activated!");break;
		case 998:sprintf_s(textString,"A terminal has been reset!");break;
		case 997:sprintf_s(textString,"You moved out of range!");break;
		case 0:sprintf_s(textString,"Press Numpad 0!");break;
		case 1:sprintf_s(textString,"Press Numpad 1!");break;
		case 2:sprintf_s(textString,"Press Numpad 2!");break;
		case 3:sprintf_s(textString,"Press Numpad 3!");break;
		case 4:sprintf_s(textString,"Press Numpad 4!");break;
		case 5:sprintf_s(textString,"Press Numpad 5!");break;
		case 6:sprintf_s(textString,"Press Numpad 6!");break;
		case 7:sprintf_s(textString,"Press Numpad 7!");break;
		case 8:sprintf_s(textString,"Press Numpad 8!");break;
		case 9:sprintf_s(textString,"Press Numpad 9!");break;
		default:sprintf_s(textString,"INVALID DISARM STRING!");break;
		}
		if (eventid == 999 || eventid == 998 || eventid == 997)
			JmgUtility::DisplayChatMessage(Player,(int)(1.0f*255),(int)(0.1f*255),(int)(0.1f*255),textString);
		else if (eventid == 99)
			JmgUtility::DisplayChatMessage(Player,(int)(0.0f*255),(int)(0.5f*255),(int)(1.0f*255),textString);
		else
			JmgUtility::DisplayChatMessage(Player,(int)(0.1f*255),(int)(1.0f*255),(int)(0.1f*255),textString);
	}

	if (parameter.X == 4920038.0f)
	{// Metroid Special Objective HUD text
		char textString[256];
		switch (eventid)
		{
		case 0:
			if ((int)parameter.Y != 3)
			{
				sprintf_s(textString,"Security Console Deactivated (%d/3)",(int)parameter.Y);
				Set_HUD_Help_Text_Player_Text(Player,7233,textString,Vector3(0.0,1.0,0.0));
			}
			else
				sprintf_s(textString,"Security restrictions cleared, resuming operations.");
				break;
		default:sprintf_s(textString,"INVALID DISARM STRING!");break;
		}
		JmgUtility::DisplayChatMessage(Player,(int)(0.0f*255),(int)(0.5f*255),(int)(1.0f*255),textString);
	} 
}
Vector3 JMGGetClosestStar(const Vector3 &Pos)
{
	GameObject *AStar = Commands->Get_A_Star(Pos);
	if (AStar)
		return Commands->Get_Position(AStar);
	return Pos;
}

class MetroidTempPlayerData
{
public:
	struct TempPlayerDataNode
	{
		char playerName[128];
		bool firstJoin;
		time_t destroyTime;
		struct TempPlayerDataNode *next;
		TempPlayerDataNode(const char *playerName)
		{
			firstJoin = true;
			destroyTime = NULL;
			if (playerName)
				sprintf(this->playerName,"%s",playerName);
			else
				sprintf(this->playerName,"");
			next = NULL;
		}
		void clearData()
		{
		}
	};
	struct TempPlayerDataNode *players[128];
private:
	struct TempPlayerDataNode *inactivePlayerData;
	struct TempPlayerDataNode *dummyNode;
	TempPlayerDataNode *FindPlayerNode(const char *playerName)
	{
		TempPlayerDataNode *Current = inactivePlayerData;
		if (!inactivePlayerData)
			return (inactivePlayerData = new TempPlayerDataNode(playerName));
		while (Current)
		{
			if (!_stricmp(Current->playerName,playerName))
				return Current;
			if (!Current->next)
				return (Current->next = new TempPlayerDataNode(playerName));
			Current = Current->next;
		}
		return NULL;
	}
public:
	MetroidTempPlayerData()
	{
		dummyNode = new TempPlayerDataNode("");
		for (int x = 0;x < 128;x++)
			players[x] = NULL;
		players[0] = dummyNode;
		inactivePlayerData = NULL;
	}
	void getPlayerNode(GameObject *player)
	{
		if (!player)
			return;
		int playerId = JmgUtility::JMG_Get_Player_ID(player);
		if (!playerId)
			return;
		if (!players[playerId] || _stricmp(players[playerId]->playerName,Get_Player_Name(player)))
			players[playerId] = FindPlayerNode(Get_Player_Name(player));
	}
	void Cleanup()
	{
		TempPlayerDataNode *temp = inactivePlayerData,*die;
		while (temp)
		{
			die = temp;
			die->clearData();
			temp = temp->next;
			delete die;
		}
		inactivePlayerData = NULL;
		for (int x = 1;x < 128;x++)
			players[x] = NULL;
	}
};
MetroidTempPlayerData mPlayerData;

class JMG_Metroid_Hide_On_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Metroid_Gibs_On_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Metroid_Increase_Perk_Unlock_On_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Metroid_HUD_Message_On_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Metroid_Game_Initilization : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
public:
	static bool IsRp2Hosted;
};
bool JMG_Metroid_Game_Initilization::IsRp2Hosted = false;

struct BriefingTextNode
{
	char Text[512];
	float Delay;
	BriefingTextNode *next;
	BriefingTextNode(const char *text)
	{
		Delay = 0.0f;
		sprintf(Text,"%s",text); 
		next = NULL;
	}
	BriefingTextNode(const char *text,float delay)
	{
		Delay = delay;
		sprintf(Text,"%s",text); 
		next = NULL;
	}
	BriefingTextNode()
	{
		next = NULL;
	}
};

class JMG_Metroid_Game_Control : public ScriptImpClass {
	int lastPlayerCount;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
	void IncreasePlayerTimeScores();
	static void AddNewTextNode(int playerId);
	static void RemoveTextNodes(int playerId);
public:
	JMG_Metroid_Game_Control();
	static void ParseBriefingString(const char *text,int playerId);
	static int EnemyShieldRegenStartTime;
	static BriefingTextNode *briefingText[128];
	static BriefingTextNode *currentNode[128];
	static int currentBriefingId;
	static int gameRemainingTime;
};
int JMG_Metroid_Game_Control::EnemyShieldRegenStartTime = 5;
BriefingTextNode *JMG_Metroid_Game_Control::briefingText[128];
BriefingTextNode *JMG_Metroid_Game_Control::currentNode[128];
int JMG_Metroid_Game_Control::currentBriefingId;
int JMG_Metroid_Game_Control::gameRemainingTime = 0;

class JMG_Metroid_Character_Control : public ScriptImpClass {
	int PlayerID;
	int LastWeaponID;
	int LastAmmoCount;
	float IgnoreDamage;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj, GameObject *damager);
	void Destroyed(GameObject *obj);
	void ClearBeacons(GameObject *obj,int ArmorMode);
};

class JMG_Metroid_Hide_Object_On_Death : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *damager);
};

class AmbushPointSystem
{
private:
	struct AmbushPointNode
	{
		int ID;
		Vector3 targetPos;
		struct AmbushPointNode *next;
		AmbushPointNode(Vector3 Pos,int id)
		{
			ID = id;
			targetPos = Pos;
			next = NULL;
		}
	};
public:
	struct CoverPointNode
	{
		int ID;
		int gameID;
		int GroupID;
		int GuardCount;
		bool Crouch;
		bool HidingSpot;
		Vector3 coverPos;
		int AmbushPoints;
		AmbushPointNode *AmbushPointNodeList;
		struct CoverPointNode *next;
		CoverPointNode(GameObject *obj,int id,int groupID,bool crouch,bool hidingSpot)
		{
			GuardCount = 0;
			ID = id;
			gameID = Commands->Get_ID(obj);
			GroupID = groupID;
			Crouch = crouch;
			HidingSpot = hidingSpot;
			coverPos = Commands->Get_Position(obj);
			AmbushPoints = 0;
			AmbushPointNodeList = NULL;
			next = NULL;
			Commands->Destroy_Object(obj);
		}
		void AddPoint(GameObject *obj)
		{
			Vector3 Pos = Commands->Get_Position(obj);
			int id = Commands->Get_ID(obj);
			Commands->Destroy_Object(obj);
			AmbushPointNode *Current = AmbushPointNodeList;
			AmbushPoints++;
			if (!Current)
				AmbushPointNodeList = new AmbushPointNode(Pos,id);
			while (Current)
			{
				if (Current->ID == id)
				{
					AmbushPoints--;
					char Err[512];
					sprintf(Err,"msg ERROR: Ambush Point %d already in list!",id);
					Console_Input(Err);
					return;
				}
				if (!Current->next)
				{
					Current->next = new AmbushPointNode(Pos,id);
					return;
				}
				Current = Current->next;
			}
		};
		Vector3 GetRandomAmbushPoint()
		{
			if (!AmbushPoints)
				return Vector3(0.0f,0.0f,0.0f);
			int random = Commands->Get_Random_Int(0,AmbushPoints*2);
			AmbushPointNode *Current = AmbushPointNodeList;
			while (Current)
			{
				if (random)
					random--;
				if (!random)
					return Current->targetPos;
				Current = Current->next;
				if (!Current)
					Current = AmbushPointNodeList;
			}
			return Vector3(0.0f,0.0f,0.0f);
		}
		void Empty_List()
		{
			AmbushPointNode *temp = AmbushPointNodeList,*die;
			while (temp)
			{
				die = temp;
				temp = temp->next;
				delete die;
			}
			AmbushPointNodeList = NULL;
		}
	};
private:
	int CoverPointCount;
	CoverPointNode *CoverPointNodeList;
	int CountCoverPointsInGroup(int groupID)
	{
		int Count = 0;
		CoverPointNode *Current = CoverPointNodeList;
		while (Current)
		{
			if (Current->GroupID == groupID)
				Count++;
			Current = Current->next;
		}
		return Count;
	}
	float FindClosestStar(const Vector3 &Pos)
	{
		float Distance = -1;
		for (int x = 1;x < 128;x++)
		{
			GameObject *Star = Get_GameObj(x);
			if (!Star)
				continue;
			float Temp = JmgUtility::SimpleDistance(Commands->Get_Position(Star),Pos);
			if (Distance == -1 || Temp < Distance)
				Distance = Temp;
		}
		return Distance;
	}
public:
	AmbushPointSystem()
	{
		CoverPointCount = 1;
		CoverPointNodeList = NULL;
	}
	void NewCoverPoint(GameObject *obj,int ID,int groupID,bool crouch,bool hidingSpot)
	{
		CoverPointCount++;
		CoverPointNode *Current = CoverPointNodeList;
		if (!Current)
			CoverPointNodeList = new CoverPointNode(obj,ID,groupID,crouch,hidingSpot);
		while (Current)
		{
			if (Current->ID == ID)
			{
				CoverPointCount--;
				char Err[512];
				sprintf(Err,"msg ERROR: Cover Point %d already in list!",ID);
				Console_Input(Err);
				return;
			}
			if (!Current->next)
			{
				Current->next = new CoverPointNode(obj,ID,groupID,crouch,hidingSpot);
				return;
			}
			Current = Current->next;
		}
	}
	void AddAmbushPoint(GameObject *obj,int ID)
	{
		CoverPointNode *Current = CoverPointNodeList;
		while (Current)
		{
			if (Current->ID == ID)
			{
				Current->AddPoint(obj);
				return;
			}
			Current = Current->next;
		}
		char Err[512];
		sprintf(Err,"msg ERROR: Ambush Point %d does not exist!",ID);
		Console_Input(Err);
	}
	void CoverPointStabilityTest()
	{
		CoverPointNode *Current = CoverPointNodeList;
		while (Current)
		{
			if (!Current->AmbushPointNodeList)
			{
				char Err[512];
				sprintf(Err,"msg ERROR: Cover Point %d has no Ambush points!",Current->gameID);
				Console_Input(Err);
			}
			Current = Current->next;
		}
	}
	CoverPointNode *FindBestCoverPoint(const Vector3 &MyPos,Vector3 *TargetPos,int groupID,float MyMaxDistance,float TargetMaxDist,int MaxSoldiersAtCP)
	{
		CoverPointNode *Current = CoverPointNodeList;
		CoverPointNode *Best = NULL;
		Vector3 Closest;
		float dist = 0.0f;
		while (Current)
		{
			if (Current->GroupID == groupID && (!MaxSoldiersAtCP || Current->GuardCount < MaxSoldiersAtCP) && (MyMaxDistance <= 0 || MyMaxDistance >= JmgUtility::SimpleDistance(MyPos,Current->coverPos)))
			{
				AmbushPointNode *Current2 = Current->AmbushPointNodeList;
				while (Current2)
				{
					if (TargetMaxDist == 0.0f || FindClosestStar(Current2->targetPos) < TargetMaxDist)
					{
						float Temp = JmgUtility::SimpleDistance(*TargetPos,Current2->targetPos);
						if (!Best || Temp < dist)
						{
							Best = Current;
							dist = Temp;
							Closest = Current2->targetPos;
						}
					}
					Current2 = Current2->next;
				}
			}
			Current = Current->next;
		}
		return Best;
	}
	CoverPointNode *GetRandomCoverPoint(Vector3 *TargetPos,const Vector3 &SearchLocation,int groupID,int MaxSoldiersAtCP,float MaxDistance)
	{
		CoverPointNode *Current = CoverPointNodeList;
		int Random = Commands->Get_Random_Int(0,CoverPointCount);
		int Count = 0;
		int AttemptCount = 0;
		while (Current)
		{
			float Temp = MaxDistance ? JmgUtility::SimpleDistance(SearchLocation,Current->coverPos) : 0.0f;
			if (Current->GroupID == groupID && (!MaxSoldiersAtCP || Current->GuardCount < MaxSoldiersAtCP) && Temp <= MaxDistance)
			{
				if (Count >= Random)
				{
					Random = Commands->Get_Random_Int(0,Current->AmbushPoints*2);
					Count = 0;
					AmbushPointNode *Current2 = Current->AmbushPointNodeList;
					while (Current2)
					{
						if (Count >= Random)
						{
							*TargetPos = Current2->targetPos;
							return Current;
						}
						Count++;
						Current2 = Current2->next;
						if (!Current2)
							Current2 = Current->AmbushPointNodeList;
					}
				}
				Count++;
			}
			Current = Current->next;
			if (!Current && AttemptCount < 100)
				Current = CoverPointNodeList,AttemptCount++;
		}
		return NULL;
	}
	CoverPointNode *GetGoodHidingSpot(Vector3 *TargetDirection,int groupID,int MaxSoldiersAtCP)
	{
		int GroupPointCount = CountCoverPointsInGroup(groupID);
		int Random = Commands->Get_Random_Int(0,CoverPointCount*2);
		int HidingSpotBonus = JmgUtility::MathClampInt((int)(GroupPointCount*0.1f),1,99);
		int Count = 0;
		int MaxAttempts = 0;
		CoverPointNode *Current = CoverPointNodeList;
		while (Current)
		{
			if (Current->GroupID == groupID && FindClosestStar(Current->coverPos) >= 2500)
			{
				Count++;
				if (Count+(Current->HidingSpot ? HidingSpotBonus : 0) >= Random)
					return Current;
			}
			Current = Current->next;
			if (!Current && MaxAttempts < 10)
			{
				MaxAttempts++;
				Current = CoverPointNodeList;
			}
		}
		return NULL;
	}	
	void CheckForErrors()
	{
		CoverPointNode *Current = CoverPointNodeList;
		while (Current)
		{
			if (!Current->AmbushPoints)
			{
				char errorMsg[512];
				sprintf(errorMsg,"msg AmbushPointSystem ERROR: ObjectID:%d PointID:%d has no target points!",Current->gameID,Current->ID);
				Console_Input(errorMsg);
			}
			Current = Current->next;
		}
	}
	void EmptyList()
	{
		CoverPointNode *Current = CoverPointNodeList,*die = NULL;
		while (Current)
		{
			Current->Empty_List();
			die = Current;
			Current = Current->next;
			delete die;
		}
		CoverPointNodeList = NULL;
	}
};

AmbushPointSystem AmbushPointSystemControl = AmbushPointSystem();

class JMG_Metroid_New_Ambush_Cover_Point : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Metroid_New_Ambush_Attack_Point : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Metroid_Move_To_Random_Ambush_Spot : public ScriptImpClass {
	AmbushPointSystem::CoverPointNode *AmbushPoint;
	AmbushPointSystem::CoverPointNode *CurrentAmbushPoint;
	Vector3 StartPos;
	Vector3 FaceLocation;
	Vector3 LastMoveToLocation;
	Vector3 MyLastPos;
	int faceReset;
	int LastEnemyID;
	bool ImAtPoint;
	bool InnateEnabled;
	int EnemySeen;
	int currentEnemyID;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Action_Complete(GameObject *obj,int action,ActionCompleteReason reason);
	void Destroyed(GameObject *obj);
	bool SelectRandomAmbushSpot(GameObject *obj);
	void QuitAmbushMove(GameObject *obj);
	void FaceAmbushLocation(GameObject *obj);
public:
	JMG_Metroid_Move_To_Random_Ambush_Spot()
	{
		AmbushPoint = NULL;
		CurrentAmbushPoint = NULL;
	}
};

class JMG_Metroid_Mine_Tower_Control : public ScriptImpClass {
	int LockDownSignIDs[3];
	int LockDownButtonIDs[3];
	bool Activated[3];
	int PendingActivation;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
};

class JMG_Metroid_Send_Custom_On_Poke : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class MetroidBossPathControl
{
public:
	struct AIPathNode;
private:
	struct LinkedAIPathNodes
	{
		struct AIPathNode *Node;
		struct LinkedAIPathNodes *next;
		LinkedAIPathNodes(AIPathNode *LinkNode)
		{
			Node = LinkNode;
			next = NULL;
		};
	};
public:
	struct AIPathNode
	{
		int PointID;
		char PathName[128];
		float MaxDeviation;
		int NumberOfLinks;
		Vector3 Position;
		struct LinkedAIPathNodes *MyLinks;
		struct AIPathNode *next;
		AIPathNode(GameObject *obj,const char *NodeName,float MaxDeviation)
		{
			sprintf(PathName,"%s",NodeName);
			MaxDeviation = abs(MaxDeviation);
			NumberOfLinks = 0;
			Position = Commands->Get_Position(obj);
			PointID = Commands->Get_ID(obj);
			MyLinks = NULL;
			next = NULL;
		}
		void AddLinkNode(AIPathNode *LinkNode)
		{
			if (!LinkNode)
				return;
			LinkedAIPathNodes *Current = MyLinks;
			if (!Current)
				MyLinks = new LinkedAIPathNodes(LinkNode);
			while (Current)
			{
				if (!Current->next)
				{
					Current->next = new LinkedAIPathNodes(LinkNode);
					return;
				}
				Current = Current->next;
			}
		}
		AIPathNode *GetNextRoadPoint()
		{
			if (NumberOfLinks == 1)
				return MyLinks->Node;
			int x = 0,Rand = Commands->Get_Random_Int(0,(NumberOfLinks+1));
			LinkedAIPathNodes *Current = MyLinks;
			while (Current)
			{
				if (Rand <= x)
					return Current->Node;
				x++;
				Current = Current->next;
				if (!Current)
					Current = MyLinks;
			}
			return NULL;
		}
		void Empty_List()
		{
			LinkedAIPathNodes *temp = MyLinks,*die;
			while (temp)
			{
				die = temp;
				temp = temp->next;
				delete die;
			}
			MyLinks = NULL;
		}
		const char *ReturnNodes()
		{
			static char String[512];
			sprintf(String,"");
			LinkedAIPathNodes *temp = MyLinks;
			while (temp)
			{
				sprintf(String,"%s%s ",String,temp->Node->PathName);
				temp = temp->next;
			}
			return String;
		}
	};
private:
	AIPathNode *AIPathNodeList;
public:
	MetroidBossPathControl()
	{
		AIPathNodeList = NULL;
	};
	AIPathNode *AddNode(GameObject *obj,const char *NodeName,float MaxDeviation)
	{
		AIPathNode *Current = AIPathNodeList;
		if (!Current)
		{
			AIPathNodeList = new AIPathNode(obj,NodeName,MaxDeviation);
			return AIPathNodeList;
		}
		while (Current)
		{
			if (!_stricmp(Current->PathName,NodeName))
			{
				char aERROR[512];
				sprintf(aERROR,"MESSAGE ERROR There is already a node with the name %s the list!",NodeName);
				Console_Input(aERROR);
				return Current;
			}
			else if (!Current->next)
			{
				Current->next = new AIPathNode(obj,NodeName,MaxDeviation);
				return Current->next;
			}
			Current = Current->next;
		}
		return NULL;
	}
	AIPathNode *Find_AIPathNode_Node(const char *NodeName)
	{
		if (!AIPathNodeList)
			return NULL;
		AIPathNode *Current = AIPathNodeList;
		while (Current)
		{
			if (!_stricmp(Current->PathName,NodeName))
				return Current;
			Current = Current->next;
		}
		return NULL;
	}
	int Find_AIPathNode_Node_ID(const char *NodeName)
	{
		if (!AIPathNodeList)
			return 0;
		AIPathNode *Current = AIPathNodeList;
		while (Current)
		{
			if (!_stricmp(Current->PathName,NodeName))
				return Current->PointID;
			Current = Current->next;
		}
		char aERROR[128];
		sprintf(aERROR,"MESSAGE Find_AIPathNode_Node_ID - ERROR: NODE ID FOR %s NOT FOUND!",NodeName);
		Console_Input(aERROR);
		return 0;
	}
	AIPathNode *GetNearestRoadNode(const Vector3 &Pos)
	{
		float ClosestDistance = 99999.0f;
		AIPathNode *Best = NULL;
		AIPathNode *Current = AIPathNodeList;
		while (Current)
		{
			float TempDistance = JmgUtility::SimpleDistance(Pos,Current->Position);
			if (TempDistance < ClosestDistance)
			{
				ClosestDistance = TempDistance;
				Best = Current;
			}
			Current = Current->next;
		}
		return Best;
	}
	void EmptyList()
	{
		AIPathNode *Current = AIPathNodeList,*die = NULL;
		while (Current)
		{
			Current->Empty_List();
			die = Current;
			Current = Current->next;
			delete die;
		}
		AIPathNodeList = NULL;
	}
	void PrintNodes()
	{
		AIPathNode *Current = AIPathNodeList;
		while (Current)
		{
			char msg[256];
			sprintf(msg,"msg Node:%s->%s",Current->PathName,Current->ReturnNodes());
			Console_Input(msg);
			Current = Current->next;
		}
	}
};

MetroidBossPathControl MetroidBossPathControlSystem = MetroidBossPathControl();

class JMG_Metroid_AI_Path_Node : public ScriptImpClass {
	MetroidBossPathControl::AIPathNode *MyNode;//Low Risk
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class MetroidSabotagedCrateSystem
{
private:
	struct CrateNode
	{
		int ID;
		float Facing;
		Vector3 Position;
		struct CrateNode *next;
		CrateNode(GameObject *obj)
		{
			ID = Commands->Get_ID(obj);
			Facing = Commands->Get_Facing(obj);
			Position = Commands->Get_Position(obj);
			next = NULL;
		}
	};
	CrateNode *CrateNodeList;
	void deleteObject(int ID)
	{
		if (!CrateNodeList)
			return;
		CrateNode *Current = CrateNodeList,*Prev = NULL;
		while (Current)
		{
			if (Current->ID == ID)
			{
				if (!Prev)
					CrateNodeList = CrateNodeList->next;
				else if (Current->next)
					Prev->next = Current->next;
				else
					Prev->next = NULL;
				delete Current;
				break;
			}
			Prev = Current;
			Current = Current->next;
		}
	}
public:
	MetroidSabotagedCrateSystem()
	{
		CrateNodeList = NULL;
	}
	MetroidSabotagedCrateSystem &operator += (GameObject *obj)
	{
		Vector3 Pos = Commands->Get_Position(obj);
		CrateNode *Current = CrateNodeList;
		if (!CrateNodeList)
			CrateNodeList = new CrateNode(obj);
		while (Current)
		{
			if (JmgUtility::SimpleDistance(Pos,Current->Position) < 0.1f)
			{
				Current->ID = Commands->Get_ID(obj);
				return *this;
			}
			if (!Current->next)
			{
				Current->next = new CrateNode(obj);
				break;
			}
			Current = Current->next;
		}
		return *this;
	};
	void RecreateCrates()
	{
		CrateNode *Current = CrateNodeList;
		while (Current)
		{
			GameObject *Crate = Commands->Create_Object("Explosive_Rigged_Trooper_Crate",Current->Position);
			Commands->Set_Facing(Crate,Current->Facing);
			Current = Current->next;
		}
	}
	void TriggerExplosives()
	{
		CrateNode *Current = CrateNodeList;
		while (Current)
		{
			GameObject *Crate = Commands->Find_Object(Current->ID);
			if (Crate)
				Commands->Apply_Damage(Crate,9999.9f,"BlamoKiller",0);
			Current = Current->next;
		}
	}
	void EmptyList()
	{
		CrateNode *Current = CrateNodeList,*die = NULL;
		while (Current)
		{
			die = Current;
			Current = Current->next;
			delete die;
		}
		CrateNodeList = NULL;
	}
};

MetroidSabotagedCrateSystem MetroidSabotagedCrateSystemControl = MetroidSabotagedCrateSystem();

#define METROIDOBJECTIVECOUNT 22
struct MetroidGame
{
private:
	char *MetroidDialogStrings(int StringID)
	{
		switch (StringID)
		{
			//case :return "";
		case 0:return "Laser wall active, find the switch!";
		case 1:return "WARNING: DOOR JAMMED! Use explosives to proceed!";
		case 2:return "Security alert cancelled.";
		case 3:return "Power failure! Please re-activate power!";
		case 4:return "Train is loaded and ready for departure.";
		case 5:return "Planetary magma core unstable. Earthquake warning at level 5.";
		case 6:return "Warning: Reactor shields offline!";
		case 7:return "Reactor shields functioning normally.";
		case 8:return "WARNING: Waste leak detected! Drains activated.";
		case 9:return "Warning: Electrified water detected! Deactivate power to proceed.";
		case 10:return "Gary was here, Ash is a loser";
		case 11:return "Lockdown cancelled, laser wall shutdown.";
		case 12:return "Security Alert Cancelled.";
		case 13:return "Test your jumping here.";
		case 14:return "Target practice.";
		case 15:return "I AM A COMPUTER, PLEASE HAVE SEX WITH ME.";
		case 16:return "Rule 29: On the internet, all girls are men, and all kids are undercover FBI agents or Perverted Justice Decoys.";
		case 17:return "Look out for more secrets like this!";
		case 18:return "Elevator disabled while Lockdown is active.";
		case 19:return "Good job, you beat the map!";
		case 20:return "Sorry: Game was won and the area you were in had to be reset.";
		case 21:return "Console has already been deactivated!";
		case 22:return "Power restored to teleporter network.";
		case 23:return "Codes to mining facility 40351 acquired.";
		case 24:return "Hazardous Anomaly Detected Quarantine Activated.";
		case 25:return "Hazardous Anomaly Eliminated, Quarantine Deactivated, have a nice day.";
		case 26:return "Intruders detected, guards dispatched.";
		case 27:return "Contact lost with guards, lockdown canceled.";
		case 28:return "Welcome to teleporter base Alpha 006. Co-ordinates have been set. Standing by.";
		case 29:return "Operation Deep Penetration: Offworld resource colony Z416 has been captured by enemy forces. Recover the mining facility at all costs.";
		case 30:return "Plecos are awesome";
		case 31:return "You will have a much easier time if you just run past the camera up ahead; the operator is not paying much attention.";
		case 32:return "Cameras are often placed near remote or unguarded passageways; avoid or destroy them before they have a chance to spot you.";
		case 33:return "You were seen by a camera, try again.";
		case 34:return "Good work, looks like you've mastered cameras!";
		case 35:return "Practice avoiding cameras.";
		case 40:return "?????";
		case 41:return "Turret Destroyed";
		case 12821:return "Press E to activate this computer terminal, press 'E' when the screen is Green to hack, 'E' on red will reset progress and shock you, 10 successful presses will hack the terminal.";
		case 12822:return "Good Job!";
		default:return "DISPLAY STRING NOT FOUND!";
		}
	}
public:
	int BossID;
	int BossMode;
	bool BossDefeated;
	int NumberOfDeactivatedTerminals;
	int MineTerminalID[3];
	bool MineTerminalDeactivated[3];
	int MineTerminalReset[3];
	bool DisplayedObjective[METROIDOBJECTIVECOUNT];
	int FinalCheckpointShieldWallID;
	bool MainElevatorDown;
	int LastPlayerCount;
	float DamageMultiplier;
	int BossDefeatedMusicID;
	bool deactivatedLaserFence;
	bool backLaserFence;
	bool reactorPowerRestored;
	bool cityShieldLowered;
	bool electrifiedWaterTurnedOff;
	bool tunnelEntryLowered;
	int winMusicId;
	MetroidGame()
	{
		LastPlayerCount = 0;
		DamageMultiplier = 0;
		MainElevatorDown = false;
		NumberOfDeactivatedTerminals = 0;
		BossID = 0;
		BossMode = 0;
		BossDefeated = false;
		BossDefeatedMusicID = 0;
		winMusicId = 0;
		for (int x = 0;x < 3;x++)
		{
			MineTerminalID[x] = 0;
			MineTerminalDeactivated[x] = false;
			MineTerminalReset[x] = 0;
		}
		for (int x = 0;x < METROIDOBJECTIVECOUNT;x++)
			DisplayedObjective[x] = false;
		deactivatedLaserFence = false;
		backLaserFence = false;
		reactorPowerRestored = false;
		cityShieldLowered = false;
		electrifiedWaterTurnedOff = false;
		tunnelEntryLowered = false;
	}
	void CheckForPlayerCountUpdate()
	{
		int PlayerCount = Get_Player_Count();
		if (LastPlayerCount == PlayerCount)
			return;
		LastPlayerCount = PlayerCount;
		if (!PlayerCount)
			return;
		DamageMultiplier = (PlayerCount/(4.000f+PlayerCount)*2.000f)-1.0f;
	}
	void DeactivateTerminal(GameObject *obj,int TerminalID)
	{
		for (int x = 0;x < 3;x++)
			if (TerminalID == MineTerminalID[x])
			{
				if (!MineTerminalDeactivated[x])
				{
					GameObject *Terminal = Commands->Find_Object(MineTerminalID[x]);
					if (Terminal)
					{
						Commands->Enable_HUD_Pokable_Indicator(Terminal,false);
						Commands->Set_Animation(Terminal,"hackterminal.hackterminal",false,0,190.0f,190.0f,false);
					}
					MineTerminalDeactivated[x] = true;
					MineTerminalReset[x] = 300;
					NumberOfDeactivatedTerminals++;
					MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(obj)]->ComputerConsolesDeactivated++;
					for (int y = 1;y < 128;y++)
					{
						GameObject *Player = Get_GameObj(y);
						if (!Player)
							continue;
						Set_Shader_Number_Vector(Player,99,Vector4(4920037.0f,(float)NumberOfDeactivatedTerminals,0.0f,0.0f));
						Commands->Send_Custom_Event(Player,Player,9870923,TerminalID,0);
					}
					MetroidObjectiveSystemControl.Set_Objective_Status(34+x,NewObjectiveSystem::Accomplished);
					if (NumberOfDeactivatedTerminals == 3)
						WinMap();
				}
				return;
			}
	}
	void DisplayHUDMessage(int StringID,const Vector3 &Color = Vector3(0.0f,0.5f,1.0f))
	{
		char text[220];
		sprintf_s(text,"%s",MetroidStringLibrary(StringID));
		for (int x = 1;x < 128;x++)
		{
			GameObject *Player = Get_GameObj(x);
			if (!Player)
				continue;
			Set_HUD_Help_Text_Player_Text(Player,7233,text,Color);
		}
	}
	void DisplayDynamicHUDMessage(int StringID,float SpecialParam1,float SpecialParam2 = 0.0f,float SpecialParam3 = 0.0f)
	{
		Vector4 Temp = Vector4(4920038.0f,SpecialParam1,SpecialParam2,SpecialParam3);
		for (int x = 1;x < 128;x++)
		{
			GameObject *Player = Get_GameObj(x);
			if (!Player)
				continue;
			Set_Shader_Number_Vector(Player,StringID,Temp);
		}
	}
	void DisplayDialogMessage(GameObject *obj,int StringID)
	{
		if (StringID == -1)
			return;
		char dialog[1000];
		sprintf(dialog,"pamsg %d %s",JmgUtility::JMG_Get_Player_ID(obj),MetroidDialogStrings(StringID));
		Console_Input(dialog);
	}
	char *Get_String(int StringID)
	{
		return MetroidDialogStrings(StringID);
	}
	void WinMap();
};
MetroidGame MetroidGameControl = MetroidGame();

class JMG_Metroid_Mine_Boss : public ScriptImpClass {
	OBBoxClass *DrillZone;// Low Risk
	float LastHealth;
	float LastArmor;
	int ZoneID;
	int SoundID;
	MetroidBossPathControl::AIPathNode *RoadPoint;// Low Risk
	int TurretID;
	int LightID[4];
	int WarningLightID;
	int EnemyID;
	int ChasingID;
	int ClearTarget;
	Vector3 RoadPointPos;
	Vector3 LastSpot;
	Vector3 CreationSpot;
	int StuckCount;
	int SuperStuckCount;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void Action_Complete(GameObject *obj,int action,ActionCompleteReason reason);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj, GameObject *damager);
	void Destroyed(GameObject *obj);
	void Goto_Location(GameObject *obj,const Vector3 &TargetLocation,bool Pathfind,float ArriveDistance,bool Backward,bool Monitor,float Speed);
	bool Check_Road_Update(const Vector3 &Pos);
	void SetupAZoneBox(GameObject *obj);
public:
	JMG_Metroid_Mine_Boss()
	{
		LastHealth = 2000.0f;
		LastArmor = 2000.0f;
		SoundID = 0;
		MetroidGameControl.BossMode = 0;
		MetroidGameControl.BossDefeated = false;
		SuperStuckCount = StuckCount = 0;
		ChasingID = EnemyID = 0;
		RoadPoint = NULL;
	}
};

class JMG_Metroid_Boss_Turret : public ScriptImpClass {
	int SeenID;
	int IgnoreID;
	int ClearTarget;
	float TargetDistance;
	float MinDistance;
	float MaxDistance;
	float RotateDirection;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Action_Complete(GameObject *obj,int action,ActionCompleteReason reason);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_Metroid_Boss_Grinder_Kill_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

#define MAXDYNAMICPOWERUPS 50
class MetroidPowerupSystem
{
private:
	struct PowerupNode
	{
		int ID;
		struct PowerupNode *next;
		PowerupNode(int id)
		{
			ID = id;
			next = NULL;
		}
	};
	int PowerupCount;
	PowerupNode *PowerupNodeList;
	void deleteObject(int ID)
	{
		if (!PowerupNodeList)
			return;
		PowerupNode *Current = PowerupNodeList,*Prev = NULL;
		while (Current)
		{
			if (Current->ID == ID)
			{
				if (!Prev)
					PowerupNodeList = PowerupNodeList->next;
				else if (Current->next)
					Prev->next = Current->next;
				else
					Prev->next = NULL;
				delete Current;
				PowerupCount--;
				break;
			}
			Prev = Current;
			Current = Current->next;
		}
	}
public:
	MetroidPowerupSystem()
	{
		PowerupCount = 0;
		PowerupNodeList = NULL;
	}
	MetroidPowerupSystem &operator += (GameObject *obj)
	{
		int ID = Commands->Get_ID(obj);
		PowerupNode *Current = PowerupNodeList;
		if (!PowerupNodeList)
		{
			PowerupCount++;
			PowerupNodeList = new PowerupNode(ID);
		}
		while (Current)
		{
			if (Current->ID == ID)
				return *this;
			if (!Current->next)
			{
				PowerupCount++;
				Current->next = new PowerupNode(ID);
				break;
			}
			Current = Current->next;
		}
		if (PowerupCount > MAXDYNAMICPOWERUPS)
		{
			GameObject *object = Commands->Find_Object(PowerupNodeList->ID);
			if (object)
				Commands->Destroy_Object(object);
		}
		return *this;
	};
	MetroidPowerupSystem &operator -= (GameObject *obj)
	{
		int ID = Commands->Get_ID(obj);
		deleteObject(ID);
		return *this;
	};
	void EmptyList()
	{
		PowerupNode *Current = PowerupNodeList,*die = NULL;
		while (Current)
		{
			die = Current;
			Current = Current->next;
			delete die;
		}
		PowerupCount = 0;
		PowerupNodeList = NULL;
	}
};

MetroidPowerupSystem MetroidPowerupSystemControl = MetroidPowerupSystem();

class JMG_Metroid_Dynamic_Powerup_System : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
};

class JMG_Metroid_Crate_Random_Powerup : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *damager);
};

#define METROIDCHAPTERS 9
class MetroidPlayerSettingsSystem
{
	struct PlayerSettingsNode
	{
		bool ChaptersCompleted[METROIDCHAPTERS];
		char PlayerName[256];
		int ArmorMode;
		float BossDamage;
		int FeedBoss;
		int IsInTeleport;
		int ScreenColorZone;
		Vector3 ScreenColor;
		float ScreenOpacity;
		int InTrainingRoom;
		int InStartRoom;
		bool hasZeroBug;
		bool hasDied;
		bool openedSecurityDoor;
		bool hasKeycard;
		struct PlayerSettingsNode *next;
		PlayerSettingsNode(const char *playerName)
		{
			for (int x = 0;x < METROIDCHAPTERS;x++)
				ChaptersCompleted[x] = false;
			sprintf(PlayerName,"%s",playerName);
			ArmorMode = -1;
			BossDamage = 0;
			FeedBoss = 0;
			IsInTeleport = 0;
			ScreenColorZone = 0;
			ScreenColor = Vector3(0.0f,0.0f,0.0f);
			ScreenOpacity = 0;
			InTrainingRoom = false;
			InStartRoom = true;
			hasZeroBug = false;
			hasDied = false;
			openedSecurityDoor = false;
			hasKeycard = false;
			next = NULL;
		}
		void ChapterCompleted(GameObject *obj,int ChapterID)
		{
			if (!Commands->Is_A_Star(obj))
				return;
			if (ChapterID >= METROIDCHAPTERS)
				return;
			if (!ChaptersCompleted[ChapterID])
			{
				MetroidScoreSystem::MetroidHighScoresNode *msNode = MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(obj)];
				switch (ChapterID)
				{
					case 0:msNode->Chapter1Completed++;break;
					case 1:msNode->Chapter2Completed++;break;
					case 2:msNode->Chapter3Completed++;break;
					case 3:msNode->Chapter4Completed++;break;
					case 4:msNode->Chapter5Completed++;break;
					case 5:msNode->Chapter6Completed++;break;
					case 6:msNode->Chapter7Completed++;break;
					case 7:msNode->Chapter8Completed++;break;
					case 8:msNode->Chapter9Completed++;break;
					case 9:msNode->Chapter10Completed++;break;
				}
				msNode->CompletedPlayThroughs = msNode->Chapter1Completed;
				if (msNode->Chapter2Completed < msNode->CompletedPlayThroughs)
					msNode->CompletedPlayThroughs = msNode->Chapter2Completed;
				if (msNode->Chapter3Completed < msNode->CompletedPlayThroughs)
					msNode->CompletedPlayThroughs = msNode->Chapter3Completed;
				if (msNode->Chapter4Completed < msNode->CompletedPlayThroughs)
					msNode->CompletedPlayThroughs = msNode->Chapter4Completed;
				if (msNode->Chapter5Completed < msNode->CompletedPlayThroughs)
					msNode->CompletedPlayThroughs = msNode->Chapter5Completed;
				if (msNode->Chapter6Completed < msNode->CompletedPlayThroughs)
					msNode->CompletedPlayThroughs = msNode->Chapter6Completed;
				if (msNode->Chapter7Completed < msNode->CompletedPlayThroughs)
					msNode->CompletedPlayThroughs = msNode->Chapter7Completed;
				if (msNode->Chapter8Completed < msNode->CompletedPlayThroughs)
					msNode->CompletedPlayThroughs = msNode->Chapter8Completed;
				if (msNode->Chapter9Completed < msNode->CompletedPlayThroughs)
					msNode->CompletedPlayThroughs = msNode->Chapter9Completed;
				ChaptersCompleted[ChapterID] = true;
				int TotalChaptersCompleted = 0;
				for (int x = 0;x < METROIDCHAPTERS;x++)
					if (ChaptersCompleted[x])
						TotalChaptersCompleted++;
				char ChapterNames[METROIDCHAPTERS][128] = {"Ruined","We need moar power!","Ship Wrecked","Pitfall","Trained on victory","Bad Traffic","Freeze Dried","Going Deep","Those are mine resources"};
				char chpmsg[512];
				sprintf(chpmsg,"Chapter %d: \"%s\" Complete (%d/%d)",ChapterID+1,ChapterNames[ChapterID],TotalChaptersCompleted,METROIDCHAPTERS);
				JmgUtility::DisplayChatMessage(obj,220,220,220,chpmsg);
				if (msNode->Chapter1Completed && msNode->Chapter2Completed && msNode->Chapter3Completed && msNode->Chapter4Completed && msNode->Chapter5Completed && msNode->Chapter6Completed && msNode->Chapter7Completed && msNode->Chapter8Completed && msNode->Chapter9Completed)
				{
					if (!CheckIfPlayerHasPerkUnlocked(obj,79))
					{
						PerkSystemIncreasePlayerPerkUnlockAmount(obj,79,1);
						IncreasePlayerPerkSlotCount(obj);
						JmgUtility::DisplayChatMessage(obj,220,220,220,"Operation: Deep Penetration completed!");
					}
					else if (ChapterID == 8)
						JmgUtility::DisplayChatMessage(obj,220,220,220,"Operation: Deep Penetration completed!");
				}
			}
		}
		int GetChaptersCompleted()
		{
			int TotalChaptersCompleted = 0;
			for (int x = 0;x < METROIDCHAPTERS;x++)
				if (ChaptersCompleted[x])
					TotalChaptersCompleted++;
			return TotalChaptersCompleted;
		}
	};
	struct PlayerSettingsNode *PlayerSettingsNodeList;
	struct PlayerSettingsNode *PlaceHolderPlayerSettingsNode;
	PlayerSettingsNode *FindPlayerNode(const char *PlayerName)
	{
		PlayerSettingsNode *Current = PlayerSettingsNodeList;
		if (!PlayerSettingsNodeList)
		{
			PlayerSettingsNodeList = new PlayerSettingsNode(PlayerName);
			return PlayerSettingsNodeList;
		}
		while (Current)
		{
			if (!_stricmp(Current->PlayerName,PlayerName))
				return Current;
			if (!Current->next)
			{
				Current->next = new PlayerSettingsNode(PlayerName);
				return Current->next;
			}
			Current = Current->next;
		}
		return NULL;
	}
public:
	PlayerSettingsNode *PlayerSettings[128];
	MetroidPlayerSettingsSystem()
	{
		PlaceHolderPlayerSettingsNode = new PlayerSettingsNode("\0");
		PlayerSettingsNodeList = NULL;
		for (int x = 0;x < 128;x++)
			PlayerSettings[x] = PlaceHolderPlayerSettingsNode;
	}
	void SpawnPlayer(int PlayerID)
	{
		if (!PlayerID)
			return;
		const char *PlayerName = Get_Player_Name_By_ID(PlayerID);
		if (!PlayerSettings[PlayerID] || _stricmp(PlayerSettings[PlayerID]->PlayerName,PlayerName))
			PlayerSettings[PlayerID] = FindPlayerNode(PlayerName);
		PlayerSettings[PlayerID]->hasDied = false;
		PlayerSettings[PlayerID]->openedSecurityDoor = false;
		PlayerSettings[PlayerID]->hasKeycard = false;
	}
	void BossKilled()
	{
		float TotalDamage = 0;
		int PlayerCount = 0;
		PlayerSettingsNode *Current = PlayerSettingsNodeList;
		while (Current)
		{
			if (Current->BossDamage)
			{
				PlayerCount++;
				TotalDamage += Current->BossDamage;
			}
			Current = Current->next;
		}
		float Average = (TotalDamage/(float)PlayerCount)*0.5f;
		for (int x = 1;x < 128;x++)
		{
			GameObject *Player = Get_GameObj(x);
			if (!Player)
				continue;
			if (PlayerSettings[x]->BossDamage >= Average)
			{
				PerkSystemIncreasePlayerPerkUnlockAmount(Player,78,1);
				Commands->Give_PowerUp(Player,"POW_Robotic_Supergrenade",false);
				if (Has_Weapon(Player,"Weapon_Robotic_Supergrenade"))
					Commands->Select_Weapon(Player,"Weapon_Robotic_Supergrenade");
			}
			MetroidGameControl.DisplayHUDMessage(17);
		}
	}
	void EmptyList()
	{
		PlayerSettingsNode *Current = PlayerSettingsNodeList,*die = NULL;
		while (Current)
		{
			die = Current;
			Current = Current->next;
			delete die;
		}
		PlayerSettingsNodeList = NULL;
		for (int x = 0;x < 128;x++)
			PlayerSettings[x] = PlaceHolderPlayerSettingsNode;
	}
};
MetroidPlayerSettingsSystem MetroidPSSControl;

inline void JMGGivePoints(GameObject *obj,float points,int playerId)
{
	Commands->Give_Points(obj,points,false);
	MetroidScoreControl.MetroidPlayerHighScoreNodes[playerId]->pointsThisRound += points;
	MetroidScoreControl.MetroidPlayerHighScoreNodes[playerId]->Points += points;
}

class JMG_Metroid_Mine_Tower_Elevator : public ScriptImpClass {
	int ElevatorCurrentFloor;
	bool ElevatorInMotion;
	int ElevatorButtonIDs[4][5];
	int LockdownMode;
	int LockDownButtonID;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
	void UpdateElevatorButtons();
};


class JMG_Metroid_Text_Dialog : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	char *MetroidDialogStrings(int StringID);
};

class JMG_Metroid_Mine_Computer_Console : public ScriptImpClass {
	int TerminalNumber;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Poked(GameObject *obj,GameObject *poker);
};

class JMG_Metroid_Mine_Computer_Console_Disarm_Script : public ScriptImpClass {
	int CompletedCombos;
	int NextExpectedKey;
	int LastSelection;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *killer);
	void SelectRandomKey(GameObject *obj);
	void PressKey(GameObject *obj,int message);
};

class JMG_Metroid_Reset_Fall_Distance_On_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Metroid_Recover_Fall_Damage_Once : public ScriptImpClass {
	bool Fall;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Metroid_Teleport_Zone : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Metroid_Tower_Put_AI_Down_Zone: public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Metroid_Electrified_Water : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Metroid_Set_Screen_Color_While_In_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exiter);
};

class SupportModeSystem
{
	int MedicalPackCount;
	struct MedicalPackNode
	{
		int ID;
		struct MedicalPackNode *next;
		MedicalPackNode(GameObject *obj)
		{
			ID = Commands->Get_ID(obj);
			next = NULL;
		}
	};
	MedicalPackNode *MedicalPackNodeList;
public:
	int GeneratorID;
	int LastDamaged;
	int LastFired;
	SupportModeSystem()
	{
		GeneratorID = 0;
		MedicalPackCount = 0;
		LastDamaged = 0;
		LastFired = 0;
		MedicalPackNodeList = NULL;
	}
	int FindFirstID()
	{
		if (!MedicalPackNodeList)
			return 0;
		MedicalPackNode *Current = MedicalPackNodeList;
		while (Current)
		{
			if (Current->ID)
				return Current->ID;
			Current = Current->next;
		}
		return 0;
	}
	void AddMedicalPack(GameObject *obj)
	{
		MedicalPackCount++;
		MedicalPackNode *Current = MedicalPackNodeList;
		if (!MedicalPackNodeList)
			MedicalPackNodeList = new MedicalPackNode(obj);
		while (Current)
		{
			if (!Current->next)
			{
				Current->next = new MedicalPackNode(obj);
				break;
			}
			Current = Current->next;
		}
		if (MedicalPackCount > 3)
		{
			GameObject *object = Commands->Find_Object(FindFirstID());
			if (object)
				Commands->Destroy_Object(object);
		}
	}
	void RemoveMedicalPack(GameObject *obj)
	{
		int ID = Commands->Get_ID(obj);
		MedicalPackNode *Current = MedicalPackNodeList,*Prev = NULL;
		while (Current)
		{
			if (Current->ID == ID)
			{
				MedicalPackCount--;
				Current->ID = 0;
				return;
			}
			Prev = Current;
			Current = Current->next;
		}
	}
	void EmptyList()
	{
		MedicalPackNode *Current = MedicalPackNodeList,*die = NULL;
		while (Current)
		{
			die = Current;
			Current = Current->next;
			delete die;
		}
		MedicalPackCount = 0;
		MedicalPackNodeList = NULL;
	}
};

SupportModeSystem SupportModeSystemControl[128] = {SupportModeSystem()};

class JMG_Metroid_Medic : public ScriptImpClass {
	int LastMedicalKitAmmo;
	int BlockMedicalKitRegen;
	int LastGeneratorAmmo;
	int BlockGeneratorRegen;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Metroid_Support_Health_Powerup_Beacon : public ScriptImpClass {
	int PlayerID;
	void Created(GameObject *obj);
	//void Timer_Expired(GameObject *obj,int number);
};

class JMG_Metroid_Support_Health_Powerup : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
};

class JMG_Metroid_Support_Generator_Powerup_Beacon : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Metroid_Support_Generator_Powerup : public ScriptImpClass {
	int PlayerID;
	int currentCount;
	float vampireShield;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
};

class JMG_Metroid_AI_Hunt_Attack : public ScriptImpClass {
	Vector3 MoveToLocation;
	Vector3 LastMoveToLocation;
	Vector3 StartPosition;
	Vector3 MyLastPos;
	float MaxRange;
	int EnemySeen;
	int EnemyID;
	int LastEnemyID;
	bool InnateEnabled;
	bool StarInRange;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void Action_Complete(GameObject *obj,int action,ActionCompleteReason reason);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Update(GameObject *obj);
	void GetClosestStar(GameObject *obj);
};

class JMG_Metroid_Powered_Teleport_Zone : public ScriptImpClass {
	bool Enabled;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enter);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Metroid_Send_Custom_On_Zone_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Metroid_Sabotaged_Crate : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Metroid_Enable_Spawners_On_Death : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *damager);
};

class RoomLockdownSystem
{
public:
	int LockDownInitiatedStringID;
	int LockDownAvertedStringID;
	char LockDownInitiatedSound[512];
	char LockDownAvertedSound[512];
	int HUDMessageLockdownInitiated;
	int HUDMessageLockdownAverted;
	int ResetLockoutTime;
private:
	int ResetLockout;
	Vector3 SpeakerPosition;
	bool LockdownInProgress;
	int CancelLockdownTerminalID;
	int EnemyArrivalDelay;
	struct SpawnedEnemyNode
	{
		int ID;
		int DeadTime;
		struct SpawnedEnemyNode *next;
		SpawnedEnemyNode(GameObject *obj)
		{
			ID = Commands->Get_ID(obj);
			DeadTime = 0;
			next = NULL;
		}
	};
	struct EnemySpawnNode
	{
		int NodeID;
		int ID;
		int RemainingSpawnTime;
		int CurrentSpawnedAtATimeCount;
		int CurrentTotalSpawnCount;
		int MaxSpawnedAtATime;
		int TotalSpawnLimit;
		int SpawnRate;
		int RespawnDelay;
		int RandomRespawnDelay;
		Vector3 Position;
		float Facing;
		char SpawnPresetName[1024];
		struct SpawnedEnemyNode *SpawnedEnemyNodeList;
		struct EnemySpawnNode *next;
		EnemySpawnNode(GameObject *obj,const char *PresetName,int maxSpawnedAtATime,int totalSpawnLimit,int spawnRate,int respawnDelay,int randomRespawnDelay,int nodeID)
		{
			NodeID = nodeID;
			ID = Commands->Get_ID(obj);
			CurrentSpawnedAtATimeCount = 0;
			CurrentTotalSpawnCount = 0;
			MaxSpawnedAtATime = maxSpawnedAtATime;
			TotalSpawnLimit = totalSpawnLimit;
			RespawnDelay = respawnDelay;
			RandomRespawnDelay = randomRespawnDelay;
			SpawnRate = spawnRate;
			RemainingSpawnTime = SpawnRate;
			Facing = Commands->Get_Facing(obj);
			Position = Commands->Get_Position(obj);
			Commands->Destroy_Object(obj);
			sprintf(SpawnPresetName,"%s",PresetName);
			SpawnedEnemyNodeList = NULL;
			next = NULL;
		}
		void SpawnEnemy()
		{
			CurrentTotalSpawnCount++;
			CurrentSpawnedAtATimeCount++;
			GameObject *AI = Commands->Create_Object(this->SpawnPresetName,this->Position);
			Commands->Set_Facing(AI,this->Facing);
			char params[512];
			sprintf(params,"%d",NodeID);
			Commands->Attach_Script(AI,"JMG_Metroid_Lockdown_zSpawned_Unit",params);
			SpawnedEnemyNode *Current = SpawnedEnemyNodeList;
			if (!SpawnedEnemyNodeList)
				SpawnedEnemyNodeList = new SpawnedEnemyNode(AI);
			while (Current)
			{
				if (!Current->next)
				{
					Current->next = new SpawnedEnemyNode(AI);
					break;
				}
				Current = Current->next;
			}
		}
		bool EnemyKilled(int killedId)
		{
			SpawnedEnemyNode *Current = SpawnedEnemyNodeList;
			while (Current)
			{
				if (Current->ID == killedId)
				{
					if (RandomRespawnDelay)
						Current->DeadTime = RespawnDelay+Commands->Get_Random_Int(-RandomRespawnDelay,RandomRespawnDelay);
					else
						Current->DeadTime = RespawnDelay;
					if (Current->DeadTime <= 0)
						Current->DeadTime = 1;
					return true;
				}
				Current = Current->next;
			}
			return false;
		}
		void RemoveEnemy(int removedId)
		{
			if (!SpawnedEnemyNodeList)
				return;
			SpawnedEnemyNode *Current = SpawnedEnemyNodeList,*Prev = NULL;
			while (Current)
			{
				if (Current->ID == removedId)
				{
					if (!Prev)
						SpawnedEnemyNodeList = SpawnedEnemyNodeList->next;
					else if (Current->next)
						Prev->next = Current->next;
					else
						Prev->next = NULL;
					delete Current;
					CurrentSpawnedAtATimeCount--;
					return;
				}
				Prev = Current;
				Current = Current->next;
			}
		}
		void Update()
		{
			SpawnedEnemyNode *Current = SpawnedEnemyNodeList;
			while (Current)
			{
				if (Current->DeadTime)
				{
					Current->DeadTime--;
					if (!Current->DeadTime)
						RemoveEnemy(Current->ID);
				}
				Current = Current->next;
			}
			if (RemainingSpawnTime && (this->MaxSpawnedAtATime > this->CurrentSpawnedAtATimeCount && (!this->TotalSpawnLimit || this->TotalSpawnLimit > this->CurrentTotalSpawnCount)))
			{
				RemainingSpawnTime--;
				if (!RemainingSpawnTime)
				{
					RemainingSpawnTime = SpawnRate;
					SpawnEnemy();
				}
			}
		}
		void ResetSystem()
		{
			SpawnedEnemyNode *Current = SpawnedEnemyNodeList,*Temp;
			while (Current)
			{
				Temp = Current;
				Current = Current->next;
				delete Temp;
			}
			SpawnedEnemyNodeList = NULL;
			CurrentTotalSpawnCount = 0;
			CurrentSpawnedAtATimeCount = 0;
		}
		~EnemySpawnNode()
		{
			SpawnedEnemyNode *Current = SpawnedEnemyNodeList,*die = NULL;
			while (Current)
			{
				die = Current;
				Current = Current->next;
				delete die;
			}
			SpawnedEnemyNodeList = NULL;
		}
	};
	EnemySpawnNode *EnemySpawnNodeList;
	struct LockDownLightNode
	{
		int ID;
		int LightEffectIDs[4];
		char ModelName[16];
		struct LockDownLightNode *next;
		LockDownLightNode(GameObject *obj)
		{
			sprintf(ModelName,"%s",Get_Model(obj));
			char Anim[32];
			sprintf(Anim,"%sOf",ModelName);
			Commands->Set_Model(obj,Anim);
			sprintf(Anim,"%sOf.%sOf",ModelName,ModelName);
			Commands->Set_Animation(obj,Anim,false,0,0,0,false);
			ID = Commands->Get_ID(obj);
			for (int x = 0;x < 4;x++)
				LightEffectIDs[x] = 0;
			next = NULL;
		}
	};
	LockDownLightNode *LockDownLightNodeList;
	struct LockdownBlockerNode
	{
		int ID;
		char PresetName[1024];
		Vector3 Position;
		float Facing;
		struct LockdownBlockerNode *next;
		LockdownBlockerNode(GameObject *obj,bool destroy = true)
		{
			ID = Commands->Get_ID(obj);
			sprintf(PresetName,"%s",Commands->Get_Preset_Name(obj));
			Position = Commands->Get_Position(obj);
			Facing = Commands->Get_Facing(obj);
			if (destroy)
				Commands->Destroy_Object(obj);
			next = NULL;
		}
	};
	LockdownBlockerNode *LockdownBlockerNodeList;
	LockdownBlockerNode *PostLockdownBlockerNodeList;
	LockdownBlockerNode *PreLockdownBlockerNodeList;
	struct LockdownSendCustomObjectNode
	{
		int ID;
		int lockdownMsg;
		int preLockdownMsg;
		int postLockdownMsg;
		struct LockdownSendCustomObjectNode *next;
		LockdownSendCustomObjectNode(GameObject *obj,int lockdownMsg,int preLockdownMsg,int postLockdownMsg)
		{
			this->ID = Commands->Get_ID(obj);
			this->lockdownMsg = lockdownMsg;
			this->preLockdownMsg = preLockdownMsg;
			this->postLockdownMsg = postLockdownMsg;
			this->next = NULL;
		}
		void sendMessage(int lockdownState)
		{
			if (!ID)
				return;
			GameObject *obj = Commands->Find_Object(ID);
			switch (lockdownState)
			{
			case 0:if (preLockdownMsg)Commands->Send_Custom_Event(obj,obj,preLockdownMsg,0,0);break;
			case 1:if (lockdownMsg)Commands->Send_Custom_Event(obj,obj,lockdownMsg,0,0);break;
			case 2:if (postLockdownMsg)Commands->Send_Custom_Event(obj,obj,postLockdownMsg,0,0);break;
			}
		}
	};
	LockdownSendCustomObjectNode *LockdownSendCustomObjectNodeList;
public:
	RoomLockdownSystem()
	{
		this->HUDMessageLockdownInitiated = 19;
		this->HUDMessageLockdownAverted = 18;
		this->ResetLockout = 0;
		this->CancelLockdownTerminalID = 0;
		this->SpeakerPosition = Vector3(0.0f,0.0f,0.0f);
		this->LockDownInitiatedStringID = 24;
		this->LockDownAvertedStringID = 25;
		sprintf(this->LockDownInitiatedSound,"LockDownInitiated.wav");
		sprintf(this->LockDownAvertedSound,"LockDownAverted.wav");
		this->LockdownInProgress = false;
		this->EnemySpawnNodeList = NULL;
		this->LockDownLightNodeList = NULL;
		this->LockdownBlockerNodeList = NULL;
		this->PostLockdownBlockerNodeList = NULL;
		this->PreLockdownBlockerNodeList = NULL;
		this->LockdownSendCustomObjectNodeList = NULL;
	}
	bool BeginLockDown(int enemyArrivalDelay,int LockDownID)
	{
		if (LockdownInProgress || ResetLockout)
			return false;
		if (HUDMessageLockdownInitiated >= 0)
			MetroidGameControl.DisplayHUDMessage(HUDMessageLockdownInitiated);
		EnemyArrivalDelay = enemyArrivalDelay;
		GameObject *LockdownTerminal = Commands->Find_Object(CancelLockdownTerminalID);
		if (LockdownTerminal)
		{
			char params[32];
			sprintf(params,"%d",LockDownID);
			Commands->Attach_Script(LockdownTerminal,"JMG_Metroid_Lockdown_zComputer_Console_Special_Disarm_Script",params);
			char Animation[32];
			sprintf(Animation,"%s.%s",Get_Model(LockdownTerminal),Get_Model(LockdownTerminal));
			Commands->Set_Animation(LockdownTerminal,Animation,false,0,0,0,false);
			Commands->Enable_HUD_Pokable_Indicator(LockdownTerminal,true);
		}
		LockdownInProgress = true;
		if (LockDownInitiatedStringID >= 0)
			JmgUtility::MessageAllPlayers(127,10,10,MetroidGameControl.Get_String(LockDownInitiatedStringID));
		Commands->Create_Sound(LockDownInitiatedSound,SpeakerPosition,0);
		DestroyPlacedBlockers(PostLockdownBlockerNodeList);
		DestroyPlacedBlockers(PreLockdownBlockerNodeList);
		CreatePlacedBlockers(LockdownBlockerNodeList);
		SendLockdownCustoms(1);
		LockDownLightNode *Current2 = LockDownLightNodeList;
		while (Current2)
		{
			GameObject *Light = Commands->Find_Object(Current2->ID);
			if (Light)
			{
				float AnimationFrame = Get_Animation_Frame(Light);
				Commands->Set_Model(Light,Current2->ModelName);
				char Anim[32];
				sprintf(Anim,"%s.%s",Current2->ModelName,Current2->ModelName);
				Commands->Set_Animation(Light,Anim,true,0,AnimationFrame,-1,false);
				char bonename[32];
				for (int x = 0;x < 4;x++)
				{
					GameObject *LightEffect = Commands->Create_Object("Warning_Light_Effect",Commands->Get_Position(Light));
					sprintf(bonename,"LightPos%d",x+1);
					Commands->Attach_To_Object_Bone(LightEffect,Light,bonename);
					Current2->LightEffectIDs[x] = Commands->Get_ID(LightEffect);
				}
			}
			Current2 = Current2->next;
		}
		return true;
	}
	void EndLockDown(bool resetEnemySpawners,int lockdownLockoutTime)
	{
		if (!LockdownInProgress)
			return;
		ResetLockoutTime = lockdownLockoutTime;
		if (HUDMessageLockdownAverted >= 0)
			MetroidGameControl.DisplayHUDMessage(HUDMessageLockdownAverted);
		LockdownInProgress = false;
		ResetLockout = ResetLockoutTime;
		GameObject *LockdownTerminal = Commands->Find_Object(CancelLockdownTerminalID);
		if (LockdownTerminal)
		{
			Remove_All_Scripts(LockdownTerminal);
			char Animation[32];
			sprintf(Animation,"%s.%s",Get_Model(LockdownTerminal),Get_Model(LockdownTerminal));
			Commands->Set_Animation(LockdownTerminal,Animation,false,0,0,0,false);
			Commands->Enable_HUD_Pokable_Indicator(LockdownTerminal,false);
		}
		for (int y = 1;y < 128;y++)
		{
			GameObject *Player = Get_GameObj(y);
			if (!Player)
				continue;
			Commands->Send_Custom_Event(Player,Player,9870924,0,0);
		}
		if (LockDownAvertedStringID >= 0)
			JmgUtility::MessageAllPlayers(127,10,10,MetroidGameControl.Get_String(LockDownAvertedStringID));
		Commands->Create_Sound(LockDownAvertedSound,SpeakerPosition,0);
		DestroyPlacedBlockers(LockdownBlockerNodeList);
		if (ResetLockoutTime)
		{
			SendLockdownCustoms(2);
			CreatePlacedBlockers(PostLockdownBlockerNodeList);
		}
		else
		{
			SendLockdownCustoms(0);
			CreatePlacedBlockers(PreLockdownBlockerNodeList);
		}
		LockDownLightNode *Current2 = LockDownLightNodeList;
		while (Current2)
		{
			GameObject *Light = Commands->Find_Object(Current2->ID);
			if (Light)
			{
				float AnimationFrame = Get_Animation_Frame(Light);
				char Anim[32];
				sprintf(Anim,"%sOf",Current2->ModelName);
				Commands->Set_Model(Light,Anim);
				sprintf(Anim,"%sOf.%sOf",Current2->ModelName,Current2->ModelName);
				Commands->Set_Animation(Light,Anim,false,0,AnimationFrame,AnimationFrame,false);
			}
			for (int x = 0;x < 4;x++)
			{
				GameObject *LightEffect = Commands->Find_Object(Current2->LightEffectIDs[x]);
				if (LightEffect)
					Commands->Destroy_Object(LightEffect);
			}
			Current2 = Current2->next;
		}
		if (resetEnemySpawners)
		{
			EnemySpawnNode *Current3 = EnemySpawnNodeList;
			while (Current3)
			{
				Current3->ResetSystem();
				Current3 = Current3->next;
			}
		}
	}
	void AddSpawnPoint(GameObject *obj,const char *PresetName,int NodeID,int MaxSpawnedAtATime,int TotalSpawnLimit,int SpawnRate,int RespawnDelay,int RandomRespawnDelay)
	{
		int ID = Commands->Get_ID(obj);
		EnemySpawnNode *Current = EnemySpawnNodeList;
		if (!EnemySpawnNodeList)
			EnemySpawnNodeList = new EnemySpawnNode(obj,PresetName,MaxSpawnedAtATime,TotalSpawnLimit,SpawnRate,RespawnDelay,RandomRespawnDelay,NodeID);
		while (Current)
		{
			if (Current->ID == ID)
				return;
			if (!Current->next)
			{
				Current->next = new EnemySpawnNode(obj,PresetName,MaxSpawnedAtATime,TotalSpawnLimit,SpawnRate,RespawnDelay,RandomRespawnDelay,NodeID);
				break;
			}
			Current = Current->next;
		}
	};
	void AddLockdownLight(GameObject *obj)
	{
		int ID = Commands->Get_ID(obj);
		LockDownLightNode *Current = LockDownLightNodeList;
		if (!LockDownLightNodeList)
			LockDownLightNodeList = new LockDownLightNode(obj);
		while (Current)
		{
			if (Current->ID == ID)
				return;
			if (!Current->next)
			{
				Current->next = new LockDownLightNode(obj);
				break;
			}
			Current = Current->next;
		}
	};
	void RemoveLockdownLight(GameObject *obj)
	{
		int ID = Commands->Get_ID(obj);
		if (!LockDownLightNodeList)
			return;
		LockDownLightNode *Current = LockDownLightNodeList,*Prev = NULL;
		while (Current)
		{
			if (Current->ID == ID)
			{
				if (!Prev)
					LockDownLightNodeList = LockDownLightNodeList->next;
				else if (Current->next)
					Prev->next = Current->next;
				else
					Prev->next = NULL;
				delete Current;
				break;
			}
			Prev = Current;
			Current = Current->next;
		}
	};
	void AddLockdownBlocker(GameObject *obj)
	{
		int ID = Commands->Get_ID(obj);
		LockdownBlockerNode *Current = LockdownBlockerNodeList;
		if (!LockdownBlockerNodeList)
			LockdownBlockerNodeList = new LockdownBlockerNode(obj);
		while (Current)
		{
			if (Current->ID == ID)
				return;
			if (!Current->next)
			{
				Current->next = new LockdownBlockerNode(obj);
				return;
			}
			Current = Current->next;
		}
	};
	void AddLockdownPostBlocker(GameObject *obj)
	{
		int ID = Commands->Get_ID(obj);
		LockdownBlockerNode *Current = PostLockdownBlockerNodeList;
		if (!PostLockdownBlockerNodeList)
			PostLockdownBlockerNodeList = new LockdownBlockerNode(obj);
		while (Current)
		{
			if (Current->ID == ID)
				return;
			if (!Current->next)
			{
				Current->next = new LockdownBlockerNode(obj);
				return;
			}
			Current = Current->next;
		}
	};
	void AddLockdownPreBlocker(GameObject *obj)
	{
		int ID = Commands->Get_ID(obj);
		LockdownBlockerNode *Current = PreLockdownBlockerNodeList;
		if (!PreLockdownBlockerNodeList)
			PreLockdownBlockerNodeList = new LockdownBlockerNode(obj,false);
		while (Current)
		{
			if (Current->ID == ID)
				return;
			if (!Current->next)
			{
				Current->next = new LockdownBlockerNode(obj,false);
				return;
			}
			Current = Current->next;
		}
	};
	void AddLockdownCustomObject(GameObject *obj,int lockdownMsg,int preLockdownMsg,int postLockdownMsg)
	{
		int ID = Commands->Get_ID(obj);
		LockdownSendCustomObjectNode *Current = LockdownSendCustomObjectNodeList;
		if (!LockdownSendCustomObjectNodeList)
			LockdownSendCustomObjectNodeList = new LockdownSendCustomObjectNode(obj,lockdownMsg,preLockdownMsg,postLockdownMsg);
		while (Current)
		{
			if (Current->ID == ID)
				return;
			if (!Current->ID)
			{
				Current->ID = ID;
				Current->lockdownMsg = lockdownMsg;
				Current->postLockdownMsg = postLockdownMsg;
				Current->preLockdownMsg = preLockdownMsg;
				return;
			}
			if (!Current->next)
			{
				Current->next = new LockdownSendCustomObjectNode(obj,lockdownMsg,preLockdownMsg,postLockdownMsg);
				return;
			}
			Current = Current->next;
		}
	};
	void RemoveLockdownCustomObject(GameObject *obj)
	{
		int ID = Commands->Get_ID(obj);
		LockdownSendCustomObjectNode *Current = LockdownSendCustomObjectNodeList;
		while (Current)
		{
			if (Current->ID == ID)
				Current->ID = 0;
			Current = Current->next;
		}
	};
	void SendLockdownCustoms(int lockdownState)
	{
		LockdownSendCustomObjectNode *Current = LockdownSendCustomObjectNodeList;
		while (Current)
		{
			if (Current->ID)
				Current->sendMessage(lockdownState);
			Current = Current->next;
		}
	};
	void CreatePlacedBlockers(LockdownBlockerNode *Current)
	{
		while (Current)
		{
			GameObject *Blocker = Commands->Create_Object(Current->PresetName,Current->Position);
			Commands->Set_Facing(Blocker,Current->Facing);
			Current->ID = Commands->Get_ID(Blocker);
			Current = Current->next;
		}
	}
	void DestroyPlacedBlockers(LockdownBlockerNode *Current)
	{
		while (Current)
		{
			GameObject *LockDownBlocker = Commands->Find_Object(Current->ID);
			if (LockDownBlocker)
				Commands->Destroy_Object(LockDownBlocker);
			Current = Current->next;
		}
	}
	void SpawnedUnitKilled(GameObject *obj)
	{
		int ID = Commands->Get_ID(obj);
		EnemySpawnNode *Current = EnemySpawnNodeList;
		while (Current)
		{
			if (Current->EnemyKilled(ID))
				return;
			Current = Current->next;
		}
	};
	void Update()
	{
		if (EnemyArrivalDelay)
			EnemyArrivalDelay--;
		if (!LockdownInProgress)
		{
			if (ResetLockout)
			{
				ResetLockout--;
				if (!ResetLockout)
				{
					DestroyPlacedBlockers(PostLockdownBlockerNodeList);
					CreatePlacedBlockers(PreLockdownBlockerNodeList);
					SendLockdownCustoms(0);
				}
			}
			return;
		}
		if (!EnemyArrivalDelay)
		{
			EnemySpawnNode *Current = EnemySpawnNodeList;
			while (Current)
			{
				Current->Update();
				Current = Current->next;
			}
		}
	}
	void SetSpeakerLocation(const Vector3 &Pos)
	{
		SpeakerPosition = Pos;
	}
	void SetLockdownTerminal(GameObject *obj)
	{
		CancelLockdownTerminalID = Commands->Get_ID(obj);
		char Animation[32];
		sprintf(Animation,"%s.%s",Get_Model(obj),Get_Model(obj));
		Commands->Set_Animation(obj,Animation,false,0,0,0,false);
	};
	void EmptyList()
	{
		{
			EnemySpawnNode *Current = EnemySpawnNodeList,*die = NULL;
			while (Current)
			{
				die = Current;
				Current = Current->next;
				delete die;
			}
			EnemySpawnNodeList = NULL;
		}
		{
			LockDownLightNode *Current = LockDownLightNodeList,*die = NULL;
			while (Current)
			{
				die = Current;
				Current = Current->next;
				delete die;
			}
			LockDownLightNodeList = NULL;
		}
		{
			LockdownBlockerNode *Current = LockdownBlockerNodeList,*die = NULL;
			while (Current)
			{
				die = Current;
				Current = Current->next;
				delete die;
			}
			LockdownBlockerNodeList = NULL;
		}
		{
			LockdownBlockerNode *Current = PostLockdownBlockerNodeList,*die = NULL;
			while (Current)
			{
				die = Current;
				Current = Current->next;
				delete die;
			}
			PostLockdownBlockerNodeList = NULL;
		}
		{
			LockdownBlockerNode *Current = PreLockdownBlockerNodeList,*die = NULL;
			while (Current)
			{
				die = Current;
				Current = Current->next;
				delete die;
			}
			PreLockdownBlockerNodeList = NULL;
		}
		{
			LockdownSendCustomObjectNode *Current = LockdownSendCustomObjectNodeList,*die = NULL;
			while (Current)
			{
				die = Current;
				Current = Current->next;
				delete die;
			}
			LockdownSendCustomObjectNodeList = NULL;
		}
		this->RoomLockdownSystem::RoomLockdownSystem();
	}
	bool GetLockdownInProgress()
	{
		return LockdownInProgress;
	}
};

#define LOCKDOWNZONECOUNT 9
RoomLockdownSystem RoomLockdownControl[LOCKDOWNZONECOUNT] = {RoomLockdownSystem()};

class JMG_Metroid_Lockdown_Controller : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Metroid_Lockdown_Lockdown_Object : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Metroid_Lockdown_Post_Lockdown_Object : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Metroid_Lockdown_Pre_Lockdown_Object : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Metroid_Lockdown_Unit_Spawner : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Metroid_Lockdown_Light : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Metroid_Lockdown_Lockdown_Send_Custom_Object : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
};

class JMG_Metroid_Lockdown_zSpawned_Unit : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *damager);
};

class JMG_Metroid_Lockdown_Trigger_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Metroid_Lockdown_Disable_Lockdown_Terminal : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Metroid_Lockdown_zComputer_Console : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JMG_Metroid_Lockdown_zComputer_Console_Disarm_Script : public ScriptImpClass {
	int CompletedCombos;
	int NextExpectedKey;
	int LastSelection;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *killer);
	void SelectRandomKey(GameObject *obj);
	void PressKey(GameObject *obj,int message);
};

class JMG_Metroid_Lockdown_Disable_Lockdown_On_Custom_Count : public ScriptImpClass {
	float MessageCount;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Metroid_Lockdown_Disable_Lockdown_On_Death : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JMG_Metroid_Lockdown_Trigger_On_Message : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Metroid_Lockdown_Disable_Lockdown_On_Time_Without_Message : public ScriptImpClass {
	int Count;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Metroid_Complete_Chapter : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Metroid_Defense_Mode : public ScriptImpClass {
	int LastGunAmmo;
	int BlockSupportGunRegen;
	int LastTelepadAmmo;
	int BlockTelepadRegen;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
};

class JMG_Metroid_Defense_Gun_Beacon : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Metroid_Defense_Gun_Powerup : public ScriptImpClass {
	int Count;
	int PlayerID;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
};

class JMG_Metroid_Defense_Telepad_Beacon : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Metroid_Defense_Telepad_Powerup : public ScriptImpClass {
	int dieTime;
	int Count;
	int PlayerID;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
};

class DefenseModeSystem
{
public:
	struct TelepadNode
	{
		int PlayerID;
		int ID;
		int ZoneID;
		Vector3 Pos;
		int playerDeathCount;
		struct TelepadNode *next;
		TelepadNode(GameObject *telepad,int playerID)
		{
			PlayerID = playerID;
			ID = Commands->Get_ID(telepad);
			Pos = Commands->Get_Position(telepad);
			Matrix3 rotation(true);
			rotation.Rotate_Z(Commands->Get_Facing(telepad));
			OBBoxClass TheBox(Pos,Vector3(0.5f,0.5f,0.5f),rotation);
			GameObject *theZone = Create_Zone("Script_Zone_All",TheBox);
			char params[32];
			sprintf(params,"%d,%d",PlayerID,ID);
			Commands->Attach_Script(theZone,"JMG_Metroid_Defense_Telepad_Zone",params);
			ZoneID = Commands->Get_ID(theZone);
			playerDeathCount = 0;
			next = NULL;
		}
	};
private:
	TelepadNode *TelepadNodeList;
	struct DefensePlayerDataNode
	{
		int TelepadID;
		int DefGunID;
		DefensePlayerDataNode()
		{
			TelepadID = 0;
			DefGunID = 0;
		}
	};
public:
	DefensePlayerDataNode DefensePlayerDataNodes[128];
	DefenseModeSystem()
	{
		for (int x = 0;x < 128;x++)
			DefensePlayerDataNodes[x] = DefensePlayerDataNode();
		TelepadNodeList = NULL;
	}
	void AddTelepad(GameObject *TelePad,int PlayerID)
	{
		TelepadNode *Current = TelepadNodeList;
		if (!TelepadNodeList)
			TelepadNodeList = new TelepadNode(TelePad,PlayerID);
		while (Current)
		{
			if (Current->PlayerID == PlayerID)
			{
				GameObject *OldPad = Commands->Find_Object(Current->ID);
				if (OldPad)
					Commands->Destroy_Object(OldPad);
			}
			if (!Current->next)
			{
				Current->next = new TelepadNode(TelePad,PlayerID);
				break;
			}
			Current = Current->next;
		}
		GameObject *Telepad = Commands->Find_Object(TelepadNodeList->ID);
		if (Telepad)
			Commands->Set_Animation(Telepad,"DefModeTelepad.DefModeTelepad",false,0,1,1,false);
	}
	void RemoveTelepad(GameObject *telepad)
	{
		int ID = Commands->Get_ID(telepad);
		TelepadNode *Current = TelepadNodeList;
		while (Current)
		{
			if (Current->ID == ID)
			{
				GameObject *Zone = Commands->Find_Object(Current->ZoneID);
				if (Zone)
					Commands->Destroy_Object(Zone);
				Current->ID = 0; 
				Current->PlayerID = 0;
				Current->ZoneID = 0;
				break;
			}
			Current = Current->next;
		}
		bool activeTeleporter = false;
		Current = TelepadNodeList;
		while (Current)
		{
			if (Current->ID && TelepadNodeList->ID != Current->ID)
			{
				activeTeleporter = true;
				break;
			}
			Current = Current->next;
		}
		if (activeTeleporter)
			return;
		if (TelepadNodeList)
		{
			GameObject *Telepad = Commands->Find_Object(TelepadNodeList->ID);
			if (Telepad)
				Commands->Set_Animation(Telepad,"DefModeTelepad.DefModeTelepad",false,0,0,0,false);
		}
	}
	void RemoveAllButSpecial()
	{
		TelepadNode *Current = TelepadNodeList;
		if (Current)
			Current = Current->next;
		while (Current)
		{
			GameObject *Zone = Commands->Find_Object(Current->ZoneID);
			if (Zone)
				Commands->Destroy_Object(Zone);
			Current->ID = 0;
			Current = Current->next;
		}
		GameObject *Telepad = Commands->Find_Object(TelepadNodeList->ID);
		if (Telepad)
			Commands->Set_Animation(Telepad,"DefModeTelepad.DefModeTelepad",false,0,0,0,false);
	}
	TelepadNode *FindNextTelepad(int TelepadID)
	{
		if (!TelepadNodeList || !TelepadNodeList->next)
			return NULL;
		bool goodToGo = false;
		TelepadNode *Current = TelepadNodeList;
		while (Current)
		{
			if (goodToGo && Current->ID)
				return Current;
			if (Current->ID == TelepadID)
				goodToGo = true;
			Current = Current->next;
		}
		return TelepadNodeList->ID == TelepadID ? NULL : TelepadNodeList;
	}
	TelepadNode *FindPlayerTelepad(int TelepadPlayerID)
	{
		if (!TelepadNodeList || !TelepadNodeList->next)
			return NULL;
		TelepadNode *Current = TelepadNodeList;
		while (Current)
		{
			if (Current->PlayerID == TelepadPlayerID)
				return Current;
			Current = Current->next;
		}
		return NULL;
	}
	void EmptyList()
	{
		TelepadNode *Current = TelepadNodeList,*die = NULL;
		while (Current)
		{
			die = Current;
			Current = Current->next;
			delete die;
		}
		TelepadNodeList = NULL;
		for (int x = 0;x < 128;x++)
			DefensePlayerDataNodes[x] = DefensePlayerDataNode();
	}
};

DefenseModeSystem DefenseModeSystemControl = DefenseModeSystem();

class JMG_Metroid_Defense_Telepad_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Metroid_Preplaced_Player_Telepad : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Metroid_Player_Turret_Damage : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj, GameObject *damager);
};

class JMG_Metroid_Screen_Fade_Red_On_Damage : public ScriptImpClass {
	float InjuryRatio;
	float ScreenOpacicity;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void ScreenFade(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Metroid_Kill_On_Damage_Amounts : public ScriptImpClass {
	bool Enabled;
	float DamageAmounts[256];
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_Metroid_Grant_Keycard : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Metroid_AI_Move_To_Position : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Metroid_Reset_Screen_Color_On_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class MetroidEquipmentSystem
{
private:
	struct EquipmentNode
	{
		int ID;
		Vector3 Pos;
		struct EquipmentNode *next;
		EquipmentNode(GameObject *obj)
		{
			ID = Commands->Get_ID(obj);
			Pos = Commands->Get_Position(obj);
			next = NULL;
		}
	};
	EquipmentNode *EquipmentNodeList;
	void deleteObject(int ID)
	{
		if (!EquipmentNodeList)
			return;
		EquipmentNode *Current = EquipmentNodeList,*Prev = NULL;
		while (Current)
		{
			if (Current->ID == ID)
			{
				Current->ID = 0;
				break;
			}
			Prev = Current;
			Current = Current->next;
		}
	}
public:
	MetroidEquipmentSystem()
	{
		EquipmentNodeList = NULL;
	}
	MetroidEquipmentSystem &operator += (GameObject *obj)
	{
		int ID = Commands->Get_ID(obj);
		EquipmentNode *Current = EquipmentNodeList;
		if (!EquipmentNodeList)
			EquipmentNodeList = new EquipmentNode(obj);
		while (Current)
		{
			if (Current->ID == ID)
				return *this;
			if (!Current->next)
			{
				Current->next = new EquipmentNode(obj);
				break;
			}
			Current = Current->next;
		}
		return *this;
	};
	MetroidEquipmentSystem &operator -= (GameObject *obj)
	{
		int ID = Commands->Get_ID(obj);
		deleteObject(ID);
		return *this;
	};
	void EmptyList()
	{
		EquipmentNode *Current = EquipmentNodeList,*die = NULL;
		while (Current)
		{
			die = Current;
			Current = Current->next;
			delete die;
		}
		EquipmentNodeList = NULL;
	}
	float FindNearestEquipment(Vector3 *Pos,int *EquipmentID)
	{
		float Dist = -1;
		EquipmentNode *Current = EquipmentNodeList,*Nearest = NULL;
		while (Current)
		{
			if (!Current->ID)
			{
				Current = Current->next;
				continue;
			}
			float Temp = JmgUtility::SimpleDistance(*Pos,Current->Pos);
			if ((!Nearest || Temp < Dist) && Current->ID != *EquipmentID)
			{
				Dist = Temp;
				Nearest = Current;
			}
			Current = Current->next;
		}
		if (Nearest)
		{
			*Pos = Nearest->Pos;
			*EquipmentID = Nearest->ID;
		}
		else
			*EquipmentID = 0;
		return Dist;
	};
};

MetroidEquipmentSystem MetroidEquipmentSystemControl = MetroidEquipmentSystem();

class JMG_Metroid_Equipment_Tracking_Script : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
};

class JMG_Metroid_AI_Hunt_Equipment : public ScriptImpClass {
	Vector3 MoveToLocation;
	Vector3 LastMoveToLocation;
	Vector3 StartPosition;
	Vector3 MyLastPos;
	int TargetEquipmentID;
	float MaxRange;
	int EnemySeen;
	int EnemyID;
	int LastEnemyID;
	bool InnateEnabled;
	bool StarInRange;
	bool StandingStill;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void Action_Complete(GameObject *obj,int action,ActionCompleteReason reason);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Update(GameObject *obj);
	void GetClosestStar(GameObject *obj);
};

class JMG_Metroid_Base_Defense : public ScriptImpClass {
	int enemyID;
	float resetTime;
	float MinDist;
	float MaxDist;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_Metroid_Camera_Behavior : public ScriptImpClass {
	int EnemyID;
	int CameraFacingCount;
	int IncreaseOrDecreaseCount;
	int SeenTime;
	Vector3 CameraFacingLocation[5];
	float CameraFacingUpdateTime;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Metroid_Zone_Create_Explosion : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Metroid_Create_Object_Once_On_Enter : public ScriptImpClass {
	bool wasCreated;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Metroid_Remove_Object_On_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Metroid_AI_Snow_Mini_Boss : public ScriptImpClass {
	int LastSeen;
	int currentTargetID;
	int huntorattack;
	int waitcount;
	Vector3 homelocation;
	float speed;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_Metroid_Banshee_Strike_Craft : public ScriptImpClass {
	int lastAnimation;
	int BansheeID;
	void Created(GameObject *obj);
	void Animation_Complete(GameObject *obj,const char *anim);
	void Destroyed(GameObject *obj);
};

class JMG_Metroid_Cheat_Mode_Death_Counter : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_Metroid_ZeroBug_Tracker : public ScriptImpClass {
	int PlayerID;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Metroid_Zone_Damage_Object_While_In_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exiter);
};

class JMG_Metroid_Attached_Zone_Damage_Object_While_In_Zone : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class MetroidRandomSpawnSystem
{
public:
	struct MetroidRandomSpawnNode
	{
		int id;
		Vector3 pos;
		int groupId;
		struct MetroidRandomSpawnNode *next;
		MetroidRandomSpawnNode(GameObject *obj,int groupId)
		{
			this->id = Commands->Get_ID(obj);
			this->pos = Commands->Get_Position(obj);
			this->groupId = groupId;
			this->next = NULL;
			Commands->Destroy_Object(obj);
		}
	};
private:
	int totalPointCount;
	struct MetroidRandomSpawnNode *MetroidRandomSpawnNodeList;
public:
	MetroidRandomSpawnSystem()
	{
		totalPointCount = 0;
		MetroidRandomSpawnNodeList = NULL;
	}
	void AddNode(GameObject *obj,int groupId)
	{
		totalPointCount++;
		int id = Commands->Get_ID(obj);
		MetroidRandomSpawnNode *Current = MetroidRandomSpawnNodeList;
		if (!MetroidRandomSpawnNodeList)
			MetroidRandomSpawnNodeList = new MetroidRandomSpawnNode(obj,groupId);
		while (Current)
		{
			if (Current->id == id)
			{
				totalPointCount--;
				Console_Input("msg MetroidRandomSpawnSystem AddNode ERROR: ID already exists in list!");
				return;
			}
			if (!Current->next)
			{
				Current->next = new MetroidRandomSpawnNode(obj,groupId);
				return;
			}
			Current = Current->next;
		}
	}
	void EmptyList()
	{
		MetroidRandomSpawnNode *Current = MetroidRandomSpawnNodeList,*die = NULL;
		while (Current)
		{
			die = Current;
			Current = Current->next;
			delete die;
		}
		MetroidRandomSpawnNodeList = NULL;
	}
	bool SelectRandomSpawnPoint(Vector3 *pos,int groupId)
	{
		int Random = Commands->Get_Random_Int(0,totalPointCount*2);
		MetroidRandomSpawnNode *Current = MetroidRandomSpawnNodeList;
		while (Current)
		{
			if (Current->groupId == groupId)
			{
				if (Random)
					Random--;
				if (Random <= 0)
				{
					*pos = Current->pos;
					return true;
				}
			}
			Current = Current->next;
		}
		return false;
	}
	float GetNearestStar(const Vector3 &pos)
	{
		float minDist = 0;
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player)
				continue;
			float tempDist = JmgUtility::SimpleDistance(pos,Commands->Get_Position(player));
			if (!minDist || tempDist < minDist)
				minDist = tempDist;
		}
		return minDist;
	}
	Vector3 SelectFurthestSpawnPoint(const Vector3 &pos,int groupId)
	{
		Vector3 bestPos = pos;
		float maxDist = 0;
		MetroidRandomSpawnNode *Current = MetroidRandomSpawnNodeList;
		while (Current)
		{
			if (Current->groupId == groupId)
			{
				float tempDist = GetNearestStar(Current->pos);
				if (!maxDist || tempDist > maxDist)
				{
					maxDist = tempDist;
					bestPos = Current->pos;
				}
			}
			Current = Current->next;
		}
		return bestPos;
	}
};

MetroidRandomSpawnSystem MetroidRandomSpawnControl = MetroidRandomSpawnSystem();

class JMG_Metroid_Random_Spawn_Point_Node : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Metroid_Spawn_At_Random_Spawn_Point : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Metroid_Granted_Shield_Powerup : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class SupplyModeSystem
{
	int UpgradePowerupCount;
	struct UpgradePowerupNode
	{
		int ID;
		struct UpgradePowerupNode *next;
		UpgradePowerupNode(GameObject *obj)
		{
			ID = Commands->Get_ID(obj);
			next = NULL;
		}
	};
	UpgradePowerupNode *UpgradePowerupNodeList;
public:
	int WeapoonSpawnerID;
	SupplyModeSystem()
	{
		WeapoonSpawnerID = 0;
		UpgradePowerupCount = 0;
		UpgradePowerupNodeList = NULL;
	}
	int GrabFirstID()
	{
		if (!UpgradePowerupNodeList)
			return 0;
		UpgradePowerupNode *Current = UpgradePowerupNodeList;
		while (Current)
		{
			if (Current->ID)
				return Current->ID;
			Current = Current->next;
		}
		return 0;
	}
	void AddUpgradePowerup(GameObject *obj)
	{
		UpgradePowerupCount++;
		UpgradePowerupNode *Current = UpgradePowerupNodeList;
		if (!UpgradePowerupNodeList)
			UpgradePowerupNodeList = new UpgradePowerupNode(obj);
		while (Current)
		{
			if (!Current->next)
			{
				Current->next = new UpgradePowerupNode(obj);
				break;
			}
			Current = Current->next;
		}
		if (UpgradePowerupCount > 5)
		{
			GameObject *object = Commands->Find_Object(GrabFirstID());
			if (object)
				Commands->Destroy_Object(object);
		}
	}
	void RemoveUpgradePowerup(GameObject *obj)
	{
		int ID = Commands->Get_ID(obj);
		UpgradePowerupNode *Current = UpgradePowerupNodeList,*Prev = NULL;
		while (Current)
		{
			if (Current->ID == ID)
			{
				UpgradePowerupCount--;
				Current->ID = 0;
				return;
			}
			Prev = Current;
			Current = Current->next;
		}
	}
	void EmptyList()
	{
		UpgradePowerupNode *Current = UpgradePowerupNodeList,*die = NULL;
		while (Current)
		{
			die = Current;
			Current = Current->next;
			delete die;
		}
		UpgradePowerupCount = 0;
		UpgradePowerupNodeList = NULL;
	}
	void DestroyList()
	{
		UpgradePowerupNode *Current = UpgradePowerupNodeList;
		while (Current)
		{
			GameObject *upgrade = Commands->Find_Object(Current->ID);
			if (upgrade)
				Commands->Destroy_Object(upgrade);
			Current = Current->next;
		}
	}
};

SupplyModeSystem SupplyModeSystemControl[128] = {SupplyModeSystem()};

class JMG_Metroid_Supply_Mode : public ScriptImpClass {
	int BlockArmorUpgradeRegen;
	int BlockWeaponSpawnRegen;
	int LastArmorUpgradeAmmo;
	int LastWeaponSpawnAmmo;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
public:
	static int ShieldUpgradeGrantedBy[128];
	static void ClearUpgrades(int playerId,const char *playerName);
};
int JMG_Metroid_Supply_Mode::ShieldUpgradeGrantedBy[128] = {0};

class JMG_Metroid_Supply_Shield_Powerup_Beacon : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Metroid_Supply_Shield_Powerup : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
};

class JMG_Metroid_Supply_Weapon_Spawner_Beacon : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Metroid_Supply_Weapon_Spawner_Decoy : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Metroid_Supply_Weapon_Spawner_Weapon : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Metroid_Display_Message_On_Pickup : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Metroid_Tutorial_Teleport : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Metroid_Shield_Regen : public ScriptImpClass {
	int playerId;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Metroid_Transform_Into_Cinematic_Object : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Metroid_Grant_Phone_If_Pkg_Is_Rp2 : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Metroid_Boss_HP_Modifier : public ScriptImpClass {
	float maxShield;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Metroid_Set_Is_Rendered_On_Custom : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Metroid_AI_Shield_Regen : public ScriptImpClass {
	int lastDamaged;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_Metroid_Invincible_Object : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Metroid_Damage_Score_Monitor : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj, GameObject *damager);
};

class JMG_Metroid_Death_Zone_Score_Monitor : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Metroid_Weapon_Pickup_Score_Monitor : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Metroid_Count_Jammed_Door_Destruction : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *damager);
};

class JMG_Metroid_Zone_Score_Monitor : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Metroid_Lockdown_zComputer_Console_Special_Disarm_Script : public ScriptImpClass {
	float pokeAmount;
	int TerminalNumber;
	int subtractionGrowth;
	time_t lastPoke;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *killer);
	void Timeout(GameObject *obj);
};

class JMG_Metroid_Mine_Computer_Console_Script_Special : public ScriptImpClass {
	float pokeAmount;
	int TerminalNumber;
	int subtractionGrowth;
	time_t lastPoke;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
	void Timeout(GameObject *obj);
};

class JMG_Metroid_Poke_Activate_Purchase_Terminal : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
};

class JMG_Metroid_Update_And_Display_Briefing_On_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Metroid_Objective_Update_On_Enter : public ScriptImpClass {
	bool triggered;
	void Entered(GameObject *obj,GameObject *enter);
	void Timer_Expired(GameObject *obj,int number);
public:
	JMG_Metroid_Objective_Update_On_Enter()
	{
		triggered = false;
	}
};

class JMG_Metroid_Objective_Update_On_Pickup : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};


class JMG_Metroid_Objective_Update_On_Killed : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *damager);
};

class JMG_Metroid_Objective_Update_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_AI_Artillery_Targeting_Fire_At_Custom : public ScriptImpClass {
	double velocity;
	double gravity;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	bool CalculateAngle(double *returnAngle,double distance,double height,bool highAngle);
};

class JMG_Metroid_AI_Forest_Mini_Boss : public ScriptImpClass {
	bool primary;
	int LastSeen;
	int currentTargetID;
	int huntorattack;
	int waitcount;
	float speed;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Attack_Target(GameObject *obj,GameObject *target);
	void Hide(GameObject *obj);
};

class JMG_Metroid_SpawnRoom_Objective_Update_On_Enter : public ScriptImpClass {
	bool triggered;
	void Entered(GameObject *obj,GameObject *enter);
	void Timer_Expired(GameObject *obj,int number);
public:
	JMG_Metroid_SpawnRoom_Objective_Update_On_Enter()
	{
		triggered = false;
	}
};

class JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_At_Custom : public ScriptImpClass {
	bool reloadComplete;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	bool CalculateAngle(double *returnAngle,double distance,double height,bool highAngle);
	void FireProjectile(GameObject *obj,double zAngle,double aimAngle);
};


class JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_Attach : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
};

class JMG_Metroid_Miniboss_Turret : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
};

class JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_Custom_Player : public ScriptImpClass {
	bool reloadComplete;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	bool CalculateAngle(double *returnAngle,double distance,double height,bool highAngle);
	void FireProjectile(GameObject *obj,double zAngle,double aimAngle);
};