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
#ifndef TT_INCLUDE__DEFINES_H
#define TT_INCLUDE__DEFINES_H

// put global defines here

#include "Types.h"
#include "stdarg.h"
#if (WWCONFIG) || (TDBEDIT) || (W3DSHADER) || (W3DLIB_EXPORTS) || (W3DMESHMENDER) || (W3DDEPENDS) || (W3DMAPPER) || (ACHASH) || (PACKAGEEDITOR) || (FIXPLANES) || (MERGELOD) || (MIXCHECK) || (DDBEDIT) || (MAKEMIX) || (ALTMAT) || (W3DVIEWER)
#define EXTERNAL 1
#endif

#pragma warning(disable: 4100) // unreferenced formal parameter
#pragma warning(disable: 4201) // nonstandard extension used : nameless struct/union
#pragma warning(disable: 4505) // unreferenced local function has been removed
#pragma warning(disable: 6509) // warning c6509: Return used on precondition

//class needs to have dll-interface to used by clients of class. Except that it doesn't. 
//If it did, the linker would complain.
#pragma warning(disable:4251)

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0601
#define _USE_MATH_DEFINES

// breakpoint
#if defined(_M_IX86)
#define TT_INTERRUPT  { __asm { __asm int 3 } }
#elif defined(_M_AMD64)
#define TT_INTERRUPT  { __debugbreak(); }
#endif

// assert that is not removed in release mode
#define TT_RELEASE_ASSERT(expression) { __analysis_assume(expression); if (!(expression)) TT_INTERRUPT; }

// assert
#ifdef DEBUG
#	define TT_ASSERT(expression) \
   {                             \
	   __analysis_assume(expression); \
      if (!(expression))         \
         TT_INTERRUPT;            \
   }
#else
#	define TT_ASSERT(expression) \
   {                             \
	   __analysis_assume(expression); \
   }
#endif

// assumption optimization
#ifdef DEBUG
#	define TT_ASSUME(x) TT_ASSERT(x)
#elif defined(NDEBUG) && defined(_MSC_VER)
#	define TT_ASSUME(x) __assume(x)
#else
#	define TT_ASSUME(x)
#endif

// unreachable code
#	ifdef DEBUG
#		define TT_UNREACHABLE TT_INTERRUPT
#	else
#		define TT_UNREACHABLE __assume(false);
#	endif

#define TT_UNIMPLEMENTED TT_INTERRUPT
#define TT_UNTESTED TT_INTERRUPT

// deprecated code
#define TT_DEPRECATED(x) __declspec(deprecated(x))

// inlined function calls
#if defined(NDEBUG) && defined(_MSC_VER)
#	define TT_INLINE __forceinline
#else
#	define TT_INLINE inline
#endif

// countof for static array
template <typename T, size_t N>
char ( &_ArraySizeHelper( T (&array)[N] ))[N];

#define countof( array ) (sizeof( _ArraySizeHelper( array ) ))

// This can be used as a workaround for when a template type in a class is needed.
// something like MACRO(Class<Param1, Param2>) will make the preprocessor think two
// parameters are supplied ("Class<Param1" and "Param2>").
#define TT_NOOP(...) __VA_ARGS__

// unknown type
#define UNK TT_DEPRECATED("Unknown type") uint32
#define UNKB TT_DEPRECATED("Unknown type") uint8
#define UNKADDRESS 0x00000000

// to communicate with Renegade
#ifndef EXTERNAL

#ifdef SHARED_EXPORTS
#   define SHARED_API __declspec(dllexport)
#else
#   define SHARED_API __declspec(dllimport)
#endif

#ifdef DRAGONADE //DA
#   define SCRIPTS_API __declspec(dllexport)
#else
#   if SSGMPLUGIN || DAPLUGIN
#   define SCRIPTS_API __declspec(dllimport)
#   else
#       define SCRIPTS_API
#   endif
#endif

SCRIPTS_API void* HookupAT3(void* a, void* b, void* c, void* patch_start); //DA
#define RENEGADE_FUNCTION  __declspec(naked) //DA
#ifndef TTLE_EXPORTS //DA
#define AT2(client, server) AT3(client, server, 0)
#endif
#define AT3(client, server, leveledit)  \
{                                       \
    __asm AT3PatchStart:                \
    __asm push ecx                      \
    __asm push edx                      \
    __asm push offset AT3PatchStart     \
    __asm push leveledit                \
    __asm push server                   \
    __asm push client                   \
    __asm call HookupAT3                \
    __asm add esp, 16                   \
    __asm pop edx                       \
    __asm pop ecx                       \
    __asm jmp eax                       \
}
SCRIPTS_API void InitEngine(); //DA
SCRIPTS_API extern int Exe;  // used by Vx()/ATx() macros //DA

template <typename T> T& ResolveGameReference(const int client, const int server, const int leveledit)
{
	if (Exe == 6) InitEngine();
#pragma warning(suppress: 6011) //warning C6011: dereferencing NULL pointer <name>
	return *((T*)((Exe == 0) ? client : ((Exe == 1) ? server : ((Exe == 4) ? leveledit : 0))));
};

template <typename T, const int size> class RefArrayHelper
{
protected:
	char _dummy[size * sizeof(T)];
public:
	operator T* ()
	{
		return (T*) this;
	};

	operator const T* () const
	{
		return (T*) this;
	};

	T* operator & ()
	{
		return (T*) this;
	};

	const T* operator & () const
	{
		return (T*) this;
	};
};


#define REF_DECL(type, name) type & name
#define REF_ARR_DECL(type, name, size) RefArrayHelper<type, size> & name

#ifndef TTLE_EXPORTS
#define REF_DEF2(type, name, client, server) type & name = ResolveGameReference<type>(client, server, 0)
#define REF_ARR_DEF2(type, name, size, client, server) RefArrayHelper<type, size> & name = ResolveGameReference<RefArrayHelper< type, size >> (client, server, 0)
#endif //TTLE_EXPORTS

#define REF_DEF3(type, name, client, server, leveledit) type & name = ResolveGameReference<type>(client, server, leveledit)
#define REF_ARR_DEF3(type, name, size, client, server, leveledit) RefArrayHelper<type, size> & name = ResolveGameReference<RefArrayHelper< type, size >> ( client, server, leveledit)



// Q: So what are these ATx things anyways?
// A: The ATx macros are are designed for transparently calling functions that may have 
// multiple implementations depending on the current system configuration. The most 
// common use of them that you'll see is calling functions inside Renegade. They work 
// by dynamically rewriting code in order to call the proper function with little
// to no overhead, so do not modify them unless you *really* know what you are doing. 
//
// Q: And I use them how exactly?
// A: For ATx, make sure the function is declared with RENEGADE_FUNCTION and 
// then use ATx as the function implemention. 
//
// Examples:
// RENEGADE_FUNCTION void ExampleFunction2()	AT2(0xdeadbeef, 0xbaadf00d);
// RENEGADE_FUNCTION void ExampleFunction3()	AT3(0xdeadbeef, 0xbaadf00d, 0xdeadc0de);

#else
#define SCRIPTS_API
#define SHARED_API
#endif
#define DA_API SCRIPTS_API //DA

template<int stackBufferLength, typename Char> class FormattedString;

template<int stackBufferLength> class FormattedString<stackBufferLength, char>
{

public:

	char stackBuffer[stackBufferLength+1];
	char* heapBuffer;
	const char* value;
	int length;

	FormattedString(const char* format, ...)
	{
		va_list arguments;
		va_start(arguments, format);
		length = vsnprintf(stackBuffer, stackBufferLength, format, arguments);
		if (length >= 0)
		{
			// The formatted string fit on the stack. Use the stack buffer.
			stackBuffer[length] = '\0'; // Fix terminator. Only necessary if length == stackBufferLength.
			heapBuffer = 0;
			value = stackBuffer;
		}
		else
		{
			// The formatted string did not fit on the stack. Allocate a heap buffer.
			length = _vscprintf(format, arguments);
			heapBuffer = new char[length + 1];
			vsprintf(heapBuffer, format, arguments);
			value = heapBuffer;
		}
		va_end(arguments);
	}

	~FormattedString()
	{
		delete[] heapBuffer;
	}
	
	const char* getValue() const { return value; }
	const int getLength() const { return length; }
};

template<int stackBufferLength> class FormattedString<stackBufferLength, wchar_t>
{

public:

	wchar_t stackBuffer[stackBufferLength+1];
	wchar_t* heapBuffer;
	const wchar_t* value;
	int length;

	FormattedString(const wchar_t* format, ...)
	{
		va_list arguments;
		va_start(arguments, format);
		length = _vsnwprintf(stackBuffer, stackBufferLength, format, arguments);
		if (length >= 0)
		{
			// The formatted string fit on the stack. Use the stack buffer.
			stackBuffer[length] = '\0'; // Fix terminator. Only necessary if length == stackBufferLength.
			heapBuffer = 0;
			value = stackBuffer;
		}
		else
		{
			// The formatted string did not fit on the stack. Allocate a heap buffer.
			length = _vscwprintf(format, arguments);
			heapBuffer = new wchar_t[length + 1];
			_vsnwprintf(heapBuffer, length + 1, format, arguments);
			value = heapBuffer;
		}
		va_end(arguments);
	}

	~FormattedString()
	{
		delete[] heapBuffer;
	}
	
	const wchar_t* getValue() const { return value; }
	const int getLength() const { return length; }
};

// Format a string (sprintf style) using a stack buffer of a given size if possible, or a heap buffer otherwise.
#define TT_FORMAT(maxFormattedLength, format, ...) FormattedString<maxFormattedLength, char>(format, __VA_ARGS__).getValue()
#define TT_FORMAT_WIDE(maxFormattedLength, format, ...) FormattedString<maxFormattedLength, wchar_t>(format, __VA_ARGS__).getValue()

// Define the possible values for the Exe variable, rather than scattering magic numbers throughout the code
#define EXE_CLIENT 0
#define EXE_SERVER 1
#define EXE_LEVELEDIT 4
#define EXE_UNINITIALISED 6

#endif
