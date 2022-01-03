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

#ifndef TT_INCLUDE__PHYSICSSCENECLASS_H
#define TT_INCLUDE__PHYSICSSCENECLASS_H



#include "SceneClass.h"
#include "VisTableMgrClass.h"
#include "VisTableClass.h"
#include "WidgetUserClass.h"
#include "multilist.h"
#include "PhysClass.h"
#include "AABTreeCull.h"
#include "GridCull.h"

class TextureClass;
class CameraClass;
class WidgetUserClass;
class AABoxClass;
class LightEnvironmentClass;
class PhysDecalSysClass;
class PathfindClass;
class CameraShakeSystemClass;
class StaticAABTreeCullClass;
class StaticLightCullClass;
class PhysGridCullClass;
class DynamicAABTreeCullClass;
class AABTreeCullSystemClass;
class GridCullSystemClass;
class LightClass;
class TexProjectClass;
class StaticPhysClass;
class VisTableClass;
class VisRenderContextClass;
class VisSampleClass;
class MeshClass;
class FrustumClass;
class VisSectorStatsClass;
class VisOptProgressClass;
enum VisDirBitsType;

class PhysicsSceneClass :
	public SceneClass, public WidgetUserClass
{
public:
	PhysicsSceneClass(void);
	virtual ~PhysicsSceneClass(void);
	static PhysicsSceneClass * Get_Instance(void) { return TheScene; }
	void Update(float dt,int frameid);
	void Pre_Render_Processing(CameraClass & camera);
	void Post_Render_Processing(void);
	void Add_Dynamic_Object(PhysClass * newobj);
	void Add_Static_Object(StaticPhysClass * newtile,int cull_node_id = -1);
	void Add_Dynamic_Light(LightPhysClass * light);
	void Add_Static_Light(LightPhysClass * newlight,int cull_node_id = -1);
	void Remove_Object(PhysClass * obj);
	void Remove_All(void);
	void Delayed_Remove_Object(PhysClass * obj);
	void Process_Release_List(void);
	bool Contains(PhysClass * obj);
	RefMultiListIterator<PhysClass> Get_Dynamic_Object_Iterator(void);
	RefMultiListIterator<PhysClass> Get_Static_Object_Iterator(void);
	RefMultiListIterator<PhysClass> Get_Static_Anim_Object_Iterator(void);
	RefMultiListIterator<PhysClass> Get_Static_Light_Iterator(void);
	MultiListIterator<TexProjectClass> Get_Static_Projector_Iterator(void);
	MultiListIterator<TexProjectClass> Get_Dynamic_Projector_Iterator(void);
	StaticPhysClass *Get_Static_Object_By_ID(uint32 id);
	void Add_To_Dirty_Cull_List(PhysClass *obj);
	void Remove_From_Dirty_Cull_List(PhysClass *obj);
	bool Is_In_Dirty_Cull_List(PhysClass *obj);
	enum {
		LIGHTING_MODE_NONE	= 0,
		LIGHTING_MODE_SURRENDER,
		LIGHTING_MODE_CHEAP
	};
	int Get_Lighting_Mode(void) {return LightingMode;}
	void Set_Lighting_Mode(int mode) {LightingMode = mode;}
	virtual void Set_Ambient_Light(const Vector3 &color) {SceneAmbientLight = color;}
	virtual const Vector3 &Get_Ambient_Light(void) {return SceneAmbientLight;}
	void Set_Lighting_LOD_Cutoff(float intensity);
	float Get_Lighting_LOD_Cutoff(void);
	bool Is_Sun_Light_Enabled(void);
	void Enable_Sun_Light(bool onoff);
	LightClass *Get_Sun_Light(void);
	void Set_Sun_Light_Orientation(float rotation,float elevation);
	void Get_Sun_Light_Orientation(float * set_rotation,float * set_elevation);
	void Get_Sun_Light_Vector(Vector3 * set_vector);
	void Compute_Static_Lighting(LightEnvironmentClass *light_env,const Vector3 & obj_center,bool use_sun,int vis_object_id);
	void Invalidate_Lighting_Caches(const AABoxClass & bounds);
	void Set_Collision_Region(const AABoxClass & bounds,int colgroup);
	void Release_Collision_Region(void);
	SCRIPTS_API bool Cast_Ray(PhysRayCollisionTestClass & raytest,bool use_collision_region = false);
	SCRIPTS_API bool Cast_AABox(PhysAABoxCollisionTestClass & boxtest,bool use_collision_region = false);
	SCRIPTS_API bool Cast_OBBox(PhysOBBoxCollisionTestClass & boxtest,bool use_collision_region = false);
	bool Intersection_Test(PhysAABoxIntersectionTestClass & boxtest,bool use_collision_region = false);
	bool Intersection_Test(PhysOBBoxIntersectionTestClass & boxtest,bool use_collision_region = false);
	bool Intersection_Test(PhysMeshIntersectionTestClass & meshtest,bool use_collision_region = false);
	bool Intersection_Test(const AABoxClass & box,int collision_group,int collision_type,bool use_collision_region = false);
	bool Intersection_Test(const OBBoxClass & box,int collision_group,int collision_type,bool use_collision_region = false);
	void Force_Dynamic_Objects_Awake(const AABoxClass & box);
	void Collect_Objects(const Vector3 & point,bool static_objs, bool dynamic_objs,MultiListClass<PhysClass> * list);
	void Collect_Objects(const AABoxClass & box,bool static_objs, bool dynamic_objs,MultiListClass<PhysClass> * list);
	SCRIPTS_API void Collect_Objects(const OBBoxClass & box,bool static_objs, bool dynamic_objs,MultiListClass<PhysClass> * list);
	void Collect_Objects(const FrustumClass & frustum,bool static_objs, bool dynamic_objs,MultiListClass<PhysClass> * list);
	void Collect_Collideable_Objects(const AABoxClass & box,int colgroup,bool static_objs, bool dynamic_objs,MultiListClass<PhysClass> * list);
	void Collect_Collideable_Objects(const OBBoxClass & box,int colgroup,bool static_objs, bool dynamic_objs,MultiListClass<PhysClass> * list);
	void Collect_Lights(const Vector3 & point,bool static_lights,bool dynamic_lights,MultiListClass<PhysClass> * list);
	void Collect_Lights(const AABoxClass & bounds,bool static_lights,bool dynamic_lights,MultiListClass<PhysClass> * list);
	StaticPhysClass *Find_Static_Object(int instance_id);
	enum {
		COLLISION_GROUP_WORLD	= 0x0F,
		MAX_COLLISION_GROUP		= 0x1F,	// previous value was 0F
		NUM_COLLISION_FLAGS		= 1024,	// previous value was 256
		COLLISION_FLAG_SHIFT	= 5,	// previous value was 4
	};
	void Enable_Collision_Detection(int group0,int group1);
	void Disable_Collision_Detection(int group0,int group1);
	void Enable_All_Collision_Detections(int group);
	void Disable_All_Collision_Detections(int group);
	bool Do_Groups_Collide(int group0,int group1);
	void Re_Partition_Static_Objects(void);
	void Re_Partition_Static_Lights(void);
	void Re_Partition_Static_Projectors(void);
	void Re_Partition_Dynamic_Culling_System(void);
	void Re_Partition_Dynamic_Culling_System(DynamicVectorClass<AABoxClass> & virtual_occludees);
	void Update_Culling_System_Bounding_Boxes(void);
	bool Verify_Culling_Systems(StringClass & set_error_report);
	void Enable_Vis(bool onoff);
	void Invert_Vis(bool onoff);
	void Set_Vis_Quick_And_Dirty(bool onoff);
	void Enable_Vis_Sector_Display(bool onoff);
	void Enable_Vis_Sector_History_Display(bool onoff);
	void Enable_Vis_Sector_Fallback(bool onoff);
	void Enable_Backface_Occluder_Debug(bool onoff);
	bool Is_Vis_Enabled(void);
	bool Is_Vis_Inverted(void);
	bool Is_Vis_Quick_And_Dirty(void);
	bool Is_Vis_Sector_Display_Enabled(void);
	bool Is_Vis_Sector_History_Display_Enabled(void);
	bool Is_Vis_Sector_Missing(void);
	bool Is_Vis_Sector_Fallback_Enabled(void);
	bool Is_Backface_Occluder_Debug_Enabled(void);
	void Lock_Vis_Sample_Point(bool onoff);
	bool Is_Vis_Sample_Point_Locked(void);
	void Compute_Vis_Sample_Point(const CameraClass & camera,Vector3 * set_point);
	VisTableClass *Get_Vis_Table(int vis_sector_id);
	VisTableClass *Get_Vis_Table(const Vector3 & point);
	VisTableClass *Get_Vis_Table(const CameraClass & camera);
	VisTableClass *Get_Vis_Table_For_Rendering(const CameraClass & camera);
	virtual void On_Vis_Occluders_Rendered(VisRenderContextClass & context,VisSampleClass & sample) {}
	enum {
		VIS_GRID_DISPLAY_NONE = 0,
		VIS_GRID_DISPLAY_ACTUAL_BOXES,
		VIS_GRID_DISPLAY_CENTERS,
		VIS_GRID_DISPLAY_OCCUPIED
	};
	void Set_Vis_Grid_Display_Mode(int mode);
	int Get_Vis_Grid_Display_Mode(void);
	void Vis_Grid_Debug_Reset_Node(void);
	bool Vis_Grid_Debug_Enter_Parent(void);
	bool Vis_Grid_Debug_Enter_Sibling(void);
	bool Vis_Grid_Debug_Enter_Front_Child(void);
	bool Vis_Grid_Debug_Enter_Back_Child(void);
	uint32 Get_Dynamic_Object_Vis_ID(const AABoxClass & obj_bounds,int * node_id = NULL);
	void Debug_Display_Dynamic_Vis_Node(int node_id);
	virtual void Reset_Vis(bool doitnow = false);
	void Validate_Vis(void);
	int Allocate_Vis_Object_ID(int count = 1);
	int Allocate_Vis_Sector_ID(int count = 1);
	int Get_Vis_Table_Size(void);
	int Get_Vis_Table_Count(void);
	int Get_Static_Light_Count(void);
	void Generate_Vis_For_Light(int light_index);
	void Export_Vis_Data(ChunkSaveClass & csave);
	void Import_Vis_Data(ChunkLoadClass & cload);
	void Show_Vis_Window(bool onoff);
	bool Is_Vis_Window_Visible(void);

	void Enable_Static_Projectors(bool onoff);
	bool Are_Static_Projectors_Enabled(void);
	void Set_Static_Shadow_Resolution(uint res);
	uint Get_Static_Shadow_Resolution();
	void Enable_Dynamic_Projectors(bool onoff);
	bool Are_Dynamic_Projectors_Enabled(void);
	void Set_Dynamic_Shadow_Resolution(uint res);
	uint Get_Dynamic_Shadow_Resolution();
	void Add_Static_Texture_Projector(TexProjectClass* newprojector);
	void Remove_Static_Texture_Projector(TexProjectClass* projector);
	void Add_Dynamic_Texture_Projector(TexProjectClass* newprojector);
	void Remove_Dynamic_Texture_Projector(TexProjectClass* projector);
	void Remove_Texture_Projector(TexProjectClass* projector);
	bool Contains(TexProjectClass* projector);
	
	enum ShadowEnum
	{
		SHADOW_MODE_NONE = 0,
		SHADOW_MODE_BLOBS,
		SHADOW_MODE_BLOBS_PLUS,
		SHADOW_MODE_HARDWARE,
		SHADOW_MODE_COUNT,
	};
	
	void Set_Shadow_Mode(ShadowEnum shadow_mode);
	ShadowEnum Get_Shadow_Mode(void);
	void Set_Shadow_Attenuation(float znear,float zfar);
	void Get_Shadow_Attenuation(float * set_znear,float * set_zfar);
	void Set_Shadow_Normal_Intensity(float normal_intensity);
	float Get_Shadow_Normal_Intensity(void);
	void Set_Shadow_Resolution(unsigned int res);
	unsigned int Get_Shadow_Resolution(void);
	void Set_Max_Simultaneous_Shadows(unsigned int count);
	unsigned int Get_Max_Simultaneous_Shadows(void);
	CameraClass *Get_Shadow_Camera(void);
	SpecialRenderInfoClass *Get_Shadow_Render_Context();
	MaterialPassClass *Get_Shadow_Material_Pass(void);
	void Invalidate_Static_Shadow_Projectors(void);
	void Generate_Static_Shadow_Projectors(void);
	void Generate_Static_Directional_Shadow(StaticAnimPhysClass& obj, const Vector3& light_dir);
	void Generate_Vis_Statistics_Report(DynamicVectorClass<VisSectorStatsClass> & report);
	void Optimize_Visibility_Data(VisOptProgressClass & progress_status);
	VisSampleClass				Update_Vis(const Matrix3D & camera,VisDirBitsType direction_bits);
	VisSampleClass				Update_Vis(const Vector3 & sample_point,const Matrix3D & camera,VisDirBitsType direction_bits,CameraClass * alternate_camera = NULL,int user_vis_id = -1);

	int Create_Decal(const Matrix3D &tm,const char *texture_name,float radius,bool is_permanent = false,bool apply_to_translucent_polys = false,PhysClass *only_this_obj = NULL);
	bool Remove_Decal(uint32 id);
	void Shatter_Mesh(MeshClass * mesh,const Vector3 & impact_point,const Vector3 & impact_normal,const Vector3 & impact_velocity);
	void Add_Camera_Shake(const Vector3 & position,float radius = 50.0f,float duration = 1.5f,float power = 1.0f);
	void Apply_Camera_Shakes( CameraClass & camera );
	bool Is_Vis_Reset_Needed (void) const { return VisResetNeeded; }
	void Vis_Reset_Needed(bool needed) { VisResetNeeded = needed; }
	void Save_Level_Static_Data(ChunkSaveClass & csave);
	void Load_Level_Static_Data(ChunkLoadClass & cload);
	void Post_Load_Level_Static_Data(void);
	void Save_Level_Static_Objects(ChunkSaveClass & csave);
	void Load_Level_Static_Objects(ChunkLoadClass & cload);
	void Post_Load_Level_Static_Objects(void);
	void Save_Level_Dynamic_Data(ChunkSaveClass & csave);
	void Load_Level_Dynamic_Data(ChunkLoadClass & cload);
	void Post_Load_Level_Dynamic_Data(void);
	void Set_Polygon_Budgets(int static_count,int dynamic_count);
	void Get_Polygon_Budgets(int * static_count,int * dynamic_count);
#ifdef TT_EXPORTS
	void Refresh_Polygon_Budgets();
#endif
	void Set_Update_Only_Visible_Objects(bool b) { UpdateOnlyVisibleObjects=b; }
	bool Get_Update_Only_Visible_Objects() { return UpdateOnlyVisibleObjects; }
	virtual void Add_Render_Object(RenderObjClass * obj);
	virtual void Remove_Render_Object(RenderObjClass * obj);
	virtual SceneIterator *Create_Iterator(bool) {return NULL;}
	virtual void Destroy_Iterator(SceneIterator *) {}
	virtual void Register(RenderObjClass * obj,RegType for_what);
	virtual void Unregister(RenderObjClass * obj,RegType for_what);
	SCRIPTS_API void Get_Level_Extents (Vector3 &min, Vector3 &max);
	void Enable_Debug_Display(bool onoff) { DebugDisplayEnabled = onoff; }
	bool Is_Debug_Display_Enabled(void) { return DebugDisplayEnabled; }
	void Enable_Projector_Debug_Display(bool onoff) { ProjectorDebugDisplayEnabled = onoff; }
	bool Is_Projector_Debug_Display_Enabled(void) { return ProjectorDebugDisplayEnabled; }
	void Enable_Dirty_Cull_Debug_Display(bool onoff) { DirtyCullDebugDisplayEnabled = onoff; }
	bool Is_Dirty_Cull_Debug_Display_Enabled(void) { return DirtyCullDebugDisplayEnabled; }
	void Enable_Lighting_Debug_Display(bool onoff) { LightingDebugDisplayEnabled = onoff; }
	bool Is_Lighting_Debug_Display_Enabled(void) { return LightingDebugDisplayEnabled; }
	const Vector3 &Get_Last_Camera_Position(void) { return LastCameraPosition; }
	struct StatsStruct
	{
		StatsStruct(void);
		void Reset(void);
		int	FrameCount;
		int	CullNodeCount;
		int	CullNodesAccepted;
		int	CullNodesTriviallyAccepted;
		int	CullNodesRejected;
	};
	void Per_Frame_Statistics_Update(void);
	const StatsStruct &Get_Statistics(void);
	float Compute_Vis_Mesh_Ram(void);
protected:
	virtual void Customized_Render(RenderInfoClass & rinfo);
	void Render_Objects(RenderInfoClass& rinfo,RefMultiListClass<PhysClass> * static_ws_list,RefMultiListClass<PhysClass> * static_list,RefMultiListClass<PhysClass> * dyn_list);
	void Render_Object(RenderInfoClass & context,PhysClass * obj);
	void Render_Backface_Occluders(RenderInfoClass & context,RefMultiListClass<PhysClass> *  static_ws_list,RefMultiListClass<PhysClass> * static_list);
	void Optimize_LODs(CameraClass & camera,RefMultiListClass<PhysClass> * dyn_obj_list,RefMultiListClass<PhysClass> * static_obj_list,RefMultiListClass<PhysClass> * static_ws_mesh_list);
	void Release_Vis_Resources(void);
	virtual void Internal_Vis_Reset(void);
	CameraClass *Get_Vis_Camera(void);
	void Vis_Render_And_Scan(VisRenderContextClass & context,VisSampleClass & sample);
	void Merge_Vis_Sector_IDs(uint32 id0,uint32 id1);
	void Merge_Vis_Object_IDs(uint32 id0,uint32 id1);
public:
	void Release_Projector_Resources(void);
protected:
	void Apply_Projectors(const CameraClass & camera);
	void Apply_Projector_To_Objects(TexProjectClass * tex_proj,const CameraClass & camera);
	float Compute_Projector_Attenuation(TexProjectClass * dynamic_projector,const Vector3 & view_pos,const Vector3 & view_dir);
public:
	void Allocate_Decal_Resources(void);
	void Release_Decal_Resources(void);
protected:
	void Save_LDD_Variables(ChunkSaveClass & csave);
	void Save_Static_Objects(ChunkSaveClass & csave);
	void Save_Dynamic_Objects(ChunkSaveClass & csave);
	void Save_Static_Lights(ChunkSaveClass & csave);
	void Save_Static_Object_States(ChunkSaveClass & csave);
	void Load_LDD_Variables(ChunkLoadClass & cload);
	void Load_Static_Objects(ChunkLoadClass & cload);
	void Load_Dynamic_Objects(ChunkLoadClass & cload);
	void Load_Static_Lights(ChunkLoadClass & cload);
	void Load_Static_Object_States(ChunkLoadClass & cload);
	void Internal_Add_Dynamic_Object(PhysClass * newobj);
	void Internal_Add_Static_Object(StaticPhysClass * newtile);
	void Internal_Add_Static_Light(LightPhysClass * newlight);
	void Reset_Sun_Light(void);
	void Load_Sun_Light(ChunkLoadClass & cload);
	void Save_Sun_Light(ChunkSaveClass & csave);
	void Add_Collected_Objects_To_List(bool static_objs,bool dynamic_objs,MultiListClass<PhysClass> * list);
	void Add_Collected_Collideable_Objects_To_List(int colgroup,bool static_objs,bool dynamic_objs,MultiListClass<PhysClass> * list);
	void Add_Collected_Lights_To_List(bool static_lights,bool dynamic_lights,MultiListClass<PhysClass> * list);
	RefMultiListClass<PhysClass> VisibleStaticObjectList;
	RefMultiListClass<PhysClass> VisibleWSMeshList;
	RefMultiListClass<PhysClass> VisibleDynamicObjectList;
	MultiListClass<TexProjectClass> ActiveTextureProjectors;
	int FrameNum;
	Vector3 LastCameraPosition;
	int LastValidVisId;
	bool DebugDisplayEnabled;
	bool ProjectorDebugDisplayEnabled;
	bool DirtyCullDebugDisplayEnabled;
	bool LightingDebugDisplayEnabled;
	StatsStruct LastValidStats;
	StatsStruct CurrentStats;
	bool StaticProjectorsDirty;
	enum 
	{ 
		VIS_RENDER_WIDTH = 256,
		VIS_RENDER_HEIGHT = 256
	};
	bool VisEnabled;
	bool VisInverted;
	bool VisQuickAndDirty;
	bool VisResetNeeded;
	bool VisSectorDisplayEnabled;
	bool VisSectorHistoryEnabled;
	int VisGridDisplayMode;
	bool VisSectorMissing;
	bool VisSectorFallbackEnabled;
	bool BackfaceDebugEnabled;
	bool VisSamplePointLocked;
	Vector3 LockedVisSamplePoint;
	CameraClass *VisCamera;
	VisTableClass *CurrentVisTable;
	bool StaticProjectorsEnabled;
	bool DynamicProjectorsEnabled;
	ShadowEnum ShadowMode;
	float ShadowAttenStart;
	float ShadowAttenEnd;
	float ShadowNormalIntensity;
	TextureClass *ShadowBlobTexture;
	SpecialRenderInfoClass *ShadowRenderContext;
	CameraClass *ShadowCamera;
	MaterialPassClass *ShadowMaterialPass;
	int ShadowResWidth;
	int ShadowResHeight;
	PhysDecalSysClass *DecalSystem;
	PathfindClass *Pathfinder;
	CameraShakeSystemClass *CameraShakeSystem;
	RefMultiListClass<RenderObjClass> UpdateList;
	RefMultiListClass<RenderObjClass> VertexProcList;
	RefMultiListClass<PhysClass> ReleaseList;
	MaterialPassClass *HighlightMaterialPass;
	StaticAABTreeCullClass *StaticCullingSystem;
	StaticLightCullClass *StaticLightingSystem;
	PhysGridCullClass *DynamicCullingSystem;
	DynamicAABTreeCullClass *DynamicObjVisSystem;
	TypedAABTreeCullSystemClass<TexProjectClass> *StaticProjectorCullingSystem;
	TypedGridCullSystemClass<TexProjectClass> *DynamicProjectorCullingSystem;
	VisTableMgrClass VisTableManager;
	int LightingMode;
	Vector3 SceneAmbientLight;
	bool UseSun;
	float SunPitch;
	float SunYaw;
	LightClass *SunLight;
	static bool AllowCollisionFlags[NUM_COLLISION_FLAGS];
	int DynamicPolyBudget;
	int StaticPolyBudget;
	RefMultiListClass<PhysClass> ObjList;
	RefMultiListClass<PhysClass> StaticObjList;
	RefMultiListClass<PhysClass> StaticLightList;
	MultiListClass<TexProjectClass> StaticProjectorList;
	MultiListClass<TexProjectClass> DynamicProjectorList;
	RefMultiListClass<PhysClass> DirtyCullList;
	RefMultiListClass<PhysClass> TimestepList;
	RefMultiListClass<PhysClass> StaticAnimList;
	MultiListClass<PhysClass> CollisionRegionList;
	bool UpdateOnlyVisibleObjects;
public:
	unsigned CurrentFrameNumber;
private:
#ifndef W3DVIEWER
	SCRIPTS_API static REF_DECL(PhysicsSceneClass*, TheScene);
#else
	static PhysicsSceneClass *TheScene;
#endif
	friend class WW3D;
	friend class VisOptimizationContextClass;
#ifdef TT_EXPORTS
	float mapStaticPolyBudget;
	float mapDynamicPolyBudget;
#endif
}; // 792, WIN: 700
#endif
