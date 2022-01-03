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
#ifndef TT_INCLUDE__DEFINITIONMGRCLASS_H
#define TT_INCLUDE__DEFINITIONMGRCLASS_H

#include "scripts.h"
#include "engine_common.h"
#include "engine_threading.h"
#include "engine_string.h"
#include "engine_vector.h"
#include "PostLoadableClass.h"
#include "SaveLoadSubSystemClass.h"
#include "HashTemplateClass.h"
class DefinitionClass;
class ChunkSaveClass;
class ChunkLoadClass;
class DefinitionMgrClass : public SaveLoadSubSystemClass
{
private:
#ifndef TTLE_EXPORTS
	static HashTemplateClass<StringClass,DynamicVectorClass<DefinitionClass *> *>* DefinitionHash;
	static int _MaxDefinitionCount;
	SHARED_API static DefinitionClass **_SortedDefinitionArray;
	static int _DefinitionCount;
#endif
public:
	enum ID_TYPE
	{
		ID_CLASS = 1,
		ID_SUPERCLASS = 2,
	};
	static unsigned long Get_New_ID(uint32 class_id);
#ifndef TTLE_EXPORTS
	SHARED_API static DefinitionClass* Find_Definition(uint32 id, bool twiddle);
	SHARED_API static DefinitionClass* Find_Named_Definition(const char* name, bool twiddle);
	SHARED_API static DefinitionClass* Find_Typed_Definition(const char* name, uint32 class_id, bool twiddle);
	SHARED_API static void Register_Definition(DefinitionClass *definition);
	SHARED_API static void Unregister_Definition(DefinitionClass *definition);
#else
	static DefinitionClass* Find_Definition(uint32 id, bool twiddle);
	static DefinitionClass* Find_Named_Definition(const char* name, bool twiddle);
	static DefinitionClass* Find_Typed_Definition(const char* name, uint32 class_id, bool twiddle);
	static void Register_Definition(DefinitionClass *definition);
	static void Unregister_Definition(DefinitionClass *definition);
#endif
	SHARED_API static DefinitionClass *Get_First(uint32 id,ID_TYPE type = ID_CLASS);
#ifndef TTLE_EXPORTS
	static DefinitionClass *Get_First()
	{
		return _SortedDefinitionArray[0];
	}
#endif
	SHARED_API static DefinitionClass *Get_Next(DefinitionClass* curr_def,uint32 id,ID_TYPE type = ID_CLASS);
	SHARED_API static DefinitionClass *Get_Next(DefinitionClass* curr_def);
	SHARED_API static void Free_Definitions();
	static void Prepare_Definition_Array();
	static int fnCompareDefinitionsCallback(const void* elem1, const void* elem2);
	DefinitionMgrClass();
	bool Save_Objects(ChunkSaveClass& csave);
	bool Load_Objects(ChunkLoadClass& cload);
	bool Save_Variables(ChunkSaveClass& csave);
	bool Load_Variables(ChunkLoadClass& cload);
	virtual ~DefinitionMgrClass();
	virtual uint32 Chunk_ID() const
	{
		return 0x101;
	}
	virtual bool Contains_Data() const
	{
		return true;
	}
	virtual bool Save(ChunkSaveClass& csave);
	virtual bool Load(ChunkLoadClass& cload);
	virtual const char* Name() const
	{
		return "DefinitionMgrClass";
	}
};

#endif
