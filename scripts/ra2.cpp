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
//************************************************************************************************
//Ra2.cpp
//e-mail: Boris_the_invincible@hotmail.com
//e-mail2: TheKGBspy@timeofwar.com
//************************************************************************************************
#include "general.h"

#include "scripts.h"
#include "ra2.h"
#include "PhysicalGameObj.h"

void Ra2Ven_MirageTank::Created(GameObject *obj)
{
	pilotID = 0;
	xpos = 0;
	ypos = 0;
	zpos = 0;
	IsHided = false;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNumber"));
}

void Ra2Ven_MirageTank::Killed(GameObject *obj,GameObject *killer)
{
	Vector3 TreeExplo;
	TreeExplo = Commands->Get_Bone_Position(obj,"origin");
	Commands->Create_Explosion(Get_Parameter("Explosion_Preset"),TreeExplo,0);
}

void Ra2Ven_MirageTank::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&pilotID,4,1);
	Auto_Save_Variable(&xpos,4,2);
	Auto_Save_Variable(&ypos,4,3);
	Auto_Save_Variable(&zpos,4,4);
	Auto_Save_Variable(&IsHided,1,5);
}

void Ra2Ven_MirageTank::Timer_Expired(GameObject *obj,int number)
{
	const char *preset;
	int rand;
	GameObject *tree;
	Vector3 TreePos;
	Vector3 TreeExplo;
	Vector3 position = Commands->Get_Position(obj);
	if ((!xpos) && (!ypos) && (!zpos))
	{
		xpos = position.X;
		ypos = position.Y;
		zpos = position.Z;
	}
	if (pilotID)
	{
		if ((fabs(position.X-xpos) <= .5) && (fabs(position.Y-ypos) <= .5) && (fabs(position.Z-zpos) <= .5))
		{
			if (!IsHided)
			{
			preset = "none";
			while (!_stricmp(preset,"none"))
			{
				rand = Commands->Get_Random_Int(1,5);
				switch(rand)
				{
					case 1:
						preset = Get_Parameter("Tree1_Preset");
						break;
					case 2:
						preset = Get_Parameter("Tree2_Preset");
						break;
					case 3:
						preset = Get_Parameter("Tree3_Preset");
						break;
					case 4:
						preset = Get_Parameter("Tree4_Preset");
						break;
				}
			}
			TreePos.X = 0;
			TreePos.Y = 0;
			TreePos.Z = 0;
			tree = Commands->Create_Object(preset,TreePos);
			Commands->Attach_To_Object_Bone(tree,obj,"origin");
			Commands->Set_Is_Rendered(obj,false);
			IsHided = true;
			}
		}
		else
		{
			xpos = position.X;
			ypos = position.Y;
			zpos = position.Z;
			if (IsHided)
			{
				Commands->Set_Is_Rendered(obj,true);
				IsHided = false;
				TreeExplo = Commands->Get_Bone_Position(obj,"origin");
				Commands->Create_Explosion(Get_Parameter("Explosion_Preset"),TreeExplo,0);
			}
		}
	}
	else
	{
		if (IsHided)
		{
			Commands->Set_Is_Rendered(obj,true);
			IsHided = false;
			TreeExplo = Commands->Get_Bone_Position(obj,"origin");
			Commands->Create_Explosion(Get_Parameter("Explosion_Preset"),TreeExplo,0);
		}
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNumber"));
}

void Ra2Ven_MirageTank::Custom(GameObject *obj,int type,int param,GameObject *sender)
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

void Ra2Ven_VehicleFalling::Created(GameObject *obj)
{
	IsFalling = false;
	xpos = 0;
	ypos = 0;
	zpos = 0;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNumber"));
}

void Ra2Ven_VehicleFalling::Timer_Expired(GameObject *obj,int number)
{
	Vector3 position = Commands->Get_Position(obj);
	const char *warh;
	double calcul;
	int convert;
	float dom;
	if ((!xpos) && (!ypos) && (!zpos))
	{
		xpos = position.X;
		ypos = position.Y;
		zpos = position.Z;
	}
	if ((position.Z-zpos) > Get_Int_Parameter("ZValueBeforeStartFallingMode"))
	{
		IsFalling = true;
	}
	else
	{
		if (IsFalling)
		{
			Vector3 ExplosionSound;
			calcul = (fabs(zpos-position.Z)/2.5)*Get_Int_Parameter("DammageMultiplier");
			convert = (int)calcul;
			dom = (float)convert;
			warh = Get_Parameter("Warhead");
			ExplosionSound = Commands->Get_Bone_Position(obj,"origin");
			Commands->Create_Explosion(Get_Parameter("ImpactExplosion_Preset"),ExplosionSound,0);
			Commands->Apply_Damage(obj,dom,warh,0);
			IsFalling = false;
		}
		zpos = position.Z;
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNumber"));
}

void Ra2Ven_VehicleFalling::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&IsFalling,1,1);
	Auto_Save_Variable(&xpos,4,2);
	Auto_Save_Variable(&ypos,4,3);
	Auto_Save_Variable(&zpos,4,4);
}

void Ra2Ven_DemoTruck::Created(GameObject *obj)
{
	pilotID = 0;
	CanExplode = true;
}

void Ra2Ven_DemoTruck::Custom(GameObject *obj,int type,int param,GameObject *sender)
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

void Ra2Ven_DemoTruck::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if ((damager == Commands->Find_Object(pilotID)) && (damager))
	{
		CanExplode = false;
		Commands->Apply_Damage(obj,Get_Float_Parameter("Dammage"),Get_Parameter("Warhead"),0);
	}
}

void Ra2Ven_DemoTruck::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&pilotID,4,1);
	Auto_Save_Variable(&CanExplode,1,2);
}

void Ra2Ven_DemoTruck::Killed(GameObject *obj,GameObject *killer)
{
	if (CanExplode)
	{
		Vector3 DemoCharge;
		DemoCharge = Commands->Get_Bone_Position(obj,"origin");
		Commands->Create_Explosion(Get_Parameter("Demo_Explosion"),DemoCharge,0);
	}
}

void Ra2Ven_Deployable_Vehicle::Created(GameObject *obj)
{
	pilotID = 0;
	CanDestroyAnim = true;
}

void Ra2Ven_Deployable_Vehicle::Custom(GameObject *obj,int type,int param,GameObject *sender)
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

void Ra2Ven_Deployable_Vehicle::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	GameObject *Animation;
	Vector3 position;
	int TempCalcul;
	if ((damager == Commands->Find_Object(pilotID)) && (damager))
	{
		position.X = 0;
		position.Y = 0;
		position.Z = 0;
		Animation = Commands->Create_Object(Get_Parameter("Animation_Preset"),position);
		TempCalcul = (int)(Commands->Get_Max_Health(Animation)*(Commands->Get_Health(obj)/Commands->Get_Max_Health(obj)));
		Commands->Set_Health(Animation,(float)TempCalcul);
		TempCalcul = (int)(Commands->Get_Max_Shield_Strength(Animation)*(Commands->Get_Shield_Strength(obj)/Commands->Get_Max_Shield_Strength(obj)));
		Commands->Set_Shield_Strength(Animation,(float)TempCalcul);
		Commands->Attach_To_Object_Bone(Animation,obj,"origin");
		CanDestroyAnim = false;
		Commands->Apply_Damage(obj,Get_Float_Parameter("oldTnk_Dammage"),Get_Parameter("oldTnk_Warhead"),0);
	}
}

void Ra2Ven_Deployable_Vehicle::Killed(GameObject *obj,GameObject *killer)
{
	Vector3 position;
	if (CanDestroyAnim)
	{
		position = Commands->Get_Bone_Position(obj,"origin");
		Commands->Create_Explosion(Get_Parameter("Explosion_preset"),position,0);
	}
}

void Ra2Ven_Deployable_Vehicle::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&pilotID,4,1);
	Auto_Save_Variable(&CanDestroyAnim,1,2);
}

void Ra2Ven_Deploy_Animation::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Animation_Time"),Get_Int_Parameter("TimerNumber"));
}

void Ra2Ven_Deploy_Animation::Timer_Expired(GameObject *obj,int number)
{
	GameObject *DeployedTnk;
	Vector3 position;
	int TempCalcul;
	position.X = 0;
	position.Y = 0;
	position.Z = 0;
	DeployedTnk = Commands->Create_Object(Get_Parameter("Deployed_Tank_Preset"),position);
	TempCalcul = (int)(Commands->Get_Max_Health(DeployedTnk)*(Commands->Get_Health(obj)/Commands->Get_Max_Health(obj)));
	Commands->Set_Health(DeployedTnk,(float)TempCalcul);
	TempCalcul = (int)(Commands->Get_Max_Shield_Strength(DeployedTnk)*(Commands->Get_Shield_Strength(obj)/Commands->Get_Max_Shield_Strength(obj)));
	Commands->Set_Shield_Strength(DeployedTnk,(float)TempCalcul);
	Commands->Attach_To_Object_Bone(DeployedTnk,obj,"origin");
	Commands->Apply_Damage(obj,Get_Float_Parameter("oldTnk_Dammage"),Get_Parameter("oldTnk_Warhead"),0);
}

void Ra2Ven_RandomTree::Created(GameObject *obj)
{
	const char *preset;
	int rand;
	GameObject *tree;
	Vector3 TreePos;
	preset = "none";
	rand = Commands->Get_Random_Int(1,6);
	switch(rand)
	{
		case 1:
			preset = Get_Parameter("Tree1_Preset");
			break;
		case 2:
			preset = Get_Parameter("Tree2_Preset");
			break;
		case 3:
			preset = Get_Parameter("Tree3_Preset");
			break;
		case 4:
			preset = Get_Parameter("Tree4_Preset");
			break;
		case 5:
			preset = "none";
			break;
	}
	if (_stricmp(preset,"none"))
	{
		TreePos.X = 0;
		TreePos.Y = 0;
		TreePos.Z = 0;
		tree = Commands->Create_Object(preset,TreePos);
		Commands->Attach_To_Object_Bone(tree,obj,"origin");
	}
	Destroy_Script();
}

void Ra2Ven_TurretSound::Created(GameObject *obj)
{
	GameObject *TurretObj;
	Vector3 TurPos;
	TurPos.X = 0;
	TurPos.Y = 0;
	TurPos.Z = 0;
	TurretObj = Commands->Create_Object(Get_Parameter("TurretSoundObj"),TurPos);
	Commands->Attach_To_Object_Bone(TurretObj,obj,"turret");
	TurretId = Commands->Get_ID(TurretObj);
}

void Ra2Ven_TurretSound::Killed(GameObject *obj,GameObject *killer)
{
	Commands->Send_Custom_Event(obj,Commands->Find_Object(TurretId),201,0,0);
}

void Ra2Ven_TurretSound::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&TurretId,4,1);
}

void Ra2Ven_TurretSoundObj::Created(GameObject *obj)
{
	int MyFacing;
	if ((int)Commands->Get_Facing(obj) <= 0)
	{
		MyFacing = (int)Commands->Get_Facing(obj) + 360;
	}
	else
	{
		MyFacing = (int)Commands->Get_Facing(obj);
	}
	LastOrientation = MyFacing;
	CanPlay = true;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNumber"));
}

void Ra2Ven_TurretSoundObj::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 201)
	{
		Vector3 TurExplo;
		TurExplo = Commands->Get_Position(obj);
		Commands->Create_Explosion(Get_Parameter("Explosion_Preset"),TurExplo,0);
	}
}

void Ra2Ven_TurretSoundObj::Timer_Expired(GameObject *obj,int number)
{
	if (number == Get_Int_Parameter("TimerNumber"))
	{
		int MyFacing;
		if ((int)Commands->Get_Facing(obj) <= 0)
		{
			MyFacing = (int)Commands->Get_Facing(obj) + 360;
		}
		else
		{
			MyFacing = (int)Commands->Get_Facing(obj);
		}
		if (MyFacing != LastOrientation)
		{
			LastOrientation = MyFacing;
			if (CanPlay)
			{
				Commands->Create_3D_Sound_At_Bone(Get_Parameter("Rotate_3dSound"),obj,"turret");
				CanPlay = false;
				Commands->Start_Timer(obj,this,Get_Float_Parameter("Wav_Length"),Get_Int_Parameter("TimerNumber2"));
			}
		}
	}
	else if (number == Get_Int_Parameter("TimerNumber2"))
	{
		CanPlay = true;
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNumber"));
}

void Ra2Ven_TurretSoundObj::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&LastOrientation,4,1);
	Auto_Save_Variable(&CanPlay,1,2);
}

void Ra2Ven_OccupentWeapon::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!_stricmp(Commands->Get_Preset_Name(sender),Get_Parameter("Character1")))
		{
			Commands->Clear_Weapons(obj);
			Commands->Give_PowerUp(obj,Get_Parameter("PowerUp1"),false);
			Commands->Select_Weapon(obj,Get_Parameter("Weapon1"));
		}
		else if (!_stricmp(Commands->Get_Preset_Name(sender),Get_Parameter("Character2")))
		{
			Commands->Clear_Weapons(obj);
			Commands->Give_PowerUp(obj,Get_Parameter("PowerUp2"),false);
			Commands->Select_Weapon(obj,Get_Parameter("Weapon2"));
		}
		else if (!_stricmp(Commands->Get_Preset_Name(sender),Get_Parameter("Character3")))
		{
			Commands->Clear_Weapons(obj);
			Commands->Give_PowerUp(obj,Get_Parameter("PowerUp3"),false);
			Commands->Select_Weapon(obj,Get_Parameter("Weapon3"));
		}
		else if (!_stricmp(Commands->Get_Preset_Name(sender),Get_Parameter("Character4")))
		{
			Commands->Clear_Weapons(obj);
			Commands->Give_PowerUp(obj,Get_Parameter("PowerUp4"),false);
			Commands->Select_Weapon(obj,Get_Parameter("Weapon4"));
		}
		else 
		{
			Commands->Clear_Weapons(obj);
			Commands->Give_PowerUp(obj,Get_Parameter("DefPowerUp"),false);
			Commands->Select_Weapon(obj,Get_Parameter("DefWeapon"));
		}
	}
}

void Ra2Ven_Dplbl_Vhcls_Cursor::Created(GameObject *obj)
{
	pilotID = 0;
	CanDestroyAnim = true;
	IsOnDeploy = false;
}

void Ra2Ven_Dplbl_Vhcls_Cursor::Custom(GameObject *obj,int type,int param,GameObject *sender)
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
			if (IsOnDeploy)
			{
				GameObject *Animation;
				Vector3 position;
				int TempCalcul;
				position = Commands->Get_Bone_Position(obj,Get_Parameter("CursorBoneName"));
				Commands->Create_Explosion(Get_Parameter("CursorExplosion"),position,0);
				position.X = 0;
				position.Y = 0;
				position.Z = 0;
				Animation = Commands->Create_Object(Get_Parameter("Animation_Preset"),position);
				TempCalcul = (int)(Commands->Get_Max_Health(Animation)*(Commands->Get_Health(obj)/Commands->Get_Max_Health(obj)));
				Commands->Set_Health(Animation,(float)TempCalcul);
				TempCalcul = (int)(Commands->Get_Max_Shield_Strength(Animation)*(Commands->Get_Shield_Strength(obj)/Commands->Get_Max_Shield_Strength(obj)));
				Commands->Set_Shield_Strength(Animation,(float)TempCalcul);
				Commands->Attach_To_Object_Bone(Animation,obj,"origin");
				CanDestroyAnim = false;
				Commands->Apply_Damage(obj,Get_Float_Parameter("oldTnk_Dammage"),Get_Parameter("oldTnk_Warhead"),0);
			}
		pilotID = 0;
		}
	}
}

void Ra2Ven_Dplbl_Vhcls_Cursor::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if ((damager == Commands->Find_Object(pilotID)) && (damager) && (!amount))
	{
		Vector3 CursorBone;
		GameObject *CursorObj;

		if (IsOnDeploy)
		{
			CursorBone = Commands->Get_Bone_Position(obj,Get_Parameter("CursorBoneName"));
			Commands->Create_Explosion(Get_Parameter("CursorExplosion"),CursorBone,0);
			IsOnDeploy = false;
		}
		else
		{
			CursorBone = Commands->Get_Bone_Position(obj,Get_Parameter("CursorBoneName"));
			Commands->Create_Explosion(Get_Parameter("CursorExplosion"),CursorBone,0);
			CursorBone.X = 0;
			CursorBone.Y = 0;
			CursorBone.Z = 0;
			CursorObj = Commands->Create_Object(Get_Parameter("Cursor_preset"),CursorBone);
			Commands->Attach_To_Object_Bone(CursorObj,obj,Get_Parameter("CursorBoneName"));
			IsOnDeploy = true;
		}
	}
}

void Ra2Ven_Dplbl_Vhcls_Cursor::Killed(GameObject *obj,GameObject *killer)
{
	Vector3 position;

	if (CanDestroyAnim)
	{
		position = Commands->Get_Bone_Position(obj,"origin");
		Commands->Create_Explosion(Get_Parameter("Explosion_preset"),position,0);
		position = Commands->Get_Bone_Position(obj,Get_Parameter("CursorBoneName"));
		Commands->Create_Explosion(Get_Parameter("CursorExplosion"),position,0);
	}
}

void Ra2Ven_Dplbl_Vhcls_Cursor::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&pilotID,4,1);
	Auto_Save_Variable(&CanDestroyAnim,1,2);
	Auto_Save_Variable(&IsOnDeploy,1,3);
}

void Ra2Ven_FireAnimation::Created(GameObject *obj)
{
	if (!_stricmp("none",Get_Parameter("AfAnimation")))
	{
		AnimMode = 0;
	}
	else
	{
		AnimMode = 1;
	}
	CanPlayAnim = true;
	InitDistance = Commands->Get_Distance(Commands->Get_Bone_Position(obj,Get_Parameter("ParentBone")),Commands->Get_Bone_Position(obj,Get_Parameter("MuzzleBone")));
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNumber"));
}

void Ra2Ven_FireAnimation::Timer_Expired(GameObject *obj,int number)
{
	float Distance;
	Distance = Commands->Get_Distance(Commands->Get_Bone_Position(obj,Get_Parameter("ParentBone")),Commands->Get_Bone_Position(obj,Get_Parameter("MuzzleBone")));
	if ((Distance == InitDistance))
	{
		CanPlayAnim = true;
	}
	if (((int)(Distance*100) != (int)(InitDistance*100)) && (CanPlayAnim))
	{
		Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("FirstFrame"),Get_Float_Parameter("LastFrame"),false);
		CanPlayAnim = false;
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNumber"));
}

void Ra2Ven_FireAnimation::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&InitDistance,4,1);
	Auto_Save_Variable(&CanPlayAnim,1,2);
	Auto_Save_Variable(&AnimMode,4,3);
}

void Ra2Ven_FireAnimation::Animation_Complete(GameObject *obj,const char *animation_name)
{
	if (!_stricmp(animation_name,Get_Parameter("Animation")))
	{
		if (AnimMode)
		{
			Commands->Set_Animation(obj,Get_Parameter("AfAnimation"),false,0,Get_Float_Parameter("AfFirstFrame"),Get_Float_Parameter("AfLastFrame"),false);
			CanPlayAnim = true;
		}
		else
		{
			obj->As_PhysicalGameObj()->Clear_Animation();
		}
	}
	else if (!_stricmp(animation_name,Get_Parameter("AfAnimation")))
	{
		obj->As_PhysicalGameObj()->Clear_Animation();
	}
}

void Ra2Ven_FireAnimation2::Created(GameObject *obj)
{
	CanPlayAnim = true;
	CurAnim = 1;
	InitDistance = Commands->Get_Distance(Commands->Get_Bone_Position(obj,Get_Parameter("ParentBone")),Commands->Get_Bone_Position(obj,Get_Parameter("MuzzleBone")));
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNumber"));
}

void Ra2Ven_FireAnimation2::Timer_Expired(GameObject *obj,int number)
{
	float Distance;
	Distance = Commands->Get_Distance(Commands->Get_Bone_Position(obj,Get_Parameter("ParentBone")),Commands->Get_Bone_Position(obj,Get_Parameter("MuzzleBone")));
	if ((Distance == InitDistance))
	{
		CanPlayAnim = true;
	}
	if (((int)(Distance*100) != (int)(InitDistance*100)) && (CanPlayAnim))
	{
		if (CurAnim == 1)
		{
			Commands->Set_Animation(obj,Get_Parameter("Animation1"),false,0,Get_Float_Parameter("FirstFrame1"),Get_Float_Parameter("LastFrame1"),false);
			CurAnim = 2;
		}
		else if (CurAnim == 2)
		{
			Commands->Set_Animation(obj,Get_Parameter("Animation2"),false,0,Get_Float_Parameter("FirstFrame2"),Get_Float_Parameter("LastFrame2"),false);
			CurAnim = 1;
		}
		CanPlayAnim = false;
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerNumber"));
}

void Ra2Ven_FireAnimation2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&InitDistance,4,1);
	Auto_Save_Variable(&CanPlayAnim,1,2);
	Auto_Save_Variable(&CurAnim,4,3);
}

void Ra2Ven_FireAnimation2::Animation_Complete(GameObject *obj,const char *animation_name)
{
	if ((!_stricmp(animation_name,Get_Parameter("Animation1"))) || (!_stricmp(animation_name,Get_Parameter("Animation2"))))
	{
		Commands->Set_Animation(obj,Get_Parameter("AfAnimation"),false,0,Get_Float_Parameter("AfFirstFrame"),Get_Float_Parameter("AfLastFrame"),false);
		CurAnim = 1;
		CanPlayAnim = true;
	}
	else if (!_stricmp(animation_name,Get_Parameter("AfAnimation")))
	{
		obj->As_PhysicalGameObj()->Clear_Animation();
	}
}

ScriptRegistrant<Ra2Ven_MirageTank> Ra2Ven_MirageTank_Registrant("Ra2Ven_MirageTank","Tree1_Preset:string,Tree2_Preset=none:string,Tree3_Preset=none:string,Tree4_Preset=none:string,Explosion_Preset:string,Time=1:float,TimerNumber:int");
ScriptRegistrant<Ra2Ven_VehicleFalling> Ra2Ven_VehicleFalling_Registrant("Ra2Ven_VehicleFalling","ZValueBeforeStartFallingMode:int=3,ImpactExplosion_Preset:string,Warhead:string,DammageMultiplier=10:int,Time=1:float,TimerNumber:int");
ScriptRegistrant<Ra2Ven_DemoTruck> Ra2Ven_DemoTruck_Registrant("Ra2Ven_DemoTruck","Dammage:float,Warhead:string,Demo_Explosion:string");
ScriptRegistrant<Ra2Ven_Deployable_Vehicle> Ra2Ven_Deployable_Vehicle_Registrant("Ra2Ven_Deployable_Vehicle","Animation_Preset:string,oldTnk_Warhead:string,oldTnk_Dammage:float,Explosion_preset:string");
ScriptRegistrant<Ra2Ven_Deploy_Animation> Ra2Ven_Deploy_Animation_Registrant("Ra2Ven_Deploy_Animation","Deployed_Tank_Preset:string,oldTnk_Warhead:string,oldTnk_Dammage:float,Animation_Time:float,TimerNumber:int");
ScriptRegistrant<Ra2Ven_RandomTree> Ra2Ven_RandomTree_Registrant("Ra2Ven_RandomTree","Tree1_Preset:string,Tree2_Preset=none:string,Tree3_Preset=none:string,Tree4_Preset=none:string");
ScriptRegistrant<Ra2Ven_TurretSound> Ra2Ven_TurretSound_Registrant("Ra2Ven_TurretSound","TurretSoundObj:string");
ScriptRegistrant<Ra2Ven_TurretSoundObj> Ra2Ven_TurretSoundObj_Registrant("Ra2Ven_TurretSoundObj","RotationAngleLimit:int,Rotate_3dSound:string,Explosion_Preset:string,Time=0.5:float,TimerNumber:int,Wav_Length:float,TimerNumber2:int");
ScriptRegistrant<Ra2Ven_OccupentWeapon> Ra2Ven_OccupentWeapon_Registrant("Ra2Ven_OccupentWeapon","Character1:string,PowerUp1:string,Weapon1:string,Character2:string,PowerUp2:string,Weapon2:string,Character3:string,PowerUp3:string,Weapon3:string,Character4:string,PowerUp4:string,Weapon4:string,DefPowerUp:string,DefWeapon:string");
ScriptRegistrant<Ra2Ven_Dplbl_Vhcls_Cursor> Ra2Ven_Dplbl_Vhcls_Cursor_Registrant("Ra2Ven_Dplbl_Vhcls_Cursor","Cursor_preset:string,CursorBoneName:string,CursorExplosion:string,Animation_Preset:string,oldTnk_Warhead:string,oldTnk_Dammage:float,Explosion_preset:string");
ScriptRegistrant<Ra2Ven_FireAnimation> Ra2Ven_FireAnimation_Registrant("Ra2Ven_FireAnimation","ParentBone:string,MuzzleBone:string,Animation:string,FirstFrame=0:float,LastFrame=0:float,AfAnimation=none:string,AfFirstFrame=0:float,AfLastFrame=0:float,Time=0.1:float,TimerNumber:int");
ScriptRegistrant<Ra2Ven_FireAnimation2> Ra2Ven_FireAnimation2_Registrant("Ra2Ven_FireAnimation2","ParentBone:string,MuzzleBone:string,Animation1:string,FirstFrame1=0:float,LastFrame1=0:float,Animation2:string,FirstFrame2=0:float,LastFrame2=0:float,AfAnimation:string,AfFirstFrame=0:float,AfLastFrame=0:float,Time=0.1:float,TimerNumber:int");
