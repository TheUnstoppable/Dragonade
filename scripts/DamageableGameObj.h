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
#ifndef TT_INCLUDE_DAMAGEABLEGAMEOBJ_H
#define TT_INCLUDE_DAMAGEABLEGAMEOBJ_H
#include "ScriptableGameObj.h"
#include "DefenseObjectClass.h"
#include "DamageableGameObjDef.h"
class DamageableGameObjDef;
class DamageableGameObj : public ScriptableGameObj
{
public:
	DamageableGameObj( void );
	virtual	~DamageableGameObj( void );
	void	Init( const DamageableGameObjDef & definition );
	void	Copy_Settings( const DamageableGameObjDef & definition );
	void	Re_Init( const DamageableGameObjDef & definition );
	const DamageableGameObjDef SCRIPTS_API & Get_Definition( void ) const ;
	virtual	bool	Save( ChunkSaveClass & csave );
	virtual	bool	Load( ChunkLoadClass & cload );
	DefenseObjectClass *	Get_Defense_Object( void )						{ return &DefenseObject; }
	virtual	void	Apply_Damage( const OffenseObjectClass & damager, float scale = 1.0f, int alternate_skin = -1 );
	virtual	void	Completely_Damaged( const OffenseObjectClass & damager ) {};
	const StringClass & 	Get_Info_Icon_Texture_Filename( void )		{ return Get_Definition().InfoIconTextureFilename; }
	int						Get_Translated_Name_ID( void ) const		{ return Get_Definition().TranslatedNameID; }
	virtual	bool			Is_Targetable( void ) const					{ return !Get_Definition().NotTargetable; }
	virtual	bool			Is_Health_Bar_Displayed( void ) const		{ return	IsHealthBarDisplayed; }
	virtual	void			Set_Is_Health_Bar_Displayed( bool state ) { IsHealthBarDisplayed = state; }
	virtual	DamageableGameObj	*As_DamageableGameObj( void )	{ return this; }
	virtual int Get_Player_Type(void) const {return PlayerType;}
	virtual void Set_Player_Type(int type);
	SCRIPTS_API bool		Is_Team_Player(void);
	Vector3		Get_Team_Color(void);
	SCRIPTS_API bool		Is_Teammate(DamageableGameObj * p_obj);
	SCRIPTS_API bool		Is_Enemy(DamageableGameObj * p_obj);
	virtual	void								Import_Occasional( BitStreamClass &packet );
	virtual	void								Export_Occasional( BitStreamClass &packet );
	bool Is_Damaged()
	{
		return (DefenseObject.Get_Shield_Strength() < DefenseObject.Get_Shield_Strength_Max() || DefenseObject.Get_Health() < DefenseObject.Get_Health_Max());
	}
protected:
	DefenseObjectClass	DefenseObject;
	int					PlayerType;
	bool					IsHealthBarDisplayed;
};

#endif
