/*	Renegade Scripts.dll
    Dragonade Infantry and Vehicle Crates
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
#include "da_event.h"
#include "da_gameobj.h"
#include "da_player.h"
#include "da_crate.h"
#include "da_translation.h"
#include "da_vehicle.h"
#include "GameObjManager.h"

/********************************************************************************************************************************/

class DAMoneyCrateClass : public DACrateClass {
	virtual void Activate(cPlayer *Player) {
		int Min = 200;
		int Max = 400;
		BuildingGameObj *PP = (BuildingGameObj*)Find_Power_Plant(Player->Get_Player_Type());
		BuildingGameObj *Ref = (BuildingGameObj*)Find_Refinery(Player->Get_Player_Type());
		if (PP && PP->Is_Destroyed()) {
			Min += 100;
			Max += 100;
		}
		if (Ref && Ref->Is_Destroyed()) {
			Min += 100;
			Max += 100;
		}
		int Amount = Get_Random_Int(Min,Max);
		Player->Set_Money(Player->Get_Money()+Amount);
		DA::Page_Player(Player,"You have received %d credits from the Money Crate.",Amount);
	}
};

Register_Crate(DAMoneyCrateClass,"Money",DACrateType::INFANTRY | DACrateType::VEHICLE);

/********************************************************************************************************************************/

class DADeathCrateClass : public DACrateClass {
	virtual void Activate(cPlayer *Player) {
		if (Player->Get_GameObj()->Get_Vehicle()) { //Kill character or vehicle.
			Commands->Apply_Damage(Player->Get_GameObj()->Get_Vehicle(),99999.0f,"None",0);
			Commands->Create_Explosion("Explosion_Mine_Remote_01",Player->Get_GameObj()->Get_Vehicle()->Get_Position(),0);
		}
		else {
			Commands->Apply_Damage(Player->Get_GameObj(),99999.0f,"None",0);
			Commands->Create_Explosion("Explosion_Mine_Remote_01",Player->Get_GameObj()->Get_Position(),0);
		}
		DA::Host_Message("Some poor %ls player just got pwned by the fearsome Death Crate!",Get_Wide_Team_Name(Player->Get_Player_Type()));
	}
};

Register_Crate(DADeathCrateClass,"Death",DACrateType::INFANTRY | DACrateType::VEHICLE);

/********************************************************************************************************************************/

class DAVeteranCrateClass : public DACrateClass {
	virtual void Activate(cPlayer *Player) {
		float amount = Get_Random_Float(1, 30);
		Commands->Send_Custom_Event(0,Player->Get_GameObj(),558223,*(int*)&amount,0);
		DA::Page_Player(Player, "You've been awarded %.2f veteran points by the Veteran Crate.", amount);
	}

	virtual bool Can_Activate(cPlayer* Player) {
		return !!Player->Get_DA_Player()->Find_Observer("DAVeterancyPlayerDataObserverClass");
	}
};

Register_Crate(DAVeteranCrateClass,"Veteran",DACrateType::INFANTRY | DACrateType::VEHICLE);

/********************************************************************************************************************************/

class DAHealthCrateClass : public DACrateClass {
	virtual void Activate(cPlayer *Player) {
		DefenseObjectClass *Defense = 0;
		if (Player->Get_GameObj()->Get_Vehicle()) { //Upgrade character or vehicle.
			Defense = Player->Get_GameObj()->Get_Vehicle()->Get_Defense_Object();
		}
		else {
			Defense = Player->Get_GameObj()->Get_Defense_Object();
		}
		Defense->Set_Health_Max(Defense->Get_Health_Max()*1.2f);
		Defense->Set_Health(Defense->Get_Health_Max());
		Defense->Set_Shield_Strength_Max(Defense->Get_Shield_Strength_Max()*1.2f);
		Defense->Set_Shield_Strength(Defense->Get_Shield_Strength_Max());
		DA::Page_Player(Player,"The Health Crate has increased your maximum health.");
	}
};

Register_Crate(DAHealthCrateClass,"Health",DACrateType::INFANTRY | DACrateType::VEHICLE);

/********************************************************************************************************************************/

static const char PowerOnSounds[2][2][50] = {
	{
		"M00EVAN_DSGN0007I1EVAN_snd.wav",
		"M00EVAG_DSGN0003I1EVAG_snd.wav"
	},
	{
		"M00EVAN_DSGN0006I1EVAN_snd.wav",
		"M00EVAG_DSGN0001I1EVAG_snd.wav"
	}
};

static const char PowerOffSounds[2][2][50] = {
	{
		"M00EVAN_DSGN0069I1EVAN_snd.wav",
		"M00EVAG_DSGN0065I1EVAG_snd.wav"
	},
	{
		"M00EVAN_DSGN0068I1EVAN_snd.wav",
		"M00EVAG_DSGN0064I1EVAG_snd.wav"
	}
};

class DAPowerCrateClass : public DACrateClass, public DAEventClass {
	virtual void Init() {
		DACrateClass::Init();
		Register_Event(DAEvent::GAMEOVER);
	}
	
	virtual bool Can_Activate(cPlayer *Player) { //Prevent crate from triggering if there are no base defenses or already in effect.
		bool DefAlive[2] = {false,false};
		for (SLNode<BuildingGameObj> *z = GameObjManager::BuildingGameObjList.Head();z;z = z->Next()) {
			if ((z->Data()->Get_Player_Type() == 0 || z->Data()->Get_Player_Type() == 1) && z->Data()->Get_Definition().Get_Type() == BuildingConstants::TYPE_BASE_DEFENSE && !z->Data()->Is_Destroyed() && !z->Data()->Find_Observer("JFW_Power_Off")) {
				DefAlive[z->Data()->Get_Player_Type()] = true;
			}
		}
		return (!Is_Timer(2) && (DefAlive[0] || DefAlive[1]));
	}

	virtual void Activate(cPlayer *Player) {
		Timer_Expired(1,Player->Get_Player_Type()?0:1); //Stagger each team so the sounds don't overlap.
		if (Is_Timer(2)) {
			Start_Timer(1,2.0f,false,Player->Get_Player_Type());
		}
		else {
			Timer_Expired(1,Player->Get_Player_Type());
		}
	}
	
	virtual void Game_Over_Event() {
		Clear_Timers();
	}

	virtual void Timer_Expired(int Number,unsigned int Team) {
		if (Number == 1) { //Start
			PowerState[Team] = BaseControllerClass::Find_Base(Team)->Is_Base_Powered();
			BuildingGameObj *Building = 0;
			int Count = 0;
			for (SLNode<BuildingGameObj> *z = GameObjManager::BuildingGameObjList.Head();z;z = z->Next()) {
				if (z->Data()->Get_Player_Type() == (int)Team && z->Data()->Get_Definition().Get_Type() == BuildingConstants::TYPE_BASE_DEFENSE && !z->Data()->Is_Destroyed() && !z->Data()->Find_Observer("JFW_Power_Off")) { //Don't affect defenses that have their own power logic.
					Building = z->Data();
					Commands->Set_Building_Power(Building,!PowerState[Team]);
					Count++;
				}
			}
			if (Count) {
				Start_Timer(2,Get_Random_Float(60.0f,90.0f),false,Team); //End
				if (PowerState[Team]) {
					StringClass Defenses;
					if (Count == 1) {
						Defenses.Format("The %s has",DATranslationManager::Translate(Building));
					}
					else {
						Defenses.Format("Defenses have");
					}
					DA::Host_Message("Power surge detected in the %ls base. %s temporarily lost power.",Get_Wide_Team_Name(Team),Defenses);
					Create_2D_WAV_Sound_Team(PowerOffSounds[Team][0],0);
					Create_2D_WAV_Sound_Team(PowerOffSounds[Team][1],1);
				}
				else {
					StringClass Defenses;
					if (Count == 1) {
						Defenses.Format("%s",DATranslationManager::Translate(Building));
					}
					else {
						Defenses.Format("defenses");
					}
					DA::Host_Message("Power surge detected in the %ls base. Power has been temporarily restored to the %s.",Get_Wide_Team_Name(Team),Defenses);
					Create_2D_WAV_Sound_Team(PowerOnSounds[Team][0],0);
					Create_2D_WAV_Sound_Team(PowerOnSounds[Team][1],1);
				}
			}
		}
		else if (Number == 2) { //End
			if (PowerState[Team] == BaseControllerClass::Find_Base(Team)->Is_Base_Powered()) { //Don't restore power state if the PP died or was revived.
				BuildingGameObj *Building = 0;
				int Count = 0;
				for (SLNode<BuildingGameObj> *z = GameObjManager::BuildingGameObjList.Head();z;z = z->Next()) {
					if (z->Data()->Get_Player_Type() == (int)Team && z->Data()->Get_Definition().Get_Type() == BuildingConstants::TYPE_BASE_DEFENSE && !z->Data()->Is_Destroyed() && !z->Data()->Find_Observer("JFW_Power_Off")) {
						Building = z->Data();
						Commands->Set_Building_Power(Building,PowerState[Team]);
						Count++;
					}
				}
				if (Count) {
					if (PowerState[Team]) {
						StringClass Defenses;
						if (Count == 1) {
							Defenses.Format("%s",DATranslationManager::Translate_With_Team_Name(Building));
						}
						else {
							Defenses.Format("%ls base defenses",Get_Wide_Team_Name(Team));
						}
						DA::Host_Message("Power has been restored to the %s.",Defenses);
						Create_2D_WAV_Sound_Team(PowerOnSounds[Team][0],0);
						Create_2D_WAV_Sound_Team(PowerOnSounds[Team][1],1);
					}
					else {
						StringClass Defenses;
						if (Count == 1) {
							Defenses.Format("%s has",DATranslationManager::Translate_With_Team_Name(Building));
						}
						else {
							Defenses.Format("%ls base defenses have",Get_Wide_Team_Name(Team));
						}
						DA::Host_Message("The %s lost power.",Defenses);
						Create_2D_WAV_Sound_Team(PowerOffSounds[Team][0],0);
						Create_2D_WAV_Sound_Team(PowerOffSounds[Team][1],1);
					}
				}
			}
		}
	}

	bool PowerState[2];
};

Register_Crate(DAPowerCrateClass,"Power",DACrateType::INFANTRY | DACrateType::VEHICLE);

/********************************************************************************************************************************/

class DARegenerationCrateObserverClass : public DAGameObjObserverClass {
	virtual const char *Get_Name() { 
		return "DARegenerationCrateObserverClass"; 
	}
	
	virtual void Init() {
		Total = 0;
		Start_Timer(2,1.0f); //10 every second.
	}

	virtual void Timer_Expired(GameObject *obj,int Number) {
		Start_Timer(2,1.0f);
		DefenseObjectClass *Defense = ((DamageableGameObj*)Get_Owner())->Get_Defense_Object();
		if (Defense->Get_Health_Max() == Defense->Get_Health()) {
			if (Defense->Get_Shield_Strength_Max() != Defense->Get_Shield_Strength()) {
				float Before = Defense->Get_Shield_Strength();
				Defense->Add_Shield_Strength(10.0f);
				Total += Defense->Get_Shield_Strength()-Before;
			}
			else {
				return;
			}
		}
		else {
			float Before = Defense->Get_Health();
			Defense->Add_Health(10.0f);
			Total += Defense->Get_Health()-Before;
		}
		if (Total >= (Defense->Get_Health_Max()+Defense->Get_Shield_Strength_Max())) {
			Set_Delete_Pending(); //Regen a maximum of their maximum health and armor combined.
		}
	}

	virtual void Kill_Received(ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
		Set_Delete_Pending(); //Turn off regen when dead.
	}

	float Total;
};

class DARegenerationCrateClass : public DACrateClass {
	virtual bool Can_Activate(cPlayer *Player) { //Prevent crate from triggering if already in effect.
		return !(Player->Get_GameObj()->Get_Vehicle()?Player->Get_GameObj()->Get_Vehicle()->Find_Observer("DARegenerationCrateObserverClass"):Player->Get_GameObj()->Find_Observer("DARegenerationCrateObserverClass"));
	}

	virtual void Activate(cPlayer *Player) {
		if (Player->Get_GameObj()->Get_Vehicle()) {
			DA::Page_Player(Player,"You picked up the Regeneration Crate. Your vehicle's health will temporarily regenerate.");
			Player->Get_GameObj()->Get_Vehicle()->Add_Observer(new DARegenerationCrateObserverClass);
		}
		else {
			DA::Page_Player(Player,"You picked up the Regeneration Crate. Your health will temporarily regenerate.");
			Player->Get_GameObj()->Add_Observer(new DARegenerationCrateObserverClass);
		}
	}
};

Register_Crate(DARegenerationCrateClass,"Regeneration",DACrateType::INFANTRY | DACrateType::VEHICLE);

/********************************************************************************************************************************/

class DAIonStormCrateClass : public DACrateClass, public DAEventClass {
	virtual bool Can_Activate(cPlayer *Player) { //Prevent crate from triggering if already in effect.
		return !(Is_Timer(1) || Is_Timer(2) || Is_Timer(3));
	}

	virtual void Activate(cPlayer *Player) {
		Clear_Timers();
		Start_Timer(1,Get_Random_Float(23.0f,53.0f)); //End
		Timer_Expired(102,0); //"Static" the HUD.
		Start_Timer(201,Get_Random_Float(3.0f,5.0f)); //Disable HUD.
		Register_Event(DAEvent::PLAYERJOIN);
		Register_Event(DAEvent::GAMEOVER);
		Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::PLAYER);
		Commands->Set_Clouds(1.0f,1.0f,10.0f); //Set weather.
		Commands->Set_Wind(0.0f,1.0f,0.0f,10.0f);
		Commands->Set_Lightning(1.0f,0.0f,1.0f,0.0f,1.0f,10.0f);
		Commands->Set_Fog_Enable(true);
		Commands->Set_Fog_Range(75.0f,100.0f,10.0f);
		Send_Message(COLORWHITE,"Detecting interference from approaching Ion Storm.");
	}

	virtual void Timer_Expired(int Number,unsigned int Data) {
		if (Number == 1) {
			Send_Message(COLORWHITE,"Searching...");
			Commands->Set_Clouds(0.0f,0.0f,7.0f);
			Commands->Set_Wind(0.0f,0.0f,0.0f,7.0f);
			Commands->Set_Lightning(0.0f,0.0f,0.0f,0.0f,0.0f,7.0f);
			Commands->Set_Fog_Range(1000.0f,1000.0f,7.0f);
			Start_Timer(2,5.0f);
			Start_Timer(4,10.0f);
		}
		else if (Number == 2) {
			Create_2D_WAV_Sound_Team("M00EVAN_DSGN0027I1EVAN_snd.wav",0);
			Create_2D_WAV_Sound_Team("M00EVAG_DSGN0031I1EVAG_snd.wav",1);
			Send_Message(COLORWHITE,"Uplink established. Please standby.");
			Start_Timer(3,2.0f);
		}
		else if (Number == 3) {
			Send_Message(COLORWHITE,"Re-initializing HUD sub-system.");
			Enable_HUD(true);
			Unregister_Event(DAEvent::PLAYERJOIN);
			Unregister_Event(DAEvent::GAMEOVER);
			Unregister_Object_Event(DAObjectEvent::CREATED);
			Commands->Set_Fog_Enable(false);
		}
		else if (Number == 101) {
			Enable_HUD(true);
			Start_Timer(102,Get_Random_Float(0.25f,0.5f));
		}
		else if (Number == 102) {
			Enable_HUD(false);
			Start_Timer(101,Get_Random_Float(0.5f,1.0f));
		}
		else if (Number == 201) {
			Create_2D_WAV_Sound_Team("M00EVAN_DSGN0031I1EVAN_snd.wav",0);
			Create_2D_WAV_Sound_Team("M00EVAG_DSGN0035I1EVAG_snd.wav",1);
			Send_Message(COLORWHITE,"Uplink signal to global EVANet lost. Searching...");
			Enable_HUD(false);
			Stop_Timer(101);
			Stop_Timer(102);
			Start_Timer(202,5.0f);
		}
		else if (Number == 202) {
			Create_2D_WAV_Sound_Team("M00EVAN_DSGN0028I1EVAN_snd.wav",0);
			Create_2D_WAV_Sound_Team("M00EVAG_DSGN0032I1EVAG_snd.wav",1);
			Send_Message(COLORWHITE,"Unable to establish uplink.");
		}
	}

	virtual void Player_Join_Event(cPlayer *Player) { //Disable HUD for players that joined after the start.
		Send_Message_Player_By_ID(Player->Get_Id(),COLORWHITE,"Detecting interference from approaching Ion Storm.");
		Send_Message_Player_By_ID(Player->Get_Id(),COLORWHITE,"Uplink signal to global EVANet lost. Searching...");
		Enable_HUD_Player_By_ID(Player->Get_Id(),false);
		Set_Fog_Enable_Player_By_ID(Player->Get_Id(),true);
		Set_Fog_Range_Player_By_ID(Player->Get_Id(),75.0f,100.0f,10.0f);
	}

	virtual void Object_Created_Event(GameObject *obj) {
		Enable_HUD_Player_By_ID(((SoldierGameObj*)obj)->Get_Player()->Get_Id(),false);
	}
	
	virtual void Game_Over_Event() { //Don't want stuff to carry over to the next game.
		Clear_Timers();
		Start_Timer(3,0.0f);
	}
};

Register_Crate(DAIonStormCrateClass,"Ion Storm",DACrateType::INFANTRY | DACrateType::VEHICLE);

/********************************************************************************************************************************/

class DASpyCratePassengerObserverClass : public DAPlayerObserverClass {
public:
	virtual const char *Get_Observer_Name() { 
		return "DASpyCratePassengerObserverClass"; 
	}

	virtual void Init() {
		Vehicle = Get_Owner()->Get_GameObj()->Get_Vehicle();
	}

	virtual void Kill_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
		if (Victim->As_BuildingGameObj()) {
			if (Vehicle) {
				Vehicle->Remove_Observer("DASpyCrateVehicleObserverClass");
			}
			Set_Delete_Pending();
		}
	}

	virtual void Destroyed() {
		Start_Timer(1,60.0f); //Enough time for any beacon or C4 planted to go off.
	}
	
	virtual void Timer_Expired(int Number,unsigned int Data) {
		Set_Delete_Pending();
	}
	
	ReferencerClass Vehicle;
};

class DASpyCrateVehicleObserverClass : public DAGameObjObserverClass {
	virtual const char *Get_Name() { 
		return "DASpyCrateVehicleObserverClass"; 
	}

	virtual void Init() {
		VehicleGameObj *Vehicle = ((VehicleGameObj*)Get_Owner());
		Vehicle->Set_Is_Scripts_Visible(false);
		for (int i = 0;i < Vehicle->Get_Definition().Get_Seat_Count();i++) {
			if (Vehicle->Get_Occupant(i)) {
				Vehicle->Get_Occupant(i)->Get_Player()->Get_DA_Player()->Add_Observer(new DASpyCratePassengerObserverClass);
			}
		}
	}

	virtual void Vehicle_Enter(cPlayer *Player,int Seat) {
		const DynamicVectorClass<DAPlayerObserverClass*> &Observers = Player->Get_DA_Player()->Get_Observers();
		for (int i = 0;i < Observers.Count();i++) {
			if (!_stricmp(Observers[i]->Get_Observer_Name(),"DASpyCratePassengerObserverClass")) {
				if (((DASpyCratePassengerObserverClass*)Observers[i])->Vehicle == Get_Owner()) {
					return;
				}
			}
		}
		Player->Get_DA_Player()->Add_Observer(new DASpyCratePassengerObserverClass);
	}

	~DASpyCrateVehicleObserverClass() {
		((VehicleGameObj*)Get_Owner())->Set_Is_Scripts_Visible(true);
	}
};

class DASpyCrateSoldierObserverClass : public DAGameObjObserverClass {
	virtual const char *Get_Name() { 
		return "DASpyCrateSoldierObserverClass"; 
	}

	virtual void Init() {
		((SoldierGameObj*)Get_Owner())->Set_Is_Visible(false);
	}

	virtual void Kill_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
		if (Victim->As_BuildingGameObj()) {
			Set_Delete_Pending();
		}
	}

	~DASpyCrateSoldierObserverClass() {
		((SoldierGameObj*)Get_Owner())->Set_Is_Visible(true);
	}
};

class DASpyCrateClass : public DACrateClass {
	virtual bool Can_Activate(cPlayer *Player) { //Prevent crate from triggering if enemy base defense is dead or powered down.
		BuildingGameObj *Defense = (BuildingGameObj*)Find_Base_Defense(!Player->Get_Player_Type());
		return (Defense && !Defense->Is_Destroyed() && Defense->Is_Power_Enabled() && (Player->Get_GameObj()->Get_Vehicle()?Player->Get_GameObj()->Get_Vehicle()->Get_Is_Scripts_Visible():Player->Get_GameObj()->Is_Visible()));
	}
	
	virtual void Activate(cPlayer *Player) {
		DA::Host_Message("A %ls player just got the Spy Crate. Better watch your base %ls!",Get_Wide_Team_Name(Player->Get_Player_Type()),Get_Wide_Team_Name(!Player->Get_Player_Type()));
		SoldierGameObj *Soldier = Player->Get_GameObj();
		if (Soldier->Get_Vehicle()) {
			Soldier->Get_Vehicle()->Add_Observer(new DASpyCrateVehicleObserverClass);
			DA::Page_Player(Player,"Base defenses will ignore your current vehicle until one of its passengers destroys an enemy building.");
		}
		else {
			SoldierGameObjDef *SBH = (SoldierGameObjDef*)Find_Named_Definition("CnC_Nod_FlameThrower_2SF");
			if (SBH) {
				Soldier->Re_Init(*SBH);
				Soldier->Post_Re_Init();
			}
			Soldier->Add_Observer(new DASpyCrateSoldierObserverClass);
			DA::Page_Player(Player,"Base defenses will ignore your current character until you destroy an enemy building.");
		}
	}
};

Register_Crate(DASpyCrateClass,"Spy",DACrateType::INFANTRY | DACrateType::VEHICLE);

/********************************************************************************************************************************/

static const char *DAUAVCrateObserverName = "DAUAVCrateObserverClass";

class DAUAVCrateObserverClass : public DAGameObjObserverClass {
public:
	virtual const char *Get_Name() { 
		return DAUAVCrateObserverName; 
	}
	
	DAUAVCrateObserverClass(int Team) {
		Radar = Create_Object("Invisible_Object",Vector3());
		((PhysicalGameObj*)Radar.Get_Ptr())->Set_Player_Type(Team);
	}

	virtual void Init() {
		Commands->Attach_To_Object_Bone(Radar,Get_Owner(),"Origin");
		if (Get_Owner()->As_SmartGameObj()) {
			Start_Timer(1,0.0f);
		}
		else {
			Set_Radar_Color();
			((PhysicalGameObj*)Radar.Get_Ptr())->Set_Radar_Blip_Shape_Type(((PhysicalGameObj*)Get_Owner())->Get_Radar_Blip_Shape_Type());
		}
	}

	void Set_Radar_Color() {
		if (((PhysicalGameObj*)Get_Owner())->Get_Player_Type() < 0) {
			Commands->Set_Obj_Radar_Blip_Color(Radar,2);
		}
		else {
			Commands->Set_Obj_Radar_Blip_Color(Radar,((PhysicalGameObj*)Get_Owner())->Get_Player_Type());
		}
	}

	virtual void Timer_Expired(GameObject *obj,int Number) {
		if (!Radar) {
			Set_Delete_Pending();
		}
		if (Number == 1) {
			if (Get_Owner()->As_VehicleGameObj() || ((PhysicalGameObj*)Radar.Get_Ptr())->Get_Player_Type() != ((PhysicalGameObj*)Get_Owner())->Get_Player_Type()) { //Vehicle or different team.
				Set_Radar_Color();
				Timer_Expired(obj,2);
			}
			else { //Same team, radar marker not needed.
				Set_Delete_Pending();
			}
		}
		else {
			if (((SmartGameObj*)Get_Owner())->Is_Stealthed()) {
				if (((PhysicalGameObj*)Radar.Get_Ptr())->Get_Radar_Blip_Shape_Type() != RADAR_BLIP_SHAPE_NONE) {
					((PhysicalGameObj*)Radar.Get_Ptr())->Set_Radar_Blip_Shape_Type(RADAR_BLIP_SHAPE_NONE);
				}
			}
			else if (((PhysicalGameObj*)Radar.Get_Ptr())->Get_Radar_Blip_Shape_Type() == RADAR_BLIP_SHAPE_NONE) {
				((PhysicalGameObj*)Radar.Get_Ptr())->Set_Radar_Blip_Shape_Type(((PhysicalGameObj*)Get_Owner())->Get_Radar_Blip_Shape_Type());
			}
			Start_Timer(2,0.5f);
		}
	}
	
	virtual void Vehicle_Enter(cPlayer *Player,int Seat) { //Change marker when player enters vehicle.
		Set_Radar_Color();
	}

	virtual void Vehicle_Exit(cPlayer *Player,int Seat) { //Change marker when player exits vehicle.
		Set_Radar_Color();
	}

	virtual void Kill_Received(ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
		Set_Delete_Pending();
	}
	
	virtual ~DAUAVCrateObserverClass() { //Destroy radar object.
		if (Radar) {
			DAGameObjManager::Set_GameObj_Delete_Pending(Radar);
		}
	}

	int Get_Team() {
		if (Radar) {
			return ((PhysicalGameObj*)Radar.Get_Ptr())->Get_Player_Type();
		}
		else {
			Set_Delete_Pending();
			return 2;
		}
	}

private:
	ReferencerClass Radar;
};

class DAUAVCrateClass : public DACrateClass, public DAEventClass {
	virtual void Init() {
		DACrateClass::Init();
		Register_Event(DAEvent::GAMEOVER);
		Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::SOLDIER | DAObjectEvent::VEHICLE | DAObjectEvent::BEACON);
		Active[0] = false;
		Active[1] = false;
	}

	virtual bool Can_Activate(cPlayer *Player) { //Don't trigger if already active or radar is disabled.
		return (!Active[Player->Get_Player_Type()] && The_Game()->Get_Radar_Mode() == 1);
	}

	virtual void Activate(cPlayer *Player) {
		int Team = Player->Get_Player_Type();
		Active[Team] = true;
		DA::Host_Message("A %ls player just picked up the UAV Crate. All enemy units are now marked on radar.",Get_Wide_Team_Name(Team));
		Start_Timer(1,Get_Random_Float(60.0f,90.0f),false,Team); //End timer.
		for (SLNode<SoldierGameObj> *x = GameObjManager::SoldierGameObjList.Head();x;x = x->Next()) { //Mark all enemy infantry.
			if (x->Data()->Get_Player_Type() != Team) {
				x->Data()->Add_Observer(new DAUAVCrateObserverClass(Team));
			}
		}
		for (SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();x;x = x->Next()) { //Mark all vehicles.
			x->Data()->Add_Observer(new DAUAVCrateObserverClass(Team));
		}
		for (SLNode<BeaconGameObj> *x = GameObjManager::BeaconGameObjList.Head();x;x = x->Next()) { //Mark all enemy beacons.
			if (x->Data()->Get_Player_Type() != Team) {
				x->Data()->Add_Observer(new DAUAVCrateObserverClass(Team));
			}
		}
		BaseControllerClass::Find_Base(Team)->Enable_Radar(true); //Re-enable radar if comm. center is dead.
	}
	
	virtual void Object_Created_Event(GameObject *obj) { //Observer must be attached to all newly created objects. It will delete itself if the object turns out to be friendly.
		if (Active[0]) {
			obj->Add_Observer(new DAUAVCrateObserverClass(0));
		}
		if (Active[1]) {
			obj->Add_Observer(new DAUAVCrateObserverClass(1));
		}
	}

	virtual void Game_Over_Event() {
		Clear_Timers();
		Timer_Expired(1,0);
		Timer_Expired(1,1);
	}
	
	virtual void Timer_Expired(int Number,unsigned int Team) {
		for (SLNode<SmartGameObj> *x = GameObjManager::SmartGameObjList.Head();x;x = x->Next()) { //Destroy observers for this team.
			const SimpleDynVecClass<GameObjObserverClass*> &Observers = x->Data()->Get_Observers();
			for (int i = 0;i < Observers.Count();i++) {
				if (Observers[i]->Get_Name() == DAUAVCrateObserverName && ((DAUAVCrateObserverClass*)Observers[i])->Get_Team() == (int)Team) {
					((DAUAVCrateObserverClass*)Observers[i])->Set_Delete_Pending();
				}
			}
		}
		for (SLNode<BeaconGameObj> *x = GameObjManager::BeaconGameObjList.Head();x;x = x->Next()) {
			const SimpleDynVecClass<GameObjObserverClass*> &Observers = x->Data()->Get_Observers();
			for (int i = 0;i < Observers.Count();i++) {
				if (Observers[i]->Get_Name() == DAUAVCrateObserverName && ((DAUAVCrateObserverClass*)Observers[i])->Get_Team() == (int)Team) {
					((DAUAVCrateObserverClass*)Observers[i])->Set_Delete_Pending();
				}
			}
		}
		Active[Team] = false;
		BaseControllerClass::Find_Base(Team)->Check_Radar(); //Disable radar if comm. center is dead.
	}

	bool Active[2];
};

Register_Crate(DAUAVCrateClass,"UAV",DACrateType::INFANTRY | DACrateType::VEHICLE);

/********************************************************************************************************************************/

class DAKamikazeCrateObserverClass : public DAGameObjObserverClass {
	virtual const char *Get_Name() { 
		return "DAKamikazeCrateObserverClass"; 
	}

	virtual void Init() {
		Killed = false;
	}

	virtual void Kill_Received(ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
		if (Get_Owner()->As_VehicleGameObj()) {
			Killed = true;
		}
		else {
			Vector3 Position;
			Get_Owner()->Get_Position(&Position);
			GameObject *ExplosionObjSave = GetExplosionObj();
			SetExplosionObj(Create_Object("Beacon_Ion_Cannon_Anim_Post",Position));
			Damage_All_Objects_Area(350.0f,"Laser_NoBuilding",Position,15.0f,Get_Owner(),Get_Owner());
			SetExplosionObj(ExplosionObjSave);
		}
	}
	
	virtual void Destroyed(GameObject *obj) { //This ensures the explosion will happen after the vehicle shell is created and destroy it.
		if (Killed) {
			Vector3 Position;
			Get_Owner()->Get_Position(&Position);
			SetExplosionObj(Create_Object("Beacon_Ion_Cannon_Anim_Post",Position));
			cPlayer *Owner = DAVehicleManager::Get_Vehicle_Owner(Get_Owner()); //Attribute damage to the vehicle's owner.
			Damage_All_Objects_Area(350.0f,"Laser_NoBuilding",Position,15.0f,Get_Owner(),Owner?Owner->Get_GameObj():0);
			SetExplosionObj(0);
		}
	}

private:
	bool Killed;
};

class DAKamikazeCrateClass : public DACrateClass {
	virtual bool Can_Activate(cPlayer *Player) { //Prevent crate from triggering if already in effect.
		return !(Player->Get_GameObj()->Get_Vehicle()?Player->Get_GameObj()->Get_Vehicle()->Find_Observer("DAKamikazeCrateObserverClass"):Player->Get_GameObj()->Find_Observer("DAKamikazeCrateObserverClass"));
	}

	virtual void Activate(cPlayer *Player) {
		DA::Host_Message("Some crazy ass %ls player just picked up the Kamikaze Crate. Watch yourself %ls.",Get_Wide_Team_Name(Player->Get_Player_Type()),Get_Wide_Team_Name(!Player->Get_Player_Type()));
		if (Player->Get_GameObj()->Get_Vehicle()) {
			DA::Page_Player(Player,"You picked up the Kamikaze Crate. Your vehicle will explode when it dies, damaging nearby enemy units.");
			Player->Get_GameObj()->Get_Vehicle()->Add_Observer(new DAKamikazeCrateObserverClass);
		}
		else {
			DA::Page_Player(Player,"You picked up the Kamikaze Crate. You will explode when you die, damaging nearby enemy units.");
			Player->Get_GameObj()->Add_Observer(new DAKamikazeCrateObserverClass);
		}
	}
};

Register_Crate(DAKamikazeCrateClass,"Kamikaze",DACrateType::INFANTRY | DACrateType::VEHICLE);

/********************************************************************************************************************************/

class DASecondWindCrateObserverClass : public DAGameObjObserverClass {
	virtual const char *Get_Name() { 
		return "DASecondWindCrateObserverClass"; 
	}

	virtual void Damage_Received(ArmedGameObj *Damager,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
		DefenseObjectClass *Defense = ((SoldierGameObj*)Get_Owner())->Get_Defense_Object();
		if (Defense->Get_Health() <= 0) {
			Reverse_Damage(Get_Owner(),(Defense->Get_Shield_Strength_Max()+Defense->Get_Health_Max())/2.0f);
			SoldierGameObj *Soldier = Get_Owner()->As_SoldierGameObj();
			if (Get_Owner()->As_VehicleGameObj()) {
				VehicleGameObj *Vehicle = (VehicleGameObj*)Get_Owner();
				for (int i = 0;i < Vehicle->Get_Definition().Get_Seat_Count();i++) {
					if (Vehicle->Get_Occupant(i)) {
						Soldier = Vehicle->Get_Occupant(i);
						break;
					}
				}
			}
			if (Soldier && Soldier->Get_Player()) {
				int ID = Soldier->Get_Player()->Get_Id();
				if (Soldier->Get_Player_Type() == 0) {
					Create_2D_WAV_Sound_Player_By_ID(ID,"M00EVAN_DSGN0022I1EVAN_SND.wav");
				}
				else if (Soldier->Get_Player_Type() == 1) {
					Create_2D_WAV_Sound_Player_By_ID(ID,"M00EVAG_DSGN0026I1EVAG_SND.wav");
				}
				Set_Emote_Icon(ID,"o_em_cross.w3d",2);
			}
			Set_Delete_Pending();
		}
	}
};

class DASecondWindCrateClass : public DACrateClass {
	virtual bool Can_Activate(cPlayer *Player) { //Prevent crate from triggering if already in effect.
		return !(Player->Get_GameObj()->Get_Vehicle()?Player->Get_GameObj()->Get_Vehicle()->Find_Observer("DASecondWindCrateObserverClass"):Player->Get_GameObj()->Find_Observer("DASecondWindCrateObserverClass"));
	}

	virtual void Activate(cPlayer *Player) {
		if (Player->Get_GameObj()->Get_Vehicle()) {
			Player->Get_GameObj()->Get_Vehicle()->Add_Observer(new DASecondWindCrateObserverClass);
			DA::Page_Player(Player,"You picked up the Second Wind Crate. Your vehicle will be partially healed when near death.");
		}
		else {
			Player->Get_GameObj()->Add_Observer(new DASecondWindCrateObserverClass);
			DA::Page_Player(Player,"You picked up the Second Wind Crate. You will be partially healed when near death.");
		}
	}
};

Register_Crate(DASecondWindCrateClass,"Second Wind",DACrateType::INFANTRY | DACrateType::VEHICLE);

/********************************************************************************************************************************/

class DAStealthCrateClass : public DACrateClass {
	virtual bool Can_Activate(cPlayer *Player) { //Don't trigger if already stealth.
		if (Player->Get_GameObj()->Get_Vehicle()) {
			return !Player->Get_GameObj()->Get_Vehicle()->Is_Stealth_Enabled();
		}
		else {
			return !Player->Get_GameObj()->Is_Stealth_Enabled();
		}
	}
	
	virtual void Activate(cPlayer *Player) {
		if (Player->Get_GameObj()->Get_Vehicle()) {
			Commands->Enable_Stealth(Player->Get_GameObj()->Get_Vehicle(),true);
			DA::Page_Player(Player,"Your vehicle has been equipped with a Lazarus Shield generator by the Stealth Crate.");
		}
		else {
			Commands->Enable_Stealth(Player->Get_GameObj(),true);
			DA::Page_Player(Player,"You have been equipped with a Lazarus Shield suit by the Stealth Crate.");
		}
	}
};

Register_Crate(DAStealthCrateClass,"Stealth",DACrateType::INFANTRY | DACrateType::VEHICLE);

/********************************************************************************************************************************/

