/*	Renegade Scripts.dll
	Copyright 2017 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#ifndef TT_INCLUDE__GAMEOBJMANAGER_H
#define TT_INCLUDE__GAMEOBJMANAGER_H



#include "SList.h"


class ChunkSaveClass;
class ChunkLoadClass;
class PhysicalGameObj;
class SmartGameObj;
class BaseGameObj;
class SoldierGameObj;
class ScriptableGameObj;
class VehicleGameObj;
class BuildingGameObj;
class C4GameObj;
class BeaconGameObj;
class ScriptZoneGameObj;

class GameObjManager
{

public:

	static SCRIPTS_API REF_DECL(SList<BaseGameObj>, GameObjList);
	static SCRIPTS_API REF_DECL(SList<BuildingGameObj>, BuildingGameObjList);
	static SCRIPTS_API REF_DECL(SList<SoldierGameObj>, StarGameObjList);
	static SCRIPTS_API REF_DECL(SList<SmartGameObj>, SmartGameObjList);
	static SHARED_API SList<C4GameObj> C4GameObjList;
	static SHARED_API SList<BeaconGameObj> BeaconGameObjList;
	static SHARED_API SList<SoldierGameObj> SoldierGameObjList;
	static SHARED_API SList<VehicleGameObj> VehicleGameObjList;
	static SHARED_API SList<ScriptZoneGameObj> ScriptZoneGameObjList;
#ifndef TTLE_EXPORTS
	static REF_DECL(bool, CinematicFreezeActive);
#endif

public:

	static void Generate_Control();
	static void Think();
	static void Post_Think();
	static PhysicalGameObj* Find_PhysicalGameObj(int);
	SCRIPTS_API static SmartGameObj* Find_SmartGameObj(int);
	static void Init();
	static void Shutdown();
	static bool Save(ChunkSaveClass&);
	static bool Load(ChunkLoadClass&);
	static void Add(BaseGameObj *obj)
	{
		GameObjList.Add_Head(obj);
	}
	static void Remove(BaseGameObj *obj)
	{
		GameObjList.Remove(obj);
	}
	static void Init_All();
	static void Destroy_All();
	static SoldierGameObj* Find_Soldier_Of_Client_ID(int);
	static SoldierGameObj* Find_Different_Player_Soldier(int);
	static SoldierGameObj* Find_Soldier_Of_Player_Type(int);
	static ScriptableGameObj* Find_ScriptableGameObj(int);
	static VehicleGameObj* Find_Vehicle_Occupied_By(SoldierGameObj*);
	static void Init_Buildings();
	static void Update_Building_Collection_Spheres();
	static void Debug_Set_All_Building_States(float, bool);
	static bool Is_In_Environment_Zone(Vector3&);
	static void Add_Building(BuildingGameObj *obj) {BuildingGameObjList.Add_Tail(obj);}
	static void Remove_Building(BuildingGameObj *obj) {BuildingGameObjList.Remove(obj);}

};



#endif
