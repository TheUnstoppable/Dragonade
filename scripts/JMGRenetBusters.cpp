/*	Renegade Scripts.dll
	Renegade Renet Busters Scripts
	Copyright 2007 Jerad Gray, Jonathan Wilson, WhiteDragon(MDB)

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
#include "SoldierGameObj.h"
#include "WeaponBagClass.h"
#include "WeaponClass.h"
#include "WeaponMgr.h"
#include "MoveablePhysClass.h"
#include "engine_tt.h"
#include "PhysicsSceneClass.h"
#include "physcoltest.h"
#include "VehicleGameObj.h"
#include "jmgUtility.h"
#include "JMGRenetBuster.h" 
#include "RigidBodyClass.h"
#include "ArmedGameObjDef.h"
#include "GameObjManager.h"
// Did a major overhaul on all these scripts, even though the mod they belonged to is dead :(
void JMG_Create_Ship_On_Poke::Created(GameObject *obj)
{
	ShipID = 0;
	YourID = 0;
	armor = "null";
	BotID = 0;
	ViewingBoxID = 0;
	cango = 0;
}
void JMG_Create_Ship_On_Poke::Poked(GameObject *obj,GameObject *poker)
{
	if (ShipID == 0)
	{
		Vector3 pos = Get_Vector3_Parameter("CreateSpot");
		Commands->Create_Explosion(Get_Parameter("SpawnSpotExplosion"),pos,0);
		Commands->Create_Explosion(Get_Parameter("SpawnSpotExplosion"),pos,0);
		Commands->Create_Explosion(Get_Parameter("SpawnSpotExplosion"),pos,0);
		Commands->Create_Explosion(Get_Parameter("SpawnSpotExplosion"),pos,0);
		YourID = Commands->Get_ID(poker);
		Your3DModel = Get_Model(poker);
		armor = Get_Shield_Type(poker);
		Commands->Set_Shield_Type(poker,"blamo");
		Commands->Set_Model(poker,"NULL");
		GameObject *bot = Commands->Create_Object(Commands->Get_Preset_Name(poker),Commands->Get_Position(poker));
		BotID = Commands->Get_ID(bot);
		GameObject *object = Commands->Create_Object(Get_Parameter("ShipPreset"),pos);
		ShipID = Commands->Get_ID(object);
		Commands->Set_Position(poker,Commands->Get_Position(object));
		Commands->Start_Timer(obj,this,0.1f,6546585);
		Commands->Start_Timer(obj,this,0.15f,6546587);
	}
}
void JMG_Create_Ship_On_Poke::Timer_Expired(GameObject *obj,int number)
{
	if (number == 6546585)
	{
		GameObject *Ship = Commands->Find_Object(ShipID);
		if ((Commands->Get_Health(Ship) == 0) && (ShipID > 0))
		{
			GameObject *object = Commands->Find_Object(YourID);
			Vector3 pos = Commands->Get_Position(object);
			GameObject *object2 = Commands->Create_Object(Get_Parameter("InvisibleObject"),pos);
			ViewingBoxID = Commands->Get_ID(object2);
			cango = 1;
			Commands->Start_Timer(obj,this,0.00f,6546588);	
			Commands->Start_Timer(obj,this,5.0f,6546586);
			Commands->Start_Timer(obj,this,0.5f,6546589);
		}
		Commands->Start_Timer(obj,this,0.1f,6546585);
	}
	if (number == 6546586)
	{
		GameObject *object = Commands->Find_Object(YourID);
		GameObject *object2 = Commands->Find_Object(ViewingBoxID);
		GameObject *object3 = Commands->Find_Object(BotID);
		Vector3 pos = Commands->Get_Position(object3);
		Commands->Destroy_Object(object3);
		Commands->Apply_Damage(object2,9999999.00f,"BlamoKiller",0);
		Commands->Set_Position(object,pos);
		Commands->Set_Shield_Type(object,armor);
		Commands->Set_Model(object,Your3DModel);
		ShipID = 0;
	}
	if (number == 6546587)
	{
		GameObject *object = Commands->Find_Object(YourID);
		Soldier_Transition_Vehicle(object);
	}
	if ((number == 6546588) && (cango == 1))
	{
		GameObject *object = Commands->Find_Object(YourID);
		Soldier_Transition_Vehicle(object);
		Commands->Start_Timer(obj,this,0.10f,6546588);
	}
	if ((number == 6546589))
	{
		cango = 0;
	}
}
void JMG_Advanced_Bounce_Zone::Entered(GameObject *obj,GameObject *enter)
{
	Vector3 pos;
	pos = Commands->Get_Position(enter);
	if ((Get_Float_Parameter("ZAmount") > 0) || (Get_Float_Parameter("ZAmount") < 0))
	{
		pos.Z += Get_Float_Parameter("ZAmount");
	}
	if ((Get_Float_Parameter("YAmount") > 0) || (Get_Float_Parameter("YAmount") < 0))
	{
		pos.Y += Get_Float_Parameter("YAmount");
	}
	if ((Get_Float_Parameter("XAmount") > 0) || (Get_Float_Parameter("XAmount") < 0))
	{
		pos.X += Get_Float_Parameter("XAmount");
	}
	Commands->Set_Position(enter,pos);
}
void JMG_Ship_Random_Hyperspace::Created(GameObject *obj)
{
	Has_Recived_Message = 0;
	OriginalModel = Get_Model(obj);
	Commands->Set_Model(obj,OriginalModel);
}
void JMG_Ship_Random_Hyperspace::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	//Start Hyper Spacing
	if ((message == 48473851) && (Has_Recived_Message == 0))
	{
		Has_Recived_Message = 1;
		Commands->Set_Model(obj,Get_Parameter("HyperspacingModel"));
		Commands->Start_Timer(obj,this,2.0f,586733);
		Commands->Send_Custom_Event(obj,obj,4537822,1,0);
	}
	//go now
	if (message == 48473852)
	{
		Vector3 pos = Commands->Get_Position(obj);
		pos.X = Commands->Get_Random(-100,100);
		pos.Y = Commands->Get_Random(-100,100);
		pos.Z += 0.0f;
		Commands->Set_Position(obj,pos);
		Commands->Set_Model(obj,OriginalModel);
		Vector3 sound = Commands->Get_Position(obj);
		Commands->Create_Sound(Get_Parameter("HyperSpaceSound"),sound,obj);
		Commands->Start_Timer(obj,this,30.0f,586734);
		Commands->Send_Custom_Event(obj,obj,4537821,1,0);
	}
}
void JMG_Ship_Random_Hyperspace::Timer_Expired(GameObject *obj,int number)
{
	if (number == 586733)
	{
		Commands->Send_Custom_Event(obj,obj,48473852,1,0);
	}
	if (number == 586734)
	{
		Has_Recived_Message = 0;
	}
}
void JMG_Random_Music::Created(GameObject *obj)
{
	songnumber = Commands->Get_Random_Int(0,2);
	if (songnumber == 0)
	{
		Commands->Set_Background_Music(Get_Parameter("Song1"));
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Song1Time"),48557224);
	}
	if (songnumber == 1)
	{
		Commands->Set_Background_Music(Get_Parameter("Song2"));
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Song2Time"),48557224);
	}
	if (songnumber == 2)
	{
		Commands->Set_Background_Music(Get_Parameter("Song3"));
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Song3Time"),48557224);
	}
}
void JMG_Random_Music::Timer_Expired(GameObject *obj,int number)
{
	if (number == 48557224)
	{
		if (songnumber == 0)
		{
			Commands->Set_Background_Music(Get_Parameter("Song1"));
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Song1Time"),48557224);
			int repeat = Commands->Get_Random_Int(0,1);
			if (repeat == 1)
			{
				songnumber = Commands->Get_Random_Int(0,2);
			}
			if (repeat == 0)
			{
				songnumber = Commands->Get_Random_Int(1,2);
			}
		}
		if (songnumber == 1)
		{
			Commands->Set_Background_Music(Get_Parameter("Song2"));
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Song2Time"),48557224);
			int repeat = Commands->Get_Random_Int(0,1);
			if (repeat == 1)
			{
				songnumber = Commands->Get_Random_Int(0,2);
			}
			if (repeat == 0)
			{
				int tempsongnumber = Commands->Get_Random_Int(0,2);
				if (tempsongnumber == 1)
				{
					tempsongnumber = Commands->Get_Random_Int(0,2);
					if (tempsongnumber == 1)
					{
						tempsongnumber = 2;
					}
				}
				if ((tempsongnumber == 0) || (tempsongnumber == 2))
				{
					songnumber = tempsongnumber;
				}
			}
		}
		if (songnumber == 2)
		{
			Commands->Set_Background_Music(Get_Parameter("Song3"));
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Song3Time"),48557224);
			int repeat = Commands->Get_Random_Int(0,1);
			if (repeat == 1)
			{
				songnumber = Commands->Get_Random_Int(0,2);
			}
			if (repeat == 0)
			{
				songnumber = Commands->Get_Random_Int(0,1);
			}
		}
	}
}
void JMG_Ship_Weapon::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,28557222);
	Commands->Start_Timer(obj,this,0.1f,28557223);
	Commands->Start_Timer(obj,this,10.0f,28557224);
	Ammo = 4;
	ammocheck = 20;
	Currentammo = 1;
	cangainammo = 1;
}
void JMG_Ship_Weapon::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 4537824 && cangainammo == 1)
	{
		if (Ammo > 0)
		{
			Ammo -= 1;
			Commands->Start_Timer(obj,this,2.00f,28557221);
		}
	}
	if (message == 4537823 && cangainammo == 1)
	{
		if (Ammo < 4 && Get_Current_Bullets(obj) < 4)
		{
			Ammo += 1;
			Set_Current_Clip_Bullets(obj,Get_Current_Bullets(obj) + 1);
			Commands->Start_Timer(obj,this,2.00f,28557221);
		}
	}
	if (message == 4537822 && cangainammo == 1)
	{
		Currentammo = Get_Current_Bullets(obj);
		cangainammo = 0;
		Set_Current_Clip_Bullets(obj,0);
		Commands->Enable_Engine(obj,false);
	}
	if (message == 4537821 && cangainammo == 0)
	{
		cangainammo = 1;
		Set_Current_Clip_Bullets(obj,Currentammo);
		Commands->Enable_Engine(obj,true);
	}
}
void JMG_Ship_Weapon::Timer_Expired(GameObject *obj,int number)
{
	if (28557221 == number && Get_Current_Bullets(obj) < 4)
	{
		Ammo += 1;
		Set_Current_Clip_Bullets(obj,Get_Current_Bullets(obj) + 1);
	}
	if (28557222 == number)
	{
		Commands->Start_Timer(obj,this,0.1f,28557222);
	}
	if (28557223 == number)
	{
		if (Get_Current_Bullets(obj) < newtotal)
		{
			ammocheck = 0;
			Commands->Send_Custom_Event(obj,obj,4537824,1,0);
		}
		newtotal = Get_Current_Bullets(obj);

		if (ammocheck < 20)
			ammocheck++;
		if (ammocheck > 20 && Get_Current_Bullets(obj) < 4)
		{
			Set_Current_Clip_Bullets(obj,4);
			Ammo = 4;
		}
		Commands->Start_Timer(obj,this,0.1f,28557223);
	}
	if (28557224 == number)
	{
		Set_Current_Clip_Bullets(obj,Ammo);
		Commands->Start_Timer(obj,this,10.0,28557224);
	}
}
void JMG_Return_Custom_When_Shot::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if ((CheckPlayerType(damager,0)) || (CheckPlayerType(damager,1)))
		Commands->Send_Custom_Event(obj,damager,4537823,1,0);
}
void JMG_Cronie_AI::Created(GameObject *obj)
{
	target = 0;
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Enable_Hibernation(obj,false);
	Commands->Start_Timer(obj,this,0.1f,4586442);
	Commands->Start_Timer(obj,this,5.0f,4586443);
}
void JMG_Cronie_AI::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (target == 0)
	{
		ActionParamsStruct params;
		params.MoveArrivedDistance = 0.0f;
		params.Set_Basic(this,100,131524);
		params.Set_Movement(seen,2.0f,0.0f,false);
		Commands->Action_Goto(obj,params);
		target = Commands->Get_ID(seen);
	}
}
void JMG_Cronie_AI::Timer_Expired(GameObject *obj,int number)
{
	if (number == 4586442)
	{
		if (target > 0)
		{
			GameObject *object = Commands->Find_Object(target);
			if (Commands->Get_Health(object) == 0)
				target = 0;
		}
		Commands->Start_Timer(obj,this,0.1f,4586442);
	}
	if (number == 4586443)
	{
		if (target == 0)
		{
			Vector3 pos = Commands->Get_Position(obj);
			pos.X += Commands->Get_Random_Int(-5,5);
			pos.Y += Commands->Get_Random_Int(-5,5);
			ActionParamsStruct params;
			params.Set_Basic(this,100,911);
			params.Set_Movement(pos,1.0f,0.0f,false);
			Commands->Action_Goto(obj,params);
		}
		Commands->Start_Timer(obj,this,2.5f,4586443);
	}
}
void JMG_Kill_Self_And_Sender_On_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Message"))
	{
		Commands->Apply_Damage(obj,9999999.00f,"BlamoKiller",0);
		Commands->Apply_Damage(sender,9999999.00f,"BlamoKiller",0);
	}
}
void JMG_Ship_Shield_Control_Script::Created(GameObject *obj)
{
	ison = 0;
	ObjID = 0;
}
void JMG_Ship_Shield_Control_Script::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if ((message == 1239464) && (ison == 0))
	{
		ison = 1;
		Vector3 pos;
		GameObject *object = Commands->Create_Object(Get_Parameter("Ship_Shield"),pos);
		Commands->Attach_To_Object_Bone(object,obj,"origin");
		Commands->Set_Player_Type(object,Get_Player_Type(obj));
		ObjID = Commands->Get_ID(object);
		char params[512];
		sprintf(params,"%d",Commands->Get_ID(obj));
		Commands->Attach_Script(object,"JMG_Ship_Shield_Script",params);
		Commands->Send_Custom_Event(obj,obj,4537822,1,0);
		//Commands->Send_Custom_Event(obj,obj,45378221,1,0);
		char Text[512];
		sprintf(Text,"MESSAGE on");
		Console_Input(Text);
		//Commands->Start_Timer(obj,this,0.1f,543424420);
	}
	if ((message == 1239464) && (ison == 1))
	{
		ison = 0;
		GameObject *object = Commands->Find_Object(ObjID);
		Commands->Apply_Damage(object,9999999.00f,"BlamoKiller",0);
		ObjID = 0;
		Commands->Send_Custom_Event(obj,obj,4537821,1,0);
		Commands->Send_Custom_Event(obj,obj,45378211,1,0);
		char Text[512];
		sprintf(Text,"MESSAGE off");
		Console_Input(Text);
		//Commands->Start_Timer(obj,this,0.1f,543424421);
	}
}
void JMG_Ship_Shield_Control_Script::Killed(GameObject *obj, GameObject *damager)
{
	if (ison == 1)
	{
		ison = 0;
		GameObject *object = Commands->Find_Object(ObjID);
		Commands->Apply_Damage(object,9999999.00f,"BlamoKiller",0);
		ObjID = 0;
		char Text[512];
		sprintf(Text,"MESSAGE DEAD, %s","Nod");
		Console_Input(Text);
	}
}
void JMG_Kill_Self_And_Sender_On_Custom_Ship::Created(GameObject *obj)
{
	canbekilled = 1;
}
void JMG_Kill_Self_And_Sender_On_Custom_Ship::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if ((message == Get_Int_Parameter("Message")) && (canbekilled == 1))
	{
		Commands->Apply_Damage(obj,9999999.00f,"BlamoKiller",0);
		Commands->Apply_Damage(sender,9999999.00f,"BlamoKiller",0);
	}
	if ((message == 45378221) && (canbekilled == 1))
	{
		canbekilled = 0;
	}
	if ((message == 45378211) && (canbekilled == 0))
	{
		canbekilled = 1;
	}
}
void JMG_Ship_Shield_Script::Created(GameObject *obj)
{
	LastSenderID = 0;
	ReccentlyDamaged = 0;
	GameObject *object = Commands->Find_Object(Get_Int_Parameter("ID"));
	if (!_stricmp(Get_Model(object),"cometbsb")){ShieldModel = "shipshieldb";}
	else if (!_stricmp(Get_Model(object),"cometbsg")){ShieldModel = "shipshieldg";}
	else if (!_stricmp(Get_Model(object),"cometbsr")){ShieldModel = "shipshieldr";}
	else if (!_stricmp(Get_Model(object),"cometbsy")){ShieldModel = "shipshieldy";}
	Commands->Set_Model(obj,ShieldModel);
	Commands->Start_Timer(obj,this,0.01f,54342442);
	char Text[512];
	sprintf(Text,"MESSAGE Shield Should Exist");
	Console_Input(Text);
}
void JMG_Ship_Shield_Script::Timer_Expired(GameObject *obj,int number)
{
	if (number == 54342442)
	{
		Send_Custom_All_Objects_Area(913,Commands->Get_Position(obj),6.53f,obj,2);
		Commands->Start_Timer(obj,this,0.01f,54342442);
	}
	if (number == 54342443)
	{
		Commands->Set_Model(obj,ShieldModel);
		if (ReccentlyDamaged == 1)
		{
			ReccentlyDamaged = 0;
		}
	}
}
void JMG_Ship_Shield_Script::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 90111315 && LastSenderID != Commands->Get_ID(sender))
	{
		LastSenderID = Commands->Get_ID(sender);
		GameObject *object = Commands->Find_Object(Get_Int_Parameter("ID"));
		Commands->Send_Custom_Event(obj,object,90111316,param,0);
		Commands->Set_Model(obj,"shipshieldw");
		Commands->Apply_Damage(sender,9999999.00f,"BlamoKiller",object);
		Commands->Start_Timer(obj,this,0.25f,54342443);
	}
}
void JMG_Ship_Shield_Script::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (ReccentlyDamaged == 0)
	{
		ReccentlyDamaged = 1;
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),90111316,15,0);
		Commands->Set_Model(obj,"shipshieldw");
		Commands->Start_Timer(obj,this,0.25f,54342443);
	}
}
void JMG_Renetbusters_Object::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 913)
	{
		float distance = Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(sender));
		if (distance <= 6.53f - Get_Float_Parameter("size"))
			Commands->Send_Custom_Event(obj,sender,90111315,Get_Int_Parameter("Power_Drain"),0);
	}
}
void JMG_New_Ship_Weapon_Control_System::Damaged(GameObject *obj,GameObject *damager,float damage)
{		
	if (Commands->Get_ID(Get_Vehicle(damager)) == Commands->Get_ID(obj) && damage == 0.0f && Get_Total_Bullets(obj,"Weapon_Ship_Plasma_Shot_Normal") < 4)
		Set_Bullets(obj,"Weapon_Ship_Plasma_Shot_Normal",Get_Bullets(obj,"Weapon_Ship_Plasma_Shot_Normal")+1);
}
void JMG_Comet_Control_Script::Created(GameObject *obj)
{
	char params[1000];
	sprintf(params,"%f,911,0.10,453,2",Get_Float_Parameter("Comet_Size"));
	Commands->Attach_Script(obj,"JFW_Send_Custom_Distance_Objects_Timer",params);
	Set_Current_Clip_Bullets(obj,Commands->Get_Random_Int(1,3));
	Commands->Start_Timer(obj,this,1.0f,24523);
}
void JMG_Comet_Control_Script::Timer_Expired(GameObject *obj,int number)
{
	if (number == 24523)
	{
		if (Commands->Get_Position(obj).Z > 1.0)
			Commands->Apply_Damage(obj,9999.99f,"none",0);
		Commands->Start_Timer(obj,this,1.0f,24523);
	}
}
void JMG_Comet_Control_Script::Killed(GameObject *obj, GameObject *damager)
{
	if (Get_Int_Parameter("Spawn_Comets") == 1)
	{
		Commands->Set_Facing(obj,Commands->Get_Random(0.00,360.00));
		Vector3 pos = Commands->Get_Position(obj);
		pos.Z = 0;
		GameObject *object = Commands->Create_Object("Daves Arrow",pos);
		Commands->Set_Model(object,Get_Parameter("Comet_Holder"));
		Commands->Attach_Script(object,"JMG_Attach_And_Remove",Get_Parameter("Smaller_Comet"));
	}
}
void JMG_Attach_And_Remove::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,581);
}
void JMG_Attach_And_Remove::Timer_Expired(GameObject *obj,int number)
{
	if (number == 581)
	{
		Commands->Create_Object(Get_Parameter("Smaller_Comet"),Commands->Get_Bone_Position(obj,"SC1"));
		Commands->Create_Object(Get_Parameter("Smaller_Comet"),Commands->Get_Bone_Position(obj,"SC2"));
		Commands->Destroy_Object(obj);
	}
}
void JMG_User_Auto_Enter_Ship::Created(GameObject *obj)
{
	char params[20];
	sprintf(params,"%d",Commands->Get_ID(obj));
	GameObject *object = Commands->Create_Object(Get_Parameter("Player_Ship"),Commands->Get_Position(obj));
	Commands->Attach_Script(object,"JMG_Remove_Object_When_Creator_Destroyed",params);
	Commands->Set_Model(object,Get_Model(object));
	Soldier_Transition_Vehicle(obj);
	Vector3 pos = Commands->Get_Position(object);
	pos.Z = 0.00;
	Commands->Set_Position(object,pos);
	Commands->Set_Position(obj,Commands->Get_Position(object));
	Commands->Start_Timer(obj,this,0.1f,453435);
}
void JMG_User_Auto_Enter_Ship::Timer_Expired(GameObject *obj,int number)
{
	if (number == 453435)
	{
		Soldier_Transition_Vehicle(obj);
		if (Commands->Get_ID(Get_Vehicle_Return(obj)) == Commands->Get_ID(obj))
		{
			Commands->Start_Timer(obj,this,0.1f,453435);
		}
	}
}
void JMG_Apply_Damage_On_Timer::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,453212);
}
void JMG_Apply_Damage_On_Timer::Timer_Expired(GameObject *obj,int number)
{
	if (number == 453212)
	{
		Commands->Apply_Damage(obj,9999.99f,"none",0);
	}
}

// Upgraded Scripts
#define CMTBPowerUpCustomMessage 90335490
RenCometBustersGameObjects RenCometBustersList = RenCometBustersGameObjects();
GameObject *JMG_CMTB_Random_Ship_Powerup(Vector3 Position,int LifeTime = 0)
{
	Position.Z = 0.25f;
	GameObject *Powerup = Commands->Create_Object("CMTB_Powerup_Object",Position);
	int PowerupType = 0;
	float Random = Commands->Get_Random(0.0f,1.0f);
	if (Commands->Get_Random(0.0f,1.0f) > 0.94f)
		if (Random < 0.33)
			if (Commands->Get_Random(0.0f,1.0f) < 0.75f)
				PowerupType = 10;
			else
				PowerupType = 14;
		else if (Random < 0.66)
		{
			if (Commands->Get_Random(0.0f,1.0f) > 0.75f)
				PowerupType = 11;
			else
				PowerupType = 13;
		}
		else
			if (Commands->Get_Random(0.0f,1.0f) < 0.5)
				PowerupType = 12;
			else
				PowerupType = 16;
	else if (Commands->Get_Random(0.0f,1.0f) < 0.97)
		if (Commands->Get_Random(0.0f,1.0f) < 0.4)
			if (Random < 0.75)
				PowerupType = 0;
			else if (Random < 0.9)
				PowerupType = 1;
			else
				PowerupType = 2;
		else
			if (Random < 0.5)
				PowerupType = 6;
			else if (Random < 0.8)
				PowerupType = 7;
			else if (Random < 0.95)
				PowerupType = 8;
			else
				PowerupType = 9;
	else if (Commands->Get_Random(0.0f,1.0f) < 95.0f)
		if (Random < 0.40)
			PowerupType = 15;
		else if (Random < 0.70)
			PowerupType = 3;
		else if (Random < 0.90)
			PowerupType = 4;
		else
			PowerupType = 5;
	else
		PowerupType = 5;
	char params[200];
	sprintf(params,"%d,%d",PowerupType,LifeTime);
	Commands->Attach_Script(Powerup,"JMG_CMTB_Powerup_Script",params);
	return Powerup;
}
void JMG_CMTB_Main_Game_Control::Created(GameObject *obj)
{
	MoonHolderID = 0;
	PlayerSOSBeaconDealy = 0;
	JMG_CMTB_Main_Game_Control::PlayerSOSBeaconID = 0;
	JMG_CMTB_Main_Game_Control::DroneSwarmCount = 0;
	CurrentMusic = MusicNone;
	BabySit = 20;
	Commands->Start_Timer(obj,this,5.0,5438);
	UFOSpawnTime = Commands->Get_Random_Int(600,1800);
	AddPerkTypes();
	RenCometBustersScoreControl.Cleanup();
	RenCometBustersScoreControl.LoadData(1);
	PlayerPerkSystemControl.LoadData();
	JMG_CMTB_Main_Game_Control::CMTBLevel = 0;
	NextLevelDelay = -1;
	LastPlayerCount = 0;
	LastCometCount = -1;
	Commands->Set_Model(obj,"RenRoidsBones");
	char params[32];
	for (int x = 0;x < MaxGamePlayerCount;x++)
	{
		MiniGamePlayerControlSystem[x] = RenCometBustersPlayerNode();
		char Spot[32];
		sprintf(Spot,"SpecSpot0%d",x);
		MiniGamePlayerControlSystem[x].SpectatorObject = Commands->Create_Object_At_Bone(obj,"Spectator_Vehicle",Spot);
		Commands->Attach_Script(MiniGamePlayerControlSystem[x].SpectatorObject,"JMG_CMTB_Vehicle_Face_Turret","");
		MiniGamePlayerControlSystem[x].specCreateSpot = Commands->Get_Position(MiniGamePlayerControlSystem[x].SpectatorObject);
		MiniGamePlayerControlSystem[x].SpectatorObject->As_PhysicalGameObj()->Peek_Physical_Object()->Set_Collision_Group(TERRAIN_ONLY_COLLISION_GROUP);
		Set_Max_Shield_Strength(MiniGamePlayerControlSystem[x].SpectatorObject,8.0f);
		sprintf(params,"%d,%d",Commands->Get_ID(obj),x);
		Commands->Attach_Script(MiniGamePlayerControlSystem[x].SpectatorObject,"JMG_CMTB_Spectator_Object",params);
		sprintf(Spot,"CreateS%d",x);
		MiniGamePlayerControlSystem[x].ReplacementShip = Commands->Create_Object_At_Bone(obj,"Player_Ships",Spot);
		char ShipName[32];
		sprintf(ShipName,"P%d_SHIP",x);
		Commands->Set_Model(MiniGamePlayerControlSystem[x].ReplacementShip,ShipName);
		sprintf(ShipName,"P%d_SHIP.P%d_SHIP",x,x);
		Commands->Set_Animation(MiniGamePlayerControlSystem[x].ReplacementShip,ShipName,false,0,0,0,false);

	}
	for (int x = 0;x < MaxGamePlayerCount;x++)
	{
		char Spot[32];
		sprintf(Spot,"SpawnS0%d",x);
		SpawnLocations[x] = Commands->Get_Bone_Position(obj,Spot);
	}
	Commands->Start_Timer(obj,this,0.1f,5434);
	Commands->Start_Timer(obj,this,1.0f,5435);
	Commands->Start_Timer(obj,this,10.0f,5436);
	Commands->Start_Timer(obj,this,300.0f,5437);
}
void JMG_CMTB_Main_Game_Control::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 57447 && NextLevelDelay == -1)
	{
		bool StartGame = true;
		int ReadyPlayerCount = 0;
		int TotalPlayerCount = 0;
		for (int x = 0;x < MaxGamePlayerCount;x++)
		{
			if (!Get_GameObj(MiniGamePlayerControlSystem[x].PlayerID) || !Commands->Find_Object(MiniGamePlayerControlSystem[x].GamePlayerID))
				MiniGamePlayerControlSystem[x].PlayerID = 0;
			if (MiniGamePlayerControlSystem[x].PlayerID)
				TotalPlayerCount++;
			if (!MiniGamePlayerControlSystem[x].isReady && MiniGamePlayerControlSystem[x].PlayerID)
				StartGame = false;
			else if (MiniGamePlayerControlSystem[x].PlayerID)
				ReadyPlayerCount++;
		}
		if (!StartGame)
		{
			char playermsg[256];
			sprintf(playermsg,"%s is ready to start the game; however, %d others are not!",Get_Player_Name(sender),TotalPlayerCount-ReadyPlayerCount);
			for (int x = 1;x < 128;x++)
			{
				GameObject *Player = Get_GameObj(x);
				if (!Player)
					continue;	
				JmgUtility::DisplayChatMessage(Player,200,125,125,playermsg);
			}
			return;
		}
		JMG_CMTB_Main_Game_Control::GameInProgress = true;
		MaxPlayerCount = 0;
		PlayerSOSBeaconDealy = 0;
		if (JMG_CMTB_Main_Game_Control::PlayerSOSBeaconID)
		{
			Commands->Destroy_Object(Commands->Find_Object(JMG_CMTB_Main_Game_Control::PlayerSOSBeaconID));
			JMG_CMTB_Main_Game_Control::PlayerSOSBeaconID = 0;
		}
		for (int x = 0;x < MaxGamePlayerCount;x++)
		{
			PlayerRequestingALife[x] = false;
			MiniGamePlayerControlSystem[x].NotInGameTime = 0;
			MiniGamePlayerControlSystem[x].SuperBombs = 0;
			MiniGamePlayerControlSystem[x].HasSOSBeacon = false;
			MiniGamePlayerControlSystem[x].DronePowerup = false;
			MiniGamePlayerControlSystem[x].DroneSwarmPowerup = false;
			MiniGamePlayerControlSystem[x].CargoWarnedCount = 0;
			MiniGamePlayerControlSystem[x].CargoTotalWarnedCount = 0;
			MiniGamePlayerControlSystem[x].CargoWarnDecreaseDelay = 0;
			MiniGamePlayerControlSystem[x].DoublePointsTime = 0;
			MiniGamePlayerControlSystem[x].isReady = false;
			MiniGamePlayerControlSystem[x].HyperspaceDriveCharge = 0;
			for (int y = 0;y < 7;y++)
				MiniGamePlayerControlSystem[x].CargoShipIDs[y] = 0;
			if (!MiniGamePlayerControlSystem[x].GamePlayerID)
				continue;
			GameObject *Player = Commands->Find_Object(MiniGamePlayerControlSystem[x].GamePlayerID);
			if (!Player)
				continue;
			Set_Money(Get_Player_ID(Player),0);
			RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->RoundsPlayed++;
			MaxPlayerCount++;
		}
		GameObject *moon = Commands->Find_Object(JMG_CMTB_Main_Game_Control::MoonID);
		if (moon)
			Commands->Apply_Damage(moon,9999.9f,"None",0);
		RenCometBustersList.ClearAllObjects();
		JMG_CMTB_Main_Game_Control::CMTBLevel = 0;
		NextLevelDelay = 0;
		UFOSpawnTime = Commands->Get_Random_Int(600,1800);
	}
	if (message == 57448)
	{
		if (!Commands->Find_Object(MiniGamePlayerControlSystem[param].GamePlayerID))
		{
			switch (CurrentMusic)
			{
			case MusicGameOver:Set_Background_Music_Player(sender,"Thunder.mp3");break;
			case MusicNormal:Set_Background_Music_Player(sender,"Android.mp3");break;
			case MusicBoss:Set_Background_Music_Player(sender,"Escape.mp3");break;
			}
			MiniGamePlayerControlSystem[param].GamePlayerID = Commands->Get_ID(sender);
			MiniGamePlayerControlSystem[param].PlayerID = Get_Player_ID(sender);
			MiniGamePlayerControlSystem[param].isReady = false;
			sprintf(MiniGamePlayerControlSystem[param].OriginalSkin,"%s",Get_Skin(sender));
			sprintf(MiniGamePlayerControlSystem[param].OriginalArmor,"%s",Get_Shield_Type(sender));
			Set_Skin(sender,"Blamo");
			Commands->Set_Shield_Type(sender,"Blamo");
			Vector3 SoldierPosition = Commands->Get_Position(sender);
			GameObject *FakeSoldier = Commands->Find_Object(MiniGamePlayerControlSystem[param].FakeSoldierID);
			if (FakeSoldier)
				Commands->Destroy_Object(FakeSoldier);
			FakeSoldier = Commands->Create_Object("CnC_Nod_MinigunnerN",SoldierPosition);
			Commands->Innate_Disable(FakeSoldier);
			Commands->Set_Model(FakeSoldier,Get_Model(sender));
			Commands->Set_Model(sender,"invisibleChar");
			Commands->Set_Facing(FakeSoldier,Commands->Get_Facing(sender));
			Commands->Set_Health(FakeSoldier,Commands->Get_Health(sender));
			Commands->Set_Shield_Strength(FakeSoldier,Commands->Get_Shield_Strength(sender));
			Commands->Set_Innate_Is_Stationary(FakeSoldier,true);
			char params[32];
			sprintf(params,"%d,%d",Commands->Get_ID(sender),param);
			Commands->Attach_Script(FakeSoldier,"JMG_CMTB_Fake_Player_Attach_Script",params);
			MiniGamePlayerControlSystem[param].OriginalLocation = SoldierPosition;
			MiniGamePlayerControlSystem[param].FakeSoldierID = Commands->Get_ID(FakeSoldier);
		}
	}
}
void JMG_CMTB_Main_Game_Control::Timer_Expired(GameObject *obj,int number)
{
	if (number == 5435)
	{
		if (!PlayerSOSBeaconDealy && JMG_CMTB_Main_Game_Control::PlayerSOSBeaconID)
			PlayerSOSBeaconDealy = 140;
		if (PlayerSOSBeaconDealy)
		{
			PlayerSOSBeaconDealy--;
			if (!PlayerSOSBeaconDealy)
			{
				Commands->Destroy_Object(Commands->Find_Object(JMG_CMTB_Main_Game_Control::PlayerSOSBeaconID));
				JMG_CMTB_Main_Game_Control::PlayerSOSBeaconID = 0;
			}
		}
		JmgUtility::GenericDateTime currentTime = JmgUtility::GenericDateTime();
		for (int x = 0;x < MaxGamePlayerCount;x++)
		{
			if (!MiniGamePlayerControlSystem[x].GamePlayerID)
				continue;
			RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->PlayTime++;
			RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->LastPlayTime = currentTime;
			if (MiniGamePlayerControlSystem[x].ShipObject)
				switch(MiniGamePlayerControlSystem[x].ShipObject->Type)
				{
				case PlayerCloak:
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->TimeCloaked++;break;
				case PlayerShield:
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->TimeShielded++;break;
				case PlayerShipDisrupter:
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->TimeDisrupted++;break;
				}
		}
		Commands->Start_Timer(obj,this,1.0,5435);
	}
	if (number == 5434)
	{
		BabySit = 0;
		if (NextLevelDelay != -1)
		{
			if (NextLevelDelay)
			{
				NextLevelDelay--;
				if (!NextLevelDelay)
				{
					for (int x = 0;x < MaxGamePlayerCount;x++)
					{
						if (MiniGamePlayerControlSystem[x].NotInGameTime || !MiniGamePlayerControlSystem[x].PlayerShip)
							continue;
						switch (MiniGamePlayerControlSystem[x].SelectedPerk)
						{
						case 1:
							if (JMG_CMTB_Main_Game_Control::CMTBLevel == 1 || !(JMG_CMTB_Main_Game_Control::CMTBLevel%25))
								Commands->Send_Custom_Event(obj,MiniGamePlayerControlSystem[x].PlayerShip,CMTBPowerUpCustomMessage+5,-1337,0);
							break;
						case 2:
							if (JMG_CMTB_Main_Game_Control::CMTBLevel == 1)
								Commands->Send_Custom_Event(obj,MiniGamePlayerControlSystem[x].PlayerShip,CMTBPowerUpCustomMessage+14,-1337,0);
							break;
						case 3:
							if (Commands->Get_Random(0.0f,1.0f) < 0.165)
								Commands->Send_Custom_Event(obj,MiniGamePlayerControlSystem[x].PlayerShip,CMTBPowerUpCustomMessage+13,-1337,0);
							break;
						case 4:
							if (Commands->Get_Random(0.0f,1.0f) < 0.05)
								Commands->Send_Custom_Event(obj,MiniGamePlayerControlSystem[x].PlayerShip,CMTBPowerUpCustomMessage+12,-1337,0);
							break;
						case 5:
							if (Commands->Get_Random(0.0f,1.0f) < 0.05)
								Commands->Send_Custom_Event(obj,MiniGamePlayerControlSystem[x].PlayerShip,CMTBPowerUpCustomMessage+16,-1337,0);
							break;
						}
					}
					if (!(JMG_CMTB_Main_Game_Control::CMTBLevel % 6))
					{
						unsigned int Count = (int)(JMG_CMTB_Main_Game_Control::CMTBLevel * 0.5);
						for (unsigned int x = 0;x < Count;x++)
						{//Drones
							GameObject *Comet = Commands->Create_Object("CMTB_Drone",Random_Map_Position());
							Commands->Attach_Script(Comet,"JMG_CMTB_Motion_Mine","1");
							MoveablePhysClass *mphys = obj->As_PhysicalGameObj() ? Comet->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
							while (mphys && !mphys->Can_Teleport(Matrix3D(Commands->Get_Position(Comet))))
								Commands->Set_Position(Comet,Random_Map_Position());
						}
					}
					else
					{
						unsigned int Count = JMG_CMTB_Main_Game_Control::CMTBLevel;
						if (!(JMG_CMTB_Main_Game_Control::CMTBLevel % 25))
							Count = JMG_CMTB_Main_Game_Control::CMTBLevel/4;
						else if (!(JMG_CMTB_Main_Game_Control::CMTBLevel % 10))
							Count = JMG_CMTB_Main_Game_Control::CMTBLevel/3;
						for (unsigned int x = 0;x < Count;x++)
						{//Comet2
							GameObject *Comet = Commands->Create_Object(((JMG_CMTB_Main_Game_Control::CMTBLevel % 10) ? "Comet2" : "CMTB_UFO"),Random_Map_Position());
							Commands->Set_Facing(Comet,Commands->Get_Random(-180.0f,180.0f));
							if (JMG_CMTB_Main_Game_Control::CMTBLevel % 10)
							{
								char params[512];
								sprintf(params,"2,%.3f %.3f %.3f",Commands->Get_Position(Comet).X+(Commands->Get_Random(0.0,1.0f) < 0.5 ? 1 : -1)*Commands->Get_Random(15,25),(Commands->Get_Random(0.0,1.0f) < 0.5 ? 1 : -1)*Commands->Get_Position(Comet).Y+Commands->Get_Random(15,25),Commands->Get_Position(Comet).Z);
								Commands->Attach_Script(Comet,"JMG_CMTB_Comet_Script",params);
							}
							else
								Commands->Attach_Script(Comet,"JMG_CMTB_UFO_Random_Fire_Control","1");
							MoveablePhysClass *mphys = Comet->As_PhysicalGameObj() ? Comet->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
							while (mphys && !mphys->Can_Teleport(Matrix3D(Commands->Get_Position(Comet))))
								Commands->Set_Position(Comet,Random_Map_Position());
						}
					}
					if (!(JMG_CMTB_Main_Game_Control::CMTBLevel % 25))
					{
						GameObject *moonHolder = Commands->Create_Object("Daves Arrow",Vector3(0.0f,-50.0f,0.0f));
						Commands->Set_Model(moonHolder,"TheMoon!ANIM");
						Commands->Set_Animation(moonHolder,"TheMoon!ANIM.TheMoon!ANIM",true,0,0.0f,-1,false);
						MoonHolderID = Commands->Get_ID(moonHolder);
						GameObject *Moon = Commands->Create_Object("TheMoon!",Vector3(0.0f,-50.0f,0.0f));
						Commands->Attach_To_Object_Bone(Moon,moonHolder,"moon");
						Set_Max_Health(Moon,200.0f*MaxPlayerCount);
						Commands->Set_Health(Moon,200.0f*MaxPlayerCount);
					}
					if (!(JMG_CMTB_Main_Game_Control::CMTBLevel % 15))
					{
						GameObject *Ship = Commands->Create_Object("CMTB_Cargo_Ship",Vector3(0.0f,0.0f,1.481f));
						Commands->Attach_Script(Ship,"JMG_CMTB_Cargo_Ship_Script","-1");
						Set_Max_Health(Ship,100.0f*MaxPlayerCount);
						Commands->Set_Health(Ship,100.0f*MaxPlayerCount);
					}
				}
			}
			else if (RenCometBustersList.CheckIfLevelComplete())
			{
				JMG_CMTB_Main_Game_Control::CMTBLevel++;
				if (JMG_CMTB_Main_Game_Control::CMTBLevel == 11)
				{
					for (int x = 0;x < MaxGamePlayerCount;x++)
					{
						if (!MiniGamePlayerControlSystem[x].GamePlayerID)
							continue;
						GameObject *Player = Commands->Find_Object(MiniGamePlayerControlSystem[x].GamePlayerID);
						if (!Player)
							continue;
						PerkSystemIncreasePlayerPerkUnlockAmount(Player,63,1);
					}
				}
				LastCometCount = -1;
				if (JMG_CMTB_Main_Game_Control::CMTBLevel == 1)
					for (int x = 0;x < MaxGamePlayerCount;x++)
					{
						MiniGamePlayerControlSystem[x].NumberOfLives = 0;
						MiniGamePlayerControlSystem[x].RespawnTime = 30;
						if (!MiniGamePlayerControlSystem[x].GamePlayerID)
							continue;
						if (!Commands->Find_Object(MiniGamePlayerControlSystem[x].GamePlayerID))
							continue;
						MiniGamePlayerControlSystem[x].NumberOfLives = 5;
						MiniGamePlayerControlSystem[x].RespawnTime = 0;
					}
				char LevelString[32];
				sprintf(LevelString,"***********Level %d***********",JMG_CMTB_Main_Game_Control::CMTBLevel);
				for (int x = 1;x < 128;x++)
				{
					GameObject *Player = Get_GameObj(x);
					if (!Player)
						continue;
					JmgUtility::DisplayChatMessage(Player,6,145,148,LevelString);
					sprintf_s(LevelString,"Level %d",JMG_CMTB_Main_Game_Control::CMTBLevel);
					Set_HUD_Help_Text_Player_Text(Player,7233,LevelString,Vector3(0.19599999f,1.0f,0.19599999f));
					if ((JMG_CMTB_Main_Game_Control::CMTBLevel-1) % 10 && (JMG_CMTB_Main_Game_Control::CMTBLevel-1) % 25)
						Create_2D_Sound_Player(Player,"SFX.Bonus_Mission_Update");
					else
						Create_2D_Sound_Player(Player,"SFX.Primary_Mission_Update");
				}
				if (JMG_CMTB_Main_Game_Control::CMTBLevel % 10 && JMG_CMTB_Main_Game_Control::CMTBLevel % 25)
				{
					if (CurrentMusic != MusicNormal)
					{
						CurrentMusic = MusicNormal;
						FadeMusic("Android.mp3");
					}
				}
				else if (CurrentMusic != MusicBoss)
				{
					CurrentMusic = MusicBoss;
					FadeMusic("Escape.mp3");
				}
				NextLevelDelay = 40;
			}
			int ContinueGame = 0;
			for (int x = 0;x < MaxGamePlayerCount;x++)
			{
				if (!MiniGamePlayerControlSystem[x].GamePlayerID)
					continue;
				GameObject *Player = Commands->Find_Object(MiniGamePlayerControlSystem[x].GamePlayerID);
				if (!Player)
					continue;
				if (!MiniGamePlayerControlSystem[x].PlayerShip && !MiniGamePlayerControlSystem[x].NumberOfLives)
				{
					if (MiniGamePlayerControlSystem[x].SpectatorObject && Commands->Get_Shield_Strength(MiniGamePlayerControlSystem[x].SpectatorObject) != 0.0f)
						Commands->Set_Shield_Strength(MiniGamePlayerControlSystem[x].SpectatorObject,0.0f);
					continue;
				}
				ContinueGame++;
			}
			LastPlayerCount = ContinueGame;
			if (!ContinueGame)
			{
				JMG_CMTB_Main_Game_Control::GameInProgress = false;
				bool teamHasPerks = false;
				for (int x = 0;x < MaxGamePlayerCount;x++)
					if (MiniGamePlayerControlSystem[x].SelectedPerk)
					{
						teamHasPerks = true;
						break;
					}
				for (int x = 0;x < MaxGamePlayerCount;x++)
				{
					MiniGamePlayerControlSystem[x].RespawnTime = 30;
					if (!MiniGamePlayerControlSystem[x].GamePlayerID)
					{
						MiniGamePlayerControlSystem[x].Score = 0;
						MiniGamePlayerControlSystem[x].DroneScore = 0;
						MiniGamePlayerControlSystem[x].CargoScore = 0;
						MiniGamePlayerControlSystem[x].SecondSpace = false;
						continue;
					}
					GameObject *Player = Commands->Find_Object(MiniGamePlayerControlSystem[x].GamePlayerID);
					if (!Player)
					{
						MiniGamePlayerControlSystem[x].Score = 0;
						MiniGamePlayerControlSystem[x].DroneScore = 0;
						MiniGamePlayerControlSystem[x].CargoScore = 0;
						MiniGamePlayerControlSystem[x].SecondSpace = false;
						continue;
					}
					RenCometBustersScoreSystem::CMTBHighScoresNode *TempNode = RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID);
					TempNode->TotalScore += (unsigned long)MiniGamePlayerControlSystem[x].Score;
					TempNode->DronePoints += (unsigned long)MiniGamePlayerControlSystem[x].DroneScore;
					TempNode->CargoShipScore += (unsigned long)MiniGamePlayerControlSystem[x].CargoScore;
					if (MaxPlayerCount > 1)
					{
						if (teamHasPerks)
							TempNode->LevelsPlayedWithPerksAsATeam++;
						else
							TempNode->LevelsPlayedWithOutPerksAsATeam++;
					}
					else
						if (MiniGamePlayerControlSystem[x].SelectedPerk)
							TempNode->LevelsPlayedWithPerks++;
						else
							TempNode->LevelsPlayedWithOutPerks++;
					if (MaxPlayerCount > 1)
					{
						if (!MiniGamePlayerControlSystem[x].SelectedPerk)
						{
							if (TempNode->HighestCleanTeamScore < (unsigned long)MiniGamePlayerControlSystem[x].Score)
								TempNode->HighestCleanTeamScore = (unsigned long)MiniGamePlayerControlSystem[x].Score;
						}
						else if (TempNode->HighestTeamScore < (unsigned long)MiniGamePlayerControlSystem[x].Score)
								TempNode->HighestTeamScore = (unsigned long)MiniGamePlayerControlSystem[x].Score;
					}
					else
					{
						if (!MiniGamePlayerControlSystem[x].SelectedPerk)
						{
							if (TempNode->HighestCleanScore < (unsigned long)MiniGamePlayerControlSystem[x].Score)
								TempNode->HighestCleanScore = (unsigned long)MiniGamePlayerControlSystem[x].Score;
						}
						else if (TempNode->HighestScore < (unsigned long)MiniGamePlayerControlSystem[x].Score)
								TempNode->HighestScore = (unsigned long)MiniGamePlayerControlSystem[x].Score;
					}
					char ScoreMessage[245];
					if (MaxPlayerCount > 1)
						if (teamHasPerks)
						{
							if (TempNode->TeamHighestLevel < JMG_CMTB_Main_Game_Control::CMTBLevel)
							{
								Create_2D_Sound_Player(Player,"SFX.Primary_Mission_Complete");
								sprintf(ScoreMessage,"You scored %.2f points and through teamwork made it to a new high level as a team: Level %d while using perks!",MiniGamePlayerControlSystem[x].Score,JMG_CMTB_Main_Game_Control::CMTBLevel);
								TempNode->TeamHighestLevel = JMG_CMTB_Main_Game_Control::CMTBLevel;
								PerkSystemIncreasePlayerPerkUnlockAmount(Player,61,JMG_CMTB_Main_Game_Control::CMTBLevel);
							}
							else
								sprintf(ScoreMessage,"You scored %.2f points and through teamwork made it to Level %d while using perks!",MiniGamePlayerControlSystem[x].Score,JMG_CMTB_Main_Game_Control::CMTBLevel);
						}
						else
						{
							if (TempNode->HighestCleanTeamLevel < JMG_CMTB_Main_Game_Control::CMTBLevel)
							{
								Create_2D_Sound_Player(Player,"SFX.Primary_Mission_Complete");
								sprintf(ScoreMessage,"You scored %.2f points and through teamwork made it to a new high level as a team: Level %d!",MiniGamePlayerControlSystem[x].Score,JMG_CMTB_Main_Game_Control::CMTBLevel);
								TempNode->HighestCleanTeamLevel = JMG_CMTB_Main_Game_Control::CMTBLevel;
								PerkSystemIncreasePlayerPerkUnlockAmount(Player,61,JMG_CMTB_Main_Game_Control::CMTBLevel);
							}
							else
								sprintf(ScoreMessage,"You scored %.2f points and through teamwork made it to Level %d!",MiniGamePlayerControlSystem[x].Score,JMG_CMTB_Main_Game_Control::CMTBLevel);
						}
					else
						if (teamHasPerks)
						{
							if (TempNode->HighestLevel < JMG_CMTB_Main_Game_Control::CMTBLevel)
							{
								Create_2D_Sound_Player(Player,"SFX.Primary_Mission_Complete");
								sprintf(ScoreMessage,"You scored %.2f points and made it to a new personal highest level: Level %d while using perks!",MiniGamePlayerControlSystem[x].Score,JMG_CMTB_Main_Game_Control::CMTBLevel);
								TempNode->HighestLevel = JMG_CMTB_Main_Game_Control::CMTBLevel;
								PerkSystemIncreasePlayerPerkUnlockAmount(Player,61,JMG_CMTB_Main_Game_Control::CMTBLevel);
							}
							else
								sprintf(ScoreMessage,"You scored %.2f points and made it to level %d while using perks!",MiniGamePlayerControlSystem[x].Score,MiniGamePlayerControlSystem[x].HighestLevel);
						}
						else
						{
							if (TempNode->HighestCleanLevel < JMG_CMTB_Main_Game_Control::CMTBLevel)
							{
								Create_2D_Sound_Player(Player,"SFX.Primary_Mission_Complete");
								sprintf(ScoreMessage,"You scored %.2f points and made it to a new personal highest level: Level %d!",MiniGamePlayerControlSystem[x].Score,JMG_CMTB_Main_Game_Control::CMTBLevel);
								TempNode->HighestCleanLevel = JMG_CMTB_Main_Game_Control::CMTBLevel;
								PerkSystemIncreasePlayerPerkUnlockAmount(Player,61,JMG_CMTB_Main_Game_Control::CMTBLevel);
							}
							else
								sprintf(ScoreMessage,"You scored %.2f points and made it to level %d",MiniGamePlayerControlSystem[x].Score,JMG_CMTB_Main_Game_Control::CMTBLevel);
						}
					JmgUtility::DisplayChatMessage(Player,6,145,148,ScoreMessage);
					MiniGamePlayerControlSystem[x].Score = 0;
					MiniGamePlayerControlSystem[x].DroneScore = 0;
					MiniGamePlayerControlSystem[x].CargoScore = 0;
					MiniGamePlayerControlSystem[x].SecondSpace = false;
				}
				RenCometBustersScoreControl.SaveData();
				PlayerPerkSystemControl.SaveData();
				//Commands->Set_HUD_Help_Text(13194,Vector3(0.19599999,1.0,0.19599999));//0.88200003
				for (int x = 1;x < 128;x++)
				{
					GameObject *Player = Get_GameObj(x);
					if (Player)
						JmgUtility::DisplayChatMessage(Player,6,145,148,"Game Over!");
					Set_HUD_Help_Text_Player_Text(Player,7233,"Game Over!",Vector3(0.19599999f,1.0f,0.19599999f));
				}
				for (int x = 0;x < MaxGamePlayerCount;x++)
					MiniGamePlayerControlSystem[x].endGamePadding = 3;
				CurrentMusic = MusicGameOver;
				FadeMusic("Thunder.mp3");
				NextLevelDelay = -1;
			}
		}
		else
		{
			int ContinueGame = 0;
			for (int x = 0;x < MaxGamePlayerCount;x++)
			{
				if (!MiniGamePlayerControlSystem[x].GamePlayerID)
					continue;
				GameObject *Player = Commands->Find_Object(MiniGamePlayerControlSystem[x].GamePlayerID);
				if (!Player)
					continue;
				ContinueGame++;
			}
			ContinueGame = JmgUtility::MathClampInt(ContinueGame,1,8);
			if (LastPlayerCount != ContinueGame)
				for (int x = 0;x < MaxGamePlayerCount;x++)
					Commands->Set_Health(MiniGamePlayerControlSystem[x].SpectatorObject,(float)JmgUtility::MathClampInt(ContinueGame,1,8));
			LastPlayerCount = ContinueGame;
		}
		RenCometBustersList.UpdateCollisions();
		for (int x = 0;x < MaxGamePlayerCount;x++)
			Create_Player_Ship(obj,x);
		if (UFOSpawnTime)
		{
			UFOSpawnTime--;
			if (!UFOSpawnTime)
			{
				int CometCount = RenCometBustersList.CountType(Asteroid);
				if (CometCount != LastCometCount || NextLevelDelay == -1)
				{
					GameObject *UFO = Commands->Create_Object("CMTB_UFO",Random_Map_Position());
					Commands->Attach_Script(UFO,"JMG_CMTB_UFO_Random_Fire_Control","0");
					MoveablePhysClass *mphys = UFO->As_PhysicalGameObj() ? UFO->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
					while (mphys && !mphys->Can_Teleport(Matrix3D(Commands->Get_Position(UFO))))
						Commands->Set_Position(UFO,Random_Map_Position());
				}
				LastCometCount = CometCount;
				UFOSpawnTime = Commands->Get_Random_Int(600,1800);
			}
		}
		Commands->Start_Timer(obj,this,0.1f,5434);
	}
	if (number == 5436)
	{//OutOfBoundsObjectsCheck
		Vector3 Pos = Commands->Get_Position(obj);
		AnObject *Current = RenCometBustersList.ObjectsList;
		while(Current)
		{
			if (!Current->AllowReplaceTime && Current->Type != TheMoon && Current->Type != CargoShip)
				if (JmgUtility::SimpleDistance(Pos,Commands->Get_Position(Current->Object)) > 28000.00)
				{
					Current->AllowReplaceTime = 1;
					Commands->Apply_Damage(Current->Object,9999.9f,"BlamoKiller",obj);
				}
			Current = Current->next;
		}
		Commands->Start_Timer(obj,this,10.0,5436);
	}
	if (number == 5437)
	{
		RenCometBustersScoreControl.StateHighScore();
		Commands->Start_Timer(obj,this,300.0f,5437);
	}
	if (number == 5438)
	{
		for (int x = 0;x < MaxGamePlayerCount;x++)
		{
			if (MiniGamePlayerControlSystem[x].PlayerShip)
				if (Commands->Get_Position(MiniGamePlayerControlSystem[x].PlayerShip).Z > 0.45)
				{
					Vector3 Pos = Commands->Get_Position(MiniGamePlayerControlSystem[x].PlayerShip);
					Pos.Z = 0.36f;
					GameObject *Object = Commands->Create_Object("Daves Arrow",Pos);
					Commands->Set_Facing(Object,Commands->Get_Facing(MiniGamePlayerControlSystem[x].PlayerShip));
					Commands->Attach_To_Object_Bone(MiniGamePlayerControlSystem[x].PlayerShip,Object,"Origin");
					Commands->Attach_To_Object_Bone(MiniGamePlayerControlSystem[x].PlayerShip,Object,"Origin");
					Commands->Destroy_Object(Object);
				}
			if (MiniGamePlayerControlSystem[x].CargoWarnDecreaseDelay)
				MiniGamePlayerControlSystem[x].CargoWarnDecreaseDelay--;
			if (!MiniGamePlayerControlSystem[x].CargoWarnDecreaseDelay)
				if (MiniGamePlayerControlSystem[x].CargoWarnedCount)
					MiniGamePlayerControlSystem[x].CargoWarnedCount--;
			if (MiniGamePlayerControlSystem[x].DoublePointsTime)
			{
				MiniGamePlayerControlSystem[x].DoublePointsTime--;
				if (!MiniGamePlayerControlSystem[x].DoublePointsTime)
				{
					GameObject *Driver = Get_Vehicle_Driver(MiniGamePlayerControlSystem[x].PlayerShip);
					if (Driver)
					{
						JmgUtility::DisplayChatMessage(Driver,200,200,125,"Your double points powerup has expired!");
						Create_2D_Sound_Player(Driver,"SFX.Comet_Busters_UI_Failed_Sound");
					}
				}
			}
		}
		BabySit++;
		if (BabySit >= 5)
		{
			//JeradTrace("CRITICAL MINIGAME ERROR DETECTED: Main timing loop stopped! Attempting to correct!",true);
			Commands->Start_Timer(obj,this,0.1f,5434);
			BabySit = 0;
		}
		Commands->Start_Timer(obj,this,1.0f,5438);
	}
}
void JMG_CMTB_Main_Game_Control::Destroyed(GameObject *obj)
{
	JMG_CMTB_Main_Game_Control::CMTBLevel = 0;
	RenCometBustersScoreControl.Cleanup();
	RenCometBustersList.ClearAllObjects();
	PlayerPerkSystemControl.ClearPerksList();
	PerkTypeSystem.Clean_Up();
}
void JMG_CMTB_Main_Game_Control::Create_Player_Ship(GameObject *obj,int PlayerNumber)
{
	GameObject *Player = Commands->Find_Object(MiniGamePlayerControlSystem[PlayerNumber].GamePlayerID);
	if (MiniGamePlayerControlSystem[PlayerNumber].RespawnTime > 30 && Player)
		MiniGamePlayerControlSystem[PlayerNumber].RespawnTime = 30;
	if (abs(MiniGamePlayerControlSystem[PlayerNumber].LastNewLifeScore-MiniGamePlayerControlSystem[PlayerNumber].Score) > 100000.0f)
	{// Give a new life
		JmgUtility::DisplayChatMessage(Player,127,255,0,"You have acquired another life!");
		Create_2D_Sound_Player(Player,"SFX.Comet_Busters_Pickup_Powerup");
		MiniGamePlayerControlSystem[PlayerNumber].NumberOfLives++;
		MiniGamePlayerControlSystem[PlayerNumber].LastNewLifeScore = MiniGamePlayerControlSystem[PlayerNumber].Score;
		RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[PlayerNumber].PlayerID)->NewLivesAcquired++;
		Set_Max_Shield_Strength(MiniGamePlayerControlSystem[PlayerNumber].SpectatorObject,(float)JmgUtility::MathClampInt(MiniGamePlayerControlSystem[PlayerNumber].NumberOfLives+1,5,100));
		Commands->Set_Shield_Strength(MiniGamePlayerControlSystem[PlayerNumber].SpectatorObject,(float)MiniGamePlayerControlSystem[PlayerNumber].NumberOfLives+1);
		if (MiniGamePlayerControlSystem[PlayerNumber].PlayerShip)
		{
			if (Commands->Get_Max_Health(MiniGamePlayerControlSystem[PlayerNumber].PlayerShip) < MiniGamePlayerControlSystem[PlayerNumber].NumberOfLives+1)
				Set_Max_Health(MiniGamePlayerControlSystem[PlayerNumber].PlayerShip,(float)MiniGamePlayerControlSystem[PlayerNumber].NumberOfLives+1);
			Commands->Set_Health(MiniGamePlayerControlSystem[PlayerNumber].PlayerShip,(float)MiniGamePlayerControlSystem[PlayerNumber].NumberOfLives+1);
		}
	}
	if (NextLevelDelay != -1 && MiniGamePlayerControlSystem[PlayerNumber].RespawnTime > 0 && MiniGamePlayerControlSystem[PlayerNumber].NumberOfLives > 0)
		MiniGamePlayerControlSystem[PlayerNumber].RespawnTime--;
	if (!MiniGamePlayerControlSystem[PlayerNumber].ReplacementShip && MiniGamePlayerControlSystem[PlayerNumber].NumberOfLives > 0)
	{// Recreate the replacement ship if it was killed
		char Spot[32];
		sprintf(Spot,"CreateS%d",PlayerNumber);
		MiniGamePlayerControlSystem[PlayerNumber].ReplacementShip = Commands->Create_Object_At_Bone(obj,"Player_Ships",Spot);
		char ShipName[32];
		sprintf(ShipName,"P%d_SHIP",PlayerNumber);
		Commands->Set_Model(MiniGamePlayerControlSystem[PlayerNumber].ReplacementShip,ShipName);
		sprintf(ShipName,"P%d_SHIP.P%d_SHIP",PlayerNumber,PlayerNumber);
		Commands->Set_Animation(MiniGamePlayerControlSystem[PlayerNumber].ReplacementShip,ShipName,false,0,0,0,false);
		Commands->Set_Health(MiniGamePlayerControlSystem[PlayerNumber].ReplacementShip,(float)MiniGamePlayerControlSystem[PlayerNumber].NumberOfLives);
	}//TODO enable end of line if spawning broken
	if (NextLevelDelay != -1 && MiniGamePlayerControlSystem[PlayerNumber].RespawnTime <= 0 && MiniGamePlayerControlSystem[PlayerNumber].NumberOfLives > 0 && !MiniGamePlayerControlSystem[PlayerNumber].PlayerShip)//5 && Get_Vehicle_Driver(MiniGamePlayerControlSystem[PlayerNumber].SpectatorObject))
	{
		int SpawnLocation = RenCometBustersList.AllowShipSpawn(PlayerNumber,SpawnLocations);
		if (SpawnLocation == MaxGamePlayerCount)
		{
			MiniGamePlayerControlSystem[PlayerNumber].RespawnTime = 10;
			return;
		}
		Commands->Create_3D_Sound_At_Bone("SFX.Comet_Busters_Ship_Spawn",obj,"origin");
		MiniGamePlayerControlSystem[PlayerNumber].PlayerShip = MiniGamePlayerControlSystem[PlayerNumber].ReplacementShip;
		MiniGamePlayerControlSystem[PlayerNumber].ShipObject = RenCometBustersList.FindObject(MiniGamePlayerControlSystem[PlayerNumber].PlayerShip);
		Commands->Set_Position(MiniGamePlayerControlSystem[PlayerNumber].PlayerShip,SpawnLocations[SpawnLocation]);
		char Spot[32];
		sprintf(Spot,"CreateS%d",PlayerNumber);
		MiniGamePlayerControlSystem[PlayerNumber].ReplacementShip = Commands->Create_Object_At_Bone(obj,"Player_Ships",Spot);
		/*int ReplacementShipID = Commands->Get_ID(MiniGamePlayerControlSystem[PlayerNumber].ReplacementShip);
		int PlayerShipID = Commands->Get_ID(MiniGamePlayerControlSystem[PlayerNumber].PlayerShip);
		for (int x = 1;x < 128;x++)
		{
			GameObject *Player = Get_GameObj(x);
			if (!Player)
				continue;
			Set_Shader_Number_Vector(Player,PlayerShipID,Vector4(4920041.0f,0.0f,0.0f,0.0f));
			Set_Shader_Number_Vector(Player,ReplacementShipID,Vector4(4920041.0f,0.0f,0.0f,0.0f));
		}*/
		Commands->Disable_Physical_Collisions(MiniGamePlayerControlSystem[PlayerNumber].ReplacementShip);
		Commands->Disable_Physical_Collisions(MiniGamePlayerControlSystem[PlayerNumber].PlayerShip);
		char ShipName[32];
		sprintf(ShipName,"P%d_SHIP",PlayerNumber);
		Commands->Set_Model(MiniGamePlayerControlSystem[PlayerNumber].ReplacementShip,ShipName);
		sprintf(ShipName,"P%d_SHIP.P%d_SHIP",PlayerNumber,PlayerNumber);
		Commands->Set_Animation(MiniGamePlayerControlSystem[PlayerNumber].ReplacementShip,ShipName,false,0,0,0,false);
		if (Commands->Get_Max_Health(MiniGamePlayerControlSystem[PlayerNumber].PlayerShip) < MiniGamePlayerControlSystem[PlayerNumber].NumberOfLives)
			Set_Max_Health(MiniGamePlayerControlSystem[PlayerNumber].PlayerShip,(float)MiniGamePlayerControlSystem[PlayerNumber].NumberOfLives);
		Commands->Set_Health(MiniGamePlayerControlSystem[PlayerNumber].PlayerShip,(float)MiniGamePlayerControlSystem[PlayerNumber].NumberOfLives);
		Commands->Set_Obj_Radar_Blip_Shape(MiniGamePlayerControlSystem[PlayerNumber].PlayerShip,RADAR_BLIP_SHAPE_VEHICLE);
		Commands->Set_Health(MiniGamePlayerControlSystem[PlayerNumber].ReplacementShip,5.0f);
		Set_Max_Shield_Strength(MiniGamePlayerControlSystem[PlayerNumber].SpectatorObject,(float)JmgUtility::MathClampInt(MiniGamePlayerControlSystem[PlayerNumber].NumberOfLives,5,100));
		Commands->Set_Shield_Strength(MiniGamePlayerControlSystem[PlayerNumber].SpectatorObject,(float)MiniGamePlayerControlSystem[PlayerNumber].NumberOfLives);
		MiniGamePlayerControlSystem[PlayerNumber].NumberOfLives--;
		switch (MiniGamePlayerControlSystem[PlayerNumber].SelectedPerk)
		{
		case 6:
			if (Commands->Get_Random(0.0f,1.0f) < 0.2)
				Commands->Send_Custom_Event(obj,MiniGamePlayerControlSystem[PlayerNumber].PlayerShip,CMTBPowerUpCustomMessage+10,-1337,1.0f);
			break;
		case 7:
			if (Commands->Get_Random(0.0f,1.0f) < 0.05)
				Commands->Send_Custom_Event(obj,MiniGamePlayerControlSystem[PlayerNumber].PlayerShip,CMTBPowerUpCustomMessage+4,-1337,1.0f);
			break;
		case 8:
			if (Commands->Get_Random(0.0f,1.0f) < 0.025)
				Commands->Send_Custom_Event(obj,MiniGamePlayerControlSystem[PlayerNumber].PlayerShip,CMTBPowerUpCustomMessage+3,-1337,1.0f);
			break;
		case 9:
			if (Commands->Get_Random(0.0f,1.0f) < 0.025)
				Commands->Send_Custom_Event(obj,MiniGamePlayerControlSystem[PlayerNumber].PlayerShip,CMTBPowerUpCustomMessage+15,-1337,1.0f);
			break;
		}
	}
	else if (MiniGamePlayerControlSystem[PlayerNumber].PlayerShip && Get_Vehicle(Player) != MiniGamePlayerControlSystem[PlayerNumber].PlayerShip)
	{
		Vector3 ShipSpot = Commands->Get_Position(MiniGamePlayerControlSystem[PlayerNumber].PlayerShip);
		if (Player && Get_Vehicle(Player) != MiniGamePlayerControlSystem[PlayerNumber].PlayerShip)
		{
			if (Get_Vehicle(Player))
				Force_Occupants_Exit(Get_Vehicle(Player));
			else
			{
				Commands->Set_Position(Player,ShipSpot);
				Soldier_Transition_Vehicle(Player);
			}
			Force_Camera_Look_Player(Player,ShipSpot);
		}
		DisableGun(Player);
	}
	else if (!MiniGamePlayerControlSystem[PlayerNumber].PlayerShip && !Get_Vehicle(Player))
	{
		if (Player)
		{
			Commands->Set_Position(Player,Commands->Get_Position(MiniGamePlayerControlSystem[PlayerNumber].SpectatorObject));
			Force_Vehicle_Entry(Player,MiniGamePlayerControlSystem[PlayerNumber].SpectatorObject);
			Force_Camera_Look_Player(Player,MiniGamePlayerControlSystem[PlayerNumber].DeathLocation);
		}
		DisableGun(Player);
	}
}
void JMG_CMTB_Main_Game_Control::DisableGun(GameObject *obj)
{
	const char *Weapon = Get_Current_Weapon(obj);
	if (Weapon)
		return;
	Commands->Select_Weapon(obj,"");
	Commands->Select_Weapon(obj,Weapon);
}
void JMG_CMTB_Main_Game_Control::FadeMusic(const char *music)
{
	for (int x = 0;x < MaxGamePlayerCount;x++)
	{
		if (MiniGamePlayerControlSystem[x].GamePlayerID)
		{
			GameObject *player = Commands->Find_Object(MiniGamePlayerControlSystem[x].GamePlayerID);
			if (player)
				Fade_Background_Music_Player(player,music,2000,2000);
		}
	}
}
void JMG_CMTB_UFO_Random_Fire_Control::Created(GameObject *obj)
{
	if (Get_Int_Parameter("UFOBoss"))
	{
		RenCometBustersList += TypeObject(obj,9.0f,3.0f,UFOBoss);
		Commands->Set_Obj_Radar_Blip_Shape(obj,RADAR_BLIP_SHAPE_OBJECTIVE);
	}
	else
		Commands->Start_Timer(obj,this,Commands->Get_Random(15.0f,45.0f),5435);
	float facing = Commands->Get_Facing(obj);
	Vector3 target = Commands->Get_Position(obj);
	target.X += cos(facing*3.14159265f/180) * 10.0f;
	target.Y += sin(facing*3.14159265f/180) * 10.0f;
	target.Z += 2.0f;
	ActionParamsStruct params;
	params.Set_Basic(this,99,1);
	params.Set_Attack(target,100.0f,0.0f,true);
	params.AttackForceFire = true;
	Commands->Action_Attack(obj,params);
	Set_Current_Clip_Bullets(obj,0);
	Commands->Set_Model(obj,"UFOJump");
	Commands->Set_Animation(obj,"UFOJump.UFOJump",false,0,0,130,false);
	Commands->Start_Timer(obj,this,1.5f,5432);
	Commands->Start_Timer(obj,this,3.0f,5433);
}
void JMG_CMTB_UFO_Random_Fire_Control::Timer_Expired(GameObject *obj,int number)
{
	if (number == 5432)
	{
		Commands->Create_Sound("SFX.Comet_Busters_Ship_Hyperspace",Commands->Get_Position(obj),obj);
	}
	if (number == 5433)
	{
		if (!Get_Int_Parameter("UFOBoss"))
			RenCometBustersList += TypeObject(obj,9.0f,3.0f,UFO);
		Commands->Set_Model(obj,"UFO");
		Commands->Set_Animation(obj,"UFO.UFO",true,0,0,-1,true);
		Commands->Start_Timer(obj,this,Commands->Get_Random(0.25,1.25)*JMG_CMTB_Main_Game_Control::DifficultyMultiplierInv,5434);
	}
	if (number == 5434)
	{
		Set_Current_Bullets(obj,1);
		Set_Current_Clip_Bullets(obj,Commands->Get_Random_Int(0,3));
		Commands->Start_Timer(obj,this,Commands->Get_Random(0.25,1.25)*JMG_CMTB_Main_Game_Control::DifficultyMultiplierInv,5434);
	}
	if (number == 5435)
	{
		RenCometBustersList -= obj;
		Commands->Action_Reset(obj,99);
		Commands->Set_Model(obj,"UFOJump");
		Commands->Set_Animation(obj,"UFOJump.UFOJump",false,0,90,0,false);
		Commands->Create_Sound("SFX.Comet_Busters_Ship_Hyperspace",Commands->Get_Position(obj),obj);
		Commands->Start_Timer(obj,this,3.0f,5436);
	}
	if (number == 5436)
	{
		Commands->Destroy_Object(obj);
	}
}
void JMG_CMTB_UFO_Random_Fire_Control::Killed(GameObject *obj, GameObject *damager)
{
	if (!(Get_Int_Parameter("UFOBoss") && RenCometBustersList.CountType(UFOBoss) == 1))
		if (Commands->Get_Random_Int(0,100) < 20+(JMG_CMTB_Main_Game_Control::CMTBLevel*0.2))// was originally 0.1 and 10
			JMG_CMTB_Random_Ship_Powerup(Commands->Get_Position(obj));
	RenCometBustersList.GrantScoreToKiller(damager,250.0f,UFO);
}
void JMG_CMTB_UFO_Random_Fire_Control::Destroyed(GameObject *obj)
{
	RenCometBustersList -= obj;
	if (Get_Int_Parameter("UFOBoss"))
		if (!RenCometBustersList.CountType(UFOBoss))
		{
			GameObject *Powerup = Commands->Create_Object("CMTB_Powerup_Object",Commands->Get_Position(obj));
			Commands->Attach_Script(Powerup,"JMG_CMTB_Powerup_Script","4,0");
		}
}
void JMG_CMTB_Ship_Control_Script::Created(GameObject *obj)
{
	prepingHyperjump = false;
	EngineDisabled = true;
	OldPos = Commands->Get_Position(obj);
	PortalInID = 0;
	PortalOutID = 0;
	ForfeitLives = 0;
	OverPoweredWeapons = false;
	Power = 100;
	ShieldLightUp = 0.0f;
	ChangingWeapons = 0;
	RapidPowerRegen = 0;
	ExtraShotActive = 0;
	RemoveExtraShot = false;
	RenCometBustersList += TypeObject(obj,1.265625f,1.125f,NormalPlayerShip);
	MyObject = RenCometBustersList.FindObject(obj);
	LastBulletCount = Get_Current_Total_Bullets(obj);
	Commands->Start_Timer(obj,this,0.1f,453212);
	Commands->Start_Timer(obj,this,0.1f,453213);
	Commands->Start_Timer(obj,this,0.1f,453214);
	Commands->Start_Timer(obj,this,0.25f,453215);
	Commands->Start_Timer(obj,this,1.0f,453216);
	Commands->Disable_Physical_Collisions(obj);
	Attach_Script_Once(obj,"JMG_RP2_Object_Send_Text_Message_On_Death",0);
}
void JMG_CMTB_Ship_Control_Script::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CMTBPowerUpCustomMessage && !Get_Vehicle_Driver(obj))
	{
		Commands->Send_Custom_Event(obj,obj,message,param,0.1f);
		return;
	}
	if (message == 7154148)
	{
		for (int x = 0;x < MaxGamePlayerCount;x++)
			if (MiniGamePlayerControlSystem[x].PlayerShip == obj)
			{
				switch (MiniGamePlayerControlSystem[x].SelectedPerk)
				{
				case 1:	JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Extra Life");break;
				case 2: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Hyperjump Powerup");break;
				case 3: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Double Points");break;
				case 4: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Energy Regen");break;
				case 5 :JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Weapons Upgrade");break;
				case 6: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Drone");break;
				case 7: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: High Yield Warhead");break;
				case 8: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: SOS Beacon");break;
				case 9: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Drone Swarm");break;
				default: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: None");break;
				}
				char inventory[220];
				sprintf(inventory,"Hyper Drive Charge %.0f.",MiniGamePlayerControlSystem[x].HyperspaceDriveCharge/5.0f*100.0f);
				JmgUtility::DisplayChatMessage(sender,200,200,125,inventory);
				if (RapidPowerRegen)
				{
					sprintf(inventory,"Rapid Energy Regen %d seconds.",RapidPowerRegen);
					JmgUtility::DisplayChatMessage(sender,200,200,125,inventory);
				}
				if (ExtraShotActive)
				{
					sprintf(inventory,"Extra Shot %d seconds.",ExtraShotActive);
					JmgUtility::DisplayChatMessage(sender,200,200,125,inventory);
				}
				if (MiniGamePlayerControlSystem[x].DoublePointsTime)
				{
					sprintf(inventory,"Double Points time %d seconds.",MiniGamePlayerControlSystem[x].DoublePointsTime);
					JmgUtility::DisplayChatMessage(sender,200,200,125,inventory);
				}
				JmgUtility::DisplayChatMessage(sender,200,200,125,"Ship Control Keys:");
				JmgUtility::DisplayChatMessage(sender,200,200,125,"R - Toggle Rapid Fire");
				JmgUtility::DisplayChatMessage(sender,200,200,125,"Q - Shield");
				JmgUtility::DisplayChatMessage(sender,200,200,125,"Z - Cloak");
				sprintf(inventory,"G - Highly Yield Warhead (x%d)",MiniGamePlayerControlSystem[x].SuperBombs);
				JmgUtility::DisplayChatMessage(sender,200,200,125,inventory);
				sprintf(inventory,"T - SOS Beacon (x%d)",MiniGamePlayerControlSystem[x].HasSOSBeacon);
				JmgUtility::DisplayChatMessage(sender,200,200,125,inventory);
				sprintf(inventory,"F - Launch Drone (x%d)",MiniGamePlayerControlSystem[x].DroneSwarmPowerup);
				JmgUtility::DisplayChatMessage(sender,200,200,125,inventory);
				sprintf(inventory,"H - Call in Drone Swarm (x%d)",MiniGamePlayerControlSystem[x].DroneSwarmPowerup);
				JmgUtility::DisplayChatMessage(sender,200,200,125,inventory);
				JmgUtility::DisplayChatMessage(sender,200,200,125,"Delete - Forfeit Lives");
				JmgUtility::DisplayChatMessage(sender,200,200,125,"Numpad[0-9] - Grant Player Life");
				break;
			}
	}
	if (message == 77454830)
	{
		if (!ForfeitLives)
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"Press ForfeitLives key again to forfeit your lives!");
			ForfeitLives = 5;
		}
		else
		{
			for (int x = 0;x < MaxGamePlayerCount;x++)
				if (MiniGamePlayerControlSystem[x].PlayerShip == obj)
				{
					MiniGamePlayerControlSystem[x].NumberOfLives = 0;
					Commands->Apply_Damage(obj,9999.9f,"BlamoKiller",0);
					GameObject *theplayer = Get_GameObj(MiniGamePlayerControlSystem[x].PlayerID);
					if (!theplayer)
						return;
					char playermsg[256];
					sprintf(playermsg,"%s forfeited %s lives!",Get_Player_Name(theplayer),(JmgUtility::JMG_Get_Sex(theplayer) == 'A' ? "his" : "her"));
					for (int y = 1;y < 128;y++)
					{
						GameObject *Player = Get_GameObj(y);
						if (!Player)
							continue;	
						JmgUtility::DisplayChatMessage(Player,200,125,125,playermsg);
					}
				}
		}
	}
	if (message == 7043453)
	{
		if (param == 2142)
		{
			param = 1;
			EngineDisabled = 25;
			Commands->Enable_Engine(obj,false);
		}
		if (MyObject->Type == PlayerSuper)
			return;
		if (!ShieldLightUp)
			Commands->Set_Animation(obj,Animation,false,0,2,2,false);
		Power -= param;
		if (Power <= 0)
		{
			Power = 0;
			ShieldOff(obj);
		}
		ShieldLightUp = 0.35f;
	}
	if (message == 77454812)
	{
		if (MyObject->Type == PlayerShipDisrupter)
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"Cannot active shield while disrupter is active!");
			Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
			return;
		}
		if (MyObject->Type == PlayerSuper || MyObject->Type == PlayerShipJumping)
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"Cannot active shields at this time!");
			Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
			return;
		}
		if (MyObject->Type == PlayerShield)
			ShieldOff(obj);
		else if (Power >= 10)
		{
			Set_Skin(obj,"Blamo");
			MyObject->Type = PlayerShield;
			MyObject->Size = 1.5625f;
			Commands->Set_Animation(obj,Animation,false,0,1,1,false);
			for (int x = 0;x < MaxGamePlayerCount;x++)
				if (MiniGamePlayerControlSystem[x].PlayerShip == obj)
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->ShieldRaisedCount++;
		}
		else
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"Insufficient power to activate shields!");
			Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
		}
	}
	if (message == 77454813)
	{
		if (MyObject->Type == PlayerShipDisrupter)
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"Cannot active cloak while disrupter is active!");
			Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
			return;
		}
		if (MyObject->Type == PlayerSuper || MyObject->Type == PlayerShipJumping)
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"Cannot active cloak at this time!");
			Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
			return;
		}
		if (MyObject->Type == PlayerShield)
		{
			if (Power < 10)
			{
				JmgUtility::DisplayChatMessage(sender,200,200,125,"Insufficient power to activate cloak!");
				Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
				return;
			}
			ShieldOff(obj);
			MyObject->Type = PlayerCloak;
			Commands->Set_Animation(obj,Animation,false,0,0,3,false);
			for (int x = 0;x < MaxGamePlayerCount;x++)
				if (MiniGamePlayerControlSystem[x].PlayerShip == obj)
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->ShipCloakedCount++;
		}
		else if (MyObject->Type == PlayerCloak)
			CloakOff(obj);
		else if (Power >= 10)
		{
			MyObject->Type = PlayerCloak;
			Commands->Set_Animation(obj,Animation,false,0,0,3,false);
			for (int x = 0;x < MaxGamePlayerCount;x++)
				if (MiniGamePlayerControlSystem[x].PlayerShip == obj)
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->ShipCloakedCount++;
		}
		else
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"Insufficient power to activate cloak!");
			Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
		}
	}
	if (message == 77454815 && MyObject->Type != PlayerSuper)
	{
		if (EngineDisabled)
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"Cannot deploy a High Yield Warhead while engines are off-line!");
			Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
			return;
		}
		if (MyObject->Type == PlayerShipDisrupter)
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"Cannot deploy a High Yield Warhead while disrupter is active!");
			Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
			return;
		}
		if (MyObject->Type == PlayerShipJumping)
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"Cannot use High Yield Warhead at this time!");
			Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
			return;
		}
		for (int x = 0;x < MaxGamePlayerCount;x++)
			if (MiniGamePlayerControlSystem[x].PlayerShip == obj)
				if (MiniGamePlayerControlSystem[x].SuperBombs)
				{
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->HEWUsed++;
					MiniGamePlayerControlSystem[x].SuperBombs--;
				}
				else
				{
					JmgUtility::DisplayChatMessage(sender,200,200,125,"You do not have any high yield warheads to deploy!");
					Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
					return;
				}
		for (int x = 1;x < 128;x++)
		{
			GameObject *Player = Get_GameObj(x);
			if (!Player)
				continue;	
			JmgUtility::DisplayChatMessage(Player,200,125,125,"//////Warning: A HIGH YIELD WARHEAD IS BEING CHARGED!\\\\\\\\\\\\");
			Create_2D_Sound_Player(Player,"SFX.Comet_Busters_UI_HYW_Fire_Sound");
		}
		ChangingWeapons = 24;
		Power = 0;
		Set_Skin(obj,"Blamo");
		MyObject->Type = PlayerSuper;
		MyObject->Size = 1.5625f;
		Commands->Set_Animation(obj,Animation,false,0,2,2,false);
		OverPoweredWeapons = false;
		Set_Current_Bullets(obj,0);
		Set_Current_Clip_Bullets(obj,0);

		GameObject *SuperWeapon = Commands->Create_Object("Daves Arrow",Commands->Get_Position(obj));
		char params[32];
		sprintf(params,"%d",Commands->Get_ID(obj));
		Commands->Attach_Script(SuperWeapon,"JMG_CMTB_Ship_Super_Weapon",params);
	}
	if (message == 7043454)
	{
		if (!param)
		{
			Commands->Set_Animation(obj,Animation,false,0,1,1,false);
			Commands->Send_Custom_Event(obj,obj,7043454,1,2.5f);
		}
		else
			ShieldOff(obj);
	}
	if (message >= CMTBPowerUpCustomMessage && message <= CMTBPowerUpCustomMessage+20)
	{
		if (_stricmp(Commands->Get_Preset_Name(sender),"CMTB_Powerup_Object") && param != -1337)
			return;
		int Points = 0;
		bool CouldJump = false;
		int ShipCharge = (message-CMTBPowerUpCustomMessage+1)*25;
		for (int x = 0;x < MaxGamePlayerCount;x++)
			if (MiniGamePlayerControlSystem[x].PlayerShip == obj)
			{
				switch (message-CMTBPowerUpCustomMessage)
				{
				case 0: case 1: case 2:
					if (Power >= 100)
					{
						JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),127,255,0,"Energy Cell Stocks are full!");
						goto SHIPFAILEDTOGETPOWERUP;
					}
					char EnergyCellMsg[200];
					sprintf(EnergyCellMsg,"%d Energy Cells acquired.",ShipCharge);
					JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),127,255,0,EnergyCellMsg);
					Power = JmgUtility::MathClampInt(Power+ShipCharge,0,100);
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->PowerPowerups++;
					Create_2D_Sound_Player(Get_Vehicle_Driver(obj),"SFX.Comet_Busters_Pickup_Powerup_Health");
					break;
				case 3:
					if (MiniGamePlayerControlSystem[x].HasSOSBeacon)
					{
						JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),127,255,0,"You can only carry one SOS Beacon at a time!");
						goto SHIPFAILEDTOGETPOWERUP;
					}
					PerkSystemIncreasePlayerPerkUnlockAmount(Get_Vehicle_Driver(obj),112,1);
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->SOSBeacons++;
					MiniGamePlayerControlSystem[x].HasSOSBeacon = true;
					JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),127,255,0,"You have acquired a SOS Beacon!");
					Create_2D_Sound_Player(Get_Vehicle_Driver(obj),"SFX.Comet_Busters_Pickup_Powerup");
					break;
				case 4:
					if (MiniGamePlayerControlSystem[x].SuperBombs >= MaxNumberOfHighYieldWarheads)
					{
						JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),127,255,0,"Sorry, your cargo bay is full!");
						goto SHIPFAILEDTOGETPOWERUP;
					}
					PerkSystemIncreasePlayerPerkUnlockAmount(Get_Vehicle_Driver(obj),111,1);
					MiniGamePlayerControlSystem[x].SuperBombs++;
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->HEWPowerups++;
					JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),127,255,0,"You have acquired a high yield warhead!");
					Create_2D_Sound_Player(Get_Vehicle_Driver(obj),"SFX.Comet_Busters_Pickup_Powerup");
					break;
				case 5:
					PerkSystemIncreasePlayerPerkUnlockAmount(Get_Vehicle_Driver(obj),105,1);
					JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),127,255,0,"You have acquired another life!");
					MiniGamePlayerControlSystem[x].NumberOfLives++;
					MiniGamePlayerControlSystem[x].LastNewLifeScore = MiniGamePlayerControlSystem[x].Score;
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->ExtraLifePowerups++;
					Set_Max_Shield_Strength(MiniGamePlayerControlSystem[x].SpectatorObject,(float)JmgUtility::MathClampInt(MiniGamePlayerControlSystem[x].NumberOfLives+1,5,100));
					Commands->Set_Shield_Strength(MiniGamePlayerControlSystem[x].SpectatorObject,(float)MiniGamePlayerControlSystem[x].NumberOfLives+1);
					if (MiniGamePlayerControlSystem[x].PlayerShip)
					{
						if (Commands->Get_Max_Health(MiniGamePlayerControlSystem[x].PlayerShip) < MiniGamePlayerControlSystem[x].NumberOfLives+1)
							Set_Max_Health(MiniGamePlayerControlSystem[x].PlayerShip,(float)MiniGamePlayerControlSystem[x].NumberOfLives+1);
						Commands->Set_Health(MiniGamePlayerControlSystem[x].PlayerShip,(float)MiniGamePlayerControlSystem[x].NumberOfLives+1);
					}
					Create_2D_Sound_Player(Get_Vehicle_Driver(obj),"SFX.Comet_Busters_Pickup_Powerup");
					break;
				case 6:	case 7:	case 8: case 9: case 11:
					if (message-CMTBPowerUpCustomMessage == 6)
						Points = 10;
					else if (message-CMTBPowerUpCustomMessage == 7)
						Points = 25;
					else if (message-CMTBPowerUpCustomMessage == 8)
						Points = 100;
					else if (message-CMTBPowerUpCustomMessage == 9)
						Points = 1000;
					else
						Points = 10000;
					if (MiniGamePlayerControlSystem[x].DoublePointsTime)
					{
						RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->DoublePointsPoints += (int)Points;
						Points *= 2;
					}
					MiniGamePlayerControlSystem[x].Score += (float)Points;
					Commands->Give_Points(Get_Vehicle_Driver(obj),(float)Points,false);
					char PointMsg[200];
					sprintf(PointMsg,"%d points acquired.",Points);
					JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),127,255,0,PointMsg);
					Create_2D_Sound_Player(Get_Vehicle_Driver(obj),"SFX.Comet_Busters_Pickup_Powerup_Points");
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->PointPowerups++;
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->PowerupPoints+=Points;
					break;
				case 10:
					if (MiniGamePlayerControlSystem[x].DronePowerup)
					{
						JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),127,255,0,"You can only carry one Drone at a time!");
						goto SHIPFAILEDTOGETPOWERUP;
					}
					PerkSystemIncreasePlayerPerkUnlockAmount(Get_Vehicle_Driver(obj),110,1);
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->PickedUpDrone++;
					MiniGamePlayerControlSystem[x].DronePowerup = true;
					JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),127,255,0,"You have acquired a Drone!");
					Create_2D_Sound_Player(Get_Vehicle_Driver(obj),"SFX.Comet_Busters_Pickup_Powerup");
					break;
				case 12:
					PerkSystemIncreasePlayerPerkUnlockAmount(Get_Vehicle_Driver(obj),50,1);
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->RegenPowerups++;
					RapidPowerRegen = 120;
					JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),127,255,0,"You have acquired a rapid energy cell regen for 2 minutes!");
					Create_2D_Sound_Player(Get_Vehicle_Driver(obj),"SFX.Comet_Busters_Pickup_Powerup_Health");
					break;
				case 13:
					PerkSystemIncreasePlayerPerkUnlockAmount(Get_Vehicle_Driver(obj),107,1);
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->DoublePointsPowerups++;
					MiniGamePlayerControlSystem[x].DoublePointsTime = 60;
					JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),127,255,0,"You have acquired a double points powerup for 1 minute!");
					Create_2D_Sound_Player(Get_Vehicle_Driver(obj),"SFX.Comet_Busters_Pickup_Powerup_Points");
					break;
				case 14:
					if (MiniGamePlayerControlSystem[x].HyperspaceDriveCharge >= 5)
					{
						JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),127,255,0,"Sorry, your hyperdrive is already fully charged!");
						goto SHIPFAILEDTOGETPOWERUP;
					}
					PerkSystemIncreasePlayerPerkUnlockAmount(Get_Vehicle_Driver(obj),106,1);
					Create_2D_Sound_Player(Get_Vehicle_Driver(obj),"SFX.Comet_Busters_Pickup_Powerup");
					if (MiniGamePlayerControlSystem[x].HyperspaceDriveCharge)
						CouldJump = true;
					if (param == -1337)
						MiniGamePlayerControlSystem[x].HyperspaceDriveCharge += 5;
					else
						MiniGamePlayerControlSystem[x].HyperspaceDriveCharge += 2;
					if (MiniGamePlayerControlSystem[x].HyperspaceDriveCharge > MaxHyperdriveCharge)
						MiniGamePlayerControlSystem[x].HyperspaceDriveCharge = MaxHyperdriveCharge;
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->DeerPowerups++;
					char drivemsg[220];
					sprintf(drivemsg,"Hyperdrive %.0f%% charged%s",100.0f*(MiniGamePlayerControlSystem[x].HyperspaceDriveCharge/(1.00f*MaxHyperdriveCharge)),(CouldJump ? "." : "; automated systems are now on-line!"));
					JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),127,255,0,drivemsg);
					if (MyObject->Type == NormalPlayerShip)
						MyObject->Type = PlayerShipJumpable;
					break;
				case 15:
					if (MiniGamePlayerControlSystem[x].DroneSwarmPowerup)
					{
						JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),127,255,0,"Your data base only has enough room for one drone swarm code!");
						goto SHIPFAILEDTOGETPOWERUP;
					}
					PerkSystemIncreasePlayerPerkUnlockAmount(Get_Vehicle_Driver(obj),113,1);
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->PickedUpDroneSwarm++;
					MiniGamePlayerControlSystem[x].DroneSwarmPowerup = true;
					JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),127,255,0,"You have acquired drone swarm calling codes!");
					Create_2D_Sound_Player(Get_Vehicle_Driver(obj),"SFX.Comet_Busters_Pickup_Powerup");
					break;
				case 16:
					PerkSystemIncreasePlayerPerkUnlockAmount(Get_Vehicle_Driver(obj),109,1);
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->RegenPowerups++;
					ExtraShotActive = 120;
					JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),127,255,0,"You have acquired a 2 minute weapon upgrade!");
					Create_2D_Sound_Player(Get_Vehicle_Driver(obj),"SFX.Comet_Busters_Pickup_Powerup_Health");
					Set_Current_Clip_Bullets(obj,Get_Current_Clip_Bullets(obj)+1);
					break;
				}
				return;
SHIPFAILEDTOGETPOWERUP:
				float Random = Commands->Get_Random(-180.0f,180.0f);
				GameObject *Powerup = Commands->Create_Object("CMTB_Powerup_Object",Vector3(Commands->Get_Position(obj).X+cos(Random*PI180)*3.0f,Commands->Get_Position(obj).Y+sin(Random*PI180)*3.0f,0.25));
				if (param == -1337)
					param = 0;
				char params[256];
				sprintf(params,"%d,%d",message-CMTBPowerUpCustomMessage,param+1);
				Commands->Attach_Script(Powerup,"JMG_CMTB_Powerup_Script",params);
				Create_2D_Sound_Player(Get_Vehicle_Driver(obj),"SFX.Comet_Busters_Pickup_Powerup_Failed");
			}
	}
	if (message == 77454816)
	{
		if (!OverPoweredWeapons && Power < 10)
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"Insufficient power to overcharge weapons!");
			Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
			return;
		}
		OverPoweredWeapons = !OverPoweredWeapons;
		ChangingWeapons = 8;
		Set_Current_Bullets(obj,0);
		Set_Current_Clip_Bullets(obj,0);
		if (OverPoweredWeapons)
			Create_2D_Sound_Player(sender,"SFX.Comet_Busters_Overcharged_Weapons_Enable");
		else
			Create_2D_Sound_Player(sender,"SFX.Comet_Busters_Overcharged_Weapons_Disable");
	}
	if (message == 77454818)
	{
		if (MyObject->Type == PlayerShipJumping)
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"Cannot deploy SOS Beacon while in hyperspace!");
			Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
			return;
		}
		if (JMG_CMTB_Main_Game_Control::PlayerSOSBeaconID)
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"Sorry, there is already a SOS Beacon deployed, please wait for it to burn out before deploying another!");
			Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
			return;
		}
		if (!(JMG_CMTB_Main_Game_Control::CMTBLevel % 25))
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"Due to sensor interference, it would do no good to deploy a beacon on this level!");
			Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
			return;
		}
		char Params[32];
		for (int x = 0;x < MaxGamePlayerCount;x++)
			if (MiniGamePlayerControlSystem[x].PlayerShip == obj)
			{
				if (MiniGamePlayerControlSystem[x].HasSOSBeacon)
				{
					if (MiniGamePlayerControlSystem[x].CargoShipIDs[6])
					{
						JmgUtility::DisplayChatMessage(sender,200,200,125,"Sorry, you can only deploy one SOS Beacon every 5 minutes!");
						Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
						return;
					}
					sprintf(Params,"%d",x);
					MiniGamePlayerControlSystem[x].Score += 10000.0f;
					MiniGamePlayerControlSystem[x].HasSOSBeacon = false;
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->UsedSOSBeacons++;
				}
				else
				{
					JmgUtility::DisplayChatMessage(sender,200,200,125,"Sorry, you don't have any SOS Beacons!");
					Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
					return;
				}
				break;
			}
		Commands->Give_Points(sender,10000.0f,false);
		Create_2D_Sound_Player(sender,"SFX.Comet_Busters_Beacon_Deploy_Sound");
		JmgUtility::DisplayChatMessage(sender,200,125,125,"SOS Beacon Deployed!");
		Vector3 ShipTargetPos = Commands->Get_Position(obj);
		ShipTargetPos.Z = 1.481f;
		GameObject *ShipBeacon = Commands->Create_Object("Daves Arrow",ShipTargetPos);
		Commands->Set_Model(ShipBeacon,"SOS_Beacon");
		JMG_CMTB_Main_Game_Control::PlayerSOSBeaconID = Commands->Get_ID(ShipBeacon);

		GameObject *Ship = Commands->Create_Object("CMTB_Cargo_Ship",ShipTargetPos);
		Commands->Attach_Script(Ship,"JMG_CMTB_Cargo_Ship_Script",Params);
		Set_Max_Health(Ship,800.0f);
		Commands->Set_Health(Ship,800.0f);
	}
	if (message == 77454819)
	{
		if (MyObject->Type == PlayerShipJumping)
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"Cannot deploy drones while in hyperspace!");
			Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
			return;
		}
		for (int x = 0;x < MaxGamePlayerCount;x++)
			if (MiniGamePlayerControlSystem[x].PlayerShip == obj)
			{
				if (MiniGamePlayerControlSystem[x].DronePowerup)
				{
					if (MiniGamePlayerControlSystem[x].Drones[0] && MiniGamePlayerControlSystem[x].Drones[1])
					{
						JmgUtility::DisplayChatMessage(sender,200,200,125,"Sorry, you can only have two drones following you at a time!");
						Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
						return;
					}
					JmgUtility::DisplayChatMessage(sender,200,125,125,"Drone Launched!");
					Create_2D_Sound_Player(sender,"SFX.Comet_Busters_Drone_Launch_Sound");
					MiniGamePlayerControlSystem[x].DronePowerup = false;
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->UsedDrones++;
					char params[32];
					sprintf(params,"%d,100,1,1",x);
					GameObject *Drone = Commands->Create_Object("CMTB_Player_Attack_Drone",Random_Map_Position());
					Commands->Attach_Script(Drone,"JMG_CMTB_Player_Drone",params);
				}
				else
				{
					JmgUtility::DisplayChatMessage(sender,200,200,125,"Sorry, you don't have any Drones to deploy!");
					Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
					return;
				}
				break;
			}
	}
	if (message == 77454833)
	{
		if (JMG_CMTB_Main_Game_Control::DroneSwarmCount > 2)
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"Sorry, a drone swarm is already present in this sector!");
			Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
			return;
		}
		if (MyObject->Type == PlayerShipJumping)
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"Cannot transmit drone swarm call codes while in hyperspace!");
			Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
			return;
		}
		for (int x = 0;x < MaxGamePlayerCount;x++)
			if (MiniGamePlayerControlSystem[x].PlayerShip == obj)
			{
				if (MiniGamePlayerControlSystem[x].DroneSwarmPowerup)
				{
					JmgUtility::DisplayChatMessage(sender,200,125,125,"Drone Swarm Called!");
					Create_2D_Sound_Player(sender,"SFX.Comet_Busters_Drone_Launch_Sound");
					MiniGamePlayerControlSystem[x].DroneSwarmPowerup = false;
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->UsedDronesSwarm++;
					char params[32];
					sprintf(params,"%d,10,1,0",x);
					for (int j = 0;j < 10;j++)
					{
						GameObject *Drone = Commands->Create_Object("CMTB_Player_Attack_Drone",Random_Map_Position());
						Commands->Attach_Script(Drone,"JMG_CMTB_Player_Drone",params);
					}
				}
				else
				{
					JmgUtility::DisplayChatMessage(sender,200,200,125,"Sorry, you don't have the codes to call a drone swarm!");
					Create_2D_Sound_Player(sender,"SFX.Comet_Busters_UI_Failed_Sound");
					return;
				}
				break;
			}
	}
	if (message >= 77454820 && message <= 77454827)
	{
		int PlayerNumber = message-77454820;
		int MyPlayerNumber = 0;
		for (int x = 0;x < MaxGamePlayerCount;x++)
			if (MiniGamePlayerControlSystem[x].PlayerShip == obj)
			{
				MyPlayerNumber = x;
				break;
			}
		if (!MiniGamePlayerControlSystem[MyPlayerNumber].NumberOfLives)
		{
			JmgUtility::DisplayChatMessage(sender,200,125,200,"Sorry, you don't have any extra lives to grant to other players!");
			return;
		}
		char LifeMsg[220];
		GameObject *OtherPlayer = Commands->Find_Object(MiniGamePlayerControlSystem[PlayerNumber].GamePlayerID);
		if (PlayerRequestingALife[PlayerNumber] && MiniGamePlayerControlSystem[MyPlayerNumber].PlayerShip && MiniGamePlayerControlSystem[MyPlayerNumber].NumberOfLives)
		{
			RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[MyPlayerNumber].PlayerID)->GrantedOtherPlayersLives++;
			RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[Get_Player_ID(OtherPlayer)].PlayerID)->ReceivedOtherPlayersLives++;
			PlayerRequestingALife[PlayerNumber] = false;
			MiniGamePlayerControlSystem[MyPlayerNumber].NumberOfLives--;
			Set_Max_Health(MiniGamePlayerControlSystem[MyPlayerNumber].PlayerShip,(float)JmgUtility::MathClampInt(MiniGamePlayerControlSystem[MyPlayerNumber].NumberOfLives+1,5,100));
			Commands->Set_Health(MiniGamePlayerControlSystem[MyPlayerNumber].PlayerShip,(float)MiniGamePlayerControlSystem[MyPlayerNumber].NumberOfLives+1);
			Set_Max_Shield_Strength(MiniGamePlayerControlSystem[MyPlayerNumber].SpectatorObject,(float)JmgUtility::MathClampInt(MiniGamePlayerControlSystem[PlayerNumber].NumberOfLives+1,5,100));
			Commands->Set_Shield_Strength(MiniGamePlayerControlSystem[MyPlayerNumber].SpectatorObject,(float)MiniGamePlayerControlSystem[PlayerNumber].NumberOfLives+1);
			Commands->Apply_Damage(obj,0.0f,"None",0);
			MiniGamePlayerControlSystem[PlayerNumber].NumberOfLives++;
			sprintf(LifeMsg,"You have granted player %d (%s) another life, thanks for being a team player!",PlayerNumber,Get_Player_Name_By_ID(Get_Player_ID(OtherPlayer)));
			JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),200,125,200,LifeMsg);
			if (Commands->Is_A_Star(OtherPlayer))
			{
				sprintf(LifeMsg,"%s has granted you another life, try to make this one last!",Get_Player_Name_By_ID(Get_Player_ID(sender)));
				JmgUtility::DisplayChatMessage(OtherPlayer,200,125,200,LifeMsg);
			}
		}
		else
		{
			sprintf(LifeMsg,"Sorry, player %d is not requesting a extra life!",PlayerNumber);
			JmgUtility::DisplayChatMessage(sender,200,125,200,LifeMsg);
		}
	}
	if (message == 1000000028)
	{
		ReturnToNormalState(obj);
		Commands->Send_Custom_Event(obj,sender,90000028,1,0);
		Commands->Set_Player_Type(sender,1);
		Commands->Set_Player_Type(obj,1);
	}
	if (message == 1000000029)
	{
		Commands->Send_Custom_Event(obj,sender,90000029,1,0);
		Commands->Set_Player_Type(sender,Get_Player_Type(sender));
	}
	if (message == 77454831)
	{
		if (prepingHyperjump)
			return;
		prepingHyperjump = true;
		Vector3 Pos = Commands->Get_Position(obj);
		float NearDist = 0;
		Vector3 IdealPos = Vector3();
		bool Found = false;
		for (int x = -6;x <= 6;x++)
			for (int y = -6;y <= 6;y++)
			{
				Pos.X = x*10.0f;
				Pos.Y = y*10.0f;
				float Dist = RenCometBustersList.FindNearestCollidableObject(obj,Pos);
				if (!Found && Dist > 10)
				{
					Found = true;
					NearDist = Dist;
					IdealPos = Pos;
				}
				else if (Dist > NearDist && Dist > 10)
				{
					NearDist = Dist;
					IdealPos = Pos;
				}
			}
		if (!Found)
			return;
		for (int x = 0;x < MaxGamePlayerCount;x++)
			if (MiniGamePlayerControlSystem[x].PlayerShip == obj)
				if (MiniGamePlayerControlSystem[x].HyperspaceDriveCharge)
				{
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->JumpsUsed++;
					MiniGamePlayerControlSystem[x].HyperspaceDriveCharge--;
					char percentMessage[200];
					sprintf(percentMessage,"(Hyperdrive charge at %.0f%%)",100.0f*(MiniGamePlayerControlSystem[x].HyperspaceDriveCharge/(1.00f*MaxHyperdriveCharge)));
					GameObject *Driver = Get_Vehicle_Driver(obj);
					if (Driver)
					{
						char drivemsg[400];
						sprintf(drivemsg,"IMMINENT COLLISION CONDITION DETECTED! AUTOMATED HYPERSPACE JUMP TRIGGERED!%s",MiniGamePlayerControlSystem[x].HyperspaceDriveCharge ? percentMessage : " ");
						JmgUtility::DisplayChatMessage(Driver,200,125,200,drivemsg);
					}
					if (!MiniGamePlayerControlSystem[x].HyperspaceDriveCharge)
					{
						JmgUtility::DisplayChatMessage(Driver,200,125,125,"HYPER DRIVES DEPLETED, HYPERSPACE NO LONGER AVAILABLE!");
						Create_2D_Sound_Player(Driver,"SFX.Comet_Busters_UI_Failed_Sound");
					}
					break;
				}
		MyObject->Type = PlayerShipJumping;
		GameObject *killMe = Commands->Find_Object(PortalInID);
		if (killMe)
			Commands->Destroy_Object(killMe);
		killMe = Commands->Find_Object(PortalOutID);
		if (killMe)
			Commands->Destroy_Object(killMe);
		GameObject *PortalIn = Commands->Create_Object("Daves Arrow",Commands->Get_Position(obj));
		Commands->Set_Model(PortalIn,"HyperIn");
		Commands->Attach_To_Object_Bone(obj,PortalIn,"origin");
		PortalInID = Commands->Get_ID(PortalIn);
		GameObject *PortalOut = Commands->Create_Object("Daves Arrow",Commands->Get_Position(obj));
		Commands->Set_Model(PortalOut,"null");
		Commands->Set_Facing(PortalOut,Commands->Get_Facing(obj));
		PortalOutID = Commands->Get_ID(PortalOut);
		Commands->Set_Is_Rendered(obj,false);
		ChangingWeapons = 24;
		Set_Current_Bullets(obj,0);
		Set_Current_Clip_Bullets(obj,0);
		Commands->Set_Position(PortalOut,IdealPos);
		Commands->Create_Sound("SFX.Comet_Busters_Ship_Hyperspace",IdealPos,obj);
		prepingHyperjump = false;
		Commands->Start_Timer(obj,this,0.5,453217);
		Commands->Start_Timer(obj,this,1.0,453218);
	}
	if (message == 77454832)
	{
		GameObject *Driver = Get_Vehicle_Driver(obj);
		if (Driver)
		{
			Set_Screen_Fade_Color_Player(Driver,0,0,0,0.25f);
			Set_Screen_Fade_Opacity_Player(Driver,0.0f,0.25f);
		}
	}
}
void JMG_CMTB_Ship_Control_Script::Timer_Expired(GameObject *obj,int number)
{
	if (number == 453214)
	{
		Commands->Set_Model(obj,Get_Model(obj));
		sprintf(Animation,"%s.%s",Get_Model(obj),Get_Model(obj));
		Commands->Set_Animation(obj,Animation,false,0,0,0,false);
	}
	if (number == 453216)
	{
		if (ForfeitLives)
			ForfeitLives--;
		if (RapidPowerRegen)
		{
			RapidPowerRegen--;
			if (!RapidPowerRegen)
			{
				GameObject *Driver = Get_Vehicle_Driver(obj);
				if (Driver)
				{
					JmgUtility::DisplayChatMessage(Driver,200,200,125,"Your power regen powerup has expired!");
					Create_2D_Sound_Player(Driver,"SFX.Comet_Busters_UI_Failed_Sound");
				}
			}
		}
		if (ExtraShotActive)
		{
			ExtraShotActive--;
			if (!ExtraShotActive)
			{
				GameObject *Driver = Get_Vehicle_Driver(obj);
				if (Driver)
				{
					JmgUtility::DisplayChatMessage(Driver,200,200,125,"Your weapon upgrade powerup has expired!");
					Create_2D_Sound_Player(Driver,"SFX.Comet_Busters_UI_Failed_Sound");
				}
				RemoveExtraShot = true;
			}
			for (int x = 0;x < MaxGamePlayerCount;x++)
				if (MiniGamePlayerControlSystem[x].PlayerShip == obj)
					RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->UpgadedWeaponTime++;
		}
		if (OverPoweredWeapons)
			DrainPower(obj);
		Commands->Start_Timer(obj,this,1.0,453216);
	}
	if (number == 453212)
	{
		if ((MyObject->Type == NormalPlayerShip || MyObject->Type == PlayerShipJumpable) && !EngineDisabled)
			if (Power < 100)
				Power++;
		if (MyObject->Type == PlayerSuper)
			Power = 0;
		if (RapidPowerRegen)
			Commands->Start_Timer(obj,this,0.5,453212);
		else
			Commands->Start_Timer(obj,this,1.0,453212);
	}
	if (number == 453215)
	{
		if (MyObject->Type == PlayerCloak)
			if (DrainPower(obj))
				CloakOff(obj);
		if (ChangingWeapons)
		{
			ChangingWeapons--;
			if (!ChangingWeapons)
			{
				Set_Current_Bullets(obj,1);
				if (OverPoweredWeapons)
					Set_Current_Clip_Bullets(obj,99);
				else
					Set_Current_Clip_Bullets(obj,ExtraShotActive ? 4 : 3);
			}
		}
		Commands->Start_Timer(obj,this,0.25,453215);
	}
	if (number == 453213)
	{
		if (Commands->Get_Shield_Strength(obj) != (float)Power)
			Commands->Set_Shield_Strength(obj,(float)Power);
		if (MyObject->Type == PlayerShield)
			if (DrainPower(obj))
				ShieldOff(obj);
		if (ShieldLightUp > 0)
		{
			ShieldLightUp -= 0.1f;
			if (ShieldLightUp <= 0.0)
			{
				ShieldLightUp = 0.0f;
				if (MyObject->Type == PlayerShield)
					Commands->Set_Animation(obj,Animation,false,0,1,1,false);
			}
		}
		int BulletCount = Get_Current_Total_Bullets(obj);
		if (LastBulletCount > BulletCount && OverPoweredWeapons)
			if (DrainPower(obj,3))
			{
				Set_Current_Bullets(obj,0);
				Set_Current_Clip_Bullets(obj,0);
			}
		LastBulletCount = BulletCount;
		float Direction = Commands->Get_Bone_Position(obj,"Backward").Z-Commands->Get_Bone_Position(obj,"Forward").Z;
		if (Direction < -0.01f && !EngineDisabled)
		{
			if (MyObject->Type != PlayerShipDisrupter && Power > 0)
			{
				ShieldOff(obj);
				MyObject->Type = PlayerShipDisrupter;
				Commands->Set_Animation(obj,Animation,false,0,4,4,false);
				JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),200,200,125,"Disruptor Engaged!");
			}
			Power -= 3;
			if (Power <= 0)
			{
				Power = 0;
				Commands->Set_Animation(obj,Animation,false,0,0,0,false);
				ReturnToNormalState(obj);
				GameObject *Driver = Get_Vehicle_Driver(obj);
				if (Driver)
				{
					JmgUtility::DisplayChatMessage(Driver,200,200,125,"Engine overload detected!");
					Create_2D_Sound_Player(Driver,"SFX.Comet_Busters_UI_Failed_Sound");
				}
				EngineDisabled = 25;
				Commands->Enable_Engine(obj,false);
			}
		}
		else if (MyObject->Type == PlayerShipDisrupter && !EngineDisabled)
		{
			Commands->Set_Animation(obj,Animation,false,0,0,0,false);
			ReturnToNormalState(obj);
		}
		else if (EngineDisabled > 0)
		{
			EngineDisabled--;
			if (!EngineDisabled)
			{
				EngineDisabled = -10;
				Commands->Enable_Engine(obj,true);
			}
		}
		else if (EngineDisabled < 0)
			EngineDisabled++;
		Commands->Start_Timer(obj,this,0.1f,453213);
	}
	if (number == 453217)
	{
		GameObject *Driver = Get_Vehicle_Driver(obj);
		if (Driver)
		{
			Set_Screen_Fade_Color_Player(Driver,0.4588f,0.7294f,1.0f,0.0f);
			Set_Screen_Fade_Opacity_Player(Driver,1.0f,0.0f);
			Commands->Send_Custom_Event(obj,obj,77454832,0,0.1f);
			Commands->Send_Custom_Event(obj,obj,77454832,0,0.25f);
		}
		GameObject *Portal = Commands->Find_Object(PortalOutID);
		if (Portal)
		{
			Commands->Attach_To_Object_Bone(obj,Portal,"origin");
			Commands->Set_Model(Portal,"HyperOut");
		}
	}
	if (number == 453218)
	{
		GameObject *Portal = Commands->Find_Object(PortalOutID);
		if (Portal)
			Commands->Destroy_Object(Portal);
		Portal = Commands->Find_Object(PortalInID);
		if (Portal)
			Commands->Destroy_Object(Portal);
		Commands->Set_Is_Rendered(obj,true);
		ChangingWeapons = 1;
		ReturnToNormalState(obj);
	}
}
void JMG_CMTB_Ship_Control_Script::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damage && damager == Get_Vehicle_Driver(obj) && !ChangingWeapons)
	{
		if (!OverPoweredWeapons)
		{
			if (Get_Current_Total_Bullets(obj) < (ExtraShotActive ? 5 : 4))
				if (RemoveExtraShot)
					RemoveExtraShot = false;
				else
					Set_Current_Clip_Bullets(obj,Get_Current_Clip_Bullets(obj)+1);
		}
		RenCometBustersScoreControl.Get_Current_Player_Score_Node(Get_Player_ID(Get_Vehicle_Driver(obj)))->RoundsFired++;
	}
	else if (!Commands->Is_A_Star(damager))
	{
		if (MyObject->Type != PlayerShield)
			return;
		if (!ShieldLightUp)
			Commands->Set_Animation(obj,Animation,false,0,2,2,false);
		ShieldLightUp = 0.35f;
	}
}
void JMG_CMTB_Ship_Control_Script::Killed(GameObject *obj, GameObject *damager)
{
	RenCometBustersList.GrantScoreToKiller(damager,-1000.0f,NormalPlayerShip);
	GameObject *Driver = Get_Vehicle_Driver(obj);
	if (Driver)
	{
		AnObject *killObj = RenCometBustersList.FindObjectByID(Commands->Get_ID(damager));
		if (killObj && killObj->Type == Asteroid)
			PerkSystemIncreasePlayerPerkUnlockAmount(Driver,104,1);
		Set_Screen_Fade_Color_Player(Driver,0,0,0,0.0f);
		Set_Screen_Fade_Opacity_Player(Driver,0.0f,0.0f);
	}
}
void JMG_CMTB_Ship_Control_Script::Destroyed(GameObject *obj)
{
	RenCometBustersList -= obj;
	for (int x = 0;x < MaxGamePlayerCount;x++)
		if (MiniGamePlayerControlSystem[x].PlayerShip == obj)
		{
			MiniGamePlayerControlSystem[x].RespawnTime = (!Get_Vehicle_Driver(obj) ? 1200 : 30);
			MiniGamePlayerControlSystem[x].DeathLocation = Commands->Get_Position(obj);
			MiniGamePlayerControlSystem[x].PlayerShip = NULL;
			MiniGamePlayerControlSystem[x].ShipObject = NULL;
			MiniGamePlayerControlSystem[x].HighestLevel = JMG_CMTB_Main_Game_Control::CMTBLevel;
			if (!MiniGamePlayerControlSystem[x].GamePlayerID)
				continue;
			GameObject *Player = Commands->Find_Object(MiniGamePlayerControlSystem[x].GamePlayerID);
			if (!Player)
				continue;
			RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->Deaths++;
			if (!MiniGamePlayerControlSystem[x].NumberOfLives)
			{
				for (int y = 0;y < MiniGamePlayerControlSystem[x].SuperBombs;y++)
				{
					Vector3 Pos = Commands->Get_Position(obj);
					Pos.Z = 0.25f;
					float Random = Commands->Get_Random(-180.0f,180.0f);
					Pos.X += cos(Random*PI180)*2.0f;
					Pos.Y += sin(Random*PI180)*2.0f;
					GameObject *Powerup = Commands->Create_Object("CMTB_Powerup_Object",Pos);
					Commands->Attach_Script(Powerup,"JMG_CMTB_Powerup_Script","4,0");
				}
				MiniGamePlayerControlSystem[x].SuperBombs = 0;
				if (MiniGamePlayerControlSystem[x].DronePowerup)
				{
					Vector3 Pos = Commands->Get_Position(obj);
					Pos.Z = 0.25f;
					float Random = Commands->Get_Random(-180.0f,180.0f);
					Pos.X += cos(Random*PI180)*2.0f;
					Pos.Y += sin(Random*PI180)*2.0f;
					MiniGamePlayerControlSystem[x].DronePowerup = false;
					GameObject *Powerup = Commands->Create_Object("CMTB_Powerup_Object",Pos);
					Commands->Attach_Script(Powerup,"JMG_CMTB_Powerup_Script","10,0");
				}
				if (MiniGamePlayerControlSystem[x].DroneSwarmPowerup)
				{
					Vector3 Pos = Commands->Get_Position(obj);
					Pos.Z = 0.25f;
					float Random = Commands->Get_Random(-180.0f,180.0f);
					Pos.X += cos(Random*PI180)*2.0f;
					Pos.Y += sin(Random*PI180)*2.0f;
					MiniGamePlayerControlSystem[x].DroneSwarmPowerup = false;
					GameObject *Powerup = Commands->Create_Object("CMTB_Powerup_Object",Pos);
					Commands->Attach_Script(Powerup,"JMG_CMTB_Powerup_Script","15,0");
				}
				if (MiniGamePlayerControlSystem[x].HasSOSBeacon)
				{
					Vector3 Pos = Commands->Get_Position(obj);
					Pos.Z = 0.25f;
					float Random = Commands->Get_Random(-180.0f,180.0f);
					Pos.X += cos(Random*PI180)*2.0f;
					Pos.Y += sin(Random*PI180)*2.0f;
					MiniGamePlayerControlSystem[x].HasSOSBeacon = false;
					GameObject *Powerup = Commands->Create_Object("CMTB_Powerup_Object",Pos);
					Commands->Attach_Script(Powerup,"JMG_CMTB_Powerup_Script","3,0");
				}
				for (int y = 0;y < MaxGamePlayerCount;y++)
				{
					GameObject *tempPlayer = Commands->Find_Object(MiniGamePlayerControlSystem[y].GamePlayerID);
					if (!tempPlayer)
						continue;
					if (y == x)
						JmgUtility::DisplayChatMessage(tempPlayer,125,200,200,"You are out of lives!");
					else
					{
						char LifeMsg[220];
						sprintf(LifeMsg,"Player %d (%s) is out of lives!",x,Get_Player_Name_By_ID(MiniGamePlayerControlSystem[x].PlayerID));
						JmgUtility::DisplayChatMessage(tempPlayer,125,200,200,LifeMsg);
					}
				}
			}
			break;
		}
		else if (MiniGamePlayerControlSystem[x].ReplacementShip == obj)
		{
			MiniGamePlayerControlSystem[x].ReplacementShip = NULL;
			break;
		}
	GameObject *Portal = Commands->Find_Object(PortalInID);
	if (Portal)
		Commands->Destroy_Object(Portal);
	Portal = Commands->Find_Object(PortalOutID);
	if (Portal)
		Commands->Destroy_Object(Portal);
}
void JMG_CMTB_Ship_Control_Script::ShieldOff(GameObject *obj)
{
	Set_Skin(obj,"None");
	MyObject->Size = 1.265625f;
	ReturnToNormalState(obj);
	Commands->Set_Animation(obj,Animation,false,0,0,0,false);
}
void JMG_CMTB_Ship_Control_Script::CloakOff(GameObject *obj)
{
	ReturnToNormalState(obj);
	Commands->Set_Animation(obj,Animation,false,0,3,0,false);
}
bool JMG_CMTB_Ship_Control_Script::DrainPower(GameObject *obj,int Drain)
{
	if (Power)
	{
		Power -= Drain;
		if (Power <= 0)
		{
			Power = 0;
			GameObject *Player = Get_Vehicle_Driver(obj);
			if (Player)
			{
				for (int x = 0;x < MaxGamePlayerCount;x++)
					if (MiniGamePlayerControlSystem[x].PlayerShip == obj)
						RenCometBustersScoreControl.Get_Current_Player_Score_Node(MiniGamePlayerControlSystem[x].PlayerID)->RanOutOfPowerCount++;
				JmgUtility::DisplayChatMessage(Get_Vehicle_Driver(obj),200,200,125,"Power levels critical! Deactivating auxiliary systems!");
				Create_2D_Sound_Player(Get_Vehicle_Driver(obj),"SFX.Comet_Busters_UI_Failed_Sound");
				if (OverPoweredWeapons)
				{
					ChangingWeapons = 8;
					Set_Current_Bullets(obj,0);
					Set_Current_Clip_Bullets(obj,0);
					OverPoweredWeapons = false;
				}
			}
			return true;
		}
	}
	return false;
}
void JMG_CMTB_Ship_Control_Script::ReturnToNormalState(GameObject *obj)
{
	for (int x = 0;x < MaxGamePlayerCount;x++)
		if (MiniGamePlayerControlSystem[x].PlayerShip == obj)
			if (MiniGamePlayerControlSystem[x].HyperspaceDriveCharge)
			{
				MyObject->Type = PlayerShipJumpable;
				break;
			}
			else
			{
				MyObject->Type = NormalPlayerShip;
				break;
			}
}
void JMG_CMTB_Comet_Script::Created(GameObject *obj)
{
	int Random = Commands->Get_Random_Int(0,8);
	if (Random <= 5)
	{
		char anim[32];
		sprintf(anim,"CometSpin%d.CometSpin%d",Random,Random);
		Commands->Set_Animation(obj,anim,true,0,0,-1,true);
	}
	int impulses = 1;
	if (Commands->Get_Random(0.0f,1.0f) < 0.25f && JMG_CMTB_Main_Game_Control::CMTBLevel)
		impulses = max((int)((Commands->Get_Random_Int(0,JMG_CMTB_Main_Game_Control::CMTBLevel)+1)*JMG_CMTB_Main_Game_Control::DifficultyMultiplier),1);
	else
		impulses = max((int)(JMG_CMTB_Main_Game_Control::CMTBLevel*JMG_CMTB_Main_Game_Control::DifficultyMultiplier),1);
	Vector3 Position = Commands->Get_Position(obj);
	Position.Z = 0.5;
	GameObject *SPOT = Commands->Create_Object("Daves Arrow",Position);
	Commands->Set_Facing(SPOT,Commands->Get_Facing(obj));
	Commands->Attach_To_Object_Bone(obj,SPOT,"origin");
	Commands->Attach_To_Object_Bone(obj,SPOT,"origin");
	switch (Get_Int_Parameter("Size"))
	{
	case 4:
		RenCometBustersList += TypeObject(obj,2626.5625f,51.25f,TheMoon);
		break;
	case 3:
		RenCometBustersList += TypeObject(obj,156.25f,12.5f,Asteroid);
		break;
	case 2:
		RenCometBustersList += TypeObject(obj,25.0f,5.0f,Asteroid);
		break;
	case 1:
		RenCometBustersList += TypeObject(obj,6.25f,2.5f,Asteroid);
		break;
	case 0:
		RenCometBustersList += TypeObject(obj,1.5625f,1.25f,Asteroid);
		break;
	}
	Commands->Destroy_Object(SPOT);

	PhysicalGameObj *physicalGameObj = obj->As_PhysicalGameObj();
	Matrix3D muzzle = physicalGameObj->As_ArmedGameObj()->Get_Muzzle();
	Vector3 impulse_pos;
	muzzle.Get_Translation(&impulse_pos);
	Vector3 impulse;
	muzzle.Get_X_Vector(&impulse);
	impulse *= Get_Mass(obj)*-0.630f*impulses;
	physicalGameObj->Peek_Physical_Object()->As_RigidBodyClass()->Apply_Impulse(impulse,impulse_pos);
	Force_Position_Update(obj);
	Force_Orientation_Update(obj);
	Force_Velocity_Update(obj);
	Update_Network_Object(obj);
}
void JMG_CMTB_Comet_Script::Killed(GameObject *obj, GameObject *damager)
{
	float Score = 0.0f;
	switch (Get_Int_Parameter("Size"))
	{
	case 3:
		Score = 10.0f;break;
	case 2:
		Score = 20.0f;break;
	case 1:
		Score = 50.0f;break;
	case 0:
		Score = 100.0f;break;
	default:
		Score = 0.0f;
		break;
	}
	RenCometBustersList.GrantScoreToKiller(damager,Score,Asteroid,Get_Int_Parameter("Size"));
}
void JMG_CMTB_Comet_Script::Destroyed(GameObject *obj)
{
	RenCometBustersList -= obj;
	if (!Get_Int_Parameter("Size"))
	{
		if (Commands->Get_Random_Int(0,100) < 10+(JMG_CMTB_Main_Game_Control::CMTBLevel*0.2))// was originally 0.1 and 8
			JMG_CMTB_Random_Ship_Powerup(Commands->Get_Position(obj));
		return;
	}
	int Number = 2;
	switch (Get_Int_Parameter("Size"))
	{
	case 3://Huge
		Number = 4;
		break;
	case 2://Large
		Number = 2;
		break;
	case 1://Medium
		Number = 2;
		break;
	case 0://Small
		Number = 0;
		break;
	}
	if (Get_Int_Parameter("Size") && JMG_CMTB_Main_Game_Control::CMTBLevel > 10 && Commands->Get_Random_Int(0,100) < (5.0f+JMG_CMTB_Main_Game_Control::CMTBLevel*0.1f))
	{
		for (int x = 0;x < Get_Int_Parameter("Size")*2;x++)
		{
			Vector3 Pos = Commands->Get_Position(obj);
			Pos.Z = 0.25f;
			float Random = Commands->Get_Random(-180.0f,180.0f);
			Pos.X += cos(Random*PI180)*(Get_Int_Parameter("Size")*2.0f);
			Pos.Y += sin(Random*PI180)*(Get_Int_Parameter("Size")*2.0f);
			GameObject *Mine = Commands->Create_Object("CMTB_Drone",Pos);
				Commands->Attach_Script(Mine,"JMG_CMTB_Motion_Mine","0");
			MoveablePhysClass *mphys = Mine->As_PhysicalGameObj() ? Mine->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
			if (mphys)
				mphys->Find_Teleport_Location(Pos,1.0f,&Pos);
			Commands->Set_Position(Mine,Pos);
		}
	}
	int UsedNumber = -1;
	char CometPreset[32];
	if (Number == 4)
		if (Commands->Get_Random(0.0f,1.0f) < 0.5)
			sprintf(CometPreset,"Comet%d",Get_Int_Parameter("Size")-2);
		else
			Number = 1;
	else
		sprintf(CometPreset,"Comet%d",Get_Int_Parameter("Size")-1);
	char params[512];
	sprintf(params,"%d,%.3f %.3f %.3f",Get_Int_Parameter("Size") == 3 ? Get_Int_Parameter("Size")-2 : Get_Int_Parameter("Size")-1,Commands->Get_Position(obj).X,Commands->Get_Position(obj).Y,Get_Int_Parameter("Size") == 3 ? 0.5 : Commands->Get_Position(obj).Z);
	for (int x = 0;x < Number;x++)
	{
		GameObject *Comet;
		if (Number != 4 && Number != 16)
		{
			int Random = Commands->Get_Random_Int(0,4);
			while (Random == UsedNumber)
				Random = Commands->Get_Random_Int(0,4);
			UsedNumber = Random;
			char BoneName[32];
			sprintf(BoneName,"Comet%02d",Random);
			Comet = Commands->Create_Object_At_Bone(obj,CometPreset,BoneName);
		}
		else
		{
			char BoneName[32];
			sprintf(BoneName,"Comet%02d",x);
			Comet = Commands->Create_Object_At_Bone(obj,CometPreset,BoneName);
		}
		Commands->Set_Facing(Comet,Commands->Get_Random(-180.0f,180.0f));
		Commands->Attach_Script(Comet,"JMG_CMTB_Comet_Script",params);
	}
}
void JMG_CMTB_Zone_Realign_Enter::Entered(GameObject *obj,GameObject *enter)
{
	Vector3 pos = Commands->Get_Position(enter);
	if (Get_Vector3_Parameter("Position").X)
		pos.X = Get_Vector3_Parameter("Position").X;
	if (Get_Vector3_Parameter("Position").Y)
		pos.Y = Get_Vector3_Parameter("Position").Y;
	if (Get_Vector3_Parameter("Position").Z)
		pos.Z = Get_Vector3_Parameter("Position").Z;
	Commands->Set_Position(enter,pos);
	if (Commands->Get_Preset_ID(enter) == 1000000250)
	{
		GameObject *Driver = Get_Vehicle_Driver(enter);
		if (Driver && Commands->Is_A_Star(Driver))
			Force_Position_Update(enter);
	}
	Commands->Set_Position(enter,pos);
}
void JMG_CMTB_Spectator_Object::Created(GameObject *obj)
{
	machineNumber = Get_Int_Parameter("Number");
	MiniGamePlayerControlSystem[machineNumber].endGamePadding = 0;
	RequestLifeDelay = 30;
	Commands->Start_Timer(obj,this,1.0f,5436);
}
void JMG_CMTB_Spectator_Object::Timer_Expired(GameObject *obj,int number)
{
	if (number == 5436)
	{
		if (RequestLifeDelay && PlayerRequestingALife[machineNumber])
		{
			GameObject *Player = Commands->Find_Object(MiniGamePlayerControlSystem[machineNumber].GamePlayerID);
			if (!Player)
				PlayerRequestingALife[machineNumber] = false;
			else
			{
				RequestLifeDelay--;
				if (!RequestLifeDelay)
				{
					RequestLifeDelay = 20;
					for (int x = 0;x < MaxGamePlayerCount;x++)
					{
						GameObject *TempPlayer = Commands->Find_Object(MiniGamePlayerControlSystem[x].GamePlayerID);
						if (TempPlayer && (MiniGamePlayerControlSystem[x].NumberOfLives && MiniGamePlayerControlSystem[x].PlayerShip) || (!MiniGamePlayerControlSystem[x].PlayerShip && MiniGamePlayerControlSystem[x].NumberOfLives > 1))
						{
							char LifeMsg[220];
							sprintf(LifeMsg,"%s is out of lives, you can give them one of you own by pressing the ShipPlayer%d key!",Get_Player_Name_By_ID(MiniGamePlayerControlSystem[machineNumber].PlayerID),machineNumber);
							JmgUtility::DisplayChatMessage(TempPlayer,200,125,200,LifeMsg);
						}
					}
				}
			}
		}
		if (MiniGamePlayerControlSystem[machineNumber].endGamePadding)
			MiniGamePlayerControlSystem[machineNumber].endGamePadding--;
		if (!JMG_CMTB_Main_Game_Control::GameInProgress)
		{
			GameObject *player = Get_Vehicle_Driver(obj);
			if (player)
			{
				bool readyPlayers = false;
				for (int x = 0;x < MaxGamePlayerCount;x++)
					if (MiniGamePlayerControlSystem[x].isReady)
					{
						readyPlayers = true;
						break;
					}
				if (readyPlayers)
				{
					MiniGamePlayerControlSystem[machineNumber].kickoutOfSpecTime++;
					if (!(MiniGamePlayerControlSystem[machineNumber].kickoutOfSpecTime % 10))
						Set_HUD_Help_Text_Player_Text(player,7233,"Press spacebar to start the game!",Vector3(0.19599999f,1.0f,0.19599999f));
					if (MiniGamePlayerControlSystem[machineNumber].kickoutOfSpecTime >= 54)
					{
						char kickoutMsg[220];
						sprintf(kickoutMsg,"%s has been kicked out of the lobby!",Get_Player_Name(player));
						JmgUtility::MessageAllPlayers(255,255,128,kickoutMsg);
						Soldier_Transition_Vehicle(player);
					}
				}
			}
		}
		Commands->Start_Timer(obj,this,1.0f,5436);
	}
}
void JMG_CMTB_Spectator_Object::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 7154148)
	{
		JmgUtility::DisplayChatMessage(sender,200,200,125,"Vehicle Special Keys:");
		JmgUtility::DisplayChatMessage(sender,200,200,125,"Spacebar - Vote to start game");
		JmgUtility::DisplayChatMessage(sender,200,200,125,"Page Up - Request a life");
		JmgUtility::DisplayChatMessage(sender,200,200,125,"E - Quit game.");
		switch (MiniGamePlayerControlSystem[machineNumber].SelectedPerk)
		{
		case 1:	JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Extra Life");break;
		case 2: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Hyperjump Powerup");break;
		case 3: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Double Points");break;
		case 4: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Energy Regen");break;
		case 5 :JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Weapons Upgrade");break;
		case 6: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Drone");break;
		case 7: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: High Yield Warhead");break;
		case 8: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: SOS Beacon");break;
		case 9: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Drone Swarm");break;
		default: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: None");break;
		}
		SpecialAbilitiesList(sender);
	}
	if (message == 77454814)
	{
		ResetCurrentAbility(sender);
		bool hasPerksToSelect = false;
		for (int x = 0;x < 8;x++)
			if (CheckIfPlayerHasPerkUnlocked(sender,105+x))
			{
				hasPerksToSelect = true;
				break;
			}
		if (hasPerksToSelect && !MiniGamePlayerControlSystem[machineNumber].SecondSpace && !MiniGamePlayerControlSystem[machineNumber].SelectedPerk)
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"You have special abilities that you could assign, or press space again to start the round:");
			SpecialAbilitiesList(sender);
			MiniGamePlayerControlSystem[machineNumber].SecondSpace = true;
			return;
		}
		MiniGamePlayerControlSystem[machineNumber].isReady = true;
		Commands->Send_Custom_Event(Get_Vehicle_Driver(obj),Commands->Find_Object(Get_Int_Parameter("ControllerID")),57447,0,0);
	}
	if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		Commands->Set_Position(obj,MiniGamePlayerControlSystem[machineNumber].specCreateSpot);
		Commands->Enable_Engine(obj,true);
		Commands->Send_Custom_Event(obj,sender,90000029,1,0);
		if (!MiniGamePlayerControlSystem[machineNumber].RespawnTime)
			return;
		Commands->Set_Position(sender,MiniGamePlayerControlSystem[machineNumber].OriginalLocation);
		Set_Skin(sender,MiniGamePlayerControlSystem[machineNumber].OriginalSkin);
		Commands->Set_Shield_Type(sender,MiniGamePlayerControlSystem[machineNumber].OriginalArmor);
		MiniGamePlayerControlSystem[machineNumber].GamePlayerID = 0;
		MiniGamePlayerControlSystem[machineNumber].PlayerID = 0;
		MiniGamePlayerControlSystem[machineNumber].RespawnTime = 30;
		GameObject *FakeSoldier = Commands->Find_Object(MiniGamePlayerControlSystem[machineNumber].FakeSoldierID);
		MiniGamePlayerControlSystem[machineNumber].FakeSoldierID = 0;
		if (!FakeSoldier)
			return;
		Commands->Set_Model(sender,Get_Model(FakeSoldier));
		Commands->Set_Shield_Strength(sender,Commands->Get_Shield_Strength(FakeSoldier));
		Commands->Set_Health(sender,Commands->Get_Health(FakeSoldier));
		Commands->Destroy_Object(FakeSoldier);
		Fade_Background_Music_Player(sender,"null.mp3",2000,2000);
	}
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!JMG_CMTB_Main_Game_Control::GameInProgress)
			ResetCurrentAbility(sender);
		if (MiniGamePlayerControlSystem[machineNumber].RespawnTime > 30)
			MiniGamePlayerControlSystem[machineNumber].RespawnTime = 30;
		MiniGamePlayerControlSystem[machineNumber].kickoutOfSpecTime = 0;
		if (!JMG_CMTB_Main_Game_Control::GameInProgress)
			if (!MiniGamePlayerControlSystem[machineNumber].endGamePadding)
			Set_HUD_Help_Text_Player_Text(sender,7233,"Press spacebar to start the game!",Vector3(0.19599999f,1.0f,0.19599999f));
	}
	if (message == 77454817)
	{
		if (!JMG_CMTB_Main_Game_Control::GameInProgress)
		{
			JmgUtility::DisplayChatMessage(sender,200,125,200,"A game must be in progress in order to request lives!");
			return;
		}
		if (MiniGamePlayerControlSystem[machineNumber].SpectatorObject == obj)
		{
			if (PlayerRequestingALife[machineNumber])
			{
				JmgUtility::DisplayChatMessage(sender,200,125,200,"Your request for a life has been withdrawn!");
				PlayerRequestingALife[machineNumber] = false;
			}
			else if (!MiniGamePlayerControlSystem[machineNumber].NumberOfLives && !MiniGamePlayerControlSystem[machineNumber].PlayerShip)
			{
				int AvailablePlayers = 0;
				for (int y = 0;y < MaxGamePlayerCount;y++)
					if ((MiniGamePlayerControlSystem[y].NumberOfLives && MiniGamePlayerControlSystem[y].PlayerShip) || (!MiniGamePlayerControlSystem[y].PlayerShip && MiniGamePlayerControlSystem[y].NumberOfLives > 1))
						AvailablePlayers++;
				if (AvailablePlayers)
				{
					RequestLifeDelay = 5;
					PlayerRequestingALife[machineNumber] = true;
					JmgUtility::DisplayChatMessage(sender,200,125,200,"Your request for a life has been dispatched to the other players in game!");
				}
				else
					JmgUtility::DisplayChatMessage(sender,200,125,200,"Sorry, no one in game has any lives to spare!");
			}
			else
				JmgUtility::DisplayChatMessage(sender,200,125,200,"Sorry, you must be out of lives before you can request more!");
		}
	}
	if (message >= 77454820 && message <= 77454829)
	{
		if (JMG_CMTB_Main_Game_Control::GameInProgress)
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"Sorry, you cannot change your special ability once the game is in progress.");
			return;
		}
		ResetCurrentAbility(sender);
		int selectedPerk = message-77454820;
		if (!CheckIfPlayerHasPerkUnlocked(sender,104+selectedPerk))
		{
			JmgUtility::DisplayChatMessage(sender,200,200,125,"Sorry, you do not have that perk unlocked yet!");
			return;
		}
		MiniGamePlayerControlSystem[machineNumber].SelectedPerk = selectedPerk;
		switch (MiniGamePlayerControlSystem[machineNumber].SelectedPerk)
		{
		case 1:	JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Extra Life");break;
		case 2: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Hyperjump Powerup");break;
		case 3: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Double Points");break;
		case 4: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Energy Regen");break;
		case 5 :JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Weapons Upgrade");break;
		case 6: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Drone");break;
		case 7: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: High Yield Warhead");break;
		case 8: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: SOS Beacon");break;
		case 9: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: Drone Swarm");break;
		default: JmgUtility::DisplayChatMessage(sender,200,200,125,"Selected Special Ability: None");break;
		}
	}
}
void JMG_CMTB_Spectator_Object::SpecialAbilitiesList(GameObject *player)
{
	if (CheckIfPlayerHasPerkUnlocked(player,105))
		JmgUtility::DisplayChatMessage(player,200,200,125,"1 - Start the game with an Extra Life and get an additional one every 25 levels.");
	if (CheckIfPlayerHasPerkUnlocked(player,106))
		JmgUtility::DisplayChatMessage(player,200,200,125,"2 - Start the game with a fully charged Hyper Drive.");
	if (CheckIfPlayerHasPerkUnlocked(player,107))
		JmgUtility::DisplayChatMessage(player,200,200,125,"3 - 16.5% chance of getting Double Points at the start of each level.");
	if (CheckIfPlayerHasPerkUnlocked(player,108))
		JmgUtility::DisplayChatMessage(player,200,200,125,"4 - 5% chance of getting Energy Regen at the start of each level.");
	if (CheckIfPlayerHasPerkUnlocked(player,109))
		JmgUtility::DisplayChatMessage(player,200,200,125,"5 - 5% chance of getting Weapons Upgrade at the start of each level.");
	if (CheckIfPlayerHasPerkUnlocked(player,110))
		JmgUtility::DisplayChatMessage(player,200,200,125,"6 - 20% chance of being granted a Drone on spawn.");
	if (CheckIfPlayerHasPerkUnlocked(player,111))
		JmgUtility::DisplayChatMessage(player,200,200,125,"7 - 5% chance of being granted a High Yield Warhead on spawn.");
	if (CheckIfPlayerHasPerkUnlocked(player,112))
		JmgUtility::DisplayChatMessage(player,200,200,125,"8 - 2.5% chance of being granted a SOS Beacon on spawn.");
	if (CheckIfPlayerHasPerkUnlocked(player,113))
		JmgUtility::DisplayChatMessage(player,200,200,125,"9 - 2.5% chance of being granted a Drone Swarm on spawn.");
}
void JMG_CMTB_Spectator_Object::ResetCurrentAbility(GameObject *player)
{
	if (_stricmp(MiniGamePlayerControlSystem[machineNumber].PlayerName,Get_Player_Name(player)))
	{
		MiniGamePlayerControlSystem[machineNumber].SelectedPerk = 0;
		sprintf(MiniGamePlayerControlSystem[machineNumber].PlayerName,"%s",Get_Player_Name(player));
	}
}
void JMG_CMTB_Arcade_Machine::Created(GameObject *obj)
{
	char Model[32];
	sprintf(Model,"ArcadeMachine_%d",Get_Int_Parameter("MachineNumber"));
	Commands->Set_Model(obj,Model);
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void JMG_CMTB_Arcade_Machine::Poked(GameObject *obj,GameObject *poker)
{
	Attach_Script_Once(poker,"JMG_CMTB_Renegade_Code","");
	Commands->Send_Custom_Event(poker,Commands->Find_Object(Get_Int_Parameter("ControllerID")),57448,Get_Int_Parameter("MachineNumber"),0.0f);
}
void JMG_CMTB_Fake_Player_Attach_Script::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,3.0f,5436);
}
void JMG_CMTB_Fake_Player_Attach_Script::Timer_Expired(GameObject *obj,int number)
{
	if (number == 5436)
	{
		if (!Commands->Find_Object(Get_Int_Parameter("MySoldierID")))
			Commands->Destroy_Object(obj);
		Commands->Start_Timer(obj,this,3.0f,5436);
	}
}
void JMG_CMTB_Fake_Player_Attach_Script::Killed(GameObject *obj, GameObject *damager)
{
	GameObject *Player = Commands->Find_Object(Get_Int_Parameter("MySoldierID"));
	if (!Player)
		return;
	Fade_Background_Music_Player(Player,"null.mp3",2000,2000);
	Commands->Set_Model(Player,Get_Model(obj));
	MiniGamePlayerControlSystem[Get_Int_Parameter("Number")].FakeSoldierID = 0;
	MiniGamePlayerControlSystem[Get_Int_Parameter("Number")].PlayerID = 0;
	MiniGamePlayerControlSystem[Get_Int_Parameter("Number")].GamePlayerID = 0;
	GameObject *MyVehicle = Get_Vehicle(Player);
	if (MyVehicle)
	{
		Vector3 Pos = Commands->Get_Position(MyVehicle);
		Commands->Set_Position(Player,Pos);
		Soldier_Transition_Vehicle(Player);
	}
	Commands->Set_Position(Player,Commands->Get_Position(obj));
	Commands->Set_Facing(Player,Commands->Get_Facing(obj));
	Commands->Apply_Damage(Player,9999.9f,"BlamoKiller",0);
	Commands->Destroy_Object(obj);
}
void JMG_CMTB_Renegade_Code::Created(GameObject *obj)
{
	PlayerPerkSystemControl.FindPlayerPerkList(obj,0);
	RenCometBustersScoreControl.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(obj));
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-ShipShield,77454812,0");
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-ShipCloak,77454813,0");	
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-StartGame,77454814,0");
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-ShipBomb,77454815,0");
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-ShipOvercharge,77454816,0");
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-RequestALife,77454817,0");
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-ShipSOSBeacon,77454818,0");
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-ShipDrone,77454819,0");
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-ForfeitLives,77454830,0");
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-UseDroneSwarmCode,77454833,0");
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-OfferLifePlayer0,77454820,0");
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-OfferLifePlayer1,77454821,0");
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-OfferLifePlayer2,77454822,0");
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-OfferLifePlayer3,77454823,0");
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-OfferLifePlayer4,77454824,0");
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-OfferLifePlayer5,77454825,0");
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-OfferLifePlayer6,77454826,0");
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-OfferLifePlayer7,77454827,0");
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-OfferLifePlayer8,77454828,0");
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-OfferLifePlayer9,77454829,0");
	Commands->Attach_Script(obj,"JFW_Send_Self_Custom_On_Key","Astrogade-KeyListing,7154148,0");
}
void JMG_CMTB_Renegade_Code::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if ((message >= 77454812 && message <= 77454819) || (message >= 77454820 && message <= 77454829) || message == 7154148 || message == 77454830 || message == 77454833)
	{
		if (sender != obj)
			return;
		GameObject *vehicle = Get_Vehicle(obj);
		if (!vehicle)
			return;
		Commands->Send_Custom_Event(obj,vehicle,message,param,0.0f);
	}
	if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		Commands->Send_Custom_Event(obj,sender,CUSTOM_EVENT_VEHICLE_EXITED,0,0.0f);
		const char *Weapon = Get_Current_Weapon(obj);
		if (Weapon)
		{
			Commands->Select_Weapon(obj,"");
			Commands->Select_Weapon(obj,Weapon);
			Commands->Send_Custom_Event(obj,obj,4564564,0,0.1f);
		}
	}
	if (message == 4564564)
	{
		const char *Weapon = Get_Current_Weapon(obj);
		if (Weapon)
			return;
		Commands->Select_Weapon(obj,"");
		Commands->Select_Weapon(obj,Weapon);
	}
}
void JMG_CMTB_Motion_Mine::Created(GameObject *obj)
{
	LastTargetObject = NULL;
	movingToLastSeenSpot = false;
	StuckCount = 0;
	NewTarget = 0;
	NewRandom = true;
	ArriveDistance = 0;
	Speed = max(0.1f,0.1f*JMG_CMTB_Main_Game_Control::DifficultyMultiplierMine);
	LastPosition = Commands->Get_Position(obj);
	FollowObjectID = 0;
	Commands->Set_Obj_Radar_Blip_Color(obj,RADAR_BLIP_COLOR_NOD);
	if (Get_Int_Parameter("Is_Boss"))
	{
		RenCometBustersList += TypeObject(obj,2.0f,1.414f,MineBoss);
		Commands->Set_Obj_Radar_Blip_Shape(obj,RADAR_BLIP_SHAPE_OBJECTIVE);
	}
	else
		RenCometBustersList += TypeObject(obj,2.0,1.414f,Mine);
	Commands->Start_Timer(obj,this,1.0f,5436);
	Commands->Start_Timer(obj,this,0.25f,5437);
}
void JMG_CMTB_Motion_Mine::Timer_Expired(GameObject *obj,int number)
{
	if (number == 5437)
	{
		Vector3 Pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(Pos,LastPosition) > 0.05)
		{
			StuckCount = 0;
			Speed = max(0.1f,Clamp_Value(Speed+0.05f*JMG_CMTB_Main_Game_Control::DifficultyMultiplierMine,0.1f,(1.25f+JMG_CMTB_Main_Game_Control::CMTBLevel*0.01f)*JMG_CMTB_Main_Game_Control::DifficultyMultiplierMine));
		}
		else
		{
			NewRandom = true;
			Speed = max(0.1f,0.1f*JMG_CMTB_Main_Game_Control::DifficultyMultiplierMine);
			GameObject *FollowObject = Commands->Find_Object(FollowObjectID);
			if (FollowObject)
			{
				StuckCount++;
				if (!(StuckCount % 10))
				{
					Pos.X += Commands->Get_Random(1.0f,5.0f)*(cos(Commands->Get_Random(-180.0f,180.0f)*3.14159265f/180));
					Pos.Y += Commands->Get_Random(1.0f,5.0f)*(sin(Commands->Get_Random(-180.0f,180.0f)*3.14159265f/180));
					Commands->Set_Position(obj,Pos);
					LastPosition = Commands->Get_Position(obj);
				}
				if (StuckCount == 24)
					Commands->Apply_Damage(obj,99999.9f,"BlamoKiller",0);
			}
		}
		GameObject *Target = Commands->Find_Object(FollowObjectID);
		if (Target && !movingToLastSeenSpot)
		{
			if (NewTarget)
			{
				NewTarget--;
				if (!NewTarget)
					NewRandom = true;
			}
			ActionParamsStruct params;
			params.Set_Basic(this,100,10);
			if (!ArriveDistance)
				params.Set_Movement(Target,Speed,0.0f,false);
			else
			{
				if (NewRandom)
				{
					NewRandom = false;
					TargetPos = Commands->Get_Position(Target);
					TargetPos.X += Commands->Get_Random(5.0f,20.0f)*(cos(Commands->Get_Random(-180.0f,180.0f)*3.14159265f/180));
					TargetPos.Y += Commands->Get_Random(5.0f,20.0f)*(sin(Commands->Get_Random(-180.0f,180.0f)*3.14159265f/180));
					NewTarget = 40;
				}
				params.Set_Movement(TargetPos,Speed,0.0f,false);
			}
			Commands->Action_Goto(obj,params);
		}
		else if (movingToLastSeenSpot)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,10);
			params.Set_Movement(lastTargetSpot,Speed,0,false);
			Commands->Action_Goto(obj,params);
			if (JmgUtility::SimpleDistance(Pos,lastTargetSpot) < 25)
				movingToLastSeenSpot = false;
		}
		else
			Commands->Action_Reset(obj,100);
		LastPosition = Pos;
		Commands->Start_Timer(obj,this,0.25f,5437);
	}
	if (number == 5436)
	{
		ArriveDistance = 0;
		if (!movingToLastSeenSpot && LastTargetObject && LastTargetObject->Type == PlayerCloak && Commands->Get_Random(0.0f,1.0f) < 0.33f)
		{
			GameObject *Target = Commands->Find_Object(LastTargetObject->ObjectID);
			if (Target)
			{
				movingToLastSeenSpot = true;
				lastTargetSpot = Commands->Get_Position(Target);
				ActionParamsStruct params;
				params.Set_Basic(this,100,10);
				params.Set_Movement(lastTargetSpot,Speed,0.0f,false);
				Commands->Action_Goto(obj,params);
			}
			LastTargetObject = NULL;
		}
		AnObject *newTargetObject = RenCometBustersList.FindMineTarget(obj,&ArriveDistance);
		if (!movingToLastSeenSpot && (!LastTargetObject || (LastTargetObject->AllowReplaceTime || LastTargetObject->Type == Mine || LastTargetObject->Type == MineBoss) || (newTargetObject && newTargetObject->ObjectID != LastTargetObject->ObjectID && Commands->Get_Random(0.0f,1.0f) < 0.05f)))
		{
			if (newTargetObject)
				FollowObjectID = newTargetObject->ObjectID;
			LastTargetObject = newTargetObject;
		}
		if (!ArriveDistance)
			NewRandom = true;
		Commands->Start_Timer(obj,this,1.0f,5436);
	}
}
void JMG_CMTB_Motion_Mine::Killed(GameObject *obj, GameObject *damager)
{
	if (!(Get_Int_Parameter("Is_Boss") && RenCometBustersList.CountType(MineBoss) == 1))
		if (Commands->Get_Random_Int(0,100) < 15+(JMG_CMTB_Main_Game_Control::CMTBLevel*0.2))// was 0.1 and 12
			JMG_CMTB_Random_Ship_Powerup(Commands->Get_Position(obj));
	Commands->Create_Explosion("Explosion_Comet_Small",Commands->Get_Position(obj),0);
	RenCometBustersList.GrantScoreToKiller(damager,200,Mine);
	Commands->Destroy_Object(obj);
}
void JMG_CMTB_Motion_Mine::Destroyed(GameObject *obj)
{
	RenCometBustersList -= obj;
	if (Get_Int_Parameter("Is_Boss"))
		if (!RenCometBustersList.CountType(MineBoss))
		{
			GameObject *Powerup = Commands->Create_Object("CMTB_Powerup_Object",Commands->Get_Position(obj));
			Commands->Attach_Script(Powerup,"JMG_CMTB_Powerup_Script","10,0");
		}
}
float JMG_CMTB_Motion_Mine::Clamp_Value(float Value,float Min,float Max)
{
	if (Value > Max)
		return Max;
	if (Value < Min)
		return Min;
	return Value;
}
void JMG_CMTB_The_Moon_Script::Created(GameObject *obj)
{
	JMG_CMTB_Main_Game_Control::MoonID = Commands->Get_ID(obj);
	lastHealth = Commands->Get_Health(obj);
	RenCometBustersList += TypeObject(obj,2626.5625f,51.25f,TheMoon);
	Commands->Start_Timer(obj,this,Commands->Get_Random(5.0f,8.75f),5437);
}
void JMG_CMTB_The_Moon_Script::Timer_Expired(GameObject *obj,int number)
{
	if (number == 5437)
	{
		if (!EnableSpawnPositions(obj))
		{
			Commands->Start_Timer(obj,this,Commands->Get_Random(5.0f,8.75f),5437);
			return;
		}
		if (RenCometBustersList.CountType(Mine) < 50)
			for (int x = 0;x < 2;x++)
			{
				int Random = Commands->Get_Random_Int(0,16);
				if (!SpawnPoints[Random])
					Random = Commands->Get_Random_Int(0,16);
				if (!SpawnPoints[Random])
					Random = Commands->Get_Random_Int(0,16);
				for (int y = (SpawnPoints[Random] ? Random : 0);y < 16;y++)
					if (SpawnPoints[y])
					{
						char Spot[32];
						sprintf(Spot,"Comet%02d",y);
						Vector3 Pos = Commands->Get_Bone_Position(obj,Spot);
						Pos.Z = 0.25f;
						GameObject *Mine = Commands->Create_Object("CMTB_Drone",Pos);
						Commands->Attach_Script(Mine,"JMG_CMTB_Motion_Mine","0");
						MoveablePhysClass *mphys = Mine->As_PhysicalGameObj() ? Mine->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
						if (mphys)
							mphys->Find_Teleport_Location(Pos,1.0f,&Pos);
						Commands->Set_Position(Mine,Pos);
						break;
					}
			}
		Commands->Start_Timer(obj,this,Commands->Get_Random(5.0f,8.75f)*JMG_CMTB_Main_Game_Control::DifficultyMultiplierInv,5437);
	}
}
void JMG_CMTB_The_Moon_Script::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damager || damage <= 0 || obj == damager)
		return;
	int damagerId = Commands->Get_Preset_ID(damager);
	if (damagerId != 1000000077 && damagerId != 1000000091 && damagerId != 1000000250 && !Commands->Is_A_Star(damager))
		Commands->Set_Health(obj,lastHealth);
	else
		lastHealth = Commands->Get_Health(obj);
}
void JMG_CMTB_The_Moon_Script::Killed(GameObject *obj, GameObject *damager)
{
	if (damager || !JMG_CMTB_Main_Game_Control::GameInProgress)
	{
		int Number = (int)(JMG_CMTB_Main_Game_Control::CMTBLevel*0.2f);
		char params[512];
		sprintf(params,"%d,%.3f %.3f %.3f",3,Commands->Get_Position(obj).X,Commands->Get_Position(obj).Y,Commands->Get_Position(obj).Z);
		for (int x = 0;x < Number;x++)
		{
			GameObject *Comet = Commands->Create_Object("Comet3",Random_Map_Position());
			Commands->Set_Facing(Comet,Commands->Get_Random(-180.0f,180.0f));
			Commands->Attach_Script(Comet,"JMG_CMTB_Comet_Script",params);
		}
		GameObject *Powerup = Commands->Create_Object("CMTB_Powerup_Object",Commands->Get_Position(obj));
		Commands->Attach_Script(Powerup,"JMG_CMTB_Powerup_Script","3,0");
		RenCometBustersList.GrantScoreToKiller(damager,0.0,TheMoon);
	}
	Commands->Create_Explosion_At_Bone("Explosion_Moon",obj,"Scale",0);
}
void JMG_CMTB_The_Moon_Script::Destroyed(GameObject *obj)
{
	GameObject *moonHolder = Commands->Find_Object(JMG_CMTB_Main_Game_Control::MoonHolderID);
	if (moonHolder)
		Commands->Destroy_Object(moonHolder);
	RenCometBustersList -= obj;
	JMG_CMTB_Main_Game_Control::MoonID = 0;
}
bool JMG_CMTB_The_Moon_Script::EnableSpawnPositions(GameObject *obj)
{
	bool success = false;
	for (int y = 0;y < 16;y++)
	{
		SpawnPoints[y] = true;
		char BoneName[32];
		sprintf(BoneName,"Comet%02d",y);
		Vector3 Pos = Commands->Get_Bone_Position(obj,BoneName);
		if (Pos.X < -85.0f || Pos.X > 85.0f)
			SpawnPoints[y] = false;
		else if (Pos.Y < -85.0f || Pos.Y > 85.0f)
			SpawnPoints[y] = false;
		else
			success = true;
	}
	return success;
}
void JMG_CMTB_Ship_Super_Weapon::Created(GameObject *obj)
{
	LastDistance = 0.0f;
	Commands->Set_Model(obj,"CMTBChargeSpark");
	Commands->Start_Timer(obj,this,3.75f,5436);
	GameObject *Player = Commands->Find_Object(Get_Int_Parameter("ControllerShipID"));
	Commands->Set_Facing(obj,Commands->Get_Facing(Player));
	Commands->Attach_To_Object_Bone(Player,obj,"origin");
	Commands->Create_Sound("SFX.Comet_Busters_Super_Weapon_Charge",Commands->Get_Position(obj),obj);
	Commands->Create_Sound("SFX.Comet_Busters_Super_Weapon_Warning",Commands->Get_Position(obj),obj);
}
void JMG_CMTB_Ship_Super_Weapon::Timer_Expired(GameObject *obj,int number)
{
	if (number == 5436)
	{
		Commands->Start_Timer(obj,this,0.25f,5437);
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj *o = current->Data();
			if (!o || !Is_Script_Attached(o,"JMG_CMTB_Player_Drone"))
				continue;
			Commands->Send_Custom_Event(obj,o,7043454,0,Commands->Get_Random(0.0f,0.25f));
		}
	}
	if (number == 5437)
	{
		LastDistance = 0.0f;
		Commands->Set_Model(obj,"CmtBSuperWeapon");
		Commands->Set_Animation(obj,"CmtBSuperWeapon.CmtBSuperWeapon",0,0,0,89,0);
		Commands->Create_Sound("SFX.Comet_Busters_Super_Weapon_Fire",Commands->Get_Position(obj),obj);
		Commands->Start_Timer(obj,this,0.1f,5438);
	}
	if (number == 5438)
	{
		GameObject *Ship = Commands->Find_Object(Get_Int_Parameter("ControllerShipID"));
		float NewDistance = Commands->Get_Distance(Commands->Get_Bone_Position(obj,"Pos150"),Commands->Get_Position(obj));
		RenCometBustersList.KillInRange(Get_Vehicle_Driver(Ship),Commands->Get_Position(obj),LastDistance*LastDistance,NewDistance*NewDistance);
		LastDistance = NewDistance;
		if (Get_Animation_Frame(obj) > 88.0f)
		{
			Commands->Send_Custom_Event(obj,Ship,7043454,0,0);
			Commands->Destroy_Object(obj);
		}
		Commands->Start_Timer(obj,this,0.1f,5438);
	}
}
void JMG_CMTB_Powerup_Script::Created(GameObject *obj)
{
	switch (Get_Int_Parameter("Custom"))
	{
	case 0:
		Commands->Set_Model(obj,"p_Energy_0");
		Commands->Set_Animation(obj,"p_Energy_0.p_Energy_0",false,0,Get_Int_Parameter("LifeTime")*30.0f,1899.0f,0);
		break;
	case 1:
		Commands->Set_Model(obj,"p_Energy_1");
		Commands->Set_Animation(obj,"p_Energy_1.p_Energy_1",false,0,Get_Int_Parameter("LifeTime")*30.0f,1899.0f,0);
		break;
	case 2:
		Commands->Set_Model(obj,"p_Energy_2");
		Commands->Set_Animation(obj,"p_Energy_2.p_Energy_2",false,0,Get_Int_Parameter("LifeTime")*30.0f,1899.0f,0);
		break;
	case 3:
		Commands->Set_Model(obj,"p_SOSBeacon");
		Commands->Set_Animation(obj,"p_SOSBeacon.p_SOSBeacon",false,0,Get_Int_Parameter("LifeTime")*30.0f,1899.0f,0);
		break;
	case 4:
		Commands->Set_Model(obj,"p_sw_HEW");
		Commands->Set_Animation(obj,"p_sw_HEW.p_sw_HEW",false,0,Get_Int_Parameter("LifeTime")*30.0f,1899.0f,0);
		break;
	case 5:
		Commands->Set_Model(obj,"p_ExtraShip");
		Commands->Set_Animation(obj,"p_ExtraShip.p_ExtraShip",false,0,Get_Int_Parameter("LifeTime")*30.0f,1899.0f,0);
		break;
	case 6:
		Commands->Set_Model(obj,"p_Points_0");
		Commands->Set_Animation(obj,"p_Points_0.p_Points_0",false,0,Get_Int_Parameter("LifeTime")*30.0f,1899.0f,0);
		break;
	case 7:
		Commands->Set_Model(obj,"p_Points_1");
		Commands->Set_Animation(obj,"p_Points_1.p_Points_1",false,0,Get_Int_Parameter("LifeTime")*30.0f,1899.0f,0);
		break;
	case 8:
		Commands->Set_Model(obj,"p_Points_2");
		Commands->Set_Animation(obj,"p_Points_2.p_Points_2",false,0,Get_Int_Parameter("LifeTime")*30.0f,1899.0f,0);
		break;
	case 9:
		Commands->Set_Model(obj,"p_Points_3");
		Commands->Set_Animation(obj,"p_Points_3.p_Points_3",false,0,Get_Int_Parameter("LifeTime")*30.0f,1899.0f,0);
		break;
	case 10:
		Commands->Set_Model(obj,"p_Drone");
		Commands->Set_Animation(obj,"p_Drone.p_Drone",false,0,Get_Int_Parameter("LifeTime")*30.0f,1899.0f,0);
		break;
	case 11:
		Commands->Set_Model(obj,"p_Points_4");
		Commands->Set_Animation(obj,"p_Points_4.p_Points_4",false,0,Get_Int_Parameter("LifeTime")*30.0f,1899.0f,0);
		break;
	case 12:
		Commands->Set_Model(obj,"p_Energy_Regen");
		Commands->Set_Animation(obj,"p_Energy_Regen.p_Energy_Regen",false,0,Get_Int_Parameter("LifeTime")*30.0f,1899.0f,0);
		break;
	case 13:
		Commands->Set_Model(obj,"P_DoublePoints");
		Commands->Set_Animation(obj,"P_DoublePoints.P_DoublePoints",false,0,Get_Int_Parameter("LifeTime")*30.0f,1899.0f,0);
		break;
	case 14:
		Commands->Set_Model(obj,"P_Deer");
		Commands->Set_Animation(obj,"P_Deer.P_Deer",false,0,Get_Int_Parameter("LifeTime")*30.0f,1899.0f,0);
		break;
	case 15:
		Commands->Set_Model(obj,"p_Swarm");
		Commands->Set_Animation(obj,"p_Swarm.p_Swarm",false,0,Get_Int_Parameter("LifeTime")*30.0f,1899.0f,0);
		break;
	case 16:
		Commands->Set_Model(obj,"p_weapUpgrade");
		Commands->Set_Animation(obj,"p_weapUpgrade.p_weapUpgrade",false,0,Get_Int_Parameter("LifeTime")*30.0f,1899.0f,0);
		break;
	}
	RenCometBustersList += TypeObject(obj,2.25,1.5f,CMTPowerup);
	TimeRemaining = Get_Int_Parameter("LifeTime");
	Commands->Start_Timer(obj,this,1.0f,92562143);
}
void JMG_CMTB_Powerup_Script::Timer_Expired(GameObject *obj,int number)
{
	if (number == 92562143)
	{
		TimeRemaining++;
		if (TimeRemaining >= 30)
			Commands->Destroy_Object(obj);
		Commands->Start_Timer(obj,this,1.0f,92562143);
	}
}
void JMG_CMTB_Powerup_Script::Killed(GameObject *obj, GameObject *damager)
{
	Commands->Send_Custom_Event(obj,damager,CMTBPowerUpCustomMessage+Get_Int_Parameter("Custom"),TimeRemaining,0);
}
void JMG_CMTB_Powerup_Script::Destroyed(GameObject *obj)
{
	RenCometBustersList -= obj;
}
void JMG_CMTB_Cargo_Ship_Script::Created(GameObject *obj)
{
	Commands->Enable_Engine(obj,true);
	RenCometBustersList += TypeObject(obj,0.0,0.0f,CargoShip,Commands->Get_ID(obj));
	CreateShipCollision(obj,0,"Colliders003",1.5f);
	CreateShipCollision(obj,1,"Colliders004",1.5f);
	CreateShipCollision(obj,2,"Colliders005",1.5f);
	CreateShipCollision(obj,6,"Colliders006",1.5f);
	CreateShipCollision(obj,3,"Colliders001",4.5f);
	CreateShipCollision(obj,4,"Colliders000",4.5f);
	CreateShipCollision(obj,5,"Colliders002",4.5f);
	
	GameObject *AAnimation = Commands->Create_Object("Daves Arrow",Commands->Get_Position(obj));//0.431f
	Commands->Set_Facing(AAnimation,Commands->Get_Random(-180.0f,180.0f));
	Commands->Set_Model(AAnimation,"CMTBCShipAnim");
	Commands->Set_Animation(AAnimation,"CMTBCShipAnim.CMTBCShipAnim",false,0,0.0f,8999.0f,false);
	AnimationControllerID = Commands->Get_ID(AAnimation);
	Commands->Attach_To_Object_Bone(obj,AAnimation,"Origin");
	Commands->Disable_Physical_Collisions(AAnimation);

	if (Get_Int_Parameter("PlayerNumber") != -1)
		MiniGamePlayerControlSystem[Get_Int_Parameter("PlayerNumber")].CargoShipIDs[6] = Commands->Get_ID(obj);
	for (int x = 0;x < 6;x++)
	{
		char TurretPosName[32];
		sprintf(TurretPosName,"TrtSpots%03d",x);
		GameObject *SPOT = Commands->Create_Object("CMTB_Cargo_Ship_Turret",Vector3(0.0f,0.0f,0.0f));
		Commands->Attach_Script(SPOT,"JMG_CMTB_Cargo_Ship_Turret","");
		Commands->Attach_To_Object_Bone(SPOT,AAnimation,TurretPosName);
		Commands->Disable_Physical_Collisions(SPOT);
		Turrets[x] = Commands->Get_ID(SPOT);
		if (Get_Int_Parameter("PlayerNumber") != -1)
			MiniGamePlayerControlSystem[Get_Int_Parameter("PlayerNumber")].CargoShipIDs[x] = Commands->Get_ID(SPOT);
	}
	Commands->Disable_Physical_Collisions(obj);
	Commands->Start_Timer(obj,this,300.0f,5437);
}
void JMG_CMTB_Cargo_Ship_Script::Timer_Expired(GameObject *obj,int number)
{
	if (number == 5437)
	{
		Commands->Destroy_Object(obj);
	}
}
void JMG_CMTB_Cargo_Ship_Script::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage && damager && Commands->Get_Player_Type(obj) != Commands->Get_Player_Type(damager))
	{
		int presetId = Commands->Get_Preset_ID(damager);
		if (presetId == 1000000074 || presetId == 1000000077)
			return;
		if (Commands->Is_A_Star(damager))
			for (int x = 0;x < MaxGamePlayerCount;x++)
			{
				GameObject *Player = Commands->Find_Object(MiniGamePlayerControlSystem[x].GamePlayerID);
				if (Player == damager)
				{
					MiniGamePlayerControlSystem[x].CargoWarnDecreaseDelay = 10;
					if (!MiniGamePlayerControlSystem[x].CargoWarnedCount)
					{
						Create_2D_Sound_Player(damager,"SFX.Comet_Busters_UI_Failed_Sound");
						if (MiniGamePlayerControlSystem[x].CargoWarnedCount > 5)
							JmgUtility::DisplayChatMessage(damager,160,127,50,"Galactic Cargo Ship: You were warned, open fire men!");
						else
							switch (MiniGamePlayerControlSystem[x].CargoTotalWarnedCount)
							{
							case 0:
								JmgUtility::DisplayChatMessage(damager,160,127,50,"Galactic Cargo Ship: Friendly fire! Repeat friendly fire! Please check your targets!");break;
							case 1:
								JmgUtility::DisplayChatMessage(damager,160,127,50,"Galactic Cargo Ship: Stop firing immanently! We are not your enemies here!");break;
							case 2:
								JmgUtility::DisplayChatMessage(damager,160,127,50,"Galactic Cargo Ship: Final warning, stop firing or we will return fire!");break;
							case 3:
								JmgUtility::DisplayChatMessage(damager,160,127,50,"Galactic Cargo Ship: You were warned, open fire men!");break;
							}
					}
					else if (MiniGamePlayerControlSystem[x].CargoWarnedCount == 4)
					{
						Create_2D_Sound_Player(damager,"SFX.Comet_Busters_UI_Failed_Sound");
						JmgUtility::DisplayChatMessage(damager,160,127,50,"Galactic Cargo Ship: You were warned, open fire men!");
						MiniGamePlayerControlSystem[x].CargoTotalWarnedCount++;
					}
					MiniGamePlayerControlSystem[x].CargoWarnedCount++;
					if (MiniGamePlayerControlSystem[x].CargoWarnedCount > 5 || MiniGamePlayerControlSystem[x].CargoTotalWarnedCount > 2)
						for (int y = 0;y < 6;y++)
						{
							GameObject *SPOT = Commands->Find_Object(Turrets[y]);
							if (SPOT)
								Commands->Send_Custom_Event(damager,SPOT,95959500,0,0);
						}
					return;
				}
			}
		for (int x = 0;x < 6;x++)
		{
			GameObject *SPOT = Commands->Find_Object(Turrets[x]);
			if (SPOT)
				Commands->Send_Custom_Event(damager,SPOT,95959500,0,0);
		}
	}
}
void JMG_CMTB_Cargo_Ship_Script::Killed(GameObject *obj, GameObject *damager)
{
	GameObject *SPOT = Commands->Find_Object(AnimationControllerID);
	if (SPOT)
	{
		int Random = (int)(Commands->Get_Random(20,25)*(Get_Animation_Frame(SPOT)/9000.0f));
		for (int x = 0;x < Random;x++)
		{
			Vector3 Spot = Commands->Get_Position(obj);
			Spot.X += Commands->Get_Random(-10.0f,10.0f);
			Spot.Y += Commands->Get_Random(-10.0f,10.0f);
			JMG_CMTB_Random_Ship_Powerup(Spot);
		}
		for (int x = 0;x < MaxGamePlayerCount;x++)
		{
			GameObject *Player = Commands->Find_Object(MiniGamePlayerControlSystem[x].GamePlayerID);
			if (!Player)
				continue;
			JmgUtility::DisplayChatMessage(Player,160,127,50,"Galactic Cargo Ship: Structural integrity failing, everyone abandon ship!");
		}
	}
	RenCometBustersList.GrantScoreToKiller(damager,0.0,CargoShip);
}
void JMG_CMTB_Cargo_Ship_Script::Destroyed(GameObject *obj)
{
	RenCometBustersList -= obj;
	for (int x = 0;x < 7;x++)
	{
		GameObject *SPOT = Commands->Find_Object(ShipCollideID[x]);
		if (SPOT)
		{
			RenCometBustersList -= SPOT;
			Commands->Destroy_Object(SPOT);
		}
	}
	for (int x = 0;x < 6;x++)
	{
		GameObject *SPOT = Commands->Find_Object(Turrets[x]);
		if (SPOT)
			Commands->Destroy_Object(SPOT);
	}
	GameObject *SPOT = Commands->Find_Object(AnimationControllerID);
	if (SPOT)
		Commands->Destroy_Object(SPOT);
	if (Get_Int_Parameter("PlayerNumber") != -1)
		for (int x = 0;x < 7;x++)
			MiniGamePlayerControlSystem[Get_Int_Parameter("PlayerNumber")].CargoShipIDs[x] = 0;
}
void JMG_CMTB_Cargo_Ship_Script::CreateShipCollision(GameObject *obj,int index,const char *boneName,float size)
{
	GameObject *SPOT = Commands->Create_Object("Daves Arrow",Vector3(0.0f,0.0f,0.0f));
	RenCometBustersList += TypeObject(SPOT,size*size,size,CargoShip,Commands->Get_ID(obj));
	Commands->Set_Model(SPOT,"w_invs");
	Commands->Attach_To_Object_Bone(SPOT,obj,boneName);
	Commands->Disable_Physical_Collisions(SPOT);
	ShipCollideID[index] = Commands->Get_ID(SPOT);
	char params[225];
	sprintf(params,"%d",Commands->Get_ID(obj));
	Commands->Attach_Script(SPOT,"JMG_CMTB_Cargo_Ship_Child_Script",params);
	Commands->Disable_Physical_Collisions(SPOT);
}
void JMG_CMTB_Cargo_Ship_Turret::Created(GameObject *obj)
{
	MissAmount = 1.0f;
	HuntingPlayer = 0;
	PrimaryFire = true;
	FireWeapons = 0;
	GiveUpTime = 0;
	NearestDistance = 9999999.9f;
	EnemyID = 0;
	ReaimRequired = false;
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,0.1f,92562142);
	Commands->Start_Timer(obj,this,Commands->Get_Random(0.75f,1.25f),92562143);
}
void JMG_CMTB_Cargo_Ship_Turret::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	int presetId = Commands->Get_Preset_ID(seen);
	if (HuntingPlayer || Commands->Is_A_Star(seen) || Get_Vehicle_Driver(seen) || presetId == PLAYERDRONE_ID || presetId == 1000000074 || presetId == 1000000077)
		return;
	int TargetID = Commands->Get_ID(seen);
	Vector3 TargetPos = Commands->Get_Position(seen);
	float ShortDistance = JmgUtility::SimpleDistance(Commands->Get_Position(obj),TargetPos);
	if (!CheckIfInTargetableRange(obj,TargetPos))
		return;
	if (EnemyID == TargetID)
	{
		GiveUpTime = 30;
		NearestDistance = ShortDistance;
	}
	else if (ShortDistance <= 10000 && ShortDistance < NearestDistance)
	{
		GiveUpTime = 30;
		ReaimRequired = true;
		EnemyID = TargetID;
		NearestDistance = ShortDistance;
	}
}
void JMG_CMTB_Cargo_Ship_Turret::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 95959500)
	{
		EndAttack(obj);
		HuntingPlayer = 30;
		PrimaryFire = false;
		EnemyID = Commands->Get_ID(Get_Vehicle(sender));
	}
}
void JMG_CMTB_Cargo_Ship_Turret::Timer_Expired(GameObject *obj,int number)
{
	if (number == 92562143)
	{
		MissAmount = Commands->Get_Random(0.0f,1.0f) < 0.5f ? 1.0f : Commands->Get_Random(0.25f,0.75f);
		Commands->Start_Timer(obj,this,Commands->Get_Random(0.75f,1.25f),92562143);
	}
	if (92562142 == number)
	{
		if (HuntingPlayer)
		{
			HuntingPlayer--;
			if (!HuntingPlayer)
				EndAttack(obj);
		}
		GameObject *Target = Commands->Find_Object(EnemyID);
		if (!Target)
		{
			if (EnemyID)
				EndAttack(obj);
		}
		else
		{
			Vector3 MyPos = Commands->Get_Position(obj);
			Vector3 EnemySpot = Commands->Get_Position(Target);
			float TargetDistance = JmgUtility::SimpleDistance(MyPos,EnemySpot);
			if (GiveUpTime)
			{
				GiveUpTime -= 1;
				if (!GiveUpTime)
					EndAttack(obj);
			}
			if (TargetDistance > 10000)
				EndAttack(obj);
			else if (!CheckIfInTargetableRange(obj,EnemySpot))
				EndAttack(obj);
			else if (ReaimRequired)
			{
				ReaimRequired = false;
				LastPosition[0] = LastPosition[1] = LastPosition[2] = Commands->Get_Position(Target);
			}
			else
			{
				Vector3 TargetSpot = EnemySpot;
				LastPosition[2] = LastPosition[1];
				LastPosition[1] = LastPosition[0];
				LastPosition[0] = EnemySpot;
				FireWeapons++;
				if (FireWeapons > 3)
				{
					FireWeapons = 0;
					float TravelTime = TargetDistance/(5625.0f*MissAmount);
					Vector3 Movement = Vector3(LastPosition[0].X - LastPosition[1].X,LastPosition[0].Y - LastPosition[1].Y,0.0f);
					Vector3 Movement2 = Vector3(LastPosition[1].X - LastPosition[2].X,LastPosition[1].Y - LastPosition[2].Y,0.0f);
					Vector3 Acceleration = Vector3(SafeDiv(Movement.X,Movement2.X),SafeDiv(Movement.Y,Movement2.Y),0.0f);
					TargetSpot.X += (10.0f * Movement.X * TravelTime);
					TargetSpot.Y += (10.0f * Movement.Y * TravelTime);
					TargetSpot.Z = Commands->Get_Bone_Position(obj,"Barrel").Z;

					ActionParamsStruct params;
					params.Set_Basic(this,0,100);
					params.Set_Attack(TargetSpot,100.0f,0,PrimaryFire);
					params.AttackCheckBlocked = false;
					Commands->Action_Attack(obj,params);
				}
			}
		}
		Commands->Start_Timer(obj,this,0.1f,92562142);
	}
}
void JMG_CMTB_Cargo_Ship_Turret::Destroyed(GameObject *obj)
{
}
float JMG_CMTB_Cargo_Ship_Turret::SafeDiv(float Number1,float Number2)
{
	if (!Number2)
		return 1.0f;
	return Number1/Number2;
}
void JMG_CMTB_Cargo_Ship_Turret::EndAttack(GameObject *obj)
{
	PrimaryFire = true;
	GiveUpTime = 0;
	EnemyID = 0;
	NearestDistance = 9999999.9f;
	ReaimRequired = true;
	Commands->Action_Reset(obj,100);
}
bool JMG_CMTB_Cargo_Ship_Turret::CheckIfInTargetableRange(GameObject *obj,const Vector3 &Pos)
{
	Vector3 MyPos = Commands->Get_Position(obj);
	MyPos.X = Pos.X-MyPos.X;
	MyPos.Y = Pos.Y-MyPos.Y;
	float TargetDirection = atan2(MyPos.Y,MyPos.X)*180/PI;
	float Facing = Commands->Get_Facing(obj);
	if (JmgUtility::DegreeDiff(TargetDirection,Facing,67.5f))
		return true;
	return false;
}
void JMG_CMTB_Player_Drone::Created(GameObject *obj)
{
	if (!Get_Int_Parameter("PlayerEscort"))
		JMG_CMTB_Main_Game_Control::DroneSwarmCount++;
	ShieldOverride = false;
	SearchTimeDelay = 0;
	TargetSize = 0.0f;
	ExtremeStuckCount = 0;
	Commands->Enable_Engine(obj,true);
	sprintf(myAnimation,"FighterDrone%d",Get_Int_Parameter("PlayerNumber"));
	Commands->Set_Model(obj,myAnimation);
	sprintf(myAnimation,"FighterDrone%d.FighterDrone%d",Get_Int_Parameter("PlayerNumber"),Get_Int_Parameter("PlayerNumber"));
	Commands->Set_Animation(obj,myAnimation,true,0,0,0,true);
	MyState = DroneIdle;
	LastState = DroneIdle;
	AIStatePriorities[DroneIdle]=0;
	AIStatePriorities[DroneGuarding] = 1;
	AIStatePriorities[DroneAvoiding] = 3;
	AIStatePriorities[DroneAttacking] = 2;
	AIStatePriorities[DroneReturning] = 2;
	StuckCount = 0;
	LastTargetID = TargetID = 0;
	NewTargetTime = 0;
	StateTimeOutTime = 0;
	LastPrimaryFire = PrimaryFire = true;
	AvoidDegree = Commands->Get_Random(-90.0f,90.0f);
	AvoidDistance = Commands->Get_Random(5.0f,20.0f);
	TargetPos = Vector3(0.0f,0.0f,0.0f);
	LastPosition = Commands->Get_Position(obj);
	FollowAngle = 0;
	CurrentSpeed = 0;
	Power = Get_Int_Parameter("Power");
	ShieldLightUp = 0;
	if (Get_Int_Parameter("PlayerEscort"))
	{
		for (int x = 0;x < 2;x++)
			if (!MiniGamePlayerControlSystem[Get_Int_Parameter("PlayerNumber")].Drones[x])
			{
				FollowAngle = (!x ? -33.0f : 33.0f);
				MiniGamePlayerControlSystem[Get_Int_Parameter("PlayerNumber")].Drones[x] = Commands->Get_ID(obj);
				break;
			}
	}
	else
	{
		sprintf(myAnimation,"FighterDronez");
		Commands->Set_Model(obj,myAnimation);
		sprintf(myAnimation,"FighterDronez.FighterDronez");
		Commands->Set_Animation(obj,myAnimation,true,0,0,0,true);
	}
	if (Get_Int_Parameter("RePosition"))
	{
		Position_At_Player(obj);
		Commands->Create_Explosion("Explosion_Drone_Warp_In",Commands->Get_Position(obj),0);
	}
	RenCometBustersList += TypeObject(obj,0.105625f,0.325f,PlayerDrone);
	MyObject = RenCometBustersList.FindObject(obj);
	if (!MyObject)
		return;
	Commands->Start_Timer(obj,this,Commands->Get_Random(9.0f,11.0f),5435);
	Commands->Start_Timer(obj,this,1.0f,5436);
	Commands->Start_Timer(obj,this,0.25f,5437);
	Commands->Start_Timer(obj,this,0.1f,5438);
	obj->As_PhysicalGameObj()->Peek_Physical_Object()->Set_Collision_Group(Collision_Group_Type::SOLDIER_GHOST_COLLISION_GROUP);
}
void JMG_CMTB_Player_Drone::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (!MyObject)
	{// Protect against crashes caused by accessing data nodes before ::Created has finished
		Commands->Send_Custom_Event(sender,obj,message,param,0.1f);
		return;
	}
	if (message == 7043453 && !ShieldOverride)
	{
		if (!ShieldLightUp)
			Commands->Set_Animation(obj,myAnimation,false,0,4,4,false);
		Power -= param;
		if (Power <= 0)
		{
			Power = 0;
			ShieldOff(obj);
		}
		ShieldLightUp = 0.35f;
	}
	if (message == 7043454)
	{
		for (int x = 0;x < 3;x++)
			Power -= Commands->Get_Random_Int(0,3);
		if (Power <= 0)
			Power = 0;
		if (Power > 0)
		{
			ShieldOverride = true;
			ShieldOn(obj);
			Commands->Send_Custom_Event(obj,obj,7043455,0,3.0f);
		}
	}
	if (message == 7043455)
	{
		ShieldOverride = false;
	}
}
void JMG_CMTB_Player_Drone::Timer_Expired(GameObject *obj,int number)
{
	if (!MyObject)
	{// Protect against crashes caused by accessing data nodes before ::Created has finished
		Commands->Start_Timer(obj,this,0.1f,number);
		return;
	}
	if (number == 5434)
	{
		PrimaryFire = true;
		if (MyState == DroneAttacking)
			Drone_Attack(obj);
		Set_Current_Bullets(obj,2);
		Set_Current_Clip_Bullets(obj,0);
	}
	if (number == 5435)
	{
		if (MyState == DroneAttacking)
		{
			PrimaryFire = false;
			Drone_Attack(obj);
		}
		Commands->Start_Timer(obj,this,0.5f,5434);
		Commands->Start_Timer(obj,this,Commands->Get_Random(9.0f,11.0f),5435);
	}
	if (number == 5436)
	{
		if (Commands->Get_Random(0.0f,1.0f) < 0.15f)
		{
			AvoidDegree = Commands->Get_Random(-90.0f,90.0f);
			AvoidDistance = Commands->Get_Random(5.0f,20.0f);
		}
		Vector3 Pos = Commands->Get_Position(obj);
		GameObject *Object = NULL;
		if (MiniGamePlayerControlSystem[Get_Int_Parameter("PlayerNumber")].PlayerShip && Get_Int_Parameter("PlayerEscort"))
			Object = RenCometBustersList.CheckForDroneTarget(obj,Commands->Get_Position(MiniGamePlayerControlSystem[Get_Int_Parameter("PlayerNumber")].PlayerShip),&TargetSize);
		else
			Object = RenCometBustersList.CheckForDroneTarget(obj,Pos,&TargetSize);
		if (Object)
			if (ChangeState(DroneAttacking))
				TargetID = Commands->Get_ID(Object);
		Commands->Start_Timer(obj,this,1.0f,5436);
	}
	if (number == 5437)
	{
		if (!Commands->Get_Health(obj))
			return;
		if (StateTimeOutTime)
		{
			StateTimeOutTime--;
			if (!StateTimeOutTime)
				Drone_Action_Complete(obj);
		}
		Vector3 Pos = Commands->Get_Position(obj);
		CurrentSpeed = JmgUtility::SimpleDistance(Pos,LastPosition);
		float AvoidanceDistance = 0;
		int ObjectID = RenCometBustersList.FindDroneAvoidObjectInRange(obj,&Pos,56.25f+CurrentSpeed,&AvoidanceDistance);
		if (ObjectID && !ShieldOverride)
		{
			if  (LastTargetID)
				LastTargetID = 0;
			ChangeState(DroneAvoiding);
			AvoidObjectID = ObjectID;
			if (AvoidanceDistance < 100*CurrentSpeed*0.5)
			{
				if (MyObject->Type != PlayerDroneShield && Power)
					ShieldOn(obj);
			}
			else if (MyObject->Type == PlayerDroneShield)
				ShieldOff(obj);
		}
		else if (MyObject->Type == PlayerDroneShield && !ShieldOverride)
			ShieldOff(obj);
		if (MyState == DroneAvoiding)
		{
			GameObject *AvoidObject = Commands->Find_Object(AvoidObjectID);
			if (!AvoidObject || JmgUtility::SimpleDistance(Commands->Get_Position(AvoidObject),Pos) > 225)
			{
				AvoidObjectID = 0;
				Drone_Action_Complete(obj);
			}
		}
		if (CurrentSpeed > 0.05)
			StuckCount = 0;
		else
		{
			StuckCount++;
			if (!(StuckCount % 10))
			{
				AI_Update(obj,true);
				ExtremeStuckCount++;
				if (!(ExtremeStuckCount % 30))
					Position_At_Player(obj);
				if (ExtremeStuckCount > 100)
					Commands->Apply_Damage(obj,10000,"BlamoKiller",0);
			}
		}
		LastPosition = Pos;
		if (MyState != DroneReturning)
			if (MiniGamePlayerControlSystem[Get_Int_Parameter("PlayerNumber")].PlayerShip && Get_Int_Parameter("PlayerEscort"))
				if (JmgUtility::SimpleDistance(Commands->Get_Position(MiniGamePlayerControlSystem[Get_Int_Parameter("PlayerNumber")].PlayerShip),Pos) > 1000)
					ChangeState(DroneReturning);
		if (MyState == DroneAttacking)
		{
			GameObject *Target = Commands->Find_Object(TargetID);
			if (Target)
			{
				if (NewTargetTime)
				{
					NewTargetTime--;
					if (!NewTargetTime)
						Drone_Action_Complete(obj);
				}
			}
			else
			{
				MyState = DroneGuarding;
				Commands->Action_Reset(obj,100);
			}
		}
		AI_Update(obj);
		if ((Commands->Get_Bone_Position(obj,"LWing").Z > Pos.Z || Commands->Get_Bone_Position(obj,"RWing").Z > Pos.Z) && !ShieldOverride)
		{
			for (int x = 0;x < 2;x++)
				if (MiniGamePlayerControlSystem[Get_Int_Parameter("PlayerNumber")].Drones[x] == Commands->Get_ID(obj))
					MiniGamePlayerControlSystem[Get_Int_Parameter("PlayerNumber")].Drones[x] = 0;
			char params[32];
			sprintf(params,"%d,%d,0,%d",Get_Int_Parameter("PlayerNumber"),Power,Get_Int_Parameter("PlayerEscort"));
			GameObject *Drone = Commands->Create_Object("CMTB_Player_Attack_Drone",Commands->Get_Position(obj));
			Commands->Set_Facing(Drone,Commands->Get_Facing(obj));
			Commands->Attach_Script(Drone,"JMG_CMTB_Player_Drone",params);
			Commands->Destroy_Object(obj);
		}
		if (ShieldLightUp > 0 && !ShieldOverride)
		{
			ShieldLightUp -= 0.1f;
			if (ShieldLightUp <= 0.0)
			{
				ShieldLightUp = 0.0f;
				if (MyObject->Type == PlayerDroneShield)
					Commands->Set_Animation(obj,myAnimation,false,0,2,2,false);
			}
		}
		Commands->Start_Timer(obj,this,0.1f,5437);
	}
	if (number == 5438)
	{
		Commands->Set_Model(obj,Get_Model(obj));
	}
}
void JMG_CMTB_Player_Drone::Killed(GameObject *obj, GameObject *damager)
{
	RenCometBustersList -= obj;
	Commands->Create_Explosion("Explosion_Comet_Small",Commands->Get_Position(obj),0);
	RenCometBustersList.GrantScoreToKiller(damager,-300.0f,PlayerDrone);
	Commands->Destroy_Object(obj);
}
void JMG_CMTB_Player_Drone::Destroyed(GameObject *obj)
{
	if (!Get_Int_Parameter("PlayerEscort"))
		JMG_CMTB_Main_Game_Control::DroneSwarmCount--;
	MyObject = NULL;
	RenCometBustersList -= obj;
	for (int x = 0;x < 2;x++)
		if (MiniGamePlayerControlSystem[Get_Int_Parameter("PlayerNumber")].Drones[x] == Commands->Get_ID(obj))
			MiniGamePlayerControlSystem[Get_Int_Parameter("PlayerNumber")].Drones[x] = 0;
	if (Is_Script_Attached(obj,"JMG_CMTB_Player_Drone"))
		Remove_Script(obj,"JMG_CMTB_Player_Drone");
}
bool JMG_CMTB_Player_Drone::ChangeState(AI_State NewState)
{
	if (AIStatePriorities[NewState] > AIStatePriorities[MyState])
	{
		LastState = MyState;
		MyState = NewState;
		return true;
	}
	return false;
}
void JMG_CMTB_Player_Drone::Drone_Action_Complete(GameObject *obj)
{
	if (LastState == MyState)
		LastState = DroneIdle;
	MyState = LastState;
	LastState = DroneIdle;
}
void JMG_CMTB_Player_Drone::AI_Update(GameObject *obj,bool Reset)
{
	if (!MyObject)
	{// Protect against crashes caused by accessing data nodes before ::Created has finished
		return;
	}
	if (Reset)
	{
		StateTimeOutTime = 0;
		MyState = DroneGuarding;
	}
	if (StateTimeOutTime)
		return;
	ActionParamsStruct params;
	GameObject *AvoidObject = NULL;
	Vector3 MyPos = Commands->Get_Position(obj);
	Vector3 Pos;
	float Facing = 0;
	float Rotation;
	float TargetDirection;
	float Distance;
	switch (MyState)
	{
	case DroneIdle:
		MyState = DroneGuarding;
		AI_Update(obj);
		break;
	case DroneGuarding: case DroneReturning:
		if (SearchTimeDelay)
			SearchTimeDelay--;
		if (MiniGamePlayerControlSystem[Get_Int_Parameter("PlayerNumber")].PlayerShip && Get_Int_Parameter("PlayerEscort"))
		{
			Facing = Commands->Get_Facing(MiniGamePlayerControlSystem[Get_Int_Parameter("PlayerNumber")].PlayerShip);
			TargetPos = Commands->Get_Position(MiniGamePlayerControlSystem[Get_Int_Parameter("PlayerNumber")].PlayerShip);
			if (MyState == DroneReturning && JmgUtility::SimpleDistance(Commands->Get_Position(obj),TargetPos) < 400.0f)
				MyState = DroneGuarding;
		}
		else
		{
			if (!SearchTimeDelay)
			{
				SearchTimeDelay = Commands->Get_Random_Int(5,15);
				Distance = 100.0f;
				TargetPos = Commands->Get_Position(obj);
				Rotation = JmgUtility::MathClampDegrees(Commands->Get_Facing(obj)-Commands->Get_Random(-45.0f,45.0f))*3.14159265f/180;
				TargetPos.X += Distance*cos(Rotation);
				TargetPos.Y += Distance*sin(Rotation);
				params.Set_Basic(this,100,1);
				params.Set_Movement(TargetPos,JmgUtility::MathClamp(Commands->Get_Distance(Commands->Get_Position(obj),TargetPos)/10.0f,1.0,5),0.0f,false);
				params.ShutdownEngineOnArrival = true;
				Commands->Action_Goto(obj,params);
			}
			return;
		}
		Distance = 10.0f;//Commands->Get_Random(15.0f,20.0f);
		Rotation = Facing-(180+FollowAngle);
		TargetPos.X += Distance*(cos(JmgUtility::MathClampDegrees(Rotation)*3.14159265f/180));
		TargetPos.Y += Distance*(sin(JmgUtility::MathClampDegrees(Rotation)*3.14159265f/180));
		params.Set_Basic(this,100,1);
		params.Set_Movement(TargetPos,JmgUtility::MathClamp(Commands->Get_Distance(Commands->Get_Position(obj),TargetPos)/10.0f,1.0,5),0.0f,false);
		params.ShutdownEngineOnArrival = true;
		Commands->Action_Goto(obj,params);
		break;
	case DroneAvoiding:
		AvoidObject = Commands->Find_Object(AvoidObjectID);
		if (AvoidObject)
			Pos = Commands->Get_Position(AvoidObject);
		else
		{
			AvoidObjectID = 0;
			Drone_Action_Complete(obj);
			AI_Update(obj);
			return;
		}
		Commands->Action_Reset(obj,100);
		MyPos.X -= Pos.X;
		MyPos.Y -= Pos.Y;
		TargetDirection = JmgUtility::MathClampDegrees(atan2(MyPos.X,MyPos.Y));
		TargetDirection = JmgUtility::MathClampDegrees(TargetDirection+AvoidDegree);
		TargetPos = Commands->Get_Position(obj);
		TargetPos.X += AvoidDistance*(cos(TargetDirection*3.14159265f/180));
		TargetPos.Y += AvoidDistance*(sin(TargetDirection*3.14159265f/180));
		params.Set_Basic(this,100,1);
		params.Set_Movement(TargetPos,1.0f,2.0f,false);
		Commands->Action_Goto(obj,params);
		break;
	case DroneAttacking:
		Drone_Attack(obj);
		break;
	}
}
void JMG_CMTB_Player_Drone::Drone_Attack(GameObject *obj)
{
	GameObject *Target = Commands->Find_Object(TargetID);
	if (Target)
	{
		if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(Target)) > 2500.0f)
		{
			TargetID = 0;
			SearchTimeDelay = 0;
			MyState = DroneGuarding;
		}

		if (LastTargetID == TargetID && LastPrimaryFire == PrimaryFire)
			return;
		ActionParamsStruct params;
		params.Set_Basic(this,100,10);
		params.Set_Movement(Target,1.0f,TargetSize+5.0f,false);
		params.MovePathfind=false;
		params.ShutdownEngineOnArrival = true;
		Commands->Action_Goto(obj,params);
		params.Set_Basic(this,100,1);
		if (PrimaryFire)
			params.Set_Attack(Target,50.0f,0,true);
		else
			params.Set_Attack(Target,75.0f,0,false);
		Commands->Action_Attack(obj,params);
		LastTargetID = TargetID;
		LastPrimaryFire = PrimaryFire;
	}
	else
	{
		TargetID = 0;
		SearchTimeDelay = 0;
		MyState = DroneGuarding;
	}
}
void JMG_CMTB_Player_Drone::Position_At_Player(GameObject *obj)
{
	if (!Get_Int_Parameter("PlayerEscort"))
		return;
	GameObject *PlayerShip = MiniGamePlayerControlSystem[Get_Int_Parameter("PlayerNumber")].PlayerShip;
	if (PlayerShip)
	{
		Vector3 CreateSpot = Commands->Get_Position(PlayerShip);
		float Rotation = Commands->Get_Facing(PlayerShip)-(180+FollowAngle);
		CreateSpot.X += 15.0f*(cos(JmgUtility::MathClampDegrees(Rotation)*3.14159265f/180));
		CreateSpot.Y += 15.0f*(sin(JmgUtility::MathClampDegrees(Rotation)*3.14159265f/180));
		Commands->Set_Position(obj,CreateSpot);
	}
}
void JMG_CMTB_Player_Drone::ShieldOff(GameObject *obj)
{
	if (!MyObject || ShieldOverride)
		return;
	MyObject->Size = 0.105625f; 
	MyObject->Type = PlayerDrone;
	Set_Skin(obj,"None");
	Commands->Set_Animation(obj,myAnimation,false,0,0,0,false);
}
void JMG_CMTB_Player_Drone::ShieldOn(GameObject *obj)
{
	if (!MyObject)
		return;
	MyObject->Type = PlayerDroneShield;
	MyObject->Size = 0.775f;
	Power -= Commands->Get_Random_Int(0,3);
	if (Power < 0)
		Power = 0;
	Set_Skin(obj,"Blamo");
	Commands->Set_Animation(obj,myAnimation,false,0,2,2,false);
}
void JMG_CMTB_Vehicle_Face_Turret::Created(GameObject *obj)
{	
	GameObject *DavesArrow = Commands->Create_Object("Daves Arrow",Commands->Get_Position(obj));
	Commands->Attach_To_Object_Bone(DavesArrow,obj,"Turret");
	Commands->Set_Model(DavesArrow,"null");
	davesArrowId = Commands->Get_ID(DavesArrow);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_CMTB_Vehicle_Face_Turret::Timer_Expired(GameObject *obj, int number)
{	
	if (number == 1)
	{
		GameObject *davesArrow = Commands->Find_Object(davesArrowId);
		if (davesArrow)
			Commands->Set_Facing(obj,Commands->Get_Facing(davesArrow));
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_CMTB_Vehicle_Face_Turret::Destroyed(GameObject *obj)
{
	GameObject *davesArrow = Commands->Find_Object(davesArrowId);
	if (davesArrow)
		Commands->Destroy_Object(davesArrow);
}
void JMG_CMTB_Poke_End_Map::Created(GameObject *obj)
{
	Commands->Set_Animation(obj,"powerbox.powerbox",false,0,0.0,0.0,false);
	startDelay = 25;
	time = 0;
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_CMTB_Poke_End_Map::Poked(GameObject *obj,GameObject *poker)
{
	if (!JMG_CMTB_Main_Game_Control::EndGameSwitchEnabled)
	{
		JmgUtility::DisplayChatMessage(poker,0,255,0,"Sorry, this switch is not enabled.");
		return;
	}
	if (JMG_CMTB_Main_Game_Control::GameInProgress)
	{
		JmgUtility::DisplayChatMessage(poker,0,255,0,"Please wait for the current game to end before pulling the switch.");
		return;
	}
	if (startDelay)
	{
		char startDelayMsg[220];
		sprintf(startDelayMsg,"Please wait %d seconds before pulling the switch.",startDelay);
		JmgUtility::DisplayChatMessage(poker,0,255,0,startDelayMsg);
		return;
	}
	if (time)
	{
		FlipSwitch(obj,0.0f);
		Console_Input("win 0");
		return;
	}
	time = 5;
	JmgUtility::DisplayChatMessage(poker,0,255,0,"Pull the switch all the way down to proceed to the next map.");
	FlipSwitch(obj,1.0f);
}
void JMG_CMTB_Poke_End_Map::Timer_Expired(GameObject *obj, int number)
{	
	if (number == 1)
	{
		if (startDelay)
		{
			startDelay--;
			if (!startDelay)
				FlipSwitch(obj,2.0f);
		}
		if (time)
		{
			time--;
			if (!time)
				FlipSwitch(obj,2.0f);
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_CMTB_Poke_End_Map::FlipSwitch(GameObject *obj,float frame)
{
	Vector3 soundPos = Commands->Get_Position(obj);
	soundPos.Z += 1.25f;
	Commands->Create_Sound("SFX.Comet_Busters_Flip_Switch",soundPos,obj);
	Commands->Set_Animation(obj,"powerbox.powerbox",false,0,frame,frame,false);
}
void JMG_CMTB_Poke_Change_Difficulty::Created(GameObject *obj)
{
	difficulty = 1;
	JMG_CMTB_Main_Game_Control::DifficultyMultiplier = 1.0f;
	JMG_CMTB_Main_Game_Control::DifficultyMultiplierMine = 1.0f;
	JMG_CMTB_Main_Game_Control::DifficultyMultiplierInv = 1.0f;
	Commands->Set_Animation(obj,"powerbox2.powerbox2",false,0,1.0,1.0,false);
}
void JMG_CMTB_Poke_Change_Difficulty::Poked(GameObject *obj,GameObject *poker)
{
	if (JMG_CMTB_Main_Game_Control::GameInProgress)
	{
		JmgUtility::DisplayChatMessage(poker,0,255,0,"Please wait for the current game to end before pulling the switch.");
		return;
	}
	Vector3 soundPos = Commands->Get_Position(obj);
	soundPos.Z += 1.25f;
	Commands->Create_Sound("SFX.Comet_Busters_Flip_Switch",soundPos,obj);
	difficulty++;
	if (difficulty >= 4)
		difficulty = 0;
	RenCometBustersScoreControl.SaveData();
	RenCometBustersScoreControl.Cleanup();
	switch (difficulty)
	{
	case 0:
		Commands->Set_Animation(obj,"powerbox2.powerbox2",false,0,0.0f,0.0f,false);
		JMG_CMTB_Main_Game_Control::DifficultyMultiplier = 0.5f;
		JMG_CMTB_Main_Game_Control::DifficultyMultiplierMine = 0.75f;
		JMG_CMTB_Main_Game_Control::DifficultyMultiplierInv = 2.0f;
		RenCometBustersScoreControl.LoadData(0);
		JmgUtility::MessageAllPlayers(6,145,148,"[DIFFICULTY SET TO EASY]");
		break;
	case 1:
		Commands->Set_Animation(obj,"powerbox2.powerbox2",false,0,1.0f,1.0f,false);
		JMG_CMTB_Main_Game_Control::DifficultyMultiplier = 1.0f;
		JMG_CMTB_Main_Game_Control::DifficultyMultiplierMine = 1.0f;
		JMG_CMTB_Main_Game_Control::DifficultyMultiplierInv = 1.0f;
		RenCometBustersScoreControl.LoadData(1);
		JmgUtility::MessageAllPlayers(6,145,148,"DIFFICULTY SET TO NORMAL]");
		break;
	case 2:
		Commands->Set_Animation(obj,"powerbox2.powerbox2",false,0,2.0f,2.0f,false);
		JMG_CMTB_Main_Game_Control::DifficultyMultiplier = 2.0f;
		JMG_CMTB_Main_Game_Control::DifficultyMultiplierMine = 1.5f;
		JMG_CMTB_Main_Game_Control::DifficultyMultiplierInv = 0.75f;
		RenCometBustersScoreControl.LoadData(2);
		JmgUtility::MessageAllPlayers(6,145,148,"[DIFFICULTY SET TO HARD]");
		break;
	case 3:
		Commands->Set_Animation(obj,"powerbox2.powerbox2",false,0,3.0f,3.0f,false);
		JMG_CMTB_Main_Game_Control::DifficultyMultiplier = 3.0f;
		JMG_CMTB_Main_Game_Control::DifficultyMultiplierMine = 2.0f;
		JMG_CMTB_Main_Game_Control::DifficultyMultiplierInv = 0.5f;
		RenCometBustersScoreControl.LoadData(3);
		JmgUtility::MessageAllPlayers(6,145,148,"[DIFFICULTY SET TO DOOMSDAY]");
		break;
	}
}
void JMG_CMTB_Cargo_Ship_Child_Script::Killed(GameObject *obj, GameObject *damager)
{
	GameObject *parent = Commands->Find_Object(Get_Int_Parameter("ParentId"));
	if (parent)
		Commands->Apply_Damage(parent,99999.0,"None",0);
}
ScriptRegistrant<JMG_Create_Ship_On_Poke> JMG_Create_Ship_On_Poke_Registrant("JMG_Create_Ship_On_Poke","CreateSpot:vector3,ShipPreset=Comet_Busters_Ship_Blue:string,SpawnSpotExplosion=Explosion_Clear_Spawn_Point:string,InvisibleObject=Invisible_Spectator_Box:string");
ScriptRegistrant<JMG_Advanced_Bounce_Zone> JMG_Advanced_Bounce_Zone_Registrant("JMG_Advanced_Bounce_Zone","XAmount=0:float,YAmount=0:float,ZAmount=0:float");
ScriptRegistrant<JMG_Ship_Random_Hyperspace> JMG_Ship_Random_Hyperspace_Registrant("JMG_Ship_Random_Hyperspace","HyperspacingModel=cometbshs:string,HyperSpaceSound=Ship_Teleport:string");
ScriptRegistrant<JMG_Random_Music> JMG_Random_Music_Registrant("JMG_Random_Music","Song1:string,Song1Time:float,Song2:string,Song2Time:float,Song3:string,Song3Time:float");
ScriptRegistrant<JMG_Ship_Weapon> JMG_Ship_Weapon_Registrant("JMG_Ship_Weapon","");
ScriptRegistrant<JMG_Return_Custom_When_Shot> JMG_Return_Custom_When_Shot_Registrant("JMG_Return_Custom_When_Shot","");
ScriptRegistrant<JMG_Cronie_AI> JMG_Cronie_AI_Registrant("JMG_Cronie_AI","");
ScriptRegistrant<JMG_Kill_Self_And_Sender_On_Custom> JMG_Kill_Self_And_Sender_On_Custom_Registrant("JMG_Kill_Self_And_Sender_On_Custom","Message:int");
ScriptRegistrant<JMG_Ship_Shield_Control_Script> JMG_Ship_Shield_Control_Script_Registrant("JMG_Ship_Shield_Control_Script","");
ScriptRegistrant<JMG_Kill_Self_And_Sender_On_Custom_Ship> JMG_Kill_Self_And_Sender_On_Custom_Ship_Registrant("JMG_Kill_Self_And_Sender_On_Custom_Ship","Message:int");
ScriptRegistrant<JMG_Ship_Shield_Script> JMG_Ship_Shield_Script_Registrant("JMG_Ship_Shield_Script","ID:int");
ScriptRegistrant<JMG_Renetbusters_Object> JMG_Renetbusters_Object_Registrant("JMG_Renetbusters_Object","size:float,Power_Drain:int");
ScriptRegistrant<JMG_New_Ship_Weapon_Control_System> JMG_New_Ship_Weapon_Control_System_Registrant("JMG_New_Ship_Weapon_Control_System","");
ScriptRegistrant<JMG_Comet_Control_Script> JMG_Comet_Control_Script_Registrant("JMG_Comet_Control_Script","Comet_Size:float,Spawn_Comets=0:int,Comet_Holder=null:string,Smaller_Comet=null:string");
ScriptRegistrant<JMG_Attach_And_Remove> JMG_Attach_And_Remove_Registrant("JMG_Attach_And_Remove","Smaller_Comet:string");
ScriptRegistrant<JMG_User_Auto_Enter_Ship> JMG_User_Auto_Enter_Ship_Registrant("JMG_User_Auto_Enter_Ship","Player_Ship=RB_Blue_Player_Ship:string");
ScriptRegistrant<JMG_Apply_Damage_On_Timer> JMG_Apply_Damage_On_Timer_Registrant("JMG_Apply_Damage_On_Timer","");

ScriptRegistrant<JMG_CMTB_Main_Game_Control> JMG_CMTB_Main_Game_Control_Registrant("JMG_CMTB_Main_Game_Control","");
ScriptRegistrant<JMG_CMTB_Ship_Control_Script> JMG_CMTB_Ship_Control_Script_Registrant("JMG_CMTB_Ship_Control_Script","");
ScriptRegistrant<JMG_CMTB_UFO_Random_Fire_Control> JMG_CMTB_UFO_Random_Fire_Control_Registrant("JMG_CMTB_UFO_Random_Fire_Control","UFOBoss:int");
ScriptRegistrant<JMG_CMTB_Comet_Script> JMG_CMTB_Comet_Script_Registrant("JMG_CMTB_Comet_Script","Size=2:int,TargetPosition=0.0 0.0 0.0:Vector3");
ScriptRegistrant<JMG_CMTB_Zone_Realign_Enter> JMG_CMTB_Zone_Realign_Enter_Registrant("JMG_CMTB_Zone_Realign_Enter","Position=0.0 0.0 0.0:Vector3");
ScriptRegistrant<JMG_CMTB_Spectator_Object> JMG_CMTB_Spectator_Object_Registrant("JMG_CMTB_Spectator_Object","ControllerID:int,Number:int");
ScriptRegistrant<JMG_CMTB_Arcade_Machine> JMG_CMTB_Arcade_Machine_Registrant("JMG_CMTB_Arcade_Machine","ControllerID=0:int,MachineNumber=0:int");
ScriptRegistrant<JMG_CMTB_Fake_Player_Attach_Script> JMG_CMTB_Fake_Player_Attach_Script_Registrant("JMG_CMTB_Fake_Player_Attach_Script","MySoldierID:int,Number:int");
ScriptRegistrant<JMG_CMTB_Renegade_Code> JMG_CMTB_Renegade_Code_Registrant("JMG_CMTB_Renegade_Code","");
ScriptRegistrant<JMG_CMTB_Motion_Mine> JMG_CMTB_Motion_Mine_Registrant("JMG_CMTB_Motion_Mine","Is_BOSS:int");
ScriptRegistrant<JMG_CMTB_The_Moon_Script> JMG_CMTB_The_Moon_Script_Registrant("JMG_CMTB_The_Moon_Script","");
ScriptRegistrant<JMG_CMTB_Ship_Super_Weapon> JMG_CMTB_Ship_Super_Weapon_Registrant("JMG_CMTB_Ship_Super_Weapon","ControllerShipID:int");
ScriptRegistrant<JMG_CMTB_Powerup_Script> JMG_CMTB_Powerup_Script_Registrant("JMG_CMTB_Powerup_Script","Custom:int,LifeTime:int");
ScriptRegistrant<JMG_CMTB_Cargo_Ship_Script> JMG_CMTB_Cargo_Ship_Script_Registrant("JMG_CMTB_Cargo_Ship_Script","PlayerNumber:int");
ScriptRegistrant<JMG_CMTB_Cargo_Ship_Turret> JMG_CMTB_Cargo_Ship_Turret_Registrant("JMG_CMTB_Cargo_Ship_Turret","");
ScriptRegistrant<JMG_CMTB_Player_Drone> JMG_CMTB_Player_Drone_Registrant("JMG_CMTB_Player_Drone","PlayerNumber:int,Power:int,RePosition:int,PlayerEscort:int");
ScriptRegistrant<JMG_CMTB_Vehicle_Face_Turret> JMG_CMTB_Vehicle_Face_Turret_Registrant("JMG_CMTB_Vehicle_Face_Turret","");
ScriptRegistrant<JMG_CMTB_Poke_End_Map> JMG_CMTB_Poke_End_Map_Registrant("JMG_CMTB_Poke_End_Map","");
ScriptRegistrant<JMG_CMTB_Poke_Change_Difficulty> JMG_CMTB_Poke_Change_Difficulty_Registrant("JMG_CMTB_Poke_Change_Difficulty","");
ScriptRegistrant<JMG_CMTB_Cargo_Ship_Child_Script> JMG_CMTB_Cargo_Ship_Child_Script_Registrant("JMG_CMTB_Cargo_Ship_Child_Script","ParentId:int");

