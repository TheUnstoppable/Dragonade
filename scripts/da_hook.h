/*	Renegade Scripts.dll
    Dragonade Hook Functions
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

void SCRIPTS_API Console_Output(const char *Output,...);

class DAHookManager {
public:
	static unsigned long Read_Hook_Address(unsigned long Address) {
		unsigned char OpCode;
		Read(Address,&OpCode,1);
		if (OpCode != 0xE9 && OpCode != 0xE8) {
			Console_Output("Error: Hook at address 0x%x could not be installed: unable to calculate jump offset. Some features may be disabled.\n",Address);
			return 0;
		}
		unsigned char JumpAddress[4];
		Read(Address+1,JumpAddress,4);
		return Address + 5 + *(int*)&JumpAddress;
	}
	static void Write(unsigned long Address,unsigned char *Patch,int Size) {
		DWORD OldProtect;
		HANDLE h = GetCurrentProcess();
		VirtualProtectEx(h,(void*)Address,Size,PAGE_EXECUTE_READWRITE,&OldProtect);
		if (!WriteProcessMemory(h,(void*)Address,Patch,Size,0)) {
			Console_Output("Error: Hook at address 0x%x could not be installed: %d. Some features may be disabled.\n",Address,GetLastError());
		}
		VirtualProtectEx(h,(void*)Address,Size,OldProtect,0);
		CloseHandle(h);
	}
	static void Read(unsigned long Address,unsigned char *Buffer,int Size) {
		DWORD OldProtect;
		HANDLE h = GetCurrentProcess();
		VirtualProtectEx(h,(void*)Address,Size,PAGE_EXECUTE_READWRITE,&OldProtect);
		if (!ReadProcessMemory(h,(void*)Address,(void*)Buffer,Size,0)) {
			Console_Output("Error: Hook at address 0x%x could not be installed: %d. Some features may be disabled.\n",Address,GetLastError());
		}
		VirtualProtectEx(h,(void*)Address,Size,OldProtect,0);
		CloseHandle(h);
	}
	static void Install_Jump_Hook(unsigned long HookAddress,void *JumpTo) {
		unsigned char Hook[5];
		memset(Hook,0x90,5);
		Hook[0] = 0xE9;
		unsigned long JumpAddress = (unsigned long)JumpTo - HookAddress - 5;
		memcpy(&Hook[1],&JumpAddress,4);
		Write(HookAddress,Hook,5);
	}
	static void Install_Call_Hook(unsigned long HookAddress,void *JumpTo) {
		unsigned char Hook[5];
		memset(Hook,0x90,5);
		Hook[0] = 0xE8;
		unsigned long JumpAddress = (unsigned long)JumpTo - HookAddress - 5;
		memcpy(&Hook[1],&JumpAddress,4);
		Write(HookAddress,Hook,5);
	}
	static void Disable_Function(unsigned long Address) {
		unsigned char Ret = 0xC3;
		Write(Address,&Ret,1);
	}
};

