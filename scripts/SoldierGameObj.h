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
#ifndef TT_INCLUDE__SOLDIERGAMEOBJ_H
#define TT_INCLUDE__SOLDIERGAMEOBJ_H

#include "DialogueClass.h"
#include "HumanStateClass.h"
#include "SmartGameObj.h"
#include "PhysClass.h"
#include "ArmorWarheadManager.h"
#include "TransitionGameObjDef.h"
#include "SoldierGameObjDef.h" //DA
#include "cPlayer.h" //DA
class TransitionInstanceClass;
class RenderObjClass;
class SimpleAnimControlClass;
class TransitionCompletionDataStruct
{
public:
	TransitionDataClass::StyleType Type;
	ReferencerClass Vehicle;
	bool Save(ChunkSaveClass &csave);
	bool Load(ChunkLoadClass &cload);
};
class TransitionEffectClass;
class SoldierObserverClass;
class DynamicSpeechAnimClass;
class PersistantSurfaceEmitterClass;
class SoldierGameObjDef;
class Phys3Class;
class AudibleSoundClass;
class SoldierGameObj : public SmartGameObj
{
public:
	SoldierGameObj();
	virtual	~SoldierGameObj();
	virtual	void	Init( void );
	void	Init( const SoldierGameObjDef & definition );
	void	Copy_Settings( const SoldierGameObjDef & definition );
	void	SCRIPTS_API Re_Init( const SoldierGameObjDef & definition );
	SCRIPTS_API const SoldierGameObjDef & Get_Definition( void ) const ;
	virtual	bool	Save( ChunkSaveClass & csave );
	virtual	bool	Load( ChunkLoadClass & cload );
	virtual	void	On_Post_Load( void );
	virtual	const	PersistFactoryClass & Get_Factory( void ) const;
	HumanPhysClass	*	Peek_Human_Phys( void ) const;
	virtual	void	Think( void );
	virtual	void	Post_Think( void );
	virtual	void	Set_Control_Owner(int control_owner);
	virtual	void	Generate_Control( void );
	virtual	void	Apply_Control( void );
	virtual	void	Apply_Damage( const OffenseObjectClass & damager, float scale = 1.0f, int alternate_skin = -1 );
  virtual	void	Apply_Damage_Extended( const OffenseObjectClass & offense, float scale = 1.0f,
			const	Vector3 & direction = Vector3( 0,0,0 ), const char * collision_box_name = NULL );
  virtual void	Set_Delete_Pending (void);

  /*! A clone of Apply_Damage() which ignores whether the soldier is currently in a vehicle - this
  * is necessary for dp88_remoteControlConsole to kill the driver of a remote control vehicle when
  * the 'dummy' version of the driver is killed. */
  virtual void Apply_Damage_IgnoreVehicleCheck( const OffenseObjectClass & damager, float scale = 1.0f, int alternate_skin = -1 );

	virtual	void	Completely_Damaged( const OffenseObjectClass & damager ) {}
	virtual CollisionReactionType	Collision_Occurred( const CollisionEventClass & event );
	virtual	Vector3	Get_Bullseye_Position( void );
	virtual bool	Is_Turreted( void )					{ return false; }
	virtual bool	Set_Targeting( const Vector3 & pos, bool do_tilt = true  );
	float		Get_Weapon_Height( void );
	float		Get_Weapon_Length( void );
	virtual const Matrix3D &Get_Muzzle( int index = 0 );
	bool		Detonate_C4( void )							{ return DetonateC4; }
	virtual void Set_Weapon_Model( const char *model_name );
	virtual void Set_Weapon_Animation( const char *anim_name );
	virtual void	Start_Transition_Animation( const char * anim_name, TransitionCompletionDataStruct * completion_data );
	virtual	void	Set_Animation( const char *animation_name, bool looping = true, float frame_offset = 0.0f );
	virtual	void	Set_Blended_Animation( const char *animation_name, bool looping = true, float frame_offset = 0.0f, bool play_backwards = false );
	void	Handle_Legs( void );
	void	Exit_Ladder( void );
	void	Enter_Ladder( bool top );
   virtual	void	Import_State_Cs( BitStreamClass & packet );
   virtual	void	Export_State_Cs( BitStreamClass & packet );
	void				Interpret_Sc_Position_Data(const Vector3 & sc_position);
	void				Interpret_Sc_State_Data(HumanStateClass::HumanStateType state, 
							int sub_state, LPCSTR trans_name, const Vector3 & velocity, const Vector3 & sc_position);
	int				Tally_Vis_Visible_Soldiers(void);
	SCRIPTS_API bool				Is_In_Elevator(void);
	virtual void	Export_Creation( BitStreamClass &packet );
	virtual void	Import_Creation( BitStreamClass &packet );
	virtual void	Export_Rare( BitStreamClass &packet );
	virtual void	Import_Rare( BitStreamClass &packet );
	virtual void	Export_Occasional( BitStreamClass &packet );
	virtual void	Import_Occasional( BitStreamClass &packet );
   virtual void	Export_Frequent( BitStreamClass & packet );
   virtual void	Import_Frequent( BitStreamClass & packet );
	bool				Is_Dead( void )			{ return Get_State() == HumanStateClass::DEATH; }
	bool				Is_Destroyed( void )		{ return Get_State() == HumanStateClass::DESTROY; }
	bool				Is_Upright( void )		{ return Get_State() == HumanStateClass::UPRIGHT; }
	bool				Is_Wounded( void )		{ return Get_State() == HumanStateClass::WOUNDED; }
	bool				In_Transition( void )	{ return Get_State() == HumanStateClass::TRANSITION; }
	bool				Is_Airborne( void )		{ return Get_State() == HumanStateClass::AIRBORNE; }
	bool				Is_Crouched( void )		{ return HumanState.Get_State_Flag( HumanStateClass::CROUCHED_FLAG ); }
	bool				Is_Sniping( void )		{ return HumanState.Get_State_Flag( HumanStateClass::SNIPING_FLAG ); }
	void				Set_Is_Sniping()        { HumanState.Toggle_State_Flag(HumanStateClass::SNIPING_FLAG); Set_Object_Dirty_Bit(BIT_OCCASIONAL, true); }
	bool				Is_Slow( void )			{ return (Get_Sub_State() & HumanStateClass::SUB_STATE_SLOW) != 0; }
	bool				Is_On_Ladder( void )		{ return Get_State() == HumanStateClass::LADDER; }
	bool				Is_In_Vehicle( void )	{ return Get_State() == HumanStateClass::IN_VEHICLE; }
	void				Reset_Loiter_Delay( void )				{ HumanState.Reset_Loiter_Delay(); }
	virtual	void	Get_Information( StringClass & string );
	virtual void	Get_Description(StringClass & description);
	SCRIPTS_API void		Toggle_Fly_Mode(void); //DA
	virtual float		Get_Max_Speed( void );
	virtual void 		Set_Max_Speed( float speed );
	virtual float		Get_Turn_Rate( void );
	void					Enter_Vehicle( VehicleGameObj * vehicle, const char * anim_name );
	void					Exit_Vehicle( void );
	void					Exit_Destroyed_Vehicle( int seat_num, const Vector3 & vehicle_pos );
	bool					Is_Permitted_To_Enter_Vehicle( void );
	VehicleGameObj	*	Get_Vehicle( void )			{ return Vehicle; }
	VehicleGameObj	*	Get_Profile_Vehicle( void );
	bool					Use_Ladder_View( void );
	StringClass			Get_Animation_Name( void )		{ return AnimationName; }
	HumanStateClass	* Get_Human_State( void )			{ return &HumanState; }
	void					Set_Model(const char *model_name);
	virtual SoldierGameObj		* As_SoldierGameObj()		{ return this; };
	void              Get_Velocity(Vector3 & vel);
   void              Set_Velocity(Vector3 & vel);
   void              Give_All_Weapons(void);
	bool					Can_See(SoldierGameObj * p_soldier);
	void					Adjust_Skeleton( float height, float width );
	void					Look_At( const Vector3 & pos, float time )	{ HeadLookTarget = pos; HeadLookDuration = time; HeadLookAngle = Vector3(0,0,0); }
	void					Update_Look_At( const Vector3 & pos )			{ HeadLookTarget = pos; }
	void					Cancel_Look_At( void )								{ HeadLookDuration = 0.001F; }
	void					Look_Random( float time );
	bool					Is_Looking( void )									{ return (HeadLookDuration > 0); }
	virtual Matrix3D	Get_Look_Transform(void);
	void					Lock_Facing( PhysicalGameObj * game_obj, bool turn_body );
	void					Innate_Enable( int bits = 0xFFFFFFFF )	{ InnateEnableBits |= bits; }
	void					Innate_Disable( int bits = 0xFFFFFFFF )	{ InnateEnableBits &= ~bits; }
	bool					Is_Innate_Enabled( int bits = 0xFFFFFFFF )	{ return (InnateEnableBits & bits) != 0; }
	void					Say_Dialogue( int dialog_id );
	static float		Say_Dynamic_Dialogue( int text_id, SoldierGameObj *speaker = NULL, AudibleSoundClass **sound_obj_to_return = NULL );
	void					Stop_Current_Speech( void );
	RenderObjClass *	Find_Head_Model( void );
	void					Prepare_Speech_Framework( void );
	void					Enable_Ghost_Collision( bool onoff );
	bool					Is_Soldier_Blocked( const Vector3 &curr_pos );
	bool					Disable_Ghost_Collision();
	DynamicSpeechAnimClass *Get_Facial_Anim (void) { return SpeechAnim; }
	void							Set_Emot_Icon (const char *model_name, float duration );
	SoldierObserverClass	*	Get_Innate_Controller( void );
	SoldierAIState		Get_AI_State( void ) const					{ return AIState; }
	void					Set_AI_State( SoldierAIState state );
	void							Set_Innate_Observer( SoldierObserverClass * observer )	{ InnateObserver = observer; }
	SoldierObserverClass *	Get_Innate_Observer( void )	{ return InnateObserver; }
	void							Clear_Innate_Observer( void )	{ InnateObserver = NULL; }
	const char * Get_First_Person_Hands_Model_Name( void );
	void					Perturb_Position(float max_perturb = 5);
	int					Get_Key_Ring( void ) const		{ return KeyRing; }
	void					Give_Key( int key_number )		{ KeyRing |= (1 << key_number); }
	void					Remove_Key( int key_number )	{ KeyRing &= ~(1 << key_number); }
	bool					Has_Key( int key_number )		{ return ((1 << key_number) & KeyRing) != 0; }
	virtual bool		Wants_Powerups( void )
	{
		if (Get_Player_Type() == -4 || Is_Dead() || Is_Destroyed())
		{
			return false;
		}
		return Is_Human_Controlled();
	}
	virtual bool		Allow_Special_Damage_State_Lock( void )	{ return (Is_Human_Controlled() == false); }
	virtual	bool	Is_Visible( void )						{ return IsVisible && !Vehicle; }
				void	Set_Is_Visible( bool visible )		{ IsVisible = visible; }	
	virtual	bool		Is_Targetable( void ) const;
	virtual float		Get_Stealth_Fade_Distance(void) const;
	bool Get_Fly_Mode() {return InFlyMode;}
	HumanStateClass::HumanStateType 	Get_State( void )			{ return HumanState.Get_State(); }
	RenderObjClass *Get_Weapon_Render_Model() {return WeaponRenderModel; }
	void						Update_Back_Gun( void );
	const char *		Get_State_Name( void )		{ return HumanState.Get_State_Name(); }
	void Set_Can_Steal_Vehicles(bool onoff) { CanStealVehicles = onoff; Set_Object_Dirty_Bit(BIT_RARE, true); }
	bool Can_Steal_Vehicles() { return CanStealVehicles; }
	void Set_Can_Drive_Vehicles(bool onoff) { CanDriveVehicles = onoff; Set_Object_Dirty_Bit(BIT_RARE, true); }
	bool Can_Drive_Vehicles() { return CanDriveVehicles; }
	void Set_Block_Action_Key(bool onoff) { BlockActionKey = onoff; Set_Object_Dirty_Bit(BIT_RARE, true); }
	bool Block_Action_Key() { return BlockActionKey; }
	void Set_Freeze(bool onoff) {Freeze = onoff; Set_Object_Dirty_Bit(BIT_RARE, true);}
	bool Is_Frozen() {return Freeze;}
	void Lock_Collision_Mode(bool lockCollisionGroup,Collision_Group_Type lock);
	Collision_Group_Type *Get_Locked_Collision_Mode();
	void Set_Can_Play_Damage_Animations(bool onoff) {CanPlayDamageAnimations = onoff;Set_Object_Dirty_Bit(BIT_RARE, true);}
	bool Can_Play_Damage_Animations() {return CanPlayDamageAnimations;}
	void Set_Scale_Across_Network(float scale){NetworkRescale = scale;Set_Object_Dirty_Bit(BIT_RARE, true);}
	float Get_Scale_Across_Network(){return NetworkRescale;}
	void Set_Movement_Loiters_Allowed(bool allowed){movementLoitersAllowed = allowed;};
	bool Get_Movement_Loiter_Allowed(){return movementLoitersAllowed;};
	bool Get_Use_Stock_Ghost_Behavior(){return useStockGhostBehavior;}
	void Set_Override_Muzzle_Direction(bool override){OverrideMuzzleDirection = override; Set_Object_Dirty_Bit(BIT_RARE, true);};
	bool Get_Override_Muzzle_Direction(){return OverrideMuzzleDirection;};
	void Set_Bot_Tag(const char* tag) { botTag = tag; Set_Object_Dirty_Bit(BIT_RARE, true); }
	const WideStringClass& Get_Bot_Tag() { return botTag; }
	bool Is_Bot() { return botTag.Get_Length() > 0; }
	virtual int Get_Contact_Surface_Type();
	virtual float		Get_Skeleton_Heigth( void );
	virtual void 		Set_Skeleton_Height( float height );
	virtual float		Get_Skeleton_Width( void );
	virtual void 		Set_Skeleton_Width( float width );
	virtual void 		Set_Override_Weapon_Hold_Style(int overrideWeaponHoldStyleId);
	virtual int			Get_Override_Weapon_Hold_Style();
	virtual void 		Set_Human_Anim_Override(bool enableHumanAnimOverride);
	virtual int			Get_Human_Anim_Override();

	cPlayer* Get_Player() { //DA
		return (cPlayer*)Get_Player_Data();
	}
	DAPlayerClass* Get_DA_Player() { //DA
		return Get_Player()->Get_DA_Player();
	}
protected:
	RenderObjClass		*	WeaponRenderModel; //2416
	RenderObjClass		*	BackWeaponRenderModel; //2420
	RenderObjClass		*	BackFlagRenderModel; //2424
	AnimControlClass	*	WeaponAnimControl; //2428
	bool						DetonateC4; //2432
	TransitionCompletionDataStruct *	TransitionCompletionData; //2436
	StringClass				AnimationName; //2440
	VehicleGameObj		*	Vehicle; //2444
	HumanStateClass		HumanState; //2448
	float						LegFacing; //2576
	bool						SyncLegs; //2580
	bool						LastLegMode; //2581
	int						KeyRing; //2584
	bool						IsUsingGhostCollision; //2588
	DialogueClass			DialogList[DIALOG_MAX]; //2592
	AudibleSoundClass *	CurrentSpeech; //3232
	float						HeadLookDuration; //3236
	Vector3					HeadRotation; //3240
	Vector3					HeadLookTarget; //3252
	Vector3					HeadLookAngle; //3264
	float						HeadLookAngleTimer; //3276
	int										Get_Sub_State( void )	{ return HumanState.Get_Sub_State(); }
	void						Update_Locked_Facing( void );
	void						Set_Back_Weapon_Model( const char *model_name );
	void						Set_Back_Flag_Model( const char *model_name, const Vector3 & tint = Vector3(0, 0, 0) );
	int						Get_Ouch_Type( const	Vector3 & direction, const char * collision_box_name ) { return HumanState.Get_Ouch_Type( direction, collision_box_name ); }
	bool						Internal_Set_Targeting( const Vector3 & pos, bool do_tilt = true  );
	ArmorWarheadManager::SpecialDamageType			SpecialDamageMode; //3280
	float						SpecialDamageTimer; //3284
	ReferencerClass 			SpecialDamageDamager; //3288
	TransitionEffectClass * SpecialDamageEffect; //3304
	void						Set_Special_Damage_Mode( ArmorWarheadManager::SpecialDamageType mode, ArmedGameObj * damager = NULL );
	TransitionEffectClass * HealingEffect; //3308
	void						Handle_Head_look( void );
	ReferencerClass 		FacingObject; //3312
	bool						FacingAllowBodyTurn; //3328
	int						InnateEnableBits; //3332
	SoldierObserverClass *InnateObserver; //3336
	SoldierAIState			AIState; //3340
	DynamicSpeechAnimClass *SpeechAnim; //3344
	float						GenerateIdleFacialAnimTimer; //3348
	RenderObjClass *		HeadModel; //3352
	RenderObjClass *		EmotIconModel; //3356
	float						EmotIconTimer; //3360
	bool						InFlyMode; //3364
	bool						IsVisible; //3365
	bool						LadderUpMask; //3366
	bool						LadderDownMask; //3367
	float						ReloadingTilt; //3368
	bool						WeaponChanged; //3372
	bool						CanStealVehicles; // padding
	bool						CanDriveVehicles; // padding
	PersistantSurfaceEmitterClass * WaterWake; //3376
	DynamicVectorClass<RenderObjClass*>	RenderObjList; //3380
	bool                        BlockActionKey;
	bool Freeze; //used to detect that this soldier is frozen (cant fire, move, jump, climb ladders, etc)
	void						Add_RenderObj( RenderObjClass * obj );
	RenderObjClass *		Find_RenderObj( const char * name );
	void						Reset_RenderObjs( void );
	void						Update_Healing_Effect( void );
	int head_bone;
	int neck_bone;
	bool                        CanPlayDamageAnimations;
	float						NetworkRescale;
	float						LastScale;
	bool						movementLoitersAllowed;
	bool						useStockGhostBehavior;
	bool						OverrideMuzzleDirection;
	bool						UpdatedTarget;
	bool						DoTilt;
	float						skeletonHeight;
	float						skeletonWidth;
	float						lastSkeletonHeight;
	float						lastSkeletonWidth;
	int							overrideWeaponHoldStyleId;
	bool						enableHumanAnimOverride;
	WideStringClass				botTag;
	int sight_bone;
	TT_DEPRECATED("Do not use") int						Check(void);
}; // size: 3404


#endif
