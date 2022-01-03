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
#ifndef TT_INCLUDE_CHUNKCLASS_H
#define TT_INCLUDE_CHUNKCLASS_H
#include "engine_string.h"
struct ChunkHeader {
	unsigned long ChunkType;
	unsigned long ChunkSize;
};

struct MicroChunkHeader {
	unsigned char ChunkType;
	unsigned char ChunkSize;
};

struct MicroChunkHeader2 {
	unsigned short ChunkType;
	unsigned short ChunkSize;
};

struct IOVector2Struct {
	float X;
	float Y;
};

struct IOVector3Struct {
	float X;
	float Y;
	float Z;
};

struct IOVector4Struct {
	float X;
	float Y;
	float Z;
	float W;
};

struct IOQuaternionStruct {
	float Q[4];
};
class FileClass;
class SCRIPTS_API ChunkLoadClass {
private:
	FileClass* File;
	int StackIndex;
	unsigned long PositionStack[256];
	ChunkHeader HeaderStack[256];
	bool InMicroChunk;
	int MicroChunkPosition;
	MicroChunkHeader MCHeader;
public:
	ChunkLoadClass(FileClass *file);
	bool Open_Chunk();
	bool Peek_Next_Chunk(unsigned int *id, unsigned int *length);
	bool Close_Chunk();
	unsigned long Cur_Chunk_ID();
	unsigned long Cur_Chunk_Length();
	int Cur_Chunk_Depth();
	int Contains_Chunks();
	bool Open_Micro_Chunk();
	bool Close_Micro_Chunk();
	unsigned long Cur_Micro_Chunk_ID();
	unsigned long Cur_Micro_Chunk_Length();
	long Seek(unsigned long nbytes);
	long Read(void *buf, unsigned long nbytes);
	long Read(IOVector2Struct *v);
	long Read(IOVector3Struct *v);
	long Read(IOVector4Struct *v);
	long Read(IOQuaternionStruct *q);
	long Read(StringClass& string);


	template<typename T> TT_INLINE long SimpleRead(T& buf)
	{
		int length = Read(&buf, sizeof(T));
		TT_ASSERT(length == sizeof(T))
		return length;
	}
};

class SCRIPTS_API ChunkSaveClass {
	FileClass* File;
	int StackIndex;
	int PositionStack[256];
	ChunkHeader HeaderStack[256];
	bool InMicroChunk;
	int MicroChunkPosition;
	MicroChunkHeader MCHeader;
public:
	ChunkSaveClass(FileClass *file);
	void Set_Contains_Chunks()
	{
		HeaderStack[StackIndex-1].ChunkSize |= 0x80000000;
	}
	bool Begin_Chunk(unsigned long id);
	bool End_Chunk();
	int Cur_Chunk_Depth();
    unsigned int Cur_Chunk_Length();
	bool Begin_Micro_Chunk(unsigned long id);
	bool End_Micro_Chunk();
	unsigned long Write(void* buf,unsigned long nbytes);
	unsigned long Write(IOVector2Struct& v);
	unsigned long Write(IOVector3Struct& v);
	unsigned long Write(IOVector4Struct& v);
	unsigned long Write(IOQuaternionStruct& q);
	unsigned long Write(const StringClass& sting);


	template<typename T> TT_INLINE long SimpleWrite(const T& buf)
	{
		int length = Write((void*)&buf, sizeof(T));
		TT_ASSERT(length == sizeof(T))
		return length;
	}
};

#endif
