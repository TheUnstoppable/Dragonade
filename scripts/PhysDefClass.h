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
#ifndef TT_INCLUDE__PHYSDEFCLASS_H
#define TT_INCLUDE__PHYSDEFCLASS_H

#include "engine_common.h"
#include "engine_vector.h"
#include "engine_io.h"
#include "engine_threading.h"
#include "engine_string.h"
#include "Definition.h"
// If you modify this list, be sure to add the friendly name to GetCollisionGroupName
typedef enum {
	DEFAULT_COLLISION_GROUP			= 0,		// collides with everything
	UNCOLLIDEABLE_GROUP,						// collides with nothing
	TERRAIN_ONLY_COLLISION_GROUP,				// collides only with terrain
	BULLET_COLLISION_GROUP,						// collides with everything but itself
	TERRAIN_AND_BULLET_COLLISION_GROUP,			// collides with terrain and bullets
	BULLET_ONLY_COLLISION_GROUP,				// collides only with bullets
	SOLDIER_COLLISION_GROUP,					// collides with everything (but only soldiers use it)
	SOLDIER_GHOST_COLLISION_GROUP,				// behaves like SOLDIER but doesn't collide with unit groups

	// new collision groups here, 8 - 14
	C4_COLLISION_GROUP,							// behaves like DEFAULT but collides with SOLDIER_GHOST and doesn't collide with WATER_EDGE or UNDERGROUND_TRANSITION
	UNDERGROUND_COLLISION_GROUP,				// collides with TERRAIN and itself
	SOLDIER_ONLY_COLLISION_GROUP, 				// collides only with SOLDIER and SOLDIER_GHOST
	SOLDIER_BULLET_COLLISION_GROUP, 			// collides with C4, BULLET, SOLDIER and SOLDIER_GHOST

	TERRAIN_COLLISION_GROUP			= 15,		// Terrain must be 15

	// more collision groups here, 16 - 31
	WATER_SURFACE_COLLISION_GROUP,				// collides with NAVAL_UNIT, BEACHING_UNIT, AMBHIBIOUS_UNIT, and BULLET
	WATER_EDGE_COLLISION_GROUP,					// collides with DEFAULT, SOLDIER, and NAVAL_UNIT
	WATER_EDGE_ALT_COLLISION_GROUP,				// collides only with NAVAL_UNIT
	BEACH_EDGE_COLLISION_GROUP,					// collides only with BEACHING_UNIT
	NAVAL_UNIT_COLLISION_GROUP,					// behaves like DEFAULT but collides with WATER_SURFACE and WATER_EDGE
	BEACHING_UNIT_COLLISION_GROUP,				// behaves like DEFAULT but collides with WATER_SURFACE and BEACH_EDGE
	HOVER_UNIT_COLLISION_GROUP,					// behaves like DEFAULT but collides with WATER_SURFACE
	AMPHIBIOUS_UNIT_COLLISION_GROUP,			// behaves like DEFAULT but collides with AMPHIBIOUS_UNIT_FLOOR
	AMPHIBIOUS_UNIT_FLOOR_COLLISION_GROUP,		// collides only with AMPHIBIOUS_UNIT
	UNDERGROUND_TRANSITION_COLLISION_GROUP,		// behaves like DEFAULT but collides with UNDERGROUND

    DEFAULT_AND_SOLDIER_ONLY_COLLISION_GROUP,   // collides with DEFAULT, SOLDIER, and SOLDIER_GHOST
	
	PLAYER_BUILDING_COLLISION_GROUP,			// behaves like a mix of TERRAIN/DEFAULT but doesn't collde with PLAYER_BUILDING_GHOST
	PLAYER_BUILDING_GHOST_COLLISION_GROUP,		// behaves like DEFAULT but doesn't collde with PLAYER_BUILDING
	TRAIN_COLLISION_GROUP,						// behaves like DEFAULT but collides with TRAIN_TRACK and doesn't collide with WATER_EDGE
	TRAIN_TRACK_COLLISION_GROUP,				// collides only with TRAIN

	COLLISION_GROUP_MAX = 32,					// not a collision group
} Collision_Group_Type;

inline const char* GetCollisionGroupName(Collision_Group_Type group)
{
	switch(group)
	{
	case DEFAULT_COLLISION_GROUP:				return "Default";
	case UNCOLLIDEABLE_GROUP:					return "Uncollideable";
	case TERRAIN_ONLY_COLLISION_GROUP:			return "Terrain-only";
	case BULLET_COLLISION_GROUP:				return "Bullet";
	case TERRAIN_AND_BULLET_COLLISION_GROUP:	return "Terrain and Bullet";
	case BULLET_ONLY_COLLISION_GROUP:			return "Bullet-only";
	case SOLDIER_COLLISION_GROUP:				return "Soldier";
	case SOLDIER_GHOST_COLLISION_GROUP:			return "Soldier-ghost";
	case UNDERGROUND_COLLISION_GROUP:			return "Underground";
	case SOLDIER_ONLY_COLLISION_GROUP:			return "Soldier-only";
	case SOLDIER_BULLET_COLLISION_GROUP:		return "Soldier-bullet";
	case TERRAIN_COLLISION_GROUP:				return "Terrain";
	case WATER_SURFACE_COLLISION_GROUP:			return "Water Surface";	
	case WATER_EDGE_COLLISION_GROUP:			return "Water Edge (Block)";
	case WATER_EDGE_ALT_COLLISION_GROUP:		return "Water Edge (Allow)";
	case BEACH_EDGE_COLLISION_GROUP:			return "Beach Edge";
	case NAVAL_UNIT_COLLISION_GROUP:			return "Naval Unit";
	case BEACHING_UNIT_COLLISION_GROUP:			return "Beaching Unit";
	case HOVER_UNIT_COLLISION_GROUP:			return "Hover Unit";
	case AMPHIBIOUS_UNIT_COLLISION_GROUP:		return "Amphibious Unit";
	case AMPHIBIOUS_UNIT_FLOOR_COLLISION_GROUP:	return "Amphibious Unit Floor";
	case UNDERGROUND_TRANSITION_COLLISION_GROUP:return "Underground Transition";
    case DEFAULT_AND_SOLDIER_ONLY_COLLISION_GROUP: return "Default and Soldier only";
	case PLAYER_BUILDING_COLLISION_GROUP:		return "Player Building";
	case PLAYER_BUILDING_GHOST_COLLISION_GROUP: return "Player Building Ghost";
	case TRAIN_COLLISION_GROUP:					return "Train";
	case TRAIN_TRACK_COLLISION_GROUP:			return "Train Track";
	default: return NULL; // NULL == do not add to friendly name list
	};
};

typedef enum
{
	// visibility mode
	VISIBILITYMODE_DEFAULT = 0,	// visibility is not affected by the collision group of the player controlled object
	VISIBILITYMODE_IFCOLLIDE,	// visible only if the player controlled object can collide with this object
	VISIBILITYMODE_IFNOCOLLIDE,	// visible only if the player controlled object won't collide with this object
	VISIBILITYMODE_MAX = 3		// not a visibilty mode
} Visibility_Mode_Type;

class PhysDefClass : public DefinitionClass
{
public:
	PhysDefClass(void);
	virtual bool					Save(ChunkSaveClass &csave);
	virtual bool					Load(ChunkLoadClass &cload);
	virtual const char *			Get_Type_Name(void)				{ return "PhysDef"; }
	virtual bool					Is_Type(const char *);
	virtual bool					Is_Valid_Config (StringClass &message);
	const StringClass &			Get_Model_Name()					{ return ModelName; }
	bool								Get_Is_Pre_Lit()					{ return IsPreLit; }
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE(PhysDefClass,DefinitionClass);
protected:
	StringClass						ModelName;
	bool								IsPreLit;
	friend class PhysClass;
	friend class PresetDump;
}; // 36

#endif
