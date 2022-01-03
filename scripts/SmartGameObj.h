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
#ifndef TT_INCLUDE__SMARTGAMEOBJ_H
#define TT_INCLUDE__SMARTGAMEOBJ_H

#include "scripts.h"
#include "ArmedGameObj.h"
#include "Action.h"
#include "ControlClass.h"
#include "PhysControllerClass.h"
#include "SmartGameObjDef.h"
#include "engine_common.h"
#include "engine_threading.h"
#include "engine_string.h"
#include "engine_vector.h"
#include "engine_math.h"

class PlayerDataClass;
class cPacket;
class StealthEffectClass;
class SmartGameObj : public ArmedGameObj {
public:
	SmartGameObj( void );
	virtual	~SmartGameObj( void );
	void	Init( const SmartGameObjDef & definition );
	void	Copy_Settings( const SmartGameObjDef & definition );
	void	Re_Init( const SmartGameObjDef & definition );
	const SmartGameObjDef SCRIPTS_API & Get_Definition( void ) const ;
	virtual	bool	Save( ChunkSaveClass & csave );
	virtual	bool	Load( ChunkLoadClass & cload );
	virtual void On_Post_Load(void);
	void	Clear_Control( void )						{ Control.Clear_Control(); }
	void	Set_Boolean_Control( ControlClass::BooleanControl control, bool state = true )	{ Control.Set_Boolean( control, state ); }
	void	Set_Analog_Control( ControlClass::AnalogControl control, float value )				{ Control.Set_Analog( control, value ); }
	void	Import_Control_Cs( BitStreamClass & packet )     { Control.Import_Cs(packet); }
	void	Export_Control_Cs( BitStreamClass & packet )     { Control.Export_Cs(packet); }
	void	Import_Control_Sc( BitStreamClass & packet )     { Control.Import_Sc(packet); }
	void	Export_Control_Sc( BitStreamClass & packet )     { Control.Export_Sc(packet); }
	ControlClass & Get_Control()						{ return Control; }
	void	Control_Enable( bool enable )				{ ControlEnabled = enable; }
	bool	Is_Control_Enabled( void )					{ return ControlEnabled; }
	void	Reset_Controller( void );
	virtual	void	Generate_Control( void );
	enum {
		SERVER_CONTROL_OWNER		= -99999,
	};
	int Get_Control_Owner(void)						{ return ControlOwner; }
	virtual int Get_Weapon_Control_Owner(void)	{ return Get_Control_Owner(); }
	virtual void Set_Control_Owner(int control_owner)		{ ControlOwner = control_owner; }
	PlayerDataClass * Get_Player_Data( void )						{ return PlayerData; }
	void Set_Player_Data( PlayerDataClass * player_data );
	SCRIPTS_API bool Has_Player(void);
	SCRIPTS_API bool Is_Human_Controlled(void);
	bool Is_Controlled_By_Me(void);
	virtual	void	Apply_Control( void );
	virtual	void		Think();
	virtual	void		Post_Think();
	virtual	void		Apply_Damage( const OffenseObjectClass & damager, float scale = 1.0f, int alternate_skin = -1 );
	virtual float		Get_Max_Speed( void )		{ return 10; }
	virtual float		Get_Turn_Rate( void )		{ return DEG2RAD(360); }
	ActionClass			*Get_Action( void )						{ return &Action; }
	virtual SmartGameObj			*As_SmartGameObj()		{ return this; };
	virtual	void	Import_Frequent( BitStreamClass & packet );
	virtual	void	Export_Frequent( BitStreamClass & packet );
	virtual	void	Import_State_Cs( BitStreamClass & packet );
	virtual	void	Export_State_Cs( BitStreamClass & packet );
	virtual	void	Export_Creation( BitStreamClass & packet );
	virtual	void	Import_Creation( BitStreamClass & packet );
	bool Is_Control_Data_Dirty(cPacket & packet);
	bool	SCRIPTS_API Is_Obj_Visible( PhysicalGameObj *obj );
	bool	SCRIPTS_API Is_Splash_Possible( PhysicalGameObj *obj );
	void	Set_Enemy_Seen_Enabled( bool enabled )	{ IsEnemySeenEnabled = enabled; }
	bool	Is_Enemy_Seen_Enabled( void )				{ return IsEnemySeenEnabled; }
	virtual Matrix3D Get_Look_Transform(void) { return Get_Transform(); }
	virtual void Get_Velocity(Vector3 & vel) {vel.Set(0, 0, 0);}
	virtual	bool	Is_Visible( void )						{ return true; }
	virtual void	On_Logical_Heard (LogicalListenerClass *listener, LogicalSoundClass *sound_obj);
	virtual	void	Begin_Hibernation( void );
	virtual	void	End_Hibernation( void );
	virtual	void	Get_Information( StringClass & string );
	void				Enable_Stealth(bool onoff);
	void				Toggle_Stealth(void);
	bool				SCRIPTS_API Is_Stealth_Enabled(void); //da
	bool				SCRIPTS_API Is_Stealthed(void) const;
	virtual float	Get_Stealth_Fade_Distance(void) const			{ return 25.0f; }
	void				Grant_Stealth_Powerup(float seconds);
	float				Remaining_Stealth_Powerup_Time(void);
	StealthEffectClass * Peek_Stealth_Effect(void) { return StealthEffect; }
	bool                 Get_Stealth_Active() { return StealthActive; }
	void                 Set_Stealth_Active(bool active) { StealthActive = active; Set_Object_Dirty_Bit(NetworkObjectClass::BIT_FREQUENT,true);}
#ifndef TTLE_EXPORTS
	static	float	Get_Global_Sight_Range_Scale( void )			{ return GlobalSightRangeScale; }
	static	void	Set_Global_Sight_Range_Scale( float scale )	{ GlobalSightRangeScale = scale; }
#endif
protected:
	ControlClass			Control;
	PhysControllerClass	Controller;
	bool						ControlEnabled;
	void						Alloc_Stealth_Effect(void);
	bool						StealthEnabled;
	bool                        StealthActive;
	float						StealthPowerupTimer;
	float						StealthFiringTimer;
	StealthEffectClass *	StealthEffect;
private:
	ActionClass				Action;
	int						ControlOwner;
	PlayerDataClass *		PlayerData;
	bool						IsEnemySeenEnabled;
	float						MovingSoundTimer;
	LogicalListenerClass* Listener;
	void Register_Listener(void);
#ifndef TTLE_EXPORTS
	static REF_DECL(float, GlobalSightRangeScale);
#endif
};


#endif
