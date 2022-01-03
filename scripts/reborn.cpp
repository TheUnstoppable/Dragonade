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
#include "reborn.h"
#include "VehicleGameObj.h"
#include "MoveablePhysClass.h"
#include "PhysColTest.h"
#include "PhysicsSceneClass.h"

void Reborn_Game_Manager::Created(GameObject *obj)
{
	MMK2lim = 0;
	Cyborglim = 0;
}

void Reborn_Game_Manager::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&MMK2lim,4,1);
	Auto_Save_Variable(&Cyborglim,4,1);
}

void Reborn_Game_Manager::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	GameObject *send;
	int msg;
	int prm;
	msg = 3054;
	prm = 0;
	send = sender;
	//1015 is inc MMK2,1025 Cyborg inc,1016 is dec MMK2,1026 is dec Cyborg
	if (type == 1015)
	{
		MMK2lim++;
		if (MMK2lim > 1)
		{
			Commands->Send_Custom_Event(obj,sender,msg,prm,0);
		}
		else
			Commands->Create_2D_Sound("MammothMk2Create");
	}
	if (type == 1016)
	{
		MMK2lim--;
		Commands->Create_2D_Sound("MammothMk2Death");
	}
	if (type == 1025)
	{
		Cyborglim++;
		if (Cyborglim > 1)
			Commands->Send_Custom_Event(obj,sender,msg,prm,0);
		else
			Commands->Create_2D_Sound("CommandoCreate");
	}
	if (type == 1026)
	{
		Cyborglim--;
		Commands->Create_2D_Sound("CommandoDeath");
	}
}

void Reborn_MMK2::Created(GameObject *obj)
{
	GameObject *send;
	int msg;
	int prm;
	msg = 1015;
	prm = 0;
	send = Commands->Find_Object(Get_Int_Parameter("GameManager_ID"));
	Commands->Send_Custom_Event(obj,send,msg,prm,0);
}

void Reborn_MMK2::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *send;
	int msg;
	int prm;
	msg = 1016;
	prm = 0;
	send = Commands->Find_Object(Get_Int_Parameter("GameManager_ID"));
	Commands->Send_Custom_Event(obj,send,msg,prm,0);
}

void Reborn_MMK2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	Vector3 v;
	v = Commands->Get_Position(obj);
	if (type == 3054)
	{
		Commands->Give_Money(obj,Get_Float_Parameter("MoneyToReturn"),false);
		Commands->Create_Explosion(Get_Parameter("Explosion_Preset_Name"),v,0);
	}
}

void Reborn_Cyborg::Created(GameObject *obj)
{
	GameObject *send;
	int msg;
	int prm;
	msg = 1025;
	prm = 0;
	send = Commands->Find_Object(Get_Int_Parameter("GameManager_ID"));
	Commands->Send_Custom_Event(obj,send,msg,prm,0);
}

void Reborn_Cyborg::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	Vector3 v;
	v = Commands->Get_Position(obj);
	if (type == 3054)
	{
		Commands->Give_Money(obj,Get_Float_Parameter("MoneyToReturn"),false);
		Commands->Create_Explosion(Get_Parameter("Explosion_Preset_Name"),v,0);
	}
}

void Reborn_Cyborg::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *send;
	int msg;
	int prm;
	msg = 1026;
	prm = 0;
	send = Commands->Find_Object(Get_Int_Parameter("GameManager_ID"));
	Commands->Send_Custom_Event(obj,send,msg,prm,0);
}

//Reborn Deployable Vehicle
void Reborn_Deployable_Vehicle::Created(GameObject *obj)
{
	pilotID = 0;
}

void Reborn_Deployable_Vehicle::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!pilotID)
		{
			pilotID = Commands->Get_ID(sender);
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (pilotID == Commands->Get_ID(sender))
		{
			pilotID = 0;
		}
	}
}

void Reborn_Deployable_Vehicle::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	const char *preset;
	GameObject *npre;
	Vector3 pos;
	float health;
	float facing;
	if ((damager == Commands->Find_Object(pilotID)) && (damager))
	{
		preset = Get_Parameter("Animated_Preset");
		pos = Commands->Get_Position(obj);
		health = Commands->Get_Health(obj);
		npre = Commands->Create_Object(preset,pos);
		Commands->Set_Health(npre,health);
		health = Commands->Get_Shield_Strength(obj);
		Commands->Set_Shield_Strength(npre,health);
		facing = Commands->Get_Facing(obj);
		Commands->Set_Facing(npre,facing);
 		preset = Get_Parameter("Explosion_Preset");
		Commands->Create_Explosion(preset,pos,0);
	}
}

void Reborn_Deployable_Vehicle::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&pilotID,4,1);
}

//Reborn Deployable Vehicle Animation
void Reborn_Deploy_Animation::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time_Till_Death") ,1);
}

void Reborn_Deploy_Animation::Timer_Expired(GameObject *obj,int number)
{
	const char *preset;
	GameObject *npre;
	Vector3 pos;
	float health;
	float facing;
	preset = Get_Parameter("Deployed_Tank_Preset");
	pos = Commands->Get_Position(obj);
	health = Commands->Get_Health(obj);
	npre = Commands->Create_Object(preset,pos);
	Commands->Set_Health(npre,health);
	health = Commands->Get_Shield_Strength(obj);
	Commands->Set_Shield_Strength(npre,health);
	facing = Commands->Get_Facing(obj);
	Commands->Set_Facing(npre,facing);
 	preset = Get_Parameter("Explosion_Preset");
	Commands->Create_Explosion(preset,pos,0);
}

//Reborn MMK2 Turret Script
void Reborn_MMK2_Turret_Spawn::Created(GameObject *obj)
{
	Vector3 sp;
	GameObject *object;
	sp.X = 0;
	sp.Y = 0;
	sp.Z = 0;
	object = Commands->Create_Object(Get_Parameter("Turret_Preset"),sp);
	Commands->Attach_To_Object_Bone(object,obj,Get_Parameter("Bone_Name"));
	turID = Commands->Get_ID(object);
}

void Reborn_MMK2_Turret_Spawn::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&turID,4,1);
}

void Reborn_MMK2_Turret_Spawn::Killed(GameObject *obj,GameObject *killer)
{
	Vector3 tmb;
	tmb = Commands->Get_Bone_Position(obj,Get_Parameter("Bone_Name"));
	Commands->Create_Explosion(Get_Parameter("Explosion_Preset"),tmb,0);
}

//Reborn Diggable Vehicle
void Reborn_Diggable_Vehicle::Created(GameObject *obj)
{
	pilotID = 0;
	ug = 0;
}

void Reborn_Diggable_Vehicle::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!pilotID)
		{
			pilotID = Commands->Get_ID(sender);
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (pilotID == Commands->Get_ID(sender))
		{
			pilotID = 0;
		}
	}
}

void Reborn_Diggable_Vehicle::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	Vector3 vpos;
	if ((damager != Commands->Find_Object(pilotID)) || (!damager))
		return;
	vpos = Commands->Get_Position(obj);
	if (!ug)
	{
		vpos.Z = vpos.Z - Get_Float_Parameter("Z_Hieght_Adjust");
		ug = 1;
	}
	else
	{
		vpos.Z = vpos.Z + Get_Float_Parameter("Z_Hieght_Adjust");
		ug = 0;
	}
	Commands->Set_Position(obj,vpos);
}

void Reborn_Diggable_Vehicle::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&pilotID,4,1);
	Auto_Save_Variable(&ug,4,1);
}

void Reborn_PlaySound_OnCreate::Created(GameObject *obj)
{
	Commands->Create_2D_Sound(Get_Parameter("Sound_Preset_Name"));
	Destroy_Script();
}

//Reborn Diggable Vehicle Animated
void Reborn_Diggable_Vehicle_Animated::Created(GameObject *obj)
{
	pilotID = 0;
	ug = 0;
}

void Reborn_Diggable_Vehicle_Animated::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!pilotID)
		{
			pilotID = Commands->Get_ID(sender);
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (pilotID == Commands->Get_ID(sender))
		{
			pilotID = 0;
		}
	}
}

void Reborn_Diggable_Vehicle_Animated::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if ((damager != Commands->Find_Object(pilotID)) || (!damager))
		return;
	ActionParamsStruct aps;
	if (!ug)
		aps.Set_Animation(Get_Parameter("Dig_Animation"),false);
	if (ug == 1)
		aps.Set_Animation(Get_Parameter("Surface_Animation"),false);
	Commands->Action_Play_Animation(0,aps);
}

void Reborn_Diggable_Vehicle_Animated::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	Vector3 vpos;
	vpos = Commands->Get_Position(obj);
	if (!ug)
	{
		vpos.Z = vpos.Z - Get_Float_Parameter("Z_Hieght_Adjust");
		ug = 1;
	}
	else
	{
		vpos.Z = vpos.Z + Get_Float_Parameter("Z_Hieght_Adjust");
		ug = 0;
	}
	Commands->Set_Position(obj,vpos);
}

void Reborn_Diggable_Vehicle_Animated::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&pilotID,4,1);
	Auto_Save_Variable(&ug,4,2);
}

void Reborn_Deployable_Vehicle_Player::Created(GameObject *obj)
{
	InstallHook(Get_Parameter("Key"),obj);
}

void Reborn_Deployable_Vehicle_Player::KeyHook()
{
	Commands->Send_Custom_Event(Owner(),Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message"),0,0);
}

void Reborn_IsDeployableMech::Created(GameObject * obj)
{
	//mode: 0=walk;1=deploy;2=deployed;3=redeploy
	mode = 0;
	PilotID = 0;
	Vector3 pos = Commands->Get_Position(obj);
	xpos = pos.X;
	ypos = pos.Y;
	zpos = pos.Z;
	enabled = false;
	play = false;
	Mode = 1;
	Commands->Start_Timer(obj,this,(float)0.1,1);
	Commands->Clear_Weapons(obj);
}

void Reborn_IsDeployableMech::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!PilotID)
		{
			PilotID = Commands->Get_ID(sender);
			char params[50];
			sprintf(params,"Deploy,%d,923572385",Commands->Get_ID(obj));
			Commands->Attach_Script(sender,"Reborn_Deployable_Vehicle_Player",params);
		}
	}
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (PilotID == Commands->Get_ID(sender))
		{
			PilotID = 0;
			Remove_Script(sender,"Reborn_Deployable_Vehicle_Player");
		}
	}
	else if (type == 923572385)
	{
		if (!obj->As_VehicleGameObj()->Can_Drive())
		{
			return;
		}
		const char *model;
		model = Get_Parameter("Model_Name");
		char text[512];
		sprintf(text,"%s_d",model);
		//deploy!
		if (!mode)
		{
			if (!obj->As_VehicleGameObj()->Is_Immovable())
			{
				obj->As_VehicleGameObj()->Set_Immovable(true);
				Mode = 2;
				//mode: 0=walk;1=deploy;2=deployed;3=redeploy
				mode = 1;
				Commands->Set_Model(obj,text);
				sprintf(text,"%s_d.%s_d",model,model);
				obj->As_PhysicalGameObj()->Clear_Animation();
				Commands->Set_Animation(obj,text,false,0,0,-1,0);
				Commands->Control_Enable(Commands->Find_Object(PilotID),false);
				obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(false);
				Commands->Create_Sound(Get_Parameter("DeploySound"),Commands->Get_Position(obj),obj);
			}
		}
		//redeploy!
		else if (mode == 2)
		{
			//mode: 0=walk;1=deploy;2=deployed;3=redeploy
			mode = 3;
			Commands->Clear_Weapons(obj);
			sprintf(text,"%s_d",model);
			Commands->Set_Model(obj,text);
			sprintf(text,"%s_d.%s_d",model,model);
			obj->As_PhysicalGameObj()->Clear_Animation();
			Commands->Set_Animation(obj,text,false,0,Get_Float_Parameter("Last_Deploy_Frame"),0,0);
			Commands->Control_Enable(Commands->Find_Object(PilotID),false);
			obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(false);
			Commands->Create_Sound(Get_Parameter("UndeploySound"),Commands->Get_Position(obj),obj);
		}
	}
}

void Reborn_IsDeployableMech::Animation_Complete(GameObject * obj,const char *animation_name)
{
	const char *model;
	model = Get_Parameter("Model_Name");
	char text[512];
	sprintf(text,"%s_d.%s_d",model,model);
	//mech deployed -> set deployed model
	if (!_stricmp(animation_name,text) && (mode == 1))
	{
		//mode: 0=walk;1=deploy;2=deployed;3=redeploy 
		mode = 2;
		sprintf(text,"%s_dd",model);
		obj->As_PhysicalGameObj()->Clear_Animation();
		Commands->Set_Model(obj,text);
		Commands->Control_Enable(Commands->Find_Object(PilotID),true);
		obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(true);
		Commands->Enable_Engine(obj,false);
		Commands->Enable_Hibernation(obj,true);
		Commands->Clear_Weapons(obj);
		Commands->Give_PowerUp(obj,Get_Parameter("Weapon_Powerup_Name"),false);
		Commands->Select_Weapon(obj,Get_Parameter("Weapon_Name"));
		return;
	}
	//mech redeployed -> set standard model
	sprintf(text,"%s_d.%s_d",model,model);
	if (!_stricmp(animation_name,text) && (mode == 3))
	{
		//mode: 0=walk;1=deploy;2=deployed;3=redeploy
		mode = 0;
		obj->As_VehicleGameObj()->Set_Immovable(false);
		Commands->Set_Model(obj,model);
		Commands->Enable_Engine(obj,true);
		Commands->Control_Enable(Commands->Find_Object(PilotID),true);
		obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(true);
		Commands->Enable_Hibernation(obj,false);
		Commands->Start_Timer(obj,this,(float)0.1,1);
		Mode = 1;
		xpos = 0;
		ypos = 0;
		zpos = 0;
		enabled = false;
		play = false;
		return;
	}
	sprintf(text,"%s.%s_m",model,model);
	if (!_stricmp(animation_name,text))
	{
		if (LastDir == 3)
		{
			char moveanim[512];
			sprintf(moveanim,"%s.%s_m",Get_Model(obj),Get_Model(obj));
			obj->As_PhysicalGameObj()->Clear_Animation();
			Commands->Set_Animation(obj,moveanim,false,0,0,-1,0);
			LastDir = 1;
		}
		else if (LastDir == 4) 
		{
			char moveanim[512];
			sprintf(moveanim,"%s.%s_b",Get_Model(obj),Get_Model(obj));
			obj->As_PhysicalGameObj()->Clear_Animation();
			Commands->Set_Animation(obj,moveanim,false,0,0,-1,0);
			LastDir = 2;
		}
		else 
		{
			enabled = false;
		}
	}
	sprintf(text,"%s.%s_b",model,model);
	if (!_stricmp(animation_name,text))
	{
		if (LastDir == 3)
		{
			char moveanim[512];
			sprintf(moveanim,"%s.%s_m",Get_Model(obj),Get_Model(obj));
			obj->As_PhysicalGameObj()->Clear_Animation();
			Commands->Set_Animation(obj,moveanim,false,0,0,-1,0);
			LastDir = 1;
		}
		else if (LastDir == 4) 
		{
			char moveanim[512];
			sprintf(moveanim,"%s.%s_b",Get_Model(obj),Get_Model(obj));
			obj->As_PhysicalGameObj()->Clear_Animation();
			Commands->Set_Animation(obj,moveanim,false,0,0,-1,0);
			LastDir = 2;
		}
		else 
		{
			enabled = false;
		}
	}
}

void Reborn_IsDeployableMech::Timer_Expired(GameObject *obj,int number)
{
	//mode: 0=walk;1=deploy;2=deployed;3=redeploy
	const char *model;
	model = Get_Parameter("Model_Name");
	char anim[512];
	sprintf(anim,"%s.%s_m",model,model);
	//reset soundflag (play)
	if (number == 2)
	{
		play = false;
	}
	//check for moveanim
	else if ((number == 1) && !mode)
	{
		Commands->Start_Timer(obj,this,(float)0.1,1);
		char moveanim[512];
		sprintf(moveanim,"%s.%s_m",Get_Model(obj),Get_Model(obj));
		char movebanim[512];
		sprintf(movebanim,"%s.%s_b",Get_Model(obj),Get_Model(obj));
		Vector3 position = Commands->Get_Bone_Position(obj,"Origin");
		Vector3 pos;
		if (Mode == 1) 
		{
			xpos = position.X;
			ypos = position.Y;
			zpos = position.Z;
			Mode = 2;
		}
		pos.X = xpos;
		pos.Y = ypos;
		pos.Z = zpos;
		float dist = Commands->Get_Distance(pos,position);
		//moved!
		if (dist > 0.1) 
		{
			float xdist = position.X - xpos;
			float ydist = position.Y - ypos;
			float rotation = Commands->Get_Facing(obj);
			//rot near 0
			if (rotation > -45 && rotation < 45)
			{
				if (xdist > ydist)
				{
					forward = true;
					backward = false;
				}
				else if (ydist > xdist)
				{
					backward = true;
					forward = false;
				}
			}
			//rot near -90
			if (rotation < -45 && rotation > -135)
			{
				if (xdist > ydist)
				{
					forward = true;
					backward = false;
				}
				else if (ydist > xdist)
				{
					backward = true;
					forward = false;
				}
			}
			//rot near -180/180
			if (rotation < -135 || rotation > 135)
			{
				if (xdist < ydist)
				{
					forward = true;
					backward = false;
				}
				else if (ydist < xdist)
				{
					backward = true;
					forward = false;
				}
			}
			//rot near 90
			if (rotation < 135 && rotation > 45)
			{
				if (xdist < ydist)
				{
					forward = true;
					backward = false;
				}
				else if (ydist < xdist)
				{
					backward = true;
					forward = false;
				}
			}
			xpos = position.X;
			ypos = position.Y;
			zpos = position.Z;
			if (!enabled && forward)
			{
				enabled = true;
				if (Mode == 2)
				{
					obj->As_PhysicalGameObj()->Clear_Animation();
					Commands->Set_Animation(obj,moveanim,false,0,0,-1,0);
					LastDir = 1;
				}
				else if (LastDir == 2)
				{
					obj->As_PhysicalGameObj()->Clear_Animation();
					Commands->Set_Animation(obj,movebanim,false,0,Get_Animation_Frame(obj),0,0);
					LastDir = 3;
				}
				else
				{
					obj->As_PhysicalGameObj()->Clear_Animation();
					Commands->Set_Animation(obj,moveanim,false,0,Get_Animation_Frame(obj),-1,0);
					LastDir = 1;
				}
			}
			if (!enabled && backward)
			{
				enabled = true;
				if (Mode == 2)
				{
					obj->As_PhysicalGameObj()->Clear_Animation();
					Commands->Set_Animation(obj,movebanim,false,0,0,-1,0);
					LastDir = 2;
				}
				else if (LastDir == 1)
				{
					obj->As_PhysicalGameObj()->Clear_Animation();
					Commands->Set_Animation(obj,moveanim,false,0,Get_Animation_Frame(obj),0,0);
					LastDir = 4;
				}
				else
				{
					obj->As_PhysicalGameObj()->Clear_Animation();
					Commands->Set_Animation(obj,movebanim,false,0,Get_Animation_Frame(obj),-1,0);
					LastDir = 2;
				}
			}
		}
		if (enabled && forward && !play)
		{
			if (((Get_Animation_Frame(obj) > (Get_Int_Parameter("Stomp1Frame_Forward") - 2)) && (Get_Animation_Frame(obj) < (Get_Int_Parameter("Stomp1Frame_Forward") + 2))) || ((Get_Animation_Frame(obj) > (Get_Int_Parameter("Stomp2Frame_Forward") - 1)) && (Get_Animation_Frame(obj) < (Get_Int_Parameter("Stomp2Frame_Forward") + 2))))
			{
				play = true;
				Commands->Create_3D_WAV_Sound_At_Bone(Get_Parameter("StompWAVSound"),obj,"Origin");
				Commands->Start_Timer(obj,this,(float)0.5,2);
			}
		}
		if (enabled && backward && !play)
		{
			if (((Get_Animation_Frame(obj) > (Get_Int_Parameter("Stomp1Frame_Backward") - 2)) && (Get_Animation_Frame(obj) < (Get_Int_Parameter("Stomp1Frame_Backward") + 2))) || ((Get_Animation_Frame(obj) > (Get_Int_Parameter("Stomp2Frame_Backward") - 1)) && (Get_Animation_Frame(obj) < (Get_Int_Parameter("Stomp2Frame_Backward") + 2))))
			{
				play = true;
				Commands->Create_3D_WAV_Sound_At_Bone(Get_Parameter("StompWAVSound"),obj,"Origin");
				Commands->Start_Timer(obj,this,(float)0.5,2);
			}
		}
	}
}

void Reborn_IsDeployableMech::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
	Auto_Save_Variable(&xpos,4,2);
	Auto_Save_Variable(&ypos,4,3);
	Auto_Save_Variable(&zpos,4,4);
	Auto_Save_Variable(&PilotID,4,7);
	Auto_Save_Variable(&mode,4,8);
	Auto_Save_Variable(&play,1,9);
	Auto_Save_Variable(&backward,1,10);
	Auto_Save_Variable(&forward,1,11);
	Auto_Save_Variable(&Mode,4,12);
	Auto_Save_Variable(&LastDir,4,13);
}

void Reborn_IsDeployableMech::Killed(GameObject *obj,GameObject *killer)
{
	if (PilotID)
	{
		Remove_Script(Commands->Find_Object(PilotID),"Reborn_Deployable_Vehicle_Player");
		if (mode)
		{
			Commands->Control_Enable(obj,true);
			obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(true);
			Commands->Control_Enable(Commands->Find_Object(PilotID),true);
		}
	}
}

void Reborn_IsDeployableTank::Created(GameObject *obj)
{
	//mode: 0=walk;1=deploy;2=deployed;3=redeploy
	mode = 0;
	PilotID = 0;
	Commands->Clear_Weapons(obj);
}

void Reborn_IsDeployableTank::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!PilotID)
		{
			PilotID = Commands->Get_ID(sender);
			char params[50];
			sprintf(params,"Deploy,%d,923572385",Commands->Get_ID(obj));
			Commands->Attach_Script(sender,"Reborn_Deployable_Vehicle_Player",params);
		}
	}
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (PilotID == Commands->Get_ID(sender))
		{
			PilotID = 0;
			Remove_Script(sender,"Reborn_Deployable_Vehicle_Player");
		}
	}
	else if (type == 923572385)
	{
		if (!obj->As_VehicleGameObj()->Can_Drive())
		{
			return;
		}
		const char *model = Get_Parameter("Model_Name");
		char deployanim[512];
		char dmodel[512];
		sprintf(deployanim,"%s_d.%s_d",model,model);
		sprintf(dmodel,"%s_d",model);
		//mode: 0=walk;1=deploy;2=deployed;3=redeploy
		if (!mode)
		{
			if (!obj->As_VehicleGameObj()->Is_Immovable())
			{
				obj->As_VehicleGameObj()->Set_Immovable(true);
				mode = 1;
				Commands->Set_Model(obj,dmodel);
				Commands->Set_Animation(obj,deployanim,false,0,0,-1,0);
				Commands->Control_Enable(sender,false);
				obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(false);
				Commands->Create_Sound(Get_Parameter("DeploySound"),Commands->Get_Position(obj),obj);
				Commands->Enable_Vehicle_Transitions(obj,false);
			}
		}
		//mode: 0=walk;1=deploy;2=deployed;3=redeploy
		else if (mode == 2)
		{
			mode = 3;
			Commands->Clear_Weapons(obj);
			Commands->Set_Model(obj,dmodel);
			Commands->Set_Animation(obj,deployanim,false,0,Get_Float_Parameter("Last_Deploy_Frame"),0,0);
			Commands->Control_Enable(sender,false);
			obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(false);
			Commands->Create_Sound(Get_Parameter("UndeploySound"),Commands->Get_Position(obj),obj);
			Commands->Enable_Vehicle_Transitions(obj,false);
		}
	}
}

void Reborn_IsDeployableTank::Animation_Complete(GameObject *obj,const char *animation_name)
{
	const char *model;
	model = Get_Parameter("Model_Name");
	char deployedmodel[512];
	sprintf(deployedmodel,"%s_dd",model);
	char deployanim[512];
	sprintf(deployanim,"%s_d.%s_d",model,model);
	//tank deploy
	//mode: 0=walk;1=deploy;2=deployed;3=redeploy
	if (!_stricmp(animation_name,deployanim) && (mode == 1))
	{
		mode = 2;
		Commands->Set_Model(obj,deployedmodel);
		obj->As_PhysicalGameObj()->Clear_Animation();
		Commands->Control_Enable(Commands->Find_Object(PilotID),true);
		obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(true);
		Commands->Enable_Engine(obj,false);
		Commands->Enable_Vehicle_Transitions(obj,true);
		Commands->Enable_Innate_Conversations(obj,true);
		Commands->Clear_Weapons(obj);
		Commands->Give_PowerUp(obj,Get_Parameter("Weapon_Powerup_Name"),false);
		Commands->Select_Weapon(obj,Get_Parameter("Weapon_Name"));
		return;
	}
	//tank redeploy
	//mode: 0=walk;1=deploy;2=deployed;3=redeploy
	if (!_stricmp(animation_name,deployanim) && (mode == 3))
	{
		obj->As_VehicleGameObj()->Set_Immovable(false);
		mode = 0;
		Commands->Set_Model(obj,model);
		obj->As_PhysicalGameObj()->Clear_Animation();
		Commands->Control_Enable(Commands->Find_Object(PilotID),true);
		obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(true);
		Commands->Enable_Hibernation(obj,false);
		Commands->Enable_Innate_Conversations(obj,true);
		Commands->Enable_Vehicle_Transitions(obj,true);
		Commands->Enable_Engine(obj,true);
		return;
	}
}

void Reborn_IsDeployableTank::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&PilotID,4,6);
	Auto_Save_Variable(&mode,4,7);
}

void Reborn_IsDeployableTank::Destroyed(GameObject *obj)
{
	Remove_Script(Commands->Find_Object(PilotID),"Reborn_Deployable_Vehicle_Player");
	Commands->Control_Enable(Commands->Find_Object(PilotID),true);
}

void Reborn_IsDeployableTank_2::Created(GameObject *obj)
{
	//mode: 0=walk;1=deploy;2=deployed;3=redeploy
	mode = 0;
	PilotID = 0;
	//Commands->Start_Timer(obj,this,(float)0.1,1);
}

void Reborn_IsDeployableTank_2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!PilotID)
		{
			PilotID = Commands->Get_ID(sender);
			char params[50];
			sprintf(params,"Deploy,%d,923572385",Commands->Get_ID(obj));
			Commands->Attach_Script(sender,"Reborn_Deployable_Vehicle_Player",params);
		}
	}
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (PilotID == Commands->Get_ID(sender))
		{
			PilotID = 0;
			Remove_Script(sender,"Reborn_Deployable_Vehicle_Player");
		}
	}
	else if (type == 923572385)
	{
		if (!obj->As_VehicleGameObj()->Can_Drive())
		{
			return;
		}
		const char *model = Get_Parameter("Model_Name");
		char deployanim[512];
		char dmodel[512];
		sprintf(deployanim,"%s_d.%s_d",model,model);
		sprintf(dmodel,"%s_d",model);
		//mode: 0=walk;1=deploy;2=deployed;3=redeploy
		if (!mode)
		{
			if (!obj->As_VehicleGameObj()->Is_Immovable())
			{
				obj->As_VehicleGameObj()->Set_Immovable(true);
				mode = 1;
				Commands->Set_Model(obj,dmodel);
				Commands->Set_Animation(obj,deployanim,false,0,0,-1,0);
				Commands->Control_Enable(sender,false);
				obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(false);
				Commands->Create_Sound(Get_Parameter("DeploySound"),Commands->Get_Position(obj),obj);
				Commands->Enable_Vehicle_Transitions(obj,false);
			}
		}
		//mode: 0=walk;1=deploy;2=deployed;3=redeploy
		else if (mode == 2)
		{
			mode = 3;
			Commands->Set_Model(obj,dmodel);
			Commands->Set_Animation(obj,deployanim,false,0,Get_Float_Parameter("Last_Deploy_Frame"),0,0);
			Commands->Control_Enable(sender,false);
			obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(false);
			Commands->Create_Sound(Get_Parameter("UndeploySound"),Commands->Get_Position(obj),obj);
			Commands->Enable_Vehicle_Transitions(obj,false);
		}
	}
}

void Reborn_IsDeployableTank_2::Animation_Complete(GameObject *obj,const char *animation_name)
{
	const char *model;
	model = Get_Parameter("Model_Name");
	char deployedmodel[512];
	sprintf(deployedmodel,"%s_dd",model);
	char deployanim[512];
	sprintf(deployanim,"%s_d.%s_d",model,model);
	//tank deploy
	//mode: 0=walk;1=deploy;2=deployed;3=redeploy
	if (!_stricmp(animation_name,deployanim) && (mode == 1))
	{
		mode = 2;
		Commands->Set_Model(obj,deployedmodel);
		obj->As_PhysicalGameObj()->Clear_Animation();
		Commands->Control_Enable(Commands->Find_Object(PilotID),true);
		obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(true);
		Commands->Enable_Engine(obj,true);
		Commands->Enable_Vehicle_Transitions(obj,true);
		Commands->Enable_Innate_Conversations(obj,true);
		return;
	}
	//tank redeploy
	//mode: 0=walk;1=deploy;2=deployed;3=redeploy
	if (!_stricmp(animation_name,deployanim) && (mode == 3))
	{
		obj->As_VehicleGameObj()->Set_Immovable(false);
		mode = 0;
		Commands->Set_Model(obj,model);
		obj->As_PhysicalGameObj()->Clear_Animation();
		Commands->Control_Enable(Commands->Find_Object(PilotID),true);
		obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(true);
		Commands->Enable_Hibernation(obj,true);
		Commands->Enable_Innate_Conversations(obj,true);
		Commands->Enable_Vehicle_Transitions(obj,true);
		Commands->Enable_Engine(obj,true);
		return;
	}
}

void Reborn_IsDeployableTank_2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&PilotID,4,6);
	Auto_Save_Variable(&mode,4,7);
}

void Reborn_IsDeployableTank_2::Destroyed(GameObject *obj)
{
	Remove_Script(Commands->Find_Object(PilotID),"Reborn_Deployable_Vehicle_Player");
	Commands->Control_Enable(Commands->Find_Object(PilotID),true);
}

void Reborn_IsMech::Created(GameObject *obj)
{
	if (Mode != 1)
	{
		Vector3 pos = Commands->Get_Position(obj);
		xpos = pos.X;
		ypos = pos.Y;
		zpos = pos.Z;
		enabled = false;
		play = false;
		Mode = 1;
		backward = false;
		forward = false;
		Commands->Start_Timer(obj,this,(float)0.5,1);
	}
}

void Reborn_IsMech::Timer_Expired(GameObject *obj,int number)
{
	if (number == 2)
	{
		play = false;
	}
	else if (number == 4)
	{
		enabled = false;
	}
	else if (number == 3)
	{
		Vector3 pos = Commands->Get_Bone_Position(obj,"Origin");
		xpos = pos.X;
		ypos = pos.Y;
		zpos = pos.Z;
		Commands->Start_Timer(obj,this,(float)0.1,1);
		return;
	}
	else if (number == 1)
	{
		char moveanim[512];
		sprintf(moveanim,"%s.%s_m",Get_Model(obj),Get_Model(obj));
		char movebanim[512];
		sprintf(movebanim,"%s.%s_b",Get_Model(obj),Get_Model(obj));
		Commands->Start_Timer(obj,this,(float)0.1,1);
		if (obj->As_PhysicalGameObj() && obj->As_PhysicalGameObj()->Is_Attached_To_An_Object())
		{	
			Commands->Set_Animation(obj,moveanim,false,0,0,0,0);
			return;
		}
		Vector3 position = Commands->Get_Bone_Position(obj,"Origin");
		Vector3 pos;
		if (Mode == 1)
		{
			xpos = position.X;
			ypos = position.Y;
			zpos = position.Z;
			Mode = 2;
		}
		pos.X = xpos;
		pos.Y = ypos;
		pos.Z = zpos;
		float dist = Commands->Get_Distance(pos,position);
		//moved!
		if (dist > 0.1)
		{
			float xdist = position.X - xpos;
			float ydist = position.Y - ypos;
			float rotation = Commands->Get_Facing(obj);
			//rot near 0
			if (rotation > -45 && rotation < 45)
			{
				if (xdist > ydist)
				{
					forward = true;
					backward = false;
				}
				else if (ydist > xdist)
				{
					backward = true;
					forward = false;
				}
			}
			//rot near -90
			if (rotation < -45 && rotation > -135)
			{
				if (xdist > ydist)
				{
					forward = true;
					backward = false;
				}
				else if (ydist > xdist)
				{
					backward = true;
					forward = false;
				}
			}
			//rot near -180/180
			if (rotation < -135 || rotation > 135)
			{
				if (xdist < ydist)
				{
					forward = true;
					backward = false;
				}
				else if (ydist < xdist)
				{
					backward = true;
					forward = false;
				}
			}
			//rot near 90
			if (rotation < 135 && rotation > 45)
			{
				if (xdist < ydist)
				{
					forward = true;
					backward = false;
				}
				else if (ydist < xdist)
				{
					backward = true;
					forward = false;
				}
			}
			xpos = position.X;
			ypos = position.Y;
			zpos = position.Z;
			if (!enabled && forward)
			{
				enabled = true;
				if (Mode == 2)
				{
					Commands->Set_Animation(obj,moveanim,true,0,0,-1,0);
					LastDir = 1;
				}
				else if (LastDir == 2) 
				{
					Commands->Set_Animation(obj,movebanim,false,0,Get_Animation_Frame(obj),0,0);
					LastDir = 3;
				}
				else 
				{
					Commands->Set_Animation(obj,moveanim,true,0,Get_Animation_Frame(obj),-1,0);
					LastDir = 1;
				}
			}
			if (!enabled && backward) 
			{
				enabled = true;
				if (Mode == 2) 
				{
					Commands->Set_Animation(obj,movebanim,true,0,0,-1,0);
					LastDir = 2;
				}
				else if (LastDir == 1) 
				{
					Commands->Set_Animation(obj,moveanim,false,0,Get_Animation_Frame(obj),0,0);
					LastDir = 4;
				}
				else 
				{
					Commands->Set_Animation(obj,movebanim,true,0,Get_Animation_Frame(obj),-1,0);
					LastDir = 2;
				}
			}
			Mode = 3;
		}
		else if (Mode == 3) 
		{
			if (Get_Animation_Frame(obj)) 
			{
				if (forward) 
				{
					LastDir = 1;
					Commands->Set_Animation(obj,moveanim,true,0,Get_Animation_Frame(obj),Get_Animation_Frame(obj),0);
				}
				if (backward) 
				{
					LastDir = 2;
					Commands->Set_Animation(obj,movebanim,true,0,Get_Animation_Frame(obj),Get_Animation_Frame(obj),0);
				}
				enabled = true;
				Commands->Start_Timer(obj,this,(float)0.1,4);
				return;
			}
		}
		if (enabled && forward && !play)
		{
			if (((Get_Animation_Frame(obj) > (Get_Int_Parameter("Stomp1Frame_Forward") - 2)) && (Get_Animation_Frame(obj) < (Get_Int_Parameter("Stomp1Frame_Forward") + 2))) || ((Get_Animation_Frame(obj) > (Get_Int_Parameter("Stomp2Frame_Forward") - 1)) && (Get_Animation_Frame(obj) < (Get_Int_Parameter("Stomp2Frame_Forward") + 2))))
			{
				play = true;
				Commands->Create_3D_WAV_Sound_At_Bone(Get_Parameter("StompWAVSound"),obj,"Origin");
				Commands->Start_Timer(obj,this,(float)0.5,2);
			}
		}
		if (enabled && backward && !play)
		{
			if (((Get_Animation_Frame(obj) > (Get_Int_Parameter("Stomp1Frame_Backward") - 2)) && (Get_Animation_Frame(obj) < (Get_Int_Parameter("Stomp1Frame_Backward") + 2))) || ((Get_Animation_Frame(obj) > (Get_Int_Parameter("Stomp2Frame_Backward") - 1)) && (Get_Animation_Frame(obj) < (Get_Int_Parameter("Stomp2Frame_Backward") + 2))))
			{
				play = true;
				Commands->Create_3D_WAV_Sound_At_Bone(Get_Parameter("StompWAVSound"),obj,"Origin");
				Commands->Start_Timer(obj,this,(float)0.5,2);
			}
		}
	}
}

void Reborn_IsMech::Animation_Complete(GameObject *obj,const char *animation_name)
{
	if (LastDir == 3)
	{
		char moveanim[512];
		sprintf(moveanim,"%s.%s_m",Get_Model(obj),Get_Model(obj));
		Commands->Set_Animation(obj,moveanim,true,0,0,-1,0);
		LastDir = 1;
	}
	else if (LastDir == 4)
	{
		char moveanim[512];
		sprintf(moveanim,"%s.%s_b",Get_Model(obj),Get_Model(obj));
		Commands->Set_Animation(obj,moveanim,true,0,0,-1,0);
		LastDir = 2;
	}
	else
	{
		enabled = false;
	}
}

void Reborn_IsMech::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
	Auto_Save_Variable(&xpos,4,2);
	Auto_Save_Variable(&ypos,4,3);
	Auto_Save_Variable(&forward,1,4);
	Auto_Save_Variable(&backward,1,5);
	Auto_Save_Variable(&play,1,7);
	Auto_Save_Variable(&Mode,4,8);
	Auto_Save_Variable(&LastDir,4,9);
}



void Reborn_IsDeployableTank_3::Created(GameObject *obj)
{
	//mode: 0=walk;1=deploy;2=deployed;3=redeploy
	mode = 0;
	PilotID = 0;
}

void Reborn_IsDeployableTank_3::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!PilotID)
		{
			PilotID = Commands->Get_ID(sender);
			char params[50];
			sprintf(params,"Deploy,%d,923572385",Commands->Get_ID(obj));
			Commands->Attach_Script(sender,"Reborn_Deployable_Vehicle_Player",params);
		}
	}
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (PilotID == Commands->Get_ID(sender))
		{
			PilotID = 0;
			Remove_Script(sender,"Reborn_Deployable_Vehicle_Player");
		}
	}
	else if (type == 923572385)
	{
		if (!obj->As_VehicleGameObj()->Can_Drive())
		{
			return;
		}
		const char *model = Get_Parameter("Model_Name");
		char deployanim[512];
		char dmodel[512];
		sprintf(deployanim,"%s_d.%s_d",model,model);
		sprintf(dmodel,"%s_d",model);
		//mode: 0=walk;1=deploy;2=deployed;3=redeploy
		if (!mode)
		{
			if (!obj->As_VehicleGameObj()->Is_Immovable())
			{
				obj->As_VehicleGameObj()->Set_Immovable(true);
				mode = 1;
				Commands->Set_Model(obj,dmodel);
				Commands->Set_Shield_Type(obj,Get_Parameter("Armor"));
				Commands->Set_Animation(obj,deployanim,false,0,0,-1,0);
				Commands->Control_Enable(sender,false);
				obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(false);
				Commands->Create_Sound(Get_Parameter("DeploySound"),Commands->Get_Position(obj),obj);
				Commands->Enable_Vehicle_Transitions(obj,false);
				Commands->Clear_Weapons(obj);
				Commands->Give_PowerUp(obj,Get_Parameter("Weapon_Powerup_Name"),false);
				Commands->Select_Weapon(obj,Get_Parameter("Weapon_Name"));
			}
		}
		//mode: 0=walk;1=deploy;2=deployed;3=redeploy
		else if (mode == 2)
		{
			mode = 3;
			Commands->Set_Model(obj,dmodel);
			Commands->Set_Animation(obj,deployanim,false,0,Get_Float_Parameter("Last_Deploy_Frame"),0,0);
			Commands->Control_Enable(sender,false);
			obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(false);
			Commands->Create_Sound(Get_Parameter("UndeploySound"),Commands->Get_Position(obj),obj);
			Commands->Enable_Vehicle_Transitions(obj,false);
		}
	}
}

void Reborn_IsDeployableTank_3::Animation_Complete(GameObject *obj,const char *animation_name)
{
	const char *model;
	model = Get_Parameter("Model_Name");
	char deployedmodel[512];
	sprintf(deployedmodel,"%s_dd",model);
	char deployanim[512];
	sprintf(deployanim,"%s_d.%s_d",model,model);
	//tank deploy
	//mode: 0=walk;1=deploy;2=deployed;3=redeploy
	if (!_stricmp(animation_name,deployanim) && (mode == 1))
	{
		mode = 2;
		Commands->Set_Model(obj,deployedmodel);
		obj->As_PhysicalGameObj()->Clear_Animation();
		Commands->Control_Enable(Commands->Find_Object(PilotID),true);
		obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(true);
		Commands->Enable_Engine(obj,false);
		Commands->Enable_Vehicle_Transitions(obj,true);
		Commands->Enable_Innate_Conversations(obj,true);
		return;
	}
	//tank redeploy
	//mode: 0=walk;1=deploy;2=deployed;3=redeploy
	if (!_stricmp(animation_name,deployanim) && (mode == 3))
	{
		obj->As_VehicleGameObj()->Set_Immovable(false);
		mode = 0;
		Commands->Set_Model(obj,model);
		obj->As_PhysicalGameObj()->Clear_Animation();
		Commands->Set_Shield_Type(obj,Get_Parameter("Armor2"));
		Commands->Control_Enable(Commands->Find_Object(PilotID),true);
		obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(true);
		Commands->Enable_Hibernation(obj,false);
		Commands->Enable_Innate_Conversations(obj,true);
		Commands->Enable_Vehicle_Transitions(obj,true);
		Commands->Enable_Engine(obj,true);
		Commands->Clear_Weapons(obj);
		Commands->Give_PowerUp(obj,Get_Parameter("Weapon_Powerup_Name2"),false);
		Commands->Select_Weapon(obj,Get_Parameter("Weapon_Name2"));
		return;
	}
}

void Reborn_IsDeployableTank_3::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&PilotID,4,6);
	Auto_Save_Variable(&mode,4,7);
}

void Reborn_IsDeployableTank_3::Destroyed(GameObject *obj)
{
	Remove_Script(Commands->Find_Object(PilotID),"Reborn_Deployable_Vehicle_Player");
	Commands->Control_Enable(Commands->Find_Object(PilotID),true);
}

void Reborn_IsDeployableTank_4::Created(GameObject *obj)
{
	//mode: 0=walk;1=deploy;2=deployed;3=redeploy
	mode = 0;
	PilotID = 0;
}

void Reborn_IsDeployableTank_4::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!PilotID)
		{
			PilotID = Commands->Get_ID(sender);
			char params[50];
			sprintf(params, "Deploy,%d,923572385", Commands->Get_ID(obj));
			Commands->Attach_Script(sender, "Reborn_Deployable_Vehicle_Player", params);
		}
	}
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (PilotID == Commands->Get_ID(sender))
		{
			PilotID = 0;
			Remove_Script(sender, "Reborn_Deployable_Vehicle_Player");
		}
	}
	else if (type == 923572385)
	{
		if (!obj->As_VehicleGameObj()->Can_Drive())
		{
			return;
		}
		const char *model = Get_Parameter("Model_Name");
		char deployanim[512];
		char dmodel[512];
		sprintf(deployanim, "%s_d.%s_d", model, model);
		sprintf(dmodel, "%s_d", model);
		//mode: 0=walk;1=deploy;2=deployed;3=redeploy
		if (!mode)
		{
			Vector3 pos = Commands->Get_Bone_Position(obj, Get_Parameter("GroundBone"));
			Vector3 pos2 = pos;
			pos2.Z -= (Get_Float_Parameter("GroundDistance") * 2);
			CastResultStruct res;
			res.ComputeContactPoint = true;
			LineSegClass ray(pos, pos2);
			PhysRayCollisionTestClass coltest(ray, &res, DEFAULT_COLLISION_GROUP);
			coltest.CheckDynamicObjs = false;
			PhysicsSceneClass::Get_Instance()->Cast_Ray(coltest, false);
			if ((coltest.CollidedRenderObj) && (Vector3::Dot_Product(res.Normal, ray.Get_Dir()) <= 0) && (pos.Z - res.ContactPoint.Z <= Get_Float_Parameter("GroundDistance")) && (!obj->As_VehicleGameObj()->Is_Immovable()))
			{
				Commands->Enable_Engine(obj,false);
				Commands->Start_Timer(obj, this, 2, Commands->Get_ID(sender));
			}
			else
			{
				if (PilotID)
				{
					Create_2D_Sound_Player(Commands->Find_Object(PilotID), Get_Parameter("Sound"));
					unsigned int red, green, blue;
					Get_Object_Color(Commands->Find_Object(PilotID), &red, &green, &blue);
					Send_Message_Player(Commands->Find_Object(PilotID), red, green, blue, Get_Parameter("Message"));
				}
			}
		}
		//mode: 0=walk;1=deploy;2=deployed;3=redeploy
		else if (mode == 2)
		{
			mode = 3;
			Commands->Clear_Weapons(obj);
			Commands->Set_Model(obj, dmodel);
			Commands->Set_Animation(obj, deployanim, false, 0, Get_Float_Parameter("Last_Deploy_Frame"), 0, 0);
			Commands->Control_Enable(sender, false);
			obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(false);
			Commands->Create_Sound(Get_Parameter("UndeploySound"),Commands->Get_Position(obj),obj);
			Commands->Enable_Vehicle_Transitions(obj, false);
		}
	}
}

void Reborn_IsDeployableTank_4::Timer_Expired(GameObject *obj, int number)
{
	const char *model = Get_Parameter("Model_Name");
	char deployanim[512];
	char dmodel[512];
	sprintf(deployanim, "%s_d.%s_d", model, model);
	sprintf(dmodel, "%s_d", model);
	obj->As_VehicleGameObj()->Set_Immovable(true);
	mode = 1;
	Commands->Set_Model(obj, dmodel);
	Commands->Set_Animation(obj, deployanim, false, 0, 0, -1, 0);
	Commands->Set_Shield_Type(obj, Get_Parameter("Armor"));
	Commands->Control_Enable(Commands->Find_Object(number), false);
	obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(false);
	Commands->Create_Sound(Get_Parameter("DeploySound"),Commands->Get_Position(obj),obj);
	Commands->Enable_Vehicle_Transitions(obj, false);
	Commands->Clear_Weapons(obj);
	Commands->Give_PowerUp(obj, Get_Parameter("Weapon_Powerup_Name"), false);
	Commands->Select_Weapon(obj, Get_Parameter("Weapon_Name"));
}

void Reborn_IsDeployableTank_4::Animation_Complete(GameObject *obj, const char *animation_name)
{
	const char *model;
	model = Get_Parameter("Model_Name");
	char deployedmodel[512];
	sprintf(deployedmodel, "%s_dd", model);
	char deployanim[512];
	sprintf(deployanim, "%s_d.%s_d", model, model);
	//tank deploy
	//mode: 0=walk;1=deploy;2=deployed;3=redeploy
	if (!_stricmp(animation_name, deployanim) && (mode == 1))
	{
		mode = 2;
		Commands->Set_Model(obj, deployedmodel);
		obj->As_PhysicalGameObj()->Clear_Animation();
		Commands->Control_Enable(Commands->Find_Object(PilotID), true);
		obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(true);
		Commands->Enable_Engine(obj, false);
		Commands->Enable_Vehicle_Transitions(obj, true);
		Commands->Enable_Innate_Conversations(obj, true);
		Commands->Clear_Weapons(obj);
		Commands->Give_PowerUp(obj, Get_Parameter("Weapon_Powerup_Name"), false);
		Commands->Select_Weapon(obj, Get_Parameter("Weapon_Name"));
		return;
	}
	//tank redeploy
	//mode: 0=walk;1=deploy;2=deployed;3=redeploy
	if (!_stricmp(animation_name, deployanim) && (mode == 3))
	{
		obj->As_VehicleGameObj()->Set_Immovable(false);
		mode = 0;
		Commands->Set_Model(obj, model);
		Commands->Set_Shield_Type(obj, Get_Parameter("Armor2"));
		obj->As_PhysicalGameObj()->Clear_Animation();
		Commands->Control_Enable(Commands->Find_Object(PilotID), true);
		obj->As_VehicleGameObj()->Set_Scripts_Can_Fire(true);
		Commands->Enable_Hibernation(obj, false);
		Commands->Enable_Innate_Conversations(obj, true);
		Commands->Enable_Vehicle_Transitions(obj, true);
		Commands->Enable_Engine(obj, true);
		Commands->Clear_Weapons(obj);
		Commands->Give_PowerUp(obj, Get_Parameter("Weapon_Powerup_Name2"), false);
		Commands->Select_Weapon(obj, Get_Parameter("Weapon_Name2"));
		return;
	}
}

void Reborn_IsDeployableTank_4::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&PilotID, 4, 6);
	Auto_Save_Variable(&mode, 4, 7);
}

void Reborn_IsDeployableTank_4::Destroyed(GameObject *obj)
{
	Remove_Script(Commands->Find_Object(PilotID),"Reborn_Deployable_Vehicle_Player");
	Commands->Control_Enable(Commands->Find_Object(PilotID),true);
}

ScriptRegistrant<Reborn_Game_Manager> Reborn_Game_Manager_Registrant("Reborn_Game_Manager", "");
ScriptRegistrant<Reborn_MMK2> Reborn_MMK2_Registrant("Reborn_MMK2","GameManager_ID:int,Explosion_Preset_Name:string,MoneyToReturn:float");
ScriptRegistrant<Reborn_Cyborg>Reborn_Cyborg_Registrant("Reborn_Cyborg","GameManager_ID:int,Explosion_Preset_Name:string,MoneyToReturn:float");
ScriptRegistrant<Reborn_Deployable_Vehicle> Reborn_Deployable_Vehicle_Registrant("Reborn_Deployable_Vehicle","Animated_Preset:string,Explosion_Preset:string");
ScriptRegistrant<Reborn_Deploy_Animation> Reborn_Deploy_Animation_Registrant("Reborn_Deploy_Animation","Time_Till_Death:float,Deployed_Tank_Preset:string,Explosion_Preset:string");
ScriptRegistrant<Reborn_MMK2_Turret_Spawn> Reborn_MMK2_Turret_Spawn_Registrant("Reborn_MMK2_Turret_Spawn","Turret_Preset:string,Bone_Name=Tur_Mount:string,Explosion_Preset:string");
ScriptRegistrant<Reborn_Diggable_Vehicle> Reborn_Diggable_Vehicle_Registrant("Reborn_Diggable_Vehicle","Mode:int,Dig_Delay:float,Z_Hieght_Adjust:float");
ScriptRegistrant<Reborn_PlaySound_OnCreate> Reborn_PlaySound_OnCreate_Registrant("Reborn_PlaySound_OnCreate","Sound_Preset_Name=BLAH!:string");
ScriptRegistrant<Reborn_Diggable_Vehicle_Animated> Reborn_Diggable_Vehicle_Animated_Registrant("Reborn_Diggable_Vehicle_Animated","Z_Hieght_Adjust:float,Dig_Animation:string,Surface_Animation:string");
ScriptRegistrant<Reborn_IsDeployableMech> Reborn_IsDeployableMech_Registrant("Reborn_IsDeployableMech","Model_Name=none:string,Weapon_Name=none:string,Weapon_Powerup_Name=none:string,Last_Deploy_Frame=0.00:float,StompWAVSound=wolverine_stomp.wav:string,Stomp1Frame_Forward=21:int,Stomp2Frame_Forward=10:int,Stomp1Frame_Backward=51:int,Stomp2Frame_Backward=40:int,DeploySound:string,UndeploySound:string");
ScriptRegistrant<Reborn_IsDeployableTank> Reborn_IsDeployableTank_Registrant("Reborn_IsDeployableTank","Model_Name=v_nod_art:string,Weapon_Name=weapon_artillery:string,Weapon_Powerup_Name=pow_artillery:string,Last_Deploy_Frame=0.00:float,Velocity:float,Message:string,Sound:string,DeploySound:string,UndeploySound:string");
ScriptRegistrant<Reborn_IsDeployableTank_2> Reborn_IsDeployableTank_2_Registrant("Reborn_IsDeployableTank_2","Model_Name=v_nod_art:string,Last_Deploy_Frame=0.00:float,Velocity:float,Message:string,Sound:string,DeploySound:string,UndeploySound:string");
ScriptRegistrant<Reborn_IsDeployableTank_3> Reborn_IsDeployableTank_3_Registrant("Reborn_IsDeployableTank_3","Model_Name=v_nod_art:string,Last_Deploy_Frame=0.00:float,Weapon_Name:string,Weapon_Powerup_Name:string,Armor:string,Weapon_Name2:string,Weapon_Powerup_Name2:string,Armor2:string,Velocity:float,Message:string,Sound:string,DeploySound:string,UndeploySound:string");
ScriptRegistrant<Reborn_IsDeployableTank_4> Reborn_IsDeployableTank_4_Registrant("Reborn_IsDeployableTank_4","Model_Name=v_nod_art:string,Last_Deploy_Frame=0.00:float,Weapon_Name:string,Weapon_Powerup_Name:string,Armor:string,Weapon_Name2:string,Weapon_Powerup_Name2:string,Armor2:string,Velocity:float,Message:string,Sound:string,GroundBone:string,GroundDistance:float,DeploySound:string,UndeploySound:string");
ScriptRegistrant<Reborn_IsMech> Reborn_IsMech_Registrant("Reborn_IsMech", "StompWAVSound=wolverine_stomp.wav:string,Stomp1Frame_Forward=21:int,Stomp2Frame_Forward=51:int,Stomp1Frame_Backward=10:int,Stomp2Frame_Backward=40:int");
ScriptRegistrant<Reborn_Deployable_Vehicle_Player> Reborn_Deployable_Vehicle_Player_Registrant("Reborn_Deployable_Vehicle_Player","Key=Deploy:string,ID=0:int,Message=0:int");
