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
#ifndef TT_INCLUDE_DAMAGEABLEGAMEOBJDEF_H
#define TT_INCLUDE_DAMAGEABLEGAMEOBJDEF_H
#include "ScriptableGameObjDef.h"
#include "DefenseObjectDefClass.h"
enum {
	PLAYERTYPE_MUTANT = -3,
	PLAYERTYPE_NEUTRAL,
	PLAYERTYPE_RENEGADE,
	PLAYERTYPE_NOD,
	PLAYERTYPE_GDI,
};
class DamageableGameObjDef : public ScriptableGameObjDef
{
public:
	DamageableGameObjDef( void );
	virtual bool								Save( ChunkSaveClass &csave );
	virtual bool								Load( ChunkLoadClass &cload );
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE( DamageableGameObjDef, ScriptableGameObjDef );
	int											Get_Name_ID (void) const	{ return TranslatedNameID; }
	int											Get_Encyclopedia_Type (void) const	{ return EncyclopediaType; }
	int											Get_Encyclopedia_ID (void) const		{ return EncyclopediaID; }
	const StringClass &						Get_Icon_Filename (void) const		{ return InfoIconTextureFilename; }
	int											Get_Translated_Name_ID (void) const	{ return TranslatedNameID; }
	void											Set_Translated_Name_ID (int TransID) {TranslatedNameID = TransID; }
	const DefenseObjectDefClass &			Get_DefenseObjectDef( void ) const	{ return DefenseObjectDef; }
	int											Get_Default_Player_Type(void) const { return DefaultPlayerType; }
	void Set_Name_ID(int id) {TranslatedNameID = id;}
//protected: //DA
	DefenseObjectDefClass						DefenseObjectDef; // 004C
	StringClass									InfoIconTextureFilename; // 006C
	int											TranslatedNameID; // 0070
	int											EncyclopediaType; // 0074
	int											EncyclopediaID; // 0078
	bool										NotTargetable; // 007C
	int											DefaultPlayerType; // 0080
	friend class DamageableGameObj;
	friend class PresetDump;
}; // 0084

#endif
