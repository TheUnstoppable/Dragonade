/*	Renegade Scripts.dll
	Copyright 2013 Tiberian Technologies

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
#include "renalert.h"
#include "ScriptableGameObj.h"
#include "DamageableGameObj.h"
#include "WeaponClass.h"
#include "ScriptZoneGameObj.h"
#include "SmartGameObj.h"
#include "VehicleGameObjDef.h"
#include "ArmorWarheadManager.h"
#include "SoldierGameObjDef.h"
#include "SoldierGameObj.h"
#include "SuperweaponGameObj.h"
#ifdef SSGM
#include "gmlog.h"
#endif
#define PTTEAM(t) (t?0:1) //Convert a normal team into a PT team
//////////////RA_Repair_Controller//////////////

void RA_Repair_Controller::Killed(GameObject * obj,GameObject * killer)
{
	Commands->Destroy_Object(Commands->Find_Object(Get_Int_Parameter("ScriptZoneID")));
}

//////////////RA_Repair_Script//////////////

void RA_Repair_Script::Timer_Expired(GameObject * obj,int number)
{
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= 10.0)
	{
		float Max_Health = Commands->Get_Max_Health(Commands->Find_Object(number));
		float Health = Commands->Get_Health(Commands->Find_Object(number));
		float Max_Shield_Strength = Commands->Get_Max_Shield_Strength(Commands->Find_Object(number));
		float Shield_Strength = Commands->Get_Shield_Strength(Commands->Find_Object(number));
		if (Health < Max_Health)
		{
			Commands->Set_Health(Commands->Find_Object(number), Health + (float)40.0);
		}
		if (Shield_Strength < Max_Shield_Strength)
		{
			Commands->Set_Shield_Strength(Commands->Find_Object(number), Shield_Strength + (float)40.0);
		}
		Commands->Start_Timer(obj, this, 1.0, number);
	}
}
void RA_Repair_Script::Entered(GameObject * obj,GameObject *enterer)
{
	if (Commands->Get_Player_Type(enterer) == Get_Int_Parameter("TeamSelection"))
	{
		int preset = Commands->Get_Preset_ID(enterer);
		switch(preset)
		{
		case 82080019: //Soviet Transport Helicopter
		case 82080024: //Soviet Ore Truck (AI Controlled)
		case 82080026: //Allied Medium Tank
		case 82080029: //Allied Ore Truck (AI Controlled)
		case 82080043: //Allied Apache
		case 82080048: //Allied APC
		case 82080051: //Allied Mobile Artillery
		case 82080052: //Allied Light Tank
		case 82080055: //Soviet Mammoth Tank
		case 82080056: //Soviet Heavy Tank
		case 82080082: //Allied Phase Tank
		case 82080115: //Soviet V2 Launcher
		case 82080121: //Allied Ranger
		case 82080124: //Soviet Hind
		case 82080141: //Allied Ore Truck (Player Controlled)
		case 82080144: //Soviet Ore Truck (Player Controlled)
		case 82080164: //Soviet Tesla Tank
		case 82080178: //Allied Mine Layer
		case 82080181: //Soviet Mine Layer
		case 82080182: //Soviet Demolition Truck
		case 82080183: //Allied Demolition Truck
		case 82080185: //Soviet Attack Sub
		case 82080188: //Soviet MAD Tank
		case 82080189: //Allied Supply Truck
		case 82080190: //Soviet Supply Truck
		case 82080192: //Allied Destroyer
		case 82080193: //Allied Gunboat
		case 82080194: //Allied Naval Transport
		case 82080195: //Soviet Missile Sub
		case 82080196: //Soviet Naval Transport
		case 82080205: //Allied Mobile Gap
			Commands->Start_Timer(obj, this, 1.0, Commands->Get_ID(enterer));
		}
	}
}

//////////////RA_Infantry_Spy//////////////

void RA_Infantry_Spy::Created(GameObject *obj)
{
	Commands->Set_Is_Visible(obj,false);
}

void RA_Infantry_Spy::Detach(GameObject *obj)
{
	ScriptImpClass::Detach(obj);
	if (Exe != 4)
	{
		Commands->Set_Is_Visible(obj,true);
	}
}

//////////////RA_Infantry_NotSpy//////////////

void RA_Infantry_NotSpy::Created(GameObject *obj)
{
	Commands->Set_Is_Visible(obj,true);
}

//////////////RA_Helipad_Zone//////////////

void RA_Helipad_Zone::Entered(GameObject *obj, GameObject *enterer)
{
	if (Commands->Get_Player_Type(enterer) == Get_Int_Parameter("TeamSelection"))
	{
		Commands->Start_Timer(obj, this, Get_Float_Parameter("Reload_Interval"), Commands->Get_ID(enterer));
	}
}

void RA_Helipad_Zone::Timer_Expired(GameObject *obj, int number)
{
	//Distance checking code copied from RA_Repair_Script
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if ((distance <= 10.0) && (Get_Vehicle_Driver(Commands->Find_Object(number)))) //reloads vehicle up to 10m from zone center
	{
		if (Commands->Get_Preset_ID(Commands->Find_Object(number)) == Get_Int_Parameter("Apache_Preset_ID"))
		{
			Commands->Give_PowerUp(Commands->Find_Object(number),Get_Parameter("Apache_Reload_Powerup"),false);
		}
		else if (Commands->Get_Preset_ID(Commands->Find_Object(number)) == Get_Int_Parameter("Hind_Preset_ID"))
		{
			Commands->Give_PowerUp(Commands->Find_Object(number),Get_Parameter("Hind_Reload_Powerup"),false);
		}
		Commands->Start_Timer(obj, this, Get_Float_Parameter("Reload_Interval"), number);
	}
}

//////////////RA_MineLayer//////////////

void RA_MineLayer::Created(GameObject *obj)
{
	driverID = 0;
}

void RA_MineLayer::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!driverID)
		{
			driverID = Commands->Get_ID(sender);
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (driverID == Commands->Get_ID(sender))
		{
			driverID = 0;
		}
	}
}

void RA_MineLayer::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (driverID) //If there's no driver, do nothing
	{
		int hitter; //This will store the ID of the damager
		hitter = Commands->Get_ID(damager);
		if (hitter == driverID)//checking if the driver 'shot' the vehicle
		{
			if (!amount)//checking if precise ammount of damage was done
			{
				Commands->Send_Custom_Event(obj, Commands->Find_Object(Get_Int_Parameter("Mine_Manager_ID")), 300300, driverID, 0.0);
			}
		}
	}
}

void RA_MineLayer::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&driverID,4,1);
}

//////////////RA_Mine//////////////

void RA_Mine::Created(GameObject *obj)
{
	//Copied Code: I assume this 'activates' the mine or something...
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	team = Commands->Get_Player_Type(obj);
	int ap_check;
	int at_check;
	ap_check = Get_Int_Parameter("Is_Anti-Personnel");
	at_check = Get_Int_Parameter("Is_Anti-Tank");
	if (!ap_check)
	{
		AP_mine = false;
	}
	else
	{
		AP_mine = true;
	}
	if (!at_check)
	{
		AT_mine = false;
	}
	else
	{
		AT_mine = true;
	}
}

void RA_Mine::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_DRIVER)
	{
		if (ownerID)
		{
			Commands->Send_Custom_Event(obj,sender,CUSTOM_EVENT_VHEICLE_DRIVER_ID,Commands->Get_ID(Get_GameObj(ownerID)),0);
		}
		else
		{
			Commands->Send_Custom_Event(obj,sender,CUSTOM_EVENT_VHEICLE_DRIVER_ID,0,0);
		}
	}
	if (type == 656565) //Is the mine manager saying something?
	{
		ownerID = Get_Player_ID(Commands->Find_Object(param));
		Commands->Set_Player_Type(obj, Commands->Get_Player_Type(Commands->Find_Object(param)));
		team = Commands->Get_Player_Type(obj);
	}
}

void RA_Mine::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	/*Note: The mine will detonate as soon as it sees its target.
	Therefore the proximity range is the 'Sight Range' on the vehicle
	*/
	if (AP_mine) //Is this mine anti-personnel?
	{
		if (enemy->As_SoldierGameObj())//using newer, more specific code
		{
			GameObject *damager = Get_GameObj(ownerID);
			int objID = Commands->Get_Player_Type(obj);
			int ownerID = Commands->Get_Player_Type(damager);
			Commands->Set_Player_Type(obj, 2); //Change teams to avoid friendly fire
			SetExplosionObj(obj);
			if (objID != ownerID) //Did the owner switch teams?
			{
				Commands->Apply_Damage(obj,Get_Float_Parameter("Trigger_Damage"),Get_Parameter("Trigger_Warhead"),0); //Die Infidel!
			}
			else //Still on same side
			{
				Commands->Apply_Damage(obj,Get_Float_Parameter("Trigger_Damage"),Get_Parameter("Trigger_Warhead"),damager); //Die Infidel!
			}
			SetExplosionObj(0);
			return; //the mine is dead, end now
		}
	}
	if (AT_mine) //Is this mine anti-tank?
	{
		if (enemy->As_VehicleGameObj())//using newer, more specific code
		{
			GameObject *damager = Get_GameObj(ownerID);
			int objID = Commands->Get_Player_Type(obj);
			int ownerID = Commands->Get_Player_Type(damager);
			Commands->Set_Player_Type(obj, 2); //Change teams to avoid friendly fire
			SetExplosionObj(obj);
			if (objID != ownerID) //Did the owner switch teams?
			{
				Commands->Apply_Damage(obj,Get_Float_Parameter("Trigger_Damage"),Get_Parameter("Trigger_Warhead"), 0); //Good-bye cruel world!
			}
			else
			{
				Commands->Apply_Damage(obj,Get_Float_Parameter("Trigger_Damage"),Get_Parameter("Trigger_Warhead"), damager); //Good-bye cruel world!
			}
			SetExplosionObj(0);
			return; //the mine is dead, end now
		}
	}
}

void RA_Mine::Destroyed(GameObject *obj)
{
	//Blaaargh! I'm dead!
	Commands->Send_Custom_Event(obj, Commands->Find_Object(Get_Int_Parameter("Mine_Manager_ID")), 200200, team, 0.0);
}

void RA_Mine::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&AP_mine,1,1);
	Auto_Save_Variable(&AT_mine,1,2);
	Auto_Save_Variable(&ownerID,4,3);
}

//////////////RA_Mine_Manager//////////////

void RA_Mine_Manager::Created(GameObject *obj)
{
	all_mines = 0;
	sov_mines = 0;
}

void RA_Mine_Manager::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// message 300300: MineLayer requesting Mine placement
	// message 200200: Mine announcing its destruction
	// message 656565: MineManager informing Mine of its Owner
	// param is driver's ID number
	/*Variables moved within the 'if's so they aren't created unless
	the mesage is valid. No need to take up space if it isn't needed*/
	if (type == 300300)
	{
		int team = Commands->Get_Player_Type(Commands->Find_Object(param));

		if (!team)//check if this is a soviet request
		{
			int limit = Get_Int_Parameter("Mine_Limit");

			if (sov_mines < limit) //is team under the limit
			{
				//Under limit, request approved, mine created
				sov_mines++;
				GameObject *mine;
				mine = Commands->Create_Object_At_Bone(sender,Get_Parameter("Soviet_Mine"),Get_Parameter("BoneName"));
				Commands->Send_Custom_Event(obj, mine, 656565, param, 1.0);
			}
			else
			{
				//Team at mine limit, request denied, refunding ammo
				Commands->Give_PowerUp(sender, Get_Parameter("Mine_Reload"), false);
			}
		}
		else if (team == 1)//check if its an allied request
		{
			int limit = Get_Int_Parameter("Mine_Limit");

			if (all_mines < limit) //is team under the limit
			{
				//Under limit, request approved, mine created
				all_mines++;
				GameObject *mine;
				mine = Commands->Create_Object_At_Bone(sender,Get_Parameter("Allied_Mine"),Get_Parameter("BoneName"));
				Commands->Send_Custom_Event(obj, mine, 656565, param, 1.0);
			}
			else
			{
				//Team at mine limit, request denied, refunding ammo
				Commands->Give_PowerUp(sender, Get_Parameter("Mine_Reload"), false);
			}
		}
	}
	else if (type == 200200)
	{
		if (!param)//Is this a soviet mine? (message param should be team type)
		{
			sov_mines--; //A moment of silence for out fallen comrade
		}
		else if (param == 1)//or is it an allied mine? (message param should be team type)
		{
			all_mines--; //Why, God?! Why did it have to die?!
		}
	}
}

void RA_Mine_Manager::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&all_mines,4,1);
	Auto_Save_Variable(&sov_mines,4,2);
}

//////////////RA_ObjectDeath_OnCreate//////////////

void RA_ObjectDeath_OnCreate::Created(GameObject *obj)
{
	/*NeoSaber's Note: The purpose of this script is to create a new 
	object at the location of another object's creation, then kill 
	the script generated one to use its KilledExplosion setting to 
	place a sound on the map. Only way I've found to 'create' sounds 
	with scripts in multiplayer*/
	const char *c;
	const char *war = "Death";
	Vector3 v;
	c = Get_Parameter("Object");
	v = Commands->Get_Position(obj);
	Commands->Apply_Damage(Commands->Create_Object(c,v),100,war,0);
	Destroy_Script();
}

//////////////RA_Naval_Yard//////////////

void RA_Naval_Yard::Created(GameObject *obj)
{
	team_count = 0;
	active = true;
	ready = true;
}

void RA_Naval_Yard::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	//951951 = purchase authorization request
	//753753 = naval unit died
	if (type == 951951) //Authorize request?
	{
		if (active) //am I still alive?
		{
			if (ready)//was a unit just built?
			{
				int limit = Get_Int_Parameter("Limit");
				if (team_count < limit) //Are we under limit?
				{			
					//Authorize purchase and increase count by 1
					Commands->Send_Custom_Event(obj, sender, 159159, 1, 0.0);
					team_count++;
					ready = false;
					Commands->Start_Timer(obj, this, Get_Float_Parameter("Disable_Time"), 0);
									}
				else
				{
					//Authorization denied
					Commands->Send_Custom_Event(obj, sender, 159159, 0, 0.0);
				}
			}
			else//Not ready to build yet
			{	
				//Authorization denied
				Commands->Send_Custom_Event(obj, sender, 159159, 0, 0.0);
			}
		}
		else
		{
			//If I'm dead, everyone dies too! Hahahahaha!
			Commands->Destroy_Object(sender);
		}
	}
	else if (type == 753753) //Death Announcement?
	{
		//Why do the young die?! Why!?
		team_count--;
	}
}

void RA_Naval_Yard::Killed(GameObject *obj, GameObject *killer)
{
	active = false;
}

void RA_Naval_Yard::Timer_Expired(GameObject *obj, int number)
{
	//'cool down' time complete, construction allowed again
	ready = true;
}

void RA_Naval_Yard::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&team_count,4,1);
	Auto_Save_Variable(&active,4,2);
	Auto_Save_Variable(&ready,4,3);
}

//////////////RA_Naval_PT_Pokable//////////////

void RA_Naval_PT_Pokable::Created(GameObject *obj)
{
	storedID = 0;
	Commands->Display_Health_Bar(obj, false);
	Commands->Set_Player_Type(obj, Get_Int_Parameter("Team"));
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
}

void RA_Naval_PT_Pokable::Poked(GameObject *obj, GameObject *poker)
{
	//first check if player matches team
	int teamtest = Get_Int_Parameter("Team");
	if ((Commands->Get_Player_Type(poker)) == teamtest)
	{
		//ok, player is on my team, does he have the cash?
		float cost = Get_Float_Parameter("Cost");
		if (Commands->Get_Money(poker) >= cost)
		{
			//player has the money, are they allowed?
			storedID = Commands->Get_ID(poker); //remember for later
			Commands->Send_Custom_Event(obj, Commands->Find_Object(Get_Int_Parameter("Controller_ID")), 951951, 0, 0.0);
		}
	}
}

void RA_Naval_PT_Pokable::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	//159159 = authorization response
	if (type == 159159)
	{
		if (param == 1)
		{
			if (storedID) //Do I remember a player?
			{
				float cost = Get_Float_Parameter("Cost");
				cost = -cost;
				Commands->Give_Money(Commands->Find_Object(storedID),cost,false);
				//tell zone to clear the the construction zone
				Commands->Send_Custom_Event(obj, Commands->Find_Object(Get_Int_Parameter("Zone_ID")), 846846, 0, 0.0);
				//1 second delay to allow zone to clear
				Commands->Start_Timer(obj,this,1.0, 0);
				//clear the ID, we don't need it now
				storedID = 0;
			}
		}
		else
		{
			storedID = 0; //request denied, erase ID
		}
	}
}

void RA_Naval_PT_Pokable::Timer_Expired(GameObject *obj, int number)
{
	//Create the object
	GameObject *newobj;
	newobj = Commands->Create_Object(Get_Parameter("Preset_Name"), Commands->Get_Position(Commands->Find_Object(Get_Int_Parameter("Zone_ID"))));
	Commands->Set_Facing(newobj, Commands->Get_Facing(Commands->Find_Object(Get_Int_Parameter("Zone_ID"))));
	Commands->Attach_Script(newobj, "RA_Naval_Unit", Get_Parameter("Controller_ID"));
}

void RA_Naval_PT_Pokable::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&storedID,4,1);
}

//////////////RA_Naval_Unit//////////////

void RA_Naval_Unit::Destroyed(GameObject *obj)
{
	int controller = Get_Int_Parameter("Controller_ID");
	Commands->Send_Custom_Event(obj, Commands->Find_Object(controller), 753753, 0, 0.0);
}

//////////////RA_Naval_Zone//////////////

void RA_Naval_Zone::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 846846)
	{
		//Fire in the hole!!!!
		Commands->Create_Explosion(Get_Parameter("Explosion"), Commands->Get_Position(obj), 0);
	}
}

//////////////RA_Damaged_Credits//////////////

void RA_Damaged_Credits::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (Commands->Is_A_Star(damager)) //Ignore if not a player
	{
		int objtype1 = obj->As_DamageableGameObj()->Get_Player_Type();
		int objtype2 = damager->As_DamageableGameObj()->Get_Player_Type();
		if (!objtype1 || objtype1 == 1) //Ignore if not soviet or allied
		{
			if (objtype1 != objtype2) //Test if on different teams
			{
				if (amount > 0) //If 'damage' was harmful
				{
					float divisor = Get_Float_Parameter("Divisor");
					Commands->Give_Money(damager, amount/divisor, false);
				}
			}
			else if (objtype1 == objtype2) //Just to be sure...
			{
				if (amount < 0) //If 'damage' was helpful
				{
					float divisor = Get_Float_Parameter("Divisor");
					Commands->Give_Money(damager, (-amount)/divisor, false);
				}
			}
		}
	}
}

//////////////RA_GameStart_Detector//////////////

void RA_GameStart_Detector::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, Get_Float_Parameter("Interval"), 564);
}

void RA_GameStart_Detector::Killed(GameObject *obj, GameObject *killer)
{
	int receiverID = Get_Int_Parameter("ReceiverID");
	int type = Get_Int_Parameter("Message");
	int param = Get_Int_Parameter("Parameter");
	Commands->Send_Custom_Event(obj, Commands->Find_Object(receiverID), type, param, 0.0);
}

void RA_GameStart_Detector::Timer_Expired(GameObject *obj, int number)
{
	if (number == 564)
	{
		Commands->Apply_Damage(obj, 100.0, "BlamoKiller", 0);
		Commands->Start_Timer(obj, this, Get_Float_Parameter("Interval"), 564);
	}
}

//////////////RA_DriverDeath//////////////

void RA_DriverDeath::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, 0.5, 12121);
}

void RA_DriverDeath::Timer_Expired(GameObject *obj, int number)
{
	if (number == 12121)
	{
		Commands->Apply_Damage(obj, 100, "Death", 0);
	}
}

//////////////RA_Conyard_Repair//////////////

void RA_Conyard_Repair::Created(GameObject * obj)
{
	Enabled = false;
	objID = Commands->Get_ID(obj);
}

void RA_Conyard_Repair::Custom(GameObject * obj, int type, int param, GameObject * sender)
{
	if (type == 3000)
	{
		if (param == 230032)
		{
			if (!Enabled) //prevents more than one timer from starting
			{
				Enabled = true;
				Commands->Start_Timer(obj, this, Get_Float_Parameter("Repair_Frequency"), objID);
			}
		}
		else if (param == 320023)
		{
			Enabled = false;
		}
	}
}

void RA_Conyard_Repair::Timer_Expired(GameObject * obj,int number)
{
	float amount = Commands->Get_Health(obj);
	if (amount > 0) 
	{
		int power = 2;
		if (!Is_Base_Powered(Commands->Get_Player_Type(obj)))
		{
			power = 1;
		}
		Commands->Set_Health(obj,(amount+power));
		//These next checks are to trigger building damage state changes as health refills
		float max = Commands->Get_Max_Health(obj);
		if (amount < (max * 0.25) && (amount + power) > (max * 0.25))
		{
			Commands->Apply_Damage(obj, 0, "None", 0);
		}
		else if (amount < (max * 0.5) && (amount + power) > (max * 0.5))
		{
			Commands->Apply_Damage(obj, 0, "None", 0);
		}
		else if (amount < (max * 0.75) && (amount + power) > (max * 0.75))
		{
			Commands->Apply_Damage(obj, 0, "None", 0);
		}
		//Only start a new timer if still enabled
		if (Enabled)
		{
			Commands->Start_Timer(obj, this, Get_Float_Parameter("Repair_Frequency"), objID);
		}
		else
		{
			return;
		}
	}
}

void RA_Conyard_Repair::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&Enabled,4,1);
	Auto_Save_Variable(&objID,4,2);
}

//////////////RA_ObjectDeath_OnDeath//////////////

void RA_ObjectDeath_OnDeath::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *BoomBox;
	BoomBox = Commands->Create_Object(Get_Parameter("Preset"),Commands->Get_Position(obj));
	Commands->Apply_Damage(BoomBox, 100, "BlamoKiller", 0);
	Destroy_Script();
}

//////////////RA_Demolition_Truck_Improved//////////////

void RA_Demolition_Truck_Improved::Created(GameObject *obj)
{
	driverID = 0;
	triggered = false;
}

void RA_Demolition_Truck_Improved::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_DRIVER)
	{
		if (driverID)
		{
			Commands->Send_Custom_Event(obj,sender,CUSTOM_EVENT_VHEICLE_DRIVER_ID,driverID,0);
		}
		else
		{
			Commands->Send_Custom_Event(obj,sender,CUSTOM_EVENT_VHEICLE_DRIVER_ID,0,0);
		}
	}
	//checks when someone gets in or out and records ID
	//this should account for when there are passengers too
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!driverID) //is there a driver already?
		{
			driverID = Commands->Get_ID(sender);
		}
	}
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		int testID;
		testID = Commands->Get_ID(sender);
		if (driverID == testID) //is it the driver that got out?
		{
			driverID = 0;
		}
	}
}

void RA_Demolition_Truck_Improved::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (!driverID)
	{
		return; //if there's no driver, don't bother
	}
	int testID;
	testID = Commands->Get_ID(damager);
	if (driverID == testID) //did the driver shoot me?
	{
		const char *warhead = "Death";
		//I can't take it any more! *bang*
		Commands->Apply_Damage(obj, 100.00, warhead, 0);
		triggered = true;
	}
}

void RA_Demolition_Truck_Improved::Killed(GameObject *obj, GameObject *killer)
{
	Vector3 pos;
	pos = Commands->Get_Position(obj);
	int testID;
	testID = Commands->Get_ID(killer);
	const char *explosion = Get_Parameter("Explosion");
	const char *warhead = Get_Parameter("Warhead");
	float damage = Get_Float_Parameter("DamageStrength");
	float radius = Get_Float_Parameter("DamageRadius");
	if (testID) //is there a real killer?
	{
		Commands->Create_Explosion(explosion, pos, killer);
		Ranged_Scale_Damage_To_Buildings(damage, warhead,Commands->Get_Position(obj),radius,killer);
	}
	else if (triggered)//then was it the driver?
	{
		Commands->Create_Explosion(explosion, pos, Commands->Find_Object(driverID));
		Ranged_Scale_Damage_To_Buildings(damage,warhead,Commands->Get_Position(obj),radius,Commands->Find_Object(driverID));
	}
	if (driverID)//If there's a driver, kill him
	{
		Commands->Attach_Script(Commands->Find_Object(driverID),"RA_DriverDeath", "0");
	}
}

void RA_Demolition_Truck_Improved::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&driverID,4,1);
	Auto_Save_Variable(&triggered,4,2);
}


///////////////////RA_CTF_Zone///////////////////

void RA_CTF_Zone::Created(GameObject * obj)
{
	GotFlag = false;
	NumCaptured = 0;
	PlayerId = 0;
	FlagId = 0;
	Vector3 newFlagPos = Commands->Get_Position(obj);
	GameObject * newFlag = Commands->Create_Object(Get_Parameter("Flag_Preset_Name"), newFlagPos);
	DecorFlag = Commands->Get_ID(newFlag);
	DecorFlagPresent = false;
}

void RA_CTF_Zone::Entered(GameObject * obj, GameObject *enterer)
{
	if (Get_Int_Parameter("Team_ID") != Commands->Get_Player_Type(enterer))
	{
		Commands->Send_Custom_Event(obj, enterer, 10004, 0, 0);				//ask player if it has the flag
	}
	else if (!GotFlag)
	{
		Commands->Attach_Script(enterer, "TDA_CTF_Attached", "0");			//load up the player script
		GameObject * tmpObj = Commands->Create_Object_At_Bone(enterer, Get_Parameter("Flag_Preset_Name"), "LHAND"); //create the flag object
		int objID = Commands->Get_ID(tmpObj);								//get the new flags ID
		FlagId = objID;
		Commands->Send_Custom_Event(obj, enterer, 10003, objID, 0);			//tell the player script what the flag id is
		Commands->Attach_To_Object_Bone(tmpObj, enterer, "LHAND");			//attach flag to player
		objID = Commands->Get_ID(obj);										//get the current zones ID
		Commands->Send_Custom_Event(obj, enterer, 10005, objID, 0);			//let the player know the ID of zone
		PlayerId = Commands->Get_ID(enterer);
		GotFlag = true;														//let zone know NOT to create a new flag
		Commands->Start_Timer(obj, this, 10, 411);							//needed to add this for the suicide and lag flags
		Commands->Give_Points(enterer, 75, false);
		Commands->Destroy_Object(Commands->Find_Object(DecorFlag));
		DecorFlagPresent = false;
	}
}

void RA_CTF_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&GotFlag,4,1);
	Auto_Save_Variable(&NumCaptured,4,2);
	Auto_Save_Variable(&DecorFlag,4,3);
	Auto_Save_Variable(&DecorFlagPresent,4,4);
	Auto_Save_Variable(&FlagId,4,5);
	Auto_Save_Variable(&PlayerId,4,6);
}

void RA_CTF_Zone::Custom(GameObject * obj, int type, int param, GameObject * sender)
{
	GameObject * newFlag;
	Vector3 newFlagPos;
	if ((type == 10002) && (GotFlag))
	{
		GotFlag = false;													//tell zone that flag can be recaptured
		Commands->Send_Custom_Event(obj, sender, 10002, 1, 0);				//tell player script "kill your flag, it is now captured"
		NumCaptured++;														//increment the # of captures
		newFlagPos = Commands->Get_Position(obj);
		newFlag = Commands->Create_Object(Get_Parameter("Flag_Preset_Name"), newFlagPos);
		DecorFlag = Commands->Get_ID(newFlag);
		Commands->Give_Points(sender, 250, false);
		if (NumCaptured == Get_Int_Parameter("Max_Capture"))								//check to see if the max captures have been met
		{
			if (!Get_Int_Parameter("Team_ID")) //Is this a Soviet Win?
			{
				//Then kill all Allied Buildings
				Kill_All_Buildings_By_Team(1);
			}
			else if (Get_Int_Parameter("Team_ID") == 1) //Is this an Allied Win?
			{
				//Then kill all Allied Buildings
				Kill_All_Buildings_By_Team(0);
			}
		}
	}
	else if ((type == 10005) && (GotFlag))
	{
		GotFlag = false;
		if (!DecorFlagPresent)
		{
			newFlagPos = Commands->Get_Position(obj);
			newFlag = Commands->Create_Object(Get_Parameter("Flag_Preset_Name"),newFlagPos);
			DecorFlag = Commands->Get_ID(newFlag);
			DecorFlagPresent=true;
		}
	}
}

void RA_CTF_Zone::Timer_Expired(GameObject * obj,int number)
{
	if (GotFlag)
	{
		Vector3 fPos = Commands->Get_Position(Commands->Find_Object(FlagId));
		Vector3 pPos = Commands->Get_Position(Commands->Find_Object(PlayerId));
		float dPos = Commands->Get_Distance(fPos, pPos);
		if (dPos > 2)
		{
			GameObject * flagObj = Commands->Find_Object(FlagId);
			Commands->Destroy_Object(flagObj);
			Commands->Send_Custom_Event(obj, obj, 10005, 0, 0);
			FlagId = 0;
			PlayerId = 0;
			GotFlag = false;
		}
		Commands->Destroy_Object(Commands->Find_Object(DecorFlag));
		DecorFlagPresent = false;
		Commands->Start_Timer(obj, this, 10, 411);
	}
}

/////////////RA_MAD_Tank_Improved/////////////


void RA_MAD_Tank_Improved::Created(GameObject *obj)
{
	//Initializing variables
	driverID = 0;
	deployed = false;
	newobjID = 0;
	ownerID = 0;
}

void RA_MAD_Tank_Improved::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_DRIVER)
	{
		if (driverID)
		{
			Commands->Send_Custom_Event(obj,sender,CUSTOM_EVENT_VHEICLE_DRIVER_ID,driverID,0);
		}
		else
		{
			Commands->Send_Custom_Event(obj,sender,CUSTOM_EVENT_VHEICLE_DRIVER_ID,0,0);
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		driverID = Commands->Get_ID(sender);
	}
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		int testID;
		testID = Commands->Get_ID(sender);
		if (driverID == testID) //is it the driver that got out?
		{
			driverID = 0;
		}
	}
}

void RA_MAD_Tank_Improved::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (!deployed) //If already deployed, don't do anything
	{
		if (driverID) //Make sure there's a driver
		{
			int testID = Commands->Get_ID(damager); //Get ID to compare
			if (driverID == testID)
			{
				//Create Object to jam MAD Tank in place
				GameObject *newobj = Commands->Create_Object_At_Bone(obj, "Invisible_Object", Get_Parameter("Bone"));
				Commands->Set_Model(newobj, Get_Parameter("Attach_Model"));
				Commands->Attach_To_Object_Bone(newobj, obj, Get_Parameter("Bone"));
				newobjID = Commands->Get_ID(newobj);
				//Animate MAD Tank, record player ID of owner
				Commands->Create_2D_Sound(Get_Parameter("Announcement"));
				Commands->Set_Animation(obj, Get_Parameter("Deployed_Animation"), false, 0, 0, -1, false);
				ownerID = Get_Player_ID(damager);
				//New stuff added for v0.9935
				Force_Occupants_Exit(obj); //kick driver out
				Commands->Enable_HUD_Pokable_Indicator(obj, false);	//this was to correct some issue
				Commands->Enable_Vehicle_Transitions(obj, false); //no getting back in
				Commands->Set_Player_Type(obj, Commands->Get_Player_Type(damager)); //set team
				//End New Stuff
				deployed = true;
			}
		}
	}
}

void RA_MAD_Tank_Improved::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, Get_Parameter("Deployed_Animation"))) //Is this the right animation?
	{
		if (Get_GameObj(ownerID)) //sonic pulse only if owner still valid
		{
			//Create/Kill Object for Sonic Pulse, this is meant for visual explosion effect only
			GameObject *SonicBox = Commands->Create_Object(Get_Parameter("Sonic_Preset"), Commands->Get_Position(obj));
			Commands->Set_Player_Type(SonicBox, 2); //just in case
			Commands->Apply_Damage(SonicBox,100,"BlamoKiller",Get_GameObj(ownerID));
			//Setup for building damage
			float percentage = Get_Float_Parameter("Percentage");
			const char *warhead = Get_Parameter("Warhead");
			float range = Get_Float_Parameter("DamageRadius");
			//Time to break a few windows
			Ranged_Percentage_Damage_To_Buildings(percentage, warhead, Commands->Get_Position(obj), range, Get_GameObj(ownerID));
			Ranged_Percentage_Damage_To_Vehicles(percentage, warhead, Commands->Get_Position(obj), range, Get_GameObj(ownerID)); 
		}
		//Kill MAD Tank 
		//'0' Damager used so owner won't get points for tank itself
		Commands->Apply_Damage(obj,100,"BlamoKiller",0);
	}
}

void RA_MAD_Tank_Improved::Destroyed(GameObject *obj)
{
	//MAD Tank dead, get rid of piston object if it exists
	if (newobjID)
	{
		Commands->Destroy_Object(Commands->Find_Object(newobjID));
	}
}

void RA_MAD_Tank_Improved::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&driverID,4,1);
	Auto_Save_Variable(&deployed,4,2);
	Auto_Save_Variable(&newobjID,4,3);
	Auto_Save_Variable(&ownerID,4,4);
}

//////////////RA_Conyard_Controller_Improved//////////////

void RA_Conyard_Controller_Improved::Created(GameObject *obj)
{
	int type = 3000;
	int param = 230032;
	int team = Get_Int_Parameter("Team");
	Send_Custom_To_Team_Buildings(team,obj,type,param,1);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset1"), obj, type, param, 1);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset2"), obj, type, param, 1);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset3"), obj, type, param, 1);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset4"), obj, type, param, 1);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset5"), obj, type, param, 1);
}

void RA_Conyard_Controller_Improved::Killed(GameObject * obj, GameObject * killer)
{
	int type = 3000;
	int param = 320023;
	int team = Get_Int_Parameter("Team");
	Send_Custom_To_Team_Buildings(team,obj,type,param,0);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset1"), obj, type, param, 0);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset2"), obj, type, param, 0);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset3"), obj, type, param, 0);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset4"), obj, type, param, 0);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset5"), obj, type, param, 0);
}

void RA_Conyard_Controller_Improved::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if ((type == 3000) && (!Is_Building_Dead(obj)))
	{
		if (param == 230032)
		{
			int _message = 3000;
			int _param = 230032;
			int team = Get_Int_Parameter("Team");
			Send_Custom_To_Team_Buildings(team,obj,_message,_param,1);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset1"), obj, _message, _param, 1);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset2"), obj, _message, _param, 1);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset3"), obj, _message, _param, 1);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset4"), obj, _message, _param, 1);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset5"), obj, _message, _param, 1);
		}
		else if (param == 320023)
		{
			int _message = 3000;
			int _param = 320023;
			int team = Get_Int_Parameter("Team");
			Send_Custom_To_Team_Buildings(team,obj,_message,_param,0);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset1"), obj, _message, _param, 0);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset2"), obj, _message, _param, 0);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset3"), obj, _message, _param, 0);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset4"), obj, _message, _param, 0);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset5"), obj, _message, _param, 0);
		}
	}
}

//////////////RA_Conyard_Controller_Improved_2//////////////

void RA_Conyard_Controller_Improved_2::Created(GameObject *obj)
{
	int type = 3000;
	int param = 230032;
	int team = Get_Int_Parameter("Team");
	Send_Custom_To_Team_Buildings(team,obj,type,param,1);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset1"), obj, type, param, 1);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset2"), obj, type, param, 1);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset3"), obj, type, param, 1);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset4"), obj, type, param, 1);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset5"), obj, type, param, 1);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset6"), obj, type, param, 1);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset7"), obj, type, param, 1);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset8"), obj, type, param, 1);
}

void RA_Conyard_Controller_Improved_2::Killed(GameObject * obj, GameObject * killer)
{
	int type = 3000;
	int param = 320023;
	int team = Get_Int_Parameter("Team");
	Send_Custom_To_Team_Buildings(team,obj,type,param,0);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset1"), obj, type, param, 0);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset2"), obj, type, param, 0);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset3"), obj, type, param, 0);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset4"), obj, type, param, 0);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset5"), obj, type, param, 0);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset6"), obj, type, param, 0);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset7"), obj, type, param, 0);
	Send_Custom_To_Team_Preset(team, Get_Parameter("Preset8"), obj, type, param, 0);
}

void RA_Conyard_Controller_Improved_2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if ((type == 3000) && (!Is_Building_Dead(obj)))
	{
		if (param == 230032)
		{
			int _message = 3000;
			int _param = 230032;
			int team = Get_Int_Parameter("Team");
			Send_Custom_To_Team_Buildings(team,obj,_message,_param,1);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset1"), obj, _message, _param, 1);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset2"), obj, _message, _param, 1);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset3"), obj, _message, _param, 1);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset4"), obj, _message, _param, 1);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset5"), obj, _message, _param, 1);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset6"), obj, _message, _param, 1);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset7"), obj, _message, _param, 1);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset8"), obj, _message, _param, 1);
		}
		else if (param == 320023)
		{
			int _message = 3000;
			int _param = 320023;
			int team = Get_Int_Parameter("Team");
			Send_Custom_To_Team_Buildings(team,obj,_message,_param,0);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset1"), obj, _message, _param, 0);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset2"), obj, _message, _param, 0);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset3"), obj, _message, _param, 0);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset4"), obj, _message, _param, 0);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset5"), obj, _message, _param, 0);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset6"), obj, _message, _param, 0);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset7"), obj, _message, _param, 0);
			Send_Custom_To_Team_Preset(team, Get_Parameter("Preset8"), obj, _message, _param, 0);
		}
	}
}

/////////////////RA_Building_DeathSound/////////////////

void RA_Building_DeathSound::Created(GameObject *obj)
{
	halfdead = false;
}

void RA_Building_DeathSound::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	const float max = Commands->Get_Max_Health(obj);
	const float current = Commands->Get_Health(obj);
	if (current <= (max/2))
	{
		if (!halfdead)
		{
			halfdead = true;
			Commands->Create_Sound(Get_Parameter("HalfSound"), Commands->Get_Position(obj), obj);
		}
	}
	else
	{
		if (halfdead)
		{
			halfdead = false;
		}
	}
}

void RA_Building_DeathSound::Killed(GameObject *obj, GameObject *killer)
{
	Commands->Create_Sound(Get_Parameter("DeadSound"), Commands->Get_Position(obj), obj);
}

void RA_Building_DeathSound::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&halfdead,4,1);
}

/////////////////RA_Base_Defense_Simple/////////////////

void RA_Base_Defense_Simple::Created(GameObject *obj)
{
	//Initialize Variables
	targetID = 0;
	attacking = false;
	current_priority = 10;
	attack_timer = 1.0f;
	loop_count = 0;
	//Standard AI Startup Junk
	Commands->Enable_Enemy_Seen(obj, true);
	Commands->Enable_Vehicle_Transitions(obj, false);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Start_Timer(obj, this, attack_timer, 11);
}

void RA_Base_Defense_Simple::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if(Get_Object_Type(obj)==-2)
	{
		return;
	}

	if (!damager || amount <= 0)
	{
		return;
	}
	if ((Get_Vehicle_Mode(damager) == VEHICLE_TYPE_FLYING) && !Is_Script_Attached(damager,"JFW_Submarine"))
	{
		if (!(Get_Int_Parameter("AntiAir")))
		{
			return; //Ignore the target
		}
	}
	else
	{
		if (!(Get_Int_Parameter("AntiGround")))
		{
			return; //Ignore the target
		}
	}
	if (targetID) //check for current target
	{
		unsigned long newID = Commands->Get_ID(damager);
		if (targetID == newID)
		{
			return; //already firing at target
		}
	}
	int objID = Commands->Get_Player_Type(obj);
	int damagerID = Commands->Get_Player_Type(damager);
	if (objID != damagerID) //Make sure an enemy attacked
	{
		if (Commands->Get_Health(damager))
		{
			Attack(obj, damager);
		}
	}
}

void RA_Base_Defense_Simple::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	if ((Get_Vehicle_Mode(enemy) == VEHICLE_TYPE_FLYING) && !Is_Script_Attached(enemy,"JFW_Submarine"))
	{
		if (!(Get_Int_Parameter("AntiAir")))
		{
			return; //Ignore the target
		}
	}
	else
	{
		if (!(Get_Int_Parameter("AntiGround")))
		{
			return; //Ignore the target
		}
	}
	if (targetID) //check for current target
	{
		unsigned long newID = Commands->Get_ID(enemy);
		if (targetID == newID)
		{
			return; //already firing at target
		}
	}
	if (Commands->Get_Health(enemy))
	{
		Attack(obj,enemy);
	}
}

void RA_Base_Defense_Simple::Timer_Expired(GameObject *obj, int number)
{
	if (number != 11)
	{
		return; //If its not the timer we're interested in, end now
	}
	bool call_attack = true;
	if (!attacking)	//skip facing action if attacking
	{
		if (loop_count > 4) //only face new direction every 5th loop
		{
			Vector3 pos = Commands->Get_Position(obj); 
			switch (Commands->Get_Random_Int(0,8))
			{
				case 0:
					pos.X += 10;
					pos.Y -= 10;
					break;
				case 1:
					pos.X -= 10;
					pos.Y -= 10;
					break;
				case 2:
					pos.X += 10;
					pos.Y += 10;
					break;
				case 3:
					pos.X -= 10;
					pos.Y += 10;
					break;
				case 4:
					pos.X += 10;
					break;
				case 5:
					pos.X -= 10;
					break;
				case 6:
					pos.Y += 10;
					break;
				case 7:
					pos.Y -= 10;
					break;
				default: 
					pos.X += 10;
					break;
			}
			ActionParamsStruct params;
			params.Set_Basic(this, 1, 30); //priority low
			params.Set_Face_Location(pos, 6);
			Commands->Action_Face_Location(obj, params);
		}
	}
	else
	{
		attacking = false;
		current_priority = 10;
		GameObject *target = Commands->Find_Object(targetID);
		if (!(target)) //If target invalid, end now
		{
			targetID = 0;
			call_attack = false;
		}
		else
		{
			int TargetType = Commands->Get_Player_Type(target);
			int myType = Commands->Get_Player_Type(obj);
			if (myType == TargetType) //Check old target for team change
			{
				targetID = 0; //old target a friend now
				call_attack = false;
			}
			//Check Range
			Vector3 objPos = Commands->Get_Position(obj);
			Vector3 targetPos = Commands->Get_Position(target);
			float range = Commands->Get_Distance(objPos, targetPos);
			float max = Get_Float_Parameter("MaxRange");
			float min = Get_Float_Parameter("MinRange");
			if (range < min || range > max)
			{
				targetID = 0; //old target out of range
				call_attack = false;
			}
			//Check Settings
			if (((Get_Vehicle_Mode(target) == VEHICLE_TYPE_FLYING) || (Get_Vehicle_Mode(Get_Vehicle(target)) == VEHICLE_TYPE_FLYING)) && !Is_Script_Attached(target,"JFW_Submarine"))
			{
				if (!(Get_Int_Parameter("AntiAir")))
				{
					targetID = 0; //old target not targetable
					call_attack = false;
				}
			}
			else
			{
				if (!(Get_Int_Parameter("AntiGround")))
				{
					targetID = 0; //old target not targetable
					call_attack = false;
				}
			}
		}
		if (call_attack && obj->As_DamageableGameObj()->Is_Enemy(target->As_DamageableGameObj())) //if attack and if call_attack was not turned false
		{
			if (Commands->Get_Health(target))
			{
				Attack(obj, target);
			}
		}
		else 
			Commands->Action_Reset(obj, 100);
	}
	if (loop_count > 4)
	{
		loop_count = 0; //reset counter
	}
	loop_count++;
	Commands->Start_Timer(obj, this, attack_timer, 11);
}

void RA_Base_Defense_Simple::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id == 100) //Attack completed
	{
		attacking = false;
		targetID = 0;
		current_priority = 10;
	}
}

void RA_Base_Defense_Simple::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&targetID,4,1);
	Auto_Save_Variable(&attacking,4,2);
	Auto_Save_Variable(&current_priority,4,3);
	Auto_Save_Variable(&attack_timer,4,4);
	Auto_Save_Variable(&loop_count,4,5);
}

void RA_Base_Defense_Simple::Attack(GameObject *obj, GameObject *target)
{
	/**************Evaluate Target Threat****************/
	GameObject *FinalTarget = Set_Target(target);
	if (FinalTarget) //Was there a non-NULL return from Set_Target?
	{
		FinalTarget = Select_Target(obj, FinalTarget); //Was Rate_Target
	}
	/****************Execute Final Attack****************/
	if (FinalTarget && FinalTarget->As_DamageableGameObj()->Is_Enemy(obj->As_DamageableGameObj()))
	{
		targetID = Commands->Get_ID(FinalTarget); //remember new target, or re-remember old target
		Vector3 objPos = Commands->Get_Position(obj);
		Vector3 targetPos = Commands->Get_Position(FinalTarget);
		float range = Commands->Get_Distance(objPos, targetPos);
		float max = Get_Float_Parameter("MaxRange");
		float min = Get_Float_Parameter("MinRange");
		if (min < range && range < max && (obj->As_SmartGameObj()->Is_Obj_Visible(FinalTarget->As_PhysicalGameObj())))
		{
			ActionParamsStruct params;
			params.Set_Basic(this,(float)current_priority, 100);
			if (Adjust_For_Infantry(FinalTarget))
			{
				params.AttackObject = NULL;
				params.Set_Attack(targetPos, range, 0, true);
				params.AttackCheckBlocked = false;
			}
			else
			{
				params.Set_Attack(FinalTarget, range, 0, true);
				params.AttackCheckBlocked = false;
			}
			Commands->Action_Attack(obj, params);
			current_priority++;
			attacking = true;
		}
		else
		{
			Commands->Action_Reset(obj, 100);
			attacking = false;
			targetID = 0;
			current_priority = 10;
		}
	}
}

GameObject* RA_Base_Defense_Simple::Select_Target(GameObject *obj, GameObject *target)
{
	//Targets passed into Select_Target have already been
	//checked for "infantry in vehicle" by Set_Target
	if (!(target)) //First see if there is a valid target
	{
		return Commands->Find_Object(targetID); //no need to test an invalid target
	}
	GameObject *old = Commands->Find_Object(targetID);
	if (!(old)) //is there a current target?
	{
		return target; //no need to test an invalid target
	}
	int toggle = Get_Int_Parameter("TargetMode");
	switch(toggle)
	{
	case 0: //Case 0 prioritizes vehicles over infantry
		if (old->As_SoldierGameObj() && !(target->As_SoldierGameObj()))
		{
			return target; //new target is a vehicle
		}
		else if (!(old->As_SoldierGameObj()) && target->As_SoldierGameObj())
		{
			return old; //old target is a vehicle
		}
		else
		{
			return Tie_Breaker(obj, target, old); //both targets the same, let tie breaker decide
		}
		break;//just to be safe
	case 1: //Case 1 prioritizes infantry over vehicles
		if (old->As_SoldierGameObj() && !(target->As_SoldierGameObj()))
		{
			if (Get_Vehicle(old))// Did the old target get in a vehicle?
			{
				return Tie_Breaker(obj, target, old); //both vehicle
			}
			else
			{
				return old; //old soldier
			}
		}
		else if (!(old->As_SoldierGameObj()) && target->As_SoldierGameObj())
		{
			if (Get_Vehicle(target))// Did the new target get in a vehicle? 
			{
				return Tie_Breaker(obj, target, old); //both vehicle 
			}
			//Needs to pass in the vehicle instead of old?
			else
			{
				return target; //target soldier
			}
		}
		else
		{
			return Tie_Breaker(obj, target, old); //both targets the same, let tie breaker decide
		}
		break; //just to be safe
	default: //Default doesn't prioritize target type
		return Tie_Breaker(obj, target, old); //all people are equal in my gun sights
		break; //yes, being safe still
	}
}

GameObject* RA_Base_Defense_Simple::Set_Target(GameObject *target)
{
	if (target) //confirm non-NULL pointer
	{
		if (Commands->Is_A_Star(target)) //Is it a player?
		{
			GameObject *vehicle = Get_Vehicle(target); 
			if (vehicle) //Are they in a vehicle
			{
				if ((Get_Vehicle_Mode(target) == VEHICLE_TYPE_FLYING) && !Is_Script_Attached(target,"JFW_Submarine"))
				{
					if (!(Get_Int_Parameter("AntiAir")))
					{
						return 0; //Ignore the target
					}
				}
				else
				{
					if (!(Get_Int_Parameter("AntiGround")))
					{
						return 0; //Ignore the target
					}
				}
				return vehicle; //if so, return vehicle
			}
			else
			{
				if ((Get_Vehicle_Mode(target) == VEHICLE_TYPE_FLYING) && !Is_Script_Attached(target,"JFW_Submarine"))
				{
					if (!(Get_Int_Parameter("AntiAir")))
					{
						return 0; //Ignore the target
					}
				}
				else
				{
					if (!(Get_Int_Parameter("AntiGround")))
					{
						return 0; //Ignore the target
					}
				}
				return target; //if not, return player
			}
		}
		return target; //nothing left but the target
	}
	return 0;
}

GameObject* RA_Base_Defense_Simple::Tie_Breaker(GameObject *obj, GameObject *new_target, GameObject *old_target)
{
	//This function compiles several methods of breaking ties
	//-lower health gives higher rating
	//-closer distance gives higher rating
	//-being player gives higher rating
	float old_rating = 0;
	float new_rating = 0;
	//Player Evaluation
	if (Commands->Is_A_Star(old_target))
	{
		old_rating += 100; //automatic +100 for being human
	}
	if (Commands->Is_A_Star(new_target))
	{
		new_rating += 100; //automatic +100 for being human
	}
	//Health Evaluation
	GameObject *old = old_target;
	GameObject *target = new_target;
	//Health needs to be tested for vehicle being driven, if it exists
	if (Get_Vehicle(old_target))
	{
		old = Get_Vehicle(old_target);
	}
	if (Get_Vehicle(new_target))
	{
		target = Get_Vehicle(old_target);
	}
	float new_max = Commands->Get_Max_Health(target);
	float old_max = Commands->Get_Max_Health(old);
	float new_percent = 0; //set to minimum
	float old_percent = 0; //set to minimum
	if (new_max > 0) //just to be sure
	{
		new_percent = ((100 * Commands->Get_Health(target)) / (new_max));
	}
	else
	{
		new_percent += 100; //If maximum health is 0, always at 100% health
	}
	if (old_max > 0) //just to be sure
	{
		old_percent = ((100 * Commands->Get_Health(old)) / (old_max));
	}
	else
	{
		old_percent += 100; //If maximum health is 0, always at 100% health
	}
	new_max = Commands->Get_Max_Shield_Strength(target);
	old_max = Commands->Get_Max_Shield_Strength(old);
	if (new_max > 0) //just to be sure
	{
		new_percent += ((100 * Commands->Get_Health(target)) / (new_max)); //Add in shield percent
	}
	else
	{
		new_percent += 100; //If maximum shield is 0, always at 100% shield
	}
	if (old_max > 0) //just to be sure
	{
		old_percent += ((100 * Commands->Get_Health(old)) / (old_max)); //Add in shield percent
	}
	else
	{
		old_percent += 100; //If maximum shield is 0, always at 100% shield
	}
	old_rating += 200 - old_percent;
	new_rating += 200 - new_percent;
	//Range Evaluation
	float max_range = Get_Float_Parameter("MaxRange");
	float min_range = Get_Float_Parameter("MinRange");
	Vector3 obj_pos = Commands->Get_Position(obj);
	Vector3 new_pos = Commands->Get_Position(new_target);
	Vector3 old_pos = Commands->Get_Position(old_target);
	float new_range = Commands->Get_Distance(obj_pos, new_pos);
	float old_range = Commands->Get_Distance(obj_pos, old_pos);
	if (new_range < max_range && new_range > min_range)
	{
		new_rating += max_range - (new_range - min_range); //not sure if this is good
	}
	if (old_range < max_range && old_range > min_range)
	{
		old_rating += max_range - (old_range - min_range);
	}
	//Final Evaluation
	float max_potential = 300 + max_range; //300 = 100 from Player vs AI + 200 from health
	if (old_rating > new_rating)
	{
		return old_target;
	}
	else if (old_rating < new_rating)
	{
		float test = new_rating - old_rating;
		if (test > (max_potential * 0.1)) //new target must be at least 10% greater priority
		{
			return new_target;
		}
		else
		{
			return old_target;
		}
	}
	else
	{
		return old_target; //rating tie, don't change targets
	}
}

bool RA_Base_Defense_Simple::Adjust_For_Infantry(GameObject *target)
{
	if (Get_Int_Parameter("AdjustAim"))
	{
		if (target->As_SoldierGameObj())
		{
			return true;
		}
	}
	//No need to adjust if parameter says not to
	return false;
}

/////////////////RA_Base_Defense_Powered/////////////////

void RA_Base_Defense_Powered::Created(GameObject *obj)
{
	//Initialize Variables
	targetID = 0;
	attacking = false;
	current_priority = 10;
	attack_timer = 1.0f;
	loop_count = 0;
	//Standard AI Startup Junk
	Commands->Enable_Enemy_Seen(obj, true);
	Commands->Enable_Vehicle_Transitions(obj, false);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Start_Timer(obj,this,1.0,1);
	Commands->Start_Timer(obj,this,attack_timer,11);
}

void RA_Base_Defense_Powered::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (!damager || amount <= 0)
	{
		return;
	}
	if (Is_Base_Powered(Get_Object_Type(obj)))
	{
		if ((Get_Vehicle_Mode(damager) == VEHICLE_TYPE_FLYING) && !Is_Script_Attached(damager,"JFW_Submarine"))
		{
			if (!(Get_Int_Parameter("AntiAir")))
			{
				return; //Ignore the target
			}
		}
		else
		{
			if (!(Get_Int_Parameter("AntiGround")))
			{
				return; //Ignore the target
			}
		}
		if (targetID) //check for current target
		{
			unsigned long newID = Commands->Get_ID(damager);
			if (targetID == newID)
			{
				return; //already firing at target
			}
		}
		int objID = Commands->Get_Player_Type(obj);
		int damagerID = Commands->Get_Player_Type(damager);
		if (objID != damagerID) //Make sure an enemy attacked
		{
			if (Commands->Get_Health(damager))
			{
				Attack(obj, damager);
			}
		}
	}
	else
	{
		Commands->Enable_Enemy_Seen(obj,false);
		//should cause the defense to stop attacking if the power goes low
		Commands->Action_Reset(obj, 100); //I have no idea what this number does...
		attacking = false;
		current_priority = 10;
	}
}
	
void RA_Base_Defense_Powered::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	if ((Get_Vehicle_Mode(enemy) == VEHICLE_TYPE_FLYING) && !Is_Script_Attached(enemy,"JFW_Submarine"))
	{
		if (!(Get_Int_Parameter("AntiAir")))
		{
			return; //Ignore the target
		}
	}
	else
	{
		if (!(Get_Int_Parameter("AntiGround")))
		{
			return; //Ignore the target
		}
	}
	if (targetID) //check for current target
	{
		unsigned long newID = Commands->Get_ID(enemy);
		if (targetID == newID)
		{
			return; //already firing at target
		}
	}
	if (Commands->Get_Health(enemy))
	{
		Attack(obj,enemy);
	}
}

void RA_Base_Defense_Powered::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1)
	{
		if (Is_Base_Powered(Get_Object_Type(obj)))
		{
			Commands->Enable_Enemy_Seen(obj,true);
		}
		else
		{
			Commands->Enable_Enemy_Seen(obj,false);
			//should cause the defense to stop attacking if the power goes low
			Commands->Action_Reset(obj, 100); //I have no idea what this number does...
			attacking = false;
			current_priority = 10;
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
	if (number != 11)
	{
		return; //If its not the timer we're interested in, end now
	}
	bool call_attack = true;
	if (!attacking && Is_Base_Powered(Get_Object_Type(obj)))	//skip facing action if attacking
	{
		if (loop_count > 4) //only face new direction every 5th loop
		{
			Vector3 pos = Commands->Get_Position(obj); 
			switch (Commands->Get_Random_Int(0,8))
			{
				case 0:
					pos.X += 10;
					pos.Y -= 10;
					break;
				case 1:
					pos.X -= 10;
					pos.Y -= 10;
					break;
				case 2:
					pos.X += 10;
					pos.Y += 10;
					break;
				case 3:
					pos.X -= 10;
					pos.Y += 10;
					break;
				case 4:
					pos.X += 10;
					break;
				case 5:
					pos.X -= 10;
					break;
				case 6:
					pos.Y += 10;
					break;
				case 7:
					pos.Y -= 10;
					break;
				default: 
					pos.X += 10;
					break;
			}
			ActionParamsStruct params;
			params.Set_Basic(this, 1, 30); //priority low
			params.Set_Face_Location(pos, 6);
			Commands->Action_Face_Location(obj, params);
		}
	}
	else
	{
		attacking = false;
		current_priority = 10;
		GameObject *target = Commands->Find_Object(targetID);
		if (!(target)) //If target invalid, end now
		{
			targetID = 0;
			call_attack = false;
		}
		else
		{
			int TargetType = Commands->Get_Player_Type(target);
			int myType = Commands->Get_Player_Type(obj);
			if (myType == TargetType) //Check old target for team change
			{
				targetID = 0; //old target a friend now
				call_attack = false;
			}
			//Check Range
			Vector3 objPos = Commands->Get_Position(obj);
			Vector3 targetPos = Commands->Get_Position(target);
			float range = Commands->Get_Distance(objPos, targetPos);
			float max = Get_Float_Parameter("MaxRange");
			float min = Get_Float_Parameter("MinRange");
			if (range < min || range > max)
			{
				targetID = 0; //old target out of range
				call_attack = false;
			}
			//Check Settings
			if (((Get_Vehicle_Mode(target) == VEHICLE_TYPE_FLYING) || (Get_Vehicle_Mode(Get_Vehicle(target)) == VEHICLE_TYPE_FLYING)) && !Is_Script_Attached(target,"JFW_Submarine"))
			{
				if (!(Get_Int_Parameter("AntiAir")))
				{
					targetID = 0; //old target not targetable
					call_attack = false;
				}
			}
			else
			{
				if (!(Get_Int_Parameter("AntiGround")))
				{
					targetID = 0; //old target not targetable
					call_attack = false;
				}
			}
		}
		if (call_attack && obj->As_DamageableGameObj()->Is_Enemy(target->As_DamageableGameObj())) //if attack and if call_attack was not turned false
		{
			if (Commands->Get_Health(target))
			{
				Attack(obj, target);
			}
		}
		else 
			Commands->Action_Reset(obj, 100);
	}
	if (loop_count > 4)
	{
		loop_count = 0; //reset counter
	}
	loop_count++;
	Commands->Start_Timer(obj, this, attack_timer, 11);
}

void RA_Base_Defense_Powered::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id == 100) //Attack completed
	{
		attacking = false;
		targetID = 0;
		current_priority = 10;
	}
}

void RA_Base_Defense_Powered::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&targetID,4,1);
	Auto_Save_Variable(&attacking,4,2);
	Auto_Save_Variable(&current_priority,4,3);
	Auto_Save_Variable(&attack_timer,4,4);
	Auto_Save_Variable(&loop_count,4,5);
}

void RA_Base_Defense_Powered::Attack(GameObject *obj, GameObject *target)
{
	/**************Evaluate Target Threat****************/
	GameObject *FinalTarget = Set_Target(target);
	if (FinalTarget) //Was there a non-NULL return from Set_Target?
	{
		FinalTarget = Select_Target(obj, FinalTarget); //Was Rate_Target
	}
	/****************Execute Final Attack****************/
	if (FinalTarget && FinalTarget->As_DamageableGameObj()->Is_Enemy(obj->As_DamageableGameObj()))
	{
		targetID = Commands->Get_ID(FinalTarget); //remember new target, or re-remember old target
		Vector3 objPos = Commands->Get_Position(obj);
		Vector3 targetPos = Commands->Get_Position(FinalTarget);
		float range = Commands->Get_Distance(objPos, targetPos);
		float max = Get_Float_Parameter("MaxRange");
		float min = Get_Float_Parameter("MinRange");
		if (min < range && range < max && (obj->As_SmartGameObj()->Is_Obj_Visible(FinalTarget->As_PhysicalGameObj())))
		{
			ActionParamsStruct params;
			params.Set_Basic(this,(float)current_priority, 100);
			if (Adjust_For_Infantry(FinalTarget))
			{
				params.AttackObject = NULL;
				params.Set_Attack(targetPos, range, 0, true);
				params.AttackCheckBlocked = false;
			}
			else
			{
				params.Set_Attack(FinalTarget, range, 0, true);
				params.AttackCheckBlocked = false;
			}
			Commands->Action_Attack(obj, params);
			current_priority++;
			attacking = true;
		}
		else
		{
			Commands->Action_Reset(obj, 100);
			attacking = false;
			targetID = 0;
			current_priority = 10;
		}
	}
}

GameObject* RA_Base_Defense_Powered::Select_Target(GameObject *obj, GameObject *target)
{
	//Targets passed into Select_Target have already been
	//checked for "infantry in vehicle" by Set_Target
	if (!(target)) //First see if there is a valid target
	{
		return Commands->Find_Object(targetID);; //no need to test an invalid target
	}
	GameObject *old = Commands->Find_Object(targetID);
	if (!(old)) //is there a current target?
	{
		return target; //no need to test an invalid target
	}
	int toggle = Get_Int_Parameter("TargetMode");
	switch(toggle)
	{
	case 0: //Case 0 prioritizes vehicles over infantry
		if (old->As_SoldierGameObj() && !(target->As_SoldierGameObj()))
		{
			return target; //new target is a vehicle
		}
		else if (!(old->As_SoldierGameObj()) && target->As_SoldierGameObj())
		{
			return old; //old target is a vehicle
		}
		else
		{
			return Tie_Breaker(obj, target, old); //both targets the same, let tie breaker decide
		}
		break;//just to be safe
	case 1: //Case 1 prioritizes infantry over vehicles
		if (old->As_SoldierGameObj() && !(target->As_SoldierGameObj()))
		{
			if (Get_Vehicle(old))// Did the old target get in a vehicle?
			{
				return Tie_Breaker(obj, target, old); //both vehicle
			}
			else
			{
				return old; //old soldier
			}
		}
		else if (!(old->As_SoldierGameObj()) && target->As_SoldierGameObj())
		{
			if (Get_Vehicle(target))// Did the new target get in a vehicle? 
			{
				return Tie_Breaker(obj, target, old); //both vehicle 
			}
			//Needs to pass in the vehicle instead of old?
			else
			{
				return target; //target soldier
			}
		}
		else
		{
			return Tie_Breaker(obj, target, old); //both targets the same, let tie breaker decide
		}
		break; //just to be safe
	default: //Default doesn't prioritize target type
		return Tie_Breaker(obj, target, old); //all people are equal in my gun sights
		break; //yes, being safe still
	}
}

GameObject* RA_Base_Defense_Powered::Set_Target(GameObject *target)
{
	if (target) //confirm non-NULL pointer
	{
		if (Commands->Is_A_Star(target)) //Is it a player?
		{
			GameObject *vehicle = Get_Vehicle(target); 
			if (vehicle) //Are they in a vehicle
			{
				if ((Get_Vehicle_Mode(target) == VEHICLE_TYPE_FLYING) && !Is_Script_Attached(target,"JFW_Submarine"))
				{
					if (!(Get_Int_Parameter("AntiAir")))
					{
						return 0; //Ignore the target
					}
				}
				else
				{
					if (!(Get_Int_Parameter("AntiGround")))
					{
						return 0; //Ignore the target
					}
				}
				return vehicle; //if so, return vehicle
			}
			else
			{
				if ((Get_Vehicle_Mode(target) == VEHICLE_TYPE_FLYING) && !Is_Script_Attached(target,"JFW_Submarine"))
				{
					if (!(Get_Int_Parameter("AntiAir")))
					{
						return 0; //Ignore the target
					}
				}
				else
				{
					if (!(Get_Int_Parameter("AntiGround")))
					{
						return 0; //Ignore the target
					}
				}
				return target; //if not, return player
			}
		}
		return target; //nothing left but the target
	}
	return 0;
}

GameObject* RA_Base_Defense_Powered::Tie_Breaker(GameObject *obj, GameObject *new_target, GameObject *old_target)
{
	//This function compiles several methods of breaking ties
	//-lower health gives higher rating
	//-closer distance gives higher rating
	//-being player gives higher rating
	float old_rating = 0;
	float new_rating = 0;
	//Player Evaluation
	if (Commands->Is_A_Star(old_target))
	{
		old_rating += 100; //automatic +100 for being human
	}
	if (Commands->Is_A_Star(new_target))
	{
		new_rating += 100; //automatic +100 for being human
	}
	//Health Evaluation
	GameObject *old = old_target;
	GameObject *target = new_target;
	//Health needs to be tested for vehicle being driven, if it exists
	if (Get_Vehicle(old_target))
	{
		old = Get_Vehicle(old_target);
	}
	if (Get_Vehicle(new_target))
	{
		target = Get_Vehicle(old_target);
	}
	float new_max = Commands->Get_Max_Health(target);
	float old_max = Commands->Get_Max_Health(old);
	float new_percent = 0; //set to minimum
	float old_percent = 0; //set to minimum
	if (new_max > 0) //just to be sure
	{
		new_percent = ((100 * Commands->Get_Health(target)) / (new_max));
	}
	else
	{
		new_percent += 100; //If maximum health is 0, always at 100% health
	}
	if (old_max > 0) //just to be sure
	{
		old_percent = ((100 * Commands->Get_Health(old)) / (old_max));
	}
	else
	{
		old_percent += 100; //If maximum health is 0, always at 100% health
	}
	new_max = Commands->Get_Max_Shield_Strength(target);
	old_max = Commands->Get_Max_Shield_Strength(old);
	if (new_max > 0) //just to be sure
	{
		new_percent += ((100 * Commands->Get_Health(target)) / (new_max)); //Add in shield percent
	}
	else
	{
		new_percent += 100; //If maximum shield is 0, always at 100% shield
	}
	if (old_max > 0) //just to be sure
	{
		old_percent += ((100 * Commands->Get_Health(old)) / (old_max)); //Add in shield percent
	}
	else
	{
		old_percent += 100; //If maximum shield is 0, always at 100% shield
	}
	old_rating += 200 - old_percent;
	new_rating += 200 - new_percent;
	//Range Evaluation
	float max_range = Get_Float_Parameter("MaxRange");
	float min_range = Get_Float_Parameter("MinRange");
	Vector3 obj_pos = Commands->Get_Position(obj);
	Vector3 new_pos = Commands->Get_Position(new_target);
	Vector3 old_pos = Commands->Get_Position(old_target);
	float new_range = Commands->Get_Distance(obj_pos, new_pos);
	float old_range = Commands->Get_Distance(obj_pos, old_pos);
	if (new_range < max_range && new_range > min_range)
	{
		new_rating += max_range - (new_range - min_range); //not sure if this is good
	}
	if (old_range < max_range && old_range > min_range)
	{
		old_rating += max_range - (old_range - min_range);
	}
	//Final Evaluation
	float max_potential = 300 + max_range; //300 = 100 from Player vs AI + 200 from health
	if (old_rating > new_rating)
	{
		return old_target;
	}
	else if (old_rating < new_rating)
	{
		float test = new_rating - old_rating;
		if (test > (max_potential * 0.1)) //new target must be at least 10% greater priority
		{
			return new_target;
		}
		else
		{
			return old_target;
		}
	}
	else
	{
		return old_target; //rating tie, don't change targets
	}
}

bool RA_Base_Defense_Powered::Adjust_For_Infantry(GameObject *target)
{
	if (Get_Int_Parameter("AdjustAim"))
	{
		if (target->As_SoldierGameObj())
		{
			return true;
		}
	}
	//No need to adjust if parameter says not to
	return false;
}

/////////////////RA_Base_Defense_Chargeup/////////////////

void RA_Base_Defense_Chargeup::Created(GameObject *obj)
{
	//Initialize Variables
	targetID = 0;
	damageID = 0;
	damagerID = 0;
	attacking = false;
	current_priority = 10;
	attack_timer = 1.0f;
	loop_count = 0;
	//Standard AI Startup Junk
	Commands->Enable_Enemy_Seen(obj, true);
	Commands->Enable_Vehicle_Transitions(obj, false);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Start_Timer(obj,this,1.0,1);
	Commands->Start_Timer(obj,this,attack_timer,11);
}

void RA_Base_Defense_Chargeup::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (Commands->Get_Health(Commands->Find_Object(damageID)))
	{
		Attack(Commands->Find_Object(damagerID),Commands->Find_Object(damageID));
	}
}

void RA_Base_Defense_Chargeup::Attack_Charged(GameObject *obj, GameObject *target)
{
	damagerID = Commands->Get_ID(obj);
	damageID = Commands->Get_ID(target);
	if (Get_Current_Weapon_Definition(obj)->ClipSize == Get_Current_Bullets(obj))
	{
		Commands->Set_Animation(obj,Get_Parameter("ChargeAnim"),false,0,0,-1,false);
		Commands->Create_Sound(Get_Parameter("ChargeSound"),Commands->Get_Position(obj),obj);
	}
	else
	{
		if (Commands->Get_Health(Commands->Find_Object(damageID)))
		{
			Attack(Commands->Find_Object(damagerID),Commands->Find_Object(damageID));
		}
	}
}

void RA_Base_Defense_Chargeup::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (Is_Base_Powered(Get_Object_Type(obj)))
	{
		if ((Get_Vehicle_Mode(damager) == VEHICLE_TYPE_FLYING) && !Is_Script_Attached(damager,"JFW_Submarine"))
		{
			if (!(Get_Int_Parameter("AntiAir")))
			{
				return; //Ignore the target
			}
		}
		else
		{
			if (!(Get_Int_Parameter("AntiGround")))
			{
				return; //Ignore the target
			}
		}
		if (targetID) //check for current target
		{
			unsigned long newID = Commands->Get_ID(damager);
			if (targetID == newID)
			{
				return; //already firing at target
			}
		}
		int objID = Commands->Get_Player_Type(obj);
		int damagerID = Commands->Get_Player_Type(damager);
		if (objID != damagerID) //Make sure an enemy attacked
		{
			Attack_Charged(obj, damager);
		}
	}
	else
	{
		Commands->Enable_Enemy_Seen(obj,false);
		//should cause the defense to stop attacking if the power goes low
		Commands->Action_Reset(obj, 100); //I have no idea what this number does...
		attacking = false;
		current_priority = 10;
	}
}
	
void RA_Base_Defense_Chargeup::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	if ((Get_Vehicle_Mode(enemy) == VEHICLE_TYPE_FLYING) && !Is_Script_Attached(enemy,"JFW_Submarine"))
	{
		if (!(Get_Int_Parameter("AntiAir")))
		{
			return; //Ignore the target
		}
	}
	else
	{
		if (!(Get_Int_Parameter("AntiGround")))
		{
			return; //Ignore the target
		}
	}
	if (targetID) //check for current target
	{
		unsigned long newID = Commands->Get_ID(enemy);
		if (targetID == newID)
		{
			return; //already firing at target
		}
	}
	Attack_Charged(obj,enemy);
}

void RA_Base_Defense_Chargeup::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1)
	{
		if (Is_Base_Powered(Get_Object_Type(obj)))
		{
			Commands->Enable_Enemy_Seen(obj,true);
		}
		else
		{
			Commands->Enable_Enemy_Seen(obj,false);
			//should cause the defense to stop attacking if the power goes low
			Commands->Action_Reset(obj, 100); //I have no idea what this number does...
			attacking = false;
			current_priority = 10;
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
	if (number != 11)
	{
		return; //If its not the timer we're interested in, end now
	}
	bool call_attack = true;
	if (!attacking)	//skip facing action if attacking
	{
		if (loop_count > 4) //only face new direction every 5th loop
		{
			Vector3 pos = Commands->Get_Position(obj); 
			switch (Commands->Get_Random_Int(0,8))
			{
				case 0:
					pos.X += 10;
					pos.Y -= 10;
					break;
				case 1:
					pos.X -= 10;
					pos.Y -= 10;
					break;
				case 2:
					pos.X += 10;
					pos.Y += 10;
					break;
				case 3:
					pos.X -= 10;
					pos.Y += 10;
					break;
				case 4:
					pos.X += 10;
					break;
				case 5:
					pos.X -= 10;
					break;
				case 6:
					pos.Y += 10;
					break;
				case 7:
					pos.Y -= 10;
					break;
				default: 
					pos.X += 10;
					break;
			}
			ActionParamsStruct params;
			params.Set_Basic(this, 1, 30); //priority low
			params.Set_Face_Location(pos, 6);
			Commands->Action_Face_Location(obj, params);
		}
	}
	else
	{
		//force ceasefire before checking to continue (helps fight client glitches... I hope...)
		Commands->Action_Reset(obj, 100); //I have no idea what this number does...
		attacking = false;
		current_priority = 10;
		GameObject *target = Commands->Find_Object(targetID);
		if (!(target)) //If target invalid, end now
		{
			targetID = 0;
			call_attack = false;
		}
		else
		{
			int TargetType = Commands->Get_Player_Type(target);
			int myType = Commands->Get_Player_Type(obj);
			if (myType == TargetType) //Check old target for team change
			{
				targetID = 0; //old target a friend now
				call_attack = false;
			}
			//Check Range
			Vector3 objPos = Commands->Get_Position(obj);
			Vector3 targetPos = Commands->Get_Position(target);
			float range = Commands->Get_Distance(objPos, targetPos);
			float max = Get_Float_Parameter("MaxRange");
			float min = Get_Float_Parameter("MinRange");
			if (range < min || range > max)
			{
				targetID = 0; //old target out of range
				call_attack = false;
			}
			//Check Settings
			if (((Get_Vehicle_Mode(target) == VEHICLE_TYPE_FLYING) || (Get_Vehicle_Mode(Get_Vehicle(target)) == VEHICLE_TYPE_FLYING)) && !Is_Script_Attached(target,"JFW_Submarine"))
			{
				if (!(Get_Int_Parameter("AntiAir")))
				{
					targetID = 0; //old target not targetable
					call_attack = false;
				}
			}
			else
			{
				if (!(Get_Int_Parameter("AntiGround")))
				{
					targetID = 0; //old target not targetable
					call_attack = false;
				}
			}
		}
		if (call_attack && obj->As_DamageableGameObj()->Is_Enemy(target->As_DamageableGameObj())) //if attack and if call_attack was not turned false
		{
			Attack_Charged(obj, target);
		}
	}
	if (loop_count > 4)
	{
		loop_count = 0; //reset counter
	}
	loop_count++;
	Commands->Start_Timer(obj, this, attack_timer, 11);
}

void RA_Base_Defense_Chargeup::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id == 100) //Attack completed
	{
		attacking = false;
		targetID = 0;
		current_priority = 10;
	}
}

void RA_Base_Defense_Chargeup::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&targetID,4,1);
	Auto_Save_Variable(&attacking,4,2);
	Auto_Save_Variable(&current_priority,4,3);
	Auto_Save_Variable(&attack_timer,4,4);
	Auto_Save_Variable(&loop_count,4,5);
}

void RA_Base_Defense_Chargeup::Attack(GameObject *obj, GameObject *target)
{
	/**************Evaluate Target Threat****************/
	GameObject *FinalTarget = Set_Target(target);
	if (FinalTarget) //Was there a non-NULL return from Set_Target?
	{
		FinalTarget = Select_Target(obj, FinalTarget); //Was Rate_Target
	}
	/****************Execute Final Attack****************/
	if (FinalTarget && FinalTarget->As_DamageableGameObj()->Is_Enemy(obj->As_DamageableGameObj()))
	{
		targetID = Commands->Get_ID(FinalTarget); //remember new target, or re-remember old target
		Vector3 objPos = Commands->Get_Position(obj);
		Vector3 targetPos = Commands->Get_Position(FinalTarget);
		float range = Commands->Get_Distance(objPos, targetPos);
		float max = Get_Float_Parameter("MaxRange");
		float min = Get_Float_Parameter("MinRange");
		if (min < range && range < max && (obj->As_SmartGameObj()->Is_Obj_Visible(FinalTarget->As_PhysicalGameObj())))
		{
			ActionParamsStruct params;
			params.Set_Basic(this,(float)current_priority, 100);
			if (Adjust_For_Infantry(FinalTarget))
			{
				params.Set_Attack(targetPos, range, 0, true);
				params.AttackCheckBlocked = false;
			}
			else
			{
				params.Set_Attack(FinalTarget, range, 0, true);
				params.AttackCheckBlocked = false;
			}
			Commands->Action_Attack(obj, params);
			current_priority++;
			attacking = true;
		}
		else
		{
			attacking = false;
			targetID = 0;
			current_priority = 10;
		}
	}
}

GameObject* RA_Base_Defense_Chargeup::Select_Target(GameObject *obj, GameObject *target)
{
	//Targets passed into Select_Target have already been
	//checked for "infantry in vehicle" by Set_Target
	if (!(target)) //First see if there is a valid target
	{
		return Commands->Find_Object(targetID);; //no need to test an invalid target
	}
	GameObject *old = Commands->Find_Object(targetID);
	if (!(old)) //is there a current target?
	{
		return target; //no need to test an invalid target
	}
	int toggle = Get_Int_Parameter("TargetMode");
	switch(toggle)
	{
	case 0: //Case 0 prioritizes vehicles over infantry
		if (old->As_SoldierGameObj() && !(target->As_SoldierGameObj()))
		{
			return target; //new target is a vehicle
		}
		else if (!(old->As_SoldierGameObj()) && target->As_SoldierGameObj())
		{
			return old; //old target is a vehicle
		}
		else
		{
			return Tie_Breaker(obj, target, old); //both targets the same, let tie breaker decide
		}
		break;//just to be safe
	case 1: //Case 1 prioritizes infantry over vehicles
		if (old->As_SoldierGameObj() && !(target->As_SoldierGameObj()))
		{
			if (Get_Vehicle(old))// Did the old target get in a vehicle?
			{
				return Tie_Breaker(obj, target, old); //both vehicle
			}
			else
			{
				return old; //old soldier
			}
		}
		else if (!(old->As_SoldierGameObj()) && target->As_SoldierGameObj())
		{
			if (Get_Vehicle(target))// Did the new target get in a vehicle? 
			{
				return Tie_Breaker(obj, target, old); //both vehicle 
			}
			//Needs to pass in the vehicle instead of old?
			else
			{
				return target; //target soldier
			}
		}
		else
		{
			return Tie_Breaker(obj, target, old); //both targets the same, let tie breaker decide
		}
		break; //just to be safe
	default: //Default doesn't prioritize target type
		return Tie_Breaker(obj, target, old); //all people are equal in my gun sights
		break; //yes, being safe still
	}
}

GameObject* RA_Base_Defense_Chargeup::Set_Target(GameObject *target)
{
	if (target) //confirm non-NULL pointer
	{
		if (Commands->Is_A_Star(target)) //Is it a player?
		{
			GameObject *vehicle = Get_Vehicle(target); 
			if (vehicle) //Are they in a vehicle
			{
				if ((Get_Vehicle_Mode(target) == VEHICLE_TYPE_FLYING) && !Is_Script_Attached(target,"JFW_Submarine"))
				{
					if (!(Get_Int_Parameter("AntiAir")))
					{
						return 0; //Ignore the target
					}
				}
				else
				{
					if (!(Get_Int_Parameter("AntiGround")))
					{
						return 0; //Ignore the target
					}
				}
				return vehicle; //if so, return vehicle
			}
			else
			{
				if ((Get_Vehicle_Mode(target) == VEHICLE_TYPE_FLYING) && !Is_Script_Attached(target,"JFW_Submarine"))
				{
					if (!(Get_Int_Parameter("AntiAir")))
					{
						return 0; //Ignore the target
					}
				}
				else
				{
					if (!(Get_Int_Parameter("AntiGround")))
					{
						return 0; //Ignore the target
					}
				}
				return target; //if not, return player
			}
		}
		return target; //nothing left but the target
	}
	return 0;
}

GameObject* RA_Base_Defense_Chargeup::Tie_Breaker(GameObject *obj, GameObject *new_target, GameObject *old_target)
{
	//This function compiles several methods of breaking ties
	//-lower health gives higher rating
	//-closer distance gives higher rating
	//-being player gives higher rating
	float old_rating = 0;
	float new_rating = 0;
	//Player Evaluation
	if (Commands->Is_A_Star(old_target))
	{
		old_rating += 100; //automatic +100 for being human
	}
	if (Commands->Is_A_Star(new_target))
	{
		new_rating += 100; //automatic +100 for being human
	}
	//Health Evaluation
	GameObject *old = old_target;
	GameObject *target = new_target;
	//Health needs to be tested for vehicle being driven, if it exists
	if (Get_Vehicle(old_target))
	{
		old = Get_Vehicle(old_target);
	}
	if (Get_Vehicle(new_target))
	{
		target = Get_Vehicle(old_target);
	}
	float new_max = Commands->Get_Max_Health(target);
	float old_max = Commands->Get_Max_Health(old);
	float new_percent = 0; //set to minimum
	float old_percent = 0; //set to minimum
	if (new_max > 0) //just to be sure
	{
		new_percent = ((100 * Commands->Get_Health(target)) / (new_max));
	}
	else
	{
		new_percent += 100; //If maximum health is 0, always at 100% health
	}
	if (old_max > 0) //just to be sure
	{
		old_percent = ((100 * Commands->Get_Health(old)) / (old_max));
	}
	else
	{
		old_percent += 100; //If maximum health is 0, always at 100% health
	}
	new_max = Commands->Get_Max_Shield_Strength(target);
	old_max = Commands->Get_Max_Shield_Strength(old);
	if (new_max > 0) //just to be sure
	{
		new_percent += ((100 * Commands->Get_Health(target)) / (new_max)); //Add in shield percent
	}
	else
	{
		new_percent += 100; //If maximum shield is 0, always at 100% shield
	}
	if (old_max > 0) //just to be sure
	{
		old_percent += ((100 * Commands->Get_Health(old)) / (old_max)); //Add in shield percent
	}
	else
	{
		old_percent += 100; //If maximum shield is 0, always at 100% shield
	}
	old_rating += 200 - old_percent;
	new_rating += 200 - new_percent;
	//Range Evaluation
	float max_range = Get_Float_Parameter("MaxRange");
	float min_range = Get_Float_Parameter("MinRange");
	Vector3 obj_pos = Commands->Get_Position(obj);
	Vector3 new_pos = Commands->Get_Position(new_target);
	Vector3 old_pos = Commands->Get_Position(old_target);
	float new_range = Commands->Get_Distance(obj_pos, new_pos);
	float old_range = Commands->Get_Distance(obj_pos, old_pos);
	if (new_range < max_range && new_range > min_range)
	{
		new_rating += max_range - (new_range - min_range); //not sure if this is good
	}
	if (old_range < max_range && old_range > min_range)
	{
		old_rating += max_range - (old_range - min_range);
	}
	//Final Evaluation
	float max_potential = 300 + max_range; //300 = 100 from Player vs AI + 200 from health
	if (old_rating > new_rating)
	{
		return old_target;
	}
	else if (old_rating < new_rating)
	{
		float test = new_rating - old_rating;
		if (test > (max_potential * 0.1)) //new target must be at least 10% greater priority
		{
			return new_target;
		}
		else
		{
			return old_target;
		}
	}
	else
	{
		return old_target; //rating tie, don't change targets
	}
}

bool RA_Base_Defense_Chargeup::Adjust_For_Infantry(GameObject *target)
{
	if (Get_Int_Parameter("AdjustAim"))
	{
		if (target->As_SoldierGameObj())
		{
			return true;
		}
	}
	//No need to adjust if parameter says not to
	return false;
}

/////////////////RA_Vehicle_Regen/////////////////

void RA_Vehicle_Regen::Created(GameObject *obj)
{
	healing = false;
}

void RA_Vehicle_Regen::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (!healing)
	{
		float max_total = Commands->Get_Max_Health(obj) + Commands->Get_Max_Shield_Strength(obj);
		float cur_total = Commands->Get_Health(obj) + Commands->Get_Shield_Strength(obj);
		if (cur_total < (max_total * Get_Float_Parameter("Percentage")))
		{
			Commands->Start_Timer(obj, this, Get_Float_Parameter("Time"), Get_Int_Parameter("TimerID"));
			healing = true;
		}
	}
}

void RA_Vehicle_Regen::Timer_Expired(GameObject *obj, int number)
{
	if (number == Get_Float_Parameter("TimerID"))
	{
		float max_total = Commands->Get_Max_Health(obj) + Commands->Get_Max_Shield_Strength(obj);
		float cur_total = Commands->Get_Health(obj) + Commands->Get_Shield_Strength(obj);
		if (cur_total < (max_total * Get_Float_Parameter("Percentage")))
		{
			float max_health = Commands->Get_Max_Health(obj);
			float health = Commands->Get_Health(obj);
			if (health < max_health) //Is the Health hurt?
			{
				Commands->Set_Health(obj, Commands->Get_Health(obj) + Get_Float_Parameter("Heal"));
			}
			else //Must be the shield then
			{
				Commands->Set_Shield_Strength(obj, Commands->Get_Shield_Strength(obj) + Get_Float_Parameter("Heal"));
			}
			Commands->Start_Timer(obj, this, Get_Float_Parameter("Time"), Get_Int_Parameter("TimerID"));
		}
		else
		{
			healing = false; //above percentage
		}
	}
}

void RA_Vehicle_Regen::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&healing,1,1);
}

///////////////////RA_Thief///////////////////

void RA_Thief::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == Get_Int_Parameter("Enter_Message"))
	{
		int enemy = param; //param will be team type to steal from
		int ally = Commands->Get_Player_Type(obj);
		if (ally == enemy)
		{
			return; //Don't bother to steal from self...
		}
		float theft = Steal_Team_Credits(Get_Float_Parameter("Percentage"), enemy);
		float credits = theft / Get_Team_Player_Count(ally);
		Commands->Give_Money(obj, credits, true);
		Remove_Script(obj, "RA_Thief"); //Strip off script after use
	}
}

///////////////////RA_Thief_Improved///////////////////

void RA_Thief_Improved::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == Get_Int_Parameter("Enter_Message"))
	{
		int enemy = param; //param will be team type to steal from
		int ally = Commands->Get_Player_Type(obj);
		if (ally == enemy)
		{
			return; //Don't bother to steal from self...
		}
		float theft = Steal_Team_Credits(Get_Float_Parameter("Percentage"), enemy);
		float credits = theft / Get_Team_Player_Count(ally);
		Commands->Give_Money(obj, credits, true);
		char str[100];
		sprintf(str,"Credits Stolen: %.0f\nCredits Given: %.0f",theft,credits);
		Send_Message_Team(ally,255,255,255,str);
		Create_2D_Sound_Team(Get_Parameter("Sound"),ally);
		Remove_Script(obj, "RA_Thief_Improved"); //Strip off script after use
	}
}

/////////////RA_Credit_Theft_Zone/////////////

void RA_Credit_Theft_Zone::Entered(GameObject *obj, GameObject *enterer)
{
	if (Is_Script_Attached(enterer, "RA_Thief") || Is_Script_Attached(enterer, "RA_Thief_Improved")) //Is it a thief?
	{
		if (!Get_Vehicle(enterer))
		{
	 		Commands->Send_Custom_Event(obj, enterer, Get_Int_Parameter("Enter_Message"), Get_Int_Parameter("Team"), 0);
		}
	}
}

/////////////RA_Credit_Theft_Zone_Timer/////////////

void RA_Credit_Theft_Zone_Timer::Entered(GameObject *obj, GameObject *enterer)
{
	if (Is_Script_Attached(enterer, "RA_Thief") || Is_Script_Attached(enterer, "RA_Thief_Improved"))
	{
		if (!Get_Vehicle(enterer))
		{
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Commands->Get_ID(enterer));
		}
	}
}

void RA_Credit_Theft_Zone_Timer::Timer_Expired(GameObject *obj, int number)
{
	GameObject *thief = Commands->Find_Object(number);
	if (thief)
	{
		if (!Get_Vehicle(thief) && obj->As_ScriptZoneGameObj()->Inside_Me(thief->As_SmartGameObj()))
		{
			Commands->Send_Custom_Event(obj, thief, Get_Int_Parameter("Enter_Message"), Get_Int_Parameter("Team"), 0);
		}
	}
}

///////////RA_DestroyNearest_OnDeath///////////

void RA_DestroyNearest_OnDeath::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *victim = 0;
	victim = Find_Nearest_Preset(Commands->Get_Position(obj), Get_Parameter("Preset_Name"));
	if (victim)
	{
		Commands->Destroy_Object(victim);
	}
}

/////////////////RA_Ore_Truck/////////////////

void RA_Ore_Truck::Created(GameObject *obj)
{
	scoops = 0;
	value = 0;
	harvesting = false;
	field = 0;
}

void RA_Ore_Truck::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == Get_Int_Parameter("Field_Enter"))
	{
		if (scoops < Get_Int_Parameter("Max_Capacity"))
		{
			field = param; //remember the field value
			if (!harvesting) //Only begin new anim if not harvesting
			{
				Commands->Set_Animation(obj, Get_Parameter("Harvest_Anim"), false, 0, 0, -1, false);
				scoops++; //take in scoop
				value += (field / Get_Int_Parameter("Max_Capacity")); //add scoop value to total value
			}
			harvesting = true; //now harvesting
		}
		else
		{
			harvesting = false; //truck full, can't harvest any more
			field = 0; //forget old field
		}
	}
	else if (type == Get_Int_Parameter("Field_Exit"))
	{
		if (harvesting)
		{
			harvesting = false;
		}
	}
	else if (type == Get_Int_Parameter("Deposit_Enter"))//Entered deposit zone
	{
		if (value) //only dump ore if there is ore to dump
		{
			Commands->Set_Animation(obj, Get_Parameter("Deliver_Anim"), false, 0, 0, -1, false); 
		}
	}
}

void RA_Ore_Truck::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, Get_Parameter("Harvest_Anim")))
	{
		obj->As_PhysicalGameObj()->Clear_Animation();
		Commands->Start_Timer(obj, this, 0.25, 95123); //wait 0.25 seconds to start new anim
	}
	else if (!_stricmp(animation_name, Get_Parameter("Deliver_Anim")))
	{
		//Ore delivered to refinery
		Commands->Give_Money(obj, value, true); //pass out the cash
#ifdef SSGM
		int PlayerID;
		GameObject *driver = Get_Vehicle_Driver(obj);
		float reward = 0.0f;
		if(driver)
		{
			PlayerID = Get_Player_ID( driver);
			if(value == 700)
			{
				reward = 5.0f;
			}
			else if(value == 1050)
			{
				reward = 7.0f;
			}
			else if(value == 1400)
			{
				reward = 10.0f;
			}
			StringClass msg;
			const char *str2 = Get_Team_Name(Get_Team(PlayerID));
			const char *str = Get_Player_Name_By_ID(PlayerID);
			msg.Format("%s dumped some Ore at the refinary, giving the %s %d additional credits each.", str, str2, (int)value);
			delete[] str2;
			delete[] str;
			SSGMGameLog::Log_Message(msg,"_OREDUMP");
			msg.Format("ppage %d Well done soldier, you have given your team mates an extra %d credits each", PlayerID, (int)value);
			Console_Input(msg);
			Commands->Give_Points(driver, reward, false);
		}
		Create_2D_WAV_Sound_Team("gsweep.wav",0);
#endif
		scoops = 0; //reset capacity
		value = 0; //reset value
	}
}

void RA_Ore_Truck::Timer_Expired(GameObject *obj, int number)
{
	if (number == 95123)
	{
		if (scoops < Get_Int_Parameter("Max_Capacity"))
		{
			if (harvesting) //is truck still in the zone
			{
				Commands->Set_Animation(obj, Get_Parameter("Harvest_Anim"), false, 0, 0, -1, false);
				scoops++; //take in scoop
				value += (field / Get_Int_Parameter("Max_Capacity")); //add now
				//Note: variable 'field' can be changed if new enter message was received between 'scoops'
				//This allows the truck to enter a new field type in middle of harvesting, 
				//Keeping the correct amount for old scoop, and receving new amount for new scoop
			}
			else
			{
				field = 0; //forget old field value
			}
		}
		else
		{
			harvesting = false;
			field = 0;
		}
	}
}

void RA_Ore_Truck::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&scoops,4,1);
	Auto_Save_Variable(&value,4,2);
	Auto_Save_Variable(&harvesting,1,3);
	Auto_Save_Variable(&field,4,4);
}

/////////////////RA_Ore_Truck_2/////////////////

void RA_Ore_Truck_2::Created(GameObject *obj)
{
	scoops = 0;
	value = 0;
	harvesting = false;
	field = 0;
}

void RA_Ore_Truck_2::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == Get_Int_Parameter("Field_Enter"))
	{
		if (scoops < Get_Int_Parameter("Max_Capacity"))
		{
			field = param; //remember the field value
			if (!harvesting) //Only begin new anim if not harvesting
			{
				Commands->Set_Animation(obj, Get_Parameter("Harvest_Anim"), false, 0, 0, -1, false);
				scoops++; //take in scoop
				Commands->Create_Sound(Get_Parameter("Harvest_Sound"),Commands->Get_Position(obj),obj);
				value += (field / Get_Int_Parameter("Max_Capacity")); //add scoop value to total value
			}
			harvesting = true; //now harvesting
		}
		else
		{
			harvesting = false; //truck full, can't harvest any more
			field = 0; //forget old field
		}
	}
	else if (type == Get_Int_Parameter("Field_Exit"))
	{
		if (harvesting)
		{
			harvesting = false;
		}
	}
	else if (type == Get_Int_Parameter("Deposit_Enter"))//Entered deposit zone
	{
		if (value) //only dump ore if there is ore to dump
		{
			Commands->Create_Sound(Get_Parameter("Dump_Sound"),Commands->Get_Position(obj),obj);
			Commands->Set_Animation(obj, Get_Parameter("Deliver_Anim"), false, 0, 0, -1, false); 
		}
	}
}

void RA_Ore_Truck_2::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, Get_Parameter("Harvest_Anim")))
	{
		obj->As_PhysicalGameObj()->Clear_Animation();
		Commands->Start_Timer(obj, this, 0.25, 95123); //wait 0.25 seconds to start new anim
	}
	else if (!_stricmp(animation_name, Get_Parameter("Deliver_Anim")))
	{
		//Ore delivered to refinery
		Commands->Give_Money(obj, value, true); //pass out the cash
#ifdef SSGM
		int PlayerID;
		GameObject *driver = Get_Vehicle_Driver(obj);
		float reward = 0.0f;
		if(driver)
		{
			PlayerID = Get_Player_ID( driver);
			if(value == 700)
			{
				reward = 5.0f;
			}
			else if(value == 1050)
			{
				reward = 7.0f;
			}
			else if(value == 1400)
			{
				reward = 10.0f;
			}
			StringClass msg;
			const char *str2 = Get_Team_Name(Get_Team(PlayerID));
			const char *str = Get_Player_Name_By_ID(PlayerID);
			msg.Format("%s dumped some Ore at the refinary, giving the %s %d additional credits each.", str, str2, (int)value);
			delete[] str2;
			delete[] str;
			SSGMGameLog::Log_Message(msg,"_OREDUMP");
			msg.Format("ppage %d Well done soldier, you have given your team mates an extra %d credits each", PlayerID, (int)value);
			Console_Input(msg);
			Commands->Give_Points(driver, reward, false);
		}
		Create_2D_WAV_Sound_Team("gsweep.wav",0);
#endif
		scoops = 0; //reset capacity
		value = 0; //reset value
	}
}

void RA_Ore_Truck_2::Timer_Expired(GameObject *obj, int number)
{
	if (number == 95123)
	{
		if (scoops < Get_Int_Parameter("Max_Capacity"))
		{
			if (harvesting) //is truck still in the zone
			{
				Commands->Set_Animation(obj, Get_Parameter("Harvest_Anim"), false, 0, 0, -1, false);
				scoops++; //take in scoop
				Commands->Create_Sound(Get_Parameter("Harvest_Sound"),Commands->Get_Position(obj),obj);
				value += (field / Get_Int_Parameter("Max_Capacity")); //add now
				//Note: variable 'field' can be changed if new enter message was received between 'scoops'
				//This allows the truck to enter a new field type in middle of harvesting, 
				//Keeping the correct amount for old scoop, and receving new amount for new scoop
			}
			else
			{
				field = 0; //forget old field value
			}
		}
		else
		{
			harvesting = false;
			field = 0;
		}
	}
}

void RA_Ore_Truck_2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&scoops,4,1);
	Auto_Save_Variable(&value,4,2);
	Auto_Save_Variable(&harvesting,1,3);
	Auto_Save_Variable(&field,4,4);
}

/////////////////RA_Ore_Field/////////////////

void RA_Ore_Field::Entered(GameObject *obj, GameObject *enterer)
{
	if (Is_Script_Attached(enterer, "RA_Ore_Truck") || Is_Script_Attached(enterer, "RA_Ore_Truck_2")) //Is this an Ore Truck?
	{
		Commands->Send_Custom_Event(obj, enterer, Get_Int_Parameter("Enter"), Get_Int_Parameter("Field_Value"), 0);
	}
}

void RA_Ore_Field::Exited(GameObject *obj, GameObject *exiter)
{
	if (Is_Script_Attached(exiter, "RA_Ore_Truck") || Is_Script_Attached(exiter, "RA_Ore_Truck_2")) //Is this an Ore Truck?
	{
		Commands->Send_Custom_Event(obj, exiter, Get_Int_Parameter("Exit"), 0, 0);
	}
}

/////////////RA_Ore_Delivery_Zone/////////////

void RA_Ore_Delivery_Zone::Entered(GameObject *obj, GameObject *enterer)
{
	if (Is_Script_Attached(enterer, "RA_Ore_Truck") || Is_Script_Attached(enterer, "RA_Ore_Truck_2")) //Is this an Ore Truck?
	{
		if (Commands->Get_Player_Type(enterer) == Get_Int_Parameter("Team")) //Is it a friendly truck?
		{
			Commands->Send_Custom_Event(obj, enterer, Get_Int_Parameter("Enter"), 0, 0);
		}
	}
}

//////////////RA_Vehicle_Team_Set//////////////

void RA_Vehicle_Team_Set::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		Remove_Script(obj, "RA_Vehicle_Team_Timer"); //strip timer script if it exists
	}
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (!Get_Vehicle_Occupant_Count(obj)) //If no one left in vehicle
		{
			Commands->Set_Player_Type(obj, Commands->Get_Player_Type(sender)); //set team type
			Attach_Script_Once(obj, "RA_Vehicle_Team_Timer", ""); //attach timer script if it doesn't exist
		}
	}
}

//////////////RA_Vehicle_Team_Timer//////////////

void RA_Vehicle_Team_Timer::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, 20, 32158);
}

void RA_Vehicle_Team_Timer::Timer_Expired(GameObject *obj, int number)
{
	if (number == 32158)
	{
		Commands->Set_Player_Type(obj, -2); //make unteamed
		Remove_Script(obj, "RA_Vehicle_Team_Timer"); //strip script (not really needed, but whatever)
	}
}

//////////////RA_Visible_Driver//////////////

void RA_Visible_Driver::Created(GameObject *obj)
{
	modelID = 0;
	driverID = 0;
	Commands->Start_Timer(obj, this, 2, 321789);
}

void RA_Visible_Driver::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!driverID)
		{
			if (modelID)
			{
				Commands->Destroy_Object(Commands->Find_Object(modelID));
			}
			Vector3 position = Commands->Get_Bone_Position(obj,Get_Parameter("BoneName"));
			GameObject *object = Commands->Create_Object("Invisible_Object", position);
			Commands->Attach_To_Object_Bone(object, obj, Get_Parameter("BoneName"));
			Commands->Send_Custom_Event(obj, sender, Get_Int_Parameter("Message"), Commands->Get_ID(object), 0);
			modelID = Commands->Get_ID(object);
			driverID = Commands->Get_ID(sender);
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (modelID)
		{
			Commands->Destroy_Object(Commands->Find_Object(modelID));
			modelID = 0;
		}
		if (Get_Vehicle_Occupant_Count(obj) == 0)
		{
			driverID = 0;
		}
	}
}

void RA_Visible_Driver::Timer_Expired(GameObject *obj, int number)
{
	if (number == 321789)
	{
		if (Is_Stealth(obj))
		{
			if (modelID)
			{
				Commands->Destroy_Object(Commands->Find_Object(modelID));
				modelID = 0;
			}
		}
		else
		{
			if (driverID)
			{
				if (!modelID)
				{
					Vector3 position = Commands->Get_Bone_Position(obj, Get_Parameter("BoneName"));
					GameObject *object = Commands->Create_Object("Invisible_Object", position);
					Commands->Attach_To_Object_Bone(object, obj, Get_Parameter("BoneName"));
					Commands->Send_Custom_Event(obj, Commands->Find_Object(driverID), Get_Int_Parameter("Message"), Commands->Get_ID(object), 0);
					modelID = Commands->Get_ID(object);
				}

			}
		}
		Commands->Start_Timer(obj, this, 2, 321789);
	}
}

void RA_Visible_Driver::Killed(GameObject *obj, GameObject *killer)
{
	if (modelID)
	{
		Commands->Destroy_Object(Commands->Find_Object(modelID));
		modelID = 0;
	}
}

void RA_Visible_Driver::Destroyed(GameObject *obj)
{
	if (modelID)
	{
		Commands->Destroy_Object(Commands->Find_Object(modelID));
		modelID = 0;
	}
}

void RA_Visible_Driver::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&modelID,4,1);
	Auto_Save_Variable(&driverID,4,2);
}

//////////////RA_Vision_Control//////////////

void RA_Vision_Control::Created(GameObject *obj)
{
	allowed = true; 
	underwater = false;
	driving = false;
	blackout = false;
	//Revert the screen incase player died while underwater
	Set_Screen_Fade_Color_Player(obj, 0, 0, 0, 0); //revert color
	Set_Screen_Fade_Opacity_Player(obj,0, 0); //revert opacity
	Set_Fog_Color_Player(obj, 256, 256, 256);
	Clear_Fog_Override_Player(obj);
}

void RA_Vision_Control::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	switch(type)
	{
	case 852852: //Disabling type, will cause 'underwater' messages to be ignored
		allowed = false;
		break;
	case 258258: //Enabling type, will allow 'underwater' messages
		allowed = true;
		break;
	case 789789: //Diving type, will activate underwater effect (overrides other effects)
		if (allowed)
		{
			if (!driving) //oops, did I go overboard?
			{
				Commands->Apply_Damage(obj, 100, "Death", 0); //I drowned
			}
			if (blackout)//if blackout in effect, deactivate
			{	
				Set_Fog_Color_Player(obj, 256, 256, 256);
				Clear_Fog_Override_Player(obj);
				blackout = false;
			}
			Set_Screen_Fade_Color_Player(obj, 0, 0, 1, 0);
			Set_Screen_Fade_Opacity_Player(obj, 150, 0);
			underwater = true;
		}
		break;
	case 987987: //Surfacing type, will deactivate underwater effect
		Set_Screen_Fade_Color_Player(obj, 0, 0, 0, 0);
		Set_Screen_Fade_Opacity_Player(obj, 0, 0);
		underwater = false;
		break;
	case 325523: //Blackout type, param 0 = blackout off, param 1 = blackout on
		if (!underwater) //Underwater takes priority
		{
			if (param == 1) //turn on effect
			{
				if (!blackout)
				{
					Set_Screen_Fade_Color_Player(obj, 0, 0, 0, 0);
					Set_Screen_Fade_Opacity_Player(obj, 150, 0);
					Set_Fog_Override_Player(obj, 0, 9);
					Set_Fog_Color_Player(obj, 0, 0, 0);
					blackout = true;
				}
			}
			else if (!param) //turn off effect
			{
				if (blackout)
				{
					Set_Screen_Fade_Color_Player(obj, 0, 0, 0, 0);
					Set_Screen_Fade_Opacity_Player(obj, 0, 0);
					Clear_Fog_Override_Player(obj);
					blackout = false;
				}
			}
		}
		break;
	case 310013: //Exited Vehicle Message
		driving = false;
		if (underwater)
			Commands->Apply_Damage(obj, 100, "Death", 0);
		break;
	case 103301: //Entered Vehicle Message
		driving = true;
		break;
	}
}

void RA_Vision_Control::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&allowed,4,1);
	Auto_Save_Variable(&underwater,4,2);
	Auto_Save_Variable(&driving,4,3);
	Auto_Save_Variable(&blackout,4,4);
}

//////////////RA_Vision_Control_2//////////////

void RA_Vision_Control_2::Created(GameObject *obj)
{
	allowed = true; 
	underwater = false;
	driving = false;
	blackout = false;
	//Revert the screen incase player died while underwater
	Set_Screen_Fade_Color_Player(obj, 0, 0, 0, 0); //revert color
	Set_Screen_Fade_Opacity_Player(obj,0, 0); //revert opacity
	Set_Fog_Color_Player(obj, 256, 256, 256);
	Clear_Fog_Override_Player(obj);
}

void RA_Vision_Control_2::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	switch(type)
	{
	case 852852: //Disabling type, will cause 'underwater' messages to be ignored
		allowed = false;
		break;
	case 258258: //Enabling type, will allow 'underwater' messages
		allowed = true;
		break;
	case 789789: //Diving type, will activate underwater effect (overrides other effects)
		if (allowed)
		{
			if (!driving) //oops, did I go overboard?
			{
				Commands->Apply_Damage(obj, 100, "Death", 0); //I drowned
			}
			if (blackout)//if blackout in effect, deactivate
			{	
				Set_Fog_Color_Player(obj, 256, 256, 256);
				Clear_Fog_Override_Player(obj);
				blackout = false;
			}
			Set_Screen_Fade_Color_Player(obj, 0, 0, 1, 0);
			Set_Screen_Fade_Opacity_Player(obj, 150, 0);
			underwater = true;
		}
		break;
	case 987987: //Surfacing type, will deactivate underwater effect
		Set_Screen_Fade_Color_Player(obj, 0, 0, 0, 0);
		Set_Screen_Fade_Opacity_Player(obj, 0, 0);
		underwater = false;
		break;
	case 325523: //Blackout type, param 0 = blackout off, param 1 = blackout on
		if (!underwater) //Underwater takes priority
		{
			if (param == 1) //turn on effect
			{
				if (!blackout)
				{
					Set_Screen_Fade_Color_Player(obj, 0, 0, 0, 0);
					Set_Screen_Fade_Opacity_Player(obj, 150, 0);
					if (Get_Vehicle(obj))
					{
						Set_Fog_Override_Player(obj, Get_Float_Parameter("VehicleFogStart"), Get_Float_Parameter("VehicleFogEnd"));
					}
					else
					{
						Set_Fog_Override_Player(obj, Get_Float_Parameter("InfantryFogStart"), Get_Float_Parameter("InfantryFogEnd"));
					}
					Set_Fog_Color_Player(obj, 0, 0, 0);
					blackout = true;
				}
			}
			else if (!param) //turn off effect
			{
				if (blackout)
				{
					Set_Screen_Fade_Color_Player(obj, 0, 0, 0, 0);
					Set_Screen_Fade_Opacity_Player(obj, 0, 0);
					Clear_Fog_Override_Player(obj);
					blackout = false;
				}
			}
		}
		break;
	case 310013: //Exited Vehicle Message
		driving = false;
		if (underwater)
			Commands->Apply_Damage(obj, 100, "Death", 0);
		break;
	case 103301: //Entered Vehicle Message
		driving = true;
		break;
	}
}

void RA_Vision_Control_2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&allowed,4,1);
	Auto_Save_Variable(&underwater,4,2);
	Auto_Save_Variable(&driving,4,3);
	Auto_Save_Variable(&blackout,4,4);
}

//////////////RA_Fog_Level_Settings//////////////

void RA_Fog_Level_Settings::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
}

//////////////RA_Missile_Beacon//////////////

void RA_Missile_Beacon::Created(GameObject *obj)
{
	GameObject *silo = Find_Object_By_Preset(2, Get_Parameter("SiloPresetName"));
	Commands->Send_Custom_Event(obj, silo, 269269, 0, 0.0);
	Destroy_Script();
}

//////////////RA_Superweapon_Beacon//////////////

void RA_Superweapon_Beacon::Created(GameObject *obj)
{
	GameObject *silo = Find_Object_By_Preset(2, Get_Parameter("SiloPresetName"));
	if (silo->As_BuildingGameObj() && silo->As_BuildingGameObj()->As_SuperweaponGameObj())
	{
		silo->As_BuildingGameObj()->As_SuperweaponGameObj()->Launch();
	}
	Destroy_Script();
}

//////////////RA_Beacon_Terminal//////////////

void RA_Beacon_Terminal::Created(GameObject *obj)
{
	active = false;
	reset = false;
	Commands->Enable_HUD_Pokable_Indicator(obj, false);
	Attach_Script_Once(obj, "RA_Beacon_Timer", "");
}

void RA_Beacon_Terminal::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if(type == 962962) //building dead
	{
		Commands->Destroy_Object(obj); //building dead, remove terminal
	}
	else if(type == 456369) //timer complete
	{
		if (!active)
		{
			active = true;
			reset = false; //if timer couldn't be reset, it now can be
			Commands->Enable_HUD_Pokable_Indicator(obj, true);
			Create_2D_Sound_Team(Get_Parameter("BeaconReadySound"), Commands->Get_Player_Type(obj));
		}
	}
}

void RA_Beacon_Terminal::Poked(GameObject *obj, GameObject *poker)
{
	if(Commands->Get_Player_Type(poker) == Commands->Get_Player_Type(obj))
	{
		if(active)
		{
			Commands->Give_PowerUp(poker, Get_Parameter("BeaconPowerup"), false);
			Attach_Script_Once(obj, "RA_Beacon_Timer", "");
			active = false;
			Commands->Enable_HUD_Pokable_Indicator(obj, false);
			Create_2D_Sound_Team(Get_Parameter("BeaconTakenSound"), Commands->Get_Player_Type(obj));
		}
	}
	else if (Is_Spy(poker))
	{
		if(!reset)
		{
			Remove_Script(obj, "RA_Beacon_Timer");
			Commands->Start_Timer(obj, this, 2, 95132); //2 second delay until timer script restarts
			active = false;
			reset = true; //timer can only be reset once per 'cycle'
			Commands->Enable_HUD_Pokable_Indicator(obj, false);
			Create_2D_Sound_Team(Get_Parameter("SiloDisableSound"), Commands->Get_Player_Type(obj));
			Create_2D_Sound_Team(Get_Parameter("SpyNotifySound"), Commands->Get_Player_Type(poker));
		}
	}
	else
	{
		Display_Security_Dialog(poker);
	}
}

void RA_Beacon_Terminal::Timer_Expired(GameObject *obj, int number)
{
	if(number == 95132)
	{
		Attach_Script_Once(obj, "RA_Beacon_Timer", ""); //restart beacon timer
	}
}

void RA_Beacon_Terminal::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&active,4,1);
	Auto_Save_Variable(&reset,4,2);
}


void RA_Beacon_Terminal_2::Created(GameObject *obj)
{
	active = false;
	Commands->Enable_HUD_Pokable_Indicator(obj, false);
	Attach_Script_Once(obj, "RA_Beacon_Timer", "");
}

void RA_Beacon_Terminal_2::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if(type == 962962) //building dead
	{
		Commands->Destroy_Object(obj); //building dead, remove terminal
	}
	else if(type == 456369) //timer complete
	{
		if (!active)
		{
			active = true;
			Commands->Enable_HUD_Pokable_Indicator(obj, true);
			Create_2D_Sound_Team(Get_Parameter("BeaconReadySound"), Commands->Get_Player_Type(obj));
		}
	}
	else if (type == Get_Int_Parameter("OfflineCustom"))
	{
		active = false;
		Remove_Script(obj, "RA_Beacon_Timer");
	}
	else if (type == Get_Int_Parameter("OnlineCustom"))
	{
		Attach_Script_Once(obj, "RA_Beacon_Timer", "");
	}
}

void RA_Beacon_Terminal_2::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	unsigned int warhead = ArmorWarheadManager::Get_Warhead_Type(Get_Parameter("Warhead"));
	if (warhead == Get_Damage_Warhead())
	{
		active = false;
		Remove_Script(obj, "RA_Beacon_Timer");
		Commands->Start_Timer(obj,this,1,1);
	}
}

void RA_Beacon_Terminal_2::Timer_Expired(GameObject *obj, int number)
{
	Attach_Script_Once(obj, "RA_Beacon_Timer", "");
}

void RA_Beacon_Terminal_2::Poked(GameObject *obj, GameObject *poker)
{
	if(Commands->Get_Player_Type(poker) == Commands->Get_Player_Type(obj))
	{
		if(active)
		{
			Commands->Give_PowerUp(poker, Get_Parameter("BeaconPowerup"), false);
			Attach_Script_Once(obj, "RA_Beacon_Timer", "");
			active = false;
			Commands->Enable_HUD_Pokable_Indicator(obj, false);
			Create_2D_Sound_Team(Get_Parameter("BeaconTakenSound"), Commands->Get_Player_Type(obj));
		}
	}
	else
	{
		Display_Security_Dialog(poker);
	}
}

//////////////RA_Beacom_Timer//////////////

void RA_Beacon_Timer::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, 300.0, 147);
	//Commands->Start_Timer(obj, this, 3.0, 147);
}

void RA_Beacon_Timer::Timer_Expired(GameObject *obj, int number)
{
	Commands->Send_Custom_Event(obj, obj, 456369, 0, 0.0);
	Remove_Script(obj, "RA_Beacon_Timer");
}

//////////////RA_Missile_Control//////////////

void RA_Missile_Control::Created(GameObject *obj)
{
	launching = false;
	alive = true;
}

void RA_Missile_Control::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if(type == 269269) //beacon deployed message
	{
		if(alive)
		{
			if(!launching)
			{
				launching = true;
				Commands->Set_Animation(obj, Get_Parameter("AnimationName"), false, 0, 0, -1, true);
			}
		}
		else
		{
			Commands->Start_Timer(obj, this, 1.0, Commands->Get_ID(sender)); //beacon kill time delay
		}
	}
	else if(type == 962962)
	{
		alive = false;
	}
}

void RA_Missile_Control::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if(!_stricmp(animation_name, Get_Parameter("AnimationName")))
	{
		obj->As_PhysicalGameObj()->Clear_Animation();
		launching = false;
	}
}

void RA_Missile_Control::Timer_Expired(GameObject *obj, int number)
{
	//This is a time delay to kill a beacon, prevents glitches
	Commands->Apply_Damage(Commands->Find_Object(number), 100, "BlamoKiller", 0);
}

void RA_Missile_Control::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&launching,4,1);
	Auto_Save_Variable(&alive,4,2);
}

//////////////RA_Missile_Controller//////////////

void RA_Missile_Controller::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *receiver = Find_Object_By_Preset(2, Get_Parameter("SiloPresetName"));
	Commands->Send_Custom_Event(obj, receiver, 962962, 0, 0.0); //inform silo of death
	receiver = Find_Object_By_Preset(2, Get_Parameter("TerminalPresetName"));
	Commands->Send_Custom_Event(obj, receiver, 962962, 0, 0.0); //inform terminal of death
}

//////////////RA_Demolition_Truck_Improved//////////////

void RA_Demolition_Truck_Retarded::Created(GameObject *obj)
{
	driverID = 0;
	triggered = false;
}

void RA_Demolition_Truck_Retarded::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	//checks when someone gets in or out and records ID
	//this should account for when there are passengers too
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!driverID) //is there a driver already?
		{
			driverID = Commands->Get_ID(sender);
		}
	}
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		int testID;
		testID = Commands->Get_ID(sender);
		if (driverID == testID) //is it the driver that got out?
		{
			driverID = 0;
		}
	}
}

void RA_Demolition_Truck_Retarded::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (!driverID)
	{
		return; //if there's no driver, don't bother
	}
	int testID;
	testID = Commands->Get_ID(damager);
	if (driverID == testID) //did the driver shoot me?
	{
		const char *warhead = "Death";
		//I can't take it any more! *bang*
		Commands->Apply_Damage(obj, 100.00, warhead, 0);
		triggered = true;
	}
}

void RA_Demolition_Truck_Retarded::Killed(GameObject *obj, GameObject *killer)
{
	Vector3 pos;
	pos = Commands->Get_Position(obj);
	int testID = 0;
	if (killer)
	{
		testID = Commands->Get_ID(killer);
	}
	const char *explosion = Get_Parameter("Explosion");
	const char *warhead = Get_Parameter("Warhead");
	float damage = Get_Float_Parameter("DamageStrength");
	float radius = Get_Float_Parameter("DamageRadius");
	if (testID) //is there a real killer?
	{
		SetExplosionObj(obj);
		Commands->Create_Explosion(explosion, pos, killer); //first half, obey friendly fire rules
		Commands->Create_Explosion(explosion, pos, 0); //second half, ignore rules
		Ranged_Scale_Damage_To_Buildings(damage, warhead,Commands->Get_Position(obj),radius,killer);
		SetExplosionObj(0);
	}
	else if (triggered)//then was it the driver?
	{
		Commands->Create_Explosion(explosion, pos, Commands->Find_Object(driverID)); //first half, obey rules
		Commands->Create_Explosion(explosion, pos, 0); //second half, ignore rules
		Ranged_Scale_Damage_To_Buildings(damage,warhead,Commands->Get_Position(obj),radius,Commands->Find_Object(driverID));
	}
	if (driverID)//If there's a driver, kill him
	{
		Commands->Attach_Script(Commands->Find_Object(driverID),"RA_DriverDeath", "0");
	}
}

void RA_Demolition_Truck_Retarded::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&driverID,4,1);
	Auto_Save_Variable(&triggered,4,2);
}

/////////////RA_MAD_Tank_Devolved/////////////


void RA_MAD_Tank_Devolved::Created(GameObject *obj)
{
	//Initializing variables
	deployed = false;
	newobjID = 0;
	ownerID = 0;
}

void RA_MAD_Tank_Devolved::KeyHook()
{
	GameObject *obj = Owner();
	if (!ownerID)
	{
		//Create Object to jam MAD Tank in place
		GameObject *newobj = Commands->Create_Object_At_Bone(obj, "Invisible_Object", Get_Parameter("Bone"));
		Commands->Set_Model(newobj, Get_Parameter("Attach_Model"));
		Commands->Attach_To_Object_Bone(newobj, obj, Get_Parameter("Bone"));
		newobjID = Commands->Get_ID(newobj);
		//Animate MAD Tank, record player ID of owner
		Commands->Create_2D_Sound(Get_Parameter("Announcement"));
		Commands->Set_Animation(obj, Get_Parameter("Deployed_Animation"), false, 0, 0, -1, false);
		ownerID = Get_Player_ID(Get_Vehicle_Gunner(obj));
		//New stuff added for v0.9935
		Force_Occupants_Exit(obj); //kick driver out
		Commands->Enable_HUD_Pokable_Indicator(obj, false);	//this was to correct some issue
		Commands->Enable_Vehicle_Transitions(obj, false); //no getting back in
		Commands->Set_Player_Type(obj, Commands->Get_Player_Type(Get_Vehicle_Gunner(obj))); //set team
		//End New Stuff
		deployed = true;
	}
}

void RA_MAD_Tank_Devolved::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		InstallHook("MAD",sender);
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		RemoveHook();
	}
}

void RA_MAD_Tank_Devolved::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, Get_Parameter("Deployed_Animation"))) //Is this the right animation?
	{
		if (Get_GameObj(ownerID)) //sonic pulse only if owner still valid
		{
			//Create/Kill Object for Sonic Pulse, this is meant for visual explosion effect only
			GameObject *SonicBox = Commands->Create_Object(Get_Parameter("Sonic_Preset"), Commands->Get_Position(obj));
			Commands->Set_Player_Type(SonicBox, 2); //just in case
			Commands->Apply_Damage(SonicBox,100,"BlamoKiller",Get_GameObj(ownerID));
			//Setup for building damage
			float Epercentage = Get_Float_Parameter("EnemyPercentage");
			float Fpercentage = Get_Float_Parameter("FriendPercentage");
			bool FF = Get_Bool_Parameter("ForceFF");
			const char *warhead = Get_Parameter("Warhead");
			float range = Get_Float_Parameter("DamageRadius");
			//Time to break a few windows
			SetExplosionObj(obj);
			Ranged_Percentage_Damage_To_Buildings(Epercentage, warhead, Commands->Get_Position(obj), range, Get_GameObj(ownerID));
			Ranged_Variable_Percent_Vehicle_Damage(Epercentage, Fpercentage, FF, warhead, Commands->Get_Position(obj), range, Get_GameObj(ownerID)); 
			SetExplosionObj(0);
		}
		//Kill MAD Tank 
		//'0' Damager used so owner won't get points for tank itself
		Commands->Apply_Damage(obj,10000,"BlamoKiller",0);
	}
}

void RA_MAD_Tank_Devolved::Destroyed(GameObject *obj)
{
	//MAD Tank dead, get rid of piston object if it exists
	if (newobjID)
	{
		Commands->Destroy_Object(Commands->Find_Object(newobjID));
	}
}

void RA_MAD_Tank_Devolved::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&deployed,4,2);
	Auto_Save_Variable(&newobjID,4,3);
	Auto_Save_Variable(&ownerID,4,4);
}

//////////////Script Registrants//////////////

ScriptRegistrant<RA_Repair_Controller> RA_Repair_Controller_Registrant("RA_Repair_Controller","ScriptZoneID=0:int");
ScriptRegistrant<RA_Repair_Script> RA_Repair_Script_Registrant("RA_Repair_Script","TeamSelection=0:int");
ScriptRegistrant<RA_Infantry_Spy> RA_Infantry_Spy_Registrant("RA_Infantry_Spy", "");
ScriptRegistrant<RA_Infantry_NotSpy> RA_Infantry_NotSpy_Registrant("RA_Infantry_NotSpy", "");
ScriptRegistrant<RA_Helipad_Zone> RA_Helipad_Zone_Registrant("RA_Helipad_Zone", "TeamSelection=0:int,Reload_Interval=2.00:float,Apache_Preset_ID=0:int,Apache_Reload_Powerup=Blank:string,Hind_Preset_ID=0:int,Hind_Reload_Powerup=Blank:string");
ScriptRegistrant<RA_MineLayer> RA_MineLayer_Registrant("RA_MineLayer", "Mine_Manager_ID=0:int");
ScriptRegistrant<RA_Mine> RA_Mine_Registrant("RA_Mine", "Mine_Manager_ID=0:int,Is_Anti-Personnel=0:int,Is_Anti-Tank=0:int,Trigger_Damage=1.0:float,Trigger_Warhead=Death:string");
ScriptRegistrant<RA_Mine_Manager> RA_Mine_Manager_Registrant("RA_Mine_Manager", "Allied_Mine=Blank:string,Soviet_Mine=Blank:string,Mine_Reload=Blank:string,BoneName=mine:string,Mine_Limit=30:int");
ScriptRegistrant<RA_ObjectDeath_OnCreate> RA_ObjectDeath_OnCreate_Registrant("RA_ObjectDeath_OnCreate","Object=null:string");
ScriptRegistrant<RA_Naval_Yard> RA_Naval_Yard_Registrant("RA_Naval_Yard", "Limit=8:int,Disable_Time=5.0:float");
ScriptRegistrant<RA_Naval_PT_Pokable> RA_Naval_PT_Pokable_Registrant("RA_Naval_PT_Pokable", "Preset_Name=blank:string,Cost=0.0:float,Team=0:int,Controller_ID=0:int,Zone_ID=0:int");
ScriptRegistrant<RA_Naval_Unit> RA_Naval_Unit_Registrant("RA_Naval_Unit", "Controller_ID=0:int");
ScriptRegistrant<RA_Naval_Zone> RA_Naval_Zone_Registrant("RA_Naval_Zone", "Explosion=blank:string");
ScriptRegistrant<RA_Damaged_Credits> RA_Damaged_Credits_Registrant("RA_Damaged_Credits","Divisor=4.0:float");
ScriptRegistrant<RA_GameStart_Detector> RA_GameStart_Detector_Registrant("RA_GameStart_Detector", "Interval=5.0:float,ReceiverID=0:int,Message=0:int,Parameter=0:int");
ScriptRegistrant<RA_DriverDeath> RA_DriverDeath_Registrant("RA_DriverDeath", "");
ScriptRegistrant<RA_Conyard_Repair> RA_Conyard_Repair_Registrant("RA_Conyard_Repair", "Repair_Frequency=1.0:float");
ScriptRegistrant<RA_ObjectDeath_OnDeath> RA_ObjectDeath_OnDeath_Registrant("RA_ObjectDeath_OnDeath","Preset=blank:string");
ScriptRegistrant<RA_Demolition_Truck_Improved> RA_Demolition_Truck_Improved_Registrant("RA_Demolition_Truck_Improved", "Explosion=Explosion_Invis_Nuke:string,Warhead=Nuke:string,DamageStrength=480.0:float,DamageRadius=120.0:float");
ScriptRegistrant<RA_CTF_Zone> RA_CTF_Zone_Registrant("RA_CTF_Zone","Team_ID=0:int,Max_Capture=5:int,Flag_Preset_Name:string");
ScriptRegistrant<RA_MAD_Tank_Improved> RA_MAD_Tank_Improved_Registrant("RA_MAD_Tank_Improved", "Attach_Model=pistons:string,Deployed_Animation=V_SO_MADTANK.ANIM_MADTANK:string,Sonic_Preset=Sonic_Box:string,Percentage=0.34:float,Warhead=Sonic:string,DamageRadius=250.0:float,Announcement=MAD_Tank_Deployed:string,Bone=ROOTTRANSFORM:string");
ScriptRegistrant<RA_Conyard_Controller_Improved> RA_Conyard_Controller_Improved_Registrant("RA_Conyard_Controller_Improved", "Team=0:int,Preset1=blank:string,Preset2=blank:string,Preset3=blank:string,Preset4=blank:string,Preset5=blank:string");
ScriptRegistrant<RA_Conyard_Controller_Improved_2> RA_Conyard_Controller_Improved_2_Registrant("RA_Conyard_Controller_Improved_2", "Team=0:int,Preset1=blank:string,Preset2=blank:string,Preset3=blank:string,Preset4=blank:string,Preset5=blank:string,Preset6=blank:string,Preset7=blank:string,Preset8=blank:string");
ScriptRegistrant<RA_Building_DeathSound> RA_Building_DeathSound_Registrant("RA_Building_DeathSound", "HalfSound=ExploSound_Bldg_Half:string,DeadSound=ExploSound_Bldg_Dead:string");
ScriptRegistrant<RA_Base_Defense_Simple> RA_Base_Defense_Simple_Registrant("RA_Base_Defense_Simple","MinRange=0.0:float,MaxRange=100.0:float,AntiAir=0:int,AntiGround=1:int,AdjustAim=0:int,TargetMode=0:int");
ScriptRegistrant<RA_Base_Defense_Powered> RA_Base_Defense_Powered_Registrant("RA_Base_Defense_Powered","MinRange=0.0:float,MaxRange=100.0:float,AntiAir=0:int,AntiGround=1:int,AdjustAim=0:int,TargetMode=0:int");
ScriptRegistrant<RA_Base_Defense_Chargeup> RA_Base_Defense_Chargeup_Registrant("RA_Base_Defense_Chargeup","MinRange=0.0:float,MaxRange=100.0:float,AntiAir=0:int,AntiGround=1:int,AdjustAim=0:int,TargetMode=0:int,ChargeAnim:string,ChargeSound:string");
ScriptRegistrant<RA_Vehicle_Regen> RA_Vehicle_Regen_Registrant("RA_Vehicle_Regen", "Percentage=0.5:float,Heal=1.0:float,Time=1.0:float,TimerID=87654:int");
ScriptRegistrant<RA_Thief> RA_Thief_Registrant("RA_Thief", "Enter_Message=357753:int,Percentage=0.5:float");
ScriptRegistrant<RA_Thief_Improved> RA_Thief_Improved_Registrant("RA_Thief_Improved", "Enter_Message=357753:int,Percentage=0.5:float,Sound:string");
ScriptRegistrant<RA_Credit_Theft_Zone> RA_Credit_Theft_Zone_Registrant("RA_Credit_Theft_Zone", "Enter_Message=357753:int,Team=0:int");
ScriptRegistrant<RA_Credit_Theft_Zone_Timer> RA_Credit_Theft_Zone_Timer_Registrant("RA_Credit_Theft_Zone_Timer", "Enter_Message=357753:int,Team=0:int,Time:float");
ScriptRegistrant<RA_DestroyNearest_OnDeath> RA_DestroyNearest_OnDeath_Registrant("RA_DestroyNearest_OnDeath", "Preset_Name:string");
ScriptRegistrant<RA_Ore_Truck> RA_Ore_Truck_Registrant("RA_Ore_Truck", "Field_Enter=456654:int,Field_Exit=75321:int,Deposit_Enter=321654:int,Max_Capacity=7:int,Harvest_Anim:string,Deliver_Anim:string");
ScriptRegistrant<RA_Ore_Truck_2> RA_Ore_Truck_2_Registrant("RA_Ore_Truck_2", "Field_Enter=456654:int,Field_Exit=75321:int,Deposit_Enter=321654:int,Max_Capacity=7:int,Harvest_Anim:string,Deliver_Anim:string,Harvest_Sound:string,Dump_Sound:string");
ScriptRegistrant<RA_Ore_Field> RA_Ore_Field_Registrant("RA_Ore_Field", "Field_Value=700:int,Enter=456654:int,Exit=75321:int");
ScriptRegistrant<RA_Ore_Delivery_Zone> RA_Ore_Delivery_Zone_Registrant("RA_Ore_Delivery_Zone", "Team=0:int,Enter=321654");
ScriptRegistrant<RA_Vehicle_Team_Set> RA_Vehicle_Team_Set_Registrant("RA_Vehicle_Team_Set","");
ScriptRegistrant<RA_Vehicle_Team_Timer> RA_Vehicle_Team_Timer_Registrant("RA_Vehicle_Team_Timer","");
ScriptRegistrant<RA_Visible_Driver> RA_Visible_Driver_Registrant("RA_Visible_Driver","BoneName=seat0:string,Message=471174:int");
ScriptRegistrant<RA_Vision_Control> RA_Vision_Control_Registrant("RA_Vision_Control","FogController=RA_Game_Manager:string");
ScriptRegistrant<RA_Vision_Control_2> RA_Vision_Control_2_Registrant("RA_Vision_Control_2","FogController=RA_Game_Manager:string,InfantryFogStart=0:float,InfantryFogEnd=9:float,VehicleFogStart=0:float,VehicleFogEnd=9:float");
ScriptRegistrant<RA_Fog_Level_Settings> RA_Fog_Level_Settings_Registrant("RA_Fog_Level_Settings", "FogEnable=1:int,FogMin=200.0:float,FogMax=300.0:float");
ScriptRegistrant<RA_Missile_Beacon> RA_Missile_Beacon_Registrant("RA_Missile_Beacon", "SiloPresetName:string");
ScriptRegistrant<RA_Superweapon_Beacon> RA_Superweapon_Beacon_Registrant("RA_Superweapon_Beacon", "SiloPresetName:string");
ScriptRegistrant<RA_Beacon_Terminal> RA_Beacon_Terminal_Registrant("RA_Beacon_Terminal", "BeaconPowerup:string,BeaconReadySound:string,BeaconTakenSound:string,SiloDisableSound:string,SpyNotifySound:string");
ScriptRegistrant<RA_Beacon_Terminal_2> RA_Beacon_Terminal_2_Registrant("RA_Beacon_Terminal_2", "BeaconPowerup:string,BeaconReadySound:string,BeaconTakenSound:string,SiloDisableSound:string,OfflineCustom:string,OnlineCustom:string,Warhead:string");
ScriptRegistrant<RA_Beacon_Timer> RA_Beacon_Timer_Registrant("RA_Beacon_Timer", "");
ScriptRegistrant<RA_Missile_Control> RA_Missile_Control_Registrant("RA_Missile_Control","AnimationName:string");
ScriptRegistrant<RA_Missile_Controller> RA_Missile_Controller_Registrant("RA_Missile_Controller","SiloPresetName:string,TerminalPresetName:string");
ScriptRegistrant<RA_Demolition_Truck_Retarded> RA_Demolition_Truck_Retarded_Registrant("RA_Demolition_Truck_Retarded", "Explosion=Explosion_Invis_Nuke:string,Warhead=Nuke:string,DamageStrength=480.0:float,DamageRadius=120.0:float");
ScriptRegistrant<RA_MAD_Tank_Devolved> RA_MAD_Tank_Devolved_Registrant("RA_MAD_Tank_Devolved", "Attach_Model=pistons:string,Deployed_Animation=V_SO_MADTANK.ANIM_MADTANK:string,Sonic_Preset=Sonic_Box:string,EnemyPercentage=0.34:float,FriendPercentage=0.17:float,ForceFF=1:int,Warhead=Sonic:string,DamageRadius=250.0:float,Announcement=MAD_Tank_Deployed:string,Bone=ROOTTRANSFORM:string");
