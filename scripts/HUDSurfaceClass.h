/*	Renegade Scripts.dll
	Copyright 2023 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#pragma once

#include "engine_string.h"
#include "engine_vector.h"
#include "vector2.h"
#include "vector4.h"
#include "rect.h"

typedef enum {
	HUD_ELEMENT_LINE,
	HUD_ELEMENT_RECTANGLE,
	HUD_ELEMENT_OUTLINE,
	HUD_ELEMENT_TEXT,
	HUD_ELEMENT_MAX // Do NOT use!
} HUDElementType;

typedef enum {
	HUD_FONT_TITLE,
	HUD_FONT_BIG_HEADER,
	HUD_FONT_SMALL_HEADER,
	HUD_FONT_BIG_BODY,
	HUD_FONT_NORMAL_BODY,
	HUD_FONT_SUBTITLE,
	HUD_FONT_MAX // Do NOT use!
} HUDFontType;

class HUDElementClass;
class HUDLineElementClass;
class HUDRectangleElementClass;
class HUDOutlineElementClass;
class HUDTextElementClass;

class HUDSurfaceClass
{
public:
	HUDSurfaceClass(int client, int srf_id);
	virtual ~HUDSurfaceClass();

	int Get_Surface_ID() const { return id; }
	int Get_Client_ID() const { return clientId; }
	bool Is_Dirty() const { return isDirty; }
	void Set_Dirty(bool dirty) { isDirty = dirty; }
	const RectClass& Get_Boundary_Area() const { return boundary; }
	float Get_Aspect_Ratio() const { return boundary.Height() > WWMATH_EPSILON ? boundary.Width() / boundary.Height() : 0; }
	const StringClass& Get_Surface_Texture() const { return surfaceTexture; }
	void Set_Surface_Texture(const char* texture) { surfaceTexture = texture; Set_Dirty(true); }

	virtual void Add_Element(HUDElementClass* element);
	virtual HUDElementClass* Create_Element(HUDElementType type);
	virtual HUDElementClass* Find_Element(int id);
	virtual int Get_Element_Count();
	virtual HUDElementClass* Get_Element_Index(int index);
	virtual void Remove_Element(int id);
	virtual void Remove_Element(HUDElementClass* element);
	virtual void Clear_Elements();

protected:
	int id;
	int clientId;
	RectClass boundary;
	StringClass surfaceTexture;
	DynamicVectorClass<HUDElementClass*> elements;
	bool isDirty;
};

class HUDElementClass
{
public:
	HUDElementClass(int elem_id);
	virtual ~HUDElementClass();

	virtual HUDElementType Get_Element_Type() const = 0;
	virtual HUDLineElementClass* As_HUDLineElementClass() { return NULL; }
	virtual HUDRectangleElementClass* As_HUDRectangleElementClass() { return NULL; }
	virtual HUDOutlineElementClass* As_HUDOutlineElementClass() { return NULL; }
	virtual HUDTextElementClass* As_HUDTextElementClass() { return NULL; }

	int Get_Element_ID() const { return elementId; }
	bool Is_Dirty() const { return isDirty; }
	void Set_Dirty(bool dirty) { isDirty = dirty; }
	bool Is_Rendered() const { return isRendered; }
	void Set_Rendered(bool rendered) { isRendered = rendered; Set_Dirty(true); }
	const RectClass& Get_UV_Range() const { return uvRange; }
	void Set_UV_Range(const RectClass& newRange) { uvRange = newRange; Set_Dirty(true); }

protected:
	int elementId;
	bool isDirty;
	bool isRendered;
	RectClass uvRange;
};

class HUDLineElementClass : public HUDElementClass {
public:
	HUDLineElementClass(int elem_id);

	virtual HUDElementType Get_Element_Type() const { return HUD_ELEMENT_LINE; }
	virtual HUDLineElementClass* As_HUDLineElementClass() { return this; }

	const Vector2& Get_P0() const { return p0; }
	void Set_P0(const Vector2& newP0) { p0 = newP0; Set_Dirty(true); }
	const Vector2& Get_P1() const { return p1; }
	void Set_P1(const Vector2& newP1) { p1 = newP1; Set_Dirty(true); }
	float Get_Length() const { return Vector2::Distance(p1, p0); }
	float Get_Thickness() const { return thickness; }
	void Set_Thickness(float newThickness) { thickness = newThickness; Set_Dirty(true); }
	const Vector4& Get_Color() const { return color; }
	void Set_Color(const Vector4& newColor) { color = newColor; Set_Dirty(true); }
	void Set_Color(int a, int r, int g, int b) { color = Vector4(a / 255.f, r / 255.f, g / 255.f, b / 255.f); Set_Dirty(true); }

protected:
	Vector2 p0;
	Vector2 p1;
	float thickness;
	Vector4 color;
};

class HUDRectangleElementClass : public HUDElementClass {
public:
	HUDRectangleElementClass(int elem_id);

	virtual HUDElementType Get_Element_Type() const { return HUD_ELEMENT_RECTANGLE; }
	virtual HUDRectangleElementClass* As_HUDRectangleElementClass() { return this; }

	const RectClass& Get_Rect() const { return rect; }
	void Set_Rect(const RectClass& newRect) { rect = newRect; Set_Dirty(true); }
	float Get_Area() const { const Vector2& a = rect.getSize(); return a.X * a.Y; }
	float Get_Diagonal_Length() const { return rect.Extent().Length() * 2; }
	const Vector4& Get_Color() const { return color; }
	void Set_Color(const Vector4& newColor) { color = newColor; Set_Dirty(true); }
	void Set_Color(int a, int r, int g, int b) { color = Vector4(a / 255.f, r / 255.f, g / 255.f, b / 255.f); Set_Dirty(true); }

protected:
	RectClass rect;
	Vector4 color;
};

class HUDOutlineElementClass : public HUDElementClass {
public:
	HUDOutlineElementClass(int elem_id);

	virtual HUDElementType Get_Element_Type() const { return HUD_ELEMENT_OUTLINE; }
	virtual HUDOutlineElementClass* As_HUDOutlineElementClass() { return this; }

	const RectClass& Get_Rect() const { return rect; }
	void Set_Rect(const RectClass& newRect) { rect = newRect; Set_Dirty(true); }
	float Get_Area() const { const Vector2& a = rect.getSize(); return a.X * a.Y; }
	float Get_Diagonal_Length() const { return rect.Extent().Length() * 2; }
	float Get_Thickness() const { return thickness; }
	void Set_Thickness(float newThickness) { thickness = newThickness; Set_Dirty(true); }
	const Vector4& Get_Color() const { return color; }
	void Set_Color(const Vector4& newColor) { color = newColor; Set_Dirty(true); }
	void Set_Color(int a, int r, int g, int b) { color = Vector4(a / 255.f, r / 255.f, g / 255.f, b / 255.f); Set_Dirty(true); }

protected:
	RectClass rect;
	float thickness;
	Vector4 color;
};

class HUDTextElementClass : public HUDElementClass {
public:
	HUDTextElementClass(int elem_id);

	virtual HUDElementType Get_Element_Type() const { return HUD_ELEMENT_TEXT; }
	virtual HUDTextElementClass* As_HUDTextElementClass() { return this; }

	const WideStringClass& Get_Text() const { return text; }
	void Set_Text(const wchar_t* newText) { text = newText; Set_Dirty(true); }
	HUDFontType Get_Font() const { return font; }
	void Set_Font(HUDFontType newFont) { font = newFont; Set_Dirty(true); }
	const RectClass& Get_Clipping_Area() const { return clipArea; }
	void Set_Clipping_Area(const RectClass& newArea) { clipArea = newArea; Set_Dirty(true); }
	float Get_Clipping_Area_Size() const { const Vector2& a = clipArea.getSize(); return a.X * a.Y; }
	float Get_Clipping_Area_Diagonal_Length() const { return clipArea.Extent().Length() * 2; }
	const Vector4& Get_Color() const { return color; }
	void Set_Color(const Vector4& newColor) { color = newColor; Set_Dirty(true); }
	void Set_Color(int a, int r, int g, int b) { color = Vector4(a / 255.f, r / 255.f, g / 255.f, b / 255.f); Set_Dirty(true); }

protected:
	WideStringClass text;
	HUDFontType font;
	RectClass clipArea;
	Vector4 color;
};