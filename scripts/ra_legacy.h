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
class RA_Thief_Script: public ScriptImpClass {
	bool enabled;
	void Timer_Expired(GameObject *obj,int number);
	void Entered(GameObject *obj,GameObject *enterer);
	void Created(GameObject *obj);
	public: void Register_Auto_Save_Variables();
};

class RA_Soviet_Defence : public ScriptImpClass {
	unsigned int id1;
	unsigned int id2;
	unsigned int id3;
	unsigned int objtype;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class RA_Soviet_Refinery_Controller : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class RA_Soviet_Refinery_Theft_Zone : public ScriptImpClass {
	bool refinery;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	public: void Register_Auto_Save_Variables();
};

class RA_Allied_Thief : public ScriptImpClass {
	bool stealing;
	bool ready; // controls timer lockout of zone entry money award
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class RA_Naval_PT : public ScriptImpClass {
	int storedID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class RA_Conyard_Controller : public ScriptImpClass {
	//Clone of TDA_Conyard_Controller with a little extra
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
};
