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
#include "Random2Class.h"
#include "Random3Class.h"
#define N 624
#define M 397
#define MATRIX_A 0x9908b0df
#define UPPER_MASK 0x80000000
#define LOWER_MASK 0x7fffffff
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U(y)  (y >> 11)
#define TEMPERING_SHIFT_S(y)  (y << 7)
#define TEMPERING_SHIFT_T(y)  (y << 15)
#define TEMPERING_SHIFT_L(y)  (y >> 18)
#ifndef TDBEDIT
#ifndef W3DVIEWER
REF_DEF2(CRandom, FreeRandom, 0x0085CB40, 0x0085BD28);
#endif
#endif

Random2Class::Random2Class(unsigned seed) : Index1(0), Index2(103)
{
	Random3Class random(seed);
	for (int index = 0; index < ARRAY_SIZE(Table); index++)
	{
		Table[index] = random;
	}
}

int Random2Class::operator() (void) 
{
	Table[Index1] ^= Table[Index2];
	int val = Table[Index1];
	Index1++;
	Index2++;
	if (Index1 >= ARRAY_SIZE(Table)) Index1 = 0;
	if (Index2 >= ARRAY_SIZE(Table)) Index2 = 0;
	return(val);
}

int Random2Class::operator() (int minval, int maxval)
{
	return(Pick_Random_Number(*this, minval, maxval));
}

Random4Class::Random4Class(unsigned int seed)
{
	if (!seed) seed=4375;
	mt[0]= seed & 0xffffffff;
	for (mti=1; mti<N; mti++)
		mt[mti] = (69069 * mt[mti-1]) & 0xffffffff;	
}
int Random4Class::operator() (void)
{
	unsigned int y;
	static unsigned int mag01[2]={0x0, MATRIX_A};
	if (mti >= N)
	{
		int kk;
		for (kk=0;kk<N-M;kk++)
		{
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		for (;kk<N-1;kk++)
		{
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
		mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1];
		mti = 0;
	}
	y = mt[mti++];
	y ^= TEMPERING_SHIFT_U(y);
	y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
	y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
	y ^= TEMPERING_SHIFT_L(y);
	int *x=(int *)&y;
	return *x;
}
int Random4Class::operator() (int minval, int maxval)
{
	return(Pick_Random_Number(*this, minval, maxval));
}

float Random4Class::Get_Float()
{
	int x=(*this)();
	unsigned int *y=(unsigned int *) &x;

	return (*y)*2.3283064370807973754314699618685e-10f;
}
