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
#ifndef TT_INCLUDE__FILEFACTORYLISTCLASS_H
#define TT_INCLUDE__FILEFACTORYLISTCLASS_H
#include "engine_io.h"
#include "engine_string.h"
extern char* lastFileName;

class FileFactoryListClass : public FileFactoryClass
{
#if !defined(WWCONFIG) && !defined(PACKAGEEDITOR) && !defined(W3DVIEWER)
	static REF_DECL(FileFactoryListClass *, Instance);
#else
	static FileFactoryListClass *Instance;
#endif
	FileFactoryClass* tempFactory; // 0004
	SimpleDynVecClass<FileFactoryClass*> factories; // 0008
	DynamicVectorClass<StringClass> fileNames; // 0018
	int searchStart; // 0030

public:

	static FileFactoryListClass* Get_Instance() { return Instance; }

	virtual ~FileFactoryListClass();
	virtual FileClass* Get_File(const char* fileName);
	virtual void Return_File(FileClass* file);
	
	FileFactoryListClass();
	void Add_FileFactory(FileFactoryClass* factory, const char* factoryName, bool addToBegin = false);
	void Remove_FileFactory(FileFactoryClass* factory);
	FileFactoryClass *Remove_FileFactory();
	void Add_Temp_FileFactory(FileFactoryClass* factory);
	FileFactoryClass *Remove_Temp_FileFactory();
	void Set_Search_Start(const char* factoryName);
	void Reset_Search_Start() { searchStart = 0; }
	FileFactoryClass *Peek_Temp_FileFactory();
	int Get_Factory_Count();
	FileFactoryClass *Get_Factory(int);

}; // 0034


#endif
