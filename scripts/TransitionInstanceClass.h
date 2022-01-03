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
#ifndef TT_INCLUDE__TRANSITIONINSTANCECLASS_H
#define TT_INCLUDE__TRANSITIONINSTANCECLASS_H
class TransitionCompletionDataStruct;
class TransitionDataClass;
class SoldierGameObj;
#include "Matrix3.h"
#include "Matrix3D.h"
#include "ReferencerClass.h"
#include "TransitionGameObjDef.h"
#include "VehicleGameObj.h"
class TransitionInstanceClass
{
private:
	Matrix3D CharacterTransform; // 0000
	OBBoxClass TriggerZone; // 0030
	ReferencerClass Vehicle; // 006C
	const TransitionDataClass* Definition; // 007C
	int LadderIndex; // 0080
public:
	TransitionInstanceClass(TransitionDataClass  const&);
	~TransitionInstanceClass();
	void Start(SoldierGameObj *soldier);
	bool Check(SoldierGameObj *soldier, bool b);
	static void End(SoldierGameObj *,TransitionCompletionDataStruct *);
	void Set_Parent_Transform(Matrix3D const &tm);
	TransitionDataClass::StyleType Get_Type() {return Definition->Type;}
	void Set_Vehicle(VehicleGameObj *obj) {Vehicle = obj;}
	void Set_Ladder_Index(int index) {LadderIndex = index;}
	OBBoxClass &Get_Trigger_Zone() {return TriggerZone;}
	Matrix3D &Get_Character_Transform() {return CharacterTransform;}
}; // 0084

#endif