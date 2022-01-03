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
#include "WeaponBagClass.h"
#include "WeaponClass.h"
#include "VehicleGameObj.h"
#include "TranslateDBClass.h"
#include "Engine_Obj.h"
#include "Engine_Def.h"

const wchar_t *VehicleGameObj::Get_Vehicle_Name()
{
	if (( Get_Definition().VehicleNameID != 0 ) && (Get_Definition().VehicleNameID-1000 < TranslateDBClass::Get_Object_Count())) {
		return TranslateDBClass::Get_String( Get_Definition().VehicleNameID );
	}
	return TRANSLATE(12648);
}
SCRIPTS_API const char *Get_Translated_String(unsigned long ID)
{
	if (ID < 1000)
	{
		return newstr("No String");
	}
	TDBObjClass *o = TranslateDBClass::Find_Object(ID);
	if (o)
	{
		return WideCharToChar(o->Get_String());
	}
	else
	{
		return newstr("ERROR: String Not Found");
	}
}

SCRIPTS_API const wchar_t *Get_Wide_Translated_String(unsigned long ID)
{
	if (ID < 1000)
	{
		return L"No String";
	}
	TDBObjClass *o = TranslateDBClass::Find_Object(ID);
	if (o)
	{
		return o->Get_String();
	}
	else
	{
		return L"ERROR: String Not Found";
	}
}

SCRIPTS_API const char *Get_Translated_Preset_Name(GameObject *obj)
{
	if (!obj)
	{
		return newstr("");
	}
	DamageableGameObj *o = obj->As_DamageableGameObj();
	if (!o)
	{
		return newstr("");
	}
	const DamageableGameObjDef *d = &o->Get_Definition();
	return Get_Translated_String(d->Get_Translated_Name_ID());
}

SCRIPTS_API const wchar_t *Get_Current_Wide_Translated_Weapon(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	PhysicalGameObj *o = obj->As_PhysicalGameObj();
	if (!o)
	{
		return 0;
	}
	ArmedGameObj *o2 = o->As_ArmedGameObj();
	if (!o2)
	{
		return 0;
	}
	WeaponBagClass *w = o2->Get_Weapon_Bag();
	if ((w->Get_Index()) && (w->Get_Index() < w->Get_Count()))
	{
		return Get_Wide_Translated_String(w->Peek_Weapon(w->Get_Index())->Get_Definition()->IconNameID);
	}
	return 0;
}

SCRIPTS_API const char *Get_Current_Translated_Weapon(GameObject *obj)
{
	return WideCharToChar(Get_Current_Wide_Translated_Weapon(obj));
}

SCRIPTS_API const char *Get_Translated_Weapon(GameObject *obj,int position)
{
	if (!obj)
	{
		return newstr("");
	}
	PhysicalGameObj *o = obj->As_PhysicalGameObj();
	if (!o)
	{
		return newstr("");
	}
	ArmedGameObj *o2 = o->As_ArmedGameObj();
	if (!o2)
	{
		return newstr("");
	}
	WeaponBagClass *w = o2->Get_Weapon_Bag();
	if ((position) && (position < w->Get_Count()))
	{
		return Get_Translated_String(w->Peek_Weapon(position)->Get_Definition()->IconNameID);
	}
	return newstr("");
}

SCRIPTS_API const char *Get_Team_Name(int Team)
{
	if (!Team)
	{
		return Get_Translated_String(7198);
	}
	else if (Team == 1)
	{
		return Get_Translated_String(7199);
	}
	return newstr("Neutral");
}

SCRIPTS_API StringClass Get_Short_Team_Name(int Team)
{
	StringClass Return;
	if (!Team)
	{
		Return = Get_Wide_Translated_String(7198);
	}
	else if (Team == 1) 
	{
		Return = Get_Wide_Translated_String(7199);
	}
	else
	{
		Return = "Neutral";
	}
	Return.cropTo(3);
	return Return;
}

SCRIPTS_API const wchar_t *Get_Wide_Team_Name(int Team) 
{
	if (!Team) 
	{
		return Get_Wide_Translated_String(7198);
	}
	else if (Team == 1)
	{
		return Get_Wide_Translated_String(7199);
	}
	return L"Neutral";
}

SCRIPTS_API const wchar_t *Get_Wide_Vehicle_Name(GameObject *obj)
{
	if (!obj)
	{
		return L"Vehicle";
	}
	VehicleGameObj *o = obj->As_VehicleGameObj();
	if (!o)
	{
		return L"Vehicle";
	}
	return o->Get_Vehicle_Name();
}

SCRIPTS_API const char *Get_Vehicle_Name(GameObject *obj)
{
	return WideCharToChar(Get_Wide_Vehicle_Name(obj));
}

SCRIPTS_API const char *Get_Translated_Definition_Name(const char *preset)
{
	DamageableGameObjDef *d = (DamageableGameObjDef *)Find_Named_Definition(preset);
	if (!d)
	{
		return newstr("");
	}
	return Get_Translated_String(d->Get_Translated_Name_ID());
}

SCRIPTS_API bool Is_Valid_String_ID(unsigned long ID)
{
	if (TranslateDBClass::Get_String((int)ID))
	{
		return true;
	}
	return false;
}

SCRIPTS_API unsigned long Get_String_ID_By_Desc(const char *desc)
{
	if ( strlen(desc) <= 0 )
	{
		return 0;
	}
	TDBObjClass *o = TranslateDBClass::Find_Object(desc);
	if (o)
	{
		return o->Get_ID();
	}
	else
	{
		return 0;
	}
}

SCRIPTS_API unsigned long Get_String_Sound_ID(unsigned long ID)
{
	TDBObjClass *o = TranslateDBClass::Find_Object(ID);
	if (o)
	{
		return o->Get_Sound_ID();
	}
	else
	{
		return 0;
	}
}
