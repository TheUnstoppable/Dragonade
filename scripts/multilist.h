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
#ifndef SCRIPTS_INCLUDE__MULTILIST_H_
#define SCRIPTS_INCLUDE__MULTILIST_H_

#include "engine_vector.h"

class GenericMultiListClass;
class MultiListObjectClass;
class GenericMultiListIterator;

class MultiListNodeClass : public AutoPoolClass<MultiListNodeClass, 256> 
{
public:
	MultiListNodeClass() 
	{ 
		Prev = Next = NextList = 0; 
		Object = 0; 
		List = 0; 
	};

	MultiListNodeClass* Prev;			// Previous object in list	0000
	MultiListNodeClass* Next;			// Next object in list		0004
	MultiListNodeClass* NextList;		// Next list for object		0008
	MultiListObjectClass* Object;		// Pointer to object		000C
	GenericMultiListClass* List;		// List for this node		0010
}; // 0014

class MultiListObjectClass
{
private:
	MultiListNodeClass* ListNode; // 0004

public:
	MultiListObjectClass(): ListNode(NULL) {};
	virtual ~MultiListObjectClass();

	MultiListNodeClass* Get_List_Node() const
	{
		return ListNode;
	};

	void Set_List_Node(MultiListNodeClass *node)
	{
		ListNode = node;
	};

};  // 0008

class GenericMultiListClass 
{
private:
	MultiListNodeClass Head; // 0004

	// not implemented
	GenericMultiListClass(const GenericMultiListClass&);
	GenericMultiListClass& operator = (const GenericMultiListClass&); 

protected:
	friend class GenericMultiListIterator;
	friend class MultiListObjectClass;
	SHARED_API bool					Internal_Add(MultiListObjectClass *obj, bool onlyonce = true);
	SHARED_API bool					Internal_Add_Tail(MultiListObjectClass *obj, bool onlyonce = true);
	SHARED_API bool					Internal_Add_After(MultiListObjectClass *obj, const MultiListObjectClass* existing_list_member, bool onlyonce = true);
	SHARED_API bool					Internal_Remove(MultiListObjectClass *obj);
	SHARED_API MultiListObjectClass*	Internal_Remove_List_Head();

	inline MultiListObjectClass* Internal_Get_List_Head() const
	{
		if (Is_Empty()) return 0;
		TT_ASSERT(Head.Next->Object != NULL);
		return Head.Next->Object;
	};

public:
	GenericMultiListClass() 
	{ 
		Head.Next = Head.Prev = &Head; 
		Head.Object = 0; 
		Head.NextList = 0; 
	};

	virtual ~GenericMultiListClass()
	{
		TT_ASSERT(Is_Empty());
	};

	inline bool Is_In_List(MultiListObjectClass *obj) const
	{
		return Contains(obj);
	};

	inline bool	Is_Empty() const
	{
		return (Head.Next == &Head); 
	};

	bool Contains(MultiListObjectClass * obj) const
	{
		TT_ASSERT(obj);
		for (MultiListNodeClass* node = obj->Get_List_Node(); node; node = node->NextList)
		{
			if (node->List == this)
			{
				return true;
			};
		};
		return false;
	}

	int Count() const;
}; // 0018

class GenericMultiListIterator
{
public:
	GenericMultiListIterator(const GenericMultiListClass *list)	
	{ 
		TT_ASSERT(list); 
		First(list); 
	};

	void First(const GenericMultiListClass *list)
	{ 
		List = list; 
		CurNode = List->Head.Next; 
	};

	void First()
	{ 
		CurNode = List->Head.Next; 
	};

	void Next()
	{ 
		CurNode = CurNode->Next; 
	};

	void Prev()
	{ 
		CurNode = CurNode->Prev; 
	};

	bool Is_Done() const
	{ 
		return (CurNode == &(List->Head)); 
	};
	
protected:
	MultiListObjectClass* Current_Object() const
	{ 
		return CurNode->Object; 
	};

	const GenericMultiListClass*	List;				// list we're working in
	const MultiListNodeClass*		CurNode;			// node we're currently at.
};

template <typename T> class MultiListIterator;
template <typename T> class MultiListClass : public GenericMultiListClass 
{
private:
	// not implemented
	MultiListClass(const MultiListClass & that);
	MultiListClass & operator = (const MultiListClass & that);

public:
	MultiListClass(): GenericMultiListClass() {};

	virtual ~MultiListClass()
	{
	 	while (!Is_Empty()) 
		{
			Remove_Head();
		}
	};

	bool Add(T* obj, bool onlyonce = true)
	{
		return Internal_Add(obj, onlyonce);
	};

	bool Add_Tail(T* obj, bool onlyonce = true)
	{
		return Internal_Add_Tail(obj, onlyonce);
	};

	bool Add_After(T* obj, const T* existing_list_member, bool onlyonce = true)
	{
		return Internal_Add_After(obj, existing_list_member, onlyonce);
	};

	bool Remove(T* obj)
	{
		return Internal_Remove(obj);
	};

	T* Get_Head() const
	{
		return (T*) Internal_Get_List_Head();
	};

	T* Peek_Head() const
	{
		return (T*) Internal_Get_List_Head();
	};

	T* Remove_Head()
	{
		return (T*) Internal_Remove_List_Head();
	};

	void Reset_List()
	{
		while (Peek_Head() != NULL) 
		{
			Remove_Head();
		};
	};

	MultiListIterator<T> Iterator() const;
}; // 0018

template <typename T> class MultiListIterator : public GenericMultiListIterator
{
public:
	MultiListIterator(const MultiListClass<T> *list) : GenericMultiListIterator(list) {};

	T* Get_Obj(void) const
	{
		return (T*)Current_Object();
	};

	T* Peek_Obj(void) const
	{
		return (T*)Current_Object();
	};

	void Remove_Current_Object()
	{
		T* obj = Peek_Obj();
		if (obj != NULL) {
			Next();
			((MultiListClass<T>*)List)->Remove(obj);
		}
	};


	void operator++() { Next(); }
	operator bool() const { return !Is_Done(); }
	T* operator->() { return Get_Obj(); }
	T* operator*() { return Get_Obj(); }

};

template <typename T> MultiListIterator<T> MultiListClass<T>::Iterator() const
{
	return MultiListIterator<T>(this);
};

template <typename T> class RefMultiListIterator;
template <typename T> class RefMultiListClass: public GenericMultiListClass 
{
private:
	// not implemented
	RefMultiListClass(const RefMultiListClass&);
	RefMultiListClass& operator = (const RefMultiListClass&); 

public:
	RefMultiListClass(): GenericMultiListClass() {};

	virtual ~RefMultiListClass()
	{
	 	while (!Is_Empty()) 
		{
			Release_Head();
		}
	};

	bool Add(T* obj, bool onlyonce = true)
	{
		bool res = Internal_Add(obj, onlyonce);
		if (res == true) obj->Add_Ref();
		return res;
	};

	bool Add_Tail(T* obj, bool onlyonce = true)
	{
		bool res = Internal_Add_Tail(obj, onlyonce);
		if (res == true) obj->Add_Ref();
		return res;
	};

	bool Add_After(T* obj, const T* existing_list_member, bool onlyonce = true)
	{
		bool res = Internal_Add_After(obj, existing_list_member, onlyonce);
		if (res == true) obj->Add_Ref();
		return res;
	};

	bool Remove(T* obj)
	{
		bool res = Internal_Remove(obj);
		if (res == true) obj->Release_Ref();
		return res;
	};

	bool Release_Head()
	{
		T* obj = (T*) Internal_Remove_List_Head();
		if (obj)
		{
			obj->Release_Ref();
			return true;
		}
		return false;
	};

	T* Get_Head() const
	{
		T* obj = (T*) Internal_Get_List_Head();
		if (obj) obj->Add_Ref();
		return obj;
	};

	T* Peek_Head() const
	{
		return (T*) Internal_Get_List_Head();
	};

	T* Remove_Head()
	{
		return (T*) Internal_Remove_List_Head();
	};

	void Reset_List()
	{
		while (Peek_Head() != NULL) 
		{
			Release_Head();
		};
	};

	RefMultiListIterator<T> Iterator() const;
}; // 0018  0018  001C  0018

template <typename T> class RefMultiListIterator : public GenericMultiListIterator
{
public:
	RefMultiListIterator(const RefMultiListClass<T> *list): GenericMultiListIterator(list) {};

	T* Get_Obj() const
	{
		T* obj = (T*)Current_Object();
		if (obj != NULL) obj->Add_Ref();
		return obj;
	};

	T* Peek_Obj() const
	{
		return (T*)Current_Object();
	};

	void Remove_Current_Object()
	{
		T* obj = Peek_Obj();
		if (obj != NULL) {
			Next();
			((RefMultiListClass<T> *)List)->Remove(obj);
		};
	};

	void operator++() { Next(); }
	operator bool() const { return !Is_Done(); }
	T* operator->() const { return Peek_Obj(); }

};

template <typename T> RefMultiListIterator<T> RefMultiListClass<T>::Iterator() const
{
	return RefMultiListIterator<T>(this);
};

TT_INLINE MultiListObjectClass::~MultiListObjectClass()
{
	while (ListNode) 
	{
		ListNode->List->Internal_Remove(this);
	};
}

TT_INLINE int GenericMultiListClass::Count() const
{
	int counter = 0;
	GenericMultiListIterator it(this);
	for (it.First(); !it.Is_Done(); it.Next()) 
	{
		++counter;
	}
	return counter;
}


template<class ObjectType> class PriorityMultiListIterator :
	public MultiListIterator<ObjectType>
{
public:
	PriorityMultiListIterator(MultiListClass<ObjectType>* list)
		:	OriginalHead (NULL),
			MultiListIterator<ObjectType>(list)
	{
		First();
	}

	bool Process_Head(ObjectType** object)
	{
		bool result = false;

		//	Check to ensure we don't wrap around the list (stop after iterating
		// the list once).
		if (CurNode != NULL && CurNode->Object != NULL && OriginalHead != CurNode) {
			if (!OriginalHead)
				OriginalHead = CurNode;
			*object = (ObjectType*)CurNode->Object;

			// Remove the node from the head of the list and
			// add it to the tail of the list
			Remove_Current_Object();
			((MultiListClass<ObjectType> *)PriorityMultiListIterator::List)->Add_Tail ((*object));

			result = true;
		}

		return result;
	}
	
protected:
	
	const MultiListNodeClass* OriginalHead;
};


#endif