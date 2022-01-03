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
#ifndef TT_INCLUDE_GRIDCULL_H
#define TT_INCLUDE_GRIDCULL_H
#include "cullsystemclass.h"
#include "frustumclass.h"
#include "aaboxclass.h"
#include "obboxclass.h"
class ChunkLoadClass;
class ChunkSaveClass;
class GridCullSystemClass : public CullSystemClass
{
public:
	GridCullSystemClass(void);
	~GridCullSystemClass(void);
	virtual void		Collect_Objects(const Vector3 & point);
	virtual void		Collect_Objects(const AABoxClass & box);
	virtual void		Collect_Objects(const OBBoxClass & box);
	virtual void		Collect_Objects(const FrustumClass & frustum);
	virtual void		Re_Partition(const Vector3 & min,const Vector3 & max,float objdim);
	virtual void		Update_Culling(CullableClass * obj);
	virtual void		Load(ChunkLoadClass & cload);
	virtual void		Save(ChunkSaveClass & csave);
	virtual int			Get_Object_Count (void) const { return ObjCount; }
	struct StatsStruct
	{
		int				NodeCount;
		int				NodesAccepted;
		int				NodesTriviallyAccepted;
		int				NodesRejected;
	};
	void					Reset_Statistics(void);
	const StatsStruct & Get_Statistics(void);
	void					Get_Min_Cell_Size (Vector3 &size) const	{ size = MinCellSize; }
	void					Set_Min_Cell_Size (const Vector3 &size)	{ MinCellSize = size; }
	int					Get_Termination_Count (void) const	{ return TerminationCellCount; }
	void					Set_Termination_Count (int count)	{ TerminationCellCount = count; }
protected:
	void					Collect_And_Unlink_All(void);
	void					Add_Object_Internal(CullableClass * obj);
	void					Remove_Object_Internal(CullableClass * obj);
	enum 
	{
		TERMINATION_CELL_COUNT	= 16384,
		UNGRIDDED_ADDRESS			= 0xFFFFFFFF
	};
	Vector3				MinCellSize;			// min dimensions for a cell (don't go below this...)
	float					MaxObjExtent;			// max extent/radius (objects bigger than this are just put in a list)
	int					TerminationCellCount;
	Vector3				Origin;
	Vector3				CellDim;
	Vector3				OOCellDim;
	int					CellCount[3];
	CullableClass **	Cells;	
	CullableClass *	NoGridList;
	int					ObjCount;
	StatsStruct				Stats;
	struct VolumeStruct
	{
		VolumeStruct(void)
		{
		}
		VolumeStruct(int i0,int j0,int k0,int i1,int j1,int k1)
		{
			Min[0] = i0;
			Min[1] = j0;
			Min[2] = k0;
			Max[0] = i1;
			Max[1] = j1;
			Max[2] = k1;
		}
		bool Is_Leaf(void) const
		{
			return ((Max[0]-Min[0] == 1) && (Max[1]-Min[1] == 1) && (Max[2]-Min[2] == 1)); 
		}
		bool Is_Empty(void) const
		{
			return ((Max[0]-Min[0] <= 0) || (Max[1]-Min[1] <= 0) || (Max[2]-Min[2] <= 0));
		}
		void Split(VolumeStruct & v0,VolumeStruct & v1) const
		{
			int split_axis = 0;
			int delta[3];
			delta[0] = Max[0] - Min[0];
			delta[1] = Max[1] - Min[1];
			delta[2] = Max[2] - Min[2];
			if (delta[1] > delta[split_axis]) split_axis = 1;
			if (delta[2] > delta[split_axis]) split_axis = 2;
			memcpy(&v0,this,sizeof(VolumeStruct));
			memcpy(&v1,this,sizeof(VolumeStruct));
			v0.Max[split_axis] = v1.Min[split_axis] = Min[split_axis] + (delta[split_axis] >> 1);
		}
		int Min[3];
		int Max[3];
	};
	void					link_object(CullableClass * obj);
	void					link_object(CullableClass * obj,int address);
	void					unlink_object(CullableClass * obj);
	void					link_object_to_list(CullableClass ** head,CullableClass * obj);
	void					unlink_object_from_list(CullableClass ** head,CullableClass * obj);
	bool					map_point_to_cell(const Vector3 & pt,int & set_i,int & set_j,int & set_k)
	{
		Vector3 dp = pt - Origin;
		set_i = (int)floor(dp.X * OOCellDim.X);
		set_j = (int)floor(dp.Y * OOCellDim.Y);
		set_k = (int)floor(dp.Z * OOCellDim.Z);
		if (	(set_i >= 0) && (set_j >= 0) && (set_k >= 0) && (set_i < CellCount[0]) && (set_j < CellCount[1]) && (set_k < CellCount[2])	)
		{
			return true;
		}
		return false;
	}
	bool					map_point_to_address(const Vector3 & pt,int & set_address)
	{
		int i,j,k;
		bool res = map_point_to_cell(pt,i,j,k);
		if (res)
		{
			set_address = map_indices_to_address(i,j,k);
			return true;
		}
		else
		{
			set_address = UNGRIDDED_ADDRESS;
			return false;
		}
	}
	int	map_indices_to_address(int i,int j,int k)
	{
		return i + j*CellCount[0] + k*CellCount[0]*CellCount[1];
	}
	void clamp_indices_to_grid(int * i,int * j,int * k)
	{
		if (*i < 0) *i = 0;
		if (*i >= CellCount[0]) *i = CellCount[0] - 1;
		if (*j < 0) *j = 0;
		if (*j >= CellCount[1]) *j = CellCount[1] - 1;
		if (*k < 0) *k = 0;
		if (*k >= CellCount[2]) *k = CellCount[2] - 1;
	}
	int total_cell_count(void)
	{
		return CellCount[0] * CellCount[1] * CellCount[2];
	}
	void compute_box(int i,int j,int k,AABoxClass * set_box)
	{
		Vector3 min,max;
		min.X = Origin.X + i * CellDim.X - MaxObjExtent;
		min.Y = Origin.Y + j * CellDim.Y - MaxObjExtent;
		min.Z = Origin.Z + k * CellDim.Z - MaxObjExtent;
		max.X = min.X + CellDim.X + 2.0f*MaxObjExtent;
		max.Y = min.Y + CellDim.Y + 2.0f*MaxObjExtent;
		max.Z = min.Z + CellDim.Z + 2.0f*MaxObjExtent;
		set_box->Init((min+max)*0.5f, (min-max)*0.5f);
	}
	void compute_box(const VolumeStruct & vol, AABoxClass * set_box)
	{
		Vector3 min,max;
		min.X = Origin.X + vol.Min[0] * CellDim.X - MaxObjExtent;
		min.Y = Origin.Y + vol.Min[1] * CellDim.Y - MaxObjExtent;
		min.Z = Origin.Z + vol.Min[2] * CellDim.Z - MaxObjExtent;
		max.X = Origin.X + vol.Max[0] * CellDim.X + MaxObjExtent;
		max.Y = Origin.Y + vol.Max[1] * CellDim.Y + MaxObjExtent;
		max.Z = Origin.Z + vol.Max[2] * CellDim.Z + MaxObjExtent;
		Vector3 center((max.X+min.X)*0.5f,(max.Y+min.Y)*0.5f,(max.Z+min.Z)*0.5f);
		Vector3 extent((max.X-min.X)*0.5f,(max.Y-min.Y)*0.5f,(max.Z-min.Z)*0.5f);
		set_box->Init(center,extent);
	}
	void init_volume(const Vector3 & bound_min,const Vector3 & bound_max,VolumeStruct * set_vol)
	{
		Vector3 grid_min = bound_min;
		grid_min.X -= MaxObjExtent;
		grid_min.Y -= MaxObjExtent;
		grid_min.Z -= MaxObjExtent;
		Vector3 grid_max = bound_max;
		grid_max.X += MaxObjExtent;
		grid_max.Y += MaxObjExtent;
		grid_max.Z += MaxObjExtent;
		GridCullSystemClass::map_point_to_cell(grid_min,set_vol->Min[0],set_vol->Min[1],set_vol->Min[2]);
		GridCullSystemClass::map_point_to_cell(grid_max,set_vol->Max[0],set_vol->Max[1],set_vol->Max[2]);
		clamp_indices_to_grid(&(set_vol->Min[0]),&(set_vol->Min[1]),&(set_vol->Min[2]));
		clamp_indices_to_grid(&(set_vol->Max[0]),&(set_vol->Max[1]),&(set_vol->Max[2]));
		set_vol->Max[0]++;
		set_vol->Max[1]++;
		set_vol->Max[2]++;
	}
	void init_volume(const Vector3 & point,VolumeStruct * set_volume);
	void init_volume(const LineSegClass & line,VolumeStruct * set_volume)
	{
		Vector3 min_pt,max_pt;
		min_pt.X = WWMath::Min(line.Get_P0().X,line.Get_P1().X);
		max_pt.X = WWMath::Max(line.Get_P0().X,line.Get_P1().X);
		min_pt.Y = WWMath::Min(line.Get_P0().Y,line.Get_P1().Y);
		max_pt.Y = WWMath::Max(line.Get_P0().Y,line.Get_P1().Y);
		min_pt.Z = WWMath::Min(line.Get_P0().Z,line.Get_P1().Z);
		max_pt.Z = WWMath::Max(line.Get_P0().Z,line.Get_P1().Z);
		init_volume(min_pt,max_pt,set_volume);
	}
	void init_volume(const AABoxClass & box,VolumeStruct * set_volume)
	{
		init_volume(box.Center - box.Extent,box.Center + box.Extent,set_volume);
	}
	void init_volume(const OBBoxClass & box,VolumeStruct * set_volume)
	{
		Vector3 aaextent;
		box.Compute_Axis_Aligned_Extent(&aaextent);
		init_volume(box.Center - aaextent,box.Center + aaextent,set_volume);
	}
	void init_volume(const FrustumClass & frustum,VolumeStruct * set_volume)
	{
		init_volume(frustum.Get_Bound_Min(),frustum.Get_Bound_Max(),set_volume);
	}
	void collect_objects_in_leaf(const Vector3 & point,CullableClass * head);
	void collect_objects_in_leaf(const AABoxClass & aabox,CullableClass * head);
	void collect_objects_in_leaf(const OBBoxClass & obbox,CullableClass * head);
	void collect_objects_in_leaf(const FrustumClass & frustum,CullableClass * head);
};
#define GRIDCULL_NODE_ACCEPTED						
#define GRIDCULL_NODE_TRIVIALLY_ACCEPTED		
#define GRIDCULL_NODE_REJECTED						
template <class T> class TypedGridCullSystemClass : public GridCullSystemClass
{
public:
	virtual void		Add_Object(T * obj)						{ Add_Object_Internal(obj); }
	virtual void		Remove_Object(T * obj)					{ Remove_Object_Internal(obj); }
	T *					Get_First_Collected_Object(void)		{ return (T*)Get_First_Collected_Object_Internal(); }
	T *					Get_Next_Collected_Object(T * obj)	{ return (T*)Get_Next_Collected_Object_Internal(obj); }
};
class GridLinkClass : public CullLinkClass, public AutoPoolClass<GridLinkClass,256>
{
public:
	GridLinkClass(GridCullSystemClass * system);
	virtual ~GridLinkClass(void);
	int									GridAddress;		// address in the grid.
	CullableClass *					Prev;					// prev object in this cell
	CullableClass *					Next;					// next object in this cell
};
class GridListIterator
{
public:
	GridListIterator(CullableClass * head)		{ First(head); }
	void			First(CullableClass * head)	{ Head = head; CurObj = head; }
	void			First(void)							{ CurObj = Head; }
	void			Next(void)							{ if (CurObj) { CurObj = ((GridLinkClass *)CurObj->Get_Cull_Link())->Next; } }
	void			Prev(void)							{ if (CurObj) { CurObj = ((GridLinkClass *)CurObj->Get_Cull_Link())->Prev; } }
	bool			Is_Done(void)						{ return (CurObj == NULL); }
	CullableClass *	Get_Obj(void)				{ if (CurObj) { CurObj->Add_Ref(); } return CurObj; }
	CullableClass *	Peek_Obj(void)				{ return CurObj; }
private:
	CullableClass *	Head;				// head of the list we're working in
	CullableClass *	CurObj;			// node we're currently at.
};

#endif