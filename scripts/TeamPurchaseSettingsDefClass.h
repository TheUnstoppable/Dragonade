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
#ifndef TT_INCLUDE_TEAMPURCHASESETTINGSDEFCLASS_H
#define TT_INCLUDE_TEAMPURCHASESETTINGSDEFCLASS_H
#include "Definition.h"
#include "Engine_String.h"
#ifndef TTLE_EXPORTS
#include "TranslateDBClass.h"
#endif
#include "NetworkObjectClass.h"
#include "ExtendedNetworkObject.h"
#define PTTEAM(t) (t?0:1) //Convert a normal team into a PT team
class TeamPurchaseSettingsDefClass abstract : public DefinitionClass, public NetworkObjectClass {
public:
	enum TEAM {
		TEAM_GDI,
		TEAM_NOD
	};
	TeamPurchaseSettingsDefClass();
	~TeamPurchaseSettingsDefClass();
	void setDirtyBitsForClient(const int clientId);
	const PersistFactoryClass &Get_Factory() const;
	bool Save(ChunkSaveClass &csave);
	bool Load(ChunkLoadClass &cload);
	void Load_Variables(ChunkLoadClass &cload);
	uint32 Get_Class_ID() const;
	virtual PersistClass *Create() const;
	static SCRIPTS_API TeamPurchaseSettingsDefClass *Get_Definition(TEAM team);
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE(TeamPurchaseSettingsDefClass,DefinitionClass);
	void Delete(void)
	{
	}
	void Set_Delete_Pending()
	{
	}
	unsigned int Get_Network_Class_ID () const { return 2005; }
#ifndef TTLE_EXPORTS
#ifndef DDBEDIT
#ifndef W3DVIEWER
	void Export_Occasional(BitStreamClass &packet);
	void Import_Occasional(BitStreamClass &packet);
#endif
#endif
	const wchar_t *Get_Enlisted_Name(int index)
	{
		if (index >= 0 && (index < 4))
		{
			return TRANSLATE(stringids[index]);
		}
		return 0;
	}
	unsigned int Get_Enlisted_String(int index)
	{
		return stringids[index];
	}
	TEAM Get_Team()
	{
		return team;
	}
    int Get_Enlisted_Cost(int index)
    {
        return costs[index];
    }
	int Get_Enlisted_Definition(int index)
	{
		return presetids[index];
	}
	const char *Get_Enlisted_Texture(int index)
	{
		return textures[index];
	}
	unsigned int Get_Beacon_String()
	{
		return beaconstringid;
	}
	const wchar_t *Get_Beacon_Name()
	{
		return TRANSLATE(beaconstringid);
	}
	int Get_Beacon_Cost()
	{
		return beaconcost;
	}
	int Get_Beacon_Definition()
	{
		return beaconpresetid;
	}
	const char *Get_Beacon_Texture()
	{
		return beacontexture;
	}
	unsigned int Get_Supply_String()
	{
		return refillstringid;
	}
	const wchar_t *Get_Supply_Name()
	{
		return TRANSLATE(refillstringid);
	}
	const char *Get_Supply_Texture()
	{
		return refilltexture;
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
	void Set_Beacon_Definition(int preset) { //DA
		beaconpresetid = preset;
	}
	void Set_Beacon_Cost(int cost) { //DA
		beaconcost = cost;
	}
	void Set_Enlisted_Definition(int index, int preset) { //DA
		presetids[index] = preset;
	}
#endif
private:
	TEAM team;
    int costs[4];
	int presetids[4];
	unsigned int stringids[4];
	StringClass textures[4];
	bool Hidden[4];
	bool Disabled[4];
	bool Busy[4];
	int beaconcost;
	int beaconpresetid;
	unsigned int beaconstringid;
	StringClass beacontexture;
	unsigned int refillstringid;
	StringClass refilltexture;
	static TeamPurchaseSettingsDefClass *DefinitionArray[2];
	friend class PresetDump;
};
	
IMPLEMENT_EXTENDED_NETWORK_OBJECT(TeamPurchaseSettingsDefClass);

#endif
