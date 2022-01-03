/*	Renegade Scripts.dll
    Dragonade Spawn System Game Mode Framework
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DASPAWNSYSTEM
#define INCLUDE_DASPAWNSYSTEM

#include "da_event.h"
#include "da_player.h"
#include "da_settings.h"
#include "da_chatcommand.h"

namespace DASpawnState {
	enum State {
		DEAD,
		WAITINGROOM,
		SPAWNING,
		SPAWNED
	};
};

class DA_API DASpawnPlayerDataClass : public DAPlayerDataClass {
	unsigned int Check_Spawn_Time(unsigned int SpawnTime) const;
	virtual void Init() {
		SpawnState = DASpawnState::DEAD;
		DeathTime = 0;
		DeathPosition = Vector3(0,0,0);
		NextSpawnLocation = "";
	}
	DASpawnState::State SpawnState;
	unsigned long DeathTime;
	Vector3 DeathPosition;
	StringClass NextSpawnLocation;
	friend class DASpawnPointClass;
	friend class DASpawnManagerClass;
};

class DA_API DASpawnPointClass : public DAEventClass {
public:
	void Clear_Spawn_List_Find_New_Spawn();
	void Clear_Spawn_List();
	float Get_Distance(const Vector3 &Position) const;
	inline void Set_Center_Point(const Vector3 &Pos) {
		CenterPoint = Pos;
	}
	inline const Vector3 &Get_Center_Point() const {
		return CenterPoint;
	}
	inline const DynamicVectorClass<Vector3> &Get_Spawners() const {
		return Spawners;
	}
	inline void Set_Enabled(bool enable) {
		Enable = enable;
		if (!Enable) {
			Clear_Spawn_List_Find_New_Spawn();
		}
	}
	inline bool Is_Enabled() const {
		return Enable;
	}
	inline void Set_Team(int team) {
		Team = team;
		Clear_Spawn_List_Find_New_Spawn();
	}
	inline int Get_Team() const {
		return Team;
	}
	inline const StringClass &Get_Group() const {
		return Group;
	}
	inline const StringClass &Get_Name() const {
		return Name;
	}
	inline void Set_Spawn_Time(unsigned int time) {
		SpawnTime = time;
		Check_Spawn_List();
	}
	inline unsigned int Get_Spawn_Time() const {
		return SpawnTime;
	}
	inline unsigned int Get_Base_Spawn_Time() const {
		return BaseSpawnTime;
	}
	inline float Get_Protect_Time() const {
		return ProtectTime;
	}
	inline void Set_Protect_Time(float time) {
		ProtectTime = time;
	}
	inline int Get_Priority() const {
		return Priority;
	}
	inline void Set_Priority(int priority) {
		Priority = priority;
	}
	inline void Inc_Priority() {
		Priority++;
	}
	inline void Dec_Priority() {
		Priority--;
	}
	struct DistanceCheckStruct {
		DistanceCheckStruct() {
			Both = FLT_MAX;
			Ally = FLT_MAX;
			Enemy = FLT_MAX;
		}
		float Both;
		float Ally;
		float Enemy;
	};
	void Distance_Check(DistanceCheckStruct &Buffer,const Vector3 &Pos,int Team) const;
	
private:
	void Init(const INIClass *INI,const StringClass &Header);
	virtual void Think();
	bool Spawn_Player(GameObject *obj) const;
	void Add_Spawn_List(int ID);
	void Add_Spawn_List(GameObject *obj);
	void Check_Spawn_List();
	inline bool Is_In_Spawn_List(int ID) {
		return (SpawnList.ID(ID) != -1);
	}
	inline bool Is_In_Spawn_List(GameObject *obj) {
		return Is_In_Spawn_List(Get_Player_ID(obj));
	}
	inline bool Remove_Spawn_List(int ID) {
		return SpawnList.DeleteObj(ID);
	}
	inline bool Remove_Spawn_List(GameObject *obj) {
		return SpawnList.DeleteObj(Get_Player_ID(obj));
	}
	
	DASpawnPointClass() { }
	StringClass Group;
	StringClass Name;
	bool Enable;
	int Team;
	unsigned int SpawnTime;
	unsigned int BaseSpawnTime;
	float ProtectTime;
	int Priority;
	float Facing;
	Vector3 CenterPoint; //The center point is the position that the spawn group is centered around or linked to. A node, for example. This ensures that players will spawn at the right spawn group even if the spawn positions are away from the center point.
	DynamicVectorClass<Vector3> Spawners;
	DynamicVectorClass<int> SpawnList;
	friend class DASpawnManagerClass;
};

class DA_API DASpawnManagerClass : public DAEventClass, public DAPlayerDataManagerClass<DASpawnPlayerDataClass>, public DASingleton<DASpawnManagerClass> {
public:
	void Init(const INIClass *INI);
	void Re_Add_Spawn_List(int ID);
	void Re_Add_Spawn_List(GameObject *obj);
	void Set_Next_Spawn_Location(GameObject *obj,const StringClass &Name);
	void Set_Next_Spawn_Location(int ID,const StringClass &Name);
	void Display_PT(GameObject *obj);
	inline DASpawnPointClass *Create_Spawn_Point(const INIClass *INI,const StringClass &Header) {
		DASpawnPointClass *SpawnPoint = new DASpawnPointClass;
		SpawnPoint->Init(INI,Header);
		SpawnPoints.Add(SpawnPoint);
		return SpawnPoint;
	}
	inline DASpawnPointClass *Find_Spawn_Point(const StringClass &Group) {
		for (int i = 0;i < SpawnPoints.Count();i++) {
			if (SpawnPoints[i]->Get_Group() == Group) {
				return SpawnPoints[i];
			}
		}
		return 0;
	}
	inline void Enable_All_Except(const StringClass &Group) {
		for (int i = 0;i < SpawnPoints.Count();i++) {
			if (SpawnPoints[i]->Get_Group() == Group) {
				SpawnPoints[i]->Set_Enabled(false);
			}
			else {
				SpawnPoints[i]->Set_Enabled(true);
			}
		}
	}
	inline void Disable_All_Except(const StringClass &Group) {
		for (int i = 0;i < SpawnPoints.Count();i++) {
			if (SpawnPoints[i]->Get_Group() == Group) {
				SpawnPoints[i]->Set_Enabled(true);
			}
			else {
				SpawnPoints[i]->Set_Enabled(false);
			}
		}
	}
	inline unsigned int Get_Spawn_Time() const {
		return SpawnTime;
	}
	inline unsigned int Get_Intermission_Time() const {
		return IntermissionTime;
	}
	inline float Get_Min_Ally_Distance() const {
		return MinAllyDistance;
	}
	inline float Get_Max_Ally_Distance() const {
		return MaxAllyDistance;
	}
	inline float Get_Min_Enemy_Distance() const {
		return MinEnemyDistance;
	}
	inline float Get_Max_Enemy_Distance() const {
		return MaxEnemyDistance;
	}
	inline bool Using_Waiting_Room() const {
		return UseWaitingRoom;
	}
	inline float Get_Protect_Time() const {
		return ProtectTime;
	}
	inline const Vector3 &Get_Death_Position(int ID) const {
		return Get_Player_Data(ID)->DeathPosition;
	}
	inline const Vector3 &Get_Death_Position(GameObject *obj) const {
		return Get_Player_Data(obj)->DeathPosition;
	}
	
private:
	virtual void Vehicle_Purchase_Event(cPlayer *Player,float Cost,const VehicleGameObjDef *Item);
	virtual void PowerUp_Purchase_Event(cPlayer *Player,float Cost,const PowerUpGameObjDef *Item);
	virtual bool Suicide_Event(cPlayer *Player);
	virtual bool Chat_Command_Event(cPlayer *Player,TextMessageEnum Type,const StringClass &Command,const DATokenClass &Text,int ReceiverID);
	virtual void Timer_Expired(int Number,unsigned int Data);
	virtual void Object_Created_Event(GameObject *obj);
	virtual void Object_Destroyed_Event(GameObject *obj);
	void Send_To_Waiting_Room(GameObject *obj);
	void Add_Spawn_List(GameObject *obj);
	void Add_Spawn_List(int ID);
	bool Remove_Spawn_List(int ID);
	bool Remove_Spawn_List(GameObject *obj);

	virtual ~DASpawnManagerClass() {
		for (int i = 0;i < SpawnPoints.Count();i++) {
			delete SpawnPoints[i];
		}
		SpawnPoints.Delete_All();
	}

	DASpawnManagerClass() { }
	DASpawnManagerClass(const DASpawnManagerClass&);
	DASpawnManagerClass &operator=(const DASpawnManagerClass&);
	Vector3 WaitingRoomPosition[2];
	DynamicVectorClass<DASpawnPointClass*> SpawnPoints;
	DynamicVectorClass<int> SpawnList;
	bool UseWaitingRoom;

	//Settings
	unsigned int SpawnTime;
	unsigned int IntermissionTime;
	float MinAllyDistance;
	float MaxAllyDistance;
	float MinEnemyDistance;
	float MaxEnemyDistance;
	float ProtectTime;
	
	friend class DASpawnPointClass;
	friend class DASingleton<DASpawnManagerClass>;
};

#define DASpawnManager DASpawnManagerClass::Get_Instance()

#endif
