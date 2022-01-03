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
#include "General.h"
#include "DefaultConnectionAcceptanceFilter.h"
#include "GameData.h"
#include "engine_game.h"
#include "ConnectionRequest.h"
#include "engine_string.h"
#include "engine_player.h"
#include "CombatManager.h"
#include "cGameType.h"
REF_DEF2(bool, CombatManager::IAmClient, 0x00855EC9, 0x008550B1);

bool DefaultConnectionAcceptanceFilter::isPlayerNameAllowed(cGameData& gameData, const WideStringClass& playerName, int id)
{
	if (IS_SOLOPLAY)
	{
		return true;
	}
	// Disallow empty names and names longer than 34 characters.
	const int playerNameLength = playerName.Get_Length();
	if (playerNameLength <= 0 || playerNameLength > 35)
		return false;

	// Disallow names that are already in use.
	if (Find_Player(playerName))
		return false;

	// Disallow names equal to the host name
	if (playerName.Compare(gameData.Get_Owner()) == 0)
	{
		if (!CombatManager::I_Am_Client())
		{
			return false;
		}
		if (id != 1)
		{
			return false;
		}
	}

	// Consider the character before the first character to be whitespace, so that names starting with a whitespace character are disallowed.
	bool isPreviousCharacterWhitespace = true;

	for (int i = 0; i < playerNameLength; i++)
	{
		wchar_t character = playerName[i];

		// Disallow names containing invalid characters
		if (character < ' ' || character > '~')
			return false;
		else if (character == ' ')
		{
			// Disallow names containing two subsequent whitespace characters.
			if (isPreviousCharacterWhitespace)
				return false;

			isPreviousCharacterWhitespace = true;
		}
		else
			isPreviousCharacterWhitespace = false;
	}

	// Disallow names ending with whitespace.
	if (isPreviousCharacterWhitespace)
		return false;

	// Allow all other names.
	return true;
}

ConnectionAcceptanceFilter::STATUS DefaultConnectionAcceptanceFilter::getStatus(const ConnectionRequest& connectionRequest, WideStringClass& refusalMessage)
{
	cGameData* gameData = The_Game();
	TT_ASSERT(gameData);

	STATUS status;
	
	if (!isPlayerNameAllowed(*gameData, connectionRequest.clientName, connectionRequest.clientId))
	{
		status = ConnectionAcceptanceFilter::STATUS_REFUSING;
		refusalMessage = L"Invalid name.";
	}
	else if (gameData->Is_Passworded() && connectionRequest.password.Compare(gameData->Get_Password()) != 0)
	{
		status = STATUS_REFUSING;
		refusalMessage = L"Invalid password.";
	}
	else if (connectionRequest.clientExeKey != gameData->Get_Version_Number())
	{
		status = STATUS_REFUSING;
		refusalMessage = L"Version mismatch.";
	}	
	else if (gameData->Get_Current_Players() > gameData->Get_Max_Players()) // This will only trigger if the player limit has been decreased, and when players with a high id (higher than the current player limit) are already ingame. Otherwise, for technical reasons, the player limit will be enforced by the netcode before consulting the acceptance filters. Also note that it is possible, although unlikely, that multiple players join at the same time before CurrentPlayers is updated, so that they go over the limit.
	{
		status = STATUS_REFUSING;
		refusalMessage = L"Game is full.";
	}	
	else if (connectionRequest.clientSerialHash.Is_Empty())
	{
		status = STATUS_INDETERMINATE;
	}
	else if (connectionRequest.clientSerialHash.Get_Length() != 32)
	{
		status = STATUS_REFUSING;
		refusalMessage = L"Invalid serial.";
	}
	else if (connectionRequest.clientVersion == 4.0f && connectionRequest.clientRevisionNumber < 5219)
	{
		refusalMessage.Format(L"You need a newer version of scripts.dll to join this server. Please get the newest version from http://www.tiberiantechnologies.org/");
		status = STATUS_REFUSING;
	}
	else
	{
		status = STATUS_ACCEPTING;
	}

	return status;
}



DefaultConnectionAcceptanceFilter defaultConnectionAcceptanceFilter;
