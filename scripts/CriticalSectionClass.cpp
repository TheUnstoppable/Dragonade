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
#include "CriticalSectionClass.h"



#include "ThreadClass.h"



CriticalSectionClass::CriticalSectionClass()
{
	handle = new CRITICAL_SECTION;
	InitializeCriticalSection((CRITICAL_SECTION*)handle);
	locked = 0;
}



CriticalSectionClass::~CriticalSectionClass()
{
	DeleteCriticalSection((CRITICAL_SECTION*)handle);
	delete handle;
	handle = 0;
}



#pragma warning(suppress: 26135) //warning C26135: Missing annotation
void CriticalSectionClass::Enter()
{
	EnterCriticalSection((CRITICAL_SECTION*)handle);
	locked++;
}



#pragma warning(suppress: 26135) //warning C26135: Missing annotation
void CriticalSectionClass::Exit()
{
	locked--;
	LeaveCriticalSection((CRITICAL_SECTION*)handle);
}



CriticalSectionClass::LockClass::LockClass(CriticalSectionClass &section) : CriticalSection(section)
{
	CriticalSection.Enter();
}



CriticalSectionClass::LockClass::~LockClass()
{
	CriticalSection.Exit();
}