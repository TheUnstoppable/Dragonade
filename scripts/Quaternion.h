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
#ifndef TT_INCLUDE__QUATERNION_H
#define TT_INCLUDE__QUATERNION_H



class Vector2;
class Vector3;
class Matrix3;
class Matrix3D;
class Matrix4;
#include "Vector3.h"


class Quaternion
{

public:

	static const Quaternion IDENTITY;

	union
	{
		struct
		{
			float X;
			float Y;
			float Z;
			float W;
		};
		struct
		{
			float i;
			float j;
			float k;
			float t;
		};
	};


	TT_INLINE Quaternion(void) {};
	TT_INLINE explicit Quaternion(bool init) { if (init) { X = 0.0f; Y = 0.0f; Z = 0.0f; W = 1.0f; } }
	TT_INLINE explicit Quaternion(float a, float b, float c, float d) { X=a; Y=b; Z=c; W=d; }
	TT_INLINE Quaternion(float angle, const Vector3& axis);
	void Normalize();
	float &	operator [](int i) { return (&X)[i]; }     
	const float &  operator [](int i) const { return (&X)[i]; }

	Quaternion conjugate() { return Quaternion(t, -i, -j, -k); }

	Matrix3 Build_Matrix3();
	Matrix3D Build_Matrix3D();
	Matrix4 Build_Matrix4();

	static Quaternion Trackball(float x0, float y0, float x1, float y1, float sphsize);
	void Scale(float s) { X = (float)(s*X); Y = (float)(s*Y); Z = (float)(s*Z); W = (float)(s*W); }

	friend TT_INLINE Quaternion operator* (const Vector3& a, const Quaternion& b);
	friend TT_INLINE Quaternion operator* (const Quaternion& a, const Quaternion& b);
	bool		Is_Valid(void) const;
	TT_INLINE void		Make_Identity(void) { Set(); };
	TT_INLINE void		Set(float a = 0.0, float b = 0.0, float c = 0.0, float d = 1.0) { X = a; Y = b; Z = c; W = d; }
	TT_INLINE Vector3	Rotate_Vector(const Vector3 & v) const;
};



TT_INLINE Quaternion operator * (const Vector3& a, const Quaternion& b)
{
	return Quaternion(a.X, a.Y, a.Z, 0) * b;
}



TT_INLINE Quaternion operator * (const Quaternion& a, const Quaternion& b)
{
	// Rv = Ar*Bv + Br*Av + Av cross Bv
	// Rr = Ar*Br + Av dot Bv

	return Quaternion
	(
		a.W*b.X + b.W*a.X + (a.Y*b.Z - a.Z*b.Y),
		a.W*b.Y + b.W*a.Y + (a.Z*b.X - a.X*b.Z),
		a.W*b.Z + b.W*a.Z + (a.X*b.Y - a.Y*b.X),
		a.W*b.W - (a.X*b.X + a.Y*b.Y + a.Z*b.Z)
	);
}


TT_INLINE bool Quaternion::Is_Valid(void) const
{
	return (	Is_Valid_Float(X) && 
				Is_Valid_Float(Y) && 
				Is_Valid_Float(Z) &&
				Is_Valid_Float(W) );
}

TT_INLINE Vector3 Quaternion::Rotate_Vector(const Vector3 & v) const
{
	float x = W*v.X + (Y*v.Z - v.Y*Z);
	float y = W*v.Y - (X*v.Z - v.X*Z);
	float z = W*v.Z + (X*v.Y - v.X*Y);
	float w = -(X*v.X + Y*v.Y + Z*v.Z);
	return Vector3
	(
		w*(-X) + W*x + (y*(-Z) - (-Y)*z),
		w*(-Y) + W*y - (x*(-Z) - (-X)*z),
		w*(-Z) + W*z + (x*(-Y) - (-X)*y)
	);
}

TT_INLINE Quaternion Inverse(const Quaternion & a)
{
	return Quaternion(-a[0],-a[1],-a[2],a[3]);
}
Quaternion Build_Quaternion(const Matrix3D & mat);

TT_INLINE Quaternion operator / (const Quaternion & a,const Quaternion & b)
{
	return a * Inverse(b);
}

TT_INLINE Quaternion operator * (float scl, const Quaternion & a)
{
	return Quaternion(scl*a[0], scl*a[1], scl*a[2], scl*a[3]);
}

TT_INLINE Quaternion operator * (const Quaternion & a, float scl)
{
	return scl*a;
}

struct SlerpInfoStruct
{
	float		SinT;
	float		Theta;
	bool		Flip;
	bool		Linear;
};
void Slerp_Setup(const Quaternion & p,const Quaternion & q,SlerpInfoStruct * slerpinfo);
void Cached_Slerp(const Quaternion & p,const Quaternion & q,float alpha,SlerpInfoStruct * slerpinfo,Quaternion * set_q);
Quaternion Cached_Slerp(const Quaternion & p,const Quaternion & q,float alpha,SlerpInfoStruct * slerpinfo);
void __cdecl Fast_Slerp(Quaternion& result, const Quaternion & a,const Quaternion & b,float t);
Matrix3	Build_Matrix3(const Quaternion & quat);
#endif
