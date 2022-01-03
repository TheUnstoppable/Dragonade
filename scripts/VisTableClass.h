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
#ifndef TT_INCLUDE__VISTABLECLASS_H
#define TT_INCLUDE__VISTABLECLASS_H



#include "engine_vector.h"
#include "multilist.h"


class CompressedVisTableClass;
class VisTableClass :
	public RefCountClass,
	public MultiListObjectClass
{

	int bitCount; // 0010  0028
	union
	{
		byte* bytes; // 0014  002C
		uint32* longs; // 0014  002C
	};

	int visSectorId; // 0018  0030
	int timeStamp; // 001C  0034

public:

	VisTableClass(uint, int);
	VisTableClass(CompressedVisTableClass*, int, int);
	VisTableClass(const VisTableClass& that);
	~VisTableClass();
	const VisTableClass& operator =(const VisTableClass& that);

	void Alloc_Buffer(int);
	byte* Get_Bytes() const { return bytes; }
	int Get_Byte_Count() const { return 4 * Get_Long_Count(); }
	uint32* Get_Longs() const { return longs; }
	int Get_Long_Count() const;
	void Reset_All();
	void Set_All();
	void Delete_Bit(int);
	void Merge(const VisTableClass &that);
	void Invert();
	bool Is_Equal_To(const VisTableClass &that) const;
	int Count_Differences(const VisTableClass &that) const;
	int Count_True_Bits() const;
	float Match_Fraction(const VisTableClass &that) const;
	int Get_Bit_Count() const { return bitCount; }
	void Set_Vis_Sector_ID(int _visSectorId) { visSectorId = _visSectorId; }
	int Get_Vis_Sector_ID() const { return visSectorId; }
	void Set_Time_Stamp(int _timeStamp) { timeStamp = _timeStamp; }
	int Get_Time_Stamp() const { return timeStamp; }
	bool Get_Bit(int) const;
	void Set_Bit(int, bool);

}; // 0020  0038

class CompressedVisTableClass
{
public:
	CompressedVisTableClass() : BufferSize(0), Buffer(0)
	{
	}
	CompressedVisTableClass(VisTableClass *bits) : BufferSize(0), Buffer(0)
	{
		Compress(bits->Get_Bytes(),bits->Get_Byte_Count());
	}
	CompressedVisTableClass(CompressedVisTableClass &that) : BufferSize(0), Buffer(0)
	{
		*this = that;
	}
	~CompressedVisTableClass()
	{
		if (Buffer)
		{
			delete[] Buffer;
		}
	}
	CompressedVisTableClass& operator=(const CompressedVisTableClass& that)
	{
		if (Buffer)
		{
			delete[] Buffer;
		}
		BufferSize = that.BufferSize;
		Buffer = new unsigned char[BufferSize];
		memcpy(Buffer,that.Buffer,BufferSize);
		return *this;
	}
	unsigned char *Get_Bytes()
	{
		return Buffer;
	}
	int Get_Byte_Count()
	{
		return BufferSize;
	}
	void Load(ChunkLoadClass &cload);
	void Save(ChunkSaveClass &csave);
	void Compress(unsigned char *bytes,int src_size);
	void Decompress(unsigned char *bytes,int src_size);
private:
	int BufferSize;
	unsigned char *Buffer;
};

#endif