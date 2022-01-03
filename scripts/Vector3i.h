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
#ifndef TT_INCLUDE_VECTOR3I_H
#define TT_INCLUDE_VECTOR3I_H
class Vector3i
{
public:
	int		I;
	int		J;
	int		K;
	inline Vector3i(void)
	{
	}
	inline Vector3i(int i,int j,int k)
	{
		I = i; J = j; K = k;
	}
	inline bool			operator== (const Vector3i & v) const
	{
		return (I == v.I && J == v.J && K == v.K);
	}
	inline bool			operator!= (const Vector3i& v) const
	{
		return !(I == v.I && J == v.J && K == v.K);
	}
	inline const	int&	operator[] (int n) const
	{
		return ((int*)this)[n];
	}
	inline int&			operator[] (int n)
	{
		return ((int*)this)[n];
	}
};
class Vector3i16 {
public:
	unsigned short I;
	unsigned short J;
	unsigned short K;
	inline Vector3i16(void)
	{
	}
	inline Vector3i16(unsigned short i,unsigned short j,unsigned short k)
	{
		I = i; J = j; K = k;
	}
	inline const unsigned short &	operator[] (int n) const
	{
		return ((unsigned short *)this)[n]; 
	}
	inline unsigned short & operator[] (int n)
	{
		return ((unsigned short *)this)[n]; 
	}
};

#endif
