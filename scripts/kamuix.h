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
class Kamuix_PAMSG_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};
class Kamuix_AdminMessage_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};
class Kamuix_Send_FDS_MSG_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};
class Kamuix_Team_Change_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};
class Kamuix_Announce_Preset_Buy : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class Kamuix_Death_Announce : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class Kamuix_Death_Team_Win : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class Kamuix_Damaged_Send_MsgFds : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float amount);
};

class Kamuix_Death_Send_MsgFds : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};
class Kamuix_Kill_Change : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class Kamuix_Player_Announce_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class Kamuix_Ban_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class Kamuix_Kick_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};
class Zone_Kamuix_Destroy_Object : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

//class Kamuix_Crash_Player_Zone : public ScriptImpClass {
	//void Entered(GameObject *obj,GameObject *enterer);
//};
class Kamuix_Set_Type : public ScriptImpClass {
	void Created(GameObject *obj);
};
class Kamuix_Revive_Building_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};
class Kamuix_Set_Building_Health_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};
class Kamuix_Rebuild_Structure_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};
class Kamuix_Death_Destroy_Building : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};
class Kamuix_Control_Spawner : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};
class Kamuix_Zone_Destroy_Building : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};
class Kamuix_Set_Model : public ScriptImpClass {
	void Created(GameObject *obj);
};
