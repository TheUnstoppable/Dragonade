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
#include "jfwsnd.h"
#include "PhysicalGameObj.h"
#include "C4GameObj.h"
#include "WeaponMgr.h"

void JFW_2D_Sound_Timer::Created(GameObject *obj)
{
	float timertime;
	int timernumber;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_2D_Sound_Timer::Timer_Expired(GameObject *obj,int number)
{
	int timernumber;
	float timertime;
	int repeat;
	const char *c;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	repeat = Get_Int_Parameter("Repeat");
	c = Get_Parameter("Sound");
	if (number == timernumber)
	{
		Commands->Create_2D_Sound(c);
		if (repeat == 1)
		{	
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}

void JFW_2D_Sound_Timer_Random::Created(GameObject *obj)
{
	float timertime,timemin,timemax;
	int timernumber;
	timemin = Get_Float_Parameter("Time_Min");
	timemax = Get_Float_Parameter("Time_Max");
	timernumber = Get_Int_Parameter("TimerNum");
	timertime = Commands->Get_Random(timemin,timemax);
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_2D_Sound_Timer_Random::Timer_Expired(GameObject *obj,int number)
{
	float timertime,timemin,timemax;
	int timernumber;
	int repeat;
	const char *c;
	timemin = Get_Float_Parameter("Time_Min");
	timemax = Get_Float_Parameter("Time_Max");
	timernumber = Get_Int_Parameter("TimerNum");
	repeat = Get_Int_Parameter("Repeat");
	c = Get_Parameter("Sound");
	if (number == timernumber)
	{
		Commands->Create_2D_Sound(c);
		if (repeat == 1)
		{	
			timertime = Commands->Get_Random(timemin,timemax);
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}

void JFW_3D_Sound_Timer_Random::Created(GameObject *obj)
{
	float timertime,timemin,timemax;
	int timernumber;
	timemin = Get_Float_Parameter("Time_Min");
	timemax = Get_Float_Parameter("Time_Max");
	timernumber = Get_Int_Parameter("TimerNum");
	timertime = Commands->Get_Random(timemin,timemax);
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_3D_Sound_Timer_Random::Timer_Expired(GameObject *obj,int number)
{
	float timertime,timemin,timemax;
	int timernumber;
	int repeat;
	Vector3 v;
	const char *c;
	timemin = Get_Float_Parameter("Time_Min");
	timemax = Get_Float_Parameter("Time_Max");
	timernumber = Get_Int_Parameter("TimerNum");
	repeat = Get_Int_Parameter("Repeat");
	v = Get_Vector3_Parameter("Position");
	c = Get_Parameter("Sound");
	if (number == timernumber)
	{
		Commands->Create_Sound(c,v,obj);
		if (repeat == 1)
		{	
			timertime = Commands->Get_Random(timemin,timemax);
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}

void JFW_2D_Sound_Timer_Health::Created(GameObject *obj)
{
	float timertime;
	int timernumber;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_2D_Sound_Timer_Health::Timer_Expired(GameObject *obj,int number)
{
	int timernumber;
	float minhealth,maxhealth,health;
	float timertime;
	int repeat;
	const char *c;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	repeat = Get_Int_Parameter("Repeat");
	minhealth = Get_Float_Parameter("Min_Health");
	maxhealth = Get_Float_Parameter("Max_Health");
	c = Get_Parameter("Sound");
	if (number == timernumber)
	{
		health = Commands->Get_Health(obj);
		if ((health >= minhealth) && (health <= maxhealth))
		{
			Commands->Create_2D_Sound(c);
		}
		if (repeat == 1)
		{	
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}

void JFW_2D_Sound_Damage::Created(GameObject *obj)
{
	enabled = true;
}

void JFW_2D_Sound_Damage::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (amount > 0)
	{
		if (Commands->Get_Health(obj) <= Get_Float_Parameter("Health"))
		{
			if (enabled)
			{
				enabled = false;
				Commands->Create_2D_Sound(Get_Parameter("Sound"));
			}
		}
		else
		{
			enabled = true;
		}
	}
}

void JFW_2D_Sound_Damage::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_3D_Sound_Damage::Created(GameObject *obj)
{
	enabled = true;
}

void JFW_3D_Sound_Damage::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (Commands->Get_Health(obj) <= Get_Float_Parameter("Health"))
	{
		if (enabled)
		{
			enabled = false;
			Vector3 pos = Commands->Get_Position(obj);
			Vector3 offset = Get_Vector3_Parameter("Offset");
			pos.X += offset.X;
			pos.Y += offset.Y;
			pos.Z += offset.Z;
			Commands->Create_Sound(Get_Parameter("Sound"),pos,obj);
		}
	}
	else
	{
		enabled = true;
	}
}

void JFW_3D_Sound_Damage::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_2D_Sound_Damage_Range::Created(GameObject *obj)
{
	enabled = true;
}

void JFW_2D_Sound_Damage_Range::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (Commands->Get_Health(obj) <= Get_Float_Parameter("MinHealth"))
	{
		if (enabled)
		{
			enabled = false;
			Commands->Create_2D_Sound(Get_Parameter("Sound"));
		}
	}
	else if (Commands->Get_Health(obj) >= Get_Float_Parameter("MaxHealth"))
	{
		enabled = true;
	}
}

void JFW_2D_Sound_Damage_Range::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_2D_Sound_Damage_Range_Team::Created(GameObject *obj)
{
	enabled = true;
}

void JFW_2D_Sound_Damage_Range_Team::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (Commands->Get_Health(obj) <= Get_Float_Parameter("MinHealth"))
	{
		if (enabled)
		{
			enabled = false;
			Create_2D_Sound_Team(Get_Parameter("Sound"),Get_Object_Type(obj));
		}
	}
	else if (Commands->Get_Health(obj) >= Get_Float_Parameter("MaxHealth"))
	{
		enabled = true;
	}
}

void JFW_2D_Sound_Damage_Range_Team_String::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_2D_Sound_Damage_Range_Team_String::Created(GameObject *obj)
{
	enabled = true;
}

extern REF_DECL(Vector3, NodHouseColor);
extern REF_DECL(Vector3, GDIHouseColor);
void JFW_2D_Sound_Damage_Range_Team_String::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (Commands->Get_Health(obj) <= Get_Float_Parameter("MinHealth"))
	{
		if (enabled)
		{
			enabled = false;
			Create_2D_Sound_Team(Get_Parameter("Sound"),Get_Object_Type(obj));
			int red = 0;
			int blue = 0;
			int green = 0;
			if (Get_Object_Type(obj) == 0)
			{
				red = (int)(NodHouseColor.X * 255);
				green = (int)(NodHouseColor.Y * 255);
				blue = (int)(NodHouseColor.Z * 255);
			}
			else
			{
				red = (int)(GDIHouseColor.X * 255);
				green = (int)(GDIHouseColor.Y * 255);
				blue = (int)(GDIHouseColor.Z * 255);
			}
			Send_Message_Team(Get_Object_Type(obj),red,green,blue,Get_Parameter("String"));
		}
	}
	else if (Commands->Get_Health(obj) >= Get_Float_Parameter("MaxHealth"))
	{
		enabled = true;
	}
}

void JFW_2D_Sound_Damage_Range_Team::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_3D_Sound_Damage_Range::Created(GameObject *obj)
{
	enabled = true;
}

void JFW_3D_Sound_Damage_Range::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (Commands->Get_Health(obj) <= Get_Float_Parameter("MinHealth"))
	{
		if (enabled)
		{
			enabled = false;
			Vector3 pos = Commands->Get_Position(obj);
			Vector3 offset = Get_Vector3_Parameter("Offset");
			pos.X += offset.X;
			pos.Y += offset.Y;
			pos.Z += offset.Z;
			Commands->Create_Sound(Get_Parameter("Sound"),pos,obj);
		}
	}
	else if (Commands->Get_Health(obj) >= Get_Float_Parameter("MaxHealth"))
	{
		enabled = true;
	}
}

void JFW_3D_Sound_Damage_Range::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_3D_Sound_Timer::Created(GameObject *obj)
{
	float timertime;
	int timernumber;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_3D_Sound_Timer::Timer_Expired(GameObject *obj,int number)
{
	int timernumber;
	float timertime;
	int repeat;
	const char *c;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	repeat = Get_Int_Parameter("Repeat");
	c = Get_Parameter("Sound");
	if (number == timernumber)
	{
		Vector3 pos = Commands->Get_Position(obj);
		Vector3 offset = Get_Vector3_Parameter("Offset");
		pos.X += offset.X;
		pos.Y += offset.Y;
		pos.Z += offset.Z;
		Commands->Create_Sound(Get_Parameter("Sound"),pos,obj);
		if (repeat == 1)
		{	
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}

void JFW_Set_Background_Music_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Commands->Set_Background_Music(Get_Parameter("Music"));
	}
}

void JFW_Fade_Background_Music_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Commands->Fade_Background_Music(Get_Parameter("Music"),Get_Int_Parameter("unk1"),Get_Int_Parameter("unk2"));
	}
}

void JFW_Stop_Background_Music_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Commands->Stop_Background_Music();
	}
}

void JFW_Set_Background_Music_On_Enter::Entered(GameObject *obj,GameObject *enterer)
{
	Set_Background_Music_Player(enterer,Get_Parameter("Music"));
}

void JFW_Fade_Background_Music_On_Enter::Entered(GameObject *obj,GameObject *enterer)
{
	Fade_Background_Music_Player(enterer,Get_Parameter("Music"),Get_Int_Parameter("unk1"),Get_Int_Parameter("unk2"));
}

void JFW_Stop_Background_Music_On_Enter::Entered(GameObject *obj,GameObject *enterer)
{
	Stop_Background_Music_Player(enterer);
}

void JFW_2D_Sound_Startup::Created(GameObject *obj)
{
	Commands->Create_2D_Sound(Get_Parameter("Sound"));
	Destroy_Script();
}

void JFW_Time_Remaining_Sounds::Created(GameObject *obj)
{
	if (The_Game()->Get_Time_Remaining_Seconds())
	{
		INIClass *config = Get_INI(Get_Parameter("ConfigFile"));
		if (config)
		{
			SoundCount = config->Get_Int("Sounds","SoundCount",0);
			Sounds = new TimeRemainingSound[SoundCount];
			memset(Sounds,0x00,sizeof(TimeRemainingSound) * SoundCount);
			for (int i = 0;i < SoundCount;i++)
			{
				Sounds[i].sound = new char[50];
				memset(Sounds[i].sound,0x00,50);
				char c[50];
				sprintf(c,"Sound%dName",i);
				config->Get_String("Sounds",c,"",Sounds[i].sound,49);
				sprintf(c,"Sound%dTime",i);
				Sounds[i].minutes = config->Get_Int("Sounds",c,0);
				Sounds[i].played = false;
			}
			Release_INI(config);
			Timer_Expired(obj,1);
		}
	}
}

void JFW_Time_Remaining_Sounds::Timer_Expired(GameObject *obj,int number)
{
	int hours = 0;
	int minutes = 0;
	int seconds = 0;
	Seconds_To_Hms(The_Game()->Get_Time_Remaining_Seconds(), hours, minutes, seconds);
	minutes += hours * 60;
	for (int i = 0; i < SoundCount; i++)
	{
		if (Sounds[i].minutes == minutes && !Sounds[i].played)
		{
			Commands->Create_2D_Sound(Sounds[i].sound);
			Sounds[i].played = true;
		}
	}
	Commands->Start_Timer(obj, this, 1, 1);
}

JFW_Time_Remaining_Sounds::JFW_Time_Remaining_Sounds(): ScriptImpClass()
{
	SoundCount = 0;
	Sounds = 0;
}

JFW_Time_Remaining_Sounds::~JFW_Time_Remaining_Sounds()
{
	if (Sounds)
	{
		for (int i = 0;i < SoundCount;i++)
		{
			SAFE_DELETE_ARRAY(Sounds[i].sound);
		}
		delete[] Sounds;
		Sounds = NULL;
	}
}

void JFW_Time_Remaining_Sounds_2::Created(GameObject *obj)
{
	if (The_Game()->Get_Time_Remaining_Seconds())
	{
		INIClass *config = Get_INI(Get_Parameter("ConfigFile"));
		if (config)
		{
			SoundCount = config->Get_Int("Sounds", "SoundCount", 0);
			Sounds = new TimeRemainingSound[SoundCount];
			memset(Sounds, 0x00, sizeof(TimeRemainingSound) * SoundCount);
			for (int i = 0; i < SoundCount; i++)
			{
				Sounds[i].sound = new char[50];
				memset(Sounds[i].sound, 0x00, 50);
				char c[50];
				sprintf(c, "Sound%dName", i);
				config->Get_String("Sounds", c, "", Sounds[i].sound, 49);
				sprintf(c, "Sound%dTime", i);
				Sounds[i].minutes = config->Get_Int("Sounds", c, 0);
				Sounds[i].played = false;
			}
			Release_INI(config);
			team = Get_Int_Parameter("Team");
			Timer_Expired(obj, 1);
		}
	}
}

void JFW_Time_Remaining_Sounds_2::Timer_Expired(GameObject *obj, int number)
{
	int hours = 0;
	int minutes = 0;
	int seconds = 0;
	Seconds_To_Hms(The_Game()->Get_Time_Remaining_Seconds(), hours, minutes, seconds);
	minutes += hours * 60;
	for (int i = 0; i < SoundCount; i++)
	{
		if (Sounds[i].minutes == minutes && !Sounds[i].played)
		{
			Create_2D_Sound_Team(Sounds[i].sound,team);
			Sounds[i].played = true;
		}
	}
	Commands->Start_Timer(obj, this, 1, 1);
}

JFW_Time_Remaining_Sounds_2::JFW_Time_Remaining_Sounds_2() : ScriptImpClass()
{
	SoundCount = 0;
	Sounds = 0;
}

JFW_Time_Remaining_Sounds_2::~JFW_Time_Remaining_Sounds_2()
{
	if (Sounds)
	{
		for (int i = 0; i < SoundCount; i++)
		{
			SAFE_DELETE_ARRAY(Sounds[i].sound);
		}
		delete [] Sounds;
		Sounds = NULL;
	}
}

void JFW_2D_Sound_Death_Team::Killed(GameObject *obj,GameObject *killer)
{
	Create_2D_Sound_Team(Get_Parameter("Sound"),Get_Object_Type(obj));
	Destroy_Script();
}

void JFW_Vehicle_Full_Sound::Created(GameObject *obj)
{
	full = false;
	float timertime;
	int timernumber;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_Vehicle_Full_Sound::Timer_Expired(GameObject *obj,int number)
{
	int timernumber;
	float timertime;
	const char *c;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	c = Get_Parameter("Sound");
	if (number == timernumber)
	{
		if (obj->As_VehicleGameObj())
		{
			if (Get_Vehicle_Occupant_Count(obj) >= Get_Vehicle_Seat_Count(obj))
			{
				if (!full)
				{
					Create_2D_Sound_Team(c,Get_Object_Type(obj));
					char Msg[100];
					GameObject *d = Get_Vehicle_Driver(obj);
					const char *player;
					if (d)
					{
						player = Get_Player_Name(d);
					}
					else
					{
						player = newstr("No-one");
					}
					const char *v = Get_Translated_Preset_Name(obj);
					sprintf(Msg,"%s's %s is now full",player,v);
					delete[] v;
					delete[] player;
					unsigned int Red = 0,Blue = 0,Green = 0;
					Get_Object_Color(obj,&Red,&Green,&Blue);
					Send_Message_Team(Get_Object_Type(obj),Red,Green,Blue,Msg);
					full = true;
				}
			}
			else
			{
				full = false;
			}
		}
		Commands->Start_Timer(obj,this,timertime,timernumber);
	}
}

void JFW_C4_Sound::Created(GameObject *obj)
{
	PhysicalGameObj *o = ((ScriptableGameObj *)obj)->As_PhysicalGameObj();
	if (!o)
	{
		return;
	}
	C4GameObj *o2 = o->As_C4GameObj();
	if (!o2)
	{
		return;
	}
	float dmg = o2->Get_Ammo_Def()->Damage;
	if (dmg >= 0.0f)
	{
		Commands->Create_2D_Sound(Get_Parameter("Sound"));
	}
	Destroy_Script();
}

void JFW_C4_Sound_2::Created(GameObject *obj)
{
	PhysicalGameObj *o = ((ScriptableGameObj *)obj)->As_PhysicalGameObj();
	if (!o)
	{
		return;
	}
	C4GameObj *o2 = o->As_C4GameObj();
	if (!o2)
	{
		return;
	}
	float dmg = o2->Get_Ammo_Def()->Damage;
	if (dmg > 0.0f)
	{
		Commands->Create_2D_Sound(Get_Parameter("Sound"));
	}
	Destroy_Script();
}

void JFW_Repair_Complete_Sound::Created(GameObject *obj)
{
	damaged = false;
}

void JFW_Repair_Complete_Sound::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (Commands->Get_Health(obj) < Commands->Get_Max_Health(obj) || Commands->Get_Shield_Strength(obj) < Commands->Get_Max_Shield_Strength(obj))
	{
		damaged = true;
	}
	if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj) && Commands->Get_Shield_Strength(obj) == Commands->Get_Max_Shield_Strength(obj) && damaged)
	{
		Create_2D_Sound_Team(Get_Parameter("NodSound"), 0);
		Create_2D_Sound_Team(Get_Parameter("GDISound"), 1);
		damaged = false;
	}
}
ScriptRegistrant<JFW_Set_Background_Music_On_Custom> JFW_Set_Background_Music_On_Custom_Registrant("JFW_Set_Background_Music_On_Custom", "Message:int,Music:string");
ScriptRegistrant<JFW_Fade_Background_Music_On_Custom> JFW_Fade_Background_Music_On_Custom_Registrant("JFW_Fade_Background_Music_On_Custom","Message:int,Music:string,unk1:int,unk2:int");
ScriptRegistrant<JFW_Stop_Background_Music_On_Custom> JFW_Stop_Background_Music_On_Custom_Registrant("JFW_Stop_Background_Music_On_Custom","Message:int");
ScriptRegistrant<JFW_Set_Background_Music_On_Enter> JFW_Set_Background_Music_On_Enter_Registrant("JFW_Set_Background_Music_On_Enter","Music:string");
ScriptRegistrant<JFW_Fade_Background_Music_On_Enter> JFW_Fade_Background_Music_On_Enter_Registrant("JFW_Fade_Background_Music_On_Enter","Music:string,unk1:int,unk2:int");
ScriptRegistrant<JFW_Stop_Background_Music_On_Enter> JFW_Stop_Background_Music_On_Enter_Registrant("JFW_Stop_Background_Music_On_Enter","");
ScriptRegistrant<JFW_2D_Sound_Damage_Range> JFW_2D_Sound_Damage_Range_Registrant("JFW_2D_Sound_Damage_Range","Sound:string,MinHealth:float,MaxHealth:float");
ScriptRegistrant<JFW_2D_Sound_Damage_Range_Team> JFW_2D_Sound_Damage_Range_Team_Registrant("JFW_2D_Sound_Damage_Range_Team","Sound:string,MinHealth:float,MaxHealth:float");
ScriptRegistrant<JFW_2D_Sound_Damage_Range_Team_String> JFW_2D_Sound_Damage_Range_Team_String_Registrant("JFW_2D_Sound_Damage_Range_Team_String","Sound:string,MinHealth:float,MaxHealth:float,String:string");
ScriptRegistrant<JFW_3D_Sound_Damage_Range> JFW_3D_Sound_Damage_Range_Registrant("JFW_3D_Sound_Damage_Range","Sound:string,MinHealth:float,MaxHealth:float,Offset:vector3");
ScriptRegistrant<JFW_3D_Sound_Timer> JFW_3D_Sound_Timer_Registrant("JFW_3D_Sound_Timer","Time:float,TimerNum:int,Repeat:int,Sound:string,Offset:vector3");
ScriptRegistrant<JFW_2D_Sound_Damage> JFW_2D_Sound_Damage_Registrant("JFW_2D_Sound_Damage","Sound:string,Health:float");
ScriptRegistrant<JFW_3D_Sound_Damage> JFW_3D_Sound_Damage_Registrant("JFW_3D_Sound_Damage","Sound:string,Health:float,Offset:vector3");
ScriptRegistrant<JFW_2D_Sound_Timer_Random> JFW_2D_Sound_Timer_Random_Registrant("JFW_2D_Sound_Timer_Random","Time_Min:float,Time_Max:float,TimerNum:int,Repeat:int,Sound:string");
ScriptRegistrant<JFW_3D_Sound_Timer_Random> JFW_3D_Sound_Timer_Random_Registrant("JFW_3D_Sound_Timer_Random","Time_Min:float,Time_Max:float,TimerNum:int,Repeat:int,Sound:string,Position:vector3");
ScriptRegistrant<JFW_2D_Sound_Timer_Health> JFW_2D_Sound_Timer_Health_Registrant("JFW_2D_Sound_Timer_Health","Time:float,TimerNum:int,Repeat:int,Sound:string,Min_Health:float,Max_Health:float");
ScriptRegistrant<JFW_2D_Sound_Timer> JFW_2D_Sound_Timer_Registrant("JFW_2D_Sound_Timer","Time:float,TimerNum:int,Repeat:int,Sound:string");
ScriptRegistrant<JFW_2D_Sound_Startup> JFW_2D_Sound_Startup_Registrant("JFW_2D_Sound_Startup","Sound:string");
ScriptRegistrant<JFW_2D_Sound_Death_Team> JFW_2D_Sound_Death_Team_Registrant("JFW_2D_Sound_Death_Team","Sound:string");
ScriptRegistrant<JFW_Time_Remaining_Sounds> JFW_Time_Remaining_Sounds_Registrant("JFW_Time_Remaining_Sounds","ConfigFile:string");
ScriptRegistrant<JFW_Time_Remaining_Sounds_2> JFW_Time_Remaining_Sounds_2_Registrant("JFW_Time_Remaining_Sounds_2", "ConfigFile:string,Team:int");
ScriptRegistrant<JFW_Vehicle_Full_Sound> JFW_Vehicle_Full_Sound_Registrant("JFW_Vehicle_Full_Sound", "Time:float,TimerNum:int,Sound:string");
ScriptRegistrant<JFW_C4_Sound> JFW_C4_Sound_Registrant("JFW_C4_Sound","Sound:string");
ScriptRegistrant<JFW_C4_Sound_2> JFW_C4_Sound_2_Registrant("JFW_C4_Sound_2","Sound:string");
ScriptRegistrant<JFW_Repair_Complete_Sound> JFW_Repair_Complete_Sound_Registrant("JFW_Repair_Complete_Sound", "GDISound:string,NodSound:string");
