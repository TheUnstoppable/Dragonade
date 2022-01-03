/*	Renegade Scripts.dll
	Copyright 2013 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#ifndef TT_INCLUDE__VECTOR3_H
#define TT_INCLUDE__VECTOR3_H
#include "wwmath.h"
class Vector3
{
public:
	float X;
	float Y;
	float Z;

	TT_INLINE bool Is_Valid(void) const;
	TT_INLINE Vector3()
	{
		X = Y = Z = 0.0f;
	}
	TT_INLINE Vector3(const Vector3 &v)
	{
		X = v.X;
		Y = v.Y;
		Z = v.Z;
	}
	TT_INLINE Vector3(float x, float y, float z)
	{
		X = x;
		Y = y;
		Z = z;
	}
	TT_INLINE explicit Vector3(const float vector[3])
	{
		TT_ASSERT(vector != NULL);
		X = vector[0];
		Y = vector[1];
		Z = vector[2];
	}
	TT_INLINE Vector3 &operator= (const Vector3 &v)
	{
		X = v.X;
		Y = v.Y;
		Z = v.Z;
		return *this;
	}
	TT_INLINE void Set(float x, float y, float z)
	{
		X = x;
		Y = y;
		Z = z;
	}
	TT_INLINE void Set(const Vector3 &that)
	{
		X = that.X;
		Y = that.Y;
		Z = that.Z;
	}
	TT_INLINE float &operator[](int i)
	{
		return (&X)[i];
	}
	TT_INLINE const float &operator[](int i) const
	{
		return (&X)[i];
	}
	TT_INLINE void Normalize()
	{
		float len2 = WWMATH_FLOAT_TINY + Length2();
		float oolen = WWMath::Inv_Sqrt(len2);
		X *= oolen;
		Y *= oolen;
		Z *= oolen;
	}
	Vector3 Normalized() const
    {
        Vector3 v = *this;
        v.Normalize();
        return v;
    }
	TT_INLINE float Length() const
	{
		return WWMath::Sqrt(Length2());
	}
	TT_INLINE float Length2() const
	{
		return X * X + Y * Y + Z * Z;
	}
	TT_INLINE void Scale(const Vector3 &scale)
	{
		X *= scale.X;
		Y *= scale.Y;
		Z *= scale.Z;
	}
	TT_INLINE void Rotate_X(float angle)
	{
		Rotate_X(sinf(angle),cosf(angle));
	}
	TT_INLINE void Rotate_X(float s_angle,float c_angle)
	{
		float tmp_y = Y;
		float tmp_z = Z;
		Y = c_angle * tmp_y - s_angle * tmp_z;
		Z = s_angle * tmp_y + c_angle * tmp_z;
	}
	TT_INLINE void Rotate_Y(float angle)
	{
		Rotate_Y(sinf(angle),cosf(angle));
	}
	TT_INLINE void Rotate_Y(float s_angle,float c_angle)
	{
		float tmp_x = X;
		float tmp_z = Z;
		X = c_angle * tmp_x + s_angle * tmp_z;
		Z = -s_angle * tmp_x + c_angle * tmp_z;
	}
	TT_INLINE void Rotate_Z(float angle)
	{
		Rotate_Z(sinf(angle),cosf(angle));
	}
	TT_INLINE void Rotate_Z(float s_angle,float c_angle)
	{
		float tmp_x = X;
		float tmp_y = Y;
		X = c_angle * tmp_x - s_angle * tmp_y;
		Y = s_angle * tmp_x + c_angle * tmp_y;
	}
	TT_INLINE Vector3 operator-() const
	{
		return(Vector3(-X,-Y,-Z));
	}
	TT_INLINE Vector3 operator+() const
	{
		return *this;
	}
	TT_INLINE Vector3 &operator+= (const Vector3 &v)
	{
		X += v.X;
		Y += v.Y;
		Z += v.Z;
		return *this;
	}
	TT_INLINE Vector3 &operator-= (const Vector3 &v)
	{
		X -= v.X;
		Y -= v.Y;
		Z -= v.Z;
		return *this;
	}
	TT_INLINE Vector3 &operator*= (float k)
	{
		X = X * k;
		Y = Y * k;
		Z = Z * k;
		return *this;
	}
	TT_INLINE Vector3 &operator/= (float k)
	{
		X = X * 1.0f / k;
		Y = Y * 1.0f / k;
		Z = Z * 1.0f / k;
		return *this;
	}
	TT_INLINE Vector3 mul(const Vector3 &b){
		return Vector3(
			X*b.X,
			Y*b.Y,
			Z*b.Z);
	}
	friend Vector3 operator* (const Vector3 &a,float k);
	friend Vector3 operator* (float k,const Vector3 &a);
	friend Vector3 operator/ (const Vector3 &a,float k);
	friend Vector3 operator+ (const Vector3 &a,const Vector3 &b);
	friend Vector3 operator- (const Vector3 &a,const Vector3 &b);
	friend bool operator== (const Vector3 &a,const Vector3 &b);
	friend bool operator!= (const Vector3 &a,const Vector3 &b);
	friend float operator* (const Vector3 &a,const Vector3 &b);
	TT_INLINE static float Dot_Product(const Vector3 &a,const Vector3 &b)
	{
		return a * b;
	}
	TT_INLINE static Vector3 Cross_Product(const Vector3 &a,const Vector3 &b)
	{
		return Vector3((a.Y * b.Z - a.Z * b.Y),(a.Z * b.X - a.X * b.Z),(a.X * b.Y - a.Y * b.X));
	}
	TT_INLINE static void Cross_Product(const Vector3 &a,const Vector3 &b, Vector3* __restrict set_result)
	{
		TT_ASSERT(!(set_result == &a || set_result == &b));
		set_result->X = (a.Y * b.Z - a.Z * b.Y);
		set_result->Y = (a.Z * b.X - a.X * b.Z);
		set_result->Z = (a.X * b.Y - a.Y * b.X);
	}
	TT_INLINE static float Cross_Product_X(const Vector3 &a,const Vector3 &b)
	{
		return a.Y * b.Z - a.Z * b.Y;
	}
	TT_INLINE static float Cross_Product_Y(const Vector3 &a,const Vector3 &b)
	{
		return a.Z * b.X - a.X * b.Z;
	}
	TT_INLINE static float Cross_Product_Z(const Vector3 &a,const Vector3 &b)
	{
		return a.X * b.Y - a.Y * b.X;
	}
	TT_INLINE static void Add(const Vector3 &a,const Vector3 &b,Vector3 *set_result)
	{
		set_result->X = a.X + b.X;
		set_result->Y = a.Y + b.Y;
		set_result->Z = a.Z + b.Z;
	}
	TT_INLINE static void Subtract(const Vector3 &a,const Vector3 &b,Vector3 *set_result)
	{
		set_result->X = a.X - b.X;
		set_result->Y = a.Y - b.Y;
		set_result->Z = a.Z - b.Z;
	}
	TT_INLINE static float Find_X_At_Y(float y, const Vector3 &p1, const Vector3 &p2)
	{
		return(p1.X + ((y - p1.Y) * ((p2.X - p1.X) / (p2.Y - p1.Y))));
	}
	TT_INLINE static float Find_X_At_Z(float z, const Vector3 &p1, const Vector3 &p2)
	{
		return(p1.X + ((z - p1.Z) * ((p2.X - p1.X) / (p2.Z - p1.Z))));
	}
	TT_INLINE static float Find_Y_At_X(float x, const Vector3 &p1, const Vector3 &p2)
	{
		return(p1.Y + ((x - p1.X) * ((p2.Y - p1.Y) / (p2.X - p1.X))));
	}
	TT_INLINE static float Find_Y_At_Z(float z, const Vector3 &p1, const Vector3 &p2)
	{
		return(p1.Y + ((z - p1.Z) * ((p2.Y - p1.Y) / (p2.Z - p1.Z))));
	}
	TT_INLINE static float Find_Z_At_X(float x, const Vector3 &p1, const Vector3 &p2)
	{
		return(p1.Z + ((x - p1.X) * ((p2.Z - p1.Z) / (p2.X - p1.X))));
	}
	TT_INLINE static float Find_Z_At_Y(float y, const Vector3 &p1, const Vector3 &p2)
	{
		return(p1.Z + ((y - p1.Y) * ((p2.Z - p1.Z) / (p2.Y - p1.Y))));
	}
	TT_INLINE void Update_Min(const Vector3 &a)
	{
		if (a.X < X)
		{
			X = a.X;
		}
		if (a.Y < Y)
		{
			Y = a.Y;
		}
		if (a.Z < Z)
		{
			Z = a.Z;
		}
	}
	TT_INLINE void Update_Max(const Vector3 &a)
	{
		if (a.X > X)
		{
			X = a.X;
		}
		if (a.Y > Y)
		{
			Y = a.Y;
		}
		if (a.Z > Z)
		{
			Z = a.Z;
		}
	}
	TT_INLINE void Cap_Absolute_To(const Vector3 &a)
	{
		if (X > 0)
		{
			if (a.X < X)
			{
				X = a.X;
			}
		}
		else
		{
			if (-a.X > X)
			{
				X = -a.X;
			}
		}
		if (Y > 0)
		{
			if (a.Y < Y)
			{
				Y = a.Y;
			}
		}
		else
		{
			if (-a.Y > Y)
			{
				Y = -a.Y;
			}
		}
		if (Z > 0)
		{
			if (a.Z < Z)
			{
				Z = a.Z;
			}
		}
		else
		{
			if (-a.Z > Z)
			{
				Z = -a.Z;
			}
		}
	}

	TT_INLINE Vector3 abs() const
	{
		return Vector3(WWMath::Fabs(X), WWMath::Fabs(Y), WWMath::Fabs(Z));
	}

	TT_INLINE static float Distance(const Vector3 &p1, const Vector3 &p2)
	{
		return (p1 - p2).Length();
	}
	TT_INLINE static float Distance_Squared(const Vector3 &p1, const Vector3 &p2)
	{
		return (p1 - p2).Length2();
	}
	TT_INLINE static void Lerp(const Vector3 &a, const Vector3 &b, float alpha,Vector3 *set_result)
	{
		set_result->X = (a.X + (b.X - a.X) * alpha);
		set_result->Y = (a.Y + (b.Y - a.Y) * alpha);
		set_result->Z = (a.Z + (b.Z - a.Z) * alpha);
	}
	unsigned	long	Convert_To_ARGB( void ) const;
	float Quick_Length(void) const
	{
		float max = WWMath::Fabs(X);
		float mid = WWMath::Fabs(Y);
		float min = WWMath::Fabs(Z);
		float tmp;
		if (max < mid) { tmp = max; max = mid; mid = tmp; }
		if (max < min) { tmp = max; max = min; min = tmp; }
		if (mid < min) { tmp = mid; mid = min; min = mid; }
		return max + (11.0f / 32.0f)*mid + (1.0f / 4.0f)*min;
	}

	TT_INLINE static Vector3 Replicate(float n)
	{
		return Vector3(n, n, n);
	}
};
TT_INLINE Vector3 operator* (const Vector3 &a,float k)
{
	return Vector3((a.X * k),(a.Y * k),(a.Z * k));
}
TT_INLINE Vector3 operator* (float k, const Vector3 &a)
{
	return Vector3((a.X * k),(a.Y * k),(a.Z * k));
}
TT_INLINE Vector3 operator/ (const Vector3 &a,float k)
{
	return Vector3((a.X * 1.0f/k),(a.Y * 1.0f/k),(a.Z * 1.0f/k));
}
TT_INLINE Vector3 operator+ (const Vector3 &a,const Vector3 &b)
{
	return Vector3(a.X + b.X,a.Y + b.Y,a.Z + b.Z);
}
TT_INLINE Vector3 operator- (const Vector3 &a,const Vector3 &b)
{
	return Vector3(a.X - b.X,a.Y - b.Y,a.Z - b.Z);
}
TT_INLINE float operator* (const Vector3 &a,const Vector3 &b)
{
	return	a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}
TT_INLINE bool operator== (const Vector3 &a,const Vector3 &b)
{
	return ((a.X == b.X) && (a.Y == b.Y) && (a.Z == b.Z));
}
TT_INLINE bool operator != (const Vector3 &a,const Vector3 &b)
{
	return ((a.X != b.X) || (a.Y != b.Y) || (a.Z != b.Z));
}
TT_INLINE Vector3 Normalize(const Vector3 &vec)
{
	float len2 = WWMATH_FLOAT_TINY + vec.Length2();
	return vec * WWMath::Inv_Sqrt(len2);
}
TT_INLINE void Swap(Vector3 &a,Vector3 &b)
{
	Vector3 tmp(a);
	a = b;
	b = tmp;
}
TT_INLINE Vector3 Lerp(const Vector3 &a, const Vector3 &b, float alpha)
{
	return Vector3((a.X + (b.X - a.X) * alpha),(a.Y + (b.Y - a.Y) * alpha),(a.Z + (b.Z - a.Z) * alpha));
}
TT_INLINE void Lerp(const Vector3 &a, const Vector3 &b, float alpha,Vector3 *set_result)
{
	set_result->X = (a.X + (b.X - a.X) * alpha);
	set_result->Y = (a.Y + (b.Y - a.Y) * alpha);
	set_result->Z = (a.Z + (b.Z - a.Z) * alpha);
}
TT_INLINE bool Is_Valid_Float(float x)
{
	unsigned long * plong = (unsigned long *)(&x);
	unsigned long exponent = ((*plong) & 0x7F800000) >> (32-9);

	// if exponent is 0xFF, this is a NAN 
	if (exponent == 0xFF) {
		return false;
	}
	return true;
}
TT_INLINE bool Vector3::Is_Valid(void) const
{
	return (Is_Valid_Float(X) && Is_Valid_Float(Y) && Is_Valid_Float(Z));
}

TT_INLINE unsigned long	Vector3::Convert_To_ARGB( void ) const 
{
	return (unsigned(255)<<24) | 
			 (unsigned(X*255.0f)<<16) | 
			 (unsigned(Y*255.0f)<<8) | 
			 (unsigned(Z*255.0f));
}
#endif
