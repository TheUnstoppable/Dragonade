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
#include "FastAllocator.h"

#pragma warning(push)
#pragma warning(disable: 4073) // warning C4073: initializers put in library initialization area
#pragma init_seg(lib) // leet hax to insure our threads and friends are initialized before everybody else
#pragma warning(pop)
static FastAllocatorGeneral* __general_allocator = new FastAllocatorGeneral();

FastAllocatorGeneral* FastAllocatorGeneral::Get_Allocator()
{
	return __general_allocator;	
};

// Please modify these next two functions to call a system allocator
DECLSPEC_RESTRICT void* SystemAllocate(size_t size)
{
	void* memory = malloc(size);
	// If you hit this assert, you've run out of memory. This is bad.
	assert(memory);
	return memory;
};

void SystemFree(void* memory)
{
	free(memory);
};
