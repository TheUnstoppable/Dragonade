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
#ifndef TT_INCLUDE_TRANSITIONGAMEOBJ_H
#define TT_INCLUDE_TRANSITIONGAMEOBJ_H
#include "BaseGameObj.h"
#include "engine_vector.h"
#include "TransitionInstanceClass.h"
#include "Matrix3D.h"
class TransitionGameObjDef;
class TransitionGameObj : public BaseGameObj {
private:
	DynamicVectorClass<TransitionInstanceClass *> Transitions;
	Matrix3D Transform;
	int LadderIndex;
public:
	const PersistFactoryClass &Get_Factory() const;
	TransitionGameObj();
	~TransitionGameObj();
	void Init();
	void Init(const TransitionGameObjDef &definition);
	SCRIPTS_API const TransitionGameObjDef & Get_Definition( void ) const ;
	bool Save(ChunkSaveClass &csave);
	bool Load(ChunkLoadClass &cload);
	void On_Post_Load();
	void Set_Transform(const Matrix3D &tm) {Transform = tm;}
	const Matrix3D &Get_Transform() const {return Transform;}
	void Create_Transitions();
	void Destroy_Transitions();
	void Update_Transitions();
	void Set_Ladder_Index(int index);
	int Get_Transition_Count() {return Transitions.Count();};
	TransitionInstanceClass *Get_Transition(int trans) {return Transitions[trans];}
	int Get_Ladder_Index() {return LadderIndex;}
};

#endif
