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
#ifndef TT_INCLUDE_CULLSYSTEMCLASS_H
#define TT_INCLUDE_CULLSYSTEMCLASS_H
#include "CullableClass.h"
class FrustumClass;

class CullSystemClass
{
public:
	CullSystemClass(void);
	virtual ~CullSystemClass(void);
	void					Reset_Collection(void);
	virtual void		Collect_Objects(const Vector3 & point)					= 0;
	virtual void		Collect_Objects(const AABoxClass & box)				= 0;
	virtual void		Collect_Objects(const OBBoxClass & box)				= 0;
	virtual void		Collect_Objects(const FrustumClass & frustum)		= 0;
	virtual void		Update_Culling(CullableClass * obj)						= 0;
protected:
	CullableClass *	Get_First_Collected_Object_Internal(void);
	CullableClass *	Get_Next_Collected_Object_Internal(CullableClass * obj);
	CullableClass *	Peek_First_Collected_Object_Internal(void);
	CullableClass *	Peek_Next_Collected_Object_Internal(CullableClass * obj);
	void					Add_To_Collection(CullableClass * obj);
	CullableClass *	CollectionHead;
	friend class CullableClass;
};



#endif
