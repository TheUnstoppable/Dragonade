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
#include "engine_string.h"
#include "engine_threading.h"

#pragma warning(disable: 4073) //warning C4073: initializers put in library initialization area - That's EXACTLY why I put that pragma in...
#pragma init_seg(lib) // Move this files static initializers up a level
#pragma warning(default: 4073)
#if (SHARED_EXPORTS || EXTERNAL)
char __declspec(thread) StringClass::TempStrings[MAX_TEMP_STRING][MAX_TEMP_BYTES] = {};
unsigned int __declspec(thread) StringClass::FreeTempStrings = 0xFF;
char __declspec(thread) WideStringClass::TempStrings[MAX_TEMP_STRING][MAX_TEMP_BYTES] = {};
unsigned int __declspec(thread) WideStringClass::FreeTempStrings = 0xF;
#endif
#ifdef EXTERNAL
char StringClass::m_NullChar = 0;
char *StringClass::m_EmptyString = &m_NullChar;
wchar_t WideStringClass::m_NullChar = 0;
wchar_t *WideStringClass::m_EmptyString = &m_NullChar;
#else
#ifdef SHARED_EXPORTS
REF_DEF3(char *, StringClass::m_EmptyString, 0x00809D38, 0x00808F10, 0x0084CAE0);
REF_DEF3(char, StringClass::m_NullChar, 0x0083EC0C, 0x0083DDF4, 0x00874688);
REF_DEF3(wchar_t *, WideStringClass::m_EmptyString, 0x00809D3C, 0x00808F14, 0x0084DE74);
REF_DEF3(wchar_t, WideStringClass::m_NullChar, 0x0083F43C, 0x0083E624, 0x00874FB8);
#endif
#endif

#if (SHARED_EXPORTS || EXTERNAL)
int __cdecl StringClass::Format(const char* format,...)
{
	va_list arg_list;
	va_start(arg_list,format);
	char temp_buffer[512];
	int x = vsnprintf(temp_buffer,512,format,arg_list);
	*this = (const char *)temp_buffer;
	va_end(arg_list);
	return x;
}

int __cdecl StringClass::Format_Args(const char* format,const va_list& arg_list)
{
	char temp_buffer[512];
	int x = vsnprintf(temp_buffer,512,format,arg_list);
	*this = (const char *)temp_buffer;
	return x;
}

void StringClass::Get_String(int length, bool is_temp)
{	
	if (!is_temp && !length) Set_Buffer_And_Allocated_Length(m_EmptyString, 0);
	else if (is_temp && length < MAX_TEMP_LEN && FreeTempStrings)
	{
		uint32 index = 0;
		BitScanForward((DWORD*)&index, FreeTempStrings); // Find the first free temp string
		FreeTempStrings &= ~(1 << index); // Remove it from the free pool
		char* buffer = TempStrings[index] + sizeof(_HEADER);
		Set_Buffer_And_Allocated_Length(buffer, MAX_TEMP_LEN);
	}
	else if (length > 0) Set_Buffer_And_Allocated_Length(Allocate_Buffer(length), length);
	else Free_String();
}

void StringClass::Resize(int new_len)
{
	if (new_len > Get_Allocated_Length())
	{
		char *x = Allocate_Buffer(new_len);
		strcpy(x,m_Buffer);
		Free_String();
		Set_Buffer_And_Allocated_Length(x,new_len);
	}
}

void StringClass::Uninitialised_Grow(int new_len)
{
	if (new_len > Get_Allocated_Length())
	{
		char *x = Allocate_Buffer(new_len);
		Free_String();
		Set_Buffer_And_Allocated_Length(x,new_len);
	}
	Store_Length(0);
}

void StringClass::Free_String()
{
	if (m_Buffer == m_EmptyString) return;

	ptrdiff_t buffer_base = intptr_t(m_Buffer) - sizeof(_HEADER);
	ptrdiff_t diff = buffer_base - intptr_t(TempStrings[0]);

	if (diff >= 0 && diff < MAX_TEMP_BYTES * MAX_TEMP_STRING)
	{
		// It was a temp string, let's get the index and cast Undead.
		ptrdiff_t index = diff / MAX_TEMP_BYTES;
		m_Buffer[0] = m_NullChar;
		FreeTempStrings |= 1 << index;
	}
	else
	{
		char* buffer = (char*)buffer_base;
		delete[] buffer;
	}

	m_Buffer = m_EmptyString;
}

void StringClass::Release_Resources()
{
}

int StringClass::Replace(const char* search, const char* replace, bool bCaseSensitive, int maxCount)
{
	if (m_Buffer == m_EmptyString) return 0;

  int nReplacements = 0;

  char* newstring = NULL;             // The modified string, NULL until we make a change
  int newstringlen = Get_Length()+1;  // The length of the modified string
  const char* searchPtr = m_Buffer;   // The starting point for the next search, always lastreplacement+1

  // Figure out lengths in advance to avoid doing it repeatedly
  int searchlen = strlen(search);
  int replacelen = strlen(replace);

  while ( NULL != searchPtr && (-1 == maxCount || nReplacements < maxCount) )
  {
    // Find the next instance of the search string
    const char* foundPtr = ( bCaseSensitive ) ? stristr(searchPtr,search) : strstr(searchPtr,search);
    searchPtr = NULL;

    if ( NULL != foundPtr )
    {
      // Figure out the 0-based index of the first character to be replaced
      int replaceindex = (int)foundPtr - (int)((NULL==newstring)?m_Buffer:newstring);

      // Allocate a new string to fit the replacement data if necessary
      if ( searchlen != replacelen || NULL == newstring )
      {
        // Cache old data so we can clean up memory
        int oldnewstringlen = newstringlen;
        char* oldnewstring = newstring;

        newstringlen = oldnewstringlen + (replacelen-searchlen);
        newstring = new char[newstringlen];

        // Copy characters preceeding the string to be replaced
        memcpy(newstring, (NULL==oldnewstring)?m_Buffer:oldnewstring, replaceindex);

        // Copy characters following the string to be replaced
        int postsearchindex = replaceindex+searchlen;
        int postreplaceindex = replaceindex+replacelen;
        memcpy(newstring+postreplaceindex, ((NULL==oldnewstring)?m_Buffer:oldnewstring)+postsearchindex, oldnewstringlen-postsearchindex);

        delete [] oldnewstring;
      }
      
      // Copy in the replacement string in the location of the located search string
      memcpy(newstring+replaceindex, replace, replacelen);

      // Update the search pointer
      searchPtr = newstring + replaceindex + replacelen;

      nReplacements++;
    }
  }

  // Update the string with the modified version, if any
  if ( NULL != newstring )
  {
    *this = newstring;
    delete [] newstring;
  }

  return nReplacements;
}

bool StringClass::Copy_Wide(const wchar_t *str)
{
	if (str)
	{
		mbstate_t ps;
		memset(&ps,0,sizeof(ps));
		int len = (int)wcsrtombs(0,&str,0,&ps);
		if (len > 0)
		{
			Uninitialised_Grow(len+1);
			wcsrtombs(m_Buffer,&str,len,&ps);
			m_Buffer[len] = 0;
			Store_Length(len);
			return true;
		}
		return false;
	}
	return false;
}

int __cdecl WideStringClass::Format(const wchar_t* format,...)
{
	if (format == NULL)
	{
		return 0;
	}
	va_list arg_list;
	va_start(arg_list,format);
	wchar_t temp_buffer[512];
	int x = _vsnwprintf(temp_buffer,512,format,arg_list);
	*this = temp_buffer;
	va_end(arg_list);
	return x;
}

int __cdecl WideStringClass::Format_Args(const wchar_t* format,const va_list& arg_list)
{
	if (format == NULL)
	{
		return 0;
	}
	wchar_t temp_buffer[512];
	int x = _vsnwprintf(temp_buffer,512,format,arg_list);
	*this = temp_buffer;
	return x;
}

void WideStringClass::Get_String(int length,bool is_temp)
{
	if (!is_temp && !length) Set_Buffer_And_Allocated_Length(m_EmptyString, 0);
	else if (is_temp && length < MAX_TEMP_LEN && FreeTempStrings)
	{
		uint32 index = 0;
		BitScanForward((DWORD*)&index, FreeTempStrings); // Find the first free temp string
		FreeTempStrings &= ~(1 << index); // Remove it from the free pool
		wchar_t* buffer = (wchar_t*)(TempStrings[index] + sizeof(_HEADER));
		Set_Buffer_And_Allocated_Length(buffer, MAX_TEMP_LEN);
	}
	else if (length > 0) Set_Buffer_And_Allocated_Length(Allocate_Buffer(length), length);
	else Free_String();
}

void WideStringClass::Resize(int new_len)
{
	if (new_len > Get_Allocated_Length())
	{
		wchar_t *x = Allocate_Buffer(new_len);
		wcscpy(x,m_Buffer);
		Free_String();
		Set_Buffer_And_Allocated_Length(x,new_len);
	}
}

void WideStringClass::Uninitialised_Grow(int new_len)
{
	if (new_len > Get_Allocated_Length())
	{
		wchar_t *x = Allocate_Buffer(new_len);
		Free_String();
		Set_Buffer_And_Allocated_Length(x,new_len);
	}
	Store_Length(0);
}

void WideStringClass::Free_String()
{
	if (m_Buffer == m_EmptyString) return;

	ptrdiff_t buffer_base = intptr_t(m_Buffer) - sizeof(_HEADER);
	ptrdiff_t diff = buffer_base - intptr_t(TempStrings[0]);

	if (diff >= 0 && diff < MAX_TEMP_BYTES * MAX_TEMP_STRING)
	{
		// It was a temp string, let's get the index and cast Undead.
		ptrdiff_t index = diff / MAX_TEMP_BYTES;
		m_Buffer[0] = m_NullChar;
		FreeTempStrings |= 1 << index;
	}
	else
	{
		char* buffer = (char*)buffer_base;
		delete[] buffer;
	}

	m_Buffer = m_EmptyString;
}

void WideStringClass::Release_Resources()
{
}

bool WideStringClass::Convert_From(const char *str)
{
	if (str)
	{
		mbstate_t ps;
		memset(&ps,0,sizeof(ps));
#pragma warning(suppress: 6387) // header entry of mbsrtowcs is incorrect, NULL is a valid first entry
		int len = (int)mbsrtowcs(NULL,&str,0,&ps);
		if (len >= 0)
		{
			Uninitialised_Grow(len+1);
			mbsrtowcs(m_Buffer,&str,len,&ps);
			m_Buffer[len] = 0;
			Store_Length(len);
			return true;
		}
	}
	return false;
}

bool WideStringClass::Is_ANSI()
{
	if (m_Buffer)
	{
		for (int i = 0;m_Buffer[i] != 0;i++)
		{
			unsigned short value = m_Buffer[i];
			if (value > 255)
			{
				return false;
			}
		}
	}
	return true;
}
WideStringClass WideStringClass::Substring(int start, int length) const
{
	TT_ASSERT(start + length <= Get_Length());

	WideStringClass result;
	result.Uninitialised_Grow(length+1);
	result.Store_Length(length);
	memcpy(result.m_Buffer, m_Buffer + start, length * sizeof(wchar_t));
	result.m_Buffer[length] = L'\0';

	return result;
}

void WideStringClass::RemoveSubstring(int start, int length)
{
	if (length > 0)
	{
		int oldLength = Get_Length();
		int newLength = oldLength - length;
		TT_ASSERT(start + length <= oldLength);

		memmove(m_Buffer + start, m_Buffer + start + length, (newLength - start) * sizeof(wchar_t));
		m_Buffer[newLength] = L'\0';
		Store_Length(newLength);
	}
}

void WideStringClass::ReplaceSubstring(int start, int length, const WideStringClass& substring)
{
	int substringLength = substring.Get_Length();
	int oldLength = Get_Length();
	int newLength = oldLength - length + substringLength;
	TT_ASSERT(start + length <= oldLength);

	if (substringLength > length)
		Resize(newLength + 1);
	
	memmove(m_Buffer + start + substringLength, m_Buffer + start + length, (oldLength - start - length) * sizeof(wchar_t));
	memcpy(m_Buffer + start, substring.m_Buffer, substringLength * sizeof(wchar_t));
	m_Buffer[newLength] = L'\0';
	Store_Length(newLength);
}
#endif
SCRIPTS_API const wchar_t *CharToWideChar(const char *str)
{
	int length = (int)strlen(str);
	wchar_t *text = new wchar_t[length+1];
	mbstowcs(text,str,length+1);
	return text;
}

SCRIPTS_API const char *WideCharToChar(const wchar_t *wcs)
{
	if (!wcs)
	{
		char *c = new char[2];
		c[0] = 0;
		c[1] = 0;
		return c;
	}
	int length = (int)wcslen(wcs);
	char *text = new char[length+1];
	wcstombs(text,wcs,length+1);
	return text;
}

SCRIPTS_API char *newstr(const char *str)
{
	size_t len = strlen(str)+1;
	char *s = new char[len];
	memcpy(s,str,len);
	return s;	
};
SCRIPTS_API wchar_t *newwcs(const wchar_t *str)
{
	size_t len = wcslen(str)+2;
	wchar_t *s = new wchar_t[len];
	memcpy(s,str,len*2);
	return s;
};
SCRIPTS_API char *strtrim(char *v)
{
	if (v)
	{
		char *r = v;
		while (*r > 0 && *r < 0x21)
			r++;
		strcpy(v,r);
		r = v + strlen(v);
		while (r > v && r[-1] > 0 && r[-1] < 0x21)
			r--;
		*r = 0;
	}
	return v;
}

SCRIPTS_API char *strrtrim(char *s) 
{
	char *t, *tt;

	TT_ASSERT(s != NULL);

	for (tt = t = s; *t != '\0'; ++t)
		if (!isspace(*(unsigned char *)t))
			tt = t+1;
	*tt = '\0';

	return s;
}

SCRIPTS_API const char *stristr(const char *str, const char *substr){
	while (*str){
		if (_strnicmp(str, substr, strlen(substr)) == 0)
			return str;		
		str++;
	}
	return NULL;
}

SCRIPTS_API const wchar_t *wcsistr(const wchar_t *str, const wchar_t *substr){
	if (!*str)
		return NULL;
	while (*str){
		if (_wcsnicmp(str, substr, wcslen(substr)) == 0)
			return str;
		str++;
	}
	return NULL;
}
