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

#include "scripts.h"
#include "engine.h"
#define	CHUNKID_SCRIPTHEADER				'shdr'
#define	CHUNKID_SCRIPTDATA				'sdat'
#define	CHUNKID_SCRIPT_AUTO_VARIABLES	'csav'
SCRIPTS_API ScriptCommands* Commands = NULL;
void (*ScriptImpClass::Request_Destroy_Script)(ScriptClass*) =  NULL;
ScriptImpClass::ScriptImpClass() : mOwner(NULL),mArgC(0),mArgV(NULL),AutoVariableList(NULL)
{
}

const char *ScriptImpClass::Get_Name()
{
	return mFactory->GetName();
}

void ScriptImpClass::Attach(GameObject *newobj)
{
	mOwner = newobj;
}

void ScriptImpClass::Set_Request_Destroy_Func(void (*function)(ScriptClass*))
{
	Request_Destroy_Script = function;
}

void ScriptImpClass::Destroy_Script()
{
	if (Request_Destroy_Script != NULL)
	{
		Request_Destroy_Script(this);
	}
}

void ScriptImpClass::Detach(GameObject *obj)
{
	mOwner = NULL;
	Destroy_Script();
}

void ScriptImpClass::Set_Parameters_String(const char *params)
{
	if ((params == NULL) || (strlen(params) == 0))
	{
		return;
	}
	char* working = newstr(params);
	if ( strlen( working ) && working[ strlen( working ) - 1 ] == '\n' )
	{
		working[ strlen( working ) - 1 ] = 0;
	}
	int count = 0;
	char * param_ptr = working;
	if ( *param_ptr )
	{
		count++;
	}
	for ( param_ptr = working; *param_ptr; param_ptr++ )
	{
		if ( *param_ptr == ',' )
		{
			count++;
		}
	}
	if (count == 0)
	{
		return;
	}
	Clear_Parameters();
	mArgC = count;
	mArgV = new char*[count];
	TT_ASSERT(mArgV != NULL);
	memset(mArgV, 0, (sizeof(char*) * count));
	strcpy(working, params);
	count = 0;
	param_ptr = working;
	while ( *param_ptr )
	{
		char * next = param_ptr;
		while ( *next && *next != ',' )
		{
			next++;
		}
		if ( *next )
		{
			*next++ = 0;
		}
		Set_Parameter(count++, param_ptr);
		param_ptr = next;
	}
	if (count == mArgC - 1)
	{
		Set_Parameter(count, "");
	}
	delete[] working;
}

void ScriptImpClass::Get_Parameters_String(char* buffer, unsigned int size)
{
	buffer[0] = '\0';
	int count = Get_Parameter_Count();
	int i = 0;
	unsigned int stringLen = 0;
	while ((i < count) && (stringLen < size))
	{
		if (i > 0)
		{
			stringLen++;
			strcat(buffer, ",");
		}
		const char* param = Get_Parameter(i);
		stringLen += strlen(param);
		if (stringLen <= size)
		{
			strcat(buffer, param);
			i++;
		}
	}
}

void ScriptImpClass::Save(ScriptSaver& saver)
{
	ScriptVariableClass * var = AutoVariableList;
	if ( var != NULL )
	{
		Commands->Begin_Chunk(saver, CHUNKID_SCRIPT_AUTO_VARIABLES);
		while ( var != NULL )
		{
			Commands->Save_Data(saver, var->Get_ID(), var->Get_Data_Size(), var->Get_Data_Ptr() );
			var = var->Get_Next();
		}
		Commands->End_Chunk(saver);
	}
}

void ScriptImpClass::Load(ScriptLoader & loader)
{
	unsigned int chunkID;
	while (Commands->Open_Chunk(loader, &chunkID))
	{
		switch (chunkID)
		{
			case CHUNKID_SCRIPT_AUTO_VARIABLES:
			{
				int id;
				while (Commands->Load_Begin(loader, &id))
				{
					ScriptVariableClass * var = AutoVariableList;
					while ( var != NULL )
					{
						if ( var->Get_ID() == id )
						{
							Commands->Load_Data(loader, var->Get_Data_Size(), var->Get_Data_Ptr() );
						}
						var = var->Get_Next();
					}
					Commands->Load_End(loader);
				}
			}
			break;
		}
		Commands->Close_Chunk(loader);
	}
}

int ScriptImpClass::CheckPlayerType(GameObject *obj,int type)
{
	if (type == 3)
	{
		GameObject *object = Owner();
		Vector3 pos = Commands->Get_Position(object);
		object = Commands->Get_A_Star(pos);
		if (object == obj)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	if (type == 2)
	{
		return 0;
	}
	int pltype = Commands->Get_Player_Type(obj);
	if (pltype == type)
	{
		return 0;
	}
	return 1;
}

void	ScriptImpClass::Auto_Save_Variable( void * data_ptr, int data_size, int id )
{
	if ( ( id < 0 ) || ( id > 255 ) )
	{
		return;
	}
	ScriptVariableClass * var = AutoVariableList;
	while ( var != NULL )
	{
		if ( var->Get_ID() == id )
		{
			return;
		}
		var = var->Get_Next();
	}
	if ( data_size > 250 )
	{
		return;
	}
	AutoVariableList = new ScriptVariableClass( data_ptr, data_size, id, AutoVariableList );
}

ScriptImpClass::~ScriptImpClass() 
{
	Clear_Parameters();
	while ( AutoVariableList != NULL )
	{
		ScriptVariableClass * next = AutoVariableList->Get_Next();
		delete AutoVariableList;
		AutoVariableList = next;
	}
}

void ScriptImpClass::Clear_Parameters()
{
	if (mArgV != NULL)
	{
		while (mArgC--)
		{
			if (mArgV[mArgC] != NULL)
			{
				delete[] mArgV[mArgC];
			}
		}
		delete[] mArgV;
	}
	mArgC = 0;
	mArgV = NULL;
}

const char* ScriptImpClass::Get_Parameter(const char* name)
{
	return Get_Parameter(Get_Parameter_Index(name));
}

int ScriptImpClass::Get_Int_Parameter(const char* parameterName)
{
	int index = Get_Parameter_Index(parameterName);
	return Get_Int_Parameter(index);
}

float ScriptImpClass::Get_Float_Parameter(const char* parameterName)
{
	int index = Get_Parameter_Index(parameterName);
	return Get_Float_Parameter(index);
}

Vector3 ScriptImpClass::Get_Vector3_Parameter(const char *parameterName)
{
	return Get_Vector3_Parameter(Get_Parameter_Index(parameterName));
}

int ScriptImpClass::Get_Parameter_Index(const char* parameterName)
{
	const char* paramDesc = mFactory->GetParamDescription();
	char string[8192];
	strncpy(string, paramDesc, (sizeof(string) - 1));
	string[sizeof(string)-1] = '\0';
	int index = 0;
	char * param_ptr = string;
	while ( *param_ptr )
	{
		char * next = param_ptr;
		while ( *next && *next != ',' )
		{
			next++;
		}
		if ( *next )
		{
			*next++ = 0;
		}
		char* tokenEnd = strpbrk(param_ptr, "=:\n");
		if (tokenEnd != NULL)
		{
			*tokenEnd = '\0';
		}
		strtrim(param_ptr);
		if (_stricmp(param_ptr, parameterName) == 0)
		{
			return index;
		}
		index++;
		param_ptr = next;
	}
	const char *path2 = Get_File_Path();
	char path[MAX_PATH];
	strcpy(path,path2);
	strcat(path,"critical.txt");
	FILE *f = fopen(path,"at");
	if (f)
	{
		fprintf(f,"Critical error the name is %s parameter is %s the map is %s\n",this->Get_Name(),parameterName,The_Game()->Get_Map_Name().Peek_Buffer());
		fclose(f);
	}
	return -1;
}

void ScriptImpClass::Set_Parameter(int index, const char* str)
{
	if (mArgV[index] != NULL)
	{
		delete[] mArgV[index];
	}
	mArgV[index] = newstr(str);
}

const char* ScriptImpClass::Get_Parameter(int index)
{
	if ((index < 0) || (index >= mArgC))
	{
		return "";
	}
	return mArgV[index];
}

Vector3 ScriptImpClass::Get_Vector3_Parameter(int index)
{
	float x,y,z;
	if (::sscanf(Get_Parameter(index),"%f %f %f",&x,&y,&z) != 3)
	{
		return Vector3(0,0,0);
	}
	return Vector3( x,y,z );
}
