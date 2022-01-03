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
#ifndef TT_INCLUDE_TDBCATEGORYCLASS_H
#define TT_INCLUDE_TDBCATEGORYCLASS_H

#include "Persist.h"
#include "engine_string.h"
#include "engine_vector.h"

class TDBCategoryClass : public PersistClass
{
public:
	TDBCategoryClass (void);
	TDBCategoryClass (const TDBCategoryClass &src);
	virtual ~TDBCategoryClass (void);
	const TDBCategoryClass &operator= (const TDBCategoryClass &src);
	const PersistFactoryClass &Get_Factory (void) const;
	bool								Save (ChunkSaveClass &csave);
	bool								Load (ChunkLoadClass &cload);
	TDBCategoryClass *			Clone (void) const				{ return new TDBCategoryClass (*this); }
	const StringClass &			Get_Name (void) const			{ return Name; }
	void								Set_Name (const char *name)	{ Name = name; }
	uint32							Get_ID (void) const				{ return ID; }
	void								Set_ID (uint32 id)				{ ID = id; }
protected:
	void								Save_Variables (ChunkSaveClass &csave);
	void								Load_Variables (ChunkLoadClass &cload);
private:
	StringClass			Name;
	uint32				ID;
};
#endif
