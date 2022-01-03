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
//
// This class is to allow virtuals to be added to a NetworkObjectClass without
// having to change the offsets of the base class.
//
// To use this class, declare the original class abstract (so that no instances
// can be created), add IMPLEMENT_EXTENDED_NETWORK_OBJECT(ClassName) to your
// class header file, and prefix all instantiations with "Extended", ie. replace
// "new ClassName()" with "new ExtendedClassName()".
//

#pragma once


class GenericExtendedNetworkObject
{
public:
	
	GenericExtendedNetworkObject();
	~GenericExtendedNetworkObject();

	virtual void setDirtyBitsForClient(const int clientId) = 0;
};


template<class BaseClass>
class ExtendedNetworkObject :
	public BaseClass,
	public GenericExtendedNetworkObject
{
public:
	
	virtual void setDirtyBitsForClient(const int clientId) { return BaseClass::setDirtyBitsForClient(clientId); }

};


#define CONCAT(a, b) CONCAT_(a, b)
#define CONCAT_(a, b) a ## b
#define IMPLEMENT_EXTENDED_NETWORK_OBJECT(BaseClass) \
	typedef ExtendedNetworkObject<BaseClass> CONCAT(Extended, BaseClass);
