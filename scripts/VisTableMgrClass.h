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
#ifndef TT_INCLUDE__VISTABLEMGRCLASS_H
#define TT_INCLUDE__VISTABLEMGRCLASS_H

#include "engine_common.h"
#include "engine_vector.h"
class CompressedVisTableClass;
class VisTableClass;
class VisDecompressionCacheClass
{
public:
	SimpleVecClass<VisTableClass *> Cache; //0
	MultiListClass<VisTableClass> tablelist; //C
	int timestamp; //24
	VisDecompressionCacheClass()
	{
	}
	~VisDecompressionCacheClass()
	{
	}
	void Reset(int size);
	VisTableClass *Get_Table(int id);
	void Add_Table(VisTableClass *table);
	void Release_Old_Tables();
};
class VisTableMgrClass
{
public:
	int count; //0
	int size; //4
	SimpleDynVecClass<CompressedVisTableClass *> tables; //8
	VisDecompressionCacheClass *cache; //18
	int frames; //1C
	VisTableMgrClass() : count(0), size(0), frames(0)
	{
		cache = new VisDecompressionCacheClass();
		Reset();
	}
	~VisTableMgrClass()
	{
		Reset();
		if (cache)
		{
			cache->Reset(0);
			delete cache;
		}
	}
	void Reset()
	{
		Delete_All_Vis_Tables();
		tables.Delete_All();
		size = 1;
		count = 0;
		frames = 0;
		cache->Reset(0);
	}
	void Delete_All_Vis_Tables();
	unsigned int Get_Vis_Table_Size()
	{
		return size;
	}
	VisTableClass *Get_Vis_Table(int id,bool b);
	void Save(ChunkSaveClass &csave);
	void Load(ChunkLoadClass &cload);
	int Allocate_Vis_Object_ID(int count);
	int Allocate_Vis_Sector_ID(int count);
	int Get_Vis_Table_Count()
	{
		return count;
	}
	bool Has_Vis_Table(int table)
	{
		return tables[table] != 0;
	}
	void Notify_Frame_Ended();
	void Set_Optimized_Vis_Object_Count(int count)
	{
		size = count;
	}
	void Update_Vis_Table(int, VisTableClass *);
}; //20

#endif
