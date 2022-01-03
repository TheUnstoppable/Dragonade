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
#include "MemoryManager.h"

PUSH_MEMORY_MACROS
#undef new
#undef delete

void CheckMemoryManagerConfiguration(int32 config)
{
	if (config != MEMORYMANAGER_RELEASECONFIGURATION)
	{
		MessageBoxW(NULL, L"This is a friendly warning to remind you that running the release-mode memory manager with debug-mode executables is not recommended and may cause unexpected results.", L"MemoryManager.dll", MB_OK | MB_ICONWARNING);
	};
};

void SetThreadTrackingInformation(const char* file, unsigned int line, const char* function)
{
	UNUSED_VARIABLE(file);
	UNUSED_VARIABLE(line);
	UNUSED_VARIABLE(function);
};

void PushAllocationTag(const char* tag)
{
	UNUSED_VARIABLE(tag);
};

void PopAllocationTag()
{
};

DECLSPEC_RESTRICT void* AllocateMemory(size_t size)
{
	return FastAllocatorGeneral::Get_Allocator()->Allocate(size);
};

DECLSPEC_RESTRICT void* AllocateMemory(size_t size, AllocType type, const char* file, unsigned int line, const char* function)
{
	UNUSED_VARIABLE(type);
	UNUSED_VARIABLE(file);
	UNUSED_VARIABLE(line);
	UNUSED_VARIABLE(function);
	return FastAllocatorGeneral::Get_Allocator()->Allocate(size);
};

DECLSPEC_RESTRICT void* ReallocateMemory(void* memory, size_t new_size)
{
	return FastAllocatorGeneral::Get_Allocator()->Reallocate(memory, new_size);
};

DECLSPEC_RESTRICT void* ReallocateMemory(void* memory, size_t new_size, const char* file, unsigned int line, const char* function)
{
	UNUSED_VARIABLE(file);
	UNUSED_VARIABLE(line);
	UNUSED_VARIABLE(function);
	return FastAllocatorGeneral::Get_Allocator()->Reallocate(memory, new_size);
};

void FreeMemory(void* memory)
{
	FastAllocatorGeneral::Get_Allocator()->Free(memory);
};

void FreeMemory(void* memory, DeallocType type, const char* file, unsigned int line, const char* function)
{
	UNUSED_VARIABLE(type);
	UNUSED_VARIABLE(file);
	UNUSED_VARIABLE(line);
	UNUSED_VARIABLE(function);
	FastAllocatorGeneral::Get_Allocator()->Free(memory);
};

size_t GetMemorySize(void* memory)
{
	return FastAllocatorGeneral::Get_Allocator()->GetAllocationSize(memory);
};

void* operator new(size_t size)
{
	return FastAllocatorGeneral::Get_Allocator()->Allocate(size);
};

void* operator new(size_t size, const char* file, unsigned int line)
{
	UNUSED_VARIABLE(file);
	UNUSED_VARIABLE(line);
	return FastAllocatorGeneral::Get_Allocator()->Allocate(size);
};

void operator delete(void* memory)
{
	FastAllocatorGeneral::Get_Allocator()->Free(memory);
};

void operator delete(void* memory, const char* file, unsigned int line)
{
	UNUSED_VARIABLE(file);
	UNUSED_VARIABLE(line);
	// If you reach this breakpoint, you have a DLL that's expecting a debug version of this DLL. 
	FastAllocatorGeneral::Get_Allocator()->Free(memory);
};

void* operator new[](size_t size)
{
	return FastAllocatorGeneral::Get_Allocator()->Allocate(size);
};

void* operator new[](size_t size, const char* file, unsigned int line)
{
	UNUSED_VARIABLE(file);
	UNUSED_VARIABLE(line);
	// If you reach this breakpoint, you have a DLL that's expecting a debug version of this DLL. 
	return FastAllocatorGeneral::Get_Allocator()->Allocate(size);
};

void operator delete[](void* memory)
{
	FastAllocatorGeneral::Get_Allocator()->Free(memory);
};

void operator delete[](void* memory, const char* file, unsigned int line)
{
	UNUSED_VARIABLE(file);
	UNUSED_VARIABLE(line);
	// If you reach this breakpoint, you have a DLL that's expecting a debug version of this DLL. 
	FastAllocatorGeneral::Get_Allocator()->Free(memory);
};

POP_MEMORY_MACROS;