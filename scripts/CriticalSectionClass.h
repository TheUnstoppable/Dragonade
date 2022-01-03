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
#ifndef TT_INCLUDE__CRITICALSECTIONCLASS_H
#define TT_INCLUDE__CRITICALSECTIONCLASS_H



#include "ThreadClass.h"



class SCRIPTS_API CriticalSectionClass
{

private:

	void* handle;
	unsigned int locked;

public:

	class LockClass
	{
	
	private:
	
		CriticalSectionClass& CriticalSection;

	public:

		LockClass(CriticalSectionClass& section);
		~LockClass();
		LockClass operator=(LockClass&);

	};

	CriticalSectionClass();
	~CriticalSectionClass();
	void Enter();
	void Exit();

};


class FastCriticalSectionClass
{

private:

	friend class LockClass;

	volatile long Flag; // 0000


	void Enter()
	{
		TT_ASSERT((size_t)&Flag % 4 == 0); // aligned to 4 bytes please
		for (;;)
		{
			if (_interlockedbittestandset(&Flag, 0) == 0) return;
			_mm_pause();
		};
	}


	void Leave()
	{
		Flag = 0;
	}


public:

	class LockClass
	{
		FastCriticalSectionClass& criticalSection;

		LockClass& operator=(const LockClass&)
		{
			return *this;
		}


	public:

		LockClass(FastCriticalSectionClass& _criticalSection) :
			criticalSection(_criticalSection)
		{
			criticalSection.Enter();
		}

		~LockClass()
		{
			criticalSection.Leave();
		}

	};

	FastCriticalSectionClass() :
		Flag(0)
	{
	}

}; // 0004



#endif
