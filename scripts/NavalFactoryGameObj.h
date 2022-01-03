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
#ifndef TT_INCLUDE__NAVALFACTORYGAMEOBJ_H
#define TT_INCLUDE__NAVALFACTORYGAMEOBJ_H
#include "BuildingGameObj.h"
class NavalFactoryGameObjDef;
class SimpleGameObj;
class ScriptZoneGameObjDef;
class NavalFactoryGameObj : public BuildingGameObj
{
public:
	NavalFactoryGameObj (void);
	~NavalFactoryGameObj (void);
	virtual	void					Init( void );
	void								Init (const NavalFactoryGameObjDef & definition);
	SCRIPTS_API const NavalFactoryGameObjDef &	Get_Definition (void) const;
	NavalFactoryGameObj *				As_NavalFactoryGameObj (void)	{ return this; }
	bool									Save (ChunkSaveClass &csave);
	bool									Load (ChunkLoadClass &cload);
	const	PersistFactoryClass &	Get_Factory (void) const;
	void					CnC_Initialize (BaseControllerClass *base);
	void					On_Destroyed (void);
	void					On_Revived (void);
	void					Think (void);
	bool					Is_Available (void) const	{ return (IsBusy == false) && (IsDestroyed == false) && (IsDisabled == false); }
	bool					Can_Spawn(int definition_id);
	bool					Is_Available_For_Purchase (void) const;
	bool					Is_Busy (void) const			{ return IsBusy; }
	bool					Is_Disabled (void) const			{ return IsDisabled; }
	void					Set_Disabled(bool disabled)         { IsDisabled = disabled; Set_Object_Dirty_Bit (BIT_RARE, true);}
	int					Get_Team_Vehicle_Count(void) const; 
	static void			Set_Max_Vehicles_Per_Team(int max)		{ MaxVehiclesPerTeam = max; }
	static int			Get_Max_Vehicles_Per_Team(void)			{ return MaxVehiclesPerTeam; }
	void					Import_Rare (BitStreamClass &packet);
	void					Export_Rare (BitStreamClass &packet);
	void					Create_Vehicle(int defintion_id,SoldierGameObj * player);
	void Set_Busy(bool busy)
	{
		IsBusy = busy;
		Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
	}
private:
	void					Load_Variables (ChunkLoadClass &cload);	
	bool					IsBusy;
    bool                    IsDisabled;
	float					Timer;
	int DefinitionID;
	ReferencerClass	Purchaser;
	ScriptZoneGameObj *SpawnPos;
	ScriptZoneGameObjDef *ConstructionZone1Def;
	DynamicVectorClass<ScriptZoneGameObj *> ConstructionZone1Instances;
	ScriptZoneGameObjDef *ConstructionZone2Def;
	DynamicVectorClass<ScriptZoneGameObj *> ConstructionZone2Instances;
	ScriptZoneGameObjDef *ConstructionZone3Def;
	DynamicVectorClass<ScriptZoneGameObj *> ConstructionZone3Instances;
	ScriptZoneGameObjDef *ConstructionZone4Def;
	DynamicVectorClass<ScriptZoneGameObj *> ConstructionZone4Instances;
	ScriptZoneGameObjDef *ConstructionZone5Def;
	DynamicVectorClass<ScriptZoneGameObj *> ConstructionZone5Instances;
	ScriptZoneGameObjDef *ConstructionZone6Def;
	DynamicVectorClass<ScriptZoneGameObj *> ConstructionZone6Instances;
	static int MaxVehiclesPerTeam;
};

#endif
