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
#ifndef TT_INCLUDE__DEFENSEOBJECTCLASS_H
#define TT_INCLUDE__DEFENSEOBJECTCLASS_H
#include "DataSafe.h"
#include "ReferencerClass.h"

class OffenseObjectClass;
class SmartGameObj;
class DamageableGameObj;
class DefenseObjectDefClass;
class BitStreamClass;
typedef	unsigned int	ArmorType;
typedef	unsigned int	WarheadType;
typedef	safe_unsigned_int	SafeArmorType;
typedef	safe_unsigned_int	SafeWarheadType;
class DefenseObjectClass {
#define	DEFAULT_HEALTH		100.0f
public:
	DefenseObjectClass( float health = DEFAULT_HEALTH, ArmorType skin = 0 ) :
      Health( health ), HealthMax( health ), Skin( skin ), ShieldStrength( 0 ), ShieldStrengthMax( 0 ), ShieldType( 0 ), CanObjectDie( true ) {};//,
      //LastSentHealth( -1 ), LastSentSkin( -1 ), LastSentShieldStrength( -1 ), LastSentShieldType( -1 )  {};
	~DefenseObjectClass() {};
	void				Init( const DefenseObjectDefClass & def, DamageableGameObj * owner);
	bool				Save( ChunkSaveClass & csave );
	bool				Load( ChunkLoadClass & cload );
	void	Set_Owner( DamageableGameObj *owner )	{ Owner = (ScriptableGameObj *)owner; }
	DamageableGameObj *Get_Owner( void ) const	{ return (DamageableGameObj *)Owner.Get_Ptr();	}
	void	Set_Skin( ArmorType skin )		{ Skin = skin; }
	ArmorType	Get_Skin( void ) const 	{ return Skin;	}
	enum {MAX_MAX_HEALTH					= 2000};//500};
	enum {MAX_MAX_SHIELD_STRENGTH		= 2000};//500};
	enum {MAX_MAX_HEALTH_NEW					= 10000};//500};
	enum {MAX_MAX_SHIELD_STRENGTH_NEW		= 10000};//500};
	void				SCRIPTS_API Set_Health(float health); //DA
	void				SCRIPTS_API Add_Health(float add_health); //DA
	float				SCRIPTS_API Get_Health(void) const ;
	void				SCRIPTS_API Set_Health_Max(float health);
	float				SCRIPTS_API Get_Health_Max(void) const ;
	void				SCRIPTS_API Set_Shield_Strength(float str); //DA
	void				SCRIPTS_API Add_Shield_Strength(float str); //DA
	float				SCRIPTS_API Get_Shield_Strength(void) const ;
	void				SCRIPTS_API Set_Shield_Strength_Max(float str);
	float				SCRIPTS_API Get_Shield_Strength_Max(void) const ;
	void				SCRIPTS_API Set_Shield_Type( ArmorType type );
	unsigned long	Get_Shield_Type( void ) const 		{ return ShieldType; }
	float			Apply_Damage( const OffenseObjectClass	& offense, float scale = 1.0f, int alternate_skin = -1 );
	float			Do_Damage( const OffenseObjectClass	& offense, float scale = 1.0f, int alternate_skin = -1 );
	void			Request_Damage( const OffenseObjectClass & offense, float scale = 1.0f );
	bool			Is_Repair( const OffenseObjectClass	& offense, float scale = 1.0f );
	bool			Would_Damage( const OffenseObjectClass	& offense, float scale = 1.0f );
	bool			Is_Soft( void );
	void			Set_Can_Object_Die( bool onoff )	{ CanObjectDie = onoff; }
	virtual	void	Import( BitStreamClass & packet );
	virtual	void	Export( BitStreamClass & packet );
	static void Set_Precision(void);
	float			Get_Damage_Points(void) const			{ return DamagePoints; }
	float			Get_Death_Points(void) const			{ return DeathPoints; }
	bool  Can_Trust_Client_Damage (SmartGameObj* damager) const;
	void				SCRIPTS_API Mark_Owner_Dirty( void );
	void Set_Damage_Points(float points) {DamagePoints = points;}
	void Set_Death_Points(float points) {DeathPoints = points;}
	bool Get_Can_Object_Die() { return CanObjectDie; } //DA
private:
	safe_float		Health;
	safe_float		HealthMax;
	SafeArmorType	Skin;
	safe_float		ShieldStrength;
	safe_float		ShieldStrengthMax;
	SafeArmorType	ShieldType;
	safe_float		DamagePoints;
	safe_float		DeathPoints;
	bool				CanObjectDie;
	ReferencerClass	Owner;
};
#endif
