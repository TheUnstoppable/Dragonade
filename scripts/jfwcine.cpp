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

#include "scripts.h"
#include "jfwcine.h"
#include "engine.h"
#include "PhysicalGameObj.h"

void JFW_Death_Play_Cinematic::Killed(GameObject *obj,GameObject *killer)
{
	const char *scriptname;
	Vector3 position;
	float facing;
	GameObject *object;
	scriptname = Get_Parameter("Script_Name");
	position = Get_Vector3_Parameter("location");
	facing = Get_Float_Parameter("Facing");
	object = Commands->Create_Object("Invisible_Object",position);
	Commands->Set_Facing(object,facing);
	Commands->Attach_Script(object,"JFW_Cinematic",scriptname);
}

void JFW_Zone_Play_Cinematic::Entered(GameObject *obj,GameObject *enterer)
{
	const char *scriptname;
	int x;
	Vector3 position;
	float facing;
	GameObject *object;
	x = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	scriptname = Get_Parameter("Script_Name");
	position = Get_Vector3_Parameter("location");
	facing = Get_Float_Parameter("Facing");
	object = Commands->Create_Object("Invisible_Object",position);
	Commands->Set_Facing(object,facing);
	Commands->Attach_Script(object,"JFW_Cinematic",scriptname);
}

void JFW_Timer_Play_Cinematic::Created(GameObject *obj)
{
	float timertime;
	int timernumber;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_Timer_Play_Cinematic::Timer_Expired(GameObject *obj,int number)
{
	int timernumber;
	int repeat;
	float timertime;
	const char *scriptname;
	Vector3 position;
	GameObject *object;
	float facing;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	repeat = Get_Int_Parameter("Repeat");
	scriptname = Get_Parameter("Script_Name");
	position = Get_Vector3_Parameter("location");
	facing = Get_Float_Parameter("Facing");
	if (number == timernumber)
	{
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"JFW_Cinematic",scriptname);
		if (repeat == 1)
		{	
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}

void JFW_Custom_Play_Cinematic::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	const char *scriptname;
	Vector3 position;
	GameObject *object;
	float facing;
	msg = Get_Int_Parameter("Message");
	scriptname = Get_Parameter("Script_Name");
	position = Get_Vector3_Parameter("Location");
	facing = Get_Float_Parameter("Facing");
	if (type == msg)
	{
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"JFW_Cinematic",scriptname);
	}
}

void JFW_Customs_Play_Cinematic::Created(GameObject *obj)
{
	if (!Get_Int_Parameter("Message1"))
	{
		custom1 = true;
	}
	else
	{
		custom1 = false;
	}
	if (!Get_Int_Parameter("Message2"))
	{
		custom2 = true;
	}
	else
	{
		custom2 = false;
	}
	if (!Get_Int_Parameter("Message3"))
	{
		custom3 = true;
	}
	else
	{
		custom3 = false;
	}
	if (!Get_Int_Parameter("Message4"))
	{
		custom4 = true;
	}
	else
	{
		custom4 = false;
	}
	if (!Get_Int_Parameter("Message5"))
	{
		custom5 = true;
	}
	else
	{
		custom5 = false;
	}
	if (!Get_Int_Parameter("Message6"))
	{
		custom6 = true;
	}
	else
	{
		custom6 = false;
	}
	if (!Get_Int_Parameter("Message7"))
	{
		custom7 = true;
	}
	else
	{
		custom7 = false;
	}
	if (!Get_Int_Parameter("Message8"))
	{
		custom8 = true;
	}
	else
	{
		custom8 = false;
	}
	trigger = false;
}

void JFW_Customs_Play_Cinematic::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	const char *scriptname;
	Vector3 position;
	GameObject *object;
	float facing;
	msg = Get_Int_Parameter("Message1");
	if (type == msg)
	{
		custom1 = true;
	}
	msg = Get_Int_Parameter("Message2");
	if (type == msg)
	{
		custom2 = true;
	}
	msg = Get_Int_Parameter("Message3");
	if (type == msg)
	{
		custom3 = true;
	}
	msg = Get_Int_Parameter("Message4");
	if (type == msg)
	{
		custom4 = true;
	}
	msg = Get_Int_Parameter("Message5");
	if (type == msg)
	{
		custom5 = true;
	}
	msg = Get_Int_Parameter("Message6");
	if (type == msg)
	{
		custom6 = true;
	}
	msg = Get_Int_Parameter("Message7");
	if (type == msg)
	{
		custom7 = true;
	}
	msg = Get_Int_Parameter("Message8");
	if (type == msg)
	{
		custom8 = true;
	}
	if (!trigger)
	{
		if ((custom1) && (custom2) && (custom3) && (custom4) && (custom5) && (custom6) && (custom7) && (custom8))
		{
			trigger = true;
			scriptname = Get_Parameter("Script_Name");
			position = Get_Vector3_Parameter("Location");
			facing = Get_Float_Parameter("Facing");
			object = Commands->Create_Object("Invisible_Object",position);
			Commands->Set_Facing(object,facing);
			Commands->Attach_Script(object,"JFW_Cinematic",scriptname);
		}
	}
}

void JFW_Customs_Play_Cinematic::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&custom1,1,1);
	Auto_Save_Variable(&custom2,1,2);
	Auto_Save_Variable(&custom3,1,3);
	Auto_Save_Variable(&custom4,1,4);
	Auto_Save_Variable(&custom5,1,5);
	Auto_Save_Variable(&custom6,1,6);
	Auto_Save_Variable(&custom7,1,7);
	Auto_Save_Variable(&custom8,1,8);
	Auto_Save_Variable(&trigger,1,9);
}

void JFW_Looping_Cinematic::Created(GameObject *obj)
{
	const char *scriptname;
	Vector3 position;
	GameObject *object;
	float facing;
	int timernumber;
	float timertime;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
	scriptname = Get_Parameter("Script_Name");
	position = Get_Vector3_Parameter("Location");
	facing = Get_Float_Parameter("Facing");
	object = Commands->Create_Object("Invisible_Object",position);
	Commands->Set_Facing(object,facing);
	Commands->Attach_Script(object,"JFW_Cinematic",scriptname);
}

void JFW_Looping_Cinematic::Timer_Expired(GameObject *obj,int number)
{
	const char *scriptname;
	Vector3 position;
	GameObject *object;
	float facing;
	int timernumber;
	float timertime;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	if (number == timernumber)
	{
		Commands->Start_Timer(obj,this,timertime,timernumber);
		scriptname = Get_Parameter("Script_Name");
		position = Get_Vector3_Parameter("Location");
		facing = Get_Float_Parameter("Facing");
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"JFW_Cinematic",scriptname);
	}
}

void JFW_Cinematic_Attack::Created(GameObject *obj)
{
	ActionParamsStruct params;
	int priority = Get_Int_Parameter("Priority");
	int targetid = Get_Int_Parameter("ID");
	float range = Get_Float_Parameter("Range");
	float deviation = Get_Float_Parameter("Deviation");
	bool primary = Get_Bool_Parameter("Primary");
	params.Set_Basic(this,(float)priority,40016);
	params.Set_Attack(Commands->Find_Object(targetid),range,deviation,primary);
	params.AttackCheckBlocked = false;
	Commands->Action_Attack(obj,params);
}

void JFW_Cinematic_Attack::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	Commands->Action_Reset(obj,100);
}

void JFW_Random_Timer_Play_Cinematic::Created(GameObject *obj)
{
	float timertimemin;
	float timertimemax;
	float timertime;
	int timernumber;
	timertimemin = Get_Float_Parameter("TimeMin");
	timertimemax = Get_Float_Parameter("TimeMax");
	timertime = Commands->Get_Random(timertimemin,timertimemax);
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_Random_Timer_Play_Cinematic::Timer_Expired(GameObject *obj,int number)
{
	int timernumber;
	int repeat;
	float timertimemin;
	float timertimemax;
	float timertime;
	const char *scriptname;
	Vector3 position;
	GameObject *object;
	float facing;
	timertimemin = Get_Float_Parameter("TimeMin");
	timertimemax = Get_Float_Parameter("TimeMax");
	timertime = Commands->Get_Random(timertimemin,timertimemax);
	timernumber = Get_Int_Parameter("TimerNum");
	repeat = Get_Int_Parameter("Repeat");
	scriptname = Get_Parameter("Script_Name");
	position = Get_Vector3_Parameter("location");
	facing = Get_Float_Parameter("Facing");
	if (number == timernumber)
	{
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"JFW_Cinematic",scriptname);
		if (repeat == 1)
		{	
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}

void JFW_Timer_Play_Random_Cinematic::Created(GameObject *obj)
{
	float timertime;
	int timernumber;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_Timer_Play_Random_Cinematic::Timer_Expired(GameObject *obj,int number)
{
	int timernumber;
	int repeat;
	float timertime;
	const char *scriptname;
	char sn[20];
	int random = Commands->Get_Random_Int(1,5);
	Vector3 position;
	GameObject *object;
	float facing;
	sprintf(sn,"Script_Name%d",random);
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	repeat = Get_Int_Parameter("Repeat");
	scriptname = Get_Parameter(sn);
	position = Get_Vector3_Parameter("location");
	facing = Get_Float_Parameter("Facing");
	if (number == timernumber)
	{
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"JFW_Cinematic",scriptname);
		if (repeat == 1)
		{	
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}

void JFW_Random_Timer_Play_Random_Cinematic::Created(GameObject *obj)
{
	float timertimemin;
	float timertimemax;
	float timertime;
	int timernumber;
	timertimemin = Get_Float_Parameter("TimeMin");
	timertimemax = Get_Float_Parameter("TimeMax");
	timertime = Commands->Get_Random(timertimemin,timertimemax);
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void JFW_Random_Timer_Play_Random_Cinematic::Timer_Expired(GameObject *obj,int number)
{
	int timernumber;
	int repeat;
	float timertimemin;
	float timertimemax;
	float timertime;
	const char *scriptname;
	char sn[20];
	int random = Commands->Get_Random_Int(1,5);
	Vector3 position;
	GameObject *object;
	float facing;
	sprintf(sn,"Script_Name%d",random);
	timertimemin = Get_Float_Parameter("TimeMin");
	timertimemax = Get_Float_Parameter("TimeMax");
	timertime = Commands->Get_Random(timertimemin,timertimemax);
	timernumber = Get_Int_Parameter("TimerNum");
	repeat = Get_Int_Parameter("Repeat");
	scriptname = Get_Parameter(sn);
	position = Get_Vector3_Parameter("location");
	facing = Get_Float_Parameter("Facing");
	if (number == timernumber)
	{
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"JFW_Cinematic",scriptname);
		if (repeat == 1)
		{	
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}

void JFW_Cinematic_Attack_Position::Created(GameObject *obj)
{
	ActionParamsStruct params;
	int priority = Get_Int_Parameter("Priority");
	Vector3 position = Get_Vector3_Parameter("Position");
	float range = Get_Float_Parameter("Range");
	float deviation = Get_Float_Parameter("Deviation");
	bool primary = Get_Bool_Parameter("Primary");
	params.Set_Basic(this,(float)priority,40016);
	params.Set_Attack(position,range,deviation,primary);
	Commands->Action_Attack(obj,params);
}

void JFW_Cinematic_Attack_Position::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	Commands->Action_Reset(obj,100);
}

void JFW_Cinematic_Attack_Command::Created(GameObject *obj)
{
	float facing = Commands->Get_Facing(Owner());
	Vector3 position = Commands->Get_Position(Owner());
	position.X += cos(DEG_TO_RADF(facing)) * 10.0f;
	position.Y += sin(DEG_TO_RADF(facing)) * 10.0f;
	position.Z += 2.0;
	ActionParamsStruct params;
	params.Set_Basic(this,99,1);
	params.Set_Attack(position,100,0,true);
	params.AttackForceFire = true;
	Commands->Action_Attack(obj,params);
	Commands->Send_Custom_Event(obj,obj,1,1,Get_Float_Parameter("AttackDuration"));
}

void JFW_Cinematic_Attack_Command::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	Commands->Action_Reset(obj,100);
}

void JFW_Cinematic::Created(GameObject *obj)
{
	Lines = 0;
	memset(Slots,0,sizeof(Slots));
	Load_Control_File(Get_Parameter("ControlFilename"));

    StartPaused = Get_Parameter_Count() >= 2 ? Get_Bool_Parameter(1) : false;

    if (!StartPaused)
    {
        Start_Cinematic(obj);
    }
}

void JFW_Cinematic::Start_Cinematic(GameObject* obj)
{
    Commands->Enable_Hibernation(obj,0);
    SyncTime = Commands->Get_Sync_Time();
    f1 = 0;
    PrimaryKilled = false;
    CameraControl = false;
    Parse_Commands(obj);
}

void JFW_Cinematic::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if ((type == 9023) && (!PrimaryKilled))
	{
		PrimaryKilled = true;
		Parse_Commands(obj);
	}
    if (type == 99000 && StartPaused)
    {
        StartPaused = false;
        Start_Cinematic(obj);
    }
	if ((type >= 10000) && ((type - 10000) <= 39))
	{
		if (!Slots[type - 10000])
		{
			Slots[type - 10000] = param;
		}
	}
}

void JFW_Cinematic::Timer_Expired(GameObject *obj,int number)
{
	Parse_Commands(obj);
}

void JFW_Cinematic::Save(ScriptSaver &saver)
{
	Commands->Begin_Chunk(saver,1000);
	unsigned int time = Commands->Get_Sync_Time() - SyncTime;
	Commands->Save_Data(saver,1,4,&time);
	Commands->Save_Data(saver,2,4,&f1);
	Commands->Save_Data(saver,7,1,&PrimaryKilled);
	Commands->End_Chunk(saver);
	Commands->Begin_Chunk(saver,1001);
	for (unsigned int i = 0;i <= 39;i++)
	{
		if (Slots[i])
		{
			Commands->Save_Data(saver,i,4,&Slots[i]);
		}
	}
	Commands->End_Chunk(saver);
	Commands->Begin_Chunk(saver,1002);
	ControlLine *l = Lines;
	while (l)
	{
		Commands->Save_Data(saver,4,4,&l->Frame);
		int x = strlen(l->Line)+1;
		Commands->Save_Data(saver,5,4,&x);
		Commands->Save_Data(saver,6,x,(void *)l->Line);
		l = l->Next;
	}
	Commands->End_Chunk(saver);
}

void JFW_Cinematic::Load(ScriptLoader &loader)
{
	CurrentLine = 0;
	Lines = 0;
	memset(Slots,0,sizeof(Slots));
	unsigned int chunktype;
	int mctype;
	float f = 0;
	unsigned int size = 0;
	char line[199];
	unsigned int time;
	while (Commands->Open_Chunk(loader,&chunktype))
	{
		switch (chunktype)
		{
		case 1001:
			while (Commands->Load_Begin(loader,&mctype))
			{
				if ((mctype >= 0) && (mctype <= 39))
				{
					Commands->Load_Data(loader,4,&Slots[mctype]);
				}
				Commands->Load_End(loader);
			}
			break;
		case 1002:
			while (Commands->Load_Begin(loader,&mctype))
			{
				switch (mctype)
				{
				case 4:
					Commands->Load_Data(loader,4,&f);
					break;
				case 5:
					Commands->Load_Data(loader,4,&size);
					break;
				case 6:
					if (size <= 199)
					{
						Commands->Load_Data(loader,size,line);
						Add_Control_Line(f,line);
					}
					break;
				}
				Commands->Load_End(loader);
			}
			break;
		case 1000:
			while (Commands->Load_Begin(loader,&mctype))
			{
				switch (mctype)
				{
				case 2:
					Commands->Load_Data(loader,4,&f1);
					break;
				case 7:
					Commands->Load_Data(loader,1,&PrimaryKilled);
					break;
				case 1:
					Commands->Load_Data(loader,4,&time);
					SyncTime = Commands->Get_Sync_Time() - time;
					break;
				}
				Commands->Load_End(loader);
			}
			break;
		}
		Commands->Close_Chunk(loader);
	}
}

void JFW_Cinematic::Load_Control_File(const char *file)
{
	int handle = Commands->Text_File_Open(file);
	char line[200];
	char *line2 = line;
	if (handle)
	{
		while (Commands->Text_File_Get_String(handle,line,199))
		{
			while (*line2)
			{
				if (*line2 == 9)
				{
					*line2 = ' ';
				}
				line2++;
			}
			line2 = line;
			while (*line2 && (*line2 <= ' '))
			{
				line2++;
			}
			int x = strlen(line2);
			while ((x) && (line2[x-1] <= ' '))
			{
				x--;
				line2[x] = 0;
			}
			if (*line2 && (*line2 != ';'))
			{
				float f = (float)atof(line2);
				if (f < 0)
				{
					f = -f /30.0f;
				}
				while (*line2 > ' ')
				{
					line2++;
				}
				while (*line2 <= ' ')
				{
					line2++;
				}
				if (*line2)
				{
					Add_Control_Line(f,line2);
				}
			}
		}
		Commands->Text_File_Close(handle);
	}
}

void JFW_Cinematic::Parse_Commands(GameObject *obj)
{
	unsigned int sync = Commands->Get_Sync_Time() - SyncTime;
	SyncTime += sync;
	float f = ((float)sync) / 1000.0f;
	f1 += f;
	CallbackID = Commands->Get_ID(obj);
	if (PrimaryKilled)
	{
		while ((Lines) && (Lines->Frame <= 999000))
		{
			Remove_Head_Control_Line();
		}
		while (Lines)
		{
			Parse_Command(Lines->Line);
			Remove_Head_Control_Line();
		}
	}
	while (Lines && Lines->Frame <= f1)
	{
		f2 = (f1 - Lines->Frame) * 30;
		Parse_Command(Lines->Line);
		Remove_Head_Control_Line();
	}
	if ((Lines) && (Lines->Frame < 999000))
	{
		Commands->Start_Timer(obj,this,Lines->Frame-f1,0);
	}
	else
	{
		Commands->Destroy_Object(obj);
	}
	if (CameraControl)
	{
		Commands->Enable_Cinematic_Freeze(obj,false);
	}
}

void JFW_Cinematic::Add_Control_Line(float frame,const char *line)
{
	ControlLine *l = new ControlLine;
	l->Frame = frame;
	l->Line = newstr(line);
	l->Next = 0;
	if ((!Lines) || (frame < Lines->Frame))
	{
		l->Next = Lines;
		Lines = l;
	}
	else
	{
		ControlLine *l2 = Lines;
		while ((l2->Next) && (frame >= l2->Next->Frame))
		{
			l2 = l2->Next;
		}
		l->Next = l2->Next;
		l2->Next = l;
	}
}

char *JFW_Cinematic::Get_First_Parameter(char *command)
{
	return Get_Command_Parameter(command);
}

char *JFW_Cinematic::Get_Next_Parameter()
{
	return Get_Command_Parameter();
}

char *JFW_Cinematic::Get_Command_Parameter(char *command)
{
	if (command)
	{
		CurrentLine = command;
	}
	if ((CurrentLine) && (*CurrentLine))
	{
		char *c = CurrentLine;
		while ((*c) && (*c <= ' '))
		{
			c++;
		}
		if (*c == '"')
		{
			c++;
			CurrentLine = c;
			while ((*CurrentLine) && (*CurrentLine != '"'))
			{
				CurrentLine++;
			}
		}
		while ((*CurrentLine) && (*CurrentLine != ','))
		{
			CurrentLine++;
		}
		if (*CurrentLine)
		{
			*CurrentLine = 0;
			CurrentLine++;
		}
		int x = strlen(c);
		while ((c[x-1] <= ' ') && x)
		{
			x--;
			c[x] = 0;
		}
		if (x && (c[x-1] == '"'))
		{
			c[x-1] = 0;
		}
		return c;
	}
	return "";
}

void JFW_Cinematic::Remove_Head_Control_Line()
{
	if (Lines)
	{
		ControlLine *l = Lines;
		Lines = l->Next;
		delete[] l->Line;
		delete l;
	}
}

bool JFW_Cinematic::Title_Match(char **command,char *match)
{
	if (!_strnicmp(*command,match,strlen(match)))
	{
		*command += strlen(match);
		while (**command && (**command != ','))
		{
			(*command)++;
		}
		if (**command == ',')
		{
			(*command)++;
		}
		return true;
	}
	else
	{
		return false;
	}
}

void JFW_Cinematic::Parse_Command(char *command)
{
	if (Title_Match(&command,"Create_Object"))
	{
		Command_Create_Object(command);
	}
	else if (Title_Match(&command,"Create_Real_Object"))
	{
		Command_Create_Real_Object(command);
	}
	else if (Title_Match(&command,"Add_Object"))
	{
		Command_Add_Object(command);
	}
	else if (Title_Match(&command,"Create_Explosion"))
	{
		Command_Create_Explosion(command);
	}
	else if (Title_Match(&command,"Destroy_Object"))
	{
		Command_Destroy_Object(command);
	}
	else if (Title_Match(&command,"Play_Complete_Animation"))
	{
		Command_Play_Complete_Animation(command);
	}
	else if (Title_Match(&command,"Stop_Animation"))
	{
		Command_Stop_Animation(command);
	}
	else if (Title_Match(&command,"Play_Animation"))
	{
		Command_Play_Animation(command);
	}
	else if (Title_Match(&command,"Play_Audio"))
	{
		Command_Play_Audio(command);
	}
	else if (Title_Match(&command,"Control_Camera"))
	{
		Command_Control_Camera(command);
	}
	else if (Title_Match(&command,"Send_Custom"))
	{
		Command_Send_Custom(command);
	}
	else if (Title_Match(&command,"Attach_To_Bone"))
	{
		Command_Attach_To_Bone(command);
	}
	else if (Title_Match(&command,"Attach_Script"))
	{
		Command_Attach_Script(command);
	}
	else if (Title_Match(&command,"Set_Primary"))
	{
		Command_Set_Primary(command);
	}
	else if (Title_Match(&command,"Move_Slot"))
	{
		Command_Move_Slot(command);
	}
	else if (Title_Match(&command,"Sniper_Control"))
	{
		Command_Sniper_Control(command);
	}
	else if (Title_Match(&command,"Shake_Camera"))
	{
		Command_Shake_Camera(command);
	}
	else if (Title_Match(&command,"Enable_Shadow"))
	{
		Command_Enable_Shadow(command);
	}
	else if (Title_Match(&command,"Enable_Letterbox"))
	{
		Command_Enable_Letterbox(command);
	}
	else if (Title_Match(&command,"Set_Screen_Fade_Color"))
	{
		Command_Set_Screen_Fade_Color(command);
	}
	else if (Title_Match(&command,"Set_Screen_Fade_Opacity"))
	{
		Command_Set_Screen_Fade_Opacity(command);
	}
    else if (Title_Match(&command,"Show_Message"))
    {
        Command_Show_Message(command);
    }
}

void JFW_Cinematic::Command_Create_Object(char *command)
{
	int slot = atoi(Get_First_Parameter(command));
	const char *model = Get_Next_Parameter();
	if ((slot < 0) || (slot >= 40))
	{
		slot = -1;
	}
	Vector3 v = Commands->Get_Position(Owner());
	GameObject *obj = Commands->Create_Object("Generic_Cinematic",v);
	Commands->Enable_Hibernation(obj,false);
	if (obj)
	{
		Commands->Add_To_Dirty_Cull_List(obj);
		Commands->Set_Model(obj,model);
		Commands->Set_Facing(obj,Commands->Get_Facing(Owner()));
		if (slot != -1)
		{
			Slots[slot] = Commands->Get_ID(obj);
		}
		if (CameraControl)
		{
			Commands->Enable_Cinematic_Freeze(obj,false);
			Commands->Enable_Hibernation(obj,false);
		}
	}
}

void JFW_Cinematic::Command_Create_Real_Object(char *command)
{
	int slot = atoi(Get_First_Parameter(command));
	const char *preset = Get_Next_Parameter();
	const char *boneslotc = Get_Next_Parameter();
	const char *bone = Get_Next_Parameter();
	GameObject *obj;
	if ((slot < 0) || (slot >= 40))
	{
		slot = -1;
	}
	if ((boneslotc) && (boneslotc[0]))
	{
		unsigned int boneslot = atoi(boneslotc);
		obj = Commands->Create_Object_At_Bone(Commands->Find_Object(Slots[boneslot]),preset,bone);
	}
	else
	{
		Vector3 v = Commands->Get_Position(Owner());
		obj = Commands->Create_Object(preset,v);
		Commands->Set_Facing(obj,Commands->Get_Facing(Owner()));
	}
	if (obj)
	{
		Commands->Enable_Engine(obj,true);
		Commands->Add_To_Dirty_Cull_List(obj);
		if (slot != -1)
		{
			Slots[slot] = Commands->Get_ID(obj);
		}
		if (CameraControl)
		{
			Commands->Enable_Cinematic_Freeze(obj,false);
			Commands->Enable_Hibernation(obj,false);
		}
	}
}

void JFW_Cinematic::Command_Add_Object(char *command)
{
	int slot = atoi(Get_First_Parameter(command));
	char *preset = Get_Next_Parameter();
	if ((slot < 0) || (slot >= 40))
	{
		slot = -1;
	}
	GameObject *obj = Find_Nearest_Preset(Commands->Get_Position(Owner()),preset);
	if (obj)
	{
		if (slot != -1)
		{
			Slots[slot] = Commands->Get_ID(obj);
		}
	}
}

void JFW_Cinematic::Command_Create_Explosion(char *command)
{
	char *explosion = Get_First_Parameter(command);
	int slot = atoi(Get_Next_Parameter());
	const char *bone = Get_Next_Parameter();
	Commands->Create_Explosion_At_Bone(explosion,Commands->Find_Object(Slots[slot]),bone,0);
}

void JFW_Cinematic::Command_Destroy_Object(char *command)
{
	int slot = atoi(Get_First_Parameter(command));
	if ((slot < 0) || (slot >= 40))
	{
		slot = -1;
	}
	if ((slot != -1) && (Slots[slot]))
	{
		GameObject *obj = Commands->Find_Object(Slots[slot]);
		if (obj)
		{
			Commands->Destroy_Object(obj);
		}
	}
}

void JFW_Cinematic::Command_Play_Animation(char *command)
{
	int slot = atoi(Get_First_Parameter(command));
	char *animation = Get_Next_Parameter();
	bool loop = atoi(Get_Next_Parameter()) != 0;
	char *subobject = Get_Next_Parameter();
	bool blended = atoi(Get_Next_Parameter()) == 1;
	if ((slot < 0) || (slot >= 40))
	{
		slot = -1;
	}
	if (slot != -1)
	{
		GameObject *obj = Commands->Find_Object(Slots[slot]);
		if (obj)
		{
			Update_Network_Object(obj);
			Set_Subobject_Animation(obj,animation,loop,subobject,f2,-1,blended);
			if (CameraControl)
			{
				Commands->Innate_Disable(obj);
			}
		}
	}
}

void JFW_Cinematic::Command_Play_Complete_Animation(char *command)
{
	int slot = atoi(Get_First_Parameter(command));
	char *animation = Get_Next_Parameter();
	if ((slot < 0) || (slot >= 40))
	{
		slot = -1;
	}
	if (slot != -1)
	{
		GameObject *obj = Commands->Find_Object(Slots[slot]);
		if (obj && obj->As_PhysicalGameObj())
		{
			obj->As_PhysicalGameObj()->Set_Animation(animation,false,0);
			if (CameraControl)
			{
				Commands->Innate_Disable(obj);
			}
		}
	}
}

void JFW_Cinematic::Command_Stop_Animation(char *command)
{
	int slot = atoi(Get_First_Parameter(command));
	if ((slot < 0) || (slot >= 40))
	{
		slot = -1;
	}
	if (slot != -1)
	{
		GameObject *obj = Commands->Find_Object(Slots[slot]);
		if (obj && obj->As_PhysicalGameObj())
		{
			obj->As_PhysicalGameObj()->Clear_Animation();
			if (CameraControl)
			{
				Commands->Innate_Disable(obj);
			}
		}
	}
}

void JFW_Cinematic::Command_Play_Audio(char *command)
{
	char *preset = Get_First_Parameter(command);
	char *slotc = Get_Next_Parameter();
	char *bone = Get_Next_Parameter();
	int slot = -1;
	if ((slotc) && (slotc[0]))
	{
		slot = atoi(slotc);
	}
	if ((slot < 0) || (slot >= 40))
	{
		slot = -1;
	}
	if (slot != -1)
	{
		GameObject *obj = Commands->Find_Object(Slots[slot]);
		if (obj)
		{
			Commands->Create_3D_Sound_At_Bone(preset,obj,bone);
		}
	}
	else
	{
		Commands->Create_2D_Sound(preset);
	}
}

void JFW_Cinematic::Command_Control_Camera(char *command)
{
	int slot = atoi(Get_First_Parameter(command));
	if ((slot < 0) || (slot >= 40))
	{
		slot = -1;
	}
	if (slot != -1)
	{
		GameObject *obj = Commands->Find_Object(Slots[slot]);
		if (obj)
		{
			Update_Network_Object(obj);
			Set_Camera_Host_Network(obj);
			Commands->Control_Enable(Commands->Get_The_Star(),false);
			Commands->Enable_HUD(false);
			CameraControl = true;
			Commands->Enable_Cinematic_Freeze(obj,false);
			Commands->Enable_Hibernation(obj,false);
		}
	}
	else
	{
		Set_Camera_Host_Network(0);
		Commands->Control_Enable(Commands->Get_The_Star(),true);
		Commands->Enable_HUD(true);
	}
}

void JFW_Cinematic::Command_Send_Custom(char *command)
{
	char *object = Get_First_Parameter(command);
	int type = atoi(Get_Next_Parameter());
	char *paramc = Get_Next_Parameter();
	int objectid = -1;
	char *c = strchr(object,'#');
	if (c)
	{
		int slot = atoi(c+1);
		if (slot < 40)
		{
			objectid = Slots[slot];
		}
	}
	else
	{
		objectid = atoi(object);
	}
	int param = 0;
	c = strchr(paramc,'#');
	if (c)
	{
		int slot = atoi(c+1);
		if (slot < 40)
		{
			param = Slots[slot];
		}
	}
	else
	{
		param = atoi(paramc);
	}
	Commands->Send_Custom_Event(Owner(),Commands->Find_Object(objectid),type,param,0);
}

void JFW_Cinematic::Command_Attach_To_Bone(char *command)
{
	int slot = atoi(Get_First_Parameter(command));
	int boneslot = atoi(Get_Next_Parameter());
	char *bone = Get_Next_Parameter();
	if ((slot < 0) || (slot >= 40))
	{
		slot = -1;
	}
	if ((boneslot < 0) || (boneslot >= 40))
	{
		boneslot = -1;
	}
	if (slot != -1)
	{
		GameObject *obj = Commands->Find_Object(Slots[slot]);
		if (obj)
		{
			if (boneslot != -1)
			{
				GameObject *boneobj = Commands->Find_Object(Slots[boneslot]);
				if (boneobj)
				{
					Commands->Attach_To_Object_Bone(obj,boneobj,bone);
				}
			}
			else
			{
				Commands->Attach_To_Object_Bone(obj,0,0);
			}
		}
	}
}

void JFW_Cinematic::Command_Attach_Script(char *command)
{
	int slot = atoi(Get_First_Parameter(command));
	char *script = Get_Next_Parameter();
	char *params = Get_Next_Parameter();
	if ((slot < 0) || (slot >= 40))
	{
		slot = -1;
	}
	if (slot != -1)
	{
		Commands->Attach_Script(Commands->Find_Object(Slots[slot]),script,params);
	}
}

void JFW_Cinematic::Command_Set_Primary(char *command)
{
	int slot = atoi(Get_First_Parameter(command));
	if ((slot < 0) || (slot >= 40))
	{
		slot = -1;
	}
	if (slot != -1)
	{
		GameObject *obj = Commands->Find_Object(Slots[slot]);
		if (obj)
		{
			Commands->Enable_Hibernation(obj,false);
			char params[20];
			sprintf(params,"%u",CallbackID);
			Commands->Attach_Script(obj,"JFW_Cinematic_Primary_Killed",params);
		}
	}
}

void JFW_Cinematic::Command_Move_Slot(char *command)
{
	int newslot = atoi(Get_First_Parameter(command));
	int slot = atoi(Get_Next_Parameter());
	if ((slot < 0) || (slot >= 40))
	{
		slot = -1;
	}
	if ((newslot < 0) || (newslot >= 40))
	{
		newslot = -1;
	}
	if ((newslot != -1) && (slot != -1) && (slot != newslot))
	{
		Slots[newslot] = Slots[slot];
		Slots[slot] = 0;
	}
}

void JFW_Cinematic::Command_Sniper_Control(char *command)
{
	bool sniper = atoi(Get_First_Parameter(command)) != 0;
	float zoom = (float)atof(Get_Next_Parameter());
	Commands->Cinematic_Sniper_Control(sniper != 0,zoom);
}

void JFW_Cinematic::Command_Shake_Camera(char *command)
{
	int slot = atoi(Get_First_Parameter(command));
	float intensity = (float)atof(Get_Next_Parameter());
	float duration = (float)atof(Get_Next_Parameter());
	if ((slot < 0) || (slot >= 40))
	{
		slot = -1;
	}
	if (slot != -1)
	{
		GameObject *obj = Commands->Find_Object(Slots[slot]);
		if (obj)
		{
			Commands->Shake_Camera(Commands->Get_Bone_Position(obj,"Camera"),100,intensity,duration);
		}
	}
}

void JFW_Cinematic::Command_Enable_Shadow(char *command)
{
	int slot = atoi(Get_First_Parameter(command));
	bool enable = atoi(Get_Next_Parameter()) != 0;
	if ((slot < 0) || (slot >= 40))
	{
		slot = -1;
	}
	if (slot != -1)
	{
		GameObject *obj = Commands->Find_Object(Slots[slot]);
		if (obj)
		{
			Commands->Enable_Shadow(obj,enable);
		}
	}
}

void JFW_Cinematic::Command_Enable_Letterbox(char *command)
{
	int enable = atoi(Get_First_Parameter(command));
	float letterbox = (float)atof(Get_Next_Parameter());
	Commands->Enable_Letterbox(!!enable,letterbox);
}

void JFW_Cinematic::Command_Set_Screen_Fade_Color(char *command)
{
	float red = (float)atof(Get_First_Parameter(command));
	float green = (float)atof(Get_Next_Parameter());
	float blue = (float)atof(Get_Next_Parameter());
	float transition = (float)atof(Get_Next_Parameter());
	Commands->Set_Screen_Fade_Color(red,green,blue,transition);
}

void JFW_Cinematic::Command_Set_Screen_Fade_Opacity(char *command)
{
	float opacity = (float)atof(Get_First_Parameter(command));
	float transition = (float)atof(Get_Next_Parameter());
	Commands->Set_Screen_Fade_Opacity(opacity,transition);
}

void JFW_Cinematic::Command_Show_Message(char* command)
{
    char* message = Get_First_Parameter(command);
    char* team_c = Get_Next_Parameter();
    char* r_c = Get_Next_Parameter();
    char* g_c = Get_Next_Parameter();
    char* b_c = Get_Next_Parameter();

    int team = team_c && team_c[0] ? atoi(team_c) : 2;
    int r,g,b;
    if (r_c && g_c && b_c && r_c[0] && g_c[0] && b_c[0])
    {
        r = atoi(r_c);
        g = atoi(g_c);
        b = atoi(b_c);
    }
    else
    {
        r = g = b = 255;
    }

    Send_Message_Team(team, r, g, b, message);
}

JFW_Cinematic::~JFW_Cinematic()
{
	while (Lines)
	{
		Remove_Head_Control_Line();
	}
}

void JFW_Cinematic_Kill_Object::Created(GameObject *obj)
{
	Commands->Apply_Damage(obj,10000,"BlamoKiller",0);
	Destroy_Script();
}

ScriptRegistrant<JFW_Zone_Play_Cinematic> JFW_Zone_Play_Cinematic_Registrant("JFW_Zone_Play_Cinematic","Script_Name:string,location:vector3,Player_Type:int,Facing:float");
ScriptRegistrant<JFW_Death_Play_Cinematic> JFW_Death_Play_Cinematic_Registrant("JFW_Death_Play_Cinematic","Script_Name:string,location:vector3,Facing:float");
ScriptRegistrant<JFW_Timer_Play_Cinematic> JFW_Timer_Play_Cinematic_Registrant("JFW_Timer_Play_Cinematic","Time:float,TimerNum:int,Repeat:int,Script_Name:string,Location:vector3,Facing:float");
ScriptRegistrant<JFW_Custom_Play_Cinematic> JFW_Custom_Play_Cinematic_Registrant("JFW_Custom_Play_Cinematic","Message:int,Script_Name:string,Location:vector3,Facing:float");
ScriptRegistrant<JFW_Customs_Play_Cinematic> JFW_Customs_Play_Cinematic_Registrant("JFW_Customs_Play_Cinematic","Message1:int,Message2:int,Message3:int,Message4:int,Message5:int,Message6:int,Message7:int,Message8:int,Script_Name:string,Location:vector3,Facing:float");
ScriptRegistrant<JFW_Looping_Cinematic> JFW_Looping_Cinematic_Registrant("JFW_Looping_Cinematic","Script_Name:string,Location:vector3,Facing:float,Time:float,TimerNum:int");
ScriptRegistrant<JFW_Cinematic_Attack> JFW_Cinematic_Attack_Registrant("JFW_Cinematic_Attack","Priority=96:int,ID:int,Range:float,Deviation:float,Primary:int");
ScriptRegistrant<JFW_Random_Timer_Play_Cinematic> JFW_Random_Timer_Play_Cinematic_Registrant("JFW_Random_Timer_Play_Cinematic","TimeMin:float,TimeMax:float,TimerNum:int,Repeat:int,Script_Name:string,Location:vector3,Facing:float");
ScriptRegistrant<JFW_Timer_Play_Random_Cinematic> JFW_Timer_Play_Random_Cinematic_Registrant("JFW_Timer_Play_Random_Cinematic","Time:float,TimerNum:int,Repeat:int,Script_Name1:string,Script_Name2:string,Script_Name3:string,Script_Name4:string,Script_Name5:string,Location:vector3,Facing:float");
ScriptRegistrant<JFW_Random_Timer_Play_Random_Cinematic> JFW_Random_Timer_Play_Random_Cinematic_Registrant("JFW_Random_Timer_Play_Random_Cinematic","TimeMin:float,TimeMax:float,TimerNum:int,Repeat:int,Script_Name1:string,Script_Name2:string,Script_Name3:string,Script_Name4:string,Script_Name5:string,Location:vector3,Facing:float");
ScriptRegistrant<JFW_Cinematic_Attack_Position> JFW_Cinematic_Attack_Position_Registrant("JFW_Cinematic_Attack_Position","Priority=96:int,Position:vector3,Range:float,Deviation:float,Primary:int");
ScriptRegistrant<JFW_Cinematic_Attack_Command> JFW_Cinematic_Attack_Command_Registrant("JFW_Cinematic_Attack_Command","AttackDuration=1.0:float");
ScriptRegistrant<JFW_Cinematic> JFW_Cinematic_Registrant("JFW_Cinematic","ControlFilename=:string,StartPaused=0:int");
ScriptRegistrant<JFW_Cinematic> Test_Cinematic_Registrant("Test_Cinematic","ControlFilename=:string");
ScriptRegistrant<JFW_Cinematic_Kill_Object> JFW_Cinematic_Kill_Object_Registrant("JFW_Cinematic_Kill_Object","");
