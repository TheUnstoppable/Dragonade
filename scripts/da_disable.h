/*	Renegade Scripts.dll
    Dragonade Disable List
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DADISABLE
#define INCLUDE_DADISABLE

#include "da_event.h"
#include "da_gamefeature.h"
#include "Definition.h"
#include "HashTemplateClass.h"

struct DACharacterDisableListStruct {
	DACharacterDisableListStruct() : EnterVehicles(false), DriveVehicles(false), StealVehicles(false) { }
	DynamicVectorClass<const DefinitionClass*> DisableList;
	bool EnterVehicles;
	bool DriveVehicles;
	bool StealVehicles;
};

class DADisableListManager : public DAEventClass {
public:
	static void Init();
	DA_API static bool Is_Preset_Disabled(GameObject *obj);
	DA_API static bool Is_Preset_Disabled(const char *Preset);
	DA_API static bool Is_Preset_Disabled(const DefinitionClass *Def);
	DA_API static bool Is_Preset_Disabled(unsigned int DefID);
	DA_API static bool Is_Preset_Disabled_For_Character(const DefinitionClass *Character,const DefinitionClass *Def);
	DA_API static bool Is_Preset_Disabled_For_Character(const DefinitionClass *Character,const char *Preset);
	DA_API static bool Is_Preset_Disabled_For_Character(const DefinitionClass *Character,unsigned int DefID);
	DA_API static bool Can_Character_Enter_Vehicles(const DefinitionClass *Character);
	DA_API static bool Can_Character_Drive_Vehicles(const DefinitionClass *Character);
	DA_API static bool Can_Character_Steal_Vehicles(const DefinitionClass *Character);
	
private:
	virtual void Settings_Loaded_Event();
	virtual void Level_Loaded_Event();
	virtual void Object_Created_Event(GameObject *obj);
	virtual int Character_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const SoldierGameObjDef *Item);
	virtual int Vehicle_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const VehicleGameObjDef *Item);
	virtual int PowerUp_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const PowerUpGameObjDef *Item);
	virtual bool PowerUp_Grant_Request_Event(cPlayer *Player,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj);
	virtual bool Add_Weapon_Request_Event(cPlayer *Player,const WeaponDefinitionClass *Weapon);
	virtual bool Refill_Event(cPlayer *Player);
	virtual bool Vehicle_Entry_Request_Event(VehicleGameObj *Vehicle,cPlayer *Player,int &Seat);
	
	static DynamicVectorClass<const DefinitionClass*> DisableList;
	static HashTemplateClass<unsigned int,DACharacterDisableListStruct> CharacterDisableList; //SoldierGameObjDef*,DACharacterDisableListStruct
};

#endif