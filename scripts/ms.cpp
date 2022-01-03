#include "general.h"
#include "ms.h"
#include "VehicleGameObj.h"
#include "dp88_custom_timer_defines.h"
#include "engine_game.h"
#include "engine_script.h"
#include "engine_obj.h"
#include "engine_obj2.h"
#include "engine_player.h"
#include "engine_def.h"
#include "engine_dmg.h"
#include "engine_weap.h"
#include "TwiddlerClass.h"
#include "DefinitionMgrClass.h"
#include "random2class.h"
#include "TranslateDBClass.h"
#include "engine_tt.h"

#define REGISTER_SCRIPT(name, params) ScriptRegistrant<name> name##Registrant(#name, params);

/*------------------------
Generic / Utility scripts 
--------------------------*/


/*!
* \brief Send custom based on health
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
*
* When attached to an object, this script can make that object send customs around when its health
* goes under or above a given value.

* The custom is sent only once until the health goes to the other side of the treshold.
* This means if the Health parameter is set to 100 and the object's health is 99 then the custom is sent once.
* Then the health needs to reach 100 and then 99 once again to send the custom once more.
*
* \param Health
*   Current HP will be always checked against this value if it is under or above it.
* \param UnderID
*   The ID of the object to send the custom message to when the health is under the treshold.
*	When set to -1, nothing will get a message.
*	0 sends to self.
* \param UnderCustom
*   The under custom message that will be sent to the object defined in the ID.
* \param UnderParam
*   The parameter of the under custom message that will be sent to the object defined in the ID.
* \param AboveID
*   The ID of the object to send the custom message to when the health is above the treshold.
*	When set to -1, nothing will get a message.
*	0 sends to self.
* \param AboveCustom
*   The above custom message that will be sent to the object defined in the ID.
* \param AboveParam
*   The parameter of the above custom message that will be sent to the object defined in the ID.
*/
class MS_Health_Send_Custom : public ScriptImpClass
{
	float tresholdHP;
	float currentHP;
	bool underIgnored;
	bool aboveIgnored;
	bool underTriggered;
	bool aboveTriggered;
	bool triggerOnInstantDeath;
	bool isPreventingSpam;
	int underID;
	int aboveID;

	void Created(GameObject* obj) override
	{
		underIgnored = false;
		aboveIgnored = false;
		underTriggered = false;
		isPreventingSpam = false;

		aboveTriggered = true;
		tresholdHP = Get_Float_Parameter("Health");
		underID = Get_Int_Parameter("UnderID");
		aboveID = Get_Int_Parameter("AboveID");
		triggerOnInstantDeath = Get_Int_Parameter("TriggerOnInstantDeath") == 1;

		if (underID == -1)
		{
			underIgnored = true;
		}

		if (aboveID == -1)
		{
			aboveIgnored = true;
		}
		Do_Checks(obj);
	}

	void Do_Checks(GameObject *obj)
	{
		currentHP = Commands->Get_Health(obj);
		if (currentHP <= 0 && !triggerOnInstantDeath)
		{
			return;
		}

		// Under
		if (!underTriggered && currentHP < tresholdHP)
		{
			underTriggered = true;
			aboveTriggered = false;
			if (!underIgnored)
			{
				GameObject *targetUnder = underID ? Commands->Find_Object(underID) : obj;
				Commands->Send_Custom_Event(obj, targetUnder, Get_Int_Parameter("UnderCustom"), Get_Int_Parameter("UnderParam"), 0);
			}
		}
		// Above
		else if (!aboveTriggered && currentHP >= tresholdHP)
		{
			aboveTriggered = true;
			underTriggered = false;
			if (!aboveIgnored)
			{
				GameObject *targetAbove = aboveID ? Commands->Find_Object(aboveID) : obj;
				Commands->Send_Custom_Event(obj, targetAbove, Get_Int_Parameter("AboveCustom"), Get_Int_Parameter("AboveParam"), 0);
			}
		}
	}

	void Damaged(GameObject *obj, GameObject *damager, float amount) override
	{
		if (!isPreventingSpam)
		{
			Do_Checks(obj);
			isPreventingSpam = true;
			Commands->Start_Timer(obj, this, 1.0f, 0);

		}
	}

	void Timer_Expired(GameObject* obj, int number) override
	{
		Do_Checks(obj);
		isPreventingSpam = false;
	}
};
REGISTER_SCRIPT(MS_Health_Send_Custom, "Health=0.0:float,UnderID=-1:int,UnderCustom=0:int,UnderParam=0:int,AboveID=-1:int,AboveCustom=0:int,AboveParam=0:int,TriggerOnInstantDeath=0:int");


/*!
* \brief Send custom when fully healed
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
*
* This script lets you send a custom when the object it is attached to reaches full health/armor after being damaged.
*
* \param ID
*   The ID of the object to send the custom message to.
*   0 sends to self.
* \param Custom
*   The custom message that will be sent to the object defined in the ID.
* \param Param
*   The parameter of the custom message that will be sent to the object defined in the ID.
* \param Delay
*   How many seconds to wait before the custom is sent to the object defined in the ID.
*/
class MS_Healed_Send_Custom : public ScriptImpClass
{
	bool damaged;

	void Created(GameObject* obj) override
	{
		damaged = false;
	}

	void Damaged(GameObject *obj, GameObject *damager, float amount) override
	{
		if (Commands->Get_Health(obj) < Commands->Get_Max_Health(obj) || Commands->Get_Shield_Strength(obj) < Commands->Get_Max_Shield_Strength(obj))
		{
			damaged = true;
		}
		if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj) && Commands->Get_Shield_Strength(obj) == Commands->Get_Max_Shield_Strength(obj) && damaged)
		{
			damaged = false;
			int id = Get_Int_Parameter("ID");
			GameObject *receiver = id ? Commands->Find_Object(id) : obj;
			if (receiver)
				Commands->Send_Custom_Event(obj, receiver, Get_Int_Parameter("Custom"), Get_Int_Parameter("Param"), Get_Float_Parameter("Delay"));
		}
	}
};
REGISTER_SCRIPT(MS_Healed_Send_Custom, "ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float");


/*!
* \brief Send custom when receive custom conditionally
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
*
* This script lets you send a custom when the object it is attached receives a custom,
* with an addition of a switch/gate that enables/disables the processing.
*
* \param ReceiveCustom
*   The custom message to listen for. Ignored until EnableCustom is received first.
* \param SendCustom
*   The custom message that will be sent to the object defined in the ID.
* \param Param
*   The parameter of the custom message that will be sent to the object defined in the ID.
* \param Delay
*   How many seconds to wait before the custom is sent to the object defined in the ID.
* \param ID
*   The ID of the object to send the custom message to.
*   0 sends to self.
* \param EnableCustom
*   The custom message that enables sending the custom.
* \param DisableCustom
*   The custom message that disables sending the custom.
*/
class MS_Custom_Send_Custom_Switch : public ScriptImpClass
{
	bool enabled;

	void Created(GameObject* obj) override
	{
		enabled = false;
	}

	void Custom(GameObject *obj,int type,int param,GameObject *sender) override
	{
		if (type == Get_Int_Parameter("EnableCustom"))
		{
			enabled = true;
		}
		else if (type == Get_Int_Parameter("DisableCustom"))
		{
			enabled = false;
		}
		else if (enabled && type == Get_Int_Parameter("ReceiveCustom"))
		{
			int id = Get_Int_Parameter("ID");
			GameObject *receiver = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
			if (receiver)
				Commands->Send_Custom_Event(obj, receiver, Get_Int_Parameter("SendCustom"), Get_Int_Parameter("Param"), Get_Float_Parameter("Delay"));
		}
	}
};
REGISTER_SCRIPT(MS_Custom_Send_Custom_Switch, "ReceiveCustom=0:int,SendCustom=0:int,Param=0:int,Delay=0.0:float,ID=0:int,EnableCustom=0:int,DisableCustom=0:int");


/*!
* \brief Set max health on custom
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
*
* This script lets you set the maximum health on the object it's attached to when it receives a custom.
* It does not heal the object after doing so.
*
* \param Custom
*   The custom message to listen for.
* \param Value
*   The value to update the maximum health to.
*/
class MS_Custom_Set_Max_Health : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender) override
	{
		if (type == Get_Int_Parameter("Custom"))
		{
			Set_Max_Health_Without_Healing(obj,Get_Float_Parameter("Value"));
		}
	}
};
REGISTER_SCRIPT(MS_Custom_Set_Max_Health, "Custom=0:int,Value=0.0:float");


/*!
* \brief Set max shield on custom
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
*
* This script lets you set the maximum shield on the object it's attached to when it receives a custom.
* It does not heal the object after doing so.
*
* \param Custom
*   The custom message to listen for.
* \param Value
*   The value to update the maximum shield to.
*/
class MS_Custom_Set_Max_Shield : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender) override
	{
		if (type == Get_Int_Parameter("Custom"))
		{
			Set_Max_Shield_Strength_Without_Healing(obj,Get_Float_Parameter("Value"));
		}
	}
};
REGISTER_SCRIPT(MS_Custom_Set_Max_Shield, "Custom=0:int,Value=0.0:float");


/*!
* \brief Grant credits to an entire team when custom is received
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
*
* This script grants credits to all players of a team when it receives a custom.
*
* \param Custom
*   The custom message to listen for. When it arrives, the credits will be distributed.
* \param Team
*   Which team to give credits to. Every player on that team will get the credits.
* \param Amount
*   The amount of credits to grant.
*/
/*class MS_Custom_Grant_Credits_Team : public ScriptImpClass
{
	void Custom(GameObject *obj, int type, int param, GameObject *sender) override
	{
		if (type == Get_Int_Parameter("Custom"))
		{
			Commands->Give_Money_To_Team(Get_Int_Parameter("Team"), (float)Get_Int_Parameter("Amount"));
		}
	}
};
REGISTER_SCRIPT(MS_Custom_Grant_Credits_Team, "Custom=0:int,Team=0:int,Amount=0:int");*/


/*!
* \brief Enable or disable specifically named spawners when custom is received
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
*
* This script lets you enable or disable one or more spawners when it receives a custom.
*
* \param Custom
*   The custom message to listen for.
* \param SpawnerName
*   The name of the spawner preset(s) that will be affected.
* \param Enable
*   Enable or disable the spawner(s).
*/
class MS_Custom_Toggle_Spawners : public ScriptImpClass
{
	void Created(GameObject* obj) override
	{
		if (!Is_Valid_Preset(Get_Parameter("SpawnerName")))
		{
			Console_Output("[%d:%hs:%hs] Critical Error: '%hs' is not a valid preset. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Parameter("SpawnerName"));
			Destroy_Script();
			return;
		}
	}

	void Custom(GameObject *obj, int type, int param, GameObject *sender) override
	{
		if (type == Get_Int_Parameter("Custom"))
		{
			Enable_Spawners_By_Name(Get_Parameter("SpawnerName"), Get_Int_Parameter("Enable") == 1);
		}
	}
};
REGISTER_SCRIPT(MS_Custom_Toggle_Spawners, "Custom=0:int,SpawnerName:string,Enable=1:int");


/*!
* \brief Paradrop script
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
*
* Make a character fall slowly with an optional parachute model attached until the character lands.
* Landing is detected by the first falling (Earth warhead) damage received.
*
* \param Parachute_Model
*   The name of the parachute W3D model that will be attached. 
*   The extension .w3d can be omitted, enter the name only.
*   If left empty, no model will be attached.
*/
/*class MS_Paradrop : public ScriptImpClass
{
	int parachuteObjID = 0;

	int fallFailCount = 0;
	int groundCloseCount = 0;

	unsigned int earthWarhead = 0;
	float cachedHP = 0.0f;
	float cachedShield = 0.0f;

	bool isArmorChanged = false;
	bool isCleanedUp = false;

	void Created(GameObject* obj) override
	{
		earthWarhead = ArmorWarheadManager::Get_Warhead_Type("Earth");
		cachedHP = Commands->Get_Health(obj);
		cachedShield = Commands->Get_Shield_Strength(obj);

		if (auto soldier = obj->As_SoldierGameObj())
		{
			soldier->Set_Grav_Scale(0);
		}

		Set_Velocity(obj, Vector3(0, 0, -5));
		const char* parachuteModel = Get_Parameter("Parachute_Model");
		if (parachuteModel[0] != '\0' && strcmp(parachuteModel, "null") != 0 && strcmp(parachuteModel, "NULL") != 0)
		{
			auto parachute = Commands->Create_Object("Invisible_Object", Commands->Get_Position(obj));
			Commands->Set_Model(parachute, parachuteModel);
			Commands->Attach_To_Object_Bone(parachute, obj, "ROOTTRANSFORM");
			parachuteObjID = parachute->Get_ID();
		}

		const char* newSkin = Get_Parameter("Parachuting_Skin");
		const char* newShield = Get_Parameter("Parachuting_Shield");
		if (strcmp(newSkin, "null") != 0 || strcmp(newShield, "null") != 0)
		{
			isArmorChanged = true;
			char scriptParams[256];
			snprintf(scriptParams, sizeof(scriptParams), "%s,%s,0", newSkin, newShield);
			Commands->Attach_Script(obj, "MS_Switch_Skin_And_Shield_Timer", scriptParams);
		}

		Commands->Start_Timer(obj, this, 0, 0);
	}

	void Timer_Expired(GameObject* obj, int number) override
	{
		// Timed destroy, started by cleanup
		if (number == -1)
		{
			Destroy_Script();
			return;
		}

		// No more timers after cleanup
		if (isCleanedUp)
			return;

		// Check for being in a vehicle, ladder or elevator for crazy situations
		if (Get_Vehicle(obj) || obj->As_SoldierGameObj()->Is_On_Ladder() || obj->As_SoldierGameObj()->Is_In_Elevator())
		{
			Clean_Up();
			return;
		}

		Commands->Start_Timer(obj, this, 0.5f, 0);

		// Keep checking ground distance
		if (Commands->Is_Something_Below(obj, "ROOTTRANSFORM", 1.5f, false))
			groundCloseCount++;
		else
			groundCloseCount = 0;

		// Set velocity to falling if the paratrooper was blocked for a moment in mid-air
		auto vel = Get_Velocity(obj);
		if (vel.Z > -5)
		{
			fallFailCount++;
			Set_Velocity(obj, Vector3(vel.X, vel.Y, -5));
		}
		else
			fallFailCount = 0;

		// Falling is blocked or is standing on the ground, standstill can have -5 velocity. Clean up
		if (fallFailCount > 4 || groundCloseCount > 4)
		{
			Clean_Up();
		}
	}

	void Damaged(GameObject *obj, GameObject *damager, float amount) override
	{
		if (Commands->Get_Damage_Warhead() == earthWarhead)
		{
			Commands->Set_Health(obj, cachedHP);
			Commands->Set_Shield_Strength(obj, cachedShield);
			Clean_Up();
		}
		else
		{
			cachedHP = Commands->Get_Health(obj);
			cachedShield = Commands->Get_Shield_Strength(obj);
		}
	}

	void Clean_Up()
	{
		if (!isCleanedUp)
		{
			isCleanedUp = true;
			if (auto soldier = Owner()->As_SoldierGameObj())
			{
				soldier->Reset_Grav_Scale();
			}
			if (isArmorChanged)
			{
				Remove_Script(Owner(), "MS_Switch_Skin_And_Shield_Timer");
			}
			Commands->Destroy_Object(Commands->Find_Object(parachuteObjID));
			Commands->Start_Timer(Owner(), this, 1.0f, -1);
		}
	}

	void Killed(GameObject *obj, GameObject *killer) override
	{
		Clean_Up();
	}

	void Destroyed(GameObject *obj) override
	{
		Clean_Up();
	}

	void Detach(GameObject* obj) override
	{
		Clean_Up();
		ScriptImpClass::Detach(obj);
	}
};
REGISTER_SCRIPT(MS_Paradrop,
	"Parachute_Model:string,"
	"Parachuting_Skin=null:string,"
	"Parachuting_Shield=null:string"
);*/


/*!
* \brief Switch skin and shield
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
*
* When attached to an object, this script will change that object's skin and/or shield type to a new one
* for a set amount of time. After the time runs out, the original skin and shield types are set on the object again.
* If the Time is set to 0, the newly set skin and armor types will stay on the object permanently.
*
* \param Skin
*   The skin type to switch to.
*	When left as "null", the skin type will not be changed.
* \param Shield
*   The skin type to switch to.
*	When left as "null", the shield type will not be changed.
* \param Time
*   The time duration for how long the object should wield the new skin and shield types.
*	When set to 0, the new skin and shield types stay permanently, or until the script is removed from the object.
*/
class MS_Switch_Skin_And_Shield_Timer : public ScriptImpClass
{
	const char *OriginalSkin;
	const char *OriginalShield;

	void Created(GameObject* obj) override
	{
		OriginalSkin = Get_Skin(obj);
		OriginalShield = Get_Shield_Type(obj);

		const char *ParamSkin = Get_Parameter("Skin");
		const char *ParamShield = Get_Parameter("Shield");

		if (strcmp(ParamSkin, "null") != 0)
		{
			Set_Skin(obj, ParamSkin);
		}
		if (strcmp(ParamShield, "null") != 0)
		{
			Commands->Set_Shield_Type(obj, ParamShield);
		}

		float time = Get_Float_Parameter("Time");
		if (time != 0)
		{
			Commands->Start_Timer(obj, this, time, 1);
		}

	}

	void Timer_Expired(GameObject* obj, int number) override
	{
		if (number == 1)
		{
			Destroy_Script();
		}
	}

	void Detach(GameObject* obj)
	{
		Set_Skin(obj, OriginalSkin);
		Commands->Set_Shield_Type(obj, OriginalShield);
		ScriptImpClass::Detach(obj);
	}
};
REGISTER_SCRIPT(MS_Switch_Skin_And_Shield_Timer, "Skin=null:string,Shield=null:string,Time=0.0:float");


/*!
* \brief Kill passengers on vehicle death
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
*
* When attached to a vehicle, this script will kill the passengers when the vehicle dies.
* This includes the driver, gunner and all other passengers as well.
* The passengers can stay visible or become invisible when the vehicle blows up.
*
* \param PassengersVisible
*   The visibility of passengers when the vehicle dies. Set to 0 to make them invisible.
* \param RemoveScriptsFromPassengers
*   Should all scripts be removed from passengers or not. Can be useful e.g. if you don't want scripts to play death sounds.
*/
class MS_Vehicle_Death_Kill_Passengers : public ScriptImpClass
{
	bool PassengersVisible;
	bool RemoveScriptsFromPassengers;

	void Created(GameObject* obj) override
	{
		PassengersVisible = true;
		if (!obj->As_VehicleGameObj())
		{
			Console_Output("[%d:%hs:%hs] Critical Error: This script is only compatible with vehicle game objects. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name());
			Destroy_Script();
			return;
		}
		PassengersVisible = Get_Int_Parameter("PassengersVisible") == 1;
		RemoveScriptsFromPassengers = Get_Int_Parameter("RemoveScriptsFromPassengers") == 1;
	}

	void Killed(GameObject *obj, GameObject *killer) override
	{
		VehicleGameObj *vObj = obj->As_VehicleGameObj();
		// Kill occupants
		for (int i = 0; i < vObj->Get_Occupant_Count(); i++)
		{
			SoldierGameObj *passenger = vObj->Get_Occupant(i);
			if (passenger)
			{
				if (!PassengersVisible)
					Commands->Set_Model(passenger, "S_A_HUMAN");
				if (RemoveScriptsFromPassengers)
					Remove_All_Scripts(passenger);

				Toggle_Fly_Mode(passenger);
				Commands->Apply_Damage(passenger, 99999, "Death", killer);
			}
		}
	}
};
REGISTER_SCRIPT(MS_Vehicle_Death_Kill_Passengers, "PassengersVisible=1:int,RemoveScriptsFromPassengers=0:int");



/*!
* \brief Send custom when powerup is collected by a member of set team
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
*
* When attached to a powerup object, this script can send a custom if collected by a
* member of the team specified in the Team parameter.
* Credits go to Jerad2142 for the base of the script.
*
* \param ID
*   The ID of the object to send the custom message to.
*   0 sends to self.
*   -1 sends back to the sender.
* \param Custom
*   The custom message that will be sent to the object defined in the ID.
* \param Param
*   The parameter of the custom message that will be sent to the object defined in the ID.
* \param Delay
*   How many seconds to wait before the custom is sent to the object defined in the ID.
* \param Team
*   Which team should the custom sending trigger on. This is the team of the powerup collector.
*/
class MS_Powerup_Send_Custom_Team : public ScriptImpClass
{
	void Custom(GameObject *obj, int type, int param, GameObject *sender) override
	{
		if (Get_Object_Type(sender) != Get_Int_Parameter("Team"))
			return;

		if (type == CUSTOM_EVENT_POWERUP_GRANTED)
		{
			int id = Get_Int_Parameter("ID");
			GameObject *receiver = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
			if (receiver)
				Commands->Send_Custom_Event(obj, receiver, Get_Int_Parameter("Custom"), Get_Int_Parameter("Param"), Get_Float_Parameter("Delay"));
		}
	}
};
REGISTER_SCRIPT(MS_Powerup_Send_Custom_Team, "ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,Team=0:int");



/*------------------------
Building Scripts
--------------------------*/


/*!
* \brief BuildingScripts Weakpoint Objective Marker
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
*
* Attach this script to minor weakpoints as an extension script to create different
* objective marker objects depending on the building's state. 
* The objects are attached to a bone named "objective".
* All objects are deleted when the building is destroyed.
*
* \param OnlinePreset
*   The preset to attach when the building is online.
* \param OfflinePreset
*   The preset to attach when the building is offline.
*/
/*class MS_BuildingScripts_Weakpoint_ObjMarker : public ScriptImpClass
{
	int ObjectID = 0;

	void Created(GameObject* obj) override
	{
		Create_Objective_Marker(obj, Get_Parameter("OnlinePreset"));
	}

	void Custom(GameObject *obj, int type, int param, GameObject *sender) override
	{
		if (type == CUSTOM_BUILDINGSCRIPTS_BUILDINGONLINE)
		{
			Create_Objective_Marker(obj, Get_Parameter("OnlinePreset"));
		}
		else if (type == CUSTOM_BUILDINGSCRIPTS_BUILDINGOFFLINE)
		{
			Create_Objective_Marker(obj, Get_Parameter("OfflinePreset"));
		}
		else if (type == CUSTOM_BUILDINGSCRIPTS_BUILDINGDESTROYED)
		{
			Create_Objective_Marker(obj, nullptr);
		}
	}

	void Create_Objective_Marker(GameObject *obj, const char *presetName)
	{
		// Destroy previous object if exists
		if (ObjectID)
		{
			Commands->Destroy_Object(Commands->Find_Object(ObjectID));
			ObjectID = 0;
		}

		if (presetName == nullptr || !Is_Valid_Preset(presetName))
			return;

		GameObject *object = Commands->Create_Object(presetName, Commands->Get_Position(obj));
		Commands->Set_Facing(object, Commands->Get_Facing(obj));
		Commands->Attach_To_Object_Bone(object, obj, "objective");
		ObjectID = Commands->Get_ID(object);
	}
};
REGISTER_SCRIPT(MS_BuildingScripts_Weakpoint_ObjMarker, "OnlinePreset=null:string,OfflinePreset=null:string");*/



/*------------------------
Anti Refill Script
--------------------------*/


/*!
* \brief Disable refilling after damage
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
*
* Place this script on infantry presets that should not be able to refill for a few
* seconds after taking damage.
* Setting the time to 0 means no refill until the script is removed somehow.
*
* \param NoRefillTime
*   For how many seconds should the soldier not be able to refill after taking damage.
*/
/*class MS_Damaged_No_Refill : public ScriptImpClass
{
	int time = 0;
	bool originalCanRefill = true;
	bool checktimerRunning = false;
	int lastAttackerID = 0;

	void Created(GameObject* obj) override
	{
		if (!obj->As_SoldierGameObj())
		{
			Commands->Console_Output(L"[%d:%hs:%hs] Critical Error: Only objects of type SoldierGameObj can use the MS_Damaged_No_Refill script. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name());
			Destroy_Script();
			return;
		}

		time = Get_Int_Parameter("NoRefillTime");
		originalCanRefill = obj->As_SoldierGameObj()->Can_Refill();
	}

	void Damaged(GameObject *obj, GameObject *damager, float amount) override
	{
		// Only accept positive damage from soldiers
		if (amount <= 0 || !damager || !damager->As_SoldierGameObj() || Get_Vehicle(damager))
		{
			return;
		}

		lastAttackerID = damager->Get_ID();
		if (!checktimerRunning)
		{
			checktimerRunning = true;
			Commands->Start_Timer(obj, this, 1.0f, 2);
		}

		// Restart current timer if damage arrives during countdown
		if (Commands->Has_Timer(obj, this, 1))
		{
			Commands->Stop_Timer2(obj, this, 1);
		}
		else
		{
			obj->As_SoldierGameObj()->Set_Can_Refill(false);
		}

		if (time != 0)
		{
			Commands->Start_Timer(obj, this, (float)time, 1);
		}
	}

	void Timer_Expired(GameObject* obj, int number) override
	{
		if (number == 1 || !Commands->Find_Object(lastAttackerID))
		{
			obj->As_SoldierGameObj()->Set_Can_Refill(originalCanRefill);
			checktimerRunning = false;
		}
		else
		{
			Commands->Start_Timer(obj, this, 1.0f, 2);
		}
	}

	void Detach(GameObject* obj)
	{
		if (obj->As_SoldierGameObj())
			obj->As_SoldierGameObj()->Set_Can_Refill(originalCanRefill);

		ScriptImpClass::Detach(obj);
	}
};
REGISTER_SCRIPT(MS_Damaged_No_Refill,
	"NoRefillTime=5:int"
);*/


/*------------------------
Target Painter Scripts
--------------------------*/


/*!
* \brief Target Painter Host
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
*
* Place this script on objects that can "place" target markers on anything on keypress.
*
* \param None
*   None
*/
/*class MS_Target_Painter_Host : public JFW_Key_Hook_Base
{
	int targetID = 0;
	int playerID = 0;
	bool canPaint = true;

	void Created(GameObject *obj) override
	{
		if (!obj->As_SmartGameObj()->Is_Human_Controlled())
		{
			Destroy_Script();
			return;
		}

		playerID = Get_Player_ID(obj);
		InstallHook("MarkTarget", obj);

		// Make sure radio bot commands are initiated
		MS_AccessHelper::Init_AI_Global_Controller();
	}

	void KeyHook()
	{
		if (canPaint)
		{
			// This makes sure the painter cannot spam markers in the 1.5 second before the created object reports back
			canPaint = false;
			Commands->Start_Timer(Owner(), this, 4.0f, 0);
		}

		// Do not spam markers, also makes sure the painter object only exists once
		if (auto targetObj = Commands->Find_Object(targetID))
		{
			Commands->Set_Model(targetObj, "NULL");
			Commands->Send_Custom_Event(Owner(), targetObj, CUSTOM_TARGETPAINTER_TARGET_OBJECT_DESTROY_SELF, 0, 3.0f);
		}

		SmartGameObj *muzzle_obj = Owner()->As_SmartGameObj();
		auto createPos = muzzle_obj->As_ArmedGameObj()->Get_Muzzle();

		Commands->Fire_C4("Experimental_Ammo_Target_Marker", Owner()->As_SoldierGameObj(), 1, createPos);
	}

	void Timer_Expired(GameObject* obj, int number) override
	{
		canPaint = true;
	}

	void Custom(GameObject *obj, int type, int param, GameObject *sender) override
	{
		if (type == CUSTOM_TARGETPAINTER_TARGET_OBJECT_CREATED)
		{
			targetID = sender->Get_ID();

			int radio_index = Radio_Message_Type::TAKE_THE_POINT;
			int custom = CUSTOM_AI_ORDER_MOVE;
			int orderObjID = sender->Get_ID();
			int team = Get_Object_Type(obj);

			GameObject *target = Commands->Find_Object(param);
			if (target)
			{

				int targetTeam = Get_Object_Type(target);


				custom = targetTeam == team ? CUSTOM_AI_ORDER_DEFEND : CUSTOM_AI_ORDER_ATTACK;
				radio_index = targetTeam == team ? Radio_Message_Type::DEFEND_THAT_STRUCTURE : Radio_Message_Type::ATTACK_THAT_STRUCTURE;

				// Vehicle
				if (auto targetVeh = target->As_VehicleGameObj())
				{
					orderObjID = targetVeh->Get_ID();
					// Not turret
					if (!targetVeh->Is_Turret())
					{
						// TODO: what if it's friendly?!
						radio_index = Radio_Message_Type::DESTROY_THAT_VEHICLE;

						if (Is_Harvester_Preset(targetVeh))
							radio_index = targetTeam == team ? Radio_Message_Type::DEFEND_THE_HARVESTER : Radio_Message_Type::ATTACK_THE_HARVESTER;
					}
				}
				else if (target->As_SoldierGameObj())
				{
					radio_index = Radio_Message_Type::DESTROY_IT_NOW;
					orderObjID = target->Get_ID();
				}
			}

			char params[256];
			snprintf(params, sizeof(params), "%d,%d,%d,%d,%d,%d,%d", team, orderObjID, 10, 1000, 1000, 1000, 1000);
			Commands->Attach_Script((target ? target : sender), "dp88_AI_Objective", params);

			int stringID = Commands->Get_Radio_Command_String(radio_index, team);
			Commands->Send_Client_Announcement(team, playerID, stringID, ANNOUNCE_TEAM, radio_index, true, true, true);

			// Work with bots
			MS_AccessHelper::Receive_Radio_Command(obj, stringID, custom, orderObjID);
		}
	}

};
REGISTER_SCRIPT(MS_Target_Painter_Host, "");*/


/*!
* \brief Target Painted Object
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
*
* Do not place this on anything manually!
*
* \param None
*   None
*/
/*class MS_Target_Painted_Object : public ScriptImpClass
{
	void Created(GameObject *obj) override
	{
		Commands->Start_Timer(obj, this, 1.5f, 0);
	}

	void Custom(GameObject *obj, int type, int param, GameObject *sender) override
	{
		if (type == CUSTOM_TARGETPAINTER_TARGET_OBJECT_DESTROY_SELF)
			Commands->Destroy_Object(obj);

	}

	void Timer_Expired(GameObject* obj, int number) override
	{
		if (auto owner = Get_C4_Planter(obj))
		{
			Commands->Send_Custom_Event(obj, owner, CUSTOM_TARGETPAINTER_TARGET_OBJECT_CREATED, Commands->Get_ID(Get_C4_Attached(obj)), 0.0f);
		}
		else
			Commands->Destroy_Object(obj);
	}
};
REGISTER_SCRIPT(MS_Target_Painted_Object, "");*/

/*!
* \brief A jukebox script that can play a music list, shuffling it.
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
*
* Attach this script to a Sound Obj preset (inside Object in the preset list).
* Then set up its parameters to have a shuffling music player on your map.
*
* \param MusicTwiddler
*   The preset name of a sound twiddler that contains non-repeating music tracks (Loop Count must be 1, Is 3D Sound OFF).
* \param NextMusicAnnouncePrefix
*   If set, this text will be displayed in front of the music preset name (E.g. *Now playing:* -> "Now playing: Hell March")
* \param Red/Blue/Green
*   The color of the message set in NextMusicAnnouncePrefix.
* \param ScoreScreenMusicPreset
*   The preset name of the music that will be played when the game ends and the score screen is shown.
*/
/*class MS_Jukebox : public ScriptImpClass
{
	int currentMusic = 0;
	int prevMusic = 0;
	bool firstTime = true;
	static MS_Jukebox *current;
	static const char *scoreMusic;
	DynamicVectorClass<int> fullMusicList;

	void Created(GameObject *obj) override
	{
	    if (!Is_Valid_Preset(Get_Parameter("MusicTwiddler")))
		{
			Commands->Console_Output(L"[%d:%hs:%hs] Critical Error: '%hs' is not a valid preset. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Parameter("MusicTwiddler"));
			Destroy_Script();
			return;
		}

		auto fullList = ((TwiddlerClass*)DefinitionMgrClass::Find_Named_Definition(Get_Parameter("MusicTwiddler"), false))->Get_Definition_List();
		for (int i = 0; i < fullList.Count(); i++)
		{
			int musicID = DefinitionMgrClass::Find_Definition(fullList[i], false)->Get_ID();
			fullMusicList.Add(musicID);
		}

		// Handle empty list case
		if (fullMusicList.Count() > 0)
		{
			Commands->Start_Timer(obj, this, Get_Float_Parameter("Delay"), 1);
		}
		current = this;
		scoreMusic = Get_Parameter("ScoreScreenMusicPreset");
		Commands->AddGameOverHook(GameOverHook);
	}

	void Custom(GameObject *obj, int type, int param, GameObject *sender) override
	{
		if (type == CUSTOM_EVENT_SOUND_ENDED)
		{
			if (!Get_Int_Parameter("NoLoop"))
			{
				PlayNext();
			}
		}
	}

	void PlayNext()
	{
		// Remove current and previous music from list
		DynamicVectorClass<int> musicList;
		for (int i = 0; i < fullMusicList.Count(); i++)
		{
			if (fullMusicList.Count() == 1 ||
				((fullMusicList.Count() == 2 || fullMusicList.Count() == 3) && fullMusicList[i] != currentMusic) ||
				(fullMusicList.Count() > 3 && fullMusicList[i] != currentMusic && fullMusicList[i] != prevMusic))
			{
				musicList.Add(fullMusicList[i]);
			}
		}

		prevMusic = currentMusic;
		if (!firstTime || !Get_Int_Parameter("FirstSong"))
		{
			currentMusic = musicList[FreeRandom.Get_Int(musicList.Count())];
		}
		else
		{
			firstTime = false;
			currentMusic = musicList[0];
		}
		SoundGameObj *s = Owner()->As_SoundGameObj();
		if (s)
		{
			s->Set_Sound(currentMusic);

			// Display text if prefix is set
			if (strlen(Get_Parameter("NextMusicAnnouncePrefix")) != 0)
			{
				DynamicVectorClass<WideStringClass> parameters;
				parameters.Add(Get_Parameter("NextMusicAnnouncePrefix"));
				parameters.Add(Get_Definition_Name(currentMusic));
				Send_Message(Get_Int_Parameter("Red"), Get_Int_Parameter("Green"), Get_Int_Parameter("Blue"), "%s %s", parameters);
			}
		}
	}

	void Detach(GameObject* obj) override
	{
		ScriptImpClass::Detach(obj);
		current = nullptr;
	}

	void Timer_Expired(GameObject* obj, int number) override
	{
		PlayNext();
	}

	static void GameOverHook()
	{
		if (current)
		{
			SoundGameObj *s = current->Owner()->As_SoundGameObj();
			if (s)
			{
				s->Set_Sound(0);
				if (strcmp(scoreMusic, "") != 0)
				{
					Commands->Create_2D_Sound(scoreMusic);
				}
			}
		}
	}
};
MS_Jukebox *MS_Jukebox::current = nullptr;
const char *MS_Jukebox::scoreMusic = "";
REGISTER_SCRIPT(MS_Jukebox, "MusicTwiddler:string,NextMusicAnnouncePrefix:string,Red=255:int,Blue=255:int,Green=255:int,ScoreScreenMusicPreset:string,NoLoop:int,Delay:float,FirstSong:int");*/


/*------------------------
Damage by poke scripts
--------------------------*/


/*!
* \brief Damage or heal MS_Damage_By_Poke_Target targets by poking
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
*
* Place this script on soldier objects to instantly damage/heal objects
* that have MS_Damage_By_Poke_Target attached.
*
* \param Cooldown
*   How many seconds must pass between the poking damage/heal ability usages
* \param Warhead
*   What warhead to use for damaging/healing
* \param Damage
*   How much damage to do to the poked object
*/
/*class MS_Damage_By_Poke_Poker : public ScriptImpClass
{
	bool canRepair = true;

	void Timer_Expired(GameObject* obj, int number) override
	{
		if (number == 0)
			canRepair = true;
	}

public:
	bool Poke()
	{
		if (canRepair)
		{
			if (Get_Int_Parameter("Cooldown") > 0)
			{
				canRepair = false;
				Commands->Start_Timer(Owner(), this, (float)Get_Int_Parameter("Cooldown"), 0);
			}
			return true;
		}
		else
		{
			StringClass str = "IDS_ABILITYCOOLDOWN";
			Send_Message_Player(Owner(), DP88_RGB_GENERAL_MSG, str);
			return false;
		}
	}

	const char *Get_Warhead()
	{
		return Get_Parameter("Warhead");
	}

	int Get_Damage()
	{
		return Get_Int_Parameter("Damage");
	}
};
REGISTER_SCRIPT(MS_Damage_By_Poke_Poker, "Cooldown=60:int,Warhead=None:string,Damage=5000:int");*/


/*!
* \brief Get damaged or healed if poked by MS_Damage_By_Poke_Poker
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
*
* Place this script on objects that can be instantly damaged/healed by a 
* soldier object with MS_Damage_By_Poke_Poker attached to it.
*
* \param None
*   None
*/
/*class MS_Damage_By_Poke_Target : public ScriptImpClass
{
	void Poked(GameObject *obj, GameObject *poker) override
	{
		if (auto script = (MS_Damage_By_Poke_Poker *)(Find_Script_On_Object(poker, "MS_Damage_By_Poke_Poker")))
		{
			if (script->Poke())
			{
				// Instant damage/repair!
				Commands->Apply_Damage(obj, (float)script->Get_Damage(), script->Get_Warhead(), poker);
			}
		}
	}
};
REGISTER_SCRIPT(MS_Damage_By_Poke_Target, "");*/


/*!
* \brief Remove key on character change
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
*
* This script removes a key from the soldier it's attached to when the player changes character.
*
* \param Key
*   The key code to remove.
*/
/*class MS_Destroyed_Remove_Key : public ScriptImpClass
{
	void Destroyed(GameObject *obj) override
	{
		Commands->Grant_Key(obj, Get_Int_Parameter("Key"), false);
	}
};
REGISTER_SCRIPT(MS_Destroyed_Remove_Key, "Key=0:int");*/
