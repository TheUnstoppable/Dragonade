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
// Include files
#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "dp88_custom_timer_defines.h"
#include "dp88_buildingScripts.h"
#include "ms.h"
#include "definition.h"
#include "BaseControllerClass.h"
#include "WarFactoryGameObj.h"
#include "AirFactoryGameObj.h"
#include "NavalFactoryGameObj.h"
#include "RefineryGameObj.h"
#include "PurchaseSettingsDefClass.h"
#include "PhysicalGameObj.h"
#include "ArmorWarheadManager.h"

// -------------------------------------------------------------------------------------------------

extern REF_DECL(Vector3, NodHouseColor);
extern REF_DECL(Vector3, GDIHouseColor);
extern REF_DECL(Vector3, PublicMessageColor);

// -------------------------------------------------------------------------------------------------

/*------------------------
Building Controller Script
--------------------------*/

void dp88_buildingScripts_controller::Created ( GameObject *obj )
{
  numChildren = 0;
  children = NULL;
  buildingOnline = true;

  /* Get string IDs and for each one determine if it has a valid sound attached to it */
  team0_buildingOfflineStringId = Get_String_ID_By_Desc(Get_Parameter("Team_0_Building_Offline_String"));
  team1_buildingOfflineStringId = Get_String_ID_By_Desc(Get_Parameter("Team_1_Building_Offline_String"));
  team0_buildingOnlineStringId = Get_String_ID_By_Desc(Get_Parameter("Team_0_Building_Online_String"));
  team1_buildingOnlineStringId = Get_String_ID_By_Desc(Get_Parameter("Team_1_Building_Online_String"));
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_controller::Destroyed ( GameObject *obj )
{
  // Clean up memory if it is still allocated
  if ( children != NULL )
  {
    delete [] children;
    children = NULL;
    numChildren = 0;
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_controller::Killed ( GameObject* obj, GameObject* killer )
{
	// Do we have any children?
	if ( children != NULL )
	{
		// Yes? OK, send building destroyed custom to all children
		for ( int i = 0; i < numChildren; i++ )
		{
			GameObject* child = Commands->Find_Object ( children[i].objectId );

			if ( child != NULL )
				Commands->Send_Custom_Event ( obj, child, CUSTOM_BUILDINGSCRIPTS_BUILDINGDESTROYED, 0, 0.0f );
		}

		// At this point we no longer need to store information about our children so erase the array
		delete [] children;
		children = NULL;
		numChildren = 0;
	}
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_controller::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Child object registrations
	if ( type == CUSTOM_BUILDINGSCRIPTS_REGISTERCHILD && sender != NULL )
	{
		// Get ID of child
		int childId = Commands->Get_ID ( sender );
		if (findChild(childId) == -1)
		{
			// Does child array exist?
			if ( children != NULL )
			{
				// It does, add this to the end of the list
				ChildData* oldChildren = children;
				children = new ChildData[++numChildren];
	
				// Copy old data across
				for ( int i = 0; i < numChildren-1; i++ )
					children[i] = oldChildren[i];
	
				// Store data for new child
				children[numChildren-1].objectId = childId;
				children[numChildren-1].type = param;
	
				// Cleanup old array
				delete [] oldChildren;
			}
	
			// Ok, no child array, create one
			else
			{
				children = new ChildData[++numChildren];
				children[0].objectId = childId;
				children[0].type = param;
			}
		}
	}




	// Child object de-registrations
	else if ( type == CUSTOM_BUILDINGSCRIPTS_UNREGISTERCHILD && sender != NULL )
	{
		// Get ID of child and find them in the data array
		int childId = Commands->Get_ID ( sender );
		int childIndex = findChild( childId );

		// If we found the child in the array we need to remove it
		if ( childIndex != -1 )
		{
			// If there is only one child then simply erase the array
			if ( numChildren == 1 && childIndex == 0 )
			{
				delete [] children;
				children = NULL;
				numChildren = 0;
			}

			// Otherwise create a new, smaller, array and copy over data for all other children
			else
			{
				ChildData* oldChildren = children;
				children = new ChildData[--numChildren];

				// Copy data across
				for ( int i = 0, j = 0; i < numChildren; i++, j++ )
				{
					// If this is the index to skip decrement j so that subsequent items
					// are moved one position forwards and ignore this item
					if ( i == childIndex )
						j--;
					else
						children[j] = oldChildren[i];
				}
			}
		}
	}




	// Child object destruction report
	else if ( type == CUSTOM_BUILDINGSCRIPTS_CHILDDESTROYED && sender != NULL )
	{
		// Find the child in the array
		int childId = Commands->Get_ID ( sender );
		int childIndex = findChild( childId );

		// If this is a minor weakpoint check to see if all other minor weakpoints (if any) are
		// destroyed. If they are then the building is now offline (if it was not before)
		if ( children[childIndex].type == BUILDING_CHILD_TYPE_MINOR_WEAKPOINT )
		{
			children[childIndex].destroyed = true;

			// If the building is not already offline (one weakpoint may have been repaired and
			// destroyed again before building came online) and all weakpoints are destroyed then
			// we set the building to offline.
			if ( buildingOnline )
			{
				bool allWeakpointsDestroyed = true;
				for ( int i = 0; i < numChildren; i++ )
				{
					// If this is a live minor weakpoint then not all weakpoints are destroyed, set flag
					// to false and break (only need to find one to disprove hypothesis)
					if ( children[i].type == BUILDING_CHILD_TYPE_MINOR_WEAKPOINT && children[i].destroyed != true )
					{
						allWeakpointsDestroyed = false;
						break;
					}
				}

				// If all weakpoints are destroyed then the building is now offline. Send the building offline
				// custom to the child who caused us to go offline with param 1 (to apply the effect) and also
				// to other children to notify them of the change of state with param 0 (do not apply effects)
				if ( allWeakpointsDestroyed )
				{
					buildingOnline = false;
					
					// Notify ourselves to trigger building functions to take action on it
					Commands->Send_Custom_Event ( obj, obj, CUSTOM_BUILDINGSCRIPTS_BUILDINGOFFLINE, param, 0.0f );

					// Notify sender to perform their weakpoint action (this will eventually be subsumed by the
					// building effects mechanism)
					Commands->Send_Custom_Event ( obj, sender, CUSTOM_BUILDINGSCRIPTS_BUILDINGOFFLINE, 1, 0.0f );

					for ( int i = 0; i < numChildren; i++ )
					{
						if ( i != childIndex )
						{
							GameObject* child = Commands->Find_Object ( children[i].objectId );
							if ( child != NULL )
								Commands->Send_Custom_Event ( obj, child, CUSTOM_BUILDINGSCRIPTS_BUILDINGOFFLINE, 0, 0.0f );
						}
					}

          /* Send messages to teams */
          if ( team0_buildingOfflineStringId )
            Send_Translated_Message_Team( team0_buildingOfflineStringId, 0, 255,255,255 );

          if ( team1_buildingOfflineStringId )
            Send_Translated_Message_Team( team1_buildingOfflineStringId, 1, 255,255,255 );
				}
			}
		}
		
		
		
		
		// If this was a major weakpoint then, if all other major weakpoints are also destroyed, the
		// entire building is destroyed
		else if ( children[childIndex].type == BUILDING_CHILD_TYPE_MAJOR_WEAKPOINT )
		{
			children[childIndex].destroyed = true;

			// If all other major weakpoints are destroyed then the building is destroyed
			bool allWeakpointsDestroyed = true;
			for ( int i = 0; i < numChildren; i++ )
			{
				// If this is a live major weakpoint then not all weakpoints are destroyed, set flag
				// to false and break (only need to find one to disprove hypothesis)
				if ( children[i].type == BUILDING_CHILD_TYPE_MAJOR_WEAKPOINT && children[i].destroyed != true )
				{
					allWeakpointsDestroyed = false;
					break;
				}
			}


			// If all major weakpoints are destroyed then kill the building (using the GameObject whose ID
			// matches param, to give credit to the appropriate killer)
			if ( allWeakpointsDestroyed )
			{
				// Inform all children that we are dead
				for ( int i = 0; i < numChildren; i++ )
				{
					GameObject* child = Commands->Find_Object ( children[i].objectId );
					if ( child != NULL )
						Commands->Send_Custom_Event ( obj, child, CUSTOM_BUILDINGSCRIPTS_BUILDINGDESTROYED, 0, 0.0f );
				}

				// And now die...
				GameObject* killer = Commands->Find_Object ( param );
				if ( killer != NULL )
					Commands->Apply_Damage ( obj, 1000.0f, "Death", killer );
				else
					Commands->Apply_Damage ( obj, 1000.0f, "Death", obj );
			}
		}
		
		
		
		
		// If this is any other type of weakpoint then just mark them as destroyed
		else
		{
			children[childIndex].destroyed = true;
		}
	}




	// Child object repaired report
	else if ( type == CUSTOM_BUILDINGSCRIPTS_CHILDREPAIRED && sender != NULL )
	{
		// Find the child in the array
		int childId = Commands->Get_ID ( sender );
		int childIndex = findChild( childId );

		// If this is a minor weakpoint check to see if all other minor weakpoints (if any) are
		// destroyed. If they are then the building is now online (if it was not before)
		if ( children[childIndex].type == BUILDING_CHILD_TYPE_MINOR_WEAKPOINT )
		{
			children[childIndex].destroyed = false;

			// If the building is not already online (one weakpoint may have been destroyed and
			// repaired again without the building going offline) and all weakpoints are repaired
			// then we set the building to online.
			if ( buildingOnline != true )
			{
				bool allWeakpointsRepaired = true;
				for ( int i = 0; i < numChildren; i++ )
				{
					// If this is a dead minor weakpoint then not all weakpoints are repaired, set flag
					// to false and break (only need to find one to disprove hypothesis)
					if ( children[i].type == BUILDING_CHILD_TYPE_MINOR_WEAKPOINT && children[i].destroyed == true )
					{
						allWeakpointsRepaired = false;
						break;
					}
				}

				// If all weakpoints are repaired then the building is now online. Send the building online
				// custom to the child who caused us to go online with param 1 (to apply the effect) and also
				// to other children to notify them of the change of state with param 0 (do not apply effects)
				if ( allWeakpointsRepaired )
				{
					buildingOnline = true;

					// Notify ourselves to trigger building functions to take action on it
					Commands->Send_Custom_Event ( obj, obj, CUSTOM_BUILDINGSCRIPTS_BUILDINGONLINE, param, 0.0f );

					// Notify sender to perform their weakpoint action (this will eventually be subsumed by the
					// building effects mechanism)
					Commands->Send_Custom_Event ( obj, sender, CUSTOM_BUILDINGSCRIPTS_BUILDINGONLINE, 1, 0.0f );

					for ( int i = 0; i < numChildren; i++ )
					{
						if ( i != childIndex )
						{
							GameObject* child = Commands->Find_Object ( children[i].objectId );
							if ( child != NULL )
								Commands->Send_Custom_Event ( obj, child, CUSTOM_BUILDINGSCRIPTS_BUILDINGONLINE, 0, 0.0f );
						}
					}

          /* Send messages to teams */
          if ( team0_buildingOnlineStringId )
            Send_Translated_Message_Team( team0_buildingOnlineStringId, 0, 255,255,255 );

          if ( team1_buildingOnlineStringId )
            Send_Translated_Message_Team( team1_buildingOnlineStringId, 1, 255,255,255 );
				}
			}
		}
		

		// If this is any other type of weakpoint then just mark them as not destroyed
		else
		{
			children[childIndex].destroyed = false;
		}
	}




	// Building captured notification sent by a capture point
	else if ( type == CUSTOM_BUILDINGSCRIPTS_BUILDINGCAPTURED && sender != NULL )
	{
		// Check the sender is one of our children (or ourself, for compatibility with dp88_Set_Team_On_Custom)
		int childIndex = findChild( Commands->Get_ID (sender) );
		if ( childIndex < 0 && sender != obj )
			return;

		// If the building has been captured by another team set the new object type
		if ( Get_Object_Type(obj) != param )
		{
			Set_Object_Type(obj,param);

			// Mirror the message to ourselves to trigger building functions scripts
			// to take action on it (they ignore messages not from the the same object)
			Commands->Send_Custom_Event ( obj, obj, CUSTOM_BUILDINGSCRIPTS_BUILDINGCAPTURED, param, 0.0f );

			// Notify all children of the change of ownership
			for ( int i = 0; i < numChildren; i++ )
			{
				GameObject* child = Commands->Find_Object ( children[i].objectId );
				if ( child != NULL )
					Commands->Send_Custom_Event ( obj, child, CUSTOM_BUILDINGSCRIPTS_BUILDINGCAPTURED, param, 0.0f );
			}
		}
	}
}

// -------------------------------------------------------------------------------------------------

int dp88_buildingScripts_controller::findChild ( int objectId )
{
	// Do we have any children to search?
	if ( children != NULL )
	{
		// Does this object ID exist in the array?
		for ( int i = 0; i < numChildren; i++ )
		{
			// Is this the right child? If so then return this index as the result
			if ( children[i].objectId == objectId )
				return i;
		}
	}

	// Not found
	return -1;
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_buildingScripts_controller> dp88_buildingScripts_controller_Registrant(
  "dp88_buildingScripts_controller",
  "Team_0_Building_Offline_String:string,"
  "Team_1_Building_Offline_String:string,"
  "Team_0_Building_Online_String:string,"
  "Team_1_Building_Online_String:string"
);








/*------------------------
Building scripts base class
--------------------------*/

dp88_buildingScripts_baseClass::dp88_buildingScripts_baseClass()
{
  m_parentId = -1;
}

// -------------------------------------------------------------------------------------------------

GameObject* dp88_buildingScripts_baseClass::RegisterWithParent ( GameObject* obj, const char* preset_name, int type )
{
  GameObject* parent = Find_Closest_Preset_By_Team( Get_Object_Type(obj), Commands->Get_Position(obj), preset_name );
  if ( !parent )
  {
    Console_Output ( "[%d:%s:%s] Critical Error: Building controller '%s' not found. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), preset_name );
    Destroy_Script();
    return NULL;
  }

  // Send with 0.5 second delay to ensure parent has been initialized
  m_parentId = Commands->Get_ID(parent);
  Commands->Send_Custom_Event ( obj, parent, CUSTOM_BUILDINGSCRIPTS_REGISTERCHILD, type, 0.5f );
  return parent;
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_baseClass::Custom ( GameObject *obj, int type, int param, GameObject *sender )
{
  if ( (m_parentId == -1 && obj == sender)
    || (m_parentId != -1 && Commands->Get_ID(sender) == m_parentId ) )
  {
    if ( type == CUSTOM_BUILDINGSCRIPTS_BUILDINGOFFLINE )
      return OnBuildingOffline(obj);
    if ( type == CUSTOM_BUILDINGSCRIPTS_BUILDINGONLINE )
      return OnBuildingOnline(obj);
    if ( type == CUSTOM_BUILDINGSCRIPTS_BUILDINGCAPTURED )
      return OnBuildingCaptured(obj, param);
    if ( type == CUSTOM_BUILDINGSCRIPTS_BUILDINGDESTROYED )
    {
      OnBuildingDestroyed(obj);
      m_parentId = -1;
      return;
    }
  }

  OnCustom(obj,type,param,sender);
}








/*------------------------
Building component base class
--------------------------*/

dp88_buildingScripts_componentBaseClass::dp88_buildingScripts_componentBaseClass()
{
  m_pAnimController = NULL;   // Prevent LE crash when Detach() is destroyed
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_componentBaseClass::Detach( GameObject* obj )
{
  dp88_buildingScripts_baseClass::Detach(obj);
  delete m_pAnimController;
  OnDetach(obj);
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_componentBaseClass::PlayAnimation( GameObject* obj, const char* animation, int nStartFrame, int nEndFrame, bool bLooped )
{
  if ( animation == NULL || strlen(animation) == 0 )
  {
    if (m_pAnimController)
      m_pAnimController->StopAnimation();
    return;
  }

  if ( m_pAnimController == NULL )
    m_pAnimController = new LoopedAnimationController(obj);

  m_pAnimController->StopAnimation();
  m_pAnimController->PlayAnimation(animation, nStartFrame, nEndFrame, (bLooped)?0:1);
}








/*------------------------
Building component script
--------------------------*/

void dp88_buildingScripts_component::Created ( GameObject *obj )
{
  if ( !RegisterWithParent(obj, Get_Parameter("Building_Controller_Preset"), BUILDING_CHILD_TYPE_GENERIC) )
    return;

  PlayAnimation ( obj,
    Get_Parameter("Animation_Name"),
    Get_Int_Parameter("Animation_Start_Frame"),
    Get_Int_Parameter("Animation_End_Frame"),
    (Get_Int_Parameter("Animation_Looped")==1) );

  // Attach link health script - no point cloning its functionality here
  Attach_Script_V ( obj,"dp88_linkHealth", "%d", m_parentId );
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_component::OnBuildingDestroyed( GameObject *obj )
{
  // Spawn dead preset
  const char* deadPreset = Get_Parameter("Dead_Preset");
  if ( Is_Valid_Preset(deadPreset) )
  {
    GameObject* dead = Commands->Create_Object( deadPreset, Commands->Get_Position(obj) );
    Commands->Set_Facing( dead, Commands->Get_Facing(obj) );
  }

  // And kill ourself
  Commands->Apply_Damage( obj, 1000.0f, "Death", obj );
}

// -------------------------------------------------------------------------------------------------

// Registrar
ScriptRegistrant<dp88_buildingScripts_component> dp88_buildingScripts_component_Registrant(
  "dp88_buildingScripts_component",
  "Building_Controller_Preset:string,"
  "Animation_Name:string,"
  "Animation_Start_Frame=0:int,"
  "Animation_End_Frame=0:int,"
  "Animation_Looped=0:int,"
  "Dead_Preset=null:string"
);








/*------------------------
Major weakpoint script
--------------------------*/

void dp88_buildingScripts_majorWeakpoint::Created ( GameObject *obj )
{
  if ( !RegisterWithParent(obj, Get_Parameter("Building_Controller_Preset"), BUILDING_CHILD_TYPE_MAJOR_WEAKPOINT) )
    return;

  PlayAnimation ( obj,
    Get_Parameter("Animation_Name"),
    Get_Int_Parameter("Animation_Start_Frame"),
    Get_Int_Parameter("Animation_End_Frame"),
    (Get_Int_Parameter("Animation_Looped")==1) );
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_majorWeakpoint::Killed ( GameObject* obj, GameObject* killer )
{
  // This might not be the only major weakpoint so send a custom to the building controller to let
  // it know we are dead and it can decide what to do... include the killers ID so they are credited
  // with destroying the building if all major weakpoints are now dead
  GameObject* parent = Commands->Find_Object ( m_parentId );
  if ( parent != NULL )
    Commands->Send_Custom_Event ( obj, parent, CUSTOM_BUILDINGSCRIPTS_CHILDDESTROYED, Commands->Get_ID(killer), 0.0f );

  // Spawn dead preset
  if ( Is_Valid_Preset ( Get_Parameter("Dead_Preset") ) )
  {
    GameObject* dead = Commands->Create_Object ( Get_Parameter("Dead_Preset"), Commands->Get_Position(obj) );
    Commands->Set_Facing ( dead, Commands->Get_Facing(obj) );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_majorWeakpoint::OnBuildingDestroyed( GameObject *obj )
{
  // Spawn dead preset
  const char* deadPreset = Get_Parameter("Dead_Preset");
  if ( Is_Valid_Preset(deadPreset) )
  {
    GameObject* dead = Commands->Create_Object( deadPreset, Commands->Get_Position(obj) );
    Commands->Set_Facing( dead, Commands->Get_Facing(obj) );
  }

  // And kill ourself
  Commands->Apply_Damage( obj, 1000.0f, "Death", obj );
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_buildingScripts_majorWeakpoint> dp88_buildingScripts_majorWeakpoint_Registrant(
  "dp88_buildingScripts_majorWeakpoint",
  "Building_Controller_Preset:string,"
  "Animation_Name:string,"
  "Animation_Start_Frame=0:int,"
  "Animation_End_Frame=0:int,"
  "Animation_Looped=0:int,"
  "Dead_Preset=null:string"
);








/*------------------------
Minor weakpoint script
--------------------------*/

void dp88_buildingScripts_minorWeakpoint::Created ( GameObject *obj )
{
  if ( !RegisterWithParent(obj, Get_Parameter("Building_Controller_Preset"), BUILDING_CHILD_TYPE_MINOR_WEAKPOINT) )
    return;

  m_bIsDestroyed = false;
  UpdateAnimation(obj);
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_minorWeakpoint::Damaged ( GameObject *obj, GameObject *damager, float amount )
{
  if (-1 == m_parentId)
    return;
  
  /* Has this weakpoint been "destroyed"? */
  if (!m_bIsDestroyed && Commands->Get_Shield_Strength(obj) == 0)
  {
    m_bIsDestroyed = true;

    // Send message to game controller indicating we have been destroyed
    GameObject* parent = Commands->Find_Object ( m_parentId );
    if ( parent != NULL )
      Commands->Send_Custom_Event ( obj, parent, CUSTOM_BUILDINGSCRIPTS_CHILDDESTROYED, 0, 0.0f );

    // Set animation
    UpdateAnimation(obj);
  }


  /* Has this weakpoint been fully repaired? */
  else if (m_bIsDestroyed && amount <= 0.0f && Commands->Get_Shield_Strength(obj) == Commands->Get_Max_Shield_Strength(obj))
  {
    m_bIsDestroyed = false;

    // Send message to game controller indicating we have been repaired
    GameObject* parent = Commands->Find_Object ( m_parentId );
    if ( parent != NULL )
      Commands->Send_Custom_Event ( obj, parent, CUSTOM_BUILDINGSCRIPTS_CHILDREPAIRED, 0, 0.0f );

    // Set animation
    UpdateAnimation(obj);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_minorWeakpoint::Killed ( GameObject* obj, GameObject* killer )
{
  // A minor weakpoint should never be killed... they should be repairable
  Console_Output ( "[%d:%s:%s] Script Error: A minor weakpoint has been killed...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_minorWeakpoint::OnBuildingDestroyed( GameObject *obj )
{
  m_bIsDestroyed = true;
  Commands->Set_Shield_Strength(obj, 0);
  Commands->Set_Shield_Type(obj, Get_Skin(obj));  // Skin type should be "Blamo" or equivalent
  UpdateAnimation(obj);
  // Don't call Destroy_Script(), this will stop the destroyed animation
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_minorWeakpoint::UpdateAnimation( GameObject* obj )
{
  if (!m_bIsDestroyed && -1 != m_parentId)
  {
    PlayAnimation ( obj,
      Get_Parameter("Animation_Name"),
      Get_Int_Parameter("Animation_Start_Frame"),
      Get_Int_Parameter("Animation_End_Frame"),
      (Get_Int_Parameter("Animation_Looped")==1) );
  }

  else
  {
    PlayAnimation ( obj,
      Get_Parameter("Destroyed_Animation_Name"),
      Get_Int_Parameter("Destroyed_Animation_Start_Frame"),
      Get_Int_Parameter("Destroyed_Animation_End_Frame"),
      (Get_Int_Parameter("Destroyed_Animation_Looped")==1) );
  }
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_buildingScripts_minorWeakpoint> dp88_buildingScripts_minorWeakpoint_Registrant(
  "dp88_buildingScripts_minorWeakpoint",
  "Building_Controller_Preset:string,"
  "Animation_Name:string,"
  "Animation_Start_Frame=0:int,"
  "Animation_End_Frame=0:int,"
  "Animation_Looped=0:int,"
  "Destroyed_Animation_Name:string,"
  "Destroyed_Animation_Start_Frame=0:int,"
  "Destroyed_Animation_End_Frame=0:int,"
  "Destroyed_Animation_Looped=0:int"
);








/*------------------------
Building function - Produce Infantry
--------------------------*/

void dp88_buildingScripts_functionProduceInfantry::OnBuildingOffline ( GameObject* obj )
{
  Set_Can_Generate_Soldiers ( Get_Object_Type(obj), false );
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_functionProduceInfantry::OnBuildingOnline ( GameObject* obj )
{
  Set_Can_Generate_Soldiers ( Get_Object_Type(obj), true );
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_buildingScripts_functionProduceInfantry> dp88_buildingScripts_functionProduceInfantry_Registrant(
  "dp88_buildingScripts_functionProduceInfantry",
  ""
);








/*------------------------
Building function - Produce Vehicles
--------------------------*/

void dp88_buildingScripts_functionProduceVehicles::OnBuildingOffline ( GameObject* obj )
{
  if (obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->As_VehicleFactoryGameObj())
  {
    obj->As_BuildingGameObj()->As_VehicleFactoryGameObj()->Set_Disabled(true);
  }

  auto vehiclesDef = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_VEHICLES, (PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(obj)));
  if (NULL != vehiclesDef)
  {
    vehiclesDef->Set_Page_Disabled(true);
  }
  auto secretVehiclesDef = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_SECRET_VEHICLES, (PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(obj)));
  if (NULL != secretVehiclesDef)
  {
    secretVehiclesDef->Set_Page_Disabled(true);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_functionProduceVehicles::OnBuildingOnline ( GameObject* obj )
{
  if (obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->As_VehicleFactoryGameObj())
  {
    obj->As_BuildingGameObj()->As_VehicleFactoryGameObj()->Set_Disabled(false);
  }

  auto vehiclesDef = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_VEHICLES, (PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(obj)));
  if (NULL != vehiclesDef)
  {
    vehiclesDef->Set_Page_Disabled(false);
  }
  auto secretVehiclesDef = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_SECRET_VEHICLES, (PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(obj)));
  if (NULL != secretVehiclesDef)
  {
    secretVehiclesDef->Set_Page_Disabled(false);
  }
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_buildingScripts_functionProduceVehicles> dp88_buildingScripts_functionProduceVehicles_Registrant(
  "dp88_buildingScripts_functionProduceVehicles",
  ""
);








/*------------------------
Building function - Produce Aircraft
--------------------------*/

void dp88_buildingScripts_functionProduceAircraft::OnBuildingOffline ( GameObject* obj )
{
  if (obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->As_AirFactoryGameObj())
  {
    obj->As_BuildingGameObj()->As_AirFactoryGameObj()->Set_Disabled(true);
  }

  auto airDef = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_AIR, (PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(obj)));
  if (NULL != airDef)
  {
    airDef->Set_Page_Disabled(true);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_functionProduceAircraft::OnBuildingOnline ( GameObject* obj )
{
  if (obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->As_AirFactoryGameObj())
  {
    obj->As_BuildingGameObj()->As_AirFactoryGameObj()->Set_Disabled(false);
  }

  auto airDef = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_AIR, (PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(obj)));
  if (NULL != airDef)
  {
    airDef->Set_Page_Disabled(false);
  }
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_buildingScripts_functionProduceAircraft> dp88_buildingScripts_functionProduceAircraft_Registrant(
  "dp88_buildingScripts_functionProduceAircraft",
  ""
);








/*------------------------
Building function - Produce Naval Units
--------------------------*/

void dp88_buildingScripts_functionProduceNavalUnits::OnBuildingOffline ( GameObject* obj )
{
  if (obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->As_NavalFactoryGameObj())
  {
    obj->As_BuildingGameObj()->As_NavalFactoryGameObj()->Set_Disabled(true);
  }

  auto navalDef = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_NAVAL, (PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(obj)));
  if (NULL != navalDef)
  {
    navalDef->Set_Page_Disabled(true);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_functionProduceNavalUnits::OnBuildingOnline ( GameObject* obj )
{
  if (obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->As_NavalFactoryGameObj())
  {
    obj->As_BuildingGameObj()->As_NavalFactoryGameObj()->Set_Disabled(false);
  }

  auto navalDef = PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_NAVAL, (PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(obj)));
  if (NULL != navalDef)
  {
    navalDef->Set_Page_Disabled(false);
  }
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_buildingScripts_functionProduceNavalUnits> dp88_buildingScripts_functionProduceNavalUnits_Registrant(
  "dp88_buildingScripts_functionProduceNavalUnits",
  ""
);








/*------------------------
Building function - Provide Power
--------------------------*/

void dp88_buildingScripts_functionProvidePower::OnBuildingOffline ( GameObject* obj )
{
  Power_Base ( Get_Object_Type(obj), false );
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_functionProvidePower::OnBuildingOnline ( GameObject* obj )
{
  Power_Base ( Get_Object_Type(obj), true );
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_buildingScripts_functionProvidePower> dp88_buildingScripts_functionProvidePower_Registrant(
  "dp88_buildingScripts_functionProvidePower",
  ""
);








/*------------------------
Building function - Provide Radar
--------------------------*/

void dp88_buildingScripts_functionProvideRadar::OnBuildingOffline ( GameObject* obj )
{
  Enable_Base_Radar ( Get_Object_Type(obj), false );
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_functionProvideRadar::OnBuildingOnline ( GameObject* obj )
{
  Enable_Base_Radar ( Get_Object_Type(obj), true );
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_buildingScripts_functionProvideRadar> dp88_buildingScripts_functionProvideRadar_Registrant(
  "dp88_buildingScripts_functionProvideRadar",
  ""
);








/*------------------------
Building function - Provide Harvester
--------------------------*/

void dp88_buildingScripts_functionProvideHarvester::OnBuildingOffline ( GameObject* obj )
{
  if (obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->As_RefineryGameObj())
    obj->As_BuildingGameObj()->As_RefineryGameObj()->Block_Harvester_Spawn();
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_functionProvideHarvester::OnBuildingOnline ( GameObject* obj )
{
  if (obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->As_RefineryGameObj())
    obj->As_BuildingGameObj()->As_RefineryGameObj()->Allow_Harvester_Spawn();
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_buildingScripts_functionProvideHarvester> dp88_buildingScripts_functionProvideHarvester_Registrant(
  "dp88_buildingScripts_functionProvideHarvester",
  ""
);








/*------------------------
Money Trickle Effect Timer
--------------------------*/

void dp88_buildingScripts_functionMoneyTrickle::Created ( GameObject *obj )
{
	isTimerRunning = false;
	if ( Get_Object_Type(obj) == 0 || Get_Object_Type(obj) == 1 )
	{
		isTimerRunning = true;
		Commands->Start_Timer(obj, this, 0.1f, TIMER_BUILDINGSCRIPTS_TRICKLEMONEY );
	}
}

void dp88_buildingScripts_functionMoneyTrickle::OnBuildingCaptured ( GameObject *obj, int team )
{
	if ( (team == 0 || team == 1) && !isTimerRunning )
	{
		isTimerRunning = true;
		Commands->Start_Timer(obj, this, 0.1f, TIMER_BUILDINGSCRIPTS_TRICKLEMONEY );
	}
}

void dp88_buildingScripts_functionMoneyTrickle::Timer_Expired ( GameObject* obj, int number )
{
	if ( number == TIMER_BUILDINGSCRIPTS_TRICKLEMONEY )
	{
		// Check team is still correct
		int team = Get_Object_Type(obj);
		if ( team == 0 || team == 1 )
		{
			MS_AccessHelper::Give_Bot_Credits(team, Get_Float_Parameter("creditsPerSec"));

			// NB: Calling Give_Money with the building controller as a parameter doesn't seem
			// to give anyone any money, so get the first player on the team and use them as
			// the target for Give_Money (true as third param to give to whole team)
			GameObject* pFirstPlayer = Find_First_Player(team);
			if ( pFirstPlayer )
				Commands->Give_Money(pFirstPlayer,Get_Float_Parameter("creditsPerSec"),true);
			Commands->Start_Timer(obj, this, 1.0f, TIMER_BUILDINGSCRIPTS_TRICKLEMONEY );
		}
		else		
			isTimerRunning = false;
	}
}


void dp88_buildingScripts_functionMoneyGrant::Created(GameObject *obj)
{
  m_credits = Get_Float_Parameter("credits");
  OnBuildingCaptured(obj, Get_Object_Type(obj));
}

void dp88_buildingScripts_functionMoneyGrant::OnBuildingCaptured(GameObject *obj, int team)
{
	if (team == 0 || team == 1)
	{
		MS_AccessHelper::Give_Bot_Credits(team, m_credits);

		GameObject* pFirstPlayer = Find_First_Player(team);
		if(pFirstPlayer)
		{
			Commands->Give_Money(pFirstPlayer, m_credits, true);
		}
		if (Get_Bool_Parameter("onceOnly"))
		{
		  m_credits = 0.0f;
			Destroy_Script();
		}
	}
}


/*------------------------
Building Function - Spawn Team Zone
--------------------------*/

void dp88_buildingScripts_functionSpawnTeamZone::Created ( GameObject *obj )
{
	// Set default values
	zoneId = NULL;

	// Create initial zone (if any)
	createZone(obj);
}


void dp88_buildingScripts_functionSpawnTeamZone::OnBuildingOffline( GameObject* obj )
{
	if ( Get_Int_Parameter("destroyZoneWhenDisabled") == 1 )
		destroyZone(obj);
}


void dp88_buildingScripts_functionSpawnTeamZone::OnBuildingOnline ( GameObject* obj )
{
	if ( Get_Int_Parameter("destroyZoneWhenDisabled") == 1 )
		createZone(obj);
}


void dp88_buildingScripts_functionSpawnTeamZone::OnBuildingCaptured ( GameObject* obj, int team )
{
	destroyZone(obj);
	createZone(obj);
}


void dp88_buildingScripts_functionSpawnTeamZone::Destroyed ( GameObject* obj )
{
	destroyZone(obj);
}


void dp88_buildingScripts_functionSpawnTeamZone::createZone(GameObject* obj)
{
	if ( zoneId == NULL )
	{
		// Decide which script zone to create based on current team type
		const char* zonePreset = "";
		if ( Get_Object_Type(obj) == 0 )
			zonePreset = Get_Parameter("scriptZonePreset_team0");
		else if ( Get_Object_Type(obj) == 1 )
			zonePreset = Get_Parameter("scriptZonePreset_team1");
		else
			zonePreset = Get_Parameter("scriptZonePreset_team2");


		if ( strlen(zonePreset) > 0 && Is_Valid_Preset(zonePreset) )
		{
			// Determine the size and rotation for the new zone
			Vector3 position = Commands->Get_Bone_Position(obj,Get_Parameter("scriptZoneBone"));
			Vector3 size = Get_Vector3_Parameter("scriptZoneSize");
			Matrix3 rotation(true);
			rotation.Rotate_Z(DEG2RAD(Commands->Get_Facing(obj)));

			// Define the bounding box and create the zone
			OBBoxClass zoneBoundingBox ( position, size, rotation );
			GameObject* zone = Create_Zone(zonePreset,zoneBoundingBox);

			// Store the zone ID
			if ( zone )
				zoneId = Commands->Get_ID(zone);
		}
	}
}


void dp88_buildingScripts_functionSpawnTeamZone::destroyZone(GameObject* obj)
{
	// Check zoneId is not null before attempting to destroy the zone
	if ( zoneId != NULL )
	{
		// Destroy the zone if it exists
		GameObject* zone = Commands->Find_Object(zoneId);
		if ( zone )
			Commands->Destroy_Object(zone);
		zoneId = NULL;
	}
}




/*------------------------
Building Script Zone
--------------------------*/

void dp88_buildingScripts_functionSpawnZone::Created ( GameObject *obj )
{
  if ( !RegisterWithParent(obj, Get_Parameter("Building_Controller_Preset"), BUILDING_CHILD_TYPE_GENERIC) )
    return;

  zoneId = NULL;

  // Check the script zone preset is valid and, if it is, create the zone
  if ( Is_Valid_Preset(Get_Parameter("scriptZonePreset")) )
    createZone(obj);

  else
  {
    Console_Output ( "[%d:%s:%s] Critical Error: A script zone preset with the name '%s' was not found. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Parameter("scriptZonePreset") );
    Destroy_Script();
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_functionSpawnZone::OnBuildingOffline( GameObject* obj )
{
  if ( Get_Int_Parameter("destroyZoneWhenDisabled") == 1 )
    destroyZone(obj);
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_functionSpawnZone::OnBuildingOnline ( GameObject* obj )
{
  if ( Get_Int_Parameter("destroyZoneWhenDisabled") == 1 )
    createZone(obj);
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_functionSpawnZone::OnBuildingDestroyed ( GameObject* obj )
{
  if ( Get_Int_Parameter("destroyZoneWhenDestroyed") == 1 )
    destroyZone(obj);
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_functionSpawnZone::createZone(GameObject* obj)
{
  // Check that zoneId is NULL before creating a new zone
  if ( zoneId == NULL )
  {
    // Determine the size and rotation for the new zone
    Vector3 position = Commands->Get_Position(obj);
    Vector3 size = Get_Vector3_Parameter("scriptZoneSize");
    Matrix3 rotation(true);
    rotation.Rotate_Z(DEG2RAD(Commands->Get_Facing(obj)));

    // Define the bounding box and create the zone
    OBBoxClass zoneBoundingBox ( position, size, rotation );
    GameObject* zone = Create_Zone(Get_Parameter("scriptZonePreset"),zoneBoundingBox);

    // Store the zone ID
    if ( zone )
      zoneId = Commands->Get_ID(zone);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_functionSpawnZone::destroyZone(GameObject* obj)
{
  // Check zoneId is not null before attempting to destroy the zone
  if ( zoneId != NULL )
  {
    // Destroy the zone if it exists
    GameObject* zone = Commands->Find_Object(zoneId);
    if ( zone )
      Commands->Destroy_Object(zone);
    zoneId = NULL;
  }
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_buildingScripts_functionSpawnZone> dp88_buildingScripts_functionSpawnZone_Registrant(
  "dp88_buildingScripts_functionSpawnZone",
  "Building_Controller_Preset:string,"
  "scriptZonePreset:string,"
  "scriptZoneSize:vector3,"
  "destroyZoneWhenDisabled=1:int,"
  "destroyZoneWhenDestroyed=1:int"
);








/*------------------------
Building Capture Point
--------------------------*/

void dp88_buildingScripts_capturePoint::Created( GameObject *obj )
{
  GameObject* parent = RegisterWithParent(obj, Get_Parameter("Building_Controller_Preset"), BUILDING_CHILD_TYPE_CAPTUREPOINT);
  if ( !parent )
    return;

  // Check parent is not a building (since changing the team of a BuildingGameObj results in some
  // very strange behaviour that we really don't want)
  if ( parent->As_BuildingGameObj() )
  {
    Console_Output ( "[%d:%s:%s] Critical Error: Building controller '%s' is of type BuildingGameObj, capturing is not possible. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
    Commands->Send_Custom_Event(obj, parent, CUSTOM_BUILDINGSCRIPTS_UNREGISTERCHILD, 0, 1.0f);
    Destroy_Script();
    return;
  }

  currentTeam = Get_Object_Type(parent);
  Set_Object_Type(obj,currentTeam);

  // Set the current health state to match the current team (0 = Soviet, 1 = Allies, 2 = Neutral)
  if ( currentTeam == 0 )
    Commands->Set_Health(obj, 0.25f);
  else if ( currentTeam == 1 )
    Commands->Set_Health(obj, Commands->Get_Max_Health(obj));
  else
    Commands->Set_Health(obj, Commands->Get_Max_Health(obj)/2.0f);
  
  m_nAnimTransitionFrames = Get_Int_Parameter("Capture_Anim_Transition_Frames");
  UpdateAnimationFrame(obj);

  // Start timer to constantly move health towards equilibrium
  Commands->Start_Timer (obj, this, 1.0f, TIMER_BUILDINGSCRIPTS_CAPTUREPOINT );
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_capturePoint::Damaged( GameObject *obj, GameObject *damager, float amount )
{
  // Check the damage was done with the correct warhead, otherwise we are not interested
  unsigned int warhead = ArmorWarheadManager::Get_Warhead_Type(Get_Parameter("Warhead"));
  if ( warhead != Get_Damage_Warhead() )
    return;

  float health = Commands->Get_Health(obj);
  float max = Commands->Get_Max_Health(obj);

  // If the damager was on team 0 then decrease the health by 1 and check if we have been captured
  if ( Commands->Get_Player_Type(damager) == 0 )
  {
    // Decrease health by 1.00 (unless we are already below 1.25 in which case we just set it to 0.25)
    health = ( health <= 1.25f ) ? 0.25f : health - 1.0f;
    Commands->Set_Health (obj, health );

    // Did we capture the structure?
    if ( health == 0.25f && currentTeam != 0 )
    {
      currentTeam = 0;

      Send_Translated_Message_Team( Get_Int_Parameter("Sov_String_All"), 1, NodHouseColor );
      Send_Translated_Message_Team( Get_Int_Parameter("Sov_String_Sov"), 0, NodHouseColor );

      Set_Object_Type(obj,currentTeam);
      Commands->Send_Custom_Event (obj, Commands->Find_Object(m_parentId), CUSTOM_BUILDINGSCRIPTS_BUILDINGCAPTURED, currentTeam, 0.0f );
    }

    // OK, we didn't capture it, but did we manage to make it neutral?
    else if ( currentTeam == 1 && health <= max/2.0f )
    {
      currentTeam = 2;

      Send_Translated_Message_Team( Get_Int_Parameter("Neutral_String_All"), 1, PublicMessageColor );
      Send_Translated_Message_Team( Get_Int_Parameter("Neutral_String_Sov"), 0, PublicMessageColor );

      Set_Object_Type(obj,currentTeam);
      Commands->Send_Custom_Event (obj, Commands->Find_Object(m_parentId), CUSTOM_BUILDINGSCRIPTS_BUILDINGCAPTURED, currentTeam, 0.0f );
    }
  }

  
  // If the damager was on team 1 then increase the health by 1 and check if we have been captured
  else if (Commands->Get_Player_Type(damager) == 1)
  {
    // Increase health by up to 1
    health = ( health >= max-1.0f ) ? max : health + 1.0f;
    Commands->Set_Health (obj, health );

    // Did we capture the structure?
    if ( health == max && currentTeam != 1 )
    {
      currentTeam = 1;

      Send_Translated_Message_Team( Get_Int_Parameter("All_String_All"), 1, GDIHouseColor );
      Send_Translated_Message_Team( Get_Int_Parameter("All_String_Sov"), 0, GDIHouseColor );

      Set_Object_Type(obj,currentTeam);
      Commands->Send_Custom_Event (obj, Commands->Find_Object(m_parentId), CUSTOM_BUILDINGSCRIPTS_BUILDINGCAPTURED, currentTeam, 0.0f );
    }

    // OK, we didn't capture it, but did we manage to make it neutral?
    else if ( currentTeam == 0 && health >= max/2.0f )
    {
      currentTeam = 2;

      Send_Translated_Message_Team( Get_Int_Parameter("Neutral_String_All"), 1, PublicMessageColor );
      Send_Translated_Message_Team( Get_Int_Parameter("Neutral_String_Sov"), 0, PublicMessageColor );

      Set_Object_Type(obj,currentTeam);
      Commands->Send_Custom_Event (obj, Commands->Find_Object(m_parentId), CUSTOM_BUILDINGSCRIPTS_BUILDINGCAPTURED, currentTeam, 0.0f );
    }
  }

  UpdateAnimationFrame(obj);
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_capturePoint::OnBuildingDestroyed( GameObject *obj )
{
  // Spawn dead preset
  if ( Is_Valid_Preset ( Get_Parameter("Dead_Preset") ) )
  {
    GameObject* dead = Commands->Create_Object ( Get_Parameter("Dead_Preset"), Commands->Get_Position(obj) );
    Commands->Set_Facing ( dead, Commands->Get_Facing(obj) );
  }

  // And kill ourself
  Commands->Apply_Damage ( obj, 1000.0f, "Death", obj );
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_capturePoint::OnBuildingCaptured( GameObject *obj, int team )
{
  // Captured by some external mechanism such as using dp88_Set_Team_On_Custom
  if ( team != Get_Object_Type(obj) && -1 != m_parentId )
  {
    currentTeam = team;
    Set_Object_Type(obj,currentTeam);

    // Set the current health state to match the current team (0 = Soviet, 1 = Allies, 2 = Neutral)
    if ( currentTeam == 0 )
      Commands->Set_Health(obj, 0.25f);
    else if ( currentTeam == 1 )
      Commands->Set_Health(obj, Commands->Get_Max_Health(obj));
    else
      Commands->Set_Health(obj, Commands->Get_Max_Health(obj)/2.0f);
  
    UpdateAnimationFrame(obj);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_capturePoint::Timer_Expired( GameObject *obj, int number )
{
  // Move health back towards equilibrium point for current owner
  if ( number == TIMER_BUILDINGSCRIPTS_CAPTUREPOINT )
  {
    float health = Commands->Get_Health(obj);

    if ( currentTeam == 0 && health != 0.25f )
    {
      // Decrease health by 0.25 (unless we are already below 0.50 in which
      // case we just set it to 0.25)
      health = ( health <= 0.50f ) ? 0.25f : health - 0.25f;

      // Apply new health
      Commands->Set_Health(obj, health);
      UpdateAnimationFrame(obj);
    }

    else
    {
      float max = Commands->Get_Max_Health(obj);

      if ( currentTeam == 1 && health != max )
      {
        // Increase health by 0.25, unless we are within 0.25 of the maximum
        // health value, in which case we use that
        health = ( health+0.25f >= max ) ? max : health + 0.25f;

        // Apply new health
        Commands->Set_Health(obj, health);
        UpdateAnimationFrame(obj);
      }

      else if ( health != max/2.0f )
      {
        // Are we above or below the halfway point?
        bool above = ( health > max/2.0f );

        // Are we within 0.25 points of the halfway point? If so set that as
        // our new health value (we have reached equilibrium)
        if ( (above && (health-0.25f <= max/2.0f)) || (!above &&  (health+0.25f >= max/2.0f)) )
          Commands->Set_Health(obj, max/2.0f);

        // Otherwise move 0.25 points closer to the halfway point
        else
          Commands->Set_Health(obj, (above) ? health-0.25f : health+0.25f );

        UpdateAnimationFrame(obj);
      }
    }


    // Start timer for next iteration
    Commands->Start_Timer (obj, this, 1.0f, TIMER_BUILDINGSCRIPTS_CAPTUREPOINT );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_capturePoint::UpdateAnimationFrame( GameObject* obj )
{
  float health = Commands->Get_Health(obj);
  if ( health == 0.25f )
    health = 0.0f;    // 0.25 is the Soviet state

  float healthPercent = (health/Commands->Get_Max_Health(obj));
  float capturePercent;
  if ( healthPercent > 0.5f )   // Only interested in progress beyond the halfway point
    capturePercent = (healthPercent-0.5f)*2;
  else                    // Invert progress (% of way from halfway to 0)
    capturePercent = (0.5f-healthPercent)*2;

  int animFrame = 1+m_nAnimTransitionFrames;  // Neutral frame
  int teamFrames = (int)floor((1+m_nAnimTransitionFrames) * capturePercent);
  animFrame += (healthPercent < 0.5f) ? (teamFrames*-1) : teamFrames;
  Commands->Set_Animation_Frame(obj, Get_Parameter("Capture_Anim"), animFrame);
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_buildingScripts_capturePoint> dp88_buildingScripts_capturePoint_Registrant(
  "dp88_buildingScripts_capturePoint",
  "Building_Controller_Preset:string,"
  "Warhead:string,"
  "Capture_Anim:string,"
  "Capture_Anim_Transition_Frames=0:int"
  "Dead_Preset=0:int,"
  "All_String_All:int,"
  "Sov_String_All:int,"
  "Neutral_String_All:int,"
  "All_String_Sov:int,"
  "Sov_String_Sov:int,"
  "Neutral_String_Sov:int"
);










/*------------------------
Spawn Building Part script
--------------------------*/

void dp88_buildingScripts_spawnPart::Created ( GameObject* obj )
{
  partObjectId = NULL;

  /* Start a timer to spawn the building part. We can't do this immediately because
  it seems to cause issues with double initializations of scripts... */
  Commands->Start_Timer ( obj, this, 0.01f, TIMER_SPAWNVEHICLEPART );
}


void dp88_buildingScripts_spawnPart::Timer_Expired( GameObject *obj, int number )
{
	/* Part spawn timer */
	if ( number == TIMER_SPAWNVEHICLEPART && partObjectId == NULL )
	{
		/* Spawn the building part and get it's ID */
		GameObject* part = Commands->Create_Object_At_Bone ( obj, Get_Parameter("Building_Part_Preset"), Get_Parameter("Spawn_At_Bone") );
		Commands->Attach_To_Object_Bone ( part, obj, Get_Parameter ( "Spawn_At_Bone" ) );
		partObjectId = Commands->Get_ID(part);

		/* Unless this is a decorative part start the timer again to attach the health and
		veterancy link scripts. We don't do this immediately as it causes a lovely crash... */
		if ( Get_Int_Parameter("IsDecorative") == 0 )
		  Commands->Start_Timer ( obj, this, 0.01f, TIMER_SPAWNVEHICLEPART );
	}


	/* Link health & veterancy script attach timer */
	else if ( number == TIMER_SPAWNVEHICLEPART )
	{
		GameObject* part = Commands->Find_Object(partObjectId);
		if ( part )
		{
			Attach_Script_Once_V ( part, "dp88_linkHealth", "%d", Commands->Get_ID(obj) );
			Attach_Script_Once_V ( part, "dp88_linkVetPoints", "%d", Commands->Get_ID(obj) );
		}
	}
}


void dp88_buildingScripts_spawnPart::OnBuildingCaptured ( GameObject* obj, int team )
{
	if ( partObjectId != NULL && Commands->Find_Object(partObjectId) )
		Set_Object_Type( Commands->Find_Object (partObjectId), team );
}


void dp88_buildingScripts_spawnPart::Destroyed ( GameObject* obj )
{
	if ( partObjectId != NULL && Commands->Find_Object(partObjectId) )
		Commands->Destroy_Object ( Commands->Find_Object (partObjectId) );
	partObjectId = NULL;
}





ScriptRegistrant<dp88_buildingScripts_functionMoneyTrickle> dp88_buildingScripts_functionMoneyTrickle_Registrant( "dp88_buildingScripts_functionMoneyTrickle", "creditsPerSec=3.0:float" );
ScriptRegistrant<dp88_buildingScripts_functionSpawnTeamZone> dp88_buildingScripts_functionSpawnTeamZone_Registrant( "dp88_buildingScripts_functionSpawnTeamZone", "scriptZonePreset_team0:string,scriptZonePreset_team1:string,scriptZonePreset_team2:string,scriptZoneBone:string,scriptZoneSize:vector3,destroyZoneWhenDisabled=1:int" );
ScriptRegistrant<dp88_buildingScripts_functionMoneyGrant> dp88_buildingScripts_functionMoneyGrant_Registrant( "dp88_buildingScripts_functionMoneyGrant", "credits:float,onceOnly=1:int");
ScriptRegistrant<dp88_buildingScripts_spawnPart> dp88_buildingScripts_spawnPart_Registrant("dp88_buildingScripts_spawnPart","Building_Part_Preset:string,Spawn_At_Bone:string,IsDecorative=0:int");
