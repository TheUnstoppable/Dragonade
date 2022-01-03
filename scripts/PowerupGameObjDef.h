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
#ifndef TT_INCLUDE_POWERUPGAMEOBJDEF_H
#define TT_INCLUDE_POWERUPGAMEOBJDEF_H
#include "SimpleGameObjDef.h"
class SmartGameObj;
class PowerUpGameObj;
class PowerUpGameObjDef : public SimpleGameObjDef
{
public:
	PowerUpGameObjDef( void );
	virtual uint32								Get_Class_ID( void ) const;
	virtual PersistClass *					Create( void ) const ;
	virtual bool								Save( ChunkSaveClass &csave );
	virtual bool								Load( ChunkLoadClass &cload );
	virtual const PersistFactoryClass &	Get_Factory( void ) const;
	bool Grant( SmartGameObj * obj, PowerUpGameObj * p_powerup = NULL, bool hud_display = true ) const ;
	int											Get_Grant_Weapon_ID (void) const	{ return GrantWeaponID; }
	bool Is_Persistant() const {return Persistent;}
	bool Is_Always_Allow_Grant() const {return AlwaysAllowGrant;}
	int Get_Grant_Sound() const {return GrantSoundID;}
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE (PowerUpGameObjDef, SimpleGameObjDef);
public:
	int											GrantShieldType;
	float											GrantShieldStrength;
	float											GrantShieldStrengthMax;
	float											GrantHealth;
	float											GrantHealthMax;
	int											GrantWeaponID;
	bool											GrantWeapon;
	int											GrantWeaponRounds;
	bool											GrantWeaponClips;
	bool											Persistent;
	int											GrantKey;
	bool											AlwaysAllowGrant;
	int											GrantSoundID;
	StringClass									GrantAnimationName;
	int											IdleSoundID;
	StringClass									IdleAnimationName;
	friend	class								PowerUpGameObj;
	friend class PresetDump;
  bool                    GrantShieldStrengthMaxIsScaled;
  bool                    GrantHealthMaxIsScaled;
};

#endif