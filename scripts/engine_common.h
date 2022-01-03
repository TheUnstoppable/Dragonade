/*	Renegade Scripts.dll
	Copyright 2013 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#ifndef SCRIPTS_INCLUDE__ENGINE_COMMON_H
#define SCRIPTS_INCLUDE__ENGINE_COMMON_H

// (float) is merely there to make sure that it uses the float overloads to send the version over network.
// and to make sure we do not forget the f suffix on the version number.
#define TT_VERSION_MAGIC '!TT!'
#define TT_VERSION ((float)4.7f)

#define SAFE_DELETE_ARRAY(p)	{ delete[] p; p = NULL; }
#define SAFE_DELETE(p)			{ delete p; p = NULL; }

template <typename T> TT_INLINE void SafeRelease(T& IUnk)
{
	if (IUnk)
	{
		IUnk->Release();
		IUnk = 0;
	}
}; 

template <typename T, typename T2> TT_INLINE void RefPtrSet(T& dest, T2& source)
{
	if (source) source->AddRef();
	if (dest)	dest->Release();
	dest = source;
};

void SCRIPTS_API Console_Input(const char *Input); //triggers console input (i.e. passes the string in as though it was typed into the console)
void SCRIPTS_API Console_Output(const char *Output,...); //prints stuff to the console, works like printf
#endif
