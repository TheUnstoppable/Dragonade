#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "DBIKScripts.h"
#include "engine_game.h"
#include "engine_obj.h"
#include "engine_script.h"
#include "VehicleGameObj.h"
#include "WeaponClass.h"
#include "PurchaseSettingsDefClass.h"
#include "GameObjManager.h"
#include "DB_General.h"
#include "PurchaseSettingsDefClass.h"
#include "dp88_custom_timer_defines.h"
#ifdef DRAGONADE
#include "engine_da.h"
#endif // DRAGONADE



#define PI 3.14159265

//////////DB_Shield_Generator_Vehicle/////////

void DB_Shield_Generator_Vehicle::Created(GameObject *obj)
{
	shroudID = 0;
	sizeID = 0;
	PilotID = 0;
	CoolDown = false;
}

void DB_Shield_Generator_Vehicle::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!PilotID)
		{
			PilotID = Commands->Get_ID(sender);
			char params[50];
			sprintf(params,"IKDeploy,%d,923572385",Commands->Get_ID(obj));
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

	if (type == 923572385 && !CoolDown)
	{
		if (!shroudID)
		{
			Vector3 position = Commands->Get_Bone_Position(obj, "ROOTTRANSFORM");
			GameObject *object = Commands->Create_Object(Get_Parameter("ShieldPreset"), position);
			Commands->Set_Player_Type(object,Commands->Get_Player_Type(obj));
			Commands->Attach_To_Object_Bone(object, obj, "ROOTTRANSFORM");
			shroudID = Commands->Get_ID(object);
			Commands->Create_3D_Sound_At_Bone(Get_Parameter("ShieldUp_Sound"),obj,"origin");
			CoolDown = true;
			Commands->Start_Timer(obj,this,2.5f,123);
		}
		else if (shroudID)
		{
			Commands->Destroy_Object(Commands->Find_Object(shroudID));
			shroudID = 0;
			Commands->Create_3D_Sound_At_Bone(Get_Parameter("ShieldDown_Sound"),obj,"origin");
			CoolDown = true;
			Commands->Start_Timer(obj,this,2.5f,123);
		}
	}
}

void DB_Shield_Generator_Vehicle::Timer_Expired(GameObject *obj, int number)
{
	if(number==123)
	{
		CoolDown = false;
	}
}

void DB_Shield_Generator_Vehicle::Destroyed(GameObject *obj)
{
	if (shroudID)
	{
		Commands->Destroy_Object(Commands->Find_Object(shroudID));
		shroudID = 0;
	}
	if(PilotID)
	{
		Remove_Script(Commands->Find_Object(PilotID),"Reborn_Deployable_Vehicle_Player");
	}
}

ScriptRegistrant<DB_Shield_Generator_Vehicle> DB_Shield_Generator_Vehicle_Registrant("DB_Shield_Generator_Vehicle","ShieldPreset:string,ShieldUp_Sound:string,ShieldDown_Sound:string");



	void DB_Controllable_Turret::Created(GameObject *obj) 	{
		Team = Get_Object_Type(obj);
		obj->As_VehicleGameObj()->Set_Lock_Team(Team);
		Commands->Enable_Vehicle_Transitions(obj,true);
		Commands->Start_Timer(obj,this,0.1f,11);
	}

	void DB_Controllable_Turret::Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == CUSTOM_EVENT_VEHICLE_EXITED)
		{
			Commands->Start_Timer(obj,this,0.1f,12);
			Commands->Enable_Enemy_Seen(obj,true);
			obj->As_VehicleGameObj()->Set_Lock_Team(Team);
		}

		if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
		{
			
			Commands->Enable_Enemy_Seen(obj,false);
			Commands->Action_Reset(obj,101.0f);
		}
	}

	void DB_Controllable_Turret::Timer_Expired(GameObject *obj,int Number)
	{
		if(Number == 11)
		{
			int Team2 = Get_Object_Type(obj);
			if(Team != Team)
			{
				Team=Team2;
				obj->As_VehicleGameObj()->Set_Lock_Team(Team);
			}
			Commands->Enable_Vehicle_Transitions(obj,true);
		}
		else if(Number == 12)
		{
			Set_Object_Type(obj,Team);
		}
	}

ScriptRegistrant<DB_Controllable_Turret> DB_Controllable_Turret_Registrant("DB_Controllable_Turret", "");


void DB_Guard_Bot::Created(GameObject *obj)
{
	if(obj->As_SoldierGameObj())
	{
		Attach_Script_Once(obj,"DB_Innate_Soldier","0.5,70,0,0");
		HomePosition = Commands->Get_Position(obj);
		Commands->Set_Innate_Soldier_Home_Location(obj,HomePosition,70);
		Commands->Innate_Soldier_Enable_Footsteps_Heard(obj,false);
		Commands->Innate_Soldier_Enable_Gunshot_Heard(obj,false);
		Commands->Start_Timer(obj,this,Commands->Get_Random(0,10),87);
		Set_Current_Clip_Bullets(obj,-1);
	}
}

void DB_Guard_Bot::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if(obj && damager)
	{
		if(obj->As_DamageableGameObj()->Is_Enemy(damager->As_DamageableGameObj()))
		{
			Commands->Innate_Force_State_Enemy_Seen(obj,damager);
			Send_Custom_Event_To_Objects_With_Script_Ranged(obj,"DB_Guard_Bot",923495,damager->Get_ID(),0.0f,18);
		}
	}
}

void DB_Guard_Bot::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if(type==923495)
	{
		GameObject *enemy = Commands->Find_Object(param);
		if(enemy && obj->As_DamageableGameObj()->Is_Enemy(enemy->As_DamageableGameObj()))
		{
			Commands->Innate_Force_State_Enemy_Seen(obj,enemy);
		}
	}
}

void DB_Guard_Bot::Timer_Expired(GameObject *obj,int number)
{
	if(number==87)
	{
		if(obj->As_SoldierGameObj()->Is_On_Ladder())
		{
			if(OnLadder)
			{
				Commands->Set_Animation(obj,"s_a_human.H_A_a0a0_L23",false,0,0.0f,0.0f,false);
				OnLadder=false;
			}
			else
			{
				OnLadder=true;
			}
		}
		else
		{
			OnLadder=false;
		}
		if(obj->As_SoldierGameObj()->Get_State()==SoldierAIState::AI_STATE_IDLE && Commands->Get_Distance(Commands->Get_Position(obj),HomePosition)>5)
		{
			
			ActionParamsStruct HomeAction;
			HomeAction.Set_Basic(this,70,1);
			HomeAction.Set_Movement(HomePosition,0.25,2,false);
			if(Commands->Get_Distance(Commands->Get_Position(obj),HomePosition)>50)
				HomeAction.Set_Movement(HomePosition,1,2,false);

			Commands->Action_Goto(obj,HomeAction);
		}
		if(Get_Current_Clip_Bullets(obj)!=-1)
		{
			Set_Current_Clip_Bullets(obj,-1);
		}

		Commands->Start_Timer(obj,this,10,87);
	}

}

ScriptRegistrant<DB_Guard_Bot> DB_Guard_Bot_Registrant("DB_Guard_Bot","");



void DB_Support_Bot::Created(GameObject *obj)
{
	if(!obj->As_SoldierGameObj())
	{
		Destroy_Script();
		return;
	}
	followid=0;
	distance = Get_Float_Parameter("Distance");
	speed = Get_Float_Parameter("Speed");
	Attach_Script_Once(obj,"DB_Innate_Soldier","0.5,0,0,0");
	Commands->Enable_HUD_Pokable_Indicator(obj,1);
	Commands->Start_Timer(obj,this,4.0f,1);
}

void DB_Support_Bot::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if(obj && damager)
	{
		if(obj->As_DamageableGameObj()->Is_Enemy(damager->As_DamageableGameObj()))
		{
			Commands->Innate_Force_State_Enemy_Seen(obj,damager);
		}
	}
}

void DB_Support_Bot::Poked(GameObject *obj,GameObject *poker)
{
	ActionParamsStruct actionThingy;
	actionThingy.Set_Basic(this,70,1);
	if (Commands->Get_Player_Type(obj) == Commands->Get_Player_Type(poker))
	{
		if(followid==poker->Get_ID())
		{
			followid=0;
			Commands->Action_Reset(obj,100);
			Send_Message_Player(poker,255,255,255,"This bot will no longer follow you.");
		}
		else
		{
			followid=poker->Get_ID();
			Send_Message_Player(poker,255,255,255,"This bot will now follow you.");
		}
	}
}

void DB_Support_Bot::Timer_Expired(GameObject *obj,int number)
{
	if(number == 1)
	{
		GameObject *follow = Commands->Find_Object(followid);
		if(follow)
		{
			if(obj && !obj->As_SoldierGameObj()->Is_On_Ladder() && !obj->As_SoldierGameObj()->Is_In_Elevator())
			{
				if(Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(follow)) > distance * 1.5f)
				{
					ActionParamsStruct action;
					action.Set_Basic(this,70,1);
					action.Set_Movement(follow,speed,distance);
					Commands->Action_Goto(obj,action);
				}
			}
		}
		else
			followid = 0;
		Commands->Start_Timer(obj,this,4.0f,1);
	}
}

ScriptRegistrant<DB_Support_Bot> DB_Support_Bot_Registrant("DB_Support_Bot","Distance:float,Speed:float");

void dblaney_Nod_Obelisk::Created(GameObject* ObeliskObj) {

	// Get weapon position
	//Vector3 WeaponPos = Commands->Get_Position(ObeliskObj);
	OffsetHeight = Get_Float_Parameter("OffsetHeight");
	//WeaponPos.Y += -4.5f*cos(Commands->Get_Facing(ObeliskObj)*(float)(PI / 180));
	//WeaponPos.X += 4.5f*sin(Commands->Get_Facing(ObeliskObj)*(float)(PI / 180));
	//WeaponPos.Z += OffsetHeight;

	// Create the Obelisk weapon
	StringClass Preset = Get_Parameter("ObeliskEmitterPreset");
	MaxRange = Get_Float_Parameter("MaxRange");
	ChargeTime = Get_Float_Parameter("ChargeTime");
	GroundOnly = Get_Int_Parameter("GroundOnly");
	/*
	GameObject* WeaponObj = Commands->Create_Object(Preset, WeaponPos);
	if (WeaponObj) {
		WeaponID = Commands->Get_ID(WeaponObj);
		Attach_Script_Once_V(WeaponObj, "dblaney_Nod_Obelisk_Weapon", "%f,%f,%i", MaxRange, ChargeTime, GroundOnly);
		//Commands->Attach_Script(WeaponObj, "Obelisk_Weapon_CnC", "");
	}
	*/
	PowerOn=!Is_Base_Powered(Get_Object_Type(ObeliskObj));
	Commands->Start_Timer(ObeliskObj,this,0.01f,151);

}


void dblaney_Nod_Obelisk::Killed(GameObject* ObeliskObj, GameObject* Killer) {
	// Kill the weapon too
	GameObject* WeaponObj = Commands->Find_Object(WeaponID);
	if (WeaponObj != 0) {
		Commands->Destroy_Object(WeaponObj);
	}
}

void dblaney_Nod_Obelisk::Destroyed(GameObject* ObeliskObj)
{
	GameObject* WeaponObj = Commands->Find_Object(WeaponID);
	if (WeaponObj != 0) {
		Commands->Destroy_Object(WeaponObj);
	}
}

void dblaney_Nod_Obelisk::Timer_Expired(GameObject *ObeliskObj, int number) {
	if (number == 151)
	{
		//Console_Output("Ob Timer");
		bool BasePower = Is_Base_Powered(Get_Object_Type(ObeliskObj));
		if (PowerOn != BasePower) {
			//Console_Output("Power Changed"); Debug Stuff
			if (BasePower == true) {
				PowerOn = true;
				Commands->Set_Model(ObeliskObj, "obel_buildable");
				Commands->Set_Animation_Frame(ObeliskObj,"obel_buildable.obel_buildable",2);
				GameObject* WeaponObj = Commands->Find_Object(WeaponID);
				if (!WeaponObj) {
					// Get weapon position
					Vector3 WeaponPos = Commands->Get_Position(ObeliskObj);
					WeaponPos.Y += -4.5f*cos(Commands->Get_Facing(ObeliskObj)*(float)(PI / 180));
					WeaponPos.X += 4.5f*sin(Commands->Get_Facing(ObeliskObj)*(float)(PI / 180));
					WeaponPos.Z += OffsetHeight;

					// Create the Obelisk weapon
					StringClass Preset = Get_Parameter("ObeliskEmitterPreset");
					GameObject* WeaponObj = Commands->Create_Object(Preset, WeaponPos);

					if (WeaponObj) {
						WeaponID = Commands->Get_ID(WeaponObj);
						Attach_Script_Once_V(WeaponObj, "dblaney_Nod_Obelisk_Weapon", "%f,%f,%i", MaxRange, ChargeTime, GroundOnly);
						//Commands->Attach_Script(WeaponObj, "Obelisk_Weapon_CnC", "");
					}
				}
			}
			else {
				// Kill the weapon object
				GameObject* WeaponObj = Commands->Find_Object(WeaponID);
				PowerOn = false;
				Commands->Set_Animation(ObeliskObj,"obel_buildable.obel_buildable",false,0,0,0,0);
				if (WeaponObj) {
					Commands->Destroy_Object(WeaponObj);
				}
			}


		}
		Commands->Start_Timer(ObeliskObj, this, 1.0, 151);
	}
}

ScriptRegistrant<dblaney_Nod_Obelisk> dblaney_Nod_Obelisk_Registrant("dblaney_Nod_Obelisk", "ObeliskEmitterPreset=Nod_Obelisk:string,MaxRange=150:float,ChargeTime=2:float,GroundOnly=0:int,OffsetHeight=22:float");

void dblaney_Nod_Obelisk_Structure::Created(GameObject* ObeliskObj) {

	// Get weapon position
	//Vector3 WeaponPos = Commands->Get_Position(ObeliskObj);
	OffsetHeight = Get_Float_Parameter("OffsetHeight");
	//WeaponPos.Y += -4.5f*cos(Commands->Get_Facing(ObeliskObj)*(float)(PI / 180));
	//WeaponPos.X += 4.5f*sin(Commands->Get_Facing(ObeliskObj)*(float)(PI / 180));
	//WeaponPos.Z += OffsetHeight;

	// Create the Obelisk weapon
	StringClass Preset = Get_Parameter("ObeliskEmitterPreset");
	MaxRange = Get_Float_Parameter("MaxRange");
	ChargeTime = Get_Float_Parameter("ChargeTime");
	GroundOnly = Get_Int_Parameter("GroundOnly");
	/*
	GameObject* WeaponObj = Commands->Create_Object(Preset, WeaponPos);
	if (WeaponObj) {
		WeaponID = Commands->Get_ID(WeaponObj);
		Attach_Script_Once_V(WeaponObj, "dblaney_Nod_Obelisk_Weapon", "%f,%f,%i", MaxRange, ChargeTime, GroundOnly);
		//Commands->Attach_Script(WeaponObj, "Obelisk_Weapon_CnC", "");
	}
	*/
	PowerOn=!Is_Base_Powered(Get_Object_Type(ObeliskObj));
	Commands->Send_Custom_Event(ObeliskObj,ObeliskObj,CUSTOM_EVENT_BUILDING_POWER_CHANGED,!PowerOn,0);

}


void dblaney_Nod_Obelisk_Structure::Killed(GameObject* ObeliskObj, GameObject* Killer) {
	// Kill the weapon too
	GameObject* WeaponObj = Commands->Find_Object(WeaponID);
	if (WeaponObj != 0) {
		Commands->Destroy_Object(WeaponObj);
	}
}

void dblaney_Nod_Obelisk_Structure::Destroyed(GameObject* ObeliskObj)
{
	GameObject* WeaponObj = Commands->Find_Object(WeaponID);
	if (WeaponObj != 0) {
		Commands->Destroy_Object(WeaponObj);
	}
}

void dblaney_Nod_Obelisk_Structure::Custom(GameObject* ObeliskObj, int type, int Param, GameObject* Sender)
{
	if (type == CUSTOM_EVENT_BUILDING_POWER_CHANGED)
	{
		//Console_Output("Ob Timer");
		int BasePower = Param;
		//if (PowerOn != BasePower) {
			//Console_Output("Power Changed"); Debug Stuff
			if (BasePower == 1) {
				PowerOn = true;
				GameObject* WeaponObj = Commands->Find_Object(WeaponID);
				if (!WeaponObj) {
					// Get weapon position
					Vector3 WeaponPos = Commands->Get_Position(ObeliskObj);
					//WeaponPos.Y += -4.5f*cos(Commands->Get_Facing(ObeliskObj)*(float)(PI / 180));
					//WeaponPos.X += 4.5f*sin(Commands->Get_Facing(ObeliskObj)*(float)(PI / 180));
					WeaponPos.Z += OffsetHeight;

					// Create the Obelisk weapon
					StringClass Preset = Get_Parameter("ObeliskEmitterPreset");
					GameObject* WeaponObj = Commands->Create_Object(Preset, WeaponPos);

					if (WeaponObj) {
						WeaponID = Commands->Get_ID(WeaponObj);
						Attach_Script_Once_V(WeaponObj, "dblaney_Nod_Obelisk_Weapon", "%f,%f,%i", MaxRange, ChargeTime, GroundOnly);
						//Commands->Attach_Script(WeaponObj, "Obelisk_Weapon_CnC", "");
					}
				}
			}
			else {
				// Kill the weapon object
				GameObject* WeaponObj = Commands->Find_Object(WeaponID);
				PowerOn = false;
				if (WeaponObj) {
					Commands->Destroy_Object(WeaponObj);
				}
			}
	}
	else if(type == CUSTOM_EVENT_BUILDING_REVIVED)
	{
		Commands->Send_Custom_Event(ObeliskObj,ObeliskObj,CUSTOM_EVENT_BUILDING_POWER_CHANGED,Commands->Get_Building_Power(ObeliskObj),0);
	}
}

ScriptRegistrant<dblaney_Nod_Obelisk_Structure> dblaney_Nod_Obelisk_Structure_Registrant("dblaney_Nod_Obelisk_Structure", "ObeliskEmitterPreset=Nod_Obelisk:string,MaxRange=150:float,ChargeTime=2:float,GroundOnly=0:int,OffsetHeight=22:float");

void dblaney_Nod_Obelisk_Weapon::Created(GameObject* WeaponObj) {
	// Some settings
	Commands->Set_Is_Rendered(WeaponObj, false); // It's not visible
	Commands->Set_Player_Type(WeaponObj, 0); // We're a Nod Obelisk, GDI will not own an Obelisk
	Commands->Enable_Enemy_Seen(WeaponObj, true); // We want to get notified when we see an enemy
	Commands->Enable_Hibernation(WeaponObj, false); // Not controlled, or whatever?
	Commands->Innate_Enable(WeaponObj); // Dunno :)

	MaxRange = Get_Float_Parameter("MaxRange");
	ChargeTime = Get_Float_Parameter("ChargeTime");
	GroundOnly = Get_Int_Parameter("GroundOnly");

	Firing = false;
	Charged = false;
	EnemyID = NULL;
	EffectID = NULL;
}

void dblaney_Nod_Obelisk_Weapon::Destroyed(GameObject* WeaponObj) {
	StopFiring(WeaponObj);
}

bool dblaney_Nod_Obelisk_Weapon::IsValidEnemy(GameObject* WeaponObj, GameObject* EnemyObj) {
	if (!EnemyObj) return false;
	// TODO: Make switch for obby kills neutral?
	// NEUTRAL:  if (Commands->Get_Player_Type(EnemyObj) == Commands->Get_Player_Type(WeaponObj)) return false;
	// GDI-ONLY: if (Commands->Get_Player_Type(EnemyObj) == 1) return false;
	//if (Commands->Get_Player_Type(EnemyObj) != 1) return false;
	if(!WeaponObj->As_DamageableGameObj()->Is_Enemy(EnemyObj->As_DamageableGameObj())) return false;
	if (Commands->Get_Health(EnemyObj) <= 0) return false;
	if (!Commands->Is_Object_Visible(WeaponObj, EnemyObj)) return false;

	Vector3 WeaponObjPos = Commands->Get_Position(WeaponObj);
	Vector3 WeaponObjPosXY = WeaponObjPos;
	WeaponObjPosXY.Z = 0;

	Vector3 EnemyObjPos = Commands->Get_Position(EnemyObj);
	Vector3 EnemyObjPosXY = EnemyObjPos;
	EnemyObjPosXY.Z = 0;

	float Distance = Commands->Get_Distance(WeaponObjPos, EnemyObjPos);
	float DistanceXY = Commands->Get_Distance(WeaponObjPosXY, EnemyObjPosXY);

	return DistanceXY > 15 && Distance < MaxRange;
}

void dblaney_Nod_Obelisk_Weapon::StartFiring(GameObject* WeaponObj) {
	// Start effect
	StartEffect(WeaponObj);

	// Start charging
	Commands->Start_Timer(WeaponObj, this, ChargeTime, 1);
	//StringClass msg;
	//msg.Format("msg %f", ChargeTime);
	//Console_Input(msg);
	Firing = true;
}

void dblaney_Nod_Obelisk_Weapon::StopFiring(GameObject* WeaponObj) {
	// Stop effect
	StopEffect(WeaponObj);

	// Stop firing
	Firing = false;
	Charged = false;
	EnemyID = NULL;
	EffectID = NULL;

	StopFireAt(WeaponObj);
}

void dblaney_Nod_Obelisk_Weapon::StartEffect(GameObject* WeaponObj) {
	Commands->Create_Sound("Obelisk_Warm_Up", Commands->Get_Position(WeaponObj), WeaponObj);

	GameObject* EffectObj = Commands->Create_Object("Obelisk Effect", Commands->Get_Position(WeaponObj));
	if (EffectObj) {
		EffectID = Commands->Get_ID(EffectObj);
	}
}
void dblaney_Nod_Obelisk_Weapon::StopEffect(GameObject* WeaponObj) {
	GameObject* EffectObj = Commands->Find_Object(EffectID);
	if (EffectObj) {
		Commands->Destroy_Object(EffectObj);
	}
}

void dblaney_Nod_Obelisk_Weapon::FireAt(GameObject* WeaponObj, GameObject* EnemyObj)
{
	Vector3 pos;
	if(EnemyObj->As_SoldierGameObj())
	{
		pos = EnemyObj->As_SoldierGameObj()->Get_Bullseye_Position();
	}
	else
	{
		pos = Commands->Get_Bone_Position(EnemyObj,"target");
	}

	ActionParamsStruct AttackParams;
	AttackParams.Set_Basic(this, 100, 0);
	//AttackParams.Set_Attack(EnemyObj, 150.f, 0, true);
	AttackParams.Set_Attack(pos, MaxRange, 0, true);
	AttackParams.AttackCheckBlocked = false;
	Commands->Action_Attack(WeaponObj, AttackParams);

	Commands->Start_Timer(WeaponObj, this, 1, 3);
}

void dblaney_Nod_Obelisk_Weapon::FaceEnemy(GameObject* WeaponObj, GameObject* EnemyObj)
{
	ActionParamsStruct FaceParams;
	FaceParams.Set_Basic(this, 90, 0);
	FaceParams.Set_Attack(EnemyObj, 0.f, 0, true);  //face enemyobj before firing.
	Commands->Action_Attack(WeaponObj, FaceParams);
}

void dblaney_Nod_Obelisk_Weapon::StopFireAt(GameObject* WeaponObj)
{
	Commands->Action_Reset(WeaponObj, 100);
}

void dblaney_Nod_Obelisk_Weapon::Timer_Expired(GameObject* WeaponObj, int Number) {
	if (Number == 1) {
		// Charged; Set variable and try to fire
		Charged = true;

		GameObject* EnemyObj = Commands->Find_Object(EnemyID);
		if (IsValidEnemy(WeaponObj, EnemyObj)) {
			// Fire at the enemy
			FireAt(WeaponObj, EnemyObj);
			Commands->Create_Sound("Obelisk_Warm_Up", Commands->Get_Position(WeaponObj), WeaponObj);

			if(!Commands->Find_Object(EffectID))
			{
				GameObject* EffectObj = Commands->Create_Object("Obelisk Effect", Commands->Get_Position(WeaponObj));
				if (EffectObj) {
					EffectID = Commands->Get_ID(EffectObj);
					}
			}

			// Check effect in 4 seconds
			Commands->Start_Timer(WeaponObj, this, 4, 1);
		} else {
			// Forget it
			StopFiring(WeaponObj);
		}
	} else if (Number == 2) {
		// Restart the effect
		StopEffect(WeaponObj);
		
		if (IsValidEnemy(WeaponObj, Commands->Find_Object(EnemyID))) {
			// Restart the effect
			StartEffect(WeaponObj);


			// Check again in 3.5 seconds
			Commands->Start_Timer(WeaponObj, this, 4, 1);
		} else {
			// Forget it
			StopFiring(WeaponObj);
		}
	} else if (Number == 3)
	{
		StopFireAt(WeaponObj);
		GameObject* EnemyObj = Commands->Find_Object(EnemyID);
		if (IsValidEnemy(WeaponObj, EnemyObj))
		{
			Commands->Create_Sound("Obelisk_Warm_Up", Commands->Get_Position(WeaponObj), WeaponObj);
			FaceEnemy(WeaponObj,EnemyObj);
		}
		else
		{
			Commands->Start_Timer(WeaponObj, this, 1.5f, 5);
		}
	}

	else if (Number == 4) {
		GameObject *EnemyObj = Commands->Find_Object(EnemyID);
		if (IsValidEnemy(WeaponObj, EnemyObj)) {
			FaceEnemy(WeaponObj,EnemyObj);
		}
	}

	else if (Number == 5) {
		GameObject* EnemyObj = Commands->Find_Object(EnemyID);
		if (!IsValidEnemy(WeaponObj, EnemyObj))
		{
			StopEffect(WeaponObj);
		}
	}
}

void dblaney_Nod_Obelisk_Weapon::Enemy_Seen(GameObject* WeaponObj, GameObject* EnemyObj) {

	if (GroundOnly == 1)
		if (Is_VTOL(EnemyObj))
			return;
	// Check for an living target which is in range
	if (!IsValidEnemy(WeaponObj, EnemyObj)) {
		return;
	}

	// If the previous enemy has gone, set the enemy to the currently detected enemy
	if (!IsValidEnemy(WeaponObj, Commands->Find_Object(EnemyID))) {
		EnemyID = Commands->Get_ID(EnemyObj);
	}

	if (Firing) {
		if (Charged) {
			// Only change the target if the previous target has gone
			if (!IsValidEnemy(WeaponObj, Commands->Find_Object(EnemyID))) {
				FireAt(WeaponObj, EnemyObj);
			}
		}

		// if not charged and previous target has gone, face new enemy
		else if(!IsValidEnemy(WeaponObj, Commands->Find_Object(EnemyID))) {
			FaceEnemy(WeaponObj,EnemyObj);
		}
	}
	else {
		StartFiring(WeaponObj);
	}
}
void dblaney_Nod_Obelisk_Weapon::Register_Auto_Save_Variables() {
	Auto_Save_Variable(&EnemyID, 4, 1);
	Auto_Save_Variable(&EffectID, 4, 1);
	Auto_Save_Variable(&Firing, 1, 3);
	Auto_Save_Variable(&Charged, 1, 4);
}

ScriptRegistrant<dblaney_Nod_Obelisk_Weapon> dblaney_Nod_Obelisk_Weapon_Registrant("dblaney_Nod_Obelisk_Weapon", "MaxRange=150:float,ChargeTime=2:float,GroundOnly=0:int,Team=0:int");

void DB_Power_Down_Console::Created(GameObject *obj)
{
	reset = false;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}

void DB_Power_Down_Console::Poked(GameObject *obj,GameObject *poker)
{
	team = Commands->Get_Player_Type(poker);
	if (Is_Base_Powered(PTTEAM(team)) && !reset && (!Is_Building_Dead(Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_POWER_PLANT))))
	{
		Commands->Create_2D_Sound(Get_Parameter("Sound"));
		Power_Base(PTTEAM(team),false);
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		StringClass message;
		if(Get_Player_Count()<=1)
		{
			message.Format("%s base power offline. Power will reset in approximately %g seconds.",Get_Team_Name(PTTEAM(team)),Get_Float_Parameter("Power_Time") + Get_Float_Parameter("SinglePlayerBonus"));
			Send_Message(255,200,200,message);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Power_Time") + Get_Float_Parameter("SinglePlayerBonus"),1);
		}
		
		else
		{
			message.Format("%s base power offline. Power will reset in approximately %g seconds.",Get_Team_Name(PTTEAM(team)),Get_Float_Parameter("Power_Time"));
			Send_Message(255,200,200,message);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Power_Time"),1);
		}
	}
}

void DB_Power_Down_Console::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if(!Is_Building_Dead(Find_Building_By_Type(PTTEAM(team),BuildingConstants::TYPE_POWER_PLANT)))
		{
			Commands->Create_2D_Sound(Get_Parameter("Sound2"));
			Power_Base(PTTEAM(team),true);
			StringClass message;
			message.Format("%s base power online.",Get_Team_Name(PTTEAM(team)));
			Send_Message(255,200,200,message);
		}
		reset = true;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Reset_Time"),2);
	}


	if (number == 2)
	{
		Commands->Enable_HUD_Pokable_Indicator(obj,true);
		reset = false;
	}
}

ScriptRegistrant<DB_Power_Down_Console> DB_Power_Down_Console_Registrant("DB_Power_Down_Console","Power_Time:float=60,Reset_Time:float=5,Sound:string,Sound2:string,SinglePlayerBonus:float=0");

void DB_Power_Down_Building_Console::Created(GameObject *obj)
{
	reset = false;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	buildingID = Get_Int_Parameter("BuildingID");
}

void DB_Power_Down_Building_Console::Poked(GameObject *obj,GameObject *poker)
{
	team = Commands->Get_Player_Type(poker);
	GameObject *building = Commands->Find_Object(buildingID);
	if (Is_Base_Powered(Get_Object_Type(building)) && !Is_Building_Dead(building) && !reset)
	{
		Commands->Create_2D_Sound(Get_Parameter("Sound"));
		Commands->Set_Building_Power(building,false);
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		StringClass message;
		if(Get_Player_Count()<=1)
		{
			message.Format("%s %s offline. Power to the %s will be reset in approximately %g seconds.",Get_Team_Name(PTTEAM(team)),Get_Translated_Preset_Name(building),Get_Translated_Preset_Name(building),Get_Float_Parameter("Power_Time") + Get_Float_Parameter("SinglePlayerBonus"));
			Send_Message(255,200,200,message);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Power_Time") + Get_Float_Parameter("SinglePlayerBonus"),1);
		}
		
		else
		{
			message.Format("%s %s offline. Power to the %s will be reset in approximately %g seconds.",Get_Team_Name(PTTEAM(team)),Get_Translated_Preset_Name(building),Get_Translated_Preset_Name(building),Get_Float_Parameter("Power_Time"));
			Send_Message(255,200,200,message);
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Power_Time"),1);
		}
	}
}

void DB_Power_Down_Building_Console::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		GameObject *building = Commands->Find_Object(buildingID);
		if(Is_Base_Powered(Get_Object_Type(building)) && !Is_Building_Dead(building))
		{
			Commands->Create_2D_Sound(Get_Parameter("Sound2"));
			Commands->Set_Building_Power(building,true);
			StringClass message;
			message.Format("%s %s online.",Get_Team_Name(Get_Object_Type(building)),Get_Translated_Preset_Name(building));
			Send_Message(255,200,200,message);
		}
		reset = true;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Reset_Time"),2);
	}


	if (number == 2)
	{
		Commands->Enable_HUD_Pokable_Indicator(obj,true);
		reset = false;
	}
}

ScriptRegistrant<DB_Power_Down_Building_Console> DB_Power_Down_Building_Console_Registrant("DB_Power_Down_Building_Console","BuildingID:int=0,Power_Time:float=60,Reset_Time:float=5,Sound:string,Sound2:string,SinglePlayerBonus:float=0");



void DB_Disable_Injured_Animations::Created(GameObject *obj)
{
	if(obj->As_SoldierGameObj())
	{
		obj->As_SoldierGameObj()->Set_Can_Play_Damage_Animations(false);
	}
}

void DB_Disable_Injured_Animations::Detach(GameObject *obj)
{
	if(obj->As_SoldierGameObj())
	{
		obj->As_SoldierGameObj()->Set_Can_Play_Damage_Animations(true);
	}
}

ScriptRegistrant<DB_Disable_Injured_Animations> DB_Disable_Injured_Animations_Registrant("DB_Disable_Injured_Animations","");

void DB_Set_Scale::Created(GameObject *obj)
{
	if(obj->As_SoldierGameObj())
	{
		originalscale = obj->As_SoldierGameObj()->Get_Scale_Across_Network();
		obj->As_SoldierGameObj()->Set_Scale_Across_Network(Get_Float_Parameter("Scale"));
	}
}

void DB_Set_Scale::Detach(GameObject *obj)
{

}

ScriptRegistrant<DB_Set_Scale> DB_Set_Scale_Registrant("DB_Set_Scale","Scale:float=1.00");


void DB_Grant_Keycard_Killed::Killed(GameObject *obj, GameObject *killer)
{
	Send_Message(255,255,255,StringClass::getFormattedString("Level %i Keycard acquired.",Get_Int_Parameter("KeyLevel")));
	GameObject *keyscript = Commands->Create_Object("Invisible_Object",Commands->Get_Position(obj));
	if(keyscript)
	{
		Attach_Script_Once_V(keyscript,"DB_Grant_Keycard_Timer","%i",Get_Int_Parameter("KeyLevel"));
	}
	Destroy_Script();
}


void DB_Grant_Keycard_Killed::Destroyed(GameObject *obj)
{
	Send_Message(255,255,255,StringClass::getFormattedString("Level %i Keycard acquired.",Get_Int_Parameter("KeyLevel")));
	GameObject *keyscript = Commands->Create_Object("Invisible_Object",Commands->Get_Position(obj));
	if(keyscript)
	{
		Attach_Script_Once_V(keyscript,"DB_Grant_Keycard_Timer","%i",Get_Int_Parameter("KeyLevel"));
	}
	
}

void DB_Grant_Keycard_Timer::Created(GameObject *obj)
{
	KeyLevel = Get_Int_Parameter("KeyLevel");
	Commands->Start_Timer(obj,this,1.0f,50);
}

void DB_Grant_Keycard_Timer::Timer_Expired(GameObject *obj, int number)
{
	if (number == 50)
	{
		SLNode<SoldierGameObj> *x = GameObjManager::StarGameObjList.Head();
		{
			while (x)
			{
				GameObject *o = x->Data();
				if (o)
				{
					if(!Commands->Has_Key(o,KeyLevel))
					{
						Commands->Grant_Key(o,KeyLevel,true);
					}
				}
				x = x->Next();
			}
		}
		Commands->Start_Timer(obj,this,5.0f,50);
	}
}

ScriptRegistrant<DB_Grant_Keycard_Killed> DB_Grant_Keycard_Killed_Registrant("DB_Grant_Keycard_Killed","KeyLevel=1:int");
ScriptRegistrant<DB_Grant_Keycard_Timer> DB_Grant_Keycard_Timer_Registrant("DB_Grant_Keycard_Timer","KeyLevel=1:int");


void  DB_Infantry_Place_Buildable_Object::Created(GameObject *obj)
{
	canRegen = false;
	previewObjectId = 0;
	reloadTime = 0;
	if (!Has_Weapon(obj,Get_Parameter("WeaponPreset")) && Get_Int_Parameter("GrantWeapon"))
		Grant_Weapon(obj,Get_Parameter("WeaponPreset"),true,1,0);
	Commands->Start_Timer(obj,this,0.25f,1);
	Commands->Start_Timer(obj,this,1.0f,2);
}
void  DB_Infantry_Place_Buildable_Object::Timer_Expired(GameObject *obj,int number)
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
				GameObject *preview = Commands->Create_Object("Invisible_Object",Commands->Get_Position(obj));
				previewObjectId = Commands->Get_ID(preview);
				Update_Network_Object(preview);
				Set_Object_Visibility(previewObjectId,false);
				//if ((int)Get_Vector3_Parameter("MatchTeam[Preview|Deployed|Repaired]").X)
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
				Send_Message_Player(obj,255,255,255,Get_Parameter("PlacementMessage")); 
			}
		}
		Commands->Start_Timer(obj,this,1.0f,2);
	}
}
void  DB_Infantry_Place_Buildable_Object::Custom(GameObject *obj,int message,int param,GameObject *sender)
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
			Send_Message_Player(obj,255,255,255,"You must be on the ground to place this!");
			return;
		}
		if (obj->As_SoldierGameObj() && obj->As_SoldierGameObj()->Is_On_Ladder())
		{
			Send_Message_Player(obj,255,255,255,"You cannot place this while climbing ladders!");
			return;
		}
		if (placementBlocked)
		{
			Send_Message_Player(obj,255,255,255,Get_Parameter("DeploymentBlockedMessage"));
			return;
		}
		if (!canRegen && reloadTime)
		{
			Send_Message_Player(obj,255,255,255,"You must repair the last one you placed before you can place another.");
			return;
		}
		if (reloadTime)
		{
			char reloadTimeMessage[220];
			sprintf(reloadTimeMessage,"You must wait another %d seconds before you can place this.",reloadTime);
			Send_Message_Player(obj,255,255,255,reloadTimeMessage);
			return;
		}
		if (Get_Float_Parameter("Cost") && Commands->Get_Money(obj) < Get_Float_Parameter("Cost"))
		{
			char costMessage[220];
			sprintf(costMessage,"You need $%i to place this.",(int)Get_Float_Parameter("Cost"));
			Send_Message_Player(obj,255,255,255,costMessage);
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
		//if ((int)Get_Vector3_Parameter("MatchTeam[Preview|Deployed|Repaired]").Y)
			//Commands->Set_Player_Type(placed,Commands->Get_Player_Type(obj));
		Commands->Set_Facing(placed,Commands->Get_Facing(obj));
		char params[220];
		sprintf(params,"%d,%d,%s,%d,%d",this->Get_ID(),Commands->Get_ID(obj),Get_Parameter("RepairedPreset"),Get_Int_Parameter("MatchTeam"),Commands->Get_Player_Type(obj));
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
void  DB_Infantry_Place_Buildable_Object::Killed(GameObject *obj,GameObject *killer)
{
	DestroyPreview();
}
void  DB_Infantry_Place_Buildable_Object::Destroyed(GameObject *obj)
{
	DestroyPreview();
}
void  DB_Infantry_Place_Buildable_Object::DestroyPreview()
{
	GameObject *preview = Commands->Find_Object(previewObjectId);
	if (preview)
		Commands->Destroy_Object(preview);
	previewObjectId = 0;
}

ScriptRegistrant<DB_Infantry_Place_Buildable_Object> DB_Infantry_Place_Buildable_Object_Registrant("DB_Infantry_Place_Buildable_Object","WeaponPreset=null:string,DeployPreset=null:string,RepairedPreset=null:string,PlaceablePreview=null:string,UnPlaceablePreview=null:string,EnableCustom:int,DisableCustom:int,DeployCustom:int,RegainTimer:int,Cost:float,DeploymentBlockedMessage=Cannot deploy when near buildings.:string,PlacementMessage=You can now place another.:string,GrantWeapon=1:int,RegenStartsAfterRepair=1:int,MatchTeam=0:int,DestroyedRegenTime=1:int");

void DB_Stationary_Stealth::Created(GameObject *obj)
{
	xpos = 0;
	ypos = 0;
	zpos = 0;
	isHidden=false;
	Commands->Start_Timer(obj,this,1.0f,572384);
}

void DB_Stationary_Stealth::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&xpos,4,2);
	Auto_Save_Variable(&ypos,4,3);
	Auto_Save_Variable(&zpos,4,4);
}

void DB_Stationary_Stealth::Timer_Expired(GameObject *obj,int number)
{
	if(number == 572384)
	{
		Vector3 position = Commands->Get_Position(obj);
		if ((!xpos) && (!ypos) && (!zpos))
		{
			xpos = position.X;
			ypos = position.Y;
			zpos = position.Z;
		}
		if ((fabs(position.X-xpos) <= .5) && (fabs(position.Y-ypos) <= .5) && (fabs(position.Z-zpos) <= .5))
		{
			if(!isHidden && !Is_Script_Attached(obj,"Stealth_Powerup"))
			{
				isHidden=true;
				Attach_Script_Once(obj,"Stealth_Stationary","");
				Commands->Enable_Stealth(obj,true);
			}
		}
		else if(isHidden)
		{
			/*
			if(!Is_Script_Attached(obj,"Stealth_Powerup"))
			{
				Commands->Enable_Stealth(obj,false);
			}
			*/
			Remove_Script(obj,"Stealth_Stationary");
			isHidden=false;
		}
		xpos = position.X;
		ypos = position.Y;
		zpos = position.Z;
		Commands->Start_Timer(obj,this,1.0f,572384);
	}
}

ScriptRegistrant<DB_Stationary_Stealth> DB_Stationary_Stealth_Registrant("DB_Stationary_Stealth","");


void DB_Turret_Sound::Created ( GameObject* obj )
{
  if ( VehicleGameObj* vObj = obj->As_VehicleGameObj() )
  {
    minRotate = Get_Float_Parameter("Min_Rotate_Amount");
    lastFacing = Get_Turret_Facing(vObj->Peek_Model());
    emitterID = -1;
    Commands->Start_Timer(obj, this, 0.1f, 1 );
  }
  else
  {
    Destroy_Script();
  }
}

void DB_Turret_Sound::Timer_Expired ( GameObject* obj, int number )
{
  if ( number == 1 )
  {
    if ( VehicleGameObj* vObj = obj->As_VehicleGameObj() )
    {
      float newFacing = Get_Turret_Facing(vObj->Peek_Model());

      // Check if we are rotating - ignore tiny rotation amounts
      bool rotating = ( abs(newFacing-lastFacing) > minRotate );

      if ( emitterID == -1 && rotating)
        Play_Sound(obj);

      else if ( emitterID != -1 && !rotating )
        Stop_Sound(obj);

      lastFacing = newFacing;
    }

    // Restart timer to check for rotation
    Commands->Start_Timer(obj, this, 0.1f, 1 );
  }
}

float DB_Turret_Sound::Get_Turret_Facing ( RenderObjClass* RenderObj )
{
  if ( RenderObj )
  {
    Matrix3D vehicleTransform = RenderObj->Get_Transform();
    Matrix3D transform = RenderObj->Get_Bone_Transform("turret");
    return abs(vehicleTransform.Get_Z_Rotation()-transform.Get_Z_Rotation());
  }

  return 0.0f;
}

void DB_Turret_Sound::Play_Sound ( GameObject* obj )
{
	GameObject *soundemitter = Commands->Create_Object_At_Bone(obj, "Invisible_Object", "turret");
	if(soundemitter)
	{
		Commands->Attach_To_Object_Bone(soundemitter,obj,"turret");
		Commands->Set_Model(soundemitter,Get_Parameter("Sound_Emitter"));
		emitterID=soundemitter->Get_ID();
	}
}

void DB_Turret_Sound::Stop_Sound ( GameObject* obj )
{
	GameObject *soundemitter = Commands->Find_Object(emitterID);
	if(soundemitter)
	{
		Commands->Destroy_Object(soundemitter);
		emitterID = -1;
	}
}

ScriptRegistrant<DB_Turret_Sound> DB_Turret_Sound_Registrant("DB_Turret_Sound","Sound_Emitter:string,Min_Rotate_Amount=0.05:float");

void DB_Turret_Spawn::Created(GameObject *obj)
{
	GameObject *turret = Commands->Create_Object(Get_Parameter("Turret_Preset"),Vector3());
	if(!turret)
	{
		Destroy_Script();
		return;
	}
	Commands->Attach_To_Object_Bone(turret,obj,Get_Parameter("Bone_Name"));
	turretId = Commands->Get_ID(turret);
	if (turret->As_VehicleGameObj())
		turret->As_VehicleGameObj()->Set_Is_Scripts_Visible(false);
	hasDriver = false;
	bombard = false;
	Attach_Script_Once(obj,"DB_Turret_Bombard_Ability","");
}
void DB_Turret_Spawn::Custom(GameObject *obj,int message,int param,GameObject *sender)
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
				Commands->Send_Custom_Event(turret,turret,CUSTOM_AI_ENABLEAI,0,0);
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
				Commands->Action_Reset(turret,201);
				bombard=false;
				Commands->Send_Custom_Event(turret,turret,CUSTOM_AI_DISABLEAI,0,0);
			}
		}
	}
	if (message == REATTACH_BONES_CUSTOM)
	{
		GameObject *turret = Commands->Find_Object(turretId);
		if (turret)
		{
			Commands->Attach_To_Object_Bone(turret,obj,Get_Parameter("Bone_Name"));
		}
	}

	if(message == BOMBARD_TOGGLE)
	{
		if(bombard == false)
		{
			GameObject *turret = Commands->Find_Object(turretId);
			if(turret)
			{
				ActionParamsStruct var;
				var.Priority=200;
				var.AttackCheckBlocked = false;
				var.Set_Attack(obj->As_VehicleGameObj()->Get_Targeting_Pos(),1000,0,true);
				Commands->Action_Attack(turret,var);
				bombard=true;
				Commands->Send_Custom_Event(turret,turret,CUSTOM_AI_DISABLEAI,0,0);
				Set_HUD_Help_Text_Player_Text(sender,7403,"Manual Targeting set, Press 'T' to switch to auto targeting",Vector3(0.3f,0.3f,1.0f));
			}
		}
		else
		{
			GameObject *turret = Commands->Find_Object(turretId);
			if(turret)
			{
				Commands->Action_Reset(turret,201);
				bombard=false;
				Commands->Send_Custom_Event(turret,turret,CUSTOM_AI_ENABLEAI,0,0);
				Set_HUD_Help_Text_Player_Text(sender,7403,"Auto Targeting set, Press 'T' to set manual target location",Vector3(0.3f,0.3f,1.0f));
			}
		}
	}
}
void DB_Turret_Spawn::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *turret = Commands->Find_Object(turretId);
	if (turret)
	{
		Commands->Set_Health(turret,0);
		Commands->Set_Shield_Strength(turret,0);
		Commands->Apply_Damage(obj, 100.00, "BlamoKiller", killer);
	}
}
void DB_Turret_Spawn::Destroyed(GameObject *obj)
{
	GameObject *turret = Commands->Find_Object(turretId);
	if (turret)
		Commands->Destroy_Object(turret);
}

ScriptRegistrant<DB_Turret_Spawn> DB_Turret_Spawn_Registrant("DB_Turret_Spawn","Turret_Preset:string,Bone_Name=Tur_Mount:string");

void DB_Spawned_Visible_Ammo::Created(GameObject *obj)
{
	Ammo = Commands->Create_Object(Get_Parameter("Ammo_Preset"),Vector3());
	if(!Ammo)
	{
		Destroy_Script();
		return;
	}
	Commands->Disable_All_Collisions(Ammo);
	Commands->Attach_To_Object_Bone(Ammo,obj,Get_Parameter("Bone_Name"));
	DB_Spawned_Visible_Ammo::Timer_Expired(obj,1);
}

void DB_Spawned_Visible_Ammo::Timer_Expired(GameObject *obj,int number)
{
	VehicleGameObj *vgo = ((ScriptableGameObj *)obj)->As_VehicleGameObj();
	if (vgo && number == 1 && Ammo)
	{
        if(vgo->Get_Weapon() != NULL)
        {
	    	int ammo = vgo->Get_Weapon()->Get_Total_Rounds();
	    	if (ammo == -1)
	    	{
	    		ammo = vgo->Get_Weapon()->Get_Clip_Rounds();
	    	}
	    	if (ammo != 0)
	    	{
				Commands->Set_Is_Rendered(Ammo,true);
	    	}
			else
			{
				Commands->Set_Is_Rendered(Ammo,false);
			}
        }
		Commands->Start_Timer(obj,this,0.01f,1);
	}
}

void DB_Spawned_Visible_Ammo::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == REATTACH_BONES_CUSTOM)
	{
		if (Ammo)
		{
			Commands->Attach_To_Object_Bone(Ammo,obj,Get_Parameter("Bone_Name"));
		}
	}
}

void DB_Spawned_Visible_Ammo::Destroyed(GameObject *obj)
{
	if (Ammo)
	{
		Commands->Destroy_Object(Ammo);
	}
}

ScriptRegistrant<DB_Spawned_Visible_Ammo> DB_Spawned_Visible_Ammo_Registrant("DB_Spawned_Visible_Ammo","Ammo_Preset:string,Bone_Name=AmmoMount:string");

void DB_Vehicle_Visible_Ammo::Created(GameObject *obj)
{
	enabled = Get_Int_Parameter("StartEnabled");
	messageOn = Get_Int_Parameter("EnableMessage");
	messageOff = Get_Int_Parameter("DisableMessage");
	Commands->Start_Timer(obj,this,0.01f,1);
}

void DB_Vehicle_Visible_Ammo::Timer_Expired(GameObject *obj,int number)
{
	VehicleGameObj *vgo = ((ScriptableGameObj *)obj)->As_VehicleGameObj();
	if (vgo && number == 1)
	{
        if(enabled && vgo->Get_Weapon() != NULL)
        {
	    	int ammo = vgo->Get_Weapon()->Get_Clip_Rounds();
	    	if (ammo >= 0)
	    	{
	    		Commands->Set_Animation_Frame(obj,Get_Parameter("Animation"),ammo);
	    	}
        }
		Commands->Start_Timer(obj,this,0.01f,1);
	}
}

void DB_Vehicle_Visible_Ammo::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == messageOn)
	{
		enabled=true;
	}
	else if (message == messageOff)
	{
		enabled=false;
	}
}

ScriptRegistrant<DB_Vehicle_Visible_Ammo> DB_Vehicle_Visible_Ammo_Registrant("DB_Vehicle_Visible_Ammo","Animation:string,StartEnabled=0:int,EnableMessage=9320945:int,DisableMessage=9320946:int");

void DB_Orca_Lifter::Created(GameObject *obj)
{
	zoneID=0;
}

void DB_Orca_Lifter::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == DEPLOY_COMPLETE_CUSTOM)
	{
		Matrix3 rotation(true);
		rotation.Rotate_Z(DEG2RAD(Commands->Get_Facing(obj)));

		// Define the bounding box and create the zone
		OBBoxClass zoneBoundingBox ( Commands->Get_Bone_Position(obj,Get_Parameter("Zone_Bone")), Get_Vector3_Parameter("Zone_Size"), rotation );
		if ( GameObject* reloadZone = Create_Zone("Script_Zone_All",zoneBoundingBox) )
		{
			zoneID = Commands->Get_ID(reloadZone);
			Attach_Script_Once_V(reloadZone,"DB_capturable_helipadzone_reload","%i,%s",obj->Get_ID(),Get_Parameter("Reload_Preset"));
			Attach_Script_Once_V(reloadZone,"DB_capturable_Repairzone","%i",obj->Get_ID());
		}
	}
	else if (message == UNDEPLOY_BEGIN_CUSTOM)
	{
		if(GameObject *reloadZone = Commands->Find_Object(zoneID))
		{
			Commands->Destroy_Object(reloadZone);
		}
		zoneID = 0;
	}

	else if (message == CUSTOM_EVENT_VEHICLE_EXITED && zoneID && obj->As_VehicleGameObj()->Get_Occupant_Count() == 0)
	{
		Update_Network_Object(obj);
		Commands->Set_Player_Type(obj,Commands->Get_Player_Type(sender));
	}
}

void DB_Orca_Lifter::Destroyed(GameObject *obj)
{
	if(GameObject *reloadZone = Commands->Find_Object(zoneID))
	{
		Commands->Destroy_Object(reloadZone);
	}
	zoneID = 0;
}

ScriptRegistrant<DB_Orca_Lifter> DB_Orca_Lifter_Registrant("DB_Orca_Lifter","Zone_Bone=Origin:string,Zone_Size:vector3,Reload_Preset:string");

void DB_Turret_Bombard_Ability::Created(GameObject *obj)
{
	PilotID = 0;
}

void DB_Turret_Bombard_Ability::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!PilotID)
		{
			PilotID = Commands->Get_ID(sender);
			char params[50];
			sprintf(params,"IKDeploy,%d",Commands->Get_ID(obj));
			if(!Is_Script_Attached(sender,"Reborn_Deployable_Vehicle_Player"))
				Commands->Attach_Script(sender,"DB_Turret_Bombard_Player",params);
		}
	}
	else if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (PilotID == Commands->Get_ID(sender))
		{
			PilotID = 0;
			Remove_Script(sender,"DB_Turret_Bombard_Player");
		}
	}
}

ScriptRegistrant<DB_Turret_Bombard_Ability> DB_Turret_Bombard_Ability_Registrant("DB_Turret_Bombard_Ability","");

void DB_Turret_Bombard_Player::Created(GameObject *obj)
{
	InstallHook(Get_Parameter("Key"),obj);
	Set_HUD_Help_Text_Player_Text(obj,7403,"Press the 'T' key to toggle auxilery turret manual targeting",Vector3(0.3f,0.3f,1.0f));
}

void DB_Turret_Bombard_Player::KeyHook()
{
	Commands->Send_Custom_Event(Owner(),Commands->Find_Object(Get_Int_Parameter("ID")),BOMBARD_TOGGLE,0,0);
}

ScriptRegistrant<DB_Turret_Bombard_Player>  DB_Turret_Bombard_Player_Registrant("DB_Turret_Bombard_Player","Key=Deploy:string,ID=0:int");

void DB_Health_Sound_Timer::Created(GameObject *obj)
{
	float timertime;
	int timernumber;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void DB_Health_Sound_Timer::Timer_Expired(GameObject *obj,int number)
{
	int timernumber;
	float minhealth,maxhealth,health;
	float timertime;
	int repeat;
	const char *c;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	repeat = Get_Int_Parameter("Repeat");
	minhealth = Get_Float_Parameter("Min_Health");
	maxhealth = Get_Float_Parameter("Max_Health");
	c = Get_Parameter("Sound");
	if (number == timernumber)
	{
		health = Commands->Get_Health(obj);
		if ((health >= minhealth) && (health <= maxhealth))
		{
			Commands->Create_3D_Sound_At_Bone(c,obj,"origin");
		}
		if (repeat == 1)
		{	
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}

ScriptRegistrant<DB_Health_Sound_Timer> DB_Health_Sound_Timer_Registrant("DB_Health_Sound_Timer","Time:float,TimerNum:int,Repeat:int,Sound:string,Min_Health:float,Max_Health:float");

void DB_Visible_Passenger::Created(GameObject *obj)
{
	modelid = 0;
	passengerid = 0;
	stealth = false;
	Commands->Start_Timer(obj, this, 0.25f, 1);
}

void DB_Visible_Passenger::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		int seat=Get_Occupant_Seat(obj,sender);
		if(seat==Get_Int_Parameter("Seat"))
		{
			
			Vector3 position = Commands->Get_Bone_Position(obj,Get_Parameter("BoneName"));
			GameObject *object = Commands->Create_Object("Invisible_Object",position);
			Commands->Disable_All_Collisions(object);
			Commands->Attach_To_Object_Bone(object,obj,Get_Parameter("BoneName"));
			Commands->Set_Model(object,Get_Model(sender));
			modelid = Commands->Get_ID(object);
			passengerid = Commands->Get_ID(sender);
			int staticAnim = Get_Int_Parameter("StaticAnim");
			if(staticAnim)
			{
				int frame = Get_Int_Parameter("StaticFrame");
				Commands->Set_Animation_Frame(object,Get_Parameter("Animation"),frame);
			}
			else
			{
				Commands->Set_Animation(object,Get_Parameter("Animation"),true,0,Get_Animation_Frame(obj),-1,false);
			}
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED && modelid && sender->Get_ID()==passengerid)
	{
		Commands->Destroy_Object(Commands->Find_Object(modelid));
		modelid = 0;
		passengerid = 0;
	}
}

void DB_Visible_Passenger::Destroyed(GameObject *obj)
{
	if (modelid)
	{
		Commands->Destroy_Object(Commands->Find_Object(modelid));
	}
}

void DB_Visible_Passenger::Timer_Expired(GameObject *obj, int number)
{
	bool b = Is_Stealth(obj);
	if (stealth != b)
	{
		stealth = b;
		if (modelid)
		{
			Commands->Set_Is_Rendered(Commands->Find_Object(modelid), !stealth);
		}
	}
	Commands->Start_Timer(obj, this, 0.25f, 1);
}

void DB_Visible_Passenger::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&passengerid,4,1);
	Auto_Save_Variable(&modelid,4,1);
	Auto_Save_Variable(&stealth,1,2);
}

ScriptRegistrant<DB_Visible_Passenger> DB_Visible_Passenger_Registrant("DB_Visible_Passenger","Seat=0:int,BoneName=seat0:string,Animation=s_a_human.h_a_a0a0:string,StaticAnim=0:int,StaticFrame=0:int");

void DB_Drop_Wreckage_On_Death::Killed(GameObject *obj,GameObject *killer)
{
	if(Get_Damage_Warhead()!=25)
	{
		if(Find_Named_Definition(Get_Parameter("Wreckage_Preset")) && obj->As_VehicleGameObj())
		{
			Matrix3D transform = obj->As_PhysicalGameObj()->Get_Transform();
			GameObject *CurTank;		
			CurTank = Commands->Create_Object(Get_Parameter("Wreckage_Preset"),Commands->Get_Position(obj));
			if(CurTank)
			{
				PhysicalGameObj *Shell = (PhysicalGameObj*)Commands->Create_Object("Mounted",Commands->Get_Position(obj));
				if(Shell)
				{
					Shell->Set_Transform(transform);
					Commands->Set_Model(Shell,Get_Model(CurTank));
					Shell->Set_Collision_Group(SOLDIER_GHOST_COLLISION_GROUP);
					Shell->Set_Player_Type(-2);
					Commands->Enable_Vehicle_Transitions(Shell,false);
					DefenseObjectClass *WreckDefense = CurTank->As_DamageableGameObj()->Get_Defense_Object();
					DefenseObjectClass *ShellDefense = Shell->Get_Defense_Object();
					ShellDefense->Set_Health_Max(WreckDefense->Get_Health_Max());
					ShellDefense->Set_Shield_Strength_Max(WreckDefense->Get_Shield_Strength_Max());
					Commands->Set_Health(Shell,WreckDefense->Get_Health());
					Commands->Set_Shield_Strength(Shell,WreckDefense->Get_Shield_Strength());
					ShellDefense->Set_Skin(WreckDefense->Get_Skin());
					ShellDefense->Set_Shield_Type(WreckDefense->Get_Shield_Type());
					ShellDefense->Set_Damage_Points(0.0f);
					ShellDefense->Set_Death_Points(0.0f);
					CurTank->Set_Delete_Pending();
					Commands->Attach_Script(Shell,"DB_Wreckage_Rebuildable",Commands->Get_Preset_Name(obj));
					Update_Network_Object(Shell);
					Force_Orientation_Update(Shell);
				}
			}
		}
	}
}

void DB_Wreckage_Rebuildable::Created(GameObject *obj)
{
	obj->As_PhysicalGameObj()->Set_Collision_Group(SOLDIER_GHOST_COLLISION_GROUP);
	Commands->Set_Player_Type(obj,-2);
	obj->As_DamageableGameObj()->Get_Defense_Object()->Set_Can_Object_Die(false);
	firstdamage=false;
	Commands->Start_Timer(obj,this,0.01f,1);
}

void DB_Wreckage_Rebuildable::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if(!firstdamage)
	{
		obj->As_DamageableGameObj()->Get_Defense_Object()->Set_Can_Object_Die(true);
		firstdamage=true;
	}

	if (amount < 0)
	{
		float TotalHealth = Commands->Get_Health(obj) + Commands->Get_Shield_Strength(obj);
		const float MaxHealth = Commands->Get_Max_Health(obj) + Commands->Get_Max_Shield_Strength(obj);
		Vector3 CurPosition = Commands->Get_Position(obj);
		if (TotalHealth == MaxHealth)
		{
			GameObject *CurTank = Commands->Create_Object(Get_Parameter("Vehicle_Preset"),Commands->Get_Position(obj));
			Commands->Set_Facing(CurTank,Commands->Get_Facing(obj));
			Commands->Set_Health(CurTank,5);
			Commands->Set_Shield_Strength(CurTank,0);
			Commands->Set_Player_Type(CurTank,Commands->Get_Player_Type(damager));
			CurPosition.Z += 1;
			Commands->Set_Position(CurTank,CurPosition);
			if(CurTank->As_VehicleGameObj())
				Attach_Script_Once(CurTank,"DB_Face_Forward","0.1");
			Commands->Destroy_Object(obj); 
			#ifdef DRAGONADE
				Fix_Stuck_Objects(Commands->Get_Position(obj),10.0f,15.0f,false);
			#endif // DRAGONADE
		}
	}
}

void DB_Wreckage_Rebuildable::Killed(GameObject *obj,GameObject *killer)
{
	Commands->Create_Explosion("Explosion_with_Debris_small",Commands->Get_Position(obj),killer);
}

void DB_Wreckage_Rebuildable::Timer_Expired(GameObject *obj, int number)
{
	if(number==1)
	{
		obj->As_DamageableGameObj()->Get_Defense_Object()->Set_Can_Object_Die(true);
	}
}

ScriptRegistrant<DB_Drop_Wreckage_On_Death> DB_Drop_Wreckage_On_Death_Registrant("DB_Drop_Wreckage_On_Death","Wreckage_Preset:string");
ScriptRegistrant<DB_Wreckage_Rebuildable> DB_Wreckage_Rebuildable_Registrant("DB_Wreckage_Rebuildable","Vehicle_Preset:string");

void DB_Supply_Truck::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (Commands->Get_ID(Get_Vehicle_Driver(obj)) == Commands->Get_ID(sender))
		{
			Send_Message_Player(sender,255,255,255,"Supply Truck: You refill everyone that enters your Truck");
		}
		else
		{
			if (Get_Vehicle_Driver(obj))
			{
				Commands->Give_PowerUp(sender,"CnC_Ammo_Crate",false);
				Commands->Set_Health(sender,Commands->Get_Max_Health(sender));
				Commands->Set_Shield_Strength(sender,Commands->Get_Max_Shield_Strength(sender));
				Send_Message_Player(sender,255,255,255,"Your ammo, health and armor have been refilled");
			}
		}
	}
}

ScriptRegistrant<DB_Supply_Truck> DB_Supply_Truck_Registrant("DB_Supply_Truck","");

void DB_Damage_Vehicle_Occupants_Area_Killed::Killed(GameObject *obj, GameObject *shooter)
{
	if(Get_Float_Parameter("Damage_Distance")>100)
	{
		//Console_Input(StringClass::getFormattedString("msg bad occupant damage settings - %s",Commands->Get_Preset_Name(obj)));
		Set_Parameters_String(",Fire,150,3.5");
	}
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *dmgobj = Commands->Create_Object("Invisible_Object",pos);
	char prms[512];
	if(shooter)
		sprintf(prms,"%f,%s,%f,%i",Get_Float_Parameter("Occupants_Damage_Amount"),Get_Parameter("Occupants_Damage_Warhead"),Get_Float_Parameter("Damage_Distance"),shooter->Get_ID());
	else
		sprintf(prms,"%f,%s,%f,%i",Get_Float_Parameter("Occupants_Damage_Amount"),Get_Parameter("Occupants_Damage_Warhead"),Get_Float_Parameter("Damage_Distance"),0);
	Attach_Script_Once(dmgobj,"DB_Damage_Vehicle_Occupants_Area_Timer",prms);
}

void DB_Damage_Vehicle_Occupants_Area_Timer::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,10000);
}
void DB_Damage_Vehicle_Occupants_Area_Timer::Timer_Expired(GameObject *obj, int number)
{
	if (number == 10000)
	{
		GameObject *damager = Commands->Find_Object(Get_Int_Parameter("ShooterID"));
		if(damager)
			Damage_All_Objects_Area_By_Team(Get_Float_Parameter("Occupants_Damage_Amount"),Get_Parameter("Occupants_Damage_Warhead"),Commands->Get_Position(obj),Get_Float_Parameter("Damage_Distance"),damager,1,0,2);
		else
			Damage_All_Objects_Area_By_Team(Get_Float_Parameter("Occupants_Damage_Amount"),Get_Parameter("Occupants_Damage_Warhead"),Commands->Get_Position(obj),Get_Float_Parameter("Damage_Distance"),0,1,0,2);
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<DB_Damage_Vehicle_Occupants_Area_Timer> DB_Damage_Vehicle_Occupants_Area_Timer_Registrant("DB_Damage_Vehicle_Occupants_Area_Timer","Occupants_Damage_Amount=1000:float,Occupants_Damage_Warhead=warhead:string,Damage_Distance=1000:float,ShooterID=0:int");
ScriptRegistrant<DB_Damage_Vehicle_Occupants_Area_Killed> DB_Damage_Vehicle_Occupants_Area_Killed_Registrant("DB_Damage_Vehicle_Occupants_Area_Killed","Area_Damaging_Explosion=explosion:string,Occupants_Damage_Warhead=warhead:string,Occupants_Damage_Amount=1000:float,Damage_Distance=1000:float");

void DB_Set_PT_Slot::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.0f,12321);
	int Type=Get_Int_Parameter("Type");
		int Team=Get_Int_Parameter("Team");
		int Slot=Get_Int_Parameter("Slot");
		int Alt=Get_Int_Parameter("Alt");
		StringClass Preset=Get_Parameter("Preset");
		if(stristr(Preset,"mirage"))
		{
			Team=0;
			Type=1;
			Slot=9;
			Alt=0;
		}



		PurchaseSettingsDefClass *PT = PurchaseSettingsDefClass::Find_Definition((PurchaseSettingsDefClass::TYPE)Type,(PurchaseSettingsDefClass::TEAM)Team);
		if(PT)
		{
			int PresetID = Get_Definition_ID(Preset);
			if(PresetID)
			{
				if(Alt==0 || Alt==1 || Alt==2)
				{
					PT->Set_Alt_Definition(Slot,Alt,PresetID);
				}
				else
				{
					PT->Set_Definition(Slot,PresetID);
				}
			}
			else
			{
				if(Alt==0 || Alt==1 || Alt==2)
				{
					PT->Set_Alt_Definition(Slot,Alt,0);
				}
				else
				{
					PT->Set_Definition(Slot,0);
				}
			}
		}

}

void DB_Set_PT_Slot::Timer_Expired(GameObject *obj,int number)
{
#ifdef DRAGONADE
	if(number==12321)
	{
		Console_Input("reload");
	}
#endif // DRAGONADE
}

ScriptRegistrant<DB_Set_PT_Slot> DB_Set_PT_Slot_Registrant("DB_Set_PT_Slot","Team=0:int,Type=0:int,Slot=0:int,Alt=-1:int,Preset=CnC_Nod_Light_Tank:String");

void DB_Spawn_When_Killed::Killed(GameObject *obj,GameObject *killer)
{
	if(Find_Named_Definition(Get_Parameter("PresetName")))
	{
		float facing = Commands->Get_Facing(obj);
		Vector3 position = Commands->Get_Position(obj);
		GameObject *newobj;
		position.Z += Get_Float_Parameter("DropHeight");
		newobj = Commands->Create_Object(Get_Parameter("PresetName"),position);
		if(newobj)
		{
			if (Get_Int_Parameter("SameFacing"))
			{
				Commands->Set_Facing(newobj,facing);
			}
			if (Get_Int_Parameter("MatchObjectTeam"))
			{
				Set_Object_Type(newobj,Get_Object_Type(obj));
			}
			float maxhealth = Commands->Get_Max_Health(newobj);
			float maxarmor = Commands->Get_Max_Shield_Strength(newobj);
			if (maxhealth) Commands->Set_Health(newobj,maxhealth*Get_Float_Parameter("HealthPercent") / 100);
			if (maxarmor) Commands->Set_Shield_Strength(newobj,maxarmor*Get_Float_Parameter("ArmorPercent") / 100);
			Update_Network_Object(newobj);
			Force_Position_Update(newobj);
		#ifdef DRAGONADE
			if(obj->As_SoldierGameObj())
			{
				obj->As_PhysicalGameObj()->Set_Collision_Group(TERRAIN_ONLY_COLLISION_GROUP);
			}
			Fix_Stuck_Objects(position,10,15,false);
		#endif // DRAGONADE
		}
	}
}

void DB_Replace_When_Repaired::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (amount >= 0 || Commands->Get_Health(obj) < Commands->Get_Max_Health(obj) || Commands->Get_Shield_Strength(obj) < Commands->Get_Max_Shield_Strength(obj))
	{
		return;
	}

	if(Find_Named_Definition(Get_Parameter("PresetName")))
	{
		float facing = Commands->Get_Facing(obj);
		Vector3 position = Commands->Get_Position(obj);
		GameObject*newobj;
		Commands->Destroy_Object(obj);
		position.Z += Get_Float_Parameter("DropHeight");
		newobj = Commands->Create_Object(Get_Parameter("PresetName"),position);
		if(newobj)
		{
			if (Get_Int_Parameter("SameFacing"))
			{
				Commands->Set_Facing(newobj,facing);
			}
			if (Get_Int_Parameter("MatchRepairTeam") && damager)
			{
				Set_Object_Type(newobj,Get_Object_Type(damager));
			}
			float maxhealth = Commands->Get_Max_Health(newobj);
			float maxarmor = Commands->Get_Max_Shield_Strength(newobj);
			if (maxhealth) Commands->Set_Health(newobj,maxhealth*Get_Float_Parameter("HealthPercent") / 100);
			if (maxarmor) Commands->Set_Shield_Strength(newobj,maxarmor*Get_Float_Parameter("ArmorPercent") / 100);
			Update_Network_Object(newobj);
			Force_Position_Update(newobj);
		#ifdef DRAGONADE
			Fix_Stuck_Objects(position,10,15,false);
		#endif // DRAGONADE
		}
	}
}

ScriptRegistrant<DB_Spawn_When_Killed> DB_Spawn_When_Killed_Registrant("DB_Spawn_When_Killed","PresetName:string,DropHeight=0:float,SameFacing=1:int,HealthPercent=100.0000:float,ArmorPercent=100.0000:float,MatchObjectTeam=0:int");
ScriptRegistrant<DB_Replace_When_Repaired> DB_Replace_When_Repaired_Registrant("DB_Replace_When_Repaired","PresetName:string,DropHeight=0:float,SameFacing=1:int,HealthPercent=100.0000:float,ArmorPercent=100.0000:float,MatchRepairTeam=0:int");

void DB_Force_Fire::Created(GameObject *obj)
{
	float startDelay = Get_Float_Parameter("StartDelay");
	if(!startDelay)
		Start_Attack(obj);
	else
		Commands->Start_Timer(obj,this,startDelay,2);
}

void DB_Force_Fire::Start_Attack(GameObject *obj)
{
	if(obj && obj->As_SmartGameObj())
	{
		ActionParamsStruct action;
		action.AttackCheckBlocked = false;
		action.AttackForceFire = true;
		action.AttackLocation = obj->As_SmartGameObj()->Get_Targeting_Pos();
		action.AttackRange = 10000000;
		action.Priority = 100;
		action.AttackPrimaryFire = !Get_Int_Parameter("Use_Secondary_Fire");
		Commands->Action_Attack(obj,action);
		float time = Get_Float_Parameter("ResetTime");
		if(time)
			Commands->Start_Timer(obj,this,time,1);
	}
}

void DB_Force_Fire::Timer_Expired(GameObject *obj, int number)
{
	if(number == 1)
		Commands->Action_Reset(obj,101);
	else if(number == 2)
		Start_Attack(obj);
}

void DB_Force_Fire::Detach(GameObject *obj)
{
	if(Exe != 4)
	{
		Commands->Action_Reset(obj,101);
	}
}

ScriptRegistrant<DB_Force_Fire>DB_Force_Fire_Registrant("DB_Force_Fire","UseSecondaryFire=0:int,ResetTime=0:float,StartDelay=0:float");