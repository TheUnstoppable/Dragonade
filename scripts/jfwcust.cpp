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
#include "jfwcust.h"
#include "PhysicalGameObj.h"

void JFW_Custom_Toggle_Radar::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Message");
	if (msg == type)
	{
		Commands->Enable_Radar(param != 0);
	}
}

void JFW_Reflect_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	Commands->Send_Custom_Event(obj,sender,type,param,0);
}

void JFW_Reflect_Custom_Delay::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	Commands->Send_Custom_Event(obj,sender,type,param,Get_Float_Parameter("Delay"));
}

void JFW_Custom_Destroy_Object::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	int id;
	GameObject *object;
	msg = Get_Int_Parameter("Message");
	id = Get_Int_Parameter("ID");
	if (msg == type)
	{
		object = Commands->Find_Object(id);
		Commands->Destroy_Object(object);
	}
}

void JFW_Custom_Objectives_Dlg::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Message");
	const char *file = Get_Parameter("File");
	if (msg == type)
	{
		Do_Objectives_Dlg(sender,file);
	}
}

void JFW_Death_Send_Custom::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	int type;
	int ID;
	int param;
	ID = Get_Int_Parameter("ID");
	type = Get_Int_Parameter("Message");
	param = Get_Int_Parameter("Param");
	object = Commands->Find_Object(ID);
	Commands->Send_Custom_Event(obj,object,type,param,0);
	Destroy_Script();
}

void JFW_Custom_Destroy_Building::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	int id;
	GameObject *object;
	msg = Get_Int_Parameter("Message");
	id = Get_Int_Parameter("ID");
	if (msg == type)
	{
		object = Commands->Find_Object(id);
		Commands->Apply_Damage(object,10000,"Explosive",0);
	}
}

void JFW_Timer_Custom::Created(GameObject *obj)
{
	float timertime;
	int timernumber;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_Timer_Custom::Timer_Expired(GameObject *obj,int number)
{
	int timernumber;
	int repeat;
	float timertime;
	GameObject *object;
	int ID,msg,param;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	repeat = Get_Int_Parameter("Repeat");
	ID = Get_Int_Parameter("ID");
	msg = Get_Int_Parameter("Message");
	param = Get_Int_Parameter("Param");
	object = Commands->Find_Object(ID);
	if (number == timernumber)
	{
		Commands->Send_Custom_Event(obj,object,msg,param,0);
		if (repeat == 1)
		{	
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}

void JFW_2D_Sound_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Message");
	if (type == msg)
	{
		Commands->Create_2D_Sound(Get_Parameter("Preset"));
	}
}

void JFW_3D_Sound_Custom_2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Message");
	if (type == msg)
	{
		Commands->Create_Sound(Get_Parameter("Preset"),Commands->Get_Position(sender),obj);
	}
}

void JFW_3D_Sound_Custom_Bone::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Message");
	if (type == msg)
	{
		Commands->Create_3D_Sound_At_Bone(Get_Parameter("Preset"),obj,Get_Parameter("Bone"));
	}
}

void JFW_Random_Custom::Created(GameObject *obj)
{
	float timertime,timemin,timemax;
	int timernumber;
	timemin = Get_Float_Parameter("Time_Min");
	timemax = Get_Float_Parameter("Time_Max");
	timernumber = Get_Int_Parameter("TimerNum");
	timertime = Commands->Get_Random(timemin,timemax);
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_Random_Custom::Timer_Expired(GameObject *obj,int number)
{
	float timertime,timemin,timemax;
	int timernumber;
	int repeat;
	int ID;
	int msg;
	timemin = Get_Float_Parameter("Time_Min");
	timemax = Get_Float_Parameter("Time_Max");
	timernumber = Get_Int_Parameter("TimerNum");
	repeat = Get_Int_Parameter("Repeat");
	if (number == timernumber)
	{
		msg = Get_Int_Parameter("Message");
		ID = Get_Int_Parameter("ID");
		Commands->Send_Custom_Event(obj,Commands->Find_Object(ID),msg,0,0);
		if (repeat == 1)
		{	
			timertime = Commands->Get_Random(timemin,timemax);
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}

void JFW_Custom_Damage_Object::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg = Get_Int_Parameter("Message");
	if (type == msg)
	{
		int ID = Get_Int_Parameter("ID");
		float damage = Get_Float_Parameter("Damage");
		GameObject *object = Commands->Find_Object(ID);
		float health = Commands->Get_Health(object);
		if (damage >= health)
		{
			health = 0;
		}
		else
		{
			health -= damage;
		}
		Commands->Set_Health(object,health);
		if (!health)
		{
			Commands->Destroy_Object(object);
		}
	}
}

void JFW_Blow_Up_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	const char *c;
	int msg;
	Vector3 v;
	msg = Get_Int_Parameter("Message");
	if (type == msg)
	{
		c = Get_Parameter("Explosion");
		v = Commands->Get_Position(sender);
		Commands->Create_Explosion(c,v,0);
	}
}

void JFW_Blow_Up_Obj_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	const char *c;
	int msg;
	Vector3 v;
	msg = Get_Int_Parameter("Message");
	if (type == msg)
	{
		c = Get_Parameter("Explosion");
		v = Commands->Get_Position(obj);
		Commands->Create_Explosion(c,v,0);
	}
}

void JFW_Delay_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	float delay = Get_Float_Parameter("Delay");
	int msg = Get_Int_Parameter("Message");
	int ID = Get_Int_Parameter("ID");
	if (msg == type)
	{
		Commands->Send_Custom_Event(sender,Commands->Find_Object(ID),type,param,delay);
	}
}

void JFW_Startup_Custom::Created(GameObject *obj)
{
	int msg = Get_Int_Parameter("Message");
	int ID = Get_Int_Parameter("ID");
	Commands->Send_Custom_Event(obj,Commands->Find_Object(ID),msg,0,0);
	Destroy_Script();
}

void JFW_Teleport_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg = Get_Int_Parameter("Message");
	if (type == msg) 
	{
		Vector3 spawn_position;
		spawn_position = Get_Vector3_Parameter("Location");
		int x = Get_Int_Parameter("Object_ID");
		if (x)
		{
			GameObject *gotoObject = Commands->Find_Object(x);
			Vector3 gotoLocation = Commands->Get_Position(gotoObject);
			Commands->Set_Position(Commands->Find_Object(param),gotoLocation);
		}
		else
			Commands->Set_Position(Commands->Find_Object(param),spawn_position);
	}
}

void JFW_Teleport_Custom_Random::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg = Get_Int_Parameter("Message");
	Vector3 spawn_position;
	int x = 0;
	if (type == msg) 
	{
		int rand = Commands->Get_Random_Int(1,5);
		if (rand == 1)
		{
			spawn_position = Get_Vector3_Parameter("Location1");
			x = Get_Int_Parameter("Object_ID1");
		}
		if (rand == 2)
		{
			spawn_position = Get_Vector3_Parameter("Location2");
			x = Get_Int_Parameter("Object_ID2");
		}
		if (rand == 3)
		{
			spawn_position = Get_Vector3_Parameter("Location3");
			x = Get_Int_Parameter("Object_ID3");
		}
		if (rand == 4)
		{
			spawn_position = Get_Vector3_Parameter("Location4");
			x = Get_Int_Parameter("Object_ID4");
		}
		if (rand == 5)
		{
			spawn_position = Get_Vector3_Parameter("Location5");
			x = Get_Int_Parameter("Object_ID5");
		}
		if (x)
		{
			GameObject *gotoObject = Commands->Find_Object(x);
			Vector3 gotoLocation = Commands->Get_Position(gotoObject);
			Commands->Set_Position(Commands->Find_Object(param),gotoLocation);
		}
		else
			Commands->Set_Position(Commands->Find_Object(param),spawn_position);
	}
}

void JFW_Teleport_Custom_Timer::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg = Get_Int_Parameter("Message");
	if ((type == msg) && (enabled))
	{
		Vector3 spawn_position;
		spawn_position = Get_Vector3_Parameter("Location");
		int x = Get_Int_Parameter("Object_ID");
		if (x)
		{
			GameObject *gotoObject = Commands->Find_Object(x);
			Vector3 gotoLocation = Commands->Get_Position(gotoObject);
			Commands->Set_Position(Commands->Find_Object(param),gotoLocation);
		}
		else
		{
			Commands->Set_Position(Commands->Find_Object(param),spawn_position);
		}
		enabled = false;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
	}
}

void JFW_Teleport_Custom_Timer::Timer_Expired(GameObject *obj,int number)
{
	enabled = true;
}

void JFW_Teleport_Custom_Timer::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Teleport_Custom_Timer::Created(GameObject *obj)
{
	enabled = true;
}

void JFW_Blow_Up_Location_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	const char *c;
	int msg;
	Vector3 v;
	msg = Get_Int_Parameter("Message");
	if (type == msg)
	{
		c = Get_Parameter("Explosion");
		v = Get_Vector3_Parameter("Location");
		Commands->Create_Explosion(c,v,0);
	}
}

void JFW_3D_Sound_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	Vector3 v;
	const char *c;
	msg = Get_Int_Parameter("Message");
	if (type == msg)
	{
		v = Get_Vector3_Parameter("Position");
		c = Get_Parameter("Preset");
		Commands->Create_Sound(c,v,obj);
	}
}

void JFW_Created_Send_Custom::Created(GameObject *obj)
{
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message"),0,0);
	Destroy_Script();
}

void JFW_Damaged_Send_Custom::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message"),0,0);
}

void JFW_Enemy_Seen_Send_Custom::Created(GameObject *obj)
{
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_Enemy_Seen_Send_Custom::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message"),Commands->Get_ID(enemy),0);
}

void JFW_Create_Object_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		GameObject *object = Commands->Create_Object(Get_Parameter("Object"),Get_Vector3_Parameter("Location"));
		Commands->Set_Facing(object,Get_Float_Parameter("Facing"));
	}
}

void JFW_Enable_Spawner_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Commands->Enable_Spawner(Get_Int_Parameter("ID"),Get_Bool_Parameter("Enable"));
	}
}

void JFW_Set_Animation_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
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
		Commands->Set_Animation(obj,Get_Parameter("Animation"),Get_Bool_Parameter("Loop"),subobject,firstframe,Get_Float_Parameter("LastFrame"),Get_Bool_Parameter("Blended"));
	}
}

void JFW_Set_Model_Animation_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Commands->Set_Model(obj,Get_Parameter("Model"));
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
		Commands->Set_Animation(obj,Get_Parameter("Animation"),Get_Bool_Parameter("Loop"),subobject,firstframe,Get_Float_Parameter("LastFrame"),Get_Bool_Parameter("Blended"));
	}
}

void JFW_Set_Model_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Commands->Set_Model(obj,Get_Parameter("Model"));
	}
}

void JFW_Enable_Disable_Animation_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Enable"))
	{
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
			Commands->Set_Animation(obj,Get_Parameter("Animation"),true,subobject,firstframe,-1,Get_Bool_Parameter("Blended"));
		}
	}
	if (type == Get_Int_Parameter("Disable"))
	{
		if (enabled)
		{
			obj->As_PhysicalGameObj()->Clear_Animation();
			enabled = false;
		}
	}
}

void JFW_Enable_Disable_Animation_On_Custom::Created(GameObject *obj)
{
	enabled = false;
}

void JFW_Enable_Disable_Animation_On_Custom::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Custom_Toggle_Visible::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Commands->Set_Is_Rendered(obj,Get_Bool_Parameter("Visible"));
	}
}

void JFW_Customs_Send_Custom::Created(GameObject *obj)
{
	if (!Get_Int_Parameter("Message1"))
	{
		custom1 = true;
	}
	else
	{
		custom1 = false;
	}
	if (!Get_Int_Parameter("Message2"))
	{
		custom2 = true;
	}
	else
	{
		custom2 = false;
	}
	if (!Get_Int_Parameter("Message3"))
	{
		custom3 = true;
	}
	else
	{
		custom3 = false;
	}
	if (!Get_Int_Parameter("Message4"))
	{
		custom4 = true;
	}
	else
	{
		custom4 = false;
	}
	if (!Get_Int_Parameter("Message5"))
	{
		custom5 = true;
	}
	else
	{
		custom5 = false;
	}
	if (!Get_Int_Parameter("Message6"))
	{
		custom6 = true;
	}
	else
	{
		custom6 = false;
	}
	if (!Get_Int_Parameter("Message7"))
	{
		custom7 = true;
	}
	else
	{
		custom7 = false;
	}
	if (!Get_Int_Parameter("Message8"))
	{
		custom8 = true;
	}
	else
	{
		custom8 = false;
	}
	trigger = false;
}

void JFW_Customs_Send_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Message1");
	if (type == msg)
	{
		custom1 = true;
	}
	msg = Get_Int_Parameter("Message2");
	if (type == msg)
	{
		custom2 = true;
	}
	msg = Get_Int_Parameter("Message3");
	if (type == msg)
	{
		custom3 = true;
	}
	msg = Get_Int_Parameter("Message4");
	if (type == msg)
	{
		custom4 = true;
	}
	msg = Get_Int_Parameter("Message5");
	if (type == msg)
	{
		custom5 = true;
	}
	msg = Get_Int_Parameter("Message6");
	if (type == msg)
	{
		custom6 = true;
	}
	msg = Get_Int_Parameter("Message7");
	if (type == msg)
	{
		custom7 = true;
	}
	msg = Get_Int_Parameter("Message8");
	if (type == msg)
	{
		custom8 = true;
	}
	if (!trigger)
	{
		if ((custom1) && (custom2) && (custom3) && (custom4) && (custom5) && (custom6) && (custom7) && (custom8))
		{
			trigger = true;
			Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message"),Get_Int_Parameter("Param"),0);
		}
	}
}

void JFW_Customs_Send_Custom::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&custom1,1,1);
	Auto_Save_Variable(&custom2,1,2);
	Auto_Save_Variable(&custom3,1,3);
	Auto_Save_Variable(&custom4,1,4);
	Auto_Save_Variable(&custom5,1,5);
	Auto_Save_Variable(&custom6,1,6);
	Auto_Save_Variable(&custom7,1,7);
	Auto_Save_Variable(&custom8,1,8);
	Auto_Save_Variable(&trigger,9,1);
}

void JFW_Custom_Multiple_Send_Custom::Created(GameObject *obj)
{
	times_recieved = 0;
	enabled = true;
}

void JFW_Custom_Multiple_Send_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (enabled)
	{
		if (type == Get_Int_Parameter("RecieveMessage"))
		{
			times_recieved++;
		}
		if (times_recieved == Get_Int_Parameter("RecieveCount"))
		{
			enabled = false;
			Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("SendID")),Get_Int_Parameter("SendMessage"),Get_Int_Parameter("SendParam"),0);
		}
	}
}

void JFW_Custom_Multiple_Send_Custom::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&times_recieved,1,4);
	Auto_Save_Variable(&enabled,2,1);
}

void JFW_Custom_Multiple_Send_Custom_2::Created(GameObject *obj)
{
	times_recieved = 0;
}

void JFW_Custom_Multiple_Send_Custom_2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("RecieveMessage"))
	{
		times_recieved++;
	}
	if (times_recieved == Get_Int_Parameter("RecieveCount"))
	{
		times_recieved = 0;
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("SendID")),Get_Int_Parameter("SendMessage"),Get_Int_Parameter("SendParam"),0);
	}
}

void JFW_Custom_Multiple_Send_Custom_2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&times_recieved,1,4);
}

void JFW_Custom_Send_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("RecieveMessage"))
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("SendID")),Get_Int_Parameter("SendMessage"),Get_Int_Parameter("SendParam"),0);
	}
}

void JFW_Custom_Send_Random_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("RecieveMessage"))
	{
		if (Commands->Get_Random_Int(0,100) < Get_Int_Parameter("Percentage"))
		{
			Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("SendID")),Get_Int_Parameter("SendMessage"),Get_Int_Parameter("SendParam"),0);
		}
	}
}

void JFW_Create_Destroy_Object_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("CreateMessage"))
	{
		if (!ObjectID)
		{
			GameObject *object = Commands->Create_Object(Get_Parameter("Preset"),Get_Vector3_Parameter("Location"));
			Commands->Set_Facing(object,Get_Float_Parameter("Facing"));
			ObjectID = Commands->Get_ID(object);
		}
	}
	if (type == Get_Int_Parameter("DestroyMessage"))
	{
		if (ObjectID)
		{
			Commands->Destroy_Object(Commands->Find_Object(ObjectID));
			ObjectID = 0;
		}
	}
}

void JFW_Create_Destroy_Object_On_Custom::Created(GameObject *obj)
{
	ObjectID = 0;
}

void JFW_Create_Destroy_Object_On_Custom::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ObjectID,1,4);
}

void JFW_Create_Destroy_Object_On_Custom_Animate::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("CreateMessage"))
	{
		if (!ObjectID)
		{
			GameObject *object = Commands->Create_Object(Get_Parameter("Preset"),Get_Vector3_Parameter("Location"));
			Commands->Set_Facing(object,Get_Float_Parameter("Facing"));
			ObjectID = Commands->Get_ID(object);
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
			Commands->Set_Animation(object,Get_Parameter("Animation"),Get_Bool_Parameter("Loop"),subobject,firstframe,Get_Float_Parameter("LastFrame"),Get_Bool_Parameter("Blended"));
		}
	}
	if (type == Get_Int_Parameter("DestroyMessage"))
	{
		if (ObjectID)
		{
			Commands->Destroy_Object(Commands->Find_Object(ObjectID));
			ObjectID = 0;
		}
	}
}

void JFW_Create_Destroy_Object_On_Custom_Animate::Created(GameObject *obj)
{
	ObjectID = 0;
}

void JFW_Create_Destroy_Object_On_Custom_Animate::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ObjectID,1,4);
}

void JFW_Create_Destroy_Object_On_Custom_Animate_2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int timernumber;
	float timertime;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	if (type == Get_Int_Parameter("CreateMessage"))
	{
		if (!ObjectID)
		{
			GameObject *object = Commands->Create_Object(Get_Parameter("Preset"),Get_Vector3_Parameter("Location"));
			Commands->Set_Facing(object,Get_Float_Parameter("Facing"));
			ObjectID = Commands->Get_ID(object);
			const char *subobject = Get_Parameter("CreateSubobject");
			if (!_stricmp(subobject,"0"))
			{
				subobject = 0;
			}
			float firstframe = Get_Float_Parameter("CreateFirstFrame");
			if (firstframe == -1)
			{
				firstframe = Get_Animation_Frame(obj);
			}
			Commands->Set_Animation(object,Get_Parameter("CreateAnimation"),Get_Bool_Parameter("CreateLoop"),subobject,firstframe,Get_Float_Parameter("CreateLastFrame"),Get_Bool_Parameter("CreateBlended"));
		}
	}
	if (type == Get_Int_Parameter("DestroyMessage"))
	{
		if (ObjectID)
		{
			const char *subobject = Get_Parameter("DestroySubobject");
			if (!_stricmp(subobject,"0"))
			{
				subobject = 0;
			}
			float firstframe = Get_Float_Parameter("DestroyFirstFrame");
			if (firstframe == -1)
			{
				firstframe = Get_Animation_Frame(obj);
			}
			Commands->Set_Animation(Commands->Find_Object(ObjectID),Get_Parameter("DestroyAnimation"),false,subobject,firstframe,Get_Float_Parameter("DestroyLastFrame"),Get_Bool_Parameter("DestroyBlended"));
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}

void JFW_Create_Destroy_Object_On_Custom_Animate_2::Created(GameObject *obj)
{
	ObjectID = 0;
}

void JFW_Create_Destroy_Object_On_Custom_Animate_2::Timer_Expired(GameObject *obj,int number)
{
	if (number == Get_Int_Parameter("TimerNum"))
	{
		Commands->Destroy_Object(Commands->Find_Object(ObjectID));
		ObjectID = 0;
	}
}

void JFW_Create_Destroy_Object_On_Custom_Animate_2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ObjectID,1,4);
}

void JFW_Damaged_Send_Custom_Amount::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (amount < Get_Float_Parameter("Damage"))
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message1"),0,0);
	}
	else
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message2"),0,0);
	}
}

void JFW_Invulnerable_On_Custom::Created(GameObject *obj)
{
	enabled = false;
}

void JFW_Invulnerable_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		if (param == 1)
		{
			enabled = true;
			health = Commands->Get_Health(obj);
			shield_strength = Commands->Get_Shield_Strength(obj);
			float maxhealth = Commands->Get_Max_Health(obj);
			float maxshieldstrength = Commands->Get_Max_Shield_Strength(obj);
			Commands->Set_Health(obj,maxhealth);
			Commands->Set_Shield_Strength(obj,maxshieldstrength);
		}
		else
		{
			enabled = false;
			Commands->Set_Health(obj,health);
			Commands->Set_Shield_Strength(obj,shield_strength);
		}
	}
}

void JFW_Invulnerable_On_Custom_2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		if (param == 1)
		{
			Commands->Set_Shield_Type(obj,Get_Parameter("InvulnerableArmour"));
		}
		else
		{
			Commands->Set_Shield_Type(obj,Get_Parameter("NormalArmour"));
		}
	}
}

void JFW_Invulnerable_On_Custom::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (enabled)
	{
		Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
		Commands->Set_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj));
	}
}

void JFW_Invulnerable_On_Custom::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
	Auto_Save_Variable(&health,2,4);
	Auto_Save_Variable(&shield_strength,3,4);
}

void JFW_Custom_Send_Custom_Parameter::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int basemessage = Get_Int_Parameter("BaseMessage");
	int limit = Get_Int_Parameter("Limit");
	int ID = Get_Int_Parameter("ID");
	if ((type >= basemessage) && (type <= limit))
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(ID),basemessage,type-basemessage,0);
	}
}

void JFW_Create_Destroy_Object_On_Custom_Bone::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("CreateMessage"))
	{
		if (!ObjectID)
		{
			GameObject *object = Commands->Create_Object(Get_Parameter("Preset"),Get_Vector3_Parameter("Location"));
			Commands->Set_Facing(object,Get_Float_Parameter("Facing"));
			Commands->Attach_To_Object_Bone(object,obj,Get_Parameter("BoneName"));
			ObjectID = Commands->Get_ID(object);
		}
	}
	if (type == Get_Int_Parameter("DestroyMessage"))
	{
		if (ObjectID)
		{
			Commands->Destroy_Object(Commands->Find_Object(ObjectID));
			ObjectID = 0;
		}
	}
}

void JFW_Create_Destroy_Object_On_Custom_Bone::Created(GameObject *obj)
{
	ObjectID = 0;
}

void JFW_Create_Destroy_Object_On_Custom_Bone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ObjectID,1,4);
}

void JFW_Custom_Send_Ordered_Customs::Created(GameObject *obj)
{
	CurrentCustom = 0;
}

void JFW_Custom_Send_Ordered_Customs::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("WaitCustom"))
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter(CurrentCustom)),Get_Int_Parameter(CurrentCustom+8),0,0);
		CurrentCustom++;
		if (CurrentCustom > 7)
		{
			CurrentCustom = 0;
		}
	}
}

void JFW_Custom_Send_Ordered_Customs::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&CurrentCustom,1,4);
}

void JFW_Custom_Send_Customs::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("WaitCustom"))
	{
		for (int CurrentCustom=0;CurrentCustom<=7;CurrentCustom++)
		{
			Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter(CurrentCustom)),Get_Int_Parameter(CurrentCustom+8),0,0);
		}
	}
}

void JFW_Custom_Send_Custom_Multiple_IDs::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("WaitCustom"))
	{
		for (int CurrentCustom=0;CurrentCustom<=7;CurrentCustom++)
		{
			Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter(CurrentCustom)),Get_Int_Parameter("SendCustom"),0,0);
		}
	}
}

void JFW_Create_Destroy_Object_On_Custom_Death::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("CreateMessage"))
	{
		if (!ObjectID)
		{
			GameObject *object = Commands->Create_Object(Get_Parameter("Preset"),Get_Vector3_Parameter("Location"));
			Commands->Set_Facing(object,Get_Float_Parameter("Facing"));
			ObjectID = Commands->Get_ID(object);
		}
	}
	if (type == Get_Int_Parameter("DestroyMessage"))
	{
		if (ObjectID)
		{
			Commands->Destroy_Object(Commands->Find_Object(ObjectID));
			ObjectID = 0;
		}
	}
}

void JFW_Create_Destroy_Object_On_Custom_Death::Created(GameObject *obj)
{
	ObjectID = 0;
}

void JFW_Create_Destroy_Object_On_Custom_Death::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ObjectID,1,4);
}

void JFW_Create_Destroy_Object_On_Custom_Death_Bone::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("CreateMessage"))
	{
		if (!ObjectID)
		{
			GameObject *object = Commands->Create_Object(Get_Parameter("Preset"),Get_Vector3_Parameter("Location"));
			Commands->Set_Facing(object,Get_Float_Parameter("Facing"));
			Commands->Attach_To_Object_Bone(object,obj,Get_Parameter("BoneName"));
			ObjectID = Commands->Get_ID(object);
		}
	}
	if (type == Get_Int_Parameter("DestroyMessage"))
	{
		if (ObjectID)
		{
			Commands->Destroy_Object(Commands->Find_Object(ObjectID));
			ObjectID = 0;
		}
	}
}

void JFW_Create_Destroy_Object_On_Custom_Death_Bone::Created(GameObject *obj)
{
	ObjectID = 0;
}

void JFW_Create_Destroy_Object_On_Custom_Death_Bone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ObjectID,1,4);
}

void JFW_Create_Destroy_Object_On_Custom_Death_Bone::Killed(GameObject *obj,GameObject *killer)
{
	if (ObjectID)
	{
		Commands->Destroy_Object(Commands->Find_Object(ObjectID));
		ObjectID = 0;
	}
}

void JFW_Create_Destroy_Object_On_Custom_Death::Killed(GameObject *obj,GameObject *killer)
{
	if (ObjectID)
	{
		Commands->Destroy_Object(Commands->Find_Object(ObjectID));
		ObjectID = 0;
	}
}

void JFW_Custom_Destroy_All_Buildings::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	Kill_All_Buildings_By_Team(Get_Int_Parameter("Player_Type"));
}

void JFW_Timer_Custom_On_Off::Created(GameObject *obj)
{
	enabled = false;
}

void JFW_Timer_Custom_On_Off::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("EnableMessage"))
	{
		enabled = true;
		float timertime;
		int timernumber;
		timertime = Get_Float_Parameter("Time");
		timernumber = Get_Int_Parameter("TimerNum");
		Commands->Start_Timer(obj,this,timertime,timernumber);
	}
	if (type == Get_Int_Parameter("DisableMessage"))
	{
		enabled = false;
	}
}

void JFW_Timer_Custom_On_Off::Timer_Expired(GameObject *obj,int number)
{
	int timernumber;
	int repeat;
	float timertime;
	GameObject *object;
	int ID,msg,param;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	repeat = Get_Int_Parameter("Repeat");
	ID = Get_Int_Parameter("ID");
	msg = Get_Int_Parameter("Message");
	param = Get_Int_Parameter("Param");
	object = Commands->Find_Object(ID);
	if (number == timernumber)
	{
		Commands->Send_Custom_Event(obj,object,msg,param,0);
		if ((repeat == 1) && (enabled))
		{
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}

void JFW_Timer_Custom_On_Off::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Create_Vehicle_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	Create_Vehicle(Get_Parameter("Preset_Name"),Get_Float_Parameter("Delay"),0,Get_Int_Parameter("Player_Type"));
}

void JFW_Set_Model_On_Custom_Sender::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Commands->Set_Model(sender,Get_Parameter("Model"));
	}
}

void JFW_Death_Send_Custom_ID::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	int type;
	int ID;
	int param;
	ID = Get_Int_Parameter("ID");
	type = Get_Int_Parameter("Message");
	param = Commands->Get_ID(obj);
	object = Commands->Find_Object(ID);
	Commands->Send_Custom_Event(obj,object,type,param,0);
	Destroy_Script();
}

void JFW_Death_Send_Custom_Driver_ID::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object;
	int type;
	int ID;
	int param;
	ID = Get_Int_Parameter("ID");
	type = Get_Int_Parameter("Message");
	param = DriverID;
	object = Commands->Find_Object(ID);
	Commands->Send_Custom_Event(obj,object,type,param,0);
	Destroy_Script();
}

void JFW_Death_Send_Custom_Driver_ID::Created(GameObject *obj)
{
	DriverID = 0;
}

void JFW_Death_Send_Custom_Driver_ID::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!DriverID)
		{
			DriverID = Commands->Get_ID(sender);
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (DriverID == Commands->Get_ID(sender))
		{
			DriverID = 0;
		}
	}
}

void JFW_Death_Send_Custom_Driver_ID::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&DriverID,1,4);
}

void JFW_Repair_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		float amount = Commands->Get_Health(obj);
		if (amount>0)
		{
			Commands->Set_Health(obj,(amount+Get_Float_Parameter("Health")));
		}
		amount = Commands->Get_Health(obj);
		if (amount>0)
		{
			Commands->Set_Shield_Strength(obj,(amount+Get_Float_Parameter("Health")));
		}
	}
}

void JFW_Fog_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (Get_Int_Parameter("Message") == type)
	{
		if (Get_Float_Parameter("Delay") > 0)
		{
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		}
		else
		{
			bool fogenable = Get_Bool_Parameter("Fog_Enable");
			Commands->Set_Fog_Enable(fogenable);
			if (fogenable)
			{
				Commands->Set_Fog_Range(Get_Float_Parameter("Fog_Start_Distance"),Get_Float_Parameter("Fog_End_Distance"),1.0f);
			}
		}
	}
}

void JFW_Fog_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		bool fogenable = Get_Bool_Parameter("Fog_Enable");
		Commands->Set_Fog_Enable(fogenable);
		if (fogenable)
		{
			Commands->Set_Fog_Range(Get_Float_Parameter("Fog_Start_Distance"),Get_Float_Parameter("Fog_End_Distance"),1.0f);
		}
	}
}

void JFW_War_Blitz_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (Get_Int_Parameter("Message") == type)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
			Commands->Set_War_Blitz(Get_Float_Parameter("War_Blitz_Intensity"),Get_Float_Parameter("Start_Distance"),Get_Float_Parameter("End_Distance"),Get_Float_Parameter("War_Blitz_Heading"),Get_Float_Parameter("War_Blitz_Distribution"),1.0f);
	}
}

void JFW_War_Blitz_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
		Commands->Set_War_Blitz(Get_Float_Parameter("War_Blitz_Intensity"),Get_Float_Parameter("Start_Distance"),Get_Float_Parameter("End_Distance"),Get_Float_Parameter("War_Blitz_Heading"),Get_Float_Parameter("War_Blitz_Distribution"),1.0f);
}

void JFW_Wind_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (Get_Int_Parameter("Message") == type)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
			Commands->Set_Wind(Get_Float_Parameter("Wind_Heading"),Get_Float_Parameter("Wind_Speed"),Get_Float_Parameter("Wind_Variability"),1.0f);
	}
}

void JFW_Wind_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
		Commands->Set_Wind(Get_Float_Parameter("Wind_Heading"),Get_Float_Parameter("Wind_Speed"),Get_Float_Parameter("Wind_Variability"),1.0f);
}

void JFW_Lightning_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (Get_Int_Parameter("Message") == type)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
			Commands->Set_Lightning(Get_Float_Parameter("Lightning_Intensity"),Get_Float_Parameter("Start_Distance"),Get_Float_Parameter("End_Distance"),Get_Float_Parameter("Lightning_Heading"),Get_Float_Parameter("Lightning_Distribution"),1.0f);
	}
}

void JFW_Lightning_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
		Commands->Set_Lightning(Get_Float_Parameter("Lightning_Intensity"),Get_Float_Parameter("Start_Distance"),Get_Float_Parameter("End_Distance"),Get_Float_Parameter("Lightning_Heading"),Get_Float_Parameter("Lightning_Distribution"),1.0f);
}

void JFW_Clouds_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (Get_Int_Parameter("Message") == type)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
			Commands->Set_Clouds(Get_Float_Parameter("Cloud_Cover"),Get_Float_Parameter("Cloud_Gloominess"),1.0f);
	}
}

void JFW_Clouds_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
		Commands->Set_Clouds(Get_Float_Parameter("Cloud_Cover"),Get_Float_Parameter("Cloud_Gloominess"),1.0f);
}

void JFW_Precipitation_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (Get_Int_Parameter("Message") == type)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
		{
			if (Get_Int_Parameter("Reset_Precipitation") > 0)
			{
				Commands->Set_Snow(0.0f,0.0f,true);
				Commands->Set_Ash(0.0f,0.0f,true);
				Commands->Set_Rain(0.0f,0.0f,true);
			}
			if (!_stricmp(Get_Parameter("Type"),"Snow")) Commands->Set_Snow(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
			else if (!_stricmp(Get_Parameter("Type"),"Ash")) Commands->Set_Ash(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
			else if (!_stricmp(Get_Parameter("Type"),"Rain")) Commands->Set_Rain(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
			else
			{
				Commands->Set_Snow(0.0f,0.0f,true);
				Commands->Set_Ash(0.0f,0.0f,true);
				Commands->Set_Rain(0.0f,0.0f,true);
			}
		}
	}
}

void JFW_Precipitation_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (Get_Int_Parameter("Reset_Precipitation") > 0)
		{
			Commands->Set_Snow(0.0f,0.0f,true);
			Commands->Set_Ash(0.0f,0.0f,true);
			Commands->Set_Rain(0.0f,0.0f,true);
		}
		if (!_stricmp(Get_Parameter("Type"),"Snow")) Commands->Set_Snow(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
		else if (!_stricmp(Get_Parameter("Type"),"Ash")) Commands->Set_Ash(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
		else if (!_stricmp(Get_Parameter("Type"),"Rain")) Commands->Set_Rain(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
		else
		{
			Commands->Set_Snow(0.0f,0.0f,true);
			Commands->Set_Ash(0.0f,0.0f,true);
			Commands->Set_Rain(0.0f,0.0f,true);
		}
	}
}

void JFW_Enable_Stealth_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Commands->Enable_Stealth(obj,Get_Bool_Parameter("Enable"));
	}
}

void JFW_Console_Message_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Console_Output(Get_Parameter("Output"));
	}
}

void JFW_Console_Input_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Console_Input(Get_Parameter("Input"));
	}
}

void JFW_Invulnerable_On_Custom_3::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		if (param == 1)
		{
			Set_Skin(obj,Get_Parameter("InvulnerableArmour"));
		}
		else
		{
			Set_Skin(obj,Get_Parameter("NormalArmour"));
		}
	}
}

void JFW_Set_Skin_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Set_Skin(obj,Get_Parameter("Armour"));
	}
}

void JFW_Set_Armor_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Commands->Set_Shield_Type(obj,Get_Parameter("Armour"));
	}
}

void JFW_Change_Team_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		int team = Get_Object_Type(obj);
		if (team == 1)
		{
			Set_Object_Type(obj,0);
		}
		else
		{
			Set_Object_Type(obj,1);
		}
	}
}

void JFW_Enable_Team_Radar_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Enable_Team_Radar(Get_Int_Parameter("Player_Type"),Get_Bool_Parameter("Enable"));
	}
}

void JFW_2D_Sound_Team_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Message");
	if (type == msg)
	{
		Create_2D_Sound_Team(Get_Parameter("Sound"),Get_Int_Parameter("Team"));
	}
}

void JFW_3D_Sound_Team_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	Vector3 v;
	const char *c;
	msg = Get_Int_Parameter("Message");
	if (type == msg)
	{
		v = Get_Vector3_Parameter("Position");
		c = Get_Parameter("Sound");
		Create_Sound_Team(c,v,obj,Get_Int_Parameter("Team"));
	}
}

void JFW_Send_Custom_Distance_Objects_Timer::Created(GameObject *obj) 
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Send_Custom_Distance_Objects_Timer::Timer_Expired(GameObject *obj,int number)
{
	if (number == Get_Int_Parameter("TimerNum"))
	{
		Send_Custom_All_Objects_Area(Get_Int_Parameter("Message"),Commands->Get_Position(obj),Get_Float_Parameter("Distance"),obj,Get_Int_Parameter("Player_Type"));
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
	}
}

void JFW_Send_Custom_Distance_Objects_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("ListenMessage"))
	{
		Send_Custom_All_Objects_Area(Get_Int_Parameter("Message"),Commands->Get_Position(obj),Get_Float_Parameter("Distance"),obj,Get_Int_Parameter("Player_Type"));
	}
}

void JFW_Send_Custom_All_Objects_Timer::Created(GameObject *obj) 
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Send_Custom_All_Objects_Timer::Timer_Expired(GameObject *obj,int number)
{
	if (number == Get_Int_Parameter("TimerNum"))
	{
		Send_Custom_All_Objects(Get_Int_Parameter("Message"),obj,Get_Int_Parameter("Player_Type"));
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
	}
}

void JFW_Send_Custom_All_Objects_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("ListenMessage"))
	{
		Send_Custom_All_Objects(Get_Int_Parameter("Message"),obj,Get_Int_Parameter("Player_Type"));
	}
}

void JFW_Destroy_Create_Object_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		GameObject *object = Commands->Find_Object(Get_Int_Parameter("ID"));
		Vector3 v = Commands->Get_Position(object);
		Commands->Destroy_Object(object);
		Commands->Create_Object(Get_Parameter("Preset"),v);
	}
}

void JFW_Send_Message_Preset::Created(GameObject *obj)
{
	Send_Custom_Event_To_Object(obj,Get_Parameter("Preset"),Get_Int_Parameter("CreateMessage"),0,0);
}

void JFW_Send_Message_Preset::Destroyed(GameObject *obj)
{
	Send_Custom_Event_To_Object(obj,Get_Parameter("Preset"),Get_Int_Parameter("DestroyMessage"),0,0);
}

void JFW_Kill_All_Buildings_Customs::Created(GameObject *obj)
{
	count = 0;
}

void JFW_Kill_All_Buildings_Customs::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		count++;
	}
	if (count >= Get_Int_Parameter("Count"))
	{
		Kill_All_Buildings_By_Team(Get_Int_Parameter("Player_Type"));
	}
}

void JFW_Send_Self_Custom_On_Key::Created(GameObject *obj)
{
	InstallHook(Get_Parameter("Key"),obj);
}

void JFW_Send_Self_Custom_On_Key::KeyHook()
{
	Commands->Send_Custom_Event(Owner(),Owner(),Get_Int_Parameter("Message"),0,0);
	if (Get_Int_Parameter("Once"))
	{
		Destroy_Script();
	}
}

void JFW_Send_Message_On_Key::Created(GameObject *obj)
{
	InstallHook(Get_Parameter("Key"), obj);
}

void JFW_Send_Message_On_Key::KeyHook()
{
	Send_Message_Player(Owner(),Get_Int_Parameter("Red"), Get_Int_Parameter("Green"), Get_Int_Parameter("Blue"), Get_Parameter("Message"));
}

void JFW_Startup_Custom_Self::Created(GameObject *obj)
{
	int msg = Get_Int_Parameter("Message");
  int param = (Get_Parameter_Count()>1)?Get_Int_Parameter("Param"):0; // Added 06-10-13 by danpaul88
  int delay = (Get_Parameter_Count()>1)?Get_Int_Parameter("Delay"):0; // Added 06-10-13 by danpaul88
	Commands->Send_Custom_Event(obj,obj,msg,param,(float)delay);
	Destroy_Script();
}

void JFW_Send_Custom_On_Damage::Created(GameObject *obj)
{
	above = true;
}

void JFW_Send_Custom_On_Damage::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	float hp = Commands->Get_Health(obj) + Commands->Get_Shield_Strength(obj);
	float val = Get_Float_Parameter("Health");
	if (hp < val && above)
	{
		above = false;
		int msg = Get_Int_Parameter("Message1");
		int ID = Get_Int_Parameter("ID1");
		Commands->Send_Custom_Event(obj,Commands->Find_Object(ID),msg,0,0);
	}
	else if (hp > val && !above)
	{
		above = true;
		int msg = Get_Int_Parameter("Message2");
		int ID = Get_Int_Parameter("ID2");
		Commands->Send_Custom_Event(obj,Commands->Find_Object(ID),msg,0,0);
	}
}

ScriptRegistrant<JFW_Send_Custom_Distance_Objects_Timer> JFW_Send_Custom_Distance_Objects_Timer_Registrant("JFW_Send_Custom_Distance_Objects_Timer","Distance:float,Message:int,Time:float,TimerNum:int,Player_Type:int");
ScriptRegistrant<JFW_Send_Custom_Distance_Objects_Custom> JFW_Send_Custom_Distance_Objects_Custom_Registrant("JFW_Send_Custom_Distance_Objects_Custom","Distance:float,Message:int,Player_Type:int,ListenMessage:int");
ScriptRegistrant<JFW_Send_Custom_All_Objects_Timer> JFW_Send_Custom_All_Objects_Timer_Registrant("JFW_Send_Custom_All_Objects_Timer","Message:int,Time:float,TimerNum:int,Player_Type:int");
ScriptRegistrant<JFW_Send_Custom_All_Objects_Custom> JFW_Send_Custom_All_Objects_Custom_Registrant("JFW_Send_Custom_All_Objects_Custom","Message:int,Player_Type:int,ListenMessage:int");
ScriptRegistrant<JFW_Send_Message_Preset> JFW_Send_Message_Preset_Registrant("JFW_Send_Message_Preset","Preset:string,CreateMessage:int,DestroyMessage:int");
ScriptRegistrant<JFW_Destroy_Create_Object_On_Custom> JFW_Destroy_Create_Object_On_Custom_Registrant("JFW_Destroy_Create_Object_On_Custom","Message:int,ID:int,Preset:string");
ScriptRegistrant<JFW_Fog_Custom> JFW_Fog_Custom_Registrant("JFW_Fog_Custom","Fog_Enable:int,Fog_Start_Distance=0.000:float,Fog_End_Distance=0.000:float,Delay=0.000:float,Message:int");
ScriptRegistrant<JFW_War_Blitz_Custom> JFW_War_Blitz_Custom_Registrant("JFW_War_Blitz_Custom","War_Blitz_Intensity=0.000:float,Start_Distance=0.000:float,End_Distance=1.000:float,War_Blitz_Heading=0.000:float,War_Blitz_Distribution=1.000:float,Delay=0.000:float,Message:int");
ScriptRegistrant<JFW_Lightning_Custom> JFW_Lightning_Custom_Registrant("JFW_Lightning_Custom","Lightning_Intensity=0.000:float,Start_Distance=0.000:float,End_Distance=1.000:float,Lightning_Heading=0.000:float,Lightning_Distribution=1.000:float,Delay=0.000:float,Message:int");
ScriptRegistrant<JFW_Wind_Custom> JFW_Wind_Custom_Registrant("JFW_Wind_Custom","Wind_Heading=0.000:float,Wind_Speed=0.000:float,Wind_Variability=0.000:float,Delay=0.000:float,Message:int");
ScriptRegistrant<JFW_Precipitation_Custom> JFW_Precipitation_Custom_Registrant("JFW_Precipitation_Custom","Type=Rain:string,Density=0.000:float,Delay=0.000:float,Reset_Precipitation=0:int,Message:int");
ScriptRegistrant<JFW_Clouds_Custom> JFW_Clouds_Custom_Registrant("JFW_Clouds_Custom","Cloud_Cover=0.000:float,Cloud_Gloominess=0.000:float,Delay=0.000:float,Message:int");
ScriptRegistrant<JFW_Enable_Stealth_On_Custom> JFW_Enable_Stealth_On_Custom_Registrant("JFW_Enable_Stealth_On_Custom","Message:int,Enable:int");
ScriptRegistrant<JFW_Console_Message_On_Custom> JFW_Console_Message_On_Custom_Registrant("JFW_Console_Message_On_Custom","Message:int,Output:string");
ScriptRegistrant<JFW_Console_Input_On_Custom> JFW_Console_Input_On_Custom_Registrant("JFW_Console_Input_On_Custom","Message:int,Input:string");
ScriptRegistrant<JFW_Invulnerable_On_Custom_3> JFW_Invulnerable_On_Custom_3_Registrant("JFW_Invulnerable_On_Custom_3","Message:int,InvulnerableArmour:string,NormalArmour:string");
ScriptRegistrant<JFW_Change_Team_On_Custom> JFW_Change_Team_On_Custom_Registrant("JFW_Change_Team_On_Custom","Message:int");
ScriptRegistrant<JFW_Enable_Team_Radar_On_Custom> JFW_Enable_Team_Radar_On_Custom_Registrant("JFW_Enable_Team_Radar_On_Custom","Player_Type:int,Message:int,Enable:int");
ScriptRegistrant<JFW_2D_Sound_Team_Custom> JFW_2D_Sound_Team_Custom_Registrant("JFW_2D_Sound_Team_Custom","Message:int,Sound:string,Team:int");
ScriptRegistrant<JFW_3D_Sound_Team_Custom> JFW_3D_Sound_Team_Custom_Registrant("JFW_3D_Sound_Team_Custom","Message:int,Sound:string,Position:vector3,Team:int");
ScriptRegistrant<JFW_Custom_Send_Ordered_Customs> JFW_Custom_Send_Ordered_Customs_Registrant("JFW_Custom_Send_Ordered_Customs","ID1:int,ID2:int,ID3:int,ID4:int,ID5:int,ID6:int,ID7:int,ID8:int,Custom1:int,Custom2:int,Custom3:int,Custom4:int,Custom5:int,Custom6:int,Custom7:int,Custom8:int,WaitCustom:int");
ScriptRegistrant<JFW_Custom_Send_Customs> JFW_Custom_Send_Customs_Registrant("JFW_Custom_Send_Customs","ID1:int,ID2:int,ID3:int,ID4:int,ID5:int,ID6:int,ID7:int,ID8:int,Custom1:int,Custom2:int,Custom3:int,Custom4:int,Custom5:int,Custom6:int,Custom7:int,Custom8:int,WaitCustom:int");
ScriptRegistrant<JFW_Custom_Send_Custom_Multiple_IDs> JFW_Custom_Send_Custom_Multiple_IDs_Registrant("JFW_Custom_Send_Custom_Multiple_IDs","ID1:int,ID2:int,ID3:int,ID4:int,ID5:int,ID6:int,ID7:int,ID8:int,WaitCustom:int,SendCustom:int");
ScriptRegistrant<JFW_Create_Destroy_Object_On_Custom_Death> JFW_Create_Destroy_Object_On_Custom_Death_Registrant("JFW_Create_Destroy_Object_On_Custom_Death","Location:vector3,Facing:float,Preset:string,CreateMessage:int,DestroyMessage:int");
ScriptRegistrant<JFW_Create_Destroy_Object_On_Custom_Death_Bone> JFW_Create_Destroy_Object_On_Custom_Death_Bone_Registrant("JFW_Create_Destroy_Object_On_Custom_Death_Bone","Location:vector3,Facing:float,Preset:string,CreateMessage:int,DestroyMessage:int,BoneName:string");
ScriptRegistrant<JFW_Custom_Destroy_All_Buildings> JFW_Custom_Destroy_All_Buildings_Registrant("JFW_Custom_Destroy_All_Buildings","Player_Type:int");
ScriptRegistrant<JFW_Timer_Custom_On_Off> JFW_Timer_Custom_On_Off_Registrant("JFW_Timer_Custom_On_Off","Time:float,TimerNum:int,Repeat:int,ID:int,Message:int,Param:int,EnableMessage:int,DisableMessage:int");
ScriptRegistrant<JFW_Create_Vehicle_On_Custom> JFW_Create_Vehicle_On_Custom_Registrant("JFW_Create_Vehicle_On_Custom","Preset_Name:string,Delay:float,Player_Type:int");
ScriptRegistrant<JFW_Set_Model_On_Custom_Sender> JFW_Set_Model_On_Custom_Sender_Registrant("JFW_Set_Model_On_Custom_Sender","Message:int,Model:string");
ScriptRegistrant<JFW_Death_Send_Custom_ID> JFW_Death_Send_Custom_ID_Registrant("JFW_Death_Send_Custom_ID","ID:int,Message:int");
ScriptRegistrant<JFW_Death_Send_Custom_Driver_ID> JFW_Death_Send_Custom_Driver_ID_Registrant("JFW_Death_Send_Custom_Driver_ID","ID:int,Message:int");
ScriptRegistrant<JFW_Repair_On_Custom> JFW_Repair_On_Custom_Registrant("JFW_Repair_On_Custom","Message:int,Health:float");
ScriptRegistrant<JFW_Teleport_Custom> JFW_Teleport_Custom_Registrant("JFW_Teleport_Custom","Location:vector3,Object_ID=0:int,Message:int");
ScriptRegistrant<JFW_Teleport_Custom_Random> JFW_Teleport_Custom_Random_Registrant("JFW_Teleport_Custom_Random","Location1:vector3,Object_ID1=0:int,Location2:vector3,Object_ID2=0:int,Location3:vector3,Object_ID3=0:int,Location4:vector3,Object_ID4=0:int,Location5:vector3,Object_ID5=0:int,Message:int");
ScriptRegistrant<JFW_Teleport_Custom_Timer> JFW_Teleport_Custom_Timer_Registrant("JFW_Teleport_Custom_Timer","Location:vector3,Object_ID=0:int,Message:int,Time:float,TimerNum:int");
ScriptRegistrant<JFW_Blow_Up_Location_On_Custom> JFW_Blow_Location_Up_On_Custom_Registrant("JFW_Blow_Location_Up_On_Custom","Explosion:string,Message:int,Location:vector3");
ScriptRegistrant<JFW_3D_Sound_Custom> JFW_3D_Sound_Custom_Registrant("JFW_3D_Sound_Custom","Message:int,Preset:string,Position:vector3");
ScriptRegistrant<JFW_Created_Send_Custom> JFW_Created_Send_Custom_Registrant("JFW_Created_Send_Custom","ID:int,Message:int");
ScriptRegistrant<JFW_Damaged_Send_Custom> JFW_Damaged_Send_Custom_Registrant("JFW_Damaged_Send_Custom","ID:int,Message:int");
ScriptRegistrant<JFW_Enemy_Seen_Send_Custom> JFW_Enemy_Seen_Send_Custom_Registrant("JFW_Enemy_Seen_Send_Custom","ID:int,Message:int");
ScriptRegistrant<JFW_Create_Object_Custom> JFW_Create_Object_Custom_Registrant("JFW_Create_Object_Custom","Object:string,Location:vector3,Facing:float,Message:int");
ScriptRegistrant<JFW_Enable_Spawner_On_Custom> JFW_Enable_Spawner_On_Custom_Registrant("JFW_Enable_Spawner_On_Custom","ID:int,Enable:int,Message:int");
ScriptRegistrant<JFW_Set_Animation_On_Custom> JFW_Set_Animation_On_Custom_Registrant("JFW_Set_Animation_On_Custom","Animation:string,Loop:int,Subobject:string,FirstFrame:float,LastFrame:float,Blended:int,Message:int");
ScriptRegistrant<JFW_Set_Model_Animation_On_Custom> JFW_Set_Model_Animation_On_Custom_Registrant("JFW_Set_Model_Animation_On_Custom","Animation:string,Loop:int,Subobject:string,FirstFrame:float,LastFrame:float,Blended:int,Model:string,Message:int");
ScriptRegistrant<JFW_Set_Model_On_Custom> JFW_Set_Model_On_Custom_Registrant("JFW_Set_Model_On_Custom","Model:string,Message:int");
ScriptRegistrant<JFW_Enable_Disable_Animation_On_Custom> JFW_Enable_Disable_Animation_On_Custom_Registrant("JFW_Enable_Disable_Animation_On_Custom","Animation:string,Subobject:string,FirstFrame:float,LastFrame:float,Blended:int,Enable:int,Disable:int");
ScriptRegistrant<JFW_Custom_Toggle_Visible> JFW_Custom_Toggle_Visible_Registrant("JFW_Custom_Toggle_Visible","Message:int,Visible:int");
ScriptRegistrant<JFW_Customs_Send_Custom> JFW_Customs_Send_Custom_Registrant("JFW_Customs_Send_Custom","Message1:int,Message2:int,Message3:int,Message4:int,Message5:int,Message6:int,Message7:int,Message8:int,Message:int,Param:int,ID:int");
ScriptRegistrant<JFW_Custom_Send_Custom> JFW_Custom_Send_Custom_Registrant("JFW_Custom_Send_Custom","RecieveMessage:int,SendMessage:int,SendParam:int,SendID:int");
ScriptRegistrant<JFW_Custom_Multiple_Send_Custom> JFW_Custom_Multiple_Send_Custom_Registrant("JFW_Custom_Multiple_Send_Custom","RecieveMessage:int,RecieveCount:int,SendMessage:int,SendParam:int,SendID:int");
ScriptRegistrant<JFW_Custom_Multiple_Send_Custom_2> JFW_Custom_Multiple_Send_Custom_2_Registrant("JFW_Custom_Multiple_Send_Custom_2","RecieveMessage:int,RecieveCount:int,SendMessage:int,SendParam:int,SendID:int");
ScriptRegistrant<JFW_Create_Destroy_Object_On_Custom> JFW_Create_Destroy_Object_On_Custom_Registrant("JFW_Create_Destroy_Object_On_Custom","Location:vector3,Facing:float,Preset:string,CreateMessage:int,DestroyMessage:int");
ScriptRegistrant<JFW_Create_Destroy_Object_On_Custom_Animate> JFW_Create_Destroy_Object_On_Custom_Animate_Registrant("JFW_Create_Destroy_Object_On_Custom_Animate","Location:vector3,Facing:float,Preset:string,CreateMessage:int,DestroyMessage:int,Animation:string,Loop:int,Subobject:string,FirstFrame:float,LastFrame:float,Blended:int");
ScriptRegistrant<JFW_Create_Destroy_Object_On_Custom_Animate_2> JFW_Create_Destroy_Object_On_Custom_Animate_2_Registrant("JFW_Create_Destroy_Object_On_Custom_Animate_2","Location:vector3,Facing:float,Preset:string,CreateMessage:int,DestroyMessage:int,CreateAnimation:string,CreateLoop:int,CreateSubobject:string,CreateFirstFrame:float,CreateLastFrame:float,CreateBlended:int,DestroyAnimation:string,DestroySubobject:string,DestroyFirstFrame:float,DestroyLastFrame:float,DestroyBlended:int,Time:float,TimerNum:int");
ScriptRegistrant<JFW_Damaged_Send_Custom_Amount> JFW_Damaged_Send_Custom_Amount_Registrant("JFW_Damaged_Send_Custom_Amount","ID:int,Message1:int,Message2:int,Damage:float");
ScriptRegistrant<JFW_Invulnerable_On_Custom> JFW_Invulnerable_On_Custom_Registrant("JFW_Invulnerable_On_Custom","Message:int");
ScriptRegistrant<JFW_Invulnerable_On_Custom_2> JFW_Invulnerable_On_Custom_2_Registrant("JFW_Invulnerable_On_Custom_2","Message:int,InvulnerableArmour:string,NormalArmour:string");
ScriptRegistrant<JFW_Custom_Send_Custom_Parameter> JFW_Custom_Send_Custom_Parameter_Registrant("JFW_Custom_Send_Custom_Parameter","ID:int,BaseMessage:int,Limit:int");
ScriptRegistrant<JFW_Create_Destroy_Object_On_Custom_Bone> JFW_Create_Destroy_Object_On_Custom_Bone_Registrant("JFW_Create_Destroy_Object_On_Custom_Bone","Location:vector3,Facing:float,Preset:string,CreateMessage:int,DestroyMessage:int,BoneName:string");
ScriptRegistrant<JFW_Timer_Custom> JFW_Timer_Custom_Registrant("JFW_Timer_Custom","Time:float,TimerNum:int,Repeat:int,ID:int,Message:int,Param:int");
ScriptRegistrant<JFW_2D_Sound_Custom> JFW_2D_Sound_Custom_Registrant("JFW_2D_Sound_Custom","Message:int,Preset:string");
ScriptRegistrant<JFW_3D_Sound_Custom_2> JFW_3D_Sound_Custom_2_Registrant("JFW_3D_Sound_Custom_2","Message:int,Preset:string");
ScriptRegistrant<JFW_3D_Sound_Custom_Bone> JFW_3D_Sound_Custom_Bone_Registrant("JFW_3D_Sound_Custom_Bone","Message:int,Preset:string,Bone:string");
ScriptRegistrant<JFW_Random_Custom> JFW_Random_Custom_Registrant("JFW_Random_Custom","Message:int,ID:int,TimerNum:int,Time_Min:float,Time_Max:float,Repeat:int");
ScriptRegistrant<JFW_Custom_Damage_Object> JFW_Custom_Damage_Object_Registrant("JFW_Custom_Damage_Object","ID:int,Message:int,Damage:float");
ScriptRegistrant<JFW_Blow_Up_On_Custom> JFW_Blow_Up_On_Custom_Registrant("JFW_Blow_Up_On_Custom","Explosion:string,Message:int");
ScriptRegistrant<JFW_Blow_Up_Obj_On_Custom> JFW_Blow_Obj_Up_On_Custom_Registrant("JFW_Blow_Obj_Up_On_Custom","Explosion:string,Message:int");
ScriptRegistrant<JFW_Delay_Custom> JFW_Delay_Custom_Registrant("JFW_Delay_Custom","Message:int,Delay:float,ID:int");
ScriptRegistrant<JFW_Startup_Custom> JFW_Startup_Custom_Registrant("JFW_Startup_Custom","Message:int,ID:int");
ScriptRegistrant<JFW_Custom_Toggle_Radar> JFW_Custom_Toggle_Radar_Registrant("JFW_Custom_Toggle_Radar","Message:int");
ScriptRegistrant<JFW_Custom_Destroy_Object> JFW_Custom_Destroy_Object_Registrant("JFW_Custom_Destroy_Object","Message:int,ID:int");
ScriptRegistrant<JFW_Custom_Objectives_Dlg> JFW_Custom_Objectives_Dlg_Registrant("JFW_Custom_Objectives_Dlg","Message:int,File:string");
ScriptRegistrant<JFW_Reflect_Custom> JFW_Reflect_Custom_Registrant("JFW_Reflect_Custom","");
ScriptRegistrant<JFW_Reflect_Custom_Delay> JFW_Reflect_Custom_Delay_Registrant("JFW_Reflect_Custom_Delay","Delay:float");
ScriptRegistrant<JFW_Death_Send_Custom> JFW_Death_Send_Custom_Registrant("JFW_Death_Send_Custom","ID:int,Message:int,Param:int");
ScriptRegistrant<JFW_Custom_Destroy_Building> JFW_Custom_Destroy_Building_Registrant("JFW_Custom_Destroy_Building","Message:int,ID:int");
ScriptRegistrant<JFW_Shader_Custom_Zone_Entry> JFW_Shader_Custom_Zone_Entry_Registrant("JFW_Shader_Custom_Zone_Entry","Player_Type:int,Number:int,Number2:float");
ScriptRegistrant<JFW_Shader_Custom_Zone_Exit> JFW_Shader_Custom_Zone_Exit_Registrant("JFW_Shader_Custom_Zone_Exit","Player_Type:int,Number:int,Number2:float");
ScriptRegistrant<JFW_Shader_Custom_Vehicle_Entry> JFW_Shader_Custom_Vehicle_Entry_Registrant("JFW_Shader_Custom_Vehicle_Entry","Number:int,Number2:float");
ScriptRegistrant<JFW_Shader_Custom_Vehicle_Exit> JFW_Shader_Custom_Vehicle_Exit_Registrant("JFW_Shader_Custom_Vehicle_Exit","Number:int,Number2:float");
ScriptRegistrant<JFW_Shader_Custom_Create> JFW_Shader_Custom_Create_Registrant("JFW_Shader_Custom_Create","Number:int,Number2:float");
ScriptRegistrant<JFW_Shader_Custom_Poke> JFW_Shader_Custom_Poke_Registrant("JFW_Shader_Custom_Poke","Number:int,Number2:float");
ScriptRegistrant<JFW_Kill_All_Buildings_Customs> JFW_Kill_All_Buildings_Customs_Registrant("JFW_Kill_All_Buildings_Customs","Message:int,Count:int,Player_Type:int");
ScriptRegistrant<JFW_Send_Self_Custom_On_Key> JFW_Send_Self_Custom_On_Key_Registrant("JFW_Send_Self_Custom_On_Key","Key=Key:string,Message=0:int,Once=1:int");
ScriptRegistrant<JFW_Send_Message_On_Key> JFW_Send_Message_On_Key_Registrant("JFW_Send_Message_On_Key", "Key:string,Message:string,Red:int,Green:int,Blue:int");
ScriptRegistrant<JFW_Custom_Send_Random_Custom> JFW_Custom_Send_Random_Custom_Registrant("JFW_Custom_Send_Random_Custom", "RecieveMessage:int,SendMessage:int,SendParam:int,SendID:int,Percentage:int");
ScriptRegistrant<JFW_Startup_Custom_Self> JFW_Startup_Custom_Self_Registrant("JFW_Startup_Custom_Self","Message:int,Param=0:int,Delay=0:int");
ScriptRegistrant<JFW_Set_Skin_Custom> JFW_Set_Skin_Custom_Registrant("JFW_Set_Skin_Custom","Message:int,Armour:string");
ScriptRegistrant<JFW_Set_Armor_Custom> JFW_Set_Armor_Custom_Registrant("JFW_Set_Armor_Custom","Message:int,Armour:string");
ScriptRegistrant<JFW_Send_Custom_On_Damage> JFW_Send_Custom_On_Damage_Registrant("JFW_Send_Custom_On_Damage","Message1:int,ID1:int,Message2:int,ID2:int,Health:float");
