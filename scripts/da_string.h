

#ifndef INCLUDE_DASTRING
#define INCLUDE_DASTRING

#include "engine_string.h"

inline StringClass ToString(int Num) {
	char Buffer[64];
	_snprintf(Buffer,64,"%d",Num);
	return Buffer;
}

inline StringClass ToString(float Num) {
	char Buffer[64];
	_snprintf(Buffer,64,"%f",Num);
	return Buffer;
}

inline StringClass ToString(unsigned int Num) {
	char Buffer[64];
	_snprintf(Buffer,64,"%u",Num);
	return Buffer;
}

inline StringClass StringFormat(const char *Format,...) {
	char Buffer[526];
	Buffer[525] = '\0'; 
	va_list arg_list; 
	va_start(arg_list,Format); 
	vsnprintf(Buffer,526,Format,arg_list); 
	va_end(arg_list);
	return Buffer;
}

inline WideStringClass WideStringFormat(const wchar_t *Format,...) {
	wchar_t Buffer[526];
	Buffer[525] = '\0'; 
	va_list arg_list; 
	va_start(arg_list,Format); 
	vswprintf(Buffer,526,Format,arg_list); 
	va_end(arg_list);
	return Buffer;
}

inline char *A_Or_An(const char *String) {
	if (String && (*String == 'A' || *String == 'a' || *String == 'E' || *String == 'e' || *String == 'I' || *String == 'i' || *String == 'O' || *String == 'o' || *String == 'U' || *String == 'u')) {
		return "An";
	}
	return "A";
}

inline wchar_t *A_Or_An(const wchar_t *String) {
	if (String && (*String == 'A' || *String == 'a' || *String == 'E' || *String == 'e' || *String == 'I' || *String == 'i' || *String == 'O' || *String == 'o' || *String == 'U' || *String == 'u')) {
		return L"An";
	}
	return L"A";
}

inline char *a_or_an(const char *String) {
	if (String && (*String == 'A' || *String == 'a' || *String == 'E' || *String == 'e' || *String == 'I' || *String == 'i' || *String == 'O' || *String == 'o' || *String == 'U' || *String == 'u')) {
		return "an";
	}
	return "a";
}

inline wchar_t *a_or_an(const wchar_t *String) {
	if (String && (*String == 'A' || *String == 'a' || *String == 'E' || *String == 'e' || *String == 'I' || *String == 'i' || *String == 'O' || *String == 'o' || *String == 'U' || *String == 'u')) {
		return L"an";
	}
	return L"a";
}

inline StringClass A_Or_An_Prepend(const char *String) {
	return StringClass(A_Or_An(String)) + " " + String;
}

inline WideStringClass A_Or_An_Prepend(const wchar_t *String) {
	return WideStringClass(A_Or_An(String)) + L" " + String;
}

inline StringClass a_or_an_Prepend(const char *String) {
	return StringClass(a_or_an(String)) + " " + String;
}

inline WideStringClass a_or_an_Prepend(const wchar_t *String) {
	return WideStringClass(a_or_an(String)) + L" " + String;
}

inline StringClass Make_Possessive(const StringClass &String) {
	if (!String.Is_Empty() && (String[String.Get_Length()] == 'S' || String[String.Get_Length()] == 's')) {
		return String + "\'";
	}
	return String + "\'s'";
}

inline WideStringClass Make_Possessive(const WideStringClass &String) {
	if (!String.Is_Empty() && (String[String.Get_Length()] == 'S' || String[String.Get_Length()] == 's')) {
		return String + L"\'";
	}
	return String + L"\'s";
}

inline bool Is_Numeric(const char *String) {
	for (;*String != '\0';String++) {
		if (*String < 48 || *String > 57) {
			return false;
		}
	}
	return true;
}

inline bool Is_Number(const char *String) {
	for (;*String != '\0';String++) {
		if ((*String < 48 || *String > 57) && (*String != '-' && *String != '.')) {
			return false;
		}
	}
	return true;
}

inline bool Is_Alpha(const char *String) {
	for (;*String != '\0';String++) {
		if (*String < 65 || *String > 122 || (*String < 97 && *String > 90)) {
			return false;
		}
	}
	return true;
}

DA_API unsigned int Get_Hash(const char *String);

#endif