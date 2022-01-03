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
#ifndef TT_INCLUDE_BASECONTROLLERCLASS_H
#define TT_INCLUDE_BASECONTROLLERCLASS_H
#include "NetworkObjectClass.h"
#include "OBBoxClass.h"
#include "Engine_Vector.h"
#include "scripts.h"
class BuildingGameObj;
class VehicleGameObj;
class BeaconGameObj;
enum RadarModeEnum {
	RADAR_NOBODY,
	RADAR_TEAMMATES,
	RADAR_ALL,
};
class BaseControllerClass :
	public NetworkObjectClass
{

private:

	float OperationTimeFactor; // 06B4
	int team; // 06B8
	DynamicVectorClass<BuildingGameObj*> BuildingList; // 06BC
	bool IsPowered; // 06D4
	bool CanGenerateSoldiers; // 06D5
	bool CanGenerateVehicles; // 06D6
	bool RadarEnabled; // 06D7
	bool BaseDestroyed; // 06D8
	bool BeaconDestroyedBase; // 06D9
	OBBoxClass BeaconZone; // 06DC
	unsigned int AnnouncementInterval; // 0718
	float AnnouncementTime; // 071C
	float AnnouncementTime2; // 0720
	unsigned int Unknown1; // 0724
	unsigned int Unknown2; // 0728

public:
	BaseControllerClass();
	~BaseControllerClass();
	void Import_Occasional(BitStreamClass &BitStream);
	void Export_Occasional(BitStreamClass &BitStream);
	void Set_Delete_Pending()
	{
	}
	void Delete()
	{
	}
	SCRIPTS_API static BaseControllerClass *Find_Base(int Team); //DA
	SCRIPTS_API VehicleGameObj *Get_Harvester_Vehicle(); //DA
	SCRIPTS_API BuildingGameObj *Find_Building(unsigned int Type); //DA
	SCRIPTS_API void Power_Base(bool Power); //DA
	SCRIPTS_API void Set_Can_Generate_Soldiers(bool CanGenerate); //DA
	SCRIPTS_API void Set_Can_Generate_Vehicles(bool CanGenerate); //DA
	SCRIPTS_API void Set_Base_Destroyed(bool BaseDesrtoyed); //DA
	SCRIPTS_API void Destroy_Base(); //DA
	SCRIPTS_API void Set_Beacon_Destroyed_Base(bool BeaconDestroyed); //DA
	SCRIPTS_API void Enable_Radar(bool Enable); //DA
	SCRIPTS_API void Distribute_Funds_To_Each_Teammate(int Amount); //DA
	SCRIPTS_API void Distribute_Funds_To_Each_Teammate(float Amount); //DA
	void On_Vehicle_Generated(VehicleGameObj *);
	void On_Beacon_Warning(BeaconGameObj* beacon);
	void Request_Harvester(int harvesterDefinitionId);
	void Check_Radar();
	static BaseControllerClass *Find_Base_For_Star();
	void On_Beacon_Armed(BeaconGameObj *);
	void On_Beacon_Disarmed(BeaconGameObj *);
	void Add_Building(BuildingGameObj *b);
	void On_Building_Damaged(BuildingGameObj *);
	void On_Building_Destroyed(BuildingGameObj *);
	void On_Building_Revived(BuildingGameObj *);
	void On_Vehicle_Delivered(VehicleGameObj *);
	void On_Vehicle_Damaged(VehicleGameObj *);
	void On_Vehicle_Destroyed(VehicleGameObj *);
	void Check_Base_Power();
	void Check_Vehicle_Factory();
	void Play_Announcement(int announcement);
	SCRIPTS_API bool Can_Build_Air(); //DA
	SCRIPTS_API bool Can_Build_Ground(); //DA
	SCRIPTS_API bool Can_Build_Naval(); //DA
	void Check_Prerequisites();

	bool Is_Base_Powered() const
	{
		return IsPowered;
	}
	float Get_Operation_Time_Factor() const
	{
		return OperationTimeFactor;
	}
	bool Can_Generate_Soldiers() const
	{
		return CanGenerateSoldiers;
	}
	bool Can_Generate_Vehicles() const
	{
		return CanGenerateVehicles;
	}
	int Get_Player_Type() const
	{
		return team;
	}
	bool Is_Base_Destroyed() const
	{
		return BaseDestroyed;
	}
	bool Did_Beacon_Destroy_Base() const
	{
		return BeaconDestroyedBase;
	}
	const OBBoxClass& Get_Beacon_Zone()
	{
		return BeaconZone;
	}
	bool Is_Radar_Enabled() const
	{
		return RadarEnabled;
	}
	DynamicVectorClass<BuildingGameObj*> &Get_Building_List()
	{
		return BuildingList;
	}
	bool Are_All_Buildings_Destroyed();
	void Initialize(int team);
	void Initialize_Building_List();
	void Reset_Building_List();
	//void Notify_Team(BaseControllerClass::Notification,BuildingConstants::BuildingType);
	void Set_Base_Powered(bool onoff);
	void Set_Operation_Time_Factor(float time);
	void Think();
	void Shutdown();
}; // 072C

#endif