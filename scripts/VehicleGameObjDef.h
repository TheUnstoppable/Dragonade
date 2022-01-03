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
#ifndef TT_INCLUDE__VEHICLEGAMEOBJDEF_H
#define TT_INCLUDE__VEHICLEGAMEOBJDEF_H

#include "engine_vector.h"
#include "SmartGameObjDef.h"
class VehicleGameObj;
class TransitionDataClass;
enum
{
	ENGINE_SOUND_STATE_STARTING = 0,
	ENGINE_SOUND_STATE_RUNNING,
	ENGINE_SOUND_STATE_STOPPING,
	ENGINE_SOUND_STATE_OFF,

	MAX_ENGINE_SOUND_STATES
};
typedef enum {
	VEHICLE_TYPE_CAR,
	VEHICLE_TYPE_TANK,
	VEHICLE_TYPE_BIKE,
	VEHICLE_TYPE_FLYING,
	VEHICLE_TYPE_TURRET,
	VEHICLE_TYPE_BOAT,
	VEHICLE_TYPE_SUB,
} VehicleType;
class VehicleGameObjDef : public SmartGameObjDef
{
public:
	VehicleGameObjDef( void );
	virtual ~VehicleGameObjDef( void );
	virtual uint32								Get_Class_ID( void ) const;
	virtual PersistClass *					Create( void ) const ;
	virtual bool								Save( ChunkSaveClass &csave );
	virtual bool								Load( ChunkLoadClass &cload );
	virtual	void	On_Post_Load( void );
	virtual const PersistFactoryClass &	Get_Factory( void ) const;
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
	virtual void                        DumpPhys (FileClass &file);
#endif
	DECLARE_EDITABLE( VehicleGameObjDef, SmartGameObjDef );
	DynamicVectorClass<TransitionDataClass*> *					Get_Transition_List( void ) { return &Transitions; }
	const DynamicVectorClass<TransitionDataClass*> &			Get_Transition_List( void ) const { return Transitions; }
	void											Free_Transition_List( void );
	int Get_Damage_Report(int team) const;
	int Get_Destroy_Report(int team) const;
	int Get_Seat_Count() const {return NumSeats;}
	VehicleType Get_Type() const {return Type;}
	const StringClass &Get_Fire0_Anim() const {return Fire0Anim;}
	const StringClass &Get_Fire1_Anim() const {return Fire1Anim;}
	bool Get_Build_At_Ground() const {return BuildAtGround;}
	float Get_Factory_Ride_Height() const {return FactoryRideHeight;}
	int Get_Purchase_Report(int team) const;
	bool Can_Repair() const { return CanRepair; }
	int Get_Engine_Sound(int type) const { return EngineSound[type]; }
//protected: //DA
	VehicleType									Type;
	StringClass									TypeName;
	StringClass									Fire0Anim;
	StringClass									Fire1Anim;
	StringClass									Profile;
	DynamicVectorClass<TransitionDataClass*>	Transitions;
	float											TurnRadius;
	bool											OccupantsVisible;
	bool											SightDownMuzzle;
	bool											Aim2D;
	float											EngineSoundMaxPitchFactor;
	int											EngineSound[MAX_ENGINE_SOUND_STATES];
	float											SquishVelocity;
	int											VehicleNameID;
	int											NumSeats;
	int GDIDamageReportID;
	int NodDamageReportID;
	int GDIDestroyReportID;
	int NodDestroyReportID;
	bool CanBeStolen;
	bool AllowStealthWhileEmpty;
	bool BuildAtGround;
	bool DualAim;
	float FactoryRideHeight;
	bool FakeTurret;
	int GDIPurchaseReportID;
	int NodPurchaseReportID;
	bool CanRepair;
	int EntryHelpMessage;
	Vector3 EntryHelpMessageColor;
	bool ApplySurfaceEffectDamage;
	bool DisableSurfaceSounds;
	friend	class								VehicleGameObj;
	friend class PresetDump;
}; // size: 324


#endif
