/*	Renegade Scripts.dll
    Dragonade Singleplayer Vehicle Spawner Game Mode Framework
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
#include "da_vehiclespawn.h"
#include "da_vehicle.h"

DA_API DAVehicleSpawnManagerClass *DASingleton<DAVehicleSpawnManagerClass>::Instance = 0;

void DAVehicleSpawnPointClass::Init(const INIClass *INI,const StringClass &Header) {
	Group = Header;

	INI->Get_String(Name,The_Game()->MapName,StringFormat("%s_Name",Header));
	Position.X = INI->Get_Float(The_Game()->MapName,StringFormat("%s_Vehicle_X",Header),0.0f);
	Position.Y = INI->Get_Float(The_Game()->MapName,StringFormat("%s_Vehicle_Y",Header),0.0f);
	Position.Z = INI->Get_Float(The_Game()->MapName,StringFormat("%s_Vehicle_Z",Header),0.0f);

	Enable = INI->Get_Bool(The_Game()->MapName,StringFormat("%s_VehicleStartEnabled",Header),true);
	Team = INI->Get_Int(The_Game()->MapName,StringFormat("%s_Team",Header),2);
	Priority = INI->Get_Int(The_Game()->MapName,StringFormat("%s_VehiclePriority",Header),1);
	Facing = INI->Get_Float(The_Game()->MapName,StringFormat("%s_VehicleFacing",Header),0.0f);

	if (Team != 0 && Team != 1) {
		Team = 2;
	}
	if (Team == 2) {
		Enable = false;
	}
	if (Enable) {
		DAVehicleSpawnManager->Enable_Vehicle_Factory(Team,true);
	}
	else if (DAVehicleSpawnManager->All_Spawn_Points_Disabled_Team(Team)) {
		DAVehicleSpawnManager->Enable_Vehicle_Factory(Team,false);
	}
}

void DAVehicleSpawnPointClass::Spawn_Vehicle(const char *Preset,SoldierGameObj *Owner) {
	DefinitionClass *Def = Find_Named_Definition(Preset);
	if (Def && Def->Get_Class_ID() == CID_Vehicle) {
		Spawn_Vehicle((VehicleGameObjDef*)Def,Owner);
	}
}

void DAVehicleSpawnPointClass::Spawn_Vehicle(unsigned int DefID,SoldierGameObj *Owner) {
	DefinitionClass *Def = Find_Definition(DefID);
	if (Def && Def->Get_Class_ID() == CID_Vehicle) {
		Spawn_Vehicle((VehicleGameObjDef*)Def,Owner);
	}
}

void DAVehicleSpawnPointClass::Spawn_Vehicle(const VehicleGameObjDef *Def,SoldierGameObj *Owner) {
	VehicleGameObj *Vehicle = (VehicleGameObj*)Create_Object(Def,Position);
	Commands->Set_Facing(Vehicle,Facing);
	Vehicle->Lock_Vehicle(Owner,30.0f);
	if (DASpawnManager && DASpawnManager->Using_Waiting_Room()) {
		DASpawnManager->Set_Next_Spawn_Location(Owner,Get_Group());
	}
}

void DAVehicleSpawnPointClass::Set_Enabled(bool enable) {
	Enable = enable;
	if (Enable) {
		DAVehicleSpawnManager->Enable_Vehicle_Factory(Team,true);
	}
	else if (DAVehicleSpawnManager->All_Spawn_Points_Disabled_Team(Team)) {
		DAVehicleSpawnManager->Enable_Vehicle_Factory(Team,false);
	}
}

void DAVehicleSpawnPointClass::Set_Team(int team) {
	int OldTeam = Team;
	Team = team;
	Set_Can_Generate_Vehicles(Team,true);
	if (DAVehicleSpawnManager->All_Spawn_Points_Disabled_Team(OldTeam)) {
		DAVehicleSpawnManager->Enable_Vehicle_Factory(OldTeam,false);
	}
}

void DAVehicleAirDropPointClass::Spawn_Vehicle(const VehicleGameObjDef *Def,SoldierGameObj *Owner) {
	VehicleGameObj *Vehicle = DAVehicleManager::Air_Drop_Vehicle(Team,Def,Position,Facing);
	Vehicle->Lock_Vehicle(Owner,45.0f);
	if (DASpawnManager && DASpawnManager->Using_Waiting_Room()) {
		DASpawnManager->Set_Next_Spawn_Location(Owner,Get_Group());
	}
}

void DAVehicleSpawnManagerClass::Init(const INIClass *INI) {
	VF[0] = (VehicleFactoryGameObj*)BaseControllerClass::Find_Base(0)->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY);
	if (!VF[0]) {
		DefinitionClass *VFDef = DefinitionMgrClass::Find_Typed_Definition("mp_Nod_Airstrip",CID_Airstrip,false);
		VF[0] = (VehicleFactoryGameObj*)VFDef->Create();;
		VF[0]->Set_Position(Vector3(0,0,0));
		VF[0]->Collect_Building_Components();
		VF[0]->Start_Observers();
		VF[0]->Set_Is_Destroyed(true);
	}

	VF[1] = (VehicleFactoryGameObj*)BaseControllerClass::Find_Base(1)->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY);
	if (!VF[1]) {
		DefinitionClass *VFDef = DefinitionMgrClass::Find_Typed_Definition("mp_GDI_War_Factory",CID_WarFactory,false);
		VF[1] = (VehicleFactoryGameObj*)VFDef->Create();;
		VF[1]->Set_Position(Vector3(0,0,0));
		VF[1]->Collect_Building_Components();
		VF[1]->Start_Observers();
		VF[1]->Set_Is_Destroyed(true);
	}
	Register_Event(DAEvent::REQUESTVEHICLE);
}

bool DAVehicleSpawnManagerClass::Request_Vehicle_Event(VehicleFactoryGameObj *Factory,const VehicleGameObjDef *Vehicle,cPlayer *Player,float Delay,SoldierGameObj *Owner) {
	Spawn_Vehicle(Factory->Get_Player_Type(),Vehicle,Player->Get_GameObj());
	return false;
}

void DAVehicleSpawnManagerClass::Spawn_Vehicle(int Team,const char *Preset,SoldierGameObj *Owner) {
	DefinitionClass *Def = Find_Named_Definition(Preset);
	if (Def && Def->Get_Class_ID() == CID_Vehicle) {
		Spawn_Vehicle(Team,(VehicleGameObjDef*)Def,Owner);
	}
}

void DAVehicleSpawnManagerClass::Spawn_Vehicle(int Team,unsigned int DefID,SoldierGameObj *Owner) {
	DefinitionClass *Def = Find_Definition(DefID);
	if (Def && Def->Get_Class_ID() == CID_Vehicle) {
		Spawn_Vehicle(Team,(VehicleGameObjDef*)Def,Owner);
	}
}


void DAVehicleSpawnManagerClass::Spawn_Vehicle(int Team,const VehicleGameObjDef *Def,SoldierGameObj *Owner) {
	Vector3 Position;
	if (DASpawnManager && DASpawnManager->Using_Waiting_Room()) { //Find the closest spawn point to where they died if using the waiting room.
		Position = DASpawnManager->Get_Death_Position(Owner);
	}
	else { //Find the closest spawn point to their current position if using the normal PT.
		Position = Commands->Get_Position(Owner);
	}
	DAVehicleSpawnPointClass *SpawnPoint = 0;
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
	
	if (SpawnPoint) {
		Set_Vehicle_Factory_Busy(Team,true);
		SpawnPoint->Spawn_Vehicle(Def,Owner);
		Start_Timer(1,10.0f,false,Team);
	}
	else {
		DA::Private_Color_Message(Owner,COLORWHITE,"There are currently no vehicle spawn points available for your team.");
	}
}

void DAVehicleSpawnManagerClass::Timer_Expired(int Number,unsigned int Data) {
	Set_Vehicle_Factory_Busy(Data,false);
}
