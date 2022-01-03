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
#ifndef TT_INCLUDE__SOLDIERFACTORYGAMEOBJ_H
#define TT_INCLUDE__SOLDIERFACTORYGAMEOBJ_H
#include "BuildingGameObj.h"
class SoldierFactoryGameObjDef;
class SoldierFactoryGameObj : public BuildingGameObj
{
public:
	SoldierFactoryGameObj (void);
	~SoldierFactoryGameObj (void);
	virtual	void					Init( void );
	void								Init (const SoldierFactoryGameObjDef & definition);
	SCRIPTS_API const SoldierFactoryGameObjDef &	Get_Definition (void) const;
	SoldierFactoryGameObj *				As_SoldierFactoryGameObj (void)	{ return this; }
	bool									Save (ChunkSaveClass &csave);
	bool									Load (ChunkLoadClass &cload);
	const	PersistFactoryClass &	Get_Factory (void) const;
	void					CnC_Initialize (BaseControllerClass *base);
	void					On_Destroyed (void);
	void					On_Revived (void);
private:
	void					Load_Variables (ChunkLoadClass &cload);	
};

#endif
