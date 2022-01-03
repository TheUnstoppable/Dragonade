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
#include "Random3Class.h"



int Random3Class::Mix1[20] = {
	0x0baa96887, 0x01e17d32c, 0x003bcdc3c, 0x00f33d1b2,
	0x076a6491d, 0x0c570d85d, 0x0e382b1e3, 0x078db4362,
	0x07439a9d4, 0x09cea8ac5, 0x089537c5c, 0x02588f55d,
	0x0415b5e1d, 0x0216e3d95, 0x085c662e7, 0x05e8ab368,
	0x03ea5cc8c, 0x0d26a0f74, 0x0f3a9222b, 0x048aad7e4
};

int Random3Class::Mix2[20] = {
	0x04b0f3b58, 0x0e874f0c3, 0x06955c5a6, 0x055a7ca46,
	0x04d9a9d86, 0x0fe28a195, 0x0b1ca7865, 0x06b235751,
	0x09a997a61, 0x0aa6e95c8, 0x0aaa98ee1, 0x05af9154c,
	0x0fc8e2263, 0x0390f5e8c, 0x058ffd802, 0x0ac0a5eba,
	0x0ac4874f6, 0x0a9df0913, 0x086be4c74, 0x0ed2c123b
};

Random3Class::Random3Class(unsigned seed1, unsigned seed2) :
	Seed(seed1),
	Index(seed2)
{
}

int Random3Class::operator() (void)
{
	int loword = Seed;
	int hiword = Index++;
	for (int i = 0; i < 4; i++)
	{
		int hihold  = hiword;
		int temp    = hihold ^  Mix1[i];
		int itmpl   = temp   &  0xffff;
		int itmph   = temp   >> 16;
		temp    = itmpl * itmpl + ~(itmph * itmph);
		temp    = (temp >> 16) | (temp << 16);
		hiword  = loword ^ ((temp ^ Mix2[i]) + itmpl * itmph);
		loword  = hihold;
	}
	return(hiword);
}	

int Random3Class::operator() (int minval, int maxval)
{
	return(Pick_Random_Number(*this, minval, maxval));
}
