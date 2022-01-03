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
#include "cAMpaScripts.h"

void cAMpa_Warn_Zone_Team::Created(GameObject *obj) 
{
}

void cAMpa_Warn_Zone_Team::Entered(GameObject *obj,GameObject *enterer)
{
	int paraTeam = Get_Int_Parameter("Player_Type");
	if (Get_Player_Type(enterer) == paraTeam)
	{
		char input[256];
		sprintf(input,"ppage %d You entered the Zone for possible Spawnkill, stay back.", Get_Player_ID(enterer));
		Console_Input(input);
	}
}

void cAMpa_Kill_Zone_Team::Created(GameObject *obj) 
{
}

void cAMpa_Kill_Zone_Team::Entered(GameObject *obj,GameObject *enterer)
{
	int paraTeam = Get_Int_Parameter("Player_Type");
	if (Get_Player_Type(enterer) == paraTeam)
	{
		char input[256];
		sprintf(input,"ppage %d I warned you, its your K/D :-P", Get_Player_ID(enterer));
		Console_Input(input);
		Commands->Apply_Damage(enterer,1000,"Death",0);
	}
}

void cAMpa_Teleport_Zone_Small::Created(GameObject *obj) 
{
	lastUsed = 0;
}

void cAMpa_Teleport_Zone_Small::Entered(GameObject *obj,GameObject *enterer)
{
	if (The_Game()->Get_Current_Players() < 11)
	{
		int Random;
		newRandom:
		Random = Commands->Get_Random_Int(1,5);
		if (Random == lastUsed)
			goto newRandom;
		if (Random == 1)
		{
			Vector3 telePos = Get_Vector3_Parameter("Location1");
			Commands->Set_Position(enterer,telePos);
		}
		else if (Random == 2)
		{
			Vector3 telePos = Get_Vector3_Parameter("Location2");
			Commands->Set_Position(enterer,telePos);
		}
		else if (Random == 3)
		{
			Vector3 telePos = Get_Vector3_Parameter("Location3");
			Commands->Set_Position(enterer,telePos);
		}
		else if (Random == 4)
		{
			Vector3 telePos = Get_Vector3_Parameter("Location4");
			Commands->Set_Position(enterer,telePos);
		}
		else
		{
			Vector3 telePos = Get_Vector3_Parameter("Location5");
			Commands->Set_Position(enterer,telePos);
		}
		lastUsed = Random;
	}
	else
	{
		char input[256];
		sprintf(input,"ppage %d There are currently more then 10 Players, please use the other Teleporter.", Get_Player_ID(enterer));
		Console_Input(input);
	}
}

void cAMpa_Teleport_Zone_Large::Created(GameObject *obj) 
{
	lastUsed = 0;
}

void cAMpa_Teleport_Zone_Large::Entered(GameObject *obj,GameObject *enterer)
{
	if (The_Game()->Get_Current_Players() >= 11)
	{
		int Random;
		newRandom:
		Random = Commands->Get_Random_Int(1,10);
		if (Random == lastUsed)
			goto newRandom;
		if (Random == 1)
		{
			Vector3 telePos = Get_Vector3_Parameter("Location1");
			Commands->Set_Position(enterer,telePos);
		}
		else if (Random == 2)
		{
			Vector3 telePos = Get_Vector3_Parameter("Location2");
			Commands->Set_Position(enterer,telePos);
		}
		else if (Random == 3)
		{
			Vector3 telePos = Get_Vector3_Parameter("Location3");
			Commands->Set_Position(enterer,telePos);
		}
		else if (Random == 4)
		{
			Vector3 telePos = Get_Vector3_Parameter("Location4");
			Commands->Set_Position(enterer,telePos);
		}
		else if (Random == 5)
		{
			Vector3 telePos = Get_Vector3_Parameter("Location5");
			Commands->Set_Position(enterer,telePos);
		}
		else if (Random == 6)
		{
			Vector3 telePos = Get_Vector3_Parameter("Location6");
			Commands->Set_Position(enterer,telePos);
		}
		else if (Random == 7)
		{
			Vector3 telePos = Get_Vector3_Parameter("Location7");
			Commands->Set_Position(enterer,telePos);
		}
		else if (Random == 8)
		{
			Vector3 telePos = Get_Vector3_Parameter("Location8");
			Commands->Set_Position(enterer,telePos);
		}
		else if (Random == 9)
		{
			Vector3 telePos = Get_Vector3_Parameter("Location9");
			Commands->Set_Position(enterer,telePos);
		}
		else
		{
			Vector3 telePos = Get_Vector3_Parameter("Location10");
			Commands->Set_Position(enterer,telePos);
		}
		lastUsed = Random;
	}
	else
	{
		char input[256];
		sprintf(input,"ppage %d There are currently less then 10 Players, please use the other Teleporter.", Get_Player_ID(enterer));
		Console_Input(input);
	}
}

ScriptRegistrant<cAMpa_Teleport_Zone_Large> cAMpa_Teleport_Zone_Large_Registrant("cAMpa_Teleport_Zone_Large","Location1:vector3,Location2:vector3,Location3:vector3,Location4:vector3,Location5:vector3,Location6:vector3,Location7:vector3,Location8:vector3,Location9:vector3,Location10:vector3");
ScriptRegistrant<cAMpa_Teleport_Zone_Small> cAMpa_Teleport_Zone_Small_Registrant("cAMpa_Teleport_Zone_Small","Location1:vector3,Location2:vector3,Location3:vector3,Location4:vector3,Location5:vector3");
ScriptRegistrant<cAMpa_Warn_Zone_Team> cAMpa_Warn_Zone_Team_Registrant("cAMpa_Warn_Zone_Team","Player_Type:int");
ScriptRegistrant<cAMpa_Kill_Zone_Team> cAMpa_Kill_Zone_Team_Registrant("cAMpa_Kill_Zone_Team","Player_Type:int");
