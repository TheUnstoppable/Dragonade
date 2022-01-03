/*	Renegade Scripts.dll
    Dragonade Vehicle Ownership Game Feature
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
#include "da_vehicleownership.h"
#include "da_vehicle.h"
#include "da_translation.h"
#include "da_settings.h"
#include "da_gameobjobservers.h"
#include "da_game.h"
#include "da_player.h"
#include "GameObjManager.h"

const char *Get_Icon_Bone(GameObject *obj,int IconNum) {
	const char *Preset = obj->Get_Definition().Get_Name();
	if (IconNum == 1) {
		if (stristr(Preset,"Nod_APC")) return "barrel";
		else if (stristr(Preset,"Apache")) return "muzzleb0";
		else if (stristr(Preset,"Mammoth")) return "muzzleb0";
		return "muzzlea0";
	}
	else {
		if (stristr(Preset,"Flame")) return "muzzlea1";
		else if (stristr(Preset,"Apache")) return "muzzleb1";
		else if (stristr(Preset,"MRLS")) return "muzzlea1";
		else if (stristr(Preset,"Mammoth")) return "muzzleb1";
		else if (stristr(Preset,"Orca")) return "muzzlea1";
		else if (stristr(Preset,"Recon")) return "muzzlea1";
		return 0;
	}
}

static char Icons[4][20] = {
	"o_em_apc", //Bind
	"p_keycrd_red", //Lock
	"p_keycrd_red", //Sell
	"p_keycrd_grn" //Free
};

static const char *ObserverName = "DAVehicleOwnershipObserverClass";

DAVehicleOwnershipObserverClass::DAVehicleOwnershipObserverClass(VehicleGameObj *Vehicle,cPlayer *Owner) : DAGameObjObserverClass() {
	Vehicle->Add_Observer(this);
	VehicleOwner = Owner;
	if (!Vehicle->Is_Stealth_Enabled()) {
		Create_Icons();
	}
	Set_State(DAVehicleOwnershipState::BOUND);
}

DAVehicleOwnershipObserverClass::~DAVehicleOwnershipObserverClass() {
	Destroy_Icons();
}

void DAVehicleOwnershipObserverClass::Create_Icons() {
	const char *IconBone = Get_Icon_Bone(Get_Vehicle(),1);
	const char *IconBone2 = Get_Icon_Bone(Get_Vehicle(),2);
	if (IconBone && !Icon[0]) {
		Icon[0] = Create_Object("Invisible_Object",Vector3(0,0,0));
		((PhysicalGameObj*)Icon[0].Get_Ptr())->Attach_To_Object_Bone(Get_Vehicle(),IconBone);
	}
	if (IconBone2 && !Icon[1]) {
		Icon[1] = Create_Object("Invisible_Object",Vector3(0,0,0));
		((PhysicalGameObj*)Icon[1].Get_Ptr())->Attach_To_Object_Bone(Get_Vehicle(),IconBone2);
	}
}

void DAVehicleOwnershipObserverClass::Destroy_Icons() {
	if (Icon[0]) {
		DAGameObjManager::Set_GameObj_Delete_Pending(Icon[0]);
	}
	if (Icon[1]) {
		DAGameObjManager::Set_GameObj_Delete_Pending(Icon[1]);
	}
}

void DAVehicleOwnershipObserverClass::Lock() {
	Set_State(DAVehicleOwnershipState::LOCKED);
	SoldierGameObj *Occupant = Get_Vehicle()->Get_Occupant(0);
	if (Occupant && Occupant->Get_Player() != Get_Vehicle_Owner()) { //Kick the driver if they're not the owner.
		if (Exit_Vehicle(Occupant)) {
			DA::Page_Player(Occupant,"%ls has removed you from the vehicle.",Get_Vehicle_Owner()->Get_Name());
			int OwnerSeat = Get_Vehicle()->Find_Seat(Get_Vehicle_Owner()->Get_GameObj());
			if (OwnerSeat != -1 && Exit_Vehicle(Get_Vehicle_Owner()->Get_GameObj())) { //Move owner to driver's seat.
				Get_Vehicle()->Add_Occupant(Get_Vehicle_Owner()->Get_GameObj());
			}
		}
	}
}

void DAVehicleOwnershipObserverClass::Unlock() {
	Set_State(DAVehicleOwnershipState::BOUND);
	Stop_Timer(1);
}

void DAVehicleOwnershipObserverClass::Sell() {
	Set_State(DAVehicleOwnershipState::SELLING);
	int Seats = Get_Vehicle()->Get_Definition().Get_Seat_Count();
	for (int i = 0;i < Seats;i++) { //Remove all players when selling.
		if (Get_Vehicle()->Get_Occupant(i)) {
			Exit_Vehicle(Get_Vehicle()->Get_Occupant(i));
		}
	}
	Start_Timer(1,10.0f);
}

void DAVehicleOwnershipObserverClass::Free() {
	VehicleOwner = 0;
	if (Get_Vehicle()->Is_Stealth_Enabled()) { //Even stealth units get the free icon.
		Create_Icons();
	}
	Set_State(DAVehicleOwnershipState::FREE);
}

void DAVehicleOwnershipObserverClass::Unbind() {
	VehicleOwner = 0;
	Set_Delete_Pending();
}

void DAVehicleOwnershipObserverClass::Update_Icons() {
	if (Icon[0]) {
		Commands->Set_Model(Icon[0],Icons[State]);
	}
	if (Icon[1]) {
		Commands->Set_Model(Icon[1],Icons[State]);
	}
}

void DAVehicleOwnershipObserverClass::Set_State(DAVehicleOwnershipState::State state) {
	State = state;
	Update_Icons();
}

void DAVehicleOwnershipObserverClass::Vehicle_Enter(cPlayer *Player,int Seat) {
	if (Seat == 0 && Get_Vehicle_Owner()) {
		if (Get_Team() == Player->Get_Player_Type()) {
			if (Get_Vehicle_Owner() != Player) {
				DA::Page_Player(Player, "This vehicle belongs to %ls.", Get_Vehicle_Owner()->Get_Name());
				DA::Page_Player(Get_Vehicle_Owner(), "%ls has entered your vehicle. You can remove them by using the \"!lock\" or \"!vkick\" command.", Player->Get_Name());
			}
			else if (Is_Selling()) {
				DA::Page_Player(Get_Vehicle_Owner(), "You have cancelled the sale of your vehicle.");
				Unlock();
			}
		}
		else {
			DA::Page_Player(Get_Vehicle_Owner(), "The enemy has stolen your vehicle!");
			Unbind();
		}
	}
}

bool DAVehicleOwnershipObserverClass::Vehicle_Entry_Request(cPlayer *Player,int &Seat) {
	if (Get_Team() == Player->Get_Player_Type()) {
		if (Is_Selling()) { //Other players are never allowed in sold vehicles.
			if (Get_Vehicle_Owner() != Player) {
				DA::Page_Player(Player,"This vehicle is waiting to be sold. You may not enter it.");
				return false;
			}
		}
		else if (Seat == 0) {
			if (Is_Locked() && Get_Vehicle_Owner() != Player) {
				int NewSeat = Find_Empty_Vehicle_Seat(Get_Vehicle(),false); //Try to find a passenger seat to put them in.
				if (NewSeat == -1) {
					DA::Page_Player(Player,"This vehicle is locked and belongs to %ls. You may not enter it.",Get_Vehicle_Owner()->Get_Name());
					return false; //If there isn't one then block the entry.
				}
				else {
					DA::Page_Player(Player,"This vehicle is locked and belongs to %ls. You have been moved to a passenger seat.",Get_Vehicle_Owner()->Get_Name());
					Seat = NewSeat; //If there is a free seat put them in it.
				}
			}
		}
		/*else if (Get_Vehicle_Owner() == Player) { //This allows the owner of a vehicle to get in the driver seat even if another player is already in it.
			int NewSeat = Find_Empty_Vehicle_Seat(Get_Vehicle(),false);
			if (NewSeat != -1) { //It only works on vehicles with an empty passenger seat, though. The client won't send the entry request if the vehicle is full.
				Get_Vehicle()->Set_Occupant(NewSeat,Get_Vehicle()->Get_Occupant(0)); //Move the current driver to the passenger seat.
				Get_Vehicle()->Set_Occupant(0,0);
				Seat = 0; //Set the owner to get in the driver seat.
			}
		}*/
	}
	return true;
}

bool DAVehicleOwnershipObserverClass::Damage_Received_Request(ArmedGameObj *Damager,float &Damage,unsigned int &Warhead,float Scale,DADamageType::Type Type) {
	if (Type != DADamageType::REPAIR) { //Block friendly players from damaging a locked vehicle.
		if ((Is_Locked() || Is_Selling()) && Is_Player(Damager) && Damager->Get_Player_Type() == Get_Team() && Get_Vehicle_Owner() != ((SoldierGameObj*)Damager)->Get_Player()) {
			return false;
		}
	}
	return true;
}

void DAVehicleOwnershipObserverClass::Destroyed(GameObject *obj) {
	if (!Is_Selling() && Get_Vehicle_Owner()) {
		DA::Page_Player(Get_Vehicle_Owner(),"Your vehicle has been destroyed.");
		VehicleOwner = 0;
	}
}

void DAVehicleOwnershipObserverClass::Timer_Expired(GameObject *obj,int Number) {
	float Cost = Round(Get_Cost(Get_Vehicle()->Get_Definition().Get_Name())*DAVehicleOwnershipGameFeature->Get_Sell_Percent());
	DA::Page_Player(Get_Vehicle_Owner(),"Your vehicle has been sold for %d credit(s).",(int)Cost);
	Get_Vehicle_Owner()->Increment_Money(Cost);
	Get_Vehicle()->Set_Delete_Pending();
}

const char *DAVehicleOwnershipObserverClass::Get_Name() {
	return ObserverName;
}

void DAVehicleOwnershipGameFeatureClass::Init() {
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Event(DAEvent::VEHICLEENTER);
	Register_Event(DAEvent::PLAYERLEAVE);
	Register_Event(DAEvent::TEAMCHANGE);
	Register_Object_Event(DAObjectEvent::DESTROYED,DAObjectEvent::PLAYER);
	Register_Object_Event(DAObjectEvent::KILLRECEIVED,DAObjectEvent::PLAYER);
	Register_Chat_Command((DAECC)&DAVehicleOwnershipGameFeatureClass::Bind_Chat_Command,"!bind|VehBind");
	Register_Chat_Command((DAECC)&DAVehicleOwnershipGameFeatureClass::Unbind_Chat_Command,"!unbind");
	Register_Chat_Command((DAECC)&DAVehicleOwnershipGameFeatureClass::Lock_Chat_Command,"!lock");
	Register_Chat_Command((DAECC)&DAVehicleOwnershipGameFeatureClass::Unlock_Chat_Command,"!unlock");
	Register_Chat_Command((DAECC)&DAVehicleOwnershipGameFeatureClass::BL_Chat_Command,"!bindlock|!bl|VehBL");
	Register_Chat_Command((DAECC)&DAVehicleOwnershipGameFeatureClass::VKick_Chat_Command,"!vkick|!vehkick|VehKick");
	Register_Chat_Command((DAECC)&DAVehicleOwnershipGameFeatureClass::VStatus_Chat_Command,"!vstatus|!vehstatus|VehStatus");
	Register_Chat_Command((DAECC)&DAVehicleOwnershipGameFeatureClass::Free_Chat_Command,"!free|!vfree");
	Register_Chat_Command((DAECC)&DAVehicleOwnershipGameFeatureClass::SellVeh_Chat_Command,"!vsell|!vehsell|!sellveh");
}

void DAVehicleOwnershipGameFeatureClass::Settings_Loaded_Event() {
	SellPercent = WWMath::Clamp(DASettingsManager::Get_Float("VehicleSellPercent",0.5f),0.0f,1.0f);
	Unregister_Chat_Command("!sellveh");
	if (SellPercent) {
		Register_Chat_Command((DAECC)&DAVehicleOwnershipGameFeatureClass::SellVeh_Chat_Command,"!sellveh|!vsell|!vehsell");
	}
}

DAVehicleOwnershipGameFeatureClass::~DAVehicleOwnershipGameFeatureClass() {
	//Unbind all vehicles if unloaded mid-game.
	if (!DAGameManager::Is_Shutdown_Pending()) {
		for (SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();x;x = x->Next()) {
			const SimpleDynVecClass<GameObjObserverClass*> &Observers = x->Data()->Get_Observers();
			for (int i = 0;i < Observers.Count();i++) {
				if (Observers[i]->Get_Name() == ObserverName) {
					((DAVehicleOwnershipObserverClass*)Observers[i])->Set_Delete_Pending();
				}
			}
		}
	}
}

DAVehicleOwnershipObserverClass *DAVehicleOwnershipGameFeatureClass::Bind_Vehicle(VehicleGameObj *Vehicle,cPlayer *Player) {
	if (!Vehicle->Peek_Physical_Object()->As_MoveablePhysClass()) {
		return 0;
	}
	DAVehicleOwnershipObserverClass *Data = Get_Vehicle_Data(Vehicle);
	if (Data) {
		if (Data->Get_Vehicle_Owner()) {
			return 0;
		}
		else { //Free vehicle
			Unbind_Vehicle(Vehicle);
		}
	}
	Unbind_Vehicle(Player); //Unbind the player's old vehicle if there is one.

	if (!Is_Script_Attached(Vehicle, "MS_AI_Marker_Vehicle_Search_Ignore")) { //Make bot script ignore this vehicle.
		StringClass scriptParams;
		scriptParams.Format("%d", Get_Object_Type(Vehicle));
		Commands->Attach_Script(Vehicle, "MS_AI_Marker_Vehicle_Search_Ignore", scriptParams.Peek_Buffer());
	}

	return new DAVehicleOwnershipObserverClass(Vehicle,Player);
}

bool DAVehicleOwnershipGameFeatureClass::Unbind_Vehicle(VehicleGameObj *Vehicle) {
	DAVehicleOwnershipObserverClass *Data = Get_Vehicle_Data(Vehicle);
	if (Data) {
		if (Is_Script_Attached(Vehicle, "MS_AI_Marker_Vehicle_Search_Ignore")) { //Remove the bot ignore script
			Find_Script_On_Object(Vehicle, "MS_AI_Marker_Vehicle_Search_Ignore")->Destroy_Script();
		}
		Data->Unbind();
		return true;
	}
	return false;
}

bool DAVehicleOwnershipGameFeatureClass::Unbind_Vehicle(cPlayer *Player) {
	DAVehicleOwnershipObserverClass *Data = Get_Vehicle_Data(Player);
	if (Data) {
		if (Is_Script_Attached(Data->Get_Vehicle(), "MS_AI_Marker_Vehicle_Search_Ignore")) { //Remove the bot ignore script
			Find_Script_On_Object(Data->Get_Vehicle(), "MS_AI_Marker_Vehicle_Search_Ignore")->Destroy_Script();
		}
		Data->Unbind();
		return true;
	}
	return false;
}

bool DAVehicleOwnershipGameFeatureClass::Lock_Vehicle(VehicleGameObj *Vehicle) {
	DAVehicleOwnershipObserverClass *Data = Get_Vehicle_Data(Vehicle);
	if (Data && !Data->Is_Locked() && !Data->Is_Selling()) {
		Data->Lock();
		return true;
	}
	return false;
}

bool DAVehicleOwnershipGameFeatureClass::Lock_Vehicle(cPlayer *Player) {
	DAVehicleOwnershipObserverClass *Data = Get_Vehicle_Data(Player);
	if (Data && !Data->Is_Locked() && !Data->Is_Selling()) {
		Data->Lock();
		return true;
	}
	return false;
}

bool DAVehicleOwnershipGameFeatureClass::Unlock_Vehicle(VehicleGameObj *Vehicle) {
	DAVehicleOwnershipObserverClass *Data = Get_Vehicle_Data(Vehicle);
	if (Data && (Data->Is_Locked() || Data->Is_Selling())) {
		Data->Unlock();
		return true;
	}
	return false;
}

bool DAVehicleOwnershipGameFeatureClass::Unlock_Vehicle(cPlayer *Player) {
	DAVehicleOwnershipObserverClass *Data = Get_Vehicle_Data(Player);
	if (Data && (Data->Is_Locked() || Data->Is_Selling())) {
		Data->Unlock();
		return true;
	}
	return false;
}

DAVehicleOwnershipObserverClass *DAVehicleOwnershipGameFeatureClass::Get_Vehicle_Data(GameObject *Vehicle) {
	const SimpleDynVecClass<GameObjObserverClass*> &Observers = Vehicle->Get_Observers();
	for (int i = 0;i < Observers.Count();i++) {
		if (Observers[i]->Get_Name() == ObserverName) {
			return (DAVehicleOwnershipObserverClass*)Observers[i];
		}
	}
	return 0;
}

DAVehicleOwnershipObserverClass *DAVehicleOwnershipGameFeatureClass::Get_Vehicle_Data(cPlayer *Player) {
	for (SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();x;x = x->Next()) {
		const SimpleDynVecClass<GameObjObserverClass*> &Observers = x->Data()->Get_Observers();
		for (int i = 0;i < Observers.Count();i++) {
			if (Observers[i]->Get_Name() == ObserverName) {
				if (((DAVehicleOwnershipObserverClass*)Observers[i])->Get_Vehicle_Owner() == Player) {
					return (DAVehicleOwnershipObserverClass*)Observers[i];
				}
			}
		}
	}
	return 0;
}

DAVehicleOwnershipObserverClass *DAVehicleOwnershipGameFeatureClass::Get_Vehicle_Data(SoldierGameObj *Soldier) {
	cPlayer *Player = Soldier->Get_Player();
	for (SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();x;x = x->Next()) {
		const SimpleDynVecClass<GameObjObserverClass*> &Observers = x->Data()->Get_Observers();
		for (int i = 0;i < Observers.Count();i++) {
			if (Observers[i]->Get_Name() == ObserverName) {
				if (((DAVehicleOwnershipObserverClass*)Observers[i])->Get_Vehicle_Owner() == Player) {
					return (DAVehicleOwnershipObserverClass*)Observers[i];
				}
			}
		}
	}
	return 0;
}

bool DAVehicleOwnershipGameFeatureClass::Bind_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	VehicleGameObj *Vehicle = Player->Get_GameObj()->Get_Vehicle();
	if (!Vehicle || !Vehicle->Peek_Physical_Object()->As_MoveablePhysClass()) {
		DA::Page_Player(Player,"You must be in a vehicle to use this command.");
	}
	else if (Vehicle->Get_Driver() != Player->Get_GameObj()) {
		DA::Page_Player(Player,"You must be the driver to use this command.");
	}
	else {
		DAVehicleOwnershipObserverClass *Data = Get_Vehicle_Data(Vehicle);
		if (Data && Data->Get_Vehicle_Owner()) {
			DA::Page_Player(Player,"This vehicle is already bound to %ls.",Data->Get_Vehicle_Owner()->Get_Name());
		}
		else {
			Bind_Vehicle(Vehicle,Player);
			DA::Page_Player(Player,"This vehicle has been bound to you. Use \"!lock\" to lock it or \"!unbind\"/\"!free\" to relinquish ownership.");
		}
	}
	return true;
}

bool DAVehicleOwnershipGameFeatureClass::Unbind_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	if (Unbind_Vehicle(Player)) {
		DA::Page_Player(Player,"Your vehicle has been unbound.");
	}
	else {
		DA::Page_Player(Player,"You do not currently own a vehicle.");
	}
	return true;
}

bool DAVehicleOwnershipGameFeatureClass::Lock_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	DAVehicleOwnershipObserverClass *Data = Get_Vehicle_Data(Player);
	if (!Data) {
		DA::Page_Player(Player,"You do not currently own a vehicle.");
	}
	else if (Data->Is_Locked() || Data->Is_Selling()) {
		DA::Page_Player(Player,"Your vehicle is already locked.");
	}
	else {
		Data->Lock();
		DA::Page_Player(Player,"Your vehicle has been locked.");
	}
	return true;
}

bool DAVehicleOwnershipGameFeatureClass::Unlock_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	DAVehicleOwnershipObserverClass *Data = Get_Vehicle_Data(Player);
	if (!Data) {
		DA::Page_Player(Player,"You do not currently own a vehicle.");
	}
	else if (!Data->Is_Locked() && !Data->Is_Selling()) {
		DA::Page_Player(Player,"Your vehicle is not locked.");
	}
	else {
		Data->Unlock();
		DA::Page_Player(Player,"Your vehicle has been unlocked.");
	}
	return true;
}

bool DAVehicleOwnershipGameFeatureClass::BL_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	VehicleGameObj *Vehicle = Player->Get_GameObj()->Get_Vehicle();
	if (!Vehicle || !Vehicle->Peek_Physical_Object()->As_MoveablePhysClass()) {
		DA::Page_Player(Player,"You must be in a vehicle to use this command.");
	}
	else if (Vehicle->Get_Driver() != Player->Get_GameObj()) {
		DA::Page_Player(Player,"You must be the driver to use this command.");
	}
	else {
		DAVehicleOwnershipObserverClass *Data = Get_Vehicle_Data(Vehicle);
		if (Data && Data->Get_Vehicle_Owner()) {
			DA::Page_Player(Player,"This vehicle is already bound to %ls.",Data->Get_Vehicle_Owner()->Get_Name());
		}
		else {
			Bind_Vehicle(Vehicle,Player)->Lock();
			DA::Page_Player(Player,"This vehicle has been bound to you and locked. Use \"!unlock\" to unlock it, or \"!unbind\"/\"!free\" to relinquish ownership.");
		}
	}
	return true;
}

bool DAVehicleOwnershipGameFeatureClass::VKick_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	VehicleGameObj *Vehicle = Get_Vehicle(Player);
	if (Vehicle) {
		bool DriverKicked = false;
		int Seats = Vehicle->Get_Definition().Get_Seat_Count();
		if (ChatType == TEXT_MESSAGE_KEYHOOK || !Text.Size()) {
			for (int i = 0;i < Seats;i++) { //Kick the first player that isn't the owner.
				SoldierGameObj *Occupant = Vehicle->Get_Occupant(i);
				if (Occupant && Occupant->Get_Player() != Player) {
					if (Exit_Vehicle(Occupant)) {
						DA::Page_Player(Occupant->Get_Player(),"%ls has removed you from the vehicle.",Player->Get_Name());
						if (i == 0) {
							DriverKicked = true;
						}
					}
					break;
				}
			}
		}
		else if (Text[1] == "all") { 
			for (int i = 0;i < Seats;i++) { //Kick all players that aren't the owner.
				SoldierGameObj *Occupant = Vehicle->Get_Occupant(i);
				if (Occupant && Occupant->Get_Player() != Player) {
					if (Exit_Vehicle(Occupant)) {
						DA::Page_Player(Occupant->Get_Player(),"%ls has removed you from the vehicle.",Player->Get_Name());
						if (i == 0) {
							DriverKicked = true;
						}
					}
				}
			}
		}
		else { //Look for any players in the vehicle that match the parameters of the command.
			for (int x = 1;x <= Text.Size();x++) { //Multiple nicks can be given at once.
				for (int i = 0;i < Seats;i++) {
					SoldierGameObj *Occupant = Vehicle->Get_Occupant(i);
					if (Occupant && Occupant->Get_Player() != Player && wcsistr(Occupant->Get_Player()->Get_Name(),Text.As_Wide(x))) {
						if (Exit_Vehicle(Occupant)) {
							DA::Page_Player(Occupant->Get_Player(),"%ls has removed you from the vehicle.",Player->Get_Name());
							if (i == 0) {
								DriverKicked = true;
							}
						}
					}
				}
			}
		}
		if (DriverKicked) { //Move owner to driver's seat.
			int OwnerSeat = Vehicle->Find_Seat(Player->Get_GameObj());
			if (OwnerSeat != -1 && Exit_Vehicle(Player->Get_GameObj())) {
				Vehicle->Add_Occupant(Player->Get_GameObj());
			}
		}
	}
	else {
		DA::Page_Player(Player,"You do not currently own a vehicle.");
	}
	return true;
}

bool DAVehicleOwnershipGameFeatureClass::VStatus_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	DAVehicleOwnershipObserverClass *Data = Get_Vehicle_Data(Player);
	if (Data) {
		StringClass Ret;
		DefenseObjectClass *Defense = Data->Get_Vehicle()->Get_Defense_Object();
		Ret.Format("Vehicle: %s%s - Health: %d/%d",DATranslationManager::Translate(Data->Get_Vehicle()),(Data->Is_Locked() || Data->Is_Selling())?"(L)":"",(int)(Defense->Get_Health()+Defense->Get_Shield_Strength()),(int)(Defense->Get_Health_Max()+Defense->Get_Shield_Strength_Max()));
		int Seats = Data->Get_Vehicle()->Get_Definition().Get_Seat_Count();	
		for (int i = 0;i < Seats;i++) {
			if (Data->Get_Vehicle()->Get_Occupant(i)) {
				Ret += StringFormat(" - Seat #%d: %ls(%s)%s",i+1,Data->Get_Vehicle()->Get_Occupant(i)->Get_Player()->Get_Name(),DATranslationManager::Translate(Data->Get_Vehicle()->Get_Occupant(i)),Has_Beacon(Data->Get_Vehicle()->Get_Occupant(i))?"(B)":"");
			}
			else { 
				Ret += StringFormat(" - Seat #%d: Empty",i+1);
			}
		}
		DA::Page_Player(Player,"%s",Ret);
	}
	else {
		DA::Page_Player(Player,"You do not currently own a vehicle.");
	}
	return true;
}

bool DAVehicleOwnershipGameFeatureClass::Free_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	DAVehicleOwnershipObserverClass *Data = Get_Vehicle_Data(Player);
	if (Data) {
		Data->Free();
		DA::Page_Player(Player,"Your vehicle has been put up for grabs.");
	}
	else {
		DA::Page_Player(Player,"You do not currently own a vehicle.");
	}
	return true;
}

bool DAVehicleOwnershipGameFeatureClass::SellVeh_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	DAVehicleOwnershipObserverClass *Data = Get_Vehicle_Data(Player);
	if (Data) {
		if (Text[1] == "cancel" || Text[1] == "halt" || Text[1] == "stop") {
			if (!Data->Is_Selling()) {
				DA::Page_Player(Player,"Your vehicle is not currently being sold.");
			}
			else {
				DA::Page_Player(Player,"You have cancelled the sale of your vehicle.");
				Data->Unlock();
			}
		}
		else if (Commands->Get_Distance(Player->Get_GameObj()->Get_Position(),Data->Get_Vehicle()->Get_Position()) <= 20.0f) {
			float Cost = Round(Get_Cost(Data->Get_Vehicle()->Get_Definition().Get_Name())*Get_Sell_Percent());
			if (Cost) {
				DA::Page_Player(Player,"Your vehicle will be sold for %d credit(s) in 10 seconds. You can stop this sale by entering the vehicle or using the \"!sellveh cancel\" command.",(int)Cost);
				Data->Sell();
			}
			else {
				DA::Page_Player(Player,"This vehicle is not worth anything.");
			}
		}
		else {
			DA::Page_Player(Player,"You must be closer to your vehicle to sell it.");
		}
	}
	else {
		DA::Page_Player(Player,"You do not currently own a vehicle.");
	}
	return true;
}

void DAVehicleOwnershipGameFeatureClass::Vehicle_Enter_Event(VehicleGameObj *Vehicle,cPlayer *Player,int Seat) {
	if (Seat == 0) {
		DAVehicleObserverClass *VehicleData = DAVehicleManager::Get_Vehicle_Data(Vehicle);
		DAVehicleOwnershipObserverClass *VehicleOwnershipData = Get_Vehicle_Data(Vehicle);
		if (VehicleData && (VehicleData->Get_Vehicle_Owner() == Player || VehicleData->Get_Team() != Player->Get_Player_Type() || (VehicleOwnershipData && VehicleOwnershipData->Is_Free())) && !Get_Vehicle_Data(Player) && Bind_Vehicle(Vehicle,Player)) { //Automatically bind the vehicle if they bought it, stole it, or it's free.
			DA::Page_Player(Player,"This vehicle has been automatically bound to you. Use \"!lock\" to lock it, or \"!unbind\"/\"!free\" to relinquish ownership.");
		}
	}
}

void DAVehicleOwnershipGameFeatureClass::Player_Leave_Event(cPlayer *Player) {
	Unbind_Vehicle(Player);
}

void DAVehicleOwnershipGameFeatureClass::Team_Change_Event(cPlayer *Player) {
	Unbind_Vehicle(Player);
}

void DAVehicleOwnershipGameFeatureClass::Object_Destroyed_Event(GameObject *obj) {
	if (!((SoldierGameObj*)obj)->Get_Player()->Get_DA_Player()->Is_Spawning()) {
		if (Unlock_Vehicle(((SoldierGameObj*)obj)->Get_Player())) {
			DA::Page_Player(((SoldierGameObj*)obj)->Get_Player(),"Your vehicle has been unlocked.");
		}
	}
}

void DAVehicleOwnershipGameFeatureClass::Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	if (Unlock_Vehicle(((SoldierGameObj*)Victim)->Get_Player())) {
		DA::Page_Player(((SoldierGameObj*)Victim)->Get_Player(),"Your vehicle has been unlocked.");
	}
}

DA_API Register_Game_Feature(DAVehicleOwnershipGameFeatureClass,"Vehicle Ownership","EnableVehicleOwnership",0);