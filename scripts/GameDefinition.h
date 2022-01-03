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
#pragma once

#include "engine_string.h"
#include "engine_vector.h"



class GameDefinition :
	public NoEqualsClass<GameDefinition>
{

public:

	GameDefinition()
	{
	}

	GameDefinition(const GameDefinition& that)
	{
		name = that.name;
		mapName = that.mapName;
		serverPresetsFile = that.serverPresetsFile;

		int packageCount = that.packages.Length();
		packages.Resize(packageCount);
		for (int i = 0; i < packageCount; ++i)
			packages[i] = that.packages[i];
	}

	GameDefinition& operator =(const GameDefinition& that)
	{
		name = that.name;
		mapName = that.mapName;
		serverPresetsFile = that.serverPresetsFile;

		int packageCount = that.packages.Length();
		packages.Resize(packageCount);
		for (int i = 0; i < packageCount; ++i)
			packages[i] = that.packages[i];

		return *this;
	}

	StringClass name;
	StringClass mapName;
	StringClass serverPresetsFile;
	SimpleVecClass<uint32> packages;

	const StringClass& GetDisplayName() const
	{
		return name.Compare_No_Case(mapName) == 0 ? mapName : name;
	}
};
