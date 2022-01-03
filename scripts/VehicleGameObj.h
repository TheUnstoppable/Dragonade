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
#ifndef TT__VEHICLEGAMEOBJ_H
#define TT__VEHICLEGAMEOBJ_H



#include "engine_vector.h"
#include "SmartGameObj.h"
#include "PhysClass.h"
#include "VehicleGameObjDef.h"
#include "SoldierGameObj.h"
#ifndef TTLE_EXPORTS
#include "UndergroundEffectClass.h"
#endif
class AudibleSoundClass;
class VehicleGameObjDef;
class PersistantSurfaceEmitterClass;
class PersistantSurfaceSoundClass;
class VehiclePhysClass;
class Sound3DClass;
class TransitionInstanceClass;
class UndergroundEffectClass;

class VehicleGameObj :
	public SmartGameObj
{
public:
	VehicleGameObj();
	virtual	~VehicleGameObj();
	virtual	void	Init( void );
	void	Init( const VehicleGameObjDef & definition );
	const VehicleGameObjDef SCRIPTS_API & Get_Definition( void ) const ;
	virtual	bool	Save( ChunkSaveClass & csave );
	virtual	bool	Load( ChunkLoadClass & cload );
	virtual	void	On_Post_Load( void );
	virtual	const	PersistFactoryClass & Get_Factory( void ) const;
	virtual	void	Startup( void );
	VehiclePhysClass * Peek_Vehicle_Phys( void ) const;
	virtual	void	Think( void );
	virtual	void	Post_Think( void );
	virtual	void	Apply_Control( void );
	virtual	int	Get_Weapon_Control_Owner(void);
	virtual bool		Set_Targeting( const Vector3 & pos, bool do_tilt = true );
	virtual bool		Use_2D_Aiming( void ) 				{ return Get_Definition().Aim2D; }
	virtual Matrix3D	Get_Look_Transform(void);
	bool					Has_Turret( void)						{ return TurretBone != 0; }
	virtual VehicleGameObj		* As_VehicleGameObj()		{ return this; }
	virtual	bool	Is_Aircraft( void )						{ return Get_Definition().Type == VEHICLE_TYPE_FLYING || Get_Definition().Type == VEHICLE_TYPE_SUB; }
	virtual  bool  Is_Turret( void )							{ return Get_Definition().Type == VEHICLE_TYPE_TURRET; }
	virtual int Get_Player_Type(void) const;
	virtual	void	Apply_Damage_Extended( const OffenseObjectClass & offense, float scale = 1.0f,
			const	Vector3 & direction = Vector3( 0,0,0 ), const char * collision_box_name = NULL );
	SCRIPTS_API void	Add_Occupant(SoldierGameObj* occupant, int seat_id); //DA
	SCRIPTS_API void	Add_Occupant( SoldierGameObj * occupant );
	void	Remove_Occupant( SoldierGameObj * occupant );
	bool	Contains_Occupant( SoldierGameObj * occupant );
	int	SCRIPTS_API Get_Occupant_Count(void);
	int	Find_Seat( SoldierGameObj * occupant );
	SCRIPTS_API SoldierGameObj* Get_Driver(void); //DA
	SCRIPTS_API SoldierGameObj* Get_Gunner(void); //DA
	SCRIPTS_API SoldierGameObj* Get_Actual_Gunner(void); //DA
	virtual bool Is_Entry_Permitted(SoldierGameObj * p_soldier);
	void	Passenger_Entering( void );
	void	Passenger_Exiting( void );
	void Set_Vehicle_Delivered(void);
	virtual	void	Export_Creation( BitStreamClass & packet );
	virtual	void	Import_Creation( BitStreamClass & packet );
	virtual	void	Export_Rare( BitStreamClass &packet );
	virtual	void	Import_Rare( BitStreamClass &packet );
	virtual void	Export_Occasional( BitStreamClass &packet );
	virtual void	Import_Occasional( BitStreamClass &packet );
	virtual	void	Import_Frequent( BitStreamClass & packet );
	virtual	void	Export_Frequent( BitStreamClass & packet );
	virtual	void	Import_State_Cs( BitStreamClass & packet );
	virtual	void	Export_State_Cs( BitStreamClass & packet );
	virtual CollisionReactionType	Collision_Occurred( const CollisionEventClass & event );
	const char *Get_Profile( void )						{ return Get_Definition().Profile; }
	void Get_Velocity(Vector3 & vel);
	void Set_Velocity(Vector3 & vel);
	virtual	void	Get_Description( StringClass & description );
	float Get_Turn_Radius (void) const	{ return Get_Definition ().TurnRadius; }
	bool	Is_Engine_Enabled (void) const;
	void	Enable_Engine (bool onoff);
	void	Init_Wheel_Effects( void );
	void	Shutdown_Wheel_Effects( void );
	void	Update_Wheel_Effects( void );
	virtual	void	Apply_Damage( const OffenseObjectClass & damager, float scale = 1.0f, int alternate_skin = -1 );
	void	Update_Damage_Meshes( void );
	float	Get_Squish_Velocity( void) { return Get_Definition().SquishVelocity; }
	void Script_Enable_Transitions( bool enable )	{	TransitionsEnabled = enable; Create_And_Destroy_Transitions(); }
	bool Get_Transitions_Enabled ( void ) { return TransitionsEnabled; }
	static void Set_Precision(void);
	static bool	Toggle_Target_Steering( void );
	static void	Set_Target_Steering( bool onoff );
	static bool Is_Target_Steering( void );
	const wchar_t *		Get_Vehicle_Name( void );
	virtual ExpirationReactionType Object_Expired( PhysClass * observed_obj );
	void						Lock_Vehicle( ScriptableGameObj *lockowner, float locktime ) { LockOwner = lockowner, LockTimer = locktime; }
	bool						Is_Locked(void) { return ((LockOwner.Get_Ptr() != NULL) && (LockTimer > 0.0f)); }
	ScriptableGameObj *	Get_Lock_Owner(void) { return LockOwner.Get_Ptr(); }
#ifndef TTLE_EXPORTS
	static void	Set_Default_Driver_Is_Gunner(bool flag)		{DefaultDriverIsGunner = flag;}
#endif
	bool	Get_Driver_Is_Gunner(void)			{return DriverIsGunner;}
	void	Toggle_Driver_Is_Gunner(void)		{ DriverIsGunner = !DriverIsGunner; }
	virtual float		Get_Stealth_Fade_Distance(void) const;
	virtual float		Get_Filter_Distance(void) const;
	void					Ignore_Occupants( void );
	void					Unignore_Occupants( void );
	SoldierGameObj *Get_Occupant(int seat) {return SeatOccupants[seat];}
	void Set_Is_Scripts_Visible(bool b) {visible = b;}
	bool Get_Is_Scripts_Visible() {return visible;}
	void Set_Scripts_Last_Team(char c) {LastTeam = c;}
	void Set_Scripts_Can_Fire(bool b) {CanFire = b;Set_Object_Dirty_Bit(NetworkObjectClass::BIT_FREQUENT,true);}
	bool Get_Scripts_Can_Fire() { return CanFire; };
	void Set_Allow_Stealth_While_Empty(bool onoff) { AllowStealthWhileEmpty = onoff; Set_Object_Dirty_Bit(BIT_RARE, true); };
	bool Get_Allow_Stealth_While_Empty() const { return AllowStealthWhileEmpty; };
	void Damage_Meshes_Update() { DamageMeshesUpdate = true; DamageMeshesNetworkUpdate = true; Set_Object_Dirty_Bit(NetworkObjectClass::BIT_FREQUENT, true); }
	void Set_Occupant(int seat, SoldierGameObj *occupant) {SeatOccupants[seat] = occupant;}
	bool Is_Underground() { return Peek_Physical_Object()->Get_Collision_Group() == UNDERGROUND_COLLISION_GROUP; }
	SCRIPTS_API void Set_Immovable(bool b);
	bool Is_Immovable() {return Peek_Physical_Object()->Is_Immovable();}
	void Set_Lock_Team(int team) {LockTeam = (char)team;Set_Object_Dirty_Bit(BIT_RARE,true);} //0 = nod, 1 = gdi, 2 = both
	int Get_Lock_Team() {return LockTeam;}
	void Set_Can_Be_Stolen(bool onoff) { CanBeStolen = onoff; Set_Object_Dirty_Bit(BIT_RARE, true); }
	bool Can_Be_Stolen() const { return CanBeStolen; }
	void Set_Owner(SoldierGameObj *obj) {Owner = obj;}
	void Set_Can_Drive(bool onoff) {CanDrive = onoff; Set_Object_Dirty_Bit(BIT_RARE, true);}
	bool Can_Drive() {return CanDrive;}
#ifndef TTLE_EXPORTS
	void Set_Color(Vector3 &color) {if (UndergroundEffect){UndergroundEffect->Set_Color(color);Set_Object_Dirty_Bit( NetworkObjectClass::BIT_RARE, true );}}
#endif
	SoldierGameObj *Get_Owner()
	{
		if (Owner.Get_Ptr())
		{
			return Owner.Get_Ptr()->As_SoldierGameObj();
		}
		else
		{
			return NULL;
		}
	}
	bool Scripts_Is_Enemy(DamageableGameObj *obj)
	{
		bool enemy = false;
		if (obj != this && obj != NULL)
		{
			int sTeamID1 = LastTeam;
			if (LastTeam == -2)
			{
				sTeamID1 = Get_Player_Type();
			}
			int sTeamID2 = obj->Get_Player_Type();
			if ( sTeamID1 != -2 && sTeamID2 != -2 && sTeamID1 != -4 && sTeamID2 != -4 )
			{
				if ( sTeamID1 != -1 && sTeamID2 != -1 )
				{
					enemy = sTeamID1 != sTeamID2;
				}
				else
				{
					enemy = true;
				}
			}
			else
			{
				enemy = false;
			}
		}
		return enemy;
	}
	bool Scripts_Is_Teammate(const DamageableGameObj* oObject);
	bool Scripts_Is_Team_Player()
	{
		int team = LastTeam;
		if (LastTeam == -2)
		{
			team = Get_Player_Type();
		}
		return ((team <= 1) && (team >= 0));
	}
	bool Are_Transitions_Enabled() { //DA
		return TransitionsEnabled;
	}
	int Get_Last_Team() { //DA
		return LastTeam;
	}
#ifndef TTLE_EXPORTS
	static void	Set_Camera_Locked_To_Turret(bool flag)		{CameraLockedToTurret = flag;}
	static bool	Get_Camera_Locked_To_Turret(void)			{return CameraLockedToTurret;}
#endif
protected:
#ifndef TTLE_EXPORTS
	static REF_DECL(bool, DefaultDriverIsGunner);
	static REF_DECL(bool, CameraLockedToTurret);
#endif
	Sound3DClass			*Sound; //2416
	int						EngineSoundState; //2420
	AudibleSoundClass		*CachedEngineSound; //2424
	int						TurretBone; //2428
	int						BarrelBone; //2432
	float						TurretTurn; //2436
	float						BarrelTilt; //2440
	float						BarrelOffset; //2444
	bool						TransitionsEnabled; //2448
	bool						HasEnterTransitions; //2449
	bool						HasExitTransitions; //2450
	bool						VehicleDelivered; //2451
	bool						DriverIsGunner; //2452
	bool                        visible; //2453
	char                        LastTeam; //2454
	bool						DamageMeshesUpdate; //2455
	VectorClass<PersistantSurfaceEmitterClass *>	WheelSurfaceEmitters; //2456
	PersistantSurfaceSoundClass *						WheelSurfaceSound; //2472
	enum {
		DRIVER_SEAT = 0,
		GUNNER_SEAT,
	};
	VectorClass<SoldierGameObj *>	SeatOccupants; //2476
	int		OccupiedSeats; //2492
	DynamicVectorClass<TransitionInstanceClass *> TransitionInstances; //2496
	ReferencerClass		LockOwner; //2520
	float						LockTimer; //2536
	bool				CanFire;
	bool				AllowStealthWhileEmpty;
	char                LockTeam;
	bool				CanBeStolen;
	UndergroundEffectClass *UndergroundEffect;
	ReferencerClass     Owner; //Owner for SSGM vehicle locking. When this is set, only this player can enter the vehicle.
	bool HasUpdatedTargeting;
	bool CanDrive; //used to identify if it should apply analog control or not, for EMP purposes
	bool DamageMeshesNetworkUpdate;
	bool FixedTurretFacing;
	bool LockedTurretFacing;
	float DesiredTurretTurn;
	float DesiredBarrelTilt;

	void		Remove_Transitions( TransitionDataClass::StyleType transition_type );
	void		Create_New_Transitions( TransitionDataClass::StyleType transition_type );
	void		Destroy_Transitions( void );
	void		Update_Transitions( void );
	void		Create_And_Destroy_Transitions( void );
public:
	void		Aquire_Turret_Bones( void );
	void		Release_Turret_Bones( void );
	void        Reset_Sound_Effects( void );
	void		Update_Turret( float weapon_turn, float weapon_tilt );
protected:
	void		Update_Sound_Effects( void );
	void		Change_Engine_Sound_State( int new_state );
	void		Update_Engine_Sound_Pitch( void );
	virtual bool Is_Visible();
public:
	virtual int		Check_If_On_Surface(int surface_type);

	void VehicleGameObj::Set_Fixed_Turret_Facing(bool fixed)
	{
		if(!LockedTurretFacing)
		{
			FixedTurretFacing = fixed;
		}
	}

	void VehicleGameObj::Set_Locked_Turret_Facing(bool locked)
	{
		LockedTurretFacing = locked;
	}

	bool VehicleGameObj::Get_Fixed_Turret_Facing()
	{
		return FixedTurretFacing;
	}

	bool VehicleGameObj::Get_Locked_Turret_Facing()
	{
		return LockedTurretFacing;
	}

	void VehicleGameObj::Set_Desired_Turret_Turn(float angle)
	{
		DesiredTurretTurn = angle;
	}

	void VehicleGameObj::Set_Desired_Barrel_Tilt(float angle)
	{
		DesiredBarrelTilt = angle;
	}

	float VehicleGameObj::Get_Desired_Turret_Turn()
	{
		return DesiredTurretTurn;
	}

	float VehicleGameObj::Get_Desired_Barrel_Tilt()
	{
		return DesiredBarrelTilt;
	}

	float VehicleGameObj::Get_Turret_Turn()
	{
		return TurretTurn;
	}

	float VehicleGameObj::Get_Barrel_Tilt()
	{
		return BarrelTilt;
	}

	void VehicleGameObj::Set_Turret_Turn(float turn)
	{
		TurretTurn=turn;
	}

	void VehicleGameObj::Set_Barrel_Tilt(float tilt)
	{
		BarrelTilt=tilt;
	}

}; // size: 2541
#endif
