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
#ifndef TT_INCLUDE__VEHICLEFACTORYGAMEOBJ_H
#define TT_INCLUDE__VEHICLEFACTORYGAMEOBJ_H

#include "BuildingGameObj.h"
#include "Matrix3D.h"
#include "OBBoxClass.h"
class VehicleFactoryGameObjDef;
class VehicleFactoryGameObj : public BuildingGameObj
{
public:
	VehicleFactoryGameObj (void);
	~VehicleFactoryGameObj (void);
	virtual	void							Init( void );
	void										Init (const VehicleFactoryGameObjDef & definition);
	SCRIPTS_API const VehicleFactoryGameObjDef &	Get_Definition (void) const;
	VehicleFactoryGameObj *			As_VehicleFactoryGameObj (void)	{ return this; }
	bool									Save (ChunkSaveClass &csave);
	bool									Load (ChunkLoadClass &cload);
	const	PersistFactoryClass &	Get_Factory (void) const;
	void					CnC_Initialize (BaseControllerClass *base);
	void					On_Destroyed (void);
	void					On_Revived (void);
	void					Think (void);
	bool					Is_Available (void) const	{ return !IsBusy && !IsDestroyed && !IsDisabled && !GeneratingVehicleID && !Vehicle; }
	bool					Is_Available_For_Purchase (void) const;
	bool					Is_Busy (void) const			{ return IsBusy; }
	bool					Is_Disabled (void) const			{ return IsDisabled != 0; }
	void					Set_Disabled(bool disabled)         { IsDisabled = disabled; Set_Object_Dirty_Bit (BIT_RARE, true);}
	int					Get_Team_Vehicle_Count(void) const; 
	bool					SCRIPTS_API Request_Vehicle (int defintion_id, float generation_time,SoldierGameObj * player = NULL);
	const Matrix3D &	Get_Creation_TM (void)						{ return CreationTM; }
	void					Set_Creation_TM (const Matrix3D &tm)	{ CreationTM = tm; }
	void					Import_Rare (BitStreamClass &packet);
	void					Export_Rare (BitStreamClass &packet);
	static void			Set_Max_Vehicles_Per_Team(int max)		{ MaxVehiclesPerTeam = max; }
	static int			Get_Max_Vehicles_Per_Team(void)			{ return MaxVehiclesPerTeam; }
	void Set_Busy(bool busy)
	{
		IsBusy = busy;
		Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
	}
	int Get_Generating_Vehicle_ID() //DA
	{
		return GeneratingVehicleID;
	}
	SoldierGameObj *Get_Purchaser() //DA
	{
		return (SoldierGameObj*)Purchaser.Get_Ptr();
	}
protected:
	void					Load_Variables (ChunkLoadClass &cload);
	virtual void		Destroy_Blocking_Objects (void);
	void					Deliver_Vehicle(void);
	VehicleGameObj *	Create_Vehicle (void);
	virtual void		Begin_Generation (void)	{};
	virtual void		On_Generation_Complete (void);
	ReferencerClass	Vehicle;
	Matrix3D				CreationTM;
	OBBoxClass			GeneratingRegion;
	float					GenerationTime;
	int					GeneratingVehicleID;
	bool					IsBusy;
    bool                IsDisabled;
	ReferencerClass	Purchaser;
	int					LastDeliveryPath;
	int                 LastFlyingDeliveryPath;
	float					EndTimer;
	static int MaxVehiclesPerTeam;
}; // 08CC


#endif
