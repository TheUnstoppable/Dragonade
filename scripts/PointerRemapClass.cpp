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
#include "general.h"
#include "PointerRemapClass.h"

void PointerRemapClass::Process()
{
	if (PointerPairTable.Count() > 0)
	{
		qsort(&(PointerPairTable[0]),PointerPairTable.Count(),sizeof(PtrPairStruct),ptr_pair_compare_function);
	}
	if (PointerRequestTable.Count() > 0)
	{
		qsort(&(PointerRequestTable[0]),PointerRequestTable.Count(),sizeof(PtrRemapStruct),ptr_request_compare_function);
		Process_Request_Table(PointerRequestTable,false);
	}
	if (RefCountRequestTable.Count() > 0)
	{
		qsort(&(RefCountRequestTable[0]),RefCountRequestTable.Count(),sizeof(PtrRemapStruct),ptr_request_compare_function);
		Process_Request_Table(RefCountRequestTable,true);
	}
}

void PointerRemapClass::Reset()
{
	PointerPairTable.Delete_All();
	PointerRequestTable.Delete_All();
	RefCountRequestTable.Delete_All();
}

void PointerRemapClass::Register_Pointer(void* old_pointer,void* new_pointer)
{
	PtrPairStruct ptr(old_pointer,new_pointer);
	PointerPairTable.Add(ptr);
}

void PointerRemapClass::Request_Pointer_Remap(void** pointer_to_convert)
{
	PtrRemapStruct ptr;
	ptr.PointerToRemap = pointer_to_convert;
	PointerRequestTable.Add(ptr);
}

void PointerRemapClass::Request_Ref_Counted_Pointer_Remap(RefCountClass** pointer_to_convert)
{
	PtrRemapStruct ptr;
	ptr.PointerToRemap = (void **)pointer_to_convert;
	RefCountRequestTable.Add(ptr);
}

void PointerRemapClass::Process_Request_Table(DynamicVectorClass<PtrRemapStruct>& request_table,bool refcount)
{
	int j = 0;
	for (int i = 0;i < request_table.Count();i++)
	{
		void *ptr = *(request_table[i].PointerToRemap);
		int pos = j;
		for (pos = j;j < PointerPairTable.Count();j++)
		{
			if (PointerPairTable[j].OldPointer >= ptr)
			{
				break;
			}
		}
		if (j >= PointerPairTable.Count())
		{
			j = pos;
			*request_table[i].PointerToRemap = 0;
		}
		if (PointerPairTable[j].OldPointer != ptr)
		{
			j = pos;
			*request_table[i].PointerToRemap = 0;
		}
		else
		{
			*request_table[i].PointerToRemap = PointerPairTable[j].NewPointer;
			if (refcount)
			{
				((RefCountClass *)(*request_table[i].PointerToRemap))->Add_Ref();
			}
		}
	}
}

int PointerRemapClass::ptr_pair_compare_function(const void* ptr1,const void* ptr2)
{
	int result = 0;
	if (((PtrPairStruct *)ptr1)->OldPointer != ((PtrPairStruct *)ptr2)->OldPointer)
	{
		result = -(((PtrPairStruct *)ptr1)->OldPointer < ((PtrPairStruct *)ptr2)->OldPointer) | 1;
	}
	return result;
}

int PointerRemapClass::ptr_request_compare_function(const void* ptr1,const void* ptr2)
{
	int result = 0;
	if (*((PtrRemapStruct *)ptr1)->PointerToRemap != *((PtrRemapStruct *)ptr2)->PointerToRemap)
	{
		result = -(*((PtrRemapStruct *)ptr1)->PointerToRemap < *((PtrRemapStruct *)ptr2)->PointerToRemap) | 1;
	}
	return result;
}
