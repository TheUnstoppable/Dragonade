/*	Renegade Scripts.dll
	Copyright 2013 Tiberian Technologies

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
#include "engine_script.h"
#include "VehicleGameObj.h"
#include "engine_obj.h"

void Bypass_Drop_Off(Vector3 Location, float Facing, GameObject *dropObj)
{	
	Location.Z += 1;

	GameObject *Drop = Commands->Create_Object("invisible_object", Location);
	Commands->Set_Facing(Drop,Facing);
	Commands->Attach_Script(Drop,"Bypass_Drop_Off_Control",StringClass::getFormattedString("%i",dropObj->Get_ID()));
}

class DB_CinematicVehicleFactoryBypass: public JFW_Object_Created_Hook_Base
{
public:
	void ObjectCreateHook(GameObject *obj)
	{
		if(obj->As_VehicleGameObj())
		{
			if(strcmp(Commands->Get_Preset_Name(obj),Get_Parameter("PresetName")) == 0)
			{
				{
					Attach_Script_Once_V(obj,"DB_Check_Airdrop","%i",Owner()->Get_ID());
				}

			}
		}
	}
};

class DB_Check_Airdrop: public ScriptImpClass {
	void Created(GameObject *obj) 	{
		Commands->Start_Timer(obj,this,0.0f,12);
	}

	void Timer_Expired(GameObject *obj,int Number)
	{
		if(Number == 12)
		{
			if(obj->As_VehicleGameObj()->Get_Lock_Owner() && Commands->Get_Player_Type(obj->As_VehicleGameObj()->Get_Lock_Owner()) == 1 && !obj->Find_Observer("DAAirDroppedVehicleObserverClass"))
			{
				GameObject *DavesArrow = Commands->Find_Object(Get_Int_Parameter("ArrowID"));
				if(DavesArrow)
				{
					Send_Message_Player(obj->As_VehicleGameObj()->Get_Lock_Owner(),150,150,255,"Your Vehicle will be Airdropped nearby.");
					Bypass_Drop_Off(Commands->Get_Position(DavesArrow),Commands->Get_Facing(DavesArrow),obj);
				}
			}
		}
	}
};

ScriptRegistrant<DB_CinematicVehicleFactoryBypass> DB_CinematicVehicleFactoryBypass_Registrant("DB_CinematicVehicleFactoryBypass","PresetName:string");
ScriptRegistrant<DB_Check_Airdrop> DB_Check_Airdrop_Registrant("DB_Check_Airdrop","ArrowID:int");

class Bypass_Drop_Off_Control : public ScriptImpClass
{
	int transbone_id;
 	int trans_id;
	int vehharness_id;
	int vehicle_id;
 	int fakeharness_id;
	bool invisible;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

void Bypass_Drop_Off_Control::Created(GameObject *obj)
{


	//SignalFlare_Gold_Phys3
	invisible = false;
	int ID = Get_Int_Parameter("ID");
	GameObject* vehicle = Commands->Find_Object(ID);
	if(vehicle)
	{
		int Team = Get_Object_Type(vehicle);
		Vector3 pos = Commands->Get_Position(obj);
	
		GameObject *transbone = Commands->Create_Object("Generic_Cinematic", pos);
		Commands->Set_Model(transbone, "XG_TransprtBone");
		Commands->Set_Animation(transbone, "XG_TransprtBone.XG_HD_TTraj", false, "", 2.0, -1.0, false);

		GameObject *trans = Commands->Create_Object_At_Bone(transbone, Team == 1 ? "GDI_Transport_Helicopter" : "Nod_Transport_Helicopter", "BN_Trajectory");

		Commands->Set_Animation(trans, "v_Nod_Trnspt.v_Nod_Trnspt", true, "", 2.0, -1.0, false);
		//Set_Vehicle_Is_Visible(trans, false);
		Commands->Set_Shield_Type(trans,"Blamo");
		Commands->Attach_To_Object_Bone(trans, transbone, "BN_Trajectory");
 
		GameObject *vehharness = Commands->Create_Object("Generic_Cinematic", pos);
		Commands->Set_Model(vehharness, "XG_HD_HTraj");
		Commands->Set_Animation(vehharness, "XG_HD_HTraj.XG_HD_HTraj", false, "", 2.0, -1.0, false);
 
		GameObject *fakeharness = Commands->Create_Object("Generic_Cinematic", pos);
		Commands->Set_Model(fakeharness, "XG_HD_Harness");
		Commands->Set_Animation(fakeharness, "XG_HD_Harness.XG_HD_Harness", false, "", 2.0, -1.0, false);

		Set_Object_Type(vehicle, Team);
		if(vehicle->As_VehicleGameObj())
		{
			if(vehicle->As_VehicleGameObj()->Get_Is_Scripts_Visible())
			{
				Commands->Set_Is_Visible(vehicle, false);
				invisible=false;
			}
			else
			{
				invisible=true;
			}
		}
		Commands->Attach_To_Object_Bone(vehicle, vehharness, "BN_Trajectory");
 
		transbone_id = Commands->Get_ID(transbone);
		trans_id = Commands->Get_ID(trans);
		vehharness_id = Commands->Get_ID(vehharness);
		vehicle_id = Commands->Get_ID(vehicle);
		fakeharness_id = Commands->Get_ID(fakeharness);
 
		Commands->Start_Timer(obj, this, 14.2f, 1);
		Commands->Start_Timer(obj, this, 30, 2);
	}
}

void Bypass_Drop_Off_Control::Timer_Expired(GameObject *obj, int number)
{
	if(number == 1)
	{
		GameObject *Vehicle = Commands->Find_Object(vehicle_id);
		if(Vehicle)
		{
			Commands->Attach_To_Object_Bone(Commands->Find_Object(vehicle_id), 0, 0);
			if(Vehicle->As_VehicleGameObj())
			{
				if(!invisible)
				{
					Commands->Set_Is_Visible(Vehicle,true);
				}
			}
		}
	}
	else if(number == 2)
	{
		GameObject *TransbonePreset = Commands->Find_Object(transbone_id);

		if(TransbonePreset) Commands->Destroy_Object(TransbonePreset);

		GameObject *HarnessPreset = Commands->Find_Object(vehharness_id);

		if(HarnessPreset) Commands->Destroy_Object(HarnessPreset);

		GameObject *FakeHarnessPreset = Commands->Find_Object(fakeharness_id);

		if(FakeHarnessPreset) Commands->Destroy_Object(FakeHarnessPreset);

		GameObject *TransportPreset = Commands->Find_Object(trans_id);

		if(TransportPreset) Commands->Destroy_Object(TransportPreset);

		Destroy_Script();
	}

}

ScriptRegistrant<Bypass_Drop_Off_Control> DropOffControlRegistrant("Bypass_Drop_Off_Control", "ID:int");