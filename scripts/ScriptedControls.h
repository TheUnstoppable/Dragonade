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
#include "Vector3.h"
#include "ScriptedControlClass.h"

typedef enum
{
	TEXTSTYLE_BODY,
	TEXTSTYLE_TITLE,
	TEXTSTYLE_LARGE_CONTROL,
	TEXTSTYLE_TOOLTIP
} TextStyle;

typedef enum
{
	TEXTORIENTATION_LEFT,
	TEXTORIENTATION_CENTER,
	TEXTORIENTATION_RIGHT
} TextOrientation;

typedef enum
{
	BUTTONSTYLE_BORDERED,
	BUTTONSTYLE_TITLE,
	BUTTONSTYLE_IMAGE
} ButtonStyle;

/********************************************************************************/

class ScriptedLabelControlClass : public ScriptedControlClass
{
public:
	ScriptedLabelControlClass(int ctl_id);

	virtual ControlType Get_Control_Type() { return CONTROLTYPE_LABEL; }
	virtual ScriptedLabelControlClass* As_ScriptedLabelControlClass() { return this; }

	const WideStringClass& Get_Label_Text() { return text; }
	void Set_Label_Text(const wchar_t* newText) { text = newText; Set_Dirty(true); }
	const Vector3& Get_Text_Color() { return color; }
	void Set_Text_Color(Vector3& newColor) { color = newColor; Set_Dirty(true); }
	void Set_Text_Color(int r, int g, int b) { color.X = r / 255.f; color.Y = g / 255.f; color.Z = b / 255.f; Set_Dirty(true); }
	TextStyle Get_Style() { return style; }
	void Set_Style(TextStyle newStyle) { style = newStyle; Set_Dirty(true); }
	TextOrientation Get_Orientation() { return orientation; }
	void Set_Orientation(TextOrientation newOrientation) { orientation = newOrientation; Set_Dirty(true); }
private:
	WideStringClass text;
	Vector3 color;
	TextStyle style;
	TextOrientation orientation;
};

/********************************************************************************/

class ScriptedImageControlClass : public ScriptedControlClass
{
public:
	ScriptedImageControlClass(int ctl_id);

	virtual ControlType Get_Control_Type() { return CONTROLTYPE_IMAGE; }
	virtual ScriptedImageControlClass* As_ScriptedImageControlClass() { return this; }

	const StringClass& Get_Image_Name() { return imageName; }
	void Set_Image_Name(const char* newName) { imageName = newName; Set_Dirty(true); }
private:
	StringClass imageName;
};

/********************************************************************************/

class ScriptedButtonControlClass : public ScriptedControlClass
{
public:
	ScriptedButtonControlClass(int ctl_id);

	virtual ControlType Get_Control_Type() { return CONTROLTYPE_BUTTON; }
	virtual ScriptedButtonControlClass* As_ScriptedButtonControlClass() { return this; }

	const WideStringClass& Get_Button_Text() { return buttonText; }
	void Set_Button_Text(const wchar_t* newText) { buttonText = newText; Set_Dirty(true); }
	const StringClass& Get_Button_Up_Image_Name() { return imageNameUp; }
	void Set_Button_Up_Image_Name(const char* newName) { imageNameUp = newName; Set_Dirty(true); }
	const StringClass& Get_Button_Down_Image_Name() { return imageNameDn; }
	void Set_Button_Down_Image_Name(const char* newName) { imageNameDn = newName; Set_Dirty(true); }
	ButtonStyle Get_Button_Style() { return style; }
	void Set_Button_Style(ButtonStyle newStyle) { style = newStyle; Set_Dirty(true); }
private:
	WideStringClass buttonText;
	StringClass imageNameUp;
	StringClass imageNameDn;
	ButtonStyle style;
};

/********************************************************************************/

class ScriptedTextAreaControlClass : public ScriptedControlClass
{
public:
	ScriptedTextAreaControlClass(int ctl_id);

	virtual ControlType Get_Control_Type() { return CONTROLTYPE_TEXTAREA; }
	virtual ScriptedTextAreaControlClass* As_ScriptedTextAreaControlClass() { return this; }

	int Get_Text_Limit() { return textLimit; }
	void Set_Text_Limit(int newLimit) { textLimit = newLimit; Set_Dirty(true); }
	bool Is_Password_Field() { return isPassword; }
	void Set_Password_Field(bool password) { isPassword = password; Set_Dirty(true); }
	bool Is_Numeric_Field() { return isNumeric; }
	void Set_Numeric_Field(bool numeric) { isNumeric = numeric; Set_Dirty(true); }
	bool Is_Automatic_Horizontal_Scroll() { return autoHorizontalScroll; }
	void Set_Automatic_Horizontal_Scroll(bool scroll) { autoHorizontalScroll = scroll; Set_Dirty(true); }
	const WideStringClass& Get_Text() { return textareaText; }
	void Set_Text(const wchar_t* newText) { textareaText = newText; Set_Dirty(true); }
	int Get_Text_Length() { return textareaText.Get_Length(); }
private:
	int textLimit;
	bool isNumeric;
	bool isPassword;
	bool autoHorizontalScroll;
	WideStringClass textareaText;
};

/********************************************************************************/

class ScriptedCheckBoxControlClass : public ScriptedControlClass
{
public:
	ScriptedCheckBoxControlClass(int ctl_id);

	virtual ControlType Get_Control_Type() { return CONTROLTYPE_CHECKBOX; }
	virtual ScriptedCheckBoxControlClass* As_ScriptedCheckBoxControlClass() { return this; }

	const WideStringClass& Get_Text() { return checkboxText; }
	void Set_Text(const wchar_t* newText) { checkboxText = newText; Set_Dirty(true); }
	bool Is_Checked() { return isChecked; }
	void Set_Checked(bool checked) { isChecked = checked; Set_Dirty(true); }
private:
	WideStringClass checkboxText;
	bool isChecked;
};

/********************************************************************************/

class ScriptedComboBoxControlClass : public ScriptedControlClass
{
public:
	ScriptedComboBoxControlClass(int ctl_id);

	virtual ControlType Get_Control_Type() { return CONTROLTYPE_COMBOBOX; }
	virtual ScriptedComboBoxControlClass* As_ScriptedComboBoxControlClass() { return this; }

	int Get_Selected_Index() { return selectedIndex; }
	void Set_Selected_Index(int newIndex) { selectedIndex = newIndex; Set_Dirty(true); }
	void Add_Item(const wchar_t* item) { options.Add(item); Set_Dirty(true); }
	void Insert_Item(int index, const wchar_t* item) { options.Insert(index, item); Set_Dirty(true); }
	int Get_Item_Count() { return options.Count(); }
	const WideStringClass& Get_Item(int index) { return options[index]; }
	const WideStringClass& Get_Selected_Item_Text() { return options[selectedIndex]; }
	void Remove_Item(int index) { options.Delete(index); Set_Dirty(true); }
	void Remove_Item(const WideStringClass& item) { options.DeleteObj(item); Set_Dirty(true); }
	void Clear_Items() { options.Delete_All(); Set_Dirty(true); }
private:
	DynamicVectorClass<WideStringClass> options;
	int selectedIndex;
};

/********************************************************************************/

class ScriptedSliderControlClass : public ScriptedControlClass
{
public:
	ScriptedSliderControlClass(int ctl_id);

	virtual ControlType Get_Control_Type() { return CONTROLTYPE_SLIDER; }
	virtual ScriptedSliderControlClass* As_ScriptedSliderControlClass() { return this; }

	int Get_Minimum() { return minVal; }
	void Set_Minimum(int newMin) { minVal = newMin; Set_Dirty(true); }
	int Get_Maximum() { return maxVal; }
	void Set_Maximum(int newMax) { maxVal = newMax; Set_Dirty(true); }
	int Get_Value() { return value; }
	void Set_Value(int newValue) { value = newValue; Set_Dirty(true); }
private:
	int minVal;
	int maxVal;
	int value;
};

/********************************************************************************/

class ScriptedProgressBarControlClass : public ScriptedControlClass
{
public:
	ScriptedProgressBarControlClass(int ctl_id);

	virtual ControlType Get_Control_Type() { return CONTROLTYPE_PROGRESSBAR; }
	virtual ScriptedProgressBarControlClass* As_ScriptedProgressBarControlClass() { return this; }

	int Get_Minimum() { return minVal; }
	void Set_Minimum(int newMin) { minVal = newMin; Set_Dirty(true); }
	int Get_Maximum() { return maxVal; }
	void Set_Maximum(int newMax) { maxVal = newMax; Set_Dirty(true); }
	int Get_Progress() { return progress; }
	void Set_Progress(int newProgress) { progress = newProgress > maxVal ? maxVal : newProgress < minVal ? minVal : newProgress; Set_Dirty(true); }
	int Get_Step_Count() { return stepCount; }
	void Set_Step_Count(int newStep) { stepCount = newStep; /* don't need to set dirty here */ }
	void Increment_Value() { progress += progress + stepCount > maxVal ? maxVal - progress : stepCount; Set_Dirty(true); }
	void Decrement_Value() { progress -= progress - stepCount < minVal ? progress - minVal : stepCount; Set_Dirty(true); }

private:
	int minVal;
	int maxVal;
	int progress;
	int stepCount;
};

/********************************************************************************/

class ScriptedHealthBarControlClass : public ScriptedControlClass
{
public:
	ScriptedHealthBarControlClass(int ctl_id);

	virtual ControlType Get_Control_Type() { return CONTROLTYPE_HEALTHBAR; }
	virtual ScriptedHealthBarControlClass* As_ScriptedHealthBarControlClass() { return this; }

	float Get_Life() { return life; }
	void Set_Life(float newLife) { life = WWMath::Clamp(newLife); Set_Dirty(true); }

private:
	float life;
};

/********************************************************************************/