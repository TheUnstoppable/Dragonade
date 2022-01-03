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
#ifndef TT_INCLUDE__TRANSLATEDBCLASS_H
#define TT_INCLUDE__TRANSLATEDBCLASS_H



#include "SaveLoadSubSystemClass.h"
#include "TDBObjClass.h"
#include "TDBCategoryClass.h"
#include "HashTemplateClass.h"



class TDBCategoryClass;



typedef DynamicVectorClass<TDBObjClass*> TDB_OBJ_LIST;
typedef DynamicVectorClass<TDBCategoryClass*> TDB_CATEGORY_LIST;



extern SHARED_API const wchar_t* STRING_NOT_FOUND;
extern SHARED_API const char* ENGLISH_STRING_NOT_FOUND;



#define TRANSLATE TranslateDBClass::Get_String



class TranslateDBClass :
	public SaveLoadSubSystemClass
{

public:
	
	enum
	{
		LANGID_ENGLISH = 0,
		LANGID_FRENCH,
		LANGID_GERMAN,
		LANGID_SPANISH,
		LANGID_CHINESE,
		LANGID_JAPANESE,
		LANGID_KOREAN
	};

	enum FILTER_OPT
	{
		FILTER_DISABLED = 0,
		FILTER_IF_EQUAL,
		FILTER_IF_NOT_EQUAL,
	};

private:

#if (TDBEDIT) || (DDBEDIT) || (W3DVIEWER)
	static TDB_OBJ_LIST m_ObjectList;
	static HashTemplateClass<StringClass, TDBObjClass*> m_ObjectHash;
	static TDB_CATEGORY_LIST m_CategoryList;
	static uint32 m_VersionNumber;
	static uint32 m_LanguageID;
	static bool IsSingleLanguageExport;
	static uint32 CategoryExportFilter;
	static FILTER_OPT FilterType;
	static uint32 FilterCategoryID;
#else
	SHARED_API static REF_DECL(TDB_OBJ_LIST, m_ObjectList);
	SHARED_API static REF_DECL(TT_NOOP(HashTemplateClass<StringClass, TDBObjClass*>), m_ObjectHash);
	static REF_DECL(TDB_CATEGORY_LIST, m_CategoryList);
	static REF_DECL(uint32, m_VersionNumber);
	SHARED_API static REF_DECL(uint32, m_LanguageID);
	static REF_DECL(bool, IsSingleLanguageExport);
	static REF_DECL(uint32, CategoryExportFilter);
	static REF_DECL(FILTER_OPT, FilterType);
	static REF_DECL(uint32, FilterCategoryID);
#endif

protected:

	enum
	{
		ID_MIN = 1000,
		ID_MAX = 999999
	};

	virtual bool Contains_Data() const;
	virtual bool Save(ChunkSaveClass& csave);
	virtual bool Load(ChunkLoadClass& cload);

	bool Load_Variables(ChunkLoadClass& cload);
	bool Load_Objects(ChunkLoadClass& cload);
	bool Load_Categories(ChunkLoadClass& cload);

	static void Validate_Data();
	static void Free_Objects();
	static void Free_Categories();

	static uint32 Find_Unique_ID();

public:

	SHARED_API static void Initialize();
	SHARED_API static void Shutdown();
	SHARED_API static uint32 Get_Version_Number();
	static void Update_Version();
	static bool Is_Loaded() { return (m_ObjectList.Count() > 0); }

	static void Import_Strings(const char* filename);
	static void	Import_C_Header(const char* filename);
	static void	Export_C_Header(const char* filename);
	static void	Export_Table(const char* filename);

	static const wchar_t* Get_String(uint32 id);
	static const wchar_t* Get_String(const char* id);
	static const char* Get_English_String(uint32 id);
	TT_INLINE static TDBObjClass* Find_Object(uint32 id);
	TT_INLINE static TDBObjClass* Find_Object(const char* id);

	static bool Add_Object(TDBObjClass* object);
	static bool Remove_Object(int index);
	static bool Clear_Object(int index);
	SHARED_API static void Remove_All();
	
	static int Get_Object_Count()
	{
		return m_ObjectList.Count();
	}
	static TDBObjClass* Get_Object(int index);
	static TDBObjClass* Get_First_Object(uint32 categoryId);
	static TDBObjClass* Get_Next_Object(uint32 categoryId, TDBObjClass* object);

	static int Get_Category_Count();
	static TDBCategoryClass* Get_Category(int index);
	static TDBCategoryClass* Find_Category(uint32 id);
	static TDBCategoryClass* Find_Category(const char* name);
	static TDBCategoryClass* Add_Category(const char* name);
	static bool Add_Category(TDBCategoryClass* category, bool assignId = true);
	static bool Remove_Category(int index);

	static void Set_Current_Language(int languageId) { m_LanguageID = languageId; }
	static uint32 Get_Current_Language() { return m_LanguageID; }
	
	static bool Is_Single_Language_Export_Enabled() { return IsSingleLanguageExport; }
	static void Enable_Single_Language_Export(bool value) { IsSingleLanguageExport = value; }
	
	static void Set_Export_Filter(FILTER_OPT filter, uint32 categoryId);


	TranslateDBClass() {}
	virtual ~TranslateDBClass() {}

	virtual uint32 Chunk_ID() const;
	virtual const char* Name() const { return "TranslateDBClass"; }

};



TT_INLINE const wchar_t* TranslateDBClass::Get_String(uint32 id)
{
	if (!id)
		return NULL;

	TDBObjClass* translation = Find_Object(id);
	if (translation)
		return translation->Get_String();
	
	return STRING_NOT_FOUND;
}


TT_INLINE const wchar_t* TranslateDBClass::Get_String(const char* id)
{
	if (!id)
		return NULL;

	TDBObjClass* translation = Find_Object(id);
	if (translation)
		return translation->Get_String();
	
	return STRING_NOT_FOUND;
}



inline const char* TranslateDBClass::Get_English_String (uint32 id)
{
	if (id == 0)
		return NULL;

	TDBObjClass* translation = Find_Object(id);
	if (translation)
		return translation->Get_English_String();
	
	return ENGLISH_STRING_NOT_FOUND;
}



TT_INLINE TDBObjClass* TranslateDBClass::Find_Object(const char* id)
{
	TT_ASSERT(Is_Loaded());

	StringClass lowercaseId(id, true);
	_strlwr(lowercaseId.Peek_Buffer());
	return m_ObjectHash.Get(lowercaseId, NULL);
}



TT_INLINE TDBObjClass* TranslateDBClass::Find_Object(uint32 id)
{
	TT_ASSERT(Is_Loaded());

	int index = id - ID_MIN;
	if (index >= 0 && index < m_ObjectList.Count())
	{
		TDBObjClass* object = m_ObjectList[index];
		//TT_ASSERT(object && object->Get_ID() == id);

		return object;
	}
	
	TT_ASSERT(index >= 0 && index < m_ObjectList.Count());
	return NULL;
}

extern TranslateDBClass _TheTranslateDB;

#endif