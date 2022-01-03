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
#ifndef TT_INCLUDE_SOUNDBUFFERCLASS_H
#define TT_INCLUDE_SOUNDBUFFERCLASS_H
#include "engine_vector.h"
class FileClass;
class SoundBufferClass : public RefCountClass {
public:
	char*  cBuffer;    // 32
	uint32 uLength;    // 36
	char*  cFile;      // 40
	uint32 uDuration;  // 44
	uint32 uRate;      // 48
	uint32 uBits;      // 52
	uint32 uChannels;  // 56
	uint32 uType;      // 60
	unsigned int flag;
	int cachecount;
	SoundBufferClass();
	virtual ~SoundBufferClass();
	virtual bool        Load_From_File   (const char* cFile);
	virtual bool        Load_From_File   (FileClass& xFile);
	virtual bool        Load_From_Memory (const char* cBuffer, uint32 uLength);
	virtual const char* Get_Raw_Buffer   () const;
	virtual uint32      Get_Raw_Length   () const;
	virtual const char* Get_Filename     () const;
	virtual void        Set_Filename     (const char* cFile);
	virtual uint32      Get_Duration     () const;
	virtual uint32      Get_Rate         () const;
	virtual uint32      Get_Bits         () const;
	virtual uint32      Get_Channels     () const;
	virtual uint32      Get_Type         () const;
	virtual bool        Is_Streaming     () const;
	virtual void        Free_Buffer      ();
	virtual void        Determine_Stats  (char *stats);
};

class StreamSoundBufferClass : public SoundBufferClass {
public:
	StreamSoundBufferClass();
	virtual ~StreamSoundBufferClass();
	virtual bool        Load_From_File   (const char* cFile);
	virtual bool        Load_From_File   (FileClass& xFile);
	virtual bool        Load_From_Memory (const char* cBuffer, uint32 uLength);
	virtual bool        Is_Streaming     () const;
	virtual void        Free_Buffer      ();
	virtual bool        Load_From_File(void *handle, uint32 u1, uint32 u2);
};

#endif