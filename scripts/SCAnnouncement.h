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
#ifndef TT_INCLUDE__SCANNOUNCEMENT_H
#define TT_INCLUDE__SCANNOUNCEMENT_H

#include "cNetEvent.h"
#include "engine_ttdef.h"


class SCAnnouncement :
	public cNetEvent
{

	int receiptientId; // 06B4
	int senderId; // 06B8
	int translationId; // 06BC
	int emoticonId; // 06C0
	AnnouncementEnum type; // 06C4

public:

	virtual uint Get_Network_Class_ID() const;
	virtual void Import_Creation(BitStreamClass& stream);
	virtual void Export_Creation(BitStreamClass& stream);
	virtual void Act();

	SCAnnouncement();
	~SCAnnouncement();
	void Init(int _receiptientId, int _senderId, int _translationId, AnnouncementEnum _type, int _emoticonId, bool dodirtybit = true, bool doact = true);
	SCRIPTS_API void Set_Dirty_Bit_For_Team(DIRTY_BIT value, int teamId);

};



#endif
