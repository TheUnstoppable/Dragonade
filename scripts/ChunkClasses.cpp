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
#include "General.h"
#include "ChunkClass.h"
#include "engine_io.h"
ChunkLoadClass::ChunkLoadClass(FileClass *file)
{
	File = file;
	StackIndex = 0;
	memset(HeaderStack,0,sizeof(HeaderStack));
	memset(PositionStack,0,sizeof(PositionStack));
	InMicroChunk = false;
	MicroChunkPosition = 0;
	MCHeader.ChunkType = 0;
}

bool ChunkLoadClass::Open_Chunk()
{
	if ((StackIndex <= 0) || (PositionStack[StackIndex-1] != (HeaderStack[StackIndex-1].ChunkSize & 0x7FFFFFFF)))
	{
		if (File->Read(&HeaderStack[StackIndex],8) == 8)
		{
			PositionStack[StackIndex] = 0;
			StackIndex++;
			return true;
		}
	}
	return false;
}

bool ChunkLoadClass::Peek_Next_Chunk(unsigned int *id, unsigned int *length)
{
	ChunkHeader h;
	if ((StackIndex <= 0) || (PositionStack[StackIndex-1] != (HeaderStack[StackIndex-1].ChunkSize & 0x7FFFFFFF)))
	{
		if (File->Read(&h,8) == 8)
		{
			File->Seek(-8,1);
			if (length)
			{
				*length = h.ChunkSize;
			}
			if (id)
			{
				*id = h.ChunkType & 0x7FFFFFFF;
			}
			return true;
		}
	}
	return false;
}

bool ChunkLoadClass::Close_Chunk()
{
	unsigned int x = HeaderStack[StackIndex-1].ChunkSize & 0x7FFFFFFF;
	if (PositionStack[StackIndex-1] < (x))
	{
		File->Seek((x) - PositionStack[StackIndex-1],1);
	}
	StackIndex--;
	if (StackIndex > 0)
	{
		PositionStack[StackIndex-1] += (x);
		PositionStack[StackIndex-1] += 8;
	}
	return true;
}

unsigned long ChunkLoadClass::Cur_Chunk_ID()
{
	return HeaderStack[StackIndex-1].ChunkType;
}

unsigned long ChunkLoadClass::Cur_Chunk_Length()
{
	return HeaderStack[StackIndex-1].ChunkSize & 0x7FFFFFFF;
}

int ChunkLoadClass::Cur_Chunk_Depth()
{
	return StackIndex;
}

int ChunkLoadClass::Contains_Chunks()
{
	return HeaderStack[StackIndex-1].ChunkSize & 0x80000000;
}

bool ChunkLoadClass::Open_Micro_Chunk()
{
	int bytesread = Read(&MCHeader,sizeof(MCHeader));
	if (bytesread == sizeof(MCHeader))
	{
		MicroChunkPosition = 0;
		InMicroChunk = true;
		return true;
	}
	return false;
}

bool ChunkLoadClass::Close_Micro_Chunk()
{
	InMicroChunk = false;
	if (MicroChunkPosition < MCHeader.ChunkSize)
	{
		File->Seek(MCHeader.ChunkSize - MicroChunkPosition,1);
		if (StackIndex > 0)
		{
			PositionStack[StackIndex-1] += MCHeader.ChunkSize - MicroChunkPosition;
		}
	}
	return true;
}

unsigned long ChunkLoadClass::Cur_Micro_Chunk_ID()
{
	return MCHeader.ChunkType;
}

unsigned long ChunkLoadClass::Cur_Micro_Chunk_Length()
{
	return MCHeader.ChunkSize;
}

long ChunkLoadClass::Seek(unsigned long nbytes)
{
	TT_ASSERT(File->Is_Open());
	if ((PositionStack[StackIndex-1] + nbytes) <= (HeaderStack[StackIndex-1].ChunkSize & 0x7FFFFFFF))
	{
		if ((!InMicroChunk) || ((MicroChunkPosition + nbytes) <= MCHeader.ChunkSize))
		{
			int pos = File->Tell();
			int seek = File->Seek(nbytes,1);
			if ((seek - pos) == (int)nbytes)
			{
				PositionStack[StackIndex-1] += nbytes;
				if (InMicroChunk)
				{
					MicroChunkPosition += nbytes;
				}
				return nbytes;
			}
		}
	}
	return 0;
}

long ChunkLoadClass::Read(void *buf, unsigned long nbytes)
{
	TT_ASSERT(File->Is_Open());
	if ((PositionStack[StackIndex-1] + nbytes) <= (HeaderStack[StackIndex-1].ChunkSize & 0x7FFFFFFF))
	{
		if ((!InMicroChunk) || ((MicroChunkPosition + nbytes) <= MCHeader.ChunkSize))
		{
			int read = File->Read(buf,nbytes);
			if (read == (int)nbytes)
			{
				PositionStack[StackIndex-1] += nbytes;
				if (InMicroChunk)
				{
					MicroChunkPosition += nbytes;
				}
				return nbytes;
			}
		}
	}
	return 0;
}

long ChunkLoadClass::Read(IOVector2Struct *v)
{
	return Read(v,sizeof(IOVector2Struct));
}

long ChunkLoadClass::Read(IOVector3Struct *v)
{
	return Read(v,sizeof(IOVector3Struct));
}

long ChunkLoadClass::Read(IOVector4Struct *v)
{
	return Read(v,sizeof(IOVector4Struct));
}

long ChunkLoadClass::Read(IOQuaternionStruct *q)
{
	return Read(q,sizeof(IOQuaternionStruct));
}

long ChunkLoadClass::Read(StringClass& string)
{
	uint16 length;
	int result = SimpleRead(length);
	result += Read(string.Get_Buffer(length+1), length);
	string[length] = '\0';
	TT_ASSERT(result == (int)sizeof(uint16) + (int)length);
	return result;
}

ChunkSaveClass::ChunkSaveClass(FileClass *file)
{
	File = file;
	StackIndex = 0;
	memset(HeaderStack,0,sizeof(HeaderStack));
	memset(PositionStack,0,sizeof(PositionStack));
	InMicroChunk = false;
	MicroChunkPosition = 0;
	MCHeader.ChunkType = 0;
}

bool ChunkSaveClass::Begin_Chunk(unsigned long id)
{
	ChunkHeader chunkh;
	chunkh.ChunkSize = 0;
	chunkh.ChunkType = 0;
	if (StackIndex > 0)
	{
		HeaderStack[StackIndex-1].ChunkSize |= 0x80000000;
	}
	chunkh.ChunkType = id;
	chunkh.ChunkSize &= 0x80000000;
	PositionStack[StackIndex] = File->Seek(0,1);
	HeaderStack[StackIndex].ChunkType = chunkh.ChunkType;
	HeaderStack[StackIndex].ChunkSize = chunkh.ChunkSize;
	StackIndex++;
	if (File->Write(&chunkh,8) == 8)
	{
		return true;
	}
	return false;
}

bool ChunkSaveClass::End_Chunk()
{
	ChunkHeader chunkh;
	int temp = File->Seek(0,1);
	StackIndex--;
	chunkh.ChunkType = HeaderStack[StackIndex].ChunkType;
	chunkh.ChunkSize = HeaderStack[StackIndex].ChunkSize;
	File->Seek(PositionStack[StackIndex],0);
	if (File->Write(&chunkh,8) == 8)
	{
		if ((StackIndex) && (StackIndex < 256))
		{
			int temp2 = (HeaderStack[StackIndex-1].ChunkSize & 0x7FFFFFFF) + (chunkh.ChunkSize & 0x7FFFFFFF) + 8;
			if (HeaderStack[StackIndex-1].ChunkSize & 0x80000000)
			{
				temp2 |= 0x80000000;
			}
			HeaderStack[StackIndex-1].ChunkSize = temp2;
		}
		File->Seek(temp,0);
		return true;
	}
	return false;
}

int ChunkSaveClass::Cur_Chunk_Depth()
{
	return StackIndex;
}

unsigned int ChunkSaveClass::Cur_Chunk_Length()
{
    return HeaderStack[StackIndex-1].ChunkSize & 0x7FFFFFFF;
}

bool ChunkSaveClass::Begin_Micro_Chunk(unsigned long id)
{
	MCHeader.ChunkType = (unsigned char)id;
	MicroChunkPosition = File->Seek(0,1);
	MCHeader.ChunkSize = 0;
	if (Write(&MCHeader,sizeof(MCHeader)) == sizeof(MCHeader))
	{
		InMicroChunk = true;
		return true;
	}
	return false;
}

bool ChunkSaveClass::End_Micro_Chunk()
{
	TT_ASSERT(InMicroChunk);
	int temp = File->Seek(0,1);
	File->Seek(MicroChunkPosition,0);
	if (File->Write(&MCHeader,sizeof(MCHeader)) == sizeof(MCHeader))
	{
		File->Seek(temp,0);
		InMicroChunk = false;
		return true;
	}
	return false;
}

unsigned long ChunkSaveClass::Write(void* buf,unsigned long nbytes)
{
	TT_ASSERT(File->Is_Open());
	if (File->Write(buf,nbytes) == (int)nbytes)
	{
		int temp = (HeaderStack[StackIndex-1].ChunkSize & 0x7FFFFFFF) + nbytes;
		if (HeaderStack[StackIndex-1].ChunkSize & 0x80000000)
		{
			temp |= 0x80000000;
		}
		HeaderStack[StackIndex-1].ChunkSize = temp;
		if (InMicroChunk)
		{
			MCHeader.ChunkSize = MCHeader.ChunkSize + (unsigned char)nbytes;
		}
		return nbytes;
	}
	return 0;
}

unsigned long ChunkSaveClass::Write(IOVector2Struct& v)
{
	return Write(&v,sizeof(IOVector2Struct));
}

unsigned long ChunkSaveClass::Write(IOVector3Struct& v)
{
	return Write(&v,sizeof(IOVector3Struct));
}

unsigned long ChunkSaveClass::Write(IOVector4Struct& v)
{
	return Write(&v,sizeof(IOVector4Struct));
}

unsigned long ChunkSaveClass::Write(IOQuaternionStruct& q)
{
	return Write(&q,sizeof(IOQuaternionStruct));
}

unsigned long ChunkSaveClass::Write(const StringClass& string)
{
	uint16 length = (uint16)string.Get_Length();
	int result = SimpleWrite(length);
	result += Write((char*)string.Peek_Buffer(), length);
	TT_ASSERT(result == (int)sizeof(uint16) + (int)length);
	return result;
}
