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
#ifndef TT_INCLUDE__RANDOM2CLASS_H
#define TT_INCLUDE__RANDOM2CLASS_H
#define	ARRAY_SIZE(x)		int(sizeof(x)/sizeof(x[0]))
class Random2Class
{
public:
	Random2Class(unsigned seed = 0);
	operator int(void)
	{
		return(operator()());
	}
	int operator() (void);
	int operator() (int minval, int maxval);
	enum
	{
		SIGNIFICANT_BITS=32
	};
protected:
	int Index1;
	int Index2;
	int Table[250];
}; // 03F0

#define	CRANDOM_FLOAT_RANGE			0x1000
class CRandom {
public:
	CRandom( void )
	{
	}
	~CRandom( void )
	{
	}
	inline int Get_Int( void )
	{
		return Generator();
	}
	inline int Get_Int( int max )
	{
		if (max)
		{
			return (Generator() & 0x7FFFFFFF) % max;
		}
		return 0;
	}
	inline int Get_Int( int min, int max )
	{
		if ( min > max )
		{
			int temp = min;
			min = max;
			max = temp;
		}
		return Get_Int( max - min ) + min;
	}
	inline float Get_Float( void )
	{
		return (float)(Get_Int( CRANDOM_FLOAT_RANGE+1 )) / (float)CRANDOM_FLOAT_RANGE;
	}
	inline float Get_Float( float max )
	{
		return Get_Float() * max;
	}
	inline float Get_Float( float min, float max )
	{
		if ( min > max )
		{
			float temp = min;
			min = max;
			max = temp;
		}
		return Get_Float() * ( max - min ) + min;
	}
private:
	Random2Class Generator;
};
class Random4Class
{
	public:
		Random4Class(unsigned int seed=4357);
		operator int(void)
		{
			return(operator()());
		}
		int operator() (void);
		int operator() (int minval, int maxval);
		float Get_Float();
		enum {
			SIGNIFICANT_BITS=32
		};
protected:
		unsigned int mt[624];
		int mti;
};
#ifndef TTLE_EXPORTS
#ifndef TDBEDIT
#ifndef W3DVIEWER
extern REF_DECL(CRandom, FreeRandom);
#endif
#endif
#endif
#endif
