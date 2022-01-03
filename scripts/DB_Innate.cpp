#include "General.h"
#include "DB_Innate.h"
#include "wwmath.h"
#include "SimplePersistFactoryClass.h"
#include "SoldierGameObj.h"
#include "CombatManager.h"
#include "soldiergameobjdef.h"
#include "WeaponBagClass.h"
#include "Random2Class.h"
#include "GameObjManager.h"
#include "WeaponClass.h"
#include "engine_tt.h"
#include "PhysicsSceneClass.h"
#include "physcoltest.h"

/*
Full replacement for the stock Innate Behavior AI that has many improvements including dealing with
enemies behind cover. It also will move and strafe much more often to avoid enemy shots. It should
work as a drop in replacement for most scripts that rely on the original Innate AI as well.
*/

typedef enum {
	SOLDIER_INNATE_EVENT_ENEMY_SEEN 		=	1 << 0,
	SOLDIER_INNATE_EVENT_FOOTSTEP_HEARD	=	1 << 1,
	SOLDIER_INNATE_EVENT_GUNSHOT_HEARD	=	1 << 2,
	SOLDIER_INNATE_EVENT_BULLET_HEARD 	=	1 << 3,
	SOLDIER_INNATE_ACTIONS					=	1 << 4,
} SoldierInnateEvents;

Vector3 Random_Vector(float f)
{
	float f3 = -f;
	float f4 = f;
	if (f3 > f)
	{
		f4 = -f;
		f3 = f;
	}
	float f5 = -f;
	float f6 = f;
	if (f5 > f)
	{
		f6 = -f;
		f5 = f;
	}
	return Vector3(FreeRandom.Get_Float(f5,f6),FreeRandom.Get_Float(f3,f4),0);

}

//Check if the position has a vantage point of the enemy object. Used to determine if it should select this location
bool Can_Position_Target_Object(PhysicalGameObj* attacker,Vector3 attackPosition,PhysicalGameObj* target)
{
	const Vector3 bullseyePosition = target->Get_Bullseye_Position();
	attackPosition.Z+=1.0;
	
	CastResultStruct castResult;
	PhysRayCollisionTestClass collisionTest(LineSegClass(attackPosition, bullseyePosition), &castResult, BULLET_COLLISION_GROUP);
	
	target->Peek_Physical_Object()->Inc_Ignore_Counter();
	PhysicsSceneClass::Get_Instance()->Cast_Ray(collisionTest, false);
	target->Peek_Physical_Object()->Dec_Ignore_Counter();
	
	return
		castResult.Fraction == 1.f ||
		collisionTest.CollidedPhysObj == target->Peek_Physical_Object();
}

DB_Innate_Soldier::DB_Innate_Soldier() : State(1), StateTimer(0), HomeFacing(9999999), ActTime(0), UseWeapon(0), Aggressiveness(0.5), TakeCoverProbability(0.5), IsStationary(false), LastState(0), Weapon(0), ConversationTimer(((WWMath::Random_Float() * 9.0f) + 1.0f))
{
}
DB_Innate_Soldier::~DB_Innate_Soldier()
{
	Destroyed(0);
	Target = 0;
}

bool DB_Innate_Soldier::Save(ChunkSaveClass &csave)
{
	csave.Begin_Chunk(410001836);
	csave.End_Chunk();
		csave.Begin_Chunk(410001837);
		WRITE_MICRO_CHUNK(csave,1,State);
		WRITE_MICRO_CHUNK(csave,2,StateTimer);
		WRITE_MICRO_CHUNK(csave,3,HomeLocation);
		WRITE_MICRO_CHUNK(csave,10,HomeFacing);
		WRITE_MICRO_CHUNK(csave,4,Position);
		WRITE_MICRO_CHUNK(csave,5,ActTime);
		WRITE_MICRO_CHUNK(csave,9,Cover);
		WRITE_MICRO_CHUNK(csave,12,ConversationTimer);
		WRITE_MICRO_CHUNK(csave,13,UseWeapon);
		WRITE_MICRO_CHUNK(csave,14,Aggressiveness);
		WRITE_MICRO_CHUNK(csave,15,TakeCoverProbability);
		WRITE_MICRO_CHUNK(csave,16,IsStationary);
		WRITE_MICRO_CHUNK(csave,17,Weapon);
		csave.End_Chunk();
		if (Target)
		{
			csave.Begin_Chunk(410001838);
			Target->Save(csave);
			csave.End_Chunk();
		}
	return true;
}
bool DB_Innate_Soldier::Load(ChunkLoadClass &cload)
{
	while (cload.Open_Chunk()) {
		switch(cload.Cur_Chunk_ID()) {
		case 410001837:
			while (cload.Open_Micro_Chunk()) {
				switch(cload.Cur_Micro_Chunk_ID()) {
					READ_MICRO_CHUNK(cload,1,State);
					READ_MICRO_CHUNK(cload,2,StateTimer);
					READ_MICRO_CHUNK(cload,3,HomeLocation);
					READ_MICRO_CHUNK(cload,10,HomeFacing);
					READ_MICRO_CHUNK(cload,4,Position);
					READ_MICRO_CHUNK(cload,5,ActTime);
					READ_MICRO_CHUNK(cload,9,Cover);
					READ_MICRO_CHUNK(cload,12,ConversationTimer);
					READ_MICRO_CHUNK(cload,13,UseWeapon);
					READ_MICRO_CHUNK(cload,14,Aggressiveness);
					READ_MICRO_CHUNK(cload,15,TakeCoverProbability);
					READ_MICRO_CHUNK(cload,16,IsStationary);
					READ_MICRO_CHUNK(cload,17,Weapon);
				}
				cload.Close_Micro_Chunk();
			}
			break;
		case 410001838:
			Target->Load(cload);
			break;
		}
		cload.Close_Chunk();
	}
	return true;
}

char *SoldierAIStateNames[6] = {"Relaxed Idle","Alert Idle","Footsteps Heard","Bullet Heard","Gunshot Heard","Enemy Seen"};
void DB_Innate_Soldier::Created(GameObject *obj)
{
	Debug = Get_Int_Parameter("Debug")!=0;
	if(!obj->As_SoldierGameObj())
	{
		Destroy_Script();
		return;
	}
	SmartGameObj *Smart = obj->As_SmartGameObj();
	Smart->Get_Position(&HomeLocation);
	Smart->Set_Enemy_Seen_Enabled(true);
	Commands->Start_Timer(obj,this, 1.0f, 100123);
	Commands->Innate_Enable(obj);
	Commands->Innate_Soldier_Enable_Bullet_Heard(obj,true);
	Commands->Innate_Soldier_Enable_Gunshot_Heard(obj,true);
	Commands->Innate_Soldier_Enable_Footsteps_Heard(obj,true);
	SoldierGameObj *Soldier = Smart->As_SoldierGameObj();
	if(Soldier)
	{
		Aggressiveness =  Get_Float_Parameter("Aggressiveness");
		float HomeRange =  Get_Float_Parameter("HomeRange");
		if(HomeRange > 0)
		{
			Set_Home_Location(Soldier->Get_Position(),HomeRange);
		}
		IsStationary = (int)Get_Int_Parameter("Stationary")!=0;
		GameObjObserverClass *InnateObserver = Soldier->Find_Observer("Innate Soldier");
		if(InnateObserver)
		{
			Soldier->Remove_Observer(InnateObserver);
			Soldier->Set_Innate_Observer(NULL);
		}
	}
}
void DB_Innate_Soldier::Destroyed(GameObject *obj)
{
	;
}
void DB_Innate_Soldier::Timer_Expired(GameObject *obj,int number)
{
	if (number == 100123)
	{
		Commands->Start_Timer(obj, this, 1.0f, 100123);
		Think_Function(obj->As_SoldierGameObj(),false);
	}
}
void DB_Innate_Soldier::Killed(GameObject *obj,GameObject *killer)
{
}
void DB_Innate_Soldier::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (!obj->Is_Hibernating())
	{
		SoldierGameObj *sobj = obj->As_SoldierGameObj();
		if (sobj->Is_Innate_Enabled(SOLDIER_INNATE_EVENT_BULLET_HEARD))
		{
			Vector3 position;
			sobj->Get_Position(&position);
			if (damager)// && FreeRandom.Get_Float() <0.8f)
			{
				damager->Get_Position(&position);
				if (sobj->Is_Enemy(damager->As_DamageableGameObj()) && damager!=Target && Commands->Get_Health(damager)>0)
					Enemy_Seen(obj,damager);
					//Set_State(sobj, GUNSHOT_HEARD, position, 0);
				else if(State < BULLET_HEARD)
					Set_State(sobj, BULLET_HEARD, position, 0);
			}
		}
		if (State == ENEMY_SEEN)
		{
			ScriptableGameObj *target = Target;
			if (damager && target != damager)
			{
				Vector3 position1;
				sobj->Get_Position(&position1);
				float f1 = 1000000;
				if (target)
				{
					Vector3 position2;
					target->Get_Position(&position2);
					f1 = (position2 - position1).Length();
				}
				Vector3 position3;
				damager->Get_Position(&position3);
				if ((position3 - position1).Length() < f1)
				{
					PhysicalGameObj *pobj = damager->As_PhysicalGameObj();
					if (pobj)
					{
						if (pobj->Is_Enemy(sobj))
						{
							Target = damager;
							ActTime = 0;
						}
					}
				}
			}
		}
	}
}
void DB_Innate_Soldier::Sound_Heard(GameObject *obj,const CombatSound & sound)
{
	if (!obj->Is_Hibernating())
	{
		SoldierGameObj *sobj = obj->As_SoldierGameObj();
		PhysicalGameObj *pobj = 0;
		if (sound.Creator)
		{
			pobj = sound.Creator->As_PhysicalGameObj();
		}
		if (sound.sound == SOUND_TYPE_FOOTSTEPS)
		{
			if (pobj && (sobj->Is_Teammate(pobj) || !sobj->Is_Innate_Enabled(SOLDIER_INNATE_EVENT_FOOTSTEP_HEARD)))
			{
				return;
			}
			if (Set_State(sobj,FOOTSTEPS_HEARD,sound.Position,0))
			{
				Notify_Neighbors_Sound(sobj,sound);
			}
			return;
		}
		if (sound.sound == SOUND_TYPE_GUNSHOT)
		{
			if (pobj && (sobj->Is_Teammate(pobj) || !sobj->Is_Innate_Enabled(SOLDIER_INNATE_EVENT_GUNSHOT_HEARD)))
			{
				return;
			}
			if (Set_State(sobj,GUNSHOT_HEARD,sound.Position,0))
			{
				Notify_Neighbors_Sound(sobj,sound);
			}
			return;
		}
		if (sound.sound == SOUND_TYPE_BULLET_HIT)
		{
			if (!sobj->Is_Innate_Enabled(SOLDIER_INNATE_EVENT_BULLET_HEARD))
			{
				return;
			}
			Vector3 pos = sound.Position;
			if (pobj && FreeRandom.Get_Float() < 0.7)
			{
				pobj->Get_Position(&pos);
			}
			if (Set_State(sobj,BULLET_HEARD,pos,0))
			{
				Notify_Neighbors_Sound(sobj,sound);
			}
		}
	}
}
void DB_Innate_Soldier::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	if (!obj->Is_Hibernating())
	{
		PhysicalGameObj *pobj = obj->As_PhysicalGameObj();
		if (pobj)
		{
			if (pobj->Get_Defense_Object()->Get_Health() > 0)
			{
				if((ScriptableGameObj*)Target && Commands->Get_Health(Target)<=0)
					Target = 0;
				ScriptableGameObj *target = Target;
				Vector3 targetpos;
				Vector3 seenpos;
				Vector3 objpos;

				if (!target || target == enemy || (target->Get_Position(&targetpos),enemy->Get_Position(&seenpos),obj->Get_Position(&objpos),((seenpos - objpos).Length2() <= (targetpos - objpos).Length2())))
				{
					SoldierGameObj *sobj = obj->As_SoldierGameObj();
					bool state = false;
					if (sobj->Is_Innate_Enabled(1))
					{
						state = Set_State(sobj,ENEMY_SEEN,Vector3(0,0,0),enemy);
					}
					if (state)
					{
						Notify_Neighbors_Enemy(sobj,enemy);
					}
				}
			}
		}
	}
}
void DB_Innate_Soldier::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
}
void DB_Innate_Soldier::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	if (action_id == 9000000)
	{
		SubState += "\nComplete";
		ScriptableGameObj *target = Target;
		if (target && State == ENEMY_SEEN && ((DamageableGameObj *)target)->Get_Defense_Object()->Get_Health() <= 0)
		{
			StateTimer = 100000;
			Target = 0;
		}
		if (complete_reason && complete_reason != ACTION_COMPLETE_PATH_BAD_START)
		{
			if (complete_reason != ACTION_COMPLETE_PATH_BAD_DEST)
			{
				return;
			}
		}
		else
		{
			if (complete_reason != ACTION_COMPLETE_PATH_BAD_DEST)
			{
				float f1 = WWMath::Random_Float() + 2.0f;
				float f2 = ActTime;
				if (f1 <= f2)
				{
					f2 = WWMath::Random_Float() + 2.0f;
				}
				ActTime = f2;
				if (State > BULLET_HEARD)
				{
					StateTimer = 100000;
				}
			}
		}
		float f1 = WWMath::Random_Float() + 1.0f;
		float f2 = ActTime;
		if (f1 <= f2)
		{
			f2 = WWMath::Random_Float() + 1.0f;
		}
		ActTime = f2;
		if (State > BULLET_HEARD)
		{
			StateTimer = 100000;
		}
	}
}
void DB_Innate_Soldier::State_Changed(SoldierGameObj *obj)
{
	if (State <= ENEMY_SEEN)
	{
		switch (State)
		{
		case RELAXED_IDLE:
			UseWeapon = false;
			break;
		case ALERT_IDLE:
		case BULLET_HEARD:
		case GUNSHOT_HEARD:
		case ENEMY_SEEN:
			UseWeapon = true;
			break;
		case FOOTSTEPS_HEARD:
			break;
		}
	}
	if (obj->Get_Weapon_Bag()->Get_Index())
	{
		Weapon = obj->Get_Weapon_Bag()->Get_Weapon()->Get_Name();
	}
	if (UseWeapon)
	{
		Commands->Select_Weapon(obj,Weapon);
	}
	else
	{
		Commands->Select_Weapon(obj,"");
	}
	ActTime = 0;
}

int StatePriorities[6] = {10,20,50,50,70,90};
bool DB_Innate_Soldier::Set_State(SoldierGameObj *obj, int state, Vector3 const &pos, ScriptableGameObj *target)
{
	LastState = state;
	int oldstate = State;
	
	if (StatePriorities[oldstate] < StatePriorities[state])
	{
		Position = pos;
		/*
		if (State != state)
		{
			State = state;
			State_Changed(obj);
		}
		*/
	}
	
	if (StatePriorities[State] <= StatePriorities[state])
	{
		if (State != state)
		{
			if(Debug)
				Send_Message(225,255,255,StringClass::getFormattedString("%s -> %s",SoldierAIStateNames[State],SoldierAIStateNames[state]));

			State = state;
			State_Changed(obj);
		}
		StateTimer = 0;
		if (((State == BULLET_HEARD) || (State == GUNSHOT_HEARD) || (State == ENEMY_SEEN && state < ENEMY_SEEN)) && ActTime > 1)
		{
			ActTime = 1;
		}
		Vector3 position;
		obj->Get_Position(&position);
		if ((Position - position).Length2() > (pos - position).Length2())
		{
			Position = pos;
		}
		PhysicalGameObj *pobj = 0;
		if (target)
		{
			pobj = target->As_PhysicalGameObj();
		}
		Target = pobj;
		Think_Function(obj,State != oldstate);
	}
	if (oldstate != State)
	{
		if (state == ENEMY_SEEN)
		{
			ConversationTimer = WWMath::Random_Float() * 25.0f + 5.0f;
		}
		else
		{
			ConversationTimer = WWMath::Random_Float() * 40.0f + 20.0f;
		}
	}
	if (oldstate && oldstate != ALERT_IDLE)
	{
		if (oldstate == ENEMY_SEEN)
		{
			if (State == FOOTSTEPS_HEARD || State == BULLET_HEARD || State == GUNSHOT_HEARD)
			{
				//obj->Say_Dialogue(DIALOG_ON_COMBAT_TO_SEARCH);
			}
		}
		else if (State == ENEMY_SEEN)
		{
			//obj->Say_Dialogue(DIALOG_ON_SEARCH_TO_COMBAT);
		}
	}
	else
	{
		if (State == FOOTSTEPS_HEARD || State == BULLET_HEARD || State == GUNSHOT_HEARD)
		{
			//obj->Say_Dialogue(DIALOG_ON_IDLE_TO_SEARCH);
		}
		if (State == ENEMY_SEEN)
		{
			//obj->Say_Dialogue(DIALOG_ON_IDLE_TO_COMBAT);
		}
	}
	return oldstate != State;
}
void DB_Innate_Soldier::Notify_Neighbors_Sound(SoldierGameObj *obj, CombatSound const &sound)
{
	Vector3 pos;
	obj->Get_Position(&pos);
	for (SLNode<SmartGameObj> *z = GameObjManager::SmartGameObjList.Head();z;z = z->Next())
	{
		SmartGameObj* o = z->Data();
		if (o)
		{
			if (o != obj)
			{
#pragma warning(suppress: 6011)
				if (o->Get_Player_Type() == obj->Get_Player_Type())
				{
					Vector3 pos2;
					o->Get_Position(&pos2);
					pos2 = pos2 - pos;
					if (pos2.Length() <= 10)
					{
						const SimpleDynVecClass<GameObjObserverClass *> & observer_list = o->Get_Observers();
						for( int index = 0; index < observer_list.Count(); index++ )
						{
							observer_list[ index ]->Sound_Heard(o,sound);
						}
					}
				}
			}
		}
	}
}
void DB_Innate_Soldier::Notify_Neighbors_Enemy(SoldierGameObj *obj, ScriptableGameObj *enemy)
{
	Vector3 pos;
	obj->Get_Position(&pos);
	for (SLNode<SmartGameObj> *z = GameObjManager::SmartGameObjList.Head();z;z = z->Next())
	{
		SmartGameObj* o = z->Data();
		if (o)
		{
			if (o != obj)
			{
#pragma warning(suppress: 6011)
				if (o->Get_Player_Type() == obj->Get_Player_Type())
				{
					Vector3 pos2;
					o->Get_Position(&pos2);
					pos2 = pos2 - pos;
					if (pos2.Length() <= 10)
					{
						const SimpleDynVecClass<GameObjObserverClass *> & observer_list = o->Get_Observers();
						for( int index = 0; index < observer_list.Count(); index++ )
						{
							observer_list[ index ]->Enemy_Seen(o,enemy);
						}
					}
				}
			}
		}
	}
}
void DB_Innate_Soldier::Poked(GameObject *obj,GameObject *poker)
{
	if (obj)
	{
		PhysicalGameObj *pobj = obj->As_PhysicalGameObj();
		if (pobj)
		{
			SoldierGameObj *sobj = pobj->As_SoldierGameObj();
			if (sobj)
			{
				bool face = false;
				if (State <= ENEMY_SEEN)
				{
					switch(State)
					{
					case RELAXED_IDLE:
					case ALERT_IDLE:
						//sobj->Say_Dialogue(DIALOG_ON_POKE_IDLE);
						face = true;
						break;
					case FOOTSTEPS_HEARD:
					case BULLET_HEARD:
					case GUNSHOT_HEARD:
						//sobj->Say_Dialogue(DIALOG_ON_POKE_SEARCH);
						face = true;
						break;
					case ENEMY_SEEN:
						//sobj->Say_Dialogue(DIALOG_ON_POKE_COMBAT);
						break;
					}
				}
				float duration = WWMath::Random_Float() * 1.5f + 0.5f + (float)2.0f;
				PhysicalGameObj *pobj2 = poker->As_PhysicalGameObj();
				if (pobj2)
				{
					Vector3 pos = pobj2->Get_Bullseye_Position();
					if (face)
					{
						ActionParamsStruct params;
						params.Set_Basic(Get_ID(),20,0);
						params.Set_Face_Location(pos,duration);
						Commands->Action_Face_Location(obj,params);
					}
				}
			}
		}
	}
}
struct StateDataStr
{
	float timer;
	int state;
};
StateDataStr StateData[6] = {{9999,0},{180,0},{6,1},{20,1},{4,3},{60,1}};
void DB_Innate_Soldier::Think_Function(SoldierGameObj *obj, bool b)
{
	StateTimer += 1;
	if (StateTimer >= StateData[State].timer)
	{
		Action_Reset(obj);
		if(Debug)
			Send_Message(225,255,255,StringClass::getFormattedString("%s -> %s",SoldierAIStateNames[State],SoldierAIStateNames[StateData[State].state]));
		State = StateData[State].state;
		State_Changed(obj);
		StateTimer = 0;
	}
	if (obj->Is_Innate_Enabled(0x10))
	{
		ActTime -= 1;
		if (ActTime <= 0 && !obj->Is_In_Elevator() && !obj->Is_On_Ladder())
		{
			State_Act(obj,b);
		}
	}
	if (!obj->Is_Human_Controlled() && (!obj->Is_In_Conversation()))
	{
		ConversationTimer -= 1;
		if (ConversationTimer <= 0)
		{
			Reset_Conversation_Timer();
		}
	}
}

void DB_Innate_Soldier::Get_Information(StringClass &string)
{
	StringClass temp;
	string += "-----------\n";
	temp.Format("State: %s\n",SoldierAIStateNames[State]);
	string += temp;
	temp.Format("Sub: %s\n",SubState);
	string += temp;
	if (IsStationary)
	{
		string += "Stationary\n";
	}
	if (State == ENEMY_SEEN)
	{
		int id = 0;
		if (Target)
		{
			id = Target->Get_Network_ID();
		}
		temp.Format("Attacking: %d\n",id);
		string += temp;
	}
	temp.Format("Last: %s\n",SoldierAIStateNames[LastState]);
	string += temp;
	temp.Format("Act Time: %f\n",ActTime);
	string += temp;
}
void DB_Innate_Soldier::Reset_Conversation_Timer()
{
	ConversationTimer = WWMath::Random_Float() * 40.0f + 20.0f;
}
void DB_Innate_Soldier::State_Act(SoldierGameObj *obj, bool b)
{
	if (State <= ENEMY_SEEN)
	{
		switch(State)
		{
		case RELAXED_IDLE:
			State_Act_Idle(obj);
			break;
		case ALERT_IDLE:
			State_Act_Idle(obj);
			break;
		case BULLET_HEARD:
			State_Act_Bullet_Heard(obj,b);
			break;
		case FOOTSTEPS_HEARD:
			State_Act_Footsteps_Heard(obj);
			break;
		case GUNSHOT_HEARD:
			State_Act_Gunshot_Heard(obj);
			break;
		case ENEMY_SEEN:
			State_Act_Attack(obj);
			break;
		}
	}
}
void DB_Innate_Soldier::State_Act_Idle(SoldierGameObj *obj)
{
	ActTime = 5;
	Vector3 pos;
	obj->Get_Position(&pos);
	if (FreeRandom.Get_Float() < 0.5f)
	{
		SubState = "Facing Random";
		Vector3 rnd = Random_Vector(1.0f);
		pos = pos + rnd;
		Action_Face_Location(obj,pos,AI_STATE_IDLE,0);
		ActTime = FreeRandom.Get_Float(4.0f,6.0f);
	}
	if (!IsStationary)
	{
		SubState = "Random Walk";
		if (Get_Random_Pathfind_Spot(pos,6,&pos))
		{
			Action_Goto_Location(obj,pos,AI_STATE_IDLE,0.3f,0.3f,0);
		}
		ActTime = FreeRandom.Get_Float(7.0f,10.0f);
	}
}
void DB_Innate_Soldier::State_Act_Footsteps_Heard(SoldierGameObj *obj)
{
	ActTime = FreeRandom.Get_Float(3.0f,4.0f);
	bool move = false;
	if (Aggressiveness > FreeRandom.Get_Float() && !IsStationary)
	{
		SubState = "Walk To Footsteps";
		Vector3 pos;
		if (Get_Random_Pathfind_Spot(Position,2,&pos))
		{
			Action_Goto_Location(obj,pos,AI_STATE_IDLE,0.3f,0.5f,0);
			move = true;
		}
	}
	if (!move)
	{
		SubState = "Face Footsteps";
		Action_Face_Location(obj,Position,AI_STATE_SEARCH,0);
	}
}
void DB_Innate_Soldier::State_Act_Bullet_Heard(SoldierGameObj *obj, bool b)
{
	ActTime = FreeRandom.Get_Float(5.0f,10.0f);
	ActTime = FreeRandom.Get_Float(4.0f,2.0f);
	obj->Get_Human_State()->Raise_Weapon();
	Vector3 pos;
	obj->Get_Position(&pos);
	Vector3 pos2 = pos - Position;
	float f = WWMath::Clamp((1.0f - (pos2.Length() - 10.0f) / 10.0f));
	bool b2 = false;
	if (b && f > FreeRandom.Get_Float() && !IsStationary)
	{
		b2 = true;
	}
	if (b2)
	{
		SubState = "Dive Away";
		ActTime = FreeRandom.Get_Float(2.0f,4.0f);
		pos2 = (pos * 2) - Position;
		Action_Dive(obj,pos2);
	}
	else
	{

		if (TakeCoverProbability > FreeRandom.Get_Float() || IsStationary)
		{
			SubState = "Face Random Crouched";
			pos2 = pos + Random_Vector(1.0f);
			Action_Face_Location(obj,pos2,AI_STATE_IDLE,1);
		}
		else
		{
			SubState = "Crouch Walk To";
			if (Get_Random_Pathfind_Spot(Position,6,&pos2))
			{
				Action_Goto_Location(obj,pos2,AI_STATE_SEARCH,0.89999998f,2.0f,1);
			}
		}
	}
}
void DB_Innate_Soldier::State_Act_Gunshot_Heard(SoldierGameObj *obj)
{
	obj->Get_Human_State()->Raise_Weapon();
	ActTime = FreeRandom.Get_Float(5.0f,10.0f);
	if (IsStationary)
	{
		SubState = "Face gunshot";
		Action_Face_Location(obj,Position,AI_STATE_SEARCH,0);
	}
	else
	{
		SubState = "Run To Random";
		Vector3 pos = Position;
		if (Get_Random_Pathfind_Spot(Position,6,&pos))
		{
			Action_Goto_Location_Facing(obj,pos,AI_STATE_SEARCH,Position,0.89999998f,2.0f,0);
		}
		else
		{
			Vector3 pos2;
			obj->Get_Position(&pos2);
			if (Get_Random_Pathfind_Spot(pos2,6,&pos))
			{
				Action_Goto_Location_Facing(obj,pos,AI_STATE_SEARCH,Position,0.30000001f,0.5f,0);
			}
		}
	}
}
void DB_Innate_Soldier::State_Act_Attack(SoldierGameObj *obj)
{
	obj->Get_Human_State()->Raise_Weapon();
	ActTime = FreeRandom.Get_Float(5.0f,8.0f);
	ScriptableGameObj *target = Target;
	if (!target || !Target->As_DamageableGameObj()->Is_Enemy(obj))
	{
		StateTimer = 100000;
		return;
	}
	Vector3 pos;
	target->Get_Position(&pos);
	float range = 60;
	if (obj->Get_Weapon())
	{
		range = obj->Get_Weapon()->Get_Range();
	}
	Vector3 pos2;
	obj->Get_Position(&pos2);

	float er = 20;
	if (obj->Get_Weapon())
	{
		er = obj->Get_Weapon()->Get_Effective_Range();
	}
	
	Vector3 pos3 = pos;
	float val = 100000;
	int count = 0;
	int positionCount = 0;
	if(!IsStationary)
	{
		//Select a random spot and then check if it will have a clear shot of the enemy object from that position.
		//Do it up to 512 times and stop when it has 4 valid locations. Select the one closest to effective range.
		do
		{
			Vector3 pos4 = pos2;
			if (Get_Random_Pathfind_Spot(pos,(er+range)/2,&pos4))
			{
				float len = (pos4 - pos).Length() - er;
				if (val > WWMath::Fabs(len) && Can_Position_Target_Object(obj,pos4,(PhysicalGameObj *)target))
				{
					val = WWMath::Fabs(len);
					pos3 = pos4;
					positionCount++;
				}
			}
			count++;
		} while (count < 512 && positionCount < 4);
	}
	else
	{
		pos3 = pos2;
	}
	SubState = "Random Run Attack";
	ActTime = 3;
	Action_Attack_Object(obj,(PhysicalGameObj *)target,range,(1.0f - Aggressiveness) > FreeRandom.Get_Float()*5.0f,pos3,0.5f);
}
void DB_Innate_Soldier::Action_Reset(SoldierGameObj *obj)
{
	Commands->Action_Reset(obj,(float)StatePriorities[State]);
}
void DB_Innate_Soldier::Action_Face_Location(SoldierGameObj *obj, Vector3 const &position, SoldierAIState aistate, bool crouched)
{
	ActionParamsStruct params;
	params.Set_Basic(Get_ID(),(float)StatePriorities[State],9000000,aistate);
	params.Set_Face_Location(position,2);
	params.MoveCrouched = crouched;
	Commands->Action_Face_Location(obj,params);
}
void DB_Innate_Soldier::Action_Goto_Location(SoldierGameObj *obj, Vector3 const &position, SoldierAIState aistate, float speed, float distance, bool crouched)
{
	Vector3 pos = position;
	Stay_Within_Home(obj,&pos,&distance);
	ActionParamsStruct params;
	params.Set_Basic(Get_ID(),(float)StatePriorities[State],9000000,aistate);
	params.Set_Movement(position,speed,distance,crouched);
	Commands->Action_Goto(obj,params);
}
void DB_Innate_Soldier::Action_Goto_Location_Facing(SoldierGameObj *obj, Vector3 const &position, SoldierAIState aistate, Vector3 const &position2, float speed, float distance, bool crouched)
{
	Vector3 pos = position;
	Stay_Within_Home(obj,&pos,&distance);
	ActionParamsStruct params;
	params.Set_Basic(Get_ID(),(float)StatePriorities[State],9000000,aistate);
	params.Set_Movement(position,speed,distance,crouched);
	params.ForceFacing = true;
	params.FaceLocation = position2;
	Commands->Action_Goto(obj,params);
}
void DB_Innate_Soldier::Action_Attack_Object(SoldierGameObj *obj, PhysicalGameObj *target, float range, bool crouched, Vector3 const &position, float distance)
{
	Vector3 pos = position;
	Stay_Within_Home(obj,&pos,&distance);
	if (IsStationary)
	{
		distance = 10000;
	}
	if (target && obj->Get_Weapon())
	{
		ActionParamsStruct params;
		params.Set_Basic(Get_ID(),(float)StatePriorities[State],9000000,AI_STATE_COMBAT);
		params.Set_Movement(pos,0.89999998f,distance,crouched);
		params.Set_Attack(target,range,FreeRandom.Get_Float() * 2.0f,true);
		params.AttackCrouched = crouched;
		params.AttackWanderAllowed = IsStationary ^ 1;
		if (FreeRandom.Get_Float() < 0.5f)
		{
			params.LookLocation = pos + Vector3(0,0,1);
			params.LookDuration = FreeRandom.Get_Float(0.30000001f,1.0f);
		}
		Commands->Action_Attack(obj,params);
	}
}
void DB_Innate_Soldier::Action_Dive(SoldierGameObj *obj, Vector3 const &position)
{
	ActionParamsStruct params;
	params.Set_Basic(Get_ID(),(float)StatePriorities[State],9000000,AI_STATE_SEARCH);
	params.MoveLocation = position;
	params.MoveCrouched = true;
	params.MoveArrivedDistance = 0.25f;
	Commands->Action_Goto(obj,params);
}
void DB_Innate_Soldier::Stay_Within_Home(SoldierGameObj *obj, Vector3 *position, float *distance)
{
	Vector3 pos;
	obj->Get_Position(&pos);
	pos.X = pos.X - HomeLocation.X;
	pos.Y = pos.Y - HomeLocation.Y;
	pos.Z = 0;
	if (pos.Length() > HomeFacing)
	{
		*position = HomeLocation;
		*distance = HomeFacing;
	}
}

ScriptRegistrant<DB_Innate_Soldier> DB_Innate_Soldier_Registrant("DB_Innate_Soldier","Aggressiveness=0.5:float,HomeRange=0:Float,Stationary=0:int,Debug=0:int");

