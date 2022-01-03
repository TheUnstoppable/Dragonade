/*	Renegade Scripts.dll
    Dragonade Example Plugin
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAEXAMPLE
#define INCLUDE_DAEXAMPLE

#include "da_event.h" //For DAEventClass definition.

//Inherit from DAEventClass.
class DAExampleEventClass : public DAEventClass {
public:
	DAExampleEventClass();
	virtual void DA_Log_Event(const char *Header,const char *Output);
	virtual bool Refill_Event(cPlayer *Player);
	virtual bool Suicide_Event(cPlayer *Player);
	virtual bool Team_Change_Request_Event(cPlayer *Player);
};

#endif