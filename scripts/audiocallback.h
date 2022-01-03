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
#ifndef TT_INCLUDE_AUDIOCALLBACK_H
#define TT_INCLUDE_AUDIOCALLBACK_H
#include "engine_vector.h"
class SoundSceneObjClass;
class LogicalListenerClass;
class LogicalSoundClass;
class AudioCallbackClass
{
public:
	typedef enum
	{
		EVENT_NONE					= 0x0000,
		EVENT_SOUND_STARTED		= 0x0001,
		EVENT_SOUND_ENDED			= 0x0002,
		EVENT_LOGICAL_HEARD		= 0x0004
	} EVENTS;
	AudioCallbackClass (void);
	virtual ~AudioCallbackClass (void);
	virtual void	On_Sound_Started (SoundSceneObjClass *sound_obj)	{ }
	virtual void	On_Sound_Ended (SoundSceneObjClass *sound_obj)		{ }
	virtual void	On_Logical_Heard (LogicalListenerClass *listener, LogicalSoundClass *sound_obj)	{ }
	void				On_Registered (SoundSceneObjClass *sound);
	void				On_UnRegistered (SoundSceneObjClass *sound);
	void				Remove_All_Callbacks (void);
	DynamicVectorClass<SoundSceneObjClass *>	SoundList;
};
#endif
