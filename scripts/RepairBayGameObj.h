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
#ifndef TT_INCLUDE__REPAIRBAYGAMEOBJ_H
#define TT_INCLUDE__REPAIRBAYGAMEOBJ_H

#include "BuildingGameObj.h"
#include "Matrix3D.h"
#include "engine_common.h"
#include "SimplePersistFactoryClass.h"
#include "RepairBayGameObjDef.h"
#include "OBBoxClass.h"
#include "SimpleGameObj.h"
class RepairBayGameObj : public BuildingGameObj
{
public:
	RepairBayGameObj (void);
	~RepairBayGameObj (void);
	virtual	void						Init( void );
	void									Init (const RepairBayGameObjDef & definition);
	SCRIPTS_API const RepairBayGameObjDef &	Get_Definition (void) const;
	RepairBayGameObj *				As_RepairBayGameObj (void)	{ return this; }
	bool									Save (ChunkSaveClass &csave);
	bool									Load (ChunkLoadClass &cload);
	const	PersistFactoryClass &	Get_Factory (void) const;
	void					CnC_Initialize (BaseControllerClass *base);
	void					Think (void);
	void								Export_Creation (BitStreamClass &packet);
	void								Import_Creation (BitStreamClass &packet);
private:
	void					Load_Variables (ChunkLoadClass &cload);
	bool					Repair_Vehicle (void);
	void					Play_Repairing_Animation (bool onoff);
	void					Update_Repairing_Animations (void);
	void					Emit_Welding_Arc (RenderObjClass *vehicle_model);
	enum
	{
		ARC_OBJ_COUNT	= 4,
		BONE_COUNT		= 6
	};
	float					RepairTimer;
	OBBoxClass			RepairZone;
	int					RepairAnimationID;
	bool					IsReparing;
	DynamicVectorClass<ReferencerClass>	VehicleList;
	StaticPhysClass *								RepairMesh;
	SimpleGameObj *		ArcObjects[ARC_OBJ_COUNT];
	float						ArcLifeRemaining[ARC_OBJ_COUNT];
	Matrix3D					EndTM;
	Matrix3D					Bones[BONE_COUNT];
	static const char *	BoneNames[BONE_COUNT];
};

#endif
