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
#include "engine_script.h"
#include "engine_vector.h"
#include "ScriptableGameObj.h"
#include "slist.h"
#include "GameObjManager.h"
#include "engine_obj.h"
#include "engine_obj2.h"
#include "engine_def.h"
#include "Definition.h"
#include "engine_phys.h"
#include "engine_game.h"
#include "BuildingGameObj.h"
#include "VehicleGameObj.h"
SCRIPTS_API void Remove_Script(GameObject *obj,const char *script)
{
	if (!obj)
	{
		return;
	}
	const SimpleDynVecClass<GameObjObserverClass *> *observers = &obj->Get_Observers();
	int x = observers->Count();
	for (int i = 0;i < x;i++)
	{
		if (!_stricmp((*observers)[i]->Get_Name(),script))
		{
			((ScriptImpClass*)(*observers)[i])->Destroy_Script();
		}
	}
}

SCRIPTS_API void Remove_Duplicate_Script(GameObject *obj, const char *script)
{
	if (!obj)
	{
		return;
	}
	const SimpleDynVecClass<GameObjObserverClass *> *observers = &obj->Get_Observers();
	bool Found = false;
	for (int i = 0;i < observers->Count();i++)
	{
		if (!_stricmp((*observers)[i]->Get_Name(),script))
		{
			if (!Found)
			{
				Found = true;
			}
			else
			{
				((ScriptImpClass*)(*observers)[i])->Destroy_Script();
			}
		}
	}
}

SCRIPTS_API void Remove_Script_Preset(const char *script,const char *preset,int team)
{
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			const DefinitionClass *d = &o2->Get_Definition();
			if (!_stricmp(d->Get_Name(),preset))
			{
				if ((Get_Object_Type(o2) == team) || (team == 2))
				{
					Remove_Script(o2,script);
				}
			}
		}
		x = x->Next();
	}
}

SCRIPTS_API void Remove_Script_Type(const char *script,unsigned long type,int team)
{
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			const DefinitionClass *d = &o2->Get_Definition();
			if (d->Get_Class_ID() == type)
			{
				if ((Get_Object_Type(o2) == team) || (team == 2))
				{
					Remove_Script(o2,script);
				}
			}
		}
		x = x->Next();
	}
}

SCRIPTS_API void Remove_All_Scripts(GameObject *obj)
{
	if (!obj)
	{
		return;
	}
	const SimpleDynVecClass<GameObjObserverClass *> *observers = &obj->Get_Observers();
	int x = observers->Count();
	for (int i = 0;i < x;i++)
	{
		if ((*observers)[i]->Owner() != 0)
		{
			((ScriptImpClass*)(*observers)[i])->Destroy_Script();
		}
	}
}

SCRIPTS_API void Attach_Script_Preset(const char *script,const char *params,const char *preset,int team)
{
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			const DefinitionClass *d = &o2->Get_Definition();
			if (!_stricmp(d->Get_Name(),preset))
			{
				if ((Get_Object_Type(o2) == team) || (team == 2))
				{
					Commands->Attach_Script(o2,script,params);
				}
			}
		}
		x = x->Next();
	}
}

SCRIPTS_API void Attach_Script_Type(const char *script,const char *params,unsigned long type,int team)
{
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			const DefinitionClass *d = &o2->Get_Definition();
			if (d->Get_Class_ID() == type)
			{
				if ((Get_Object_Type(o2) == team) || (team == 2))
				{
					Commands->Attach_Script(o2,script,params);
				}
			}
		}
		x = x->Next();
	}
}

SCRIPTS_API bool Is_Script_Attached(GameObject *obj,const char *script)
{
	if (!obj)
	{
		return false;
	}
	return obj->Find_Observer(script) != 0;
}

SCRIPTS_API void Attach_Script_Once(GameObject *obj,const char *script,const char *params)
{
	if (!obj)
	{
		return;
	}
	if (!Is_Script_Attached(obj,script))
	{
		Commands->Attach_Script(obj,script,params);
	}
}

SCRIPTS_API void Attach_Script_Preset_Once(const char *script,const char *params,const char *preset,int team)
{
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			const DefinitionClass *d = &o2->Get_Definition();
			if (!_stricmp(d->Get_Name(),preset))
			{
				if ((Get_Object_Type(o2) == team) || (team == 2))
				{
					if (!Is_Script_Attached(o2,script))
					{
						Commands->Attach_Script(o2,script,params);
					}
				}
			}
		}
		x = x->Next();
	}
}

SCRIPTS_API void Attach_Script_Type_Once(const char *script,const char *params,unsigned long type,int team)
{
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			const DefinitionClass *d = &o2->Get_Definition();
			if (d->Get_Class_ID() == type)
			{
				if ((Get_Object_Type(o2) == team) || (team == 2))
				{
					if (!Is_Script_Attached(o2,script))
					{
						Commands->Attach_Script(o2,script,params);
					}
				}
			}
		}
		x = x->Next();
	}
}

SCRIPTS_API void Attach_Script_Player_Once(const char *script,const char *params,int team)
{
	SLNode<SoldierGameObj> *x = GameObjManager::StarGameObjList.Head();
	while (x)
	{
		SoldierGameObj *o = x->Data();
		if ((Get_Object_Type(o) == team) || (team == 2))
		{
			if (!Is_Script_Attached(o,script))
			{
				Commands->Attach_Script(o,script,params);
			}
		}
		x = x->Next();
	}
}

SCRIPTS_API void Attach_Script_Building(const char *script,const char *params,int team)
{
	SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			if ((Get_Object_Type(o) == team) || (team == 2))
			{
				Commands->Attach_Script(o,script,params);
			}
		}
		x = x->Next();
	}
}

SCRIPTS_API void Attach_Script_Is_Preset(GameObject *obj,const char *preset,const char *script,const char *params,int team)
{
	if (!obj)
	{
		return;
	}
	const DefinitionClass *d = &obj->Get_Definition();
	if (!_stricmp(d->Get_Name(),preset))
	{
		if ((Get_Object_Type(obj) == team) || (team == 2))
		{
			Commands->Attach_Script(obj,script,params);
		}
	}
}

SCRIPTS_API void Attach_Script_Is_Type(GameObject *obj,unsigned long type,const char *script,const char *params,int team)
{
	if (!obj)
	{
		return;
	}
	const DefinitionClass *d = &obj->Get_Definition();
	if (d->Get_Class_ID() == type)
	{
		if ((Get_Object_Type(obj) == team) || (team == 2))
		{
			Commands->Attach_Script(obj,script,params);
		}
	}
}

SCRIPTS_API void Attach_Script_All_Buildings_Team(int Team,const char *Script,const char *Params,bool Once)
{
	SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			if (Get_Object_Type(o) == Team || Team == 2)
			{
				if (Once)
				{
					Attach_Script_Once(o,Script,Params);
				}
				else
				{
					Commands->Attach_Script(o,Script,Params);
				}
			}
		}
		x = x->Next();
	}
}

SCRIPTS_API void Attach_Script_All_Turrets_Team(int Team,const char *Script,const char *Params,bool Once)
{
	SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();
	while (x)
	{
		VehicleGameObj *o = x->Data();
		if (Get_Vehicle_Mode(o) == VEHICLE_TYPE_TURRET && (Get_Object_Type(o) == Team || Team == 2))
		{
			if (Once)
			{
				Attach_Script_Once(o,Script,Params);
			}
			else
			{
				Commands->Attach_Script(o,Script,Params);
			}
		}
		x = x->Next();
	}
}

SCRIPTS_API GameObject *Find_Building_With_Script(int Team,int Type,const char *Script,GameObject *Caller)
{
	SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			if ((Type == -1 || Building_Type(o) == Type) && (Get_Object_Type(o) == Team || Team == 2) && !Is_Building_Dead(o) && Is_Script_Attached(o,Script) && o != Caller)
			{
				return o;
			}
		}
		x = x->Next();
	}
	return 0;
}

SCRIPTS_API GameObject *Find_Object_With_Script(const char *script)
{
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if (Is_Script_Attached(o2, script))
			{
				return o2;
			}
		}
		x = x->Next();
	}
	return NULL;
}

SCRIPTS_API GameObject *Find_Closest_Object_With_Script(const char *script, Vector3 pos)
{
	float closestdist = FLT_MAX;
	GameObject *closest = NULL;
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if (Is_Script_Attached(o2, script))
			{
				float dist = Vector3::Distance_Squared(Commands->Get_Position(o2), pos);
				if (dist < closestdist)
				{
					closestdist = dist;
					closest = o2;
				}
			}
		}
		x = x->Next();
	}
	return closest;
}

SCRIPTS_API void Find_All_Objects_With_Script(const char *script, SList<GameObject>& objects)
{
	objects.Remove_All();

	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if (Is_Script_Attached(o2, script))
			{
				objects.Add_Tail(o2);
			}
		}
		x = x->Next();
	}
}

SCRIPTS_API void Find_All_Objects_With_Script_By_Distance(const char *script, SList<GameObject>& objects, Vector3 position)
{
  objects.Remove_All();

  // Internal list of distances, in the same order as GameObjects in objects
  SList<float> distances;

  for ( SLNode<BaseGameObj>* objNode = GameObjManager::GameObjList.Head(); objNode != NULL; objNode = objNode->Next() )
  {
    ScriptableGameObj* obj = (objNode->Data()) ? objNode->Data()->As_ScriptableGameObj() : NULL;
    if ( obj && Is_Script_Attached(obj,script) )
    {
      // SList cannot contain non-pointer types... stupid thing!
      float* distance = new float;
      *distance = Commands->Get_Distance(Commands->Get_Position(obj), position);

      SLNode<float>* d = distances.Head();
      for ( SLNode<GameObject>* o = objects.Head();
        d != NULL;
        d = d->Next(), o = o->Next() )
      {
        if ( *distance < *d->Data() )
        {
          objects.insertBefore(obj, *o);
          distances.insertBefore(distance, *d);
          distance = NULL;
          break;
        }
      }

      // OK, all existing objects are closer than this one so add it to the end of the list
      if ( distance != NULL )
      {
        objects.Add_Tail(obj);
        distances.Add_Tail(distance);
      }
    }
  }

  // Clean up memory since SList insists on having heap objects...
  for ( SLNode<float>* d = distances.Head(); d != NULL; d = d->Next() )
    delete d->Data();
}

SCRIPTS_API void Find_All_Vehicles_By_Distance(SList<GameObject>& objects, Vector3 position)
{
  objects.Remove_All();

  // Internal list of distances, in the same order as GameObjects in objects
  SList<float> distances;

  for ( SLNode<BaseGameObj>* objNode = GameObjManager::GameObjList.Head(); objNode != NULL; objNode = objNode->Next() )
  {
    ScriptableGameObj* obj = (objNode->Data()) ? objNode->Data()->As_ScriptableGameObj() : NULL;
	if ( obj && obj->As_VehicleGameObj() )
    {
      // SList cannot contain non-pointer types... stupid thing!
      float* distance = new float;
      *distance = Commands->Get_Distance(Commands->Get_Position(obj), position);

      SLNode<float>* d = distances.Head();
      for ( SLNode<GameObject>* o = objects.Head();
        d != NULL;
        d = d->Next(), o = o->Next() )
      {
        if ( *distance < *d->Data() )
        {
          objects.insertBefore(obj, *o);
          distances.insertBefore(distance, *d);
          distance = NULL;
          break;
        }
      }

      // OK, all existing objects are closer than this one so add it to the end of the list
      if ( distance != NULL )
      {
        objects.Add_Tail(obj);
        distances.Add_Tail(distance);
      }
    }
  }

  // Clean up memory since SList insists on having heap objects...
  for ( SLNode<float>* d = distances.Head(); d != NULL; d = d->Next() )
    delete d->Data();
}

SCRIPTS_API void Send_Custom_Event_To_Objects_With_Script( GameObject *sender, const char *script, int message, int param, float delay )
{
	if (!sender)
	{
		return;
	}
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if ( Is_Script_Attached( o2, script )
				&& Commands->Get_ID ( sender ) != Commands->Get_ID ( o2 ) )
			{
				Commands->Send_Custom_Event(sender,o2,message,param,delay);
			}
		}
		x = x->Next();
	}
}

SCRIPTS_API void Send_Custom_Event_To_Objects_With_Script_Ranged( GameObject *sender, const char *script, int message, int param, float delay, float range )
{
	if (!sender)
	{
		return;
	}
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if ( Is_Script_Attached( o2, script )
				&& Commands->Get_ID ( sender ) != Commands->Get_ID ( o2 )
				&& Vector3::Distance_Squared ( Commands->Get_Position ( sender ), Commands->Get_Position ( o2 ) ) <= range * range )
			{
				Commands->Send_Custom_Event(sender,o2,message,param,delay);
			}
		}
		x = x->Next();
	}
}

SCRIPTS_API ScriptImpClass* Find_Script_On_Object(GameObject* obj, const char *script)
{
	if (!obj)
	{
		return NULL;
	}
	return (ScriptImpClass *)obj->Find_Observer(script);
}

void SCRIPTS_API Attach_Script_Occupants(GameObject *obj,const char *script,const char *params)
{
	if (!obj || !obj->As_VehicleGameObj())
	{
		return;
	}
	VehicleGameObj *o = obj->As_VehicleGameObj();
	int x = o->Get_Definition().Get_Seat_Count();
	for (int i = 0;i < x;i++)
	{
		if (o->Get_Occupant(i))
		{
			Commands->Attach_Script(o->Get_Occupant(i),script,params);
		}
	}
}


void Attach_Script_V ( GameObject* pObj, const char* script, const char* params, va_list vargs )
{
  if ( !pObj )
    return;

  int length = _vscprintf_p(params,vargs);
  char* formattedParams = new char[length+1];
  _vsnprintf_s(formattedParams, length+1, length, params, vargs);

  Commands->Attach_Script(pObj, script, formattedParams);

  delete[] formattedParams;
}

void SCRIPTS_API Attach_Script_V ( GameObject* pObj, const char* script, const char* params, ... )
{
  va_list vargs;
  va_start ( vargs, params );
  Attach_Script_V(pObj,script,params,vargs);
  va_end(vargs);
}

void SCRIPTS_API Attach_Script_Once_V ( GameObject* pObj, const char* script, const char* params, ... )
{
  if ( !pObj || Is_Script_Attached(pObj,script))
    return;

  va_list vargs;
  va_start ( vargs, params );
  Attach_Script_V(pObj,script,params,vargs);
  va_end(vargs);
}

int Get_Param_Type(const char *str)
{
	for (int i = 0; i < PARAM_TYPE_COUNT; i++)
	{
		if (!lstrcmpiA(str, PARAM_TYPE_STRINGS[i]))
		{
			return i;
		}
	}
	return 0;
}

SCRIPTS_API void Get_Script_Parameters(const char *script, DynamicVectorClass<ScriptParameter> &parameters)
{
	parameters.Delete_All();
	auto factory = ScriptRegistrar::GetScriptFactory(script);
	if (!factory)
	{
		return;
	}
	char* working = newstr(factory->GetParamDescription());
	char *param = strtok(working, ",");
	while (param)
	{
		char *sname = param;
		char *type = strchr(param, ':');
		type[0] = '\0';
		type++;
		char *value = strrchr(param, '=');
		if (value)
		{
			value[0] = '\0';
			value++;
		}
		ScriptParameter p;
		p.name = sname;
		p.value = value;
		p.type = Get_Param_Type(type);
		parameters.Add(p);
		param = strtok(nullptr, ",");
	}
	delete[] working;
}
