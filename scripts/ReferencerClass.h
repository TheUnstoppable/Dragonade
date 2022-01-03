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
#ifndef TT_INCLUDE__REFERENCERCLASS_H
#define TT_INCLUDE__REFERENCERCLASS_H

#include "PostLoadableClass.h"
#include "ReferenceableClass.h"
template <class T> class ReferenceableClass;
class SCRIPTS_API ReferencerClass : public PostLoadableClass
{

private:
	ReferenceableClass<ScriptableGameObj>* ReferenceTarget;
	ReferencerClass* TargetReferencerListNext;

public:
	friend class ReferenceableClass<ScriptableGameObj>;
	~ReferencerClass();

	bool Save         (ChunkSaveClass& chunkSaver);
	bool Load         (ChunkLoadClass& chunkLoader);
	void On_Post_Load ();

	ReferencerClass& operator = (ScriptableGameObj* object);
	ReferencerClass& operator = (const ReferencerClass& referencer);

	operator ScriptableGameObj* () const;
	inline ScriptableGameObj* operator ->() { return Get_Ptr(); }

	inline ReferencerClass()
		: ReferenceTarget(0), TargetReferencerListNext(0)
	{
	}


	inline ReferencerClass
		(ScriptableGameObj* object)
		: ReferenceTarget(0), TargetReferencerListNext(0)
	{
		*this = object;
	}


	inline ScriptableGameObj* Get_Ptr() const
	{
		return ReferenceTarget ? ReferenceTarget->Get_Data() : NULL;
	}


	inline void Set_Ptr
		(ScriptableGameObj* object)
	{
		*this = object;
	}


}; // size: 16

#endif
