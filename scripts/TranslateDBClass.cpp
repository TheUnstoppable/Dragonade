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
#include "General.h"
#include "TranslateDBClass.h"
#include "PersistFactoryClass.h"
#include "SaveLoadSystemClass.h"
#include "RawFileClass.h"

const wchar_t* STRING_NOT_FOUND = L"TDBERR";
const char* ENGLISH_STRING_NOT_FOUND = "TDBERR";

#if (SHARED_EXPORTS) || (TDBEDIT)
TranslateDBClass _TheTranslateDB;
#endif

#if (TDBEDIT) || (DDBEDIT)
TDB_OBJ_LIST TranslateDBClass::m_ObjectList;
HashTemplateClass<StringClass,TDBObjClass*> TranslateDBClass::m_ObjectHash;
TDB_CATEGORY_LIST TranslateDBClass::m_CategoryList;
uint32 TranslateDBClass::m_VersionNumber = 0;
uint32 TranslateDBClass::m_LanguageID = 0;
bool TranslateDBClass::IsSingleLanguageExport = false;
TranslateDBClass::FILTER_OPT TranslateDBClass::FilterType = FILTER_DISABLED;
uint32 TranslateDBClass::FilterCategoryID = 0;
#else
REF_DEF2(TDB_OBJ_LIST, TranslateDBClass::m_ObjectList, 0x0085FD08, 0x0085EEF0);
REF_DEF2(TT_NOOP(HashTemplateClass<StringClass, TDBObjClass*>), TranslateDBClass::m_ObjectHash, 0x0085FCD0, 0x0085EEB8);
REF_DEF2(TDB_CATEGORY_LIST, TranslateDBClass::m_CategoryList, 0x0085FCF0, 0x0085EED8);
REF_DEF2(uint32, TranslateDBClass::m_VersionNumber, 0x00815864, 0x00814A3C);
REF_DEF2(uint32, TranslateDBClass::m_LanguageID, 0x0085FD24, 0x0085EF0C);
REF_DEF2(bool, TranslateDBClass::IsSingleLanguageExport, 0x0085FD28, 0x0085EF10);
REF_DEF2(TranslateDBClass::FILTER_OPT, TranslateDBClass::FilterType, 0x0085FD2C, 0x0085EF14);
REF_DEF2(uint32, TranslateDBClass::FilterCategoryID, 0x00815868, 0x00814A40);
#endif

void TranslateDBClass::Initialize()
{
	m_ObjectList.Set_Growth_Step(1000);
}

uint32 TranslateDBClass::Get_Version_Number()
{
	return m_VersionNumber;
}

void TranslateDBClass::Shutdown()
{
	Free_Objects();
	Free_Categories();
}

void TranslateDBClass::Free_Categories()
{
	for (int i = 0;i < m_CategoryList.Count();i++)
	{
		delete m_CategoryList[i];
	}
	m_CategoryList.Delete_All();
}

void TranslateDBClass::Free_Objects()
{
	for (int i = 0;i < m_ObjectList.Count();i++)
	{
		delete m_ObjectList[i];
	}
	m_ObjectList.Delete_All();
	m_ObjectHash.Remove_All();
}

uint32 TranslateDBClass::Chunk_ID() const
{
	return 0x90000;
}

bool TranslateDBClass::Contains_Data() const
{
	return true;
}

bool TranslateDBClass::Save(ChunkSaveClass &csave)
{
	csave.Begin_Chunk(0x7141200);
	WRITE_MICRO_CHUNK(csave,1,m_VersionNumber);
	WRITE_MICRO_CHUNK(csave,2,m_LanguageID);
	csave.End_Chunk();
	csave.Begin_Chunk(0x7141202);
	for (int i = 0;i < m_CategoryList.Count();i++)
	{
		TDBCategoryClass *cat = m_CategoryList[i];
		if (cat)
		{
			csave.Begin_Chunk(cat->Get_Factory().Chunk_ID());
			cat->Get_Factory().Save(csave,cat);
			csave.End_Chunk();
		}
	}
	csave.End_Chunk();
	csave.Begin_Chunk(0x7141201);
	for (int i = 0;i < m_ObjectList.Count();i++)
	{
		TDBObjClass *obj = m_ObjectList[i];
		if (obj)
		{
			bool b = (obj->Get_Category_ID() == FilterCategoryID);
			if ((FilterType == FILTER_DISABLED) || ((FilterType == FILTER_IF_EQUAL) && !b) || ((FilterType == FILTER_IF_NOT_EQUAL) && b))
			{
				csave.Begin_Chunk(obj->Get_Factory().Chunk_ID());
				obj->Get_Factory().Save(csave,obj);
				csave.End_Chunk();
			}
		}
	}
	csave.End_Chunk();
	return true;
}

bool TranslateDBClass::Load(ChunkLoadClass &cload)
{
	bool b = true;
	bool b2 = true;
	Free_Objects();
	Free_Categories();
	while (cload.Open_Chunk())
	{
		switch(cload.Cur_Chunk_ID()) 
		{
			case 0x7141201:
				b2 = Load_Objects(cload);
				break;
			case 0x7141202:
				b2 = Load_Categories(cload);
				break;
			case 0x7141200:
				b2 = Load_Variables(cload);
				break;
		}
		b = (b2 & b) != 0;
		cload.Close_Chunk();
	}
	Validate_Data();
	return b;
}

bool TranslateDBClass::Load_Categories(ChunkLoadClass &cload)
{
	while (cload.Open_Chunk())
	{
		PersistFactoryClass *factory = SaveLoadSystemClass::Find_Persist_Factory(cload.Cur_Chunk_ID());
		if (factory)
		{
			TDBCategoryClass *obj = (TDBCategoryClass *)factory->Load(cload);
			if (obj)
			{
				Add_Category(obj);
			}
		}
		cload.Close_Chunk();
	}
	return true;
}

bool TranslateDBClass::Load_Objects(ChunkLoadClass &cload)
{
	while (cload.Open_Chunk())
	{
		PersistFactoryClass *factory = SaveLoadSystemClass::Find_Persist_Factory(cload.Cur_Chunk_ID());
		if (factory)
		{
			TDBObjClass *obj = (TDBObjClass *)factory->Load(cload);
			if (obj)
			{
				Add_Object(obj);
			}
		}
		cload.Close_Chunk();
	}
	return true;
}

bool TranslateDBClass::Load_Variables(ChunkLoadClass &cload)
{
	while (cload.Open_Micro_Chunk ())
	{
		switch (cload.Cur_Micro_Chunk_ID ())
		{
			READ_MICRO_CHUNK (cload, 1,	m_VersionNumber);
			READ_MICRO_CHUNK (cload, 2,	m_LanguageID);
			default:
				break;
		}
		cload.Close_Micro_Chunk();
	}
	m_VersionNumber = 145;
	return true;
}

void TranslateDBClass::Validate_Data()
{
	if (!m_CategoryList.Count())
	{
		TDBCategoryClass *t = new TDBCategoryClass();
		t->Set_Name("Default");
		m_CategoryList.Add(t);
	}

#ifndef TDBEDIT
	// add a few custom translation entries if they don't already exist
	if (!Find_Object("IDS_MENU_HIGH_RES_SHADOWS"))
	{
		TDBObjClass* obj = new TDBObjClass();
		obj->Set_ID_Desc("IDS_MENU_HIGH_RES_SHADOWS");
		obj->Set_English_String("High Resolution Shadows");
		obj->Set_String(TranslateDBClass::Get_Current_Language(), L"High Resolution Shadows");
		Add_Object(obj);
	}

	TDBObjClass* texture_filter = Find_Object("IDS_TEXTURE_FILTER");
	if (texture_filter)
	{
		char* en_string = (char*)texture_filter->Get_English_String().Peek_Buffer();
		if (char* en_star = strrchr(en_string, '*')) *en_star = ' ';

		wchar_t* lang_string = (wchar_t*)texture_filter->Get_String().Peek_Buffer();
		if (wchar_t* lang_star = wcsrchr(lang_string, L'*')) *lang_star = L' ';
	}

	if (!Find_Object("IDS_ANTIALIAS_MODE"))
	{
		TDBObjClass* obj = new TDBObjClass();
		obj->Set_ID_Desc("IDS_ANTIALIAS_MODE");
		obj->Set_English_String("Anti-aliasing Mode:");
		obj->Set_String(TranslateDBClass::Get_Current_Language(), L"Anti-aliasing Mode:");
		Add_Object(obj);
	}
	if (!Find_Object("IDS_STRUCT_GUARD_TOWER"))
	{
		TDBObjClass* obj = new TDBObjClass();
		obj->Set_ID_Desc("IDS_STRUCT_GUARD_TOWER");
		obj->Set_English_String("Guard Tower");
		obj->Set_String(TranslateDBClass::Get_Current_Language(), L"Guard Tower");
		Add_Object(obj);
	}
#endif

}

TDBObjClass *TranslateDBClass::Get_Object(int index)
{
	if (index >= 0)
	{
		if (index < m_ObjectList.Count())
		{
			return m_ObjectList[index];
		}
	}
	return 0;
}

uint32 TranslateDBClass::Find_Unique_ID()
{
	for (int i = 0;i < m_ObjectList.Count();i++)
	{
		if (!m_ObjectList[i])
		{
			return i + 1000;
		}
	}
	return m_ObjectList.Count() + 1000;
}

TDBCategoryClass *TranslateDBClass::Add_Category(const char *name)
{
	TDBCategoryClass *t = new TDBCategoryClass();
	t->Set_Name(name);
	Add_Category(t,true);
	return 0;
}

bool TranslateDBClass::Add_Category(TDBCategoryClass *category, bool assignId)
{
	if (category)
	{
		if (assignId)
		{
			if (!category->Get_ID())
			{
				unsigned int id = 1;
				for (int i = 0;i < m_CategoryList.Count();i++)
				{
					if (id < m_CategoryList[i]->Get_ID() + 1)
					{
						id = m_CategoryList[i]->Get_ID() + 1;
					}
				}
				category->Set_ID(id);
			}
		}
		m_CategoryList.Add(category);
		return true;
	}
	return false;
}

bool TranslateDBClass::Remove_Category(int index)
{
	if (index >= 0)
	{
		if (index < m_CategoryList.Count())
		{
			TDBCategoryClass *cat = m_CategoryList[index];
			if (cat)
			{
				TDBObjClass *obj;
				for (obj = Get_First_Object(cat->Get_ID());obj;obj = Get_Next_Object(cat->Get_ID(),obj));
				{
					delete obj;
				}
				delete cat;
			}
			m_CategoryList.Delete(index);
			return true;
		}
	}
	return false;
}

bool TranslateDBClass::Remove_Object(int index)
{
	if ((index >= 0) && (index < m_ObjectList.Count()))
	{
		TDBObjClass *object = m_ObjectList[index];
		if (object)
		{
			StringClass str = object->Get_ID_Desc();
			_strlwr(str.Peek_Buffer());
			m_ObjectHash.Remove(str);
			delete object;
		}
		m_ObjectList[index] = 0;
		return true;
	}
	return false;
}

bool TranslateDBClass::Clear_Object(int index)
{
	if ((index >= 0) && (index < m_ObjectList.Count()))
	{
		TDBObjClass *object = m_ObjectList[index];
		if (object)
		{
			StringClass str = object->Get_ID_Desc();
			_strlwr(str.Peek_Buffer());
			m_ObjectHash.Remove(str);
		}
		m_ObjectList[index] = 0;
		return true;
	}
	return false;
}

bool TranslateDBClass::Add_Object(TDBObjClass *object)
{
	if (object)
	{
		if (object->Get_ID() <= 999)
		{
			object->Set_ID(Find_Unique_ID());
		}
		int id = object->Get_ID() - 1000;
		while (m_ObjectList.Count() <= id)
		{
			TDBObjClass *obj = 0;
			m_ObjectList.Add(obj);
		}
		m_ObjectList[id] = object;
		StringClass str = object->Get_ID_Desc();
		_strlwr(str.Peek_Buffer());
		m_ObjectHash.Insert(str,object);
		return true;
	}
	return false;
}

void TranslateDBClass::Remove_All()
{
	Free_Objects();
}

void TranslateDBClass::Update_Version()
{
	m_VersionNumber++;
}

int TranslateDBClass::Get_Category_Count()
{
	return m_CategoryList.Count();
}

TDBCategoryClass *TranslateDBClass::Get_Category(int index)
{
	if (index >= 0)
	{
		if (index < m_CategoryList.Count())
		{
			return m_CategoryList[index];
		}
	}
	return 0;
}

TDBCategoryClass *TranslateDBClass::Find_Category(uint32 id)
{
	for (int i = 0;i < m_CategoryList.Count();i++)
	{
		if (m_CategoryList[i]->Get_ID() == id)
		{
			return m_CategoryList[i];
		}
	}
	return 0;
}

TDBCategoryClass *TranslateDBClass::Find_Category(const char *name)
{
	for (int i = 0;i < m_CategoryList.Count();i++)
	{
		if (!_stricmp(m_CategoryList[i]->Get_Name(),name))
		{
			return m_CategoryList[i];
		}
	}
	return 0;
}

TDBObjClass *TranslateDBClass::Get_First_Object(uint32 categoryId)
{
	for (int i = 0;i < m_ObjectList.Count();i++)
	{
		TDBObjClass *obj = m_ObjectList[i];
		if (obj && obj->Get_Category_ID() == categoryId)
		{
			return obj;
		}
	}
	return 0;
}

TDBObjClass *TranslateDBClass::Get_Next_Object(uint32 categoryId, TDBObjClass *object)
{
	for (int i = object->Get_ID() - 999;i < m_ObjectList.Count();i++)
	{
		TDBObjClass *obj = m_ObjectList[i];
		if (obj && obj->Get_Category_ID() == categoryId)
		{
			return obj;
		}
	}
	return 0;
}

void TranslateDBClass::Export_C_Header(const char *filename)
{
	HANDLE h = CreateFileA(filename,0x40000000,0,0,2,0,0);
	if (h != INVALID_HANDLE_VALUE)
	{
		TextFileClass f;
		f.Attach(h,1);
		StringClass str("#if defined(_MSC_VER)");
		f.Write_Line(str);
		str = "#pragma once";
		f.Write_Line(str);
		str = "#endif";
		f.Write_Line(str);
		str = "";
		f.Write_Line(str);
		str = "#ifndef __STRING_IDS_H";
		f.Write_Line(str);
		str = "#define __STRING_IDS_H";
		f.Write_Line(str);
		str = "";
		f.Write_Line(str);
		str = "";
		str.Format("#define STRINGS_VER\t\t%d",m_VersionNumber);
		f.Write_Line(str);
		str = "";
		f.Write_Line(str);
		str = "// TRANSLATEDB: Begin ID Block";
		f.Write_Line(str);
		for (int i = 0;i < m_ObjectList.Count();i++)
		{
			TDBObjClass *obj = m_ObjectList[i];
			if (obj)
			{
				str = "";
				str.Format("#define %s\t\t%d",obj->Get_ID_Desc(),obj->Get_ID());
				f.Write_Line(str);
			}
		}
		str = "// TRANSLATEDB: End ID Block";
		f.Write_Line(str);
		str = "";
		f.Write_Line(str);
		str = "#endif //__STRING_IDS_H";
		f.Write_Line(str);
	}
}

void TranslateDBClass::Export_Table(const char *filename)
{
	/* NB: Using fopen instead of CreateFileA and TextFileClass as this file must be in
	UTF-8 to preserve wchar_t string arrays from the TranslatedStrings array. */
	FILE* f = fopen(filename, "w,ccs=UTF-8");
	if(f)
	{
		/* Export will use the following format; 
		[category_id,category_name]
			string_id,string_name,sound_id,anim_name
			0=lang 1 string
			1=lang 2 string
			...
			n=lang n string

			string_id,string_name,sound_id,anim_name
			0=lang 1 string
			1=lang 2 string
			...
			n=lang n string
		*/


		/* Iterate through categories */
		for (int i = 0; i < m_CategoryList.Count(); i++)
		{
			TDBCategoryClass *cat = m_CategoryList[i];
			if (cat)
			{
				fwprintf_s (f, L"[%u,%hs]\r\n",cat->Get_ID(), (const char *)cat->Get_Name());

				/* Iterate through strings in class */
				TDBObjClass *obj;
				for (obj = TranslateDBClass::Get_First_Object(cat->Get_ID());obj;obj = TranslateDBClass::Get_Next_Object(cat->Get_ID(),obj))
				{
					fwprintf_s (f, L"%u,%hs,%u,%hs\r\n", obj->Get_ID(), (const char *)obj->Get_ID_Desc(), obj->Get_Sound_ID(), (const char *)obj->Get_Animation_Name());

					/* Print out all language strings, even those with no entries, as users may
					wish to enter values for those while editing the export file. Note: assumes
					last language ID is KOREAN, if more are added this will need changing. */
					for (uint32 j=0; j <= LANGID_KOREAN; j++)
						fwprintf_s (f, L"%u=%s\r\n",j,(const wchar_t *)obj->Get_String(j));
				}
			}
		}
	}
}


void TranslateDBClass::Import_Strings(const char *filename)
{
	TDBCategoryClass *currentCategory = NULL;
	TDBObjClass *currentObj = NULL;

	/* NB: Using fopen instead of CreateFileA and TextFileClass as this file should
	be using UTF-8 encoding */
	FILE* f = fopen(filename, "r,ccs=UTF-8");
	if(f)
	{
		/* Create variables - we initialise input to 1024 characters, but this
		will automatically be extended if required */
		wchar_t* input = new wchar_t[1024];
		uint32 input_max = 1024;
		
		/* Read a line from the file */
		while ( fgetws(input, input_max, f) )
		{
			/* If length of input == input_max-1 and input[input_max-2] is NOT \n then
			we have a line which is longer than our current maximum. Extend the array
			by 1024 and read some more content */
			while ( wcslen(input) == input_max-1 && input[wcslen(input)-2] != L'\n' )
			{
				wchar_t *oldInput = input;
				input_max += 1024;
				input = new wchar_t[input_max];
				memcpy(input,oldInput,sizeof(wchar_t)*(input_max-1024));
				delete [] oldInput;

				/* Read more content - NB: Read from input_max-1025 because we need to
				overwrite the old null terminator. Hence we can actually read 1025
				additional characters, including the new null terminator. */
				fgetws((input+(input_max-1025)), 1025, f);
			}


			/* Nullify trailing whitespace or newline characters */
			while ( wcslen(input) > 0 && (input[wcslen(input)-1] == L' ' || input[wcslen(input)-1] == L'\r' || input[wcslen(input)-1] == L'\n') )
				input[wcslen(input)-1] = L'\0';




			/* Process line... */
			if ( input[0] == L'[' && wcschr(input,L',') != NULL )
			{
				// Category
				uint32 id;
				StringClass name;

				// We don't need the trailing ] from the category name so nullify it
				if ( input[wcslen(input)-1] == L']' )
					input[wcslen(input)-1] = '\0';
				else
					continue;		// Invalid category line, not terminated with ]

				// Read ID and name
				id = _wtoi(input+1);
				name.Format("%ls",wcschr(input,L',')+1);
				
				/* If category name is NOT default then create category. */
				currentCategory = new TDBCategoryClass();
				currentCategory->Set_ID(id);
				currentCategory->Set_Name(name);
				Add_Category(currentCategory);

				// Unset current object
				currentObj = NULL;
			}




			else if (currentCategory && ( wcschr(input,L',') != NULL && (wcschr(input,L'=') == NULL || wcschr(input,L',') < wcschr(input,L'=')) ))
			{
				// String
				uint32 id;
				StringClass name;
				uint32 sound_id;
				StringClass animation;

				// Set ID
				id = _wtoi(input);

				// Extract name (nullify final , so format doesnt overrun name)
				wchar_t *itemStart = wcschr(input,L',')+1;
				wchar_t *itemEnd = wcschr(itemStart,L',');
				if ( itemEnd == NULL ) continue;		//Invalid String line, not enough parameters
				*itemEnd = '\0';
				name.Format("%ls",itemStart);

				// Set sound ID
				itemStart = itemEnd+1;
				sound_id = _wtoi(itemStart);

				// Extract animation name (all remaining content on line, could be blank)
				itemStart = wcschr(itemStart,L',')+1;
				if ( itemStart == NULL ) continue;		//Invalid String line, not enough parameters
				animation.Format("%ls",itemStart);
				
				/* Create string object */
				currentObj = new TDBObjClass();
				currentObj->Set_ID(id);
				currentObj->Set_ID_Desc(name);
				currentObj->Set_Sound_ID(sound_id);
				currentObj->Set_Animation_Name(animation);
				currentObj->Set_Category_ID (currentCategory->Get_ID());
				Add_Object(currentObj);
			}




			else if (currentObj && ( wcschr(input,L'=') != NULL && (wcschr(input,L',') == NULL || wcschr(input,L'=') < wcschr(input,L',')) ))
			{
				// String Translation
				uint32 lang_id;
				WideStringClass translation;

				// Set language ID
				lang_id = _wtoi(input);

				// Extract translation name (all remaining content on line after the =, could be blank)
				translation.Format(L"%s", wcschr(input,L'=')+1);
				
				/* Register translation with current string object */
				if ( wcslen(translation) > 0 )
					currentObj->Set_String(lang_id, translation);
			}
		}

		delete [] input;
	}
}
