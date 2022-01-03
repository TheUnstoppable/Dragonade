
#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "MoveablePhysClass.h"
#include "GameObjManager.h"

#include "jmgUtility.h"
#include "JmgDeathMatch.h"

void JMG_DeathMatch_Game_Control::Created(GameObject *obj)
{
	Set_Nod_Soldier_Name("DeathMatch_Spawn_Soldier");
	Set_GDI_Soldier_Name("DeathMatch_Spawn_Soldier");
	Commands->Start_Timer(obj,this,1.0f,1);
	Commands->Start_Timer(obj,this,60.0f,2);
	Commands->Start_Timer(obj,this,120.0f,3);
	Commands->Start_Timer(obj,this,240.0f,4);
	Commands->Start_Timer(obj,this,45.0f,5);
	Commands->Start_Timer(obj,this,90.0f,6);
	Commands->Start_Timer(obj,this,135.0f,7);
}
void JMG_DeathMatch_Game_Control::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		RP2DeathMatchSpawnControl.update();
		if (The_Game()->Is_Game_Over())
		{
			RP2DeathMatchSpawnControl.emptyList();
			return;
		}
		Commands->Start_Timer(obj,this,0.25f,1);
	}
	if (2 == number)
		EnableSpawners("Tier_1_Weapon_Spawner");
	if (3 == number)
		EnableSpawners("Tier_2_Weapon_Spawner");
	if (4 == number)
		EnableSpawners("Tier_3_Weapon_Spawner");
	if (5 == number)
		EnableSpawners("Tier_1_Item_Spawner");
	if (6 == number)
		EnableSpawners("Tier_2_Item_Spawner");
	if (7 == number)
		EnableSpawners("Tier_3_Item_Spawner");
}
void JMG_DeathMatch_Game_Control::EnableSpawners(const char *name)
{
	for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
	{
		ScriptableGameObj *o = 0;
		if (current->Data())
		{
			o = current->Data()->As_ScriptableGameObj();
		}
		if (o && !_stricmp(Commands->Get_Preset_Name(o),name))
			Commands->Enable_Spawner(Commands->Get_ID(o),true);
	}
}
void JMG_DeathMatch_Spawn_Character::Created(GameObject *obj)
{
	if (RP2DeathMatchSpawnControl.isEmpty())
		Commands->Start_Timer(obj,this,0.25f,1);
	else
		Timer_Expired(obj,1);
}
void JMG_DeathMatch_Spawn_Character::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (RP2DeathMatchSpawnControl.isEmpty())
		{
			Commands->Start_Timer(obj,this,0.25f,1);
			return;
		}
		RP2DeathMatchSpawnSystem::RP2DeathMatchSpawnNode *spawnPoint = RP2DeathMatchSpawnControl.findSpawnPoint(JmgUtility::JMG_Get_Player_ID(obj));
		if (!spawnPoint)
		{
			Commands->Start_Timer(obj,this,0.25f,1);
			return;
		}
		Toggle_Fly_Mode(obj);
		Commands->Set_Player_Type(obj,-1);
		if (spawnPoint->obj)
		{
			float facing = Commands->Get_Facing(spawnPoint->obj);
			Vector3 pos = spawnPoint->pos;
			Commands->Set_Position(obj,pos);
			pos.X += 20.0f*(cos(facing*PI180));
			pos.Y += 20.0f*(sin(facing*PI180));
			pos.Z += 2.0f;
			Force_Camera_Look_Player(obj,pos);
			Commands->Send_Custom_Event(spawnPoint->obj,obj,FORCELOOKATLOCATIONCUSTOM,0,0.01f);
		}
		Vector3 pos = Commands->Get_Position(obj);
		MoveablePhysClass *mphys = obj->As_PhysicalGameObj() ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
		if (mphys && !mphys->Can_Teleport(Matrix3D(pos)))
		{
			mphys->Find_Teleport_Location(pos,2.5f,&pos);
			Commands->Set_Position(obj,pos);
		}
		Commands->Attach_Script(obj,"JMG_DeathMatch_Player","");
		Toggle_Fly_Mode(obj);
	}
}
void JMG_DeathMatch_Player::Created(GameObject *obj)
{
	playerId = JmgUtility::JMG_Get_Player_ID(obj);
}
void JMG_DeathMatch_Player::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	if (playerId && Commands->Get_Preset_ID(seen) == 82080520)
		RP2DeathMatchSpawnControl.seePoint(Commands->Get_ID(seen));
}
void JMG_DeathMatch_Player::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == FORCELOOKATLOCATIONCUSTOM)
	{
		if (!sender)
			return;
		float facing = Commands->Get_Facing(sender);
		Vector3 pos = Commands->Get_Position(obj);
		pos.X += 20.0f*(cos(facing*PI180));
		pos.Y += 20.0f*(sin(facing*PI180));
		pos.Z += 2.0f;
		Force_Camera_Look_Player(obj,pos);
	}
}
void JMG_DeathMatch_Random_Spawn_Point::Created(GameObject *obj)
{
	Commands->Set_Player_Type(obj,-3);
	Commands->Set_Model(obj,"HumanMarkerNC");
	myNode = RP2DeathMatchSpawnControl.AddNode(obj);
	Commands->Enable_Enemy_Seen(obj,true);
}
void JMG_DeathMatch_Random_Spawn_Point::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	if (myNode && Commands->Is_A_Star(seen))
		myNode->lastSeen = 0;
}
void JMG_DeathMatch_Random_Spawn_Point::Destroyed(GameObject *obj)
{
	myNode = NULL;
	RP2DeathMatchSpawnControl -= obj;
}
void JMG_DeathMatch_Teleport_Zone_Script::Created(GameObject *obj)
{
	if (Get_Int_Parameter("Object_ID"))
	{
		GameObject *TeleObject = Commands->Find_Object(Get_Int_Parameter("Object_ID"));
		if (TeleObject)
		{
			Teleport_Pos = Commands->Get_Position(TeleObject);
			Commands->Start_Timer(obj,this,5.0f,5653912); 
			return;
		}
	}
	Teleport_Pos = Get_Vector3_Parameter("TeleportLocation");
}
void JMG_DeathMatch_Teleport_Zone_Script::Timer_Expired(GameObject *obj,int number)
{
	if (number == 5653912)
	{
		GameObject *TeleObject = Commands->Find_Object(Get_Int_Parameter("Object_ID"));
		if (TeleObject)
			Commands->Destroy_Object(TeleObject);
	}
}
void JMG_DeathMatch_Teleport_Zone_Script::Entered(GameObject *obj,GameObject *enter)//For Doors
{
	if (enter->As_SoldierGameObj() && Commands->Is_A_Star(enter))
	{
		char params[1024];
		sprintf(params,"%.2f %.2f %.2f,%f,%f,%s,%s,%s,%s,%s",Teleport_Pos.X,Teleport_Pos.Y,Teleport_Pos.Z,Get_Float_Parameter("Enter_Rotation"),Get_Float_Parameter("Exit_Rotation"),Get_Parameter("Enter_Animation"),Get_Parameter("Exit_Animation"),Get_Parameter("Enter_Path_Animation"),Get_Parameter("Exit_Path_Animation"),Get_Parameter("Bone_Attach_Name"));
		Attach_Script_Once(enter,"JMG_DeathMatch_Teleport_Zone_Script_Attach",params);
	}
}
void JMG_DeathMatch_Teleport_Zone_Script_Attach::Created(GameObject *obj)
{
	Commands->Innate_Enable(obj);
	Vector3 pos = Commands->Get_Bone_Position(obj,"c head");
	pos.X += 20.0f*(cos(Get_Float_Parameter("Enter_Rotation")*3.14159265f/180));
	pos.Y += 20.0f*(sin(Get_Float_Parameter("Enter_Rotation")*3.14159265f/180));
	GameObject *object = Commands->Create_Object("Daves Arrow",pos);
	Set_Facing_ID = Commands->Get_ID(object);
	Commands->Set_Model(object,"null");
	pos = Commands->Get_Position(obj);
	char anim[32];
	sprintf(anim,"%s.%s",Get_Parameter("Enter_Path_Animation"),Get_Parameter("Enter_Path_Animation"));
	GameObject *object2 = Commands->Create_Object("Daves Arrow",pos);
	Commands->Set_Model(object2,Get_Parameter("Enter_Path_Animation"));
	Commands->Set_Facing(object2,Get_Float_Parameter("Enter_Rotation"));
	Commands->Attach_To_Object_Bone(obj,object2,Get_Parameter("Bone_Attach_Name"));
	if (Commands->Get_Health(obj))
	{
		Commands->Set_Animation(object2,anim,true,0,60,90.0f,0);
		Commands->Set_Animation(obj,Get_Parameter("Enter_Animation"),true,0,0,90.0f,0);
		Set_Screen_Fade_Opacity_Player(obj,1.0,0.1f);
		Set_Screen_Fade_Color_Player(obj,0.0f,0.0f,0.0f,0.1f);
	}
	Movement_Object = Commands->Get_ID(object2);
	Commands->Start_Timer(obj,this,0.1f,453453542);
	Commands->Start_Timer(obj,this,1.0f,453453543);
	Commands->Start_Timer(obj,this,1.5f,453453544);
	Commands->Start_Timer(obj,this,0.1f,453453545);
}
void JMG_DeathMatch_Teleport_Zone_Script_Attach::Timer_Expired(GameObject *obj,int number)
{
	if (number == 453453543) // 1 sec in
	{
		Vector3 pos = Get_Vector3_Parameter("ExitPosition");
		MoveablePhysClass *mphys = obj->As_PhysicalGameObj() ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
		if (mphys && !mphys->Can_Teleport(Matrix3D(pos)))
			mphys->Find_Teleport_Location(pos,1.0f,&pos);
		Commands->Set_Position(obj,pos);
		Set_Screen_Fade_Opacity_Player(obj,1.0f,0.1f);
		Commands->Destroy_Object(Commands->Find_Object(Set_Facing_ID));
		Commands->Destroy_Object(Commands->Find_Object(Movement_Object));
		pos = Commands->Get_Bone_Position(obj,"c head");			
		pos.X += 20.0f*(cos(Get_Float_Parameter("Exit_Rotation")*PI180));
		pos.Y += 20.0f*(sin(Get_Float_Parameter("Exit_Rotation")*PI180));
		GameObject *object = Commands->Create_Object("Daves Arrow",pos);
		Commands->Set_Model(object,"null");
		Set_Facing_ID = Commands->Get_ID(object);
		char anim[32];
		sprintf(anim,"%s.%s",Get_Parameter("Exit_Path_Animation"),Get_Parameter("Exit_Path_Animation"));
		GameObject *object2 = Commands->Create_Object("Daves Arrow",Commands->Get_Position(obj));
		Commands->Set_Facing(object2,Get_Float_Parameter("Exit_Rotation"));
		Commands->Set_Model(object2,Get_Parameter("Exit_Path_Animation"));
		Commands->Attach_To_Object_Bone(obj,object2,Get_Parameter("Bone_Attach_Name"));
		Movement_Object = Commands->Get_ID(object2);
		if (Commands->Get_Health(obj))
		{
			Commands->Set_Animation(object2,anim,true,0,15,90,0);
			Commands->Set_Animation(obj,Get_Parameter("Exit_Animation"),true,0,24,30.0f,0);
			Set_Screen_Fade_Color_Player(obj,0,0.0f,0.0f,0.0f);
			Set_Screen_Fade_Opacity_Player(obj,0.0,0.0);
		}
		Commands->Start_Timer(obj,this,0.05f,453453542);// Set Facing
		Commands->Start_Timer(obj,this,0.2f,453453542);// Set Facing
		Commands->Start_Timer(obj,this,0.22f,453453542);// Set Facing
	}
	if (number == 453453544) // 1.5 sec in
	{
		Commands->Destroy_Object(Commands->Find_Object(Set_Facing_ID));
		Commands->Destroy_Object(Commands->Find_Object(Movement_Object));
		Set_Facing_ID = 0;
		Commands->Innate_Disable(obj);
		if (Is_Script_Attached(obj,"JMG_DeathMatch_Teleport_Zone_Script_Attach"))
			Remove_Script(obj,"JMG_DeathMatch_Teleport_Zone_Script_Attach");
	}
	if (number == 453453542)
	{
		GameObject *object = Commands->Find_Object(Set_Facing_ID);
		if (Set_Facing_ID > 0 && object)
		{
			if (Commands->Get_Health(object) > 0.00)
			{
				Vector3 pos = Commands->Get_Position(object);
				Force_Camera_Look_Player(obj,pos);
			}
		}
	}
	if (number == 453453545)
	{
		if (Get_Vehicle(obj))
			Commands->Apply_Damage(Get_Vehicle(obj),99999.99f,"blamokiller",0);
		Commands->Start_Timer(obj,this,0.1f,453453545);
	}
}
void JMG_DeathMatch_Damage_When_Outside_Of_Range::Created(GameObject *obj)
{
	for (int x = 0;x < 128;x++)
	{
		warnTime[x] = 0;
		screenEffectOn[x] = false;
	}
	warnDistance = Get_Float_Parameter("Warn_Distance")*Get_Float_Parameter("Warn_Distance");
	damageDistance = Get_Float_Parameter("Damage_Distance")*Get_Float_Parameter("Damage_Distance");
	vehicleWarnDistance = Get_Float_Parameter("Vehicle_Warn_Distance")*Get_Float_Parameter("Vehicle_Warn_Distance");
	vehicleDamageDistance = Get_Float_Parameter("Vehicle_Damage_Distance")*Get_Float_Parameter("Vehicle_Damage_Distance");
	aircraftWarnDistance = Get_Float_Parameter("Aircraft_Warn_Distance")*Get_Float_Parameter("Aircraft_Warn_Distance");
	aircraftDamageDistance = Get_Float_Parameter("Aircraft_Damage_Distance")*Get_Float_Parameter("Aircraft_Damage_Distance");
	maxSurviveDistance = Get_Float_Parameter("Max_Survive_Distance")*Get_Float_Parameter("Max_Survive_Distance");
	myPos = Commands->Get_Position(obj);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_DeathMatch_Damage_When_Outside_Of_Range::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1)
	{
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
					float TempDistCalc = 1-(dist-maxSurviveDistance)/(warnDistance-maxSurviveDistance);
					Set_Screen_Fade_Color_Player(player,0.25f,0.0f,0.0f,0.1f);
					Set_Screen_Fade_Opacity_Player(player,TempDistCalc,0.1f);
				}
				else if (screenEffectOn[x])
				{
					screenEffectOn[x] = false;
					Set_Screen_Fade_Color_Player(player,0.0f,0.0f,0.0f,0.1f);
					Set_Screen_Fade_Opacity_Player(player,0.0f,0.1f);
				}
				if (dist >= warnDistance && !warnTime[x])
				{
					JmgUtility::DisplayChatMessage(player,255,127,0,"You are leaving the battlefield!");
					warnTime[x] = 60;
				}
				if (dist >= damageDistance)
				{
					Commands->Apply_Damage(player,0.25f,"None",player);
					Commands->Apply_Damage(player,0.25f,"SwimmingDamage",player);
				}
			}
			else
			{
				if (Is_VTOL(playerVehicle))
				{
					if (dist > aircraftWarnDistance)
					{
						screenEffectOn[x] = true;
						float TempDistCalc = 1-(dist-maxSurviveDistance)/(aircraftWarnDistance-maxSurviveDistance);
						Set_Screen_Fade_Color_Player(player,0.25f,0.0f,0.0f,0.1f);
						Set_Screen_Fade_Opacity_Player(player,TempDistCalc,0.1f);
					}
					else if (screenEffectOn[x])
					{
						screenEffectOn[x] = false;
						Set_Screen_Fade_Color_Player(player,0.0f,0.0f,0.0f,0.1f);
						Set_Screen_Fade_Opacity_Player(player,0.0f,0.1f);
					}
					if (dist >= aircraftWarnDistance && !warnTime[x])
					{
						JmgUtility::DisplayChatMessage(player,255,127,0,"You are leaving the battlefield!");
						warnTime[x] = 60;
					}
				}
				else
				{
					if (dist > vehicleWarnDistance)
					{
						screenEffectOn[x] = true;
						float TempDistCalc = 1-(dist-maxSurviveDistance)/(vehicleWarnDistance-maxSurviveDistance);
						Set_Screen_Fade_Color_Player(player,0.25f,0.0f,0.0f,0.1f);
						Set_Screen_Fade_Opacity_Player(player,TempDistCalc,0.1f);
					}
					else if (screenEffectOn[x])
					{
						screenEffectOn[x] = false;
						Set_Screen_Fade_Color_Player(player,0.0f,0.0f,0.0f,0.1f);
						Set_Screen_Fade_Opacity_Player(player,0.0f,0.1f);
					}
					if (dist >= vehicleWarnDistance && !warnTime[x])
					{
						JmgUtility::DisplayChatMessage(player,255,127,0,"You are leaving the battlefield!");
						warnTime[x] = 60;
					}
				}
				if (Is_VTOL(playerVehicle))
				{
					if (dist >= aircraftDamageDistance && Get_Vehicle_Driver(playerVehicle) == player)
						Commands->Apply_Damage(playerVehicle,1.0f,"None",player);
				}
				else if (dist >= vehicleDamageDistance && Get_Vehicle_Driver(playerVehicle) == player)
					Commands->Apply_Damage(playerVehicle,1.0f,"None",player);
			}
			if (dist >= maxSurviveDistance)
				if (playerVehicle)
					Commands->Apply_Damage(playerVehicle,99999.0f,"BlamoKiller",player);
				else
					Commands->Apply_Damage(player,99999.0f,"BlamoKiller",player);
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_DeathMatch_Random_Weapon_Spawner::Created(GameObject *obj)
{
	Commands->Set_Is_Rendered(obj,false);
	spawnLimit = Get_Int_Parameter("SpawnLimit");
	powerupId = 0;
	if (Get_Float_Parameter("InnitalSpawnDelay") >= 0)
		Commands->Send_Custom_Event(obj,obj,1000000024,0,Get_Float_Parameter("InnitalSpawnDelay"));
	else
		enabled = false;
}
void JMG_DeathMatch_Random_Weapon_Spawner::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED && Commands->Get_ID(sender) == powerupId && spawnLimit && enabled)
	{
		float respawnTime = Get_Float_Parameter("RespawnTime");
		if (Get_Float_Parameter("RespawnTimeVariation"))
			respawnTime += Commands->Get_Random(-Get_Float_Parameter("RespawnTimeVariation"),Get_Float_Parameter("RespawnTimeVariation"));
		powerupId = 0;
		Commands->Send_Custom_Event(obj,obj,1000000024,0,respawnTime);
	}
	if (message == 1000000024 && spawnLimit && enabled && !powerupId && sender == obj)
	{
		if (param && param != enableId)
			return;
		powerupId = CreatePowerup(obj);
	}
	if (Get_Int_Parameter("EnableMessage") && message == Get_Int_Parameter("EnableMessage"))
	{
		enabled = param ? true : false;
		if (enabled)
		{
			enableId = Commands->Get_Random_Int(1000000,9000000);
			Commands->Send_Custom_Event(obj,obj,1000000024,enableId,(float)param);
		}
	}
}
int JMG_DeathMatch_Random_Weapon_Spawner::CreatePowerup(GameObject *obj)
{
	if (spawnLimit > 0)
		spawnLimit--;
	GameObject *powerup = Commands->Create_Object(Get_Parameter("PowerupPreset"),Commands->Get_Position(obj));
	Commands->Set_Facing(powerup,Commands->Get_Random(-180.0f,180.0f));
	char params[512];
	sprintf(params,"%d",Commands->Get_ID(obj));
	Commands->Attach_Script(powerup,"JMG_DeathMatch_Random_Weapon_Spawner_Attached",params);
	return Commands->Get_ID(powerup);
}
void JMG_DeathMatch_Random_Weapon_Spawner_Attached::Destroyed(GameObject *obj)
{
	GameObject *controller = Commands->Find_Object(Get_Int_Parameter("ControllerId"));
	if (controller)
		Commands->Send_Custom_Event(obj,controller,CUSTOM_EVENT_POWERUP_GRANTED,CUSTOM_EVENT_POWERUP_GRANTED,0);
}
ScriptRegistrant<JMG_DeathMatch_Game_Control> JMG_DeathMatch_Game_Control_Registrant("JMG_DeathMatch_Game_Control","");
ScriptRegistrant<JMG_DeathMatch_Player> JMG_DeathMatch_Player_Registrant("JMG_DeathMatch_Player","");
ScriptRegistrant<JMG_DeathMatch_Random_Spawn_Point> JMG_DeathMatch_Random_Spawn_Point_Registrant("JMG_DeathMatch_Random_Spawn_Point","");
ScriptRegistrant<JMG_DeathMatch_Teleport_Zone_Script> JMG_DeathMatch_Teleport_Zone_Script_Registrant("JMG_DeathMatch_Teleport_Zone_Script","TeleportLocation:Vector3,Object_ID=0:int,Enter_Rotation=0.0:float,Exit_Rotation=0.0:float,Enter_Animation=s_a_human.h_a_a0b1:string,Exit_Animation=s_a_human.walkinanim:string,Enter_Path_Animation=walkin:string,Exit_Path_Animation=walkout:string,Bone_Attach_Name=humanpos:string");
ScriptRegistrant<JMG_DeathMatch_Teleport_Zone_Script_Attach> JMG_DeathMatch_Teleport_Zone_Script_Attach_Registrant("JMG_DeathMatch_Teleport_Zone_Script_Attach","ExitPosition:Vector3,Enter_Rotation=0.0:float,Exit_Rotation=0.0:float,Enter_Animation=null:string,Exit_Animation=null:string,Enter_Path_Animation=null:string,Exit_Path_Animation=null:string,Bone_Attach_Name=null:string");
ScriptRegistrant<JMG_DeathMatch_Damage_When_Outside_Of_Range> JMG_DeathMatch_Damage_When_Outside_Of_Range_Registrant("JMG_DeathMatch_Damage_When_Outside_Of_Range","Warn_Distance=0.0:float,Damage_Distance=0.0:float,Vehicle_Warn_Distance=0.0:float,Vehicle_Damage_Distance=0.0:float,Aircraft_Warn_Distance=0.0:float,Aircraft_Damage_Distance=0.0:float,Max_Survive_Distance=0.0:float");
ScriptRegistrant<JMG_DeathMatch_Random_Weapon_Spawner> JMG_DeathMatch_Random_Weapon_Spawner_Registrant("JMG_DeathMatch_Random_Weapon_Spawner","PowerupPreset:string,InnitalSpawnDelay:float,RespawnTime:float,RespawnTimeVariation:float,SpawnLimit=-1:int,EnableMessage=0:int");
ScriptRegistrant<JMG_DeathMatch_Random_Weapon_Spawner_Attached> JMG_DeathMatch_Random_Weapon_Spawner_Attached_Registrant("JMG_DeathMatch_Random_Weapon_Spawner_Attached","ControllerId:int");
