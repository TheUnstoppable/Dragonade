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
#ifndef SCRIPTS_INCLUDE__ENGINE_PT_H
#define SCRIPTS_INCLUDE__ENGINE_PT_H
#include "BuildingGameObjDef.h"
using namespace BuildingConstants;
unsigned int SCRIPTS_API Get_Team_Cost(const char *preset,unsigned int team); //Get the cost of a preset for a given team. Returns zero if the preset is not found in any of the purchase terminal data or if it is one of the free units.
unsigned int SCRIPTS_API Get_Team_Cost(int def_id,unsigned int team); //Get the cost of a preset for a given team. Returns zero if the preset is not found in any of the purchase terminal data or if it is one of the free units.
unsigned int SCRIPTS_API Get_Cost(const char *preset); //Get the cost of a preset. Returns zero if the preset is not found in any of the purchase terminal data or if it is one of the free units.
unsigned int SCRIPTS_API Get_Cost(int def_id); //Get the cost of a preset. Returns zero if the preset is not found in any of the purchase terminal data or if it is one of the free units.
void SCRIPTS_API Disable_Preset_By_Name(unsigned int Team,const char *Name, bool enable); //Disable a preset by name
void SCRIPTS_API Hide_Preset_By_Name(unsigned int Team,const char *Name, bool enable); //Hides a preset by name
void SCRIPTS_API Busy_Preset_By_Name(unsigned int Team,const char *Name, bool enable); //Marks a preset as busy by name
const char SCRIPTS_API *Get_Team_Icon(const char *preset,unsigned int team); //Get the icon texture name for a given preset of a given team. Returns NULL if it cant find that preset in the PT data.
const char SCRIPTS_API *Get_Icon(const char *preset); //Get the icon texture name for a given preset. Returns NULL if it cant find that preset in the PT data.
void SCRIPTS_API Disable_All_Presets_By_Factory_Tech(BuildingType type,unsigned int team,bool disable); //Iterate all PT data for that team and disable/enable all presets who's factory or tech building matches the specified type.
#endif
