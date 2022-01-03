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
#include "platform.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "FastAllocator.h"
#include "MemTracker.h"
#include "MemoryManager.h"

PUSH_MEMORY_MACROS
#undef new
#undef delete

void CheckMemoryManagerConfiguration(int32 config)
{
	UNUSED_VARIABLE(config);
};

void SetThreadTrackingInformation(const char* file, unsigned int line, const char* function)
{
	MemoryTracker::SetThreadLocalInformation(file, function, line);
};

void PushAllocationTag(const char* tag)
{
	MemoryTracker::PushAllocationTag(tag);
};

void PopAllocationTag()
{
	MemoryTracker::PopAllocationTag();
};

DECLSPEC_RESTRICT void* AllocateMemory(size_t size)
{
	return MemoryTracker::Allocate(size, AllocType_Malloc);
};

DECLSPEC_RESTRICT void* AllocateMemory(size_t size, AllocType type, const char* file, unsigned int line, const char* function)
{
	return MemoryTracker::Allocate(size, type, file, function, line);
};

DECLSPEC_RESTRICT void* ReallocateMemory(void* memory, size_t new_size)
{
	return MemoryTracker::Reallocate(memory, new_size);
};

DECLSPEC_RESTRICT void* ReallocateMemory(void* memory, size_t new_size, const char* file, unsigned int line, const char* function)
{
	return MemoryTracker::Reallocate(memory, new_size, file, function, line);
};

void FreeMemory(void* memory)
{
	MemoryTracker::Free(memory, DeallocType_Free);
};

void FreeMemory(void* memory, DeallocType type, const char* file, unsigned int line, const char* function)
{
	MemoryTracker::Free(memory, type, file, function, line);
};

size_t GetMemorySize(void* memory)
{
	return MemoryTracker::GetAllocationSize(memory);
};

void* operator new(size_t size)
{
	MemoryTrackerThreadLocalInformation* info = MemoryTracker::GetThreadLocalInformation();
	return MemoryTracker::Allocate(size, AllocType_New, info->CurrentSourceFile, info->CurrentSourceFunction, info->CurrentSourceLine);
};

void* operator new(size_t size, const char* file, unsigned int line)
{
	return MemoryTracker::Allocate(size, AllocType_New, file, "(Unknown Function)", line);
};

void operator delete(void* memory)
{
	MemoryTrackerThreadLocalInformation* info = MemoryTracker::GetThreadLocalInformation();	
	MemoryTracker::Free(memory, DeallocType_Delete, info->CurrentSourceFile, info->CurrentSourceFunction, info->CurrentSourceLine);
};

void operator delete(void* memory, const char* file, unsigned int line)
{
	MemoryTracker::Free(memory, DeallocType_Delete, file, "(Unknown Function)", line);
};

void* operator new[](size_t size)
{
	MemoryTrackerThreadLocalInformation* info = MemoryTracker::GetThreadLocalInformation();
	return MemoryTracker::Allocate(size, AllocType_VectorNew, info->CurrentSourceFile, info->CurrentSourceFunction, info->CurrentSourceLine);
};

void* operator new[](size_t size, const char* file, unsigned int line)
{
	return MemoryTracker::Allocate(size, AllocType_VectorNew, file, "(Unknown Function)", line);
};

void operator delete[](void* memory)
{
	MemoryTrackerThreadLocalInformation* info = MemoryTracker::GetThreadLocalInformation();	
	MemoryTracker::Free(memory, DeallocType_VectorDelete, info->CurrentSourceFile, info->CurrentSourceFunction, info->CurrentSourceLine);
};

void operator delete[](void* memory, const char* file, unsigned int line)
{
	MemoryTracker::Free(memory, DeallocType_VectorDelete, file, "(Unknown Function)", line);
};

POP_MEMORY_MACROS;