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
#ifndef TT_INCLUDE__VECTOR4_H
#define TT_INCLUDE__VECTOR4_H
#include "wwmath.h"
class Vector4 
{
public:
	float X;
	float Y;
	float Z;
	float W;

	// default constructors
	TT_INLINE Vector4() {};
	TT_INLINE Vector4(const Vector4& v) { X = v.X; Y = v.Y; Z = v.Z; W = v.W; };
	TT_INLINE Vector4(float x, float y, float z, float w) {	X = x; Y = y; Z = z; W = w;	};
	TT_INLINE explicit Vector4(const float v[4]) { TT_ASSERT(v != NULL); X = v[0]; Y = v[1]; Z = v[2]; W = v[3]; };

	// conversion constructors
	//TT_INLINE explicit Vector4(const Vector2& v) { X = v.X; Y = v.Y; Z = 0.0f; W = 0.0f; };
	//TT_INLINE explicit Vector4(const Vector3& v) { X = v.X; Y = v.Y; Z = v.Z; W = 0.0f; };

	// assignment operators
	TT_INLINE Vector4& operator=(const Vector4& v) { X = v.X; Y = v.Y; Z = v.Z; W = v.W; return *this; };	
	TT_INLINE void Set(float x, float y, float z, float w) { X = x; Y = y; Z = z; W = w; };

	// array access operators
	TT_INLINE float& operator[](int i) { return (&X)[i]; };
	TT_INLINE const float& operator[](int i) const { return (&X)[i]; };
	TT_INLINE void Normalize(void)
	{
		float len2 = WWMATH_FLOAT_TINY + Length2();
		float oolen = WWMath::Inv_Sqrt(len2);
		X *= oolen;
		Y *= oolen;
		Z *= oolen;
		W *= oolen;
	}
	TT_INLINE float Length(void) const
	{
		return WWMath::Sqrt(Length2());
	}
	TT_INLINE float Length2(void) const
	{
		return X*X + Y*Y + Z*Z + W*W;
	}

	// unary operators
	TT_INLINE Vector4 operator-() const { return(Vector4(-X,-Y,-Z,-W)); } ;
	TT_INLINE Vector4 operator+() const { return *this; }; 

	TT_INLINE Vector4& operator+=(const Vector4& v) { X += v.X; Y += v.Y; Z += v.Z; W += v.W; return *this;	};
	TT_INLINE Vector4& operator-=(const Vector4& v) { X -= v.X; Y -= v.Y; Z -= v.Z; W -= v.W; return *this;	};
	TT_INLINE Vector4& operator*=(float f) { X *= f; Y *= f; Z *= f; W *= f; return *this;	};
	TT_INLINE Vector4& operator/=(float f)	{ f = 1.0f / f; X /= f; Y /= f; Z /= f; W /= f;	return *this; };

	// vector addition/subtraction
	friend Vector4 operator+(const Vector4& a, const Vector4& b);
	friend Vector4 operator-(const Vector4& a, const Vector4& b);

	// scalar multiplication/division
	friend Vector4 operator*(const Vector4 &a, float b);
	friend Vector4 operator*(float a, const Vector4 &b);
	friend Vector4 operator/(const Vector4 &a, float b);

	// equality operators
	friend bool operator== (const Vector4 &a, const Vector4 &b);
	friend bool operator!= (const Vector4 &a, const Vector4 &b);

	friend float operator * (const Vector4 &a,const Vector4 &b);
	TT_INLINE static float Dot_Product(const Vector4 &a,const Vector4 &b)
	{
		return a * b;
	}
	TT_INLINE static Vector4 Lerp(const Vector4 & a, const Vector4 & b, float alpha)
	{
		return Vector4((a.X + (b.X - a.X)*alpha),(a.Y + (b.Y - a.Y)*alpha),(a.Z + (b.Z - a.Z)*alpha),(a.W + (b.W - a.W)*alpha));
	}
	TT_INLINE static void Lerp(const Vector4 & a, const Vector4 & b, float alpha,Vector4 * set_result)
	{
		set_result->X = (a.X + (b.X - a.X)*alpha);
		set_result->Y = (a.Y + (b.Y - a.Y)*alpha);
		set_result->Z = (a.Z + (b.Z - a.Z)*alpha);
		set_result->W = (a.W + (b.W - a.W)*alpha);
	}
};

TT_INLINE Vector4 operator+(const Vector4& a, const Vector4& b)
{
	return Vector4(a.X + b.X, a.Y + b.Y, a.Z + b.Z,	a.W + b.W);
};

TT_INLINE Vector4 operator-(const Vector4& a, const Vector4& b)
{
	return Vector4(a.X - b.X, a.Y - b.Y, a.Z - b.Z,	a.W - b.W);
};

TT_INLINE Vector4 operator*(const Vector4 &a, float b)
{
	return Vector4(a.X * b, a.Y * b, a.Z * b, a.W * b);
};

TT_INLINE Vector4 operator*(float a, const Vector4 &b)
{
	return b * a;
};

TT_INLINE Vector4 operator/(const Vector4 &a, float k)
{
	return Vector4(a.X * 1.0f / k, a.Y * 1.0f / k, a.Z * 1.0f / k, a.W * 1.0f / k);
};

TT_INLINE bool operator==(const Vector4 &a, const Vector4 &b)
{
	return (a.X == b.X) && (a.Y == b.Y) && (a.Z == b.Z) && (a.W == b.W);
};

TT_INLINE bool operator!=(const Vector4 &a, const Vector4 &b)
{
	return (a.X != b.X) || (a.Y != b.Y) || (a.Z != b.Z) || (a.W != b.W);
};

TT_INLINE float operator * (const Vector4 &a,const Vector4 &b)
{
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3];
};

TT_INLINE Vector4 Normalize(const Vector4 & vec)
{
	float len2 = WWMATH_FLOAT_TINY + vec.Length2();
	return vec * WWMath::Inv_Sqrt(len2);
}

TT_INLINE void Swap(Vector4 & a,Vector4 & b)
{
	Vector4 tmp(a);
	a = b;
	b = tmp;
}

TT_INLINE Vector4 Lerp(const Vector4 & a, const Vector4 & b, float alpha)
{
	return Vector4((a.X + (b.X - a.X) * alpha),(a.Y + (b.Y - a.Y) * alpha),(a.Z + (b.Z - a.Z) * alpha),(a.W + (b.W - a.W) * alpha));
}

#endif
