/*	Renegade Scripts.dll
	Copyright 2013 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#ifndef TT_INCLUDE_PURCHASESETTINGSDEFCLASS_H
#define TT_INCLUDE_PURCHASESETTINGSDEFCLASS_H
#include "Definition.h"
#include "Engine_String.h"
#ifndef TTLE_EXPORTS
#include "TranslateDBClass.h"
#endif
#include "NetworkObjectClass.h"
#include "ExtendedNetworkObject.h"
#include "BuildingGameObjDef.h"
using namespace BuildingConstants;
#define PTTEAM(t) (t?0:1) //Convert a normal team into a PT team
class PurchaseSettingsDefClass abstract : public DefinitionClass, public NetworkObjectClass  {
public:
	enum TEAM {
		TEAM_GDI,
		TEAM_NOD,
		TEAM_MUTANT_GDI,
		TEAM_MUTANT_NOD,
	};
	enum TYPE {
		TYPE_CLASSES,
		TYPE_VEHICLES,
		TYPE_EQUIPMENT,
		TYPE_SECRET_CLASSES,
		TYPE_SECRET_VEHICLES,
		TYPE_AIR,
		TYPE_NAVAL,
	};
	PurchaseSettingsDefClass();
	~PurchaseSettingsDefClass();
	void setDirtyBitsForClient(const int clientId);
	const PersistFactoryClass &Get_Factory() const;
	bool Save(ChunkSaveClass &csave);
	bool Load(ChunkLoadClass &cload);
	void Load_Variables(ChunkLoadClass &cload);
	uint32 Get_Class_ID() const;
	PersistClass *Create() const;
	static SCRIPTS_API PurchaseSettingsDefClass *Find_Definition(TYPE type,TEAM team);
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE(PurchaseSettingsDefClass,DefinitionClass);
	void Delete(void)
	{
	}
	void Set_Delete_Pending()
	{
	}
	unsigned int Get_Network_Class_ID () const { return 2004; }
#ifndef TTLE_EXPORTS
#ifndef DDBEDIT
#ifndef W3DVIEWER
	void Export_Occasional(BitStreamClass &packet);
	void Import_Occasional(BitStreamClass &packet);
#endif
#endif
	int Get_Definition(int index)
	{
		return presetids[index];
	}
	int Get_Cost(int index)
	{
		return costs[index];
	}
	const wchar_t *Get_Name(int index)
	{
		return TRANSLATE(stringids[index]);
	}
	const char *Get_Texture(int index)
	{
		return textures[index];
	}
	int Get_Alt_Definition(int index,int index2)
	{
		return altpresetids[index][index2];
	}
	const char *Get_Alt_Texture(int index,int index2)
	{
		return alttextures[index][index2];
	}
	int Get_String(int index)
	{
		return stringids[index];
	}
	TEAM Get_Team()
	{
		return team;
	}
	TYPE Get_Type()
	{
		return type;
	}
	int Get_Tech_Level(int index)
	{
		return TechLevel[index];
	}
	bool Get_Factory_Not_Required(int index)
	{
		return FactoryNotRequired[index];
	}
	BuildingType Get_Factory_Building(int index)
	{
		return Factory[index];
	}
	BuildingType Get_Tech_Building(int index)
	{
		return TechBuilding[index];
	}
	void Set_Tech_Level(int index,int level)
	{
		TechLevel[index] = level;
	}
	void Set_Factory_Building(int index,BuildingType type)
	{
		Factory[index] = type;
	}
	void Set_Tech_Building(int index,BuildingType type)
	{
		TechBuilding[index] = type;
	}
	bool Get_Hidden(int index)
	{
		return Hidden[index];
	}
	bool Get_Disabled(int index)
	{
		return Disabled[index];
	}
	bool Get_Busy(int index)
	{
		return Busy[index];
	}
	void Set_Hidden(int index, bool b)
	{
		Hidden[index] = b;
		Set_Object_Dirty_Bit(BIT_OCCASIONAL,true);
	}
	void Set_Disabled(int index, bool b)
	{
		Disabled[index] = b;
		Set_Object_Dirty_Bit(BIT_OCCASIONAL,true);
	}
	void Set_Busy(int index, bool b)
	{
		Busy[index] = b;
		Set_Object_Dirty_Bit(BIT_OCCASIONAL,true);
	}
	bool Get_Page_Hidden()
	{
		return PageHidden;
	}
	void Set_Page_Hidden(bool b)
	{
		PageHidden = b;
		Set_Object_Dirty_Bit(BIT_OCCASIONAL,true);
	}
	bool Get_Page_Disabled()
	{
		return PageDisabled;
	}
	void Set_Page_Disabled(bool b)
	{
		PageDisabled = b;
		Set_Object_Dirty_Bit(BIT_OCCASIONAL,true);
	}
	bool Get_Page_Busy()
	{
		return PageBusy;
	}
	void Set_Page_Busy(bool b)
	{
		PageBusy = b;
		Set_Object_Dirty_Bit(BIT_OCCASIONAL,true);
	}
	void Set_Definition(int index,int preset) {
		presetids[index] = preset;
	}
	void Set_Alt_Definition(int index,int index2,int preset) {
		altpresetids[index][index2] = preset;
	}
	void Set_Cost(int index,int cost) {
		costs[index] = cost;
	}
#endif
private:
	TEAM team;
	TYPE type;
	int costs[10];
	int presetids[10];
	unsigned int stringids[10];
	StringClass textures[10];
	bool Hidden[10];
	bool Disabled[10];
	bool Busy[10];
	int TechLevel[10];
	BuildingType Factory[10];
	BuildingType TechBuilding[10];
	bool FactoryNotRequired[10];
	int altpresetids[10][3];
	StringClass alttextures[10][3];
	bool PageHidden;
	bool PageDisabled;
	bool PageBusy;
	static PurchaseSettingsDefClass *DefinitionArray[4][7];
	friend class PresetDump;
};

IMPLEMENT_EXTENDED_NETWORK_OBJECT(PurchaseSettingsDefClass);

#endif
