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
#ifndef TT_INCLUDE__ARMORWARHEADMANAGER_H
#define TT_INCLUDE__ARMORWARHEADMANAGER_H
#ifdef DDBEDIT
#error cant use this in ddbedit
#endif
#include "engine_string.h"
#include "datasafe.h"
typedef	unsigned int	ArmorType;
typedef	unsigned int	WarheadType;
class SCRIPTS_API ArmorWarheadManager
{
public:
	static void	Init( void );
	static void Shutdown( void );
	static bool				Is_Armor_Soft( ArmorType armor );
	static int			 	Get_Num_Armor_Types( void );
	static ArmorType		Get_Armor_Type( const char *name );
	static const char * 	Get_Armor_Name( ArmorType type );
	static int			 	Get_Num_Warhead_Types( void );
	static WarheadType	Get_Warhead_Type( const char *name );
	static const char *	Get_Warhead_Name( WarheadType type );
	static int			 	Get_Armor_Save_ID( ArmorType type );
	static ArmorType	 	Find_Armor_Save_ID( int id );
	static int			 	Get_Warhead_Save_ID( WarheadType type );
	static ArmorType	 	Find_Warhead_Save_ID( int id );
	static float	Get_Damage_Multiplier( ArmorType armor, WarheadType warhead );
	static float	Get_Shield_Absorbsion( ArmorType armor, WarheadType warhead );
	typedef enum { 
		SPECIAL_DAMAGE_TYPE_NONE,
		SPECIAL_DAMAGE_TYPE_FIRE,
		SPECIAL_DAMAGE_TYPE_CHEM,
		SPECIAL_DAMAGE_TYPE_ELECTRIC,
		SPECIAL_DAMAGE_TYPE_CNC_FIRE,
		SPECIAL_DAMAGE_TYPE_CNC_CHEM,
		SPECIAL_DAMAGE_TYPE_SUPER_FIRE,
		NUM_SPECIAL_DAMAGE_TYPES,			// Keep synced with ouch_types
	} SpecialDamageType;
	static SpecialDamageType	Get_Special_Damage_Type( WarheadType type );
	static float					Get_Special_Damage_Probability( WarheadType type );
	static WarheadType 			Get_Special_Damage_Warhead( SpecialDamageType type );
	static float					Get_Special_Damage_Duration( SpecialDamageType type );
	static float					Get_Special_Damage_Scale( SpecialDamageType type );
	static const char *			Get_Special_Damage_Explosion( SpecialDamageType type );
	static float					Get_Visceroid_Probability( WarheadType type );
	static bool						Is_Skin_Impervious( SpecialDamageType type, ArmorType skin );
	static	safe_float			*Multipliers;
	static	safe_float			*Absorbsion;
};


#endif
