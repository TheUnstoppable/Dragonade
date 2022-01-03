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
#include "SimpleFileFactoryClass.h"
#include "BufferedFileClass.h"
void SimpleFileFactoryClass::Get_Sub_Directory(StringClass& new_dir) const
{
	CriticalSectionClass::LockClass lock(Mutex);
#ifndef TTLE_EXPORTS
    new_dir = subdirectories.c_str();
#else
	new_dir = SubDirectory;
#endif
}

void SimpleFileFactoryClass::Set_Sub_Directory(const char *sub_directory)
{
	CriticalSectionClass::LockClass lock(Mutex);
	if (sub_directory)
	{
#ifndef TTLE_EXPORTS
        subdirectories = sub_directory;
#else
		SubDirectory = sub_directory;
#endif
	}
}

void SimpleFileFactoryClass::Append_Sub_Directory(const char *sub_directory)
{
	char temp_sub_dir[1024];
	if (strlen(sub_directory) <= 1022 && strlen(sub_directory) >= 1)
	{
		strcpy(temp_sub_dir,sub_directory);
		if (temp_sub_dir[strlen(sub_directory)] != '\\')
		{
			temp_sub_dir[strlen(sub_directory)] = '\\';
			temp_sub_dir[strlen(sub_directory) + 1] = 0;
		}

		CriticalSectionClass::LockClass lock(Mutex);
#ifndef TTLE_EXPORTS
        if (subdirectories.size() && subdirectories[subdirectories.size() - 1] != ';')
		{
            subdirectories += ';';
		}
        subdirectories += temp_sub_dir;
#else
		if (SubDirectory.Get_Length() && SubDirectory[SubDirectory.Get_Length() - 1] != ';')
		{
			SubDirectory += ';';
		}
		SubDirectory += temp_sub_dir;
#endif
	}
}

void SimpleFileFactoryClass::Prepend_Sub_Directory(const char *sub_directory)
{
	char temp_sub_dir[1024];
	if (strlen(sub_directory) <= 1021 && strlen(sub_directory) >= 1)
	{
		strcpy(temp_sub_dir,sub_directory);
		if (temp_sub_dir[strlen(sub_directory)] != '\\')
		{
			temp_sub_dir[strlen(sub_directory)] = '\\';
			temp_sub_dir[strlen(sub_directory) + 1] = 0;
		}
		temp_sub_dir[strlen(sub_directory)] = ';';
		temp_sub_dir[strlen(sub_directory) + 1] = 0;

		CriticalSectionClass::LockClass lock(Mutex);
#ifndef TTLE_EXPORTS
        subdirectories = temp_sub_dir + subdirectories;
#else
		SubDirectory = temp_sub_dir + SubDirectory;
#endif
	}
}

SimpleFileFactoryClass::SimpleFileFactoryClass() : IsStripPath(false)
{
}

bool Is_Full_Path(const char *path)
{
	if (!path)
		return false;
	return path[1] == ':' || (path[0] == '\\' && path[1] == '\\');
}

FileClass* SimpleFileFactoryClass::Get_File(const char* filename)
{
#ifndef TTLE_EXPORTS
    const char* stripped_name = filename;

    if (IsStripPath)
    {
        const char* separator = strrchr(filename, '/');
        if (separator)
            stripped_name = separator + 1;
        else
            stripped_name = filename;
    }
    else
        stripped_name = filename;

    std::string new_name;

    BufferedFileClass *file = new BufferedFileClass;
    if (!Is_Full_Path(stripped_name))
    {
        CriticalSectionClass::LockClass lock(Mutex);
        if (subdirectories.size() != 0)
        {
            if (strchr(subdirectories.c_str(), ';'))
            {
                char* subdir = _strdup(subdirectories.c_str());

                for (const char* token = strtok(subdir, ";"); token; token = strtok(NULL, ";"))
                {
                    new_name = token;
                    new_name.append(stripped_name);
                    file->Set_Name(new_name.c_str());
                    if (file->Is_Available(0)) break;
                }

                free(subdir);
            }
            else
            {
                new_name = subdirectories + stripped_name;
            }
        }
	}
    else
    {
        new_name = stripped_name;
    }

    file->Set_Name(new_name.c_str());

    return file;
#else
	StringClass stripped_name(248, true);
	StringClass new_name(248, true);
	if (IsStripPath)
	{
		const char* separator = strrchr(filename, '/');
		if (separator)
			stripped_name = separator + 1;
		else
			stripped_name = filename;
	}
	else
		stripped_name = filename;


	BufferedFileClass *file = new BufferedFileClass;
	if (!Is_Full_Path(stripped_name))
	{
		CriticalSectionClass::LockClass lock(Mutex);
		if (!SubDirectory.Is_Empty())
		{
			if (strchr(SubDirectory, ';'))
			{
				StringClass subdir = SubDirectory;
				for (const char* token = strtok(subdir.Peek_Buffer(), ";"); token; token = strtok(NULL, ";"))
				{
					new_name.Format("%s%s", token, stripped_name);
					file->Set_Name(new_name);
					if (file->Is_Available(0)) break;
				}
			}
			else
				new_name.Format("%s%s", SubDirectory, stripped_name);
		}
	}
	else
	{
		new_name = stripped_name;
	}
	file->Set_Name(new_name);
	return file;
#endif
}

void SimpleFileFactoryClass::Return_File(FileClass* file)
{
	delete file;
}
