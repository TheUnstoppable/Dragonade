#ifndef TT_INCLUDE__DB_Innate_Soldier_H
#define TT_INCLUDE__DB_Innate_Soldier_H



#include "ReferencerClass.h"
#include "engine_string.h"
#include "SoldierGameObj.h"


enum SoldierAIStates
{
	RELAXED_IDLE = 0,
	ALERT_IDLE = 1,
	FOOTSTEPS_HEARD = 2,
	BULLET_HEARD = 3,
	GUNSHOT_HEARD = 4,
	ENEMY_SEEN = 5
};


/*
Full replacement for the stock Innate Behavior AI that has many improvements including dealing with
enemies behind cover. It also will move and strafe much more often to avoid enemy shots. It should
work as a drop in replacement for most scripts that rely on the original Innate AI as well.
*/
class DB_Innate_Soldier :
	public ScriptImpClass
{
	int State; //10
	float StateTimer; //14
	Vector3 HomeLocation; //18 1C 20
	float HomeFacing; //24
	Vector3 Position; //28 2C 30
	float ActTime; //34
	ReferencerClass Target; //38 3C 40 44
	bool Cover; //48
	float ConversationTimer; //50
	bool UseWeapon; //54
	float Aggressiveness; //58
	float TakeCoverProbability; //5C
	bool IsStationary; //60
	StringClass SubState; //64
	int LastState; //68
	const char *Weapon; //6C
	bool Debug;
public:
	DB_Innate_Soldier();
	~DB_Innate_Soldier();
	bool Save(ChunkSaveClass &csave);
	bool Load(ChunkLoadClass &cload);
	virtual const char *Get_Name()
	{
		return "DB_Innate_Soldier";
	}
	virtual void Created(GameObject *obj);
	virtual void Destroyed(GameObject *obj);
	virtual void Killed(GameObject *obj,GameObject *killer);
	virtual void Damaged(GameObject *obj,GameObject *damager,float amount);
	virtual void Custom(GameObject *obj,int type,int param,GameObject *sender);
	virtual void Sound_Heard(GameObject *obj,const CombatSound & sound);
	virtual void Enemy_Seen(GameObject *obj,GameObject *enemy);
	virtual void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
	virtual void Timer_Expired(GameObject *obj,int number);
	virtual void Animation_Complete(GameObject *obj,const char *animation_name)
	{
	}
	virtual void Poked(GameObject *obj,GameObject *poker);
	virtual void Entered(GameObject *obj,GameObject *enterer)
	{
	}
	virtual void Exited(GameObject *obj,GameObject *exiter)
	{
	}
	void Action_Attack_Object(SoldierGameObj *, PhysicalGameObj *, float, bool, Vector3  const&, float);
	void Action_Dive(SoldierGameObj *, Vector3  const&);
	void Action_Face_Location(SoldierGameObj *, Vector3  const&, SoldierAIState, bool);
	void Action_Goto_Location(SoldierGameObj *, Vector3  const&, SoldierAIState, float, float, bool);
	void Action_Goto_Location_Facing(SoldierGameObj *, Vector3  const&, SoldierAIState, Vector3  const&, float, float, bool);
	void Action_Reset(SoldierGameObj *);
	void Get_Information(StringClass &);
	void Notify_Neighbors_Sound(SoldierGameObj *, CombatSound  const&);
	void Notify_Neighbors_Enemy(SoldierGameObj *, ScriptableGameObj *);
	void Reset_Conversation_Timer();
	bool Set_State(SoldierGameObj *, int, Vector3  const&, ScriptableGameObj *);
	void State_Act(SoldierGameObj *, bool);
	void State_Act_Idle(SoldierGameObj *);
	void State_Act_Footsteps_Heard(SoldierGameObj *);
	void State_Act_Bullet_Heard(SoldierGameObj *, bool);
	void State_Act_Gunshot_Heard(SoldierGameObj *);
	void State_Act_Attack(SoldierGameObj *);
	void State_Changed(SoldierGameObj *);
	void Stay_Within_Home(SoldierGameObj *, Vector3 *, float *);
	void Think_Function(SoldierGameObj *, bool);
	void Set_Home_Location(Vector3 const &pos, float facing)
	{
		HomeLocation = pos;
		HomeFacing = facing;
	}
	void Set_Aggressiveness(float f)
	{
		Aggressiveness = f;
	}
	void Set_Take_Cover_Probability(float f)
	{
		TakeCoverProbability = f;
	}
	void Set_Is_Stationary(bool b)
	{
		IsStationary = b;
	}
}; // 0070

#endif