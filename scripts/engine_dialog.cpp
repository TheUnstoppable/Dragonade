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
#include "General.h"
#include "engine.h"
#include "engine_dialog.h"

SCRIPTS_API ScriptedLabelControlClass* Create_Label_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, const wchar_t* text, TextStyle style, Vector3 label_color)
{
	ScriptedLabelControlClass* control = dialog->Create_Control(CONTROLTYPE_LABEL)->As_ScriptedLabelControlClass();
	Set_Control_Bounds(control, x, y, width, height);
	control->Set_Text_Color(label_color);
	control->Set_Label_Text(text);
	control->Set_Style(style);
	return control;
}

SCRIPTS_API ScriptedImageControlClass* Create_Image_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, const char* texture_name)
{
	ScriptedImageControlClass* control = dialog->Create_Control(CONTROLTYPE_IMAGE)->As_ScriptedImageControlClass();
	Set_Control_Bounds(control, x, y, width, height);
	control->Set_Image_Name(texture_name);
	return control;
}

SCRIPTS_API ScriptedButtonControlClass* Create_Bordered_Button_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, const wchar_t* text)
{
	ScriptedButtonControlClass* control = dialog->Create_Control(CONTROLTYPE_BUTTON)->As_ScriptedButtonControlClass();
	Set_Control_Bounds(control, x, y, width, height);
	control->Set_Button_Style(BUTTONSTYLE_BORDERED);
	control->Set_Button_Text(text);
	return control;
}

SCRIPTS_API ScriptedButtonControlClass* Create_Image_Button_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, const char* button_up_texture, const char* button_down_texture)
{
	ScriptedButtonControlClass* control = dialog->Create_Control(CONTROLTYPE_BUTTON)->As_ScriptedButtonControlClass();
	Set_Control_Bounds(control, x, y, width, height);
	control->Set_Button_Style(BUTTONSTYLE_IMAGE);
	control->Set_Button_Up_Image_Name(button_up_texture);
	control->Set_Button_Down_Image_Name(button_down_texture);
	return control;
}

SCRIPTS_API ScriptedButtonControlClass* Create_Title_Button_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, const wchar_t* text)
{
	ScriptedButtonControlClass* control = dialog->Create_Control(CONTROLTYPE_BUTTON)->As_ScriptedButtonControlClass();
	Set_Control_Bounds(control, x, y, width, height);
	control->Set_Button_Style(BUTTONSTYLE_TITLE);
	control->Set_Button_Text(text);
	return control;
}

SCRIPTS_API ScriptedTextAreaControlClass* Create_TextArea_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, const wchar_t* default_text, int limit)
{
	ScriptedTextAreaControlClass* control = dialog->Create_Control(CONTROLTYPE_TEXTAREA)->As_ScriptedTextAreaControlClass();
	Set_Control_Bounds(control, x, y, width, height);
	control->Set_Text_Limit(limit);
	control->Set_Text(default_text);
	return control;
}

SCRIPTS_API ScriptedCheckBoxControlClass* Create_CheckBox_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, const wchar_t* text, bool checked)
{
	ScriptedCheckBoxControlClass* control = dialog->Create_Control(CONTROLTYPE_CHECKBOX)->As_ScriptedCheckBoxControlClass();
	Set_Control_Bounds(control, x, y, width, height);
	control->Set_Text(text);
	control->Set_Checked(checked);
	return control;
}

SCRIPTS_API ScriptedComboBoxControlClass* Create_ComboBox_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, DynamicVectorClass<WideStringClass>* items, int selected_index)
{
	ScriptedComboBoxControlClass* control = dialog->Create_Control(CONTROLTYPE_COMBOBOX)->As_ScriptedComboBoxControlClass();
	Set_Control_Bounds(control, x, y, width, height);

	if (items)
	{
		for (int i = 0; i < items->Count(); ++i)
		{
			control->Add_Item((*items)[i].Peek_Buffer());
		}

		control->Set_Selected_Index(selected_index);
	}

	return control;
}

SCRIPTS_API ScriptedSliderControlClass* Create_Slider_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, int value, int min, int max)
{
	ScriptedSliderControlClass* control = dialog->Create_Control(CONTROLTYPE_SLIDER)->As_ScriptedSliderControlClass();
	Set_Control_Bounds(control, x, y, width, height);
	control->Set_Minimum(min);
	control->Set_Maximum(max);
	control->Set_Value(value);
	return control;
}

SCRIPTS_API ScriptedProgressBarControlClass* Create_ProgressBar_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, int value, int min, int max)
{
	ScriptedProgressBarControlClass* control = dialog->Create_Control(CONTROLTYPE_PROGRESSBAR)->As_ScriptedProgressBarControlClass();
	Set_Control_Bounds(control, x, y, width, height);
	control->Set_Minimum(min);
	control->Set_Maximum(max);
	control->Set_Progress(value);
	return control;
}

SCRIPTS_API ScriptedHealthBarControlClass* Create_HealthBar_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, float value)
{
	ScriptedHealthBarControlClass* control = dialog->Create_Control(CONTROLTYPE_HEALTHBAR)->As_ScriptedHealthBarControlClass();
	Set_Control_Bounds(control, x, y, width, height);
	control->Set_Life(value);
	return control;
}

SCRIPTS_API void Set_Control_Bounds(ScriptedControlClass* control, int x, int y, int width, int height)
{
	control->Set_Location(x > 400 ? 400 : (x < 0 ? 0 : x), y > 300 ? 300 : (y < 0 ? 0 : y));
	control->Set_Size(width + x > 400 ? 400 - x : width, height + y > 300 ? 300 - y : height);
}

ScriptedPopupDialogClass* Create_Centered_Popup(int client, int width, int height, const wchar_t* title)
{
	ScriptedPopupDialogClass* dialog = Create_Popup_Dialog(client);
	dialog->Set_Dialog_Size(width, height);
	dialog->Set_Orientation(DIALOGORIENTATION_MIDDLECENTER);
	dialog->Set_Dialog_Title(title);
	return dialog;
}

SCRIPTS_API ScriptedDialogClass* Create_Yes_No_Dialog(GameObject* obj, const wchar_t* popup_title, const wchar_t* label_text, int& yes_button_id, int& no_button_id, Vector3 label_color)
{
	if (!obj)
	{
		return NULL;
	}

	int playerId = Get_Player_ID(obj);
	if (playerId == -1)
	{
		return NULL;
	}

	ScriptedPopupDialogClass* dialog = Create_Centered_Popup(playerId, 230, 80, popup_title);

	Create_Label_Control(dialog, 10, 10, 210, 30, label_text, TEXTSTYLE_BODY, label_color);

	ScriptedButtonControlClass* yes_button = Create_Bordered_Button_Control(dialog, 30, 50, 60, 20, L"Yes");
	ScriptedButtonControlClass* no_button = Create_Bordered_Button_Control(dialog, 140, 50, 60, 20, L"No");

	yes_button_id = yes_button->Get_Control_ID();
	no_button_id = no_button->Get_Control_ID();

	return dialog;
}

SCRIPTS_API ScriptedDialogClass* Create_Yes_No_Cancel_Dialog(GameObject* obj, const wchar_t* popup_title, const wchar_t* label_text, int& yes_button_id, int& no_button_id, int& cancel_button_id, Vector3 label_color)
{
	if (!obj)
	{
		return NULL;
	}

	int playerId = Get_Player_ID(obj);
	if (playerId == -1)
	{
		return NULL;
	}

	ScriptedPopupDialogClass* dialog = Create_Centered_Popup(playerId, 230, 80, popup_title);

	Create_Label_Control(dialog, 10, 10, 210, 30, label_text, TEXTSTYLE_BODY, label_color);

	ScriptedButtonControlClass* yes_button = Create_Bordered_Button_Control(dialog, 10, 50, 60, 20, L"Yes");
	ScriptedButtonControlClass* no_button = Create_Bordered_Button_Control(dialog, 85, 50, 60, 20, L"No");
	ScriptedButtonControlClass* cancel_button = Create_Bordered_Button_Control(dialog, 160, 50, 60, 20, L"Cancel");

	yes_button_id = yes_button->Get_Control_ID();
	no_button_id = no_button->Get_Control_ID();
	cancel_button_id = cancel_button->Get_Control_ID();

	return dialog;
}

SCRIPTS_API ScriptedDialogClass* Create_User_Prompt(GameObject* obj, const wchar_t* popup_title, const wchar_t* label_text, int text_limit, int& textbox_id, int& ok_button_id, int& cancel_button_id, const wchar_t* initial_text, Vector3 label_color)
{
	if (!obj)
	{
		return NULL;
	}

	int playerId = Get_Player_ID(obj);
	if (playerId == -1)
	{
		return NULL;
	}

	ScriptedPopupDialogClass* dialog = Create_Centered_Popup(playerId, 230, 85, popup_title);

	Create_Label_Control(dialog, 10, 10, 210, 20, label_text, TEXTSTYLE_BODY, label_color);

	ScriptedTextAreaControlClass* textarea = Create_TextArea_Control(dialog, 10, 30, 210, 15, initial_text, text_limit);
	ScriptedButtonControlClass* ok_button = Create_Bordered_Button_Control(dialog, 35, 55, 60, 20, L"Okay");
	ScriptedButtonControlClass* cancel_button = Create_Bordered_Button_Control(dialog, 135, 55, 60, 20, L"Cancel");

	textbox_id = textarea->Get_Control_ID();
	ok_button_id = ok_button->Get_Control_ID();
	cancel_button_id = cancel_button->Get_Control_ID();

	return dialog;
}

