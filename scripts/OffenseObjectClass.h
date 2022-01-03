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
#ifndef TT_INCLUDE_OFFENSEOBJECTCLASS_H
#define TT_INCLUDE_OFFENSEOBJECTCLASS_H
class ArmedGameObj;
class OffenseObjectClass {
public:
#define	DEFAULT_DAMAGE		1.0f
	OffenseObjectClass( float damage = DEFAULT_DAMAGE, unsigned int warhead = 0, ArmedGameObj *owner = NULL ) : Damage( damage ), Warhead( warhead ), ForceServerDamage( false ), EnableClientDamage( false ) { Set_Owner( owner ); }
	OffenseObjectClass( const OffenseObjectClass & base );
	~OffenseObjectClass() { Set_Owner( NULL ); };
	bool				Save( ChunkSaveClass & csave );
	bool				Load( ChunkLoadClass & cload );
	void	Set_Damage( float damage )	{ Damage = damage;	}
	void	Scale_Damage(float Scale) { Damage *= Scale; } //DA
	float	Get_Damage( void ) const 			{ return Damage;	}
	void	Set_Warhead( unsigned int warhead )	{ Warhead = warhead;	}
	unsigned int Get_Warhead( void ) const 				{ return Warhead;	}
	void	Set_Owner( ArmedGameObj *owner )	{ Owner = (ScriptableGameObj *)owner; }
	ArmedGameObj *Get_Owner( void ) const	{ return (ArmedGameObj *)Owner.Get_Ptr();	}
	bool ForceServerDamage;
	bool EnableClientDamage;
private:
	float Damage;
	unsigned int Warhead;
	ReferencerClass Owner;
};

#endif