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
#ifndef TT_INCLUDE_TWIDDLERCLASS_H
#define TT_INCLUDE_TWIDDLERCLASS_H
#include "Definition.h"
class TwiddlerClass : public DefinitionClass {
public:
	TwiddlerClass();
	~TwiddlerClass();
	virtual DefinitionClass *Twiddle() const;
	PersistClass *Create() const;
	const PersistFactoryClass &Get_Factory() const;
	const DynamicVectorClass<int>& Get_Definition_List() { return m_DefinitionList; }
	bool Save(ChunkSaveClass &csave);
	bool Load(ChunkLoadClass &cload);
	bool Save_Variables(ChunkSaveClass &csave);
	bool Load_Variables(ChunkLoadClass &cload);
	uint32 Get_Class_ID() const {return 57344;}
	virtual uint32 Get_Indirect_Class_ID() const {return m_IndirectClassID;}
	virtual void Set_Indirect_Class_ID(uint32 class_id) {m_IndirectClassID = class_id;}
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE( TwiddlerClass, DefinitionClass );
private:
	uint32 m_IndirectClassID;
	DynamicVectorClass<int> m_DefinitionList;
	friend class PresetDump;
};

#endif
