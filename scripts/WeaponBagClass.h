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
#ifndef TT_INCLUDE_WEAPONBAGCLASS_H
#define TT_INCLUDE_WEAPONBAGCLASS_H
#include "ArmedGameObj.h"
class WeaponDefinitionClass;
class InventoryClass;
class	WeaponBagClass {
public:
	WeaponBagClass( ArmedGameObj * owner );
	~WeaponBagClass( void );
	bool				Save( ChunkSaveClass & csave );
	bool				Load( ChunkLoadClass & cload );
	bool				Is_Weapon_Owned( int weapon_id );
	bool				Is_Ammo_Full( int weapon_id );
	SCRIPTS_API WeaponClass * 	Add_Weapon( const WeaponDefinitionClass * def, int rounds = 0, bool give_weapon = true ); //DA
	SCRIPTS_API WeaponClass * 	Add_Weapon( int id, int rounds = 0, bool give_weapon = true ); //DA
	SCRIPTS_API WeaponClass * 	Add_Weapon( const char *weapon_name, int rounds = 0, bool give_weapon = true ); //DA
	SCRIPTS_API void				Remove_Weapon( int index ); //DA
	SCRIPTS_API void				Remove_Weapon(const WeaponDefinitionClass *WeaponDef); //DA
	SCRIPTS_API void				Remove_Weapon(const char *Weapon); //DA
	SCRIPTS_API void				Remove_Weapon(unsigned int WeaponDefID); //DA
	SCRIPTS_API void				Clear_Weapons( void ); //DA
	int				Get_Count( void )					{ return WeaponList.Count(); }
	WeaponClass *	Peek_Weapon( int index )		{ return WeaponList[ index ]; }
	WeaponClass	*	Get_Weapon( void )				{ return	WeaponList[ WeaponIndex ]; }
	WeaponClass	*	Get_Next_Weapon( void );
	void				Import_Weapon_List(BitStreamClass & packet);
	void				Export_Weapon_List(BitStreamClass & packet);
	void				Import_Ammo_List(BitStreamClass & packet);
	void				Export_Ammo_List(BitStreamClass & packet);
	int				Get_Index( void )					{ return WeaponIndex; }
	SCRIPTS_API void				Select_Index( int index ); //DA
	SCRIPTS_API void				Select_Next( void ); //DA
	SCRIPTS_API void				Select_Prev( void ); //DA
	void				Select_Key_Number( int key_number );
	SCRIPTS_API void				Select_Weapon( WeaponClass * weapon ); //DA
	SCRIPTS_API void				Select_Weapon_ID( int weapon_id ); //DA
	SCRIPTS_API void				Select_Weapon_Name( const char * name ); //DA
	void				Deselect( void );
 	bool				Is_Changed( void )					{ return IsChanged; }
 	void				Force_Changed( void )				{ IsChanged = true; }
	void				Reset_Changed( void )				{ IsChanged = false; }
 	bool				HUD_Is_Changed( void )					{ return HUDIsChanged; }
	void				HUD_Reset_Changed( void )				{ HUDIsChanged = false; }
	bool				Move_Contents( WeaponBagClass * source );
	void				Store_Inventory( InventoryClass * );
	void				Restore_Inventory(InventoryClass *); //DA
	ArmedGameObj *Get_Owner() { return Owner; } //DA
	SCRIPTS_API WeaponClass *Find_Weapon(const WeaponDefinitionClass *WeaponDef); //DA
	SCRIPTS_API WeaponClass *Find_Weapon(const char *Weapon); //DA
	SCRIPTS_API WeaponClass *Find_Weapon(unsigned int WeaponDefID); //DA
	SCRIPTS_API int Get_Weapon_Position(const WeaponDefinitionClass *WeaponDef); //DA
	SCRIPTS_API int Get_Weapon_Position(const char *Weapon); //DA
	SCRIPTS_API int Get_Weapon_Position(unsigned int WeaponDefID); //DA
	SCRIPTS_API int Get_Weapon_Position(WeaponClass *Weapon); //DA
	void                Owner_Initialized(bool b) {OwnerInitialized = b;}
	bool                Is_Owner_Initialized() {return OwnerInitialized;}
private:

	ArmedGameObj						*Owner; //0
	DynamicVectorClass<WeaponClass*>	WeaponList; //4
	int										WeaponIndex; //1C
	bool										IsChanged; //20
	bool										HUDIsChanged; //21
	bool                                        OwnerInitialized; //22
	//WeaponClass *	Find_Weapon( const WeaponDefinitionClass * def ); //DA
	void		Mark_Owner_Dirty( void );
	bool		Should_Skip_Weapon(WeaponClass* weapon);
}; // 0024

#endif