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
#ifndef TT_INCLUDE__ANIMCONTROLCLASS_H
#define TT_INCLUDE__ANIMCONTROLCLASS_H

typedef	enum {
	ANIM_MODE_ONCE,
	ANIM_MODE_LOOP,
	ANIM_MODE_STOP,
	ANIM_MODE_TARGET
} AnimMode;

class ChunkLoadClass;
class ChunkSaveClass;
class HAnimClass;
class RenderObjClass;


class AnimControlClass
{

protected:

   RenderObjClass *Model;


public:

	AnimControlClass( void );
	virtual ~AnimControlClass( void );

	virtual	bool 	Save( ChunkSaveClass & csave );
	virtual	bool	Load( ChunkLoadClass & cload );
	virtual	void	Set_Model( RenderObjClass	*anim_model );
	virtual	RenderObjClass	*Peek_Model( void )	{ return Model; }
	virtual	void	Set_Animation( const char *name, float blendtime = 0.0f, float start_frame = 0.0f )	= 0;
	virtual	void	Set_Animation( const HAnimClass * anim, float blendtime = 0.0f, float start_frame = 0.0f  ) = 0;
	virtual	void	Set_Mode( AnimMode mode, float frame = -1 )						= 0;
	virtual	AnimMode	Get_Mode( void )														= 0;
	virtual	bool	Is_Complete( void )														= 0;
	virtual	const char *Get_Animation_Name( void )										= 0;
	virtual	void	Set_Target_Frame( float frame )										= 0;
	virtual	float	Get_Target_Frame( void )												= 0;
	virtual	float	Get_Current_Frame( void )												= 0;
	virtual HAnimClass *	Peek_Animation( void ) { return NULL; }
	virtual	void	Update( float dtime )													= 0;

}; // 8

#endif
