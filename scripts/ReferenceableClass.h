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
#ifndef TT_INCLUDE_REFERENCABLECLASS_H
#define TT_INCLUDE_REFERENCABLECLASS_H
#include "engine_io.h"
#include "SaveLoadSystemClass.h"
class ChunkSaveClass;
class ChunkLoadClass;
class ReferencerClass;
class ScriptableGameObj;
template	<class T>
class ReferenceableClass {
public:
	friend class ReferencerClass;
	ReferenceableClass( T *reference_data ) : ReferenceData( reference_data ), ReferencerListHead( NULL ) {}
	~ReferenceableClass( void );
	bool	Save( ChunkSaveClass & csave );
	bool	Load( ChunkLoadClass & cload );
protected:
	ReferencerClass	*ReferencerListHead;	
	T * Get_Data( void ) const	{ return ReferenceData; }
private:
	T							*ReferenceData;	
};
template<class T>
ReferenceableClass<T>::~ReferenceableClass( void )	
{
	while ( ReferencerListHead != NULL )
	{
		ReferencerClass *referencer = ReferencerListHead;
		ReferencerListHead = referencer->TargetReferencerListNext;
		referencer->ReferenceTarget = NULL;
		referencer->TargetReferencerListNext = NULL;
	}
}
typedef ReferenceableClass<ScriptableGameObj> ReferenceableGameObj;
enum	{
	CHUNKID_REF_VARIABLES				=	913991844,
	XXXX_MICROCHUNKID_LIST_HEAD		=	1,
	MICROCHUNKID_XXXXXXXXX,
	MICROCHUNKID_PTR,
	MICROCHUNKID_TARGET,
	XXXX_MICROCHUNKID_NEXT,
};
template<class T>
bool	ReferenceableClass<T>::Save( ChunkSaveClass & csave )
{
	csave.Begin_Chunk( CHUNKID_REF_VARIABLES );
		void * ptr = this;
		WRITE_MICRO_CHUNK( csave, MICROCHUNKID_PTR, ptr );
	csave.End_Chunk();
	return true;
}
template<class T>
bool	ReferenceableClass<T>::Load( ChunkLoadClass & cload )
{
	void * old_ptr;
	cload.Open_Chunk();
	while (cload.Open_Micro_Chunk())
	{
		switch(cload.Cur_Micro_Chunk_ID())
		{
			case MICROCHUNKID_PTR:
				cload.Read(&old_ptr,sizeof(void*));
				SaveLoadSystemClass::Register_Pointer(old_ptr, this);
				break;
			default:
				break;
		}
		cload.Close_Micro_Chunk();
	}
	cload.Close_Chunk();
	return true;
}

#endif