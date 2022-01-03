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
#ifndef TT_INCLUDE__WARFACTORYGAMEOBJ_H
#define TT_INCLUDE__WARFACTORYGAMEOBJ_H

#include "VehicleFactoryGameObj.h"

class WarFactoryGameObjDef;
class WarFactoryGameObj : public VehicleFactoryGameObj
{
public:
	WarFactoryGameObj (void);
	~WarFactoryGameObj (void);
	virtual	void						Init( void );
	void									Init (const WarFactoryGameObjDef & definition);
	SCRIPTS_API const WarFactoryGameObjDef &	Get_Definition (void) const;
	WarFactoryGameObj *				As_WarFactoryGameObj (void)	{ return this; }
	bool									Save (ChunkSaveClass &csave);
	bool									Load (ChunkLoadClass &cload);
	const	PersistFactoryClass &	Get_Factory (void) const;
	void					CnC_Initialize (BaseControllerClass *base);
	void					Think (void);
protected:
	void					Load_Variables (ChunkLoadClass &cload);
	void					Play_Creation_Animation (bool onoff);
	void					Begin_Generation (void);
	int					CreationAnimationID;
	float					CreationFinishedTimer;
};


#endif
