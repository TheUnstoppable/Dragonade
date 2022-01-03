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
class JFW_Attatch_Script : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Poked(GameObject *obj,GameObject *poker);
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Remove_Script_Preset_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Attach_Script_Preset_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Remove_Script_Type_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Attach_Script_Type_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Remove_Script_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Remove_Script_Death : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

/*!
* \brief Attach Script on Custom
* \author jonwil
* \ingroup scripts_customconsumers
*
* Upon receiving the specified custom message this script will attach another script to the object
* it is attached to.
*
* \param Script
*   The name of the script to be attached
* \param Params
*   The parameters for the script to be attached
* \param Delim
*   The delimiter between each value in the <i>Params</i> parameter
* \param Message
*   The custom message to listen for
*/
class JFW_Attach_Script_Custom : public ScriptImpClass
{
  void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Attach_Script_Custom_Until_Custom : public ScriptImpClass
{
  void Custom(GameObject *obj, int type, int param, GameObject *sender);
};

class JFW_Remove_All_Scripts_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Attach_Script_Preset_Once_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Attach_Script_Type_Once_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Attach_Script_Once_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Attach_Script_Preset_Startup : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Attach_Script_Building_Startup : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Attach_Script_Type_Startup : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Attach_Script_Collector : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Attach_Script_Sender : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};
