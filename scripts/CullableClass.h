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
#ifndef TT_INCLUDE__CULLABLECLASS_H
#define TT_INCLUDE__CULLABLECLASS_H
#include "AABoxClass.h"
#include "engine_vector.h"

class CullSystemClass;
class CullLinkClass
{
public:
	inline CullLinkClass(CullSystemClass * system)								{ System = system;}
	virtual ~CullLinkClass(void)												{ }
	inline void					Set_Culling_System(CullSystemClass * sys)		{ System = sys; }	
	inline CullSystemClass * Get_Culling_System(void)							{ return System; }
protected:
	CullSystemClass * System;
};
class CullableClass : public RefCountClass
{
public:
	CullableClass(void);
	virtual ~CullableClass(void);
	inline const AABoxClass & Get_Cull_Box(void) const							{ return CullBox; }
	void								Set_Cull_Box(const AABoxClass & box,bool just_loaded = false);
	void								Set_Culling_System(CullSystemClass * sys);
	CullSystemClass *				Get_Culling_System(void) const;
	inline void					Set_Cull_Link(CullLinkClass * c)					{ CullLink = c; }
	inline CullLinkClass *	Get_Cull_Link(void) const							{ return CullLink; }
private:
	inline void					Set_Next_Collected(CullableClass * c)			{ NextCollected = c; }
	inline CullableClass *	Get_Next_Collected(void)							{ return NextCollected; }
	CullLinkClass *				CullLink;
	AABoxClass						CullBox;
	CullableClass *				NextCollected;
	CullableClass(const CullableClass & src);
	CullableClass & operator = (const CullableClass & src);
	friend class CullSystemClass;
}; // 0028  0040

#endif
