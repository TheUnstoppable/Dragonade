/*	Renegade Scripts.dll
    Dragonade Advanced Kill Messages Game Feature
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAKILLMESSAGES
#define INCLUDE_DAKILLMESSAGES

#include "da_event.h"
#include "da_gamefeature.h"

class DAAdvancedKillMessagesGameFeatureClass : public DAEventClass, public DAGameFeatureClass {
	virtual void Init();
	virtual void Settings_Loaded_Event();
	virtual void DA_Log_Event(const char *Header,const char *Output);

	//Settings
	bool EnablePlayer;
	bool EnableVehicle;
	bool EnableBuilding;
	bool EnableBot;
};

#endif