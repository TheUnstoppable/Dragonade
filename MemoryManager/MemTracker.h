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
#ifndef _MEMTRACKER_H_
#define _MEMTRACKER_H_

#ifndef _MEMORYMANGER_INTERNAL_
#error This is not a public interface, please do not include this file outside of MemoryManager.dll
#endif

#include "DList.h"
#include "FastAllocator.h"

#define DETAILED_LEAK_LOGS 0
//#define LARGE_ALLOCATION_DEBUG_STATS

#pragma warning(push)
#pragma warning(disable: 4351) // warning C4351: new behavior: elements of array will be default initialized

enum AllocType
{
	AllocType_Unknown,
	AllocType_New,
	AllocType_VectorNew,
	AllocType_Malloc,
	AllocType_Calloc,
	AllocType_Realloc,
	AllocType_Unvalidated,
};

enum DeallocType
{
	DeallocType_Unknown,
	DeallocType_Realloc,
	DeallocType_Delete,
	DeallocType_VectorDelete,
	DeallocType_Free,
	DeallocType_Unvalidated,
};

#define ALLOCTYPE_DEFINED

class AllocationUnit: public DLNode<AllocationUnit>
{
public:
	AllocationUnit() : 
		ActualAddress(NULL), 
		ActualSize(0), 
		ReportedAddress(NULL), 
		ReportedSize(NULL),
		SourceFile(),
		SourceFunction(),
		SourceLine(-1),
		AllocationType(AllocType_Unknown),
		AllocationNumber((unsigned int)-1),
		AllocationTag(),
		BreakOnFree(false),
		BreakOnRealloc(false)
#if DETAILED_LEAK_LOGS
		,stack()
#endif
	{

	};

	static void*	operator new(size_t size);
	static void		operator delete(void* memory);

	void*			ActualAddress;
	size_t			ActualSize;
	void*			ReportedAddress;
	size_t			ReportedSize;

	char			SourceFile[128];
	char			SourceFunction[128];
	int				SourceLine;

	AllocType		AllocationType;
	unsigned int	AllocationNumber;
	char			AllocationTag[32];

	bool			BreakOnFree;
	bool			BreakOnRealloc;

#if DETAILED_LEAK_LOGS
	unsigned char	stack[512];
#endif
};

class MemoryTrackerThreadLocalInformation
{
public:
	char		CurrentSourceFile[128];
	char		CurrentSourceFunction[128];
	int			CurrentSourceLine;

	char		AllocationTagStack[32][32]; // stack depth = 32 items deep, 32 chars each
	int			CurrentAllocationTag;
};

class MemoryTracker
{
protected:
	enum 
	{
		SENTINEL_SIZE =		8,				// how many sentinels on either side of the memory
		PREFIX_PATTERN =	0xbaadf00d,		// used to fill the bytes preceding allocations
		POSTFIX_PATTERN =	0xdeadc0de,		// used to fill the bytes following allocations
		UNUSED_PATTERN =	0xfeedface,		// used to fill freshly allocated memory
		RELEASED_PATTERN =	0xdeadbeef,		// used to fill freshly deallocated memory
	};

	static long			CurrentAllocationCount;

	static inline void* CalculateActualAddress(void* reported_address)
	{
		// memory start - sentinel size - pointer size 
		return (char*)reported_address - (SENTINEL_SIZE * sizeof(int32)) - sizeof(AllocationUnit*);
	};
	
	static inline void* CalculateReportedAddress(void* actual_address)
	{
		// actual + pointer size + sentinel size 
		return (char*)actual_address + sizeof(AllocationUnit*) + (SENTINEL_SIZE * sizeof(int32));
	};

	static inline void* CalculatePrefixSentinelStartAddress(AllocationUnit* unit)
	{
		// actual + pointer size
		return (char*)unit->ActualAddress + sizeof(AllocationUnit*);
	};

	static inline void* CalculatePostfixSentinelStartAddress(AllocationUnit* unit)
	{
		// memory start + memory size
		return (char*)unit->ReportedAddress + unit->ReportedSize;
	};

	static inline void ClearThreadLocalInformation()
	{
		SetThreadLocalInformation(NULL, NULL, -1);
	};

	static const size_t		GetMemoryTrackingCost();
	static AllocationUnit*	FindAllocationUnit(void* memory);
	static void				WipeMemoryWithPattern(AllocationUnit* unit, int32 pattern, size_t bias = 0);
	static bool				ValidateAllocationUnit(AllocationUnit* unit);
public:
	~MemoryTracker();
	static MemoryTrackerThreadLocalInformation* GetThreadLocalInformation();
	static void		SetThreadLocalInformation(const char* source_file, const char* source_function, const int source_line);
	static void		PushAllocationTag(const char* tag);
	static void		PopAllocationTag();
	DECLSPEC_RESTRICT static void* Allocate(const size_t size, const AllocType type = AllocType_Unknown, const char* source_file = "(Unknown File)", const char* source_function = "(Unknown Function)", const int source_line = -1);
	DECLSPEC_RESTRICT static void* Reallocate(void* memory, const size_t size, const char* source_file = "(Unknown File)", const char* source_function = "(Unknown Function)", const int source_line = -1);
	static void		Free(void* memory, const DeallocType type = DeallocType_Unknown, const char* source_file = "(Unknown File)", const char* source_function = "(Unknown Function)", const int source_line = -1);
	static size_t	GetAllocationSize(void* memory);
};

#pragma warning(pop)

#endif
