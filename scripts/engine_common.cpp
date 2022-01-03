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

#include "scripts.h"
#include "engine_string.h"
int Exe = 6;
#ifndef TTLE_EXPORTS

RENEGADE_FUNCTION
void SCRIPTS_API Console_Output(const char *Output,...)
AT2(0x00428B50,0x00428CD0);

RENEGADE_FUNCTION
void Parse_Console_Input(const char *)
AT2(0x004287E0,0x00428960);

void SCRIPTS_API Console_Input(const char *Input)
{
	if (strlen(Input) > 255)
	{
		return;
	}
	Parse_Console_Input(Input);
}

void ConsoleOutputDirect(const char *Output){
	Console_Output("%s", Output);
}


void Console_OutputV(const char *Output, va_list va){
	StringClass buf;
	buf.Format_Args(Output, va);
	Console_Output("%s", buf.Peek_Buffer());
}

#endif

void* HookupAT3(void* a, void* b, void* c, void* patch_start)
{
    static const char relJumpOp = '\xE9';

    void* function = nullptr;
    if (Exe == 6) InitEngine();
    switch (Exe)
    {
        case 0: function = a; break;
        case 1: function = b; break;
        case 4: function = c; break;
        default: TT_UNREACHABLE;
    }
    TT_ASSERT(function != nullptr);

    size_t patch_size = 5;
    char* function_rel = (char*)((char*)function - (char*)patch_start - 5);
    char* current_addr = (char*)patch_start;

    DWORD oldProtection;
    HANDLE process = GetCurrentProcess();
    VirtualProtectEx(process, patch_start, patch_size, PAGE_EXECUTE_READWRITE, &oldProtection);

    memcpy(current_addr, &relJumpOp, sizeof(relJumpOp));
    memcpy(current_addr + 1, &function_rel, sizeof(function_rel));

    VirtualProtectEx(process, patch_start, patch_size, oldProtection, &oldProtection);

    return function;
};

const unsigned char Code[19] = {0x6A,0x40,0xFF,0x74,0x24,0x0C,0xFF,0x74,0x24,0x0C,0xE8,0xC0,0xFF,0xFF,0xFF,0x83,0xC4,0x0C,0xC3};

#if (_MSC_VER >= 1400)
void AppInvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line,	uintptr_t pReserved )
{
	DebugBreak();
}
#endif
void InitEngine()
{
#ifdef TT_EXPORTS
#if (_MSC_VER >= 1400)
	_set_invalid_parameter_handler(AppInvalidParameterHandler);
#endif
#endif
	if (!memcmp((void *)0x0078CE49, Code, sizeof(Code))) Exe = 0;
	if (!memcmp((void *)0x0078C6E9, Code, sizeof(Code))) Exe = 1;
	if (!memcmp((void *)0x0078790C, Code, sizeof(Code))) Exe = 4;
}
