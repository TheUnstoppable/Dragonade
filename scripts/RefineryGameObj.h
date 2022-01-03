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
#ifndef TT_INCLUDE__REFINERYGAMEOBJ_H
#define TT_INCLUDE__REFINERYGAMEOBJ_H

#include "BuildingGameObj.h"
#include "Matrix3D.h"
#include "OBBoxClass.h"
#include "RefineryGameObjDef.h"

class HarvesterClass;
class RefineryGameObjDef;
class RefineryGameObj : public BuildingGameObj
{
public:
	RefineryGameObj (void);
	~RefineryGameObj (void);
	virtual	void					Init( void );
	void								Init (const RefineryGameObjDef & definition);
	const RefineryGameObjDef SCRIPTS_API &	Get_Definition (void) const;
	RefineryGameObj *				As_RefineryGameObj (void)	{ return this; }
	bool									Save (ChunkSaveClass &csave);
	bool									Load (ChunkLoadClass &cload);
	const	PersistFactoryClass &	Get_Factory (void) const;
	void					CnC_Initialize (BaseControllerClass *base);
	void					On_Destroyed (void);
	void					Think (void);
	void					Manage_Money_Trickle_Sound (void);
	bool					Needs_Harvester (void) const			{ return (!IsDestroyed) && (Harvester == NULL); }
	int					Get_Harvester_Def_ID (void) const	{ return Get_Definition ().HarvesterDefID; }
	void					Set_Is_Harvester_Docked(bool flag);
	bool					Get_Is_Harvester_Docked(void)			{ return IsHarvesterDocked; }
	SCRIPTS_API VehicleGameObj *	Get_Harvester_Vehicle (void); //DA
	void					Set_Harvester_Vehicle (VehicleGameObj *harvester);
	HarvesterClass *	Get_Harvester (void)								{ return Harvester; }
	void					Set_Harvester (HarvesterClass *harvester)	{ Harvester = harvester; }
	void					On_Harvester_Docked (void);
	void					On_Harvester_Damaged(VehicleGameObj*);
	void					On_Harvester_Destroyed(VehicleGameObj*);
	void					Play_Unloading_Animation (bool onoff);
	const Matrix3D	&	Get_Dock_TM (void) const			{ return DockTM; }
	void					Set_Dock_TM (const Matrix3D &tm)	{ DockTM = tm; }
	virtual	void		Export_Rare (BitStreamClass &packet);
	virtual	void		Import_Rare (BitStreamClass &packet);
	void SCRIPTS_API Destroy_Harvester(); //DA
	void					Block_Harvester_Spawn()
	{
		//Destroy_Harvester(); //DA
		AllowHarvesterSpawn = false;
	}
	void					Allow_Harvester_Spawn()
	{
		AllowHarvesterSpawn = true;
	}
	bool Get_Allow_Harvester_Spawn() { //DA
		return AllowHarvesterSpawn;
	}
	static float HarvesterDumpRate;
private:
	void					Load_Variables (ChunkLoadClass &cload);	
	HarvesterClass *	Harvester;
	OBBoxClass			TiberiumField;
	bool					IsHarvesterDocked;
	float					UnloadTimer;
	int					UnloadAnimationID;
	float					TotalFunds;
	float					FundsPerSecond;
	float					DistributionTimer;
	Matrix3D				DockTM;
	AudibleSoundClass * MoneyTrickleSound;
	bool					AllowHarvesterSpawn;
	ReferencerClass HarvesterVehicle;
	float accumulatedDumpCredits;
	float DumpRate;
}; // 2232, RH7 2252


#endif
