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
#ifndef _PLATFORM_WIN32_H_
#define _PLATFORM_WIN32_H_

#ifndef WIN32
#error This file can only be included for Win32/64 builds for now.
#endif

#include <intrin.h>
#include <assert.h>

#ifndef PLATFORM_NOTYPES
typedef __int16				int16;
typedef unsigned __int16	uint16;
typedef __int32				int32;
typedef unsigned __int32	uint32;
typedef __int64				int64;
typedef unsigned __int64	uint64;
typedef unsigned int		uint;
typedef void*				handle_t;
#endif

#if defined (_M_IX86)
#define PLATFORM_MEMORY_ALIGNMENT 8
#elif defined (_M_X64)
#define PLATFORM_MEMORY_ALIGNMENT 16
#endif

inline void YieldThread() { _mm_pause(); };
#define DECLSPEC_ALIGN(alignment)	__declspec(align(alignment))
#define DECLSPEC_RESTRICT			__declspec(restrict)

#ifdef NDEBUG
#define ASSUME(_Expression) __assume(_Expression);
#else
#define ASSUME(_Expression) assert(_Expression);
#endif

// A method for preventing the unused variable warning
template <typename T> inline void UNUSED_VARIABLE(const T&) {};

#endif
