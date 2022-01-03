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
#include "ScriptableGameObj.h"

ReferencerClass::~ReferencerClass()
{
   *this = 0;
}



void ReferencerClass::On_Post_Load()
{
   if (ReferenceTarget)
   {
      ScriptableGameObj* tempObject = ReferenceTarget->ReferenceData;
      ReferenceTarget = 0;

      *this = tempObject;
   }
}



ReferencerClass& ReferencerClass::operator =
   (const ReferencerClass& referencer)
{
   *this = referencer.Get_Ptr();
   return *this;
}



ReferencerClass& ReferencerClass::operator =
   (ScriptableGameObj* object)
{
   if (ReferenceTarget)
      if (ReferenceTarget->ReferencerListHead == this)
      {
         ReferenceTarget->ReferencerListHead = this->TargetReferencerListNext;
         ReferenceTarget = 0;
         TargetReferencerListNext   = 0;
      }
      else
         for (ReferencerClass* referencer = ReferenceTarget->ReferencerListHead; referencer; referencer = referencer->TargetReferencerListNext)
            if (referencer->TargetReferencerListNext == this)
            {
               referencer->TargetReferencerListNext = this->TargetReferencerListNext;

               ReferenceTarget = 0;
               TargetReferencerListNext   = 0;

               break;
            }

   if (object)
   {
      ReferenceTarget  = object;
      TargetReferencerListNext    = object->ReferencerListHead;
      object->ReferencerListHead = this;
   }

   return *this;
}

ReferencerClass::operator ScriptableGameObj* () const
{
   return Get_Ptr();
}
