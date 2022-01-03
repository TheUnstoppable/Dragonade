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
#ifndef TT_INCLUDE_AABTREECULL_H
#define TT_INCLUDE_AABTREECULL_H
#include "cullsystemclass.h"
#include "aaplaneclass.h"
#include "AABoxClass.h"
#include "wwmath.h"
#include "engine_vector.h"
#include <math.h>
#include <float.h>

class AABTreeNodeClass;
class ChunkLoadClass;
class ChunkSaveClass;
class SphereClass;
class AABTreeCullSystemClass : public CullSystemClass
{
public:
	AABTreeCullSystemClass(void);
	virtual ~AABTreeCullSystemClass(void);

	void			Re_Partition(void);
	void			Re_Partition(const AABoxClass & bounds,SimpleDynVecClass<AABoxClass> & boxes);
	void			Update_Bounding_Boxes(void);
	virtual void				Update_Culling(CullableClass * obj);
	int							Partition_Node_Count(void) const;
	int							Partition_Tree_Depth(void) const;
	int							Object_Count(void) const;
	virtual void				Collect_Objects(const Vector3 & point);
	virtual void				Collect_Objects(const AABoxClass & box);
	virtual void				Collect_Objects(const OBBoxClass & box);
	virtual void				Collect_Objects(const FrustumClass & frustum);
	virtual void				Collect_Objects(const SphereClass & sphere);
	virtual void	Load(ChunkLoadClass & cload);
	virtual void				Save(ChunkSaveClass & csave);
	void			Load_Object_Linkage(ChunkLoadClass & cload, CullableClass * obj);
	void						Save_Object_Linkage(ChunkSaveClass & csave, CullableClass * obj);
	const AABoxClass &			Get_Bounding_Box(void);
	void						Get_Node_Bounds(int node_id,AABoxClass * set_bounds);

	struct StatsStruct
	{
		int				NodeCount;
		int				NodesAccepted;
		int				NodesTriviallyAccepted;
		int				NodesRejected;
	};

	void					Reset_Statistics(void);
	const StatsStruct & Get_Statistics(void);

protected:

	void	NODE_ACCEPTED(void)					{ }
	void	NODE_TRIVIALLY_ACCEPTED(void)		{ }
	void	NODE_REJECTED(void)					{ }

	void	Add_Object_Internal(CullableClass * obj, int node_index = -1);
	void				Remove_Object_Internal(CullableClass * obj);
	void				Re_Index_Nodes(void);
	void				Re_Index_Nodes_Recursive(AABTreeNodeClass * node, int & counter);
	int					Partition_Node_Count_Recursive(AABTreeNodeClass * node) const;
	void				Partition_Tree_Depth_Recursive(AABTreeNodeClass * node, int cur_depth, int & max_depth) const;
	void				Add_Object_Recursive(AABTreeNodeClass * node, CullableClass * obj);
	void	Add_Loaded_Object(AABTreeNodeClass * node, CullableClass * obj);
	void				Collect_Branch(AABTreeNodeClass * node);
	void				Collect_Objects_Recursive(AABTreeNodeClass * node, const FrustumClass & frustum, int planes_passed);
	void				Update_Bounding_Boxes_Recursive(AABTreeNodeClass * node);
	void				Load_Nodes(AABTreeNodeClass * node, ChunkLoadClass & cload);
	void				Save_Nodes(AABTreeNodeClass * node, ChunkSaveClass & csave);
	virtual void		Load_Node_Contents(AABTreeNodeClass * node, ChunkLoadClass & cload) { }
	virtual void		Save_Node_Contents(AABTreeNodeClass * node, ChunkSaveClass & csave) { }

	AABTreeNodeClass *	RootNode;
	int					ObjectCount;
	int					NodeCount;
	AABTreeNodeClass **	IndexedNodes;
	StatsStruct			Stats;

	friend class AABTreeIterator;
};
class AABTreeIterator
{
public:
	AABTreeIterator(AABTreeCullSystemClass * tree);
	void Reset(void);
	bool Enter_Parent(void);
	bool Enter_Sibling(void);
	bool Has_Front_Child(void);
	bool Enter_Front_Child(void);
	bool Has_Back_Child(void);
	bool Enter_Back_Child(void);
	int Get_Current_Node_Index(void);
	void Get_Current_Box(AABoxClass * set_box);

private:
	void validate(void);

	AABTreeCullSystemClass *	Tree;
	int							CurNodeIndex;
};

template <class T> class TypedAABTreeCullSystemClass : public AABTreeCullSystemClass
{
public: 
	virtual void	Add_Object(T * obj, int node_index=-1)	{ Add_Object_Internal(obj,node_index); }
	virtual void	Remove_Object(T * obj)					{ Remove_Object_Internal(obj); }
	T *				Get_First_Collected_Object(void)		{ return (T*)Get_First_Collected_Object_Internal(); }
	T *				Get_Next_Collected_Object(T * obj)		{ return (T*)Get_Next_Collected_Object_Internal(obj); }
	T *				Peek_First_Collected_Object(void)		{ return (T*)Peek_First_Collected_Object_Internal(); }
	T *				Peek_Next_Collected_Object(T * obj)		{ return (T*)Peek_Next_Collected_Object_Internal(obj); }
};

class AABTreeNodeClass : public AutoPoolClass<AABTreeNodeClass, 256>
{
public:
	AABTreeNodeClass(void);
	~AABTreeNodeClass(void);

	void				Add_Object(CullableClass * obj,bool update_bounds = true);
	void				Remove_Object(CullableClass * obj);
	int					Object_Count(void);
	CullableClass *		Peek_Object(int index);

	uint32				Index;
	AABoxClass			Box;
	AABTreeNodeClass *	Parent;
	AABTreeNodeClass *	Front;
	AABTreeNodeClass *	Back;
	CullableClass *		Object;
	uint32				UserData;

	struct SplitChoiceStruct
	{
		SplitChoiceStruct(void) : Cost(FLT_MAX),FrontCount(0),BackCount(0),Plane(AAPlaneClass::XNORMAL,0.0f) 
		{
			FrontBox.Init_Empty();
			BackBox.Init_Empty();
		}

		float				Cost;
		int					FrontCount;
		int					BackCount;
		MinMaxAABoxClass	FrontBox;
		MinMaxAABoxClass	BackBox;
		AAPlaneClass		Plane;
	};

	void						Compute_Bounding_Box(void);
	void						Compute_Local_Bounding_Box(void);
	float						Compute_Volume(void);
	void						Transfer_Objects(AABTreeNodeClass * dummy_node);
	void						Partition(void);
	void						Split_Objects(const SplitChoiceStruct & sc, AABTreeNodeClass * front, AABTreeNodeClass * back);
	void						Partition(const AABoxClass & bounds, SimpleDynVecClass<AABoxClass> & boxes);
	void						Split_Boxes(const SplitChoiceStruct & sc, SimpleDynVecClass<AABoxClass> & boxes, SimpleDynVecClass<AABoxClass> & frontboxes, SimpleDynVecClass<AABoxClass> & backboxes);
	void						Select_Splitting_Plane(SplitChoiceStruct * sc,SimpleDynVecClass<AABoxClass> & boxes);
	void						Select_Splitting_Plane_Brute_Force(SplitChoiceStruct * sc, SimpleDynVecClass<AABoxClass> & boxes);
	void						Compute_Score(SplitChoiceStruct * sc, SimpleDynVecClass<AABoxClass> & boxes);
};

class AABTreeLinkClass : public CullLinkClass, public AutoPoolClass<AABTreeLinkClass,256>
{
public:
	AABTreeLinkClass(AABTreeCullSystemClass * system) : CullLinkClass(system),Node(NULL), NextObject(NULL) { }
	AABTreeNodeClass *	Node;
	CullableClass *		NextObject;
};
#endif
