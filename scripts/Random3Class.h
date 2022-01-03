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
#ifndef TT_INCLUDE__RANDOM3CLASS_H
#define TT_INCLUDE__RANDOM3CLASS_H
template<class T>
int Pick_Random_Number(T & generator, int minval, int maxval)
{
	if (minval == maxval) return(minval);
	if (minval > maxval)
	{
		int temp = minval;
		minval = maxval;
		maxval = temp;
	}
	int magnitude = maxval - minval;
	int highbit = T::SIGNIFICANT_BITS-1;
	while ((magnitude & (1 << highbit)) == 0 && highbit > 0)
	{
		highbit--;
	}
	int mask = ~( (~0L) << (highbit+1));
	int pick = magnitude+1;
	while (pick > magnitude)
	{
		pick = generator() & mask;
	}
	return(pick + minval);
}

class Random3Class
{
public:
	Random3Class(unsigned seed1 = 0, unsigned seed2 = 0);
	operator int(void)
	{
		return(operator()());
	}
	int operator() (void);
	int operator() (int minval, int maxval);
	enum {
		SIGNIFICANT_BITS=32
	};
protected:
	static int Mix1[20];
	static int Mix2[20];
	int Seed;
	int Index;
};
#endif
