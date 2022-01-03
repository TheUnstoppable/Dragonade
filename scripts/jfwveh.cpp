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
#include "jfwveh.h"
#include "VehicleGameObj.h"
#include "WeaponClass.h"
#include "VehicleGameObjDef.h"

void JFW_Aircraft_Fuel::Created(GameObject *obj)
{
	entered = false;
}

void JFW_Aircraft_Fuel::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	float timertime;
	int timernumber;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	msg = Get_Int_Parameter("Refuel_Message");
	if (msg == type)
	{
		Commands->Start_Timer(obj,this,timertime,timernumber);
	}
	if ((type == CUSTOM_EVENT_VEHICLE_ENTERED) && (!entered))
	{
		Commands->Start_Timer(obj,this,timertime,timernumber);
		entered = true;
	}
}

void JFW_Aircraft_Fuel::Timer_Expired(GameObject *obj,int number)
{
	int timernumber;
	const char *c;
	Vector3 v;
	timernumber = Get_Int_Parameter("TimerNum");
	c = Get_Parameter("Explosion");
	v = Commands->Get_Position(obj);
	if (number == timernumber)
	{
		Commands->Create_Explosion(c,v,0);
	}
}

void JFW_Aircraft_Fuel::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&entered,1,1);
}

void JFW_Turret_Spawn_2::Created(GameObject *obj)
{
	Vector3 sp;
	GameObject *object;
	sp.X = 0;
	sp.Y = 0;
	sp.Z = 0;
	object = Commands->Create_Object(Get_Parameter("Turret_Preset"),sp);
	Commands->Attach_To_Object_Bone(object,obj,Get_Parameter("Bone_Name"));
	turID = Commands->Get_ID(object);
}

void JFW_Turret_Spawn_2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		Commands->Set_Player_Type(Commands->Find_Object(turID),Commands->Get_Player_Type(sender));
	}
}

void JFW_Turret_Spawn_2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&turID,4,1);
}

void JFW_Turret_Spawn_2::Killed(GameObject *obj,GameObject *killer)
{
	Vector3 tmb;
	tmb = Commands->Get_Bone_Position(obj,Get_Parameter("Explosion_Bone"));
	Commands->Create_Explosion(Get_Parameter("Explosion_Preset"),tmb,0);
}

void JFW_Turret_Spawn_3::Created(GameObject *obj)
{
  Vector3 sp;
  GameObject *object;
  sp.X = 0;
  sp.Y = 0;
  sp.Z = 0;
  object = Commands->Create_Object(Get_Parameter("Turret_Preset"),sp);
  if(NULL == object)
  {
    Console_Output("[%d:%s:%s] Critical Error: Failed to create an instance of the preset %s. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Parameter("Turret_Preset"));
    Destroy_Script();
    return;
  }

  Commands->Attach_To_Object_Bone(object,obj,Get_Parameter("Bone_Name"));
  turID = Commands->Get_ID(object);
  Commands->Send_Custom_Event(obj,Commands->Find_Object(turID),Get_Int_Parameter("Driver_Exited_Custom"),0,0);
  Attach_Script_Once_V(object,"dp88_linkHealth","%d",Commands->Get_ID(obj));
  if (object->As_VehicleGameObj())
  {
    object->As_VehicleGameObj()->Set_Is_Scripts_Visible(false);
  }
  m_bHasDriver = false;
}

void JFW_Turret_Spawn_3::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
  if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
  {
    if ( !m_bHasDriver )
    {
      m_bHasDriver = true;
      Commands->Set_Player_Type(Commands->Find_Object(turID),Commands->Get_Player_Type(sender));
      Commands->Send_Custom_Event(obj,Commands->Find_Object(turID),Get_Int_Parameter("Driver_Entered_Custom"),0,0);
    }
  }
  if (type == CUSTOM_EVENT_VEHICLE_EXITED)
  {
    if ( m_bHasDriver && obj->As_VehicleGameObj() && obj->As_VehicleGameObj()->Get_Occupant(0) == NULL )
    {
      m_bHasDriver = false;
      Commands->Send_Custom_Event(obj,Commands->Find_Object(turID),Get_Int_Parameter("Driver_Exited_Custom"),0,0);
    }
  }
}

void JFW_Turret_Spawn_3::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&turID,4,1);
}

void JFW_Turret_Spawn_3::Killed(GameObject *obj,GameObject *killer)
{
	Vector3 tmb;
	tmb = Commands->Get_Bone_Position(obj,Get_Parameter("Explosion_Bone"));
	Commands->Create_Explosion(Get_Parameter("Explosion_Preset"),tmb,0);
}

void JFW_Drive_To_Player::Created(GameObject *obj)
{
	ActionParamsStruct params;
	params.MoveArrivedDistance = 100;
	params.Set_Basic(this,100,777);
	params.Set_Movement(Commands->Get_A_Star(Commands->Get_Position(obj)),Get_Float_Parameter("Speed"),Get_Float_Parameter("Arrive_Distance"));
	Commands->Action_Goto(obj,params);
	Destroy_Script();
}

void JFW_Vechicle_Animation::Timer_Expired(GameObject *obj,int number)
{
	Vector3 position = Commands->Get_Position(obj);
	if ((!xpos) && (!ypos) && (!zpos))
	{
		xpos = position.X;
		ypos = position.Y;
		zpos = position.Z;
	}
	if ((position.X != xpos) || (position.Y != ypos) || (position.Z != zpos))
	{
		xpos = position.X;
		ypos = position.Y;
		zpos = position.Z;
		if (!enabled)
		{
			enabled = true;
			const char *subobject = Get_Parameter("Subobject");
			if (!_stricmp(subobject,"0"))
			{
				subobject = 0;
			}
			float firstframe = Get_Float_Parameter("FirstFrame");
			if (firstframe == -1)
			{
				firstframe = Get_Animation_Frame(obj);
			}
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,subobject,firstframe,Get_Float_Parameter("LastFrame"),Get_Bool_Parameter("Blended"));
		}
	}
	else 
	{
		obj->As_PhysicalGameObj()->Clear_Animation();
		enabled = false;
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Vechicle_Animation::Animation_Complete ( GameObject *obj, const char *animation_name )
{
	if (!_stricmp(animation_name,Get_Parameter("Animation")))
	{
		obj->As_PhysicalGameObj()->Clear_Animation();
		const char *subobject = Get_Parameter("Subobject");
		if (!_stricmp(subobject,"0"))
		{
			subobject = 0;
		}
		float firstframe = Get_Float_Parameter("FirstFrame");
		if (firstframe == -1)
		{
			firstframe = Get_Animation_Frame(obj);
		}
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,subobject,firstframe,Get_Float_Parameter("LastFrame"),Get_Bool_Parameter("Blended"));
	}
}

void JFW_Vechicle_Animation::Created(GameObject *obj)
{
	xpos = 0;
	ypos = 0;
	zpos = 0;
	enabled = false;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Vechicle_Animation::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
	Auto_Save_Variable(&xpos,4,2);
	Auto_Save_Variable(&ypos,4,3);
	Auto_Save_Variable(&zpos,4,4);
}

void JFW_Visible_Person_In_Vechicle::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		Vector3 position = Commands->Get_Bone_Position(obj,Get_Parameter("BoneName"));
		GameObject *object = Commands->Create_Object("Invisible_Object",position);
		Commands->Attach_To_Object_Bone(object,obj,Get_Parameter("BoneName"));
		Commands->Set_Model(object,Get_Parameter("ModelName"));
		modelid = Commands->Get_ID(object);
		const char *subobject = Get_Parameter("Subobject");
		if (!_stricmp(subobject,"0"))
		{
			subobject = 0;
		}
		float firstframe = Get_Float_Parameter("FirstFrame");
		if (firstframe == -1)
		{
			firstframe = Get_Animation_Frame(obj);
		}
		Commands->Set_Animation(object,Get_Parameter("Animation"),false,subobject,firstframe,Get_Float_Parameter("LastFrame"),Get_Bool_Parameter("Blended"));
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		Commands->Destroy_Object(Commands->Find_Object(modelid));
		modelid = 0;
	}
}

void JFW_Visible_Person_In_Vechicle::Created(GameObject *obj)
{
	modelid = 0;
}

void JFW_Visible_Person_In_Vechicle::Killed(GameObject *obj,GameObject *killer)
{
	if (modelid)
	{
		Commands->Destroy_Object(Commands->Find_Object(modelid));
	}
}

void JFW_Visible_Person_In_Vechicle::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&modelid,4,1);
}

void JFW_Visible_People_In_Vechicle::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!modelid1)
		{
			Vector3 position = Commands->Get_Bone_Position(obj,Get_Parameter("BoneName1"));
			GameObject *object = Commands->Create_Object("Invisible_Object",position);
			Commands->Attach_To_Object_Bone(object,obj,Get_Parameter("BoneName1"));
			Commands->Set_Model(object,Get_Parameter("ModelName1"));
			modelid1 = Commands->Get_ID(object);
			const char *subobject = Get_Parameter("Subobject1");
			if (!_stricmp(subobject,"0"))
			{
				subobject = 0;
			}
			float firstframe = Get_Float_Parameter("FirstFrame1");
			if (firstframe == -1)
			{
				firstframe = Get_Animation_Frame(obj);
			}
			Commands->Set_Animation(object,Get_Parameter("Animation1"),false,subobject,firstframe,Get_Float_Parameter("LastFrame1"),Get_Bool_Parameter("Blended1"));
		}
		else
		{
			if (!modelid2)
			{
				Vector3 position = Commands->Get_Bone_Position(obj,Get_Parameter("BoneName2"));
				GameObject *object = Commands->Create_Object("Invisible_Object",position);
				Commands->Attach_To_Object_Bone(object,obj,Get_Parameter("BoneName2"));
				Commands->Set_Model(object,Get_Parameter("ModelName2"));
				modelid2 = Commands->Get_ID(object);
				const char *subobject = Get_Parameter("Subobject2");
				if (!_stricmp(subobject,"0"))
				{
					subobject = 0;
				}
				float firstframe = Get_Float_Parameter("FirstFrame2");
				if (firstframe == -1)
				{
					firstframe = Get_Animation_Frame(obj);
				}
				Commands->Set_Animation(object,Get_Parameter("Animation2"),false,subobject,firstframe,Get_Float_Parameter("LastFrame2"),Get_Bool_Parameter("Blended2"));
			}
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (modelid2)
		{
			Commands->Destroy_Object(Commands->Find_Object(modelid2));
			modelid2 = 0;
		}
		else
		{
			if (modelid1)
			{
				Commands->Destroy_Object(Commands->Find_Object(modelid1));
				modelid1 = 0;
			}
		}
	}
}

void JFW_Visible_People_In_Vechicle::Created(GameObject *obj)
{
	modelid1 = 0;
	modelid2 = 0;
}

void JFW_Visible_People_In_Vechicle::Killed(GameObject *obj,GameObject *killer)
{
	if (modelid1)
	{
		Commands->Destroy_Object(Commands->Find_Object(modelid1));
	}
	if (modelid2)
	{
		Commands->Destroy_Object(Commands->Find_Object(modelid2));
	}
}

void JFW_Visible_People_In_Vechicle::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&modelid1,4,2);
	Auto_Save_Variable(&modelid2,4,2);
}

void JFW_Per_Preset_Visible_Person_In_Vechicle::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		//Code added by NeoSaber
		//This is to destroy the object incase it still 
		//remained for whatever reason
		if (modelid)
			Commands->Destroy_Object(Commands->Find_Object(modelid));
		//End Code added by NeoSaber

		Vector3 position = Commands->Get_Bone_Position(obj,Get_Parameter("BoneName"));
		GameObject *object = Commands->Create_Object("Invisible_Object",position);
		Commands->Attach_To_Object_Bone(object,obj,Get_Parameter("BoneName"));
		Commands->Send_Custom_Event(obj,sender,Get_Int_Parameter("Message"),Commands->Get_ID(object),0);
		modelid = Commands->Get_ID(object);
		Commands->Set_Is_Rendered(object, !stealth);
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		Commands->Destroy_Object(Commands->Find_Object(modelid));
		modelid = 0;
	}
}

void JFW_Per_Preset_Visible_Person_In_Vechicle::Created(GameObject *obj)
{
	modelid = 0;
	stealth = false;
	Commands->Start_Timer(obj, this, 1, 1);
}

void JFW_Per_Preset_Visible_Person_In_Vechicle::Killed(GameObject *obj,GameObject *killer)
{
	if (modelid)
	{
		Commands->Destroy_Object(Commands->Find_Object(modelid));
	}
}

void JFW_Per_Preset_Visible_Person_In_Vechicle::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&modelid,4,1);
	Auto_Save_Variable(&stealth,1,2);
}

void JFW_Per_Preset_Visible_Person_In_Vechicle::Timer_Expired(GameObject *obj, int number)
{
	bool b = Is_Stealth(obj);
	if (stealth != b)
	{
		stealth = b;
		if (modelid)
		{
			Commands->Set_Is_Rendered(Commands->Find_Object(modelid), !stealth);
		}
	}
	Commands->Start_Timer(obj, this, 1, 1);
}

void JFW_Per_Preset_Visible_People_In_Vechicle::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!modelid1 && Get_Vehicle_Occupant(obj,0) == sender)
		{
			Vector3 position = Commands->Get_Bone_Position(obj,Get_Parameter("BoneName1"));
			GameObject *object = Commands->Create_Object("Invisible_Object",position);
			Commands->Attach_To_Object_Bone(object,obj,Get_Parameter("BoneName1"));
			Commands->Send_Custom_Event(obj,sender,Get_Int_Parameter("Message1"),Commands->Get_ID(object),0);
			modelid1 = Commands->Get_ID(object);
			Commands->Set_Is_Rendered(object, !stealth);
			driverid1 = Commands->Get_ID(sender);
		}
		else
		{
			if (!modelid2 && Get_Vehicle_Occupant(obj,1) == sender)
			{
				Vector3 position = Commands->Get_Bone_Position(obj,Get_Parameter("BoneName2"));
				GameObject *object = Commands->Create_Object("Invisible_Object",position);
				Commands->Attach_To_Object_Bone(object,obj,Get_Parameter("BoneName2"));
				Commands->Send_Custom_Event(obj,sender,Get_Int_Parameter("Message2"),Commands->Get_ID(object),0);
				modelid2 = Commands->Get_ID(object);
				Commands->Set_Is_Rendered(object, !stealth);
				driverid2 = Commands->Get_ID(sender);
			}
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (Commands->Get_ID(sender) == driverid1)
		{
			Commands->Destroy_Object(Commands->Find_Object(modelid1));
			modelid1 = 0;
			driverid1 = 0;
		}
		else
		{
			if (Commands->Get_ID(sender) == driverid2)
			{
				Commands->Destroy_Object(Commands->Find_Object(modelid2));
				modelid2 = 0;
				driverid2 = 0;
			}
		}
	}
}

void JFW_Per_Preset_Visible_People_In_Vechicle::Created(GameObject *obj)
{
	modelid1 = 0;
	modelid2 = 0;
	driverid1 = 0;
	driverid2 = 0;
	stealth = false;
	Commands->Start_Timer(obj, this, 1, 1);
}

void JFW_Per_Preset_Visible_People_In_Vechicle::Killed(GameObject *obj,GameObject *killer)
{
	if (modelid1)
	{
		Commands->Destroy_Object(Commands->Find_Object(modelid1));
	}
	if (modelid2)
	{
		Commands->Destroy_Object(Commands->Find_Object(modelid2));
	}
}

void JFW_Per_Preset_Visible_People_In_Vechicle::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&modelid1,4,1);
	Auto_Save_Variable(&modelid2,4,1);
	Auto_Save_Variable(&driverid1,4,1);
	Auto_Save_Variable(&driverid2,4,1);
	Auto_Save_Variable(&stealth,1,2);
}

void JFW_Per_Preset_Visible_People_In_Vechicle::Timer_Expired(GameObject *obj, int number)
{
	bool b = Is_Stealth(obj);
	if (stealth != b)
	{
		stealth = b;
		if (modelid1)
		{
			Commands->Set_Is_Rendered(Commands->Find_Object(modelid1), !stealth);
		}
		if (modelid2)
		{
			Commands->Set_Is_Rendered(Commands->Find_Object(modelid2), !stealth);
		}
	}
	Commands->Start_Timer(obj, this, 1, 1);
}

void JFW_Visible_Person_Settings::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		GameObject *object = Commands->Find_Object(param);
		Commands->Set_Model(object,Get_Parameter("ModelName"));
		const char *subobject = Get_Parameter("Subobject");
		if (!_stricmp(subobject,"0"))
		{
			subobject = 0;
		}
		float firstframe = Get_Float_Parameter("FirstFrame");
		if (firstframe == -1)
		{
			firstframe = Get_Animation_Frame(obj);
		}
		Commands->Set_Animation(object,Get_Parameter("Animation"),false,subobject,firstframe,Get_Float_Parameter("LastFrame"),Get_Bool_Parameter("Blended"));
	}
}

void JFW_Visible_Person_Settings_2::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		GameObject *object = Commands->Find_Object(param);
		Commands->Set_Model(object, Get_Parameter("ModelName"));
		Commands->Set_Animation_Frame(object, Get_Parameter("Animation"), Get_Int_Parameter("Frame"));
	}
}

void JFW_Vechicle_Animation_2::Timer_Expired(GameObject *obj,int number)
{
	Vector3 position = Commands->Get_Position(obj);
	if ((!xpos) && (!ypos) && (!zpos))
	{
		xpos = position.X;
		ypos = position.Y;
		zpos = position.Z;
	}
	if ((position.X != xpos) || (position.Y != ypos) || (position.Z != zpos))
	{
		xpos = position.X;
		ypos = position.Y;
		zpos = position.Z;
		if (!enabled)
		{
			enabled = true;
			const char *subobject = Get_Parameter("UpSubobject");
			if (!_stricmp(subobject,"0"))
			{
				subobject = 0;
			}
			float firstframe = Get_Float_Parameter("UpFirstFrame");
			if (firstframe == -1)
			{
				firstframe = Get_Animation_Frame(obj);
			}
			Commands->Set_Animation(obj,Get_Parameter("UpAnimation"),false,subobject,firstframe,Get_Float_Parameter("UpLastFrame"),Get_Bool_Parameter("UpBlended"));
		}
	}
	else 
	{
		const char *subobject = Get_Parameter("DownSubobject");
		if (!_stricmp(subobject,"0"))
		{
			subobject = 0;
		}
		float firstframe = Get_Float_Parameter("DownFirstFrame");
		if (firstframe == -1)
		{
			firstframe = Get_Animation_Frame(obj);
		}
		Commands->Set_Animation(obj,Get_Parameter("DownAnimation"),false,subobject,firstframe,Get_Float_Parameter("DownLastFrame"),Get_Bool_Parameter("DownBlended"));
		enabled = false;
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Vechicle_Animation_2::Created(GameObject *obj)
{
	xpos = 0;
	ypos = 0;
	zpos = 0;
	enabled = false;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Vechicle_Animation_2::Animation_Complete(GameObject *obj,const char *animation_name)
{
	if (!_stricmp(animation_name,Get_Parameter("UpAnimation")) || !_stricmp(animation_name,Get_Parameter("Animation")))
	{
		const char *subobject = Get_Parameter("Subobject");
		if (!_stricmp(subobject,"0"))
		{
			subobject = 0;
		}
		float firstframe = Get_Float_Parameter("FirstFrame");
		if (firstframe == -1)
		{
			firstframe = Get_Animation_Frame(obj);
		}
		obj->As_PhysicalGameObj()->Clear_Animation();
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,subobject,firstframe,Get_Float_Parameter("LastFrame"),Get_Bool_Parameter("Blended"));
	}
}

void JFW_Vechicle_Animation_2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
	Auto_Save_Variable(&xpos,4,2);
	Auto_Save_Variable(&ypos,4,3);
	Auto_Save_Variable(&zpos,4,4);
}

void JFW_Vehicle_Model_Team::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if ((!CheckPlayerType(sender,team)) && (type == CUSTOM_EVENT_VEHICLE_ENTERED))
	{
		Commands->Set_Model(obj,Get_Parameter("model"));
	}
}

void JFW_Vehicle_Model_Preset::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if ((!_stricmp(Commands->Get_Preset_Name(sender),Get_Parameter("Preset"))) && (type == CUSTOM_EVENT_VEHICLE_ENTERED))
	{
		Commands->Set_Model(obj,Get_Parameter("Model"));
	}
}

void JFW_Vehicle_Model_Weapon::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if ((Has_Weapon(sender,Get_Parameter("CharWeapon"))) && (type == CUSTOM_EVENT_VEHICLE_ENTERED))
	{
		Commands->Set_Model(obj,Get_Parameter("Model"));
	}
}

void JFW_Vechicle_Animation_Trigger::Timer_Expired(GameObject *obj,int number)
{
	Vector3 position = Commands->Get_Position(obj);
	if ((!xpos) && (!ypos) && (!zpos))
	{
		xpos = position.X;
		ypos = position.Y;
		zpos = position.Z;
	}
	if ((position.X != xpos) || (position.Y != ypos) || (position.Z != zpos))
	{
		xpos = position.X;
		ypos = position.Y;
		zpos = position.Z;
		if (!enabled)
		{
			enabled = true;
			const char *subobject = Get_Parameter("Subobject");
			if (!_stricmp(subobject,"0"))
			{
				subobject = 0;
			}
			float firstframe = Get_Float_Parameter("FirstFrame");
			if (firstframe == -1)
			{
				firstframe = Get_Animation_Frame(obj);
			}
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,subobject,firstframe,Get_Float_Parameter("LastFrame"),Get_Bool_Parameter("Blended"));
		}
	}
	else 
	{
		obj->As_PhysicalGameObj()->Clear_Animation();
		enabled = false;
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Vechicle_Animation_Trigger::Animation_Complete ( GameObject *obj, const char *animation_name )
{
	if (!_stricmp(animation_name,Get_Parameter("Animation")))
	{
		const char *subobject = Get_Parameter("Subobject");
		if (!_stricmp(subobject,"0"))
		{
			subobject = 0;
		}
		float firstframe = Get_Float_Parameter("FirstFrame");
		if (firstframe == -1)
		{
			firstframe = Get_Animation_Frame(obj);
		}
		obj->As_PhysicalGameObj()->Clear_Animation();
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,subobject,firstframe,Get_Float_Parameter("LastFrame"),Get_Bool_Parameter("Blended"));
	}
}

void JFW_Vechicle_Animation_Trigger::Created(GameObject *obj)
{
	xpos = 0;
	ypos = 0;
	zpos = 0;
	enabled = false;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Vechicle_Animation_Trigger::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
	Auto_Save_Variable(&xpos,4,2);
	Auto_Save_Variable(&ypos,4,3);
	Auto_Save_Variable(&zpos,4,4);
}

void JFW_Vechicle_Animation_Trigger::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("UpTrigger"))
	{
			const char *subobject = Get_Parameter("UpSubobject");
			if (!_stricmp(subobject,"0"))
			{
				subobject = 0;
			}
			float firstframe = Get_Float_Parameter("UpFirstFrame");
			if (firstframe == -1)
			{
				firstframe = Get_Animation_Frame(obj);
			}
			Commands->Set_Animation(obj,Get_Parameter("UpAnimation"),false,subobject,firstframe,Get_Float_Parameter("UpLastFrame"),Get_Bool_Parameter("UpBlended"));
	}
	if (type == Get_Int_Parameter("DownTrigger"))
	{
			const char *subobject = Get_Parameter("DownSubobject");
			if (!_stricmp(subobject,"0"))
			{
				subobject = 0;
			}
			float firstframe = Get_Float_Parameter("DownFirstFrame");
			if (firstframe == -1)
			{
				firstframe = Get_Animation_Frame(obj);
			}
			Commands->Set_Animation(obj,Get_Parameter("DownAnimation"),false,subobject,firstframe,Get_Float_Parameter("DownLastFrame"),Get_Bool_Parameter("DownBlended"));
	}
}

void JFW_Vehicle_Block_Preset::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if ((!_stricmp(Commands->Get_Preset_Name(sender),Get_Parameter("Preset"))) && (type == CUSTOM_EVENT_VEHICLE_ENTERED))
	{
		Soldier_Transition_Vehicle(sender);
	}
}

void JFW_Vehicle_Extra::Created(GameObject *obj)
{
	GameObject *object;
	object = Commands->Create_Object_At_Bone(obj,Get_Parameter("Extra_Preset"),Get_Parameter("Bone_Name"));
	Commands->Attach_To_Object_Bone(object,obj,Get_Parameter("Bone_Name"));
	turID = Commands->Get_ID(object);
}

void JFW_Vehicle_Extra::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&turID,4,1);
}

void JFW_Vehicle_Extra::Killed(GameObject *obj,GameObject *killer)
{
	Commands->Destroy_Object(Commands->Find_Object(turID));
}

void JFW_Vehicle_Extra_Attach::Killed(GameObject *obj,GameObject *killer)
{
	Commands->Destroy_Object(Commands->Find_Object(Get_Int_Parameter("Main_Object")));
}

void JFW_Vehicle_Extra_Attach::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	GameObject *object = Commands->Find_Object(Get_Int_Parameter("Main_Object"));
	Commands->Set_Health(object,Commands->Get_Health(obj));
	Commands->Set_Shield_Strength(object,Commands->Get_Shield_Strength(obj));
}

void JFW_Vehicle_Extra_2::Created(GameObject *obj)
{
	GameObject *object;
	object = Commands->Create_Object_At_Bone(obj,Get_Parameter("Extra_Preset"),Get_Parameter("Bone_Name"));
	Commands->Attach_To_Object_Bone(object,obj,Get_Parameter("Bone_Name"));
	turID = Commands->Get_ID(object);
	char id[10];
	sprintf(id,"%d",Commands->Get_ID(obj));
	Commands->Attach_Script(object,"JFW_Vehicle_Extra_Attach",id);
	Commands->Set_Player_Type(object,Commands->Get_Player_Type(obj));
}

void JFW_Vehicle_Extra_2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		GameObject *object = Commands->Find_Object(turID);
		Commands->Set_Player_Type(object,Commands->Get_Player_Type(sender));
		Force_Occupants_Exit_Team(obj,Commands->Get_Player_Type(sender));
	}
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		GameObject *object = Commands->Find_Object(turID);
		Commands->Set_Player_Type(object,-2);
	}
}

void JFW_Vehicle_Extra_2::Killed(GameObject *obj,GameObject *killer)
{
	Commands->Destroy_Object(Commands->Find_Object(turID));
}

void JFW_Vehicle_Extra_2::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	GameObject *object = Commands->Find_Object(turID);
	Commands->Set_Health(object,Commands->Get_Health(obj));
	Commands->Set_Shield_Strength(object,Commands->Get_Shield_Strength(obj));
}

void JFW_Vehicle_Extra_2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&turID,4,1);
}

void JFW_Vehicle_Lock::Created(GameObject *obj)
{
	OwnerID = 0;
}

void JFW_Vehicle_Lock::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		if (param)
		{
			OwnerID = param;
		}
		else
		{
			OwnerID = Commands->Get_ID(Get_Vehicle_Driver(obj));
		}
		Commands->Set_Player_Type(obj,Commands->Get_Player_Type(Commands->Find_Object(OwnerID)));
		if (Get_Float_Parameter("Time"))
		{
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),2);
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if ((OwnerID) && (Commands->Find_Object(OwnerID)))
		{
			if ((Commands->Get_ID(sender) != OwnerID) && (!Is_Script_Attached(sender,Get_Parameter("Spy_Script"))))
			{
				if (Get_Vehicle_Occupant_Count(obj) == 1 && Get_Vehicle_Occupant(obj,0) == sender)
				{
					Force_Occupant_Exit(obj,0);
				}
			}
		}
	}
}

void JFW_Vehicle_Lock::Timer_Expired(GameObject *obj,int number)
{
	if (number == 2)
	{
		OwnerID = 0;
	}
}

void JFW_Vehicle_Lock::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&OwnerID,4,1);
}

void JFW_Vehicle_Effect_Animation::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		count++;
		if ((!enabled) && (count == 1))
		{
			enabled = true;
			const char *subobject = Get_Parameter("Subobject");
			if (!_stricmp(subobject,"0"))
			{
				subobject = 0;
			}
			float firstframe = Get_Float_Parameter("FirstFrame");
			if (firstframe == -1)
			{
				firstframe = Get_Animation_Frame(obj);
			}
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,subobject,firstframe,Get_Float_Parameter("LastFrame"),Get_Bool_Parameter("Blended"));
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if ((enabled) && (count == 1))
		{
			obj->As_PhysicalGameObj()->Clear_Animation();
			enabled = false;
		}
		count--;
	}
}

void JFW_Vehicle_Effect_Animation::Animation_Complete ( GameObject *obj, const char *animation_name )
{
	if (!_stricmp(animation_name,Get_Parameter("Animation")))
	{
		const char *subobject = Get_Parameter("Subobject");
		if (!_stricmp(subobject,"0"))
		{
			subobject = 0;
		}
		float firstframe = Get_Float_Parameter("FirstFrame");
		if (firstframe == -1)
		{
			firstframe = Get_Animation_Frame(obj);
		}
		obj->As_PhysicalGameObj()->Clear_Animation();
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,subobject,firstframe,Get_Float_Parameter("LastFrame"),Get_Bool_Parameter("Blended"));
	}
}

void JFW_Vehicle_Effect_Animation::Created(GameObject *obj)
{
	enabled = false;
	count = 0;
}

void JFW_Vehicle_Effect_Animation::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
	Auto_Save_Variable(&count,4,2);
}

void JFW_Vehicle_Regen_2::Created(GameObject *obj)
{
	Commands->Send_Custom_Event(obj,obj,0,0,0);
}

void JFW_Vehicle_Regen_2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (!type)
	{
		float health = Commands->Get_Health(obj);
		float maxhealth = Commands->Get_Max_Health(obj);
		if (health < maxhealth)
		{
			Commands->Apply_Damage(obj,Get_Float_Parameter("Health"),"RegenHealth",0);
		}
		Commands->Send_Custom_Event(obj,obj,0,0,Get_Float_Parameter("Time"));
	}
}


void JFW_Vehicle_Regen_3::Created(GameObject *obj)
{
	Commands->Send_Custom_Event(obj,obj,0,0,0);
}

void JFW_Vehicle_Regen_3::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (!type)
	{
		float health = Commands->Get_Health(obj);
		float maxhealth = Commands->Get_Max_Health(obj);
		if (health < maxhealth)
		{
			Commands->Apply_Damage(obj,Get_Float_Parameter("Health"),Get_Parameter("Warhead"),0);
		}
		Commands->Send_Custom_Event(obj,obj,0,0,Get_Float_Parameter("Time"));
	}
}

void JFW_Aircraft_Refill::Created(GameObject *obj)
{
	active = false;
}

void JFW_Aircraft_Refill::Entered(GameObject *obj, GameObject *enterer)
{
	if (Commands->Get_Player_Type(enterer) == Get_Int_Parameter("TeamSelection"))
	{
		if (!active)
		{
			GameObject *vobj = 0;
			if (enterer->As_VehicleGameObj())
			{
				vobj = enterer;
			}
			if (Get_Vehicle(enterer))
			{
				vobj = Get_Vehicle(enterer);
			}
			if (vobj)
			{
				if (Get_Vehicle_Mode(vobj) == VEHICLE_TYPE_FLYING)
				{
					Commands->Start_Timer(obj,this,Get_Float_Parameter("Reload_Interval"),Commands->Get_ID(vobj));
					active = true;
				}
			}
		}
	}
}

void JFW_Aircraft_Refill::Timer_Expired(GameObject *obj, int number)
{
	active = false;
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	GameObject *object = Commands->Find_Object(number);
	if (!object)
	{
		return;
	}
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(object);
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= 10.0)
	{
		if (object->As_VehicleGameObj())
		{
			if (Get_Vehicle_Mode(object) == VEHICLE_TYPE_FLYING && Get_Vehicle_Driver(object))
			{
				Commands->Give_PowerUp(object,Get_Parameter("Reload_Powerup"),false);
				Commands->Start_Timer(obj, this, Get_Float_Parameter("Reload_Interval"), number);
				active = true;
			}
		}
	}
}

void JFW_Aircraft_Refill_Animation::Created(GameObject *obj)
{
	active = false;
	GameObject *anim = Find_Nearest_Preset(Commands->Get_Position(obj),Get_Parameter("AnimPreset"));
	if (anim->As_PhysicalGameObj())
	{
		Commands->Set_Animation_Frame(anim,Get_Parameter("Animation"),0);
	}
}

void JFW_Aircraft_Refill_Animation::Entered(GameObject *obj, GameObject *enterer)
{
	if (Commands->Get_Player_Type(enterer) == Get_Int_Parameter("TeamSelection"))
	{
		if (!active)
		{
			GameObject *vobj = 0;
			if (enterer->As_VehicleGameObj())
			{
				vobj = enterer;
			}
			if (Get_Vehicle(enterer))
			{
				vobj = Get_Vehicle(enterer);
			}
			if (vobj)
			{
				if (Get_Vehicle_Mode(vobj) == VEHICLE_TYPE_FLYING)
				{
					if (Get_Vehicle_Driver(vobj) && vobj->As_VehicleGameObj()->Get_Weapon() && !vobj->As_VehicleGameObj()->Get_Weapon()->Is_Ammo_Maxed() && vobj->As_VehicleGameObj()->Get_Weapon()->Get_Definition()->MaxInventoryRounds != 0)
					{
						GameObject *anim = Find_Nearest_Preset(Commands->Get_Position(obj),Get_Parameter("AnimPreset"));
						if (anim->As_PhysicalGameObj())
						{
							Commands->Set_Animation(anim,Get_Parameter("Animation"),true,0,0,-1,false);
						}
						Commands->Start_Timer(obj,this,Get_Float_Parameter("Reload_Interval"),Commands->Get_ID(vobj));
						active = true;
					}
				}
			}
		}
	}
}

void JFW_Aircraft_Refill_Animation::Timer_Expired(GameObject *obj, int number)
{
	active = false;
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	GameObject *object = Commands->Find_Object(number);
	if (!object)
	{
		GameObject *anim = Find_Nearest_Preset(Commands->Get_Position(obj),Get_Parameter("AnimPreset"));
		if (anim && anim->As_PhysicalGameObj())
		{
			anim->As_PhysicalGameObj()->Clear_Animation();
			Commands->Set_Animation_Frame(anim,Get_Parameter("Animation"),0);
		}
		return;
	}
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(object);
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= 10.0 && object->As_VehicleGameObj() && Get_Vehicle_Mode(object) == VEHICLE_TYPE_FLYING && Get_Vehicle_Driver(object) && object->As_VehicleGameObj()->Get_Weapon() && !object->As_VehicleGameObj()->Get_Weapon()->Is_Ammo_Maxed() && object->As_VehicleGameObj()->Get_Weapon()->Get_Definition()->MaxInventoryRounds != 0)
	{
		Commands->Give_PowerUp(object,Get_Parameter("Reload_Powerup"),false);
		Commands->Start_Timer(obj, this, Get_Float_Parameter("Reload_Interval"), number);
		active = true;
	}
	else
	{
		GameObject *anim = Find_Nearest_Preset(Commands->Get_Position(obj),Get_Parameter("AnimPreset"));
		if (anim && anim->As_PhysicalGameObj())
		{
			anim->As_PhysicalGameObj()->Clear_Animation();
			Commands->Set_Animation_Frame(anim,Get_Parameter("Animation"),0);
		}
	}
}

void JFW_Vehicle_Reinforcement::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, Get_Float_Parameter("Time"),1);
}

void JFW_Vehicle_Reinforcement::Timer_Expired(GameObject *obj,int number)
{
	int ID = Get_Int_Parameter("FactoryID");
	if (ID)
	{
		GameObject *object = Commands->Find_Object(ID);
		if (object)
		{
			if (Is_Building_Dead(object))
			{
				return;
			}
		}
	}
	Vector3 v = Commands->Get_Position(obj);
	GameObject *object2 = Commands->Create_Object(Get_Parameter("Vehicle"),v);
	int waypathid;
	float speed;
	waypathid = Get_Int_Parameter("Waypathid");
	speed = Get_Float_Parameter("Speed");
	ActionParamsStruct params;
	params.MoveArrivedDistance = 100;
	params.Set_Basic(this,100,777);
	params.Set_Movement(0,speed,10);
	params.WaypathID = waypathid;
	params.WaypathSplined = true;
	params.AttackActive = false;
	params.AttackCheckBlocked = false;
	Commands->Action_Goto(object2,params);
}

void JFW_Empty_Vehicle_Timer::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	float timertime;
	int timernumber;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (!Get_Vehicle_Occupant_Count(obj))
		{
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}

void JFW_Empty_Vehicle_Timer::Timer_Expired(GameObject *obj,int number)
{
	if (!Get_Vehicle_Occupant_Count(obj))
	{
		Commands->Destroy_Object(obj);
	}
}

void JFW_Vehicle_Visible_Weapon::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.01f,1);
}

void JFW_Vehicle_Visible_Weapon::Timer_Expired(GameObject *obj,int number)
{
	VehicleGameObj *vgo = ((ScriptableGameObj *)obj)->As_VehicleGameObj();
	if (vgo && number == 1)
	{
        if(vgo->Get_Weapon() != NULL)
        {
	    	int ammo = vgo->Get_Weapon()->Get_Total_Rounds();
	    	if (ammo == -1)
	    	{
	    		ammo = vgo->Get_Weapon()->Get_Clip_Rounds();
	    	}
	    	if (ammo >= 0)
	    	{
	    		Commands->Set_Animation_Frame(obj,Get_Parameter("Animation"),ammo);
	    	}
        }
		Commands->Start_Timer(obj,this,0.01f,1);
	}
}

void JFW_Per_Preset_Visible_Multiple_People_In_Vehicle::Created(GameObject *obj)
{
	SoldierObjects.Resize(Get_Vehicle_Seat_Count(obj));
	stealth = false;
	Commands->Start_Timer(obj, this, 1, 1);
}

void JFW_Per_Preset_Visible_Multiple_People_In_Vehicle::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		int seat = Get_Occupant_Seat(obj,sender);
		StringClass bone;
		bone.Format("SEAT%d",seat);
		Vector3 position = Commands->Get_Bone_Position(obj,bone);
		GameObject *object = Commands->Create_Object("Invisible_Object",position);
		Commands->Attach_To_Object_Bone(object,obj,bone);
		Commands->Send_Custom_Event(obj,sender,Get_Int_Parameter("Message"),Commands->Get_ID(object),0);
		SoldierObjects[seat].ModelID = Commands->Get_ID(object);
		Commands->Set_Is_Rendered(object, !stealth);
		SoldierObjects[seat].SoldierID = Commands->Get_ID(sender);
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		for (int i = 0;i < SoldierObjects.Length();i++)
		{
			int senderid = Commands->Get_ID(sender);
			if (senderid == SoldierObjects[i].SoldierID)
			{
				if (SoldierObjects[i].ModelID)
				{
					GameObject *o = Commands->Find_Object(SoldierObjects[i].ModelID);
					if (o)
					{
						Commands->Destroy_Object(o);
					}
				}
			}
		}
	}
}

void JFW_Per_Preset_Visible_Multiple_People_In_Vehicle::Killed(GameObject *obj,GameObject *killer)
{
	for (int i = 0;i < SoldierObjects.Length();i++)
	{
		if (SoldierObjects[i].ModelID)
		{
			GameObject *o = Commands->Find_Object(SoldierObjects[i].ModelID);
			if (o)
			{
				Commands->Destroy_Object(o);
			}
		}
	}
}

void JFW_Per_Preset_Visible_Multiple_People_In_Vehicle::Timer_Expired(GameObject *obj, int number)
{
	bool b = Is_Stealth(obj);
	if (stealth != b)
	{
		stealth = b;
		for (int i = 0; i < SoldierObjects.Length(); i++)
		{
			if (SoldierObjects[i].ModelID)
			{
				GameObject *o = Commands->Find_Object(SoldierObjects[i].ModelID);
				if (o)
				{
					Commands->Set_Is_Rendered(o, !stealth);
				}
			}
		}
	}
	Commands->Start_Timer(obj, this, 1, 1);
}

void JFW_Per_Preset_Visible_Multiple_People_In_Vehicle_2::Created(GameObject *obj)
{
	SoldierObjects.Resize(Get_Vehicle_Seat_Count(obj));
	stealth = false;
	Commands->Start_Timer(obj, this, 1, 1);
}

void JFW_Per_Preset_Visible_Multiple_People_In_Vehicle_2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		int seat = Get_Occupant_Seat(obj,sender);
		if ((seat >= Get_Int_Parameter("FirstSeat")) && (seat <= Get_Int_Parameter("LastSeat")))
		{
			StringClass bone;
			bone.Format("SEAT%d",seat);
			Vector3 position = Commands->Get_Bone_Position(obj,bone);
			GameObject *object = Commands->Create_Object("Invisible_Object",position);
			Commands->Attach_To_Object_Bone(object,obj,bone);
			Commands->Send_Custom_Event(obj,sender,Get_Int_Parameter("Message"),Commands->Get_ID(object),0);
			SoldierObjects[seat].ModelID = Commands->Get_ID(object);
			Commands->Set_Is_Rendered(object, !stealth);
			SoldierObjects[seat].SoldierID = Commands->Get_ID(sender);
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		for (int i = Get_Int_Parameter("FirstSeat");i <= Get_Int_Parameter("LastSeat");i++)
		{
			int senderid = Commands->Get_ID(sender);
			if (senderid == SoldierObjects[i].SoldierID)
			{
				if (SoldierObjects[i].ModelID)
				{
					GameObject *o = Commands->Find_Object(SoldierObjects[i].ModelID);
					if (o)
					{
						Commands->Destroy_Object(o);
					}
				}
			}
		}
	}
}

void JFW_Per_Preset_Visible_Multiple_People_In_Vehicle_2::Killed(GameObject *obj,GameObject *killer)
{
	for (int i = Get_Int_Parameter("FirstSeat");i <= Get_Int_Parameter("LastSeat");i++)
	{
		if (SoldierObjects[i].ModelID)
		{
			GameObject *o = Commands->Find_Object(SoldierObjects[i].ModelID);
			if (o)
			{
				Commands->Destroy_Object(o);
			}
		}
	}
}

void JFW_Per_Preset_Visible_Multiple_People_In_Vehicle_2::Timer_Expired(GameObject *obj, int number)
{
	bool b = Is_Stealth(obj);
	if (stealth != b)
	{
		stealth = b;
		for (int i = Get_Int_Parameter("FirstSeat");i <= Get_Int_Parameter("LastSeat");i++)
		{
			if (SoldierObjects[i].ModelID)
			{
				GameObject *o = Commands->Find_Object(SoldierObjects[i].ModelID);
				if (o)
				{
					Commands->Set_Is_Rendered(o, !stealth);
				}
			}
		}
	}
	Commands->Start_Timer(obj, this, 1, 1);
}

ScriptRegistrant<JFW_Vehicle_Model_Team> JFW_Vehicle_Model_Team_Registrant("JFW_Vehicle_Model_Team", "Model:string,Player_Type:int");
ScriptRegistrant<JFW_Vehicle_Model_Preset> JFW_Vehicle_Model_Preset_Registrant("JFW_Vehicle_Model_Preset","Model:string,Preset:string");
ScriptRegistrant<JFW_Vehicle_Model_Weapon> JFW_Vehicle_Model_Weapon_Registrant("JFW_Vehicle_Model_Weapon","Model:string,CharWeapon:string");
ScriptRegistrant<JFW_Vechicle_Animation_Trigger> JFW_Vechicle_Animation_Trigger_Registrant("JFW_Vechicle_Animation_Trigger","Animation:string,Subobject:string,FirstFrame:float,LastFrame:float,Blended:int,Time:float,TimerNum:int,UpAnimation:string,UpSubobject:string,UpFirstFrame:float,UpLastFrame:float,UpBlended:int,DownAnimation:string,DownSubobject:string,DownFirstFrame:float,DownLastFrame:float,DownBlended:int,UpTrigger:int,DownTrigger:int");
ScriptRegistrant<JFW_Vechicle_Animation_2> JFW_Vechicle_Animation_2_Registrant("JFW_Vechicle_Animation_2","Animation:string,Subobject:string,FirstFrame:float,LastFrame:float,Blended:int,Time:float,TimerNum:int,UpAnimation:string,UpSubobject:string,UpFirstFrame:float,UpLastFrame:float,UpBlended:int,DownAnimation:string,DownSubobject:string,DownFirstFrame:float,DownLastFrame:float,DownBlended:int");
ScriptRegistrant<JFW_Turret_Spawn_2> JFW_Turret_Spawn_2_Registrant("JFW_Turret_Spawn_2","Turret_Preset:string,Bone_Name=Tur_Mount:string,Explosion_Preset:string,Explosion_Bone:string");
ScriptRegistrant<JFW_Turret_Spawn_3> JFW_Turret_Spawn_3_Registrant("JFW_Turret_Spawn_3","Turret_Preset:string,Bone_Name=Tur_Mount:string,Explosion_Preset:string,Explosion_Bone:string,Driver_Entered_Custom:int,Driver_Exited_Custom:int");
ScriptRegistrant<JFW_Drive_To_Player> JFW_Drive_To_Player_Registrant("JFW_Drive_To_Player","Speed:float,Arrive_Distance:float");
ScriptRegistrant<JFW_Vechicle_Animation> JFW_Vechicle_Animation_Registrant("JFW_Vechicle_Animation","Animation:string,Subobject:string,FirstFrame:float,LastFrame:float,Blended:int,Time:float,TimerNum:int");
ScriptRegistrant<JFW_Visible_Person_In_Vechicle> JFW_Visible_Person_In_Vechicle_Registrant("JFW_Visible_Person_In_Vechicle","BoneName:string,ModelName:string,Animation:string,SubObject:string,FirstFrame:float,LastFrame:float,Blended:int");
ScriptRegistrant<JFW_Visible_People_In_Vechicle> JFW_Visible_People_In_Vechicle_Registrant("JFW_Visible_People_In_Vechicle","BoneName1:string,ModelName1:string,Animation1:string,SubObject1:string,FirstFrame1:float,LastFrame1:float,Blended1:int,BoneName2:string,ModelName2:string,Animation2:string,SubObject2:string,FirstFrame2:float,LastFrame2:float,Blended2:int");
ScriptRegistrant<JFW_Per_Preset_Visible_Person_In_Vechicle> JFW_Per_Preset_Visible_Person_In_Vechicle_Registrant("JFW_Per_Preset_Visible_Person_In_Vechicle","BoneName:string,Message:int");
ScriptRegistrant<JFW_Per_Preset_Visible_People_In_Vechicle> JFW_Per_Preset_Visible_People_In_Vechicle_Registrant("JFW_Per_Preset_Visible_People_In_Vechicle","BoneName1:string,BoneName2:string,Message1:int,Message2:int");
ScriptRegistrant<JFW_Visible_Person_Settings> JFW_Visible_Person_Settings_Registrant("JFW_Visible_Person_Settings","Message:int,ModelName:string,Animation:string,SubObject:string,FirstFrame:float,LastFrame:float,Blended:int");
ScriptRegistrant<JFW_Visible_Person_Settings_2> JFW_Visible_Person_Settings_Registrant_2("JFW_Visible_Person_Settings_2", "Message:int,ModelName:string,Animation:string,Frame:float");
ScriptRegistrant<JFW_Aircraft_Fuel> JFW_Aircraft_Fuel_Registrant("JFW_Aircraft_Fuel", "Time:float,TimerNum:int,Explosion:string,Refuel_Message:int");
ScriptRegistrant<JFW_Vehicle_Block_Preset> JFW_Vehicle_Block_Preset_Registrant("JFW_Vehicle_Block_Preset","Preset:string");
ScriptRegistrant<JFW_Vehicle_Extra> JFW_Vehicle_Extra_Registrant("JFW_Vehicle_Extra","Extra_Preset:string,Bone_Name:string");
ScriptRegistrant<JFW_Vehicle_Extra_2> JFW_Vehicle_Extra_2_Registrant("JFW_Vehicle_Extra_2","Extra_Preset:string,Bone_Name:string");
ScriptRegistrant<JFW_Vehicle_Extra_Attach> JFW_Vehicle_Extra_Attach_Registrant("JFW_Vehicle_Extra_Attach","Main_Object:int");
ScriptRegistrant<JFW_Vehicle_Lock> JFW_Vehicle_Lock_Registrant("JFW_Vehicle_Lock","Message:int,Spy_Script:string,Time:float");
ScriptRegistrant<JFW_Vehicle_Effect_Animation> JFW_Vehicle_Effect_Animation_Registrant("JFW_Vehicle_Effect_Animation","Animation:string,Subobject:string,FirstFrame:float,LastFrame:float,Blended:int");
ScriptRegistrant<JFW_Vehicle_Regen_2> JFW_Vechicle_Regen_2_Registrant("JFW_Vechicle_Regen_2","Health:float,Time:float");
ScriptRegistrant<JFW_Vehicle_Regen_3> JFW_Vechicle_Regen_3_Registrant("JFW_Vechicle_Regen_3","Health:float,Time:float,Warhead:string");
ScriptRegistrant<JFW_Aircraft_Refill> JFW_Aircraft_Refill_Registrant("JFW_Aircraft_Refill","TeamSelection:int,Reload_Interval:float,Reload_Powerup:string");
ScriptRegistrant<JFW_Aircraft_Refill_Animation> JFW_Aircraft_Refill_Animation_Registrant("JFW_Aircraft_Refill_Animation","TeamSelection:int,Reload_Interval:float,Reload_Powerup:string,AnimPreset:string,Animation:string");
ScriptRegistrant<JFW_Vehicle_Reinforcement> JFW_Vehicle_Reinforcement_Registrant("JFW_Vehicle_Reinforcement","Vehicle:string,Time:float,FactoryID:int,Waypathid:int,Speed:float");
ScriptRegistrant<JFW_Empty_Vehicle_Timer> JFW_Empty_Vehicle_Timer_Registrant("JFW_Empty_Vehicle_Timer","Time:float,TimerNum:int");
ScriptRegistrant<JFW_Vehicle_Visible_Weapon> JFW_Vehicle_Visible_Weapon_Registrant("JFW_Vehicle_Visible_Weapon","Animation:string");
ScriptRegistrant<JFW_Per_Preset_Visible_Multiple_People_In_Vehicle> JFW_Per_Preset_Visible_Multiple_People_In_Vehicle_Registrant("JFW_Per_Preset_Visible_Multiple_People_In_Vehicle","Message:int");
ScriptRegistrant<JFW_Per_Preset_Visible_Multiple_People_In_Vehicle_2> JFW_Per_Preset_Visible_Multiple_People_In_Vehicle_2_Registrant("JFW_Per_Preset_Visible_Multiple_People_In_Vehicle_2","Message:int,FirstSeat:int,LastSeat:int");
