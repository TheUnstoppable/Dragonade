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
#include "wwmath.h"
#include <stdlib.h>
#ifndef W3DVIEWER
SCRIPTS_API REF_ARR_DEF3(float, _FastAcosTable, ARC_TABLE_SIZE, 0x00852FB0, 0x00852198, 0x0087145C);
SCRIPTS_API REF_ARR_DEF3(float, _FastAsinTable, ARC_TABLE_SIZE, 0x00850FB0, 0x00850198, 0x0086F45C);
SCRIPTS_API REF_ARR_DEF3(float, _FastSinTable, SIN_TABLE_SIZE, 0x0084FFB0, 0x0084F198, 0x0086E45C);
SCRIPTS_API REF_ARR_DEF3(float, _FastInvSinTable, SIN_TABLE_SIZE, 0x00851FB0, 0x00851198, 0x0087045C);
#else
float _FastAcosTable[ARC_TABLE_SIZE];
float _FastAsinTable[ARC_TABLE_SIZE];
float _FastSinTable[SIN_TABLE_SIZE];
float _FastInvSinTable[SIN_TABLE_SIZE];
#endif
void WWMath::Init(void)
{
	for (int a=0;a<ARC_TABLE_SIZE;++a) {
		float cv=float(a-ARC_TABLE_SIZE/2)*(1.0f/(ARC_TABLE_SIZE/2));
		_FastAcosTable[a]=acos(cv);
		_FastAsinTable[a]=asin(cv);
	}
	for (int a=0;a<SIN_TABLE_SIZE;++a) {
		float cv= (float)a * 2.0f * WWMATH_PI / SIN_TABLE_SIZE;
		_FastSinTable[a]=sin(cv);
		if (a>0) {
			_FastInvSinTable[a]=1.0f/_FastSinTable[a];
		} else {
			_FastInvSinTable[a]=WWMATH_FLOAT_MAX;
		}
	}
}
void WWMath::Shutdown(void)
{
}
float WWMath::Random_Float(void) 
{ 
	return ((float)(rand() & 0xFFF)) / (float)(0xFFF); 
}
