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
#ifndef TT_INCLUDE__AIRSTRIPGAMEOBJ_H
#define TT_INCLUDE__AIRSTRIPGAMEOBJ_H



#include "VehicleFactoryGameObj.h"



class PersistFactoryClass;
class AirStripGameObjDef;
class ChunkSaveClass;
class ChunkLoadClass;
class BaseControllerClass;



class AirStripGameObj :
	public VehicleFactoryGameObj
{

private:

	float dropoffTime; // 08CC
	float unk08D0; // 08D0
	float lengthToVehicleDisplay; // 08D4
	float lengthToDropoff; // 08D8;
	bool isCinematicPlaying; // 08DC;
	PhysicalGameObj* dropCinematic; // 08E0

public:

	AirStripGameObj();
	virtual ~AirStripGameObj();
	virtual const PersistFactoryClass& Get_Factory() const;
	virtual void Init();
	void Init(const AirStripGameObjDef&);
	SCRIPTS_API const AirStripGameObjDef & Get_Definition( void ) const ;
	virtual bool Save(ChunkSaveClass&);
	virtual bool Load(ChunkLoadClass&);
	void Load_Variables(ChunkLoadClass&);
	virtual void CnC_Initialize(BaseControllerClass*);
	virtual void Think();
	virtual void Begin_Generation();
	void Start_Cinematic();
	virtual AirStripGameObj* As_AirStripGameObj() {return this;}

}; // 08E4



#endif