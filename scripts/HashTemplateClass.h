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
#ifndef TT_INCLUDE__HASHTEMPLATECLASS_H
#define TT_INCLUDE__HASHTEMPLATECLASS_H



#include "HashTemplateKeyClass.h"
#include "engine_math.h"


template<typename Key, typename Value> class HashTemplateIterator;



template<typename Key, typename Value> class HashTemplateClass
{

private:

	friend HashTemplateIterator<Key, Value>;

	struct Entry
	{
		int next;
		Key key;
		Value value;
	};
	
	int* indices; // 0000
	Entry* entries; // 0004
	int unusedEntryIndex; // 0008
	uint maxHashCount; // 000C
	
	
	
	static uint computeHash(const Key& key, uint maxHashCount)
	{
		TT_ASSERT(isPowerOfTwo(maxHashCount)); // Make sure maxHashCount is a power of two, or the fast modulo code below will not work
		return HashTemplateKeyClass<Key>::Get_Hash_Value(key) & (maxHashCount - 1);
	}
	
	
	
	uint computeHash(const Key& key) const
	{
		return computeHash(key, maxHashCount);
	}
	
	
	
	void Re_Hash()
	{
		uint newMaxHashCount = max(maxHashCount * 2, 4); // Increase the size and make sure there are at least 4 entries.
		
		Entry* newEntries = new Entry[newMaxHashCount];
		int* newIndices = new int[newMaxHashCount];
		unusedEntryIndex = 0;
		
		for (uint index = 0; index < newMaxHashCount; index++)
			newIndices[index] = -1;
		
		for (uint hash = 0; hash < maxHashCount; hash++)
		{
			for (int index = indices[hash]; index != -1; index = entries[index].next)
			{
				uint hash2 = computeHash(entries[index].key, newMaxHashCount);
				
				newEntries[unusedEntryIndex].next = newIndices[hash2];
				newEntries[unusedEntryIndex].key = entries[index].key;
				newEntries[unusedEntryIndex].value = entries[index].value;
				
				newIndices[hash2] = unusedEntryIndex;
				unusedEntryIndex++;
			}
		}
		
		delete[] indices;
		delete[] entries;
		
		for (uint i = unusedEntryIndex; i < newMaxHashCount-1; i++)
			newEntries[i].next = i + 1;
	
		newEntries[newMaxHashCount-1].next = -1;
		
		indices = newIndices;
		entries = newEntries;
		maxHashCount = newMaxHashCount;
	}


	
public:



	HashTemplateClass()
	{
		indices = NULL;
		entries = NULL;
		unusedEntryIndex = -1;
		maxHashCount = 0;
	}



	~HashTemplateClass()
	{
		delete[] indices;
		delete[] entries;
	}
	
	
	
	Value* Get(const Key& key) const
	{
		if (indices)
			for (uint index = indices[computeHash(key)]; index != -1; index = entries[index].next)
				if (entries[index].key == key)
					return &entries[index].value;
		
		return NULL;
	}
	
	
	
	Value Get(const Key& key, Value default) const
	{
		Value* result = Get(key);
		return result ? *result : default;
	}
	
	
	
	bool Exists(const Key& key) const
	{
		return Get(key) != NULL;
	}
	
	
	
	void Insert(const Key& key, const Value& value)
	{
		// If all entries are in use, enlarge the table
		if (unusedEntryIndex == -1)
			Re_Hash();
		
		uint index = unusedEntryIndex;
		unusedEntryIndex = entries[index].next;
		
		uint hash = computeHash(key);
		entries[index].key = key;
		entries[index].value = value;
		entries[index].next = indices[hash];
		indices[hash] = index;
	}
	
	
	
	void Remove(const Key& key)
	{
		if (indices)
		{
			uint hash = computeHash(key);
			int* lastEntryNext = &indices[hash];
			
			for (uint index = indices[hash]; index != -1; index = entries[index].next)
			{
				Entry& entry = entries[index];
				if (entry.key == key)
				{
					*lastEntryNext = entry.next;
					entries[index].next = unusedEntryIndex;
					unusedEntryIndex = index;
					
					break;
				}
				
				lastEntryNext = &entries[index].next;
			}
		}
	}
	
	
	
	// Remove if keys and values match
	void Remove(const Key& key, const Value& value)
	{
		if (indices)
		{
			uint hash = computeHash(key);
			int* lastEntryNext = &indices[hash];
			
			for (uint index = indices[hash]; index != -1; index = entries[index].next)
			{
				Entry& entry = entries[index];
				if (entry.key == key && entry.value == value)
				{
					*lastEntryNext = entry.next;
					entries[index].next = unusedEntryIndex;
					unusedEntryIndex = index;
					
					break;
				}
				
				lastEntryNext = &entries[index].next;
			}
		}
	}
	
	
	
	void Remove_All()
	{
		for (uint hash = 0; hash < maxHashCount; hash++)
		{
			uint index = indices[hash];
			if (index != -1)
			{
				uint lastHash = index;
				while (entries[lastHash].next != -1)
					lastHash = entries[lastHash].next;
				
				entries[lastHash].next = unusedEntryIndex;
				unusedEntryIndex = index;
				indices[hash] = -1;
			}
		}
	}



	uint Get_Size() const
	{
		uint result = 0;
		
		for (uint hash = 0; hash < maxHashCount; hash++)
		{
			uint index = indices[hash];
			while (index != -1)
			{
				++result;
				index = entries[index].next;
			}
		}

		return result;
	}

	const Value getValueByIndex(int index) const
	{
		Value value = entries[index].value;
		return value;
	}

	void _validate()
	{
		for (uint hash = 0; hash < maxHashCount; hash++)
		{
			uint index = indices[hash];
			while (index != -1)
			{
				TT_ASSERT(computeHash(entries[index].key) == hash);
				index = entries[index].next;
			}
		}
	}



	static void _test()
	{
		HashTemplateClass<uint, uint> table;
		
		for (uint run = 0; run < 10; run++)
		{
			bool bits[1234];
			for (uint i = 0; i < 1234; i++)
			{
				bits[i] = rand() > (RAND_MAX & 1);
				if (bits[i])
					table.Insert(i*45, i*2+5678);
			}

			for (uint i = 0; i < 1234; i++)
			{
				TT_ASSERT(bits[i] == table.Exists(i*45));
				if (bits[i])
					TT_ASSERT(*table.Get(i*45) == i*2+5678);
			}

			for (uint i = 0; i < 1234/2; i++)
			{
				if (rand() > (RAND_MAX & 1))
				{
					bits[i] = !bits[i];
					if (bits[i])
						table.Insert(i*45, i*2+5678);
					else
						table.Remove(i*45);
				}
			}

			for (uint i = 0; i < 1234; i++)
			{
				TT_ASSERT(bits[i] == table.Exists(i*45));
				if (bits[i])
					TT_ASSERT(*table.Get(i*45) == i*2+5678);
			}

			table.Remove_All();

			for (uint i = 0; i < 1234; i++)
				TT_ASSERT(table.Exists(i*45) == false);
		}

		TT_INTERRUPT; // Test success.
	}

}; // 0010



#endif