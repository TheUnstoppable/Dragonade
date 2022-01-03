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
class GDI_AGT : public ScriptImpClass {
	int MissileID;
	int GunID[4];

	void Created(GameObject* AGTObj);
	void Killed(GameObject* AGTObj, GameObject* KillerObj);
	void Custom(GameObject* AGTObj, int Type, int Param, GameObject* Sender);
	void Create_Guns(GameObject* AGTObj);
	void Destroy_Guns(GameObject* AGTObj);
};

class GDI_AGT_Gun : public ScriptImpClass {
	int MissileID;
	int EnemyID;
	
	void Created(GameObject* GunObj);
	void Destroyed(GameObject* GunObj);
	void Enemy_Seen(GameObject* GunObj, GameObject* EnemyObj);
	void Timer_Expired(GameObject* GunObj, int Number);
	void Custom(GameObject* MissileObj, int type, int Param, GameObject* SenderObj);

	bool IsValidEnemy(GameObject* GunObj, GameObject* EnemyObj);
};

class GDI_AGT_Missile : public ScriptImpClass {
	int EnemyID;
	
	void Created(GameObject* MissileObj);
	void Destroyed(GameObject* MissileObj);
	void Timer_Expired(GameObject* MissileObj, int Number);
	void Custom(GameObject* MissileObj, int type, int Param, GameObject* SenderObj);

	bool IsValidEnemy(GameObject* WeaponObj, GameObject* EnemyObj);
};
