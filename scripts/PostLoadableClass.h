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
#ifndef TT_INCLUDE__POSTLOADABLECLASS_H
#define TT_INCLUDE__POSTLOADABLECLASS_H

class SCRIPTS_API PostLoadableClass
{
public:
	PostLoadableClass(void) : IsPostLoadRegistered(false)						{ }
	virtual ~PostLoadableClass(void)													{ }
	virtual void						On_Post_Load (void)							{ }
	bool									Is_Post_Load_Registered(void) const				{ return IsPostLoadRegistered; }
	void									Set_Post_Load_Registered(bool onoff)	{ IsPostLoadRegistered = onoff; }
private:
	bool									IsPostLoadRegistered;
}; // 0008

#endif
