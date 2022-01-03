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
#ifndef TT_INCLUDE__THREADCLASS_H
#define TT_INCLUDE__THREADCLASS_H



class SCRIPTS_API ThreadClass
{

public:
	bool running; //4
	char name[67]; //5
	int threadid; //48
	LONG (WINAPI *exceptionhandler)(int, LPEXCEPTION_POINTERS); //4C
	HANDLE handle; //50
	int thread_priority; //54
	ThreadClass(char  const *Name, LONG (WINAPI *ExceptionHandler)(int, LPEXCEPTION_POINTERS));
	virtual ~ThreadClass();
	virtual void Thread_Function() = 0;
	static void Internal_Thread_Function(void*);
	void Execute();
	void Set_Priority(int);
	void Stop(uint);
	static void Sleep_Ms(uint);

	bool Is_Running()
	{
		return handle != 0;
	}
	static int Get_Current_Thread_ID()
	{
		return GetCurrentThreadId();
	}
	const char *Get_Name()
	{
		return name;
	}
};



#endif