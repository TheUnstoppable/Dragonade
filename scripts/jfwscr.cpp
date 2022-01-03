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
#include "jfwscr.h"

void JFW_Attatch_Script::Entered(GameObject *obj,GameObject *enterer)
{
	const char *script;
	const char *param;
	char *params;
	char delim;
	int pltype;
	int destroy;
	int trigger;
	trigger = Get_Int_Parameter("Trigger");
	if (trigger != 1)
	{
		return;
	}
	pltype = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,pltype))
	{
		return;
	}
	script = Get_Parameter("Script_Name");
	param = Get_Parameter("Script_Params");
	params = newstr(param);
	delim = Get_Parameter("Delim")[0];
	unsigned int x = strlen(params);
	for (unsigned int i=0;i<x;i++)
	{
		if (params[i] == delim)
		{
			params[i] = ',';
		}
	}
	Commands->Attach_Script(enterer,script,params);
	delete[] params;
	destroy = Get_Int_Parameter("Destroy");
	if (destroy == 1)
	{
		Commands->Destroy_Object(obj);
	}
}

void JFW_Attatch_Script::Killed(GameObject *obj,GameObject *killer)
{
	const char *script;
	const char *param;
	char *params;
	char delim;
	int destroy;
	int trigger;
	trigger = Get_Int_Parameter("Trigger");
	if (trigger != 2)
	{
		return;
	}
	script = Get_Parameter("Script_Name");
	param = Get_Parameter("Script_Params");
	params = newstr(param);
	delim = Get_Parameter("Delim")[0];
	unsigned int x = strlen(params);
	for (unsigned int i=0;i<x;i++)
	{
		if (params[i] == delim)
		{
			params[i] = ',';
		}
	}
	Commands->Attach_Script(killer,script,params);
	delete[] params;
	destroy = Get_Int_Parameter("Destroy");
	if (destroy == 1)
	{
		Commands->Destroy_Object(obj);
	}
}

void JFW_Attatch_Script::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	const char *script;
	const char *paramx;
	char *params;
	char delim;
	int msg;
	int destroy;
	int trigger;
	trigger = Get_Int_Parameter("Trigger");
	if (trigger != 3)
	{
		return;
	}
	msg = Get_Int_Parameter("Custom");
	if (type != msg)
	{
		return;
	}
	script = Get_Parameter("Script_Name");
	paramx = Get_Parameter("Script_Params");
	params = newstr(paramx);
	delim = Get_Parameter("Delim")[0];
	unsigned int x = strlen(params);
	for (unsigned int i=0;i<x;i++)
	{
		if (params[i] == delim)
		{
			params[i] = ',';
		}
	}
	Commands->Attach_Script(sender,script,params);
	delete[] params;
	destroy = Get_Int_Parameter("Destroy");
	if (destroy == 1)
	{
		Commands->Destroy_Object(obj);
	}
}

void JFW_Attatch_Script::Poked(GameObject *obj,GameObject *poker)
{
	const char *script;
	const char *param;
	char *params;
	char delim;
	int trigger;
	trigger = Get_Int_Parameter("Trigger");
	if (trigger != 4)
	{
		return;
	}
	int pltype;
	int destroy;
	pltype = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,pltype))
	{
		return;
	}
	script = Get_Parameter("Script_Name");
	param = Get_Parameter("Script_Params");
	params = newstr(param);
	delim = Get_Parameter("Delim")[0];
	unsigned int x = strlen(params);
	for (unsigned int i=0;i<x;i++)
	{
		if (params[i] == delim)
		{
			params[i] = ',';
		}
	}
	Commands->Attach_Script(poker,script,params);
	delete[] params;
	destroy = Get_Int_Parameter("Destroy");
	if (destroy == 1)
	{
		Commands->Destroy_Object(obj);
	}
}

void JFW_Remove_Script_Preset_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Remove_Script_Preset(Get_Parameter("Script"),Get_Parameter("Preset"),Get_Int_Parameter("Player_Type"));
	}
}

void JFW_Attach_Script_Preset_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		const char *script;
		const char *paramx;
		char *params;
		char delim;
		script = Get_Parameter("Script");
		paramx = Get_Parameter("Params");
		params = newstr(paramx);
		delim = Get_Parameter("Delim")[0];
		unsigned int x = strlen(params);
		for (unsigned int i=0;i<x;i++)
		{
			if (params[i] == delim)
			{
				params[i] = ',';
			}
		}
		Attach_Script_Preset(script,params,Get_Parameter("Preset"),Get_Int_Parameter("Player_Type"));
		delete[] params;
	}
}

void JFW_Remove_Script_Type_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Remove_Script_Type(Get_Parameter("Script"),Get_Int_Parameter("Type"),Get_Int_Parameter("Player_Type"));
	}
}

void JFW_Attach_Script_Type_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		const char *script;
		const char *paramx;
		char *params;
		char delim;
		script = Get_Parameter("Script");
		paramx = Get_Parameter("Params");
		params = newstr(paramx);
		delim = Get_Parameter("Delim")[0];
		unsigned int x = strlen(params);
		for (unsigned int i=0;i<x;i++)
		{
			if (params[i] == delim)
			{
				params[i] = ',';
			}
		}
		Attach_Script_Type(script,params,Get_Int_Parameter("Type"),Get_Int_Parameter("Player_Type"));
		delete[] params;
	}
}

void JFW_Remove_Script_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Remove_Script(obj,Get_Parameter("Script"));
	}
}

void JFW_Remove_Script_Death::Killed(GameObject *obj,GameObject *killer)
{
	Remove_Script(obj,Get_Parameter("Script"));
	Destroy_Script();
}

void JFW_Attach_Script_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		const char *script;
		const char *paramx;
		char *params;
		char delim;
		script = Get_Parameter("Script");
		paramx = Get_Parameter("Params");
		params = newstr(paramx);
		delim = Get_Parameter("Delim")[0];
		unsigned int x = strlen(params);
		for (unsigned int i=0;i<x;i++)
		{
			if (params[i] == delim)
			{
				params[i] = ',';
			}
		}
		Commands->Attach_Script(obj,script,params);
		delete[] params;
	}
}

void JFW_Attach_Script_Custom_Until_Custom::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == Get_Int_Parameter("AttachMessage"))
	{
		const char *script = Get_Parameter("Script");
		const char *paramx = Get_Parameter("Params");
		char *params = newstr(paramx);
		char delim = Get_Parameter("Delim")[0];
		size_t x = strlen(params);
		for (size_t i = 0; i < x; i++)
		{
			if (params[i] == delim)
			{
				params[i] = ',';
			}
		}
		Commands->Attach_Script(obj, script, params);
		delete[] params;
	}
	else if (type == Get_Int_Parameter("RemoveMessage"))
	{
		Remove_Script(obj, Get_Parameter("Script"));
	}
}

void JFW_Remove_All_Scripts_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Remove_All_Scripts(obj);
	}
}

void JFW_Attach_Script_Preset_Once_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		const char *script;
		const char *paramx;
		char *params;
		char delim;
		script = Get_Parameter("Script");
		paramx = Get_Parameter("Params");
		params = newstr(paramx);
		delim = Get_Parameter("Delim")[0];
		unsigned int x = strlen(params);
		for (unsigned int i=0;i<x;i++)
		{
			if (params[i] == delim)
			{
				params[i] = ',';
			}
		}
		Attach_Script_Preset_Once(script,params,Get_Parameter("Preset"),Get_Int_Parameter("Player_Type"));
		delete[] params;
	}
}

void JFW_Attach_Script_Type_Once_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		const char *script;
		const char *paramx;
		char *params;
		char delim;
		script = Get_Parameter("Script");
		paramx = Get_Parameter("Params");
		params = newstr(paramx);
		delim = Get_Parameter("Delim")[0];
		unsigned int x = strlen(params);
		for (unsigned int i=0;i<x;i++)
		{
			if (params[i] == delim)
			{
				params[i] = ',';
			}
		}
		Attach_Script_Type_Once(script,params,Get_Int_Parameter("Type"),Get_Int_Parameter("Player_Type"));
		delete[] params;
	}
}

void JFW_Attach_Script_Once_Custom::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		const char *script;
		const char *paramx;
		char *params;
		char delim;
		script = Get_Parameter("Script");
		paramx = Get_Parameter("Params");
		params = newstr(paramx);
		delim = Get_Parameter("Delim")[0];
		unsigned int x = strlen(params);
		for (unsigned int i=0;i<x;i++)
		{
			if (params[i] == delim)
			{
				params[i] = ',';
			}
		}
		Attach_Script_Once(obj,script,params);
		delete[] params;
	}
}

void JFW_Attach_Script_Preset_Startup::Created(GameObject *obj)
{
	const char *script;
	const char *paramx;
	char *params;
	char delim;
	script = Get_Parameter("Script");
	paramx = Get_Parameter("Params");
	params = newstr(paramx);
	delim = Get_Parameter("Delim")[0];
	unsigned int x = strlen(params);
	for (unsigned int i=0;i<x;i++)
	{
		if (params[i] == delim)
		{
			params[i] = ',';
		}
	}
	Attach_Script_Preset(script,params,Get_Parameter("Preset"),Get_Int_Parameter("Player_Type"));
	delete[] params;
	Destroy_Script();
}

void JFW_Attach_Script_Building_Startup::Created(GameObject *obj)
{
	const char *script;
	const char *paramx;
	char *params;
	char delim;
	script = Get_Parameter("Script");
	paramx = Get_Parameter("Params");
	params = newstr(paramx);
	delim = Get_Parameter("Delim")[0];
	unsigned int x = strlen(params);
	for (unsigned int i=0;i<x;i++)
	{
		if (params[i] == delim)
		{
			params[i] = ',';
		}
	}
	Attach_Script_Building(script,params,Get_Int_Parameter("Player_Type"));
	delete[] params;
	Destroy_Script();
}

void JFW_Attach_Script_Type_Startup::Created(GameObject *obj)
{
	const char *script;
	const char *paramx;
	char *params;
	char delim;
	script = Get_Parameter("Script");
	paramx = Get_Parameter("Params");
	params = newstr(paramx);
	delim = Get_Parameter("Delim")[0];
	unsigned int x = strlen(params);
	for (unsigned int i=0;i<x;i++)
	{
		if (params[i] == delim)
		{
			params[i] = ',';
		}
	}
	Attach_Script_Type(script,params,Get_Int_Parameter("Type"),Get_Int_Parameter("Player_Type"));
	delete[] params;
	Destroy_Script();
}

void JFW_Attach_Script_Collector::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		const char *script;
		const char *paramx;
		char *params;
		char delim;
		script = Get_Parameter("Script");
		paramx = Get_Parameter("Params");
		params = newstr(paramx);
		delim = Get_Parameter("Delim")[0];
		unsigned int x = strlen(params);
		for (unsigned int i=0;i<x;i++)
		{
			if (params[i] == delim)
			{
				params[i] = ',';
			}
		}
		Commands->Attach_Script(sender,script,params);
		delete[] params;
	}
}

void JFW_Attach_Script_Sender::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		const char *script;
		const char *paramx;
		char *params;
		char delim;
		script = Get_Parameter("Script");
		paramx = Get_Parameter("Params");
		params = newstr(paramx);
		delim = Get_Parameter("Delim")[0];
		unsigned int x = strlen(params);
		for (unsigned int i=0;i<x;i++)
		{
			if (params[i] == delim)
			{
				params[i] = ',';
			}
		}
		Commands->Attach_Script(sender,script,params);
		delete[] params;
	}
}

ScriptRegistrant<JFW_Attatch_Script> JFW_Attatch_Script_Registrant("JFW_Attatch_Script","Trigger:int,Player_Type:int,Script_Name:string,Script_Params:string,Delim:string,Destroy:int,Custom:int");
ScriptRegistrant<JFW_Remove_Script_Preset_Custom> JFW_Remove_Script_Preset_Custom_Registrant("JFW_Remove_Script_Preset_Custom","Script:string,Message:int,Preset:string,Player_Type:int");
ScriptRegistrant<JFW_Attach_Script_Preset_Custom> JFW_Attach_Script_Preset_Custom_Registrant("JFW_Attach_Script_Preset_Custom","Script:string,Params:string,Delim:string,Message:int,Preset:string,Player_Type:int");
ScriptRegistrant<JFW_Remove_Script_Type_Custom> JFW_Remove_Script_Type_Custom_Registrant("JFW_Remove_Script_Type_Custom","Script:string,Message:int,Type:int,Player_Type:int");
ScriptRegistrant<JFW_Attach_Script_Type_Custom> JFW_Attach_Script_Type_Custom_Registrant("JFW_Attach_Script_Type_Custom","Script:string,Params:string,Delim:string,Message:int,Type:int,Player_Type:int");
ScriptRegistrant<JFW_Remove_Script_Custom> JFW_Remove_Script_Custom_Registrant("JFW_Remove_Script_Custom","Script:string,Message:int");
ScriptRegistrant<JFW_Remove_Script_Death> JFW_Remove_Script_Death_Registrant("JFW_Remove_Script_Death","Script:string");
ScriptRegistrant<JFW_Attach_Script_Custom> JFW_Attach_Script_Custom_Registrant("JFW_Attach_Script_Custom","Script:string,Params:string,Delim:string,Message:int");
ScriptRegistrant<JFW_Attach_Script_Custom_Until_Custom> JFW_Attach_Script_Custom_Until_Custom_Registrant("JFW_Attach_Script_Custom_Until_Custom","Script:string,Params:string,Delim:string,AttachMessage:int,RemoveMessage:int");
ScriptRegistrant<JFW_Remove_All_Scripts_Custom> JFW_Remove_All_Scripts_Custom_Registrant("JFW_Remove_All_Scripts_Custom","Message:int");
ScriptRegistrant<JFW_Attach_Script_Preset_Once_Custom> JFW_Attach_Script_Preset_Once_Custom_Registrant("JFW_Attach_Script_Preset_Once_Custom","Script:string,Params:string,Delim:string,Message:int,Preset:string,Player_Type:int");
ScriptRegistrant<JFW_Attach_Script_Type_Once_Custom> JFW_Attach_Script_Type_Once_Custom_Registrant("JFW_Attach_Script_Type_Once_Custom","Script:string,Params:string,Delim:string,Message:int,Type:int,Player_Type:int");
ScriptRegistrant<JFW_Attach_Script_Once_Custom> JFW_Attach_Script_Once_Custom_Registrant("JFW_Attach_Script_Once_Custom","Script:string,Params:string,Delim:string,Message:int");
ScriptRegistrant<JFW_Attach_Script_Preset_Startup> JFW_Attach_Script_Preset_Startup_Registrant("JFW_Attach_Script_Preset_Startup","Script:string,Params:string,Delim:string,Preset:string,Player_Type:int");
ScriptRegistrant<JFW_Attach_Script_Type_Startup> JFW_Attach_Script_Type_Startup_Registrant("JFW_Attach_Script_Type_Startup","Script:string,Params:string,Delim:string,Type:int,Player_Type:int");
ScriptRegistrant<JFW_Attach_Script_Collector> JFW_Attach_Script_Collector_Registrant("JFW_Attach_Script_Collector","Script:string,Params:string,Delim:string");
ScriptRegistrant<JFW_Attach_Script_Sender> JFW_Attach_Script_Sender_Registrant("JFW_Attach_Script_Sender","Script:string,Params:string,Delim:string,Message:int");
ScriptRegistrant<JFW_Attach_Script_Building_Startup> JFW_Attach_Script_Building_Startup_Registrant("JFW_Attach_Script_Building_Startup","Script:string,Params:string,Delim:string,Player_Type:int");
