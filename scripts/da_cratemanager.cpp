/*	Renegade Scripts.dll
    Dragonade Crate Manager
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
#include "da_crate.h"
#include "da_cratemanager.h"
#include "da_settings.h"
#include "da_player.h"
#include "da_log.h"
#include "da_game.h"
#include "da_gameobj.h"
#include "SpawnerClass.h"
#include "SpawnerDefClass.h"
#include "GameObjManager.h"

#pragma warning(disable: 4073)
#pragma init_seg(lib)

DynamicVectorClass <DACrateFactoryClass*> DACrateManager::Crates;
DynamicVectorClass <DACrateModifierFactoryClass*> DACrateManager::CrateModifiers;
float DACrateManager::Odds = 0;

void DACrateManager::Static_Init() {
	DASettingsManager::Add_Settings("da_crates.ini");
}

void DACrateManager::Add_Crate(DACrateFactoryClass *Factory) {
	for (int i = 0;i < Crates.Count();i++) {
		if (!_stricmp(Crates[i]->Get_Name(),Factory->Get_Name())) {
			Crates.Delete(i);
			break;
		}
	}
	Crates.Add(Factory);
}

void DACrateManager::Add_Crate_Modifier(DACrateModifierFactoryClass *Factory) {
	for (int i = 0;i < CrateModifiers.Count();i++) {
		if (!_stricmp(CrateModifiers[i]->Get_Name(),Factory->Get_Name())) {
			CrateModifiers.Delete(i);
			break;
		}
	}
	CrateModifiers.Add(Factory);
}

DACrateClass *DACrateManager::Get_Crate(const char *Name) {
	Iterate_Crates(Crate,i)
		if (!_stricmp(Crate->Get_Name(),Name)) {
			return Crate;
		}
	}
	return 0;
}

DACrateModifierClass *DACrateManager::Create_Crate_Modifier(const char *Name,const char *Parameters) {
	for (int i = 0;i < CrateModifiers.Count();i++) {
		if (!_stricmp(CrateModifiers[i]->Get_Name(),Name)) {
			return CrateModifiers[i]->Create(Parameters);
		}
	}
	return 0;
}

void DACrateManager::Calculate_Odds(cPlayer *Player) {
	Odds = 0;
	float BaseOdds = 0;
	float ModifiedOdds = 0;
	Iterate_Crates(Crate,i) //Apply modifiers.
		if (Crate->Check_Type(Player)) {
			Crate->Calculate_Odds(Player);
			BaseOdds += Crate->Get_Base_Odds();
			ModifiedOdds += Crate->Get_Modified_Odds();
		}
	}
	if (BaseOdds != ModifiedOdds) { //The odds difference from modified crates needs to be taken from crates with greater or equal base odds.
		Iterate_Crates(Crate,i) //If this is not done the total odds will increase which will lead to the odds of other crates being unitentionally affected.
			if (Crate->Check_Type(Player) && Crate->Get_Modified_Odds() != Crate->Get_Base_Odds()) { //This crate has modified odds.
				float OddsDifference = Crate->Get_Base_Odds()-Crate->Get_Modified_Odds(); //Get the difference between base odds and modified odds.
				float OddsGreater = 0;
				int OddsEqual = 0;
				if (OddsDifference > 0 && Crate->Get_Final_Odds() && !Crate->Get_Cap_Odds()) {
					OddsEqual--;
				}
				Iterate_Crates(Crate2,x) //Add up the base odds of all crates with greater or equal base odds and not disabled(0 modified odds).
					if (Crate2->Check_Type(Player) && Crate2->Get_Final_Odds() && (OddsDifference < 0 || !Crate2->Get_Cap_Odds())) {
						if (Crate2->Get_Base_Odds() > Crate->Get_Base_Odds()) { 
							OddsGreater += Crate2->Get_Final_Odds();
						}
						else if (Crate2->Get_Base_Odds() == Crate->Get_Base_Odds()) { 
							OddsEqual++;
						}
					}
				}
				Iterate_Crates(Crate2,x)
					if (Crate2->Check_Type(Player) && Crate2->Get_Final_Odds() && (OddsDifference < 0 || !Crate2->Get_Cap_Odds())) {
						if (Crate2->Get_Base_Odds() > Crate->Get_Base_Odds()) {
							float OddsBefore = Crate2->Get_Final_Odds();
							Crate2->Adjust_Odds(OddsBefore/OddsGreater*OddsDifference); //Modify odds as a percentage of this crate's odds relative to the odds of all crates that are being modified.
							OddsDifference += OddsBefore-Crate2->Get_Final_Odds(); //I.E Crates with higher odds get more odds taken/granted.
							OddsGreater -= OddsBefore;
						}
						else if (Crate2->Get_Base_Odds() == Crate->Get_Base_Odds() && Crate != Crate2) {
							float OddsBefore = Crate2->Get_Final_Odds();
							Crate2->Adjust_Odds(OddsDifference/OddsEqual);
							OddsDifference += OddsBefore-Crate2->Get_Final_Odds();
							OddsEqual--;
						}
					}
				}
				if (OddsDifference < 0) {
					Crate->Adjust_Odds(OddsDifference);
				}
			}
		}
		Iterate_Crates(Crate,i)
			if (Crate->Check_Type(Player)) {
				Odds += Crate->Get_Final_Odds();
			}
		}
	}
	else {
		Odds = BaseOdds;
	}
}

float DACrateManager::Get_Odds() {
	return Odds;
}

DACrateClass *DACrateManager::Select_Crate(cPlayer *Player) {
	Calculate_Odds(Player);
	float Rand = Get_Random_Float(1,Odds);
	float Total = 0;
	Iterate_Crates(Crate,i)
		if (Crate->Check_Type(Player)) {
			if (Crate->Get_Final_Odds() && Rand <= (Total+=Crate->Get_Final_Odds())) {
				return Crate;
			}
		}
	}
	return 0;
}

void DACrateManager::Init() {
	Register_Event(DAEvent::LEVELLOADED);
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Event(DAEvent::GAMEOVER);
	Register_Event(DAEvent::POWERUPGRANT);
	Register_Object_Event(DAObjectEvent::DESTROYED,DAObjectEvent::POWERUP);
	Register_Chat_Command((DAECC)&DACrateManager::Crate_Chat_Command,"!crate");
	Register_Chat_Command((DAECC)&DACrateManager::Crates_Chat_Command,"!crates");
	Register_Chat_Command((DAECC)&DACrateManager::ShowCrateSpawners_Chat_Command,"!showcratespawners",0,DAAccessLevel::ADMINISTRATOR);
	Register_Chat_Command((DAECC)&DACrateManager::HideCrateSpawners_Chat_Command,"!hidecratespawners",0,DAAccessLevel::ADMINISTRATOR);
}

DACrateManager::~DACrateManager() {
	for (int i = 0;i < Crates.Count();i++) {
		Crates[i]->Destroy_Instance();
	}
	if (!DAGameManager::Is_Shutdown_Pending()) {
		for (int i = 0;i < CrateObjs.Count();i++) {
			CrateObjs[i]->Set_Delete_Pending();
		}
	}
}

void DACrateManager::Settings_Loaded_Event() {
	//Basic settings
	MaxCrates = DASettingsManager::Get_Int("MaxCrates",1);
	DASettingsManager::Get_String(Model,"CrateModel","vehcol2m");
	StringClass SpawnTime;
	DASettingsManager::Get_String(SpawnTime,"CrateSpawnTime","120-180");
	DATokenParserClass Parser(SpawnTime,'-');
	if (!Parser.Get_Float(SpawnTimeMin) || !Parser.Get_Float(SpawnTimeMax)) {
		SpawnTimeMin = 120.0f;
		SpawnTimeMax = 180.0f;
	}
	DASettingsManager::Get_String(SpawnTime,"FirstCrateSpawnTime","60-90");
	Parser.Set(SpawnTime,'-');
	if (!Parser.Get_Float(FirstSpawnTimeMin) || !Parser.Get_Float(FirstSpawnTimeMax)) {
		FirstSpawnTimeMin = 60.0f;
		FirstSpawnTimeMax = 90.0f;
	}
	
	//Crates
	for (int i = 0;i < Crates.Count();i++) {
		if (Crates[i]->Get_Instance()) {
			if (!Crates[i]->Check_Enabled()) {
				Crates[i]->Destroy_Instance();
			}
			else {
				Crates[i]->Get_Instance()->Settings_Loaded();
			}
		}
		else if (Crates[i]->Check_Enabled()) {
			Crates[i]->Create_Instance()->Settings_Loaded();
		}
	}
	DynamicVectorClass<DACrateFactoryClass*> CratesCopy = Crates;
	Crates.Delete_All();
	for (int i = 0;i < CratesCopy.Count();i++) { //Order crates from highest base odds to lowest base odds.
		bool Added = false;
		if (CratesCopy[i]->Get_Instance()) {
			for (int x = 0;x < Crates.Count();x++) {
				if (Crates[x]->Get_Instance() && Crates[x]->Get_Instance()->Get_Base_Odds() < CratesCopy[i]->Get_Instance()->Get_Base_Odds()) {
					Crates.Insert(x,CratesCopy[i]);
					Added = true;
					break;
				}
			}
		}
		if (!Added) {
			Crates.Add(CratesCopy[i]);
		}
	}
	
	//Spawn locations
	ActiveSpawners.Delete_All();
	Spawners.Delete_All();
	LastCratePosition = Vector3(-1000.0f,-1000.0f,-1000.0f);

	if (DASettingsManager::Get_Bool("EnableNewCrateSpawners",true)) { //Get new crate spawners from da_crates.ini if enabled.
		for (int i = 1;;i++) {
			Vector3 Buffer;
			DASettingsManager::Get_Vector3(Buffer,StringFormat("Crate%d",i),Buffer);
			if (Buffer.X || Buffer.Y || Buffer.Z) {
				if (Spawners.ID(Buffer) == -1) {
					Spawners.Add(Buffer);
				}
			}
			else {
				break;
			}
		}
	}
	if (DASettingsManager::Get_Bool("EnableOldCrateSpawners",true) || !Spawners.Count()) { //Get old crate spawners from level if enabled or if no new spawners could be loaded.
		for (int i = 0;i < SpawnerList.Count();i++) {
			const SpawnerDefClass *Def = SpawnerList[i]->Get_Definition();
			if (Def->Get_Spawn_Definition_ID_List().Count() && stristr(Get_Definition_Name(Def->Get_Spawn_Definition_ID_List()[0]),"Crate")) {
				Vector3 Buffer;
				for (int x = 0;x < SpawnerList[i]->Get_Spawn_Point_List().Count();x++) { //Alternate positions.
					SpawnerList[i]->Get_Spawn_Point_List()[x].Get_Translation(&Buffer);
					if (Spawners.ID(Buffer) == -1 && (Buffer.X || Buffer.Y || Buffer.Z)) {
						Spawners.Add(Buffer);
					}
				}
				SpawnerList[i]->Get_TM().Get_Translation(&Buffer); //Default position.
				if (Spawners.ID(Buffer) == -1 && (Buffer.X || Buffer.Y || Buffer.Z)) {
					Spawners.Add(Buffer);
				}
				SpawnerList[i]->Enable(false); //Disable default crate spawner.
			}
		}
	}
	else {
		for (int i = 0;i < SpawnerList.Count();i++) {
			const SpawnerDefClass *Def = SpawnerList[i]->Get_Definition();
			if (Def->Get_Spawn_Definition_ID_List().Count() && stristr(Get_Definition_Name(Def->Get_Spawn_Definition_ID_List()[0]),"Crate")) {
				SpawnerList[i]->Enable(false); //Disable default crate spawner.
			}
		}
	}
	for (int i = 0;i < Spawners.Count();i++) {
		ActiveSpawners.Add(&Spawners[i]);
	}

	//Start spawn timer
	Stop_Timer(1);
	if (Spawners.Count()) {
		float Rand = Get_Random_Float(FirstSpawnTimeMin,FirstSpawnTimeMax);
		for (int i = 1;i <= (MaxCrates-CrateObjs.Count());i++) {
			Start_Timer(1,Rand*i);
		}
	}
}

void DACrateManager::Game_Over_Event() {
	Stop_Timer(1);
}

Vector3 *DACrateManager::Select_Spawner() {
	Vector3 *Spawner = 0;
	if (CrateObjs.Count()) {
		float MaxDistance = FLT_MIN;
		for (int i = 0;i < ActiveSpawners.Count();i++) { //Select active spawner farthest from any crate.
			float SpawnerMinDistance = FLT_MAX;
			for (int x = 0;x < CrateObjs.Count();x++) {
				float Distance = Commands->Get_Distance(CrateObjs[x]->Get_Position(),*ActiveSpawners[i]);
				if (Distance < SpawnerMinDistance) {
					SpawnerMinDistance = Distance;
				}
			}
			float Distance = Commands->Get_Distance(LastCratePosition,*ActiveSpawners[i]);
			if (Distance < SpawnerMinDistance) {
				SpawnerMinDistance = Distance;
			}
			if (SpawnerMinDistance > MaxDistance) {
				MaxDistance = SpawnerMinDistance;
				Spawner = ActiveSpawners[i];
			}
		}
	}
	else if (ActiveSpawners.Count()) { //No crates exist, so get a random spawner.
		int Rand = Get_Random_Int(0,ActiveSpawners.Count());
		Spawner = ActiveSpawners[Rand];
	}
	ActiveSpawners.DeleteObj(Spawner);
	if (!ActiveSpawners.Count()) { //All spawners have been used, refill active list.
		for (int i = 0;i < Spawners.Count();i++) {
			if (&Spawners[i] != Spawner) {
				bool Valid = true;
				for (int x = 0;x < CrateObjs.Count();x++) { //Don't use any spawner that is near an existing crate.
					if (Commands->Get_Distance(CrateObjs[x]->Get_Position(),Spawners[i]) < 10.0f) {
						Valid = false;
					}
				}
				if (Valid) {
					ActiveSpawners.Add(&Spawners[i]);
				}
			}
		}
	}
	return Spawner;
}

void DACrateManager::Timer_Expired(int Number,unsigned int Data) {
	if (The_Game()->Get_Current_Players() && The_Game()->Is_Gameplay_Permitted()) {
		Vector3 *Position = Select_Spawner();
		if (Position) {
			PhysicalGameObj *Crate = Create_Object("Soldier PowerUps",*Position);
			Commands->Set_Model(Crate,Model);
			CrateObjs.Add(Crate);
			return;
		}
	}
	Start_Timer(1,Get_Random_Float(SpawnTimeMin,SpawnTimeMax));
}

void DACrateManager::PowerUp_Grant_Event(cPlayer *Player,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) {
	if (PowerUpObj && CrateObjs.ID(PowerUpObj) != -1 && (Player->Get_Player_Type() == 0 || Player->Get_Player_Type() == 1)) {
		DACrateClass *Crate = DACrateManager::Select_Crate(Player);
		if (Crate) {
			SoldierGameObj *Soldier = Player->Get_GameObj();
			Soldier->Get_Position(&LastCratePosition);
			DALogManager::Write_Log("_CRATE","%ls picked up the %s Crate(%s).",Player->Get_Name(),Crate->Get_Name(),Soldier->Get_Vehicle()?"Vehicle":"Infantry");
			DALogManager::Write_GameLog("CRATE;%s;%s;%d;%s;%f;%f;%f;%f;%f;%f;%d",Crate->Get_Name(),Soldier->Get_Vehicle()?"Vehicle":"Infantry",Soldier->Get_ID(),Soldier->Get_Definition().Get_Name(),LastCratePosition.Y,LastCratePosition.X,LastCratePosition.Z,Soldier->Get_Facing(),Soldier->Get_Defense_Object()->Get_Health_Max(),Soldier->Get_Defense_Object()->Get_Shield_Strength_Max(),Soldier->Get_Player_Type());
			Crate->Activate(Player);
			PowerUpObj->Set_Delete_Pending();
			Start_Timer(1,Get_Random_Float(SpawnTimeMin,SpawnTimeMax));
			DA::Private_HUD_Message(Player,COLORGRAY, "%s Crate", Crate->Get_Name());
		}
	}
}

void DACrateManager::Object_Destroyed_Event(GameObject *obj) {
	CrateObjs.DeleteObj((PhysicalGameObj*)obj);
}

bool DACrateManager::Crate_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	if (!Text.Size()) {
		DACrateClass *Crate = Select_Crate(Player);
		if (Crate) {
			DA::Page_Player(Player,"You got the %s Crate with a %.2f%% chance!",Crate->Get_Name(),Crate->Get_Final_Odds()/Odds*100);
		}
		else {
			DA::Page_Player(Player,"A crate could not be selected.");
		}
	}
	else if (Player->Get_DA_Player()->Get_Access_Level() >= DAAccessLevel::ADMINISTRATOR && (Player->Get_Player_Type() == 0 || Player->Get_Player_Type() == 1)) {
		DACrateClass *Crate = Get_Crate(Text[0]);
		if (Crate) {
			if (!Crate->Check_Type(Player) || !Crate->Can_Activate(Player)) {
				DA::Page_Player(Player,"You cannot currently receive that crate.");
			}
			else {
				Crate->Activate(Player);
			}
		}
		else {
			DA::Page_Player(Player,"Crate could not be found.");
		}
	}
	return true;
}

bool DACrateManager::Crates_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	float InfantryOdds = 0;
	float VehicleOdds = 0;
	Iterate_Crates(Crate,i)
		if (Crate->Check_Type(DACrateType::INFANTRY)) {
			InfantryOdds += Crate->Get_Base_Odds();
		}
		if (Crate->Check_Type(DACrateType::VEHICLE)) {
			VehicleOdds += Crate->Get_Base_Odds();
		}
	}
	Calculate_Odds(Player);
	if (InfantryOdds) {
		DA::Page_Player(Player,"Base Infantry Odds:");
		StringClass Send;
		Iterate_Crates(Crate,i)
			if (Crate->Check_Type(DACrateType::INFANTRY)) {
				StringClass Buffer;
				Buffer.Format("%s(%.2f%%), ",Crates[i]->Get_Name(),Crate->Get_Base_Odds()/InfantryOdds*100);
				if (Send.Get_Length() + Buffer.Get_Length() > 220) {
					Send.TruncateRight(2);
					DA::Page_Player(Player,"%s",Send);
					Send = Buffer;
				}
				else {
					Send += Buffer;
				}
			}
		}
		if (!Send.Is_Empty()) {
			Send.TruncateRight(2);
			DA::Page_Player(Player,"%s",Send);
		}
	}
	if (VehicleOdds) {
		DA::Page_Player(Player,"Base Vehicle Odds:");
		StringClass Send;
		Iterate_Crates(Crate,i)
			if (Crate->Check_Type(DACrateType::VEHICLE)) {
				StringClass Buffer;
				Buffer.Format("%s(%.2f%%), ",Crates[i]->Get_Name(),Crate->Get_Base_Odds()/VehicleOdds*100);
				if (Send.Get_Length() + Buffer.Get_Length() > 220) {
					Send.TruncateRight(2);
					DA::Page_Player(Player,"%s",Send);
					Send = Buffer;
				}
				else {
					Send += Buffer;
				}
			}
		}
		if (!Send.Is_Empty()) {
			Send.TruncateRight(2);
			DA::Page_Player(Player,"%s",Send);
		}
	}
	if (Odds) {
		DA::Page_Player(Player,"Current Odds:");
		StringClass Send;
		Iterate_Crates(Crate,i)
			if (Crate->Check_Type(Player)) {
				StringClass Buffer;
				if (Crate->Get_Final_Odds()) {
					Buffer.Format("%s(%.2f%%), ",Crates[i]->Get_Name(),Crate->Get_Final_Odds()/Odds*100);
				}
				else {
					Buffer.Format("%s(0%%), ",Crates[i]->Get_Name());
				}
				if (Send.Get_Length() + Buffer.Get_Length() > 220) {
					Send.TruncateRight(2);
					DA::Page_Player(Player,"%s",Send);
					Send = Buffer;
				}
				else {
					Send += Buffer;
				}
			}
		}
		if (!Send.Is_Empty()) {
			Send.TruncateRight(2);
			DA::Page_Player(Player,"%s",Send);
		}
	}
	return true;
}

class DAShowCrateSpawnersObserverClass : public DAGameObjObserverClass {
public:
	virtual const char *Get_Name() { 
		return "DAShowCrateSpawnersObserverClass"; 
	}

	DAShowCrateSpawnersObserverClass(int Number) {
		this->Number = Number;
	}

	virtual void Poked(GameObject *obj,GameObject *Poker) {
		Vector3 Position;
		Get_Owner()->Get_Position(&Position);
		DA::Host_Message("Crate spawner #%d at %f %f %f",Number,Position.X,Position.Y,Position.Z);
	}
	
	int Number;
};

bool DACrateManager::ShowCrateSpawners_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	HideCrateSpawners_Chat_Command(Player,Text,ChatType);
	int i = 0;
	for (;i < Spawners.Count();i++) {
		PhysicalGameObj *Marker = Create_Object("Invisible_Object",Spawners[i]);
		Marker->Set_Collision_Group(TERRAIN_AND_BULLET_COLLISION_GROUP);
		Marker->Peek_Physical_Object()->Set_Immovable(true);
		Commands->Set_Model(Marker,"dsp_holo");
		Marker->Add_Observer(new DAShowCrateSpawnersObserverClass(i+1));
	}
	DA::Host_Message("Created markers for %d crate spawners.",i);
	return true;
}

bool DACrateManager::HideCrateSpawners_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	for (SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();x;x = x->Next()) {
		if (x->Data()->As_ScriptableGameObj() && ((ScriptableGameObj*)x->Data())->Find_Observer("DAShowCrateSpawnersObserverClass")) {
			x->Data()->Set_Delete_Pending();
		}
	}
	return true;
}

Register_Game_Feature(DACrateManager,"Crates","EnableCrates",0);

