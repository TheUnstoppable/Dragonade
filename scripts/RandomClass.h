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
#ifndef TT_INCLUDE_RANDOMCLASS_H
#define TT_INCLUDE_RANDOMCLASS_H
class RandomClass
{
	public:
		RandomClass(unsigned seed=0);
		operator int(void)
		{
			return(operator()());
		}
		int operator() (void);
		int operator() (int minval, int maxval);
		enum
		{
			SIGNIFICANT_BITS=15
		};
	protected:
		unsigned long Seed;
		enum
		{
			MULT_CONSTANT=0x41C64E6D,
			ADD_CONSTANT=0x00003039,
			THROW_AWAY_BITS=10
		};
};
#endif
