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
#ifndef TT_INCLUDE__HTREECLASS_H
#define TT_INCLUDE__HTREECLASS_H
#include "Matrix3D.h"
class HAnimClass;
class HAnimComboClass;
class ChunkLoadClass;
enum WW3DErrorType;
struct PivotClass
{
	char Name[16]; //0
	int Index; //16
	PivotClass* Parent; //20
	Matrix3D BaseTransform; //24
	Matrix3D Transform; //72
	bool IsVisible; //120
	bool IsCaptured; //121
	Matrix3D CapTransform; //124
	bool WorldSpaceTranslation; //172

	PivotClass() : Index(0), Parent(0), BaseTransform(true), Transform(true), IsVisible(true), IsCaptured(false), CapTransform(true), WorldSpaceTranslation(false)
	{
	}

	void Capture_Update()
	{
		if (WorldSpaceTranslation)
		{
			Matrix3D CapOrientation = CapTransform;
			CapOrientation[0].W = 0;
			CapOrientation[1].W = 0;
			CapOrientation[2].W = 0;
			Matrix3D::Multiply(Transform,CapOrientation,&Transform);
			Transform[0].W += CapTransform[0].W;
			Transform[1].W += CapTransform[1].W;
			Transform[2].W += CapTransform[2].W;
		}
		else
		{
			Matrix3D::Multiply(Transform,CapTransform,&Transform);
		}
	}
};

class HTreeClass
{
private:
	char Name[16]; //0
	int NumPivots; //16
	PivotClass* Pivot; //20
	float ScaleFactor; //24

public:
	static HTreeClass* Create_Interpolated(const HTreeClass* base, const HTreeClass* a, const HTreeClass* b, float weightA, float weightB);
	HTreeClass(const HTreeClass& that);
	HTreeClass();
	~HTreeClass();
	WW3DErrorType	Load_W3D(ChunkLoadClass& cload);
	void			Init_Default();
	const char *Get_Name() const
	{
		return Name;
	}
	int Num_Pivots() const
	{
		return NumPivots;
	}
	int Get_Bone_Index(const char *) const;
	const char *Get_Bone_Name(int) const;
	int Get_Parent_Index(int boneidx) const;
	void Base_Update(const Matrix3D& root);
	void Anim_Update(Matrix3D  const&, HAnimClass *, float);
	void Blend_Update(Matrix3D  const&, HAnimClass *, float, HAnimClass *, float, float);
	void Combo_Update(Matrix3D  const&, HAnimComboClass *);
	const Matrix3D& Get_Transform(int pivot) const
	{
		return this->Pivot[pivot].Transform;
	}
	bool Get_Visibility(int pivot) const
	{
		return this->Pivot[pivot].IsVisible;
	}
	const Matrix3D& Get_Root_Transform() const
	{
		return this->Pivot[0].Transform;
	}
	void Capture_Bone(int boneindex);
	void Release_Bone(int boneindex);
	bool Is_Bone_Captured(int boneindex);
	void Control_Bone(int boneindex, Matrix3D const &relative_tm, bool world_space_translation);
	bool Simple_Evaluate_Pivot(HAnimClass *motion, int pivot_index, float frame,const Matrix3D &obj_tm, Matrix3D *end_tm) const;
	bool Simple_Evaluate_Pivot(int pivot_index,const Matrix3D &obj_tm, Matrix3D *end_tm) const;
	void Scale(float factor);
	void			Free();
	void Get_Bone_Control(int, Matrix3D&) const;
private:
	HTreeClass& operator = (const HTreeClass& that); // unimplemented
	WW3DErrorType read_pivots(ChunkLoadClass& cload, bool create_root);
};

#endif
