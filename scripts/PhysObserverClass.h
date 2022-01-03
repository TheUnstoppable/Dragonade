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
#ifndef TT_INCLUDE_PHYSOBSERVERCLASS_H
#define TT_INCLUDE_PHYSOBSERVERCLASS_H
class CollisionEventClass;
class PhysClass;
class PhysicalGameObj;
class DamageableGameObj;
class BuildingGameObj;
enum _CollisionReactionType
{
	COLLISION_REACTION_DEFAULT,
	COLLISION_REACTION_STOP_MOTION,
	COLLISION_REACTION_NO_BOUNCE,
};
typedef int CollisionReactionType;
enum _ExpirationReactionType
{
	EXPIRATION_DENIED,
	EXPIRATION_APPROVED,
};
typedef int ExpirationReactionType;
class PhysObserverClass
{
public:
	virtual CollisionReactionType		Collision_Occurred(const CollisionEventClass & event) { return COLLISION_REACTION_DEFAULT; }
	virtual ExpirationReactionType	Object_Expired(PhysClass * observed_obj)					{ return EXPIRATION_APPROVED; }
	virtual void							Object_Removed_From_Scene(PhysClass * observed_obj)	{ };
	virtual void							Object_Shattered_Something(PhysClass * observed_obj, PhysClass * shattered_obj, int surface_type) { };
}; // 0004
class	CombatPhysObserverClass : public PhysObserverClass {
public:
	virtual	DamageableGameObj *	As_DamageableGameObj( void )	{ return NULL; }
	virtual	PhysicalGameObj *		As_PhysicalGameObj( void )		{ return NULL; }
	virtual	BuildingGameObj *		As_BuildingGameObj( void )		{ return NULL; }
}; // 0004

#endif