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
#ifndef TT_INCLUDE__HARVESTERCLASS_H
#define TT_INCLUDE__HARVESTERCLASS_H

#include "scripts.h"
#include "engine_string.h"
#include "OBBoxClass.h"
#include "RefineryGameObj.h"
#include "VehicleGameObj.h"
class RefineryGameObj;
class VehicleGameObj;
class HarvesterClass : public GameObjObserverClass
{

public:

   enum State
   {
      NoState             = 0,
      StateGoingToHarvest = 1,
      StateHarvesting     = 2,
      StateGoingToUnload  = 3,
      StateUnloading      = 4,
   };


private:

   RefineryGameObj* refinery;            //   8
   Vector3          dockLocation;        //  12
   Vector3          dockEntrance;        //  24
   State            state;               //  36
   OBBoxClass       tiberiumRegion;      //  40
   VehicleGameObj*  harvester;           // 100
   StringClass      harvestAnimation;    // 104
   float            harvestRelocateTime; // 108
   float            harvestTime;         // 112
   bool             harvesting;          // 116


public:

            HarvesterClass();
   virtual ~HarvesterClass();

   virtual const char*       Get_Name           ()
   {
	   return "HarvesterClass";
   }
   virtual void        Attach             (GameObject* object);
   virtual void        Detach             (GameObject* object);
   virtual void        Created            (GameObject* object)
   {
   }
   virtual void        Destroyed          (GameObject* object);
   virtual void        Killed             (GameObject* object, GameObject*)
   {
   }
   virtual void        Damaged            (GameObject* object, GameObject*, float);
   virtual void        Custom             (GameObject* object, int message, int, GameObject*);
   virtual void        Sound_Heard        (GameObject* object, const CombatSound&)
   {
   }
   virtual void        Enemy_Seen         (GameObject* object, GameObject*)
   {
   }
   virtual void        Action_Complete    (GameObject* object, int id, ActionCompleteReason reason);
   virtual void        Timer_Expired      (GameObject* object, int)
   {
   }
   virtual void        Animation_Complete (GameObject* object, const char*)
   {
   }
   virtual void        Poked              (GameObject* object, GameObject*)
   {
   }
   virtual void        Entered            (GameObject* object, GameObject*)
   {
   }
   virtual void        Exited             (GameObject* object, GameObject*)
   {
   }

   void Initialize();
   void            Go_Harvest             ();
   void            Go_Unload_Tiberium     ();
   VehicleGameObj* Get_Vehicle            ()
   {
	   return harvester;
   }
   State Get_State() {return state;}
   void            Harvest_Tiberium       ();
   void            Play_Harvest_Animation (bool play);
   void            Think                  ();
   void            Unload_Tiberium        ();
   void Set_Refinery(RefineryGameObj *obj) {refinery = obj;}
   void Set_Dock_Location(Vector3 &location) {dockLocation = location;}
   void Set_Dock_Entrance(Vector3 &location) {dockEntrance = location;}
   void Set_Tiberium_Region(OBBoxClass &region) {tiberiumRegion = region;}
   void Set_Harvest_Anim(const char *anim) {harvestAnimation = anim;}
   void Stop();
};

#endif
