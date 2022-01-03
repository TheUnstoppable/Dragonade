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
#ifndef TT_INCLUDE_WEAPONCLASS_H
#define TT_INCLUDE_WEAPONCLASS_H
#include "WeaponMgr.h"
#include "MuzzleFlashClass.h"
#include "ReferencerClass.h"
#include "Matrix3D.h"
class ArmedGameObj;
class ParticleEmitterClass;
class AudibleSoundClass;
class PhysicalGameObj;
typedef	enum {
	WEAPON_HOLD_STYLE_C4					= 0,
	WEAPON_HOLD_STYLE_NOT_USED,
	WEAPON_HOLD_STYLE_AT_SHOULDER,	// 2
	WEAPON_HOLD_STYLE_AT_HIP,
	WEAPON_HOLD_STYLE_LAUNCHER,
	WEAPON_HOLD_STYLE_HANDGUN,
	WEAPON_HOLD_STYLE_BEACON,
	WEAPON_HOLD_STYLE_EMPTY_HANDS,
	WEAPON_HOLD_STYLE_AT_CHEST,
	WEAPON_HOLD_STYLE_HANDS_DOWN,
	NUM_WEAPON_HOLD_STYLES
} WeaponHoldStyleType;

typedef	enum {
	WEAPON_ANIM_NOT_FIRING,
	WEAPON_ANIM_FIRING_0,
	WEAPON_ANIM_FIRING_1
} WeaponAnimState;

typedef	enum {
	WEAPON_MODEL_UPDATE_WILL_BE_NEEDED,
	WEAPON_MODEL_UPDATE_IS_NEEDED,
	WEAPON_MODEL_UPDATE_NOT_NEEDED,
} WeaponModelUpdateState;

class WeaponClass
{

public:

	enum {
		MUZZLE_PRIMARY_0,
		MUZZLE_PRIMARY_1,
		MUZZLE_SECONDARY_0,
		MUZZLE_SECONDARY_1,
	};

	typedef	enum {
		STATE_IDLE,
		STATE_READY,
		STATE_CHARGE,
		STATE_FIRE_PRIMARY,
		STATE_FIRE_SECONDARY,
		STATE_RELOAD,
		STATE_START_SWITCH,
		STATE_END_SWITCH,
	} WeaponStateType;

	WeaponClass( const WeaponDefinitionClass *weapon_def = NULL );
	~WeaponClass( void );
	void				Init( const WeaponDefinitionClass *weapon_def );
	bool				Save( ChunkSaveClass & csave );
	bool				Load( ChunkLoadClass & cload );
	void				Set_Model( RenderObjClass *model );
	void				Set_Owner( ArmedGameObj *owner );
	ArmedGameObj	*Get_Owner( void )				{ return (ArmedGameObj *)Owner.Get_Ptr();	}
	void				Select( void );
	void				Deselect( void );
	void				Update( void );
	bool				Is_Muzzle_Clear( void );
	void				Compute_Bullet_Start_Point(const Matrix3D & muzzle,Vector3 * set_start_point);
	void				Next_C4_Detonation_Mode( void );			// Take the weapon to the next state (for C4)
	const WeaponDefinitionClass *Get_Definition( void )		{ return Definition; }
	const char * 	Get_Name( void )		  		{ return Definition->Get_Name(); }
	int				Get_ID( void )		  			{ return Definition->Get_ID(); }
	const char *	Get_HUD_Icon_Texture_Name( void )	{ return Definition->HUDIconTextureName; }
	const char *	Get_Model_Name( void ) 		{ return Definition->Model; }
	const char *	Get_Back_Model_Name( void ){ return Definition->BackModel; }
	const char *	Get_Anim_Name( void )  		{ return (NextAnimState != WEAPON_ANIM_NOT_FIRING) ? Definition->FireAnim : Definition->IdleAnim; }
	const char *	Get_First_Person_Anim_Name( void ) const { return Definition->FirstPersonAnim; }
	int				Get_Style( void )		  		{ return Definition->Style; }
	float			Get_Key_Number( void )		{ return Definition->KeyNumber; }
	bool				Get_Can_Snipe( void )		{ return Definition->CanSnipe; }
	float				Get_Rating( void )		  	{ return Definition->Rating; }
	float				Get_Effective_Range( void ){ return PrimaryAmmoDefinition->EffectiveRange; }
	float				Get_Primary_Fire_Rate( void ){ return PrimaryAmmoDefinition->RateOfFire; }
	const char * 	Get_First_Person_Model_Name( void )	const		{ return Definition->FirstPersonModel; }
	const Vector3& Get_First_Person_Model_Offset( void ) const	{ return Definition->FirstPersonOffset; }
	float				Get_Recoil_Time( void )		{ return Definition->RecoilTime; }
	float				Get_Recoil_Scale( void )	{ return Definition->RecoilScale; }
	void				Set_Total_Rounds( int num );
	SCRIPTS_API int				Get_Total_Rounds( void );
	void Import_Ammo_Info(int16 ammoInfo);
	int16 Export_Ammo_Info();
	void				Add_Rounds( int num );
	void				Add_Rounds_Client( int num );
	void				Set_Clip_Rounds( int rounds );
	void				Set_Inventory_Rounds( int rounds );
	void				Set_Clip_Rounds_Client( int rounds );
	void				Set_Inventory_Rounds_Client( int rounds );
	int				Get_Clip_Rounds( void )					{ return ClipRounds; }
	int				Get_Inventory_Rounds( void )			{ return InventoryRounds; }
	void				Do_Reload( void );
	void				Decrement_Rounds( int rounds = 1 );
	SCRIPTS_API bool				Is_Ammo_Maxed( void );
	bool				Is_Loaded( void )							{ return (ClipRounds != 0); }
	bool				Is_Reload_OK( void );
	bool				Is_Reload_Needed( void )				{ return !Is_Loaded() && Is_Reload_OK(); }
	SCRIPTS_API float	Get_Range( void );
	bool				Is_Model_Update_Needed( void )	{ return (UpdateModel == WEAPON_MODEL_UPDATE_IS_NEEDED); }
	void				Reset_Model_Update( void )			{ UpdateModel = WEAPON_MODEL_UPDATE_NOT_NEEDED; CurrentAnimState = (WeaponAnimState)-1; }
	bool				Is_Anim_Update_Needed( void )		{ return ( NextAnimState != CurrentAnimState ); }
	void				Reset_Anim_Update( void )			{ CurrentAnimState = NextAnimState; }
	WeaponAnimState	Get_Anim_State( void )			{ return CurrentAnimState; }
	int				Get_Total_Rounds_Fired( void )	{ return TotalRoundsFired; }
	void				Display_Targeting( void );
	void				Set_Target( const Vector3 &target)	{ Target = target; }
	const Vector3 &Get_Target( void )						{ return Target; }
	void				Set_Target_Object( PhysicalGameObj * obj );
	PhysicalGameObj *	Get_Target_Object( PhysicalGameObj * obj );
	WeaponStateType	Get_State( void )					{ return State; }
	bool				Is_Firing( void )						{ return DidFire; }
	bool				Is_Reloading( void )					{ return (State == STATE_RELOAD); }
	bool				Is_Switching( void )					{ return (State == STATE_START_SWITCH) || (State == STATE_END_SWITCH); }
	void				Force_Reload( void );
	void				Set_Safety( bool saftey )			{ SafetySet = saftey; }
	bool				Is_Safety_Set( void )				{ return SafetySet; }
	void				Set_Primary_Triggered( bool triggered );
	void				Set_Secondary_Triggered( bool triggered );
	bool				Is_Triggered( void )	{ return IsSecondaryTriggered || IsPrimaryTriggered; }
	void				Init_Muzzle_Flash( RenderObjClass * robj )	{ MuzzleFlash.Init( robj ); }
	void				Update_Muzzle_Flash( bool flashA0, bool flashA1, bool flashB0, bool flashB1 ) 	{ MuzzleFlash.Update( flashA0, flashA1, flashB0, flashB1 ); }
	bool				Does_Weapon_Exist( void )				{ return WeaponExists; }
	void				Set_Weapon_Exists( bool exists )		{ WeaponExists = exists; }
	PhysicalGameObj * Cast_Weapon( const Vector3 & target );
	bool				Cast_Weapon_Down_Muzzle( Vector3 & hit_pos );
	void				Make_Shell_Eject( const Matrix3D & tm );
	void				Stop_Firing_Sound( void );
	void				Clear_Firing_Sound() {FiringSound = 0;}
	void				Set_Did_Fire(bool fire) {DidFire = fire;}
    void				Set_State( WeaponStateType new_state );

	//new
	SCRIPTS_API float	Get_Targeting_Range(void);
	bool				Tilt_Gun_While_Reloading(){return tiltGunWhileReloading && Definition->DoTiltWhileReloading;}
private:
	void				Fire_C4( const AmmoDefinitionClass *ammo_def );
	bool				Fire_Beacon( const AmmoDefinitionClass *ammo_def );
	void				Fire_Bullet( const AmmoDefinitionClass *ammo_def, bool primary );
	void				clientReloadingSync();
	void				cleintReloadAmmoSync();
	void				sendReloadNetworkCall(int reloadingObjectId,int originatingClientId);
	void				sendClientAmmoAcrossNetwork(int originalPlayerId,int reloadingObjectId,int weaponId,int clipRounds,int backpackRounds);

	const WeaponDefinitionClass* Definition; // 0000
	ReferencerClass Owner; // 0004
	RenderObjClass *Model; // 0014
	int C4DetonationMode; // 0018
public:
	const AmmoDefinitionClass *PrimaryAmmoDefinition; // 001C
	const AmmoDefinitionClass *SecondaryAmmoDefinition; // 0020
private:
	safe_int ClipRounds; // 0024
	safe_int InventoryRounds; // 0028
	safe_float BurstDelayTimer; // 002C
	safe_int BurstCount; // 0030
	float BulletBumpTime; // 0034
	bool WeaponExists; // 0038
	WeaponStateType State; // 003C
	float StateTimer; // 0040
	bool DidFire; // 0044
	bool LastFrameIsPrimaryTriggered; // 0045
	bool LastFrameIsSecondaryTriggered; // 0046
	bool IsPrimaryTriggered; // 0047
	bool IsSecondaryTriggered; // 0048
	int TotalRoundsFired; // 004C
	bool SafetySet; // 0050
	bool LockTriggers; // 0051
	float EmptySoundTimer; // 0054
	WeaponAnimState NextAnimState; // 0058
	WeaponAnimState CurrentAnimState; // 005C
	WeaponModelUpdateState UpdateModel; // 0060
	Vector3 Target; // 0064
	ReferencerClass TargetObject; // 0070
	SimpleVecClass<ParticleEmitterClass*> ContinuousEmitters; // 0080
	AudibleSoundClass *ContinuousSound; // 008C
	AudibleSoundClass *FiringSound; // 0090
	int FiringSoundDefID; // 0094
	const Matrix3D & Get_Muzzle(int index = MUZZLE_PRIMARY_0);
	void Do_Fire(bool primary);
	void				Update_State( float dtime );
	void				Do_Firing_Effects( void );
	void				Do_Continuous_Effects( bool enable );
	MuzzleFlashClass MuzzleFlash; // 0098
	void				Ignore_Owner( void );
	void				Unignore_Owner( void );
	AudibleSoundClass *GlobalFiringSound;
	bool				tiltGunWhileReloading;
public:
	static void			Set_Ammo_By_WeaponId_No_Networking(GameObject *obj,int weaponId,int clipBullets,int inventoryRounds);
}; // 00AC

#endif