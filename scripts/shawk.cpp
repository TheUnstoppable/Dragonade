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
#include "shawk.h"
#include "VehicleGameObj.h"
#include "engine_tt.h"
#include "C4GameObj.h"

void SH_PCT_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		if (!Commands->Get_Player_Type(sender))
		{
			Display_NOD_Player_Terminal_Player(sender);
		}
		else
		{
			Display_GDI_Player_Terminal_Player(sender);
		}
	}
}

void SH_PCT_Powerup::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		if (!Commands->Get_Player_Type(sender))
		{
			Display_NOD_Player_Terminal_Player(sender);
		}
		else
		{
			Display_GDI_Player_Terminal_Player(sender);
		}
	}
}

void SH_Spawn_Difficulty::Created(GameObject *obj)
{
	int level = Commands->Get_Difficulty_Level();
	if (level == 1)
	{
		if (Get_Int_Parameter("ObjectEasyEnabled") == 1)
		{
			GameObject *newObj = Commands->Create_Object(Get_Parameter("ObjectEasy"),Commands->Get_Position(obj));
			Commands->Set_Facing(newObj,Commands->Get_Facing(obj));
		}
	}
	else if (level == 2) 
	{
		if (Get_Int_Parameter("ObjectMediumEnabled") == 1)
		{
			GameObject *newObj = Commands->Create_Object(Get_Parameter("ObjectMedium"),Commands->Get_Position(obj));
			Commands->Set_Facing(newObj,Commands->Get_Facing(obj));
		}
	}
	else if (level == 3)
	{
		if (Get_Int_Parameter("ObjectHardEnabled") == 1)
		{
			GameObject *newObj = Commands->Create_Object(Get_Parameter("ObjectHard"),Commands->Get_Position(obj));
			Commands->Set_Facing(newObj,Commands->Get_Facing(obj));
		}
	}
}

class SH_PTHidePresetOnDeath: public ScriptImpClass
{
	void Killed(GameObject *obj, GameObject *killer)
	{
		obj;
		killer;
		char* presets = newstr(Get_Parameter("Presets"));
		char* token = strtok(presets, ";");
		int type = Get_Int_Parameter("Player_Type");
		while (token != NULL)
		{
			Hide_Preset_By_Name(type, token, true);
			token = strtok (NULL, ";");
		}
		delete[] presets;
		Destroy_Script();
	};
};

class SH_VehicleStealthedWhileEmpty: public ScriptImpClass
{
	void Created(GameObject* obj)
	{
		VehicleGameObj* vehicle_obj = obj->As_VehicleGameObj();
		if (!vehicle_obj) return;
		vehicle_obj->Set_Allow_Stealth_While_Empty(true);
		Destroy_Script();
	}
};

class SH_VehicleCanBeStolen: public ScriptImpClass 
{ 
	void Created(GameObject* obj)
	{
		if (VehicleGameObj* vobj = obj->As_VehicleGameObj()) vobj->Set_Can_Be_Stolen(true);
		Destroy_Script();
	}
};

class SH_SoldierCanStealVehicles: public ScriptImpClass
{
	void Created(GameObject* obj)
	{
		if (SoldierGameObj* sobj = obj->As_SoldierGameObj()) sobj->Set_Can_Steal_Vehicles(true);
		Destroy_Script();
	}
};

class SH_SoldierCantDriveVehicles: public ScriptImpClass
{
	void Created(GameObject* obj)
	{
		if (SoldierGameObj* sobj = obj->As_SoldierGameObj()) sobj->Set_Can_Drive_Vehicles(false);
		Destroy_Script();
	}
};

class SH_HurtUnitTimer: public ScriptImpClass
{
	float time;
	const char* warhead;
	float amount;

	void Created(GameObject* obj)
	{
		time = Get_Float_Parameter("Time");
		warhead = Get_Parameter("Warhead");
		amount = Get_Float_Parameter("Amount");

		Commands->Start_Timer(obj, this, time, 0);
	}

	void Timer_Expired(GameObject* obj, int id)
	{
		Commands->Apply_Damage(obj, amount, warhead, nullptr);
		Commands->Start_Timer(obj, this, time, 0);
	}

	void Custom(GameObject* obj, int type, int param, GameObject* sender)
	{
		if (type == 0xFEEDFEED && param == 0xDEADDEAD) Destroy_Script();
	}

	void Load_Data(ScriptLoader& loader)
	{
		time = Get_Float_Parameter("Time");
		warhead = Get_Parameter("Warhead");
		amount = Get_Float_Parameter("Amount");
	}
};

class SH_HurtUnitTimerZone: public ScriptImpClass
{
	void Entered(GameObject* obj, GameObject* referer)
	{
		char param_string[128];
		sprintf_s(param_string, "%f,%s,%f", Get_Float_Parameter("Time"), Get_Parameter("Warhead"), Get_Float_Parameter("Amount"));
		Commands->Attach_Script(referer, "SH_HurtUnitTimer", param_string);
	}

	void Exited(GameObject* obj, GameObject* referer)
	{
		Commands->Send_Custom_Event(obj, referer, 0xFEEDFEED, 0xDEADDEAD, 0);
	}
};

class SH_DangerWarning: public ScriptImpClass
{
	bool enabled;
	float r,g,b,a;
	float time;
	float in_time, out_time;

public:
	void Created(GameObject* obj)
	{
		enabled = true;
		time = Get_Float_Parameter("Time");
		in_time = Get_Float_Parameter("TransitionIn");
		out_time = Get_Float_Parameter("TransitionOut");
		int res = sscanf_s(Get_Parameter("Color"),"%f/%f/%f/%f", &r, &g, &b, &a);
		if (res != 4)
		{
			Destroy_Script();
			return;
		}

		Commands->Start_Timer(obj, this, time, 0);
		Set_Screen_Fade_Color_Player(obj, r, g, b, 0);
		Set_Screen_Fade_Opacity_Player(obj, 0, 0);
	}

	void Timer_Expired(GameObject* obj, int id)
	{
		if (id)
		{
			Commands->Start_Timer(obj, this, time, 0);
			Set_Screen_Fade_Opacity_Player(obj, 0, out_time);
		}
		else if (enabled)
		{
			Commands->Start_Timer(obj, this, time, 1);
			Set_Screen_Fade_Opacity_Player(obj, a, in_time);
		}
		else Destroy_Script();
	}

	void Custom(GameObject* obj, int type, int param, GameObject* sender)
	{
		if (type == 0xAAAAAAAA && param == 0xEEEEEEEE) enabled = false;
	}
	
	void Destroyed(GameObject *obj)
	{
		enabled = false;
		Set_Screen_Fade_Opacity_Player(obj,0.0f,0.0f);
	}

	void Register_Auto_Save_Variables()
	{
		Auto_Save_Variable(&enabled, sizeof(enabled), 1);
	}

	void Load_Data(ScriptLoader& loader)
	{
		time = Get_Float_Parameter("Time");
		in_time = Get_Float_Parameter("TransitionIn");
		out_time = Get_Float_Parameter("TransitionOut");
		int res = sscanf_s(Get_Parameter("Color"),"%f/%f/%f/%f", &r, &g, &b, &a);
		if (res != 4) Destroy_Script();
	}
};

class SH_DangerZone: public ScriptImpClass
{
	void Entered(GameObject* obj, GameObject* referer)
	{
		char param_string[128];
		sprintf_s(param_string, "%f,%f,%f,%s", Get_Float_Parameter("Time"), Get_Float_Parameter("TransitionIn"), Get_Float_Parameter("TransitionOut"), Get_Parameter("Color"));
		Commands->Attach_Script(referer, "SH_DangerWarning", param_string);
	}

	void Exited(GameObject* obj, GameObject* referer)
	{
		Commands->Send_Custom_Event(obj, referer, 0xAAAAAAAA, 0xEEEEEEEE, 0);
	}
};

class SH_C4FreeZone: public ScriptImpClass
{
	void Entered(GameObject* obj, GameObject* referer)
	{
		PhysicalGameObj* physical_obj = referer->As_PhysicalGameObj();
		if (physical_obj)
		{
			C4GameObj* c4_obj = physical_obj->As_C4GameObj();
			if (c4_obj)
			{
				c4_obj->Defuse();
			}
		}
	}
};

class SH_VehicleAttachedC4FreeZone: public ScriptImpClass
{
	void Entered(GameObject* obj, GameObject* referer)
	{
		PhysicalGameObj* physical_obj = referer->As_PhysicalGameObj();
		if (physical_obj)
		{
			C4GameObj* c4_obj = physical_obj->As_C4GameObj();
			if (c4_obj)
			{
				GameObject* stuck_object = c4_obj->Get_Stuck_Object();
				if (stuck_object && stuck_object->As_VehicleGameObj())
				{
					c4_obj->Defuse();
				}
			}
		}
	}
};

class SH_VTOLAttachedC4FreeZone: public ScriptImpClass
{
	void Entered(GameObject* obj, GameObject* referer)
	{
		PhysicalGameObj* physical_obj = referer->As_PhysicalGameObj();
		if (physical_obj)
		{
			C4GameObj* c4_obj = physical_obj->As_C4GameObj();
			if (c4_obj)
			{
				GameObject* stuck_object = c4_obj->Get_Stuck_Object();
				if (stuck_object && stuck_object->As_VehicleGameObj() && stuck_object->As_PhysicalGameObj()->Peek_Physical_Object()->As_VTOLVehicleClass())
				{
					c4_obj->Defuse();
				}
			}
		}
	}
};

class SH_Invulnerable: public ScriptImpClass
{
public:
    ArmorType original_skin;
    ArmorType original_armor;

    void Created(GameObject* obj)
    {
        auto damageable_obj = obj->As_DamageableGameObj();
        if (!damageable_obj)
        {
            Destroy_Script();
            return;
        }

        auto defense_obj = damageable_obj->Get_Defense_Object();
        original_skin = defense_obj->Get_Skin();
        original_armor = defense_obj->Get_Shield_Type();

        Set_Skin(obj, "Blamo");
        Commands->Set_Shield_Type(obj, "Blamo");
    }

    void Detach(GameObject* obj)
    {
        auto damageable_obj = obj->As_DamageableGameObj();
        if (damageable_obj)
        {
            auto defense_obj = damageable_obj->Get_Defense_Object();
            defense_obj->Set_Skin(original_skin);
            defense_obj->Set_Shield_Type(original_armor);
        }

        ScriptImpClass::Detach(obj);
    }
};

class SH_ResetDoorKeysOnCreated: public ScriptImpClass
{
    void Created(GameObject* obj)
    {
        for (int i = 0; i < 32; ++i) Commands->Grant_Key(obj, i, false);

        char* keys = newstr(Get_Parameter("Keys"));
        char* token = strtok(keys, ";");
        while (token != NULL)
        {
            int key;
            if (sscanf(token, "%d", &key) == 1)
            {
                Commands->Grant_Key(obj, key, true);
            }
            token = strtok(NULL, ";");
        }
        delete[] keys;
    }
};

#define REGISTER_SCRIPT(name, params) ScriptRegistrant<name> name##Registrant(#name, params)

ScriptRegistrant<SH_PCT_Powerup> SH_PCT_Powerup_Registrant("SH_PCT_Powerup","");
ScriptRegistrant<SH_PCT_Custom> SH_PCT_On_Custom_Registrant("SH_PCT_On_Custom","Message:int");
ScriptRegistrant<SH_Spawn_Difficulty> SH_Spawn_Difficulty_Created("SH_Spawn_Difficulty","ObjectEasy:string,ObjectEasyEnabled:int,ObjectMedium:string,ObjectMediumEnabled:int,ObjectHard:string,ObjectHardEnabled:int");
ScriptRegistrant<SH_PTHidePresetOnDeath> SH_PTHidePresetOnDeath_Registant("SH_PTHidePresetOnDeath","Presets:string,Player_Type:int");
REGISTER_SCRIPT(SH_VehicleStealthedWhileEmpty, "");
REGISTER_SCRIPT(SH_VehicleCanBeStolen, "");
REGISTER_SCRIPT(SH_SoldierCanStealVehicles, "");
REGISTER_SCRIPT(SH_SoldierCantDriveVehicles, "");
REGISTER_SCRIPT(SH_HurtUnitTimer, "Time:float,Warhead:string,Amount:float");
REGISTER_SCRIPT(SH_HurtUnitTimerZone, "Time:float,Warhead:string,Amount:float");
REGISTER_SCRIPT(SH_DangerWarning, "Time:float,TransitionIn:float,TransitionOut:float,Color:string");
REGISTER_SCRIPT(SH_DangerZone, "Time:float,TransitionIn:float,TransitionOut:float,Color:string");
REGISTER_SCRIPT(SH_C4FreeZone, "");
REGISTER_SCRIPT(SH_VehicleAttachedC4FreeZone, "");
REGISTER_SCRIPT(SH_VTOLAttachedC4FreeZone, "");
REGISTER_SCRIPT(SH_Invulnerable, "");
REGISTER_SCRIPT(SH_ResetDoorKeysOnCreated, "Keys:string");
