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
#include "MemTracker.h"

DWORD TLSIndex = TLS_OUT_OF_INDEXES;
BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{ 
	UNUSED_VARIABLE(instance);
	UNUSED_VARIABLE(reserved);
	switch (reason) 
	{ 
	case DLL_PROCESS_ATTACH: 
		// Allocate a TLS index.
		TLSIndex = TlsAlloc();
		if (TLSIndex == TLS_OUT_OF_INDEXES)
		{
			// Uhoh.
			return FALSE;
		}
		// No break: Initialize the index for first thread.
	case DLL_THREAD_ATTACH: 
		// Initialize the TLS index for this thread.
		{
			void* tls_data = SystemAllocate(sizeof(MemoryTrackerThreadLocalInformation));
			memset(tls_data, 0x00, sizeof(MemoryTrackerThreadLocalInformation));
			if (tls_data != NULL)
			{
				TlsSetValue(TLSIndex, tls_data);
			};
		}
		break; 
	case DLL_THREAD_DETACH: 
		// Release the allocated memory for this thread.
		{
			void* tls_data = TlsGetValue(TLSIndex);
			if (tls_data != NULL)
			{
				SystemFree(tls_data);
			};
		}
		break; 
	case DLL_PROCESS_DETACH: 
		// Release the allocated memory for this thread.
		void* tls_data = TlsGetValue(TLSIndex);
		if (tls_data != NULL)
		{
			SystemFree(tls_data);
		};

		// Release the TLS index.
		TlsFree(TLSIndex); 
		break; 
	} 

	return TRUE; 
};