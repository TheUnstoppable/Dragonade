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
#ifndef TT_INCLUDE_BEACONGAMEOBJ_H
#define TT_INCLUDE_BEACONGAMEOBJ_H
#include "SimpleGameObj.h"
class AudibleSoundClass;
class WeaponDefinitionClass;
class PlayerDataClass;
class BeaconGameObjDef;
class BaseControllerClass;
class BeaconGameObj :
	public SimpleGameObj
{
private:
	enum StateEnum
	{
		STATE_0, // Probably some idle/startup state
		STATE_DEPLOYING,
		STATE_DEPLOYED,
		STATE_DISARMED,
		STATE_DETONATED,
	};
	ReferencerClass Owner; // 07B0
	int State; // 07C0
	float ArmTime; // 07C4
	float PreDetonateCinematicDelay; // 07C8
	float DetonateTime; // 07CC
	float BroadcastToAllTime; // 07D0
	AudibleSoundClass* ArmedSound; // 07D4
	AudibleSoundClass* CurrentMessageSound; // 07D8
	bool PreDetonateCinematicActive; // 07DC
	const WeaponDefinitionClass* WeaponDef; // 07E0
	ReferencerClass CinematicObj; // 07E4
	PlayerDataClass* Player; // 07F4
public:
	BeaconGameObj();
	virtual ~BeaconGameObj();
	virtual const PersistFactoryClass& Get_Factory() const;
	virtual void Init();
	void Init(const BeaconGameObjDef &);
	SCRIPTS_API const BeaconGameObjDef & Get_Definition( void ) const ;
	void Init_Beacon(const WeaponDefinitionClass* definition, SoldierGameObj* owner, const Vector3& position);
	virtual bool Save(ChunkSaveClass& chunks);
	virtual bool Load(ChunkLoadClass& chunks);
	bool Load_Variables(ChunkLoadClass&);
	virtual void Think();
	virtual void Get_Information(StringClass& information);
	void Start_Cinematic(int);
	void Stop_Armed_Sound();
	void Set_State(int _state);
	void Update_State();
	BaseControllerClass* Get_Enemy_Base();
	bool Can_Place_Here(const Vector3& position);
	bool Is_In_Enemy_Base();
	void Stop_Current_Message_Sound();
	void Display_Message(int);
	void Begin_Arming();
	void Start_Owner_Animation();
	void Stop_Owner_Animation();
	bool Was_Owner_Interrupted();
	SoldierGameObj* Get_Owner();
	virtual void Completely_Damaged(const OffenseObjectClass& offense);
	void Create_Explosion();
	virtual void Export_Rare(BitStreamClass& stream);
	virtual void Import_Rare(BitStreamClass& stream);
	SoldierGameObj* Restore_Owner();
	virtual BeaconGameObj* As_BeaconGameObj() { return this; }
	PlayerDataClass *Get_Player_Data() {return Player;}
	int Get_State() { return State; }
	float Get_Arm_Time() { return ArmTime; }
	float Get_Detonate_Time() { return DetonateTime; }
	const WeaponDefinitionClass *Get_WeaponDef() { return WeaponDef; }
}; // 07F8

#endif