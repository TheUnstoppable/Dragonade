
#pragma once
#include "jmgUtility.h"
#include "direct.h"
NewObjectiveSystem NewObjectiveSystemControl = NewObjectiveSystem(1);
#define PI 3.14159265f
#define PI180 PI/180
class TempPlayerData
{
public:
	struct TempPlayerDataNode
	{
		bool hasGotIntro;
		int killsWithoutGettingHurt;
		bool hasAnnounced10;
		int totalKills;
		time_t friendlyFireTime;
		time_t talkToPrezTime;
		int friendlyFireCount;
		char playerName[128];
		bool firstJoin;
		time_t destroyTime;
		struct TempPlayerDataNode *next;
		unsigned int deaths;
		int binocularTime;
		int sentryTurretId;
		bool portablePumpjackBeingPlaced;
		int portablePumpjackId;
		bool displayedKillMessage;
		TempPlayerDataNode(const char *playerName)
		{
			hasGotIntro = false;
			displayedKillMessage = false;
			killsWithoutGettingHurt = 0;
			totalKills = 0;
			talkToPrezTime = NULL;
			friendlyFireTime = clock();
			friendlyFireCount = 0;
			firstJoin = true;
			destroyTime = NULL;
			deaths = 0;
			binocularTime = 100;
			sentryTurretId = 0;
			portablePumpjackId = 0;
			if (playerName)
				sprintf(this->playerName,"%s",playerName);
			else
				sprintf(this->playerName,"");
			next = NULL;
		}
		void clearData()
		{
		}
	};
	struct TempPlayerDataNode *players[128];
private:
	struct TempPlayerDataNode *inactivePlayerData;
	struct TempPlayerDataNode *dummyNode;
	TempPlayerDataNode *FindPlayerNode(const char *playerName)
	{
		TempPlayerDataNode *Current = inactivePlayerData;
		if (!inactivePlayerData)
			return (inactivePlayerData = new TempPlayerDataNode(playerName));
		while (Current)
		{
			if (!_stricmp(Current->playerName,playerName))
				return Current;
			if (!Current->next)
				return (Current->next = new TempPlayerDataNode(playerName));
			Current = Current->next;
		}
		return NULL;
	}
public:
	TempPlayerData()
	{
		dummyNode = new TempPlayerDataNode("");
		for (int x = 0;x < 128;x++)
			players[x] = NULL;
		players[0] = dummyNode;
		inactivePlayerData = NULL;
	}
	void getPlayerNode(GameObject *player)
	{
		if (!player)
			return;
		int playerId = JmgUtility::JMG_Get_Player_ID(player);
		if (!playerId)
			return;
		if (!players[playerId] || _stricmp(players[playerId]->playerName,Get_Player_Name(player)))
			players[playerId] = FindPlayerNode(Get_Player_Name(player));
	}
	void Cleanup()
	{
		TempPlayerDataNode *temp = inactivePlayerData,*die;
		while (temp)
		{
			die = temp;
			die->clearData();
			temp = temp->next;
			delete die;
		}
		inactivePlayerData = NULL;
		for (int x = 1;x < 128;x++)
			players[x] = NULL;
	}
};
TempPlayerData PlayerData;
struct PCTDataObject
{
	int id;
	char animation[32];
	PCTDataObject(int id,const char *animation)
	{
		this->id = id;
		sprintf(this->animation,"%s",animation);
	}
	PCTDataObject()
	{
		this->PCTDataObject::PCTDataObject(0,"");
	}
};
PCTDataObject pctObjectArray[20] = {PCTDataObject()};
class PowerupLimiterSystem
{
	struct PowerupNode
	{
		int id;
		GameObject *obj;
		struct PowerupNode *next;
		PowerupNode(GameObject *obj)
		{
			id = Commands->Get_ID(obj);
			this->obj = obj;
			next = NULL;
		}
	};
	PowerupNode *powerupNodeList;
	int powerupCount;
public:
	PowerupLimiterSystem()
	{
		powerupCount = 0;
		powerupNodeList = NULL;
	}
	PowerupLimiterSystem &operator += (GameObject *obj)
	{
		PowerupNode *current = powerupNodeList;
		if (!powerupNodeList)
			powerupNodeList = new PowerupNode(obj);
		while (current)
		{
			if (!current->next)
			{
				current->next = new PowerupNode(obj);
				break;
			}
			current = current->next;
		}
		powerupCount++;
		if (powerupCount > 30)
			Commands->Destroy_Object(powerupNodeList->obj);
		return *this;
	}
	PowerupLimiterSystem &operator -= (GameObject *obj)
	{
		PowerupNode *current = powerupNodeList,*prev = NULL;
		while (current)
		{
			if (current->obj == obj)
			{
				if (!prev)
				{
					powerupNodeList = powerupNodeList->next;
					delete(current);
				}
				else
				{
					prev->next = current->next;
					delete(current);
				}
				powerupCount--;
				break;
			}
			prev = current;
			current = current->next;
		}
		return *this;
	}
};
class BearHunterVoiceSystem
{
public:
	static int voiceId[128];
	static void PlayVoice(GameObject *obj,const char *soundFile,const char *text)
	{
		JmgUtility::DisplayChatMessage(obj,127,127,255,text);
		int playerId = JmgUtility::JMG_Get_Player_ID(obj);
		Stop_Sound_Player(obj,voiceId[playerId],true);
		voiceId[playerId] = Create_2D_Wave_Sound_Dialog_Player(obj,soundFile);
	}
	static void PlayVoice(const char *soundFile,const char *text)
	{
		JmgUtility::MessageAllPlayers(127,127,255,text);
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player)
				continue;
			Stop_Sound_Player(player,voiceId[x],true);
			voiceId[x] = Create_2D_Wave_Sound_Dialog_Player(player,soundFile);
		}
	}
};
Rp2SimplePositionSystem::SimplePositionNode *Rp2SimplePositionSystem::GetSpotNotVisibileFromSpot(Vector3 pos)
{
	pos.Z += 2;
	SimplePositionNode *current = SimplePositionNodeList;
	int random = Commands->Get_Random_Int(0,ObjectCount*2)+1;
	int originalRandom = random;
	while (current)
	{
		Vector3 targetpos = current->position;
		targetpos.Z += 2.0f;
		CastResultStruct res;
		LineSegClass ray(pos,targetpos);
		PhysRayCollisionTestClass coltest(ray, &res, SOLDIER_GHOST_COLLISION_GROUP);
		PhysicsSceneClass::Get_Instance()->Cast_Ray(coltest,false);
		if (JmgUtility::SimpleDistance(pos,current->position) > 5625 && coltest.CollidedRenderObj)
		{
			if (random)
				random--;
			if (!random)
				return current;
		}
		current = current->next;
		if (!current && originalRandom != random)
			current = SimplePositionNodeList;
	}
	return NULL;
}
Rp2SimplePositionSystem::SimplePositionNode *Rp2SimplePositionSystem::GetNonVisibleSpotFromPlayers(int value)
{
	SimplePositionNode *current = SimplePositionNodeList;
	int random = Commands->Get_Random_Int(0,ObjectCount*2)+1;
	int originalRandom = random;
	while (current)
	{
		if (current->value == value)
		{
			bool visibile = false;
			for (int x = 1;x < 128;x++)
			{
				GameObject *player = Get_GameObj(x);
				if (!player)
					continue;
				Vector3 pos = Commands->Get_Position(player);
				Vector3 targetpos = current->position;
				targetpos.Z += 1.0f;
				pos.Z += 1.0;
				CastResultStruct res;
				LineSegClass ray(pos,targetpos);
				PhysRayCollisionTestClass coltest(ray, &res, SOLDIER_GHOST_COLLISION_GROUP);
				PhysicsSceneClass::Get_Instance()->Cast_Ray(coltest,false);
				if (!coltest.CollidedRenderObj)
					visibile = true;
			}
			if (!visibile)
			{
				if (random)
					random--;
				if (!random)
					return current;
			}
		}
		current = current->next;
		if (!current && originalRandom != random)
			current = SimplePositionNodeList;
	}
	return GetFurthestSpotFromPlayers(value);
}
PowerupLimiterSystem PowerupLimiterSystemControl = PowerupLimiterSystem();
Rp2SimplePositionSystem randomSelectableSpawnPoints[7] = {Rp2SimplePositionSystem()};
Rp2SimplePositionSystem aiDefensePoints[6] = {Rp2SimplePositionSystem()};
Rp2SimplePositionSystem randomWeaponContainerSpawnPositions[6] = {Rp2SimplePositionSystem()};
Rp2SimplePositionSystem ComancheDefensePoints[6] = {Rp2SimplePositionSystem()};
Rp2SimplePositionSystem aiPtLocations = Rp2SimplePositionSystem();
Rp2SimplePositionSystem bossSpawnProtectionPoints = Rp2SimplePositionSystem();
class Rp2SimpleObjectList
{
public:
	struct SimpleObjectNode
	{
		int ID;
		int enemyId;
		GameObject *obj;
		struct SimpleObjectNode *next;
		SimpleObjectNode(GameObject *_obj)
		{
			obj = _obj;
			enemyId = 0;
			ID = Commands->Get_ID(_obj);
			next = NULL;
		}
	};
private:
	SimpleObjectNode *SimpleObjectNodeList;
public:
	int objectCount;
	Rp2SimpleObjectList()
	{
		objectCount = 0;
		SimpleObjectNodeList = NULL;
	}
	Rp2SimpleObjectList &operator += (GameObject *obj)
	{
		SimpleObjectNode *Current = SimpleObjectNodeList;
		if (!SimpleObjectNodeList)
			SimpleObjectNodeList = new SimpleObjectNode(obj);
		while (Current)
		{
			if (!Current->obj)
			{
				Current->ID = Commands->Get_ID(obj);
				Current->obj = obj;
				break;
			}
			if (Current->obj == obj)
				return *this;
			if (!Current->next)
			{
				Current->next = new SimpleObjectNode(obj);
				break;
			}
			Current = Current->next;
		}
		objectCount++;
		return *this;
	};
	Rp2SimpleObjectList &operator -= (GameObject *obj)
	{
		SimpleObjectNode *Current = SimpleObjectNodeList;
		while (Current)
		{
			if (Current->obj == obj)
			{
				Current->ID = 0;
				Current->obj = NULL;
				if (objectCount)
					objectCount--;
			}
			Current = Current->next;
		}
		return *this;
	};
	bool IsInList(GameObject *obj)
	{
		SimpleObjectNode *Current = SimpleObjectNodeList;
		while (Current)
		{
			if (Current->obj == obj)
				return true;
			Current = Current->next;
		}
		return false;
	}
	SimpleObjectNode *find(GameObject *obj)
	{
		SimpleObjectNode *Current = SimpleObjectNodeList;
		while (Current)
		{
			if (Current->obj == obj)
				return Current;
			Current = Current->next;
		}
		return SimpleObjectNodeList;
	}
	void Empty_List()
	{
		objectCount = 0;
		SimpleObjectNode *temp,*die;
		temp = SimpleObjectNodeList;
		while (temp)
		{
			die = temp;
			temp = temp->next;
			delete die;
		}
		SimpleObjectNodeList = NULL;
	}
	void Kill_All()
	{
		SimpleObjectNode *temp = SimpleObjectNodeList,*die;
		while (temp)
		{
			die = temp;
			temp = temp->next;
			if (die->obj)
				Commands->Apply_Damage(die->obj,9999.9f,"BlamoKiller",0);
		}
	}
	int Recount()
	{
		int Count = 0;
		SimpleObjectNode *Current = SimpleObjectNodeList;
		while (Current)
		{
			if (Commands->Find_Object(Current->ID))
				Count++;
			Current = Current->next;
		}
		return Count;
	}
	void DestroyLeastSignificant()
	{
		float LongestDistance = 0;
		GameObject *TempObject = NULL;
		SimpleObjectNode *Current = SimpleObjectNodeList;
		while (Current)
		{
			GameObject *Object = Commands->Find_Object(Current->ID);
			if (Object)
			{
				GameObject *Star = Commands->Get_A_Star(Commands->Get_Position(Object));
				if (Star)
				{
					float Temp = JmgUtility::SimpleDistance(Commands->Get_Position(Object),Commands->Get_Position(Star));
					if (Temp > LongestDistance)
					{
						TempObject = Object;
						LongestDistance = Temp;
					}
				}
			}
			Current = Current->next;
		}
		if (TempObject)
			Commands->Destroy_Object(TempObject);
	}
	void DestroyOutsideOfRange(float range,Vector3 pos)
	{
		SimpleObjectNode *Current = SimpleObjectNodeList;
		while (Current)
		{
			GameObject *Object = Commands->Find_Object(Current->ID);
			if (Object && JmgUtility::SimpleFlatDistance(Commands->Get_Position(Object),pos) > range)
				Commands->Destroy_Object(Object);
			Current = Current->next;
		}
	}
	GameObject *GetRandom()
	{
		if (!objectCount)
		{
			Console_Input("msg SimpleObjectNodeList GetRandom ERROR: List is empty!");
			return NULL;
		}
		int random = Commands->Get_Random_Int(0,objectCount*2);
		SimpleObjectNode *Current = SimpleObjectNodeList;
		while (Current)
		{
			GameObject *Object = Commands->Find_Object(Current->ID);
			if (Object)
			{
				if (random)
					random--;
				if (!random)
					return Current->obj;
			}
			Current = Current->next;
			if (!Current)
				Current = SimpleObjectNodeList;
		}
		return NULL;
	}
	int GetAndRemoveListHeadObject()
	{
		while (SimpleObjectNodeList)
		{
			SimpleObjectNode *Current = SimpleObjectNodeList;
			int id = Current->ID;
			SimpleObjectNodeList = SimpleObjectNodeList->next;
			delete Current;
			if (Commands->Find_Object(id))
				return id;
		}
		return 0;
	}
	int countInRangeTargetingId(Vector3 pos,float range,int targetId)
	{
		int inRangeCount = 0;
		range = range*range;
		SimpleObjectNode *current = SimpleObjectNodeList;
		while (current)
		{
			if (current->ID && current->enemyId == targetId && current->obj && JmgUtility::SimpleFlatDistance(Commands->Get_Position(current->obj),pos) <= range && Commands->Get_Health(current->obj))
				inRangeCount++;
			current = current->next;
		}
		return inRangeCount;
	}
	GameObject *GetNearest(Vector3 pos)
	{
		float LongestDistance = 0;
		GameObject *TempObject = NULL;
		SimpleObjectNode *Current = SimpleObjectNodeList;
		while (Current)
		{
			GameObject *Object = Commands->Find_Object(Current->ID);
			if (Object)
			{
				float Temp = JmgUtility::SimpleDistance(Commands->Get_Position(Current->obj),pos);
				if (!TempObject || Temp < LongestDistance)
				{
					TempObject = Current->obj;
					LongestDistance = Temp;
				}
			}
			Current = Current->next;
		}
		if (TempObject)
			return TempObject;
		return NULL;
	}
	Vector3 GetNearestPosition(Vector3 pos)
	{
		float LongestDistance = 0;
		GameObject *TempObject = NULL;
		SimpleObjectNode *Current = SimpleObjectNodeList;
		while (Current)
		{
			GameObject *Object = Commands->Find_Object(Current->ID);
			if (Object)
			{
				float Temp = JmgUtility::SimpleDistance(Commands->Get_Position(Current->obj),pos);
				if (!TempObject || Temp < LongestDistance)
				{
					TempObject = Current->obj;
					LongestDistance = Temp;
				}
			}
			Current = Current->next;
		}
		if (TempObject)
			return Commands->Get_Position(TempObject);
		return Vector3(0.0f,0.0f,0.0f);
	}
};
Rp2SimpleObjectList bearObjectListControl = Rp2SimpleObjectList();
Rp2SimpleObjectList mutantObjectListControl = Rp2SimpleObjectList();
class JMG_Bear_Hunter_Player_Soldier : public ScriptImpClass {
	int playerId;
	time_t LastTime;
	Vector3 LastPosition;
	int clipBullets;
	int bullets;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *killer);
	void Destroyed(GameObject *obj);
	void CreateExplosion(GameObject *obj,const char *explosion,Vector3 pos,float *distance,int *explosives);
	bool LifeSystem(GameObject *obj);
	void GrantUnlockedWeapons(GameObject *obj);
	void GrantSpecialUnlocks(GameObject *obj);
public:
	struct ArmoredCarWeapon
	{
		char weaponName[256];
		int lastFireTime;
		int lastammo;
		double calculatedGrant;
		double addedGrant;
		ArmoredCarWeapon()
		{
			sprintf(this->weaponName,"");
			this->lastFireTime = 0;
			this->lastammo = 0;
			this->calculatedGrant = 0.0;
			this->addedGrant = 0.0;
		}
		void Reset()
		{
			this->addedGrant = 0.0;
			this->lastFireTime = 75;
		}
	};
	JMG_Bear_Hunter_Player_Soldier()
	{
		playerId = 0;
		lastDamaged = 0;
		armoredCarWeapon = ArmoredCarWeapon();
	}
	static double speed[128];
	static char primaryWeapon[128][256];
	static bool hasGrenadeVest[128];
	int lastDamaged;
	ArmoredCarWeapon armoredCarWeapon;
};
double JMG_Bear_Hunter_Player_Soldier::speed[128] = {0.0};
char JMG_Bear_Hunter_Player_Soldier::primaryWeapon[128][256] = {""};
bool JMG_Bear_Hunter_Player_Soldier::hasGrenadeVest[128] = {false};
struct PlayerEmulatedSound
{
	enum logicalSoundType{stNone = 0,stOldWeapon = 1,stFootsteps = 2,stVehicle = 3,stGunshot = 4,stBulletHit = 5,stNoticeMe = 6};
	float Range;
	logicalSoundType SoundType;
	PlayerEmulatedSound()
	{
		Range = 0;
		SoundType = stNone;
	}
	void SetPlayerEmulatedSound(float range,logicalSoundType soundType)
	{
		Range = range;
		SoundType = soundType;
	}
};
PlayerEmulatedSound PlayerEmulatedSoundList[128] = {PlayerEmulatedSound()};

#define EmulatedSoundCustom 450040
class JMG_Rp2_Dedicated_Server_Sound_Emulator : public ScriptImpClass {
	int vehicleId;
	int vehicleRounds;
	int SoundOverrideTime;
	int LastWeaponID;
	float weaponSoundRange;
	int LastBullets;
	int PlayerID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *killer);
	void Destroyed(GameObject *obj);
};

class BearHunterGameControl
{
public:
	struct BearHunterBearPositionNode
	{
		int id;
		Vector3 pos;
		int badCount;
		struct BearHunterBearPositionNode *xNodes;
		struct BearHunterBearPositionNode *yNodes;
		struct BearHunterBearPositionNode *next;
		BearHunterBearPositionNode(int id,Vector3 pos)
		{
			this->id = id;
			this->pos = pos;
			this->badCount = 0;
			this->yNodes = NULL;
			this->xNodes = NULL;
			this->next = NULL;
		}
	};
private:
	bool builtNetwork;
	int NodeCount;
	BearHunterBearPositionNode *BearHunterBearPositionNodeList;
	Vector2 MiddleValues;
	BearHunterBearPositionNode *BearHunterBearPositionNodeGrouping;
	void AddListNode(int id,const Vector3 &pos)
	{
		BearHunterBearPositionNode *Current = BearHunterBearPositionNodeList;
		if (!BearHunterBearPositionNodeList)
			BearHunterBearPositionNodeList = new BearHunterBearPositionNode(id,pos);
		while (Current)
		{
			if (!Current->next)
			{
				Current->next = new BearHunterBearPositionNode(id,pos);
				break;
			}
			Current = Current->next;
		}
	}
	void AddNodesToLookupTable(BearHunterBearPositionNode *currentNode)
	{
		BearHunterBearPositionNode *Current = BearHunterBearPositionNodeGrouping,*Prev = NULL;
		if (!BearHunterBearPositionNodeGrouping)
			BearHunterBearPositionNodeGrouping = currentNode;
		while (Current)
		{
			if (currentNode->pos.X < Current->pos.X && (!Prev || currentNode->pos.X > Prev->pos.X))
			{// Add an xNode before
				if (!Prev)
					BearHunterBearPositionNodeGrouping = currentNode;
				else
					Prev->xNodes = currentNode;
				currentNode->xNodes = Current;
				return;
			}
			if (currentNode->pos.X > Current->pos.X && (!Current->xNodes || currentNode->pos.X < Current->xNodes->pos.X))
			{// Add an xNode after
				currentNode->xNodes = Current->xNodes;
				Current->xNodes = currentNode;
				return;
			}
			if (Current->pos.X == currentNode->pos.X)
			{//add a Y node
				BearHunterBearPositionNode *YNode = Current->yNodes,*yPrev = NULL;
				if (!Current->yNodes)
				{
					Current->yNodes = currentNode;
					return;
				}
				while (YNode)
				{
					if (currentNode->pos.Y < YNode->pos.Y && (!yPrev || currentNode->pos.Y > yPrev->pos.Y))
					{// add node before
						if (!yPrev)
							Current->yNodes = currentNode;
						else
							yPrev->yNodes = currentNode;
						currentNode->yNodes = YNode;
						return;
					}
					if (currentNode->pos.Y > YNode->pos.Y && (!YNode->yNodes || currentNode->pos.Y < YNode->yNodes->pos.Y))
					{// add node after
						currentNode->yNodes = YNode->yNodes;
						YNode->yNodes = currentNode;
						return;
					}
					yPrev = YNode;
					YNode = YNode->yNodes;
				}
				return;
			}
			Prev = Current;
			Current = Current->xNodes;
		}
	}
	void BuildLookupTable()
	{
		BearHunterBearPositionNode *Current = BearHunterBearPositionNodeList;
		while (Current)
		{
			AddNodesToLookupTable(Current);
			Current = Current->next;
		}
	}
	void PrintNodeTable()
	{
		int count = 0;
		BearHunterBearPositionNode *Current = BearHunterBearPositionNodeList;
		while (Current)
		{
			count++;
			Current = Current->next;
		}
		char debugTxt[220];
		sprintf(debugTxt,"msg Total Nodes In List:%d",count);
		Console_Input(debugTxt);
		BearHunterBearPositionNode *table = BearHunterBearPositionNodeGrouping;
		while (table)
		{
			BearHunterBearPositionNode *y = table->yNodes;
			sprintf(debugTxt,"\nX");
			while (y)
			{
				sprintf(debugTxt,"X");
				y = y->yNodes;
			}
			table = table->xNodes;
		}
	}
public:
	BearHunterGameControl()
	{
		builtNetwork = false;
		NodeCount = 0;
		BearHunterBearPositionNodeList = NULL;
		BearHunterBearPositionNodeGrouping = NULL;
	}
	void AddNodeObject(GameObject *obj)
	{
		AddListNode(Commands->Get_ID(obj),Commands->Get_Position(obj));
		NodeCount++;
		Commands->Destroy_Object(obj);
	}
	void Load(const char *fileName)
	{
		int File = Commands->Text_File_Open(fileName);
		if (!File)
		{
			char errorMsg[220];
			sprintf(errorMsg,"msg BearHunterGameControl ERROR: File %s not found!",fileName);
			Console_Input(errorMsg);
			return;
		}
		char TextString[1024];
		while (Commands->Text_File_Get_String(File,TextString,1024))
		{
			int id;
			float x,y,z;
			sscanf(TextString,"%d %f %f %f",&id,&x,&y,&z);
			AddListNode(id,Vector3(x,y,z));
			NodeCount++;
		}
		BuildLookupTable();
		Commands->Text_File_Close(File);
	}
	Vector3 LookupNearestPosition(Vector2 pos)
	{
		if (BearHunterBearPositionNodeGrouping)
		{
			BearHunterBearPositionNode *table = BearHunterBearPositionNodeGrouping;
			while (table)
			{
				if (pos.X < table->pos.X || (pos.X >= table->pos.X && (!table->xNodes || pos.X < table->xNodes->pos.X)))
				{
					BearHunterBearPositionNode *y = table->yNodes;
					while (y)
					{
						if (pos.Y < y->pos.Y || (pos.Y >= y->pos.Y && (!y->yNodes || pos.Y < y->yNodes->pos.Y)))
							return y->pos;
						y = y->yNodes;
					}
				}
				table = table->xNodes;
			}
			char debugmsg[220];
			sprintf(debugmsg,"msg BearHunterGameControl::LookupNearestPosition ERROR: Could not find pos for location <%.2f,%.2f>",pos.X,pos.Y);
			Console_Input(debugmsg);
		}
		Vector3 StarPos = Vector3(pos.X,pos.Y,0.0f);
		float bestDist = -1;
		BearHunterBearPositionNode *Current = BearHunterBearPositionNodeList,*BestNode = NULL;
		while (Current)
		{
			float dist = JmgUtility::SimpleFlatDistance(StarPos,Current->pos);
			if (!BestNode || dist < bestDist)
			{
				dist = bestDist;
				BestNode = Current;
			}
			Current = Current->next;
		}
		if (BestNode)
			return BestNode->pos;
		return Vector3(0.0f,0.0f,0.0f);
	}
	bool getRandomPosition(Vector3 *pos,float minRange,float range)
	{
		if (BearHunterBearPositionNodeGrouping)
		{
			int Random = Commands->Get_Random_Int(0,(NodeCount ? NodeCount : 1)+1);
			int original = Random;
			BearHunterBearPositionNode *table = BearHunterBearPositionNodeGrouping;
			while (table)
			{
				BearHunterBearPositionNode *y = table->yNodes;
				while (y)
				{
					float tempDist = JmgUtility::SimpleDistance(y->pos,*pos);
					if (tempDist <= range && tempDist >= minRange)
					{
						Random--;
						if (!Random)
						{
							*pos = y->pos;
							return true;
						}
					}
					y = y->yNodes;
				}
				table = table->xNodes;
				if (!table && Random != original)
				{
					original = Random;
					table = BearHunterBearPositionNodeGrouping;
				}
			}
		}
		return false;
	}
	void emptyList()
	{
		builtNetwork = false;
		NodeCount = 0;
		BearHunterBearPositionNodeGrouping = NULL;
		BearHunterBearPositionNode *temp,*die;
		temp = BearHunterBearPositionNodeList;
		while (temp)
		{
			die = temp;
			temp = temp->next;
			delete die;
		}
		BearHunterBearPositionNodeList = NULL;
	}
	void TestGrid();
	BearHunterBearPositionNode *FindNodeByID(int id)
	{
		BearHunterBearPositionNode *Current = BearHunterBearPositionNodeList;
		while (Current)
		{
			if (Current->id == id)
				return Current;
			Current = Current->next;
		}
		return NULL;
	}
	void SaveCleanedTestGrid()
	{
		BearHunterBearPositionNode *Current = BearHunterBearPositionNodeList;
		while (Current)
		{
			if (Current->badCount)
			{
				char badNodes[220];
				sprintf(badNodes,"msg %d was marked as bad",Current->id);
				Console_Input(badNodes);
			}
			Current = Current->next;
		}
	}
};
BearHunterGameControl BearHunterGameControlSystem = BearHunterGameControl();
struct EASTowerNode
{
	int id;
	Vector3 position;
	int activeTime;
	EASTowerNode()
	{
		this->id = 0;
		this->position = Vector3();
		this->activeTime = 0;
	}
	EASTowerNode(GameObject *obj)
	{
		this->id = Commands->Get_ID(obj);
		this->position = Commands->Get_Position(obj);
		this->activeTime = 0;
	}
	bool operator == (const int value)
	{
		if (id == value)
			return true;
		return false;
	}
	bool operator != (const int value)
	{
		if (id == value)
			return false;
		return false;
	}
};
class JMG_Bear_Hunter_Game_Control : public ScriptImpClass {
	bool smoothFade;
	bool needsCleanUp;
	EASTowerNode easTowers[2];
	bool alarmSwitchActive;
	bool atCabin;
	bool starGold;
	int maxBears;
	bool musicOn;
	unsigned long meteoriteTime;
	int maxPlayerCount;
	static GameObject *myObject;
	double bearChance;
	double deerChance;
	double gcatChance;
	double bcatChance;
	double ccatChance;
	int blackBearId;
	bool cleanUpNeeded;
	bool engineerSpawnedOnce[5];
	int engineerScanTime;
	int lastUpdateEngineersDead;
	int wildMountainLionIds[8];
	int wildMountainLionRespawn[8];
	int wildMountainLionsPet;
	int cowId;
	int cowWanderId;
	int playerReturnedTurkeys[128];
	int playerReturnedTurkeysDelay[128];
	bool turkeysExist;
	bool hasReturnedTurkeys;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
	bool CheckIfPlayerInRange(Vector3 pos,float distance);
	void EndGameDataCleanup(GameObject *obj,bool saveData);
	void CreateMouse();
	void CreateNormalAnimals(int normalBears);
	void CreateBears(int normalBears);
	void SpawnAngryTinyDeer();
	void CreateCow();
	void TriggerCowObjective();
	static GameObject *MutantSpawn();
public:
	JMG_Bear_Hunter_Game_Control();
	static void ObjectiveCompleteReward(float credits);
	static enum GameState{LeaveBase = 0,HuntBears = 1,MutationStart = 2,FallBack = 3,DefendPrez = 4,SecureBase = 5,BossArrive = 6} gameState;
	static int mutantTargetId;
	static bool gameOver;
	static int gameTime;
	static int spawnGroup;
	static char currentSong[256];
	static int objective;
	static int bearTransition;
	static bool hasGotTurrets;
	static int aiIgnorePlayers[128];
	static int remainingLives[128];
	static bool playerAlive[128];
	static int bossRabitDead;
	static unsigned int earthWarheadId;
	static unsigned int deathWarheadId;
	static int turretsDestroyed;
	static Vector3 engineerSpawnLocations[5];
	static int entineersReturned;
	static bool weaponsFound[6];
	static bool weaponsReturned[6];
	static bool weaponsUnlocked[7];
	static float soundAttentionWhoreNotify;
	static bool diedInWater[128];
	static bool hasActivatedTower;
	static int winWait;
	static bool playersDead;
	static int medicineDropInTime;
	static float attackSubstationChance;
	static int mutantBears;
	static float mutantRespawnTime;
	static float CreatePositionZ;
	static float GamePercent;
	static Vector3 centerOfBase;
	static bool truckTimeExtended;
	static int spawnKarma;
	static bool hasBeenInjured;
	static int karmaDeerIds[6];
	static int friendlyTinyDeerIds[5];
	static int friendlyTinyDeerRespawnTime[5];
	static int wanderingAiIgnorePlayers[128];
	static void IncreaseBonusObjectiveCount(int objectiveId);
	static int bonusObjectiveCount;
	static int pumpJackIds[7];
	static int damagedPumpJackIds[3];
	static Vector3 pumpJackPos[7];
	static Vector3 damagedPumpJackPos[3];
};
GameObject *JMG_Bear_Hunter_Game_Control::myObject = NULL;
int JMG_Bear_Hunter_Game_Control::mutantTargetId = 600172;
JMG_Bear_Hunter_Game_Control::GameState JMG_Bear_Hunter_Game_Control::gameState = LeaveBase;
bool JMG_Bear_Hunter_Game_Control::gameOver = false;
int JMG_Bear_Hunter_Game_Control::gameTime = 0;
int JMG_Bear_Hunter_Game_Control::spawnGroup = 0;
char JMG_Bear_Hunter_Game_Control::currentSong[256] = {"SpecForce_Sneak_01.mp3"};
int JMG_Bear_Hunter_Game_Control::objective = 0;
int JMG_Bear_Hunter_Game_Control::bearTransition = 0;
bool JMG_Bear_Hunter_Game_Control::hasGotTurrets = false;
int JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[128] = {0};
int JMG_Bear_Hunter_Game_Control::remainingLives[128] = {0};
bool JMG_Bear_Hunter_Game_Control::playerAlive[128] = {true};
int JMG_Bear_Hunter_Game_Control::bossRabitDead = 0;
unsigned int JMG_Bear_Hunter_Game_Control::earthWarheadId = 22;
unsigned int JMG_Bear_Hunter_Game_Control::deathWarheadId = 22;
int JMG_Bear_Hunter_Game_Control::turretsDestroyed = 0;
Vector3 JMG_Bear_Hunter_Game_Control::engineerSpawnLocations[5] = {Vector3(-263.045f,150.827f,0.839f),Vector3(-266.169f,150.874f,0.839f),Vector3(-198.472f,-262.617f,0.251f),Vector3(-198.408f,-259.017f,0.259f),Vector3(-203.714f,-260.658f,0.216f)};
int JMG_Bear_Hunter_Game_Control::entineersReturned = 0;
bool JMG_Bear_Hunter_Game_Control::weaponsFound[6] = {false};
bool JMG_Bear_Hunter_Game_Control::weaponsReturned[6] = {false};
bool JMG_Bear_Hunter_Game_Control::weaponsUnlocked[7] = {false};
float JMG_Bear_Hunter_Game_Control::soundAttentionWhoreNotify = 0.0025f;
bool JMG_Bear_Hunter_Game_Control::diedInWater[128] = {false};
bool JMG_Bear_Hunter_Game_Control::hasActivatedTower = false;
int JMG_Bear_Hunter_Game_Control::winWait = 0;
bool JMG_Bear_Hunter_Game_Control::playersDead = false;
int JMG_Bear_Hunter_Game_Control::medicineDropInTime = 0;
float JMG_Bear_Hunter_Game_Control::attackSubstationChance = 1.0f;
int JMG_Bear_Hunter_Game_Control::mutantBears = 0;
float JMG_Bear_Hunter_Game_Control::mutantRespawnTime = 0.0f;
float JMG_Bear_Hunter_Game_Control::CreatePositionZ = 0.0f;
float JMG_Bear_Hunter_Game_Control::GamePercent = 0.0f;
Vector3 JMG_Bear_Hunter_Game_Control::centerOfBase = Vector3(45.146f,-689.362f,-0.166f);
bool JMG_Bear_Hunter_Game_Control::truckTimeExtended = false;
int JMG_Bear_Hunter_Game_Control::spawnKarma = 0;
bool JMG_Bear_Hunter_Game_Control::hasBeenInjured = false;
int JMG_Bear_Hunter_Game_Control::karmaDeerIds[6] = {0};
int JMG_Bear_Hunter_Game_Control::friendlyTinyDeerIds[5] = {0};
int JMG_Bear_Hunter_Game_Control::friendlyTinyDeerRespawnTime[5] = {0};
int JMG_Bear_Hunter_Game_Control::wanderingAiIgnorePlayers[128] = {0};
int JMG_Bear_Hunter_Game_Control::bonusObjectiveCount = 0;
int JMG_Bear_Hunter_Game_Control::pumpJackIds[7] = {601041,601038,601044,601040,601045,601039,601043};
int JMG_Bear_Hunter_Game_Control::damagedPumpJackIds[3] = {601061,601062,601063};
Vector3 JMG_Bear_Hunter_Game_Control::pumpJackPos[7];
Vector3 JMG_Bear_Hunter_Game_Control::damagedPumpJackPos[3];

struct BearHunterScoreSystem
{
public:
	#define BHHighScoreListCount 124
	struct BHScoreNode
	{
		char PlayerName[256];
		unsigned long PlayTime;
		unsigned long PreGameTime;
		unsigned long IdleTime;
		unsigned long RoundsPlayed;
		unsigned long RoundsCompleted;
		unsigned long RoundsQuit;
		unsigned long RoundsWon;
		unsigned long RoundsLost;
		unsigned long MostKillsInARound;
		unsigned long MostDeathsInARound;
		unsigned long MostBonusObjectivesCompletedInARound;
		unsigned long Deaths;
		unsigned long Kills;
		unsigned long VehicleKills;
		unsigned long KilledSelf;
		unsigned long KilledPlayers;
		unsigned long KilledPresident;
		unsigned long KilledTurrets;
		unsigned long KilledBears;
		unsigned long KilledBlackBears;
		unsigned long KilledMutantBears;
		unsigned long KilledMutantDeer;
		unsigned long KilledMutantCats;
		unsigned long KilledMutantCatsB;
		unsigned long KilledMutantCatsR;
		unsigned long KilledMutantRabbits;
		unsigned long ObjectiveActivatedAlarm;
		unsigned long ObjectiveTurretTruck;
		unsigned long ObjectiveTurretTruckAlarm;
		unsigned long ObjectiveOilRigsActivated;
		unsigned long ObjectiveOilRigsRepaired;
		unsigned long ObjectiveEngineersSaved;
		unsigned long ObjectiveWeaponsFound;
		unsigned long ObjectiveWeaponsReturned;
		unsigned long ObjectivePlasmaRifleReturned;
		unsigned long BonusObjectivesCompleted;
		unsigned long PickedupHealthPowerups;
		unsigned long PickedupArmorPowerups;
		unsigned long PickedupCashPowerups;
		unsigned long PickedupAmmoPowerups;
		unsigned long PickedupHealthTotal;
		unsigned long PickedupArmorTotal;
		unsigned long PickedupCashTotal;
		unsigned long PickedupAmmoTotal;
		unsigned long PickedupTotalPowerups;
		unsigned long PickedupTotalPowerupsInARound;
		unsigned long KilledHumanAi;
		unsigned long VehiclesDestroyed;
		unsigned long VehiclesLost;
		unsigned long JazzsLost;
		unsigned long CleasansLost;
		unsigned long TrucksLost;
		unsigned long TanksLost;
		unsigned long TurretTruckLost;
		unsigned long C4VestPowerups;
		unsigned long ActivatedCommTower;
		unsigned long PlayedGamesWithDefenseTurrets;
		unsigned long PlayedGamesWithGuardianHelicopter;
		unsigned long TimesDrown;
		unsigned long TimesFallen;
		unsigned long KillsWithSentryTurret;
		unsigned long KilledSentryTurrets;
		unsigned long SentryTurretsPlaced;
		unsigned long SentryTurretsLost;
		unsigned long PickedUpMedicalNeedle;
		unsigned long ReturnedMedicalNeedle;
		unsigned long RepairedSubstation;
		unsigned long SubstationOnLineAtEnd;
		unsigned long SubstationNotDamaged;
		unsigned long GiantDeerKilled;
		unsigned long WolfKilled;
		unsigned long MutantDogKilled;
		unsigned long BlueDeerKilled;
		unsigned long SurvivedAlarm;
		unsigned long CheatedRounds;
		unsigned long NeverInjured;
		unsigned long MooseKilled;
		unsigned long EatenByRabbit;
		unsigned long PickedUpDeerStatue;
		unsigned long DroppedDeerStatue;
		unsigned long ReturnedDeerStatue;
		unsigned long TinyDeerKilled;
		unsigned long MutantSquirrelsKilled;
		unsigned long WildDeerKilled;
		unsigned long WildSquirrelsKilled;
		unsigned long ArmoredCarsLost;
		unsigned long WarriorsLost;
		unsigned long TimeOnFoot;
		unsigned long TimeInAJazzs;
		unsigned long TimeInACleasans;
		unsigned long TimeInASecurityTruck;
		unsigned long TimeInArmoredCars;
		unsigned long TimeInAUDVs;
		unsigned long TimeInGatlingTanks;
		unsigned long TimeInIFVs;
		unsigned long FriendlyTinyDeerKilled;
		unsigned long SupportReceivedInfantryHp;
		unsigned long SupportReceivedVehicleHp;
		unsigned long SupportReceivedAmmo;
		unsigned long SupportGrantedHpInfantry;
		unsigned long SupportGrantedHpVehicle;
		unsigned long SupportGrantedAmmo;
		unsigned long SupportReceivedHpInfantrySelf;
		unsigned long SupportReceivedHpVehicleSelf;
		unsigned long SupportReceivedAmmoSelf;
		unsigned long PettedCougars;
		unsigned long KilledCougars;
		unsigned long KilledMutantCougars;
		unsigned long FailedPettingCougars;
		unsigned long KilledFriendlyCougars;
		unsigned long DiedTryingToEscapeBase;
		JmgUtility::GenericDateTime LastPlayTime;
		unsigned long RescuedCows;
		unsigned long KilledCows;
		unsigned long LostCows;
		unsigned long CompletedCowObjective;
		unsigned long KilledMice;
		unsigned long CompledMiceObjective;
		unsigned long KilledPortablePumpJacks;
		unsigned long PortablePumpJacksPlaced;
		unsigned long PortablePumpJacksLost;
		unsigned long PumpJackMoney;
		unsigned long MobilePumpJackMoney;
		unsigned long Rank;
		unsigned long KilledTurkey;
		unsigned long ReturnedTurkey;

		bool startedRound;
		unsigned long totalObjectivesCompleted;
		unsigned long totalPowerupsPickedup;
		float lastFacing;
		Vector3 lastPos;
		bool isMoving;
		bool isTurning;
		unsigned int idleDelay;
		unsigned long tmpPlayTime;
		float tmpSupportReceivedInfantryHp;
		float tmpSupportReceivedVehicleHp;
		float tmpSupportGrantedHpInfantry;
		float tmpSupportGrantedHpVehicle;
		float tmpSupportReceivedHpInfantrySelf;
		float tmpSupportReceivedHpVehicleSelf;
		int tmpPettedCougars;
		double weightedRank;
		BHScoreNode *next;
		BHScoreNode(const char *PlayerName)
		{
			sprintf(this->PlayerName,"%s",PlayerName);
			PlayTime = 0;
			PreGameTime = 0;
			IdleTime = 0;
			RoundsPlayed = 0;
			RoundsCompleted = 0;
			RoundsQuit = 0;
			RoundsWon = 0;
			RoundsLost = 0;
			MostKillsInARound = 0;
			MostDeathsInARound = 0;
			MostBonusObjectivesCompletedInARound = 0;
			Deaths = 0;
			Kills = 0;
			VehicleKills = 0;
			KilledSelf = 0;
			KilledPlayers = 0;
			KilledPresident = 0;
			KilledTurrets = 0;
			KilledBears = 0;
			KilledBlackBears = 0;
			KilledMutantBears = 0;
			KilledMutantDeer = 0;
			KilledMutantCats = 0;
			KilledMutantCatsB = 0;
			KilledMutantCatsR = 0;
			KilledMutantRabbits = 0;
			ObjectiveActivatedAlarm = 0;
			ObjectiveTurretTruck = 0;
			ObjectiveTurretTruckAlarm = 0;
			ObjectiveOilRigsActivated = 0;
			ObjectiveOilRigsRepaired = 0;
			ObjectiveEngineersSaved = 0;
			ObjectiveWeaponsFound = 0;
			ObjectiveWeaponsReturned = 0;
			ObjectivePlasmaRifleReturned = 0;
			BonusObjectivesCompleted = 0;
			PickedupHealthPowerups = 0;
			PickedupArmorPowerups = 0;
			PickedupCashPowerups = 0;
			PickedupAmmoPowerups = 0;
			PickedupHealthTotal = 0;
			PickedupArmorTotal = 0;
			PickedupCashTotal = 0;
			PickedupAmmoTotal = 0;
			PickedupTotalPowerups = 0;
			PickedupTotalPowerupsInARound = 0;
			KilledHumanAi = 0;
			VehiclesDestroyed = 0;
			VehiclesLost = 0;
			JazzsLost = 0;
			CleasansLost = 0;
			TrucksLost = 0;
			TanksLost = 0;
			TurretTruckLost = 0;
			C4VestPowerups = 0;
			ActivatedCommTower = 0;
			PlayedGamesWithDefenseTurrets = 0;
			PlayedGamesWithGuardianHelicopter = 0;
			TimesDrown = 0;
			TimesFallen = 0;
			KillsWithSentryTurret = 0;
			KilledSentryTurrets = 0;
			SentryTurretsPlaced = 0;
			SentryTurretsLost = 0;
			PickedUpMedicalNeedle = 0;
			ReturnedMedicalNeedle = 0;
			RepairedSubstation = 0;
			SubstationOnLineAtEnd = 0;
			SubstationNotDamaged = 0;
			GiantDeerKilled = 0;
			WolfKilled = 0;
			MutantDogKilled = 0;
			BlueDeerKilled = 0;
			SurvivedAlarm = 0;
			CheatedRounds = 0;
			NeverInjured = 0;
			MooseKilled = 0;
			EatenByRabbit = 0;
			PickedUpDeerStatue = 0;
			DroppedDeerStatue = 0;
			ReturnedDeerStatue = 0;
			TinyDeerKilled = 0;
			MutantSquirrelsKilled = 0;
			WildDeerKilled = 0;
			WildSquirrelsKilled = 0;
			ArmoredCarsLost = 0;
			WarriorsLost = 0;
			TimeOnFoot = 0;
			TimeInAJazzs = 0;
			TimeInACleasans = 0;
			TimeInASecurityTruck = 0;
			TimeInArmoredCars = 0;
			TimeInAUDVs = 0;
			TimeInGatlingTanks = 0;
			TimeInIFVs = 0;
			FriendlyTinyDeerKilled = 0;
			SupportReceivedInfantryHp = 0;
			SupportReceivedVehicleHp = 0;
			SupportReceivedAmmo = 0;
			SupportGrantedHpInfantry = 0;
			SupportGrantedHpVehicle = 0;
			SupportGrantedAmmo = 0;
			SupportReceivedHpInfantrySelf = 0;
			SupportReceivedHpVehicleSelf = 0;
			SupportReceivedAmmoSelf = 0;
			PettedCougars = 0;
			KilledCougars = 0;
			KilledMutantCougars = 0;
			FailedPettingCougars = 0;
			KilledFriendlyCougars = 0;
			DiedTryingToEscapeBase = 0;
			JmgUtility::GenericDateTime LastPlayTime = JmgUtility::GenericDateTime();
			RescuedCows = 0;
			KilledCows = 0;
			LostCows = 0;
			CompletedCowObjective = 0;
			KilledMice = 0;
			CompledMiceObjective = 0;
			KilledPortablePumpJacks = 0;
			PortablePumpJacksPlaced = 0;
			PortablePumpJacksLost = 0;
			PumpJackMoney = 0;
			MobilePumpJackMoney = 0;
			Rank = 0;
			KilledTurkey = 0;
			ReturnedTurkey = 0;

			startedRound = JMG_Bear_Hunter_Game_Control::gameState >= JMG_Bear_Hunter_Game_Control::HuntBears ? true : false;
			totalObjectivesCompleted = 0;
			totalPowerupsPickedup = 0;
			lastFacing = 0;
			lastPos = Vector3(0.0f,0.0f,0.0f);
			idleDelay = 0;
			tmpPlayTime = 0;
			isMoving = false;
			isTurning = false;
			tmpSupportReceivedInfantryHp = 0.0f;
			tmpSupportReceivedVehicleHp = 0.0f;
			tmpSupportGrantedHpInfantry = 0.0f;
			tmpSupportGrantedHpVehicle = 0.0f;
			tmpSupportReceivedHpInfantrySelf = 0.0f;
			tmpSupportReceivedHpVehicleSelf = 0.0f;
			tmpPettedCougars = 0;
			next = NULL;
		}
	};
private:
	char savePath[256];
	bool hasLoaded;
	int selectRandomMatchingScore;
	BHScoreNode *BHScoreNodeList;
	BHScoreNode *BHScoreNodeEmptyNode;
	BHScoreNode *BHPlayerHighScoreNodes[128];
	BHScoreNode *FindOrAddPlayerBHHighScoreNode(const char *PlayerName)
	{
		if (!BHScoreNodeList)
		{
			BHScoreNodeList = new BHScoreNode(PlayerName);
			return BHScoreNodeList;
		}
		BHScoreNode *Current = BHScoreNodeList;
		while (Current)
		{
			if (!_stricmp(Current->PlayerName,PlayerName))
				return Current;
			if (!Current->next)
			{
				 Current->next = new BHScoreNode(PlayerName);
				 return Current->next;
			}
			Current = Current->next;
		}
		return NULL;
	}
	int lastDispalyedHighScore;
	BHScoreNode *mergeList(BHScoreNode *split1,BHScoreNode *split2)
	{
		if(split1 == NULL)
			return split2;
		if(split2 == NULL)
			return split1;
		BHScoreNode *newhead = NULL;
		if(split1->weightedRank >= split2->weightedRank)
		{
			newhead = split1;
			newhead->next = mergeList(split1->next,split2);
		}
		else
		{
			newhead = split2;
			newhead->next = mergeList(split1,split2->next);
		}
		return newhead;
	}
	void splitList(BHScoreNode *head,BHScoreNode **split1,BHScoreNode **split2)
	{
		BHScoreNode *slow = head;
		BHScoreNode *fast = head->next;
		while(fast != NULL)
		{
			fast = fast->next;
			if(fast != NULL)
			{
				slow = slow->next;
				fast = fast->next;
			}
		}
		*split1 = head;
		*split2 = slow->next;
		slow->next = NULL;
	}
	void mergeSort(BHScoreNode **refToHead)
	{
		BHScoreNode *head = *refToHead;
		BHScoreNode *split1,*split2;
		if(head == NULL || head->next == NULL)
			return;
		splitList(head,&split1,&split2);
		mergeSort(&split1);
		mergeSort(&split2);
		*refToHead = mergeList(split1,split2);
		return;    
	}
	void CalculateRank()
	{
		BHScoreNode *current = BHScoreNodeList;
		unsigned long maxRoundsWon = 0,maxKills = 0,maxBonusObjectivesCompleted = 0,maxPlayTime = 0;
		while (current)
		{
			if (current->RoundsWon > maxRoundsWon)
				maxRoundsWon = current->RoundsWon;
			if (current->Kills > maxKills)
				maxKills = current->Kills;
			if (current->BonusObjectivesCompleted > maxBonusObjectivesCompleted)
				maxBonusObjectivesCompleted = current->BonusObjectivesCompleted;
			if (current->PlayTime > maxPlayTime)
				maxPlayTime = current->PlayTime;
			current = current->next;
		}
		current = BHScoreNodeList;
		while (current)
		{
			current->weightedRank = current->RoundsWon/(double)maxRoundsWon*1.0+
									current->Kills/(double)maxKills*0.5+
									current->BonusObjectivesCompleted/(double)maxBonusObjectivesCompleted*0.5+
									current->PlayTime/(double)maxPlayTime*0.1;
			current = current->next;
		}
		mergeSort(&BHScoreNodeList);
	}
public:
	BearHunterScoreSystem()
	{
		hasLoaded = false;
		lastDispalyedHighScore = -1;
		BHScoreNodeList = NULL;
		BHScoreNodeEmptyNode = new BHScoreNode("\0");
		if (Exe != EXE_LEVELEDIT)
		{
			sprintf(savePath, "%s\\Save\\", Get_File_Path());
		}
		for (int x = 0;x < 128;x++)
			BHPlayerHighScoreNodes[x] = BHScoreNodeEmptyNode;
	};
	BHScoreNode *Get_Current_Player_Score_Node(int PlayerID)
	{
		if (!PlayerID)
			return BHScoreNodeEmptyNode;
		const char *PlayerName = Get_Player_Name_By_ID(PlayerID);
		if (!PlayerName)
			return BHScoreNodeEmptyNode;
		if (!BHPlayerHighScoreNodes[PlayerID])
			BHPlayerHighScoreNodes[PlayerID] = FindOrAddPlayerBHHighScoreNode(PlayerName);
		else if (_stricmp(BHPlayerHighScoreNodes[PlayerID]->PlayerName,PlayerName))
			BHPlayerHighScoreNodes[PlayerID] = FindOrAddPlayerBHHighScoreNode(PlayerName);
		return BHPlayerHighScoreNodes[PlayerID];
	}
	void Cleanup()
	{
		hasLoaded = false;
		lastDispalyedHighScore = -1;
		for (int x = 0;x < 128;x++)
		{
			if (BHPlayerHighScoreNodes[x])
				BHPlayerHighScoreNodes[x] = BHScoreNodeEmptyNode;
		}
		if (BHScoreNodeList)
		{
			BHScoreNode *temp,*die;
			temp = BHScoreNodeList;
			while (temp != NULL)
			{
				die = temp;
				temp = temp->next;
				delete die;
			}
			BHScoreNodeList = NULL;
		}
	}
	void SaveData()
	{
		if (!hasLoaded)
		{
			Console_Input("msg ERROR: Could not save high scores, no successful load detected!");
			return;
		}
		FILE *SaveScores;
		FILE *SaveScores2;
		char tempPath[512],textTmpPath[512],realPath[512],textPath[512];
		_mkdir(savePath);
		sprintf(tempPath,"%sBearHunterPlayerRecords.tmp",savePath);
		sprintf(realPath,"%sBearHunterPlayerRecords.dat",savePath);
		sprintf(textTmpPath,"%sBearHunterPlayerRecordsTmp.txt",savePath);
		sprintf(textPath,"%sBearHunterPlayerRecords.txt",savePath);
		SaveScores = fopen(tempPath,"w");
		SaveScores2 = fopen(textTmpPath,"w");
		CalculateRank();
		int currentRank = 0;
		BHScoreNode *Current = BHScoreNodeList;
		while (Current)
		{
			if (Current->tmpPlayTime >= 60)
			{
				if (JMG_Bear_Hunter_Game_Control::spawnKarma > 3)
					Current->CheatedRounds++;
				if (!JMG_Bear_Hunter_Game_Control::hasBeenInjured)
					Current->NeverInjured++;
			}
			Current->SupportReceivedInfantryHp += (unsigned long)Current->tmpSupportReceivedInfantryHp;
			Current->SupportReceivedVehicleHp += (unsigned long)Current->tmpSupportReceivedVehicleHp;
			Current->SupportGrantedHpInfantry += (unsigned long)Current->tmpSupportGrantedHpInfantry;
			Current->SupportGrantedHpVehicle += (unsigned long)Current->tmpSupportGrantedHpVehicle;
			Current->SupportReceivedHpInfantrySelf += (unsigned long)Current->tmpSupportReceivedHpInfantrySelf;
			Current->SupportReceivedHpVehicleSelf += (unsigned long)Current->tmpSupportReceivedHpVehicleSelf;
			Current->Rank = ++currentRank;
			char EncryptString[2048];
 			sprintf(EncryptString,"%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu 0",
				Current->PlayTime,Current->PreGameTime,Current->IdleTime,Current->RoundsPlayed,Current->RoundsCompleted,Current->RoundsQuit,Current->RoundsWon,Current->RoundsLost,Current->MostKillsInARound,Current->MostDeathsInARound,Current->MostBonusObjectivesCompletedInARound,Current->Deaths,Current->Kills,Current->VehicleKills,Current->KilledSelf,Current->KilledPlayers,Current->KilledPresident,Current->KilledTurrets,Current->KilledBears,Current->KilledBlackBears,Current->KilledMutantBears,Current->KilledMutantDeer,Current->KilledMutantCats,Current->KilledMutantCatsB,
				Current->KilledMutantCatsR,Current->KilledMutantRabbits,Current->ObjectiveActivatedAlarm,Current->ObjectiveTurretTruck,Current->ObjectiveTurretTruckAlarm,Current->ObjectiveOilRigsActivated,Current->ObjectiveOilRigsRepaired,Current->ObjectiveEngineersSaved,Current->ObjectiveWeaponsFound,Current->ObjectiveWeaponsReturned,Current->ObjectivePlasmaRifleReturned,Current->BonusObjectivesCompleted,Current->PickedupHealthPowerups,Current->PickedupArmorPowerups,Current->PickedupCashPowerups,Current->PickedupAmmoPowerups,Current->PickedupHealthTotal,Current->PickedupArmorTotal,
				Current->PickedupCashTotal,Current->PickedupAmmoTotal,Current->PickedupTotalPowerups,Current->PickedupTotalPowerupsInARound,Current->KilledHumanAi,Current->VehiclesDestroyed,Current->VehiclesLost,Current->JazzsLost,Current->CleasansLost,Current->TrucksLost,Current->TanksLost,Current->TurretTruckLost,Current->C4VestPowerups,Current->ActivatedCommTower,Current->PlayedGamesWithDefenseTurrets,Current->PlayedGamesWithGuardianHelicopter,Current->TimesDrown,Current->TimesFallen,Current->KillsWithSentryTurret,Current->KilledSentryTurrets,Current->SentryTurretsPlaced,
				Current->SentryTurretsLost,Current->PickedUpMedicalNeedle,Current->ReturnedMedicalNeedle,Current->RepairedSubstation,Current->SubstationOnLineAtEnd,Current->SubstationNotDamaged,Current->GiantDeerKilled,Current->SurvivedAlarm,Current->WolfKilled,Current->MutantDogKilled,Current->BlueDeerKilled,Current->CheatedRounds,Current->NeverInjured,Current->MooseKilled,Current->MooseKilled,Current->EatenByRabbit,Current->EatenByRabbit,Current->PickedUpDeerStatue,Current->DroppedDeerStatue,Current->ReturnedDeerStatue,Current->TinyDeerKilled,Current->MutantSquirrelsKilled,
				Current->WildDeerKilled,Current->WildSquirrelsKilled,Current->ArmoredCarsLost,Current->WarriorsLost,Current->TimeOnFoot,Current->TimeInAJazzs,Current->TimeInACleasans,Current->TimeInASecurityTruck,Current->TimeInArmoredCars,Current->TimeInAUDVs,Current->TimeInGatlingTanks,Current->TimeInIFVs,Current->FriendlyTinyDeerKilled,Current->LastPlayTime.day,Current->LastPlayTime.month,Current->LastPlayTime.year,Current->LastPlayTime.second,Current->LastPlayTime.minute,Current->LastPlayTime.hour,Current->LastPlayTime.lTime,Current->SupportReceivedInfantryHp,
				Current->SupportReceivedVehicleHp,Current->SupportReceivedAmmo,Current->SupportGrantedHpInfantry,Current->SupportGrantedHpVehicle,Current->SupportGrantedAmmo,Current->SupportReceivedHpInfantrySelf,Current->SupportReceivedHpVehicleSelf,Current->SupportReceivedAmmoSelf,Current->PettedCougars,Current->KilledCougars,Current->KilledMutantCougars,Current->FailedPettingCougars,Current->KilledFriendlyCougars,Current->DiedTryingToEscapeBase,Current->RescuedCows,Current->KilledCows,Current->LostCows,Current->CompletedCowObjective,Current->KilledMice,
				Current->CompledMiceObjective,Current->KilledPortablePumpJacks,Current->PortablePumpJacksPlaced,Current->PortablePumpJacksLost,Current->PumpJackMoney,Current->MobilePumpJackMoney,Current->Rank,Current->KilledTurkey,Current->ReturnedTurkey);
			fprintf(SaveScores2,"%s\n%s\n",Current->PlayerName,EncryptString);
			fprintf(SaveScores,"%s\n%s",JmgUtility::Rp2Encrypt(Current->PlayerName,25,5),JmgUtility::Rp2Encrypt2(EncryptString,Current->PlayerName[0],Current->PlayerName[1]));
			fprintf(SaveScores,"\n%s",JmgUtility::Rp2Encrypt(EncryptString,Current->PlayerName[1],Current->PlayerName[0]));
			if (Current->next)
				fprintf(SaveScores,"\n");
			Current = Current->next;
		}
		fclose(SaveScores);
		fclose(SaveScores2);
		remove(realPath);
		rename(tempPath,realPath);
		remove(textPath);
		rename(textTmpPath,textPath);
	}
	void LoadData()
	{
		hasLoaded = true;
		char PlayerName[256];
		FILE *LoadScores;
		char realPath[512];
		sprintf(realPath,"%sBearHunterPlayerRecords.dat",savePath);
		LoadScores = fopen(realPath,"r");
		if (LoadScores)
		{
			while (!feof(LoadScores))
			{
				fgets(PlayerName,256,LoadScores);
				int Length = strlen(PlayerName);
				if (Length <= 0)
				{
					Console_Input("msg MutantAssaultHighScoreSystem::LoadData ERROR: Length is less than 1");
					continue;
				}
				PlayerName[Length-1] = '\0';
				BHScoreNode *Current = FindOrAddPlayerBHHighScoreNode(JmgUtility::Rp2Decrypt(PlayerName,25,5));
				char DecryptString[2048],DecryptString2[2048],decryptedString[2048],decryptedString2[2048];
				fgets(DecryptString,2048,LoadScores);
				fgets(DecryptString2,2048,LoadScores);
				sprintf(decryptedString,"%s",JmgUtility::Rp2Decrypt(DecryptString,Current->PlayerName[0],Current->PlayerName[1]));
				sprintf(decryptedString2,"%s",JmgUtility::Rp2Decrypt(DecryptString2,Current->PlayerName[1],Current->PlayerName[0]));
				bool match = true;
				unsigned int stringLength = strlen(decryptedString);
				if (stringLength == strlen(decryptedString2))
					for (unsigned int x = 0;x < stringLength;x++)
						if (decryptedString[x] != decryptedString2[x])
						{
							match = false;
							char noMatchMsg[220];
							sprintf(noMatchMsg,"msg Error: Cou%s%syp%say%st%sr %s","ld ","Not Decr","t Pl","er Da","a Fo",Current->PlayerName);
							Console_Input(noMatchMsg);
							break;
						}
				if (match)
					sscanf(decryptedString,"%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
						&Current->PlayTime,&Current->PreGameTime,&Current->IdleTime,&Current->RoundsPlayed,&Current->RoundsCompleted,&Current->RoundsQuit,&Current->RoundsWon,&Current->RoundsLost,&Current->MostKillsInARound,&Current->MostDeathsInARound,&Current->MostBonusObjectivesCompletedInARound,&Current->Deaths,&Current->Kills,&Current->VehicleKills,&Current->KilledSelf,&Current->KilledPlayers,&Current->KilledPresident,&Current->KilledTurrets,&Current->KilledBears,&Current->KilledBlackBears,&Current->KilledMutantBears,&Current->KilledMutantDeer,&Current->KilledMutantCats,&Current->KilledMutantCatsB,
						&Current->KilledMutantCatsR,&Current->KilledMutantRabbits,&Current->ObjectiveActivatedAlarm,&Current->ObjectiveTurretTruck,&Current->ObjectiveTurretTruckAlarm,&Current->ObjectiveOilRigsActivated,&Current->ObjectiveOilRigsRepaired,&Current->ObjectiveEngineersSaved,&Current->ObjectiveWeaponsFound,&Current->ObjectiveWeaponsReturned,&Current->ObjectivePlasmaRifleReturned,&Current->BonusObjectivesCompleted,&Current->PickedupHealthPowerups,&Current->PickedupArmorPowerups,&Current->PickedupCashPowerups,&Current->PickedupAmmoPowerups,&Current->PickedupHealthTotal,&Current->PickedupArmorTotal,
						&Current->PickedupCashTotal,&Current->PickedupAmmoTotal,&Current->PickedupTotalPowerups,&Current->PickedupTotalPowerupsInARound,&Current->KilledHumanAi,&Current->VehiclesDestroyed,&Current->VehiclesLost,&Current->JazzsLost,&Current->CleasansLost,&Current->TrucksLost,&Current->TanksLost,&Current->TurretTruckLost,&Current->C4VestPowerups,&Current->ActivatedCommTower,&Current->PlayedGamesWithDefenseTurrets,&Current->PlayedGamesWithGuardianHelicopter,&Current->TimesDrown,&Current->TimesFallen,&Current->KillsWithSentryTurret,&Current->KilledSentryTurrets,&Current->SentryTurretsPlaced,
						&Current->SentryTurretsLost,&Current->PickedUpMedicalNeedle,&Current->ReturnedMedicalNeedle,&Current->RepairedSubstation,&Current->SubstationOnLineAtEnd,&Current->SubstationNotDamaged,&Current->GiantDeerKilled,&Current->SurvivedAlarm,&Current->WolfKilled,&Current->MutantDogKilled,&Current->BlueDeerKilled,&Current->CheatedRounds,&Current->NeverInjured,&Current->MooseKilled,&Current->MooseKilled,&Current->EatenByRabbit,&Current->EatenByRabbit,&Current->PickedUpDeerStatue,&Current->DroppedDeerStatue,&Current->ReturnedDeerStatue,&Current->TinyDeerKilled,&Current->MutantSquirrelsKilled,
						&Current->WildDeerKilled,&Current->WildSquirrelsKilled,&Current->ArmoredCarsLost,&Current->WarriorsLost,&Current->TimeOnFoot,&Current->TimeInAJazzs,&Current->TimeInACleasans,&Current->TimeInASecurityTruck,&Current->TimeInArmoredCars,&Current->TimeInAUDVs,&Current->TimeInGatlingTanks,&Current->TimeInIFVs,&Current->FriendlyTinyDeerKilled,&Current->LastPlayTime.day,&Current->LastPlayTime.month,&Current->LastPlayTime.year,&Current->LastPlayTime.second,&Current->LastPlayTime.minute,&Current->LastPlayTime.hour,&Current->LastPlayTime.lTime,&Current->SupportReceivedInfantryHp,
						&Current->SupportReceivedVehicleHp,&Current->SupportReceivedAmmo,&Current->SupportGrantedHpInfantry,&Current->SupportGrantedHpVehicle,&Current->SupportGrantedAmmo,&Current->SupportReceivedHpInfantrySelf,&Current->SupportReceivedHpVehicleSelf,&Current->SupportReceivedAmmoSelf,&Current->PettedCougars,&Current->KilledCougars,&Current->KilledMutantCougars,&Current->FailedPettingCougars,&Current->KilledFriendlyCougars,&Current->DiedTryingToEscapeBase,&Current->RescuedCows,&Current->KilledCows,&Current->LostCows,&Current->CompletedCowObjective,&Current->KilledMice,&Current->CompledMiceObjective,
						&Current->KilledPortablePumpJacks,&Current->PortablePumpJacksPlaced,&Current->PortablePumpJacksLost,&Current->PumpJackMoney,&Current->MobilePumpJackMoney,&Current->Rank,&Current->KilledTurkey,&Current->ReturnedTurkey);
			}
			fclose(LoadScores);	
		}
	}
	inline BHScoreNode *EveluateHighestScore(unsigned long highValue,unsigned long currentValue,BHScoreNode *High,BHScoreNode *Current)
	{
		if (highValue == currentValue)
		{
			selectRandomMatchingScore++;
			if (Commands->Get_Random(0.0f,(float)selectRandomMatchingScore) < 0.5)
				return Current;
		}
		else if (highValue < currentValue)
			selectRandomMatchingScore = 0;
		return (highValue >= currentValue ? High : Current);
	}
	BHScoreNode *CompareHighScoreNodes(BHScoreNode *High,BHScoreNode *Current,int CompVal)
	{
		if (!High)
			return Current;
		switch (CompVal)
		{
		case 0: return EveluateHighestScore(High->PlayTime,Current->PlayTime,High,Current);
		case 1: return EveluateHighestScore(High->PreGameTime,Current->PreGameTime,High,Current);
		case 2: return EveluateHighestScore(High->IdleTime,Current->IdleTime,High,Current);
		case 3: return EveluateHighestScore(High->RoundsPlayed,Current->RoundsPlayed,High,Current);
		case 4: return EveluateHighestScore(High->RoundsCompleted,Current->RoundsCompleted,High,Current);
		case 5: return EveluateHighestScore(High->RoundsQuit,Current->RoundsQuit,High,Current);
		case 6: return EveluateHighestScore(High->RoundsWon,Current->RoundsWon,High,Current);
		case 7: return EveluateHighestScore(High->RoundsLost,Current->RoundsLost,High,Current);
		case 8: return EveluateHighestScore(High->MostKillsInARound,Current->MostKillsInARound,High,Current);
		case 9: return EveluateHighestScore(High->MostDeathsInARound,Current->MostDeathsInARound,High,Current);
		case 10: return EveluateHighestScore(High->MostBonusObjectivesCompletedInARound,Current->MostBonusObjectivesCompletedInARound,High,Current);
		case 11: return EveluateHighestScore(High->Deaths,Current->Deaths,High,Current);
		case 12: return EveluateHighestScore(High->Kills,Current->Kills,High,Current);
		case 13: return EveluateHighestScore(High->VehicleKills,Current->VehicleKills,High,Current);
		case 14: return EveluateHighestScore(High->KilledSelf,Current->KilledSelf,High,Current);
		case 15: return EveluateHighestScore(High->KilledPlayers,Current->KilledPlayers,High,Current);
		case 16: return EveluateHighestScore(High->KilledPresident,Current->KilledPresident,High,Current);
		case 17: return EveluateHighestScore(High->KilledTurrets,Current->KilledTurrets,High,Current);
		case 18: return EveluateHighestScore(High->KilledBears,Current->KilledBears,High,Current);
		case 19: return EveluateHighestScore(High->KilledBlackBears,Current->KilledBlackBears,High,Current);
		case 20: return EveluateHighestScore(High->KilledMutantBears,Current->KilledMutantBears,High,Current);
		case 21: return EveluateHighestScore(High->KilledMutantDeer,Current->KilledMutantDeer,High,Current);
		case 22: return EveluateHighestScore(High->KilledMutantCats,Current->KilledMutantCats,High,Current);
		case 23: return EveluateHighestScore(High->KilledMutantCatsB,Current->KilledMutantCatsB,High,Current);
		case 24: return EveluateHighestScore(High->KilledMutantCatsR,Current->KilledMutantCatsR,High,Current);
		case 25: return EveluateHighestScore(High->KilledMutantRabbits,Current->KilledMutantRabbits,High,Current);
		case 26: return EveluateHighestScore(High->ObjectiveActivatedAlarm,Current->ObjectiveActivatedAlarm,High,Current);
		case 27: return EveluateHighestScore(High->ObjectiveTurretTruck,Current->ObjectiveTurretTruck,High,Current);
		case 28: return EveluateHighestScore(High->ObjectiveTurretTruckAlarm,Current->ObjectiveTurretTruckAlarm,High,Current);
		case 29: return EveluateHighestScore(High->ObjectiveOilRigsActivated,Current->ObjectiveOilRigsActivated,High,Current);
		case 30: return EveluateHighestScore(High->ObjectiveOilRigsRepaired,Current->ObjectiveOilRigsRepaired,High,Current);
		case 31: return EveluateHighestScore(High->ObjectiveEngineersSaved,Current->ObjectiveEngineersSaved,High,Current);
		case 32: return EveluateHighestScore(High->ObjectiveWeaponsFound,Current->ObjectiveWeaponsFound,High,Current);
		case 33: return EveluateHighestScore(High->ObjectiveWeaponsReturned,Current->ObjectiveWeaponsReturned,High,Current);
		case 34: return EveluateHighestScore(High->ObjectivePlasmaRifleReturned,Current->ObjectivePlasmaRifleReturned,High,Current);
		case 35: return EveluateHighestScore(High->BonusObjectivesCompleted,Current->BonusObjectivesCompleted,High,Current);
		case 36: return EveluateHighestScore(High->PickedupHealthPowerups,Current->PickedupHealthPowerups,High,Current);
		case 37: return EveluateHighestScore(High->PickedupArmorPowerups,Current->PickedupArmorPowerups,High,Current);
		case 38: return EveluateHighestScore(High->PickedupCashPowerups,Current->PickedupCashPowerups,High,Current);
		case 39: return EveluateHighestScore(High->PickedupAmmoPowerups,Current->PickedupAmmoPowerups,High,Current);
		case 40: return EveluateHighestScore(High->PickedupHealthTotal,Current->PickedupHealthTotal,High,Current);
		case 41: return EveluateHighestScore(High->PickedupArmorTotal,Current->PickedupArmorTotal,High,Current);
		case 42: return EveluateHighestScore(High->PickedupCashTotal,Current->PickedupCashTotal,High,Current);
		case 43: return EveluateHighestScore(High->PickedupAmmoTotal,Current->PickedupAmmoTotal,High,Current);
		case 44: return EveluateHighestScore(High->PickedupTotalPowerups,Current->PickedupTotalPowerups,High,Current);
		case 45: return EveluateHighestScore(High->PickedupTotalPowerupsInARound,Current->PickedupTotalPowerupsInARound,High,Current);
		case 46: return EveluateHighestScore(High->KilledHumanAi,Current->KilledHumanAi,High,Current);
		case 47: return EveluateHighestScore(High->VehiclesDestroyed,Current->VehiclesDestroyed,High,Current);
		case 48: return EveluateHighestScore(High->VehiclesLost,Current->VehiclesLost,High,Current);
		case 49: return EveluateHighestScore(High->JazzsLost,Current->JazzsLost,High,Current);
		case 50: return EveluateHighestScore(High->CleasansLost,Current->CleasansLost,High,Current);
		case 51: return EveluateHighestScore(High->TrucksLost,Current->TrucksLost,High,Current);
		case 52: return EveluateHighestScore(High->TanksLost,Current->TanksLost,High,Current);
		case 53: return EveluateHighestScore(High->TurretTruckLost,Current->TurretTruckLost,High,Current);
		case 54: return EveluateHighestScore(High->C4VestPowerups,Current->C4VestPowerups,High,Current);
		case 55: return EveluateHighestScore(High->ActivatedCommTower,Current->ActivatedCommTower,High,Current);
		case 56: return EveluateHighestScore(High->PlayedGamesWithDefenseTurrets,Current->PlayedGamesWithDefenseTurrets,High,Current);
		case 57: return EveluateHighestScore(High->PlayedGamesWithGuardianHelicopter,Current->PlayedGamesWithGuardianHelicopter,High,Current);
		case 58: return EveluateHighestScore(High->TimesDrown,Current->TimesDrown,High,Current);
		case 59: return EveluateHighestScore(High->TimesFallen,Current->TimesFallen,High,Current);
		case 60: return EveluateHighestScore(High->KillsWithSentryTurret,Current->KillsWithSentryTurret,High,Current);
		case 61: return EveluateHighestScore(High->KilledSentryTurrets,Current->KilledSentryTurrets,High,Current);
		case 62: return EveluateHighestScore(High->SentryTurretsPlaced,Current->SentryTurretsPlaced,High,Current);
		case 63: return EveluateHighestScore(High->SentryTurretsLost,Current->SentryTurretsLost,High,Current);
		case 64: return EveluateHighestScore(High->PickedUpMedicalNeedle,Current->PickedUpMedicalNeedle,High,Current);
		case 65: return EveluateHighestScore(High->ReturnedMedicalNeedle,Current->ReturnedMedicalNeedle,High,Current);
		case 66: return EveluateHighestScore(High->RepairedSubstation,Current->RepairedSubstation,High,Current);
		case 67: return EveluateHighestScore(High->SubstationOnLineAtEnd,Current->SubstationOnLineAtEnd,High,Current);
		case 68: return EveluateHighestScore(High->SubstationNotDamaged,Current->SubstationNotDamaged,High,Current);
		case 69: return EveluateHighestScore(High->GiantDeerKilled,Current->GiantDeerKilled,High,Current);
		case 70: return EveluateHighestScore(High->SurvivedAlarm,Current->SurvivedAlarm,High,Current);
		case 71: return EveluateHighestScore(High->WolfKilled,Current->WolfKilled,High,Current);
		case 72: return EveluateHighestScore(High->MutantDogKilled,Current->MutantDogKilled,High,Current);
		case 73: return EveluateHighestScore(High->BlueDeerKilled,Current->BlueDeerKilled,High,Current);
		case 74: return EveluateHighestScore(High->CheatedRounds,Current->CheatedRounds,High,Current);
		case 75: return EveluateHighestScore(High->NeverInjured,Current->NeverInjured,High,Current);
		case 76: return EveluateHighestScore(High->MooseKilled,Current->MooseKilled,High,Current);
		case 77: return EveluateHighestScore(High->EatenByRabbit,Current->EatenByRabbit,High,Current);
		case 78: return EveluateHighestScore(High->PickedUpDeerStatue,Current->PickedUpDeerStatue,High,Current);
		case 79: return EveluateHighestScore(High->DroppedDeerStatue,Current->DroppedDeerStatue,High,Current);
		case 80: return EveluateHighestScore(High->ReturnedDeerStatue,Current->ReturnedDeerStatue,High,Current);
		case 81: return EveluateHighestScore(High->TinyDeerKilled,Current->TinyDeerKilled,High,Current);
		case 82: return EveluateHighestScore(High->MutantSquirrelsKilled,Current->MutantSquirrelsKilled,High,Current);
		case 83: return EveluateHighestScore(High->WildDeerKilled,Current->WildDeerKilled,High,Current);
		case 84: return EveluateHighestScore(High->WildSquirrelsKilled,Current->WildSquirrelsKilled,High,Current);
		case 85: return EveluateHighestScore(High->ArmoredCarsLost,Current->ArmoredCarsLost,High,Current);
		case 86: return EveluateHighestScore(High->WarriorsLost,Current->WarriorsLost,High,Current);
		case 87: return EveluateHighestScore(High->TimeOnFoot,Current->TimeOnFoot,High,Current);
		case 88: return EveluateHighestScore(High->TimeInAJazzs,Current->TimeInAJazzs,High,Current);
		case 89: return EveluateHighestScore(High->TimeInACleasans,Current->TimeInACleasans,High,Current);
		case 90: return EveluateHighestScore(High->TimeInASecurityTruck,Current->TimeInASecurityTruck,High,Current);
		case 91: return EveluateHighestScore(High->TimeInArmoredCars,Current->TimeInArmoredCars,High,Current);
		case 92: return EveluateHighestScore(High->TimeInAUDVs,Current->TimeInAUDVs,High,Current);
		case 93: return EveluateHighestScore(High->TimeInGatlingTanks,Current->TimeInGatlingTanks,High,Current);
		case 94: return EveluateHighestScore(High->TimeInIFVs,Current->TimeInIFVs,High,Current);
		case 95: return EveluateHighestScore(High->FriendlyTinyDeerKilled,Current->FriendlyTinyDeerKilled,High,Current);
		case 96: return EveluateHighestScore(High->SupportReceivedInfantryHp,Current->SupportReceivedInfantryHp,High,Current);
		case 97: return EveluateHighestScore(High->SupportReceivedVehicleHp,Current->SupportReceivedVehicleHp,High,Current);
		case 98: return EveluateHighestScore(High->SupportReceivedAmmo,Current->SupportReceivedAmmo,High,Current);
		case 99: return EveluateHighestScore(High->SupportGrantedHpInfantry,Current->SupportGrantedHpInfantry,High,Current);
		case 100: return EveluateHighestScore(High->SupportGrantedHpVehicle,Current->SupportGrantedHpVehicle,High,Current);
		case 101: return EveluateHighestScore(High->SupportGrantedAmmo,Current->SupportGrantedAmmo,High,Current);
		case 102: return EveluateHighestScore(High->SupportReceivedHpInfantrySelf,Current->SupportReceivedHpInfantrySelf,High,Current);
		case 103: return EveluateHighestScore(High->SupportReceivedHpVehicleSelf,Current->SupportReceivedHpVehicleSelf,High,Current);
		case 104: return EveluateHighestScore(High->SupportReceivedAmmoSelf,Current->SupportReceivedAmmoSelf,High,Current);
		case 105: return EveluateHighestScore(High->PettedCougars,Current->PettedCougars,High,Current);
		case 106: return EveluateHighestScore(High->KilledCougars,Current->KilledCougars,High,Current);
		case 107: return EveluateHighestScore(High->KilledMutantCougars,Current->KilledMutantCougars,High,Current);
		case 108: return EveluateHighestScore(High->FailedPettingCougars,Current->FailedPettingCougars,High,Current);
		case 109: return EveluateHighestScore(High->KilledFriendlyCougars,Current->KilledFriendlyCougars,High,Current);
		case 110: return EveluateHighestScore(High->DiedTryingToEscapeBase,Current->DiedTryingToEscapeBase,High,Current);
		case 111: return EveluateHighestScore(High->RescuedCows,Current->RescuedCows,High,Current);
		case 112: return EveluateHighestScore(High->KilledCows,Current->KilledCows,High,Current);
		case 113: return EveluateHighestScore(High->LostCows,Current->LostCows,High,Current);
		case 114: return EveluateHighestScore(High->CompletedCowObjective,Current->CompletedCowObjective,High,Current);
		case 115: return EveluateHighestScore(High->KilledMice,Current->KilledMice,High,Current);
		case 116: return EveluateHighestScore(High->CompledMiceObjective,Current->CompledMiceObjective,High,Current);
		case 117: return EveluateHighestScore(High->KilledPortablePumpJacks,Current->KilledPortablePumpJacks,High,Current);
		case 118: return EveluateHighestScore(High->PortablePumpJacksPlaced,Current->PortablePumpJacksPlaced,High,Current);
		case 119: return EveluateHighestScore(High->PortablePumpJacksLost,Current->PortablePumpJacksLost,High,Current);
		case 120: return EveluateHighestScore(High->PumpJackMoney,Current->PumpJackMoney,High,Current);
		case 121: return EveluateHighestScore(High->MobilePumpJackMoney,Current->MobilePumpJackMoney,High,Current);
		case 122: return EveluateHighestScore(High->KilledTurkey,Current->KilledTurkey,High,Current);
		case 123: return EveluateHighestScore(High->ReturnedTurkey,Current->ReturnedTurkey,High,Current);
		default: return High;
		}
	}
	bool ReturnHighScoreNodeValue(BHScoreNode *Node,int CompVal)
	{
		if (!Node)
			return false;
		switch (CompVal)
		{
		case 0: return Node->PlayTime ? true : false;
		case 1: return Node->PreGameTime ? true : false;
		case 2: return Node->IdleTime ? true : false;
		case 3: return Node->RoundsPlayed ? true : false;
		case 4: return Node->RoundsCompleted ? true : false;
		case 5: return Node->RoundsQuit ? true : false;
		case 6: return Node->RoundsWon ? true : false;
		case 7: return Node->RoundsLost ? true : false;
		case 8: return Node->MostKillsInARound ? true : false;
		case 9: return Node->MostDeathsInARound ? true : false;
		case 10: return Node->MostBonusObjectivesCompletedInARound ? true : false;
		case 11: return Node->Deaths ? true : false;
		case 12: return Node->Kills ? true : false;
		case 13: return Node->VehicleKills ? true : false;
		case 14: return Node->KilledSelf ? true : false;
		case 15: return Node->KilledPlayers ? true : false;
		case 16: return Node->KilledPresident ? true : false;
		case 17: return Node->KilledTurrets ? true : false;
		case 18: return Node->KilledBears ? true : false;
		case 19: return Node->KilledBlackBears ? true : false;
		case 20: return Node->KilledMutantBears ? true : false;
		case 21: return Node->KilledMutantDeer ? true : false;
		case 22: return Node->KilledMutantCats ? true : false;
		case 23: return Node->KilledMutantCatsB ? true : false;
		case 24: return Node->KilledMutantCatsR ? true : false;
		case 25: return Node->KilledMutantRabbits ? true : false;
		case 26: return Node->ObjectiveActivatedAlarm ? true : false;
		case 27: return Node->ObjectiveTurretTruck ? true : false;
		case 28: return Node->ObjectiveTurretTruckAlarm ? true : false;
		case 29: return Node->ObjectiveOilRigsActivated ? true : false;
		case 30: return Node->ObjectiveOilRigsRepaired ? true : false;
		case 31: return Node->ObjectiveEngineersSaved ? true : false;
		case 32: return Node->ObjectiveWeaponsFound ? true : false;
		case 33: return Node->ObjectiveWeaponsReturned ? true : false;
		case 34: return Node->ObjectivePlasmaRifleReturned ? true : false;
		case 35: return Node->BonusObjectivesCompleted ? true : false;
		case 36: return Node->PickedupHealthPowerups ? true : false;
		case 37: return Node->PickedupArmorPowerups ? true : false;
		case 38: return Node->PickedupCashPowerups ? true : false;
		case 39: return Node->PickedupAmmoPowerups ? true : false;
		case 40: return Node->PickedupHealthTotal ? true : false;
		case 41: return Node->PickedupArmorTotal ? true : false;
		case 42: return Node->PickedupCashTotal ? true : false;
		case 43: return Node->PickedupAmmoTotal ? true : false;
		case 44: return Node->PickedupTotalPowerups ? true : false;
		case 45: return Node->PickedupTotalPowerupsInARound ? true : false;
		case 46: return Node->KilledHumanAi ? true : false;
		case 47: return Node->VehiclesDestroyed ? true : false;
		case 48: return Node->VehiclesLost ? true : false;
		case 49: return Node->JazzsLost ? true : false;
		case 50: return Node->CleasansLost ? true : false;
		case 51: return Node->TrucksLost ? true : false;
		case 52: return Node->TanksLost ? true : false;
		case 53: return Node->TurretTruckLost ? true : false;
		case 54: return Node->C4VestPowerups ? true : false;
		case 55: return Node->ActivatedCommTower ? true : false;
		case 56: return Node->PlayedGamesWithDefenseTurrets ? true : false;
		case 57: return Node->PlayedGamesWithGuardianHelicopter ? true : false;
		case 58: return Node->TimesDrown ? true : false;
		case 59: return Node->TimesFallen ? true : false;
		case 60: return Node->KillsWithSentryTurret ? true : false;
		case 61: return Node->KilledSentryTurrets ? true : false;
		case 62: return Node->SentryTurretsPlaced ? true : false;
		case 63: return Node->SentryTurretsLost ? true : false;
		case 64: return Node->PickedUpMedicalNeedle ? true : false;
		case 65: return Node->ReturnedMedicalNeedle ? true : false;
		case 66: return Node->RepairedSubstation ? true : false;
		case 67: return Node->SubstationOnLineAtEnd ? true : false;
		case 68: return Node->RepairedSubstation ? true : false;
		case 69: return Node->GiantDeerKilled ? true : false;
		case 70: return Node->SurvivedAlarm ? true : false;
		case 71: return Node->WolfKilled ? true : false;
		case 72: return Node->MutantDogKilled ? true : false;
		case 73: return Node->BlueDeerKilled ? true : false;
		case 74: return Node->CheatedRounds ? true : false;
		case 75: return Node->NeverInjured ? true : false;
		case 76: return Node->MooseKilled ? true : false;
		case 77: return Node->EatenByRabbit ? true : false;
		case 78: return Node->PickedUpDeerStatue ? true : false;
		case 79: return Node->DroppedDeerStatue ? true : false;
		case 80: return Node->ReturnedDeerStatue ? true : false;
		case 81: return Node->TinyDeerKilled ? true : false;
		case 82: return Node->MutantSquirrelsKilled ? true : false;
		case 83: return Node->WildDeerKilled ? true : false;
		case 84: return Node->WildSquirrelsKilled ? true : false;
		case 85: return Node->ArmoredCarsLost ? true : false;
		case 86: return Node->WarriorsLost ? true : false;
		case 87: return Node->TimeOnFoot ? true : false;
		case 88: return Node->TimeInAJazzs ? true : false;
		case 89: return Node->TimeInACleasans ? true : false;
		case 90: return Node->TimeInASecurityTruck ? true : false;
		case 91: return Node->TimeInArmoredCars ? true : false;
		case 92: return Node->TimeInAUDVs ? true : false;
		case 93: return Node->TimeInGatlingTanks ? true : false;
		case 94: return Node->TimeInIFVs ? true : false;
		case 95: return Node->FriendlyTinyDeerKilled ? true : false;
		case 96: return Node->SupportReceivedInfantryHp ? true : false;
		case 97: return Node->SupportReceivedVehicleHp ? true : false;
		case 98: return Node->SupportReceivedAmmo ? true : false;
		case 99: return Node->SupportGrantedHpInfantry ? true : false;
		case 100: return Node->SupportGrantedHpVehicle ? true : false;
		case 101: return Node->SupportGrantedAmmo ? true : false;
		case 102: return Node->SupportReceivedHpInfantrySelf ? true : false;
		case 103: return Node->SupportReceivedHpVehicleSelf ? true : false;
		case 104: return Node->SupportReceivedAmmoSelf ? true : false;
		case 105: return Node->PettedCougars ? true : false;
		case 106: return Node->KilledCougars ? true : false;
		case 107: return Node->KilledMutantCougars ? true : false;
		case 108: return Node->FailedPettingCougars ? true : false;
		case 109: return Node->KilledFriendlyCougars ? true : false;
		case 110: return Node->DiedTryingToEscapeBase ? true : false;
		case 111: return Node->RescuedCows ? true : false;
		case 112: return Node->KilledCows ? true : false;
		case 113: return Node->LostCows ? true : false;
		case 114: return Node->CompletedCowObjective ? true : false;
		case 115: return Node->KilledMice ? true : false;
		case 116: return Node->CompledMiceObjective ? true : false;
		case 117: return Node->KilledPortablePumpJacks ? true : false;
		case 118: return Node->PortablePumpJacksPlaced ? true : false;
		case 119: return Node->PortablePumpJacksLost ? true : false;
		case 120: return Node->PumpJackMoney ? true : false;
		case 121: return Node->MobilePumpJackMoney ? true : false;
		case 122: return Node->KilledTurkey ? true : false;
		case 123: return Node->ReturnedTurkey ? true : false;
		default: Console_Input("msg SCORE SYSTEM ERROR: Out of bounds!");return false;
		}
	}
	char *ReturnScore(BHScoreNode *High,int CompVal)
	{
		static char RetChar[200];
		if (!High)
			return " ";
		switch (CompVal)
		{
		case 0: sprintf(RetChar,"Server Record: %s has played bear hunter for %s mintues.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PlayTime/60.0f));return RetChar;
		case 1: sprintf(RetChar,"Server Record: %s has dicked around for %s minutes before finally leaving the base to go hunting.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PreGameTime/60.0f));return RetChar;
		case 2: sprintf(RetChar,"Server Record: %s has been idle for %s minutes.",High->PlayerName,JmgUtility::formatDigitGrouping(High->IdleTime/60.0f));return RetChar;
		case 3: sprintf(RetChar,"Server Record: %s has joined %s rounds of bear hunter.",High->PlayerName,JmgUtility::formatDigitGrouping(High->RoundsPlayed));return RetChar;
		case 4: sprintf(RetChar,"Server Record: %s has completed %s rounds of bear hunter.",High->PlayerName,JmgUtility::formatDigitGrouping(High->RoundsCompleted));return RetChar;
		case 5: sprintf(RetChar,"Server Record: %s has quit %s rounds of bear hunter.",High->PlayerName,JmgUtility::formatDigitGrouping(High->RoundsQuit));return RetChar;
		case 6: sprintf(RetChar,"Server Record: %s has won %s rounds of bear hunter.",High->PlayerName,JmgUtility::formatDigitGrouping(High->RoundsWon));return RetChar;
		case 7: sprintf(RetChar,"Server Record: %s has lost %s rounds of bear hunter.",High->PlayerName,JmgUtility::formatDigitGrouping(High->RoundsLost));return RetChar;
		case 8: sprintf(RetChar,"Server Record: %s has the most kills in a round: %s.",High->PlayerName,JmgUtility::formatDigitGrouping(High->MostKillsInARound));return RetChar;
		case 9: sprintf(RetChar,"Server Record: %s has the most deaths in a round: %s.",High->PlayerName,JmgUtility::formatDigitGrouping(High->MostDeathsInARound));return RetChar;
		case 10: sprintf(RetChar,"Server Record: %s has completed the most objectives in a round: %s.",High->PlayerName,JmgUtility::formatDigitGrouping(High->MostBonusObjectivesCompletedInARound));return RetChar;
		case 11: sprintf(RetChar,"Server Record: %s has %s deaths.",High->PlayerName,JmgUtility::formatDigitGrouping(High->Deaths));return RetChar;
		case 12: sprintf(RetChar,"Server Record: %s has %s kills.",High->PlayerName,JmgUtility::formatDigitGrouping(High->Kills));return RetChar;
		case 13: sprintf(RetChar,"Server Record: %s has %s vehicle kills.",High->PlayerName,JmgUtility::formatDigitGrouping(High->VehicleKills));return RetChar;
		case 14: sprintf(RetChar,"Server Record: %s has comitted suicide %s times.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledSelf));return RetChar;
		case 15: sprintf(RetChar,"Server Record: %s has killed %s other players.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledPlayers));return RetChar;
		case 16: sprintf(RetChar,"Server Record: %s has killed the President of Corporate America %s times.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledPresident));return RetChar;
		case 17: sprintf(RetChar,"Server Record: %s has killed %s shredder turrets.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledTurrets));return RetChar;
		case 18: sprintf(RetChar,"Server Record: %s has killed %s brown bears.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledBears));return RetChar;
		case 19: sprintf(RetChar,"Server Record: %s has killed %s black bears.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledBlackBears));return RetChar;
		case 20: sprintf(RetChar,"Server Record: %s has killed %s mutant bears.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledMutantBears));return RetChar;
		case 21: sprintf(RetChar,"Server Record: %s has killed %s mutant deer.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledMutantDeer));return RetChar;
		case 22: sprintf(RetChar,"Server Record: %s has killed %s mutant cats.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledMutantCats));return RetChar;
		case 23: sprintf(RetChar,"Server Record: %s has killed %s mutant ion cats.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledMutantCatsB));return RetChar;
		case 24: sprintf(RetChar,"Server Record: %s has killed %s mutant nuclear cats.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledMutantCatsR));return RetChar;
		case 25: sprintf(RetChar,"Server Record: %s has killed the boss rabbit %s times.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledMutantRabbits));return RetChar;
		case 26: sprintf(RetChar,"Server Record: %s has activated the alarm %s times.",High->PlayerName,JmgUtility::formatDigitGrouping(High->ObjectiveActivatedAlarm));return RetChar;
		case 27: sprintf(RetChar,"Server Record: %s has returned the security truck %s times.",High->PlayerName,JmgUtility::formatDigitGrouping(High->ObjectiveTurretTruck));return RetChar;
		case 28: sprintf(RetChar,"Server Record: %s has deactivated the security alarm %s times.",High->PlayerName,JmgUtility::formatDigitGrouping(High->ObjectiveTurretTruckAlarm));return RetChar;
		case 29: sprintf(RetChar,"Server Record: %s has activated %s pump jacks.",High->PlayerName,JmgUtility::formatDigitGrouping(High->ObjectiveOilRigsActivated));return RetChar;
		case 30: sprintf(RetChar,"Server Record: %s has repaired %s pump jacks.",High->PlayerName,JmgUtility::formatDigitGrouping(High->ObjectiveOilRigsRepaired));return RetChar;
		case 31: sprintf(RetChar,"Server Record: %s has saved %s engineers.",High->PlayerName,JmgUtility::formatDigitGrouping(High->ObjectiveEngineersSaved));return RetChar;
		case 32: sprintf(RetChar,"Server Record: %s has found %s weapon containers.",High->PlayerName,JmgUtility::formatDigitGrouping(High->ObjectiveWeaponsFound));return RetChar;
		case 33: sprintf(RetChar,"Server Record: %s has returned %s weapon containers.",High->PlayerName,JmgUtility::formatDigitGrouping(High->ObjectiveWeaponsReturned));return RetChar;
		case 34: sprintf(RetChar,"Server Record: %s has retrived the plasma rifle weapon container %s times.",High->PlayerName,JmgUtility::formatDigitGrouping(High->ObjectivePlasmaRifleReturned));return RetChar;
		case 35: sprintf(RetChar,"Server Record: %s has completed %s bonus objectives.",High->PlayerName,JmgUtility::formatDigitGrouping(High->BonusObjectivesCompleted));return RetChar;
		case 36: sprintf(RetChar,"Server Record: %s has acquired %s health powerups.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PickedupHealthPowerups));return RetChar;
		case 37: sprintf(RetChar,"Server Record: %s has acquired %s armor powerups.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PickedupArmorPowerups));return RetChar;
		case 38: sprintf(RetChar,"Server Record: %s has acquired %s cash powerups.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PickedupCashPowerups));return RetChar;
		case 39: sprintf(RetChar,"Server Record: %s has acquired %s ammo powerups.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PickedupAmmoPowerups));return RetChar;
		case 40: sprintf(RetChar,"Server Record: %s has acquired %s health from health powerups.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PickedupHealthTotal));return RetChar;
		case 41: sprintf(RetChar,"Server Record: %s has acquired %s armor from armor powerups.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PickedupArmorTotal));return RetChar;
		case 42: sprintf(RetChar,"Server Record: %s has acquired $%s.00 dollars from cash powerups.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PickedupCashTotal));return RetChar;
		case 43: sprintf(RetChar,"Server Record: %s has acquired %s clips of ammo from ammo powerups.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PickedupAmmoTotal));return RetChar;
		case 44: sprintf(RetChar,"Server Record: %s has acquired %s powerups.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PickedupTotalPowerups));return RetChar;
		case 45: sprintf(RetChar,"Server Record: %s has acquired %s powerups in a round.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PickedupTotalPowerupsInARound));return RetChar;
		case 46: sprintf(RetChar,"Server Record: %s has killed %s friendly AI.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledHumanAi));return RetChar;
		case 47: sprintf(RetChar,"Server Record: %s has destroyed %s vehicles.",High->PlayerName,JmgUtility::formatDigitGrouping(High->VehiclesDestroyed));return RetChar;
		case 48: sprintf(RetChar,"Server Record: %s has lost %s vehicles.",High->PlayerName,JmgUtility::formatDigitGrouping(High->VehiclesLost));return RetChar;
		case 49: sprintf(RetChar,"Server Record: %s has lost %s Punda Jazzs.",High->PlayerName,JmgUtility::formatDigitGrouping(High->JazzsLost));return RetChar;
		case 50: sprintf(RetChar,"Server Record: %s has lost %s Cleasan Primera.",High->PlayerName,JmgUtility::formatDigitGrouping(High->CleasansLost));return RetChar;
		case 51: sprintf(RetChar,"Server Record: %s has lost %s Urban Defense Vehicles.",High->PlayerName,JmgUtility::formatDigitGrouping(High->TrucksLost));return RetChar;
		case 52: sprintf(RetChar,"Server Record: %s has lost %s Gattling Tanks.",High->PlayerName,JmgUtility::formatDigitGrouping(High->TanksLost));return RetChar;
		case 53: sprintf(RetChar,"Server Record: %s has lost %s Cargo Trucks.",High->PlayerName,JmgUtility::formatDigitGrouping(High->TurretTruckLost));return RetChar;
		case 54: sprintf(RetChar,"Server Record: %s has acquired %s C4 Vests.",High->PlayerName,JmgUtility::formatDigitGrouping(High->C4VestPowerups));return RetChar;
		case 55: sprintf(RetChar,"Server Record: %s activated the transmitter %s times.",High->PlayerName,JmgUtility::formatDigitGrouping(High->ActivatedCommTower));return RetChar;
		case 56: sprintf(RetChar,"Server Record: %s played in %s games where the security truck has been returned to base.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PlayedGamesWithDefenseTurrets));return RetChar;
		case 57: sprintf(RetChar,"Server Record: %s has played %s games with assistance from Comanche 4573.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PlayedGamesWithGuardianHelicopter));return RetChar;
		case 58: sprintf(RetChar,"Server Record: %s has gone too far from the shore %s times.",High->PlayerName,JmgUtility::formatDigitGrouping(High->TimesDrown));return RetChar;
		case 59: sprintf(RetChar,"Server Record: %s has fell to their death %s times.",High->PlayerName,JmgUtility::formatDigitGrouping(High->TimesFallen));return RetChar;
		case 60: sprintf(RetChar,"Server Record: %s has got %s kills with a sentry turret.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KillsWithSentryTurret));return RetChar;
		case 61: sprintf(RetChar,"Server Record: %s has destroyed %s sentry turrets.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledSentryTurrets));return RetChar;
		case 62: sprintf(RetChar,"Server Record: %s has placed %s sentry turrets.",High->PlayerName,JmgUtility::formatDigitGrouping(High->SentryTurretsPlaced));return RetChar;
		case 63: sprintf(RetChar,"Server Record: %s has lost %s sentry turrets.",High->PlayerName,JmgUtility::formatDigitGrouping(High->SentryTurretsLost));return RetChar;
		case 64: sprintf(RetChar,"Server Record: %s has picked up the medical needle %s times.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PickedUpMedicalNeedle));return RetChar;
		case 65: sprintf(RetChar,"Server Record: %s has returned the medical needle %s times.",High->PlayerName,JmgUtility::formatDigitGrouping(High->ReturnedMedicalNeedle));return RetChar;
		case 66: sprintf(RetChar,"Server Record: %s has repaired the Power Substation %s times.",High->PlayerName,JmgUtility::formatDigitGrouping(High->RepairedSubstation));return RetChar;
		case 67: sprintf(RetChar,"Server Record: %s has finished %s rounds with the substation online.",High->PlayerName,JmgUtility::formatDigitGrouping(High->SubstationOnLineAtEnd));return RetChar;
		case 68: sprintf(RetChar,"Server Record: %s has played %s rounds in which the substation was never damaged.",High->PlayerName,JmgUtility::formatDigitGrouping(High->SubstationNotDamaged));return RetChar;
		case 69: sprintf(RetChar,"Server Record: %s has killed %s Genitically Enhanced Deer.",High->PlayerName,JmgUtility::formatDigitGrouping(High->GiantDeerKilled));return RetChar;
		case 70: sprintf(RetChar,"Server Record: %s has successfully held the cabin %s times for the duration of the EAS.",High->PlayerName,JmgUtility::formatDigitGrouping(High->SurvivedAlarm));return RetChar;
		case 71: sprintf(RetChar,"Server Record: %s has killed %s Wolfs.",High->PlayerName,JmgUtility::formatDigitGrouping(High->WolfKilled));return RetChar;
		case 72: sprintf(RetChar,"Server Record: %s has killed %s Mutant Dogs.",High->PlayerName,JmgUtility::formatDigitGrouping(High->MutantDogKilled));return RetChar;
		case 73: sprintf(RetChar,"Server Record: %s has killed %s Karma Deer.",High->PlayerName,JmgUtility::formatDigitGrouping(High->BlueDeerKilled));return RetChar;
		case 74: sprintf(RetChar,"Server Record: %s has been in %s games that were won unbelievably well.",High->PlayerName,JmgUtility::formatDigitGrouping(High->CheatedRounds));return RetChar;
		case 75: sprintf(RetChar,"Server Record: %s has been in %s games without the president getting hurt by mutants.",High->PlayerName,JmgUtility::formatDigitGrouping(High->NeverInjured));return RetChar;
		case 76: sprintf(RetChar,"Server Record: %s has killed %s Moose.",High->PlayerName,JmgUtility::formatDigitGrouping(High->MooseKilled));return RetChar;
		case 77: sprintf(RetChar,"Server Record: %s has been eaten by the rabbit %s times.",High->PlayerName,JmgUtility::formatDigitGrouping(High->EatenByRabbit));return RetChar;
		case 78: sprintf(RetChar,"Server Record: %s has picked up the golden deer statue %s times.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PickedUpDeerStatue));return RetChar;
		case 79: sprintf(RetChar,"Server Record: %s has dropped up the golden deer statue %s times.",High->PlayerName,JmgUtility::formatDigitGrouping(High->DroppedDeerStatue));return RetChar;
		case 80: sprintf(RetChar,"Server Record: %s has returned up the golden deer statue %s times.",High->PlayerName,JmgUtility::formatDigitGrouping(High->ReturnedDeerStatue));return RetChar;
		case 81: sprintf(RetChar,"Server Record: %s has killed %s tiny deer.",High->PlayerName,JmgUtility::formatDigitGrouping(High->TinyDeerKilled));return RetChar;
		case 82: sprintf(RetChar,"Server Record: %s has killed %s mutant squirrels.",High->PlayerName,JmgUtility::formatDigitGrouping(High->MutantSquirrelsKilled));return RetChar;
		case 83: sprintf(RetChar,"Server Record: %s has killed %s wild deer.",High->PlayerName,JmgUtility::formatDigitGrouping(High->WildDeerKilled));return RetChar;
		case 84: sprintf(RetChar,"Server Record: %s has killed %s wild squirrels.",High->PlayerName,JmgUtility::formatDigitGrouping(High->WildSquirrelsKilled));return RetChar;
		case 85: sprintf(RetChar,"Server Record: %s has lost %s Armored Cars.",High->PlayerName,JmgUtility::formatDigitGrouping(High->ArmoredCarsLost));return RetChar;
		case 86: sprintf(RetChar,"Server Record: %s has lost %s Warrior Infantry Fighting Vehicles.",High->PlayerName,JmgUtility::formatDigitGrouping(High->WarriorsLost));return RetChar;
		case 87: sprintf(RetChar,"Server Record: %s has spent %s minutes on foot.",High->PlayerName,JmgUtility::formatDigitGrouping(High->TimeOnFoot/60.0f));return RetChar;
		case 88: sprintf(RetChar,"Server Record: %s has spent %s minutes in Punda Jazzes.",High->PlayerName,JmgUtility::formatDigitGrouping(High->TimeInAJazzs/60.0f));return RetChar;
		case 89: sprintf(RetChar,"Server Record: %s has spent %s minutes in Cleasan Primeras.",High->PlayerName,JmgUtility::formatDigitGrouping(High->TimeInACleasans/60.0f));return RetChar;
		case 90: sprintf(RetChar,"Server Record: %s has spent %s minutes in Turret Trucks.",High->PlayerName,JmgUtility::formatDigitGrouping(High->TimeInASecurityTruck/60.0f));return RetChar;
		case 91: sprintf(RetChar,"Server Record: %s has spent %s minutes in Armored Cars.",High->PlayerName,JmgUtility::formatDigitGrouping(High->TimeInArmoredCars/60.0f));return RetChar;
		case 92: sprintf(RetChar,"Server Record: %s has spent %s minutes in Urban Defense Vehicles.",High->PlayerName,JmgUtility::formatDigitGrouping(High->TimeInAUDVs/60.0f));return RetChar;
		case 93: sprintf(RetChar,"Server Record: %s has spent %s minutes in Gatling Tanks.",High->PlayerName,JmgUtility::formatDigitGrouping(High->TimeInGatlingTanks/60.0f));return RetChar;
		case 94: sprintf(RetChar,"Server Record: %s has spent %s minutes in Warrior Infantry Fighting Vehicles.",High->PlayerName,JmgUtility::formatDigitGrouping(High->TimeInIFVs/60.0f));return RetChar;
		case 95: sprintf(RetChar,"Server Record: %s has killed %s friendly tiny deer.",High->PlayerName,JmgUtility::formatDigitGrouping(High->FriendlyTinyDeerKilled));return RetChar;
		case 96: sprintf(RetChar,"Server Record: %s has received %s points of HP to infantry from Armored Supply Trucks.",High->PlayerName,JmgUtility::formatDigitGrouping(High->SupportReceivedInfantryHp));return RetChar;
		case 97: sprintf(RetChar,"Server Record: %s has received %s points of HP to vehicles from Armored Supply Trucks.",High->PlayerName,JmgUtility::formatDigitGrouping(High->SupportReceivedVehicleHp));return RetChar;
		case 98: sprintf(RetChar,"Server Record: %s has received %s points of ammo from Armored Supply Trucks.",High->PlayerName,JmgUtility::formatDigitGrouping(High->SupportReceivedAmmo));return RetChar;
		case 99: sprintf(RetChar,"Server Record: %s has given %s points of HP to infantry from Armored Supply Trucks.",High->PlayerName,JmgUtility::formatDigitGrouping(High->SupportGrantedHpInfantry));return RetChar;
		case 100: sprintf(RetChar,"Server Record: %s has given %s points of HP to vehicles from Armored Supply Trucks.",High->PlayerName,JmgUtility::formatDigitGrouping(High->SupportGrantedHpVehicle));return RetChar;
		case 101: sprintf(RetChar,"Server Record: %s has given %s points of ammo from Armored Supply Trucks.",High->PlayerName,JmgUtility::formatDigitGrouping(High->SupportGrantedAmmo));return RetChar;
		case 102: sprintf(RetChar,"Server Record: %s has given %s points of HP to infantry from Armored Supply Trucks to theirself.",High->PlayerName,JmgUtility::formatDigitGrouping(High->SupportReceivedHpInfantrySelf));return RetChar;
		case 103: sprintf(RetChar,"Server Record: %s has given %s points of HP to vehicles from Armored Supply Trucks to theirself.",High->PlayerName,JmgUtility::formatDigitGrouping(High->SupportReceivedHpVehicleSelf));return RetChar;
		case 104: sprintf(RetChar,"Server Record: %s has given %s points of ammo from Armored Supply Trucks to theirself.",High->PlayerName,JmgUtility::formatDigitGrouping(High->SupportReceivedAmmoSelf));return RetChar;
		case 105: sprintf(RetChar,"Server Record: %s has petted %s cougars.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PettedCougars));return RetChar;
		case 106: sprintf(RetChar,"Server Record: %s has killed %s wild cougars.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledCougars));return RetChar;
		case 107: sprintf(RetChar,"Server Record: %s has killed %s mutant cougars.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledMutantCougars));return RetChar;
		case 108: sprintf(RetChar,"Server Record: %s has killed %s cougars instead of petting them.",High->PlayerName,JmgUtility::formatDigitGrouping(High->FailedPettingCougars));return RetChar;
		case 109: sprintf(RetChar,"Server Record: %s has killed %s friendly cougars.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledFriendlyCougars));return RetChar;
		case 110: sprintf(RetChar,"Server Record: %s has died trying to escape the base %s times when the boss was lurking about.",High->PlayerName,JmgUtility::formatDigitGrouping(High->DiedTryingToEscapeBase));return RetChar;
		case 111: sprintf(RetChar,"Server Record: %s has rescued %s cows.",High->PlayerName,JmgUtility::formatDigitGrouping(High->RescuedCows));return RetChar;
		case 112: sprintf(RetChar,"Server Record: %s has killed %s cows.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledCows));return RetChar;
		case 113: sprintf(RetChar,"Server Record: %s has lost %s cows.",High->PlayerName,JmgUtility::formatDigitGrouping(High->LostCows));return RetChar;
		case 114: sprintf(RetChar,"Server Record: %s has completed the cow objective %s times.",High->PlayerName,JmgUtility::formatDigitGrouping(High->CompletedCowObjective));return RetChar;
		case 115: sprintf(RetChar,"Server Record: %s has killed %s mice.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledMice));return RetChar;
		case 116: sprintf(RetChar,"Server Record: %s has completed the pest objective %s times.",High->PlayerName,JmgUtility::formatDigitGrouping(High->CompledMiceObjective));return RetChar;
		case 117: sprintf(RetChar,"Server Record: %s has destroyed %s Portable Pumpjacks.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledPortablePumpJacks));return RetChar;
		case 118: sprintf(RetChar,"Server Record: %s has placed %s Portable Pumpjacks.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PortablePumpJacksPlaced));return RetChar;
		case 119: sprintf(RetChar,"Server Record: %s has lost %s Portable Pumpjacks.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PortablePumpJacksLost));return RetChar;
		case 120: sprintf(RetChar,"Server Record: %s has made $%s.00 from Pumpjacks.",High->PlayerName,JmgUtility::formatDigitGrouping(High->PumpJackMoney));return RetChar;
		case 121: sprintf(RetChar,"Server Record: %s has made $%s.00 from Portable Pumpjacks.",High->PlayerName,JmgUtility::formatDigitGrouping(High->MobilePumpJackMoney));return RetChar;
		case 122: sprintf(RetChar,"Server Record: %s has killed %s turkeys.",High->PlayerName,JmgUtility::formatDigitGrouping(High->KilledTurkey));return RetChar;
		case 123: sprintf(RetChar,"Server Record: %s has made $%s.00 from bringing turkeys back to base.",High->PlayerName,JmgUtility::formatDigitGrouping(High->ReturnedTurkey));return RetChar;
		default: sprintf(RetChar,"Server Record ERROR: Record index out of bounds!"); return RetChar;
		}
	}
	void StateHighScore()
	{
		int LastHighScoreList = 1;
		int Random = Commands->Get_Random_Int(0,BHHighScoreListCount);
		while (Random == lastDispalyedHighScore)
			Random = Commands->Get_Random_Int(0,BHHighScoreListCount);
BHStartOfHighScoreSelectProcess:
		selectRandomMatchingScore = 0;
		BHScoreNode *Current = BHScoreNodeList,*Best = NULL;
		while (Current)
		{
			Best = CompareHighScoreNodes(Best,Current,Random);
			Current = Current->next;
		}
		if (!ReturnHighScoreNodeValue(Best,Random))
		{
			if (LastHighScoreList < BHHighScoreListCount)
			{
				LastHighScoreList++;
				if (Random+1 < BHHighScoreListCount)
				{
					Random++;
					if (Random == lastDispalyedHighScore)
						Random++;
					if (Random >= BHHighScoreListCount)
						Random = 0;
				}
				else
					Random = lastDispalyedHighScore ? 0 : 1;
				goto BHStartOfHighScoreSelectProcess;
			}
			if (LastHighScoreList == BHHighScoreListCount)
				LastHighScoreList = 1;
		}
		lastDispalyedHighScore = Random;
		char DisplayMessage[128];
		sprintf(DisplayMessage,"%s",ReturnScore(Best,Random));
		for (int x = 1;x < 128;x++)
		{
			GameObject *Player = Get_GameObj(x);
			if (Player)
				JmgUtility::DisplayChatMessage(Player,6,145,148,DisplayMessage);
		}
	}
	void EndGameUpdatePlayerStats(bool won)
	{
		for (SLNode<cPlayer>* PlayerIter = Get_Player_List()->Head(); (PlayerIter != NULL); PlayerIter = PlayerIter->Next())
		{
			cPlayer *p = PlayerIter->Data();
			BHScoreNode *node = FindOrAddPlayerBHHighScoreNode(WideCharToChar(p->Get_Name()));
			if (node->startedRound)
				if (!p->Is_Active())
					node->RoundsQuit++;
				else
					node->RoundsCompleted++;
			if (won)
				node->RoundsWon++;
			else
				node->RoundsLost++;
			node->RoundsPlayed++;
			if (JMG_Bear_Hunter_Game_Control::hasGotTurrets)
				node->PlayedGamesWithDefenseTurrets++;
			if (JMG_Bear_Hunter_Game_Control::hasActivatedTower)
				node->PlayedGamesWithGuardianHelicopter++;
			if ((unsigned long)p->Get_Kills() > node->MostKillsInARound)
				node->MostKillsInARound = (unsigned long)p->Get_Kills();
			if ((unsigned long)p->Get_Deaths() > node->MostDeathsInARound)
				node->MostDeathsInARound = (unsigned long)p->Get_Deaths();
			if (node->MostBonusObjectivesCompletedInARound < node->totalObjectivesCompleted)
				node->MostBonusObjectivesCompletedInARound = node->totalObjectivesCompleted;
			if (node->PickedupTotalPowerupsInARound < node->totalPowerupsPickedup)
				node->PickedupTotalPowerupsInARound = node->totalPowerupsPickedup;
		}
	}
	void IncreasePlayerTimeScores()
	{
		JmgUtility::GenericDateTime currentTime = JmgUtility::GenericDateTime();
		for (int y = 1;y < 128;y++)
		{
			GameObject *Player = Get_GameObj(y);
			if (!Player)
				continue;
			BearHunterScoreSystem::BHScoreNode *pobj = BHPlayerHighScoreNodes[y];	
			if (!pobj)
				continue;
			Vector3 Pos = Commands->Get_Position(Player);
			float Facing = Commands->Get_Facing(Player);
			bool IsMoving = true,IsTurning = true;
			if (!JmgUtility::SimpleDistance(Pos,pobj->lastPos))
				IsMoving = false;
			pobj->lastPos = Pos;
			if (abs(Facing - pobj->lastFacing) < 1.0f)
				IsTurning = false;
			pobj->lastFacing = Facing;
			pobj->LastPlayTime = currentTime;
			if (pobj->isMoving == IsMoving && pobj->isTurning == IsTurning)
				if (pobj->idleDelay > 60)
				{
					pobj->IdleTime++;
					continue;
				}
				else
				{
					pobj->idleDelay++;
					GameObject *pVehicle = Get_Vehicle(Player);
					if (pVehicle)
					{
						switch (Commands->Get_Preset_ID(pVehicle))
						{
						case 1000001144:case 1000001158:case 1000001162:case 1000001160:
							pobj->TimeInAJazzs++;break;
						case 1000001321:case 1000001323:case 1000001327:case 1000001325:
							pobj->TimeInACleasans++;break;
						case 1000001310:
							pobj->TimeInAUDVs++;break;
						case 1000001306:
							pobj->TimeInGatlingTanks++;break;
						case 1000001174:
							pobj->TimeInASecurityTruck++;break;
						case 1000001811:
							pobj->TimeInArmoredCars++;break;
						case 1000003594:
							pobj->TimeInIFVs++;break;
						}
					}
					else
						pobj->TimeOnFoot++;
				}
			else
				pobj->idleDelay = 0;
			pobj->isMoving = IsMoving;
			pobj->isTurning = IsTurning;
			if (pobj->startedRound)
			{
				pobj->PlayTime++;
				pobj->tmpPlayTime++;
			}
			else
				pobj->PreGameTime++;
		}
	}
	void GameStarted()
	{
		BHScoreNode *Current = BHScoreNodeList;
		while (Current)
		{
			Current->startedRound = true;
			Current = Current->next;
		}
	}
};
BearHunterScoreSystem bearHunterScoreSystem;

class JMG_Rp2_Hostile_Mutant_AI : public ScriptImpClass {
	Rp2SimpleObjectList::SimpleObjectNode *myNode;
	bool damaged;
	int returnHome;
	Vector3 lastPos;
	int PlayerCount;
	int CanHearDelay;
	int LastSeen;
	int currentTargetID;
	int secondaryEnemyId;
	int lastSeenSecondary;
	int waitcount;
	Vector3 homelocation;
	float speed;
	bool huntEnemy;
	int returnHomeDelay;
	int vehicleChaseTime;
	int vehicleChaseTimeReset;
	int maxVehicleChaseTime;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void SoundDHeard(GameObject *obj,const CombatSound &sound);
public:
	JMG_Rp2_Hostile_Mutant_AI()
	{
		myNode = NULL;
	}
	static Vector3 AllowedSoundPos;
	static bool CanInvestigateSound;
};
Vector3 JMG_Rp2_Hostile_Mutant_AI::AllowedSoundPos = Vector3(0.0f,0.0f,0.0f);
bool JMG_Rp2_Hostile_Mutant_AI::CanInvestigateSound = true;

class JMG_Bear_Hunt_Mutant_Attacker : public ScriptImpClass {
	Rp2SimpleObjectList::SimpleObjectNode *myNode;
	int LastSeen;
	int secondaryEnemyId;
	int lastSeenSecondary;
	int targetedId;
	float targetDistance;
	int targetUpdate;
	bool ignoreEnemies;
	float weaponRange;
	float attackRange;
	int stuckCount;
	Vector3 lastPos;
	bool ignoreDamage;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void AttackTarget(GameObject *obj,GameObject *target,float distance);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
public:
	JMG_Bear_Hunt_Mutant_Attacker()
	{
		myNode = NULL;
	}
};

class JMG_Bear_Hunter_Animal_Control : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
	void Create_Powerup(GameObject *powerup,GameObject *obj,float random,const char *powerup1,const char *powerup2,const char *powerup3);
};

/*!
* \brief Remake of RMV_Camera_Behavior with more settings and more user control
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Security_Camera_Behavior : public ScriptImpClass {
	float scanAngle;
	float updateRateMultiplier;
	float zHeight;
	int originalTeam;
	bool enabled;
	int floodLightId;
	int EnemyID;
	int CameraFacingCount;
	int IncreaseOrDecreaseCount;
	int SeenTime;
	int stealthModeOverride;
	float cameraAngles[5];
	float CameraFacingUpdateTime;
	float idleZAimAngleModifier;
	float minDistanceSquared;
	bool canOnlySeeTargetScript;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
};

class JMG_Bear_Hunt_Mutant_Cat_Explosion : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JMG_Bear_Hunter_President_Controller : public ScriptImpClass {
	int regenTime;
	bool injuredAnimation;
	time_t lastTime;
	int lastDisplayHealth;
	int hurtRecently;
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
	void Animation_Complete(GameObject* obj,const char *anim);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *killer);
	inline void damagedEvent(GameObject *obj,bool announce,bool palyinjured);
	void Greeting(GameObject *obj);
	void InPain(GameObject *obj);
	void Critical(GameObject *obj);
	void Congrats(GameObject *obj);
	void Rabbit(GameObject *obj);
public:
	static double lastHPRecorded;
};
double JMG_Bear_Hunter_President_Controller::lastHPRecorded = 500.0;

class JMG_Bear_Hunter_Turret_Death_Alert : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JMG_Powerup_Grant_Cash : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Bear_Hunter_Powerup_Tracker : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
};

class JMG_Bear_Hunter_Grenade_Vest_Powerup : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Powerup_Grant_Weapon_Clips : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Bear_Hunter_SpawnPoint : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Bear_Hunter_Player_Spawn : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	Rp2SimplePositionSystem::SimplePositionNode *JMG_Bear_Hunter_Player_Spawn::GetSpotNotVisibileFromSpots(int points,Vector3 pos[],float ranges[]);
	Rp2SimplePositionSystem::SimplePositionNode *JMG_Bear_Hunter_Player_Spawn::GetFurthestSpotFromSpots(int points,Vector3 pos[],float weight[]);
	bool TestRay(Vector3 pos,Vector3 targetPos);
};

class JMG_Bear_Hunter_Player_Vehicle : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *killer);
public:
	int lastDamaged;
};

class JMG_Bear_Hunter_Bear_Tracker : public ScriptImpClass {
	void Destroyed(GameObject *obj);
};

class JMG_Bear_Hunter_Mutant_Tracker : public ScriptImpClass {
	void Destroyed(GameObject *obj);
};

class JMG_AI_Ignore_Object : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Bear_Hunter_Security_Turret_Truck : public ScriptImpClass {
	GameObject *driver;
	bool firstTime;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
public:
	static int id;
};
int JMG_Bear_Hunter_Security_Turret_Truck::id = 0;
class JMG_Bear_Hunter_Security_Turret_Truck_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};
class JMG_Bear_Hunt_Final_Boss : public ScriptImpClass {
	float moveSpeed;
	float rabbitSize;
	int actionUpdate;
	int enemyId;
	int jumperId;
	int targetedId;
	float targetDistance;
	int targetUpdate;
	int stuckCount;
	Vector3 lastPos;
	float lastHealth;
	float lastArmor;
	bool attackingPlayer;
	bool targetIsMine;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void AttackTarget(GameObject *obj,GameObject *target,float distance);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *killer);
	void Respawn(GameObject *obj);
	void ChooseTarget(GameObject *obj,Vector3 pos,GameObject *target);
	bool inRange(GameObject *obj);
	void SnackTime(GameObject *obj,GameObject *target);
	void MineHunt(GameObject *obj,Vector3 myPos,GameObject *enemy);
public:
	static Vector3 bossPos;
	static int bossChoose;
};
Vector3 JMG_Bear_Hunt_Final_Boss::bossPos = Vector3();
int JMG_Bear_Hunt_Final_Boss::bossChoose = 7;

class JMG_Bear_Hunter_Spectator : public ScriptImpClass {
	int playerId;
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
};

class JMG_Bear_Hunter_Dummy_Script : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Set_Player_Type_On_Create : public ScriptImpClass {
	void Created(GameObject *obj);
};

class AIPurchaseSystem
{
	struct AICharacterNode
	{
		int cost;
		char name[127];
		int requiredPurchaseState;
		double rating;
		struct AICharacterNode *next;
		AICharacterNode(const char *name,int cost,int requiredPurchaseState,double rating)
		{
			this->cost = cost;
			sprintf(this->name,"%s",name);
			this->requiredPurchaseState = requiredPurchaseState;
			this->rating = rating;
			next = NULL;
		}
	};
	AICharacterNode *AICharacterNodeList;
	int presetCount;
	void add(const char *name,int cost,int requiredPurchaseState,double rating)
	{
		AICharacterNode *current = AICharacterNodeList;
		if (!current)
			AICharacterNodeList = new AICharacterNode(name,cost,requiredPurchaseState,rating);
		while (current)
		{
			if (!current->next)
			{
				current->next = new AICharacterNode(name,cost,requiredPurchaseState,rating);
				break;
			}
			current = current->next;
		}
		presetCount++;
	};
public:
	AIPurchaseSystem()
	{
		AICharacterNodeList = NULL;
		presetCount = 0;
		add("CnC_Nod_Shotgunner",0,-1,0.1);
		add("CnC_GDI_Grenadier",0,-1,0.0);
		add("CnC_Nod_Flamethrower",0,-1,0.1);
		add("CnC_GDI_Sydney",150,1,0.2);
		add("CnC_Nod_Petrova",150,1,0.15);
		add("CnC_GDI_Officer",175,-1,0.25);
		add("CnC_Nod_Officer",175,-1,0.25);
		add("CnC_Nod_Rocket_Officer",225,-1,0.33);
		add("CnC_GDI_Rocket_Soldier_Officer",225,-1,0.33);
		add("CnC_GDI_Gunner",400,-1,0.4);
		add("CnC_Nod_Stealth_Blackhand",400,-1,0.45);
		add("CnC_Nod_Chaingunner",450,-1,0.5);
		add("CnC_GDI_Patch",450,1,0.45);
		add("CnC_Nod_Raveshaw",1000,-1,0.8);
		add("CnC_Nod_Raveshaw_Alt",1000,-1,0.7);
		add("CnC_GDI_Sydney_Ion",1000,-1,0.65);
		add("CnC_GDI_Sydney_Ion_ALT",1000,-1,0.6);
		add("CnC_GDI_Ignatio_Mobius",1000,-1,0.65);
		add("CnC_GDI_Ignatio_Mobius_ALT",1000,-1,0.6);
		add("CnC_Nod_Mendoza",1000,-1,0.8);
		add("CnC_Nod_Mendoza_ALT",1000,-1,0.7);
		add("CnC_GDI_Patch",500,-1,0.5);
		add("CnC_Nod_Blackhand_Sniper",500,-1,0.5);
		add("CnC_Nod_Sakura",1000,-1,0.65);
		add("CnC_Nod_Sakura_ALT",1000,-1,0.6);
		add("CnC_GDI_Havoc",1000,-1,0.65);
		add("CnC_GDI_Havoc_ALT",1000,-1,0.6);
	}
	void Empty_List()
	{
		presetCount = 0;
		AICharacterNode *temp = AICharacterNodeList,*die;
		while (temp)
		{
			die = temp;
			temp = temp->next;
			delete die;
		}
		AICharacterNodeList = NULL;
	}
	void PurchaseSoldier(GameObject *obj,float *cash)
	{
		AICharacterNode *current = AICharacterNodeList,*best = NULL;
		if (*cash > 1250)
			while (current)
			{
				if (current->requiredPurchaseState == -1 || current->requiredPurchaseState == JMG_Bear_Hunter_Game_Control::gameState)
					if (*cash >= current->cost && (!best || (current->rating > best->rating || (current->rating == best->rating && Commands->Get_Random(0.0f,1.0f) < 0.5f))))
						best = current;
				current = current->next;
			}
		else
		{
			int random = Commands->Get_Random_Int(0,presetCount*2);
			while (current)
			{
				if ((current->requiredPurchaseState == -1 || current->requiredPurchaseState == JMG_Bear_Hunter_Game_Control::gameState) && *cash >= current->cost)
				{
					random--;
					if (!random)
					{
						best = current;
						break;
					}
				}
				current = current->next;
			}
		}
		if (!best)
			return;
		*cash = *cash-best->cost;
		Change_Character(obj,best->name);
	}
};
AIPurchaseSystem AIPurchaseSystemControl;

class JMG_Bear_Hunter_Player_Assist_AI : public ScriptImpClass {
	Rp2SimplePositionSystem::SimplePositionNode *currentDefensePoint;
	enum AIStates{NoAction = 0,AccessPT = 4,PTReload = 5,GoHome = 3,WaitForGroup = 1,FollowPlayer = 3,MoveToCabin = 2,HoldCabin = 2,DefendGate = 2,DefendPres = 2};
	AIStates aiState,lastAiState;
	JMG_Bear_Hunter_Game_Control::GameState lastGameState;
	float lastArmor;
	float lastHealth;
	int enemyId;
	float enemyDistance;
	int lastSeen;
	int actionRefresh;
	Vector3 targetDestination;
	float requiredDestinationRange;
	float requiredDestinationRangeSquared;
	float moveSpeed;
	char primaryWeapon[256];
	int ptRefillBlock;
	bool respawning;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *killer);
	void Destroyed(GameObject *obj);
	void GotoLocation(GameObject *obj,GameObject *player);
	void SetAIState(GameObject *obj,AIStates newState,Vector3 pos,float distance,float speed,Rp2SimplePositionSystem::SimplePositionNode *newNode = NULL);
	void Respawn(GameObject *obj);
	void PurchaseCharacter(GameObject *obj);
	void ChooseAction(GameObject *obj);
public:
	#define maxTotalBearHunterPlayerAssistAI 5
	static int aiIds[maxTotalBearHunterPlayerAssistAI];
	static int maxWait;
	static float cash[maxTotalBearHunterPlayerAssistAI];
	static unsigned int aiDeaths[maxTotalBearHunterPlayerAssistAI];
};
int JMG_Bear_Hunter_Player_Assist_AI::aiIds[maxTotalBearHunterPlayerAssistAI] = {0};
int JMG_Bear_Hunter_Player_Assist_AI::maxWait = 15;
float JMG_Bear_Hunter_Player_Assist_AI::cash[maxTotalBearHunterPlayerAssistAI] = {0};
unsigned int JMG_Bear_Hunter_Player_Assist_AI::aiDeaths[maxTotalBearHunterPlayerAssistAI] = {0};

void JMG_Bear_Hunter_Game_Control::ObjectiveCompleteReward(float credits)
{
	for (int x = 0;x < maxTotalBearHunterPlayerAssistAI;x++)
		JMG_Bear_Hunter_Player_Assist_AI::cash[x] += credits;
	for (int x = 1;x < 128;x++)
	{
		GameObject *player = Get_GameObj(x);
		if (!player)
			continue;
		Commands->Give_Money(player,credits,0);
	}
}

class JMG_Bear_Hunter_AI_PT_Location : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Bear_Hunter_AI_Defense_Point : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Bear_Hunter_Warning_Light_Zone : public ScriptImpClass {
	bool lightOn;
	char ModelName[16];
	int LightEffectIDs[2];
	int warnTimeCountdown;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Entered(GameObject *obj,GameObject *enterer);
};

class JMG_Bear_Hunter_Vehicle_Purchase_System : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void AirDropFunction(GameObject *obj,Vector3 pos,float facing,int deathZoneId,int waypath1,int waypath2,int waypath3);
};

class JMG_Airdrop_Attached_Object_On_Create : public ScriptImpClass {
	char skinType[256];
	char armorType[256];
	bool entered;
	bool timeUp;
	Vector3 origPos;
	float origFacing;
	int flareId;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	GameObject *CreateAObject(GameObject *obj,GameObject *Object,const char *Preset,const char *Model,const char *Animation,const Vector3 &Pos,float Facing,GameObject *GAttachTo,const char *Bone,float DeathFrame,float LastFrame);
};

class JMG_Animate_While_Firing : public ScriptImpClass {
	unsigned int spinDown;
	bool firing;
	int bulletCount;
	char animation[32];
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Bear_Hunter_Alarm_Switch : public ScriptImpClass {
	bool waitingToDeactivate;
	int switchPosition;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Poked(GameObject *obj, GameObject *poker);
	void ActivateSwitch(GameObject *obj,int position);
	void UpdateTower(int position);
};

class JMG_Pickup_Display_Message : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
class JMG_AI_Follow_Player_When_Near : public ScriptImpClass {
	int targetId;
	int lastSeenTime;
	float targetDistance;
	float weaponRange;
	float attackUpdate;
	int playerId;
	float followDistance;
	float followMaxDistance;
	float lastMoveSpeed;
	Vector3 createLocation;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
	inline float GetSpeed(GameObject *player,float distance);
};

class JMG_Bear_Hunter_Engineer_Follow_Player_When_Near : public ScriptImpClass {
	int targetId;
	int lastSeenTime;
	float targetDistance;
	float weaponRange;
	float attackUpdate;
	float followDistance;
	float followMaxDistance;
	float lastMoveSpeed;
	Vector3 createLocation;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *killer);
	inline float GetSpeed(GameObject *player,float distance);
public:
	static int followPlayerId[5];
};
int JMG_Bear_Hunter_Engineer_Follow_Player_When_Near::followPlayerId[5] = {0};

class JMG_Bear_Hunter_Engineer_AI : public ScriptImpClass {
	Vector3 repairSoldierVehicleTurret;
	float repairGunRange;
	float weaponRange;
	bool repairGun;
	int actionUpdate;
	int targetId;
	float targetDistance;
	int targetUpdate;
	int samePosition;
	int repairTargetId;
	Vector3 lastPos;
	Vector3 moveLocation;
	int lastRepairTargetId;
	GameObject *lastTarget;
	GameObject *lastSecondaryTarget;
	bool lastRepairTarget;
	bool lastUseRepairGun;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void AttackTarget(GameObject *obj,GameObject *target,GameObject *secondaryTarget,bool repairTarget,bool useRepairGun);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *killer);
	bool inRange(GameObject *obj);
	inline bool Valid_Repair_Target(GameObject *obj,GameObject *target,int playerType);
public:
	static int scanAgainForEngineerDeaths;
	static int engineersDead;
	static int engineerIds[5];
	static bool engineerMode[5];
};
int JMG_Bear_Hunter_Engineer_AI::engineerIds[5] = {0};
int JMG_Bear_Hunter_Engineer_AI::engineersDead = 0;
int JMG_Bear_Hunter_Engineer_AI::scanAgainForEngineerDeaths = 0;
bool JMG_Bear_Hunter_Engineer_AI::engineerMode[5] = {false};

class JMG_Bear_Hunter_Oil_Rig : public ScriptImpClass {
public:
	struct PumpjackNode
	{
		int pumpId;
		bool active;
		PumpjackNode()
		{
			this->pumpId = 0;
			this->active = false;
		}
		PumpjackNode(int pumpId)
		{
			this->pumpId = pumpId;
			this->active = false;
		}
	};
private:
	float intPayout;
	bool active;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Poked(GameObject *obj, GameObject *poker);
	void Killed(GameObject *obj,GameObject *killer);
public:
	static int oilRigCount;
	static PumpjackNode pumpJacks[7]; 

};
int JMG_Bear_Hunter_Oil_Rig::oilRigCount = 0;
JMG_Bear_Hunter_Oil_Rig::PumpjackNode JMG_Bear_Hunter_Oil_Rig::pumpJacks[7] = {JMG_Bear_Hunter_Oil_Rig::PumpjackNode(0)};

class JMG_Bear_Hunter_Damaged_Oil_Rig : public ScriptImpClass {
	bool repaired;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *killer);
public:
	static int rigCount;
};
int JMG_Bear_Hunter_Damaged_Oil_Rig::rigCount = 0;

class JMG_Complete_Objective_On_Poke : public ScriptImpClass {
	void Poked(GameObject *obj, GameObject *poker);
};
class JMG_Bear_Hunter_Weapons_Container_Spawn_Point : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Bear_Hunter_Weapons_Container : public ScriptImpClass {
	Vector3 createLocation;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Bear_Hunter_Weapons_Container_Attached : public ScriptImpClass {
	bool drop;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
public:
	JMG_Bear_Hunter_Weapons_Container_Attached()
	{
		drop = true;
	}
};

class JMG_Bear_Hunter_Giant_Deer_Boss : public ScriptImpClass {
	bool damaged;
	int returnHome;
	Vector3 lastPos;
	int PlayerCount;
	int CanHearDelay;
	int LastSeen;
	int currentTargetID;
	int waitcount;
	Vector3 homelocation;
	float speed;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *killer);
	void ReturnHome(GameObject *obj);
};

class JMG_Bear_Hunter_Kill_Score_Tracker : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JMG_Bear_Hunter_Powerup_Score_Control : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Bear_Hunter_Increase_Score_On_Poke : public ScriptImpClass {
	void Poked(GameObject *obj, GameObject *poker);
};

class JMG_Bear_Hunter_Radio_Tower_Control : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
public:
	static int radioTowerId;
	static bool destroyed;
};
int JMG_Bear_Hunter_Radio_Tower_Control::radioTowerId = 0;
bool JMG_Bear_Hunter_Radio_Tower_Control::destroyed = false;

class JMG_Bear_Hunter_Radio_Tower_Switch : public ScriptImpClass {
	bool activated;
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
public:
	static int supportId;
};
int JMG_Bear_Hunter_Radio_Tower_Switch::supportId = 0;

class JMG_Bear_Hunter_Radio_Tower_Support : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JMG_Bear_Hunter_Death_Water_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JMG_Bear_Hunter_Guardian_Aircraft : public ScriptImpClass {
	enum{SECONDARY_RELOADING,SECONDARY_READY,SECONDARY_SWITCHING,SECONDARY_ACTIVE} secondaryState;
	Rp2SimplePositionSystem::SimplePositionNode *DPNode;
	char primaryWeapon[256];
	bool hasSecondaryWeapon;
	int enemyID;
	int enemyTimeOutTime;
	int newPosTime;
	int putAwayTime;
	Vector3 lastPos;
	Vector3 movePos;
	bool usePrimary;
	float altReloadTime;
	float enemyDistance;
	bool secondaryReady;
	bool reloadingSecondary;
	int firstMove;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Animation_Complete(GameObject* obj,const char *anim);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Select_New_Location(GameObject *obj);
	void Attack_Move_Update(GameObject *obj);
	void Switch_Weapon(GameObject *obj,bool primary);
public:
	JMG_Bear_Hunter_Guardian_Aircraft()
	{
		DPNode = NULL;
	}
};

class JMG_Bear_Hunter_Comanche : public ScriptImpClass {
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Destroyed(GameObject *obj);
public:
	static int id;
};
int JMG_Bear_Hunter_Comanche::id = 0;

class JMG_Bear_Hunter_Comanche_Defense_Point : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Bear_Hunter_Defense_Gun_Beacon : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Bear_Hunter_Defense_Gun_Powerup : public ScriptImpClass {
	int Count;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
};

class JMG_Bear_Hunter_Camera_Behavior : public ScriptImpClass {
	BearHunterScoreSystem::BHScoreNode *playerScoreNode;
	bool enabled;
	bool cameraActive;
	int EnemyID;
	int CameraFacingCount;
	int IncreaseOrDecreaseCount;
	int SeenTime;
	Vector3 CameraFacingLocation[5];
	float CameraFacingUpdateTime;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Poked(GameObject *obj,GameObject *poker);
	void Killed(GameObject *obj,GameObject *killer);
public:
	JMG_Bear_Hunter_Camera_Behavior()
	{
		playerScoreNode = NULL;
	}
};

class JMG_Bear_Hunter_Sentry_Turret_Placement_Tester : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason);
};

class JMG_Powerup_Prez_Medical_Needle : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Prez_Medical_Needle_Player_Control : public ScriptImpClass {
	Vector3 pickupLocation;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Destroyed(GameObject *obj);
};

class JMG_Prez_Cinematic_C130 : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Bear_Hunter_Power_Transformer : public ScriptImpClass {
	int lastHealthPercent;
	bool objectiveActive;
	time_t announceTime;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void ToggleTurrets(bool enabled);
	void UpdateHealthDisplays(float shieldStrength);
	void UpdateMCTs(int team);
public:
	static bool online;
	static int mctIds[2];
};
bool JMG_Bear_Hunter_Power_Transformer::online = true;
int JMG_Bear_Hunter_Power_Transformer::mctIds[2] = {0};

class JMG_Bear_Hunter_Power_Transformer_MCT : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class AiPlayerRespawnController
{
private:
	struct AiPlayerRespawnNode
	{
		int id;
		float spawnTime;
		bool spawning;
		AiPlayerRespawnNode *next;
	};
	AiPlayerRespawnNode *newRespawnNode(AiPlayerRespawnNode *Node,int id)
	{
		playerCount++;
		Node->id = id;
		Node->spawnTime = 0.0f;
		Node->spawning = false;
		Node->next = NULL;
		return Node;
	}
	void AddAPlayer(int id)
	{
		if (!AiPlayerRespawnNodeList)
		{
			AiPlayerRespawnNodeList = newRespawnNode(new AiPlayerRespawnNode(),id);
			return;
		}
		AiPlayerRespawnNode *Current = AiPlayerRespawnNodeList;
		while (Current)
		{
			if (Current->id == id)
				return;
			Current = Current->next;
		}
		Current = AiPlayerRespawnNodeList;
		while (Current)
		{
			if (!Current->id)
			{
				playerCount++;
				Current->id = id;
				Current->spawnTime = 0.0f;
				Current->spawning = false;
				return;
			}
			if (!Current->next)
			{
				Current->next = newRespawnNode(new AiPlayerRespawnNode(),id);
				return;
			}
			Current = Current->next;
		}
		return;
	}
	void RemoveAPlayer(int id)
	{
		if (!AiPlayerRespawnNodeList || !id)
			return;
		AiPlayerRespawnNode *Current = AiPlayerRespawnNodeList;
		while (Current)
		{
			if (Current->id == id)
			{
				Current->spawning = false;
				Current->id = 0;
				playerCount--;
				return;
			}
			Current = Current->next;
		}
	}
	AiPlayerRespawnNode *FindAPlayer(int id)
	{
		if (!AiPlayerRespawnNodeList)
			return NULL;
		AiPlayerRespawnNode *Current = AiPlayerRespawnNodeList;
		while (Current)
		{
			if (Current->id == id)
				return Current;
			Current = Current->next;
		}
		return NULL;
	}
	void UpdatePlayerSpawnList()
	{
		AiPlayerRespawnNode *Current = AiPlayerRespawnNodeList;
		while (Current)
		{
			if (Current->spawning)
			{
				Current->spawnTime -= 0.1f;
				if (Current->spawnTime <= 0.0f)
					RemoveAPlayer(Current->id);
			}
			Current = Current->next;
		}
	}
public:
	AiPlayerRespawnNode *AiPlayerRespawnNodeList;
	int playerCount;
	AiPlayerRespawnController()
	{
		AiPlayerRespawnNodeList = NULL;
		ClearList();
	}
	void ClearList()
	{
		if (!AiPlayerRespawnNodeList)
			return;
		playerCount = 0;
		AiPlayerRespawnNode *temp,*die;
		temp = AiPlayerRespawnNodeList;
		while (temp != NULL)
		{
			die = temp;
			temp = temp->next;
			delete die;
		}
		AiPlayerRespawnNodeList = NULL;
	}
	void PlayerCreate(int id)
	{
		AddAPlayer(id);
	}
	void PlayerDeath(int id,int EnemyTeamPlayerCount,float RespawnTime)
	{
		if (!AiPlayerRespawnNodeList)
			return;
		if (EnemyTeamPlayerCount >= playerCount)
		{
			AiPlayerRespawnNode *Node = FindAPlayer(id);
			if (!Node)
				return;
			Node->spawnTime = RespawnTime;
			Node->spawning = true;
		}
		else
			RemoveAPlayer(id);
	}
	void KillAll()
	{
		playerCount = 0;
		if (!AiPlayerRespawnNodeList)
			return;
		AiPlayerRespawnNode *Current = AiPlayerRespawnNodeList;
		while (Current)
		{
			GameObject *Bot = Commands->Find_Object(Current->id);
			if (Bot)
				Commands->Destroy_Object(Bot);
			Current = Current->next;
		}
	}
	void UpdateTeamAndSpawnCount(int enemyCount,GameObject *(SpawnObject)())
	{
		UpdatePlayerSpawnList();
		for (int x = playerCount;x < enemyCount;x++)
			AddAPlayer(Commands->Get_ID(SpawnObject()));
	}
	int TotalSpawnedPlayers()
	{
		int Count = 0;
		AiPlayerRespawnNode *Current = AiPlayerRespawnNodeList;
		while (Current)
		{
			GameObject *Bot = Commands->Find_Object(Current->id);
			if (Bot)
				Count++;
			Current = Current->next;
		}
		return Count;
	}
	void SendCustomToAllMembers(int message,int param,float delay,GameObject *sender = NULL)
	{
		AiPlayerRespawnNode *Current = AiPlayerRespawnNodeList;
		while (Current)
		{
			GameObject *Bot = Commands->Find_Object(Current->id);
			if (Bot)
				Commands->Send_Custom_Event(sender ? sender : Bot,Bot,message,param,delay);
			Current = Current->next;
		}
	}
};
AiPlayerRespawnController AiMutantRespawnSystem = AiPlayerRespawnController();

class JMG_Bear_Hunter_Mutant_Respawn_Tracker : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
	void Destroyed(GameObject *obj);
};
class JMG_Wandering_AI : public ScriptImpClass {
	bool damaged;
	int returnHome;
	Vector3 lastPos;
	Vector3 homelocation;
	int PlayerCount;
	int CanHearDelay;
	int LastSeen;
	int currentTargetID;
	int secondaryEnemyId;
	int lastSeenSecondary;
	int waitcount;
	float speed;
	bool huntEnemy;
	int vehicleChaseTime;
	int vehicleChaseTimeReset;
	int maxVehicleChaseTime;
	int wanderDelay;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void SoundDHeard(GameObject *obj,const CombatSound &sound);
	Vector3 GetRandomPosition();
public:
	static Vector3 AllowedSoundPos;
	static bool CanInvestigateSound;
};
Vector3 JMG_Wandering_AI::AllowedSoundPos = Vector3(0.0f,0.0f,0.0f);
bool JMG_Wandering_AI::CanInvestigateSound = true;

class JMG_Wandering_AI_Controller : public ScriptImpClass {
	void Destroyed(GameObject *obj);
	void Detach(GameObject *obj);
public:
	static Rp2SimplePositionSystem wanderPoints;
	static bool setup;
	JMG_Wandering_AI_Controller()
	{
		wanderPoints = Rp2SimplePositionSystem();
		JMG_Wandering_AI_Controller::setup = true;
	}
};
Rp2SimplePositionSystem JMG_Wandering_AI_Controller::wanderPoints;
bool JMG_Wandering_AI_Controller::setup = false;

class JMG_Wandering_AI_Wander_Point : public ScriptImpClass {
	void Created(GameObject *obj);
};
class JMG_Wandering_AI_Wander_Point_Dont_Remove : public ScriptImpClass {
	void Created(GameObject *obj);
};
class JMG_Wandering_AI_Wander_Point_Mobile : public ScriptImpClass {
	Rp2SimplePositionSystem::SimplePositionNode *node;
	int faceObjectId;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};
class JMG_Utility_Custom_Spawn_System_Controller : public ScriptImpClass {
	void Destroyed(GameObject *obj);
public:
	static Rp2SimplePositionSystem spawnPoints;
	static bool setup;
	JMG_Utility_Custom_Spawn_System_Controller()
	{
		JMG_Utility_Custom_Spawn_System_Controller::setup = true;
		spawnPoints = Rp2SimplePositionSystem();
	}
};
Rp2SimplePositionSystem JMG_Utility_Custom_Spawn_System_Controller::spawnPoints;
bool JMG_Utility_Custom_Spawn_System_Controller::setup = false;

/*!
* \brief This defines a spawn point for JMG_Utility_Custom_Spawn_System, each group id defines what group the spawn points belong to, one controller is needed for each group of spawn points (defined by group id)
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Spawn_System_Point : public ScriptImpClass {
	void Created(GameObject *obj);
};

/*!
* \brief A more complicated spawn system, this allows for defining presets you wish to use with precent chances, spawn points are defined with JMG_Utility_Custom_Spawn_System_Point
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Spawn_System : public ScriptImpClass {
	bool enabled;
	int totalChance;
	int eachChance[10];
	bool available[10];
	int avaliableToSpawn;
	int spawnLimit;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	GameObject *Create_Preset(Vector3 location);
};

class JMG_Utility_Custom_Spawn_System_Attached : public ScriptImpClass {
	void Destroyed(GameObject *obj);
};

class JMG_Bear_Hunter_Player_Count_Scaled_Object_Health : public ScriptImpClass {
	float originalMaxHealth;
	float originalPoints;
	bool damaged;
	int PlayerCount;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};
class BearHunterWolfHivemindSystem
{
public:
	int controllerId;
	struct BearHunterWolfHivemindNode
	{
		int id;
		char presetName[256];
		Vector3 territoryCenterSpot;
		float territoryRange;
		int maxPackSize;
		int secondaryTargetId;
		Vector3 targetWanderPosition;
		bool huntEnemy;
		int currentPackSize;
		int livingCount;
		int lastSeenSecondary;
		bool heardSound;
		Vector3 soundHeardPos;
		int soundId;
		int wanderTime;
		int randomWanderTime;
		int currentWanderTime;
		float respawnTime;
		float respawnTimeRandom;
		float listenerScale;
		bool hearsGlobalSounds;
		Rp2SimplePositionSystem wanderPositionNodes;
		struct BearHunterWolfHivemindNode *next;
		BearHunterWolfHivemindNode(int id,const char *presetName,int maxPackSize,Vector3 territoryCenterSpot,float territoryRange,int wanderTime,int randomWanderTime,float respawnTime,float respawnTimeRandom,float listenerScale,bool hearsGlobalSounds)
		{
			this->id = id;
			sprintf(this->presetName,"%s",presetName);
			this->territoryCenterSpot = territoryCenterSpot;
			this->territoryRange = territoryRange;
			this->maxPackSize = maxPackSize;
			this->targetWanderPosition = territoryCenterSpot;
			this->wanderTime = wanderTime;
			this->randomWanderTime = randomWanderTime;
			currentWanderTime = wanderTime + (randomWanderTime ? Commands->Get_Random_Int(-randomWanderTime,randomWanderTime) : 0);
			this->secondaryTargetId = 0;
			this->lastSeenSecondary = 0;
			this->heardSound = false;
			this->soundHeardPos = Vector3();
			this->soundId = 0;
			this->huntEnemy = false;
			this->currentPackSize = 0;
			this->livingCount = 0;
			this->respawnTime = respawnTime;
			this->respawnTimeRandom = respawnTimeRandom;
			this->listenerScale = listenerScale;
			this->hearsGlobalSounds = hearsGlobalSounds;
			this->wanderPositionNodes = Rp2SimplePositionSystem();
			this->next = NULL;
		}
	};
private:
	BearHunterWolfHivemindNode *BearHunterWolfHivemindNodeList;
public:
	BearHunterWolfHivemindSystem()
	{
		BearHunterWolfHivemindNodeList = NULL;
	}
	bool addNode(int id,const char *presetName,int maxPackSize,Vector3 territoryCenterSpot,float territoryRange,int wanderTime,int randomWanderTime,float respawnTime,float respawnTimeRandom,float listenerScale,bool hearsGlobalSounds)
	{
		BearHunterWolfHivemindNode *current = BearHunterWolfHivemindNodeList;
		if (!BearHunterWolfHivemindNodeList)
		{
			BearHunterWolfHivemindNodeList = new BearHunterWolfHivemindNode(id,presetName,maxPackSize,territoryCenterSpot,territoryRange,wanderTime,randomWanderTime,respawnTime,respawnTimeRandom,listenerScale,hearsGlobalSounds);
			return true;
		}
		while (current)
		{
			if (current->id == id)
				return false;
			if (!current->next)
			{
				current->next = new BearHunterWolfHivemindNode(id,presetName,maxPackSize,territoryCenterSpot,territoryRange,wanderTime,randomWanderTime,respawnTime,respawnTimeRandom,listenerScale,hearsGlobalSounds);
				return true;
			}
			current = current->next;
		}
		return false;
	};
	BearHunterWolfHivemindNode *find(int id)
	{
		BearHunterWolfHivemindNode *Current = BearHunterWolfHivemindNodeList;
		while (Current)
		{
			if (Current->id == id)
				return Current;
			Current = Current->next;
		}
		return BearHunterWolfHivemindNodeList;
	}
	bool update()
	{
		BearHunterWolfHivemindNode *current = BearHunterWolfHivemindNodeList;
		while (current)
		{
			if (!current->livingCount)
			{
				current->secondaryTargetId = 0;
				current->heardSound = false;
			}
			if (current->lastSeenSecondary)
			{
				current->lastSeenSecondary--;
				if (!current->lastSeenSecondary)
					current->secondaryTargetId = 0;
			}
			if (current->currentWanderTime)
			{
				current->currentWanderTime--;
				if (!current->currentWanderTime)
				{
					Vector3 pos = Vector3();
					Rp2SimplePositionSystem::SimplePositionNode *spn = current->wanderPositionNodes.GetRandom();
					current->targetWanderPosition = (spn ? spn->position : current->territoryCenterSpot);
					current->currentWanderTime = current->wanderTime + (current->randomWanderTime ? Commands->Get_Random_Int(-current->randomWanderTime,current->randomWanderTime) : 0);
				}
			}
			if (current->currentPackSize < current->maxPackSize)
			{
				Vector3 createLocation = current->targetWanderPosition;
				if (Get_Random_Pathfind_Spot(createLocation,25.0f,&createLocation))
				{
					GameObject *packMember = Commands->Create_Object(current->presetName,createLocation);
					char params[512];
					sprintf(params,"%d,%d",current->id,controllerId);
					Commands->Attach_Script(packMember,"JMG_Bear_Hunter_Wolf",params);
					Commands->Attach_Script(packMember,"JMG_Bear_Hunter_Give_AI_Cash_For_Kills","");
					MoveablePhysClass *mphys = packMember->As_PhysicalGameObj() ? packMember->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
					if (mphys && !mphys->Can_Teleport(Matrix3D(createLocation)))
					{
						mphys->Find_Teleport_Location(createLocation,2.5f,&createLocation);
						Commands->Set_Position(packMember,createLocation);
					}
				}
			}
			current = current->next;
		}
		return false;
	};
	void Empty_List()
	{
		BearHunterWolfHivemindNode *temp,*die;
		temp = BearHunterWolfHivemindNodeList;
		while (temp)
		{
			die = temp;
			temp = temp->next;
			die->wanderPositionNodes.Empty_List();
			delete die;
		}
		BearHunterWolfHivemindNodeList = NULL;
	}
};
BearHunterWolfHivemindSystem bearHunterWolfHivemindControl = BearHunterWolfHivemindSystem();
class JMG_Bear_Hunter_Wolf_Pack_Definition : public ScriptImpClass {
	void Created(GameObject *obj);
};
class JMG_Bear_Hunter_Wolf_Pack_Controller : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
};
class JMG_Bear_Hunter_Wolf : public ScriptImpClass {
	BearHunterWolfHivemindSystem::BearHunterWolfHivemindNode *controller;
	bool damaged;
	int returnHome;
	Vector3 lastPos;
	int PlayerCount;
	int CanHearDelay;
	int waitcount;
	float speed;
	int returnHomeDelay;
	int vehicleChaseTime;
	int vehicleChaseTimeReset;
	int maxVehicleChaseTime;
	int soundUpdate;
	int currentTargetId;
	int lastSeen;
	Vector3 targetPosition;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void SoundDHeard(GameObject *obj,const CombatSound &sound);
	void Destroyed(GameObject *obj);
public:
	JMG_Bear_Hunter_Wolf()
	{
		controller = NULL;
	}
};
class JMG_Bear_Hunter_Wolf_Wander_Point : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Bear_Hunter_Armored_Car : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
public:
	JMG_Bear_Hunter_Armored_Car()
	{
		ownerNode = NULL;
	}
	bool lightsOn;
	int effectRangeId;
	BearHunterScoreSystem::BHScoreNode *ownerNode;
};

class JMG_Bear_Hunter_Armored_Car_Controller : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void ArmoredCarSupport(GameObject *obj,BearHunterScoreSystem::BHScoreNode *ownerNode);
	void RegenHp(GameObject *obj,BearHunterScoreSystem::BHScoreNode *ownerNode);
	void GrantScore(const char *playerName,float score);
	void DecideScoreVehicleOrInfantry(GameObject *obj,GameObject *owner,BearHunterScoreSystem::BHScoreNode *ownerNode,float grantHp);
	void GrantVehicleRepairScore(GameObject *obj,GameObject *owner,BearHunterScoreSystem::BHScoreNode *ownerNode,float grantHp);
	void GrantInfantryRepairScore(GameObject *obj,GameObject *owner,BearHunterScoreSystem::BHScoreNode *ownerNode,float grantHp);
};

class JMG_Bear_Hunt_Final_Boss_Support : public ScriptImpClass {
	float lastHealth;
	float lastArmor;
	int jumperId;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_Bear_Hunter_Golden_Deer_Statue : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
public:
	static int playerWithTheStatue;
	static int tinyDeerIds[25];
	static int statueId;
};

class JMG_Bear_Hunter_Golden_Deer_Statue_Standin : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Bear_Hunter_Golden_Deer_Statue_Attached : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
};

class JMG_Bear_Hunter_AI_Avoid_Enemies : public ScriptImpClass {
	int closestEnemyId;
	float enemyDist;
	int seenTime;
	Vector3 movePosition;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *killer);
	void Destroyed(GameObject *obj);
	void GotoLocation(GameObject *obj,const Vector3 &pos,GameObject *Enemy,float speed);
	void setRetreatLocation(GameObject *obj,GameObject *enemy);
	void getRandomLocation(GameObject *obj);
public:
	bool mouse;
	static int wildAnimalCount;
	static int mouseAnimalCount;
};

class JMG_Security_Camera_Behavior_Ignore : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Bear_Hunter_Friendly_Cougar : public ScriptImpClass {
	int gotoId;
	float noPathfindRange;
	int LastSeen;
	int lastSeenSecondary;
	int currentTargetID;
	int secondaryTargetId;
	int huntorattack;
	int waitcount;
	Vector3 homelocation;
	float speed;
	int minVisibilityTime;
	int maxVisibilityTime;
	float maxHuntDistance;
	Vector3 lastPos;
	int stuckTime;
	bool movingSlow;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	bool chooseTarget(GameObject *obj,GameObject *damager,int *compareId,int *seenTimer);
	void FollowPlayer(GameObject *obj,bool moveSlow);
};

class JMG_Bear_Hunter_SpawnPoint_Final_Boss_Line_Of_Sight : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Bear_Hunter_SpawnPoint_Final_Boss : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Bear_Hunter_PumpJack_Beacon : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Bear_Hunter_PumpJack_Powerup : public ScriptImpClass {
	int Count;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
};

class JMG_Bear_Hunter_Portable_Pumpjack : public ScriptImpClass {
	BearHunterScoreSystem::BHScoreNode *playerScoreNode;
	float intPayout;
	float giveMoney;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *killer);
};

class JMG_Bear_Hunter_PumpJack_Detect_Oil : public ScriptImpClass {
	int playerId;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	static float FindClosestPumpjack(GameObject *obj);
public:
	static float CalculateEffectiveness(GameObject *obj);
};

class JMG_Bear_Hunter_Milk_Drink : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Bear_Hunter_AI_Guardian_Generic : public ScriptImpClass {
	float fireRange;
	float flightHeight;
	float arriveDistance;
	float arriveDistanceSq;
	Vector3 dpPosition;
	int EnemyID;
	int EnemyTimeOutTime;
	Vector3 LastPos;
	bool primaryFire;
	int stealthModeOverride;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Goto_Location(GameObject *obj);
	bool Get_A_Defense_Point(Vector3 *position);
};

class JMG_Bear_Hunter_Give_AI_Cash_For_Kills : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

/*!
* \brief Makes a wander point follow an object on the map
* \GroupId - ID of the group the point belongs to
* \PresetName - Preset to follow by object
* \WeaponName - Object with weapon to follow
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Wandering_AI_Wander_Point_Follow_Weapon_Or_Obj : public ScriptImpClass {
	Rp2SimplePositionSystem::SimplePositionNode *node;
	int followObjectId;
	char presetName[128];
	char weaponName[128];
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	GameObject *FindTargetObject(GameObject *obj);
};

class JMG_Bear_Hunter_Turkey : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

/*!
* \If the security camera script has CanOnlyTargetCameraTargets set this script is needed for a target to be visible
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Security_Camera_Behavior_Target : public ScriptImpClass {
	void Created(GameObject *obj);
};