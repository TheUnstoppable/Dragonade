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
#ifndef _FASTCRITICALSECTION_H_
#define _FASTCRITICALSECTION_H_
#include "platform.h"
class FastCriticalSection
{
protected:
	volatile long Flag;
public:
	inline FastCriticalSection(): Flag(0) {};
	inline FastCriticalSection(const FastCriticalSection&): Flag(0) { assert(false); }; 
	inline void Enter()
	{
		for (;;)
		{
			if (_interlockedbittestandset(&Flag, 0) == 0) return;
			YieldThread();
		};
	};
	inline void Leave()
	{
		Flag = 0;
	};

	class Lock
	{
	protected:
		FastCriticalSection& cs;
	public:
		Lock& operator=(const Lock&) 
		{ 
			return *this; 
		};
		inline Lock(FastCriticalSection& _cs): cs(_cs)
		{
			cs.Enter();
		};
		inline ~Lock()
		{
			cs.Leave();
		};
	};
};
#endif