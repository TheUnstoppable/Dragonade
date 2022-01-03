/*	Renegade Scripts.dll
	Copyright 2013 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#pragma once

#include "ConnectionAcceptanceFilter.h"

class cGameData;
class StringClass;
struct ConnectionRequest;


class DefaultConnectionAcceptanceFilter :
	public ConnectionAcceptanceFilter
{
	bool isPlayerNameAllowed(cGameData& gameData, const WideStringClass& playerName, int id);

public:
	virtual void handleInitiation(const ConnectionRequest& connectionRequest) {}
	virtual void handleTermination(const ConnectionRequest& connectionRequest) {}
	virtual void handleCancellation(const ConnectionRequest& connectionRequest) {}
	virtual STATUS getStatus(const ConnectionRequest& connectionRequest, WideStringClass& refusalMessage);
};



extern DefaultConnectionAcceptanceFilter defaultConnectionAcceptanceFilter;
