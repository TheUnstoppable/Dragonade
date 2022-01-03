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

#pragma warning(disable: 4073) //warning C4073: initializers put in library initialization area - That's EXACTLY why I put that pragma in...
#pragma init_seg(lib) // Move this files static initializers up a level
#pragma warning(default: 4073)
#include "SysTimeClass.h"
extern "C" DECLSPEC_IMPORT DWORD WINAPI timeGetTime(void);
#if (TDBEDIT) || (DDBEDIT) || (W3DVIEWER)
bool SysTimeClass::_is_init;
SysTimeClass SysTimeClass::SystemTime;
#else
REF_DEF3(bool, SysTimeClass::_is_init, 0x0081C644, 0x0081B824, 0x0085A544);
REF_DEF3(SysTimeClass, SysTimeClass::SystemTime, 0x0083F450, 0x0083E638, 0x008851B0);
#endif
uint32 SysTimeClass::Get()
{

   if (!_is_init)
   {
      _is_init = true;
      this->Reset();
   }

   unsigned int u = timeGetTime();
   if (u <= uTimeInit)
   {
       return uTimeInitNeg + u;
   }
   else
   {
       return u - uTimeInit;
   }
}



void SysTimeClass::Reset()
{
   this->uTimeInit = timeGetTime();
   this->uTimeInitNeg = (uint32)-(sint32)this->uTimeInit;
}
