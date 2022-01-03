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
#ifndef TT_INCLUDE__SINGLETON_H
#define TT_INCLUDE__SINGLETON_H



/*==============================================================================


To use this class properly, look at the example below:


#define someClass (SomeClass::getInstance())



class SomeClass :
	public Singleton<SomeClass>
{

protected:

	friend Singleton<SomeClass>;
	SomeClass();

};


------------------------------------------------------------------------------*/



template<typename T> class Singleton
{

protected:

#ifdef DEBUG
	Singleton()
	{
		static uint instanceCount = 0;
		TT_ASSERT(++instanceCount == 1);
	}
#endif

public:
	
	static T& getInstance()
	{
		static T instancex;
		return instancex;
	}

};


#endif