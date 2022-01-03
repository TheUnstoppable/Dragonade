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
#include "ra_legacy.h"

//////////////RA_Thief_Script//////////////

/*
NeoSaber's Note: This script is currently obsolete. It's been
replaced with the combination of the three scripts RA_Allied_Thief, 
RA_Soviet_Refinery_Controller, and RA_Soviet_Refinery_Theft_Zone
*/

void RA_Thief_Script::Entered(GameObject *obj,GameObject *enterer)
{
	//0 = Soviet, 1 = Allies for team #
	if ((enabled) && (Get_Int_Parameter("Thief_Preset_ID") == Commands->Get_Preset_ID(enterer)))
	{
		enabled = false;
		Commands->Give_Money(enterer,Get_Float_Parameter("Credits_To_Award"),true);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Prevent_Theft_Timer"),1);
	}
}

void RA_Thief_Script::Timer_Expired(GameObject * obj,int number)
{
	enabled = true;
}

void RA_Thief_Script::Created(GameObject * obj)
{
	enabled = true;
}

void RA_Thief_Script::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

//////////////RA_Soviet_Defence//////////////

void RA_Soviet_Defence::Created(GameObject * obj)
{
	Vector3 pos,pos1,pos2,pos3;
	GameObject * object;
	objtype = Commands->Get_Player_Type(obj);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	pos = Commands->Get_Position(obj);
	pos1.X = pos.X - 10;
	pos1.Y = pos.Y - 10;
	pos1.Z = pos.Z + 2;
	pos2.X = pos.X + 10;
	pos2.Y = pos.Y;
	pos2.Z = pos.Z + 2;
	pos3.X = pos.X + 10;
	pos3.Y = pos.Y - 10;
	pos3.Z = pos.Z + 2;
	object = Commands->Create_Object("Invisible_Object", pos1);
	if (object)
	{
		id1 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object", pos2);
	if (object)
	{
		id2 = Commands->Get_ID(object);
	}
	object = Commands->Create_Object("Invisible_Object", pos3);
	if (object)
	{
		id3 = Commands->Get_ID(object);
	}
	Commands->Start_Timer(obj, this, 10, 1);
}
void RA_Soviet_Defence::Enemy_Seen(GameObject * obj,GameObject * seen)
{
	ActionParamsStruct params;
	Vector3 pos,pos2;
	float distance;
	float minattack,maxattack;
	float attacktimer;
	int IgnorePreset;
	IgnorePreset = Commands->Get_Preset_ID(seen);
	if ((IgnorePreset != Get_Int_Parameter("Spy_Preset_ID")) && (IgnorePreset != Get_Int_Parameter("Aircraft_Preset_ID"))) 
	{
		pos = Commands->Get_Position(obj);
		pos2 = Commands->Get_Position(seen);
		distance = Commands->Get_Distance(pos2,pos);
		minattack = Get_Float_Parameter("MinAttackDistance");
		if (distance >= minattack) 
		{
			maxattack = Get_Float_Parameter("MaxAttackDistance");
			params.Set_Basic(this,100,2);
			params.Set_Attack(seen,maxattack,0.0,true);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			attacktimer = Get_Float_Parameter("AttackTimer");
			Commands->Start_Timer(obj,this,attacktimer,2);
		}
	}
}
void RA_Soviet_Defence::Action_Complete(GameObject * obj,int action_id,ActionCompleteReason complete_reason)
{
	if (action_id == 2)
	{
		Commands->Action_Reset(obj,100);
	}
}
void RA_Soviet_Defence::Timer_Expired(GameObject * obj,int number)
{
	ActionParamsStruct var;
	GameObject * object;
	float f;
	switch (number)
	{
	case 1:
		f = Commands->Get_Random(0.0f, 2.9999f);
		switch (__min(int(f), 2))
		{
		case 0:
			object = Commands->Find_Object(id1);
			if (object)
			{
				var.Set_Basic(this, 70, 1);
				var.Set_Attack(object, 0.0, 0.0, true);
				Commands->Action_Attack(obj, var);
			}
			break;
		case 1:
			object = Commands->Find_Object(id2);
			if (object)
			{
				var.Set_Basic(this, 70, 1);
				var.Set_Attack(object, 0.0, 0.0, true);
				Commands->Action_Attack(obj, var);
			}
			break;
		default:
			object = Commands->Find_Object(id3);
			if (object)
			{
				var.Set_Basic(this, 70, 1);
				var.Set_Attack(object, 0.0, 0.0, true);
				Commands->Action_Attack(obj, var);
			}
		}
	Commands->Start_Timer(obj, this, 10, 1);
	break;
	case 2:
		Commands->Action_Reset(obj, 100);
		break;
	}
}
void RA_Soviet_Defence::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,4,1);
	Auto_Save_Variable(&id2,4,1);
	Auto_Save_Variable(&id3,4,3);
	Auto_Save_Variable(&objtype,4,4);
}

//////////////RA_Soviet_Refinery_Controller//////////////

void RA_Soviet_Refinery_Controller::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *zone = Commands->Find_Object(Get_Int_Parameter("ScriptZoneID"));
	Commands->Send_Custom_Event(obj, zone, 0, 0, 0.0);
}

//////////////RA_Soviet_Refinery_Theft_Zone//////////////

void RA_Soviet_Refinery_Theft_Zone::Created(GameObject *obj)
{
	refinery = true;
}

void RA_Soviet_Refinery_Theft_Zone::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		refinery = false;
	}
}

void RA_Soviet_Refinery_Theft_Zone::Entered(GameObject *obj, GameObject *enterer)
{
	if ((refinery) && (Get_Int_Parameter("Thief_Preset_ID") == Commands->Get_Preset_ID(enterer)))
	{
		Commands->Send_Custom_Event(obj, enterer, 1, 0, 0.0);
	}
}

void RA_Soviet_Refinery_Theft_Zone::Exited(GameObject *obj, GameObject *exiter)
{
	if ((Get_Int_Parameter("Thief_Preset_ID") == Commands->Get_Preset_ID(exiter)))
	{
		Commands->Send_Custom_Event(obj, exiter, 0, 0, 0.0);
	}
}

void RA_Soviet_Refinery_Theft_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&refinery,1,1);
}

//////////////RA_Allied_Thief//////////////

void RA_Allied_Thief::Created(GameObject *obj)
{
	stealing = false;
	ready = true;
}

void RA_Allied_Thief::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 1)
	{
		stealing = true;
		if (ready) //check if thief has entered zone before timer expired
		{
			ready = false; //prevent another zone entry theft for now
			Commands->Start_Timer(obj,this,0.0,1); //immediately grant money
		}
	}
	else if (!type)
	{
		stealing = false;
	}
}

void RA_Allied_Thief::Timer_Expired(GameObject *obj, int number)
{
	if (stealing)
	{
		//Give to teammates
		Commands->Give_Money(obj,Get_Float_Parameter("Credits_To_Award"),true);
		//Steal from enemy (code doesn't work, so it was removed)
		//GameObject *victim = 0 //Don't have a way to handle this yet, 0 was a placeholder
		//Commands->Give_Money(victim, (-1 * (Get_Float_Parameter("Credits_To_Award"))), true);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Prevent_Theft_Timer"),1);
		ready = false; // just making sure this stays false for now
	}
	else
	{
		ready = true; // all timers have expired, thief can now get credits for zone entry again
	}
}

void RA_Allied_Thief::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&stealing,1,1);
	Auto_Save_Variable(&ready,1,2);
}

//////////////RA_Naval_PT//////////////

void RA_Naval_PT::Created(GameObject *obj)
{
	storedID = Get_Int_Parameter("TestID");
	Commands->Display_Health_Bar(obj, false);
	Commands->Set_Player_Type(obj, Get_Int_Parameter("Team"));
}

void RA_Naval_PT::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	//159159 = authorization response
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		bool send = false; //for later use
		//first check if player matches team
		int teamtest = Get_Int_Parameter("Team");
		if ((Commands->Get_Player_Type(sender)) == teamtest)
		{
			//ok, player is on my team, does he have the cash?
			float cost = Get_Float_Parameter("Cost");
			if (Commands->Get_Money(sender) >= cost)
			{
				//player has the money, are they allowed?
				storedID = Commands->Get_ID(sender); //remember for later
				send = true; //get ready to ask controller
			}
		}
		//Now to 'refresh' the PT
		//All the info we'll need from this terminal
		Vector3 pos = Commands->Get_Position(obj);
		float face = Commands->Get_Facing(obj);
		GameObject *newobj;
		char params[100];
		sprintf(params,"%s,%s,%s,%s,%s,%d",Get_Parameter("Preset_Name"),Get_Parameter("Cost"),Get_Parameter("Team"),Get_Parameter("Controller_ID"),Get_Parameter("Zone_ID"),storedID); //Get parameters
		//time to die
		Commands->Destroy_Object(obj);
		//Create new terminal and attach script
		newobj = Commands->Create_Object(Commands->Get_Preset_Name(obj), pos);
		Commands->Set_Facing(newobj, face);
		Commands->Attach_Script(newobj, "RA_Naval_PT", params);
		if (send) //should I bother the boss?
		{
			//Have the new PT send message so it receives response
			Commands->Send_Custom_Event(newobj, Commands->Find_Object(Get_Int_Parameter("Controller_ID")), 951951, 0, 0.0);
		}
	}
	else if (type == 159159)
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

void RA_Naval_PT::Timer_Expired(GameObject *obj, int number)
{
	GameObject *newobj;
	//Create the object
	newobj = Commands->Create_Object(Get_Parameter("Preset_Name"), Commands->Get_Position(Commands->Find_Object(Get_Int_Parameter("Zone_ID"))));
	Commands->Set_Facing(newobj, Commands->Get_Facing(Commands->Find_Object(Get_Int_Parameter("Zone_ID"))));//Set_Facing won't work on a script zone
	Commands->Attach_Script(newobj, "RA_Naval_Unit", Get_Parameter("Controller_ID"));
}

void RA_Naval_PT::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&storedID,4,1);
}

//////////////RA_Conyard_Controller//////////////

void RA_Conyard_Controller::Created(GameObject *obj)
{
	GameObject * object;
	int id = 0;
	int type = 3000;
	int param = 230032;
	id = Get_Int_Parameter("Building1_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 1);
	}
	id = Get_Int_Parameter("Building2_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 1);
	}
	id = Get_Int_Parameter("Building3_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 1);
	}
	id = Get_Int_Parameter("Building4_ID");
	if (id) 
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 1);
	}
	id = Get_Int_Parameter("Building5_ID");
	if (id) 
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 1);
	}
	id = Get_Int_Parameter("Building6_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 1);
	}
	id = Get_Int_Parameter("Building7_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 1);
	}
	id = Get_Int_Parameter("Building8_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 1);
	}
	id = Get_Int_Parameter("Building9_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 1);
	}
	id = Get_Int_Parameter("Building10_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 1);
	}
}

void RA_Conyard_Controller::Killed(GameObject * obj, GameObject * killer)
{
	GameObject * object;
	int id = 0;
	int type = 3000;
	int param = 320023;
	id = Get_Int_Parameter("Building1_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 0);
	}
	id = Get_Int_Parameter("Building2_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 0);
	}
	id = Get_Int_Parameter("Building3_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 0);
	}
	id = Get_Int_Parameter("Building4_ID");
	if (id) 
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 0);
	}
	id = Get_Int_Parameter("Building5_ID");
	if (id) 
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 0);
	}
	id = Get_Int_Parameter("Building6_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 0);
	}
	id = Get_Int_Parameter("Building7_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 0);
	}
	id = Get_Int_Parameter("Building8_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 0);
	}
	id = Get_Int_Parameter("Building9_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 0);
	}
	id = Get_Int_Parameter("Building10_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj, object, type, param, 0);
	}
}

//////////////Script Registrants//////////////

/********************************************************************/
ScriptRegistrant<RA_Thief_Script> RA_Thief_Script_Registrant("RA_Thief_Script","Credits_To_Award=500.00:float,Prevent_Theft_Timer=30.00:float,Thief_Preset_ID:int");
ScriptRegistrant<RA_Soviet_Defence> RA_Soviet_Defence_Registrant("RA_Soviet_Defence","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.00:float,Spy_Preset_ID:int,Aircraft_Preset_ID:int");
ScriptRegistrant<RA_Soviet_Refinery_Controller> RA_Soviet_Refinery_Controller_Registrant("RA_Soviet_Refinery_Controller", "ScriptZoneID=0:int");
ScriptRegistrant<RA_Soviet_Refinery_Theft_Zone> RA_Soviet_Refinery_Theft_Zone_Registrant("RA_Soviet_Refinery_Theft_Zone", "Thief_Preset_ID=0:int");
ScriptRegistrant<RA_Allied_Thief> RA_Allied_Thief_Registrant("RA_Allied_Thief", "Credits_To_Award=500.00:float, Prevent_Theft_Timer=30.00:float");
ScriptRegistrant<RA_Naval_PT> RA_Naval_PT_Registrant("RA_Naval_PT", "Preset_Name=blank:string,Cost=0.0:float,Team=0:int,Controller_ID=0:int,Zone_ID=0:int,TestID=0:int");
ScriptRegistrant<RA_Conyard_Controller> RA_Conyard_Controller_Registrant("RA_Conyard_Controller", "Building1_ID=0:int,Building2_ID=0:int,Building3_ID=0:int,Building4_ID=0:int,Building5_ID=0:int,Building6_ID=0:int,Building7_ID=0:int,Building8_ID=0:int,Building9_ID=0:int,Building10_ID=0:int");
