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
#include "mdbevf.h"
#include "PurchaseSettingsDefClass.h"
#include "TeamPurchaseSettingsDefClass.h"
#include "VehicleGameObjDef.h"
#include "ScriptZoneGameObj.h"
ExpVehFacClass *ExpVehFac[2] = {0,0};

void DestroyExpVehFacClass()
{
	SAFE_DELETE(ExpVehFac[0]);
	SAFE_DELETE(ExpVehFac[1]);
}

ExpVehFacClass::ExpVehFacClass(unsigned int team)
{
	Team = team;
	Loaded = false;
	LastFlyVehID = 0;
	LastFlyVehOwner = 0;
	PHookID = 0;
	OHookID = 0;
	Last_Flying_Landing_Pos = 0;
	Naval_Creation_Positions = 0;
	Flying_Landing_Positions = 0;
	Last_Naval_Creation_Pos = 0;
	Using_Naval = false;
}
ExpVehFacClass::~ExpVehFacClass()
{
	if (Loaded)
	{
		Clear();
	}
}
void ExpVehFacClass::Set_Ground_Currently_Building(bool building)
{
	if (Loaded)
	{
		Is_Ground_Building = building;
		int x = Units.Count();
		for (int i = 0; i < x; i++)
		{
			if (Units[i])
			{
				if (Units[i]->VehType == VEHTYPE_GROUND)
				{
					unsigned int Pos = Units[i]->Pos;
					PurchaseSettingsDefClass *p = PurchaseSettingsDefClass::Find_Definition(Units[i]->Type,(PurchaseSettingsDefClass::TEAM)PTTEAM(Team));
					if ((!p) || (Units[i]->Alternate))
					{
						continue;
					}
					p->Set_Busy(Pos,building);
				}
			}
		}
	}
}
void ExpVehFacClass::Set_Flying_Currently_Building(bool building)
{
	if (Loaded)
	{
		Is_Flying_Building = building;
		int x = Units.Count();
		for (int i = 0; i < x; i++)
		{
			if (Units[i])
			{
				if (Units[i]->VehType == VEHTYPE_FLYING)
				{
					unsigned int Pos = Units[i]->Pos;
					PurchaseSettingsDefClass *p = PurchaseSettingsDefClass::Find_Definition(Units[i]->Type,(PurchaseSettingsDefClass::TEAM)PTTEAM(Team));
					if ((!p) || (Units[i]->Alternate))
					{
						continue;
					}
					p->Set_Busy(Pos,building);
				}
			}
		}
	}
}

void ExpVehFacClass::Set_Naval_Currently_Building(bool building)
{
	if (Loaded)
	{
		Is_Naval_Building = building;
		int x = Units.Count();
		for (int i = 0; i < x; i++)
		{
			if (Units[i])
			{
				if (Units[i]->VehType == VEHTYPE_NAVAL)
				{
					unsigned int Pos = Units[i]->Pos;
					PurchaseSettingsDefClass *p = PurchaseSettingsDefClass::Find_Definition(Units[i]->Type,(PurchaseSettingsDefClass::TEAM)PTTEAM(Team));
					if ((!p) || (Units[i]->Alternate))
					{
						continue;
					}
					p->Set_Busy(Pos,building);
				}
			}
		}
	}
}
void ExpVehFacClass::Set_Ground_Limit_Reached(bool Reached)
{
	if (Loaded)
	{
		int x = Units.Count();
		for (int i = 0; i < x; i++)
		{
			if (Units[i])
			{
				if (Units[i]->VehType == VEHTYPE_GROUND && !Units[i]->Support)
				{
					unsigned int Pos = Units[i]->Pos;
					PurchaseSettingsDefClass *p = PurchaseSettingsDefClass::Find_Definition(Units[i]->Type,(PurchaseSettingsDefClass::TEAM)PTTEAM(Team));
					if ((!p) || (Units[i]->Alternate))
					{
						continue;
					}
					if (Reached)
					{
						if (Units[i]->DMode == DISABLED_UNIT)
						{
							Units[i]->DMode = DISABLED_BOTH;
						}
						else
						{
							Units[i]->DMode = DISABLED_EVF;
							p->Set_Disabled(Pos,true);
						}
					}
					else if (p->Get_Disabled(Pos))
					{
						if (Units[i]->DMode == DISABLED_EVF)
						{
							p->Set_Disabled(Pos,false);
							Units[i]->DMode = NOT_DISABLED;
						}
						else if (Units[i]->DMode == DISABLED_BOTH)
						{
							Units[i]->DMode = DISABLED_UNIT;
						}
					}
				}
			}
		}
	}
}

void ExpVehFacClass::Set_Flying_Limit_Reached(bool Reached)
{
	if (Loaded)
	{
		int x = Units.Count();
		for (int i = 0; i < x; i++)
		{
			if (Units[i])
			{
				if (Units[i]->VehType == VEHTYPE_FLYING && !Units[i]->Support)
				{
					unsigned int Pos = Units[i]->Pos;
					PurchaseSettingsDefClass *p = PurchaseSettingsDefClass::Find_Definition(Units[i]->Type,(PurchaseSettingsDefClass::TEAM)PTTEAM(Team));
					if ((!p) || (Units[i]->Alternate))
					{
						continue;
					}
					if (Reached)
					{
						if (Units[i]->DMode == DISABLED_UNIT)
						{
							Units[i]->DMode = DISABLED_BOTH;
						}
						else
						{
							Units[i]->DMode = DISABLED_EVF;
							p->Set_Disabled(Pos,true);
						}
					}
					else if (p->Get_Disabled(Pos))
					{
						if (Units[i]->DMode == DISABLED_EVF)
						{
							p->Set_Disabled(Pos,false);
							Units[i]->DMode = NOT_DISABLED;
						}
						else if (Units[i]->DMode == DISABLED_BOTH)
						{
							Units[i]->DMode = DISABLED_UNIT;
						}
					}
				}
			}
		}
	}
}

void ExpVehFacClass::Set_Naval_Limit_Reached(bool Reached)
{
	if (Loaded)
	{
		int x = Units.Count();
		for (int i = 0; i < x; i++)
		{
			if (Units[i])
			{
				if (Units[i]->VehType == VEHTYPE_NAVAL && !Units[i]->Support)
				{
					unsigned int Pos = Units[i]->Pos;
					PurchaseSettingsDefClass *p = PurchaseSettingsDefClass::Find_Definition(Units[i]->Type,(PurchaseSettingsDefClass::TEAM)PTTEAM(Team));
					if ((!p) || (Units[i]->Alternate))
					{
						continue;
					}
					if (Reached)
					{
						if (Units[i]->DMode == DISABLED_UNIT)
						{
							Units[i]->DMode = DISABLED_BOTH;
						}
						else
						{
							Units[i]->DMode = DISABLED_EVF;
							p->Set_Disabled(Pos,true);
						}
					}
					else if (p->Get_Disabled(Pos))
					{
						if (Units[i]->DMode == DISABLED_EVF)
						{
							p->Set_Disabled(Pos,false);
							Units[i]->DMode = NOT_DISABLED;
						}
						else if (Units[i]->DMode == DISABLED_BOTH)
						{
							Units[i]->DMode = DISABLED_UNIT;
						}
					}
				}
			}
		}
	}
}

void ExpVehFacClass::Set_Support_Limit_Reached(bool Reached)
{
	if (Loaded)
	{
		int x = Units.Count();
		for (int i = 0; i < x; i++)
		{
			if (Units[i])
			{
				if (Units[i]->Support)
				{
					unsigned int Pos = Units[i]->Pos;
					PurchaseSettingsDefClass *p = PurchaseSettingsDefClass::Find_Definition(Units[i]->Type,(PurchaseSettingsDefClass::TEAM)PTTEAM(Team));
					if ((!p) || (Units[i]->Alternate))
					{
						continue;
					}
					if (Reached)
					{
						if (Units[i]->DMode == DISABLED_UNIT)
						{
							Units[i]->DMode = DISABLED_BOTH;
						}
						else
						{
							Units[i]->DMode = DISABLED_EVF;
							p->Set_Disabled(Pos,true);
						}
					}
					else if (p->Get_Disabled(Pos))
					{
						if (Units[i]->DMode == DISABLED_EVF)
						{
							p->Set_Disabled(Pos,false);
							Units[i]->DMode = NOT_DISABLED;
						}
						else if (Units[i]->DMode == DISABLED_BOTH)
						{
							Units[i]->DMode = DISABLED_UNIT;
						}
					}
				}
			}
		}
	}
}
bool ExpVehFacClass::Is_In_Vector(SimpleDynVecClass<unsigned int> &Vec, unsigned int Preset)
{
	int x = Vec.Count();
	for (int i = 0; i < x; i++)
	{
		if (Vec[i] == Preset)
		{
			return true;
		}
	}
	return false;
}

UnitInfo *ExpVehFacClass::Get_Unit_Info(unsigned int Preset)
{
	if (Loaded)
	{
		int x = Units.Count();
		for (int i = 0; i < x; i++)
		{
			if (Units[i])
			{
				if ((Units[i])->Preset == Preset)
				{
					return (Units[i]);
				}
			}
		}
	}
	return 0;
}

Vector3 ExpVehFacClass::Get_Flying_Landing_Position()
{
	if (Loaded)
	{
		Last_Flying_Landing_Pos++;
		if (Last_Flying_Landing_Pos >= Flying_Landing_Positions->Count())
		{
			Last_Flying_Landing_Pos = 0;
		}
		return Commands->Get_Position((*Flying_Landing_Positions)[Last_Flying_Landing_Pos]);
	}
	return Vector3(0.0f,0.0f,0.0f);
}

GameObject *ExpVehFacClass::Get_Naval_Creation_Position(unsigned int Preset)
{
	if (Loaded)
	{
		if (Naval_Creation_Positions->Count())
		{
			Last_Naval_Creation_Pos++;
			if (Last_Naval_Creation_Pos >= Naval_Creation_Positions->Count())
			{
				Last_Naval_Creation_Pos = 0;
			}
			return (*Naval_Creation_Positions)[Last_Naval_Creation_Pos];
		}
		else
		{
			int x = Naval_Creation_Pos_Per_Vehicle.Count();
			for (int i = 0; i < x; i++)
			{
				if (Naval_Creation_Pos_Per_Vehicle[i])
				{
					if (Naval_Creation_Pos_Per_Vehicle[i]->Preset == Preset)
					{
						return Naval_Creation_Pos_Per_Vehicle[i]->Pos;
					}
				}
			}
		}
	}
	return 0;
}
void ExpVehFacClass::Load(const char *NavalVehicles, const char *NavalCreationPresets,GameObject *script,const char *SupportVehicles)
{
	Naval_Creation_Positions = new SimpleDynVecClass<ScriptableGameObj *>;
	Flying_Landing_Positions = new SimpleDynVecClass<ScriptableGameObj *>;
	Units.Delete_All();
	Ground_Limit = 0;
	Flying_Limit = 0;
	Naval_Limit = 0;
	Support_Limit = 0;
	Ground_Amount = 0;
	Flying_Amount = 0;
	Naval_Amount = 0;
	Support_Amount = 0;
	Is_Ground_Building = false;
	Is_Flying_Building = false;
	Is_Naval_Building = false;
	SimpleDynVecClass<unsigned int> Support(0);
	if (_stricmp(SupportVehicles,"0"))
	{
		char *STokens = newstr(SupportVehicles);
		char *STok = strtok(STokens,";");
		while (STok)
		{
			unsigned int ID = Get_Definition_ID(STok);
			Support.Add(ID);
			STok = strtok(0,";");
		}
		delete[] STokens;
	}
	if (NavalCreationPresets)
	{
		char *Tokenz = newstr(NavalCreationPresets);
		int size = strlen(Tokenz);
		for (int i = 0; i < size; i++)
		{
			if (Tokenz[i] == ';')
			{
				Tokenz[i] = '=';
			}
		}
		char *p = strtok(Tokenz,"=");
		while (p)
		{
			CreationPos *Temp = new CreationPos;
			Temp->Preset = Get_Definition_ID(p);
			char *create = strtok(0,"=");
			Temp->Pos = Find_Object_By_Preset(2,create);
			Naval_Creation_Pos_Per_Vehicle.Add(Temp);
			p = strtok(0,"=");
		}
		delete[] Tokenz;
	}
	if (!NavalVehicles)
	{
		PurchaseSettingsDefClass *p = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_VEHICLES,(PurchaseSettingsDefClass::TEAM)PTTEAM(Team));
		if (p)
		{
			unsigned int j;
			for (j = 0; j < 10; j++)
			{
				if (p->Get_Definition(j))
				{
					UnitInfo *Temp = new UnitInfo(PurchaseSettingsDefClass::TYPE_VEHICLES,j,p->Get_Definition(j),false);
					if (Is_In_Vector(Support,p->Get_Definition(j)))
					{
						Temp->Support = true;
					}
					if (Get_Vehicle_Definition_Mode_By_ID(p->Get_Definition(j)) == VEHICLE_TYPE_FLYING)
					{
						Temp->VehType = VEHTYPE_FLYING;
					}
					else
					{
						Temp->VehType = VEHTYPE_GROUND;
					}
					Units.Add(Temp);
					unsigned int k;
					for (k = 0;k < 3;k++)
					{
						if (p->Get_Alt_Definition(j,k))
						{
							UnitInfo *Temp2 = new UnitInfo(PurchaseSettingsDefClass::TYPE_VEHICLES,j,p->Get_Alt_Definition(j,k),true);
							if (Is_In_Vector(Support,p->Get_Alt_Definition(j,k)))
							{
								Temp2->Support = true;
							}
							if (Get_Vehicle_Definition_Mode_By_ID(p->Get_Alt_Definition(j,k)) == VEHICLE_TYPE_FLYING)
							{
								Temp2->VehType = VEHTYPE_FLYING;
							}
							else
							{
								Temp2->VehType = VEHTYPE_GROUND;
							}
							Units.Add(Temp2);
						}
					}
				}
			}
		}
		p = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_SECRET_VEHICLES,(PurchaseSettingsDefClass::TEAM)PTTEAM(Team));
		if (p)
		{
			for (int j = 0; j < 10; j++)
			{
				if (p->Get_Definition(j))
				{
					UnitInfo *Temp = new UnitInfo(PurchaseSettingsDefClass::TYPE_SECRET_VEHICLES,j,p->Get_Definition(j),false);
					if (Is_In_Vector(Support,p->Get_Definition(j)))
					{
						Temp->Support = true;
					}
					if (Get_Vehicle_Definition_Mode_By_ID(p->Get_Definition(j)) == VEHICLE_TYPE_FLYING)
					{
						Temp->VehType = VEHTYPE_FLYING;
					}
					else
					{
						Temp->VehType = VEHTYPE_GROUND;
					}
					Units.Add(Temp);
					unsigned int k;
					for (k = 0;k < 3;k++)
					{
						if (p->Get_Alt_Definition(j,k))
						{
							UnitInfo *Temp2 = new UnitInfo(PurchaseSettingsDefClass::TYPE_SECRET_VEHICLES,j,p->Get_Alt_Definition(j,k),true);
							if (Is_In_Vector(Support,p->Get_Alt_Definition(j,k)))
							{
								Temp2->Support = true;
							}
							if (Get_Vehicle_Definition_Mode_By_ID(p->Get_Alt_Definition(j,k)) == VEHICLE_TYPE_FLYING)
							{
								Temp2->VehType = VEHTYPE_FLYING;
							}
							else
							{
								Temp2->VehType = VEHTYPE_GROUND;
							}
							Units.Add(Temp2);
						}
					}
				}
			}
		}
		p = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_NAVAL,(PurchaseSettingsDefClass::TEAM)PTTEAM(Team));
		if (p)
		{
			for (int j = 0; j < 10; j++)
			{
				if (p->Get_Definition(j))
				{
					UnitInfo *Temp = new UnitInfo(PurchaseSettingsDefClass::TYPE_NAVAL,j,p->Get_Definition(j),false);
					if (Is_In_Vector(Support,p->Get_Definition(j)))
					{
						Temp->Support = true;
					}
					if (Get_Vehicle_Definition_Mode_By_ID(p->Get_Definition(j)) == VEHICLE_TYPE_FLYING)
					{
						Temp->VehType = VEHTYPE_FLYING;
					}
					else
					{
						Temp->VehType = VEHTYPE_GROUND;
					}
					Units.Add(Temp);
					unsigned int k;
					for (k = 0;k < 3;k++)
					{
						if (p->Get_Alt_Definition(j,k))
						{
							UnitInfo *Temp2 = new UnitInfo(PurchaseSettingsDefClass::TYPE_NAVAL,j,p->Get_Alt_Definition(j,k),true);
							if (Is_In_Vector(Support,p->Get_Alt_Definition(j,k)))
							{
								Temp2->Support = true;
							}
							if (Get_Vehicle_Definition_Mode_By_ID(p->Get_Alt_Definition(j,k)) == VEHICLE_TYPE_FLYING)
							{
								Temp2->VehType = VEHTYPE_FLYING;
							}
							else
							{
								Temp2->VehType = VEHTYPE_GROUND;
							}
							Units.Add(Temp2);
						}
					}
				}
			}
		}
		p = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_AIR,(PurchaseSettingsDefClass::TEAM)PTTEAM(Team));
		if (p)
		{
			for (int j = 0; j < 10; j++)
			{
				if (p->Get_Definition(j))
				{
					UnitInfo *Temp = new UnitInfo(PurchaseSettingsDefClass::TYPE_AIR,j,p->Get_Definition(j),false);
					if (Is_In_Vector(Support,p->Get_Definition(j)))
					{
						Temp->Support = true;
					}
					if (Get_Vehicle_Definition_Mode_By_ID(p->Get_Definition(j)) == VEHICLE_TYPE_FLYING)
					{
						Temp->VehType = VEHTYPE_FLYING;
					}
					else
					{
						Temp->VehType = VEHTYPE_GROUND;
					}
					Units.Add(Temp);
					unsigned int k;
					for (k = 0;k < 3;k++)
					{
						if (p->Get_Alt_Definition(j,k))
						{
							UnitInfo *Temp2 = new UnitInfo(PurchaseSettingsDefClass::TYPE_AIR,j,p->Get_Alt_Definition(j,k),true);
							if (Is_In_Vector(Support,p->Get_Alt_Definition(j,k)))
							{
								Temp2->Support = true;
							}
							if (Get_Vehicle_Definition_Mode_By_ID(p->Get_Alt_Definition(j,k)) == VEHICLE_TYPE_FLYING)
							{
								Temp2->VehType = VEHTYPE_FLYING;
							}
							else
							{
								Temp2->VehType = VEHTYPE_GROUND;
							}
							Units.Add(Temp2);
						}
					}
				}
			}
		}
	}
	else
	{
		Using_Naval = true;
		SimpleDynVecClass<unsigned int> NavalIDs(0);
		char *Tokens = newstr(NavalVehicles);
		char *Tok = strtok(Tokens,";");
		while (Tok)
		{
			unsigned int ID = Get_Definition_ID(Tok);
			NavalIDs.Add(ID);
			Tok = strtok(0,";");
		}
		delete[] Tokens;
		PurchaseSettingsDefClass *p = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_VEHICLES,(PurchaseSettingsDefClass::TEAM)PTTEAM(Team));
		if (p)
		{
			unsigned int j = 0;
			for (j = 0; j < 10; j++)
			{
				if (p->Get_Definition(j))
				{
					UnitInfo *Temp = new UnitInfo(PurchaseSettingsDefClass::TYPE_VEHICLES,j,p->Get_Definition(j),false);
					if (Is_In_Vector(Support,p->Get_Definition(j)))
					{
						Temp->Support = true;
					}
					if (Is_In_Vector(NavalIDs,p->Get_Definition(j)))
					{
						Temp->VehType = VEHTYPE_NAVAL;
					}
					else if (Get_Vehicle_Definition_Mode_By_ID(p->Get_Definition(j)) == VEHICLE_TYPE_FLYING)
					{
						Temp->VehType = VEHTYPE_FLYING;
					}
					else
					{
						Temp->VehType = VEHTYPE_GROUND;
					}
					Units.Add(Temp);
					unsigned int k;
					for (k = 0;k < 3;k++)
					{
						if (p->Get_Alt_Definition(j,k))
						{
							UnitInfo *Temp2 = new UnitInfo(PurchaseSettingsDefClass::TYPE_VEHICLES,j,p->Get_Alt_Definition(j,k),true);
							if (Is_In_Vector(Support,p->Get_Alt_Definition(j,k)))
							{
								Temp2->Support = true;
							}
							if (Get_Vehicle_Definition_Mode_By_ID(p->Get_Alt_Definition(j,k)) == VEHICLE_TYPE_FLYING)
							{
								Temp2->VehType = VEHTYPE_FLYING;
							}
							else
							{
								Temp2->VehType = VEHTYPE_GROUND;
							}
							Units.Add(Temp2);
						}
					}
				}
			}
		}
		p = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_SECRET_VEHICLES,(PurchaseSettingsDefClass::TEAM)PTTEAM(Team));
		if (p)
		{
			for (int j = 0; j < 10; j++)
			{
				if (p->Get_Definition(j))
				{
					UnitInfo *Temp = new UnitInfo(PurchaseSettingsDefClass::TYPE_SECRET_VEHICLES,j,p->Get_Definition(j),false);
					if (Is_In_Vector(Support,p->Get_Definition(j)))
					{
						Temp->Support = true;
					}
					if (Is_In_Vector(NavalIDs,p->Get_Definition(j)))
					{
						Temp->VehType = VEHTYPE_NAVAL;
					}
					else if (Get_Vehicle_Definition_Mode_By_ID(p->Get_Definition(j)) == VEHICLE_TYPE_FLYING)
					{
						Temp->VehType = VEHTYPE_FLYING;
					}
					else
					{
						Temp->VehType = VEHTYPE_GROUND;
					}
					Units.Add(Temp);
					unsigned int k;
					for (k = 0;k < 3;k++)
					{
						if (p->Get_Alt_Definition(j,k))
						{
							UnitInfo *Temp2 = new UnitInfo(PurchaseSettingsDefClass::TYPE_SECRET_VEHICLES,j,p->Get_Alt_Definition(j,k),true);
							if (Is_In_Vector(Support,p->Get_Alt_Definition(j,k)))
							{
								Temp2->Support = true;
							}
							if (Get_Vehicle_Definition_Mode_By_ID(p->Get_Alt_Definition(j,k)) == VEHICLE_TYPE_FLYING)
							{
								Temp2->VehType = VEHTYPE_FLYING;
							}
							else
							{
								Temp2->VehType = VEHTYPE_GROUND;
							}
							Units.Add(Temp2);
						}
					}
				}
			}
		}
		p = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_NAVAL,(PurchaseSettingsDefClass::TEAM)PTTEAM(Team));
		if (p)
		{
			for (int j = 0; j < 10; j++)
			{
				if (p->Get_Definition(j))
				{
					UnitInfo *Temp = new UnitInfo(PurchaseSettingsDefClass::TYPE_NAVAL,j,p->Get_Definition(j),false);
					if (Is_In_Vector(Support,p->Get_Definition(j)))
					{
						Temp->Support = true;
					}
					if (Is_In_Vector(NavalIDs,p->Get_Definition(j)))
					{
						Temp->VehType = VEHTYPE_NAVAL;
					}
					else if (Get_Vehicle_Definition_Mode_By_ID(p->Get_Definition(j)) == VEHICLE_TYPE_FLYING)
					{
						Temp->VehType = VEHTYPE_FLYING;
					}
					else
					{
						Temp->VehType = VEHTYPE_GROUND;
					}
					Units.Add(Temp);
					unsigned int k;
					for (k = 0;k < 3;k++)
					{
						if (p->Get_Alt_Definition(j,k))
						{
							UnitInfo *Temp2 = new UnitInfo(PurchaseSettingsDefClass::TYPE_NAVAL,j,p->Get_Alt_Definition(j,k),true);
							if (Is_In_Vector(Support,p->Get_Alt_Definition(j,k)))
							{
								Temp2->Support = true;
							}
							if (Get_Vehicle_Definition_Mode_By_ID(p->Get_Alt_Definition(j,k)) == VEHICLE_TYPE_FLYING)
							{
								Temp2->VehType = VEHTYPE_FLYING;
							}
							else
							{
								Temp2->VehType = VEHTYPE_GROUND;
							}
							Units.Add(Temp2);
						}
					}
				}
			}
		}
		p = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_AIR,(PurchaseSettingsDefClass::TEAM)PTTEAM(Team));
		if (p)
		{
			for (int j = 0; j < 10; j++)
			{
				if (p->Get_Definition(j))
				{
					UnitInfo *Temp = new UnitInfo(PurchaseSettingsDefClass::TYPE_AIR,j,p->Get_Definition(j),false);
					if (Is_In_Vector(Support,p->Get_Definition(j)))
					{
						Temp->Support = true;
					}
					if (Is_In_Vector(NavalIDs,p->Get_Definition(j)))
					{
						Temp->VehType = VEHTYPE_NAVAL;
					}
					else if (Get_Vehicle_Definition_Mode_By_ID(p->Get_Definition(j)) == VEHICLE_TYPE_FLYING)
					{
						Temp->VehType = VEHTYPE_FLYING;
					}
					else
					{
						Temp->VehType = VEHTYPE_GROUND;
					}
					Units.Add(Temp);
					unsigned int k;
					for (k = 0;k < 3;k++)
					{
						if (p->Get_Alt_Definition(j,k))
						{
							UnitInfo *Temp2 = new UnitInfo(PurchaseSettingsDefClass::TYPE_AIR,j,p->Get_Alt_Definition(j,k),true);
							if (Is_In_Vector(Support,p->Get_Alt_Definition(j,k)))
							{
								Temp2->Support = true;
							}
							if (Get_Vehicle_Definition_Mode_By_ID(p->Get_Alt_Definition(j,k)) == VEHICLE_TYPE_FLYING)
							{
								Temp2->VehType = VEHTYPE_FLYING;
							}
							else
							{
								Temp2->VehType = VEHTYPE_GROUND;
							}
							Units.Add(Temp2);
						}
					}
				}
			}
		}
	}
	char File[20];
	sprintf(File,"ExpVehFac_%u.cfg",Team);
	int FID = Commands->Text_File_Open(File);
	if (!FID)
	{
		FID = Commands->Text_File_Open("ExpVehFac.cfg");
	}
	if (FID)
	{
		char Data[128];
		while (Commands->Text_File_Get_String(FID,Data,127))
		{
			if (Data[0] != ';')
			{
				char *First = strtok(Data,"=");
				unsigned int Second = (unsigned int)atoi(strtok(0,"="));
				if (!_stricmp(First,"Ground_Limit"))
				{
					Ground_Limit = Second;
				}
				else if (!_stricmp(First,"Flying_Limit"))
				{
					Flying_Limit = Second;
				}
				else if (!_stricmp(First,"Naval_Limit"))
				{
					Naval_Limit = Second;
				}
				else if (!_stricmp(First,"Support_Limit"))
				{
					Support_Limit = Second;
				}
			}
		}
		unsigned int Limit = Ground_Limit+Flying_Limit+Naval_Limit+Support_Limit+(Find_Refinery(Team)?1:0);
		if (Limit > 1 && Limit > Get_Vehicle_Limit())
		{
			Set_Vehicle_Limit(Limit);
		}
		Commands->Text_File_Close(FID);
	}
	PHookID = AddVehiclePurchaseHook(Purchase_Hook,"");
	OHookStruct = new ObjectCreateHookStruct;
	OHookStruct->hook = Object_Hook;
	OHookStruct->data = 0;
	OHookID = AddObjectCreateHook(OHookStruct);
	Script = script;
	Loaded = true;
}
void ExpVehFacClass::Clear()
{
	RemoveVehiclePurchaseHook(PHookID);
	PHookID = 0;
	RemoveObjectCreateHook(OHookID);
	OHookID = 0;
	delete OHookStruct;
	OHookStruct = 0;
	Script = 0;
	Using_Naval = false;
	int x = Units.Count();
	for (int i = 0; i < x; i++)
	{
		if (Units[i])
		{
			delete Units[i];
		}
	}
	Units.Delete_All();
	x = Naval_Creation_Pos_Per_Vehicle.Count();
	for (int i = 0; i < x; i++)
	{
		if (Naval_Creation_Pos_Per_Vehicle[i])
		{
			delete Naval_Creation_Pos_Per_Vehicle[i];
		}
	}
	Naval_Creation_Pos_Per_Vehicle.Delete_All();
	Naval_Creation_Positions->Delete_All();
	delete Naval_Creation_Positions;
	Naval_Creation_Positions = 0;
	Flying_Landing_Positions->Delete_All();
	delete Flying_Landing_Positions;
	Flying_Landing_Positions = 0;
	Loaded = false;
}
int ExpVehFacClass::Purchase_Hook(BaseControllerClass *base, GameObject *purchaser, unsigned int cost, unsigned int preset,const char *data)
{
	int Team = Get_Object_Type(purchaser);
	if (!ExpVehFac[Team] || !ExpVehFac[Team]->Loaded)
	{
		return -1;
	}
	if (!Can_Team_Build_Vehicle(Team) || cost >= 100000 || !preset)
	{
		return 3;
	}
	UnitInfo *Temp = ExpVehFac[Team]->Get_Unit_Info(preset);
	if (Temp->VehType == VEHTYPE_NAVAL)
	{
		if (Temp->Support)
		{
			if (ExpVehFac[Team]->Support_Limit && ExpVehFac[Team]->Support_Amount >= ExpVehFac[Team]->Support_Limit)
			{
				Send_Message_Player(purchaser,255,255,255,"The limit for Support vehicles has been reached.");
				return 3;
			}
		}
		else if (ExpVehFac[Team]->Naval_Limit && ExpVehFac[Team]->Naval_Amount >= ExpVehFac[Team]->Naval_Limit)
		{
			Send_Message_Player(purchaser,255,255,255,"The limit for Naval vehicles has been reached.");
			return 3;
		}
		if (ExpVehFac[Team]->Is_Naval_Building)
		{
			return 3;
		}
		else if (Purchase_Item(purchaser,cost))
		{
			if (ExpVehFac[Team]->Naval_Build_Time[0] > 0.0f)
			{
				Set_Currently_Building(true,Team);
				Commands->Send_Custom_Event(0,ExpVehFac[Team]->Script,129310,Team,ExpVehFac[Team]->Naval_Build_Time[0]);
			}
			if (ExpVehFac[Team]->Naval_Build_Time[1] > 0.0f)
			{
				ExpVehFac[Team]->Set_Naval_Currently_Building(true);
				Commands->Send_Custom_Event(0,ExpVehFac[Team]->Script,129311,Team,ExpVehFac[Team]->Naval_Build_Time[1]);
			}
			GameObject *obj = ExpVehFac[Team]->Get_Naval_Creation_Position(preset);
			Vector3 Pos = Commands->Get_Position(obj);
			GameObject *Zone = Find_Closest_Zone(Pos,ZoneConstants::TYPE_VEHICLE_CONSTRUCTION);
			Damage_All_Objects_In_Zone(Zone,99999.0f,"None",0);
			GameObject *Veh = Commands->Create_Object(Get_Definition_Name(preset),Pos);
			Commands->Set_Facing(Veh,Commands->Get_Facing(obj));
			Commands->Send_Custom_Event(Veh,Veh,CUSTOM_EVENT_VEHICLE_OWNER,Commands->Get_ID(purchaser),0);
			return 0;
		}
		return 2;
	}
	else if (Temp->VehType == VEHTYPE_FLYING)
	{
		if (Temp->Support)
		{
			if (ExpVehFac[Team]->Support_Limit && ExpVehFac[Team]->Support_Amount >= ExpVehFac[Team]->Support_Limit)
			{
				Send_Message_Player(purchaser,255,255,255,"The limit for Support vehicles has been reached.");
				return 3;
			}
		}
		else if (ExpVehFac[Team]->Flying_Limit && ExpVehFac[Team]->Flying_Amount >= ExpVehFac[Team]->Flying_Limit)
		{
			Send_Message_Player(purchaser,255,255,255,"The limit for Flying vehicles has been reached.");
			return 3;
		}
		if (ExpVehFac[Team]->Is_Flying_Building)
		{
			return 3;
		}
		else if (Purchase_Item(purchaser,cost))
		{
			if (ExpVehFac[Team]->Flying_Build_Time[0] > 0.0f)
			{
				Set_Currently_Building(true,Team);
				Commands->Send_Custom_Event(0,ExpVehFac[Team]->Script,129310,Team,ExpVehFac[Team]->Flying_Build_Time[0]);
			}
			if (ExpVehFac[Team]->Flying_Build_Time[1] > 0.0f)
			{
				ExpVehFac[Team]->Set_Flying_Currently_Building(true);
				Commands->Send_Custom_Event(0,ExpVehFac[Team]->Script,129312,Team,ExpVehFac[Team]->Flying_Build_Time[1]);
			}
			ExpVehFac[Team]->LastFlyVehID = preset;
			ExpVehFac[Team]->LastFlyVehOwner = Commands->Get_ID(purchaser);
			Commands->Attach_Script(Commands->Create_Object("Invisible_Object",ExpVehFac[Team]->Get_Flying_Landing_Position()),"Test_Cinematic",Team?"ExpVehFac_1.txt":"ExpVehFac_0.txt");
			return 0;
		}
		return 2;
	}
	else
	{
		if (Temp->Support)
		{
			if (ExpVehFac[Team]->Support_Limit && ExpVehFac[Team]->Support_Amount >= ExpVehFac[Team]->Support_Limit)
			{
				Send_Message_Player(purchaser,255,255,255,"The limit for Support vehicles has been reached.");
				return 3;
			}
		}
		else if (ExpVehFac[Team]->Ground_Limit && ExpVehFac[Team]->Ground_Amount >= ExpVehFac[Team]->Ground_Limit)
		{
			Send_Message_Player(purchaser,255,255,255,"The limit for Ground vehicles has been reached.");
			return 3;
		}
		if (ExpVehFac[Team]->Is_Ground_Building)
		{
			return 3;
		}
		else if (Purchase_Item(purchaser,cost))
		{
			Create_Vehicle(Get_Definition_Name(preset),5.0f * Get_Build_Time_Multiplier(base->Get_Player_Type()),purchaser,base->Get_Player_Type());
			if (ExpVehFac[Team]->Ground_Build_Time[0] > 0.0f)
			{
				Set_Currently_Building(true,Team);
				Commands->Send_Custom_Event(0,ExpVehFac[Team]->Script,129310,Team,ExpVehFac[Team]->Ground_Build_Time[0]);
			}
			if (ExpVehFac[Team]->Ground_Build_Time[1] > 0.0f)
			{
				ExpVehFac[Team]->Set_Ground_Currently_Building(true);
				Commands->Send_Custom_Event(0,ExpVehFac[Team]->Script,129313,Team,ExpVehFac[Team]->Ground_Build_Time[1]);
			}
			return 0;
		}
		return 2;
	}
}
void ExpVehFacClass::Object_Hook(void *,GameObject *obj)
{
	if (obj->As_VehicleGameObj())
	{
		int Team = Get_Object_Type(obj);
		if (ExpVehFac[Team] && ExpVehFac[Team]->Loaded)
		{
			if (!Team || Team == 1)
			{
				if (!Is_Script_Attached(obj,"MDB_ExpVehFac_Limit"))
				{
					UnitInfo *Temp = ExpVehFac[Team]->Get_Unit_Info(Commands->Get_Preset_ID(obj));
					if (Temp)
					{
						if (ExpVehFac[Team]->Support_Limit && Temp->Support)
						{
							ExpVehFac[Team]->Support_Amount++;
							Commands->Attach_Script(obj,"MDB_ExpVehFac_Limit","4");
							if (ExpVehFac[Team]->Support_Amount >= ExpVehFac[Team]->Support_Limit)
							{
								ExpVehFac[Team]->Set_Support_Limit_Reached(true);
							}
						}
						else if (ExpVehFac[Team]->Ground_Limit && Temp->VehType == VEHTYPE_GROUND)
						{
							ExpVehFac[Team]->Ground_Amount++;
							Commands->Attach_Script(obj,"MDB_ExpVehFac_Limit","1");
							if (ExpVehFac[Team]->Ground_Amount >= ExpVehFac[Team]->Ground_Limit)
							{
								ExpVehFac[Team]->Set_Ground_Limit_Reached(true);
							}
						}
						else if (ExpVehFac[Team]->Flying_Limit && Temp->VehType == VEHTYPE_FLYING)
						{
							ExpVehFac[Team]->Flying_Amount++;
							Commands->Attach_Script(obj,"MDB_ExpVehFac_Limit","2");
							if (ExpVehFac[Team]->Flying_Amount >= ExpVehFac[Team]->Flying_Limit)
							{
								ExpVehFac[Team]->Set_Flying_Limit_Reached(true);
							}
						}
						else if (ExpVehFac[Team]->Naval_Limit && Temp->VehType == VEHTYPE_NAVAL)
						{
							ExpVehFac[Team]->Naval_Amount++;
							Commands->Attach_Script(obj,"MDB_ExpVehFac_Limit","3");
							if (ExpVehFac[Team]->Naval_Amount >= ExpVehFac[Team]->Naval_Limit)
							{
								ExpVehFac[Team]->Set_Naval_Limit_Reached(true);
							}
						}
					}
				}
			}
		}
	}
}
void MDB_ExpVehFac_Helipad::Created(GameObject *obj)
{
	int Team = Get_Object_Type(obj);
	if (!ExpVehFac[Team])
	{
		ExpVehFac[Team] = new ExpVehFacClass(Team);
	}
	if (!ExpVehFac[Team]->Loaded && !Find_Building_With_Script(Team,-1,"MDB_ExpVehFac_Naval",0))
	{
		ExpVehFac[Team]->Load(0,0,obj,Get_Parameter("Support_Vehicles"));
		Set_Can_Generate_Vehicles(Team,true);
	}
	if (ExpVehFac[Team]->Flying_Landing_Positions && !ExpVehFac[Team]->Flying_Landing_Positions->Count())
	{
		delete ExpVehFac[Team]->Flying_Landing_Positions;
		ExpVehFac[Team]->Flying_Landing_Positions = 0;
	}
	if (!ExpVehFac[Team]->Flying_Landing_Positions)
	{
		ExpVehFac[Team]->Flying_Landing_Positions = Get_All_Objects_By_Preset(2,Get_Parameter("Landing_Preset"));
	}
	ExpVehFac[Team]->Flying_Build_Time[0] = Get_Float_Parameter("Build_Time");
	ExpVehFac[Team]->Flying_Build_Time[1] = Get_Float_Parameter("Flying_Build_Time");
}

void MDB_ExpVehFac_Helipad::Killed(GameObject *obj,GameObject *killer)
{
	int Team = Get_Object_Type(obj);
	if (!Find_Building_With_Script(Team,-1,"MDB_ExpVehFac_Helipad",obj))
	{
		GameObject *WF = Find_Building_With_Script(Team,-1,"MDB_ExpVehFac_Vehicle_Factory",0);
		GameObject *Naval = Find_Building_With_Script(Team,-1,"MDB_ExpVehFac_Naval",0);
		if (!WF && !Naval)
		{
			Set_Can_Generate_Vehicles(Team,false);
		}
	}
}

void MDB_ExpVehFac_Helipad::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 129310)
	{
		Set_Currently_Building(false,param);
	}
	else if (type == 129311)
	{
		ExpVehFac[Get_Object_Type(obj)]->Set_Naval_Currently_Building(false);
	}
	else if (type == 129312)
	{
		ExpVehFac[Get_Object_Type(obj)]->Set_Flying_Currently_Building(false);
	}
	else if (type == 129313)
	{
		ExpVehFac[Get_Object_Type(obj)]->Set_Ground_Currently_Building(false);
	}
}

void MDB_ExpVehFac_Helipad::Destroyed(GameObject *obj)
{
	int Team = Get_Object_Type(obj);
	if (ExpVehFac[Team] && ExpVehFac[Team]->Loaded)
	{
		ExpVehFac[Team]->Clear();
	}
}

void MDB_ExpVehFac_Naval::Created(GameObject *obj)
{
	int Team = Get_Object_Type(obj);
	if (!ExpVehFac[Team])
	{
		ExpVehFac[Team] = new ExpVehFacClass(Team);
	}
	if (!ExpVehFac[Team]->Loaded)
	{
		if (strstr(Get_Parameter("Creation_Preset"),";"))
		{
			ExpVehFac[Team]->Load(Get_Parameter("Naval_Vehicles"),Get_Parameter("Creation_Preset"),obj,Get_Parameter("Support_Vehicles"));
		}
		else
		{
			ExpVehFac[Team]->Load(Get_Parameter("Naval_Vehicles"),0,obj,Get_Parameter("Support_Vehicles"));
			delete ExpVehFac[Team]->Naval_Creation_Positions;
			ExpVehFac[Team]->Naval_Creation_Positions = Get_All_Objects_By_Preset(2,Get_Parameter("Creation_Preset"));
		}
		Set_Can_Generate_Vehicles(Team,true);
	}
	ExpVehFac[Team]->Naval_Build_Time[0] = Get_Float_Parameter("Build_Time");
	ExpVehFac[Team]->Naval_Build_Time[1] = Get_Float_Parameter("Naval_Build_Time");
}

void MDB_ExpVehFac_Naval::Killed(GameObject *obj,GameObject *killer)
{
	int Team = Get_Object_Type(obj);
	if (!Find_Building_With_Script(Team,-1,"MDB_ExpVehFac_Naval",obj))
	{
		GameObject *WF = Find_Building_With_Script(Team,-1,"MDB_ExpVehFac_Vehicle_Factory",0);
		GameObject *Heli = Find_Building_With_Script(Team,-1,"MDB_ExpVehFac_Helipad",0);
		if (!WF && !Heli)
		{
			Set_Can_Generate_Vehicles(Team,false);
		}
	}
}

void MDB_ExpVehFac_Naval::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 129310)
	{
		Set_Currently_Building(false,param);
	}
	else if (type == 129311)
	{
		ExpVehFac[Get_Object_Type(obj)]->Set_Naval_Currently_Building(false);
	}
	else if (type == 129312)
	{
		ExpVehFac[Get_Object_Type(obj)]->Set_Flying_Currently_Building(false);
	}
	else if (type == 129313)
	{
		ExpVehFac[Get_Object_Type(obj)]->Set_Ground_Currently_Building(false);
	}
}

void MDB_ExpVehFac_Naval::Destroyed(GameObject *obj)
{
	int Team = Get_Object_Type(obj);
	if (ExpVehFac[Team] && ExpVehFac[Team]->Loaded)
	{
		ExpVehFac[Team]->Clear();
	}
}

void MDB_ExpVehFac_Vehicle_Factory::Destroyed(GameObject *obj)
{
	int Team = Get_Object_Type(obj);
	if (ExpVehFac[Team] && ExpVehFac[Team]->Loaded)
	{
		ExpVehFac[Team]->Clear();
	}
}

void MDB_ExpVehFac_Vehicle_Factory::Created(GameObject *obj)
{
	int Team = Get_Object_Type(obj);
	if (!ExpVehFac[Team])
	{
		ExpVehFac[Team] = new ExpVehFacClass(Team);
	}
	if (!ExpVehFac[Team]->Loaded && !Find_Building_With_Script(Team,-1,"MDB_ExpVehFac_Naval",0))
	{
		ExpVehFac[Team]->Load(0,0,obj,Get_Parameter("Support_Vehicles"));
		Set_Can_Generate_Vehicles(Team,true);
	}
	ExpVehFac[Team]->Ground_Build_Time[0] = Get_Float_Parameter("Build_Time");
	ExpVehFac[Team]->Ground_Build_Time[1] = Get_Float_Parameter("Ground_Build_Time");
}

void MDB_ExpVehFac_Vehicle_Factory::Killed(GameObject *obj,GameObject *killer)
{
	int Team = Get_Object_Type(obj);
	GameObject *WF = Find_Building_With_Script(Team,-1,"MDB_ExpVehFac_Vehicle_Factory",obj);
	GameObject *Naval = Find_Building_With_Script(Team,-1,"MDB_ExpVehFac_Naval",0);
	GameObject *Heli = Find_Building_With_Script(Team,-1,"MDB_ExpVehFac_Helipad",0);
	if (Naval || Heli)
	{
		Commands->Start_Timer(obj,this,0.5f,1);
	}
	else if (!WF)
	{
		Set_Can_Generate_Vehicles(Team,false);
	}
}

void MDB_ExpVehFac_Vehicle_Factory::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 129310)
	{
		Set_Currently_Building(false,param);
	}
	else if (type == 129311)
	{
		ExpVehFac[Get_Object_Type(obj)]->Set_Naval_Currently_Building(false);
	}
	else if (type == 129312)
	{
		ExpVehFac[Get_Object_Type(obj)]->Set_Flying_Currently_Building(false);
	}
	else if (type == 129313)
	{
		ExpVehFac[Get_Object_Type(obj)]->Set_Ground_Currently_Building(false);
	}
}

void MDB_ExpVehFac_Vehicle_Factory::Timer_Expired(GameObject *obj, int number)
{
	Set_Can_Generate_Vehicles(Get_Object_Type(obj),true);
}

void MDB_ExpVehFac_Cinematic::Created(GameObject *obj)
{
	int Team = Get_Int_Parameter("Team");
	Commands->Set_Player_Type(obj,Team);
	GameObject *Veh = Commands->Create_Object(Get_Definition_Name(ExpVehFac[Team]->LastFlyVehID),Commands->Get_Position(obj));
	Commands->Attach_To_Object_Bone(Veh,obj,"Origin");
	Commands->Attach_Script(Veh,"MDB_ExpVehFac_Cinematic_Vehicle","");
	Commands->Enable_Engine(Veh,true);
	Commands->Set_Is_Visible(Veh,false);
	Commands->Send_Custom_Event(Veh,Veh,CUSTOM_EVENT_VEHICLE_OWNER,ExpVehFac[Team]->LastFlyVehOwner,0);
	ExpVehFac[Team]->LastFlyVehID = 0;
	ExpVehFac[Team]->LastFlyVehOwner = 0;
	ID = Commands->Get_ID(Veh);
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),0);
}

void MDB_ExpVehFac_Cinematic::Timer_Expired(GameObject *obj,int number)
{
	GameObject *Veh = Commands->Find_Object(ID);
	Commands->Attach_To_Object_Bone(Veh,0,0);
	Remove_Script(Veh,"MDB_ExpVehFac_Cinematic_Vehicle");
	Commands->Enable_Engine(Veh,false);
	Commands->Set_Is_Visible(Veh,true);
}

void MDB_ExpVehFac_Cinematic::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ID,4,1);
}

void MDB_ExpVehFac_Cinematic_Vehicle::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
	Commands->Set_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj));
}

void MDB_ExpVehFac_Limit::Created(GameObject *obj)
{
	Team = Get_Object_Type(obj);
}

void MDB_ExpVehFac_Limit::Destroyed(GameObject *obj)
{
	int Type = Get_Int_Parameter("Type");
	if (Type == VEHTYPE_GROUND)
	{
		ExpVehFac[Team]->Ground_Amount--;
		if (ExpVehFac[Team]->Ground_Amount == ExpVehFac[Team]->Ground_Limit-1)
		{
			ExpVehFac[Team]->Set_Ground_Limit_Reached(false);
		}
	}
	else if (Type == VEHTYPE_FLYING)
	{
		ExpVehFac[Team]->Flying_Amount--;
		if (ExpVehFac[Team]->Flying_Amount == ExpVehFac[Team]->Flying_Limit-1)
		{
			ExpVehFac[Team]->Set_Flying_Limit_Reached(false);
		}
	}
	else if (Type == VEHTYPE_NAVAL)
	{
		ExpVehFac[Team]->Naval_Amount--;
		if (ExpVehFac[Team]->Naval_Amount == ExpVehFac[Team]->Naval_Limit-1)
		{
			ExpVehFac[Team]->Set_Naval_Limit_Reached(false);
		}
	}
	else if (Type == VEHTYPE_SUPPORT)
	{
		ExpVehFac[Team]->Support_Amount--;
		if (ExpVehFac[Team]->Support_Amount == ExpVehFac[Team]->Support_Limit-1)
		{
			ExpVehFac[Team]->Set_Support_Limit_Reached(false);
		}
	}
}

void MDB_ExpVehFac_Limit::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&Team,4,1);
}
ScriptRegistrant<MDB_ExpVehFac_Helipad> MDB_ExpVehFac_Helipad_Registrant("MDB_ExpVehFac_Helipad","Landing_Preset=Landing:string,Build_Time=1.0:float,Flying_Build_Time=1.0:float,Remove_on_Death=0:int,Support_Vehicles=Veh1;Veh2;etc.:string,Sidebar=0:int");
ScriptRegistrant<MDB_ExpVehFac_Naval> MDB_ExpVehFac_Naval_Registrant("MDB_ExpVehFac_Naval","Creation_Preset=Preset:string,Naval_Vehicles=Veh1;Veh2;etc.:string,Build_Time=1.0:float,Naval_Build_Time=1.0:float,Remove_on_Death=0:int,Support_Vehicles=Veh1;Veh2;etc.:string,Sidebar=0:int");
ScriptRegistrant<MDB_ExpVehFac_Vehicle_Factory> MDB_ExpVehFac_Vehicle_Factory_Registrant("MDB_ExpVehFac_Vehicle_Factory","Build_Time=1.0:float,Ground_Build_Time=1.0:float,Remove_on_Death=0:int,Support_Vehicles=Veh1;Veh2;etc.:string,Sidebar=0:int");
ScriptRegistrant<MDB_ExpVehFac_Cinematic> MDB_ExpVehFac_Cinematic_Registrant("MDB_ExpVehFac_Cinematic","Team=0:int,Time:float");
ScriptRegistrant<MDB_ExpVehFac_Cinematic_Vehicle> MDB_ExpVehFac_Cinematic_Vehicle_Registrant("MDB_ExpVehFac_Cinematic_Vehicle","");
ScriptRegistrant<MDB_ExpVehFac_Limit> MDB_ExpVehFac_Limit_Registrant("MDB_ExpVehFac_Limit","Type:int");
