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
#pragma once
class Nod_Obelisk_CnC : public ScriptImpClass {
	int WeaponID;

	void Created(GameObject* ObeliskObj);
	void Killed(GameObject* ObeliskObj, GameObject* Killer);
	void Custom(GameObject* ObeliskObj, int Type, int Param, GameObject* Sender);

	void Create_Weapon(GameObject* ObeliskObj);
	void Destroy_Weapon(GameObject* ObeliskObj);
};

class Obelisk_Weapon_CnC : public ScriptImpClass {
	int EnemyID;
	int EffectID;
	bool Firing;
	bool Charged;

	void Created(GameObject* WeaponObj);
	void Destroyed(GameObject* WeaponObj);
	bool IsValidEnemy(GameObject* WeaponObj, GameObject* EnemyObj);

	void StartFiring(GameObject* WeaponObj);
	void StopFiring(GameObject* WeaponObj);
	void StartEffect(GameObject* WeaponObj);
	void StopEffect(GameObject* WeaponObj);
	void FireAt(GameObject* WeaponObj, GameObject* EnemyObj);
	void FaceEnemy(GameObject* WeaponObj, GameObject* EnemyObj);
	void StopFireAt(GameObject* WeaponObj);
	void Timer_Expired(GameObject* WeaponObj, int Number);
	void Enemy_Seen(GameObject* WeaponObj, GameObject* EnemyObj);
	public: void Register_Auto_Save_Variables();
};

class Nod_Obelisk_CnC_Ground : public ScriptImpClass {
	int WeaponID;

	void Created(GameObject* ObeliskObj);
	void Killed(GameObject* ObeliskObj, GameObject* Killer);
	
	void Custom(GameObject* ObeliskObj, int type, int Param, GameObject* Sender);
};

class Obelisk_Weapon_CnC_Ground : public ScriptImpClass {
	int EnemyID;
	int EffectID;
	bool Firing;
	bool Charged;

	void Created(GameObject* WeaponObj);
	void Destroyed(GameObject* WeaponObj);
	bool IsValidEnemy(GameObject* WeaponObj, GameObject* EnemyObj);

	void StartFiring(GameObject* WeaponObj);
	void StopFiring(GameObject* WeaponObj);
	void StartEffect(GameObject* WeaponObj);
	void StopEffect(GameObject* WeaponObj);
	void FireAt(GameObject* WeaponObj, GameObject* EnemyObj);
	void FaceEnemy(GameObject* WeaponObj, GameObject* EnemyObj);
	void StopFireAt(GameObject* WeaponObj);
	void Timer_Expired(GameObject* WeaponObj, int Number);
	void Enemy_Seen(GameObject* WeaponObj, GameObject* EnemyObj);
	public: void Register_Auto_Save_Variables();
};
