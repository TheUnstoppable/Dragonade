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
class JFW_Zone_Play_Cinematic : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Death_Play_Cinematic : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_Timer_Play_Cinematic : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Custom_Play_Cinematic : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Customs_Play_Cinematic : public ScriptImpClass {
	bool custom1;
	bool custom2;
	bool custom3;
	bool custom4;
	bool custom5;
	bool custom6;
	bool custom7;
	bool custom8;
	bool trigger;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Looping_Cinematic : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Cinematic_Attack : public ScriptImpClass {
	void Created(GameObject *obj);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
};

class JFW_Random_Timer_Play_Cinematic : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Timer_Play_Random_Cinematic : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Random_Timer_Play_Random_Cinematic : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Cinematic_Attack_Position : public ScriptImpClass {
	void Created(GameObject *obj);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
};

class JFW_Cinematic_Attack_Command : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

struct ControlLine {
	float Frame;
	char *Line;
	ControlLine *Next;
};

class JFW_Cinematic : public ScriptImpClass {
	unsigned int Slots[40];
	unsigned int CallbackID;
	unsigned int SyncTime;
	float f1;
	float f2;
	bool PrimaryKilled;
	bool CameraControl;
    bool StartPaused;
	char *CurrentLine;
	ControlLine *Lines;
	void Created(GameObject *obj);
    void Start_Cinematic(GameObject* obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Save(ScriptSaver &saver);
	void Load(ScriptLoader &loader);
	void Load_Control_File(const char *file);
	void Parse_Commands(GameObject *obj);
	void Add_Control_Line(float frame,const char *line);
	void Parse_Command(char *command);
	char *Get_First_Parameter(char *command);
	char *Get_Next_Parameter();
	char *Get_Command_Parameter(char *command = NULL);
	void Remove_Head_Control_Line();
	bool Title_Match(char **commands,char *match);
	void Command_Create_Object(char *command);
	void Command_Create_Real_Object(char *command);
	void Command_Add_Object(char *command);
	void Command_Create_Explosion(char *command);
	void Command_Destroy_Object(char *command);
	void Command_Play_Animation(char *command);
	void Command_Play_Complete_Animation(char *command);
	void Command_Stop_Animation(char *command);
	void Command_Play_Audio(char *command);
	void Command_Control_Camera(char *command);
	void Command_Send_Custom(char *command);
	void Command_Attach_To_Bone(char *command);
	void Command_Attach_Script(char *command);
	void Command_Set_Primary(char *command);
	void Command_Move_Slot(char *command);
	void Command_Sniper_Control(char *command);
	void Command_Shake_Camera(char *command);
	void Command_Enable_Shadow(char *command);
	void Command_Enable_Letterbox(char *command);
	void Command_Set_Screen_Fade_Color(char *command);
	void Command_Set_Screen_Fade_Opacity(char *command);
    void Command_Show_Message(char* command);
public:
	JFW_Cinematic() {Lines = 0; CurrentLine = 0;}
	~JFW_Cinematic();
};

class JFW_Cinematic_Kill_Object : public ScriptImpClass {
	void Created(GameObject *obj);
};
