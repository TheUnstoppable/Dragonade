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
#ifndef TT_INCLUDE__CNETEVENT_H
#define TT_INCLUDE__CNETEVENT_H



#include "engine_common.h"
#include "engine_string.h"
#include "NetworkObjectClass.h"

class cNetEvent :
	public NetworkObjectClass
{

public:

   virtual ~cNetEvent();
   virtual unsigned int Get_Network_Class_ID() const = 0;
   virtual void Import_Creation (BitStreamClass& oStream);
   virtual void Export_Creation (BitStreamClass& oStream);
   virtual void Delete          ();
   virtual void Act             () = 0;

   void Init();
   SCRIPTS_API void Set_Object_Dirty_Bit_For_Revision(uint revision,DIRTY_BIT dirty_bit, bool onoff);

}; // 06B4



#endif
