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
#ifndef TT_INCLUDE__CONTROLCLASS_H
#define TT_INCLUDE__CONTROLCLASS_H
#include "Vector4.h"
class BitStreamClass;
class ChunkLoadClass;
class ChunkSaveClass;
class ControlClass
{

public:

	typedef enum {
		BOOLEAN_ONE_TIME_FIRST,
		BOOLEAN_JUMP					=	BOOLEAN_ONE_TIME_FIRST,
		BOOLEAN_WEAPON_NEXT,
		BOOLEAN_WEAPON_PREV,
		BOOLEAN_WEAPON_RELOAD,
		BOOLEAN_WEAPON_USE,
		BOOLEAN_DIVE_FORWARD,
		BOOLEAN_DIVE_BACKWARD,
		BOOLEAN_DIVE_LEFT,
		BOOLEAN_DIVE_RIGHT,
		BOOLEAN_ACTION,
		BOOLEAN_SELECT_NO_WEAPON,
		BOOLEAN_SELECT_WEAPON_0,
		BOOLEAN_SELECT_WEAPON_1,
		BOOLEAN_SELECT_WEAPON_2,
		BOOLEAN_SELECT_WEAPON_3,
		BOOLEAN_SELECT_WEAPON_4,
		BOOLEAN_SELECT_WEAPON_5,
		BOOLEAN_SELECT_WEAPON_6,
		BOOLEAN_SELECT_WEAPON_7,
		BOOLEAN_SELECT_WEAPON_8,
		BOOLEAN_SELECT_WEAPON_9,
		BOOLEAN_DROP_FLAG,
		BOOLEAN_VEHICLE_TOGGLE_GUNNER,
		BOOLEAN_CONTINUOUS_FIRST,
		BOOLEAN_WEAPON_FIRE_PRIMARY		=	BOOLEAN_CONTINUOUS_FIRST,
		BOOLEAN_WEAPON_FIRE_SECONDARY,
		BOOLEAN_WALK,
		BOOLEAN_CROUCH,
		BOOLEAN_TOTAL,
		NUM_BOOLEAN_ONE_TIME		= 	BOOLEAN_CONTINUOUS_FIRST - BOOLEAN_ONE_TIME_FIRST,
		NUM_BOOLEAN_CONTINUOUS	= 	BOOLEAN_TOTAL - BOOLEAN_CONTINUOUS_FIRST,
	} BooleanControl;
	typedef enum {
		ANALOG_MOVE_FORWARD,
		ANALOG_MOVE_LEFT,
		ANALOG_MOVE_UP,
		ANALOG_TURN_LEFT,
		ANALOG_CONTROL_COUNT
	} AnalogControl;
	ControlClass() : PendingOneTimeBooleanBits( 0 ), ActionTrigger(false)
	{
		Clear_Control();
	}
	~ControlClass()
	{
	}
	bool				Save( ChunkSaveClass & csave );
	bool				Load( ChunkLoadClass & cload );
	void	Clear_Control( void );
	void	Clear_Boolean( void );
	void    Set_Action_Trigger(bool trigger) {ActionTrigger = trigger;}
	bool    Get_Action_Trigger() {return ActionTrigger;}
	ControlClass & operator = (const ControlClass & src)
	{
		OneTimeBooleanBits = src.OneTimeBooleanBits;
		ContinuousBooleanBits = src.ContinuousBooleanBits;
		AnalogValues[0] = src.AnalogValues[0];
		AnalogValues[1] = src.AnalogValues[1];
		AnalogValues[2] = src.AnalogValues[2];
		AnalogValues[3] = src.AnalogValues[3];
		return *this;
	}
	void	Set_Boolean( BooleanControl control, bool state = true );
	bool	Get_Boolean( BooleanControl control );
	void	Clear_One_Time_Boolean( void )				{OneTimeBooleanBits = 0;}
	void    Clear_Analog() {AnalogValues[0] = 0;AnalogValues[1] = 0;AnalogValues[2] = 0;AnalogValues[3] = 0;}
	unsigned	long Get_One_Time_Boolean_Bits(void)	{return OneTimeBooleanBits;}
	unsigned	char Get_Continuous_Boolean_Bits(void) {return ContinuousBooleanBits;}
	void	Set_Analog( AnalogControl control, float value );
	float	Get_Analog( AnalogControl control );
	void	Import_Cs( BitStreamClass & packet );
	void	Export_Cs( BitStreamClass & packet );
	void	Import_Sc( BitStreamClass & packet );
	void	Export_Sc( BitStreamClass & packet );
	static void Set_Precision(void);
private:
	unsigned		long	OneTimeBooleanBits;
	unsigned		long	PendingOneTimeBooleanBits;
	unsigned		char	ContinuousBooleanBits;
	unsigned		char	PendingContinuousBooleanBits;
	bool            ActionTrigger;
	float			AnalogValues[ ANALOG_CONTROL_COUNT ];
}; // size: 28

inline void	ControlClass::Set_Analog( AnalogControl control, float value )	
{
	AnalogValues[ control ] = value; 
}
inline float ControlClass::Get_Analog( AnalogControl control )
{ 
	return AnalogValues[ control ]; 
}

#endif
