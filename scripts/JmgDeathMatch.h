
#pragma once
#define PI 3.14159265f
#define PI180 PI/180
#define FORCELOOKATLOCATIONCUSTOM 6531234

class JMG_DeathMatch_Game_Control : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void EnableSpawners(const char *name);
};

class JMG_DeathMatch_Spawn_Character : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_DeathMatch_Player : public ScriptImpClass {
	int playerId;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class RP2DeathMatchSpawnSystem
{
public:
	struct RP2DeathMatchSpawnNode
	{
		int id;
		unsigned long lastSeen;
		GameObject *obj;
		Vector3 pos;
		struct RP2DeathMatchSpawnNode *next;
		RP2DeathMatchSpawnNode(GameObject *obj)
		{
			this->id = Commands->Get_ID(obj);
			this->obj = obj;
			this->pos = Commands->Get_Position(obj);
			Vector3 temppos = this->pos;
			temppos.Z += 1.9f;
			Commands->Set_Position(this->obj,temppos);
			this->lastSeen = Commands->Get_Random_Int(0,10);
			this->next = NULL;
		}
	};
private:
	struct RP2DeathMatchSpawnNode *RP2DeathMatchSpawnNodeList;
	float FindNearestPlayer(const Vector3 &pos,int ignorePlayer)
	{
		float nearest = -1.0f;
		for (int x = 1;x < 128;x++)
		{
			if (x == ignorePlayer)
				continue;
			GameObject *player = Get_GameObj(x);
			if (!player)
				continue;
			float tempDist = JmgUtility::SimpleDistance(pos,Commands->Get_Position(player));
			if (nearest < 0 || tempDist < nearest)
				nearest = tempDist;
		}
		return nearest;
	}
public:
	RP2DeathMatchSpawnSystem()
	{
		RP2DeathMatchSpawnNodeList = NULL;
	}
	RP2DeathMatchSpawnNode *AddNode(GameObject *obj)
	{
		int id = Commands->Get_ID(obj);
		RP2DeathMatchSpawnNode *Current = RP2DeathMatchSpawnNodeList;
		if (!RP2DeathMatchSpawnNodeList)
			return (RP2DeathMatchSpawnNodeList = new RP2DeathMatchSpawnNode(obj));
		while (Current)
		{
			if (Current->id == id)
				return Current;
			if (!Current->next)
			{
				Current->next = new RP2DeathMatchSpawnNode(obj);
				return Current->next;
			}
			Current = Current->next;
		}
		return NULL;
	}
	RP2DeathMatchSpawnSystem &operator -= (GameObject *obj)
	{
		int id = Commands->Get_ID(obj);
		if (!RP2DeathMatchSpawnNodeList)
			return *this;
		RP2DeathMatchSpawnNode *Current = RP2DeathMatchSpawnNodeList, *Prev = NULL;
		while (Current)
		{
			if (Current->id == id)
			{
				if (!Prev)
					RP2DeathMatchSpawnNodeList = Current->next;
				else
					Prev->next = Current->next;
				RP2DeathMatchSpawnNode *temp = Current;
				Current = Current->next;
				delete temp;
				continue;
			}
			Prev = Current;
			Current = Current->next;
		}
		return *this;
	}
	void emptyList()
	{
		RP2DeathMatchSpawnNode *temp,*die;
		temp = RP2DeathMatchSpawnNodeList;
		while (temp)
		{
			die = temp;
			temp = temp->next;
			delete die;
		}
		RP2DeathMatchSpawnNodeList = NULL;
	}
	void update()
	{
		RP2DeathMatchSpawnNode *Current = RP2DeathMatchSpawnNodeList;
		while (Current)
		{
			Current->lastSeen++;
			Current = Current->next;
		}
	}
	void seePoint(int pointId)
	{
		RP2DeathMatchSpawnNode *Current = RP2DeathMatchSpawnNodeList;
		while (Current)
		{
			if (Current->id == pointId)
			{
				Current->lastSeen = 0;
				return;
			}
			Current = Current->next;
		}
	}
	RP2DeathMatchSpawnNode *findSpawnPoint(int playerId)
	{
		float bestDistance = 0;
		unsigned long bestTime = 0;
		RP2DeathMatchSpawnNode *Current = RP2DeathMatchSpawnNodeList,*best = NULL;
		while (Current)
		{
			float tempDist = FindNearestPlayer(Current->pos,playerId);
			if ((tempDist > 25.0f || tempDist == -1) && (!bestTime || (Current->lastSeen > 1 && bestTime < Current->lastSeen) || (bestTime == Current->lastSeen && tempDist > bestDistance)))
			{
				bestDistance = tempDist;
				bestTime = Current->lastSeen;
				best = Current;
			}
			Current = Current->next;
		}
		if (best)
		{
			best->lastSeen = 0;
			return best;
		}
		Current = RP2DeathMatchSpawnNodeList;
		while (Current)
		{
			float tempDist = FindNearestPlayer(Current->pos,playerId);
			if (!bestDistance || tempDist > bestDistance)
			{
				bestDistance = tempDist;
				best = Current;
			}
			Current = Current->next;
		}
		if (best)
		{
			best->lastSeen = 0;
			return best;
		}
		return NULL;
	}
	bool isEmpty()
	{
		if (!RP2DeathMatchSpawnNodeList)
			return true;
		return false;
	}
};
RP2DeathMatchSpawnSystem RP2DeathMatchSpawnControl = RP2DeathMatchSpawnSystem();

class JMG_DeathMatch_Random_Spawn_Point : public ScriptImpClass {
	RP2DeathMatchSpawnSystem::RP2DeathMatchSpawnNode *myNode;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Destroyed(GameObject *obj);
};

class JMG_DeathMatch_Teleport_Zone_Script : public ScriptImpClass {
	Vector3 Teleport_Pos;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_DeathMatch_Teleport_Zone_Script_Attach : public ScriptImpClass {
	int Set_Facing_ID;
	int Movement_Object;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_DeathMatch_Damage_When_Outside_Of_Range : public ScriptImpClass {
	float warnDistance;
	float damageDistance;
	float vehicleWarnDistance;
	float vehicleDamageDistance;
	float aircraftWarnDistance;
	float aircraftDamageDistance;
	float maxSurviveDistance;
	Vector3 myPos;
	int warnTime[128];
	bool screenEffectOn[128];
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};


class JMG_DeathMatch_Random_Weapon_Spawner : public ScriptImpClass {
	int spawnLimit;
	int powerupId;
	bool enabled;
	int enableId;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	int CreatePowerup(GameObject *obj);
};

class JMG_DeathMatch_Random_Weapon_Spawner_Attached : public ScriptImpClass {
	void Destroyed(GameObject *obj);
};