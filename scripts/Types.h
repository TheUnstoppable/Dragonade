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
#ifndef TT_INCLUDE__TYPES_H
#define TT_INCLUDE__TYPES_H

// fixed-size types

#if defined(_MSC_VER)    // Microsoft Visual Studio

   typedef unsigned __int64 uint64;
   typedef   signed __int64 sint64;
   typedef unsigned __int32 uint32;
   typedef   signed __int32 sint32;
   typedef unsigned __int16 uint16;
   typedef   signed __int16 sint16;
   typedef unsigned __int8  uint8;
   typedef   signed __int8  sint8;

#elif defined(__GNUC__)  // GNU C++

   typedef unsigned long long uint64;
   typedef   signed long long sint64;
   typedef unsigned int       uint32;
   typedef   signed int       sint32;
   typedef unsigned short     uint16;
   typedef   signed short     sint16;
   typedef unsigned char      uint8;
   typedef   signed char      sint8;

#else
#   error Unknown compiler. Please specify fixed-size integer types in 'Types.h'.
#endif


typedef uint8  byte;

// we compile 32 bit only
typedef sint32 sint;
typedef uint32 uint;


#endif
