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
#ifndef TT_INCLUDE__MIXFILEFACTORYCLASS_H
#define TT_INCLUDE__MIXFILEFACTORYCLASS_H
#include "engine_common.h"
#include "engine_vector.h"
#include "engine_io.h"
#include "engine_string.h"
struct mix_header
{
	int id;
	int index_offset;
	int tailer_offset;
	int zero;
};
class SimpleFileFactoryClass;
class MixFileFactoryClass : public FileFactoryClass
{
public:
	struct FileInfoStruct
	{
		unsigned int id;
		unsigned int offset;
		unsigned int size;
		bool operator== (const FileInfoStruct &src)
		{
			return false;
		}
		bool operator!= (const FileInfoStruct &src)
		{
			return true;
		}
		bool operator < (const FileInfoStruct& b) const
		{
			return id < b.id;
		}
	};
	struct AddInfoStruct
	{
		StringClass Filename1;
		StringClass Filename2;
		bool operator== (const AddInfoStruct &src)
		{
			return false;
		}
		bool operator!= (const AddInfoStruct &src)
		{
			return true;
		}
	};
protected:
	FileFactoryClass *Factory; //4
	DynamicVectorClass<FileInfoStruct> FileInformation; //8
	StringClass MixFilename; //32
	unsigned int UnkOffset; //36
	unsigned int FileCount; //40
	unsigned int MixFilenameOffset; //44
	bool IsValid; //48
	DynamicVectorClass<StringClass> Filenames; //52
	DynamicVectorClass<AddInfoStruct> AddedFiles; //76
	bool FileAdded; //100
public:
	MixFileFactoryClass(const char*, FileFactoryClass *);
	virtual ~MixFileFactoryClass ();
	virtual FileClass* Get_File    (const char* fileName);
	virtual void       Return_File (FileClass* file);
	void Build_Filename_List(DynamicVectorClass<StringClass>&list);
	bool Is_Valid() {return IsValid;}
};

class MixFileCreator {
public:
	struct FileInfoStruct {
		unsigned int CRC;
		unsigned int pos;
		unsigned int size;
		StringClass name;
		bool operator== (const FileInfoStruct &src)
		{
			return false;
		}
		bool operator!= (const FileInfoStruct &src)
		{
			return true;
		}
	};
	DynamicVectorClass<FileInfoStruct> Files;
	FileClass *File;
	MixFileCreator(const char *name);
	static int File_Info_Compare(const void *v1, const void *v2);
	~MixFileCreator();
	void Add_File(char  const *name, char  const *name2);
	void Add_File(char  const *name, FileClass *file);
};

class MixClass {
	struct PathStr {
		StringClass path;
		StringClass name;
	};
	struct FileInfo {
		int next;
		StringClass name;
		PathStr *path;
	};
	int *order;
	FileInfo *files;
	int count;
	int count2;
	static int File_Info_Compare(const void *v1, const void *v2);
	void Create_Mix(const char *name);
};
void BuildMixFile(const char *path,DynamicVectorClass<StringClass> filenames,DynamicVectorClass<FileFactoryClass *> filefactories);
#endif
