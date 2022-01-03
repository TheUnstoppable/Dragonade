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
#ifndef TT_INCLUDE__PHYSCONTROLLERCLASS_H
#define TT_INCLUDE__PHYSCONTROLLERCLASS_H

#include "Vector3.h"
class ChunkLoadClass;
class ChunkSaveClass;
class PhysControllerClass 
{
public:
	PhysControllerClass(void) : MoveVector(0,0,0), TurnLeft(0) { }
	void					Reset(void)									{ Reset_Move(); Reset_Turn(); }
	void					Set_Move_Forward(float scl)			{ MoveVector.X = scl; }
	void					Set_Move_Left(float scl)				{ MoveVector.Y = scl; }
	void					Set_Move_Up(float scl)					{ MoveVector.Z = scl; }
	void					Set_Turn_Left(float scl)				{ TurnLeft = scl; }
	float					Get_Move_Forward(void)					{ return MoveVector.X; }
	float					Get_Move_Left(void)						{ return MoveVector.Y; }
	float					Get_Move_Up(void)							{ return MoveVector.Z; }
	float					Get_Turn_Left(void)						{ return TurnLeft; }
	void					Reset_Move(void)							{ MoveVector.Set(0,0,0); }
	const Vector3	&	Get_Move_Vector(void)					{ return MoveVector; }	
	void					Reset_Turn(void)							{ TurnLeft = 0.0f; }
	bool					Is_Inactive(void)
	{
		return ((TurnLeft == 0.0f) && (MoveVector.Length2() == 0.0f));
	}
	bool					Save(ChunkSaveClass & csave);
	bool					Load(ChunkLoadClass & cload);
private:
	Vector3				MoveVector;
	float					TurnLeft;
}; // size: 16


#endif
