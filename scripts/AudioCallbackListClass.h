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
#ifndef TT_INCLUDE__AUDIOCALLBACKLISTCLASS_H
#define TT_INCLUDE__AUDIOCALLBACKLISTCLASS_H



#include "engine_vector.h"



template<typename Callback>
struct AUDIO_CALLBACK_STRUCT
{
	Callback callback; // 0000
	uint32 userData; // 0004
}; // 0008



template<typename Callback>
class AudioCallbackListClass :
	public SimpleDynVecClass<AUDIO_CALLBACK_STRUCT<Callback>>
{

public:

	Callback Get_Callback(int index, uint32* userData)
	{
		if (userData)
			*userData = (*this)[index].userData;
		return (*this)[index].callback;
	}



	void Add_Callback(Callback callback, uint32 userData)
	{
		AUDIO_CALLBACK_STRUCT<Callback> callbackStruct = {callback, userData};
		Add(callbackStruct);
	}



	void Remove_Callback(Callback callback)
	{
		for (int index = 0; index < Count(); ++index)
		{
			if ((*this)[index].callback == callback)
			{
				Remove(index);
				break;
			}
		}
	}

};



#endif