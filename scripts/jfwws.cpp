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
#include "jfwws.h"
#include "engine_tt.h"

void JFW_Building_Explode_No_Damage::Killed(GameObject *obj,GameObject *killer)
{
	Vector3 pos = Commands->Get_Position(obj);
	Commands->Shake_Camera(pos,(float)25.0,(float)0.1,(float)4.0);
	Destroy_Script();
}

void JFW_Viceroid_Innate::Created(GameObject *obj)
{
	Commands->Set_Loiters_Allowed(obj,false);
	Commands->Set_Animation(obj,"C_Visceroid.C_Visceroid",true,0,0,-1.0,false);
}

void JFW_Viceroid_Innate::Killed(GameObject *obj,GameObject *killer)
{
	Commands->Set_Animation(obj,"C_Visceroid.C_Visceroid_Die",false,0,0,-1.0,false);
}

void JFW_No_Innate::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);
	Destroy_Script();
}

void JFW_Credit_Trickle::Created(GameObject *obj)
{
	float delay;
	delay = Get_Float_Parameter("Delay");
	Commands->Start_Timer(obj,this,delay,667);
}

void JFW_Credit_Trickle::Timer_Expired(GameObject *obj,int number)
{
	if (number == 667)
	{
		Commands->Give_Money(obj,(float)Get_Int_Parameter("Credits"),true);
		float delay;
		delay = Get_Float_Parameter("Delay");
		Commands->Start_Timer(obj,this,delay,667);
	}
}

void JFW_PCT_Pokable::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
	Commands->Set_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj));
}

void JFW_PCT_Pokable::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object = Commands->Create_Object(Commands->Get_Preset_Name(obj),Commands->Get_Position(obj));
	Commands->Set_Facing(object,Commands->Get_Facing(obj));
}

void JFW_PCT_Pokable::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	Commands->Display_Health_Bar(obj,false);
}

void JFW_Vehicle_Regen::Created(GameObject *obj)
{
	Commands->Send_Custom_Event(obj,obj,0,0,0);
}

void JFW_Vehicle_Regen::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (!type)
	{
		float health = Commands->Get_Health(obj);
		float maxhealth = Commands->Get_Max_Health(obj);
		if (health < maxhealth)
		{
			Commands->Apply_Damage(obj,-2.0,"RegenHealth",0);
		}
		Commands->Send_Custom_Event(obj,obj,0,0,1.0);
	}
}

void JFW_Disable_Transition::Created(GameObject *obj)
{
	Commands->Enable_Vehicle_Transitions(obj,false);
	Destroy_Script();
}

void JFW_Enable_Transition::Created(GameObject *obj)
{
	Commands->Enable_Vehicle_Transitions(obj, true);
	Destroy_Script();
}

void JFW_Permanent_No_Falling_Damage::Created(GameObject *obj)
{
	health = Commands->Get_Health(obj);
}

void JFW_Permanent_No_Falling_Damage::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (!amount)
	{
		Commands->Set_Health(obj,health);
	}
}

void JFW_Permanent_No_Falling_Damage::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_FALLING_DAMAGE)
	{
		health = Commands->Get_Health(obj);
	}
}

void JFW_Permanent_No_Falling_Damage::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&health,4,1);
}

void JFW_Death_Powerup::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if ((type == CUSTOM_EVENT_POWERUP_GRANTED) && (sender))
	{
		Commands->Apply_Damage(sender,10000,"Death",0);
	}
}

void JFW_GrantPowerup_Created::Created(GameObject *obj)
{
	Commands->Give_PowerUp(obj,Get_Parameter("WeaponDef"),false);
	Destroy_Script();
}

void JFW_CnC_Crate::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if ((type == CUSTOM_EVENT_POWERUP_GRANTED) && (sender))
	{
		Commands->Give_Money(sender,100,false);
	}
}

void JFW_Tiberium_Refinery::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,(float)Get_Int_Parameter("TimerLength"),1);
}

void JFW_Tiberium_Refinery::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (Commands->Get_Health(obj))
		{
			Commands->Give_Money(obj,(float)Get_Int_Parameter("MoneyAmount"),true);
			Commands->Start_Timer(obj,this,(float)Get_Int_Parameter("TimerLength"),1);
		}
	}
}

void JFW_GrantMoney_Powerup::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	float scoreamount = Get_Float_Parameter("ScoreAmount");
	bool entireteam = Get_Bool_Parameter("Entire_Team");
	int randomizer = Get_Int_Parameter("Randomizer");
	if (!randomizer)
	{
		randomizer = 1;
	}
	randomizer++;
	int random = Commands->Get_Random_Int(1,randomizer);
	float amount = scoreamount*random;
	Commands->Give_Money(sender,amount,entireteam);
}

void JFW_GrantScore_Powerup::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	float scoreamount = Get_Float_Parameter("ScoreAmount");
	bool entireteam = Get_Bool_Parameter("Entire_Team");
	int randomizer = Get_Int_Parameter("Randomizer");
	if (!randomizer)
	{
		randomizer = 1;
	}
	randomizer++;
	int random = Commands->Get_Random_Int(1,randomizer);
	float amount = scoreamount*random;
	Commands->Give_Points(sender,amount,entireteam);
}

void JFW_Disable_Physical_Collision::Created(GameObject *obj)
{
	Commands->Disable_Physical_Collisions(obj);
	Destroy_Script();
}

void JFW_Enable_Physical_Collision::Created(GameObject *obj)
{
	Commands->Enable_Collisions(obj);
	Destroy_Script();
}

void JFW_DestroyedStateObject::Destroyed(GameObject *obj)
{
	GameObject *object = Commands->Create_Object(Get_Parameter("DestroyedModelPreset"),Commands->Get_Position(obj));
	Commands->Set_Facing(object,Get_Float_Parameter("OriginalModelFacing"));
}

void JFW_Play_Sound::Created(GameObject *obj)
{
	if (Get_Int_Parameter("Frequency_Min") == -1)
	{
		Timer_Expired(obj,0);
	}
	else
	{
		float random = Commands->Get_Random(Get_Float_Parameter("Frequency_Min"),Get_Float_Parameter("Frequency_Max"));
		Commands->Start_Timer(obj,this,random,0);
	}
}

void JFW_Play_Sound::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if ((Get_Int_Parameter("Frequency_Min") != -1) && (type == CUSTOM_EVENT_SOUND_ENDED))
	{
		float random = Commands->Get_Random(Get_Float_Parameter("Frequency_Min"),Get_Float_Parameter("Frequency_Max"));
		if(!Has_Timer(obj,this,0))
		{
			Commands->Start_Timer(obj,this,random,0);
		}
	}
}

void JFW_Play_Sound::Timer_Expired(GameObject *obj,int number)
{
	const char *soundpreset = Get_Parameter("Sound_Preset");
	bool is3d = Get_Bool_Parameter("Is_3D");
	int sound;
	Vector3 position = Commands->Get_Position(obj);
	Vector3 position2 = Get_Vector3_Parameter("Offset");
	position.X += position2.X;
	position.Y += position2.Y;
	position.Z += position2.Z;
	Vector3 random = Get_Vector3_Parameter("Offset_Randomness");
	position.X += Commands->Get_Random(-random.X,random.X);
	position.Y += Commands->Get_Random(-random.Y,random.Y);
	position.Z += Commands->Get_Random(-random.Z,random.Z);
	if (is3d)
	{
		sound = Commands->Create_Sound(soundpreset,position,obj);
	}
	else
	{
		sound = Commands->Create_2D_Sound(soundpreset);
	}
	Commands->Monitor_Sound(obj,sound);
}

void JFW_Engine_Sound::Created(GameObject *obj)
{
	sound = Commands->Create_3D_Sound_At_Bone(Get_Parameter("Preset"),obj,Get_Parameter("Bone"));
}

void JFW_Engine_Sound::Destroyed(GameObject *obj)
{
	Commands->Stop_Sound(sound,true);
}

void JFW_Engine_Sound::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&sound,4,1);
}

void JFW_Mobius_Script::Created(GameObject *obj)
{
	follower = 0;
	Commands->Start_Timer(obj,this,0.5,777);
}

void JFW_Mobius_Script::Timer_Expired(GameObject *obj,int number)
{
	if (number == 777)
	{
		Commands->Innate_Disable(obj);
		Vector3 v = Commands->Get_Position(obj);
		GameObject *object = Commands->Find_Closest_Soldier(v,(float)0.1,(float)2.0,true);
		if ((object) && (object != Commands->Find_Object(follower)))
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,100);
			params.Set_Movement(object,1.0,1.0);
			params.MoveFollow = true;
			Commands->Action_Goto(obj,params);
			follower = Commands->Get_ID(object);
		}
		Commands->Start_Timer(obj,this,0.5,777);
	}
}

void JFW_Mobius_Script::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&follower,4,1);
}

void JFW_HealthMedal_TextMessage::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Vector3 v;
		v.X = (float)0.19599999;
		v.Y = (float)0.88200003;
		v.Z = (float)0.19599999;
		Commands->Set_HUD_Help_Text(7669,v);
	}
}

void JFW_ArmorMedal_TextMessage::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Vector3 v;
		v.X = (float)0.19599999;
		v.Y = (float)0.88200003;
		v.Z = (float)0.19599999;
		Commands->Set_HUD_Help_Text(7670,v);
	}
}

void JFW_C130_Explosion::Killed(GameObject *obj,GameObject *killer)
{
	Commands->Create_Explosion_At_Bone("Explosion_Cargo_Plane",obj,"BODYMAIN",0);
}

void JFW_Send_Object_ID::Created(GameObject *obj)
{
	int recieverid = Get_Int_Parameter("Receiver_ID");
	int param = Get_Int_Parameter("Param");
	float delay = Get_Float_Parameter("Delay");
	if (Commands->Find_Object(recieverid))
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(recieverid),9035,param,delay);
	}
	Destroy_Script();
}

void JFW_Ion_Cannon_Sound::Created(GameObject *obj)
{
	int number = Get_Int_Parameter("Number");
	Vector3 v = Commands->Get_Position(obj);
	if (!number)
	{
		Commands->Create_Sound("Ion_Cannon_Buildup",v,obj);
	}
	else
	{
		Commands->Create_Sound("Ion_Cannon_Fire",v,obj);
	}
	Destroy_Script();
}

void JFW_NukeStrike_Anim::Created(GameObject *obj)
{
	Commands->Send_Custom_Event(obj,obj,1,0,6.0);
	Commands->Send_Custom_Event(obj,obj,2,0,8.0);
	Commands->Send_Custom_Event(obj,obj,3,0,11.0);
	Vector3 v = Commands->Get_Position(obj);
	GameObject *object = Commands->Create_Object("Generic_Cinematic",v);
	if (object)
	{
		Commands->Set_Model(obj,"XG_AG_Nuke");
		Commands->Attach_Script(obj,"JFW_PlayAnimation_DestroyObject","Nuke_Missle.Nuke_Missle");
		Commands->Create_3D_Sound_At_Bone("SFX.Nuclear_Strike_Buildup",obj,"ROOTTRANSFORM");
	}
}

void JFW_NukeStrike_Anim::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 1)
	{
		Vector3 v = Commands->Get_Position(obj);
		GameObject *object = Commands->Create_Object("Generic_Cinematic",v);
		if (object)
		{
			Commands->Set_Model(obj,"XG_AG_Nukecloud");
			Commands->Attach_Script(obj,"JFW_PlayAnimation_DestroyObject","Nuke_cloud.Nuke_cloud");
		}
		Commands->Shake_Camera(v,2.0,0.5,2.0);
	}
	if (type == 2)
	{
		Vector3 v = Commands->Get_Position(obj);
		Commands->Shake_Camera(v,2.0,1.0,3.0);
	}
	if (type == 3)
	{
		Vector3 v = Commands->Get_Position(obj);
		GameObject *object = Commands->Create_Object("Generic_Cinematic",v);
		if (object)
		{
			Commands->Set_Model(obj,"XG_AG_Nukecloud");
			Commands->Attach_Script(obj,"JFW_PlayAnimation_DestroyObject","XG_Nukecloud_02");
		}
	}
}

void JFW_PlayAnimation_DestroyObject::Created(GameObject *obj)
{
	Commands->Set_Animation(obj,Get_Parameter("AnimationName"),false,0,0,-1.0,false);
}

void JFW_PlayAnimation_DestroyObject::Animation_Complete(GameObject *obj,const char *animation_name)
{
	Commands->Destroy_Object(obj);
}

void JFW_BuildingStateSoundSpeaker::Created(GameObject *obj)
{
	destroyed = false;
	explode = true;
	GameObject *object = Commands->Find_Object(Get_Int_Parameter("BuildingController_ID"));
	if (object)
	{
		char s[18];
		sprintf(s,"%d",Commands->Get_ID(obj));
		Commands->Attach_Script(object,"JFW_BuildingStateSoundController",s);
	}
	if (Get_Int_Parameter("Frequency_Min") == -1)
	{
		Timer_Expired(obj,0);
	}
	else
	{
		float random = Commands->Get_Random(Get_Float_Parameter("Frequency_Min"),Get_Float_Parameter("Frequency_Max"));
		Commands->Start_Timer(obj,this,random,0);
	}
}

void JFW_BuildingStateSoundSpeaker::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	switch (type)
	{
	case CUSTOM_EVENT_SOUND_ENDED:
		if (destroyed)
		{
			if (Get_Int_Parameter("Frequency_Min_Destroyed") != -1)
			{
				float min = Get_Float_Parameter("Frequency_Min_Destroyed");
				float max = Get_Float_Parameter("Frequency_Max_Destroyed");
				float frequency = Commands->Get_Random(min,max);
				Commands->Start_Timer(obj,this,frequency,0);
			}
		}
		else
		{
			if (Get_Int_Parameter("Frequency_Min") != -1)
			{
				float min = Get_Float_Parameter("Frequency_Min");
				float max = Get_Float_Parameter("Frequency_Max");
				float frequency = Commands->Get_Random(min,max);
				Commands->Start_Timer(obj,this,frequency,0);
			}
		}
		break;
	case 9026:
		if (param == 1)
		{
			Commands->Stop_Sound(sound,1);
			Timer_Expired(obj,0);
		}
		break;
	case 9027:
		if (explode)
		{
			Vector3 v = Commands->Get_Position(obj);
			v.X += Commands->Get_Random(2.0,5.0);
			v.Y += Commands->Get_Random(2.0,5.0);
			v.Z += Commands->Get_Random(2.0,5.0);
			Commands->Create_Explosion(Get_Parameter("Explosion_Name"),v,0);
			Commands->Send_Custom_Event(obj,obj,9027,1,Commands->Get_Random(3.0,6.0));
		}
		break;
	case 9028:
		explode = false;
		break;
	case 9029:
		explode = true;
		break;
	}
}

void JFW_BuildingStateSoundSpeaker::Timer_Expired(GameObject *obj,int number)
{
	if (destroyed)
	{
		bool is3d = Get_Bool_Parameter("Is_3D_Destroyed");
		Vector3 pos = Commands->Get_Position(obj);
		Vector3 offset = Get_Vector3_Parameter("Offset_Destroyed");
		pos.X += offset.X;
		pos.Y += offset.Y;
		pos.Z += offset.Z;
		offset = Get_Vector3_Parameter("Offset_Randomness_Destroyed");
		pos.X += Commands->Get_Random(-offset.X,offset.X);
		pos.Y += Commands->Get_Random(-offset.Y,offset.Y);
		pos.Z += Commands->Get_Random(-offset.Z,offset.Z);
		if (is3d)
		{
			sound = Commands->Create_Sound(Get_Parameter("Sound_Destroyed"),pos,obj);
		}
		else
		{
			sound = Commands->Create_2D_Sound(Get_Parameter("Sound_Destroyed"));
		}
	}
	else
	{
		bool is3d = Get_Bool_Parameter("Is_3D");
		Vector3 pos = Commands->Get_Position(obj);
		Vector3 offset = Get_Vector3_Parameter("Offset");
		pos.X += offset.X;
		pos.Y += offset.Y;
		pos.Z += offset.Z;
		offset = Get_Vector3_Parameter("Offset_Randomness");
		pos.X += Commands->Get_Random(-offset.X,offset.X);
		pos.Y += Commands->Get_Random(-offset.Y,offset.Y);
		pos.Z += Commands->Get_Random(-offset.Z,offset.Z);
		if (is3d)
		{
			sound = Commands->Create_Sound(Get_Parameter("Sound_Normal"),pos,obj);
		}
		else
		{
			sound = Commands->Create_2D_Sound(Get_Parameter("Sound_Normal"));
		}
	}
	Commands->Monitor_Sound(obj,sound);
}

void JFW_BuildingStateSoundSpeaker::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&sound,4,1);
	Auto_Save_Variable(&destroyed,1,3);
}

void JFW_BuildingStateSoundController::Created(GameObject *obj)
{
	speakerid = Get_Int_Parameter("BuildingSpeaker_ID");
}

void JFW_BuildingStateSoundController::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object = Commands->Find_Object(speakerid);
	if (object)
	{
		Commands->Send_Custom_Event(obj,object,9026,1,0);
		Commands->Send_Custom_Event(obj,object,9027,1,0);
	}
}

void JFW_BuildingStateSoundController::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 9028)
	{
		GameObject *object = Commands->Find_Object(speakerid);
		if (object)
		{
			Commands->Send_Custom_Event(obj,object,9028,1,0);
		}
	}
	if (type == 9029)
	{
		GameObject *object = Commands->Find_Object(speakerid);
		if (object)
		{
			Commands->Send_Custom_Event(obj,object,9029,1,0);
		}
	}
}

void JFW_BuildingStateSoundController::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&speakerid,4,1);
}

void JFW_Play_Sound_Object_Bone::Created(GameObject *obj)
{
	if (Get_Int_Parameter("Frequency_Min") == -1)
	{
		Timer_Expired(obj,0);
	}
	else
	{
		float random = Commands->Get_Random(Get_Float_Parameter("Frequency_Min"),Get_Float_Parameter("Frequency_Max"));
		Commands->Start_Timer(obj,this,random,0);
	}
}

void JFW_Play_Sound_Object_Bone::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_SOUND_ENDED)
	{
		if (Get_Int_Parameter("Frequency_Min") != -1)
		{
			float random = Commands->Get_Random(Get_Float_Parameter("Frequency_Min"),Get_Float_Parameter("Frequency_Max"));
			Commands->Start_Timer(obj,this,random,0);
		}
	}
}

void JFW_Play_Sound_Object_Bone::Timer_Expired(GameObject *obj,int number)
{
	const char *preset = Get_Parameter("Sound_Preset");
	int sound = Commands->Create_3D_Sound_At_Bone(preset,obj,"ROOTTRANSFORM");
	Commands->Monitor_Sound(obj,sound);
}

void JFW_Cinematic_Primary_Killed::Created(GameObject *obj)
{
	killed = false;
}

void JFW_Cinematic_Primary_Killed::Killed(GameObject *obj,GameObject *killer)
{
	if (!killed)
	{
		killed = true;
		GameObject *object = Commands->Find_Object(Get_Int_Parameter("CallbackID"));
		if (object)
		{
			Commands->Send_Custom_Event(obj,object,9023,0,0);
		}
	}
}

void JFW_Cinematic_Primary_Killed::Destroyed(GameObject *obj)
{
	if (!killed)
	{
		killed = true;
		GameObject *object = Commands->Find_Object(Get_Int_Parameter("CallbackID"));
		if (object)
		{
			Commands->Send_Custom_Event(obj,object,9023,0,0);
		}
	}
}

void JFW_Cinematic_Primary_Killed::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&killed,1,1);
}

void JFW_Disable_Loiter::Created(GameObject *obj)
{
	Commands->Set_Loiters_Allowed(obj,false);
}

void JFW_Disable_Loiter::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == 9036)
	{
		Commands->Set_Loiters_Allowed(obj,true);
	}
}

void JFW_InnateIsStationary::Created(GameObject *obj)
{
	Commands->Set_Innate_Is_Stationary(obj,true);
	Destroy_Script();
}

void JFW_Generic_Conv::Created(GameObject *obj)
{
	int convid = Commands->Create_Conversation(Get_Parameter("ConvName"),99,2000,false);
	Commands->Join_Conversation(obj,convid,true,true,true);
	Commands->Start_Conversation(convid,100000);
	Commands->Monitor_Conversation(obj,convid);
	Destroy_Script();
}

void JFW_Disable_Hibernation::Created(GameObject *obj)
{
	Commands->Enable_Hibernation(obj,0);
	Destroy_Script();
}

ScriptRegistrant<JFW_Building_Explode_No_Damage> JFW_Building_Explode_No_Damage_Registrant("JFW_Building_Explode_No_Damage","");
ScriptRegistrant<JFW_Viceroid_Innate> JFW_Viceroid_Innate_Registrant("JFW_Viceroid_Innate","");
ScriptRegistrant<JFW_No_Innate> JFW_No_Innate_Registrant("JFW_No_Innate","");
ScriptRegistrant<JFW_Credit_Trickle> JFW_Credit_Trickle_Registrant("JFW_Credit_Trickle","Credits=1:int,Delay=2.0:float");
ScriptRegistrant<JFW_PCT_Pokable> JFW_PCT_Pokable_Registrant("JFW_PCT_Pokable","");
ScriptRegistrant<JFW_PCT_Pokable> M00_PCT_Pokable_Registrant("M00_PCT_Pokable_DAK","");
ScriptRegistrant<JFW_Vehicle_Regen> JFW_Vechicle_Regen_Registrant("JFW_Vechicle_Regen","");
ScriptRegistrant<JFW_Disable_Transition> JFW_Disable_Transition_Registrant("JFW_Disable_Transition","");
ScriptRegistrant<JFW_Enable_Transition> JFW_Enable_Transition_Registrant("JFW_Enable_Transition", "");
ScriptRegistrant<JFW_Disable_Transition> M00_Disable_Transition_Registrant("M00_Disable_Transition", "");
ScriptRegistrant<JFW_Permanent_No_Falling_Damage> JFW_Permanent_No_Falling_Damage_Registrant("JFW_Permanent_No_Falling_Damage","");
ScriptRegistrant<JFW_Death_Powerup> JFW_Death_Powerup_Registrant("JFW_Death_Powerup","");
ScriptRegistrant<JFW_GrantPowerup_Created> JFW_GrantPowerup_Created_Registrant("JFW_GrantPowerup_Created","WeaponDef:string");
ScriptRegistrant<JFW_GrantPowerup_Created> M00_GrantPowerup_Created_Registrant("M00_GrantPowerup_Created","WeaponDef:string");
ScriptRegistrant<JFW_CnC_Crate> JFW_CnC_Crate_Registrant("JFW_CnC_Crate","");
ScriptRegistrant<JFW_Tiberium_Refinery> JFW_Tiberium_Refinery_Registrant("JFW_Tiberium_Refinery","MoneyAmount:int,TimerLength:int");
ScriptRegistrant<JFW_GrantMoney_Powerup> JFW_GrantMoney_Powerup_Registrant("JFW_GrantMoney_Powerup","ScoreAmount:float,Entire_Team=0:int,Randomizer=1:int");
ScriptRegistrant<JFW_GrantScore_Powerup> JFW_GrantScore_Powerup_Registrant("JFW_GrantScore_Powerup","ScoreAmount:float,Entire_Team=0:int,Randomizer=1:int");
ScriptRegistrant<JFW_Disable_Physical_Collision> JFW_Disable_Physical_Collision_Registrant("JFW_Disable_Physical_Collision","");
ScriptRegistrant<JFW_Enable_Physical_Collision> JFW_Enable_Physical_Collision_Registrant("JFW_Enable_Physical_Collision","");
ScriptRegistrant<JFW_DestroyedStateObject> JFW_DestroyedStateObject_Registrant("JFW_DestroyedStateObject","OriginalModelFacing:float,DestroyedModelPreset:string");
ScriptRegistrant<JFW_Play_Sound> JFW_Play_Sound_Registrant("JFW_Play_Sound","Sound_Preset:string,Is_3D=1:int,Offset:vector3,Offset_Randomness:vector3,Frequency_Min=-1:float,Frequency_Max:float");
ScriptRegistrant<JFW_Play_Sound> M00_Play_Sound_Registrant("M00_Play_Sound","Sound_Preset:string,Is_3D=1:int,Offset:vector3,Offset_Randomness:vector3,Frequency_Min=-1:float,Frequency_Max:float");
ScriptRegistrant<JFW_Engine_Sound> JFW_Engine_Sound_Registrant("JFW_Engine_Sound","Preset:string,Bone:string");
ScriptRegistrant<JFW_Mobius_Script> JFW_Mobius_Script_Registrant("JFW_Mobius_Script","");
ScriptRegistrant<JFW_Mobius_Script> Dr_Mobius_Script_Registrant("Dr_Mobius_Script","");
ScriptRegistrant<JFW_ArmorMedal_TextMessage> JFW_ArmorMedal_TextMessage_Registrant("JFW_ArmorMedal_TextMessage","");
ScriptRegistrant<JFW_HealthMedal_TextMessage> JFW_HealthMedal_TextMessage_Registrant("JFW_HealthMedal_TextMessage","");
ScriptRegistrant<JFW_C130_Explosion> JFW_C130_Explosion_Registrant("JFW_C130_Explosion","");
ScriptRegistrant<JFW_Send_Object_ID> JFW_Send_Object_ID_Registrant("JFW_Send_Object_ID","Receiver_ID:int,Param=0:int,Delay=1.0f:int");
ScriptRegistrant<JFW_Ion_Cannon_Sound> JFW_Ion_Cannon_Sound_Registrant("JFW_Ion_Cannon_Sound","Number=0:int");
ScriptRegistrant<JFW_NukeStrike_Anim> JFW_NukeStrike_Anim_Registrant("JFW_NukeStrike_Anim","");
ScriptRegistrant<JFW_PlayAnimation_DestroyObject> JFW_PlayAnimation_DestroyObject_Registrant("JFW_PlayAnimation_DestroyObject","AnimationName:string");
ScriptRegistrant<JFW_BuildingStateSoundSpeaker> JFW_BuildingStateSoundSpeaker_Registrant("JFW_BuildingStateSoundSpeaker","Sound_Normal:string,Sound_Destroyed:string,BuildingController_ID:int,Is_3D=1:int,Offset:vector3,Offset_Randomness:vector3,Frequency_Min=-1:float,Frequency_Max:float,Is_3D_Destroyed=1:int,Offset_Destroyed:vector3,Offset_Randomness_Destroyed:vector3,Frequency_Min_Destroyed=-1:float,Frequency_Max_Destroyed:float,Explosion_Name:string");
ScriptRegistrant<JFW_BuildingStateSoundController> JFW_BuildingStateSoundController_Registrant("JFW_BuildingStateSoundController","BuildingSpeaker_ID:int");
//ScriptRegistrant<JFW_BuildingStateSoundSpeaker> M00_BuildingStateSoundSpeaker_Registrant("M00_BuildingStateSoundSpeaker","Sound_Normal:string,Sound_Destroyed:string,BuildingController_ID:int,Is_3D=1:int,Offset:vector3,Offset_Randomness:vector3,Frequency_Min=-1:float,Frequency_Max:float,Is_3D_Destroyed=1:int,Offset_Destroyed:vector3,Offset_Randomness_Destroyed:vector3,Frequency_Min_Destroyed=-1:float,Frequency_Max_Destroyed:float,Explosion_Name:string");
ScriptRegistrant<JFW_BuildingStateSoundController> M00_BuildingStateSoundController_Registrant("M00_BuildingStateSoundController","BuildingSpeaker_ID:int");
ScriptRegistrant<JFW_Play_Sound_Object_Bone> JFW_Play_Sound_Object_Bone_Registrant("JFW_Play_Sound_Object_Bone","Sound_Preset:string,Frequency_Min=-1.0:float,Frequency_Max:float");
ScriptRegistrant<JFW_Cinematic_Primary_Killed> JFW_Cinematic_Primary_Killed_Registrant("JFW_Cinematic_Primary_Killed","CallbackID=:int");
ScriptRegistrant<JFW_Disable_Loiter> JFW_Disable_Loiter_Registrant("JFW_Disable_Loiter","");
ScriptRegistrant<JFW_InnateIsStationary> JFW_InnateIsStationary_Registrant("JFW_InnateIsStationary","");
ScriptRegistrant<JFW_Generic_Conv> JFW_Generic_Conv_Registrant("JFW_Generic_Conv","ConvName:string");
ScriptRegistrant<JFW_Disable_Hibernation> JFW_Disable_Hibernation_Registrant("JFW_Disable_Hibernation","");
