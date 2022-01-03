/*	Renegade Scripts.dll
    Dragonade Singleplayer Vehicle Spawner Game Mode Framework
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAVEHICLEPURCHASE
#define INCLUDE_DAVEHICLEPURCHASE

#include "da_event.h"
#include "da_gameobj.h"

class DA_API DAVehicleSpawnPointClass {
public:
	void Spawn_Vehicle(const char *Preset,SoldierGameObj *Owner);
	void Spawn_Vehicle(unsigned int DefID,SoldierGameObj *Owner);
	virtual void Spawn_Vehicle(const VehicleGameObjDef *Def,SoldierGameObj *Owner);
	inline const StringClass &Get_Group() const {
		return Group;
	}
	inline const StringClass &Get_Name() const {
		return Name;
	}
	inline const Vector3 &Get_Position() const {
		return Position;
	}
	inline bool Is_Enabled() const {
		return Enable;
	}
	void Set_Enabled(bool enable);
	inline int Get_Team() const {
		return Team;
	}
	void Set_Team(int team);
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
	inline float Get_Distance(const Vector3 &Pos) {
		float Dis = Commands->Get_Distance(Position,Pos);
		return Dis;
	}

protected:
	virtual void Init(const INIClass *INI,const StringClass &Header);
	virtual ~DAVehicleSpawnPointClass() { }
	DAVehicleSpawnPointClass() { }
	DAVehicleSpawnPointClass(const DAVehicleSpawnPointClass&);
	DAVehicleSpawnPointClass &operator=(const DAVehicleSpawnPointClass&);
	StringClass Group;
	StringClass Name;
	Vector3 Position;
	bool Enable;
	int Team;
	int Priority;
	float Facing;
	
	friend class DAVehicleSpawnManagerClass;
};

class DA_API DAVehicleAirDropPointClass : public DAVehicleSpawnPointClass {
public:
	virtual void Spawn_Vehicle(const VehicleGameObjDef *Def,SoldierGameObj *Owner);
	
protected:
	DAVehicleAirDropPointClass() { }
	DAVehicleAirDropPointClass(const DAVehicleAirDropPointClass&);
	DAVehicleAirDropPointClass &operator=(const DAVehicleAirDropPointClass&);

	friend class DAVehicleSpawnManagerClass;
};

class DA_API DAVehicleSpawnManagerClass : public DAEventClass, public DASingleton<DAVehicleSpawnManagerClass> {
public:
	void Init(const INIClass *INI);
	void Spawn_Vehicle(int Team,const char *Preset,SoldierGameObj *Owner);
	void Spawn_Vehicle(int Team,unsigned int DefID,SoldierGameObj *Owner);
	void Spawn_Vehicle(int Team,const VehicleGameObjDef *Def,SoldierGameObj *Owner);
	inline DAVehicleSpawnPointClass *Create_Spawn_Point(const INIClass *INI,const StringClass &Header) {
		DAVehicleSpawnPointClass *NewPoint = new DAVehicleSpawnPointClass;
		NewPoint->Init(INI,Header);
		SpawnPoints.Add(NewPoint);
		return NewPoint;
	}

	inline DAVehicleAirDropPointClass *Create_Air_Drop_Point(const INIClass *INI,const StringClass &Header) {
		DAVehicleAirDropPointClass *NewPoint = new DAVehicleAirDropPointClass;
		NewPoint->Init(INI,Header);
		SpawnPoints.Add(NewPoint);
		return NewPoint;
	}

	inline DAVehicleSpawnPointClass *Find_Spawn_Point(const StringClass &Group) {
		for (int i = 0;i < SpawnPoints.Count();i++) {
			if (SpawnPoints[i]->Get_Group() == Group) {
				return SpawnPoints[i];
			}
		}
		return 0;
	}
	inline void Enable_All_Spawn_Points_Except(const StringClass &Group) {
		for (int i = 0;i < SpawnPoints.Count();i++) {
			if (SpawnPoints[i]->Get_Group() == Group) {
				SpawnPoints[i]->Set_Enabled(false);
			}
			else {
				SpawnPoints[i]->Set_Enabled(true);
			}
		}
	}
	inline void Disable_All_Spawn_Points_Except(const StringClass &Group) {
		for (int i = 0;i < SpawnPoints.Count();i++) {
			if (SpawnPoints[i]->Get_Group() == Group) {
				SpawnPoints[i]->Set_Enabled(true);
			}
			else {
				SpawnPoints[i]->Set_Enabled(false);
			}
		}
	}
	inline void Disable_All_Spawn_Points_Team(int Team) {
		for (int i = 0;i < SpawnPoints.Count();i++) {
			if (SpawnPoints[i]->Get_Team() == Team) {
				SpawnPoints[i]->Set_Enabled(false);
			}
		}
	}
	inline void Enable_All_Spawn_Points_Team(int Team) {
		for (int i = 0;i < SpawnPoints.Count();i++) {
			if (SpawnPoints[i]->Get_Team() == Team) {
				SpawnPoints[i]->Set_Enabled(true);
			}
		}
	}
	inline void Set_All_Spawn_Points_Team(int Team) {
		for (int i = 0;i < SpawnPoints.Count();i++) {
			SpawnPoints[i]->Set_Team(Team);
		}
	}
	inline bool All_Spawn_Points_Disabled_Team(int Team) {
		for (int i = 0;i < SpawnPoints.Count();i++) {
			if (SpawnPoints[i]->Get_Team() == Team && SpawnPoints[i]->Is_Enabled()) {
				return false;
			}
		}
		return true;
	}
	inline void Enable_Vehicle_Factory(int Team,bool Enable) {
		VF[Team]->Set_Is_Destroyed(!Enable);
	}
	inline void Set_Vehicle_Factory_Busy(int Team,bool Busy) {
		VF[Team]->Set_Busy(Busy);
	}

protected:
	virtual bool Request_Vehicle_Event(VehicleFactoryGameObj *Factory,const VehicleGameObjDef *Vehicle,cPlayer *Player,float Delay,SoldierGameObj *Owner);
	virtual void Timer_Expired(int Number,unsigned int Data);

	virtual ~DAVehicleSpawnManagerClass() {
		for (int i = 0;i < SpawnPoints.Count();i++) {
			delete SpawnPoints[i];
		}
		SpawnPoints.Delete_All();
	}
	DAVehicleSpawnManagerClass() { }
	DAVehicleSpawnManagerClass(const DAVehicleSpawnManagerClass&);
	DAVehicleSpawnManagerClass &operator=(const DAVehicleSpawnManagerClass&);
	DynamicVectorClass<DAVehicleSpawnPointClass*> SpawnPoints;
	VehicleFactoryGameObj *VF[2];
	
	friend class DASingleton<DAVehicleSpawnManagerClass>;
};

#define DAVehicleSpawnManager DAVehicleSpawnManagerClass::Get_Instance()


#endif