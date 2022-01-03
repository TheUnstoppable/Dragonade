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
#ifndef TT_INCLUDE_CONVERSATIONCLASS_H
#define TT_INCLUDE_CONVERSATIONCLASS_H
#include "engine_vector.h"
#include "ReferencerClass.h"
#include "PhysicalGameObj.h"
#include "Vector3.h"
class ConversationRemarkClass {
public:
	ConversationRemarkClass() : OratorID(0), TextID(0)
	{
	}
	virtual ~ConversationRemarkClass()
	{
	}
	ConversationRemarkClass(const ConversationRemarkClass & that) : OratorID(0), TextID(0)
	{
		*this = that;
	}
	ConversationRemarkClass const &operator = (ConversationRemarkClass const & that)
	{
		OratorID = that.OratorID;
		TextID = that.TextID;
		AnimationName = that.AnimationName;
		return *this;
	}
	int Get_Orator_ID() const
	{
		return OratorID;
	}
	void Set_Orator_ID(int id)
	{
		OratorID = id;
	}
	uint32 Get_Text_ID() const
	{
		return TextID;
	}
	void Set_Text_ID(uint32 id)
	{
		TextID = id;
	}
	const char *Get_Animation_Name() const
	{
		return AnimationName;
	}
	void Set_Animation_Name(const char *name)
	{
		AnimationName = name;
	}
	bool operator ==(ConversationRemarkClass const & that)
	{
		if ((TextID == that.TextID) && (OratorID == that.OratorID))
		{
			return true;
		}
		return false;
	}
	bool operator !=(ConversationRemarkClass const & that)
	{
		if ((TextID != that.TextID) || (OratorID != that.OratorID))
		{
			return true;
		}
		return false;
	}
	bool Save(ChunkSaveClass &cload);
	bool Load(ChunkLoadClass &cload);
	void Load_Variables(ChunkLoadClass &cload);
private:
	int OratorID;
	uint32 TextID;
	StringClass AnimationName;
};
class OratorClass {
private:
	OratorClass(const OratorClass& that); // Disallow copying
	void *unk; //0
	ReferencerClass Owner; //4
	Vector3 Position; //18
	bool Arrived; //20
	int Flags; //24
	int OratorID;
	int unk2C;
	int FacingID;
	bool unk34;
public:
	OratorClass& operator=(const OratorClass& that)
	{
		unk = that.unk;
		Owner = that.Owner;
		Position = that.Position;
		Arrived = that.Arrived;
		Flags = that.Flags;
		OratorID = that.OratorID;
		unk2C = that.unk2C;
		FacingID = that.FacingID;
		unk34 = that.unk34;
		return *this;
	}
	OratorClass() : unk(0), Position(0, 0, 0), Arrived(false), Flags(0), OratorID(-1), unk2C(0), FacingID(0), unk34(false)
	{
	}
	~OratorClass()
	{
	}
	bool operator ==(const OratorClass &that)
	{
		return OratorID == that.OratorID;
	}
	bool operator !=(const OratorClass &that)
	{
		return OratorID != that.OratorID;
	}
	bool Get_Flag(int flag)
	{
		return (Flags & flag) == flag;
	}
	PhysicalGameObj *Get_Game_Obj()
	{
		if (Owner)
		{
			return Owner->As_PhysicalGameObj();
		}
		return 0;
	}
	void Set_Flag(int flag, bool onoff)
	{
		int t = Flags & ~flag;
		Flags &= ~flag;
		if (onoff)
		{
			Flags = flag | t;
		}
		if (Flags & FLAG_DONT_MOVE)
		{
			Arrived = true;
		}
	}
	Vector3 &Get_Position()
	{
		return Position;
	}
	void Set_Flags(int newflags)
	{
		Flags = newflags;
		if (Flags & FLAG_DONT_MOVE)
		{
			Arrived = true;
		}
	}
	void Set_Arrived(bool arrived)
	{
		Arrived = arrived;
	}
	void Initialize(PhysicalGameObj *obj)
	{
		Owner = obj;
	}
	void Set_Orator_ID(int id)
	{
		OratorID = id;
	}
	enum {
		FLAG_DONT_MOVE = 1,
		FLAG_DONT_TURN_HEAD = 2,
		FLAG_TEMP_DONT_FACE = 4,
		FLAG_DONT_FACE = 8,
	};
	void Set_Facing_ID(int id)
	{
		FacingID = id;
	}
	bool Save(ChunkSaveClass &cload);
	bool Load(ChunkLoadClass &cload);
	void Load_Variables(ChunkLoadClass &cload);
};
class ConversationClass : public RefCountClass {
private:
	ConversationClass(const ConversationClass& that);
	ConversationClass& operator=(const ConversationClass& that);
	StringClass Name;
	int ID;
	DynamicVectorClass<ConversationRemarkClass> Remarks;
	DynamicVectorClass<OratorClass> Orators;
	SoldierAIState AIState;
	bool IsInnate;
	bool IsKey;
	float Probability;
	int CategoryID;
	int LookAtObjID;
	int Priority;
	float MaxDist;
	bool IsInterruptable;
public:
	ConversationClass();
	void Clear_Orators();
	void Clear_Remarks();
	~ConversationClass();
	int Get_Orator_Count() const
	{
		return Orators.Count();
	}
	void Add_Orator(OratorClass const&);
	void Add_Remark(ConversationRemarkClass const&);
	int Get_Remark_Count() const
	{
		return Remarks.Count();
	}
	bool Get_Remark_Info(int index,ConversationRemarkClass &remark)
	{
		if (index >= 0 && index < Remarks.Count())
		{
			remark = Remarks[index];
			return true;
		}
		return false;
	}
	bool Save(ChunkSaveClass &cload);
	bool Load(ChunkLoadClass &cload);
	void Load_Variables(ChunkLoadClass &cload);
	OratorClass *Get_Orator(int);
	OratorClass *Find_Orator(int);
	int Get_ID() const { return ID; }
	void Set_ID(int id) { ID = id; }
	const char *Get_Name() const { return Name; }
	void Set_Name(const char *name) { Name = name; }
	SoldierAIState Get_AI_State() const { return AIState; }
	void Set_AI_State(SoldierAIState state) { AIState = state; }
	bool Is_Innate() const { return IsInnate; }
	void Set_Is_Innate(bool b) { IsInnate = b; }
	bool Is_Key() const { return IsKey; }
	void Set_Is_Key(bool b) { IsKey = b; }
	float Get_Probability() const { return Probability; }
	void Set_Probability(float f) { Probability = f; }
	int Get_Category_ID() const { return CategoryID; }
	void Set_Category_ID(int id) { CategoryID = id; }
	int Get_Look_At_Obj_ID() const { return LookAtObjID; }
	void Set_Look_At_Obj_ID(int id) { LookAtObjID = id; }
	int Get_Priority() const { return Priority; }
	void Set_Priority(int i) { Priority = i; }
	float Get_Max_Dist() const { return MaxDist; }
	void Set_Max_Dist(float f) { MaxDist = f; }
	bool Is_Interruptable() const { return IsInterruptable; }
	void Set_Is_Interruptable(bool b) { IsInterruptable = b; }
};

#endif