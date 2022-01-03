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
#ifndef TT_INCLUDE__ITERATOR_H
#define TT_INCLUDE__ITERATOR_H



#include "SList.h"
#include "engine_vector.h"

#define TT_FOREACH(iterator, collection) for (Iterator<decltype(collection)&> iterator(collection); iterator; ++iterator)
#define TT_CONST_FOREACH(iterator, collection) for (Iterator<const decltype(collection)&> iterator(collection); iterator; ++iterator)



template<class T> class Iterator;



template<class T> class Iterator<SList<T>&>
{

	SLNode<T>* node;

public:

	inline Iterator(SList<T>& list) { node = list.Head(); }
	inline operator bool() { return node != NULL; }
	inline operator T*() { return node->Data(); }
	inline T* operator ->() { return (T*)*this; }
	inline Iterator& operator ++() { node = node->Next(); return *this; }

};



template<class T> class Iterator<const SList<T>&>
{

	const SLNode<T>* node;

public:

	inline Iterator(const SList<T>& list) { node = list.Head(); }
	inline operator bool() { return node != NULL; }
	inline operator const T*() { return node->Data(); }
	inline const T* operator ->() { return (T*)*this; }
	inline Iterator& operator ++() { node = node->Next(); return *this; }

};



template<class T> class Iterator<VectorClass<T>&>
{

	T* current;
	T* end;

public:

	inline Iterator(VectorClass<T>& vector) { current = &vector[0]; end = &vector[vector.Length()]; }
	inline operator bool() { return current != end; }
	inline operator T*() { return current; }
	inline T* operator ->() { return (T*)*this; }
	inline Iterator& operator ++() { ++current; return *this; }

};



template<class T> class Iterator<const VectorClass<T>&>
{

	const T* current;
	const T* end;

public:

	inline Iterator(const VectorClass<T>& vector) { current = &vector[0]; end = &vector[vector.Length()]; }
	inline operator bool() { return current != end; }
	inline operator const T*() { return current; }
	inline const T* operator ->() { return (const T*)*this; }
	inline Iterator& operator ++() { ++current; return *this; }

};



template<class T> class Iterator<DynamicVectorClass<T>&>
{

	T* current;
	T* end;

public:

	inline Iterator(DynamicVectorClass<T>& vector) { current = &vector[0]; end = &vector[vector.Count()]; }
	inline operator bool() { return current != end; }
	inline operator T*() { return current; }
	inline T* operator ->() { return (T*)*this; }
	inline Iterator& operator ++() { ++current; return *this; }

};



template<class T> class Iterator<const DynamicVectorClass<T>&>
{

	const T* current;
	const T* end;

public:

	inline Iterator(const DynamicVectorClass<T>& vector) { current = &vector[0]; end = &vector[vector.Count()]; }
	inline operator bool() { return current != end; }
	inline operator const T*() { return current; }
	inline const T* operator ->() { return (const T*)*this; }
	inline Iterator& operator ++() { ++current; return *this; }

};



template<class T> class Iterator<SimpleVecClass<T>&>
{

	T* current;
	T* end;

public:

	inline Iterator(SimpleVecClass<T>& vector) { current = &vector[0]; end = &vector[vector.Length()]; }
	inline operator bool() { return current != end; }
	inline operator T*() { return current; }
	inline T* operator ->() { return (T*)*this; }
	inline Iterator& operator ++() { ++current; return *this; }

};



template<class T> class Iterator<const SimpleVecClass<T>&>
{

	const T* current;
	const T* end;

public:

	inline Iterator(const SimpleVecClass<T>& vector) { current = &vector[0]; end = &vector[vector.Length()]; }
	inline operator bool() { return current != end; }
	inline operator const T*() { return current; }
	inline const T* operator ->() { return (const T*)*this; }
	inline Iterator& operator ++() { ++current; return *this; }

};



template<class T> class Iterator<SimpleDynVecClass<T>&>
{

	T* current;
	T* end;

public:

	inline Iterator(SimpleDynVecClass<T>& vector) { current = &vector[0]; end = &vector[vector.Count()]; }
	inline operator bool() { return current != end; }
	inline operator T*() { return current; }
	inline T* operator ->() { return (T*)*this; }
	inline Iterator& operator ++() { ++current; return *this; }

};



template<class T> class Iterator<const SimpleDynVecClass<T>&>
{

	const T* current;
	const T* end;

public:

	inline Iterator(const SimpleDynVecClass<T>& vector) { current = &vector[0]; end = &vector[vector.Count()]; }
	inline operator bool() { return current != end; }
	inline operator const T*() { return current; }
	inline const T* operator ->() { return (const T*)*this; }
	inline Iterator& operator ++() { ++current; return *this; }

};


#endif