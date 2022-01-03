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
#ifndef TT_INCLUDE_DEFINITIONFACTORYMGRCLASS_H
#define TT_INCLUDE_DEFINITIONFACTORYMGRCLASS_H
class DefinitionFactoryClass;
class DefinitionFactoryMgrClass {
public:
	static void Register_Factory(DefinitionFactoryClass* factory);
	static void Unregister_Factory(DefinitionFactoryClass* factory);
	static void Link_Factory(DefinitionFactoryClass *factory);
	static void Unlink_Factory(DefinitionFactoryClass *factory);
	static DefinitionFactoryClass *Find_Factory(uint32 id);
	static DefinitionFactoryClass *Get_First(uint32 id);
	static DefinitionFactoryClass *Get_Next(DefinitionFactoryClass *factory,uint32 id);
#if (TDBEDIT) || (DDBEDIT) || (W3DVIEWER)
	static DefinitionFactoryClass *_FactoryListHead;
#else
	static REF_DECL(DefinitionFactoryClass *, _FactoryListHead);
#endif
};

#endif