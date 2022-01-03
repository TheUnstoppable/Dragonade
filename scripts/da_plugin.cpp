/*	Renegade Scripts.dll
    Dragonade Plugin Manager
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#include "general.h"
#include "scripts.h"
#include "da.h"
#include "da_plugin.h"
#include "da_settings.h"
#include "da_ssgm.h"
#include "da_log.h"

DynamicVectorClass<HINSTANCE> DAPluginManager::Plugins;

typedef void (*PluginInit)();
typedef void (*PluginShutdown)();

void DAPluginManager::Init() {
	DASSGMPluginManager::Init();
	INISection *Section = DASettingsManager::Get_Section("Plugins");
	if (Section) {
		bool NewLine = false;
		for (INIEntry *i = Section->EntryList.First();i && i->Is_Valid();i = i->Next()) {
			if (!_stricmp(i->Value,"1") || !_stricmp(i->Value,"true")) {
				NewLine = true;
				HINSTANCE Handle = LoadLibrary(i->Entry);
				if (Handle) {
					PluginInit Init = (PluginInit)GetProcAddress(Handle,"Plugin_Init");
					if (Init) {
						Init();
					}
					Plugins.Add(Handle);
					Console_Output("Plugin %s loaded\n",i->Entry);
					DALogManager::Write_Log("_PLUGIN","load %s",i->Entry);
				}
				else {
					Console_Output("Could not load plugin %s\n",i->Entry);
					DALogManager::Write_Log("_PLUGIN","failure %s",i->Entry);
				}
			}
		}
		if (NewLine) {
			Console_Output("\n");
		}
	}
}

void DAPluginManager::Shutdown() {
	for (int i = 0;i < Plugins.Count();i++) {
		PluginShutdown Shutdown = (PluginShutdown)GetProcAddress(Plugins[i],"Plugin_Shutdown");
		if (Shutdown) {
			Shutdown();
		}
		FreeLibrary(Plugins[i]);
	}
}
