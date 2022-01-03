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
#ifndef TT_INCLUDE__MESHCLASS_H
#define TT_INCLUDE__MESHCLASS_H
#include "scripts.h"
#include "engine_common.h"
#include "engine_threading.h"
#include "engine_string.h"
#include "engine_vector.h"
#include "RenderObjClass.h"
#include "PrototypeClass.h"

struct VertexFormatXYZNDUV2;
class VertexMaterialClass;
class DX8FVFCategoryContainer;
class DecalMeshClass;
class MaterialPassClass;
class IndexBufferClass;
class DX8PolygonRendererClass;
class MeshBuilderClass;
class MeshLoadInfoClass;
enum WW3DErrorType;
class MeshModelClass;
class LightEnvironmentClass;
#ifndef TTLE_EXPORTS
class MeshClass : public RenderObjClass
{
public:
	MeshClass(void);
	MeshClass(const MeshClass & src);
private:	
	MeshClass & operator = (const MeshClass &);
public:
	virtual ~MeshClass(void);
	virtual RenderObjClass *	Clone(void) const;
	virtual int						Class_ID(void) const { return CLASSID_MESH; }
	virtual const char *			Get_Name(void) const;
	virtual void					Set_Name(const char * name);
	virtual int						Get_Num_Polys(void) const;
	virtual void					Render(RenderInfoClass & rinfo);
	void								Render_Material_Pass(MaterialPassClass * pass,IndexBufferClass * ib);
	virtual void					Special_Render(SpecialRenderInfoClass & rinfo);
	virtual bool					Cast_Ray(RayCollisionTestClass & raytest);
	virtual bool					Cast_AABox(AABoxCollisionTestClass & boxtest);
	virtual bool					Cast_OBBox(OBBoxCollisionTestClass & boxtest);
	virtual bool					Intersect_AABox(AABoxIntersectionTestClass & boxtest);
	virtual bool					Intersect_OBBox(OBBoxIntersectionTestClass & boxtest);
	virtual void					Get_Obj_Space_Bounding_Sphere(SphereClass & sphere) const;
	virtual void					Get_Obj_Space_Bounding_Box(AABoxClass & box) const;
	virtual void					Scale(float scale);
	virtual void					Scale(float scalex, float scaley, float scalez);
	virtual MaterialInfoClass * Get_Material_Info(void);
	virtual int						Get_Sort_Level(void) const;
	virtual void					Set_Sort_Level(int level);	
	virtual void					Create_Decal(DecalGeneratorClass * generator);
	virtual void					Delete_Decal(uint32 decal_id);
	WW3DErrorType					Init(const MeshBuilderClass & builder,MaterialInfoClass * matinfo,const char * name,const char * hmodelname);
	WW3DErrorType					Load_W3D(ChunkLoadClass & cload);
	void								Generate_Culling_Tree(void);
	MeshModelClass *				Get_Model(void);
	MeshModelClass *				Peek_Model(void)
	{
		return Model;
	}
	uint32							Get_W3D_Flags(void);
	const char *					Get_User_Text(void) const;
	bool								Contains(const Vector3 &point);

    void                            Get_Deformed_Vertices(Vector3* dst_vert, Vector3* dst_norm);
    void                            Get_Deformed_Vertices(Vector3* dst_vert, Vector3* dst_norm, uint32 stride);
    void                Get_Deformed_Vertices(Vector3* dst_vert);

	void								Set_Lighting_Environment(LightEnvironmentClass * light_env) { LightEnvironment = light_env; }
	LightEnvironmentClass *		Get_Lighting_Environment(void) { return LightEnvironment; }

	void								Set_Next_Visible_Skin(MeshClass * next_visible) { NextVisibleSkin = next_visible; }
	MeshClass *						Peek_Next_Visible_Skin(void) { return NextVisibleSkin; }

	void								Set_Base_Vertex_Offset(int base) { BaseVertexOffset = base; }
	int								Get_Base_Vertex_Offset(void) { return BaseVertexOffset; }
	static bool						Legacy_Meshes_Fogged;
	void								Make_Unique();
	unsigned							Get_Debug_Id() const { return  MeshDebugId; }
	void								Set_Debugger_Disable(bool b) { IsDisabledByDebugger=b; }
	bool								Is_Disabled_By_Debugger() const { return IsDisabledByDebugger; }

    void                Install_User_Lighting_Array(Vector4* lighting);
    unsigned int*       Get_User_Lighting_Array(bool alloc = false);
    virtual void                    Save_User_Lighting (ChunkSaveClass& csave);
    virtual void        Load_User_Lighting (ChunkLoadClass& cload);
    bool                Use_User_Lighting(); //should this mesh have user lighting setup on it

	void				Enable_Alternate_Material_Description(bool onoff);

protected:
	void								Free(void);
	virtual void					Add_Dependencies_To_List (DynamicVectorClass<StringClass> &file_list, bool textures_only = false);
	virtual void					Update_Cached_Bounding_Volumes(void) const;
	DX8FVFCategoryContainer*	Peek_FVF_Category_Container(void);
	void								install_materials(MeshLoadInfoClass * loadinfo);
	void								clone_materials(const MeshClass & srcmesh);
	void								setup_materials_for_user_lighting(void);
	void								setup_material_for_user_lighting(VertexMaterialClass * mtl);
	MeshModelClass *				Model;
	DecalMeshClass *				DecalMesh;
	LightEnvironmentClass *		LightEnvironment;
	int								BaseVertexOffset;
	MeshClass *						NextVisibleSkin;
	unsigned							MeshDebugId;
	bool								IsDisabledByDebugger;
	unsigned int *					UserLighting;
	MultiListClass<DX8PolygonRendererClass>		PolygonRendererList;

	friend class MeshBuilderClass;
	friend class DX8MeshRendererClass;
	friend class DX8PolygonRendererClass;
};

class PrimitivePrototypeClass : public PrototypeClass {
private:
	RenderObjClass* Proto;
public:
	PrimitivePrototypeClass(RenderObjClass* proto)
	{
		Proto = proto;
		proto->Add_Ref();
	}
	~PrimitivePrototypeClass()
	{
		Proto->Release_Ref();
	}
	virtual const char *Get_Name() const
	{
		return Proto->Get_Name();
	}
	virtual int Get_Class_ID() const
	{
		return Proto->Class_ID();
	}
	virtual RenderObjClass *Create()
	{
		return Proto->Clone();
	}
};
class MeshLoaderClass : public PrototypeLoaderClass
{
public:
	MeshLoaderClass(void)
	{
	}
	~MeshLoaderClass(void)
	{
	}
	virtual int Chunk_Type(void)
	{
		return 0;
	}
	virtual PrototypeClass *Load_W3D(ChunkLoadClass & cload);
};

#else
class MeshClass : public RenderObjClass
{
public:
	uint32							Get_W3D_Flags(void);
	const char *					Get_User_Text(void) const;
    bool                Use_User_Lighting(); //should this mesh have user lighting setup on it
    void Real_Install_User_Lighting_Array(Vector4* lighting);
    void Real_Load_User_Lighting(ChunkLoadClass& cload);
    void Install_User_Lighting_Array(Vector4* lighting);
	void setup_materials_for_user_lighting();
	void setup_material_for_user_lighting(VertexMaterialClass* material);
	void Load_User_Lighting(ChunkLoadClass& cload);
protected:
	MeshModelClass *model;
};
#endif
#endif
