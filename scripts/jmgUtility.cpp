
#include "general.h"

#include "JmgUtility.h"
#include "GameObjManager.h"
#include "VehicleGameObj.h"
#include "WeaponClass.h"
#include "VehicleGameObjDef.h"
#include "OffenseObjectClass.h"
#include "SoldierGameObj.h"
#include "BuildingGameObj.h"
#include "engine_player.h"
#include "DefinitionMgrClass.h"
#include "AudibleSoundDefinitionClass.h"
#include "TranslateDBClass.h"
bool JmgUtility::hasStatedDeathMessage[128] = {false};
void JMG_Utility_Check_If_Script_Is_In_Library::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Check_If_Script_Is_In_Library::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		if (!ScriptRegistrar::GetScriptFactory(Get_Parameter("ScriptName")))
		{
			char errMsg[220],pErrMsg[220];
			sprintf(errMsg,"ERROR: The Script %s was not found in scripts.dll. Please check your workspace and make sure you have included %s.cpp and its includes!",Get_Parameter("ScriptName"),Get_Parameter("CppName"));
			Console_Input(errMsg);
			sprintf(pErrMsg,"msg %s",errMsg);
			Console_Input(pErrMsg);
		}
	}
}
void JMG_Send_Custom_When_Custom_Sequence_Matched::Created(GameObject *obj)
{
	depth = 0;
	failCount = 0;
	enabled = Get_Int_Parameter("Starts_Enabled") == 0 ? false : true;
	if (!Get_Int_Parameter("Custom_0"))
	{
		Console_Input("msg JMG_Send_Custom_When_Custom_Sequence_Matched ERROR: Custom_0 must not be 0!");
		enabled = false;
	}
}
void JMG_Send_Custom_When_Custom_Sequence_Matched::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom_0") || message == Get_Int_Parameter("Custom_1") || message == Get_Int_Parameter("Custom_2") || message == Get_Int_Parameter("Custom_3") || message == Get_Int_Parameter("Custom_4") || message == Get_Int_Parameter("Custom_5") || message == Get_Int_Parameter("Custom_6") || message == Get_Int_Parameter("Custom_7") || message == Get_Int_Parameter("Custom_8") || message == Get_Int_Parameter("Custom_9"))
	{
		if (!enabled)
			return;
		char custom[32];
		sprintf(custom,"Custom_%0d",depth);
		bool correctStep = false;
		if (message == Get_Int_Parameter(custom))
		{
			correctStep = true;
			depth++;
		}
		else
		{
			if (!Get_Int_Parameter("Failure_Saftey") || depth)
			{
				failCount++;
				if (failCount < Get_Int_Parameter("Max_Failures"))
				{
					Send_Custom(sender,Get_Int_Parameter("Partial_Failure_Custom"),depth);
					return;
				}
				Send_Custom(sender,Get_Int_Parameter("Failure_Custom"),depth);
				if (Get_Int_Parameter("Disable_On_Failure"))
					enabled = false;
			}
			depth = 0;
			failCount = 0;
		}
		sprintf(custom,"Custom_%0d",depth);
		if (depth >= 10 || !Get_Int_Parameter(custom))
		{
			if (Get_Int_Parameter("Disable_On_Success"))
				enabled = false;
			depth = 0;
			failCount = 0;
			Send_Custom(sender,Get_Int_Parameter("Success_Custom"),param);
		}
		else if (correctStep && (!Get_Int_Parameter("Correct_Step_Saftey") || depth))
			Send_Custom(sender,Get_Int_Parameter("Correct_Step_Custom"),depth);
	}
	if (message == Get_Int_Parameter("Enable_Custom"))
	{
		if (enabled)
			return;
		depth = 0;
		failCount = 0;
		enabled = true;
		if (!Get_Int_Parameter("Custom_0"))
		{
			Console_Input("msg JMG_Send_Custom_When_Custom_Sequence_Matched ERROR: Custom_0 must not be 0!");
			enabled = false;
		}
	}
}
void JMG_Send_Custom_When_Custom_Sequence_Matched::Send_Custom(GameObject *obj,int custom,int param)
{
	GameObject *sendId = Commands->Find_Object(Get_Int_Parameter("Send_To_ID"));
	if (sendId)
		Commands->Send_Custom_Event(obj,sendId,custom,param,0);
}
void JMG_Utility_Change_Model_On_Timer::Created(GameObject *obj)
{
	if (Get_Float_Parameter("Time") == 0)
		Timer_Expired(obj,1);
	else
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
}
void JMG_Utility_Change_Model_On_Timer::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Set_Model(obj,Get_Parameter("Model"));
	}
}
void JMG_Utility_Emulate_DamageableStaticPhys::Created(GameObject *obj)
{
	team = Commands->Get_Player_Type(obj);
	alive = true;
	playingTwitch = false;
	playingTransition = false;
	Set_Death_Points(obj,0);
	Set_Damage_Points(obj,Get_Float_Parameter("Damage_Points"));
	Play_Animation(obj,true,Get_Float_Parameter("Live_Loop_Start"),Get_Float_Parameter("Live_Loop_End"));
}
void JMG_Utility_Emulate_DamageableStaticPhys::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damage)
		return;
	if (alive && !Commands->Get_Shield_Strength(obj))
	{
		alive = false;
		if (Get_Int_Parameter("Neutral_When_Dead"))
			Commands->Set_Player_Type(obj,-2);
		Set_Damage_Points(obj,Get_Float_Parameter("Damage_Points"));
		Play_Animation(obj,false,Get_Float_Parameter("Death_Transition_Start"),Get_Float_Parameter("Death_Transition_End"));
		playingTransition = true;
		GameObject *sendId = Commands->Find_Object(Get_Int_Parameter("Send_To_ID"));
		if (sendId)
			Commands->Send_Custom_Event(obj,sendId,Get_Int_Parameter("Death_Custom"),Commands->Get_ID(damager),0);
		if (_stricmp(Get_Parameter("Killed_Explosion"),"null"))
			Commands->Create_Explosion(Get_Parameter("Killed_Explosion"),Commands->Get_Position(obj),damager);
		if (damager)
			Commands->Give_Points(damager,(Commands->Get_Player_Type(obj) == Commands->Get_Player_Type(damager) ? -1 : 1) * Get_Float_Parameter("Death_Points"),0);
		if (Get_Int_Parameter("Disable_Collision_When_Dead"))
			Commands->Disable_Physical_Collisions(obj);
	}
	if (!alive && Commands->Get_Shield_Strength(obj) >= Commands->Get_Max_Shield_Strength(obj))
	{
		alive = true;
		Commands->Set_Player_Type(obj,team);
		Set_Damage_Points(obj,Get_Float_Parameter("Heal_Points"));
		Play_Animation(obj,false,Get_Float_Parameter("Live_Transition_Start"),Get_Float_Parameter("Live_Transition_End"));
		playingTransition = true;
		GameObject *sendId = Commands->Find_Object(Get_Int_Parameter("Send_To_ID"));
		if (sendId)
			Commands->Send_Custom_Event(obj,sendId,Get_Int_Parameter("Repaired_Custom"),Commands->Get_ID(damager),0);
		if (_stricmp(Get_Parameter("Repaired_Explosion"),"null"))
			Commands->Create_Explosion(Get_Parameter("Repaired_Explosion"),Commands->Get_Position(obj),damager);
		if (damager)
			Commands->Give_Points(damager,(Commands->Get_Player_Type(obj) == Commands->Get_Player_Type(damager) ? 1 : -1) * Get_Float_Parameter("Repair_Points"),0);
		if (Get_Int_Parameter("Disable_Collision_When_Dead"))
			Commands->Enable_Collisions(obj);
	}
	if (!playingTransition && !playingTwitch)
	{
		if (alive)
			Play_Animation(obj,false,Get_Float_Parameter("Live_Twitch_Start"),Get_Float_Parameter("Live_Twitch_End"));
		else
			Play_Animation(obj,false,Get_Float_Parameter("Death_Twitch_Start"),Get_Float_Parameter("Death_Twitch_End"));
		playingTwitch = Get_Int_Parameter("Play_Twitches_To_Completion") ? true : false;
	}
}
void JMG_Utility_Emulate_DamageableStaticPhys::Animation_Complete(GameObject *obj,const char *anim)
{
	if (alive)
		Play_Animation(obj,true,Get_Float_Parameter("Live_Loop_Start"),Get_Float_Parameter("Live_Loop_End"));
	else
		Play_Animation(obj,true,Get_Float_Parameter("Death_Loop_Start"),Get_Float_Parameter("Death_Loop_End"));
	if (playingTransition)
		playingTransition = false;
	if (playingTwitch)
		playingTwitch = false;
}
void JMG_Utility_Emulate_DamageableStaticPhys::Play_Animation(GameObject *obj,bool loop,float start,float end)
{
	if (start == -1 || end == -1)
		return;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),loop,0,start,end,false);
}
void JMG_Utility_Display_HUD_Info_Text_All_Players_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		JmgUtility::SetHUDHelpText(Get_Int_Parameter("StringId"),Get_Vector3_Parameter("ColorRGB"));
		if (!Get_Int_Parameter("Repeatable"))
			Destroy_Script();
	}
}
void JMG_Utility_Display_HUD_Info_Text_To_Sender_On_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		GameObject *player = sender;
		GameObject *tempPlayer = Get_GameObj(param);
		if (tempPlayer)
			player = tempPlayer;
		Set_HUD_Help_Text_Player(player,Get_Int_Parameter("StringId"),Get_Vector3_Parameter("ColorRGB"));
	if (!Get_Int_Parameter("Repeatable"))
		Destroy_Script();
	}
}
void JMG_Utility_Soldier_Transition_On_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		if (Get_Vehicle(obj))
			Soldier_Transition_Vehicle(obj);
		else
		{
			Vector3 pos = Commands->Get_Position(obj);
			float dist = 0;
			GameObject *nearest = NULL;
			for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
			{
				SmartGameObj* o = current->Data();
				if (o->As_VehicleGameObj() && Get_Vehicle_Occupant_Count(o) < Get_Vehicle_Seat_Count(o))
				{
					float tempDistance = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o));
					if (!nearest || tempDistance < dist)
					{
						nearest = o;
						dist = tempDistance;
					}
				}
			}
			if (nearest)
				Force_Vehicle_Entry(obj,nearest);
		}
	}
}
void JMG_Utility_Poke_Send_Self_Custom::Created(GameObject *obj)
{
	poked = false;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void JMG_Utility_Poke_Send_Self_Custom::Poked(GameObject *obj, GameObject *poker)
{
	if (!poked && Commands->Is_A_Star(poker))
	{
		poked= true;
		Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		if (Get_Int_Parameter("LockoutTime") >= 0.0f)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("LockoutTime"),1);
	}
}
void JMG_Utility_Poke_Send_Self_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		poked = false;
		Commands->Enable_HUD_Pokable_Indicator(obj,true);
	}
}
void JMG_Turret_Spawn::Created(GameObject *obj)
{
	GameObject *turret = Commands->Create_Object(Get_Parameter("Turret_Preset"),Vector3());
	if(!turret)
	{
		Console_Output("[%d:%s:%s] JMG_Turret_Spawn Critical Error: Failed to create an instance of the preset %s. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Parameter("Turret_Preset"));
		Destroy_Script();
		return;
	}
	Commands->Attach_To_Object_Bone(turret,obj,Get_Parameter("Bone_Name"));
	turretId = Commands->Get_ID(turret);
	if (turret->As_VehicleGameObj())
		turret->As_VehicleGameObj()->Set_Is_Scripts_Visible(false);
	hasDriver = false;
}
void JMG_Turret_Spawn::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!hasDriver)
		{
			hasDriver = true;
			GameObject *turret = Commands->Find_Object(turretId);
			if (turret)
			{
				Commands->Set_Player_Type(turret,Commands->Get_Player_Type(sender));
				Commands->Action_Reset(turret,100);
			}
		}
	}
	if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (hasDriver && obj->As_VehicleGameObj() && !Get_Vehicle_Occupant_Count(obj))
		{
			hasDriver = false;
			GameObject *turret = Commands->Find_Object(turretId);
			if (turret)
			{
				Commands->Set_Player_Type(turret,Commands->Get_Player_Type(obj));
				Commands->Action_Reset(turret,100);
			}
		}
	}
}
void JMG_Turret_Spawn::Killed(GameObject *obj,GameObject *killer)
{
	Destroyed(obj);
}
void JMG_Turret_Spawn::Destroyed(GameObject *obj)
{
	GameObject *turret = Commands->Find_Object(turretId);
	if (turret)
		Commands->Destroy_Object(turret);
}
void JMG_Turret_Spawn::Detach(GameObject *obj)
{
	if (Exe == 4)
		return;
	Destroyed(obj);
}
ClientNetworkObjectPositionSync *clientNetworkObjectPositionSyncControl = NULL;
void JMG_Utility_Sync_System_Object::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.25f,1);
	Commands->Start_Timer(obj,this,0.0f,2);
}
void JMG_Utility_Sync_System_Object::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!clientNetworkObjectPositionSyncControl)
		{
			if (!The_Game()->Is_Game_Over())
				Console_Input("msg JMG_Utility_Sync_System_Object ERROR: Make sure there is a JMG_Utility_Sync_System_Controller on the map!");
			return;
		}
		syncNode = clientNetworkObjectPositionSyncControl->addNode(obj);
	}
	if (number == 2)
	{
		Force_Position_Update(obj);
		Force_Orientation_Update(obj);
	}
}
void JMG_Utility_Sync_System_Object::Killed(GameObject *obj,GameObject *killer)
{
	if (syncNode)
		syncNode->id = 0;
}
void JMG_Utility_Sync_System_Object::Destroyed(GameObject *obj)
{
	if (The_Game()->Is_Game_Over())
		return;
	if (syncNode)
		syncNode->id = 0;
}
void JMG_Utility_Sync_System_Object::Detach(GameObject *obj)
{
	if (Exe == 4)
		return;
	Destroyed(obj);
}
JMG_Utility_Sync_System_Controller::JMG_Utility_Sync_System_Controller()
{
	clientNetworkObjectPositionSyncControl = new ClientNetworkObjectPositionSync();
}
void JMG_Utility_Sync_System_Controller::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,2.5f,1);
	Commands->Start_Timer(obj,this,2.5f,2);
}
void JMG_Utility_Sync_System_Controller::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (The_Game()->Is_Game_Over())
		{
			Destroyed(obj);
			return;
		}
		else
			clientNetworkObjectPositionSyncControl->checkForPlayersThatLeftTheGame();
		Commands->Start_Timer(obj,this,0.25f,1);
	}
	if (number == 2)
	{
		if (The_Game()->Is_Game_Over())
			return;
		clientNetworkObjectPositionSyncControl->triggerSingleNetworkSync();
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Sync_Rate"),2);
	}
}
void JMG_Utility_Sync_System_Controller::Destroyed(GameObject *obj)
{
	if (!clientNetworkObjectPositionSyncControl)
		return;
	clientNetworkObjectPositionSyncControl->Empty_List();
	clientNetworkObjectPositionSyncControl = NULL;
}
void JMG_Utility_Sync_System_Controller::Detach(GameObject *obj)
{
	Destroyed(obj);
}
void JMG_Utility_Sync_Object_Periodically::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_Sync_Object_Periodically::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Force_Position_Update(obj);
		Force_Orientation_Update(obj);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Sync_Rate"),1);
	}
}
Vector3 JMG_Utility_Basic_Spawner::preSpawnLocation = Vector3();
void JMG_Utility_Basic_Spawner::Created(GameObject *obj)
{
	spawnedId = 0;
	spawnLimit = Get_Int_Parameter("SpawnLimit");
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	attachScriptsGroupId = Get_Int_Parameter("Attach_Scripts_Group_ID");
	CalculateRespawnTime();
	Commands->Disable_All_Collisions(obj);
	Commands->Set_Is_Rendered(obj,false);
	Commands->Set_Is_Visible(obj,false);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Basic_Spawner::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!spawnedId && respawnTime > 0 && spawnLimit && enabled && !The_Game()->Is_Game_Over())
		{
			respawnTime -= 0.1f;
			if (respawnTime <= 0)
			{
				Vector3 createPos = Commands->Get_Position(obj);
				GameObject *spawnedObject = Commands->Create_Object(Get_Parameter("SpawnPreset"),preSpawnLocation);
				Commands->Set_Facing(spawnedObject,Commands->Get_Facing(obj));
				MoveablePhysClass *mphys = spawnedObject->As_PhysicalGameObj() ? spawnedObject->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
				if (!mphys || mphys->Can_Teleport(Matrix3D(createPos)))
				{
					if (spawnLimit > 0)
						spawnLimit--;
					Commands->Set_Position(spawnedObject,createPos);
					spawnedId = Commands->Get_ID(spawnedObject);
					char params[256];
					sprintf(params,"%d",Commands->Get_ID(obj));
					Commands->Attach_Script(spawnedObject,"JMG_Utility_Basic_Spawner_Spawned_Object",params);
					Commands->Send_Custom_Event(spawnedObject,obj,6873522,attachScriptsGroupId,0.0f);
				}
				else
				{
					Commands->Destroy_Object(spawnedObject);
					if (spawnedObject->As_SoldierGameObj())
						ChangeTeamDeaths(Commands->Get_Player_Type(spawnedObject),-1);
					respawnTime = 1.0f;
				}
			}
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Basic_Spawner::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (Get_Int_Parameter("EnableOrDisableCustom") && message == Get_Int_Parameter("EnableOrDisableCustom"))
	{
		if (param == 1)
		{
			enabled = true;
			CalculateRespawnTime();
		}
		else if (!param)
			enabled = false;
		else if (param == -1)
		{
			enabled = false;
			GameObject *spawnedObject = Commands->Find_Object(spawnedId);
			if (spawnedObject)
				Commands->Apply_Damage(spawnedObject,9999.9f,"BlamoKiller",NULL);
		}
		else if (param == -2)
		{
			enabled = false;
			GameObject *spawnedObject = Commands->Find_Object(spawnedId);
			if (spawnedObject)
				Commands->Destroy_Object(spawnedObject);
		}
	}
	if (message == 6873521)
	{
		respawnTime = Get_Float_Parameter("RespawnTime")+(Get_Float_Parameter("RespawnTimeRandom") ? Commands->Get_Random(-Get_Float_Parameter("RespawnTimeRandom"),Get_Float_Parameter("RespawnTimeRandom")) : 0.0f);
		if (respawnTime <= 0.0f)
			respawnTime = 0.1f;
		spawnedId = 0;
	}
}
void JMG_Utility_Basic_Spawner::CalculateRespawnTime()
{
	respawnTime = min((Get_Int_Parameter("DelayOnStartup") ? (Get_Float_Parameter("RespawnTime")+(Get_Float_Parameter("RespawnTimeRandom") ? Commands->Get_Random(-Get_Float_Parameter("RespawnTimeRandom"),Get_Float_Parameter("RespawnTimeRandom")) : 0.0f)) : 0),0.1f);
	if (respawnTime <= 0.0f)
		respawnTime = 0.1f;
}
void JMG_Utility_Basic_Spawner_Spawned_Object::Destroyed(GameObject *obj)
{
	GameObject *spawner = Commands->Find_Object(Get_Int_Parameter("ControllerId"));
	if (spawner)
		Commands->Send_Custom_Event(obj,spawner,6873521,0,0.0f);
}
void JMG_Utility_AI_Engineer::Created(GameObject *obj)
{
	centerLocation = (JmgUtility::SimpleDistance(Get_Vector3_Parameter("BaseCenterPoint"),Vector3()) <= 0.0f ? Commands->Get_Position(obj) : Get_Vector3_Parameter("BaseCenterPoint"));
	maxRange = Get_Float_Parameter("PatrolRange")*Get_Float_Parameter("PatrolRange");
	if (!Has_Weapon(obj,Get_Parameter("RepaiarGun_Preset")))
		Grant_Weapon(obj,Get_Parameter("RepaiarGun_Preset"),true,-1,true);
	if (Has_Weapon(obj,Get_Parameter("RepaiarGun_Preset")))
		Commands->Select_Weapon(obj,Get_Parameter("RepaiarGun_Preset"));
	if (_stricmp(Get_Parameter("Weapon_Preset"),"null"))
	{
		if (!Has_Weapon(obj,Get_Parameter("Weapon_Preset")))
			Grant_Weapon(obj,Get_Parameter("Weapon_Preset"),true,-1,true);
		canFight = true;
	}
	else
		canFight = false;
	const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Parameter("RepaiarGun_Preset"),true);
	if (ammo)
	{
		repairGunRange = ammo->Range;
		repairGunEffectiveRange = ammo->Range < ammo->EffectiveRange ? ammo->Range : ammo->EffectiveRange;
	}
	ammo = Get_Weapon_Ammo_Definition(Get_Parameter("Weapon_Preset"),true);
	if (ammo)
	{
		weaponRange = ammo->Range;
		weaponEffectiveRange = ammo->Range < ammo->EffectiveRange ? ammo->Range : ammo->EffectiveRange;
	}
	targetId = 0;
	actionUpdate = 0;
	repairGun = true;
	lastPos = Commands->Get_Position(obj);
	samePosition = 0;
	repairTargetId = 0;
	lastTarget = NULL;
	lastSecondaryTarget = NULL;
	lastRepairTarget = false;
	lastUseRepairGun = false;
	for (int x = 0;x < 128;x++)
		aiIgnorePlayers[x] = 0;
	randomResetAmount = Commands->Get_Random_Int(0,4);
	wanderPointGroup = Get_Int_Parameter("WanderGroupID");
	Commands->Start_Timer(obj,this,0.25f,1);
	Commands->Start_Timer(obj,this,1.0f,5);
	Commands->Start_Timer(obj,this,1.0f,6);
	Commands->Enable_Enemy_Seen(obj,true);
}
void JMG_Utility_AI_Engineer::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	if (!canFight)
		return;
	if (!JmgUtility::CanSeeStealth(0,obj,seen))
		return;
	if (targetId && targetUpdate)
		return;
	if (targetId && JmgUtility::SimpleDistance(Commands->Get_Position(seen),Commands->Get_Position(obj)) > targetDistance)
		return;
	if (aiIgnorePlayers[JmgUtility::JMG_Get_Player_ID(seen)])
		return;
	if (!Commands->Get_Health(seen))
		return;
	targetId = Commands->Get_ID(seen);
	targetUpdate = 20;
}
void JMG_Utility_AI_Engineer::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (Commands->Get_Health(obj) <= 0)
			return;
		int playerType = Commands->Get_Player_Type(obj);
		GameObject *target = targetId ? Commands->Find_Object(targetId) : NULL;
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *lastRep = Commands->Find_Object(repairTargetId);
		if (repairTargetId && (!lastRep || Get_Hitpoints(lastRep) >= Get_Max_Hitpoints(lastRep) || !Commands->Get_Health(lastRep)))
		{
			if (!lastRep || !Commands->Get_Health(lastRep) || Commands->Get_Random(0.0f,1.0f) < 0.33f)
				samePosition = 100;
			else
				samePosition = randomResetAmount;
			lastRep = NULL;
			repairTargetId = 0;
		}
		float nearestObject = lastRep ? JmgUtility::SimpleDistance(pos,Commands->Get_Position(lastRep)) : 0.0f;
		GameObject *repairTarget = lastRep;
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj* o = current->Data();
			if (Valid_Repair_Target(obj,o,playerType))
			{
				if (!inRange(o))
					continue;
				float tempDistance = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o))*Get_Vector3_Parameter("Priority[RepairTargets|C4|Beacons]").X;
				if (!repairTarget || tempDistance < nearestObject)
				{
					repairTarget = o;
					nearestObject = tempDistance;
				}
			}
		}
		if (Get_Vector3_Parameter("MinHP[EngineerTarget|C4|Beacon]").Y < 1.0)
			for (SLNode<C4GameObj> *current = GameObjManager::C4GameObjList.Head();current;current = current->Next())
			{
				GameObject *o = (GameObject *)current->Data();
				if (Valid_Repair_Target_C4(obj,o,playerType))
				{
					if (!inRange(o))
						continue;
					float tempDistance = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o))*Get_Vector3_Parameter("Priority[RepairTargets|C4|Beacons]").Y;
					if (!repairTarget || tempDistance < nearestObject)
					{
						repairTarget = o;
						nearestObject = tempDistance;
					}
				}
			}
		if (Get_Vector3_Parameter("MinHP[EngineerTarget|C4|Beacon]").Z < 1.0)
			for (SLNode<BeaconGameObj> *current = GameObjManager::BeaconGameObjList.Head();current;current = current->Next())
			{
				GameObject *o = (GameObject *)current->Data();
				if (Valid_Repair_Target_C4(obj,o,playerType))
				{
					if (!inRange(o))
						continue;
					float tempDistance = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o))*Get_Vector3_Parameter("Priority[RepairTargets|C4|Beacons]").Z;
					if (!repairTarget || tempDistance < nearestObject)
					{
						repairTarget = o;
						nearestObject = tempDistance;
					}
				}
			}
		if (repairTarget)
			repairTargetId = Commands->Get_ID(repairTarget);
		if (targetId && (!target || !Commands->Get_Health(target) || !inRange(target)))
		{
			target = NULL;
			targetUpdate = 0;
			targetId = 0;
		}
		if (target)
		{
			float repairTargetDistance = JmgUtility::SimpleDistance(Commands->Get_Position(repairTarget),pos);
			if (!repairTarget || repairTargetDistance > repairGunRange*repairGunRange || repairTargetDistance > JmgUtility::SimpleDistance(Commands->Get_Position(target),pos))
				AttackTarget(obj,repairTarget,target,true,false);
			else
				AttackTarget(obj,repairTarget,target,true,true);
		}
		else if (repairTarget)
			AttackTarget(obj,repairTarget,target,true,true);
		if (targetUpdate)
			targetUpdate--;
		if (actionUpdate)
			actionUpdate--;
		if (!targetId && !repairTargetId && samePosition >= 10 && (wanderPointGroup != -1 || (Get_Float_Parameter("PatrolRange") > 0 ? true : false)))
		{
			randomResetAmount = Commands->Get_Random_Int(0,4);
			samePosition = randomResetAmount;
			Vector3 wanderPos = Vector3();
			if (wanderPointGroup != -1 && Get_Random_Wander_Point(&wanderPos))
			{
				Commands->Action_Reset(obj,100);
				ActionParamsStruct params;
				params.Set_Movement(wanderPos,1.0f,2.5f,false);
				params.MovePathfind = true;
				Commands->Action_Goto(obj,params);
			}
			else
			{
				pos = centerLocation;
				Vector3 returnPos;
				for (int x = 0;x < 10;x++)
				{
					if (Get_Random_Pathfind_Spot(pos,Get_Float_Parameter("PatrolRange"),&returnPos))
					{
						Commands->Action_Reset(obj,100);
						ActionParamsStruct params;
						params.Set_Movement(returnPos,1.0f,2.5f,false);
						params.MovePathfind = true;
						Commands->Action_Goto(obj,params);
						break;
					}
					pos.Z = centerLocation.Z+Commands->Get_Random(-Get_Float_Parameter("PatrolRange"),Get_Float_Parameter("PatrolRange"));
				}
			}
		}
		if (JmgUtility::SimpleDistance(pos,lastPos) < 0.1)
			samePosition++;
		else
			samePosition = randomResetAmount;
		lastPos = pos;
		Commands->Start_Timer(obj,this,0.25f,1);
	}
	if (number == 5)
	{
		for (int x = 1;x < 128;x++)
			if (aiIgnorePlayers[x])
				aiIgnorePlayers[x]--;
		if (!targetId && !repairTargetId && Commands->Get_Random(0.0f,1.0f) <= 0.35f)
		{
			ActionParamsStruct params;
			Commands->Get_Action_Params(obj,params);
			Vector3 pos = Commands->Get_Position(obj);
			float rotation = Commands->Get_Random(0,9)*45;
			pos.X += cos(rotation)*100.0f;
			pos.Y += sin(rotation)*100.0f;
			params.Set_Look(pos,Commands->Get_Random(1.0f,5.0f));
			Commands->Action_Goto(obj,params);
		}
		Commands->Start_Timer(obj,this,1.0f,5);
	}
	if (number == 6)
	{
		Vector3 speed = Vector3();
		if (obj->As_SoldierGameObj())
			obj->As_SoldierGameObj()->Get_Velocity(speed);
		float repeatTime = Commands->Get_Random(5.0f,7.5f);
		if (!targetId && !repairTargetId && JmgUtility::SimpleDistance(Vector3(),speed) <= 0.0f)
		{
			ActionParamsStruct params;
			Commands->Get_Action_Params(obj,params);
			Vector3 pos = Commands->Get_Position(obj);
			float rotation = Commands->Get_Random(0,9)*45;
			pos.X += cos(rotation)*100.0f;
			pos.Y += sin(rotation)*100.0f;
			params.Set_Face_Location(pos,repeatTime);
			Commands->Action_Face_Location(obj,params);
		}
		Commands->Start_Timer(obj,this,repeatTime,6);
	}
}
void JMG_Utility_AI_Engineer::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!canFight)
		return;
	if (damage <= 0 || !damager || obj == damager || (targetId && targetUpdate) || targetId == Commands->Get_ID(damager) || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (aiIgnorePlayers[JmgUtility::JMG_Get_Player_ID(damager)])
		return;
	if (targetId && JmgUtility::SimpleDistance(Commands->Get_Position(damager),Commands->Get_Position(obj)) > targetDistance)
		return;
	targetId = Commands->Get_ID(damager);
	targetUpdate = 40;
}
void JMG_Utility_AI_Engineer::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		if (Commands->Get_Health(obj) <= 0)
			return;
		GameObject *target = Commands->Find_Object(targetId);
		int playerId = JmgUtility::JMG_Get_Player_ID(target);
		if (playerId && Commands->Get_Health(target))
			aiIgnorePlayers[playerId] = 30;
	}
}
void JMG_Utility_AI_Engineer::AttackTarget(GameObject *obj,GameObject *target,GameObject *secondaryTarget,bool repairTarget,bool useRepairGun)
{
	if (useRepairGun != repairGun)
	{
		actionUpdate = 0;
		repairGun = useRepairGun;
		Commands->Select_Weapon(obj,useRepairGun ? Get_Parameter("RepaiarGun_Preset") : Get_Parameter("Weapon_Preset"));
	}
	if (target != lastTarget || secondaryTarget != lastSecondaryTarget || repairTarget != lastRepairTarget || lastUseRepairGun != useRepairGun)
		actionUpdate = 0;
	if (actionUpdate)
		return;
	lastTarget = target;
	lastSecondaryTarget = secondaryTarget;
	lastRepairTarget = repairTarget;
	lastUseRepairGun = useRepairGun;
	actionUpdate = 15;
	ActionParamsStruct params;
	if (repairTarget)
		params.Set_Movement(target,1.0f,repairGun ? repairGunEffectiveRange*0.9f : weaponEffectiveRange*0.9f,false);
	else if (secondaryTarget)
		params.Set_Movement(secondaryTarget,1.0f,repairGun ? repairGunEffectiveRange*0.9f : weaponEffectiveRange*0.9f,false);
	else
		params.Set_Movement(moveLocation,1.0f,repairGun ? repairGunEffectiveRange*0.9f : weaponEffectiveRange*0.9f,false);
	params.MovePathfind = true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,100,100,AI_STATE_COMBAT);
	params.AttackCheckBlocked = false;
	if (repairTarget && !useRepairGun && secondaryTarget)
	{
		params.MoveObject = secondaryTarget;
		params.Set_Attack(secondaryTarget,repairGun ? repairGunRange : weaponRange,0.0f,true);
		Commands->Action_Attack(obj,params);
	}
	else if (target && (repairTarget && useRepairGun) || (!repairTarget && !useRepairGun))
	{
		params.MoveObject = target;
		params.Set_Attack(target,repairGun ? repairGunRange : weaponRange,0.0f,true);
		Commands->Action_Attack(obj,params);
	}
}
bool JMG_Utility_AI_Engineer::inRange(GameObject *obj)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (JmgUtility::SimpleFlatDistance(pos,centerLocation) < maxRange)
		return true;
	return false;
}
bool JMG_Utility_AI_Engineer::Valid_Repair_Target(GameObject *obj,GameObject *target,int playerType)
{
	if (!target || target == obj || Commands->Get_Player_Type(target) != playerType || Get_Hitpoints(target) >= Get_Max_Hitpoints(target) || Is_Script_Attached(target,"JMG_Utility_AI_Engineer_Ignore_Target"))
		return false;
	if (Get_Vector3_Parameter("MinHP[EngineerTarget|C4|Beacon]").X && Is_Script_Attached(target,"JMG_Utility_AI_Engineer_Repair_Target") && Get_Hitpoints(target) < Get_Max_Hitpoints(target)*Get_Vector3_Parameter("MinHP[EngineerTarget|C4|Beacon]").X)
		return true;
	if (Get_Vector3_Parameter("MinHP[Soldiers|Vehicles|Turrets]").X && target->As_SoldierGameObj() && Get_Hitpoints(target) < Get_Max_Hitpoints(target)*Get_Vector3_Parameter("MinHP[Soldiers|Vehicles|Turrets]").X)
		return true;
	if (Get_Vector3_Parameter("MinHP[Soldiers|Vehicles|Turrets]").Y && target->As_VehicleGameObj() && Get_Vehicle_Mode(target) != VEHICLE_TYPE_TURRET && Get_Hitpoints(target) < Get_Max_Hitpoints(target)*Get_Vector3_Parameter("MinHP[Soldiers|Vehicles|Turrets]").Y)
		return true;
	if (Get_Vector3_Parameter("MinHP[Soldiers|Vehicles|Turrets]").Z && target->As_VehicleGameObj() && Get_Vehicle_Mode(target) == VEHICLE_TYPE_TURRET && Get_Hitpoints(target) < Get_Max_Hitpoints(target)*Get_Vector3_Parameter("MinHP[Soldiers|Vehicles|Turrets]").Z)
		return true;
	return false;
}
bool JMG_Utility_AI_Engineer::Valid_Repair_Target_C4(GameObject *obj,GameObject *target,int playerType)
{
	if (!target || target == obj || Commands->Get_Player_Type(target) == playerType)
		return false;
	if (Get_Vector3_Parameter("MinHP[EngineerTarget|C4|Beacon]").Y < 1.0 && target->As_PhysicalGameObj() && target->As_PhysicalGameObj()->As_C4GameObj() && Get_Hitpoints(target) > Get_Max_Hitpoints(target)*Get_Vector3_Parameter("MinHP[EngineerTarget|C4|Beacon]").Y)
		if (Get_C4_Attached(target) == obj)
			return false;
		else
			return true;
	if (Get_Vector3_Parameter("MinHP[EngineerTarget|C4|Beacon]").Z < 1.0 && target->As_PhysicalGameObj() && target->As_PhysicalGameObj()->As_BeaconGameObj() && Get_Hitpoints(target) > Get_Max_Hitpoints(target)*Get_Vector3_Parameter("MinHP[EngineerTarget|C4|Beacon]").Z)
		return true;
	return false;
}
void JMG_Utility_Set_Object_Visibility_For_Player_On_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		Set_Object_Visibility_For_Player(Get_GameObj(Get_Int_Parameter("PlayerID")),Commands->Get_ID(obj),Get_Int_Parameter("Visible") ? true : false);
	}
}
void JMG_Utility_AI_Vehicle::Created(GameObject *obj)
{
	const char *weap = Get_Current_Weapon(obj);
	if (weap)
	{
		primary = DefineAmmo(Get_Weapon_Ammo_Definition(weap,true));
		secondary = DefineAmmo(Get_Weapon_Ammo_Definition(weap,true));
	}
	minDistanceSquared = Get_Float_Parameter("MinAttackRange")*Get_Float_Parameter("MinAttackRange");
	retreatTime = 0;
	lastHealth = 0;
	moving = false;
	attacking = false;
	reverseTime = 0;
	stuckCount = 0;
	drivingBackward = false;
	badDestAttempt = 0;
	doNotUsePathfind = 0;
	firstRetreat = true;
	maxHuntRangeSquared = Get_Float_Parameter("MaxHuntRange")*Get_Float_Parameter("MaxHuntRange");
	retreatDistanceSquared = Get_Float_Parameter("RetreatDistance")*Get_Float_Parameter("RetreatDistance");
	lastWanderPointSpot = currentAction.position = retreatPos = homepos = Commands->Get_Position(obj);
	myteam = Commands->Get_Player_Type(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	if (obj->As_VehicleGameObj()) 
	{
		Commands->Innate_Enable(obj);
		if (Get_Int_Parameter("EnableTransitions")) 
			Commands->Enable_Vehicle_Transitions(obj,true);
		else 
			Commands->Enable_Vehicle_Transitions(obj,false);
		Commands->Enable_Engine(obj,true);
		Commands->Enable_Hibernation(obj,false);
	} 
	else 
	{
		Commands->Innate_Enable(obj);
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Enable_Hibernation(obj,false);
	}
	if (Get_Int_Parameter("OverrideFireMode"))
	{
		overrideFireMode = true;
		overridePrimary = Get_Int_Parameter("OverrideFireMode") == 1 ? true : false;
	}
	Commands->Start_Timer(obj,this,1.0,1);
	grabNextPointDistance = Get_Float_Parameter("NextWanderPointWhenLastReached")*Get_Float_Parameter("NextWanderPointWhenLastReached");
	if (Get_Int_Parameter("WanderingAIGroupID") != -1)
		if (grabNextPointDistance)
		{
			lastWanderPointSpot = Commands->Get_Position(obj);
			Commands->Start_Timer(obj,this,0.1f,9);
		}
		else if (Get_Float_Parameter("MinRandWander") != 0.0f && Get_Float_Parameter("MaxRandWander") != 0.0f)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("MinRandWander") == Get_Float_Parameter("MaxRandWander") ? Get_Float_Parameter("MaxRandWander") : Commands->Get_Random(Get_Float_Parameter("MinRandWander"),Get_Float_Parameter("MaxRandWander")),2);
	Commands->Start_Timer(obj,this,5.0,3);
	Commands->Start_Timer(obj,this,0.1f,8);
}
void JMG_Utility_AI_Vehicle::Custom(GameObject *obj,int message,int param,GameObject *sender) 
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		attacking = false;
		moving = false;
		int testTeam = Commands->Get_Player_Type(sender);
		if(testTeam != Commands->Get_Player_Type(obj))
			Commands->Start_Timer(obj,this,0.5,4);
	}
	if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		Commands->Set_Player_Type(obj,myteam);
	}
}
void JMG_Utility_AI_Vehicle::Action_Complete(GameObject *obj,int action,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_START)
	{
		bool refreshAction = false;
		if (!doNotUsePathfind)
			refreshAction = true;
		doNotUsePathfind = 3;
		if (refreshAction)
			Commands->Start_Timer(obj,this,0.1f,7);
	}
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		if (!badDestAttempt)
			Commands->Start_Timer(obj,this,0.25f,5);
		else
			Commands->Start_Timer(obj,this,0.25f,6);
	}
	if (action == 10)
		moving = false;
	Commands->Action_Reset(obj,action*1.0f);
	Commands->Action_Reset(obj,100*1.0f);
}
void JMG_Utility_AI_Vehicle::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0 || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (Is_Script_Attached(damager,"JMG_Utility_AI_Vehicle_Ignored"))
		return;
	if (!_stricmp(Get_Skin(damager),"Blamo") || !_stricmp(Get_Shield_Type(damager),"Blamo"))
		return;
	if(!attacking)
		RunAttack(obj,SetTarget(damager));
	else 
		RunAttack(obj,SelectTarget(obj,damager));
	lastSeenCount = Commands->Get_Random_Int(6,15);
}
void JMG_Utility_AI_Vehicle::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Vehicle_Ignored"))
		return;
	Vector3 enemyPos = Commands->Get_Position(seen);
	float dist = JmgUtility::SimpleDistance(Commands->Get_Position(obj),enemyPos);
	if (!_stricmp(Get_Skin(seen),"Blamo") || !_stricmp(Get_Shield_Type(seen),"Blamo"))
		return;
	if (Get_Float_Parameter("MaxHuntRange") > 0.0f && JmgUtility::SimpleDistance(lastWanderPointSpot,enemyPos) > maxHuntRangeSquared)
		return;
	if (Get_Float_Parameter("MinAttackRange") && dist < minDistanceSquared)
		return;
	if (!Commands->Get_Health(seen))
		return;
	if (seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed())
		if (seen->As_SoldierGameObj() && dist > seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance()*seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance())
			return;
		else if (seen->As_VehicleGameObj() && dist > seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance()*seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance())
			return;	
		else if (dist > seen->As_SmartGameObj()->Get_Stealth_Fade_Distance()*seen->As_SmartGameObj()->Get_Stealth_Fade_Distance())
			return;
	if (Commands->Get_ID(seen) == currentAction.targetId)
		lastSeenCount = Commands->Get_Random_Int(3,16);
	if(!attacking)
		RunAttack(obj,SetTarget(seen));
	else
		RunAttack(obj,SelectTarget(obj,seen));
}
void JMG_Utility_AI_Vehicle::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1) 
	{
		StuckCheck(obj);
		if (lastSeenCount)
		{
			lastSeenCount--;
			if (!lastSeenCount)
				currentAction.targetId = -1;
		}
		if (retreatTime)
			retreatTime--;
		if (doNotUsePathfind)
			doNotUsePathfind--;
		Commands->Start_Timer(obj,this,1.0,1);
	}
	if (number == 2)
	{
		Vector3 wanderPos = Vector3();
		if (GetRandomPosition(&wanderPos) && (!currentAction.targetId || (currentAction.targetId && !Get_Int_Parameter("FollowTarget"))))
		{
			moving = true;
			lastWanderPointSpot = wanderPos;
			AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,wanderPos,currentAction.useAmmo,0.0f,false,2.5f);
		}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("MinRandWander") == Get_Float_Parameter("MaxRandWander") ? Get_Float_Parameter("MaxRandWander") : Commands->Get_Random(Get_Float_Parameter("MinRandWander"),Get_Float_Parameter("MaxRandWander")),2);
	}
	if (number == 3) 
	{
		if (retreatTime)
		{
			GameObject *target = Commands->Find_Object(currentAction.targetId);
			if(target)
			{
				AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,currentAction.position,currentAction.useAmmo,0.0f,false,2.5f);
				Commands->Start_Timer(obj,this,1.0f,3);
			}
			return;
		}
		if(attacking)
		{
			attacking = false;
			if (!lastSeenCount && currentAction.targetId)
				currentAction.targetId = 0;
			GameObject *target = Commands->Find_Object(currentAction.targetId);
			if(target)
				RunAttack(obj,SetTarget(target));
			else 
			{
				currentAction.useAmmo = 1;
				currentAction.targetId = 0;
				if (Get_Int_Parameter("ReturnHome") && !retreatTime) 
					ReturnHome(obj);
				else
					AttackMove(obj,NULL,false,currentAction.position,currentAction.useAmmo,0.0f,false,2.5f);
			}
		} 
		else
			if (Get_Int_Parameter("ReturnHome") && !retreatTime)
				ReturnHome(obj);
			else
				AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,currentAction.position,currentAction.useAmmo,0.0f,false,2.5f);
	}
	if (number == 4)
	{
		if (Get_Vehicle_Occupant_Count(obj))
			Force_Occupants_Exit(obj);
	}
	if (number == 5) 
	{
		badDestAttempt = 1;
		AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,Commands->Get_Position(obj),currentAction.useAmmo,0.0f,false,2.5f);
	}
	if (number == 6) 
	{
		badDestAttempt = 0;
		AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,homepos,currentAction.useAmmo,0.0f,false,2.5f);
	}
	if (number == 7) 
	{
		AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,currentAction.position,currentAction.useAmmo,0.0f,true,2.5f);
	}
	if (number == 8) 
	{
		if (attacking || currentAction.targetId)
		{
			GameObject *target = Commands->Find_Object(currentAction.targetId);
			if ((attacking && !target) || (target && !Commands->Get_Health(target)) || (target && Commands->Get_Player_Type(obj) != -2 && Commands->Get_Player_Type(target) == Commands->Get_Player_Type(obj)))
			{
				lastSeenCount = 0;
				attacking = false;
				currentAction.targetId = 0;
				Vector3 wanderPos = Vector3();
				if (Get_Int_Parameter("WanderingAIGroupID") != -1 && GetRandomPosition(&wanderPos) && (!Get_Int_Parameter("FollowTarget") || !currentAction.targetId))
				{
					moving = true;
					lastWanderPointSpot = wanderPos;
					AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,wanderPos,currentAction.useAmmo,0.0f,false,2.5f);
				}
				else
				{
					lastWanderPointSpot = homepos;
					AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,homepos,currentAction.useAmmo,0.0f,false,2.5f);
				}
			}
		}
		Commands->Start_Timer(obj,this,0.1f,8);
	}
	if (number == 9)
	{
		if (!currentAction.targetId || (currentAction.targetId && !Get_Int_Parameter("FollowTarget")))
		{
			if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),lastWanderPointSpot) <= grabNextPointDistance)
			{
				GetRandomPosition(&lastWanderPointSpot);
				moving = true;
			}
			AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,lastWanderPointSpot,currentAction.useAmmo,0.0f,false,2.5f);
		}
		Commands->Start_Timer(obj,this,1.0f,9);
	}
}
void JMG_Utility_AI_Vehicle::ReturnHome(GameObject * obj)
{
	currentAction = JMGVehicleAction();
	lastHealth = 0;
	attacking = false;
	moving = true;
	Vector3 pos;
	pos = mypos = Commands->Get_Position(obj);
	if (retreatTime && Commands->Find_Object(lastAction.targetId) && JmgUtility::SimpleDistance(Commands->Get_Position(Commands->Find_Object(lastAction.targetId)),retreatPos) > minDistanceSquared) 
		AttackMove(obj,Commands->Find_Object(lastAction.targetId),false,retreatPos,lastAction.useAmmo,0.0f,true,2.5f);
	else if (GetRandomPositionOutsideOfRange(&pos))
		AttackMove(obj,Commands->Find_Object(lastAction.targetId),false,pos,lastAction.useAmmo,0.0f,false,2.5f);
	else
		AttackMove(obj,Commands->Find_Object(lastAction.targetId),false,homepos,lastAction.useAmmo,0.0f,false,2.5f);
	lastWanderPointSpot = homepos;
}
void JMG_Utility_AI_Vehicle::RunAttack(GameObject *obj,GameObject *target) 
{
	if(target) 	
	{
		if (retreatTime)
		{
			AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,currentAction.position,currentAction.useAmmo,0.0f,false,2.5f);
			return;
		}
		int ammo = SelectAmmo(target);
		if (ammo == -1) 
		{
			attacking = false;
			useAmmo = 1;
			currentAction.targetId = 0;
			return;
		}
		if (ammo == 2) 
		{
			if (lastHealth == Commands->Get_Health(target))
				useAmmo = useAmmo == 1 ? 0 : 1;
			ammo = useAmmo;
		}
		useAmmo = ammo;
		lastHealth = Commands->Get_Health(target);
		attacking = true;
		mypos = Commands->Get_Position(obj);
		if (Get_Int_Parameter("FollowTarget")) 
		{
			moving = true;
			AttackMove(obj,target,true,Vector3(),useAmmo,0.0f,false,Get_Float_Parameter("MaxRange"));
		} 
		else 
		{
			moving = false;
			AttackMove(obj,target,false,currentAction.position,useAmmo,0.0f,false,Get_Float_Parameter("MaxRange"));
		}
		Commands->Start_Timer(obj,this,1.0f,3);
	}
	else 
	{
		moving = false;
		attacking = false;
		useAmmo = 1;
		AttackMove(obj,NULL,false,currentAction.position,useAmmo,0.0f,false,currentAction.arriveDistance);
		return;
	}
}
int JMG_Utility_AI_Vehicle::GetThreatRating(GameObject * obj) 
{
	if (obj == NULL)
		return 0;
	int rating=0;
	if(Commands->Is_A_Star(obj)) 
	{ 
		rating += 1;
		GameObject *vehicle = Get_Vehicle(obj);
		if (vehicle) 
			rating += 3; 
	}
	if (obj->As_SoldierGameObj()) 
		rating += 1; 
	if (obj->As_VehicleGameObj()) 
		rating += 3; 
	return rating;
}
GameObject * JMG_Utility_AI_Vehicle::GetAttackObject(GameObject * obj) 
{
	if (obj == NULL)
		return NULL;
	if(Commands->Is_A_Star(obj)) 
	{
		GameObject *vehicle = Get_Vehicle(obj); 
		if(vehicle)
			return vehicle;
		else 
			return obj;
	}
	return obj;
}
GameObject *JMG_Utility_AI_Vehicle::SelectTarget(GameObject *obj,GameObject *target)
{
	GameObject *old = currentAction.targetId ? Commands->Find_Object(currentAction.targetId) : NULL;
	if (!target || !Commands->Get_ID(target))
		return GetAttackObject(old);
	if (!old) 
	{
		currentAction.targetId = Commands->Get_ID(target);
		return GetAttackObject(target);
	}
	int trOld = GetThreatRating(old);
	int trNew = GetThreatRating(target);
	if (trOld > trNew)
		return GetAttackObject(old);
	if (trNew > trOld) 
	{
		currentAction.targetId = Commands->Get_ID(target);
		return GetAttackObject(target);
	}
	GameObject * closest = GetClosest(obj,target,old);
	currentAction.targetId = Commands->Get_ID(closest);
	return GetAttackObject(closest);
}
GameObject *JMG_Utility_AI_Vehicle::SetTarget(GameObject *target)
{
	currentAction.targetId = Commands->Get_ID(target);
	if(!attacking)
		return GetAttackObject(target);
	return 0;
}
GameObject *JMG_Utility_AI_Vehicle::GetClosest(GameObject *obj,GameObject *new_target,GameObject *old_target)
{
	Vector3 objPos = Commands->Get_Position(obj);
	if(JmgUtility::SimpleDistance(objPos,Commands->Get_Position(new_target)) < JmgUtility::SimpleDistance(objPos,Commands->Get_Position(old_target)))
		return new_target;
	else
		return old_target;
}
int JMG_Utility_AI_Vehicle::SelectAmmo(GameObject *target)
{
	if (target)
	{
		if (target->As_SoldierGameObj())
			return Get_Int_Parameter("vsSoldier");
		if (target->As_VehicleGameObj()) 
		{
			if (Is_VTOL(target))
				return Get_Int_Parameter("vsAircraft");
			return Get_Int_Parameter("vsVehicle");
		}
	}
	return -1;
}
void JMG_Utility_AI_Vehicle::StuckCheck(GameObject *obj)
{
	Vector3 current = Commands->Get_Position(obj);
	if (reverseTime)
	{
		reverseTime--;
		stuckCount = 0;
		mypos = current;
		drivingBackward = false;
		AttackMove(obj,Commands->Find_Object(lastAction.targetId),lastAction.following,lastAction.position,lastAction.useAmmo,0.0f,true,0.0f);
		return;
	}
	if (!moving)
	{
		drivingBackward = false;
		return;
	}
	if(JmgUtility::SimpleDistance(mypos,current) < 1.0f)
	{
		if(stuckCount > 2)
		{
			reverseTime = Commands->Get_Random_Int(2,6);
			stuckCount = 0;
			drivingBackward = true;
			AttackMove(obj,Commands->Find_Object(lastAction.targetId),lastAction.following,lastAction.position,lastAction.useAmmo,0.0f,true,currentAction.arriveDistance);
		}
		stuckCount++;
	}
	else
	{
		stuckCount = 0;
		mypos = current;
	}
}
void JMG_Utility_AI_Vehicle::AttackMove(GameObject *obj,GameObject *target,bool followTarget,Vector3 targetLocation,int fireMode,float weaponError,bool forceUpdate,float arriveDistance)
{
	mypos = Commands->Get_Position(obj);
	float dist = 9999.9f;
	if (target)
		if (followTarget)
			dist = JmgUtility::SimpleDistance(Commands->Get_Position(target),mypos);
		else
			dist = JmgUtility::SimpleDistance(currentAction.position,mypos);
	if (minDistanceSquared && dist > minDistanceSquared+1089.0f)
		retreatPos = mypos;
	if (!followTarget)
		currentAction.position = targetLocation;
	if (((minDistanceSquared && dist < minDistanceSquared) || (retreatDistanceSquared && dist < retreatDistanceSquared)) && !(Get_Int_Parameter("SquishInfantry") && target->As_SoldierGameObj()))
	{
		if (!retreatTime && minDistanceSquared && dist < minDistanceSquared)
		{
			retreatTime = 5;
			lastSeenCount = 7;
			ReturnHome(obj);
			return;
		}
		Vector3 pos = mypos;
		followTarget = false;
		if (JmgUtility::SimpleDistance(mypos,currentAction.position) < 25.0f || firstRetreat)
		{
			firstRetreat = false;
			if (GetRandomPositionOutsideOfRange(&pos))
 				currentAction.position = pos;
			else
				currentAction.position = homepos;
		}
	}
	currentAction.useAmmo = fireMode;
	currentAction.following = followTarget;
	currentAction.backward = drivingBackward;
	currentAction.arriveDistance = arriveDistance;
	if (lastAction == currentAction && !forceUpdate)
		return;
	lastAction = currentAction;
	bool canSquish = false;
	bool isMoving = moving;
	Commands->Action_Reset(obj,10.0f);
	moving = isMoving;
	ActionParamsStruct params;
	if (target)
	{
		JMGVehicleAmmo ammo = (fireMode ? primary : secondary) ;
		weaponError = 0.0f;
		if (Get_Float_Parameter("WeaponError") != -1.00f)
			weaponError = Get_Float_Parameter("WeaponError");
		else if (ammo.allowError)
			weaponError = JmgUtility::SimpleDistance(Commands->Get_Position(target),mypos)/ammo.speed*5.0f;
		params.Set_Attack(target,ammo.range,weaponError,!overrideFireMode ? (fireMode ? true : false) : overridePrimary);
		if (Get_Int_Parameter("ForceFire"))	
		{
			params.AttackCheckBlocked = false;
			params.AttackForceFire = true;
		}
		if (Get_Int_Parameter("SquishInfantry") && target->As_SoldierGameObj() && !Is_Unsquishable(target))
			canSquish = true;
	}
	if (followTarget)
		if (Is_VTOL(obj) && Get_Float_Parameter("VTOLHover") && !canSquish)
		{
			if (target)
			{
				targetLocation = Commands->Get_Position(target);
				targetLocation.Z += Get_Float_Parameter("VTOLHover");
				params.AttackFaceTarget = false;
			}
			params.Set_Movement(targetLocation,JmgUtility::MathClamp(dist,1.0f,100.0f),arriveDistance,false);
			params.MoveFollow = false;
		}
		else
			params.Set_Movement(target,JmgUtility::MathClamp(dist,1.0f,100.0f),canSquish ? 1.0f : arriveDistance,false);
	else
	{
		if (Is_VTOL(obj) && Get_Float_Parameter("VTOLHover") && !canSquish)
			targetLocation.Z += Get_Float_Parameter("VTOLHover");
		params.Set_Movement(targetLocation,JmgUtility::MathClamp(dist,1.0f,100.0f),canSquish ? 1.0f : arriveDistance,false);
	}
	params.MoveBackup = currentAction.backward;
	if (Is_VTOL(obj))
		params.MovePathfind = false;
	else
	{
		if (Get_Int_Parameter("DisableEngineOnArrival"))
			params.ShutdownEngineOnArrival = true;
		params.MovePathfind = !doNotUsePathfind && Get_Int_Parameter("UsePathfind") ? true : false;
	}
	if (!target)
		params.Set_Basic(this,10,10);
	Commands->Action_Goto(obj,params);
	if (target)
	{
		params.Set_Basic(this,10,10);
		Commands->Action_Attack(obj,params);
	}
}
JMG_Utility_AI_Vehicle::JMGVehicleAmmo JMG_Utility_AI_Vehicle::DefineAmmo(const AmmoDefinitionClass *ammo)
{
	JMGVehicleAmmo theammo;
	if (ammo)
	{
		theammo.range = ammo->Range;
		if (ammo->Velocity >= 400.0f || (ammo->IsTracking && ammo->TurnRate))
			theammo.allowError = false;
		else
			theammo.allowError = true;
		theammo.speed = ammo->Velocity*ammo->Velocity;
	}
	return theammo;
}
DynamicClockSystem DynamicClockSystemControl = DynamicClockSystem();
void JMG_Utility_Dynamic_Clock_Control::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,1.0,1);
}
void JMG_Utility_Dynamic_Clock_Control::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1) 
	{
		if (lastMinute != getMinute())
		{
			lastMinute = getMinute();
			DynamicClockSystemControl.updateClocks();
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}
void JMG_Utility_Dynamic_Clock_Control::Destroyed(GameObject *obj)
{
	DynamicClockSystemControl.emptyList();
}
int JMG_Utility_Dynamic_Clock_Control::getMinute()
{
	time_t rawtime;
	struct tm * ptm;
	time (&rawtime);
	ptm = gmtime (&rawtime);
	return ptm->tm_min;
}
void JMG_Utility_Dynamic_Clock_Object::Created(GameObject *obj)
{
	for (int x = 0;x < 128;x++)
		animSynced[x] = false;
	DynamicClockSystemControl += obj;
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_Dynamic_Clock_Object::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1) 
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (player && animSynced[x] < 3)
			{
				animSynced[x]++;
				if (animSynced[x] == 3)
					Set_Subobject_Animation_Player(player,obj,"s_ecwClock.a_ecwClock",false,0,Get_Animation_Frame(obj),Get_Animation_Frame(obj),false);
			}
			if (!player && animSynced[x])
				animSynced[x] = 0;
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Dynamic_Clock_Object::Destroyed(GameObject *obj)
{
	DynamicClockSystemControl -= obj;
}
void JMG_Utility_Change_Screen_Color_While_In_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (Commands->Is_A_Star(enterer))
	{
		Set_Screen_Fade_Color_Player(enterer,Get_Vector3_Parameter("Color[Red|Green|Blue]").X,Get_Vector3_Parameter("Color[Red|Green|Blue]").Y,Get_Vector3_Parameter("Color[Red|Green|Blue]").Z,Get_Float_Parameter("EnterFadeInTime"));
		Set_Screen_Fade_Opacity_Player(enterer,Get_Float_Parameter("Opacity"),Get_Float_Parameter("EnterFadeInTime"));
		Attach_Script_Once(enterer,"JMG_Utility_Reset_Screen_Color_When_Destroyed","");
	}
}
void JMG_Utility_Change_Screen_Color_While_In_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (Commands->Is_A_Star(exiter))
	{
		Set_Screen_Fade_Color_Player(exiter,0.0f,0.0f,0.0f,Get_Float_Parameter("ExitFadeOutTime"));
		Set_Screen_Fade_Opacity_Player(exiter,0.0f,Get_Float_Parameter("ExitFadeOutTime"));
		if (Is_Script_Attached(exiter,"JMG_Utility_Reset_Screen_Color_When_Destroyed"))
			Remove_Script(exiter,"JMG_Utility_Reset_Screen_Color_When_Destroyed");
	}
}
void JMG_Utility_Reset_Screen_Color_When_Destroyed::Destroyed(GameObject *obj)
{
	if (!Commands->Is_A_Star(obj))
		return;
	Set_Screen_Fade_Color_Player(obj,0.0f,0.0f,0.0f,0.0f);
	Set_Screen_Fade_Opacity_Player(obj,0.0f,0.0f);
}
char JMG_Utility_Play_Music_On_Join_Controller::musicFileName[256] = {"null.mp3"};
bool JMG_Utility_Play_Music_On_Join_Controller::controllerPlaced = false;
void JMG_Utility_Play_Music_On_Join_Controller::Created(GameObject *obj)
{
	if (controllerPlaced)
	{
		Console_Input("msg JMG_Utility_Play_Music_On_Join_Controller ERROR: Only place one controller on each map!");
		return;
	}
	controllerPlaced = true;
	sprintf(JMG_Utility_Play_Music_On_Join_Controller::musicFileName,"%s",Get_Parameter("Music"));
	for (int x = 1;x < 128;x++)
		playingMusic[x] = false;
	Commands->Start_Timer(obj,this,1.0,23453451);
}
void JMG_Utility_Play_Music_On_Join_Controller::Timer_Expired(GameObject *obj,int number)
{
	if (number == 23453451) 
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (player && !playingMusic[x])
			{
				playingMusic[x] = true;
				Set_Background_Music_Player(player,JMG_Utility_Play_Music_On_Join_Controller::musicFileName);
			}
			if (!player && playingMusic[x])
				playingMusic[x] = false;
		}
		Commands->Start_Timer(obj,this,1.0,23453451);
	}
}
void JMG_Utility_Play_Music_On_Join_Controller::Destroyed(GameObject *obj)
{
	controllerPlaced = false;
}
void JMG_Utility_Play_Music_On_Join_Controller::Detach(GameObject *obj)
{
	if (Exe == 4)
		return;
	Destroyed(obj);
}
void JMG_Utility_Play_Music_On_Join_Controller::Set_Music(const char *musicFilName,int fadeOut,int fadeIn)
{
	sprintf(JMG_Utility_Play_Music_On_Join_Controller::musicFileName,"%s",musicFilName);
	if (fadeOut || fadeIn)
		Commands->Fade_Background_Music(musicFilName,fadeOut,fadeIn);
	else
		Commands->Set_Background_Music(musicFilName);
}
void JMG_Utility_Play_Music_On_Join_Change_Music::Created(GameObject *obj)
{
	if (!JMG_Utility_Play_Music_On_Join_Controller::controllerPlaced)
	{
		Console_Input("msg JMG_Utility_Play_Music_On_Join_Change_Music ERROR: You need to place JMG_Utility_Play_Music_On_Join_Controller on an object in order to use this script!");
		return;
	}
	JMG_Utility_Play_Music_On_Join_Controller::Set_Music(Get_Parameter("Music"),Get_Int_Parameter("FadeOut"),Get_Int_Parameter("FadeIn"));
	Destroy_Script();
}
void JMG_Utility_Toggle_Door::Created(GameObject *obj)
{
	preDamagedFrame = 0.0f;
	sprintf(originalModel,"%s",Get_Model(obj));
	enabled = true;
	open = false;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	if (_stricmp(Get_Parameter("SwitchAnimation"),"null"))
		Commands->Set_Animation(obj,Get_Parameter("SwitchAnimation"),false,0,0.0f,0.0f,false);
}
void JMG_Utility_Toggle_Door::Poked(GameObject *obj,GameObject *poker)
{
	GameObject *theDoor = Commands->Find_Object(Get_Int_Parameter("DoorID"));
	if (poker != obj)
	{
		if (!theDoor)
		{
			if (_stricmp(Get_Parameter("NoDoorMessage"),"null"))
					JmgUtility::DisplayChatMessage(poker,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").X,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Y,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Z,Get_Parameter("NoDoorMessage"));
			if (_stricmp(Get_Parameter("SwitchBlockedSound"),"null"))
					Commands->Create_Sound(Get_Parameter("SwitchBlockedSound"),Commands->Get_Position(obj),obj);
			SendCustom(obj,6);
			return;
		}
		if (!enabled)
		{
			if (_stricmp(Get_Parameter("SwitchDamagedPokeMessage"),"null"))
					JmgUtility::DisplayChatMessage(poker,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").X,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Y,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Z,Get_Parameter("SwitchDamagedPokeMessage"));
			if (_stricmp(Get_Parameter("SwitchDamagedPokeSound"),"null"))
					Commands->Create_Sound(Get_Parameter("SwitchDamagedPokeSound"),Commands->Get_Position(obj),obj);
			SendCustom(obj,5);
			return;
		}
		if ((Get_Int_Parameter("Key") && !Commands->Has_Key(poker,Get_Int_Parameter("Key"))))
		{
			if (_stricmp(Get_Parameter("WrongKeyMessage"),"null"))
				JmgUtility::DisplayChatMessage(poker,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").X,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Y,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Z,Get_Parameter("WrongKeyMessage"));
			if (_stricmp(Get_Parameter("WrongKeySound"),"null"))
				Commands->Create_Sound(Get_Parameter("WrongKeySound"),Commands->Get_Position(obj),obj);
			SendCustom(obj,4);
			return;
		}
		if (CheckPlayerType(poker,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(poker) == -4)
		{
			if (_stricmp(Get_Parameter("WrongPlayerTypeMessage"),"null"))
				JmgUtility::DisplayChatMessage(poker,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").X,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Y,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Z,Get_Parameter("WrongPlayerTypeMessage"));
			if (_stricmp(Get_Parameter("WrongPlayerTypeSound"),"null"))
				Commands->Create_Sound(Get_Parameter("WrongPlayerTypeSound"),Commands->Get_Position(obj),obj);
			SendCustom(obj,3);
			return;
		}
	}
	if (!enabled)
		return;
	if (!open)
	{
		open = true;
		if (Get_Animation_Frame(theDoor) == 0.0f)
		{
			Commands->Set_Animation(theDoor,Get_Parameter("Animation"),false,0,0.0f,Get_Float_Parameter("LastFrame"),false);
			if (_stricmp(Get_Parameter("GotoLastFrameMessage"),"null"))
				JmgUtility::DisplayChatMessage(poker,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").X,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Y,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Z,Get_Parameter("GotoLastFrameMessage"));
			if (_stricmp(Get_Parameter("SwitchAnimation"),"null"))
				Commands->Set_Animation(obj,Get_Parameter("SwitchAnimation"),false,0,0.0f,Get_Float_Parameter("SwitchLastFrame"),false);
			if (_stricmp(Get_Parameter("DoorSound"),"null"))
				Commands->Create_Sound(Get_Parameter("DoorSound"),Commands->Get_Position(theDoor),theDoor);
			if (_stricmp(Get_Parameter("SwitchSound"),"null"))
				Commands->Create_Sound(Get_Parameter("SwitchSound"),Commands->Get_Position(obj),obj);
			SendCustom(obj,1);

		}
		else
		{
			if (_stricmp(Get_Parameter("ActionBlockedMessage"),"null"))
				JmgUtility::DisplayChatMessage(poker,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").X,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Y,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Z,Get_Parameter("ActionBlockedMessage"));
			if (_stricmp(Get_Parameter("SwitchBlockedSound"),"null"))
				Commands->Create_Sound(Get_Parameter("SwitchBlockedSound"),Commands->Get_Position(obj),obj);
			SendCustom(obj,2);
		}

	}
	else
	{
		open = false;
		if (Get_Animation_Frame(theDoor) == Get_Float_Parameter("LastFrame"))
		{
			Commands->Set_Animation(theDoor,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0.0f,false);
			if (_stricmp(Get_Parameter("GotoFirstFrameMessage"),"null"))
				JmgUtility::DisplayChatMessage(poker,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").X,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Y,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Z,Get_Parameter("GotoFirstFrameMessage"));
			if (_stricmp(Get_Parameter("SwitchAnimation"),"null"))
				Commands->Set_Animation(obj,Get_Parameter("SwitchAnimation"),false,0,Get_Float_Parameter("SwitchLastFrame"),0.0f,false);
			if (_stricmp(Get_Parameter("DoorSound"),"null"))
				Commands->Create_Sound(Get_Parameter("DoorSound"),Commands->Get_Position(theDoor),theDoor);
			if (_stricmp(Get_Parameter("SwitchSound"),"null"))
				Commands->Create_Sound(Get_Parameter("SwitchSound"),Commands->Get_Position(obj),obj);
			SendCustom(obj,0);
		}
		else
		{
			if (_stricmp(Get_Parameter("ActionBlockedMessage"),"null"))
				JmgUtility::DisplayChatMessage(poker,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").X,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Y,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Z,Get_Parameter("ActionBlockedMessage"));
			if (_stricmp(Get_Parameter("SwitchBlockedSound"),"null"))
				Commands->Create_Sound(Get_Parameter("SwitchBlockedSound"),Commands->Get_Position(obj),obj);
			SendCustom(obj,2);
		}
	}
}
void JMG_Utility_Toggle_Door::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damage || !Get_Int_Parameter("DisableSwitchWhenNoShield"))
		return;
	if (enabled && Commands->Get_Shield_Strength(obj) <= 0)
	{
		enabled = false;
		if (_stricmp(Get_Parameter("SwitchAnimation"),"null"))
			preDamagedFrame = Get_Animation_Frame(obj);
		if (_stricmp(Get_Parameter("SwitchDisabledModel"),"null"))
			Commands->Set_Model(obj,Get_Parameter("SwitchDisabledModel"));
		char animation[32];
		sprintf(animation,"%s.%s",Get_Parameter("SwitchDisabledModel"),Get_Parameter("SwitchDisabledModel"));
		Commands->Set_Animation(obj,animation,false,0,0,0,false);
		
	}
	if (!enabled && Commands->Get_Shield_Strength(obj) >= Commands->Get_Max_Shield_Strength(obj))
	{
		enabled = true;
		if (_stricmp(Get_Parameter("SwitchDisabledModel"),"null"))
			Commands->Set_Model(obj,originalModel);
		if (_stricmp(Get_Parameter("SwitchAnimation"),"null"))
			Commands->Set_Animation(obj,Get_Parameter("SwitchAnimation"),false,0,preDamagedFrame,preDamagedFrame,false);
	}
}
void JMG_Utility_Toggle_Door::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("DoorControlCustom") && Get_Int_Parameter("DoorControlCustom"))
	{
		if (param == 1 && !open)
			Poked(obj,obj);
		if (param == 0 && open)
			Poked(obj,obj);
	}
}
void JMG_Utility_Toggle_Door::SendCustom(GameObject *obj,int param)
{
	if (!Get_Int_Parameter("SendCustom") || !Get_Int_Parameter("SendCustomId"))
		return;
	GameObject *sendObj = Commands->Find_Object(Get_Int_Parameter("SendCustomId"));
	if (sendObj)
		Commands->Send_Custom_Event(obj,sendObj,Get_Int_Parameter("SendCustom"),param,0);
}
void JMG_Utility_Set_Animation_Frame_On_Creation::Created(GameObject *obj)
{
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("Frame"),Get_Float_Parameter("Frame"),false);
}
void JMG_Utility_Set_Model_On_Damage_Percent_3_States::Created(GameObject *obj)
{
	healthThresholds[0] = 0.0f;
	healthThresholds[1] = Commands->Get_Max_Shield_Strength(obj)*0.5f;
	healthThresholds[2] = Commands->Get_Max_Shield_Strength(obj);
	damageState =  2;
	if (Commands->Get_Shield_Strength(obj) != Commands->Get_Max_Shield_Strength(obj))
	{
		damageState =  1;
		Commands->Set_Model(obj,Get_Parameter("HalfShieldModel"));
		SetModel(obj);
	}
}
void JMG_Utility_Set_Model_On_Damage_Percent_3_States::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damage)
		return;
	SetModel(obj);
}
void JMG_Utility_Set_Model_On_Damage_Percent_3_States::SetModel(GameObject *obj)
{
	if (Commands->Get_Shield_Strength(obj) >= healthThresholds[2] && damageState != 2)
	{
		damageState = 2;
		Commands->Set_Model(obj,Get_Parameter("FullShieldModel"));
	}
	if (Commands->Get_Shield_Strength(obj) <= healthThresholds[0] && damageState != 0)
	{
		damageState = 0;
		Commands->Set_Model(obj,Get_Parameter("NoShieldModel"));
	}
	if ((!damageState && Commands->Get_Shield_Strength(obj) >= healthThresholds[1]) || (damageState == 2 && Commands->Get_Shield_Strength(obj) <= healthThresholds[1]))
	{
		damageState = 1;
		Commands->Set_Model(obj,Get_Parameter("HalfShieldModel"));
	}
}
void JMG_Utility_Set_Model_On_Damage_Percent_4_States::Created(GameObject *obj)
{
	healthThresholds[0] = 0.0f;
	healthThresholds[1] = Commands->Get_Max_Shield_Strength(obj)*0.33f;
	healthThresholds[2] = Commands->Get_Max_Shield_Strength(obj)*0.66f;
	healthThresholds[3] = Commands->Get_Max_Shield_Strength(obj);
	damageState =  3;
	if (Commands->Get_Shield_Strength(obj) != Commands->Get_Max_Shield_Strength(obj))
	{
		damageState =  1;
		Commands->Set_Model(obj,Get_Parameter("HalfShieldModel"));
		SetModel(obj);
	}
}
void JMG_Utility_Set_Model_On_Damage_Percent_4_States::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damage)
		return;
	SetModel(obj);
}
void JMG_Utility_Set_Model_On_Damage_Percent_4_States::SetModel(GameObject *obj)
{
	if (Commands->Get_Shield_Strength(obj) >= healthThresholds[3] && damageState != 3)
	{
		damageState = 3;
		Commands->Set_Model(obj,Get_Parameter("FullShieldModel"));
	}
	if (Commands->Get_Shield_Strength(obj) <= healthThresholds[0] && damageState != 0)
	{
		damageState = 0;
		Commands->Set_Model(obj,Get_Parameter("NoShieldModel"));
	}
	if ((!damageState && Commands->Get_Shield_Strength(obj) >= healthThresholds[1]) || (damageState == 2 && Commands->Get_Shield_Strength(obj) <= healthThresholds[1]))
	{
		damageState = 1;
		Commands->Set_Model(obj,Get_Parameter("ThirdShieldModel"));
	}
	if ((damageState == 1 && Commands->Get_Shield_Strength(obj) >= healthThresholds[2]) || (damageState == 3 && Commands->Get_Shield_Strength(obj) <= healthThresholds[2]))
	{
		damageState = 2;
		Commands->Set_Model(obj,Get_Parameter("TwoThirdsShieldModel"));
	}
}
int JMG_Utility_PCT::pctInaccessible[128] = {0};
void JMG_Utility_PCT::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void JMG_Utility_PCT::Poked(GameObject *obj,GameObject *poker)
{
	if (Commands->Get_Player_Type(poker) != Commands->Get_Player_Type(obj))
	{
		char displayMsg[220];
		sprintf(displayMsg,"pamsg %d %s",JmgUtility::JMG_Get_Player_ID(poker),Get_Translated_String(8780));
		Console_Input(displayMsg);
		return;
	}
	if (JMG_Utility_PCT::pctInaccessible[JmgUtility::JMG_Get_Player_ID(poker)])
	{
		char displayMsg[220];
		sprintf(displayMsg,"pamsg %d You cannot access this terminal from your current location.",JmgUtility::JMG_Get_Player_ID(poker));
		Console_Input(displayMsg);
		return;
	}
	if (!Commands->Get_Player_Type(poker))
		Display_NOD_Player_Terminal_Player(poker);
	else
		Display_GDI_Player_Terminal_Player(poker);
}
void JMG_Utility_PCT_Inaccessible_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (Commands->Is_A_Star(enterer))
	{
		JMG_Utility_PCT::pctInaccessible[JmgUtility::JMG_Get_Player_ID(enterer)]++;
		Attach_Script_Once(enterer,"JMG_Utility_PCT_Inaccessible_Zone_Attach","");
	}
}
void JMG_Utility_PCT_Inaccessible_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (Commands->Is_A_Star(exiter) && JMG_Utility_PCT::pctInaccessible[JmgUtility::JMG_Get_Player_ID(exiter)])
		JMG_Utility_PCT::pctInaccessible[JmgUtility::JMG_Get_Player_ID(exiter)]--;
}
void JMG_Utility_PCT_Inaccessible_Zone_Attach::Destroyed(GameObject *obj)
{
		JMG_Utility_PCT::pctInaccessible[JmgUtility::JMG_Get_Player_ID(obj)] = 0;
}
void JMG_Utility_Display_HUD_Info_Text_All_Players_Custom_Replace_String::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		JmgUtility::SetHUDHelpText(Get_Int_Parameter("StringId"),Get_Parameter("ReplaceString"),Get_Vector3_Parameter("ColorRGB"));
		if (!Get_Int_Parameter("Repeatable"))
			Destroy_Script();
	}
}
void JMG_Utility_Display_Text_Message_To_All_Players_On_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		JmgUtility::MessageAllPlayers((int)Get_Vector3_Parameter("ColorRGB").X,(int)Get_Vector3_Parameter("ColorRGB").Y,(int)Get_Vector3_Parameter("ColorRGB").Z,Get_Parameter("Message"));
		if (!Get_Int_Parameter("Repeatable"))
			Destroy_Script();
	}
}
void JMG_Utility_Set_Team_On_Create::Created(GameObject *obj)
{
	if (!Get_Float_Parameter("Delay"))
	{
		Commands->Set_Player_Type(obj,Get_Int_Parameter("PlayerType"));
		Destroy_Script();
		return;
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
}
void JMG_Utility_Set_Team_On_Create::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		Commands->Set_Player_Type(obj,Get_Int_Parameter("PlayerType"));
		Destroy_Script();
	}
}
void JMG_Utility_AI_Aggressive_Melee::Created(GameObject *obj)
{
	LastSeen = 0;
	currentTargetID = 0;
	secondaryTargetId = 0;
	lastSeenSecondary = 0;
	huntorattack = 0;
	waitcount = 0;
	speed = 2.0f;
	stuckTime = 0;
	lastPos = homelocation = Commands->Get_Position(obj);
	noPathfindRange = Get_Float_Parameter("NoPathfindDistance")*Get_Float_Parameter("NoPathfindDistance");
	minVisibilityTime = Get_Int_Parameter("MinLooseVisibilityTime")*10;
	maxVisibilityTime = Get_Int_Parameter("MaxLooseVisibilityTime")*10;
	maxHuntDistance = Get_Float_Parameter("MaxHuntDistance")*Get_Float_Parameter("MaxHuntDistance");
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,0.1f,92562343);
}
void JMG_Utility_AI_Aggressive_Melee::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	if (!Commands->Get_Health(seen))
		return;
	int SeenID = Commands->Get_ID(seen);
	if (!currentTargetID || !LastSeen)
	{
		LastSeen = minVisibilityTime == maxVisibilityTime ? maxVisibilityTime : Commands->Get_Random_Int(minVisibilityTime,maxVisibilityTime);
		huntorattack = 0;
		currentTargetID = SeenID;
	}
	else if (currentTargetID == SeenID)
		LastSeen = minVisibilityTime == maxVisibilityTime ? maxVisibilityTime : Commands->Get_Random_Int(minVisibilityTime,maxVisibilityTime);
	else if (!secondaryTargetId || !lastSeenSecondary)
	{
		lastSeenSecondary = maxVisibilityTime;
		secondaryTargetId = SeenID;
	}
	else if (secondaryTargetId == SeenID)
		lastSeenSecondary = maxVisibilityTime;
}
void JMG_Utility_AI_Aggressive_Melee::Timer_Expired(GameObject *obj,int number)
{
	if (92562343 == number)
	{
		if (LastSeen)
		{
			LastSeen--;
			if (!LastSeen)
				currentTargetID = 0;
		}
		if (lastSeenSecondary)
		{
			lastSeenSecondary--;
			if (!lastSeenSecondary)
				secondaryTargetId = 0;
		}
		if (currentTargetID)
		{
			GameObject *target = Commands->Find_Object(currentTargetID);
			GameObject *target2 = Commands->Find_Object(secondaryTargetId);
			if (!target2 || Commands->Get_Health(target2) == 0.0f)
				secondaryTargetId = 0;
			if (!target || Commands->Get_Health(target) == 0.0f)
			{
				if (target2)
				{
					currentTargetID = secondaryTargetId;
					LastSeen = maxVisibilityTime;
				}
				else
				{
					waitcount = 0;
					currentTargetID = 0;
					Commands->Action_Reset(obj,10);
					ActionParamsStruct params;
					params.Set_Basic(this,999,10);
					params.Set_Movement(homelocation,Get_Float_Parameter("ReturnHomeSpeed"),Get_Float_Parameter("ReturnHomeArriveDistance"),false);
					Commands->Action_Goto(obj,params);
				}
			}
			else if (Get_Float_Parameter("MaxHuntDistance") && JmgUtility::SimpleDistance(homelocation,Commands->Get_Position(target)) > maxHuntDistance)
			{
				currentTargetID = 0;
				waitcount = 0;
				if (huntorattack == 1)
					Commands->Action_Reset(obj,10);
			}
			else
			{
				waitcount++;
				if (waitcount > 2)
				{
					waitcount = 0;
					ActionParamsStruct params;
					params.Set_Movement(target,Get_Float_Parameter("ChaseSpeed"),Get_Float_Parameter("AttackArriveDistance"),false);
					params.MovePathfind = JmgUtility::SimpleDistance(Commands->Get_Position(target),Commands->Get_Position(obj)) <= noPathfindRange ? false : true;
					Commands->Action_Goto(obj,params);
					params.Set_Basic(this,999,10);
					params.Set_Attack(target,Get_Float_Parameter("StartAttackDistance"),1,true);
					Commands->Action_Attack(obj,params);
				}
			}
		}
		Commands->Start_Timer(obj,this,0.1f,92562343);
	}
}
void JMG_Utility_AI_Aggressive_Melee::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damager || Commands->Get_Player_Type(damager) != 1 || currentTargetID || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (!chooseTarget(obj,damager,&currentTargetID,&LastSeen))
		chooseTarget(obj,damager,&secondaryTargetId,&lastSeenSecondary);
	else
	{
		huntorattack = 0;
		waitcount = 0;
	}
}
bool JMG_Utility_AI_Aggressive_Melee::chooseTarget(GameObject *obj,GameObject *damager,int *compareId,int *seenTimer)
{
	GameObject *target = Commands->Find_Object(*compareId);
	if (target)
	{
		float targetDistance = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target));
		float damagerDistance = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(damager));
		if (targetDistance < damagerDistance)
			return false;
	}
	*seenTimer = minVisibilityTime == maxVisibilityTime ? maxVisibilityTime : Commands->Get_Random_Int(minVisibilityTime,maxVisibilityTime);
	*compareId = Commands->Get_ID(damager);
	return true;
}
void JMG_Utility_Infantry_Placed_Buildable_Object::Created(GameObject *obj)
{
	canRegen = false;
	previewObjectId = 0;
	reloadTime = 0;
	if (!Has_Weapon(obj,Get_Parameter("WeaponPreset")) && Get_Int_Parameter("GrantWeapon"))
		Grant_Weapon(obj,Get_Parameter("WeaponPreset"),true,1,0);
	Commands->Start_Timer(obj,this,0.25f,1);
	Commands->Start_Timer(obj,this,1.0f,2);
}
void JMG_Utility_Infantry_Placed_Buildable_Object::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!Commands->Get_Health(obj))
			return;
		const char *weap = Get_Current_Weapon(obj);
		if (weap && !_stricmp(weap,Get_Parameter("WeaponPreset")) && !Get_Vehicle(obj) && !reloadTime)
		{
			if (!previewObjectId)
			{
				GameObject *preview = Commands->Create_Object(Get_Parameter("PreviewPreset"),Commands->Get_Position(obj));
				previewObjectId = Commands->Get_ID(preview);
				Update_Network_Object(preview);
				Set_Object_Visibility(previewObjectId,false);
				if ((int)Get_Vector3_Parameter("MatchTeam[Preview|Deployed|Repaired]").X)
					Commands->Set_Player_Type(preview,Commands->Get_Player_Type(obj));
				Commands->Attach_To_Object_Bone(preview,obj,"origin");
				if (!placementBlocked)
					Commands->Set_Model(preview,Get_Parameter("PlaceablePreview"));
				else
					Commands->Set_Model(preview,Get_Parameter("UnPlaceablePreview"));
				Set_Object_Visibility_For_Player(obj,previewObjectId,true);
			}
		}
		else if (previewObjectId)
			DestroyPreview();
		Commands->Start_Timer(obj,this,0.25f,1);
	}
	if (number == 2)
	{
		if (reloadTime && canRegen)
		{
			reloadTime--;
			if (!reloadTime)
			{
				Set_Bullets(obj,Get_Parameter("WeaponPreset"),1);
				JmgUtility::DisplayChatMessage(obj,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").X,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Y,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Z,Get_Parameter("PlacementMessage")); 
			}
		}
		Commands->Start_Timer(obj,this,1.0f,2);
	}
}
void JMG_Utility_Infantry_Placed_Buildable_Object::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("DeployCustom"))
	{
		if (Get_Vehicle(obj) || !Commands->Get_Health(obj))
			return;
		const char *weap = Get_Current_Weapon(obj);
		if (!weap || _stricmp(weap,Get_Parameter("WeaponPreset")))
			return;
		if (obj->As_SoldierGameObj() && obj->As_SoldierGameObj()->Is_Airborne())
		{
			JmgUtility::DisplayChatMessage(obj,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").X,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Y,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Z,"You must be on the ground to place this!");
			return;
		}
		if (obj->As_SoldierGameObj() && obj->As_SoldierGameObj()->Is_On_Ladder())
		{
			JmgUtility::DisplayChatMessage(obj,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").X,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Y,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Z,"You cannot place this while climbing ladders!");
			return;
		}
		if (placementBlocked)
		{
			JmgUtility::DisplayChatMessage(obj,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").X,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Y,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Z,Get_Parameter("DeploymentBlockedMessage"));
			return;
		}
		if (!canRegen && reloadTime)
		{
			JmgUtility::DisplayChatMessage(obj,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").X,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Y,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Z,"You must repair the last one you placed before you can place another.");
			return;
		}
		if (reloadTime)
		{
			char reloadTimeMessage[220];
			sprintf(reloadTimeMessage,"You must wait another %d seconds before you can place this.",reloadTime);
			JmgUtility::DisplayChatMessage(obj,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").X,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Y,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Z,reloadTimeMessage);
			return;
		}
		if (Get_Float_Parameter("Cost") && Commands->Get_Money(obj) < Get_Float_Parameter("Cost"))
		{
			char costMessage[220];
			sprintf(costMessage,"You need $%s to place this.",JmgUtility::formatDigitGrouping(Get_Float_Parameter("Cost")));
			JmgUtility::DisplayChatMessage(obj,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").X,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Y,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Z,costMessage);
			return;
		}
		if (Get_Float_Parameter("Cost"))
			Commands->Give_Money(obj,-Get_Float_Parameter("Cost"),false);
		canRegen = Get_Int_Parameter("RegenStartsAfterRepair") ? false : true;
		reloadTime = Get_Int_Parameter("RegainTimer");
		Set_Bullets(obj,Get_Parameter("WeaponPreset"),0);
		GameObject *preview = Commands->Find_Object(previewObjectId);
		if (preview)
			Commands->Destroy_Object(preview);
		previewObjectId = 0;
		GameObject *placed = Commands->Create_Object(Get_Parameter("DeployPreset"),Commands->Get_Position(obj));
		if ((int)Get_Vector3_Parameter("MatchTeam[Preview|Deployed|Repaired]").Y)
			Commands->Set_Player_Type(placed,Commands->Get_Player_Type(obj));
		Commands->Set_Facing(placed,Commands->Get_Facing(obj));
		char params[220];
		sprintf(params,"%d,%d,%s,%.0f,%d",this->Get_ID(),Commands->Get_ID(obj),Get_Parameter("RepairedPreset"),Get_Vector3_Parameter("MatchTeam[Preview|Deployed|Repaired]").Z,Commands->Get_Player_Type(obj));
		Commands->Attach_Script(placed,"JMG_Utility_Infantry_Placed_Buildable_Object_Attached",params);
	}
	if (message == Get_Int_Parameter("EnableCustom"))
	{
		if (placementBlocked)
			placementBlocked--;
		if (!placementBlocked)
		{
			GameObject *preview = Commands->Find_Object(previewObjectId);
			if (preview)
				Commands->Set_Model(preview,Get_Parameter("PlaceablePreview"));
		}
	}
	if (message == Get_Int_Parameter("DisableCustom"))
	{
		if (!placementBlocked)
		{
			GameObject *preview = Commands->Find_Object(previewObjectId);
			if (preview)
				Commands->Set_Model(preview,Get_Parameter("UnPlaceablePreview"));
		}
		placementBlocked++;
	}
	if (message == 9117115 && param == this->Get_ID())
	{
		reloadTime = Get_Int_Parameter("DestroyedRegenTime");
		canRegen = true;
	}
	if (message == 9117116 && param == this->Get_ID())
	{
		reloadTime = 0;
		Set_Bullets(obj,Get_Parameter("WeaponPreset"),1);
	}
	if (message == 9117117 && param == this->Get_ID())
	{
		canRegen = true;
	}
	if (message == 9117118 && param == this->Get_ID())
	{
		if (Get_Float_Parameter("Cost"))
			Commands->Give_Money(obj,Get_Float_Parameter("Cost"),false);
	}
}
void JMG_Utility_Infantry_Placed_Buildable_Object::Killed(GameObject *obj,GameObject *killer)
{
	DestroyPreview();
}
void JMG_Utility_Infantry_Placed_Buildable_Object::Destroyed(GameObject *obj)
{
	DestroyPreview();
}
void JMG_Utility_Infantry_Placed_Buildable_Object::DestroyPreview()
{
	GameObject *preview = Commands->Find_Object(previewObjectId);
	if (preview)
		Commands->Destroy_Object(preview);
	previewObjectId = 0;
}
void JMG_Utility_Infantry_Placed_Buildable_Object_Attached::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage < 0 && Get_Hitpoints(obj) == Get_Max_Hitpoints(obj))
	{
		GameObject *placed = Commands->Create_Object(Get_Parameter("RepairedPreset"),Commands->Get_Position(obj));
		if (Get_Int_Parameter("MatchTeam"))
			Commands->Set_Player_Type(placed,Get_Int_Parameter("Team"));
		Commands->Set_Facing(placed,Commands->Get_Facing(obj));
		Commands->Destroy_Object(obj);
		GameObject *placer = Commands->Find_Object(Get_Int_Parameter("PlacerId"));
		if (placer)
			Commands->Send_Custom_Event(obj,placer,9117117,Get_Int_Parameter("ScriptId"),0);
	}
}
void JMG_Utility_Infantry_Placed_Buildable_Object_Attached::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *placer = Commands->Find_Object(Get_Int_Parameter("PlacerId"));
	if (Commands->Get_Player_Type(placer) == Commands->Get_Player_Type(killer))
		Commands->Send_Custom_Event(obj,placer,9117116,Get_Int_Parameter("ScriptId"),0);
	else if (placer)
		Commands->Send_Custom_Event(obj,placer,9117115,Get_Int_Parameter("ScriptId"),0);
	Commands->Send_Custom_Event(obj,placer,9117118,Get_Int_Parameter("ScriptId"),0);
}
void JMG_Utility_Lock_Weapon_Selection_While_Script_Attached::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Lock_Weapon_Selection_While_Script_Attached::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		const char *weap = Get_Current_Weapon(obj);
		if (Has_Weapon(obj,Get_Parameter("WeaponPreset")) && (!weap || _stricmp(weap,Get_Parameter("WeaponPreset"))))
			Commands->Select_Weapon(obj,Get_Parameter("WeaponPreset"));
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
float JMG_Utility_Swimming_Zone::fogMinDistance = 200.0f;
float JMG_Utility_Swimming_Zone::fogMaxDistance = 300.0f;
void JMG_Utility_Swimming_zDefault_Map_Fog_Values::Created(GameObject *obj)
{
	Get_Fog_Range(JMG_Utility_Swimming_Zone::fogMinDistance,JMG_Utility_Swimming_Zone::fogMaxDistance);
}
void JMG_Utility_Swimming_Zone::Created(GameObject *obj)
{
	Get_Fog_Range(fogMinDistance,fogMaxDistance);
}
void JMG_Utility_Swimming_Zone::Entered(GameObject *obj,GameObject *enter)
{
	if (!enter->As_SoldierGameObj())
		return;
	if (!Is_Script_Attached(enter,"JMG_Utility_Swimming_Infantry") && !Is_Script_Attached(enter,"JMG_Utility_Swimming_Infantry_Advanced") && !Is_Script_Attached(enter,"JMG_Utility_Swimming_Infantry_AI") && !Is_Script_Attached(enter,"JMG_Utility_Swimming_Infantry_Advanced_AI"))
	{
		Commands->Apply_Damage(enter,9999.9f,"Death",obj);
		if (Commands->Is_A_Star(enter) && !Commands->Get_Health(enter))
		{
			Commands->Attach_Script(enter,"JMG_Utility_Reset_Screen_Fade_And_Fog_On_Destroy","");
			Set_Screen_Fade_Color_Player(enter,Get_Vector3_Parameter("WaterColor[R|G|B]").X,Get_Vector3_Parameter("WaterColor[R|G|B]").Y,Get_Vector3_Parameter("WaterColor[R|G|B]").Z,0.0f);
			Set_Screen_Fade_Opacity_Player(enter,Get_Float_Parameter("WaterOpacity"),0.1f);
			Set_Fog_Range_Player(obj,Get_Float_Parameter("WaterMinViewDistance"),JMG_Utility_Swimming_Zone::Get_Float_Parameter("WaterMaxViewDistance"),0.1f);
		}
		return;
	}
	waterNode[JmgUtility::JMG_Get_Player_ID(enter)] = PlayerWaterNode(Get_Vector3_Parameter("WaterColor[R|G|B]"),Get_Float_Parameter("WaterOpacity"),Get_Float_Parameter("WaterMinViewDistance"),Get_Float_Parameter("WaterMaxViewDistance"));
	Commands->Send_Custom_Event(obj,enter,347341,(int)(Get_Float_Parameter("SwimmingSpeedMultiplier")*100),0);
}
void JMG_Utility_Swimming_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (!exiter->As_SoldierGameObj())
		return;
	Commands->Send_Custom_Event(obj,exiter,347340,0,0);
}
void JMG_Utility_Swimming_Zone::Update_Fog_Settings(float minFog,float maxFog)
{
	fogMinDistance = minFog;
	fogMaxDistance = maxFog;
}
JMG_Utility_Swimming_Zone::PlayerWaterNode JMG_Utility_Swimming_Zone::waterNode[128] = {JMG_Utility_Swimming_Zone::PlayerWaterNode()};
bool JMG_Utility_Swimming_Infantry::isUnderwater[128] = {false};
bool JMG_Utility_Swimming_Infantry::isInWater[128] = {false};
void JMG_Utility_Swimming_Infantry::Created(GameObject *obj)
{
	if (!Commands->Is_A_Star(obj))
	{
		char params[1024];
		sprintf(params,"%s,%.2f,%.2f,%s,%s,%s,%.2f%,%.2f,%.2f,%s,%d,%d,%s,%s,%s",Get_Parameter("WeaponPreset"),Get_Float_Parameter("DrownTime"),Get_Float_Parameter("StartDrownSequence"),Get_Parameter("GaspForBreath"),Get_Parameter("PantingSoundEmitterModel"),Get_Parameter("HeartBeatSoundEmitterModel"),Get_Float_Parameter("DrownDamageRate"),Get_Float_Parameter("CatchBreathRate"),Get_Float_Parameter("WaterDamageAmount"),Get_Parameter("WaterDamageWarhead"),Get_Int_Parameter("WaterDamageDelayTime"),Get_Int_Parameter("WaterDamageDelayTimeRecover"),Get_Parameter("SwimmingSkin"),Get_Parameter("SwimmingArmor"),Get_Parameter("SwimmingModel"));
		Commands->Attach_Script(obj,"JMG_Utility_Swimming_Infantry_AI",params);
		Destroy_Script();
		return;
	}
	lastDisplayTime = NULL;
	lastWaterZoneId = 0;
	heartBeatSoundId = 0;
	pantSoundId = 0;
	playerId = JmgUtility::JMG_Get_Player_ID(obj);
	underwater = false;
	waterZoneCount = 0;
	startedFadeRed = false;
	drownTime = 0.0f;
	defaultSpeed = obj->As_SoldierGameObj()->Get_Max_Speed();
	waterDamageDelayTime = remainingWaterDamageDelay = Get_Int_Parameter("WaterDamageDelayTime");
	waterDamageDelayTimeRecover = Get_Int_Parameter("WaterDamageDelayTimeRecover");
	sprintf(originalSkin,"");
	sprintf(originalArmor,"");
	sprintf(originalModel,"");
	for (SLNode<ScriptZoneGameObj>* node = GameObjManager::ScriptZoneGameObjList.Head();node;node = node->Next())
	{
		GameObject *zone = (GameObject *)node->Data();
		if (Is_Script_Attached(zone,"JMG_Utility_Swimming_Zone") && IsInsideZone(zone,obj))
			Commands->Send_Custom_Event(obj,obj,347341,347341,0.25f);
	}
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Swimming_Infantry::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && Commands->Get_Health(obj))
	{
		if (obj->As_SoldierGameObj()->Is_Crouched() && !obj->As_SoldierGameObj()->Is_Airborne() && waterZoneCount)
		{
			if (!underwater)
			{
				underwater = true;
				isUnderwater[playerId] = true;
				HideSoundEmitter(&pantSoundId);
				if (startedFadeRed)
				{
					Set_Screen_Fade_Color_Player(obj,1.0f,0.0f,0.0f,JmgUtility::MathClamp(Get_Float_Parameter("DrownTime")-drownTime,0,Get_Float_Parameter("StartDrownSequence")));
					CreateSoundEmitter(obj,Get_Parameter("HeartBeatSoundEmitterModel"),&heartBeatSoundId);
				}
				else
					Set_Screen_Fade_Color_Player(obj,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.X,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.Y,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.Z,0.0f);
				Set_Screen_Fade_Opacity_Player(obj,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColorOpacity,0.1f);
				Set_Fog_Range_Player(obj,JMG_Utility_Swimming_Zone::waterNode[playerId].waterMinViewDistance,JMG_Utility_Swimming_Zone::waterNode[playerId].waterMaxViewDistance,0.1f);
			}
			drownTime += 0.1f;
			if (!startedFadeRed && drownTime >= Get_Float_Parameter("DrownTime")-Get_Float_Parameter("StartDrownSequence"))
			{
				startedFadeRed = true;
				Set_Screen_Fade_Color_Player(obj,1.0f,0.0f,0.0f,Get_Float_Parameter("StartDrownSequence"));
				CreateSoundEmitter(obj,Get_Parameter("HeartBeatSoundEmitterModel"),&heartBeatSoundId);
			}
			if (drownTime >= Get_Float_Parameter("DrownTime"))
				Commands->Apply_Damage(obj,Get_Float_Parameter("DrownDamageRate"),"None",obj);
		}
		else
		{
			if (underwater)
			{
				underwater = false;
				isUnderwater[playerId] = false;
				JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Player(obj,0.1f);
				Set_Fog_Range_Player(obj,JMG_Utility_Swimming_Zone::fogMinDistance,JMG_Utility_Swimming_Zone::fogMaxDistance,0.1f);
				HideSoundEmitter(&heartBeatSoundId);
				if (drownTime > 1.0f)
					CreateSoundEmitter(obj,Get_Parameter("PantingSoundEmitterModel"),&pantSoundId);
				if (drownTime > Get_Float_Parameter("DrownTime"))
					Commands->Create_3D_Sound_At_Bone(Get_Parameter("GaspForBreath"),obj,"c head");
			}
			if (drownTime)
			{
				drownTime -= Get_Float_Parameter("CatchBreathRate");
				if (startedFadeRed && drownTime < Get_Float_Parameter("DrownTime")-Get_Float_Parameter("StartDrownSequence"))
					startedFadeRed = false;
				if (drownTime <= 0)
				{
					drownTime = 0.0f;
					HideSoundEmitter(&pantSoundId);
				}
			}
		}
		if (waterZoneCount)
		{
			const char *weap = Get_Current_Weapon(obj);
			if (Has_Weapon(obj,Get_Parameter("WeaponPreset")) && (!weap || _stricmp(weap,Get_Parameter("WeaponPreset"))))
				Commands->Select_Weapon(obj,Get_Parameter("WeaponPreset"));
			if (remainingWaterDamageDelay)
				remainingWaterDamageDelay--;
			if (!remainingWaterDamageDelay && Get_Float_Parameter("WaterDamageAmount"))
				Commands->Apply_Damage(obj,Get_Float_Parameter("WaterDamageAmount"),Get_Parameter("WaterDamageWarhead"),Commands->Find_Object(lastWaterZoneId));
		}
		else if (!isInWater[playerId] && remainingWaterDamageDelay < waterDamageDelayTime)
		{
			remainingWaterDamageDelay += waterDamageDelayTimeRecover;
			if (remainingWaterDamageDelay > waterDamageDelayTime)
				remainingWaterDamageDelay = waterDamageDelayTime;
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Swimming_Infantry::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 347340)
	{
		Commands->Send_Custom_Event(sender,obj,347342,param,0.25f);
	}
	if (message == 347342)
	{
		if (obj->As_SoldierGameObj()->Is_Airborne())
		{
			Commands->Send_Custom_Event(sender,obj,message,param,0.25f);
			return;
		}
		waterZoneCount--;
		if (!waterZoneCount)
		{
			isInWater[playerId] = false;
			obj->As_SoldierGameObj()->Set_Can_Play_Damage_Animations(true);
			obj->As_SoldierGameObj()->Set_Movement_Loiters_Allowed(true);
			if (Has_Weapon(obj,enterWeapon))
				Commands->Select_Weapon(obj,enterWeapon);
			if (Has_Weapon(obj,Get_Parameter("WeaponPreset")))
				Remove_Weapon(obj,Get_Parameter("WeaponPreset"));
			obj->As_SoldierGameObj()->Set_Max_Speed(defaultSpeed);
			if (_stricmp(Get_Parameter("SwimmingSkin"),""))
				Set_Skin(obj,originalSkin);
			if (_stricmp(Get_Parameter("SwimmingArmor"),""))
				Commands->Set_Shield_Type(obj,originalArmor);
			if (_stricmp(Get_Parameter("SwimmingModel"),""))
				Commands->Set_Model(obj,originalModel);
		}
	}
	if (message == 347341)
	{
		lastWaterZoneId = Commands->Get_ID(sender);
		if (!waterZoneCount)
		{
			if (Get_Int_Parameter("EnterWaterMessageStringId"))
			{
				time_t currentTime = clock();
				if (difftime(currentTime,lastDisplayTime) > 10000.0f)
				{
					Set_HUD_Help_Text_Player(obj,Get_Int_Parameter("EnterWaterMessageStringId"),Get_Vector3_Parameter("WaterEnterMessageColor[R|G|B]"));
					lastDisplayTime = currentTime;
				}
				obj->As_SoldierGameObj()->Set_Max_Speed(defaultSpeed*(param/100.0f));
			}
			isInWater[playerId] = true;
			sprintf(enterWeapon,"%s",Get_Current_Weapon(obj) ? Get_Current_Weapon(obj) : "");
			Grant_Weapon(obj,Get_Parameter("WeaponPreset"),true,-1,1);
			obj->As_SoldierGameObj()->Set_Can_Play_Damage_Animations(false);
			obj->As_SoldierGameObj()->Set_Movement_Loiters_Allowed(false);
			Commands->Set_Loiters_Allowed(obj,false);
			if (_stricmp(Get_Parameter("SwimmingSkin"),""))
			{
				sprintf(originalSkin,"%s",Get_Skin(obj));
				Set_Skin(obj,Get_Parameter("SwimmingSkin"));
			}
			if (_stricmp(Get_Parameter("SwimmingArmor"),""))
			{
				sprintf(originalArmor,"%s",Get_Shield_Type(obj));
				Commands->Set_Shield_Type(obj,Get_Parameter("SwimmingArmor"));
			}
			if (_stricmp(Get_Parameter("SwimmingModel"),""))
			{
				sprintf(originalModel,"%s",Get_Model(obj));
				Commands->Set_Model(obj,Get_Parameter("SwimmingModel"));
			}
		}
		waterZoneCount++;
	}
}
void JMG_Utility_Swimming_Infantry::Killed(GameObject *obj,GameObject *killer)
{
	DestroySoundEmitter(&heartBeatSoundId);
	DestroySoundEmitter(&pantSoundId);
	if (!waterZoneCount)
		return;
	if (startedFadeRed)
	{
		Set_Screen_Fade_Color_Player(obj,1.0f,0.0f,0.0f,JmgUtility::MathClamp(Get_Float_Parameter("DrownTime")-drownTime,0,Get_Float_Parameter("StartDrownSequence")));
		CreateSoundEmitter(obj,Get_Parameter("HeartBeatSoundEmitterModel"),&heartBeatSoundId);
	}
	else
		Set_Screen_Fade_Color_Player(obj,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.X,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.Y,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.Z,0.0f);
	Set_Screen_Fade_Opacity_Player(obj,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColorOpacity,0.1f);
	Set_Fog_Range_Player(obj,JMG_Utility_Swimming_Zone::waterNode[playerId].waterMinViewDistance,JMG_Utility_Swimming_Zone::waterNode[playerId].waterMaxViewDistance,0.1f);
}
void JMG_Utility_Swimming_Infantry::Destroyed(GameObject *obj)
{
	JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Player(obj,0.0f);
	Set_Fog_Range_Player(obj,JMG_Utility_Swimming_Zone::fogMinDistance,JMG_Utility_Swimming_Zone::fogMaxDistance,0.0f);
	DestroySoundEmitter(&heartBeatSoundId);
	DestroySoundEmitter(&pantSoundId);
}
void JMG_Utility_Swimming_Infantry::Detach(GameObject *obj)
{
	if (Exe == 4)
		return;
	Destroyed(obj);
}
void JMG_Utility_Swimming_Infantry::CreateSoundEmitter(GameObject *obj,const char *model,int *soundId)
{
	GameObject *soundEmitter = Commands->Find_Object(*soundId);
	if (!soundEmitter)
	{
		soundEmitter = Commands->Create_Object("Daves Arrow",Commands->Get_Position(obj));
		*soundId = Commands->Get_ID(soundEmitter);
		Commands->Attach_To_Object_Bone(soundEmitter,obj,"c head");
	}
	Commands->Set_Model(soundEmitter,model);
}
void JMG_Utility_Swimming_Infantry::HideSoundEmitter(int *soundId)
{
	GameObject *soundEmitter = Commands->Find_Object(*soundId);
	if (soundEmitter)
		Commands->Set_Model(soundEmitter,"null");
}
void JMG_Utility_Swimming_Infantry::DestroySoundEmitter(int *soundId)
{
	GameObject *soundEmitter = Commands->Find_Object(*soundId);
	if (soundEmitter)
		Commands->Destroy_Object(soundEmitter);
	*soundId = 0;
}
void JMG_Utility_Zone_Enable_Spawners_In_Range::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	int end = Get_Int_Parameter("EndID");
	bool enable = Get_Int_Parameter("Enable") ? true : false;
	for (int x = Get_Int_Parameter("StartID");x <= end;x++)
		Commands->Enable_Spawner(x,enable);
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
void JMG_Utility_Display_Message_On_Vehicle_Enter::Created(GameObject *obj)
{
	for (int x = 0;x < 128;x++)
		hasShownMessage[x] = false;
}
void JMG_Utility_Display_Message_On_Vehicle_Enter::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		int playerId = JmgUtility::JMG_Get_Player_ID(sender);
		if (!playerId)
			return;
		if (CheckPlayerType(sender,Get_Int_Parameter("PlayerType")))
			return;
		if (Get_Int_Parameter("DriverOnly") && Get_Vehicle_Driver(obj) != sender)
			return;
		if (!Get_Int_Parameter("ShowOnce") || !hasShownMessage[playerId])
		{
			hasShownMessage[playerId] = true;
			if (_stricmp(Get_Parameter("MessageOverride"),"null"))
				Set_HUD_Help_Text_Player_Text(sender,Get_Int_Parameter("StringId"),Get_Parameter("MessageOverride"),Get_Vector3_Parameter("Color[R|G|B]"));
			else
				Set_HUD_Help_Text_Player(sender,Get_Int_Parameter("StringId"),Get_Vector3_Parameter("Color[R|G|B]"));
		}
	}
}
void JMG_Utility_Zone_Apply_Damage_On_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	GameObject *target = Get_Int_Parameter("ID") ? Commands->Find_Object(Get_Int_Parameter("ID")) : enter;
	if (target)
	{
		VehicleGameObj *vehicleGameObj = target->As_VehicleGameObj();
		if (vehicleGameObj && Get_Int_Parameter("DamageOccupants"))
		{
			int x = vehicleGameObj->Get_Definition().Get_Seat_Count();
			PhysicalGameObj *physicalGameObj = obj->As_PhysicalGameObj();
			OffenseObjectClass offenseObjectClass = OffenseObjectClass(Get_Float_Parameter("DamageAmount"),ArmorWarheadManager::Get_Warhead_Type(Get_Parameter("Warhead")),physicalGameObj ? physicalGameObj->As_ArmedGameObj() : NULL);
			for (int i = 0;i < x;i++)
			{
				SoldierGameObj *soldierGameObj = vehicleGameObj->Get_Occupant(i);
				if (soldierGameObj)
				{
					soldierGameObj->Apply_Damage_IgnoreVehicleCheck(offenseObjectClass,1,-1);
					if (!Commands->Get_Health(soldierGameObj))
					{
						if (Get_Vehicle_Driver(target) == soldierGameObj)
							Commands->Enable_Engine(target,false);
						Toggle_Fly_Mode(soldierGameObj);
						Commands->Apply_Damage(soldierGameObj,1.0,"BlamoKiller",obj);
					}
				}
			}
		}
		Commands->Apply_Damage(target,Get_Float_Parameter("DamageAmount"),Get_Parameter("Warhead"),obj);
	}
	if (Get_Int_Parameter("OnlyOnce"))
		this->Destroy_Script();
}
void JMG_Utility_AI_Guardian_Aircraft::Created(GameObject *obj)
{
	newPointDelay = Get_Int_Parameter("NewPointDelay");
	remainingDelay = 0;
	dpPosition = Commands->Get_Position(obj);
	EnemyID = 0;
	EnemyTimeOutTime = 0;
	LastPos = Commands->Get_Position(obj);
	stealthModeOverride = Get_Int_Parameter("StealthModeOverride");
	float weaponRange = 0.0f;
	const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Current_Weapon(obj),true);
	if (ammo)
		weaponRange = ammo->Range;
	fireRange = Get_Float_Parameter("FireRange") >= 0 ? Get_Float_Parameter("FireRange") : weaponRange;
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Enable_Engine(obj,true);
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_AI_Guardian_Aircraft::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (EnemyTimeOutTime)
			EnemyTimeOutTime--;
		if (!EnemyTimeOutTime)
		{
			EnemyID = 0;
			Goto_Location(obj);
		}
		if (EnemyID)
		{
			GameObject *Target = Commands->Find_Object(EnemyID);
			if (!Target || !Commands->Get_Health(Target) || (Commands->Get_Player_Type(obj) != -2 && Commands->Get_Player_Type(Target) == Commands->Get_Player_Type(obj)))
			{
				EnemyID = 0;
				Goto_Location(obj);
			}
		}
		Vector3 Pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(Pos,LastPos) < 25.0)
		{
			if (!remainingDelay && Get_A_Defense_Point(&dpPosition))
			{
				remainingDelay = newPointDelay;
				Goto_Location(obj);
			}
		}
		if (remainingDelay)
			remainingDelay--;
		LastPos = Pos;
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_AI_Guardian_Aircraft::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Guardian_Ignored"))
		return;
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Guardian_Aircraft_Ignored"))
		return;
	if (!JmgUtility::CanSeeStealth(stealthModeOverride,obj,seen))
		return;
	if (!Commands->Get_Health(seen))
		return;
	if (!EnemyID || !EnemyTimeOutTime)
	{
		EnemyID = Commands->Get_ID(seen);
		EnemyTimeOutTime = 2;
		Goto_Location(obj);
	}
	else if (EnemyID == Commands->Get_ID(seen))
		EnemyTimeOutTime = 2;
}
void JMG_Utility_AI_Guardian_Aircraft::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0 || (EnemyID && EnemyTimeOutTime) || (Commands->Get_Player_Type(obj) != -2 && Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj)))
		return;
	EnemyID = Commands->Get_ID(damager);
	EnemyTimeOutTime = 2;
	Goto_Location(obj);
}
void JMG_Utility_AI_Guardian_Aircraft::Goto_Location(GameObject *obj)
{
	ActionParamsStruct params;
	Commands->Action_Reset(obj,100);
	Vector3 GoToPos = dpPosition;
	GoToPos.Z += Get_Float_Parameter("FlightHeight");
	params.Set_Movement(GoToPos,1.0f,1.0f,false);
	params.MovePathfind = false;
	params.Set_Basic(this,100,10);
	GameObject *Target = Commands->Find_Object(EnemyID);
	if (Target)
	{
		if (Get_Int_Parameter("FaceTarget"))
			params.Set_Face_Location(Commands->Get_Position(Target),5.0f);
		else
			params.AttackFaceTarget = false;
		params.Set_Attack(Target,fireRange,0,true);
		Commands->Action_Attack(obj,params);
	}
	else
	{
		if (Get_Int_Parameter("FaceTarget"))
			params.Set_Face_Location(GoToPos,5.0f);
		else
			params.AttackFaceTarget = false;
		Commands->Action_Goto(obj,params);
	}
}
void JMG_Utility_Switch_Weapon_While_Primary_Empty::Created(GameObject *obj)
{
	if (Get_Float_Parameter("SecondaryToPrimaryTime") != -1.0)
		Commands->Set_Animation(obj,Get_Parameter("IdlePrimaryAnim"),true,0,0.0,-1,false);
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_Switch_Weapon_While_Primary_Empty::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		const char *weap = Get_Current_Weapon(obj);
		if (!Get_Bullets(obj,Get_Parameter("PrimaryWeapon")) && !_stricmp(weap,Get_Parameter("PrimaryWeapon")))
		{
			Commands->Select_Weapon(obj,Get_Parameter("SecondaryWeapon"));
			Commands->Start_Timer(obj,this,Get_Float_Parameter("ReloadTime"),2);
			if (Get_Float_Parameter("PrimaryToSecondaryTime") != -1.0)
				Commands->Start_Timer(obj,this,Get_Float_Parameter("PrimaryToSecondaryTime"),3);
			else
				Commands->Start_Timer(obj,this,0.0f,3);
			if (Get_Float_Parameter("SecondaryToPrimaryTime") != -1.0)
				Commands->Start_Timer(obj,this,Get_Float_Parameter("ReloadTime")-Get_Float_Parameter("SecondaryToPrimaryTime"),5);
			else
				Commands->Start_Timer(obj,this,Get_Float_Parameter("ReloadTime"),5);
			if (_stricmp(Get_Parameter("PrimaryToSecondaryAnim"),"null"))
				Commands->Set_Animation(obj,Get_Parameter("PrimaryToSecondaryAnim"),false,0,0.0,-1,false);
			if (_stricmp(Get_Parameter("PrimaryToSecondarySound"),"null"))
				Commands->Create_Sound(Get_Parameter("PrimaryToSecondarySound"),Commands->Get_Position(obj),obj);
		}
		Commands->Start_Timer(obj,this,0.25f,1);
	}
	if (number == 2)
	{
		Set_Bullets(obj,Get_Parameter("PrimaryWeapon"),Get_Max_Bullets(obj,Get_Parameter("PrimaryWeapon")));
		Commands->Select_Weapon(obj,Get_Parameter("PrimaryWeapon"));
	}
	if (number == 3 && _stricmp(Get_Parameter("IdleSecondaryAnim"),"null"))
	{
		Commands->Set_Animation(obj,Get_Parameter("IdleSecondaryAnim"),true,0,0.0,-1,false);
	}
	if (number == 4 && _stricmp(Get_Parameter("IdlePrimaryAnim"),"null"))
	{
		Commands->Set_Animation(obj,Get_Parameter("IdlePrimaryAnim"),true,0,0.0,-1,false);
	}
	if (number == 5)
	{
		if (_stricmp(Get_Parameter("SecondaryToPrimaryAnim"),"null"))
			Commands->Set_Animation(obj,Get_Parameter("SecondaryToPrimaryAnim"),false,0,0.0,-1,false);
			if (_stricmp(Get_Parameter("SecondaryToPrimarySound"),"null"))
				Commands->Create_Sound(Get_Parameter("SecondaryToPrimarySound"),Commands->Get_Position(obj),obj);
		if (Get_Float_Parameter("SecondaryToPrimaryTime") > -1.0f)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("SecondaryToPrimaryTime"),4);
		else
			Commands->Start_Timer(obj,this,0.0f,4);
	}
}
void JMG_Utility_Send_Custom_When_Near_Building::Created(GameObject *obj)
{
	nearBuilding = false;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("CheckRate"),1);
}
void JMG_Utility_Send_Custom_When_Near_Building::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && Commands->Get_Health(obj))
	{
		Vector3 myPos = Commands->Get_Position(obj);
		float ClosestDist = FLT_MAX;
		for (SLNode<BuildingGameObj> *z = GameObjManager::BuildingGameObjList.Head();z;z = z->Next())
		{
			if ((Get_Int_Parameter("BuildingPlayerType") == 2 || z->Data()->Get_Player_Type() == Get_Int_Parameter("BuildingPlayerType")) && (Get_Int_Parameter("CheckDeadBuildings") || !z->Data()->Is_Destroyed()))
			{
				float Dist = z->Data()->Find_Closest_Poly(myPos);
				if (Dist < ClosestDist)
					ClosestDist = Dist;
			}
		}
		if (ClosestDist < Get_Float_Parameter("CloseToBuildingDistance") && !nearBuilding)
		{
			nearBuilding = true;
			if (Get_Int_Parameter("SendCustomObjectID"))
				Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("SendCustomObjectID")),Get_Int_Parameter("NearToBuildingCustom"),0,0);
			else
				Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("NearToBuildingCustom"),0,0);
		}
		if (ClosestDist > Get_Float_Parameter("CloseToBuildingDistance") && nearBuilding)
		{
			nearBuilding = false;
			if (Get_Int_Parameter("SendCustomObjectID"))
				Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("SendCustomObjectID")),Get_Int_Parameter("FarFromBuildingCustom"),0,0);
			else
				Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("FarFromBuildingCustom"),0,0);
		}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("CheckRate"),1);
	}
}
void JMG_Utility_AI_Engineer_Repair_Target::Created(GameObject *obj)
{
}
void JMG_Utility_Reset_Screen_Fade_And_Fog_On_Destroy::Destroyed(GameObject *obj)
{
	JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Player(obj,0.0f);
	Set_Fog_Range_Player(obj,JMG_Utility_Swimming_Zone::fogMinDistance,JMG_Utility_Swimming_Zone::fogMaxDistance,0.0f);
}
void JMG_Utility_Reset_Screen_Fade_And_Fog_On_Destroy::Detach(GameObject *obj)
{
	if (Exe == 4)
		return;
	Destroyed(obj);
}
void JMG_Utility_AI_Goto_Player::Created(GameObject *obj)
{
	state = IDLE;
	moveBackward = false;
	lastPosition = homeLocation = Commands->Get_Position(obj);
	targetId = 0;
	lastSeenTime = 0;
	huntingStarId = 0;
	stuckTime = 0;
	reverseTime = 0;
	wanderingAiGroupId = Get_Int_Parameter("WanderingAIGroupID");
	wanderSpeed = Get_Float_Parameter("WanderSpeed");
	huntSpeed = Get_Float_Parameter("HuntSpeed");
	attackSpeed = Get_Float_Parameter("AttackSpeed");
	returnHomeSpeed = Get_Float_Parameter("ReturnHomeSpeed");
	changeWanderGroupCustom = Get_Int_Parameter("ChangeWanderGroupCustom");
	changeWanderSpeedCustom = Get_Int_Parameter("ChangeWanderSpeedCustom");
	changeHuntDistanceCustom = Get_Int_Parameter("ChangeHuntDistanceCustom");
	changeHuntSpeedCustom = Get_Int_Parameter("ChangeHuntSpeedCustom");
	changeReturnHomeSpeedCustom = Get_Int_Parameter("ChangeReturnHomeSpeedCustom");
	changeMaxSightFromHomeLocationCustom = Get_Int_Parameter("ChangeMaxSightFromHomeLocationCustom");
	changeAttackSpeedCustom = Get_Int_Parameter("ChangeAttackSpeedCustom");
	maxSightFromHomeLocation = Get_Float_Parameter("MaxSightRangeFromHome");
	maxSightFromHomeLocation *= maxSightFromHomeLocation;
	huntStealth = Get_Int_Parameter("HuntStealth") ? true : false;
	huntSearchDistance = Get_Float_Parameter("HuntSearchDistance") < 0.0f ? Get_Float_Parameter("HuntSearchDistance") : Get_Float_Parameter("HuntSearchDistance")*Get_Float_Parameter("HuntSearchDistance");
	for (int x = 0;x < 128;x++)
		ignoreStarsTime[x] = 0;
	const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Current_Weapon(obj),true);
	if (ammo)
	{
		weaponRange = ammo->Range;
		weaponEffectiveRange = ammo->Range < ammo->EffectiveRange ? ammo->Range : ammo->EffectiveRange;
	}
	attackArriveDistance = Get_Float_Parameter("AttackDistance") >= 0 ? Get_Float_Parameter("AttackDistance") : weaponEffectiveRange;
	huntArriveDistance = Get_Float_Parameter("HuntArriveDistance");
	wanderDistanceOverride = Get_Float_Parameter("WanderDistanceOverride");
	wanderDistanceOverride *= wanderDistanceOverride;
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,1.0f,1);
	Commands->Start_Timer(obj,this,1.0f,2);
	if (Get_Float_Parameter("RandomAttackDistance"))
		Commands->Start_Timer(obj,this,0.0f,3);
	if (Get_Float_Parameter("RandomHuntArriveDistance"))
		Commands->Start_Timer(obj,this,0.0f,4);
}
void JMG_Utility_AI_Goto_Player::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Goto_Player_Ignore_Object"))
		return;
	if (!_stricmp(Get_Skin(seen),"Blamo") || !_stricmp(Get_Shield_Type(seen),"Blamo"))
		return;
	if (!Commands->Get_Health(seen))
		return;
	if (Get_Int_Parameter("CanSeeStealth") != 2 && seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed())
	{
		if (!Get_Int_Parameter("CanSeeStealth"))
			return;
		float dist = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen));
		if (seen->As_SoldierGameObj() && dist > seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance()*seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance())
			return;
		else if (seen->As_VehicleGameObj() && dist > seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance()*seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance())
			return;	
		else if (dist > seen->As_SmartGameObj()->Get_Stealth_Fade_Distance()*seen->As_SmartGameObj()->Get_Stealth_Fade_Distance())
			return;
	}
	if (maxSightFromHomeLocation > 0.0f && JmgUtility::SimpleDistance(homeLocation,Commands->Get_Position(seen)) > maxSightFromHomeLocation)
		return;
	if (!targetId || !lastSeenTime || Choose_Target(obj,seen))
	{
		targetId = Commands->Get_ID(seen);
		lastSeenTime = Commands->Get_Random_Int(30,60);
		state = ATTACKING_TARGET;
		Attack_Move(obj,seen,Vector3(),attackSpeed,attackArriveDistance,true,false);
	}
	if (targetId == Commands->Get_ID(seen))
		lastSeenTime = Commands->Get_Random_Int(30,60);
}
void JMG_Utility_AI_Goto_Player::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 targetPos = Commands->Get_Position(obj);
		if (lastSeenTime)
		{
			lastSeenTime--;
			if (!lastSeenTime)
			{
				Return_Home(obj,ValidLastLocation(targetId));
				targetId = 0;
			}
		}
		if (targetId)
		{
			GameObject *target = Commands->Find_Object(targetId);
			targetPos = Commands->Get_Position(target);
			if (!target || !Commands->Get_Health(target) || (Commands->Get_Player_Type(obj) != -2 && Commands->Get_Player_Type(target) == Commands->Get_Player_Type(obj)) || Is_Script_Attached(target,"JMG_Utility_AI_Goto_Player_Ignore_Object"))
			{
				lastSeenTime = 0;
				Return_Home(obj,ValidLastLocation(targetId));
				targetId = 0;
			}
		}
		if (state == HUNTING_STAR)
		{
			GameObject *star = Get_GameObj(huntingStarId);
			if (!star || !Commands->Get_Health(star) || (huntSearchDistance >= 0.0f && JmgUtility::SimpleDistance(Commands->Get_Position(star),Commands->Get_Position(obj)) > huntSearchDistance) || Is_Script_Attached(star,"JMG_Utility_AI_Goto_Player_Ignore_Object"))
				Return_Home(obj,false);
		}
		if (state == IDLE || state == RETURNING_HOME || state == WANDERING_GROUP)
		{
			GameObject *star = findClosestStar(obj);
			if (star)
			{
				state = HUNTING_STAR;
				Attack_Move(obj,star,Vector3(),huntSpeed,huntArriveDistance,false,false);
			}
		}
		if (state == IDLE && wanderingAiGroupId != -1)
		{
			Vector3 wanderPos = Vector3();
			if (GetRandomPosition(&wanderPos))
			{
				state = WANDERING_GROUP;
				homeLocation = wanderPos;
				Attack_Move(obj,NULL,wanderPos,wanderSpeed,1.0f,false,false);
			}
		}
		if (state == RETURNING_HOME || state == WANDERING_GROUP)
			if (JmgUtility::SimpleDistance(lastAction.location,Commands->Get_Position(obj)) <= (wanderDistanceOverride != 0 ? wanderDistanceOverride : (obj->As_VehicleGameObj() ? 25.0f : 1.0f)))
				state = IDLE;
		if (state == ACTION_BADPATH)
			Cant_Get_To_target(obj);
		if (state != IDLE)
			Stuck_Check(obj,targetPos);
		Commands->Start_Timer(obj,this,0.1f,1);
	}
	if (number == 2)
	{
		for (int x = 1;x < 128;x++)
		{
			if (ignoreStarsTime[x])
				ignoreStarsTime[x]--;
		}
		Commands->Start_Timer(obj,this,1.0f,2);
	}
	if (number == 3)
	{
		attackArriveDistance = Get_Float_Parameter("AttackDistance") >= 0 ? Get_Float_Parameter("AttackDistance") : weaponEffectiveRange;
		attackArriveDistance += Commands->Get_Random(Get_Float_Parameter("RandomAttackDistance"),Get_Float_Parameter("RandomAttackDistance"));
	}
	if (number == 4)
	{
		huntArriveDistance = Get_Float_Parameter("HuntArriveDistance");
		huntArriveDistance += Commands->Get_Random(Get_Float_Parameter("RandomHuntArriveDistance"),Get_Float_Parameter("RandomHuntArriveDistance"));
	}
}
void JMG_Utility_AI_Goto_Player::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (changeWanderGroupCustom != 0 && changeWanderGroupCustom == message)
	{
		wanderingAiGroupId = param;
		if (state == WANDERING_GROUP)
		{
			Vector3 wanderPos = Vector3();
			if (GetRandomPosition(&wanderPos))
			{
				homeLocation = wanderPos;
				Attack_Move(obj,Commands->Find_Object(lastAction.targetId),homeLocation,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
			}
		}
	}
	if (changeWanderSpeedCustom != 0 && changeWanderSpeedCustom == message)
	{
		wanderSpeed = param/100.0f;
		if (state == WANDERING_GROUP)
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,wanderSpeed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
	}
	if (changeHuntDistanceCustom != 0 && changeHuntDistanceCustom == message)
	{
		huntSearchDistance = param/100.0f;
		if (huntSearchDistance > 0)
			huntSearchDistance *= huntSearchDistance;
	}
	if (changeHuntSpeedCustom != 0 && changeHuntSpeedCustom == message)
	{
		huntSpeed = param/100.0f;
		if (state == HUNTING_STAR)
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,huntSpeed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
	}
	if (changeReturnHomeSpeedCustom != 0 && changeReturnHomeSpeedCustom == message)
	{
		returnHomeSpeed = param/100.0f;
		if (state == RETURNING_HOME)
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,returnHomeSpeed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
	}
	if (changeMaxSightFromHomeLocationCustom != 0 && changeMaxSightFromHomeLocationCustom == message)
	{
		maxSightFromHomeLocation = param/100.0f;
		maxSightFromHomeLocation *= maxSightFromHomeLocation;
	}
	if (changeAttackSpeedCustom != 0 && changeAttackSpeedCustom == message)
	{
		attackSpeed = param/100.0f;
		if (state == ATTACKING_TARGET)
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,attackSpeed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
	}
}
void JMG_Utility_AI_Goto_Player::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		if (state == HUNTING_STAR)
		{
			ignoreStarsTime[huntingStarId] = 10;
			state = ACTION_BADPATH;
		}
		else if (state == WANDERING_GROUP)
			state = ACTION_BADPATH;
		else if (state == ATTACKING_TARGET)
			state = ACTION_BADPATH;
	}
}
void JMG_Utility_AI_Goto_Player::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0.0f || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (!_stricmp(Get_Skin(damager),"Blamo") || !_stricmp(Get_Shield_Type(damager),"Blamo"))
		return;
	if (!targetId || !lastSeenTime || Choose_Target(obj,damager))
	{
		targetId = Commands->Get_ID(damager);
		lastSeenTime = Commands->Get_Random_Int(30,60);
		state = ATTACKING_TARGET;
		Attack_Move(obj,damager,Vector3(),attackSpeed,attackArriveDistance,true,false);
	}
}
void JMG_Utility_AI_Goto_Player::Attack_Move(GameObject *obj,GameObject *target,Vector3 location,float speed,float distance,bool attack,bool overrideLocation)
{
	lastAction.location = location;
	Commands->Action_Reset(obj,100);
	lastAction = LastAction(targetId,location,speed,distance,attack,overrideLocation);
	ActionParamsStruct params;
	params.ObserverID = this->Get_ID();
	if (target && !overrideLocation)
	{
		params.Set_Movement(target,speed,distance,false);
		params.MoveFollow = true;
	}
	else
	{
		params.Set_Movement(location,speed,distance,false);
		params.MoveFollow = false;
	}
	params.MoveBackup = moveBackward;
	params.MovePathfind = true;
	if (Get_Int_Parameter("ShutdownEngineOnArrival"))
		params.ShutdownEngineOnArrival = true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,100,100,AI_STATE_COMBAT);
	params.AttackCheckBlocked = Get_Int_Parameter("AttackCheckBlocked") ? true : false;
	if (target && attack)
	{
		params.Set_Attack(target,weaponRange,0.0f,true);
		Commands->Action_Attack(obj,params);
	}
}
GameObject *JMG_Utility_AI_Goto_Player::findClosestStar(GameObject *obj)
{
	GameObject *nearest = NULL;
	float nearDist = -1.0f;
	Vector3 pos = Commands->Get_Position(obj);
	for (int x = 1;x < 128;x++)
	{
		GameObject *player = Get_GameObj(x);
		if (!player || ignoreStarsTime[x] || !Commands->Get_Health(player) || Commands->Get_Player_Type(player) == -4)
			continue;
		if (Is_Script_Attached(player,"JMG_Utility_AI_Goto_Player_Ignore_Object"))
			continue;
		if (!huntStealth)
		{
			GameObject *vehicle = Get_Vehicle(player);
			if (vehicle && vehicle->As_SmartGameObj() && vehicle->As_SmartGameObj()->Is_Stealthed())
				continue;
			else if (player->As_SmartGameObj() && player->As_SmartGameObj()->Is_Stealthed())
				continue;
		}
		if (maxSightFromHomeLocation > 0 && JmgUtility::SimpleDistance(homeLocation,Commands->Get_Position(player)) > maxSightFromHomeLocation)
			continue;
		float tempDist = JmgUtility::SimpleDistance(pos,Commands->Get_Position(player));
		if (huntSearchDistance >= 0.0f && tempDist > huntSearchDistance)
			continue;
		if (!nearest || (tempDist < nearDist))
		{
			nearest = player;
			nearDist = tempDist;
			huntingStarId = x;
		}
	}
	return nearest;
}
void JMG_Utility_AI_Goto_Player::Return_Home(GameObject *obj,ValidLastLocation goNearLastWanderPoint)
{
	Vector3 wanderPos;
	if (goNearLastWanderPoint.valid && Commands->Get_Random(0.0f,1.0f) < Get_Float_Parameter("ChanceToInvestigateLastSeenLocation"))
	{
		state = WANDERING_GROUP;
		Attack_Move(obj,NULL,goNearLastWanderPoint.location,wanderSpeed,1.0f,false,false);
	}
	else if (wanderingAiGroupId != -1 && GetRandomPosition(&wanderPos))
	{
		state = WANDERING_GROUP;
		Attack_Move(obj,NULL,wanderPos,wanderSpeed,1.0f,false,false);
	}
	else if (Get_Int_Parameter("ReturnHome"))
	{
		state = RETURNING_HOME;
		Attack_Move(obj,NULL,homeLocation,returnHomeSpeed,1.0f,false,false);
	}
	else
	{
		state = IDLE;
		Attack_Move(obj,NULL,Commands->Get_Position(obj),1.0f,1.0f,false,false);
	}
}
void JMG_Utility_AI_Goto_Player::Stuck_Check(GameObject *obj,Vector3 targetPos)
{
	if (obj->As_VehicleGameObj())
	{
		Vector3 pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(pos,lastPosition) < 1.0f)
		{
			if (targetId && JmgUtility::SimpleDistance(pos,targetPos) <= attackArriveDistance*attackArriveDistance)
			{
				stuckTime = 0;
				return;
			}
			if (!reverseTime)
			{
				stuckTime++;
				if (stuckTime > 30)
				{
					moveBackward = true;
					Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
					reverseTime = Commands->Get_Random_Int(10,60);
					stuckTime = 0;
				}
			}
		}
		else
		{
			stuckTime = 0;
			lastPosition = pos;
		}
		if (reverseTime)
		{
			reverseTime--;
			if (!reverseTime)
			{
				moveBackward = false;
				Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
			}
		}
	}
	else
	{
		Vector3 pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(pos,lastPosition) < 1.0f)
		{
			if (targetId && JmgUtility::SimpleDistance(pos,targetPos) <= attackArriveDistance*attackArriveDistance)
			{
				stuckTime = 0;
				return;
			}
			stuckTime++;
			if (stuckTime > 30)
				Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
			if (stuckTime > 60)
				Attack_Move(obj,NULL,homeLocation,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
		}
		else
		{
			stuckTime = 0;
			lastPosition = pos;
		}
	}
}
void JMG_Utility_AI_Goto_Player::Cant_Get_To_target(GameObject *obj)
{
	Vector3 wanderPos = Vector3();
	GameObject *star = findClosestStar(obj);
	if (star)
	{
		state = HUNTING_STAR;
		Attack_Move(obj,Commands->Find_Object(lastAction.targetId),Commands->Get_Position(star),huntSpeed,huntArriveDistance,true,true);
	}
	else if (wanderingAiGroupId != -1 && GetRandomPosition(&wanderPos))
	{
		state = WANDERING_GROUP;
		Attack_Move(obj,NULL,wanderPos,wanderSpeed,1.0f,true,true);
	}
	else
	{
		state = RETURNING_HOME;
		Attack_Move(obj,NULL,homeLocation,returnHomeSpeed,1.0f,true,true);
	}
}
bool JMG_Utility_AI_Goto_Player::Choose_Target(GameObject *obj,GameObject *target)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *currentTarget = Commands->Find_Object(targetId);
	if (!currentTarget)
		return true;
	if (JmgUtility::SimpleDistance(pos,Commands->Get_Position(target)) < JmgUtility::SimpleDistance(pos,Commands->Get_Position(currentTarget)))
		return true;
	return false;
}
JMG_Utility_AI_Goto_Player::ValidLastLocation::ValidLastLocation(int enemyId)
{
	GameObject *target = Commands->Find_Object(enemyId);
	if (!target || !Commands->Get_Health(target))
	{
		this->valid = false;
		return;
	}
	this->location = Commands->Get_Position(target);
	this->valid = true;
}
AggressiveAttackSpotSystem *aggressiveAttackSpotSystemControl = NULL;
void JMG_Utility_AI_Aggressive_Attack_Spot_Control::Created(GameObject *obj)
{
	aggressiveAttackSpotSystemControl = new AggressiveAttackSpotSystem();
}
void JMG_Utility_AI_Aggressive_Attack_Spot_Control::Destroyed(GameObject *obj)
{
	if (aggressiveAttackSpotSystemControl)
	{
		AggressiveAttackSpotSystem *temp = aggressiveAttackSpotSystemControl;
		aggressiveAttackSpotSystemControl = NULL;
		temp->Empty_List();
		delete(temp);
	}
}
void JMG_Utility_AI_Aggressive_Attack_Spot_Point::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_AI_Aggressive_Attack_Spot_Point::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!aggressiveAttackSpotSystemControl)
		{
			Console_Input("msg JMG_Utility_AI_Aggressive_Attack_Spot_Point ERROR: Make sure JMG_Utility_AI_Aggressive_Attack_Spot_Control is placed on the map!");
			return;
		}
		node = aggressiveAttackSpotSystemControl->addNode(obj,Get_Int_Parameter("GroupId"),Get_Vector3_Parameter("AttackOffset"));
	}
}
void JMG_Utility_AI_Aggressive_Attack_Spot_Point::Killed(GameObject *obj,GameObject *killer)
{
	Destroyed(obj);
}
void JMG_Utility_AI_Aggressive_Attack_Spot_Point::Destroyed(GameObject *obj)
{
	if (!The_Game()->Is_Game_Over() && node)
		node->alive = false;
}
void JMG_Utility_AI_Aggressive_Attack_Spot_Point2::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_AI_Aggressive_Attack_Spot_Point2::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!aggressiveAttackSpotSystemControl)
		{
			Console_Input("msg JMG_Utility_AI_Aggressive_Attack_Spot_Point2 ERROR: Make sure JMG_Utility_AI_Aggressive_Attack_Spot_Control is placed on the map!");
			return;
		}
		Vector3 mySpot = Commands->Get_Position(obj);
		Vector3 attackSpot = Get_Vector3_Parameter("AbsolutePosition");
		int objectId = Get_Int_Parameter("ObjectID");
		if (objectId)
		{
			GameObject *point = Commands->Find_Object(objectId);
			if (!point)
			{
				char errorDebug[220];
				sprintf(errorDebug,"msg JMG_Utility_AI_Aggressive_Attack_Spot_Point2 ERROR: %d Could not find the ObjectID %d!",Commands->Get_ID(obj),objectId);
				Console_Input(errorDebug);
			}
			attackSpot = Commands->Get_Position(point);
			Commands->Destroy_Object(point);
		}
		attackSpot.X -= mySpot.X;
		attackSpot.Y -= mySpot.Y;
		attackSpot.Z -= mySpot.Z;
		node = aggressiveAttackSpotSystemControl->addNode(obj,Get_Int_Parameter("GroupId"),attackSpot);
	}
}
void JMG_Utility_AI_Aggressive_Attack_Spot_Point2::Destroyed(GameObject *obj)
{
	if (!The_Game()->Is_Game_Over() && node)
		node->alive = false;
}
void JMG_Utility_AI_Aggressive_Attack_Spot::Created(GameObject *obj)
{
	state = IDLE;
	moveBackward = false;
	lastPosition = homeLocation = Commands->Get_Position(obj);
	targetId = 0;
	lastSeenTime = 0;
	stuckTime = 0;
	reverseTime = 0;
	usePrimaryFire = Get_Int_Parameter("UseSecondaryFire") ? false : true;
	const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Current_Weapon(obj),usePrimaryFire);
	if (ammo)
	{
		weaponRange = ammo->Range;
		weaponEffectiveRange = ammo->Range < ammo->EffectiveRange ? ammo->Range : ammo->EffectiveRange;
	}
	attackArriveDistance = Get_Float_Parameter("AttackDistance") >= 0 ? Get_Float_Parameter("AttackDistance") : weaponEffectiveRange;
	attackPointArriveDistance = Get_Float_Parameter("AttackPointDistance") >= 0 ? Get_Float_Parameter("AttackPointDistance") : weaponEffectiveRange;
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,1.0f,1);
	if (Get_Float_Parameter("RandomAttackPointDistance"))
		Commands->Start_Timer(obj,this,0.0f,2);
	if (Get_Float_Parameter("RandomAttackDistance"))
		Commands->Start_Timer(obj,this,0.0f,3);
	if (Get_Float_Parameter("ReactToAttackChance") == 0)
		reactToAttackChance = false;
	else if (Get_Float_Parameter("ReactToAttackChance") == 1.0f)
		reactToAttackChance = true;
	else
		Commands->Start_Timer(obj,this,0.0f,4);
}
void JMG_Utility_AI_Aggressive_Attack_Spot::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (!Commands->Get_Health(seen))
		return;
	if ((state == IDLE || state == RETURNING_HOME) && (!targetId || !lastSeenTime || Choose_Target(obj,seen)))
	{
		targetId = Commands->Get_ID(seen);
		lastSeenTime = Commands->Get_Random_Int(30,60);
		state = ATTACKING_ATTACKER;
		Attack_Move(obj,seen,Vector3(),Get_Float_Parameter("AttackSpeed"),attackArriveDistance,true,false,false);
	}
	if (targetId == Commands->Get_ID(seen))
		lastSeenTime = Commands->Get_Random_Int(30,60);
}
void JMG_Utility_AI_Aggressive_Attack_Spot::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 targetPos = Commands->Get_Position(obj);
		if (lastSeenTime)
		{
			lastSeenTime--;
			if (!lastSeenTime)
			{
				targetId = 0;
				IdleChoice(obj,true);
			}
		}
		if (targetId)
		{
			GameObject *target = Commands->Find_Object(targetId);
			targetPos = Commands->Get_Position(target);
			if (!target || !Commands->Get_Health(target) || (Commands->Get_Player_Type(obj) != -2 && Commands->Get_Player_Type(target) == Commands->Get_Player_Type(obj)))
			{
				targetId = 0;
				IdleChoice(obj,true);
			}
		}
		if (state == ATTACKING_POINT && !aggressiveAttackSpotSystemControl)
		{
			IdleChoice(obj,true);
		}
		if (aggressiveAttackSpotSystemControl && (state == IDLE || (state == ATTACKING_POINT && (!attackNode || !attackNode->alive))))
		{
			attackNode = aggressiveAttackSpotSystemControl->GetRandomNode(Get_Int_Parameter("GroupId"));
			if (!attackNode && Get_Int_Parameter("SecondaryGroupId"))
				attackNode = aggressiveAttackSpotSystemControl->GetRandomNode(Get_Int_Parameter("SecondaryGroupId"));
			if (attackNode && attackNode->alive)
			{
				state = ATTACKING_POINT;
				Attack_Move(obj,NULL,attackNode->position,Get_Float_Parameter("AttackPointSpeed"),attackPointArriveDistance,false,false,true);
			}
			else
				IdleChoice(obj,false);
		}
		if (state == RETURNING_HOME)
			if (JmgUtility::SimpleDistance(lastAction.location,Commands->Get_Position(obj)) <= (obj->As_VehicleGameObj() ? 25.0f : 1.0f))
				state = IDLE;
		if (state == ACTION_BADPATH)
			IdleChoice(obj,false);
		if (state != IDLE)
			Stuck_Check(obj,targetPos);
		Commands->Start_Timer(obj,this,0.1f,1);
	}
	if (number == 2)
	{
		attackPointArriveDistance = Get_Float_Parameter("AttackPointDistance") >= 0 ? Get_Float_Parameter("AttackPointDistance") : weaponEffectiveRange;
		attackPointArriveDistance += Commands->Get_Random(Get_Float_Parameter("RandomAttackPointDistance"),Get_Float_Parameter("RandomAttackPointDistance"));
	}
	if (number == 3)
	{
		attackArriveDistance = Get_Float_Parameter("AttackDistance") >= 0 ? Get_Float_Parameter("AttackDistance") : weaponEffectiveRange;
		attackArriveDistance += Commands->Get_Random(Get_Float_Parameter("RandomAttackDistance"),Get_Float_Parameter("RandomAttackDistance"));
	}
	if (number == 4)
	{
		if (Commands->Get_Random(0.0f,1.0f) <= Get_Float_Parameter("ReactToAttackChance"))
			reactToAttackChance = true;
		else
			reactToAttackChance = false;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("ReactToAttackRate"),4);
	}
}
void JMG_Utility_AI_Aggressive_Attack_Spot::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		if (state == ATTACKING_POINT)
		{
			if (Get_Int_Parameter("EnableDebug"))
			{
				char errorMsg[220];
				sprintf(errorMsg,"msg ERROR: Point %d is not reachable by the pathfind!",(attackNode ? attackNode->id : 0));
				Console_Input(errorMsg);
			}
			attackNode = NULL;
		}
		state = ACTION_BADPATH;
	}
}
void JMG_Utility_AI_Aggressive_Attack_Spot::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!reactToAttackChance && state == ATTACKING_POINT)
		return;
	if (damage <= 0.0f)
		return;
	if (!targetId || !lastSeenTime || Choose_Target(obj,damager))
	{
		targetId = Commands->Get_ID(damager);
		lastSeenTime = Commands->Get_Random_Int(30,60);
		state = ATTACKING_ATTACKER;
		Attack_Move(obj,damager,Vector3(),Get_Float_Parameter("AttackSpeed"),attackArriveDistance,true,false,false);
	}
}
void JMG_Utility_AI_Aggressive_Attack_Spot::Attack_Move(GameObject *obj,GameObject *target,Vector3 location,float speed,float distance,bool attack,bool overrideLocation,bool attackingPoint)
{
	lastAction.location = location;
	Commands->Action_Reset(obj,100);
	lastAction = LastAction(targetId,location,speed,distance,attack,overrideLocation,attackingPoint);
	ActionParamsStruct params;
	if (!attackingPoint && target && !overrideLocation)
	{
		params.Set_Movement(target,speed,distance,false);
		params.MoveFollow = true;
	}
	else
	{
		params.Set_Movement(location,speed,distance,false);
		params.MoveFollow = false;
	}
	params.MoveBackup = moveBackward;
	params.MovePathfind = true;
	if (Get_Int_Parameter("ShutdownEngineOnArrival"))
		params.ShutdownEngineOnArrival = true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,100,100,AI_STATE_COMBAT);
	if (attackingPoint)
	{
		location.X += attackNode->attackOffset.X;
		location.Y += attackNode->attackOffset.Y;
		location.Z += attackNode->attackOffset.Z;
		params.Set_Attack(location,weaponRange,0.0f,usePrimaryFire);
		params.AttackLocation = location;
		params.AttackCheckBlocked = false;
		Commands->Action_Attack(obj,params);
	}
	else if (target && attack)
	{
		params.Set_Attack(target,weaponRange,0.0f,usePrimaryFire);
		Commands->Action_Attack(obj,params);
	}
}
void JMG_Utility_AI_Aggressive_Attack_Spot::Stuck_Check(GameObject *obj,Vector3 targetPos)
{
	if (!obj->As_VehicleGameObj())
		return;
	Vector3 pos = Commands->Get_Position(obj);
	if (JmgUtility::SimpleDistance(pos,lastPosition) < 1.0f)
	{
		if (state == ATTACKING_ATTACKER && targetId && JmgUtility::SimpleDistance(pos,targetPos) <= attackArriveDistance*attackArriveDistance)
		{
			stuckTime = 0;
			return;
		}
		if (state == ATTACKING_POINT && attackNode && JmgUtility::SimpleDistance(pos,attackNode->position) <= attackPointArriveDistance*attackPointArriveDistance)
		{
			stuckTime = 0;
			return;
		}
		if (!reverseTime)
		{
			stuckTime++;
			if (stuckTime > 30)
			{
				moveBackward = true;
				Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation,lastAction.attackingPoint);
				reverseTime = Commands->Get_Random_Int(10,60);
				stuckTime = 0;
			}
		}
	}
	else
	{
		stuckTime = 0;
		lastPosition = pos;
	}
	if (reverseTime)
	{
		reverseTime--;
		if (!reverseTime)
		{
			moveBackward = false;
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation,lastAction.attackingPoint);
		}
	}
}
void JMG_Utility_AI_Aggressive_Attack_Spot::IdleChoice(GameObject *obj,bool allowAttackPoint)
{
	attackNode = aggressiveAttackSpotSystemControl ? ((!attackNode || !attackNode->alive) ? aggressiveAttackSpotSystemControl->GetRandomNode(Get_Int_Parameter("GroupId")) : attackNode) : NULL;
	if (!attackNode && Get_Int_Parameter("SecondaryGroupId"))
		attackNode = aggressiveAttackSpotSystemControl ? ((!attackNode || !attackNode->alive) ? aggressiveAttackSpotSystemControl->GetRandomNode(Get_Int_Parameter("SecondaryGroupId")) : attackNode) : NULL;
	if (allowAttackPoint && attackNode && attackNode->alive)
	{
		state = ATTACKING_POINT;
		Attack_Move(obj,NULL,attackNode->position,Get_Float_Parameter("AttackPointSpeed"),attackPointArriveDistance,false,false,true);
	}
	else if (Get_Int_Parameter("ReturnHome"))
	{
		state = RETURNING_HOME;
		Attack_Move(obj,Commands->Find_Object(lastAction.targetId),homeLocation,Get_Float_Parameter("ReturnHomeSpeed"),1.0f,true,true,false);
	}
	else
	{
		state = IDLE;
		Attack_Move(obj,NULL,Commands->Get_Position(obj),1.0f,1.0f,false,false,false);
	}
}
bool JMG_Utility_AI_Aggressive_Attack_Spot::Choose_Target(GameObject *obj,GameObject *target)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *currentTarget = Commands->Find_Object(targetId);
	if (!currentTarget)
		return true;
	if (JmgUtility::SimpleDistance(pos,Commands->Get_Position(target)) < JmgUtility::SimpleDistance(pos,Commands->Get_Position(currentTarget)))
		return true;
	return false;
}
void JMG_Utility_Destroy_Objects_In_ID_Range_On_Death::Destroyed(GameObject *obj)
{
	int end = Get_Int_Parameter("EndID");
	for (int x = Get_Int_Parameter("StartID");x <= end;x++)
	{
		GameObject *destroy = Commands->Find_Object(x);
		if (destroy)
			Commands->Destroy_Object(destroy);
	}
}
void JMG_Utility_Custom_Enable_Spawners_In_Range::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		int end = Get_Int_Parameter("EndID");
		bool enable = Get_Int_Parameter("Enable") == -1 ? (param ? true : false) : (Get_Int_Parameter("Enable") ? true : false);
		for (int x = Get_Int_Parameter("StartID");x <= end;x++)
			Commands->Enable_Spawner(x,enable);
	}
}
void JMG_Utility_Send_Custom_On_Player_Count::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	id = Get_Int_Parameter("ID");
	supressMatchSpam = Get_Int_Parameter("SupressMatchSpam") ? true : false;
	playerCountParam = Get_Int_Parameter("PlayerCount");
	triggerEq = Get_Vector3_Parameter("TriggerOn[eq|<|>]");
	destroyAfterTrigger = Get_Int_Parameter("DestroyAfterTrigger") ? true : false;
	playerCount = -1;
	conditionMatching = false;
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_Send_Custom_On_Player_Count::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		int tempCount = Get_Player_Count();
		if (playerCount != tempCount)
		{
			if (triggerEq.X && tempCount == playerCountParam)
				Send_The_Message(obj,tempCount);
			else if (triggerEq.Y && tempCount < playerCountParam)
				Send_The_Message(obj,tempCount);
			else if (triggerEq.Z && tempCount > playerCountParam)
				Send_The_Message(obj,tempCount);
			else
				conditionMatching = false;
			playerCount = tempCount;
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_Send_Custom_On_Player_Count::Send_The_Message(GameObject *obj,int tempCount)
{
	if (conditionMatching && supressMatchSpam)
		return;
	conditionMatching = true;
	GameObject *target = id ? Commands->Find_Object(id) : obj;
	if (target)
		Commands->Send_Custom_Event(obj,target,custom,param,delay);
	if (destroyAfterTrigger)
		this->Destroy_Script();
}
void JMG_Utility_Send_Custom_To_Preset_On_Message::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("TriggerCustom"))
	{
		int Param = Get_Int_Parameter("Param");
		Send_Custom_Event_To_Object(obj,Get_Parameter("Preset"),Get_Int_Parameter("Custom"),Param == -1 ? param : Param,Get_Float_Parameter("Delay"));
	}
}
void JMG_Utility_Basic_Spawner_Attach_Script::Created(GameObject *obj)
{
	scriptsGroupId = Get_Int_Parameter("Scripts_Group_ID");
	sprintf(script,"%s",Get_Parameter("Script"));
	const char *parameters = Get_Parameter("Params");
	params = newstr(parameters);
	char delim = Get_Parameter("Delim")[0];
	unsigned int length = strlen(parameters);
	for (unsigned int x = 0;x < length;x++)
		if (params[x] == delim)
			params[x] = ',';
}
void JMG_Utility_Basic_Spawner_Attach_Script::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 6873522 && (scriptsGroupId == -1 || param == -1 || scriptsGroupId == param))
	{
		Commands->Attach_Script(sender,script,params);
	}
}
void JMG_Utility_Send_Custom_On_Preset_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (_stricmp(Commands->Get_Preset_Name(enter),Get_Parameter("PresetName")))
		return;
	GameObject *target = NULL;
	switch (Get_Int_Parameter("ID"))
	{
	case -1:target = enter;break;
	case 0:target = obj;break;
	default:target = Commands->Find_Object(Get_Int_Parameter("ID"));break;
	}
	if (target)
		Commands->Send_Custom_Event(obj,target,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
}
void JMG_Utility_Basic_Spawner_Set_Prespawn_Location::Created(GameObject *obj)
{
	Vector3 newLoaction = Get_Vector3_Parameter("OverrideLocation");
	if (JmgUtility::SimpleDistance(newLoaction,Vector3()) <= 0.0f)
		newLoaction = Commands->Get_Position(obj);
	JMG_Utility_Basic_Spawner::preSpawnLocation = newLoaction;
}
void JMG_Utility_Basic_Spawner_Set_Prespawn_Location::Destroyed(GameObject *obj)
{
	JMG_Utility_Basic_Spawner::preSpawnLocation = Vector3();
}
void JMG_Utility_Teleport_On_Pickup::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		if (Get_Vehicle(sender))
			return;
		if (!JmgUtility::IsTruePhysicalObject(sender))
			return;
		GameObject *Object = Commands->Find_Object(Get_Int_Parameter("ObjectID"));
		Vector3 TargetPos = Get_Vector3_Parameter("Position");
		if (Object)
			TargetPos = Commands->Get_Position(Object);
		MoveablePhysClass *mphys = sender->As_PhysicalGameObj() ? sender->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
		if (mphys && mphys->Find_Teleport_Location(TargetPos,Get_Float_Parameter("MaxDistance"),&TargetPos))
		{
			Toggle_Fly_Mode(sender);
			Commands->Set_Position(sender,TargetPos);
			Toggle_Fly_Mode(sender);
			Force_Position_Update(sender);
			float facing = Get_Float_Parameter("Facing");
			if (Is_Script_Attached(sender,"JMG_Utility_Delay_Then_Rotate_Camera"))
				Remove_Script(sender,"JMG_Utility_Delay_Then_Rotate_Camera");
			if (facing != -999.0f)
			{
				char params[220];
				sprintf(params,"0.1,%.2f",facing);
				Commands->Attach_Script(sender,"JMG_Utility_Delay_Then_Rotate_Camera",params);
			}
		}
	}
}
void JMG_Utility_Zone_Set_Animation::Entered(GameObject *obj,GameObject *enter)
{
	GameObject *target = Commands->Find_Object(Get_Int_Parameter("ObjectID"));
	if (target)
		Commands->Set_Animation(target,Get_Parameter("Animation"),Get_Int_Parameter("Looping") ? true : false,0,Get_Float_Parameter("StartFrame"),Get_Float_Parameter("EndFrame"),Get_Int_Parameter("Blended") ? true : false);
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
void JMG_Utility_Scale_Infantry::Created(GameObject *obj)
{
	if (!obj->As_SoldierGameObj())
	{
		Destroy_Script();
		return;
	}
	obj->As_SoldierGameObj()->Set_Scale_Across_Network(Get_Float_Parameter("Scale"));
}
void JMG_Utility_Set_Innate_On_Create::Created(GameObject *obj)
{
	if (!obj->As_SoldierGameObj())
	{
		Destroy_Script();
		return;
	}
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_Set_Innate_On_Create::Timer_Expired(GameObject *obj,int number)
{
	if (Get_Int_Parameter("Enable"))
		Commands->Innate_Enable(obj);
	else
		Commands->Innate_Disable(obj);
}
void JMG_Utility_Set_Soldier_Damage_Animations::Created(GameObject *obj)
{
	if (!obj->As_SoldierGameObj())
	{
		Destroy_Script();
		return;
	}
	obj->As_SoldierGameObj()->Set_Can_Play_Damage_Animations(Get_Int_Parameter("Enable") ? true : false);
}
void JMG_Utility_Play_Music_On_Join_Enter_Change_Music::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	if (!JMG_Utility_Play_Music_On_Join_Controller::controllerPlaced)
	{
		Console_Input("msg JMG_Utility_Play_Music_On_Join_Enter_Change_Music ERROR: You need to place JMG_Utility_Play_Music_On_Join_Controller on an object in order to use this script!");
		return;
	}
	JMG_Utility_Play_Music_On_Join_Controller::Set_Music(Get_Parameter("Music"),Get_Int_Parameter("FadeOut"),Get_Int_Parameter("FadeIn"));
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
void JMG_Utility_Play_Music_On_Join_Killed_Change_Music::Killed(GameObject *obj,GameObject *killer)
{
	if (!JMG_Utility_Play_Music_On_Join_Controller::controllerPlaced)
	{
		Console_Input("msg JMG_Utility_Play_Music_On_Join_Killed_Change_Music ERROR: You need to place JMG_Utility_Play_Music_On_Join_Controller on an object in order to use this script!");
		return;
	}
	JMG_Utility_Play_Music_On_Join_Controller::Set_Music(Get_Parameter("Music"),Get_Int_Parameter("FadeOut"),Get_Int_Parameter("FadeIn"));
	Destroy_Script();
}
NewObjectiveSystem BasicObjectiveSystem = NewObjectiveSystem(2,true,"mObjective","null","null");
bool JMG_Utility_Objective_System_Controller::controllerPlaced = false;
void JMG_Utility_Objective_System_Controller::Created(GameObject *obj)
{
	controllerPlaced = true;
	for (int x = 0;x < 128;x++)
		playerNames[x][0] = '\0';
	BasicObjectiveSystem.controllerId = Commands->Get_ID(obj);
	BasicObjectiveSystem = NewObjectiveSystem(Get_Int_Parameter("Team"),Get_Int_Parameter("ShowMarkersOnRadar") ? true : false,Get_Parameter("PrimaryObjectiveModel"),Get_Parameter("SecondaryObjectiveModel"),Get_Parameter("TertiaryObjectiveModel"));
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_Objective_System_Controller::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (playerNames[x][0] != '\0')
			{
				if (!player)
					playerNames[x][0] = '\0';
				else if (_stricmp(playerNames[x],Get_Player_Name(player)))
				{
					BasicObjectiveSystem.Display_All_Objectives(player);
					sprintf(playerNames[x],"%s",Get_Player_Name(player));
				}
			}
			else if (player)
			{
				BasicObjectiveSystem.Display_All_Objectives(player);
				BasicObjectiveSystem.Dispaly_First_Pending_Primary_Objective_On_Hud(player);
				sprintf(playerNames[x],"%s",Get_Player_Name(player));
			}
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_Objective_System_Controller::Destroyed(GameObject *obj)
{
	if (!controllerPlaced)
		return;
	BasicObjectiveSystem.~NewObjectiveSystem();
	controllerPlaced = false;
}
void JMG_Utility_Objective_System_Controller::Detach(GameObject *obj)
{
	Destroyed(obj);
}
void JMG_Utility_Objective_System_Objective_Update_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (!Commands->Is_A_Star(enter) || triggered)
		return;
	if (!JMG_Utility_Objective_System_Controller::controllerPlaced)
	{
		Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Enter ERROR: Make sure you have a JMG_Utility_Objective_System_Controller placed on the map!");
		return;
	}
	triggered = true;
	if (Get_Int_Parameter("CompleteObjectiveID") && BasicObjectiveSystem.Get_Objective_Status(Get_Int_Parameter("CompleteObjectiveID")) != NewObjectiveSystem::Failed)
	{
		BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
		BasicObjectiveSystem.Set_Objective_Status(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Accomplished);
	}
	Commands->Send_Custom_Event(enter,obj,7531456,4562843,Get_Float_Parameter("Delay"));
}
void JMG_Utility_Objective_System_Objective_Update_Enter::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 7531456 && param == 4562843)
	{
		if (Get_Int_Parameter("NewObjectiveID"))
		{
			int objectiveMarkerId = Get_Int_Parameter("ObjectiveMarkerObjectID");
			GameObject *ObjectiveMarker = BasicObjectiveSystem.GetObjectiveMarker(objectiveMarkerId,sender,Get_Int_Parameter("NewObjectiveID"));
			if (!ObjectiveMarker && objectiveMarkerId)
			{
				Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Enter ERROR: Could not find objective marker object!");
				return;
			}
			bool added = false;
			if (ObjectiveMarker)
				added = BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"),ObjectiveMarker);
			else
				added = BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"));
			if (added)
				JmgUtility::SetHUDHelpText(Get_Int_Parameter("NewObjectiveStringID"),BasicObjectiveSystem.Get_Hud_Help_Text_Color(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority")));
		}
	}
}
void JMG_Utility_Objective_System_Objective_Update_Pickup::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		if (!JMG_Utility_Objective_System_Controller::controllerPlaced)
		{
			Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Pickup ERROR: Make sure you have a JMG_Utility_Objective_System_Controller placed on the map!");
			return;
		}
		if (!Commands->Is_A_Star(sender))
			return;
		if (Get_Int_Parameter("CompleteObjectiveID") && BasicObjectiveSystem.Get_Objective_Status(Get_Int_Parameter("CompleteObjectiveID")) != NewObjectiveSystem::Failed)
		{
			BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
			BasicObjectiveSystem.Set_Objective_Status(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Accomplished);
		}
		int objectiveMarkerId = Get_Int_Parameter("ObjectiveMarkerObjectID");
		GameObject *ObjectiveMarker = BasicObjectiveSystem.GetObjectiveMarker(objectiveMarkerId,sender,Get_Int_Parameter("NewObjectiveID"));
		if (!ObjectiveMarker && objectiveMarkerId)
		{
			Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Pickup ERROR: Could not find objective marker object!");
			return;
		}
		bool added = false;
		if (ObjectiveMarker)
			added = BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"),ObjectiveMarker);
		else
			added = BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"));
		if (added)
			JmgUtility::SetHUDHelpText(Get_Int_Parameter("NewObjectiveStringID"),BasicObjectiveSystem.Get_Hud_Help_Text_Color(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority")));
	}
}
void JMG_Utility_Objective_System_Objective_Update_Killed::Killed(GameObject *obj, GameObject *damager)
{
	if (The_Cnc_Game()->Get_Game_Duration_S() < 10)
		return;
	if (!JMG_Utility_Objective_System_Controller::controllerPlaced)
	{
		Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Killed ERROR: Make sure you have a JMG_Utility_Objective_System_Controller placed on the map!");
		return;
	}
	if (Get_Int_Parameter("CompleteObjectiveID") && BasicObjectiveSystem.Get_Objective_Status(Get_Int_Parameter("CompleteObjectiveID")) != NewObjectiveSystem::Failed)
	{
		BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
		BasicObjectiveSystem.Set_Objective_Status(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Accomplished);
	}
	int objectiveMarkerId = Get_Int_Parameter("ObjectiveMarkerObjectID");
	GameObject *ObjectiveMarker = objectiveMarkerId == -1 ? damager : Commands->Find_Object(objectiveMarkerId);
	if (!ObjectiveMarker && objectiveMarkerId)
	{
		Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Killed ERROR: Could not find objective marker object!");
		return;
	}
	bool added = false;
	if (ObjectiveMarker)
		added = BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"),ObjectiveMarker);
	else
		added = BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"));
	if (added)
		JmgUtility::SetHUDHelpText(Get_Int_Parameter("NewObjectiveStringID"),BasicObjectiveSystem.Get_Hud_Help_Text_Color(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority")));
}
void JMG_Utility_Objective_System_Objective_Update_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		if (!JMG_Utility_Objective_System_Controller::controllerPlaced)
		{
			Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Custom ERROR: Make sure you have a JMG_Utility_Objective_System_Controller placed on the map!");
			return;
		}
		if (Get_Int_Parameter("CompleteObjectiveID") && BasicObjectiveSystem.Get_Objective_Status(Get_Int_Parameter("CompleteObjectiveID")) != NewObjectiveSystem::Failed)
		{
			BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
			BasicObjectiveSystem.Set_Objective_Status(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Accomplished);
		}
		int objectiveMarkerId = Get_Int_Parameter("ObjectiveMarkerObjectID");
		GameObject *ObjectiveMarker = BasicObjectiveSystem.GetObjectiveMarker(objectiveMarkerId,sender,Get_Int_Parameter("NewObjectiveID"));
		if (!ObjectiveMarker && objectiveMarkerId)
		{
			Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Custom ERROR: Could not find objective marker object!");
			return;
		}
		bool added = false;
		if (ObjectiveMarker)
			added = BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"),ObjectiveMarker);
		else
			added = BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"));
		if (added)
			JmgUtility::SetHUDHelpText(Get_Int_Parameter("NewObjectiveStringID"),BasicObjectiveSystem.Get_Hud_Help_Text_Color(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority")));
	}
}
void JMG_Utility_Destroyed_Apply_Damage::Destroyed(GameObject *obj)
{
	GameObject *target = Commands->Find_Object(Get_Int_Parameter("ID"));
	if (target)
		Commands->Apply_Damage(target,Get_Float_Parameter("Damage"),Get_Parameter("Warhead"),0);
}
void JMG_Utility_Scale_Damage_By_Player_Count::Created(GameObject *obj)
{
	damageRefund = 0;
	resurrectCount = 0;
	maxPlayersToScaleFor = Get_Int_Parameter("MaxPlayersToScaleFor");
	maxScaleFactor = Get_Float_Parameter("MaxScaleFactor");
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_Scale_Damage_By_Player_Count::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		damageRefund = min((float)max(Get_Player_Count()-1,0)/(float)maxPlayersToScaleFor,maxScaleFactor);
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_Scale_Damage_By_Player_Count::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0.0f)
		return;
	damage *= damageRefund;
	float health = Commands->Get_Health(obj);
	if (!health)
	{
		resurrectCount++;
		if (Get_Player_Count()/4 < resurrectCount)
			return;
	}
	float maxHealth = Commands->Get_Max_Health(obj);
	health += damage;
	if (health > maxHealth)
	{
		damage = health-maxHealth;
		health = maxHealth;
		if (damage > 0)
		{
			float armor = Commands->Get_Shield_Strength(obj);
			float maxArmor = Commands->Get_Max_Shield_Strength(obj);
			armor += damage;
			if (armor > maxArmor)
				armor = maxArmor;
			Commands->Set_Shield_Strength(obj,armor);
		}
	}
	Commands->Set_Health(obj,health);
}
void JMG_Utility_Scale_Damage_Square_By_Player_Count::Created(GameObject *obj)
{
	resurrectCount = 0;
	damageRefund = 0;
	maxPlayersToScaleFor = Get_Int_Parameter("MaxPlayersToScaleFor")*Get_Int_Parameter("MaxPlayersToScaleFor");
	maxScaleFactor = Get_Float_Parameter("MaxScaleFactor");
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_Scale_Damage_Square_By_Player_Count::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		damageRefund = min((float)max(Get_Player_Count()*Get_Player_Count()-1,0)/(float)maxPlayersToScaleFor,maxScaleFactor);
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_Scale_Damage_Square_By_Player_Count::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0.0f)
		return;
	damage *= damageRefund;
	float health = Commands->Get_Health(obj);
	if (!health)
	{
		resurrectCount++;
		if (Get_Player_Count()/4 < resurrectCount)
			return;
	}
	float maxHealth = Commands->Get_Max_Health(obj);
	health += damage;
	if (health > maxHealth)
	{
		damage = health-maxHealth;
		health = maxHealth;
		if (damage > 0)
		{
			float armor = Commands->Get_Shield_Strength(obj);
			float maxArmor = Commands->Get_Max_Shield_Strength(obj);
			armor += damage;
			if (armor > maxArmor)
				armor = maxArmor;
			Commands->Set_Shield_Strength(obj,armor);
		}
	}
	Commands->Set_Health(obj,health);
}
void JMG_Utility_Regen_HitPoints::Created(GameObject *obj)
{
	regenHealth = Get_Int_Parameter("RegenHealth") ? true : false;
	regenArmor = Get_Int_Parameter("RegenArmor");
	healthAmount = Get_Float_Parameter("HealthAmount");
	armorAmount = Get_Float_Parameter("ArmorAmount");
	healthPerPlayer = Get_Float_Parameter("HealthPerPlayer");
	armorPerPlayer = Get_Float_Parameter("ArmorPerPlayer");
	damageDelay = Get_Float_Parameter("DamageDelay");
	rate = Get_Float_Parameter("Rate");
	scaleHealthPerHeal = Get_Float_Parameter("ScaleHealthPerHeal")*0.00001f;
	scaleArmorPerHeal = Get_Float_Parameter("ScaleArmorPerHeal")*0.00001f;
	enabled = true;
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Regen_HitPoints::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (enabled)
		{
			float health = Commands->Get_Health(obj);
			float maxHealth = Commands->Get_Max_Health(obj);
			float armor = Commands->Get_Shield_Strength(obj);
			float maxArmor = Commands->Get_Max_Shield_Strength(obj);
			if (regenArmor == -1 && armor < maxArmor && armorAmount+armorPerPlayer)
			{
				Commands->Set_Shield_Strength(obj,armor+armorAmount+armorPerPlayer*Get_Player_Count());
				if (Commands->Get_Shield_Strength(obj) > maxArmor)
					Commands->Set_Shield_Strength(obj,maxArmor);
			}
			else if (regenHealth && health < maxHealth && healthAmount+healthPerPlayer)
			{
				Commands->Set_Health(obj,health+healthAmount+healthPerPlayer*Get_Player_Count());
				if (Commands->Get_Health(obj) > maxHealth)
					Commands->Set_Health(obj,maxHealth);
			}
			else if (regenArmor && armor < maxArmor && armorAmount+armorPerPlayer)
			{
				Commands->Set_Shield_Strength(obj,armor+armorAmount+armorPerPlayer*Get_Player_Count());
				if (Commands->Get_Shield_Strength(obj) > maxArmor)
					Commands->Set_Shield_Strength(obj,maxArmor);
			}
			healthAmount = ScaleValue(healthAmount,scaleHealthPerHeal);
			armorAmount = ScaleValue(armorAmount,scaleArmorPerHeal);
			if ((!regenHealth || healthAmount == 0.0f) && (!regenArmor || armorAmount == 0.0f))
			{
				Destroy_Script();
				return;
			}
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
	if (number == 2)
	{
		enabled = true;
	}
}
void JMG_Utility_Regen_HitPoints::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0.0f || damageDelay <= 0.0f)
		return;
	enabled = false;
	Stop_Timer2(obj,this,2);
	Commands->Start_Timer(obj,this,damageDelay,2);
}
float JMG_Utility_Regen_HitPoints::ScaleValue(float value,float scale)
{
	if (scale == 0.0f || value == 0.0f)
		return value;
	value += scale;
	if (value < 0.0f)
		return 0.0f;
	return value;
}
void JMG_Utility_Toggle_Flight_On_Delay::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
}
void JMG_Utility_Toggle_Flight_On_Delay::Timer_Expired(GameObject *obj,int number)
{
	Toggle_Fly_Mode(obj);
}
void JMG_Utility_Fainting_Soldier::Created(GameObject *obj)
{
	if (!obj->As_SoldierGameObj())
	{
		Console_Input("msg JMG_Utility_Fainting_Soldier ERROR: JMG_Utility_Fainting_Soldier can only be attached to soldier game objects!");
		Destroy_Script();
		return;
	}
	changeArmorTypeWhenKnockedOut = Get_Int_Parameter("ChangeArmorTypeWhenKnockedOut") ? true : false;
	posLockId = 0;
	fainted = false;
	sprintf(faintAnimation,"%s",Get_Parameter("FaintAnimation"));
	sprintf(layAnimation,"%s",Get_Parameter("LayAnimation"));
	sprintf(standAnimation,"%s",Get_Parameter("StandAnimation"));
	sprintf(faintSound,"%s",Get_Parameter("FaintSound"));
	sprintf(standSound,"%s",Get_Parameter("StandSound"));
	sprintf(armorTypeWhileKnockedOut,"%s",Get_Parameter("ArmorTypeWhileKnockedOut"));
	teamWhileKnockedOut = Get_Int_Parameter("TeamWhileKnockedOut");
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_Fainting_Soldier::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (fainted && Commands->Get_Shield_Strength(obj) >= Commands->Get_Max_Shield_Strength(obj))
		{
			if (changeArmorTypeWhenKnockedOut)
				Commands->Set_Shield_Type(obj,armorType);
			fainted = false;
			Commands->Set_Is_Visible(obj,true);
			if (teamWhileKnockedOut != -99)
				Commands->Set_Player_Type(obj,originalTeam);
			if (Get_Fly_Mode(obj))
				Toggle_Fly_Mode(obj);
			Unlock_Soldier_Collision_Group(obj);
			GameObject *locker = Commands->Find_Object(posLockId);
			if (locker)
				Commands->Destroy_Object(locker);
			Commands->Create_Sound(standSound,Commands->Get_Position(obj),obj);
			Commands->Set_Animation(obj,standAnimation,false,0,0.0f,-1,false);
		}
		if (!fainted && !Commands->Get_Shield_Strength(obj) && !obj->As_SoldierGameObj()->Is_Airborne())
		{
			if (changeArmorTypeWhenKnockedOut)
			{
				sprintf(armorType,"%s",Get_Shield_Type(obj));
				Commands->Set_Shield_Type(obj,armorTypeWhileKnockedOut);
			}
			Lock_Soldier_Collision_Group(obj,SOLDIER_GHOST_COLLISION_GROUP);
			GameObject *locker = Commands->Create_Object("Daves Arrow",Commands->Get_Position(obj));
			Commands->Set_Model(locker,"null");
			Commands->Set_Facing(locker,Commands->Get_Facing(obj));
			Commands->Attach_To_Object_Bone(obj,locker,"origin");
			posLockId = Commands->Get_ID(locker);
			fainted = true;
			Commands->Set_Is_Visible(obj,false);
			if (teamWhileKnockedOut != -99)
			{
				originalTeam = Commands->Get_Player_Type(obj);
				Commands->Set_Player_Type(obj,teamWhileKnockedOut);
			}
			if (!Get_Fly_Mode(obj))
				Toggle_Fly_Mode(obj);
			Commands->Start_Timer(obj,this,0.01f,2);
		}
		Commands->Start_Timer(obj,this,0.25f,1);
	}
	if (number == 2)
	{
		Commands->Create_Sound(faintSound,Commands->Get_Position(obj),obj);
		Commands->Set_Animation(obj,faintAnimation,false,0,0.0f,-1,false);
	}
}
void JMG_Utility_Fainting_Soldier::Animation_Complete(GameObject *obj,const char *animation)
{
	if (!_stricmp(animation,faintAnimation))
	{
		Commands->Set_Animation(obj,layAnimation,true,0,0.0f,-1,true);
	}
}
void JMG_Utility_Fainting_Soldier::Destroyed(GameObject *obj)
{
	GameObject *locker = Commands->Find_Object(posLockId);
	if (locker)
		Commands->Destroy_Object(locker);
}
void JMG_Utility_AI_Guardian_Infantry::Created(GameObject *obj)
{
	dpPosition = Commands->Get_Position(obj);
	EnemyID = 0;
	EnemyTimeOutTime = 0;
	LastPos = Commands->Get_Position(obj);
	stealthModeOverride = Get_Int_Parameter("StealthModeOverride");
	float weaponRange = 0.0f;
	const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Current_Weapon(obj),true);
	if (ammo)
		weaponRange = ammo->Range;
	fireRange = Get_Float_Parameter("FireRange") >= 0 ? Get_Float_Parameter("FireRange") : weaponRange;
	Commands->Innate_Disable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Enable_Engine(obj,true);
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_AI_Guardian_Infantry::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (EnemyTimeOutTime)
		{
			EnemyTimeOutTime--;
			if (!EnemyTimeOutTime)
			{
				EnemyID = 0;
				Goto_Location(obj);
			}
		}
		if (EnemyID)
		{
			GameObject *Target = Commands->Find_Object(EnemyID);
			if (!Target || !Commands->Get_Health(Target) || (Commands->Get_Player_Type(obj) != -2 && Commands->Get_Player_Type(Target) == Commands->Get_Player_Type(obj)))
			{
				EnemyID = 0;
				Goto_Location(obj);
			}
		}
		Vector3 Pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(Pos,LastPos) < 1.0f)
		{
			if (Get_A_Defense_Point(&dpPosition))
				Goto_Location(obj);
		}
		LastPos = Pos;
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_AI_Guardian_Infantry::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (!Commands->Get_Health(seen))
		return;
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Guardian_Ignored"))
		return;
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Guardian_Infantry_Ignored"))
		return;
	if (!JmgUtility::CanSeeStealth(stealthModeOverride,obj,seen))
		return;
	if (!EnemyID || !EnemyTimeOutTime)
	{
		EnemyID = Commands->Get_ID(seen);
		EnemyTimeOutTime = Commands->Get_Random_Int(2,5);
		Goto_Location(obj);
	}
	else if (EnemyID == Commands->Get_ID(seen))
		EnemyTimeOutTime = Commands->Get_Random_Int(2,5);
}
void JMG_Utility_AI_Guardian_Infantry::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0 || (EnemyID && EnemyTimeOutTime) || (Commands->Get_Player_Type(obj) != -2 && Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj)))
		return;
	EnemyID = Commands->Get_ID(damager);
	EnemyTimeOutTime = Commands->Get_Random_Int(2,5);
	Goto_Location(obj);
}
void JMG_Utility_AI_Guardian_Infantry::Goto_Location(GameObject *obj)
{
	ActionParamsStruct params;
	Commands->Action_Reset(obj,100);
	Vector3 GoToPos = dpPosition;
	params.Set_Movement(GoToPos,Get_Float_Parameter("WanderSpeed"),1.0f,false);
	params.MovePathfind = true;
	params.Set_Basic(this,100,10);
	GameObject *Target = Commands->Find_Object(EnemyID);
	if (Target)
	{
		if (Get_Int_Parameter("FaceTarget"))
			params.Set_Face_Location(Commands->Get_Position(Target),5.0f);
		else
			params.AttackFaceTarget = false;
		params.Set_Attack(Target,fireRange,0,true);
		params.AttackCheckBlocked = Get_Int_Parameter("CheckBlocked") ? true : false;
		Commands->Action_Attack(obj,params);
	}
	else
	{
		if (Get_Int_Parameter("FaceTarget"))
			params.Set_Face_Location(GoToPos,5.0f);
		else
			params.AttackFaceTarget = false;
		Commands->Action_Goto(obj,params);
	}
}
void JMG_Utility_Set_Innate_Max_Wander_Distance::Created(GameObject *obj)
{
	Commands->Set_Innate_Soldier_Home_Location(obj,Commands->Get_Position(obj),Get_Float_Parameter("Distance"));
}
void JMG_Utility_Switch_Weapon_To_Empty_Hands_Until_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	sprintf(weaponName,"%s",Get_Current_Weapon(obj));
	Commands->Select_Weapon(obj,"");
}
void JMG_Utility_Switch_Weapon_To_Empty_Hands_Until_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Select_Weapon(obj,weaponName);
	}
}
void JMG_Utility_Set_Skin_And_Shield_Type_On_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Set_Skin_And_Shield_Type_On_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Set_Skin(obj,Get_Parameter("SkinType"));
		Commands->Set_Shield_Type(obj,Get_Parameter("ShieldType"));
	}
}
void JMG_Utility_Zone_Damage_All_In_ID_Range_On_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	int end = Get_Int_Parameter("EndID");
	float damage = Get_Float_Parameter("Damage");
	const char *warhead = Get_Parameter("Warhead");
	int damager = Get_Int_Parameter("Damager");
	GameObject *theDamager = NULL;
	if (damager == 1)
		theDamager = enter;
	if (damager == 2)
		theDamager = obj;
	for (int x = Get_Int_Parameter("StartID");x <= end;x++)
	{
		GameObject *object = Commands->Find_Object(x);
		if (!object)
			continue;
		Commands->Apply_Damage(object,damage,warhead,theDamager);
	}
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
void JMG_Utility_Zone_Send_Custom_To_All_With_Script::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	int custom = Get_Int_Parameter("Custom");
	int param = Get_Int_Parameter("Param");
	float delay = Get_Float_Parameter("Delay");
	const char *theScript = Get_Parameter("AttachedScript");
	for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
	{
		SmartGameObj* o = current->Data();
		if (o && Is_Script_Attached(o,theScript))
			Commands->Send_Custom_Event(enter,o,custom,param,delay);
	}
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
void JMG_Utility_HitPoints_In_Range_Change_Model::Created(GameObject *obj)
{
	modelSet = false;
	upperHP = Get_Float_Parameter("UpperHitPointsPercent")*0.01f;
	lowerHP = Get_Float_Parameter("LowerHitPointsPercent")*0.01f;
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_HitPoints_In_Range_Change_Model::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float hp = Get_Hitpoints(obj)/Get_Max_Hitpoints(obj);
		if (hp <= upperHP && hp > lowerHP)
		{
			if (!modelSet)
			{
				modelSet = true;
				Commands->Set_Model(obj,Get_Parameter("Model"));
			}
		}
		else if (modelSet)
			modelSet = false;
		Commands->Start_Timer(obj,this,0.25f,1);
	}
}
void JMG_Utility_HitPoints_In_Range_Attach_Preset::Created(GameObject *obj)
{
	presetId = 0;
	upperHP = Get_Float_Parameter("UpperHitPointsPercent")*0.01f;
	lowerHP = Get_Float_Parameter("LowerHitPointsPercent")*0.01f;
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_HitPoints_In_Range_Attach_Preset::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && Commands->Get_Health(obj))
	{
		float hp = Get_Hitpoints(obj)/Get_Max_Hitpoints(obj);
		if (hp <= upperHP && hp > lowerHP)
		{
			if (!presetId)
			{
				GameObject *turret = Commands->Create_Object(Get_Parameter("Preset"),Commands->Get_Position(obj));
				Commands->Attach_To_Object_Bone(turret,obj,Get_Parameter("BoneName"));
				presetId = Commands->Get_ID(turret);
			}
		}
		else if (presetId)
		{
			GameObject *turret = Commands->Find_Object(presetId);
			Commands->Destroy_Object(turret);
			presetId = 0;
		}
		Commands->Start_Timer(obj,this,0.25f,1);
	}
}
void JMG_Utility_HitPoints_In_Range_Attach_Preset::Killed(GameObject *obj,GameObject *killer)
{
	Destroyed(obj);
}
void JMG_Utility_HitPoints_In_Range_Attach_Preset::Destroyed(GameObject *obj)
{
	GameObject *turret = Commands->Find_Object(presetId);
	Commands->Destroy_Object(turret);
}
void JMG_Utility_HitPoints_In_Range_Enable_Spawner::Created(GameObject *obj)
{
	enabled = false;
	upperHP = Get_Float_Parameter("UpperHitPointsPercent")*0.01f;
	lowerHP = Get_Float_Parameter("LowerHitPointsPercent")*0.01f;
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_HitPoints_In_Range_Enable_Spawner::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && Commands->Get_Health(obj))
	{
		float hp = Get_Hitpoints(obj)/Get_Max_Hitpoints(obj);
		if (hp <= upperHP && hp > lowerHP)
		{
			if (!enabled)
			{
				enabled = true;
				Commands->Enable_Spawner(Get_Int_Parameter("SpawnerID"),true);
			}
		}
		else if (enabled)
		{
			enabled = false;
			Commands->Enable_Spawner(Get_Int_Parameter("SpawnerID"),false);
		}
		Commands->Start_Timer(obj,this,0.25f,1);
	}
}
void JMG_Utility_HitPoints_In_Range_Create_Preset::Created(GameObject *obj)
{
	presetId = 0;
	upperHP = Get_Float_Parameter("UpperHitPointsPercent")*0.01f;
	lowerHP = Get_Float_Parameter("LowerHitPointsPercent")*0.01f;
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_HitPoints_In_Range_Create_Preset::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && Commands->Get_Health(obj))
	{
		float hp = Get_Hitpoints(obj)/Get_Max_Hitpoints(obj);
		if (hp <= upperHP && hp > lowerHP)
		{
			if (!presetId)
			{
				GameObject *turret = Commands->Create_Object(Get_Parameter("Preset"),Get_Vector3_Parameter("Location"));
				Commands->Set_Facing(turret,Get_Float_Parameter("Facing"));
				presetId = Commands->Get_ID(turret);
			}
		}
		else if (presetId)
		{
			GameObject *turret = Commands->Find_Object(presetId);
			Commands->Destroy_Object(turret);
			presetId = 0;
		}
		Commands->Start_Timer(obj,this,0.25f,1);
	}
}
void JMG_Utility_HitPoints_In_Range_Create_Preset::Killed(GameObject *obj,GameObject *killer)
{
	Destroyed(obj);
}
void JMG_Utility_HitPoints_In_Range_Create_Preset::Destroyed(GameObject *obj)
{
	GameObject *turret = Commands->Find_Object(presetId);
	Commands->Destroy_Object(turret);
}
void JMG_Utility_HitPoints_In_Range_Send_Custom::Created(GameObject *obj)
{
	inRange = false;
	upperHP = Get_Float_Parameter("UpperHitPointsPercent")*0.01f;
	lowerHP = Get_Float_Parameter("LowerHitPointsPercent")*0.01f;
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_HitPoints_In_Range_Send_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && Commands->Get_Health(obj))
	{
		float hp = Get_Hitpoints(obj)/Get_Max_Hitpoints(obj);
		if (hp <= upperHP && hp > lowerHP)
		{
			if (!inRange)
			{
				int id = Get_Int_Parameter("ID");
				GameObject *object = id ? Commands->Find_Object(id) : obj;
				if (object)
					Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
				inRange = true;
			}
		}
		else if (inRange)
			inRange = false;
		Commands->Start_Timer(obj,this,0.25f,1);
	}
}
void JMG_Utility_Custom_Set_Weather_Precipitation::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Set_Weather_Precipitation::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		float transitionTime = Get_Float_Parameter("TransitionTime");
		Vector3 changeRSA = Get_Vector3_Parameter("Change[Rain|Snow|Ash|]");
		Vector3 setRSA = Get_Vector3_Parameter("Set[Rain|Snow|Ash|]");
		if (changeRSA.X)
			Commands->Set_Rain(setRSA.X,transitionTime,true);
		if (changeRSA.Y)
			Commands->Set_Snow(setRSA.Y,transitionTime,true);
		if (changeRSA.Z)
			Commands->Set_Ash(setRSA.Z,transitionTime,true);
	}
}
void JMG_Utility_Custom_Set_Weather_Fog::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Set_Weather_Fog::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Set_Fog_Range(Get_Float_Parameter("StartDistance"),Get_Float_Parameter("EndDistance"),Get_Float_Parameter("TransitionTime"));
	}
}
void JMG_Utility_Custom_Set_Weather_Wind::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Set_Weather_Wind::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Set_Wind(Get_Float_Parameter("Heading"),Get_Float_Parameter("Speed"),Get_Float_Parameter("Variability"),Get_Float_Parameter("TransitionTime"));
	}
}
void JMG_Utility_Custom_Set_Weather_Lightning::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Set_Weather_Lightning::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Set_Lightning(Get_Float_Parameter("Intensity"),Get_Float_Parameter("StartDistance"),Get_Float_Parameter("EndDistance"),Get_Float_Parameter("Heading"),Get_Float_Parameter("Distribution"),Get_Float_Parameter("TransitionTime"));
	}
}
void JMG_Utility_Custom_Set_Weather_War_Blitz::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Set_Weather_War_Blitz::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Set_War_Blitz(Get_Float_Parameter("Intensity"),Get_Float_Parameter("StartDistance"),Get_Float_Parameter("EndDistance"),Get_Float_Parameter("Heading"),Get_Float_Parameter("Distribution"),Get_Float_Parameter("TransitionTime"));
	}
}
void JMG_Utility_Custom_Set_Weather_Clouds::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Set_Weather_Clouds::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Set_Clouds(Get_Float_Parameter("Cover"),Get_Float_Parameter("Gloominess"),Get_Float_Parameter("TransitionTime"));
	}
}
void JMG_Utility_Play_Music_On_Join_Custom_Change_Music::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,1.0,1);
}
void JMG_Utility_Play_Music_On_Join_Custom_Change_Music::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		custom = Get_Int_Parameter("Custom");
		if (!JMG_Utility_Play_Music_On_Join_Controller::controllerPlaced)
		{
			Console_Input("msg JMG_Utility_Play_Music_On_Join_Custom_Change_Music ERROR: You need to place JMG_Utility_Play_Music_On_Join_Controller on an object in order to use this script!");
			Destroy_Script();
			return;
		}
	}
}
void JMG_Utility_Play_Music_On_Join_Custom_Change_Music::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		JMG_Utility_Play_Music_On_Join_Controller::Set_Music(Get_Parameter("Music"),Get_Int_Parameter("FadeOut"),Get_Int_Parameter("FadeIn"));
		if (!Get_Int_Parameter("Repeat"))
			Destroy_Script();
	}
}
void JMG_Utility_Zone_Send_Custom_If_Has_Weapon::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	if (!Has_Weapon(enter,Get_Parameter("WeaponName")))
		return;
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? (id == -1 ? enter : Commands->Find_Object(id)) : obj;
	if (object)
		Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	if (Get_Int_Parameter("RemoveWeapon"))
		Remove_Weapon(enter,Get_Parameter("WeaponName"));
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
void JMG_Utility_Receive_All_Customs_To_Send_Custom::Created(GameObject *obj)
{
	resetCustom = Get_Int_Parameter("ResetCustom");
	for (int x = 0;x < 10;x++)
	{
		char param[20];
		sprintf(param,"Custom%0d",x);
		customs[x] = Get_Int_Parameter(param);
		receivedCustoms[x] = customs[x] ? false : true;
	}
}
void JMG_Utility_Receive_All_Customs_To_Send_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == resetCustom)
	{
		for (int x = 0;x < 10;x++)
			receivedCustoms[x] = customs[x] ? false : true;
	}
	for (int x = 0;x < 10;x++)
		if (message == customs[x])
		{
			receivedCustoms[x] = true;
			for (int y = 0;y < 10;y++)
				if (!receivedCustoms[y])
					return;
			int id = Get_Int_Parameter("ID");
			GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
			if (object)
				Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
			if (Get_Int_Parameter("TriggerOnce"))
				Destroy_Script();
			break;
		}
}
void JMG_Utility_Zone_Teleport_To_Random_Wander_Point::Created(GameObject *obj)
{
	retryOnFailure = Get_Int_Parameter("RetryOnFailure") ? true : false;
	changeGroupIDCustom = Get_Int_Parameter("ChangeGroupIDCustom");
	playerType = Get_Int_Parameter("PlayerType");
	safeTeleportDistance = Get_Float_Parameter("SafeTeleportDistance");
	wanderPointGroup = Get_Int_Parameter("WanderingAIGroupID");
	aiOnly = Get_Int_Parameter("AiOnly") != 0;
}
void JMG_Utility_Zone_Teleport_To_Random_Wander_Point::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == changeGroupIDCustom)
	{
		wanderPointGroup = param;
	}
}
void JMG_Utility_Zone_Teleport_To_Random_Wander_Point::Entered(GameObject *obj,GameObject *enter)
{
	if (aiOnly && Commands->Is_A_Star(enter))
		return;
	if (CheckPlayerType(enter,playerType))
		return;
	Grab_Teleport_Spot(enter,5);
}
bool JMG_Utility_Zone_Teleport_To_Random_Wander_Point::Grab_Teleport_Spot(GameObject *enter,int attempts)
{
	if (The_Game()->Get_Game_Duration_S() < 1.0f)
	{
		char params[220];
		sprintf(params,"%d,%.2f",wanderPointGroup,safeTeleportDistance);
		Commands->Attach_Script(enter,"JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach",params);
		return false;
	}
	if (!attempts)
	{
		if (retryOnFailure)
		{
			char params[220];
			sprintf(params,"%d,%.2f",wanderPointGroup,safeTeleportDistance);
			Commands->Attach_Script(enter,"JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach",params);
		}
		return false;
	}
	attempts--;
	Vector3 targetPos;
	float facing;
	if (!Get_A_Defense_Point(&targetPos,&facing))
	{
		char errormsg[220];
		sprintf(errormsg,"msg JMG_Utility_Zone_Teleport_To_Random_Wander_Point ERROR: No wander points found for group %d!",wanderPointGroup);
		Console_Input(errormsg);
		return false;
	}
	MoveablePhysClass *mphys = enter->As_PhysicalGameObj() ? enter->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	if (mphys && mphys->Find_Teleport_Location(targetPos,safeTeleportDistance,&targetPos))
	{
		Toggle_Fly_Mode(enter);
		Commands->Set_Position(enter,targetPos);
		Force_Position_Update(enter);
		Toggle_Fly_Mode(enter);
		char params[220];
		sprintf(params,"0.1,%.2f",facing);
		Commands->Attach_Script(enter,"JMG_Utility_Delay_Then_Rotate_Camera",params);
		return true;
	}
	else
	{
		Commands->Set_Position(enter,targetPos);
		return Grab_Teleport_Spot(enter,attempts);
	}
}
void JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach::Created(GameObject *obj)
{
	safeTeleportDistance = Get_Float_Parameter("SafeTeleportDistance");
	wanderPointGroup = Get_Int_Parameter("WanderingAIGroupID");
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 targetPos;
		float facing;
		if (!Get_A_Defense_Point(&targetPos,&facing))
		{
			Commands->Start_Timer(obj,this,0.25f,1);
			return;
		}
		MoveablePhysClass *mphys = obj->As_PhysicalGameObj() ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
		if (mphys && mphys->Find_Teleport_Location(targetPos,safeTeleportDistance,&targetPos))
		{
			Toggle_Fly_Mode(obj);
			Commands->Set_Position(obj,targetPos);
			Force_Position_Update(obj);
			Toggle_Fly_Mode(obj);
			char params[220];
			sprintf(params,"0.1,%.2f",facing);
			Commands->Attach_Script(obj,"JMG_Utility_Delay_Then_Rotate_Camera",params);
		}
		else
		{
			Commands->Start_Timer(obj,this,0.25f,1);
			return;
		}
		Destroy_Script();
	}
}
void JMG_Utility_Zone_Set_Player_Type::Created(GameObject *obj)
{
	requiredPlayerType = Get_Int_Parameter("RequiredPlayerType");
	setPlayerType = Get_Int_Parameter("SetPlayerType");
}
void JMG_Utility_Zone_Set_Player_Type::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,requiredPlayerType) || Commands->Get_Player_Type(enter) == -4)
		return;
	Commands->Set_Player_Type(enter,setPlayerType);
}
void JMG_Utility_Zone_Send_Custom_Enter::Created(GameObject *obj)
{
	playerType = Get_Int_Parameter("PlayerType");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Custom");
	param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	triggerOnce = Get_Int_Parameter("TriggerOnce") ? true : false;
}
void JMG_Utility_Zone_Send_Custom_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,playerType) || Commands->Get_Player_Type(enter) == -4)
		return;
	GameObject *object = id ? (id == -1 ? enter : Commands->Find_Object(id)) : obj;
	if (object)
		Commands->Send_Custom_Event(obj,object,custom,param,delay);
	if (triggerOnce)
		Destroy_Script();
}
void JMG_Utility_Custom_Destroy_Closest_Object_To_Position::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Destroy_Closest_Object_To_Position::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		GameObject *nearest = NULL;
		float dist = 0.0f;
		Vector3 pos = Get_Vector3_Parameter("Position");
		const char *presetName = Get_Parameter("PresetName");
		float maxDist = Get_Float_Parameter("MaxDistance");
		maxDist *= maxDist;
		for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
		{
			GameObject* o = current->Data()->As_ScriptableGameObj();
			if (!_stricmp(presetName,Commands->Get_Preset_Name(o)))
			{
				float tempDistance = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o));
				if (!nearest || (tempDistance < dist && (maxDist == 0.0 || tempDistance <= maxDist)))
				{
					nearest = o;
					dist = tempDistance;
				}
			}
		}
		if (nearest)
			Commands->Destroy_Object(nearest);
		if (Get_Int_Parameter("TriggerOnce"))
			Destroy_Script();
	}
}
void JMG_Utility_Zone_Set_Spawner::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	Commands->Enable_Spawner(Get_Int_Parameter("SpawnerID"),Get_Int_Parameter("Enable") ? true : false);
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
void JMG_Utility_Persistant_Weapon_Powerup::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		const char *powerupNameOverride = Get_Parameter("PowerupNameOverride");
		if (!_stricmp(powerupNameOverride,"null"))
			powerupNameOverride = Commands->Get_Preset_Name(obj);
		char params[256];
		sprintf(params,"%s,%s,%d",powerupNameOverride,Get_Parameter("WeaponName"),Get_Int_Parameter("DisarmCustom"));
		Commands->Attach_Script(sender,"JMG_Utility_Persistant_Weapon_zAttached",params);
	}
}
void JMG_Utility_Persistant_Weapon_zStandin::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_Persistant_Weapon_zStandin::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 myPos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(Commands->Get_Position(Commands->Get_A_Star(myPos)),myPos) >= 9.0f)
		{
			myPos.Z += 0.25f;
			GameObject *statue = Commands->Create_Object(Get_Parameter("PowerupName"),myPos);
			Commands->Set_Facing(statue,Commands->Get_Facing(obj));
			Commands->Destroy_Object(obj);
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_Persistant_Weapon_zAttached::Created(GameObject *obj)
{
	disarmCustom = Get_Int_Parameter("DisarmCustom");
	sprintf(weaponName,"%s",Get_Parameter("WeaponName"));
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Persistant_Weapon_zAttached::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!Has_Weapon(obj,weaponName))
		{
			Destroyed(obj);
			return;
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Persistant_Weapon_zAttached::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == disarmCustom)
	{
		if (Has_Weapon(obj,weaponName))
			Remove_Weapon(obj,weaponName);
		Destroy_Script();
	}
}
void JMG_Utility_Persistant_Weapon_zAttached::Destroyed(GameObject *obj)
{
	GameObject *standin = Commands->Create_Object("Daves Arrow",Commands->Get_Position(obj));
	Commands->Set_Model(standin,"null");
	Commands->Set_Facing(standin,Commands->Get_Facing(obj));
	char params[256];
	sprintf(params,"%s",Get_Parameter("PowerupName"));
	Commands->Attach_Script(standin,"JMG_Utility_Persistant_Weapon_zStandin",params);
	Destroy_Script();
}
void JMG_Utility_Custom_Teleport_Players_Outside_Range_To_Wanderpoints::Created(GameObject *obj)
{
	scanTeleportSpot = Get_Vector3_Parameter("ScanTeleportSpot");
	teleportIgnoreDistance = Get_Float_Parameter("TeleportIgnoreDistance");
	teleportIgnoreDistance *= teleportIgnoreDistance;
	safeTeleportDistance = Get_Float_Parameter("SafeTeleportDistance");
	wanderPointGroup = Get_Int_Parameter("WanderingAIGroupID");
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Teleport_Players_Outside_Range_To_Wanderpoints::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player)
				continue;
			if (JmgUtility::SimpleDistance(scanTeleportSpot,Commands->Get_Position(player)) <= teleportIgnoreDistance)
				continue;
			Grab_Teleport_Spot(player,5);
		}
	}
}
bool JMG_Utility_Custom_Teleport_Players_Outside_Range_To_Wanderpoints::Grab_Teleport_Spot(GameObject *player,int attempts)
{
	if (!attempts)
	{
		char params[220];
		sprintf(params,"%d,%.2f",wanderPointGroup,safeTeleportDistance);
		Commands->Attach_Script(player,"JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach",params);
		return false;
	}
	attempts--;
	Vector3 targetPos;
	float facing;
	if (!Get_A_Defense_Point(&targetPos,&facing))
	{
		char errormsg[220];
		sprintf(errormsg,"msg JMG_Utility_Custom_Teleport_Players_Outside_Range_To_Wanderpoints ERROR: No wander points found for group %d!",wanderPointGroup);
		Console_Input(errormsg);
		return false;
	}
	MoveablePhysClass *mphys = player->As_PhysicalGameObj() ? player->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	if (mphys && mphys->Find_Teleport_Location(targetPos,safeTeleportDistance,&targetPos))
	{
		Toggle_Fly_Mode(player);
		Commands->Set_Position(player,targetPos);
		Force_Position_Update(player);
		Toggle_Fly_Mode(player);
		char params[220];
		sprintf(params,"0.1,%.2f",facing);
		Commands->Attach_Script(player,"JMG_Utility_Delay_Then_Rotate_Camera",params);
		return true;
	}
	else
	{
		Commands->Set_Position(player,targetPos);
		return Grab_Teleport_Spot(player,attempts);
	}
}
void JMG_Utility_Custom_Set_Tile_Frame::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Set_Tile_Frame::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Static_Anim_Phys_Goto_Frame(Get_Int_Parameter("ID"),Get_Float_Parameter("Frame"),Get_Parameter("Animation"));
	}
}
void JMG_Utility_Zone_Send_Custom_No_Weapon::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	if (Has_Weapon(enter,Get_Parameter("WeaponName")))
		return;
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? (id == -1 ? enter : Commands->Find_Object(id)) : obj;
	if (object)
		Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
int JMG_Utility_Custom_Display_Briefing_Message::voiceId = 0;
void JMG_Utility_Custom_Display_Briefing_Message::Created(GameObject *obj)
{
	triggered = false;
	BriefingText = NULL;
	CurrentNode = NULL;
	int File = Commands->Text_File_Open(Get_Parameter("FileName"));
	if (!File)
	{
		Console_Input("msg JMG_RP2_Mutant_Assault_Briefing_Custom_Message ERROR: Text File not found!");
		return;
	}
	bool NextLineIsTarget = false;
	char TextString[16384];
	while (Commands->Text_File_Get_String(File,TextString,16383))
	{
		if (!_stricmp(TextString,""))
			continue;
		if (!NextLineIsTarget)
			for (int x = 0;x < 16384;x++)
				if (TextString[x] == '\n' || TextString[x] == 13 || TextString[x] == '\0')
					TextString[x] = '\0';
		if (NextLineIsTarget)
		{
			int Count = 0;
			for (int x = 0;x < 16384;x++)
			{
				if ((Count > 150 && TextString[x] == ' ') || Count == 210)
				{// String getting too long, start a new one.
					CurrentNode->Text[Count] = '\0';
					AddNewTextNode();
					Count = 0;
				}
				else if (TextString[x] == '\n' || TextString[x] == '\0')
				{
					CurrentNode->Text[Count] = '\0';
					break;// end of line your done
				}
				else if (TextString[x] == '\f')
				{
					CurrentNode->Text[Count] = '\0';
					AddNewTextNode();
					Count = 0;
				}
				else
				{
					CurrentNode->Text[Count] = TextString[x];
					Count++;
				}
			}
			break;
		}
		else if (!_stricmp(TextString,Get_Parameter("TextTag")))
		{
			NextLineIsTarget = true;
			AddNewTextNode();
		}
		TextString[0] = '\0';
	}
	Commands->Text_File_Close(File);
	CurrentNode = BriefingText;
	voiceId = 0;
	Commands->Start_Timer(obj,this,0.1f,56345343);
}
void JMG_Utility_Custom_Display_Briefing_Message::Timer_Expired(GameObject *obj,int number)
{
	if (number == 56345344)
	{
		if (!CurrentNode)
		{
			RemoveTextNodes();
			Destroy_Script();
			return;
		}
		if (CurrentNode->Text[0] != '\0')
			for (int x = 1;x < 128;x++)
			{
				GameObject *Player = Get_GameObj(x);
				if (Player)
					JmgUtility::DisplayChatMessage(Player,(int)Get_Vector3_Parameter("MessageColor").X,(int)Get_Vector3_Parameter("MessageColor").Y,(int)Get_Vector3_Parameter("MessageColor").Z,CurrentNode->Text);
			}
		CurrentNode = CurrentNode->next;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("LineDelayTime"),56345344);
	}
}
void JMG_Utility_Custom_Display_Briefing_Message::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		if (triggered)
			return;
		triggered = true;
		Commands->Start_Timer(obj,this,0.0f,56345344);
		if (voiceId)
			Commands->Stop_Sound(voiceId,true);
		voiceId = Commands->Create_2D_Sound(Get_Parameter("UISound"));
	}
}
void JMG_Utility_Custom_Display_Briefing_Message::Destroyed(GameObject *obj)
{
	RemoveTextNodes();
}
void JMG_Utility_Custom_Display_Briefing_Message::AddNewTextNode()
{
	if (!BriefingText)
	{
		BriefingText = new BriefingTextNode;
		sprintf(BriefingText->Text,"\0");
		BriefingText->next = NULL;
		CurrentNode = BriefingText;
		return;
	}
	BriefingTextNode *Current = BriefingText;
	while (Current)
	{
		if (!Current->next)
		{
			Current->next = new BriefingTextNode;
			Current = Current->next;
			sprintf(Current->Text,"\0");
			Current->next = NULL;
			CurrentNode = Current;
			return;
		}
		Current = Current->next;
	}
}
void JMG_Utility_Custom_Display_Briefing_Message::RemoveTextNodes()
{
	BriefingTextNode *Current = BriefingText,*Temp;
	while (Current)
	{
		Temp = Current;
		Current = Current->next;
		delete Temp;
	}
	BriefingText = NULL;
	CurrentNode = NULL;
}
void JMG_Utility_Zone_Set_Player_Team::Created(GameObject *obj)
{
	requiredPlayerTeam = Get_Int_Parameter("RequiredPlayerTeam");
	setPlayerTeam = Get_Int_Parameter("SetPlayerTeam");
}
void JMG_Utility_Zone_Set_Player_Team::Entered(GameObject *obj,GameObject *enter)
{
	int playerId = JmgUtility::JMG_Get_Player_ID(enter);
	if (!playerId || (requiredPlayerTeam != 2 && requiredPlayerTeam != Get_Player_Team(playerId)))
		return;
	Change_Team_2(enter,setPlayerTeam);
}
void JMG_Utility_Objective_System_Objective_Update_Create::Created(GameObject *obj)
{
	if (!JMG_Utility_Objective_System_Controller::controllerPlaced)
	{
		Commands->Start_Timer(obj,this,0.1f,1);
		return;
	}
	Timer_Expired(obj,2);
}
void JMG_Utility_Objective_System_Objective_Update_Create::Timer_Expired(GameObject *obj,int number)
{
	if (number == 2)
	{
		if (!JMG_Utility_Objective_System_Controller::controllerPlaced)
		{
			Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Create ERROR: Make sure you have a JMG_Utility_Objective_System_Controller placed on the map!");
			return;
		}
		if (triggered)
			return;
		triggered = true;
		if (Get_Int_Parameter("CompleteObjectiveID") && BasicObjectiveSystem.Get_Objective_Status(Get_Int_Parameter("CompleteObjectiveID")) != NewObjectiveSystem::Failed)
		{
			BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
			BasicObjectiveSystem.Set_Objective_Status(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Accomplished);
		}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
	}
	if (number == 1)
	{
		if (Get_Int_Parameter("NewObjectiveID"))
		{
			int objectiveMarkerId = Get_Int_Parameter("ObjectiveMarkerObjectID");
			GameObject *ObjectiveMarker = objectiveMarkerId == -1 ? obj : Commands->Find_Object(objectiveMarkerId);
			if (!ObjectiveMarker && objectiveMarkerId)
			{
				Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Create ERROR: Could not find objective marker object!");
				return;
			}
			bool added = false;
			if (ObjectiveMarker)
				added = BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"),ObjectiveMarker);
			else
				added = BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"));
			if (added)
				JmgUtility::SetHUDHelpText(Get_Int_Parameter("NewObjectiveStringID"),BasicObjectiveSystem.Get_Hud_Help_Text_Color(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority")));
		}
	}
}
void JMG_Utility_Objective_System_Objective_Remove_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Objective_System_Objective_Remove_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		int objectiveId = Get_Int_Parameter("ObjectiveID");
		if (Get_Int_Parameter("OnlyRemovePending") && BasicObjectiveSystem.Get_Objective_Status(objectiveId) != NewObjectiveSystem::Pending)
			return;
		BasicObjectiveSystem.Remove_Objective(objectiveId);
	}
}
void JMG_Utility_Delay_Then_Rotate_Camera::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
}
void JMG_Utility_Delay_Then_Rotate_Camera::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 pos = Commands->Get_Bone_Position(obj,"c head");
		float facing = Get_Float_Parameter("Facing");
		pos.X += 100.0f*(cos(facing*PI180));
		pos.Y += 100.0f*(sin(facing*PI180));
		Force_Camera_Look_Player(obj,pos);
	}
	Destroy_Script();
}
void JMG_Utility_Damage_SmartGameObjects_In_Range::Created(GameObject *obj)
{
	range = Get_Float_Parameter("Range");
	range *= range;
	damage = Get_Float_Parameter("Damage");
	sprintf(warhead,"%s",Get_Parameter("Warhead"));
	rate = Get_Float_Parameter("Rate");
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Damage_SmartGameObjects_In_Range::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 mypos = Commands->Get_Position(obj);
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj* o = current->Data();
			if (o != obj && JmgUtility::SimpleDistance(Commands->Get_Position(o),mypos) <= range)
				Commands->Apply_Damage(o,damage,warhead,obj);
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Unstick_Infantry_If_Stuck::Created(GameObject *obj)
{
	distance = Get_Float_Parameter("Distance");
	rate = Get_Float_Parameter("Rate");
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Unstick_Infantry_If_Stuck::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 myPos = Commands->Get_Position(obj);
		MoveablePhysClass *mphys = obj->As_PhysicalGameObj() ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
		if (mphys && !mphys->Can_Teleport(Matrix3D(myPos)))
		{
			if (mphys->Find_Teleport_Location(myPos,distance,&myPos))
				Commands->Set_Position(obj,myPos);
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Custom_Send_Custom_On_Count::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	resetCustom = Get_Int_Parameter("ResetCustom");
	count = Get_Int_Parameter("Count");
}
void JMG_Utility_Custom_Send_Custom_On_Count::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == resetCustom)
	{
		count = Get_Int_Parameter("Count");
	}
	if (message == custom && count)
	{
		count--;
		if (count)
			return;
		int id = Get_Int_Parameter("ID");
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
		if (object)
		{
			int Param = Get_Int_Parameter("Param");
			Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("SendCustom"),Param == -1 ? param : Param,Get_Float_Parameter("Delay"));
		}
	}
}
void JMG_Utility_Custom_Destroy_Self::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Destroy_Self::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Destroy_Object(obj);
	}
}
void JMG_Utility_Zone_Set_Player_Team2::Created(GameObject *obj)
{
	requiredPlayerTeam = Get_Int_Parameter("RequiredPlayerTeam");
	setPlayerTeam = Get_Int_Parameter("SetPlayerTeam");
}
void JMG_Utility_Zone_Set_Player_Team2::Entered(GameObject *obj,GameObject *enter)
{
	int playerId = JmgUtility::JMG_Get_Player_ID(enter);
	if (!playerId || (requiredPlayerTeam != 2 && requiredPlayerTeam != Get_Player_Team(playerId)))
		return;
	Change_Team_2(enter,setPlayerTeam);
	cPlayer *thePlayer = Find_Player(playerId);
	thePlayer->Set_Deaths(thePlayer->Get_Deaths()-1);
	Commands->Destroy_Object(enter);
}
void JMG_Utility_Poke_Send_Custom::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void JMG_Utility_Poke_Send_Custom::Poked(GameObject *obj, GameObject *poker)
{
	if (CheckPlayerType(poker,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(poker) == -4)
		return;
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? (id == -1 ? poker : Commands->Find_Object(id)) : obj;
	if (object)
		Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	if (Get_Int_Parameter("TriggerOnce"))
	{
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		Destroy_Script();
	}
}
void JMG_Utility_Set_Collision_Group::Created(GameObject *obj)
{
	if (!obj->As_SoldierGameObj())
		obj->As_PhysicalGameObj()->Peek_Physical_Object()->Set_Collision_Group((Collision_Group_Type)Get_Int_Parameter("CollisionGroupID"));
	else
		Lock_Soldier_Collision_Group(obj,(Collision_Group_Type)Get_Int_Parameter("CollisionGroupID"));
}
void JMG_Utility_Cap_Credits::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	team = Get_Int_Parameter("Team");
	credits = Get_Float_Parameter("Credits");
	Commands->Start_Timer(obj,this,0.01f,1);
}
void JMG_Utility_Cap_Credits::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player || CheckPlayerType(player,team))
				continue;
			if (Get_Money(x) > credits)
				Set_Money(x,credits);
		}
		Commands->Start_Timer(obj,this,0.01f,1);
	}
}
void JMG_Utility_Cap_Credits::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		credits = (float)param;
	}
}
void JMG_Utility_Custom_Apply_Damage::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Apply_Damage::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		int id = Get_Int_Parameter("ID");
		GameObject *object = id ? Commands->Find_Object(id) : obj;
		int damagerId = Get_Int_Parameter("DamagerID");
		GameObject *damager = damagerId > 0 ? Commands->Find_Object(damagerId) : (damagerId == -1 ? sender : (damagerId == -2 ? obj : object));
		if (object)
			Commands->Apply_Damage(object,Get_Float_Parameter("Damage"),Get_Parameter("Warhead"),damager);
	}
}
void JMG_Utility_AI_Goto_Enemy::Created(GameObject *obj)
{
	state = IDLE;
	moveBackward = false;
	lastPosition = homeLocation = Commands->Get_Position(obj);
	targetId = 0;
	lastSeenTime = 0;
	huntingEnemyId = 0;
	stuckTime = 0;
	reverseTime = 0;
	huntStealth = Get_Int_Parameter("HuntStealth") ? true : false;
	removeIgnoreTime = 0;
	ignoreEnemyId = 0;
	wanderingAiGroupId = Get_Int_Parameter("WanderingAIGroupID");
	wanderSpeed = Get_Float_Parameter("WanderSpeed");
	huntSpeed = Get_Float_Parameter("HuntSpeed");
	attackSpeed = Get_Float_Parameter("AttackSpeed");
	returnHomeSpeed = Get_Float_Parameter("ReturnHomeSpeed");
	changeWanderGroupCustom = Get_Int_Parameter("ChangeWanderGroupCustom");
	changeWanderSpeedCustom = Get_Int_Parameter("ChangeWanderSpeedCustom");
	changeHuntDistanceCustom = Get_Int_Parameter("ChangeHuntDistanceCustom");
	changeHuntSpeedCustom = Get_Int_Parameter("ChangeHuntSpeedCustom");
	changeReturnHomeSpeedCustom = Get_Int_Parameter("ChangeReturnHomeSpeedCustom");
	changeMaxSightFromHomeLocationCustom = Get_Int_Parameter("ChangeMaxSightFromHomeLocationCustom");
	changeAttackSpeedCustom = Get_Int_Parameter("ChangeAttackSpeedCustom");
	maxSightFromHomeLocation = Get_Float_Parameter("MaxSightRangeFromHome");
	maxSightFromHomeLocation *= maxSightFromHomeLocation;
	huntSearchDistance = Get_Float_Parameter("HuntSearchDistance") < 0.0f ? Get_Float_Parameter("HuntSearchDistance") : Get_Float_Parameter("HuntSearchDistance")*Get_Float_Parameter("HuntSearchDistance");
	const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Current_Weapon(obj),true);
	if (ammo)
	{
		weaponRange = ammo->Range;
		weaponEffectiveRange = ammo->Range < ammo->EffectiveRange ? ammo->Range : ammo->EffectiveRange;
	}
	attackArriveDistance = Get_Float_Parameter("AttackDistance") >= 0 ? Get_Float_Parameter("AttackDistance") : weaponEffectiveRange;
	huntArriveDistance = Get_Float_Parameter("HuntArriveDistance");
	wanderDistanceOverride = Get_Float_Parameter("WanderDistanceOverride");
	wanderDistanceOverride *= wanderDistanceOverride;
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,1.0f,1);
	Commands->Start_Timer(obj,this,1.0f,2);
	if (Get_Float_Parameter("RandomAttackDistance"))
		Commands->Start_Timer(obj,this,0.0f,3);
	if (Get_Float_Parameter("RandomHuntArriveDistance"))
		Commands->Start_Timer(obj,this,0.0f,4);
}
void JMG_Utility_AI_Goto_Enemy::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (!Commands->Get_Health(seen))
		return;
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Goto_Enemy_Ignore_Object"))
		return;
	if (!_stricmp(Get_Skin(seen),"Blamo") || !_stricmp(Get_Shield_Type(seen),"Blamo"))
		return;
	if (Get_Int_Parameter("CanSeeStealth") != 2 && seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed())
	{
		if (!Get_Int_Parameter("CanSeeStealth"))
			return;
		float dist = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen));
		if (seen->As_SoldierGameObj() && dist > seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance()*seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance())
			return;
		else if (seen->As_VehicleGameObj() && dist > seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance()*seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance())
			return;	
		else if (dist > seen->As_SmartGameObj()->Get_Stealth_Fade_Distance()*seen->As_SmartGameObj()->Get_Stealth_Fade_Distance())
			return;
	}
	if (maxSightFromHomeLocation > 0.0f && JmgUtility::SimpleDistance(homeLocation,Commands->Get_Position(seen)) > maxSightFromHomeLocation)
		return;
	if (!targetId || !lastSeenTime || Choose_Target(obj,seen))
	{
		targetId = Commands->Get_ID(seen);
		lastSeenTime = Commands->Get_Random_Int(30,60);
		state = ATTACKING_TARGET;
		Attack_Move(obj,seen,Vector3(),attackSpeed,attackArriveDistance,true,false);
	}
	if (targetId == Commands->Get_ID(seen))
		lastSeenTime = Commands->Get_Random_Int(30,60);
}
void JMG_Utility_AI_Goto_Enemy::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (changeWanderGroupCustom != 0 && changeWanderGroupCustom == message)
	{
		wanderingAiGroupId = param;
		if (state == WANDERING_GROUP)
		{
			Vector3 wanderPos = Vector3();
			if (GetRandomPosition(&wanderPos))
			{
				homeLocation = wanderPos;
				Attack_Move(obj,Commands->Find_Object(lastAction.targetId),homeLocation,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
			}
		}
	}
	if (changeWanderSpeedCustom != 0 && changeWanderSpeedCustom == message)
	{
		wanderSpeed = param/100.0f;
		if (state == WANDERING_GROUP)
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,wanderSpeed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
	}
	if (changeHuntDistanceCustom != 0 && changeHuntDistanceCustom == message)
	{
		huntSearchDistance = param/100.0f;
		if (huntSearchDistance > 0)
			huntSearchDistance *= huntSearchDistance;
	}
	if (changeHuntSpeedCustom != 0 && changeHuntSpeedCustom == message)
	{
		huntSpeed = param/100.0f;
		if (state == HUNTING_STAR)
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,huntSpeed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
	}
	if (changeReturnHomeSpeedCustom != 0 && changeReturnHomeSpeedCustom == message)
	{
		returnHomeSpeed = param/100.0f;
		if (state == RETURNING_HOME)
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,returnHomeSpeed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
	}
	if (changeMaxSightFromHomeLocationCustom != 0 && changeMaxSightFromHomeLocationCustom == message)
	{
		maxSightFromHomeLocation = param/100.0f;
		maxSightFromHomeLocation *= maxSightFromHomeLocation;
	}
	if (changeAttackSpeedCustom != 0 && changeAttackSpeedCustom == message)
	{
		attackSpeed = param/100.0f;
		if (state == ATTACKING_TARGET)
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,attackSpeed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
	}
}
void JMG_Utility_AI_Goto_Enemy::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 targetPos = Commands->Get_Position(obj);
		if (lastSeenTime)
		{
			lastSeenTime--;
			if (!lastSeenTime)
			{
				Return_Home(obj,ValidLastLocation(targetId));
				targetId = 0;
			}
		}
		if (targetId)
		{
			GameObject *target = Commands->Find_Object(targetId);
			targetPos = Commands->Get_Position(target);
			if (!target || !Commands->Get_Health(target) || (Commands->Get_Player_Type(obj) != -2 && Commands->Get_Player_Type(target) == Commands->Get_Player_Type(obj)) || Is_Script_Attached(target,"JMG_Utility_AI_Goto_Enemy_Ignore_Object"))
			{
				lastSeenTime = 0;
				Return_Home(obj,ValidLastLocation(targetId));
				targetId = 0;
			}
		}
		if (state == HUNTING_STAR)
		{
			GameObject *star = Commands->Find_Object(huntingEnemyId);
			if (!star || !Commands->Get_Health(star) || (huntSearchDistance >= 0.0f && JmgUtility::SimpleDistance(Commands->Get_Position(star),Commands->Get_Position(obj)) > huntSearchDistance) || Is_Script_Attached(star,"JMG_Utility_AI_Goto_Enemy_Ignore_Object"))
				Return_Home(obj,false);
		}
		if (state == IDLE || state == RETURNING_HOME || state == WANDERING_GROUP)
		{
			GameObject *star = findClosestStar(obj);
			if (star)
			{
				state = HUNTING_STAR;
				Attack_Move(obj,star,Vector3(),huntSpeed,huntArriveDistance,false,false);
			}
		}
		if (state == IDLE && wanderingAiGroupId != -1)
		{
			Vector3 wanderPos = Vector3();
			if (GetRandomPosition(&wanderPos))
			{
				state = WANDERING_GROUP;
				homeLocation = wanderPos;
				Attack_Move(obj,NULL,wanderPos,wanderSpeed,1.0f,false,false);
			}
		}
		if (state == RETURNING_HOME || state == WANDERING_GROUP)
			if (JmgUtility::SimpleDistance(lastAction.location,Commands->Get_Position(obj)) <= (wanderDistanceOverride != 0 ? wanderDistanceOverride : (obj->As_VehicleGameObj() ? 25.0f : 1.0f)))
				state = IDLE;
		if (state == ACTION_BADPATH)
			Cant_Get_To_target(obj);
		if (state != IDLE)
			Stuck_Check(obj,targetPos);
		Commands->Start_Timer(obj,this,0.1f,1);
	}
	if (number == 2)
	{
		if (removeIgnoreTime)
		{
			removeIgnoreTime--;
			if (!removeIgnoreTime)
				ignoreEnemyId = 0;
		}
		Commands->Start_Timer(obj,this,1.0f,2);
	}
	if (number == 3)
	{
		attackArriveDistance = Get_Float_Parameter("AttackDistance") >= 0 ? Get_Float_Parameter("AttackDistance") : weaponEffectiveRange;
		attackArriveDistance += Commands->Get_Random(Get_Float_Parameter("RandomAttackDistance"),Get_Float_Parameter("RandomAttackDistance"));
	}
	if (number == 4)
	{
		huntArriveDistance = Get_Float_Parameter("HuntArriveDistance");
		huntArriveDistance += Commands->Get_Random(Get_Float_Parameter("RandomHuntArriveDistance"),Get_Float_Parameter("RandomHuntArriveDistance"));
	}
}
void JMG_Utility_AI_Goto_Enemy::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		if (state == HUNTING_STAR)
		{
			ignoreEnemyId = huntingEnemyId;
			removeIgnoreTime = 10;
			state = ACTION_BADPATH;
		}
		else if (state == WANDERING_GROUP)
			state = ACTION_BADPATH;
		else if (state == ATTACKING_TARGET)
			state = ACTION_BADPATH;
	}
}
void JMG_Utility_AI_Goto_Enemy::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0.0f || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (!_stricmp(Get_Skin(damager),"Blamo") || !_stricmp(Get_Shield_Type(damager),"Blamo"))
		return;
	if (!targetId || !lastSeenTime || Choose_Target(obj,damager))
	{
		targetId = Commands->Get_ID(damager);
		lastSeenTime = Commands->Get_Random_Int(30,60);
		state = ATTACKING_TARGET;
		Attack_Move(obj,damager,Vector3(),attackSpeed,attackArriveDistance,true,false);
	}
}
void JMG_Utility_AI_Goto_Enemy::Attack_Move(GameObject *obj,GameObject *target,Vector3 location,float speed,float distance,bool attack,bool overrideLocation)
{
	lastAction.location = location;
	Commands->Action_Reset(obj,100);
	lastAction = LastAction(targetId,location,speed,distance,attack,overrideLocation);
	ActionParamsStruct params;
	params.ObserverID = this->Get_ID();
	if (target && !overrideLocation)
	{
		params.Set_Movement(target,speed,distance,false);
		params.MoveFollow = true;
	}
	else
	{
		params.Set_Movement(location,speed,distance,false);
		params.MoveFollow = false;
	}
	params.MoveBackup = moveBackward;
	params.MovePathfind = true;
	if (Get_Int_Parameter("ShutdownEngineOnArrival"))
		params.ShutdownEngineOnArrival = true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,100,100,AI_STATE_COMBAT);
	params.AttackCheckBlocked = Get_Int_Parameter("AttackCheckBlocked") ? true : false;
	if (target && attack)
	{
		params.Set_Attack(target,weaponRange,0.0f,true);
		Commands->Action_Attack(obj,params);
	}
}
GameObject *JMG_Utility_AI_Goto_Enemy::findClosestStar(GameObject *obj)
{
	if (Commands->Get_Player_Type(obj) == -2)
		return NULL;
	GameObject *nearest = NULL;
	float nearDist = -1.0f;
	Vector3 pos = Commands->Get_Position(obj);
	int myPlayerType = Commands->Get_Player_Type(obj);
	for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
	{
		SmartGameObj* o = current->Data();
		int oPlayerType = Commands->Get_Player_Type(o);
		int enemyId = Commands->Get_ID(o);
		if (Is_Script_Attached(o,"JMG_Utility_AI_Goto_Enemy_Ignore_Object"))
			continue;
		if (!(o->As_SoldierGameObj() || o->As_VehicleGameObj()))
			continue;
		if (!Commands->Get_Health(o) || oPlayerType == -4 || oPlayerType == -2 || oPlayerType == myPlayerType || enemyId == ignoreEnemyId)
			continue;
		if (!_stricmp(Get_Skin(o),"Blamo") || !_stricmp(Get_Shield_Type(o),"Blamo"))
			continue;
		if (!huntStealth)
		{
			GameObject *vehicle = Get_Vehicle(o);
			if (vehicle && vehicle->As_SmartGameObj() && vehicle->As_SmartGameObj()->Is_Stealthed())
				continue;
			else if (o->As_SmartGameObj() && o->As_SmartGameObj()->Is_Stealthed())
				continue;
		}
		if (maxSightFromHomeLocation > 0 && JmgUtility::SimpleDistance(homeLocation,Commands->Get_Position(o)) > maxSightFromHomeLocation)
			continue;
		float tempDist = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o));
		if (huntSearchDistance >= 0.0f && tempDist > huntSearchDistance)
			continue;
		if (!nearest || (tempDist < nearDist))
		{
			nearest = o;
			nearDist = tempDist;
			huntingEnemyId = enemyId;
		}
	}
	return nearest;
}
void JMG_Utility_AI_Goto_Enemy::Return_Home(GameObject *obj,ValidLastLocation goNearLastWanderPoint)
{
	Vector3 wanderPos;
	if (goNearLastWanderPoint.valid && Commands->Get_Random(0.0f,1.0f) < Get_Float_Parameter("ChanceToInvestigateLastSeenLocation"))
	{
		state = WANDERING_GROUP;
		Attack_Move(obj,NULL,goNearLastWanderPoint.location,wanderSpeed,1.0f,false,false);
	}
	else if (wanderingAiGroupId != -1 && GetRandomPosition(&wanderPos))
	{
		state = WANDERING_GROUP;
		Attack_Move(obj,NULL,wanderPos,wanderSpeed,1.0f,false,false);
	}
	else if (Get_Int_Parameter("ReturnHome"))
	{
		state = RETURNING_HOME;
		Attack_Move(obj,NULL,homeLocation,returnHomeSpeed,1.0f,false,false);
	}
	else
	{
		state = IDLE;
		Attack_Move(obj,NULL,Commands->Get_Position(obj),1.0f,1.0f,false,false);
	}
}
void JMG_Utility_AI_Goto_Enemy::Stuck_Check(GameObject *obj,Vector3 targetPos)
{
	if (obj->As_VehicleGameObj())
	{
		Vector3 pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(pos,lastPosition) < 1.0f)
		{
			if (targetId && JmgUtility::SimpleDistance(pos,targetPos) <= attackArriveDistance*attackArriveDistance)
			{
				stuckTime = 0;
				return;
			}
			if (!reverseTime)
			{
				stuckTime++;
				if (stuckTime > 30)
				{
					moveBackward = true;
					Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
					reverseTime = Commands->Get_Random_Int(10,60);
					stuckTime = 0;
				}
			}
		}
		else
		{
			stuckTime = 0;
			lastPosition = pos;
		}
		if (reverseTime)
		{
			reverseTime--;
			if (!reverseTime)
			{
				moveBackward = false;
				Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
			}
		}
	}
	else
	{
		Vector3 pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(pos,lastPosition) < 1.0f)
		{
			if (targetId && JmgUtility::SimpleDistance(pos,targetPos) <= attackArriveDistance*attackArriveDistance)
			{
				stuckTime = 0;
				return;
			}
			stuckTime++;
			if (stuckTime > 30)
				Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
			if (stuckTime > 60)
				Attack_Move(obj,NULL,homeLocation,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
		}
		else
		{
			stuckTime = 0;
			lastPosition = pos;
		}
	}
}
void JMG_Utility_AI_Goto_Enemy::Cant_Get_To_target(GameObject *obj)
{
	Vector3 wanderPos = Vector3();
	GameObject *star = findClosestStar(obj);
	if (star && Commands->Get_ID(star) != ignoreEnemyId)
	{
		state = HUNTING_STAR;
		Attack_Move(obj,Commands->Find_Object(lastAction.targetId),Commands->Get_Position(star),huntSpeed,huntArriveDistance,true,true);
	}
	else if (wanderingAiGroupId != -1 && GetRandomPosition(&wanderPos))
	{
		state = WANDERING_GROUP;
		Attack_Move(obj,NULL,wanderPos,wanderSpeed,1.0f,true,true);
	}
	else
	{
		state = RETURNING_HOME;
		Attack_Move(obj,NULL,homeLocation,returnHomeSpeed,1.0f,true,true);
	}
}
bool JMG_Utility_AI_Goto_Enemy::Choose_Target(GameObject *obj,GameObject *target)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *currentTarget = Commands->Find_Object(targetId);
	if (!currentTarget)
		return true;
	if (JmgUtility::SimpleDistance(pos,Commands->Get_Position(target)) < JmgUtility::SimpleDistance(pos,Commands->Get_Position(currentTarget)))
		return true;
	return false;
}
JMG_Utility_AI_Goto_Enemy::ValidLastLocation::ValidLastLocation(int enemyId)
{
	GameObject *target = Commands->Find_Object(enemyId);
	if (!target || !Commands->Get_Health(target))
	{
		this->valid = false;
		return;
	}
	this->location = Commands->Get_Position(target);
	this->valid = true;
}
void JMG_Utility_AI_Goto_Enemy_Not_Star::Created(GameObject *obj)
{
	state = IDLE;
	moveBackward = false;
	lastPosition = homeLocation = Commands->Get_Position(obj);
	targetId = 0;
	lastSeenTime = 0;
	huntingEnemyId = 0;
	stuckTime = 0;
	reverseTime = 0;
	huntStealth = Get_Int_Parameter("HuntStealth") ? true : false;
	removeIgnoreTime = 0;
	ignoreEnemyId = 0;
	wanderingAiGroupId = Get_Int_Parameter("WanderingAIGroupID");
	wanderSpeed = Get_Float_Parameter("WanderSpeed");
	huntSpeed = Get_Float_Parameter("HuntSpeed");
	attackSpeed = Get_Float_Parameter("AttackSpeed");
	returnHomeSpeed = Get_Float_Parameter("ReturnHomeSpeed");
	changeWanderGroupCustom = Get_Int_Parameter("ChangeWanderGroupCustom");
	changeWanderSpeedCustom = Get_Int_Parameter("ChangeWanderSpeedCustom");
	changeHuntDistanceCustom = Get_Int_Parameter("ChangeHuntDistanceCustom");
	changeHuntSpeedCustom = Get_Int_Parameter("ChangeHuntSpeedCustom");
	changeReturnHomeSpeedCustom = Get_Int_Parameter("ChangeReturnHomeSpeedCustom");
	changeMaxSightFromHomeLocationCustom = Get_Int_Parameter("ChangeMaxSightFromHomeLocationCustom");
	changeAttackSpeedCustom = Get_Int_Parameter("ChangeAttackSpeedCustom");
	maxSightFromHomeLocation = Get_Float_Parameter("MaxSightRangeFromHome");
	maxSightFromHomeLocation *= maxSightFromHomeLocation;
	huntSearchDistance = Get_Float_Parameter("HuntSearchDistance") < 0.0f ? Get_Float_Parameter("HuntSearchDistance") : Get_Float_Parameter("HuntSearchDistance")*Get_Float_Parameter("HuntSearchDistance");
	const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Current_Weapon(obj),true);
	if (ammo)
	{
		weaponRange = ammo->Range;
		weaponEffectiveRange = ammo->Range < ammo->EffectiveRange ? ammo->Range : ammo->EffectiveRange;
	}
	attackArriveDistance = Get_Float_Parameter("AttackDistance") >= 0 ? Get_Float_Parameter("AttackDistance") : weaponEffectiveRange;
	huntArriveDistance = Get_Float_Parameter("HuntArriveDistance");
	wanderDistanceOverride = Get_Float_Parameter("WanderDistanceOverride");
	wanderDistanceOverride *= wanderDistanceOverride;
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,1.0f,1);
	Commands->Start_Timer(obj,this,1.0f,2);
	if (Get_Float_Parameter("RandomAttackDistance"))
		Commands->Start_Timer(obj,this,0.0f,3);
	if (Get_Float_Parameter("RandomHuntArriveDistance"))
		Commands->Start_Timer(obj,this,0.0f,4);
}
void JMG_Utility_AI_Goto_Enemy_Not_Star::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (!Commands->Get_Health(seen))
		return;
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Goto_Enemy_Not_Star_Ignore_Object"))
		return;
	if (!_stricmp(Get_Skin(seen),"Blamo") || !_stricmp(Get_Shield_Type(seen),"Blamo"))
		return;
	if (Get_Int_Parameter("CanSeeStealth") != 2 && seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed())
	{
		if (!Get_Int_Parameter("CanSeeStealth"))
			return;
		float dist = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen));
		if (seen->As_SoldierGameObj() && dist > seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance()*seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance())
			return;
		else if (seen->As_VehicleGameObj() && dist > seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance()*seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance())
			return;	
		else if (dist > seen->As_SmartGameObj()->Get_Stealth_Fade_Distance()*seen->As_SmartGameObj()->Get_Stealth_Fade_Distance())
			return;
	}
	if (maxSightFromHomeLocation > 0.0f && JmgUtility::SimpleDistance(homeLocation,Commands->Get_Position(seen)) > maxSightFromHomeLocation)
		return;
	if (!targetId || !lastSeenTime || Choose_Target(obj,seen))
	{
		targetId = Commands->Get_ID(seen);
		lastSeenTime = Commands->Get_Random_Int(30,60);
		state = ATTACKING_TARGET;
		Attack_Move(obj,seen,Vector3(),attackSpeed,attackArriveDistance,true,false);
	}
	if (targetId == Commands->Get_ID(seen))
		lastSeenTime = Commands->Get_Random_Int(30,60);
}
void JMG_Utility_AI_Goto_Enemy_Not_Star::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 targetPos = Commands->Get_Position(obj);
		if (lastSeenTime)
		{
			lastSeenTime--;
			if (!lastSeenTime)
			{
				Return_Home(obj,ValidLastLocation(targetId));
				targetId = 0;
			}
		}
		if (targetId)
		{
			GameObject *target = Commands->Find_Object(targetId);
			targetPos = Commands->Get_Position(target);
			if (!target || !Commands->Get_Health(target) || (Commands->Get_Player_Type(obj) != -2 && Commands->Get_Player_Type(target) == Commands->Get_Player_Type(obj)) || Is_Script_Attached(target,"JMG_Utility_AI_Goto_Enemy_Not_Star_Ignore_Object"))
			{
				lastSeenTime = 0;
				Return_Home(obj,ValidLastLocation(targetId));
				targetId = 0;
			}
		}
		if (state == HUNTING_STAR)
		{
			GameObject *star = Commands->Find_Object(huntingEnemyId);
			if (!star || !Commands->Get_Health(star) || (huntSearchDistance >= 0.0f && JmgUtility::SimpleDistance(Commands->Get_Position(star),Commands->Get_Position(obj)) > huntSearchDistance) || Is_Script_Attached(star,"JMG_Utility_AI_Goto_Enemy_Not_Star_Ignore_Object"))
				Return_Home(obj,false);
		}
		if (state == IDLE || state == RETURNING_HOME || state == WANDERING_GROUP)
		{
			GameObject *star = findClosestStar(obj);
			if (star)
			{
				state = HUNTING_STAR;
				Attack_Move(obj,star,Vector3(),huntSpeed,huntArriveDistance,false,false);
			}
		}
		if (state == IDLE && wanderingAiGroupId != -1)
		{
			Vector3 wanderPos = Vector3();
			if (GetRandomPosition(&wanderPos))
			{
				state = WANDERING_GROUP;
				homeLocation = wanderPos;
				Attack_Move(obj,NULL,wanderPos,wanderSpeed,1.0f,false,false);
			}
		}
		if (state == RETURNING_HOME || state == WANDERING_GROUP)
			if (JmgUtility::SimpleDistance(lastAction.location,Commands->Get_Position(obj)) <= (wanderDistanceOverride != 0 ? wanderDistanceOverride : (obj->As_VehicleGameObj() ? 25.0f : 1.0f)))
				state = IDLE;
		if (state == ACTION_BADPATH)
			Cant_Get_To_target(obj);
		if (state != IDLE)
			Stuck_Check(obj,targetPos);
		Commands->Start_Timer(obj,this,0.1f,1);
	}
	if (number == 2)
	{
		if (removeIgnoreTime)
		{
			removeIgnoreTime--;
			if (!removeIgnoreTime)
				ignoreEnemyId = 0;
		}
		Commands->Start_Timer(obj,this,1.0f,2);
	}
	if (number == 3)
	{
		attackArriveDistance = Get_Float_Parameter("AttackDistance") >= 0 ? Get_Float_Parameter("AttackDistance") : weaponEffectiveRange;
		attackArriveDistance += Commands->Get_Random(Get_Float_Parameter("RandomAttackDistance"),Get_Float_Parameter("RandomAttackDistance"));
	}
	if (number == 4)
	{
		huntArriveDistance = Get_Float_Parameter("HuntArriveDistance");
		huntArriveDistance += Commands->Get_Random(Get_Float_Parameter("RandomHuntArriveDistance"),Get_Float_Parameter("RandomHuntArriveDistance"));
	}
}
void JMG_Utility_AI_Goto_Enemy_Not_Star::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (changeWanderGroupCustom != 0 && changeWanderGroupCustom == message)
	{
		wanderingAiGroupId = param;
		if (state == WANDERING_GROUP)
		{
			Vector3 wanderPos = Vector3();
			if (GetRandomPosition(&wanderPos))
			{
				homeLocation = wanderPos;
				Attack_Move(obj,Commands->Find_Object(lastAction.targetId),homeLocation,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
			}
		}
	}
	if (changeWanderSpeedCustom != 0 && changeWanderSpeedCustom == message)
	{
		wanderSpeed = param/100.0f;
		if (state == WANDERING_GROUP)
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,wanderSpeed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
	}
	if (changeHuntDistanceCustom != 0 && changeHuntDistanceCustom == message)
	{
		huntSearchDistance = param/100.0f;
		if (huntSearchDistance > 0)
			huntSearchDistance *= huntSearchDistance;
	}
	if (changeHuntSpeedCustom != 0 && changeHuntSpeedCustom == message)
	{
		huntSpeed = param/100.0f;
		if (state == HUNTING_STAR)
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,huntSpeed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
	}
	if (changeReturnHomeSpeedCustom != 0 && changeReturnHomeSpeedCustom == message)
	{
		returnHomeSpeed = param/100.0f;
		if (state == RETURNING_HOME)
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,returnHomeSpeed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
	}
	if (changeMaxSightFromHomeLocationCustom != 0 && changeMaxSightFromHomeLocationCustom == message)
	{
		maxSightFromHomeLocation = param/100.0f;
		maxSightFromHomeLocation *= maxSightFromHomeLocation;
	}
	if (changeAttackSpeedCustom != 0 && changeAttackSpeedCustom == message)
	{
		attackSpeed = param/100.0f;
		if (state == ATTACKING_TARGET)
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,attackSpeed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
	}
}
void JMG_Utility_AI_Goto_Enemy_Not_Star::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		if (state == HUNTING_STAR)
		{
			ignoreEnemyId = huntingEnemyId;
			removeIgnoreTime = 10;
			state = ACTION_BADPATH;
		}
		else if (state == WANDERING_GROUP)
			state = ACTION_BADPATH;
		else if (state == ATTACKING_TARGET)
			state = ACTION_BADPATH;
	}
}
void JMG_Utility_AI_Goto_Enemy_Not_Star::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0.0f || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (!_stricmp(Get_Skin(damager),"Blamo") || !_stricmp(Get_Shield_Type(damager),"Blamo"))
		return;
	if (!targetId || !lastSeenTime || Choose_Target(obj,damager))
	{
		targetId = Commands->Get_ID(damager);
		lastSeenTime = Commands->Get_Random_Int(30,60);
		state = ATTACKING_TARGET;
		Attack_Move(obj,damager,Vector3(),attackSpeed,attackArriveDistance,true,false);
	}
}
void JMG_Utility_AI_Goto_Enemy_Not_Star::Attack_Move(GameObject *obj,GameObject *target,Vector3 location,float speed,float distance,bool attack,bool overrideLocation)
{
	lastAction.location = location;
	Commands->Action_Reset(obj,100);
	lastAction = LastAction(targetId,location,speed,distance,attack,overrideLocation);
	ActionParamsStruct params;
	params.ObserverID = this->Get_ID();
	if (target && !overrideLocation)
	{
		params.Set_Movement(target,speed,distance,false);
		params.MoveFollow = true;
	}
	else
	{
		params.Set_Movement(location,speed,distance,false);
		params.MoveFollow = false;
	}
	params.MoveBackup = moveBackward;
	params.MovePathfind = true;
	if (Get_Int_Parameter("ShutdownEngineOnArrival"))
		params.ShutdownEngineOnArrival = true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,100,100,AI_STATE_COMBAT);
	params.AttackCheckBlocked = Get_Int_Parameter("AttackCheckBlocked") ? true : false;
	if (target && attack)
	{
		params.Set_Attack(target,weaponRange,0.0f,true);
		Commands->Action_Attack(obj,params);
	}
}
GameObject *JMG_Utility_AI_Goto_Enemy_Not_Star::findClosestStar(GameObject *obj)
{
	if (Commands->Get_Player_Type(obj) == -2)
		return NULL;
	GameObject *nearest = NULL;
	float nearDist = -1.0f;
	Vector3 pos = Commands->Get_Position(obj);
	int myPlayerType = Commands->Get_Player_Type(obj);
	for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
	{
		SmartGameObj* o = current->Data();
		int oPlayerType = Commands->Get_Player_Type(o);
		int enemyId = Commands->Get_ID(o);
		if (Is_Script_Attached(o,"JMG_Utility_AI_Goto_Enemy_Not_Star_Ignore_Object"))
			continue;
		if (!(o->As_SoldierGameObj() || o->As_VehicleGameObj()))
			continue;
		if (!Commands->Get_Health(o) || Commands->Is_A_Star(o) || oPlayerType == -4 || oPlayerType == -2 || oPlayerType == myPlayerType || enemyId == ignoreEnemyId)
			continue;
		if (!_stricmp(Get_Skin(o),"Blamo") || !_stricmp(Get_Shield_Type(o),"Blamo"))
			continue;
		if (!huntStealth)
		{
			GameObject *vehicle = Get_Vehicle(o);
			if (vehicle && vehicle->As_SmartGameObj() && vehicle->As_SmartGameObj()->Is_Stealthed())
				continue;
			else if (o->As_SmartGameObj() && o->As_SmartGameObj()->Is_Stealthed())
				continue;
		}
		if (maxSightFromHomeLocation > 0 && JmgUtility::SimpleDistance(homeLocation,Commands->Get_Position(o)) > maxSightFromHomeLocation)
			continue;
		float tempDist = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o));
		if (huntSearchDistance >= 0.0f && tempDist > huntSearchDistance)
			continue;
		if (!nearest || (tempDist < nearDist))
		{
			nearest = o;
			nearDist = tempDist;
			huntingEnemyId = enemyId;
		}
	}
	return nearest;
}
void JMG_Utility_AI_Goto_Enemy_Not_Star::Return_Home(GameObject *obj,ValidLastLocation goNearLastWanderPoint)
{
	Vector3 wanderPos;
	if (goNearLastWanderPoint.valid && Commands->Get_Random(0.0f,1.0f) < Get_Float_Parameter("ChanceToInvestigateLastSeenLocation"))
	{
		state = WANDERING_GROUP;
		Attack_Move(obj,NULL,goNearLastWanderPoint.location,wanderSpeed,1.0f,false,false);
	}
	else if (wanderingAiGroupId != -1 && GetRandomPosition(&wanderPos))
	{
		state = WANDERING_GROUP;
		Attack_Move(obj,NULL,wanderPos,wanderSpeed,1.0f,false,false);
	}
	else if (Get_Int_Parameter("ReturnHome"))
	{
		state = RETURNING_HOME;
		Attack_Move(obj,NULL,homeLocation,returnHomeSpeed,1.0f,false,false);
	}
	else
	{
		state = IDLE;
		Attack_Move(obj,NULL,Commands->Get_Position(obj),1.0f,1.0f,false,false);
	}
}
void JMG_Utility_AI_Goto_Enemy_Not_Star::Stuck_Check(GameObject *obj,Vector3 targetPos)
{
	if (obj->As_VehicleGameObj())
	{
		Vector3 pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(pos,lastPosition) < 1.0f)
		{
			if (targetId && JmgUtility::SimpleDistance(pos,targetPos) <= attackArriveDistance*attackArriveDistance)
			{
				stuckTime = 0;
				return;
			}
			if (!reverseTime)
			{
				stuckTime++;
				if (stuckTime > 30)
				{
					moveBackward = true;
					Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
					reverseTime = Commands->Get_Random_Int(10,60);
					stuckTime = 0;
				}
			}
		}
		else
		{
			stuckTime = 0;
			lastPosition = pos;
		}
		if (reverseTime)
		{
			reverseTime--;
			if (!reverseTime)
			{
				moveBackward = false;
				Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
			}
		}
	}
	else
	{
		Vector3 pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(pos,lastPosition) < 1.0f)
		{
			if (targetId && JmgUtility::SimpleDistance(pos,targetPos) <= attackArriveDistance*attackArriveDistance)
			{
				stuckTime = 0;
				return;
			}
			stuckTime++;
			if (stuckTime > 30)
				Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
			if (stuckTime > 60)
				Attack_Move(obj,NULL,homeLocation,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
		}
		else
		{
			stuckTime = 0;
			lastPosition = pos;
		}
	}
}
void JMG_Utility_AI_Goto_Enemy_Not_Star::Cant_Get_To_target(GameObject *obj)
{
	Vector3 wanderPos = Vector3();
	GameObject *star = findClosestStar(obj);
	if (star && Commands->Get_ID(star) != ignoreEnemyId)
	{
		state = HUNTING_STAR;
		Attack_Move(obj,Commands->Find_Object(lastAction.targetId),Commands->Get_Position(star),huntSpeed,huntArriveDistance,true,true);
	}
	else if (wanderingAiGroupId != -1 && GetRandomPosition(&wanderPos))
	{
		state = WANDERING_GROUP;
		Attack_Move(obj,NULL,wanderPos,wanderSpeed,1.0f,true,true);
	}
	else
	{
		state = RETURNING_HOME;
		Attack_Move(obj,NULL,homeLocation,returnHomeSpeed,1.0f,true,true);
	}
}
bool JMG_Utility_AI_Goto_Enemy_Not_Star::Choose_Target(GameObject *obj,GameObject *target)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *currentTarget = Commands->Find_Object(targetId);
	if (!currentTarget)
		return true;
	if (JmgUtility::SimpleDistance(pos,Commands->Get_Position(target)) < JmgUtility::SimpleDistance(pos,Commands->Get_Position(currentTarget)))
		return true;
	return false;
}
JMG_Utility_AI_Goto_Enemy_Not_Star::ValidLastLocation::ValidLastLocation(int enemyId)
{
	GameObject *target = Commands->Find_Object(enemyId);
	if (!target || !Commands->Get_Health(target))
	{
		this->valid = false;
		return;
	}
	this->location = Commands->Get_Position(target);
	this->valid = true;
}
void JMG_Utility_Grant_Key_On_Create::Created(GameObject *obj)
{
	Commands->Grant_Key(obj,Get_Int_Parameter("Key"),Get_Int_Parameter("Grant") ? true : false);
}
void JMG_Utility_Custom_Send_Custom::Created(GameObject *obj)
{
	recieveMessage = Get_Int_Parameter("Custom");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	randomDelay = Get_Float_Parameter("RandomDelay");
	randomChance = Get_Float_Parameter("RandomChance");
}
void JMG_Utility_Custom_Send_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		if (randomChance && randomChance < Commands->Get_Random(0.0f,1.0f))
			return;
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
 		Commands->Send_Custom_Event(obj,object,custom,Param == -1 ? param : Param,delay+(randomDelay > 0 ? Commands->Get_Random(0.0f,randomDelay) : 0.0f));
	}
}
void JMG_Utility_Damage_Unoccupied_Vehicle::Created(GameObject *obj)
{
	hasBeenOccupied = false;
	rate = Get_Float_Parameter("Rate");
	damage = Get_Float_Parameter("Damage");
	sprintf(warhead,"%s",Get_Parameter("Warhead"));
	maxDelay = delay = Get_Float_Parameter("Delay");
	decreaseTick = Get_Float_Parameter("DecreaseTick");
	increaseTick = Get_Float_Parameter("IncreaseTick");
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Damage_Unoccupied_Vehicle::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (hasBeenOccupied)
		{
			if (Get_Vehicle_Occupant_Count(obj) && delay < maxDelay)
			{
				delay += increaseTick;
				if (delay > maxDelay)
					delay = maxDelay;
			}
			else
			{
				if (delay > 0)
				{
					delay -=decreaseTick;
					if (delay <= 0)
						delay = 0;
				}
				else
					Commands->Apply_Damage(obj,damage,warhead,obj);
			}

		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Damage_Unoccupied_Vehicle::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		hasBeenOccupied = true;
	}
}
void JMG_Utility_Custom_Damage_All_Soldiers_On_Team::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	team = Get_Int_Parameter("Team");
	damage = Get_Float_Parameter("Damage");
	sprintf(warhead,"%s",Get_Parameter("Warhead"));
}
void JMG_Utility_Custom_Damage_All_Soldiers_On_Team::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj* o = current->Data();
			if (o->As_SoldierGameObj() && !CheckPlayerType(o,team))
				Commands->Apply_Damage(o,damage,warhead,o);
		}
	}
}
void JMG_Utility_AI_Guardian_Vehicle::Created(GameObject *obj)
{
	backward = 0;
	aimAtFeet = Get_Int_Parameter("AimAtFeet") ? true : false;
	dpPosition = Commands->Get_Position(obj);
	EnemyID = 0;
	EnemyTimeOutTime = 0;
	LastPos = Commands->Get_Position(obj);
	stealthModeOverride = Get_Int_Parameter("StealthModeOverride");
	float weaponRange = 0.0f;
	const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Current_Weapon(obj),true);
	if (ammo)
		weaponRange = ammo->Range;
	fireRange = Get_Float_Parameter("FireRange") >= 0 ? Get_Float_Parameter("FireRange") : weaponRange;
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Enable_Engine(obj,true);
	Commands->Start_Timer(obj,this,1.0f,1);
	if (aimAtFeet)
		Commands->Start_Timer(obj,this,0.25f,2);
}
void JMG_Utility_AI_Guardian_Vehicle::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (EnemyTimeOutTime)
		{
			EnemyTimeOutTime--;
			if (!EnemyTimeOutTime)
			{
				EnemyID = 0;
				Goto_Location(obj);
			}
		}
		if (EnemyID)
		{
			GameObject *Target = Commands->Find_Object(EnemyID);
			if (!Target || !Commands->Get_Health(Target) || (Commands->Get_Player_Type(obj) != -2 && Commands->Get_Player_Type(Target) == Commands->Get_Player_Type(obj)))
			{
				EnemyID = 0;
				Goto_Location(obj);
			}
		}
		if (backward)
		{
			backward--;
			if (!backward)
				Goto_Location(obj);
		}
		Vector3 Pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(Pos,LastPos) < 1.0f)
		{
			stuckTime++;
			if (stuckTime > 1)
			{
				stuckTime = 0;
				backward = Commands->Get_Random_Int(1,4);
				Goto_Location(obj);
			}
			if (Get_A_Defense_Point(&dpPosition))
				Goto_Location(obj);
		}
		else
			stuckTime = 0;
		LastPos = Pos;
		Commands->Start_Timer(obj,this,1.0f,1);
	}
	if (number == 2)
	{
		GameObject *Target = Commands->Find_Object(EnemyID);
		if (Target && Commands->Get_Health(Target) && Target->As_SoldierGameObj())
		{
			ActionParamsStruct params;
			Commands->Get_Action_Params(obj,params);
			params.AttackCheckBlocked = false;
			params.Set_Attack(Commands->Get_Position(Target),Get_Float_Parameter("FireRange"),0,true);
			Commands->Action_Attack(obj,params);
		}
		Commands->Start_Timer(obj,this,0.25f,2);
	}
}
void JMG_Utility_AI_Guardian_Vehicle::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (!Commands->Get_Health(seen))
		return;
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Guardian_Ignored"))
		return;
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Guardian_Vehicle_Ignored"))
		return;
	if (!JmgUtility::CanSeeStealth(stealthModeOverride,obj,seen))
		return;
	if (!EnemyID || !EnemyTimeOutTime)
	{
		EnemyID = Commands->Get_ID(seen);
		EnemyTimeOutTime = Commands->Get_Random_Int(2,5);
		Goto_Location(obj);
	}
	else if (EnemyID == Commands->Get_ID(seen))
		EnemyTimeOutTime = Commands->Get_Random_Int(2,5);
}
void JMG_Utility_AI_Guardian_Vehicle::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0 || (EnemyID && EnemyTimeOutTime) || (Commands->Get_Player_Type(obj) != -2 && Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj)))
		return;
	EnemyID = Commands->Get_ID(damager);
	EnemyTimeOutTime = Commands->Get_Random_Int(2,5);
	Goto_Location(obj);
}
void JMG_Utility_AI_Guardian_Vehicle::Goto_Location(GameObject *obj)
{
	ActionParamsStruct params;
	Commands->Action_Reset(obj,100);
	Vector3 GoToPos = dpPosition;
	params.Set_Movement(GoToPos,Get_Float_Parameter("WanderSpeed"),1.0f,false);
	params.MovePathfind = true;
	params.MoveBackup = backward ? true : false;
	params.Set_Basic(this,100,10);
	GameObject *Target = Commands->Find_Object(EnemyID);
	if (Target)
	{
		if (Get_Int_Parameter("FaceTarget"))
			params.Set_Face_Location(Commands->Get_Position(Target),5.0f);
		else
			params.AttackFaceTarget = false;
		params.AttackCheckBlocked = Get_Int_Parameter("CheckBlocked") ? true : false;
		if (aimAtFeet && Target->As_SoldierGameObj())
		{
			params.AttackCheckBlocked = false;
			params.Set_Attack(Commands->Get_Position(Target),fireRange,0,true);
		}
		else
			params.Set_Attack(Target,fireRange,0,true);
		Commands->Action_Attack(obj,params);
	}
	else
	{
		if (Get_Int_Parameter("FaceTarget"))
			params.Set_Face_Location(GoToPos,5.0f);
		else
			params.AttackFaceTarget = false;
		params.ShutdownEngineOnArrival = Get_Int_Parameter("TurnOffEngineOnArrival") ? true : false;
		Commands->Action_Goto(obj,params);
	}
}
void JMG_Utility_Custom_Destroy_Closest_Model_To_Position::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Destroy_Closest_Model_To_Position::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		GameObject *nearest = NULL;
		float dist = 0.0f;
		Vector3 pos = Get_Vector3_Parameter("Position");
		const char *modelName = Get_Parameter("Model");
		float maxDist = Get_Float_Parameter("MaxDistance");
		maxDist *= maxDist;
		for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
		{
			GameObject* o = current->Data()->As_ScriptableGameObj();
			if (!_stricmp(modelName,Get_Model(o)))
			{
				float tempDistance = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o));
				if (!nearest || (tempDistance < dist && (maxDist == 0.0 || tempDistance <= maxDist)))
				{
					nearest = o;
					dist = tempDistance;
				}
			}
		}
		if (nearest)
			Commands->Destroy_Object(nearest);
	}
}
int JMG_Utility_Send_Custom_On_Deaths_Controller::deathCount = 0;
int JMG_Utility_Send_Custom_On_Deaths_Controller::maxDeaths = 0;
int JMG_Utility_Send_Custom_On_Deaths_Controller::deathReminder = 0;
int JMG_Utility_Send_Custom_On_Deaths_Controller::urgentDeathReminder = 0;
int JMG_Utility_Send_Custom_On_Deaths_Controller::stringId = 0;
Vector3 JMG_Utility_Send_Custom_On_Deaths_Controller::reminderColor = Vector3();
char JMG_Utility_Send_Custom_On_Deaths_Controller::reminderMessage[220] = "";
char JMG_Utility_Send_Custom_On_Deaths_Controller::deathSingular[220] = "";
char JMG_Utility_Send_Custom_On_Deaths_Controller::deathPlural[220] = "";
char JMG_Utility_Send_Custom_On_Deaths_Controller::remainingSingular[220] = "";
char JMG_Utility_Send_Custom_On_Deaths_Controller::remainingPlural[220] = "";
int JMG_Utility_Send_Custom_On_Deaths_Controller::reminderMessageOrder = 0;
int JMG_Utility_Send_Custom_On_Deaths_Controller::myId = 0;
bool JMG_Utility_Send_Custom_On_Deaths_Controller::controllerPlaced = false;
bool JMG_Utility_Send_Custom_On_Deaths_Controller::announceOnFirstDeath = false;
bool JMG_Utility_Send_Custom_On_Deaths_Controller::onlyTrackPlayerDeaths = false;
bool JMG_Utility_Send_Custom_On_Deaths_Controller::recordAIDeaths = false;
void JMG_Utility_Send_Custom_On_Deaths_Controller::Created(GameObject *obj)
{
	if (controllerPlaced)
	{
		Console_Input("msg JMG_Utility_Send_Custom_On_Deaths_Controller ERROR: Only place one controller on the map at a time!");
		return;
	}
	deathCount = 0;
	maxDeaths = Get_Int_Parameter("MaxDeaths");
	deathReminder = Get_Int_Parameter("DeathReminder");
	urgentDeathReminder = Get_Int_Parameter("UrgentDeathReminder");
	stringId = Get_Int_Parameter("StringId");
	reminderColor = Get_Vector3_Parameter("ReminderColor");
	const char *parameters = Get_Parameter("ReminderMessage");
	sprintf(reminderMessage,"%s",parameters);
	char delim = Get_Parameter("Delim")[0];
	unsigned int length = strlen(parameters);
	for (unsigned int x = 0;x < length;x++)
		if (reminderMessage[x] == delim)
			reminderMessage[x] = ',';
	reminderMessageOrder = Get_Int_Parameter("ReminderMessageOrder");
	sprintf(deathSingular,"%s",Get_Parameter("DeathSingular"));
	sprintf(deathPlural,"%s",Get_Parameter("DeathPlural"));
	sprintf(remainingSingular,"%s",Get_Parameter("RemainingSingular"));
	sprintf(remainingPlural,"%s",Get_Parameter("RemainingPlural"));
	addDeathsWhenNoPlayers = Get_Float_Parameter("AddDeathsWhenNoPlayers");
	currentNoPlayersAddDeathSaftyTime = noPlayersAddDeathSaftyTime = Get_Int_Parameter("NoPlayersAddDeathSaftyTime");
	if (addDeathsWhenNoPlayers > 0)
		Commands->Start_Timer(obj,this,addDeathsWhenNoPlayers,1);
	controllerPlaced = true;
	requiresADeathToStartNoPlayerAdd = Get_Int_Parameter("RequiresADeathToStartNoPlayerAdd") ? true : false;
	announceOnFirstDeath = Get_Int_Parameter("AnnounceOnFirstDeath") ? true : false;
	onlyTrackPlayerDeaths = Get_Int_Parameter("OnlyTrackPlayerDeaths") ? true : false;
	recordAIDeaths = Get_Int_Parameter("RecordAIDeaths") ? true : false;
	myId = Commands->Get_ID(obj);
}
void JMG_Utility_Send_Custom_On_Deaths_Controller::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!Get_Player_Count() && (!requiresADeathToStartNoPlayerAdd || JMG_Utility_Send_Custom_On_Deaths_Controller::deathCount))
		{
			if (currentNoPlayersAddDeathSaftyTime)
				currentNoPlayersAddDeathSaftyTime--;
			if (!currentNoPlayersAddDeathSaftyTime)
			{
				JMG_Utility_Send_Custom_On_Deaths_Controller::deathCount++;
				int remainingLives = JMG_Utility_Send_Custom_On_Deaths_Controller::maxDeaths-JMG_Utility_Send_Custom_On_Deaths_Controller::deathCount;
				if (!remainingLives)
					Commands->Send_Custom_Event(obj,obj,8765432,8765432,0);
			}
		}
		else
			currentNoPlayersAddDeathSaftyTime = noPlayersAddDeathSaftyTime;
		Commands->Start_Timer(obj,this,addDeathsWhenNoPlayers,1);
	}
}
void JMG_Utility_Send_Custom_On_Deaths_Controller::Destroyed(GameObject *obj)
{
	controllerPlaced = false;
}
void JMG_Utility_Send_Custom_On_Deaths_Controller::Detach(GameObject *obj)
{
	controllerPlaced = false;
}
void JMG_Utility_Send_Custom_On_Deaths_Controller::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 8765432 && param == 8765432)
	{
		int id = Get_Int_Parameter("ID");
		GameObject *object = id ? Commands->Find_Object(id) : obj;
		if (object)
			Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	}
	if (message == 8765433)
	{
		int id = Get_Int_Parameter("ID");
		GameObject *object = id ? Commands->Find_Object(id) : obj;
		if (object)
			Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("ReminderCustom"),param,0);
	}
}
void JMG_Utility_Send_Custom_On_Deaths_Controller::ReportLogic(GameObject *obj)
{
	if (!controllerPlaced)
		return;
	if (onlyTrackPlayerDeaths && !(recordAIDeaths || Commands->Is_A_Star(obj)))
		return;
	deathCount++;
	int remainingLives = maxDeaths-deathCount;
	if ((!(remainingLives%deathReminder) || remainingLives <= urgentDeathReminder) || (deathCount == 1 && announceOnFirstDeath))
	{
		if (remainingLives > 0)
		{
			char remainingLivesMessage[220];
			switch (reminderMessageOrder)
			{
			case 0:sprintf(remainingLivesMessage,"%s",formatReminderString(reminderMessage,deathCount,deathCount == 1 ? deathSingular : deathPlural,remainingLives,remainingLives == 1 ? remainingSingular : remainingPlural));break;
			case 1:sprintf(remainingLivesMessage,"%s",formatReminderString(reminderMessage,remainingLives,remainingLives == 1 ? remainingSingular : remainingPlural,deathCount,deathCount == 1 ? deathSingular : deathPlural));break;
			case 2:sprintf(remainingLivesMessage,"%s",formatReminderString(reminderMessage,remainingLives,remainingLives == 1 ? remainingSingular : remainingPlural));break;
			default:sprintf(remainingLivesMessage,"JMG_Utility_Send_Custom_On_Deaths_Controller ERROR: Invalid reminderMessageOrder!");break;
			}
			JmgUtility::SetHUDHelpText(stringId,remainingLivesMessage,reminderColor);
		}
		GameObject *controller = Commands->Find_Object(myId);
		Commands->Send_Custom_Event(obj,controller,8765433,remainingLives,0);
	}
	if (!remainingLives)
	{
		GameObject *controller = Commands->Find_Object(myId);
		Commands->Send_Custom_Event(obj,controller,8765432,8765432,0);
	}
}
char *JMG_Utility_Send_Custom_On_Deaths_Controller::formatReminderString(const char *format,...)
{
	static char displayMsg[256];
	va_list args;
	va_start(args,format);
	vsprintf(displayMsg,format,args);
	va_end(args);
	return displayMsg;
}
void JMG_Utility_Send_Custom_On_Deaths_Reporter::Killed(GameObject *obj,GameObject *killer)
{
	JMG_Utility_Send_Custom_On_Deaths_Controller::ReportLogic(obj);
}
void JMG_Utility_AI_Goto_Player_Ignore_Object::Created(GameObject *obj)
{
}
void JMG_Utility_AI_Goto_Enemy_Ignore_Object::Created(GameObject *obj)
{
}
void JMG_Utility_AI_Goto_Enemy_Not_Star_Ignore_Object::Created(GameObject *obj)
{
}
void JMG_Utility_Custom_Set_Team_And_Model::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Set_Team_And_Model::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Set_Model(obj,Get_Parameter("Model"));
		Commands->Set_Player_Type(obj,Get_Int_Parameter("Team"));
		Commands->Action_Reset(obj,Get_Float_Parameter("ResetActionPriority"));
	}
}
void JMG_Utility_Death_Warhead_Create_Object::Created(GameObject *obj)
{
	lastDamageWarhead = 9999;
}
void JMG_Utility_Death_Warhead_Create_Object::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	lastDamageWarhead = Get_Damage_Warhead();
}
void JMG_Utility_Death_Warhead_Create_Object::Killed(GameObject *obj,GameObject *killer)
{
	if (lastDamageWarhead != ArmorWarheadManager::Get_Warhead_Type(Get_Parameter("WarheadName")))
		return;
	if (Commands->Get_Random(0.0f,1.0f) > Get_Float_Parameter("Chance"))
		return;
	GameObject *spawned = Commands->Create_Object(Get_Parameter("Preset"),Commands->Get_Position(obj));
	Commands->Set_Facing(spawned,Commands->Get_Facing(obj));
	Commands->Set_Model(obj,"null");
}
void JMG_Utility_Zone_Change_Character_On_Preset_Enter::Created(GameObject *obj)
{
	playerType = Get_Int_Parameter("PlayerType");
	sprintf(entererPreset,"%s",Get_Parameter("EntererPreset"));
}
void JMG_Utility_Zone_Change_Character_On_Preset_Enter::Entered(GameObject *obj,GameObject *enterer)
{
	if (CheckPlayerType(enterer,playerType) || _stricmp(Commands->Get_Preset_Name(enterer),entererPreset) || Commands->Get_Player_Type(enterer) == -4)
		return;
	Change_Character(enterer,Get_Parameter("Preset"));
}
void JMG_Utility_Sync_HP_With_Object::Created(GameObject *obj)
{
	id = Get_Int_Parameter("ID");
	rate = Get_Float_Parameter("Rate");
	GameObject *object = Commands->Find_Object(id);
	if (object)
	{
		Set_Max_Health(obj,Commands->Get_Max_Health(object));
		Set_Max_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(object));
		Commands->Set_Health(obj,Commands->Get_Health(object));
		Commands->Set_Shield_Strength(obj,Commands->Get_Shield_Strength(object));
		Commands->Start_Timer(obj,this,rate,1);
	}
	else
		Commands->Destroy_Object(obj);
}
void JMG_Utility_Sync_HP_With_Object::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		GameObject *object = Commands->Find_Object(id);
		if (object)
		{
			Commands->Set_Health(obj,Commands->Get_Health(object));
			Commands->Set_Shield_Strength(obj,Commands->Get_Shield_Strength(object));
		}
		else
			Commands->Destroy_Object(obj);
		Commands->Start_Timer(obj,this,rate,1);
	}
}
Vector3 JMG_Utility_Set_Screen_Color_Fade_Controller::color[128] = {Vector3(0.0f,0.0f,0.0f)};
float JMG_Utility_Set_Screen_Color_Fade_Controller::opacity[128] = {0.0f};
bool JMG_Utility_Set_Screen_Color_Fade_Controller::controllerPlaced = false;
Vector3 JMG_Utility_Set_Screen_Color_Fade_Controller::controllerDefaultColor = Vector3(0.0f,0.0f,0.0f);
float JMG_Utility_Set_Screen_Color_Fade_Controller::controllerDefaultOpacity = 0.0f;
void JMG_Utility_Set_Screen_Color_Fade_Controller::Created(GameObject *obj)
{
	controllerDefaultColor = Get_Vector3_Parameter("Color");
	controllerDefaultOpacity = Get_Float_Parameter("Opacity");
	for (int x = 1;x < 128;x++)
	{
		color[x] = Get_Vector3_Parameter("Color");
		opacity[x] = Get_Float_Parameter("Opacity");
		syncedScreen[x] = false;
	}
	Commands->Start_Timer(obj,this,1.0,1);
}
void JMG_Utility_Set_Screen_Color_Fade_Controller::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1) 
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (player && !syncedScreen[x])
			{
				syncedScreen[x] = true;
				Set_Screen_Fade_Color_Player(player,color[x].X,color[x].Y,color[x].Z,0.0f);
				Set_Screen_Fade_Opacity_Player(player,opacity[x],0.0f);
			}
			if (!player && syncedScreen[x])
				syncedScreen[x] = false;
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}
void JMG_Utility_Set_Screen_Color_Fade_Controller::Destroyed(GameObject *obj)
{
	Update_Colors(Vector3(),0.0f);
	controllerPlaced = false;
}
void JMG_Utility_Set_Screen_Color_Fade_Controller::Detach(GameObject *obj)
{
	Destroyed(obj);
}
void JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Colors(Vector3 Color,float Opacity)
{
	controllerDefaultColor = Color;
	controllerDefaultOpacity = Opacity;
	for (int x = 0;x < 128;x++)
	{
		color[x] = Color;
		opacity[x] = Opacity;
	}
}
void JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Player_Colors(GameObject *player,Vector3 Color,float Opacity)
{
	int playerId = JmgUtility::JMG_Get_Player_ID(player);
	if (Color.X >= 0.0f)
		color[playerId] = Color;
	if (Opacity >= 0.0f)
		opacity[playerId] = Opacity;
}
void JMG_Utility_Set_Screen_Color_Fade_Controller::Reset_To_Default(GameObject *player)
{
	int playerId = JmgUtility::JMG_Get_Player_ID(player);
	color[playerId] = controllerDefaultColor;
	opacity[playerId] = controllerDefaultOpacity;
}
void JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Player(GameObject *player,float transition)
{
	int playerId = JmgUtility::JMG_Get_Player_ID(player);
	Set_Screen_Fade_Color_Player(player,color[playerId].X,color[playerId].Y,color[playerId].Z,transition);
	Set_Screen_Fade_Opacity_Player(player,opacity[playerId],transition);
}
void JMG_Utility_Set_Screen_Color_Fade_Controller::Update_All_Players(float transition)
{
	for (int x = 1;x < 128;x++)
	{
		GameObject *player = Get_GameObj(x);
		if (!player)
			continue;
		Set_Screen_Fade_Color_Player(player,color[x].X,color[x].Y,color[x].Z,transition);
		Set_Screen_Fade_Opacity_Player(player,opacity[x],transition);
	}
}
void JMG_Utility_Set_Screen_Color_Fade_On_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Set_Screen_Color_Fade_On_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		if (!JMG_Utility_Set_Screen_Color_Fade_Controller::controllerPlaced)
		{
			if (!The_Game()->Is_Game_Over())
				Console_Input("msg JMG_Utility_Set_Screen_Color_Fade_On_Enter ERROR: You need to place JMG_Utility_Set_Screen_Color_Fade_Controller on an object in order to use this script!");
			return;
		}
		JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Colors(Get_Vector3_Parameter("Color"),Get_Float_Parameter("Opacity"));
		JMG_Utility_Set_Screen_Color_Fade_Controller::Update_All_Players(Get_Float_Parameter("Transition"));
		if (Get_Int_Parameter("TriggerOnce"))
			Destroy_Script();
	}
}
void JMG_Utility_Set_Screen_Color_Fade_Reset_Player_Create::Created(GameObject *obj)
{
	if (!Commands->Is_A_Star(obj))
	{
		Destroy_Script();
		return;
	}
	JMG_Utility_Set_Screen_Color_Fade_Controller::Reset_To_Default(obj);
	JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Player(obj,Get_Float_Parameter("Transition"));
}
void JMG_Utility_Set_Screen_Color_Fade_Reset_Player_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || !Commands->Is_A_Star(enter))
		return;
	JMG_Utility_Set_Screen_Color_Fade_Controller::Reset_To_Default(obj);
	JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Player(enter,Get_Float_Parameter("Transition"));
}
void JMG_Utility_Simple_Mech::Created(GameObject *obj)
{
	idleCooldown = 0;
	releaseDelay = Get_Int_Parameter("ReleaseDelay");
	currentDirection = mvmtDir::IDLE;
	lastMovementDirection = mvmtDir::FORWARD;
	sprintf(idleAnimation,"%s",Get_Parameter("IdleAnimation"));
	hasIdleAnimation = true;
	if (!_stricmp(idleAnimation,""))
		hasIdleAnimation = false;
	sprintf(forwardAnimation,"%s",Get_Parameter("ForwardAnimation"));
	sprintf(backwardAnimation,"%s",Get_Parameter("BackwardAnimation"));
	sprintf(turnLeftAnimation,"%s",Get_Parameter("TurnLeftAnimation"));
	sprintf(turnRightAnimation,"%s",Get_Parameter("TurnRightAnimation"));
	if (!hasIdleAnimation)
		Commands->Set_Animation(obj,idleAnimation,true,0,0.0f,-1,true);
	else
		Commands->Set_Animation(obj,forwardAnimation,false,0,0.0f,0.0f,true);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Simple_Mech::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (idleCooldown)
			idleCooldown--;
		PhysControllerClass *controller = obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass()->Get_Controller();
		mvmtDir newDirection = mvmtDir::IDLE;
        if(controller->Get_Move_Forward() > 0)
            newDirection = mvmtDir::FORWARD;
        else if(controller->Get_Move_Forward() < 0)
            newDirection = mvmtDir::BACKWARD;
        if (newDirection == mvmtDir::IDLE)
            if(controller->Get_Turn_Left() > 0)
                newDirection = mvmtDir::LEFT;
            else if(controller->Get_Turn_Left() < 0)
                newDirection = mvmtDir::RIGHT;
		if (obj->As_VehicleGameObj()->Is_Immovable())
			newDirection = mvmtDir::IDLE;
		if (!idleCooldown && currentDirection != newDirection)
		{
			float currentAnimationFrame = Get_Animation_Frame(obj);
			if (newDirection == mvmtDir::IDLE)
			{
				if (hasIdleAnimation)
					Commands->Set_Animation(obj,idleAnimation,true,0,currentAnimationFrame,-1,true);
				else
					PlayAnimation(obj,currentDirection,currentAnimationFrame,currentAnimationFrame,false);
			}
			else
				if (currentAnimationFrame != 0.0f && lastMovementDirection != newDirection)
					PlayAnimation(obj,lastMovementDirection,currentAnimationFrame,0.0f,true);
				else
					PlayAnimation(obj,newDirection,currentAnimationFrame,-1,true);
			if (newDirection != mvmtDir::IDLE)
				lastMovementDirection = newDirection;
			currentDirection = newDirection;
		}
		if (currentDirection == newDirection && (controller->Get_Move_Forward() || controller->Get_Turn_Left()))
			idleCooldown = releaseDelay;
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Simple_Mech::Animation_Complete(GameObject *obj,const char *animation_name)
{
	PlayAnimation(obj,currentDirection,0.0f,-1,true);
}
void JMG_Utility_Simple_Mech::PlayAnimation(GameObject *obj,mvmtDir direction,float startFrame,float endFrame,bool looped)
{
	if (direction == mvmtDir::FORWARD)
		Commands->Set_Animation(obj,forwardAnimation,looped,0,startFrame,endFrame,true);
	else if (direction == mvmtDir::BACKWARD)
		Commands->Set_Animation(obj,backwardAnimation,looped,0,startFrame,endFrame,true);
	else if (direction == mvmtDir::LEFT)
		Commands->Set_Animation(obj,turnLeftAnimation,looped,0,startFrame,endFrame,true);
	else if (direction == mvmtDir::RIGHT)
		Commands->Set_Animation(obj,turnRightAnimation,looped,0,startFrame,endFrame,true);
}
void JMG_Utility_Send_Custom_When_Preset_In_Range::Created(GameObject *obj)
{
	range = Get_Float_Parameter("Range");
	range *= range;
	sprintf(preset,"%s",Get_Parameter("Preset"));
	rate = Get_Float_Parameter("Rate");
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Send_Custom_When_Preset_In_Range::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 mypos = Commands->Get_Position(obj);
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj* o = current->Data();
			if (o != obj && JmgUtility::SimpleDistance(Commands->Get_Position(o),mypos) <= range && !_stricmp(Commands->Get_Preset_Name(o),preset))
			{
				int id = Get_Int_Parameter("ID");
				GameObject *object = id ? (id == -1 ? o : Commands->Find_Object(id)) : obj;
				if (object)
					Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
				if (Get_Int_Parameter("TriggerOnce"))
				{
					Destroy_Script();
					return;
				}
				break;
			}
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Death_Create_Object_At_Bone_Position::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object = Commands->Create_Object(Get_Parameter("Preset"),Commands->Get_Position(obj));
	Commands->Attach_To_Object_Bone(object,obj,Get_Parameter("Bone"));
	Vector3 targetPos = Commands->Get_Position(object);
	MoveablePhysClass *mphys = object->As_PhysicalGameObj() ? object->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	Commands->Attach_To_Object_Bone(object,NULL,"");
	if (mphys && mphys->Find_Teleport_Location(targetPos,Get_Float_Parameter("MaxDistance"),&targetPos))
	{
		Commands->Set_Position(object,targetPos);
		Force_Position_Update(object);
	}
}
void JMG_Utility_Enable_Loiter::Created(GameObject *obj)
{
	Commands->Set_Loiters_Allowed(obj,true);
}
void JMG_Utility_Custom_Switch_Weapon::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Switch_Weapon::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Select_Weapon(obj,Get_Parameter("Weapon"));
	}
}
JMG_Utility_HUD_Count_Down::SendCustomOnSecondNode *JMG_Utility_HUD_Count_Down::sendCustomOnSecondController = NULL;
JMG_Utility_HUD_Count_Down::CountdownScreenCharacterNode *JMG_Utility_HUD_Count_Down::countdownScreenCharacterController = NULL;
int JMG_Utility_HUD_Count_Down::highestCharPos = -1;
bool JMG_Utility_HUD_Count_Down::controllerPlaced = false;
void JMG_Utility_HUD_Count_Down::Created(GameObject *obj)
{
	seconds = Get_Int_Parameter("TimeInSeconds");
	stringId = Get_Int_Parameter("StringID");
	color = Get_Vector3_Parameter("Color");
	sprintf(hourSingular,"%s",Get_Parameter("HourSingular"));
	sprintf(hourPlural,"%s",Get_Parameter("HourPlural"));
	sprintf(minuteSingular,"%s",Get_Parameter("MinuteSingular"));
	sprintf(minutePlural,"%s",Get_Parameter("MinutePlural"));
	sprintf(secondSingular,"%s",Get_Parameter("SecondSingular"));
	sprintf(secondPlural,"%s",Get_Parameter("SecondPlural"));
	pauseCustom = Get_Int_Parameter("PauseCustom");
	paused = Get_Int_Parameter("StartsPaused") != 0;
	const char *parameters = Get_Parameter("WarningMessage");
	sprintf(warningMessage,"%s",parameters);
	char delim = Get_Parameter("Delim")[0];
	unsigned int length = strlen(parameters);
	for (unsigned int x = 0;x < length;x++)
		if (warningMessage[x] == delim)
			warningMessage[x] = ',';
	controllerPlaced = true;
	Commands->Start_Timer(obj,this,1.0f,1);
	if (paused)
		Commands->Start_Timer(obj,this,1.0f,2);
}
void JMG_Utility_HUD_Count_Down::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (seconds && !paused)
		{
			seconds--;
			UpdateCountdownHudNodes();
			if (seconds >= 3600 && !(seconds % 3600))
				JmgUtility::SetHUDHelpText(stringId,formatReminderString(warningMessage,seconds/3600,seconds/3600 == 1 ? hourSingular : hourPlural),color);
			else if (seconds >= 60 && (seconds == 1800 || seconds == 1200 || seconds == 600 || (seconds <= 300 && !(seconds % 60))))
				JmgUtility::SetHUDHelpText(stringId,formatReminderString(warningMessage,seconds/60,seconds/60 == 1 ? minuteSingular : minutePlural),color);
			else if (seconds && (seconds <= 10 || seconds == 30 || seconds == 20))
				JmgUtility::SetHUDHelpText(stringId,formatReminderString(warningMessage,seconds,seconds == 1 ? secondSingular : secondPlural),color);
			NodeSendCustom(obj,seconds);
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
	if (number == 2)
	{
		UpdateCountdownHudNodes();
	}
}
void JMG_Utility_HUD_Count_Down::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == pauseCustom)
	{
		paused = param != 0;
	}
}
void JMG_Utility_HUD_Count_Down::Destroyed(GameObject *obj)
{
	if (controllerPlaced)
		CleanupSecondNodes();
}
void JMG_Utility_HUD_Count_Down::Detach(GameObject *obj)
{
	Destroyed(obj);
}
char *JMG_Utility_HUD_Count_Down::formatReminderString(const char *format,...)
{
	static char displayMsg[256];
	va_list args;
	va_start(args,format);
	vsprintf(displayMsg,format,args);
	va_end(args);
	return displayMsg;
}
void JMG_Utility_HUD_Count_Down_Send_Custom::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_HUD_Count_Down_Send_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		int id = Get_Int_Parameter("ID");
		id = id ? id : Commands->Get_ID(obj);
		JMG_Utility_HUD_Count_Down::AddSecondNode(Get_Int_Parameter("TriggerTime"),id,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	}
}
void JMG_Utility_Zone_Screen_Fade::Entered(GameObject *obj,GameObject *enterer)
{
	if (CheckPlayerType(enterer,Get_Int_Parameter("PlayerType")) || !Commands->Is_A_Star(enterer))
		return;
	Vector3 color = Get_Vector3_Parameter("Color");
	float opacity = Get_Float_Parameter("Opacity");
	JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Player_Colors(enterer,color,opacity);
	float transition = Get_Float_Parameter("Transition");
	if (opacity >= 0.0f)
		Set_Screen_Fade_Opacity_Player(enterer,opacity,transition);
	if (color.X >= 0.0f)
		Set_Screen_Fade_Color_Player(enterer,color.X,color.Y,color.Z,transition);
}
void JMG_Utility_Custom_Triggers_Enemy_Seen::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Triggers_Enemy_Seen::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Set_Is_Visible(obj,Get_Int_Parameter("Visible") ? true : false);
	}
}
void JMG_Utility_Screen_Fade_Red_On_Damage::Created(GameObject *obj)
{
	if (!Commands->Is_A_Star(obj))
	{
		Destroy_Script();
		return;
	}
	fadeColor = Get_Vector3_Parameter("FadeColor");
	fadeOpacity = Get_Float_Parameter("FadeOpacity");
	playerId = JmgUtility::JMG_Get_Player_ID(obj);
	injuryRatio = 0.0f;
	screenOpacity = 0.0f;
	lastScreenOpacity = 0.0f;
	lastInjuryRatio = 0.0f;
	ScreenFade(obj);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Screen_Fade_Red_On_Damage::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1 && Commands->Get_Health(obj) > 0)
	{
		if (injuryRatio > 0)
			injuryRatio -= 0.01f;
		else
			injuryRatio = 0.0f;
		if (screenOpacity > 0)
			screenOpacity -= 0.01f;
		else
			screenOpacity = 0.0f;
		ScreenFade(obj);
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Screen_Fade_Red_On_Damage::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage > 1.0f)
	{
		float totalHP = Commands->Get_Max_Health(obj) + Commands->Get_Max_Shield_Strength(obj);
		float temp = damage/totalHP;
		if (screenOpacity + temp < fadeOpacity)
			screenOpacity += temp;
		else
			screenOpacity = fadeOpacity;
		if (injuryRatio + temp < 1.0f)
			injuryRatio += temp;
		else
			injuryRatio = 1.0f;
		if (!Commands->Get_Health(obj))
			ScreenFade(obj);
	}
}
void JMG_Utility_Screen_Fade_Red_On_Damage::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 596911)
	{
		float tempParam = (float)param / 100.0f;
		if (tempParam > 1.0f)
		{
			float totalHP = Commands->Get_Max_Health(obj) + Commands->Get_Max_Shield_Strength(obj);
			float temp = tempParam/totalHP;
			if (screenOpacity + temp < fadeOpacity)
				screenOpacity += temp;
			else
				screenOpacity = fadeOpacity;
			if (injuryRatio + temp < 1.0f)
				injuryRatio += temp;
			else
				injuryRatio = 1.0f;
		}
	}
}
void JMG_Utility_Screen_Fade_Red_On_Damage::ScreenFade(GameObject *obj)
{
	if (lastScreenOpacity == screenOpacity && lastInjuryRatio == injuryRatio)
		return;
	if ((JMG_Utility_Swimming_Infantry::isUnderwater[playerId] && JMG_Utility_Swimming_Zone::waterNode[playerId].waterColorOpacity) || JMG_Utility_Set_Screen_Color_Fade_Controller::opacity[playerId])
	{
		float temp = 1.0f-injuryRatio;
		if (JMG_Utility_Swimming_Infantry::isUnderwater[playerId])
		{
			Set_Screen_Fade_Color_Player(obj,(JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.X*temp+injuryRatio*fadeColor.X),JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.Y*temp+injuryRatio*fadeColor.Y,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.Z*temp+injuryRatio*fadeColor.Z,0.1f);
			Set_Screen_Fade_Opacity_Player(obj,JmgUtility::MathClamp(screenOpacity+JMG_Utility_Swimming_Zone::waterNode[playerId].waterColorOpacity*temp,0.0f,fadeOpacity),0.1f);
		}
		else
		{
			Set_Screen_Fade_Color_Player(obj,(JMG_Utility_Set_Screen_Color_Fade_Controller::color[playerId].X*temp+injuryRatio*fadeColor.X),JMG_Utility_Set_Screen_Color_Fade_Controller::color[playerId].Y*temp+injuryRatio*fadeColor.Y,JMG_Utility_Set_Screen_Color_Fade_Controller::color[playerId].Z*temp+injuryRatio*fadeColor.Z,0.1f);
			Set_Screen_Fade_Opacity_Player(obj,JmgUtility::MathClamp(screenOpacity+JMG_Utility_Set_Screen_Color_Fade_Controller::opacity[playerId]*temp,0.0f,fadeOpacity),0.1f);
		}
	}
	else
	{
		Set_Screen_Fade_Color_Player(obj,fadeColor.X,fadeColor.Y,fadeColor.Z,0.1f);
		Set_Screen_Fade_Opacity_Player(obj,screenOpacity,0.1f);
	}
	lastScreenOpacity = screenOpacity;
	lastInjuryRatio = injuryRatio;
}
void JMG_Utility_Player_Count_Enable_Spawners::Created(GameObject *obj)
{
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	enableCustom = Get_Int_Parameter("EnableOrDisableCustom");
	playerCount = Get_Vector3_Parameter("PlayerCount[eq|<|>]");
	playerCount2 = Get_Vector3_Parameter("PlayerCount[neq|between(|)]");
	conditionMatching = false;
	EnableSpawners(false);
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_Player_Count_Enable_Spawners::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (enabled)
		{
			int tempCount = Get_Player_Count();
			if (playerCount.X >= 0.0 && tempCount == playerCount.X)// equal
				EnableSpawners(true);
			else if (playerCount.Y >= 0.0 && tempCount < playerCount.Y)// less than
				EnableSpawners(true);
			else if (playerCount.Z >= 0.0 && tempCount > playerCount.Z)// greater than
				EnableSpawners(true);
			else if (playerCount2.X >= 0.0 && tempCount != playerCount2.X)// not equal
				EnableSpawners(true);
			else if (playerCount2.Y >= 0.0 && playerCount2.Z >= 0.0 && tempCount > playerCount2.Y && tempCount < playerCount2.Z)// between
				EnableSpawners(true);
			else
				EnableSpawners(false);
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_Player_Count_Enable_Spawners::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == enableCustom)
	{
		enabled = param ? true : false;
	}
}
void JMG_Utility_Player_Count_Enable_Spawners::EnableSpawners(bool enable)
{
	if (conditionMatching == enable)
		return;
	conditionMatching = enable;
	int end = Get_Int_Parameter("EndID");
	for (int x = Get_Int_Parameter("StartID");x <= end;x++)
		Commands->Enable_Spawner(x,enable);
}
void JMG_Utility_AI_Engineer_Ignore_Target::Created(GameObject *obj)
{
}
void JMG_Utility_Give_Weapon_In_Range::Created(GameObject *obj)
{
	sprintf(weaponName,"%s",Get_Parameter("WeaponName"));
	location = Get_Vector3_Parameter("Location");
	if (JmgUtility::SimpleDistance(location,Vector3()) <= 0.0f)
		location = Commands->Get_Position(obj);
	range = Get_Float_Parameter("Range");
	range *= range;
	enableCustom = Get_Int_Parameter("EnableOrDisableCustom");
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	selectWeapon = Get_Int_Parameter("SelectWeapon") ? true : false;
	weaponAmmo = Get_Int_Parameter("WeaponAmmo");
	rate = Get_Float_Parameter("Rate");
	if (rate)
		Commands->Start_Timer(obj,this,rate,1);
	refillAmount = Get_Int_Parameter("RefillAmount");
	refillRate = Get_Float_Parameter("RefillRate");
	if (refillRate && refillAmount)
		Commands->Start_Timer(obj,this,refillRate,2);
}
void JMG_Utility_Give_Weapon_In_Range::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (enabled)
			GrantWeapon();
		Commands->Start_Timer(obj,this,rate,1);
	}
	if (number == 2)
	{
		if (enabled)
			for (int x = 1;x < 128;x++)
			{
				GameObject *player = Get_GameObj(x);
				if (!player || !Has_Weapon(player,weaponName) || JmgUtility::SimpleDistance(location,Commands->Get_Position(player)) > range)
					continue;
				int maxTotalBullets = Get_Max_Total_Bullets(player,weaponName);
				int totalBullets = Get_Total_Bullets(player,weaponName);
				if (totalBullets < maxTotalBullets)
				{
					int addBullets = min(maxTotalBullets-totalBullets,refillAmount);
					Set_Clip_Bullets(player,weaponName,Get_Clip_Bullets(player,weaponName)+addBullets);
				}
			}
		Commands->Start_Timer(obj,this,refillRate,2);
	}
}
void JMG_Utility_Give_Weapon_In_Range::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == enableCustom)
	{
		enabled = param ? true : false;
		if (enabled)
			GrantWeapon();
	}
}
void JMG_Utility_Give_Weapon_In_Range::GrantWeapon()
{
	for (int x = 1;x < 128;x++)
	{
		GameObject *player = Get_GameObj(x);
		if (!player || Has_Weapon(player,weaponName) || JmgUtility::SimpleDistance(location,Commands->Get_Position(player)) > range)
			continue;
		Grant_Weapon(player,weaponName,true,weaponAmmo,true);
		if (selectWeapon)
			Commands->Select_Weapon(player,weaponName);
	}
}
int JMG_Utility_AI_Follow_Player_On_Poke_Controller::followingPlayer[128] = {0};
int JMG_Utility_AI_Follow_Player_On_Poke_Controller::maxFollowers = 0;
bool JMG_Utility_AI_Follow_Player_On_Poke_Controller::controllerPlaced = false;
void JMG_Utility_AI_Follow_Player_On_Poke_Controller::Created(GameObject *obj)
{
	maxFollowers = Get_Int_Parameter("MaxFollowers");
	for (int x = 0;x < 128;x++)
		followingPlayer[x] = 0;
}
void JMG_Utility_AI_Follow_Player_On_Poke_Controller::Destroyed(GameObject *obj)
{
	JMG_Utility_AI_Follow_Player_On_Poke_Controller::controllerPlaced = false;
}
void JMG_Utility_AI_Follow_Player_On_Poke_Controller::Detach(GameObject *obj)
{
	Destroyed(obj);
}
void JMG_Utility_AI_Follow_Player_On_Poke::Created(GameObject *obj)
{
	if (!JMG_Utility_AI_Follow_Player_On_Poke_Controller::controllerPlaced)
	{
		Console_Input("msg JMG_Utility_AI_Follow_Player_On_Poke ERROR: JMG_Utility_AI_Follow_Player_On_Poke_Controller must be placed on the map!");
		this->Destroy_Script();
		return;
	}
	const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Current_Weapon(obj),true);
	if (ammo)
		weaponRange = ammo->Range;
	enemyId = 0;
	actionUpdate = 0;
	enemySeen = 0;
	hurt = false;
	lastTarget = NULL;
	lastFollow = NULL;
	lastLocation = homeLocation = Commands->Get_Position(obj);
	Commands->Set_Player_Type(obj,-2);
	followDistance = Get_Float_Parameter("FollowDistance");
	followNearSpeed = Get_Float_Parameter("FollowNearSpeed");
	followFarSpeed = Get_Float_Parameter("FollowFarSpeed");
	followFarDistance = Get_Float_Parameter("FollowFarDistance");
	followFarDistance *= followFarDistance;
	followVeryFarSpeed = Get_Float_Parameter("FollowVeryFarSpeed");
	followVeryFarDistance = Get_Float_Parameter("FollowVeryFarDistance");
	followVeryFarDistance *= followVeryFarDistance;
	runHomeSpeed = Get_Float_Parameter("RunHomeSpeed");
	fallbackWithoutArmor = Get_Int_Parameter("FallBackWhenOutOfArmor") ? true : false;
	healWhileAtHomeLocation = Get_Float_Parameter("HealWhileAtHomeLocation");
	sendCustomId = Get_Int_Parameter("SendCustomID");
	pokedCustom = Get_Int_Parameter("PokedCustom");
	lostCustom = Get_Int_Parameter("LostCustom");
	killedCustom = Get_Int_Parameter("KilledCustom");
	injuredCustom = Get_Int_Parameter("InjuredCustom");
	healedCustom = Get_Int_Parameter("HealedCustom");
	messageColor = Get_Vector3_Parameter("HUDMessagesColor");
	messagePokeFollowerId = Get_Int_Parameter("MessagePokeFollowerID");
	if (messagePokeFollowerId)
		sprintf(messagePokeFollower,"%s",Get_Translated_String(messagePokeFollowerId));
	messageFollowingYouId = Get_Int_Parameter("MessageFollowingYouID");
	messageFollowingPlayerId = Get_Int_Parameter("MessageFollowingPlayerID");
	messageMaxFollowersId = Get_Int_Parameter("MessageMaxFollowersID");
	if (messageMaxFollowersId)
		sprintf(messageMaxFollowers,"%s",Get_Translated_String(messageMaxFollowersId));
	messageHealingRequiredId = Get_Int_Parameter("MessageHealingRequiredID");
	messageFollowerLostId = Get_Int_Parameter("MessageFollowerLostID");
	messageFollowerInjuredId = Get_Int_Parameter("MessageFollowerInjuredID");
	messageFollowerKilledId = Get_Int_Parameter("MessageFollowerKilledID");
	lastPos = Commands->Get_Position(obj);
	maxFollowTime = Get_Int_Parameter("MaxFollowTime");
	maxIdleTime = Get_Int_Parameter("MaxIdleTime");
	Commands->Start_Timer(obj,this,0.1f,1);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void JMG_Utility_AI_Follow_Player_On_Poke::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (!Commands->Get_Health(seen))
		return;
	if (seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed() && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 900)
		return;
	if (enemyId == Commands->Get_ID(seen))
	{
		enemySeen = 30;
		return;
	}
	if (enemyId && JmgUtility::SimpleDistance(Commands->Get_Position(seen),Commands->Get_Position(obj)) > targetDistance)
		return;
	enemyId = Commands->Get_ID(seen);
	enemySeen = 30;
}
void JMG_Utility_AI_Follow_Player_On_Poke::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float speed = runHomeSpeed;
		float distance = followDistance;
		Vector3 myPos = Commands->Get_Position(obj);
		if (enemySeen)
		{
			enemySeen--;
			if (!enemySeen)
				ForgetEnemy(obj);
		}
		if (actionUpdate)
			actionUpdate--;
		GameObject *enemy = Commands->Find_Object(enemyId);
		if (enemy)
			targetDistance = JmgUtility::SimpleDistance(Commands->Get_Position(enemy),myPos);
		else if (enemyId)
			ForgetEnemy(obj);
		GameObject *player = Commands->Find_Object(playerObjectId);
		if (player)
		{
			bool quitFollowing = false;
			if (maxFollowTime)
			{
				currentFollowTime++;
				if (currentFollowTime == maxFollowTime)
					quitFollowing = true;
			}
			if (maxIdleTime && JmgUtility::SimpleDistance(myPos,lastPos) <= 0.0f)
			{
				currentIdleTime++;
				if (currentIdleTime == maxIdleTime)
					quitFollowing = true;
			}
			else
				currentIdleTime = 0;
			if (quitFollowing)
			{
				ShowPlayerMessage(player,messageFollowerLostId,messageColor);
				SendCustom(obj,lostCustom);
				GiveUpOnPlayer(obj,true);
				player = NULL;
			}
			lastPos = myPos;
		}
		if (pokerPlayerId && !player)
		{
			SendCustom(obj,lostCustom);
			GiveUpOnPlayer(obj,true);
		}
		else if (player)
		{
			float tempDist = JmgUtility::SimpleDistance(Commands->Get_Position(player),myPos);
			if (tempDist >= followVeryFarDistance)
				speed = followVeryFarSpeed;
			else if (tempDist >= followFarDistance)
				speed = followFarSpeed;
			else
				speed = followNearSpeed;
		}
		if (hurt || !player)
		{
			distance = 1.0f;
			speed = runHomeSpeed;
		}
		if (healWhileAtHomeLocation && Commands->Get_Shield_Strength(obj) < Commands->Get_Max_Shield_Strength(obj) && JmgUtility::SimpleDistance(myPos,homeLocation) <= 2.0f)
		{
			Commands->Apply_Damage(obj,-healWhileAtHomeLocation,"None",0);
			if (hurt && !pokerPlayerId && fallbackWithoutArmor && Commands->Get_Shield_Strength(obj) >= Commands->Get_Max_Shield_Strength(obj))
			{
				Commands->Enable_HUD_Pokable_Indicator(obj,true);
				SendCustom(obj,healedCustom);
				hurt = false;
			}
		}
		AttackTarget(obj,player,enemy,homeLocation,speed,distance);
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_AI_Follow_Player_On_Poke::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!hurt && fallbackWithoutArmor && Commands->Get_Shield_Strength(obj) <= 0.0f)
	{
		GameObject *player = Commands->Find_Object(playerObjectId);
		if (player)
			ShowPlayerMessage(player,messageFollowerInjuredId,messageColor);
		if (pokerPlayerId)
			SendCustom(obj,injuredCustom);
		GiveUpOnPlayer(obj,false);
		hurt = true;
	}
	if (hurt && Commands->Get_Shield_Strength(obj) >= Commands->Get_Max_Shield_Strength(obj))
	{
		Commands->Enable_HUD_Pokable_Indicator(obj,true);
		SendCustom(obj,healedCustom);
		hurt = false;
	}
	if (damage <= 0 || !damager || obj == damager || enemyId == Commands->Get_ID(damager) || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (enemyId && JmgUtility::SimpleDistance(Commands->Get_Position(damager),Commands->Get_Position(obj)) > targetDistance)
		return;
	enemyId = Commands->Get_ID(damager);
	enemySeen = 30;
}
void JMG_Utility_AI_Follow_Player_On_Poke::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		if (Commands->Get_Health(obj) <= 0)
			return;
		GameObject *player = Commands->Find_Object(playerObjectId);
		if (player)
			ShowPlayerMessage(player,messageFollowerLostId,messageColor);
		SendCustom(obj,lostCustom);
		GiveUpOnPlayer(obj,true);
	}
}
void JMG_Utility_AI_Follow_Player_On_Poke::Poked(GameObject *obj, GameObject *poker)
{
	int playerId = JmgUtility::JMG_Get_Player_ID(poker);
	if (pokerPlayerId)
	{
		if (pokerPlayerId == playerId)
			ShowPlayerMessage(poker,messageFollowingYouId,messageColor);
		else
			ShowPlayerMessage(poker,messageFollowingPlayerId,messageColor);
		return;
	}
	if (JMG_Utility_AI_Follow_Player_On_Poke_Controller::maxFollowers > 0 && JMG_Utility_AI_Follow_Player_On_Poke_Controller::followingPlayer[playerId] >= JMG_Utility_AI_Follow_Player_On_Poke_Controller::maxFollowers)
	{
		if (messageMaxFollowersId)
			Set_HUD_Help_Text_Player_Text(poker,7234,formatMaxFollowingString(messageMaxFollowers,JMG_Utility_AI_Follow_Player_On_Poke_Controller::maxFollowers,JMG_Utility_AI_Follow_Player_On_Poke_Controller::maxFollowers != 1 ? "s " : " "),messageColor);
		return;
	}
	if (hurt)
	{
		ShowPlayerMessage(poker,messageHealingRequiredId,messageColor);
		return;
	}
	currentFollowTime = 0;
	currentIdleTime = 0;
	SendCustom(obj,pokedCustom);
	pokerPlayerId = playerId;
	playerObjectId = Commands->Get_ID(poker);
	JMG_Utility_AI_Follow_Player_On_Poke_Controller::followingPlayer[playerId]++;
	if (messagePokeFollowerId)
		Set_HUD_Help_Text_Player_Text(poker,7234,formatMaxFollowingString(messagePokeFollower,JMG_Utility_AI_Follow_Player_On_Poke_Controller::followingPlayer[playerId],JMG_Utility_AI_Follow_Player_On_Poke_Controller::maxFollowers),messageColor);
	Commands->Set_Player_Type(obj,Commands->Get_Player_Type(poker));
	Commands->Enable_HUD_Pokable_Indicator(obj,false);
}
void JMG_Utility_AI_Follow_Player_On_Poke::Killed(GameObject *obj, GameObject *killer)
{
	Destroyed(obj);
}
void JMG_Utility_AI_Follow_Player_On_Poke::Destroyed(GameObject *obj)
{
	if (detached)
		return;
	detached = true;
	GameObject *player = Commands->Find_Object(playerObjectId);
	if (player)
		ShowPlayerMessage(player,messageFollowerKilledId,messageColor);
	JMG_Utility_AI_Follow_Player_On_Poke_Controller::followingPlayer[pokerPlayerId]--;
	pokerPlayerId = 0;
	SendCustom(obj,killedCustom);
}
void JMG_Utility_AI_Follow_Player_On_Poke::Detach(GameObject *obj)
{
	if (detached)
		return;
	detached = true;
	JMG_Utility_AI_Follow_Player_On_Poke_Controller::followingPlayer[pokerPlayerId]--;
	pokerPlayerId = 0;
}
void JMG_Utility_AI_Follow_Player_On_Poke::GiveUpOnPlayer(GameObject *obj,bool pokeable)
{
	JMG_Utility_AI_Follow_Player_On_Poke_Controller::followingPlayer[pokerPlayerId]--;
	pokerPlayerId = 0;
	playerObjectId = 0;
	Commands->Enable_HUD_Pokable_Indicator(obj,pokeable);
	Commands->Set_Player_Type(obj,-2);
}
void JMG_Utility_AI_Follow_Player_On_Poke::ForgetEnemy(GameObject *obj)
{
	enemyId = 0;
	Commands->Action_Reset(obj,100.0f);
}
void JMG_Utility_AI_Follow_Player_On_Poke::SendCustom(GameObject *obj,int custom)
{
	if (!custom)
		return;
	GameObject *controller = sendCustomId ? Commands->Find_Object(sendCustomId) : obj;
	if (controller)
		Commands->Send_Custom_Event(obj,controller,custom,0,0);
}
void JMG_Utility_AI_Follow_Player_On_Poke::AttackTarget(GameObject *obj,GameObject *follow,GameObject *target,Vector3 location,float speed,float distance)
{
	if (follow != lastFollow || lastTarget != target || JmgUtility::SimpleDistance(location,lastLocation) > 0.0f)
		actionUpdate = 0;
	if (actionUpdate)
		return;
	lastTarget = target;
	lastFollow = follow;
	lastLocation = location;
	actionUpdate = 15;
	ActionParamsStruct params;
	if (follow)
		params.Set_Movement(follow,speed,distance,false);
	else
		params.Set_Movement(location,speed,distance,false);
	params.MovePathfind = true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,100,100,AI_STATE_COMBAT);
	params.AttackCheckBlocked = true;
	if (target)
	{
		params.Set_Attack(target,weaponRange*0.95f,0.0f,true);
		Commands->Action_Attack(obj,params);
	}
}
char *JMG_Utility_AI_Follow_Player_On_Poke::formatMaxFollowingString(const char *format,...)
{
	static char displayMsg[256];
	va_list args;
	va_start(args,format);
	vsprintf(displayMsg,format,args);
	va_end(args);
	return displayMsg;
}
void JMG_Utility_AI_Follow_Player_On_Poke::ShowPlayerMessage(GameObject *player,int stringId,Vector3 color)
{
	if (!stringId)
		return;
	Set_HUD_Help_Text_Player(player,stringId,color);
}
void JMG_Utility_Timer_Damage_And_Teleport::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
}
void JMG_Utility_Timer_Damage_And_Teleport::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		GameObject *target = Commands->Find_Object(Get_Int_Parameter("TeleportToObjectID"));
		if (target)
		{
			Commands->Set_Position(obj,Commands->Get_Position(target));
			Commands->Set_Facing(obj,Commands->Get_Facing(target));
		}
		else
			Commands->Set_Position(obj,Get_Vector3_Parameter("Location"));
		Commands->Apply_Damage(obj,Get_Float_Parameter("DamageAmount"),Get_Parameter("DamageWarhead"),Get_Int_Parameter("DamageSelf") ? obj : NULL);
	}
}
void JMG_Utility_Zone_Send_Custom_If_Has_Script::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	if (!Is_Script_Attached(enter,Get_Parameter("ScriptName")))
		return;
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? (id == -1 ? enter : Commands->Find_Object(id)) : obj;
	if (object)
		Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
void JMG_Utility_Damage_When_Outside_Of_Range::Created(GameObject *obj)
{
	for (int x = 0;x < 128;x++)
	{
		warnTime[x] = 0;
		screenEffectOn[x] = false;
	}
	warnDistance = Get_Float_Parameter("Warn_Distance");
	warnDistance *= warnDistance;
	damageDistance = Get_Float_Parameter("Damage_Distance");
	damageDistance *= damageDistance;
	vehicleWarnDistance = Get_Float_Parameter("Vehicle_Warn_Distance");
	vehicleWarnDistance *= vehicleWarnDistance;
	vehicleDamageDistance = Get_Float_Parameter("Vehicle_Damage_Distance");
	vehicleDamageDistance *= vehicleDamageDistance;
	aircraftWarnDistance = Get_Float_Parameter("Aircraft_Warn_Distance");
	aircraftWarnDistance *= aircraftWarnDistance;
	aircraftDamageDistance = Get_Float_Parameter("Aircraft_Damage_Distance");
	aircraftDamageDistance *= aircraftDamageDistance;
	maxSurviveDistance = Get_Float_Parameter("Max_Survive_Distance");
	maxSurviveDistance *= maxSurviveDistance;
	leavingFieldStringId = Get_String_ID_By_Desc(Get_Parameter("Warning_StringID"));
	errorMessageColor = Get_Vector3_Parameter("Warning_Color");
	screenFadeColor = Get_Vector3_Parameter("Screen_Fade_Color");
	sprintf(damageWarhead,"%s",Get_Parameter("DamageWarhead"));
	sprintf(instantKillWarhead,"%s",Get_Parameter("InstantKillWarhead"));
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Damage_When_Outside_Of_Range::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1)
	{
		Vector3 myPos = Commands->Get_Position(obj);
		for (int x = 1;x < 128;x++)
		{
			if (warnTime[x])
				warnTime[x]--;
			GameObject *player = Get_GameObj(x);
			if (!player)
				continue;
			float dist = JmgUtility::SimpleDistance(Commands->Get_Position(player),myPos);
			GameObject *playerVehicle = Get_Vehicle(player);
			if (!playerVehicle)
			{
				if (dist > warnDistance)
				{
					screenEffectOn[x] = true;
					float TempDistCalc = min(1-(dist-maxSurviveDistance)/(warnDistance-maxSurviveDistance),1.0f);
					float inverted = 1-TempDistCalc;
					Set_Screen_Fade_Color_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].X*inverted+screenFadeColor.X,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Y*inverted+screenFadeColor.Y,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Z*inverted+screenFadeColor.Z,0.1f);
					Set_Screen_Fade_Opacity_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::opacity[x]*inverted+TempDistCalc,0.1f);
				}
				else if (screenEffectOn[x])
				{
					screenEffectOn[x] = false;
					Set_Screen_Fade_Color_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].X,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Y,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Z,0.1f);
					Set_Screen_Fade_Opacity_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::opacity[x],0.1f);
				}
				if (dist >= warnDistance && !warnTime[x])
				{
					Set_HUD_Help_Text_Player(player,leavingFieldStringId,errorMessageColor);
					warnTime[x] = 30;
				}
				if (dist >= damageDistance)
					Commands->Apply_Damage(player,0.25f,damageWarhead,player);
			}
			else
			{
				if (Is_VTOL(playerVehicle))
				{
					if (dist > aircraftWarnDistance)
					{
						screenEffectOn[x] = true;
						float TempDistCalc = min(1-(dist-maxSurviveDistance)/(aircraftWarnDistance-maxSurviveDistance),1.0f);
						float inverted = 1-TempDistCalc;
						Set_Screen_Fade_Color_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].X*inverted+screenFadeColor.X,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Y*inverted+screenFadeColor.Y,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Z*inverted+screenFadeColor.Z,0.1f);
						Set_Screen_Fade_Opacity_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::opacity[x]*inverted+TempDistCalc,0.1f);
					}
					else if (screenEffectOn[x])
					{
						screenEffectOn[x] = false;
						Set_Screen_Fade_Color_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].X,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Y,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Z,0.1f);
						Set_Screen_Fade_Opacity_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::opacity[x],0.1f);
					}
					if (dist >= aircraftWarnDistance && !warnTime[x])
					{
						Set_HUD_Help_Text_Player(player,leavingFieldStringId,errorMessageColor);
						warnTime[x] = 30;
					}
				}
				else
				{
					if (dist > vehicleWarnDistance)
					{
						screenEffectOn[x] = true;
						float TempDistCalc = min(1-(dist-maxSurviveDistance)/(vehicleWarnDistance-maxSurviveDistance),1.0f);
						float inverted = 1-TempDistCalc;
						Set_Screen_Fade_Color_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].X*inverted+screenFadeColor.X,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Y*inverted+screenFadeColor.Y,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Z*inverted+screenFadeColor.Z,0.1f);
						Set_Screen_Fade_Opacity_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::opacity[x]*inverted+TempDistCalc,0.1f);
					}
					else if (screenEffectOn[x])
					{
						screenEffectOn[x] = false;
						Set_Screen_Fade_Color_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].X,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Y,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Z,0.1f);
						Set_Screen_Fade_Opacity_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::opacity[x],0.1f);
					}
					if (dist >= vehicleWarnDistance && !warnTime[x])
					{
						Set_HUD_Help_Text_Player(player,leavingFieldStringId,errorMessageColor);
						warnTime[x] = 30;
					}
				}
				if (Is_VTOL(playerVehicle))
				{
					if (dist >= aircraftDamageDistance && Get_Vehicle_Driver(playerVehicle) == player)
						Commands->Apply_Damage(playerVehicle,1.0f,damageWarhead,player);
				}
				else if (dist >= vehicleDamageDistance && Get_Vehicle_Driver(playerVehicle) == player)
					Commands->Apply_Damage(playerVehicle,1.0f,damageWarhead,player);
			}
			if (dist >= maxSurviveDistance)
				if (playerVehicle)
					Commands->Apply_Damage(playerVehicle,99999.0f,instantKillWarhead,player);
				else
					Commands->Apply_Damage(player,99999.0f,instantKillWarhead,player);
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Send_Custom_When_No_More_Presets_Of_Type_Exist::Created(GameObject *obj)
{
	sprintf(presetName,"%s",Get_Parameter("PresetName"));
	enableCustom = Get_Int_Parameter("EnableOrDisableCustom");
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Send_Custom_When_No_More_Presets_Of_Type_Exist::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == enableCustom)
	{
		enabled = param ? true : false;
	}
}
void JMG_Utility_Send_Custom_When_No_More_Presets_Of_Type_Exist::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (enabled)
		{
			bool found = false;
			for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
			{
				GameObject* o = current->Data()->As_ScriptableGameObj();
				if (!_stricmp(Commands->Get_Preset_Name(o),presetName))
					found = true;
			}
			if (!found)
			{
				int id = Get_Int_Parameter("ID");
				GameObject *object = id ? Commands->Find_Object(id) : obj;
				if (object)
					Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
				this->Destroy_Script();
				return;
			}
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Send_Custom_When_Player_Outside_Of_Range::Created(GameObject *obj)
{
	sleeping = 0;
	distance = Get_Float_Parameter("Distance");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Custom");
	param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	distance *= distance;
	triggerOnce = Get_Int_Parameter("TriggerOnce") ? true : false;
	sleepTime = Get_Int_Parameter("SleepTime");
	maxRange = Get_Float_Parameter("MaxRange");
	maxRange *= maxRange;
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Send_Custom_When_Player_Outside_Of_Range::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1)
	{
		if (sleeping)
			sleeping--;
		else
			for (int x = 1;x < 128;x++)
			{
				GameObject *player = Get_GameObj(x);
				if (!player)
					continue;
				float playerDist = JmgUtility::SimpleDistance(Commands->Get_Position(player),Commands->Get_Position(obj));
				if (playerDist > distance && (!maxRange || playerDist <= maxRange))
				{
					GameObject *object = id ? Commands->Find_Object(id) : obj;
					if (object)
						Commands->Send_Custom_Event(obj,object,custom,param,delay);
					if (triggerOnce)
					{
						this->Destroy_Script();
						return;
					}
					sleeping = sleepTime;
				}
			}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Set_Team_To_Neutral_When_No_Armor::Created(GameObject *obj)
{
	hurt = Commands->Get_Shield_Strength(obj) < Commands->Get_Max_Shield_Strength(obj);
	originalPlayerType = Commands->Get_Player_Type(obj);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Set_Team_To_Neutral_When_No_Armor::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (hurt && Commands->Get_Shield_Strength(obj) >= Commands->Get_Max_Shield_Strength(obj))
		{
			hurt = false;
			Commands->Set_Player_Type(obj,originalPlayerType);
		}
		if (!hurt && !Commands->Get_Shield_Strength(obj))
		{
			hurt = true;
			Commands->Set_Player_Type(obj,-2);
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist::Created(GameObject *obj)
{
	debug = Get_Int_Parameter("Debug") ? true : false;
	team = Get_Int_Parameter("Team");
	enableCustom = Get_Int_Parameter("EnableOrDisableCustom");
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == enableCustom)
	{
		enabled = param ? true : false;
	}
}
void JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (enabled)
		{
			bool found = false;
			for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
			{
				GameObject* o = current->Data()->As_ScriptableGameObj();
				int preId = Commands->Get_Preset_ID(o);
				if (Commands->Get_Player_Type(o) == team && !Is_Script_Attached(o,"JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist_Ignore") && preId != 82080086 && preId != 82080122 && preId != 82080023)
				{
					if (debug)
					{
						char debugMsg[220];
						sprintf(debugMsg,"msg %s",Commands->Get_Preset_Name(o));
						Console_Input(debugMsg);
					}
					found = true;
					break;
				}
			}
			if (!found)
			{
				int id = Get_Int_Parameter("ID");
				GameObject *object = id ? Commands->Find_Object(id) : obj;
				if (object)
					Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
				this->Destroy_Script();
				return;
			}
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Custom_Create_Sound_At_Object_Bone::Created(GameObject *obj)
{
	sprintf(sound,"%s",Get_Parameter("Sound"));
	sprintf(bone,"%s",Get_Parameter("Bone"));
	custom = Get_Int_Parameter("Custom");
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Custom_Create_Sound_At_Object_Bone::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Create_3D_Sound_At_Bone(sound,obj,bone);
	}
}
void JMG_Utility_Custom_Send_Custom_On_Player_Count::Created(GameObject *obj)
{
	runningCount = 0;
	custom = Get_Int_Parameter("Custom");
	resetCustom = Get_Int_Parameter("ResetCustom");
	baseCount = Get_Int_Parameter("BaseCount");
	playerCount = Get_Int_Parameter("PlayerCount");
}
void JMG_Utility_Custom_Send_Custom_On_Player_Count::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == resetCustom)
	{
		runningCount = 0;
	}
	if (message == custom)
	{
		runningCount++;
		if (runningCount < baseCount+playerCount*Get_Player_Count())
			return;
		int id = Get_Int_Parameter("ID");
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
		if (object)
		{
			int Param = Get_Int_Parameter("Param");
			Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("SendCustom"),Param == -1 ? param : Param,Get_Float_Parameter("Delay"));
		}
		this->Destroy_Script();
	}
}
void JMG_Utility_Killed_By_Player_Send_Custom::Killed(GameObject *obj,GameObject *killer)
{
	if (!Commands->Is_A_Star(killer))
		return;
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? (id == -1 ? killer : Commands->Find_Object(id)) : obj;
	Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	Destroy_Script();
}
void JMG_Utility_AI_Guardian_Ignored::Created(GameObject *obj)
{
}
void JMG_Utility_AI_Vehicle_Ignored::Created(GameObject *obj)
{
}
void JMG_Utility_Killed_By_PresetID_Send_Custom::Killed(GameObject *obj,GameObject *killer)
{
	if (Commands->Get_Preset_ID(killer) != Get_Int_Parameter("PresetID"))
		return;
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? (id == -1 ? killer : Commands->Find_Object(id)) : obj;
	Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	Destroy_Script();
}
void JMG_Utility_Created_Override_AI_Soldier_Muzzle_Direction::Created(GameObject *obj)
{
	if (obj->As_SoldierGameObj())
		obj->As_SoldierGameObj()->Set_Override_Muzzle_Direction(true);
	this->Destroy_Script();
}
void JMG_Utility_Custom_Send_Custom_On_Player_Count_HUD::Created(GameObject *obj)
{
	enabled = true;
	hudStringId = Get_Int_Parameter("HudStringId");
	sprintf(hudMessage,"%s",Get_Translated_String(hudStringId));
	hudStringColor = Get_Vector3_Parameter("HudStringColor");
	runningCount = 0;
	custom = Get_Int_Parameter("Custom");
	resetCustom = Get_Int_Parameter("ResetCustom");
	baseCount = Get_Int_Parameter("BaseCount");
	playerCount = Get_Int_Parameter("PlayerCount");
}
void JMG_Utility_Custom_Send_Custom_On_Player_Count_HUD::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (!enabled)
		return;
	if (message == resetCustom)
	{
		runningCount = 0;
	}
	if (message == custom)
	{
		runningCount++;
		int requiredCount = baseCount+playerCount*Get_Player_Count();
		if (runningCount <= requiredCount)
			JmgUtility::SetHUDHelpText(hudStringId,formatHUDMessage(hudMessage,runningCount,requiredCount),hudStringColor);
		if (runningCount < requiredCount)
			return;
		int id = Get_Int_Parameter("ID");
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
		if (object)
		{
			int Param = Get_Int_Parameter("Param");
			Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("SendCustom"),Param == -1 ? param : Param,Get_Float_Parameter("Delay"));
		}
		enabled = false;
	}
}
char *JMG_Utility_Custom_Send_Custom_On_Player_Count_HUD::formatHUDMessage(const char *format,...)
{
	static char displayMsg[256];
	va_list args;
	va_start(args,format);
	vsprintf(displayMsg,format,args);
	va_end(args);
	return displayMsg;
}
void JMG_Utility_Zone_Send_Custom_If_Has_Weapon_Ammo::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	char weaponName[256];
	sprintf(weaponName,"%s",Get_Parameter("WeaponName"));
	if (!Has_Weapon(enter,weaponName))
		return;
	int totalBullets = Get_Total_Bullets(enter,weaponName);
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? (id == -1 ? enter : Commands->Find_Object(id)) : obj;
	int param = Get_Int_Parameter("Param");
	param = param == -1 ? Commands->Get_ID(enter) : param;
	if (object && totalBullets)
		for (int x = 0;x < totalBullets;x++)
			Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),param,Get_Float_Parameter("Delay"));
	if (Get_Int_Parameter("RemoveWeapon"))
		Remove_Weapon(enter,weaponName);
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
void JMG_Utility_Custom_Remove_Weapon_From_All_Players::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	sprintf(weaponName,"%s",Get_Parameter("WeaponName"));
}
void JMG_Utility_Custom_Remove_Weapon_From_All_Players::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player || !Has_Weapon(player,weaponName))
				continue;
			Remove_Weapon(player,weaponName);
		}
	}
}
void JMG_Utility_Custom_Destroy_All_Presets_By_Name::Created(GameObject *obj)
{
	sprintf(presetName,"%s",Get_Parameter("PresetName"));
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Destroy_All_Presets_By_Name::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
		{
			GameObject* o = current->Data()->As_ScriptableGameObj();
			if (!_stricmp(presetName,Commands->Get_Preset_Name(o)))
				Commands->Destroy_Object(o);
		}
	}
}
void JMG_Utility_Send_Custom_When_Damage_Exceedes_Threshold::Created(GameObject *obj)
{
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Custom");
	param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	damageThreshold = Get_Float_Parameter("DamageThreshold");
	lockoutTime = Get_Float_Parameter("LockoutTime");
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Send_Custom_When_Damage_Exceedes_Threshold::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (timeRemaining)
		{
			timeRemaining -= 0.1f;
			if (timeRemaining < 0)
				timeRemaining = 0.0f;
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Send_Custom_When_Damage_Exceedes_Threshold::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage < damageThreshold)
		return;
	if (timeRemaining)
		return;
	timeRemaining = lockoutTime;
	GameObject *object = id ? (id == -1 ? damager : Commands->Find_Object(id)) : obj;
	if (object)
		Commands->Send_Custom_Event(obj,object,custom,param,delay);
}
void JMG_Utility_Custom_Send_Translated_String_To_All_Players::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	stringId = Get_Int_Parameter("StringID");
	color = Get_Vector3_Parameter("ColorRGB");
	repeatable = Get_Int_Parameter("Repeatable") ? true : false;
}
void JMG_Utility_Custom_Send_Translated_String_To_All_Players::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		JmgUtility::MessageAllPlayers((int)color.X,(int)color.Y,(int)color.Z,Get_Translated_String(stringId));
		if (!repeatable)
			Destroy_Script();
	}
}
void JMG_Utility_Destroyed_Drop_Powerup::Destroyed(GameObject *obj)
{
	if (_stricmp(Get_Parameter("RequiredWeaponPreset"),"") && !Has_Weapon(obj,Get_Parameter("RequiredWeaponPreset")))
		return;
	Vector3 pos = Commands->Get_Position(obj);
	pos.Z += Get_Float_Parameter("HeightAdjust");
	GameObject *powerup = Commands->Create_Object(Get_Parameter("PowerupName"),pos);
	Commands->Set_Facing(powerup,Commands->Get_Facing(obj));
}
void JMG_Utility_Pickup_Attach_Script::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		const char *parameters = Get_Parameter("Params");
		char *params = newstr(parameters);
		char delim = Get_Parameter("Delim")[0];
		unsigned int length = strlen(parameters);
		for (unsigned int x = 0;x < length;x++)
			if (params[x] == delim)
				params[x] = ',';
		Commands->Attach_Script(sender,Get_Parameter("Script"),params);
	}
}
void JMG_Utility_Objective_System_Objective_Status_Update_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		if (!JMG_Utility_Objective_System_Controller::controllerPlaced)
		{
			Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Custom ERROR: Make sure you have a JMG_Utility_Objective_System_Controller placed on the map!");
			return;
		}
		int objectiveId = Get_Int_Parameter("ObjectiveID");
		int objectiveMarkerId = Get_Int_Parameter("NewObjectiveMarkerObjectID");
		GameObject *ObjectiveMarker = BasicObjectiveSystem.GetObjectiveMarker(objectiveMarkerId,sender,objectiveId);
		if (!ObjectiveMarker && objectiveMarkerId)
		{
			Console_Input("msg JMG_Utility_Objective_System_Objective_Status_Update_Custom ERROR: Could not find objective marker object!");
			return;
		}
		int stringId = Get_Int_Parameter("NewObjectiveStringID");
		BasicObjectiveSystem.Add_Objective(objectiveId,NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
		BasicObjectiveSystem.Set_Objective_Mission(objectiveId,stringId,stringId);
		if (ObjectiveMarker)
			BasicObjectiveSystem.Set_Radar_Blip(objectiveId,ObjectiveMarker,NULL);
		else if (!objectiveMarkerId)
			BasicObjectiveSystem.Remove_Radar_Blip(objectiveId);
		JmgUtility::SetHUDHelpText(stringId,BasicObjectiveSystem.Get_Hud_Help_Text_Color(objectiveId,(NewObjectiveSystem::Priority)BasicObjectiveSystem.Get_Objective_Priority(objectiveId)));
	}
}
void JMG_Utility_Objective_System_Objective_Failed_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Objective_System_Objective_Failed_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		if (BasicObjectiveSystem.Get_Objective_Status(Get_Int_Parameter("ObjectiveID")) != NewObjectiveSystem::Accomplished)
		{
			BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("ObjectiveID"),NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
			BasicObjectiveSystem.Set_Objective_Status(Get_Int_Parameter("ObjectiveID"),NewObjectiveSystem::Failed);
		}
	}
}
void JMG_Utility_Force_Player_Team_At_Gameover::Created(GameObject *obj)
{
	team = Get_Int_Parameter("Team");
	Commands->Start_Timer(obj,this,1.0,1);
}
void JMG_Utility_Force_Player_Team_At_Gameover::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (The_Game()->Is_Game_Over())
			for (int x = 1;x < 128;x++)
			{
				cPlayer *cplayer = Find_Player(x);
				if (!cplayer)
					continue;
				cplayer->Set_Player_Type(team);
			}
	}
	Commands->Start_Timer(obj,this,1.0,1);
}
void JMG_Utility_AI_Guardian_Generic::Created(GameObject *obj)
{
	flightHeight = Get_Float_Parameter("FlightHeight");
	primaryFire = Get_Int_Parameter("UseSecondaryAttack") ? false : true;
	arriveDistance = Get_Float_Parameter("ArriveDistance");
	arriveDistanceSq = (arriveDistance+1.0f)*(arriveDistance+1.0f);
	dpPosition = Commands->Get_Position(obj);
	EnemyID = 0;
	EnemyTimeOutTime = 0;
	LastPos = Commands->Get_Position(obj);
	stealthModeOverride = Get_Int_Parameter("StealthModeOverride");
	float weaponRange = 0.0f;
	const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Current_Weapon(obj),true);
	if (ammo)
		weaponRange = ammo->Range;
	fireRange = Get_Float_Parameter("FireRange") >= 0 ? Get_Float_Parameter("FireRange") : weaponRange;
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Enable_Engine(obj,true);
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_AI_Guardian_Generic::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (EnemyTimeOutTime)
		{
			EnemyTimeOutTime--;
			if (!EnemyTimeOutTime)
			{
				EnemyID = 0;
				Goto_Location(obj);
			}
		}
		if (EnemyID)
		{
			GameObject *Target = Commands->Find_Object(EnemyID);
			if (!Target || !Commands->Get_Health(Target) || (Commands->Get_Player_Type(obj) != -2 && Commands->Get_Player_Type(Target) == Commands->Get_Player_Type(obj)))
			{
				EnemyID = 0;
				Goto_Location(obj);
			}
		}
		Vector3 Pos = Commands->Get_Position(obj);
		Pos.Z -= flightHeight;
		if (JmgUtility::SimpleDistance(Pos,LastPos) < arriveDistanceSq)
		{
			if (Get_A_Defense_Point(&dpPosition))
				Goto_Location(obj);
		}
		LastPos = Pos;
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_AI_Guardian_Generic::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (!Commands->Get_Health(seen))
		return;
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Guardian_Ignored"))
		return;
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Guardian_Generic_Ignored"))
		return;
	if (!JmgUtility::CanSeeStealth(stealthModeOverride,obj,seen))
		return;
	if (!EnemyID || !EnemyTimeOutTime)
	{
		EnemyID = Commands->Get_ID(seen);
		EnemyTimeOutTime = Commands->Get_Random_Int(2,5);
		Goto_Location(obj);
	}
	else if (EnemyID == Commands->Get_ID(seen))
		EnemyTimeOutTime = Commands->Get_Random_Int(2,5);
}
void JMG_Utility_AI_Guardian_Generic::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0 || (EnemyID && EnemyTimeOutTime) || (Commands->Get_Player_Type(obj) != -2 && Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj)))
		return;
	EnemyID = Commands->Get_ID(damager);
	EnemyTimeOutTime = Commands->Get_Random_Int(2,5);
	Goto_Location(obj);
}
void JMG_Utility_AI_Guardian_Generic::Goto_Location(GameObject *obj)
{
	ActionParamsStruct params;
	Commands->Action_Reset(obj,100);
	Vector3 GoToPos = dpPosition;
	if (flightHeight)
		GoToPos.Z += flightHeight;
	params.MovePathfind = flightHeight ? false : true;
	params.Set_Movement(GoToPos,Get_Float_Parameter("WanderSpeed"),arriveDistance,false);
	params.ShutdownEngineOnArrival = Get_Int_Parameter("TurnOffEngineOnArrival") ? true : false;
	params.Set_Basic(this,100,10);
	GameObject *Target = Commands->Find_Object(EnemyID);
	if (Target)
	{
		if (Get_Int_Parameter("FaceTarget"))
			params.Set_Face_Location(Commands->Get_Position(Target),5.0f);
		else
			params.AttackFaceTarget = false;
		params.Set_Attack(Target,fireRange,0,primaryFire);
		params.AttackCheckBlocked = Get_Int_Parameter("CheckBlocked") ? true : false;
		Commands->Action_Attack(obj,params);
	}
	else
	{
		if (Get_Int_Parameter("FaceTarget"))
			params.Set_Face_Location(GoToPos,5.0f);
		else
			params.AttackFaceTarget = false;
		Commands->Action_Goto(obj,params);
	}
}
void JMG_Utility_Custom_Create_Object_In_Front_Of_Self::Created(GameObject *obj)
{
	sprintf(presetName,"%s",Get_Parameter("PresetName"));
	custom = Get_Int_Parameter("Custom");
	distance = Get_Float_Parameter("Distance");
	height = Get_Float_Parameter("Height");
	rotation = Get_Float_Parameter("Rotation");
}
void JMG_Utility_Custom_Create_Object_In_Front_Of_Self::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Vector3 pos = Commands->Get_Position(obj);
		float facing = Commands->Get_Facing(obj);
		pos.X += cos(facing*PI180)*distance;
		pos.Y += sin(facing*PI180)*distance;
		facing += rotation;
		pos.Z += height;
		GameObject *object = Commands->Create_Object(presetName,pos);
		Commands->Set_Facing(object,facing);
	}
}
void JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist_Ignore::Created(GameObject *obj)
{
}
void JMG_Utility_Custom_Damage_All_Vehicles_On_Team::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	team = Get_Int_Parameter("Team");
	damage = Get_Float_Parameter("Damage");
	sprintf(warhead,"%s",Get_Parameter("Warhead"));
}
void JMG_Utility_Custom_Damage_All_Vehicles_On_Team::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj* o = current->Data();
			if (o->As_VehicleGameObj() && !CheckPlayerType(o,team))
				Commands->Apply_Damage(o,damage,warhead,o);
		}
	}
}
void JMG_Utility_Kill_Unit_If_Not_Moving_Enough::Created(GameObject *obj)
{
	resetTime = time = (int)(Get_Float_Parameter("Time")*10);
	distance = Get_Float_Parameter("Distance");
	distance *= distance;
	lastPos = Commands->Get_Position(obj);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Kill_Unit_If_Not_Moving_Enough::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),lastPos) < distance)
		{
			time--;
			if (!time)
				Commands->Apply_Damage(obj,999999.9f,"BlamoKiller",obj);
		}
		else
		{
			lastPos = Commands->Get_Position(obj);
			time = resetTime;
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Custom_Display_Briefing_Message_StringID::Created(GameObject *obj)
{
	triggered = false;
	BriefingText = NULL;
	CurrentNode = NULL;
	AddNewTextNode();
	char TextString[16384];
	sprintf(TextString,"%s",Get_Translated_String(Get_String_ID_By_Desc(Get_Parameter("StringID"))));
	int Count = 0;
	for (int x = 0;x < 16384;x++)
	{
		if ((Count > 150 && TextString[x] == ' ') || Count == 210)
		{// String getting too long, start a new one.
			CurrentNode->Text[Count] = '\0';
			AddNewTextNode();
			Count = 0;
		}
		else if (TextString[x] == '\n' || TextString[x] == '\0')
		{
			CurrentNode->Text[Count] = '\0';
			break;// end of line your done
		}
		else if (TextString[x] == '\f')
		{
			CurrentNode->Text[Count] = '\0';
			AddNewTextNode();
			Count = 0;
		}
		else
		{
			CurrentNode->Text[Count] = TextString[x];
			Count++;
		}
	}
	JMG_Utility_Custom_Display_Briefing_Message::voiceId = 0;
	CurrentNode = BriefingText;
}
void JMG_Utility_Custom_Display_Briefing_Message_StringID::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!CurrentNode)
		{
			RemoveTextNodes();
			Destroy_Script();
			return;
		}
		if (CurrentNode->Text[0] != '\0')
			for (int x = 1;x < 128;x++)
			{
				GameObject *Player = Get_GameObj(x);
				if (Player)
					JmgUtility::DisplayChatMessage(Player,(int)Get_Vector3_Parameter("MessageColor").X,(int)Get_Vector3_Parameter("MessageColor").Y,(int)Get_Vector3_Parameter("MessageColor").Z,CurrentNode->Text);
			}
		CurrentNode = CurrentNode->next;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("LineDelayTime"),1);
	}
}
void JMG_Utility_Custom_Display_Briefing_Message_StringID::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		if (triggered)
			return;
		triggered = true;
		Commands->Start_Timer(obj,this,0.0f,1);
		if (JMG_Utility_Custom_Display_Briefing_Message::voiceId)
			Commands->Stop_Sound(JMG_Utility_Custom_Display_Briefing_Message::voiceId,true);
		JMG_Utility_Custom_Display_Briefing_Message::voiceId = Commands->Create_2D_Sound(Get_Parameter("UISound"));
	}
}
void JMG_Utility_Custom_Display_Briefing_Message_StringID::Destroyed(GameObject *obj)
{
	RemoveTextNodes();
}
void JMG_Utility_Custom_Display_Briefing_Message_StringID::AddNewTextNode()
{
	if (!BriefingText)
	{
		BriefingText = new BriefingTextNode;
		sprintf(BriefingText->Text,"\0");
		BriefingText->next = NULL;
		CurrentNode = BriefingText;
		return;
	}
	BriefingTextNode *Current = BriefingText;
	while (Current)
	{
		if (!Current->next)
		{
			Current->next = new BriefingTextNode;
			Current = Current->next;
			sprintf(Current->Text,"\0");
			Current->next = NULL;
			CurrentNode = Current;
			return;
		}
		Current = Current->next;
	}
}
void JMG_Utility_Custom_Display_Briefing_Message_StringID::RemoveTextNodes()
{
	BriefingTextNode *Current = BriefingText,*Temp;
	while (Current)
	{
		Temp = Current;
		Current = Current->next;
		delete Temp;
	}
	BriefingText = NULL;
	CurrentNode = NULL;
}
void JMG_Utility_Destroy_Objects_In_ID_Range_On_Killed::Killed(GameObject *obj,GameObject *killer)
{
	int end = Get_Int_Parameter("EndID");
	for (int x = Get_Int_Parameter("StartID");x <= end;x++)
	{
		GameObject *destroy = Commands->Find_Object(x);
		if (destroy)
			Commands->Destroy_Object(destroy);
	}
}
void JMG_Utility_Timer_Custom::Created(GameObject *obj)
{
	time = Get_Float_Parameter("Time");
	repeat = Get_Int_Parameter("Repeat") ? true : false;
	id = Get_Int_Parameter("ID");
	message = Get_Int_Parameter("Message");
	param = Get_Int_Parameter("Param");
	enableCustom = Get_Int_Parameter("EnableCustom");
	if (!enableCustom)
		Commands->Start_Timer(obj,this,time,1);
}
void JMG_Utility_Timer_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		GameObject *object = !id ? obj : Commands->Find_Object(id);
		if (object)
			Commands->Send_Custom_Event(obj,object,message,param,0);
		if (repeat)
			Commands->Start_Timer(obj,this,time,1);
	}
}
void JMG_Utility_Timer_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (enableCustom && enableCustom == message)
	{
		if (param)
			Commands->Start_Timer(obj,this,time,1);
		else
			Stop_Timer(obj,1);
	}
}
void JMG_Utility_Zone_Change_Character_Model::Created(GameObject *obj)
{
	playerType = Get_Int_Parameter("PlayerType");
	sprintf(newModel,"%s",Get_Parameter("NewModel"));
}
void JMG_Utility_Zone_Change_Character_Model::Entered(GameObject *obj,GameObject *enterer)
{
	if (CheckPlayerType(enterer,playerType) || Commands->Get_Player_Type(enterer) == -4)
		return;
	Commands->Set_Model(enterer,newModel);
}
void JMG_Utility_Timer_Change_Character_Models_On_Team::Created(GameObject *obj)
{
	time = Get_Float_Parameter("Time");
	repeat = Get_Int_Parameter("Repeat") ? true : false;
	playerType = Get_Int_Parameter("PlayerType");
	sprintf(newModel,"%s",Get_Parameter("NewModel"));
	enableCustom = Get_Int_Parameter("EnableCustom");
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	if (enabled)
		Commands->Start_Timer(obj,this,time,1);
}
void JMG_Utility_Timer_Change_Character_Models_On_Team::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player || CheckPlayerType(player,playerType) || Commands->Get_Player_Type(player) == -4 || !_stricmp(Get_Model(player),newModel))
				continue;
			Commands->Set_Model(player,newModel);
		}
		if (repeat)
			Commands->Start_Timer(obj,this,time,1);
	}
}
void JMG_Utility_Timer_Change_Character_Models_On_Team::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (enableCustom == message)
	{
		bool tEnabled = param ? true : false;
		if (enabled == tEnabled)
			return;
		enabled = tEnabled;
		if (!enabled)
			Stop_Timer(obj,1);
		else
			Commands->Start_Timer(obj,this,time,1);
	}
}
void JMG_Utility_Timer_Change_Character_Models_On_HP::Created(GameObject *obj)
{
	time = Get_Float_Parameter("Time");
	repeat = Get_Int_Parameter("Repeat") ? true : false;
	playerType = Get_Int_Parameter("PlayerType");
	sprintf(newModel,"%s",Get_Parameter("NewModel"));
	enableCustom = Get_Int_Parameter("EnableCustom");
	minHp = Get_Float_Parameter("MinHP");
	maxHp = Get_Float_Parameter("MaxHP");
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	if (enabled)
		Commands->Start_Timer(obj,this,time,1);
}
void JMG_Utility_Timer_Change_Character_Models_On_HP::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			float hp = Get_Max_Hitpoints(player);
			if (!player || CheckPlayerType(player,playerType) || Commands->Get_Player_Type(player) == -4 || hp < minHp || hp > maxHp || !_stricmp(Get_Model(player),newModel))
				continue;
			Commands->Set_Model(player,newModel);
		}
		if (repeat)
			Commands->Start_Timer(obj,this,time,1);
	}
}
void JMG_Utility_Timer_Change_Character_Models_On_HP::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (enableCustom == message)
	{
		bool tEnabled = param ? true : false;
		if (enabled == tEnabled)
			return;
		enabled = tEnabled;
		if (!enabled)
			Stop_Timer(obj,1);
		else
			Commands->Start_Timer(obj,this,time,1);
	}
}
void JMG_Utility_Destroy_Send_Custom::Destroyed(GameObject *obj)
{
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? Commands->Find_Object(id) : obj;
	if (object)
		Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Message"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
}
void JMG_Utility_Set_Vehicle_Collisions_Ghost_When_Unoccupied::Created(GameObject *obj)
{
	if (!obj->As_PhysicalGameObj() || !obj->As_VehicleGameObj())
	{
		Console_Input("msg JMG_Utility_Set_Vehicle_Collisions_Ghost_When_Unoccupied ERROR: This script is only for vehicles!");
		Destroy_Script();
		return;
	}
	ghost = false;
	attemptingCollidable = false;
	myCollisionGroup = obj->As_PhysicalGameObj()->Peek_Physical_Object()->Get_Collision_Group();
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_Set_Vehicle_Collisions_Ghost_When_Unoccupied::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (ghost && attemptingCollidable)
		{
			MoveablePhysClass *mphys = obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass();
			obj->As_PhysicalGameObj()->Peek_Physical_Object()->Set_Collision_Group(myCollisionGroup);
			if (mphys->Can_Teleport(Matrix3D(Commands->Get_Position(obj))))
			{
				ghost = false;
				attemptingCollidable = false;
			}
			else
				obj->As_PhysicalGameObj()->Peek_Physical_Object()->Set_Collision_Group(Collision_Group_Type::SOLDIER_GHOST_COLLISION_GROUP);
		}
		Commands->Start_Timer(obj,this,0.25f,1);
	}
}
void JMG_Utility_Set_Vehicle_Collisions_Ghost_When_Unoccupied::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (ghost && !attemptingCollidable)
			attemptingCollidable = true;
	}
	if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (!Get_Vehicle_Occupant_Count(obj) && !ghost)
		{
			ghost = true;
			attemptingCollidable = false;
			myCollisionGroup = obj->As_PhysicalGameObj()->Peek_Physical_Object()->Get_Collision_Group();
			obj->As_PhysicalGameObj()->Peek_Physical_Object()->Set_Collision_Group(Collision_Group_Type::SOLDIER_GHOST_COLLISION_GROUP);
		}
	}
}
void JMG_Utility_Attach_Script_To_All_Players::Created(GameObject *obj)
{
	sprintf(scriptName,"%s",Get_Parameter("Script"));
	sprintf(params,"%s",Get_Parameter("Params"));
	char delim = Get_Parameter("Delim")[0];
	unsigned int length = strlen(params);
	for (unsigned int x = 0;x < length;x++)
		if (params[x] == delim)
			params[x] = ',';
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Attach_Script_To_All_Players::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player || Is_Script_Attached(player,scriptName))
				continue;
			Commands->Attach_Script(player,scriptName,params);
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Send_Custom_On_Powerup_Pickup::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		int id = Get_Int_Parameter("ID");
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
		if (object)
			Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Message"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	}
}
void JMG_Utility_Set_Bullets_On_Custom_Or_Damage::Created(GameObject *obj)
{
	sprintf(setWeapon,"%s",Get_Parameter("WeaponName"));
	if (!_stricmp(setWeapon,"null"))
		sprintf(setWeapon,"%s",Get_Current_Weapon(obj));
	repeat = Get_Int_Parameter("Repeat") ? true : false;
	custom = Get_Int_Parameter("Custom");
	triggerOnDamage = Get_Int_Parameter("TriggerOnDamage") ? true : false;
	fullClip = Get_Int_Parameter("FullClip") ? true : false;
	setBackpackBullets = Get_Int_Parameter("SetBackpackBullets");
	if (Get_Int_Parameter("StartWithNoBullets"))
	{
		Set_Bullets(obj,setWeapon,0);
		Set_Clip_Bullets(obj,setWeapon,0);
	}
}
void JMG_Utility_Set_Bullets_On_Custom_Or_Damage::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0.0f || !triggerOnDamage)
		return;
	UpdateBullets(obj);
}
void JMG_Utility_Set_Bullets_On_Custom_Or_Damage::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		UpdateBullets(obj);
	}
}
void JMG_Utility_Set_Bullets_On_Custom_Or_Damage::UpdateBullets(GameObject *obj)
{
	if (Has_Weapon(obj,setWeapon))
	{
		if (fullClip)
			Set_Bullets(obj,setWeapon,Get_Max_Bullets(obj,setWeapon));
		Set_Clip_Bullets(obj,setWeapon,setBackpackBullets);
	}
	if (!repeat)
		Destroy_Script();
}
void JMG_Utility_Custom_Damage_All_Presets::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	sprintf(preset,"%s",Get_Parameter("PresetName"));
	damage = Get_Float_Parameter("Damage");
	sprintf(warhead,"%s",Get_Parameter("Warhead"));
}
void JMG_Utility_Custom_Damage_All_Presets::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj* o = current->Data();
			if (!_stricmp(Commands->Get_Preset_Name(o),preset))
				Commands->Apply_Damage(o,damage,warhead,o);
		}
	}
}
void JMG_Utility_Death_Weapon_Create_Object::Killed(GameObject *obj,GameObject *killer)
{
	const char *weap = Get_Current_Weapon(killer);
	if (!weap || _stricmp(weap,Get_Parameter("WeaponPreset")))
		return;
	if (Commands->Get_Random(0.0f,1.0f) > Get_Float_Parameter("Chance"))
		return;
	GameObject *spawned = Commands->Create_Object(Get_Parameter("Preset"),Commands->Get_Position(obj));
	Commands->Set_Facing(spawned,Commands->Get_Facing(obj));
	Commands->Set_Model(obj,"null");
}
void JMG_Utility_Send_Custom_When_Speed_Exceeds_Amount::Created(GameObject *obj)
{
	speed = Get_Float_Parameter("Speed");
	speed *= speed;
	rate = Get_Float_Parameter("Rate");
	repeat = Get_Int_Parameter("Repeat") ? true : false;
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Message");
	paramx = Get_Int_Parameter("Param");
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	enableCustom = Get_Int_Parameter("EnableCustom");
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Send_Custom_When_Speed_Exceeds_Amount::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == enableCustom)
	{
		enabled = param ? true : false;
	}
}
void JMG_Utility_Send_Custom_When_Speed_Exceeds_Amount::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (enabled && !obj->As_PhysicalGameObj()->Is_Attached_To_An_Object())
		{
			Vector3 tempSpeed;
			if (obj->As_SoldierGameObj())
				obj->As_SoldierGameObj()->Get_Velocity(tempSpeed);
			else if (obj->As_VehicleGameObj())
				obj->As_VehicleGameObj()->Get_Velocity(tempSpeed);
			if (tempSpeed.Length2() >= speed)
			{
				GameObject *object = !id ? obj : Commands->Find_Object(id);
				if (object)
					Commands->Send_Custom_Event(obj,object,custom,paramx,0);
				if (!repeat)
				{
					Destroy_Script();
					return;
				}
			}
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Send_Custom_When_Speed_Below_Amount::Created(GameObject *obj)
{
	speed = Get_Float_Parameter("Speed");
	speed *= speed;
	rate = Get_Float_Parameter("Rate");
	repeat = Get_Int_Parameter("Repeat") ? true : false;
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Message");
	paramx = Get_Int_Parameter("Param");
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	enableCustom = Get_Int_Parameter("EnableCustom");
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Send_Custom_When_Speed_Below_Amount::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == enableCustom)
	{
		enabled = param ? true : false;
	}
}
void JMG_Utility_Send_Custom_When_Speed_Below_Amount::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (enabled && !obj->As_PhysicalGameObj()->Is_Attached_To_An_Object())
		{
			Vector3 tempSpeed;
			if (obj->As_SoldierGameObj())
				obj->As_SoldierGameObj()->Get_Velocity(tempSpeed);
			else if (obj->As_VehicleGameObj())
				obj->As_VehicleGameObj()->Get_Velocity(tempSpeed);
			if (tempSpeed.Length2() < speed)
			{
				GameObject *object = !id ? obj : Commands->Find_Object(id);
				if (object)
					Commands->Send_Custom_Event(obj,object,custom,paramx,0);
				if (!repeat)
				{
					Destroy_Script();
					return;
				}
			}
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Send_Custom_When_Velocity_Exceeds_Amount::Created(GameObject *obj)
{
	velocityFBL = Get_Vector3_Parameter("Velocity[Forward|Backward|Left]");
	velocityFBL = SquareVectorSpecial(velocityFBL);
	velocityRUD = Get_Vector3_Parameter("Velocity[Right|Up|Down]");
	velocityRUD = SquareVectorSpecial(velocityRUD);
	onlyTriggerOn = (SpecialCondition)Get_Int_Parameter("OnlyTriggerOn");
	onlyTriggerOnMinHV = Get_Vector3_Parameter("OnlyTriggerOnMin[Horizontal|Vertical]");
	onlyTriggerOnMinHV = SquareVectorSpecial(onlyTriggerOnMinHV);
	onlyTriggerOnMaxHV = Get_Vector3_Parameter("OnlyTriggerOnMax[Horizontal|Vertical]");
	onlyTriggerOnMaxHV = SquareVectorSpecial(onlyTriggerOnMaxHV);
	rate = Get_Float_Parameter("Rate");
	repeat = Get_Int_Parameter("Repeat") ? true : false;
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Message");
	paramx = Get_Int_Parameter("Param");
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	enableCustom = Get_Int_Parameter("EnableCustom");
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Send_Custom_When_Velocity_Exceeds_Amount::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == enableCustom)
	{
		enabled = param ? true : false;
	}
}
void JMG_Utility_Send_Custom_When_Velocity_Exceeds_Amount::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (enabled && !obj->As_PhysicalGameObj()->Is_Attached_To_An_Object())
		{
			Vector3 tempSpeed;
			if (obj->As_SoldierGameObj())
				obj->As_SoldierGameObj()->Get_Velocity(tempSpeed);
			else if (obj->As_VehicleGameObj())
				obj->As_VehicleGameObj()->Get_Velocity(tempSpeed);
			float speedHorizontal,speedVertical = tempSpeed.Z*tempSpeed.Z;
			tempSpeed.Z = 0;
			speedHorizontal = tempSpeed.Length2();

			SpecialCondition specialCondition = DOES_NOT_MEET_CONDITION;
			if (onlyTriggerOn && OnlyTriggerOnTest(speedHorizontal,speedVertical))
				if (onlyTriggerOn == HORIZANTAL_IS_GREATER && speedHorizontal > speedVertical)
					specialCondition = HORIZANTAL_IS_GREATER;
				else if (onlyTriggerOn == VERTICAL_IS_GREATER && speedHorizontal < speedVertical)
					specialCondition = VERTICAL_IS_GREATER;
			PhysControllerClass *controller = obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass()->Get_Controller();
			if (!onlyTriggerOn || (onlyTriggerOn == HORIZANTAL_IS_GREATER &&  specialCondition == onlyTriggerOn))
				if (controller->Get_Move_Forward() > 0 && velocityFBL.X >= 0 && tempSpeed.Length2() > velocityFBL.X)
					SendCustom(obj,1);
				else if (controller->Get_Move_Forward() < 0 && velocityFBL.Y >= 0 && tempSpeed.Length2() > velocityFBL.Y)
					SendCustom(obj,2);
				else if (controller->Get_Move_Left() > 0 && velocityFBL.Z >= 0 && tempSpeed.Length2() > velocityFBL.Z)
					SendCustom(obj,3);
				else if (controller->Get_Move_Left() < 0 && velocityRUD.X >= 0 && tempSpeed.Length2() > velocityRUD.X)
					SendCustom(obj,4);
			if (!onlyTriggerOn || (onlyTriggerOn == VERTICAL_IS_GREATER &&  specialCondition == onlyTriggerOn))
				if (controller->Get_Move_Up() > 0 && velocityRUD.Y >= 0 && speedVertical > velocityRUD.Y)
					SendCustom(obj,5);
				else if (controller->Get_Move_Up() < 0 && velocityRUD.Z >= 0 && speedVertical > velocityRUD.Z)
					SendCustom(obj,6);
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Send_Custom_When_Velocity_Exceeds_Amount::SendCustom(GameObject *obj,int paramOverride)
{
	GameObject *object = !id ? obj : Commands->Find_Object(id);
	if (object)
		Commands->Send_Custom_Event(obj,object,custom,paramx == -1 ? paramOverride : paramx,0);
	if (!repeat)
		Destroy_Script();
}
Vector3 JMG_Utility_Send_Custom_When_Velocity_Exceeds_Amount::SquareVectorSpecial(Vector3 in)
{
	if (in.X > 0)
		in.X *= in.X;
	if (in.Y > 0)
		in.Y *= in.Y;
	if (in.Z > 0)
		in.Z *= in.Z;
	return in;
}
bool JMG_Utility_Send_Custom_When_Velocity_Exceeds_Amount::OnlyTriggerOnTest(float horizontalSpeed,float verticalSpeed)
{
	if (onlyTriggerOnMinHV.X >= 0.0 && horizontalSpeed < onlyTriggerOnMinHV.X)
		return false;
	if (onlyTriggerOnMinHV.Y >= 0.0 && verticalSpeed < onlyTriggerOnMinHV.Y)
		return false;
	if (onlyTriggerOnMaxHV.X >= 0.0 && horizontalSpeed > onlyTriggerOnMaxHV.X)
		return false;
	if (onlyTriggerOnMaxHV.Y >= 0.0 && verticalSpeed > onlyTriggerOnMaxHV.Y)
		return false;
	return true;
}
void JMG_Utility_Custom_Change_Character::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Change_Character::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (custom == message)
	{
		if (!sender->As_SoldierGameObj())
			return;
		const char *preset = Get_Parameter("Preset");
		if (!Get_Int_Parameter("AllowRepurchase") && !_stricmp(Commands->Get_Preset_Name(sender),preset))
		{
			Commands->Create_Sound(Get_Parameter("FailSound"),Commands->Get_Position(sender),sender);
			return;
		}
		float cost = Get_Float_Parameter("Cost");
		if (cost <= Commands->Get_Money(sender))
		{
			Commands->Give_Money(sender,-cost,0);
			Change_Character(sender,preset);
			obj->As_SoldierGameObj()->Set_Collision_Group(7);
			Commands->Create_Sound(Get_Parameter("SuccessSound"),Commands->Get_Position(sender),sender);
		}
		else
			Commands->Create_Sound(Get_Parameter("FailSound"),Commands->Get_Position(sender),sender);
	}
}
void JMG_Utility_Apply_Damage_While_In_Zone::Created(GameObject *obj)
{
	team = Get_Int_Parameter("Team");
	int damagerId = Get_Int_Parameter("DamagerId");
	sprintf(params,"%.2f,%.2f,%s,%d",Get_Float_Parameter("Rate"),Get_Float_Parameter("DamageAmount"),Get_Parameter("Warhead"),damagerId == -2 ? Commands->Get_ID(obj) : damagerId);
}
void JMG_Utility_Apply_Damage_While_In_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (CheckPlayerType(enterer,team))
		return;
	Attach_Script_Once(enterer,"JMG_Utility_Apply_Damage_While_In_Zone_Attached",params);
}
void JMG_Utility_Apply_Damage_While_In_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	Remove_Script(exiter,"JMG_Utility_Apply_Damage_While_In_Zone_Attached");
}
void JMG_Utility_Apply_Damage_On_Timer_Base::Created(GameObject *obj)
{
	rate = Get_Float_Parameter("Rate");
	damageAmount = Get_Float_Parameter("DamageAmount");
	sprintf(warhead,"%s",Get_Parameter("Warhead"));
	damagerId = Get_Int_Parameter("DamagerId");
	Commands->Start_Timer(obj,this,0.0f,1);
}
void JMG_Utility_Apply_Damage_On_Timer_Base::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Apply_Damage(obj,damageAmount,warhead,damagerId ? (damagerId == -1 ? obj : Commands->Find_Object(damagerId)) : 0);
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Zone_Send_Custom_Exit::Created(GameObject *obj)
{
	playerType = Get_Int_Parameter("PlayerType");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Custom");
	param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	triggerOnce = Get_Int_Parameter("TriggerOnce") ? true : false;
}
void JMG_Utility_Zone_Send_Custom_Exit::Exited(GameObject *obj,GameObject *exiter)
{
	if (CheckPlayerType(exiter,playerType) || Commands->Get_Player_Type(exiter) == -4)
		return;
	GameObject *object = id ? (id == -1 ? exiter : Commands->Find_Object(id)) : obj;
	if (object)
		Commands->Send_Custom_Event(obj,object,custom,param,delay);
	if (triggerOnce)
		Destroy_Script();
}
void JMG_Utility_Custom_Enable_Spawners_In_Range_Modular::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		int mod = Get_Int_Parameter("Mod");
		if (mod == -1)
			mod = param;
		int end = Get_Int_Parameter("EndID");
		bool enable = Get_Int_Parameter("Enable") == -1 ? (param ? true : false) : (Get_Int_Parameter("Enable") ? true : false);
		for (int x = Get_Int_Parameter("StartID");x <= end;x++)
			if (!(x % mod))
				Commands->Enable_Spawner(x,enable);
	}
}
void JMG_Utility_Custom_Create_Explosion_At_Bone::Created(GameObject *obj)
{
	sprintf(explosion,"%s",Get_Parameter("Explosion"));
	sprintf(bone,"%s",Get_Parameter("Bone"));
	owner = Get_Int_Parameter("Owner");
	custom = Get_Int_Parameter("Custom");
	alive = Get_Int_Parameter("Alive") ? true : false;
}
void JMG_Utility_Custom_Create_Explosion_At_Bone::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		GameObject *exploder = !owner ? obj : (owner == -1 ? sender : Commands->Find_Object(owner));
		if (alive && (!exploder || !Commands->Get_Health(exploder)))
			return;
		Commands->Create_Explosion_At_Bone(explosion,obj,bone,exploder); 
	}
}
void JMG_Utility_Custom_Send_Custom_Repeat_Ignore_Time::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	sendCustom = Get_Int_Parameter("SendCustom");
	params = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	id = Get_Int_Parameter("ID");
	ignoreTime = (int)(Get_Float_Parameter("IgnoreTime")*1000.0f);
	lastTriggerTime = clock()-ignoreTime;
	enable = Get_Int_Parameter("StartsEnabled") ? true : false;
	enableCustom = Get_Int_Parameter("EnableCustom");
}
void JMG_Utility_Custom_Send_Custom_Repeat_Ignore_Time::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom && enable)
	{
		time_t currentTime = clock();
		if (difftime(currentTime,lastTriggerTime) < ignoreTime)
			return;
		lastTriggerTime = currentTime;
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
		if (object)
			Commands->Send_Custom_Event(obj,object,sendCustom,params == -1 ? param : params,delay);
	}
	if (message == enableCustom)
	{
		enable = param ? true : false;
		if (enable)
			lastTriggerTime = clock()-ignoreTime;
	}
}
void JMG_Utility_Damage_Send_Custom::Created(GameObject *obj)
{
	minDamage = Get_Float_Parameter("MinDamage");
	custom = Get_Int_Parameter("Custom");
	params = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	id = Get_Int_Parameter("ID");
	senderId = Get_Int_Parameter("SenderID");
	enable = Get_Int_Parameter("StartsEnabled") ? true : false;
	enableCustom = Get_Int_Parameter("EnableCustom");
	repeat = Get_Int_Parameter("Repeat") ? true : false;
}
void JMG_Utility_Damage_Send_Custom::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= minDamage || !enable)
		return;
	GameObject *object = id ? (id == -1 ? damager : Commands->Find_Object(id)) : obj;
	GameObject *sender = senderId ? (senderId == -1 ? damager : Commands->Find_Object(senderId)) : obj;
	if (object)
		Commands->Send_Custom_Event(sender,object,custom,params,delay);
	if (!repeat)
		Destroy_Script();
}
void JMG_Utility_Damage_Send_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == enableCustom)
	{
		enable = param ? true : false;
	}
}
void JMG_Utility_Send_Custom_When_Player_Inside_Of_Range::Created(GameObject *obj)
{
	sleeping = 0;
	distance = Get_Float_Parameter("Distance");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Custom");
	param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	distance *= distance;
	triggerOnce = Get_Int_Parameter("TriggerOnce") ? true : false;
	sleepTime = Get_Int_Parameter("SleepTime");
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Send_Custom_When_Player_Inside_Of_Range::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1)
	{
		if (sleeping)
			sleeping--;
		else
			for (int x = 1;x < 128;x++)
			{
				GameObject *player = Get_GameObj(x);
				if (!player)
					continue;
				float playerDist = JmgUtility::SimpleDistance(Commands->Get_Position(player),Commands->Get_Position(obj));
				if (playerDist <= distance)
				{
					GameObject *object = id ? Commands->Find_Object(id) : obj;
					if (object)
						Commands->Send_Custom_Event(obj,object,custom,param,delay);
					if (triggerOnce)
					{
						this->Destroy_Script();
						return;
					}
					sleeping = sleepTime;
				}
			}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Custom_Enable_Spawners_In_Range_ModPlayer::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		int mod = Get_Int_Parameter("Mod");
		if (mod == -1)
			mod = param;
		int playerMulti = Get_Int_Parameter("PlayerMulti");
		int newMod = JmgUtility::MathClampInt(mod-Get_Player_Count()*playerMulti,1,mod);
		bool invert = Get_Int_Parameter("Invert") ? true : false;
		int end = Get_Int_Parameter("EndID");
		bool enable = Get_Int_Parameter("Enable") == -1 ? (param ? true : false) : (Get_Int_Parameter("Enable") ? true : false);
		for (int x = Get_Int_Parameter("StartID");x <= end;x++)
			if (!(x % newMod))
				Commands->Enable_Spawner(x,enable);
			else if (invert)
				Commands->Enable_Spawner(x,!enable);
	}
}
bool JMG_Utility_Spawn_With_Last_Selected_Gun_Control::controllerPlaced = false;
char JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerWeapons[128][256];
char JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerNames[128][256];
SList<JMG_Utility_Spawn_With_Last_Selected_Gun_Control::StringNode> JMG_Utility_Spawn_With_Last_Selected_Gun_Control::ignoredWeapons;
void JMG_Utility_Spawn_With_Last_Selected_Gun_Control::Created(GameObject *obj)
{
	for (int x = 0;x < 128;x++)
	{
		sprintf(playerWeapons[x],"");
		sprintf(playerNames[x],"");
	}
	controllerPlaced = true;
}
void JMG_Utility_Spawn_With_Last_Selected_Gun_Control::Destroyed(GameObject *obj)
{
	controllerPlaced = false;
}
void JMG_Utility_Spawn_With_Last_Selected_Gun_Control::Detach(GameObject *obj)
{
	Destroyed(obj);
}
void JMG_Utility_Spawn_With_Last_Selected_Gun_Player::Created(GameObject *obj)
{
	if (!Commands->Is_A_Star(obj))
		Destroy_Script();
	if (!JMG_Utility_Spawn_With_Last_Selected_Gun_Control::controllerPlaced)
	{
		Commands->Start_Timer(obj,this,0.1f,2);
		return;
	}
	InitialSetup(obj);
}
void JMG_Utility_Spawn_With_Last_Selected_Gun_Player::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1)
	{
		GrantPlayersWeapon(obj);
	}
	if (number == 2)
	{
		if (!JMG_Utility_Spawn_With_Last_Selected_Gun_Control::controllerPlaced)
		{
			Console_Input("msg JMG_Utility_Spawn_With_Last_Selected_Gun_Player ERROR: JMG_Utility_Spawn_With_Last_Selected_Gun_Control must be placed on the map!");
			Destroy_Script();
			return;
		}
		InitialSetup(obj);
	}
}
void JMG_Utility_Spawn_With_Last_Selected_Gun_Player::Destroyed(GameObject *obj)
{
	const char *weapon = Get_Current_Weapon(obj);
	if (!weapon)
		return;
	for (SLNode<JMG_Utility_Spawn_With_Last_Selected_Gun_Control::StringNode> *node = JMG_Utility_Spawn_With_Last_Selected_Gun_Control::ignoredWeapons.Head();node;node = node->Next())
		if (node->Data() && !_stricmp(node->Data()->preset,weapon))
			return;
	sprintf(JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerWeapons[playerId],"%s",weapon);
}
void JMG_Utility_Spawn_With_Last_Selected_Gun_Player::GrantPlayersWeapon(GameObject *obj)
{
	if (!_stricmp(JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerWeapons[playerId],""))
		return;
	if (!Has_Weapon(obj,JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerWeapons[playerId]) && Get_Int_Parameter("GrantWeapon"))
		Grant_Weapon(obj,JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerWeapons[playerId],true,99999,true);
	const char *weap = Get_Current_Weapon(obj);
	if ((!weap || _stricmp(weap,JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerWeapons[playerId])) && Has_Weapon(obj,JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerWeapons[playerId]))
		Commands->Select_Weapon(obj,JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerWeapons[playerId]);
}
void JMG_Utility_Spawn_With_Last_Selected_Gun_Player::InitialSetup(GameObject *obj)
{
	playerId = JmgUtility::JMG_Get_Player_ID(obj);
	const char *playerName = Get_Player_Name(obj);
	if (_stricmp(JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerNames[playerId],playerName))
	{
		sprintf(JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerNames[playerId],"%s",playerName);
		sprintf(JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerWeapons[playerId],"");
	}
	float delay = Get_Float_Parameter("GrantDelay");
	if (!delay)
		GrantPlayersWeapon(obj);
	else
		Commands->Start_Timer(obj,this,delay,1);
}
void JMG_Utility_Created_Give_Weapon::Created(GameObject *obj)
{
	const char *weaponName = Get_Parameter("WeaponName");
	if (weaponName && _stricmp(weaponName,""))
		Grant_Weapon(obj,Get_Parameter("WeaponName"),Get_Int_Parameter("GrantWeapon") ? true : false,Get_Int_Parameter("GrantRounds"),Get_Int_Parameter("GrantClips") ? true : false);
}
void JMG_Utility_Credit_Trickle_To_Ammount::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	team = Get_Int_Parameter("Team");
	trickleCap = Get_Float_Parameter("TrickleCap");
	credits = Get_Float_Parameter("Credits");
	rate = Get_Float_Parameter("Rate");
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Credit_Trickle_To_Ammount::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player || CheckPlayerType(player,team))
				continue;
			if (!trickleCap || Commands->Get_Money(player) < trickleCap)
				Commands->Give_Money(player,credits,false);
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Credit_Trickle_To_Ammount::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		trickleCap = (float)param;
	}
}
void JMG_Utility_Custom_Damage_Objects_On_Team::Created(GameObject *obj)
{
	damage = Get_Float_Parameter("Damage");
	custom = Get_Int_Parameter("Custom");
	team = Get_Int_Parameter("Team");
	soldierVehicleOther = Get_Vector3_Parameter("Damage[Soldiers|Vehicles|Other]");
	sprintf(warhead,"%s",Get_Parameter("Warhead"));
	theDamager = Get_Int_Parameter("Damager");
	triggerOnce = Get_Int_Parameter("TriggerOnce") ? true : false;
}
void JMG_Utility_Custom_Damage_Objects_On_Team::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		GameObject *damager = theDamager ? (theDamager > 0 ? Commands->Find_Object(theDamager) : sender) : obj;
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj* o = current->Data();
			if (o && !CheckPlayerType(o,team) && (soldierVehicleOther.X && o->As_SoldierGameObj()) || (soldierVehicleOther.Y && o->As_VehicleGameObj()) || (soldierVehicleOther.Z && !o->As_SoldierGameObj() && !o->As_VehicleGameObj()))
				Commands->Apply_Damage(o,damage,warhead,damager);
		}
		if (triggerOnce)
			Destroy_Script();
	}
}
void JMG_Utility_Custom_Set_Animation::Created(GameObject *obj)
{
	objectId = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Custom");
	sprintf(animation,"%s",Get_Parameter("Animation"));
	looping = Get_Int_Parameter("Looping") ? true : false;
	startFrame = Get_Float_Parameter("StartFrame");
	endFrame = Get_Float_Parameter("EndFrame");
	blended = Get_Int_Parameter("Blended") ? true : false;
	triggerOnce = Get_Int_Parameter("TriggerOnce") ? true : false;
}
void JMG_Utility_Custom_Set_Animation::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		GameObject *target = objectId ? (objectId == -1 ? sender : Commands->Find_Object(objectId)) : obj;
		if (target)
			Commands->Set_Animation(target,animation,looping,0,startFrame,endFrame,blended);
		if (triggerOnce)
			Destroy_Script();
	}
}
void JMG_Utility_Objective_System_Objective_Marker_Update_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		if (!JMG_Utility_Objective_System_Controller::controllerPlaced)
		{
			Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Custom ERROR: Make sure you have a JMG_Utility_Objective_System_Controller placed on the map!");
			return;
		}
		int objectiveId = Get_Int_Parameter("ObjectiveID");
		int objectiveMarkerId = Get_Int_Parameter("NewObjectiveMarkerObjectID");
		GameObject *ObjectiveMarker = BasicObjectiveSystem.GetObjectiveMarker(objectiveMarkerId,sender,objectiveId);
		if (!ObjectiveMarker && objectiveMarkerId)
		{
			Console_Input("msg JMG_Utility_Objective_System_Objective_Marker_Update_Custom ERROR: Could not find objective marker object!");
			return;
		}
		BasicObjectiveSystem.Add_Objective(objectiveId,NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
		if (ObjectiveMarker)
			BasicObjectiveSystem.Set_Radar_Blip(objectiveId,ObjectiveMarker,NULL);
	}
}
void JMG_Utility_Send_Custom_On_Deaths_Reporter_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	JMG_Utility_Send_Custom_On_Deaths_Controller::ReportLogic(enterer);
}
void JMG_Utility_Killed_Give_Money::Killed(GameObject *obj,GameObject *killer)
{
	if (!Commands->Is_A_Star(killer))
		return;
	Commands->Give_Money(killer,Get_Float_Parameter("Money"),false);
}
void JMG_Utility_Created_Set_Damage_And_Death_Points::Created(GameObject *obj)
{
	float points = Get_Float_Parameter("DeathPoints");
	if (points != -1)
		Set_Death_Points(obj,points);
	points = Get_Float_Parameter("Damage_Points");
	if (points != -1)
		Set_Damage_Points(obj,points);
}
bool JMG_Utility_Detect_AFK_Controller::controllerPlaced = false;
bool JMG_Utility_Detect_AFK_Controller::isAFK[128] = {false};
void JMG_Utility_Detect_AFK_Controller::Created(GameObject *obj)
{
	for (int x = 1;x < 128;x++)
	{
		isMoving[x] = false;
		isTurning[x] = false;
		isFiring[x] = false;
		isMovingTarget[x] = false;
		lastTargetPos[x] = Vector3();
		afkTime[x] = 0;
		facing[x] = 0.0f;
		isAFK[x] = false;
	}
	afkThreshold = Get_Int_Parameter("AFKThreshold")*10;
	controllerPlaced = true;
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Detect_AFK_Controller::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player)
				continue;
			bool firing = false, moving = false, turning = false,movingTarget = false;
			Vector3 speed;
			GameObject *controlled = Get_Vehicle(player) ? Get_Vehicle(player) : player;
			SmartGameObj *smartGameObj = controlled->As_SmartGameObj();
			if (smartGameObj)
			{
				if (smartGameObj->Get_Weapon_Bag()->Get_Index())
					firing = smartGameObj->Get_Weapon_Bag()->Get_Weapon()->Is_Triggered();
				Vector3 tempAim = smartGameObj->Get_Targeting_Pos();
				if (JmgUtility::SimpleDistance(tempAim,lastTargetPos[x]) > 0.1f)
					movingTarget = true;
				lastTargetPos[x] = tempAim;
				PhysicalGameObj *physicalGameObj = controlled->As_PhysicalGameObj();
				PhysControllerClass *controller = physicalGameObj ? physicalGameObj->Peek_Physical_Object()->As_MoveablePhysClass()->Get_Controller() : NULL;
				if (controller)
				{
					moving = controller->Get_Move_Forward() || controller->Get_Move_Left() || controller->Get_Move_Up() ? true : false;
					turning = abs(controller->Get_Turn_Left()) >= 1.0? true : false;
				}
				else
				{
					smartGameObj->Get_Velocity(speed);
					moving = speed.Length2() > 0.1f ? true : false;
					float tempFacing = abs(Commands->Get_Facing(controlled));
					if (abs(tempFacing - facing[x]) > 1.5f)
						turning = true;
					facing[x] = tempFacing;
				}
			}
			if (moving == isMoving[x] && turning == isTurning[x] && firing == isFiring[x] && movingTarget == isMovingTarget[x])
			{
				if (afkTime[x] < afkThreshold)
				{
					afkTime[x]++;
					if (afkTime[x] == afkThreshold)
						isAFK[x] = true;
				}
			}
			else
			{
				afkTime[x] = 0;
				isAFK[x] = false;
			}
			isMoving[x] = moving;
			isTurning[x] = turning;
			isFiring[x] = firing;
			isMovingTarget[x] = movingTarget;
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Detect_AFK_Controller::Destroyed(GameObject *obj)
{
	controllerPlaced = false;
}
void JMG_Utility_Detect_AFK_Controller::Detach(GameObject *obj)
{
	Destroyed(obj);
}
void JMG_Utility_Credit_Trickle_When_Not_AFK::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	team = Get_Int_Parameter("Team");
	trickleCap = Get_Float_Parameter("TrickleCap");
	credits = Get_Float_Parameter("Credits");
	rate = Get_Float_Parameter("Rate");
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Credit_Trickle_When_Not_AFK::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!JMG_Utility_Detect_AFK_Controller::controllerPlaced)
		{
			Console_Input("msg JMG_Utility_Credit_Trickle_When_Not_AFK ERROR: JMG_Utility_Detect_AFK_Controller must be placed on the map!");
			Destroy_Script();
		}
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player || CheckPlayerType(player,team) || JMG_Utility_Detect_AFK_Controller::isAFK[x])
				continue;
			if (!trickleCap || Commands->Get_Money(player) < trickleCap)
				Commands->Give_Money(player,credits,false);
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Credit_Trickle_When_Not_AFK::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		trickleCap = (float)param;
	}
}
void JMG_Utility_Killed_Create_Object::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *newObject = Commands->Create_Object(Get_Parameter("Preset"),Commands->Get_Position(obj));
	Commands->Set_Facing(newObject,Commands->Get_Facing(obj));
	Commands->Destroy_Object(obj);
}
void JMG_Utility_Damaged_Create_Object_When_Shield_Zero::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (Commands->Get_Shield_Strength(obj))
		return;
	GameObject *newObject = Commands->Create_Object(Get_Parameter("Preset"),Commands->Get_Position(obj));
	Commands->Set_Facing(newObject,Commands->Get_Facing(obj));
	Commands->Destroy_Object(obj);
}
SList<JMG_Utility_Basic_Spawner_In_Radius::SpawnObjectNode> JMG_Utility_Basic_Spawner_In_Radius::spawnObjectNodeList;
void JMG_Utility_Basic_Spawner_In_Radius::Created(GameObject *obj)
{
	attachScriptsGroupId = Get_Int_Parameter("Attach_Scripts_Group_ID");
	spawnedObjectScriptID = Get_Int_Parameter("Spawned_Object_Script_ID");
	if (spawnedObjectScriptID == -1)
		spawnedObjectScriptID = this->Get_ID();
	spawnedObjects = 0;
	spawnLocation = Get_Vector3_Parameter("Spawn_Location");
	if (spawnLocation.X == -1.0f && spawnLocation.Y == -1.0f)
	{
		Vector3 mypos = Commands->Get_Position(obj);
		spawnLocation.X = mypos.X;
		spawnLocation.Y = mypos.Y;
		if (spawnLocation.Z == -1.0f)
			spawnLocation.Z = mypos.Z;
	}
	spawnAtATime = Get_Int_Parameter("Spawn_At_A_Time");
	maxTotalSpawned = Get_Int_Parameter("Max_Total_Spawned");
	minRadius = Get_Float_Parameter("Min_Spawn_Radius");
	maxRadius = Get_Float_Parameter("Max_Spawn_Radius");
	initialSpawnHeight = Get_Float_Parameter("Initial_Spawn_Height");
	xMultiplier = Get_Float_Parameter("X_Multiplier");
	yMultiplier = Get_Float_Parameter("Y_Multiplier");
	rate = Get_Float_Parameter("Spawn_Rate");
	randomRate = Get_Float_Parameter("Random_Spawn_Rate");
	collisionCheck = Get_Int_Parameter("Collision_Check")?true:false;
	retryAttempts = Get_Int_Parameter("Collision_Retry_Attempts");
	addHeight = Get_Float_Parameter("Collision_Add_Height");
	changeSpawnCapCustom = Get_Int_Parameter("Change_Spawn_Cap_Custom");
	sprintf(preset,"%s",Get_Parameter("Spawn_Preset"));
	spawnCount = 0;
	initialSpawn = Get_Int_Parameter("Initial_Spawn");
	if (initialSpawn == -1)
		initialSpawn = spawnAtATime;
	pointMustBeInPathfind = Get_Int_Parameter("Point_Must_Be_In_Pathfind")?true:false;
	manualFacing = Get_Int_Parameter("Manual_Facing")?true:false;
	faceLocation = Get_Vector3_Parameter("Face_Location");
	faceDirection = Get_Float_Parameter("Face_Direction");
	ignoreRayCastFailure = Get_Int_Parameter("Ignore_Ray_Cast_Failure")?true:false;
	minDistanceBetweenObjects = Get_Float_Parameter("Min_Distance_Between_Objects");
	minDistanceBetweenObjects *= minDistanceBetweenObjects;
	spawnGroupId = Get_Int_Parameter("Spawn_Group_ID");
	enabled = Get_Int_Parameter("Starts_Disabled") == 0 ? true : false;
	enableDisableCustom = Get_Int_Parameter("Enable_Disable_Custom");
	raycastRange = Get_Vector3_Parameter("Raycast_Direction_Override[Bottom|Top|Unused]");
	playersAddToSpawnAtATime = Get_Float_Parameter("PlayersAddToSpawnAtATime");
	if (enabled)
		Initial_Spawn(obj);
	Commands->Start_Timer(obj,this,rate+(randomRate ? Commands->Get_Random(-randomRate,randomRate) : 0.0f),1);
	lastPlayerCount = 0;
	Commands->Start_Timer(obj,this,2.5f,3);
}
void JMG_Utility_Basic_Spawner_In_Radius::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && !The_Game()->Is_Game_Over())
	{
		if (enabled)
		{
			JMG_Utility_Basic_Spawner_In_Radius::SpawnFailureTypes spawnResult = AttemptSpawn(obj);
			if (spawnResult == SpawnFailureTypes::SUCCESS)
				Commands->Start_Timer(obj,this,rate+(randomRate ? Commands->Get_Random(-randomRate,randomRate) : 0.0f),1);
			else if (spawnResult == SpawnFailureTypes::SPAWN_BLOCKED)
				Commands->Start_Timer(obj,this,0.1f,1);
		}
		else
			Commands->Start_Timer(obj,this,0.1f,1);
	}
	if (number == 3 && !The_Game()->Is_Game_Over())
	{
		int change = GetPlayerLimitModifier()-lastPlayerCount;
		lastPlayerCount = change;
		for (int x = 0;x < change;x++)
			Commands->Send_Custom_Event(obj,obj,6873525,spawnedObjectScriptID,0.1f);
		Commands->Start_Timer(obj,this,2.5f,3);
	}
}
void JMG_Utility_Basic_Spawner_In_Radius::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (changeSpawnCapCustom && message == changeSpawnCapCustom)
	{
		int change = param-spawnAtATime;
		spawnAtATime = param;
		for (int x = 0;x < change;x++)
			Commands->Send_Custom_Event(obj,obj,6873525,spawnedObjectScriptID,0.1f);
	}
	if (message == 6873523 && param == spawnedObjectScriptID)
	{
		spawnedObjects++;
	}
	if (message == 6873524 && param == spawnedObjectScriptID)
	{
		if (spawnedObjects > 0)
			spawnedObjects--;
		Commands->Send_Custom_Event(obj,obj,6873525,spawnedObjectScriptID,rate+(randomRate ? Commands->Get_Random(-randomRate,randomRate) : 0.0f));
	}
	if (message == 6873525 && param == spawnedObjectScriptID && !The_Game()->Is_Game_Over())
	{
		if (!enabled)
			return;
		if (AttemptSpawn(obj) == SpawnFailureTypes::SPAWN_BLOCKED)
			Commands->Send_Custom_Event(obj,obj,6873525,spawnedObjectScriptID,0.1f);
	}
	if (enableDisableCustom && message == enableDisableCustom && !The_Game()->Is_Game_Over())
	{
		enabled = param != 0 ? true : false;
		if (param == 1)
			Initial_Spawn(obj);
	}
}
JMG_Utility_Basic_Spawner_In_Radius::SpawnFailureTypes JMG_Utility_Basic_Spawner_In_Radius::AttemptSpawn(GameObject *obj)
{
	if (!(spawnedObjects < spawnAtATime+GetPlayerLimitModifier()))
		return SpawnFailureTypes::LIMIT_REACHED;
	if (!((maxTotalSpawned >= 0 && spawnCount < maxTotalSpawned) || maxTotalSpawned < 0))
		return SpawnFailureTypes::LIMIT_REACHED;
	float distance = (minRadius < maxRadius ? Commands->Get_Random(minRadius,maxRadius) : minRadius);
	float facing = Commands->Get_Random(-180.0f,180.0f);
	Vector3 topRay = spawnLocation,bottomRay;
	topRay.X = spawnLocation.X+cos(facing)*(distance*xMultiplier);
	topRay.Y = spawnLocation.Y+sin(facing)*(distance*yMultiplier);
	bottomRay = topRay;
	topRay.Z = spawnLocation.Z;
	Vector3 bottom,top;
	PhysicsSceneClass::Get_Instance()->Get_Level_Extents(bottom,top);
	if (raycastRange.X != 0.0f && raycastRange.Y != 0.0f)
	{
		bottomRay.Z = raycastRange.X;
		topRay.Z = raycastRange.Y;
	}
	else if (spawnLocation.Z < bottom.Z)
		bottomRay.Z = top.Z+1.0f;
	else
		bottomRay.Z = bottom.Z-1.0f;
	CastResultStruct res;
	res.ComputeContactPoint = true;
	LineSegClass ray(topRay,bottomRay);
	PhysRayCollisionTestClass coltest(ray, &res, TERRAIN_ONLY_COLLISION_GROUP,COLLISION_TYPE_PHYSICAL);
	PhysicsSceneClass::Get_Instance()->Cast_Ray(coltest,false);
	if (coltest.CollidedRenderObj && Vector3::Dot_Product(res.Normal,ray.Get_Dir()) <= 0)
	{
		bottomRay.Z = res.ContactPoint.Z+initialSpawnHeight;
		Vector3 unneeded;
		if (pointMustBeInPathfind && !Get_Random_Pathfind_Spot(bottomRay,0.0f,&unneeded))
			return SpawnFailureTypes::SPAWN_BLOCKED;
		if (CheckIfObjectIsNearAnotherObject(bottomRay))
			return SpawnFailureTypes::SPAWN_BLOCKED;
		GameObject *spawned = Commands->Create_Object(preset,bottomRay);
		if (!spawned)
		{
			char errorMsg[220];
			sprintf(errorMsg,"msg JMG_Utility_Basic_Spawner_In_Radius_Attached ERROR: Preset %s not found!",preset);
			Console_Input(errorMsg);
			return SpawnFailureTypes::SPAWN_CODE_ERROR;
		}
		if (manualFacing)
		{
			if (JmgUtility::SimpleDistance(Vector3(0.0f,0.0f,0.0f),faceLocation) > 0.0f)
			{
				Vector3 facePos = faceLocation;
				facePos.X -= bottomRay.X;
				facePos.Y -= bottomRay.Y;
				SetFacing(spawned,atan2(facePos.Y,facePos.X)*(180.0f/PI));
			}
			else
				SetFacing(spawned,faceDirection);
		}
		else
			SetFacing(spawned,Commands->Get_Random(-180.0f,180.0f));
		if (collisionCheck)
		{
			for (int x = 0;x <= retryAttempts;x++)
			{
				MoveablePhysClass *mphys = spawned->As_PhysicalGameObj() ? spawned->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : nullptr;
				if (!mphys)
				{
					Console_Input("msg JMG_Utility_Basic_Spawner_In_Radius ERROR: Collision check is turned on but the created object isn't a PhysicalGameObj!");
					Destroy_Script();
					return SpawnFailureTypes::SPAWN_CODE_ERROR;
				}
				if (!mphys->Can_Teleport(Matrix3D(bottomRay)) || (pointMustBeInPathfind && !Get_Random_Pathfind_Spot(bottomRay,0.0f,&unneeded)))
				{
					if (x >= retryAttempts)
					{
						Commands->Destroy_Object(spawned);
						return SpawnFailureTypes::SPAWN_BLOCKED;
					}
					bottomRay.Z += addHeight;
				}
			}
			Commands->Set_Position(spawned,bottomRay);
		}
		spawnCount++;
		char params[512];
		sprintf(params,"%d,%d,%d",Commands->Get_ID(obj),spawnedObjectScriptID,spawnGroupId);
		Commands->Attach_Script(spawned,"JMG_Utility_Basic_Spawner_In_Radius_Attached",params);
		Commands->Send_Custom_Event(spawned,obj,6873522,attachScriptsGroupId,0.0f);
		return SpawnFailureTypes::SUCCESS;
	}
	if (!ignoreRayCastFailure)
	{
		Console_Input("msg JMG_Utility_Basic_Spawner_In_Radius_Attached ERROR: Failed to cast Ray!");
		return SpawnFailureTypes::SPAWN_CODE_ERROR;
	}
	return SpawnFailureTypes::SPAWN_BLOCKED;
}
void JMG_Utility_Basic_Spawner_In_Radius::Initial_Spawn(GameObject *obj)
{
	int addSpawn = GetPlayerLimitModifier();
	lastPlayerCount = addSpawn;
	for (int x = spawnedObjects;x < initialSpawn+addSpawn;x++)
	{
		JMG_Utility_Basic_Spawner_In_Radius::SpawnFailureTypes spawnResult;
		for (int y = 0;y < 5;y++)
		{
			 spawnResult = AttemptSpawn(obj);
			if (spawnResult == SpawnFailureTypes::SUCCESS)
				break;
			if (spawnResult == SpawnFailureTypes::LIMIT_REACHED)
				x = initialSpawn+1+addSpawn;
		}
		if (spawnResult == SpawnFailureTypes::SPAWN_BLOCKED)
			Commands->Send_Custom_Event(obj,obj,6873525,spawnedObjectScriptID,0.1f);
		if (initialSpawn < spawnAtATime)
		{
			Stop_Timer(obj,1);
			Commands->Start_Timer(obj,this,0.1f,1);
		}
	}
}
bool JMG_Utility_Basic_Spawner_In_Radius::CheckIfObjectIsNearAnotherObject(Vector3 pos)
{
	if (minDistanceBetweenObjects <= 0)
		return false;
	for (SLNode<SpawnObjectNode> *node = spawnObjectNodeList.Head();node != NULL;node = node->Next())
	{
		if (SpawnObjectNode *data = node->Data())
		{
			if (!data->obj || (spawnGroupId > 0 && spawnGroupId != data->groupId))
				continue;
			if (JmgUtility::SimpleDistance(Commands->Get_Position(data->obj),pos) <= minDistanceBetweenObjects)
				return true;
		}
	}
	return false;
}
void JMG_Utility_Basic_Spawner_In_Radius::SetFacing(GameObject *obj,float facing)
{
	if (obj->As_SoldierGameObj())
	{
		char params[512];
		sprintf(params,"0.1,%.2f",facing);
		Commands->Attach_Script(obj,"JMG_Utility_Delay_Then_Rotate_Camera",params);
	}
	else
		Commands->Set_Facing(obj,facing);
}
int JMG_Utility_Basic_Spawner_In_Radius::GetPlayerLimitModifier()
{
	return (int)playersAddToSpawnAtATime*Get_Player_Count();
}
void JMG_Utility_Basic_Spawner_In_Radius_Attached::Created(GameObject *obj)
{
	if (sentCreateMessage)
		return;
	sentCreateMessage = true;
	GameObject *controller = Commands->Find_Object(Get_Int_Parameter("Controller_ID"));
	if (controller)
		Commands->Send_Custom_Event(obj,controller,6873523,Get_Int_Parameter("Spawned_Object_Script_ID"),0.0f);
	AddSpawnedObjectToGroup(obj);
}
void JMG_Utility_Basic_Spawner_In_Radius_Attached::Destroyed(GameObject *obj)
{
	if (sentDeathMessage)
		return;
	sentDeathMessage = true;
	GameObject *controller = Commands->Find_Object(Get_Int_Parameter("Controller_ID"));
	if (controller)
		Commands->Send_Custom_Event(obj,controller,6873524,Get_Int_Parameter("Spawned_Object_Script_ID"),0.0f);
}
void JMG_Utility_Basic_Spawner_In_Radius_Attached::AddSpawnedObjectToGroup(GameObject *spawned)
{
	int spawnGroupId = Get_Int_Parameter("Spawn_Group_ID");
	if (spawnGroupId == 0)
		return;
	for (SLNode<JMG_Utility_Basic_Spawner_In_Radius::SpawnObjectNode> *node = JMG_Utility_Basic_Spawner_In_Radius::spawnObjectNodeList.Head();node != NULL;node = node->Next())
	{
		if (JMG_Utility_Basic_Spawner_In_Radius::SpawnObjectNode *data = node->Data())
		{
			if (data->obj)
				continue;
			data->obj = spawned;
			data->groupId = spawnGroupId;
			return;
		}
	}
	JMG_Utility_Basic_Spawner_In_Radius::SpawnObjectNode *node = new JMG_Utility_Basic_Spawner_In_Radius::SpawnObjectNode(spawned,spawnGroupId);
	JMG_Utility_Basic_Spawner_In_Radius::spawnObjectNodeList.Add_Tail(node);
}
void JMG_Utility_Flying_Vehicle_Crash_Apply_Damage::Created(GameObject *obj)
{
	subtractMinSpeed = Get_Int_Parameter("Subtract_Min_Speed")?true:false; 
	minCollisionSpeed = Get_Float_Parameter("Min_Collision_Speed");
	maxCollisionSpeed = Get_Float_Parameter("Max_Collision_Speed");
	sprintf(collisionSound,Get_Parameter("Collision_Sound"));
	sprintf(explosionPreset,Get_Parameter("Explosion_Preset"));
	allowCrash = false;
	if (!obj->As_VehicleGameObj())
	{
		Console_Input("msg JMG_Utility_Flying_Vehicle_Crash_Apply_Damage ERROR: Must be attached to a vehicle");
		Destroy_Script();
		return;
	}
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Flying_Vehicle_Crash_Apply_Damage::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!obj->As_PhysicalGameObj()->Is_Attached_To_An_Object())
		{
			Vector3 tempSpeed,futurePos;
			obj->As_VehicleGameObj()->Get_Velocity(tempSpeed);
			float speed = tempSpeed.Length2()-(subtractMinSpeed?minCollisionSpeed:0.0f);
			futurePos = Commands->Get_Position(obj);
			if ((subtractMinSpeed && speed > 0.0f) || speed > minCollisionSpeed)
			{
				float facing = Commands->Get_Facing(obj);
				float positionOffset = (speed/202.336f)*15.0f;
				futurePos.X += cos(facing*PI180)*positionOffset;
				futurePos.Y += sin(facing*PI180)*positionOffset;
				MoveablePhysClass *mphys = obj->As_PhysicalGameObj() ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
				if (!mphys || mphys->Can_Teleport(Matrix3D(futurePos)))
				{
					if (allowCrash)
					{
						allowCrash = false;
						Commands->Apply_Damage(obj,(speed/maxCollisionSpeed*(Commands->Get_Max_Health(obj)+Commands->Get_Max_Shield_Strength(obj))*0.5f),"None",obj);
						Commands->Create_Sound(collisionSound,Commands->Get_Position(obj),obj);
						if (!Commands->Get_Health(obj))
							Commands->Create_Explosion(explosionPreset,Commands->Get_Position(obj),obj);
					}
				}
				else
					allowCrash = true;
			}
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Enemy_Seen_Send_Custom::Created(GameObject *obj)
{
	lastEnemyId = 0;
	seenTime = 0;
	enemyPresetId = Get_Int_Parameter("Enemy_Preset_ID");
	id = Get_Int_Parameter("ID");
	visibleMessage = Get_Int_Parameter("Visible_Message");
	notVisibleMessage = Get_Int_Parameter("Not_Visible_Message");
	visibleParam = Get_Int_Parameter("Visible_Param");
	notVisibleParam = Get_Int_Parameter("Not_Visible_Param");
	maxNotSeenTime = (int)(Get_Float_Parameter("Max_Lost_Sight_Time")*10);
	repeatSendSeenCustom = Get_Int_Parameter("RepeatSendSeenCustom") != 0;
	carTankBike = Get_Vector3_Parameter("AttackPriority[Car|Tank|Bike]");
	flyingTurretBoat = Get_Vector3_Parameter("AttackPriority[Flying|Turret|Boat]");
	submarineInfantryUnused = Get_Vector3_Parameter("AttackPriority[Submarine|Infantry|Unused]");
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Enemy_Seen_Send_Custom::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	GameObject *vehicle = Get_Vehicle(seen);
	if (vehicle)
		seen = vehicle;
	if (!JmgUtility::CanSeeStealth(0,obj,seen))
		return;
	float seenPriority = GetPriority(seen);
	if (!seenPriority)
		return;
	if (enemyPresetId && enemyPresetId != Commands->Get_Preset_ID(seen))
		return;
	if (Is_Script_Attached(seen,"JMG_Utility_Enemy_Seen_Send_Custom_Ignore"))
		return;
	if (!Commands->Get_Health(seen))
		return;
	int seenId = Commands->Get_ID(seen);
	if (!seenTime || !lastEnemyId)
	{
		lastEnemyId = seenId;
		SendCustom(obj,visibleMessage,visibleParam);
	}
	else if (lastEnemyId != seenId)
	{
		GameObject *enemy = Commands->Find_Object(seenId);
		if (!enemy || seenPriority > GetPriority(enemy))
		{
			if (lastEnemyId)
				SendCustom(obj,notVisibleMessage,notVisibleParam);
			lastEnemyId = seenId;
			SendCustom(obj,visibleMessage,visibleParam);
		}
	}
	if (lastEnemyId == seenId)
		seenTime = maxNotSeenTime;

}
void JMG_Utility_Enemy_Seen_Send_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		GameObject *target = Commands->Find_Object(lastEnemyId);
		if (!target || !Commands->Get_Health(target))
		{
			SendCustom(obj,notVisibleMessage,notVisibleParam);
			lastEnemyId = 0;
			seenTime = 0;
		}
		if (seenTime)
		{
			seenTime--;
			if (!seenTime)
			{
				SendCustom(obj,notVisibleMessage,notVisibleParam);
				lastEnemyId = 0;
			}
		}
		if (repeatSendSeenCustom && target && seenTime)
			SendCustom(obj,visibleMessage,visibleParam);
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
float JMG_Utility_Enemy_Seen_Send_Custom::GetPriority(GameObject *seen)
{
	if (seen->As_VehicleGameObj())
		switch (Get_Vehicle_Mode(seen))
		{
			case VEHICLE_TYPE_CAR: return carTankBike.X;
			case VEHICLE_TYPE_TANK: return carTankBike.Y;
			case VEHICLE_TYPE_BIKE: return carTankBike.Z;
			case VEHICLE_TYPE_FLYING: return flyingTurretBoat.X;
			case VEHICLE_TYPE_TURRET: return flyingTurretBoat.Y;
			case VEHICLE_TYPE_BOAT: return flyingTurretBoat.Z;
			case VEHICLE_TYPE_SUB: return submarineInfantryUnused.X;
		}
	else if (seen->As_SoldierGameObj())
		return submarineInfantryUnused.Y;
	Console_Input("msg JMG_Utility_Enemy_Seen_Send_Custom ERROR: Undefined vehicle type!");
	return 0;
}
void JMG_Utility_Enemy_Seen_Send_Custom::SendCustom(GameObject *obj,int custom,int param)
{
	if (!custom)
		return;
	GameObject *object = !id ? obj : (id == -1 ? Commands->Find_Object(lastEnemyId) : Commands->Find_Object(id));
	if (object)
		Commands->Send_Custom_Event(obj,object,custom,param == -1 ? lastEnemyId : param,0);
}
void JMG_Utility_Custom_Send_Custom_If_Script_Attached::Created(GameObject *obj)
{
	recieveMessage = Get_Int_Parameter("Custom");
	sprintf(script,"%s",Get_Parameter("Script"));
}
void JMG_Utility_Custom_Send_Custom_If_Script_Attached::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		if (!Is_Script_Attached(obj,script))
			return;
		int id = Get_Int_Parameter("ID");
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
		int Param = Get_Int_Parameter("Param");
		Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("SendCustom"),Param == -1 ? param : Param,Get_Float_Parameter("Delay"));
	}
}
void JMG_Utility_Custom_Send_Custom_If_Script_Not_Attached::Created(GameObject *obj)
{
	recieveMessage = Get_Int_Parameter("Custom");
	sprintf(script,"%s",Get_Parameter("Script"));
}
void JMG_Utility_Custom_Send_Custom_If_Script_Not_Attached::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		if (Is_Script_Attached(obj,script))
			return;
		int id = Get_Int_Parameter("ID");
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
		int Param = Get_Int_Parameter("Param");
		Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("SendCustom"),Param == -1 ? param : Param,Get_Float_Parameter("Delay"));
	}
}
void JMG_Utility_Basic_Spawner_In_Radius_Controller::Destroyed(GameObject *obj)
{
	JMG_Utility_Basic_Spawner_In_Radius::spawnObjectNodeList.Remove_All();
}
void JMG_Utility_Basic_Spawner_In_Radius_Controller::Detach(GameObject *obj)
{
	Destroyed(obj);
}
void JMG_Utility_Custom_Set_Engine::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	enable = Get_Int_Parameter("Enable");
}
void JMG_Utility_Custom_Set_Engine::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Enable_Engine(obj,(enable == -1 ? param : enable) != 0 ? true : false);
	}
}
void JMG_Utility_Send_Custom_Player_Count_Matches_Preset_Count::Created(GameObject *obj)
{
	presetId = Get_Int_Parameter("Preset_ID");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Message");
	paramx = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	rate = Get_Float_Parameter("Rate");
	minPlayers = Get_Int_Parameter("Min_Player_Count");
	maxPlayers = Get_Int_Parameter("Max_Player_Count");
	repeat = Get_Int_Parameter("Repeat") == 0 ? false : true;
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Send_Custom_Player_Count_Matches_Preset_Count::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		int playerCount = Get_Player_Count();
		if ((minPlayers == -1 || playerCount >= minPlayers) && (maxPlayers == -1 || playerCount <= maxPlayers))
		{
			int count = 0;
			for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
			{
				GameObject* o = current->Data()->As_ScriptableGameObj();
				if (Commands->Get_Preset_ID(o) == presetId)
					count++;
			}
			if (count == playerCount)
			{
				GameObject *object = !id ? obj : Commands->Find_Object(id);
				if (object)
					Commands->Send_Custom_Event(obj,object,custom,paramx,delay);
				if (!repeat)
				{
					Destroy_Script();
					return;
				}
			}
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Custom_Set_Position::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	position = Get_Vector3_Parameter("Position");
	id = Get_Int_Parameter("ID");
}
void JMG_Utility_Custom_Set_Position::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		GameObject *target = id ? Commands->Find_Object(id) : obj;
		Commands->Set_Position(target,position);
		if (target->As_ScriptZoneGameObj())
		{
			OBBoxClass *zone = Get_Zone_Box(target);
			zone->Center = position;
			JmgUtility::RotateZoneBox(Commands->Get_Facing(target),zone->Basis);
			Set_Zone_Box(target,*zone);
		}
	}
}
void JMG_Utility_Custom_Delay_Send_Custom::Created(GameObject *obj)
{
	watchMessage = Get_Int_Parameter("Custom");
	id = Get_Int_Parameter("ID");
	sendMessage = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	randomDelay = Get_Float_Parameter("RandomDelay");
	cancelCustom = Get_Int_Parameter("CancelCustom");
	lastSender = NULL;
}
void JMG_Utility_Custom_Delay_Send_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == watchMessage)
	{
		Stop_Timer2(obj,this,1);
		lastSender = sender;
		lastParam = param;
		Commands->Start_Timer(obj,this,delay+(randomDelay ? Commands->Get_Random(0.0f,randomDelay) : 0.0f),1);
	}
	if (message == cancelCustom)
	{
		Stop_Timer2(obj,this,1);
	}
}
void JMG_Utility_Custom_Delay_Send_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		GameObject *object = id ? (id == -1 ? lastSender : Commands->Find_Object(id)) : obj;
		Commands->Send_Custom_Event(obj,object,sendMessage,Param == -1 ? lastParam : Param,0);
	}
}
void JMG_Utility_Scale_HP_By_Player_Count::Created(GameObject *obj)
{
	maxPlayerCount = Get_Int_Parameter("Max_Player_Count");
	lastPlayerCount = 1;
	originalHealth = Commands->Get_Max_Health(obj);
	originalArmor = Commands->Get_Max_Shield_Strength(obj);;
	healthMultiplier = Get_Float_Parameter("Health_Multiplier");
	armorMultiplier = Get_Float_Parameter("Armor_Multiplier");
	repeat = Get_Int_Parameter("Repeat") == 0 ? false : true;
	updateScaleCustom = Get_Int_Parameter("UpdateScaleCustom");
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Scale_HP_By_Player_Count::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		RescaleHP(obj);
		if (!repeat)
		{
			Destroy_Script();
			return;
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Scale_HP_By_Player_Count::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (updateScaleCustom && message == updateScaleCustom)
	{
		RescaleHP(obj);
	}
}
void JMG_Utility_Scale_HP_By_Player_Count::RescaleHP(GameObject *obj)
{
	int playerCount = maxPlayerCount == -1 ? Get_Player_Count() : min(maxPlayerCount,Get_Player_Count());
	if (playerCount && lastPlayerCount != playerCount)
	{
		float healthRatio = Commands->Get_Health(obj)/Commands->Get_Max_Health(obj);
		float armorRatio = Commands->Get_Shield_Strength(obj)/Commands->Get_Max_Shield_Strength(obj);
		Set_Max_Health(obj,healthMultiplier*playerCount*originalHealth);
		Set_Max_Shield_Strength(obj,armorMultiplier*playerCount*originalArmor);
		Commands->Set_Health(obj,Commands->Get_Max_Health(obj)*healthRatio);
		Commands->Set_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj)*armorRatio);
		lastPlayerCount = playerCount;
	}
}
void JMG_Utility_Custom_Send_Custom_To_All_Objects::Created(GameObject *obj)
{
	recieveMessage = Get_Int_Parameter("Custom");
	team = Get_Int_Parameter("Team");
	custom = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
}
void JMG_Utility_Custom_Send_Custom_To_All_Objects::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
		{
			GameObject* o = current->Data()->As_ScriptableGameObj();
			if (o && (team == 2 || CheckPlayerType(o,team)))
 				Commands->Send_Custom_Event(obj,o,custom,Param == -1 ? param : Param,delay);
		}
	}
}
void JMG_Utility_Enemy_Seen_Send_Custom_Ignore::Created(GameObject *obj)
{
}
void JMG_Utility_In_Line_Of_Sight_Send_Custom::Created(GameObject *obj)
{
	lastEnemyId = 0;
	enemyPresetId = Get_Int_Parameter("Enemy_Preset_ID");
	id = Get_Int_Parameter("ID");
	visibleMessage = Get_Int_Parameter("Visible_Message");
	notVisibleMessage = Get_Int_Parameter("Not_Visible_Message");
	visibleParam = Get_Int_Parameter("Visible_Param");
	notVisibleParam = Get_Int_Parameter("Not_Visible_Param");
	rate = Get_Float_Parameter("Scan_Rate");
	enemyOnly = Get_Int_Parameter("Enemy_Only") == 0 ? false : true;
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_In_Line_Of_Sight_Send_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		bool objectSeen = false;
		for (SLNode<SmartGameObj> *node = GameObjManager::SmartGameObjList.Head();node;node = node->Next())
		{
			SmartGameObj* object = node->Data();
			if (object == obj)
				continue;
			if ((!enemyOnly || obj->As_SmartGameObj()->Is_Enemy(object)) && object->Is_Visible() && obj->As_SmartGameObj()->Is_Obj_Visible(object))
			{
				if (object->Peek_Physical_Object())
				{
					RenderObjClass* model = object->Peek_Physical_Object()->Peek_Model();
					if (model && model->Is_Hidden())
						continue;
				}
				if (Test_Line_Of_Sight(obj,object))
					objectSeen = true;
			}
		}
		if (!objectSeen && lastEnemyId)
		{
			GameObject *object = !id ? obj : (id == -1 ? Commands->Find_Object(lastEnemyId) : Commands->Find_Object(id));
			if (object)
				Commands->Send_Custom_Event(obj,object,notVisibleMessage,notVisibleParam,0);
			lastEnemyId = 0;
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
bool JMG_Utility_In_Line_Of_Sight_Send_Custom::Test_Line_Of_Sight(GameObject *obj,GameObject *seen)
{
	if (!JmgUtility::CanSeeStealth(0,obj,seen))
		return false;
	if (enemyPresetId && enemyPresetId != Commands->Get_Preset_ID(seen))
		return false;
	if (Is_Script_Attached(seen,"JMG_Utility_In_Line_Of_Sight_Send_Custom_Ignore"))
		return false;
	int seenId = Commands->Get_ID(seen);
	if (!lastEnemyId)
	{
		GameObject *object = !id ? obj : (id == -1 ? seen : Commands->Find_Object(id));
		if (object)
			Commands->Send_Custom_Event(obj,object,visibleMessage,visibleParam,0);
		lastEnemyId = seenId;
		return true;
	}
	if (lastEnemyId == seenId)
		return true;
	return false;
}
void JMG_Utility_In_Line_Of_Sight_Send_Custom_Ignore::Created(GameObject *obj)
{
}
void JMG_Utility_Timer_Trigger_Enemy_Seen::Created(GameObject *obj)
{
	rate = Get_Float_Parameter("Scan_Rate");
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Timer_Trigger_Enemy_Seen::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		for (SLNode<SmartGameObj> *node = GameObjManager::SmartGameObjList.Head();node;node = node->Next())
		{
			SmartGameObj* object = node->Data();
			if (object == obj)
				continue;
			if (obj->As_SmartGameObj()->Is_Enemy(object) && object->Is_Visible() && obj->As_SmartGameObj()->Is_Obj_Visible(object))
			{
				if (object->Peek_Physical_Object())
				{
					RenderObjClass* model = object->Peek_Physical_Object()->Peek_Model();
					if (model && model->Is_Hidden())
						continue;
				}
				const SimpleDynVecClass<GameObjObserverClass *> & observer_list = obj->Get_Observers();
				for(int index = 0;index < observer_list.Count();index++)
					observer_list[index]->Enemy_Seen(obj,object);
			}
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Custom_Teleport_To_Random_Wander_Point::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	retryOnFailure = Get_Int_Parameter("RetryOnFailure") ? true : false;
	safeTeleportDistance = Get_Float_Parameter("SafeTeleportDistance");
	wanderPointGroup = Get_Int_Parameter("WanderingAIGroupID");
}
void JMG_Utility_Custom_Teleport_To_Random_Wander_Point::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Grab_Teleport_Spot(obj,5);
	}
}
bool JMG_Utility_Custom_Teleport_To_Random_Wander_Point::Grab_Teleport_Spot(GameObject *enter,int attempts)
{
	if (The_Game()->Get_Game_Duration_S() < 1.0f)
	{
		char params[220];
		sprintf(params,"%d,%.2f",wanderPointGroup,safeTeleportDistance);
		Commands->Attach_Script(enter,"JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach",params);
		return false;
	}
	if (!attempts)
	{
		if (retryOnFailure)
		{
			char params[220];
			sprintf(params,"%d,%.2f",wanderPointGroup,safeTeleportDistance);
			Commands->Attach_Script(enter,"JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach",params);
		}
		return false;
	}
	attempts--;
	Vector3 targetPos;
	float facing;
	if (!Get_A_Defense_Point(&targetPos,&facing))
	{
		char errormsg[220];
		sprintf(errormsg,"msg JMG_Utility_Custom_Teleport_To_Random_Wander_Point ERROR: No wander points found for group %d!",wanderPointGroup);
		Console_Input(errormsg);
		return false;
	}
	MoveablePhysClass *mphys = enter->As_PhysicalGameObj() ? enter->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	if (mphys && mphys->Find_Teleport_Location(targetPos,safeTeleportDistance,&targetPos))
	{
		Toggle_Fly_Mode(enter);
		Commands->Set_Position(enter,targetPos);
		Force_Position_Update(enter);
		Toggle_Fly_Mode(enter);
		char params[220];
		sprintf(params,"0.1,%.2f",facing);
		Commands->Attach_Script(enter,"JMG_Utility_Delay_Then_Rotate_Camera",params);
		return true;
	}
	else
	{
		Commands->Set_Position(enter,targetPos);
		return Grab_Teleport_Spot(enter,attempts);
	}
}
void JMG_Utility_Send_Custom_If_Not_Moving_Enough::Created(GameObject *obj)
{
	resetTime = time = (int)(Get_Float_Parameter("Time")*10);
	distance = Get_Float_Parameter("Distance");
	distance *= distance;
	lastPos = Commands->Get_Position(obj);
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Send_Custom_If_Not_Moving_Enough::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),lastPos) < distance)
		{
			time--;
			if (!time)
			{
				GameObject *object = id ? Commands->Find_Object(id) : obj;
 				Commands->Send_Custom_Event(obj,object,custom,Param,delay);
			}
			if (time < 0)
				time = resetTime;
		}
		else
		{
			lastPos = Commands->Get_Position(obj);
			time = resetTime;
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
JMG_Utility_AI_Skittish_Herd_Animal::HerdAnimalPositionSystem JMG_Utility_AI_Skittish_Herd_Animal::HerdAnimalPositionControl[128] = {HerdAnimalPositionSystem()};
void JMG_Utility_AI_Skittish_Herd_Animal::Created(GameObject *obj)
{
	actionCrouched = Get_Float_Parameter("ActionsCrouched");
	weaponRange = 3.75;
	sprintf(defaultWeapon,"%s",Get_Current_Weapon(obj));
	int minHerdId = Get_Int_Parameter("MinHerdID");
	int maxHerdId = Get_Int_Parameter("MaxHerdID");
	wanderPointGroup = Get_Int_Parameter("WanderGroupID");
	if (minHerdId > 128 || maxHerdId > 128)
	{
		Console_Input("msg JMG_Utility_AI_Skittish_Herd_Animal ERROR: Herd ID must not exceed 128!");
		Destroy_Script();
		return;
	}
	minRetreatRange = Get_Float_Parameter("MinRetreatRange");
	maxRetreatRange = Get_Float_Parameter("MaxRetreatRange");
	wanderRadiusAroundHerdCenter = Get_Float_Parameter("WanderRadiusAroundHerdCenter");
	wanderRadiusAroundHerdCenterSq = wanderRadiusAroundHerdCenter+10.0f;
	wanderRadiusAroundHerdCenterSq *= wanderRadiusAroundHerdCenterSq;
	minWanderFrequency = Get_Float_Parameter("MinWanderFrequency");
	maxWanderFrequency = Get_Float_Parameter("MaxWanderFrequency");
	minRetreatTime = Get_Float_Parameter("MinRetreatTime")*1000.0f;
	maxRetreatTime = Get_Float_Parameter("MaxRetreatTime")*1000.0f;
	minUpdateHerdCenter = Get_Float_Parameter("MinUpdateHerdCenter")*1000.0f;
	maxUpdateHerdCenter = Get_Float_Parameter("MaxUpdateHerdCenter")*1000.0f;
	runTowardThreatChance = Get_Float_Parameter("RunTowardThreatChance");
	herdId = minHerdId == maxHerdId ? maxHerdId : Commands->Get_Random_Int(minHerdId,maxHerdId);
	HerdPositionNode = HerdAnimalPositionControl[herdId].AddNode(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,1.1f,1);
	Commands->Start_Timer(obj,this,1.1f,2);
}
void JMG_Utility_AI_Skittish_Herd_Animal::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	if (!Commands->Get_Health(seen))
		return;
	if (((HerdAnimalPositionControl[herdId].checkRetreatCloseEnough() || difftime(clock(),HerdAnimalPositionControl[herdId].herdRetreatStart) > HerdAnimalPositionControl[herdId].herdRetreatTime)))
		setRetreatLocation(obj,seen);
}
void JMG_Utility_AI_Skittish_Herd_Animal::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && Commands->Get_Health(obj))
	{
		if (HerdPositionNode)
			HerdPositionNode->pos = Commands->Get_Position(obj);
		if (difftime(clock(),HerdAnimalPositionControl[herdId].herdRetreatStart) < HerdAnimalPositionControl[herdId].herdRetreatTime)
		{
			Vector3 pos = Commands->Get_Position(obj);
			if (JmgUtility::SimpleDistance(pos,HerdAnimalPositionControl[herdId].herdRetreatLocation) < wanderRadiusAroundHerdCenterSq)
			{
				pos = HerdAnimalPositionControl[herdId].herdRetreatLocation;
				float randomRot = Commands->Get_Random(-180.0f,180.0f);
				float dist = Commands->Get_Random(0.0f,wanderRadiusAroundHerdCenter);
				pos.X += cos(randomRot)*dist;
				pos.Y += sin(randomRot)*dist;
				Vector3 finalPos;
				if (Get_Random_Pathfind_Spot(pos,wanderRadiusAroundHerdCenter,&finalPos))
					pos = finalPos;
			}
			else
				pos = HerdAnimalPositionControl[herdId].herdRetreatLocation;
			GotoLocation(obj,pos,NULL,1.25f);
			if (HerdAnimalPositionControl[herdId].checkRetreatSuccessful())
				HerdAnimalPositionControl[herdId].herdRetreatTime = 0;
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
	if (number == 2)
	{
		if (difftime(clock(),HerdAnimalPositionControl[herdId].herdRetreatStart) > HerdAnimalPositionControl[herdId].herdRetreatTime)
		{
			const char *weap = Get_Current_Weapon(obj);
			if (weap && !_stricmp(weap,defaultWeapon))
			{
				Commands->Select_Weapon(obj,"");
				Commands->Select_Weapon(obj,defaultWeapon);
			}
		}
		Vector3 pos = HerdAnimalPositionControl[herdId].getCenterLocation(true,wanderPointGroup,minUpdateHerdCenter,maxUpdateHerdCenter);
		float randomRot = Commands->Get_Random(-180.0f,180.0f);
		float dist = Commands->Get_Random(0.0f,wanderRadiusAroundHerdCenter);
		pos.X += cos(randomRot)*dist;
		pos.Y += sin(randomRot)*dist;
		Vector3 finalPos;
		if (Get_Random_Pathfind_Spot(pos,wanderRadiusAroundHerdCenter,&finalPos))
			GotoLocation(obj,finalPos,NULL);
		else
			GotoLocation(obj,pos,NULL);
		Commands->Start_Timer(obj,this,Commands->Get_Random(minWanderFrequency,maxWanderFrequency),2);
	}
}
void JMG_Utility_AI_Skittish_Herd_Animal::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damager && damage > 0 && (herdId && (HerdAnimalPositionControl[herdId].checkRetreatCloseEnough() || difftime(clock(),HerdAnimalPositionControl[herdId].herdRetreatStart) > HerdAnimalPositionControl[herdId].herdRetreatTime)))
		setRetreatLocation(obj,damager);
}
void JMG_Utility_AI_Skittish_Herd_Animal::Destroyed(GameObject *obj)
{
	HerdAnimalPositionControl[herdId] -= obj;
}
void JMG_Utility_AI_Skittish_Herd_Animal::GotoLocation(GameObject *obj,const Vector3 &pos,GameObject *Enemy,float speed)
{
	ActionParamsStruct params;
	Commands->Get_Action_Params(obj,params);
	params.Set_Basic(this,100,1);
	params.Set_Movement(pos,speed ? speed : (Enemy ? 1.0f : Commands->Get_Random(0.25f,1.0f)),Enemy ? weaponRange*Commands->Get_Random(0.01f,1.0f) : 1.0f,false);
	params.MovePathfind = true;
	Vector3 mypos = Commands->Get_Position(obj);
	if (!speed && JmgUtility::SimpleDistance(mypos,pos) < 625 && Commands->Get_Random(0.0f,1.0f) < actionCrouched)
	{
		params.MoveCrouched = true;
		params.AttackCrouched = true;
	}
	Commands->Action_Goto(obj,params);
}
void JMG_Utility_AI_Skittish_Herd_Animal::setRetreatLocation(GameObject *obj,GameObject *enemy)
{
	Vector3 pos = Commands->Get_Position(obj);
	Vector3 mypos = Commands->Get_Position(enemy);
	pos.X -= mypos.X;
	pos.Y -= mypos.Y;
	float TempRotation = atan2(pos.Y,pos.X);
	float dist = Commands->Get_Random(minRetreatRange,maxRetreatRange);
	if (Commands->Get_Random(0.0f,1.0f) < runTowardThreatChance)
		dist *= -1.0f;
	pos = Commands->Get_Position(obj);
	pos.X += cos(TempRotation)*dist;
	pos.Y += sin(TempRotation)*dist;
	if (Get_A_Wander_Point(&pos,wanderPointGroup))
	{
		HerdAnimalPositionControl[herdId].herdRetreatLocation = pos;
		HerdAnimalPositionControl[herdId].herdRetreatTime = Commands->Get_Random(minRetreatTime,maxRetreatTime);
		HerdAnimalPositionControl[herdId].herdRetreatStart = clock();
	}
	GotoLocation(obj,pos,NULL,1.25f);
}
Vector3 JMG_Utility_AI_Skittish_Herd_Animal::HerdAnimalPositionSystem::getCenterLocation(bool allowWander,int wanderPointGroup,float minUpdateHerdCenter,float maxUpdateHerdCenter)
{
	if (difftime(clock(),herdRetreatStart) < herdRetreatTime)
		return herdRetreatLocation;
	if (difftime(clock(),lastPosCalcTime) < minUpdateDelay && hascalculatedPos)
		return centerPos;
	minUpdateDelay = Commands->Get_Random(minUpdateHerdCenter,maxUpdateHerdCenter);
	hascalculatedPos = true;
	lastPosCalcTime = clock();
	if (allowWander && Commands->Get_Random(0.00,1.00) < 0.1f && JMG_Utility_AI_Skittish_Herd_Animal::Get_A_Wander_Point(&centerPos,wanderPointGroup))
		return centerPos;
	HerdPositionNode *Current = HerdPositionNodeList;
	Vector3 activePos;
	int activeCount = 0;
	while (Current)
	{
		if (Current->alive)
		{
			activeCount++;
			activePos += Current->pos;
		}
		Current = Current->next;			
	}
	if (!activeCount)
	{
		JMG_Utility_AI_Skittish_Herd_Animal::Get_A_Wander_Point(&centerPos,wanderPointGroup);
		return centerPos;
	}
	centerPos.X = activePos.X/activeCount;
	centerPos.Y = activePos.Y/activeCount;
	centerPos.Z = activePos.Z/activeCount;
	return centerPos;
}
void JMG_Utility_AI_Skittish_Herd_Animal_Ignore::Created(GameObject *obj)
{
}
void JMG_Utility_AI_Skittish_Herd_Animal_Controller::Destroyed(GameObject *obj)
{
	for (int x = 0;x < 128;x++)
		JMG_Utility_AI_Skittish_Herd_Animal::HerdAnimalPositionControl[x].Empty_List();
}
void JMG_Utility_AI_Skittish_Herd_Animal_Controller::Detach(GameObject *obj)
{
	Destroyed(obj);
}
void JMG_Utility_Custom_Display_Dialog_Box::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	textMessage = newstr(Get_Parameter("Message"));
	char delim = Get_Parameter("Delim")[0];
	unsigned int length = strlen(textMessage);
	for (unsigned int x = 0;x < length;x++)
		if (textMessage[x] == delim)
			textMessage[x] = ',';
}
void JMG_Utility_Custom_Display_Dialog_Box::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		char dialog[1000];
		sprintf(dialog,"pamsg %d %s",JmgUtility::JMG_Get_Player_ID(obj),textMessage);
		Console_Input(dialog);
	}
}
void JMG_Utility_Custom_Send_Custom_On_Preset_Count::Created(GameObject *obj)
{
	recieveMessage = Get_Int_Parameter("Custom");
	presetId = Get_Int_Parameter("PresetID");
	minCount = Get_Int_Parameter("MinCount");
	maxCount = Get_Int_Parameter("MaxCount");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
}
void JMG_Utility_Custom_Send_Custom_On_Preset_Count::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		int count = 0;
		for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
		{
			GameObject* o = current->Data()->As_ScriptableGameObj();
			if (Commands->Get_Preset_ID(o) == presetId)
				count++;
		}
		if (count >= minCount && (maxCount == -1 || count <= maxCount))
		{
			GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
 			Commands->Send_Custom_Event(obj,object,custom,Param == -1 ? param : Param,delay);
		}
	}
}
void JMG_Utility_Custom_Send_Custom_On_Script_Count::Created(GameObject *obj)
{
	recieveMessage = Get_Int_Parameter("Custom");
	sprintf(script,"%s",Get_Parameter("Script"));
	minCount = Get_Int_Parameter("MinCount");
	maxCount = Get_Int_Parameter("MaxCount");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
}
void JMG_Utility_Custom_Send_Custom_On_Script_Count::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		int count = 0;
		for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
		{
			GameObject* o = current->Data()->As_ScriptableGameObj();
			if (Is_Script_Attached(o,script))
				count++;
		}
		if (count >= minCount && (maxCount == -1 || count <= maxCount))
		{
			GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
 			Commands->Send_Custom_Event(obj,object,custom,Param == -1 ? param : Param,delay);
		}
	}
}
void JMG_Utility_Poke_Send_Custom_From_Poker::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void JMG_Utility_Poke_Send_Custom_From_Poker::Poked(GameObject *obj, GameObject *poker)
{
	if (CheckPlayerType(poker,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(poker) == -4)
		return;
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? (id == -1 ? poker : Commands->Find_Object(id)) : obj;
	if (object)
		Commands->Send_Custom_Event(poker,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	if (Get_Int_Parameter("TriggerOnce"))
	{
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		Destroy_Script();
	}
}
void JMG_Utility_Custom_Grant_Scaled_Score::Created(GameObject *obj)
{
	maxPlayerCount = Get_Int_Parameter("MaxPlayerCount");
	lastPlayerCount = 1;
	theScore = score = Get_Float_Parameter("Score");
	scoreMultiplier = Get_Float_Parameter("ScoreMultiplier");
	repeat = Get_Int_Parameter("Repeat") == 0 ? false : true;
	grantCustom = Get_Int_Parameter("GrantCustom");
	updateScaleCustom = Get_Int_Parameter("UpdateScaleCustom");
	stopUpdateCustom = Get_Int_Parameter("StopUpdateCustom");
	grantToSender = Get_Int_Parameter("GrantToSender") == 0 ? false : true;
	entireTeam = Get_Int_Parameter("EntireTeam") == 0 ? false : true;
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Custom_Grant_Scaled_Score::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		RescaleScore(obj);
		if (!repeat)
		{
			Destroy_Script();
			return;
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Custom_Grant_Scaled_Score::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (grantCustom && message == grantCustom)
	{
		Commands->Give_Points(grantToSender ? sender : obj,theScore,entireTeam);
	}
	if (updateScaleCustom && message == updateScaleCustom)
	{
		RescaleScore(obj);
	}
	if (stopUpdateCustom && message == stopUpdateCustom)
	{
		Stop_Timer2(obj,this,1);
	}
}
void JMG_Utility_Custom_Grant_Scaled_Score::RescaleScore(GameObject *obj)
{
	int playerCount = maxPlayerCount == -1 ? Get_Player_Count() : min(maxPlayerCount,Get_Player_Count());
	if (playerCount && lastPlayerCount != playerCount)
	{
		theScore = score*scoreMultiplier*playerCount;
		lastPlayerCount = playerCount;
	}
}
void JMG_Utility_Custom_Set_Infantry_Height::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	height = Get_Float_Parameter("Height");
}
void JMG_Utility_Custom_Set_Infantry_Height::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		if (height != -9999.9)
			obj->As_SoldierGameObj()->Set_Skeleton_Height(param/100.0f);
		else
			obj->As_SoldierGameObj()->Set_Skeleton_Height(height);
	}
}
void JMG_Utility_Custom_Set_Infantry_Width::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	width = Get_Float_Parameter("Width");
}
void JMG_Utility_Custom_Set_Infantry_Width::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		if (width != -9999.9)
			obj->As_SoldierGameObj()->Set_Skeleton_Width(param/100.0f);
		else
			obj->As_SoldierGameObj()->Set_Skeleton_Width(width);
	}
}
void JMG_Utility_Custom_Set_Weapon_Hold_Sytle::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	style = Get_Int_Parameter("Style");
}
void JMG_Utility_Custom_Set_Weapon_Hold_Sytle::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		obj->As_SoldierGameObj()->Set_Override_Weapon_Hold_Style(style == -2 ? param : style);
	}
}
void JMG_Utility_Custom_Set_Human_Anim_Override::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	enable = Get_Int_Parameter("Enable") ? true : false;
}
void JMG_Utility_Custom_Set_Human_Anim_Override::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		obj->As_SoldierGameObj()->Set_Human_Anim_Override(enable);
	}
}
void JMG_Utility_Swimming_Infantry_Advanced::Created(GameObject *obj)
{
	if (!Commands->Is_A_Star(obj))
	{
		char params[1024];
		sprintf(params,"%d,%s,%d,%d,%.2f,%.2f,%.2f,%s,%s,%s,%.2f%,%.2f,%.2f,%s,%d,%d,%s,%s,%s,%.2f,%.2f",Get_Int_Parameter("WeaponsGroupID"),Get_Parameter("WeaponPreset"),Get_Int_Parameter("ForceDefinedWeapons"),Get_Int_Parameter("DefaultHoldStyle"),Get_Float_Parameter("DefaultSwimSpeed"),Get_Float_Parameter("DrownTime"),Get_Float_Parameter("StartDrownSequence"),Get_Parameter("GaspForBreath"),Get_Parameter("PantingSoundEmitterModel"),Get_Parameter("HeartBeatSoundEmitterModel"),Get_Float_Parameter("DrownDamageRate"),Get_Float_Parameter("CatchBreathRate"),Get_Float_Parameter("WaterDamageAmount"),Get_Parameter("WaterDamageWarhead"),Get_Int_Parameter("WaterDamageDelayTime"),Get_Int_Parameter("WaterDamageDelayTimeRecover"),Get_Parameter("SwimmingSkin"),Get_Parameter("SwimmingArmor"),Get_Parameter("SwimmingModel"),Get_Float_Parameter("SwimmingHeightScale"),Get_Float_Parameter("SwimmingWidthScale"));
		Commands->Attach_Script(obj,"JMG_Utility_Swimming_Infantry_Advanced_AI",params);
		Destroy_Script();
		return;
	}
	lastDisplayTime = NULL;
	lastWaterZoneId = 0;
	heartBeatSoundId = 0;
	pantSoundId = 0;
	playerId = JmgUtility::JMG_Get_Player_ID(obj);
	underwater = false;
	waterZoneCount = 0;
	startedFadeRed = false;
	drownTime = 0.0f;
	defaultSpeed = obj->As_SoldierGameObj()->Get_Max_Speed();
	defaultHoldStyle = Get_Int_Parameter("DefaultHoldStyle");
	defaultSwimSpeedMultiplier = Get_Float_Parameter("DefaultSwimSpeed");
	waterDamageDelayTime = remainingWaterDamageDelay = Get_Int_Parameter("WaterDamageDelayTime");
	waterDamageDelayTimeRecover = Get_Int_Parameter("WaterDamageDelayTimeRecover");
	sprintf(originalSkin,"");
	sprintf(originalArmor,"");
	sprintf(originalModel,"");
	waterSpeedMultiplier = 1.0f;
	sprintf(defaultWeaponPreset,"%s",Get_Parameter("WeaponPreset"));
	defaultDrownTime = Get_Float_Parameter("DrownTime");
	startDrownSequence = Get_Float_Parameter("StartDrownSequence");
	waterDamageAmount = Get_Float_Parameter("WaterDamageAmount");
	sprintf(waterDamageWarhead,"%s",Get_Parameter("WaterDamageWarhead"));
	sprintf(swimmingSkin,"%s",Get_Parameter("SwimmingSkin"));
	sprintf(swimmingArmor,"%s",Get_Parameter("SwimmingArmor"));
	sprintf(swimmingModel,"%s",Get_Parameter("SwimmingModel"));
	enterWaterMessageStringId = Get_Int_Parameter("EnterWaterMessageStringId");
	waterEnterMessageColorRGB = Get_Vector3_Parameter("WaterEnterMessageColor[R|G|B]");
	sprintf(heartBeatSoundEmitterModel,"%s",Get_Parameter("HeartBeatSoundEmitterModel"));
	sprintf(pantingSoundEmitterModel,"%s",Get_Parameter("PantingSoundEmitterModel"));
	sprintf(gaspForBreath,"%s",Get_Parameter("GaspForBreath"));
	catchBreathRate = Get_Float_Parameter("CatchBreathRate");
	forceDefinedWeapons = Get_Int_Parameter("ForceDefinedWeapons") ? true : false;
	weaponGroupId = Get_Int_Parameter("WeaponsGroupID");
	swimmingHeightScale = Get_Float_Parameter("SwimmingHeightScale");
	swimmingWidthScale = Get_Float_Parameter("SwimmingWidthScale");
	originalHeightScale = 0.0f;
	originalWidthScale = 0.0f;
	drownDamageRate = drownDamageRate;
	weaponSwitchForward = true;
	for (SLNode<ScriptZoneGameObj> *node = GameObjManager::ScriptZoneGameObjList.Head();node;node = node->Next())
	{
		GameObject *zone = (GameObject *)node->Data();
		if (Is_Script_Attached(zone,"JMG_Utility_Swimming_Zone") && IsInsideZone(zone,obj))
			Commands->Send_Custom_Event(obj,obj,347341,347341,0.25f);
	}
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Swimming_Infantry_Advanced::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && Commands->Get_Health(obj) && !The_Game()->Is_Game_Over())
	{
		if (obj->As_SoldierGameObj()->Is_Crouched() && !obj->As_SoldierGameObj()->Is_Airborne() && waterZoneCount)
		{
			if (!underwater)
			{
				underwater = true;
				JMG_Utility_Swimming_Infantry::isUnderwater[playerId] = true;
				HideSoundEmitter(&pantSoundId);
				if (startedFadeRed)
				{
					Set_Screen_Fade_Color_Player(obj,1.0f,0.0f,0.0f,JmgUtility::MathClamp(defaultDrownTime-drownTime,0,startDrownSequence));
					CreateSoundEmitter(obj,heartBeatSoundEmitterModel,&heartBeatSoundId);
				}
				else
					Set_Screen_Fade_Color_Player(obj,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.X,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.Y,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.Z,0.0f);
				Set_Screen_Fade_Opacity_Player(obj,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColorOpacity,0.1f);
				Set_Fog_Range_Player(obj,JMG_Utility_Swimming_Zone::waterNode[playerId].waterMinViewDistance,JMG_Utility_Swimming_Zone::waterNode[playerId].waterMaxViewDistance,0.1f);
			}
			drownTime += 0.1f;
			if (!startedFadeRed && drownTime >= defaultDrownTime-startDrownSequence)
			{
				startedFadeRed = true;
				Set_Screen_Fade_Color_Player(obj,1.0f,0.0f,0.0f,startDrownSequence);
				CreateSoundEmitter(obj,heartBeatSoundEmitterModel,&heartBeatSoundId);
			}
			if (drownTime >= defaultDrownTime)
				Commands->Apply_Damage(obj,drownDamageRate,"None",obj);
		}
		else
		{
			if (underwater)
			{
				underwater = false;
				JMG_Utility_Swimming_Infantry::isUnderwater[playerId] = false;
				JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Player(obj,0.1f);
				Set_Fog_Range_Player(obj,JMG_Utility_Swimming_Zone::fogMinDistance,JMG_Utility_Swimming_Zone::fogMaxDistance,0.1f);
				HideSoundEmitter(&heartBeatSoundId);
				if (drownTime > 1.0f)
					CreateSoundEmitter(obj,pantingSoundEmitterModel,&pantSoundId);
				if (drownTime > defaultDrownTime)
					Commands->Create_3D_Sound_At_Bone(gaspForBreath,obj,"c head");
			}
			if (drownTime)
			{
				drownTime -= catchBreathRate;
				if (startedFadeRed && drownTime < defaultDrownTime-startDrownSequence)
					startedFadeRed = false;
				if (drownTime <= 0)
				{
					drownTime = 0.0f;
					HideSoundEmitter(&pantSoundId);
				}
			}
		}
		if (waterZoneCount)
		{
			SwitchWeapon(obj);
			if (remainingWaterDamageDelay)
				remainingWaterDamageDelay--;
			if (!remainingWaterDamageDelay && waterDamageAmount)
				Commands->Apply_Damage(obj,waterDamageAmount,waterDamageWarhead,Commands->Find_Object(lastWaterZoneId));
		}
		else if (!JMG_Utility_Swimming_Infantry::isInWater[playerId] && remainingWaterDamageDelay < waterDamageDelayTime)
		{
			remainingWaterDamageDelay += waterDamageDelayTimeRecover;
			if (remainingWaterDamageDelay > waterDamageDelayTime)
				remainingWaterDamageDelay = waterDamageDelayTime;
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Swimming_Infantry_Advanced::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 347340)
	{
		Commands->Send_Custom_Event(sender,obj,347342,param,0.25f);
	}
	if (message == 347342)
	{
		if (obj->As_SoldierGameObj()->Is_Airborne())
		{
			Commands->Send_Custom_Event(sender,obj,message,param,0.25f);
			return;
		}
		waterZoneCount--;
		if (!waterZoneCount)
		{
			JMG_Utility_Swimming_Infantry::isInWater[playerId] = false;
			obj->As_SoldierGameObj()->Set_Can_Play_Damage_Animations(true);
			obj->As_SoldierGameObj()->Set_Movement_Loiters_Allowed(true);
			obj->As_SoldierGameObj()->Set_Human_Anim_Override(true);
			obj->As_SoldierGameObj()->Set_Override_Weapon_Hold_Style(-1);
			if (forceDefinedWeapons && Has_Weapon(obj,enterWeapon))
				Commands->Select_Weapon(obj,enterWeapon);
			if (Has_Weapon(obj,defaultWeaponPreset))
				Remove_Weapon(obj,defaultWeaponPreset);
			obj->As_SoldierGameObj()->Set_Max_Speed(defaultSpeed);
			if (_stricmp(swimmingSkin,""))
				Set_Skin(obj,originalSkin);
			if (_stricmp(swimmingArmor,""))
				Commands->Set_Shield_Type(obj,originalArmor);
			if (_stricmp(swimmingModel,""))
				Commands->Set_Model(obj,originalModel);
			if (abs(swimmingHeightScale-999.99f) > 0.1f)
				obj->As_SoldierGameObj()->Set_Skeleton_Height(originalHeightScale);
			if (abs(swimmingWidthScale-999.99f) > 0.1f)
				obj->As_SoldierGameObj()->Set_Skeleton_Width(originalWidthScale);
		}
	}
	if (message == 347341)
	{
		lastWaterZoneId = Commands->Get_ID(sender);
		if (!waterZoneCount)
		{
			if (enterWaterMessageStringId)
			{
				time_t currentTime = clock();
				if (difftime(currentTime,lastDisplayTime) > 10000.0f)
				{
					Set_HUD_Help_Text_Player(obj,enterWaterMessageStringId,waterEnterMessageColorRGB);
					lastDisplayTime = currentTime;
				}
			}
			if (abs(swimmingHeightScale-999.99f) > 0.1f)
			{
				originalHeightScale = obj->As_SoldierGameObj()->Get_Skeleton_Heigth();
				obj->As_SoldierGameObj()->Set_Skeleton_Height(swimmingHeightScale);
			}
			if (abs(swimmingWidthScale-999.99f) > 0.1f)
			{
				originalWidthScale = obj->As_SoldierGameObj()->Get_Skeleton_Width();
				obj->As_SoldierGameObj()->Set_Skeleton_Width(swimmingWidthScale);
			}
			defaultSpeed = obj->As_SoldierGameObj()->Get_Max_Speed();
			waterSpeedMultiplier = (param/100.0f);
			JMG_Utility_Swimming_Infantry::isInWater[playerId] = true;
			sprintf(enterWeapon,"%s",Get_Current_Weapon(obj) ? Get_Current_Weapon(obj) : "");
			if (forceDefinedWeapons)
				Grant_Weapon(obj,defaultWeaponPreset,true,-1,1);
			obj->As_SoldierGameObj()->Set_Can_Play_Damage_Animations(false);
			obj->As_SoldierGameObj()->Set_Movement_Loiters_Allowed(false);
			obj->As_SoldierGameObj()->Set_Human_Anim_Override(false);
			currentWeaponId = 1;
			SwitchWeapon(obj);
			Commands->Set_Loiters_Allowed(obj,false);
			if (_stricmp(swimmingSkin,""))
			{
				sprintf(originalSkin,"%s",Get_Skin(obj));
				Set_Skin(obj,swimmingSkin);
			}
			if (_stricmp(swimmingArmor,""))
			{
				sprintf(originalArmor,"%s",Get_Shield_Type(obj));
				Commands->Set_Shield_Type(obj,swimmingArmor);
			}
			if (_stricmp(swimmingModel,""))
			{
				sprintf(originalModel,"%s",Get_Model(obj));
				Commands->Set_Model(obj,swimmingModel);
			}
		}
		waterZoneCount++;
	}
}
void JMG_Utility_Swimming_Infantry_Advanced::Killed(GameObject *obj,GameObject *killer)
{
	DestroySoundEmitter(&heartBeatSoundId);
	DestroySoundEmitter(&pantSoundId);
	if (!waterZoneCount)
		return;
	if (startedFadeRed)
	{
		Set_Screen_Fade_Color_Player(obj,1.0f,0.0f,0.0f,JmgUtility::MathClamp(defaultDrownTime-drownTime,0,startDrownSequence));
		CreateSoundEmitter(obj,heartBeatSoundEmitterModel,&heartBeatSoundId);
	}
	else
		Set_Screen_Fade_Color_Player(obj,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.X,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.Y,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.Z,0.0f);
	Set_Screen_Fade_Opacity_Player(obj,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColorOpacity,0.1f);
	Set_Fog_Range_Player(obj,JMG_Utility_Swimming_Zone::waterNode[playerId].waterMinViewDistance,JMG_Utility_Swimming_Zone::waterNode[playerId].waterMaxViewDistance,0.1f);
}
void JMG_Utility_Swimming_Infantry_Advanced::Destroyed(GameObject *obj)
{
	JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Player(obj,0.0f);
	Set_Fog_Range_Player(obj,JMG_Utility_Swimming_Zone::fogMinDistance,JMG_Utility_Swimming_Zone::fogMaxDistance,0.0f);
	DestroySoundEmitter(&heartBeatSoundId);
	DestroySoundEmitter(&pantSoundId);
}
void JMG_Utility_Swimming_Infantry_Advanced::Detach(GameObject *obj)
{
	if (Exe == 4)
		return;
	Destroyed(obj);
}
void JMG_Utility_Swimming_Infantry_Advanced::CreateSoundEmitter(GameObject *obj,const char *model,int *soundId)
{
	GameObject *soundEmitter = Commands->Find_Object(*soundId);
	if (!soundEmitter)
	{
		soundEmitter = Commands->Create_Object("Daves Arrow",Commands->Get_Position(obj));
		*soundId = Commands->Get_ID(soundEmitter);
		Commands->Attach_To_Object_Bone(soundEmitter,obj,"c head");
	}
	Commands->Set_Model(soundEmitter,model);
}
void JMG_Utility_Swimming_Infantry_Advanced::HideSoundEmitter(int *soundId)
{
	GameObject *soundEmitter = Commands->Find_Object(*soundId);
	if (soundEmitter)
		Commands->Set_Model(soundEmitter,"null");
}
void JMG_Utility_Swimming_Infantry_Advanced::DestroySoundEmitter(int *soundId)
{
	GameObject *soundEmitter = Commands->Find_Object(*soundId);
	if (soundEmitter)
		Commands->Destroy_Object(soundEmitter);
	*soundId = 0;
}
void JMG_Utility_Swimming_Infantry_Advanced::SwitchWeapon(GameObject *obj)
{
	const WeaponDefinitionClass *weaponDef = Get_Current_Weapon_Definition(obj);
	if ((weaponDef && currentWeaponId != weaponDef->Get_ID()) || (!weaponDef && currentWeaponId != 1))
	{
		int currentPosition = GetWeaponPosition(obj,currentWeaponId);
		GetWeaponId(weaponDef);
		if (forceDefinedWeapons)
		{
			int newPosition = GetWeaponPosition(obj,currentWeaponId);
			bool forward = newPosition > currentPosition;
			int diff = abs(newPosition-currentPosition);
			if ((diff <= 1 && forward) || (diff > 1 && weaponSwitchForward))
				currentWeapon = JMG_Utility_Swimming_Infantry_Advanced_Controller::getNext(obj,weaponGroupId,currentWeapon);
			else
				currentWeapon = JMG_Utility_Swimming_Infantry_Advanced_Controller::getPrev(obj,weaponGroupId,currentWeapon);
			weaponSwitchForward = forward;
			if (currentWeapon)
			{
				Commands->Select_Weapon(obj,currentWeapon->weaponName);
				GetWeaponId(Get_Current_Weapon_Definition(obj));
			}
		}
		else
			currentWeapon = JMG_Utility_Swimming_Infantry_Advanced_Controller::getWeapon(weaponGroupId,currentWeaponId);
		if (currentWeapon)
		{
			obj->As_SoldierGameObj()->Set_Override_Weapon_Hold_Style(currentWeapon->holdStyle);
			obj->As_SoldierGameObj()->Set_Max_Speed(defaultSpeed*currentWeapon->speed*waterSpeedMultiplier);
		}
		else
		{
			obj->As_SoldierGameObj()->Set_Override_Weapon_Hold_Style(defaultHoldStyle);
			obj->As_SoldierGameObj()->Set_Max_Speed(defaultSpeed*defaultSwimSpeedMultiplier*waterSpeedMultiplier);
		}
	}
}
void JMG_Utility_Swimming_Infantry_Advanced::GetWeaponId(const WeaponDefinitionClass *weaponDef)
{
	if (weaponDef)
		currentWeaponId = weaponDef->Get_ID();
	else
		currentWeaponId = 1;
}
int JMG_Utility_Swimming_Infantry_Advanced::GetWeaponPosition(GameObject *obj,int weaponId)
{
	if (ArmedGameObj *armedGameObj = obj->As_PhysicalGameObj()->As_ArmedGameObj())
	{
		WeaponBagClass *weaponBagClass = armedGameObj->Get_Weapon_Bag();
		int x = weaponBagClass->Get_Count();
		for (int i = 0;i < x;i++)
			if (weaponBagClass->Peek_Weapon(i))
				if (weaponBagClass->Peek_Weapon(i)->Get_ID() == weaponId)
					return i;
	}
	return -1;
}
JMG_Utility_Swimming_Infantry_Advanced_Controller::WeaponNode *JMG_Utility_Swimming_Infantry_Advanced_Controller::weaponNodeGroups[128] = {NULL};
JMG_Utility_Swimming_Infantry_Advanced_Controller::WeaponNode *JMG_Utility_Swimming_Infantry_Advanced_Controller::weaponNodeGroupsEnd[128] = {NULL};
bool JMG_Utility_Swimming_Infantry_Advanced_Controller::exists = false;
void JMG_Utility_Swimming_Infantry_Advanced_Controller::Destroyed(GameObject *obj)
{
	exists = false;
	Empty_List();
}
void JMG_Utility_Swimming_Infantry_Advanced_Controller::Detach(GameObject *obj)
{
	Destroyed(obj);
}
void JMG_Utility_Swimming_Infantry_Advanced_Add_All_Of_Style::Created(GameObject *obj)
{
	if (!JMG_Utility_Swimming_Infantry_Advanced_Controller::exists)
	{
		Console_Input("msg JMG_Utility_Swimming_Infantry_Advanced_Add_All_Of_Style ERROR: JMG_Utility_Swimming_Infantry_Advanced_Controller is required to be placed on the map!");
		Destroy_Script();
		return;
	}
	float delay = Get_Float_Parameter("Delay");
	if (delay)
		Commands->Start_Timer(obj,this,delay,1);
	else
		Timer_Expired(obj,1);
}
void JMG_Utility_Swimming_Infantry_Advanced_Add_All_Of_Style::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		int weaponGroupId = Get_Int_Parameter("WeaponGroupID");
		if (weaponGroupId < 0 || weaponGroupId > 127)
		{
			Console_Input("msg JMG_Utility_Swimming_Infantry_Advanced_Add_All_Of_Style ERROR: WeaponGroupID must be 0-127!");
			Destroy_Script();
			return;
		}
		int holdStyle = Get_Int_Parameter("HoldStyle");
		int animHoldStyle = Get_Int_Parameter("AnimHoldStyle");
		float movementSpeed = Get_Float_Parameter("MovementSpeed");
		for (WeaponDefinitionClass *weaponDef = (WeaponDefinitionClass *)DefinitionMgrClass::Get_First(0xB001);weaponDef != NULL;weaponDef = (WeaponDefinitionClass *)DefinitionMgrClass::Get_Next(weaponDef,0xB001))
			if (weaponDef->Style == holdStyle)
				JMG_Utility_Swimming_Infantry_Advanced_Controller::addNode(weaponGroupId,weaponDef->Get_Name(),weaponDef->Get_ID(),weaponDef->KeyNumber,animHoldStyle,movementSpeed);
	}
}
void JMG_Utility_Swimming_Infantry_Advanced_Add_Weapon::Created(GameObject *obj)
{
	if (!JMG_Utility_Swimming_Infantry_Advanced_Controller::exists)
	{
		Console_Input("msg JMG_Utility_Swimming_Infantry_Advanced_Add_Weapon ERROR: JMG_Utility_Swimming_Infantry_Advanced_Controller is required to be placed on the map!");
		Destroy_Script();
		return;
	}
	float delay = Get_Float_Parameter("Delay");
	if (delay)
		Commands->Start_Timer(obj,this,delay,1);
	else
		Timer_Expired(obj,1);
}
void JMG_Utility_Swimming_Infantry_Advanced_Add_Weapon::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		int weaponGroupId = Get_Int_Parameter("WeaponGroupID");
		if (weaponGroupId < 0 || weaponGroupId > 127)
		{
			Console_Input("msg JMG_Utility_Swimming_Infantry_Advanced_Add_Weapon ERROR: WeaponGroupID must be 0-127!");
			Destroy_Script();
			return;
		}
		const char *weapName = Get_Parameter("WeaponName");
		int animHoldStyle = Get_Int_Parameter("AnimHoldStyle");
		float movementSpeed = Get_Float_Parameter("MovementSpeed");
		for (WeaponDefinitionClass *weaponDef = (WeaponDefinitionClass *)DefinitionMgrClass::Get_First(0xB001);weaponDef != NULL;weaponDef = (WeaponDefinitionClass *)DefinitionMgrClass::Get_Next(weaponDef,0xB001))
			if (!_stricmp(weaponDef->Get_Name(),weapName))
			{
				JMG_Utility_Swimming_Infantry_Advanced_Controller::addNode(weaponGroupId,weaponDef->Get_Name(),weaponDef->Get_ID(),weaponDef->KeyNumber,animHoldStyle,movementSpeed);
				return;
			}
		char errorMsg[220];
		sprintf(errorMsg,"msg JMG_Utility_Swimming_Infantry_Advanced_Add_Weapon ERROR: The weapon definition %s could not be found!",weapName);
		Console_Input(errorMsg);
	}
}
void JMG_Utility_Custom_Send_Cycled_Customs::Created(GameObject *obj)
{
	cycle = 0;
	recieveMessage = Get_Int_Parameter("Custom");
	id = Get_Int_Parameter("ID");
	for (int x = 0;x < 10;x++)
	{
		char parameterName[128];
		sprintf(parameterName,"SendCustom%0d",x);
		custom[x] = Get_Int_Parameter(parameterName);
		sprintf(parameterName,"Param%0d",x);
		Param[x] = Get_Int_Parameter(parameterName);
		sprintf(parameterName,"Delay%0d",x);
		delay[x] = Get_Float_Parameter(parameterName);
	}
	randomDelay = Get_Float_Parameter("RandomDelay");
	randomChance = Get_Float_Parameter("RandomChance");
}
void JMG_Utility_Custom_Send_Cycled_Customs::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		if (randomChance && randomChance < Commands->Get_Random(0.0f,1.0f))
			return;
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
 		Commands->Send_Custom_Event(obj,object,custom[cycle],Param[cycle] == -1 ? param : Param[cycle],delay[cycle]+(randomDelay > 0 ? Commands->Get_Random(0.0f,randomDelay) : 0.0f));
		cycle++;
		if (!custom[cycle])
			cycle = 0;
	}
}
void JMG_Utility_Killed_Send_Custom_From_Killer::Killed(GameObject *obj,GameObject *killer)
{
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? (id == -1 ? killer : Commands->Find_Object(id)) : obj;
	Commands->Send_Custom_Event(killer,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	Destroy_Script();
}
void JMG_Utility_Emulate_Sound_Heard_On_FDS::Created(GameObject *obj)
{
	if (!The_Game()->Is_Dedicated())
	{
		Destroy_Script();
		return;
	}
	if (!obj->As_SoldierGameObj())
	{
		Console_Input("msg JMG_Utility_Emulate_Sound_Heard_On_FDS::ERROR Only attach to soldiers!");
		Destroy_Script();
		return;
	}
	crouchSoundId = Get_Int_Parameter("DefaultCrouchMoveSoundID");
	walkSoundId = Get_Int_Parameter("DefaultWalkMoveSoundID");
	runSoundId = Get_Int_Parameter("DefaultRunMoveSoundID");
	updateRate = Get_Float_Parameter("UpdateRate");
	currentWeapon = WeaponNode(obj);
	vehicleWeapon = WeaponNode();
	Commands->Start_Timer(obj,this,updateRate,1);
}
void JMG_Utility_Emulate_Sound_Heard_On_FDS::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && Commands->Get_Health(obj))
	{
		CombatSound sound;
		sound.Creator = obj;
		sound.sound = 0;
		sound.Position = Commands->Get_Position(obj);
		WeaponNode::FireWeaponReturn weaponFire = FiredWeapon(obj);
		if (weaponFire == WeaponNode::FireWeaponReturn::INFANTRY_FIRE)
		{
			const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(currentWeapon.weaponName,true);
			if (ammo)
				sound.sound = ammo->FireSoundDefID;
		}
		else if (weaponFire == WeaponNode::FireWeaponReturn::VEHICLE_FIRE)
		{
			const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(vehicleWeapon.weaponName,true);
			if (ammo)
				sound.sound = ammo->FireSoundDefID;
		}
		else
		{
			Vector3 tempSpeed;
			obj->As_SoldierGameObj()->Get_Velocity(tempSpeed);
			if (tempSpeed.Length2() > 0)
			{
				SoldierGameObj *solderGameObj = obj->As_SoldierGameObj();
				if (solderGameObj->Is_Crouched())
					sound.sound = crouchSoundId;
				else if (solderGameObj->Is_Slow())
					sound.sound = walkSoundId;
				else
					sound.sound = runSoundId;
			}
		}
		if (sound.sound)
		{
			AudibleSoundDefinitionClass *soundDef = (AudibleSoundDefinitionClass *)DefinitionMgrClass::Find_Definition(sound.sound,true);
			if (soundDef)
			{
				float soundRangeSquared = soundDef->Get_Logical_DropOff_Radius()*soundDef->Get_Logical_DropOff_Radius();
				sound.sound = soundDef->Get_Logical_Type();
				for (SLNode<SmartGameObj> *node = GameObjManager::SmartGameObjList.Head();node;node = node->Next())
				{
					SmartGameObj* object = node->Data();
					if (object == obj)
						continue;
					if (JmgUtility::SimpleDistance(Commands->Get_Position(object),sound.Position) > soundRangeSquared)
						continue;
					const SimpleDynVecClass<GameObjObserverClass *>& observer_list = object->Get_Observers();
					for(int index = 0;index < observer_list.Count();index++)
						observer_list[index]->Sound_Heard(object,sound);
				}
			}
		}
		Commands->Start_Timer(obj,this,updateRate,1);
	}
}
JMG_Utility_Emulate_Sound_Heard_On_FDS::WeaponNode::FireWeaponReturn JMG_Utility_Emulate_Sound_Heard_On_FDS::FiredWeapon(GameObject *obj)
{
	if (_stricmp(currentWeapon.weaponName,Get_Current_Weapon(obj)))
	{
		currentWeapon = WeaponNode(obj);
		return WeaponNode::FireWeaponReturn::NONE;
	}
	GameObject *vehicle = Get_Vehicle(obj);
	if (vehicle)
	{
		int currentBullets = vehicleWeapon.GetWeaponBullets(vehicle);
		if (currentBullets < vehicleWeapon.totalBullets)
		{
			vehicleWeapon.totalBullets = currentBullets;
			return WeaponNode::FireWeaponReturn::VEHICLE_FIRE;
		}
		return WeaponNode::FireWeaponReturn::NONE;
	}
	int currentBullets = currentWeapon.GetWeaponBullets(obj);
	if (currentBullets < currentWeapon.totalBullets)
	{
		currentWeapon.totalBullets = currentBullets;
		return WeaponNode::FireWeaponReturn::INFANTRY_FIRE;
	}
	return WeaponNode::FireWeaponReturn::NONE;
}
void JMG_Utility_Send_Custom_When_Team_Dominates_Zone::Created(GameObject *obj)
{
	team = Get_Int_Parameter("Team");
	teamHoldingZone = sBool::sNULL;
	enabled = Get_Int_Parameter("StartsEnabled") != 0;
	enableCustom = Get_Int_Parameter("EnableCustom");
	id = Get_Int_Parameter("ID");
	captureCustom = Get_Int_Parameter("CaptureCustom");
	captureParam = Get_Int_Parameter("CaptureParam");
	lostCustom = Get_Int_Parameter("LostCustom");
	lostParam = Get_Int_Parameter("LostParam");
	delay = Get_Float_Parameter("CaptureDelay");
	rate = Get_Float_Parameter("Rate");
	sendCustomEveryTick = Get_Int_Parameter("SendCustomEveryTick")!=0;
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Send_Custom_When_Team_Dominates_Zone::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1)
	{
		if (enabled)
		{
			int teamCount = 0,enemyCount = 0;
			for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
			{
				SmartGameObj* o = current->Data();
				if (o != obj && Commands->Get_Health(o) && IsInsideZone(obj,o))
				{
					int myTeam = Commands->Get_Player_Type(o);
					if (myTeam == -2 || Is_Script_Attached(o,"JMG_Utility_Send_Custom_When_Team_Dominates_Zone_Ignore"))
						continue;
					if (myTeam == team)
						teamCount++;
					else
						enemyCount++;
				}
			}
			if (teamCount > enemyCount)
			{
				if (teamHoldingZone != sBool::sTRUE || sendCustomEveryTick)
				{
					GameObject *object = id ? Commands->Find_Object(id) : obj;
 					Commands->Send_Custom_Event(obj,object,captureCustom,captureParam == -1 ? teamCount : captureParam,delay);
					teamHoldingZone = sBool::sTRUE;
				}
			}
			else if (teamHoldingZone != sBool::sFALSE || sendCustomEveryTick)
			{
				GameObject *object = id ? Commands->Find_Object(id) : obj;
 				Commands->Send_Custom_Event(obj,object,lostCustom,lostParam == -1 ? teamCount : lostParam,delay);
				teamHoldingZone = sBool::sFALSE;
			}
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Send_Custom_When_Team_Dominates_Zone::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == enableCustom)
	{
		if (enabled != (param!=0))
			teamHoldingZone = sBool::sNULL;
		enabled = param!=0;
	}
}
void JMG_Utility_Send_Custom_When_Team_Zone::Created(GameObject *obj)
{
	team = Get_Int_Parameter("Team");
	teamInZone = sBool::sNULL;
	enabled = Get_Int_Parameter("StartsEnabled") != 0;
	enableCustom = Get_Int_Parameter("EnableCustom");
	id = Get_Int_Parameter("ID");
	inCustom = Get_Int_Parameter("InCustom");
	inParam = Get_Int_Parameter("InParam");
	outCustom = Get_Int_Parameter("OutCustom");
	outParam = Get_Int_Parameter("OutParam");
	delay = Get_Float_Parameter("Delay");
	rate = Get_Float_Parameter("Rate");
	sendCustomEveryTick = Get_Int_Parameter("SendCustomEveryTick") != 0;
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Send_Custom_When_Team_Zone::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1)
	{
		if (enabled)
		{
			int teamCount = 0;
			for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
			{
				SmartGameObj* o = current->Data();
				if (o != obj && Commands->Get_Health(o) && IsInsideZone(obj,o))
				{
					int myTeam = Commands->Get_Player_Type(o);
					if (myTeam == -2 || Is_Script_Attached(o,"JMG_Utility_Send_Custom_When_Team_Zone_Ignore"))
						continue;
					if (myTeam == team)
						teamCount++;
				}
			}
			if (teamCount > 0)
			{
				if (teamInZone != sBool::sTRUE || sendCustomEveryTick)
				{
					GameObject *object = id ? Commands->Find_Object(id) : obj;
 					Commands->Send_Custom_Event(obj,object,inCustom,inParam,delay);
					teamInZone = sBool::sTRUE;
				}
			}
			else
			{
				if (teamInZone != sBool::sFALSE || sendCustomEveryTick)
				{
					GameObject *object = id ? Commands->Find_Object(id) : obj;
 					Commands->Send_Custom_Event(obj,object,outCustom,outParam,delay);
					teamInZone = sBool::sFALSE;
				}
			}
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Send_Custom_When_Team_Zone::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == enableCustom)
	{
		if (enabled != (param!=0))
			teamInZone = sBool::sNULL;
		enabled = param!=0;
	}
}
void JMG_Utility_HUD_Count_Down_Visible_Countdown::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_HUD_Count_Down_Visible_Countdown::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		JMG_Utility_HUD_Count_Down::AddVisualCountdownNode(Commands->Get_ID(obj),Get_Int_Parameter("CharPos"),Get_Parameter("BaseModelName"));
	}
}
void JMG_Utility_Custom_Send_Custom_On_Secondary_Count::Created(GameObject *obj)
{
	triggerCustom = Get_Int_Parameter("TriggerCustom");
	countCustom = Get_Int_Parameter("CountCustom");
	count = Get_Int_Parameter("MatchCount");
	id = Get_Int_Parameter("ID");
	sendCustom = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	resetCustom = Get_Int_Parameter("ResetCountCustom");
	onceMatchedContinue = Get_Int_Parameter("OnceMatchedContinue") != 0;
}
void JMG_Utility_Custom_Send_Custom_On_Secondary_Count::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == resetCustom)
	{
		count = Get_Int_Parameter("Count");
	}
	if (message == countCustom)
	{
		if ((!onceMatchedContinue && count >= 0) || (onceMatchedContinue && count))
			count--;
	}
	if (message == triggerCustom && !count)
	{
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
		if (object)
		{
			Commands->Send_Custom_Event(obj,object,sendCustom,Param == -1 ? param : Param,delay);
		}
	}
}
void JMG_Utility_Send_Custom_When_Not_Team_Zone::Created(GameObject *obj)
{
	team = Get_Int_Parameter("Team");
	teamInZone = sBool::sNULL;
	enabled = Get_Int_Parameter("StartsEnabled") != 0;
	enableCustom = Get_Int_Parameter("EnableCustom");
	id = Get_Int_Parameter("ID");
	inCustom = Get_Int_Parameter("InCustom");
	inParam = Get_Int_Parameter("InParam");
	outCustom = Get_Int_Parameter("OutCustom");
	outParam = Get_Int_Parameter("OutParam");
	delay = Get_Float_Parameter("Delay");
	rate = Get_Float_Parameter("Rate");
	sendCustomEveryTick = Get_Int_Parameter("SendCustomEveryTick") != 0;
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Send_Custom_When_Not_Team_Zone::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1)
	{
		if (enabled)
		{
			int teamCount = 0;
			for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
			{
				SmartGameObj* o = current->Data();
				if (o != obj && Commands->Get_Health(o) && IsInsideZone(obj,o))
				{
					int myTeam = Commands->Get_Player_Type(o);
					if (myTeam == -2 || Is_Script_Attached(o,"JMG_Utility_Send_Custom_When_Not_Team_Zone_Ignore"))
						continue;
					if (myTeam != team)
						teamCount++;
				}
			}
			if (teamCount > 0)
			{
				if (teamInZone != sBool::sTRUE || sendCustomEveryTick)
				{
					GameObject *object = id ? Commands->Find_Object(id) : obj;
 					Commands->Send_Custom_Event(obj,object,inCustom,inParam,delay);
					teamInZone = sBool::sTRUE;
				}
			}
			else
			{
				if (teamInZone != sBool::sFALSE || sendCustomEveryTick)
				{
					GameObject *object = id ? Commands->Find_Object(id) : obj;
 					Commands->Send_Custom_Event(obj,object,outCustom,outParam,delay);
					teamInZone = sBool::sFALSE;
				}
			}
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Send_Custom_When_Not_Team_Zone::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == enableCustom)
	{
		if (enabled != (param!=0))
			teamInZone = sBool::sNULL;
		enabled = param!=0;
	}
}
void JMG_Utility_Send_Custom_When_Team_Dominates_Zone_Ignore::Created(GameObject *obj)
{
}
void JMG_Utility_Send_Custom_When_Team_Zone_Ignore::Created(GameObject *obj)
{
}
void JMG_Utility_Send_Custom_When_Not_Team_Zone_Ignore::Created(GameObject *obj)
{
}
void JMG_Utility_Send_Custom_On_Armor::Created(GameObject *obj)
{
	id = Get_Int_Parameter("ID");
	armorCustom = Get_Int_Parameter("ArmorCustom");
	armorParam = Get_Int_Parameter("ArmorParam");
	noArmorCustom = Get_Int_Parameter("NoArmorCustom");
	noArmorParam = Get_Int_Parameter("NoArmorParam");
	delay = Get_Float_Parameter("Delay");
	hurt = Commands->Get_Shield_Strength(obj) < Commands->Get_Max_Shield_Strength(obj);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Send_Custom_On_Armor::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (hurt && Commands->Get_Shield_Strength(obj) >= Commands->Get_Max_Shield_Strength(obj))
		{
			hurt = false;
			GameObject *object = id ? Commands->Find_Object(id) : obj;
 			Commands->Send_Custom_Event(obj,object,armorCustom,armorParam,delay);
		}
		if (!hurt && !Commands->Get_Shield_Strength(obj))
		{
			hurt = true;
			GameObject *object = id ? Commands->Find_Object(id) : obj;
 			Commands->Send_Custom_Event(obj,object,noArmorCustom,noArmorParam,delay);
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Base_Defense_Simple::Created(GameObject *obj)
{
	resetTime = 0;
	Commands->Enable_Enemy_Seen(obj,true);
	enemyId = 0;
	minDist = Get_Float_Parameter("MinAttackDistance")*Get_Float_Parameter("MinAttackDistance");
	maxDistance = Get_Float_Parameter("MaxAttackDistance");
	carTankBike = Get_Vector3_Parameter("AttackPriority[Car|Tank|Bike]");
	flyingTurretBoat = Get_Vector3_Parameter("AttackPriority[Flying|Turret|Boat]");
	submarineInfantryUnused = Get_Vector3_Parameter("AttackPriority[Submarine|Infantry|Unused]");
	userSetResetTime = (int)(Get_Float_Parameter("ResetTime")*10);
	maxDist = maxDistance*maxDistance;
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Base_Defense_Simple::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	GameObject *vehicle = Get_Vehicle(seen);
	if (vehicle)
		seen = vehicle;
	float seenPriority = GetPriority(seen);
	if (!seenPriority)
		return;
	if (!Commands->Get_Health(seen))
		return;
	float distance = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen));
	if (distance < minDist || distance > maxDist)
		return;
	int seenId = Commands->Get_ID(seen);
	if (!enemyId)
		AttackTarget(obj,seenId,seen);
	else if (enemyId != seenId)
	{
		GameObject *enemy = Commands->Find_Object(enemyId);
		if (!enemy || seenPriority > GetPriority(enemy))
			AttackTarget(obj,seenId,seen);

	}
	if (enemyId == seenId)
		resetTime = userSetResetTime;
}
void JMG_Utility_Base_Defense_Simple::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (enemyId)
		return;
	Enemy_Seen(obj,damager);
}
void JMG_Utility_Base_Defense_Simple::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (enemyId)
		{
			GameObject *enemy = Commands->Find_Object(enemyId);
			if (!enemy || !Commands->Get_Health(enemy))
			{
				enemyId = 0;
				Commands->Action_Reset(obj,100);
			}
		}
		if (resetTime)
		{
			resetTime--;
			if (!resetTime)
			{
				enemyId = 0;
				Commands->Action_Reset(obj,100);
			}
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
float JMG_Utility_Base_Defense_Simple::GetPriority(GameObject *seen)
{
	if (!seen)
		return 0;
	if (seen->As_VehicleGameObj())
		switch (Get_Vehicle_Mode(seen))
		{
			case VEHICLE_TYPE_CAR: return carTankBike.X;
			case VEHICLE_TYPE_TANK: return carTankBike.Y;
			case VEHICLE_TYPE_BIKE: return carTankBike.Z;
			case VEHICLE_TYPE_FLYING: return flyingTurretBoat.X;
			case VEHICLE_TYPE_TURRET: return flyingTurretBoat.Y;
			case VEHICLE_TYPE_BOAT: return flyingTurretBoat.Z;
			case VEHICLE_TYPE_SUB: return submarineInfantryUnused.X;
		}
	else if (seen->As_SoldierGameObj())
		return submarineInfantryUnused.Y;
	Console_Input("msg JMG_Utility_Base_Defense_Simple ERROR: Undefined vehicle type!");
	return 0;
}
void JMG_Utility_Base_Defense_Simple::AttackTarget(GameObject *obj,int seenId,GameObject *seen)
{
	enemyId = seenId;
	ActionParamsStruct params;
	params.Set_Basic(this,100,2);
	params.Set_Attack(seen,maxDistance,0.0,true);
	Commands->Action_Attack(obj,params);
	params.AttackCheckBlocked = false;
}
void JMG_Utility_Custom_Send_Custom_Supress_Spam::Created(GameObject *obj)
{
	char parameterName[128];
	for (int x = 0;x < 10;x++)
	{
		sprintf(parameterName,"Custom%0d",x);
		catchCustom[x] = Get_Int_Parameter(parameterName);
	}
	id = Get_Int_Parameter("ID");
	lastCustom = 0;
	lastParam = 0;
	lastSentCustom = 0;
	lastSentParam = 0;
	senderId = 0;
	spamDelay = Get_Float_Parameter("SpamDelay");
	sendDuplicates = Get_Int_Parameter("SendDuplicates") != 0;
	supressingSpam = false;
}
void JMG_Utility_Custom_Send_Custom_Supress_Spam::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		bool sent = false;
		if (sendDuplicates || (lastCustom != lastSentCustom || lastParam != lastSentParam))
			sent = SendCustom(obj);
		if (!sent)
			supressingSpam = false;
		else
			Commands->Start_Timer(obj,this,spamDelay,1);
	}
}
void JMG_Utility_Custom_Send_Custom_Supress_Spam::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (catchCustom[0] == 0 || message == catchCustom[0] || message == catchCustom[1] || message == catchCustom[2] || message == catchCustom[3] || message == catchCustom[4] || message == catchCustom[5] || message == catchCustom[6] || message == catchCustom[7] || message == catchCustom[8] || message == catchCustom[9])
	{
		senderId = Commands->Get_ID(sender);
		lastCustom = message;
		lastParam = param;
		if (!supressingSpam && (sendDuplicates || (message != lastSentCustom || param != lastSentParam)))
		{
			SendCustom(obj);
			supressingSpam = true;
			Commands->Start_Timer(obj,this,spamDelay,1);
		}
	}
}
bool JMG_Utility_Custom_Send_Custom_Supress_Spam::SendCustom(GameObject *obj)
{
	if (!lastCustom)
		return false;
	GameObject *sender = Commands->Find_Object(senderId);
	if (!sender)
		sender = obj;
	GameObject *object = id ? Commands->Find_Object(id) : obj;
 	Commands->Send_Custom_Event(sender,object,lastCustom,lastParam,0);
	lastSentCustom = lastCustom;
	lastSentParam = lastParam;
	senderId = 0;
	lastCustom = 0;
	lastParam = 0;
	return true;
}
void JMG_Utility_Send_Custom_When_Player_Between_Range::Created(GameObject *obj)
{
	sleeping = 0;
	minDistance = Get_Float_Parameter("MinDistance");
	minDistance *= minDistance;
	maxDistance = Get_Float_Parameter("MaxDistance");
	maxDistance *= maxDistance;
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Custom");
	param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	triggerOnce = Get_Int_Parameter("TriggerOnce") ? true : false;
	sleepTime = Get_Int_Parameter("SleepTime");
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Send_Custom_When_Player_Between_Range::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1)
	{
		if (sleeping)
			sleeping--;
		else
			for (int x = 1;x < 128;x++)
			{
				GameObject *player = Get_GameObj(x);
				if (!player)
					continue;
				float playerDist = JmgUtility::SimpleDistance(Commands->Get_Position(player),Commands->Get_Position(obj));
				if (playerDist <= maxDistance && playerDist >= minDistance)
				{
					GameObject *object = id ? Commands->Find_Object(id) : obj;
					if (object)
						Commands->Send_Custom_Event(obj,object,custom,param,delay);
					if (triggerOnce)
					{
						this->Destroy_Script();
						return;
					}
					sleeping = sleepTime;
				}
			}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Custom_Display_Briefing_PAMSG_Message_To_Player::Created(GameObject *obj)
{
	BriefingText = NULL;
	lastNode = NULL;
	int File = Commands->Text_File_Open(Get_Parameter("FileName"));
	if (!File)
	{
		Console_Input("msg JMG_Utility_Custom_Display_Briefing_PAMSG_Message_To_Player ERROR: Text File not found!");
		return;
	}
	bool NextLineIsTarget = false;
	char TextString[16384];
	while (Commands->Text_File_Get_String(File,TextString,16383))
	{
		if (!_stricmp(TextString,""))
			continue;
		if (!NextLineIsTarget)
			for (int x = 0;x < 16384;x++)
				if (TextString[x] == '\n' || TextString[x] == 13 || TextString[x] == '\0')
					TextString[x] = '\0';
		if (NextLineIsTarget)
		{
			int Count = 0;
			for (int x = 0;x < 16384;x++)
			{
				if ((Count > 140 && TextString[x] == ' ') || Count == 200)
				{// String getting too long, start a new one.
					lastNode->Text[Count] = '\0';
					AddNewTextNode();
					Count = 0;
				}
				else if (TextString[x] == '\n' || TextString[x] == '\0')
				{
					lastNode->Text[Count] = '\0';
					break;// end of line your done
				}
				else if (TextString[x] == '\f')
				{
					lastNode->Text[Count] = '\0';
					AddNewTextNode();
					Count = 0;
				}
				else
				{
					lastNode->Text[Count] = TextString[x];
					Count++;
				}
			}
			break;
		}
		else if (!_stricmp(TextString,Get_Parameter("TextTag")))
		{
			NextLineIsTarget = true;
			AddNewTextNode();
		}
		TextString[0] = '\0';
	}
	Commands->Text_File_Close(File);
}
void JMG_Utility_Custom_Display_Briefing_PAMSG_Message_To_Player::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		if (!Commands->Is_A_Star(sender))
			return;
		for (BriefingTextNode *node = lastNode;node;node = node->prev)
		{
			char tmpMsg[220];
			sprintf(tmpMsg,"pamsg %d %s",JmgUtility::JMG_Get_Player_ID(sender),node->Text);
			Console_Input(tmpMsg);
		}
	}
}
void JMG_Utility_Custom_Display_Briefing_PAMSG_Message_To_Player::Destroyed(GameObject *obj)
{
	RemoveTextNodes();
}
void JMG_Utility_Custom_Display_Briefing_PAMSG_Message_To_Player::AddNewTextNode()
{
	if (!BriefingText)
	{
		BriefingText = new BriefingTextNode;
		sprintf(BriefingText->Text,"\0");
		BriefingText->next = NULL;
		lastNode = BriefingText;
		return;
	}
	BriefingTextNode *Current = BriefingText;
	while (Current)
	{
		if (!Current->next)
		{
			Current->next = new BriefingTextNode;
			Current->next->prev = Current;
			Current = Current->next;
			sprintf(Current->Text,"\0");
			Current->next = NULL;
			lastNode = Current;
			return;
		}
		Current = Current->next;
	}
}
void JMG_Utility_Custom_Display_Briefing_PAMSG_Message_To_Player::RemoveTextNodes()
{
	BriefingTextNode *Current = BriefingText,*Temp;
	while (Current)
	{
		Temp = Current;
		Current = Current->next;
		delete Temp;
	}
	BriefingText = NULL;
	Destroy_Script();
}
void JMG_Utility_Send_Custom_On_Powerup_Pickup_Collector::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		int id = Get_Int_Parameter("ID");
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
		if (object)
			Commands->Send_Custom_Event(sender,object,Get_Int_Parameter("Message"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	}
}
void JMG_Utility_Zone_Send_Custom_Enter_From_Enterer::Created(GameObject *obj)
{
	playerType = Get_Int_Parameter("PlayerType");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Custom");
	param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	triggerOnce = Get_Int_Parameter("TriggerOnce") ? true : false;
}
void JMG_Utility_Zone_Send_Custom_Enter_From_Enterer::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,playerType) || Commands->Get_Player_Type(enter) == -4)
		return;
	GameObject *object = id ? (id == -1 ? enter : Commands->Find_Object(id)) : obj;
	if (object)
		Commands->Send_Custom_Event(enter,object,custom,param,delay);
	if (triggerOnce)
		Destroy_Script();
}
void JMG_Utility_AI_Lobbed_Vehicle_Projectile_Custom::Created(GameObject *obj)
{
	targetCustom = Get_Int_Parameter("TargetCustom");
	minDistanceSquared = Get_Float_Parameter("MinDistance");
	minDistanceSquared *= minDistanceSquared;
	minAngle = Get_Float_Parameter("MinAngle");
	maxAngle = Get_Float_Parameter("MaxAngle");
	lowAngleMaxDistance = Get_Float_Parameter("UseLowAngleMaxDistance");
	lowAngleMaxDistance *= lowAngleMaxDistance;
	useLowAngleWhenAboveMinDistance = Get_Int_Parameter("UseLowAngleWhenAboveMinDistance") != 0;
	useLowAngleTargetAboveHeight = Get_Float_Parameter("UseLowAngleTargetAboveHeight");
	sprintf(vehicleProjectilePreset,"%s",Get_Parameter("VehicleProjectilePreset"));
	fireVelocity = Get_Float_Parameter("FireVelocity");
	gravityScale = Get_Float_Parameter("GravityScale");
	missAmountPerMeter = Get_Float_Parameter("MissAmountPerMeter");
	baseMissAmount = Get_Float_Parameter("BaseMissAmount");
	fireDelay = Get_Float_Parameter("FireRate") == 0.0f ? 9999.9f : 1.0f/Get_Float_Parameter("FireRate");
	reloadTime = Get_Float_Parameter("ReloadTime");
	currentClipCount = clipCount = max(0,Get_Int_Parameter("ClipCount"));
	velocitySquared = fireVelocity*fireVelocity;
	sprintf(fireSound,"%s",Get_Parameter("FireSound"));
	sprintf(projectileExplosion,"%s",Get_Parameter("ProjectileExplosion"));
	sprintf(muzzleFlashExplosion,"%s",Get_Parameter("MuzzleFlashExplosion"));
	sprintf(reloadSound,"%s",Get_Parameter("ReloadSound"));
	aimTurret = Get_Int_Parameter("AimTurret") != 0;
	refreshTime = Get_Int_Parameter("CustomTimeoutTime")*10;
	projectedShotsChance = Get_Float_Parameter("ProjectedShotsChance");
	if (Commands->Get_Random(0.0f,0.99999f) < projectedShotsChance)
		projectShots = true;
	delayComplete = true;
	reloadComplete = true;
	lastAngle = 9999.0f;
	Commands->Disable_All_Collisions(obj);
	Commands->Start_Timer(obj,this,0.01f,1);
	Commands->Start_Timer(obj,this,0.1f,4);
}
void JMG_Utility_AI_Lobbed_Vehicle_Projectile_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (targetCustom == message)
	{
		if (!targetId)
			targetId = param;
		if (param == targetId)
			timeoutTime = refreshTime;
	}
}
void JMG_Utility_AI_Lobbed_Vehicle_Projectile_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		GameObject *seen = Commands->Find_Object(targetId);
		if (!seen || !Commands->Get_Health(seen))
			targetId = 0;
		if (reloadComplete && delayComplete && targetId)
		{
			Vector3 myPos = Commands->Get_Bone_Position(obj,"FireSpot"),enemyPos = Commands->Get_Bone_Position(seen,"target");
			if (seen->As_SmartGameObj() && projectShots)
			{
				double tmp = (fireVelocity*sin(lastAngle));
				float flightTime = (float)((fireVelocity*sin(lastAngle)+sqrt(tmp*tmp+2*(gravityScale*9.8)*(myPos.Z-enemyPos.Z)))/(gravityScale*9.8));
				Vector3 speed;
				seen->As_SmartGameObj()->Get_Velocity(speed);
				if (flightTime < 600.0f)
				{
					enemyPos.X += speed.X*flightTime;
					enemyPos.Y += speed.Y*flightTime;
					enemyPos.Z += speed.Z*flightTime;
				}
			}
			float targetDist = JmgUtility::SimpleDistance(myPos,enemyPos),targetFlatDistance = JmgUtility::SimpleFlatDistance(myPos,enemyPos);
			if (targetDist < minDistanceSquared)
			{
				Commands->Start_Timer(obj,this,0.01f,1);
				return;
			}
			float targetMissAmount = 0.0f;
			if (missAmountPerMeter)
			{
				float missAmount = Commands->Get_Distance(enemyPos,myPos)*missAmountPerMeter;
				targetMissAmount = missAmount < baseMissAmount ? baseMissAmount : missAmount;
			}
			else if (baseMissAmount)
				targetMissAmount = baseMissAmount;
			if (targetMissAmount)
			{
				float rotation = Commands->Get_Random(-180.0f,180.0f);
				enemyPos.X += cos(rotation)*Commands->Get_Random(0.0f,targetMissAmount);
				enemyPos.Y += sin(rotation)*Commands->Get_Random(0.0f,targetMissAmount);
			}
			bool useHighAngle = true;
			if (targetDist <= lowAngleMaxDistance)
				useHighAngle = false;
			else if (useLowAngleWhenAboveMinDistance && targetFlatDistance <= minDistanceSquared)
				useHighAngle = false;
			else if (useLowAngleTargetAboveHeight && Commands->Get_Position(obj).Z+useLowAngleTargetAboveHeight < Commands->Get_Position(seen).Z)
				useHighAngle = false;
			double height = enemyPos.Z-myPos.Z,angle;
			myPos.Z = enemyPos.Z = 0.0f;
			if (!CalculateAngle(&angle,Commands->Get_Distance(enemyPos,myPos),height,useHighAngle))
			{
				Commands->Start_Timer(obj,this,0.01f,1);
				return;
			}
			lastAngle = angle;
			double zRotation = atan2(enemyPos.Y-myPos.Y,enemyPos.X-myPos.X);
			FireProjectile(obj,zRotation,angle);
			if (currentClipCount)
				currentClipCount--;
			if (!currentClipCount)
			{
				reloadComplete = false;
				Commands->Start_Timer(obj,this,reloadTime,2);
				Commands->Create_3D_Sound_At_Bone(reloadSound,obj,"Turret");
			}
			else
			{
				delayComplete = false;
				Commands->Start_Timer(obj,this,fireDelay,3);
			}
			if (aimTurret)
			{
				float TempRotation = atan2(enemyPos.Y-myPos.Y,enemyPos.X-myPos.X);
				Vector3 pos = Commands->Get_Bone_Position(obj,"barrel");
				pos.X += (float)cos(TempRotation);
				pos.Y += (float)sin(TempRotation);
				pos.Z += (float)tan(angle);
				ActionParamsStruct params;
				params.Set_Attack(pos,0.0f,0,1);
				params.AttackForceFire = false;
				params.AttackCheckBlocked = false;
				Commands->Action_Attack(obj,params);
			}
		}
		Commands->Start_Timer(obj,this,0.01f,1);
	}
	if (number == 2)
	{
		if (Commands->Get_Random(0.0f,0.99999f) < projectedShotsChance)
			projectShots = true;
		else
			projectShots = false;
		currentClipCount = clipCount;
		reloadComplete = true;
	}
	if (number == 3)
	{
		delayComplete = true;
	}
	if (number == 4)
	{
		if (timeoutTime)
		{
			timeoutTime--;
			if (!timeoutTime)
				targetId = 0;
		}
		Commands->Start_Timer(obj,this,0.1f,4);
	}
}
bool JMG_Utility_AI_Lobbed_Vehicle_Projectile_Custom::CalculateAngle(double *returnAngle,double distance,double height,bool highAngle)
{
	double calculatedGravity = gravityScale*9.8;
	if (distance <= 0.0)
		return false;
	double x = velocitySquared*velocitySquared-calculatedGravity*(calculatedGravity*(distance*distance)+2*height*velocitySquared);
	if (x < 0)
		return false;
	*returnAngle = atan((velocitySquared+(highAngle ? 1 : -1)*sqrt(x))/(calculatedGravity*distance));
	if (*returnAngle*180.0/PI < minAngle || *returnAngle*180.0/PI > maxAngle)
		return false;
	return true;
}
void JMG_Utility_AI_Lobbed_Vehicle_Projectile_Custom::FireProjectile(GameObject *obj,double zAngle,double aimAngle)
{
	Commands->Create_3D_Sound_At_Bone(fireSound,obj,"MuzzleA0");
	GameObject *projectile = Commands->Create_Object(vehicleProjectilePreset,Commands->Get_Bone_Position(obj,"FireSpot"));
	Commands->Create_Explosion(muzzleFlashExplosion,Commands->Get_Bone_Position(obj,"muzzleA0"),obj);
	char params[220];
	sprintf(params,"%d,%s",Commands->Get_ID(obj),projectileExplosion);
	Commands->Attach_Script(projectile,"JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_Attach",params);
	Vector3 normalizedVector = Vector3((float)cos(zAngle),(float)sin(zAngle),(float)tan(aimAngle));
	normalizedVector.Normalize();
	normalizedVector.X *= fireVelocity;
	normalizedVector.Y *= fireVelocity;
	normalizedVector.Z *= fireVelocity;
	Set_Velocity(projectile,normalizedVector);
}
void JMG_Utility_Vehicle_Enter_Send_Custom_From_Enterer::Created(GameObject *obj)
{
	team = Get_Int_Parameter("PlayerType");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Custom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	triggerOnce = Get_Int_Parameter("TriggerOnce") != 0;
}
void JMG_Utility_Vehicle_Enter_Send_Custom_From_Enterer::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (CheckPlayerType(sender,team))
			return;
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
		Commands->Send_Custom_Event(sender,object,custom,Param,delay);
		if (triggerOnce)
			Destroy_Script();
	}
}
void JMG_Utility_Custom_Send_Custom_On_Count_From_Sender::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	resetCustom = Get_Int_Parameter("ResetCustom");
	count = Get_Int_Parameter("Count");
}
void JMG_Utility_Custom_Send_Custom_On_Count_From_Sender::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == resetCustom)
	{
		count = Get_Int_Parameter("Count");
	}
	if (message == custom && count)
	{
		count--;
		if (count)
			return;
		int id = Get_Int_Parameter("ID");
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
		if (object)
		{
			int Param = Get_Int_Parameter("Param");
			Commands->Send_Custom_Event(sender,object,Get_Int_Parameter("SendCustom"),Param == -1 ? param : Param,Get_Float_Parameter("Delay"));
		}
	}
}
void JMG_Utility_Player_Seen_Send_Custom::Created(GameObject *obj)
{
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Custom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	triggerOnce = Get_Int_Parameter("TriggerOnce") != 0;
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Player_Seen_Send_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player || Commands->Get_Player_Type(player) == -4)
				continue;
			SmartGameObj *smartGameObject = player->As_SmartGameObj();
			if (!smartGameObject || !smartGameObject->Is_Visible())
				continue;
			if (obj->As_SmartGameObj()->Is_Obj_Visible(smartGameObject))
			{
				GameObject *object = id ? (id == -1 ? player : Commands->Find_Object(id)) : obj;
				Commands->Send_Custom_Event(obj,object,custom,Param == -1 ? Commands->Get_ID(player) : Param,delay);
				if (triggerOnce)
					Destroy_Script();
			}
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Enter_Play_Animation_And_Relocate::Created(GameObject *obj)
{
	sprintf(presetName,"%s",Get_Parameter("PresetName"));
	sprintf(animation,"%s",Get_Parameter("Animation"));
	sprintf(attachedModel,"%s",Get_Parameter("AttachedModel"));
	sprintf(attachedAnimation,"%s",Get_Parameter("AttachedAnimation"));
	centerToZone = Get_Int_Parameter("CenterToZone") ? true : false;
	custom = Get_Int_Parameter("Custom");
	customParam = Get_Int_Parameter("Param");
	OBBoxClass *zone = Get_Zone_Box(obj);
	if (!zone)
	{
		Console_Input("msg JMG_Utility_Enter_Play_Animation_And_Relocate ERROR: Must be attached to a script zone!");
		Destroy_Script();
		return;
	}
	zoneRotation = zone->Basis.Get_Z_Rotation()*(180/PI);
}
void JMG_Utility_Enter_Play_Animation_And_Relocate::Entered(GameObject *obj,GameObject *enterer)
{
	if (Is_Script_Attached(enterer,"JMG_Utility_Enter_Play_Animation_And_Relocate_Attached"))
		return;
	if (!enterer->As_SmartGameObj())
		return;
	if (!_stricmp(presetName,"") || !_stricmp(presetName,Commands->Get_Preset_Name(enterer)))
	{
		if (centerToZone)
			Commands->Set_Position(enterer,Commands->Get_Position(obj));
		if (enterer->As_SoldierGameObj())
		{
			enterer->As_SoldierGameObj()->Set_Override_Weapon_Hold_Style(14);
			enterer->As_SoldierGameObj()->Set_Can_Play_Damage_Animations(false);
		}
		Commands->Set_Animation(enterer,animation,false,NULL,0.0f,-1.0f,true);
		char params[512];
		sprintf(params,"%s,%s,%.2f,%d,%d",attachedModel,attachedAnimation,zoneRotation,custom,customParam);
		Attach_Script_Once(enterer,"JMG_Utility_Enter_Play_Animation_And_Relocate_Attached",params);
	}
}
void JMG_Utility_Enter_Play_Animation_And_Relocate_Attached::Created(GameObject *obj)
{
	GameObject *holder = Commands->Create_Object("Daves Arrow",Commands->Get_Position(obj));
	Commands->Set_Model(holder,Get_Parameter("AttachedModel"));
	Commands->Set_Animation(holder,Get_Parameter("AttachedAnimation"),false,NULL,0.0f,-1.0f,true);
	Commands->Set_Facing(holder,Get_Float_Parameter("Facing"));
	Commands->Attach_To_Object_Bone(obj,holder,"attach");
	holderId = Commands->Get_ID(holder);
}
void JMG_Utility_Enter_Play_Animation_And_Relocate_Attached::Animation_Complete(GameObject *obj,const char *anim)
{
	if (obj->As_SoldierGameObj())
	{
		obj->As_SoldierGameObj()->Set_Override_Weapon_Hold_Style(-1);
		obj->As_SoldierGameObj()->Set_Can_Play_Damage_Animations(true);
		Commands->Action_Reset(obj,100.0f);
	}
	obj->As_PhysicalGameObj()->Peek_Physical_Object()->Set_Collision_Group(Collision_Group_Type::SOLDIER_GHOST_COLLISION_GROUP);
	Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),0);
	Destroyed(obj);
	Destroy_Script();
}
void JMG_Utility_Enter_Play_Animation_And_Relocate_Attached::Destroyed(GameObject *obj)
{
	GameObject *holder = Commands->Find_Object(holderId);
	if (holder)
		Commands->Destroy_Object(holder);
}
void JMG_Utility_Custom_Destroy_Closest_Object_To_Self::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Destroy_Closest_Object_To_Self::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		GameObject *nearest = NULL;
		float dist = 0.0f;
		Vector3 pos = Commands->Get_Position(obj);
		const char *presetName = Get_Parameter("PresetName");
		float maxDist = Get_Float_Parameter("MaxDistance");
		maxDist *= maxDist;
		for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
		{
			GameObject* o = current->Data()->As_ScriptableGameObj();
			if (!_stricmp(presetName,Commands->Get_Preset_Name(o)))
			{
				float tempDistance = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o));
				if (!nearest || (tempDistance < dist && (maxDist == 0.0 || tempDistance <= maxDist)))
				{
					nearest = o;
					dist = tempDistance;
				}
			}
		}
		if (nearest)
			Commands->Destroy_Object(nearest);
		if (Get_Int_Parameter("TriggerOnce"))
			Destroy_Script();
	}
}
void JMG_Utility_Custom_Apply_Damage_Closest_Object_To_Self::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Apply_Damage_Closest_Object_To_Self::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		GameObject *nearest = NULL;
		float dist = 0.0f;
		Vector3 pos = Commands->Get_Position(obj);
		const char *presetName = Get_Parameter("PresetName");
		float maxDist = Get_Float_Parameter("MaxDistance");
		maxDist *= maxDist;
		for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
		{
			GameObject* o = current->Data()->As_ScriptableGameObj();
			if (!_stricmp(presetName,Commands->Get_Preset_Name(o)))
			{
				float tempDistance = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o));
				if (!nearest || (tempDistance < dist && (maxDist == 0.0 || tempDistance <= maxDist)))
				{
					nearest = o;
					dist = tempDistance;
				}
			}
		}
		if (nearest)
			Commands->Apply_Damage(nearest,Get_Float_Parameter("DamageAmount"),Get_Parameter("Warhead"),sender);
		if (Get_Int_Parameter("TriggerOnce"))
			Destroy_Script();
	}
}
void JMG_Utility_Custom_Restore_Building::Created(GameObject *obj)
{
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Custom");
	triggerOnce = Get_Int_Parameter("TriggerOnce") ? true : false;
}
void JMG_Utility_Custom_Restore_Building::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		GameObject *building = obj;
		if (id)
			if (id < 0)
				building = sender;
			else
				building = Commands->Find_Object(id);
		Restore_Building(building);
		if (triggerOnce)
			Destroy_Script();
	}
}
void JMG_Utility_Timer_Custom_Random::Created(GameObject *obj)
{
	time = Get_Float_Parameter("Time");
	random = Get_Float_Parameter("Random");
	repeat = Get_Int_Parameter("Repeat") ? true : false;
	id = Get_Int_Parameter("ID");
	message = Get_Int_Parameter("Message");
	param = Get_Int_Parameter("Param");
	Commands->Start_Timer(obj,this,time+(random ? Commands->Get_Random(-random,random) : 0.0f),1);
}
void JMG_Utility_Timer_Custom_Random::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		GameObject *object = !id ? obj : Commands->Find_Object(id);
		if (object)
			Commands->Send_Custom_Event(obj,object,message,param,0);
		if (repeat)
			Commands->Start_Timer(obj,this,time+(random ? Commands->Get_Random(-random,random) : 0.0f),1);
	}
}
DynamicAttachScript JMG_Utility_Dynamic_Script_Controller::dynamicAttachScript;
void JMG_Utility_Dynamic_Script_Controller::Destroyed(GameObject *obj)
{
	JMG_Utility_Dynamic_Script_Controller::dynamicAttachScript.EmptyList();
}
void JMG_Utility_Dynamic_Script_Definition::Created(GameObject *obj)
{
	JMG_Utility_Dynamic_Script_Controller::dynamicAttachScript.AddDynamicScript(Get_Int_Parameter("ScriptID"),Get_Parameter("ScriptName"));
	Destroy_Script();
}
void JMG_Utility_Dynamic_Script_Created_Add_Update_Parameter::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.01f,1);
}
void JMG_Utility_Dynamic_Script_Created_Add_Update_Parameter::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		const char *parameters = Get_Parameter("Param");
		char *params = newstr(parameters);
		char delim = Get_Parameter("Delim")[0];
		if (delim != '\0')
		{
			unsigned int length = strlen(parameters);
			for (unsigned int x = 0;x < length;x++)
				if (params[x] == delim)
					params[x] = ',';
		}
		JMG_Utility_Dynamic_Script_Controller::dynamicAttachScript.AddParamToScript(Get_Int_Parameter("ScriptID"),Get_Int_Parameter("Index"),params);
		Destroy_Script();
	}
}
void JMG_Utility_Dynamic_Script_Custom_Add_Update_Parameter::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Dynamic_Script_Custom_Add_Update_Parameter::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		const char *parameters = Get_Parameter("Param");
		char *params = newstr(parameters);
		char delim = Get_Parameter("Delim")[0];
		if (delim != '\0')
		{
			unsigned int length = strlen(parameters);
			for (unsigned int x = 0;x < length;x++)
				if (params[x] == delim)
					params[x] = ',';
		}
		JMG_Utility_Dynamic_Script_Controller::dynamicAttachScript.AddParamToScript(Get_Int_Parameter("ScriptID"),Get_Int_Parameter("Index"),params);
	}
}
void JMG_Utility_Dynamic_Script_Custom_Attach::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Dynamic_Script_Custom_Attach::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		DynamicAttachScript::DynamicAttachScriptNode *script = JMG_Utility_Dynamic_Script_Controller::dynamicAttachScript.FindDynamicScript(Get_Int_Parameter("ScriptID"));
		if (!script)
			return;
		char attachParams[2048];
		sprintf(attachParams,"");
		int addedParams = 0;
		for (int x = 0;x < 128 && addedParams < script->dynamicAttachScriptParams.objectCount;x++)
		{
			DynamicAttachScript::DynamicAttachScriptParams::DynamicAttachScriptParamNode *scriptParam = script->dynamicAttachScriptParams.FindParam(x);
			if (scriptParam)
			{
				sprintf(attachParams,"%s%s%s",attachParams,addedParams ? "," : "",scriptParam->param);
				addedParams++;
			}
		}
		Commands->Attach_Script(sender,script->scriptName,attachParams);
	}
}
void JMG_Utility_Objective_System_Custom_Add_Objective_Send_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Objective_System_Custom_Add_Objective_Send_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		if (!JMG_Utility_Objective_System_Controller::controllerPlaced)
		{
			Console_Input("msg JMG_Utility_Objective_System_Custom_Add_Objective_Send_Custom ERROR: Make sure you have a JMG_Utility_Objective_System_Controller placed on the map!");
			return;
		}
		if (Get_Int_Parameter("CompleteObjectiveID") && BasicObjectiveSystem.Get_Objective_Status(Get_Int_Parameter("CompleteObjectiveID")) != NewObjectiveSystem::Failed)
		{
			BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
			BasicObjectiveSystem.Set_Objective_Status(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Accomplished);
		}
		int objectiveMarkerId = Get_Int_Parameter("ObjectiveMarkerObjectID");
		GameObject *ObjectiveMarker = BasicObjectiveSystem.GetObjectiveMarker(objectiveMarkerId,sender,Get_Int_Parameter("NewObjectiveID"));
		if (!ObjectiveMarker && objectiveMarkerId)
		{
			Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Custom ERROR: Could not find objective marker object!");
			return;
		}
		int status = BasicObjectiveSystem.Get_Objective_Status(Get_Int_Parameter("NewObjectiveID"));
		if (status == NewObjectiveSystem::NotDefined || status == NewObjectiveSystem::Removed)
		{
			int id = Get_Int_Parameter("ID");
			int Param = Get_Int_Parameter("Param");
			GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
			Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("SendCustom"),Param == -1 ? param : Param,Get_Float_Parameter("CustomDelay"));
			bool added = false;
			if (ObjectiveMarker)
				added = BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"),ObjectiveMarker);
			else
				added = BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"));
			if (added)
				JmgUtility::SetHUDHelpText(Get_Int_Parameter("NewObjectiveStringID"),BasicObjectiveSystem.Get_Hud_Help_Text_Color(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority")));
		}
	}
}
void JMG_Utility_Objective_System_Custom_Send_Custom_Status::Created(GameObject *obj)
{
	objectiveId = Get_Int_Parameter("ObjectiveID");
	status = Get_Int_Parameter("Status");
	recieveMessage = Get_Int_Parameter("Custom");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
}
void JMG_Utility_Objective_System_Custom_Send_Custom_Status::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		if (BasicObjectiveSystem.Get_Objective_Status(objectiveId) != status)
			return;
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
 		Commands->Send_Custom_Event(obj,object,custom,Param == -1 ? param : Param,delay);
	}
}
void JMG_Utility_Objective_System_Custom_Send_Custom_Does_Not_Exist::Created(GameObject *obj)
{
	objectiveId = Get_Int_Parameter("ObjectiveID");
	recieveMessage = Get_Int_Parameter("Custom");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
}
void JMG_Utility_Objective_System_Custom_Send_Custom_Does_Not_Exist::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		int status = BasicObjectiveSystem.Get_Objective_Status(objectiveId);
		if (status != NewObjectiveSystem::NotDefined && status != NewObjectiveSystem::Removed)
			return;
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
 		Commands->Send_Custom_Event(obj,object,custom,Param == -1 ? param : Param,delay);
	}
}
void JMG_Utility_Objective_System_Send_Random_Objective_As_Custom::Created(GameObject *obj)
{
	for (int x = 0;x < 10;x++)
	{
		char objectiveName[15];
		sprintf(objectiveName,"ObjectiveID%0d",x);
		objectiveId[x] = Get_Int_Parameter(objectiveName);
	}
	recieveMessage = Get_Int_Parameter("Custom");
	id = Get_Int_Parameter("ID");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
}
void JMG_Utility_Objective_System_Send_Random_Objective_As_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		int random = Commands->Get_Random_Int(1,10);
		int originalRandom = random;
		int tmpObjectiveId = -1;
		for (int x = 0;;x++)
		{
			if (x == 10)
			{
				if (random == originalRandom)
					return;
				x = 0;
			}
			if (objectiveId[x] == -999)
				continue;
			int status = BasicObjectiveSystem.Get_Objective_Status(objectiveId[x]);
			if (status != NewObjectiveSystem::NotDefined && status != NewObjectiveSystem::Removed)
				continue;
			random--;
			if (!random)
			{
				tmpObjectiveId = objectiveId[x];
				break;
			}
		}
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
 		Commands->Send_Custom_Event(obj,object,tmpObjectiveId,Param == -1 ? param : Param,delay);
	}
}
void JMG_Utility_Objective_System_Custom_Send_Custom_Not_Status::Created(GameObject *obj)
{
	objectiveId = Get_Int_Parameter("ObjectiveID");
	status = Get_Int_Parameter("Status");
	recieveMessage = Get_Int_Parameter("Custom");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
}
void JMG_Utility_Objective_System_Custom_Send_Custom_Not_Status::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		if (BasicObjectiveSystem.Get_Objective_Status(objectiveId) == status)
			return;
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
 		Commands->Send_Custom_Event(obj,object,custom,Param == -1 ? param : Param,delay);
	}
}
GlobalKeycardSystem globalKeycardSystem = GlobalKeycardSystem();
void GlobalKeycardSystem::AddKeycardToPlayerObjects(int keycardId,int groupId)
{
	for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
	{
		SmartGameObj* o = current->Data();
		if (o)
		{
			JMG_Utility_Global_Keycard_System_Soldier *keycardScript = (JMG_Utility_Global_Keycard_System_Soldier*)Find_Script_On_Object(o,"JMG_Utility_Global_Keycard_System_Soldier");
			if (keycardScript && keycardScript->groupId == groupId)
				Commands->Grant_Key(o,keycardId,true);
		}
	}
}
void GlobalKeycardSystem::RemoveKeycardFromPlayerObjects(int keycardId,int groupId)
{
	for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
	{
		SmartGameObj* o = current->Data();
		if (o)
		{
			JMG_Utility_Global_Keycard_System_Soldier *keycardScript = (JMG_Utility_Global_Keycard_System_Soldier*)Find_Script_On_Object(o,"JMG_Utility_Global_Keycard_System_Soldier");
			if (keycardScript && keycardScript->groupId == groupId)
				Commands->Grant_Key(o,keycardId,false);
		}
	}
}
void JMG_Utility_Global_Keycard_System_Controller::Destroyed(GameObject *obj)
{
	globalKeycardSystem.emptyList();
}
void JMG_Utility_Global_Keycard_System_Created_Add_Keycard::Created(GameObject *obj)
{
	globalKeycardSystem.AddKeycard(Get_Int_Parameter("KeycardID"),Get_Int_Parameter("GroupID"));
}
void JMG_Utility_Global_Keycard_System_Custom_Add_Keycard::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Global_Keycard_System_Custom_Add_Keycard::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		globalKeycardSystem.AddKeycard(Get_Int_Parameter("KeycardID"),Get_Int_Parameter("GroupID"));
	}
}
void JMG_Utility_Global_Keycard_System_Custom_Remove_Keycard::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Global_Keycard_System_Custom_Remove_Keycard::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		globalKeycardSystem.RemoveKeycard(Get_Int_Parameter("KeycardID"),Get_Int_Parameter("GroupID"));
	}
}
void JMG_Utility_Global_Keycard_System_Soldier::Created(GameObject *obj)
{
	groupId = Get_Int_Parameter("GroupID");
	globalKeycardSystem.GrantKeycards(obj,groupId);
}
void JMG_Utility_Zone_Send_Custom_On_Player_Occupation_Change::Created(GameObject *obj)
{
	occupiedId = Get_Int_Parameter("OccupiedID");
	occupiedCustom = Get_Int_Parameter("OccupiedCustom");
	occupiedParam = Get_Int_Parameter("OccupiedParam");
	occupiedDelay = Get_Float_Parameter("OccupiedDelay");
	vacantId = Get_Int_Parameter("VacantID");
	vacantCustom = Get_Int_Parameter("VacantCustom");
	vacantParam = Get_Int_Parameter("VacantParam");
	vacantDelay = Get_Float_Parameter("VacantDelay");
}
void JMG_Utility_Zone_Send_Custom_On_Player_Occupation_Change::Entered(GameObject *obj,GameObject *enterer)
{
	if (!Commands->Is_A_Star(enterer))
		return;
	int playerId = JmgUtility::JMG_Get_Player_ID(enterer);
	if (inZone[playerId])
		return;
	inZone[playerId] = true;
	if (!count && occupiedCustom)
	{
		GameObject *object = occupiedId ? (occupiedId == -1 ? enterer : Commands->Find_Object(occupiedId)) : obj;
 		Commands->Send_Custom_Event(obj,object,occupiedCustom,occupiedParam,occupiedDelay);
	}
	count++;
	char params[128];
	sprintf(params,"%d",Commands->Get_ID(obj));
	Commands->Attach_Script(enterer,"JMG_Utility_Zone_Send_Custom_On_Player_Occupation_Change_Attached",params);
}
void JMG_Utility_Zone_Send_Custom_On_Player_Occupation_Change::Exited(GameObject *obj,GameObject *exiter)
{
	if (!Commands->Is_A_Star(exiter))
		return;
	int playerId = JmgUtility::JMG_Get_Player_ID(exiter);
	if (!inZone[playerId])
		return;
	inZone[playerId] = false;
	count--;
	if (!count && vacantCustom)
	{
		GameObject *object = vacantId ? (vacantId == -1 ? exiter : Commands->Find_Object(vacantId)) : obj;
 		Commands->Send_Custom_Event(obj,object,vacantCustom,vacantParam,vacantDelay);
	}
}
void JMG_Utility_Zone_Send_Custom_On_Player_Occupation_Change_Attached::Destroyed(GameObject *obj)
{
	GameObject *controller = Commands->Find_Object(Get_Int_Parameter("ID"));
	if (!controller)
		return;
	JMG_Utility_Zone_Send_Custom_On_Player_Occupation_Change *occupationScript = (JMG_Utility_Zone_Send_Custom_On_Player_Occupation_Change*)Find_Script_On_Object(controller,"JMG_Utility_Zone_Send_Custom_On_Player_Occupation_Change");
	if (!occupationScript)
		return;
	occupationScript->Exited(controller,obj);
}
bool JMG_Utility_Silent_Countdown_Controller::controllerPlaced = false;
JMG_Utility_Silent_Countdown_Controller::SendCustomIdNode *JMG_Utility_Silent_Countdown_Controller::sendCustomIdNodeList = NULL;
void JMG_Utility_Silent_Countdown_Controller::Created(GameObject *obj)
{
	controllerPlaced = true;
}
void JMG_Utility_Silent_Countdown_Controller::Destroyed(GameObject *obj)
{
	EmptyList();
}
void JMG_Utility_Silent_Countdown_Timer::Created(GameObject *obj)
{
	paused = true;
	sendCustomIdNode = JMG_Utility_Silent_Countdown_Controller::FindOrCreateCountdownId(Get_Int_Parameter("TimerID"));
	time = Get_Int_Parameter("Time");
	startCustom = Get_Int_Parameter("StartCustom");
	pausedCustom = Get_Int_Parameter("PausedCustom");
	cancelCustom = Get_Int_Parameter("CancelCustom");
}
void JMG_Utility_Silent_Countdown_Timer::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number && JMG_Utility_Silent_Countdown_Controller::controllerPlaced && time)
	{
		time--;
		JMG_Utility_Silent_Countdown_Controller::NodeSendCustom(obj,sendCustomIdNode,time);
		if (!paused)
			Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_Silent_Countdown_Timer::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (startCustom == message)
	{
		if (paused)
			Commands->Start_Timer(obj,this,1.0f,1);
		paused = false;
	}
	if (pausedCustom == message)
	{
		Stop_Timer(obj,1);
		paused = true;
	}
	if (cancelCustom == message)
	{
		Stop_Timer(obj,1);
		paused = true;
		time = Get_Int_Parameter("Time");
	}
}
void JMG_Utility_Silent_Countdown_Send_Custom::Created(GameObject *obj)
{
	int id = Get_Int_Parameter("ID");
	id = id ? id : Commands->Get_ID(obj);
	JMG_Utility_Silent_Countdown_Controller::AddSecondNode(Get_Int_Parameter("TimerID"),Get_Int_Parameter("TriggerTime"),id,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
}
int JMG_Utility_Zone_Teleport_To_Random_WP_Boss::BossObjectId = 0;
void JMG_Utility_Zone_Teleport_To_Random_WP_Boss::Created(GameObject *obj)
{
	retryOnFailure = Get_Int_Parameter("RetryOnFailure") ? true : false;
	changeGroupIDCustom = Get_Int_Parameter("ChangeGroupIDCustom");
	playerType = Get_Int_Parameter("PlayerType");
	safeTeleportDistance = Get_Float_Parameter("SafeTeleportDistance");
	wanderPointGroup = Get_Int_Parameter("WanderingAIGroupID");
	aiOnly = Get_Int_Parameter("AiOnly") != 0;
	faceBoss = Get_Int_Parameter("FaceBoss") ? true : false;
}
void JMG_Utility_Zone_Teleport_To_Random_WP_Boss::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == changeGroupIDCustom)
	{
		wanderPointGroup = param;
	}
}
void JMG_Utility_Zone_Teleport_To_Random_WP_Boss::Entered(GameObject *obj,GameObject *enter)
{
	if (aiOnly && Commands->Is_A_Star(enter))
		return;
	if (CheckPlayerType(enter,playerType))
		return;
	Grab_Teleport_Spot(enter,5);
}
bool JMG_Utility_Zone_Teleport_To_Random_WP_Boss::Grab_Teleport_Spot(GameObject *enter,int attempts)
{
	if (The_Game()->Get_Game_Duration_S() < 1.0f)
	{
		char params[220];
		sprintf(params,"%d,%.2f",wanderPointGroup,safeTeleportDistance);
		Commands->Attach_Script(enter,"JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach",params);
		return false;
	}
	if (!attempts)
	{
		if (retryOnFailure)
		{
			char params[220];
			sprintf(params,"%d,%.2f",wanderPointGroup,safeTeleportDistance);
			Commands->Attach_Script(enter,"JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach",params);
		}
		return false;
	}
	attempts--;
	Vector3 targetPos;
	float facing;
	GameObject *boss = Commands->Find_Object(JMG_Utility_Zone_Teleport_To_Random_WP_Boss::BossObjectId);
	if (!Get_A_Defense_Point(&targetPos,&facing,boss))
	{
		char errormsg[220];
		sprintf(errormsg,"msg JMG_Utility_Zone_Teleport_To_Random_Wander_Point ERROR: No wander points found for group %d!",wanderPointGroup);
		Console_Input(errormsg);
		return false;
	}
	MoveablePhysClass *mphys = enter->As_PhysicalGameObj() ? enter->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	if (mphys && mphys->Find_Teleport_Location(targetPos,safeTeleportDistance,&targetPos))
	{
		Toggle_Fly_Mode(enter);
		Commands->Set_Position(enter,targetPos);
		Force_Position_Update(enter);
		Toggle_Fly_Mode(enter);
		if (faceBoss)
		{
			Vector3 bossPos = Commands->Get_Position(boss);
			bossPos.X -= targetPos.X;
			bossPos.Y -= targetPos.Y;
			facing = atan2(bossPos.Y,bossPos.X) * 180 / PI;
		}
		char params[220];
		sprintf(params,"0.1,%.2f",facing);
		Commands->Attach_Script(enter,"JMG_Utility_Delay_Then_Rotate_Camera",params);
		return true;
	}
	else
	{
		Commands->Set_Position(enter,targetPos);
		return Grab_Teleport_Spot(enter,attempts);
	}
}
void JMG_Utility_Zone_Teleport_To_Random_WP_Boss_Object::Created(GameObject *obj)
{
	JMG_Utility_Zone_Teleport_To_Random_WP_Boss::BossObjectId = Commands->Get_ID(obj);
}
void JMG_Utility_Custom_Set_Soldier_Speed::Created(GameObject *obj)
{
	if (!obj->As_SoldierGameObj())
	{
		Console_Input("msg JMG_Utility_Custom_Set_Soldier_Speed ERROR: Script only works on soldiers!");
		Destroy_Script();
		return;
	}
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Set_Soldier_Speed::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		obj->As_SoldierGameObj()->Set_Max_Speed(Get_Float_Parameter("Speed"));
	}
}
SList<JMG_Utility_Control_Point_Controller::ControlPointTeamData> JMG_Utility_Control_Point_Controller::controlPointTeamData;
SList<GameObject> JMG_Utility_Control_Point_Controller::controlPoints;
SList<GameObject> JMG_Utility_Control_Point_Controller::wanderPoints;
bool JMG_Utility_Control_Point_Controller::controllerPlaced = false;
bool JMG_Utility_Control_Point_Controller::allSetupComplete = false;
void JMG_Utility_Control_Point_Controller::Created(GameObject *obj)
{
	JMG_Utility_Control_Point_Controller::controlPointTeamData.Add_Tail(new JMG_Utility_Control_Point_Controller::ControlPointTeamData(-2,Get_Parameter("NeutralTeamName"),Get_Int_Parameter("NeutralPlayerType"),Get_Int_Parameter("NeutralRadarBlipColor"),Get_Parameter("NeutralDefaultPointModel"),Get_Parameter("NeutralDefaultAnimation"),Get_Float_Parameter("NeutralAnimationLength"),Get_Parameter("NeutralCaptureSound"),Get_Parameter("NeutralLostSound"),Get_Parameter("NeutralLockedModel"),Get_Parameter("NeutralLockedAnim"),Get_Float_Parameter("NeutralLockedAnimLength"),Get_Int_Parameter("NeutralDefenseMultiplier"),Get_Int_Parameter("NeutralCaptureMultiplier"),Get_Int_Parameter("NeutralUnoccupiedMultiplier"),Get_Int_Parameter("NeutralCustomID"),Get_Int_Parameter("NeutralTeamMemberCaptureCustom"),Get_Int_Parameter("NeutralTeamMembmerLostCustom"),Get_Int_Parameter("NeutralTeamCaptureCustom"),Get_Int_Parameter("NeutralTeamLostCustom"),Get_Int_Parameter("NeutralTeamMemberNeutralize")));
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Control_Point_Controller::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
		{
			GameObject* o = current->Data()->As_ScriptableGameObj();
			if (!Is_Script_Attached(o,"JMG_Utility_Control_Point"))
				continue;
			JMG_Utility_Control_Point *cpScript = (JMG_Utility_Control_Point*)Find_Script_On_Object(o,"JMG_Utility_Control_Point");
			if (!cpScript || !cpScript->setupComplete)
			{
				Commands->Start_Timer(obj,this,0.1f,1);
				return;
			}
		}
		allSetupComplete = true;
	}
}
void JMG_Utility_Control_Point_Controller::Destroyed(GameObject *obj)
{
	controllerPlaced = false;
	controlPointTeamData.Remove_All();
	controlPoints.Remove_All();
	wanderPoints.Remove_All();
	allSetupComplete = false;
}
void JMG_Utility_Control_Point_Team_Setting::Created(GameObject *obj)
{
	if (!JMG_Utility_Control_Point_Controller::controllerPlaced)
	{
		Console_Input("msg JMG_Utility_Control_Point_Team_Setting ERROR: JMG_Utility_Control_Point_Controller must be placed on the map!");
		return;
	}
	if (Get_Int_Parameter("TeamID") == -2)
	{
		Console_Input("msg JMG_Utility_Control_Point_Team_Setting ERROR: Team -2 is reserved for team neutral!");
		return;
	}
	JMG_Utility_Control_Point_Controller::controlPointTeamData.Add_Tail(new JMG_Utility_Control_Point_Controller::ControlPointTeamData(Get_Int_Parameter("TeamID"),Get_Parameter("TeamName"),Get_Int_Parameter("PlayerType"),Get_Int_Parameter("RadarBlipColor"),Get_Parameter("PointModel"),Get_Parameter("Animation"),Get_Float_Parameter("AnimationLength"),Get_Parameter("CaptureSound"),Get_Parameter("LostSound"),Get_Parameter("LockedModel"),Get_Parameter("LockedAnim"),Get_Float_Parameter("LockedAnimLength"),Get_Int_Parameter("DefenseMultiplier"),Get_Int_Parameter("CaptureMultiplier"),Get_Int_Parameter("UnoccupiedMultiplier"),Get_Int_Parameter("CustomID"),Get_Int_Parameter("TeamMemberCaptureCustom"),Get_Int_Parameter("TeamMembmerLostCustom"),Get_Int_Parameter("TeamCaptureCustom"),Get_Int_Parameter("TeamLostCustom"),Get_Int_Parameter("TeamMemberNeutralize")));
	Destroy_Script();
}
void JMG_Utility_Control_Point::Created(GameObject *obj)
{
	occupied = false;
	sprintf(controlPointName,"%s",Get_Parameter("ControlPointName"));
	controllingTeamId = Get_Int_Parameter("ControllingTeamID");
	maxCapturePoints = Get_Int_Parameter("MaxCapturePoints");
	controlDistance = Get_Float_Parameter("ControlDistance");
	controlDistance *= controlDistance;
	controlHeightMinMax = Get_Vector3_Parameter("ControlHeight[Min|Max]");
	zoneId = Get_Int_Parameter("ZoneID");
	captureScore = Get_Float_Parameter("CaptureScore");
	neutralizeScore = Get_Float_Parameter("NeutralizeScore");
	currerntCapturePoints = controllingTeamId != -2 ? maxCapturePoints : 0;
	currentFrame = 0;
	captured = controllingTeamId != -2 ? true : false;
	lockCustom = Get_Int_Parameter("LockCustom");
	specialLocked = Get_Int_Parameter("StartsLocked") ? true : false;
	locked = false;
	cpGroupId = Get_Int_Parameter("ControlGroupID");
	controllingTeamOverride = NULL;
	controllingTeam = NULL;
	wasLocked = false;
	radarBipType = obj->As_PhysicalGameObj()->Get_Radar_Blip_Shape_Type();
	JMG_Utility_Control_Point_Controller::controlPoints.Add_Head(obj);
	Commands->Set_Is_Visible(obj,false);
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_Control_Point::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == lockCustom)
	{
		specialLocked = param ? true : false;
		if (!controllingTeam)
			return;
		ChangeModelAndTeam(obj);
		UpdateAnimation(obj);
	}
}
void JMG_Utility_Control_Point::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		UpdateTeam(controllingTeamId);
		UpdateTeamOverride(controllingTeamId);
		for (SLNode<JMG_Utility_Control_Point_Controller::ControlPointTeamData> *node = JMG_Utility_Control_Point_Controller::controlPointTeamData.Head();node != NULL;node = node->Next())
			if (JMG_Utility_Control_Point_Controller::ControlPointTeamData *data = node->Data())
				if (data->teamId == -2)
				{
					neutralTeam = data;
					break;
				}
		if (!controllingTeam)
		{
			char errorMsg[220];
			sprintf(errorMsg,"msg JMG_Utility_Control_Point ERROR: Team %d has not been defined!",controllingTeamId);
			Console_Input(errorMsg);
			return;
		}
		Final_CP_Calculation(obj);
		Commands->Set_Player_Type(obj,controllingTeam->playerType);
		setupComplete = true;
		if (!zoneId)
			Timer_Expired(obj,2);
		else
			Timer_Expired(obj,3);
	}
	if (2 == number)
	{
		Vector3 pos = Commands->Get_Position(obj);
		occupied = false;
		if (!locked && !specialLocked)
			for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
			{
				SmartGameObj* o = current->Data();
				if (!o)
					continue;
				Vector3 unitPos = Commands->Get_Position(o);
				if (controlHeightMinMax.X && (pos.Z + controlHeightMinMax.X) > unitPos.Z)
					continue;
				if (controlHeightMinMax.Y && (pos.Z + controlHeightMinMax.Y) < unitPos.Z)
					continue;
				if (JmgUtility::SimpleDistance(pos,unitPos) > controlDistance)
					continue;
				JMG_Utility_Control_Point_Team_Member *unitScript = (JMG_Utility_Control_Point_Team_Member*)Find_Script_On_Object(o,"JMG_Utility_Control_Point_Team_Member");
				if (!unitScript || !unitScript->setupComplete)
					continue;
				AddUpCpPoints(obj,unitScript->teamData,unitScript->multiplier);
			}
		ChangeModelAndTeam(obj);
		Final_CP_Calculation(obj);
		Commands->Start_Timer(obj,this,0.25f,2);
	}
	if (3 == number)
	{
		GameObject *zone = Commands->Find_Object(zoneId);
		if (!zone)
		{
			Console_Input("msg JMG_Utility_Control_Point ERROR: Zone not found!");
			return;
		}
		Vector3 pos = Commands->Get_Position(obj);
		occupied = false;
		if (!locked && !specialLocked)
			for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
			{
				SmartGameObj* o = current->Data();
				if (!o)
					continue;
				Vector3 unitPos = Commands->Get_Position(o);
				if (!IsInsideZone(zone,o))
					continue;
				JMG_Utility_Control_Point_Team_Member *unitScript = (JMG_Utility_Control_Point_Team_Member*)Find_Script_On_Object(o,"JMG_Utility_Control_Point_Team_Member");
				if (!unitScript || !unitScript->setupComplete)
					continue;
				AddUpCpPoints(obj,unitScript->teamData,unitScript->multiplier);
			}
		ChangeModelAndTeam(obj);
		Final_CP_Calculation(obj);
		Commands->Start_Timer(obj,this,0.25f,3);
	}
}
void JMG_Utility_Control_Point::AddUpCpPoints(GameObject *obj,JMG_Utility_Control_Point_Controller::ControlPointTeamData *unitTeamData,int multiplier)
{
	if (unitTeamData->teamId != controllingTeamId)
		currerntCapturePoints -= unitTeamData->captureMultiplier*multiplier;
	if (unitTeamData->teamId == controllingTeamId)
		currerntCapturePoints += unitTeamData->defenseMultiplier*multiplier;
	if (currerntCapturePoints < 0 && !captured)
	{
		currerntCapturePoints = abs(currerntCapturePoints);
		controllingTeamId = unitTeamData->teamId;
	}
	if (!currerntCapturePoints && !captured)
		controllingTeamId = neutralTeam->teamId;
	occupied = true;
}
void JMG_Utility_Control_Point::Final_CP_Calculation(GameObject *obj)
{
	if (!occupied)
	{
		if (captured)
			currerntCapturePoints += controllingTeam->unoccupiedMultiplier;
		else
			currerntCapturePoints += neutralTeam->unoccupiedMultiplier;
	}
	if (currerntCapturePoints > maxCapturePoints)
	{
		if (!captured)
		{
			captured = true;
			UpdateTeam(controllingTeamId);
			UpdateTeamOverride(controllingTeamId);
			ChangeModelAndTeam(obj);
			CaptureControlPoint(obj);
		}
		currerntCapturePoints = maxCapturePoints;
	}
	if (currerntCapturePoints < 0)
	{
		lastTeam = controllingTeam;
		lastTeamOverride = controllingTeamOverride;
		currerntCapturePoints = 0;
		controllingTeam = neutralTeam;
		UpdateTeamOverride(controllingTeam->teamId);
		if (captured)
		{
			captured = false;
			ChangeModelAndTeam(obj);
			LostControlPoint(obj);
		}
	}
	UpdateAnimation(obj);
}
void JMG_Utility_Control_Point::ChangeModelAndTeam(GameObject *obj)
{
	Commands->Set_Player_Type(obj,controllingTeam->playerType);
	obj->As_PhysicalGameObj()->Set_Radar_Blip_Color_Type(controllingTeam->radarBlipColor);
	if (!locked && !specialLocked)
	{
		if (wasLocked)
			obj->As_PhysicalGameObj()->Set_Radar_Blip_Shape_Type(radarBipType);
		if (controllingTeamOverride && _stricmp(controllingTeamOverride->pointModelOverride,""))
			Commands->Set_Model(obj,controllingTeamOverride->pointModelOverride);
		else
			Commands->Set_Model(obj,controllingTeam->pointModel);
	}
	else
	{
		if (!wasLocked)
			obj->As_PhysicalGameObj()->Set_Radar_Blip_Shape_Type(RADAR_BLIP_SHAPE_NONE);
		if (controllingTeamOverride && _stricmp(controllingTeamOverride->lockedPointModelOverride,""))
				Commands->Set_Model(obj,controllingTeamOverride->lockedPointModelOverride);
			else
				Commands->Set_Model(obj,controllingTeam->lockedModel);
	}
	wasLocked = locked | specialLocked;
}
void JMG_Utility_Control_Point::UpdateAnimation(GameObject *obj)
{
	if (!locked && !specialLocked)
		if (controllingTeamOverride && _stricmp(controllingTeamOverride->animOverride,""))
			SetAnimation(obj,controllingTeamOverride->animOverride,controllingTeamOverride->animationLength);
		else
			SetAnimation(obj,controllingTeam->defaultAnimation,controllingTeam->animationLength);
	else
		if (controllingTeamOverride && _stricmp(controllingTeamOverride->lockedAnimOverride,""))
			SetAnimation(obj,controllingTeamOverride->lockedAnimOverride,controllingTeamOverride->lockedAnimationLength);
		else
			SetAnimation(obj,controllingTeam->lockedAnim,controllingTeam->lockedAnimLen);
}
void JMG_Utility_Control_Point::SetAnimation(GameObject *obj,const char *anim,float length)
{
	float frame = length-((float)currerntCapturePoints/(float)maxCapturePoints)*length;
	if (frame != currentFrame)
	{
		Commands->Set_Animation(obj,anim,false,0,frame,frame,false);
		currentFrame = frame;
	}
}
void JMG_Utility_Control_Point::UpdateTeamOverride(int teamId)
{
	for (SLNode<ControlPointSettingOverride> *node = controlPointSettingOverride.Head();node != NULL;node = node->Next())
		if (ControlPointSettingOverride *data = node->Data())
			if (data->teamId == teamId)
			{
				controllingTeamOverride = data;
				break;
			}
}
void JMG_Utility_Control_Point::UpdateTeam(int teamId)
{
	for (SLNode<JMG_Utility_Control_Point_Controller::ControlPointTeamData> *node = JMG_Utility_Control_Point_Controller::controlPointTeamData.Head();node != NULL;node = node->Next())
		if (JMG_Utility_Control_Point_Controller::ControlPointTeamData *data = node->Data())
			if (data->teamId == controllingTeamId)
			{
				controllingTeam = data;
				break;
			}
}
void JMG_Utility_Control_Point::LostControlPoint(GameObject *obj)
{
	if (lastTeam)
	{
		char LostMessage[512];
		sprintf(LostMessage,"%s lost control of %s!",lastTeam->teamName,controlPointName);
		JmgUtility::MessageAllPlayers(127,255,127,LostMessage);
		Commands->Create_2D_Sound(lastTeam->lostSound);
		SendCustomToAllInRange(obj,lastTeam->customId,lastTeam->teamId,lastTeam->teamMembmerLostCustom);
		if (lastTeam->teamLostCustom)
		{
			GameObject *object = lastTeam->customId ? Commands->Find_Object(lastTeam->customId) : obj;
 			Commands->Send_Custom_Event(obj,object,lastTeam->teamLostCustom,0,0);
		}
		SendNeutralizeToAllInRange(obj,lastTeam->teamId);
		if (lastTeamOverride)
		{
			GameObject *object = lastTeamOverride->id ? Commands->Find_Object(lastTeamOverride->id) : obj;
			Commands->Send_Custom_Event(obj,object,lastTeamOverride->lostCustom,0,0.0f);
		}
		GrantPointsToTeamMembersInRange(obj,neutralizeScore,false);
		TriggerAssaultLinesUpdate();
		UpdateControllerWanderPoints();
	}
}
void JMG_Utility_Control_Point::CaptureControlPoint(GameObject *obj)
{
	char CapMessage[512];
	sprintf(CapMessage,"%s gained control of %s!",controllingTeam->teamName,controlPointName);
	JmgUtility::MessageAllPlayers(127,255,127,CapMessage);
	Commands->Create_2D_Sound(controllingTeam->captureSound);
	GrantPointsToTeamMembersInRange(obj,captureScore,true);
	SendCustomToAllInRange(obj,controllingTeam->customId,controllingTeam->teamId,controllingTeam->teamMemberCaptureCustom);
	if (controllingTeam->teamCaptureCustom)
	{
		GameObject *object = controllingTeam->customId ? Commands->Find_Object(controllingTeam->customId) : obj;
 		Commands->Send_Custom_Event(obj,object,controllingTeam->teamCaptureCustom,0,0);
	}
	if (controllingTeamOverride)
	{
		GameObject *object = controllingTeamOverride->id ? Commands->Find_Object(controllingTeamOverride->id) : obj;
		Commands->Send_Custom_Event(obj,object,controllingTeamOverride->captureCustom,0,0.0f);
	}
	TriggerAssaultLinesUpdate();
	UpdateControllerWanderPoints();
}
void JMG_Utility_Control_Point::GrantPointsToTeamMembersInRange(GameObject *obj,float points,bool matchTeam)
{
	int UnitCount = 0;
	Vector3 pos = Commands->Get_Position(obj);
	for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
	{
		SmartGameObj* o = current->Data();
		if (!o)
			continue;
		if (JmgUtility::SimpleDistance(pos,Commands->Get_Position(o)) > controlDistance)
			continue;
		JMG_Utility_Control_Point_Team_Member *unitScript = (JMG_Utility_Control_Point_Team_Member*)Find_Script_On_Object(o,"JMG_Utility_Control_Point_Team_Member");
		if (!unitScript || !unitScript->setupComplete || (matchTeam && unitScript->teamId != controllingTeamId) || (!matchTeam && unitScript->teamId == controllingTeamId))
			continue;
		UnitCount++;
	}
	float GivePoints = points/UnitCount;
	for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
	{
		SmartGameObj* o = current->Data();
		if (!o)
			continue;
		if (JmgUtility::SimpleDistance(pos,Commands->Get_Position(o)) > controlDistance)
			continue;
		JMG_Utility_Control_Point_Team_Member *unitScript = (JMG_Utility_Control_Point_Team_Member*)Find_Script_On_Object(o,"JMG_Utility_Control_Point_Team_Member");
		if (!unitScript || !unitScript->setupComplete || (matchTeam && unitScript->teamId != controllingTeamId) || (!matchTeam && unitScript->teamId == controllingTeamId))
			continue;
		Commands->Give_Points(o,GivePoints,true);
	}
}
void JMG_Utility_Control_Point::SendCustomToAllInRange(GameObject *obj,int id,int teamId,int custom)
{
	if (!custom)
		return;
	Vector3 pos = Commands->Get_Position(obj);
	for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
	{
		SmartGameObj* o = current->Data();
		if (!o)
			continue;
		Vector3 unitPos = Commands->Get_Position(o);
		if (controlHeightMinMax.X && (pos.Z + controlHeightMinMax.X) > unitPos.Z)
			continue;
		if (controlHeightMinMax.Y && (pos.Z + controlHeightMinMax.Y) < unitPos.Z)
			continue;
		if (JmgUtility::SimpleDistance(pos,unitPos) > controlDistance)
			continue;
		JMG_Utility_Control_Point_Team_Member *unitScript = (JMG_Utility_Control_Point_Team_Member*)Find_Script_On_Object(o,"JMG_Utility_Control_Point_Team_Member");
		if (!unitScript || !unitScript->setupComplete)
			continue;
		if (unitScript->teamData->teamId == teamId)
		{
			GameObject *object = id ? Commands->Find_Object(id) : obj;
 			Commands->Send_Custom_Event(o,object,custom,0,0);
		}
	}
}
void JMG_Utility_Control_Point::SendNeutralizeToAllInRange(GameObject *obj,int teamId)
{
	Vector3 pos = Commands->Get_Position(obj);
	for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
	{
		SmartGameObj* o = current->Data();
		if (!o)
			continue;
		Vector3 unitPos = Commands->Get_Position(o);
		if (controlHeightMinMax.X && (pos.Z + controlHeightMinMax.X) > unitPos.Z)
			continue;
		if (controlHeightMinMax.Y && (pos.Z + controlHeightMinMax.Y) < unitPos.Z)
			continue;
		if (JmgUtility::SimpleDistance(pos,unitPos) > controlDistance)
			continue;
		JMG_Utility_Control_Point_Team_Member *unitScript = (JMG_Utility_Control_Point_Team_Member*)Find_Script_On_Object(o,"JMG_Utility_Control_Point_Team_Member");
		if (!unitScript || !unitScript->setupComplete)
			continue;
		if (unitScript->teamData->teamId == teamId || !unitScript->teamData->teamMemberNeutralize)
			continue;
		GameObject *object = unitScript->teamData->customId ? Commands->Find_Object(unitScript->teamData->customId) : obj;
		Commands->Send_Custom_Event(o,object,unitScript->teamData->teamMemberNeutralize,0,0);
	}
}
void JMG_Utility_Control_Point::Destroyed(GameObject *obj)
{
	controlPointSettingOverride.Remove_All();
	controlPointWanderPoints.Remove_All();
}
void JMG_Utility_Control_Point::TriggerAssaultLinesUpdate()
{
	if (!JMG_Utility_Control_Point_Assault_Mode::controllerId)
		return;
	GameObject *assaultController = Commands->Find_Object(JMG_Utility_Control_Point_Assault_Mode::controllerId);
	if (!assaultController)
		return;
	JMG_Utility_Control_Point_Assault_Mode *assaultScript = (JMG_Utility_Control_Point_Assault_Mode*)Find_Script_On_Object(assaultController,"JMG_Utility_Control_Point_Assault_Mode");
	if (!assaultScript)
		return;
	assaultScript->UpdateAssaultLine(assaultController,false);
}
void JMG_Utility_Control_Point::UpdateControllerWanderPoints()
{
	for (SLNode<GameObject> *current = JMG_Utility_Control_Point_Controller::wanderPoints.Head();current;current = current->Next())
	{
		GameObject* o = current->Data();
		if (!o)
			continue;
		if (Is_Script_Attached(o,"JMG_Utility_Control_Point_Wander_Point"))
		{
			JMG_Utility_Control_Point_Wander_Point *cpScript = (JMG_Utility_Control_Point_Wander_Point*)Find_Script_On_Object(o,"JMG_Utility_Control_Point_Wander_Point");
			if (!cpScript)
				continue;
			cpScript->UpdateWanderpointSettings();
		}
		if (Is_Script_Attached(o,"JMG_Utility_Control_Point_Team_Target_Wander_Point"))
		{
			JMG_Utility_Control_Point_Team_Target_Wander_Point *cpScript = (JMG_Utility_Control_Point_Team_Target_Wander_Point*)Find_Script_On_Object(o,"JMG_Utility_Control_Point_Team_Target_Wander_Point");
			if (!cpScript)
				continue;
			cpScript->UpdateWanderpointSettings();
		}
	}
}
void JMG_Utility_Control_Point_Setting_Override::Created(GameObject *obj)
{
	JMG_Utility_Control_Point *cpScript = (JMG_Utility_Control_Point*)Find_Script_On_Object(obj,"JMG_Utility_Control_Point");
	if (!cpScript)
	{
		Console_Input("msg JMG_Utility_Control_Point_Setting_Override ERROR: JMG_Utility_Control_Point must be on the attached object!");
		return;
	}
	cpScript->controlPointSettingOverride.Add_Tail(new JMG_Utility_Control_Point::ControlPointSettingOverride(Get_Int_Parameter("TeamID"),Get_Int_Parameter("ID"),Get_Int_Parameter("CaptureCustom"),Get_Int_Parameter("LostCustom"),Get_Parameter("PointModelOverride"),Get_Parameter("AnimationOverride"),Get_Float_Parameter("AnimationLength"),Get_Parameter("LockedPointModelOverride"),Get_Parameter("LockedAnimationOverride"),Get_Float_Parameter("LockedAnimationLength")));
	Destroy_Script();
}
void JMG_Utility_Control_Point_Team_Member::Created(GameObject *obj)
{
	if (!JMG_Utility_Control_Point_Controller::controllerPlaced)
	{
		Destroy_Script();
		return;
	}
	teamId = Get_Int_Parameter("TeamID");
	multiplier = Get_Int_Parameter("Multiplier");
	Timer_Expired(obj,1);
}
void JMG_Utility_Control_Point_Team_Member::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		for (SLNode<JMG_Utility_Control_Point_Controller::ControlPointTeamData> *node = JMG_Utility_Control_Point_Controller::controlPointTeamData.Head();node != NULL;node = node->Next())
			if (JMG_Utility_Control_Point_Controller::ControlPointTeamData *data = node->Data())
				if (data->teamId == teamId)
				{
					teamData = data;
					setupComplete =  true;
					return;
				}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Control_Point_Team_Member::Killed(GameObject *obj,GameObject *killer)
{
	setupComplete = false;
}
int JMG_Utility_Control_Point_Assault_Mode::teamId = 0;
int JMG_Utility_Control_Point_Assault_Mode::controllerId = 0;
int JMG_Utility_Control_Point_Assault_Mode::frontLineGroup = 0;
int JMG_Utility_Control_Point_Assault_Mode::spawnGroup = 0;
int JMG_Utility_Control_Point_Assault_Mode::enemySpawnGroup = 0;
void JMG_Utility_Control_Point_Assault_Mode::Created(GameObject *obj)
{
	id = Get_Int_Parameter("ID");
	advanceCustom = Get_Int_Parameter("AdvanceCustom");
	pushedBackCustom = Get_Int_Parameter("PushBackCustom");
	controlAllCustom = Get_Int_Parameter("ControlAllCustom");
	lostAllCustom = Get_Int_Parameter("LostAllCustom");
	frontLineGroup = -1;
	spawnGroup = -1;
	enemySpawnGroup = -1;
	teamId = Get_Int_Parameter("TeamID");
	controllerId = Commands->Get_ID(obj);
	Timer_Expired(obj,1);
}
void JMG_Utility_Control_Point_Assault_Mode::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!JMG_Utility_Control_Point_Controller::allSetupComplete)
		{
			Commands->Start_Timer(obj,this,0.1f,1);
			return;
		}
		for (SLNode<GameObject> *current = JMG_Utility_Control_Point_Controller::controlPoints.Head();current;current = current->Next())
		{
			GameObject* o = current->Data();
			if (!o || !Is_Script_Attached(o,"JMG_Utility_Control_Point"))
				continue;
			JMG_Utility_Control_Point *cpScript = (JMG_Utility_Control_Point*)Find_Script_On_Object(o,"JMG_Utility_Control_Point");
			if (!cpScript || !cpScript->setupComplete)
			{
				Commands->Start_Timer(obj,this,0.1f,1);
				return;
			}
		}
		UpdateAssaultLine(obj,true);
	}
}
void JMG_Utility_Control_Point_Assault_Mode::UpdateAssaultLine(GameObject *obj,bool initialSetup)
{
	int friendlyCount = 0;
	int enemyCount = 0;
	int maxEnemyGroup = 0;
	int maxEnemySpawnGroup = 0;
	int minFriendlyGroup = -1;
	int minSpawnGroup = -1;
	for (SLNode<GameObject> *current = JMG_Utility_Control_Point_Controller::controlPoints.Head();current;current = current->Next())
	{
		GameObject* o = current->Data();
		if (!o || !Is_Script_Attached(o,"JMG_Utility_Control_Point"))
			continue;
		JMG_Utility_Control_Point *cpScript = (JMG_Utility_Control_Point*)Find_Script_On_Object(o,"JMG_Utility_Control_Point");
		if (!cpScript || !cpScript->setupComplete || cpScript->cpGroupId < 0)
			continue;
		if (cpScript->controllingTeamId != teamId && cpScript->captured)
			enemyCount++;
		if (cpScript->controllingTeamId == teamId && cpScript->captured)
			friendlyCount++;
		if ((cpScript->controllingTeamId != teamId || !cpScript->captured) && cpScript->cpGroupId > maxEnemyGroup)
			maxEnemyGroup = cpScript->cpGroupId;
		if (cpScript->controllingTeamId != teamId && cpScript->captured && cpScript->cpGroupId > maxEnemySpawnGroup)
			maxEnemySpawnGroup = cpScript->cpGroupId;
		if ((cpScript->controllingTeamId == teamId || !cpScript->captured) && (minFriendlyGroup == -1 || cpScript->cpGroupId < minFriendlyGroup))
			minFriendlyGroup = cpScript->cpGroupId;
		if ((cpScript->controllingTeamId == teamId && cpScript->captured) && (minSpawnGroup == -1 || cpScript->cpGroupId < minSpawnGroup))
			minSpawnGroup = cpScript->cpGroupId;
	}
	for (SLNode<GameObject> *current = JMG_Utility_Control_Point_Controller::controlPoints.Head();current;current = current->Next())
	{
		GameObject* o = current->Data();
		if (!o || !Is_Script_Attached(o,"JMG_Utility_Control_Point"))
			continue;
		JMG_Utility_Control_Point *cpScript = (JMG_Utility_Control_Point*)Find_Script_On_Object(o,"JMG_Utility_Control_Point");
		if (!cpScript || !cpScript->setupComplete || cpScript->cpGroupId < 0)
			continue;
		if (cpScript->cpGroupId != minFriendlyGroup && cpScript->cpGroupId != maxEnemyGroup && !cpScript->locked)
		{
			cpScript->locked = true;
			cpScript->ChangeModelAndTeam(o);
			cpScript->UpdateAnimation(o);
		}
		if ((cpScript->cpGroupId == minFriendlyGroup || cpScript->cpGroupId == maxEnemyGroup) && cpScript->locked)
		{
			cpScript->locked = false;
			cpScript->ChangeModelAndTeam(o);
			cpScript->UpdateAnimation(o);
		}
	}
	if (!initialSetup)
	{
		if (spawnGroup != minSpawnGroup && minSpawnGroup != -1 && enemyCount)
		{
			if (spawnGroup < minSpawnGroup && spawnGroup != -1)
				SendCustom(obj,pushedBackCustom,frontLineGroup);
			else
				SendCustom(obj,advanceCustom,maxEnemySpawnGroup);
		}
		if (!enemyCount)
			SendCustom(obj,controlAllCustom,frontLineGroup);
		if (!friendlyCount)
			SendCustom(obj,lostAllCustom,frontLineGroup);
	}
	frontLineGroup = minFriendlyGroup;
	spawnGroup = minSpawnGroup;
	enemySpawnGroup = maxEnemySpawnGroup;
	for (SLNode<GameObject> *current = JMG_Utility_Control_Point_Controller::wanderPoints.Head();current;current = current->Next())
	{
		GameObject* o = current->Data();
		if (!o)
			continue;
		if (Is_Script_Attached(o,"JMG_Utility_Control_Point_Wander_Point"))
		{
			JMG_Utility_Control_Point_Wander_Point *cpScript = (JMG_Utility_Control_Point_Wander_Point*)Find_Script_On_Object(o,"JMG_Utility_Control_Point_Wander_Point");
			if (!cpScript)
				continue;
			cpScript->UpdateWanderpointSettings();
		}
		if (Is_Script_Attached(o,"JMG_Utility_Control_Point_Team_Target_Wander_Point"))
		{
			JMG_Utility_Control_Point_Team_Target_Wander_Point *cpScript = (JMG_Utility_Control_Point_Team_Target_Wander_Point*)Find_Script_On_Object(o,"JMG_Utility_Control_Point_Team_Target_Wander_Point");
			if (!cpScript)
				continue;
			cpScript->UpdateWanderpointSettings();
		}
	}
}
void JMG_Utility_Control_Point_Assault_Mode::Destroyed(GameObject *obj)
{
	controllerId = 0;
}
void JMG_Utility_Control_Point_Assault_Mode::SendCustom(GameObject* obj,int custom,int thisFrontLineGroup)
{
	if (!custom)
		return;
	GameObject *object = id ? Commands->Find_Object(id) : obj;
	Commands->Send_Custom_Event(obj,object,custom,thisFrontLineGroup,0.0f);
}
void JMG_Utility_Control_Point_Wander_Point::Created(GameObject *obj)
{
	controlPointId = Get_Int_Parameter("ControlPointID");
	teamId = Get_Int_Parameter("TeamID");
	spawnableGroupId = Get_Int_Parameter("SpawnableGroupID");
	unspawnableGroupId = Get_Int_Parameter("UnspawnableGroupId");
	wanderPoint = AddAndReturnWanderpoint(obj);
	wanderPoint->value = -999;
	Commands->Set_Is_Rendered(obj,false);
	Commands->Set_Is_Visible(obj,false);
	Timer_Expired(obj,1);
}
void JMG_Utility_Control_Point_Wander_Point::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		GameObject *controller = Commands->Find_Object(controlPointId);
		if (!controller)
		{
			Commands->Start_Timer(obj,this,0.1f,1);
			return;
		}
		cpScript = (JMG_Utility_Control_Point*)Find_Script_On_Object(controller,"JMG_Utility_Control_Point");
		if (!cpScript || !cpScript->setupComplete)
		{
			Commands->Start_Timer(obj,this,0.1f,1);
			return;
		}
		cpScript->controlPointWanderPoints.Add_Tail(obj);
		JMG_Utility_Control_Point_Controller::wanderPoints.Add_Tail(obj);
		UpdateWanderpointSettings();
	}
}
void JMG_Utility_Control_Point_Wander_Point::UpdateWanderpointSettings()
{
	if (!cpScript || !cpScript->setupComplete || cpScript->cpGroupId < 0)
		return;
	if (spawnableGroupId != -999 && cpScript->captured && cpScript->controllingTeamId == teamId && (!cpScript->locked || 
	   (cpScript->cpGroupId == JMG_Utility_Control_Point_Assault_Mode::spawnGroup && JMG_Utility_Control_Point_Assault_Mode::teamId == teamId) || 
	   (cpScript->cpGroupId == JMG_Utility_Control_Point_Assault_Mode::enemySpawnGroup && JMG_Utility_Control_Point_Assault_Mode::teamId != teamId)))
		wanderPoint->value = spawnableGroupId;
	else if (unspawnableGroupId != -999)
		wanderPoint->value = unspawnableGroupId;
}
void JMG_Utility_Control_Point_Select_Spawn_System::Created(GameObject *obj)
{
	stringId = Get_Int_Parameter("StringId");
	controlPointId = 0;
	ungroupedControlPointId = 0;
	selectedCpId = 0;
	teamId = Get_Int_Parameter("TeamID");
	groupChangeCustom = Get_Int_Parameter("GroupChangeCustom");
	ungroupedChangeCustom = Get_Int_Parameter("UngroupedChangeCustom");
	spawnCustom = Get_Int_Parameter("SpawnCustom");
	safeTeleportDistance = Get_Float_Parameter("SafeTeleportDistance");
	maxSpawnTime = spawnTime = Get_Int_Parameter("SpawnTime")*10;
	maxWanderRange = Get_Float_Parameter("MaxWanderRange");
	startFadeRange = maxWanderRange*0.833f;
	maxWanderRange *= maxWanderRange;
	startFadeRange *= startFadeRange;
	lastSpawnGroup = -1;
	Commands->Set_Player_Type(obj,-4);
	Lock_Soldier_Collision_Group(obj,Collision_Group_Type::SOLDIER_GHOST_COLLISION_GROUP);
	Set_Screen_Fade_Opacity_Player(obj,0.25f,0.0f);
	Timer_Expired(obj,1);
}
void JMG_Utility_Control_Point_Select_Spawn_System::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!JMG_Utility_Control_Point_Controller::allSetupComplete)
		{
			Commands->Start_Timer(obj,this,0.1f,1);
			return;
		}
		Custom(obj,groupChangeCustom,0,obj);
		if (!selectedCpId)
			Commands->Start_Timer(obj,this,0.1f,1);
		else
		{
			if (spawnTime)
				DisplaySpawnTime(obj);
			Timer_Expired(obj,2);
		}
	}
	if (number == 2)
	{
		if (The_Game()->Is_Game_Over())
			return;
		if (selectedCpId)
		{
			GameObject *controlPoint = Commands->Find_Object(selectedCpId);
			JMG_Utility_Control_Point *cpScript = (JMG_Utility_Control_Point*)Find_Script_On_Object(controlPoint,"JMG_Utility_Control_Point");
			if (cpScript && cpScript->setupComplete && (!cpScript->captured || cpScript->controllingTeamId != teamId))
			{
				lastSpawnGroup = -1;
				Custom(obj,groupChangeCustom,0,obj);
			}
			else
			{
				float spawnTimeRatio = spawnTime/(float)maxSpawnTime;
				Vector3 spawnScreenColors = Vector3(spawnTimeRatio*0.75f,abs((spawnTimeRatio*0.75f)-((1-spawnTimeRatio)*0.39f)),(1-spawnTimeRatio)*0.78f);
				float tempSimpDist = JmgUtility::SimpleDistance(Commands->Get_Position(controlPoint),Commands->Get_Position(obj));
				float tempDistCalc = min(max(tempSimpDist-startFadeRange,0.0f)/(maxWanderRange-startFadeRange),1.0f);
				spawnScreenColors *= (1-tempDistCalc);
				Set_Screen_Fade_Color_Player(obj,min(tempDistCalc+spawnScreenColors.X,1.0f),spawnScreenColors.Y,spawnScreenColors.Z,0.1f);
				if (tempSimpDist > maxWanderRange)
					MoveToControlledWanderPointForCp(obj,selectedCpId);
			}
		}
		if (spawnTime)
		{
			if ((spawnTime <= 100 && !(spawnTime%10)) || (spawnTime > 100 && !(spawnTime%100)))
				DisplaySpawnTime(obj);
			spawnTime--;
			if (!spawnTime)
			{
				Set_HUD_Help_Text_Player_Text(obj,stringId,"Left click to spawn",Vector3(1.0f,0.5f,0.0f));
				Commands->Start_Timer(obj,this,5.0f,3);
			}
		}
		Commands->Start_Timer(obj,this,0.1f,2);
	}
	if (number == 3)
	{
		Set_HUD_Help_Text_Player_Text(obj,stringId,"Right click to cycle front line",Vector3(1.0f,0.5f,0.0f));
		Commands->Start_Timer(obj,this,5.0f,4);
	}
	if (number == 4)
	{
		Set_HUD_Help_Text_Player_Text(obj,stringId,"Middle click to cycle bonus points",Vector3(1.0f,0.5f,0.0f));
		Commands->Start_Timer(obj,this,5.0f,2);
	}
}
void JMG_Utility_Control_Point_Select_Spawn_System::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == spawnCustom)
	{
		if (spawnTime || The_Game()->Is_Game_Over())
			return;
		Set_HUD_Help_Text_Player_Text(obj,stringId," ",Vector3(1.0f,0.5f,0.0f));
		Set_Screen_Fade_Opacity_Player(obj,0.0f,0.1f);
		Change_Character(obj,Get_Parameter("SpawnPreset"));
		obj->As_SoldierGameObj()->Set_Collision_Group(Collision_Group_Type::SOLDIER_GHOST_COLLISION_GROUP);
	}
	if (message == groupChangeCustom)
	{
		if (The_Game()->Is_Game_Over())
			return;
		if (lastSpawnGroup != JMG_Utility_Control_Point_Assault_Mode::spawnGroup)
			controlPointId = 0;
		lastSpawnGroup = JMG_Utility_Control_Point_Assault_Mode::spawnGroup;
		selectedCpId = controlPointId = SelectCpToSpawnFrom(obj,controlPointId,true);
	}
	if (message == ungroupedChangeCustom)
	{
		if (The_Game()->Is_Game_Over())
			return;
		selectedCpId = ungroupedControlPointId = SelectCpToSpawnFrom(obj,ungroupedControlPointId,false);
	}
}
int JMG_Utility_Control_Point_Select_Spawn_System::SelectCpToSpawnFrom(GameObject *obj,int cpId,bool assaultLines)
{
	JMG_Utility_Control_Point *cpScript = NULL;
	int originalCpId = cpId;
	bool foundCp = false;
	SLNode<GameObject> *current = JMG_Utility_Control_Point_Controller::controlPoints.Head();
	while (current)
	{
		GameObject* o = current->Data();
		if (!o || !Is_Script_Attached(o,"JMG_Utility_Control_Point"))
			goto JMG_UTILITY_CONTROL_POINT_SELECT_SPAWN_SYSTEM_FIRST_LOOP;
		cpScript = (JMG_Utility_Control_Point*)Find_Script_On_Object(o,"JMG_Utility_Control_Point");
		if (!cpScript || !cpScript->setupComplete)
			goto JMG_UTILITY_CONTROL_POINT_SELECT_SPAWN_SYSTEM_FIRST_LOOP;
		if (cpScript->captured && cpScript->controllingTeamId == teamId && 
			(assaultLines && ((!cpScript->locked && cpScript->cpGroupId >= 0) || (cpScript->cpGroupId == JMG_Utility_Control_Point_Assault_Mode::spawnGroup && JMG_Utility_Control_Point_Assault_Mode::teamId == teamId))) || 
			(!assaultLines && cpScript->cpGroupId < 0))
		{
			int tmpId = Commands->Get_ID(o);
			if (!cpId || foundCp)
			{
				cpId = tmpId;
				break;
			}
			if (originalCpId == tmpId)
				foundCp = true;
		}
JMG_UTILITY_CONTROL_POINT_SELECT_SPAWN_SYSTEM_FIRST_LOOP:
		current = current->Next();
		if (!current)
		{
			if (originalCpId == cpId && !foundCp)
				break;
			current = JMG_Utility_Control_Point_Controller::controlPoints.Head();
		}
	}
	if (!cpId || !cpScript)
		return cpId;
	if (!MoveToControlledWanderPointForCp(obj,cpId))
		return 0;
	char selectCpMsg[220];
	sprintf(selectCpMsg,"Selected Control Point is %s.",cpScript->controlPointName);
	JmgUtility::DisplayChatMessage(obj,255,127,0,selectCpMsg);	
	return cpId;
}
bool JMG_Utility_Control_Point_Select_Spawn_System::MoveToControlledWanderPointForCp(GameObject *obj,int cpId)
{
	JMG_Utility_Control_Point *cpScript = (JMG_Utility_Control_Point*)Find_Script_On_Object(Commands->Find_Object(cpId),"JMG_Utility_Control_Point");
	if (!cpScript || !cpScript->setupComplete || cpScript->controlPointWanderPoints.Is_Empty())
		return false;
	int randomPoint = Commands->Get_Random_Int(0,cpScript->controlPointWanderPoints.Get_Count())+1;
	int origianlRandom = randomPoint;
	SLNode<GameObject> *current = cpScript->controlPointWanderPoints.Head();
	while (current)
	{
		GameObject* o = current->Data();
		if (!o || !Is_Script_Attached(o,"JMG_Utility_Control_Point_Wander_Point"))
			goto JMG_UTILITY_CONTROL_POINT_SELECT_SPAWN_SYSTEM_LOOP_END;
		JMG_Utility_Control_Point_Wander_Point *wanderScript = (JMG_Utility_Control_Point_Wander_Point*)Find_Script_On_Object(o,"JMG_Utility_Control_Point_Wander_Point");
		if (!wanderScript || wanderScript->teamId != teamId)
			goto JMG_UTILITY_CONTROL_POINT_SELECT_SPAWN_SYSTEM_LOOP_END;
		if (randomPoint)
			randomPoint--;
		if (!randomPoint)
		{
			Vector3 pos = Commands->Get_Position(o);
			MoveablePhysClass *mphys = obj->As_PhysicalGameObj() ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
			if (mphys && mphys->Find_Teleport_Location(pos,safeTeleportDistance,&pos))
			{
				Toggle_Fly_Mode(obj);
				Commands->Set_Position(obj,pos);
				Force_Position_Update(obj);
				Toggle_Fly_Mode(obj);
				Lock_Soldier_Collision_Group(obj,Collision_Group_Type::SOLDIER_GHOST_COLLISION_GROUP);
				char params[220];
				sprintf(params,"0.1,%.2f",Commands->Get_Facing(o));
				Commands->Attach_Script(obj,"JMG_Utility_Delay_Then_Rotate_Camera",params);
			}
			return true;
		}
JMG_UTILITY_CONTROL_POINT_SELECT_SPAWN_SYSTEM_LOOP_END:
		current = current->Next();
		if (!current)
		{
			if (origianlRandom == randomPoint)
				return false;
			current = cpScript->controlPointWanderPoints.Head();
		}
	}
	return false;
}
void JMG_Utility_Control_Point_Select_Spawn_System::DisplaySpawnTime(GameObject *obj)
{
	char disString[220];
	sprintf(disString,"%d",spawnTime/10);
	Set_HUD_Help_Text_Player_Text(obj,stringId,disString,Vector3(1.0f,0.5f,0.0f));
}
void JMG_Utility_Custom_And_Param_Send_Custom::Created(GameObject *obj)
{
	recieveMessage = Get_Int_Parameter("Custom");
	recieveParam = Get_Int_Parameter("Param");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("SendParam");
	delay = Get_Float_Parameter("Delay");
	randomDelay = Get_Float_Parameter("RandomDelay");
	randomChance = Get_Float_Parameter("RandomChance");
}
void JMG_Utility_Custom_And_Param_Send_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage && param == recieveParam)
	{
		if (randomChance && randomChance < Commands->Get_Random(0.0f,1.0f))
			return;
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
 		Commands->Send_Custom_Event(obj,object,custom,Param == -1 ? param : Param,delay+(randomDelay > 0 ? Commands->Get_Random(0.0f,randomDelay) : 0.0f));
	}
}
void JMG_Utility_Create_Move_To_Nearest_Pathfind::Created(GameObject *obj)
{
	Vector3 pos;
	Get_Closest_Pathfind_Spot_Size(Commands->Get_Position(obj),999999.0,&pos,0.5f);
	Commands->Set_Position(obj,pos);
}
void JMG_Utility_Objective_System_Objective_GameObject::Created(GameObject *obj)
{
	objectiveId = Get_Int_Parameter("ObjectiveID");
	objectivePriority = Get_Int_Parameter("ObjectivePriority");
	sprintf(preset,"%s",Get_Parameter("Preset"));
	attach = Get_Int_Parameter("Attach") ? true : false;
}
void JMG_Utility_Objective_System_Objective_GameObject_Tracker::Created(GameObject *obj)
{
	gameObjectId = Get_Int_Parameter("GameObjectID");
	markerId = Get_Int_Parameter("MarkerID");
}
void JMG_Utility_Attach_Script_To_Object_With_Weapon::Created(GameObject *obj)
{
	sprintf(weaponName,"%s",Get_Parameter("WeaponName"));
	rate = Get_Float_Parameter("Rate");
	sprintf(script,"%s",Get_Parameter("Script"));
	const char *parameters = Get_Parameter("Params");
	params = newstr(parameters);
	char delim = Get_Parameter("Delim")[0];
	unsigned int length = strlen(parameters);
	for (unsigned int x = 0;x < length;x++)
		if (params[x] == delim)
			params[x] = ',';
	playerType = Get_Int_Parameter("PlayerType");
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Attach_Script_To_Object_With_Weapon::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 mypos = Commands->Get_Position(obj);
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj *o = current->Data();
			if (!o)
				continue;
			const char *weap = Get_Current_Weapon(o);
			if (!weap || _stricmp(weap,weaponName) || CheckPlayerType(o,playerType))
				continue;
			Attach_Script_Once(o,script,params);
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Drop_Object_On_Death_And_Attach_Script::Killed(GameObject *obj,GameObject *killer)
{
	if (Commands->Get_Random(0,100) <= Get_Float_Parameter("Probability"))
	{
		Vector3 dropLocation = Commands->Get_Position(obj);
		dropLocation.Z += Get_Float_Parameter("Drop_Height");
		GameObject *drop = Commands->Create_Object(Get_Parameter("Drop_Object"),dropLocation);
		Commands->Set_Facing(drop,Commands->Get_Facing(obj));
		char script[128];
		sprintf(script,"%s",Get_Parameter("Script"));
		if (_stricmp(script,""))
		{
			const char *parameters = Get_Parameter("Params");
			char *params = newstr(parameters);
			char delim = Get_Parameter("Delim")[0];
			unsigned int length = strlen(parameters);
			for (unsigned int x = 0;x < length;x++)
				if (params[x] == delim)
					params[x] = ',';
			Commands->Attach_Script(drop,script,params);
		}
	}
}
void JMG_SinglePlayer_M04_Modifier::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_SinglePlayer_M04_Modifier::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!setupComplete)
		{
			setupComplete = true;
			float difficultyModifier = 100.0f-(Commands->Get_Difficulty_Level()/3.0f)*100.0f;
			char params[256];
			sprintf(params,"Weapon_Shotgun_Ai,JMG_Utility_Drop_Object_On_Death_And_Attach_Script,POW_Shotgun_Player@0.25@%.2f@SUR_Timed_Death@30.0^9999.0^BlamoKiller@^,@,1.0,0",difficultyModifier);
			Commands->Attach_Script(obj,"JMG_Utility_Attach_Script_To_Object_With_Weapon",params);
			sprintf(params,"Weapon_Shotgun_Ai_BH,JMG_Utility_Drop_Object_On_Death_And_Attach_Script,POW_Shotgun_Player@0.25@%.2f@SUR_Timed_Death@30.0^9999.0^BlamoKiller@^,@,1.0,0",difficultyModifier);
			Commands->Attach_Script(obj,"JMG_Utility_Attach_Script_To_Object_With_Weapon",params);
			sprintf(params,"Weapon_Shotgun_Player,JMG_Utility_Drop_Object_On_Death_And_Attach_Script,POW_Shotgun_Player@0.25@%.2f@SUR_Timed_Death@30.0^9999.0^BlamoKiller@^,@,1.0,0",difficultyModifier);
			Commands->Attach_Script(obj,"JMG_Utility_Attach_Script_To_Object_With_Weapon",params);
			GameObject *bubble = Commands->Create_Object("Daves Arrow",Vector3(0.0f,0.0f,-14.648f));
			Commands->Set_Model(bubble,"m04_bubbler");
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_SinglePlayer_M04_Modifier::Register_Auto_Save_Variables()
{
}
void JMG_Utility_Custom_Send_Delayed_Ordered_Customs::Created(GameObject *obj)
{
	delay = false;
	id = Get_Int_Parameter("ID");
	for (int x = 0;x < 10;x++)
	{
		char param[20];
		sprintf(param,"Custom%0d",x);
		customs[x] = Get_Int_Parameter(param);
		sprintf(param,"Time%0d",x);
		times[x] = Get_Float_Parameter(param);
	}
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Custom_Send_Delayed_Ordered_Customs::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		SLNode<TimedOrderedCustom> *current = timedOrderedCustom.Head();
		if (current)
		{
			TimedOrderedCustom *data = current->Data();
			GameObject *object = !id ? obj : (id == -1 ? Commands->Find_Object(data->senderId) : Commands->Find_Object(id));
			Commands->Send_Custom_Event(Commands->Find_Object(data->senderId),object,data->custom,data->param,0.0f);
			Commands->Start_Timer(obj,this,data->time,1);
			timedOrderedCustom.Remove_Head();
		}
		else
			delay = false;
	}
}
void JMG_Utility_Custom_Send_Delayed_Ordered_Customs::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	for (int x = 0;x < 10;x++)
		if (customs[x] == message)
		{
			if (delay)
				timedOrderedCustom.Add_Tail(new TimedOrderedCustom(Commands->Get_ID(sender),message,param,times[x]));
			else
			{
				GameObject *object = !id ? obj : (id == -1 ? sender : Commands->Find_Object(id));
				Commands->Send_Custom_Event(sender,object,message,param,0.0f);
				Commands->Start_Timer(obj,this,times[x],1);
				delay = true;
			}
			break;
		}
}
void JMG_Utility_Custom_Send_Delayed_Ordered_Customs::Destroyed(GameObject *obj)
{
	timedOrderedCustom.Remove_All();
}
void JMG_Utility_Custom_Send_Delayed_Ordered_Customs::Detach(GameObject *obj)
{
	timedOrderedCustom.Remove_All();
}
void JMG_Utility_Custom_Send_Custom_If_Model::Created(GameObject *obj)
{
	sprintf(model,"%s",Get_Parameter("Model"));
	recieveMessage = Get_Int_Parameter("Custom");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
}
void JMG_Utility_Custom_Send_Custom_If_Model::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage && !_stricmp(model,Get_Model(obj)))
	{
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
 		Commands->Send_Custom_Event(obj,object,custom,Param == -1 ? param : Param,delay);
	}
}
void JMG_Utility_Poke_Grant_Weapon_Destroy_Self::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void JMG_Utility_Poke_Grant_Weapon_Destroy_Self::Poked(GameObject *obj, GameObject *poker)
{
	if (Commands->Get_Player_Type(poker) == -4)
		return;
	Grant_Weapon(poker,Get_Parameter("Weapon"),true,Get_Int_Parameter("Rounds"),Get_Int_Parameter("Backpack") ? true : false);
	Commands->Destroy_Object(obj);
}
void JMG_Utility_Killed_Drop_Object_If_Weapon_Present::Killed(GameObject *obj,GameObject *killer)
{
	Destroyed(obj);
}
void JMG_Utility_Killed_Drop_Object_If_Weapon_Present::Destroyed(GameObject *obj)
{
	if (dropped)
		return;
	dropped = true;
	if (!Has_Weapon(obj,Get_Parameter("Weapon")))
		return;
	Vector3 pos = Commands->Get_Position(obj);
	pos.Z += Get_Float_Parameter("Height");
	GameObject *drop = Commands->Create_Object(Get_Parameter("DropPreset"),pos);
	Commands->Set_Facing(drop,Commands->Get_Facing(obj));
}
void JMG_Utility_Killed_Drop_Object_If_Weapon_Present::Detach(GameObject *obj)
{
	if (Exe == 4)
		return;
	Destroyed(obj);
}
void JMG_Utility_Poke_Send_Custom_If_Has_Weapon::Created(GameObject *obj)
{
	id = Get_Int_Parameter("ID");
	playerType = Get_Int_Parameter("PlayerType");
	custom = Get_Int_Parameter("Custom");
	param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	triggerOnce = Get_Int_Parameter("TriggerOnce");
	sprintf(weapon,"%s",Get_Parameter("Weapon"));
	removeWeapon = Get_Int_Parameter("RemoveWeapon");
	mustBeHeld = Get_Int_Parameter("MustBeHeld");
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void JMG_Utility_Poke_Send_Custom_If_Has_Weapon::Poked(GameObject *obj, GameObject *poker)
{
	if (CheckPlayerType(poker,playerType) || Commands->Get_Player_Type(poker) == -4)
		return;
	if (mustBeHeld)
	{
		const char *pWeapon = Get_Current_Weapon(poker);
		if (!pWeapon || _stricmp(weapon,pWeapon))
			return;
	}
	else if (!Has_Weapon(poker,weapon))
		return;
	GameObject *object = id ? (id == -1 ? poker : Commands->Find_Object(id)) : obj;
	if (object)
		Commands->Send_Custom_Event(poker,object,custom,param,delay);
	if (removeWeapon)
		Remove_Weapon(poker,weapon);
	if (triggerOnce)
	{
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		Destroy_Script();
	}
}
void JMG_Utility_Send_Custom_If_Weapon_Held_In_Range::Created(GameObject *obj)
{
	range = Get_Float_Parameter("Range");
	range *= range;
	id = Get_Int_Parameter("ID");
	playerType = Get_Int_Parameter("PlayerType");
	custom = Get_Int_Parameter("Custom");
	param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	triggerOnce = Get_Int_Parameter("TriggerOnce");
	sprintf(weapon,"%s",Get_Parameter("Weapon"));
	removeWeapon = Get_Int_Parameter("RemoveWeapon");
	mustBeHeld = Get_Int_Parameter("MustBeHeld");
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Send_Custom_If_Weapon_Held_In_Range::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 pos = Commands->Get_Position(obj);
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player || JmgUtility::SimpleDistance(pos,Commands->Get_Position(player)) > range)
				continue;
			if (CheckPlayerType(player,playerType) || Commands->Get_Player_Type(player) == -4)
				continue;
			if (mustBeHeld)
			{
				const char *pWeapon = Get_Current_Weapon(player);
				if (!pWeapon || _stricmp(weapon,pWeapon))
					continue;
			}
			else if (!Has_Weapon(player,weapon))
				continue;
			GameObject *object = id ? (id == -1 ? player : Commands->Find_Object(id)) : obj;
			if (object)
				Commands->Send_Custom_Event(player,object,custom,param,delay);
			if (removeWeapon)
				Remove_Weapon(player,weapon);
			if (triggerOnce)
			{
				Commands->Enable_HUD_Pokable_Indicator(obj,false);
				Destroy_Script();
			}
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Custom_Refect_Custom_If_Model::Created(GameObject *obj)
{
	sendFromSelf = Get_Int_Parameter("SendFromSelf");
	sprintf(model,"%s",Get_Parameter("Model"));
}
void JMG_Utility_Custom_Refect_Custom_If_Model::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (obj != sender && !_stricmp(model,Get_Model(obj)))
	{
		if (sendFromSelf)
 			Commands->Send_Custom_Event(sender,sender,message,param,0);
		else
 			Commands->Send_Custom_Event(obj,sender,message,param,0);
	}
}
void JMG_Utility_Custom_Set_Team::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	team = Get_Int_Parameter("Team");
}
void JMG_Utility_Custom_Set_Team::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Set_Player_Type(obj,team);
	}
}
void JMG_Utility_Custom_Create_Object_At_Random_Wander_Point::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	sprintf(preset,"%s",Get_Parameter("Preset"));
	wanderPointGroup = Get_Int_Parameter("WanderingAIGroupID");
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Custom_Create_Object_At_Random_Wander_Point::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Vector3 targetPos;
		float facing;
		if (!Get_A_Defense_Point(&targetPos,&facing))
		{
			Console_Input("msg No Wanderpoint found for that group!");
			return;
		}
		GameObject *created = Commands->Create_Object(Get_Parameter("Preset"),targetPos);
		Commands->Set_Facing(created,facing);
	}
}
void JMG_Utility_Zone_Teleport_To_Non_Visible_Wander_Point::Created(GameObject *obj)
{
	retryOnFailure = Get_Int_Parameter("RetryOnFailure") ? true : false;
	changeGroupIDCustom = Get_Int_Parameter("ChangeGroupIDCustom");
	playerType = Get_Int_Parameter("PlayerType");
	safeTeleportDistance = Get_Float_Parameter("SafeTeleportDistance");
	wanderPointGroup = Get_Int_Parameter("WanderingAIGroupID");
	aiOnly = Get_Int_Parameter("AiOnly") != 0;
}
void JMG_Utility_Zone_Teleport_To_Non_Visible_Wander_Point::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == changeGroupIDCustom)
	{
		wanderPointGroup = param;
	}
}
void JMG_Utility_Zone_Teleport_To_Non_Visible_Wander_Point::Entered(GameObject *obj,GameObject *enter)
{
	if (aiOnly && Commands->Is_A_Star(enter))
		return;
	if (CheckPlayerType(enter,playerType))
		return;
	Grab_Teleport_Spot(enter,5);
}
bool JMG_Utility_Zone_Teleport_To_Non_Visible_Wander_Point::Grab_Teleport_Spot(GameObject *enter,int attempts)
{
	if (The_Game()->Get_Game_Duration_S() < 1.0f)
	{
		char params[220];
		sprintf(params,"%d,%.2f",wanderPointGroup,safeTeleportDistance);
		Commands->Attach_Script(enter,"JMG_Utility_Zone_Teleport_To_Non_Visible_Wander_Point_Attach",params);
		return false;
	}
	if (!attempts)
	{
		if (retryOnFailure)
		{
			char params[220];
			sprintf(params,"%d,%.2f",wanderPointGroup,safeTeleportDistance);
			Commands->Attach_Script(enter,"JMG_Utility_Zone_Teleport_To_Non_Visible_Wander_Point_Attach",params);
		}
		return false;
	}
	attempts--;
	Vector3 targetPos;
	float facing;
	if (!Get_A_Defense_Point(&targetPos,&facing))
	{
		char errormsg[220];
		sprintf(errormsg,"msg JMG_Utility_Zone_Teleport_To_Non_Visible_Wander_Point ERROR: No wander points found for group %d!",wanderPointGroup);
		Console_Input(errormsg);
		return false;
	}
	MoveablePhysClass *mphys = enter->As_PhysicalGameObj() ? enter->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	if (mphys && mphys->Find_Teleport_Location(targetPos,safeTeleportDistance,&targetPos))
	{
		Toggle_Fly_Mode(enter);
		Commands->Set_Position(enter,targetPos);
		Force_Position_Update(enter);
		Toggle_Fly_Mode(enter);
		char params[220];
		sprintf(params,"0.1,%.2f",facing);
		Commands->Attach_Script(enter,"JMG_Utility_Delay_Then_Rotate_Camera",params);
		return true;
	}
	else
	{
		Commands->Set_Position(enter,targetPos);
		return Grab_Teleport_Spot(enter,attempts);
	}
}
void JMG_Utility_Zone_Teleport_To_Non_Visible_Wander_Point_Attach::Created(GameObject *obj)
{
	safeTeleportDistance = Get_Float_Parameter("SafeTeleportDistance");
	wanderPointGroup = Get_Int_Parameter("WanderingAIGroupID");
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Zone_Teleport_To_Non_Visible_Wander_Point_Attach::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 targetPos;
		float facing;
		if (!Get_A_Defense_Point(&targetPos,&facing))
		{
			Commands->Start_Timer(obj,this,0.25f,1);
			return;
		}
		MoveablePhysClass *mphys = obj->As_PhysicalGameObj() ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
		if (mphys && mphys->Find_Teleport_Location(targetPos,safeTeleportDistance,&targetPos))
		{
			Toggle_Fly_Mode(obj);
			Commands->Set_Position(obj,targetPos);
			Force_Position_Update(obj);
			Toggle_Fly_Mode(obj);
			char params[220];
			sprintf(params,"0.1,%.2f",facing);
			Commands->Attach_Script(obj,"JMG_Utility_Delay_Then_Rotate_Camera",params);
		}
		else
		{
			Commands->Start_Timer(obj,this,0.25f,1);
			return;
		}
		Destroy_Script();
	}
}
void JMG_Utility_Security_System_Random_NumberPad_Control::Created(GameObject *obj)
{
	if (Get_Int_Parameter("RandomCombinationDigits") >= 25)
	{
		Console_Input("msg JMG_Utility_Security_System_Random_NumberPad_Control ERROR: Check length of arrays!");
		return;
	}
	char presetName[220],boneName[220],params[225];
	const char *realPresetName = Commands->Get_Preset_Name(obj);
	for (int x = 0;x < 10;x++)
	{
		sprintf(presetName,"%s_Key_%0d",realPresetName,x);
		sprintf(boneName,"Number%0d",x);
		GameObject *button = Commands->Create_Object(presetName,Commands->Get_Position(obj));
		Commands->Attach_To_Object_Bone(button,obj,boneName);
		sprintf(params,"%d,4356341,%0d,null",Commands->Get_ID(obj),x);
		Commands->Attach_Script(button,"JMG_Utility_Custom_Combination_Lock_Key",params);
		buttonIds[x] = Commands->Get_ID(button);
	}
	sprintf(presetName,"%s_Key_Clear",realPresetName);
	GameObject *button = Commands->Create_Object(presetName,Commands->Get_Position(obj));
	Commands->Attach_To_Object_Bone(button,obj,"Clear");
	sprintf(params,"%d,4356340,0,null",Commands->Get_ID(obj));
	Commands->Attach_Script(button,"JMG_Utility_Custom_Combination_Lock_Key",params);
	buttonIds[10] = Commands->Get_ID(button);

	sprintf(presetName,"%s_Key_Enter",realPresetName);
	button = Commands->Create_Object(presetName,Commands->Get_Position(obj));
	Commands->Attach_To_Object_Bone(button,obj,"Enter");
	sprintf(params,"%d,4356342,0,null",Commands->Get_ID(obj));
	Commands->Attach_Script(button,"JMG_Utility_Custom_Combination_Lock_Key",params);
	buttonIds[11] = Commands->Get_ID(button);

	int rand = Get_Int_Parameter("RandomCombinationDigits");
	for (int x = 0;x < rand;x++)
	{
		combination[x] = (char)('0'+Commands->Get_Random_Int(0,10));
		combination[x+1] = '\0';
	}
	sprintf(params,"%d,%s,4356341,4356342,4356340,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s",Commands->Get_ID(obj),combination,Get_Int_Parameter("Success_Custom"),Get_Int_Parameter("Partial_Failure_Custom"),Get_Int_Parameter("Failure_Custom"),Get_Int_Parameter("Disable_On_Success"),Get_Int_Parameter("Disable_On_Failure"),Get_Int_Parameter("Starts_Enabled"),Get_Int_Parameter("Enable_Custom"),Get_Int_Parameter("Failure_Safty"),Get_Int_Parameter("Max_Failures"),Get_Parameter("Sound_Name_Base"));
	Commands->Attach_Script(obj,"JMG_Utility_Custom_Combination_Lock",params);
}
void JMG_Utility_Security_System_Random_NumberPad_Control::Destroyed(GameObject *obj)
{
	for (int x = 0;x < 12;x++)
	{
		GameObject *button = Commands->Find_Object(buttonIds[x]);
		if (button)
			Commands->Destroy_Object(button);
	}
}
void JMG_Utility_Custom_Combination_Lock::Created(GameObject *obj)
{
	id = Get_Int_Parameter("ID");
	inputCustom = Get_Int_Parameter("Input_Number_Custom");
	inputEnter = Get_Int_Parameter("Input_Enter_Custom");
	inputClear = Get_Int_Parameter("Input_Clear_Custom");
	enableCustom = Get_Int_Parameter("Enable_Custom");
	sprintf(combination,"%s",Get_Parameter("Combination"));
	successCustom = Get_Int_Parameter("Success_Custom");
	failureSaftey = Get_Int_Parameter("Failure_Safty");
	maxFailures = Get_Int_Parameter("Max_Failures");
	partialFailCustom = Get_Int_Parameter("Partial_Failure_Custom");
	failureCustom = Get_Int_Parameter("Failure_Custom");
	disableOnFailure = Get_Int_Parameter("Disable_On_Failure") ? true : false;
	disableOnSuccess = Get_Int_Parameter("Disable_On_Success") ? true : false;
	ClearUserEntry();
	failCount = 0;
	Enable(Get_Int_Parameter("Starts_Enabled") == 0 ? false : true);
	sprintf(soundNameBase,"%s",Get_Parameter("Sound_Name_Base"));
}
void JMG_Utility_Custom_Combination_Lock::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == inputCustom)
	{
		if (!enabled)
			return;
		if (depth >= 127)
			return;
		char sound[128];
		sprintf(sound,"%s%c",soundNameBase,(char)(param+'0'));
		Commands->Create_Sound(sound,Commands->Get_Position(obj),obj);
		inputCode[depth] = (char)(param+'0');
		inputCode[depth+1] = '\0';
		depth++;
	}
	if (message == inputEnter)
	{
		if (!enabled)
			return;
		char sound[128];
		sprintf(sound,"%sEnter",soundNameBase);
		Commands->Create_Sound(sound,Commands->Get_Position(obj),obj);
		if (_stricmp(inputCode,combination))
		{
			if (failureSaftey && depth)
			{
				failCount++;
				if (failCount < maxFailures)
					Send_Custom(sender,partialFailCustom,depth);
				else
				{
					failCount = 0;
					Send_Custom(sender,failureCustom,depth);
					if (disableOnFailure)
						enabled = false;
				}
			}
			else if (!failureSaftey)
			{
				Send_Custom(sender,failureCustom,depth);
				if (disableOnFailure)
					enabled = false;
			}
			ClearUserEntry();
		}
		else
		{
			if (disableOnSuccess)
				enabled = false;
			ClearUserEntry();
			failCount = 0;
			Send_Custom(sender,successCustom,param);
		}
	}
	if (message == inputClear)
	{
		if (!enabled)
			return;
		char sound[128];
		sprintf(sound,"%sClear",soundNameBase);
		Commands->Create_Sound(sound,Commands->Get_Position(obj),obj);
		ClearUserEntry();
	}
	if (message == enableCustom)
	{
		if (enabled)
			return;
		Enable(true);
	}
}
void JMG_Utility_Custom_Combination_Lock::Send_Custom(GameObject *obj,int custom,int param)
{
	GameObject *object = id ? Commands->Find_Object(id) : obj;
	if (object)
		Commands->Send_Custom_Event(obj,object,custom,param,0.0f);
}
void JMG_Utility_Custom_Combination_Lock::Enable(bool enableLock)
{
	ClearUserEntry();
	failCount = 0;
	enabled = enableLock;
	for (unsigned int x = 0;x < strlen(combination);x++)
		if (combination[x] < '0' || combination[x] > '9')
		{
			Console_Input("msg JMG_Custom_Combination_Lock ERROR: Combination must be a number 0 - 9!");
			enabled = false;
			break;
		}
}
void JMG_Utility_Custom_Combination_Lock::ClearUserEntry()
{
	depth = 0;
	sprintf(inputCode,"\0");
}
void JMG_Utility_Custom_Combination_Lock_Key::Created(GameObject *obj)
{
	id = Get_Int_Parameter("ID");
	message = Get_Int_Parameter("Message");
	param = Get_Int_Parameter("Param");
	sprintf(soundName,"%s",Get_Parameter("Sound_Name"));
	useAltAnimation = false;
	sprintf(animation,"%s.%s",Get_Model(obj),Get_Model(obj));
	sprintf(animation2,"%s.%sA",Get_Model(obj),Get_Model(obj));
}
void JMG_Utility_Custom_Combination_Lock_Key::Poked(GameObject *obj,GameObject *poker)
{
	if (Commands->Get_Player_Type(poker) == -4)
		return;
	if (id == 0)
		Commands->Send_Custom_Event(poker,obj,message,param,0);
	if (id != 0)
		Commands->Send_Custom_Event(poker,Commands->Find_Object(id),message,param,0);
	Commands->Create_Sound(soundName,Commands->Get_Position(obj),obj);
	Commands->Set_Animation(obj,useAltAnimation ? animation2 : animation,false,0,1,5,false);
	useAltAnimation = !useAltAnimation;
}
void JMG_Utility_Security_System_Sync_String_Random_NumberPad_Control::Created(GameObject *obj)
{
	delim = Get_Parameter("Delim")[0];
	stringId = Get_Int_Parameter("StringID");
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Security_System_Sync_String_Random_NumberPad_Control::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		JMG_Utility_Security_System_Random_NumberPad_Control *script = (JMG_Utility_Security_System_Random_NumberPad_Control*)Find_Script_On_Object(obj,"JMG_Utility_Security_System_Random_NumberPad_Control");
		if (!script)
			return;
		TDBObjClass *tdbobj = TranslateDBClass::Find_Object(stringId);
		char newString[2048];
		sprintf(newString,"%s",tdbobj->Get_English_String());
		char tmpString[2048];
		int offset = 0;
		for (int x = 0;x < 2048;x++,offset++)
		{
			if (newString[x] == delim)
				for (int y = 0;y < 25;y++)
				{
					if (script->combination[y] == '\0')
					{
						offset--;
						break;
					}
					tmpString[offset] = script->combination[y];
					offset++;
				}
			else
				tmpString[offset] = newString[x];
		}
		if (tdbobj)
		{
			WideStringClass str(tmpString);
			tdbobj->Set_String(TranslateDBClass::Get_Current_Language(),str);
		}
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player)
				continue;
			Change_String_Player(player,stringId,tmpString);
		}
	}
}
void JMG_Utility_Created_Animate_SubObject_On_Join::Created(GameObject *obj)
{
	sprintf(animation,"%s",Get_Parameter("Animation"));
	sprintf(subobject,"%s",Get_Parameter("SubObject"));
	for (int x = 1;x < 128;x++)
		animating[x] = false;
	Timer_Expired(obj,1024);
}
void JMG_Utility_Created_Animate_SubObject_On_Join::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1024) 
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (player && !animating[x])
			{
				animating[x] = true;
				TriggerAnimationForThePlayer(obj,x);
				Commands->Start_Timer(obj,this,0.25f,x);
			}
			if (!player && animating[x])
				animating[x] = false;
		}
		Commands->Start_Timer(obj,this,0.1f,1024);
	}
	if (number < 128)
		TriggerAnimationForThePlayer(obj,number);

}
void JMG_Utility_Created_Animate_SubObject_On_Join::TriggerAnimationForThePlayer(GameObject *obj,int playerId)
{
	GameObject *player = Get_GameObj(playerId);
	if (!player)
		return;
	Set_Subobject_Animation_Player(player,obj,animation,true,subobject,0,-1,true);
}
void JMG_Utility_Custom_Play_Animation_Send_Custom_When_Complete::Created(GameObject *obj)
{
	receivedCustom = Get_Int_Parameter("ReceivedCustom");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Custom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	sprintf(animation,"%s",Get_Parameter("Animation"));
	startFrame = Get_Float_Parameter("StartFrame");
	endFrame = Get_Float_Parameter("EndFrame");
}
void JMG_Utility_Custom_Play_Animation_Send_Custom_When_Complete::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == receivedCustom)
	{
		if (obj->As_SoldierGameObj())
		{
			obj->As_SoldierGameObj()->Set_Override_Weapon_Hold_Style(14);
			obj->As_SoldierGameObj()->Set_Can_Play_Damage_Animations(false);
		}
		Commands->Set_Animation(obj,animation,false,0,startFrame,endFrame,true);
	}
}
void JMG_Utility_Custom_Play_Animation_Send_Custom_When_Complete::Animation_Complete(GameObject *obj,const char *anim)
{
	if (_stricmp(anim,animation))
		return;
	if (obj->As_SoldierGameObj())
	{
		obj->As_SoldierGameObj()->Set_Override_Weapon_Hold_Style(-1);
		obj->As_SoldierGameObj()->Set_Can_Play_Damage_Animations(true);
	}
	GameObject *object = id ? Commands->Find_Object(id) : obj;
	if (object)
		Commands->Send_Custom_Event(obj,object,custom,Param,0.0f);
}
void JMG_Utility_Created_Play_Locked_Infantry_Animation::Created(GameObject *obj)
{
	if (!obj->As_SoldierGameObj())
	{
		Console_Input("msg JMG_Utility_Custom_Play_Locked_Infantry_Animation ERROR: Can only be used on infantry!");
		return;
	}
	obj->As_SoldierGameObj()->Set_Override_Weapon_Hold_Style(14);
	obj->As_SoldierGameObj()->Set_Can_Play_Damage_Animations(false);
	Commands->Set_Animation(obj,Get_Parameter("Animation"),true,0,0.0f,-1.0,true);
}
void JMG_Utility_Custom_Send_Custom_To_Preset::Created(GameObject *obj)
{
	receivedCustom = Get_Int_Parameter("ReceivedCustom");
	sprintf(preset,"%s",Get_Parameter("Preset"));
	custom = Get_Int_Parameter("Custom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
}
void JMG_Utility_Custom_Send_Custom_To_Preset::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == receivedCustom)
	{
		for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
		{
			GameObject *target = current->Data()->As_ScriptableGameObj();
			if (target && target->As_ScriptableGameObj() && !_stricmp(preset,Commands->Get_Preset_Name(target))) 
			{
				Commands->Send_Custom_Event(obj,target,custom,Param,delay);
			}
		}
	}
}
void JMG_Utility_Custom_Talk::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	stringId = Get_Int_Parameter("StringID");
}
void JMG_Utility_Custom_Talk::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Say_Dynamic_Dialogue(stringId,obj->As_SoldierGameObj(),&soundId);
	}
}
void JMG_Utility_Custom_Talk::Killed(GameObject *obj,GameObject *killer)
{
	Stop_Sound_Team(soundId,true,2);
}
void JMG_Utility_Zone_Send_Custom_Enter_Preset::Created(GameObject *obj)
{
	sprintf(preset,"%s",Get_Parameter("Preset"));
	playerType = Get_Int_Parameter("PlayerType");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Custom");
	param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	triggerOnce = Get_Int_Parameter("TriggerOnce") ? true : false;
}
void JMG_Utility_Zone_Send_Custom_Enter_Preset::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,playerType) || Commands->Get_Player_Type(enter) == -4 || _stricmp(preset,Commands->Get_Preset_Name(enter)))
		return;
	GameObject *object = id ? (id == -1 ? enter : Commands->Find_Object(id)) : obj;
	if (object)
		Commands->Send_Custom_Event(obj,object,custom,param,delay);
	if (triggerOnce)
		Destroy_Script();
}
float JMG_Utility_Global_Armor_Controller::maxArmor = 0.0f;
float JMG_Utility_Global_Armor_Controller::armor = 0.0f;
void JMG_Utility_Global_Armor_Controller::Created(GameObject *obj)
{
	maxArmor = Get_Float_Parameter("DefaultMaxArmor");
	armor = Get_Float_Parameter("DefaultArmor");
}
void JMG_Utility_Global_Armor_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	maxArmor = Get_Float_Parameter("MaxArmor");
	armor = Get_Float_Parameter("Armor");
	updateAllObjects = Get_Int_Parameter("UpdateAllObjects") ? true : false;
}
void JMG_Utility_Global_Armor_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		JMG_Utility_Global_Armor_Controller::maxArmor = maxArmor == -1.0f ? param : maxArmor;
		JMG_Utility_Global_Armor_Controller::armor = armor == -1.0f ? param : armor;
		if (!updateAllObjects)
			return;
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj *o = current->Data();
			if (!o)
				continue;
			if (!Is_Script_Attached(o,"JMG_Utility_Global_Armor_Object"))
				continue;
			Set_Max_Shield_Strength(o,JMG_Utility_Global_Armor_Controller::maxArmor);
			Commands->Set_Shield_Strength(o,JMG_Utility_Global_Armor_Controller::armor);
		}
	}
}
void JMG_Utility_Global_Armor_Object::Created(GameObject *obj)
{
	Set_Max_Shield_Strength(obj,JMG_Utility_Global_Armor_Controller::maxArmor);
	Commands->Set_Shield_Strength(obj,JMG_Utility_Global_Armor_Controller::armor);
}
float JMG_Utility_Global_Health_Controller::maxHealth = 0.0f;
float JMG_Utility_Global_Health_Controller::health = 0.0f;
void JMG_Utility_Global_Health_Controller::Created(GameObject *obj)
{
	maxHealth = Get_Float_Parameter("DefaultMaxHealth");
	health = Get_Float_Parameter("DefaultHealth");
}
void JMG_Utility_Global_Health_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	maxHealth = Get_Float_Parameter("MaxHealth");
	health = Get_Float_Parameter("Health");
	updateAllObjects = Get_Int_Parameter("UpdateAllObjects") ? true : false;
}
void JMG_Utility_Global_Health_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		JMG_Utility_Global_Health_Controller::maxHealth = maxHealth == -1.0f ? param : maxHealth;
		JMG_Utility_Global_Health_Controller::health = health == -1.0f ? param : health;
		if (!updateAllObjects)
			return;
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj *o = current->Data();
			if (!o)
				continue;
			if (!Is_Script_Attached(o,"JMG_Utility_Global_Health_Object"))
				continue;
			Set_Max_Health(o,JMG_Utility_Global_Health_Controller::maxHealth);
			Commands->Set_Health(o,JMG_Utility_Global_Health_Controller::health);
		}
	}
}
void JMG_Utility_Global_Health_Object::Created(GameObject *obj)
{
	Set_Max_Health(obj,JMG_Utility_Global_Health_Controller::maxHealth);
	Commands->Set_Health(obj,JMG_Utility_Global_Health_Controller::health);
}
void JMG_Utility_Custom_Create_Random_Explosions::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	sprintf(explosionPreset,"%s",Get_Parameter("ExplosionPreset"));
	count = Get_Int_Parameter("Count");
	distance = Get_Float_Parameter("MaxDistance");
	killerId = Get_Int_Parameter("KillerID");
}
void JMG_Utility_Custom_Create_Random_Explosions::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		GameObject *killer = killerId ? (killerId == -1 ? sender : Commands->Find_Object(killerId)) : obj;
		Vector3 originalPos = Commands->Get_Position(obj);
		for (int x = 0;x < count;x++)
		{
			Vector3 pos = originalPos;
			float Distance = Commands->Get_Random(0.0f,distance);
			float Rotation = Commands->Get_Random(-180.0,180);
			pos.X += cos(Rotation*PI180)*Distance;
			pos.Y += sin(Rotation*PI180)*Distance;
			Commands->Create_Explosion(explosionPreset,pos,killer);
		}
	}
}
void JMG_Utility_Custom_Enable_Spawners_Incrementally_In_Range::Created(GameObject *obj)
{
	currentId = Get_Int_Parameter("StartID");
	enable = Get_Int_Parameter("Enable") ? true : false;
	custom = Get_Int_Parameter("Custom");
	endId = Get_Int_Parameter("EndID");
}
void JMG_Utility_Custom_Enable_Spawners_Incrementally_In_Range::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		if (currentId > endId)
			return;
		Commands->Enable_Spawner(currentId,enable);
		currentId++;
	}
}
void JMG_Utility_Control_Point_Team_Target_Wander_Point::Created(GameObject *obj)
{
	controlPointId = Get_Int_Parameter("ControlPointID");
	teamId = Get_Int_Parameter("TeamID");
	targetGroupId = Get_Int_Parameter("TargetGroupID");
	nonTargetGroupId = Get_Int_Parameter("NonTargetGroupId");
	wanderPoint = AddAndReturnWanderpoint(obj);
	wanderPoint->value = -999;
	Commands->Set_Is_Rendered(obj,false);
	Commands->Set_Is_Visible(obj,false);
	Timer_Expired(obj,1);
}
void JMG_Utility_Control_Point_Team_Target_Wander_Point::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		GameObject *controller = Commands->Find_Object(controlPointId);
		if (!controller)
		{
			Commands->Start_Timer(obj,this,0.1f,1);
			return;
		}
		cpScript = (JMG_Utility_Control_Point*)Find_Script_On_Object(controller,"JMG_Utility_Control_Point");
		if (!cpScript || !cpScript->setupComplete)
		{
			Commands->Start_Timer(obj,this,0.1f,1);
			return;
		}
		cpScript->controlPointWanderPoints.Add_Tail(obj);
		JMG_Utility_Control_Point_Controller::wanderPoints.Add_Tail(obj);
		UpdateWanderpointSettings();
	}
}
void JMG_Utility_Control_Point_Team_Target_Wander_Point::UpdateWanderpointSettings()
{
	if (!cpScript || !cpScript->setupComplete || cpScript->cpGroupId < 0)
		return;
	if (targetGroupId != -999 && !cpScript->locked && (!cpScript->captured || cpScript->controllingTeamId != teamId) && (!cpScript->captured || 
	   (cpScript->cpGroupId == JMG_Utility_Control_Point_Assault_Mode::spawnGroup && JMG_Utility_Control_Point_Assault_Mode::teamId != teamId) || 
	   (cpScript->cpGroupId == JMG_Utility_Control_Point_Assault_Mode::enemySpawnGroup && JMG_Utility_Control_Point_Assault_Mode::teamId == teamId)))
		wanderPoint->value = targetGroupId;
	else
		wanderPoint->value = nonTargetGroupId;
}
void JMG_Utility_AI_Control_Point::Created(GameObject *obj)
{
	state = IDLE;
	moveBackward = false;
	lastPosition = Commands->Get_Position(obj);
	targetId = 0;
	lastSeenTime = 0;
	stuckTime = 0;
	reverseTime = 0;
	allowCaptureAttackDistract = false;
	captureCpGroupId = Get_Int_Parameter("CaptureCpGroupId");
	captureCpChance = Get_Float_Parameter("CaptureCpChance");
	captureSpeed = Get_Float_Parameter("CaptureSpeed");
	captureDistance = Get_Float_Parameter("CaptureDistance");
	captureDistanceSquared = captureDistance*captureDistance;
	defendCpGroupId = Get_Int_Parameter("DefendCpGroupId");
	defendSpeed = Get_Float_Parameter("DefendSpeed");
	defendDistance = Get_Float_Parameter("DefendDistance");
	defendDistanceSquared = defendDistance*defendDistance;
	closeDefendDistanceSquared = Get_Float_Parameter("CloseDefendDistance");
	closeDefendDistanceSquared *= closeDefendDistanceSquared;
	chooseFarDefendChance = Get_Float_Parameter("ChooseFarDefendChance");
	attackSpeed = Get_Float_Parameter("AttackSpeed");
	attackDistance = Get_Float_Parameter("AttackDistance");
	randomAttackDistance = Get_Float_Parameter("RandomAttackDistance");
	attackDistractFromCaptureChance = Get_Float_Parameter("AttackDistractFromCaptureChance");
	chanceToInvestigateLastSeenLocation = Get_Float_Parameter("ChanceToInvestigateLastSeenLocation");
	attackCheckBlocked = Get_Int_Parameter("AttackCheckBlocked") ? true : false;
	canSeeStealth = Get_Int_Parameter("CanSeeStealth");
	shutdownEngineOnArrival = Get_Int_Parameter("ShutdownEngineOnArrival") ? true : false;
	changeDefendSpeedCustom = Get_Int_Parameter("ChangeDefendSpeedCustom");
	changeCaptureSpeedCustom = Get_Int_Parameter("ChangeCaptureSpeedCustom");
	changeAttackSpeedCustom = Get_Int_Parameter("ChangeAttackSpeedCustom");

	const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Current_Weapon(obj),true);
	if (ammo)
	{
		weaponRange = ammo->Range;
		weaponEffectiveRange = ammo->Range < ammo->EffectiveRange ? ammo->Range : ammo->EffectiveRange;
	}
	attackArriveDistance = attackDistance >= 0 ? attackDistance : weaponEffectiveRange;
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,1.0f,1);
	Commands->Start_Timer(obj,this,1.0f,2);
	if (randomAttackDistance)
		Commands->Start_Timer(obj,this,0.0f,3);
	Commands->Start_Timer(obj,this,10.0f,4);
}
void JMG_Utility_AI_Control_Point::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (changeDefendSpeedCustom != 0 && changeDefendSpeedCustom == message)
	{
		defendSpeed = param/100.0f;
		if (state == CONTROL_POINT_DEFENSE)
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,defendSpeed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
	}
	if (changeCaptureSpeedCustom != 0 && changeCaptureSpeedCustom == message)
	{
		captureSpeed = param/100.0f;
		if (state == CONTROL_POINT_ATTACK)
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,captureSpeed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
	}
	if (changeAttackSpeedCustom != 0 && changeAttackSpeedCustom == message)
	{
		attackSpeed = param/100.0f;
		if (state == ATTACKING_TARGET)
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,attackSpeed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
	}
}
void JMG_Utility_AI_Control_Point::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (!Commands->Get_Health(seen))
		return;
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Control_Point_Ignore_Object"))
		return;
	if (!_stricmp(Get_Skin(seen),"Blamo") || !_stricmp(Get_Shield_Type(seen),"Blamo"))
		return;
	if (canSeeStealth != 2 && seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed())
	{
		if (!canSeeStealth)
			return;
		float dist = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen));
		if (seen->As_SoldierGameObj() && dist > seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance()*seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance())
			return;
		else if (seen->As_VehicleGameObj() && dist > seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance()*seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance())
			return;	
		else if (dist > seen->As_SmartGameObj()->Get_Stealth_Fade_Distance()*seen->As_SmartGameObj()->Get_Stealth_Fade_Distance())
			return;
	}
	if (!targetId || !lastSeenTime || Choose_Target(obj,seen))
		TriggerAttack(obj,seen);
	if (targetId == Commands->Get_ID(seen))
		lastSeenTime = Commands->Get_Random_Int(30,60);
}
void JMG_Utility_AI_Control_Point::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && !The_Game()->Is_Game_Over())
	{
		if ((state == CONTROL_POINT_ATTACK || state == CONTROL_POINT_DEFENSE || state == CHECKING_LOCATION) && !lastWanderPoint)
			state = IDLE;
		Vector3 targetPos = Commands->Get_Position(obj);
		if (lastSeenTime)
		{
			lastSeenTime--;
			if (!lastSeenTime)
			{
				if (state == CONTROL_POINT_ATTACK && lastWanderPoint)
					Attack_Move(obj,NULL,lastWanderPoint->position,captureSpeed,captureDistance,false,false);
				else
					SelectNextMission(obj,ValidLastLocation(targetId));
				targetId = 0;
			}
		}
		if (targetId)
		{
			GameObject *target = Commands->Find_Object(targetId);
			targetPos = Commands->Get_Position(target);
			if (!target || !Commands->Get_Health(target) || (Commands->Get_Player_Type(obj) != -2 && Commands->Get_Player_Type(target) == Commands->Get_Player_Type(obj)) || Is_Script_Attached(target,"JMG_Utility_AI_Control_Point_Ignore_Object"))
			{
				lastSeenTime = 0;
				if (state == CONTROL_POINT_ATTACK && lastWanderPoint)
					Attack_Move(obj,NULL,lastWanderPoint->position,captureSpeed,captureDistance,false,false);
				else
					SelectNextMission(obj,ValidLastLocation(targetId));
				targetId = 0;
			}
		}
		if (state == CONTROL_POINT_ATTACK && lastWanderPoint)
		{
			if (lastWanderPoint->value != captureCpGroupId)
				SelectNextMission(obj,false);
		}
		if (state == CONTROL_POINT_DEFENSE && lastWanderPoint)
		{
			if (JmgUtility::SimpleDistance(lastWanderPoint->position,Commands->Get_Position(obj)) < defendDistanceSquared)
				SelectNextMission(obj,false);
		}
		if (state == CHECKING_LOCATION && lastWanderPoint)
		{
			if (JmgUtility::SimpleDistance(lastWanderPoint->position,Commands->Get_Position(obj)) < (obj->As_VehicleGameObj() ? 25.0f : 1.0f))
				SelectNextMission(obj,false);
		}
		if (state == IDLE)
		{
			SelectNextMission(obj,false);
		}
		if (state == CHECKING_LOCATION)
			if (JmgUtility::SimpleDistance(lastAction.location,Commands->Get_Position(obj)) <= (obj->As_VehicleGameObj() ? 25.0f : 1.0f))
				state = IDLE;
		if (state == ACTION_BADPATH)
			state = IDLE;
		if (state != IDLE)
			Stuck_Check(obj,targetPos);
		Commands->Start_Timer(obj,this,0.1f,1);
	}
	if (number == 3)
	{
		attackArriveDistance = attackDistance >= 0 ? attackDistance : weaponEffectiveRange;
		attackArriveDistance += Commands->Get_Random(randomAttackDistance,randomAttackDistance);
	}
	if (number == 4)
	{
		allowCaptureAttackDistract = Commands->Get_Random(0.0f,1.0f) < attackDistractFromCaptureChance;
		Commands->Start_Timer(obj,this,10.0f,4);
	}
}
void JMG_Utility_AI_Control_Point::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		if (state == CONTROL_POINT_ATTACK)
			state = ACTION_BADPATH;
		if (state == CONTROL_POINT_DEFENSE)
			state = ACTION_BADPATH;
		else if (state == CHECKING_LOCATION)
			state = ACTION_BADPATH;
		else if (state == ATTACKING_TARGET)
			state = ACTION_BADPATH;
	}
}
void JMG_Utility_AI_Control_Point::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0.0f || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (!_stricmp(Get_Skin(damager),"Blamo") || !_stricmp(Get_Shield_Type(damager),"Blamo"))
		return;
	if (!targetId || !lastSeenTime || Choose_Target(obj,damager))
		TriggerAttack(obj,damager);
}
void JMG_Utility_AI_Control_Point::Attack_Move(GameObject *obj,GameObject *target,Vector3 location,float speed,float distance,bool attack,bool overrideLocation)
{
	Commands->Action_Reset(obj,100);
	lastAction = LastAction(targetId,location,speed,distance,attack,overrideLocation);
	ActionParamsStruct params;
	params.ObserverID = this->Get_ID();
	if (target && !overrideLocation)
	{
		params.Set_Movement(target,speed,distance,false);
		params.MoveFollow = true;
	}
	else
	{
		params.Set_Movement(location,speed,distance,false);
		params.MoveFollow = false;
	}
	params.MoveBackup = moveBackward;
	params.MovePathfind = true;
	if (shutdownEngineOnArrival)
		params.ShutdownEngineOnArrival = true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,100,100,AI_STATE_COMBAT);
	params.AttackCheckBlocked = attackCheckBlocked;
	if (target && attack)
	{
		params.Set_Attack(target,weaponRange,0.0f,true);
		Commands->Action_Attack(obj,params);
	}
}
void JMG_Utility_AI_Control_Point::SelectNextMission(GameObject *obj,ValidLastLocation goNearLastWanderPoint)
{
	state = IDLE;
	Vector3 wanderPos;
	if (goNearLastWanderPoint.valid && state != CONTROL_POINT_ATTACK && Commands->Get_Random(0.0f,1.0f) < chanceToInvestigateLastSeenLocation)
	{
		state = CHECKING_LOCATION;
		Attack_Move(obj,NULL,goNearLastWanderPoint.location,attackSpeed,0.0f,false,false);
		return;
	}
	GameObject *target = Commands->Find_Object(targetId);
	if (Commands->Get_Random(0.0f,1.0f) < captureCpChance)
	{
		Rp2SimplePositionSystem::SimplePositionNode *node = Get_Capture_Point();
		if (node)
		{
			state = CONTROL_POINT_ATTACK;
			lastWanderPoint = node;
			Attack_Move(obj,target,lastWanderPoint->position,captureSpeed,captureDistance,false,true);
			return;
		}
	}
	Rp2SimplePositionSystem::SimplePositionNode *node = Get_Defense_Point(obj);
	if (node)
	{
		state = CONTROL_POINT_DEFENSE;
		lastWanderPoint = node;
		Attack_Move(obj,target,lastWanderPoint->position,captureSpeed,0.0f,false,true);
		return;
	}
}
void JMG_Utility_AI_Control_Point::Stuck_Check(GameObject *obj,Vector3 targetPos)
{
	if (!obj->As_VehicleGameObj())
	{
		Vector3 pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(pos,lastPosition) < 1.0f)
		{
			if (state == ATTACKING_TARGET && targetId && JmgUtility::SimpleDistance(pos,Commands->Get_Position(Commands->Find_Object(targetId))) <= attackArriveDistance*attackArriveDistance)
			{
				stuckTime = 0;
				return;
			}
			else if (state == CONTROL_POINT_ATTACK && lastWanderPoint &&JmgUtility::SimpleDistance(lastWanderPoint->position,pos) <= captureDistance)
			{
				stuckTime = 0;
				return;
			}
			if (!reverseTime)
			{
				stuckTime++;
				if (stuckTime > 30)
				{
					moveBackward = true;
					Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
					reverseTime = Commands->Get_Random_Int(10,60);
					stuckTime = 0;
				}
			}
		}
		else
		{
			stuckTime = 0;
			lastPosition = pos;
		}
		if (reverseTime)
		{
			reverseTime--;
			if (!reverseTime)
			{
				moveBackward = false;
				Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
			}
		}
	}
	else
	{
		Vector3 pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(pos,lastPosition) < 1.0f)
		{
			if (state == ATTACKING_TARGET && targetId && JmgUtility::SimpleDistance(pos,Commands->Get_Position(Commands->Find_Object(targetId))) <= attackArriveDistance*attackArriveDistance)
			{
				stuckTime = 0;
				return;
			}
			else if (state == CONTROL_POINT_ATTACK && lastWanderPoint && JmgUtility::SimpleDistance(lastWanderPoint->position,pos) <= captureDistance)
			{
				stuckTime = 0;
				return;
			}
			stuckTime++;
			if (stuckTime > 5)
			{
				Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
				stuckTime = 0;
			}
		}
		else
		{
			stuckTime = 0;
			lastPosition = pos;
		}
	}
}
bool JMG_Utility_AI_Control_Point::Choose_Target(GameObject *obj,GameObject *target)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *currentTarget = Commands->Find_Object(targetId);
	if (!currentTarget)
		return true;
	if (JmgUtility::SimpleDistance(pos,Commands->Get_Position(target)) < JmgUtility::SimpleDistance(pos,Commands->Get_Position(currentTarget)))
		return true;
	return false;
}
JMG_Utility_AI_Control_Point::ValidLastLocation::ValidLastLocation(int enemyId)
{
	GameObject *target = Commands->Find_Object(enemyId);
	if (!target || !Commands->Get_Health(target))
	{
		this->valid = false;
		return;
	}
	this->location = Commands->Get_Position(target);
	this->valid = true;
}
void JMG_Utility_AI_Control_Point::TriggerAttack(GameObject *obj,GameObject *target)
{
	targetId = Commands->Get_ID(target);
	lastSeenTime = Commands->Get_Random_Int(30,60);
	if ((state == CONTROL_POINT_ATTACK && !allowCaptureAttackDistract))
		Attack_Move(obj,target,lastWanderPoint->position,captureSpeed,captureDistance,true,true);
	else
	{
		state = ATTACKING_TARGET;
		Attack_Move(obj,target,Vector3(),attackSpeed,attackArriveDistance,true,false);
	}
}
void JMG_Utility_Objective_System_Set_Infantry_Attach_Bone::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_Objective_System_Set_Infantry_Attach_Bone::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		sprintf(BasicObjectiveSystem.infantryAttachBone,"%s",Get_Parameter("InfantryAttachBone"));
	}
}
void JMG_Utility_Security_System_Fixed_NumberPad_Control::Created(GameObject *obj)
{
	char presetName[220],boneName[220],params[225];
	const char *realPresetName = Commands->Get_Preset_Name(obj);
	for (int x = 0;x < 10;x++)
	{
		sprintf(presetName,"%s_Key_%0d",realPresetName,x);
		sprintf(boneName,"Number%0d",x);
		GameObject *button = Commands->Create_Object(presetName,Commands->Get_Position(obj));
		Commands->Attach_To_Object_Bone(button,obj,boneName);
		sprintf(params,"%d,4356341,%0d,null",Commands->Get_ID(obj),x);
		Commands->Attach_Script(button,"JMG_Utility_Custom_Combination_Lock_Key",params);
		buttonIds[x] = Commands->Get_ID(button);
	}
	sprintf(presetName,"%s_Key_Clear",realPresetName);
	GameObject *button = Commands->Create_Object(presetName,Commands->Get_Position(obj));
	Commands->Attach_To_Object_Bone(button,obj,"Clear");
	sprintf(params,"%d,4356340,0,null",Commands->Get_ID(obj));
	Commands->Attach_Script(button,"JMG_Utility_Custom_Combination_Lock_Key",params);
	buttonIds[10] = Commands->Get_ID(button);

	sprintf(presetName,"%s_Key_Enter",realPresetName);
	button = Commands->Create_Object(presetName,Commands->Get_Position(obj));
	Commands->Attach_To_Object_Bone(button,obj,"Enter");
	sprintf(params,"%d,4356342,0,null",Commands->Get_ID(obj));
	Commands->Attach_Script(button,"JMG_Utility_Custom_Combination_Lock_Key",params);
	buttonIds[11] = Commands->Get_ID(button);

	sprintf(params,"%d,%s,4356341,4356342,4356340,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s",Commands->Get_ID(obj),Get_Parameter("KeyCode"),Get_Int_Parameter("Success_Custom"),Get_Int_Parameter("Partial_Failure_Custom"),Get_Int_Parameter("Failure_Custom"),Get_Int_Parameter("Disable_On_Success"),Get_Int_Parameter("Disable_On_Failure"),Get_Int_Parameter("Starts_Enabled"),Get_Int_Parameter("Enable_Custom"),Get_Int_Parameter("Failure_Safty"),Get_Int_Parameter("Max_Failures"),Get_Parameter("Sound_Name_Base"));
	Commands->Attach_Script(obj,"JMG_Utility_Custom_Combination_Lock",params);
}
void JMG_Utility_Security_System_Fixed_NumberPad_Control::Destroyed(GameObject *obj)
{
	for (int x = 0;x < 12;x++)
	{
		GameObject *button = Commands->Find_Object(buttonIds[x]);
		if (button)
			Commands->Destroy_Object(button);
	}
}
void JMG_Utility_Custom_Teleport_Sender_Wanderpoint::Created(GameObject *obj)
{
	customMsg = Get_Int_Parameter("Custom");
	retryOnFailure = Get_Int_Parameter("RetryOnFailure") ? true : false;
	safeTeleportDistance = Get_Float_Parameter("SafeTeleportDistance");
	wanderPointGroup = Get_Int_Parameter("WanderingAIGroupID");
}
void JMG_Utility_Custom_Teleport_Sender_Wanderpoint::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == customMsg)
	{
		Grab_Teleport_Spot(sender,5);
	}
}
bool JMG_Utility_Custom_Teleport_Sender_Wanderpoint::Grab_Teleport_Spot(GameObject *enter,int attempts)
{
	if (The_Game()->Get_Game_Duration_S() < 1.0f)
	{
		char params[220];
		sprintf(params,"%d,%.2f",wanderPointGroup,safeTeleportDistance);
		Commands->Attach_Script(enter,"JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach",params);
		return false;
	}
	if (!attempts)
	{
		if (retryOnFailure)
		{
			char params[220];
			sprintf(params,"%d,%.2f",wanderPointGroup,safeTeleportDistance);
			Commands->Attach_Script(enter,"JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach",params);
		}
		return false;
	}
	attempts--;
	Vector3 targetPos;
	float facing;
	if (!Get_A_Defense_Point(&targetPos,&facing))
	{
		char errormsg[220];
		sprintf(errormsg,"msg JMG_Utility_Zone_Teleport_To_Random_Wander_Point ERROR: No wander points found for group %d!",wanderPointGroup);
		Console_Input(errormsg);
		return false;
	}
	MoveablePhysClass *mphys = enter->As_PhysicalGameObj() ? enter->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	if (mphys && mphys->Find_Teleport_Location(targetPos,safeTeleportDistance,&targetPos))
	{
		Toggle_Fly_Mode(enter);
		Commands->Set_Position(enter,targetPos);
		Force_Position_Update(enter);
		Toggle_Fly_Mode(enter);
		char params[220];
		sprintf(params,"0.1,%.2f",facing);
		Commands->Attach_Script(enter,"JMG_Utility_Delay_Then_Rotate_Camera",params);
		return true;
	}
	else
	{
		Commands->Set_Position(enter,targetPos);
		return Grab_Teleport_Spot(enter,attempts);
	}
}
void JMG_Utility_Create_Set_Random_Model::Created(GameObject *obj)
{
	char model[32];
	sprintf(model,"%s%0d",Get_Parameter("BaseName"),Commands->Get_Random_Int(0,Get_Int_Parameter("FinalModelNumber")+1));
	Commands->Set_Model(obj,model);
}
void JMG_Utility_Custom_Destroy_Sender::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Destroy_Sender::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Destroy_Object(sender);
	}
}
void JMG_Utility_Custom_Grant_Weapon_Destroy_Sender::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	sprintf(weapon,"%s",Get_Parameter("Weapon"));
	sprintf(fullAmmoString,"%s",Get_Parameter("FullAmmoString"));
}
void JMG_Utility_Custom_Grant_Weapon_Destroy_Sender::Custom(GameObject *obj,int message,int cparam,GameObject *sender)
{
	if (message == custom)
	{
		if (Has_Weapon(obj,weapon) && Get_Max_Total_Bullets(obj,weapon) == Get_Total_Bullets(obj,weapon))
		{
			Set_HUD_Help_Text_Player(obj,Get_String_ID_By_Desc(fullAmmoString),Vector3(0.0f,1.0f,0.0f));
			return;
		}
		Grant_Weapon(obj,weapon,true,1,true);
		Commands->Destroy_Object(sender);
	}
}
void JMG_Utility_Remove_Script_While_Has_Weapon::Created(GameObject *obj)
{
	sprintf(weaponName,"%s",Get_Parameter("WeaponName"));
	rate = Get_Float_Parameter("Rate");
	sprintf(script,"%s",Get_Parameter("Script"));
	const char *parameters = Get_Parameter("Params");
	params = newstr(parameters);
	char delim = Get_Parameter("Delim")[0];
	unsigned int length = strlen(parameters);
	for (unsigned int x = 0;x < length;x++)
		if (params[x] == delim)
			params[x] = ',';
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Remove_Script_While_Has_Weapon::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!Has_Weapon(obj,weaponName))
			Attach_Script_Once(obj,script,params);
		else if (Is_Script_Attached(obj,script))
			Remove_Script(obj,script);
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Swimming_Infantry_AI::Created(GameObject *obj)
{
	if (Commands->Is_A_Star(obj))
	{
		Destroy_Script();
		return;
	}
	lastWaterZoneId = 0;
	heartBeatSoundId = 0;
	pantSoundId = 0;
	underwater = false;
	waterZoneCount = 0;
	startedFadeRed = false;
	drownTime = 0.0f;
	defaultSpeed = obj->As_SoldierGameObj()->Get_Max_Speed();
	waterDamageDelayTime = remainingWaterDamageDelay = Get_Int_Parameter("WaterDamageDelayTime");
	waterDamageDelayTimeRecover = Get_Int_Parameter("WaterDamageDelayTimeRecover");
	sprintf(originalSkin,"");
	sprintf(originalArmor,"");
	sprintf(originalModel,"");
	for (SLNode<ScriptZoneGameObj>* node = GameObjManager::ScriptZoneGameObjList.Head();node;node = node->Next())
	{
		GameObject *zone = (GameObject *)node->Data();
		if (Is_Script_Attached(zone,"JMG_Utility_Swimming_Zone") && IsInsideZone(zone,obj))
			Commands->Send_Custom_Event(obj,obj,347341,347341,0.25f);
	}
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Swimming_Infantry_AI::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && Commands->Get_Health(obj))
	{
		if (obj->As_SoldierGameObj()->Is_Crouched() && !obj->As_SoldierGameObj()->Is_Airborne() && waterZoneCount)
		{
			if (!underwater)
			{
				underwater = true;
				isUnderwater = true;
				HideSoundEmitter(&pantSoundId);
			}
			drownTime += 0.1f;
			if (!startedFadeRed && drownTime >= Get_Float_Parameter("DrownTime")-Get_Float_Parameter("StartDrownSequence"))
			{
				startedFadeRed = true;
				CreateSoundEmitter(obj,Get_Parameter("HeartBeatSoundEmitterModel"),&heartBeatSoundId);
			}
			if (drownTime >= Get_Float_Parameter("DrownTime"))
				Commands->Apply_Damage(obj,Get_Float_Parameter("DrownDamageRate"),"None",obj);
		}
		else
		{
			if (underwater)
			{
				underwater = false;
				isUnderwater = false;
				HideSoundEmitter(&heartBeatSoundId);
				if (drownTime > 1.0f)
					CreateSoundEmitter(obj,Get_Parameter("PantingSoundEmitterModel"),&pantSoundId);
				if (drownTime > Get_Float_Parameter("DrownTime"))
					Commands->Create_3D_Sound_At_Bone(Get_Parameter("GaspForBreath"),obj,"c head");
			}
			if (drownTime)
			{
				drownTime -= Get_Float_Parameter("CatchBreathRate");
				if (startedFadeRed && drownTime < Get_Float_Parameter("DrownTime")-Get_Float_Parameter("StartDrownSequence"))
					startedFadeRed = false;
				if (drownTime <= 0)
				{
					drownTime = 0.0f;
					HideSoundEmitter(&pantSoundId);
				}
			}
		}
		if (waterZoneCount)
		{
			const char *weap = Get_Current_Weapon(obj);
			if (Has_Weapon(obj,Get_Parameter("WeaponPreset")) && (!weap || _stricmp(weap,Get_Parameter("WeaponPreset"))))
				Commands->Select_Weapon(obj,Get_Parameter("WeaponPreset"));
			if (remainingWaterDamageDelay)
				remainingWaterDamageDelay--;
			if (!remainingWaterDamageDelay && Get_Float_Parameter("WaterDamageAmount"))
				Commands->Apply_Damage(obj,Get_Float_Parameter("WaterDamageAmount"),Get_Parameter("WaterDamageWarhead"),Commands->Find_Object(lastWaterZoneId));
		}
		else if (!waterZoneCount && remainingWaterDamageDelay < waterDamageDelayTime)
		{
			remainingWaterDamageDelay += waterDamageDelayTimeRecover;
			if (remainingWaterDamageDelay > waterDamageDelayTime)
				remainingWaterDamageDelay = waterDamageDelayTime;
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Swimming_Infantry_AI::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 347340)
	{
		Commands->Send_Custom_Event(sender,obj,347342,param,0.25f);
	}
	if (message == 347342)
	{
		if (obj->As_SoldierGameObj()->Is_Airborne())
		{
			Commands->Send_Custom_Event(sender,obj,message,param,0.25f);
			return;
		}
		waterZoneCount--;
		if (!waterZoneCount)
		{
			obj->As_SoldierGameObj()->Set_Can_Play_Damage_Animations(true);
			obj->As_SoldierGameObj()->Set_Movement_Loiters_Allowed(true);
			if (Has_Weapon(obj,enterWeapon))
				Commands->Select_Weapon(obj,enterWeapon);
			if (Has_Weapon(obj,Get_Parameter("WeaponPreset")))
				Remove_Weapon(obj,Get_Parameter("WeaponPreset"));
			obj->As_SoldierGameObj()->Set_Max_Speed(defaultSpeed);
			if (_stricmp(Get_Parameter("SwimmingSkin"),""))
				Set_Skin(obj,originalSkin);
			if (_stricmp(Get_Parameter("SwimmingArmor"),""))
				Commands->Set_Shield_Type(obj,originalArmor);
			if (_stricmp(Get_Parameter("SwimmingModel"),""))
				Commands->Set_Model(obj,originalModel);
		}
	}
	if (message == 347341)
	{
		lastWaterZoneId = Commands->Get_ID(sender);
		if (!waterZoneCount)
		{
			sprintf(enterWeapon,"%s",Get_Current_Weapon(obj) ? Get_Current_Weapon(obj) : "");
			Grant_Weapon(obj,Get_Parameter("WeaponPreset"),true,-1,1);
			obj->As_SoldierGameObj()->Set_Can_Play_Damage_Animations(false);
			obj->As_SoldierGameObj()->Set_Movement_Loiters_Allowed(false);
			Commands->Set_Loiters_Allowed(obj,false);
			if (_stricmp(Get_Parameter("SwimmingSkin"),""))
			{
				sprintf(originalSkin,"%s",Get_Skin(obj));
				Set_Skin(obj,Get_Parameter("SwimmingSkin"));
			}
			if (_stricmp(Get_Parameter("SwimmingArmor"),""))
			{
				sprintf(originalArmor,"%s",Get_Shield_Type(obj));
				Commands->Set_Shield_Type(obj,Get_Parameter("SwimmingArmor"));
			}
			if (_stricmp(Get_Parameter("SwimmingModel"),""))
			{
				sprintf(originalModel,"%s",Get_Model(obj));
				Commands->Set_Model(obj,Get_Parameter("SwimmingModel"));
			}
		}
		waterZoneCount++;
	}
}
void JMG_Utility_Swimming_Infantry_AI::Killed(GameObject *obj,GameObject *killer)
{
	DestroySoundEmitter(&heartBeatSoundId);
	DestroySoundEmitter(&pantSoundId);
}
void JMG_Utility_Swimming_Infantry_AI::Destroyed(GameObject *obj)
{
}
void JMG_Utility_Swimming_Infantry_AI::Detach(GameObject *obj)
{
	if (Exe == 4)
		return;
	Destroyed(obj);
}
void JMG_Utility_Swimming_Infantry_AI::CreateSoundEmitter(GameObject *obj,const char *model,int *soundId)
{
	GameObject *soundEmitter = Commands->Find_Object(*soundId);
	if (!soundEmitter)
	{
		soundEmitter = Commands->Create_Object("Daves Arrow",Commands->Get_Position(obj));
		*soundId = Commands->Get_ID(soundEmitter);
		Commands->Attach_To_Object_Bone(soundEmitter,obj,"c head");
	}
	Commands->Set_Model(soundEmitter,model);
}
void JMG_Utility_Swimming_Infantry_AI::HideSoundEmitter(int *soundId)
{
	GameObject *soundEmitter = Commands->Find_Object(*soundId);
	if (soundEmitter)
		Commands->Set_Model(soundEmitter,"null");
}
void JMG_Utility_Swimming_Infantry_AI::DestroySoundEmitter(int *soundId)
{
	GameObject *soundEmitter = Commands->Find_Object(*soundId);
	if (soundEmitter)
		Commands->Destroy_Object(soundEmitter);
	*soundId = 0;
}
void JMG_Utility_Damaged_Refund_Damage::Created(GameObject *obj)
{
	warhead = ArmorWarheadManager::Get_Warhead_Type(Get_Parameter("WarheadName"));
}
void JMG_Utility_Damaged_Refund_Damage::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (Get_Damage_Warhead() != warhead)
		return;
	float healthDiff = Commands->Get_Max_Health(obj)-Commands->Get_Health(obj);
	Commands->Set_Health(obj,JmgUtility::MathClamp(Commands->Get_Health(obj)+damage,0.0f,Commands->Get_Max_Health(obj)));
	damage -= healthDiff;
	if (damage > 0)
		Commands->Set_Shield_Strength(obj,JmgUtility::MathClamp(Commands->Get_Shield_Strength(obj)+damage,0.0f,Commands->Get_Max_Shield_Strength(obj)));
}
bool JMG_Utility_Turret_Spawn_Global_Flag_Controller::AllowAttach = true;
void JMG_Utility_Turret_Spawn_Global_Flag_Controller::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Turret_Spawn_Global_Flag_Controller::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		AllowAttach = param ? true : false;
	}
}
void JMG_Utility_Turret_Spawn_Global_Flag::Created(GameObject *obj)
{
	if (!JMG_Utility_Turret_Spawn_Global_Flag_Controller::AllowAttach)
	{
		Destroy_Script();
		return;
	}
	GameObject *turret = Commands->Create_Object(Get_Parameter("Turret_Preset"),Vector3());
	Commands->Attach_To_Object_Bone(turret,obj,Get_Parameter("Bone_Name"));
	turretId = Commands->Get_ID(turret);
	if (turret->As_VehicleGameObj())
		turret->As_VehicleGameObj()->Set_Is_Scripts_Visible(false);
	hasDriver = false;
}
void JMG_Utility_Turret_Spawn_Global_Flag::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!hasDriver)
		{
			hasDriver = true;
			GameObject *turret = Commands->Find_Object(turretId);
			if (turret)
			{
				Commands->Set_Player_Type(turret,Commands->Get_Player_Type(sender));
				Commands->Action_Reset(turret,100);
			}
		}
	}
	if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (hasDriver && obj->As_VehicleGameObj() && !Get_Vehicle_Occupant_Count(obj))
		{
			hasDriver = false;
			GameObject *turret = Commands->Find_Object(turretId);
			if (turret)
			{
				Commands->Set_Player_Type(turret,Commands->Get_Player_Type(obj));
				Commands->Action_Reset(turret,100);
			}
		}
	}
}
void JMG_Utility_Turret_Spawn_Global_Flag::Killed(GameObject *obj,GameObject *killer)
{
	Destroyed(obj);
}
void JMG_Utility_Turret_Spawn_Global_Flag::Destroyed(GameObject *obj)
{
	GameObject *turret = Commands->Find_Object(turretId);
	if (turret)
		Commands->Destroy_Object(turret);
}
void JMG_Utility_Turret_Spawn_Global_Flag::Detach(GameObject *obj)
{
	if (Exe == 4)
		return;
	Destroyed(obj);
}
void JMG_Utility_Timer_Death_If_Not_Damaged_Over_Peroid::Created(GameObject *obj)
{
	time = originalTime = Get_Int_Parameter("Time");
	giveDamage = Get_Float_Parameter("Damage");
	sprintf(warhead,"%s",Get_Parameter("Warhead"));
	Timer_Expired(obj,1);
}
void JMG_Utility_Timer_Death_If_Not_Damaged_Over_Peroid::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (time)
		{
			time--;
			if (!time)
				Commands->Apply_Damage(obj,giveDamage,warhead,obj);
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_Timer_Death_If_Not_Damaged_Over_Peroid::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0.0f)
		return;
	time = originalTime;
}
int JMG_Utility_Global_Attach_Script_On_Flag_Controller::globalFlag = -1;
void JMG_Utility_Global_Attach_Script_On_Flag_Controller::Created(GameObject *obj)
{
	globalFlag = Get_Int_Parameter("GlobalFlag");
}
void JMG_Utility_Global_Attach_Script_On_Flag_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	globalFlag = Get_Int_Parameter("GlobalFlag");
}
void JMG_Utility_Global_Attach_Script_On_Flag_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		JMG_Utility_Global_Attach_Script_On_Flag_Controller::globalFlag = globalFlag;
	}
}
void JMG_Utility_Global_Attach_Script_On_Flag::Created(GameObject *obj)
{
	if (JMG_Utility_Global_Attach_Script_On_Flag_Controller::globalFlag != Get_Int_Parameter("GlobalFlag"))
	{
		Destroy_Script();
		return;
	}
	char script[128];
	sprintf(script,"%s",Get_Parameter("Script"));
	const char *parameters = Get_Parameter("Params");
	char *params = newstr(parameters);
	char delim = Get_Parameter("Delim")[0];
	unsigned int length = strlen(parameters);
	for (unsigned int x = 0;x < length;x++)
		if (params[x] == delim)
			params[x] = ',';
	Attach_Script_Once(obj,script,params);
}
void JMG_Utility_Custom_Attach_Script_To_Presets::Created(GameObject *obj)
{
	sprintf(preset,"%s",Get_Parameter("Preset"));
	sprintf(script,"%s",Get_Parameter("Script"));
	const char *parameters = Get_Parameter("Params");
	params = newstr(parameters);
	char delim = Get_Parameter("Delim")[0];
	unsigned int length = strlen(parameters);
	for (unsigned int x = 0;x < length;x++)
		if (params[x] == delim)
			params[x] = ',';
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Attach_Script_To_Presets::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
		{
			GameObject *target = current->Data()->As_ScriptableGameObj();
			if (target && target->As_ScriptableGameObj() && !_stricmp(preset,Commands->Get_Preset_Name(target))) 
				Commands->Attach_Script(target,script,params);
		}
	}
}
void JMG_Utility_Custom_Send_Custom_On_Player_Count_Animation::Created(GameObject *obj)
{
	controllerId = Get_Int_Parameter("ControllingID");
	sprintf(animation,"%s",Get_Parameter("Animation"));
	maxFrame = Get_Float_Parameter("MaxFrame");
	lastCalculation = -1;
	Timer_Expired(obj,1);
}
void JMG_Utility_Custom_Send_Custom_On_Player_Count_Animation::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && controllerId)
	{
		GameObject *controller =  Commands->Find_Object(controllerId);
		if (controller)
		{
			JMG_Utility_Custom_Send_Custom_On_Player_Count_HUD *script = (JMG_Utility_Custom_Send_Custom_On_Player_Count_HUD*)Find_Script_On_Object(controller,"JMG_Utility_Custom_Send_Custom_On_Player_Count_HUD");
			if (script)
			{
				int requiredCount = script->baseCount+script->playerCount*Get_Player_Count();
				if (requiredCount)
				{
					float frame = JmgUtility::MathClamp((float)script->runningCount/(float)requiredCount*maxFrame,0.0f,maxFrame);
					if (frame != lastCalculation && requiredCount)
					{
						Commands->Set_Animation(obj,animation,false,0,frame,frame,true);
						lastCalculation = frame;
					}
				}
			}
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Created_Destroy_If_Not_In_Pathfind::Created(GameObject *obj)
{
	Vector3 out;
	if (!Get_Random_Pathfind_Spot(Commands->Get_Position(obj),0.0f,&out))
		Commands->Destroy_Object(obj);
}
void JMG_Utility_Teleport_If_Not_Moving_To_Location::Created(GameObject *obj)
{
	time = originalTime = (Get_Int_Parameter("Time")*10);
	location = Get_Vector3_Parameter("Location");
	lastSpot = Commands->Get_Position(obj);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Teleport_If_Not_Moving_To_Location::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(pos,lastSpot) <= 0.0f)
		{
			time--;
			if (!time)
			{
				time = originalTime;
				MoveablePhysClass *mphys = obj->As_PhysicalGameObj() ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
				if (mphys && mphys->Find_Teleport_Location(location,0.75f,&pos))
					Commands->Set_Position(obj,pos);
			}
		}
		else
		{
			time = originalTime;
			lastSpot = pos;
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
char JMG_Utility_Global_Set_Random_Model_Controller::extension[16];
void JMG_Utility_Global_Set_Random_Model_Controller::Created(GameObject *obj)
{
	sprintf(JMG_Utility_Global_Set_Random_Model_Controller::extension,Get_Parameter("DefaultExtension"));
}
void JMG_Utility_Global_Set_Random_Model_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	sprintf(extension,"%s",Get_Parameter("Extension"));
}
void JMG_Utility_Global_Set_Random_Model_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		sprintf(JMG_Utility_Global_Set_Random_Model_Controller::extension,extension);
		for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
		{
			GameObject *target = current->Data()->As_ScriptableGameObj();
			if (!target || !target->As_ScriptableGameObj())
				continue;
			JMG_Utility_Global_Set_Random_Model *script = (JMG_Utility_Global_Set_Random_Model*)Find_Script_On_Object(target,"JMG_Utility_Global_Set_Random_Model");
			if (!script)
				continue;
			script->UpdateModel(target);
		}
	}
}
void JMG_Utility_Global_Set_Random_Model::Created(GameObject *obj)
{
	sprintf(baseName,"%s",Get_Parameter("BaseName"));
	randomValue = Commands->Get_Random_Int(0,Get_Int_Parameter("FinalModelNumber")+1);
	UpdateModel(obj);
}
void JMG_Utility_Global_Set_Random_Model::UpdateModel(GameObject *obj)
{
	char model[32];
	sprintf(model,"%s%0d%s",baseName,randomValue,JMG_Utility_Global_Set_Random_Model_Controller::extension);
	Commands->Set_Model(obj,model);
}
void JMG_Utility_Swimming_Infantry_Advanced_AI::Created(GameObject *obj)
{
	if (Commands->Is_A_Star(obj))
	{
		Destroy_Script();
		return;
	}
	lastWaterZoneId = 0;
	heartBeatSoundId = 0;
	pantSoundId = 0;
	underwater = false;
	waterZoneCount = 0;
	startedFadeRed = false;
	drownTime = 0.0f;
	defaultSpeed = obj->As_SoldierGameObj()->Get_Max_Speed();
	defaultHoldStyle = Get_Int_Parameter("DefaultHoldStyle");
	defaultSwimSpeedMultiplier = Get_Float_Parameter("DefaultSwimSpeed");
	waterDamageDelayTime = remainingWaterDamageDelay = Get_Int_Parameter("WaterDamageDelayTime");
	waterDamageDelayTimeRecover = Get_Int_Parameter("WaterDamageDelayTimeRecover");
	sprintf(originalSkin,"");
	sprintf(originalArmor,"");
	sprintf(originalModel,"");
	waterSpeedMultiplier = 1.0f;
	sprintf(defaultWeaponPreset,"%s",Get_Parameter("WeaponPreset"));
	defaultDrownTime = Get_Float_Parameter("DrownTime");
	startDrownSequence = Get_Float_Parameter("StartDrownSequence");
	waterDamageAmount = Get_Float_Parameter("WaterDamageAmount");
	sprintf(waterDamageWarhead,"%s",Get_Parameter("WaterDamageWarhead"));
	sprintf(swimmingSkin,"%s",Get_Parameter("SwimmingSkin"));
	sprintf(swimmingArmor,"%s",Get_Parameter("SwimmingArmor"));
	sprintf(swimmingModel,"%s",Get_Parameter("SwimmingModel"));
	sprintf(heartBeatSoundEmitterModel,"%s",Get_Parameter("HeartBeatSoundEmitterModel"));
	sprintf(pantingSoundEmitterModel,"%s",Get_Parameter("PantingSoundEmitterModel"));
	sprintf(gaspForBreath,"%s",Get_Parameter("GaspForBreath"));
	catchBreathRate = Get_Float_Parameter("CatchBreathRate");
	forceDefinedWeapons = Get_Int_Parameter("ForceDefinedWeapons") ? true : false;
	weaponGroupId = Get_Int_Parameter("WeaponsGroupID");
	swimmingHeightScale = Get_Float_Parameter("SwimmingHeightScale");
	swimmingWidthScale = Get_Float_Parameter("SwimmingWidthScale");
	originalHeightScale = 0.0f;
	originalWidthScale = 0.0f;
	drownDamageRate = drownDamageRate;
	weaponSwitchForward = true;
	for (SLNode<ScriptZoneGameObj> *node = GameObjManager::ScriptZoneGameObjList.Head();node;node = node->Next())
	{
		GameObject *zone = (GameObject *)node->Data();
		if (Is_Script_Attached(zone,"JMG_Utility_Swimming_Zone") && IsInsideZone(zone,obj))
			Commands->Send_Custom_Event(obj,obj,347341,347341,0.25f);
	}
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Swimming_Infantry_Advanced_AI::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && Commands->Get_Health(obj) && !The_Game()->Is_Game_Over())
	{
		if (obj->As_SoldierGameObj()->Is_Crouched() && !obj->As_SoldierGameObj()->Is_Airborne() && waterZoneCount)
		{
			if (!underwater)
			{
				underwater = true;
				HideSoundEmitter(&pantSoundId);
			}
			drownTime += 0.1f;
			if (!startedFadeRed && drownTime >= defaultDrownTime-startDrownSequence)
			{
				startedFadeRed = true;
				CreateSoundEmitter(obj,heartBeatSoundEmitterModel,&heartBeatSoundId);
			}
			if (drownTime >= defaultDrownTime)
				Commands->Apply_Damage(obj,drownDamageRate,"None",obj);
		}
		else
		{
			if (underwater)
			{
				underwater = false;
				HideSoundEmitter(&heartBeatSoundId);
				if (drownTime > 1.0f)
					CreateSoundEmitter(obj,pantingSoundEmitterModel,&pantSoundId);
				if (drownTime > defaultDrownTime)
					Commands->Create_3D_Sound_At_Bone(gaspForBreath,obj,"c head");
			}
			if (drownTime)
			{
				drownTime -= catchBreathRate;
				if (startedFadeRed && drownTime < defaultDrownTime-startDrownSequence)
					startedFadeRed = false;
				if (drownTime <= 0)
				{
					drownTime = 0.0f;
					HideSoundEmitter(&pantSoundId);
				}
			}
		}
		if (waterZoneCount)
		{
			SwitchWeapon(obj);
			if (remainingWaterDamageDelay)
				remainingWaterDamageDelay--;
			if (!remainingWaterDamageDelay && waterDamageAmount)
				Commands->Apply_Damage(obj,waterDamageAmount,waterDamageWarhead,Commands->Find_Object(lastWaterZoneId));
		}
		else if (!waterZoneCount && remainingWaterDamageDelay < waterDamageDelayTime)
		{
			remainingWaterDamageDelay += waterDamageDelayTimeRecover;
			if (remainingWaterDamageDelay > waterDamageDelayTime)
				remainingWaterDamageDelay = waterDamageDelayTime;
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Swimming_Infantry_Advanced_AI::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 347340)
	{
		Commands->Send_Custom_Event(sender,obj,347342,param,0.25f);
	}
	if (message == 347342)
	{
		if (obj->As_SoldierGameObj()->Is_Airborne())
		{
			Commands->Send_Custom_Event(sender,obj,message,param,0.25f);
			return;
		}
		waterZoneCount--;
		if (!waterZoneCount)
		{
			obj->As_SoldierGameObj()->Set_Can_Play_Damage_Animations(true);
			obj->As_SoldierGameObj()->Set_Movement_Loiters_Allowed(true);
			obj->As_SoldierGameObj()->Set_Human_Anim_Override(true);
			obj->As_SoldierGameObj()->Set_Override_Weapon_Hold_Style(-1);
			if (forceDefinedWeapons && Has_Weapon(obj,enterWeapon))
				Commands->Select_Weapon(obj,enterWeapon);
			if (Has_Weapon(obj,defaultWeaponPreset))
				Remove_Weapon(obj,defaultWeaponPreset);
			obj->As_SoldierGameObj()->Set_Max_Speed(defaultSpeed);
			if (_stricmp(swimmingSkin,""))
				Set_Skin(obj,originalSkin);
			if (_stricmp(swimmingArmor,""))
				Commands->Set_Shield_Type(obj,originalArmor);
			if (_stricmp(swimmingModel,""))
				Commands->Set_Model(obj,originalModel);
			if (abs(swimmingHeightScale-999.99f) > 0.1f)
				obj->As_SoldierGameObj()->Set_Skeleton_Height(originalHeightScale);
			if (abs(swimmingWidthScale-999.99f) > 0.1f)
				obj->As_SoldierGameObj()->Set_Skeleton_Width(originalWidthScale);
		}
	}
	if (message == 347341)
	{
		lastWaterZoneId = Commands->Get_ID(sender);
		if (!waterZoneCount)
		{
			if (abs(swimmingHeightScale-999.99f) > 0.1f)
			{
				originalHeightScale = obj->As_SoldierGameObj()->Get_Skeleton_Heigth();
				obj->As_SoldierGameObj()->Set_Skeleton_Height(swimmingHeightScale);
			}
			if (abs(swimmingWidthScale-999.99f) > 0.1f)
			{
				originalWidthScale = obj->As_SoldierGameObj()->Get_Skeleton_Width();
				obj->As_SoldierGameObj()->Set_Skeleton_Width(swimmingWidthScale);
			}
			defaultSpeed = obj->As_SoldierGameObj()->Get_Max_Speed();
			waterSpeedMultiplier = (param/100.0f);
			sprintf(enterWeapon,"%s",Get_Current_Weapon(obj) ? Get_Current_Weapon(obj) : "");
			if (forceDefinedWeapons)
				Grant_Weapon(obj,defaultWeaponPreset,true,-1,1);
			obj->As_SoldierGameObj()->Set_Can_Play_Damage_Animations(false);
			obj->As_SoldierGameObj()->Set_Movement_Loiters_Allowed(false);
			obj->As_SoldierGameObj()->Set_Human_Anim_Override(false);
			currentWeaponId = 1;
			SwitchWeapon(obj);
			Commands->Set_Loiters_Allowed(obj,false);
			if (_stricmp(swimmingSkin,""))
			{
				sprintf(originalSkin,"%s",Get_Skin(obj));
				Set_Skin(obj,swimmingSkin);
			}
			if (_stricmp(swimmingArmor,""))
			{
				sprintf(originalArmor,"%s",Get_Shield_Type(obj));
				Commands->Set_Shield_Type(obj,swimmingArmor);
			}
			if (_stricmp(swimmingModel,""))
			{
				sprintf(originalModel,"%s",Get_Model(obj));
				Commands->Set_Model(obj,swimmingModel);
			}
		}
		waterZoneCount++;
	}
}
void JMG_Utility_Swimming_Infantry_Advanced_AI::Killed(GameObject *obj,GameObject *killer)
{
	DestroySoundEmitter(&heartBeatSoundId);
	DestroySoundEmitter(&pantSoundId);
}
void JMG_Utility_Swimming_Infantry_Advanced_AI::Destroyed(GameObject *obj)
{
	DestroySoundEmitter(&heartBeatSoundId);
	DestroySoundEmitter(&pantSoundId);
}
void JMG_Utility_Swimming_Infantry_Advanced_AI::Detach(GameObject *obj)
{
	if (Exe == 4)
		return;
	Destroyed(obj);
}
void JMG_Utility_Swimming_Infantry_Advanced_AI::CreateSoundEmitter(GameObject *obj,const char *model,int *soundId)
{
	GameObject *soundEmitter = Commands->Find_Object(*soundId);
	if (!soundEmitter)
	{
		soundEmitter = Commands->Create_Object("Daves Arrow",Commands->Get_Position(obj));
		*soundId = Commands->Get_ID(soundEmitter);
		Commands->Attach_To_Object_Bone(soundEmitter,obj,"c head");
	}
	Commands->Set_Model(soundEmitter,model);
}
void JMG_Utility_Swimming_Infantry_Advanced_AI::HideSoundEmitter(int *soundId)
{
	GameObject *soundEmitter = Commands->Find_Object(*soundId);
	if (soundEmitter)
		Commands->Set_Model(soundEmitter,"null");
}
void JMG_Utility_Swimming_Infantry_Advanced_AI::DestroySoundEmitter(int *soundId)
{
	GameObject *soundEmitter = Commands->Find_Object(*soundId);
	if (soundEmitter)
		Commands->Destroy_Object(soundEmitter);
	*soundId = 0;
}
void JMG_Utility_Swimming_Infantry_Advanced_AI::SwitchWeapon(GameObject *obj)
{
	const WeaponDefinitionClass *weaponDef = Get_Current_Weapon_Definition(obj);
	if ((weaponDef && currentWeaponId != weaponDef->Get_ID()) || (!weaponDef && currentWeaponId != 1))
	{
		int currentPosition = GetWeaponPosition(obj,currentWeaponId);
		GetWeaponId(weaponDef);
		if (forceDefinedWeapons)
		{
			int newPosition = GetWeaponPosition(obj,currentWeaponId);
			bool forward = newPosition > currentPosition;
			int diff = abs(newPosition-currentPosition);
			if ((diff <= 1 && forward) || (diff > 1 && weaponSwitchForward))
				currentWeapon = JMG_Utility_Swimming_Infantry_Advanced_Controller::getNext(obj,weaponGroupId,currentWeapon);
			else
				currentWeapon = JMG_Utility_Swimming_Infantry_Advanced_Controller::getPrev(obj,weaponGroupId,currentWeapon);
			weaponSwitchForward = forward;
			if (currentWeapon)
			{
				Commands->Select_Weapon(obj,currentWeapon->weaponName);
				GetWeaponId(Get_Current_Weapon_Definition(obj));
			}
		}
		else
			currentWeapon = JMG_Utility_Swimming_Infantry_Advanced_Controller::getWeapon(weaponGroupId,currentWeaponId);
		if (currentWeapon)
		{
			obj->As_SoldierGameObj()->Set_Override_Weapon_Hold_Style(currentWeapon->holdStyle);
			obj->As_SoldierGameObj()->Set_Max_Speed(defaultSpeed*currentWeapon->speed*waterSpeedMultiplier);
		}
		else
		{
			obj->As_SoldierGameObj()->Set_Override_Weapon_Hold_Style(defaultHoldStyle);
			obj->As_SoldierGameObj()->Set_Max_Speed(defaultSpeed*defaultSwimSpeedMultiplier*waterSpeedMultiplier);
		}
	}
}
void JMG_Utility_Swimming_Infantry_Advanced_AI::GetWeaponId(const WeaponDefinitionClass *weaponDef)
{
	if (weaponDef)
		currentWeaponId = weaponDef->Get_ID();
	else
		currentWeaponId = 1;
}
int JMG_Utility_Swimming_Infantry_Advanced_AI::GetWeaponPosition(GameObject *obj,int weaponId)
{
	if (ArmedGameObj *armedGameObj = obj->As_PhysicalGameObj()->As_ArmedGameObj())
	{
		WeaponBagClass *weaponBagClass = armedGameObj->Get_Weapon_Bag();
		int x = weaponBagClass->Get_Count();
		for (int i = 0;i < x;i++)
			if (weaponBagClass->Peek_Weapon(i))
				if (weaponBagClass->Peek_Weapon(i)->Get_ID() == weaponId)
					return i;
	}
	return -1;
}
int JMG_Utility_Global_Custom_Send_Custom_Flag_Controller::globalFlag = -1;
void JMG_Utility_Global_Custom_Send_Custom_Flag_Controller::Created(GameObject *obj)
{
	globalFlag = Get_Int_Parameter("GlobalFlag");
}
void JMG_Utility_Global_Custom_Send_Custom_Flag_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	globalFlag = Get_Int_Parameter("GlobalFlag");
}
void JMG_Utility_Global_Custom_Send_Custom_Flag_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		JMG_Utility_Global_Custom_Send_Custom_Flag_Controller::globalFlag = globalFlag;
	}
}
void JMG_Utility_Global_Custom_Send_Custom_Flag::Created(GameObject *obj)
{
	globalFlag = Get_Int_Parameter("GlobalFlag");
	recieveMessage = Get_Int_Parameter("Custom");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	randomDelay = Get_Float_Parameter("RandomDelay");
	randomChance = Get_Float_Parameter("RandomChance");
}
void JMG_Utility_Global_Custom_Send_Custom_Flag::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage && JMG_Utility_Global_Custom_Send_Custom_Flag_Controller::globalFlag == globalFlag)
	{
		if (randomChance && randomChance < Commands->Get_Random(0.0f,1.0f))
			return;
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
 		Commands->Send_Custom_Event(obj,object,custom,Param == -1 ? param : Param,delay+(randomDelay > 0 ? Commands->Get_Random(0.0f,randomDelay) : 0.0f));
	}
}
void JMG_Utility_AI_Goto_Location_While_Player_Nearby::Created(GameObject *obj)
{
	const char *weap = Get_Current_Weapon(obj);
	if (weap)
	{
		primary = DefineAmmo(Get_Weapon_Ammo_Definition(weap,true));
		secondary = DefineAmmo(Get_Weapon_Ammo_Definition(weap,true));
	}
	gotoObjectId = Get_Int_Parameter("GotoObjectId");
	gotoLocation = Get_Vector3_Parameter("GotoLocation");
	maxPlayerDistance = Get_Float_Parameter("MaxPlayerDistance")*Get_Float_Parameter("MaxPlayerDistance");
	minDistanceSquared = Get_Float_Parameter("MinAttackRange")*Get_Float_Parameter("MinAttackRange");
	lastHealth = 0;
	moving = false;
	attacking = false;
	reverseTime = 0;
	stuckCount = 0;
	drivingBackward = false;
	badDestAttempt = 0;
	doNotUsePathfind = 0;
	currentAction.position = homepos = Commands->Get_Position(obj);
	myteam = Commands->Get_Player_Type(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Innate_Disable(obj);
	Commands->Enable_Hibernation(obj,false);
	minAttackRange = Get_Float_Parameter("MinAttackRange");
	definedWeaponError = Get_Float_Parameter("WeaponError");
	forceFire = Get_Int_Parameter("ForceFire");
	vtolHover = Get_Float_Parameter("VTOLHover");
	vsSoldier = Get_Int_Parameter("vsSoldier");
	vsAircraft = Get_Int_Parameter("vsAircraft");
	vsVehicle = Get_Int_Parameter("vsVehicle");
	overrideSpeed = Get_Float_Parameter("OverrideSpeed");
	playerType = Get_Int_Parameter("PlayerType");
	if (obj->As_VehicleGameObj()) 
	{
		Commands->Enable_Vehicle_Transitions(obj,false);
		Commands->Enable_Engine(obj,true);
	}
	if (Get_Int_Parameter("OverrideFireMode"))
	{
		overrideFireMode = true;
		overridePrimary = Get_Int_Parameter("OverrideFireMode") == 1 ? true : false;
	}
	Commands->Start_Timer(obj,this,1.0,1);
	Commands->Start_Timer(obj,this,1.0,2);
	Commands->Start_Timer(obj,this,0.1f,8);
}
void JMG_Utility_AI_Goto_Location_While_Player_Nearby::Action_Complete(GameObject *obj,int action,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_START)
	{
		bool refreshAction = false;
		if (!doNotUsePathfind)
			refreshAction = true;
		doNotUsePathfind = 3;
		if (refreshAction)
			Commands->Start_Timer(obj,this,0.1f,7);
	}
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		if (!badDestAttempt)
			Commands->Start_Timer(obj,this,0.25f,5);
		else
			Commands->Start_Timer(obj,this,0.25f,6);
	}
	Commands->Action_Reset(obj,action*1.0f);
	Commands->Action_Reset(obj,100*1.0f);
}
void JMG_Utility_AI_Goto_Location_While_Player_Nearby::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0 || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (Is_Script_Attached(damager,"JMG_Utility_AI_Goto_Location_While_Player_Nearby_Ignored"))
		return;
	if (!_stricmp(Get_Skin(damager),"Blamo") || !_stricmp(Get_Shield_Type(damager),"Blamo"))
		return;
	if(!attacking)
		RunAttack(obj,SetTarget(damager));
	else 
		RunAttack(obj,SelectTarget(obj,damager));
	lastSeenCount = Commands->Get_Random_Int(6,15);
}
void JMG_Utility_AI_Goto_Location_While_Player_Nearby::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Goto_Location_While_Player_Nearby_Ignored"))
		return;
	Vector3 enemyPos = Commands->Get_Position(seen);
	float dist = JmgUtility::SimpleDistance(Commands->Get_Position(obj),enemyPos);
	if (!_stricmp(Get_Skin(seen),"Blamo") || !_stricmp(Get_Shield_Type(seen),"Blamo"))
		return;
	if (minAttackRange && dist < minDistanceSquared)
		return;
	if (!Commands->Get_Health(seen))
		return;
	if (seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed())
		if (seen->As_SoldierGameObj() && dist > seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance()*seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance())
			return;
		else if (seen->As_VehicleGameObj() && dist > seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance()*seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance())
			return;	
		else if (dist > seen->As_SmartGameObj()->Get_Stealth_Fade_Distance()*seen->As_SmartGameObj()->Get_Stealth_Fade_Distance())
			return;
	if (Commands->Get_ID(seen) == currentAction.targetId)
		lastSeenCount = Commands->Get_Random_Int(3,16);
	if(!attacking)
		RunAttack(obj,SetTarget(seen));
	else
		RunAttack(obj,SelectTarget(obj,seen));
}
void JMG_Utility_AI_Goto_Location_While_Player_Nearby::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1) 
	{
		StuckCheck(obj);
		if (lastSeenCount)
		{
			lastSeenCount--;
			if (!lastSeenCount)
				currentAction.targetId = -1;
		}
		if (doNotUsePathfind)
			doNotUsePathfind--;
		Commands->Start_Timer(obj,this,1.0,1);
	}
	if (number == 2)
	{
		Vector3 targetSpot = gotoLocation;
		if (gotoObjectId)
			targetSpot = Commands->Get_Position(Commands->Find_Object(gotoObjectId));
		Vector3 myPos = Commands->Get_Position(obj);
		GameObject *player = Commands->Get_A_Star(myPos);
		if (player && JmgUtility::SimpleDistance(Commands->Get_Position(player),myPos) <= maxPlayerDistance && !CheckPlayerType(player,playerType) && JmgUtility::SimpleDistance(myPos,targetSpot) > 6.25f && Commands->Get_Player_Type(player) != -4)
		{
			if (!moving)
			{
				moving = true;
				AttackMove(obj,Commands->Find_Object(lastAction.targetId),gotoObjectId ? true : false,gotoLocation,useAmmo,0.0f,true,2.5f);
			}
		}
		else if (moving)
		{
			moving = false;
			AttackMove(obj,Commands->Find_Object(lastAction.targetId),false,myPos,useAmmo,0.0f,true,1000.0f);
		}
		Commands->Start_Timer(obj,this,0.25f,2);
	}
	if (number == 3) 
	{
		if(attacking)
		{
			attacking = false;
			if (!lastSeenCount && currentAction.targetId)
				currentAction.targetId = 0;
			GameObject *target = Commands->Find_Object(currentAction.targetId);
			if(target && Commands->Get_Health(target))
				RunAttack(obj,SetTarget(target));
			else 
			{
				currentAction.useAmmo = 1;
				currentAction.targetId = 0;
				AttackMove(obj,NULL,currentAction.following,currentAction.position,currentAction.useAmmo,0.0f,false,currentAction.arriveDistance);
			}
		} 
		else
			AttackMove(obj,Commands->Find_Object(currentAction.targetId),currentAction.following,currentAction.position,currentAction.useAmmo,0.0f,false,currentAction.arriveDistance);
	}
	if (number == 4)
	{
		if (Get_Vehicle_Occupant_Count(obj))
			Force_Occupants_Exit(obj);
	}
	if (number == 5) 
	{
		badDestAttempt = 1;
		AttackMove(obj,Commands->Find_Object(currentAction.targetId),currentAction.following,Commands->Get_Position(obj),currentAction.useAmmo,0.0f,false,currentAction.arriveDistance);
	}
	if (number == 6) 
	{
		badDestAttempt = 0;
		AttackMove(obj,Commands->Find_Object(currentAction.targetId),currentAction.following,homepos,currentAction.useAmmo,0.0f,false,currentAction.arriveDistance);
	}
	if (number == 7) 
	{
		AttackMove(obj,Commands->Find_Object(currentAction.targetId),currentAction.following,currentAction.position,currentAction.useAmmo,0.0f,true,currentAction.arriveDistance);
	}
}
void JMG_Utility_AI_Goto_Location_While_Player_Nearby::RunAttack(GameObject *obj,GameObject *target) 
{
	if (target) 	
	{
		int ammo = SelectAmmo(target);
		if (ammo == -1) 
		{
			attacking = false;
			useAmmo = 1;
			currentAction.targetId = 0;
			return;
		}
		if (ammo == 2) 
		{
			if (lastHealth == Commands->Get_Health(target))
				useAmmo = useAmmo == 1 ? 0 : 1;
			ammo = useAmmo;
		}
		useAmmo = ammo;
		lastHealth = Commands->Get_Health(target);
		attacking = true;
		AttackMove(obj,target,currentAction.following,currentAction.position,useAmmo,0.0f,false,currentAction.arriveDistance);
		Commands->Start_Timer(obj,this,1.0f,3);
	}
}
int JMG_Utility_AI_Goto_Location_While_Player_Nearby::GetThreatRating(GameObject * obj) 
{
	if (obj == NULL)
		return 0;
	int rating=0;
	if(Commands->Is_A_Star(obj)) 
	{ 
		rating += 1;
		GameObject *vehicle = Get_Vehicle(obj);
		if (vehicle) 
			rating += 3; 
	}
	if (obj->As_SoldierGameObj()) 
		rating += 1; 
	if (obj->As_VehicleGameObj()) 
		rating += 3; 
	return rating;
}
GameObject * JMG_Utility_AI_Goto_Location_While_Player_Nearby::GetAttackObject(GameObject * obj) 
{
	if (obj == NULL)
		return NULL;
	if(Commands->Is_A_Star(obj)) 
	{
		GameObject *vehicle = Get_Vehicle(obj); 
		if(vehicle)
			return vehicle;
		else 
			return obj;
	}
	return obj;
}
GameObject *JMG_Utility_AI_Goto_Location_While_Player_Nearby::SelectTarget(GameObject *obj,GameObject *target)
{
	GameObject *old = currentAction.targetId ? Commands->Find_Object(currentAction.targetId) : NULL;
	if (!target || !Commands->Get_ID(target))
		return GetAttackObject(old);
	if (!old) 
	{
		currentAction.targetId = Commands->Get_ID(target);
		return GetAttackObject(target);
	}
	int trOld = GetThreatRating(old);
	int trNew = GetThreatRating(target);
	if (trOld > trNew)
		return GetAttackObject(old);
	if (trNew > trOld) 
	{
		currentAction.targetId = Commands->Get_ID(target);
		return GetAttackObject(target);
	}
	GameObject * closest = GetClosest(obj,target,old);
	currentAction.targetId = Commands->Get_ID(closest);
	return GetAttackObject(closest);
}
GameObject *JMG_Utility_AI_Goto_Location_While_Player_Nearby::SetTarget(GameObject *target)
{
	currentAction.targetId = Commands->Get_ID(target);
	if(!attacking)
		return GetAttackObject(target);
	return 0;
}
GameObject *JMG_Utility_AI_Goto_Location_While_Player_Nearby::GetClosest(GameObject *obj,GameObject *new_target,GameObject *old_target)
{
	Vector3 objPos = Commands->Get_Position(obj);
	if(JmgUtility::SimpleDistance(objPos,Commands->Get_Position(new_target)) < JmgUtility::SimpleDistance(objPos,Commands->Get_Position(old_target)))
		return new_target;
	else
		return old_target;
}
int JMG_Utility_AI_Goto_Location_While_Player_Nearby::SelectAmmo(GameObject *target)
{
	if (target)
	{
		if (target->As_SoldierGameObj())
			return vsSoldier;
		if (target->As_VehicleGameObj()) 
		{
			if (Is_VTOL(target))
				return vsAircraft;
			return vsVehicle;
		}
	}
	return -1;
}
void JMG_Utility_AI_Goto_Location_While_Player_Nearby::StuckCheck(GameObject *obj)
{
	Vector3 current = Commands->Get_Position(obj);
	if (reverseTime)
	{
		reverseTime--;
		stuckCount = 0;
		lastPos = current;
		drivingBackward = false;
		AttackMove(obj,Commands->Find_Object(lastAction.targetId),lastAction.following,lastAction.position,lastAction.useAmmo,0.0f,true,0.0f);
		return;
	}
	if (!moving)
	{
		drivingBackward = false;
		return;
	}
	if(JmgUtility::SimpleDistance(lastPos,current) < 1.0f)
	{
		if(stuckCount > 2)
		{
			reverseTime = Commands->Get_Random_Int(2,6);
			stuckCount = 0;
			drivingBackward = true;
			AttackMove(obj,Commands->Find_Object(lastAction.targetId),lastAction.following,lastAction.position,lastAction.useAmmo,0.0f,true,currentAction.arriveDistance);
		}
		stuckCount++;
	}
	else
	{
		stuckCount = 0;
		lastPos = current;
	}
}
void JMG_Utility_AI_Goto_Location_While_Player_Nearby::AttackMove(GameObject *obj,GameObject *target,bool gotoObject,Vector3 targetLocation,int fireMode,float weaponError,bool forceUpdate,float arriveDistance)
{
	Vector3 myPos = Commands->Get_Position(obj);
	float dist = 9999.9f;
	GameObject *followingObject = Commands->Find_Object(gotoObjectId);
	if (gotoObject && followingObject)
		dist = JmgUtility::SimpleDistance(Commands->Get_Position(followingObject),myPos);
	else
		dist = JmgUtility::SimpleDistance(currentAction.position,myPos);
	if (!gotoObject)
		currentAction.position = targetLocation;
	currentAction.useAmmo = fireMode;
	currentAction.following = gotoObject;
	currentAction.backward = drivingBackward;
	currentAction.arriveDistance = arriveDistance;
	if (lastAction == currentAction && !forceUpdate)
		return;
	lastAction = currentAction;
	Commands->Action_Reset(obj,10.0f);
	ActionParamsStruct params;
	if (target)
	{
		JMGVehicleAmmo ammo = (fireMode ? primary : secondary);
		weaponError = 0.0f;
		if (definedWeaponError != -1.00f)
			weaponError = definedWeaponError;
		else if (ammo.allowError)
			weaponError = JmgUtility::SimpleDistance(Commands->Get_Position(target),myPos)/ammo.speed*5.0f;
		params.Set_Attack(target,ammo.range,weaponError,!overrideFireMode ? (fireMode ? true : false) : overridePrimary);
		if (forceFire)	
		{
			params.AttackCheckBlocked = false;
			params.AttackForceFire = true;
		}
	}
	if (Is_VTOL(obj) && vtolHover)
		targetLocation.Z += vtolHover;
	if (gotoObject)
		params.Set_Movement(followingObject,overrideSpeed == -1.0 ? JmgUtility::MathClamp(dist,1.0f,100.0f) : overrideSpeed,arriveDistance,false);
	else
		params.Set_Movement(targetLocation,overrideSpeed == -1.0 ? JmgUtility::MathClamp(dist,1.0f,100.0f) : overrideSpeed,arriveDistance,false);
	params.MoveBackup = currentAction.backward;
	if (Is_VTOL(obj))
		params.MovePathfind = false;
	else
	{
		params.ShutdownEngineOnArrival = true;
		params.MovePathfind = !doNotUsePathfind ? true : false;
	}
	if (!target)
		params.Set_Basic(this,10,10);
	Commands->Action_Goto(obj,params);
	if (target)
	{
		params.Set_Basic(this,10,10);
		Commands->Action_Attack(obj,params);
	}
}
JMG_Utility_AI_Goto_Location_While_Player_Nearby::JMGVehicleAmmo JMG_Utility_AI_Goto_Location_While_Player_Nearby::DefineAmmo(const AmmoDefinitionClass *ammo)
{
	JMGVehicleAmmo theammo;
	if (ammo)
	{
		theammo.range = ammo->Range;
		if (ammo->Velocity >= 400.0f || (ammo->IsTracking && ammo->TurnRate))
			theammo.allowError = false;
		else
			theammo.allowError = true;
		theammo.speed = ammo->Velocity*ammo->Velocity;
	}
	return theammo;
}
void JMG_Utility_AI_Goto_Location_While_Player_Nearby_Ignored::Created(GameObject *obj)
{
}
void JMG_Utility_Killed_Drop_Powerup_Become_Corpse::Killed(GameObject *obj,GameObject *killer)
{
	if (Get_Float_Parameter("Chance") >= 1.0f || Commands->Get_Random(0.0f,1.0f) < Get_Float_Parameter("Chance"))
		Commands->Start_Timer(obj,this,0.0f,1);
}
void JMG_Utility_Killed_Drop_Powerup_Become_Corpse::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (_stricmp(Get_Parameter("RequiredWeaponPreset"),"") && !Has_Weapon(obj,Get_Parameter("RequiredWeaponPreset")))
			return;
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *powerup = Commands->Create_Object(Get_Parameter("PowerupName"),pos);
		Commands->Set_Facing(powerup,Commands->Get_Facing(obj));
		Commands->Set_Model(powerup,Get_Model(obj));
		if (!obj->As_SmartGameObj())
			return;
		AnimControlClass *control = obj->As_SmartGameObj()->Get_Anim_Control();
		if (!control || control->Get_Animation_Name() == NULL || !_stricmp(control->Get_Animation_Name(),""))
			return;
		Commands->Set_Animation(powerup,control->Get_Animation_Name(),false,0,control->Get_Current_Frame(),-1,true);
		Commands->Destroy_Object(obj);
	}
}
void JMG_Utility_Objective_System_Objectives_Complete_Send_Custom::Created(GameObject *obj)
{
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	randomDelay = Get_Float_Parameter("RandomDelay");
	char completedObjectives[256];
	sprintf(completedObjectives,"%s",Get_Parameter("CompletedObjectives"));
	char delim[2];
	int count = 0;
	sprintf(delim,"%s",Get_Parameter("Delim"));
	char *token = strtok(completedObjectives, delim);
	while(token != NULL)
	{
		objectiveIds[count] = atoi(token);
		token = strtok(NULL,delim);
		count++;
	}
	objectiveCount = count;
	repeat = Get_Int_Parameter("Repeat") ? true : false;
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_Objective_System_Objectives_Complete_Send_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (BasicObjectiveSystem.Check_If_All_Objectives_Are_Complete(objectiveIds,objectiveCount))
		{
			GameObject *object = id ? Commands->Find_Object(id) : obj;
 			Commands->Send_Custom_Event(obj,object,custom,Param,delay+(randomDelay > 0 ? Commands->Get_Random(0.0f,randomDelay) : 0.0f));
			if (!repeat)
			{
				Destroy_Script();
				return;
			}
		}
		Commands->Start_Timer(obj,this,0.25f,1);
	}
}
void JMG_Utility_Damage_Update_Animation_Frame::Created(GameObject *obj)
{
	if (_stricmp(Get_Parameter("Model"),""))
		Commands->Set_Model(obj,Get_Parameter("Model"));
	sprintf(animation,"%s",Get_Parameter("Animation"));
	maxFrame = Get_Float_Parameter("MaxFrame");
	lastCalculation = -1;
	Damaged(obj,NULL,0);
}
void JMG_Utility_Damage_Update_Animation_Frame::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	float percent = Get_Hitpoints(obj)/Get_Max_Hitpoints(obj);
	float frame = JmgUtility::MathClamp(percent*maxFrame,0.0f,maxFrame);
	if (frame != lastCalculation)
	{
		Commands->Set_Animation(obj,animation,false,0,frame,frame,true);
		lastCalculation = frame;
	}
}
void JMG_Utility_Security_System_Sync_Model_Random_NumberPad_Control::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Security_System_Sync_Model_Random_NumberPad_Control::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		GameObject *keypad = Commands->Find_Object(Get_Int_Parameter("KeypadID"));
		JMG_Utility_Security_System_Random_NumberPad_Control *script = (JMG_Utility_Security_System_Random_NumberPad_Control*)Find_Script_On_Object(keypad,"JMG_Utility_Security_System_Random_NumberPad_Control");
		if (!script)
			return;
		char theModel[16];
		sprintf(theModel,"%s%c",Get_Parameter("BaseModel"),script->combination[Get_Int_Parameter("Index")]);
		Commands->Set_Model(obj,theModel);
	}
}
void JMG_Utility_AI_Goto_Target_Script::Created(GameObject *obj)
{
	state = IDLE;
	moveBackward = false;
	lastPosition = homeLocation = Commands->Get_Position(obj);
	targetId = 0;
	lastSeenTime = 0;
	huntingEnemyId = 0;
	stuckTime = 0;
	reverseTime = 0;
	huntStealth = Get_Int_Parameter("HuntStealth") ? true : false;
	removeIgnoreTime = 0;
	ignoreEnemyId = 0;
	wanderingAiGroupId = Get_Int_Parameter("WanderingAIGroupID");
	wanderSpeed = Get_Float_Parameter("WanderSpeed");
	huntSpeed = Get_Float_Parameter("HuntSpeed");
	attackSpeed = Get_Float_Parameter("AttackSpeed");
	returnHomeSpeed = Get_Float_Parameter("ReturnHomeSpeed");
	changeWanderGroupCustom = Get_Int_Parameter("ChangeWanderGroupCustom");
	changeWanderSpeedCustom = Get_Int_Parameter("ChangeWanderSpeedCustom");
	changeHuntDistanceCustom = Get_Int_Parameter("ChangeHuntDistanceCustom");
	changeHuntSpeedCustom = Get_Int_Parameter("ChangeHuntSpeedCustom");
	changeReturnHomeSpeedCustom = Get_Int_Parameter("ChangeReturnHomeSpeedCustom");
	changeMaxSightFromHomeLocationCustom = Get_Int_Parameter("ChangeMaxSightFromHomeLocationCustom");
	changeAttackSpeedCustom = Get_Int_Parameter("ChangeAttackSpeedCustom");
	maxSightFromHomeLocation = Get_Float_Parameter("MaxSightRangeFromHome");
	maxSightFromHomeLocation *= maxSightFromHomeLocation;
	huntSearchDistance = Get_Float_Parameter("HuntSearchDistance") < 0.0f ? Get_Float_Parameter("HuntSearchDistance") : Get_Float_Parameter("HuntSearchDistance")*Get_Float_Parameter("HuntSearchDistance");
	const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Current_Weapon(obj),true);
	if (ammo)
	{
		weaponRange = ammo->Range;
		weaponEffectiveRange = ammo->Range < ammo->EffectiveRange ? ammo->Range : ammo->EffectiveRange;
	}
	attackArriveDistance = Get_Float_Parameter("AttackDistance") >= 0 ? Get_Float_Parameter("AttackDistance") : weaponEffectiveRange;
	huntArriveDistance = Get_Float_Parameter("HuntArriveDistance");
	wanderDistanceOverride = Get_Float_Parameter("WanderDistanceOverride");
	wanderDistanceOverride *= wanderDistanceOverride;
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,1.0f,1);
	Commands->Start_Timer(obj,this,1.0f,2);
	if (Get_Float_Parameter("RandomAttackDistance"))
		Commands->Start_Timer(obj,this,0.0f,3);
	if (Get_Float_Parameter("RandomHuntArriveDistance"))
		Commands->Start_Timer(obj,this,0.0f,4);
}
void JMG_Utility_AI_Goto_Target_Script::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (!Commands->Get_Health(seen))
		return;
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Goto_Target_Script_Ignore_Object"))
		return;
	if (!_stricmp(Get_Skin(seen),"Blamo") || !_stricmp(Get_Shield_Type(seen),"Blamo"))
		return;
	if (Get_Int_Parameter("CanSeeStealth") != 2 && seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed())
	{
		if (!Get_Int_Parameter("CanSeeStealth"))
			return;
		float dist = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen));
		if (seen->As_SoldierGameObj() && dist > seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance()*seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance())
			return;
		else if (seen->As_VehicleGameObj() && dist > seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance()*seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance())
			return;	
		else if (dist > seen->As_SmartGameObj()->Get_Stealth_Fade_Distance()*seen->As_SmartGameObj()->Get_Stealth_Fade_Distance())
			return;
	}
	if (maxSightFromHomeLocation > 0.0f && JmgUtility::SimpleDistance(homeLocation,Commands->Get_Position(seen)) > maxSightFromHomeLocation)
		return;
	if (!targetId || !lastSeenTime || Choose_Target(obj,seen))
	{
		targetId = Commands->Get_ID(seen);
		lastSeenTime = Commands->Get_Random_Int(30,60);
		state = ATTACKING_TARGET;
		Attack_Move(obj,seen,Vector3(),attackSpeed,attackArriveDistance,true,false);
	}
	if (targetId == Commands->Get_ID(seen))
		lastSeenTime = Commands->Get_Random_Int(30,60);
}
void JMG_Utility_AI_Goto_Target_Script::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (changeWanderGroupCustom != 0 && changeWanderGroupCustom == message)
	{
		wanderingAiGroupId = param;
		if (state == WANDERING_GROUP)
		{
			Vector3 wanderPos = Vector3();
			if (GetRandomPosition(&wanderPos))
			{
				homeLocation = wanderPos;
				Attack_Move(obj,Commands->Find_Object(lastAction.targetId),homeLocation,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
			}
		}
	}
	if (changeWanderSpeedCustom != 0 && changeWanderSpeedCustom == message)
	{
		wanderSpeed = param/100.0f;
		if (state == WANDERING_GROUP)
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,wanderSpeed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
	}
	if (changeHuntDistanceCustom != 0 && changeHuntDistanceCustom == message)
	{
		huntSearchDistance = param/100.0f;
		if (huntSearchDistance > 0)
			huntSearchDistance *= huntSearchDistance;
	}
	if (changeHuntSpeedCustom != 0 && changeHuntSpeedCustom == message)
	{
		huntSpeed = param/100.0f;
		if (state == HUNTING_STAR)
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,huntSpeed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
	}
	if (changeReturnHomeSpeedCustom != 0 && changeReturnHomeSpeedCustom == message)
	{
		returnHomeSpeed = param/100.0f;
		if (state == RETURNING_HOME)
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,returnHomeSpeed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
	}
	if (changeMaxSightFromHomeLocationCustom != 0 && changeMaxSightFromHomeLocationCustom == message)
	{
		maxSightFromHomeLocation = param/100.0f;
		maxSightFromHomeLocation *= maxSightFromHomeLocation;
	}
	if (changeAttackSpeedCustom != 0 && changeAttackSpeedCustom == message)
	{
		attackSpeed = param/100.0f;
		if (state == ATTACKING_TARGET)
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,attackSpeed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
	}
}
void JMG_Utility_AI_Goto_Target_Script::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 targetPos = Commands->Get_Position(obj);
		if (lastSeenTime)
		{
			lastSeenTime--;
			if (!lastSeenTime)
			{
				Return_Home(obj,ValidLastLocation(targetId));
				targetId = 0;
			}
		}
		if (targetId)
		{
			GameObject *target = Commands->Find_Object(targetId);
			targetPos = Commands->Get_Position(target);
			if (!target || !Commands->Get_Health(target) || (Commands->Get_Player_Type(obj) != -2 && Commands->Get_Player_Type(target) == Commands->Get_Player_Type(obj)) || Is_Script_Attached(target,"JMG_Utility_AI_Goto_Target_Script_Ignore_Object"))
			{
				lastSeenTime = 0;
				Return_Home(obj,ValidLastLocation(targetId));
				targetId = 0;
			}
		}
		if (state == HUNTING_STAR)
		{
			GameObject *star = Commands->Find_Object(huntingEnemyId);
			if (!star || !Commands->Get_Health(star) || (huntSearchDistance >= 0.0f && JmgUtility::SimpleDistance(Commands->Get_Position(star),Commands->Get_Position(obj)) > huntSearchDistance) || Is_Script_Attached(star,"JMG_Utility_AI_Goto_Target_Script_Ignore_Object"))
				Return_Home(obj,false);
		}
		if (state == IDLE || state == RETURNING_HOME || state == WANDERING_GROUP)
		{
			GameObject *star = findClosestStar(obj);
			if (star)
			{
				state = HUNTING_STAR;
				Attack_Move(obj,star,Vector3(),huntSpeed,huntArriveDistance,false,false);
			}
		}
		if (state == IDLE && wanderingAiGroupId != -1)
		{
			Vector3 wanderPos = Vector3();
			if (GetRandomPosition(&wanderPos))
			{
				state = WANDERING_GROUP;
				homeLocation = wanderPos;
				Attack_Move(obj,NULL,wanderPos,wanderSpeed,1.0f,false,false);
			}
		}
		if (state == RETURNING_HOME || state == WANDERING_GROUP)
			if (JmgUtility::SimpleDistance(lastAction.location,Commands->Get_Position(obj)) <= (wanderDistanceOverride != 0 ? wanderDistanceOverride : (obj->As_VehicleGameObj() ? 25.0f : 1.0f)))
				state = IDLE;
		if (state == ACTION_BADPATH)
			Cant_Get_To_target(obj);
		if (state != IDLE)
			Stuck_Check(obj,targetPos);
		Commands->Start_Timer(obj,this,0.1f,1);
	}
	if (number == 2)
	{
		if (removeIgnoreTime)
		{
			removeIgnoreTime--;
			if (!removeIgnoreTime)
				ignoreEnemyId = 0;
		}
		Commands->Start_Timer(obj,this,1.0f,2);
	}
	if (number == 3)
	{
		attackArriveDistance = Get_Float_Parameter("AttackDistance") >= 0 ? Get_Float_Parameter("AttackDistance") : weaponEffectiveRange;
		attackArriveDistance += Commands->Get_Random(Get_Float_Parameter("RandomAttackDistance"),Get_Float_Parameter("RandomAttackDistance"));
	}
	if (number == 4)
	{
		huntArriveDistance = Get_Float_Parameter("HuntArriveDistance");
		huntArriveDistance += Commands->Get_Random(Get_Float_Parameter("RandomHuntArriveDistance"),Get_Float_Parameter("RandomHuntArriveDistance"));
	}
}
void JMG_Utility_AI_Goto_Target_Script::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		if (state == HUNTING_STAR)
		{
			ignoreEnemyId = huntingEnemyId;
			removeIgnoreTime = 10;
			state = ACTION_BADPATH;
		}
		else if (state == WANDERING_GROUP)
			state = ACTION_BADPATH;
		else if (state == ATTACKING_TARGET)
			state = ACTION_BADPATH;
	}
}
void JMG_Utility_AI_Goto_Target_Script::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0.0f || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (!_stricmp(Get_Skin(damager),"Blamo") || !_stricmp(Get_Shield_Type(damager),"Blamo"))
		return;
	if (!targetId || !lastSeenTime || Choose_Target(obj,damager))
	{
		targetId = Commands->Get_ID(damager);
		lastSeenTime = Commands->Get_Random_Int(30,60);
		state = ATTACKING_TARGET;
		Attack_Move(obj,damager,Vector3(),attackSpeed,attackArriveDistance,true,false);
	}
}
void JMG_Utility_AI_Goto_Target_Script::Attack_Move(GameObject *obj,GameObject *target,Vector3 location,float speed,float distance,bool attack,bool overrideLocation)
{
	lastAction.location = location;
	Commands->Action_Reset(obj,100);
	lastAction = LastAction(targetId,location,speed,distance,attack,overrideLocation);
	ActionParamsStruct params;
	params.ObserverID = this->Get_ID();
	if (target && !overrideLocation)
	{
		params.Set_Movement(target,speed,distance,false);
		params.MoveFollow = true;
	}
	else
	{
		params.Set_Movement(location,speed,distance,false);
		params.MoveFollow = false;
	}
	params.MoveBackup = moveBackward;
	params.MovePathfind = true;
	if (Get_Int_Parameter("ShutdownEngineOnArrival"))
		params.ShutdownEngineOnArrival = true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,100,100,AI_STATE_COMBAT);
	params.AttackCheckBlocked = Get_Int_Parameter("AttackCheckBlocked") ? true : false;
	if (target && attack)
	{
		params.Set_Attack(target,weaponRange,0.0f,true);
		Commands->Action_Attack(obj,params);
	}
}
GameObject *JMG_Utility_AI_Goto_Target_Script::findClosestStar(GameObject *obj)
{
	if (Commands->Get_Player_Type(obj) == -2)
		return NULL;
	GameObject *nearest = NULL;
	float nearDist = -1.0f;
	Vector3 pos = Commands->Get_Position(obj);
	int myPlayerType = Commands->Get_Player_Type(obj);
	for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
	{
		SmartGameObj* o = current->Data();
		int oPlayerType = Commands->Get_Player_Type(o);
		int enemyId = Commands->Get_ID(o);
		if (!Is_Script_Attached(o,"JMG_Utility_AI_Goto_Target_Script_Target"))
			continue;
		if (!(o->As_SoldierGameObj() || o->As_VehicleGameObj()))
			continue;
		if (!Commands->Get_Health(o) || oPlayerType == -4 || oPlayerType == -2 || oPlayerType == myPlayerType || enemyId == ignoreEnemyId)
			continue;
		if (!_stricmp(Get_Skin(o),"Blamo") || !_stricmp(Get_Shield_Type(o),"Blamo"))
			continue;
		if (!huntStealth)
		{
			GameObject *vehicle = Get_Vehicle(o);
			if (vehicle && vehicle->As_SmartGameObj() && vehicle->As_SmartGameObj()->Is_Stealthed())
				continue;
			else if (o->As_SmartGameObj() && o->As_SmartGameObj()->Is_Stealthed())
				continue;
		}
		if (maxSightFromHomeLocation > 0 && JmgUtility::SimpleDistance(homeLocation,Commands->Get_Position(o)) > maxSightFromHomeLocation)
			continue;
		float tempDist = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o));
		if (huntSearchDistance >= 0.0f && tempDist > huntSearchDistance)
			continue;
		if (!nearest || (tempDist < nearDist))
		{
			nearest = o;
			nearDist = tempDist;
			huntingEnemyId = enemyId;
		}
	}
	return nearest;
}
void JMG_Utility_AI_Goto_Target_Script::Return_Home(GameObject *obj,ValidLastLocation goNearLastWanderPoint)
{
	Vector3 wanderPos;
	if (goNearLastWanderPoint.valid && Commands->Get_Random(0.0f,1.0f) < Get_Float_Parameter("ChanceToInvestigateLastSeenLocation"))
	{
		state = WANDERING_GROUP;
		Attack_Move(obj,NULL,goNearLastWanderPoint.location,wanderSpeed,1.0f,false,false);
	}
	else if (wanderingAiGroupId != -1 && GetRandomPosition(&wanderPos))
	{
		state = WANDERING_GROUP;
		Attack_Move(obj,NULL,wanderPos,wanderSpeed,1.0f,false,false);
	}
	else if (Get_Int_Parameter("ReturnHome"))
	{
		state = RETURNING_HOME;
		Attack_Move(obj,NULL,homeLocation,returnHomeSpeed,1.0f,false,false);
	}
	else
	{
		state = IDLE;
		Attack_Move(obj,NULL,Commands->Get_Position(obj),1.0f,1.0f,false,false);
	}
}
void JMG_Utility_AI_Goto_Target_Script::Stuck_Check(GameObject *obj,Vector3 targetPos)
{
	if (obj->As_VehicleGameObj())
	{
		Vector3 pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(pos,lastPosition) < 1.0f)
		{
			if (targetId && JmgUtility::SimpleDistance(pos,targetPos) <= attackArriveDistance*attackArriveDistance)
			{
				stuckTime = 0;
				return;
			}
			if (!reverseTime)
			{
				stuckTime++;
				if (stuckTime > 30)
				{
					moveBackward = true;
					Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
					reverseTime = Commands->Get_Random_Int(10,60);
					stuckTime = 0;
				}
			}
		}
		else
		{
			stuckTime = 0;
			lastPosition = pos;
		}
		if (reverseTime)
		{
			reverseTime--;
			if (!reverseTime)
			{
				moveBackward = false;
				Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
			}
		}
	}
	else
	{
		Vector3 pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(pos,lastPosition) < 1.0f)
		{
			if (targetId && JmgUtility::SimpleDistance(pos,targetPos) <= attackArriveDistance*attackArriveDistance)
			{
				stuckTime = 0;
				return;
			}
			stuckTime++;
			if (stuckTime > 30)
				Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
			if (stuckTime > 60)
				Attack_Move(obj,NULL,homeLocation,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
		}
		else
		{
			stuckTime = 0;
			lastPosition = pos;
		}
	}
}
void JMG_Utility_AI_Goto_Target_Script::Cant_Get_To_target(GameObject *obj)
{
	Vector3 wanderPos = Vector3();
	GameObject *star = findClosestStar(obj);
	if (star && Commands->Get_ID(star) != ignoreEnemyId)
	{
		state = HUNTING_STAR;
		Attack_Move(obj,Commands->Find_Object(lastAction.targetId),Commands->Get_Position(star),huntSpeed,huntArriveDistance,true,true);
	}
	else if (wanderingAiGroupId != -1 && GetRandomPosition(&wanderPos))
	{
		state = WANDERING_GROUP;
		Attack_Move(obj,NULL,wanderPos,wanderSpeed,1.0f,true,true);
	}
	else
	{
		state = RETURNING_HOME;
		Attack_Move(obj,NULL,homeLocation,returnHomeSpeed,1.0f,true,true);
	}
}
bool JMG_Utility_AI_Goto_Target_Script::Choose_Target(GameObject *obj,GameObject *target)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *currentTarget = Commands->Find_Object(targetId);
	if (!currentTarget)
		return true;
	if (JmgUtility::SimpleDistance(pos,Commands->Get_Position(target)) < JmgUtility::SimpleDistance(pos,Commands->Get_Position(currentTarget)))
		return true;
	return false;
}
JMG_Utility_AI_Goto_Target_Script::ValidLastLocation::ValidLastLocation(int enemyId)
{
	GameObject *target = Commands->Find_Object(enemyId);
	if (!target || !Commands->Get_Health(target))
	{
		this->valid = false;
		return;
	}
	this->location = Commands->Get_Position(target);
	this->valid = true;
}
void JMG_Utility_AI_Goto_Target_Script_Ignore_Object::Created(GameObject *obj)
{
}
void JMG_Utility_AI_Goto_Target_Script_Target::Created(GameObject *obj)
{
}
void JMG_Utility_Custom_Send_Custom_If_Script_Count::Created(GameObject *obj)
{
	recieveMessage = Get_Int_Parameter("Custom");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	randomDelay = Get_Float_Parameter("RandomDelay");
	randomChance = Get_Float_Parameter("RandomChance");
	sprintf(script,"%s",Get_Parameter("Script"));
	maxCount = Get_Int_Parameter("MaxCount");
	playerAddMaxCount = Get_Float_Parameter("PlayerAddMaxCount");
}
void JMG_Utility_Custom_Send_Custom_If_Script_Count::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		int count = 0;
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj *o = current->Data();
			if (!o || !o->As_ScriptableGameObj() || !Is_Script_Attached(o,script))
				continue;
			count++;
		}
		if (count >= maxCount+((int)playerAddMaxCount*Get_Player_Count()))
			return;
		if (randomChance && randomChance < Commands->Get_Random(0.0f,1.0f))
			return;
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
 		Commands->Send_Custom_Event(obj,object,custom,Param == -1 ? param : Param,delay+(randomDelay > 0 ? Commands->Get_Random(0.0f,randomDelay) : 0.0f));
	}
}
void JMG_Utility_Created_Trigger_Create_Vehicle::Created(GameObject *obj)
{
	int ownerId = Get_Int_Parameter("OwnerID");
	GameObject *owner = ownerId ? (ownerId == -1 ? obj : Commands->Find_Object(ownerId)) : NULL;
	Create_Vehicle(Get_Parameter("PresetName"),Get_Float_Parameter("Delay"),owner,Get_Int_Parameter("TeamID"));
}
void JMG_Utility_Custom_Remove_And_Attach_Script::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	const char *parameters = Get_Parameter("Params");
	params = newstr(parameters);
	char delim = Get_Parameter("Delim")[0];
	unsigned int length = strlen(parameters);
	for (unsigned int x = 0;x < length;x++)
		if (params[x] == delim)
			params[x] = ',';
	attachScript = newstr(Get_Parameter("AttachScript"));
	removeScript = newstr(Get_Parameter("RemoveScript"));
	repeat = Get_Int_Parameter("Repeat") ? true : false;
	requiresRemoveScript = Get_Int_Parameter("RequiresRemoveScript") ? true : false;
}
void JMG_Utility_Custom_Remove_And_Attach_Script::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		if (requiresRemoveScript && !Is_Script_Attached(obj,removeScript))
			return;
		Remove_Script(obj,removeScript);
		Commands->Attach_Script(obj,attachScript,params);
		if (!repeat)
		{
			Destroy_Script();
			return;
		}
	}
}
void JMG_Utility_AI_Guardian_Aircraft_Ignored::Created(GameObject *obj)
{
}
void JMG_Utility_AI_Guardian_Infantry_Ignored::Created(GameObject *obj)
{
}
void JMG_Utility_AI_Guardian_Vehicle_Ignored::Created(GameObject *obj)
{
}
void JMG_Utility_AI_Guardian_Generic_Ignored::Created(GameObject *obj)
{
}
void JMG_Utility_Objective_System_Objective_Marker::Created(GameObject *obj)
{
	objectiveId = Get_Int_Parameter("ObjectiveID");
}
GameObject *NewObjectiveSystem::GetObjectiveMarker(int objectiveMarkerId,GameObject *sender,int objectiveId)
{
	if (objectiveMarkerId == -2)
	{
		for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
		{
			GameObject* o = current->Data()->As_ScriptableGameObj();
			if (!o || !o->As_ScriptableGameObj())
				continue;
			JMG_Utility_Objective_System_Objective_Marker *script = (JMG_Utility_Objective_System_Objective_Marker*)Find_Script_On_Object(o,"JMG_Utility_Objective_System_Objective_Marker");
			if (!script)
				continue;
			if (script->objectiveId != objectiveId)
				continue;
			return o;		
		}
	}
	return objectiveMarkerId == -1 ? sender : Commands->Find_Object(objectiveMarkerId);
}
void JMG_Utility_Remove_Script_If_Doesnt_Have_Weapon::Created(GameObject *obj)
{
	sprintf(weaponName,"%s",Get_Parameter("WeaponName"));
	rate = Get_Float_Parameter("Rate");
	sprintf(script,"%s",Get_Parameter("Script"));
	const char *parameters = Get_Parameter("Params");
	params = newstr(parameters);
	char delim = Get_Parameter("Delim")[0];
	unsigned int length = strlen(parameters);
	for (unsigned int x = 0;x < length;x++)
		if (params[x] == delim)
			params[x] = ',';
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Remove_Script_If_Doesnt_Have_Weapon::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (Has_Weapon(obj,weaponName))
			Attach_Script_Once(obj,script,params);
		else if (!Is_Script_Attached(obj,script))
			Remove_Script(obj,script);
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Spawn_With_Last_Selected_Gun_Ignore::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_Spawn_With_Last_Selected_Gun_Ignore::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		JMG_Utility_Spawn_With_Last_Selected_Gun_Control::ignoredWeapons.Add_Tail(new JMG_Utility_Spawn_With_Last_Selected_Gun_Control::StringNode(Get_Parameter("WeaponPreset")));
	}
}
void JMG_Utility_Objective_System_Override_Visible_Settings::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_Objective_System_Override_Visible_Settings::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		NewObjectiveSystem::ObjectiveVisibleSettingOverride *overrideObject = new NewObjectiveSystem::ObjectiveVisibleSettingOverride(Get_Int_Parameter("ObjectiveID"),Get_Parameter("MarkerModel"),Get_Int_Parameter("MarkerColor"),Get_Parameter("AttachBone"),Get_Int_Parameter("OverrideTextColor") ? true : false,Get_Vector3_Parameter("TextColor"),Get_Int_Parameter("OverrideHudColor") ? true : false,Get_Vector3_Parameter("HudColor"));
		BasicObjectiveSystem.overrideVisibleObjectiveSettings.Add_Tail(overrideObject);
	}
}
void JMG_Utility_Custom_Create_Object_At_Bone::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	repeat = Get_Int_Parameter("Repeat") ? true : false;
}
void JMG_Utility_Custom_Create_Object_At_Bone::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		GameObject *object = Commands->Create_Object(Get_Parameter("Preset"),Commands->Get_Position(obj));
		Commands->Attach_To_Object_Bone(object,obj,Get_Parameter("Bone"));
		Vector3 targetPos = Commands->Get_Position(object);
		MoveablePhysClass *mphys = object->As_PhysicalGameObj() ? object->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
		Commands->Attach_To_Object_Bone(object,NULL,"");
		if (mphys && mphys->Find_Teleport_Location(targetPos,Get_Float_Parameter("MaxDistance"),&targetPos))
		{
			Commands->Set_Position(object,targetPos);
			Force_Position_Update(object);
		}
		if (!repeat)
			Destroy_Script();
	}
}
void JMG_Utility_Killed_Send_Custom_When_Killed_By_Nothing::Killed(GameObject *obj,GameObject *killer)
{
	if (killer != NULL)
		return;
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? Commands->Find_Object(id) : obj;
	Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	Destroy_Script();
}
void JMG_Utility_Custom_Drop_Corpse::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	repeat = Get_Int_Parameter("Repeat") ? true : false;
	sprintf(powerupName,"%s",Get_Parameter("PowerupName"));
}
void JMG_Utility_Custom_Drop_Corpse::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Start_Timer(obj,this,0.0f,1);
	}
}
void JMG_Utility_Custom_Drop_Corpse::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *powerup = Commands->Create_Object(powerupName,pos);
		Commands->Set_Facing(powerup,Commands->Get_Facing(obj));
		Commands->Set_Model(powerup,Get_Model(obj));
		if (!obj->As_SmartGameObj())
			return;
		AnimControlClass *control = obj->As_SmartGameObj()->Get_Anim_Control();
		if (!control || control->Get_Animation_Name() == NULL || !_stricmp(control->Get_Animation_Name(),""))
			return;
		Commands->Set_Animation(powerup,control->Get_Animation_Name(),false,0,control->Get_Current_Frame(),-1,true);
		Commands->Destroy_Object(obj);
		if (!repeat)
			Destroy_Script();
	}
}
void JMG_Utility_Custom_Send_Shuffled_Customs::Created(GameObject *obj)
{
	recieveMessage = Get_Int_Parameter("Custom");
	id = Get_Int_Parameter("ID");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	char sendCustomsStr[256];
	sprintf(sendCustomsStr,"%s",Get_Parameter("SendCustoms"));
	char delim[2];
	sprintf(delim,"%s",Get_Parameter("Delim"));
	char *token = strtok(sendCustomsStr, delim);
	while(token != NULL)
	{
		customs.Add_Tail(new int(atoi(token)));
		token = strtok(NULL,delim);
	}
}
void JMG_Utility_Custom_Send_Shuffled_Customs::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		if (customs.Get_Count() <= 0)
		{
			Console_Input("msg JMG_Utility_Custom_Send_Shuffled_Customs ERROR: All customs have already been sent!");
			Destroy_Script();
			return;
		}
		int random = customs.Get_Count() > 0 ? Commands->Get_Random_Int(0,customs.Get_Count()) : customs.Get_Count();
		int randomCustom = -1;
		for (SLNode<int> *current = customs.Head();current;current = current->Next())
		{
			if (!random)
			{
				randomCustom = *current->Data();
				customs.Remove(current->Data());
				break;
			}
			random--;
		}
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
 		Commands->Send_Custom_Event(obj,object,randomCustom,Param == -1 ? param : Param,delay);
	}
}
void JMG_Utility_Custom_Send_Shuffled_Customs_And_Ids::Created(GameObject *obj)
{
	recieveMessage = Get_Int_Parameter("Custom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	char sendCustomsStr[256];
	sprintf(sendCustomsStr,"%s",Get_Parameter("SendCustoms"));
	char delim[2];
	sprintf(delim,"%s",Get_Parameter("Delim"));
	char *token = strtok(sendCustomsStr, delim);
	while(token != NULL)
	{
		customs.Add_Tail(new int(atoi(token)));
		token = strtok(NULL,delim);
	}
	char sendIdsStr[256];
	sprintf(sendIdsStr,"%s",Get_Parameter("IDs"));
	token = strtok(sendIdsStr, delim);
	while(token != NULL)
	{
		ids.Add_Tail(new int(atoi(token)));
		token = strtok(NULL,delim);
	}
	if (customs.Get_Count() != ids.Get_Count())
	{
		Console_Input("msg JMG_Utility_Custom_Send_Shuffled_Customs_And_Ids ERROR: IDs and Customs must have the same count!");
		Destroy_Script();
		return;
	}
}
void JMG_Utility_Custom_Send_Shuffled_Customs_And_Ids::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		if (customs.Get_Count() <= 0)
		{
			Console_Input("msg JMG_Utility_Custom_Send_Shuffled_Customs_And_Ids ERROR: All customs/IDs have already been sent!");
			Destroy_Script();
			return;
		}
		int random = customs.Get_Count() > 0 ? Commands->Get_Random_Int(0,customs.Get_Count()) : customs.Get_Count();
		int randomCustom = -1;
		for (SLNode<int> *current = customs.Head();current;current = current->Next())
		{
			if (!random)
			{
				randomCustom = *current->Data();
				customs.Remove(current->Data());
				break;
			}
			random--;
		}
		random = ids.Get_Count() > 0 ? Commands->Get_Random_Int(0,ids.Get_Count()) : ids.Get_Count();
		int randomId = -1;
		for (SLNode<int> *current = ids.Head();current;current = current->Next())
		{
			if (!random)
			{
				randomId = *current->Data();
				ids.Remove(current->Data());
				break;
			}
			random--;
		}
 		Commands->Send_Custom_Event(obj,Commands->Find_Object(randomId),randomCustom,Param == -1 ? param : Param,delay);
	}
}
void JMG_Utility_Custom_Force_All_Players_Into_Vehicle::Created(GameObject *obj)
{
	if (!obj->As_VehicleGameObj())
	{
		Console_Input("msg JMG_Utility_Custom_Force_All_Players_Into_Vehicle ERROR: Only works when attached to vehicles!");
		Destroy_Script();
		return;
	}
	custom = Get_Int_Parameter("Custom");
	forceOutOfOtherVehicles = Get_Int_Parameter("ForceOutOfOtherVehicles") ? true : false;
}
void JMG_Utility_Custom_Force_All_Players_Into_Vehicle::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		if (param != this->Get_ID())
		{
			for (int x = 1;x < 128;x++)
			{
				GameObject *player = Get_GameObj(x);
				if (!player || !Commands->Get_Health(player))
					continue;
				SuccessState successState = ForceIntoVehicle(obj,player);
				if (successState == SuccessState::VEHICLE_FULL || successState == SuccessState::FAILED)
					return;
			}
		}
		else
		{
			ForceIntoVehicle(obj,sender);
		}
	}
}
JMG_Utility_Custom_Force_All_Players_Into_Vehicle::SuccessState JMG_Utility_Custom_Force_All_Players_Into_Vehicle::ForceIntoVehicle(GameObject *obj,GameObject *player)
{
	if (obj == Get_Vehicle(player))
		return SuccessState::ALREADY_IN;
	if (Get_Vehicle_Seat_Count(obj) <= Get_Vehicle_Occupant_Count(obj))
		return SuccessState::VEHICLE_FULL;
	if (Get_Vehicle(player))
	{
		if (!forceOutOfOtherVehicles)
			return SuccessState::STUCK_IN_ANOTHER_VEHICLE;
		Soldier_Transition_Vehicle(player);
		Commands->Send_Custom_Event(player,obj,custom,this->Get_ID(),0.1f);
		return SuccessState::RETRY;
	}
	Force_Vehicle_Entry(player,obj);
	if (obj == Get_Vehicle(player))
		return SuccessState::SUCCESS;
	return SuccessState::FAILED;
}
void JMG_Utility_Custom_Force_Occupants_Out_And_Lock::Created(GameObject *obj)
{
	if (!obj->As_VehicleGameObj())
	{
		Console_Input("msg JMG_Utility_Custom_Force_Occupants_Out_And_Lock ERROR: Only works when attached to vehicles!");
		Destroy_Script();
		return;
	}
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Force_Occupants_Out_And_Lock::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Timer_Expired(obj,1);
	}
}
void JMG_Utility_Custom_Force_Occupants_Out_And_Lock::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Force_Occupants_Exit(obj);
		if (!Get_Vehicle_Occupant_Count(obj))
		{
			Commands->Enable_Vehicle_Transitions(obj,false);
			Destroy_Script();
			return;
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Custom_Send_Custom_If_Occupant_LorE::Created(GameObject *obj)
{
	if (!obj->As_VehicleGameObj())
	{
		Console_Input("msg JMG_Utility_Custom_Send_Custom_If_Vehicle_Occupant_Count_Less_Than ERROR: Only works when attached to vehicles!");
		Destroy_Script();
		return;
	}
	recieveMessage = Get_Int_Parameter("Custom");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	occupants = Get_Int_Parameter("OccupantCount");
	repeat = Get_Int_Parameter("Repeat") ? true : false;
}
void JMG_Utility_Custom_Send_Custom_If_Occupant_LorE::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		if (Get_Vehicle_Occupant_Count(obj) > occupants)
			return;
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
 		Commands->Send_Custom_Event(obj,object,custom,Param == -1 ? param : Param,delay);
		if (!repeat)
			Destroy_Script();
	}
}
void JMG_Utility_Custom_Send_Custom_If_Occupant_GorE::Created(GameObject *obj)
{
	if (!obj->As_VehicleGameObj())
	{
		Console_Input("msg JMG_Utility_Custom_Send_Custom_If_Occupant_GorE ERROR: Only works when attached to vehicles!");
		Destroy_Script();
		return;
	}
	recieveMessage = Get_Int_Parameter("Custom");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	occupants = Get_Int_Parameter("OccupantCount");
	repeat = Get_Int_Parameter("Repeat") ? true : false;
}
void JMG_Utility_Custom_Send_Custom_If_Occupant_GorE::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		if (Get_Vehicle_Occupant_Count(obj) < occupants)
			return;
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
 		Commands->Send_Custom_Event(obj,object,custom,Param == -1 ? param : Param,delay);
		if (!repeat)
			Destroy_Script();
	}
}
SList<JMG_Utility_Basic_Spawner_Wander_Point::SpawnObjectNode> JMG_Utility_Basic_Spawner_Wander_Point::spawnObjectNodeList;
void JMG_Utility_Basic_Spawner_Wander_Point::Created(GameObject *obj)
{
	attachScriptsGroupId = Get_Int_Parameter("Attach_Scripts_Group_ID");
	spawnedObjectScriptID = Get_Int_Parameter("Spawned_Object_Script_ID");
	if (spawnedObjectScriptID == -1)
		spawnedObjectScriptID = this->Get_ID();
	spawnedObjects = 0;
	spawnAtATime = Get_Int_Parameter("Spawn_At_A_Time");
	maxTotalSpawned = Get_Int_Parameter("Max_Total_Spawned");
	rate = Get_Float_Parameter("Spawn_Rate");
	randomRate = Get_Float_Parameter("Random_Spawn_Rate");
	changeSpawnCapCustom = Get_Int_Parameter("Change_Spawn_Cap_Custom");
	sprintf(preset,"%s",Get_Parameter("Spawn_Preset"));
	spawnCount = 0;
	initialSpawn = Get_Int_Parameter("Initial_Spawn");
	if (initialSpawn == -1)
		initialSpawn = spawnAtATime;
	spawnGroupId = Get_Int_Parameter("Spawn_Group_ID");
	enabled = Get_Int_Parameter("Starts_Disabled") == 0 ? true : false;
	enableDisableCustom = Get_Int_Parameter("Enable_Disable_Custom");
	playersAddToSpawnAtATime = Get_Float_Parameter("PlayersAddToSpawnAtATime");
	wanderPointGroup = Get_Int_Parameter("WanderingPointGroupID");
	safeTeleportDistance = Get_Float_Parameter("SafeTeleportDistance");
	if (The_Game()->Get_Game_Duration_S() < 1.5f)
		Commands->Start_Timer(obj,this,1.5f,2);
	else
		Timer_Expired(obj,2);
	lastPlayerCount = 0;
	Commands->Start_Timer(obj,this,2.5f,3);
}
void JMG_Utility_Basic_Spawner_Wander_Point::Timer_Expired(GameObject *obj,int number)
{
	if (number == 2 && !The_Game()->Is_Game_Over())
	{
		if (enabled)
			Initial_Spawn(obj);
		Commands->Start_Timer(obj,this,rate+(randomRate ? Commands->Get_Random(-randomRate,randomRate) : 0.0f),1);
	}
	if (number == 1 && !The_Game()->Is_Game_Over())
	{
		if (enabled)
		{
			JMG_Utility_Basic_Spawner_Wander_Point::SpawnFailureTypes spawnResult = AttemptSpawn(obj);
			if (spawnResult == SpawnFailureTypes::SUCCESS)
				Commands->Start_Timer(obj,this,rate+(randomRate ? Commands->Get_Random(-randomRate,randomRate) : 0.0f),1);
			else if (spawnResult == SpawnFailureTypes::SPAWN_BLOCKED)
				Commands->Start_Timer(obj,this,0.1f,1);
		}
		else
			Commands->Start_Timer(obj,this,0.1f,1);
	}
	if (number == 3 && !The_Game()->Is_Game_Over())
	{
		int change = GetPlayerLimitModifier()-lastPlayerCount;
		lastPlayerCount = change;
		for (int x = 0;x < change;x++)
			Commands->Send_Custom_Event(obj,obj,6873525,spawnedObjectScriptID,0.1f);
		Commands->Start_Timer(obj,this,2.5f,3);
	}
}
void JMG_Utility_Basic_Spawner_Wander_Point::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (changeSpawnCapCustom && message == changeSpawnCapCustom)
	{
		int change = param-spawnAtATime;
		spawnAtATime = param;
		for (int x = 0;x < change;x++)
			Commands->Send_Custom_Event(obj,obj,6873525,spawnedObjectScriptID,0.1f);
	}
	if (message == 6873523 && param == spawnedObjectScriptID)
	{
		spawnedObjects++;
	}
	if (message == 6873524 && param == spawnedObjectScriptID)
	{
		if (spawnedObjects > 0)
			spawnedObjects--;
		Commands->Send_Custom_Event(obj,obj,6873525,spawnedObjectScriptID,rate+(randomRate ? Commands->Get_Random(-randomRate,randomRate) : 0.0f));
	}
	if (message == 6873525 && param == spawnedObjectScriptID && !The_Game()->Is_Game_Over())
	{
		if (!enabled)
			return;
		if (AttemptSpawn(obj) == SpawnFailureTypes::SPAWN_BLOCKED)
			Commands->Send_Custom_Event(obj,obj,6873525,spawnedObjectScriptID,0.1f);
	}
	if (enableDisableCustom && message == enableDisableCustom && !The_Game()->Is_Game_Over())
	{
		enabled = param != 0 ? true : false;
		if (param == 1)
			Initial_Spawn(obj);
	}
}
JMG_Utility_Basic_Spawner_Wander_Point::SpawnFailureTypes JMG_Utility_Basic_Spawner_Wander_Point::AttemptSpawn(GameObject *obj)
{
	if (!(spawnedObjects < spawnAtATime+GetPlayerLimitModifier()))
		return SpawnFailureTypes::LIMIT_REACHED;
	if (!((maxTotalSpawned >= 0 && spawnCount < maxTotalSpawned) || maxTotalSpawned < 0))
		return SpawnFailureTypes::LIMIT_REACHED;

	GameObject *spawned = Commands->Create_Object(preset,Commands->Get_Position(obj));
	if (Grab_Teleport_Spot(spawned,5))
	{
		char params[512];
		sprintf(params,"%d,%d,%d",Commands->Get_ID(obj),spawnedObjectScriptID,spawnGroupId);
		Commands->Attach_Script(spawned,"JMG_Utility_Basic_Spawner_In_Radius_Attached",params);
		Commands->Send_Custom_Event(spawned,obj,6873522,attachScriptsGroupId,0.0f);
		return SpawnFailureTypes::SUCCESS;
	}
	Commands->Destroy_Object(spawned);
	return SpawnFailureTypes::SPAWN_BLOCKED;
}
void JMG_Utility_Basic_Spawner_Wander_Point::Initial_Spawn(GameObject *obj)
{
	int addSpawn = GetPlayerLimitModifier();
	lastPlayerCount = addSpawn;
	for (int x = spawnedObjects;x < initialSpawn+addSpawn;x++)
	{
		JMG_Utility_Basic_Spawner_Wander_Point::SpawnFailureTypes spawnResult;
		for (int y = 0;y < 5;y++)
		{
			 spawnResult = AttemptSpawn(obj);
			if (spawnResult == SpawnFailureTypes::SUCCESS)
				break;
			if (spawnResult == SpawnFailureTypes::LIMIT_REACHED)
				x = initialSpawn+1+addSpawn;
		}
		if (spawnResult == SpawnFailureTypes::SPAWN_BLOCKED)
			Commands->Send_Custom_Event(obj,obj,6873525,spawnedObjectScriptID,0.1f);
		if (initialSpawn < spawnAtATime)
		{
			Stop_Timer(obj,1);
			Commands->Start_Timer(obj,this,0.1f,1);
		}
	}
}
int JMG_Utility_Basic_Spawner_Wander_Point::GetPlayerLimitModifier()
{
	return (int)playersAddToSpawnAtATime*Get_Player_Count();
}
bool JMG_Utility_Basic_Spawner_Wander_Point::Grab_Teleport_Spot(GameObject *spawnedObject,int attempts)
{
	if (!attempts)
		return false;
	attempts--;
	Vector3 targetPos;
	float facing;
	if (!Get_A_Defense_Point(&targetPos,&facing))
	{
		char errormsg[220];
		sprintf(errormsg,"msg JMG_Utility_Basic_Spawner_Wander_Point ERROR: No wander points found for group %d!",wanderPointGroup);
		Console_Input(errormsg);
		return false;
	}
	MoveablePhysClass *mphys = spawnedObject->As_PhysicalGameObj() ? spawnedObject->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	if (mphys && mphys->Find_Teleport_Location(targetPos,safeTeleportDistance,&targetPos))
	{
		spawnCount++;
		Toggle_Fly_Mode(spawnedObject);
		Commands->Set_Position(spawnedObject,targetPos);
		Force_Position_Update(spawnedObject);
		Toggle_Fly_Mode(spawnedObject);
		char params[220];
		sprintf(params,"0.1,%.2f",facing);
		Commands->Attach_Script(spawnedObject,"JMG_Utility_Delay_Then_Rotate_Camera",params);
		return true;
	}
	else
	{
		Commands->Set_Position(spawnedObject,targetPos);
		return Grab_Teleport_Spot(spawnedObject,attempts);
	}
}
int JMG_Utility_Global_CSC_With_Global_Param_Controller::globalParam = -1;
void JMG_Utility_Global_CSC_With_Global_Param_Controller::Created(GameObject *obj)
{
	globalParam = Get_Int_Parameter("GlobalParam");
}
void JMG_Utility_Global_CSC_With_Global_Param_Update_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	globalParam = Get_Int_Parameter("GlobalParam");
}
void JMG_Utility_Global_CSC_With_Global_Param_Update_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		JMG_Utility_Global_CSC_With_Global_Param_Controller::globalParam = globalParam;
	}
}
void JMG_Utility_Global_CSC_With_Global_Param::Created(GameObject *obj)
{
	recieveMessage = Get_Int_Parameter("Custom");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("SendCustom");
	delay = Get_Float_Parameter("Delay");
	randomDelay = Get_Float_Parameter("RandomDelay");
	randomChance = Get_Float_Parameter("RandomChance");
}
void JMG_Utility_Global_CSC_With_Global_Param::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		if (randomChance && randomChance < Commands->Get_Random(0.0f,1.0f))
			return;
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
		Commands->Send_Custom_Event(obj,object,custom,JMG_Utility_Global_CSC_With_Global_Param_Controller::globalParam,delay+(randomDelay > 0 ? Commands->Get_Random(0.0f,randomDelay) : 0.0f));
	}
}
float JMG_Utility_Global_Armor_Scaled_Controller::scale = 100.0f;
void JMG_Utility_Global_Armor_Scaled_Controller::Created(GameObject *obj)
{
	scale = Get_Float_Parameter("Scale")*0.01f;
}
void JMG_Utility_Global_Armor_Scaled_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	scale = Get_Float_Parameter("Scale");
	if (scale > 0)
		scale *= 0.01f;
}
void JMG_Utility_Global_Armor_Scaled_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		JMG_Utility_Global_Armor_Scaled_Controller::scale = scale == -1.0f ? param*0.01f : scale;
	}
}
void JMG_Utility_Global_Armor_Scaled_Object::Created(GameObject *obj)
{
	Set_Max_Shield_Strength(obj,JMG_Utility_Global_Armor_Scaled_Controller::scale*Commands->Get_Max_Shield_Strength(obj));
	Commands->Set_Shield_Strength(obj,JMG_Utility_Global_Armor_Scaled_Controller::scale*Commands->Get_Shield_Strength(obj));
}
float JMG_Utility_Global_Health_Scaled_Controller::scale = 100.0f;
void JMG_Utility_Global_Health_Scaled_Controller::Created(GameObject *obj)
{
	scale = Get_Float_Parameter("Scale")*0.01f;
}
void JMG_Utility_Global_Health_Scaled_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	scale = Get_Float_Parameter("Scale");
	if (scale > 0)
		scale *= 0.01f;
}
void JMG_Utility_Global_Health_Scaled_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		JMG_Utility_Global_Health_Scaled_Controller::scale = scale == -1.0f ? param*0.01f : scale;
	}
}
void JMG_Utility_Global_Health_Scaled_Object::Created(GameObject *obj)
{
	Set_Max_Health(obj,JMG_Utility_Global_Health_Scaled_Controller::scale*Commands->Get_Max_Health(obj));
	Commands->Set_Health(obj,JMG_Utility_Global_Health_Scaled_Controller::scale*Commands->Get_Health(obj));
}
float JMG_Utility_Global_Infantry_Speed_Controller::speed = 6.0f;
void JMG_Utility_Global_Infantry_Speed_Controller::Created(GameObject *obj)
{
	speed = Get_Float_Parameter("Speed");
}
void JMG_Utility_Global_Infantry_Speed_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	speed = Get_Float_Parameter("Scale");
}
void JMG_Utility_Global_Infantry_Speed_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		JMG_Utility_Global_Infantry_Speed_Controller::speed = speed == -1.0f ? param*0.01f : speed;
	}
}
void JMG_Utility_Global_Infantry_Speed_Object::Created(GameObject *obj)
{
	if (!obj->As_SoldierGameObj())
	{
		Console_Input("msg JMG_Utility_Global_Infantry_Speed_Object ERROR: This script can only be attached to infantry");
		Destroy_Script();
		return;
	}
	obj->As_SoldierGameObj()->Set_Max_Speed(JMG_Utility_Global_Infantry_Speed_Controller::speed);
}
void JMG_Utility_AI_Control_Point_Ignore_Object::Created(GameObject *obj)
{
}
void JMG_Utility_Custom_Send_Custom_No_Presets_Outside_Of_Vehicle::Created(GameObject *obj)
{
	if (!obj->As_VehicleGameObj())
	{
		Console_Input("msg JMG_Utility_Custom_Send_Custom_No_Presets_Outside_Of_Vehicle ERROR: Only works when attached to vehicles!");
		Destroy_Script();
		return;
	}
	recieveMessage = Get_Int_Parameter("Custom");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	occupants = Get_Int_Parameter("OccupantCount");
	sprintf(preset,"%s",Get_Parameter("Preset"));
	repeat = Get_Int_Parameter("Repeat") ? true : false;
}
void JMG_Utility_Custom_Send_Custom_No_Presets_Outside_Of_Vehicle::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj* o = current->Data();
			if (o != obj && !_stricmp(Commands->Get_Preset_Name(o),preset) && Get_Vehicle(o) != obj)
				return;
		}
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
 		Commands->Send_Custom_Event(obj,object,custom,Param == -1 ? param : Param,delay);
		if (!repeat)
			Destroy_Script();
	}
}
void JMG_Utility_Custom_Send_Custom_No_Presets_Inside_Of_Vehicle::Created(GameObject *obj)
{
	if (!obj->As_VehicleGameObj())
	{
		Console_Input("msg JMG_Utility_Custom_Send_Custom_No_Presets_Inside_Of_Vehicle ERROR: Only works when attached to vehicles!");
		Destroy_Script();
		return;
	}
	recieveMessage = Get_Int_Parameter("Custom");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	occupants = Get_Int_Parameter("OccupantCount");
	sprintf(preset,"%s",Get_Parameter("Preset"));
	repeat = Get_Int_Parameter("Repeat") ? true : false;
}
void JMG_Utility_Custom_Send_Custom_No_Presets_Inside_Of_Vehicle::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		for (int x = 0;x < Get_Vehicle_Seat_Count(obj);x++)
		{
			GameObject *occupant = Get_Vehicle_Occupant(obj,x);
			if (occupant && !_stricmp(Commands->Get_Preset_Name(occupant),preset))
				return;
		}
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
 		Commands->Send_Custom_Event(obj,object,custom,Param == -1 ? param : Param,delay);
		if (!repeat)
			Destroy_Script();
	}
}
void JMG_Utility_Custom_Send_Custom_From_All_Players::Created(GameObject *obj)
{
	recieveMessage = Get_Int_Parameter("Custom");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("SendCustom");
	Param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
}
void JMG_Utility_Custom_Send_Custom_From_All_Players::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player)
				continue;
			GameObject *object = id ? (id == -2 ? player : (id == -1 ? sender : Commands->Find_Object(id))) : obj;
 			Commands->Send_Custom_Event(player,object,custom,Param == -1 ? param : Param,delay);
		}
	}
}
void JMG_Utility_Killed_By_Player_Send_Custom_From_Player::Killed(GameObject *obj,GameObject *killer)
{
	if (!Commands->Is_A_Star(killer))
		return;
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? (id == -1 ? killer : Commands->Find_Object(id)) : obj;
	Commands->Send_Custom_Event(killer,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	Destroy_Script();
}
float JMG_Utility_Sync_Fog_Controller::min = 0.0f;
float JMG_Utility_Sync_Fog_Controller::max = 300.0f;
void JMG_Utility_Sync_Fog_Controller::Created(GameObject *obj)
{
	min = Get_Float_Parameter("MinRange");
	max = Get_Float_Parameter("MaxRange");
	for (int x = 1;x < 128;x++)
		fogSynced[x] = false;
	Commands->Start_Timer(obj,this,0.25,23453451);
}
void JMG_Utility_Sync_Fog_Controller::Timer_Expired(GameObject *obj,int number)
{
	if (number == 23453451) 
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (player && !fogSynced[x])
			{
				fogSynced[x] = true;
				Set_Fog_Enable_Player(player,true);
				Set_Fog_Range_Player(player,min,max,0);
			}
			if (!player && fogSynced[x])
				fogSynced[x] = false;
		}
		Commands->Start_Timer(obj,this,0.25,23453451);
	}
}
void JMG_Utility_Sync_Fog_Custom_Update::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	min = Get_Float_Parameter("MinRange");
	max = Get_Float_Parameter("MaxRange");
	transition = Get_Float_Parameter("Transition");
}
void JMG_Utility_Sync_Fog_Custom_Update::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Set_Fog_Range(min,max,transition);
		JMG_Utility_Sync_Fog_Controller::min = min;
		JMG_Utility_Sync_Fog_Controller::max = max;
	}
}
void JMG_Utility_Sync_Animation_On_Join::Created(GameObject *obj)
{
	for (int x = 1;x < 128;x++)
		synced[x] = false;
	Commands->Start_Timer(obj,this,0.25,23453451);
}
void JMG_Utility_Sync_Animation_On_Join::Timer_Expired(GameObject *obj,int number)
{
	if (number == 23453451) 
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (player && !synced[x])
			{
				synced[x] = true;
				const char *anim = Get_Animation_Name(obj);
				if (anim != NULL)
					Set_Subobject_Animation_Player(player,obj,anim,false,NULL,Get_Animation_Frame(obj),Get_Animation_Target_Frame(obj),true);
			}
			if (!player && synced[x])
				synced[x] = false;
		}
		Commands->Start_Timer(obj,this,0.25,23453451);
	}
}
void JMG_Utility_Set_Animation_Frame_To_Match_Ammo::Created(GameObject *obj)
{
	sprintf(animation,"%s",Get_Parameter("Animation"));
	lastFrame = Get_Current_Bullets(obj);
	Commands->Set_Animation(obj,animation,false,NULL,(float)lastFrame,(float)lastFrame,false);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Set_Animation_Frame_To_Match_Ammo::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		int currentFrame = Get_Current_Bullets(obj);
		if (lastFrame != currentFrame)
		{
			lastFrame = currentFrame;
			Commands->Set_Animation(obj,animation,false,NULL,(float)lastFrame,(float)lastFrame,false);
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Poke_Grant_Weapon::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void JMG_Utility_Poke_Grant_Weapon::Poked(GameObject *obj, GameObject *poker)
{
	if (Commands->Get_Player_Type(poker) == -4)
		return;
	Grant_Weapon(poker,Get_Parameter("Weapon"),true,Get_Int_Parameter("Rounds"),Get_Int_Parameter("Backpack") ? true : false);
}
void JMG_Utility_Zone_Create_Object_While_Occupied::Created(GameObject *obj)
{
	id = 0;
	objectsInZone = 0;
	sprintf(preset,"%s",Get_Parameter("Preset"));
	location = Get_Vector3_Parameter("Location");
	facing = Get_Float_Parameter("Facing");
	enableCustom = Get_Int_Parameter("EnableCustom");
	reCreateOnDeath = Get_Int_Parameter("ReCreateOnDeath") ? true : false;
	if (enableCustom)
		enabled = false;
	else
		enabled = true;
}
void JMG_Utility_Zone_Create_Object_While_Occupied::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (enableCustom && message == enableCustom)
	{
		enabled = param ? true : false;
	}
	if (message == Get_ID() && param == Get_ID())
	{// Object occupying zone died
		Exited(obj,sender);
	}
	if (message == Get_ID() && param == Get_ID()+1)
	{
		id = 0;
		if (!reCreateOnDeath)
		{
			Destroy_Script();
			return;
		}
		if (!objectsInZone)
			return;
		TriggerCreate(obj);
	}
}
void JMG_Utility_Zone_Create_Object_While_Occupied::Entered(GameObject *obj,GameObject *enterer)
{
	objectsInZone++;
	char params[256];
	sprintf(params,"%d,%d",Commands->Get_ID(obj),Get_ID());
	Commands->Attach_Script(enterer,"JMG_Utility_Zone_Create_Object_While_Occupied_Attached",params);
	TriggerCreate(obj);
}
void JMG_Utility_Zone_Create_Object_While_Occupied::TriggerCreate(GameObject *obj)
{
	if (id || !enabled)
		return;
	char params[256];
	GameObject *object = Commands->Create_Object(preset,location);
	Commands->Set_Facing(object,facing);
	id = Commands->Get_ID(object);
	sprintf(params,"%d,%d",Commands->Get_ID(obj),Get_ID());
	Commands->Attach_Script(object,"JMG_Utility_Zone_Create_Object_While_Occupied_Object_Attached",params);
}
void JMG_Utility_Zone_Create_Object_While_Occupied::Exited(GameObject *obj,GameObject *exiter)
{
	objectsInZone--;
	Remove_Script(exiter,"JMG_Utility_Zone_Create_Object_While_Occupied_Attached");
	if (!id || !enabled || objectsInZone)
		return;
	Commands->Send_Custom_Event(obj,Commands->Find_Object(id),Get_ID(),Get_ID(),0);
	id = 0;
}
void JMG_Utility_Zone_Create_Object_While_Occupied_Attached::Killed(GameObject *obj,GameObject *killer)
{
	Destroyed(obj);
}
void JMG_Utility_Zone_Create_Object_While_Occupied_Attached::Destroyed(GameObject *obj)
{
	if (Exe == 4)
		return;
	int scriptId = Get_Int_Parameter("ScriptId");
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ScriptZoneId")),scriptId,scriptId,0);
	Destroy_Script();
}
void JMG_Utility_Zone_Create_Object_While_Occupied_Object_Attached::Created(GameObject *obj)
{
	deathByScript = false;
	ownerScriptId = Get_Int_Parameter("ScriptId");
}
void JMG_Utility_Zone_Create_Object_While_Occupied_Object_Attached::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == ownerScriptId && param == ownerScriptId)
	{
		deathByScript = true;
		Commands->Destroy_Object(obj);
	}
}
void JMG_Utility_Zone_Create_Object_While_Occupied_Object_Attached::Destroyed(GameObject *obj)
{
	if (Exe == 4 || deathByScript)
		return;
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ScriptZoneId")),ownerScriptId,ownerScriptId+1,0);
	Destroy_Script();
}
void JMG_Utility_Animate_While_Moving_Idle_Or_Dead::Created(GameObject *obj)
{
	if (!obj->As_SmartGameObj())
	{
		Console_Input("msg JMG_Utility_Animate_While_Moving_Idle_Or_Dead ERROR: Only attach to smart game objects!");
		Commands->Destroy_Object(obj);
		return;
	}
	sprintf(subObject,"%s",Get_Parameter("SubObject"));
	sprintf(idle,"%s",Get_Parameter("IdleAnimation"));
	sprintf(death,"%s",Get_Parameter("DeathAnimation"));
	sprintf(move,"%s",Get_Parameter("MoveAnimation"));
	deathFrame = Get_Float_Parameter("DeathFrame");
	PlayAnimation(obj,idle,-1);
	moving = false;
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Animate_While_Moving_Idle_Or_Dead::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && Commands->Get_Health(obj))
	{
		Vector3 speed;
		obj->As_SmartGameObj()->Get_Velocity(speed);
		if (speed.Length2() > 0.0f)
		{
			if (!moving)
			{
				moving = true;
				PlayAnimation(obj,move,-1);
			}
		}
		else if (moving)
		{
			moving = false;
			PlayAnimation(obj,idle,-1);
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Animate_While_Moving_Idle_Or_Dead::Killed(GameObject *obj,GameObject *killer)
{
	PlayAnimation(obj,death,deathFrame);
}
void JMG_Utility_Animate_While_Moving_Idle_Or_Dead::PlayAnimation(GameObject *obj,const char *aniamtionName,float frame)
{
	if (_stricmp(subObject,""))
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player)
				continue;
			Set_Subobject_Animation_Player(player,obj,aniamtionName,frame < 0 ? true : false,subObject,0,frame,true);
		}
	}
	else
		Commands->Set_Animation(obj,aniamtionName,frame < 0 ? true : false,NULL,0,frame,true);
}
void JMG_Utility_Send_Custom_When_HP_Crosses_Threshold::Created(GameObject *obj)
{
	id = Get_Int_Parameter("ID");
	targetRatio = Get_Float_Parameter("TargetRatio");
	aboveCustom = Get_Int_Parameter("AboveCustom");
	aboveParam = Get_Int_Parameter("AboveParam");
	belowCustom = Get_Int_Parameter("BelowCustom");
	belowParam = Get_Int_Parameter("BelowParam");
	below = Get_Hitpoints(obj) < Get_Max_Hitpoints(obj)*targetRatio;
}
void JMG_Utility_Send_Custom_When_HP_Crosses_Threshold::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damager)
		damager = obj;
	if (Get_Hitpoints(obj) < Get_Max_Hitpoints(obj)*targetRatio)
	{
		if (!below)
		{
			below = true;
			GameObject *object = id ? (id == -1 ? damager : Commands->Find_Object(id)) : obj;
			Commands->Send_Custom_Event(damager,object,belowCustom,belowParam,0);
		}
	}
	else if (below)
	{
		below = false;
		GameObject *object = id ? (id == -1 ? damager : Commands->Find_Object(id)) : obj;
		Commands->Send_Custom_Event(damager,object,aboveCustom,aboveParam,0);
	}
}
void JMG_Utility_Change_SkinType_To_Blamo_Until_Damaged::Created(GameObject *obj)
{
	minHealthRatio = Get_Float_Parameter("MinHealthRatio");
	sprintf(skinType,"%s",Get_Skin(obj));
	Set_Skin(obj,"Blamo");
}
void JMG_Utility_Change_SkinType_To_Blamo_Until_Damaged::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage < 0)
		return;
	if (Get_Hitpoints(obj)+damage < Get_Max_Hitpoints(obj)*minHealthRatio)
	{
		Set_Skin(obj,skinType);
		Destroy_Script();
	}
}
void JMG_Utility_Send_Custom_When_Moved_Distance_From_Spawn::Created(GameObject *obj)
{
	distance = Get_Float_Parameter("Distance");
	distance *= distance;
	requireInPathfind = Get_Int_Parameter("RequireInPathfind") ? true : false;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("StartDelay"),1);
}
void JMG_Utility_Send_Custom_When_Moved_Distance_From_Spawn::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		location = Commands->Get_Position(obj);
		Vector3 out;
		if (!requireInPathfind || Get_Random_Pathfind_Spot(Commands->Get_Position(obj),0.0f,&out))
			Commands->Start_Timer(obj,this,0.1f,2);
		else
			Commands->Start_Timer(obj,this,0.1f,1);
	}
	if (number == 2)
	{
		Vector3 out;
		if ((!requireInPathfind || Get_Random_Pathfind_Spot(Commands->Get_Position(obj),0.0f,&out)) && JmgUtility::SimpleDistance(Commands->Get_Position(obj),location) >= distance)
		{
			GameObject *object = Get_Int_Parameter("ID") ? Commands->Find_Object(Get_Int_Parameter("ID")) : obj;
			Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),0);
		}
		Commands->Start_Timer(obj,this,0.1f,2);
	}
}

ScriptRegistrant<JMG_Utility_Check_If_Script_Is_In_Library> JMG_Utility_Check_If_Script_Is_In_Library_Registrant("JMG_Utility_Check_If_Script_Is_In_Library","ScriptName:string,CppName:string");
ScriptRegistrant<JMG_Send_Custom_When_Custom_Sequence_Matched> JMG_Send_Custom_When_Custom_Sequence_Matched_Registrant("JMG_Send_Custom_When_Custom_Sequence_Matched","Success_Custom=0:int,Correct_Step_Custom=0:int,Partial_Failure_Custom=0:int,Failure_Custom=0:int,Send_To_ID=0:int,Custom_0=0:int,Custom_1=0:int,Custom_2=0:int,Custom_3=0:int,Custom_4=0:int,Custom_5=0:int,Custom_6=0:int,Custom_7=0:int,Custom_8=0:int,Custom_9=0:int,Disable_On_Success=1:int,Disable_On_Failure=0:int,Starts_Enabled=1:int,Enable_Custom=0:int,Correct_Step_Saftey=0:int,Failure_Saftey=1:int,Max_Failures=1:int");
ScriptRegistrant<JMG_Utility_Change_Model_On_Timer> JMG_Utility_Change_Model_On_Timer_Registrant("JMG_Utility_Change_Model_On_Timer","Model=null:string,Time=0:float");
ScriptRegistrant<JMG_Utility_Emulate_DamageableStaticPhys> JMG_Utility_Emulate_DamageableStaticPhys_Registrant("JMG_Utility_Emulate_DamageableStaticPhys","Animation=null:string,Killed_Explosion=null:string,Live_Loop_Start=-1.0:float,Live_Loop_End=-1.0:float,Live_Twitch_Start=-1.0:float,Live_Twitch_End=-1.0:float,Death_Transition_Start=-1.0:float,Death_Transition_End=-1.0:float,Death_Loop_Start=-1.0:float,Death_Loop_End=-1.0:float,Death_Twitch_Start=-1.0:float,Death_Twitch_End=-1.0:float,Live_Transition_Start=-1.0:float,Live_Transition_End=-1.0:float,Repaired_Explosion=null:string,Play_Twitches_To_Completion:int,Send_To_ID:int,Death_Custom:int,Repaired_Custom:int,Damage_Points:float,Death_Points:float,Heal_Points:float,Repair_Points:float,Neutral_When_Dead:int,Disable_Collision_When_Dead:int");
ScriptRegistrant<JMG_Utility_Display_HUD_Info_Text_All_Players_Custom> JMG_Utility_Display_HUD_Info_Text_To_All_Players_On_Custom_Registrant("JMG_Utility_Display_HUD_Info_Text_All_Players_Custom","Custom:int,StringId:int,ColorRGB:Vector3,Repeatable=1:int");
ScriptRegistrant<JMG_Utility_Display_HUD_Info_Text_To_Sender_On_Custom> JMG_Utility_Display_HUD_Info_Text_To_Sender_On_Custom_Registrant("JMG_Utility_Display_HUD_Info_Text_To_Sender_On_Custom","Custom:int,StringId:int,ColorRGB:Vector3,Repeatable=1:int");
ScriptRegistrant<JMG_Utility_Soldier_Transition_On_Custom> JMG_Utility_Soldier_Transition_On_Custom_Registrant("JMG_Utility_Soldier_Transition_On_Custom","Custom:int");
ScriptRegistrant<JMG_Utility_Poke_Send_Self_Custom> JMG_Utility_Poke_Send_Self_Custom_Registrant("JMG_Utility_Poke_Send_Self_Custom","Custom:int,Param:int,Delay:float,LockoutTime=-1.0:float");
ScriptRegistrant<JMG_Turret_Spawn> JMG_Turret_Spawn_Registrant("JMG_Turret_Spawn","Turret_Preset:string,Bone_Name=Tur_Mount:string");
ScriptRegistrant<JMG_Utility_Sync_System_Object> JMG_Utility_Sync_System_Object_Registrant("JMG_Utility_Sync_System_Object","");
ScriptRegistrant<JMG_Utility_Sync_System_Controller> JMG_Utility_Sync_System_Controller_Registrant("JMG_Utility_Sync_System_Controller","Sync_Rate=1.0:float");
ScriptRegistrant<JMG_Utility_Sync_Object_Periodically> JMG_Utility_Sync_Object_Periodically_Registrant("JMG_Utility_Sync_Object_Periodically","Sync_Rate=1.0:float");
ScriptRegistrant<JMG_Utility_Basic_Spawner> JMG_Utility_Basic_Spawner_Registrant("JMG_Utility_Basic_Spawner","SpawnPreset:string,RespawnTime=0.0:float,RespawnTimeRandom=0:float,StartsEnabled=1:int,EnableOrDisableCustom=0:int,SpawnLimit=-1:int,DelayOnStartup=0:int,Attach_Scripts_Group_ID=-1:int");
ScriptRegistrant<JMG_Utility_Basic_Spawner_Spawned_Object> JMG_Utility_Basic_Spawner_Spawned_Object_Registrant("JMG_Utility_Basic_Spawner_Spawned_Object","ControllerId:int");
ScriptRegistrant<JMG_Utility_AI_Engineer> JMG_Utility_AI_Engineer_Registrant("JMG_Utility_AI_Engineer","RepaiarGun_Preset=Weapon_RepairGun_Player:string,Weapon_Preset=Weapon_Pistol_Player:string,PatrolRange=50.0:float,BaseCenterPoint=0.0 0.0 0.0:vector3,MinHP[Soldiers|Vehicles|Turrets]=0.5 0.75 0.9:vector3,MinHP[EngineerTarget|C4|Beacon]=0.25 0.0 0.0:vector3,Priority[RepairTargets|C4|Beacons]=1.0 1.0 1.0:vector3,WanderGroupID=-1:int");
ScriptRegistrant<JMG_Utility_Set_Object_Visibility_For_Player_On_Custom> JMG_Utility_Set_Object_Visibility_For_Player_On_Custom_Registrant("JMG_Utility_Set_Object_Visibility_For_Player_On_Custom","Custom:int,PlayerID:int,Visible:int");
ScriptRegistrant<JMG_Utility_AI_Vehicle> JMG_Utility_AI_Vehicle_Registrant("JMG_Utility_AI_Vehicle","MaxRange=25.0:float,MinAttackRange=0.0:float,FollowTarget=1:int,ReturnHome=1:int,ForceFire=0:int,VTOLHover=0.0:float,vsSoldier=1:int,vsVehicle=1:int,vsAircraft=1:int,WanderingAIGroupID=-1:int,MinRandWander=60.0:float,MaxRandWander=90.0:float,SquishInfantry=1:int,EnableTransitions=0:int,UsePathfind=1:int,DisableEngineOnArrival=1:int,WeaponError=-1.0:float,MaxHuntRange=0.0:float,NextWanderPointWhenLastReached=0.0:float,OverrideFireMode=0:int,RetreatDistance=0.0:float");
ScriptRegistrant<JMG_Utility_Dynamic_Clock_Control> JMG_Utility_Dynamic_Clock_Control_Registrant("JMG_Utility_Dynamic_Clock_Control","");
ScriptRegistrant<JMG_Utility_Dynamic_Clock_Object> JMG_Utility_Dynamic_Clock_Object_Registrant("JMG_Utility_Dynamic_Clock_Object","");
ScriptRegistrant<JMG_Utility_Change_Screen_Color_While_In_Zone> JMG_Utility_Change_Screen_Color_While_In_Zone_Registrant("JMG_Utility_Change_Screen_Color_While_In_Zone","Color[Red|Green|Blue]:vector3,Opacity:float,EnterFadeInTime:float,ExitFadeOutTime:float");
ScriptRegistrant<JMG_Utility_Reset_Screen_Color_When_Destroyed> JMG_Utility_Reset_Screen_Color_When_Destroyed_Registrant("JMG_Utility_Reset_Screen_Color_When_Destroyed","");
ScriptRegistrant<JMG_Utility_Play_Music_On_Join_Controller> JMG_Utility_Play_Music_On_Join_Controller_Registrant("JMG_Utility_Play_Music_On_Join_Controller","Music=null:string");
ScriptRegistrant<JMG_Utility_Play_Music_On_Join_Change_Music> JMG_Utility_Play_Music_On_Join_Change_Music_Registrant("JMG_Utility_Play_Music_On_Join_Change_Music","Music=null:string,FadeOut:int,FadeIn:int");
ScriptRegistrant<JMG_Utility_Toggle_Door> JMG_Utility_Toggle_Door_Registrant("JMG_Utility_Toggle_Door","DoorID=0:int,Animation=null:string,SwitchAnimation=null:string,LastFrame=0.0:float,SwitchLastFrame=0.0:float,Key=0:int,PlayerType=2:int,DisableSwitchWhenNoShield=0:int,SwitchDisabledModel=null:string,DoorSound=null:string,SwitchSound=null:string,SwitchBlockedSound=null:string,SwitchDamagedPokeSound=null:string,WrongKeySound=null:string,WrongPlayerTypeSound=null:string,TextColor[Red|Green|Blue]=0.0 0.0 0.0:vector3,GotoLastFrameMessage=null:string,GotoFirstFrameMessage=null:string,ActionBlockedMessage=null:string,NoDoorMessage=null:string,SwitchDamagedPokeMessage=null:string,WrongKeyMessage=null:string,WrongPlayerTypeMessage=null:string,DoorControlCustom=0:int,SendCustomId=0:int,SendCustom=0:int");
ScriptRegistrant<JMG_Utility_Set_Animation_Frame_On_Creation> JMG_Utility_Set_Animation_Frame_On_Creation_Registrant("JMG_Utility_Set_Animation_Frame_On_Creation","Animation:string,Frame:float");
ScriptRegistrant<JMG_Utility_Set_Model_On_Damage_Percent_3_States> JMG_Utility_Set_Model_On_Damage_Percent_3_States_Registrant("JMG_Utility_Set_Model_On_Damage_Percent_3_States","FullShieldModel:string,HalfShieldModel:string,NoShieldModel:string");
ScriptRegistrant<JMG_Utility_Set_Model_On_Damage_Percent_4_States> JMG_Utility_Set_Model_On_Damage_Percent_4_States_Registrant("JMG_Utility_Set_Model_On_Damage_Percent_4_States","FullShieldModel:string,TwoThirdsShieldModel:string,ThirdShieldModel:string,NoShieldModel:string");
ScriptRegistrant<JMG_Utility_PCT> JMG_Utility_PCT_Registrant("JMG_Utility_PCT","");
ScriptRegistrant<JMG_Utility_PCT_Inaccessible_Zone> JMG_Utility_PCT_Inaccessible_Zone_Registrant("JMG_Utility_PCT_Inaccessible_Zone","");
ScriptRegistrant<JMG_Utility_PCT_Inaccessible_Zone_Attach> JMG_Utility_PCT_Inaccessible_Zone_Attach_Registrant("JMG_Utility_PCT_Inaccessible_Zone_Attach","");
ScriptRegistrant<JMG_Utility_Display_HUD_Info_Text_All_Players_Custom_Replace_String> JMG_Utility_Display_HUD_Info_Text_All_Players_Custom_2_Registrant("JMG_Utility_Display_HUD_Info_Text_All_Players_Custom_Replace_String","Custom:int,StringId=7233:int,ReplaceString:string,ColorRGB:Vector3,Repeatable=1:int");
ScriptRegistrant<JMG_Utility_Display_Text_Message_To_All_Players_On_Custom> JMG_Utility_Display_Text_Message_To_All_Players_On_Custom_Registrant("JMG_Utility_Display_Text_Message_To_All_Players_On_Custom","Custom:int,Message:string,ColorRGB:Vector3,Repeatable=1:int");
ScriptRegistrant<JMG_Utility_Set_Team_On_Create> JMG_Utility_Set_Team_On_Create_Registrant("JMG_Utility_Set_Team_On_Create","PlayerType:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_AI_Aggressive_Melee> JMG_Utility_AI_Aggressive_Melee_Registrant("JMG_Utility_AI_Aggressive_Melee","ReturnHomeSpeed=1.0:float,ReturnHomeArriveDistance=1.0:float,ChaseSpeed=1.0:float,AttackArriveDistance=0.0:float,StartAttackDistance=3.0:float,MaxHuntDistance=0.0:float,MinLooseVisibilityTime=3:int,MaxLooseVisibilityTime=6:int,NoPathfindDistance=5.0:float");
ScriptRegistrant<JMG_Utility_Infantry_Placed_Buildable_Object> JMG_Utility_Infantry_Placed_Buildable_Object_Registrant("JMG_Utility_Infantry_Placed_Buildable_Object","WeaponPreset=null:string,PreviewPreset=Daves Arrow:string,DeployPreset=null:string,RepairedPreset=null:string,PlaceablePreview=null:string,UnPlaceablePreview=null:string,EnableCustom:int,DisableCustom:int,DeployCustom:int,RegainTimer:int,Cost:float,MessageColor[R|G|B]=255.0 255.0 255.0:vector3,DeploymentBlockedMessage=Cannot deploy when near buildings.:string,PlacementMessage=You can now place another :string,GrantWeapon=1:int,RegenStartsAfterRepair=1:int,MatchTeam[Preview|Deployed|Repaired]=0.0 0.0 1.0:vector3,DestroyedRegenTime=1:int");
ScriptRegistrant<JMG_Utility_Infantry_Placed_Buildable_Object_Attached> JMG_Utility_Infantry_Placed_Buildable_Object_Attached_Registrant("JMG_Utility_Infantry_Placed_Buildable_Object_Attached","ScriptId:int,PlacerId:int,RepairedPreset:string,MatchTeam:int,Team:int");
ScriptRegistrant<JMG_Utility_Lock_Weapon_Selection_While_Script_Attached> JMG_Utility_Lock_Weapon_Selection_While_Script_Attached_Registrant("JMG_Utility_Lock_Weapon_Selection_While_Script_Attached","WeaponPreset:string");
ScriptRegistrant<JMG_Utility_Swimming_zDefault_Map_Fog_Values> JMG_Utility_Swimming_zDefault_Map_Fog_Values_Registrant("JMG_Utility_Swimming_zDefault_Map_Fog_Values","");
ScriptRegistrant<JMG_Utility_Swimming_Zone> JMG_Utility_Swim_While_In_Zone_Registrant("JMG_Utility_Swimming_Zone","WaterColor[R|G|B]=0.28 0.43 0.55:vector3,WaterOpacity=0.5:float,WaterMinViewDistance=5.0:float,WaterMaxViewDistance=15.0:float,SwimmingSpeedMultiplier=0.75:float");
ScriptRegistrant<JMG_Utility_Swimming_Infantry> JMG_Utility_Swimming_Infantry_Registrant("JMG_Utility_Swimming_Infantry","WeaponPreset=Weapon_Swimming_Animations:string,DrownTime=10.0:float,StartDrownSequence=3.0:float,GaspForBreath=SFX.SwimmingGaspForBreath:string,PantingSoundEmitterModel=s_panting:string,HeartBeatSoundEmitterModel=s_heartBeat:string,DrownDamageRate=2.5:float,CatchBreathRate=0.33:float,WaterDamageAmount=0.0:float,WaterDamageWarhead=None:string,EnterWaterMessageStringId=0:int,WaterEnterMessageColor[R|G|B]=0.25 0.25 1.0:vector3,WaterDamageDelayTime=0:int,WaterDamageDelayTimeRecover=0:int,SwimmingSkin:string,SwimmingArmor:string,SwimmingModel:string");
ScriptRegistrant<JMG_Utility_Zone_Enable_Spawners_In_Range> JMG_Utility_Zone_Enable_Spawners_In_Range_Registrant("JMG_Utility_Zone_Enable_Spawners_In_Range","StartID:int,EndID:int,PlayerType=2:int,Enable=1:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Display_Message_On_Vehicle_Enter> JMG_Utility_Display_Message_On_Vehicle_Enter_Registrant("JMG_Utility_Display_Message_On_Vehicle_Enter","StringId:int,MessageOverride=null:string,Color[R|G|B]=0.0 1.0 0.0:vector3,DriverOnly=1:int,ShowOnce=1:int,PlayerType=2:int");
ScriptRegistrant<JMG_Utility_Zone_Apply_Damage_On_Enter> JMG_Utility_Zone_Apply_Damage_On_Enter_Registrant("JMG_Utility_Zone_Apply_Damage_On_Enter","ID:int,DamageAmount:float,Warhead=None:string,DamageOccupants=1:int,PlayerType=2:int,OnlyOnce=0:int");
ScriptRegistrant<JMG_Utility_AI_Guardian_Aircraft> JMG_Utility_AI_Guardian_Aircraft_Registrant("JMG_Utility_AI_Guardian_Aircraft","WanderingAIGroupID:int,FlightHeight=25.0:float,FireRange=-1.0:float,FaceTarget=1:int,StealthModeOverride=0:int,NewPointDelay=0:int");
ScriptRegistrant<JMG_Utility_Switch_Weapon_While_Primary_Empty> JMG_Utility_Switch_Weapon_While_Primary_Empty_Registrant("JMG_Utility_Switch_Weapon_While_Primary_Empty","PrimaryWeapon=null:string,SecondaryWeapon=null:string,ReloadTime=0.0:float,PrimaryToSecondaryTime=-1.0:float,SecondaryToPrimaryTime=-1.0:float,IdlePrimaryAnim=null:string,IdleSecondaryAnim=null:string,PrimaryToSecondaryAnim=null:string,SecondaryToPrimaryAnim=null:string,PrimaryToSecondarySound=null:string,SecondaryToPrimarySound=null:string");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Near_Building> JMG_Utility_Send_Custom_When_Near_Building_Registrant("JMG_Utility_Send_Custom_When_Near_Building","SendCustomObjectID=0:int,NearToBuildingCustom:int,FarFromBuildingCustom:int,CloseToBuildingDistance=1.0:float,BuildingPlayerType=2:int,CheckDeadBuildings=1:int,CheckRate=0.25:float");
ScriptRegistrant<JMG_Utility_AI_Engineer_Repair_Target> JMG_Utility_AI_Engineer_Repair_Target_Registrant("JMG_Utility_AI_Engineer_Repair_Target","");
ScriptRegistrant<JMG_Utility_Reset_Screen_Fade_And_Fog_On_Destroy> JMG_Utility_Reset_Screen_Fade_And_Fog_On_Destroy_Registrant("JMG_Utility_Reset_Screen_Fade_And_Fog_On_Destroy","");
ScriptRegistrant<JMG_Utility_AI_Goto_Player> JMG_Utility_AI_Goto_Player_Registrant("JMG_Utility_AI_Goto_Player","HuntSearchDistance=-1.0:float,HuntSpeed=1.0:float,HuntArriveDistance=1.0:float,RandomHuntArriveDistance=0.0:float,HuntStealth=0:int,AttackSpeed=1.0:float,AttackDistance=-1.0:float,RandomAttackDistance=0.0:float,ReturnHome=1:int,ReturnHomeSpeed=1.0:float,WanderingAIGroupID=-1:int,WanderSpeed=1.0:float,CanSeeStealth=1:int,ShutdownEngineOnArrival=0:int,AttackCheckBlocked=1:int,MaxSightRangeFromHome=0.0:float,WanderDistanceOverride=0.0:float,ChangeWanderGroupCustom=0:int,ChangeWanderSpeedCustom=0:int,ChangeHuntDistanceCustom=0:int,ChangeHuntSpeedCustom=0:int,ChangeReturnHomeSpeedCustom=0:int,ChangeMaxSightFromHomeLocationCustom=0:int,ChangeAttackSpeedCustom=0:int,ChanceToInvestigateLastSeenLocation=0.66:float");
ScriptRegistrant<JMG_Utility_AI_Aggressive_Attack_Spot_Control> JMG_Utility_AI_Aggressive_Attack_Spot_Control_Registrant("JMG_Utility_AI_Aggressive_Attack_Spot_Control","");
ScriptRegistrant<JMG_Utility_AI_Aggressive_Attack_Spot_Point> JMG_Utility_AI_Aggressive_Attack_Spot_Point_Registrant("JMG_Utility_AI_Aggressive_Attack_Spot_Point","GroupId=0:int,AttackOffset=0.0 0.0 0.0:Vector3");
ScriptRegistrant<JMG_Utility_AI_Aggressive_Attack_Spot_Point2> JMG_Utility_AI_Aggressive_Attack_Spot_Point2_Registrant("JMG_Utility_AI_Aggressive_Attack_Spot_Point2","GroupId=0:int,AbsolutePosition=0.0 0.0 0.0:Vector3,ObjectID=0:int");
ScriptRegistrant<JMG_Utility_AI_Aggressive_Attack_Spot> JMG_Utility_AI_Aggressive_Attack_Spot_Registrant("JMG_Utility_AI_Aggressive_Attack_Spot","GroupId=-1:int,SecondaryGroupId=-1:int,AttackPointSpeed=1.0:float,AttackPointDistance=-1.0:float,RandomAttackPointDistance=0.0:float,AttackSpeed=1.0:float,AttackDistance=-1.0:float,RandomAttackDistance=0.0:float,ReturnHome=0:int,ReturnHomeSpeed=1.0:float,ReactToAttackChance=0.5:float,ReactToAttackRate=10.0:float,ShutdownEngineOnArrival=0:int,EnableDebug=0:int,UseSecondaryFire=0:int");
ScriptRegistrant<JMG_Utility_Destroy_Objects_In_ID_Range_On_Death> JMG_Utility_Destroy_Objects_In_ID_Range_On_Death_Registrant("JMG_Utility_Destroy_Objects_In_ID_Range_On_Death","StartID:int,EndID:int");
ScriptRegistrant<JMG_Utility_Custom_Enable_Spawners_In_Range> JMG_Utility_Custom_Enable_Spawners_In_Range_Registrant("JMG_Utility_Custom_Enable_Spawners_In_Range","StartID:int,EndID:int,Custom:int,Enable=1:int");
ScriptRegistrant<JMG_Utility_Send_Custom_On_Player_Count> JMG_Utility_Send_Custom_On_Player_Count_Registrant("JMG_Utility_Send_Custom_On_Player_Count","PlayerCount=0:int,ID=0:int,Custom:int,Param:int,Delay:float,TriggerOn[eq|<|>]=1.0 0.0 0.0:vector3,SupressMatchSpam=1:int,DestroyAfterTrigger=0:int");
ScriptRegistrant<JMG_Utility_Send_Custom_To_Preset_On_Message> JMG_Utility_Send_Custom_To_Preset_On_Message_Registrant("JMG_Utility_Send_Custom_To_Preset_On_Message","TriggerCustom:int,Preset:string,Custom:int,Param:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Basic_Spawner_Attach_Script> JMG_Utility_Basic_Spawner_Attach_Script_Registrant("JMG_Utility_Basic_Spawner_Attach_Script","Script:string,Params:string,Delim=@:string,Scripts_Group_ID=-1:int");
ScriptRegistrant<JMG_Utility_Send_Custom_On_Preset_Enter> JMG_Utility_Send_Custom_On_Preset_Enter_Registrant("JMG_Utility_Send_Custom_On_Preset_Enter","PresetName:string,ID:int,Custom:int,Param:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Basic_Spawner_Set_Prespawn_Location> JMG_Utility_Basic_Spawner_Set_Prespawn_Location_Registrant("JMG_Utility_Basic_Spawner_Set_Prespawn_Location","OverrideLocation:Vector3");
ScriptRegistrant<JMG_Utility_Teleport_On_Pickup> JMG_Utility_Teleport_On_Pickup_Registrant("JMG_Utility_Teleport_On_Pickup","ObjectID:int,Position=0.0 0.0 0.0:Vector3,MaxDistance=1.5:float,Facing=-999.0:float");
ScriptRegistrant<JMG_Utility_Zone_Set_Animation> JMG_Utility_Zone_Set_Animation_Registrant("JMG_Utility_Zone_Set_Animation","ObjectID:int,Animation:string,Looping=0:int,StartFrame=0.0:float,EndFrame=-1.0:float,Blended=1:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Scale_Infantry> JMG_Utility_Scale_Infantry_Registrant("JMG_Utility_Scale_Infantry","Scale=1.0:float");
ScriptRegistrant<JMG_Utility_Set_Innate_On_Create> JMG_Utility_Set_Innate_On_Create_Registrant("JMG_Utility_Set_Innate_On_Create","Enable=1:int");
ScriptRegistrant<JMG_Utility_Set_Soldier_Damage_Animations> JMG_Utility_Set_Soldier_Damage_Animations_Registrant("JMG_Utility_Set_Soldier_Damage_Animations","Enable=1:int");
ScriptRegistrant<JMG_Utility_Play_Music_On_Join_Enter_Change_Music> JMG_Utility_Play_Music_On_Join_Enter_Change_Music_Registrant("JMG_Utility_Play_Music_On_Join_Enter_Change_Music","Music=null:string,FadeOut:int,FadeIn:int,PlayerType=2:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Play_Music_On_Join_Killed_Change_Music> JMG_Utility_Play_Music_On_Join_Killed_Change_Music_Registrant("JMG_Utility_Play_Music_On_Join_Killed_Change_Music","Music=null:string,FadeOut:int,FadeIn:int");
ScriptRegistrant<JMG_Utility_Objective_System_Controller> JMG_Utility_Objective_System_Controller_Registrant("JMG_Utility_Objective_System_Controller","ShowMarkersOnRadar=1:int,PrimaryObjectiveModel=null:string,SecondaryObjectiveModel=null:string,TertiaryObjectiveModel=null:string,Team=2:int");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_Update_Enter> JMG_Utility_Objective_System_Objective_Update_Enter_Registrant("JMG_Utility_Objective_System_Objective_Update_Enter","NewObjectiveID:int,NewObjectiveStringID:int,ObjectiveMarkerObjectID:int,CompleteObjectiveID:int,Delay:float,NewObjectivePriority=1:int");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_Update_Pickup> JMG_Utility_Objective_System_Objective_Update_Pickup_Registrant("JMG_Utility_Objective_System_Objective_Update_Pickup","NewObjectiveID:int,NewObjectiveStringID:int,ObjectiveMarkerObjectID:int,CompleteObjectiveID:int,Delay:float,NewObjectivePriority=1:int");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_Update_Killed> JMG_Utility_Objective_System_Objective_Update_Killed_Registrant("JMG_Utility_Objective_System_Objective_Update_Killed","NewObjectiveID:int,NewObjectiveStringID:int,ObjectiveMarkerObjectID:int,CompleteObjectiveID:int,Delay:float,NewObjectivePriority=1:int");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_Update_Custom> JMG_Utility_Objective_System_Objective_Update_Custom_Registrant("JMG_Utility_Objective_System_Objective_Update_Custom","Custom:int,NewObjectiveID:int,NewObjectiveStringID:int,ObjectiveMarkerObjectID:int,CompleteObjectiveID:int,Delay:float,NewObjectivePriority=1:int");
ScriptRegistrant<JMG_Utility_Destroyed_Apply_Damage> JMG_Utility_Destroyed_Apply_Damage_Registrant("JMG_Utility_Destroyed_Apply_Damage","ID:int,Damage:float,Warhead:string");
ScriptRegistrant<JMG_Utility_Scale_Damage_By_Player_Count> JMG_Utility_Scale_Damage_By_Player_Count_Registrant("JMG_Utility_Scale_Damage_By_Player_Count","MaxPlayersToScaleFor=50:int,MaxScaleFactor=0.95:float");
ScriptRegistrant<JMG_Utility_Scale_Damage_Square_By_Player_Count> JMG_Utility_Scale_Damage_Square_By_Player_Count_Registrant("JMG_Utility_Scale_Damage_Square_By_Player_Count","MaxPlayersToScaleFor=50:int,MaxScaleFactor=0.95:float");
ScriptRegistrant<JMG_Utility_Regen_HitPoints> JMG_Utility_Regen_HitPoints_Registrant("JMG_Utility_Regen_HitPoints","RegenHealth=1:int,HealthAmount=1.0:float,HealthPerPlayer=0.0:float,RegenArmor=1:int,ArmorAmount=1.0:float,ArmorPerPlayer=0.0:float,Rate=1.0:float,DamageDelay=0.0:float,ScaleHealthPerHeal=0.0:float,ScaleArmorPerHeal=0.0:float");
ScriptRegistrant<JMG_Utility_Toggle_Flight_On_Delay> JMG_Utility_Toggle_Flight_On_Delay_Registrant("JMG_Utility_Toggle_Flight_On_Delay","Delay:float");
ScriptRegistrant<JMG_Utility_Fainting_Soldier> JMG_Utility_Fainting_Soldier_Registrant("JMG_Utility_Fainting_Soldier","FaintAnimation:string,LayAnimation:string,StandAnimation:string,FaintSound=null:string,StandSound=null:string,ChangeArmorTypeWhenKnockedOut=0:int,ArmorTypeWhileKnockedOut=Blamo:string,TeamWhileKnockedOut=-2:int");
ScriptRegistrant<JMG_Utility_AI_Guardian_Infantry> JMG_Utility_AI_Guardian_Infantry_Registrant("JMG_Utility_AI_Guardian_Infantry","WanderingAIGroupID:int,WanderSpeed=1.0:float,FireRange=-1.0:float,FaceTarget=1:int,CheckBlocked=1:int,StealthModeOverride=0:int");
ScriptRegistrant<JMG_Utility_Set_Innate_Max_Wander_Distance> JMG_Utility_Set_Innate_Max_Wander_Distance_Registrant("JMG_Utility_Set_Innate_Max_Wander_Distance","Distance:float");
ScriptRegistrant<JMG_Utility_Switch_Weapon_To_Empty_Hands_Until_Custom> JMG_Utility_Switch_Weapon_To_Empty_Hands_Until_Custom_Registrant("JMG_Utility_Switch_Weapon_To_Empty_Hands_Until_Custom","Custom:int");
ScriptRegistrant<JMG_Utility_Set_Skin_And_Shield_Type_On_Custom> JMG_Utility_Set_Skin_And_Shield_Type_On_Custom_Registrant("JMG_Utility_Set_Skin_And_Shield_Type_On_Custom","Custom:int,SkinType=None:string,ShieldType=None:string");
ScriptRegistrant<JMG_Utility_Zone_Damage_All_In_ID_Range_On_Enter>JMG_Utility_Zone_Damage_All_In_ID_Range_On_Enter_Registrant("JMG_Utility_Zone_Damage_All_In_ID_Range_On_Enter","StartID:int,EndID:int,PlayerType=2:int,Damage=0.0:float,Warhead=None:string,Damager=0:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Zone_Send_Custom_To_All_With_Script>JMG_Utility_Zone_Send_Custom_To_All_With_Script_Registrant("JMG_Utility_Zone_Send_Custom_To_All_With_Script","PlayerType=2:int,AttachedScript=ScriptName:string,Custom=0:int,Param=0:int,Delay=0.0:float,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_HitPoints_In_Range_Change_Model> JMG_Utility_HitPoints_In_Range_Change_Model_Registrant("JMG_Utility_HitPoints_In_Range_Change_Model","LowerHitPointsPercent=0.0:float,UpperHitPointsPercent=100.0:float,Model=null:string");
ScriptRegistrant<JMG_Utility_HitPoints_In_Range_Attach_Preset> JMG_Utility_HitPoints_In_Range_Attach_Preset_Registrant("JMG_Utility_HitPoints_In_Range_Attach_Preset","LowerHitPointsPercent=0.0:float,UpperHitPointsPercent=100.0:float,Preset=null:string,BoneName=null:string");
ScriptRegistrant<JMG_Utility_HitPoints_In_Range_Enable_Spawner> JMG_Utility_HitPoints_In_Range_Enable_Spawner_Registrant("JMG_Utility_HitPoints_In_Range_Enable_Spawner","LowerHitPointsPercent=0.0:float,UpperHitPointsPercent=100.0:float,SpawnerID=0:int");
ScriptRegistrant<JMG_Utility_HitPoints_In_Range_Create_Preset> JMG_Utility_HitPoints_In_Range_Create_Preset_Registrant("JMG_Utility_HitPoints_In_Range_Create_Preset","LowerHitPointsPercent=0.0:float,UpperHitPointsPercent=100.0:float,Preset=null:string,Location=0.0 0.0 0.0:Vector3,Facing=0.0:float");
ScriptRegistrant<JMG_Utility_HitPoints_In_Range_Send_Custom> JMG_Utility_HitPoints_In_Range_Send_Custom_Registrant("JMG_Utility_HitPoints_In_Range_Send_Custom","LowerHitPointsPercent=0.0:float,UpperHitPointsPercent=100.0:float,ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Custom_Set_Weather_Precipitation> JMG_Utility_Custom_Set_Weather_Precipitation_Registrant("JMG_Utility_Custom_Set_Weather_Precipitation","Custom:int,Set[Rain|Snow|Ash|]=0.0 0.0 0.0:Vector3,Change[Rain|Snow|Ash|]=1.0 1.0 1.0:Vector3,TransitionTime=0.0:float");
ScriptRegistrant<JMG_Utility_Custom_Set_Weather_Fog> JMG_Utility_Custom_Set_Weather_Fog_Registrant("JMG_Utility_Custom_Set_Weather_Fog","Custom:int,StartDistance=100.0:float,EndDistance=300.0:float,TransitionTime=0.0:float");
ScriptRegistrant<JMG_Utility_Custom_Set_Weather_Wind> JMG_Utility_Custom_Set_Weather_Wind_Registrant("JMG_Utility_Custom_Set_Weather_Wind","Custom:int,Heading=0.0:float,Speed=0.0:float,Variability=0.0:float,TransitionTime=0.0:float");
ScriptRegistrant<JMG_Utility_Custom_Set_Weather_Lightning> JMG_Utility_Custom_Set_Weather_Lightning_Registrant("JMG_Utility_Custom_Set_Weather_Lightning","Custom:int,Intensity=0.0:float,StartDistance=0.0:float,EndDistance=0.0:float,Heading=0.0:float,Distribution=0.0:float,TransitionTime=0.0:float");
ScriptRegistrant<JMG_Utility_Custom_Set_Weather_War_Blitz> JMG_Utility_Custom_Set_Weather_War_Blitz_Registrant("JMG_Utility_Custom_Set_Weather_War_Blitz","Custom:int,Intensity=0.0:float,StartDistance=0.0:float,EndDistance=0.0:float,Heading=0.0:float,Distribution=0.0:float,TransitionTime=0.0:float");
ScriptRegistrant<JMG_Utility_Custom_Set_Weather_Clouds> JMG_Utility_Custom_Set_Weather_Clouds_Registrant("JMG_Utility_Custom_Set_Weather_Clouds","Custom:int,Cover=0.0:float,Gloominess=0.0:float,TransitionTime=0.0:float");
ScriptRegistrant<JMG_Utility_Play_Music_On_Join_Custom_Change_Music> JMG_Utility_Play_Music_On_Join_Custom_Change_Music_Registrant("JMG_Utility_Play_Music_On_Join_Custom_Change_Music","Custom:int,Music=null:string,FadeOut:int,FadeIn:int,Repeat=1:int");
ScriptRegistrant<JMG_Utility_Zone_Send_Custom_If_Has_Weapon> JMG_Utility_Zone_Send_Custom_If_Has_Weapon_Registrant("JMG_Utility_Zone_Send_Custom_If_Has_Weapon","WeaponName:string,ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,PlayerType=2:int,TriggerOnce=1:int,RemoveWeapon=0:int");
ScriptRegistrant<JMG_Utility_Receive_All_Customs_To_Send_Custom> JMG_Utility_Receive_All_Customs_To_Send_Custom_Registrant("JMG_Utility_Receive_All_Customs_To_Send_Custom","Custom0=0:int,Custom1=0:int,Custom2=0:int,Custom3=0:int,Custom4=0:int,Custom5=0:int,Custom6=0:int,Custom7=0:int,Custom8=0:int,Custom9=0:int,ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,TriggerOnce=1:int,ResetCustom=0:int");
ScriptRegistrant<JMG_Utility_Zone_Teleport_To_Random_Wander_Point> JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Registrant("JMG_Utility_Zone_Teleport_To_Random_Wander_Point","WanderingAIGroupID=-1:int,SafeTeleportDistance=1.5:float,ChangeGroupIDCustom=0:int,PlayerType=2:int,RetryOnFailure=0:int,AiOnly=0:int");
ScriptRegistrant<JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach> JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach_Registrant("JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach","WanderingAIGroupID=-1:int,SafeTeleportDistance=1.5:float");
ScriptRegistrant<JMG_Utility_Zone_Set_Player_Type> JMG_Utility_Zone_Set_Player_Type_Registrant("JMG_Utility_Zone_Set_Player_Type","SetPlayerType=0:int,RequiredPlayerType=2:int");
ScriptRegistrant<JMG_Utility_Zone_Send_Custom_Enter> JMG_Utility_Zone_Send_Custom_Enter_Registrant("JMG_Utility_Zone_Send_Custom_Enter","ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,PlayerType=2:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Custom_Destroy_Closest_Object_To_Position> JMG_Utility_Custom_Destroy_Closest_Object_To_Position_Registrant("JMG_Utility_Custom_Destroy_Closest_Object_To_Position","Custom:int,PresetName:string,Position:Vector3,MaxDistance=0.0:float");
ScriptRegistrant<JMG_Utility_Zone_Set_Spawner> JMG_Utility_Zone_Set_Spawner_Registrant("JMG_Utility_Zone_Set_Spawner","SpawnerID:int,PlayerType=2:int,Enable=1:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Persistant_Weapon_Powerup> JMG_Utility_Persistant_Weapon_Powerup_Registrant("JMG_Utility_Persistant_Weapon_Powerup","WeaponName:string,PowerupNameOverride=null:string,DisarmCustom=0:int");
ScriptRegistrant<JMG_Utility_Persistant_Weapon_zStandin> JMG_Utility_Persistant_Weapon_zStandin_Registrant("JMG_Utility_Persistant_Weapon_zStandin","PowerupName:string");
ScriptRegistrant<JMG_Utility_Persistant_Weapon_zAttached> JMG_Utility_Persistant_Weapon_zAttached_Registrant("JMG_Utility_Persistant_Weapon_zAttached","PowerupName:string,WeaponName:string,DisarmCustom:int");
ScriptRegistrant<JMG_Utility_Custom_Teleport_Players_Outside_Range_To_Wanderpoints> JMG_Utility_Custom_Teleport_Players_Outside_Range_To_Wanderpoints_Registrant("JMG_Utility_Custom_Teleport_Players_Outside_Range_To_Wanderpoints","Custom:int,ScanTeleportSpot:vector3,TeleportIgnoreDistance:float,WanderingAIGroupID:int,SafeTeleportDistance=1.5:float");
ScriptRegistrant<JMG_Utility_Custom_Set_Tile_Frame> JMG_Utility_Custom_Set_Tile_Frame_Registrant("JMG_Utility_Custom_Set_Tile_Frame","Custom:int,ID:int,Animation:string,Frame:Float");
ScriptRegistrant<JMG_Utility_Zone_Send_Custom_No_Weapon> JMG_Utility_Zone_Send_Custom_No_Weapon_Registrant("JMG_Utility_Zone_Send_Custom_No_Weapon","WeaponName:string,ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,PlayerType=2:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Custom_Display_Briefing_Message> JMG_Utility_Custom_Display_Briefing_Message_Registrant("JMG_Utility_Custom_Display_Briefing_Message","Custom=0:int,FileName=null:string,TextTag=[]:string,MessageColor=239.0 228.0 176.0:Vector3,LineDelayTime=10.0:float,UISound=null:string");
ScriptRegistrant<JMG_Utility_Zone_Set_Player_Team> JMG_Utility_Zone_Set_Player_Team_Registrant("JMG_Utility_Zone_Set_Player_Team","SetPlayerTeam=0:int,RequiredPlayerTeam=2:int");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_Update_Create> JMG_Utility_Objective_System_Objective_Update_Create_Registrant("JMG_Utility_Objective_System_Objective_Update_Create","NewObjectiveID:int,NewObjectiveStringID:int,ObjectiveMarkerObjectID:int,CompleteObjectiveID:int,Delay:float,NewObjectivePriority=1:int");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_Remove_Custom> JMG_Utility_Objective_System_Objective_Remove_Custom_Registrant("JMG_Utility_Objective_System_Objective_Remove_Custom","Custom:int,ObjectiveID:int,OnlyRemovePending=0:int");
ScriptRegistrant<JMG_Utility_Delay_Then_Rotate_Camera> JMG_Utility_Delay_Then_Rotate_Camera_Registrant("JMG_Utility_Delay_Then_Rotate_Camera","Delay:float,Facing:float");
ScriptRegistrant<JMG_Utility_Damage_SmartGameObjects_In_Range> JMG_Utility_Damage_SmartGameObjects_In_Range_Registrant("JMG_Utility_Damage_SmartGameObjects_In_Range","Range=1.0:float,Damage=1.0:float,Warhead=None:string,Rate=0.1:float");
ScriptRegistrant<JMG_Utility_Unstick_Infantry_If_Stuck> JMG_Utility_Unstick_Infantry_If_Stuck_Registrant("JMG_Utility_Unstick_Infantry_If_Stuck","Rate=0.25:float,Distance=1.25:float");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_On_Count> JMG_Utility_Custom_Send_Custom_On_Count_Registrant("JMG_Utility_Custom_Send_Custom_On_Count","Custom:int,Count:int,ID=0:int,SendCustom:int,Param:int,Delay:float,ResetCustom:int");
ScriptRegistrant<JMG_Utility_Custom_Destroy_Self> JMG_Utility_Custom_Destroy_Self_Registrant("JMG_Utility_Custom_Destroy_Self","Custom:int");
ScriptRegistrant<JMG_Utility_Zone_Set_Player_Team2> JMG_Utility_Zone_Set_Player_Team2_Registrant("JMG_Utility_Zone_Set_Player_Team2","SetPlayerTeam=0:int,RequiredPlayerTeam=2:int");
ScriptRegistrant<JMG_Utility_Poke_Send_Custom> JMG_Utility_Poke_Send_Custom_Registrant("JMG_Utility_Poke_Send_Custom","ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,TriggerOnce=1:int,PlayerType=2:int");
ScriptRegistrant<JMG_Utility_Set_Collision_Group> JMG_Utility_Set_Collision_Group_Registrant("JMG_Utility_Set_Collision_Group","CollisionGroupID:int");
ScriptRegistrant<JMG_Utility_Cap_Credits> JMG_Utility_Cap_Credits_Registrant("JMG_Utility_Cap_Credits","Credits:float,Team=2:int,Custom=0:int");
ScriptRegistrant<JMG_Utility_Custom_Apply_Damage> JMG_Utility_Custom_Apply_Damage_Registrant("JMG_Utility_Custom_Apply_Damage","Custom:int,ID=0:int,Damage:float,Warhead=None:string,DamagerID=0:int");
ScriptRegistrant<JMG_Utility_AI_Goto_Enemy> JMG_Utility_AI_Goto_Enemy_Registrant("JMG_Utility_AI_Goto_Enemy","HuntSearchDistance=-1.0:float,HuntSpeed=1.0:float,HuntArriveDistance=1.0:float,RandomHuntArriveDistance=0.0:float,HuntStealth=0:int,AttackSpeed=1.0:float,AttackDistance=-1.0:float,RandomAttackDistance=0.0:float,ReturnHome=1:int,ReturnHomeSpeed=1.0:float,WanderingAIGroupID=-1:int,WanderSpeed=1.0:float,CanSeeStealth=1:int,ShutdownEngineOnArrival=0:int,AttackCheckBlocked=1:int,MaxSightRangeFromHome=0.0:float,WanderDistanceOverride=0.0:float,ChangeWanderGroupCustom=0:int,ChangeWanderSpeedCustom=0:int,ChangeHuntDistanceCustom=0:int,ChangeHuntSpeedCustom=0:int,ChangeReturnHomeSpeedCustom=0:int,ChangeMaxSightFromHomeLocationCustom=0:int,ChangeAttackSpeedCustom=0:int,ChanceToInvestigateLastSeenLocation=0.66:float");
ScriptRegistrant<JMG_Utility_AI_Goto_Enemy_Not_Star> JMG_Utility_AI_Goto_Enemy_Not_Star_Registrant("JMG_Utility_AI_Goto_Enemy_Not_Star","HuntSearchDistance=-1.0:float,HuntSpeed=1.0:float,HuntArriveDistance=1.0:float,RandomHuntArriveDistance=0.0:float,HuntStealth=0:int,AttackSpeed=1.0:float,AttackDistance=-1.0:float,RandomAttackDistance=0.0:float,ReturnHome=1:int,ReturnHomeSpeed=1.0:float,WanderingAIGroupID=-1:int,WanderSpeed=1.0:float,CanSeeStealth=1:int,ShutdownEngineOnArrival=0:int,AttackCheckBlocked=1:int,MaxSightRangeFromHome=0.0:float,WanderDistanceOverride=0.0:float,ChangeWanderGroupCustom=0:int,ChangeWanderSpeedCustom=0:int,ChangeHuntDistanceCustom=0:int,ChangeHuntSpeedCustom=0:int,ChangeReturnHomeSpeedCustom=0:int,ChangeMaxSightFromHomeLocationCustom=0:int,ChangeAttackSpeedCustom=0:int,ChanceToInvestigateLastSeenLocation=0.66:float");
ScriptRegistrant<JMG_Utility_Grant_Key_On_Create> JMG_Utility_Grant_Key_On_Create_Registrant("JMG_Utility_Grant_Key_On_Create","Key:int,Grant=1:int");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom> JMG_Utility_Custom_Send_Custom_Registrant("JMG_Utility_Custom_Send_Custom","Custom:int,ID=0:int,SendCustom:int,Param:int,Delay=0.0:float,RandomDelay=0.0:float,RandomChance=0.0:float");
ScriptRegistrant<JMG_Utility_Damage_Unoccupied_Vehicle> JMG_Utility_Damage_Unoccupied_Vehicle_Registrant("JMG_Utility_Damage_Unoccupied_Vehicle","Rate=0.1:float,Delay=60.0:float,DecreaseTick=0.1:float,IncreaseTick=0.1:float,Damage=1.0:float,Warhead=None:string");
ScriptRegistrant<JMG_Utility_Custom_Damage_All_Soldiers_On_Team> JMG_Utility_Custom_Damage_All_Soldiers_On_Team_Registrant("JMG_Utility_Custom_Damage_All_Soldiers_On_Team","Custom:int,Team:int,Damage:float,Warhead=None:string");
ScriptRegistrant<JMG_Utility_AI_Guardian_Vehicle> JMG_Utility_AI_Guardian_Vehicle_Registrant("JMG_Utility_AI_Guardian_Vehicle","WanderingAIGroupID:int,WanderSpeed=1.0:float,FireRange=-1.0:float,FaceTarget=1:int,CheckBlocked=1:int,AimAtFeet=1:int,TurnOffEngineOnArrival=1:int,StealthModeOverride=0:int");
ScriptRegistrant<JMG_Utility_Custom_Destroy_Closest_Model_To_Position> JMG_Utility_Custom_Destroy_Closest_Model_To_Position_Registrant("JMG_Utility_Custom_Destroy_Closest_Model_To_Position","Custom:int,Model:string,Position:Vector3,MaxDistance=0.0:float");
ScriptRegistrant<JMG_Utility_Send_Custom_On_Deaths_Controller> JMG_Utility_Send_Custom_On_Deaths_Controller_Registrant("JMG_Utility_Send_Custom_On_Deaths_Controller","MaxDeaths=15:int,DeathReminder=5:int,UrgentDeathReminder=5:int,StringID=12628:int,ReminderMessage=%d %s^ only %d more %s allowed!:string,Delim=^:string,ReminderMessageOrder=0:int,DeathSingular=casualty:string,DeathPlural=casualties:string,RemainingSingular=death:string,RemainingPlural=deaths:string,ReminderColor=1.0 0.0 0.0:Vector3,ID=0:int,Custom=0:int,Param=0:int,Delay=0:int,ReminderCustom=0:int,AddDeathsWhenNoPlayers=10.0:float,NoPlayersAddDeathSaftyTime=150:int,RequiresADeathToStartNoPlayerAdd=1:int,AnnounceOnFirstDeath=1:int,OnlyTrackPlayerDeaths=1:int,RecordAIDeaths=0:int");
ScriptRegistrant<JMG_Utility_Send_Custom_On_Deaths_Reporter> JMG_Utility_Send_Custom_On_Deaths_Reporter_Registrant("JMG_Utility_Send_Custom_On_Deaths_Reporter","");
ScriptRegistrant<JMG_Utility_AI_Goto_Player_Ignore_Object> JMG_Utility_AI_Goto_Player_Ignore_Object_Registrant("JMG_Utility_AI_Goto_Player_Ignore_Object","");
ScriptRegistrant<JMG_Utility_AI_Goto_Enemy_Ignore_Object> JMG_Utility_AI_Goto_Enemy_Ignore_Object_Registrant("JMG_Utility_AI_Goto_Enemy_Ignore_Object","");
ScriptRegistrant<JMG_Utility_AI_Goto_Enemy_Not_Star_Ignore_Object> JMG_Utility_AI_Goto_Enemy_Not_Star_Ignore_Object_Registrant("JMG_Utility_AI_Goto_Enemy_Not_Star_Ignore_Object","");
ScriptRegistrant<JMG_Utility_Custom_Set_Team_And_Model> JMG_Utility_Custom_Set_Team_And_Model_Registrant("JMG_Utility_Custom_Set_Team_And_Model","Custom:int,Model:string,Team:int,ResetActionPriority=100.0:float");
ScriptRegistrant<JMG_Utility_Death_Warhead_Create_Object> JMG_Utility_Death_Warhead_Create_Object_Registrant("JMG_Utility_Death_Warhead_Create_Object","WarheadName:string,Chance=1.0:float,Preset:string");
ScriptRegistrant<JMG_Utility_Zone_Change_Character_On_Preset_Enter> JMG_Utility_Zone_Change_Character_On_Preset_Enter_Registrant("JMG_Utility_Zone_Change_Character_On_Preset_Enter","EntererPreset:string,PlayerType:int,Preset:string");
ScriptRegistrant<JMG_Utility_Sync_HP_With_Object> JMG_Utility_Sync_HP_With_Object_Registrant("JMG_Utility_Sync_HP_With_Object","ID:int,Rate=0.1:float");
ScriptRegistrant<JMG_Utility_Set_Screen_Color_Fade_Controller> JMG_Utility_Set_Screen_Color_Fade_Controller_Registrant("JMG_Utility_Set_Screen_Color_Fade_Controller","Color=0.0 0.0 0.0:Vector3,Opacity=0.0:float");
ScriptRegistrant<JMG_Utility_Set_Screen_Color_Fade_On_Custom> JMG_Utility_Set_Screen_Color_Fade_On_Custom_Registrant("JMG_Utility_Set_Screen_Color_Fade_On_Custom","Custom:int,Color=0.0 0.0 0.0:Vector3,Opacity=0.0:float,Transition=0.0:float,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Set_Screen_Color_Fade_Reset_Player_Create> JMG_Utility_Set_Screen_Color_Fade_Reset_Player_Create_Registrant("JMG_Utility_Set_Screen_Color_Fade_Reset_Player_Create","Transition=0.0:float");
ScriptRegistrant<JMG_Utility_Set_Screen_Color_Fade_Reset_Player_Enter> JMG_Utility_Set_Screen_Color_Fade_Reset_Player_Enter_Registrant("JMG_Utility_Set_Screen_Color_Fade_Reset_Player_Enter","Transition:float,PlayerType=2:int");
ScriptRegistrant<JMG_Utility_Simple_Mech> JMG_Utility_Simple_Mech_Registrant("JMG_Utility_Simple_Mech","ForwardAnimation:string,BackwardAnimation:string,IdleAnimation:string,TurnLeftAnimation:string,TurnRightAnimation:string,ReleaseDelay=4:int");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Preset_In_Range> JMG_Utility_Send_Custom_When_Preset_In_Range_Registrant("JMG_Utility_Send_Custom_When_Preset_In_Range","Preset:string,Range:float,Rate=0.1:float,ID:int,Custom:int,Param:int,Delay=0.0:float,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Death_Create_Object_At_Bone_Position> JMG_Utility_Death_Create_Object_At_Bone_Position_Registrant("JMG_Utility_Death_Create_Object_At_Bone_Position","Preset:string,Bone:string,MaxDistance=1.5:float");
ScriptRegistrant<JMG_Utility_Enable_Loiter> JMG_Utility_Enable_Loiter_Registrant("JMG_Utility_Enable_Loiter","");
ScriptRegistrant<JMG_Utility_Custom_Switch_Weapon> JMG_Utility_Custom_Switch_Weapon_Registrant("JMG_Utility_Custom_Switch_Weapon","Custom:int,Weapon:string");
ScriptRegistrant<JMG_Utility_HUD_Count_Down> JMG_Utility_HUD_Count_Down_Registrant("JMG_Utility_HUD_Count_Down","TimeInSeconds:int,StringID=12628:int,Color=1.0 0.0 0.0:Vector3,WarningMessage=%d %s before launch.:string,Delim=^:string,HourSingular=hour:string,HourPlural=hours:string,MinuteSingular=minute:string,MinutePlural=minutes:string,SecondSingular=second:string,SecondPlural=seconds:string,StartsPaused=0:int,PauseCustom=0:int");
ScriptRegistrant<JMG_Utility_HUD_Count_Down_Send_Custom> JMG_Utility_HUD_Count_Down_Send_Custom_Registrant("JMG_Utility_HUD_Count_Down_Send_Custom","TriggerTime:int,ID:int,Custom:int,Param=0:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Zone_Screen_Fade> JMG_Utility_Zone_Screen_Fade_Registrant("JMG_Utility_Zone_Screen_Fade","Color:vector3,Opacity:float,Transition:float,PlayerType=2:int");
ScriptRegistrant<JMG_Utility_Custom_Triggers_Enemy_Seen> JMG_Utility_Custom_Triggers_Enemy_Seen_Registrant("JMG_Utility_Custom_Triggers_Enemy_Seen","Custom:int,Visible:int");
ScriptRegistrant<JMG_Utility_Screen_Fade_Red_On_Damage> JMG_Utility_Screen_Fade_Red_On_Damage_Registrant("JMG_Utility_Screen_Fade_Red_On_Damage","FadeColor=0.75 0.0 0.0:vector3,FadeOpacity=0.85:float");
ScriptRegistrant<JMG_Utility_Player_Count_Enable_Spawners> JMG_Utility_Player_Count_Enable_Spawners_Registrant("JMG_Utility_Player_Count_Enable_Spawners","StartID:int,EndID:int,PlayerCount[eq|<|>]=-1.0 -1.0 -1.0:vector3,PlayerCount[neq|between(|)]=-1.0 -1.0 -1.0:vector3,EnableOrDisableCustom=0:int,StartsEnabled=1:int");
ScriptRegistrant<JMG_Utility_AI_Engineer_Ignore_Target> JMG_Utility_AI_Engineer_Ignore_Target_Registrant("JMG_Utility_AI_Engineer_Ignore_Target","");
ScriptRegistrant<JMG_Utility_Give_Weapon_In_Range> JMG_Utility_Give_Weapon_In_Range_Registrant("JMG_Utility_Give_Weapon_In_Range","WeaponName:string,Location:vector3,Range:float,EnableOrDisableCustom:int,StartsEnabled=1:int,Rate=0.1:float,SelectWeapon=0:int,WeaponAmmo=1000:int,RefillRate=0.0:float,RefillAmount=0:int");
ScriptRegistrant<JMG_Utility_AI_Follow_Player_On_Poke_Controller> JMG_Utility_AI_Follow_Player_On_Poke_Controller_Registrant("JMG_Utility_AI_Follow_Player_On_Poke_Controller","MaxFollowers=-1:int");
ScriptRegistrant<JMG_Utility_AI_Follow_Player_On_Poke> JMG_Utility_AI_Follow_Player_On_Poke_Registrant("JMG_Utility_AI_Follow_Player_On_Poke","FollowDistance=1.0:float,FollowNearSpeed=0.25:float,FollowFarDistance=10.0:float,FollowFarSpeed=1.0:float,FollowVeryFarDistance=100.0:float,FollowVeryFarSpeed=10.0:float,FallBackWhenOutOfArmor=1:int,RunHomeSpeed=1.0:float,HealWhileAtHomeLocation=0.0:float,HUDMessagesColor=1.0 0.0 0.0:vector3,MessagePokeFollowerID:int,MessageFollowingYouID:int,MessageFollowingPlayerID:int,MessageMaxFollowersID:int,MessageHealingRequiredID:int,MessageFollowerLostID:int,MessageFollowerInjuredID:int,MessageFollowerKilledID:int,SendCustomID:int,PokedCustom:int,LostCustom:int,InjuredCustom:int,KilledCustom:int,HealedCustom:int,MaxFollowTime=3000:int,MaxIdleTime=300:int");
ScriptRegistrant<JMG_Utility_Timer_Damage_And_Teleport> JMG_Utility_Timer_Damage_And_Teleport_Registrant("JMG_Utility_Timer_Damage_And_Teleport","Delay:float,TeleportToObjectID:int,Location:vector3,DamageAmount:float,DamageWarhead=None:string,DamageSelf=1:int");
ScriptRegistrant<JMG_Utility_Zone_Send_Custom_If_Has_Script> JMG_Utility_Zone_Send_Custom_If_Has_Script_Registrant("JMG_Utility_Zone_Send_Custom_If_Has_Script","ScriptName:string,ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,PlayerType=2:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Damage_When_Outside_Of_Range> JMG_Utility_Damage_When_Outside_Of_Range_Registrant("JMG_Utility_Damage_When_Outside_Of_Range","Warn_Distance=0.0:float,Damage_Distance=0.0:float,Vehicle_Warn_Distance=0.0:float,Vehicle_Damage_Distance=0.0:float,Aircraft_Warn_Distance=0.0:float,Aircraft_Damage_Distance=0.0:float,Max_Survive_Distance=0.0:float,Warning_StringID:string,Warning_Color=1.0 0.0 0.0:vector3,Screen_Fade_Color=0.25 0.0 0.0:vector3,DamageWarhead=None:string,InstantKillWarhead=BlamoKiller:string");
ScriptRegistrant<JMG_Utility_Send_Custom_When_No_More_Presets_Of_Type_Exist> JMG_Utility_Send_Custom_When_No_More_Presets_Of_Type_Exist_Registrant("JMG_Utility_Send_Custom_When_No_More_Presets_Of_Type_Exist","PresetName:string,StartsEnabled=0:int,EnableOrDisableCustom:int,ID:int,Custom:int,Param:int,Delay:float");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Player_Outside_Of_Range> JMG_Utility_Send_Custom_When_Player_Outside_Of_Range_Registrant("JMG_Utility_Send_Custom_When_Player_Outside_Of_Range","Distance:float,ID:int,Custom:int,Param:int,Delay:float,TriggerOnce=1:int,SleepTime=10:int,MaxRange=0.0:float");
ScriptRegistrant<JMG_Utility_Set_Team_To_Neutral_When_No_Armor> JMG_Utility_Set_Team_To_Neutral_When_No_Armor_Registrant("JMG_Utility_Set_Team_To_Neutral_When_No_Armor","");
ScriptRegistrant<JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist> JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist_Registrant("JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist","Team:int,StartsEnabled=0:int,EnableOrDisableCustom:int,ID:int,Custom:int,Param:int,Delay:float,Debug=0:int");
ScriptRegistrant<JMG_Utility_Custom_Create_Sound_At_Object_Bone> JMG_Utility_Custom_Create_Sound_At_Object_Bone_Registrant("JMG_Utility_Custom_Create_Sound_At_Object_Bone","Sound:string,Bone:string,Custom:int");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_On_Player_Count> JMG_Utility_Custom_Send_Custom_On_Player_Count_Registrant("JMG_Utility_Custom_Send_Custom_On_Player_Count","Custom:int,BaseCount:int,PlayerCount:int,ID=0:int,SendCustom:int,Param:int,Delay:float,ResetCustom:int");
ScriptRegistrant<JMG_Utility_Killed_By_Player_Send_Custom> JMG_Utility_Killed_By_Player_Send_Custom_Registrant("JMG_Utility_Killed_By_Player_Send_Custom","ID:int,Custom:int,Param:int,Delay:float");
ScriptRegistrant<JMG_Utility_AI_Guardian_Ignored> JMG_Utility_AI_Guardian_Ignored_Registrant("JMG_Utility_AI_Guardian_Ignored","");
ScriptRegistrant<JMG_Utility_AI_Vehicle_Ignored> JMG_Utility_AI_Vehicle_Ignored_Registrant("JMG_Utility_AI_Vehicle_Ignored","");
ScriptRegistrant<JMG_Utility_Killed_By_PresetID_Send_Custom> JMG_Utility_Killed_By_PresetID_Send_Custom_Registrant("JMG_Utility_Killed_By_PresetID_Send_Custom","PresetID:int,ID:int,Custom:int,Param:int,Delay:float");
ScriptRegistrant<JMG_Utility_Created_Override_AI_Soldier_Muzzle_Direction> JMG_Utility_Created_Override_AI_Soldier_Muzzle_Direction_Registrant("JMG_Utility_Created_Override_AI_Soldier_Muzzle_Direction","");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_On_Player_Count_HUD> JMG_Utility_Custom_Send_Custom_On_Player_Count_HUD_Registrant("JMG_Utility_Custom_Send_Custom_On_Player_Count_HUD","Custom:int,BaseCount:int,PlayerCount:int,ID=0:int,SendCustom:int,Param:int,Delay:float,ResetCustom:int,HudStringId:int,HudStringColor:Vector3");
ScriptRegistrant<JMG_Utility_Zone_Send_Custom_If_Has_Weapon_Ammo> JMG_Utility_Zone_Send_Custom_If_Has_Weapon_Ammo_Registrant("JMG_Utility_Zone_Send_Custom_If_Has_Weapon_Ammo","WeaponName:string,ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,PlayerType=2:int,RemoveWeapon=0:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Custom_Remove_Weapon_From_All_Players> JMG_Utility_Custom_Remove_Weapon_From_All_Players_Registrant("JMG_Utility_Custom_Remove_Weapon_From_All_Players","Custom:int,WeaponName:string");
ScriptRegistrant<JMG_Utility_Custom_Destroy_All_Presets_By_Name> JMG_Utility_Custom_Destroy_All_Presets_By_Name_Registrant("JMG_Utility_Custom_Destroy_All_Presets_By_Name","Custom:int,PresetName:string");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Damage_Exceedes_Threshold> JMG_Utility_Send_Custom_When_Damage_Exceedes_Threshold_Registrant("JMG_Utility_Send_Custom_When_Damage_Exceedes_Threshold","DamageThreshold:float,LockoutTime:float,ID:int,Custom:int,Param:int,Delay:float");
ScriptRegistrant<JMG_Utility_Custom_Send_Translated_String_To_All_Players> JMG_Utility_Custom_Send_Translated_String_To_All_Players_Registrant("JMG_Utility_Custom_Send_Translated_String_To_All_Players","Custom:int,StringID:string,ColorRGB:Vector3,Repeatable=1:int");
ScriptRegistrant<JMG_Utility_Destroyed_Drop_Powerup> JMG_Utility_Destroyed_Drop_Powerup_Registrant("JMG_Utility_Destroyed_Drop_Powerup","RequiredWeaponPreset:string,PowerupName:string,HeightAdjust=0.0:float");
ScriptRegistrant<JMG_Utility_Pickup_Attach_Script> JMG_Utility_Pickup_Attach_Script_Registrant("JMG_Utility_Pickup_Attach_Script","Script:string,Params:string,Delim=@:string");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_Status_Update_Custom> JMG_Utility_Objective_System_Objective_Status_Update_Custom_Registrant("JMG_Utility_Objective_System_Objective_Status_Update_Custom","Custom:int,ObjectiveID:int,NewObjectiveStringID:int,NewObjectiveMarkerObjectID:int");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_Failed_Custom> JMG_Utility_Objective_System_Objective_Failed_Custom_Registrant("JMG_Utility_Objective_System_Objective_Failed_Custom","Custom:int,ObjectiveID:int");
ScriptRegistrant<JMG_Utility_Force_Player_Team_At_Gameover> JMG_Utility_Force_Player_Team_At_Gameover_Registrant("JMG_Utility_Force_Player_Team_At_Gameover","Team:int");
ScriptRegistrant<JMG_Utility_AI_Guardian_Generic> JMG_Utility_AI_Guardian_Generic_Registrant("JMG_Utility_AI_Guardian_Generic","WanderingAIGroupID:int,WanderSpeed=1.0:float,FireRange=-1.0:float,FaceTarget=1:int,CheckBlocked=1:int,ArriveDistance=1.0:float,FlightHeight=0.0:float,TurnOffEngineOnArrival=1:int,UseSecondaryAttack=0:int,StealthModeOverride=0:int");
ScriptRegistrant<JMG_Utility_Custom_Create_Object_In_Front_Of_Self> JMG_Utility_Custom_Create_Object_In_Front_Of_Self_Registrant("JMG_Utility_Custom_Create_Object_In_Front_Of_Self","Custom:int,PresetName:string,Distance:float,Height:float,Rotation:float");
ScriptRegistrant<JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist_Ignore> JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist_Ignore_Registrant("JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist_Ignore","Ignore:int");
ScriptRegistrant<JMG_Utility_Custom_Damage_All_Vehicles_On_Team> JMG_Utility_Custom_Damage_All_Vehicles_On_Team_Registrant("JMG_Utility_Custom_Damage_All_Vehicles_On_Team","Custom:int,Team:int,Damage:float,Warhead=None:string");
ScriptRegistrant<JMG_Utility_Kill_Unit_If_Not_Moving_Enough> JMG_Utility_Kill_Unit_If_Not_Moving_Enough_Registrant("JMG_Utility_Kill_Unit_If_Not_Moving_Enough","Time=60.0:float,Distance=1.0:float");
ScriptRegistrant<JMG_Utility_Custom_Display_Briefing_Message_StringID> JMG_Utility_Custom_Display_Briefing_Message_StringID_Registrant("JMG_Utility_Custom_Display_Briefing_Message_StringID","Custom=0:int,StringID:string,MessageColor=239.0 228.0 176.0:Vector3,LineDelayTime=10.0:float,UISound=null:string");
ScriptRegistrant<JMG_Utility_Destroy_Objects_In_ID_Range_On_Killed> JMG_Utility_Destroy_Objects_In_ID_Range_On_Killed_Registrant("JMG_Utility_Destroy_Objects_In_ID_Range_On_Killed","StartID:int,EndID:int");
ScriptRegistrant<JMG_Utility_Timer_Custom> JMG_Utility_Timer_Custom_Registrant("JMG_Utility_Timer_Custom","Time:float,ID:int,Message:int,Param=0:int,Repeat=0:int,EnableCustom=0:int");
ScriptRegistrant<JMG_Utility_Zone_Change_Character_Model> JMG_Utility_Zone_Change_Character_Model_Registrant("JMG_Utility_Zone_Change_Character_Model","NewModel:string,PlayerType=2:int");
ScriptRegistrant<JMG_Utility_Timer_Change_Character_Models_On_Team> JMG_Utility_Timer_Change_Character_Models_On_Team_Registrant("JMG_Utility_Timer_Change_Character_Models_On_Team","Time:float,NewModel:string,PlayerType=2:int,Repeat=0:int,StartsEnabled=1:int,EnableCustom=0:int");
ScriptRegistrant<JMG_Utility_Timer_Change_Character_Models_On_HP> JMG_Utility_Timer_Change_Character_Models_On_HP_Registrant("JMG_Utility_Timer_Change_Character_Models_On_HP","Time:float,NewModel:string,MinHP:float,MaxHP:float,PlayerType=2:int,Repeat=0:int,StartsEnabled=1:int,EnableCustom=0:int");
ScriptRegistrant<JMG_Utility_Destroy_Send_Custom> JMG_Utility_Destroy_Send_Custom_Registrant("JMG_Utility_Destroy_Send_Custom","ID:int,Message:int,Param=0:int,Delay=0:float");
ScriptRegistrant<JMG_Utility_Set_Vehicle_Collisions_Ghost_When_Unoccupied> JMG_Utility_Set_Vehicle_Collisions_Ghost_When_Unoccupied_Registrant("JMG_Utility_Set_Vehicle_Collisions_Ghost_When_Unoccupied","");
ScriptRegistrant<JMG_Utility_Attach_Script_To_All_Players> JMG_Utility_Attach_Script_To_All_Players_Registrant("JMG_Utility_Attach_Script_To_All_Players","Script:string,Params:string,Delim=@:string");
ScriptRegistrant<JMG_Utility_Send_Custom_On_Powerup_Pickup> JMG_Utility_Send_Custom_On_Powerup_Pickup_Registrant("JMG_Utility_Send_Custom_On_Powerup_Pickup","ID:int,Message:int,Param=0:int,Delay=0:float");
ScriptRegistrant<JMG_Utility_Set_Bullets_On_Custom_Or_Damage> JMG_Utility_Set_Bullets_On_Custom_Or_Damage_Registrant("JMG_Utility_Set_Bullets_On_Custom_Or_Damage","WeaponName=null:string,Custom:int,TriggerOnDamage=1:int,SetBackpackBullets=-1:int,StartWithNoBullets=1:int,Repeat=0:int,FullClip=1:int");
ScriptRegistrant<JMG_Utility_Custom_Damage_All_Presets> JMG_Utility_Custom_Damage_All_Presets_Registrant("JMG_Utility_Custom_Damage_All_Presets","Custom:int,PresetName:string,Damage:float,Warhead=None:string");
ScriptRegistrant<JMG_Utility_Death_Weapon_Create_Object> JMG_Utility_Death_Weapon_Create_Object_Registrant("JMG_Utility_Death_Weapon_Create_Object","WeaponPreset:string,Chance=1.0:float,Preset:string");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Speed_Exceeds_Amount> JMG_Utility_Send_Custom_When_Speed_Exceeds_Amount_Registrant("JMG_Utility_Send_Custom_When_Speed_Exceeds_Amount","Speed:float,ID:int,Message:int,Param=0:int,Rate=0.1:float,Repeat=0:int,StartsEnabled=1:int,EnableCustom=0:int");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Speed_Below_Amount> JMG_Utility_Send_Custom_When_Speed_Below_Amount_Registrant("JMG_Utility_Send_Custom_When_Speed_Below_Amount","Speed:float,ID:int,Message:int,Param=0:int,Rate=0.1:float,Repeat=0:int,StartsEnabled=1:int,EnableCustom=0:int");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Velocity_Exceeds_Amount> JMG_Utility_Send_Custom_When_Velocity_Exceeds_Amount_Registrant("JMG_Utility_Send_Custom_When_Velocity_Exceeds_Amount","Velocity[Forward|Backward|Left]=-1.0 -1.0 -1.0:Vector3,Velocity[Right|Up|Down]=-1.0 -1.0 -1.0:Vector3,OnlyTriggerOn=0:int,OnlyTriggerOnMin[Horizontal|Vertical]=-1.0 -1.0 0.0:Vector3,OnlyTriggerOnMax[Horizontal|Vertical]=-1.0 -1.0 0.0:Vector3,ID:int,Message:int,Param=0:int,Rate=0.1:float,Repeat=0:int,StartsEnabled=1:int,EnableCustom=0:int");
ScriptRegistrant<JMG_Utility_Custom_Change_Character> JMG_Utility_Custom_Change_Character_Registrant("JMG_Utility_Custom_Change_Character","Custom:int,Preset:string,Cost:float,SuccessSound:string,FailSound:string,AllowRepurchase=0:int");
ScriptRegistrant<JMG_Utility_Apply_Damage_While_In_Zone> JMG_Utility_Apply_Damage_While_In_Zone_Registrant("JMG_Utility_Apply_Damage_While_In_Zone","Rate:float,DamageAmount:float,Warhead:string,DamagerId:int,Team=2:int");
ScriptRegistrant<JMG_Utility_Apply_Damage_On_Timer_Base> JMG_Utility_Apply_Damage_While_In_Zone_Attached_Registrant("JMG_Utility_Apply_Damage_While_In_Zone_Attached","Rate:float,DamageAmount:float,Warhead:string,DamagerId:int");
ScriptRegistrant<JMG_Utility_Apply_Damage_On_Timer_Base> JMG_Utility_Apply_Damage_On_Timer_Registrant("JMG_Utility_Apply_Damage_On_Timer","Rate:float,DamageAmount:float,Warhead:string,DamagerId:int");
ScriptRegistrant<JMG_Utility_Zone_Send_Custom_Exit> JMG_Utility_Zone_Send_Custom_Exit_Registrant("JMG_Utility_Zone_Send_Custom_Exit","ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,PlayerType=2:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Custom_Enable_Spawners_In_Range_Modular> JMG_Utility_Custom_Enable_Spawners_In_Range_Modular_Registrant("JMG_Utility_Custom_Enable_Spawners_In_Range_Modular","StartID:int,EndID:int,Custom:int,Enable=1:int,Mod=1:int");
ScriptRegistrant<JMG_Utility_Custom_Create_Explosion_At_Bone> JMG_Utility_Custom_Create_Explosion_At_Bone_Registrant("JMG_Utility_Custom_Create_Explosion_At_Bone","Custom:int,Explosion:string,Bone:string,Owner:int,Alive:int");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_Repeat_Ignore_Time> JMG_Utility_Custom_Send_Custom_Repeat_Ignore_Time_Registrant("JMG_Utility_Custom_Send_Custom_Repeat_Ignore_Time","Custom:int,ID:int,SendCustom:int,Param:int,Delay:float,IgnoreTime:float,StartsEnabled=1:int,EnableCustom=0:int");
ScriptRegistrant<JMG_Utility_Damage_Send_Custom> JMG_Utility_Damage_Send_Custom_Registrant("JMG_Utility_Damage_Send_Custom","MinDamage:float,Custom:int,Param:int,Delay:int,ID:int,SenderID:int,StartsEnabled=1:int,EnableCustom=0:int,Repeat=1:int");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Player_Inside_Of_Range> JMG_Utility_Send_Custom_When_Player_Inside_Of_Range_Registrant("JMG_Utility_Send_Custom_When_Player_Inside_Of_Range","Distance:float,ID:int,Custom:int,Param:int,Delay:float,TriggerOnce=1:int,SleepTime=10:int");
ScriptRegistrant<JMG_Utility_Custom_Enable_Spawners_In_Range_ModPlayer> JMG_Utility_Custom_Enable_Spawners_In_Range_ModPlayer_Registrant("JMG_Utility_Custom_Enable_Spawners_In_Range_ModPlayer","StartID:int,EndID:int,Custom:int,Enable=1:int,Mod=1:int,PlayerMulti=1.0:float,Invert=0:int");
ScriptRegistrant<JMG_Utility_Spawn_With_Last_Selected_Gun_Control> JMG_Utility_Spawn_With_Last_Selected_Gun_Control_Registrant("JMG_Utility_Spawn_With_Last_Selected_Gun_Control","");
ScriptRegistrant<JMG_Utility_Spawn_With_Last_Selected_Gun_Player> JMG_Utility_Spawn_With_Last_Selected_Gun_Player_Registrant("JMG_Utility_Spawn_With_Last_Selected_Gun_Player","GrantWeapon=1:int,GrantDelay=0.0:float");
ScriptRegistrant<JMG_Utility_Created_Give_Weapon> JMG_Utility_Created_Give_Weapon_Registrant("JMG_Utility_Created_Give_Weapon","WeaponName:string,GrantWeapon=1:int,GrantRounds=99999:int,GrantClips=1:int");
ScriptRegistrant<JMG_Utility_Credit_Trickle_To_Ammount> JMG_Utility_Credit_Trickle_To_Ammount_Registrant("JMG_Utility_Credit_Trickle_To_Ammount","Credits=1.0:float,Rate=1.0:float,TrickleCap=0.0:float,Team=2:int,Custom=0:int");
ScriptRegistrant<JMG_Utility_Custom_Damage_Objects_On_Team> JMG_Utility_Custom_Damage_Objects_On_Team_Registrant("JMG_Utility_Custom_Damage_Objects_On_Team","Custom:int,Team:int,Damage=0.0:float,Warhead=None:string,Damage[Soldiers|Vehicles|Other]=1.0 1.0 1.0:vector3,Damager=0:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Custom_Set_Animation> JMG_Utility_Custom_Set_Animation_Registrant("JMG_Utility_Custom_Set_Animation","Custom:int,ID:int,Animation:string,Looping=0:int,StartFrame=0.0:float,EndFrame=-1.0:float,Blended=1:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_Marker_Update_Custom> JMG_Utility_Objective_System_Objective_Marker_Update_Custom_Registrant("JMG_Utility_Objective_System_Objective_Marker_Update_Custom","Custom:int,ObjectiveID:int,NewObjectiveMarkerObjectID:int");
ScriptRegistrant<JMG_Utility_Send_Custom_On_Deaths_Reporter_Zone> JMG_Utility_Send_Custom_On_Deaths_Reporter_Zone_Registrant("JMG_Utility_Send_Custom_On_Deaths_Reporter_Zone","");
ScriptRegistrant<JMG_Utility_Killed_Give_Money> JMG_Utility_Killed_Give_Money_Registrant("JMG_Utility_Killed_Give_Money","Money:float");
ScriptRegistrant<JMG_Utility_Created_Set_Damage_And_Death_Points> JMG_Utility_Created_Set_Damage_And_Death_Points_Registrant("JMG_Utility_Created_Set_Damage_And_Death_Points","Damage_Points=-1.0:float,DeathPoints=-1.0:float");
ScriptRegistrant<JMG_Utility_Detect_AFK_Controller> JMG_Utility_Detect_AFK_Controller_Registrant("JMG_Utility_Detect_AFK_Controller","AFKThreshold:int");
ScriptRegistrant<JMG_Utility_Credit_Trickle_When_Not_AFK> JMG_Utility_Credit_Trickle_When_Not_AFK_Registrant("JMG_Utility_Credit_Trickle_When_Not_AFK","Credits=1.0:float,Rate=1.0:float,TrickleCap=0.0:float,Team=2:int,Custom=0:int");
ScriptRegistrant<JMG_Utility_Killed_Create_Object> JMG_Utility_Killed_Create_Object_Registrant("JMG_Utility_Killed_Create_Object","Preset:string");
ScriptRegistrant<JMG_Utility_Damaged_Create_Object_When_Shield_Zero> JMG_Utility_Damaged_Create_Object_When_Shield_Zero_Registrant("JMG_Utility_Damaged_Create_Object_When_Shield_Zero","Preset:string");
ScriptRegistrant<JMG_Utility_Basic_Spawner_In_Radius> JMG_Utility_Basic_Spawner_In_Radius_Registrant("JMG_Utility_Basic_Spawner_In_Radius","Spawn_Preset:string,Spawn_Rate:float,Random_Spawn_Rate:float,Spawn_At_A_Time:int,Min_Spawn_Radius:float,Max_Spawn_Radius:float,Initial_Spawn_Height=0.0:float,Max_Total_Spawned=-1:int,Spawn_Location=-1.0 -1.0 -1.0:Vector3,X_Multiplier=1.0:float,Y_Multiplier=1.0:float,Collision_Check=1:int,Collision_Retry_Attempts=3:int,Collision_Add_Height=1.0:float,Change_Spawn_Cap_Custom=-1:int,Initial_Spawn=-1:int,Spawned_Object_Script_ID=-1:int,Point_Must_Be_In_Pathfind=1:int,Manual_Facing=0:int,Face_Location=0.0 0.0 0.0:vector3,Face_Direction=0.0:float,Ignore_Ray_Cast_Failure=0:int,Min_Distance_Between_Objects=0.0:float,Spawn_Group_ID=0:int,Starts_Disabled=0:int,Enable_Disable_Custom=0:int,Raycast_Direction_Override[Bottom|Top|Unused]=0.0 0.0 0.0:Vector3,Attach_Scripts_Group_ID=-1:int,PlayersAddToSpawnAtATime=0.0:float");
ScriptRegistrant<JMG_Utility_Basic_Spawner_In_Radius_Attached> JMG_Utility_Basic_Spawner_In_Radius_Attached_Registrant("JMG_Utility_Basic_Spawner_In_Radius_Attached","Controller_ID:int,Spawned_Object_Script_ID:int,Spawn_Group_ID:int");
ScriptRegistrant<JMG_Utility_Flying_Vehicle_Crash_Apply_Damage> JMG_Utility_Flying_Vehicle_Crash_Apply_Damage_Registrant("JMG_Utility_Flying_Vehicle_Crash_Apply_Damage","Min_Collision_Speed=33.528:float,Max_Collision_Speed=42.4688:float,Collision_Sound=vehicle_collide_01:string,Explosion_Preset=Plane_Air_Collision:string,Subtract_Min_Speed=0:int");
ScriptRegistrant<JMG_Utility_Enemy_Seen_Send_Custom> JMG_Utility_Enemy_Seen_Send_Custom_Registrant("JMG_Utility_Enemy_Seen_Send_Custom","Enemy_Preset_ID:int,ID:int,Visible_Message:int,Visible_Param:int,Not_Visible_Message:int,Not_Visible_Param:int,Max_Lost_Sight_Time=2.5:float,RepeatSendSeenCustom=0:int,AttackPriority[Car|Tank|Bike]=1.0 1.0 1.0:vector3,AttackPriority[Flying|Turret|Boat]=1.0 1.0 1.0:vector3,AttackPriority[Submarine|Infantry|Unused]=1.0 1.0 0.0:vector3");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_If_Script_Attached> JMG_Utility_Custom_Send_Custom_If_Script_Attached_Registrant("JMG_Utility_Custom_Send_Custom_If_Script_Attached","Custom:int,Script:string,ID=0:int,SendCustom:int,Param:int,Delay:float");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_If_Script_Not_Attached> JMG_Utility_Custom_Send_Custom_If_Script_Not_Attached_Registrant("JMG_Utility_Custom_Send_Custom_If_Script_Not_Attached","Custom:int,Script:string,ID=0:int,SendCustom:int,Param:int,Delay:float");
ScriptRegistrant<JMG_Utility_Basic_Spawner_In_Radius_Controller> JMG_Utility_Basic_Spawner_In_Radius_Controller_Registrant("JMG_Utility_Basic_Spawner_In_Radius_Controller","");
ScriptRegistrant<JMG_Utility_Custom_Set_Engine> JMG_Utility_Custom_Set_Engine_Registrant("JMG_Utility_Custom_Set_Engine","Custom:int,Enable:int");
ScriptRegistrant<JMG_Utility_Send_Custom_Player_Count_Matches_Preset_Count> JMG_Utility_Send_Custom_Player_Count_Matches_Preset_Count_Registrant("JMG_Utility_Send_Custom_Player_Count_Matches_Preset_Count","Preset_ID:int,ID:int,Message:int,Param:int,Delay:float,Min_Player_Count=1:int,Max_Player_Count=-1:int,Rate=0.1:float,Repeat=0:int");
ScriptRegistrant<JMG_Utility_Custom_Set_Position> JMG_Utility_Custom_Set_Position_Registrant("JMG_Utility_Custom_Set_Position","Custom:int,Position:Vector3,ID=0:int");
ScriptRegistrant<JMG_Utility_Custom_Delay_Send_Custom> JMG_Utility_Custom_Delay_Send_Custom_Registrant("JMG_Utility_Custom_Delay_Send_Custom","Custom:int,ID=0:int,SendCustom:int,Param:int,Delay:float,RandomDelay:float,CancelCustom=0:int");
ScriptRegistrant<JMG_Utility_Scale_HP_By_Player_Count> JMG_Utility_Scale_HP_By_Player_Count_Registrant("JMG_Utility_Scale_HP_By_Player_Count","Health_Multiplier=1.0:float,Armor_Multiplier=1.0:float,Max_Player_Count=-1:int,Repeat=1:int,UpdateScaleCustom=0:int");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_To_All_Objects> JMG_Utility_Custom_Send_Custom_To_All_Objects_Registrant("JMG_Utility_Custom_Send_Custom_To_All_Objects","Custom:int,SendCustom:int,Param:int,Team=2:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Enemy_Seen_Send_Custom_Ignore> JMG_Utility_Enemy_Seen_Send_Custom_Ignore_Registrant("JMG_Utility_Enemy_Seen_Send_Custom_Ignore","");
ScriptRegistrant<JMG_Utility_In_Line_Of_Sight_Send_Custom> JMG_Utility_In_Line_Of_Sight_Send_Custom_Registrant("JMG_Utility_In_Line_Of_Sight_Send_Custom","Enemy_Preset_ID:int,ID:int,Visible_Message:int,Visible_Param:int,Not_Visible_Message:int,Not_Visible_Param:int,Scan_Rate=0.1:float,Enemy_Only=1:int");
ScriptRegistrant<JMG_Utility_In_Line_Of_Sight_Send_Custom_Ignore> JMG_Utility_In_Line_Of_Sight_Send_Custom_Ignore_Registrant("JMG_Utility_In_Line_Of_Sight_Send_Custom_Ignore","");
ScriptRegistrant<JMG_Utility_Timer_Trigger_Enemy_Seen> JMG_Utility_Timer_Trigger_Enemy_Seen_Registrant("JMG_Utility_Timer_Trigger_Enemy_Seen","Scan_Rate=0.1:float");
ScriptRegistrant<JMG_Utility_Custom_Teleport_To_Random_Wander_Point> JMG_Utility_Custom_Teleport_To_Random_Wander_Point_Registrant("JMG_Utility_Custom_Teleport_To_Random_Wander_Point","Custom:int,WanderingAIGroupID=-1:int,SafeTeleportDistance=1.5:float,RetryOnFailure=0:int");
ScriptRegistrant<JMG_Utility_Send_Custom_If_Not_Moving_Enough> JMG_Utility_Send_Custom_If_Not_Moving_Enough_Registrant("JMG_Utility_Send_Custom_If_Not_Moving_Enough","Time:float,Distance:float,ID=0:int,SendCustom:int,Param:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_AI_Skittish_Herd_Animal> JMG_Utility_AI_Skittish_Herd_Animal_Registrant("JMG_Utility_AI_Skittish_Herd_Animal","MinHerdID=0:int,MaxHerdID=0:int,WanderGroupID=0:int,WanderRadiusAroundHerdCenter=25.0:float,MinWanderFrequency=5.0:float,MaxWanderFrequency=30.0:float,MinRetreatRange=50.0:float,MaxRetreatRange=200.0:float,MinRetreatTime=6.0:float,MaxRetreatTime=24.0:float,MinUpdateHerdCenter=10.0:float,MaxUpdateHerdCenter=25.0:float,RunTowardThreatChance=0.0:float,ActionsCrouched=0.25:float");
ScriptRegistrant<JMG_Utility_AI_Skittish_Herd_Animal_Ignore> JMG_Utility_AI_Skittish_Herd_Animal_Ignore_Registrant("JMG_Utility_AI_Skittish_Herd_Animal_Ignore","");
ScriptRegistrant<JMG_Utility_AI_Skittish_Herd_Animal_Controller> JMG_Utility_AI_Skittish_Herd_Animal_Controller_Registrant("JMG_Utility_AI_Skittish_Herd_Animal_Controller","");
ScriptRegistrant<JMG_Utility_Custom_Display_Dialog_Box> JMG_Utility_Custom_Display_Dialog_Box_Registrant("JMG_Utility_Custom_Display_Dialog_Box","Custom:int,Message:string,Delim=@:string");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_On_Preset_Count> JMG_Utility_Custom_Send_Custom_On_Preset_Count_Registrant("JMG_Utility_Custom_Send_Custom_On_Preset_Count","Custom:int,PresetID:int,MinCount=1:int,MaxCount=-1:int,ID=0:int,SendCustom:int,Param:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_On_Script_Count> JMG_Utility_Custom_Send_Custom_On_Script_Count_Registrant("JMG_Utility_Custom_Send_Custom_On_Script_Count","Custom:int,Script:string,MinCount=1:int,MaxCount=-1:int,ID=0:int,SendCustom:int,Param:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Poke_Send_Custom_From_Poker> JMG_Utility_Poke_Send_Custom_From_Poker_Registrant("JMG_Utility_Poke_Send_Custom_From_Poker","ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,TriggerOnce=1:int,PlayerType=2:int");
ScriptRegistrant<JMG_Utility_Custom_Grant_Scaled_Score> JMG_Utility_Custom_Grant_Scaled_Score_Registrant("JMG_Utility_Custom_Grant_Scaled_Score","GrantCustom:int,Score:float,ScoreMultiplier:float,EntireTeam=0:int,MaxPlayerCount=-1:int,Repeat=1:int,GrantToSender=0:int,UpdateScaleCustom=0:int,StopUpdateCustom=0:int");
ScriptRegistrant<JMG_Utility_Custom_Set_Infantry_Height> JMG_Utility_Custom_Set_Infantry_Height_Registrant("JMG_Utility_Custom_Set_Infantry_Height","Custom:int,Height=-9999.0:float");
ScriptRegistrant<JMG_Utility_Custom_Set_Infantry_Width> JMG_Utility_Custom_Set_Infantry_Width_Registrant("JMG_Utility_Custom_Set_Infantry_Width","Custom:int,Width=-9999.0:float");
ScriptRegistrant<JMG_Utility_Custom_Set_Weapon_Hold_Sytle> JMG_Utility_Custom_Set_Weapon_Hold_Sytle_Registrant("JMG_Utility_Custom_Set_Weapon_Hold_Sytle","Custom:int,Style=-1:int");
ScriptRegistrant<JMG_Utility_Custom_Set_Human_Anim_Override> JMG_Utility_Custom_Set_Human_Anim_Override_Registrant("JMG_Utility_Custom_Set_Human_Anim_Override","Custom:int,Enable:int");
ScriptRegistrant<JMG_Utility_Swimming_Infantry_Advanced> JMG_Utility_Swimming_Infantry_Advanced_Registrant("JMG_Utility_Swimming_Infantry_Advanced","WeaponsGroupID=0:int,WeaponPreset=Weapon_Swimming_Animations:string,ForceDefinedWeapons=0:int,DefaultHoldStyle=4:int,DefaultSwimSpeed=1.0:float,DrownTime=10.0:float,StartDrownSequence=3.0:float,GaspForBreath=SFX.SwimmingGaspForBreath:string,PantingSoundEmitterModel=s_panting:string,HeartBeatSoundEmitterModel=s_heartBeat:string,DrownDamageRate=2.5:float,CatchBreathRate=0.33:float,WaterDamageAmount=0.0:float,WaterDamageDelayTime=0:int,WaterDamageDelayTimeRecover=0:int,WaterDamageWarhead=None:string,EnterWaterMessageStringId=0:int,WaterEnterMessageColor[R|G|B]=0.25 0.25 1.0:vector3,SwimmingSkin:string,SwimmingArmor:string,SwimmingModel:string,SwimmingHeightScale=999.99:float,SwimmingWidthScale=999.99:float");
ScriptRegistrant<JMG_Utility_Swimming_Infantry_Advanced_Controller> JMG_Utility_Swimming_Infantry_Advanced_Controller_Registrant("JMG_Utility_Swimming_Infantry_Advanced_Controller","");
ScriptRegistrant<JMG_Utility_Swimming_Infantry_Advanced_Add_All_Of_Style> JMG_Utility_Swimming_Infantry_Advanced_Add_All_Of_Style_Registrant("JMG_Utility_Swimming_Infantry_Advanced_Add_All_Of_Style","WeaponGroupID:int,HoldStyle:int,AnimHoldStyle:int,MovementSpeed=1.0:float,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Swimming_Infantry_Advanced_Add_Weapon> JMG_Utility_Swimming_Infantry_Advanced_Add_Weapon_Registrant("JMG_Utility_Swimming_Infantry_Advanced_Add_Weapon","WeaponGroupID:int,WeaponName:string,AnimHoldStyle:int,MovementSpeed=1.0:float,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Custom_Send_Cycled_Customs> JMG_Utility_Custom_Send_Cycled_Customs_Registrant("JMG_Utility_Custom_Send_Cycled_Customs","Custom:int,ID=0:int,SendCustom0=0:int,SendCustom1=0:int,SendCustom2=0:int,SendCustom3=0:int,SendCustom4=0:int,SendCustom5=0:int,SendCustom6=0:int,SendCustom7=0:int,SendCustom8=0:int,SendCustom9=0:int,Param0=0:int,Param1=0:int,Param2=0:int,Param3=0:int,Param4=0:int,Param5=0:int,Param6:=0int,Param7=0:int,Param8=0:int,Param9=0:int,Delay0=0.0:float,Delay1=0.0:float,Delay2=0.0:float,Delay3=0.0:float,Delay4=0.0:float,Delay5=0.0:float,Delay6=0.0:float,Delay7=0.0:float,Delay8=0.0:float,Delay9=0.0:float,RandomDelay=0.0:float,RandomChance=0.0:float");
ScriptRegistrant<JMG_Utility_Killed_Send_Custom_From_Killer> JMG_Utility_Killed_Send_Custom_From_Killer_Registrant("JMG_Utility_Killed_Send_Custom_From_Killer","ID:int,Custom:int,Param:int,Delay:float");
ScriptRegistrant<JMG_Utility_Emulate_Sound_Heard_On_FDS> JMG_Utility_Emulate_Sound_Heard_On_FDS_Registrant("JMG_Utility_Emulate_Sound_Heard_On_FDS","DefaultCrouchMoveSoundID=3446:int,DefaultWalkMoveSoundID=3446:int,DefaultRunMoveSoundID=3569:int,UpdateRate=0.1:float");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Team_Dominates_Zone> JMG_Utility_Send_Custom_When_Team_Dominates_Zone_Registrant("JMG_Utility_Send_Custom_When_Team_Dominates_Zone","Team:int,ID:int,CaptureCustom:int,CaptureParam:int,LostCustom:int,LostParam:int,CaptureDelay:float,Rate=0.1:float,StartsEnabled=1:int,EnableCustom=0:int,SendCustomEveryTick=0:int");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Team_Zone> JMG_Utility_Send_Custom_When_Team_Zone_Registrant("JMG_Utility_Send_Custom_When_Team_Zone","Team:int,ID:int,InCustom:int,InParam:int,OutCustom:int,OutParam:int,Delay:float,Rate=0.1:float,StartsEnabled=1:int,EnableCustom=0:int,SendCustomEveryTick=0:int");
ScriptRegistrant<JMG_Utility_HUD_Count_Down_Visible_Countdown> JMG_Utility_HUD_Count_Down_Visible_Countdown_Registrant("JMG_Utility_HUD_Count_Down_Visible_Countdown","CharPos:int,BaseModelName:string");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_On_Secondary_Count> JMG_Utility_Custom_Send_Custom_On_Secondary_Count_Registrant("JMG_Utility_Custom_Send_Custom_On_Secondary_Count","TriggerCustom:int,CountCustom:int,MatchCount:int,ID:int,SendCustom:int,Param:int,Delay:float,ResetCountCustom:int,OnceMatchedContinue:int");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Not_Team_Zone> JMG_Utility_Send_Custom_When_Not_Team_Zone_Registrant("JMG_Utility_Send_Custom_When_Not_Team_Zone","Team:int,ID:int,InCustom:int,InParam:int,OutCustom:int,OutParam:int,Delay:float,Rate=0.1:float,StartsEnabled=1:int,EnableCustom=0:int,SendCustomEveryTick=0:int");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Team_Dominates_Zone_Ignore> JMG_Utility_Send_Custom_When_Team_Dominates_Zone_Ignore_Registrant("JMG_Utility_Send_Custom_When_Team_Dominates_Zone_Ignore","");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Team_Zone_Ignore> JMG_Utility_Send_Custom_When_Team_Zone_Ignore_Registrant("JMG_Utility_Send_Custom_When_Team_Zone_Ignore","");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Not_Team_Zone_Ignore> JMG_Utility_Send_Custom_When_Not_Team_Zone_Ignore_Registrant("JMG_Utility_Send_Custom_When_Not_Team_Zone_Ignore","");
ScriptRegistrant<JMG_Utility_Send_Custom_On_Armor> JMG_Utility_Send_Custom_On_Armor_Registrant("JMG_Utility_Send_Custom_On_Armor","ID:int,ArmorCustom:int,ArmorParam:int,NoArmorCustom:int,NoArmorParam:int,Delay:float");
ScriptRegistrant<JMG_Utility_Base_Defense_Simple> JMG_Utility_Base_Defense_Simple_Registrant("JMG_Utility_Base_Defense_Simple","MinAttackDistance:float,MaxAttackDistance:float,AttackPriority[Car|Tank|Bike]=1.0 1.0 1.0:vector3,AttackPriority[Flying|Turret|Boat]=1.0 1.0 1.0:vector3,AttackPriority[Submarine|Infantry|Unused]=1.0 1.0 0.0:vector3,ResetTime=3:int");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_Supress_Spam> JMG_Utility_Custom_Send_Custom_Supress_Spam_Registrant("JMG_Utility_Custom_Send_Custom_Supress_Spam","Custom0:int,Custom1:int,Custom2:int,Custom3:int,Custom4:int,Custom5:int,Custom6:int,Custom7:int,Custom8:int,Custom9:int,ID:int,SpamDelay:float,SendDuplicates=0:int");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Player_Between_Range> JMG_Utility_Send_Custom_When_Player_Between_Range_Registrant("JMG_Utility_Send_Custom_When_Player_Between_Range","MinDistance:float,MaxDistance:float,ID:int,Custom:int,Param:int,Delay:float,TriggerOnce=1:int,SleepTime=10:int");
ScriptRegistrant<JMG_Utility_Custom_Display_Briefing_PAMSG_Message_To_Player> JMG_Utility_Custom_Display_Briefing_PAMSG_Message_To_Player_Registrant("JMG_Utility_Custom_Display_Briefing_PAMSG_Message_To_Player","Custom=0:int,FileName=null:string,TextTag=[]:string");
ScriptRegistrant<JMG_Utility_Send_Custom_On_Powerup_Pickup_Collector> JMG_Utility_Send_Custom_On_Powerup_Pickup_Collector_Registrant("JMG_Utility_Send_Custom_On_Powerup_Pickup_Collector","ID:int,Message:int,Param=0:int,Delay=0:float");
ScriptRegistrant<JMG_Utility_Zone_Send_Custom_Enter_From_Enterer> JMG_Utility_Zone_Send_Custom_Enter_From_Enterer_Registrant("JMG_Utility_Zone_Send_Custom_Enter_From_Enterer","ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,PlayerType=2:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_AI_Lobbed_Vehicle_Projectile_Custom> JMG_Utility_AI_Lobbed_Vehicle_Projectile_Custom_Registrant("JMG_Utility_AI_Lobbed_Vehicle_Projectile_Custom","TargetCustom:int,MinDistance=0.0:float,MinAngle=-90.0:float,MaxAngle=90.0:float,UseLowAngleMaxDistance=0.0:float,UseLowAngleWhenAboveMinDistance=1:int,UseLowAngleTargetAboveHeight=9999.9:float,VehicleProjectilePreset:string,FireVelocity=1.0:float,GravityScale=1.0:float,MissAmountPerMeter=0.0:float,BaseMissAmount=0.0:float,ProjectedShotsChance=1.0:float,FireRate=1.0:float,ReloadTime=1.0:float,ReloadSound=null:string,ClipCount=1:int,FireSound=null:string,MuzzleFlashExplosion=null:string,ProjectileExplosion=null:string,CustomTimeoutTime=3:int,AimTurret=1:int");
ScriptRegistrant<JMG_Utility_Vehicle_Enter_Send_Custom_From_Enterer> JMG_Utility_Vehicle_Enter_Send_Custom_From_Enterer_Registrant("JMG_Utility_Vehicle_Enter_Send_Custom_From_Enterer","PlayerType=2:int,ID:int,Custom:int,Param:int,Delay:float,TriggerOnce:int");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_On_Count_From_Sender> JMG_Utility_Custom_Send_Custom_On_Count_From_Sender_Registrant("JMG_Utility_Custom_Send_Custom_On_Count_From_Sender","Custom:int,Count:int,ID=0:int,SendCustom:int,Param:int,Delay:float,ResetCustom:int");
ScriptRegistrant<JMG_Utility_Player_Seen_Send_Custom> JMG_Utility_Player_Seen_Send_Custom_Registrant("JMG_Utility_Player_Seen_Send_Custom","ID:int,Custom:int,Param:int,Delay:float,TriggerOnce:int");
ScriptRegistrant<JMG_Utility_Enter_Play_Animation_And_Relocate> JMG_Utility_Enter_Play_Animation_And_Relocate_Registrant("JMG_Utility_Enter_Play_Animation_And_Relocate","PresetName:string,Animation:string,AttachedModel:string,AttachedAnimation:string,CenterToZone:int,Custom:int,Param:int");
ScriptRegistrant<JMG_Utility_Enter_Play_Animation_And_Relocate_Attached> JMG_Utility_Enter_Play_Animation_And_Relocate_Attached_Registrant("JMG_Utility_Enter_Play_Animation_And_Relocate_Attached","AttachedModel:string,AttachedAnimation:string,Facing:float,Custom:int,Param:int");
ScriptRegistrant<JMG_Utility_Custom_Destroy_Closest_Object_To_Self> JMG_Utility_Custom_Destroy_Closest_Object_To_Self_Registrant("JMG_Utility_Custom_Destroy_Closest_Object_To_Self","Custom:int,PresetName:string,MaxDistance=0.0:float,TriggerOnce=0:int");
ScriptRegistrant<JMG_Utility_Custom_Apply_Damage_Closest_Object_To_Self> JMG_Utility_Custom_Apply_Damage_Closest_Object_To_Self_Registrant("JMG_Utility_Custom_Apply_Damage_Closest_Object_To_Self","Custom:int,PresetName:string,Warhead:string,DamageAmount:float,MaxDistance=0.0:float,TriggerOnce=0:int");
ScriptRegistrant<JMG_Utility_Custom_Restore_Building> JMG_Utility_Custom_Restore_Building_Registrant("JMG_Utility_Custom_Restore_Building","Custom:int,ID:int,TriggerOnce:int");
ScriptRegistrant<JMG_Utility_Timer_Custom_Random> JMG_Utility_Timer_Custom_Random_Registrant("JMG_Utility_Timer_Custom_Random","Time:float,Random:float,ID:int,Message:int,Param=0:int,Repeat=0:int");
ScriptRegistrant<JMG_Utility_Dynamic_Script_Controller> JMG_Utility_Dynamic_Script_Controller_Registrant("JMG_Utility_Dynamic_Script_Controller","");
ScriptRegistrant<JMG_Utility_Dynamic_Script_Definition> JMG_Utility_Dynamic_Script_Definition_Registrant("JMG_Utility_Dynamic_Script_Definition","ScriptID:int,ScriptName:string");
ScriptRegistrant<JMG_Utility_Dynamic_Script_Created_Add_Update_Parameter> JMG_Utility_Dynamic_Script_Created_Add_Update_Parameter_Registrant("JMG_Utility_Dynamic_Script_Created_Add_Update_Parameter","ScriptID:int,Index:int,Param:string,Delim:string");
ScriptRegistrant<JMG_Utility_Dynamic_Script_Custom_Add_Update_Parameter> JMG_Utility_Dynamic_Script_Custom_Add_Update_Parameter_Registrant("JMG_Utility_Dynamic_Script_Custom_Add_Update_Parameter","Custom:int,ScriptID:int,Index:int,Param:string,Delim:string");
ScriptRegistrant<JMG_Utility_Dynamic_Script_Custom_Attach> JMG_Utility_Dynamic_Script_Custom_Attach_Registrant("JMG_Utility_Dynamic_Script_Custom_Attach","Custom:int,ScriptID:int");
ScriptRegistrant<JMG_Utility_Objective_System_Custom_Add_Objective_Send_Custom> JMG_Utility_Objective_System_Custom_Add_Objective_Send_Custom_Registrant("JMG_Utility_Objective_System_Custom_Add_Objective_Send_Custom","Custom:int,NewObjectiveID:int,NewObjectiveStringID:int,ObjectiveMarkerObjectID:int,CompleteObjectiveID:int,Delay:float,NewObjectivePriority=1:int,SendCustom:int,Param:int,CustomDelay:float");
ScriptRegistrant<JMG_Utility_Objective_System_Custom_Send_Custom_Status> JMG_Utility_Objective_System_Custom_Send_Custom_Status_Registrant("JMG_Utility_Objective_System_Custom_Send_Custom_Status","Custom:int,ObjectiveID:int,Status:int,ID=0:int,SendCustom:int,Param:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Objective_System_Custom_Send_Custom_Does_Not_Exist> JMG_Utility_Objective_System_Custom_Send_Custom_Does_Not_Exist_Registrant("JMG_Utility_Objective_System_Custom_Send_Custom_Does_Not_Exist","Custom:int,ObjectiveID:int,ID=0:int,SendCustom:int,Param:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Objective_System_Send_Random_Objective_As_Custom> JMG_Utility_Objective_System_Send_Random_Objective_As_Custom_Registrant("JMG_Utility_Objective_System_Send_Random_Objective_As_Custom","Custom:int,ObjectiveID0=-999:int,ObjectiveID1=-999:int,ObjectiveID2=-999:int,ObjectiveID3=-999:int,ObjectiveID4=-999:int,ObjectiveID5=-999:int,ObjectiveID6=-999:int,ObjectiveID7=-999:int,ObjectiveID8=-999:int,ObjectiveID9=-999:int,ID=0:int,Param:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Objective_System_Custom_Send_Custom_Not_Status> JMG_Utility_Objective_System_Custom_Send_Custom_Not_Status_Registrant("JMG_Utility_Objective_System_Custom_Send_Custom_Not_Status","Custom:int,ObjectiveID:int,Status:int,ID=0:int,SendCustom:int,Param:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Global_Keycard_System_Controller> JMG_Utility_Global_Keycard_System_Controller_Registrant("JMG_Utility_Global_Keycard_System_Controller","");
ScriptRegistrant<JMG_Utility_Global_Keycard_System_Created_Add_Keycard> JMG_Utility_Global_Keycard_System_Created_Add_Keycard_Registrant("JMG_Utility_Global_Keycard_System_Created_Add_Keycard","KeycardID:int,GroupID:int");
ScriptRegistrant<JMG_Utility_Global_Keycard_System_Custom_Add_Keycard> JMG_Utility_Global_Keycard_System_Custom_Add_Keycard_Registrant("JMG_Utility_Global_Keycard_System_Custom_Add_Keycard","Custom:int,KeycardID:int,GroupID:int");
ScriptRegistrant<JMG_Utility_Global_Keycard_System_Custom_Remove_Keycard> JMG_Utility_Global_Keycard_System_Custom_Remove_Keycard_Registrant("JMG_Utility_Global_Keycard_System_Custom_Remove_Keycard","Custom:int,KeycardID:int,GroupID:int");
ScriptRegistrant<JMG_Utility_Global_Keycard_System_Soldier> JMG_Utility_Global_Keycard_System_Soldier_Registrant("JMG_Utility_Global_Keycard_System_Soldier","GroupID:int");
ScriptRegistrant<JMG_Utility_Zone_Send_Custom_On_Player_Occupation_Change> JMG_Utility_Zone_Send_Custom_On_Player_Occupation_Change_Registrant("JMG_Utility_Zone_Send_Custom_On_Player_Occupation_Change","OccupiedID:int,OccupiedCustom:int,OccupiedParam:int,OccupiedDelay:float,VacantID:int,VacantCustom:int,VacantParam:int,VacantDelay:float");
ScriptRegistrant<JMG_Utility_Zone_Send_Custom_On_Player_Occupation_Change_Attached> JMG_Utility_Zone_Send_Custom_On_Player_Occupation_Change_Attached_Registrant("JMG_Utility_Zone_Send_Custom_On_Player_Occupation_Change_Attached","ID:int");
ScriptRegistrant<JMG_Utility_Silent_Countdown_Controller> JMG_Utility_Silent_Countdown_Controller_Registrant("JMG_Utility_Silent_Countdown_Controller","");
ScriptRegistrant<JMG_Utility_Silent_Countdown_Timer> JMG_Utility_Silent_Countdown_Timer_Registrant("JMG_Utility_Silent_Countdown_Timer","TimerID:int,Time:int,StartCustom:int,PausedCustom:int,CancelCustom:int");
ScriptRegistrant<JMG_Utility_Silent_Countdown_Send_Custom> JMG_Utility_Silent_Countdown_Send_Custom_Registrant("JMG_Utility_Silent_Countdown_Send_Custom","TimerID:int,TriggerTime:int,ID:int,Custom:int,Param:int,Delay:float");
ScriptRegistrant<JMG_Utility_Zone_Teleport_To_Random_WP_Boss> JMG_Utility_Zone_Teleport_To_Random_WP_Boss_Registrant("JMG_Utility_Zone_Teleport_To_Random_WP_Boss","WanderingAIGroupID=-1:int,SafeTeleportDistance=1.5:float,ChangeGroupIDCustom=0:int,PlayerType=2:int,RetryOnFailure=0:int,AiOnly=0:int,FaceBoss=1:int");
ScriptRegistrant<JMG_Utility_Zone_Teleport_To_Random_WP_Boss_Object> JMG_Utility_Zone_Teleport_To_Random_WP_Boss_Object_Registrant("JMG_Utility_Zone_Teleport_To_Random_WP_Boss_Object","");
ScriptRegistrant<JMG_Utility_Custom_Set_Soldier_Speed> JMG_Utility_Custom_Set_Soldier_Speed_Registrant("JMG_Utility_Custom_Set_Soldier_Speed","Custom:int,Speed:float");
ScriptRegistrant<JMG_Utility_Control_Point_Controller> JMG_Utiliwty_Control_Point_Controller_Registrant("JMG_Utility_Control_Point_Controller","NeutralTeamName=Neutral:string,NeutralPlayerType=-2:int,NeutralRadarBlipColor=2:int,NeutralDefaultPointModel:string,NeutralDefaultAnimation:string,NeutralAnimationLength:float,NeutralCaptureSound:string,NeutralLostSound:string,NeutralLockedModel:string,NeutralLockedAnim:string,NeutralLockedAnimLength:float,NeutralDefenseMultiplier=0:int,NeutralCaptureMultiplier=0:int,NeutralUnoccupiedMultiplier=-1:int,NeutralCustomID=0:int,NeutralTeamMemberCaptureCustom=0:int,NeutralTeamMemberNeutralize=0:int,NeutralTeamMembmerLostCustom=0:int,NeutralTeamCaptureCustom=0:int,NeutralTeamLostCustom=0:int");
ScriptRegistrant<JMG_Utility_Control_Point_Team_Setting> JMG_Utility_Control_Point_Team_Setting_Registrant("JMG_Utility_Control_Point_Team_Setting","TeamID:int,TeamName:string,PlayerType:int,RadarBlipColor:int,PointModel:string,Animation:string,AnimationLength:float,CaptureSound:string,LostSound:string,LockedModel:string,LockedAnim:string,LockedAnimLength:float,DefenseMultiplier=1:int,CaptureMultiplier=1:int,UnoccupiedMultiplier=1:int,CustomID=0:int,TeamMemberCaptureCustom=0:int,TeamMemberNeutralize=0:int,TeamMembmerLostCustom=0:int,TeamCaptureCustom=0:int,TeamLostCustom=0:int");
ScriptRegistrant<JMG_Utility_Control_Point> JMG_Utility_Control_Point_Registrant("JMG_Utility_Control_Point","ControlPointName:string,ControllingTeamID:int,MaxCapturePoints=100:int,ControlDistance=15.0:float,ControlHeight[Min|Max]=0.0 0.0 0.0:Vector3,ZoneID=0:int,CaptureScore=1000.0:float,NeutralizeScore=1000.0:float,ControlGroupID=-1:int,StartsLocked=0:int,LockCustom=0:int");
ScriptRegistrant<JMG_Utility_Control_Point_Setting_Override> JMG_Utility_Control_Point_Setting_Override_Registrant("JMG_Utility_Control_Point_Setting_Override","TeamID:int,ID:int,CaptureCustom:int,LostCustom:int,PointModelOverride:string,AnimationOverride:string,AnimationLength:float,LockedPointModelOverride:string,LockedAnimationOverride:string,LockedAnimationLength:float");
ScriptRegistrant<JMG_Utility_Control_Point_Team_Member> JMG_Utility_Control_Point_Team_Member_Registrant("JMG_Utility_Control_Point_Team_Member","TeamID:int,Multiplier=1:int");
ScriptRegistrant<JMG_Utility_Control_Point_Assault_Mode> JMG_Utility_Control_Point_Assault_Mode_Registrant("JMG_Utility_Control_Point_Assault_Mode","TeamID:int,ID:int,AdvanceCustom:int,PushBackCustom:int,ControlAllCustom:int,LostAllCustom:int");
ScriptRegistrant<JMG_Utility_Control_Point_Wander_Point> JMG_Utility_Control_Point_Wander_Point_Registrant("JMG_Utility_Control_Point_Wander_Point","ControlPointID:int,TeamID:int,SpawnableGroupID=-999:int,UnspawnableGroupId=-999:int");
ScriptRegistrant<JMG_Utility_Control_Point_Select_Spawn_System> JMG_Utility_Control_Point_Select_Spawn_System_Registrant("JMG_Utility_Control_Point_Select_Spawn_System","SpawnPreset:string,TeamID:int,SpawnCustom:int,GroupChangeCustom:int,UngroupedChangeCustom:int,SafeTeleportDistance=1.5:float,SpawnTime=10:int,MaxWanderRange=60.0:float,StringID=12628:int");
ScriptRegistrant<JMG_Utility_Custom_And_Param_Send_Custom> JMG_Utility_Custom_And_Param_Send_Custom_Registrant("JMG_Utility_Custom_And_Param_Send_Custom","Custom:int,Param:int,ID=0:int,SendCustom:int,SendParam:int,Delay=0.0:float,RandomDelay=0.0:float,RandomChance=0.0:float");
ScriptRegistrant<JMG_Utility_Create_Move_To_Nearest_Pathfind> JMG_Utility_Create_Move_To_Nearest_Pathfind_Registrant("JMG_Utility_Create_Move_To_Nearest_Pathfind","");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_GameObject> JMG_Utility_Objective_System_Objective_GameObject_Registrant("JMG_Utility_Objective_System_Objective_GameObject","ObjectivePriority=-2:int,ObjectiveID=-2:int,Preset:string,Attach=1:int");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_GameObject_Tracker> JMG_Utility_Objective_System_Objective_GameObject_Tracker_Registrant("JMG_Utility_Objective_System_Objective_GameObject_Tracker","GameObjectID:int,MarkerID:int");
ScriptRegistrant<JMG_Utility_Attach_Script_To_Object_With_Weapon> JMG_Utility_Attach_Script_To_Object_With_Weapon_Registrant("JMG_Utility_Attach_Script_To_Object_With_Weapon","WeaponName:string,Script:string,Params:string,Delim=@:string,Rate=0.1:float,PlayerType=2:int");
ScriptRegistrant<JMG_Utility_Drop_Object_On_Death_And_Attach_Script> JMG_Utility_Drop_Object_On_Death_And_Attach_Script_Registrant("JMG_Utility_Drop_Object_On_Death_And_Attach_Script","Drop_Object:string,Drop_Height=0.25:float,Probability=100.0:float,Script:string,Params:string,Delim=@:string");
ScriptRegistrant<JMG_SinglePlayer_M04_Modifier> JMG_SinglePlayer_M04_Modifier_Registrant("JMG_SinglePlayer_M04_Modifier","");
ScriptRegistrant<JMG_Utility_Custom_Send_Delayed_Ordered_Customs> JMG_Utility_Custom_Send_Delayed_Ordered_Customs_Registrant("JMG_Utility_Custom_Send_Delayed_Ordered_Customs","ID:int,Custom0:int,Time0:float,Custom1:int,Time1:float,Custom2:int,Time2:float,Custom3:int,Time3:float,Custom4:int,Time4:float,Custom5:int,Time5:float,Custom6:int,Time6:float,Custom7:int,Time7:float,Custom8:int,Time8:float,Custom9:int,Time9:float");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_If_Model> JMG_Utility_Custom_Send_Custom_If_Model_Registrant("JMG_Utility_Custom_Send_Custom_If_Model","Model:string,Custom:int,ID=0:int,SendCustom:int,Param:int,Delay=0.0:float,RandomDelay=0.0:float,RandomChance=0.0:float");
ScriptRegistrant<JMG_Utility_Poke_Grant_Weapon_Destroy_Self> JMG_Utility_Poke_Grant_Weapon_Destroy_Self_Registrant("JMG_Utility_Poke_Grant_Weapon_Destroy_Self","Weapon:string,Rounds:int,Backpack:int");
ScriptRegistrant<JMG_Utility_Killed_Drop_Object_If_Weapon_Present> JMG_Utility_Killed_Drop_Object_If_Weapon_Present_Registrant("JMG_Utility_Killed_Drop_Object_If_Weapon_Present","Weapon:string,DropPreset:string,Height=0.25:float");
ScriptRegistrant<JMG_Utility_Poke_Send_Custom_If_Has_Weapon> JMG_Utility_Poke_Send_Custom_If_Has_Weapon_Registrant("JMG_Utility_Poke_Send_Custom_If_Has_Weapon","Weapon:string,ID:int,Custom:int,Param:int,Delay:float,RemoveWeapon:int,PlayerType=2:int,TriggerOnce=1:int,MustBeHeld=0:int");
ScriptRegistrant<JMG_Utility_Send_Custom_If_Weapon_Held_In_Range> JMG_Utility_Send_Custom_If_Weapon_Held_In_Range_Registrant("JMG_Utility_Send_Custom_If_Weapon_Held_In_Range","Weapon:string,Range:float,ID:int,Custom:int,Param:int,Delay:float,RemoveWeapon:int,PlayerType=2:int,TriggerOnce=1:int,MustBeHeld=1:int");
ScriptRegistrant<JMG_Utility_Custom_Refect_Custom_If_Model> JMG_Utility_Custom_Refect_Custom_If_Model_Registrant("JMG_Utility_Custom_Refect_Custom_If_Model","Model:string,SendFromSelf:int");
ScriptRegistrant<JMG_Utility_Custom_Set_Team> JMG_Utility_Custom_Set_Team_Registrant("JMG_Utility_Custom_Set_Team","Custom:int,Team:int");
ScriptRegistrant<JMG_Utility_Custom_Create_Object_At_Random_Wander_Point> JMG_Utility_Custom_Create_Object_At_Random_Wander_Point_Registrant("JMG_Utility_Custom_Create_Object_At_Random_Wander_Point","Custom:int,Preset:string,WanderingAIGroupID:int");
ScriptRegistrant<JMG_Utility_Zone_Teleport_To_Non_Visible_Wander_Point> JMG_Utility_Zone_Teleport_To_Non_Visible_Wander_Point_Registrant("JMG_Utility_Zone_Teleport_To_Non_Visible_Wander_Point","WanderingAIGroupID=-1:int,SafeTeleportDistance=1.5:float,ChangeGroupIDCustom=0:int,PlayerType=2:int,RetryOnFailure=0:int,AiOnly=0:int");
ScriptRegistrant<JMG_Utility_Zone_Teleport_To_Non_Visible_Wander_Point_Attach> JMG_Utility_Zone_Teleport_To_Non_Visible_Wander_Point_Attach_Registrant("JMG_Utility_Zone_Teleport_To_Non_Visible_Wander_Point_Attach","WanderingAIGroupID=-1:int,SafeTeleportDistance=1.5:float");
ScriptRegistrant<JMG_Utility_Security_System_Random_NumberPad_Control> JMG_Utility_Security_System_Random_NumberPad_Control_Registrant("JMG_Utility_Security_System_Random_NumberPad_Control","RandomCombinationDigits:int,ID:int,Success_Custom=0:int,Partial_Failure_Custom=0:int,Failure_Custom=0:int,Disable_On_Success=1:int,Disable_On_Failure=0:int,Starts_Enabled=1:int,Enable_Custom=0:int,Failure_Safty=1:int,Max_Failures=1:int,Sound_Name_Base:string");
ScriptRegistrant<JMG_Utility_Custom_Combination_Lock> JMG_Utility_Custom_Combination_Lock_Registrant("JMG_Utility_Custom_Combination_Lock","ID:int,Combination:string,Input_Number_Custom:int,Input_Enter_Custom:int,Input_Clear_Custom:int,Success_Custom=0:int,Partial_Failure_Custom=0:int,Failure_Custom=0:int,Disable_On_Success=1:int,Disable_On_Failure=0:int,Starts_Enabled=1:int,Enable_Custom=0:int,Failure_Safty=1:int,Max_Failures=1:int,Sound_Name_Base:string");
ScriptRegistrant<JMG_Utility_Custom_Combination_Lock_Key> JMG_Utility_Custom_Combination_Lock_Key_Registrant("JMG_Utility_Custom_Combination_Lock_Key","ID=0:int,Message:int,Param=0:int,Sound_Name=null:string");
ScriptRegistrant<JMG_Utility_Security_System_Sync_String_Random_NumberPad_Control> JMG_Utility_Security_System_Sync_String_Random_NumberPad_Control_Registrant("JMG_Utility_Security_System_Sync_String_Random_NumberPad_Control","StringID:int,Delim=@:string");
ScriptRegistrant<JMG_Utility_Created_Animate_SubObject_On_Join> JMG_Utility_Created_Animate_SubObject_On_Join_Registrant("JMG_Utility_Created_Animate_SubObject_On_Join","SubObject:string,Animation:string");
ScriptRegistrant<JMG_Utility_Custom_Play_Animation_Send_Custom_When_Complete> JMG_Utility_Custom_Play_Animation_Send_Custom_When_Complete_Registrant("JMG_Utility_Custom_Play_Animation_Send_Custom_When_Complete","ReceivedCustom:int,Animation:string,StartFrame:float,EndFrame:float,ID:int,Custom:int,Param:int,Delay:float");
ScriptRegistrant<JMG_Utility_Created_Play_Locked_Infantry_Animation> JMG_Utility_Created_Play_Locked_Infantry_Animation_Registrant("JMG_Utility_Created_Play_Locked_Infantry_Animation","Animation:string");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_To_Preset> JMG_Utility_Custom_Send_Custom_To_Preset_Registrant("JMG_Utility_Custom_Send_Custom_To_Preset","ReceivedCustom:int,Preset:string,Custom:int,Param:int,Delay:float");
ScriptRegistrant<JMG_Utility_Custom_Talk> JMG_Utility_Custom_Talk_Registrant("JMG_Utility_Custom_Talk","Custom:int,StringID:int");
ScriptRegistrant<JMG_Utility_Zone_Send_Custom_Enter_Preset> JMG_Utility_Zone_Send_Custom_Enter_Preset_Registrant("JMG_Utility_Zone_Send_Custom_Enter_Preset","Preset:string,ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,PlayerType=2:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Global_Armor_Controller> JMG_Utility_Global_Armor_Controller_Registrant("JMG_Utility_Global_Armor_Controller","DefaultMaxArmor:float,DefaultArmor:float");
ScriptRegistrant<JMG_Utility_Global_Armor_Custom> JMG_Utility_Global_Armor_Custom_Registrant("JMG_Utility_Global_Armor_Custom","Custom:int,MaxArmor:float,Armor:float,UpdateAllObjects=1:int");
ScriptRegistrant<JMG_Utility_Global_Armor_Object> JMG_Utility_Global_Armor_Object_Registrant("JMG_Utility_Global_Armor_Object","");
ScriptRegistrant<JMG_Utility_Global_Health_Controller> JMG_Utility_Global_Health_Controller_Registrant("JMG_Utility_Global_Health_Controller","DefaultMaxHealth:float,DefaultHealth:float");
ScriptRegistrant<JMG_Utility_Global_Health_Custom> JMG_Utility_Global_Health_Custom_Registrant("JMG_Utility_Global_Health_Custom","Custom:int,MaxHealth:float,Health:float,UpdateAllObjects=1:int");
ScriptRegistrant<JMG_Utility_Global_Health_Object> JMG_Utility_Global_Health_Object_Registrant("JMG_Utility_Global_Health_Object","");
ScriptRegistrant<JMG_Utility_Custom_Create_Random_Explosions> JMG_Utility_Custom_Create_Random_Explosions_Registrant("JMG_Utility_Custom_Create_Random_Explosions","Custom:int,ExplosionPreset:string,Count:int,MaxDistance:float,KillerID:int");
ScriptRegistrant<JMG_Utility_Custom_Enable_Spawners_Incrementally_In_Range> JMG_Utility_Custom_Enable_Incrementally_Spawners_In_Range_Registrant("JMG_Utility_Custom_Enable_Spawners_Incrementally_In_Range","StartID:int,EndID:int,Custom:int,Enable=1:int");
ScriptRegistrant<JMG_Utility_Control_Point_Team_Target_Wander_Point> JMG_Utility_Control_Point_Team_Target_Wander_Point_Registrant("JMG_Utility_Control_Point_Team_Target_Wander_Point","ControlPointID:int,TeamID:int,TargetGroupID:int,NonTargetGroupId:int");
ScriptRegistrant<JMG_Utility_AI_Control_Point> JMG_Utility_AI_Control_Point_Registrant("JMG_Utility_AI_Control_Point","CaptureCpGroupId:int,CaptureCpChance=0.33:float,CaptureSpeed=1.0:float,CaptureDistance=5.0:float,DefendCpGroupId:int,DefendSpeed=1.0:float,DefendDistance=1.0:float,CloseDefendDistance=25.0:float,ChooseFarDefendChance=0.1:float,AttackSpeed=1.0:float,AttackDistance=-1.0:float,RandomAttackDistance=0.0:float,AttackDistractFromCaptureChance=0.25:float,ChanceToInvestigateLastSeenLocation=0.66:float,AttackCheckBlocked=1:int,CanSeeStealth=1:int,ShutdownEngineOnArrival=0:int,ChangeDefendSpeedCustom=0:int,ChangeCaptureSpeedCustom=0:int,ChangeAttackSpeedCustom=0:int");
ScriptRegistrant<JMG_Utility_Objective_System_Set_Infantry_Attach_Bone> JMG_Utility_Objective_System_Set_Infantry_Attach_Bone_Registrant("JMG_Utility_Objective_System_Set_Infantry_Attach_Bone","InfantryAttachBone:string");
ScriptRegistrant<JMG_Utility_Security_System_Fixed_NumberPad_Control> JMG_Utility_Security_System_Fixed_NumberPad_Control_Registrant("JMG_Utility_Security_System_Fixed_NumberPad_Control","KeyCode:string,ID:int,Success_Custom=0:int,Partial_Failure_Custom=0:int,Failure_Custom=0:int,Disable_On_Success=1:int,Disable_On_Failure=0:int,Starts_Enabled=1:int,Enable_Custom=0:int,Failure_Safty=1:int,Max_Failures=1:int,Sound_Name_Base:string");
ScriptRegistrant<JMG_Utility_Custom_Teleport_Sender_Wanderpoint> JMG_Utility_Custom_Teleport_Sender_Wanderpoint_Registrant("JMG_Utility_Custom_Teleport_Sender_Wanderpoint","Custom:int,WanderingAIGroupID=-1:int,SafeTeleportDistance=1.5:float,RetryOnFailure=0:int");
ScriptRegistrant<JMG_Utility_Create_Set_Random_Model> JMG_Utility_Create_Set_Random_Model_Registrant("JMG_Utility_Create_Set_Random_Model","BaseName:string,FinalModelNumber:int");
ScriptRegistrant<JMG_Utility_Custom_Destroy_Sender> JMG_Utility_Custom_Destroy_Sender_Registrant("JMG_Utility_Custom_Destroy_Sender","Custom:int");
ScriptRegistrant<JMG_Utility_Custom_Grant_Weapon_Destroy_Sender> JMG_Utility_Custom_Grant_Weapon_Destroy_Sender_Registrant("JMG_Utility_Custom_Grant_Weapon_Destroy_Sender","Custom:int,Weapon:string,FullAmmoString=IDS_M00_AMMO_FULL:string");
ScriptRegistrant<JMG_Utility_Remove_Script_While_Has_Weapon> JMG_Utility_Remove_Script_While_Has_Weapon_Registrant("JMG_Utility_Remove_Script_While_Has_Weapon","WeaponName:string,Script:string,Params:string,Delim=@:string,Rate=0.1:float");
ScriptRegistrant<JMG_Utility_Swimming_Infantry_AI> JMG_Utility_Swimming_Infantry_AI_Registrant("JMG_Utility_Swimming_Infantry_AI","WeaponPreset=Weapon_Swimming_Animations:string,DrownTime=10.0:float,StartDrownSequence=3.0:float,GaspForBreath=SFX.SwimmingGaspForBreath:string,PantingSoundEmitterModel=s_panting:string,HeartBeatSoundEmitterModel=s_heartBeat:string,DrownDamageRate=2.5:float,CatchBreathRate=0.33:float,WaterDamageAmount=0.0:float,WaterDamageWarhead=None:string,WaterDamageDelayTime=0:int,WaterDamageDelayTimeRecover=0:int,SwimmingSkin:string,SwimmingArmor:string,SwimmingModel:string");
ScriptRegistrant<JMG_Utility_Damaged_Refund_Damage> JMG_Utility_Damaged_Refund_Damage_Registrant("JMG_Utility_Damaged_Refund_Damage","WarheadName:string");
ScriptRegistrant<JMG_Utility_Turret_Spawn_Global_Flag_Controller> JMG_Utility_Turret_Spawn_Global_Flag_Controller_Registrant("JMG_Utility_Turret_Spawn_Global_Flag_Controller","Custom:int");
ScriptRegistrant<JMG_Utility_Turret_Spawn_Global_Flag> JMG_Utility_Turret_Spawn_Global_Flag_Registrant("JMG_Utility_Turret_Spawn_Global_Flag","Turret_Preset:string,Bone_Name=Tur_Mount:string");
ScriptRegistrant<JMG_Utility_Timer_Death_If_Not_Damaged_Over_Peroid> JMG_Utility_Timer_Death_If_Not_Damaged_Over_Peroid_Registrant("JMG_Utility_Timer_Death_If_Not_Damaged_Over_Peroid","Time:int,Damage:float,Warhead:string");
ScriptRegistrant<JMG_Utility_Global_Attach_Script_On_Flag_Controller> JMG_Utility_Global_Attach_Script_On_Flag_Controller_Registrant("JMG_Utility_Global_Attach_Script_On_Flag_Controller","GlobalFlag:int");
ScriptRegistrant<JMG_Utility_Global_Attach_Script_On_Flag_Custom> JMG_Utility_Global_Attach_Script_On_Flag_Custom_Registrant("JMG_Utility_Global_Attach_Script_On_Flag_Custom","Custom:int,GlobalFlag:int");
ScriptRegistrant<JMG_Utility_Global_Attach_Script_On_Flag> JMG_Utility_Global_Attach_Script_On_Flag_Registrant("JMG_Utility_Global_Attach_Script_On_Flag","GlobalFlag:int,Script:string,Params:string,Delim=@:string");
ScriptRegistrant<JMG_Utility_Custom_Attach_Script_To_Presets> JMG_Utility_Custom_Attach_Script_To_Presets_Registrant("JMG_Utility_Custom_Attach_Script_To_Presets","Custom:int,Preset:string,Script:string,Params:string,Delim=@:string");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_On_Player_Count_Animation> JMG_Utility_Custom_Send_Custom_On_Player_Count_Animation_Registrant("JMG_Utility_Custom_Send_Custom_On_Player_Count_Animation","ControllingID:int,Animation:string,MaxFrame:float");
ScriptRegistrant<JMG_Utility_Created_Destroy_If_Not_In_Pathfind> JMG_Utility_Created_Destroy_If_Not_In_Pathfind_Registrant("JMG_Utility_Created_Destroy_If_Not_In_Pathfind","");
ScriptRegistrant<JMG_Utility_Teleport_If_Not_Moving_To_Location> JMG_Utility_Teleport_If_Not_Moving_To_Location_Registrant("JMG_Utility_Teleport_If_Not_Moving_To_Location","Time:int,Location:vector3");
ScriptRegistrant<JMG_Utility_Global_Set_Random_Model_Controller> JMG_Utility_Global_Set_Random_Model_Controller_Registrant("JMG_Utility_Global_Set_Random_Model_Controller","DefaultExtension:string");
ScriptRegistrant<JMG_Utility_Global_Set_Random_Model_Custom> JMG_Utility_Global_Set_Random_Model_Custom_Registrant("JMG_Utility_Global_Set_Random_Model_Custom","Custom:int,Extension:string");
ScriptRegistrant<JMG_Utility_Global_Set_Random_Model> JMG_Utility_Global_Set_Random_Model_Registrant("JMG_Utility_Global_Set_Random_Model","BaseName:string,FinalModelNumber:int");
ScriptRegistrant<JMG_Utility_Swimming_Infantry_Advanced_AI> JMG_Utility_Swimming_Infantry_Advanced_AI_Registrant("JMG_Utility_Swimming_Infantry_Advanced_AI","WeaponsGroupID=0:int,WeaponPreset=Weapon_Swimming_Animations:string,ForceDefinedWeapons=0:int,DefaultHoldStyle=4:int,DefaultSwimSpeed=1.0:float,DrownTime=10.0:float,StartDrownSequence=3.0:float,GaspForBreath=SFX.SwimmingGaspForBreath:string,PantingSoundEmitterModel=s_panting:string,HeartBeatSoundEmitterModel=s_heartBeat:string,DrownDamageRate=2.5:float,CatchBreathRate=0.33:float,WaterDamageAmount=0.0:float,WaterDamageDelayTime=0:int,WaterDamageDelayTimeRecover=0:int,WaterDamageWarhead=None:string,SwimmingSkin:string,SwimmingArmor:string,SwimmingModel:string,SwimmingHeightScale=999.99:float,SwimmingWidthScale=999.99:float");
ScriptRegistrant<JMG_Utility_Global_Custom_Send_Custom_Flag_Controller> JMG_Utility_Global_Custom_Send_Custom_Flag_Controller_Registrant("JMG_Utility_Global_Custom_Send_Custom_Flag_Controller","GlobalFlag:int");
ScriptRegistrant<JMG_Utility_Global_Custom_Send_Custom_Flag_Custom> JMG_Utility_Global_Custom_Send_Custom_Flag_Custom_Registrant("JMG_Utility_Global_Custom_Send_Custom_Flag_Custom","Custom:int,GlobalFlag:int");
ScriptRegistrant<JMG_Utility_Global_Custom_Send_Custom_Flag> JMG_Utility_Global_Custom_Send_Custom_Registrant("JMG_Utility_Global_Custom_Send_Custom_Flag","GlobalFlag:int,Custom:int,ID=0:int,SendCustom:int,Param:int,Delay=0.0:float,RandomDelay=0.0:float,RandomChance=0.0:float");
ScriptRegistrant<JMG_Utility_AI_Goto_Location_While_Player_Nearby> JMG_Utility_AI_Goto_Location_While_Player_Nearby_Registrant("JMG_Utility_AI_Goto_Location_While_Player_Nearby","GotoObjectId:int,GotoLocation:Vector3,MaxPlayerDistance:float,MinAttackRange=0.0:float,ForceFire=0:int,VTOLHover=0.0:float,vsSoldier=1:int,vsVehicle=1:int,vsAircraft=1:int,WeaponError=-1.0:float,OverrideFireMode=0:int,OverrideSpeed=-1.0:float,PlayerType=2:int");
ScriptRegistrant<JMG_Utility_AI_Goto_Location_While_Player_Nearby_Ignored> JMG_Utility_AI_Goto_Location_While_Player_Nearby_Ignored_Registrant("JMG_Utility_AI_Goto_Location_While_Player_Nearby_Ignored","");
ScriptRegistrant<JMG_Utility_Killed_Drop_Powerup_Become_Corpse> JMG_Utility_Killed_Drop_Powerup_Become_Corpse_Registrant("JMG_Utility_Killed_Drop_Powerup_Become_Corpse","PowerupName:string,RequiredWeaponPreset:string,Chance=1.0:float");
ScriptRegistrant<JMG_Utility_Objective_System_Objectives_Complete_Send_Custom> JMG_Utility_Objective_System_Objectives_Complete_Send_Custom_Registrant("JMG_Utility_Objective_System_Objectives_Complete_Send_Custom","CompletedObjectives:string,Delim=@:string,ID=0:int,SendCustom:int,Param:int,Delay=0.0:float,RandomDelay=0.0:float,Repeat=0:int");
ScriptRegistrant<JMG_Utility_Damage_Update_Animation_Frame> JMG_Utility_Damage_Update_Animation_Frame_Registrant("JMG_Utility_Damage_Update_Animation_Frame","Animation:string,MaxFrame:float,Model:string");
ScriptRegistrant<JMG_Utility_Security_System_Sync_Model_Random_NumberPad_Control> JMG_Utility_Security_System_Sync_Model_Random_NumberPad_Control_Registrant("JMG_Utility_Security_System_Sync_Model_Random_NumberPad_Control","KeypadID:int,BaseModel:string,Index:int");
ScriptRegistrant<JMG_Utility_AI_Goto_Target_Script> JMG_Utility_AI_Goto_Target_Script_Registrant("JMG_Utility_AI_Goto_Target_Script","HuntSearchDistance=-1.0:float,HuntSpeed=1.0:float,HuntArriveDistance=1.0:float,RandomHuntArriveDistance=0.0:float,HuntStealth=0:int,AttackSpeed=1.0:float,AttackDistance=-1.0:float,RandomAttackDistance=0.0:float,ReturnHome=1:int,ReturnHomeSpeed=1.0:float,WanderingAIGroupID=-1:int,WanderSpeed=1.0:float,CanSeeStealth=1:int,ShutdownEngineOnArrival=0:int,AttackCheckBlocked=1:int,MaxSightRangeFromHome=0.0:float,WanderDistanceOverride=0.0:float,ChangeWanderGroupCustom=0:int,ChangeWanderSpeedCustom=0:int,ChangeHuntDistanceCustom=0:int,ChangeHuntSpeedCustom=0:int,ChangeReturnHomeSpeedCustom=0:int,ChangeMaxSightFromHomeLocationCustom=0:int,ChangeAttackSpeedCustom=0:int,ChanceToInvestigateLastSeenLocation=0.66:float");
ScriptRegistrant<JMG_Utility_AI_Goto_Target_Script_Ignore_Object> JMG_Utility_AI_Goto_Target_Script_Ignore_Object_Registrant("JMG_Utility_AI_Goto_Target_Script_Ignore_Object","");
ScriptRegistrant<JMG_Utility_AI_Goto_Target_Script_Target> JMG_Utility_AI_Goto_Target_Script_Target_Registrant("JMG_Utility_AI_Goto_Target_Script_Target","");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_If_Script_Count> JMG_Utility_Custom_Send_Custom_If_Script_Count_Registrant("JMG_Utility_Custom_Send_Custom_If_Script_Count","Custom:int,Script:string,MaxCount:int,PlayerAddMaxCount:float,ID=0:int,SendCustom:int,Param:int,Delay=0.0:float,RandomDelay=0.0:float,RandomChance=0.0:float");
ScriptRegistrant<JMG_Utility_Created_Trigger_Create_Vehicle> JMG_Utility_Created_Trigger_Create_Vehicle_Registrant("JMG_Utility_Created_Trigger_Create_Vehicle","PresetName:string,Delay:float,OwnerID:int,TeamID:int");
ScriptRegistrant<JMG_Utility_Custom_Remove_And_Attach_Script> JMG_Utility_Custom_Remove_And_Attach_Script_Registrant("JMG_Utility_Custom_Remove_And_Attach_Script","Custom:int,RemoveScript:string,AttachScript:string,Params:string,Delim:string,Repeat:int,RequiresRemoveScript:int");
ScriptRegistrant<JMG_Utility_AI_Guardian_Aircraft_Ignored> JMG_Utility_AI_Guardian_Aircraft_Ignored_Registrant("JMG_Utility_AI_Guardian_Aircraft_Ignored","");
ScriptRegistrant<JMG_Utility_AI_Guardian_Infantry_Ignored> JMG_Utility_AI_Guardian_Infantry_Ignored_Registrant("JMG_Utility_AI_Guardian_Infantry_Ignored","");
ScriptRegistrant<JMG_Utility_AI_Guardian_Vehicle_Ignored> JMG_Utility_AI_Guardian_Vehicle_Ignored_Registrant("JMG_Utility_AI_Guardian_Vehicle_Ignored","");
ScriptRegistrant<JMG_Utility_AI_Guardian_Generic_Ignored> JMG_Utility_AI_Guardian_Generic_Ignored_Registrant("JMG_Utility_AI_Guardian_Generic_Ignored","");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_Marker> JMG_Utility_Objective_System_Objective_Marker_Registrant("JMG_Utility_Objective_System_Objective_Marker","ObjectiveID:int");
ScriptRegistrant<JMG_Utility_Remove_Script_If_Doesnt_Have_Weapon> JMG_Utility_Remove_Script_If_Doesnt_Have_Weapon_Registrant("JMG_Utility_Remove_Script_If_Doesnt_Have_Weapon","WeaponName:string,Script:string,Params:string,Delim=@:string,Rate=0.1:float");
ScriptRegistrant<JMG_Utility_Spawn_With_Last_Selected_Gun_Ignore> JMG_Utility_Spawn_With_Last_Selected_Gun_Ignore_Registrant("JMG_Utility_Spawn_With_Last_Selected_Gun_Ignore","WeaponPreset:string");
ScriptRegistrant<JMG_Utility_Objective_System_Override_Visible_Settings> JMG_Utility_Objective_System_Override_Visible_Settings_Registrant("JMG_Utility_Objective_System_Override_Visible_Settings","ObjectiveID:int,MarkerModel:string,MarkerColor=-1:int,AttachBone:string,OverrideTextColor=0:int,TextColor:vector3,OverrideHudColor=0:int,HudColor:vector3");
ScriptRegistrant<JMG_Utility_Custom_Create_Object_At_Bone> JMG_Utility_Custom_Create_Object_At_Bone_Registrant("JMG_Utility_Custom_Create_Object_At_Bone","Custom:int,Preset:string,Bone:string,MaxDistance=1.5:float,Repeat=0:int");
ScriptRegistrant<JMG_Utility_Killed_Send_Custom_When_Killed_By_Nothing> JMG_Utility_Killed_Send_Custom_When_Killed_By_Nothing_Registrant("JMG_Utility_Killed_Send_Custom_When_Killed_By_Nothing","ID:int,Custom:int,Param:int,Delay:float");
ScriptRegistrant<JMG_Utility_Custom_Drop_Corpse> JMG_Utility_Custom_Drop_Corpse_Registrant("JMG_Utility_Custom_Drop_Corpse","Custom:int,PowerupName:string,Repeat=0:int");
ScriptRegistrant<JMG_Utility_Custom_Send_Shuffled_Customs> JMG_Utility_Custom_Send_Shuffled_Customs_Registrant("JMG_Utility_Custom_Send_Shuffled_Customs","Custom:int,ID=0:int,SendCustoms:string,Delim=@:string,Param:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Custom_Send_Shuffled_Customs_And_Ids> JMG_Utility_Custom_Send_Shuffled_Customs_And_Ids_Registrant("JMG_Utility_Custom_Send_Shuffled_Customs_And_Ids","Custom:int,IDs=0:string,SendCustoms:string,Delim=@:string,Param:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Custom_Force_All_Players_Into_Vehicle> JMG_Utility_Custom_Force_All_Players_Into_Vehicle_Registrant("JMG_Utility_Custom_Force_All_Players_Into_Vehicle","Custom:int,ForceOutOfOtherVehicles=1:int");
ScriptRegistrant<JMG_Utility_Custom_Force_Occupants_Out_And_Lock> JMG_Utility_Custom_Force_Occupants_Out_And_Lock_Registrant("JMG_Utility_Custom_Force_Occupants_Out_And_Lock","Custom:int");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_If_Occupant_LorE> JMG_Utility_Custom_Send_Custom_If_Occupant_LorE_Registrant("JMG_Utility_Custom_Send_Custom_If_Occupant_LorE","Custom:int,OccupantCount:int,ID=0:int,SendCustom:int,Param:int,Delay=0.0:float,Repeat=0:int");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_If_Occupant_GorE> JMG_Utility_Custom_Send_Custom_If_Occupant_GorE_Registrant("JMG_Utility_Custom_Send_Custom_If_Occupant_GorE","Custom:int,OccupantCount:int,ID=0:int,SendCustom:int,Param:int,Delay=0.0:float,Repeat=0:int");
ScriptRegistrant<JMG_Utility_Basic_Spawner_Wander_Point> JMG_Utility_Basic_Spawner_Wander_Point_Registrant("JMG_Utility_Basic_Spawner_Wander_Point","Spawn_Preset:string,WanderingPointGroupID:int,SafeTeleportDistance=1.5:float,Spawn_Rate:float,Random_Spawn_Rate:float,Spawn_At_A_Time:int,Max_Total_Spawned=-1:int,Change_Spawn_Cap_Custom=-1:int,Initial_Spawn=-1:int,Spawned_Object_Script_ID=-1:int,Spawn_Group_ID=0:int,Starts_Disabled=0:int,Enable_Disable_Custom=0:int,Attach_Scripts_Group_ID=-1:int,PlayersAddToSpawnAtATime=0.0:float");
ScriptRegistrant<JMG_Utility_Global_CSC_With_Global_Param_Controller> JMG_Utility_Global_CSC_With_Global_Param_Controller_Registrant("JMG_Utility_Global_CSC_With_Global_Param_Controller","GlobalParam:int");
ScriptRegistrant<JMG_Utility_Global_CSC_With_Global_Param_Update_Custom> JMG_Utility_Global_CSC_With_Global_Param_Update_Custom_Registrant("JMG_Utility_Global_CSC_With_Global_Param_Update_Custom","Custom:int,GlobalParam:int");
ScriptRegistrant<JMG_Utility_Global_CSC_With_Global_Param> JMG_Utility_Global_CSC_With_Global_Param_Registrant("JMG_Utility_Global_CSC_With_Global_Param","Custom:int,ID=0:int,SendCustom:int,Delay=0.0:float,RandomDelay=0.0:float,RandomChance=0.0:float");
ScriptRegistrant<JMG_Utility_Global_Armor_Scaled_Controller> JMG_Utility_Global_Armor_Scaled_Controller_Registrant("JMG_Utility_Global_Armor_Scaled_Controller","Scale=100.0:float");
ScriptRegistrant<JMG_Utility_Global_Armor_Scaled_Custom> JMG_Utility_Global_Armor_Scaled_Custom_Registrant("JMG_Utility_Global_Armor_Scaled_Custom","Custom:int,Scale=100.0:float");
ScriptRegistrant<JMG_Utility_Global_Armor_Scaled_Object> JMG_Utility_Global_Armor_Scaled_Object_Registrant("JMG_Utility_Global_Armor_Scaled_Object","");
ScriptRegistrant<JMG_Utility_Global_Health_Scaled_Controller> JMG_Utility_Global_Health_Scaled_Controller_Registrant("JMG_Utility_Global_Health_Scaled_Controller","Scale=100.0:float");
ScriptRegistrant<JMG_Utility_Global_Health_Scaled_Custom> JMG_Utility_Global_Health_Scaled_Custom_Registrant("JMG_Utility_Global_Health_Scaled_Custom","Custom:int,Scale=100.0:float");
ScriptRegistrant<JMG_Utility_Global_Health_Scaled_Object> JMG_Utility_Global_Health_Scaled_Object_Registrant("JMG_Utility_Global_Health_Scaled_Object","");
ScriptRegistrant<JMG_Utility_Global_Infantry_Speed_Controller> JMG_Utility_Global_Infantry_Speed_Controller_Registrant("JMG_Utility_Global_Infantry_Speed_Controller","Speed=6.0:float");
ScriptRegistrant<JMG_Utility_Global_Infantry_Speed_Custom> JMG_Utility_Global_Infantry_Speed_Custom_Registrant("JMG_Utility_Global_Infantry_Speed_Custom","Custom:int,Speed=6.0:float");
ScriptRegistrant<JMG_Utility_Global_Infantry_Speed_Object> JMG_Utility_Global_Infantry_Speed_Object_Registrant("JMG_Utility_Global_Infantry_Speed_Object","");
ScriptRegistrant<JMG_Utility_AI_Control_Point_Ignore_Object> JMG_Utility_AI_Control_Point_Ignore_Object_Registrant("JMG_Utility_AI_Control_Point_Ignore_Object","");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_No_Presets_Outside_Of_Vehicle> JMG_Utility_Custom_Send_Custom_No_Presets_Outside_Of_Vehicle_Registrant("JMG_Utility_Custom_Send_Custom_No_Presets_Outside_Of_Vehicle","Custom:int,Preset:string,ID=0:int,SendCustom:int,Param:int,Delay=0.0:float,Repeat=0:int");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_No_Presets_Inside_Of_Vehicle> JMG_Utility_Custom_Send_Custom_No_Presets_Inside_Of_Vehicle_Registrant("JMG_Utility_Custom_Send_Custom_No_Presets_Inside_Of_Vehicle","Custom:int,Preset:string,ID=0:int,SendCustom:int,Param:int,Delay=0.0:float,Repeat=0:int");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_From_All_Players> JMG_Utility_Custom_Send_Custom_From_All_Players_Registrant("JMG_Utility_Custom_Send_Custom_From_All_Players","Custom:int,ID=0:int,SendCustom:int,Param:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Killed_By_Player_Send_Custom_From_Player> JMG_Utility_Killed_By_Player_Send_Custom_From_Player_Registrant("JMG_Utility_Killed_By_Player_Send_Custom_From_Player","ID:int,Custom:int,Param:int,Delay:float");
ScriptRegistrant<JMG_Utility_Sync_Fog_Controller> JMG_Utility_Sync_Fog_Controller_Registrant("JMG_Utility_Sync_Fog_Controller","MinRange:float,MaxRange:float");
ScriptRegistrant<JMG_Utility_Sync_Fog_Custom_Update> JMG_Utility_Sync_Fog_Custom_Update_Registrant("JMG_Utility_Sync_Fog_Custom_Update","Custom:int,MinRange:float,MaxRange:float,Transition:float");
ScriptRegistrant<JMG_Utility_Sync_Animation_On_Join> JMG_Utility_Sync_Animation_On_Join_Registrant("JMG_Utility_Sync_Animation_On_Join","");
ScriptRegistrant<JMG_Utility_Set_Animation_Frame_To_Match_Ammo> JMG_Utility_Set_Animation_Frame_To_Match_Ammo_Registrant("JMG_Utility_Set_Animation_Frame_To_Match_Ammo","Animation:string");
ScriptRegistrant<JMG_Utility_Poke_Grant_Weapon> JMG_Utility_Poke_Grant_Weapon_Registrant("JMG_Utility_Poke_Grant_Weapon","Weapon:string,Rounds:int,Backpack:int");
ScriptRegistrant<JMG_Utility_Zone_Create_Object_While_Occupied> JMG_Utility_Zone_Create_Object_While_Occupied_Registrant("JMG_Utility_Zone_Create_Object_While_Occupied","Preset:string,Location:Vector3,Facing:float,EnableCustom:int,ReCreateOnDeath:int");
ScriptRegistrant<JMG_Utility_Zone_Create_Object_While_Occupied_Attached> JMG_Utility_Zone_Create_Object_While_Occupied_Attached_Registrant("JMG_Utility_Zone_Create_Object_While_Occupied_Attached","ScriptZoneId:int,ScriptId:int");
ScriptRegistrant<JMG_Utility_Zone_Create_Object_While_Occupied_Object_Attached> JMG_Utility_Zone_Create_Object_While_Occupied_Object_Attached_Registrant("JMG_Utility_Zone_Create_Object_While_Occupied_Object_Attached","ScriptZoneId:int,ScriptId:int");
ScriptRegistrant<JMG_Utility_Animate_While_Moving_Idle_Or_Dead> JMG_Utility_Animate_While_Moving_Idle_Or_Dead_Registrant("JMG_Utility_Animate_While_Moving_Idle_Or_Dead","SubObject:string,IdleAnimation:string,MoveAnimation:string,DeathAnimation:string,DeathFrame:float");
ScriptRegistrant<JMG_Utility_Send_Custom_When_HP_Crosses_Threshold> JMG_Utility_Send_Custom_When_HP_Crosses_Threshold_Registrant("JMG_Utility_Send_Custom_When_HP_Crosses_Threshold","TargetRatio:float,ID:int,AboveCustom:int,AboveParam:int,BelowCustom:int,BelowParam:int");
ScriptRegistrant<JMG_Utility_Change_SkinType_To_Blamo_Until_Damaged> JMG_Utility_Change_SkinType_To_Blamo_Until_Damaged_Registrant("JMG_Utility_Change_SkinType_To_Blamo_Until_Damaged","MinHealthRatio=1.0:float");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Moved_Distance_From_Spawn> JMG_Utility_Send_Custom_When_Moved_Distance_From_Spawn_Registrant("JMG_Utility_Send_Custom_When_Moved_Distance_From_Spawn","Distance:float,ID:int,Custom:int,Param:int,StartDelay=0.0:float,RequireInPathfind=0:int");
