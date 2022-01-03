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

#include "engine_io.h"
#include <string>

class SimpleFileFactoryClass : public FileFactoryClass {
public:
#ifndef TTLE_EXPORTS
    std::string subdirectories;
#else
	StringClass SubDirectory;
#endif
	bool IsStripPath;
	mutable CriticalSectionClass Mutex;
	SimpleFileFactoryClass();
	virtual ~SimpleFileFactoryClass() {};
	virtual FileClass* Get_File    (const char* filename);
	virtual void       Return_File (FileClass* file);
	void Get_Sub_Directory(StringClass& new_dir) const;
	void Set_Sub_Directory(const char* sub_directory);
	void Append_Sub_Directory(const char* sub_directory);
	void Prepend_Sub_Directory(const char* sub_directory);
	void Set_Strip_Path(bool strip)
	{
		IsStripPath = strip;
	}
	bool Get_Strip_Path() const
	{
		return IsStripPath;
	}
};

