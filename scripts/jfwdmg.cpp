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
#include "jfwdmg.h"
#include "BuildingGameObj.h"
#include "SoldierGameObj.h"
#include "VehicleGameObjDef.h"

void JFW_Spawn_Object_Death::Killed(GameObject *obj,GameObject *killer)
{
	Vector3 spawn_location;
	float facing;
	float f;
	const char *c;
	GameObject *object;
	spawn_location = Commands->Get_Position(obj);
	facing = Commands->Get_Facing(obj);
	c = Get_Parameter("Drop_Object");
	f = Get_Float_Parameter("Drop_Height");
	spawn_location.Z = spawn_location.Z + f;
	object = Commands->Create_Object (c,spawn_location);
	Commands->Set_Facing(object,facing);
	Destroy_Script();
}

void JFW_Timer_Destroy_Building::Created(GameObject *obj)
{
	float timertime;
	int timernumber;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_Timer_Destroy_Building::Timer_Expired(GameObject *obj,int number)
{
	int timernumber;
	timernumber = Get_Int_Parameter("TimerNum");
	if (number == timernumber)
	{
		Commands->Apply_Damage(obj,10000,"Explosive",0);
	}
}

void JFW_Timer_Destroy_Object::Created(GameObject *obj)
{
	float timertime;
	int timernumber;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_Timer_Destroy_Object::Timer_Expired(GameObject *obj,int number)
{
	int timernumber;
	timernumber = Get_Int_Parameter("TimerNum");
	if (number == timernumber)
	{
		Commands->Apply_Damage(obj,Get_Float_Parameter("Amount"),Get_Parameter("Warhead"),0);
	}
}

void JFW_Blow_Up_On_Death::Killed(GameObject *obj,GameObject *killer)
{
	Vector3 v;
	const char *c;
	c = Get_Parameter("Explosion");
	v = Commands->Get_Position(obj);
	Commands->Create_Explosion (c,v,0);
	Destroy_Script();
}

void JFW_Give_Points_On_Death::Killed(GameObject *obj,GameObject *killer)
{
	int pltype;
	pltype = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(killer,pltype))
	{
		return;
	}
	Commands->Give_Points(killer,Get_Float_Parameter("Points"),Get_Bool_Parameter("Whole_Team"));
	Destroy_Script();
}

void JFW_Armour_Regen::Created(GameObject *obj)
{
	float timertime;
	int timernumber;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_Armour_Regen::Timer_Expired(GameObject *obj,int number)
{
	int timernumber;
	float timertime;
	float armour;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	if (number == timernumber)
	{
		armour = Commands->Get_Shield_Strength(obj);
		if (armour > 0)
		{
			armour += Get_Float_Parameter("Points");
		}
		if (armour > Commands->Get_Max_Shield_Strength(obj))
		{
			armour = Commands->Get_Max_Shield_Strength(obj);
		}
		Commands->Set_Shield_Strength(obj,armour);
		Commands->Start_Timer(obj,this,timertime,timernumber);
	}
}

void JFW_Health_Regen::Created(GameObject *obj)
{
	float timertime;
	int timernumber;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_Health_Regen::Timer_Expired(GameObject *obj,int number)
{
	int timernumber;
	float timertime;
	float armour;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	if (number == timernumber)
	{
		armour = Commands->Get_Health(obj);
		if (armour > 0)
		{
			armour += Get_Float_Parameter("Points");
		}
		if (armour > Commands->Get_Max_Health(obj))
		{
			armour = Commands->Get_Max_Health(obj);
		}
		Commands->Set_Health(obj,armour);
		Commands->Start_Timer(obj,this,timertime,timernumber);
	}
}

void JFW_Death_Destroy_Object::Killed(GameObject *obj,GameObject *killer)
{
	Commands->Destroy_Object(Commands->Find_Object(Get_Int_Parameter("ID")));
	Destroy_Script();
}

void JFW_Invulnerability_Timer::Created(GameObject *obj)
{
	enabled = true;
	float timertime;
	int timernumber;
	timertime = Get_Float_Parameter("Invulnerability_Time");
	timernumber = Get_Int_Parameter("Invulnerability_TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
	Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
	Commands->Set_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj));
}

void JFW_Invulnerability_Timer::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (enabled)
	{
		Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
		Commands->Set_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj));
	}
}

void JFW_Invulnerability_Timer::Timer_Expired(GameObject *obj,int number)
{
	if (number == Get_Int_Parameter("Invulnerability_TimerNum"))
	{
		enabled = false;
	}
}

void JFW_Invulnerability_Timer::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Spawn_Object_Death_Enable::Created(GameObject *obj)
{
	enabled = false;
}

void JFW_Spawn_Object_Death_Enable::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("EnableMessage"))
	{
		enabled = true;
	}
	if (type == Get_Int_Parameter("DisableMessage"))
	{
		enabled = false;
	}
}

void JFW_Spawn_Object_Death_Enable::Killed(GameObject *obj,GameObject *killer)
{
	if (enabled)
	{
		Vector3 spawn_location;
		float facing;
		float f;
		const char *c;
		GameObject *object;
		spawn_location = Commands->Get_Position(obj);
		facing = Commands->Get_Facing(obj);
		c = Get_Parameter("Drop_Object");
		f = Get_Float_Parameter("Drop_Height");
		spawn_location.Z = spawn_location.Z + f;
		object = Commands->Create_Object (c,spawn_location);
		Commands->Set_Facing(object,facing);
	}
	Destroy_Script();
}

void JFW_Spawn_Object_Death_Enable::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Spawn_Object_Death_Random::Killed(GameObject *obj,GameObject *killer)
{
	int random = Commands->Get_Random_Int(0,100);
	if (random <= Get_Int_Parameter("Probobility"))
	{
		Vector3 spawn_location;
		float facing;
		float f;
		const char *c;
		GameObject *object;
		spawn_location = Commands->Get_Position(obj);
		facing = Commands->Get_Facing(obj);
		c = Get_Parameter("Drop_Object");
		f = Get_Float_Parameter("Drop_Height");
		spawn_location.Z = spawn_location.Z + f;
		object = Commands->Create_Object (c,spawn_location);
		Commands->Set_Facing(object,facing);
	}
	Destroy_Script();
}

void JFW_Spawn_Object_Death_Enable_Random::Created(GameObject *obj)
{
	enabled = false;
}

void JFW_Spawn_Object_Death_Enable_Random::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("EnableMessage"))
	{
		enabled = true;
	}
	if (type == Get_Int_Parameter("DisableMessage"))
	{
		enabled = false;
	}
}

void JFW_Spawn_Object_Death_Enable_Random::Killed(GameObject *obj,GameObject *killer)
{
	int random = Commands->Get_Random_Int(0,100);
	if (random <= Get_Int_Parameter("Probobility"))
	{
		if (enabled)
		{
			Vector3 spawn_location;
			float facing;
			float f;
			const char *c;
			GameObject *object;
			spawn_location = Commands->Get_Position(obj);
			facing = Commands->Get_Facing(obj);
			c = Get_Parameter("Drop_Object");
			f = Get_Float_Parameter("Drop_Height");
			spawn_location.Z = spawn_location.Z + f;
			object = Commands->Create_Object (c,spawn_location);
			Commands->Set_Facing(object,facing);
		}
	}
	Destroy_Script();
}

void JFW_Spawn_Object_Death_Enable_Random::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Engineer_Target::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&repairid,4,1);
	Auto_Save_Variable(&health,4,2);
	Auto_Save_Variable(&shieldstrength,4,3);
}

void JFW_Engineer_Target::Created(GameObject *obj)
{
	health = Commands->Get_Health(obj);
	shieldstrength = Commands->Get_Shield_Strength(obj);
}

void JFW_Engineer_Target::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 40011)
	{
		repairid = param;
	}
}

void JFW_Engineer_Target::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	Vector3 position;
	position = Commands->Get_Position(obj);
	Commands->Create_Logical_Sound(obj,40012,position,150);
	Commands->Start_Timer(obj,this,3.0,40013);
}

void JFW_Engineer_Target::Destroyed(GameObject *obj)
{
	Vector3 position;
	position = Commands->Get_Position(obj);
	Commands->Create_Logical_Sound(obj,40014,position,150);
}

void JFW_Engineer_Target::Timer_Expired(GameObject *obj,int number)
{
	float currenthealth;
	float currentshieldstrength;
	Vector3 position;
	if (number == 40013)
	{
		currenthealth = Commands->Get_Health(obj);
		currentshieldstrength = Commands->Get_Shield_Strength(obj);
		if ((currenthealth == health) && (currentshieldstrength == shieldstrength))
		{
			position = Commands->Get_Position(obj);
			Commands->Create_Logical_Sound(obj,40014,position,150);
		}
		if ((currenthealth != health) || (currentshieldstrength != shieldstrength))
		{
			position = Commands->Get_Position(obj);
			Commands->Create_Logical_Sound(obj,40012,position,150);
			Commands->Start_Timer(obj,this,3.0,40013);
		}
	}
}

void JFW_Engineer_Repair::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&active,1,1);
	Auto_Save_Variable(&repairid,4,2);
	Auto_Save_Variable(&targetid,4,3);
	Auto_Save_Variable(&priority,4,4);
}

void JFW_Engineer_Repair::Created(GameObject *obj)
{
	active = false;
	priority = Get_Int_Parameter("Repair_Priority");
}

void JFW_Engineer_Repair::Sound_Heard(GameObject *obj,const CombatSound & sound)
{
	ActionParamsStruct params;
	if ((sound.sound == 40012) && (!active) && (Commands->Get_Player_Type(sound.Creator) == Commands->Get_Player_Type(obj)))
	{
		repairid = Commands->Get_ID(obj);
		targetid = Commands->Get_ID(sound.Creator);
		Commands->Send_Custom_Event(obj,sound.Creator,40011,repairid,0);
		active = true;
		params.Set_Basic(this,(float)priority,40015);
		params.Set_Movement(Commands->Get_Position(sound.Creator),(float)0.8,(float)5.0,0);
		Commands->Action_Goto(obj,params);
	}
	else
	{
		if ((sound.sound == 40014) && (targetid == Commands->Get_ID(sound.Creator)))
		{
			Commands->Action_Reset(obj,100);
			active = false;
		}
	}
}

void JFW_Engineer_Repair::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	ActionParamsStruct params;
	if (action_id == 40015)
	{
		params.Set_Basic(this,(float)priority,40016);
		params.Set_Attack(Commands->Find_Object(targetid),50,0,false);
		params.AttackCheckBlocked = false;
		Commands->Action_Attack(obj,params);
	}
}

void JFW_Death_Destroy_Object_Delay::Killed(GameObject *obj,GameObject *killer)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Death_Destroy_Object_Delay::Timer_Expired(GameObject *obj,int number)
{
	Commands->Destroy_Object(Commands->Find_Object(Get_Int_Parameter("ID")));
}

void JFW_Set_Health_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Custom"))
	{
		Commands->Set_Health(obj,Get_Float_Parameter("Value"));
	}
}

void JFW_Add_Health_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Custom"))
	{
		Commands->Set_Health(obj,Commands->Get_Health(obj)+Get_Float_Parameter("Value"));
	}
}

void JFW_Set_Max_Health_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Custom"))
	{
		Set_Max_Health(obj,Get_Float_Parameter("Value"));
	}
}

void JFW_Add_Max_Health_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Custom"))
	{
		Set_Max_Health(obj,Commands->Get_Max_Health(obj)+Get_Float_Parameter("Value"));
	}
}

void JFW_Set_Shield_Strength_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Custom"))
	{
		Commands->Set_Shield_Strength(obj,Get_Float_Parameter("Value"));
	}
}

void JFW_Add_Shield_Strength_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Custom"))
	{
		Commands->Set_Shield_Strength(obj,Commands->Get_Shield_Strength(obj)+Get_Float_Parameter("Value"));
	}
}

void JFW_Set_Max_Shield_Strength_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Custom"))
	{
		Set_Max_Shield_Strength(obj,Get_Float_Parameter("Value"));
	}
}

void JFW_Add_Max_Shield_Strength_On_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Custom"))
	{
		Set_Max_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj)+Get_Float_Parameter("Value"));
	}
}

void JFW_Set_Health_On_Custom_Sender::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Custom"))
	{
		Commands->Set_Health(sender,Get_Float_Parameter("Value"));
	}
}

void JFW_Add_Health_On_Custom_Sender::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Custom"))
	{
		Commands->Set_Health(sender,Commands->Get_Health(sender)+Get_Float_Parameter("Value"));
	}
}

void JFW_Set_Max_Health_On_Custom_Sender::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Custom"))
	{
		Set_Max_Health(sender,Get_Float_Parameter("Value"));
	}
}

void JFW_Add_Max_Health_On_Custom_Sender::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Custom"))
	{
		Set_Max_Health(sender,Commands->Get_Max_Health(sender)+Get_Float_Parameter("Value"));
	}
}

void JFW_Set_Shield_Strength_On_Custom_Sender::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Custom"))
	{
		Commands->Set_Shield_Strength(sender,Get_Float_Parameter("Value"));
	}
}

void JFW_Add_Shield_Strength_On_Custom_Sender::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Custom"))
	{
		Commands->Set_Shield_Strength(sender,Commands->Get_Shield_Strength(sender)+Get_Float_Parameter("Value"));
	}
}

void JFW_Set_Max_Shield_Strength_On_Custom_Sender::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Custom"))
	{
		Set_Max_Shield_Strength(sender,Get_Float_Parameter("Value"));
	}
}

void JFW_Add_Max_Shield_Strength_On_Custom_Sender::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Custom"))
	{
		Set_Max_Shield_Strength(sender,Commands->Get_Max_Shield_Strength(sender)+Get_Float_Parameter("Value"));
	}
}

void JFW_Regenerate_Health_Conditional::Created(GameObject *obj)
{
	enabled = false;
}

void JFW_Regenerate_Health_Conditional::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("EnableMessage"))
	{
		enabled = true;
		Commands->Send_Custom_Event(obj,obj,100000000,0,0);
		return;
	}
	if (type == Get_Int_Parameter("DisableMessage"))
	{
		enabled = false;
		return;
	}
	if ((type == 100000000) && (enabled))
	{
		float health = Commands->Get_Health(obj);
		Commands->Set_Health(obj,health+Get_Int_Parameter("Amount"));
		health = Commands->Get_Shield_Strength(obj);
		Commands->Set_Shield_Strength(obj,health+Get_Int_Parameter("Amount"));
		return;
	}
}

void JFW_Regenerate_Health_Conditional::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Building_Damage::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Ranged_Damage_To_Buildings_Team(Get_Int_Parameter("Player_Type"),Get_Float_Parameter("Damage"),Get_Parameter("Warhead"),Commands->Get_Position(sender),Get_Float_Parameter("Distance"),Commands->Find_Object(param));
	}
}

void JFW_Building_Damage_Scale::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Ranged_Scale_Damage_To_Buildings_Team(Get_Int_Parameter("Player_Type"),Get_Float_Parameter("Damage"),Get_Parameter("Warhead"),Commands->Get_Position(sender),Get_Float_Parameter("Distance"),Commands->Find_Object(param));
	}
}

void JFW_Blow_Up_On_Death_Driver::Created(GameObject *obj)
{
	DriverID = 0;
}

void JFW_Blow_Up_On_Death_Driver::Custom(GameObject *obj,int type,int param,GameObject *sender)
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

void JFW_Blow_Up_On_Death_Driver::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&DriverID,4,1);
}

void JFW_Blow_Up_On_Death_Driver::Killed(GameObject *obj,GameObject *killer)
{
	Vector3 v;
	const char *c;
	c = Get_Parameter("Explosion");
	v = Commands->Get_Position(obj);
	Commands->Create_Explosion (c,v,Commands->Find_Object(DriverID));
	Destroy_Script();
}

void JFW_Random_DriverDeath::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.5,12121);
}

void JFW_Random_DriverDeath::Timer_Expired(GameObject *obj,int number)
{
	if (number == 12121)
	{
		int percent = Commands->Get_Random_Int(0,100);
		if (percent >= Get_Int_Parameter("Percentage"))
		{
			Commands->Apply_Damage(obj,100,"Death",0);
		}
	}
}

void JFW_Building_Damage_Percentage::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Ranged_Percentage_Damage_To_Buildings_Team(Get_Int_Parameter("Player_Type"),Get_Float_Parameter("Percentage"),Get_Parameter("Warhead"),Commands->Get_Position(sender),Get_Float_Parameter("Distance"),Commands->Find_Object(param));
	}
}

void JFW_Animation_Frame_Damage::Created(GameObject *obj)
{
	enabled = true;
}

void JFW_Animation_Frame_Damage::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if ((Commands->Get_Health(obj) <= Get_Float_Parameter("MaxHealth")) && (Commands->Get_Health(obj) >= Get_Float_Parameter("MinHealth")))
	{
		if (enabled)
		{
			enabled = false;
			Commands->Set_Animation_Frame(obj,Get_Parameter("Animation"),Get_Int_Parameter("Frame"));
		}
	}
	else
	{
		enabled = true;
	}
}

void JFW_Animation_Frame_Damage::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Damage_Do_Damage::Created(GameObject *obj)
{
	enabled = true;
}

void JFW_Damage_Do_Damage::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (Commands->Get_Health(obj) <= Get_Float_Parameter("Health"))
	{
		if (enabled)
		{
			enabled = false;
			int TimerNum = Get_Int_Parameter("TimerNum");
			float Time = Get_Float_Parameter("Time");
			Commands->Start_Timer(obj,this,Time,TimerNum);
		}
	}
	else
	{
		enabled = true;
	}
}

void JFW_Damage_Do_Damage::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Damage_Do_Damage::Timer_Expired(GameObject *obj,int number)
{
		if (!enabled)
		{
			float f;
			const char *w;
			w = Get_Parameter("Warhead");
			f = Get_Float_Parameter("Damage");
			Commands->Apply_Damage(obj,f,w,0);
			int TimerNum = Get_Int_Parameter("TimerNum");
			float Time = Get_Float_Parameter("Time");
			Commands->Start_Timer(obj,this,Time,TimerNum);
		}
}

void JFW_Invulnerable_On_Create::Created(GameObject *obj)
{
	Commands->Set_Shield_Type(obj,Get_Parameter("InvulnerableArmour"));
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Invulnerable_On_Create::Timer_Expired(GameObject *obj,int number)
{
	int TimerNum = Get_Int_Parameter("TimerNum");
	if (TimerNum == number)
	{
		Commands->Set_Shield_Type(obj,Get_Parameter("NormalArmour"));
	}
}

void JFW_Destroy_Self_Timer::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Destroy_Self_Timer::Timer_Expired(GameObject *obj,int number)
{
	int TimerNum = Get_Int_Parameter("TimerNum");
	if (TimerNum == number)
	{
		Commands->Destroy_Object(obj);
	}
}

void JFW_Change_Model_Health::Created(GameObject *obj)
{
	enabled = true;
}

void JFW_Change_Model_Health::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (Commands->Get_Health(obj) <= Get_Float_Parameter("Health"))
	{
		if (enabled)
		{
			enabled = false;
			Commands->Set_Model(obj,Get_Parameter("Model"));
		}
	}
	else
	{
		enabled = true;
	}
}

void JFW_Change_Model_Health::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Change_Model_Health2::Created(GameObject *obj)
{
	enabled = true;
}

void JFW_Change_Model_Health2::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (Commands->Get_Health(obj) > Get_Float_Parameter("Health"))
	{
		if (enabled)
		{
			enabled = false;
			Commands->Set_Model(obj,Get_Parameter("Model"));
		}
	}
	else
	{
		enabled = true;
	}
}

void JFW_Change_Model_Health2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Damage_Animation::Created(GameObject *obj)
{
	enable = false;
}

void JFW_Damage_Animation::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (enable == false)
	{
		enable = true;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,-1,false);
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
}

void JFW_Damage_Animation::Timer_Expired(GameObject *obj,int number)
{
	enable = false;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Animation_Frame(obj),0,false);
}

void JFW_Damage_Animation::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enable,1,1);
}

void JFW_Spawn_Object_Death_Weapon::Killed(GameObject *obj,GameObject *killer)
{
	if (!_stricmp(Get_Current_Weapon(obj),Get_Parameter("Weapon")))
	{
		Vector3 spawn_location;
		float facing;
		float f;
		const char *c;
		GameObject *object;
		spawn_location = Commands->Get_Position(obj);
		facing = Commands->Get_Facing(obj);
		c = Get_Parameter("Drop_Object");
		f = Get_Float_Parameter("Drop_Height");
		spawn_location.Z = spawn_location.Z + f;
		object = Commands->Create_Object (c,spawn_location);
		Commands->Set_Facing(object,facing);
	}
	Destroy_Script();
}

void JFW_Spawn_Object_Death_Team::Killed(GameObject *obj,GameObject *killer)
{
	Vector3 spawn_location;
	float facing;
	float f;
	const char *c;
	GameObject *object;
	spawn_location = Commands->Get_Position(obj);
	facing = Commands->Get_Facing(obj);
	if (Commands->Get_Player_Type(killer) == 1)
	{
		c = Get_Parameter("Drop_Object_GDI");
	}
	else
	{
		c = Get_Parameter("Drop_Object_Nod");
	}
	f = Get_Float_Parameter("Drop_Height");
	spawn_location.Z = spawn_location.Z + f;
	object = Commands->Create_Object (c,spawn_location);
	Commands->Set_Facing(object,facing);
	Destroy_Script();
}

void JFW_Pilot_Repair::Created(GameObject *obj)
{
	int TimerNum = Get_Int_Parameter("TimerNum");
	float Time = Get_Float_Parameter("Time");
	Commands->Start_Timer(obj,this,Time,TimerNum);
}

void JFW_Pilot_Repair::Timer_Expired(GameObject *obj,int number)
{
	int TimerNum = Get_Int_Parameter("TimerNum");
	float Time = Get_Float_Parameter("Time");
	if (number == TimerNum)
	{
		GameObject *o = Get_Vehicle(obj);
		if ((o) && (Get_Vehicle_Driver(o) == obj))
		{
			if (Commands->Get_Max_Health(o) != Commands->Get_Health(o))
			{
				Commands->Set_Health(o,(Commands->Get_Health(o) + Get_Float_Parameter("Health")));
			}
			else if (Commands->Get_Max_Shield_Strength(o) != Commands->Get_Shield_Strength(o))
			{
				Commands->Set_Shield_Strength(o,(Commands->Get_Shield_Strength(o) + Get_Float_Parameter("Health")));
			}
		}
		Commands->Start_Timer(obj,this,Time,TimerNum);
	}
}

void JFW_Damage_Occupants_Death::Killed(GameObject *obj,GameObject *killer)
{
	Damage_Occupants(obj,Get_Float_Parameter("Damage"),Get_Parameter("Warhead"));
	Destroy_Script();
}

void JFW_Engineer_Target_2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&repairid,4,1);
	Auto_Save_Variable(&health,4,2);
	Auto_Save_Variable(&shieldstrength,4,3);
}

void JFW_Engineer_Target_2::Created(GameObject *obj)
{
	health = Commands->Get_Health(obj);
	shieldstrength = Commands->Get_Shield_Strength(obj);
}

void JFW_Engineer_Target_2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 40011)
	{
		repairid = param;
	}
}

void JFW_Engineer_Target_2::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	Vector3 position;
	position = Commands->Get_Position(obj);
	Commands->Create_Logical_Sound(obj,40012,position,150);
	Commands->Start_Timer(obj,this,3.0,40013);
}

void JFW_Engineer_Target_2::Destroyed(GameObject *obj)
{
	Vector3 position;
	position = Commands->Get_Position(obj);
	Commands->Create_Logical_Sound(obj,40014,position,150);
}

void JFW_Engineer_Target_2::Timer_Expired(GameObject *obj,int number)
{
	float currenthealth;
	float currentshieldstrength;
	Vector3 position;
	if (number == 40013)
	{
		currenthealth = Commands->Get_Health(obj);
		currentshieldstrength = Commands->Get_Shield_Strength(obj);
		if ((currenthealth == health) && (currentshieldstrength == shieldstrength))
		{
			position = Commands->Get_Position(obj);
			Commands->Create_Logical_Sound(obj,40014,position,150);
		}
		if ((currenthealth != health) || (currentshieldstrength != shieldstrength))
		{
			position = Commands->Get_Position(obj);
			Commands->Create_Logical_Sound(obj,40012,position,150);
			Commands->Start_Timer(obj,this,3.0,40013);
		}
	}
}

void JFW_Engineer_Repair_2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&active,1,1);
	Auto_Save_Variable(&repairid,4,2);
	Auto_Save_Variable(&targetid,4,3);
	Auto_Save_Variable(&priority,4,4);
}

void JFW_Engineer_Repair_2::Created(GameObject *obj)
{
	active = false;
	priority = Get_Int_Parameter("Repair_Priority");
}

void JFW_Engineer_Repair_2::Sound_Heard(GameObject *obj,const CombatSound & sound)
{
	ActionParamsStruct params;
	if ((sound.sound == 40012) && (!active) && (Commands->Get_Player_Type(sound.Creator) == Commands->Get_Player_Type(obj)))
	{
		repairid = Commands->Get_ID(obj);
		targetid = Get_Int_Parameter("Target_ID");
		Commands->Send_Custom_Event(obj,sound.Creator,40011,repairid,0);
		active = true;
		params.Set_Basic(this,(float)priority,40015);
		params.Set_Movement(Commands->Get_Position(Commands->Find_Object(targetid)),(float)0.8,(float)5.0,0);
		Commands->Action_Goto(obj,params);
	}
	else
	{
		if ((sound.sound == 40014) && (targetid == Get_Int_Parameter("Target_ID")))
		{
			Commands->Action_Reset(obj,100);
			active = false;
		}
	}
}

void JFW_Engineer_Repair_2::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	ActionParamsStruct params;
	if (action_id == 40015)
	{
		params.Set_Basic(this,(float)priority,40016);
		params.Set_Attack(Commands->Find_Object(targetid),50,0,false);
		params.AttackCheckBlocked = false;
		Commands->Action_Attack(obj,params);
	}
}

void JFW_Kill_Message_Display::Created(GameObject *obj)
{
	INIClass *config = Get_INI(Get_Parameter("ConfigFile"));
	count = config->Get_Int("PresetNames","PresetNameCount",0);
	messages = new KillMessage[count];
	KillerIsMSBot = false;
	KilledIsMSBot = false;
	char preset[50];
	char c[50];
	for (int i = 0;i < count;i++)
	{
		sprintf(c,"Preset%dName",i);
		config->Get_String("PresetNames",c,"",preset,49);
		messages[i].PresetID = Get_Definition_ID(preset);
		sprintf(c,"Preset%dStringID",i);
		messages[i].StringID = config->Get_Int("PresetNames",c,0);
		sprintf(c,"Preset%dSendDriverCustom",i);
		messages[i].SendDriverCustom = config->Get_Bool("PresetNames",c,false);
	}
	Release_INI(config);
}

void JFW_Kill_Message_Display::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (!param || !Commands->Find_Object(param) ||!sender)
	{
		return;
	}
	if (type == Get_Int_Parameter("Message"))
	{
		KillerIsMSBot = false;
		KilledIsMSBot = false;

		if (Commands->Is_A_Star(sender))
		{
			KilledID = 0;
			KilledPlayerID = Get_Player_ID(sender);
		}
		else if (sender->As_SoldierGameObj() && sender->As_SoldierGameObj()->Is_Bot())
		{
			KilledID = 0;
			KilledPlayerID = sender->Get_ID();
			KilledIsMSBot = true;
		}
		else if (sender->As_VehicleGameObj() && (Get_Vehicle_Driver(sender)))
		{
			KilledID = Commands->Get_Preset_ID(sender);
			if (Get_Vehicle_Driver(sender)->As_SoldierGameObj()->Is_Bot())
			{
				KilledIsMSBot = true;
				KilledPlayerID = Get_Vehicle_Driver(sender)->Get_ID();
			}
			else
				KilledPlayerID = Get_Player_ID(Get_Vehicle_Driver(sender));
		}
		else
		{
			KilledID = Commands->Get_Preset_ID(sender);
			KilledPlayerID = 0;
		}

		GameObject *Killer = Commands->Find_Object(param);
		GameObject *explosion = GetExplosionObj();
		if (!explosion || (explosion->As_VehicleGameObj() && Get_Vehicle_Mode(explosion) != VEHICLE_TYPE_TURRET))
		{
			Killer = Get_Vehicle_Return(Killer);
		}
		if (Killer->As_SoldierGameObj())
		{
			KillerTeam = Get_Object_Type(Killer);
			if (Commands->Is_A_Star(Killer))
			{
				KillerPlayerID = Get_Player_ID(Killer);
				KillerTeam = Get_Object_Type(Killer);
			}
			else if (Killer->As_SoldierGameObj()->Is_Bot())
			{
				KillerIsMSBot = true;
				KillerPlayerID = Killer->Get_ID();
				KillerTeam = Get_Object_Type(Killer);
			}
			else
			{
				KillerPlayerID = 0;
			}
			if (explosion)
			{
				KillerID = Commands->Get_Preset_ID(explosion);
			}
			else
			{
				KillerID = Get_Definition_ID(Get_Current_Weapon(Killer));
			}
		}
		else if (Killer->As_VehicleGameObj())
		{
			KillerID = Commands->Get_Preset_ID(Killer);
			if (explosion)
			{
				KillerID = Commands->Get_Preset_ID(explosion);
			}
			KillerTeam = Get_Object_Type(Killer);
			if (Get_Vehicle_Gunner(Killer))
			{
				if (Get_Vehicle_Gunner(Killer)->As_SoldierGameObj()->Is_Bot())
				{
					KillerIsMSBot = true;
					KillerPlayerID = Get_Vehicle_Gunner(Killer)->Get_ID();
				}
				else
					KillerPlayerID = Get_Player_ID(Get_Vehicle_Gunner(Killer));

				KillerTeam = Get_Object_Type(Get_Vehicle_Gunner(Killer));
			}
			else
			{
				KillerPlayerID = 0;
				for (int i = 0;i < count;i++)
				{
					if ((messages[i].PresetID == KillerID) && (messages[i].SendDriverCustom))
					{
						Commands->Send_Custom_Event(obj,Killer,CUSTOM_EVENT_VEHICLE_DRIVER,0,0);
						return;
					}
				}
			}
		}
	}
	else if (type == CUSTOM_EVENT_VHEICLE_DRIVER_ID)
	{
		if (Commands->Find_Object(param)->As_SoldierGameObj()->Is_Bot())
		{
			KillerIsMSBot = true;
			KillerPlayerID = param;
		}
		else
			KillerPlayerID = Get_Player_ID(Commands->Find_Object(param));

		KillerTeam = Get_Object_Type(Commands->Find_Object(param));
	}
	else
	{
		return;
	}
	const char *KilledString = 0;
	const char *KillerString = 0;
	const char *KilledPlayerString = 0;
	const char *KillerPlayerString = 0;
	if (KilledID)
	{
		for (int i = 0;i < count;i++)
		{
			if (messages[i].PresetID == KilledID)
			{
				KilledString = Get_Translated_String(messages[i].StringID);
			}
		}
	}
	else
	{
		KilledString = newstr("");
	}
	if (KillerID)
	{
		for (int i = 0;i < count;i++)
		{
			if (messages[i].PresetID == KillerID)
			{
				KillerString = Get_Translated_String(messages[i].StringID);
			}
		}
	}
	else
	{
		KillerString = newstr("");
	}
	if (KilledPlayerID)
	{
		if (KilledIsMSBot)
			KilledPlayerString = Get_Player_Name(Commands->Find_Object(KilledPlayerID));
		else
			KilledPlayerString = Get_Player_Name_By_ID(KilledPlayerID);
	}
	else
	{
		KilledPlayerString = newstr("");
	}
	if (KillerPlayerID)
	{
		if (KillerIsMSBot)
			KillerPlayerString = Get_Player_Name(Commands->Find_Object(KillerPlayerID));
		else
			KillerPlayerString = Get_Player_Name_By_ID(KillerPlayerID);
	}
	else
	{
		KillerPlayerString = newstr("");
	}
	if ((!KilledString) || (!KillerString) || (!KilledPlayerString) || (!KillerPlayerString))
	{
		if (KilledString)
		{
			delete[] KilledString;
		}
		if (KillerString)
		{
			delete[] KillerString;
		}
		if (KilledPlayerString)
		{
			delete[] KilledPlayerString;
		}
		if (KillerPlayerString)
		{
			delete[] KillerPlayerString;
		}
		return;
	}
	char msg[256];
	sprintf(msg,"%s%s%s%s",KillerPlayerString,KillerString,KilledPlayerString,KilledString);
	Send_Message_With_Team_Color(KillerTeam,msg);
	delete[] KilledString;
	delete[] KillerString;
	delete[] KilledPlayerString;
	delete[] KillerPlayerString;
}

JFW_Kill_Message_Display::~JFW_Kill_Message_Display()
{
	SAFE_DELETE_ARRAY(messages);
}

void JFW_Kill_Message::Killed(GameObject *obj,GameObject *killer)
{
	if (killer)
	{
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("JFW_Kill_Message_Display"),Get_Int_Parameter("Message"),Commands->Get_ID(killer),0);
	}
	else
	{
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("JFW_Kill_Message_Display"),Get_Int_Parameter("Message"),0,0);
	}
	Destroy_Script();
}

void JFW_C4_Explode::Created(GameObject *obj)
{
	GameObject *object = Get_C4_Planter(obj);
	if (!_stricmp(Commands->Get_Preset_Name(object),Get_Parameter("Preset")))
	{
		Commands->Create_Explosion(Get_Parameter("Explosion"),Commands->Get_Position(obj),0);
	}
	Destroy_Script();
}

void JFW_Beacon_Building::Created(GameObject *obj)
{
	pos = AddPowerupPurchaseHook(PowerupPurchaseHook,(const char *)obj);
}

JFW_Beacon_Building::~JFW_Beacon_Building()
{
	if (Exe == 0 || Exe == 1)
	{
		RemovePowerupPurchaseHook(pos);
	}
}

int JFW_Beacon_Building::PowerupPurchaseHook(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data)
{
	if (((GameObject *)data)->As_BuildingGameObj() && ((GameObject *)data)->As_BuildingGameObj()->Is_Destroyed() && ((GameObject *)data)->As_BuildingGameObj()->Get_Player_Type() == Commands->Get_Player_Type(purchaser))
	{
		return 3;
	}
	return -1;
}

void JFW_Spawn_Object_Repair::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj) && amount)
	{
		Vector3 pos = Commands->Get_Position(obj);
		float facing = Commands->Get_Facing(obj);
		Commands->Destroy_Object(obj);
		GameObject *o = Commands->Create_Object(Get_Parameter("Object"),pos);
		Commands->Set_Facing(o,facing);
	}
}

ScriptRegistrant<JFW_Invulnerable_On_Create> JFW_Invulnerable_On_Create_Registrant("JFW_Invulnerable_On_Create","Time:float,TimerNum:int,InvulnerableArmour:string,NormalArmour:string");
ScriptRegistrant<JFW_Destroy_Self_Timer> JFW_Destroy_Self_Timer_Registrant("JFW_Destroy_Self_Timer","Time:float,TimerNum:int");
ScriptRegistrant<JFW_Change_Model_Health> JFW_Change_Model_Health_Registrant("JFW_Change_Model_Health","Model:string,Health:float");
ScriptRegistrant<JFW_Change_Model_Health2> JFW_Change_Model_Health2_Registrant("JFW_Change_Model_Health2","Model:string,Health:float");
ScriptRegistrant<JFW_Damage_Animation> JFW_Damage_Animation_Registrant("JFW_Damage_Animation","Animation:string,Time:float,TimerNum:int");
ScriptRegistrant<JFW_Spawn_Object_Death_Weapon> JFW_Spawn_Object_Death_Weapon_Registrant("JFW_Spawn_Object_Death_Weapon","Drop_Object:string,Drop_Height:float,Weapon:string");
ScriptRegistrant<JFW_Spawn_Object_Death_Team> JFW_Spawn_Object_Death_Team_Registrant("JFW_Spawn_Object_Death_Team","Drop_Object_GDI:string,Drop_Object_Nod:string,Drop_Height:float");
ScriptRegistrant<JFW_Building_Damage_Percentage> JFW_Building_Damage_Percentage_Registrant("JFW_Building_Damage_Percentage","Message:int,Player_Type:int,Percentage:float,Warhead:string,Distance:float");
ScriptRegistrant<JFW_Animation_Frame_Damage> JFW_Animation_Frame_Damage_Registrant("JFW_Animation_Frame_Damage","Animation:string,Frame:int,MinHealth:float,MaxHealth:float");
ScriptRegistrant<JFW_Damage_Do_Damage> JFW_Damage_Do_Damage_Registrant("JFW_Damage_Do_Damage","Health:float,Damage:float,Warhead:string,Time:float,TimerNum:int");
ScriptRegistrant<JFW_Death_Destroy_Object_Delay> JFW_Death_Destroy_Object_Delay_Registrant("JFW_Death_Destroy_Object_Delay","ID:int,Time:float,TimerNum:int");
ScriptRegistrant<JFW_Set_Health_On_Custom> JFW_Set_Health_On_Custom_Registrant("JFW_Set_Health_On_Custom","Custom:int,Value:float");
ScriptRegistrant<JFW_Add_Health_On_Custom> JFW_Add_Health_On_Custom_Registrant("JFW_Add_Health_On_Custom","Custom:int,Value:float");
ScriptRegistrant<JFW_Set_Max_Health_On_Custom> JFW_Set_Max_Health_On_Custom_Registrant("JFW_Set_Max_Health_On_Custom","Custom:int,Value:float");
ScriptRegistrant<JFW_Add_Max_Health_On_Custom> JFW_Add_Max_Health_On_Custom_Registrant("JFW_Add_Max_Health_On_Custom","Custom:int,Value:float");
ScriptRegistrant<JFW_Set_Shield_Strength_On_Custom> JFW_Set_Shield_Strength_On_Custom_Registrant("JFW_Set_Shield_Strength_On_Custom","Custom:int,Value:float");
ScriptRegistrant<JFW_Add_Shield_Strength_On_Custom> JFW_Add_Shield_Strength_On_Custom_Registrant("JFW_Add_Shield_Strength_On_Custom","Custom:int,Value:float");
ScriptRegistrant<JFW_Set_Max_Shield_Strength_On_Custom> JFW_Set_Max_Shield_Strength_On_Custom_Registrant("JFW_Set_Max_Shield_Strength_On_Custom","Custom:int,Value:float");
ScriptRegistrant<JFW_Add_Max_Shield_Strength_On_Custom> JFW_Add_Max_Shield_Strength_On_Custom_Registrant("JFW_Add_Max_Shield_Strength_On_Custom","Custom:int,Value:float");
ScriptRegistrant<JFW_Set_Health_On_Custom_Sender> JFW_Set_Health_On_Custom_Sender_Registrant("JFW_Set_Health_On_Custom_Sender","Custom:int,Value:float");
ScriptRegistrant<JFW_Add_Health_On_Custom_Sender> JFW_Add_Health_On_Custom_Sender_Registrant("JFW_Add_Health_On_Custom_Sender","Custom:int,Value:float");
ScriptRegistrant<JFW_Set_Max_Health_On_Custom_Sender> JFW_Set_Max_Health_On_Custom_Sender_Registrant("JFW_Set_Max_Health_On_Custom_Sender","Custom:int,Value:float");
ScriptRegistrant<JFW_Add_Max_Health_On_Custom_Sender> JFW_Add_Max_Health_On_Custom_Sender_Registrant("JFW_Add_Max_Health_On_Custom_Sender","Custom:int,Value:float");
ScriptRegistrant<JFW_Set_Shield_Strength_On_Custom_Sender> JFW_Set_Shield_Strength_On_Custom_Sender_Registrant("JFW_Set_Shield_Strength_On_Custom_Sender","Custom:int,Value:float");
ScriptRegistrant<JFW_Add_Shield_Strength_On_Custom_Sender> JFW_Add_Shield_Strength_On_Custom_Sender_Registrant("JFW_Add_Shield_Strength_On_Custom_Sender","Custom:int,Value:float");
ScriptRegistrant<JFW_Set_Max_Shield_Strength_On_Custom_Sender> JFW_Set_Max_Shield_Strength_On_Custom_Sender_Registrant("JFW_Set_Max_Shield_Strength_On_Custom_Sender","Custom:int,Value:float");
ScriptRegistrant<JFW_Add_Max_Shield_Strength_On_Custom_Sender> JFW_Add_Max_Shield_Strength_On_Custom_Sender_Registrant("JFW_Add_Max_Shield_Strength_On_Custom_Sender","Custom:int,Value:float");
ScriptRegistrant<JFW_Regenerate_Health_Conditional> JFW_Regenerate_Health_Conditional_Registrant("JFW_Regenerate_Health_Conditional","EnableMessage:int,DisableMessage:int,Amount:int");
ScriptRegistrant<JFW_Building_Damage> JFW_Building_Damage_Registrant("JFW_Building_Damage","Message:int,Player_Type:int,Damage:float,Warhead:string,Distance:float");
ScriptRegistrant<JFW_Building_Damage_Scale> JFW_Building_Damage_Scale_Registrant("JFW_Building_Damage_Scale","Message:int,Player_Type:int,Damage:float,Warhead:string,Distance:float");
ScriptRegistrant<JFW_Blow_Up_On_Death_Driver> JFW_Blow_Up_On_Death_Driver_Registrant("JFW_Blow_Up_On_Death_Driver","Explosion:string");
ScriptRegistrant<JFW_Random_DriverDeath> JFW_Random_DriverDeath_Registrant("JFW_Random_DriverDeath","Percentage:int");
ScriptRegistrant<JFW_Armour_Regen> JFW_Armour_Regen_Registrant("JFW_Armour_Regen","Time:float,TimerNum:int,Points:float");
ScriptRegistrant<JFW_Health_Regen> JFW_Health_Regen_Registrant("JFW_Health_Regen","Time:float,TimerNum:int,Points:float");
ScriptRegistrant<JFW_Give_Points_On_Death> JFW_Give_Points_On_Death_Registrant("JFW_Give_Points_On_Death","Points:float,Player_Type:int,Whole_Team:int");
ScriptRegistrant<JFW_Death_Destroy_Object> JFW_Death_Destroy_Object_Registrant("JFW_Death_Destroy_Object","ID:int");
ScriptRegistrant<JFW_Invulnerability_Timer> JFW_Invulnerability_Timer_Registrant("JFW_Invulnerability_Timer","Invulnerability_Time:float,Invulnerability_TimerNum:int");
ScriptRegistrant<JFW_Spawn_Object_Death_Enable> JFW_Spawn_Object_Death_Enable_Registrant("JFW_Spawn_Object_Death_Enable","Drop_Object:string,Drop_Height:float,EnableMessage:int,DisableMessage:int");
ScriptRegistrant<JFW_Spawn_Object_Death_Enable_Random> JFW_Spawn_Object_Death_Enable_Random_Registrant("JFW_Spawn_Object_Death_Enable_Random","Drop_Object:string,Drop_Height:float,EnableMessage:int,DisableMessage:int,Probobility:int");
ScriptRegistrant<JFW_Spawn_Object_Death_Random> JFW_Spawn_Object_Death_Random_Registrant("JFW_Spawn_Object_Death_Random","Drop_Object:string,Drop_Height:float,Probobility:int");
ScriptRegistrant<JFW_Engineer_Target> JFW_Engineer_Target_Registrant("JFW_Engineer_Target","");
ScriptRegistrant<JFW_Engineer_Repair> JFW_Engineer_Repair_Registrant("JFW_Engineer_Repair","Repair_Priority=96:int");
ScriptRegistrant<JFW_Blow_Up_On_Death> JFW_Blow_Up_On_Death_Registrant("JFW_Blow_Up_On_Death","Explosion:string");
ScriptRegistrant<JFW_Spawn_Object_Death> JFW_Spawn_Object_Death_Registrant("JFW_Spawn_Object_Death","Drop_Object:string,Drop_Height:float");
ScriptRegistrant<JFW_Timer_Destroy_Building> JFW_Timer_Destroy_Building_Registrant("JFW_Timer_Destroy_Building","Time:float,TimerNum:int");
ScriptRegistrant<JFW_Pilot_Repair> JFW_Pilot_Repair_Registrant("JFW_Pilot_Repair","Time:float,TimerNum:int,Health:float");
ScriptRegistrant<JFW_Damage_Occupants_Death> JFW_Damage_Occupants_Death_Registrant("JFW_Damage_Occupants_Death","Damage:float,Warhead:string");
ScriptRegistrant<JFW_Engineer_Target_2> JFW_Engineer_Target_2_Registrant("JFW_Engineer_Target_2","");
ScriptRegistrant<JFW_Engineer_Repair_2> JFW_Engineer_Repair_2_Registrant("JFW_Engineer_Repair_2","Repair_Priority=96:int,Target_ID:int");
ScriptRegistrant<JFW_Kill_Message_Display> JFW_Kill_Message_Display_Registrant("JFW_Kill_Message_Display","ConfigFile:string,Message:int");
ScriptRegistrant<JFW_Kill_Message> JFW_Kill_Message_Registrant("JFW_Kill_Message","Message:int");
ScriptRegistrant<JFW_C4_Explode> JFW_C4_Explode_Registrant("JFW_C4_Explode","Preset:string,Explosion:string");
ScriptRegistrant<JFW_Timer_Destroy_Object> JFW_Timer_Destroy_Object_Registrant("JFW_Timer_Destroy_Object","Time:float,TimerNum:int,Amount:float,Warhead:string");
ScriptRegistrant<JFW_Beacon_Building> JFW_Beacon_Building_Registrant("JFW_Beacon_Building","");
ScriptRegistrant<JFW_Spawn_Object_Repair> JFW_Spawn_Object_Repair_Registrant("JFW_Spawn_Object_Repair","Object:string");
