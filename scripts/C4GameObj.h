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
#ifndef TT_INCLUDE_C4GAMEOBJ_H
#define TT_INCLUDE_C4GAMEOBJ_H
#include "SimpleGameObj.h"

class PlayerDataClass;
class C4GameObjDef;
class AmmoDefinitionClass;

class C4GameObj : public SimpleGameObj
{
private:
	float TriggerTime; // 07B0
	ReferencerClass Owner; // 07B4
	PlayerDataClass *Player; // 07C4
	const AmmoDefinitionClass* AmmoDef; // 07C8
	int DetonationMode; // 07CC
    bool attached; // 07D0
    bool attachedToDynamic; // 07D1
	ReferencerClass attachObject; // 07D4
	Vector3 AttachLocation; // 07E4
	int AttachBoneIndex; // 07F0
	PhysClass* attachPhysics; // 07F4
	bool IsAttachedToMCT; // 07F8
	float SleepTime; // 07FC
public:

	const PersistFactoryClass &Get_Factory() const;
	C4GameObj();
	~C4GameObj();
	void Init();
	void Init(const C4GameObjDef &);
	SCRIPTS_API const C4GameObjDef & Get_Definition( void ) const ;
	void Init_C4(AmmoDefinitionClass const *ammoPreset, SoldierGameObj* owner, uint32 detonationMode, const Matrix3D& muzzle);
	CollisionReactionType Collision_Occurred(const CollisionEventClass& collide);
	bool Save(ChunkSaveClass &csave);
	bool Load(ChunkLoadClass &cload);
	void Think();
	void Post_Think();
	void Detonate();
	void Completely_Damaged(const OffenseObjectClass &offense);
	void Get_Information(StringClass &str);
	void Export_Rare(BitStreamClass &BitStream);
	void Import_Rare(BitStreamClass &BitStream);
	void Defuse();
    void Restore_Owner();
	static void Maintain_C4_Limit(int limit);
	C4GameObj * As_C4GameObj() {return this;}
	ScriptableGameObj * Get_Stuck_Object() {return attachObject;}
	SoldierGameObj *Get_Owner() const {return (SoldierGameObj*)Owner.Get_Ptr();}
	const AmmoDefinitionClass *Get_Ammo_Def() {return AmmoDef;}
	PlayerDataClass *Get_Player_Data() {return Player;}
	void Set_Player_Data(PlayerDataClass *player) { Player = player; } //DA
	int Get_Detonation_Mode() { return DetonationMode; } //DA
	void Set_Owner(ScriptableGameObj *obj) { Owner = obj; } //DA
};

#endif