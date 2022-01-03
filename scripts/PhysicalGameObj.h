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
#ifndef TT_INCLUDE_PHYSICALGAMEOBJ_H
#define TT_INCLUDE_PHYSICALGAMEOBJ_H
#include "DamageableGameObj.h"
#include "PhysObserverClass.h"
#include "PhysicalGameObjDef.h"
#include "PhysClass.h"
#include "AnimControlClass.h"
class ActiveConversationClass;
class PowerUpGameObj;
class C4GameObj;
class BeaconGameObj;
class ArmedGameObj;
class CinematicGameObj;
class SimpleGameObj;
class Matrix3D;
class RenderObjClass;
class PhysicalGameObj : public DamageableGameObj, public CombatPhysObserverClass {
public:
	PhysicalGameObj( void );
	virtual	~PhysicalGameObj( void );
	void	Init( const PhysicalGameObjDef & definition );
	void	Copy_Settings( const PhysicalGameObjDef & definition );
	void	Re_Init( const PhysicalGameObjDef & definition );
	const PhysicalGameObjDef SCRIPTS_API & Get_Definition( void ) const ;
	virtual	bool	Save( ChunkSaveClass & csave );
	virtual	bool	Load( ChunkLoadClass & cload );
	virtual	void	On_Post_Load(void);
	virtual	void	Startup( void )	{}	
	inline PhysClass *	Peek_Physical_Object( void ) const { return PhysObj; }
	void					Attach_To_Object_Bone( PhysicalGameObj * host, const char * bone_name );
	bool					Is_Attached_To_An_Object( void ) { return (HostGameObj.Get_Ptr () != NULL); }
	void					Teleport_To_Host_Bone( void );
	SCRIPTS_API void		Set_Transform(const Matrix3D & tm);
	SCRIPTS_API const Matrix3D &	Get_Transform(void) const;
	virtual	void		Get_Position(Vector3 * set_pos) const;
	SCRIPTS_API Vector3 Get_Position() { Vector3 Return; Peek_Physical_Object()->Get_Position(&Return); return Return; }
	SCRIPTS_API void	Set_Position(const Vector3 & pos);
	SCRIPTS_API float	Get_Facing(void) const;
	inline RenderObjClass	*	Peek_Model( void ) { return Peek_Physical_Object()->Peek_Model(); }
	SCRIPTS_API AnimControlClass*	Get_Anim_Control( void );
	void					Set_Anim_Control( AnimControlClass * anim_control );
	virtual	void	Set_Animation( const char *animation_name, bool looping = true, float frame_offset = 0.0f );
	virtual	void	Set_Animation_Frame ( const char *animation_name, int frame );
	float		Get_Bullseye_Offset_Z( void ) const 		{ return Get_Definition().BullseyeOffsetZ; }
	virtual	Vector3	Get_Bullseye_Position( void );
	virtual	void	Get_Information( StringClass & string );
	virtual	void	Apply_Damage( const OffenseObjectClass & damager, float scale = 1.0f, int alternate_skin = -1 );
	virtual	void	Apply_Damage_Extended( const OffenseObjectClass & offense, float scale = 1.0f,
			const	Vector3 & direction = Vector3( 0,0,0 ), const char * collision_box_name = NULL );
	virtual	void	Completely_Damaged( const OffenseObjectClass & damager );
	bool	Is_Soft( void );
	virtual	bool	Takes_Explosion_Damage( void )						{ return true; }
	int	Get_Type()	const		{ return Get_Definition().Type; }
	virtual	void	Post_Think();
	void		SCRIPTS_API Set_Collision_Group( int group );
	Collision_Group_Type Get_Collision_Group() const { return PhysObj->Get_Collision_Group(); }
	virtual ExpirationReactionType	Object_Expired( PhysClass * observed_obj ) { Set_Delete_Pending(); return 1; }
	virtual	PhysicalGameObj		*As_PhysicalGameObj( void )	{ return this; }
	virtual	DamageableGameObj		*As_DamageableGameObj( void )	{ return this; }		// Re-implement for CombatPhysObserverClass
	virtual	SoldierGameObj			*As_SoldierGameObj( void )		{ return (SoldierGameObj *)NULL; }
	virtual	PowerUpGameObj			*As_PowerUpGameObj( void )		{ return (PowerUpGameObj *)NULL; }
	virtual  VehicleGameObj		   *As_VehicleGameObj( void )		{ return (VehicleGameObj *)NULL; }
	virtual  C4GameObj		      *As_C4GameObj( void )	      { return (C4GameObj *)NULL; }
	virtual	BeaconGameObj			*As_BeaconGameObj (void)		{ return (BeaconGameObj *)NULL; }
	virtual	ArmedGameObj			*As_ArmedGameObj( void )		{ return (ArmedGameObj *)NULL; }
	virtual	CinematicGameObj		*As_CinematicGameObj( void )	{ return NULL; }
	virtual	SimpleGameObj			*As_SimpleGameObj( void )		{ return NULL; }
	BYTE		Get_Server_Skips(void) {return ServerUpdateSkips;}
	void		Reset_Server_Skips(BYTE value);
	void		Increment_Server_Skips(void);
	virtual	bool	Is_Hibernating( void )					{ return HibernationTimer <= 0; }
	void				Enable_Hibernation( bool enable )	{ HibernationEnable = enable; if ( Is_Hibernating() ) HibernationTimer = 1;}
	void				Reset_Hibernating( void );
	void				Do_Not_Hibernate( void )				{ if ( HibernationTimer < 1 ) HibernationTimer = 1; }
	virtual	void	Begin_Hibernation( void );
	virtual	void	End_Hibernation( void );
	int				Get_Radar_Blip_Shape_Type( void )				{ return RadarBlipShapeType; }
	void				Set_Radar_Blip_Shape_Type( int type )			{ RadarBlipShapeType = type; Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);}
	void				Reset_Radar_Blip_Shape_Type( void )				{ RadarBlipShapeType = Get_Definition().RadarBlipType; Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);}
	int				Get_Radar_Blip_Color_Type( void )				{ return RadarBlipColorType; }
	void				Set_Radar_Blip_Color_Type( int type )			{ RadarBlipColorType = type; Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);}
	void				Reset_Radar_Blip_Color_Type( void );
	float				Get_Radar_Blip_Intensity( void )					{ return RadarBlipIntensity; }
	void				Set_Radar_Blip_Intensity( float value )		{ RadarBlipIntensity = value; }
	virtual void	Export_Creation( BitStreamClass &packet );
	virtual void	Import_Creation( BitStreamClass &packet );
	virtual void	Export_Rare( BitStreamClass &packet );
	virtual void	Import_Rare( BitStreamClass &packet );
	virtual void	Export_Frequent( BitStreamClass &packet );
	virtual void	Import_Frequent( BitStreamClass &packet );
	virtual int		Get_Vis_ID(void);
	virtual bool	Get_World_Position (Vector3 &pos) const		{ Get_Position (&pos); return true; }
	bool				Are_Innate_Conversations_Enabled( void )				{ return Get_Definition().AllowInnateConversations && IsInnateConversationsEnabled; }
	void				Enable_Innate_Conversations( bool enable )				{ IsInnateConversationsEnabled = enable; }
	bool				Is_In_Conversation( void ) const							{ return ActiveConversation != NULL; }
	void				Set_Conversation( ActiveConversationClass *conversation );
	void				Hide_Muzzle_Flashes( bool hide = true );
	void				Enable_HUD_Pokable_Indicator( bool enable );
	bool				Is_HUD_Pokable_Indicator_Enabled( void )		{ return HUDPokableIndicatorEnabled; } 
	virtual void		Set_Player_Type(int type);
	virtual void	Object_Shattered_Something(PhysClass * observed_obj, PhysClass * shattered_obj, int surface_type);
	void                Clear_Animation() {if (AnimControl) {AnimControl->Set_Animation((const char*)NULL, 0, 0);AnimControl->Set_Mode(ANIM_MODE_STOP,0);ClearAnimation = true;Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);}}
	void                Set_Team_Visibility(TeamVisibilitySetting setting) {TeamVisibility = (char)setting;Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);}
	void                Set_Player_Visible(bool visible) {PlayerVisible = visible;}
	GameObject *		Get_Parent_Object(){return HostGameObj;}
protected:
	ActiveConversationClass *ActiveConversation;
private:
	PhysClass *				PhysObj;
	AnimControlClass *	AnimControl;
	BYTE						ServerUpdateSkips;
	float						HibernationTimer;
	bool						HibernationEnable;
	char                        TeamVisibility;
	ReferencerClass		HostGameObj;
	int						HostGameObjBone;
	int						RadarBlipShapeType;
	int						RadarBlipColorType;
	float						RadarBlipIntensity;
	int						PendingHostObjID;
	bool					HUDPokableIndicatorEnabled;
	bool					IsInnateConversationsEnabled;
	bool					ClearAnimation;
	bool                    PlayerVisible;
}; // 07B0

#endif