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
#ifndef TT_INCLUDE_SIMPLEGAMEOBJ_H
#define TT_INCLUDE_SIMPLEGAMEOBJ_H
#include "PhysicalGameObj.h"
#include "SimpleGameObjDef.h"
class SimpleGameObj : public PhysicalGameObj {
public:
	SimpleGameObj();
	~SimpleGameObj();
	void On_Post_Load();
	const PersistFactoryClass &Get_Factory() const;
	bool Save(ChunkSaveClass &csave);
	bool Load(ChunkLoadClass &cload);
	void Init();
	void Init(const SimpleGameObjDef &);
	bool Is_Always_Dirty() {return false;}
	SimpleGameObj * As_SimpleGameObj() {return this;}
	const SimpleGameObjDef SCRIPTS_API & Get_Definition( void ) const;
	bool Is_Hidden_Object() {return Get_Definition().IsHiddenObject;}
	void Export_Rare( BitStreamClass &packet );
	void Import_Rare( BitStreamClass &packet );
}; // 07B0

#endif