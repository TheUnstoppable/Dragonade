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
#ifndef TT_INCLUDE_DEFINITION_H
#define TT_INCLUDE_DEFINITION_H
#include "Editable.h"
class FileClass;
class DefinitionClass : public EditableClass {
public:
	DefinitionClass (void);
	virtual ~DefinitionClass (void);
	virtual uint32						Get_Class_ID (void) const = 0;
	virtual uint32						Get_ID (void) const;
	virtual void						Set_ID (uint32 id);
	virtual PersistClass *			Create (void) const = 0;
	virtual const char *				Get_Name (void) const;
	virtual void						Set_Name (const char *new_name);	
	virtual bool						Is_Valid_Config (StringClass &message);
	virtual bool						Save (ChunkSaveClass &csave);
	virtual bool						Load (ChunkLoadClass &cload);
	uint32								Get_User_Data (void) const		{ return m_GenericUserData; }
	void									Set_User_Data (uint32 data)	{ m_GenericUserData = data; }
	bool									Is_Save_Enabled (void) const	{ return m_SaveEnabled; }
	void									Enable_Save (bool onoff)		{ m_SaveEnabled = onoff; }
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
	virtual void                        DumpPhys (FileClass &file);
#endif
	DECLARE_EDITABLE(DefinitionClass, EditableClass);
protected:
	int											m_DefinitionMgrLink; // 0008
private:
	bool											Save_Variables (ChunkSaveClass &csave);
	bool											Load_Variables (ChunkLoadClass &cload);
	StringClass				m_Name; // 000C
	uint32					m_ID; // 0010
	uint32					m_GenericUserData; // 0014
	bool						m_SaveEnabled; // 0018
	friend class DefinitionMgrClass;
}; // 001C

inline DefinitionClass::DefinitionClass (void)
	:	m_ID (0),
		m_SaveEnabled (true),
		m_DefinitionMgrLink (-1)
{
	return ;
}

inline DefinitionClass::~DefinitionClass (void)
{
	return ;
}

inline const char *DefinitionClass::Get_Name (void) const
{
	return m_Name;
}

inline void DefinitionClass::Set_Name (const char *new_name)
{
	m_Name = new_name;
	return ;
}

inline uint32 DefinitionClass::Get_ID (void) const
{
	return m_ID;
}

inline bool DefinitionClass::Is_Valid_Config (StringClass &message)
{
	return true;
}

#endif