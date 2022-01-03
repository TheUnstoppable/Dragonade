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
#ifndef TT_INCLUDE__SPAWNERCLASS_H
#define TT_INCLUDE__SPAWNERCLASS_H

#include "engine_vector.h"
#include "Matrix3D.h"
#include "engine_string.h"
#include "ReferencerClass.h"
#include "Persist.h"

class SpawnerDefClass;
class PhysicalGameObj;


class SpawnerClass :
	public PersistClass
{

public:

	sint32                          id;                   // 8
	Matrix3D                        transform;            // 12
	Matrix3D                        spawnEffectTransform; // 60
	const SpawnerDefClass*          definition;           // 108
	ReferencerClass                 currentObject;        // 112
	bool                            enabled;              // 128
	sint32                          spawnCount;           // 132
	float                           spawnTimeRemaining;   // 136
	DynamicVectorClass<Matrix3D>    alternateTransforms;  // 140
	DynamicVectorClass<StringClass> scriptNames;          // 164
	DynamicVectorClass<StringClass> scriptParameters;     // 188

	SpawnerClass();
	virtual ~SpawnerClass();

	virtual const PersistFactoryClass& Get_Factory() const;
	virtual bool Save(ChunkSaveClass& chunkSaver);
	virtual bool Load(ChunkLoadClass& chunkLoader);

	void Add_Script(const char* name, const char* parameters);
	bool Can_Spawn_Object(sint32 id) const;
	void Check_Auto_Spawn(float);
	PhysicalGameObj* Create_Spawned_Object(sint32 id);
	DynamicVectorClass<Matrix3D> &Get_Spawn_Point_List() {return alternateTransforms;}
	bool Determine_Spawn_TM(PhysicalGameObj* object);
	bool TT_Determine_Spawn_TM(PhysicalGameObj* object);
	bool Original_Determine_Spawn_TM(PhysicalGameObj* object);
	void Enable(bool _enabled) { enabled = _enabled; }
	const SpawnerDefClass* Get_Definition() const;
	uint32 Get_ID() const {return id;}
	Matrix3D Get_TM() const;
	void Init(const SpawnerDefClass& definition);
	void Set_ID(uint32 id);
	void Set_TM(const Matrix3D& transform);
	PhysicalGameObj* Spawn(sint32 id);
	PhysicalGameObj* Spawn_Object(sint32 id);

}; // size: 212


#endif
