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
#ifndef TT_INCLUDE_DAMAGEZONEGAMEOBJ_H
#define TT_INCLUDE_DAMAGEZONEGAMEOBJ_H
#include "DamageZoneGameObjDef.h"
#include "BaseGameObj.h"
#include "OBBoxClass.h"
class DamageZoneGameObj : public BaseGameObj {
private:
	OBBoxClass BoundingBox;
	float Time;
public:
	DamageZoneGameObj();
	~DamageZoneGameObj();
	const PersistFactoryClass &Get_Factory() const;
	bool Save(ChunkSaveClass &csave);
	bool Load(ChunkLoadClass &cload);
	void Init();
	void Init(const DamageZoneGameObjDef &);
	SCRIPTS_API const DamageZoneGameObjDef & Get_Definition( void ) const ;
	void Think();
	void Set_Bounding_Box(OBBoxClass &box) {BoundingBox = box;}
	OBBoxClass &Get_Bounding_Box() {return BoundingBox;}
};

#endif