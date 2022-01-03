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
#include "tda.h"

void TDA_Stealth_Armor::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Commands->Attach_Script(sender,"TDA_Stealth_Armor",Get_Parameter("Stealth_Length"));
		Commands->Send_Custom_Event(obj,sender,1000,1,0);
	}
	else if (type == 1000)
	{
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Stealth_Length"),1);
		Commands->Enable_Stealth(obj,1);
	}
}

void TDA_Stealth_Armor::Timer_Expired(GameObject *obj,int number)
{
	Commands->Enable_Stealth(obj,0);
}

void TDA_Send_Custom_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (CheckPlayerType(enterer,Get_Int_Parameter("Team_ID")))
		return;
	GameObject *object = Commands->Find_Object(Get_Int_Parameter("ID"));
	int type;
	int param;
	type = Get_Int_Parameter("EnterMessage");
	param = Get_Int_Parameter("EnterParam");
	if (type)
	{
		if (!param)
		{
			param = Commands->Get_ID(enterer);
		}
		Commands->Send_Custom_Event(obj,object,type,param,0);
	}
}

void TDA_Send_Custom_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (CheckPlayerType(exiter,Get_Int_Parameter("Team_ID")))
		return;
	GameObject *object = Commands->Find_Object(Get_Int_Parameter("ID"));
	int type;
	int param;
	type = Get_Int_Parameter("ExitMessage");
	param = Get_Int_Parameter("ExitParam");
	if (type)
	{
		if (!param)
		{
			param = Commands->Get_ID(exiter);
		}
		Commands->Send_Custom_Event(obj,object,type,param,0);
	}
}

void TDA_Conyard_Controller::Killed(GameObject *obj,GameObject *killer)
{
	Enable_Repairs(obj,false);
}

void TDA_Conyard_Controller::Custom(GameObject *obj,int type,int param,GameObject *sender) 
{
	if (type == CUSTOM_EVENT_BUILDING_REVIVED)
	{
		Enable_Repairs(obj,true);
	}
}

void TDA_Conyard_Controller::Enable_Repairs(GameObject *obj,bool Enable) 
{
	GameObject *object = 0;
	int id = 0;
	int type = Enable?3001:3000;
	int param = type;
	id = Get_Int_Parameter("Building1_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj,object,type,param,0);
	}
	id = Get_Int_Parameter("Building2_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj,object,type,param,0);
	}
	id = Get_Int_Parameter("Building3_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj,object,type,param,0);
	}
	id = Get_Int_Parameter("Building4_ID");
	if (id) 
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj,object,type,param,0);
	}
	id = Get_Int_Parameter("Building5_ID");
	if (id) 
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj,object,type,param,0);
	}
	id = Get_Int_Parameter("Building6_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj,object,type,param,0);
	}
	id = Get_Int_Parameter("Building7_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj,object,type,param,0);
	}
	id = Get_Int_Parameter("Building8_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj,object,type,param,0);
	}
	id = Get_Int_Parameter("Building9_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj,object,type,param,0);
	}
	id = Get_Int_Parameter("Building10_ID");
	if (id)
	{
		object = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj,object,type,param,0);
	}
}

void TDA_User_Purchased_VTOL_Object::Created(GameObject *obj)
{
	Vector3 spawn_position;
	spawn_position = Get_Vector3_Parameter("HelipadLocation");
	Commands->Create_Object(Get_Parameter("Preset_Name"),spawn_position);
	Commands->Destroy_Object(obj);
}

void TDA_Helipad_Controller::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	int type = 11000;
	int param = 0;
	object = Commands->Find_Object(Get_Int_Parameter("VTOL_Controller1"));
	Commands->Send_Custom_Event(obj,object,type,param,0);
	object = Commands->Find_Object(Get_Int_Parameter("VTOL_Controller2"));
	Commands->Send_Custom_Event(obj,object,type,param,0);
	object = Commands->Find_Object(Get_Int_Parameter("VTOL_Controller3"));
	Commands->Send_Custom_Event(obj,object,type,param,0);
	object = Commands->Find_Object(Get_Int_Parameter("VTOL_Controller4"));
	Commands->Send_Custom_Event(obj,object,type,param,0);
}

void TDA_VTOL_Controller::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 11000)
	{
		Commands->Destroy_Object(obj);
		return;
	}
	if (type == 11001)
	{
		Vector3 spawn_location = Commands->Get_Position(obj);
		Commands->Create_Object(Get_Parameter("Cinematic_Object"),spawn_location);
		return;
	}
	return;
}

void TDA_VTOL_Object::Created(GameObject *obj)
{
	GameObject *object;
	object = Commands->Find_Object(Get_Int_Parameter("VTOL_Controller"));
	Commands->Send_Custom_Event(obj,object,11001,0,0);
	Commands->Destroy_Object(obj);
}

void TDA_Toggle_Stealth_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (Get_Int_Parameter("Trigger_Type") == 1)
		return;
	if (CheckPlayerType(enterer,Get_Int_Parameter("Player_Type")))
		return;
	Commands->Enable_Stealth(enterer,1);
}

void TDA_Toggle_Stealth_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (Get_Int_Parameter("Trigger_Type") == 2)
		return;
	if (CheckPlayerType(exiter,Get_Int_Parameter("Player_Type")))
		return;
	Commands->Enable_Stealth(exiter,0);
}

void TDA_Teleport_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	Vector3 spawn_position;
	spawn_position = Get_Vector3_Parameter("Location");
	int x = Get_Int_Parameter("Object_ID");
	if (x)
	{
		GameObject *gotoObject = Commands->Find_Object(x);
		Vector3 gotoLocation = Commands->Get_Position(gotoObject);
		Commands->Set_Position(enterer,gotoLocation);
	}
	else
		Commands->Set_Position(enterer,spawn_position);
}

void TDA_Disable_Building_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	GameObject *sendTo = Commands->Find_Object(Get_Int_Parameter("Building_ID"));
	if (CheckPlayerType(enterer,Get_Int_Parameter("Team_ID")))
		return;
	Commands->Send_Custom_Event(obj,sendTo,1000082,0,0);
}

void TDA_Disable_Building_Zone::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 1000082)
		Commands->Destroy_Object(obj);
	else
		return;
}

void TDA_Conyard_Repair::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 3000)
	{
		Enabled = false;
	}
	else if (type == 3001)
	{
		Enabled = true;
	}
}

void TDA_Conyard_Repair::Created(GameObject *obj)
{
	Enabled = true;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Repair_Frequency"),Get_Int_Parameter("Timer_ID"));
}

void TDA_Conyard_Repair::Timer_Expired(GameObject *obj,int number)
{
	float amount = Commands->Get_Health(obj);
	if (amount > 0 && Enabled)
	{
		Commands->Set_Health(obj,(amount+2));
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Repair_Frequency"),Get_Int_Parameter("Timer_ID"));
}

void TDA_Conyard_Repair::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&Enabled,4,1);
}

void TDA_Stealth_Generator::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *zoneObj = Commands->Find_Object(Get_Int_Parameter("Zone_ID"));
	Commands->Send_Custom_Event(obj,zoneObj,100800,100800,0);
}

void TDA_Stealth_Generator_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (Disabled)
		return;
	if (CheckPlayerType(enterer,Get_Int_Parameter("Player_Type")))
		return;
	Commands->Enable_Stealth(enterer,1);
}

void TDA_Stealth_Generator_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (CheckPlayerType(exiter,Get_Int_Parameter("Player_Type")))
		return;
	Commands->Enable_Stealth(exiter,0);
}

void TDA_Stealth_Generator_Zone::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type != 100800)
		return;
	Disabled = true;
}

void TDA_Stealth_Generator_Zone::Created(GameObject *obj)
{
	Disabled = false;
}

void TDA_Stealth_Generator_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&Disabled,4,1);
}

void TDA_CTF_Attached::Created(GameObject *obj)
{
	MyFlagId = 0;
	ZoneCapturedId = 0;
}

void TDA_CTF_Attached::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&MyFlagId,4,1);
	Auto_Save_Variable(&ZoneCapturedId,4,1);
}

void TDA_CTF_Attached::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *flagObj = Commands->Find_Object(MyFlagId);
	Commands->Destroy_Object(flagObj);
	Commands->Send_Custom_Event(obj,Commands->Find_Object(ZoneCapturedId),10005,0,0);
	ZoneCapturedId = 0;
	Commands->Give_Points(killer,100,false);
}

void TDA_CTF_Attached::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 10000)
		Commands->Send_Custom_Event(obj,sender,10001,Commands->Get_ID(obj),0);
	else if (type == 10002)
	{
		GameObject *flagObj = Commands->Find_Object(MyFlagId);
		Commands->Destroy_Object(flagObj);
		Commands->Send_Custom_Event(obj,Commands->Find_Object(ZoneCapturedId),10005,0,0);
		ZoneCapturedId = 0;
		MyFlagId = 0;
	}
	else if (type == 10003)
		MyFlagId = param;
	else if ((type == 10004) && (MyFlagId))
		Commands->Send_Custom_Event(obj,Commands->Find_Object(ZoneCapturedId),10002,1,0);
	else if (type == 10005)
		ZoneCapturedId = param;
}

void TDA_CTF_Zone::Created(GameObject *obj)
{
	GotFlag = false;
	NumCaptured = 0;
	PlayerId = 0;
	FlagId = 0;
	Vector3 newFlagPos = Commands->Get_Position(obj);
	GameObject *newFlag = Commands->Create_Object(Get_Parameter("Flag_Preset_Name"),newFlagPos);
	DecorFlag = Commands->Get_ID(newFlag);
	DecorFlagPresent = false;
}

void TDA_CTF_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (Get_Int_Parameter("Team_ID") != Commands->Get_Player_Type(enterer))
		Commands->Send_Custom_Event(obj,enterer,10004,0,0);				//ask player if it has the flag
	else if (!GotFlag)
	{
		Commands->Attach_Script(enterer,"TDA_CTF_Attached","0");			//load up the player script
		GameObject *tmpObj = Commands->Create_Object_At_Bone(enterer,Get_Parameter("Flag_Preset_Name"),"LHAND"); //create the flag object
		int objID = Commands->Get_ID(tmpObj);								//get the new flags ID
		FlagId = objID;
		Commands->Send_Custom_Event(obj,enterer,10003,objID,0);			//tell the player script what the flag id is
		Commands->Attach_To_Object_Bone(tmpObj,enterer,"LHAND");			//attach flag to player
		objID = Commands->Get_ID(obj);										//get the current zones ID
		Commands->Send_Custom_Event(obj,enterer,10005,objID,0);			//let the player know the ID of zone
		PlayerId = Commands->Get_ID(enterer);
		GotFlag = true;														//let zone know NOT to create a new flag
		Commands->Start_Timer(obj,this,10,411);							//needed to add this for the suicide and lag flags
		if (Get_Int_Parameter("Play_Capture_Sounds") == 1)
		{
			Vector3 SoundLocal = Commands->Get_Position(obj);
			Commands->Create_Sound("SFX.Flag_Pickup",SoundLocal,obj);
		}
		Commands->Give_Points(enterer,75,false);
		Commands->Destroy_Object(Commands->Find_Object(DecorFlag));
		DecorFlagPresent = false;
	}
}

void TDA_CTF_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&GotFlag,4,1);
	Auto_Save_Variable(&NumCaptured,4,2);
	Auto_Save_Variable(&DecorFlag,4,3);
	Auto_Save_Variable(&DecorFlagPresent,4,4);
	Auto_Save_Variable(&FlagId,4,5);
	Auto_Save_Variable(&PlayerId,4,6);
}

void TDA_CTF_Zone::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	GameObject *newFlag;
	Vector3 newFlagPos;
	if ((type == 10002) && (GotFlag))
	{
		float maxHealth;
		GotFlag = false;													//tell zone that flag can be recaptured
		Commands->Send_Custom_Event(obj,sender,10002,1,0);				//tell player script "kill your flag,it is now captured"
		NumCaptured++;														//increment the # of captures
		newFlagPos = Commands->Get_Position(obj);
		newFlag = Commands->Create_Object(Get_Parameter("Flag_Preset_Name"),newFlagPos);
		DecorFlag = Commands->Get_ID(newFlag);
		Commands->Give_Points(sender,250,false);
		if (Get_Int_Parameter("Play_Capture_Sounds") == 1)
		{
			Vector3 SoundLocal = Commands->Get_Position(obj);
			Commands->Create_Sound("SFX.Flag_Capture",SoundLocal,obj);
		}
		if (NumCaptured == Get_Int_Parameter("Max_Capture"))								//check to see if the max captures have been met
		{
			GameObject *tmpObj;
			tmpObj = Commands->Find_Object(Get_Int_Parameter("Building_To_Destroy1"));				//start getting all the Building ID's and Destorying them for game over.
			maxHealth = Commands->Get_Max_Health(tmpObj);					//get buildings max health
			Commands->Apply_Damage(tmpObj,10000,"Explosive",0);			//Apply_Damage (only one that don't crash game)
			tmpObj = Commands->Find_Object(Get_Int_Parameter("Building_To_Destroy2"));				//start getting all the Building ID's and Destorying them for game over.
			maxHealth = Commands->Get_Max_Health(tmpObj);					//get buildings max health
			Commands->Apply_Damage(tmpObj,10000,"Explosive",0);			//Apply_Damage (only one that don't crash game)
			tmpObj = Commands->Find_Object(Get_Int_Parameter("Building_To_Destroy3"));				//start getting all the Building ID's and Destorying them for game over.
			maxHealth = Commands->Get_Max_Health(tmpObj);					//get buildings max health
			Commands->Apply_Damage(tmpObj,10000,"Explosive",0);			//Apply_Damage (only one that don't crash game)
			tmpObj = Commands->Find_Object(Get_Int_Parameter("Building_To_Destroy4"));				//start getting all the Building ID's and Destorying them for game over.
			maxHealth = Commands->Get_Max_Health(tmpObj);					//get buildings max health
			Commands->Apply_Damage(tmpObj,10000,"Explosive",0);			//Apply_Damage (only one that don't crash game)
			tmpObj = Commands->Find_Object(Get_Int_Parameter("Building_To_Destroy5"));				//start getting all the Building ID's and Destorying them for game over.
			maxHealth = Commands->Get_Max_Health(tmpObj);					//get buildings max health
			Commands->Apply_Damage(tmpObj,10000,"Explosive",0);			//Apply_Damage (only one that don't crash game)
		}
	}
	else if ((type == 10005) && (GotFlag))
	{
		if (Get_Int_Parameter("Play_Capture_Sounds") == 1)
		{
			Vector3 SoundLocal = Commands->Get_Position(obj);
			Commands->Create_Sound("SFX.Flag_Drop",SoundLocal,obj);
		}
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

void TDA_CTF_Zone::Timer_Expired(GameObject *obj,int number)
{
	if (GotFlag)
	{
		Vector3 fPos = Commands->Get_Position(Commands->Find_Object(FlagId));
		Vector3 pPos = Commands->Get_Position(Commands->Find_Object(PlayerId));
		float dPos = Commands->Get_Distance(fPos,pPos);

		if (dPos > 2)
		{
			GameObject *flagObj = Commands->Find_Object(FlagId);
			Commands->Destroy_Object(flagObj);
			Commands->Send_Custom_Event(obj,obj,10005,0,0);
			FlagId = 0;
			PlayerId = 0;
			GotFlag = false;
		}
		Commands->Destroy_Object(Commands->Find_Object(DecorFlag));
		DecorFlagPresent = false;
		Commands->Start_Timer(obj,this,10,411);
	}
}

ScriptRegistrant<TDA_Stealth_Armor> TDA_Stealth_Armor_Registrant("TDA_Stealth_Armor","Stealth_Length=60.0:float");
ScriptRegistrant<TDA_Send_Custom_Zone> TDA_Send_Custom_Zone_Registrant("TDA_Send_Custom_Zone","ID:int,EnterMessage:int,EnterParam:int,ExitMessage:int,ExitParam:int,Team_ID:int");
ScriptRegistrant<TDA_Conyard_Controller> TDA_Conyard_Controller_Registrant("TDA_ConYard_Controller","Building1_ID=0:int,Building2_ID=0:int,Building3_ID=0:int,Building4_ID=0:int,Building5_ID=0:int,Building6_ID=0:int,Building7_ID=0:int,Building8_ID=0:int,Building9_ID=0:int,Building10_ID=0:int");
ScriptRegistrant<TDA_User_Purchased_VTOL_Object> TDA_User_Purchased_VTOL_Object_Registrant("TDA_User_Purchased_VTOL_Object","Preset_Name:string,HelipadLocation:vector3");
ScriptRegistrant<TDA_Helipad_Controller> TDA_Helipad_Controller_Registrant("TDA_Helipad_Controller","VTOL_Controller1=0:int,VTOL_Controller2=0:int,VTOL_Controller3=0:int,VTOL_Controller4=0:int");
ScriptRegistrant<TDA_VTOL_Controller> TDA_VTOL_Controller_Registrant("TDA_VTOL_Controller","Cinematic_Object:string");
ScriptRegistrant<TDA_VTOL_Object> TDA_VTOL_Object_Registrant("TDA_VTOL_Object","VTOL_Controller=0:int");
ScriptRegistrant<TDA_Toggle_Stealth_Zone> TDA_Toggle_Stealth_Zone_Registrant("TDA_Toggle_Stealth_Zone","Trigger_Type:int,Player_Type:int");
ScriptRegistrant<TDA_Teleport_Zone> TDA_Teleport_Zone_Registrant("TDA_Teleport_Zone","Location:vector3,Object_ID=0:int");
ScriptRegistrant<TDA_Disable_Building_Zone> TDA_Disable_Building_Zone_Registrant("TDA_Disable_Building_Zone","Building_ID:int,Team_ID:int");
ScriptRegistrant<TDA_Conyard_Repair> TDA_Conyard_Repair_Registrant("TDA_Conyard_Repair","Repair_Frequency=1.0:float,Timer_ID=0:int");
ScriptRegistrant<TDA_Stealth_Generator> TDA_Stealth_Generator_Registrant("TDA_Stealth_Generator","Zone_ID=0:int");
ScriptRegistrant<TDA_Stealth_Generator_Zone> TDA_Stealth_Generator_Zone_Registrant("TDA_Stealth_Generator_Zone","Player_Type=0:int");
ScriptRegistrant<TDA_CTF_Attached> TDA_CTF_Attached_Registrant("TDA_CTF_Attached","");
ScriptRegistrant<TDA_CTF_Zone> TDA_CTF_Zone_Registrant("TDA_CTF_Zone","Team_ID=0:int,Max_Capture=5:int,Flag_Preset_Name:string,Building_To_Destroy1=0:int,Building_To_Destroy2=0:int,Building_To_Destroy3=0:int,Building_To_Destroy4=0:int,Building_To_Destroy5=0:int,Play_Capture_Sounds=1:int");
