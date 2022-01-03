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
#ifndef TT_INCLUDE_EDITABLE_H
#define TT_INCLUDE_EDITABLE_H
#include "Persist.h"
#include "Parameter.h"
class EditableClass : public PersistClass {
public:
	virtual int						Get_Parameter_Count (void) const;
	virtual ParameterClass *	Lock_Parameter (int i);
	virtual void					Unlock_Parameter (int i);
}; // 0008
inline int
EditableClass::Get_Parameter_Count (void) const
{
	return 0;
}
inline ParameterClass *
EditableClass::Lock_Parameter (int i)
{
	return NULL;
}
inline void
EditableClass::Unlock_Parameter (int i)
{
}

#endif