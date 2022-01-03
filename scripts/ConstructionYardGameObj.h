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
#ifndef TT_INCLUDE__CONSTRUCTIONYARDGAMEOBJ_H
#define TT_INCLUDE__CONSTRUCTIONYARDGAMEOBJ_H

#include "BuildingGameObj.h"

class ConstructionYardGameObjDef;
class ConstructionYardGameObj : public BuildingGameObj
{
public:
	ConstructionYardGameObj (void);
	~ConstructionYardGameObj (void);
	virtual	void					Init( void );
	void								Init (const ConstructionYardGameObjDef & definition);
	SCRIPTS_API const ConstructionYardGameObjDef &	Get_Definition (void) const;
	ConstructionYardGameObj *				As_ConstructionYardGameObj (void)	{ return this; }
	bool									Save (ChunkSaveClass &csave);
	bool									Load (ChunkLoadClass &cload);
	const	PersistFactoryClass &	Get_Factory (void) const;
	void					Think (void);
	void Set_Spy(bool b) {IsSpy = b;}
private:
	float RepairTime;
	float VehicleRepairTime;
	bool IsSpy;
	void					Load_Variables (ChunkLoadClass &cload);	
};


#endif
