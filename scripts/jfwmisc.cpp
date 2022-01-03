/*  Renegade Scripts.dll
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
#include "jfwmisc.h"
#include "TeamPurchaseSettingsDefClass.h"
#include "PurchaseSettingsDefClass.h"
#include "BuildingGameObjDef.h"
#include "ConstructionYardGameObj.h"
#include "RefineryGameObj.h"
#include "ArmorWarheadManager.h"
#include "VehicleGameObj.h"
#include "WeaponClass.h"
#include "VehicleFactoryGameObj.h"
#include "AirFactoryGameObj.h"
#include "NavalFactoryGameObj.h"
#include "PurchaseSettingsDefClass.h"
#include "GameObjManager.h"
#include "dp88_custom_timer_defines.h"
#include "VehicleGameObjDef.h"
#include "PhysicsSceneClass.h"
#include "physcoltest.h"
#include "Random2Class.h"
#include "WeaponManager.h"
#include "MoveablePhysClass.h"
void JFW_User_Settable_Parameters::Created(GameObject *obj)
{
	const char *filename;
	const char *scriptname;
	char params[100];
	params[99] = 0;
	filename = Get_Parameter("File_Name");
	scriptname = Get_Parameter("Script_Name");
	FILE* f = fopen(filename, "r");
	if(f)
	{
#pragma warning(suppress: 6031) //warning C6031: return value ignored
		fscanf(f, "%99[^\n]", params);
		fclose(f);
		char* lineEnd = strpbrk(params, "\n\r");
		if(lineEnd)
		{
			lineEnd[0] = 0;
		}
		Commands->Attach_Script(obj,scriptname,params);
	}
}

void JFW_Debug_Text_File::Created(GameObject *obj)
{
	int id;
	t = time(0);
	filename = Get_Parameter("Log_File");
	description = Get_Parameter("Description");
	const char *path2 = Get_File_Path();
	char path[MAX_PATH];
	strcpy(path,path2);
	strcat(path,filename);
	f = fopen(path,"wt");
	id = Commands->Get_ID(obj);
	fprintf(f,"%s [ID %d] created.\n",description,id);
	fflush(f);
}

void JFW_Debug_Text_File::Destroyed(GameObject *obj)
{
	int id;
	id = Commands->Get_ID(obj);
	fprintf(f,"%s [ID %d] shutdown.\n",description,id);
	fflush(f);
	fclose(f);
}

void JFW_Debug_Text_File::Killed(GameObject *obj,GameObject *killer)
{
	float timediff;
	int id,id2;
	t2 = time(0);
	timediff = (float)difftime(t2,t);
	id = Commands->Get_ID(obj);
	id2 = Commands->Get_ID(killer);
	fprintf(f,"%s [ID %d] killed by object %d. %f sec.\n",description,id,id2,timediff);
	fflush(f);
}

void JFW_Debug_Text_File::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	float timediff;
	int id,id2;
	t2 = time(0);
	timediff = (float)difftime(t2,t);
	id = Commands->Get_ID(obj);
	id2 = Commands->Get_ID(damager);
	fprintf(f,"%s [ID %d] damaged by object %d. Damage was %f %f sec.\n",description,id,id2,amount,timediff);
	fflush(f);
}

void JFW_Debug_Text_File::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	float timediff;
	int id,id2;
	t2 = time(0);
	timediff = (float)difftime(t2,t);
	id = Commands->Get_ID(obj);
	id2 = Commands->Get_ID(sender);
	fprintf(f,"%s [ID %d] received custom event of type %d and param %d. Sender was object %d. %f sec.\n",description,id,type,param,id2,timediff);
	fflush(f);
}

void JFW_Debug_Text_File::Sound_Heard(GameObject *obj,const CombatSound & sound)
{
	float timediff;
	int id;
	t2 = time(0);
	timediff = (float)difftime(t2,t);
	id = Commands->Get_ID(obj);
	fprintf(f,"%s [ID %d] heard sound %d at position %f %f %f object %d. %f sec.\n",description,id,sound.sound,sound.Position.X,sound.Position.Y,sound.Position.Z,Commands->Get_ID(sound.Creator),timediff);
	fflush(f);
}

void JFW_Debug_Text_File::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	float timediff;
	int id,id2;
	t2 = time(0);
	timediff = (float)difftime(t2,t);
	id = Commands->Get_ID(obj);
	id2 = Commands->Get_ID(enemy);
	fprintf(f,"%s [ID %d] saw enemy: object %d. %f sec.\n",description,id,id2,timediff);
	fflush(f);
}

void JFW_Debug_Text_File::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	float timediff;
	int id;
	const char *reasonstr;
	t2 = time(0);
	timediff = (float)difftime(t2,t);
	id = Commands->Get_ID(obj);
	switch (complete_reason)
	{
	case ACTION_COMPLETE_ATTACK_OUT_OF_RANGE:
		reasonstr = "ACTION_COMPLETE_ATTACK_OUT_OF_RANGE";
		break;
	case ACTION_COMPLETE_MOVE_NO_PROGRESS_MADE:
		reasonstr = "ACTION_COMPLETE_MOVE_NO_PROGRESS_MADE";
		break;
	case ACTION_COMPLETE_PATH_BAD_DEST:
		reasonstr = "ACTION_COMPLETE_PATH_BAD_DEST";
		break;
	case ACTION_COMPLETE_PATH_BAD_START:
		reasonstr = "ACTION_COMPLETE_PATH_BAD_START";
		break;
	case ACTION_COMPLETE_LOW_PRIORITY:
		reasonstr = "ACTION_COMPLETE_LOW_PRIORITY";
		break;
	case ACTION_COMPLETE_NORMAL:
		reasonstr = "ACTION_COMPLETE_NORMAL";
		break;
	default:
		reasonstr = "ACTION_COMPLETE_NORMAL";
	}
	fprintf(f,"Action %d complete on %s [ID %d] -- Reason: %s. %f sec.\n",action_id,description,id,reasonstr,timediff);
	fflush(f);
}

void JFW_Debug_Text_File::Timer_Expired(GameObject *obj,int number)
{
	float timediff;
	int id;
	t2 = time(0);
	timediff = (float)difftime(t2,t);
	id = Commands->Get_ID(obj);
	fprintf(f,"timer %d on %s [ID %d] expired. %f sec.\n",number,description,id,timediff);
	fflush(f);
}

void JFW_Debug_Text_File::Animation_Complete(GameObject *obj,const char *animation_name)
{
	float timediff;
	int id;
	t2 = time(0);
	timediff = (float)difftime(t2,t);
	id = Commands->Get_ID(obj);
	fprintf(f,"animation %s on %s [ID %d] complete. %f sec.\n",animation_name,description,id,timediff);
	fflush(f);
}

void JFW_Debug_Text_File::Poked(GameObject *obj,GameObject *poker)
{
	float timediff;
	int id,id2;
	t2 = time(0);
	timediff = (float)difftime(t2,t);
	id = Commands->Get_ID(obj);
	id2 = Commands->Get_ID(poker);
	fprintf(f,"%s [ID %d] was poked by: object %d. %f sec.\n",description,id,id2,timediff);
	fflush(f);
}

void JFW_Debug_Text_File::Entered(GameObject *obj,GameObject *enterer)
{
	float timediff;
	int id,id2;
	t2 = time(0);
	timediff = (float)difftime(t2,t);
	id = Commands->Get_ID(obj);
	id2 = Commands->Get_ID(enterer);
	fprintf(f,"%s [ID %d] was entered by: object %d. %f sec.\n",description,id,id2,timediff);
	fflush(f);
}

void JFW_Debug_Text_File::Exited(GameObject *obj,GameObject *exiter)
{
	float timediff;
	int id,id2;
	t2 = time(0);
	timediff = (float)difftime(t2,t);
	id = Commands->Get_ID(obj);
	id2 = Commands->Get_ID(exiter);
	fprintf(f,"%s [ID %d] was exited by: object %d. %f sec.\n",description,id,id2,timediff);
	fflush(f);
}

void JFW_Power_Off::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg,msg2;
	msg = Get_Int_Parameter("Message_Off");
	msg2 = Get_Int_Parameter("Message_On");
	if (msg == type)
	{
		Commands->Set_Building_Power(obj,false);
	}
	if (msg2 == type)
	{
		Commands->Set_Building_Power(obj,true);
	}
}

void JFW_Follow_Waypath::Created(GameObject *obj)
{
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
	Commands->Action_Goto(obj,params);
	Destroy_Script();
}

void JFW_Follow_Waypath_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (!_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Preset")))
	{
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
		Commands->Action_Goto(enterer,params);
	}
}

void JFW_Object_Draw_In_Order::Created(GameObject *obj)
{
	currentmodelid = 0;
	currentmodelnumber = 1;
}

void JFW_Object_Draw_In_Order::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	Vector3 location;
	const char *basename;
	char name[20];
	int count;
	float facing;
	GameObject *object;
	msg = Get_Int_Parameter("Custom");
	if (type == msg)
	{
		if (currentmodelid)
		{
			Commands->Destroy_Object(Commands->Find_Object(currentmodelid));
		}
		location = Get_Vector3_Parameter("location");
		basename = Get_Parameter("BaseName");
		count = Get_Int_Parameter("Count");
		facing = Get_Float_Parameter("Facing");
		sprintf(name,"%s%i",basename,currentmodelnumber);
		object = Commands->Create_Object("Generic_Cinematic",location);
		Commands->Enable_Hibernation(object,false);
		Commands->Add_To_Dirty_Cull_List(object);
		Commands->Set_Model(object,name);
		Commands->Set_Facing(object,facing);
		currentmodelid = Commands->Get_ID(object);
		currentmodelnumber++;
		if (currentmodelnumber > count)
		{
			currentmodelnumber = 1;
		}
	}
}

void JFW_Object_Draw_In_Order::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&currentmodelnumber,4,1);
	Auto_Save_Variable(&currentmodelid,4,2);
}

void JFW_Object_Draw_In_Order_2::Created(GameObject *obj)
{
	currentmodelid = 0;
	currentmodelnumber = Get_Int_Parameter("Start_Number");
}

void JFW_Object_Draw_In_Order_2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	Vector3 location;
	const char *basename;
	char name[20];
	int count;
	float facing;
	GameObject *object;
	msg = Get_Int_Parameter("Custom");
	if (type == msg)
	{
		if (currentmodelid)
		{
			Commands->Destroy_Object(Commands->Find_Object(currentmodelid));
		}
		location = Get_Vector3_Parameter("location");
		basename = Get_Parameter("BaseName");
		count = Get_Int_Parameter("Count");
		facing = Get_Float_Parameter("Facing");
		sprintf(name,"%s%i",basename,currentmodelnumber);
		object = Commands->Create_Object("Generic_Cinematic",location);
		Commands->Enable_Hibernation(object,false);
		Commands->Add_To_Dirty_Cull_List(object);
		Commands->Set_Model(object,name);
		Commands->Set_Facing(object,facing);
		currentmodelid = Commands->Get_ID(object);
		currentmodelnumber++;
		if (currentmodelnumber > count)
		{
			currentmodelnumber = Get_Int_Parameter("Start_Number");
		}
	}
}

void JFW_Object_Draw_In_Order_2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&currentmodelnumber,4,1);
	Auto_Save_Variable(&currentmodelid,4,2);
}

void JFW_Object_Draw_Random::Created(GameObject *obj)
{
	currentmodelid = 0;
}

void JFW_Object_Draw_Random::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	Vector3 location;
	const char *basename;
	char name[20];
	int count;
	int currentmodelnumber;
	float facing;
	GameObject *object;
	msg = Get_Int_Parameter("Custom");
	if (type == msg)
	{
		if (currentmodelid)
		{
			Commands->Destroy_Object(Commands->Find_Object(currentmodelid));
		}
		Commands->Destroy_Object(Commands->Find_Object(currentmodelid));
		location = Get_Vector3_Parameter("location");
		basename = Get_Parameter("BaseName");
		count = Get_Int_Parameter("Count");
		facing = Get_Float_Parameter("Facing");
		currentmodelnumber = Commands->Get_Random_Int(1,count);
		sprintf(name,"%s%i",basename,currentmodelnumber);
		object = Commands->Create_Object("Generic_Cinematic",location);
		Commands->Enable_Hibernation(object,false);
		Commands->Add_To_Dirty_Cull_List(object);
		Commands->Set_Model(object,name);
		Commands->Set_Facing(object,facing);
		currentmodelid = Commands->Get_ID(object);
	}
}

void JFW_Object_Draw_Random::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&currentmodelid,4,1);
}

void JFW_Play_Animation_Destroy_Object::Created(GameObject *obj)
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
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,subobject,firstframe,Get_Float_Parameter("LastFrame"),Get_Bool_Parameter("Blended"));
}

void JFW_Model_Animation::Created(GameObject *obj)
{
	const char *model = Get_Model(obj);
	char *anim = new char[(strlen(model) * 2) + 2];
	sprintf(anim,"%s.%s",model,model);
	Commands->Set_Animation(obj,anim,false,0,0,-1,false);
	delete[] anim;
	Destroy_Script();
}

void JFW_Model_Animation_2::Created(GameObject *obj)
{
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,-1,false);
	Destroy_Script();
}

void JFW_Play_Animation_Destroy_Object::Animation_Complete(GameObject *obj,const char *animation_name)
{
	Commands->Destroy_Object(obj);
}

void JFW_Animated_Effect::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Vector3 pos = Get_Vector3_Parameter("Location");
		GameObject *object = Commands->Create_Object("Generic_Cinematic",pos);
		Commands->Set_Model(object,Get_Parameter("Model"));
		const char *subobject = Get_Parameter("Subobject");
		if (!_stricmp(subobject,"0"))
		{
			subobject = 0;
		}
		float firstframe = Get_Float_Parameter("FirstFrame");
		Commands->Set_Animation(object,Get_Parameter("Animation"),false,subobject,firstframe,Get_Float_Parameter("LastFrame"),Get_Bool_Parameter("Blended"));
	}
}

void JFW_Animated_Effect_2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *object = Commands->Create_Object("Generic_Cinematic",pos);
		Commands->Set_Model(object,Get_Parameter("Model"));
		const char *subobject = Get_Parameter("Subobject");
		if (!_stricmp(subobject,"0"))
		{
			subobject = 0;
		}
		float firstframe = Get_Float_Parameter("FirstFrame");
		Commands->Set_Animation(object,Get_Parameter("Animation"),false,subobject,firstframe,Get_Float_Parameter("LastFrame"),Get_Bool_Parameter("Blended"));
}

void JFW_Random_Animated_Effect::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Vector3 pos = Get_Vector3_Parameter("Location");
		Vector3 offset = Get_Vector3_Parameter("Offset");
		pos.X += Commands->Get_Random(-offset.X,offset.X);
		pos.Y += Commands->Get_Random(-offset.Y,offset.Y);
		pos.Z += Commands->Get_Random(-offset.Z,offset.Z);
		GameObject *object = Commands->Create_Object("Generic_Cinematic",pos);
		Commands->Set_Model(object,Get_Parameter("Model"));
		const char *subobject = Get_Parameter("Subobject");
		if (!_stricmp(subobject,"0"))
		{
			subobject = 0;
		}
		float firstframe = Get_Float_Parameter("FirstFrame");
		Commands->Set_Animation(object,Get_Parameter("Animation"),false,subobject,firstframe,Get_Float_Parameter("LastFrame"),Get_Bool_Parameter("Blended"));
	}
}

void JFW_Random_Animated_Effect_2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
		Vector3 pos = Commands->Get_Position(obj);
		Vector3 offset = Get_Vector3_Parameter("Offset");
		pos.X += Commands->Get_Random(-offset.X,offset.X);
		pos.Y += Commands->Get_Random(-offset.Y,offset.Y);
		pos.Z += Commands->Get_Random(-offset.Z,offset.Z);
		GameObject *object = Commands->Create_Object("Generic_Cinematic",pos);
		Commands->Set_Model(object,Get_Parameter("Model"));
		const char *subobject = Get_Parameter("Subobject");
		if (!_stricmp(subobject,"0"))
		{
			subobject = 0;
		}
		float firstframe = Get_Float_Parameter("FirstFrame");
		Commands->Set_Animation(object,Get_Parameter("Animation"),false,subobject,firstframe,Get_Float_Parameter("LastFrame"),Get_Bool_Parameter("Blended"));
}

void JFW_Animated_Effect::Animation_Complete(GameObject *obj,const char *animation_name)
{
	Commands->Destroy_Object(obj);
}

void JFW_Animated_Effect_2::Animation_Complete(GameObject *obj,const char *animation_name)
{
	Commands->Destroy_Object(obj);
}

void JFW_Random_Animated_Effect_2::Animation_Complete(GameObject *obj,const char *animation_name)
{
	Commands->Destroy_Object(obj);
}

void JFW_Random_Animated_Effect::Animation_Complete(GameObject *obj,const char *animation_name)
{
	Commands->Destroy_Object(obj);
}

void JFW_Fog_Create::Created(GameObject *obj)
{
	if (Get_Int_Parameter("OnCreate") > 0)
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

void JFW_Fog_Create::Destroyed(GameObject *obj)
{
	if (Get_Int_Parameter("OnDestroy") > 0)
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

void JFW_Fog_Create::Timer_Expired(GameObject *obj,int number)
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

void JFW_War_Blitz_Create::Created(GameObject *obj)
{
	if (Get_Int_Parameter("OnCreate") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
			Commands->Set_War_Blitz(Get_Float_Parameter("War_Blitz_Intensity"),Get_Float_Parameter("Start_Distance"),Get_Float_Parameter("End_Distance"),Get_Float_Parameter("War_Blitz_Heading"),Get_Float_Parameter("War_Blitz_Distribution"),1.0f);
	}
}

void JFW_War_Blitz_Create::Destroyed(GameObject *obj)
{
	if (Get_Int_Parameter("OnDestroy") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else 
			Commands->Set_War_Blitz(Get_Float_Parameter("War_Blitz_Intensity"),Get_Float_Parameter("Start_Distance"),Get_Float_Parameter("End_Distance"),Get_Float_Parameter("War_Blitz_Heading"),Get_Float_Parameter("War_Blitz_Distribution"),1.0f);
	}
}

void JFW_War_Blitz_Create::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
		Commands->Set_War_Blitz(Get_Float_Parameter("War_Blitz_Intensity"),Get_Float_Parameter("Start_Distance"),Get_Float_Parameter("End_Distance"),Get_Float_Parameter("War_Blitz_Heading"),Get_Float_Parameter("War_Blitz_Distribution"),1.0f);
}

void JFW_Goto_Object_On_Startup::Created(GameObject *obj)
{
	ActionParamsStruct params;
	params.Set_Basic(this,100,2);
	params.Set_Movement(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Float_Parameter("Speed"),Get_Float_Parameter("Arrivedistance"));
	Commands->Action_Goto(obj,params);
	Destroy_Script();
}

void JFW_Scope::Created(GameObject *obj)
{
}

void JFW_HUD_INI::Created(GameObject *obj)
{
	Load_New_HUD_INI(obj,Get_Parameter("HUDINI"));
}

void JFW_Screen_Fade_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Set_Screen_Fade_Opacity_Player(obj,Get_Float_Parameter("Opacity"),0.0f);
		Set_Screen_Fade_Color_Player(obj,Get_Float_Parameter("Red"),Get_Float_Parameter("Green"),Get_Float_Parameter("Blue"),0.0f);
	}
}

void JFW_Screen_Fade_On_Enter::Entered(GameObject *obj,GameObject *enterer)
{
	Set_Screen_Fade_Opacity_Player(enterer,Get_Float_Parameter("Opacity"),0.0f);
	Set_Screen_Fade_Color_Player(enterer,Get_Float_Parameter("Red"),Get_Float_Parameter("Green"),Get_Float_Parameter("Blue"),0.0f);
}

void JFW_Screen_Fade_On_Exit::Exited(GameObject *obj,GameObject *exiter)
{
	Set_Screen_Fade_Opacity_Player(exiter,Get_Float_Parameter("Opacity"),0.0f);
	Set_Screen_Fade_Color_Player(exiter,Get_Float_Parameter("Red"),Get_Float_Parameter("Green"),Get_Float_Parameter("Blue"),0.0f);
}

void JFW_BHS_DLL::Created(GameObject *obj)
{
	Console_Output("BANDTEST.DLL is required for this map");
	Destroy_Script();
}

void JFW_Screen_Fade_Custom_Timer::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		int TimerNum = Get_Int_Parameter("TimerNum");
		float Time = Get_Float_Parameter("Time");
		Commands->Start_Timer(obj,this,Time,TimerNum);
		Commands->Set_Screen_Fade_Color(Get_Float_Parameter("Red"),Get_Float_Parameter("Green"),Get_Float_Parameter("Blue"),0);
		Commands->Set_Screen_Fade_Opacity(Get_Float_Parameter("Opacity"),0);
	}
}

void JFW_Screen_Fade_Custom_Timer::Timer_Expired(GameObject *obj,int number)
{
	int TimerNum = Get_Int_Parameter("TimerNum");
	if (TimerNum == number)
	{
		Commands->Set_Screen_Fade_Color(0.0,0.0,0.0,0.0);
		Commands->Set_Screen_Fade_Opacity(0.0,0.0);
	}
}

void JFW_Stealthable_Object::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Commands->Enable_Stealth(obj,1);
		Commands->Start_Timer(obj,this,(float)2.80,1);
		Commands->Start_Timer(obj,this,(float)3.20,2);
		enabled = true;
	}
}

void JFW_Stealthable_Object::Timer_Expired(GameObject *obj,int number) 
{
	if (number == 1)
	{
		enabled = false;
	}
	if (number == 2 && !enabled)
	{
		Commands->Enable_Stealth(obj,0);
	}
}

void JFW_Stealthable_Object::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Object_Counter::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("IncrementMessage"))
	{
		count++;
	}
	if (type == Get_Int_Parameter("DecrementMessage"))
	{
		count--;
	}
	if (count >= Get_Int_Parameter("Count"))
	{
		Commands->Destroy_Object(sender);
	}
}

void JFW_Object_Counter::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&count,4,1);
}

void JFW_Change_Spawn_Character::Created(GameObject *obj)
{
	if (Get_Int_Parameter("Player_Type") == 0)
	{
		if(!The_Game()-> Get_Game_Duration_S())
		{
			SLNode<SoldierGameObj> *x = GameObjManager::StarGameObjList.Head();
			while (x)
			{
				GameObject *o = x->Data();
				if (o && Get_Object_Type(o)==0)
				{
					Change_Character(o,Get_Parameter("Character"));
				}
				x = x->Next();
			}
		}
		Set_Nod_Soldier_Name(Get_Parameter("Character"));
	}
	else
	{
		if(!The_Game()-> Get_Game_Duration_S())
		{
			SLNode<SoldierGameObj> *x = GameObjManager::StarGameObjList.Head();
			while (x)
			{
				GameObject *o = x->Data();
				if (o && Get_Object_Type(o)==1)
				{
					Change_Character(o,Get_Parameter("Character"));
				}
				x = x->Next();
			}
		}
		Set_GDI_Soldier_Name(Get_Parameter("Character"));
	}
	Destroy_Script();
}

void JFW_Show_Info_Texture::Created(GameObject *obj)
{
	Set_Info_Texture(obj,Get_Parameter("Texture"));
	int TimerNum = Get_Int_Parameter("TimerNum");
	float Time = Get_Float_Parameter("Time");
	Commands->Start_Timer(obj,this,Time,TimerNum);
}

void JFW_Show_Info_Texture::Timer_Expired(GameObject *obj,int number)
{
	Clear_Info_Texture(obj);
	Destroy_Script();
}

void JFW_PT_Hide::Created(GameObject *obj)
{
	Hide_Preset_By_Name(Get_Int_Parameter("Player_Type"),Get_Parameter("Preset"),true);
	Destroy_Script();
}

void JFW_PT_Hide_Death::Killed(GameObject *obj,GameObject *killer)
{
	Hide_Preset_By_Name(Get_Int_Parameter("Player_Type"),Get_Parameter("Preset"),true);
	Destroy_Script();
}

void JFW_PT_Hide_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Hide_Preset_By_Name(Get_Int_Parameter("Player_Type"),Get_Parameter("Preset"),true);
	}
}

void JFW_PT_Hide_Timer::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
}

void JFW_PT_Hide_Timer::Timer_Expired(GameObject *obj,int number)
{
	Hide_Preset_By_Name(Get_Int_Parameter("Player_Type"),Get_Parameter("Preset"),true);
	Destroy_Script();
}

void JFW_PT_Show::Created(GameObject *obj)
{
	Hide_Preset_By_Name(Get_Int_Parameter("Player_Type"),Get_Parameter("Preset"),false);
	Destroy_Script();
}

void JFW_PT_Show_Death::Killed(GameObject *obj,GameObject *killer)
{
	Hide_Preset_By_Name(Get_Int_Parameter("Player_Type"),Get_Parameter("Preset"),false);
	Destroy_Script();
}

void JFW_PT_Show_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Hide_Preset_By_Name(Get_Int_Parameter("Player_Type"),Get_Parameter("Preset"),false);
	}
}

void JFW_PT_Show_Timer::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
}

void JFW_PT_Show_Timer::Timer_Expired(GameObject *obj,int number)
{
	Hide_Preset_By_Name(Get_Int_Parameter("Player_Type"),Get_Parameter("Preset"),false);
	Destroy_Script();
}

void JFW_Change_Radar_Map::Created(GameObject *obj)
{
	Change_Radar_Map(Get_Float_Parameter("Scale"),Get_Float_Parameter("OffsetX"),Get_Float_Parameter("OffsetY"),Get_Parameter("Texture"));
	Destroy_Script();
}

void JFW_Goto_Player_Timer::Created(GameObject *obj)
{
	int TimerNum = Get_Int_Parameter("TimerNum");
	float Time = Get_Float_Parameter("Time");
	Commands->Start_Timer(obj,this,Time,TimerNum);
}

void JFW_Goto_Player_Timer::Timer_Expired(GameObject *obj,int number)
{
	int TimerNum = Get_Int_Parameter("TimerNum");
	float Time = Get_Float_Parameter("Time");
	if (number == TimerNum)
	{
		Commands->Action_Reset(obj,100);
		GameObject *object = Commands->Get_A_Star(Commands->Get_Position(obj));
		ActionParamsStruct params;
		params.Set_Basic(this,100,2);
		params.Set_Movement(object,Get_Float_Parameter("Speed"),Get_Float_Parameter("Arrivedistance"));
		Commands->Action_Goto(obj,params);
		Commands->Start_Timer(obj,this,Time,TimerNum);
	}
}

void JFW_Goto_Player_Timer::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	Commands->Action_Reset(obj,100);
}

void JFW_Credit_Grant::Created(GameObject *obj)
{
	float delay;
	delay = Get_Float_Parameter("Delay");
	Commands->Start_Timer(obj,this,delay,667);
}

void JFW_Credit_Grant::Timer_Expired(GameObject *obj,int number)
{
	if (number == 667)
	{
		Commands->Give_Money(Find_Smart_Object_By_Team(0),(float)Get_Int_Parameter("Credits"),true);
		Commands->Give_Money(Find_Smart_Object_By_Team(1),(float)Get_Int_Parameter("Credits"),true);
		float delay;
		delay = Get_Float_Parameter("Delay");
		Commands->Start_Timer(obj,this,delay,667);
	}
}

void JFW_Set_Info_Texture::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Set_Info_Texture(obj,Get_Parameter("Texture"));
	}
}

void JFW_Clear_Info_Texture::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Clear_Info_Texture(obj);
	}
}

void JFW_Set_Time_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Change_Time_Remaining(Get_Float_Parameter("Time"));
	}
}

void JFW_Set_Time_Limit_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Change_Time_Limit(Get_Int_Parameter("TimeLimit"));
	}
}

void JFW_Radar_Low_Power::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *o = Find_Building_By_Type(Get_Object_Type(obj),BuildingConstants::TYPE_COM_CENTER);
	if (o && !Is_Building_Dead(o))
	{
		Enable_Base_Radar(Get_Object_Type(obj),false);
	}
	Destroy_Script();
}

void JFW_Game_Hint_Soldier::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,5,1);
}

void JFW_Game_Hint_Soldier::Timer_Expired(GameObject *obj,int number)
{
	Display_Game_Hint(obj,Get_Int_Parameter("EventID"),Get_Parameter("Sound"),Get_Int_Parameter("TitleID"),Get_Int_Parameter("TextID"),Get_Int_Parameter("TextID2"),Get_Int_Parameter("TextID3"));
}

void JFW_Game_Hint_Vehicle::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_OWNER)
	{
		Commands->Start_Timer(obj,this,5,param);
	}
}

void JFW_Game_Hint_Vehicle::Timer_Expired(GameObject *obj,int number)
{
	Display_Game_Hint(Commands->Find_Object(number),Get_Int_Parameter("EventID"),Get_Parameter("Sound"),Get_Int_Parameter("TitleID"),Get_Int_Parameter("TextID"),Get_Int_Parameter("TextID2"),Get_Int_Parameter("TextID3"));
}

void JFW_Game_Hint_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	Display_Game_Hint(enterer,Get_Int_Parameter("EventID"),Get_Parameter("Sound"),Get_Int_Parameter("TitleID"),Get_Int_Parameter("TextID"),Get_Int_Parameter("TextID2"),Get_Int_Parameter("TextID3"));
}

void JFW_Game_Hint_Image_Soldier::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,5,1);
}

void JFW_Game_Hint_Image_Soldier::Timer_Expired(GameObject *obj,int number)
{
	Display_Game_Hint_Image(obj,Get_Int_Parameter("EventID"),Get_Parameter("Sound"),Get_Int_Parameter("TitleID"),Get_Int_Parameter("TextID"),Get_Int_Parameter("TextID2"),Get_Int_Parameter("TextID3"),Get_Parameter("Texture"));
}

void JFW_Game_Hint_Image_Vehicle::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_OWNER)
	{
		Commands->Start_Timer(obj,this,5,param);
	}
}

void JFW_Game_Hint_Image_Vehicle::Timer_Expired(GameObject *obj,int number)
{
	Display_Game_Hint_Image(Commands->Find_Object(number),Get_Int_Parameter("EventID"),Get_Parameter("Sound"),Get_Int_Parameter("TitleID"),Get_Int_Parameter("TextID"),Get_Int_Parameter("TextID2"),Get_Int_Parameter("TextID3"),Get_Parameter("Texture"));
}

void JFW_Game_Hint_Image_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,team))
	{
		return;
	}
	Display_Game_Hint_Image(enterer,Get_Int_Parameter("EventID"),Get_Parameter("Sound"),Get_Int_Parameter("TitleID"),Get_Int_Parameter("TextID"),Get_Int_Parameter("TextID2"),Get_Int_Parameter("TextID3"),Get_Parameter("Texture"));
}

void JFW_Moon_Is_Earth::Created(GameObject *obj)
{
	Set_Moon_Is_Earth(true);
	Destroy_Script();
}

void JFW_Points_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Commands->Give_Points(obj,(float)Get_Int_Parameter("Points"),Get_Bool_Parameter("Team"));
	}
}

void JFW_Display_String_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		const char *string = Get_Parameter("String");
		const char *sound = Get_Parameter("Sound");
		int red = Get_Int_Parameter("Red");
		int green = Get_Int_Parameter("Green");
		int blue = Get_Int_Parameter("Blue");
		int team = Get_Int_Parameter("Team");
		Send_Message_Team(team,red,green,blue,string);
		Create_2D_Sound_Team(sound,team);
	}
}

void JFW_Preset_Disable::Created(GameObject *obj)
{
	count = 0;
}

void JFW_Preset_Disable::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Disable_Custom"))
	{
		if (count == 0)
		{
			Disable_Preset_By_Name(Get_Object_Type(obj),Get_Parameter("Preset"),true);
		}
		count++;
	}
	if (type == Get_Int_Parameter("Enable_Custom"))
	{
		count--;
		if (count == 0)
		{
			Disable_Preset_By_Name(Get_Object_Type(obj),Get_Parameter("Preset"),false);
		}
	}
}

void JFW_Preset_Disable::Killed(GameObject *obj,GameObject *killer)
{
	Destroy_Script();
}

void JFW_Power_Disable::Created(GameObject *obj)
{
	count = 0;
}

void JFW_Power_Disable::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Disable_Custom"))
	{
		if (count == 0)
		{
			Power_Base(Get_Object_Type(obj),false);
		}
		count++;
	}
	if (type == Get_Int_Parameter("Enable_Custom"))
	{
		count--;
		if (count == 0)
		{
			Power_Base(Get_Object_Type(obj),true);
		}
	}
}

void JFW_Power_Disable::Killed(GameObject *obj,GameObject *killer)
{
	Destroy_Script();
}

void JFW_Radar_Disable::Created(GameObject *obj)
{
	count = 0;
}

void JFW_Radar_Disable::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Disable_Custom"))
	{
		if (count == 0)
		{
			Enable_Base_Radar(Get_Object_Type(obj),false);
		}
		count++;
	}
	if (type == Get_Int_Parameter("Enable_Custom"))
	{
		count--;
		if (count == 0)
		{
			Enable_Base_Radar(Get_Object_Type(obj),true);
		}
	}
}

void JFW_Radar_Disable::Killed(GameObject *obj,GameObject *killer)
{
	Destroy_Script();
}

void JFW_Conyard_Disable::Created(GameObject *obj)
{
	count = 0;
}

void JFW_Conyard_Disable::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->As_ConstructionYardGameObj())
	{
		if (type == Get_Int_Parameter("Disable_Custom"))
		{
			if (count == 0)
			{
				obj->As_BuildingGameObj()->As_ConstructionYardGameObj()->Set_Spy(true);
			}
			count++;
		}
		if (type == Get_Int_Parameter("Enable_Custom"))
		{
			count--;
			if (count == 0)
			{
				obj->As_BuildingGameObj()->As_ConstructionYardGameObj()->Set_Spy(false);
			}
		}
	}
}

void JFW_Conyard_Disable::Killed(GameObject *obj,GameObject *killer)
{
	Destroy_Script();
}

void JFW_Refinery_Disable::Created(GameObject *obj)
{
	count = 0;
}

void JFW_Refinery_Disable::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->As_RefineryGameObj())
	{
		if (type == Get_Int_Parameter("Disable_Custom"))
		{
			if (count == 0)
			{
				obj->As_BuildingGameObj()->As_RefineryGameObj()->Block_Harvester_Spawn();
			}
			count++;
		}
		if (type == Get_Int_Parameter("Enable_Custom"))
		{
			count--;
			if (count == 0)
			{
				obj->As_BuildingGameObj()->As_RefineryGameObj()->Allow_Harvester_Spawn();
			}
		}
	}
}

void JFW_Refinery_Disable::Killed(GameObject *obj,GameObject *killer)
{
	Destroy_Script();
}

void JFW_VehicleFactory_Disable::Created(GameObject *obj)
{
	count = 0;
}

void JFW_VehicleFactory_Disable::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->As_VehicleFactoryGameObj())
	{
		if (type == Get_Int_Parameter("Disable_Custom"))
		{
			if (count == 0)
			{
				obj->As_BuildingGameObj()->As_VehicleFactoryGameObj()->Set_Disabled(true);
				PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_VEHICLES,(PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(obj)))->Set_Page_Disabled(true);
				PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_SECRET_VEHICLES,(PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(obj)))->Set_Page_Disabled(true);
			}
			count++;
		}
		if (type == Get_Int_Parameter("Enable_Custom"))
		{
			count--;
			if (count == 0)
			{
				obj->As_BuildingGameObj()->As_VehicleFactoryGameObj()->Set_Disabled(false);
				PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_VEHICLES,(PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(obj)))->Set_Page_Disabled(false);
				PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_SECRET_VEHICLES,(PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(obj)))->Set_Page_Disabled(false);
			}
		}
	}
}

void JFW_VehicleFactory_Disable::Killed(GameObject *obj,GameObject *killer)
{
	Destroy_Script();
}

void JFW_AirFactory_Disable::Created(GameObject *obj)
{
	count = 0;
}

void JFW_AirFactory_Disable::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->As_AirFactoryGameObj())
	{
		if (type == Get_Int_Parameter("Disable_Custom"))
		{
			if (count == 0)
			{
				obj->As_BuildingGameObj()->As_AirFactoryGameObj()->Set_Disabled(true);
				PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_AIR,(PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(obj)))->Set_Page_Disabled(true);
			}
			count++;
		}
		if (type == Get_Int_Parameter("Enable_Custom"))
		{
			count--;
			if (count == 0)
			{
				obj->As_BuildingGameObj()->As_AirFactoryGameObj()->Set_Disabled(false);
				PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_AIR,(PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(obj)))->Set_Page_Disabled(false);
			}
		}
	}
}

void JFW_AirFactory_Disable::Killed(GameObject *obj,GameObject *killer)
{
	Destroy_Script();
}

void JFW_NavalFactory_Disable::Created(GameObject *obj)
{
	count = 0;
}

void JFW_NavalFactory_Disable::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->As_NavalFactoryGameObj())
	{
		if (type == Get_Int_Parameter("Disable_Custom"))
		{
			if (count == 0)
			{
				obj->As_BuildingGameObj()->As_NavalFactoryGameObj()->Set_Disabled(true);
				PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_NAVAL,(PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(obj)))->Set_Page_Disabled(true);
			}
			count++;
		}
		if (type == Get_Int_Parameter("Enable_Custom"))
		{
			count--;
			if (count == 0)
			{
				obj->As_BuildingGameObj()->As_NavalFactoryGameObj()->Set_Disabled(false);
				PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_NAVAL,(PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(obj)))->Set_Page_Disabled(false);
			}
		}
	}
}

void JFW_NavalFactory_Disable::Killed(GameObject *obj,GameObject *killer)
{
	Destroy_Script();
}

void JFW_SoldierFactory_Disable::Created(GameObject *obj)
{
	count = 0;
}

void JFW_SoldierFactory_Disable::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->As_SoldierFactoryGameObj())
	{
		if (type == Get_Int_Parameter("Disable_Custom"))
		{
			if (count == 0)
			{
				Set_Can_Generate_Soldiers(Commands->Get_Player_Type(obj),false);
			}
			count++;
		}
		if (type == Get_Int_Parameter("Enable_Custom"))
		{
			count--;
			if (count == 0)
			{
				Set_Can_Generate_Soldiers(Commands->Get_Player_Type(obj),true);
			}
		}
	}
}

void JFW_SoldierFactory_Disable::Killed(GameObject *obj,GameObject *killer)
{
	Destroy_Script();
}

void JFW_Engineer_Hack::Created(GameObject *obj)
{
	Warhead = ArmorWarheadManager::Get_Warhead_Type(Get_Parameter("Hack_Warhead"));
	HackHealth = Get_Float_Parameter("Hack_Health");
	IndcatorID = Commands->Get_ID(Find_Nearest_Preset(Commands->Get_Position(obj),Get_Parameter("Hack_Indicator_Preset")));
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Hack_Restore_Timer"),1);
}
void JFW_Engineer_Hack::Killed(GameObject *obj,GameObject *killer)
{
	Destroy_Script();
}
void JFW_Engineer_Hack::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (Warhead == Get_Damage_Warhead())
	{
		if (Get_Object_Type(obj) == Get_Object_Type(damager))
		{
			Apply_Repair(obj,Get_Float_Parameter("Hack_Repair"));
		}
		else
		{
			Apply_Damage(obj,Get_Float_Parameter("Hack_Damage"));
		}
	}
}
void JFW_Engineer_Hack::Timer_Expired(GameObject *obj,int number)
{
	Apply_Repair(obj,Get_Float_Parameter("Hack_Restore_Amount"));
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Hack_Restore_Timer"),1);
}
void JFW_Engineer_Hack::Apply_Damage(GameObject *obj,float damage)
{
	float oldhealth = HackHealth;
	HackHealth -= damage;
	float offline = Get_Float_Parameter("Hack_Offline_Amount");
	if (HackHealth <= 0)
	{
		Commands->Apply_Damage(obj,99999,"Death",0);
	}
	if (oldhealth > offline && HackHealth <= offline)
	{
		Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("Hack_Offline_Custom"),0,0);
		Commands->Send_Custom_Event(Commands->Find_Object(IndcatorID),obj,Get_Int_Parameter("Hack_Offline_Custom"),0,0);
	}
}
void JFW_Engineer_Hack::Apply_Repair(GameObject *obj,float repair)
{
	float oldhealth = HackHealth;
	HackHealth += repair;
	if (HackHealth > Get_Float_Parameter("Hack_Health"))
	{
		HackHealth = Get_Float_Parameter("Hack_Health");
	}
	float offline = Get_Float_Parameter("Hack_Offline_Amount");
	if (oldhealth <= offline && HackHealth > offline)
	{
		Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("Hack_Online_Custom"),0,0);
		Commands->Send_Custom_Event(Commands->Find_Object(IndcatorID),obj,Get_Int_Parameter("Hack_Online_Custom"),0,0);
	}
}

void JFW_Double_Animation::Created(GameObject *obj)
{
	StringClass str = Get_Model(obj);
	str += ".";
	str += Get_Model(obj);
	Commands->Set_Animation(obj,str,0,0,0,Get_Float_Parameter("End_Frame"),0);
}
void JFW_Double_Animation::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("First_Custom"))
	{
		StringClass str = Get_Model(obj);
		str += ".";
		str += Get_Model(obj);
		Commands->Set_Animation(obj,str,0,0,0,Get_Float_Parameter("End_Frame"),0);
	}
	if (type == Get_Int_Parameter("Second_Custom"))
	{
		StringClass str = Get_Model(obj);
		str += ".";
		str += Get_Model(obj);
		Commands->Set_Animation(obj,str,0,0,Get_Float_Parameter("End_Frame")+1,-1,0);
	}
}
bool DisableEmp = false;
void JFW_EMP::Created(GameObject *obj)
{
	Warhead = ArmorWarheadManager::Get_Warhead_Type(Get_Parameter("Warhead"));
}

void JFW_EMP::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (Warhead == Get_Damage_Warhead() && !DisableEmp)
	{
		VehicleGameObj *o = obj->As_VehicleGameObj();
		if (o && o->Get_Is_Scripts_Visible() && o->Get_Scripts_Can_Fire() && o->Can_Drive())
		{
			Commands->Enable_Engine(obj,false);
			o->Set_Can_Drive(false);
			o->Set_Scripts_Can_Fire(false);
			o->Set_Stealth_Active(false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
			if (Get_Parameter("Animation") && Get_Parameter("Animation")[0])
			{
				Commands->Set_Animation(obj,Get_Parameter("Animation"),true,0,0,-1,false);
			}
			obj->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
		}
		SoldierGameObj *s = obj->As_SoldierGameObj();
		if (s && !s->Is_Frozen() && s->Is_Visible())
		{
			s->Set_Freeze(true);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
		}
	}
}

void JFW_EMP::Timer_Expired(GameObject *obj,int number)
{
	VehicleGameObj *o = obj->As_VehicleGameObj();
	if (o)
	{
		o->Clear_Animation();
		obj->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
		o->Set_Can_Drive(true);
		o->Set_Stealth_Active(true);
		Commands->Enable_Engine(obj,true);
		o->Set_Scripts_Can_Fire(true);
		const VehicleGameObjDef &definition = obj->As_VehicleGameObj()->Get_Definition();
		if (!definition.Get_Animation().Is_Empty())
		{
			obj->As_VehicleGameObj()->Set_Animation(definition.Get_Animation(),true,0);
		}
	}
	SoldierGameObj *s = obj->As_SoldierGameObj();
	if (s)
	{
		s->Set_Freeze(false);
	}
}

void JFW_EMP_Mine::Created(GameObject *obj)
{
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	if (obj->As_VehicleGameObj())
	{
		obj->As_VehicleGameObj()->Set_Is_Scripts_Visible(false);
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
}

void JFW_EMP_Mine::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	if ((Is_Script_Attached(enemy,"JFW_EMP") || Is_Script_Attached(enemy,"JFW_EMP_Indicator")) && !DisableEmp)
	{
		bool fire = false;
		VehicleGameObj *o = enemy->As_VehicleGameObj();
		if (o && o->Get_Is_Scripts_Visible() && o->Get_Scripts_Can_Fire() && o->Can_Drive())
		{
			fire = true;
		}
		SoldierGameObj *s = enemy->As_SoldierGameObj();
		if (s && !s->Is_Frozen() && s->Is_Visible())
		{
			fire = true;
		}
		if (fire)
		{
			Commands->Create_Explosion(Get_Parameter("Explosion"),Commands->Get_Position(obj),0);
			Commands->Apply_Damage(obj,99999,"Death",0);
		}
	}
}

void JFW_EMP_Mine::Timer_Expired(GameObject *obj,int number)
{
	Commands->Enable_Enemy_Seen(obj,true);
}

void JFW_EMP_Mine::Destroyed(GameObject *obj)
{
	Commands->Send_Custom_Event(obj, Commands->Find_Object(Get_Int_Parameter("Mine_Manager_ID")),111111,0,0);
}

void JFW_EMP_Mine_Manager::Created(GameObject *obj)
{
	DisableEmp = false;
	mines = 0;
}

void JFW_EMP_Mine_Manager::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 111111)
	{
		mines--;
	}
	else if (type == 222222)
	{
		int limit = Get_Int_Parameter("Mine_Limit");
		if (mines < limit)
		{
			mines++;
			SoldierGameObj *s = sender->As_SoldierGameObj();
			if (s)
			{
				Vector3 v = Commands->Get_Position(sender);
				v.Z += Get_Float_Parameter("Mine_Z_Offset");
				float len = Get_Float_Parameter("Mine_Distance");
				float angle = DEG_TO_RADF(Commands->Get_Facing(sender));
				v.X += cos(angle) * len;
				v.Y += sin(angle) * len;
				GameObject *mine = Commands->Create_Object(Get_Parameter("Mine_Preset"),v);
				Commands->Set_Player_Type(mine,Commands->Get_Player_Type(sender));
				PhysicalGameObj *p = mine->As_PhysicalGameObj();
				p->Peek_Physical_Object()->Set_Collision_Group(TERRAIN_AND_BULLET_COLLISION_GROUP);
			}
		}
		else
		{
			Commands->Give_PowerUp(sender, Get_Parameter("Mine_Reload"), false);
		}
	}
}

void JFW_EMP_Mine_Manager_2::Created(GameObject *obj)
{
	DisableEmp = false;
	mines = 0;
}

void JFW_EMP_Mine_Manager_2::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 111111)
	{
		mines--;
	}
	else if (type == 222222)
	{
		int limit = Get_Int_Parameter("Mine_Limit");
		if (mines < limit)
		{
			mines++;
			SoldierGameObj *s = sender->As_SoldierGameObj();
			if (s)
			{
				Vector3 v = Commands->Get_Position(sender);
				GameObject *mine = Commands->Create_Object(Get_Parameter("Mine_Preset"),v);
				Commands->Set_Player_Type(mine,Commands->Get_Player_Type(sender));
				PhysicalGameObj *p = mine->As_PhysicalGameObj();
				p->Peek_Physical_Object()->Set_Collision_Group(TERRAIN_AND_BULLET_COLLISION_GROUP);
				ArmedGameObj *a = p->As_ArmedGameObj();
				Matrix3D muzzle = a->Get_Muzzle();
				Matrix3D transform(true);
				transform.Rotate_Z(muzzle.Get_Z_Rotation());
				transform.Set_Translation(muzzle.Get_Translation());
				p->Peek_Physical_Object()->Set_Transform(transform);
				MoveablePhysClass *m = p->Peek_Physical_Object()->As_MoveablePhysClass();
				Vector3 velocity = muzzle.Get_X_Vector() * Get_Float_Parameter("Throw_Velocity");
				m->Set_Velocity(velocity);
				p->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
			}
		}
		else
		{
			Commands->Give_PowerUp(sender, Get_Parameter("Mine_Reload"), false);
		}
	}
}

void JFW_EMP_Mine_Layer::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	unsigned int warhead = ArmorWarheadManager::Get_Warhead_Type(Get_Parameter("Warhead"));
	if (warhead == Get_Damage_Warhead() && damager == obj)
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("Mine_Manager_ID")), 222222,0,0);
	}
}

void JFW_Warhead_Custom::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	unsigned int warhead = ArmorWarheadManager::Get_Warhead_Type(Get_Parameter("Warhead"));
	if (warhead == Get_Damage_Warhead())
	{
		Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("Message"),0,0);
	}
}

void JFW_Spy_Disguise_Target::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	unsigned int warhead = ArmorWarheadManager::Get_Warhead_Type(Get_Parameter("Warhead"));
	if (warhead == Get_Damage_Warhead())
	{
		const char *model = Get_Parameter("Model");
		if (!*model)
		{
			model = Get_Model(obj);
		}
		if (*model)
		{
			Commands->Set_Model(damager, model);
		}
	}
}

void JFW_Radar_Disable_Death::Killed(GameObject *obj,GameObject *killer)
{
	Enable_Base_Radar(Get_Object_Type(obj),false);
	Destroy_Script();
}

void JFW_Cyborg_Reaper::Created(GameObject *obj)
{
	driverid = 0;
}

void JFW_Cyborg_Reaper::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		Commands->Start_Timer(obj,this,1,1);
		Commands->Attach_Script(Commands->Find_Object(driverid),"RA_DriverDeath", "0");
	}
	else if (type == CUSTOM_EVENT_VEHICLE_OWNER)
	{
		Force_Vehicle_Entry(sender,obj);
		driverid = Commands->Get_ID(sender);
	}
}

void JFW_Cyborg_Reaper::Timer_Expired(GameObject *obj,int number)
{
	Commands->Apply_Damage(obj,99999,"Death",0);
}

void JFW_Cyborg_Reaper::Killed(GameObject *obj,GameObject *killer)
{
	Commands->Attach_Script(Commands->Find_Object(driverid),"RA_DriverDeath", "0");
}

void JFW_Limpet_Drone::Created(GameObject *obj)
{
	stealth = false;
	if (obj->As_VehicleGameObj())
	{
		obj->As_VehicleGameObj()->Set_Is_Scripts_Visible(false);
	}
}

void JFW_Limpet_Drone::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 923572385)
	{
		stealth = !stealth;
		Commands->Enable_Stealth(obj,stealth);
	}
}

void JFW_Forward_Custom_Object::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("Object_ID")),type,param,0);
}

void JFW_Death_Send_Custom_Self::Killed(GameObject *obj,GameObject *killer)
{
	Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("Message"),0,0);
	Destroy_Script();
}

void JFW_Hunter_Seeker::Created(GameObject *obj)
{
	InstallHook(Get_Parameter("Key"),obj);
	driverid = 0;
}

void JFW_Hunter_Seeker::KeyHook()
{
	Commands->Start_Timer(Owner(),this,5,1);
	Soldier_Transition_Vehicle(Commands->Find_Object(driverid));
}

void JFW_Hunter_Seeker::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		Commands->Start_Timer(obj,this,5,2);
	}
	else if (type == CUSTOM_EVENT_VEHICLE_OWNER)
	{
		driverid = Commands->Get_ID(sender);
	}
}

void JFW_Hunter_Seeker::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Apply_Damage(obj,99999,"Death",0);
		Commands->Create_Explosion(Get_Parameter("Explosion"),Commands->Get_Position(obj),0);
	}
	else if (number == 2)
	{
		Commands->Apply_Damage(obj,99999,"Death",0);
	}
}

void JFW_Ion_Storm::Created(GameObject *obj)
{
	DisableEmp = false;
	int chance = Commands->Get_Random_Int(0,100);
	//FILE *f = fopen("ion.log","wt");
	//fprintf(f,"Ion Storm script initializing, random chance chosen is %d, IonChance is %d\n",chance,Get_Int_Parameter("IonChance"));
	//fclose(f);
	if (chance <= Get_Int_Parameter("IonChance"))
	{
		//FILE *f = fopen("ion.log","at");
		//fprintf(f,"Ion Storm random chance too low, no storms this game\n");
		//fclose(f);
		return;
	}
	storm = false;
	float min = Get_Float_Parameter("Min_Delay");
	float max = Get_Float_Parameter("Max_Delay");
	if (max > The_Game()->Get_Time_Remaining_Seconds())
	{
		max = The_Game()->Get_Time_Remaining_Seconds();
	}
	if (min > The_Game()->Get_Time_Remaining_Seconds())
	{
		Destroy_Script();
		return;
	}
	if (min > max)
	{
		Destroy_Script();
		return;
	}
	float timer = Commands->Get_Random(min,max);
	//FILE *f2 = fopen("ion.log","at");
	//fprintf(f2,"Ion Storm timer starting, max delay is %f, min delay is %f, actual delay is %f\n",min,max,timer);
	//fclose(f2);
	Commands->Start_Timer(obj,this,timer,1);
}
void JFW_Ion_Storm::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1) //start storm
	{
		//FILE *f = fopen("ion.log","at");
		//fprintf(f,"Ion Storm starting\n");
		storm = true;
		float min = Get_Float_Parameter("Min_Time");
		float max = Get_Float_Parameter("Max_Time");
		if (max > The_Game()->Get_Time_Remaining_Seconds())
		{
			max = The_Game()->Get_Time_Remaining_Seconds();
		}
		if (min > The_Game()->Get_Time_Remaining_Seconds())
		{
			Destroy_Script();
			//fclose(f);
			return;
		}
		if (min > max)
		{
			Destroy_Script();
			//fclose(f);
			return;
		}
		if (Get_Int_Parameter("DisableEmp"))
		{
			//fprintf(f,"Disable EMP\n");
			DisableEmp = true;
		}
		//fprintf(f,"Disable Stealth\n");
		Set_Global_Stealth_Disable(true);
		float timer = Commands->Get_Random(min,max);
		//fprintf(f,"Ion Storm run starting, max time is %f, min time is %f, actual time is %f\n",min,max,timer);
		Commands->Start_Timer(obj,this,timer,2);
		//fprintf(f,"Enable Ion Storm weather\n");
		Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("On_Weather_Custom"),0,0);
		if (Find_Building_By_Type(0,BuildingConstants::TYPE_COM_CENTER))
		{
			//fprintf(f,"Disable Nod Radar\n");
			Commands->Send_Custom_Event(obj,Find_Building_By_Type(0,BuildingConstants::TYPE_COM_CENTER),Get_Int_Parameter("Disable_Custom"),0,0);
		}
		if (Find_Building_By_Type(1,BuildingConstants::TYPE_COM_CENTER))
		{
			//fprintf(f,"Disable GDI Radar\n");
			Commands->Send_Custom_Event(obj,Find_Building_By_Type(1,BuildingConstants::TYPE_COM_CENTER),Get_Int_Parameter("Disable_Custom"),0,0);
		}
		if (Find_Building_By_Type(0,BuildingConstants::TYPE_POWER_PLANT))
		{
			//fprintf(f,"Disable Nod Power\n");
			Commands->Send_Custom_Event(obj,Find_Building_By_Type(0,BuildingConstants::TYPE_POWER_PLANT),Get_Int_Parameter("Disable_Custom"),0,0);
		}
		if (Find_Building_By_Type(1,BuildingConstants::TYPE_POWER_PLANT))
		{
			//fprintf(f,"Disable GDI Power\n");
			Commands->Send_Custom_Event(obj,Find_Building_By_Type(1,BuildingConstants::TYPE_POWER_PLANT),Get_Int_Parameter("Disable_Custom"),0,0);
		}
		//fprintf(f,"Disable Base Defenses\n");
		SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();
		while (x)
		{
			VehicleGameObj *o = x->Data();
			if (o && o->Get_Definition().Get_Encyclopedia_Type() == 3)
			{
				Commands->Send_Custom_Event(obj,0,CUSTOM_AI_DISABLEAI,0,0);
			}
			if (Is_Script_Attached(obj,"JFW_EMP_Mine") && Get_Int_Parameter("DestroyMines"))
			{
				Commands->Destroy_Object(obj);
			}
			x = x->Next();
		}
		Timer_Expired(obj,3);
		Create_2D_Sound_Team(Get_Parameter("Announcement_Sound_Nod"),0);
		Create_2D_Sound_Team(Get_Parameter("Announcement_Sound_GDI"),1);
		const char *string = Get_Parameter("Announcement_String");
		int red = Get_Int_Parameter("Red");
		int green = Get_Int_Parameter("Green");
		int blue = Get_Int_Parameter("Blue");
		//fprintf(f,"Send Ion Storm starting message\n");
		Send_Message(red,green,blue,string);
		//fclose(f);
	}
	else if (number == 2) //storm ended
	{
		//FILE *f = fopen("ion.log","at");
		//fprintf(f,"Ion Storm stopping\n");
		storm = false;
		//fprintf(f,"Enable Stealth\n");
		Set_Global_Stealth_Disable(false);
		if (Get_Int_Parameter("DisableEmp"))
		{
			//fprintf(f,"Enable EMP\n");
			DisableEmp = false;
		}
		//fprintf(f,"Disable Ion Storm Weather\n");
		Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("Off_Weather_Custom"),0,0);
		if (Find_Building_By_Type(0,BuildingConstants::TYPE_COM_CENTER))
		{
			//fprintf(f,"Enable Nod Radar\n");
			Commands->Send_Custom_Event(obj,Find_Building_By_Type(0,BuildingConstants::TYPE_COM_CENTER),Get_Int_Parameter("Enable_Custom"),0,0);
		}
		if (Find_Building_By_Type(1,BuildingConstants::TYPE_COM_CENTER))
		{
			//fprintf(f,"Enable GDI Radar\n");
			Commands->Send_Custom_Event(obj,Find_Building_By_Type(1,BuildingConstants::TYPE_COM_CENTER),Get_Int_Parameter("Enable_Custom"),0,0);
		}
		if (Find_Building_By_Type(0,BuildingConstants::TYPE_POWER_PLANT))
		{
			//fprintf(f,"Enable Nod Power\n");
			Commands->Send_Custom_Event(obj,Find_Building_By_Type(0,BuildingConstants::TYPE_POWER_PLANT),Get_Int_Parameter("Enable_Custom"),0,0);
		}
		if (Find_Building_By_Type(1,BuildingConstants::TYPE_POWER_PLANT))
		{
			//fprintf(f,"Enable GDI Power\n");
			Commands->Send_Custom_Event(obj,Find_Building_By_Type(1,BuildingConstants::TYPE_POWER_PLANT),Get_Int_Parameter("Enable_Custom"),0,0);
		}
		SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();
		//fprintf(f,"Enable Base Defenses\n");
		while (x)
		{
			VehicleGameObj *o = x->Data();
			if (o && o->Get_Definition().Get_Encyclopedia_Type() == 3)
			{
				Commands->Send_Custom_Event(obj,0,CUSTOM_AI_ENABLEAI,0,0);
			}
			x = x->Next();
		}
		Create_2D_Sound_Team(Get_Parameter("End_Announcement_Sound_Nod"),0);
		Create_2D_Sound_Team(Get_Parameter("End_Announcement_Sound_GDI"),1);
		const char *string = Get_Parameter("End_Announcement_String");
		int red = Get_Int_Parameter("Red");
		int green = Get_Int_Parameter("Green");
		int blue = Get_Int_Parameter("Blue");
		//fprintf(f,"Send Ion Storm ending message\n");
		Send_Message(red,green,blue,string);
		float min = Get_Float_Parameter("Min_Delay");
		float max = Get_Float_Parameter("Max_Delay");
		if (max > The_Game()->Get_Time_Remaining_Seconds())
		{
			max = The_Game()->Get_Time_Remaining_Seconds();
		}
		if (min > The_Game()->Get_Time_Remaining_Seconds())
		{
			Destroy_Script();
			//fclose(f);
			return;
		}
		if (min > max)
		{
			Destroy_Script();
			//fclose(f);
			return;
		}
		float timer = Commands->Get_Random(min,max);
		//fprintf(f,"Ion Storm timer restarting, max delay is %f, min delay is %f, actual delay is %f\n",min,max,timer);
		Commands->Start_Timer(obj,this,timer,1);
		//fclose(f);
	}
	else if (number == 3)
	{
		if (storm)
		{
			Commands->Create_2D_Sound(Get_Parameter("Ion_Effect_Sound"));
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Ion_Effect_Time"),3);
		}
	}
}

void JFW_Ion_Storm_2::Created(GameObject *obj)
{
	DisableEmp = false;
	int chance = Commands->Get_Random_Int(0,100);
	//FILE *f = fopen("ion.log","wt");
	//fprintf(f,"Ion Storm script initializing, random chance chosen is %d, IonChance is %d\n",chance,Get_Int_Parameter("IonChance"));
	//fclose(f);
	if (chance <= Get_Int_Parameter("IonChance"))
	{
		//FILE *f = fopen("ion.log","at");
		//fprintf(f,"Ion Storm random chance too low, no storms this game\n");
		//fclose(f);
		return;
	}
	storm = false;
	float min = Get_Float_Parameter("Min_Delay");
	float max = Get_Float_Parameter("Max_Delay");
	if (max > The_Game()->Get_Time_Remaining_Seconds())
	{
		max = The_Game()->Get_Time_Remaining_Seconds();
	}
	if (min > The_Game()->Get_Time_Remaining_Seconds())
	{
		Destroy_Script();
		return;
	}
	if (min > max)
	{
		Destroy_Script();
		return;
	}
	float timer = Commands->Get_Random(min,max);
	//FILE *f2 = fopen("ion.log","at");
	//fprintf(f2,"Ion Storm timer starting, max delay is %f, min delay is %f, actual delay is %f\n",min,max,timer);
	//fclose(f2);
	Commands->Start_Timer(obj,this,timer,1);
}
void JFW_Ion_Storm_2::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1) //start storm
	{
		//FILE *f = fopen("ion.log","at");
		//fprintf(f,"Ion Storm starting\n");
		storm = true;
		float min = Get_Float_Parameter("Min_Time");
		float max = Get_Float_Parameter("Max_Time");
		if (max > The_Game()->Get_Time_Remaining_Seconds())
		{
			max = The_Game()->Get_Time_Remaining_Seconds();
		}
		if (min > The_Game()->Get_Time_Remaining_Seconds())
		{
			Destroy_Script();
			//fclose(f);
			return;
		}
		if (min > max)
		{
			Destroy_Script();
			//fclose(f);
			return;
		}
		if (Get_Int_Parameter("DisableEmp"))
		{
			//fprintf(f,"Disable EMP\n");
			DisableEmp = true;
		}
		if (Get_Int_Parameter("DisableStealth"))
		{
			//fprintf(f,"Disable Stealth\n");
			Set_Global_Stealth_Disable(true);
		}
		float timer = Commands->Get_Random(min,max);
		//fprintf(f,"Ion Storm run starting, max time is %f, min time is %f, actual time is %f\n",min,max,timer);
		Commands->Start_Timer(obj,this,timer,2);
		//fprintf(f,"Enable Ion Storm weather\n");
		Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("On_Weather_Custom"),0,0);
		if (Get_Int_Parameter("DisableRadar"))
		{
			if (Find_Building_By_Type(0,BuildingConstants::TYPE_COM_CENTER))
			{
				//fprintf(f,"Disable Nod Radar\n");
				Commands->Send_Custom_Event(obj,Find_Building_By_Type(0,BuildingConstants::TYPE_COM_CENTER),Get_Int_Parameter("Disable_Custom"),0,0);
			}
			if (Find_Building_By_Type(1,BuildingConstants::TYPE_COM_CENTER))
			{
				//fprintf(f,"Disable GDI Radar\n");
				Commands->Send_Custom_Event(obj,Find_Building_By_Type(1,BuildingConstants::TYPE_COM_CENTER),Get_Int_Parameter("Disable_Custom"),0,0);
			}
		}
		if (Get_Int_Parameter("DisablePower"))
		{
			if (Find_Building_By_Type(0,BuildingConstants::TYPE_POWER_PLANT))
			{
				//fprintf(f,"Disable Nod Power\n");
				Commands->Send_Custom_Event(obj,Find_Building_By_Type(0,BuildingConstants::TYPE_POWER_PLANT),Get_Int_Parameter("Disable_Custom"),0,0);
			}
			if (Find_Building_By_Type(1,BuildingConstants::TYPE_POWER_PLANT))
			{
				//fprintf(f,"Disable GDI Power\n");
				Commands->Send_Custom_Event(obj,Find_Building_By_Type(1,BuildingConstants::TYPE_POWER_PLANT),Get_Int_Parameter("Disable_Custom"),0,0);
			}
		}
		//fprintf(f,"Disable Base Defenses\n");
		SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();
		while (x)
		{
			VehicleGameObj *o = x->Data();
			if (Get_Int_Parameter("DisableBaseDefenses"))
			{
				if (o && o->Get_Definition().Get_Encyclopedia_Type() == 3)
				{
					Commands->Send_Custom_Event(obj,0,CUSTOM_AI_DISABLEAI,0,0);
				}
			}
			if (Is_Script_Attached(obj,"JFW_EMP_Mine") && Get_Int_Parameter("DestroyMines"))
			{
				Commands->Destroy_Object(obj);
			}
			x = x->Next();
		}
		Timer_Expired(obj,3);
		Create_2D_Sound_Team(Get_Parameter("Announcement_Sound_Nod"),0);
		Create_2D_Sound_Team(Get_Parameter("Announcement_Sound_GDI"),1);
		const char *string = Get_Parameter("Announcement_String");
		int red = Get_Int_Parameter("Red");
		int green = Get_Int_Parameter("Green");
		int blue = Get_Int_Parameter("Blue");
		//fprintf(f,"Send Ion Storm starting message\n");
		Send_Message(red,green,blue,string);
		//fclose(f);
	}
	else if (number == 2) //storm ended
	{
		//FILE *f = fopen("ion.log","at");
		//fprintf(f,"Ion Storm stopping\n");
		storm = false;
		//fprintf(f,"Enable Stealth\n");
		Set_Global_Stealth_Disable(false);
		if (Get_Int_Parameter("DisableEmp"))
		{
			//fprintf(f,"Enable EMP\n");
			DisableEmp = false;
		}
		//fprintf(f,"Disable Ion Storm Weather\n");
		Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("Off_Weather_Custom"),0,0);
		if (Find_Building_By_Type(0,BuildingConstants::TYPE_COM_CENTER))
		{
			//fprintf(f,"Enable Nod Radar\n");
			Commands->Send_Custom_Event(obj,Find_Building_By_Type(0,BuildingConstants::TYPE_COM_CENTER),Get_Int_Parameter("Enable_Custom"),0,0);
		}
		if (Find_Building_By_Type(1,BuildingConstants::TYPE_COM_CENTER))
		{
			//fprintf(f,"Enable GDI Radar\n");
			Commands->Send_Custom_Event(obj,Find_Building_By_Type(1,BuildingConstants::TYPE_COM_CENTER),Get_Int_Parameter("Enable_Custom"),0,0);
		}
		if (Find_Building_By_Type(0,BuildingConstants::TYPE_POWER_PLANT))
		{
			//fprintf(f,"Enable Nod Power\n");
			Commands->Send_Custom_Event(obj,Find_Building_By_Type(0,BuildingConstants::TYPE_POWER_PLANT),Get_Int_Parameter("Enable_Custom"),0,0);
		}
		if (Find_Building_By_Type(1,BuildingConstants::TYPE_POWER_PLANT))
		{
			//fprintf(f,"Enable GDI Power\n");
			Commands->Send_Custom_Event(obj,Find_Building_By_Type(1,BuildingConstants::TYPE_POWER_PLANT),Get_Int_Parameter("Enable_Custom"),0,0);
		}
		SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();
		//fprintf(f,"Enable Base Defenses\n");
		while (x)
		{
			VehicleGameObj *o = x->Data();
			if (o && o->Get_Definition().Get_Encyclopedia_Type() == 3)
			{
				Commands->Send_Custom_Event(obj,0,CUSTOM_AI_ENABLEAI,0,0);
			}
			x = x->Next();
		}
		Create_2D_Sound_Team(Get_Parameter("End_Announcement_Sound_Nod"),0);
		Create_2D_Sound_Team(Get_Parameter("End_Announcement_Sound_GDI"),1);
		const char *string = Get_Parameter("End_Announcement_String");
		int red = Get_Int_Parameter("Red");
		int green = Get_Int_Parameter("Green");
		int blue = Get_Int_Parameter("Blue");
		//fprintf(f,"Send Ion Storm ending message\n");
		Send_Message(red,green,blue,string);
		float min = Get_Float_Parameter("Min_Delay");
		float max = Get_Float_Parameter("Max_Delay");
		if (max > The_Game()->Get_Time_Remaining_Seconds())
		{
			max = The_Game()->Get_Time_Remaining_Seconds();
		}
		if (min > The_Game()->Get_Time_Remaining_Seconds())
		{
			Destroy_Script();
			//fclose(f);
			return;
		}
		if (min > max)
		{
			Destroy_Script();
			//fclose(f);
			return;
		}
		float timer = Commands->Get_Random(min,max);
		//fprintf(f,"Ion Storm timer restarting, max delay is %f, min delay is %f, actual delay is %f\n",min,max,timer);
		Commands->Start_Timer(obj,this,timer,1);
		//fclose(f);
	}
	else if (number == 3)
	{
		if (storm)
		{
			Commands->Create_2D_Sound(Get_Parameter("Ion_Effect_Sound"));
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Ion_Effect_Time"),3);
		}
	}
}

void JFW_Ion_Storm_3::Created(GameObject *obj)
{
	DisableEmp = false;
	if (Get_Int_Parameter("DisableEmp"))
	{
		DisableEmp = true;
	}
	if (Get_Int_Parameter("DisableStealth"))
	{
		Set_Global_Stealth_Disable(true);
	}
	Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("Weather_Custom"),0,0);
	if (Get_Int_Parameter("DisableRadar"))
	{
		if (Find_Building_By_Type(0,BuildingConstants::TYPE_COM_CENTER))
		{
			Commands->Send_Custom_Event(obj,Find_Building_By_Type(0,BuildingConstants::TYPE_COM_CENTER),Get_Int_Parameter("Disable_Custom"),0,0);
		}
		if (Find_Building_By_Type(1,BuildingConstants::TYPE_COM_CENTER))
		{
			Commands->Send_Custom_Event(obj,Find_Building_By_Type(1,BuildingConstants::TYPE_COM_CENTER),Get_Int_Parameter("Disable_Custom"),0,0);
		}
	}
	if (Get_Int_Parameter("DisablePower"))
	{
		if (Find_Building_By_Type(0,BuildingConstants::TYPE_POWER_PLANT))
		{
			Commands->Send_Custom_Event(obj,Find_Building_By_Type(0,BuildingConstants::TYPE_POWER_PLANT),Get_Int_Parameter("Disable_Custom"),0,0);
		}
		if (Find_Building_By_Type(1,BuildingConstants::TYPE_POWER_PLANT))
		{
			Commands->Send_Custom_Event(obj,Find_Building_By_Type(1,BuildingConstants::TYPE_POWER_PLANT),Get_Int_Parameter("Disable_Custom"),0,0);
		}
	}
	SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();
	while (x)
	{
		VehicleGameObj *o = x->Data();
		if (Get_Int_Parameter("DisableBaseDefenses"))
		{
			if (o && o->Get_Definition().Get_Encyclopedia_Type() == 3)
			{
				Commands->Send_Custom_Event(obj,0,CUSTOM_AI_DISABLEAI,0,0);
			}
		}
		if (Is_Script_Attached(obj,"JFW_EMP_Mine") && Get_Int_Parameter("DestroyMines"))
		{
			Commands->Destroy_Object(obj);
		}
		x = x->Next();
	}
	Timer_Expired(obj,3);
	Create_2D_Sound_Team(Get_Parameter("Announcement_Sound_Nod"),0);
	Create_2D_Sound_Team(Get_Parameter("Announcement_Sound_GDI"),1);
	const char *string = Get_Parameter("Announcement_String");
	int red = Get_Int_Parameter("Red");
	int green = Get_Int_Parameter("Green");
	int blue = Get_Int_Parameter("Blue");
	Send_Message(red,green,blue,string);
}
void JFW_Ion_Storm_3::Timer_Expired(GameObject *obj,int number)
{
	if (number == 3)
	{
		Commands->Create_2D_Sound(Get_Parameter("Ion_Effect_Sound"));
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Ion_Effect_Time"),3);
	}
}

void JFW_Ion_Storm_Weather::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Commands->Set_Lightning(Get_Float_Parameter("Lightning_Intensity"),Get_Float_Parameter("Lightning_Start_Distance"),Get_Float_Parameter("Lightning_End_Distance"),Get_Float_Parameter("Lightning_Heading"),Get_Float_Parameter("Lightning_Distribution"),0);
		Commands->Set_Clouds(Get_Float_Parameter("Cloud_Cover"),Get_Float_Parameter("Cloud_Gloominess"),0);
		Commands->Set_Screen_Fade_Color(Get_Float_Parameter("Screen_Red"),Get_Float_Parameter("Screen_Green"),Get_Float_Parameter("Screen_Blue"),0);
		Commands->Set_Screen_Fade_Opacity(Get_Float_Parameter("Screen_Opacity"),0);
	}
}

void JFW_Ion_Storm_Weather_2::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == Get_Int_Parameter("Enable_Message"))
	{
		Get_Lightning(Lightning_Intensity, Lightning_Start_Distance, Lightning_End_Distance, Lightning_Heading, Lightning_Distribution);
		Get_Clouds(Cloud_Cover, Cloud_Gloominess);
		Commands->Set_Lightning(Get_Float_Parameter("Lightning_Intensity"), Get_Float_Parameter("Lightning_Start_Distance"), Get_Float_Parameter("Lightning_End_Distance"), Get_Float_Parameter("Lightning_Heading"), Get_Float_Parameter("Lightning_Distribution"), 0);
		Commands->Set_Clouds(Get_Float_Parameter("Cloud_Cover"), Get_Float_Parameter("Cloud_Gloominess"), 0);
		Commands->Set_Screen_Fade_Color(Get_Float_Parameter("Screen_Red"), Get_Float_Parameter("Screen_Green"), Get_Float_Parameter("Screen_Blue"), 0);
		Commands->Set_Screen_Fade_Opacity(Get_Float_Parameter("Screen_Opacity"), 0);
	}
	else if (type == Get_Int_Parameter("Disable_Message"))
	{
		Commands->Set_Lightning(Lightning_Intensity, Lightning_Start_Distance, Lightning_End_Distance, Lightning_Heading, Lightning_Distribution, 0);
		Commands->Set_Clouds(Cloud_Cover, Cloud_Gloominess, 0);
		Commands->Set_Screen_Fade_Color(0, 0, 0, 0);
		Commands->Set_Screen_Fade_Opacity(0, 0);
	}
}

void JFW_Tech_Level_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Set_Tech_Level(Get_Int_Parameter("Tech_Level"));
		Destroy_Script();
	}
}

void JFW_Tech_Level_Startup::Created(GameObject *obj)
{
	Set_Tech_Level(Get_Int_Parameter("Tech_Level"));
	Destroy_Script();
}

void JFW_Tech_Level_Timer::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
}
void JFW_Tech_Level_Timer::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Create_2D_Sound(Get_Parameter("Sound"));
		Send_Message(Get_Int_Parameter("Red"),Get_Int_Parameter("Green"),Get_Int_Parameter("Blue"),Get_Parameter("Display_Message"));
		Set_Tech_Level(Get_Int_Parameter("Tech_Level"));
		Destroy_Script();
	}
}

void JFW_Change_Character_Created::Created(GameObject *obj)
{
	Change_Character(obj,Get_Parameter("Character"));
	Destroy_Script();
}

void JFW_Spawn_Object_Created::Created(GameObject *obj)
{
	Commands->Create_Object(Get_Parameter("Object"),Commands->Get_Position(obj));
	Destroy_Script();
}

void JFW_Change_Model_Created::Created(GameObject *obj)
{
	const char *model = 0;
	do
	{
		int rand = Commands->Get_Random_Int(0,6);
		if (rand == 1)
		{
			model = Get_Parameter("Model1");
		}
		if (rand == 2)
		{
			model = Get_Parameter("Model2");
		}
		if (rand == 3)
		{
			model = Get_Parameter("Model3");
		}
		if (rand == 4)
		{
			model = Get_Parameter("Model4");
		}
		if (rand == 5)
		{
			model = Get_Parameter("Model5");
		}
	} while (!model || !model[0]);
	Commands->Set_Model(obj,model);
	Destroy_Script();
}

extern REF_DECL(Vector3, NodHouseColor);
extern REF_DECL(Vector3, GDIHouseColor);
extern REF_DECL(Vector3, PublicMessageColor);
void JFW_Killed_String_Sound::Killed(GameObject *obj,GameObject *killer)
{
	if (Get_Object_Type(obj) == 0)
	{
		int red = (int)(NodHouseColor.X * 255);
		int green = (int)(NodHouseColor.Y * 255);
		int blue = (int)(NodHouseColor.Z * 255);
		int stringgdi = Get_Int_Parameter("Nod_String_GDI");
		if (stringgdi && Is_Valid_String_ID(stringgdi))
		{
			const char *str = Get_Translated_String(stringgdi);
			Send_Message_Team(1,red,green,blue,str);
			delete[] str;
			int soundId = Get_String_Sound_ID(stringgdi);
			if (soundId && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000)
			{
				Create_2D_Sound_Team(Get_Definition_Name(soundId),1);
			}
		}
		int stringnod = Get_Int_Parameter("Nod_String_Nod");
		if (stringnod && Is_Valid_String_ID(stringnod))
		{
			const char *str = Get_Translated_String(stringnod);
			Send_Message_Team(0,red,green,blue,str);
			delete[] str;
			int soundId = Get_String_Sound_ID(stringnod);
			if (soundId && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000)
			{
				Create_2D_Sound_Team(Get_Definition_Name(soundId),0);
			}
		}
	}
	else if (Get_Object_Type(obj) == 1)
	{
		int red = (int)(GDIHouseColor.X * 255);
		int green = (int)(GDIHouseColor.Y * 255);
		int blue = (int)(GDIHouseColor.Z * 255);
		int stringgdi = Get_Int_Parameter("GDI_String_GDI");
		if (stringgdi && Is_Valid_String_ID(stringgdi))
		{
			const char *str = Get_Translated_String(stringgdi);
			Send_Message_Team(1,red,green,blue,str);
			delete[] str;
			int soundId = Get_String_Sound_ID(stringgdi);
			if (soundId && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000)
			{
				Create_2D_Sound_Team(Get_Definition_Name(soundId),1);
			}
		}
		int stringnod = Get_Int_Parameter("GDI_String_Nod");
		if (stringnod && Is_Valid_String_ID(stringnod))
		{
			const char *str = Get_Translated_String(stringnod);
			Send_Message_Team(0,red,green,blue,str);
			delete[] str;
			int soundId = Get_String_Sound_ID(stringnod);
			if (soundId && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000)
			{
				Create_2D_Sound_Team(Get_Definition_Name(soundId),0);
			}
		}
	}
	else
	{
		int red = (int)(PublicMessageColor.X * 255);
		int green = (int)(PublicMessageColor.Y * 255);
		int blue = (int)(PublicMessageColor.Z * 255);
		int stringgdi = Get_Int_Parameter("Neutral_String_GDI");
		if (stringgdi && Is_Valid_String_ID(stringgdi))
		{
			const char *str = Get_Translated_String(stringgdi);
			Send_Message_Team(1,red,green,blue,str);
			delete[] str;
			int soundId = Get_String_Sound_ID(stringgdi);
			if (soundId && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000)
			{
				Create_2D_Sound_Team(Get_Definition_Name(soundId),1);
			}
		}
		int stringnod = Get_Int_Parameter("Neutral_String_Nod");
		if (stringnod && Is_Valid_String_ID(stringnod))
		{
			const char *str = Get_Translated_String(stringnod);
			Send_Message_Team(0,red,green,blue,str);
			delete[] str;
			int soundId = Get_String_Sound_ID(stringnod);
			if (soundId && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000)
			{
				Create_2D_Sound_Team(Get_Definition_Name(soundId),0);
			}
		}
	}
}

void JFW_Custom_Create_Object_At_Bone::Created(GameObject *obj)
{
	id = 0;
}

void JFW_Custom_Create_Object_At_Bone::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message") && !id)
	{
		if (obj->As_SoldierGameObj() && obj->As_SoldierGameObj()->Is_In_Vehicle())
		{
			return;
		}
		GameObject *o = Commands->Create_Object_At_Bone(obj,Get_Parameter("Object"),Get_Parameter("Bone"));
		Commands->Attach_To_Object_Bone(o,obj,Get_Parameter("Bone"));
		Commands->Set_Player_Type(o,Commands->Get_Player_Type(obj));
		id = Commands->Get_ID(o);
		float Time = Get_Float_Parameter("Time");
		if (Time)
		{
			Commands->Start_Timer(obj,this,Time,1);
		}
	}
}

void JFW_Custom_Create_Object_At_Bone::Killed(GameObject *obj,GameObject *killer)
{
	if (id)
	{
		Commands->Destroy_Object(Commands->Find_Object(id));
		id = 0;
	}
}

void JFW_Custom_Create_Object_At_Bone::Destroyed(GameObject *obj)
{
	if (id)
	{
		if(Commands->Find_Object(id))
			Commands->Destroy_Object(Commands->Find_Object(id));
	}
}

void JFW_Custom_Create_Object_At_Bone::Timer_Expired(GameObject *obj,int number)
{
	if (id)
	{
		Commands->Destroy_Object(Commands->Find_Object(id));
		id = 0;
	}
}

void JFW_MSA::Created(GameObject *obj)
{
	sizeID = 0;
	deploy = false;
	float Time = Get_Float_Parameter("Time");
	Commands->Start_Timer(obj,this,Time,1);
	Commands->Start_Timer(obj,this,1,2);
}

void JFW_MSA::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_EXITED && deploy && obj->As_VehicleGameObj()->Get_Occupant_Count() == 0)
	{
		Update_Network_Object(obj);
		Commands->Set_Player_Type(obj,Commands->Get_Player_Type(sender));
	}
}

void JFW_MSA::Timer_Expired(GameObject *obj,int number)
{
	if (number == 2)
	{
		Commands->Start_Timer(obj,this,1,2);
		if (obj->As_VehicleGameObj()->Is_Immovable())
		{
			if (!deploy)
			{
				deploy = true;
				obj->As_VehicleGameObj()->Set_Lock_Team(obj->As_VehicleGameObj()->Get_Player_Type());
				if (!sizeID)
				{
					Vector3 position = Commands->Get_Bone_Position(obj, "ROOTTRANSFORM");
					GameObject *object = Commands->Create_Object(Get_Parameter("SizePreset"), position);
					Commands->Set_Player_Type(object,Commands->Get_Player_Type(obj));
					Commands->Attach_To_Object_Bone(object, obj, "ROOTTRANSFORM");
					sizeID = Commands->Get_ID(object);
				}
			}
		}
		else
		{
			if (deploy)
			{
				deploy = false;
				obj->As_VehicleGameObj()->Set_Lock_Team(2);
				if (sizeID)
				{
					Commands->Destroy_Object(Commands->Find_Object(sizeID));
					sizeID = 0;
				}
			}
		}
	}
	if (number == 1 && deploy)
	{
		Vector3 v,v2;
		obj->Get_Position(&v);
		SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();
		while (x)
		{
			VehicleGameObj *o = x->Data();
			if (o)
			{
				if (o->Is_Underground() || o->Is_Stealthed())
				{
					if (!o->Is_Teammate(obj->As_DamageableGameObj()))
					{
						o->Get_Position(&v2);
						if (Commands->Get_Distance(v,v2) <= Get_Float_Parameter("Range"))
						{
							Commands->Send_Custom_Event(obj,o,Get_Int_Parameter("Message"),0,0);
						}
					}
				}
			}
			x = x->Next();
		}
		SLNode<SoldierGameObj> *x2 = GameObjManager::SoldierGameObjList.Head();
		while (x2)
		{
			SoldierGameObj *o = x2->Data();
			if (o)
			{
				if (o->Is_Stealthed())
				{
					if (!o->Is_Teammate(obj->As_DamageableGameObj()))
					{
						o->Get_Position(&v2);
						if (Commands->Get_Distance(v,v2) <= Get_Float_Parameter("Range"))
						{
							Commands->Send_Custom_Event(obj,o,Get_Int_Parameter("Message"),0,0);
						}
					}
				}
			}
			x2 = x2->Next();
		}
	}
	float Time = Get_Float_Parameter("Time");
	Commands->Start_Timer(obj,this,Time,1);
}

void JFW_MSA::Destroyed(GameObject *obj)
{
	if (sizeID)
	{
		Commands->Destroy_Object(Commands->Find_Object(sizeID));
		sizeID = 0;
	}
}

void JFW_Ion_Lightning::Created(GameObject *obj)
{
	storm = false;
}

void JFW_Ion_Lightning::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("OnMessage"))
	{
		float time = Commands->Get_Random(Get_Float_Parameter("MinTime"),Get_Float_Parameter("MaxTime"));
		Commands->Start_Timer(obj,this,time,1);
		storm = true;
	}
	if (type == Get_Int_Parameter("OffMessage"))
	{
		storm = false;
	}
}

void JFW_Ion_Lightning::Timer_Expired(GameObject *obj,int number)
{
	if (storm && number == 1)
	{
		Vector3 min,max;
		PhysicsSceneClass::Get_Instance()->Get_Level_Extents(min,max);
		CastResultStruct res;
		max.Z += 1;
		min.Z -= 1;
		res.ComputeContactPoint = true;
		float x = FreeRandom.Get_Float(min.X,max.X);
		float y = FreeRandom.Get_Float(min.Y,max.Y);
		Vector3 v(x,y,max.Z);
		Vector3 v2(x,y,min.Z);
		LineSegClass ray(v,v2);
		PhysRayCollisionTestClass coltest(ray, &res, DEFAULT_COLLISION_GROUP);
		PhysicsSceneClass::Get_Instance()->Cast_Ray(coltest,false);
		if (coltest.CollidedRenderObj)
		{
			if (Vector3::Dot_Product(res.Normal,ray.Get_Dir()) <= 0)
			{
				v2.Z = res.ContactPoint.Z;
				const AmmoDefinitionClass *def = WeaponManager::Find_Ammo_Definition(Get_Parameter("Ammo"));
				Create_Lightning(def,v,v2);
				Commands->Create_Sound(Get_Definition_Name(def->FireSoundDefID),v2,obj);
				v2.Z += 0.1f;
				Commands->Create_Explosion(Get_Definition_Name(def->ExplosionDefID),v2,0);
				if (coltest.CollidedPhysObj)
				{
					CombatPhysObserverClass * old_observer = (CombatPhysObserverClass *)coltest.CollidedPhysObj->Get_Observer();
					if (old_observer && old_observer->As_BuildingGameObj())
					{
						Commands->Apply_Damage(old_observer->As_BuildingGameObj(),def->Damage,ArmorWarheadManager::Get_Warhead_Name(def->Warhead),0);
					}
				}
			}
		}
		float time = Commands->Get_Random(Get_Float_Parameter("MinTime"),Get_Float_Parameter("MaxTime"));
		Commands->Start_Timer(obj,this,time,1);
	}
}

void JFW_Vehicle_Zone::Created(GameObject *obj)
{
	Vector3 position = Commands->Get_Bone_Position(obj,Get_Parameter("BoneName"));
	Vector3 size = Get_Vector3_Parameter("ZoneSize");
	Matrix3 rotation(true);
	rotation.Rotate_Z(DEG2RAD(Commands->Get_Facing(obj)));
	OBBoxClass box(position,size,rotation);
	GameObject *zone = Create_Zone(Get_Parameter("ZonePreset"),box);
	if (zone)
	{
		zoneID = Commands->Get_ID(zone);
	}
}

void JFW_Vehicle_Zone::Destroyed(GameObject *obj)
{
	if (zoneID)
	{
		Commands->Destroy_Object(Commands->Find_Object(zoneID));
	}
}

void JFW_Building_Zone_Controller::Created(GameObject *obj)
{
	zoneID = 0;
}

void JFW_Building_Zone_Controller::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 0x123456)
	{
		Vector3 position = Commands->Get_Position(sender);
		Vector3 size = Get_Vector3_Parameter("ZoneSize");
		Matrix3 rotation(true);
		rotation.Rotate_Z(DEG2RAD(Commands->Get_Facing(obj)));
		OBBoxClass box(position,size,rotation);
		GameObject *zone = Create_Zone(Get_Parameter("ZonePreset"),box);
		if (zone)
		{
			zoneID = Commands->Get_ID(zone);
		}
	}
}

void JFW_Building_Zone_Controller::Killed(GameObject *obj)
{
	if (zoneID)
	{
		Commands->Destroy_Object(Commands->Find_Object(zoneID));
	}
}

void JFW_Building_Zone::Created(GameObject *obj)
{
	Commands->Send_Custom_Event(obj,Find_Closest_Preset_By_Team(2,Commands->Get_Position(obj),Get_Parameter("ParentPreset")),0x123456,0,0.5f);
}

void JFW_Send_Message_Preset_Death::Killed(GameObject *obj)
{
	GameObject *receiver = Find_Object_By_Preset(2, Get_Parameter("Preset"));
	Commands->Send_Custom_Event(obj, receiver, Get_Int_Parameter("Message"),0,0.0);
}

void JMG_Send_Custom_To_Self_On_Timer::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("Timer_Number"));
}

void JMG_Send_Custom_To_Self_On_Timer::Timer_Expired(GameObject *obj,int number)
{
	if (number == Get_Int_Parameter("Timer_Number"))
	{
		Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("Message"),Get_Int_Parameter("Param"),0);
		if (Get_Int_Parameter("Repeat") == 1)
		{
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("Timer_Number"));
		}
		if (Get_Int_Parameter("Repeat") == -1)
		{
			Remove_Script(obj,"JMG_Send_Custom_To_Self_On_Timer");
		}
	}
}

void JFW_Vehicle_Crate::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Vector3 position = Commands->Get_Position(obj);
		position += Get_Vector3_Parameter("Offset");
		Force_Vehicle_Entry(sender,Commands->Create_Object(Get_Parameter("Preset"), position));
	}
}

void JFW_Airstrike_Cinematic::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Vector3 position = Commands->Get_Position(obj);
		float facing = Commands->Get_Facing(obj);
		const char *scriptname = Get_Parameter("Script_Name");
		GameObject *object = Commands->Create_Object("Invisible_Object", position);
		Commands->Set_Facing(object, facing);
		Commands->Attach_Script(object, "JFW_Cinematic", scriptname);
	}
}

void JFW_Spawner_Delay::Created(GameObject *obj)
{
	Commands->Enable_Spawner(Get_Int_Parameter("ID"),false);
	Commands->Start_Timer(obj,this,Get_Float_Parameter("time"),1);
}

void JFW_Spawner_Delay::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Enable_Spawner(Get_Int_Parameter("ID"),true);
	}
}

void JFW_EMP_Indicator::Created(GameObject *obj)
{
	id = 0;
	Warhead = ArmorWarheadManager::Get_Warhead_Type(Get_Parameter("Warhead"));
}

void JFW_EMP_Indicator::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (Warhead == Get_Damage_Warhead() && !DisableEmp)
	{
		VehicleGameObj *o = obj->As_VehicleGameObj();
		if (o && o->Get_Is_Scripts_Visible() && o->Get_Scripts_Can_Fire() && o->Can_Drive())
		{
			Commands->Enable_Engine(obj,false);
			o->Set_Can_Drive(false);
			o->Set_Scripts_Can_Fire(false);
			o->Set_Stealth_Active(false);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
			GameObject *indicator = Commands->Create_Object_At_Bone(obj,Get_Parameter("Preset"),Get_Parameter("Bone"));
			Commands->Attach_To_Object_Bone(indicator,obj,Get_Parameter("Bone"));
			id = Commands->Get_ID(indicator);
			obj->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
		}
		SoldierGameObj *s = obj->As_SoldierGameObj();
		if (s && !s->Is_Frozen() && s->Is_Visible())
		{
			s->Set_Freeze(true);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
		}
	}
}

void JFW_EMP_Indicator::Timer_Expired(GameObject *obj,int number)
{
	VehicleGameObj *o = obj->As_VehicleGameObj();
	if (o)
	{
		o->Clear_Animation();
		obj->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
		o->Set_Can_Drive(true);
		o->Set_Stealth_Active(true);
		Commands->Enable_Engine(obj,true);
		o->Set_Scripts_Can_Fire(true);
		GameObject *indicator = Commands->Find_Object(id);
		if (indicator)
		{
			Commands->Destroy_Object(indicator);
		}
	}
	SoldierGameObj *s = obj->As_SoldierGameObj();
	if (s)
	{
		s->Set_Freeze(false);
	}
}

ScriptRegistrant<JFW_Tech_Level_Timer> JFW_Tech_Level_Timer_Registrant("JFW_Tech_Level_Timer", "Display_Message:string,Red:int,Blue:int,Green:int,Sound:string,Time:float,Tech_Level:int");
ScriptRegistrant<JFW_Tech_Level_Startup> JFW_Tech_Level_Startup_Registrant("JFW_Tech_Level_Startup","Tech_Level:int");
ScriptRegistrant<JFW_Tech_Level_Custom> JFW_Tech_Level_Custom_Registrant("JFW_Tech_Level_Custom","Message:int,Tech_Level:int");

ScriptRegistrant<JFW_Stealthable_Object> JFW_Stealthable_Object_Registrant("JFW_Stealthable_Object","Message:int");
ScriptRegistrant<JFW_Object_Counter> JFW_Object_Counter_Registrant("JFW_Object_Counter","Count:int,IncrementMessage:int,DecrementMessage:int");
ScriptRegistrant<JFW_Fog_Create> JFW_Fog_Create_Registrant("JFW_Fog_Create","Fog_Enable:int,Fog_Start_Distance=0.000:float,Fog_End_Distance=0.000:float,Delay=0.000:float,OnCreate=0:int,OnDestroy=0:int");
ScriptRegistrant<JFW_War_Blitz_Create> JFW_War_Blitz_Create_Registrant("JFW_War_Blitz_Create","War_Blitz_Intensity=0.000:float,Start_Distance=0.000:float,End_Distance=1.000:float,War_Blitz_Heading=0.000:float,War_Blitz_Distribution=1.000:float,Delay=0.000:float,OnCreate=0:int,OnDestroy=0:int");
ScriptRegistrant<JFW_Goto_Object_On_Startup> JFW_Goto_Object_On_Startup_Registrant("JFW_Goto_Object_On_Startup","ID:int,Speed:float,ArriveDistance:float");
ScriptRegistrant<JFW_Animated_Effect> JFW_Animated_Effect_Registrant("JFW_Animated_Effect","Animation:string,Subobject:string,FirstFrame:float,LastFrame:float,Blended:int,Model:string,Message:int,Location:vector3");
ScriptRegistrant<JFW_Animated_Effect_2> JFW_Animated_Effect_2_Registrant("JFW_Animated_Effect_2","Animation:string,Subobject:string,FirstFrame:float,LastFrame:float,Blended:int,Model:string,Message:int");
ScriptRegistrant<JFW_Random_Animated_Effect> JFW_Random_Animated_Effect_Registrant("JFW_Random_Animated_Effect","Animation:string,Subobject:string,FirstFrame:float,LastFrame:float,Blended:int,Model:string,Message:int,Location:vector3,Offset:vector3");
ScriptRegistrant<JFW_Random_Animated_Effect_2> JFW_Random_Animated_Effect_2_Registrant("JFW_Random_Animated_Effect_2","Animation:string,Subobject:string,FirstFrame:float,LastFrame:float,Blended:int,Model:string,Message:int,Offset:vector3");
ScriptRegistrant<JFW_Object_Draw_In_Order> JFW_Object_Draw_In_Order_Registrant("JFW_Object_Draw_In_Order"," Location:vector3,Custom:int,BaseName:string,Count:int,Facing:float");
ScriptRegistrant<JFW_Object_Draw_In_Order_2> JFW_Object_Draw_In_Order_2_Registrant("JFW_Object_Draw_In_Order_2"," Location:vector3,Custom:int,BaseName:string,Count:int,Facing:float,Start_Number:int");
ScriptRegistrant<JFW_Object_Draw_Random> JFW_Object_Draw_Random_Registrant("JFW_Object_Draw_Random"," Location:vector3,Custom:int,BaseName:string,Count:int,Facing:float");
ScriptRegistrant<JFW_Play_Animation_Destroy_Object> JFW_Play_Animation_Destroy_Object_Registrant("JFW_Play_Animation_Destroy_Object","Animation:string,Subobject:string,FirstFrame:float,LastFrame:float,Blended:int");
ScriptRegistrant<JFW_Model_Animation> JFW_Model_Animation_Registrant("JFW_Model_Animation","");
ScriptRegistrant<JFW_Model_Animation_2> JFW_Model_Animation_2_Registrant("JFW_Model_Animation_2","Animation:string");
ScriptRegistrant<JFW_Debug_Text_File> JFW_Debug_Text_File_Registrant("JFW_Debug_Text_File","Log_File:string,Description:string");
ScriptRegistrant<JFW_Power_Off> JFW_Power_Off_Registrant("JFW_Power_Off","Message_Off:int,Message_On:int");
ScriptRegistrant<JFW_Follow_Waypath> JFW_Follow_Waypath_Registrant("JFW_Follow_Waypath","Waypathid:int,Speed:float");
ScriptRegistrant<JFW_User_Settable_Parameters> JFW_User_Settable_Parameters_Registrant("JFW_User_Settable_Parameters","File_Name:string,Script_Name:string");
ScriptRegistrant<JFW_Change_Spawn_Character> JFW_Change_Spawn_Character_Registrant("JFW_Change_Spawn_Characher","Player_Type:int,Character:string");
ScriptRegistrant<JFW_HUD_INI> JFW_HUD_INI_Registrant("JFW_HUD_INI","HUDINI:string");
ScriptRegistrant<JFW_Scope> JFW_Scope_Registrant("JFW_Scope","Scope:int");
ScriptRegistrant<JFW_Screen_Fade_On_Enter> JFW_Screen_Fade_On_Enter_Registrant("JFW_Screen_Fade_On_Enter","Red:float,Green:float,Blue:float,Opacity:float");
ScriptRegistrant<JFW_Screen_Fade_On_Exit> JFW_Screen_Fade_On_Exit_Registrant("JFW_Screen_Fade_On_Exit","Red:float,Green:float,Blue:float,Opacity:float");
ScriptRegistrant<JFW_Screen_Fade_On_Custom> JFW_Screen_Fade_On_Custom_Registrant("JFW_Screen_Fade_On_Custom","Message:int,Red:float,Green:float,Blue:float,Opacity:float");
ScriptRegistrant<JFW_Screen_Fade_Custom_Timer> JFW_Screen_Fade_Custom_Timer_Registrant("JFW_Screen_Fade_Custom_Timer","Message:int,Red:float,Blue:float,Green:float,Opacity:float,Time:float,TimerNum:int");
ScriptRegistrant<JFW_BHS_DLL> JFW_BHS_DLL_Registrant("JFW_BHS_DLL","");
ScriptRegistrant<JFW_Show_Info_Texture> JFW_Show_Info_Texture_Registrant("JFW_Show_Info_Texture","Time:float,TimerNum:int,Texture:string");
ScriptRegistrant<JFW_Wireframe_Mode> JFW_Wireframe_Mode_Registrant("JFW_Wireframe_Mode","Mode:int");
ScriptRegistrant<JFW_Change_Radar_Map> JFW_Change_Radar_Map_Registrant("JFW_Change_Radar_Map","Scale:float,OffsetX:float,OffsetY:float,Texture:string");
ScriptRegistrant<JFW_Goto_Player_Timer> JFW_Goto_Player_Timer_Registrant("JFW_Goto_Player_Timer","Time:float,TimerNum:int,Speed:float,ArriveDistance:float");
ScriptRegistrant<JFW_Credit_Grant> JFW_Credit_Grant_Registrant("JFW_Credit_Grant","Credits:int,Delay:float");
ScriptRegistrant<JFW_PT_Hide> JFW_PT_Hide_Registrant("JFW_PT_Hide","Player_Type:int,Preset:string");
ScriptRegistrant<JFW_PT_Hide_Death> JFW_PT_Hide_Death_Registrant("JFW_PT_Hide_Death","Player_Type:int,Preset:string");
ScriptRegistrant<JFW_PT_Hide_Custom> JFW_PT_Hide_Custom_Registrant("JFW_PT_Hide_Custom","Player_Type:int,Preset:string,Message:int");
ScriptRegistrant<JFW_PT_Hide_Timer> JFW_PT_Hide_Timer_Registrant("JFW_PT_Hide_Timer","Player_Type:int,Preset:string,Time:float");
ScriptRegistrant<JFW_PT_Show> JFW_PT_Show_Registrant("JFW_PT_Show","Player_Type:int,Preset:string");
ScriptRegistrant<JFW_PT_Show_Death> JFW_PT_Show_Death_Registrant("JFW_PT_Show_Death","Player_Type:int,Preset:string");
ScriptRegistrant<JFW_PT_Show_Custom> JFW_PT_Show_Custom_Registrant("JFW_PT_Show_Custom","Player_Type:int,Preset:string,Message:int");
ScriptRegistrant<JFW_PT_Show_Timer> JFW_PT_Show_Timer_Registrant("JFW_PT_Show_Timer","Player_Type:int,Preset:string,Time:float");
ScriptRegistrant<JFW_Follow_Waypath_Zone> JFW_Follow_Waypath_Zone_Registrant("JFW_Follow_Waypath_Zone","Waypathid:int,Speed:float,Preset:string");
ScriptRegistrant<JFW_Set_Info_Texture> JFW_Set_Info_Texture_Registrant("JFW_Set_Info_Texture","Message:int,Texture:string");
ScriptRegistrant<JFW_Clear_Info_Texture> JFW_Clear_Info_Texture_Registrant("JFW_Clear_Info_Texture","Message:int");
ScriptRegistrant<JFW_Set_Time_Custom> JFW_Set_Time_Custom_Registrant("JFW_Set_Time_Custom","Message:int,Time:float");
ScriptRegistrant<JFW_Set_Time_Limit_Custom> JFW_Set_Time_Limit_Custom_Registrant("JFW_Set_Time_Limit_Custom","Message:int,TimeLimit:int");
ScriptRegistrant<JFW_Radar_Low_Power> JFW_Radar_Low_Power_Registrant("JFW_Radar_Low_Power","");
ScriptRegistrant<JFW_Game_Hint_Soldier> JFW_Game_Hint_Soldier_Registrant("JFW_Game_Hint_Soldier","EventID:int,Sound:string,TitleID:int,TextID:int,TextID2:int,TextID3:int");
ScriptRegistrant<JFW_Game_Hint_Vehicle> JFW_Game_Hint_Vehicle_Registrant("JFW_Game_Hint_Vehicle","EventID:int,Sound:string,TitleID:int,TextID:int,TextID2:int,TextID3:int");
ScriptRegistrant<JFW_Game_Hint_Zone> JFW_Game_Hint_Zone_Registrant("JFW_Game_Hint_Zone","Player_Type:int,EventID:int,Sound:string,TitleID:int,TextID:int,TextID2:int,TextID3:int");
ScriptRegistrant<JFW_Game_Hint_Image_Soldier> JFW_Game_Hint_Image_Soldier_Registrant("JFW_Game_Hint_Image_Soldier","EventID:int,Sound:string,TitleID:int,TextID:int,TextID2:int,TextID3:int,Texture:string");
ScriptRegistrant<JFW_Game_Hint_Image_Vehicle> JFW_Game_Hint_Image_Vehicle_Registrant("JFW_Game_Hint_Image_Vehicle","EventID:int,Sound:string,TitleID:int,TextID:int,TextID2:int,TextID3:int,Texture:string");
ScriptRegistrant<JFW_Game_Hint_Image_Zone> JFW_Game_Hint_Image_Zone_Registrant("JFW_Game_Hint_Image_Zone","Player_Type:int,EventID:int,Sound:string,TitleID:int,TextID:int,TextID2:int,TextID3:int,Texture:string");
ScriptRegistrant<JFW_Moon_Is_Earth> JFW_Moon_Is_Earth_Registrant("JFW_Moon_Is_Earth","");
ScriptRegistrant<JFW_Points_Custom> JFW_Points_Custom_Registrant("JFW_Points_Custom","Message:int,Points:int,Team:int");
ScriptRegistrant<JFW_Engineer_Hack> JFW_Engineer_Hack_Registrant("JFW_Engineer_Hack","Hack_Warhead:string,Hack_Health:float,Hack_Damage:float,Hack_Repair:float,Hack_Restore_Timer:float,Hack_Restore_Amount:float,Hack_Offline_Amount:float,Hack_Offline_Custom:int,Hack_Online_Custom:int,Hack_Indicator_Preset:string");
ScriptRegistrant<JFW_Double_Animation> JFW_Double_Animation_Registrant("JFW_Double_Animation","End_Frame:float,First_Custom:int,Second_Custom:int");
ScriptRegistrant<JFW_Display_String_Custom> JFW_Display_String_Custom_Registrant("JFW_Display_String_Custom","Message:int,Team:int,String:string,Sound:string,Red:int,Green:int,Blue:int");
ScriptRegistrant<JFW_Preset_Disable> JFW_Preset_Disable_Registrant("JFW_Preset_Disable","Preset:string,Disable_Custom:int,Enable_Custom:int");
ScriptRegistrant<JFW_Power_Disable> JFW_Power_Disable_Registrant("JFW_Power_Disable","Disable_Custom:int,Enable_Custom:int");
ScriptRegistrant<JFW_Radar_Disable> JFW_Radar_Disable_Registrant("JFW_Radar_Disable","Disable_Custom:int,Enable_Custom:int");
ScriptRegistrant<JFW_Conyard_Disable> JFW_Conyard_Disable_Registrant("JFW_Conyard_Disable","Disable_Custom:int,Enable_Custom:int");
ScriptRegistrant<JFW_Refinery_Disable> JFW_Refinery_Disable_Registrant("JFW_Refinery_Disable","Disable_Custom:int,Enable_Custom:int");
ScriptRegistrant<JFW_VehicleFactory_Disable> JFW_VehicleFactory_Disable_Registrant("JFW_VehicleFactory_Disable","Disable_Custom:int,Enable_Custom:int");
ScriptRegistrant<JFW_AirFactory_Disable> JFW_AirFactory_Disable_Registrant("JFW_AirFactory_Disable","Disable_Custom:int,Enable_Custom:int");
ScriptRegistrant<JFW_NavalFactory_Disable> JFW_NavalFactory_Disable_Registrant("JFW_NavalFactory_Disable","Disable_Custom:int,Enable_Custom:int");
ScriptRegistrant<JFW_SoldierFactory_Disable> JFW_SoldierFactory_Disable_Registrant("JFW_SoldierFactory_Disable","Disable_Custom:int,Enable_Custom:int");
ScriptRegistrant<JFW_EMP> JFW_EMP_Registrant("JFW_EMP","Warhead:string,Time:float,Animation:string");
ScriptRegistrant<JFW_EMP_Mine> JFW_EMP_Mine_Registrant("JFW_EMP_Mine","Mine_Manager_ID:int,Explosion:string,Time:float");
ScriptRegistrant<JFW_EMP_Mine_Manager> JFW_EMP_Mine_Manager_Registrant("JFW_EMP_Mine_Manager","Mine_Preset:string,Mine_Limit:int,Mine_Reload:string,Mine_Z_Offset:float,Mine_Distance:float");
ScriptRegistrant<JFW_EMP_Mine_Manager_2> JFW_EMP_Mine_Manager_2_Registrant("JFW_EMP_Mine_Manager_2","Mine_Preset:string,Mine_Limit:int,Mine_Reload:string,Throw_Velocity:float");
ScriptRegistrant<JFW_EMP_Mine_Layer> JFW_EMP_Mine_Layer_Registrant("JFW_EMP_Mine_Layer","Mine_Manager_ID:int,Warhead:string");
ScriptRegistrant<JFW_Warhead_Custom> JFW_Warhead_Custom_Registrant("JFW_Warhead_Custom", "Message:int,Warhead:string");
ScriptRegistrant<JFW_Radar_Disable_Death> JFW_Radar_Disable_Death_Registrant("JFW_Radar_Disable_Death", "");
ScriptRegistrant<JFW_Cyborg_Reaper> JFW_Cyborg_Reaper_Registrant("JFW_Cyborg_Reaper","");
ScriptRegistrant<JFW_Limpet_Drone> JFW_Limpet_Drone_Registrant("JFW_Limpet_Drone","");
ScriptRegistrant<JFW_Forward_Custom_Object> JFW_Forward_Custom_Object_Registrant("JFW_Forward_Custom_Object","Object_ID:int");
ScriptRegistrant<JFW_Death_Send_Custom_Self> JFW_Death_Send_Custom_Self_Registrant("JFW_Death_Send_Custom_Self","Message:int");
ScriptRegistrant<JFW_Hunter_Seeker> JFW_Hunter_Seeker_Registrant("JFW_Hunter_Seeker","Key:string,Explosion:string");
ScriptRegistrant<JFW_Ion_Storm> JFW_Ion_Storm_Registrant("JFW_Ion_Storm","Min_Delay:float,Max_Delay:float,Min_Time:float,Max_Time:float,Disable_Custom:int,Enable_Custom:int,Announcement_Sound_Nod:string,Announcement_Sound_GDI:string,Announcement_String:string,Red:int,Green:int,Blue:int,Ion_Effect_Sound:string,Ion_Effect_Time:float,End_Announcement_Sound_Nod:string,End_Announcement_Sound_GDI:string,End_Announcement_String:string,On_Weather_Custom:int,Off_Weather_Custom:int,DestroyMines:int,DisableEmp:int,IonChance:int");
ScriptRegistrant<JFW_Ion_Storm_2> JFW_Ion_Storm_2_Registrant("JFW_Ion_Storm_2","Min_Delay:float,Max_Delay:float,Min_Time:float,Max_Time:float,Disable_Custom:int,Enable_Custom:int,Announcement_Sound_Nod:string,Announcement_Sound_GDI:string,Announcement_String:string,Red:int,Green:int,Blue:int,Ion_Effect_Sound:string,Ion_Effect_Time:float,End_Announcement_Sound_Nod:string,End_Announcement_Sound_GDI:string,End_Announcement_String:string,On_Weather_Custom:int,Off_Weather_Custom:int,DestroyMines:int,DisableEmp:int,IonChance:int,DisableStealth:int,DisableRadar:int,DisablePower:int,DisableBaseDefenses:int");
ScriptRegistrant<JFW_Ion_Storm_3> JFW_Ion_Storm_3_Registrant("JFW_Ion_Storm_3","Disable_Custom:int,Enable_Custom:int,Announcement_Sound_Nod:string,Announcement_Sound_GDI:string,Announcement_String:string,Red:int,Green:int,Blue:int,Ion_Effect_Sound:string,Ion_Effect_Time:float,Weather_Custom:int,DestroyMines:int,DisableEmp:int,DisableStealth:int,DisableRadar:int,DisablePower:int,DisableBaseDefenses:int");
ScriptRegistrant<JFW_Ion_Storm_Weather> JFW_Ion_Storm_Weather_Registrant("JFW_Ion_Storm_Weather","Lightning_Intensity:float,Lightning_Start_Distance:float,Lightning_End_Distance:float,Lightning_Heading:float,Lightning_Distribution:float,Cloud_Cover:float,Cloud_Gloominess:float,Screen_Red:float,Screen_Green:float,Screen_Blue:float,Screen_Opacity:float,Message:int");
ScriptRegistrant<JFW_Ion_Storm_Weather_2> JFW_Ion_Storm_Weather_2_Registrant("JFW_Ion_Storm_Weather_2", "Lightning_Intensity:float,Lightning_Start_Distance:float,Lightning_End_Distance:float,Lightning_Heading:float,Lightning_Distribution:float,Cloud_Cover:float,Cloud_Gloominess:float,Screen_Red:float,Screen_Green:float,Screen_Blue:float,Screen_Opacity:float,Enable_Message:int,Disable_Message:int");
ScriptRegistrant<JFW_Change_Character_Created> JFW_Change_Character_Created_Registrant("JFW_Change_Character_Created", "Character:string");
ScriptRegistrant<JFW_Change_Model_Created> JFW_Change_Model_Created_Registrant("JFW_Change_Model_Created","Model1:string,Model2:string,Model3:string,Model4:string,Model5:string");
ScriptRegistrant<JFW_Spawn_Object_Created> JFW_Spawn_Object_Created_Registrant("JFW_Spawn_Object_Created","Object:string");
ScriptRegistrant<JFW_Killed_String_Sound> JFW_Killed_String_Sound_Registrant("JFW_Killed_String_Sound","GDI_String_GDI:int,Nod_String_GDI:int,Neutral_String_GDI:int,GDI_String_Nod:int,Nod_String_Nod:int,Neutral_String_Nod:int");
ScriptRegistrant<JFW_Custom_Create_Object_At_Bone> JFW_Custom_Create_Object_At_Bone_Registrant("JFW_Custom_Create_Object_At_Bone","Message:int,Object:string,Bone:string,Time:float");
ScriptRegistrant<JFW_MSA> JFW_MSA_Registrant("JFW_MSA","Time:float,Message:int,Range:float,SizePreset:string");
ScriptRegistrant<JFW_Ion_Lightning> JFW_Ion_Lightning_Registrant("JFW_Ion_Lightning","MinTime:float,MaxTime:float,OnMessage:int,OffMessage:int,Ammo:string");
ScriptRegistrant<JFW_Vehicle_Zone> JFW_Vehicle_Zone_Registrant("JFW_Vehicle_Zone","BoneName:string,ZoneSize:vector3,ZonePreset:string");
ScriptRegistrant<JFW_Building_Zone> JFW_Building_Zone_Registrant("JFW_Building_Zone","ParentPreset:string");
ScriptRegistrant<JFW_Building_Zone_Controller> JFW_Building_Zone_Controller_Registrant("JFW_Building_Zone_Controler","ZoneSize:vector3,ZonePreset:string");
ScriptRegistrant<JFW_Send_Message_Preset_Death> JFW_Send_Message_Preset_Death_Registrant("JFW_Send_Message_Preset_Death","Preset:string,Message:int");
ScriptRegistrant<JMG_Send_Custom_To_Self_On_Timer> JMG_Send_Custom_To_Self_On_Timer_Registrant("JMG_Send_Custom_To_Self_On_Timer","Message:int,Param:int,Time:float,Timer_Number:int,Repeat:int");
ScriptRegistrant<JFW_Vehicle_Crate> JFW_Vehicle_Crate_Registrant("JFW_Vehicle_Crate", "Preset:string,Offset:vector3");
ScriptRegistrant<JFW_Airstrike_Cinematic> JFW_Airstrike_Cinematic_Registrant("JFW_Airstrike_Cinematic", "Script_Name:string,Message:int");
ScriptRegistrant<JFW_Spy_Disguise_Target> JFW_Spy_Disguise_Target_Registrant("JFW_Spy_Disguise_Target", "Model:string,Warhead:string");
ScriptRegistrant<JFW_Spawner_Delay> JFW_Spawner_Delay_Registrant("JFW_Spawner_Delay", "ID:int,time:float");
ScriptRegistrant<JFW_EMP_Indicator> JFW_EMP_Indicator_Registrant("JFW_EMP_Indicator","Warhead:string,Time:float,Preset:string,Bone:string");
