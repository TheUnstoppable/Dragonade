/*	Renegade Scripts.dll
    Dragonade Vehicle Manager
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
#include "da_vehicle.h"
#include "da_translation.h"
#include "da_log.h"
#include "da_settings.h"
#include "da_damagelog.h"
#include "da_chatcommand.h"
#include "da_player.h"
#include "VehicleFactoryGameObj.h"
#include "AirFactoryGameObj.h"
#include "NavalFactoryGameObj.h"
#include "MoveablePhysDefClass.h"
#include "RefineryGameObj.h"
#include "GameObjManager.h"

static const char *ObserverName = "DAVehicleObserverClass";

void DAVehicleObserverClass::Init() {
	VehicleOwner = 0;
	LastExitTime = 0;
	LastTheftTime = 0;
	Team = -2;
	Start_Timer(1,0.0f);
}

void DAVehicleObserverClass::Timer_Expired(GameObject* obj, int Number) {
	if (!VehicleOwner && Get_Vehicle()->Get_Lock_Owner()) {
		VehicleOwner = ((SoldierGameObj*)Get_Vehicle()->Get_Lock_Owner())->Get_Player();
		if (VehicleOwner)
			Team = VehicleOwner->Get_Player_Type();
		else
			Team = Get_Object_Type(Get_Vehicle()->Get_Lock_Owner());
	}
	if (Team == -2) {
		Team = Get_Vehicle()->Get_Player_Type();
	}
}

void DAVehicleObserverClass::Vehicle_Exit(cPlayer *Player,int Seat) {
	Reset_Last_Exit_Time();
}

cPlayer *DAVehicleObserverClass::Get_Vehicle_Owner() {
	return VehicleOwner;
}

void DAVehicleObserverClass::Set_Vehicle_Owner(cPlayer *Player) {
	VehicleOwner = Player;
}

int DAVehicleObserverClass::Get_Team() {
	return Team;
}

void DAVehicleObserverClass::Set_Team(int team) {
	Team = team;
}

void DAVehicleObserverClass::Reset_Last_Exit_Time() {
	LastExitTime = The_Game()->Get_Game_Duration_S();
}

unsigned int DAVehicleObserverClass::Get_Time_Since_Last_Exit() {
	return The_Game()->Get_Game_Duration_S()-LastExitTime;
}

void DAVehicleObserverClass::Reset_Last_Theft_Time() {
	LastTheftTime = The_Game()->Get_Game_Duration_S();
}

unsigned int DAVehicleObserverClass::Get_Time_Since_Last_Theft() {
	return The_Game()->Get_Game_Duration_S()-LastTheftTime;
}

const char *DAVehicleObserverClass::Get_Name() {
	return ObserverName;
}

void DAAirDroppedVehicleObserverClass::Init() {
	Start_Timer(1,14.0f);
	Start_Timer(3,20.0f);
	((VehicleGameObj*)Get_Owner())->Set_Is_Scripts_Visible(false);
	CollisionGroup = ((VehicleGameObj*)Get_Owner())->Get_Collision_Group();
	((VehicleGameObj*)Get_Owner())->Set_Collision_Group(TERRAIN_AND_BULLET_COLLISION_GROUP);
}

bool DAAirDroppedVehicleObserverClass::Damage_Received_Request(ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type) {
	return false;
}

void DAAirDroppedVehicleObserverClass::Timer_Expired(GameObject *obj,int Number) {
	if (Number == 1) {
		LastZ = ((VehicleGameObj*)Get_Owner())->Get_Position().Z;
		Start_Timer(2,0.5f);
	}
	else if (Number == 2) {
		float CurrZ = ((VehicleGameObj*)Get_Owner())->Get_Position().Z;
		if (CurrZ >= LastZ) {
			Timer_Expired(Get_Owner(),3);
		}
		else {
			LastZ = CurrZ;
			Start_Timer(2,0.5f);
		}
	}
	else if (Number == 3) {
		((VehicleGameObj*)Get_Owner())->Set_Is_Scripts_Visible(true);
		((VehicleGameObj*)Get_Owner())->Set_Collision_Group(CollisionGroup);
		Fix_Stuck_Objects(((PhysicalGameObj*)Get_Owner())->Get_Position(),10.0f,15.0f);
		Set_Delete_Pending();
	}
}

void DAAirDroppedVehicleObserverClass::Vehicle_Enter(cPlayer *Player,int Seat) {
	Timer_Expired(Get_Owner(),3);
}

void DAVehicleManager::Init() {
	static DAVehicleManager Instance;

	Instance.Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::VEHICLE,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::KILLRECEIVED,DAObjectEvent::VEHICLE | DAObjectEvent::BUILDING,INT_MAX);
	Instance.Register_Event(DAEvent::LEVELLOADED,INT_MAX);
	Instance.Register_Event(DAEvent::SETTINGSLOADED,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEPURCHASEREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEENTRYREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEENTER,INT_MIN); //Theft stuff needs to trigger after all other vehicle enter events.

	static DefaultPurchaseEvent Instance2;
	Instance2.Register_Event(DAEvent::VEHICLEPURCHASEREQUEST,INT_MIN);
}

bool DAVehicleManager::Check_Limit_For_Player(cPlayer *Player) {
	return !((unsigned int)Get_Ground_Vehicle_Count(Player->Get_Player_Type()) >= Get_Vehicle_Limit());
}

DAVehicleObserverClass *DAVehicleManager::Get_Vehicle_Data(GameObject *obj) {
	const SimpleDynVecClass<GameObjObserverClass*> &Observers = obj->Get_Observers();
	for (int i = 0;i < Observers.Count();i++) {
		if (Observers[i]->Get_Name() == ObserverName) {
			return (DAVehicleObserverClass*)Observers[i];
		}
	}
	return 0;
}

cPlayer *DAVehicleManager::Get_Vehicle_Owner(GameObject *obj) {
	DAVehicleObserverClass *Data = Get_Vehicle_Data(obj);
	if (Data) {
		return Data->Get_Vehicle_Owner();
	}
	return 0;
}

int DAVehicleManager::Get_Team(GameObject *obj) {
	DAVehicleObserverClass *Data = Get_Vehicle_Data(obj);
	if (Data) {
		return Data->Get_Team();
	}
	return -2;
}

void DAVehicleManager::Air_Drop_Vehicle(int Team,VehicleGameObj *Vehicle,const Vector3 &Position,float Facing) {
	Vehicle->Set_Player_Type(Team);
	GameObject *Cin = Create_Object("Invisible_Object",Position);
	Commands->Set_Facing(Cin,Facing);
	if (Vehicle->Get_Definition().Get_Type() == VEHICLE_TYPE_FLYING) {
		Commands->Attach_Script(Cin,"Test_Cinematic","Vehicle_Fly_In.txt"); //Flying vehicles fly in.
	}
	else {
		Commands->Attach_Script(Cin,"Test_Cinematic",StringFormat("%s_Vehicle_Drop.txt",Team?"GDI":"Nod")); //Ground vehicles are carried by a transport helicopter.
	}
	Commands->Send_Custom_Event(Vehicle,Cin,10004,Vehicle->Get_ID(),0); //Insert vehicle into cinematic at slot 4.
	PhysicalGameObj *Flare = Create_Object("SignalFlare_Gold_Phys3",Position);
	if (Flare) {
		Flare->Peek_Physical_Object()->Set_Collision_Group(TERRAIN_ONLY_COLLISION_GROUP);
		Commands->Send_Custom_Event(Flare,Cin,10008,Flare->Get_ID(),0); //Insert flare into cinematic at slot 8.
	}
	Vehicle->Add_Observer(new DAAirDroppedVehicleObserverClass);
}

VehicleGameObj *DAVehicleManager::Air_Drop_Vehicle(int Team,const VehicleGameObjDef *Vehicle,const Vector3 &Position,float Facing) {
	VehicleGameObj *Veh = (VehicleGameObj*)Create_Object(Vehicle,Vector3(0,0,0));
	if (Veh) {
		Air_Drop_Vehicle(Team,Veh,Position,Facing);
		return Veh;
	}
	return 0;
}

VehicleGameObj *DAVehicleManager::Air_Drop_Vehicle(int Team,unsigned int Vehicle,const Vector3 &Position,float Facing) {
	VehicleGameObj *Veh = (VehicleGameObj*)Create_Object(Vehicle,Vector3(0,0,0));
	if (Veh) {
		Air_Drop_Vehicle(Team,Veh,Position,Facing);
		return Veh;
	}
	return 0;
}

VehicleGameObj *DAVehicleManager::Air_Drop_Vehicle(int Team,const char *Vehicle,const Vector3 &Position,float Facing) {
	VehicleGameObj *Veh = (VehicleGameObj*)Create_Object(Vehicle,Vector3(0,0,0));
	if (Veh) {
		Air_Drop_Vehicle(Team,Veh,Position,Facing);
		return Veh;
	}
	return 0;
}

void DAVehicleManager::Level_Loaded_Event() {
	VehicleGameObjDef *Def = (VehicleGameObjDef*)Find_Named_Definition("CnC_Nod_Recon_Bike"); //Setup Recon Bike for use by other things.
	if (Def) {
		Def->WeaponDefID = Get_Definition_ID("Weapon_StealthTank_Player");
		DefenseObjectDefClass &Defense = const_cast<DefenseObjectDefClass&>(Def->Get_DefenseObjectDef());
		Defense.Skin = ArmorWarheadManager::Get_Armor_Type("CNCVehicleMedium");
		Defense.ShieldType = ArmorWarheadManager::Get_Armor_Type("CNCVehicleMedium");
		Defense.ShieldStrengthMax = 150;
		Defense.ShieldStrength = 150;
	}
	Def = (VehicleGameObjDef*)Find_Named_Definition("Nod_SSM_Launcher_Player"); //Setup SSM for use by other things.
	if (Def) {
		Def->WeaponDefID = 0;
		DefenseObjectDefClass &Defense = const_cast<DefenseObjectDefClass&>(Def->Get_DefenseObjectDef());
		Defense.Skin = ArmorWarheadManager::Get_Armor_Type("CNCVehicleHeavy");
		Defense.ShieldType = ArmorWarheadManager::Get_Armor_Type("CNCVehicleHeavy");
		Defense.ShieldStrengthMax = 125;
		Defense.ShieldStrength = 125;
	}
}

void DAVehicleManager::Settings_Loaded_Event() {
	EnableTheftMessage = DASettingsManager::Get_Bool("EnableVehicleTheftMessage",true);
	Set_Vehicle_Limit((unsigned int)DASettingsManager::Get_Int("VehicleLimit",8));
	if (DASettingsManager::Get_Bool("DisableVehicleFlipKill",false)) {
		Register_Event(DAEvent::VEHICLEFLIP);
	}
	else {
		Unregister_Event(DAEvent::VEHICLEFLIP);
	}
}

void DAVehicleManager::Object_Created_Event(GameObject *obj) {
	obj->Add_Observer(new DAVehicleObserverClass);
	int Team = ((VehicleGameObj*)obj)->Get_Definition().Get_Default_Player_Type();
	if (Team == 0 || Team == 1) {
		RefineryGameObj *Ref = (RefineryGameObj*)BaseControllerClass::Find_Base(Team)->Find_Building(BuildingConstants::TYPE_REFINERY);
		if (Ref) {
			if ((unsigned int)Ref->Get_Harvester_Def_ID() == obj->Get_Definition().Get_ID() && Ref->Is_Destroyed()) { //Fix bug where the GDI Harvester can still exist if it is building when the Refinery is killed.
				obj->Set_Delete_Pending();
			}
		}
	}
}

//Limit checking is done here for per-player and per-team limits(NYI)
int DAVehicleManager::Vehicle_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const VehicleGameObjDef *Item) {
	VehicleFactoryGameObj *VF = (VehicleFactoryGameObj*)Base->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY);
	AirFactoryGameObj *AF = (AirFactoryGameObj*)Base->Find_Building(BuildingConstants::TYPE_HELIPAD);
	NavalFactoryGameObj *NF = (NavalFactoryGameObj*)Base->Find_Building(BuildingConstants::TYPE_NAVAL_FACTORY);
	if (AF && Item->Get_Type() == VEHICLE_TYPE_FLYING) { //Flying vehicle
		if ((unsigned int)Get_Air_Vehicle_Count(Base->Get_Player_Type()) >= Get_Air_Vehicle_Limit()) {
			return 4;
		}
		else {
			return -1;
		}
	}
	else if (NF && Item->Get_Type() == VEHICLE_TYPE_BOAT || Item->Get_Type() == VEHICLE_TYPE_SUB) { //Naval vehicle
		if ((unsigned int)Get_Naval_Vehicle_Count(Base->Get_Player_Type()) >= Get_Naval_Vehicle_Limit()) {
			return 4;
		}
		else {
			return -1;
		}
	}
	else if (VF) { //Ground vehicle
		if (!Check_Limit_For_Player(Player)) {
			return 4;
		}
		else {
			return -1;
		}
	}
	return 3;
}

//Default vehicle purchase handler.
//Seperate from the above so the vehicle purchase event can be overloaded.
int DAVehicleManager::DefaultPurchaseEvent::Vehicle_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const VehicleGameObjDef *Item) {
	VehicleFactoryGameObj *VF = (VehicleFactoryGameObj*)Base->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY);
	AirFactoryGameObj *AF = (AirFactoryGameObj*)Base->Find_Building(BuildingConstants::TYPE_HELIPAD);
	NavalFactoryGameObj *NF = (NavalFactoryGameObj*)Base->Find_Building(BuildingConstants::TYPE_NAVAL_FACTORY);
	if (AF && Item->Get_Type() == VEHICLE_TYPE_FLYING) { //Flying vehicle
		if (!AF->Is_Available()) {
			return 3;
		}
		else if (Player->Purchase_Item((int)Cost)) {
			AF->Create_Vehicle(Item->Get_ID(),Player->Get_GameObj());
			return 0;
		}
		return 2;
	}
	else if (NF && Item->Get_Type() == VEHICLE_TYPE_BOAT || Item->Get_Type() == VEHICLE_TYPE_SUB) { //Naval vehicle
		if (!NF->Is_Available() || !NF->Can_Spawn(Item->Get_ID())) {
			return 3;
		}
		else if (Player->Purchase_Item((int)Cost)) {
			NF->Create_Vehicle(Item->Get_ID(),Player->Get_GameObj());
			return 0;
		}
		return 2;
	}
	else if (VF) { //Ground vehicle
		if (!VF->Is_Available()) {
			return 3;
		}
		else if (Player->Purchase_Item((int)Cost)) {
			float Delay = 5.0f;
			if (!Base->Is_Base_Powered()) {
				Delay *= Get_Build_Time_Multiplier(Base->Get_Player_Type());
			}
			VF->Request_Vehicle(Item->Get_ID(),Delay,Player->Get_GameObj());
			return 0;
		}
		return 2;
	}
	return 3;
}

void DAVehicleManager::Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	if (Victim->As_VehicleGameObj()) {
		if (Is_Player(Killer)) { //Fix this stat so it only counts enemy vehicles.
			((SoldierGameObj*)Killer)->Get_Player()->VehiclesDestroyed--;
		}
		StringClass Message;
		if (!((VehicleGameObj*)Victim)->Are_Transitions_Enabled() || !((VehicleGameObj*)Victim)->Get_Definition().Get_Seat_Count()) { //Harvesters/Defenses/AI Vehicles
			if (Victim->Get_Player_Type() != -2) {
				StringClass VictimName;
				StringClass Header;
				if (Is_Harvester_Preset(Victim)) {
					VictimName = "the " + DATranslationManager::Translate_With_Team_Name(Victim);
					Header = "_HARVKILL";
				}
				else if (((VehicleGameObj*)Victim)->Is_Turret()) {
					VictimName = a_or_an_Prepend(DATranslationManager::Translate_With_Team_Name(Victim));
					Header = "_BUILDINGKILL";
				}
				else {
					VictimName = a_or_an_Prepend(DATranslationManager::Translate(Victim));
					Header = "_BOTKILL";
				}
				if (!Killer) { //No killer
					VictimName[0] = (char)toupper(VictimName[0]);
					Message.Format("%d %s was destroyed.",Victim->Get_Player_Type(),VictimName);
				}
				else {
					if (Killer->As_SoldierGameObj()) { //Killed by player
						if (((SoldierGameObj*)Killer)->Get_Player()) {
							if (Type == DADamageType::EXPLOSION) {
								Message.Format("%d %ls destroyed %s (%s VS. %s)", Killer->Get_Player_Type(), Get_Wide_Player_Name(Killer), VictimName, DATranslationManager::Translate(GetExplosionObj()), DATranslationManager::Translate(Victim));
							}
							else {
								Message.Format("%d %ls destroyed %s (%s VS. %s)", Killer->Get_Player_Type(), Get_Wide_Player_Name(Killer), VictimName, DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate(Victim));
							}
						}
						else { //Killed by bot
							if (Type == DADamageType::EXPLOSION) {
								if (Is_Smart_Bot(Killer)) {
									Message.Format("%d %ls destroyed %s (%s VS. %s)", Killer->Get_Player_Type(), ((SoldierGameObj*)Killer)->Get_Bot_Tag(), VictimName, DATranslationManager::Translate(GetExplosionObj()), DATranslationManager::Translate(Victim));
								}
								else {
									Message.Format("%d %s destroyed %s (%s VS. %s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Killer)), VictimName, DATranslationManager::Translate(GetExplosionObj()), DATranslationManager::Translate(Victim));
								}
							}
							else {
								if (Is_Smart_Bot(Killer)) {
									Message.Format("%d %ls destroyed %s (%s VS. %s)", Killer->Get_Player_Type(), ((SoldierGameObj*)Killer)->Get_Bot_Tag(), VictimName, DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate(Victim));
								}
								else {
									Message.Format("%d %s destroyed %s (%s VS. %s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Killer)), VictimName, DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate(Victim));
								}
							}
						}
					}
					else if (Killer->As_VehicleGameObj()) { //Killed by vehicle. Could be defense or AI vehicle.
						if (Killer == Victim) { //Suicide
							VictimName[0] = (char)toupper(VictimName[0]);
							Message.Format("%d %s destroyed itself.", Victim->Get_Player_Type(), VictimName);
						}
						else if (Killer->Get_Defense_Object()->Get_Shield_Type() == 1) {
							Message.Format("%d The %s destroyed %s (%s VS. %s)", Killer->Get_Player_Type(), DATranslationManager::Translate_With_Team_Name(Killer), VictimName, DATranslationManager::Translate(Killer), DATranslationManager::Translate(Victim));
						}
						else if (((VehicleGameObj*)Killer)->Is_Turret()) {
							Message.Format("%d %s destroyed %s (%s VS. %s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate_With_Team_Name(Killer)), VictimName, DATranslationManager::Translate(Killer), DATranslationManager::Translate(Victim));
						}
						else if (Is_Smart_Bot(Killer)) {
							Message.Format("%d %ls destroyed %s (%s VS. %s)", Killer->Get_Player_Type(), ((VehicleGameObj*)Killer)->Get_Driver()->Get_Bot_Tag(), VictimName, DATranslationManager::Translate(Killer), DATranslationManager::Translate(Victim));
						}
						else {
							Message.Format("%d %s destroyed %s (%s VS. %s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Killer)), VictimName, DATranslationManager::Translate(Killer), DATranslationManager::Translate(Victim));
						}
					}
				}
				DALogManager::Write_Log(Header,"%s",Message);
			}
		}
		else { //Player vehicle
			cPlayer *Owner = 0;
			if (((VehicleGameObj*)Victim)->Get_Occupant(0)) {
				Owner = ((VehicleGameObj*)Victim)->Get_Occupant(0)->Get_Player();
			}
			else if (cPlayer *Player = Get_Vehicle_Owner(Victim)) {
				if (Player->Is_Active()) {
					Owner = Player;
				}
			}
			else if (((VehicleGameObj*)Victim)->Get_Owner()) {
				Owner = ((VehicleGameObj*)Victim)->Get_Owner()->Get_Player();
			}
			else if (((VehicleGameObj*)Victim)->Get_Lock_Owner()) {
				Owner = ((SoldierGameObj*)((VehicleGameObj*)Victim)->Get_Lock_Owner())->Get_Player();
			}
			StringClass VictimName;
			if (Owner) {
				if (Owner->Get_GameObj() == Killer) {
					if (!DADamageLog::Get_Percent_Other_Team_Damage(Victim,DAVehicleManager::Get_Team(Victim))) {
						return;
					}
					VictimName = "their own ";
					VictimName += DATranslationManager::Translate(Victim);
				}
				else {
					VictimName = Make_Possessive(Owner->Get_Name());
					VictimName += " ";
					VictimName += DATranslationManager::Translate(Victim);
				}
			}
			else {
				if (Is_Smart_Bot(Victim)) {
					VictimName = StringClass(((VehicleGameObj*)Victim)->Get_Driver()->Get_Bot_Tag().Peek_Buffer());
					VictimName += "'s ";
					VictimName += DATranslationManager::Translate(Victim);
				}
				else {
					VictimName = a_or_an_Prepend(DATranslationManager::Translate(Victim));
				}
			}
			if (!Killer) { //No killer
				if (!Owner) {
					VictimName[0] = (char)toupper(VictimName[0]);
					Message.Format("2 %s was destroyed.",VictimName);
				}
				else {
					Message.Format("%d %s was destroyed.",Owner->Get_Player_Type(),VictimName);
				}
			}
			else if (Killer->As_SoldierGameObj()) { //Killed by soldier
				if (((SoldierGameObj*)Killer)->Get_Player()) { //Killed by player
					if (Killer->Get_Player_Type() != DAVehicleManager::Get_Team(Victim)) {
						((SoldierGameObj*)Killer)->Get_Player()->VehiclesDestroyed++;
					}
					if (Type == DADamageType::EXPLOSION) {
						Message.Format("%d %ls destroyed %s (%s VS. %s)",Killer->Get_Player_Type(),Get_Wide_Player_Name(Killer),VictimName,DATranslationManager::Translate(GetExplosionObj()),DATranslationManager::Translate(Victim));
					}
					else {
						Message.Format("%d %ls destroyed %s (%s VS. %s)",Killer->Get_Player_Type(),Get_Wide_Player_Name(Killer),VictimName,DATranslationManager::Translate_Soldier(Killer),DATranslationManager::Translate(Victim));
					}
				}
				else { //Killed by bot
					if (Type == DADamageType::EXPLOSION) {
						if (Is_Smart_Bot(Killer)) {
							Message.Format("%d %ls destroyed %s (%s VS. %s)", Killer->Get_Player_Type(), Killer->As_SoldierGameObj()->Get_Bot_Tag(), VictimName, DATranslationManager::Translate(GetExplosionObj()), DATranslationManager::Translate(Victim));
						}
						else {
							Message.Format("%d %s destroyed %s (%s VS. %s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Killer)), VictimName, DATranslationManager::Translate(GetExplosionObj()), DATranslationManager::Translate(Victim));
						}
					}
					else {
						if (Is_Smart_Bot(Killer)) {
							Message.Format("%d %ls destroyed %s (%s VS. %s)", Killer->Get_Player_Type(), Killer->As_SoldierGameObj()->Get_Bot_Tag(), VictimName, DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate(Victim));
						}
						else {
							Message.Format("%d %s destroyed %s (%s VS. %s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Killer)), VictimName, DATranslationManager::Translate_Soldier(Killer), DATranslationManager::Translate(Victim));
						}
					}
				}
			}
			else if (Killer->As_VehicleGameObj()) { //Killed by vehicle. Could be defense or AI vehicle.
				if (Victim == Killer) {
					if (!Owner) {
						VictimName[0] = (char)toupper(VictimName[0]);
						Message.Format("2 %s destroyed itself.",VictimName);
					}
					else {
						Message.Format("%d %s destroyed itself.",Owner->Get_Player_Type(),VictimName);
					}
				}
				else if (Killer->Get_Defense_Object()->Get_Shield_Type() == 1) {
					Message.Format("%d The %s destroyed %s (%s VS. %s)",Killer->Get_Player_Type(),DATranslationManager::Translate_With_Team_Name(Killer),VictimName,DATranslationManager::Translate(Killer),DATranslationManager::Translate(Victim));
				}
				else if (((VehicleGameObj*)Killer)->Is_Turret()) {
					Message.Format("%d %s destroyed %s (%s VS. %s)",Killer->Get_Player_Type(),A_Or_An_Prepend(DATranslationManager::Translate_With_Team_Name(Killer)),VictimName,DATranslationManager::Translate(Killer),DATranslationManager::Translate(Victim));
				}
				else if (Is_Smart_Bot(Killer)) {
					Message.Format("%d %ls destroyed %s (%s VS. %s)", Killer->Get_Player_Type(), ((VehicleGameObj*)Killer)->Get_Driver()->Get_Bot_Tag(), VictimName, DATranslationManager::Translate(Killer), DATranslationManager::Translate(Victim));
				}
				else {
					Message.Format("%d %s destroyed %s (%s VS. %s)", Killer->Get_Player_Type(), A_Or_An_Prepend(DATranslationManager::Translate(Killer)), VictimName, DATranslationManager::Translate(Killer), DATranslationManager::Translate(Victim));
				}
			}
			DALogManager::Write_Log("_VEHKILL","%s",Message);
		}
	}
	else if (((BuildingGameObj*)Victim)->As_RefineryGameObj()) { //Fix bug where the Nod Harvester can still exist if it is building when the Refinery is killed.
		for (SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();x;x = x->Next()) {
			if (x->Data()->Get_Definition().Get_ID() == (unsigned int)((RefineryGameObj*)Victim)->Get_Harvester_Def_ID()) {
				x->Data()->Set_Delete_Pending();
			}
		}
	}
}

bool DAVehicleManager::Vehicle_Flip_Event(VehicleGameObj *Vehicle) {
	return false;
}

void DAVehicleManager::Vehicle_Enter_Event(VehicleGameObj *Vehicle,cPlayer *Player,int Seat) {
	DAVehicleObserverClass *VehicleData = Get_Vehicle_Data(Vehicle);
	if (VehicleData->Get_Team() != -2 && VehicleData->Get_Team() != Player->Get_Player_Type()) {
		StringClass String;
		if (VehicleData->Get_Vehicle_Owner() && VehicleData->Get_Vehicle_Owner()->Is_Active()) {
			String.Format("%ls has stolen %ls %s!",Player->Get_Name(),Make_Possessive(VehicleData->Get_Vehicle_Owner()->Get_Name()),DATranslationManager::Translate(Vehicle));
		}
		else {
			String.Format("%ls has stolen %s!",Player->Get_Name(),a_or_an_Prepend(DATranslationManager::Translate(Vehicle)));
		}
		if (EnableTheftMessage) {
			DA::Color_Message_With_Team_Color(Player->Get_Player_Type(),"%s",String);
		}
		DALogManager::Write_Log("_VEHICLE","%d %s",Player->Get_Player_Type(),String);
		if (VehicleData->Get_Time_Since_Last_Theft() >= 10) { //Reward players for stealing vehicles. 10 second timer to prevent exploits.
			Player->Increment_Score(Vehicle->Get_Defense_Object()->Get_Death_Points());
		}
		VehicleData->Reset_Last_Theft_Time();
	}
	if (Seat == 0) {
		VehicleData->Set_Vehicle_Owner(Player);
	}
	VehicleData->Set_Team(Player->Get_Player_Type());
}

bool DAVehicleManager::Vehicle_Entry_Request_Event(VehicleGameObj *Vehicle,cPlayer *Player,int &Seat) {
	if (Seat == 0 && Player->Get_DA_Player()->Is_Stock_Client()) { //Prevent stock clients from entering Recon Bikes, otherwise they will trigger cheat warnings because of the changed weapon.
		if (!_stricmp(Vehicle->Get_Definition().Get_Name(),"CnC_Nod_Recon_Bike")) {
			return false;
		}
	}
	return true;
}



class DAAirDroppedVehicleScript : public ScriptImpClass {
	virtual void Created(GameObject *obj) {
		Commands->Enable_Vehicle_Transitions(obj,false);
		((VehicleGameObj*)obj)->Set_Is_Scripts_Visible(false);
		((VehicleGameObj*)obj)->Set_Collision_Group(UNCOLLIDEABLE_GROUP);
		((VehicleGameObj*)obj)->Get_Defense_Object()->Set_Can_Object_Die(false);
	}
};
ScriptRegistrant<DAAirDroppedVehicleScript> DAAirDroppedVehicleScriptRegistrant("DAAirDroppedVehicleScript","");



class DAVehicleChatCommandClass: public DAChatCommandClass { //This will get overloaded by the Vehicle Queue if it is enabled.
	bool Activate(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
		int Team = Player->Get_Player_Type();
		if (Team != 0 && Team != 1) {
			return true;
		}
		DA::Private_Color_Message(Player,COLORGRAY,"Limit: %d/%d",Get_Ground_Vehicle_Count(Team),Get_Vehicle_Limit());
		VehicleFactoryGameObj *VF = (VehicleFactoryGameObj*)BaseControllerClass::Find_Base(Team)->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY);
		if (VF && VF->Is_Busy() && VF->Get_Generating_Vehicle_ID()) {
			StringClass Name;
			if (VF->Get_Purchaser()) {
				if (VF->Get_Purchaser()->Get_Player()) {
					Name = VF->Get_Purchaser()->Get_Player()->Get_Name();
				}
				else if(Name = VF->Get_Purchaser()->Get_Bot_Tag()) {
					Name = VF->Get_Purchaser()->Get_Bot_Tag();
				}
				else {
					Name = A_Or_An_Prepend(DATranslationManager::Translate(VF->Get_Purchaser()));
				}
				Name += " - ";
			}
			else {
				RefineryGameObj *Ref = (RefineryGameObj*)BaseControllerClass::Find_Base(Team)->Find_Building(BuildingConstants::TYPE_REFINERY);
				if (Ref && !Ref->Get_Harvester_Vehicle() && Ref->Get_Harvester_Def_ID() == VF->Get_Generating_Vehicle_ID()) {
					Name = DATranslationManager::Translate(Ref);
					Name += " - ";
				}
			}
			DA::Private_Color_Message(Player,COLORGRAY,"Building: %s%s",Name,DATranslationManager::Translate(VF->Get_Generating_Vehicle_ID()));
		}
		return true;
	}
};
Register_Simple_Chat_Command(DAVehicleChatCommandClass,"!vq|!queue|!q|!veh|!vehicle|!vehlimit|!vehiclelimit|!vlimit|!limit");

class DAVKillsChatCommandClass: public DAChatCommandClass { 
	bool Activate(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
		if (Text.Size()) {
			cPlayer *MatchPlayer = Match_Player(Player,Text[1],false,true);
			if (MatchPlayer) {
				DA::Page_Player(Player,"%ls has destroyed %d enemy vehicles this match.",MatchPlayer->Get_Name(),MatchPlayer->VehiclesDestroyed);
			}
		}
		else {
			DA::Page_Player(Player,"You have destroyed %d enemy vehicles this match.",Player->VehiclesDestroyed);
		}
		return true;
	}
};
Register_Simple_Chat_Command(DAVKillsChatCommandClass,"!vkills|!vkill|!vehkills|!vehkill|!vehiclekills|!vehiclekill");
