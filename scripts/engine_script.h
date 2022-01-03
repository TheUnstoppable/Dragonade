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
#ifndef SCRIPTS_INCLUDE__ENGINE_SCRIPT_H
#define SCRIPTS_INCLUDE__ENGINE_SCRIPT_H
#include "scripts.h"
#include "engine_string.h"
#include "SList.h"

SCRIPTS_API void Remove_Script(GameObject *obj,const char *script); //removes all copies of <script> from an object
SCRIPTS_API void Remove_All_Scripts(GameObject *obj); //removes all scripts from an object
SCRIPTS_API void Attach_Script_Preset(const char *script,const char *params,const char *preset,int team); //attached <script> to all objects of <preset> in team <team>
SCRIPTS_API void Attach_Script_Type(const char *script,const char *params,unsigned long type,int team); //attaches <script> to all objects of <type> in team <team>
SCRIPTS_API void Remove_Script_Preset(const char *script,const char *preset,int team); //removes all copies of <script> from all objects of <preset> in team <team>
SCRIPTS_API void Remove_Script_Type(const char *script,unsigned long type,int team); //removes all copies of <script> from all objects of <type> in team <team>
SCRIPTS_API bool Is_Script_Attached(GameObject *obj,const char *script); //is the script attached
SCRIPTS_API void Attach_Script_Once(GameObject *obj,const char *script,const char *params); //attach a script if its not already attached
SCRIPTS_API void Attach_Script_Preset_Once(const char *script,const char *params,const char *preset,int team); //attach a script to all objects of preset if its not already attached
SCRIPTS_API void Attach_Script_Type_Once(const char *script,const char *params,unsigned long type,int team); //attach a script to all objects of type if its not already attached
SCRIPTS_API void Attach_Script_Building(const char *script,const char *params,int team); //attach a script to all buildings
SCRIPTS_API void Attach_Script_Is_Preset(GameObject *obj,const char *preset,const char *script,const char *params,int team); //attach the script if object is of preset
SCRIPTS_API void Attach_Script_Is_Type(GameObject *obj,unsigned long type,const char *script,const char *params,int team); //attach the script if object is of type
SCRIPTS_API void Attach_Script_Player_Once(const char *script,const char *params,int team); //attach a script to all players if its not already attached
SCRIPTS_API void Remove_Duplicate_Script(GameObject *obj, const char *script); //remove duplicate scripts from an object
SCRIPTS_API void Attach_Script_All_Buildings_Team(int Team,const char *Script,const char *Params,bool Once); //attach a script to all buildings by team
SCRIPTS_API void Attach_Script_All_Turrets_Team(int Team,const char *Script,const char *Params,bool Once); //attach a script to all static vehicles by team
SCRIPTS_API GameObject *Find_Building_With_Script(int Team,int Type,const char *Script,GameObject *Caller); //Find a building of this type with this script attached to it
SCRIPTS_API GameObject *Find_Object_With_Script(const char *script); //Find the first object with this script on it
SCRIPTS_API GameObject *Find_Closest_Object_With_Script(const char *script, Vector3 pos); //Find the closest object to pos with this script on it
SCRIPTS_API void Find_All_Objects_With_Script(const char *script, SList<GameObject>& objects); // Find all objects with this script on it

/*!
* \brief Find Objects With Script - Distance Sorted
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup api_objects_find
*
* Finds all GameObjects with one or more instances of the specified script attached and returns them
* in a list which is sorted by the distance from that object to a specified position
*
* \param[in] script
*   Name of the script to search for on the object
* \param[out] objects
*   Reference to an SList<GameObject> in which the results should be stored
* \param[in] position
*   The position to calculate distance from
*/
SCRIPTS_API void Find_All_Objects_With_Script_By_Distance(const char *script, SList<GameObject>& objects, Vector3 position);
SCRIPTS_API void Find_All_Vehicles_By_Distance(SList<GameObject>& objects, Vector3 position);

SCRIPTS_API void Send_Custom_Event_To_Objects_With_Script( GameObject *sender, const char *script, int message, int param, float delay ); // Script to send a custom to all objects with a specific script
SCRIPTS_API void Send_Custom_Event_To_Objects_With_Script_Ranged( GameObject *sender, const char *script, int message, int param, float delay, float range ); // Script to send a custom to all objects with a specific script in a specified range
SCRIPTS_API ScriptImpClass* Find_Script_On_Object(GameObject* obj, const char *script); // Returns a pointer to the first instance of the named script on the given object, or NULL if not found
SCRIPTS_API void Attach_Script_Occupants(GameObject *obj,const char *script,const char *params); //attaches a script to all occupants of a vehicle

/*!
* \brief Attach Script with Formatted Parameters
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup api_scripts
*
* This is a variant of ScriptCommands::Attach_Script which uses a variable length arguments list to
* format the script parameters, rather than having to do it manually inside scripts using a string
* variable of some sort.
*
* \param[in] pObj
*   The object to attach this script to
* \param[in] script
*   The name of the script to be attached to the object
* \param[in] params
*   The format string for the script parameters
*/
SCRIPTS_API void Attach_Script_V ( GameObject* pObj, const char* script, const char* params, ... );

/*!
* \brief Attach Script with Formatted Parameters (va_list)
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup api_scripts
*
* A variant of Attach_Script which takes a va_list object rather than a variable number of arguments
* so it can be called from within another function which itself takes a variable number of arguments
*
* \param[in] pObj
*   The object to attach this script to
* \param[in] script
*   The name of the script to be attached to the object
* \param[in] params
*   The format string for the script parameters
* \param[in] vargs
*   The variable arguments object
*/
SCRIPTS_API void Attach_Script_V ( GameObject* pObj, const char* script, const char* params, va_list vargs );

/*!
* \brief Attach Script Once with Formatted Parameters
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup api_scripts
*
* This is a variant of Attach_Script_Once which uses a variable length arguments list to format the
* script parameters, rather than having to do it manually inside scripts using a string variable of
* some sort.
*
* \param[in] pObj
*   The object to attach this script to
* \param[in] script
*   The name of the script to be attached to the object
* \param[in] params
*   The format string for the script parameters
*/
SCRIPTS_API void Attach_Script_Once_V ( GameObject* pObj, const char* script, const char* params, ... );

class ScriptParameter : public NoEqualsClass<ScriptParameter>
{
public:
	StringClass name;
	StringClass value;
	int type;
};

typedef enum
{
	PARAM_TYPE_INT = 0,
	PARAM_TYPE_FLOAT,
	PARAM_TYPE_STRING,
	PARAM_TYPE_BOOL,
	PARAM_TYPE_ID,
	PARAM_TYPE_VECTOR3,
	PARAM_TYPE_ENUM,
	PARAM_TYPE_EMITTER,
	PARAM_TYPE_WEAPON,
	PARAM_TYPE_AMMO,
	PARAM_TYPE_EXPLOSION,
	PARAM_TYPE_ANIMATION,
	PARAM_TYPE_GANG,
	PARAM_TYPE_FILE,
	PARAM_TYPE_SOUND,
	PARAM_TYPE_COLOR,
	PARAM_TYPE_COUNT
} PARAM_TYPES;

const char * const PARAM_TYPE_STRINGS[PARAM_TYPE_COUNT] =
{
	"int",
	"float",
	"string",
	"bool",
	"ID",
	"vector3",
	"enum",
	"emitter",
	"weapon",
	"ammo",
	"explosion",
	"animation",
	"gang",
	"file",
	"sound",
	"color",
};

SCRIPTS_API void Get_Script_Parameters(const char *script, DynamicVectorClass<ScriptParameter> &parameters);

#endif
