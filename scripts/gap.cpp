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
#include "gap.h"

//////////RA_Global_Gap_Controller/////////

void RA_Global_Gap_Controller::Created(GameObject *obj)
{
	ListHead = NULL;
	Commands->Start_Timer(obj, this, 5, Get_Int_Parameter("Timer_Number"));
}

void RA_Global_Gap_Controller::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	//message 1111 = Allied Team Generator Created 
	//message 1000 = Soviet Team Generator Created
	//message 2222 = Generator Switch State (param shows new state)
	//message 3333 = Generator Destroyed
	switch(type)
	{
	case 1111: //Add Allied Generator
		Add_Generator(Commands->Get_ID(sender), (float)param, 1);
		break;
	case 1000: //Add Soviet Generator
		Add_Generator(Commands->Get_ID(sender), (float)param, 0);
		break;
	//Change Generator Status -- make sure to check team type somehow...
	//param: 0 = Mobile Gap Offline, 1 = Mobile Gap Online, 2 = Gap Offline, 3 = Gap Online
	case 2222:
		switch(param)
		{
		case 0:
			Update_Generator(Commands->Get_ID(sender), false); //Don't Update Team
			break;
		case 1:
			Update_Generator(Commands->Get_ID(sender), true, Commands->Get_Player_Type(sender));
			break;
		case 2:
			Update_Generator(Commands->Get_ID(sender), false);
			break;
		case 3:
			Update_Generator(Commands->Get_ID(sender), true);
			break;
		}
		break;
	case 3333: //Remove Generator from List
		Remove_Generator(Commands->Get_ID(sender));
		break;
	}
}

void RA_Global_Gap_Controller::Timer_Expired(GameObject *obj, int number)
{
	if (number == Get_Int_Parameter("Timer_Number"))
	{
		Ranged_Gap_Effect(ListHead); //Ranged_Gap_Effect needs to know if there's a null list now
		Commands->Start_Timer(obj, this, Get_Float_Parameter("Update_Delay"), Get_Int_Parameter("Timer_Number"));
	}
}

void RA_Global_Gap_Controller::Destroyed(GameObject *obj)
{
	//Destroy List
	Gap_ListNode *Current = ListHead;
	Gap_ListNode *Previous = NULL;
	while (Current != NULL)
	{
		Previous = Current;
		Current = Current->Next;
		Previous->Next = NULL; //safety
		delete Previous;
		Previous = NULL;
	}
	ListHead = NULL;
}

void RA_Global_Gap_Controller::Add_Generator(unsigned long ObjectID, float Range, int Team)
{
	if (!ListHead) //no list exists yet
	{
		ListHead = new Gap_ListNode;
		ListHead->Next = NULL;
		ListHead->unitID = ObjectID;
		ListHead->range = Range;
		ListHead->teamID = Team;
		ListHead->active = false; //all generators initialize off
	}
	else //List exists, add node
	{
		Gap_ListNode *Current = ListHead;
		while (Current->Next != NULL)
		{
			if (Current->unitID == ObjectID)
			{	//Same ID, replace current node 
				Current->range = Range;
				Current->teamID = Team;
				Current->active = false; //all generators initialize off
				return; //job done, end now
			}
			Current = Current->Next;
		}
		//At End of list, Tack on new node
		Current->Next = new Gap_ListNode;
		Current = Current->Next; //Switch to new node
		//Initialize new node
		Current->Next = NULL;
		Current->unitID = ObjectID;
		Current->range = Range;
		Current->teamID = Team;
		Current->active = false; //all generators initialize off
	}
}

void RA_Global_Gap_Controller::Remove_Generator(unsigned long ID)
{
	if (ListHead) //No need to remove if list doesn't exist
	{
		Gap_ListNode *Previous = ListHead;
		do //Check if ListHead is pointing to a deletable Node
		{
			if (ListHead->unitID == ID)
			{
				ListHead = ListHead->Next;
				delete Previous;
				Previous = ListHead;
				if (ListHead == NULL)
				{
					return; //Hit the End of the List
				}
			}
		}
		while (ListHead->unitID == ID); //Continue until ListHead no longer is effected by change
		Gap_ListNode *Current = ListHead->Next;
		while (Current)
		{
			if (Current->unitID == ID)
			{
				Previous->Next = Current->Next;
				Current->Next = NULL; //safety
				delete Current;
				Current = Previous->Next;
			}
			else
			{
				Previous = Current;
				Current = Current->Next;
			}
		}
	}
}

void RA_Global_Gap_Controller::Update_Generator(unsigned long ID, bool Active, int Team)
{
	if (ListHead) //always check for List's existence
	{
		Gap_ListNode *Current = ListHead;
		while (Current)
		{
			if (Current->unitID == ID)
			{
				Current->active = Active;
				Current->teamID = Team;
			}
			Current = Current->Next;
		}
	}
}
	
void RA_Global_Gap_Controller::Update_Generator(unsigned long ID, bool Active)
{
	if (ListHead) //always check for List's existence
	{
		Gap_ListNode *Current = ListHead;
		while (Current)
		{
			if (Current->unitID == ID)
			{
				Current->active = Active;
			}
			Current = Current->Next;
		}
	}
}

//////////RA_Gap_Generator_Building/////////

//message 1111 = Allied Team Generator Created 
//message 1000 = Soviet Team Generator Created
//message 2222 = Generator Switch State (param shows new state)
//message 3333 = Generator Destroyed

void RA_Gap_Generator_Building::Created(GameObject *obj)
{
	GameObject *GapControl = Find_Object_By_Preset(2, Get_Parameter("Gap_Controller"));
	if (!Get_Int_Parameter("Team"))
	{
		Commands->Send_Custom_Event(obj, GapControl, 1000, Get_Int_Parameter("Stealth_Range"), 2);
		Commands->Send_Custom_Event(obj, GapControl, 2222, 3, 3);
	}
	else if (Get_Int_Parameter("Team") == 1)
	{
		Commands->Send_Custom_Event(obj, GapControl, 1111, Get_Int_Parameter("Stealth_Range"), 2);
		Commands->Send_Custom_Event(obj, GapControl, 2222, 3, 3);
	}
	shroudID = 0;
	sizeID = 0;
	Vector3 position = Commands->Get_Position(obj);
	GameObject *object = Commands->Create_Object(Get_Parameter("ShroudPreset"), position);
	Commands->Set_Player_Type(object,Commands->Get_Player_Type(obj));
	shroudID = Commands->Get_ID(object);
	GameObject *object2 = Commands->Create_Object(Get_Parameter("SizePreset"), position);
	Commands->Set_Player_Type(object2,Commands->Get_Player_Type(obj));
	sizeID = Commands->Get_ID(object2);
	Commands->Start_Timer(obj, this, 2, Get_Int_Parameter("Timer_Number"));
}

void RA_Gap_Generator_Building::Timer_Expired(GameObject *obj, int number)
{
	if (number == Get_Int_Parameter("Timer_Number"))
	{
		if (!Is_Base_Powered(Get_Object_Type(obj)))
		{
			GameObject *GapControl = Find_Object_By_Preset(2, Get_Parameter("Gap_Controller"));
			Commands->Send_Custom_Event(obj, GapControl, 2222, 2, 0);
			if (shroudID)
			{
				Commands->Destroy_Object(Commands->Find_Object(shroudID));
				shroudID = 0;
			}
			if (sizeID)
			{
				Commands->Destroy_Object(Commands->Find_Object(sizeID));
				sizeID = 0;
			}
		}
		Commands->Start_Timer(obj, this, 2, Get_Int_Parameter("Timer_Number"));
	}
}
	
void RA_Gap_Generator_Building::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *GapControl = Find_Object_By_Preset(2, Get_Parameter("Gap_Controller"));
	Commands->Send_Custom_Event(obj, GapControl, 3333, 0, 0);
	if (shroudID)
	{
		Commands->Destroy_Object(Commands->Find_Object(shroudID));
		shroudID = 0;
	}
	if (sizeID)
	{
		Commands->Destroy_Object(Commands->Find_Object(sizeID));
		sizeID = 0;
	}
}

//////////RA_Gap_Generator_Vehicle/////////

void RA_Gap_Generator_Vehicle::Created(GameObject *obj)
{
	shroudID = 0;
	sizeID = 0;
	GameObject *GapControl = Find_Object_By_Preset(2, Get_Parameter("Gap_Controller"));
	if (!Commands->Get_Player_Type(obj))
	{
		Commands->Send_Custom_Event(obj, GapControl, 1000, Get_Int_Parameter("Stealth_Range"), 0.5);
	}
	else if (Commands->Get_Player_Type(obj) == 1)
	{
		Commands->Send_Custom_Event(obj, GapControl, 1111, Get_Int_Parameter("Stealth_Range"), 0.5);
	}
}

void RA_Gap_Generator_Vehicle::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	GameObject *GapControl = Find_Object_By_Preset(2, Get_Parameter("Gap_Controller"));
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		Commands->Send_Custom_Event(obj, GapControl, 2222, 1, 0); //Activate Stealth Field
		if (!shroudID)
		{
			Vector3 position = Commands->Get_Bone_Position(obj, "ROOTTRANSFORM");
			GameObject *object = Commands->Create_Object(Get_Parameter("ShroudPreset"), position);
			Commands->Set_Player_Type(object,Commands->Get_Player_Type(obj));
			Commands->Attach_To_Object_Bone(object, obj, "ROOTTRANSFORM");
			shroudID = Commands->Get_ID(object);
		}
		if (!sizeID)
		{
			Vector3 position = Commands->Get_Bone_Position(obj, "ROOTTRANSFORM");
			GameObject *object = Commands->Create_Object(Get_Parameter("SizePreset"), position);
			Commands->Set_Player_Type(object,Commands->Get_Player_Type(obj));
			Commands->Attach_To_Object_Bone(object, obj, "ROOTTRANSFORM");
			sizeID = Commands->Get_ID(object);
		}
	}
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		Commands->Send_Custom_Event(obj, GapControl, 2222, 0, 0); //Deactivate Stealth Field
		if (shroudID)
		{
			Commands->Destroy_Object(Commands->Find_Object(shroudID));
			shroudID = 0;
		}
		if (sizeID)
		{
			Commands->Destroy_Object(Commands->Find_Object(sizeID));
			sizeID = 0;
		}
	}
}

void RA_Gap_Generator_Vehicle::Destroyed(GameObject *obj)
{
	GameObject *GapControl = Find_Object_By_Preset(2, Get_Parameter("Gap_Controller"));
	Commands->Send_Custom_Event(obj, GapControl, 3333, 0, 0);
	if (shroudID)
	{
		Commands->Destroy_Object(Commands->Find_Object(shroudID));
		shroudID = 0;
	}
	if (sizeID)
	{
		Commands->Destroy_Object(Commands->Find_Object(sizeID));
		sizeID = 0;
	}
}

//////////RA_Gap_Generator_Ignored/////////

//////////////Script Registrants//////////////

/********************************************************************/
ScriptRegistrant<RA_Global_Gap_Controller> RA_Global_Gap_Controller_Registrant("RA_Global_Gap_Controller","Update_Delay=2.0:float,Timer_Number=687:int");
ScriptRegistrant<RA_Gap_Generator_Building> RA_Gap_Generator_Building_Registrant("RA_Gap_Generator_Building","Timer_Number=876:int,Stealth_Range=50:int,Gap_Controller=RA_Game_Manager:string,Team=1:int,ShroudPreset:string,SizePreset:string");
ScriptRegistrant<RA_Gap_Generator_Vehicle> RA_Gap_Generator_Vehicle_Registrant("RA_Gap_Generator_Vehicle","Stealth_Range=50:int,Gap_Controller=RA_Game_Manager:string,ShroudPreset:string,SizePreset:string");
ScriptRegistrant<RA_Gap_Generator_Ignored> RA_Gap_Generator_Ignored_Registrant("RA_Gap_Generator_Ignored","");
