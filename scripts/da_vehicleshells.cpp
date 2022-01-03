/*	Renegade Scripts.dll
    Dragonade Vehicle Shells Game Feature
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
#include "engine_da.h"
#include "da.h"
#include "da_vehicleshells.h"
#include "da_damagelog.h"
#include "da_settings.h"
#include "da_vehicleownership.h"
#include "da_player.h"
#include "da_vehicle.h"
#include "da_gameobjobservers.h"
#include "MoveablePhysClass.h"

DAVehicleShellObserverClass::DAVehicleShellObserverClass(DAVehicleShellStruct *ShellDef,VehicleGameObj *Vehicle) {
	VOGFOwner = DAVehicleOwnershipGameFeature?DAVehicleOwnershipGameFeature->Get_Vehicle_Owner(Vehicle):0; //Previous owner according to the Vehicle Ownership Game Feature. Used for rebinding the revived vehicle.
	DAVehicleObserverClass *VehicleData = DAVehicleManager::Get_Vehicle_Data(Vehicle); //Previous owner according to the Vehicle Manager. Used to restore the revived vehicle's owner and team so the vehicle theft message is correct.
	VMOwner = VehicleData->Get_Vehicle_Owner();
	Team = VehicleData->Get_Team();

	VehicleDef = &Vehicle->Get_Definition();
	this->ShellDef = ShellDef;
	Matrix3D Transform = Vehicle->Get_Transform();
	const DefenseObjectDefClass *VehicleDefense = &VehicleDef->Get_DefenseObjectDef();

	//This is the new and improved implementation of vehicle shells. They have physics and as such can be at an angle and won't float when killed in mid-air. 
	//However, there is a bug that causes clients to crash when using the destroyed vehicle models with an object with moveable physics. 
	//This bug was fixed in 4.0, but we still can't use this method if it causes stock clients to crash. The solution is to create two shells,
	//one using the new method which 4.0 clients see, and one using the old method that stock clients see.

	//This is the "shadow" shell that stock clients see. Any damage done to it will be copied over to the "true" shell.
	VehicleGameObj *Shadow = (VehicleGameObj*)Create_Object(ShellDef->Def,Transform);
	Shadow->Set_Collision_Group(TERRAIN_AND_BULLET_COLLISION_GROUP);
	Shadow->Set_Player_Type(-2);
	Commands->Enable_Vehicle_Transitions(Shadow,false);
	if (!ShellDef->Model.Is_Empty()) {
		Commands->Set_Model(Shadow,ShellDef->Model);
	}
	DefenseObjectClass *ShadowDefense = Shadow->Get_Defense_Object();
	ShadowDefense->Set_Health_Max(VehicleDefense->HealthMax);
	ShadowDefense->Set_Shield_Strength_Max(VehicleDefense->ShieldStrengthMax);
	ShadowDefense->Set_Health((ShadowDefense->Get_Health_Max()+ShadowDefense->Get_Shield_Strength_Max())*0.1f);
	ShadowDefense->Set_Shield_Strength(0.0f);
	ShadowDefense->Set_Skin(VehicleDefense->Skin);
	ShadowDefense->Set_Shield_Type(VehicleDefense->ShieldType);
	ShadowDefense->Set_Damage_Points(0.0f);
	ShadowDefense->Set_Death_Points(0.0f);
	DAGameObjManager::Set_GameObj_Stock_Only(Shadow); //Only stock clients will see this object.
	this->Shadow = Shadow;

	//This is the "true" shell that 4.0 clients see.
	VehicleGameObjDef *MountedDef = (VehicleGameObjDef*)Find_Named_Definition("Mounted"); //This preset is used because it has no default team and won't count towards the vehicle limit.
	int PhysDefIDSave = MountedDef->PhysDefID;
	MountedDef->PhysDefID = VehicleDef->PhysDefID; //Copy the physics of the vehicle so the shell will have the proper physics.
	VehicleGameObj *Shell = (VehicleGameObj*)Create_Object(MountedDef,Transform);
	MountedDef->PhysDefID = PhysDefIDSave;
	Commands->Set_Model(Shell,Get_Model(Shadow));
	Shell->Set_Collision_Group(SOLDIER_GHOST_COLLISION_GROUP);
	Shell->Set_Player_Type(-2);
	Commands->Enable_Vehicle_Transitions(Shell,false);
	DefenseObjectClass *ShellDefense = Shell->Get_Defense_Object();
	ShellDefense->Set_Health_Max(VehicleDefense->HealthMax);
	ShellDefense->Set_Shield_Strength_Max(VehicleDefense->ShieldStrengthMax);
	ShellDefense->Set_Health((ShellDefense->Get_Health_Max()+ShellDefense->Get_Shield_Strength_Max())*0.1f);
	ShellDefense->Set_Shield_Strength(0.0f);
	ShellDefense->Set_Skin(VehicleDefense->Skin);
	ShellDefense->Set_Shield_Type(VehicleDefense->ShieldType);
	ShellDefense->Set_Damage_Points(0.0f);
	ShellDefense->Set_Death_Points(0.0f);
	DAGameObjManager::Set_GameObj_TT_Only(Shell); //Only TT clients will see this object.

	Shell->Add_Observer(this);
	Shadow->Add_Observer(new DAVehicleShellShadowObserverClass(Shell));

	Transform.Get_Translation(&LastPos);
	Update_Network_Object(Shell);
	Force_Orientation_Update(Shell);
	StartZ = LastPos.Z;
	Start_Timer(1,0.5f);
}

void DAVehicleShellObserverClass::Damage_Received(ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	DefenseObjectClass *ShellDefense = ((DamageableGameObj*)Get_Owner())->Get_Defense_Object();
	if (Damage < 0.0f) {
		if (ShellDefense->Get_Shield_Strength() == ShellDefense->Get_Shield_Strength_Max()) { //Spawn revived vehicle if fully repaired.
			Matrix3D Transform = ((PhysicalGameObj*)Get_Owner())->Get_Transform();
			Transform.Adjust_Z_Translation(ShellDef->ZOffset);
			PhysicalGameObj *Vehicle = Create_Object(VehicleDef,Transform);
			DefenseObjectClass *VehicleDefense = Vehicle->Get_Defense_Object();
			VehicleDefense->Set_Health((VehicleDefense->Get_Health_Max()+VehicleDefense->Get_Shield_Strength_Max())*0.05f);
			VehicleDefense->Set_Shield_Strength(0.0f);
			Vehicle->Set_Player_Type(-2);
			((PhysicalGameObj*)Get_Owner())->Set_Collision_Group(UNCOLLIDEABLE_GROUP);
			Fix_Stuck_Objects(Transform.Get_Translation(),10.0f,15.0f); //Move any objects that got stuck by the revived vehicle spawning on them.
			if (DAVehicleOwnershipGameFeature) {
				cPlayer *HighestRepairer = DADamageLog::Get_Highest_Repairer_Team(Get_Owner(),2);
				if (HighestRepairer) {
					if (VOGFOwner && VOGFOwner->Is_Active() && HighestRepairer->Get_Player_Type() == VOGFOwner->Get_Player_Type() && !DAVehicleOwnershipGameFeature->Get_Vehicle_Data(VOGFOwner) && DAVehicleOwnershipGameFeature->Bind_Vehicle((VehicleGameObj*)Vehicle,VOGFOwner)) {
						DA::Page_Player(VOGFOwner,"Your previously bound vehicle has been revived and rebound to you.");
					}
					else if (!DAVehicleOwnershipGameFeature->Get_Vehicle_Data(HighestRepairer) && DAVehicleOwnershipGameFeature->Bind_Vehicle((VehicleGameObj*)Vehicle,HighestRepairer)) {
						DA::Page_Player(HighestRepairer,"You have been given ownership of this vehicle. Use \"!lock\" to lock it, or \"!unbind\"/\"!free\" to relinquish ownership.");
					}
				}
			}
			Vehicle->Add_Observer(new DANoPointsUntilEnteredObserverClass);
			DAVehicleObserverClass *VehicleData = DAVehicleManager::Get_Vehicle_Data(Vehicle);
			VehicleData->Set_Vehicle_Owner(VMOwner);
			VehicleData->Set_Team(Team);
			Get_Owner()->Set_Delete_Pending();
		}
	}
	if (Shadow) { //Make sure the shadow has the same health as the shell.
		((DamageableGameObj*)Shadow.Get_Ptr())->Get_Defense_Object()->Set_Health(ShellDefense->Get_Health());
		((DamageableGameObj*)Shadow.Get_Ptr())->Get_Defense_Object()->Set_Shield_Strength(ShellDefense->Get_Shield_Strength());
	}
}

void DAVehicleShellObserverClass::Kill_Received(ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	Commands->Create_Explosion("Explosion_with_Debris_small",((PhysicalGameObj*)Get_Owner())->Get_Position(),Killer);
}

void DAVehicleShellObserverClass::Timer_Expired(GameObject *obj,int Number) {
	Vector3 CurrPos;
	((PhysicalGameObj*)obj)->Get_Position(&CurrPos);
	Start_Timer(1,0.5f);
	if (LastPos != CurrPos) {
		if (Diff(LastPos.Z,CurrPos.Z) <= 0.1) {
			if (Diff(StartZ,CurrPos.Z) > 15.0f) { //Shells will explode if they fall more than 15 meters.
				Commands->Apply_Damage(obj,99999.0f,"None",0);
			}
		}
		LastPos = CurrPos;
		if (this->Shadow) {
			this->Shadow->Set_Delete_Pending();
		}
		DefenseObjectClass *ShellDefense = ((DamageableGameObj*)Get_Owner())->Get_Defense_Object();
		
		//Re-create shadow at new position.
		VehicleGameObj *Shadow = (VehicleGameObj*)Create_Object(ShellDef->Def,((PhysicalGameObj*)obj)->Get_Transform());
		Shadow->Set_Collision_Group(TERRAIN_AND_BULLET_COLLISION_GROUP);
		Shadow->Set_Player_Type(-2);
		Commands->Enable_Vehicle_Transitions(Shadow,false);
		if (!ShellDef->Model.Is_Empty()) {
			Commands->Set_Model(Shadow,ShellDef->Model);
		}
		DefenseObjectClass *ShadowDefense = Shadow->Get_Defense_Object();
		ShadowDefense->Set_Health_Max(ShellDefense->Get_Health_Max());
		ShadowDefense->Set_Shield_Strength_Max(ShellDefense->Get_Shield_Strength_Max());
		ShadowDefense->Set_Health(ShellDefense->Get_Health());
		ShadowDefense->Set_Shield_Strength(ShellDefense->Get_Shield_Strength());
		ShadowDefense->Set_Skin(ShellDefense->Get_Skin());
		ShadowDefense->Set_Shield_Type(ShellDefense->Get_Shield_Type());
		ShadowDefense->Set_Damage_Points(0.0f);
		ShadowDefense->Set_Death_Points(0.0f);
		DAGameObjManager::Set_GameObj_Stock_Only(Shadow);
		Shadow->Add_Observer(new DAVehicleShellShadowObserverClass(obj));
		this->Shadow = Shadow;
	}
}

DAVehicleShellObserverClass::~DAVehicleShellObserverClass() {
	if (Shadow) {
		DAGameObjManager::Set_GameObj_Delete_Pending(Shadow);
	}
}

DAVehicleShellShadowObserverClass::DAVehicleShellShadowObserverClass(GameObject *Shell) {
	this->Shell = Shell;
}

bool DAVehicleShellShadowObserverClass::Damage_Received_Request(ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type) {
	if (Type == DADamageType::EXPLOSION || Type == DADamageType::SPLASH) {
		return false;
	}
	return true;
}

void DAVehicleShellShadowObserverClass::Damage_Received(ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	Commands->Apply_Damage(Shell,Damage,"None",Damager); //Copy damage to shell.
}

void DAVehicleShellsGameFeatureClass::Init() {
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Object_Event(DAObjectEvent::KILLRECEIVED,DAObjectEvent::VEHICLE);
}

void DAVehicleShellsGameFeatureClass::Settings_Loaded_Event() {
	Shells.Remove_All();
	INISection *Section = DASettingsManager::Get_Section("Vehicle_Shells");
	if (Section) {
		for (INIEntry *i = Section->EntryList.First();i && i->Is_Valid();i = i->Next()) {
			VehicleGameObjDef *VehicleDef = (VehicleGameObjDef*)Find_Named_Definition(i->Entry);
			if (VehicleDef && VehicleDef->Get_Class_ID() == CID_Vehicle) {
				DATokenParserClass Parser(i->Value,'|');
				const char *DefName = Parser.Get_String();
				if (DefName) {
					VehicleGameObjDef *ShellDef = (VehicleGameObjDef*)Find_Named_Definition(DefName);
					if (ShellDef) {
						if (ShellDef->Get_Class_ID() == CID_Vehicle) {
							DAVehicleShellStruct Struct;
							Struct.Def = ShellDef;
							Struct.Model = Parser.Get_String();
							if (Struct.Model == " ") {
								Struct.Model = "";
							}
							float ZOffset = 0.0f;
							Parser.Get_Float(ZOffset);
							Struct.ZOffset = ZOffset;
							Shells.Insert((unsigned int)VehicleDef,Struct);
						}
						else if (ShellDef->Get_Class_ID() == CID_Explosion) {
							DAVehicleShellStruct Struct;
							Struct.Def = 0;
							Struct.ZOffset = 0.0f;
							Struct.Explosion = DefName;
							Shells.Insert((unsigned int)VehicleDef,Struct);
						}
					}
				}
			}
		}
	}
}

void DAVehicleShellsGameFeatureClass::Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	DAVehicleShellStruct *Shell = Get_Shell((VehicleGameObj*)Victim);
	if (Shell) {
		if (Shell->Def && ((VehicleGameObj*)Victim)->Are_Transitions_Enabled() && ((VehicleGameObj*)Victim)->Get_Definition().Get_Seat_Count()) { //Don't spawn shells for AI vehicles.
			new DAVehicleShellObserverClass(Shell,(VehicleGameObj*)Victim);
		}
		else {
			Commands->Create_Explosion(Shell->Explosion,((PhysicalGameObj*)Victim)->Get_Position(),Killer);
		}
	}
}

Register_Game_Feature(DAVehicleShellsGameFeatureClass,"Vehicle Shells","EnableVehicleShells",0);

