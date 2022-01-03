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
#ifndef TT_INCLUDE__STANDARD_H
#define TT_INCLUDE__STANDARD_H

// put includes from STL and other standard or third-party libraries here

#include <assert.h>
#include <errno.h>
#include <float.h>
#include <intrin.h>
#include <math.h>
#include <memory>
#include <new>
#include <process.h>
#include <shlwapi.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <tlhelp32.h>
#include <wchar.h>
//

// the order of these includes is essential
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <wininet.h>
#include <dbghelp.h>
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x800
#include <dinput.h>
#include <imm.h>

// #include <dlfcn.h>

#include "MemoryManager.h"

#endif
