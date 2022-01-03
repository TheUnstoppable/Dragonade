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
#ifndef TT_INCLUDE__SOUND3DCLASS_H
#define TT_INCLUDE__SOUND3DCLASS_H



#include "AudibleSoundClass.h"



class Sound3DClass :
	public AudibleSoundClass
{

public:
	bool IsTransformValid;
	bool IsAutoCalcVelocity;
	Vector3 Velocity;
	float MaxVolRadius;
	bool IsStatic;
	uint32 Time;
	Sound3DClass();
	virtual ~Sound3DClass();
	virtual Sound3DClass*       As_Sound3DClass        ();
	virtual bool                On_Frame_Update         (uint32 timePast);
	virtual void                Set_Position            (const Vector3& position);
	virtual Vector3             Get_Position            () const;
	virtual void                Set_Listener_Transform  (const Matrix3D& listenerTransform);
	virtual void                Set_Transform           (const Matrix3D& transform);
	virtual Matrix3D            Get_Transform           () const;
	virtual void                Add_To_Scene            (bool);
	virtual void                Remove_From_Scene       ();
	virtual void                Set_DropOff_Radius      (float dropOffRadius);
	virtual bool                Save (ChunkSaveClass& xSave);
	virtual bool                Load (ChunkLoadClass& xLoad);
	virtual SoundCullObjClass*  Peek_Cullable_Wrapper  () const;
	virtual void                Set_Cullable_Wrapper   (SoundCullObjClass* xCullObj);
	virtual bool                Play                    (bool addToPlaylist);
	virtual float               Get_Priority            () const;
	virtual const PersistFactoryClass &Get_Factory             () const;
	virtual void                Set_Miles_Handle        (HSAMPLE milesHandle);
	virtual void                Initialize_Miles_Handle ();
	virtual void                Allocate_Miles_Handle   ();
	virtual void                On_Loop_End             ();
	virtual void                Make_Static             (bool);
	virtual bool                Is_Static               ();
	virtual void                Set_Velocity            (Vector3 const&);
	virtual Vector3             Get_Velocity            ();
	virtual void                Get_Velocity            (Vector3 &);
	virtual void                Auto_Calc_Velocity      (bool);
	virtual bool                Is_Auto_Calc_Velocity_On();
	virtual void                Set_Max_Vol_Radius      (float);
	virtual float               Get_Max_Vol_Radius      ();
	virtual float               Get_DropOff_Radius      ();
	virtual unsigned long       Get_Class_ID            () const { return CLASSID_3D; }
	void Update_Edge_Volume();
};

#endif
