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
#ifndef SCRIPTS_INCLUDE__ENGINE_DIALOG_H
#define SCRIPTS_INCLUDE__ENGINE_DIALOG_H
#include "ScriptedDialogClass.h"
#include "ScriptedControlClass.h"
#include "ScriptedControls.h"

SCRIPTS_API ScriptedLabelControlClass* Create_Label_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, const wchar_t* text, TextStyle style = TEXTSTYLE_BODY, Vector3 label_color = Vector3(1.0f, 0.9f, 0.0f));
SCRIPTS_API ScriptedImageControlClass* Create_Image_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, const char* texture_name);
SCRIPTS_API ScriptedButtonControlClass* Create_Bordered_Button_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, const wchar_t* text);
SCRIPTS_API ScriptedButtonControlClass* Create_Image_Button_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, const char* button_up_texture, const char* button_down_texture);
SCRIPTS_API ScriptedButtonControlClass* Create_Title_Button_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, const wchar_t* text);
SCRIPTS_API ScriptedTextAreaControlClass* Create_TextArea_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, const wchar_t* default_text = NULL, int limit = 128 /* 128 is maximum */);
SCRIPTS_API ScriptedCheckBoxControlClass* Create_CheckBox_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, const wchar_t* text, bool checked = false);
SCRIPTS_API ScriptedComboBoxControlClass* Create_ComboBox_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, DynamicVectorClass<WideStringClass>* items = NULL, int selected_index = -1); // "height" is the height of dropdown, combobox height is calculated separately.
SCRIPTS_API ScriptedSliderControlClass* Create_Slider_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, int value = 50, int min = 0, int max = 100);
SCRIPTS_API ScriptedProgressBarControlClass* Create_ProgressBar_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, int value = 50, int min = 0, int max = 100);
SCRIPTS_API ScriptedHealthBarControlClass* Create_HealthBar_Control(ScriptedDialogClass* dialog, int x, int y, int width, int height, float value = 1);
void SCRIPTS_API Set_Control_Bounds(ScriptedControlClass* control, int x, int y, int width, int height);
SCRIPTS_API ScriptedPopupDialogClass* Create_Centered_Popup(int client, int width, int height, const wchar_t* title = NULL);
SCRIPTS_API ScriptedDialogClass* Create_Yes_No_Dialog(GameObject* obj, const wchar_t* popup_title, const wchar_t* label_text, int& yes_button_id, int& no_button_id, Vector3 label_color = Vector3(1.0f, 0.9f, 0.0f));
SCRIPTS_API ScriptedDialogClass* Create_Yes_No_Cancel_Dialog(GameObject* obj, const wchar_t* popup_title, const wchar_t* label_text, int& yes_button_id, int& no_button_id, int& cancel_button_id, Vector3 label_color = Vector3(1.0f, 0.9f, 0.0f));
SCRIPTS_API ScriptedDialogClass* Create_User_Prompt(GameObject* obj, const wchar_t* popup_title, const wchar_t* label_text, int text_limit, int& textbox_id, int& ok_button_id, int& cancel_button_id, const wchar_t* initial_text = NULL, Vector3 label_color = Vector3(1.0f, 0.9f, 0.0f));
#endif