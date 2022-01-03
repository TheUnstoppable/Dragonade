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
#include "tfx.h"

void TFX_Spawn_When_Killed::Killed(GameObject *obj,GameObject *killer)
{
	float facing = Commands->Get_Facing(obj);
	Vector3 position = Commands->Get_Position(obj);
	GameObject *newobj;
	position.Z += Get_Float_Parameter("DropHeight");
	newobj = Commands->Create_Object(Get_Parameter("PresetName"),position);
	if (!Get_Int_Parameter("SameFacing"))
	{
		return;
	}
	Commands->Set_Facing(newobj,facing);
	float maxhealth = Commands->Get_Max_Health(newobj);
	float maxarmor = Commands->Get_Max_Shield_Strength(newobj);
	if (maxhealth) Commands->Set_Health(newobj,maxhealth*Get_Float_Parameter("HealthPercent") / 100);
	if (maxarmor) Commands->Set_Shield_Strength(newobj,maxarmor*Get_Float_Parameter("ArmorPercent") / 100);
}

void TFX_Replace_When_Repaired::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (amount >= 0 || Commands->Get_Health(obj) < Commands->Get_Max_Health(obj) || Commands->Get_Shield_Strength(obj) < Commands->Get_Max_Shield_Strength(obj))
	{
		return;
	}
	float facing = Commands->Get_Facing(obj);
	Vector3 position = Commands->Get_Position(obj);
	GameObject*newobj;
	Commands->Destroy_Object(obj);
	position.Z += Get_Float_Parameter("DropHeight");
	newobj = Commands->Create_Object(Get_Parameter("PresetName"),position);
	if (!Get_Int_Parameter("SameFacing"))
	{
		return;
	}
	Commands->Set_Facing(newobj,facing);
	float maxhealth = Commands->Get_Max_Health(newobj);
	float maxarmor = Commands->Get_Max_Shield_Strength(newobj);
	if (maxhealth) Commands->Set_Health(newobj,maxhealth*Get_Float_Parameter("HealthPercent") / 100);
	if (maxarmor) Commands->Set_Shield_Strength(newobj,maxarmor*Get_Float_Parameter("ArmorPercent") / 100);
}

ScriptRegistrant<TFX_Spawn_When_Killed> TFX_Spawn_When_Killed_Registrant("TFX_Spawn_When_Killed","PresetName:string,DropHeight=0:float,SameFacing=1:int,HealthPercent=100.0000:float,ArmorPercent=100.0000:float");
ScriptRegistrant<TFX_Replace_When_Repaired> TFX_Replace_When_Repaired_Registrant("TFX_Replace_When_Repaired","PresetName:string,DropHeight=0:float,SameFacing=1:int,HealthPercent=100.0000:float,ArmorPercent=100.0000:float");
