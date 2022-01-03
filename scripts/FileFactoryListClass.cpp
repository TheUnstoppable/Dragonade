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
#include "general.h"

#include "FileFactoryListClass.h"

FileClass* FileFactoryListClass::Get_File(const char* fileName)
{
	FileClass* result = NULL; // The first file for which Is_Available returns true, or else the first file that is non-null, or otherwise null.
	FileFactoryClass* resultFactory = NULL;
	if (tempFactory)
	{
		result = tempFactory->Get_File(fileName);
		resultFactory = tempFactory;
	}
	if (!result || !result->Is_Available())
	{
		for (int i = 0; i < factories.Count(); ++i)
		{
			TT_ASSERT(!result || !result->Is_Available());
			FileFactoryClass* factory = factories[i];
			FileClass* file = factory->Get_File(fileName);
			if (file)
			{
				if (!result)
				{
					result = file;
					resultFactory = factory;
					if (file->Is_Available())
						break;
				}
				else if (file->Is_Available())
				{
					TT_ASSERT(result && !result->Is_Available());
					resultFactory->Return_File(result);
					result = file;
					resultFactory = factory;
					break;
				}
				else
				{
					factory->Return_File(file);
				}
			}
		}
	}

	return result;
}

void FileFactoryListClass::Return_File
   (FileClass* file)
{
   this->factories[0]->Return_File (file);
}

void FileFactoryListClass::Add_FileFactory
   (FileFactoryClass* factory, const char* name, bool addToBegin)
{
	if (addToBegin)
	{
		factories.Add_Head(factory);
		fileNames.Add_Head(name);
	}
	else
	{
		factories.Add(factory);
		fileNames.Add(name);
	}
	searchStart = 0;
}

void FileFactoryListClass::Remove_FileFactory(FileFactoryClass *factory)
{
	for (int i = 0;i < factories.Count();i++)
	{
		if (factories[i] == factory)
		{
			factories.Delete(i);
			fileNames.Delete(i);
			searchStart = 0;
		}
	}
}

#if !defined(WWCONFIG) && !defined(PACKAGEEDITOR) && !defined(W3DVIEWER)
#pragma warning(disable: 4073) //warning C4073: initializers put in library initialization area - That's EXACTLY why I put that pragma in...
#pragma init_seg(lib) // Move this files static initializers up a level
#pragma warning(default: 4073)
REF_DEF3(FileFactoryListClass *, FileFactoryListClass::Instance, 0x0085DB70, 0x0085CD58, 0x0085CE48);
#else
FileFactoryListClass *FileFactoryListClass::Instance = 0;
#endif

FileFactoryClass *FileFactoryListClass::Remove_Temp_FileFactory()
{
	FileFactoryClass *f = tempFactory;
	tempFactory = 0;
	return f;
}
void FileFactoryListClass::Add_Temp_FileFactory(FileFactoryClass *factory)
{
	tempFactory = factory;
}



void FileFactoryListClass::Set_Search_Start(const char* factoryName)
{
	for (int factoryIndex = 0; factoryIndex < fileNames.Length(); ++factoryIndex)
	{
		if (_stricmp(fileNames[factoryIndex], factoryName) == 0)
		{
			searchStart = factoryIndex;
			return;
		}
	}
	
	searchStart = 0;
}

FileFactoryClass *FileFactoryListClass::Remove_FileFactory()
{
	FileFactoryClass *factory = 0;
	if (factories.Count())
	{
		factory = factories[0];
		factories.Delete(0);
		fileNames.Delete(0);
	}
	return factory;
}

FileFactoryListClass::FileFactoryListClass() : tempFactory(0), searchStart(0)
{
	Instance = this;
}

FileFactoryListClass::~FileFactoryListClass()
{
	Instance = 0;
}
