/*	Renegade Scripts.dll
    Dragonade Node Manager Game Mode Framework
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DANODEMANAGER
#define INCLUDE_DANODEMANAGER

#include "da_event.h"
#include "da_spawnsystem.h"
#include "da_settings.h"

class DANodeManagerClass;

#define SpawnNodeType "Spawn"

class DA_API DABaseNodeClass abstract : public DAEventClass {
public:
	virtual ~DABaseNodeClass() { }
	bool operator ==(const DABaseNodeClass &That) {
		return false;
	}
	bool operator !=(const DABaseNodeClass &That) {
		return true;
	}
	inline const Vector3 &Get_Position() const {
		return Position;
	}
	inline const char *Get_Preset() const {
		return Object->Get_Definition().Get_Name();
	}
	inline const char *Get_Model() const {
		return Object->Peek_Model()->Get_Base_Model_Name();
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
	inline bool Is_Contested() const {
		return IsContested;
	}
	void Set_Contested(bool contested) {
		if (IsContested != contested) {
			IsContested = contested;
			Contested_Event();
		}
	}
	inline bool Is_Capturable() const {
		return IsCapturable;
	}
	inline void Set_Capturable(bool cap) {
		IsCapturable = cap;
	}
	inline bool Is_Capture_Once() const {
		return CaptureOnce;
	}
	inline void Set_Capture_Once(bool cap) {
		CaptureOnce = cap;
	}
	virtual const char *Get_Type() const = 0;

protected:
	virtual void Init(const INIClass *INI,const StringClass &Header);
	virtual void Think();
	void Captured(int CaptureTeam);
	void Attack_Tick(int AttackTeam);
	void Defend_Tick();
	virtual void Contested_Event() { }
	virtual void Capture_Event() { }
	void Update_Icon_And_Radar();
	bool Is_Player_In_Range(SoldierGameObj *Player);
	void Give_Attack_Defend_Tick_Points(int team);
	void Give_Capture_Points();
	
	DABaseNodeClass() { }
	int Team;
	PhysicalGameObj *Object;
	PhysicalGameObj *Icon;
	PhysicalGameObj *Radar[2];
	bool SentAttackMessage;
	bool SentDefendMessage[2];
	unsigned long LastAttackMessage;
	unsigned long LastAttackTick;
	unsigned long LastDefendTick;
	int PlayerCheck[2];
	int LastAttackTeam;
	bool IsContested;

	StringClass Preset;
	StringClass Model;
	StringClass Animation;
	Vector3 Position;
	StringClass Group;
	StringClass Name;
	float AttackDefendDistance;
	bool IgnoreLOS;
	bool IsCapturable;
	bool CaptureOnce;

	friend class DANodeManagerClass;
};

class DA_API DASpawnNodeClass : public DABaseNodeClass {
public:
	virtual const char *Get_Type() const {
		return SpawnNodeType;
	}
protected:
	virtual void Capture_Event();
	virtual void Contested_Event();
	virtual void Init(const INIClass *INI,const StringClass &Header);
	DASpawnPointClass *SpawnPoint;
};

class DA_API DANodeManagerClass : public DAEventClass, public DASingleton<DANodeManagerClass> {
public:
	void Init(const INIClass *INI);
	template<typename N> DABaseNodeClass *Create_Node(const INIClass *INI,const StringClass &Header) {
		DABaseNodeClass *NewNode = new N;
		NewNode->Init(INI,Header);
		Nodes.Add(NewNode);
		return NewNode;
	}
	
	inline float Get_Attack_Defend_Distance() const {
		return AttackDefendDistance;
	}
	inline float Get_Attack_Defend_Tick_Amount() const {
		return AttackDefendTickAmount;
	}
	inline unsigned int Get_Attack_Defend_Tick_Time() const {
		return AttackDefendTickTime;
	}
	inline float Get_Attack_Defend_Tick_Points() const {
		return AttackDefendTickPoints;
	}
	inline int Get_Attack_Defend_Tick_Veteran_Points() const {
		return AttackDefendTickVeteranPoints;
	}
	inline float Get_Capture_Points() const {
		return CapturePoints;
	}
	inline int Get_Capture_Veteran_Points() const {
		return CaptureVeteranPoints;
	}
	inline unsigned int Get_Contested_Spawn_Time() const {
		return ContestedSpawnTime;
	}
	inline int Get_Node_Count() {
		return Nodes.Count();
	}
	inline DABaseNodeClass *Get_Node(int Index) {
		return Nodes[Index];
	}
	inline DABaseNodeClass *Find_Node(const StringClass &Group) {
		for (int i = 0;i < Nodes.Count();i++) {
			if (Nodes[i]->Get_Group() == Group) {
				return Nodes[i];
			}
		}
		return 0;
	}
	
private:
	
	virtual ~DANodeManagerClass() {
		for (int i = 0;i < Nodes.Count();i++) {
			delete Nodes[i];
		}
		Nodes.Delete_All();
	}
	DANodeManagerClass() { }
	DANodeManagerClass(const DANodeManagerClass&);
	DANodeManagerClass &operator=(const DANodeManagerClass&);

	DynamicVectorClass<DABaseNodeClass*> Nodes;

	//Settings
	float AttackDefendDistance;
	unsigned int AttackDefendTickTime;
	float AttackDefendTickAmount;
	float AttackDefendTickPoints;
	int AttackDefendTickVeteranPoints;
	float CapturePoints;
	int CaptureVeteranPoints;
	unsigned int ContestedSpawnTime;
	
	friend class DABaseNodeClass;
	friend class DASingleton<DANodeManagerClass>;
};

#define DANodeManager DANodeManagerClass::Get_Instance()

class NodesChatCommand;

#endif