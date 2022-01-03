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
#ifndef TT_INCLUDE__POINTERREMAPCLASS_H
#define TT_INCLUDE__POINTERREMAPCLASS_H
#include "engine_vector.h"

class PointerRemapClass
{
	struct PtrPairStruct {
		void* OldPointer;
		void* NewPointer;
		PtrPairStruct(void* oldptr, void* newptr) : OldPointer(oldptr), NewPointer(newptr)
		{
		}
		PtrPairStruct() : OldPointer(0), NewPointer(0)
		{
		}
		bool operator==(const PtrPairStruct &that)
		{
			return (OldPointer == that.OldPointer) && (NewPointer == that.NewPointer);
		}
		bool operator!=(const PtrPairStruct &that)
		{
			return (OldPointer != that.OldPointer) || (NewPointer != that.NewPointer);
		}
	};
	struct PtrRemapStruct {
		void** PointerToRemap;
		PtrRemapStruct() : PointerToRemap(0)
		{
		}
		bool operator==(const PtrRemapStruct &that)
		{
			return PointerToRemap == that.PointerToRemap;
		}
		bool operator!=(const PtrRemapStruct &that)
		{
			return PointerToRemap != that.PointerToRemap;
		}
	};
	DynamicVectorClass<PtrPairStruct> PointerPairTable;
	DynamicVectorClass<PtrRemapStruct> PointerRequestTable;
	DynamicVectorClass<PtrRemapStruct> RefCountRequestTable;
	static int ptr_pair_compare_function(const void* ptr1,const void* ptr2);
	static int ptr_request_compare_function(const void* ptr1,const void* ptr2);
public:
	PointerRemapClass()
	{
		PointerPairTable.Set_Growth_Step(4096);
		PointerRequestTable.Set_Growth_Step(4096);
		RefCountRequestTable.Set_Growth_Step(4096);
	}
	~PointerRemapClass()
	{
	}
	void Reset();
	void Process();
	void Register_Pointer(void* old_pointer,void* new_pointer);
	void Request_Pointer_Remap(void** pointer_to_convert);
	void Request_Ref_Counted_Pointer_Remap(RefCountClass** pointer_to_convert);
	void Process_Request_Table(DynamicVectorClass<PtrRemapStruct>& request_table,bool refcount);
};


#endif
