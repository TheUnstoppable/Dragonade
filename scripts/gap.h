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
class RA_Global_Gap_Controller : public ScriptImpClass {
	Gap_ListNode *ListHead;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	void Destroyed(GameObject *obj);
	void Add_Generator(unsigned long ObjectID, float Range, int Team);
	void Remove_Generator(unsigned long ID);
	void Update_Generator(unsigned long ID, bool Active, int Team);
	void Update_Generator(unsigned long ID, bool Active);
};

class RA_Gap_Generator_Building : public ScriptImpClass {
	unsigned long shroudID;
	unsigned long sizeID;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Killed(GameObject *obj, GameObject *killer);
};

class RA_Gap_Generator_Vehicle : public ScriptImpClass {
	unsigned long shroudID;
	unsigned long sizeID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	void Destroyed(GameObject *obj);
};

class RA_Gap_Generator_Ignored : public ScriptImpClass {
};
