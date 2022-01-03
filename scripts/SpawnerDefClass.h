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
#ifndef TT_INCLUDE__SPAWNERDEFCLASS_H
#define TT_INCLUDE__SPAWNERDEFCLASS_H

#include "engine_vector.h"
#include "Definition.h"
#include "engine_string.h"
#include "SpawnerClass.h"

class SpawnerDefClass : public DefinitionClass
{
public:
   SpawnerDefClass();
   virtual ~SpawnerDefClass();
   virtual const PersistFactoryClass&       Get_Factory  () const;
   virtual bool                       Save         (ChunkSaveClass& chunkSaver);
   virtual bool                       Load         (ChunkLoadClass& chunkLoader);
   virtual uint32                     Get_Class_ID () const;
   virtual PersistClass*              Create       () const;
   sint32                            Get_Player_Type              () const { return PlayerType; }
   const DynamicVectorClass<int>& Get_Spawn_Definition_ID_List () const { return SpawnDefinitionIDList; }
   bool                              Is_Multiplay_Weapon_Spawner  () const { return IsMultiplayWeaponSpawner; }
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE(SpawnerDefClass,DefinitionClass);
   DynamicVectorClass<int>      SpawnDefinitionIDList;          // 28
   sint32                          PlayerType;                 // 52
   sint32                          SpawnMax;        // 56
   float                           SpawnDelay;           // 60
   float                           SpawnDelayVariation;  // 64
   bool                            IsPrimary;              // 68
   bool                            IsSoldierStartup;   // 69
   bool                            GotoSpawnerPos;            // 70
   float                           GotoSpawnerPosPriority;    // 72
   bool                            TeleportFirstSpawn;   // 76
   sint32                          SpecialEffectsObjID;        // 80
   float                           PostVisualSpawnDelay;      // 84
   bool                            StartsDisabled;      // 88
   bool                            KillHibernatingSpawn; // 89
   bool                            ApplySpawnMaterialEffect;     // 90
   bool                            IsMultiplayWeaponSpawner;    // 91
public:
   DynamicVectorClass<StringClass> ScriptNames;          // 92
   DynamicVectorClass<StringClass> ScriptParameters;     // 116
   friend class PresetDump;
   friend class SpawnManager;
}; // size: 140


#endif
