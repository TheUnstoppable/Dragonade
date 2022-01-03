/*	Renegade Scripts.dll
    Dragonade Teleporter Game Mode Framework
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
#include "da_teleporter.h"
#include "da_settings.h"
#include "MoveablePhysClass.h"


DA_API DATeleporterManagerClass *DASingleton<DATeleporterManagerClass>::Instance = 0;


void DATeleporterClass::Init(const INIClass *INI,const StringClass &Header) {
	Group = Header;
	
	INI->Get_String(Name,The_Game()->MapName,StringFormat("%s_Name",Header));
	INI->Get_String(Link,The_Game()->MapName,StringFormat("%s_TeleporterLink",Header));
	Enable = INI->Get_Bool(The_Game()->MapName,StringFormat("%s_TeleporterStartEnabled",Header),true);
	Team = INI->Get_Int(The_Game()->MapName,StringFormat("%s_Team",Header),2);
	if (Team != 0 && Team != 1) {
		Team = 2;
	}

	Vector3 Entrance;
	Entrance.X = INI->Get_Float(The_Game()->MapName,StringFormat("%s_TeleporterEntrance_X",Header),0.0f);
	Entrance.Y = INI->Get_Float(The_Game()->MapName,StringFormat("%s_TeleporterEntrance_Y",Header),0.0f);
	Entrance.Z = INI->Get_Float(The_Game()->MapName,StringFormat("%s_TeleporterEntrance_Z",Header),0.0f);

	Vector3 Extent;
	Extent.X = INI->Get_Float(The_Game()->MapName,StringFormat("%s_TeleporterExtent_X",Header),0.0f);
	Extent.Y = INI->Get_Float(The_Game()->MapName,StringFormat("%s_TeleporterExtent_Y",Header),0.0f);
	Extent.Z = INI->Get_Float(The_Game()->MapName,StringFormat("%s_TeleporterExtent_Z",Header),0.0f);

	float Facing = INI->Get_Float(The_Game()->MapName,StringFormat("%s_TeleporterEntrance_Facing",Header),0.0f);

	Matrix3D Transform(true);
	Transform.Rotate_Z(DEG_TO_RADF(Facing));
	OBBoxClass BoundingBox(Entrance,Extent,Transform);

	Create_Zone("Script_Zone_Star",BoundingBox)->Add_Observer(this);

	INI->Get_String(Model,The_Game()->MapName,StringFormat("%s_TeleporterModel",Header),"null");
	Display = Commands->Create_Object("Generic_Cinematic",Entrance);
	if (Enable) {
		Commands->Set_Model(Display,Model);
	}

	for (int i = 1;;i++) {
		Vector3 SpawnPos;
		SpawnPos.X = INI->Get_Float(The_Game()->MapName,StringFormat("%s_TeleporterExit%d_X",Header,i),0.0f);
		SpawnPos.Y = INI->Get_Float(The_Game()->MapName,StringFormat("%s_TeleporterExit%d_Y",Header,i),0.0f);
		SpawnPos.Z = INI->Get_Float(The_Game()->MapName,StringFormat("%s_TeleporterExit%d_Z",Header,i),0.0f);
		if (SpawnPos.X || SpawnPos.Y || SpawnPos.Z) {
			Spawners.Add(SpawnPos);
		}
		else {
			break;
		}
	}
}

void DATeleporterClass::Entered(GameObject *obj,GameObject *enterer) {
	if (Is_Enabled() && (Team == 2 || Get_Object_Type(enterer) == Team) && !Is_In_Ignore_List(enterer)) {
		Teleport_Player(enterer);
	}
}

void DATeleporterClass::Exited(GameObject *obj,GameObject *exiter) {
	Remove_Ignore_List(exiter);
}

#define Do_Teleport() { Set_Transform(obj,SpawnPos); Commands->Create_Object("Spawner Created Special Effect",SpawnPos.Get_Translation()); }

void DATeleporterClass::Teleport_Player(GameObject *obj) const {
	if (!Link.Is_Empty()) {
		DATeleporterClass *Tele = DATeleporterManager->Find_Teleporter(Link);
		if (Tele) {
			Tele->Add_Ignore_List(obj);
		}
	}
	obj = Get_Vehicle_Return(obj);
	int Rand = Commands->Get_Random_Int(0,Spawners.Count());
	Matrix3D SpawnPos = Get_Transform(obj);
	SpawnPos.Set_Translation(Spawners[Rand]);
	MoveablePhysClass *Phys = obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass();
	if (Phys->Can_Teleport(SpawnPos)) {
		Do_Teleport();
		return;
	}
	for (int i = 0;i < Spawners.Count();i++) {
		SpawnPos.Set_Translation(Spawners[i]);
		if (Phys->Can_Teleport(SpawnPos)) {
			Do_Teleport();
			return;
		}
	}
	Rand = Commands->Get_Random_Int(0,Spawners.Count());
	SpawnPos.Set_Translation(Spawners[Rand]);
	Damage_All_Vehicles_Area(999999.0f,"Laser_NoBuilding",SpawnPos.Get_Translation(),3.0f,obj,0);
	Damage_All_Objects_Area(999999.0f,"Laser_NoBuilding",SpawnPos.Get_Translation(),2.0f,obj,0);
	Do_Teleport();
}