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
#ifndef TT_INCLUDE_SCRIPTZONEGAMEOBJ_H
#define TT_INCLUDE_SCRIPTZONEGAMEOBJ_H
#include "SList.h"
#include "ReferencerClass.h"
#include "ScriptZoneGameObjDef.h"
#include "ScriptableGameObj.h"
#include "OBBoxClass.h"
class SmartGameObj;
class RenderObjClass;
class ScriptZoneGameObj : public ScriptableGameObj {
private:
	OBBoxClass BoundingBox;
	int PlayerType;
	SList<ReferencerClass> ZoneContents;

	RenderObjClass* Model;
public:
	ScriptZoneGameObj();
	~ScriptZoneGameObj();
	const PersistFactoryClass &Get_Factory() const;
	void Init();
	void Init(const ScriptZoneGameObjDef &);
	const ScriptZoneGameObjDef SCRIPTS_API & Get_Definition( void ) const ;
	bool Save(ChunkSaveClass &csave);
	bool Load(ChunkLoadClass &cload);
	void On_Post_Load();
	void Think();
	bool In_List(PhysicalGameObj *);
	bool Inside_Me(PhysicalGameObj *obj);
	static ScriptZoneGameObj *Find_Closest_Zone(Vector3 &,ZoneConstants::ZoneType);
	int Count_Team_Members_Inside(int);
	ScriptZoneGameObj * As_ScriptZoneGameObj() {return this;}
	void Get_Position(Vector3 *Position) const {*Position = BoundingBox.Center;}
	void Set_Bounding_Box(OBBoxClass &box) {BoundingBox = box;}
	OBBoxClass &Get_Bounding_Box() {return BoundingBox;}
	int Get_Player_Type() const {return PlayerType;}
	void Set_Player_Type(int type) {PlayerType = type;}
	bool Is_Environment_Zone() {return Get_Definition().IsEnvironmentZone;}
	virtual bool Point_In_Zone(const Vector3 &v);
};

#endif