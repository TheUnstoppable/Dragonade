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
#include "mdb.h"
#include "mdbevf.h"
#include "PurchaseSettingsDefClass.h"
#include "TeamPurchaseSettingsDefClass.h"
#include "ScriptableGameObj.h"
#include "ScriptableGameObjDef.h"

void MDB_Weapon_Scope::Created(GameObject *obj)
{
}

void MDB_Weapon_Scope::Timer_Expired(GameObject *obj,int number)
{
}

void MDB_Weapon_Scope::Register_Auto_Save_Variables()
{
}

void MDB_Weapon_Scope_Global::Created(GameObject *obj)
{
}

void MDB_Weapon_Scope_Global::ObjectCreateHook(GameObject *obj)
{
}

void MDB_Change_Spawn_Char_Custom::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		if (param == 0)
		{
			Set_Nod_Soldier_Name(Get_Parameter("Character"));
		}
		else
		{
			Set_GDI_Soldier_Name(Get_Parameter("Character"));
		}
	}
}

void MDB_Change_Spawn_Char_Timer::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
}

void MDB_Change_Spawn_Char_Timer::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1)
	{
		if (Get_Int_Parameter("Team") == 0)
		{
			Set_Nod_Soldier_Name(Get_Parameter("Character"));
		}
		else
		{
			Set_GDI_Soldier_Name(Get_Parameter("Character"));
		}
	}
}

void MDB_ConYard::Created(GameObject *obj)
{
	Time = Get_Float_Parameter("Interval");
	Amount = Get_Float_Parameter("Heal_Amount");
	Self = Get_Bool_Parameter("Repair_Self");
	PMode = Get_Int_Parameter("Power_Mode");
	Commands->Start_Timer(obj,this,Time,1);
}

void MDB_ConYard::Timer_Expired(GameObject *obj, int number)
{
	Repair_All_Buildings_By_Team(Get_Object_Type(obj),Self?0:Commands->Get_ID(obj),Amount);
	Commands->Start_Timer(obj,this,Time,1);
}

void MDB_ConYard::Custom(GameObject *obj, int type, int param, GameObject *sender) 
{
	if (type == CUSTOM_EVENT_BUILDING_POWER_CHANGED) 
	{
		if (!param)
		{
			if (PMode == 0) 
			{
				Amount = 0.0f;
			}
			else if (PMode == 2)
			{
				Amount = (Amount*Get_Float_Parameter("Power_Amount"));
			}
			else if (PMode == 3)
			{
				Time = (Time*Get_Float_Parameter("Power_Amount"));
			}
		}
		else 
		{
			Time = Get_Float_Parameter("Interval");
			Amount = Get_Float_Parameter("Heal_Amount");
		}
	}
	else if (type == CUSTOM_EVENT_BUILDING_REVIVED)
	{
		Amount = Get_Float_Parameter("Heal_Amount");
	}
}

void MDB_ConYard::Killed(GameObject *obj,GameObject *killer)
{
	Amount = 0.0f;
}

void MDB_ConYard::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&PMode,4,1);
	Auto_Save_Variable(&Time,4,2);
	Auto_Save_Variable(&Amount,4,3);
	Auto_Save_Variable(&Self,1,4);
}

void MDB_Send_Custom_Enemy_Seen::Created(GameObject *obj)
{
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
}

void MDB_Send_Custom_Enemy_Seen::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	GameObject *Temp = Commands->Find_Object(Get_Int_Parameter("ID"));
	if (!Temp)
	{
		Commands->Destroy_Object(obj);
	}
	else
	{
		Commands->Send_Custom_Event(enemy,Temp,Get_Int_Parameter("Message"),0,0);
	}
}

void MDB_Water_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (!Is_Script_Attached(enterer,"MDB_Water_Unit"))
	{
		Commands->Apply_Damage(enterer,Get_Float_Parameter("Damage"),Get_Parameter("Warhead"),0);
	}
}

void MDB_Vehicle_Limit::Created(GameObject *obj)
{
	char Limit[20];
	sprintf(Limit,"vlimit %d",Get_Int_Parameter("Limit"));
	Console_Input(Limit);
	Destroy_Script();
}

void MDB_Mine_Limit::Created(GameObject *obj)
{
	char Limit[20];
	int Num = Get_Int_Parameter("Limit");
	if (Num > 127) Num = 127;
	sprintf(Limit,"mlimit %d",Num);
	Console_Input(Limit);
	Destroy_Script();
}

void MDB_Unit_Limit::Created(GameObject *obj)
{
	Position = ~0u;
	int team = Get_Object_Type(obj);
	if (team < 0 || team >= 2)
	{
		Destroy_Script();
		return;
	}
	Team = (PurchaseSettingsDefClass::TEAM)!team;

	uint def_id = obj->Get_Definition().Get_ID();
	for (int i = 0; i < 7; ++i)
	{
		PurchaseSettingsDefClass* p = PurchaseSettingsDefClass::Find_Definition((PurchaseSettingsDefClass::TYPE)i, Team);
		if (!p) continue;

		for (int position = 0; position < 10; ++position)
		{
			Definitions[0] = p->Get_Definition(position);
			bool is_match = Definitions[0] == def_id;
			if (!is_match)
			{
				for (int alt_def = 0; alt_def < 3; ++alt_def)
				{
					Definitions[alt_def + 1] = p->Get_Alt_Definition(position, alt_def);
					if (Definitions[alt_def + 1] == def_id)
					{
						is_match = true;
					}
				}
			}

			if (is_match)
			{
				if (Verify_Preset_Limit(2, Get_Int_Parameter("Limit"), Definitions, 4))
				{
					if (ExpVehFac[team] && ExpVehFac[team]->Loaded && obj->As_VehicleGameObj())
					{
						UnitInfo* info = ExpVehFac[team]->Get_Unit_Info(def_id);
						info->DMode = (info->DMode == DISABLED_EVF) ? DISABLED_BOTH : DISABLED_UNIT;
					}
					p->Set_Disabled(position, true);
				}

				Free = false;
				Position = position;
				Type = (PurchaseSettingsDefClass::TYPE)i;
				return;
			}
		}
	}

	TeamPurchaseSettingsDefClass* t = TeamPurchaseSettingsDefClass::Get_Definition((TeamPurchaseSettingsDefClass::TEAM)Team);
	for (int i = 0; i < 4; ++i)
	{
		if ((uint)t->Get_Enlisted_Definition(i) == def_id)
		{
			Free = true;
			Position = i;

			if (Verify_Preset_Limit(2, Get_Int_Parameter("Limit"), &def_id, 4))
			{
				t->Set_Disabled(Position, true);
			}
			return;
		}
	}

	Destroy_Script();
}

void MDB_Unit_Limit::ReEnable()
{
	if (Position == ~0u) return;
	GameObject* obj = Owner();

	if (!Verify_Preset_Limit(2, Get_Int_Parameter("Limit"), Definitions, 4, obj))
	{
		if (Free)
		{
			TeamPurchaseSettingsDefClass* t = TeamPurchaseSettingsDefClass::Get_Definition((TeamPurchaseSettingsDefClass::TEAM)Team);
			t->Set_Disabled(Position, false);
		}
		else
		{
			PurchaseSettingsDefClass* p = PurchaseSettingsDefClass::Find_Definition(Type, Team);
			
			bool enable_preset = true;
			int evf_team = !Team;
			if (ExpVehFac[evf_team] && ExpVehFac[evf_team]->Loaded && obj->As_VehicleGameObj())
			{
				UnitInfo* info = ExpVehFac[evf_team]->Get_Unit_Info(Commands->Get_Preset_ID(obj));
				if (info)
				{
					if (info->DMode == DISABLED_UNIT) info->DMode = NOT_DISABLED;
					else if (info->DMode == DISABLED_BOTH) 
					{
						info->DMode = DISABLED_EVF;
						enable_preset = false;
					}
				}
			}

			if (enable_preset)
			{
				p->Set_Disabled(Position, false);
			}
		}
	}
}

void MDB_Unit_Limit::Killed(GameObject *obj,GameObject *killer)
{
	ReEnable();
}

void MDB_Unit_Limit::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&Position,4,1);
	Auto_Save_Variable(&Type,4,2);
	Auto_Save_Variable(&Team,4,3);
	Auto_Save_Variable(&Free,1,6);
	Auto_Save_Variable(Definitions, sizeof(Definitions), 7);
}

void MDB_Send_Custom_On_Key::Created(GameObject *obj)
{
	InstallHook(Get_Parameter("Key"),obj);
}

void MDB_Send_Custom_On_Key::KeyHook()
{
	Commands->Send_Custom_Event(Owner(),Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message"),0,0);
	if (Get_Int_Parameter("Once"))
	{
		Destroy_Script();
	}
}

void MDB_Remote_Controlled_Vehicle::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,1.0f,1);
	BotID = 0;
}

void MDB_Remote_Controlled_Vehicle::Timer_Expired(GameObject *obj,int number)
{
	GameObject *Own = Get_Vehicle_Owner(obj);
	if (Own)
	{
		Send_Message_Player(Own,255,255,255,"Verifying encryption protocol...");
		char msg[256];
		const char *c = Get_Translated_Preset_Name(obj);
		sprintf(msg,"Remote connection to %s established",c);
		delete[] c;
		Send_Message_Player(Own,255,255,255,msg);
		Vector3 Pos = Commands->Get_Position(Own);
		float Facing = Commands->Get_Facing(Own);
		Commands->Set_Position(Own,Commands->Get_Position(obj));
		Commands->Enable_Engine(obj,false);
		GameObject *Bot = Commands->Create_Object(Commands->Get_Preset_Name(Own),Pos);
		BotID = Commands->Get_ID(Bot);
		Soldier_Transition_Vehicle(Own);
		Commands->Set_Facing(Bot,Facing);
		Commands->Set_Health(Bot,Commands->Get_Health(Own));
		Commands->Set_Shield_Strength(Bot,Commands->Get_Shield_Strength(Own));
		char params[20];
		sprintf(params,"%d",Commands->Get_ID(Own));
		Commands->Attach_Script(Bot,"MDB_Remote_Controlled_Vehicle_Bot",params);
	}
}

void MDB_Remote_Controlled_Vehicle::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_EXITED && BotID)
	{
		GameObject *Bot = Commands->Find_Object(BotID);
		Commands->Set_Model(Bot,"null");
		Commands->Attach_To_Object_Bone(sender,Bot,"Origin");
		Commands->Send_Custom_Event(obj,Bot,72386,0,0.5f);
		BotID = 0;
		Commands->Enable_Vehicle_Transitions(obj,false);
	}
}

void MDB_Remote_Controlled_Vehicle::Destroyed(GameObject *obj)
{
	GameObject *Bot = Commands->Find_Object(BotID);
	GameObject *Player = Get_Vehicle_Occupant(obj,0);
	Commands->Set_Model(Bot,"null");
	Commands->Attach_To_Object_Bone(Player,Bot,"Origin");
	Commands->Send_Custom_Event(obj,Bot,72386,0,0.5f);
	char msg[256];
	const char *c = Get_Translated_Preset_Name(obj);
	sprintf(msg,"Remote connection to %s lost",c);
	delete[] c;
	Send_Message_Player(Player,255,255,255,msg);
}

void MDB_Remote_Controlled_Vehicle::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&BotID,4,1);
}

void MDB_Remote_Controlled_Vehicle_Bot::Created(GameObject *obj)
{
	ID = Get_Int_Parameter("ID");
}

void MDB_Remote_Controlled_Vehicle_Bot::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (Commands->Get_Health(obj) > 0)
	{
		GameObject *Player = Commands->Find_Object(ID);
		if (!Player)
		{
			Commands->Destroy_Object(obj);
			return;
		}
		Commands->Set_Health(Player,Commands->Get_Health(obj));
		Commands->Set_Shield_Strength(Player,Commands->Get_Shield_Strength(obj));
	}
}

void MDB_Remote_Controlled_Vehicle_Bot::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *Player = Commands->Find_Object(ID);
	Soldier_Transition_Vehicle(Player);
	Commands->Apply_Damage(Player,99999.0f,"Death",killer);
}

void MDB_Remote_Controlled_Vehicle_Bot::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 72386)
	{
		Commands->Destroy_Object(obj);
	}
}

void MDB_Remote_Controlled_Vehicle_Bot::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ID,4,1);
	Auto_Save_Variable(&VehID,4,2);
}

void MDB_Set_Ammo_Granted_Weapon_On_Pickup::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		const char *Weapon = Get_Powerup_Weapon_By_Obj(obj);
		int Ammo = Get_Int_Parameter("Ammo");
		int Clip_Ammo = Get_Int_Parameter("Clip_Ammo");
		if (Ammo != -2)
		{
			Set_Bullets(sender,Weapon,Ammo);
		}
		if (Clip_Ammo != -2)
		{
			Set_Clip_Bullets(sender,Weapon,Clip_Ammo);
		}
	}
}

void MDB_Set_Ammo_Current_Weapon_On_Pickup::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		int Ammo = Get_Int_Parameter("Ammo");
		int Clip_Ammo = Get_Int_Parameter("Clip_Ammo");
		if (Ammo != -2)
		{
			Set_Current_Bullets(sender,Ammo);
		}
		if (Clip_Ammo != -2)
		{
			Set_Current_Clip_Bullets(sender,Clip_Ammo);
		}
	}
}

void MDB_Set_Ammo_On_Pickup::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		const char *Weapon = Get_Parameter("Weapon");
		int Ammo = Get_Int_Parameter("Ammo");
		int Clip_Ammo = Get_Int_Parameter("Clip_Ammo");
		if (Ammo != -2)
		{
			Set_Bullets(sender,Weapon,Ammo);
		}
		if (Clip_Ammo != -2)
		{
			Set_Clip_Bullets(sender,Weapon,Clip_Ammo);
		}
	}
}

ScriptRegistrant<MDB_Weapon_Scope> MDB_Weapon_Scope_Registrant("MDB_Weapon_Scope","Default_Scope=-1:int");
ScriptRegistrant<MDB_Weapon_Scope_Global> MDB_Weapon_Scope_Global_Registrant("MDB_Weapon_Scope_Global","Default_Scope=-1:int");
ScriptRegistrant<MDB_Change_Spawn_Char_Custom> MDB_Change_Spawn_Char_Custom_Registrant("MDB_Change_Spawn_Char_Custom","Message=0:int,Character:string");
ScriptRegistrant<MDB_Change_Spawn_Char_Timer> MDB_Change_Spawn_Char_Timer_Registrant("MDB_Change_Spawn_Char_Timer","Time=0:float,Team=-1:int,Character:string");
ScriptRegistrant<MDB_ConYard> MDB_ConYard_Registrant("MDB_ConYard","Interval=5.0:float,Heal_Amount=5.0:float,Power_Mode=1:int,Power_Amount=0.0:float,Repair_Self=0:int");
ScriptRegistrant<MDB_Send_Custom_Enemy_Seen> MDB_Send_Custom_Enemy_Seen_Registrant("MDB_Send_Custom_Enemy_Seen","Message=0:int,ID=0:int");
ScriptRegistrant<MDB_Water_Zone> MDB_Water_Zone_Registrant("MDB_Water_Zone","Damage=0.0:float,Warhead=None:string");
ScriptRegistrant<MDB_Water_Unit> MDB_Water_Unit_Registrant("MDB_Water_Unit","");
ScriptRegistrant<MDB_Vehicle_Limit> MDB_Vehicle_Limit_Registrant("MDB_Vehicle_Limit","Limit=7:int");
ScriptRegistrant<MDB_Mine_Limit> MDB_Mine_Limit_Registrant("MDB_Mine_Limit","Limit=30:int");
ScriptRegistrant<MDB_Unit_Limit> MDB_Unit_Limit_Registrant("MDB_Unit_Limit","Limit=1:int,Sidebar=0:int");
ScriptRegistrant<MDB_Send_Custom_On_Key> MDB_Send_Custom_On_Key_Registrant("MDB_Send_Custom_On_Key","Key=Key:string,ID=0:int,Message=0:int,Once=1:int");
ScriptRegistrant<MDB_Remote_Controlled_Vehicle> MDB_Remote_Controlled_Vehicle_Registrant("MDB_Remote_Controlled_Vehicle","");
ScriptRegistrant<MDB_Remote_Controlled_Vehicle_Bot> MDB_Remote_Controlled_Vehicle_Bot_Registrant("MDB_Remote_Controlled_Vehicle_Bot","ID=0:int");
ScriptRegistrant<MDB_Sidebar_Key> MDB_Sidebar_Key_Registrant("MDB_Sidebar_Key","Key=Sidebar:string,Start_Enabled=0:int,Enable_Custom=0:int,Disable_Custom=0:int");
ScriptRegistrant<MDB_Set_Ammo_Granted_Weapon_On_Pickup> MDB_Set_Ammo_Granted_Weapon_On_Pickup_Registrant("MDB_Set_Ammo_Granted_Weapon_On_Pickup","Ammo=-2:int,Clip_Ammo=-2:int");
ScriptRegistrant<MDB_Set_Ammo_Current_Weapon_On_Pickup> MDB_Set_Ammo_Current_Weapon_On_Pickup_Registrant("MDB_Set_Ammo_Current_Weapon_On_Pickup","Weapon=WeaponName:string,Ammo=-2:int,Clip_Ammo=-2:int");
ScriptRegistrant<MDB_Set_Ammo_On_Pickup> MDB_Set_Ammo_On_Pickup_Registrant("MDB_Set_Ammo_On_Pickup","Ammo=-2:int,Clip_Ammo=-2:int");
