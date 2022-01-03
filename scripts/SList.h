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
#ifndef TT_INCLUDE__SLIST_H
#define TT_INCLUDE__SLIST_H



#include "engine_vector.h"



class GenericSLNode :
	public AutoPoolClass<GenericSLNode, 256>
{

protected:

	void Internal_Set_Next(void *n)
	{
		NodeNext = n;
	}
	void Internal_Set_Data(void *d)
	{
		NodeData = d;
	}
	void* NodeNext; // 0
	void* NodeData; // 4
	GenericSLNode(void *obj)
	{
		NodeData = obj;
		NodeNext = NULL;
	}
	GenericSLNode(void* data, GenericSLNode* next)
	{
		NodeData = data;
		NodeNext = next;
	}
};

template<class T> class SList;
template<class T>
class SLNode : public GenericSLNode
{
public:

	friend class SList<T>;
	void Set_Next(SLNode<T>* n)
	{
		NodeNext = n;
	}
	SLNode<T> *Next() const
	{
		return (SLNode<T>*)NodeNext;
	}
	T* Data() const
	{
		return (T*)NodeData;
	}
protected:
	SLNode(T *obj) : GenericSLNode(obj) {}
	SLNode(T* data, SLNode<T>* next) : GenericSLNode(data, next) {}
private:
	SLNode() {}
};



template<class T>
class SList
{
private:
	SLNode<T>* HeadNode;
	SLNode<T>* TailNode;
public:
	SList() :
		HeadNode(0), TailNode(0)
	{
	}

	virtual ~SList()
	{
		Remove_All();
	}


	virtual bool Add_Head(T* data)
	{
		if (!data)
		{
			return false;
		}
		SLNode<T> *temp	= new SLNode<T>(data);
		temp->Set_Next(HeadNode);
		HeadNode = temp;
		if (!TailNode)
		{
			TailNode = temp;
		}
		return true;
	}


	virtual bool Add_Head(SList<T>& list) 
	{
		if (list.HeadNode == NULL)
		{
			return false;
		}
		SLNode<T> *addpoint = NULL;
		for (SLNode<T> *cur = list.HeadNode; cur; cur = cur->Next())
		{
			if (addpoint)
			{
				SLNode<T> *temp = new SLNode<T>(cur->Data());
				temp->Set_Next(addpoint->Next());
				addpoint->Set_Next(temp);
				addpoint = temp;
			}
			else
			{
				Add_Head(cur->Data());
				addpoint = HeadNode;
			}
		}
		return true;
	}


	virtual bool Add_Tail(T* data)
	{
		if (data == NULL)
		{
			return false;
		}
		SLNode<T> *temp = new SLNode<T> (data);
		if (HeadNode == NULL)
		{
			HeadNode = TailNode = temp;
		}
		else
		{
			TailNode->Set_Next(temp);
			TailNode = temp;
		}
		return true;
	}


	virtual bool Add_Tail(SList<T>& list)
	{
		if (list.HeadNode == NULL)
		{
			return false;
		}
		for (SLNode<T> *cur = list.HeadNode; cur; cur = cur->Next()) 
		{
			Add_Tail(cur->Data());
		}
		return true;
	}


	virtual T* Remove_Head()
	{
		if (HeadNode == NULL)
		{
			return ((T* )NULL);
		}
		SLNode<T> *temp = HeadNode;
		HeadNode = HeadNode->Next();
		if (HeadNode == NULL)
		{
			TailNode = NULL;
		}
		T *data = temp->Data();
		delete temp;
		return data;
	}


	virtual T* Remove_Tail()
	{
		if (HeadNode == NULL)
		{
			return ((T *)NULL);
		}
		T* data = TailNode->Data();
		return (Remove(data) ? data : (T*)NULL);
	}


	virtual bool Remove(const T* element)
	{
		if (element == NULL || HeadNode == NULL)
		{
			return false;
		}
		if (HeadNode->Data() == element)
		{
			return Remove_Head() != NULL ? true : false;
		}
		SLNode<T> *cur;
		for (cur = HeadNode; cur->Next() && cur->Next()->Data() != element; cur=cur->Next())
		{
		}
		if (cur->Next() != NULL && cur->Next()->Data() == element)
		{
			SLNode<T> *temp	= cur->Next();
			cur->Set_Next(temp->Next());
			if (temp == TailNode)
			{
				TailNode = cur;
			}
			delete temp;
			return true;
		}
		return false;
	}


	virtual void Remove_All()
	{
		SLNode<T> *next;
		for (SLNode<T> *cur = HeadNode; cur; cur = next)
		{
			next = cur->Next();
			delete cur;
		}
		HeadNode = TailNode = NULL;
	}


	virtual bool Insert_Before(T* newnode, const T* oldnode)
	{
		if (newnode == NULL)
		{
			return false;
		}
		if (oldnode == NULL || HeadNode == NULL || HeadNode->Data() == oldnode)
		{
			return Add_Head(newnode);
		}
		SLNode<T> *cur;
		for (cur=HeadNode; cur->Next() && cur->Next()->Data() != oldnode; cur=cur->Next())
		{
		}
		if (cur->Next() != NULL && cur->Next()->Data() == oldnode)
		{
			SLNode<T> *temp	= new SLNode<T> (newnode);
			temp->Set_Next(cur->Next());
			cur->Set_Next(temp);
			return true;
		}
		return false;
	}

	virtual bool Insert_After(T* newnode, const T* oldnode)
	{
		if (newnode == NULL)
		{
			return false;
		}
		if (oldnode == NULL || HeadNode == NULL)
		{
			return(Add_Head(newnode));
		}
		SLNode<T> *cur;
		for (cur = HeadNode; cur && cur->Data() != oldnode; cur = cur->Next())
		{
		}
		if (cur != NULL  && cur->Data() == oldnode)
		{
			if (cur == TailNode)
			{
				return(Add_Tail(newnode));
			}
			SLNode<T> *temp = new SLNode<T>(newnode);
			temp->Set_Next(cur->Next());
			cur->Set_Next(temp);
			return true;
		}
		return false;
	}


	virtual bool Is_Empty() const
	{
		return !HeadNode;
	}


	virtual uint32 Get_Count() const
	{
		uint32 count = 0;

		for (SLNode<T>* node = HeadNode; node; node = node->Next())
			++count;

		return count;
	}


	SLNode<T>* Head() const
	{
		return HeadNode;
	}

	SLNode<T>* Tail() const
	{
		return TailNode;
	}
	
	SLNode<T> *Find_Node(T * data) const
	{
		SLNode<T> * cur;
		for (cur = HeadNode;cur && cur->Data() != data;cur = cur->Next())
		{
		}
		return cur;
	}

	void insertBefore(T* data, SLNode<T>& node)
	{
		// Instead of adding before (which we can't), move the current data to the next node and replace the current node.
		SLNode<T>* nextNode = new SLNode<T>(node.Data(), node.Next());
		node.NodeNext = nextNode;
		node.NodeData = data;
		if (&node == TailNode)
			TailNode = nextNode;
	}

	void insertAfter(T* data, SLNode<T>& node)
	{
		SLNode<T>* nextNode = new SLNode<T>(data, node.Next());
		node.NodeNext = nextNode;
		if (&node == TailNode)
			TailNode = nextNode;
	}

}; // 000C

template <typename T>
class SLIterator
{
public:
	SLIterator(SList<T>& list): Node(list.Head()) { };

	bool IsDone() const { return !Node->Next(); }
	T* operator ->() { return Node->Data(); }
	T* operator * () { return Node->Data(); }
	SLIterator<T>& operator ++ () { Node = Node->Next(); }

private:
	SLNode<T>*	Node;
};

#endif