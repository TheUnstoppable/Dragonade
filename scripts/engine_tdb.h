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
#ifndef SCRIPTS_INCLUDE__ENGINE_TDB_H
#define SCRIPTS_INCLUDE__ENGINE_TDB_H
#include "scripts.h"

/*!
* \ingroup api_translations
*
* Get a string from the translation database given its ID
*
* \param[in] ID Translation database ID to get the string for
*/
SCRIPTS_API const char *Get_Translated_String(unsigned long ID);

/*!
* \ingroup api_translations
*
* Get a wide string from the translation database given its ID
*
* \param[in] ID Translation database ID to get the string for
*/
SCRIPTS_API const wchar_t *Get_Wide_Translated_String(unsigned long ID);

/*!
* \ingroup api_translations
*
* Get the translated name of the preset for the specified object, if it has one
*
* \param[in] obj GameObject to get the translated name of
*/
SCRIPTS_API const char *Get_Translated_Preset_Name(GameObject *obj);

/*!
* \ingroup api_translations
*
* Get the translated name of the preset for a specified weapon in a GameObject weapons bag
*
* \param[in] obj GameObject which has the weapon in question
* \param[in] position Position in the weapon bag of the weapon to get the translated name of
*/
SCRIPTS_API const char *Get_Translated_Weapon(GameObject *obj,int position);

/*!
* \ingroup api_translations
*
* Get the translated name of the preset for the currently equipped weapon of a GameObject
*
* \param[in] obj GameObject for which to get the translated name of their current weapon
*/
SCRIPTS_API const char *Get_Current_Translated_Weapon(GameObject *obj);

/*!
* \ingroup api_translations
*
* Get the wide translated name of the preset for the currently equipped weapon of a GameObject
*
* \param[in] obj GameObject for which to get the translated name of their current weapon
*/
SCRIPTS_API const wchar_t *Get_Current_Wide_Translated_Weapon(GameObject *obj);

/*!
* \ingroup api_translations
*
* Get the translated name of the specified team
*
* \param[in] Team ID of the team to get the translated name of
*/
SCRIPTS_API const char *Get_Team_Name(int Team);

/*!
* \ingroup api_translations
*
* Get the first 3 characters of the translated name of the specified team
*
* \param[in] Team ID of the team to get the translated name of
*/
SCRIPTS_API StringClass Get_Short_Team_Name(int Team);

/*!
* \ingroup api_translations
*
* Get the translated name of the specified team
*
* \param[in] Team ID of the team to get the translated name of
*/
SCRIPTS_API const wchar_t *Get_Wide_Team_Name(int Team);

/*!
* \ingroup api_translations
*
* Get the translated name of a specified vehicle, as shown in the HUD display
*
* \param[in] obj VehicleGameObject to get the translated name of
*/
SCRIPTS_API const char *Get_Vehicle_Name(GameObject *obj);

/*!
* \ingroup api_translations
*
* Get the wide translated name of a specified vehicle, as shown in the HUD display
*
* \param[in] obj VehicleGameObject to get the translated name of
*/
SCRIPTS_API const wchar_t *Get_Wide_Vehicle_Name(GameObject *obj); //Get the name of a vehicle (as used for the HUD display) as a wide character string

/*!
* \ingroup api_translations
*
* Get the translated name string for a specified preset
*
* \param[in] preset Name of the preset to get the translated name of
*/
SCRIPTS_API const char *Get_Translated_Definition_Name(const char *preset);

/*!
* \ingroup api_translations
*
* Check if the specified ID is a valid translation database ID
*
* \param[in] ID An ID to check for validity
* \return Returns true if the ID exists in the translations database, false otherwise
*/
SCRIPTS_API bool Is_Valid_String_ID(unsigned long ID);

/*!
* \ingroup api_translations
*
* Gets the ID of a string in the translation database given it's description
*
* \param[in] desc The description string for the translation to get the ID of
* \return
*   The translation database ID for the translation matching the specified description, or 0
*   if no such translation was found
*/
SCRIPTS_API unsigned long Get_String_ID_By_Desc(const char *desc); //Get the ID of a string given it's ID_Desc

/*!
* \ingroup api_translations
*
* Gets the ID of the sound preset associated with the specified translation
*
* \param[in] ID The ID of the translation to get the sound preset of
* \return
*   The preset ID of the sound preset associated with the translation, or 0 if no translation was
*   found with the specified translation ID or the translation has no associated sound
*/
SCRIPTS_API unsigned long Get_String_Sound_ID(unsigned long ID);

#endif
