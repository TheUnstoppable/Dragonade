/*	Renegade Scripts.dll
    Dragonade Spawn System Game Mode Framework
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
#include "da_spawnsystem.h"
#include "da_gameobjobservers.h"
#include "MoveablePhysClass.h"
#include "GameObjManager.h"
#include "BuildingGameObj.h"
#include "DefinitionMgrClass.h"
#include "TeamPurchaseSettingsDefClass.h"

DA_API DASpawnManagerClass *DASingleton<DASpawnManagerClass>::Instance = 0;


unsigned int DASpawnPlayerDataClass::Check_Spawn_Time(unsigned int SpawnTime) const {
	if (DASpawnManager->Using_Waiting_Room()) {
		if (The_Game()->GameDuration_Seconds >= DASpawnManager->Get_Intermission_Time()) {
			if ((GetTickCount()-DeathTime) >= SpawnTime) {
				return 0;
			}
			return (SpawnTime-(GetTickCount()-DeathTime));
		}
		return (DASpawnManager->Get_Intermission_Time()-The_Game()->GameDuration_Seconds)*1000;
	}
	return 0;
}

void DASpawnPointClass::Init(const INIClass *INI,const StringClass &Header) {
	Group = Header;
	
	INI->Get_String(Name,The_Game()->MapName,StringFormat("%s_Name",Header));
	SpawnTime = (unsigned int)(INI->Get_Float(The_Game()->MapName,StringFormat("%s_SpawnTime",Header),(float)(DASpawnManager->Get_Spawn_Time())/1000)*1000);
	BaseSpawnTime = SpawnTime;
	Enable = INI->Get_Bool(The_Game()->MapName,StringFormat("%s_SpawnStartEnabled",Header),true);
	Team = INI->Get_Int(The_Game()->MapName,StringFormat("%s_Team",Header),2);
	if (Team != 0 && Team != 1) {
		Team = 2;
	}
	if (Team == 2) {
		Set_Enabled(false);
	}
	ProtectTime = INI->Get_Float(The_Game()->MapName,StringFormat("%s_SpawnProtectionTime",Header),DASpawnManager->Get_Protect_Time());
	Priority = INI->Get_Int(The_Game()->MapName,StringFormat("%s_SpawnPriority",Header),1);
	Facing = INI->Get_Float(The_Game()->MapName,StringFormat("%s_SpawnFacing",Header),0.0f);

	SoldierGameObj *Test = Commands->Create_Object(Get_Definition_Name(TeamPurchaseSettingsDefClass::Get_Definition(TeamPurchaseSettingsDefClass::TEAM_GDI)->Get_Enlisted_Definition(0)),Vector3(0,0,0))->As_SoldierGameObj();
	Test->Clear_Object_Dirty_Bits();
	MoveablePhysClass *Phys = Test->Peek_Physical_Object()->As_MoveablePhysClass();
	Matrix3D Transform = Phys->Get_Transform();
	for (int i = 1;;i++) {
		Vector3 SpawnPos;
		SpawnPos.X = INI->Get_Float(The_Game()->MapName,StringFormat("%s_Spawn%d_X",Header,i),0.0f);
		SpawnPos.Y = INI->Get_Float(The_Game()->MapName,StringFormat("%s_Spawn%d_Y",Header,i),0.0f);
		SpawnPos.Z = INI->Get_Float(The_Game()->MapName,StringFormat("%s_Spawn%d_Z",Header,i),0.0f);
		if (SpawnPos.X || SpawnPos.Y || SpawnPos.Z) {
			Transform.Set_Translation(SpawnPos);
			if (Phys->Can_Teleport(Transform)) {
				Spawners.Add(SpawnPos);
			}
			else {
				Console_Output(StringFormat("Spawn Point Error: %s_Spawn%d collides with terrain.\n",Header,i));
			}
		}
		else {
			break;
		}
	}
	Test->Set_Delete_Pending();
	Test->Clear_Object_Dirty_Bits();
	Set_Center_Point(Spawners[0]);
	Register_Event(DAEvent::THINK);
}

void DASpawnPointClass::Think() {
	for (int i = 0;i < SpawnList.Count();i++) {
		DASpawnPlayerDataClass *Player = DASpawnManager->Get_Player_Data(SpawnList[i]);
		if (!Player->Check_Spawn_Time(SpawnTime)) {
			if (Spawn_Player(Get_GameObj(SpawnList[i]))) {
				Player->SpawnState = DASpawnState::SPAWNED;
			}
			else {
				DA::Private_Color_Message(SpawnList[i],COLORWHITE,"There are currently no available spawn points for your team.");
				Player->SpawnState = DASpawnState::WAITINGROOM;
			}
			SpawnList.Delete(i);
			i--;
		}
	}
}

void DASpawnPointClass::Add_Spawn_List(int ID) {
	Add_Spawn_List(Get_GameObj(ID));
}

void DASpawnPointClass::Add_Spawn_List(GameObject *obj) {
	int ID = Get_Player_ID(obj);
	DASpawnPlayerDataClass *Player = DASpawnManager->Get_Player_Data(ID);
	if (SpawnList.ID(ID) == -1) {
		unsigned int Time = Player->Check_Spawn_Time(SpawnTime);
		if (!Time) {
			if (Spawn_Player(obj)) {
				Player->SpawnState = DASpawnState::SPAWNED;
			}
			else {
				DA::Private_Color_Message(ID,COLORWHITE,"There are currently no available spawn points for your team.");
				Player->SpawnState = DASpawnState::WAITINGROOM;
			}
		}
		else {
			SpawnList.Add(ID);
			Player->SpawnState = DASpawnState::SPAWNING;
			if (!Get_Name().Is_Empty()) {
				DA::Private_Color_Message(ID,COLORWHITE,"You will spawn at the %s spawn point in %.1f second(s).",Get_Name(),(float)Time/1000);
			}
			else {
				DA::Private_Color_Message(ID,COLORWHITE,"You will spawn in %.1f second(s).",(float)Time/1000);
			}
		}
	}
}

void DASpawnPointClass::Check_Spawn_List() {
	for (int i = 0;i < SpawnList.Count();i++) {
		DASpawnPlayerDataClass *Player = DASpawnManager->Get_Player_Data(SpawnList[i]);
		unsigned int Time = Player->Check_Spawn_Time(SpawnTime);
		if (!Time) {
			if (Spawn_Player(Get_GameObj(SpawnList[i]))) {
				Player->SpawnState = DASpawnState::SPAWNED;
			}
			else {
				DA::Private_Color_Message(SpawnList[i],COLORWHITE,"There are currently no available spawn points for your team.");
				Player->SpawnState = DASpawnState::WAITINGROOM;
			}
			SpawnList.Delete(i);
			i--;
		}
		else if (!Get_Name().Is_Empty()) {
			DA::Private_Color_Message(SpawnList[i],COLORWHITE,"You will spawn at the %s spawn point in %.1f second(s).",Get_Name(),(float)Time/1000);
		}
		else {
			DA::Private_Color_Message(SpawnList[i],COLORWHITE,"You will spawn in %.1f second(s).",(float)Time/1000);
		}
	}
}

void DASpawnPointClass::Clear_Spawn_List() {
	for (int i = 0;i < SpawnList.Count();i++) {
		DASpawnManager->Get_Player_Data(SpawnList[i])->SpawnState = DASpawnState::WAITINGROOM;
	}
	SpawnList.Delete_All();
}

void DASpawnPointClass::Clear_Spawn_List_Find_New_Spawn() {
	for (int i = 0;i < SpawnList.Count();i++) {
		DASpawnManager->Add_Spawn_List(SpawnList[i]);
	}
	SpawnList.Delete_All();
}

float DASpawnPointClass::Get_Distance(const Vector3 &Position) const {
	float Return = 9999.0f;
	for (int j = 0;j < Spawners.Count();j++) {
		float Dis = Commands->Get_Distance(Position,Spawners[j]);
		//Dis += Diff(Spawners[j].Z,Position.Z)*4.0f;
		if (Dis < Return) {
			Return = Dis;
		}
	}
	float Dis = Commands->Get_Distance(Position,CenterPoint);
	//Dis += Diff(CenterPoint.Z,Position.Z)*4.0f;
	if (Dis < Return) {
		Return = Dis;
	}
	return Return;
}

#define Do_Spawn() { Phys->Set_Transform(SpawnPos); Commands->Create_Object("Spawner Created Special Effect",SpawnPos.Get_Translation()); obj->Add_Observer(new DATimedInvincibilityObserverClass(ProtectTime)); }

bool DASpawnPointClass::Spawn_Player(GameObject *obj) const {
	MoveablePhysClass *Phys = obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass();
	int Rand = Commands->Get_Random_Int(0,Spawners.Count());
	Matrix3D SpawnPos = Phys->Get_Transform();
	SpawnPos.Set_Translation(Spawners[Rand]);
	DistanceCheckStruct DistanceCheck;
	if (Phys->Can_Teleport(SpawnPos)) {
		Distance_Check(DistanceCheck,SpawnPos.Get_Translation(),Get_Object_Type(obj));
		if (DASpawnManager->Get_Min_Ally_Distance() <= DistanceCheck.Ally && DistanceCheck.Ally <= DASpawnManager->Get_Max_Ally_Distance() && DASpawnManager->Get_Min_Enemy_Distance() <= DistanceCheck.Enemy && DistanceCheck.Enemy <= DASpawnManager->Get_Max_Enemy_Distance()) {
			Do_Spawn();
			return true;
		}
	}
	for (int i = 0;i < Spawners.Count();i++) {
		SpawnPos.Set_Translation(Spawners[i]);
		if (Phys->Can_Teleport(SpawnPos)) {
			Distance_Check(DistanceCheck,SpawnPos.Get_Translation(),Get_Object_Type(obj));
			if (DASpawnManager->Get_Min_Ally_Distance() <= DistanceCheck.Ally && DistanceCheck.Ally <= DASpawnManager->Get_Max_Ally_Distance() && DASpawnManager->Get_Min_Enemy_Distance() <= DistanceCheck.Enemy && DistanceCheck.Enemy <= DASpawnManager->Get_Max_Enemy_Distance()) {			
				Do_Spawn();
				return true;
			}
		}
	}
	for (int i = 0;i < Spawners.Count();i++) {
		SpawnPos.Set_Translation(Spawners[i]);
		if (Phys->Can_Teleport(SpawnPos)) {
			Distance_Check(DistanceCheck,SpawnPos.Get_Translation(),Get_Object_Type(obj));
			if (DASpawnManager->Get_Min_Enemy_Distance() <= DistanceCheck.Enemy && DistanceCheck.Enemy <= DASpawnManager->Get_Max_Enemy_Distance()) {			
				Do_Spawn();
				return true;
			}
		}
	}
	for (int i = 0;i < Spawners.Count();i++) {
		SpawnPos.Set_Translation(Spawners[i]);
		if (Phys->Can_Teleport(SpawnPos)) {
			Do_Spawn();
			return true;
		}
	}
	return false;
}

void DASpawnPointClass::Distance_Check(DistanceCheckStruct &Buffer,const Vector3 &Pos,int Team) const {
	Buffer.Ally = FLT_MAX;
	Buffer.Both = FLT_MAX;
	Buffer.Enemy = FLT_MAX;
	for (SLNode<SoldierGameObj> *z = GameObjManager::SoldierGameObjList.Head();z;z = z->Next()) {
		SoldierGameObj *Soldier = z->Data();
		float Dis = Commands->Get_Distance(Commands->Get_Position(Soldier),Pos);
		if (Dis < Buffer.Both) {
			Buffer.Both = Dis;
		}
		if (Soldier->Get_Player_Type() == Team) {
			if (Dis < Buffer.Ally) {
				Buffer.Ally = Dis;
			}
		}
		else if (Dis < Buffer.Enemy) {
			Buffer.Enemy = Dis;
		}
	}
}

void DASpawnManagerClass::Init(const INIClass *INI) {
	DAPlayerDataManagerClass<DASpawnPlayerDataClass>::Init();

	SpawnTime = (unsigned int)(INI->Get_Float(The_Game()->MapName,"SpawnTime",INI->Get_Float("General","SpawnTime",0.0f))*1000);
	IntermissionTime = (unsigned int)INI->Get_Int(The_Game()->MapName,"SpawnIntermissionTime",INI->Get_Int("General","SpawnIntermissionTime",10));
	MinAllyDistance = INI->Get_Float(The_Game()->MapName,"SpawnMinAllyDistance",INI->Get_Float("General","SpawnMinAllyDistance",0.0f));
	MaxAllyDistance = INI->Get_Float(The_Game()->MapName,"SpawnMaxAllyDistance",INI->Get_Float("General","SpawnMaxAllyDistance",0.0f));
	if (!MaxAllyDistance) {
		MaxAllyDistance = FLT_MAX;
	}
	MinEnemyDistance = INI->Get_Float(The_Game()->MapName,"SpawnMinEnemyDistance",INI->Get_Float("General","SpawnMinEnemyDistance",0.0f));
	MaxEnemyDistance = INI->Get_Float(The_Game()->MapName,"SpawnMaxEnemyDistance",INI->Get_Float("General","SpawnMaxEnemyDistance",0.0f));
	if (!MaxEnemyDistance) {
		MaxEnemyDistance = FLT_MAX;
	}
	ProtectTime = INI->Get_Float(The_Game()->MapName,"SpawnProtectionTime",INI->Get_Float("General","SpawnProtectionTime",2.0f));

	WaitingRoomPosition[0].X = INI->Get_Float(The_Game()->MapName,"Nod_WaitingRoom_X",0.0f);
	WaitingRoomPosition[0].Y = INI->Get_Float(The_Game()->MapName,"Nod_WaitingRoom_Y",0.0f);
	WaitingRoomPosition[0].Z = INI->Get_Float(The_Game()->MapName,"Nod_WaitingRoom_Z",0.0f);

	WaitingRoomPosition[1].X = INI->Get_Float(The_Game()->MapName,"GDI_WaitingRoom_X",0.0f);
	WaitingRoomPosition[1].Y = INI->Get_Float(The_Game()->MapName,"GDI_WaitingRoom_Y",0.0f);
	WaitingRoomPosition[1].Z = INI->Get_Float(The_Game()->MapName,"GDI_WaitingRoom_Z",0.0f);

	if (WaitingRoomPosition[0].X || WaitingRoomPosition[0].Y || WaitingRoomPosition[0].Z || WaitingRoomPosition[1].X || WaitingRoomPosition[1].Y || WaitingRoomPosition[1].Z) {
		UseWaitingRoom = true;
		GameObject *PCT = Commands->Create_Object("pct_zone_nod",WaitingRoomPosition[0]);
		Commands->Set_Facing(PCT,0.0f);
		PCT = Commands->Create_Object("pct_zone_nod",WaitingRoomPosition[0]);
		Commands->Set_Facing(PCT,90.0f);

		PCT = Commands->Create_Object("pct_zone_gdi",WaitingRoomPosition[1]);
		Commands->Set_Facing(PCT,0.0f);
		PCT = Commands->Create_Object("pct_zone_gdi",WaitingRoomPosition[1]);
		Commands->Set_Facing(PCT,90.0f);

	
		Register_Event(DAEvent::SUICIDE,1000);
		Register_Event(DAEvent::CHATCOMMAND,1000);
		Register_Event(DAEvent::VEHICLEPURCHASE);
		Register_Event(DAEvent::POWERUPPURCHASE);
		
		if (!BaseControllerClass::Find_Base(0)->Find_Building(BuildingConstants::TYPE_SOLDIER_FACTORY)) {
			DefinitionClass *BuildingDef = DefinitionMgrClass::Find_Typed_Definition("mp_Hand_of_Nod",CID_SoldierFactory,false);
			BuildingGameObj *Building = (BuildingGameObj*)BuildingDef->Create();
			Building->Set_Position(Vector3(0,0,0));
			Building->Collect_Building_Components();
			Building->Start_Observers();
		}
		
		if (!BaseControllerClass::Find_Base(1)->Find_Building(BuildingConstants::TYPE_SOLDIER_FACTORY)) {
			DefinitionClass *BuildingDef = DefinitionMgrClass::Find_Typed_Definition("mp_GDI_Barracks",CID_SoldierFactory,false);
			BuildingGameObj *Building = (BuildingGameObj*)BuildingDef->Create();;
			Building->Set_Position(Vector3(0,0,0));
			Building->Collect_Building_Components();
			Building->Start_Observers();
		}
	}
	else {
		UseWaitingRoom = false;
	}
	Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::PLAYER);
	Register_Object_Event(DAObjectEvent::DESTROYED,DAObjectEvent::PLAYER);
}

void DASpawnManagerClass::Vehicle_Purchase_Event(cPlayer *Player,float Cost,const VehicleGameObjDef *Item) { 
	if (Get_Player_Data(Player)->SpawnState == DASpawnState::WAITINGROOM) {
		Display_PT(Player->Get_GameObj());
	}
}

void DASpawnManagerClass::PowerUp_Purchase_Event(cPlayer *Player,float Cost,const PowerUpGameObjDef *Item) {
	if (Get_Player_Data(Player)->SpawnState == DASpawnState::WAITINGROOM) {
		Display_PT(Player->Get_GameObj());
	}
}

//Have to really drive this home.
bool DASpawnManagerClass::Suicide_Event(cPlayer *Player) {
	DASpawnState::State SpawnState = Get_Player_Data(Player)->SpawnState;
	if (SpawnState == DASpawnState::WAITINGROOM || SpawnState == DASpawnState::SPAWNING) {
		DA::Private_Color_Message(Player,COLORWHITE,"Use the Purchase Terminal to select your spawn character.");
		DA::Private_Admin_Message(Player,"Use the Purchase Terminal to select your spawn character.");
		Display_PT(Player->Get_GameObj());
		return false;
	}
	return true;
}

bool DASpawnManagerClass::Chat_Command_Event(cPlayer *Player,TextMessageEnum Type,const StringClass &Command,const DATokenClass &Text,int ReceiverID) {
	if (Command == "!killme" || Command == "!unstuck" || Command == "!unstick" || Command == "!stuck" || Command == "!stick") {
		DASpawnState::State SpawnState = Get_Player_Data(Player)->SpawnState;
		if (SpawnState == DASpawnState::WAITINGROOM || SpawnState == DASpawnState::SPAWNING) {
			DA::Private_Color_Message(Player,COLORWHITE,"Use the Purchase Terminal to select your spawn character.");
			DA::Private_Admin_Message(Player,"Use the Purchase Terminal to select your spawn character.");
			Display_PT(Player->Get_GameObj());
			return false;
		}
	}
	return true;
}

void DASpawnManagerClass::Timer_Expired(int Number,unsigned int Data) {
	Display_PT(Commands->Find_Object(Data));
}

void DASpawnManagerClass::Object_Created_Event(GameObject *obj) {
	DASpawnPlayerDataClass *Player = Get_Player_Data(obj);
	if (Using_Waiting_Room()) {
		if (Player->SpawnState == DASpawnState::DEAD) {
			Send_To_Waiting_Room(obj);
			Start_Timer(1,0.5f,false,Commands->Get_ID(obj)); //Displaying the PT here will crash players if the PT packet arrives before their new gameobject.
			Player->SpawnState = DASpawnState::WAITINGROOM;
		}
		else if (Player->SpawnState == DASpawnState::WAITINGROOM) {
			Add_Spawn_List(obj);
		}
	}
	else if (Player->SpawnState == DASpawnState::DEAD) {
		Add_Spawn_List(obj);
	}
}

void DASpawnManagerClass::Object_Destroyed_Event(GameObject *obj) {
	DASpawnPlayerDataClass *Player = Get_Player_Data(obj);
	if (!Player->Get_Owner()->Get_DA_Player()->Is_Spawning()) {
		Remove_Spawn_List(obj);
		Player->SpawnState = DASpawnState::DEAD;
		Player->DeathTime = GetTickCount();
		Player->DeathPosition = Commands->Get_Position(obj);
	}
}

void DASpawnManagerClass::Send_To_Waiting_Room(GameObject *obj) {
	int Team = Get_Object_Type(obj);
	if (Team == 0) {
		Commands->Set_Position(obj,WaitingRoomPosition[0]);
	}
	else if (Team == 1) {
		Commands->Set_Position(obj,WaitingRoomPosition[1]);
	}
	DA::Private_Color_Message(obj,COLORWHITE,"Use the Purchase Terminal to select your spawn character.");
}

void DASpawnManagerClass::Display_PT(GameObject *obj) {
	int Team = Get_Object_Type(obj);
	if (Team == 0) {
		Display_NOD_Player_Terminal_Player(obj);
	}
	else if (Team == 1) {
		Display_GDI_Player_Terminal_Player(obj);
	}
}

void DASpawnManagerClass::Add_Spawn_List(GameObject *obj) {
	int ID = Get_Player_ID(obj);
	DASpawnPlayerDataClass *Player = Get_Player_Data(ID);
	int Team = Get_Object_Type(obj);
	Vector3 Position = Player->DeathPosition;

	DASpawnPointClass *SpawnPoint = 0;
	
	if (!Player->NextSpawnLocation.Is_Empty()) {
		for (int i = 0;i < SpawnPoints.Count();i++) {
			if (SpawnPoints[i]->Is_Enabled() && SpawnPoints[i]->Get_Team() == Team && Player->NextSpawnLocation == SpawnPoints[i]->Get_Group()) {
				SpawnPoint = SpawnPoints[i];
				break;
			}
		}
	}
	if (!SpawnPoint) {
		float ClosestDis = FLT_MAX;
		int HighPriority = INT_MIN;
		for (int i = 0;i < SpawnPoints.Count();i++) {
			if (SpawnPoints[i]->Is_Enabled() && SpawnPoints[i]->Get_Team() == Team) {
				if (SpawnPoints[i]->Get_Priority() > HighPriority) {
					ClosestDis = SpawnPoints[i]->Get_Distance(Position);
					HighPriority = SpawnPoints[i]->Get_Priority();
					SpawnPoint = SpawnPoints[i];
				}
				else if (SpawnPoints[i]->Get_Priority() == HighPriority) {
					float Dis = SpawnPoints[i]->Get_Distance(Position);
					if (Dis < ClosestDis) {
						ClosestDis = Dis;
						SpawnPoint = SpawnPoints[i];
					}
				}
			}
		}
	}
	
	Player->NextSpawnLocation = "";

	if (SpawnPoint) {
		SpawnPoint->Add_Spawn_List(obj);
	}
	else {
		DA::Private_Color_Message(obj,COLORWHITE,"There are currently no available spawn points for your team.");
	}
}

void DASpawnManagerClass::Add_Spawn_List(int ID) {
	Add_Spawn_List(Get_GameObj(ID));
}

void DASpawnManagerClass::Re_Add_Spawn_List(GameObject *obj) {
	if (Remove_Spawn_List(obj)) {
		Add_Spawn_List(obj);
	}
}

void DASpawnManagerClass::Re_Add_Spawn_List(int ID) {
	Re_Add_Spawn_List(Get_GameObj(ID));
}

bool DASpawnManagerClass::Remove_Spawn_List(int ID) {
	for (int i = 0;i < SpawnPoints.Count();i++) {
		if (SpawnPoints[i]->Remove_Spawn_List(ID)) {
			Get_Player_Data(ID)->SpawnState = DASpawnState::WAITINGROOM;
			return true;
		}
	}
	return false;
}

bool DASpawnManagerClass::Remove_Spawn_List(GameObject *obj) {
	return Remove_Spawn_List(Get_Player_ID(obj));
}

void DASpawnManagerClass::Set_Next_Spawn_Location(GameObject *obj,const StringClass &Name) {
	Get_Player_Data(obj)->NextSpawnLocation = Name;
	Re_Add_Spawn_List(obj);
}

void DASpawnManagerClass::Set_Next_Spawn_Location(int ID,const StringClass &Name) {
	Get_Player_Data(ID)->NextSpawnLocation = Name;
	Re_Add_Spawn_List(ID);
}
