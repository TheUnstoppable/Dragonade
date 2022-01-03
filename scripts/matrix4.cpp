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
#include "Matrix4.h"
#include "plane.h"

const Matrix4 Matrix4::IDENTITY = Matrix4(Vector4(1,0,0,0),
										  Vector4(0,1,0,0),
										  Vector4(0,0,1,0),
										  Vector4(0,0,0,1));


void Matrix4::Multiply(const Matrix4 &a,const Matrix4 &b,Matrix4 * res)
{
	#define ROWCOL(i,j) a[i][0]*b[0][j] + a[i][1]*b[1][j] + a[i][2]*b[2][j] + a[i][3]*b[3][j]
	(*res)[0][0] = ROWCOL(0,0);
	(*res)[0][1] = ROWCOL(0,1);
	(*res)[0][2] = ROWCOL(0,2);
	(*res)[0][3] = ROWCOL(0,3);
	(*res)[1][0] = ROWCOL(1,0);
	(*res)[1][1] = ROWCOL(1,1);
	(*res)[1][2] = ROWCOL(1,2);
	(*res)[1][3] = ROWCOL(1,3);
	(*res)[2][0] = ROWCOL(2,0);
	(*res)[2][1] = ROWCOL(2,1);
	(*res)[2][2] = ROWCOL(2,2);
	(*res)[2][3] = ROWCOL(2,3);
	(*res)[3][0] = ROWCOL(3,0);
	(*res)[3][1] = ROWCOL(3,1);
	(*res)[3][2] = ROWCOL(3,2);
	(*res)[3][3] = ROWCOL(3,3);
	#undef ROWCOL
}
void Matrix4::Multiply(const Matrix3D &a,const Matrix4 &b,Matrix4 * res)
{
	#define ROWCOL(i,j) a[i][0]*b[0][j] + a[i][1]*b[1][j] + a[i][2]*b[2][j] + a[i][3]*b[3][j]
	(*res)[0][0] = ROWCOL(0,0);
	(*res)[0][1] = ROWCOL(0,1);
	(*res)[0][2] = ROWCOL(0,2);
	(*res)[0][3] = ROWCOL(0,3);
	(*res)[1][0] = ROWCOL(1,0);
	(*res)[1][1] = ROWCOL(1,1);
	(*res)[1][2] = ROWCOL(1,2);
	(*res)[1][3] = ROWCOL(1,3);
	(*res)[2][0] = ROWCOL(2,0);
	(*res)[2][1] = ROWCOL(2,1);
	(*res)[2][2] = ROWCOL(2,2);
	(*res)[2][3] = ROWCOL(2,3);
	(*res)[3][0] = b[3][0];
	(*res)[3][1] = b[3][1];
	(*res)[3][2] = b[3][2];
	(*res)[3][3] = b[3][3];
	#undef ROWCOL
}
void Matrix4::Multiply(const Matrix4 & a,const Matrix3D & b,Matrix4 * res)
{
	#define ROWCOL(i,j) a[i][0]*b[0][j] + a[i][1]*b[1][j] + a[i][2]*b[2][j]
	#define ROWCOL4(i,j) a[i][0]*b[0][j] + a[i][1]*b[1][j] + a[i][2]*b[2][j] + a[i][3]
	(*res)[0][0] = ROWCOL(0,0);
	(*res)[0][1] = ROWCOL(0,1);
	(*res)[0][2] = ROWCOL(0,2);
	(*res)[0][3] = ROWCOL4(0,3);
	(*res)[1][0] = ROWCOL(1,0);
	(*res)[1][1] = ROWCOL(1,1);
	(*res)[1][2] = ROWCOL(1,2);
	(*res)[1][3] = ROWCOL4(1,3);
	(*res)[2][0] = ROWCOL(2,0);
	(*res)[2][1] = ROWCOL(2,1);
	(*res)[2][2] = ROWCOL(2,2);
	(*res)[2][3] = ROWCOL4(2,3);
	(*res)[3][0] = ROWCOL(3,0);
	(*res)[3][1] = ROWCOL(3,1);
	(*res)[3][2] = ROWCOL(3,2);
	(*res)[3][3] = ROWCOL4(3,3);
	#undef ROWCOL
	#undef ROWCOL4
}

Matrix4 Matrix4::ReflectPlane(const PlaneClass& _plane)
{
	Matrix4 temp;
	PlaneClass plane = _plane;
	plane.Normalize();
	temp[0][0] = -2 * plane.N.X * plane.N.X + 1;	temp[0][1] = -2 * plane.N.Y * plane.N.X;		temp[0][2] = -2 * plane.N.Z * plane.N.X;		temp[0][3] = 0;
	temp[1][0] = -2 * plane.N.X * plane.N.Y;		temp[1][1] = -2 * plane.N.Y * plane.N.Y + 1;	temp[1][2] = -2 * plane.N.Z * plane.N.Y;		temp[1][3] = 0;
	temp[2][0] = -2 * plane.N.X * plane.N.Z;		temp[2][1] = -2 * plane.N.Y * plane.N.Z;		temp[2][2] = -2 * plane.N.Z * plane.N.Z + 1;	temp[2][3] = 0;
	temp[3][0] = -2 * plane.N.X * plane.D;			temp[3][1] = -2 * plane.N.Y * plane.D;			temp[3][2] = -2 * plane.N.Z * plane.D;			temp[3][3] = 1;
	return temp;
};