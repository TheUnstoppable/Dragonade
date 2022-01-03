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
#ifndef TT_INCLUDE__TDBOBJCLASS_H
#define TT_INCLUDE__TDBOBJCLASS_H

#include "Persist.h"
#include "engine_string.h"
#include "engine_vector.h"
class StringTwiddlerClass;

class TDBObjClass : public PersistClass
{
public:
	TDBObjClass (void);
	TDBObjClass (const TDBObjClass &src);
	virtual ~TDBObjClass (void);
	const TDBObjClass &	operator= (const TDBObjClass &src);
	virtual StringTwiddlerClass *	As_StringTwiddlerClass (void)		{ return NULL; }
	const PersistFactoryClass &	Get_Factory (void) const;
	bool									Save (ChunkSaveClass &csave);
	bool									Load (ChunkLoadClass &cload);
	virtual TDBObjClass *				Clone (void) const					{ return new TDBObjClass (*this); }
	virtual const WideStringClass &	Get_String (uint32 lang_id);
	virtual const WideStringClass &	Get_String (void);
	virtual const StringClass &		Get_English_String (void)			{ return EnglishString; }
	virtual uint32							Get_ID (void)							{ return ID; }
	virtual const StringClass &		Get_ID_Desc (void)					{ return IDDesc; }
	virtual uint32							Get_Sound_ID (void)					{ return SoundID; }
	virtual const StringClass &		Get_Animation_Name (void)			{ return AnimationName; }
	virtual uint32							Get_Category_ID (void)				{ return CategoryID; }
	virtual void							Set_String (uint32 lang_id, const wchar_t *string);
	virtual void							Set_English_String (const char *string);
	virtual void							Set_ID (uint32 id);
	virtual void							Set_ID_Desc (const char *desc);
	virtual void							Set_Animation_Name (const char *name)	{ AnimationName = name; }
	virtual void							Set_Sound_ID (uint32 id)					{ SoundID = id; }
	virtual void							Set_Category_ID (uint32 id)				{ CategoryID = id; }
	virtual bool							Contains_Translation (uint32 lang_id);
protected:
	void								Save_Variables (ChunkSaveClass &csave);
	void								Load_Variables (ChunkLoadClass &cload);
	StringClass									EnglishString;
	StringClass									IDDesc;
	uint32										ID;
	uint32										SoundID;
	uint32										CategoryID;
	StringClass									AnimationName;
	DynamicVectorClass<WideStringClass>	TranslatedStrings;
};

#endif