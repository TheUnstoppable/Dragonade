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
#include "TeamPurchaseSettingsDefClass.h"
#include "PurchaseSettingsDefClass.h"
#include "engine_def.h"
#include "engine_obj2.h"

unsigned int SCRIPTS_API Get_Team_Cost(int def_id,unsigned int team)
{
	if (TeamPurchaseSettingsDefClass::Get_Definition((TeamPurchaseSettingsDefClass::TEAM)PTTEAM(team))->Get_Beacon_Definition() == def_id)
	{
		return TeamPurchaseSettingsDefClass::Get_Definition((TeamPurchaseSettingsDefClass::TEAM)PTTEAM(team))->Get_Beacon_Cost();
	}
	for (unsigned int i = 0;i < 7;i++)
	{
		PurchaseSettingsDefClass *p = PurchaseSettingsDefClass::Find_Definition((PurchaseSettingsDefClass::TYPE)i,(PurchaseSettingsDefClass::TEAM)PTTEAM(team));
		if (p)
		{
			for (unsigned int j = 0;j < 10;j++)
			{
				if ((p->Get_Definition(j) == def_id) || (p->Get_Alt_Definition(j,0) == def_id) || (p->Get_Alt_Definition(j,1) == def_id) || (p->Get_Alt_Definition(j,2) == def_id))
				{
					return p->Get_Cost(j);
				}
			}
		}
	}
	return 0;
}

unsigned int SCRIPTS_API Get_Team_Cost(const char *preset,unsigned int team)
{
	return Get_Team_Cost(Get_Definition_ID(preset), team);
}

unsigned int SCRIPTS_API Get_Cost(int def_id)
{
	unsigned int cost = Get_Team_Cost(def_id,0);
	if (!cost)
	{
		cost = Get_Team_Cost(def_id,1);
	}
	return cost;
}

unsigned int SCRIPTS_API Get_Cost(const char *preset)
{
	return Get_Cost(Get_Definition_ID(preset));
}

void SCRIPTS_API Disable_Preset_By_Name(unsigned int Team,const char *Name, bool enable)
{
	int ID = Get_Definition_ID(Name);
	TeamPurchaseSettingsDefClass *t = TeamPurchaseSettingsDefClass::Get_Definition((TeamPurchaseSettingsDefClass::TEAM)PTTEAM(Team));
	for (unsigned int j = 0; j < 4; j++)
	{
		if (t->Get_Enlisted_Definition(j) == ID)
		{
			t->Set_Disabled(j,enable);
		}
	}
	PurchaseSettingsDefClass *p = 0;
	unsigned int i;
	for (i = 0; i < 7; i++)
	{
		p = PurchaseSettingsDefClass::Find_Definition((PurchaseSettingsDefClass::TYPE)i,(PurchaseSettingsDefClass::TEAM)PTTEAM(Team));
		if (!p)
		{
			continue;
		}
		for (unsigned int j = 0; j < 10; j++)
		{
			if (p->Get_Definition(j) == ID)
			{
				p->Set_Disabled(j,enable);
			}
		}
	}
}

void SCRIPTS_API Hide_Preset_By_Name(unsigned int Team,const char *Name, bool enable)
{
	int ID = Get_Definition_ID(Name);
	TeamPurchaseSettingsDefClass *t = TeamPurchaseSettingsDefClass::Get_Definition((TeamPurchaseSettingsDefClass::TEAM)PTTEAM(Team));
	for (unsigned int j = 0; j < 4; j++)
	{
		if (t->Get_Enlisted_Definition(j) == ID)
		{
			t->Set_Hidden(j,enable);
		}
	}
	PurchaseSettingsDefClass *p = 0;
	unsigned int i;
	for (i = 0; i < 7; i++)
	{
		p = PurchaseSettingsDefClass::Find_Definition((PurchaseSettingsDefClass::TYPE)i,(PurchaseSettingsDefClass::TEAM)PTTEAM(Team));
		if (!p)
		{
			continue;
		}
		for (unsigned int j = 0; j < 10; j++)
		{
			if (p->Get_Definition(j) == ID)
			{
				p->Set_Hidden(j,enable);
			}
		}
	}
}

void SCRIPTS_API Busy_Preset_By_Name(unsigned int Team,const char *Name, bool enable)
{
	int ID = Get_Definition_ID(Name);
	TeamPurchaseSettingsDefClass *t = TeamPurchaseSettingsDefClass::Get_Definition((TeamPurchaseSettingsDefClass::TEAM)PTTEAM(Team));
	for (unsigned int j = 0; j < 4; j++)
	{
		if (t->Get_Enlisted_Definition(j) == ID)
		{
			t->Set_Busy(j,enable);
		}
	}
	PurchaseSettingsDefClass *p = 0;
	unsigned int i;
	for (i = 0; i < 7; i++)
	{
		p = PurchaseSettingsDefClass::Find_Definition((PurchaseSettingsDefClass::TYPE)i,(PurchaseSettingsDefClass::TEAM)PTTEAM(Team));
		if (!p)
		{
			continue;
		}
		for (unsigned int j = 0; j < 10; j++)
		{
			if (p->Get_Definition(j) == ID)
			{
				p->Set_Busy(j,enable);
			}
		}
	}
}

const char SCRIPTS_API *Get_Team_Icon(const char *preset,unsigned int team)
{
	int ID = Get_Definition_ID(preset);
	TeamPurchaseSettingsDefClass *t = TeamPurchaseSettingsDefClass::Get_Definition((TeamPurchaseSettingsDefClass::TEAM)team);
	if (t->Get_Beacon_Definition() == ID)
	{
		return t->Get_Beacon_Texture();
	}
	for (unsigned int i = 0;i < 3;i++)
	{
		if (t->Get_Enlisted_Definition(i) == ID)
		{
			return t->Get_Enlisted_Texture(i);
		}
	}
	for (unsigned int i = 0;i < 7;i++)
	{
		PurchaseSettingsDefClass *p = PurchaseSettingsDefClass::Find_Definition((PurchaseSettingsDefClass::TYPE)i,(PurchaseSettingsDefClass::TEAM)team);
		if (p)
		{
			for (unsigned int j = 0;j < 10;j++)
			{
				if (p->Get_Definition(j) == ID)
				{
					return p->Get_Texture(j);
				}
				if (p->Get_Alt_Definition(j,0) == ID)
				{
					return p->Get_Alt_Texture(j,0);
				}
				if (p->Get_Alt_Definition(j,1) == ID)
				{
					return p->Get_Alt_Texture(j,1);
				}
				if (p->Get_Alt_Definition(j,2) == ID)
				{
					return p->Get_Alt_Texture(j,2);
				}
			}
		}
	}
	return 0;
}

const char SCRIPTS_API *Get_Icon(const char *preset)
{
	const char *icon = Get_Team_Icon(preset,0);
	if (!icon)
	{
		icon = Get_Team_Icon(preset,1);
	}
	return icon;
}

void SCRIPTS_API Disable_All_Presets_By_Factory_Tech(BuildingType type,unsigned int team,bool disable)
{
	for (unsigned int i = 0;i < 7;i++)
	{
		PurchaseSettingsDefClass *p = PurchaseSettingsDefClass::Find_Definition((PurchaseSettingsDefClass::TYPE)i,(PurchaseSettingsDefClass::TEAM)PTTEAM(team));
		if (p)
		{
			for (unsigned int j = 0;j < 10;j++)
			{
				if ((p->Get_Factory_Building(j) == type) || (p->Get_Tech_Building(j) == type))
				{
					p->Set_Disabled(j,disable);
				}
			}
		}
	}
}
