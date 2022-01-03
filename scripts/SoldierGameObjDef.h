/*	Renegade Scripts.dll
	Copyright 2013 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#ifndef TT_INCLUDE__SOLDIERGAMEOBJDEF_H
#define TT_INCLUDE__SOLDIERGAMEOBJDEF_H

#include "scripts.h"
#include "SmartGameObjDef.h"
#include "DialogueClass.h"
#include "engine_common.h"
#include "engine_vector.h"
#include "engine_threading.h"
#include "engine_string.h"
class SoldierGameObjDef : public SmartGameObjDef
{
public:
	SoldierGameObjDef( void );
	virtual uint32								Get_Class_ID( void ) const;
	virtual PersistClass *					Create( void ) const ;
	virtual bool								Save( ChunkSaveClass &csave );
	virtual bool								Load( ChunkLoadClass &cload );
	virtual const PersistFactoryClass &	Get_Factory( void ) const;
	DialogueClass *							Get_Dialog_List( void )			{ return DialogList; }
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
	virtual void                        DumpPhys (FileClass &file);
#endif
	DECLARE_EDITABLE( SoldierGameObjDef, SmartGameObjDef );
	float Get_Skeleton_Height() const {return SkeletonHeight;};
	float Get_Skeleton_Width() const {return SkeletonWidth;};
	bool Is_Spy() const {return IsSpy;}
	bool Can_Refill() const { return CanRefill; }
	bool Is_Unsquishable() const { return IsUnsquishable; }
protected:
	float											TurnRate;
	float											JumpVelocity;
	float											SkeletonHeight;
	float											SkeletonWidth;
	bool											UseInnateBehavior;
	float											InnateAggressiveness;
	float											InnateTakeCoverProbability;
	bool											InnateIsStationary;
	DialogueClass								DialogList[DIALOG_MAX];
	StringClass									FirstPersonHands;
	int											HumanAnimOverrideDefID;
	int											HumanLoiterCollectionDefID;
	int											DeathSoundPresetID;
	bool										CanStealVehicles;
	bool										CanDriveVehicles;
	bool										IsSpy;
	bool										IsUnsquishable;
	bool CanRefill;

	friend	class								SoldierGameObj;
	friend	class								SoldierObserverClass;
	friend class PresetDump;
public:
	bool	GetInnateBehavior(){ return UseInnateBehavior;}
	float	GetInnateAggressiveness(){ return InnateAggressiveness;}
	float	GetInnateTakeCoverProbability(){ return InnateTakeCoverProbability;}
	bool	GetInnateIsStationary(){ return InnateIsStationary;}
}; // size: 912


#endif
