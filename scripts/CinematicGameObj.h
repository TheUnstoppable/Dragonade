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
#ifndef TT_INCLUDE_CINEMATICGAMEOBJ_H
#define TT_INCLUDE_CINEMATICGAMEOBJ_H

#include "ArmedGameObj.h"
class AudibleSoundClass;
class CinematicGameObjDef;
class CinematicGameObj : public ArmedGameObj {
private:
	AudibleSoundClass *Sound;
public:
	const PersistFactoryClass &Get_Factory() const;
	CinematicGameObj();
	~CinematicGameObj();
	void Init();
	void Init(const CinematicGameObjDef &);
	void Cinematic_Init();
	SCRIPTS_API const CinematicGameObjDef & Get_Definition( void ) const ;
	bool Save(ChunkSaveClass &csave);
	bool Load(ChunkLoadClass &cload);
	void On_Post_Load();
	void Set_Sound(int, char const *);
	void Think();
	void Post_Think();
	void Completely_Damaged(const OffenseObjectClass &offense);
	float Get_Animation_Length();
	void Export_Rare(BitStreamClass &BitStream);
	void Import_Rare(BitStreamClass &BitStream);
	bool Takes_Explosion_Damage() {return false;}
	CinematicGameObj * As_CinematicGameObj() {return this;}
};

#endif