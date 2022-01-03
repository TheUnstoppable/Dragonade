/*	Renegade Scripts.dll
    Dragonade Crate Manager
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DACRATEMANAGER
#define INCLUDE_DACRATEMANAGER

#include "da_event.h"
#include "da_gamefeature.h"
#include "da_gameobj.h"

class DACrateClass;
class DACrateFactoryClass;
class DACrateModifierClass;
class DACrateModifierFactoryClass;

class DA_API DACrateManager : public DAEventClass, public DAGameFeatureClass {
public:
	static void Static_Init();
	static void Add_Crate(DACrateFactoryClass *Factory);
	static void Add_Crate_Modifier(DACrateModifierFactoryClass *Factory);
	static DACrateClass *Get_Crate(const char *Name);
	static DACrateModifierClass *Create_Crate_Modifier(const char *Name,const char *Parameters);
	static void Calculate_Odds(cPlayer *Player);
	static float Get_Odds();
	static DACrateClass *Select_Crate(cPlayer *Player);

	virtual void Init();
	virtual ~DACrateManager();
	virtual void Settings_Loaded_Event();
	virtual void Game_Over_Event();
	Vector3 *Select_Spawner();
	virtual void Timer_Expired(int Number,unsigned int Data);
	virtual void PowerUp_Grant_Event(cPlayer *Player,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj);
	virtual void Object_Destroyed_Event(GameObject *obj);
	bool Crate_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	bool Crates_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	bool ShowCrateSpawners_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	bool HideCrateSpawners_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);

protected:
	static DynamicVectorClass <DACrateFactoryClass*> Crates;
	static DynamicVectorClass <DACrateModifierFactoryClass*> CrateModifiers;
	static float Odds;

	DynamicVectorClass<PhysicalGameObj*> CrateObjs;
	Vector3 LastCratePosition;

	//Settings
	int MaxCrates;
	StringClass Model;
	float SpawnTimeMin;
	float SpawnTimeMax;
	float FirstSpawnTimeMin;
	float FirstSpawnTimeMax;
	DynamicVectorClass<Vector3> Spawners;
	DynamicVectorClass<Vector3*> ActiveSpawners;
};

#define Iterate_Crates(Variable,Iterator) for (int Iterator = 0;Iterator < Crates.Count();Iterator++) { if (!Crates[Iterator]->Get_Instance()) { continue; } DACrateClass *Variable = Crates[Iterator]->Get_Instance(); Variable;
#define Reverse_Iterate_Crates(Variable,Iterator) for (int Iterator = Crates.Count()-1;Iterator >= 0;Iterator--) { if (!Crates[Iterator]->Get_Instance()) { continue; } DACrateClass *Variable = Crates[Iterator]->Get_Instance(); Variable;

#endif