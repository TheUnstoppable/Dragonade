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
#include "general.h"
#include "multilist.h"

bool GenericMultiListClass::Internal_Add(MultiListObjectClass *obj, bool onlyonce)
{
	TT_ASSERT(obj);

	if (onlyonce && Is_In_List(obj))
	{
		return false;
	}

PUSH_MEMORY_MACROS;
#undef new
	MultiListNodeClass *node = new MultiListNodeClass;
POP_MEMORY_MACROS;
	node->Object = obj;

	node->NextList = obj->Get_List_Node();
	obj->Set_List_Node(node);

	node->Prev = &(Head);
	node->Next = Head.Next;
	node->Next->Prev = node;
	node->Prev->Next = node;

	node->List = this;

	return true;
};

bool GenericMultiListClass::Internal_Add_Tail(MultiListObjectClass *obj, bool onlyonce)
{
	TT_ASSERT(obj);

	if (onlyonce && Is_In_List(obj))
	{
		return false;
	}

PUSH_MEMORY_MACROS;
#undef new
	MultiListNodeClass *node = new MultiListNodeClass;
POP_MEMORY_MACROS;
	node->Object = obj;

	node->NextList = obj->Get_List_Node();
	obj->Set_List_Node(node);

	node->Prev = Head.Prev;
	node->Next = &(Head);
	node->Next->Prev = node;
	node->Prev->Next = node;

	node->List = this;

	return true;
};

bool GenericMultiListClass::Internal_Add_After(MultiListObjectClass *obj, const MultiListObjectClass* existing_list_member, bool onlyonce)
{
	TT_ASSERT(obj);
	TT_ASSERT(existing_list_member);

	if (onlyonce && Is_In_List(obj))
	{
		return false;
	}

	MultiListNodeClass * existing_node = existing_list_member->Get_List_Node();
	while (existing_node && existing_node->List != this)
	{
		existing_node = existing_node->NextList;
	}
	
	if (existing_node == NULL)
	{
		return false;
	}

PUSH_MEMORY_MACROS;
#undef new
	MultiListNodeClass * node = new MultiListNodeClass;
POP_MEMORY_MACROS;
	node->Object = obj;
	
	node->NextList = obj->Get_List_Node();
	obj->Set_List_Node(node);

	node->Prev = existing_node;
	node->Next = existing_node->Next;
	node->Next->Prev = node;
	node->Prev->Next = node;
	node->List = this;

	return true;
};

bool GenericMultiListClass::Internal_Remove(MultiListObjectClass *obj)
{
	TT_ASSERT(obj);
	
	MultiListNodeClass* node = obj->Get_List_Node();
	MultiListNodeClass* prevnode = NULL;

	while (node && node->List != this)
	{
		prevnode = node;
		node = node->NextList;
	}

	if (!node)
	{
		return false;
	}

	node->Prev->Next = node->Next;
	node->Next->Prev = node->Prev;

	if (prevnode) 
	{
		prevnode->NextList = node->NextList;
	}
	else
	{
		TT_ASSERT(obj->Get_List_Node() == node);
		obj->Set_List_Node(node->NextList);
	}

PUSH_MEMORY_MACROS;
#undef delete
	delete node;
POP_MEMORY_MACROS;
	return true;
};

MultiListObjectClass* GenericMultiListClass::Internal_Remove_List_Head()
{
	if (Is_Empty())
	{
		return NULL;
	}
	MultiListObjectClass* obj = Head.Next->Object;
	Internal_Remove(obj);
	return obj;
};
