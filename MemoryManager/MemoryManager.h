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
#ifndef _MEMORYMANAGER_H_
#define _MEMORYMANAGER_H_

#define USING_NEW_OVERRIDE
//#define DECLARE_PLACEMENT_NEW

#include "platform.h"

#ifndef ALLOCTYPE_DEFINED
enum AllocType
{
	AllocType_Unknown,
	AllocType_New,
	AllocType_VectorNew,
	AllocType_Malloc,
	AllocType_Calloc,
	AllocType_Realloc,
	AllocType_Unvalidated,
};

enum DeallocType
{
	DeallocType_Unknown,
	DeallocType_Realloc,
	DeallocType_Delete,
	DeallocType_VectorDelete,
	DeallocType_Free,
	DeallocType_Unvalidated,
};
#endif

#define MEMORYMANAGER_DEBUGCONFIGURATION	0xbaadc0de
#define MEMORYMANAGER_RELEASECONFIGURATION	0xdeadbeef

#ifndef NDEBUG
#define MEMORYMANAGER_CONFIGURATION MEMORYMANAGER_DEBUGCONFIGURATION
#else
#define MEMORYMANAGER_CONFIGURATION MEMORYMANAGER_RELEASECONFIGURATION
#endif

void	CheckMemoryManagerConfiguration(int32 config);	// Call this function with MEMORYMANAGER_CONFIGURATION as the value
void	SetThreadTrackingInformation(const char* file, unsigned int line, const char* function);
void	PushAllocationTag(const char* tag);
void	PopAllocationTag();

class MemoryManagerTagger
{
public:
	MemoryManagerTagger(const char* tag) { PushAllocationTag(tag); };
	~MemoryManagerTagger() { PopAllocationTag(); };
private:
	/* Not implemented */
	MemoryManagerTagger(const MemoryManagerTagger&);
	MemoryManagerTagger operator=(const MemoryManagerTagger&);
};

#ifndef NDEBUG
#define MMGR_TAG(tag) MemoryManagerTagger mmgr_tag(tag);
#else
#define MMGR_TAG(tag) 
#endif

DECLSPEC_RESTRICT void* AllocateMemory(size_t size);
DECLSPEC_RESTRICT void* AllocateMemory(size_t size, AllocType type, const char* file, unsigned int line, const char* function);
DECLSPEC_RESTRICT void* ReallocateMemory(void* memory, size_t new_size);
DECLSPEC_RESTRICT void* ReallocateMemory(void* memory, size_t new_size, const char* file, unsigned int line, const char* function);
void	FreeMemory(void* memory);
void	FreeMemory(void* memory, DeallocType type, const char* file, unsigned int line, const char* function);
size_t	GetMemorySize(void* memory);

#pragma warning(suppress: 28251) //warning C28251: Inconsistent annotation for function: this instance has an error 
void*	operator new(size_t size);
void*	operator new(size_t size, const char* file, unsigned int line);
void	operator delete(void* memory);
void	operator delete(void* memory, const char* file, unsigned int line);

#pragma warning(suppress: 28251) //warning C28251: Inconsistent annotation for function: this instance has an error 
void*	operator new[](size_t size);
void*	operator new[](size_t size, const char* file, unsigned int line);
void	operator delete[](void* memory);
void	operator delete[](void* memory, const char* file, unsigned int line);

#ifdef DECLARE_PLACEMENT_NEW
inline void* operator new(size_t size, void *location) throw()
{
#ifndef NDEBUG
	SetThreadTrackingInformation(NULL, 0, NULL);
#endif
	return location;
};

inline void* operator new[](size_t size, void *location) throw()
{
#ifndef NDEBUG
	SetThreadTrackingInformation(NULL, 0, NULL);
#endif
	return location;
};
#endif

#define PUSH_MEMORY_MACROS	__pragma(push_macro("new")) __pragma(push_macro("delete")) 
#define POP_MEMORY_MACROS	__pragma(pop_macro("new")) __pragma(pop_macro("delete"))
	
#ifndef NDEBUG
#ifdef USING_NEW_OVERRIDE 
#define new			(SetThreadTrackingInformation(__FILE__, __LINE__, __FUNCTION__), false) ? (assert(false), NULL) : new
#define	delete		(SetThreadTrackingInformation(__FILE__, __LINE__, __FUNCTION__), false) ? assert(false) : delete
#endif
#endif

#endif