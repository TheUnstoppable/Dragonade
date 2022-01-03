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
#include "kak.h"

void KAK_Warp_On_Pickup::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Vector3 WarpPosition = Get_Vector3_Parameter("Position");
		Commands->Set_Position(sender,WarpPosition);
	}
}

void KAK_Precipitation_Create::Created(GameObject *obj)
{
	if (Get_Int_Parameter("OnCreate") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
		{
			if (Get_Int_Parameter("Reset_Precipitation") > 0)
			{
				Commands->Set_Snow(0.0f,0.0f,true);
				Commands->Set_Ash(0.0f,0.0f,true);
				Commands->Set_Rain(0.0f,0.0f,true);
			}
			if (!_stricmp(Get_Parameter("Type"),"Snow")) Commands->Set_Snow(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
			else if (!_stricmp(Get_Parameter("Type"),"Ash")) Commands->Set_Ash(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
			else if (!_stricmp(Get_Parameter("Type"),"Rain")) Commands->Set_Rain(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
			else
			{
				Commands->Set_Snow(0.0f,0.0f,true);
				Commands->Set_Ash(0.0f,0.0f,true);
				Commands->Set_Rain(0.0f,0.0f,true);
			}
		}
	}
}

void KAK_Precipitation_Create::Destroyed(GameObject *obj)
{
	if (Get_Int_Parameter("OnDestroy") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
		{
			if (Get_Int_Parameter("Reset_Precipitation") > 0)
			{
				Commands->Set_Snow(0.0f,0.0f,true);
				Commands->Set_Ash(0.0f,0.0f,true);
				Commands->Set_Rain(0.0f,0.0f,true);
			}
			if (!_stricmp(Get_Parameter("Type"),"Snow")) Commands->Set_Snow(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
			else if (!_stricmp(Get_Parameter("Type"),"Ash")) Commands->Set_Ash(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
			else if (!_stricmp(Get_Parameter("Type"),"Rain")) Commands->Set_Rain(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
			else
			{
				Commands->Set_Snow(0.0f,0.0f,true);
				Commands->Set_Ash(0.0f,0.0f,true);
				Commands->Set_Rain(0.0f,0.0f,true);
			}
		}
	}
}

void KAK_Precipitation_Create::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (Get_Int_Parameter("Reset_Precipitation") > 0)
		{
			Commands->Set_Snow(0.0f,0.0f,true);
			Commands->Set_Ash(0.0f,0.0f,true);
			Commands->Set_Rain(0.0f,0.0f,true);
		}
		if (!_stricmp(Get_Parameter("Type"),"Snow")) Commands->Set_Snow(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
		else if (!_stricmp(Get_Parameter("Type"),"Ash")) Commands->Set_Ash(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
		else if (!_stricmp(Get_Parameter("Type"),"Rain")) Commands->Set_Rain(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
		else
		{
			Commands->Set_Snow(0.0f,0.0f,true);
			Commands->Set_Ash(0.0f,0.0f,true);
			Commands->Set_Rain(0.0f,0.0f,true);
		}
	}
}

void KAK_Precipitation_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (Get_Int_Parameter("OnEnter") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
		{
			if (Get_Int_Parameter("Reset_Precipitation") > 0)
			{
				Commands->Set_Snow(0.0f,0.0f,true);
				Commands->Set_Ash(0.0f,0.0f,true);
				Commands->Set_Rain(0.0f,0.0f,true);
			}
			if (!_stricmp(Get_Parameter("Type"),"Snow")) Commands->Set_Snow(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
			else if (!_stricmp(Get_Parameter("Type"),"Ash")) Commands->Set_Ash(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
			else if (!_stricmp(Get_Parameter("Type"),"Rain")) Commands->Set_Rain(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
			else
			{
				Commands->Set_Snow(0.0f,0.0f,true);
				Commands->Set_Ash(0.0f,0.0f,true);
				Commands->Set_Rain(0.0f,0.0f,true);
			}
		}
	}
}

void KAK_Precipitation_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (Get_Int_Parameter("OnExit") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
		{
			if (Get_Int_Parameter("Reset_Precipitation") > 0)
			{
				Commands->Set_Snow(0.0f,0.0f,true);
				Commands->Set_Ash(0.0f,0.0f,true);
				Commands->Set_Rain(0.0f,0.0f,true);
			}
			if (!_stricmp(Get_Parameter("Type"),"Snow")) Commands->Set_Snow(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
			else if (!_stricmp(Get_Parameter("Type"),"Ash")) Commands->Set_Ash(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
			else if (!_stricmp(Get_Parameter("Type"),"Rain")) Commands->Set_Rain(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
			else
			{
				Commands->Set_Snow(0.0f,0.0f,true);
				Commands->Set_Ash(0.0f,0.0f,true);
				Commands->Set_Rain(0.0f,0.0f,true);
			}
		}
	}
}

void KAK_Precipitation_Zone::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (Get_Int_Parameter("Reset_Precipitation") > 0)
		{
			Commands->Set_Snow(0.0f,0.0f,true);
			Commands->Set_Ash(0.0f,0.0f,true);
			Commands->Set_Rain(0.0f,0.0f,true);
		}
		if (!_stricmp(Get_Parameter("Type"),"Snow")) Commands->Set_Snow(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
		else if (!_stricmp(Get_Parameter("Type"),"Ash")) Commands->Set_Ash(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
		else if (!_stricmp(Get_Parameter("Type"),"Rain")) Commands->Set_Rain(Get_Float_Parameter("Density"),Get_Float_Parameter("Density"),true);
		else
		{
			Commands->Set_Snow(0.0f,0.0f,true);
			Commands->Set_Ash(0.0f,0.0f,true);
			Commands->Set_Rain(0.0f,0.0f,true);
		}
	}
}

void KAK_Wind_Create::Created(GameObject *obj)
{
	if (Get_Int_Parameter("OnCreate") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
			Commands->Set_Wind(Get_Float_Parameter("Wind_Heading"),Get_Float_Parameter("Wind_Speed"),Get_Float_Parameter("Wind_Variability"),1.0f);
	}
}

void KAK_Wind_Create::Destroyed(GameObject *obj)
{
	if (Get_Int_Parameter("OnDestroy") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
			Commands->Set_Wind(Get_Float_Parameter("Wind_Heading"),Get_Float_Parameter("Wind_Speed"),Get_Float_Parameter("Wind_Variability"),1.0f);
	}
}

void KAK_Wind_Create::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
		Commands->Set_Wind(Get_Float_Parameter("Wind_Heading"),Get_Float_Parameter("Wind_Speed"),Get_Float_Parameter("Wind_Variability"),1.0f);
}

void KAK_Wind_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (Get_Int_Parameter("OnEnter") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
			Commands->Set_Wind(Get_Float_Parameter("Wind_Heading"),Get_Float_Parameter("Wind_Speed"),Get_Float_Parameter("Wind_Variability"),1.0f);
	}
}

void KAK_Wind_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (Get_Int_Parameter("OnExit") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
			Commands->Set_Wind(Get_Float_Parameter("Wind_Heading"),Get_Float_Parameter("Wind_Speed"),Get_Float_Parameter("Wind_Variability"),1.0f);
	}
}

void KAK_Wind_Zone::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
		Commands->Set_Wind(Get_Float_Parameter("Wind_Heading"),Get_Float_Parameter("Wind_Speed"),Get_Float_Parameter("Wind_Variability"),1.0f);
}

void KAK_Clouds_Create::Created(GameObject *obj)
{
	if (Get_Int_Parameter("OnCreate") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
			Commands->Set_Clouds(Get_Float_Parameter("Cloud_Cover"),Get_Float_Parameter("Cloud_Gloominess"),1.0f);
	}
}

void KAK_Clouds_Create::Destroyed(GameObject *obj)
{
	if (Get_Int_Parameter("OnDestroy") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
			Commands->Set_Clouds(Get_Float_Parameter("Cloud_Cover"),Get_Float_Parameter("Cloud_Gloominess"),1.0f);
	}
}

void KAK_Clouds_Create::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
		Commands->Set_Clouds(Get_Float_Parameter("Cloud_Cover"),Get_Float_Parameter("Cloud_Gloominess"),1.0f);
}

void KAK_Clouds_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (Get_Int_Parameter("OnEnter") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
			Commands->Set_Clouds(Get_Float_Parameter("Cloud_Cover"),Get_Float_Parameter("Cloud_Gloominess"),1.0f);
	}
}

void KAK_Clouds_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (Get_Int_Parameter("OnExit") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
			Commands->Set_Clouds(Get_Float_Parameter("Cloud_Cover"),Get_Float_Parameter("Cloud_Gloominess"),1.0f);
	}
}

void KAK_Clouds_Zone::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
		Commands->Set_Clouds(Get_Float_Parameter("Cloud_Cover"),Get_Float_Parameter("Cloud_Gloominess"),1.0f);
}

void KAK_Lightning_Create::Created(GameObject *obj)
{
	if (Get_Int_Parameter("OnCreate") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
			Commands->Set_Lightning(Get_Float_Parameter("Lightning_Intensity"),Get_Float_Parameter("Start_Distance"),Get_Float_Parameter("End_Distance"),Get_Float_Parameter("Lightning_Heading"),Get_Float_Parameter("Lightning_Distribution"),1.0f);
	}
}

void KAK_Lightning_Create::Destroyed(GameObject *obj)
{
	if (Get_Int_Parameter("OnDestroy") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else 
			Commands->Set_Lightning(Get_Float_Parameter("Lightning_Intensity"),Get_Float_Parameter("Start_Distance"),Get_Float_Parameter("End_Distance"),Get_Float_Parameter("Lightning_Heading"),Get_Float_Parameter("Lightning_Distribution"),1.0f);
	}
}

void KAK_Lightning_Create::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
		Commands->Set_Lightning(Get_Float_Parameter("Lightning_Intensity"),Get_Float_Parameter("Start_Distance"),Get_Float_Parameter("End_Distance"),Get_Float_Parameter("Lightning_Heading"),Get_Float_Parameter("Lightning_Distribution"),1.0f);
}

void KAK_Lightning_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (Get_Int_Parameter("OnEnter") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
			Commands->Set_Lightning(Get_Float_Parameter("Lightning_Intensity"),Get_Float_Parameter("Start_Distance"),Get_Float_Parameter("End_Distance"),Get_Float_Parameter("Lightning_Heading"),Get_Float_Parameter("Lightning_Distribution"),1.0f);
	}
}

void KAK_Lightning_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (Get_Int_Parameter("OnExit") > 0)
	{
		if (Get_Float_Parameter("Delay") > 0)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
		else
			Commands->Set_Lightning(Get_Float_Parameter("Lightning_Intensity"),Get_Float_Parameter("Start_Distance"),Get_Float_Parameter("End_Distance"),Get_Float_Parameter("Lightning_Heading"),Get_Float_Parameter("Lightning_Distribution"),1.0f);
	}
}

void KAK_Lightning_Zone::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
		Commands->Set_Lightning(Get_Float_Parameter("Lightning_Intensity"),Get_Float_Parameter("Start_Distance"),Get_Float_Parameter("End_Distance"),Get_Float_Parameter("Lightning_Heading"),Get_Float_Parameter("Lightning_Distribution"),1.0f);
}

void KAK_Flowing_Weather::Created(GameObject *obj)
{
	Range = Commands->Get_Random(0,1);
	CurrentClouds = Commands->Get_Random(0,1);
	Commands->Set_Clouds(CurrentClouds,CurrentClouds,1.0f);
	CurrentPrecipitation = 0.0f;
	CurrentWind = 0.0f;
	CurrentWindHeading = Commands->Get_Random(0,360);
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Speed"),0);
}

void KAK_Flowing_Weather::Timer_Expired(GameObject *obj,int number)
{
	if (!number)
	{
		if (CurrentClouds < Range)
		{
			if ((CurrentClouds + 0.001f) >= Range) CurrentClouds = Range;
			else
			{
				CurrentClouds += 0.001f;
				CurrentWindHeading += 0.01f;
			}
		}
		else if (CurrentClouds > Range)
		{
			if ((CurrentClouds - 0.001f) <= Range) CurrentClouds = Range;
			else
			{
				CurrentClouds -= 0.001f;
				CurrentWindHeading -= 0.01f;
			}
		}

		Commands->Set_Clouds(CurrentClouds,CurrentClouds,1.0f);
		CurrentWind = Get_Float_Parameter("Wind_Variablity")*CurrentClouds;
		Commands->Set_Wind(CurrentWindHeading,CurrentWind,1.0f,1.0f);

		if (CurrentClouds >= Get_Float_Parameter("Minimum_Precipitation_Start"))
		{
			CurrentPrecipitation = ((Get_Float_Parameter("Max_Precipitation_Density") / (1 - Get_Float_Parameter("Minimum_Precipitation_Start")))*(CurrentClouds + 1 - Get_Float_Parameter("Minimum_Precipitation_Start"))) - (Get_Float_Parameter("Max_Precipitation_Density") / (1 - Get_Float_Parameter("Minimum_Precipitation_Start")));
			if (!_stricmp(Get_Parameter("Precipitation_Type"),"Ash")) Commands->Set_Ash(CurrentPrecipitation,CurrentPrecipitation,true);
			else if (!_stricmp(Get_Parameter("Precipitation_Type"),"Snow")) Commands->Set_Snow(CurrentPrecipitation,CurrentPrecipitation,true);
			else if (!_stricmp(Get_Parameter("Precipitation_Type"),"Rain")) Commands->Set_Rain(CurrentPrecipitation,CurrentPrecipitation,true);
		}
		else
		{
			if (!_stricmp(Get_Parameter("Precipitation_Type"),"Ash")) Commands->Set_Ash(0.0f,0.0f,true);
			else if (!_stricmp(Get_Parameter("Precipitation_Type"),"Snow")) Commands->Set_Snow(0.0f,0.0f,true);
			else if (!_stricmp(Get_Parameter("Precipitation_Type"),"Rain")) Commands->Set_Rain(0.0f,0.0f,true);
		}

		if (CurrentClouds >= Get_Float_Parameter("Minimum_Lightning_Start"))
		{
			CurrentLightning = ((Get_Float_Parameter("Maximum_Lightning_Intensity") / (1 - Get_Float_Parameter("Minimum_Lightning_Start")))*(CurrentClouds + 1 - Get_Float_Parameter("Minimum_Lightning_Start"))) - (Get_Float_Parameter("Maximum_Lightning_Intensity") / (1 - Get_Float_Parameter("Minimum_Lightning_Start")));
			Commands->Set_Lightning(CurrentLightning,0.0f,1.0f,0.0f,1.0f,1.0f);
		}
		else Commands->Set_Lightning(0.0f,0.0f,1.0f,0.0f,1.0f,1.0f);
		
		if (CurrentClouds == Range) Range = Commands->Get_Random(0,1);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Speed"),0);
	}
}

void KAK_Flowing_Weather::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&Range,4,1);
	Auto_Save_Variable(&CurrentClouds,4,2);
	Auto_Save_Variable(&CurrentPrecipitation,4,3);
	Auto_Save_Variable(&CurrentLightning,4,4);
	Auto_Save_Variable(&CurrentWind,4,5);
	Auto_Save_Variable(&CurrentWindHeading,4,6);
}

void KAK_Give_Powerup_On_Pickup::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Commands->Give_PowerUp(sender,Get_Parameter("Powerup_Preset"),true);
	}
}

void KAK_Select_Weapon_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (Get_Int_Parameter("OnEnter"))
	{
		Commands->Select_Weapon(enterer,Get_Parameter("Weapon_Preset"));
	}
}

void KAK_Select_Weapon_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (Get_Int_Parameter("OnExit"))
	{
		Commands->Select_Weapon(exiter,Get_Parameter("Weapon_Preset"));
	}
}

void KAK_Select_Weapon_On_Pickup::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Commands->Select_Weapon(sender,Get_Parameter("Weapon_Preset"));
	}
}

void KAK_Harvester_Logic::Created(GameObject *obj)
{
	enabled = true;
	HarvyID = 0;
	Commands->Send_Custom_Event(obj,obj,100001,0,0);
}

void KAK_Harvester_Logic::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if ((type == 100001) && (enabled))
	{
		float facing = Commands->Get_Facing(obj);
		Commands->Create_Explosion(Get_Parameter("Explosion_Preset"),Commands->Get_Position(obj),0);
		GameObject *Harvy = Commands->Create_Object(Get_Parameter("Harvester_Preset"),Commands->Get_Position(obj));
		Commands->Set_Facing(Harvy,facing);
		Commands->Send_Custom_Event(obj,Harvy,100004,(int &)facing,0);
	}
	else if (type == 100002)
	{
		if (HarvyID)
		{
			Commands->Apply_Damage(Commands->Find_Object(HarvyID),99999,"Explosive",0);
			Commands->Destroy_Object(obj);
		}
	}
	else if (type == 100003)
	{
		HarvyID = param;
	}
	else if (type == 100005)
	{
		enabled = false;
	}
	else if (type == 100006)
	{
		HarvyID = 0;
	}
}

void KAK_Harvester_Logic::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&HarvyID,4,1);
	Auto_Save_Variable(&enabled,1,2);
}

void KAK_Harvester_Attached::Created(GameObject *obj)
{
	End_Timer = false;
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("LogicID")),100003,Commands->Get_ID(obj),0);
	if (Get_Int_Parameter("Number_Of_Paths") > 3) PathNumber = Commands->Get_Random_Int(1,4);
	else PathNumber = Commands->Get_Random_Int(1,(Get_Int_Parameter("Number_Of_Paths") + 1));
	Vector3 CrystalRelPos = Vector3(0,0,0);
	Vector3 CrystalPos;
	ActionParamsStruct HarvesterToField;
	HarvesterToField.MoveArrivedDistance = 100;
	HarvesterToField.Set_Basic(this,100,1);
	if (PathNumber == 1)
	{
		HarvesterToField.Set_Movement(0,1.0f,0.1f);
		HarvesterToField.WaypathID = Get_Int_Parameter("Waypath1_Dock2Field");
		HarvesterToField.WaypathSplined = true;
		HarvesterToField.AttackActive = false;
		HarvesterToField.AttackCheckBlocked = false;
		CrystalRelPos = Get_Vector3_Parameter("Waypath1_Tib_Zone");
	}
	else if (PathNumber == 2)
	{
		HarvesterToField.Set_Movement(0,1.0f,0.1f);
		HarvesterToField.WaypathID = Get_Int_Parameter("Waypath2_Dock2Field");
		HarvesterToField.WaypathSplined = true;
		HarvesterToField.AttackActive = false;
		HarvesterToField.AttackCheckBlocked = false;
		CrystalRelPos = Get_Vector3_Parameter("Waypath2_Tib_Zone");
	}
	else if (PathNumber == 3)
	{
		HarvesterToField.Set_Movement(0,1.0f,0.1f);
		HarvesterToField.WaypathID = Get_Int_Parameter("Waypath3_Dock2Field");
		HarvesterToField.WaypathSplined = true;
		HarvesterToField.AttackActive = false;
		HarvesterToField.AttackCheckBlocked = false;
		CrystalRelPos = Get_Vector3_Parameter("Waypath3_Tib_Zone");
	}
	if (_stricmp(Get_Parameter("Crystal_Preset"),"0"))
	{
		for (int x = 1;x <= 10;x++)
		{
			int XChange,YChange;
			CrystalPos = CrystalRelPos;
			XChange = Commands->Get_Random_Int(1,3);
			YChange = Commands->Get_Random_Int(1,3);
			if (XChange == 1) CrystalPos.X += Commands->Get_Random(0.0f,5.001f);
			else if (XChange == 2) CrystalPos.X -= Commands->Get_Random(0.0f,5.001f);
			if (YChange == 1) CrystalPos.Y += Commands->Get_Random(0.0f,5.001f);
			else if (YChange == 2) CrystalPos.Y -= Commands->Get_Random(0.0f,5.001f);
			if (x == 1)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal1 = Commands->Get_ID(Crystal);
			}
			else if (x == 2)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal2 = Commands->Get_ID(Crystal);
			}
			else if (x == 3)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal3 = Commands->Get_ID(Crystal);
			}
			else if (x == 4)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal4 = Commands->Get_ID(Crystal);
			}
			else if (x == 5)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal5 = Commands->Get_ID(Crystal);
			}
			else if (x == 6)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal6 = Commands->Get_ID(Crystal);
			}
			else if (x == 7)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal7 = Commands->Get_ID(Crystal);
			}
			else if (x == 8)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal8 = Commands->Get_ID(Crystal);
			}
			else if (x == 9)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal9 = Commands->Get_ID(Crystal);
			}
			else if (x == 10)
			{
				GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
				Commands->Disable_All_Collisions(Crystal);
				Crystal10 = Commands->Get_ID(Crystal);
			}
		}
	}
	Commands->Action_Goto(obj,HarvesterToField);
	Commands->Start_Timer(obj,this,0.1f,5);
}

void KAK_Harvester_Attached::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	if (action_id == 1)
	{
		if (!complete_reason) 
		{
			const char *harvest_animation = Get_Parameter("Harvest_Animation");
			if (_stricmp(harvest_animation,"0"))
			{
				Commands->Set_Animation(obj,harvest_animation,true,0,0.0f,-1.0f,false);
			}
		}
		Commands->Start_Timer(obj,this,(Get_Float_Parameter("Tiberium_Idle_Time") / 10),1);
		Commands->Start_Timer(obj,this,(Get_Float_Parameter("Tiberium_Idle_Time") / 10),4);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Tiberium_Idle_Time"),2);
	}
	else if (action_id == 2)
	{
		if (!complete_reason) 
		{
			const char *harvest_animation = Get_Parameter("Harvest_Animation");
			if (_stricmp(harvest_animation,"0"))
			{
				Commands->Set_Animation(obj,harvest_animation,true,0,0.0f,-1.0f,false);
			}
		}
		Commands->Start_Timer(obj,this,10.0f,1);
	}
	else if (action_id == 3)
	{
		ActionParamsStruct HarvesterDock;
		HarvesterDock.MoveArrivedDistance = 100;
		HarvesterDock.Set_Basic(this,100,4);
		HarvesterDock.Dock_Vehicle(Get_Vector3_Parameter("Dock_Location"),Commands->Get_Position(obj));
		HarvesterDock.Set_Movement(Get_Vector3_Parameter("Dock_Location"),1.0f,0.5f,false);
		Commands->Action_Dock(obj,HarvesterDock);
	}
	else if (action_id == 4)
	{
		if (!complete_reason) 
		{
			Commands->Set_Facing(obj,facing);
			const char *dock_animation = Get_Parameter("Dock_Animation");
			if (_stricmp(dock_animation,"0"))
			{
				Commands->Set_Animation(obj,dock_animation,false,0,0.0f,-1.0f,false);
			}
		}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Dock_Idle_Time"),3);
	}
}

void KAK_Harvester_Attached::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 100004)
	{
		facing = (float &)param;
	}
}

void KAK_Harvester_Attached::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && !End_Timer)
	{
		const char *harvest_animation = Get_Parameter("Harvest_Animation");
		if (_stricmp(harvest_animation,"0"))
		{
			Commands->Set_Animation(obj,harvest_animation,false,0,0.0f,-1.0f,false);
		}
		Vector3 Root_Position = Vector3(0,0,0);
		if (PathNumber == 1)
		{
			Root_Position = Get_Vector3_Parameter("Waypath1_Tib_Zone");
		}
		else if (PathNumber == 2)
		{
			Root_Position = Get_Vector3_Parameter("Waypath2_Tib_Zone");
		}
		else if (PathNumber == 3)
		{
			Root_Position = Get_Vector3_Parameter("Waypath3_Tib_Zone");
		}
		int X_Level = Commands->Get_Random_Int(1,3);
		if (X_Level == 1) Root_Position.X += Commands->Get_Random(0.0f,2.001f);
		else if (X_Level == 2) Root_Position.X -= Commands->Get_Random(0.0f,2.001f);
		int Y_Level = Commands->Get_Random_Int(1,3);
		if (Y_Level == 1) Root_Position.Y += Commands->Get_Random(0.0f,2.001f);
		else if (Y_Level == 2) Root_Position.Y -= Commands->Get_Random(0.0f,2.001f);
		ActionParamsStruct HarvesterTib;
		HarvesterTib.MoveArrivedDistance = 100;
		HarvesterTib.Set_Basic(this,100,2);
		HarvesterTib.Set_Movement(Root_Position,1.0f,0.1f,false);
		Commands->Action_Goto(obj,HarvesterTib);
	}
	else if (number == 2)
	{
		End_Timer = true;
		const char *harvest_animation = Get_Parameter("Harvest_Animation");
		if (_stricmp(harvest_animation,"0"))
		{
			Commands->Set_Animation(obj,harvest_animation,false,0,0.0f,-1.0f,false);
		}
		ActionParamsStruct HarvesterReturn;
		HarvesterReturn.MoveArrivedDistance = 100;
		HarvesterReturn.Set_Basic(this,100,3);
		if (PathNumber == 1)
		{
			HarvesterReturn.Set_Movement(0,1.0f,0.1f);
			HarvesterReturn.WaypathID = Get_Int_Parameter("Waypath1_Field2Dock");
			HarvesterReturn.WaypathSplined = true;
			HarvesterReturn.AttackActive = false;
			HarvesterReturn.AttackCheckBlocked = false;
		}
		else if (PathNumber == 2)
		{
			HarvesterReturn.Set_Movement(0,1.0f,0.1f);
			HarvesterReturn.WaypathID = Get_Int_Parameter("Waypath2_Field2Dock");
			HarvesterReturn.WaypathSplined = true;
			HarvesterReturn.AttackActive = false;
			HarvesterReturn.AttackCheckBlocked = false;
		}
		else if (PathNumber == 3)
		{
			HarvesterReturn.Set_Movement(0,1.0f,0.1f);
			HarvesterReturn.WaypathID = Get_Int_Parameter("Waypath3_Field2Dock");
			HarvesterReturn.WaypathSplined = true;
			HarvesterReturn.AttackActive = false;
			HarvesterReturn.AttackCheckBlocked = false;
		}
		Commands->Action_Goto(obj,HarvesterReturn);
	}
	else if (number == 3)
	{
		End_Timer = false;
		Commands->Give_Money(obj,Get_Float_Parameter("Dock_Credits"),true);
		if (Get_Int_Parameter("Number_Of_Paths") > 3) PathNumber = Commands->Get_Random_Int(1,4);
		else PathNumber = Commands->Get_Random_Int(1,(Get_Int_Parameter("Number_Of_Paths") + 1));
		Vector3 CrystalRelPos = Vector3(0,0,0);
		Vector3 CrystalPos;
		ActionParamsStruct HarvesterReturnToField;
		HarvesterReturnToField.MoveArrivedDistance = 100;
		HarvesterReturnToField.Set_Basic(this,100,1);
		if (PathNumber == 1)
		{
			HarvesterReturnToField.Set_Movement(0,1.0f,0.1f);
			HarvesterReturnToField.WaypathID = Get_Int_Parameter("Waypath1_Dock2Field");
			HarvesterReturnToField.WaypathSplined = true;
			HarvesterReturnToField.AttackActive = false;
			HarvesterReturnToField.AttackCheckBlocked = false;
			CrystalRelPos = Get_Vector3_Parameter("Waypath1_Tib_Zone");
		}
		else if (PathNumber == 2)
		{
			HarvesterReturnToField.Set_Movement(0,1.0f,0.1f);
			HarvesterReturnToField.WaypathID = Get_Int_Parameter("Waypath2_Dock2Field");
			HarvesterReturnToField.WaypathSplined = true;
			HarvesterReturnToField.AttackActive = false;
			HarvesterReturnToField.AttackCheckBlocked = false;
			CrystalRelPos = Get_Vector3_Parameter("Waypath2_Tib_Zone");
		}
		else if (PathNumber == 3)
		{
			HarvesterReturnToField.Set_Movement(0,1.0f,0.1f);
			HarvesterReturnToField.WaypathID = Get_Int_Parameter("Waypath3_Dock2Field");
			HarvesterReturnToField.WaypathSplined = true;
			HarvesterReturnToField.AttackActive = false;
			HarvesterReturnToField.AttackCheckBlocked = false;
			CrystalRelPos = Get_Vector3_Parameter("Waypath3_Tib_Zone");
		}
		if (_stricmp(Get_Parameter("Crystal_Preset"),"0"))
		{
			for (int x = 1;x <= 10;x++)
			{
				int XChange,YChange;
				CrystalPos = CrystalRelPos;
				XChange = Commands->Get_Random_Int(1,3);
				YChange = Commands->Get_Random_Int(1,3);
				if (XChange == 1) CrystalPos.X += Commands->Get_Random(0.0f,5.001f);
				else if (XChange == 2) CrystalPos.X -= Commands->Get_Random(0.0f,5.001f);
				if (YChange == 1) CrystalPos.Y += Commands->Get_Random(0.0f,5.001f);
				else if (YChange == 2) CrystalPos.Y -= Commands->Get_Random(0.0f,5.001f);
				if (x == 1)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal1 = Commands->Get_ID(Crystal);
				}
				else if (x == 2)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal2 = Commands->Get_ID(Crystal);
				}
				else if (x == 3)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal3 = Commands->Get_ID(Crystal);
				}
				else if (x == 4)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal4 = Commands->Get_ID(Crystal);
				}
				else if (x == 5)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal5 = Commands->Get_ID(Crystal);
				}
				else if (x == 6)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal6 = Commands->Get_ID(Crystal);
				}
				else if (x == 7)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal7 = Commands->Get_ID(Crystal);
				}
				else if (x == 8)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal8 = Commands->Get_ID(Crystal);
				}
				else if (x == 9)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal9 = Commands->Get_ID(Crystal);
				}
				else if (x == 10)
				{
					GameObject *Crystal = Commands->Create_Object(Get_Parameter("Crystal_Preset"),CrystalPos);
					Commands->Disable_All_Collisions(Crystal);
					Crystal10 = Commands->Get_ID(Crystal);
				}
			}
		}
		Commands->Action_Goto(obj,HarvesterReturnToField);
	}
	else if (number == 4)
	{
		if (_stricmp(Get_Parameter("Crystal_Preset"),"0"))
		{
			if (CrystalNumber == 1)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal1));
			}
			else if (CrystalNumber == 2)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal2));
			}
			else if (CrystalNumber == 3)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal3));
			}
			else if (CrystalNumber == 4)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal4));
			}
			else if (CrystalNumber == 5)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal5));
			}
			else if (CrystalNumber == 6)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal6));
			}
			else if (CrystalNumber == 7)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal7));
			}
			else if (CrystalNumber == 8)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal8));
			}
			else if (CrystalNumber == 9)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal9));
			}
			else if (CrystalNumber == 10)
			{
				Commands->Destroy_Object(Commands->Find_Object(Crystal10));
			}
			CrystalNumber++;
			if (CrystalNumber <= 10)
			{
				Commands->Start_Timer(obj,this,(Get_Float_Parameter("Tiberium_Idle_Time") / 10),4);
			}
			else
			{
				CrystalNumber = 1;
			}
		}
	}
	else if (number == 5)
	{
		CrystalNumber = 1;
	}
}

void KAK_Harvester_Attached::Killed(GameObject *obj,GameObject *killer)
{
	if (Commands->Get_ID(Commands->Find_Object(Crystal1))) Commands->Destroy_Object(Commands->Find_Object(Crystal1));
	if (Commands->Get_ID(Commands->Find_Object(Crystal2))) Commands->Destroy_Object(Commands->Find_Object(Crystal2));
	if (Commands->Get_ID(Commands->Find_Object(Crystal3))) Commands->Destroy_Object(Commands->Find_Object(Crystal3));
	if (Commands->Get_ID(Commands->Find_Object(Crystal4))) Commands->Destroy_Object(Commands->Find_Object(Crystal4));
	if (Commands->Get_ID(Commands->Find_Object(Crystal5))) Commands->Destroy_Object(Commands->Find_Object(Crystal5));
	if (Commands->Get_ID(Commands->Find_Object(Crystal6))) Commands->Destroy_Object(Commands->Find_Object(Crystal6));
	if (Commands->Get_ID(Commands->Find_Object(Crystal7))) Commands->Destroy_Object(Commands->Find_Object(Crystal7));
	if (Commands->Get_ID(Commands->Find_Object(Crystal8))) Commands->Destroy_Object(Commands->Find_Object(Crystal8));
	if (Commands->Get_ID(Commands->Find_Object(Crystal9))) Commands->Destroy_Object(Commands->Find_Object(Crystal9));
	if (Commands->Get_ID(Commands->Find_Object(Crystal10))) Commands->Destroy_Object(Commands->Find_Object(Crystal10));
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("LogicID")),100006,0,0);
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("LogicID")),100001,0,Get_Float_Parameter("Harvester_Create_Idle_Time"));
}

void KAK_Harvester_Attached::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&PathNumber,4,1);
	Auto_Save_Variable(&Crystal1,4,2);
	Auto_Save_Variable(&Crystal2,4,3);
	Auto_Save_Variable(&Crystal3,4,4);
	Auto_Save_Variable(&Crystal4,4,5);
	Auto_Save_Variable(&Crystal5,4,6);
	Auto_Save_Variable(&Crystal6,4,7);
	Auto_Save_Variable(&Crystal7,4,8);
	Auto_Save_Variable(&Crystal8,4,9);
	Auto_Save_Variable(&Crystal9,4,10);
	Auto_Save_Variable(&Crystal10,4,11);
	Auto_Save_Variable(&End_Timer,1,12);
	Auto_Save_Variable(&CrystalNumber,4,13);
	Auto_Save_Variable(&facing,4,14);
}

void KAK_Harvester_Kill::Killed(GameObject *obj,GameObject *killer)
{
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("LogicID")),100002,0,0);
}

void KAK_Harvester_Spawn_Kill::Killed(GameObject *obj,GameObject *killer)
{
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("LogicID")),100005,0,0);
}

void KAK_Prevent_Kill::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
	Commands->Set_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj));
}

void KAK_Prevent_Kill::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object = Commands->Create_Object(Commands->Get_Preset_Name(obj),Commands->Get_Position(obj));
	Commands->Set_Facing(object,Commands->Get_Facing(obj));
}

void KAK_Convert_Visceroid::Created(GameObject *obj)
{
	Commands->Set_Model(obj,"c_visceroid");
	Commands->Give_PowerUp(obj,"POW_ChemSprayer_Player",true);
	Commands->Set_Shield_Type(obj,"SkinMutant");
	Commands->Start_Timer(obj,this,0.1f,1);
}

void KAK_Convert_Visceroid::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Give_PowerUp(obj,"CnC_POW_Ammo_ClipMax",true);
		Commands->Select_Weapon(obj,"Weapon_ChemSprayer_Player");
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}

void KAK_Convert_Visceroid::Killed(GameObject *obj,GameObject *killer)
{
	Commands->Destroy_Object(obj);
}

void KAK_Freeze_Object::Created(GameObject *obj)
{
	elapsedtime = 0.0f;
	Vector3 OriginalPos = Commands->Get_Position(obj);
	xpos = OriginalPos.X;
	ypos = OriginalPos.Y;
	zpos = OriginalPos.Z;
	Commands->Start_Timer(obj,this,0.1f,1);
}

void KAK_Freeze_Object::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		elapsedtime += 0.1f;
		if (elapsedtime <= Get_Float_Parameter("Time"))
		{
			Vector3 OriginalPos;
			OriginalPos.X = xpos;
			OriginalPos.Y = ypos;
			OriginalPos.Z = zpos;
			Commands->Set_Position(obj,OriginalPos);
			Commands->Start_Timer(obj,this,0.1f,1);
		}
		else Destroy_Script();
	}
}

void KAK_Freeze_Object::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 3256) Destroy_Script();
}

void KAK_Regen_Custom::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Interval"),1);
}

void KAK_Regen_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (Commands->Get_Max_Health(obj) != Commands->Get_Health(obj))
		{
			Commands->Set_Health(obj,(Commands->Get_Health(obj) + Get_Float_Parameter("Health")));
		}
		else if (Commands->Get_Max_Shield_Strength(obj) != Commands->Get_Shield_Strength(obj))
		{
			Commands->Set_Shield_Strength(obj,(Commands->Get_Shield_Strength(obj) + Get_Float_Parameter("Health")));
		}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Interval"),1);
	}
}

void KAK_Regen_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 3251) Destroy_Script();
}

void KAK_Freeze_Object::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&xpos,4,1);
	Auto_Save_Variable(&ypos,4,2);
	Auto_Save_Variable(&zpos,4,3);
	Auto_Save_Variable(&elapsedtime,4,4);
}

ScriptRegistrant<KAK_Warp_On_Pickup> KAK_Warp_On_Pickup_Registrant("KAK_Warp_On_Pickup","Position:vector3");
ScriptRegistrant<KAK_Precipitation_Create> KAK_Precipitation_Create_Registrant("KAK_Precipitation_Create","Type=Rain:string,Density=0.000:float,Delay=0.000:float,Reset_Precipitation=0:int,OnCreate=0:int,OnDestroy=0:int");
ScriptRegistrant<KAK_Precipitation_Zone> KAK_Precipitation_Zone_Registrant("KAK_Precipitation_Zone","Type=Rain:string,Density=0.000:float,Delay=0.000:float,Reset_Precipitation=0:int,OnEnter=0:int,OnExit=0:int");
ScriptRegistrant<KAK_Wind_Create> KAK_Wind_Create_Registrant("KAK_Wind_Create","Wind_Heading=0.000:float,Wind_Speed=0.000:float,Wind_Variability=0.000:float,Delay=0.000:float,OnCreate=0:int,OnDestroy=0:int");
ScriptRegistrant<KAK_Wind_Zone> KAK_Wind_Zone_Registrant("KAK_Wind_Zone","Wind_Heading=0.000:float,Wind_Speed=0.000:float,Wind_Variability=0.000:float,Delay=0.000:float,OnEnter=0:int,OnExit=0:int");
ScriptRegistrant<KAK_Clouds_Create> KAK_Clouds_Create_Registrant("KAK_Clouds_Create","Cloud_Cover=0.000:float,Cloud_Gloominess=0.000:float,Delay=0.000:float,OnCreate=0:int,OnDestroy=0:int");
ScriptRegistrant<KAK_Clouds_Zone> KAK_Clouds_Zone_Registrant("KAK_Clouds_Zone","Cloud_Cover=0.000:float,Cloud_Gloominess=0.000:float,Delay=0.000:float,OnEnter=0:int,OnExit=0:int");
ScriptRegistrant<KAK_Lightning_Create> KAK_Lightning_Create_Registrant("KAK_Lightning_Create","Lightning_Intensity=0.000:float,Start_Distance=0.000:float,End_Distance=1.000:float,Lightning_Heading=0.000:float,Lightning_Distribution=1.000:float,Delay=0.000:float,OnCreate=0:int,OnDestroy=0:int");
ScriptRegistrant<KAK_Lightning_Zone> KAK_Lightning_Zone_Registrant("KAK_Lightning_Zone","Lightning_Intensity=0.000:float,Start_Distance=0.000:float,End_Distance=1.000:float,Lightning_Heading=0.000:float,Lightning_Distribution=1.000:float,Delay=0.000:float,OnEnter=0:int,OnExit=0:int");
ScriptRegistrant<KAK_Flowing_Weather> KAK_Flowing_Weather_Registrant("KAK_Flowing_Weather","Precipitation_Type=Rain:string,Minimum_Precipitation_Start=0.500:float,Max_Precipitation_Density=5.000:float,Minimum_Lightning_Start=0.850:float,Maximum_Lightning_Intensity=1.000:float,Wind_Variablity=10.000:float,Speed=0.010:float");
ScriptRegistrant<KAK_Give_Powerup_On_Pickup> KAK_Give_Powerup_On_Pickup_Registrant("KAK_Give_Powerup_On_Pickup","Powerup_Preset:string");
ScriptRegistrant<KAK_Select_Weapon_Zone> KAK_Select_Weapon_Zone_Registrant("KAK_Select_Weapon_Zone","Weapon_Preset:string,OnEnter=0:int,OnExit=0:int");
ScriptRegistrant<KAK_Select_Weapon_On_Pickup> KAK_Select_Weapon_On_Pickup_Registrant("KAK_Select_Weapon_On_Pickup","Weapon_Preset:string");
ScriptRegistrant<KAK_Harvester_Logic> KAK_Harvester_Logic_Registrant("KAK_Harvester_Logic","Harvester_Preset:string,Explosion_Preset:string");
ScriptRegistrant<KAK_Harvester_Attached> KAK_Harvester_Attached_Registrant("KAK_Harvester_Attached","LogicID=0:int,Number_Of_Paths=0:int,Waypath1_Dock2Field=0:int,Waypath1_Field2Dock=0:int,Waypath1_Tib_Zone:vector3,Waypath2_Dock2Field=0:int,Waypath2_Field2Dock=0:int,Waypath2_Tib_Zone:vector3,Waypath3_Dock2Field=0:int,Waypath3_Field2Dock=0:int,Waypath3_Tib_Zone:vector3,Harvest_Animation:string,Dock_Animation:string,Tiberium_Idle_Time=45.00:float,Dock_Location:vector3,Dock_Idle_Time=10.00:float,Dock_Credits=400.00:float,Harvester_Create_Idle_Time=5.00:float,Crystal_Preset:string");
ScriptRegistrant<KAK_Harvester_Kill> KAK_Harvester_Kill_Registrant("KAK_Harvester_Kill","LogicID=0:int");
ScriptRegistrant<KAK_Harvester_Spawn_Kill> KAK_Harvester_Spawn_Kill_Registrant("KAK_Harvester_Spawn_Kill","LogicID=0:int");
ScriptRegistrant<KAK_Prevent_Kill> KAK_Prevent_Kill_Registrant("KAK_Prevent_Kill","");
ScriptRegistrant<KAK_Convert_Visceroid> KAK_Convert_Visceroid_Registrant("KAK_Convert_Visceroid","");
ScriptRegistrant<KAK_Freeze_Object> KAK_Freeze_Object_Registrant("KAK_Freeze_Object","Time=5.0:float");
ScriptRegistrant<KAK_Regen_Custom> KAK_Regen_Custom_Registrant("KAK_Regen_Custom","Interval=0.5:float,Health=2.0:float");
