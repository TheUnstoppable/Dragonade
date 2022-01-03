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
#ifndef TT__INCLUDE__WWMATH_H
#define TT__INCLUDE__WWMATH_H
#include <math.h>
#include <float.h>
#define WWMATH_EPSILON		0.0001f
#define WWMATH_EPSILON2		WWMATH_EPSILON * WWMATH_EPSILON
#define WWMATH_PI				3.141592654f
#define WWMATH_FLOAT_MAX	(FLT_MAX)
#define WWMATH_FLOAT_MIN	(FLT_MIN)
#define WWMATH_FLOAT_TINY	(1.0e-37f)
#define WWMATH_SQRT2			1.414213562f
#define WWMATH_SQRT3			1.732050808f
#define WWMATH_OOSQRT2		0.707106781f
#define WWMATH_OOSQRT3		0.577350269f
#ifndef RAD_TO_DEG
#define RAD_TO_DEG(x)	(((double)x)*180.0/WWMATH_PI)
#endif
#ifndef DEG_TO_RAD
#define DEG_TO_RAD(x)	(((double)x)*WWMATH_PI/180.0)
#endif
#ifndef RAD_TO_DEGF
#define RAD_TO_DEGF(x)	(((float)x)*180.0f/WWMATH_PI)
#endif
#ifndef DEG_TO_RADF
#define DEG_TO_RADF(x)	(((float)x)*WWMATH_PI/180.0f)
#endif
const int ARC_TABLE_SIZE=1024;
const int SIN_TABLE_SIZE=1024;
#ifndef EXTERNAL
SCRIPTS_API extern REF_ARR_DECL(float, _FastAcosTable, ARC_TABLE_SIZE);
SCRIPTS_API extern REF_ARR_DECL(float, _FastAsinTable, ARC_TABLE_SIZE);
SCRIPTS_API extern REF_ARR_DECL(float, _FastSinTable, SIN_TABLE_SIZE);
SCRIPTS_API extern REF_ARR_DECL(float, _FastInvSinTable, SIN_TABLE_SIZE);
#else
extern float _FastAcosTable[ARC_TABLE_SIZE];
extern float _FastAsinTable[ARC_TABLE_SIZE];
extern float _FastSinTable[SIN_TABLE_SIZE];
extern float _FastInvSinTable[SIN_TABLE_SIZE];
#endif
class SCRIPTS_API WWMath
{
public:
	static void			Init(void);
	static void			Shutdown(void);
	static TT_INLINE float Fabs(float val)
	{
		int value=*(int*)&val;
		value&=0x7fffffff;
		return *(float*)&value;
	}
	static TT_INLINE int Float_To_Int_Chop(const float& f);
	static TT_INLINE int Float_To_Int_Floor(const float& f);
	static TT_INLINE float Cos(float val);
	static TT_INLINE float Sin(float val);
	static TT_INLINE float Sqrt(float val);
	static float __fastcall Inv_Sqrt(float a);
	static TT_INLINE long	 Float_To_Long(float f);
	static TT_INLINE float Fast_Sin(float val);
	static TT_INLINE float Fast_Inv_Sin(float val);
	static TT_INLINE float Fast_Cos(float val);
	static TT_INLINE float Fast_Inv_Cos(float val);
	static TT_INLINE float Fast_Acos(float val);
	static TT_INLINE float Fast_Asin(float val);
	static TT_INLINE float Acos(float val);
	static TT_INLINE float Asin(float val);
	static float Atan(float x) { return static_cast<float>(atan(x)); }
	static float Atan2(float y,float x) { return static_cast<float>(atan2(y,x)); }
	static float Sign(float val);
	static float Ceil(float val) { return ceilf(val); }
	static float Floor(float val) { return floorf(val); }
	static bool Fast_Is_Float_Positive(const float & val);
	static float Random_Float(void);
	static float Random_Float(float min,float max);
	static float Clamp(float val, float min = 0.0f, float max = 1.0f);
	static double Clamp(double val, double min = 0.0f, double max = 1.0f);
	static int Clamp_Int(int val, int min_val, int max_val);
	static float Wrap(float val, float min = 0.0f, float max = 1.0f);
	static double Wrap(double val, double min = 0.0f, double max = 1.0f);
	static float Min(float a, float b);
	static float Max(float a, float b);
	static float Lerp(float a, float b, float lerp );
	static double Lerp(double a, double b, float lerp );
	static int Lerp(int a, int b, float lerp );
	static long Float_To_Long(double f);
	static unsigned char Unit_Float_To_Byte(float f) { return (unsigned char)(f*255.0f); }
	static float Byte_To_Unit_Float(unsigned char byte) { return ((float)byte) / 255.0f; }
	static bool Is_Valid_Float(float x);
	static bool Is_Valid_Double(double x);
};
TT_INLINE float WWMath::Sign(float val) 
{ 
	if (val > 0.0f) { 
		return +1.0f; 
	} 
	if (val < 0.0f) { 
		return -1.0f; 
	} 
	return 0.0f; 
}

TT_INLINE bool WWMath::Fast_Is_Float_Positive(const float & val)
{
	return !((*(int *)(&val)) & 0x80000000);
}

TT_INLINE float WWMath::Random_Float(float min,float max) 
{ 
	return Random_Float() * (max-min) + min; 
}

TT_INLINE float WWMath::Clamp(float val, float min, float max) 
{
	if(val < min) return min;
	if(val > max) return max;
	return val;
}

TT_INLINE double WWMath::Clamp(double val, double min, double max) 
{
	if(val < min) return min;
	if(val > max) return max;
	return val;
}

TT_INLINE int WWMath::Clamp_Int(int val, int min_val, int max_val) 
{
	if(val < min_val) return min_val;
	if(val > max_val) return max_val;
	return val;
}

TT_INLINE float WWMath::Wrap(float val, float min, float max) 
{
	if ( val >= max )	val -= (max-min);
	if ( val < min )	val += (max-min);
	if ( val < min ) {
		val = min;
	}
	if ( val > max ) {
		val = max;
	}
	return val;
}

TT_INLINE double WWMath::Wrap(double val, double min, double max) 
{
	if ( val >= max )	val -= (max-min);
	if ( val < min )	val += (max-min);
	if ( val < min ) {
		val = min;
	}
	if ( val > max ) {
		val = max;
	}
	return val;
}

TT_INLINE float WWMath::Min(float a, float b)
{
	if (a<b) return a;
	return b;
}

TT_INLINE float WWMath::Max(float a, float b)
{
	if (a>b) return a;
	return b;
}

TT_INLINE float WWMath::Lerp(float a, float b, float lerp )
{
	return (a + (b - a)*lerp);
}

TT_INLINE double WWMath::Lerp(double a, double b, float lerp )
{
	return (a + (b - a)*lerp);
}

TT_INLINE int WWMath::Lerp(int a, int b, float lerp )
{
	return (a + (int)((b - a)*lerp));
}

TT_INLINE bool WWMath::Is_Valid_Float(float x)
{
	unsigned long * plong = (unsigned long *)(&x);
	unsigned long exponent = ((*plong) & 0x7F800000) >> (32-9);
	if (exponent == 0xFF) {
		return false;
	}
	return true;
}

TT_INLINE bool WWMath::Is_Valid_Double(double x)
{
	unsigned long * plong = (unsigned long *)(&x) + 1;
	unsigned long exponent = ((*plong) & 0x7FF00000) >> (32-12);
	if (exponent == 0x7FF) {
		return false;
	}
	return true;
}

TT_INLINE long WWMath::Float_To_Long(float f)
{
    return _mm_cvtt_ss2si(_mm_load_ss(&f));
}

TT_INLINE long WWMath::Float_To_Long(double f)	
{
    return _mm_cvttsd_si32(_mm_load_pd(&f));
}

TT_INLINE float WWMath::Cos(float val)
{
    return ::cos(val);
}

TT_INLINE float WWMath::Sin(float val)
{
    return ::sin(val);
}

TT_INLINE float WWMath::Fast_Sin(float val)
{
	val*=float(SIN_TABLE_SIZE) / (2.0f * WWMATH_PI);
	int idx0=Float_To_Int_Floor(val);
	int idx1=idx0+1;
	float frac=val-(float)idx0;
	idx0 = ((unsigned)idx0) & (SIN_TABLE_SIZE-1);
	idx1 = ((unsigned)idx1) & (SIN_TABLE_SIZE-1);
	return (1.0f - frac) * _FastSinTable[idx0] + frac * _FastSinTable[idx1];
}

TT_INLINE float WWMath::Fast_Inv_Sin(float val)
{
	return 1.0f / WWMath::Fast_Sin(val);
}

TT_INLINE float WWMath::Fast_Cos(float val)
{
	val+=(WWMATH_PI * 0.5f);
	val*=float(SIN_TABLE_SIZE) / (2.0f * WWMATH_PI);
	int idx0=Float_To_Int_Floor(val);
	int idx1=idx0+1;
	float frac=val-(float)idx0;
	idx0 = ((unsigned)idx0) & (SIN_TABLE_SIZE-1);
	idx1 = ((unsigned)idx1) & (SIN_TABLE_SIZE-1);
	return (1.0f - frac) * _FastSinTable[idx0] + frac * _FastSinTable[idx1];
}

TT_INLINE float WWMath::Fast_Inv_Cos(float val)
{
	return 1.0f / WWMath::Fast_Cos(val);
}

TT_INLINE float WWMath::Fast_Acos(float val)
{
	if (WWMath::Fabs(val) > 0.975f) {
		return WWMath::Acos(val);
	}
	val*=float(ARC_TABLE_SIZE/2);
	int idx0=Float_To_Int_Floor(val);
	int idx1=idx0+1;
	float frac=val-(float)idx0;
	idx0+=ARC_TABLE_SIZE/2;
	idx1+=ARC_TABLE_SIZE/2;
	TT_ASSERT((idx0 >= 0) && (idx0 < ARC_TABLE_SIZE));
	TT_ASSERT((idx1 >= 0) && (idx1 < ARC_TABLE_SIZE));
	return (1.0f - frac) * _FastAcosTable[idx0] + frac * _FastAcosTable[idx1];
}

TT_INLINE float WWMath::Acos(float val)
{
	return (float)acos(val);
}

TT_INLINE float WWMath::Fast_Asin(float val)
{
	if (WWMath::Fabs(val) > 0.975f) {
		return WWMath::Asin(val);
	}
	val*=float(ARC_TABLE_SIZE/2);
	int idx0=Float_To_Int_Floor(val); 
	int idx1=idx0+1;
	float frac=val-(float)idx0;
	idx0+=ARC_TABLE_SIZE/2;
	idx1+=ARC_TABLE_SIZE/2;
	TT_ASSERT((idx0 >= 0) && (idx0 < ARC_TABLE_SIZE));
	TT_ASSERT((idx1 >= 0) && (idx1 < ARC_TABLE_SIZE));
	return (1.0f - frac) * _FastAsinTable[idx0] + frac * _FastAsinTable[idx1];
}
TT_INLINE float WWMath::Asin(float val)
{
	return (float)asin(val);
}

TT_INLINE float WWMath::Sqrt(float val)
{
    return ::sqrtf(val);
}

TT_INLINE int WWMath::Float_To_Int_Chop(const float& f)
{
    int a = *reinterpret_cast<const int*>(&f);
    int sign = (a>>31);
    int mantissa = (a&((1<<23)-1))|(1<<23);
    int exponent = ((a&0x7fffffff)>>23)-127;
    int r = ((unsigned int)(mantissa)<<8)>>(31-exponent);
    return ((r ^ (sign)) - sign ) &~ (exponent>>31);
}

TT_INLINE int WWMath::Float_To_Int_Floor (const float& f)
{
	int a = *reinterpret_cast<const int*>(&f);
	int sign = (a>>31);
	a&=0x7fffffff;
	int exponent = (a>>23)-127;
	int expsign	= ~(exponent>>31);
	int imask = ( (1<<(31-(exponent))))-1;
	int mantissa = (a&((1<<23)-1));
	int r = ((unsigned int)(mantissa|(1<<23))<<8)>>(31-exponent);
#pragma warning(suppress: 6290) //warning C6290: Bitwise operation on logical result: ! has higher precedence than &. Use && or (!(x & y)) instead
	r = ((r & expsign) ^ (sign)) + ((!((mantissa<<8)&imask)&(expsign^((a-1)>>31)))&sign);
	return r;
}

inline float WWMath::Inv_Sqrt(float number)
{
	return 1.f / sqrt(number);
}

#endif
