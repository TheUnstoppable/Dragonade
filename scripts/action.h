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
#ifndef TT_INCLUDE_ACTION_H
#define TT_INCLUDE_ACTION_H
#include "scripts.h"
#include "engine_io.h"
#include "ReferencerClass.h"
class		ActionCodeClass;
class		SmartGameObj;
class		SafeActionParamsStruct : public ActionParamsStruct {
public:
	SafeActionParamsStruct & operator = (const ActionParamsStruct & src);
	bool					Save( ChunkSaveClass & csave );
	bool					Load( ChunkLoadClass & cload );
	ReferencerClass	MoveObjectRef;
	ReferencerClass	AttackObjectRef;
	ReferencerClass	LookObjectRef;
	StringClass			SafeAnimationName;
	StringClass			SafeConversationName;
};
class	ActionClass {
public:
	ActionClass( SmartGameObj *obj );
	virtual ~ActionClass( void );
	bool				Save( ChunkSaveClass & csave );
	bool				Load( ChunkLoadClass & cload );
	SmartGameObj *	Get_Action_Obj( void )								{ return ActionObj; }
	SafeActionParamsStruct &	Get_Parameters( void )				{ return Parameters; }
	SCRIPTS_API bool				Is_Acting( void );
	bool				Is_Animating( void );
	void				Begin_Hibernation( void );
	void				End_Hibernation( void );
	bool				Reset( float priority );
	bool				Follow_Input( const ActionParamsStruct & parameters );		
	bool				Stand( const ActionParamsStruct & parameters );
	bool				Play_Animation( const ActionParamsStruct & parameters );
	bool				Goto( const ActionParamsStruct & parameters );
	bool				Enter_Exit( const ActionParamsStruct & parameters );
	bool				Dive( const ActionParamsStruct & parameters );
	bool				Attack( const ActionParamsStruct & parameters );
	bool				Face_Location( const ActionParamsStruct & parameters );
	bool				Have_Conversation( const ActionParamsStruct & parameters );
	bool				Dock_Vehicle( const ActionParamsStruct & parameters );
	bool				Modify( const ActionParamsStruct & parameters, bool modify_move, bool modify_attack );
	void				Act( void );
	unsigned int	Get_Act_Count( void ) { return ActCount; }
	bool				Is_Active( void );
	bool				Is_Busy( void );
	bool				Is_Paused( void ) const	{ return IsPaused; }
	void				Pause( bool onoff)		{ IsPaused = onoff; }
	void				Done( int reason );
protected:
	void				Notify_Completed( int observer_id, int action_id, int reason );
	bool				Request_Action( ActionCodeClass * action, const ActionParamsStruct & parameters );
	SmartGameObj *				ActionObj;
	ActionCodeClass *			ActionCode;
	SafeActionParamsStruct	Parameters;
	bool							IsPaused;
	unsigned int				ActCount;
	void				Set_Action_Code( ActionCodeClass * code );
};
#endif
