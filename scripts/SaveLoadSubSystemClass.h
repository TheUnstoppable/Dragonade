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
#ifndef TT_INCLUDE__SAVELOADSUBSYSTEMCLASS_H
#define TT_INCLUDE__SAVELOADSUBSYSTEMCLASS_H

#include "PostLoadableClass.h"
#include "scripts.h"
#include "engine_vector.h"
#include "engine_io.h"

class SaveLoadSubSystemClass : public PostLoadableClass
{

public:

   SaveLoadSubSystemClass *NextSubSystem;
   SaveLoadSubSystemClass();
   ~SaveLoadSubSystemClass();
   virtual uint32      Chunk_ID      () const = 0;
   virtual bool        Contains_Data () const;
   virtual bool        Save          (ChunkSaveClass& oSave) = 0;
   virtual bool        Load          (ChunkLoadClass& oLoad) = 0;
   virtual const char* Name          () const = 0;

}; // size: 12


#endif
