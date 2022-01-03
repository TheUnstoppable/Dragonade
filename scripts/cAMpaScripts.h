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
class cAMpa_Warn_Zone_Team : public ScriptImpClass {
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
};

class cAMpa_Kill_Zone_Team : public ScriptImpClass {
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
};

class cAMpa_Teleport_Zone_Small : public ScriptImpClass {
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	int lastUsed;
};

class cAMpa_Teleport_Zone_Large : public ScriptImpClass {
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	int lastUsed;
};
