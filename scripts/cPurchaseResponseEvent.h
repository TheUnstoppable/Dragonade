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
#ifndef TT_INCLUDE__CPURCHASERESPONSEEVENT_H
#define TT_INCLUDE__CPURCHASERESPONSEEVENT_H

#include "cNetEvent.h"


class cPurchaseResponseEvent : public cNetEvent
{

private:

   sint32 clientId;        // 1716
   sint32 reply;           // 1720


public:

   cPurchaseResponseEvent();

   virtual unsigned int Get_Network_Class_ID () const;
   virtual void   Import_Creation      (BitStreamClass& oStream);
   virtual void   Export_Creation      (BitStreamClass& oStream);
   virtual void   Act                  ();

   void Init (sint32 reply, sint32 clientId);

};


#endif
