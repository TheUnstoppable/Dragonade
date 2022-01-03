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
#ifndef TT_INCLUDE_TRANSITIONGAMEOBJDEF_H
#define TT_INCLUDE_TRANSITIONGAMEOBJDEF_H
#include "OBBoxClass.h"
#include "BaseGameObjDef.h"
class TransitionDataClass
{
public:
	enum StyleType {
		LADDER_EXIT_TOP,
		LADDER_EXIT_BOTTOM,
		LADDER_ENTER_TOP,
		LADDER_ENTER_BOTTOM,
		LEGACY_VEHICLE_ENTER_0,
		LEGACY_VEHICLE_ENTER_1,
		LEGACY_VEHICLE_EXIT_0,
		LEGACY_VEHICLE_EXIT_1,
		VEHICLE_ENTER,
		VEHICLE_EXIT,
	};
	StyleType Type; // 0000
	OBBoxClass TriggerZone; // 0004
	StringClass AnimationName; // 0040
	Matrix3D CharacterTransform; // 0044
	TransitionDataClass() : Type(LADDER_EXIT_TOP)
	{
	}
	bool Save(ChunkSaveClass &csave);
	bool Load(ChunkLoadClass &cload);
	StyleType Get_Type() const {return Type;}
}; // 0074

class TransitionGameObjDef : public BaseGameObjDef {
public:
	TransitionGameObjDef() {};
	~TransitionGameObjDef();
	const PersistFactoryClass &Get_Factory() const;
	bool Save(ChunkSaveClass &csave);
	bool Load(ChunkLoadClass &cload);
	uint32 Get_Class_ID() const;
	PersistClass *Create() const;
	void Free_Transition_List();
	DynamicVectorClass<TransitionDataClass *> &Get_Transitions() {return Transitions;}
	const DynamicVectorClass<TransitionDataClass *> &Get_Transitions() const {return Transitions;}
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
private:
	DynamicVectorClass<TransitionDataClass *> Transitions;
	friend class PresetDump;
};

#endif