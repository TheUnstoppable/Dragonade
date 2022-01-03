/*	Renegade Scripts.dll
    Dragonade Singleplayer Level Cleaner
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DALEVELCLEANER
#define INCLUDE_DALEVELCLEANER

class DA_API DALevelCleaner {
public:
	static void Clean_Level();
	static void Destroy_SP_Buildings();
	static void Destroy_SP_Objects();
	static void Destroy_SP_Spawners();
	static void Load_Blockers(const INIClass *INI);
	
private:
	static bool Check_Scripts(GameObject *obj);
	static bool Check_Model(GameObject *obj);
	static bool Check_Preset(GameObject *obj);
};

#endif

