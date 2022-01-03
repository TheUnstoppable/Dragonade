/*	Renegade Scripts.dll
	Copyright 2017 Tiberian Technologies

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
#include "renalert2.h"

/*
*************************************************
*				Script Functions				*
*************************************************
*/

/******************Missle Silo Script Definitions********************/

//////////////RA_ABomb_Beacon//////////////

void RA_ABomb_Beacon::Created(GameObject *obj)
{
	//message 269269 is the beacon talking to controller
	int receiverID = Get_Int_Parameter("ManagerID");
	Commands->Send_Custom_Event(obj,Commands->Find_Object(receiverID),269269,0,0.0);
}

//////////////RA_Missile_Silo//////////////

void RA_Missile_Silo::Created(GameObject *obj)
{
	alive = true;
	launching = false;
	int team = Get_Int_Parameter("Team");
	if (!team)
	{
		//Create Door/Missile Model
		GameObject *newobj = Commands->Create_Object("silo_missile", Commands->Get_Position(obj));
		Commands->Set_Facing(newobj, Get_Float_Parameter("ZRotation"));
		//Create Beacon Terminal
		GameObject *terminal = Commands->Create_Object_At_Bone(newobj, "Sov_ABomb_PT", "terminal");
		char params[100];
		sprintf(params, "%d,%d,%s", team, Commands->Get_ID(obj), "CnC_POW_Soviet_AtomBomb");
		Commands->Attach_Script(terminal, "RA_ABomb_Terminal", params);
	}
	else if (team == 1)
	{
		//Create Door/Missile Model
		GameObject *newobj = Commands->Create_Object("silo_missile", Commands->Get_Position(obj));
		Commands->Set_Facing(newobj, Get_Float_Parameter("ZRotation"));
		//Create Beacon Terminal
		GameObject *terminal = Commands->Create_Object_At_Bone(newobj, "All_ABomb_PT", "terminal");
		char params[100];
		sprintf(params, "%d,%d,%s", team, Commands->Get_ID(obj), "CnC_POW_Allied_AtomBomb");
		Commands->Attach_Script(terminal, "RA_ABomb_Terminal", params);
	}
}

void RA_Missile_Silo::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	//message 269269: incoming beacon deployed
	//message 486486: incoming 'new door' announcement
	if (type == 269269) //Is this from a beacon?
	{
		if (alive)
		{
			if (!launching)
			{
				launching = true;
				Commands->Create_Explosion("Explosion_Clear_Cinematic", Commands->Get_Position(obj), 0);
				//Create launching animation, destroy door
				GameObject *launchobj = Commands->Create_Object("anim_silo_launch", Commands->Get_Position(obj));
				Commands->Set_Facing(launchobj, Get_Float_Parameter("ZRotation"));
				char params[100];
				sprintf(params, "%d,%d,%s", Commands->Get_ID(obj), 1,"LAUNCH_MSILO.LAUNCH_MSILO");
				Commands->Attach_Script(launchobj, "RA_Silo_Animation", params);
			}
		}
		else
		{
			Commands->Start_Timer(obj, this, 1.0, Commands->Get_ID(sender));
		}
	}
	else if (type == 486486) //is it from the launch animation?
	{
		launching = false;
		//Recreate standard door
		GameObject *newobj = Commands->Create_Object("silo_missile", Commands->Get_Position(obj));
		Commands->Set_Facing(newobj, Get_Float_Parameter("ZRotation"));
		Commands->Destroy_Object(sender);
	}
	else if (type == 985741) //is it from the terminal?
	{
		if (alive)
		{
			Commands->Send_Custom_Event(obj, sender, 147589, param, 0.0);
		}
		else
		{
			Commands->Destroy_Object(sender);
		}
	}
}

void RA_Missile_Silo::Killed(GameObject *obj, GameObject *killer)
{
	alive = false;
}

void RA_Missile_Silo::Timer_Expired(GameObject *obj, int number)
{
	Commands->Apply_Damage(Commands->Find_Object(number), 100, "BlamoKiller", 0);
}

void RA_Missile_Silo::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&alive,4,1);
	Auto_Save_Variable(&launching,4,2);
}

/////////////RA_Missile_Manager/////////////

void RA_Missile_Manager::Created(GameObject *obj)
{
	//Setup Soviet Missile Silo
	if (Get_Int_Parameter("SovietSiloID"))
	{
		char sparams[100];
		sprintf(sparams, "%d,%f", 0, Get_Float_Parameter("Soviet_ZRotation")); 
		Commands->Attach_Script(Commands->Find_Object(Get_Int_Parameter("SovietSiloID")), "RA_Missile_Silo", sparams);
	}
	//Setup Allied Missile Silo
	if (Get_Int_Parameter("AlliedSiloID"))
	{
		char aparams[100];
		sprintf(aparams, "%d,%f", 1, Get_Float_Parameter("Allied_ZRotation")); 
		Commands->Attach_Script(Commands->Find_Object(Get_Int_Parameter("AlliedSiloID")), "RA_Missile_Silo", aparams);
	}
}

void RA_Missile_Manager::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 269269)
	{
		//Test team the message is for
		int test = Commands->Get_Player_Type(sender);
		int controller = 0; //initializing just because I can
		if (!test)
		{
			controller = Get_Int_Parameter("SovietSiloID");
		}
		else if (test == 1)
		{
			controller = Get_Int_Parameter("AlliedSiloID");
		}
		//relay message to controller
		Commands->Send_Custom_Event(sender, Commands->Find_Object(controller), 269269, 0, 0.0);
	}
}

//////////////RA_Silo_Animation//////////////

void RA_Silo_Animation::Created(GameObject *obj)
{
	if (Get_Int_Parameter("Mode") == 1)
	{
		Commands->Start_Timer(obj, this, 2.6f, 688);
	}
}
void RA_Silo_Animation::Timer_Expired(GameObject *obj, int number)
{
	if (number == 688)
	{
		int controller = Get_Int_Parameter("ControllerID");
		Commands->Send_Custom_Event(obj, Commands->Find_Object(controller), 486486, 0, 0.0);
	}
}

//////////////RA_ABomb_Terminal//////////////

void RA_ABomb_Terminal::Created(GameObject *obj)
{
	//Initialize Variable(s)
	active = false; 
	//Initialize settings
	Commands->Set_Player_Type(obj, Get_Int_Parameter("Team"));
	Commands->Display_Health_Bar(obj, false);
	Commands->Set_Shield_Type(obj, "Blamo");
	Commands->Enable_HUD_Pokable_Indicator(obj, false);
	//Start Timer
	Commands->Start_Timer(obj, this, 300.0, 471);
}

void RA_ABomb_Terminal::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	//receives messages from Controller here
	if (type == 147589)
	{
		Commands->Give_PowerUp(Commands->Find_Object(param), Get_Parameter("Powerup"), true);
		active = false;
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
		Commands->Start_Timer(obj, this, 300.0, 471);
	}
}

void RA_ABomb_Terminal::Poked(GameObject *obj, GameObject *poker)
{
	if (active)
	{
		int type = Commands->Get_Player_Type(poker);

		if (type == Get_Int_Parameter("Team"))
		{
			Commands->Send_Custom_Event(obj, Commands->Find_Object(Get_Int_Parameter("ControllerID")), 985741, Commands->Get_ID(poker), 0.0);
		}
		else
		{
			Display_Security_Dialog(poker);
		}
	}
}

void RA_ABomb_Terminal::Timer_Expired(GameObject *obj, int number)
{
	if (number == 471)
	{
		active = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}

void RA_ABomb_Terminal::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&active,4,1);
}

/****************Weather Change Script Definitions*******************/

//////////////RA_CanyonRiver_Weather//////////////

void RA_CanyonRiver_Weather::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	int msg = Get_Int_Parameter("Message");
	if (msg == type)
	{
		Commands->Start_Timer(obj, this, Get_Float_Parameter("Storm_Delay"), 789456);
	}
}

void RA_CanyonRiver_Weather::Timer_Expired(GameObject *obj, int number)
{
	if (number == 789456)
	{
		//general variables
		float time = Get_Float_Parameter("Storm_Transition");
		float heading = Get_Float_Parameter("Storm_Heading");
		//Commands
		Commands->Set_Fog_Range(Get_Float_Parameter("Fog_Start"), Get_Float_Parameter("Fog_End"), time);
		Commands->Set_Rain(Get_Float_Parameter("Rain_Density"), time, true);
		Commands->Set_Lightning(Get_Float_Parameter("Lightning_Intensity"), Get_Float_Parameter("Lightning_Start"), Get_Float_Parameter("Lightning_End"), heading, Get_Float_Parameter("Lightning_Distribution"), time);
		Commands->Set_Wind(heading, Get_Float_Parameter("Wind_Strength"), Get_Float_Parameter("Wind_Variability"), time);
	}
}

//////////////RA_Metro_Weather//////////////

void RA_Metro_Weather::Created(GameObject *obj)
{
	max = true;
	Commands->Start_Timer(obj, this, Get_Float_Parameter("Time"), 55);
}

void RA_Metro_Weather::Timer_Expired(GameObject *obj, int number)
{
	if (number == 55)
	{
		float time = Get_Float_Parameter("Time");
		if (max) //If storm is at maximum strength go to minimum
		{
			max = false;
			Commands->Set_Rain(0.0, time, true);
			Commands->Set_Clouds(0.25, 1.0, time);
			Commands->Set_Lightning(0.0, 0.8f, 1.0, 104.0, 1.00, time);
			Commands->Set_Wind(0.0, 1.0, 0.0, time);
			Commands->Start_Timer(obj, this, (time + 60.0f), 55);
		}
		else //If storm at minimum strength go to maximum
		{
			max = true;
			Commands->Set_Rain(0.350f, time, true);
			Commands->Set_Clouds(1.0, 1.0, time);
			Commands->Set_Lightning(0.4f, 0.8f, 1.0, 104.0, 1.00, time);
			Commands->Set_Wind(0.0, 2.0, 0.0, time);
			Commands->Start_Timer(obj, this, (time + 60.0f), 55);
		}
	}
}

void RA_Metro_Weather::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&max,4,1);
}
/*******************Underwater Script Definitions********************/

//////////////RA_Underwater_OnCustom//////////////

void RA_Underwater_OnCustom::Created(GameObject *obj)
{
	allowed = true; 
	underwater = false;
	driving = false;
	//Revert the screen incase player died while underwater
	Set_Screen_Fade_Color_Player(obj, 0, 0, 0, 0); //revert color
	Set_Screen_Fade_Opacity_Player(obj,0, 0); //revert opacity
}

void RA_Underwater_OnCustom::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == Get_Int_Parameter("Disable_Message"))
	{
		allowed = false; //disable diving effect
	}
	else if (type == Get_Int_Parameter("Enable_Message"))
	{
		allowed = true; //enable diving effect
	}
	else if (type == Get_Int_Parameter("Dive_Message"))
	{
		if (allowed)
		{
			if (!driving) //oops, did I go overboard?
			{
				Commands->Apply_Damage(obj, 100, "Death", 0); //I drowned
			}
			Set_Screen_Fade_Color_Player(obj, Get_Float_Parameter("Dive_Red"), Get_Float_Parameter("Dive_Green"), Get_Float_Parameter("Dive_Blue"), 0);
			Set_Screen_Fade_Opacity_Player(obj, Get_Float_Parameter("Dive_Opacity"), 0);
			underwater = true;
		}
	}
	else if (type == Get_Int_Parameter("Surface_Message"))
	{
		Set_Screen_Fade_Color_Player(obj, 0, 0, 0, 0);
		Set_Screen_Fade_Opacity_Player(obj, 0, 0);
		underwater = false;
	}
	else if (type == 879879) //Dive infantry walk to deep?
	{
		if (allowed)
		{
			if (!driving) //oops, did I go overboard?
			{
				//Only dive and die if not in a vehicle
				Commands->Apply_Damage(obj, 100, "Death", 0); //I drowned
				Set_Screen_Fade_Color_Player(obj, Get_Float_Parameter("Dive_Red"), Get_Float_Parameter("Dive_Green"), Get_Float_Parameter("Dive_Blue"), 0);
				Set_Screen_Fade_Opacity_Player(obj, Get_Float_Parameter("Dive_Opacity"), 0);
				underwater = true;
			}
		}
	}
	else if (type == 310013) //player left vehicle
	{
		driving = false;
		if (underwater)
		{
			Commands->Apply_Damage(obj, 100, "Death", 0);
		}
	}
	else if (type == 103301) //player got in a vehicle
	{
		driving = true;
	}
}

void RA_Underwater_OnCustom::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&allowed,4,1);
	Auto_Save_Variable(&underwater,4,2);
	Auto_Save_Variable(&driving,4,3);
}

///////////////RA_Underwater_Zone///////////////

void RA_Underwater_Zone::Entered(GameObject *obj, GameObject *enterer)
{
	Commands->Send_Custom_Event(obj,enterer,Get_Int_Parameter("Enter_Message"),0,0.0);
}

void RA_Underwater_Zone::Exited(GameObject *obj, GameObject *exiter)
{
	Commands->Send_Custom_Event(obj,exiter,Get_Int_Parameter("Exit_Message"),0,0.0);
}

/******************Miscellaneous Script Definitions******************/

/////////////////RA_Submarine/////////////////

void RA_Submarine::Created(GameObject *obj)
{
	driverID = 0;
	for (int i = 0; i < 4; i++)
	{
		blockers[i] = 0;
	}
	occupied = false;
	underwater = false;
}

void RA_Submarine::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		occupied = true;
		if (!driverID)
		{
			driverID = Commands->Get_ID(sender);
		}
	}
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		occupied = false;
		if (underwater)
		{
			Commands->Apply_Damage(obj, 100, "Death", 0);
		}
		if (driverID == Commands->Get_ID(sender))
		{
			driverID = 0;
		}
	}
	else if (type == 789789) //Did I just dive?
	{
		underwater = true;
		if (occupied)
		{
			if (Get_Int_Parameter("Disable"))
			{
				GameObject *block = Commands->Create_Object_At_Bone(obj, "gun_jammer", "MuzzleA0");
				Commands->Attach_To_Object_Bone(block, obj, "MuzzleA0");
				blockers[0] = Commands->Get_ID(block);
				block = Commands->Create_Object_At_Bone(obj, "gun_jammer", "MuzzleA1");
				Commands->Attach_To_Object_Bone(block, obj, "MuzzleA1");
				blockers[1] = Commands->Get_ID(block);
				block = Commands->Create_Object_At_Bone(obj, "gun_jammer", "MuzzleB0");
				Commands->Attach_To_Object_Bone(block, obj, "MuzzleB0");
				blockers[2] = Commands->Get_ID(block);
				block = Commands->Create_Object_At_Bone(obj, "gun_jammer", "MuzzleB1");
				Commands->Attach_To_Object_Bone(block, obj, "MuzzleB1");
				blockers[3] = Commands->Get_ID(block);
			}
		}
		else
		{
			Commands->Apply_Damage(obj, 100, "Death", 0);
		}
	}
	else if (type == 987987) //Did I just surface?
	{
		underwater = false;
		for (int i = 0; i < 4; i++)
		{
			if (blockers[i])
			{
				Commands->Destroy_Object(Commands->Find_Object(blockers[i]));
			}
		}
	}
}
void RA_Submarine::Killed(GameObject *obj, GameObject *killer)
{
	if (underwater)
	{
		if (driverID) //do I have a driver ID
		{
			GameObject *driver = Commands->Find_Object(driverID);
			if (driver) //double checking validity
			{
				Commands->Attach_Script(driver, "RA_DriverDeath", "0");
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (blockers[i])
		{
			Commands->Destroy_Object(Commands->Find_Object(blockers[i]));
		}
	}
}

void RA_Submarine::Destroyed(GameObject *obj)
{
	for (int i = 0; i < 4; i++)
	{
		if (blockers[i])
		{
			Commands->Destroy_Object(Commands->Find_Object(blockers[i]));
		}
	}
}

void RA_Submarine::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&blockers[0],4,1);
	Auto_Save_Variable(&blockers[1],4,2);
	Auto_Save_Variable(&blockers[2],4,3);
	Auto_Save_Variable(&blockers[3],4,4);
	Auto_Save_Variable(&underwater,4,5);
	Auto_Save_Variable(&occupied,4,6);
	Auto_Save_Variable(&driverID,4,7);
}

//////////////RA_Vehicle//////////////

void RA_Vehicle::Created(GameObject *obj)
{
	//initialize array
	for (int i = 0; i < 6; i++)
	{
		IDarray[i] = 0;
	}
}

void RA_Vehicle::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	//message 301103 - incoming driver ID request
	// -param is 'seat number' of driver/passenger
	//message 103301 - outgoing ID report
	// -param is ID for driver/passenger of requested seat
	//message 310013
	// -param 0: outgoing exit message
	// -param 1: outgoing I'm dead message
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		for (int i = 0; i < 6; i++)
		{
			if (!IDarray[i])
			{
				IDarray[i] = Commands->Get_ID(sender);
				Commands->Send_Custom_Event(obj, sender, 103301, IDarray[param], 0);
				return; //ID recorded, no need to continue
			}
		}
	}
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		for (int i = 0; i < 6; i++)
		{
			if (IDarray[i] == Commands->Get_ID(sender))
			{
				IDarray[i] = 0;
				Commands->Send_Custom_Event(obj, sender, 310013, 0, 0);
				return; //ID recorded, no need to continue
			}
		}
	}
	else if (type == 301103)
	{
		if (param < 6) //safety check before accessing array
		{
			Commands->Send_Custom_Event(obj, sender, 103301, IDarray[param], 0);
		}
	}
}

void RA_Vehicle::Killed(GameObject *obj, GameObject *killer)
{
	//message 310013
	// -param 0: outgoing exit message
	// -param 1: outgoing I'm dead message
	for (int i = 0; i < 6; i++)
	{
		if (IDarray[i]) //is there an ID here?
		{
			Commands->Send_Custom_Event(obj, Commands->Find_Object(IDarray[i]), 310013, 1, 0.5); //0.5 second delay so driver has returned to normal gameplay
			IDarray[i] = 0;
		}
	}
}

void RA_Vehicle::Destroyed(GameObject *obj)
{
	//message 310013
	// -param 0: outgoing exit message
	// -param 1: outgoing I'm dead message
	for (int i = 0; i < 6; i++)
	{
		if (IDarray[i]) //is there an ID here?
		{
			Commands->Send_Custom_Event(obj, Commands->Find_Object(IDarray[i]), 310013, 1, 0.5); //0.5 second delay so driver has returned to normal gameplay
		}
	}
}

void RA_Vehicle::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&IDarray[0],4,1);
	Auto_Save_Variable(&IDarray[1],4,2);
	Auto_Save_Variable(&IDarray[2],4,3);
	Auto_Save_Variable(&IDarray[3],4,4);
	Auto_Save_Variable(&IDarray[4],4,5);
	Auto_Save_Variable(&IDarray[5],4,6);
}

ScriptRegistrant<RA_ABomb_Beacon> RA_ABomb_Beacon_Registrant("RA_ABomb_Beacon", "ManagerID=100100:int");
ScriptRegistrant<RA_Missile_Silo> RA_Missile_Silo_Registrant("RA_Missile_Silo", "Team=0:int,ZRotation=0.0:float");
ScriptRegistrant<RA_Missile_Manager> RA_Missile_Manager_Registrant("RA_Missile_Manager", "AlliedSiloID=0:int,Allied_ZRotation=0.0:float,SovietSiloID=0:int,Soviet_ZRotation=0.0:float");
ScriptRegistrant<RA_Silo_Animation> RA_Silo_Animation_Registrant("RA_Silo_Animation", "ControllerID:int,Mode:int,Animation:string");
ScriptRegistrant<RA_ABomb_Terminal> RA_ABomb_Terminal_Registrant("RA_ABomb_Terminal", "Team:int,ControllerID:int,Powerup:string");
ScriptRegistrant<RA_CanyonRiver_Weather> RA_CanyonRiver_Weather_Registrant("RA_CanyonRiver_Weather", "Message=754754:int,Storm_Delay=900.0:float,Storm_Transition=300.0:float,Storm_Heading=60.0:float,Fog_Start=100.0:float,Fog_End=150.0:float,Rain_Density=1.0:float,Lightning_Intensity=1.0:float,Lightning_Start=0.2:float,Lightning_End=0.8:float,Lightning_Distribution=0.5:float,Wind_Strength=10.0:float,Wind_Variability=0.9:float");
ScriptRegistrant<RA_Metro_Weather> RA_Metro_Weather_Registrant("RA_Metro_Weather", "Time=300.0:float");
ScriptRegistrant<RA_Underwater_OnCustom> RA_Underwater_OnCustom_Registrant("RA_Underwater_OnCustom","Dive_Message=789789:int,Surface_Message=987987:int,Disable_Message=852852:int,Enable_Message=258258:int,Dive_Red=0.0:float,Dive_Green=0.0:float,Dive_Blue=1.0:float,Dive_Opacity=150.0:float");
ScriptRegistrant<RA_Underwater_Zone> RA_Underwater_Zone_Registrant("RA_Underwater_Zone","Enter_Message=0:int,Exit_Message=0:int");
ScriptRegistrant<RA_Submarine> RA_Submarine_Registrant("RA_Submarine", "Disable=0:int");
ScriptRegistrant<RA_Vehicle> RA_Vehicle_Registrant("RA_Vehicle","");
