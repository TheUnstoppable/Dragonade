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
#ifndef TT_INCLUDE_PARAMETER_H
#define TT_INCLUDE_PARAMETER_H
#include "Engine_String.h"
#include "Engine_Vector.h"
class DefParameterClass;
class FileClass;
class ParameterClass
{
public:
	typedef enum
	{
		TYPE_INT					= 0,
		TYPE_FLOAT,
		TYPE_STRING,
		TYPE_VECTOR3,
		TYPE_MATRIX3D,
		TYPE_BOOL,
		TYPE_TRANSITION,
		TYPE_MODELDEFINITIONID,
		TYPE_FILENAME,
		TYPE_ENUM,
		TYPE_GAMEOBJDEFINITIONID,
		TYPE_SCRIPT,
		TYPE_SOUND_FILENAME,
		TYPE_ANGLE,
		TYPE_WEAPONOBJDEFINITIONID,
		TYPE_AMMOOBJDEFINITIONID,
		TYPE_SOUNDDEFINITIONID,
		TYPE_COLOR,
		TYPE_PHYSDEFINITIONID,
		TYPE_EXPLOSIONDEFINITIONID,
		TYPE_DEFINITIONIDLIST,
		TYPE_ZONE,
		TYPE_FILENAMELIST,
		TYPE_SEPARATOR,
		TYPE_GENERICDEFINITIONID,
		TYPE_SCRIPTLIST,
		TYPE_VECTOR2,
		TYPE_RECT,
		TYPE_STRINGSDB_ID
	}	Type;
	ParameterClass (void);
	ParameterClass (const ParameterClass &src);
	virtual ~ParameterClass (void);
	const ParameterClass &	operator= (const ParameterClass &src);
	virtual bool				operator== (const ParameterClass &src) = 0;
	virtual DefParameterClass *	As_DefParameterClass (void)	{ return NULL; }
	virtual Type				Get_Type (void) const = 0;
	virtual bool				Is_Type (Type type) const { return false; }
	virtual bool				Is_Modifed (void) const				{ return IsModified; }
	virtual void				Set_Modified (bool onoff = true)	{ IsModified = onoff; }
	virtual const char *		Get_Name (void) const;
	virtual void				Set_Name (const char *new_name);
	virtual const char *		Get_Units_Name (void) const;
	virtual void				Set_Units_Name (const char *units_name);
	virtual void				Copy_Value (const ParameterClass &src) { };
#ifdef DDBEDIT
	virtual void                Dump(FileClass &file) = 0;
#endif
	static ParameterClass *	Construct (ParameterClass::Type type, void *data, const char *param_name);
private:
	bool				IsModified;
	const char *	m_Name;
	StringClass		m_UnitsName;
};
inline
ParameterClass::ParameterClass (void)
	:	m_Name (NULL),
		IsModified (false)
{
	return ;
}
inline
ParameterClass::ParameterClass (const ParameterClass &src)
	:	m_Name (NULL),
		IsModified (false)
{
	(*this) = src;
	return ;
}
inline
ParameterClass::~ParameterClass (void)
{
	Set_Name (NULL);
	return ;
}
inline const ParameterClass &
ParameterClass::operator= (const ParameterClass &src)
{
	IsModified = src.IsModified;
	Set_Name (src.m_Name);
	return *this;
}
inline const char *
ParameterClass::Get_Name (void) const
{
	return m_Name;
}
inline void
ParameterClass::Set_Name (const char *new_name)
{
	if (m_Name != NULL) {
		free ((void *)m_Name);
		m_Name = NULL;
	}

	if (new_name != NULL) {
		m_Name = _strdup (new_name);
	}

	return ;
}
inline const char *
ParameterClass::Get_Units_Name (void) const
{
	return m_UnitsName;
}
inline void
ParameterClass::Set_Units_Name (const char *new_name)
{
	m_UnitsName = new_name;
	return ;
}
class ParameterListClass : public DynamicVectorClass<ParameterClass *>
{
public:
	~ParameterListClass (void);
	void			Add (void *data, const char *param_name, ParameterClass::Type type);
	void			Add (ParameterClass *parameter);
protected:
	void			Free_Parameters (void);
private:
	DynamicVectorClass<ParameterClass *>		m_Parameters;
};
inline
ParameterListClass::~ParameterListClass (void)
{
	Free_Parameters ();
	return ;
}
inline void
ParameterListClass::Add (void *data, const char *param_name, ParameterClass::Type type)
{
	ParameterClass *new_param = ParameterClass::Construct (type, data, param_name);
	if (new_param != NULL) {
		DynamicVectorClass<ParameterClass *>::Add (new_param);
	}
}
inline void
ParameterListClass::Add (ParameterClass *new_param)
{
	if (new_param != NULL) {
		DynamicVectorClass<ParameterClass *>::Add (new_param);
	}
}
inline void
ParameterListClass::Free_Parameters (void)
{
	for (int index = 0; index < Count (); index ++) {		
		ParameterClass *param = Vector[index];
		if (param != NULL) {
			delete param;
		}
	}
	m_Parameters.Delete_All ();
}
#ifdef	PARAM_EDITING_ON
	#define DECLARE_EDITABLE(_class, _parent)										\
	ParameterListClass plist_##_class;												\
	virtual int _class::Get_Parameter_Count(void) const						\
	{																							\
		return plist_##_class.Count () + _parent::Get_Parameter_Count ();	\
	}																							\
	virtual ParameterClass *_class::Lock_Parameter(int i)						\
	{																							\
		if (i < _parent::Get_Parameter_Count()) {									\
			return _parent::Lock_Parameter (i);										\
		}																						\
		return plist_##_class[i - _parent::Get_Parameter_Count()];			\
	}																							
#else
	#define DECLARE_EDITABLE(_class, _parent)
#endif //PARAM_EDITING_ON

#endif