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
#include "ThreadClass.h"
#include "SysTimeClass.h"

void ThreadClass::Sleep_Ms(uint time)
{
	Sleep(time);
}

void ThreadClass::Internal_Thread_Function(void *data)
{
	ThreadClass *thread = (ThreadClass *)data;
	thread->running = true;
	int threadid = GetCurrentThreadId();
	thread->threadid = threadid;
	if (thread->exceptionhandler)
	{
		__try {
			thread->Thread_Function();
#pragma warning(suppress: 6322) //warning C6322: empty _except block
		} __except(thread->exceptionhandler(GetExceptionCode(), GetExceptionInformation())) {};
	}
	else
	{
		thread->Thread_Function();
	}
	thread->handle = 0;
	thread->threadid = 0;
}

void ThreadClass::Execute()
{
	handle = (HANDLE)_beginthread(&Internal_Thread_Function,0,this);
	SetThreadPriority(handle,thread_priority);
}

void ThreadClass::Set_Priority(int priority)
{
	thread_priority = priority;
	if (handle)
	{
		SetThreadPriority(handle,priority);
	}
}

void ThreadClass::Stop(uint value)
{
	running = false;
	unsigned int time = TIMEGETTIME();
	while (handle)
	{
		unsigned int time2 = TIMEGETTIME();
		if (time2 - time > value)
		{
#pragma warning(suppress: 6258) //warning C6258: using TerminateThread does not allow proper thread clean up.
			TerminateThread(handle,0);
			handle = 0;
		}
		Sleep(0);
	}
}

ThreadClass::ThreadClass(char  const *Name, LONG (WINAPI *ExceptionHandler)(int, LPEXCEPTION_POINTERS)) : running(false), handle(0)
{
	if (Name && strlen(Name) < 70)
	{
		strcpy(name,Name);
	}
	else
	{
		strcpy(name,"No Name");
	}
	exceptionhandler = ExceptionHandler;
}

ThreadClass::~ThreadClass()
{
	Stop(3000);
}
