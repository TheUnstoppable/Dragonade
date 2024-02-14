/*	Renegade Scripts.dll
	Copyright 2022 Tiberian Technologies

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
#include "ScriptedControlClass.h"

class ScriptedMenuDialogClass;
class ScriptedPopupDialogClass;

typedef enum
{
	DIALOGTYPE_POPUP,
	DIALOGTYPE_MENU
} DialogType;

typedef enum
{
	DIALOGORIENTATION_NONE,
	DIALOGORIENTATION_TOPLEFT,
	DIALOGORIENTATION_TOPCENTER,
	DIALOGORIENTATION_TOPRIGHT,
	DIALOGORIENTATION_MIDDLELEFT,
	DIALOGORIENTATION_MIDDLECENTER,
	DIALOGORIENTATION_MIDDLERIGHT,
	DIALOGORIENTATION_BOTTOMLEFT,
	DIALOGORIENTATION_BOTTOMCENTER,
	DIALOGORIENTATION_BOTTOMRIGHT
} DialogOrientation;

class ScriptedDialogClass
{
public:
	ScriptedDialogClass(int client, int dlg_id);
	virtual ~ScriptedDialogClass();

	virtual DialogType Get_Dialog_Type() = 0;
	virtual ScriptedMenuDialogClass* As_ScriptedMenuDialogClass() { return NULL; }
	virtual ScriptedPopupDialogClass* As_ScriptedPopupDialogClass() { return NULL; }

	int Get_Dialog_ID() { return id; }
	int Get_Client_ID() { return clientId; }
	bool Is_Dirty() { return isDirty; }
	void Set_Dirty(bool dirty) { isDirty = dirty; }

	virtual void Add_Control(ScriptedControlClass* control);
	virtual ScriptedControlClass* Create_Control(ControlType type);
	virtual ScriptedControlClass* Find_Control(int id);
	virtual int Get_Control_Count();
	virtual ScriptedControlClass* Get_Control_Index(int index);
	virtual void Remove_Control(int id);
	virtual void Remove_Control(ScriptedControlClass* control);
	virtual void Clear_Controls();
	virtual void Focus_Control(ScriptedControlClass* control);

protected:
	int id;
	int clientId;
	DynamicVectorClass<ScriptedControlClass*> controls;
	bool isDirty;
};

class ScriptedMenuDialogClass : public ScriptedDialogClass
{
public:
	ScriptedMenuDialogClass(int client, int dlg_id);
	virtual ~ScriptedMenuDialogClass();

	virtual DialogType Get_Dialog_Type() { return DIALOGTYPE_MENU; }
	virtual ScriptedMenuDialogClass* As_ScriptedMenuDialogClass() { return this; }
};

class ScriptedPopupDialogClass : public ScriptedDialogClass
{
public:
	ScriptedPopupDialogClass(int client, int dlg_id);
	virtual ~ScriptedPopupDialogClass();

	virtual DialogType Get_Dialog_Type() { return DIALOGTYPE_POPUP; }
	virtual ScriptedPopupDialogClass* As_ScriptedPopupDialogClass() { return this; }

	DialogOrientation Get_Orientation() { return orientation; }
	void Set_Orientation(DialogOrientation newOrientation) { orientation = newOrientation; Set_Dirty(true); }
	const Vector2& Get_Dialog_Size() { return size; }
	void Set_Dialog_Size(Vector2& newSize) { size = newSize; Set_Dirty(true); }
	void Set_Dialog_Size(int width, int height) { size.X = (float)width; size.Y = (float)height; Set_Dirty(true); }
	const Vector2& Get_Dialog_Location() { return location; }
	void Set_Dialog_Location(Vector2& newLocation) { location = newLocation; Set_Dirty(true); }
	void Set_Dialog_Location(int x, int y) { location.X = (float)x; location.Y = (float)y; Set_Dirty(true); }
	const WideStringClass& Get_Dialog_Title() { return title; }
	void Set_Dialog_Title(const wchar_t* newTitle) { title = newTitle; Set_Dirty(true); }

protected:
	Vector2 size;
	Vector2 location;
	DialogOrientation orientation;
	WideStringClass title;
};