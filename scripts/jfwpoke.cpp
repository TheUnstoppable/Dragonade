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
#include "jfwpoke.h"

void JFW_Poke_Send_Custom::Created(GameObject *obj)
{
	int type;
	active = 0;
	type = Get_Int_Parameter("Activate_Message");
	if (!type)
	{
		active = 1;
	}
	Commands->Enable_HUD_Pokable_Indicator(obj,1);
}

void JFW_Poke_Send_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	msg = Get_Int_Parameter("Activate_Message");
	if (msg == type)
	{
		active = param;
		if ((!param) || (param == 1))
		{
			Commands->Enable_HUD_Pokable_Indicator(obj,1);
		}
		else
		{
			Commands->Enable_HUD_Pokable_Indicator(obj,0);
		}
	}
}

void JFW_Poke_Send_Custom::Poked(GameObject *obj,GameObject *poker)
{
	const char *c;
	int msg;
	int param;
	int id;
	float delay;
	Vector3 v;
	GameObject *sendobj;
	if (!active)
	{
		c = Get_Parameter("Sound_Name");
		v = Commands->Get_Position(obj);
		Commands->Create_Sound(c,v,obj);
	}
	else
	{
		if (active == 1)
		{
		active = 2;
		id = Get_Int_Parameter("Object_Id");
		msg = Get_Int_Parameter("Message");
		param = Get_Int_Parameter("Param");
		delay = Get_Float_Parameter("Delay");
		Commands->Enable_HUD_Pokable_Indicator(obj,0);
		sendobj = Commands->Find_Object(id);
		Commands->Send_Custom_Event(obj,sendobj,msg,param,delay);
		}
	}
}

void JFW_Poke_Send_Custom::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&active,1,1);
}

void JFW_Pokeable_Item::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj,1);
	Destroy_Script();
}

void JFW_Preset_Buy_Poke::Poked(GameObject *obj,GameObject *poker)
{
	const char *preset;
	int x;
	int cost;
	Vector3 spawn_position;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	preset = Get_Parameter("Preset_Name");
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(poker))
	{
		cost = -cost;
		spawn_position = Get_Vector3_Parameter("location");
		Commands->Give_Money(poker,(float)cost,0);
		Commands->Create_Object(preset,spawn_position);
	}
}

void JFW_Character_Buy_Poke::Poked(GameObject *obj,GameObject *poker)
{
	const char *preset;
	int x;
	int cost;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	preset = Get_Parameter("Preset_Name");
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(poker))
	{
		cost = -cost;
		Commands->Give_Money(poker,(float)cost,0);
		Change_Character(poker,preset);
	}
}

void JFW_Refill_Buy_Poke::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	int cost;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(poker))
	{
		cost = -cost;
		Commands->Give_Money(poker,(float)cost,0);
		Grant_Refill(poker);
	}
}

void JFW_Powerup_Buy_Poke::Poked(GameObject *obj,GameObject *poker)
{
	const char *preset;
	int x;
	int cost;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	preset = Get_Parameter("Preset_Name");
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(poker))
	{
		cost = -cost;
		Commands->Give_Money(poker,(float)cost,0);
		Grant_Powerup(poker,preset);
	}
}

void JFW_Vehicle_Buy_Poke::Poked(GameObject *obj,GameObject *poker)
{
	const char *preset;
	int x;
	int cost;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	preset = Get_Parameter("Preset_Name");
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(poker))
	{
		cost = -cost;
		Commands->Give_Money(poker,(float)cost,0);
		Create_Vehicle(preset,5.0,poker,x);
	}
}

void JFW_Group_Purchase_Poke::Created(GameObject *obj)
{
	cash = 0;
	enabled = true;
}

void JFW_Group_Purchase_Poke::Poked(GameObject *obj,GameObject *poker)
{
	if (enabled)
	{
		float money = Commands->Get_Money(poker);
		if (money >= Get_Float_Parameter("CashPerPerson"))
		{
			Commands->Give_Money(poker,Get_Float_Parameter("CashPerPerson"),0);
			cash += Get_Float_Parameter("CashPerPerson");
		}
		if (cash >= Get_Float_Parameter("TotalCash"))
		{
			cash = 0;
			enabled = false;
			Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message"),0,0);
		}
	}
}

void JFW_Group_Purchase_Poke::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&cash,4,1);
	Auto_Save_Variable(&enabled,1,2);
}

void JFW_Gate_Poke::Poked(GameObject *obj,GameObject *poker)
{
	int Player_Type = Get_Int_Parameter("Player_Type");
	int ID = Get_Int_Parameter("ID");
	const char *Animation = Get_Parameter("Animation");
	int TimerNum = Get_Int_Parameter("TimerNum");
	float Time = Get_Float_Parameter("Time");
	if (CheckPlayerType(poker,Player_Type))
	{
		return;
	}
	Commands->Set_Animation(Commands->Find_Object(ID),Animation,false,0,0,-1,false);
	Commands->Start_Timer(obj,this,Time,TimerNum);
}

void JFW_Gate_Poke::Timer_Expired(GameObject *obj,int number)
{
	int ID = Get_Int_Parameter("ID");
	const char *Animation = Get_Parameter("Animation");
	int TimerNum = Get_Int_Parameter("TimerNum");
	if (TimerNum == number)
	{
		Commands->Set_Animation(Commands->Find_Object(ID),Animation,false,0,Get_Animation_Frame(Commands->Find_Object(ID)),0,false);
	}
}

void JFW_Poke_Send_Custom_2::Poked(GameObject *obj,GameObject *poker)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,team))
	{
		return;
	}
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message"),Get_Int_Parameter("Param"),0);
}

void JFW_Poke_Send_Custom_Preset::Poked(GameObject *obj,GameObject *poker)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,team))
	{
		return;
	}
	if (_stricmp(Commands->Get_Preset_Name(poker),Get_Parameter("Preset")))
	{
		return;
	}
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message"),Get_Int_Parameter("Param"),0);
}

void JFW_Poke_Send_Custom_Keycard::Poked(GameObject *obj,GameObject *poker)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,team))
	{
		return;
	}
	if (!Commands->Has_Key(poker,Get_Int_Parameter("Key")))
	{
		return;
	}
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message"),Get_Int_Parameter("Param"),0);
}

void JFW_Poke_Send_Custom_Cost::Poked(GameObject *obj,GameObject *poker)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,team))
	{
		return;
	}
	if (Commands->Get_Money(poker) < Get_Float_Parameter("Cost"))
	{
		return;
	}
	Commands->Give_Money(poker,-Get_Float_Parameter("Cost"),false);
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message"),Get_Int_Parameter("Param"),0);
}

void JFW_Poke_Play_2D_Sound::Poked(GameObject *obj,GameObject *poker)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,team))
	{
		return;
	}
	Commands->Create_2D_Sound(Get_Parameter("Sound"));
}

void JFW_Poke_Play_3D_Sound::Poked(GameObject *obj,GameObject *poker)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,team))
	{
		return;
	}
	Commands->Create_Sound(Get_Parameter("Sound"),Get_Vector3_Parameter("Position"),obj);
}

void JFW_Powerup_Buy_Poke_Timer::Poked(GameObject *obj,GameObject *poker)
{
	const char *preset;
	int x;
	int cost;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	if (enabled)
	{
		preset = Get_Parameter("Preset_Name");
		cost = Get_Int_Parameter("Cost");
		if (cost <= Commands->Get_Money(poker))
		{
			cost = -cost;
			Commands->Give_Money(poker,(float)cost,0);
			Grant_Powerup(poker,preset);
			enabled = false;
			int TimerNum = Get_Int_Parameter("TimerNum");
			float Time = Get_Float_Parameter("Time");
			Commands->Start_Timer(obj,this,Time,TimerNum);
		}
	}
}

void JFW_Powerup_Buy_Poke_Timer::Created(GameObject *obj)
{
	enabled = true;
}

void JFW_Powerup_Buy_Poke_Timer::Timer_Expired(GameObject *obj,int number)
{
	int TimerNum = Get_Int_Parameter("TimerNum");
	if (TimerNum == number)
	{
		enabled = true;
	}
}

void JFW_Powerup_Buy_Poke_Timer::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_PCT::Poked(GameObject *obj,GameObject *poker)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,team))
	{
		return;
	}
	if (!team)
	{
		Display_NOD_Player_Terminal_Player(poker);
	}
	if (team == 1)
	{
		Display_GDI_Player_Terminal_Player(poker);
	}
	if (team == 2)
	{
		if (!Commands->Get_Player_Type(poker))
		{
			Display_NOD_Player_Terminal_Player(poker);
		}
		if (Commands->Get_Player_Type(poker) == 1)
		{
			Display_GDI_Player_Terminal_Player(poker);
		}
	}
}

void JFW_Poke_Play_2D_Sound_Team::Poked(GameObject *obj,GameObject *poker)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,team))
	{
		return;
	}
	Create_2D_Sound_Team(Get_Parameter("Sound"),team);
}

void JFW_Poke_Play_3D_Sound_Team::Poked(GameObject *obj,GameObject *poker)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,team))
	{
		return;
	}
	Create_Sound_Team(Get_Parameter("Sound"),Get_Vector3_Parameter("Position"),obj,team);
}

void JFW_Poke_Play_2D_Sound_Player::Poked(GameObject *obj,GameObject *poker)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,team))
	{
		return;
	}
	Create_2D_Sound_Player(poker,Get_Parameter("Sound"));
}

void JFW_Poke_Play_3D_Sound_Player::Poked(GameObject *obj,GameObject *poker)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,team))
	{
		return;
	}
	Create_Sound_Player(poker,Get_Parameter("Sound"),Get_Vector3_Parameter("Position"),obj);
}

void JFW_Look_At_Location_Poke::Poked(GameObject *obj,GameObject *poker)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,team))
	{
		return;
	}
	Force_Camera_Look_Player(poker,Get_Vector3_Parameter("Location"));
}

void JFW_Look_At_Object_Poke::Poked(GameObject *obj,GameObject *poker)
{
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,team))
	{
		return;
	}
	Force_Camera_Look_Player(poker,Commands->Get_Position(Commands->Find_Object(Get_Int_Parameter("ObjectID"))));
}

void JFW_Weapon_Buy_Poke::Poked(GameObject *obj,GameObject *poker)
{
	const char *preset;
	int x;
	int cost;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	preset = Get_Parameter("Weapon_Name");
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(poker))
	{
		cost = -cost;
		Commands->Give_Money(poker,(float)cost,0);
		Commands->Give_PowerUp(poker,preset,true);
	}
}

void JFW_Preset_Buy_Poke_Timer::Poked(GameObject *obj,GameObject *poker)
{
	Vector3 spawn_position;
	const char *preset;
	int cost;
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	if (enabled)
	{
		preset = Get_Parameter("Preset_Name");
		cost = Get_Int_Parameter("Cost");
		if (cost <= Commands->Get_Money(poker))
		{
			cost = -cost;
			spawn_position = Get_Vector3_Parameter("location");
			Commands->Give_Money(poker,(float)cost,0);
			Commands->Create_Object(preset,spawn_position);
			enabled = false;
			int TimerNum = Get_Int_Parameter("TimerNum");
			float Time = Get_Float_Parameter("Time");
			Commands->Start_Timer(obj,this,Time,TimerNum);
		}
	}
}

void JFW_Preset_Buy_Poke_Timer::Created(GameObject *obj)
{
	enabled = true;
}

void JFW_Preset_Buy_Poke_Timer::Timer_Expired(GameObject *obj,int number)
{
	int TimerNum = Get_Int_Parameter("TimerNum");
	if (TimerNum == number)
	{
		enabled = true;
	}
}

void JFW_Preset_Buy_Poke_Timer::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Preset_Buy_Poke_Custom::Poked(GameObject *obj,GameObject *poker)
{
	Vector3 spawn_position;
	const char *preset;
	int x;
	int cost;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	if (enabled)
	{
		preset = Get_Parameter("Preset_Name");
		cost = Get_Int_Parameter("Cost");
		if (cost <= Commands->Get_Money(poker))
		{
			cost = -cost;
			spawn_position = Get_Vector3_Parameter("location");
			Commands->Give_Money(poker,(float)cost,0);
			Commands->Create_Object(preset,spawn_position);
			enabled = false;
		}
	}
}

void JFW_Preset_Buy_Poke_Custom::Created(GameObject *obj)
{
	enabled = true;
}

void JFW_Preset_Buy_Poke_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		enabled = true;
	}
}

void JFW_Preset_Buy_Poke_Custom::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Slot_Machine::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	int cost = Get_Int_Parameter("Cost");
	int winnings = Get_Int_Parameter("Winnings");
	int percent = Get_Int_Parameter("Percent");
	int chance = Commands->Get_Random_Int(0,99);
	if (chance < percent)
	{
		Commands->Give_Money(poker,(float)winnings,0);
	}
	else
	{
		cost = -cost;
		Commands->Give_Money(poker,(float)cost,0);
	}
}

void JFW_Slot_Machine_2::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	int cost = Get_Int_Parameter("Cost");
	int winnings = Get_Int_Parameter("Winnings");
	int percent = Get_Int_Parameter("Percent");
	int chance = Commands->Get_Random_Int(0,99);
	if (chance < percent)
	{
		Commands->Give_Money(poker,(float)winnings,0);
	}
	else
	{
		cost = -cost;
		Commands->Give_Money(poker,(float)cost,0);
	}
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,-1,false);
	Vector3 v = Commands->Get_Position(obj);
	Commands->Create_Sound(Get_Parameter("Sound"),v,obj);
}

void JFW_Switch_Door::Poked(GameObject *obj,GameObject *poker)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
	Commands->Set_Animation(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Parameter("Animation"),false,0,0,-1,false);
}

void JFW_Switch_Door::Timer_Expired(GameObject *obj,int number)
{
	Commands->Set_Animation(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Parameter("Animation"),false,0,Get_Animation_Frame(Commands->Find_Object(Get_Int_Parameter("ID"))),0,false);
}

void JFW_Switch_Lock_Door::Poked(GameObject *obj,GameObject *poker)
{
	if (Commands->Has_Key(poker,Get_Int_Parameter("key")))
	{
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
		Commands->Set_Animation(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Parameter("Animation"),false,0,0,-1,false);
	}
}

void JFW_Switch_Lock_Door::Timer_Expired(GameObject *obj,int number)
{
	Commands->Set_Animation(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Parameter("Animation"),false,0,Get_Animation_Frame(Commands->Find_Object(Get_Int_Parameter("ID"))),0,false);
}

void JFW_Toggle_Door::Poked(GameObject *obj,GameObject *poker)
{
	if (!open)
	{
		open = true;
		Commands->Set_Animation(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Parameter("Animation"),false,0,0,-1,false);
	}
	else
	{
		open = false;
		Commands->Set_Animation(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Parameter("Animation"),false,0,Get_Animation_Frame(Commands->Find_Object(Get_Int_Parameter("ID"))),0,false);
	}
}

void JFW_Toggle_Door::Created(GameObject *obj)
{
	open = false;
}

void JFW_Toggle_Door::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&open,1,1);
}

void JFW_Toggle_Lock_Door::Poked(GameObject *obj,GameObject *poker)
{
	if (Commands->Has_Key(poker,Get_Int_Parameter("key")))
	{
		if (!open)
		{
			open = true;
			Commands->Set_Animation(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Parameter("Animation"),false,0,0,-1,false);
		}
		else
		{
			open = false;
			Commands->Set_Animation(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Parameter("Animation"),false,0,Get_Animation_Frame(Commands->Find_Object(Get_Int_Parameter("ID"))),0,false);
		}
	}
}

void JFW_Toggle_Lock_Door::Created(GameObject *obj)
{
	open = false;
}

void JFW_Toggle_Lock_Door::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&open,1,1);
}

void JFW_Switch_Door_Team::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
	Commands->Set_Animation(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Parameter("Animation"),false,0,0,-1,false);
}

void JFW_Switch_Door_Team::Timer_Expired(GameObject *obj,int number)
{
	Commands->Set_Animation(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Parameter("Animation"),false,0,Get_Animation_Frame(Commands->Find_Object(Get_Int_Parameter("ID"))),0,false);
}

void JFW_Toggle_Door_Team::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	if (!open)
	{
		open = true;
		Commands->Set_Animation(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Parameter("Animation"),false,0,0,-1,false);
	}
	else
	{
		open = false;
		Commands->Set_Animation(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Parameter("Animation"),false,0,Get_Animation_Frame(Commands->Find_Object(Get_Int_Parameter("ID"))),0,false);
	}
}

void JFW_Toggle_Door_Team::Created(GameObject *obj)
{
	open = false;
}

void JFW_Toggle_Door_Team::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&open,1,1);
}

void JFW_Preset_Buy_Poke_Sound::Poked(GameObject *obj,GameObject *poker)
{
	const char *preset;
	int x;
	int cost;
	Vector3 spawn_position;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	preset = Get_Parameter("Preset_Name");
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(poker))
	{
		cost = -cost;
		spawn_position = Get_Vector3_Parameter("location");
		Commands->Give_Money(poker,(float)cost,0);
		Commands->Create_Object(preset,spawn_position);
	}
	else
	{
		Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(poker),poker);
	}
}

void JFW_Character_Buy_Poke_Sound::Poked(GameObject *obj,GameObject *poker)
{
	const char *preset;
	int x;
	int cost;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	preset = Get_Parameter("Preset_Name");
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(poker))
	{
		cost = -cost;
		Commands->Give_Money(poker,(float)cost,0);
		Change_Character(poker,preset);
	}
	else
	{
		Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(poker),poker);
	}
}

void JFW_Refill_Buy_Poke_Sound::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	int cost;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(poker))
	{
		cost = -cost;
		Commands->Give_Money(poker,(float)cost,0);
		Grant_Refill(poker);
	}
	else
	{
		Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(poker),poker);
	}
}

void JFW_Powerup_Buy_Poke_Sound::Poked(GameObject *obj,GameObject *poker)
{
	const char *preset;
	int x;
	int cost;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	preset = Get_Parameter("Preset_Name");
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(poker))
	{
		cost = -cost;
		Commands->Give_Money(poker,(float)cost,0);
		Grant_Powerup(poker,preset);
	}
	else
	{
		Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(poker),poker);
	}
}

void JFW_Vehicle_Buy_Poke_Sound::Poked(GameObject *obj,GameObject *poker)
{
	const char *preset;
	int x;
	int cost;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	preset = Get_Parameter("Preset_Name");
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(poker))
	{
		cost = -cost;
		Commands->Give_Money(poker,(float)cost,0);
		Create_Vehicle(preset,5.0,poker,x);
	}
	else
	{
		Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(poker),poker);
	}
}

void JFW_Group_Purchase_Poke_Sound::Created(GameObject *obj)
{
	cash = 0;
	enabled = true;
}

void JFW_Group_Purchase_Poke_Sound::Poked(GameObject *obj,GameObject *poker)
{
	if (enabled)
	{
		float money = Commands->Get_Money(poker);
		if (money >= Get_Float_Parameter("CashPerPerson"))
		{
			Commands->Give_Money(poker,Get_Float_Parameter("CashPerPerson"),0);
			cash += Get_Float_Parameter("CashPerPerson");
		}
		else
		{
			Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(poker),poker);
		}
		if (cash >= Get_Float_Parameter("TotalCash"))
		{
			cash = 0;
			enabled = false;
			Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message"),0,0);
		}
	}
}

void JFW_Group_Purchase_Poke_Sound::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&cash,4,1);
	Auto_Save_Variable(&enabled,1,2);
}

void JFW_Powerup_Buy_Poke_Timer_Sound::Poked(GameObject *obj,GameObject *poker)
{
	const char *preset;
	int x;
	int cost;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	if (enabled)
	{
		preset = Get_Parameter("Preset_Name");
		cost = Get_Int_Parameter("Cost");
		if (cost <= Commands->Get_Money(poker))
		{
			cost = -cost;
			Commands->Give_Money(poker,(float)cost,0);
			Grant_Powerup(poker,preset);
			enabled = false;
			int TimerNum = Get_Int_Parameter("TimerNum");
			float Time = Get_Float_Parameter("Time");
			Commands->Start_Timer(obj,this,Time,TimerNum);
		}
		else
		{
			Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(poker),poker);
		}
	}
}

void JFW_Powerup_Buy_Poke_Timer_Sound::Created(GameObject *obj)
{
	enabled = true;
}

void JFW_Powerup_Buy_Poke_Timer_Sound::Timer_Expired(GameObject *obj,int number)
{
	int TimerNum = Get_Int_Parameter("TimerNum");
	if (TimerNum == number)
	{
		enabled = true;
	}
}

void JFW_Powerup_Buy_Poke_Timer_Sound::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Weapon_Buy_Poke_Sound::Poked(GameObject *obj,GameObject *poker)
{
	const char *preset;
	int x;
	int cost;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	preset = Get_Parameter("Weapon_Name");
	cost = Get_Int_Parameter("Cost");
	if (cost <= Commands->Get_Money(poker))
	{
		cost = -cost;
		Commands->Give_Money(poker,(float)cost,0);
		Commands->Give_PowerUp(poker,preset,true);
	}
	else
	{
		Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(poker),poker);
	}
}

void JFW_Preset_Buy_Poke_Timer_Sound::Poked(GameObject *obj,GameObject *poker)
{
	Vector3 spawn_position;
	const char *preset;
	int cost;
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	if (enabled)
	{
		preset = Get_Parameter("Preset_Name");
		cost = Get_Int_Parameter("Cost");
		if (cost <= Commands->Get_Money(poker))
		{
			cost = -cost;
			spawn_position = Get_Vector3_Parameter("location");
			Commands->Give_Money(poker,(float)cost,0);
			Commands->Create_Object(preset,spawn_position);
			enabled = false;
			int TimerNum = Get_Int_Parameter("TimerNum");
			float Time = Get_Float_Parameter("Time");
			Commands->Start_Timer(obj,this,Time,TimerNum);
		}
		else
		{
			Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(poker),poker);
		}
	}
}

void JFW_Preset_Buy_Poke_Timer_Sound::Created(GameObject *obj)
{
	enabled = true;
}

void JFW_Preset_Buy_Poke_Timer_Sound::Timer_Expired(GameObject *obj,int number)
{
	int TimerNum = Get_Int_Parameter("TimerNum");
	if (TimerNum == number)
	{
		enabled = true;
	}
}

void JFW_Preset_Buy_Poke_Timer_Sound::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Preset_Buy_Poke_Custom_Sound::Poked(GameObject *obj,GameObject *poker)
{
	Vector3 spawn_position;
	const char *preset;
	int x;
	int cost;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	if (enabled)
	{
		preset = Get_Parameter("Preset_Name");
		cost = Get_Int_Parameter("Cost");
		if (cost <= Commands->Get_Money(poker))
		{
			cost = -cost;
			spawn_position = Get_Vector3_Parameter("location");
			Commands->Give_Money(poker,(float)cost,0);
			Commands->Create_Object(preset,spawn_position);
			enabled = false;
		}
		else
		{
			Commands->Create_Sound(Get_Parameter("Sound"),Commands->Get_Position(poker),poker);
		}
	}
}

void JFW_Preset_Buy_Poke_Custom_Sound::Created(GameObject *obj)
{
	enabled = true;
}

void JFW_Preset_Buy_Poke_Custom_Sound::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		enabled = true;
	}
}

void JFW_Preset_Buy_Poke_Custom_Sound::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Switch_Door_2::Poked(GameObject *obj,GameObject *poker)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,-1,false);
}

void JFW_Switch_Door_2::Timer_Expired(GameObject *obj,int number)
{
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Animation_Frame(obj),0,false);
}

void JFW_Switch_Lock_Door_2::Poked(GameObject *obj,GameObject *poker)
{
	if (Commands->Has_Key(poker,Get_Int_Parameter("key")))
	{
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,-1,false);
	}
}

void JFW_Switch_Lock_Door_2::Timer_Expired(GameObject *obj,int number)
{
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Animation_Frame(obj),0,false);
}

void JFW_Toggle_Door_2::Poked(GameObject *obj,GameObject *poker)
{
	if (!open)
	{
		open = true;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,-1,false);
	}
	else
	{
		open = false;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Animation_Frame(obj),0,false);
	}
}

void JFW_Toggle_Door_2::Created(GameObject *obj)
{
	open = false;
}

void JFW_Toggle_Door_2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&open,1,1);
}

void JFW_Toggle_Lock_Door_2::Poked(GameObject *obj,GameObject *poker)
{
	if (Commands->Has_Key(poker,Get_Int_Parameter("key")))
	{
		if (!open)
		{
			open = true;
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,-1,false);
		}
		else
		{
			open = false;
			Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Animation_Frame(obj),0,false);
		}
	}
}

void JFW_Toggle_Lock_Door_2::Created(GameObject *obj)
{
	open = false;
}

void JFW_Toggle_Lock_Door_2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&open,1,1);
}

void JFW_Switch_Door_Team_2::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNum"));
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,0,-1,false);
}

void JFW_Switch_Door_Team_2::Timer_Expired(GameObject *obj,int number)
{
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Animation_Frame(Commands->Find_Object(Get_Int_Parameter("ID"))),0,false);
}

void JFW_Toggle_Door_Team_2::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	if (!open)
	{
		open = true;
		Commands->Set_Animation(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Parameter("Animation"),false,0,0,-1,false);
	}
	else
	{
		open = false;
		Commands->Set_Animation(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Parameter("Animation"),false,0,Get_Animation_Frame(Commands->Find_Object(Get_Int_Parameter("ID"))),0,false);
	}
}

void JFW_Toggle_Door_Team_2::Created(GameObject *obj)
{
	open = false;
}

void JFW_Toggle_Door_Team_2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&open,1,1);
}

void JFW_Lock_Sound::Poked(GameObject *obj,GameObject *poker)
{
	Vector3 v;
	const char *c;
	if (Commands->Has_Key(poker,Get_Int_Parameter("Key")))
	{
		c = Get_Parameter("UnlockSound");
		v = Commands->Get_Position(obj);
		Commands->Create_Sound(c,v,obj);
	}
	else
	{
		c = Get_Parameter("LockSound");
		v = Commands->Get_Position(obj);
		Commands->Create_Sound(c,v,obj);
	}
}

void JFW_Poke_Send_Custom_Toggle::Created(GameObject *obj)
{
	toggle = false;
}

void JFW_Poke_Send_Custom_Toggle::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,x))
	{
		return;
	}
	if (!toggle)
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message1"),0,0);
		toggle = true;
	}
	else
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message2"),0,0);
		toggle = false;
	}
}

void JFW_Poke_Send_Custom_Toggle::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&toggle,1,1);
}

void JFW_Poke_Play_2D_Sound_Timer::Created(GameObject *obj)
{
	active = true;
}

void JFW_Poke_Play_3D_Sound_Timer::Created(GameObject *obj)
{
	active = true;
}

void JFW_Poke_Play_2D_Sound_Timer::Timer_Expired(GameObject *obj,int number)
{
	active = true;
}

void JFW_Poke_Play_3D_Sound_Timer::Timer_Expired(GameObject *obj,int number)
{
	active = true;
}

void JFW_Poke_Play_2D_Sound_Timer::Poked(GameObject *obj,GameObject *poker)
{
	if (!active)
	{
		return;
	}
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,team))
	{
		return;
	}
	Commands->Create_2D_Sound(Get_Parameter("Sound"));
	int TimerNum = Get_Int_Parameter("TimerNum");
	float Time = Get_Float_Parameter("Time");
	active = false;
	Commands->Start_Timer(obj,this,Time,TimerNum);
}

void JFW_Poke_Play_3D_Sound_Timer::Poked(GameObject *obj,GameObject *poker)
{
	if (!active)
	{
		return;
	}
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,team))
	{
		return;
	}
	Commands->Create_Sound(Get_Parameter("Sound"),Get_Vector3_Parameter("Position"),obj);
	int TimerNum = Get_Int_Parameter("TimerNum");
	float Time = Get_Float_Parameter("Time");
	active = false;
	Commands->Start_Timer(obj,this,Time,TimerNum);
}

void JFW_Poke_Play_2D_Sound_Timer::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&active,1,1);
}

void JFW_Poke_Play_3D_Sound_Timer::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&active,1,1);
}

void JFW_Escort_Poke::Created(GameObject *obj)
{
	follower = 0;
}

void JFW_Escort_Poke::Poked(GameObject *obj,GameObject *poker)
{
	if (Commands->Find_Object(follower) != poker)
	{
		follower = Commands->Get_ID(poker);
		Commands->Innate_Disable(obj);
		Vector3 v = Commands->Get_Position(obj);
		ActionParamsStruct params;
		params.Set_Basic(this,100,100);
		params.Set_Movement(poker,1.0,1.0);
		params.MoveFollow = true;
		Commands->Action_Goto(obj,params);
		if (Commands->Get_Player_Type(poker) == Get_Int_Parameter("Player_Type"))
		{
			Commands->Set_Shield_Type(obj,Get_Parameter("Shield"));
		}
	}
	else
	{
		Commands->Action_Reset(obj,100);
		if (Commands->Get_Player_Type(poker) == Get_Int_Parameter("Player_Type"))
		{
			Commands->Set_Shield_Type(obj,Get_Parameter("Shield2"));
		}
	}
}

ScriptRegistrant<JFW_Poke_Send_Custom> JFW_Poke_Send_Custom_Registrant("JFW_Poke_Send_Custom","Sound_Name:string,Activate_Message:int,Object_Id:int,Message:int,Param:int,Delay:float");
ScriptRegistrant<JFW_Pokeable_Item> JFW_Pokeable_Item_Registrant("JFW_Pokeable_Item","");
ScriptRegistrant<JFW_Character_Buy_Poke> JFW_Character_Buy_Poke_Registrant("JFW_Character_Buy_Poke","Preset_Name:string,Cost:int,Player_Type:int");
ScriptRegistrant<JFW_Refill_Buy_Poke> JFW_Refill_Buy_Poke_Registrant("JFW_Refill_Buy_Poke","Cost:int,Player_Type:int");
ScriptRegistrant<JFW_Vehicle_Buy_Poke> JFW_Vehicle_Buy_Poke_Registrant("JFW_Vehicle_Buy_Poke","Preset_Name:string,Cost:int,Player_Type:int");
ScriptRegistrant<JFW_Powerup_Buy_Poke> JFW_Powerup_Buy_Poke_Registrant("JFW_Powerup_Buy_Poke","Preset_Name:string,Cost:int,Player_Type:int");
ScriptRegistrant<JFW_Preset_Buy_Poke> JFW_Preset_Buy_Poke_Registrant("JFW_Preset_Buy_Poke","Preset_Name:string,Cost:int,location:vector3,Player_Type:int");
ScriptRegistrant<JFW_Group_Purchase_Poke> JFW_Group_Purchase_Poke_Registrant("JFW_Group_Purchase_Poke","CashPerPerson:float,TotalCash:float,ID:int,Custom:int");
ScriptRegistrant<JFW_Gate_Poke> JFW_Gate_Poke_Registrant("JFW_Gate_Poke","Player_Type:int,Time:float,TimerNum:int,ID:int,Animation:string");
ScriptRegistrant<JFW_Poke_Send_Custom_2> JFW_Poke_Send_Custom_2_Registrant("JFW_Poke_Send_Custom_2","Player_Type:int,ID:int,Message:int,Param:int");
ScriptRegistrant<JFW_Poke_Send_Custom_Preset> JFW_Poke_Send_Custom_Preset_Registrant("JFW_Poke_Send_Custom_Preset","Player_Type:int,ID:int,Message:int,Param:int,Preset:string");
ScriptRegistrant<JFW_Poke_Send_Custom_Keycard> JFW_Poke_Send_Custom_Keycard_Registrant("JFW_Poke_Send_Custom_Keycard","Player_Type:int,ID:int,Message:int,Param:int,Key:int");
ScriptRegistrant<JFW_Poke_Send_Custom_Cost> JFW_Poke_Send_Custom_Cost_Registrant("JFW_Poke_Send_Custom_Cost","Player_Type:int,ID:int,Message:int,Param:int,Cost:float");
ScriptRegistrant<JFW_Poke_Play_2D_Sound> JFW_Poke_Play_2D_Sound_Registrant("JFW_Poke_Play_2D_Sound","Player_Type:int,Sound:string");
ScriptRegistrant<JFW_Poke_Play_3D_Sound> JFW_Poke_Play_3D_Sound_Registrant("JFW_Poke_Play_3D_Sound","Player_Type:int,Sound:string,Position:vector3");
ScriptRegistrant<JFW_Powerup_Buy_Poke_Timer> JFW_Powerup_Buy_Poke_Timer_Registrant("JFW_Powerup_Buy_Poke_Timer","Preset_Name:string,Cost:int,Player_Type:int,Time:float,TimerNum:int");
ScriptRegistrant<JFW_Poke_Play_2D_Sound_Team> JFW_Poke_Play_2D_Sound_Team_Registrant("JFW_Poke_Play_2D_Sound_Team","Player_Type:int,Sound:string");
ScriptRegistrant<JFW_Poke_Play_3D_Sound_Team> JFW_Poke_Play_3D_Sound_Team_Registrant("JFW_Poke_Play_3D_Sound_Team","Player_Type:int,Sound:string,Position:vector3");
ScriptRegistrant<JFW_Poke_Play_2D_Sound_Player> JFW_Poke_Play_2D_Sound_Player_Registrant("JFW_Poke_Play_2D_Sound_Player","Player_Type:int,Sound:string");
ScriptRegistrant<JFW_Poke_Play_3D_Sound_Player> JFW_Poke_Play_3D_Sound_Player_Registrant("JFW_Poke_Play_3D_Sound_Player","Player_Type:int,Sound:string,Position:vector3");
ScriptRegistrant<JFW_Look_At_Location_Poke> JFW_Look_At_Location_Poke_Registrant("JFW_Look_At_Location_Poke","Player_Type:int,Location:vector3");
ScriptRegistrant<JFW_Look_At_Object_Poke> JFW_Look_At_Object_Poke_Registrant("JFW_Look_At_Object_Poke","Player_Type:int,ObjectID:int");
ScriptRegistrant<JFW_PCT> JFW_PCT_Registrant("JFW_PCT","Player_Type:int");
ScriptRegistrant<JFW_Weapon_Buy_Poke> JFW_Weapon_Buy_Poke_Registrant("JFW_Weapon_Buy_Poke","Weapon_Name:string,Cost:int,Player_Type:int");
ScriptRegistrant<JFW_Preset_Buy_Poke_Timer> JFW_Preset_Buy_Poke_Timer_Registrant("JFW_Preset_Buy_Poke_Timer","Preset_Name:string,Cost:int,location:vector3,Player_Type:int,Time:float,TimerNum:int");
ScriptRegistrant<JFW_Preset_Buy_Poke_Custom> JFW_Preset_Buy_Poke_Custom_Registrant("JFW_Preset_Buy_Poke_Custom","Preset_Name:string,Cost:int,location:vector3,Player_Type:int,Message:int");
ScriptRegistrant<JFW_Slot_Machine> JFW_Slot_Machine_Registrant("JFW_Slot_Machine","Cost:int,Winnings:int,Percent:int,Player_Type:int");
ScriptRegistrant<JFW_Slot_Machine_2> JFW_Slot_Machine_2_Registrant("JFW_Slot_Machine_2","Cost:int,Winnings:int,Percent:int,Player_Type:int,Animation:string,Sound:string");
ScriptRegistrant<JFW_Switch_Door> JFW_Switch_Door_Registrant("JFW_Switch_Door","ID:int,Animation:string,Time:float,TimerNum:int");
ScriptRegistrant<JFW_Switch_Lock_Door> JFW_Switch_Lock_Door_Registrant("JFW_Switch_Lock_Door","ID:int,Animation:string,Time:float,TimerNum:int,Key:int");
ScriptRegistrant<JFW_Toggle_Door> JFW_Toggle_Door_Registrant("JFW_Toggle_Door","ID:int,Animation:string");
ScriptRegistrant<JFW_Toggle_Lock_Door> JFW_Toggle_Lock_Door_Registrant("JFW_Toggle_Lock_Door","ID:int,Animation:string,Key:int");
ScriptRegistrant<JFW_Switch_Door_Team> JFW_Switch_Door_Team_Registrant("JFW_Switch_Door_Team","ID:int,Animation:string,Time:float,TimerNum:int,Player_Type:int");
ScriptRegistrant<JFW_Toggle_Door_Team> JFW_Toggle_Door_Team_Registrant("JFW_Toggle_Door_Team","ID:int,Animation:string,Player_Type:int");
ScriptRegistrant<JFW_Character_Buy_Poke_Sound> JFW_Character_Buy_Poke_Sound_Registrant("JFW_Character_Buy_Poke_Sound","Preset_Name:string,Cost:int,Player_Type:int,Sound:string");
ScriptRegistrant<JFW_Refill_Buy_Poke_Sound> JFW_Refill_Buy_Poke_Sound_Registrant("JFW_Refill_Buy_Poke_Sound","Cost:int,Player_Type:int,Sound:string");
ScriptRegistrant<JFW_Vehicle_Buy_Poke_Sound> JFW_Vehicle_Buy_Poke_Sound_Registrant("JFW_Vehicle_Buy_Poke_Sound","Preset_Name:string,Cost:int,Player_Type:int,Sound:string");
ScriptRegistrant<JFW_Powerup_Buy_Poke_Sound> JFW_Powerup_Buy_Poke_Sound_Registrant("JFW_Powerup_Buy_Poke_Sound","Preset_Name:string,Cost:int,Player_Type:int,Sound:string");
ScriptRegistrant<JFW_Preset_Buy_Poke_Sound> JFW_Preset_Buy_Poke_Sound_Registrant("JFW_Preset_Buy_Poke_Sound","Preset_Name:string,Cost:int,location:vector3,Player_Type:int,Sound:string");
ScriptRegistrant<JFW_Group_Purchase_Poke_Sound> JFW_Group_Purchase_Poke_Sound_Registrant("JFW_Group_Purchase_Poke_Sound","CashPerPerson:float,TotalCash:float,ID:int,Custom:int,Sound:string");
ScriptRegistrant<JFW_Powerup_Buy_Poke_Timer_Sound> JFW_Powerup_Buy_Poke_Timer_Sound_Registrant("JFW_Powerup_Buy_Poke_Timer_Sound","Preset_Name:string,Cost:int,Player_Type:int,Time:float,TimerNum:int,Sound:string");
ScriptRegistrant<JFW_Weapon_Buy_Poke_Sound> JFW_Weapon_Buy_Poke_Sound_Registrant("JFW_Weapon_Buy_Poke_Sound","Weapon_Name:string,Cost:int,Player_Type:int,Sound:string");
ScriptRegistrant<JFW_Preset_Buy_Poke_Timer_Sound> JFW_Preset_Buy_Poke_Timer_Sound_Registrant("JFW_Preset_Buy_Poke_Timer_Sound","Preset_Name:string,Cost:int,location:vector3,Player_Type:int,Time:float,TimerNum:int,Sound:string");
ScriptRegistrant<JFW_Preset_Buy_Poke_Custom_Sound> JFW_Preset_Buy_Poke_Custom_Sound_Registrant("JFW_Preset_Buy_Poke_Custom_Sound","Preset_Name:string,Cost:int,location:vector3,Player_Type:int,Message:int,Sound:string");
ScriptRegistrant<JFW_Switch_Door_2> JFW_Switch_Door_2_Registrant("JFW_Switch_Door_2","Animation:string,Time:float,TimerNum:int");
ScriptRegistrant<JFW_Switch_Lock_Door_2> JFW_Switch_Lock_Door_2_Registrant("JFW_Switch_Lock_Door_2","Animation:string,Time:float,TimerNum:int,Key:int");
ScriptRegistrant<JFW_Toggle_Door_2> JFW_Toggle_Door_2_Registrant("JFW_Toggle_Door_2","Animation:string");
ScriptRegistrant<JFW_Toggle_Lock_Door_2> JFW_Toggle_Lock_Door_2_Registrant("JFW_Toggle_Lock_Door_2","Animation:string,Key:int");
ScriptRegistrant<JFW_Switch_Door_Team_2> JFW_Switch_Door_Team_2_Registrant("JFW_Switch_Door_Team_2","Animation:string,Time:float,TimerNum:int,Player_Type:int");
ScriptRegistrant<JFW_Toggle_Door_Team_2> JFW_Toggle_Door_Team_2_Registrant("JFW_Toggle_Door_Team_2","Animation:string,Player_Type:int");
ScriptRegistrant<JFW_Lock_Sound> JFW_Lock_Sound_Registrant("JFW_Lock_Sound","Key:int,LockSound:string,UnlockSound:string");
ScriptRegistrant<JFW_Poke_Send_Custom_Toggle> JFW_Poke_Send_Custom_Toggle_Registrant("JFW_Poke_Send_Custom_Toggle","Player_Type:int,ID:int,Message1:int,Message2:int");
ScriptRegistrant<JFW_Poke_Play_2D_Sound_Timer> JFW_Poke_Play_2D_Sound_Timer_Registrant("JFW_Poke_Play_2D_Sound_Timer","Player_Type:int,Sound:string,Time:float,TimerNum:int");
ScriptRegistrant<JFW_Poke_Play_3D_Sound_Timer> JFW_Poke_Play_3D_Sound_Timer_Registrant("JFW_Poke_Play_3D_Sound_Timer","Player_Type:int,Sound:string,Position:vector3,Time:float,TimerNum:int");
ScriptRegistrant<JFW_Sidebar_PT> JFW_Sidebar_PT_Registrant("JFW_Sidebar_PT","Player_Type:int");
ScriptRegistrant<JFW_Escort_Poke> JFW_Escort_Poke_Registrant("JFW_Escort_Poke","Shield:string,Shield2:string,Player_Type:int");
