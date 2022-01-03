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

#include "engine_threading.h"

void SetThreadName(DWORD dwThreadID, LPCSTR szThreadName)
{
dwThreadID; 
szThreadName;
#ifdef DEBUG
	THREADNAME_INFO info;
	{
		info.dwType = 0x1000;
		info.szName = szThreadName;
		info.dwThreadID = dwThreadID;
		info.dwFlags = 0;
	}
	__try
	{
		RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info );
	}
#pragma warning(suppress: 6312) //warning C6312: Possible infinite loop: use of the constant EXCEPTION_CONTINUE_EXECUTION in the exception-filter expression of a try-except
	__except (EXCEPTION_CONTINUE_EXECUTION)
#pragma warning(suppress: 6322) //warning C6322: empty _except block
	{
	}
#endif
}
