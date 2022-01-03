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

#ifndef SCRIPTS_INCLUDE__ENGINE_MATH_H
#define SCRIPTS_INCLUDE__ENGINE_MATH_H

#define RAD2DEG(rad) ((rad) / WWMATH_PI * 180)
#define DEG2RAD(deg) ((deg) / 180.f * WWMATH_PI)
#define divideRoundUp(x, y) (((x) + (y) - 1) / (y))

class Vector3;

inline unsigned long F2DW(float f)
{
	return *((unsigned long*)&f);
}

inline unsigned long F2DW(float* f)
{
	return *((unsigned long*)f);
};

template<typename T> inline T sqr(T value)
{
	return value * value;
}

template<typename T> inline T clamp(T value, T min, T max)
{
	if (value < min)
		return min;

	if (value > max)
		return max;

	return value;
}

template<typename T> inline T wrap(T value, T min, T max)
{
	return (max - min) % (value - min) + min;
}

template<> inline float wrap<float>(float value, float min, float max)
{
	return fmod(max - min, value - min) + min;
}



template<typename T> inline T lerp(T a, T b, float fraction)
{
	return a + (T)((b - a) * fraction);
}



inline bool isPowerOfTwo(int number)
{
	return number != 0 && ((number - 1) & number) == 0;
}



inline uint GetColorUInt(int a, int r, int g, int b)
{
	return ((a & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
}

inline float sgn(float x)
{
	return (x < 0.0f) ? -1.0f : 1.0f;
}

int SCRIPTS_API Find_POT_LT(int i);

/**
 * Computes the intersection point of a ray through the origin and a plane.
 * (If the distance to the plane and the plane's normal are already computed, use the other overload instead.)
 * rayDirection: a unit vector in the ray's direction.
 * planeOrigin: the projection of the origin on the plane.
 * return: the intersection point or (0, 0, 0) if the ray and the plane do not intersect.
 */
Vector3 rayPlaneIntersect(const Vector3& rayDirection, const Vector3& planeOrigin);

/**
 * Computes the intersection point of a ray through the origin and a plane.
 * rayDirection: a unit vector in the ray's direction.
 * planeNormal: a unit vector normal to the plane.
 * planeDistance: the distance from the origin to the plane. Must be positive.
 * return: the intersection point or (0, 0, 0) if the ray and the plane do not intersect.
 */
Vector3 rayPlaneIntersect(const Vector3& rayDirection, const Vector3& planeNormal, const float planeDistance);

#endif
