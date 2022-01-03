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
#pragma warning(disable: 4073) //warning C4073: initializers put in library initialization area - That's EXACTLY why I put that pragma in...
#pragma init_seg(lib) // Move this files static initializers up a level
#pragma warning(default: 4073)
#include "engine_io.h"
#include "straw.h"
#include "BufferedFileClass.h"
#include "Crc32.h"
#include <shlobj.h>
void SCRIPTS_API Strip_Path_From_Filename(StringClass& target, const char* fileName)
{
	if (strchr(fileName,'\\'))
	{
		const char *c = strrchr(fileName,'\\');
		c++;
		target = c;
	}
	else
	{
		target = fileName;
	}
}
#ifndef TTLE_EXPORTS
#ifndef DDBEDIT
#ifndef EXTERNAL
SCRIPTS_API REF_DEF2(FileFactoryClass *, _TheFileFactory, 0x00809E74, 0x0080904C);
#else
FileFactoryClass *_TheFileFactory;
#endif
FileClass SCRIPTS_API *Get_Data_File(const char *file)
{
	return _TheFileFactory->Get_File(file);
}

void SCRIPTS_API Close_Data_File(FileClass *file)
{
	_TheFileFactory->Return_File(file);
}
char FilePath[MAX_PATH];
char AppDataPath[MAX_PATH];
char RegPath[MAX_PATH];
bool PathsInit = false;
void Read_Paths()
{
#ifndef EXTERNAL
	const bool isClient = (Exe == 0);
#else
#if defined(PACKAGEEDITOR)
	const bool isClient = false;
#else
	const bool isClient = true;
#endif
#endif

	char RegBase[MAX_PATH];
	char RegClient[MAX_PATH];
	char RegFDS[MAX_PATH];
	char FileBase[MAX_PATH];
	char FileClient[MAX_PATH];
	char FileFDS[MAX_PATH];

	bool UseRenFolder = !isClient;
	INIClass *ini = Get_INI("paths.ini");
	if (ini)
	{
		ini->Get_String("paths","RegBase","Westwood",RegBase,MAX_PATH);
		ini->Get_String("paths","RegClient","Renegade",RegClient,MAX_PATH);
		ini->Get_String("paths","RegFDS","RenegadeFDS",RegFDS,MAX_PATH);
		ini->Get_String("paths","FileBase","Renegade",FileBase,MAX_PATH);
		ini->Get_String("paths","FileClient","Client",FileClient,MAX_PATH);
		ini->Get_String("paths","FileFDS","FDS",FileFDS,MAX_PATH);
		UseRenFolder = ini->Get_Bool("paths","UseRenFolder",UseRenFolder);
		Release_INI(ini);
	}
	else
	{
		strcpy(RegBase,"Westwood");
		strcpy(RegClient,"Renegade");
		strcpy(RegFDS,"RenegadeFDS");
		strcpy(FileBase,"Renegade");
		strcpy(FileClient,"Client");
		strcpy(FileFDS,"FDS");
	}
	FilePath[0] = 0;
	if (!UseRenFolder)
	{
		wchar_t fpath[MAX_PATH];
		SHGetSpecialFolderPathW(NULL, fpath, CSIDL_MYDOCUMENTS, FALSE);
		bool unicode = false;
		for (int i = 0;fpath[i] != 0;i++)
		{
			unsigned short value = fpath[i];
			if (value > 255)
			{
				unicode = true;
				break;
			}
		}
		if (unicode)
		{
			GetModuleFileName(NULL,FilePath,MAX_PATH);
			strrchr(FilePath,'\\')[0] = 0;
		}
		else
		{
			SHGetSpecialFolderPath(NULL,  FilePath, CSIDL_MYDOCUMENTS, FALSE);
		}
	}
	else
	{
		GetModuleFileName(NULL,FilePath,MAX_PATH);
		strrchr(FilePath,'\\')[0] = 0;
	}
	strcat(FilePath,"\\");
	strcat(FilePath,FileBase);
	strcat(FilePath,"\\");
	strcat(FilePath,isClient ? FileClient : FileFDS);
	strcat(FilePath,"\\");

	AppDataPath[0] = 0;
	if (!UseRenFolder)
	{
		wchar_t fpath[MAX_PATH];
		SHGetSpecialFolderPathW(NULL, fpath, CSIDL_MYDOCUMENTS, FALSE);
		bool unicode = false;
		for (int i = 0;fpath[i] != 0;i++)
		{
			unsigned short value = fpath[i];
			if (value > 255)
			{
				unicode = true;
				break;
			}
		}
		if (unicode)
		{
			GetModuleFileName(NULL,AppDataPath,MAX_PATH);
			strrchr(AppDataPath,'\\')[0] = 0;
		}
		else
		{
			SHGetSpecialFolderPath(NULL, AppDataPath, CSIDL_APPDATA, FALSE);
		}
	}
	else
	{
		GetModuleFileName(NULL,AppDataPath,MAX_PATH);
		strrchr(AppDataPath,'\\')[0] = 0;
	}
	strcat(AppDataPath,"\\");
	strcat(AppDataPath,FileBase);
	strcat(AppDataPath,"\\");
	strcat(AppDataPath,isClient ? FileClient : FileFDS);
	strcat(AppDataPath,"\\");

	strcpy(RegPath,"Software\\");
	strcat(RegPath,RegBase);
	strcat(RegPath,"\\");
	strcat(RegPath,isClient ? RegClient : RegFDS);

	PathsInit = true;
}
const char SCRIPTS_API *Get_File_Path()
{
	if (!PathsInit)
	{
		Read_Paths();
	}
	return FilePath;
}
const char SCRIPTS_API *Get_App_Data_Path()
{
	if (!PathsInit)
	{
		Read_Paths();
	}
	return AppDataPath;
}
const char SCRIPTS_API *Get_Registry_Path()
{
	if (!PathsInit)
	{
		Read_Paths();
	}
	return RegPath;
}
#endif
#endif
#ifndef TTLE_EXPORTS
#ifndef DDBEDIT
INIClass SCRIPTS_API *Get_INI(char const *filename)
{
	INIClass *ini = 0;
	FileClass *f = _TheFileFactory->Get_File(filename);
	if (f)
	{
		if (f->Is_Available())
		{
			ini = new INIClass(*f);
		}
		_TheFileFactory->Return_File(f);
	}
	return ini;
}

void SCRIPTS_API Release_INI(INIClass *ini)
{
	if (ini)
	{
		delete ini;
	}
}

int INIClass::Section_Count() const
{
	return SectionIndex->Count();
}

int INIClass::Load(FileClass& file)
{
	FileStraw straw(file);
	if (Filename)
	{
		delete[] Filename;
	}
	Filename = newstr(file.File_Name());
	return Load(straw);
}

#ifndef EXTERNAL
RENEGADE_FUNCTION
bool INIClass::Put_Wide_String(const char* section, const char* entry, const wchar_t* string)
AT2(0x005E08B0,0x005E0150);
RENEGADE_FUNCTION
WideStringClass &INIClass::Get_Wide_String(WideStringClass &,char  const*,char  const*,wchar_t  const*) const
AT2(0x005E06E0,0x005DFF80);
#endif

int Read_Line(Straw& straw,char* line,int lineSize,bool& isLast)
{
		if (!line || lineSize == 0)
				return 0;
		int i;
		for (i = 0;;)
		{
				char c;
				int getResult = straw.Get(&c, 1);
				if (getResult != 1)
				{
						isLast = true;
						break;
				}
				if (c == '\n')
						break;
				if (c != '\r' && i + 1 < lineSize)
						line[i++] = c;
		}
		line[i] = '\0';
		strtrim(line);
		return (int)strlen(line);
}

int INIClass::Load(Straw& straw)
{
		bool isLastLine = false;
		CacheStraw cacheStraw(4096);
		cacheStraw.Get_From(&straw);
		char line[512];
 
		// Ignore everything above first section (indicated by a line like "[sectionName]")
		while (!isLastLine)
		{
				Read_Line(cacheStraw, line, 512, isLastLine);
				if (isLastLine)
						return false;
				if (line[0] == '[' && strchr(line, ']'))
						break;
		}
 
		if (Section_Count() > 0)
		{
				while (!isLastLine)
				{
						TT_ASSERT(line[0] == '[' && strchr(line, ']')); // at start of section
						line[0] = ' ';
						*strchr(line, ']') = '\0';
						strtrim(line);
						char sectionName[64];
						strcpy(sectionName, line);
						while (!isLastLine)
						{
								int count = Read_Line(cacheStraw, line, 512, isLastLine);
								if (line[0] == '[' && strchr(line, ']'))
										break;
								Strip_Comments(line);
								if (count)
								{
										if (line[0] != ';')
										{
												if (line[0] != '=')
												{
														char* delimiter = strchr(line, '=');
														if (delimiter)
														{
																*delimiter = '\0';
																char* key = line;
																char* value = delimiter + 1;
																strtrim(key);
																if (key[0] != '\0')
																{
																		strtrim(value);
																		if (value[0] == '\0')
																		{
																				continue;
																		}
																		if (!Put_String(sectionName, key, value))
																				return false;
																}
														}
												}
										}
								}
						}
				}
		}
		else
		{
				while (!isLastLine)
				{
						TT_ASSERT(line[0] == '[' && strchr(line, ']')); // at start of section
						line[0] = ' ';
						*strchr(line, ']') = '\0';
						strtrim(line);
						INISection* section = new INISection(newstr(line));
						if (!section)
						{
								Clear(0, 0);
								return false;
						}
						while (!isLastLine)
						{
								int count = Read_Line(cacheStraw, line, 512, isLastLine);
								if (line[0] == '[' && strchr(line, ']'))
										break;
								Strip_Comments(line);
								char* delimiter = strchr(line, '=');
								if (count)
								{
										if (line[0] != ';')
										{
												if (line[0] != '=')
												{
														if (delimiter)
														{
																*delimiter = '\0';
																char* key = line;
																char* value = delimiter + 1;
																strtrim(key);
																if (key[0] != '\0')
																{
																		strtrim(value);
																		if (value[0] == '\0')
																		{
																				continue;
																		}
																		INIEntry* entry = new INIEntry(newstr(key), newstr(value));
																		if (!entry)
																		{
																				delete section;
																				Clear(0, 0);
																				return false;
																		}
																		uint32 crc = CRC_String(entry->Entry, 0);
																		if (section->EntryIndex.Is_Present(crc))
																				DuplicateCRCError(__FUNCTION__, section->Section, line);
																		section->EntryIndex.Add_Index(crc, entry);
																		section->EntryList.Add_Tail(entry);
																}
														}
												}
										}
								}
						}
						if (section->EntryList.Is_Empty())
						{
								delete section;
						}
						else
						{
								uint32 crc = CRC_String(section->Section, 0);
								SectionIndex->Add_Index(crc, section);
								SectionList->Add_Tail(section);
						}
				}
		}
		return true;
}

StringClass &INIClass::Get_String(StringClass& string, const char* section, const char* entry, const char *defaultvalue) const
{
	const char *value = defaultvalue;
	if (!section || !entry)
	{
		string = "";
	}
	INIEntry *Entry = Find_Entry(section,entry);
	if (Entry)
	{
		value = Entry->Value;
	}
	if (value)
	{
		string = value;
	}
	else
	{
		string = "";
	}
	return string;
}

StringClass& INIClass::Get_String_Advanced(StringClass& string, const char* section, const char* entry, const char* defaultvalue, bool updateIfNotFound) const
{
	const char* value = defaultvalue;
	if (!section || !entry)
	{
		string = "";
	}
	INIEntry* Entry = Find_Entry(section, entry);
	if (Entry)
	{
		value = Entry->Value;
	}
	else if (!updateIfNotFound)
	{
		return string;
	}

	if (value)
	{
		string = value;
	}
	else
	{
		string = "";
	}
	return string;
}
INISection *INIClass::Find_Section(const char* section) const
{
	if (section)
	{
		/*int crc = CRC_String(section,0); //DA
		if (SectionIndex->Is_Present(crc))
		{
			return (*SectionIndex)[crc];
		}*/
		for (INISection *i = SectionList->First(); i && i->Is_Valid(); i = i->Next()) { //DA
			if (!_stricmp(section, i->Section)) {
				return i;
			}
		}
	}
	return 0;
}
INIEntry *INIClass::Find_Entry(const char* section,const char* entry) const
{
	INISection *Section = Find_Section(section);
	if (Section)
	{
		return Section->Find_Entry(entry);
	}
	return 0;
}
int INIClass::Get_Int(char const *section,char const *entry,int defaultvalue) const
{
	if (section)
	{
		if (entry)
		{
			INIEntry *Entry = Find_Entry(section,entry);
			if (Entry)
			{
				if (Entry->Value)
				{
					int *value;
					const char *pattern;
					if (Entry->Value[0] == '$')
					{
						value = &defaultvalue;
						pattern = "$%x";
					}
					else
					{
						if (tolower(Entry->Value[strlen(Entry->Value) - 1]) != 'h')
						{
							return atoi(Entry->Value);
						}
						value = &defaultvalue;
						pattern = "%xh";
					}
#pragma warning(suppress: 6031) //warning C6031: return value ignored
					sscanf(Entry->Value, pattern, value);
				}
			}
		}
	}
	return defaultvalue;
}
float INIClass::Get_Float(char const *section,char const *entry,float defaultvalue) const
{
	if (section)
	{
		if (entry)
		{
			INIEntry *Entry = Find_Entry(section,entry);
			if (Entry)
			{
				if (Entry->Value)
				{
					float c = defaultvalue;
#pragma warning(suppress: 6031) //warning C6031: return value ignored
					sscanf(Entry->Value, "%f", &c);
					defaultvalue = c;
					if (strchr(Entry->Value, '%'))
					{
						defaultvalue = defaultvalue / 100.0f;
					}
				}
			}
		}
	}
	return defaultvalue;
}
bool INIClass::Get_Bool(char const *section,char const *entry,bool defaultvalue) const
{
	if (section)
	{
		if (entry)
		{
			INIEntry *Entry = Find_Entry(section,entry);
			if (Entry)
			{
				if (Entry->Value)
				{
					switch ( toupper(Entry->Value[0]) )
					{
						case '1':
						case 'T':
						case 'Y':
							return true;
							break;
						case '0':
						case 'F':
						case 'N':
							return false;
							break;
					}
				}
			}
		}
	}
	return defaultvalue;
}
int INIClass::Get_String(char const *section,char const *entry,char const *defaultvalue,char *result,int size) const
{
	if (!result || size <= 1 || !section || !entry)
	{
		return 0;
	}
	INIEntry *Entry = Find_Entry(section,entry);
	const char *value = defaultvalue;
	if (Entry)
	{
		if (Entry->Value)
		{
			value = Entry->Value;
		}
	}
	if (!value)
	{
		result[0] = 0;
		return 0;
	}
	strncpy(result, value, size);
	result[size - 1] = 0;
	strtrim(result);
	return (int)strlen(result);
}
int INIClass::Get_String_Advanced(char const *section,char const *entry,char const *defaultvalue,char *result,int size,bool updateIfNotFound) const
{
	if (!result || size <= 1 || !section || !entry)
		return 0;
	INIEntry *Entry = Find_Entry(section,entry);
	const char *value = defaultvalue;
	if (Entry && Entry->Value)
		value = Entry->Value;
	if ((!Entry || !Entry->Value) && !updateIfNotFound) 
		return 0;
	if (!value)
	{
		result[0] = 0;
		return 0;
	}
	strncpy(result, value, size);
	result[size - 1] = 0;
	strtrim(result);
	return (int)strlen(result);
}
int INIClass::Entry_Count(char const *section) const
{
	INISection *Section = Find_Section(section);
	if (Section)
	{
		return Section->EntryIndex.Count();
	}
	return 0;
}
const char *INIClass::Get_Entry(char const *section,int index) const
{
	int count = index;
	INISection *Section = Find_Section(section);
	if (Section)
	{
		if (Section && index < Section->EntryIndex.Count())
		{
			for (INIEntry *i = Section->EntryList.First();i; i = i->Next())
			{
				if (!i->Is_Valid())
				{
					break;
				}
				if (!count)
				{
					return i->Entry;
				}
				count--;
			}
		}
	}
	return 0;
}
void INIClass::Initialize()
{
	SectionList = new List<INISection *>;
	SectionIndex = new IndexClass<int,INISection *>;
	Filename = newstr("<unknown>");
}
void INIClass::Shutdown()
{
	if (SectionList)
	{
		delete SectionList;
	}
	if (SectionIndex)
	{
		delete SectionIndex;
	}
	if (Filename)
	{
		delete[] Filename;
	}
}
bool INIClass::Clear(char* section,char* entry)
{
	if (section)
	{
		INISection *Section = Find_Section(section);
		if (Section)
		{
			if (entry)
			{
				INIEntry *Entry = Section->Find_Entry(entry);
				if (!Entry)
				{
					return true;
				}
				Section->EntryIndex.Remove_Index(CRC_String(Entry->Entry,0));
				delete Entry;
				return true;
			}
			else
			{
				SectionIndex->Remove_Index(CRC_String(Section->Section,0));
				delete Section;
				return true;
			}
		}
	}
	else
	{
		SectionList->Delete();
		SectionIndex->Clear();
		delete[] Filename;
		Filename = newstr("<unknown>");
	}
	return true;
}
int INIClass::CRC(char *string)
{
	return CRC_String(string,0);
}

INIClass::INIClass()
{
	Filename = 0;
	Initialize();
}

INIClass::INIClass(FileClass &file)
{
	Filename = 0;
	Initialize();
	Load(file);
}

INIClass::~INIClass()
{
	Clear(0,0);
	Shutdown();
}

uint INIClass::Get_Color_UInt(char const *section, char const *entry, uint defaultvalue) const
{
	char buf[256], hex[10];

	sprintf(buf,"%sHex",entry);
	this->Get_String(section, buf, "0xNotValid", hex, 10); 
	if (strcmp(hex, "0xNotValid") != 0) // We've got us a supposedly valid hex value
	{
		uint color;
		int res = sscanf(hex, "%lx", &color);
		if (res == 1) return color; // Yay, we've got a color. Party!
	};

	uint a, r, g, b;
	sprintf(buf,"%sAlpha",entry);
	a = this->Get_Int(section, buf, (defaultvalue >> 24) & 0xFF);
	sprintf(buf,"%sRed",entry);
	r = this->Get_Int(section, buf, (defaultvalue >> 16) & 0xFF);
	sprintf(buf,"%sGreen",entry);
	g = this->Get_Int(section, buf, (defaultvalue >> 8) & 0xFF);
	sprintf(buf,"%sBlue",entry);
	b = this->Get_Int(section, buf, defaultvalue & 0xFF);

	return ((a & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
};

void INIClass::Strip_Comments(char* buffer)
{
	if (buffer)
	{
		char *buf = strrchr(buffer,';');
		if (buf)
		{
			buf[0] = 0;
			strtrim(buffer);
		}
	}
}

void INIClass::DuplicateCRCError(const char *function,const char* section,const char* entry)
{
	char OutputString[512];
	_snprintf(OutputString,512,"%s - Duplicate Entry \"%s\" in section \"%s\" (%s)\n",function,entry,section,Filename);
	OutputString[511] = 0;
	OutputDebugString(OutputString);
	MessageBox(0,OutputString,"Duplicate CRC in INI file.",16);
}

int INIClass::Load(char* filename)
{
	file_auto_ptr ptr(_TheFileFactory,filename);
	int ret = Load(*ptr);
	if (Filename)
	{
		delete[] Filename;
	}
	Filename = newstr(filename);
	return ret;
}

int INIClass::Save(FileClass& file)
{
	FilePipe pipe(&file);
	if (Filename)
	{
		delete[] Filename;
	}
	Filename = newstr(file.File_Name());
	return Save(pipe);
}

int INIClass::Save(Pipe& pipe)
{
	int pos = 0;
	for (INISection *i = SectionList->First();i;i = i->Next())
	{
		if (!i->Is_Valid())
		{
			break;
		}
		int i1 = pipe.Put("[",1) + pos;
		int i2 = pipe.Put(i->Section,(int)strlen(i->Section)) + i1;
		int i3 = pipe.Put("]",1) + i2;
		int i4 = pipe.Put("\n",(int)strlen("\n")) + i3;
		for (INIEntry *j = i->EntryList.First();j;j = j->Next())
		{
			if (!j->Is_Valid())
			{
				break;
			}
			int i5 = pipe.Put(j->Entry,(int)strlen(j->Entry)) + i4;
			int i6 = pipe.Put("=",1) + i5;
			int i7 = pipe.Put(j->Value,(int)strlen(j->Value)) + i6;
			i4 = pipe.Put("\n",(int)strlen("\n")) + i7;
		}
		pos = pipe.Put("\n",(int)strlen("\n")) + i4;
	}
	return pipe.End() + pos;
}

bool INIClass::Put_String(const char* section, const char* entry, const char* string)
{
	if (!section || !entry)
	{
		return false;
	}
	INISection *sec = Find_Section(section);
	if (!sec)
	{
		sec = new INISection(newstr(section));
		SectionList->Add_Tail(sec);
		SectionIndex->Add_Index(CRC_String(sec->Section,0),sec);
	}
	INIEntry *ent = sec->Find_Entry(entry);
	if (ent)
	{
		if (strcmp(ent->Entry,entry))
		{
			DuplicateCRCError("INIClass::Put_String",section,entry);
		}
		SectionIndex->Remove_Index(CRC_String(ent->Entry,0));
		if (ent)
		{
			delete ent;
		}
	}
	if (string && *string)
	{
		ent = new INIEntry(newstr(entry),newstr(string));
		sec->EntryList.Add_Tail(ent);
		sec->EntryIndex.Add_Index(CRC_String(ent->Entry,0),ent);
	}
	return true;
}

bool INIClass::Put_Int(const char* section, const char* entry, int value, int format)
{
	char *form;
	if (format == 1)
	{
		form = "%Xh";
	}
	else
	{
		if (format > 1 && format == 2)
		{
			form = "$%X";
		}
		else
		{
			form = "%d";
		}
	}
	char buf[524];
	sprintf(buf,form,value);
	return Put_String(section,entry,buf);
}

bool INIClass::Put_Bool(const char* section, const char* entry, bool value)
{
	char *str;
	if (value)
	{
		str = "yes";
	}
	else
	{
		str = "no";
	}
	return Put_String(section,entry,str);
}

bool INIClass::Put_Float(const char* section, const char* entry, float value)
{
	char buf[524];
	sprintf(buf,"%f",value);
	return Put_String(section,entry,buf);
}

unsigned int SCRIPTS_API Get_Registry_Int(const char *entry,int defaultvalue)
{
	unsigned int value;
	HKEY key;
	LONG error = RegOpenKeyEx(HKEY_CURRENT_USER,Get_Registry_Path(),0,KEY_READ,&key);
	value = 0;
	unsigned long size = 4;
	unsigned long type;
	error = RegQueryValueEx(key,entry,0,&type,(BYTE *)&value,&size);
	if (error == ERROR_FILE_NOT_FOUND)
	{
		value = defaultvalue;
	}
	RegCloseKey(key);
	return value;
}

file_auto_ptr::file_auto_ptr(FileFactoryClass* fac, const char* filename)
{
	_Ptr = 0;
	_Fac = fac;
	_Ptr = fac->Get_File(filename);
	if (!_Ptr)
	{
		_Ptr = new BufferedFileClass();
	}
}

file_auto_ptr::~file_auto_ptr()
{
	_Fac->Return_File(_Ptr);
}
#endif
#endif