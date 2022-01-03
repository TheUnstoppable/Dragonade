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
#include "General.h"
#include "Vector3.h"
#include "GameObjManager.h"

SList<C4GameObj> GameObjManager::C4GameObjList;
SList<BeaconGameObj> GameObjManager::BeaconGameObjList;
SList<SoldierGameObj> GameObjManager::SoldierGameObjList;
SList<VehicleGameObj> GameObjManager::VehicleGameObjList;
SList<ScriptZoneGameObj> GameObjManager::ScriptZoneGameObjList;
