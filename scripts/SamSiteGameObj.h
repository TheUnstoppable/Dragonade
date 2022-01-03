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
#ifndef TT_INCLUDE_SAMSITEGAMEOBJ_H
#define TT_INCLUDE_SAMSITEGAMEOBJ_H
#include "SmartGameObj.h"
#include "Vector3.h"
class SAMSiteGameObjDef;
class SAMSiteGameObj : public SmartGameObj {
private:
	int turretbone;
	int barrelbone;
	int mode;
	float time;
public:
	const PersistFactoryClass &Get_Factory() const;
	SAMSiteGameObj();
	~SAMSiteGameObj();
	void Init();
	void Init(const SAMSiteGameObjDef &);
	SCRIPTS_API const SAMSiteGameObjDef & Get_Definition( void ) const ;
	bool Save(ChunkSaveClass &csave);
	bool Load(ChunkLoadClass &cload);
	void Export_Frequent(BitStreamClass &BitStream);
	void Import_Frequent(BitStreamClass &BitStream);
	void Update_Turret(float weapon_turn,float weapon_tilt);
	bool Set_Targeting(Vector3  const&, bool);
	void Generate_Control();
	void Think();
};
#endif