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
#ifndef TT_INCLUDE_PHYSICALGAMEOBJDEF_H
#define TT_INCLUDE_PHYSICALGAMEOBJDEF_H
#include "DamageableGameObjDef.h"
typedef enum
{
	TEAM_VISIBILITY_ALL,
	TEAM_VISIBILITY_FRIENDLY,
	TEAM_VISIBILITY_ENEMY
} TeamVisibilitySetting;
class PhysicalGameObjDef : public DamageableGameObjDef
{
public:
	PhysicalGameObjDef( void );
	virtual bool								Save( ChunkSaveClass &csave );
	virtual bool								Load( ChunkLoadClass &cload );
	virtual bool								Is_Valid_Config (StringClass &message);
	int		Get_Phys_Def_ID( void ) const	{ return PhysDefID; }
	int		Get_Orator_Type( void ) const	{ return OratorType; }
	StringClass Get_Animation() const {return Animation;}
	bool Ignore_For_Tracking() const {return IgnoreForTracking;}
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE( PhysicalGameObjDef, DamageableGameObjDef );
//protected: //DA
	int											Type;					
	int											RadarBlipType;
	float										BullseyeOffsetZ;
	StringClass									Animation;
	int											PhysDefID;
	int											KilledExplosion;
	bool										DefaultHibernationEnable;
	bool										AllowInnateConversations;
	int											OratorType;
	bool										UseCreationEffect;
	char                                        TeamVisibility;
	bool                                        IgnoreForTracking;
	friend	class								PhysicalGameObj;
	friend class PresetDump;
};

#endif