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
#ifndef TT_INCLUDE__HUMANSTATECLASS_H
#define TT_INCLUDE__HUMANSTATECLASS_H

#include "Matrix3D.h"
#include "engine_io.h"
class WeaponClass;
class HumanPhysClass;
class HumanAnimControlClass;
class HumanAnimOverrideDef;
class HumanLoiterGlobalSettingsDef;
class HumanStateClass
{

public:

	typedef enum {
		// Interruptable states
		UPRIGHT,
		LAND,
		ANIMATION,
		WOUNDED,
		LOITER,

		// Uninterruptable states
		AIRBORNE,
		DIVE,
		DEATH,
		LADDER,
		IN_VEHICLE,
		TRANSITION,
		TRANSITION_COMPLETE,
		DESTROY,
		DEBUG_FLY,
		ON_FIRE,
		ON_CHEM,
		ON_ELECTRIC,
		ON_CNC_FIRE,
		ON_CNC_CHEM,
		LOCKED_ANIMATION,

		HIGHEST_HUMAN_STATE = LOCKED_ANIMATION

	} HumanStateType;

	typedef enum {
		CROUCHED_FLAG		=	1 << 0,
		SNIPING_FLAG		=	1 << 1,
		HIGHEST_HUMAN_STATE_FLAGS = (1 << 2) - 1
	} HumanStateFlagsType;

	typedef enum {
		SUB_STATE_FORWARD			=	1 << 0,
		SUB_STATE_BACKWARD		=	1 << 1,
		SUB_STATE_UP				=	1 << 2,
		SUB_STATE_DOWN				=	1 << 3,
		SUB_STATE_LEFT				=	1 << 4,
		SUB_STATE_RIGHT			=	1 << 5,
		SUB_STATE_TURN_LEFT		=	1 << 6,
		SUB_STATE_TURN_RIGHT		=	1 << 7,
		SUB_STATE_SLOW				=	1 << 8,
		HIGHEST_HUMAN_SUB_STATE = (1 << 9) - 1
	} HumanSubStateType;

	typedef enum {
		HEAD_FROM_BEHIND,
		HEAD_FROM_FRONT,
		TORSO_FROM_BEHIND,
		TORSO_FROM_FRONT,
		LEFT_ARM_FROM_BEHIND,
		LEFT_ARM_FROM_FRONT,
		RIGHT_ARM_FROM_BEHIND,
		RIGHT_ARM_FROM_FRONT,
		LEFT_LEG_FROM_BEHIND,
		LEFT_LEG_FROM_FRONT,
		RIGHT_LEG_FROM_BEHIND,
		RIGHT_LEG_FROM_FRONT,
		GROIN,
		OUCH_FIRE,
		OUCH_CHEM,
		OUCH_ELECTRIC,
		OUCH_SUPER_FIRE,
	} HumanOuchType;

	HumanStateClass( void );
	~HumanStateClass( void );
	void				Init( HumanPhysClass	* human_phys );
	void				Reset(void);
	void				Set_Anim_Control( HumanAnimControlClass * anim_control );
	void				Set_Human_Anim_Override( const char * name );
	void				Set_Human_Anim_Override( int def_id );
	void				Set_Human_Loiter_Collection( int def_id );
	bool 				Save( ChunkSaveClass & csave );
	bool				Load( ChunkLoadClass & cload );
	void				Set_State( HumanStateType state, int sub_state = 0 );
	HumanStateType	Get_State( void )												{ return State; }
	const char *	Get_State_Name( void );
	bool				Is_State_Interruptable( void );
	void				Set_Sub_State( int sub_state );
	int				Get_Sub_State( void )										{ return SubState; }
	bool				Is_Sub_State_Adjustable( void );
	float				Get_State_Timer( void )										{ return StateTimer; }
	void				Set_State_Timer( float timer )							{ StateTimer = timer; }
	void				Toggle_State_Flag( int flag )								{ StateFlags ^= flag; }
	bool				Get_State_Flag( int flag )									{ return (StateFlags & flag) != 0; }
	void				Drop_Weapon( void )											{ WeaponHoldTimer = 0.001f; }
	void				Raise_Weapon( void )											{ WeaponHoldTimer = 10; }
	void				Start_Transition_Animation( const char * anim_name, bool blend = true );
	void				Start_Scripted_Animation( const char * anim_name, bool blend = true, bool looping = true );
	void				Stop_Scripted_Animation( void );
	void				Force_Animation( const char * anim_name, bool blend = true );
	void				Set_Turn_Velocity( float vel )							{ TurnVelocity = vel; }
	void				Update_Weapon( WeaponClass * weapon, bool new_weapon );
	void				Update_Aiming( float tilt, float facing );
	void				Update_State( void );
	void				Post_Think( void );
	void				Update_Animation( void );
	bool				Is_Locked( void )												{ return StateLocked; }
	bool				Get_Leg_Mode( void );
	int				Get_Ouch_Type( const	Vector3 & direction, const char * collision_box_name );
	static void		Set_Precision(void);
	void				Reset_Loiter_Delay( void );
	void				Set_Loiters_Allowed( bool allowed )						{ LoitersAllowed = allowed; }
	void				Get_Information( StringClass & string );
	void				Set_Jump_Transform(const Matrix3D &Transform) { JumpTM = Transform; } //DA
	const Matrix3D &	Get_Jump_Transform() { return JumpTM; } //DA
private:
	bool					StateLocked;
	HumanStateType		State;
	float					StateTimer;
	int					StateFlags;
	int					SubState;
	int					WeaponHoldStyle;
	float					WeaponHoldTimer;
	bool					LoitersAllowed;
	float					LoiterDelay;
	float					AimingTilt;
	float					AimingTurn;
	HumanPhysClass	*	HumanPhys;
	HumanAnimControlClass * AnimControl;
	float					TurnVelocity;
	float					LegRotation;
	Matrix3D				JumpTM;
	float					RecoilTimer;
	float					RecoilScale;
	bool					NoAnimBlend;
	bool					WeaponFired;
	HumanAnimOverrideDef	* HumanAnimOverride;
	HumanLoiterGlobalSettingsDef * HumanLoiterCollection;
	static	const char *	Get_Wound_Anim( int ouch_type );
	static	const char *	Get_Death_Anim( int ouch_type );
	void					Update_Recoil(WeaponClass * weapon);
	void					Begin_Jump( void );
	void					Complete_Jump( void );
	void					GetLegAndHoldStyle (int *leg_style,int *hold_style);
};



#endif
