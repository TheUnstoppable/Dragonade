/*	Renegade Scripts.dll
    Dragonade Teleporter Game Mode Framework
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DATELEPORTER
#define INCLUDE_DATELEPORTER

#include "da_event.h"
#include "da_gameobj.h"

class DA_API DATeleporterClass : public DAGameObjObserverClass {
public:
	void Teleport_Player(GameObject *obj) const;
	void Add_Ignore_List(GameObject *obj) {
		int ID = Commands->Get_ID(obj);
		if (IgnoreList.ID(ID) == -1) {
			IgnoreList.Add(ID);
		}
	}
	void Remove_Ignore_List(GameObject *obj) {
		IgnoreList.DeleteObj(Commands->Get_ID(obj));
	}
	bool Is_In_Ignore_List(GameObject *obj) {
		return !(IgnoreList.ID(Commands->Get_ID(obj)) == -1);
	}
	inline const DynamicVectorClass<Vector3> &Get_Spawners() const {
		return Spawners;
	}
	inline void Set_Enabled(bool enable) {
		Enable = enable;
		if (Enable) {
			Commands->Set_Model(Display,Model);
		}
		else {
			Commands->Set_Model(Display,"null");
		}
	}
	inline bool Is_Enabled() const {
		return Enable;
	}
	inline void Set_Team(int team) {
		Team = team;
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
	virtual const char *Get_Name() {
		return "DATeleporterClass";
	}
private:
	void Init(const INIClass *INI,const StringClass &Header);
	virtual void Entered(GameObject *obj,GameObject *enterer);
	virtual void Exited(GameObject *obj,GameObject *exiter);

	DATeleporterClass() { }
	DATeleporterClass(const DATeleporterClass&);
	DATeleporterClass &operator=(const DATeleporterClass&);
	virtual ~DATeleporterClass() { }

	ReferencerClass Display;

	StringClass Group;
	StringClass Name;
	StringClass Link;
	StringClass Model;
	bool Enable;
	int Team;
	DynamicVectorClass<Vector3> Spawners;
	DynamicVectorClass<int> IgnoreList;

	friend class DATeleporterManagerClass;
};

class DA_API DATeleporterManagerClass : public DASingleton<DATeleporterManagerClass> {
public:
	DATeleporterClass *Create_Teleporter(const INIClass *INI,const StringClass &Header) {
		DATeleporterClass *NewTeleporter = new DATeleporterClass;
		NewTeleporter->Init(INI,Header);
		Teleporters.Add(NewTeleporter);
		return NewTeleporter;
	}
	DATeleporterClass *Find_Teleporter(const StringClass &Group) {
		for (int i = 0;i < Teleporters.Count();i++) {
			if (Teleporters[i]->Get_Group() == Group) {
				return Teleporters[i];
			}
		}
		return 0;
	}
	
private:
	DATeleporterManagerClass() { }
	DATeleporterManagerClass(const DATeleporterManagerClass&);
	DATeleporterManagerClass &operator=(const DATeleporterManagerClass&);
	DynamicVectorClass<DATeleporterClass*> Teleporters;

	friend class DASingleton<DATeleporterManagerClass>;
};

#define DATeleporterManager DATeleporterManagerClass::Get_Instance()


#endif