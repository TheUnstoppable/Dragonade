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
#include "VehicleGameObj.h"
#include "engine.h"
#include "iran_scripts.h"

void Iran_Beaconing_Prevention_Zone::Entered(GameObject *obj,GameObject *enter) 
{																		 
	Commands->Start_Timer(obj,this,0.1f,Commands->Get_ID(enter));
}

void Iran_Beaconing_Prevention_Zone::Timer_Expired(GameObject *obj,int number)
{
	GameObject* checkplayerobj = Commands->Find_Object(number);
	if ( IsInsideZone(obj,checkplayerobj))
	{	
		if (Get_Total_Bullets(checkplayerobj, "CnC_Weapon_NukeBeacon_Player"))
		{
			const char* currentweapon = Get_Current_Weapon(checkplayerobj);
			if (strcmp(currentweapon, "CnC_Weapon_NukeBeacon_Player") == 0 && Get_Current_Bullets(checkplayerobj) > 0) 
			{
				Commands->Select_Weapon(checkplayerobj,"");
				Commands->Select_Weapon(checkplayerobj,"CnC_Weapon_NukeBeacon_Player");
				
			}
		Commands->Start_Timer(obj,this,0.5,number);	
		}
		else if (Get_Total_Bullets(checkplayerobj, "CnC_Weapon_IonCannonBeacon_Player"))
		{
			const char* currentweapon = Get_Current_Weapon(checkplayerobj);
			if(strcmp(currentweapon,"CnC_Weapon_IonCannonBeacon_Player") == 0 && Get_Current_Bullets(checkplayerobj) > 0) 
			{
				Commands->Select_Weapon(checkplayerobj,"");
				Commands->Select_Weapon(checkplayerobj,"CnC_Weapon_IonCannonBeacon_Player");
			}
		Commands->Start_Timer(obj,this,0.5,number);	
		}		
	}
}

void Iran_Beaconing_Prevention_Zone::Remove_Weapon_Safely(GameObject *obj,const char *WeaponName)
{
	const char *CurrentWeapon = Get_Current_Weapon(obj);
	if (CurrentWeapon)
	{
		if (!_stricmp(CurrentWeapon,WeaponName))
		{
			if (Has_Weapon(obj,"Weapon_Pistol_Player"))
			{
				Commands->Select_Weapon(obj,"Weapon_Pistol_Player");
				Remove_Weapon(obj,WeaponName);
				Commands->Select_Weapon(obj,"Weapon_Pistol_Player");
			}
			else
			{
				Grant_Powerup(obj, "POW_Pistol_Player");
				Commands->Select_Weapon(obj,"Weapon_Pistol_Player");
				Remove_Weapon(obj,WeaponName);
				Commands->Select_Weapon(obj,"Weapon_Pistol_Player");
			}
		}
	}	
}

void Iran_Vehicle_All_Kill_Zone::Entered(GameObject *obj,GameObject *enter)
{

	if (stristr(Commands->Get_Preset_Name(enter),"Destroyed"))
	{
		saved = Commands->Get_ID(enter);
		Commands->Start_Timer(obj,this,2.5f,1);
	}
	else if (enter->As_VehicleGameObj())
	{
		int type = Commands->Get_Player_Type(enter);
		if ( type < 0 || type > 1 )
		{
			return;
		}
		saved = Commands->Get_ID(enter);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
	}
}

void Iran_Vehicle_All_Kill_Zone::Timer_Expired(GameObject *obj,int number)
{
	if (IsInsideZone(obj, Commands->Find_Object(saved)))
	{
		Commands->Apply_Damage(Commands->Find_Object(saved), 9999.0f, "Blamo", 0);
	}
}

void Iran_All_Kill_Zone::Entered(GameObject *obj,GameObject *enter)
{
	if (stristr(Commands->Get_Preset_Name(enter),"Destroyed"))
	{
		saved = Commands->Get_ID(enter);
		Commands->Start_Timer(obj,this,2.5f,1);
	}

	else if (enter)
	{
		int type = Commands->Get_Player_Type(enter);
		if ( type < 0 || type > 1 )
		{
			return;
		}
		if (Get_Fly_Mode(enter))
		{
			return;
		}
		saved = Commands->Get_ID(enter);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
	} 
}

void Iran_All_Kill_Zone::Timer_Expired(GameObject *obj,int number)
{
	if (IsInsideZone(obj, Commands->Find_Object(saved)))
	{
		Commands->Apply_Damage(Commands->Find_Object(saved), 9999.0f, "Blamo", 0);
	}
}

void Iran_Star_Kill_Zone::Entered(GameObject *obj,GameObject *enter)
{
	int type = Commands->Get_Player_Type(enter);
	if ( type < 0 || type > 1 )
	{
		return;
	}

	if (Get_Fly_Mode(enter))
	{
		return;
	}
	saved = Commands->Get_ID(enter);
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
}

void Iran_Star_Kill_Zone::Timer_Expired(GameObject *obj,int number)
{
	if (IsInsideZone(obj, Commands->Find_Object(saved)))
	{
		Commands->Apply_Damage(Commands->Find_Object(saved), 9999.0f, "Blamo", 0);
	}
}

void Iran_Vehicle_Ground_Kill_Zone::Entered(GameObject *obj,GameObject *enter)
{
	if (stristr(Commands->Get_Preset_Name(enter),"_Destroyed"))
	{
		saved = Commands->Get_ID(enter);
		Commands->Start_Timer(obj,this,2.5f,1);
	}

	else if (enter->As_VehicleGameObj())
	{
		if (Get_Vehicle_Mode(enter) != VEHICLE_TYPE_FLYING)
		{
			saved = Commands->Get_ID(enter);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
		}
	}
}

void Iran_Vehicle_Ground_Kill_Zone::Timer_Expired(GameObject *obj,int number)
{
	if (IsInsideZone(obj, Commands->Find_Object(saved)))
	{
		Commands->Apply_Damage(Commands->Find_Object(saved), 9999.0f, "Blamo", 0);
	}
}

void Iran_Vehicle_Flying_Kill_Zone::Entered(GameObject *obj,GameObject *enter)
{
	if (enter->As_VehicleGameObj())
	{
		if (Get_Vehicle_Mode(enter) == VEHICLE_TYPE_FLYING)
		{
			int type = Commands->Get_Player_Type(enter);
			if ( type < 0 || type > 1 )
			{
				return;
			}
			saved = Commands->Get_ID(enter);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
		}
	}
}

void Iran_Vehicle_Flying_Kill_Zone::Timer_Expired(GameObject *obj,int number)
{
	if (IsInsideZone(obj, Commands->Find_Object(saved)))
	{
		Commands->Apply_Damage(Commands->Find_Object(saved), 9999.0f, "Blamo", 0);
	}
}

void Iran_Invisible_Blocker::Created(GameObject *obj)
{
	Vector3 Position = Commands->Get_Position(obj);
	float Facing = Commands->Get_Facing(obj);

	Commands->Destroy_Object(obj);

	GameObject* Blocker = Commands->Create_Object("GDI_Ceiling_Gun_AGT", Position);
	Commands->Set_Facing(Blocker, Facing);
	Commands->Set_Model(Blocker,"dsp_torture_wal");

	Commands->Set_Player_Type(Blocker, -2);

	Commands->Set_Is_Visible(Blocker, false);
	Commands->Set_Is_Rendered(Blocker, false);
	Commands->Display_Health_Bar(Blocker, false);
}

void Iran_Invisible_Blocker_Visible::Created(GameObject *obj)
{
	Vector3 Position = Commands->Get_Position(obj);
	float Facing = Commands->Get_Facing(obj);

	Commands->Destroy_Object(obj);

	GameObject* Blocker = Commands->Create_Object("GDI_Ceiling_Gun_AGT", Position);
	Commands->Set_Facing(Blocker, Facing);
	Commands->Set_Model(Blocker,"dsp_torture_wal");

	Commands->Set_Player_Type(Blocker, -2);

}

ScriptRegistrant<Iran_Beaconing_Prevention_Zone> Iran_Beaconing_Prevention_Zone_Registrant("Iran_Beaconing_Prevention_Zone", "");

ScriptRegistrant<Iran_Star_Kill_Zone> Iran_Star_Kill_Zone_Registrant("Iran_Star_Kill_Zone", "Time:float");
ScriptRegistrant<Iran_All_Kill_Zone> Iran_All_Kill_Zone_Registrant("Iran_All_Kill_Zone", "Time:float");

ScriptRegistrant<Iran_Vehicle_All_Kill_Zone> Iran_Vehicle_All_Kill_Zone_Registrant("Iran_Vehicle_All_Kill_Zone", "Time:float");
ScriptRegistrant<Iran_Vehicle_Ground_Kill_Zone> Iran_Vehicle_Ground_Kill_Zone_Registrant("Iran_Vehicle_Ground_Kill_Zone", "Time:float");
ScriptRegistrant<Iran_Vehicle_Flying_Kill_Zone> Iran_Vehicle_Flying_Kill_Zone_Registrant("Iran_Vehicle_Flying_Kill_Zone", "Time:float");

ScriptRegistrant<Iran_Invisible_Blocker> Iran_Invisible_Blocker_Registrant("Iran_Invisible_Blocker", "");
ScriptRegistrant<Iran_Invisible_Blocker_Visible> Iran_Invisible_Blocker_Visible_Registrant("Iran_Invisible_Blocker_Visible", "");

