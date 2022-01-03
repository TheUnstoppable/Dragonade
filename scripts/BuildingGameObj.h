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
#ifndef TT_INCLUDE__BUILDINGGAMEOBJ_H
#define TT_INCLUDE__BUILDINGGAMEOBJ_H

#include "scripts.h"
#include "engine_common.h"
#include "engine_threading.h"
#include "engine_string.h"
#include "engine_vector.h"
#include "SphereClass.h"
#include "BuildingAggregateClass.h"
#include "LightPhysClass.h"
#include "DamageableGameObj.h"
#include "BuildingGameObjDef.h"
class BuildingMonitorClass;
class PowerPlantGameObj;
class SoldierFactoryGameObj;
class VehicleFactoryGameObj;
class AirStripGameObj;
class WarFactoryGameObj;
class RefineryGameObj;
class ComCenterGameObj;
class RepairBayGameObj;
class AudibleSoundClass;
class BaseControllerClass;
class ConstructionYardGameObj;
class AirFactoryGameObj;
class NavalFactoryGameObj;
class SuperweaponGameObj;
class BuildingGameObj : public DamageableGameObj, public CombatPhysObserverClass
{
public:
	BuildingGameObj( void );
	virtual	~BuildingGameObj( void );
	virtual	void	Init( void );
	void	Init( const BuildingGameObjDef & definition );
	const BuildingGameObjDef SCRIPTS_API & Get_Definition( void ) const ;
	virtual	bool	Save( ChunkSaveClass & csave );
	virtual	bool	Load( ChunkLoadClass & cload );
	virtual	const	PersistFactoryClass & Get_Factory( void ) const;
	virtual	BuildingGameObj	*		As_BuildingGameObj( void )			{ return this; }
	virtual	DamageableGameObj	*		As_DamageableGameObj( void )		{ return this; }
	virtual	PowerPlantGameObj *		As_PowerPlantGameObj (void)		{ return NULL; }
	virtual	SoldierFactoryGameObj *	As_SoldierFactoryGameObj (void)	{ return NULL; }
	virtual	VehicleFactoryGameObj *	As_VehicleFactoryGameObj (void)	{ return NULL; }
	virtual	AirStripGameObj *			As_AirStripGameObj (void)			{ return NULL; }
	virtual	WarFactoryGameObj *		As_WarFactoryGameObj (void)		{ return NULL; }
	virtual	RefineryGameObj *			As_RefineryGameObj (void)			{ return NULL; }
	virtual	ComCenterGameObj *		As_ComCenterGameObj (void)			{ return NULL; }
	virtual	RepairBayGameObj *		As_RepairBayGameObj (void)			{ return NULL; }
	void											Get_Position(Vector3 * pos) const		{ *pos = Position; }
	void											Set_Position(const Vector3 & pos)		{ Position = pos; CollectionSphere.Center = pos; }
	virtual	void								Apply_Damage( const OffenseObjectClass & damager, 
																	  float scale = 1.0f, 
																	  int alternate_skin = -1 );
	void											Apply_Damage_Building(	const OffenseObjectClass & offense,
																				StaticPhysClass * component ); 
	void											Apply_Damage_Building(	const OffenseObjectClass & offense,
																				bool mct_damage ); 
	void											Set_Normalized_Health (float health);
	void											Enable_Power(bool onoff);
	bool											Is_Power_Enabled(void) const			{ return IsPowerOn; }
	void											Play_Announcement( int text_id, bool broadcast );
	void											Stop_Current_Announcement( void );
	bool											Name_Prefix_Matches_This_Building(const char * name);
	static bool									Is_Interior_Mesh_Name(const char * name);
	static bool									Is_Exterior_Mesh_Name(const char * name);
	const char *								Get_Name_Prefix (void) const { return Get_Definition().MeshPrefix; }
	void											Collect_Building_Components (void);
	void											Get_Collection_Sphere (SphereClass *sphere) const	{ *sphere = CollectionSphere; }
	void											Set_Collection_Sphere (const SphereClass &sphere)	{ CollectionSphere = sphere; }
	virtual void								CnC_Initialize (BaseControllerClass *base);
	virtual void								On_Destroyed (void);
	virtual void								On_Damaged (void);
	virtual void								On_Revived (void);
	bool											Is_Destroyed (void) const { return IsDestroyed; }
	void Set_Is_Destroyed(const bool _IsDestroyed) { IsDestroyed = _IsDestroyed; }
	bool											Is_Spy_Disabled() const { return IsSpyDisabled; }
	void Set_Is_Spy_Disabled(const bool _IsSpyDisabled) {IsSpyDisabled = _IsSpyDisabled; }
	virtual	void								Import_Rare (BitStreamClass &packet);
	virtual	void								Export_Rare (BitStreamClass &packet);
	virtual	void								Export_Creation (BitStreamClass &packet);
	virtual	void								Import_Creation (BitStreamClass &packet);
	static void									Set_Precision (void);
	virtual bool								Get_World_Position (Vector3 &pos) const	{ pos = Position; return true; }
	virtual	void								Get_Description( StringClass & description );
	virtual bool								Is_Tagged(void)									{ return false; }
	SCRIPTS_API BuildingAggregateClass *                    Find_MCT();
	bool	Is_GDI( void )						{ return Get_Player_Type() == PLAYERTYPE_GDI; }
	bool	Is_Nod( void )						{ return Get_Player_Type() == PLAYERTYPE_NOD; }
#ifndef TTLE_EXPORTS
#ifndef W3DVIEWER
    const AABoxClass& Get_Bounds()                      { return BoundingBox; }
	static void	Set_Can_Repair_Buildings(bool flag)		{CanRepairBuildings = flag;}
	static bool	Get_Can_Repair_Buildings(void)			{return CanRepairBuildings;}
#endif
#endif
	SCRIPTS_API void											Find_Closest_Poly (const Vector3 &pos, float *distance2);
	float Find_Closest_Poly(const Vector3 &pos) { float distance2; Find_Closest_Poly(pos,&distance2); return distance2; }
	void Find_Closest_Poly(const Vector3& pos, float* distance2, Vector3* polyPos);
	int											Building_In_Range(const Vector3 &point, float range);	//return 0 for no, 1 for yes and 2 for MCT in range
	bool										Is_In_Range_Coarse(const Vector3& point, float range_sq);
	bool Cast_Ray(RayCollisionTestClass& raytest);

	virtual ConstructionYardGameObj *				As_ConstructionYardGameObj (void)	{ return NULL; }
	virtual	AirFactoryGameObj *		As_AirFactoryGameObj (void)			{ return NULL; }
	virtual	NavalFactoryGameObj *		As_NavalFactoryGameObj (void)			{ return NULL; }
	virtual	SuperweaponGameObj *		As_SuperweaponGameObj (void)			{ return NULL; }

	int Get_State() { return CurrentState; } //DA
	BaseControllerClass *Get_Base() { return BaseController; } //DA
	void Update_State(bool force_update = false); //DA
	static REF_DECL(bool, CanRepairBuildings); //DA

protected:
	BuildingMonitorClass *					BuildingMonitor;
	BaseControllerClass *					BaseController;
	bool											IsDestroyed;
	Vector3										Position;
	bool											IsPowerOn;
	bool											IsSpyDisabled;
	int											CurrentState;						// derived from the health and IsPowerOn
	AudibleSoundClass *						CurrentAnnouncement;
	SphereClass									AnnouncementSphere;
	SphereClass									CollectionSphere;
	RefMultiListClass<StaticPhysClass>				InteriorMeshes;
	RefMultiListClass<StaticPhysClass>				ExteriorMeshes;
	RefMultiListClass<BuildingAggregateClass>		Aggregates;
	RefMultiListClass<LightPhysClass>				PowerOnLights;
	RefMultiListClass<LightPhysClass>				PowerOffLights;
#ifndef TTLE_EXPORTS
#ifndef W3DVIEWER
	AABoxClass										BoundingBox;
	//static REF_DECL(bool, CanRepairBuildings); //DA
#endif
#endif
	void											Initialize_Building(void);	
	void											Reset_Components(void);
	void											Add_Mesh(StaticPhysClass * terrain);
	void											Remove_Mesh(StaticPhysClass * terrain);
	void											Add_Aggregate(BuildingAggregateClass * aggregate);
	void											Remove_Aggregate(BuildingAggregateClass * aggregate);
	void											Add_Light(LightPhysClass * light);
	void											Find_Closest_Poly_For_Model (RenderObjClass *model, const Vector3 &pos, float *distance2, Vector3* polyPos = NULL);
	//void											Update_State(bool force_update = false); //DA
	void											Enable_Alternate_Materials(RefMultiListClass<StaticPhysClass> & models, bool onoff);
	void											Enable_Alternate_Materials(RenderObjClass * model,bool onoff);
	friend class GameObjManager;
}; // 2092, RH7: 2112


#endif
