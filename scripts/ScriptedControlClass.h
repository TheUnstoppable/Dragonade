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

#include "engine_vector.h"
#include "vector2.h"

class ScriptedDialogClass;
class ScriptedLabelControlClass;
class ScriptedImageControlClass;
class ScriptedButtonControlClass;
class ScriptedTextAreaControlClass;
class ScriptedCheckBoxControlClass;
class ScriptedComboBoxControlClass;
class ScriptedSliderControlClass;
class ScriptedProgressBarControlClass;
class ScriptedHealthBarControlClass;

typedef enum
{
	CONTROLTYPE_LABEL = 1,
	CONTROLTYPE_IMAGE,
	CONTROLTYPE_BUTTON,
	CONTROLTYPE_TEXTAREA,
	CONTROLTYPE_CHECKBOX,
	CONTROLTYPE_COMBOBOX,
	CONTROLTYPE_SLIDER,
	CONTROLTYPE_PROGRESSBAR,
	CONTROLTYPE_HEALTHBAR
} ControlType;

class ScriptedControlClass
{
public:
	ScriptedControlClass(int ctl_id);
	virtual ~ScriptedControlClass();

	virtual ControlType Get_Control_Type() = 0;
	virtual ScriptedLabelControlClass* As_ScriptedLabelControlClass() { return NULL; }
	virtual ScriptedImageControlClass* As_ScriptedImageControlClass() { return NULL; }
	virtual ScriptedButtonControlClass* As_ScriptedButtonControlClass() { return NULL; }
	virtual ScriptedTextAreaControlClass* As_ScriptedTextAreaControlClass() { return NULL; }
	virtual ScriptedCheckBoxControlClass* As_ScriptedCheckBoxControlClass() { return NULL; }
	virtual ScriptedComboBoxControlClass* As_ScriptedComboBoxControlClass() { return NULL; }
	virtual ScriptedSliderControlClass* As_ScriptedSliderControlClass() { return NULL; }
	virtual ScriptedProgressBarControlClass* As_ScriptedProgressBarControlClass() { return NULL; }
	virtual ScriptedHealthBarControlClass* As_ScriptedHealthBarControlClass() { return NULL; }

	int Get_Control_ID() { return controlId; }
	const Vector2& Get_Location() { return location; }
	void Set_Location(Vector2& newLocation) { location = newLocation; Set_Dirty(true); }
	void Set_Location(int x, int y) { location.X = (float)x; location.Y = (float)y; Set_Dirty(true); }
	const Vector2& Get_Size() { return size; }
	void Set_Size(Vector2& newSize) { size = newSize; Set_Dirty(true); }
	void Set_Size(int width, int height) { size.X = (float)width; size.Y = (float)height; Set_Dirty(true); }
	bool Is_Enabled() { return isEnabled; }
	void Set_Enabled(bool enabled) { isEnabled = enabled; Set_Dirty(true); }
	bool Is_Dirty() { return isDirty; }
	void Set_Dirty(bool dirty) { isDirty = dirty; }

protected:
	int controlId;
	Vector2 location;
	Vector2 size;
	bool isEnabled;
	bool isDirty;
};