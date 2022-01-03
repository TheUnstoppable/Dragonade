/*	  Renegade Scripts.dll
		Copyright 2017 Tiberian Technologies
 
		This file is part of the Renegade scripts.dll
		The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
		the terms of the GNU General Public License as published by the Free
		Software Foundation; either version 2, or (at your option) any later
		version. See the file COPYING for more details.
		In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
		Only the source code to the module(s) containing the licenced code has to be released.
*/
 
#include "General.h"
#include "Xpert.h"
#include "engine.h"
#include "PhysClass.h"
#include "PhysicalGameObj.h"


Xpert_Teleport_Zone_Random::Xpert_Teleport_Zone_Random() : NextLocation(0)
{
}
 
void Xpert_Teleport_Zone_Random::Entered(GameObject* Zone, GameObject* Enterer)
{
		Vector3 Location;
		int ObjectId;
	   
		switch (NextLocation)
		{
				case 0:
						Location = Get_Vector3_Parameter("Location1");
						ObjectId = Get_Int_Parameter("Object_ID1");
						break;
				case 1:
						Location = Get_Vector3_Parameter("Location2");
						ObjectId = Get_Int_Parameter("Object_ID2");
						break;
				case 2:
						Location = Get_Vector3_Parameter("Location3");
						ObjectId = Get_Int_Parameter("Object_ID3");
						break;
				case 3:
						Location = Get_Vector3_Parameter("Location4");
						ObjectId = Get_Int_Parameter("Object_ID4");
						break;
				case 4:
						Location = Get_Vector3_Parameter("Location5");
						ObjectId = Get_Int_Parameter("Object_ID5");
						break;
				default:
						return; // Should never happen
		}
		if (ObjectId)
		{
				Location = Commands->Get_Position(Commands->Find_Object(ObjectId));
		}
	   
			Commands->Set_Position(Enterer, Location);
		if (Enterer->As_PhysicalGameObj() && Enterer->As_PhysicalGameObj()->Peek_Physical_Object())
		{
				Enterer->As_PhysicalGameObj()->Peek_Physical_Object()->Set_Collision_Group(SOLDIER_GHOST_COLLISION_GROUP);
		}
	   
		NextLocation = (NextLocation + 1) % 5;
}
 
void Xpert_Teleport_Zone_Random::Register_Auto_Save_Variables()
{
		Auto_Save_Variable(&NextLocation, sizeof(NextLocation), 1);
}

ScriptRegistrant<Xpert_Teleport_Zone_Random> Xpert_Teleport_Zone_Random_Registrant("Xpert_Teleport_Zone_Random", "Location1:vector3,Object_ID1=0:int,Location2:vector3,Object_ID2=0:int,Location3:vector3,Object_ID3=0:int,Location4:vector3,Object_ID4=0:int,Location5:vector3,Object_ID5=0:int");
