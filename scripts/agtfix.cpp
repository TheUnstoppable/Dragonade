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
#include "agtfix.h"
#include "engine_game.h"
#include "BuildingGameObj.h"
#include "BuildingAggregateClass.h"
	
void GDI_AGT::Created(GameObject* AGTObj)
{
	Commands->Enable_Hibernation(AGTObj,false);
	if (Commands->Get_Building_Power(AGTObj)) 
	{
		Create_Guns(AGTObj);
	}
}


void GDI_AGT::Killed(GameObject* AGTObj, GameObject* KillerObj)
{
	Destroy_Guns(AGTObj);
}

void GDI_AGT::Custom(GameObject* AGTObj, int Type, int Param, GameObject* Sender) 
{
	if (Type == CUSTOM_EVENT_BUILDING_POWER_CHANGED)
	{
		if (!Param) 
		{
			Destroy_Guns(AGTObj);
		}
		else 
		{
			Create_Guns(AGTObj);
		}
	}
	else if (Type == CUSTOM_EVENT_BUILDING_REVIVED)
	{
		if (Commands->Get_Building_Power(AGTObj)) 
		{
			Create_Guns(AGTObj);
		}
	}
}

void GDI_AGT::Create_Guns(GameObject* AGTObj) 
{
	Destroy_Guns(AGTObj);

	Vector3 AGTPos;
	Vector3 MissilePos;
	Vector3 GunPos[4];

	AGTPos = Commands->Get_Position(AGTObj);

	float AGTFacing=0;
	float AGTHeight=0;
	BuildingGameObj *AGTBuilding = AGTObj->As_BuildingGameObj();
	BuildingAggregateClass *AGTMCT = 0;
	if (AGTBuilding)
	{
		AGTMCT = AGTBuilding->Find_MCT();
		if (AGTMCT)
		{
			Vector3 MCTPos;
			AGTFacing = AGTMCT->Get_Facing();
			AGTMCT->Get_Position(&MCTPos);
			AGTHeight=MCTPos.Z+8.0f;
		}
		
	}

	//Calculations for setting coordinates at different angles.
	float CosFacing = cos(AGTFacing);
	float SinFacing = sin(AGTFacing);

	//Check if single floor AGT, if not fall back to using building controller height.
	if(AGTPos.Z - AGTHeight < 11 && AGTMCT)
		AGTPos.Z=AGTHeight+9.2f;

	//MissilePosition
	MissilePos.X = AGTPos.X;
	MissilePos.Y = AGTPos.Y;
	MissilePos.Z = AGTPos.Z + 4.0f;

	//GunPosition 1
	GunPos[0] = AGTPos;
	//X Offsets
	GunPos[0].X += 5.0189999f * CosFacing;
	GunPos[0].Y += 5.0189999f * SinFacing;
	//Y Offsets
	GunPos[0].Y += -3.4089998f * CosFacing;
	GunPos[0].X += 3.4089998f * SinFacing;
	//Z offset
	GunPos[0].Z = AGTPos.Z - 9.0f;

	//GunPosition 2
	GunPos[1] = AGTPos;
	//X Offsets
	GunPos[1].X += 5.0109998f * CosFacing;
	GunPos[1].Y += 5.0109998f * SinFacing;
	//Y Offsets
	GunPos[1].Y += 3.6720001f * CosFacing;
	GunPos[1].X += -3.6720001f * SinFacing;
	//Z offset
	GunPos[1].Z = AGTPos.Z - 9.0f;

	//GunPosition 3
	GunPos[2] = AGTPos;
	//X Offsets
	GunPos[2].X += -5.0110002f * CosFacing;
	GunPos[2].Y += -5.0110002f * SinFacing;
	//Y Offsets
	GunPos[2].Y += 3.657f * CosFacing;
	GunPos[2].X += -3.657f * SinFacing;
	//Z offset
	GunPos[2].Z = AGTPos.Z - 9.0f;

	//GunPosition 4
	GunPos[3] = AGTPos;
	//X Offsets
	GunPos[3].X += -5.0129999f * CosFacing;
	GunPos[3].Y += -5.0129999f * SinFacing;
	//Y Offsets
	GunPos[3].Y += -3.4060002f * CosFacing;
	GunPos[3].X += 3.4060002f * SinFacing;
	//Z offset
	GunPos[3].Z = AGTPos.Z - 9.0f;

	GameObject* MissileObj = Commands->Create_Object("GDI_AGT", MissilePos);
	if (MissileObj) 
	{
		Commands->Attach_Script(MissileObj, "GDI_AGT_Missile", "0");
		MissileID = Commands->Get_ID(MissileObj);
	}
	
	for (int i = 0;i < 4;i++) 
	{
		GameObject* GunObj = Commands->Create_Object("GDI_Ceiling_Gun_AGT", GunPos[i]);
		if (GunObj) 
		{
			Commands->Attach_Script(GunObj, "GDI_AGT_Gun", "");
			Commands->Send_Custom_Event(AGTObj, GunObj, 0, MissileID, 0);
			GunID[i] = Commands->Get_ID(GunObj);
		}
	}
}

void GDI_AGT::Destroy_Guns(GameObject* AGTObj)
{
	for (int i = 0;i < 4;i++) 
	{
		Commands->Destroy_Object(Commands->Find_Object(GunID[i]));
	}
	Commands->Destroy_Object(Commands->Find_Object(MissileID));
}


void GDI_AGT_Gun::Created(GameObject* GunObj)
{
	EnemyID = NULL;
	
	Commands->Set_Shield_Type(GunObj, "Blamo");
	Commands->Enable_Hibernation(GunObj, false);
	Commands->Innate_Enable(GunObj);
	Commands->Enable_Enemy_Seen(GunObj, true);
}


void GDI_AGT_Gun::Destroyed(GameObject* GunObj)
{
	Commands->Action_Reset(GunObj, 100);
}


void GDI_AGT_Gun::Enemy_Seen(GameObject* GunObj, GameObject* EnemyObj)
{
	// Check for an living target which is in range
	if (!IsValidEnemy(GunObj, EnemyObj))
		return;

	// If the current enemy is still a valid target, ignore this enemy
	if (IsValidEnemy(GunObj, Commands->Find_Object(EnemyID)))
		return;
	
	// Attack!
	ActionParamsStruct ActionParams;
	ActionParams.Set_Basic(this, 100, 1);
	ActionParams.Set_Attack(EnemyObj, 300, 0, true);
	ActionParams.AttackCheckBlocked = false;
	Commands->Action_Attack(GunObj, ActionParams);

	Commands->Start_Timer(GunObj, this, .1f, 0);
	
	EnemyID = Commands->Get_ID(EnemyObj);
}


void GDI_AGT_Gun::Timer_Expired(GameObject* GunObj, int Number)
{
	switch (Number) {
		case 0:
			// If the current enemy is no longer a valid target, stop shooting,
			// otherwise restart the times
			if (!IsValidEnemy(GunObj, Commands->Find_Object(EnemyID))) {
				Commands->Action_Reset(GunObj, 100);
				EnemyID = NULL;
			} else {
				Commands->Start_Timer(GunObj, this, .1f, 0);
			}
			break;
	}
}


void GDI_AGT_Gun::Custom(GameObject* MissileObj, int type, int Param, GameObject* SenderObj)
{
	switch (type)
	{
		case 0:
			MissileID = Param;
			break;
	}
}



bool GDI_AGT_Gun::IsValidEnemy(GameObject* GunObj, GameObject* EnemyObj) {
	if (!EnemyObj)
		return false;
	
	// TODO: Make switch for agt kills neutral?
	if (Commands->Get_Player_Type(EnemyObj) != 0)
		return false;
	
	if (Commands->Get_Health(EnemyObj) <= 0)
		return false;
	
	if (!Commands->Is_Object_Visible(GunObj, EnemyObj))
		return false;

	if (Is_Harvester(EnemyObj))
		return false;
	
	Commands->Send_Custom_Event(GunObj, Commands->Find_Object(MissileID), 0, Commands->Get_ID(EnemyObj), 0);
	
	Vector3 GunObjPos = Commands->Get_Position(GunObj);
	Vector3 EnemyObjPos = Commands->Get_Position(EnemyObj);
	return Commands->Get_Distance(GunObjPos, EnemyObjPos) > 20;
}




void GDI_AGT_Missile::Created(GameObject* MissileObj)
{
	EnemyID = NULL;

	Commands->Set_Is_Rendered(MissileObj, false);
	Commands->Enable_Hibernation(MissileObj, false);
}


void GDI_AGT_Missile::Destroyed(GameObject* MissileObj)
{
	Commands->Action_Reset(MissileObj, 100);
}


void GDI_AGT_Missile::Timer_Expired(GameObject* MissileObj, int Number)
{
	switch (Number) {
		case 0:
			// If the current enemy is no longer a valid target, stop shooting,
			// otherwise restart the times
			if (!IsValidEnemy(MissileObj, Commands->Find_Object(EnemyID))) {
				Commands->Action_Reset(MissileObj, 100);
				EnemyID = NULL;
			} else {
				Commands->Start_Timer(MissileObj, this, .1f, 0);
			}
			break;
	}
}


void GDI_AGT_Missile::Custom(GameObject* MissileObj, int type, int Param, GameObject* SenderObj)
{
	switch (type)
	{
		case 0:
			// If the current enemy is still a valid target, ignore this enemy
			if (IsValidEnemy(MissileObj, Commands->Find_Object(EnemyID)))
				return;

			GameObject* EnemyObj = Commands->Find_Object(Param);

			// Check for an living target which is in range
			if (!IsValidEnemy(MissileObj, EnemyObj))
				return;
			
			// Attack!
			ActionParamsStruct ActionParams;
			ActionParams.Set_Basic(this, 100, 1);
			ActionParams.Set_Attack(EnemyObj, 300, 0, true);
			ActionParams.AttackCheckBlocked = false;
			Commands->Action_Attack(MissileObj, ActionParams);

			Commands->Start_Timer(MissileObj, this, .1f, 0);
			
			EnemyID = Param;
	}
}


bool GDI_AGT_Missile::IsValidEnemy(GameObject* MissileObj, GameObject* EnemyObj)
{
	if (!EnemyObj)
		return false;
	
	// TODO: Make switch for agt kills neutral?
	if (Commands->Get_Player_Type(EnemyObj) != 0)
		return false;
	
	if (Commands->Get_Health(EnemyObj) <= 0)
		return false;
	
	if (!Commands->Is_Object_Visible(MissileObj, EnemyObj))
		return false;

	if (Is_Harvester(EnemyObj))
		return false;
	
	Vector3 MissileObjPos = Commands->Get_Position(MissileObj);
	Vector3 EnemyObjPos = Commands->Get_Position(EnemyObj);

	return Commands->Get_Distance(MissileObjPos, EnemyObjPos) > 30;
}



ScriptRegistrant<GDI_AGT> M00_Advanced_Guard_Tower_Registrant("M00_Advanced_Guard_Tower", "");
ScriptRegistrant<GDI_AGT> GDI_AGT_Registrant("GDI_AGT", "");
ScriptRegistrant<GDI_AGT_Gun> GDI_AGT_Gun_Registrant("GDI_AGT_Gun", "");
ScriptRegistrant<GDI_AGT_Missile> GDI_AGT_Missile_Registrant("GDI_AGT_Missile", "");