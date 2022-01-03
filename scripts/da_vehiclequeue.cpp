/*	Renegade Scripts.dll
    Dragonade Vehicle Purchase Queue Game Feature
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
#include "da_vehiclequeue.h"
#include "da_translation.h"
#include "da_player.h"
#include "da_game.h"
#include "VehicleFactoryGameObjDef.h"
#include "RefineryGameObj.h"

void DAVehicleQueueGameFeatureClass::Init() {
	Register_Event(DAEvent::LEVELLOADED);
	Register_Event(DAEvent::VEHICLEPURCHASEREQUEST);
	Register_Event(DAEvent::REQUESTVEHICLE,INT_MAX);
	Register_Event(DAEvent::TEAMCHANGE);
	Register_Event(DAEvent::PLAYERLEAVE);
	Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::VEHICLE);
	Register_Object_Event(DAObjectEvent::KILLRECEIVED,DAObjectEvent::BUILDING);
	Register_Chat_Command((DAECC)&DAVehicleQueueGameFeatureClass::VQ_Chat_Command,"!vq|!queue|!q|!veh|!vehicle|!vehlimit|!vehiclelimit|!vlimit|!limit");
	Queue[0].Resize(8);
	Queue[1].Resize(8);
	Building[0] = 0;
	Building[1] = 0;
}

DAVehicleQueueGameFeatureClass::~DAVehicleQueueGameFeatureClass() {
	if (!DAGameManager::Is_Shutdown_Pending()) {
		BaseControllerClass *Base = BaseControllerClass::Find_Base(0);
		VehicleFactoryGameObj *VF = (VehicleFactoryGameObj*)Base->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY);
		if (VF && Is_Building(0)) {
			VF->Set_Busy(true);
			if (Building[0]->Cost == -1) {
				RefineryGameObj *Ref = (RefineryGameObj*)Base->Find_Building(BuildingConstants::TYPE_REFINERY);
				if (Ref) {
					Ref->Allow_Harvester_Spawn();
				}
			}
		}

		Base = BaseControllerClass::Find_Base(1);
		VF = (VehicleFactoryGameObj*)Base->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY);
		if (VF && Is_Building(1)) {
			VF->Set_Busy(true);
			if (Building[1]->Cost == -1) {
				RefineryGameObj *Ref = (RefineryGameObj*)Base->Find_Building(BuildingConstants::TYPE_REFINERY);
				if (Ref) {
					Ref->Allow_Harvester_Spawn();
				}
			}
		}
	}

	Clear(0);
	Clear(1);
}

void DAVehicleQueueGameFeatureClass::Level_Loaded_Event() {
	Stop_Timer(1);
	Clear(0);
	Clear(1);
}

int DAVehicleQueueGameFeatureClass::Vehicle_Purchase_Request_Event(BaseControllerClass* Base, cPlayer* Player, float& Cost, const VehicleGameObjDef* Item) {
	int Team = Base->Get_Player_Type();
	if (Team == 0 || Team == 1) {
		if ((unsigned int)Player->Get_Money() < Cost) {
			return 2;
		}
		else if (!Is_Building(Team)) { //Build if VF is free.
			Player->Purchase_Item(Cost);
			Spawn_Vehicle(Team, Player, Item, Cost, 0);
			return 0;
		}
		else if (Building[Team]->Player == Player) {
			return 3;
		}
		else { //Add to queue if VF is busy.
			Add(Team, Player, Item, Cost, 0);
			return 1;
		}
	}
	return -1;
}

bool DAVehicleQueueGameFeatureClass::Request_Vehicle_Event(VehicleFactoryGameObj *Factory,const VehicleGameObjDef *Vehicle,cPlayer *Player,float Delay,SoldierGameObj *Owner) {
	int Team = Factory->Get_Player_Type();
	if ((Team == 0 || Team == 1) && Factory->Is_Available()) {
		if (Building[Team]) {
			if (Vehicle != Building[Team]->Vehicle && Player != Building[Team]->Player) { //Check if the VQ ordered this vehicle.
				if (!Player) {
					RefineryGameObj *Ref = (RefineryGameObj*)BaseControllerClass::Find_Base(Team)->Find_Building(BuildingConstants::TYPE_REFINERY);
					if (Ref && !Ref->Is_Destroyed() && !Ref->Get_Harvester_Vehicle() && (unsigned int)Ref->Get_Harvester_Def_ID() == Vehicle->Get_ID() && Ref->Get_Allow_Harvester_Spawn()) {
						Ref->Block_Harvester_Spawn(); //Block refinery from requesting a harvester so it doesn't spam requests. Will be re-enabled after the harvester is built.
						Add(Team,0,Vehicle,-1,0,true);
					}
					else {
						Add(Team,0,Vehicle,0,Owner);
					}
				}
				else {
					Add(Team,Player,Vehicle,0,0);
				}
			}
			else if (!Factory->Get_Generating_Vehicle_ID()) { //Allow VF to create the vehicle.
				return true;
			}
		}
		else if (!Player) {
			RefineryGameObj *Ref = (RefineryGameObj*)BaseControllerClass::Find_Base(Team)->Find_Building(BuildingConstants::TYPE_REFINERY);
			if (Ref && !Ref->Is_Destroyed() && !Ref->Get_Harvester_Vehicle() && (unsigned int)Ref->Get_Harvester_Def_ID() == Vehicle->Get_ID() && Ref->Get_Allow_Harvester_Spawn()) {
				Ref->Block_Harvester_Spawn();
				Spawn_Vehicle(Team,0,Vehicle,-1,0);
			}
			else {
				Spawn_Vehicle(Team,0,Vehicle,0,Owner);
			}
		}
		else {
			Spawn_Vehicle(Team,Player,Vehicle,0,0);
		}
	}
	return false;
}

void DAVehicleQueueGameFeatureClass::Team_Change_Event(cPlayer *Player) {
	Remove(0,Player);
	Remove(1,Player);
}

void DAVehicleQueueGameFeatureClass::Player_Leave_Event(cPlayer *Player) {
	Remove(0,Player);
	Remove(1,Player);
}

void DAVehicleQueueGameFeatureClass::Object_Created_Event(GameObject *obj) {
	int Team = ((VehicleGameObj*)obj)->Get_Definition().Get_Default_Player_Type();
	if (Team == 0 || Team == 1) {
		VehicleFactoryGameObj *VF = (VehicleFactoryGameObj*)BaseControllerClass::Find_Base(Team)->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY);
		if (VF && !VF->Is_Destroyed() && ((VehicleGameObj*)obj)->Peek_Physical_Object()->As_MoveablePhysClass() && (unsigned int)Get_Ground_Vehicle_Count(Team) == Get_Vehicle_Limit()) { //Clear the queue if the vehicle limit has been reached.
			DA::Team_Color_Message(Team,COLORGRAY,"Vehicle limit reached.");
			for (int i = Queue[Team].Count()-1;i >= 0;i--) {
				if (Queue[Team][i]->Cost != -1) {
					delete Queue[Team][i];
					Queue[Team].Delete(i);
				}
			}
		}
	}
}

void DAVehicleQueueGameFeatureClass::Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	int Team = Victim->Get_Player_Type();
	if (Team == 0 || Team == 1) {
		if (((BuildingGameObj*)Victim)->As_PowerPlantGameObj()) {
			if (!BaseControllerClass::Find_Base(Team)->Is_Base_Powered()) { //Double cost of everything in the queue.
				for (int i = 0;i < Queue[Team].Count();i++) {
					if (Queue[Team][i]->Cost != -1) {
						Queue[Team][i]->Cost *= 2;
					}
				}
			}
		}
		else if (((BuildingGameObj*)Victim)->As_VehicleFactoryGameObj()) { //Clear queue.
			Clear(Team);
		}
		else if (((BuildingGameObj*)Victim)->As_RefineryGameObj()) { //Remove harvester from queue.
			if (Queue[Team].Count() && Queue[Team][0]->Cost == -1) {
				((RefineryGameObj*)Victim)->Allow_Harvester_Spawn();
				delete Queue[Team][0];
				Queue[Team].Delete(0);
				Send_Positions(Team);
			}
		}
	}
}

bool DAVehicleQueueGameFeatureClass::VQ_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	int Team = Player->Get_Player_Type();
	if (Team == 0 || Team == 1) {
		if (!Text.Size()) {
			DA::Private_Color_Message(Player,COLORGRAY,"Limit: %d/%d",Get_Ground_Vehicle_Count(Team),Get_Vehicle_Limit());
			if (Building[Team]) {
				StringClass Name;
				if (Building[Team]->Player) {
					Name = Building[Team]->Player->Get_Name();
					Name += " - ";
				}
				else if (Building[Team]->Cost == -1) {
					RefineryGameObj *Ref = (RefineryGameObj*)BaseControllerClass::Find_Base(Team)->Find_Building(BuildingConstants::TYPE_REFINERY);
					if (Ref) {
						Name = DATranslationManager::Translate(Ref);
						Name += " - ";
					}
				}
				DA::Private_Color_Message(Player,COLORGRAY,"Building: %s%s",Name,DATranslationManager::Translate(Building[Team]->Vehicle));
			}
			for (int i = 0;i < Queue[Team].Count();++i) {
				StringClass Name;
				if (Queue[Team][i]->Player) {
					Name = Queue[Team][i]->Player->Get_Name();
					Name += " - ";
				}
				else if (Queue[Team][i]->Cost == -1) {
					RefineryGameObj *Ref = (RefineryGameObj*)BaseControllerClass::Find_Base(Team)->Find_Building(BuildingConstants::TYPE_REFINERY);
					if (Ref) {
						Name = DATranslationManager::Translate(Ref);
						Name += " - ";
					}
				}
				DA::Private_Color_Message(Player,COLORGRAY,"%d/%d: %s%s",i+1,Queue[Team].Count(),Name,DATranslationManager::Translate(Queue[Team][i]->Vehicle));
			}
		}
		else if (Text[1] == "cancel" || Text[1] == "halt" || Text[1] == "stop" || Text[1] == "leave" || Text[1] == "end") {
			if (Remove(Player->Get_Player_Type(),Player)) {
				DA::Private_Color_Message(Player,COLORGRAY,"You have left the vehicle queue.");
			}
		}
	}
	return true;
}

void DAVehicleQueueGameFeatureClass::Spawn_Vehicle(int Team,DAVehicleQueueStruct *Q) {
	VehicleFactoryGameObj *VF = (VehicleFactoryGameObj*)BaseControllerClass::Find_Base(Team)->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY);
	if (VF && VF->Is_Available()) {
		Building[Team] = Q;
		float Delay = 5.0f;
		if (!BaseControllerClass::Find_Base(Team)->Is_Base_Powered()) {
			Delay *= Get_Build_Time_Multiplier(Team);
		}
		VF->Request_Vehicle(Q->Vehicle->Get_ID(),Delay,Q->Player?Q->Player->Get_GameObj():Q->Owner);
		if (Q->Player && Q->Player->Is_Alive_And_Kicking()) { //Gray out for the Player.
			Update_Network_Object_Player(VF,Q->Player->Get_Id());
			VF->Set_Busy(false); //Prevent from graying out for other players.
			VF->Set_Object_Dirty_Bit(Q->Player->Get_Id(),NetworkObjectClass::BIT_RARE,false);
		}
		else {
			VF->Set_Busy(false);
		}
		Start_Timer(1,VF->Get_Definition().Get_Total_Building_Time()+0.1f,false,Team);
	}
	else {
		delete Q;
	}
}

void DAVehicleQueueGameFeatureClass::Spawn_Vehicle(int Team,cPlayer *Player,const VehicleGameObjDef *Vehicle,float Cost,SoldierGameObj *Owner) {
	Spawn_Vehicle(Team,new DAVehicleQueueStruct(Player,Vehicle,Cost,Owner));
}

void DAVehicleQueueGameFeatureClass::Timer_Expired(int Number,unsigned int Team) {
	if (Building[Team]) {
		if (Building[Team]->Player) {
			if (Building[Team]->Player->Is_Alive_And_Kicking()) { //Ungray menu for purchasing player.
				VehicleFactoryGameObj *VF = (VehicleFactoryGameObj*)BaseControllerClass::Find_Base(Team)->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY);
				VF->Set_Object_Dirty_Bit(Building[Team]->Player->Get_Id(),NetworkObjectClass::BIT_RARE,true);
			}
		}
		else if (Building[Team]->Cost == -1) { //Reallow refinery to request harvesters.
			RefineryGameObj *Ref = (RefineryGameObj*)BaseControllerClass::Find_Base(Team)->Find_Building(BuildingConstants::TYPE_REFINERY);
			if (Ref) {
				Ref->Allow_Harvester_Spawn();
			}
		}
		delete Building[Team];
		Building[Team] = 0;
	}
	if (Queue[Team].Count()) {
		DAVehicleQueueStruct *Q = Queue[Team][0];
		Queue[Team].Delete(0);
		Send_Positions(Team);
		if (!Q->Player) { //Harvester
			Spawn_Vehicle(Team,Q);
		}
		else if (Q->Player->Purchase_Item(Q->Cost)) {
			Spawn_Vehicle(Team,Q);
			DAEventManager::Vehicle_Purchase_Event(Q->Player,Q->Cost,Q->Vehicle); //Trigger the proper events.
			Send_Purchase_Response(Q->Player->Get_Id(),0);
		}
		else {
			Timer_Expired(Number,Team);
			DA::Private_Color_Message(Q->Player,COLORGRAY,"Insufficient funds. You have been removed from the vehicle queue.");
			if (Team == 0) {
				DA::Create_2D_Sound_Player(Q->Player,"M00EVAN_DSGN0024I1EVAN_snd.wav");
			}
			else {
				DA::Create_2D_Sound_Player(Q->Player,"M00EVAG_DSGN0028I1EVAG_snd.wav");
			}
		}
	}
}

void DAVehicleQueueGameFeatureClass::Clear(int Team) {
	if (Building[Team]) {
		if (Building[Team]->Cost == -1) { //Reallow refinery to request harvesters.
			RefineryGameObj *Ref = (RefineryGameObj*)BaseControllerClass::Find_Base(Team)->Find_Building(BuildingConstants::TYPE_REFINERY);
			if (Ref) {
				Ref->Allow_Harvester_Spawn();
			}
		}
		delete Building[Team];
		Building[Team] = 0;
	}
	for (int i = 0;i < Queue[Team].Count();i++) {
		if (Queue[Team][i]->Cost == -1) {
			RefineryGameObj *Ref = (RefineryGameObj*)BaseControllerClass::Find_Base(Team)->Find_Building(BuildingConstants::TYPE_REFINERY);
			if (Ref) {
				Ref->Allow_Harvester_Spawn();
			}
		}
		delete Queue[Team][i];
	}
	Queue[Team].Delete_All();
}

void DAVehicleQueueGameFeatureClass::Add(int Team,cPlayer *Player,const VehicleGameObjDef *Vehicle,float Cost,SoldierGameObj *Owner,bool Head) {
	if (Player) {
		if (Team == 0) {
			DA::Create_2D_Sound_Player(Player,"M00EVAN_DSGN0009I1EVAN_SND.wav");
		}
		else {
			DA::Create_2D_Sound_Player(Player,"M00EVAG_DSGN0012I1EVAG_SND.wav");
		}
		for (int i = 0;i < Queue[Team].Count();i++) {
			if (Queue[Team][i]->Player == Player) {
				Queue[Team][i]->Vehicle = Vehicle;
				Queue[Team][i]->Cost = Cost;
				DA::Private_Color_Message(Player,COLORGRAY,"You are in position %d/%d of the vehicle queue.",i+1,Queue[Team].Count());
				return;
			}
		}
		if (Head) {
			Queue[Team].Add_Head(new DAVehicleQueueStruct(Player,Vehicle,Cost,Owner));
			Send_Positions(Team);
		}
		else {
			Queue[Team].Add(new DAVehicleQueueStruct(Player,Vehicle,Cost,0));
			DA::Private_Color_Message(Player,COLORGRAY,"You are in position %d/%d of the vehicle queue.",Queue[Team].Count(),Queue[Team].Count());
		}
	}
	else if (Head) {
		Queue[Team].Add_Head(new DAVehicleQueueStruct(Player,Vehicle,Cost,Owner));
		Send_Positions(Team);
	}
	else {
		Queue[Team].Add(new DAVehicleQueueStruct(Player,Vehicle,Cost,Owner));
	}
}

bool DAVehicleQueueGameFeatureClass::Remove(int Team,cPlayer *Player) {
	for (int i = 0;i < Queue[Team].Count();i++) {
		if (Queue[Team][i]->Player == Player) {
			delete Queue[Team][i];
			Queue[Team].Delete(i);
			Send_Positions(Team);
			return true;
		}
	}
	return false;
}

void DAVehicleQueueGameFeatureClass::Send_Positions(int Team) {
	for (int i = 0;i < Queue[Team].Count();i++) {
		if (Queue[Team][i]->Player) {
			DA::Private_Color_Message(Queue[Team][i]->Player,COLORGRAY,"You are now in position %d/%d of the vehicle queue.",i+1,Queue[Team].Count());
		}
	}
}

Register_Game_Feature(DAVehicleQueueGameFeatureClass,"Vehicle Queue","EnableVehicleQueue",0);