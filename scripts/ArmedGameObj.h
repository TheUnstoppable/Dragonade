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
#ifndef TT_INCLUDE_ARMEDGAMEOBJ_H
#define TT_INCLUDE_ARMEDGAMEOBJ_H
#include "PhysicalGameObj.h"
#include "ArmedGameObjDef.h"
#include "MuzzleRecoilClass.h"
class WeaponBagClass;
class WeaponClass;
class ArmedGameObj : public PhysicalGameObj {
public:
									ArmedGameObj( void );
	virtual						~ArmedGameObj( void );
	void							Init( const ArmedGameObjDef & definition );
	void							Copy_Settings( const ArmedGameObjDef & definition );
	void							Re_Init( const ArmedGameObjDef & definition );
	SCRIPTS_API const	ArmedGameObjDef & Get_Definition( void ) const ;
	virtual	bool				Save( ChunkSaveClass & csave );
	virtual	bool				Load( ChunkLoadClass & cload );
	virtual	void				On_Post_Load( void );
	virtual void                Export_Creation( BitStreamClass & packet );
	virtual	void				Import_Frequent( BitStreamClass & packet );
	virtual	void				Export_Frequent( BitStreamClass & packet );
	virtual	void				Import_State_Cs( BitStreamClass & packet );
	virtual	void				Export_State_Cs( BitStreamClass & packet );
	virtual	void				Post_Think();
	SCRIPTS_API WeaponClass	*				Get_Weapon( void );
	WeaponBagClass	*			Get_Weapon_Bag( void )						{ return WeaponBag; }
	bool							Muzzle_Exists( int index = 0 );
	virtual const Matrix3D &Get_Muzzle( int index = 0 );
	void							Start_Recoil( int muzzle_index,float recoil_scale,float recoil_time );
	float							Get_Weapon_Error( void )					{ return Get_Definition().WeaponError; }
	float							Get_Weapon_Turn_Min(void)					{ return Get_Definition().WeaponTurnMin; }
	float							Get_Weapon_Turn_Max(void)					{ return Get_Definition().WeaponTurnMax; }
	float							Get_Weapon_Tilt_Min(void)					{ return Get_Definition().WeaponTiltMin; }
	float							Get_Weapon_Tilt_Max(void)					{ return Get_Definition().WeaponTiltMax; }
	Vector3						Get_Targeting_Pos( void )					{ return TargetingPos; }
	virtual bool				Set_Targeting( const Vector3 & pos, bool do_tilt = true );
	virtual	ArmedGameObj	*As_ArmedGameObj( void )					{ return this; }
	bool Is_Recoiling(int index) {return MuzzleRecoilController[index].Is_Recoiling();}
protected:
	WeaponBagClass *			WeaponBag;
private:
	Vector3						TargetingPos;
	int							MuzzleA0Bone;
	int							MuzzleA1Bone;
	int							MuzzleB0Bone;
	int							MuzzleB1Bone;
	enum { MAX_MUZZLES = 4 };
	MuzzleRecoilClass			MuzzleRecoilController[MAX_MUZZLES];
public:
	void							Init_Muzzle_Bones( void );
};

#endif