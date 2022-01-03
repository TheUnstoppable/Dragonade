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
#ifndef TT_INCLUDE__RECT_H
#define TT_INCLUDE__RECT_H
#include "Vector2.h"
class RectClass {
public:
	float Left;
	float Top;
	float Right;
	float Bottom;
	RectClass( const RectClass & r )	{ Left = r.Left; Top = r.Top; Right = r.Right; Bottom = r.Bottom; }
	RectClass( const Vector2 & top_left, const Vector2 & bottom_right )	{ Left = top_left.X; Top = top_left.Y; Right = bottom_right.X; Bottom = bottom_right.Y; }
	RectClass& operator = (const RectClass & r) { Left = r.Left; Top = r.Top; Right = r.Right; Bottom = r.Bottom; return *this; }	
	void Set(float left, float top, float right, float bottom) { Left = left; Top = top; Right = right; Bottom = bottom; }
	void Set( const Vector2 & top_left, const Vector2 & bottom_right )	{ Left = top_left.X; Top = top_left.Y; Right = bottom_right.X; Bottom = bottom_right.Y; }
	void Set(const RectClass & r) { Left = r.Left; Top = r.Top; Right = r.Right; Bottom = r.Bottom; }
	Vector2 Upper_Right( void ) const { return Vector2( Right, Top ); }
	Vector2 Lower_Left( void ) const { return Vector2( Left, Bottom ); }
	float Width(void) const		{ return Right - Left; }
	float Height(void) const	{ return Bottom - Top; }
	Vector2 Center( void ) const { return Vector2( (Left + Right)/2, (Top + Bottom)/2 ); }
	Vector2 Extent( void ) const { return Vector2( (Right - Left)/2, (Bottom - Top)/2 ); }
	RectClass & operator *= (float k) { return Scale( k ); }
	RectClass & operator /= (float k) { return Scale( 1/k ); }
	RectClass & Scale_Relative_Center( float k )	{ Vector2 center = Center(); *this-=center; Left*=k; Top*=k; Right*=k; Bottom*=k; *this+=center; return *this; }
	RectClass & Scale( float k )				{ Left*=k; Top*=k; Right*=k; Bottom*=k; return *this; }
	RectClass & Scale( const Vector2 &k )				{ Left*=k.X; Top*=k.Y; Right*=k.X; Bottom*=k.Y; return *this; }
	RectClass & Inverse_Scale( const Vector2 &k )	{ Left/=k.X; Top/=k.Y; Right/=k.X; Bottom/=k.Y; return *this; }
	RectClass & operator += ( const Vector2 & o ) { Left+=o.X; Top+=o.Y; Right+=o.X; Bottom+=o.Y; return *this; }
	RectClass & operator -= ( const Vector2 & o ) { Left-=o.X; Top-=o.Y; Right-=o.X; Bottom-=o.Y; return *this; }
	void Inflate( const Vector2 & o ) { Left-=o.X; Top-=o.Y; Right+=o.X; Bottom+=o.Y; }
	RectClass & operator += ( const RectClass & r ) { Left=min(Left,r.Left); Top=min(Top,r.Top); Right=max(Right,r.Right); Bottom=max(Bottom,r.Bottom); return *this; }
	RectClass(float NewLeft,float NewTop,float NewRight,float NewBottom)
	{
		Left = NewLeft;
		Top = NewTop;
		Right = NewRight;
		Bottom = NewBottom;
	}
	RectClass()
	{
	}
	Vector2 Upper_Left() const
	{
		return Vector2(Left,Top);
	}
	Vector2 Lower_Right() const
	{
		return Vector2(Right,Bottom);
	}
	bool operator == ( const RectClass &rval ) const { return (rval.Left == Left) && (rval.Right == Right) && (rval.Top == Top) && (rval.Bottom == Bottom); }
	bool operator != ( const RectClass &rval ) const { return (rval.Left != Left) || (rval.Right != Right) || (rval.Top != Top) || (rval.Bottom != Bottom); }
	bool	Contains ( const Vector2 &pos ) const { return (pos.X >= Left) && (pos.X <= Right) && (pos.Y >= Top) && (pos.Y <= Bottom); }
	void	Snap_To_Units( const Vector2 & u ) { Left = (int)(Left / u.X + 0.5f) * u.X; Right = (int)(Right / u.X + 0.5f) * u.X; Top = (int)(Top / u.Y + 0.5f) * u.Y; Bottom = (int)(Bottom / u.Y + 0.5f) * u.Y; }

	Vector2 getSize() const { return Vector2(Right - Left, Bottom - Top); }
};

#endif
