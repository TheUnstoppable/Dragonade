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
#ifndef TT_INCLUDE__BUILDINGAGGREGATEDEFCLASS_H
#define TT_INCLUDE__BUILDINGAGGREGATEDEFCLASS_H

#include "StaticAnimPhysDefClass.h"


class BuildingAggregateDefClass : public StaticAnimPhysDefClass 
{
public:
	static BuildingAggregateDefClass* repairBayAnimation;
	BuildingAggregateDefClass(void);
	virtual uint32								Get_Class_ID( void ) const;
	virtual const char *						Get_Type_Name(void)				{ return "BuildingAggregateDef"; }
	virtual bool								Is_Type(const char *);
	virtual PersistClass *					Create( void ) const ;
	virtual bool								Save( ChunkSaveClass &csave );
	virtual bool								Load( ChunkLoadClass &cload );
	virtual const PersistFactoryClass &	Get_Factory( void ) const;
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE( BuildingAggregateDefClass, StaticAnimPhysDefClass );
protected:
	bool											Save_State_Animation_Data(ChunkSaveClass & csave,int state_index);
	bool											Load_State_Animation_Data(ChunkLoadClass & cload,int state_index);
	enum 
	{
		ANIM_LOGIC_LINEAR						= 0,
		ANIM_LOGIC_LOOP,
		ANIM_LOGIC_SEQUENCE,
	};
	int											AnimLogicMode;
	bool											IsMCT;
	int											Frame0[10];
	int											Frame1[10];
	bool											AnimationEnabled[10];
	friend class BuildingAggregateClass;
	friend class PresetDump;
}; // RH7 216, RH8 214


#endif
