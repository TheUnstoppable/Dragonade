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
#pragma once

#include "StaticAnimPhysClass.h"


class BuildingAggregateDefClass;
class BuildingAggregateClass : public StaticAnimPhysClass 
{
public:
	BuildingAggregateClass( void );
	virtual ~BuildingAggregateClass( void );
	void	Init( const BuildingAggregateDefClass & definition );
	const BuildingAggregateDefClass * Get_BuildingAggregateDef( void ) const {return (BuildingAggregateDefClass *)Definition; }
	int				Get_Current_State(void);
	void				Set_Current_State(int new_state,bool force_update = false);
	SCRIPTS_API bool				Is_MCT(void);
	virtual	bool	Save( ChunkSaveClass & csave );
	virtual	bool	Load( ChunkLoadClass & cload );
	virtual	void	Save_State( ChunkSaveClass & csave );
	virtual	void	Load_State( ChunkLoadClass & cload );
	virtual	void	On_Post_Load( void );
	virtual	const	PersistFactoryClass & Get_Factory( void ) const;
protected:
	bool				Is_Power_On(int state);
	int				CurrentState;
}; // 00E8  00E8  0104  0100
