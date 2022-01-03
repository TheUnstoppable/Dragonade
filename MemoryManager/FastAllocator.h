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
// Do you know what you are doing?
// No? Then don't touch this file.
#ifndef _FASTALLOCATOR_H_
#define _FASTALLOCATOR_H_

#ifndef _MEMORYMANGER_INTERNAL_
#error This is not a public interface, please do not include this file outside of MemoryManager.dll
#endif

#include "platform.h"

#if defined(_DEBUG) && defined(WIN32) && defined(_M_IX86) // FASTALLOC_STATS only works for x86 windows builds
#define FASTALLOC_STATS
#endif
#include "TSList.h"
#include <string.h>

DECLSPEC_RESTRICT void*	SystemAllocate(size_t size);
void					SystemFree(void* memory);

class SystemAllocatedClass
{
protected:
	DECLSPEC_RESTRICT static inline void* SystemAllocate(size_t size)
	{
		return ::SystemAllocate(size);
	};
	static inline void SystemFree(void* memory)
	{
		::SystemFree(memory);
	};
public:
	DECLSPEC_RESTRICT static inline void* operator new(size_t size) 
	{ 
		return SystemAllocate(size); 
	};

	static inline void operator delete(void* memory) 
	{ 
		SystemFree(memory); 
	};
};

class FastFixedAllocator: public SystemAllocatedClass
{
protected:
	struct Link: public TSLNode
	{
	};
	struct Chunk: public SystemAllocatedClass
	{
		enum 
		{
			size = 32 * 1024 - sizeof(TSLNode)
		};
		TSLNode			Node;
		char			Memory[size];
	};

	TSList<Link>	FreeList;
	TSList<Chunk>	Chunks;
	size_t			ElementSize;
#ifdef FASTALLOC_STATS
public:
	unsigned long TotalHeapSize;
	unsigned long TotalAllocatedSize;
	unsigned long TotalAllocationCount;
protected:
#endif
public:
	FastFixedAllocator(size_t element_size = 1);
	~FastFixedAllocator();
	void Init(size_t element_size);
	void Grow();

	inline size_t GetSize() const { return ElementSize; }

	DECLSPEC_RESTRICT void* Allocate();
	void Free(void* memory);
};

inline FastFixedAllocator::FastFixedAllocator(size_t element_size): 
ElementSize(1), 
Chunks(), 
FreeList()
#ifdef FASTALLOC_STATS
,
TotalHeapSize(0),
TotalAllocatedSize(0),
TotalAllocationCount(0)
#endif
{
	Init(element_size);
};

inline FastFixedAllocator::~FastFixedAllocator() // *not* thread safe
{
	for (Chunk* n = (Chunk*)Chunks.Pop(); n; n = Chunks.Pop())
	{
		delete n;
	};
};

inline void FastFixedAllocator::Grow()
{
	Chunk* n = new Chunk;
	Chunks.Push(n);

#ifdef FASTALLOC_STATS
	_InterlockedExchangeAdd((long *)&TotalHeapSize, sizeof(Chunk));
#endif

	const size_t number_elements = Chunk::size / ElementSize;

	for (size_t i = 0; i < number_elements; ++i)
	{
		Link* link = (Link*)(n->Memory + (i * ElementSize) );
		link->Next = 0;
		FreeList.Push(link);
	};
};

inline void FastFixedAllocator::Init(size_t element_size)
{
	this->ElementSize = element_size < sizeof(Link) ? sizeof (Link) : element_size;
};

DECLSPEC_RESTRICT inline void* FastFixedAllocator::Allocate()
{
#ifdef FASTALLOC_STATS
	_InterlockedIncrement((long *)&TotalAllocationCount);
	_InterlockedExchangeAdd((long *)&TotalAllocatedSize, (long)ElementSize);
#endif

	void* memory = FreeList.Pop();
	while (!memory)
	{
		Grow();
		memory = FreeList.Pop();
		YieldThread();
	}
	return memory;
};

inline void FastFixedAllocator::Free(void* memory)
{
	assert(memory); // If you hit this assert, you tried to free NULL. NULL doesn't want to be free ;)
#ifdef FASTALLOC_STATS
	_InterlockedDecrement((long*)&TotalAllocationCount);
	_InterlockedExchangeAdd((long*)&TotalAllocatedSize, -(long)ElementSize);
#endif

	FreeList.Push((Link*)memory);
};

class FastAllocatorGeneral: public SystemAllocatedClass
{
protected:
	enum 
	{	
		NUM_ALLOCATORS = 11, 
#if defined(_M_IX86)
		MIN_ALLOC_BIT = 3,
#elif defined(_M_X64)
		MIN_ALLOC_BIT = 4,
#endif
		MIN_ALLOC_SIZE = 1 << MIN_ALLOC_BIT,
		MAX_ALLOC_SIZE = 1 << (NUM_ALLOCATORS + MIN_ALLOC_BIT - 1),
	};

	unsigned long	GetAllocatorIndex(size_t input) const;

	FastFixedAllocator allocators[NUM_ALLOCATORS];
#ifdef FASTALLOC_STATS
	size_t ActualMemoryUsage;
	unsigned long ExternalMemoryUsage;
	unsigned long ExternalMemoryCount;
#endif
public:
	static FastAllocatorGeneral* Get_Allocator();
	FastAllocatorGeneral();

	DECLSPEC_RESTRICT void*	Allocate(size_t size);
	void	Free(void* memory);
	DECLSPEC_RESTRICT void*	Reallocate(void* memory, size_t newSize);
	size_t	GetAllocationSize(void* memory) const;
#ifdef FASTALLOC_STATS
	size_t GetTotalMemoryUsage() const { return ActualMemoryUsage; };
#endif
};

inline FastAllocatorGeneral::FastAllocatorGeneral()
#ifdef FASTALLOC_STATS
:
ActualMemoryUsage(0), 
ExternalMemoryUsage(0), 
ExternalMemoryCount(0)
#endif
{
	for (int i = 0; i < NUM_ALLOCATORS; ++i)
	{
#pragma warning(suppress: 6297) //warning c6297: Result of shift cast to larger size
		allocators[i].Init(1 << (i + MIN_ALLOC_BIT));
		allocators[i].Grow(); // let's do the first growing here so that we don't fight over allocating
	};
};

inline unsigned long FastAllocatorGeneral::GetAllocatorIndex(size_t input) const
{
	unsigned long result;
	ASSUME((input & 0xFFFFFFFF00000000) == 0); // assume we are only doing allocation below 32 bits, please?
	_BitScanReverse(&result, (int32)(input - 1) / (MIN_ALLOC_SIZE / 2) | 1);
	return result;
};

DECLSPEC_RESTRICT inline void* FastAllocatorGeneral::Allocate(size_t size)
{
	void* memory = 0;

	// let's actually allocate n+4 bytes so we can store the
	// number of bytes we allocate and return a pointer to the
	// rest back to the user.
	size += sizeof(size_t);

	size_t index = 0;

	if (size < MAX_ALLOC_SIZE)
	{
		index = GetAllocatorIndex(size);
		memory = allocators[index].Allocate();
#ifdef FASTALLOC_STATS
		_InterlockedExchangeAdd((long*)&ActualMemoryUsage, (long)allocators[index].GetSize());
#endif
	}
	else
	{
#ifdef FASTALLOC_STATS
		_InterlockedExchangeAdd((long*)&ActualMemoryUsage, size);
		_InterlockedExchangeAdd((long*)&ExternalMemoryUsage, size);
		_InterlockedIncrement((long*)&ExternalMemoryCount);
#endif
		// Hello mister, this memory is too big. Can you carry it for me?
		memory = SystemAllocate(size);
		index = size;
	};
	
	*((size_t*)memory) = index; // save the augmented size so we can use it later to liberate the memory and not royally screw things up
	return ((size_t*)memory) + 1; // and don't tell the user about the augmented size cuz we are evil and all that
};

inline void FastAllocatorGeneral::Free(void* _memory)
{
	if (_memory)
	{
		void* memory = ((size_t*)_memory) - 1; // Retrieve the real memory
		
		size_t index = *((size_t*)memory);

		if (index < NUM_ALLOCATORS)
		{
#ifdef FASTALLOC_STATS
			_InterlockedExchangeAdd((long*)&ActualMemoryUsage, -(long)allocators[index].GetSize());
#endif
			allocators[index].Free(memory);
		}
		else
		{
#ifdef FASTALLOC_STATS
			_InterlockedExchangeAdd((long*)&ActualMemoryUsage, -(long)index);
			_InterlockedExchangeAdd((long*)&ExternalMemoryUsage, -(long)index);
			_InterlockedDecrement((long*)&ExternalMemoryCount);
#endif
			// Mister, can you set this memory free for me?
			SystemFree(memory);
		};
	};
};

DECLSPEC_RESTRICT inline void* FastAllocatorGeneral::Reallocate(void* _memory, size_t newSize)
{
	if (_memory)
	{
		void* memory = ((size_t*)_memory) - 1; // Retrieve the real memory
		
		size_t index = *((size_t*)memory);
		
		if(newSize > allocators[index].GetSize() - sizeof(size_t))
		{
			void* newMemory = Allocate(newSize);
			memcpy(newMemory, _memory, allocators[index].GetSize() - sizeof(size_t));
			allocators[index].Free(memory);
			_memory = newMemory;
		}
	}
	else
	{
		_memory = Allocate(newSize);
	}
	return _memory;
};

inline size_t FastAllocatorGeneral::GetAllocationSize(void* memory) const
{
	size_t index = *(((size_t*)memory) - 1);
#pragma warning(suppress: 6297) //warning c6297: Result of shift cast to larger size
	return (index < NUM_ALLOCATORS) ? size_t(1 << (index + MIN_ALLOC_BIT)) : index;
};

template <typename T> class FastObjectAllocator
{
protected:
	FastFixedAllocator allocator;

public:
	FastObjectAllocator()
	{
		allocator.Init(sizeof(T));
		allocator.Grow();
	};

	~FastObjectAllocator()
	{
		// TODO: make sure everything is free'd
	};

	DECLSPEC_RESTRICT void* Allocate()
	{
		return allocator.Allocate();
	};

	void Free(void* object)
	{
		allocator.Free(object);
	};
};

//TODO: AutoObjectPool

#endif
