/*	Renegade Scripts.dll
	Copyright 2016 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#pragma once



enum
{
	CUSTOM_EVENT_REDTIB_ENTERED = 10499,
	CUSTOM_EVENT_GREENTIB_ENTERED = 10500,
	CUSTOM_EVENT_BLUETIB_ENTERED = 10501,
	CUSTOM_EVENT_TIB_ENTERED = 10502,
	CUSTOM_EVENT_HARVY_LEFT_TIBFIELD = 10503,
	CUSTOM_EVENT_HARVY_DEPOSITING = 10504,	
};


enum
{
	TIBTYPE_GREEN = 10100,
	TIBTYPE_BLUE = 10101,
	TIBTYPE_RED = 10102,
};



HashTemplateClass<uint,uint> CharacterID;
float CostScale=1;
float InsertScale=1;

class DB_Research_Listener : public ScriptImpClass {
	void Created(GameObject *obj);
};

class DB_Research_Infantry_Armor_Terminal : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	void Detach(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	bool Allow_Poke;
	bool Upgrading_In_Progress;
	bool Upgrade_Completed;
	int Terminal_Team;
	int UpgradeID;
	const char *UpgradeName;
	const char *Upgrade_In_Progress_Sound;
	const char *Upgrade_Money_Added_Sound;
	const char *Upgrade_Completed_Sound;
	const char *Upgrade_Insufficient_Funds_Sound;
	const char *Upgrade_Access_Denied_Sound;
	const char *Upgrade_Complete_Text;
	const char *Upgrade_In_Progress_Text;
	float Upgrade_Add_Funds_Cost;
	float Upgrade_Total_Cost;
	float Upgrade_Current_Funds;
};

class DB_Rebuild_Building_Terminal : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	void Detach(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	bool Allow_Poke;
	bool Upgrading_In_Progress;
	bool Upgrade_Completed;
	int Terminal_Team;
	int BuildingType;
	const char *UpgradeName;
	const char *Upgrade_In_Progress_Sound;
	const char *Upgrade_Money_Added_Sound;
	const char *Upgrade_Completed_Sound;
	const char *Upgrade_Insufficient_Funds_Sound;
	const char *Upgrade_Access_Denied_Sound;
	const char *Upgrade_Complete_Text;
	const char *Upgrade_In_Progress_Text;
	float Upgrade_Add_Funds_Cost;
	float Upgrade_Total_Cost;
	float Upgrade_Current_Funds;
};

class DB_Research_Infantry_Armor_Ready : public ScriptImpClass {
	void Created(GameObject *obj);
};

class DB_Research_Infantry_Armor_Ability : public ScriptImpClass {
	int UpgradeID;
	bool Upgraded;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class DB_Research_Infantry_Hitpoints_Ability : public ScriptImpClass {
	int UpgradeID;
	bool Upgraded;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class DB_Research_Weapon_Upgrade : public ScriptImpClass {
	int UpgradeID;
	bool Upgraded;
	int Secondary;
	StringClass RemoveWeapon;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class DB_Research_Armor_Model : public ScriptImpClass {
	int UpgradeID;
	bool Upgraded;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class DB_Research_Credit_Tick : public ScriptImpClass {
	int UpgradeID;
	bool Upgraded;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};


class DB_Tiberium_Harvester : public ScriptImpClass 
{
	int upgradeID;
	float multiplier;
	bool upgraded;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	float Get_Cash();
	void Clear_Cash();
	int PlayerTeam;
	int DriverID;
	bool Harvesting;
	int TibType;
	float CashCollected[8];
	int CashIndex;
};

class DB_Tiberium_Field : public ScriptImpClass 
{
	void Created(GameObject *obj);
	void Entered(GameObject *obj, GameObject *enterer);
	void Exited(GameObject *obj, GameObject *exiter);
	int Zonetype;
};

class DB_Tiberium_Deposit : public ScriptImpClass 
{
	void Entered(GameObject *obj, GameObject *enterer);
};

class DB_DeployableTank_Upgradeable : public JFW_Key_Hook_Base {
	int UpgradeID;
	bool Upgraded;
	int PilotID,mode;
	StringClass model;
	void Created(GameObject *obj);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	void KeyHook();
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
	public: void Register_Auto_Save_Variables();
};