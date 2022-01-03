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
#include "general.h"

#include "scripts.h"
#include "PhysClass.h"
#include "MoveablePhysClass.h"
#include "RenderObjClass.h"
#include "HTreeClass.h"
#include "engine_obj.h"
#include "PhysicalGameObj.h"
#include "engine_def.h"
#include "slist.h"
#include "GameObjManager.h"
#include "engine_obj2.h"
#include "SmartGameObj.h"
#include "VehicleGameObj.h"
#include "AnimControlClass.h"
#include "boxrenderobjclass.h"
#include "PhysicsSceneClass.h"
const char SCRIPTS_API *Get_Model(GameObject *obj)
{
	if (!obj)
	{
		return "DUMMY";
	}
	PhysicalGameObj *o = obj->As_PhysicalGameObj();
	if (!o)
	{
		return "DUMMY";
	}
	return o->Peek_Physical_Object()->Peek_Model()->Get_Name();
}

//doesnt work for CinematicGameObjs
float SCRIPTS_API Get_Animation_Frame(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	PhysicalGameObj *o = obj->As_PhysicalGameObj();
	if (!o)
	{
		return 0;
	}
	if (o->As_CinematicGameObj())
	{
		return 0;
	}
	AnimControlClass *c = o->Get_Anim_Control();
	if (!c)
	{
		return 0;
	}
	float a = c->Get_Current_Frame();
	return a;
}

//doesnt work for CinematicGameObjs
float SCRIPTS_API Get_Animation_Target_Frame(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	PhysicalGameObj *o = obj->As_PhysicalGameObj();
	if (!o)
	{
		return 0;
	}
	if (o->As_CinematicGameObj())
	{
		return 0;
	}
	AnimControlClass *c = o->Get_Anim_Control();
	if (!c)
	{
		return 0;
	}
	float a = c->Get_Target_Frame();
	return a;
}

void SCRIPTS_API Copy_Transform(GameObject *in,GameObject *out)
{
	if (!in || !out)
	{
		return;
	}
	PhysicalGameObj *i2 = in->As_PhysicalGameObj();
	if (!i2)
	{
		return;
	}
	PhysicalGameObj *o2 = out->As_PhysicalGameObj();
	if (!o2)
	{
		return;
	}
	o2->Set_Transform(i2->Get_Transform());
}

float SCRIPTS_API Get_Mass(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	PhysicalGameObj *o = obj->As_PhysicalGameObj();
	if (!o)
	{
		return 0;
	}
	PhysClass *phys = o->Peek_Physical_Object();
	if (phys && phys->As_MoveablePhysClass())
	{
		return phys->As_MoveablePhysClass()->Get_Mass();
	}
	return 0;
}

const char SCRIPTS_API *Get_Htree_Name(GameObject *obj)
{
	if (!obj)
	{
		return "DUMMY";
	}
	PhysicalGameObj *o = obj->As_PhysicalGameObj();
	if (!o)
	{
		return "DUMMY";
	}
	return o->Peek_Physical_Object()->Peek_Model()->Get_HTree()->Get_Name();
}

char SCRIPTS_API Get_Sex(GameObject *obj)
{
	return Get_Htree_Name(obj)[2];
}

void SCRIPTS_API Create_Effect_All_Stealthed_Objects_Area(const Vector3 &Position,float Distance,const char *object,const Vector3 &offset,int team)
{
	Vector3 TestPosition = Position;
	TestPosition.Z = 0;
	SLNode<SmartGameObj> *x = GameObjManager::SmartGameObjList.Head();
	while (x) 
	{
		SmartGameObj *obj = x->Data();
		Vector3 ObjPosition = Commands->Get_Position(obj);
		ObjPosition.Z = 0;
		if ((Vector3::Distance_Squared(ObjPosition,TestPosition) <= Distance * Distance))
		{
			if ((Get_Object_Type(obj) == team) || (team == 2))
			{
				bool stealth = Is_Stealth(obj);
				bool underground = obj->As_VehicleGameObj() && obj->As_VehicleGameObj()->Is_Underground();
				if (stealth || underground)
				{
					ObjPosition = Commands->Get_Position(obj);
					ObjPosition.X += offset.X;
					ObjPosition.Y += offset.Y;
					ObjPosition.Z += offset.Z;
					Commands->Create_Object(object,ObjPosition);
				}
			}
		}
		x = x->Next();
	}
}

void SCRIPTS_API Create_Effect_All_Of_Preset(const char *object,const char *preset,float ZAdjust,bool ZSet)
{
	SLNode<SmartGameObj> *x = GameObjManager::SmartGameObjList.Head();
	while (x) 
	{
		GameObject *obj = x->Data();
		if (obj)
		{
			if (!_stricmp(preset,Commands->Get_Preset_Name(obj)))
			{
				Vector3 ObjPosition = Commands->Get_Position(obj);
				if (ZSet)
				{
					ObjPosition.Z = ZAdjust;
				}
				else
				{
					ObjPosition.Z += ZAdjust;
				}
				GameObject *o = Commands->Create_Object(object,ObjPosition);
				Commands->Set_Facing(o,Commands->Get_Facing(obj));
			}
		}
		x = x->Next();
	}
}


SCRIPTS_API const char* Get_Animation_Name ( GameObject* pObj )
{
  if (!pObj || !pObj->As_PhysicalGameObj() || pObj->As_PhysicalGameObj()->As_CinematicGameObj())
    return NULL;

  AnimControlClass* pAcc = pObj->As_PhysicalGameObj()->Get_Anim_Control();
  if ( pAcc )
    return pAcc->Get_Animation_Name();
  else
    return NULL;
}

void SCRIPTS_API Wake_Up_Objects_In_OBBox(OBBoxClass &BoundingBox)
{
	MultiListClass<PhysClass> list;
	PhysicsSceneClass::Get_Instance()->Collect_Objects(BoundingBox, false, true, &list);
    for (auto it = list.Iterator(); !it.Is_Done(); it.Next())
    {
		if (it->As_MoveablePhysClass())
		{
			Matrix3D m = it->Get_Transform();
			m.Adjust_Z_Translation(0.001f);
			it->Set_Transform(m);
		}
	}
}
void SCRIPTS_API Wake_Up_Objects_In_Box(GameObject *obj,const char *box)
{
	if (!obj || !obj->As_PhysicalGameObj() || !obj->As_PhysicalGameObj()->Peek_Model())
	{
		return;
	}
	RenderObjClass *o = obj->As_PhysicalGameObj()->Peek_Model();
	StringClass str = o->Get_Name();
	str += '.';
	str += box;
	RenderObjClass *boxobj = o->Get_Sub_Object_By_Name(str);
	if (!boxobj || boxobj->Class_ID() != RenderObjClass::CLASSID_OBBOX)
	{
		return;
	}
	OBBoxClass &BoundingBox = ((OBBoxRenderObjClass *)boxobj)->Get_Box();
	Wake_Up_Objects_In_OBBox(BoundingBox);
	boxobj->Release_Ref();
}
