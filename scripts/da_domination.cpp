/*	Renegade Scripts.dll
    Dragonade Domination Game Mode
	Copyright 2017 Whitedragon, Tiberian Technologies

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
#include "engine.h"
#include "engine_DA.h"
#include "da.h"
#include "da_domination.h"
#include "da_levelcleaner.h"
#include "da_settings.h"
#include "da_string.h"
#include "GameObjManager.h"
#include "PurchaseSettingsDefClass.h"
#include "TeamPurchaseSettingsDefClass.h"
#include "cTeam.h"
#include "HashTemplateClass.h"
#include "HashTemplateIterator.h"

void DAControlNodeClass::Init(const INIClass *INI,const StringClass &Header) {
	Preset = DADominationManager->Get_Control_Node_Preset();
	Model = DADominationManager->Get_Control_Node_Model();
	Animation = DADominationManager->Get_Control_Node_Animation();
	DASpawnNodeClass::Init(INI,Header);
}

void DAControlNodeClass::Timer_Expired(int Number,unsigned int Data) {
	if (Number == 10) {
		DADominationManager->Increment_Score(Team);
	}
	else {
		DASpawnNodeClass::Timer_Expired(Number,Data);
	}
}

void DAControlNodeClass::Capture_Event() {
	DASpawnNodeClass::Capture_Event();
	Stop_Timer(10);
	Start_Timer(10,DADominationManager->Get_Point_Tick_Time(),true);
	DA::Create_2D_Sound_Team(Team,"M00EVAG_DSGN0048I1EVAG_snd.wav");
	DADominationManager->Update_Node_Count();
}

void DAControlNodeClass::Contested_Event() {
	if (Is_Contested()) {
		Stop_Timer(10);
	}
	else {
		Stop_Timer(10);
		Start_Timer(10,DADominationManager->Get_Point_Tick_Time(),true);
	}
	DASpawnNodeClass::Contested_Event();
}

void DATiberiumNodeClass::Init(const INIClass *INI,const StringClass &Header) {
	Preset = DADominationManager->Get_Tiberium_Node_Preset();
	Model = DADominationManager->Get_Tiberium_Node_Model();
	Animation = DADominationManager->Get_Tiberium_Node_Animation();
	DABaseNodeClass::Init(INI,Header);
}

void DATiberiumNodeClass::Timer_Expired(int Number,unsigned int Data) {
	if (Number == 10) {
		DADominationManager->Increment_Credits(Team);
	}
	else {
		Stop_Timer(10);
		DABaseNodeClass::Timer_Expired(Number,Data);
	}
}

void DATiberiumNodeClass::Capture_Event() {
	Stop_Timer(10);
	Start_Timer(10,DADominationManager->Get_Credit_Tick_Time(),true);
	DA::Create_2D_Sound_Team(Team,"M00EVAG_DSGN0049I1EVAG_snd.wav");
	DADominationManager->Update_Node_Count();
}

void DATiberiumNodeClass::Contested_Event() {
	if (Is_Contested()) {
		Stop_Timer(10);
	}
	else {
		Start_Timer(10,DADominationManager->Get_Credit_Tick_Time(),true);
	}
}

void DADominationManagerClass::Init() {
	Score[0] = 0.0f;
	Score[1] = 0.0f;
	
	const DASettingsClass *Settings = DASettingsManager::Get_Settings(DASettingsManager::Get_Settings_Count()-1); //Gamemode.ini will be last in settings chain at this point.
	Settings->Get_String(ControlNodePreset,"ControlNodePreset",0);
	Settings->Get_String(ControlNodeModel,"ControlNodeModel",0);
	Settings->Get_String(ControlNodeAnimation,"ControlNodeAnimation",0);

	Settings->Get_String(TiberiumNodePreset,"TiberiumNodePreset",0);
	Settings->Get_String(TiberiumNodeModel,"TiberiumNodeModel",0);
	Settings->Get_String(TiberiumNodeAnimation,"TiberiumNodeAnimation",0);

	PointTickAmount = Settings->Get_Float("DominationPointTickAmount",10.0f);
	PointTickTime = Settings->Get_Float("DominationPointTickTime",10.0f);
	CreditTickAmount = Settings->Get_Float("DominationCreditTickAmount",10.0f);
	CreditTickTime = Settings->Get_Float("DominationCreditTickTime",10.0f);

	WinningPoints = Settings->Get_Float("DominationWinningPoints",1000.0f);

	if ((The_Game()->MapName[0] == 'M' || The_Game()->MapName[0] == 'm')) {
		DALevelCleaner::Clean_Level();
		DALevelCleaner::Load_Blockers(Settings->Get_INI());
	}

	if (Settings->Get_Float(The_Game()->MapName,"GDI_Spawn1_X",0)) {
		DASpawnManager->Create_Instance()->Init(Settings->Get_INI());
		DASpawnManager->Create_Spawn_Point(Settings->Get_INI(),"Nod"); //Starting spawners
		DASpawnManager->Create_Spawn_Point(Settings->Get_INI(),"GDI");
	}
	if (Settings->Get_Float(The_Game()->MapName,"GDI_Vehicle_X",0)) {
		DAVehicleSpawnManager->Create_Instance()->Init(Settings->Get_INI());
		DAVehicleSpawnManager->Create_Air_Drop_Point(Settings->Get_INI(),"Nod"); //Vehicle spawn locations
		DAVehicleSpawnManager->Create_Air_Drop_Point(Settings->Get_INI(),"GDI");
	}
	if (Settings->Get_Float(The_Game()->MapName,"Teleporter1_TeleporterEntrance_X",0)) {
		DATeleporterManager->Create_Instance();
		for (int i = 1;;i++) {
			StringClass Header;
			Header.Format("Teleporter%d",i);
			StringClass X;
			Settings->Get_String(X,The_Game()->MapName,StringFormat("%s_TeleporterEntrance_X",Header),0);
			if (!X.Is_Empty()) {
				DATeleporterManager->Create_Teleporter(Settings->Get_INI(),Header);
			}
			else {
				break;
			}
		}
	}
	
	DANodeManager->Create_Instance()->Init(Settings->Get_INI());
	
	for (int i = 1;;i++) {
		StringClass Header;
		Header.Format("Control%d",i);
		StringClass X;
		Settings->Get_String(X,The_Game()->MapName,StringFormat("%s_Node_X",Header),0);
		if (!X.Is_Empty()) {
			DANodeManager->Create_Node<DAControlNodeClass>(Settings->Get_INI(),Header);
		}
		else {
			break;
		}
	}
	for (int i = 1;;i++) {
		StringClass Header;
		Header.Format("Tiberium%d",i);
		StringClass X;
		Settings->Get_String(X,The_Game()->MapName,StringFormat("%s_Node_X",Header),0);
		if (!X.Is_Empty()) {
			DANodeManager->Create_Node<DATiberiumNodeClass>(Settings->Get_INI(),Header);
		}
		else {
			break;
		}
	}
	
	//Change Stealth Tanks into Recon Bikes.
	PurchaseSettingsDefClass *PT = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_VEHICLES,PurchaseSettingsDefClass::TEAM_NOD);
	if (PT->Get_Definition(5) == Get_Definition_ID("CnC_Nod_Stealth_Tank")) {
		VehicleGameObjDef *Recon = (VehicleGameObjDef*)Find_Named_Definition("CnC_Nod_Recon_Bike");
		if (Recon) {
			PT->Set_Definition(5,Recon->Get_ID());
			PT->Set_Cost(5,500);
		}
	}

	//Change beacons into Remote C4.
	TeamPurchaseSettingsDefClass *PT2 = TeamPurchaseSettingsDefClass::Get_Definition(TeamPurchaseSettingsDefClass::TEAM_NOD);
	if (PT2->Get_Beacon_Definition() == Get_Definition_ID("CnC_POW_Nuclear_Missle_Beacon")) {
		PowerUpGameObjDef *RemotePowerUp = (PowerUpGameObjDef*)Find_Named_Definition("POW_MineRemote_Player");
		if (RemotePowerUp) {
			WeaponDefinitionClass *RemoteWeapon = (WeaponDefinitionClass*)Find_Definition(RemotePowerUp->GrantWeaponID);
			if (RemoteWeapon) {
				RemoteWeapon->MaxInventoryRounds = 1;
				RemoteWeapon->CanReceiveGenericCnCAmmo = false;

				PT2->Set_Beacon_Definition(RemotePowerUp->Get_ID());
				PT2->Set_Beacon_Cost(200);

				PT2 = TeamPurchaseSettingsDefClass::Get_Definition(TeamPurchaseSettingsDefClass::TEAM_GDI);
				PT2->Set_Beacon_Definition(RemotePowerUp->Get_ID());
				PT2->Set_Beacon_Cost(200);
			}
		}
	}

	//Change Stealth Black Hand into Black Hand Rocket Soldier.
	PT = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_CLASSES,PurchaseSettingsDefClass::TEAM_NOD);
	SoldierGameObjDef *SBH = (SoldierGameObjDef*)Find_Named_Definition("CnC_Nod_FlameThrower_2SF");
	if (SBH && PT->Get_Definition(5) == (int)SBH->Get_ID()) {
		SoldierGameObjDef *BHRS = (SoldierGameObjDef*)Find_Named_Definition("CnC_Nod_MiniGunner_2SF_Skirmish");
		if (BHRS) {
			BHRS->WeaponDefID = Get_Definition_ID("CnC_Weapon_RocketLauncher_Player");
			BHRS->WeaponRounds = 42;
			BHRS->ScriptNameList = SBH->ScriptNameList;
			BHRS->ScriptParameterList = SBH->ScriptParameterList;
			PT->Set_Definition(5,BHRS->Get_ID());
		}
	}

	//Change 4 shot Ramjets into 1 shot.
	DefinitionClass *OldWeaponDef = Find_Named_Definition("CnC_Weapon_RamjetRifle_Player");
	DefinitionClass *NewWeaponDef = Find_Named_Definition("Weapon_RamjetRifle_Player");
	if (OldWeaponDef && OldWeaponDef->Get_Class_ID() == CID_Weapon && NewWeaponDef && NewWeaponDef->Get_Class_ID() == CID_Weapon) {
		for (PowerUpGameObjDef *PowerUpDef = (PowerUpGameObjDef*)DefinitionMgrClass::Get_First(CID_PowerUp);PowerUpDef;PowerUpDef = (PowerUpGameObjDef*)DefinitionMgrClass::Get_Next(PowerUpDef,CID_PowerUp)) {
			if ((unsigned int)PowerUpDef->GrantWeaponID == OldWeaponDef->Get_ID()) {
				PowerUpDef->GrantWeaponID = NewWeaponDef->Get_ID();
			}
		}
		for (SoldierGameObjDef *SoldierDef = (SoldierGameObjDef*)DefinitionMgrClass::Get_First(CID_Soldier);SoldierDef;SoldierDef = (SoldierGameObjDef*)DefinitionMgrClass::Get_Next(SoldierDef,CID_Soldier)) {
			if ((unsigned int)SoldierDef->WeaponDefID == OldWeaponDef->Get_ID()) {
				SoldierDef->WeaponDefID = NewWeaponDef->Get_ID();
			}
			if ((unsigned int)SoldierDef->SecondaryWeaponDefID == OldWeaponDef->Get_ID()) {
				SoldierDef->SecondaryWeaponDefID = NewWeaponDef->Get_ID();
			}
		}
	}
	
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Event(DAEvent::PLAYERLOADED);
	Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::PLAYER);
	
	Start_Timer(10,CreditTickTime,true);

	Register_Chat_Command((DAECC)&DADominationManagerClass::Nodes_Chat_Command,"!nodes|!node|!score|!points|!captures|!caps|!cap|!flag|!flags");
}

void DADominationManagerClass::Settings_Loaded_Event() {
	DAPlayerManager::Set_Disable_Team_Kill_Counter(true);
	DAPlayerManager::Set_Disable_Team_Death_Counter(true);
	DAPlayerManager::Set_Disable_Team_Score_Counter(true);
}

void DADominationManagerClass::Player_Loaded_Event(cPlayer *Player) {
	Send_Info_Message(Player->Get_Id());
}

void DADominationManagerClass::Object_Created_Event(GameObject *obj) {
	((SoldierGameObj*)obj)->Give_Key(1);
	((SoldierGameObj*)obj)->Give_Key(2);
	((SoldierGameObj*)obj)->Give_Key(3);
}

void DADominationManagerClass::Timer_Expired(int Number,unsigned int Data) {
	if (Number == 10) {
		Increment_Credits(0);
		Increment_Credits(1);
	}
	else if (Number == 11) {
		Console_Input("timeout");
	}
}

void DADominationManagerClass::Set_Winner(int Winner) {
	if (Winner == 0) {
		DA::Create_2D_Sound_Team(0,"M00EVAN_DSGN0077I1EVAN_snd.wav");
		DA::Create_2D_Sound_Team(1,"M00EVAG_DSGN0011I1EVAG_snd.wav");
	}
	else if (Winner == 1) {
		DA::Create_2D_Sound_Team(0,"M00EVAN_DSGN0078I1EVAN_snd.wav");
		DA::Create_2D_Sound_Team(1,"M00EVAG_DSGN0005I1EVAG_snd.wav");
	}
	DA::Host_Message("%ls has won the game!",Get_Wide_Team_Name(Winner));

	int ControlNodeCount[2];
	ControlNodeCount[0] = 0;
	ControlNodeCount[1] = 0;

	int TiberiumNodeCount[2];
	TiberiumNodeCount[0] = 0;
	TiberiumNodeCount[1] = 0;

	for (int i = 0;i < DANodeManager->Get_Node_Count();i++) {
		DABaseNodeClass *Node = DANodeManager->Get_Node(i);
		Node->Unregister_Event(DAEvent::THINK);
		Node->Stop_Timer(10);
		if (!_stricmp(Node->Get_Type(),ControlNodeType)) {
			if (Node->Get_Team() == 0) {
				ControlNodeCount[0]++;
			}
			else if (Node->Get_Team() == 1) {
				ControlNodeCount[1]++;
			}
		}
		else if (!_stricmp(Node->Get_Type(),TiberiumNodeType)) {
			if (Node->Get_Team() == 0) {
				TiberiumNodeCount[0]++;
			}
			else if (Node->Get_Team() == 1) {
				TiberiumNodeCount[1]++;
			}
		}
	}

	DA::Host_Message("%ls controlled %d Tiberium Node(s), %d Control Node(s), and had a score of %.0f.",Get_Wide_Team_Name(0),TiberiumNodeCount[0],ControlNodeCount[0],Score[0]);
	DA::Host_Message("%ls controlled %d Tiberium Node(s), %d Control Node(s), and had a score of %.0f.",Get_Wide_Team_Name(1),TiberiumNodeCount[1],ControlNodeCount[1],Score[1]);

	Stop_Timer(10);
	Start_Timer(11,5.0f,false,Winner?0:1);
}

void DADominationManagerClass::Update_Node_Count() {
	int ControlNodeCount[2];
	ControlNodeCount[0] = 0;
	ControlNodeCount[1] = 0;

	int TiberiumNodeCount[2];
	TiberiumNodeCount[0] = 0;
	TiberiumNodeCount[1] = 0;

	for (int i = 0;i < DANodeManager->Get_Node_Count();i++) {
		DABaseNodeClass *Node = DANodeManager->Get_Node(i);
		if (!_stricmp(Node->Get_Type(),ControlNodeType)) {
			if (Node->Get_Team() == 0) {
				ControlNodeCount[0]++;
			}
			else if (Node->Get_Team() == 1) {
				ControlNodeCount[1]++;
			}
		}
		else if (!_stricmp(Node->Get_Type(),TiberiumNodeType)) {
			if (Node->Get_Team() == 0) {
				TiberiumNodeCount[0]++;
			}
			else if (Node->Get_Team() == 1) {
				TiberiumNodeCount[1]++;
			}
		}
	}
	cTeam *Nod = Find_Team(0);
	cTeam *GDI = Find_Team(1);
	Nod->Set_Kills(ControlNodeCount[0]);
	Nod->Set_Deaths(TiberiumNodeCount[0]);
	GDI->Set_Kills(ControlNodeCount[1]);
	GDI->Set_Deaths(TiberiumNodeCount[1]);
}

void DADominationManagerClass::Send_Info_Message(int ID) {
	if (DASpawnManager && DASpawnManager->Using_Waiting_Room()) {
		DA::Private_Admin_Message(ID,"When you die you will be asked to use the Purchase Terminal to select a character to spawn as. Once selected you will spawn at either the closest node controlled by your team or your team's starting location.");
	}
	DA::Private_Color_Message(ID,COLORWHITE,"This is a Domination Mode match. The objective is to capture and hold Tiberium Nodes for money and Control Nodes for points. The first team to reach %.0f points wins.",WinningPoints);
	DA::Private_Admin_Message(ID,"This is a Domination Mode match. The objective is to capture and hold Tiberium Nodes for money and Control Nodes for points. The first team to reach %.0f points wins.",WinningPoints);
	if (DASpawnManager && DASpawnManager->Using_Waiting_Room()) {
		DA::Private_Color_Message(ID,COLORWHITE,"When you die you will be asked to use the Purchase Terminal to select a character to spawn as. Once selected you will spawn at either the closest node controlled by your team or your team's starting location.");
	}
}

bool DADominationManagerClass::Nodes_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	HashTemplateClass<StringClass,PairClass<int,StringClass>> NodeMap[3];
	for (int i = 0;i < DANodeManager->Get_Node_Count();i++) {
		DABaseNodeClass *Node = DANodeManager->Get_Node(i);
		if (!Node->Get_Name().Is_Empty()) {
			int Team = Node->Get_Team();
			PairClass<int,StringClass> *Pair = NodeMap[Team].Get(Node->Get_Type());
			if (Pair) {
				Pair->First++;
				Pair->Second += StringFormat(", %s",Node->Get_Name());
			}
			else {
				NodeMap[Team].Insert(Node->Get_Type(),PairClass<int,StringClass>(1,Node->Get_Name()));
			}
		}
	}
	int ID = Player->Get_Id();
	for (HashTemplateIterator<StringClass,PairClass<int,StringClass>> it = NodeMap[2];it;++it) {
		DA::Private_Color_Message(ID,COLORWHITE,"%s(%d): %s",it.getKey(),it.getValue().First,it.getValue().Second);
	}
	for (HashTemplateIterator<StringClass,PairClass<int,StringClass>> it = NodeMap[0];it;++it) {
		DA::Private_Color_Message_With_Team_Color(ID,0,"%s(%d): %s",it.getKey(),it.getValue().First,it.getValue().Second);
	}
	DA::Private_Color_Message_With_Team_Color(ID,0,"Score: %.0f/%.0f",Score[0],WinningPoints);
	for (HashTemplateIterator<StringClass,PairClass<int,StringClass>> it = NodeMap[1];it;++it) {
		DA::Private_Color_Message_With_Team_Color(ID,1,"%s(%d): %s",it.getKey(),it.getValue().First,it.getValue().Second);
	}
	DA::Private_Color_Message_With_Team_Color(ID,1,"Score: %.0f/%.0f",Score[1],WinningPoints);
	return true;
}

Register_Game_Mode(DADominationManagerClass,"Domination","Domination","Control1_Node_X");
