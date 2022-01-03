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
#ifndef TT_INCLUDE__SYSTIMECLASS_H
#define TT_INCLUDE__SYSTIMECLASS_H


class SCRIPTS_API SysTimeClass
{
private:
	uint32 uTimeInit;
	uint32 uTimeInitNeg;
public:
#if (TDBEDIT) || (DDBEDIT) || (W3DVIEWER)
	static bool _is_init;
	static SysTimeClass SystemTime;
#else
	static REF_DECL(bool, _is_init);
	static REF_DECL(SysTimeClass, SystemTime);
#endif
	uint32 Get   ();
	void   Reset ();

};
#define TIMEGETTIME() SysTimeClass::SystemTime.Get()
#endif
