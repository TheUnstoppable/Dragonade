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
#ifndef TT_INCLUDE_DATASAFE_H
#define TT_INCLUDE_DATASAFE_H
#include "SysTimeClass.h"
#if (PARAM_EDITING_ON) || (DDBEDIT) || (W3DVIEWER)
#define safe_int int
#define safe_unsigned_int unsigned int
#define safe_long long
#define safe_unsigned_long unsigned long
#define safe_float float
#define safe_double double
#else //PARAM_EDITING_ON
#define safe_int SafeDataClass<int>
#define safe_unsigned_int SafeDataClass<unsigned int>
#define safe_long SafeDataClass<int>
#define safe_unsigned_long SafeDataClass<unsigned int>
#define safe_float SafeDataClass<float>
#define safe_double SafeDataClass<double>
#define MAX_ENTRIES_PER_LIST 16
#define MAX_DATASAFE_LISTS 8192
#define MAX_DATASAFE_TYPES 128
#define MAX_OBJECT_COPIES 8
#define DATASAFE_TIME_CHECK_CALLS 100000
#define TICKS_PER_SECOND 1000
#define SECURITY_CHECK_TIME (TICKS_PER_SECOND * 15)
#define SENTINEL_ONE 0x29874362
#define SENTINEL_TWO 0x0bad0bad
template <typename T, int size> class RefArray
{
protected:
	char _dummy[size * sizeof(T)];
public:
	operator T* ()
	{
		return (T*) this;
	};
	operator const T* () const
	{
		return (T*) this;
	};
	T* operator & ()
	{
		return (T*) this;
	};
	const T* operator & () const
	{
		return (T*) this;
	};
};
class DataSafeHandleClass
{
public:
	DataSafeHandleClass(int val = 0)
	{
		Handle.Whole.WholeHandle = val;
	}
	bool Is_Valid(void) const
	{
		return Handle.Whole.WholeHandle != -1;
	}
	inline int operator =(const int &val)
	{
		Handle.Whole.WholeHandle = val;
		return val;
	}
	inline operator int(void)
	{
		return Handle.Whole.WholeHandle;
	}
	inline operator int(void) const
	{
		return Handle.Whole.WholeHandle;
	}
	inline int &operator() (void)
	{
		return Handle.Whole.WholeHandle;
	}
	union tHandle
	{
		struct
		{
			int WholeHandle;
		} Whole;
		struct
		{
			unsigned char ID;
			unsigned char Type;
			unsigned short List;
		} Part;
	} Handle;
};

#pragma pack(push)
#pragma pack(4)
class DataSafeEntryClass
{
public:
	DataSafeHandleClass Handle;
	DataSafeEntryClass *Next;
	DataSafeEntryClass *Prev;
	unsigned long Size;
	bool IsSlop;
};

template<class T>
class DataSafeEntryDataClass : public DataSafeEntryClass
{
public:
	T Data;
};

#pragma pack(pop)
class DataSafeEntryListClass
{
public:
	DataSafeEntryListClass(void)
	{
		SafeList = NULL;
		EntryCount = 0;
		EntryType = -1;
		SlopCount = 0;
		memset(HandleIDUsage, 0, sizeof(HandleIDUsage));
	};
	DataSafeEntryClass *SafeList;
	int EntryCount;
	int EntryType;
	int SlopCount;
	char HandleIDUsage[MAX_ENTRIES_PER_LIST];
};

class DataSafeEntryTypeClass
{
public:
	DataSafeEntryTypeClass(void)
	{
		TypeCode = 0;
		ID = -1;
		Size = 0;
	};
	unsigned long TypeCode;
	int ID;
	int Size;
};

class GenericDataSafeClass
{
public:
	GenericDataSafeClass(void);
	SCRIPTS_API static DataSafeEntryClass *Get_Entry(DataSafeHandleClass handle);
	SCRIPTS_API static int Get_Entry_Type(DataSafeHandleClass handle);
	SCRIPTS_API static void Reset_Timers(void);
	SCRIPTS_API static void Dump_Safe_Stats(char *dump_buffer, int buffer_size);
	SCRIPTS_API static void Print_Safe_Stats_To_Debug_Output(void);
	SCRIPTS_API static void Reset(void);
	static void Set_Preferred_Thread(unsigned int thread_id) {PreferredThread = thread_id;};
protected:
	SCRIPTS_API void Shutdown(void);
	SCRIPTS_API static DataSafeEntryClass *Get_Entry_By_Index(int list, int index);
	SCRIPTS_API static void Shuffle(bool forced = false);
	SCRIPTS_API static void Swap_Entries(DataSafeEntryClass *first, DataSafeEntryClass *second, int type);
	SCRIPTS_API static void Encrypt(void *data, int size, unsigned long key = SimpleKey, bool do_checksum = true);
	SCRIPTS_API static void Decrypt(void *data, int size, unsigned long key = SimpleKey, bool do_checksum = true);
	SCRIPTS_API static void Mem_Copy_Encrypt(void *dest, void *src, int size, bool do_checksum);
	SCRIPTS_API static void Mem_Copy_Decrypt(void *dest, void *src, int size, bool do_checksum);
	static __forceinline void Security_Check(void);
	static __forceinline void Security_Fault(void);
	SCRIPTS_API static void Say_Security_Fault(void);
	SCRIPTS_API static int Get_Random_List_For_Insertion(int type);
	SCRIPTS_API static void Random_Insertion(DataSafeEntryClass *entry, int list, int type, bool is_slop = false);
	SCRIPTS_API static int Create_Safe_List(int type);
	SCRIPTS_API static int Get_Handle_ID(int list);
	SCRIPTS_API static void Remove_From_List(int list, DataSafeEntryClass *entry_ptr);
	SCRIPTS_API static void Free_Handle_ID(int list, int id);
	static inline int Get_Type_Size(int type);
	SCRIPTS_API static REF_DECL(unsigned int, PreferredThread);
	SCRIPTS_API static REF_DECL(unsigned long, SimpleKey);
	SCRIPTS_API static REF_DECL(unsigned long, HandleKey);
	SCRIPTS_API static REF_DECL(int, NumLists);
	SCRIPTS_API static REF_DECL(TT_NOOP(RefArray<DataSafeEntryListClass *, MAX_DATASAFE_LISTS>), Safe);
	SCRIPTS_API static REF_DECL(unsigned long, Checksum);
	SCRIPTS_API static REF_DECL(unsigned long, ShuffleDelay);
	SCRIPTS_API static REF_DECL(unsigned long, SecurityCheckDelay);
	SCRIPTS_API static REF_DECL(TT_NOOP(RefArray<DataSafeEntryTypeClass, MAX_DATASAFE_TYPES>), TypeList);
	SCRIPTS_API static REF_DECL(int, TypeListCount);
	SCRIPTS_API static REF_DECL(DataSafeHandleClass, SentinelOne);
	SCRIPTS_API static REF_DECL(DataSafeHandleClass, SentinelTwo);
	SCRIPTS_API static REF_DECL(int, CRCErrors);
};

template<class T>
class DataSafeClass : public GenericDataSafeClass
{
public:
	DataSafeClass(T *bogus_ptr = NULL, int slopcount = 3);
	~DataSafeClass(void);
	static bool Get(DataSafeHandleClass handle, T* &item);
	static inline bool In_Return_List(T *ptr)
	{
		if (ptr)
		{
			void *temp = (void*)ptr;
			if (temp >= &(ReturnList[0][0]) && temp < &(ReturnList[MAX_OBJECT_COPIES][0])) {
				if (((unsigned long) temp - (unsigned long)(&(ReturnList[0][0]))) % sizeof(T) == 0)
				{
					return true;
				}
			}
		}
		return false;
	};
	static bool Set(DataSafeHandleClass handle, T *data);
	static DataSafeHandleClass Add_Entry(T &value, bool is_slop = false);
	static void Delete_Entry(DataSafeHandleClass handle);
private:
	static int Get_Type_ID(unsigned long type_code, int size);
	static unsigned long Get_Type_Code(void);
	SCRIPTS_API static REF_DECL(int, Type);
	SCRIPTS_API static REF_DECL(int, MinSlop);
	SCRIPTS_API static REF_DECL(TT_NOOP(RefArray<RefArray<char, sizeof(T)>, MAX_OBJECT_COPIES>), ReturnList);
	SCRIPTS_API static REF_DECL(int, ReturnIndex);
};

SCRIPTS_API extern char ErrorVal[1024]; //DA
template<class T>
class SafeDataClass
{
public:
	SafeDataClass(void);
	SafeDataClass(T data);
	~SafeDataClass(void);
	SafeDataClass(SafeDataClass &)
	{
		TT_ASSERT(false);
	};
	inline T* Get_Ptr(void) const;
	inline bool Commit(T* data_ptr) const;
	inline T &operator() (void) const;
	inline T &operator = (T const &data);
	inline T &operator = (SafeDataClass<T> &safedata);
	inline operator T() const;
	inline bool operator == (T const &data);
	inline bool operator == (SafeDataClass<T> &safedata);
	inline bool operator != (T const &data);
	inline bool operator != (SafeDataClass<T> &safedata);
	inline bool operator > (T const &data);
	inline bool operator > (SafeDataClass<T> &safedata);
	inline bool operator >= (T const &data);
	inline bool operator >= (SafeDataClass<T> &safedata);
	inline bool operator < (T const &data);
	inline bool operator < (SafeDataClass<T> &safedata);
	inline bool operator <= (T const &data);
	inline bool operator <= (SafeDataClass<T> &safedata);
	inline T &operator + (T const &data);
	inline T &operator + (SafeDataClass<T> &safedata);
	inline T &operator += (T const &data);
	inline T &operator += (SafeDataClass<T> &safedata);
	inline T &operator - (T const &data);
	inline T &operator - (SafeDataClass<T> &safedata);
	inline T &operator -= (T const &data);
	inline T &operator -= (SafeDataClass<T> &safedata);
	inline T &operator * (T const &data);
	inline T &operator * (SafeDataClass<T> &safedata);
	inline T &operator *= (T const &data);
	inline T &operator *= (SafeDataClass<T> &safedata);
	inline T &operator / (T const &data);
	inline T &operator / (SafeDataClass<T> &safedata);
	inline T &operator /= (T const &data);
	inline T &operator /= (SafeDataClass<T> &safedata);
	inline T &operator ++ (void);
	inline T &operator -- (void);
	inline T &operator ++ (int);
	inline T &operator -- (int);
	DataSafeHandleClass Get_Handle(void)
	{
		return(Handle);
	}
	T Get(void);
	void Set(T);
private:
	DataSafeHandleClass Handle;
};

inline int GenericDataSafeClass::Get_Type_Size(int type)
{
	return TypeList[type].Size;
}

__forceinline void GenericDataSafeClass::Security_Fault(void)
{
	CRCErrors++;
	Say_Security_Fault();
}

__forceinline void GenericDataSafeClass::Security_Check(void)
{
	static bool _checking = false;
	static unsigned long _calls = 0;
	_calls++;
	if (_calls < DATASAFE_TIME_CHECK_CALLS)
	{
		return;
	}
	_calls = 0;
	if (HandleKey - SimpleKey == HandleKey)
	{
		Security_Fault();
	}
	if (SimpleKey - HandleKey == SimpleKey)
	{
		Security_Fault();
	}
	unsigned long time = TIMEGETTIME();
	if (time < SecurityCheckDelay || (time | SecurityCheckDelay) == 0 || (time - SecurityCheckDelay) > SECURITY_CHECK_TIME)
	{
		if (!_checking)
		{
			_checking = true;
			SecurityCheckDelay = time;
			unsigned long checkey = ~SimpleKey;
			for (int i=0 ; i<NumLists ; i++)
			{
				if (Safe[i] != NULL && Safe[i]->EntryCount > 0)
				{
					DataSafeEntryClass *entry_ptr = Safe[i]->SafeList;
					unsigned long *data = NULL;
					int data_size = entry_ptr->Size;
					data_size = data_size >> 2;
					if (entry_ptr)
					{
						for (int j=0 ; j<Safe[i]->EntryCount ; j++)
						{
							checkey ^= entry_ptr->Handle;
							data = (unsigned long *) (((char*)entry_ptr) + sizeof(*entry_ptr));
							for (int z=0 ; z<data_size ; z++)
							{
								checkey ^= *data++;
							}
							entry_ptr = entry_ptr->Next;
						}
					}
				}
			}
			if (checkey != Checksum)
			{
				Security_Fault();
			}
			if (NumLists)
			{
				int *val = NULL;
				if ((int)SentinelOne != 0)
				{
					bool got = DataSafeClass<int>::Get(SentinelOne, val);
					if (!got || val == NULL || *val != SENTINEL_ONE)
					{
						Security_Fault();
					}
				}
				val = NULL;
				if ((int)SentinelTwo != 0)
				{
					bool got = DataSafeClass<int>::Get(SentinelTwo, val);
					if (!got || val == NULL || *val != SENTINEL_TWO)
					{
						Security_Fault();
					}
				}
			}
			_checking = false;
		}
	}
}

template <class T>
DataSafeClass<T>::DataSafeClass(T*, int slopcount)
{
	memset(ReturnList, 0, MAX_OBJECT_COPIES * sizeof(T));
	ReturnIndex = 0;
	int data_size = sizeof(T);
	int type_code = Get_Type_Code();
	int id = Get_Type_ID(type_code, data_size);
	Type = id;
	MinSlop = slopcount;
	for (int i=0 ; i<slopcount ; i++)
	{
#pragma warning(suppress: 6255) // _alloca indicates failure by raising a stack overflow exception
#pragma warning(suppress: 6263) // warning c6263: Using alloca in a loop
		void *stackmem = _alloca(sizeof(T));
		T *slop_ptr = new (stackmem) T;
		Add_Entry(*slop_ptr, true);
	}
}

template <class T>
DataSafeClass<T>::~DataSafeClass(void)
{
	for (int i=0 ; i<NumLists ; i++)
	{
		if (Safe[i] != NULL && Safe[i]->EntryType == Type && Safe[i]->EntryCount > 0)
		{
			while (Safe[i]->EntryCount > 0)
			{
				int entries = Safe[i]->EntryCount;
				DataSafeHandleClass handle = Safe[i]->SafeList->Handle ^ SimpleKey;
				handle = handle ^ HandleKey;
				Delete_Entry(handle);
				if (entries != Safe[i]->EntryCount+1)
				{
					break;
				}
			}
		}
	}
	Shutdown();
}

template <class T>
unsigned long DataSafeClass<T>::Get_Type_Code(void)
{
    static int dummy;
    return (unsigned long)&dummy;
}

template <class T>
int DataSafeClass<T>::Get_Type_ID(unsigned long type_code, int size)
{
	int id = 0;
	for (int i=0 ; i<TypeListCount ; i++)
	{
		if (TypeList[i].TypeCode == type_code)
		{
			return(TypeList[i].ID);
		}
	}
	if (TypeListCount < MAX_DATASAFE_TYPES)
	{
		TypeList[TypeListCount].TypeCode = type_code;
		TypeList[TypeListCount].ID = TypeListCount;
		TypeList[TypeListCount].Size = size;
		id = TypeListCount++;
	}
	return id;
}

template <class T>
DataSafeHandleClass DataSafeClass<T>::Add_Entry(T &value, bool is_slop)
{
	Security_Check();
	DataSafeEntryClass *entry_ptr = (DataSafeEntryClass*) (new char [sizeof(DataSafeEntryDataClass<T>)]);
	if (entry_ptr)
	{
		entry_ptr->Next = NULL;
		entry_ptr->Prev = NULL;
		entry_ptr->Size = sizeof(value);
		entry_ptr->IsSlop = is_slop;
		DataSafeEntryDataClass<T>* data_ptr = (DataSafeEntryDataClass<T>*)  entry_ptr;
		Mem_Copy_Encrypt(& data_ptr->Data, &value, sizeof(value), true);
		int list = Get_Random_List_For_Insertion(Type);
		if (list != -1 && Safe[list] != NULL)
		{
			int index = Get_Handle_ID(list);
			DataSafeHandleClass handle;
			handle.Handle.Part.List = (unsigned short)list;
			handle.Handle.Part.ID = (unsigned char)index;
			handle.Handle.Part.Type = (unsigned char)Type;
			DataSafeHandleClass encrypted_handle = handle ^ SimpleKey;
			Checksum = Checksum ^ encrypted_handle;
			entry_ptr->Handle = encrypted_handle;
			Random_Insertion(entry_ptr, list, Type, is_slop);
			if (Safe[list]->EntryCount == 1 && !is_slop)
			{
				for (int i=0 ; i<MinSlop ; i++)
				{
					entry_ptr = (DataSafeEntryClass*) (new char [sizeof(DataSafeEntryDataClass<T>)]);
					if (entry_ptr)
					{
						entry_ptr->Next = NULL;
						entry_ptr->Prev = NULL;
						entry_ptr->Size = sizeof(T);
						entry_ptr->IsSlop = true;
						Encrypt(&((DataSafeEntryDataClass<T>*)entry_ptr)->Data, sizeof(T), SimpleKey, true);
						DataSafeHandleClass slop_handle;
						slop_handle.Handle.Part.ID = (unsigned char)Get_Handle_ID(list);
						slop_handle.Handle.Part.List = (unsigned short)list;
						slop_handle.Handle.Part.Type = (unsigned char)Type;
						DataSafeHandleClass encrypted_slop_handle = slop_handle ^ SimpleKey;
						Checksum = Checksum ^ encrypted_slop_handle;
						entry_ptr->Handle = encrypted_slop_handle;
						Random_Insertion(entry_ptr, list, Type, true);
					}
				}
			}
			if (Safe[list]->SlopCount != 0 && Safe[list]->EntryCount > Safe[list]->SlopCount * 2 && !is_slop)
			{
				entry_ptr = Safe[list]->SafeList;
				DataSafeHandleClass removal_list[MAX_ENTRIES_PER_LIST];
				int removal_count = 0;
				if (entry_ptr)
				{
					for (int j=0 ; j<Safe[list]->EntryCount ; j++)
					{
						if (entry_ptr->IsSlop)
						{
							DataSafeHandleClass decode_handle = entry_ptr->Handle ^ SimpleKey;
							decode_handle = decode_handle ^ HandleKey;
							removal_list[removal_count++] = decode_handle;
						}
						entry_ptr = entry_ptr->Next;
					}
				}
				for (int j=0 ; j<removal_count ; j++)
				{
					Delete_Entry(removal_list[j]);
				}
			}
			handle.Handle.Whole.WholeHandle ^= HandleKey;
			return handle;
		}
	}
	return (DataSafeHandleClass)-1;
}

template <class T>
void DataSafeClass<T>::Delete_Entry(DataSafeHandleClass handle)
{
	int list;
	int id;
	Security_Check();
	DataSafeHandleClass new_handle = handle ^ HandleKey;
	list = new_handle.Handle.Part.List;
	id = new_handle.Handle.Part.ID;
	DataSafeEntryClass *entry_ptr = Get_Entry(handle);
	if (entry_ptr != NULL)
	{
		DataSafeHandleClass match_handle = new_handle ^ SimpleKey;
		Checksum ^= match_handle;
		DataSafeEntryDataClass<T> *whole_entry_ptr = (DataSafeEntryDataClass<T>*) entry_ptr;
		Decrypt(&whole_entry_ptr->Data, Get_Type_Size(Safe[list]->EntryType), SimpleKey);
		if (!whole_entry_ptr->IsSlop)
		{
			whole_entry_ptr->Data.T::~T();
		}
		Remove_From_List(list, entry_ptr);
		Free_Handle_ID(list, id);
		delete [] (void*) entry_ptr;
		return;
	}
}

template <class T>
bool DataSafeClass<T>::Get(DataSafeHandleClass handle, T* &item)
{
	DataSafeEntryClass *entry_ptr = Get_Entry(handle);
	if (entry_ptr == NULL)
	{
		return(false);
	}
	DataSafeEntryDataClass<T> *data_entry_ptr = (DataSafeEntryDataClass<T>*) entry_ptr;
	Mem_Copy_Decrypt(&(ReturnList[ReturnIndex][0]), &data_entry_ptr->Data, sizeof(T), false);
	item = (T*)(&(ReturnList[ReturnIndex][0]));
	ReturnIndex++;
	if (ReturnIndex >= MAX_OBJECT_COPIES)
	{
		ReturnIndex = 0;
	}
	Shuffle();
	Security_Check();
	return(true);
}

template <class T>
bool DataSafeClass<T>::Set(DataSafeHandleClass handle, T *data)
{
	DataSafeEntryClass *entry_ptr = Get_Entry(handle);
	if (entry_ptr == NULL)
	{
		return(false);
	}
	DataSafeEntryDataClass<T> *data_entry_ptr = (DataSafeEntryDataClass<T>*) entry_ptr;
	Decrypt(&data_entry_ptr->Data, sizeof(T), SimpleKey, true);
	Mem_Copy_Encrypt(&data_entry_ptr->Data, data, sizeof(T), true);
	Shuffle();
	Security_Check();
	return true;
}

template <class T>
inline SafeDataClass<T>::SafeDataClass(T data)
{
	Handle = DataSafeClass<T>::Add_Entry(data);
}

template <class T>
SafeDataClass<T>::SafeDataClass(void)
{
#pragma warning(suppress: 6255) // _alloca indicates failure by raising a stack overflow exception
	void *stackmem = _alloca(sizeof(T));
#pragma warning(suppress: 28193) // warning C28193: The variable holds a value that must be examined
	T *data = new (stackmem) T;
#pragma warning(suppress: 6011) //warning C6011: dereferencing NULL pointer 'data'
	Handle = DataSafeClass<T>::Add_Entry(*data);
}

template <class T>
inline SafeDataClass<T>::~SafeDataClass(void)
{
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Delete_Entry(Handle);
	}
}

template <class T>
inline T &SafeDataClass<T>::operator = (T const &data)
{
	if (Handle.Is_Valid())
	{
			DataSafeClass<T>::Set(Handle, (T*) &data);
	}
	return (T&)data;
}

template <class T>
inline T &SafeDataClass<T>::operator = (SafeDataClass<T> &safedata)
{
	T *other_value = NULL;
	if (Handle.Is_Valid())
	{
		other_value = safedata.Get_Ptr();
		if (other_value)
		{
			DataSafeClass<T>::Set(Handle, (T*) other_value);
		}
		else
		{
			return (T&)*other_value;
		}
	}
#pragma warning(suppress: 6011) //warning C6011: dereferencing NULL pointer 'other_value'
	return (T&)*other_value;
}

template <class T>
inline bool SafeDataClass<T>::operator == (T const &data)
{
	T *data_ptr = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			return(*data_ptr == data);
		}
	}
	return false;
}

template <class T>
inline bool SafeDataClass<T>::operator == (SafeDataClass<T> &safedata)
{
	T *data_ptr = NULL;
	T *other_value = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			other_value = safedata.Get_Ptr();
			if (other_value)
			{
				return *data_ptr == *other_value;
			}
		}
	}
	return false;
}

template <class T>
inline bool SafeDataClass<T>::operator != (T const &data)
{
	T *data_ptr = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			return *data_ptr != data;
		}
	}
	return true;
}

template <class T>
inline bool SafeDataClass<T>::operator != (SafeDataClass<T> &safedata)
{
	T *data_ptr = NULL;
	T *other_value = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			other_value = safedata.Get_Ptr();
			if (other_value)
			{
				return(*data_ptr != *other_value);
			}
		}
	}
	return false;
}

template <class T>
inline bool SafeDataClass<T>::operator > (T const &data)
{
	T *data_ptr = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			return *data_ptr > data;
		}
	}
	return false;
}

template <class T>
inline bool SafeDataClass<T>::operator > (SafeDataClass<T> &safedata)
{
	T *data_ptr = NULL;
	T *other_value = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			other_value = safedata.Get_Ptr();
			if (other_value)
			{
				return *data_ptr > *other_value;
			}
		}
	}
	return false;
}

template <class T>
inline bool SafeDataClass<T>::operator >= (T const &data)
{
	T *data_ptr = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			return(*data_ptr >= data);
		}
	}
	return false;
}

template <class T>
inline bool SafeDataClass<T>::operator >= (SafeDataClass<T> &safedata)
{
	T *data_ptr = NULL;
	T *other_value = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			other_value = safedata.Get_Ptr();
			if (other_value)
			{
				return(*data_ptr >= *other_value);
			}
		}
	}
	return(false);
}
template <class T>
inline bool SafeDataClass<T>::operator < (T const &data)
{
	T *data_ptr = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			return *data_ptr < data;
		}
	}
	return false;
}
template <class T>
inline bool SafeDataClass<T>::operator < (SafeDataClass<T> &safedata)
{
	T *data_ptr = NULL;
	T *other_value = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			other_value = safedata.Get_Ptr();
			if (other_value)
			{
				return *data_ptr < *other_value;
			}
		}
	}
	return false;
}
template <class T>
inline bool SafeDataClass<T>::operator <= (T const &data)
{
	T *data_ptr = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			return *data_ptr <= data;
		}
	}
	return false;
}
template <class T>
inline bool SafeDataClass<T>::operator <= (SafeDataClass<T> &safedata)
{
	T *data_ptr = NULL;
	T *other_value = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			other_value = safedata.Get_Ptr();
			if (other_value)
			{
				return *data_ptr <= *other_value;
			}
		}
	}
	return false;
}
template <class T>
inline T &SafeDataClass<T>::operator + (T const &value)
{
	T *data_ptr = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			*data_ptr = *data_ptr + value;
			return (T&)*data_ptr;
		}
	}
	return (T&)ErrorVal[0];
}
template <class T>
inline T &SafeDataClass<T>::operator + (SafeDataClass<T> &safevalue)
{
	T *data_ptr = NULL;
	T *other_value = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			other_value = safevalue.Get_Ptr();
			if (other_value)
			{
				*data_ptr = *data_ptr + *other_value;
				return (T&)*data_ptr;
			}
			else
			{
				return (T&)*data_ptr;
			}
		}
	}
	return (T&)ErrorVal[0];
}
template <class T>
inline T &SafeDataClass<T>::operator += (T const &value)
{
	T *data_ptr = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			*data_ptr = *data_ptr + value;
			DataSafeClass<T>::Set(Handle, data_ptr);
			return (T&)*data_ptr;
		}
	}
	return (T&)ErrorVal[0];
}
template <class T>
inline T &SafeDataClass<T>::operator += (SafeDataClass<T> &safevalue)
{
	T *data_ptr = NULL;
	T *other_value = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			other_value = safevalue.Get_Ptr();
			if (other_value)
			{
				*data_ptr = *data_ptr + *other_value;
				DataSafeClass<T>::Set(Handle, data_ptr);
				return (T&)*data_ptr;
			}
			else
			{
				return (T&)*data_ptr;
			}
		}
	}
	return (T&)ErrorVal[0];
}
template <class T>
inline T &SafeDataClass<T>::operator - (T const &value)
{
	T *data_ptr = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			*data_ptr = *data_ptr - value;
			return (T&)*data_ptr;
		}
	}
	return (T&)ErrorVal[0];
}
template <class T>
inline T &SafeDataClass<T>::operator - (SafeDataClass<T> &safevalue)
{
	T *data_ptr = NULL;
	T *other_value = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			other_value = safevalue.Get_Ptr();
			if (other_value)
			{
				*data_ptr = *data_ptr - *other_value;
				return (T&)*data_ptr;
			}
			else
			{
				return (T&)*data_ptr;
			}
		}
	}
	return (T&)ErrorVal[0];
}
template <class T>
inline T &SafeDataClass<T>::operator -= (T const &value)
{
	T *data_ptr = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			*data_ptr = *data_ptr - value;
			DataSafeClass<T>::Set(Handle, data_ptr);
			return (T&)*data_ptr;
		}
	}
	return (T&)ErrorVal[0];
}
template <class T>
inline T &SafeDataClass<T>::operator -= (SafeDataClass<T> &safevalue)
{
	T *data_ptr = NULL;
	T *other_value = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			other_value = safevalue.Get_Ptr();
			if (other_value)
			{
				*data_ptr = *data_ptr - *other_value;
				DataSafeClass<T>::Set(Handle, data_ptr);
				return (T&)*data_ptr;
			}
			else
			{
				return (T&)*data_ptr;
			}
		}
	}
	return (T&)ErrorVal[0];
}
template <class T>
inline T &SafeDataClass<T>::operator * (T const &value)
{
	T *data_ptr = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			*data_ptr = *data_ptr * value;
			return (T&)*data_ptr;
		}
	}
	return (T&)ErrorVal[0];
}
template <class T>
inline T &SafeDataClass<T>::operator * (SafeDataClass<T> &safevalue)
{
	T *data_ptr = NULL;
	T *other_value = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			other_value = safevalue.Get_Ptr();
			if (other_value)
			{
				*data_ptr = *data_ptr * *other_value;
				return (T&)*data_ptr;
			}
			else
			{
				return (T&)*data_ptr;
			}
		}
	}
	return (T&)ErrorVal[0];
}
template <class T>
inline T &SafeDataClass<T>::operator *= (T const &value)
{
	T *data_ptr = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			*data_ptr = *data_ptr * value;
			DataSafeClass<T>::Set(Handle, data_ptr);
			return (T&)*data_ptr;
		}
	}
	return (T&)ErrorVal[0];
}
template <class T>
inline T &SafeDataClass<T>::operator *= (SafeDataClass<T> &safevalue)
{
	T *data_ptr = NULL;
	T *other_value = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			other_value = safevalue.Get_Ptr();
			if (other_value)
			{
				*data_ptr = *data_ptr * *other_value;
				DataSafeClass<T>::Set(Handle, data_ptr);
				return (T&)*data_ptr;
			}
			else
			{
				return (T&)*data_ptr;
			}
		}
	}
	return (T&)ErrorVal[0];
}
template <class T>
inline T &SafeDataClass<T>::operator / (T const &value)
{
	T *data_ptr = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			*data_ptr = *data_ptr / value;
			return (T&)*data_ptr;
		}
	}
	return (T&)ErrorVal[0];
}
template <class T>
inline T &SafeDataClass<T>::operator / (SafeDataClass<T> &safevalue)
{
	T *data_ptr = NULL;
	T *other_value = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			other_value = safevalue.Get_Ptr();
			if (other_value)
			{
				*data_ptr = *data_ptr / *other_value;
				return (T&)*data_ptr;
			}
			else
			{
				return (T&)*data_ptr;
			}
		}
	}
	return (T&)ErrorVal[0];
}
template <class T>
inline T &SafeDataClass<T>::operator /= (T const &value)
{
	T *data_ptr = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			*data_ptr = *data_ptr / value;
			DataSafeClass<T>::Set(Handle, data_ptr);
			return (T&)*data_ptr;
		}
	}
	return (T&)ErrorVal[0];
}
template <class T>
inline T &SafeDataClass<T>::operator /= (SafeDataClass<T> &safevalue)
{
	T *data_ptr = NULL;
	T *other_value = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			other_value = safevalue.Get_Ptr();
			if (other_value)
			{
				*data_ptr = *data_ptr / *other_value;
				DataSafeClass<T>::Set(Handle, data_ptr);
				return (T&)*data_ptr;
			}
			else
			{
				return (T&)*data_ptr;
			}
		}
	}
	return (T&)ErrorVal[0];
}
template <class T>
inline T &SafeDataClass<T>::operator ++ (void)
{
	T *data_ptr = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			(*data_ptr)++;
			DataSafeClass<T>::Set(Handle, data_ptr);
			return (T&)*data_ptr;
		}
	}
	return (T&)ErrorVal[0];
}
template <class T>
inline T &SafeDataClass<T>::operator -- (void)
{
	T *data_ptr = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			(*data_ptr)--;
			DataSafeClass<T>::Set(Handle, data_ptr);
			return (T&)*data_ptr;
		}
	}
	return (T&)ErrorVal[0];
}
template <class T>
inline T &SafeDataClass<T>::operator ++ (int)
{
	return this->operator ++ ();
}
template <class T>
inline T &SafeDataClass<T>::operator -- (int)
{
	return this->operator -- ();
}
template <class T>
inline T &SafeDataClass<T>::operator () (void) const
{
	T *data_ptr = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			return *data_ptr;
		}
	}
	static T oh_dear;
	return oh_dear;
}
template<class T>
inline SafeDataClass<T>::operator T() const
{
	return (*this)();
}
template <class T>
inline T *SafeDataClass<T>::Get_Ptr(void) const
{
	T *data_ptr = NULL;
	if (Handle.Is_Valid())
	{
		DataSafeClass<T>::Get(Handle, data_ptr);
		if (data_ptr)
		{
			return data_ptr;
		}
	}
	return NULL;
}
template <class T>
inline bool SafeDataClass<T>::Commit(T *data_ptr) const
{
	if (Handle.Is_Valid())
	{
		if (DataSafeClass<T>::In_Return_List(data_ptr))
		{
			return DataSafeClass<T>::Set(Handle, data_ptr);
		}
	}
	return false;
}
#endif	//PARAM_EDITING_ON
#endif
