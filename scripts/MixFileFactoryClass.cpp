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
#include "general.h"

#include "MixFileFactoryClass.h"
#include "SimpleFileFactoryClass.h"
#include "Crc32.h"
#include <algorithm>
#include "RawFileClass.h"

MixFileFactoryClass::MixFileFactoryClass
   (const char* filePath, FileFactoryClass *parentFileFactory) : FileFactoryClass(), Factory(0), UnkOffset(0), FileCount(0), MixFilenameOffset(0), IsValid(false), FileAdded(false)
{
	MixFilename = filePath;
	Factory = parentFileFactory;
	Filenames.Set_Growth_Step(1000);
	FileClass *f = Factory->Get_File(filePath);
	if (f)
	{
		if (f->Is_Available())
		{
			f->Open(1);
			IsValid = true;
			mix_header header;
			IsValid = (f->Read(&header,sizeof(mix_header)) == sizeof(mix_header));
			if (IsValid)
			{
				if (header.id != 0x3158494D)
				{
					IsValid = false;
				}
			}
			FileCount = 0;
			if (IsValid)
			{
				f->Seek(header.index_offset,0);
				IsValid = (f->Read(&FileCount,4)) == 4;
				if (IsValid)
				{
					FileInformation.Resize(FileCount,0);
					FileInformation.Set_Active(FileCount);
					IsValid = (f->Read(&FileInformation[0],FileCount * sizeof(FileInfoStruct)) == (int)(FileCount * sizeof(FileInfoStruct)));
					if (IsValid)
					{
						// check if the index isn't sorted
						if (!std::is_sorted(&FileInformation[0], &FileInformation[0] + FileInformation.Count()))
						{
							// fix it
							std::sort(&FileInformation[0], &FileInformation[0] + FileInformation.Count());
						}
						UnkOffset = 0;
						MixFilenameOffset = header.tailer_offset;
						Factory->Return_File(f);
						return;
					}
				}
			}
			FileInformation.Resize(0,0);
		}
		Factory->Return_File(f);
	}
}

MixFileFactoryClass::~MixFileFactoryClass()
{
	FileInformation.Resize(0,0);
}

FileClass* MixFileFactoryClass::Get_File(const char* fileName)
{
	int file_count = FileInformation.Count();
	if (file_count)
	{
		unsigned long crc = CRC_Stringi(fileName,0);

		// binary search through the index to find the file
		int min = 0;
		int max = file_count;

		while (min <= max)
		{
			int i = min + (max - min) / 2;
			if (FileInformation[i].id == crc) 
			{
				FileClass *f = Factory->Get_File(MixFilename);
				if (f) f->Bias(FileInformation[i].offset + UnkOffset,FileInformation[i].size);
				return f;
			}
			else if (FileInformation[i].id < crc) min = i + 1;
			else max = i - 1;
		}
	}
	return NULL;
}

void MixFileFactoryClass::Return_File(FileClass* file)
{
	if (file)
	{
		Factory->Return_File(file);
	}
}
void MixFileFactoryClass::Build_Filename_List(DynamicVectorClass<StringClass>&list)
{
	if (IsValid)
	{
		FileClass *f = Factory->Get_File(MixFilename);
		if (f->Open())
		{
			f->Seek(MixFilenameOffset,0);
			int count;
			if (f->Read(&count,4) == 4)
			{
				for (int i = 0;i < count;i++)
				{
					int size = 0;
					if (f->Read(&size,1) == 1)
					{
						StringClass name(size,false);
						if (f->Read(name.Peek_Buffer(),size) == size)
						{
							list.Add(name);
						}
					}
				}
			}
		}
		Factory->Return_File(f);
	}
}
SimpleFileFactoryClass SimpleFileFactory;

MixFileCreator::MixFileCreator(const char *name)
{
	File = SimpleFileFactory.Get_File(name);
	if (File)
	{
		File->Open(2);
		File->Write("MIX1",4);
		int i = 0;
		File->Write(&i,4);
		File->Write(&i,4);
		File->Write(&i,4);
	}
}

int MixFileCreator::File_Info_Compare(const void *v1, const void *v2)
{
	int ret = -1;
	if (((FileInfoStruct *)v1)->CRC >= ((FileInfoStruct *)v2)->CRC)
	{
		ret = ((FileInfoStruct *)v1)->CRC > ((FileInfoStruct *)v2)->CRC;
	}
	return ret;
}

static void NormalizeFilename(char* filename)
{
    for ( ;*filename; ++filename)
    {
        if (*filename & 0x80) *filename = '-';
    }
}

int MixClass::File_Info_Compare(const void *v1, const void *v2)
{
    StringClass file0_name(((FileInfo *)v1)->path->name, true);
    StringClass file1_name(((FileInfo *)v2)->path->name, true);

    NormalizeFilename(file0_name.Peek_Buffer());
    NormalizeFilename(file1_name.Peek_Buffer());

    unsigned long crc1 = CRC_Stringi(file0_name);
    unsigned long crc2 = CRC_Stringi(file1_name);
    int ret = -1;
    if (crc1 >= crc2)
    {
        ret = crc1 > crc2;
    }
    return ret;
}

void MixClass::Create_Mix(const char *name)
{
	MixFileCreator mix(name);
	int c = count;
	if (c == -1)
	{
		c = count2;
	}
	qsort(files,c,sizeof(FileInfo),File_Info_Compare);
	for (int i = 0;i < c;i++)
	{
		mix.Add_File(files[i].path->path,files[i].path->name);
	}
}

MixFileCreator::~MixFileCreator()
{
	if (File)
	{
		int fpos1 = File->Tell();
		int count = Files.Count();
		File->Write(&count,4);
		if (count > 1)
		{
			qsort(&Files[0],count,0x10,File_Info_Compare);
		}
		for (int i = 0;i < count;i++)
		{
			File->Write(&Files[i].CRC,4);
			File->Write(&Files[i].pos,4);
			File->Write(&Files[i].size,4);
		}
		int fpos2 = File->Tell();
		File->Write(&count,4);
		for (int i = 0;i < count;i++)
		{
			int len = Files[i].name.Get_Length() + 1;
			File->Write(&len,1);
			File->Write(Files[i].name.Peek_Buffer(),len);
		}
		File->Seek(4,0);
		File->Write(&fpos1,4);
		File->Write(&fpos2,4);
		File->Close();
		SimpleFileFactory.Return_File(File);
	}
}

void MixFileCreator::Add_File(char  const *name, char  const *name2)
{
	const char *fname = name2;
	if (!name2)
	{
		fname = name;
	}
	if (File)
	{
		FileClass *file = SimpleFileFactory.Get_File(name);
		if (file)
		{
			if (file->Is_Available())
			{
				file->Open(1);
				FileInfoStruct str;
				str.pos = File->Tell();
				str.size = file->Size();
				str.name = fname;

                NormalizeFilename(str.name.Peek_Buffer());
                str.CRC = CRC_Stringi(str.name.Peek_Buffer());

				Files.Add(str);
				int size = file->Size();
				char buffer[4096];
				while (size)
				{
					int sz = size;
					if (size > 4096)
					{
						sz = 4096;
					}
					size -= sz;
					file->Read(buffer,sz);
					File->Write(buffer,sz);
				}
				int tell = File->Tell() & 7;
				if (-tell & 7)
				{
					int i[2] = {0,0};
					File->Write(i,-tell & 7);
				}
				file->Close();
				SimpleFileFactory.Return_File(file);
			}
		}
	}
}

class IndexEntry : public NoEqualsClass<IndexEntry>
{
public:
	StringClass name;
	FileFactoryClass *factory;
	unsigned int id;
	unsigned int size;
	unsigned int offset;
};

int Compare_Entry(const void *v1, const void *v2)
{
	int ret = -1;
	if (((IndexEntry *)v1)->id >= ((IndexEntry *)v2)->id)
	{
		ret = ((IndexEntry *)v1)->id > ((IndexEntry *)v2)->id;
	}
	return ret;
}
void CopyTo(FileClass &out,FileClass &in)
{
	char buffer[10240];
	int remaining = in.Size();
	while (0 != remaining)
	{
		int nBytes = (remaining > sizeof(buffer)) ? sizeof(buffer) : remaining;
		in.Read(buffer,nBytes);
		out.Write(buffer,nBytes);
		remaining -= nBytes;
	}
}
void BuildMixFile(const char *path,DynamicVectorClass<StringClass> filenames,DynamicVectorClass<FileFactoryClass *> filefactories)
{
	DynamicVectorClass<IndexEntry> entries;
	for (int i = 0;i < filenames.Count();i++)
	{
		IndexEntry entry;
		StringClass fname = filenames[i];
		NormalizeFilename(fname.Peek_Buffer());
		entry.name = fname.Peek_Buffer();
		entry.id = CRC_Stringi(entry.name.Peek_Buffer());
		entry.factory = filefactories[i];
		entries.Add(entry);
	}
	qsort(&entries[0],entries.Count(),sizeof(IndexEntry),Compare_Entry);
	RawFileClass out(path);
	out.Open(0);
	int m = 0x3158494D;
	out.Write(&m,sizeof(m));
	out.Seek(16,SEEK_SET);
	for (int i = 0;i < entries.Count();i++)
	{
		FileClass *in = entries[i].factory->Get_File(entries[i].name);
		in->Open(1);
		entries[i].size = in->Size();
		entries[i].offset = out.Tell();
		CopyTo(out,*in);
		int pos = out.Tell();
		pos += -pos & 7;
		out.Seek(pos,SEEK_SET);
		entries[i].factory->Return_File(in);
	}
	int index_offset = out.Tell();
	int count = entries.Count();
	out.Write(&count,sizeof(count));
	for (int i = 0;i < entries.Count();i++)
	{
		out.Write(&entries[i].id,sizeof(entries[i].id));
		out.Write(&entries[i].offset,sizeof(entries[i].offset));
		out.Write(&entries[i].size,sizeof(entries[i].size));
	}
	int filename_offset = out.Tell();
	out.Write(&count,sizeof(count));
	for (int i = 0;i < entries.Count();i++)
	{
		char length = (char)(entries[i].name.Get_Length() + 1);
		out.Write(&length,sizeof(length));
		out.Write(entries[i].name.Peek_Buffer(),length);
	}
	out.Seek(4,SEEK_SET);
	out.Write(&index_offset,sizeof(index_offset));
	out.Write(&filename_offset,sizeof(filename_offset));
	out.Close();
}
