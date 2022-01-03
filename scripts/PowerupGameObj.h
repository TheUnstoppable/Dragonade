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
#ifndef TT_INCLUDE_POWERUPGAMEOBJ_H
#define TT_INCLUDE_POWERUPGAMEOBJ_H
#include "SimpleGameObj.h"
class AudibleSoundClass;
class WeaponBagClass;
class SmartGameObj;
class PowerUpGameObjDef;
class PowerUpGameObj : public SimpleGameObj {
public:
				PowerUpGameObj( void );
	virtual ~PowerUpGameObj( void );
	virtual	void	Init( void );
	void	Init( const PowerUpGameObjDef & definition );
	const PowerUpGameObjDef SCRIPTS_API & Get_Definition( void ) const ;
	virtual	bool	Save( ChunkSaveClass & csave );
	virtual	bool	Load( ChunkLoadClass & cload );
	virtual	void	On_Post_Load( void );		// MOVED
	virtual	const	PersistFactoryClass & Get_Factory( void ) const;
	virtual	void	Think( void );
	void	SCRIPTS_API Grant( SmartGameObj * obj );
	virtual PowerUpGameObj		*As_PowerUpGameObj()		{ return this; }
	static	PowerUpGameObj *	Create_Backpack( ArmedGameObj * provider );
	virtual	bool	Is_Always_Dirty( void )					{ return false; }
	virtual	void	Get_Description( StringClass & description );
	void	Expire( void );
	void	Set_State(int state); //DA
protected:
	enum
	{
		STATE_BECOMING_IDLE			= 0,
		STATE_IDLING,
		STATE_GRANTING,
		STATE_EXPIRING,
	};
	//void						Set_State( int state ); //DA
	void						Update_State( void );
	AudibleSoundClass *	IdleSoundObj;
	int						State;
	float						StateEndTimer;
	WeaponBagClass *		WeaponBag;				
	friend	class			PowerUpGameObjDef;
};

#endif