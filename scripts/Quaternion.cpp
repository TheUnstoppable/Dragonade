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
#include "Quaternion.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Matrix3.h"
#include "Matrix3D.h"
#include "Matrix4.h"
#include "wwmath.h"


const Quaternion Quaternion::IDENTITY(0.f, 0.f, 0.f, 1.f);



Quaternion::Quaternion(float angle, const Vector3& axis)
{
	float cosAngle = cos(angle / 2.f);
	float sinAngle = sin(angle / 2.f);

	t = cosAngle;
	i = axis.X * sinAngle;
	j = axis.Y * sinAngle;
	k = axis.Z * sinAngle;
}



void Quaternion::Normalize()
{
	float len2=X * X + Y * Y + Z * Z + W * W;
	if (0.0f == len2)
	{
		return;
	}
	else
	{
		float inv_mag = WWMath::Inv_Sqrt(len2);

		X *= inv_mag;
		Y *= inv_mag;
		Z *= inv_mag;
		W *= inv_mag;
	}
}



Matrix3	Quaternion::Build_Matrix3()
{
	return Matrix3
	(
		1.f - 2.f * (j * j + k * k),
		      2.f * (i * j - k * t),
		      2.f * (k * i + j * t),

		      2.f * (i * j + k * t),
		1.f - 2.f * (k * k + i * i),
		      2.f * (j * k - i * t),

		      2.f * (k * i - j * t),
		      2.f * (j * k + i * t),
		1.f - 2.f * (j * j + i * i)
	);
}



Matrix3D Quaternion::Build_Matrix3D()
{
	return Matrix3D(Build_Matrix3(), Vector3(0, 0, 0));
}



Matrix4 Quaternion::Build_Matrix4()
{
	return Matrix4(Build_Matrix3D());
}

float project_to_sphere(float r, float x, float y)
{
	const float SQRT2 = 1.41421356f;
	float t, z;
	float d = WWMath::Sqrt(x * x + y * y);

	if (d < r * (SQRT2/(2.0f)))			// inside sphere
		z = WWMath::Sqrt(r * r - d * d);
	else {								// on hyperbola
		t = r / SQRT2;
		z = t * t / d;
	}

	return z;
}

Quaternion Axis_To_Quat(const Vector3 &a, float phi)
{
	Quaternion q;
	Vector3 tmp = a;  

	tmp.Normalize();
	q[0] = tmp[0];
	q[1] = tmp[1];
	q[2] = tmp[2];

	q.Scale(WWMath::Sin(phi / 2.0f));
	q[3] =  WWMath::Cos(phi / 2.0f);

	return q;
}

Quaternion Quaternion::Trackball(float x0, float y0, float x1, float y1, float sphsize)
{
	Vector3	a;
	Vector3	p1;
	Vector3	p2;
	Vector3	d;	

	float phi,t;

	if ((x0 == x1) && (y0 == y1)) {
		return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);	// Zero rotation
	}


	// Compute z coordinates for projection of p1 and p2 to
	// deformed sphere
	p1[0] = x0;
	p1[1] = y0;
	p1[2] = project_to_sphere(sphsize, x0, y0);

	p2[0] = x1;
	p2[1] = y1;
	p2[2] = project_to_sphere(sphsize, x1, y1);


	// Find their cross product
	Vector3::Cross_Product(p2,p1,&a);

	// Compute how much to rotate
	d = p1 - p2;
	t = d.Length() / (2.0f * sphsize);

	// Avoid problems with out of control values
	if (t >  1.0f) t =  1.0f;
	if (t < -1.0f) t = -1.0f;
	phi = 2.0f * WWMath::Asin(t);

	return Axis_To_Quat(a, phi);
}

static int _nxt[3] = { 1 , 2 , 0 };
Quaternion Build_Quaternion(const Matrix3D & mat)
{
	float tr,s;
	int i,j,k;
	Quaternion q;
	tr = mat[0][0] + mat[1][1] + mat[2][2];
	if (tr > 0.0f)
	{
		s = sqrt(tr + 1.0f);
		q[3] = s * 0.5f;
		s = 0.5f / s;
		q[0] = (mat[2][1] - mat[1][2]) * s;
		q[1] = (mat[0][2] - mat[2][0]) * s;
		q[2] = (mat[1][0] - mat[0][1]) * s;
	}
	else
	{
		i=0;
		if (mat[1][1] > mat[0][0])
		{
			i = 1;
		}
		if (mat[2][2] > mat[i][i])
		{
			i = 2;
		}
		j = _nxt[i];
		k = _nxt[j];
		s = sqrt((mat[i][i] - (mat[j][j] + mat[k][k])) + 1.0f);
		q[i] = s * 0.5f;
		if (s != 0.0f)
		{
			s = 0.5f / s;
		}
		q[3] = ( mat[k][j] - mat[j][k] ) * s;
		q[j] =	( mat[j][i] + mat[i][j] ) * s;    
		q[k] =	( mat[k][i] + mat[i][k] ) * s;
	}
	return q;
}

#define SLERP_EPSILON		0.001
void Slerp_Setup(const Quaternion & p,const Quaternion & q,SlerpInfoStruct * slerpinfo)
{
	float cos_t;
	cos_t = p.X * q.X + p.Y * q.Y + p.Z * q.Z + p.W * q.W;
	if (cos_t < 0.0f)
	{
		cos_t = -cos_t;
		slerpinfo->Flip = true;
	}
	else
	{
		slerpinfo->Flip = false;
	}
	if (1.0f - cos_t < SLERP_EPSILON)
	{
		slerpinfo->Linear = true;
		slerpinfo->Theta = 0.0f;
		slerpinfo->SinT = 0.0f;
	}
	else
	{
		slerpinfo->Linear = false;
		slerpinfo->Theta = WWMath::Acos(cos_t);
		slerpinfo->SinT = WWMath::Sin(slerpinfo->Theta);
	}
}
Quaternion Cached_Slerp(const Quaternion & p,const Quaternion & q,float alpha,SlerpInfoStruct * slerpinfo)
{
	float beta;
	float oo_sin_t;
	if (slerpinfo->Linear)
	{
		beta = 1.0f - alpha;
	}
	else
	{
		oo_sin_t = 1.0f / slerpinfo->Theta;
		beta = WWMath::Sin(slerpinfo->Theta - alpha*slerpinfo->Theta) * oo_sin_t;
		alpha = WWMath::Sin(alpha*slerpinfo->Theta) * oo_sin_t;
	}
	if (slerpinfo->Flip)
	{
		alpha = -alpha;
	}
	Quaternion res;
	res.X = beta*p.X + alpha*q.X;
	res.Y = beta*p.Y + alpha*q.Y;
	res.Z = beta*p.Z + alpha*q.Z;
	res.W = beta*p.W + alpha*q.W;
	return res;
}
void Cached_Slerp(const Quaternion & p,const Quaternion & q,float alpha,SlerpInfoStruct * slerpinfo,Quaternion * set_q)
{
	float beta;
	float oo_sin_t;
	if (slerpinfo->Linear)
	{
		beta = 1.0f - alpha;
	}
	else
	{
		oo_sin_t = 1.0f / slerpinfo->Theta;
		beta = WWMath::Sin(slerpinfo->Theta - alpha*slerpinfo->Theta) * oo_sin_t;
		alpha = WWMath::Sin(alpha*slerpinfo->Theta) * oo_sin_t;
	}
	if (slerpinfo->Flip)
	{
		alpha = -alpha;
	}
	set_q->X = beta*p.X + alpha*q.X;
	set_q->Y = beta*p.Y + alpha*q.Y;
	set_q->Z = beta*p.Z + alpha*q.Z;
	set_q->W = beta*p.W + alpha*q.W;
}

void __cdecl Fast_Slerp(Quaternion& res, const Quaternion & p,const Quaternion & q,float alpha)
{		
	float beta;			// complementary interploation parameter
	float theta;		// angle between p and q
	float cos_t; 		// sine, cosine of theta
	float oo_sin_t;
	int qflip;			// use flip of q?
	
	// cos theta = dot product of p and q
	cos_t = p.X * q.X + p.Y * q.Y + p.Z * q.Z + p.W * q.W;

	// if q is on opposite hemisphere from A, use -B instead
	if (cos_t < 0.0f) {
		cos_t = -cos_t;
		qflip = true;
	} else {
		qflip = false;
	}

	if (1.0f - cos_t < WWMATH_EPSILON * WWMATH_EPSILON) {

		// if q is very close to p, just linearly interpolate
		// between the two.
		beta = 1.0f - alpha;

	} else {

		theta = WWMath::Fast_Acos(cos_t);
		float sin_t = WWMath::Fast_Sin(theta);
		oo_sin_t = 1.0f / sin_t;
		beta = WWMath::Fast_Sin(theta - alpha*theta) * oo_sin_t;
		alpha = WWMath::Fast_Sin(alpha*theta) * oo_sin_t;
	}

	if (qflip) {
		alpha = -alpha;
	}

	res.X = beta*p.X + alpha*q.X;
	res.Y = beta*p.Y + alpha*q.Y;
	res.Z = beta*p.Z + alpha*q.Z;
	res.W = beta*p.W + alpha*q.W;
}

Matrix3	Build_Matrix3(const Quaternion & q)
{
	Matrix3 m;

	m[0][0] = (float)(1.0 - 2.0 * (q[1] * q[1] + q[2] * q[2]));
	m[0][1] = (float)(2.0 * (q[0] * q[1] - q[2] * q[3]));
	m[0][2] = (float)(2.0 * (q[2] * q[0] + q[1] * q[3]));

	m[1][0] = (float)(2.0 * (q[0] * q[1] + q[2] * q[3]));
	m[1][1] = (float)(1.0 - 2.0f * (q[2] * q[2] + q[0] * q[0]));
	m[1][2] = (float)(2.0 * (q[1] * q[2] - q[0] * q[3]));

	m[2][0] = (float)(2.0 * (q[2] * q[0] - q[1] * q[3]));
	m[2][1] = (float)(2.0 * (q[1] * q[2] + q[0] * q[3]));
	m[2][2] =(float)(1.0 - 2.0 * (q[1] * q[1] + q[0] * q[0]));

	return m;
}
