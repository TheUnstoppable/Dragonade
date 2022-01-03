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
#ifndef TT_INCLUDE__SAVELOADSYSTEMCLASS_H
#define TT_INCLUDE__SAVELOADSYSTEMCLASS_H
#include "PointerRemapClass.h"
#include "SaveLoadSubSystemClass.h"
class PersistFactoryClass;

class SaveLoadSystemClass
{

public:

#if (TDBEDIT) || (DDBEDIT) || (W3DVIEWER)
   static PointerRemapClass PointerRemapper;
   static PersistFactoryClass *_FactoryListHead;
   static SaveLoadSubSystemClass *_SubSystemListHead;
#else
   static REF_DECL(PointerRemapClass, PointerRemapper);
   static REF_DECL(PersistFactoryClass *, _FactoryListHead);
   static REF_DECL(SaveLoadSubSystemClass *, _SubSystemListHead);
#endif

   static SaveLoadSubSystemClass* Find_Sub_System                   (uint32 id);
   static bool                    Load                              (ChunkLoadClass& chunkLoader, bool);
   static bool                    Save                              (ChunkSaveClass& chunkSaver, SaveLoadSubSystemClass &subSystem);
   static void                    Post_Load_Processing              (void(*)());
   static void                    Register_Pointer                  (void*, void*);
   static void                    Register_Post_Load_Callback       (PostLoadableClass*);
   static void                    Request_Pointer_Remap             (void**);
   static void                    Register_Sub_System               (SaveLoadSubSystemClass *);
   static void                    Unregister_Sub_System             (SaveLoadSubSystemClass *);
   static void                    Link_Sub_System                   (SaveLoadSubSystemClass *);
   static void                    Unlink_Sub_System                 (SaveLoadSubSystemClass *);
   static void                    Register_Persist_Factory          (PersistFactoryClass *);
   static void                    Unregister_Persist_Factory        (PersistFactoryClass *);
   static PersistFactoryClass *   Find_Persist_Factory              (uint32);
   static void                    Link_Factory                      (PersistFactoryClass *);
   static void                    Unlink_Factory                    (PersistFactoryClass *);
   static void                    Request_Ref_Counted_Pointer_Remap (RefCountClass **);
};


#endif
