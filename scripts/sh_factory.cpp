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
#include "engine_tt.h"
#include "engine_player.h"
#include "engine_def.h"
#include "BaseControllerClass.h"
#include "engine_script.h"

enum
{
    CINEMATIC_SET_SLOT  = 10000,
    CINEMATIC_START     = 99000,
    SH_CVFB_DELIVERED   = 43000,
};

class SH_CinematicVehicleFactoryBypass: public ScriptImpClass
{
public:

    int hook_handle;
    int monitor_handle;
    unsigned int preset_id;

    SH_CinematicVehicleFactoryBypass(): hook_handle(0), monitor_handle(0), preset_id(0)
    {
    }

    void Created(GameObject* obj)
    {
        hook_handle = AddVehiclePurchaseHook(PurchaseHook, (const char*)this);
        monitor_handle = AddVehiclePurchaseMonHook(PurchaseMonitor, (const char*)this);
        preset_id = Get_Definition_ID(Get_Parameter("PresetName"));
    }

    void Destroyed(GameObject *obj)
    {
        RemoveVehiclePurchaseHook(hook_handle);
        RemoveVehiclePurchaseMonHook(monitor_handle);
        hook_handle = 0;
        monitor_handle = 0;
    }

    int PurchaseHook(BaseControllerClass* base, GameObject* purchaser, unsigned int cost, unsigned int preset)
    {
       if (preset != preset_id || base->Get_Player_Type() != Get_Player_Type(purchaser)) return -1; // not ours, process normally

       return -3; // process normally, but prevent creation
    }

    void PurchaseMonitor(BaseControllerClass* base, GameObject* purchaser, unsigned int cost, unsigned int preset, unsigned int purchase_status)
    {
        if (purchase_status != 0 || preset != preset_id || base->Get_Player_Type() != Get_Player_Type(purchaser)) return; // not ours

        auto owner = Owner();
        Vector3 position = Commands->Get_Position(owner);
        float facing = Commands->Get_Facing(owner);

        // create cinematic
        char cinematic_params[128];
        sprintf_s(cinematic_params, "%s,1", Get_Parameter("Cinematic"));

        auto cinematic = Commands->Create_Object("Invisible_Object", position);
        Commands->Set_Facing(cinematic, facing);
        Commands->Attach_Script(cinematic, "JFW_Cinematic", cinematic_params);

        // create vehicle at delivery site
        GameObject* vehicle = Commands->Create_Object(Get_Parameter("PresetName"), position);
        Commands->Set_Facing(vehicle, facing);
        Commands->Attach_Script(vehicle, "SH_CinematicVehicleDeliveryCallback", "");

        // load vehicle and purchaser into cinematic, start it
        Commands->Send_Custom_Event(owner, cinematic, CINEMATIC_SET_SLOT + 0, Commands->Get_ID(vehicle), 0);
        Commands->Send_Custom_Event(owner, cinematic, CINEMATIC_SET_SLOT + 1, Commands->Get_ID(purchaser), 0);
        Commands->Send_Custom_Event(owner, cinematic, CINEMATIC_START, 0, 0);
    }

    static int PurchaseHook(BaseControllerClass* base, GameObject* purchaser, unsigned int cost, unsigned int preset, const char* instance)
    {
        return ((SH_CinematicVehicleFactoryBypass*)instance)->PurchaseHook(base, purchaser, cost, preset);
    }

    static void PurchaseMonitor(BaseControllerClass* base, GameObject* purchaser, unsigned int cost, unsigned int preset, unsigned int purchase_status, const char* instance)
    {
        ((SH_CinematicVehicleFactoryBypass*)instance)->PurchaseMonitor(base, purchaser, cost, preset, purchase_status);
    }
};

class SH_CinematicVehicleDeliveryCallback: public ScriptImpClass
{
    void Created(GameObject *obj)
    {
        Commands->Attach_Script(obj, "SH_Invulnerable", "");
        Commands->Enable_Vehicle_Transitions(obj, false);
    }

    void Custom(GameObject* obj, int type, int param, GameObject* sender)
    {
        if (type == SH_CVFB_DELIVERED)
        {
            Remove_Script(obj, "SH_Invulnerable");
            Commands->Enable_Vehicle_Transitions(obj, true);
        }
    }
};

#define REGISTER_SCRIPT(name, params) ScriptRegistrant<name> name##Registrant(#name, params)

REGISTER_SCRIPT(SH_CinematicVehicleFactoryBypass, "PresetName:string,Cinematic:string");
REGISTER_SCRIPT(SH_CinematicVehicleDeliveryCallback, "");