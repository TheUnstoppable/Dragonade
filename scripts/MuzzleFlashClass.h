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
#ifndef TT_INCLUDE__MUZZLEFLASHCLASS_H
#define TT_INCLUDE__MUZZLEFLASHCLASS_H



class RenderObjClass;
class MuzzleFlashClass
{
public:
	MuzzleFlashClass( void );
	~MuzzleFlashClass( void );
	void	Init( RenderObjClass * robj );
	void	Update( bool flashA0, bool flashA1, bool flashB0, bool flashB1 );
private:

	void UpdateMuzzleFlash(int bone, bool flash, bool& last_flash);

	int MuzzleA0Bone; // 0000
	int MuzzleA1Bone; // 0004
	int Rotation; // 0008
	RenderObjClass* Model; // 000C
	bool LastFlashA0; // 0010
	bool LastFlashA1; // 0011
// Added by scripts
	int MuzzleB0Bone;
	int MuzzleB1Bone;
	bool LastFlashB0;
	bool LastFlashB1;
}; // 0014
#endif
