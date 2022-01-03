#include "general.h"
#include "ms.h"
#include "PurchaseSettingsDefClass.h"
#include "VehicleGameObj.h"
#include "BeaconGameObj.h"
#include "BeaconGameObjDef.h"
#include "C4GameObj.h"
#include "DefinitionMgrClass.h"
#include "GameObjManager.h"
#include "MoveablePhysClass.h"
#include "objectlibrarymanager.h"
#include "PowerUpGameObjDef.h"
#include "Random2Class.h"
#include "RefineryGameObj.h"
#include "SoldierGameObjDef.h"
#include "TwiddlerClass.h"
#include "WeaponClass.h"
#include "WeaponBagClass.h"
#include "dp88_ar.h"
#include "dp88_custom_timer_defines.h"
#include "engine_game.h"
#include "engine_script.h"
#include "engine_obj.h"
#include "engine_obj2.h"
#include "engine_player.h"
#include "engine_def.h"
#include "engine_tdb.h"
#include "engine_pt.h"
#include "engine_tt.h"
#include <unordered_map>
//#include "cgamedata.h"
//#include "iniclass.h"
#include "cPlayer.h"
#define REGISTER_SCRIPT(name, params) ScriptRegistrant<name> name##Registrant(#name, params);


int Radio_Message_Type::BUILDING_NEEDS_REPAIRS;
int Radio_Message_Type::GET_IN_THE_VEHICLE;
int Radio_Message_Type::GET_OUT_OF_THE_VEHICLE;
int Radio_Message_Type::DESTROY_THAT_VEHICLE;
int Radio_Message_Type::DONT_GET_IN_MY_WAY;
int Radio_Message_Type::AFFIRMATIVE;
int Radio_Message_Type::NEGATIVE;

int Radio_Message_Type::TAKE_THE_POINT;
int Radio_Message_Type::MOVE_OUT;
int Radio_Message_Type::FOLLOW_ME;
int Radio_Message_Type::COVER_ME;
int Radio_Message_Type::DESTROY_IT_NOW;

int Radio_Message_Type::ATTACK_THE_HARVESTER;
int Radio_Message_Type::ATTACK_THAT_STRUCTURE;
int Radio_Message_Type::DEFEND_THE_BASE;
int Radio_Message_Type::DEFEND_THE_HARVESTER;
int Radio_Message_Type::DEFEND_THAT_STRUCTURE;



/*------------------------
Branch differences to eliminate false diff positives
--------------------------*/


inline void M_Console_Input(const char *Input)
{
	Console_Input(Input);
}

// -------------------------------------------------------------------------------------------------

inline void M_Console_Output(const char *Output)
{
	Console_Output(Output);
}

// -------------------------------------------------------------------------------------------------

inline bool M_Has_Timer(GameObject* obj, ScriptClass* script, int number)
{
	return Has_Timer(obj, script, number);
}

// -------------------------------------------------------------------------------------------------

inline void M_Stop_Timer2(GameObject* obj, ScriptClass* script, int number)
{
	Stop_Timer2(obj, script, number);
}

// -------------------------------------------------------------------------------------------------

inline bool M_Test_Raycast_Collision(const Vector3 &pos1, const Vector3 &pos2, const bool checkDynamicObjects, Vector3 *contactPoint = nullptr, GameObject *compareObj = nullptr)
{
	Vector3 newPoint;
	if (!contactPoint)
		contactPoint = &newPoint;

	return Test_Raycast_Collision(pos1, pos2, checkDynamicObjects, contactPoint, compareObj);
}


/*------------------------
Non script-specific functions
--------------------------*/

static bool IsFirstDebugMessageOnMapSent = false;
static bool DebugPathfindFailMessagesLiveEnabled = false;
static bool DebugPathfindFailMessagesFileEnabled = false;

void Print_Debug(const StringClass& text)
{
	if (DebugPathfindFailMessagesFileEnabled) {
		if (!IsFirstDebugMessageOnMapSent)
		{
			Write_File_Async("_debug_bots.txt", The_Game()->Get_Map_Name().Peek_Buffer());
			Write_File_Async("_debug_bots.txt", "\n");
		}
		Write_File_Async("_debug_bots.txt", text.Peek_Buffer());
		Write_File_Async("_debug_bots.txt", "\n");
	}

	if (DebugPathfindFailMessagesLiveEnabled) {
		M_Console_Input("msg " + text);
	}

	if (!IsFirstDebugMessageOnMapSent)
		IsFirstDebugMessageOnMapSent = true;
}

// -------------------------------------------------------------------------------------------------

bool Can_Move_To(GameObject *obj, Vector3 position)
{
	MoveablePhysClass* phys = obj->As_PhysicalGameObj() ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : nullptr;
	if (phys && phys->Can_Teleport(Matrix3D(position)))
		return true;
	return false;
}

// -------------------------------------------------------------------------------------------------

// param cos_cone_behind - 0 is everything behind the middle line. Set this higher (in rad) to restrict "behind" more.
bool Is_Behind_Cone(const Matrix3D &transform, const Vector3 &target_location, float cos_cone_behind = 0)
{
	auto to_target = Normalize(target_location - transform.Get_Translation());
	float dot = transform.Get_X_Vector() * to_target;
	return dot < -cos_cone_behind;
}

// -------------------------------------------------------------------------------------------------

float Calculate_Relative_Facing_Diff(GameObject *obj, Vector3 targetLocation)
{
	Matrix3D m1 = obj->As_SmartGameObj()->Get_Transform();
	Matrix3D currentTM = Matrix3D::Identity;
	currentTM.Rotate_Z(m1.Get_Z_Rotation());
	currentTM.Set_Translation(m1.Get_Translation());

	Vector3 out;
	Matrix3D::Inverse_Transform_Vector(currentTM, targetLocation, &out);
	float x = out.X;
	float y = out.Y;
	float f = atan2(y, x);
	float result = WWMath::Wrap(f, 0, 6.2831855f);
	result = WWMath::Wrap(result, -3.1415927f, 3.1415927f);

	return result;
}

// -------------------------------------------------------------------------------------------------

bool Get_Random_Pathfind_Spot_Around_Position(GameObject *obj, const Vector3 &centerPos, float minDist, float maxDist, Vector3 *returnPos, GameObject *target = nullptr)
{
	// Default to centerpos if all else fails
	*returnPos = centerPos;

	Vector3 pathfindPos;
	// Check if position is in a sector
	if (!Get_Random_Pathfind_Spot(centerPos, maxDist, &pathfindPos))
	{
		// Try to get the closest sector mid
		if (!Get_Closest_Pathfind_Spot(centerPos, 15.0f, &pathfindPos))
			return false;
	}
	else
		pathfindPos = centerPos;

	int count = 0;
	while (count < 50)
	{
		bool success = Get_Random_Pathfind_Spot(pathfindPos, maxDist, returnPos);
		if (!success)
			return false;

		float dist_sq = Vector3::Distance_Squared(pathfindPos, *returnPos);
		if (dist_sq >= minDist * minDist)
		{
			if (obj->As_VehicleGameObj())
				(*returnPos).Z += 1.5f;

			if (Can_Move_To(obj, *returnPos) && (!target || !M_Test_Raycast_Collision(Commands->Get_Position(obj) + Vector3(0, 0, 1.5f), Commands->Get_Position(target) + Vector3(0, 0, 1.5f), false)))
			{
				if (obj->As_VehicleGameObj())
					(*returnPos).Z -= 1.5f;

				return true;
			}
		}
		count++;
	}
	return false;
}

// -------------------------------------------------------------------------------------------------

bool Get_Random_Pathfind_Spot_Around_Self(GameObject *obj, float minDist, float maxDist, Vector3 *returnPosition, GameObject* target = nullptr)
{
	Vector3 pos = Commands->Get_Position(obj);
	return Get_Random_Pathfind_Spot_Around_Position(obj, pos, minDist, maxDist, returnPosition, target);
}

// -------------------------------------------------------------------------------------------------

template <class T>
DynamicVectorClass<T> Combine_Lists(const DynamicVectorClass<T>& list1, const DynamicVectorClass<T>& list2)
{
	DynamicVectorClass<T> result;
	result.Grow(list1.Count() + list2.Count());
	result.Add_Multiple(list1);
	result.Add_Multiple(list2);
	return result;
}
template <class T>
SimpleDynVecClass<T> Combine_Lists(const SimpleDynVecClass<T>& list1, const SimpleDynVecClass<T>& list2)
{
	SimpleDynVecClass<T> result;
	result.Grow(list1.Count() + list2.Count());
	result.Add_Multiple(list1);
	result.Add_Multiple(list2);
	return result;
}

// -------------------------------------------------------------------------------------------------

DynamicVectorClass<Vector3> Order_Points_By_Distance(const DynamicVectorClass<Vector3> &inArray, const Vector3 &compareToPoint, float maxDistance)
{
	DynamicVectorClass<Vector3> outArray;
	float maxDistance_sq = maxDistance * maxDistance;

	for (int i = 0; i < inArray.Count(); i++)
	{
		const Vector3 &newPoint = inArray[i];
		const float newDist_sq = Vector3::Distance_Squared(compareToPoint, newPoint);

		// Skip out of range points
		if (newDist_sq > maxDistance_sq)
			continue;

		// First element can just go in
		if (!outArray.Count())
		{
			outArray.Add(newPoint);
			continue;
		}

		// Search where to insert new point
		for (int j = 0; j < outArray.Count(); j++)
		{
			// Visited point is bigger: insert before it (works with any 1+ sized array)
			if (newDist_sq < Vector3::Distance_Squared(compareToPoint, outArray[j]))
			{
				outArray.Insert(j, newPoint);
				break;
			}
			// No bigger points found, reached the end of the array: add to end of array
			else if (j == outArray.Count() - 1)
			{
				outArray.Add(newPoint);
				break;
			}
		}
	}
	return outArray;
}

// -------------------------------------------------------------------------------------------------

void Remove_Stock_AI_Behaviour(GameObject *obj)
{
	if (auto soldier = obj->As_SoldierGameObj())
	{
		GameObjObserverClass *InnateObserver = obj->As_SoldierGameObj()->Find_Observer("Innate Soldier");
		if(InnateObserver)
		{
			soldier->Remove_Observer(InnateObserver);
			soldier->Set_Innate_Observer(NULL);
		}
	}
}

// -------------------------------------------------------------------------------------------------

/* Damage of one shot from this weapon against the target, including any explosion component, scaled against target's skin/shield, average between the two */
float Calculate_Average_Scaled_Damage(WeaponClass* weapon, GameObject* target)
{
	ArmorType skin = target->As_DamageableGameObj()->Get_Defense_Object()->Get_Skin();
	ArmorType shield = target->As_DamageableGameObj()->Get_Defense_Object()->Get_Shield_Type();

	WarheadType awarhead = weapon->PrimaryAmmoDefinition->Warhead();
	float adamage = weapon->PrimaryAmmoDefinition->Damage();
	float askinMultiplier = ArmorWarheadManager::Get_Damage_Multiplier(skin, awarhead);
	float ashieldMultiplier = ArmorWarheadManager::Get_Damage_Multiplier(shield, awarhead);

	float scaledDamage = (askinMultiplier * adamage + ashieldMultiplier * adamage) / 2;

	// Add explosion component damage
	if (auto expDef = (ExplosionDefinitionClass*)Find_Definition(weapon->PrimaryAmmoDefinition->ExplosionDefID))
	{
		WarheadType ewarhead = expDef->Warhead;
		float edamage = expDef->DamageStrength;
		float eskinMultiplier = ArmorWarheadManager::Get_Damage_Multiplier(skin, ewarhead);
		float eshieldMultiplier = ArmorWarheadManager::Get_Damage_Multiplier(shield, ewarhead);
		scaledDamage += (eskinMultiplier * edamage + eshieldMultiplier * edamage) / 2;
	}

	// Add spraycount
	scaledDamage *= weapon->PrimaryAmmoDefinition->SprayCount();

	return scaledDamage;
}

// -------------------------------------------------------------------------------------------------

/* Calculate the raw Damage Per Second of this weapon. The passed damage parameter can be the scaled damage against a particular target for a more specific DPS calculation. */
float Calculate_Weapon_DPS(WeaponClass* weapon, float damage)
{
	int clipSize = weapon->Get_Definition()->ClipSize();
	// If clip size is infinite (-1), don't consider reload times into calculation
	return (clipSize * damage) / ((clipSize / weapon->PrimaryAmmoDefinition->RateOfFire) + (clipSize != -1 ? weapon->Get_Definition()->ReloadTime() + weapon->PrimaryAmmoDefinition->ChargeTime() : 0));
}

// -------------------------------------------------------------------------------------------------

int Create_Timed_Objective(int team, float seconds, Vector3 position, int range, int type = 0, int attachObjID = 0)
{
	auto timedObjective = Commands->Create_Object("Invisible_Object", position);
	int id = timedObjective->Get_ID();

	int objectiveType = type == 0 ? id : type;
	StringClass params;
	params.Format("%d,%d,%d,%d,%d,%d,%d", team, objectiveType, range, 1000, 1000, 1000, 1000);
	Commands->Attach_Script(timedObjective, "dp88_AI_Objective", params.Peek_Buffer());

	// 0 seconds is forever
	if (seconds > 0)
	{
		params.Format("%.2f,0", seconds);
		Commands->Attach_Script(timedObjective, "JFW_Destroy_Self_Timer", params.Peek_Buffer());
	}

	if (auto hostObj = Commands->Find_Object(attachObjID))
	{
		Attach_Script_V(hostObj, "JFW_Death_Destroy_Object", "%d", id);
		Commands->Attach_To_Object_Bone(timedObjective, hostObj, "ROOTTRANSFORM");
	}

	return id;
}

// -------------------------------------------------------------------------------------------------

class BotPersonality : public RefCountClass
{
	float spawnTimeMin, spawnTimeMax;
	float radioTimeMin, radioTimeMax;
	float thinkTimeMin, thinkTimeMax;
	float attackErrorMultiplyMin, attackErrorMultiplyMax;
	float buyinfChance;
	float buyvehChance;
	float teamworkChance;
	float sprintChance;
	float jumpChanceRegular;
	float jumpChanceCombat;
	float crouchChanceCalm;

	// Skill-independent personality traits
	float investigateChance;
	float chatChance;
	float squishEvadeChance;
	float squishEvadeDistMin, squishEvadeDistMax;
	float sympathyVarMin, sympathyVarMax;

public:
	int personalityType;

	enum INTERVAL {
		SPAWN_TIME,
		RADIO_TIME,
		THINK_TIME,
		ATTACK_ERROR_MULTIPLIER,
		SYMPATHY_VARIABLE,
		BUYINF_CHANCE,
		BUYVEH_CHANCE,
		TEAMWORK_CHANCE,
		SPRINT_CHANCE,
		JUMP_CHANCE_REGULAR,
		JUMP_CHANCE_COMBAT,
		CROUCH_CHANCE_CALM,
		INVESTIGATE_CHANCE,
		CHAT_CHANCE,
		SQUISH_EVADE_CHANCE,
		SQUISH_EVADE_DISTANCE,
	};

	float Get_Random(BotPersonality::INTERVAL intervalType)
	{
		switch (intervalType)
		{
		case SPAWN_TIME:
			return FreeRandom.Get_Float(spawnTimeMin, spawnTimeMax);
		case RADIO_TIME:
			return FreeRandom.Get_Float(radioTimeMin, radioTimeMax);
		case THINK_TIME:
			return FreeRandom.Get_Float(thinkTimeMin, thinkTimeMax);
		case ATTACK_ERROR_MULTIPLIER:
			return FreeRandom.Get_Float(attackErrorMultiplyMin, attackErrorMultiplyMax) + 1;
		case SYMPATHY_VARIABLE:
			return FreeRandom.Get_Float(sympathyVarMin, sympathyVarMax);
		case BUYINF_CHANCE:
			return FreeRandom.Get_Float() < buyinfChance ? 1.0f : 0.0f;
		case BUYVEH_CHANCE:
			return FreeRandom.Get_Float() < buyvehChance ? 1.0f : 0.0f;
		case TEAMWORK_CHANCE:
			return FreeRandom.Get_Float() < teamworkChance ? 1.0f : 0.0f;
		case SPRINT_CHANCE:
			return FreeRandom.Get_Float() < sprintChance ? 1.0f : 0.0f;
		case JUMP_CHANCE_REGULAR:
			return FreeRandom.Get_Float() < jumpChanceRegular ? 1.0f : 0.0f;
		case JUMP_CHANCE_COMBAT:
			return FreeRandom.Get_Float() < jumpChanceCombat ? 1.0f : 0.0f;
		case CROUCH_CHANCE_CALM:
			return FreeRandom.Get_Float() < crouchChanceCalm ? 1.0f : 0.0f;
		case INVESTIGATE_CHANCE:
			return FreeRandom.Get_Float() < investigateChance ? 1.0f : 0.0f;
		case CHAT_CHANCE:
			return FreeRandom.Get_Float() < chatChance ? 1.0f : 0.0f;
		case SQUISH_EVADE_CHANCE:
			return FreeRandom.Get_Float() < squishEvadeChance ? 1.0f : 0.0f;
		case SQUISH_EVADE_DISTANCE:
			return FreeRandom.Get_Float(squishEvadeDistMin, squishEvadeDistMax);
		}
		return 0.0f;
	}

	// -------------------------------------------------------------------------------------------------

	bool Wants_To_Repair(GameObject *obj, float distance)
	{
		float currentHitp = Commands->Get_Health(obj) + Commands->Get_Shield_Strength(obj);
		float maxHitp = Commands->Get_Max_Health(obj) + Commands->Get_Max_Shield_Strength(obj);

		float ratio = 1 - (currentHitp / maxHitp);

		float chance = (ratio*ratio) * 4;

		float correctedChance = chance - (distance*distance / 40000);

		/*StringClass buff;
		buff.Format("msg hp: %.2f/%.2f, ratio: %.2f, chance: %.2f, dist: %.2f, correctedChance: %.2f", currentHitp, maxHitp, ratio, chance, distance, correctedChance);
		M_Console_Input(buff.Peek_Buffer());*/

		if (currentHitp == maxHitp)
			return false;

		// TODO: personality randoms should affect this
		return correctedChance >= 1.0f;
	}

	BotPersonality(int type)
	{
		personalityType = type;
		// Temporary? template personalities
		switch (type)
		{
		// Newbie
		case 0:
			spawnTimeMin = 2.5f;
			spawnTimeMax = 7.5f;
			radioTimeMin = 2.0f;
			radioTimeMax = 3.5f;
			thinkTimeMin = FreeRandom.Get_Float(0.95f, 1.05f);
			thinkTimeMax = FreeRandom.Get_Float(1.15f, 1.25f);
			attackErrorMultiplyMin = +0.0f;
			attackErrorMultiplyMax = +0.4f;
			buyinfChance = FreeRandom.Get_Float(0.15f, 0.60f);
			buyvehChance = FreeRandom.Get_Float(0.20f, 0.65f);
			teamworkChance = FreeRandom.Get_Float(0.15f, 0.35f);
			sprintChance = FreeRandom.Get_Float(0.00f, 0.10f);
			jumpChanceRegular = FreeRandom.Get_Float(0.00f, 0.02f);
			jumpChanceCombat = FreeRandom.Get_Float(0.04f, 0.12f);
			crouchChanceCalm = FreeRandom.Get_Float(0.10f, 0.20f);
			squishEvadeChance = FreeRandom.Get_Float(0.50f, 0.75f);
			squishEvadeDistMin = 30.0f;
			squishEvadeDistMax = 45.0f;
			break;
		// Regular
		case 1:
			spawnTimeMin = 1.5f;
			spawnTimeMax = 5.5f;
			radioTimeMin = 1.5f;
			radioTimeMax = 3.0f;
			thinkTimeMin = FreeRandom.Get_Float(0.85f, 0.95f);
			thinkTimeMax = FreeRandom.Get_Float(1.05f, 1.15f);
			attackErrorMultiplyMin = -0.1f;
			attackErrorMultiplyMax = +0.2f;
			buyinfChance = FreeRandom.Get_Float(0.40f, 0.80f);
			buyvehChance = FreeRandom.Get_Float(0.30f, 0.75f);
			teamworkChance = FreeRandom.Get_Float(0.35f, 0.55f);
			sprintChance = FreeRandom.Get_Float(0.10f, 0.30f);
			jumpChanceRegular = FreeRandom.Get_Float(0.03f, 0.06f);
			jumpChanceCombat = FreeRandom.Get_Float(0.20f, 0.40f);
			crouchChanceCalm = FreeRandom.Get_Float(0.20f, 0.40f);
			squishEvadeChance = FreeRandom.Get_Float(0.80f, 0.90f);
			squishEvadeDistMin = 40.0f;
			squishEvadeDistMax = 50.0f;
			break;
		// Experienced
		case 2:
			spawnTimeMin = 0.5f;
			spawnTimeMax = 2.0f;
			radioTimeMin = 1.0f;
			radioTimeMax = 2.0f;
			thinkTimeMin = FreeRandom.Get_Float(0.75f, 0.85f);
			thinkTimeMax = FreeRandom.Get_Float(0.95f, 1.05f);
			attackErrorMultiplyMin = -0.4f;
			attackErrorMultiplyMax = -0.1f;
			buyinfChance = FreeRandom.Get_Float(0.25f, 0.70f);
			buyvehChance = FreeRandom.Get_Float(0.40f, 0.85f);
			teamworkChance = FreeRandom.Get_Float(0.55f, 0.75f);
			sprintChance = FreeRandom.Get_Float(0.60f, 0.90f);
			jumpChanceRegular = FreeRandom.Get_Float(0.08f, 0.16f);
			jumpChanceCombat = FreeRandom.Get_Float(0.60f, 0.80f);
			crouchChanceCalm = FreeRandom.Get_Float(0.70f, 1.00f);
			squishEvadeChance = FreeRandom.Get_Float(0.95f, 1.00f);
			squishEvadeDistMin = 50.0f;
			squishEvadeDistMax = 60.0f;
			break;
		}
		// Untied to fixed personalities
		investigateChance = FreeRandom.Get_Float(0.1f, 0.9f);
		chatChance = FreeRandom.Get_Float(0.04f, 0.09f);
		sympathyVarMin = FreeRandom.Get_Float(0.004f, 0.01f);
		sympathyVarMax = FreeRandom.Get_Float(0.011f, 0.018f);
	}
};

// -------------------------------------------------------------------------------------------------

class PlayerRelationData
{
	// Player sympathy stuff
	struct PlayerSympathy
	{
		float sympathy;
		PlayerSympathy(float sympathy) : sympathy(sympathy) {}
	};

	std::unordered_map<StringClass, PlayerSympathy, hash_string, equals_string> sympathyList;

public:

	PlayerSympathy* GetPlayerData(const StringClass &name)
	{
		if (name.Is_Empty() || strcmp(name.Peek_Buffer(), "None") == 0)
			return nullptr;

		auto it = sympathyList.find(name);
		if (it == sympathyList.end())
			it = sympathyList.insert(std::make_pair(name, PlayerSympathy(0.15f))).first;
	
		return &it->second;
	}
};

// -------------------------------------------------------------------------------------------------



/*------------------------
AI Helper / Marker Scripts
--------------------------*/


/*!
* \brief AI Bomber Aircraft marker
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* Place this script on aircraft presets that attack by ejecting bombs downwards from the sky.
*
* This is a logic marker script for presets that have MS_AI_Behaviour on them, or if they have
* the MS_AI_Preferences script attached and bots are created by one MS_AI_Player_Count_Controller
* or individual MS_AI_Player_Controller scripts.
*
* \param None
*   None
*/
class MS_AI_Marker_Bomber_Aircraft : public ScriptImpClass
{
	void Created(GameObject* obj) override
	{
		if (!obj->As_VehicleGameObj() || Get_Vehicle_Mode(obj) != VEHICLE_TYPE_FLYING)
		{
			StringClass msg;
			msg.Format("[%d:%hs:%hs] Critical Error: Only flying vehicles can use the MS_AI_Bomber_Aircraft script. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name());
			M_Console_Output(msg.Peek_Buffer());
			Destroy_Script();
			return;
		}
	}
};
REGISTER_SCRIPT(MS_AI_Marker_Bomber_Aircraft, "");


/*!
* \brief AI Deployable marker
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* Place this script on unit presets that can use a deploy script to change behaviour in combat.
*
* This is a logic marker script for presets that have MS_AI_Behaviour on them, or if they have
* the MS_AI_Preferences script attached and bots are created by one MS_AI_Player_Count_Controller
* or individual MS_AI_Player_Controller scripts.
*
* \param Deploy_Script_Name
*   This parameter should match the name of the deploy script.
* \param Deploy_VS_Infantry:
*   Should the unit use its deploy ability against infantry: 1 to deploy, 0 to not deploy.
* \param Deploy_VS_Light_Vehicle:
*   Should the unit use its deploy ability against light vehicles: 1 to deploy, 0 to not deploy.
* \param Deploy_VS_Heavy_Vehicle:
*   Should the unit use its deploy ability against heavy vehicles: 1 to deploy, 0 to not deploy.
* \param Deploy_VS_VTOL:
*   Should the unit use its deploy ability against flying/VTOL units: 1 to deploy, 0 to not deploy.
* \param Deploy_VS_Building:
*   Should the unit use its deploy ability against buildings: 1 to deploy, 0 to not deploy.
* \param Deploy_Is_Stationary:
*   Setting this parameter to 1 is telling the script this unit becomes stationary when deployed. 0 means it can move after deploy.
*/
class MS_AI_Marker_Deployable : public ScriptImpClass
{
	void Created(GameObject *obj) override
	{
		if (!Is_Script_Attached(obj, Get_Deploy_Script_Name()))
		{
			StringClass msg;
			msg.Format("[%d:%hs:%hs] Critical Error: The script '%hs' does not exist on this object. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Deploy_Script_Name());
			M_Console_Output(msg.Peek_Buffer());
			Destroy_Script();
			return;
		}
	}

public:
	const char *Get_Deploy_Script_Name() { return Get_Parameter("Deploy_Script_Name"); }

	bool Get_Deploy_VS_Infantry() { return Get_Int_Parameter("Deploy_VS_Infantry") == 1; }

	bool Get_Deploy_VS_Light_Vehicle() { return Get_Int_Parameter("Deploy_VS_Light_Vehicle") == 1; }

	bool Get_Deploy_VS_Heavy_Vehicle() { return Get_Int_Parameter("Deploy_VS_Heavy_Vehicle") == 1; }

	bool Get_Deploy_VS_VTOL() { return Get_Int_Parameter("Deploy_VS_VTOL") == 1; }

	bool Get_Deploy_VS_Building() { return Get_Int_Parameter("Deploy_VS_Building") == 1; }

	bool Get_Deploy_Is_Stationary() { return Get_Int_Parameter("Deploy_Is_Stationary") == 1; }
};
REGISTER_SCRIPT(MS_AI_Marker_Deployable,
	"Deploy_Script_Name=null:string,"
	"Deploy_VS_Infantry=1:int,"
	"Deploy_VS_Light_Vehicle=1:int,"
	"Deploy_VS_Heavy_Vehicle=1:int,"
	"Deploy_VS_VTOL=1:int,"
	"Deploy_VS_Building=1:int,"
	"Deploy_Is_Stationary=1:int"
);


/*!
* \brief AI No Jump marker
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* Place this script on soldier presets to explicitly tell them to never jump.
*
* This is a logic marker script for presets that have MS_AI_Behaviour on them, or if they have
* the MS_AI_Preferences script attached and bots are created by one MS_AI_Player_Count_Controller
* or individual MS_AI_Player_Controller scripts.
*
* \param None
*   None
*/
class MS_AI_Marker_No_Jump : public ScriptImpClass
{
	void Created(GameObject* obj) override
	{
		if (!obj->As_SoldierGameObj())
		{
			StringClass msg;
			msg.Format("[%d:%hs:%hs] Critical Error: Only soldiers can use the MS_AI_Marker_No_Jump script. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name());
			M_Console_Output(msg.Peek_Buffer());
			Destroy_Script();
			return;
		}
	}
};
REGISTER_SCRIPT(MS_AI_Marker_No_Jump, "");


/*!
* \brief AI Refiller Vehicle marker
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* Place this script on vehicle presets that need to refill their ammo in a script zone.
*
* This is a logic marker script for presets that have MS_AI_Behaviour on them, or if they have
* the MS_AI_Preferences script attached and bots are created by one MS_AI_Player_Count_Controller
* or individual MS_AI_Player_Controller scripts.
*
* \param RefillZonePreset
*   Name of the zone preset where the vehicle can refill
*/
class MS_AI_Marker_Refiller_Vehicle : public ScriptImpClass
{
	void Created(GameObject* obj) override
	{
		if (!obj->As_VehicleGameObj())
		{
			StringClass msg;
			msg.Format("[%d:%hs:%hs] Critical Error: Only vehicles can use the MS_AI_Marker_Refiller_Vehicle script. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name());
			M_Console_Output(msg.Peek_Buffer());
			Destroy_Script();
			return;
		}
		if (!Is_Valid_Preset(Get_Zone_Preset_Name()))
		{
			StringClass msg;
			msg.Format("[%d:%hs:%hs] Critical Error: '%hs' is not a valid preset. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Zone_Preset_Name());
			M_Console_Output(msg.Peek_Buffer());
			Destroy_Script();
			return;
		}
	}

public:
	const char *Get_Zone_Preset_Name()
	{
		return Get_Parameter("RefillZonePreset");
	}

	const char *Get_Exit_Zone_Preset_Name()
	{
		return Get_Parameter("ExitZonePreset");
	}
};
REGISTER_SCRIPT(MS_AI_Marker_Refiller_Vehicle, "RefillZonePreset=null:string,ExitZonePreset=null:string");


/*!
* \brief AI Vehicle Repair Zone marker
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* Place this script on script zone presets where vehicles can get repaired.
*
* This is a logic marker script for presets that have MS_AI_Behaviour on them, or if they have
* the MS_AI_Preferences script attached and bots are created by one MS_AI_Player_Count_Controller
* or individual MS_AI_Player_Controller scripts.
*
* \param Team
*   The team this repair zone belongs to.
*/
class MS_AI_Marker_Vehicle_Repair_Zone : public ScriptImpClass
{
	void Created(GameObject* obj) override
	{
		if (!obj->As_ScriptZoneGameObj())
		{
			StringClass msg;
			msg.Format("[%d:%hs:%hs] Critical Error: Only script zones can use the MS_AI_Marker_Vehicle_Repair_Zone script. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name());
			M_Console_Output(msg.Peek_Buffer());
			Destroy_Script();
			return;
		}
	}

public:
	int Get_Team()
	{
		return Get_Int_Parameter("Team");
	}
};
REGISTER_SCRIPT(MS_AI_Marker_Vehicle_Repair_Zone, "Team=0:int");


/*!
* \brief AI Vehicle ignore marker
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* Place this script on vehicle presets to explicitly mark them as vehicles bots should not search for.
*
* This is a logic marker script for vehicle presets that MS_AI_Behaviour bots don't want to use.
*
* \param IgnorerTeam
*   The team that will ignore this vehicle. 2 is all teams.
*/
class MS_AI_Marker_Vehicle_Search_Ignore : public ScriptImpClass
{
	void Created(GameObject* obj) override
	{
		if (!obj->As_VehicleGameObj())
		{
			StringClass msg;
			msg.Format("[%d:%hs:%hs] Critical Error: Only vehicles can use the MS_AI_Marker_Vehicle_Search_Ignore script. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name());
			M_Console_Output(msg.Peek_Buffer());
			Destroy_Script();
			return;
		}
	}

public:
	int Get_Ignorer_Team()
	{
		return Get_Int_Parameter("IgnorerTeam");
	}
};
REGISTER_SCRIPT(MS_AI_Marker_Vehicle_Search_Ignore, "IgnorerTeam=2:int");


/*!
* \brief AI Transport vehicle override marker
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* Place this script on vehicle presets to explicitly mark them as transport vehicles.
* It makes sure bots use it as a transport, even if the seat count is less than the one set
* TransportVehicleMinimumSeatCount in the bot config file.
*
* This is a logic marker script for vehicle presets that MS_AI_Behaviour bots want to use.
*
* \param None
*   None
*/
class MS_AI_Marker_Transport_Vehicle_Override : public ScriptImpClass
{
	void Created(GameObject* obj) override
	{
		if (!obj->As_VehicleGameObj())
		{
			StringClass msg;
			msg.Format("[%d:%hs:%hs] Critical Error: Only vehicles can use the MS_AI_Marker_Transport_Vehicle_Override script. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name());
			M_Console_Output(msg.Peek_Buffer());
			Destroy_Script();
			return;
		}
	}
};
REGISTER_SCRIPT(MS_AI_Marker_Transport_Vehicle_Override, "");



// -------------------------------------------------------------------------------------------------

static DynamicVectorClass<GameObject*> AIBuildings;

/* Building debug stuff stored outside the globalcontroller because this script can exist without palyer logic bots  */
static bool BuildingDebugInited = false;
static bool BuildingDebugLocationsDisplayEnabled = false;
StringClass BuildingDebugTargetModel;
StringClass BuildingDebugObjectiveModel;

void Init_Building_Debug()
{
	if (BuildingDebugInited)
		return;

	INIClass* config = Get_INI("bot_config.cfg");
	if (config)
	{
		BuildingDebugLocationsDisplayEnabled = config->Get_Bool("Debug", "EnableBuildingTargetsDisplay", false);
		config->Get_String(BuildingDebugTargetModel, "Debug", "BuildingTargetModel", "o_davesarrow");
		config->Get_String(BuildingDebugObjectiveModel, "Debug", "BuildingObjectiveModel", "o_davesarrow");
		Release_INI(config);
		BuildingDebugInited = true;
	}
}

/*!
* \brief AI Building behaviour controller
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* Subject to change.
*
* \param None
*   None
*/
class MS_AI_Building : public ScriptImpClass
{
	const char* customWeakpointPresetName;
	Vector3 mctTargetLocation;
	ReferencerClass objectiveMCTAttackLocationObj;
	ReferencerClass objectiveRepairLocationObj;
	DynamicVectorClass<ReferencerClass> objectiveVehicleAttackLocations;
	float averageGroundHeight;
	bool isDeleted;
	bool isReachableOnGround;

public:
	Vector3 mctObjectiveLocation;
	ReferencerClass objectiveMCTAttackTargetObj;
	ReferencerClass objectiveRepairTargetObj;
	ReferencerClass centralTarget;
	ReferencerClass centralTargetGround;
	DynamicVectorClass<ReferencerClass> exteriorTargets;
	DynamicVectorClass<Vector3> exteriorTargetWatchpoints;
	
private:
	// TODO: optimize reviving procedure, do not recalculate the target positions, only 1x at created OR if there are 0 targets (e.g. script was attached first when building was already dead or started dead)
	void Created(GameObject* obj) override
	{
		Init_Building_Debug();
		isReachableOnGround = true;

		objectiveMCTAttackLocationObj = nullptr;
		objectiveMCTAttackTargetObj = nullptr;
		objectiveRepairLocationObj = nullptr;
		objectiveRepairTargetObj = nullptr;
		centralTarget = nullptr;
		centralTargetGround = nullptr;
		customWeakpointPresetName = Get_Parameter("CustomWeakpointPresetName");
		if ((!obj->As_BuildingGameObj() && !obj->As_PhysicalGameObj()) || obj->As_SoldierGameObj() || (obj->As_VehicleGameObj() && !obj->As_VehicleGameObj()->Is_Turret()))
		{
			StringClass msg;
			msg.Format("[%d:%hs:%hs] Critical Error: Only BuildingGameObjects, turret type Vehicles and Objects can use the MS_AI_Building script. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name());
			M_Console_Output(msg.Peek_Buffer());
			Destroy_Script();
			return;
		}
		else if (strcmp(customWeakpointPresetName, "") != 0 && !Is_Valid_Preset(customWeakpointPresetName))
		{
			StringClass msg;
			msg.Format("[%d:%hs:%hs] Critical Error: '%hs' is not a valid preset. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), customWeakpointPresetName);
			M_Console_Output(msg.Peek_Buffer());
			Destroy_Script();
			return;
		}
		else
		{
			Init(obj);

			// Setup automatic repair needed triggers
			StringClass params;
			params.Format("%d,%d,%d,%d,%d,%d,%d,%s", (int)(Commands->Get_Max_Health(obj) * ((float)(100 - (Get_Int_Parameter("DamagePercentToRepair"))) / 100)), 0, CUSTOM_AI_REPAIR_NEEDED, 0, -1, 0, 0, "false");
			Commands->Attach_Script(obj, "MS_Health_Send_Custom", params.Peek_Buffer());
			params.Format("%d,%d,%d,%d,%d,%d,%d,%s", (int)(Commands->Get_Max_Health(obj)), -1, 0, 0, 0, CUSTOM_AI_REPAIR_COMPLETE, 0, "false");
			Commands->Attach_Script(obj, "MS_Health_Send_Custom", params.Peek_Buffer());
		}
	}

	void Init(GameObject* obj)
	{
		// Check if building has any polygons at all, don't compute unnecessarily in cases where there is only a building controller but no mesh belonging to it (e.g. C&C_MiniFrost)
		if (obj->As_BuildingGameObj())
		{
			Vector3 closestPoly;
			Vector3 bldCenter = Commands->Get_Position(obj);
			Find_Closest_Poly_Position(obj->As_BuildingGameObj(), bldCenter, &closestPoly);
			// If this building has no polygons at all, then bots cannot attack it or repair it, destroy script
			if (closestPoly == Vector3())
			{
				if (BuildingDebugLocationsDisplayEnabled)
				{
					StringClass debug;
					debug.Format("msg %s: ATTENTION: This building has no polygons (only a building controller)", Commands->Get_Preset_Name(obj));
					M_Console_Input(debug.Peek_Buffer());
				}
				Destroy_Script();
				return;
			}
		}

		auto &boundingBox = obj->As_BuildingGameObj() ? obj->As_BuildingGameObj()->Get_Bounds() : obj->As_PhysicalGameObj()->Peek_Model()->Get_Bounding_Box();
		const int bldRadius = (int)(max(boundingBox.Extent.X, boundingBox.Extent.Y));
		mctObjectiveLocation = Vector3();
		objectiveRepairLocationObj = nullptr;
		objectiveRepairTargetObj = nullptr;

		Vector3 MCTPos = Vector3();
		float facing = 0;
		bool hasWeakpoint = true;

		if (customWeakpointPresetName[0] != '\0')
		{
			if (auto customMct = Find_Nearest_Preset(Commands->Get_Position(obj), customWeakpointPresetName))
			{
				Matrix3D transform = Matrix3D::Identity;
				// Check if it has a target1, target2, etc. bone for precision
				for (int i = 1; i < 4; i++) {
					StringClass boneName;
					boneName.Format("target%d", i);

					if (int boneIdx = customMct->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Index(boneName.Peek_Buffer()))
					{
						// M_Console_Input("msg Target bone found!");
						transform = customMct->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Transform(boneIdx);
						break;
					}
				}

				if (transform == Matrix3D::Identity)
				{
					customMct->Get_Position(&MCTPos);
					MCTPos.Z += 1.2f;
					facing = DEG2RAD(Commands->Get_Facing(customMct));
					// M_Console_Input("msg Target bone NOT found!");
				}
				else
				{
					MCTPos = transform.Get_Translation();
					facing = transform.Get_Z_Rotation();
				}
			}
			else
			{
				StringClass msg;
				msg.Format("[%d:%hs:%hs] Critical Error: custom weakpoint not found. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name());
				M_Console_Output(msg.Peek_Buffer());
				Destroy_Script();
				return;
			}
		}
		else if (obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->Find_MCT())
		{
			BuildingAggregateClass* MCT = obj->As_BuildingGameObj()->Find_MCT();
			auto &mctBoundingBox = MCT->Get_Bounding_Box();
			const Vector3 mctBoundingBoxCenter = mctBoundingBox.Center;

			//MCT->Get_Position(&MCTPos);

			// MCT location correction needed
			// if (Vector3::Distance(MCTPos, mctBoundingBoxCenter) > 1.0f)
			MCTPos = mctBoundingBoxCenter;
			facing = MCT->Get_Facing();

			// MCT mesh is somewhere inside the big aggregate... 
			if (mctBoundingBox.Extent.X > 2 || mctBoundingBox.Extent.Y > 2 || mctBoundingBox.Extent.Z > 2)
			{
				// M_Console_Input("msg Big MCT aggregate.");

				// Check for help from config
				auto bldName = Commands->Get_Preset_Name(obj);
				INIClass* config = Get_INI("bot_config.cfg");

				if (config)
				{
					if (auto bldSection = config->Find_Section(bldName))
					{
						Vector3 offset;
						offset.X += config->Get_Float(bldName, "X", 0) * cos(facing);
						offset.Y += config->Get_Float(bldName, "X", 0) * sin(facing);

						offset.Y += config->Get_Float(bldName, "Y", 0) * cos(facing);
						offset.X -= config->Get_Float(bldName, "Y", 0) * sin(facing);

						offset.Z += config->Get_Float(bldName, "Z", 0);

						MCTPos += offset;
					}
					Release_INI(config);
				}
			}
		}
		else
		{
			// No MCT, no custom weakpoint defined, find a proper goto/repair location still (e.g. APB Helipads)
			hasWeakpoint = false;
			const Vector3 bldCenter = obj->As_BuildingGameObj() ? boundingBox.Center : Commands->Get_Position(obj);

			bool isWithinPathfindGrid = false;
			for (int i = 1; i < bldRadius + 5; i += 1)
			{
				isWithinPathfindGrid = Get_Closest_Pathfind_Spot_Size(bldCenter, (float)i, &mctObjectiveLocation, 0.5f);
				if (isWithinPathfindGrid)
				{
					if (obj->As_BuildingGameObj())
					{
						Find_Closest_Poly_Position(obj->As_BuildingGameObj(), mctObjectiveLocation, &mctTargetLocation);
					}
					else
					{
						Matrix3D transform = Matrix3D::Identity;

						int boneIdx = obj->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Index("repair");
						if (!boneIdx)
						{
							boneIdx = obj->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Index("target");
						}
						if (boneIdx)
						{
							// M_Console_Input("msg Target bone found!");
							transform = obj->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Transform(boneIdx);
						}

						if (transform == Matrix3D::Identity)
						{
							mctTargetLocation = bldCenter;
							mctTargetLocation.Z += 0.2f;
							facing = DEG2RAD(Commands->Get_Facing(obj));
							// M_Console_Input("msg Target bone NOT found!");
						}
						else
						{
							mctTargetLocation = transform.Get_Translation();
							facing = transform.Get_Z_Rotation();
						}
					}
					break;
				}
			}

			if (!isWithinPathfindGrid)
			{
				if (BuildingDebugLocationsDisplayEnabled)
				{
					StringClass debug;
					debug.Format("msg %s: WARNING: Unable to find a suitable MCT/weakpoint objective location for bots", Commands->Get_Preset_Name(obj));
					M_Console_Input(debug.Peek_Buffer());
				}
				isReachableOnGround = false;
			}
		}

		if (hasWeakpoint)
		{
			Vector3 testPos;
			facing -= WWMATH_PI / 2;

			const float mctTargetForwardOffset = 0.3f;


			bool isSuitable = false;
			// Find a good position
			for (int i = 0; i < 7; i++)
			{
				facing = facing + WWMATH_PI / 2;
				testPos = MCTPos;
				testPos.X += 1.3f * cos(facing);
				testPos.Y += 1.3f * sin(facing);
				testPos.Z -= 0.3f;

				Vector3 helpPos = Vector3();
				bool isWithinPathfindGrid = Get_Closest_Pathfind_Spot_Size(testPos, 0.25f, &helpPos, 0.5f);
				helpPos = MCTPos;
				helpPos.X += mctTargetForwardOffset * cos(facing);
				helpPos.Y += mctTargetForwardOffset * sin(facing);
				if (isWithinPathfindGrid && Can_Generic_Soldier_Teleport(testPos) && !M_Test_Raycast_Collision(helpPos, testPos, false))
				{
					isSuitable = true;
					break;
				}
				// else Wrong MCT objective pos, trying a different one
				/*else
				{
					if (!isWithinPathfindGrid)
						M_Console_Input("msg Wrong MCT objective pos Reason: not in pathfind grid.");
					else if (!Can_Generic_Soldier_Teleport(testPos))
						M_Console_Input("msg Wrong MCT objective pos Reason: Can_Generic_Soldier_Teleport NOT");
					else if (M_Test_Raycast_Collision(helpPos, testPos, false))
						M_Console_Input("msg Wrong MCT objective pos Reason: M_Test_Raycast_Collision problems.");
				}*/
			}

			if (!isSuitable)
			{
				if (BuildingDebugLocationsDisplayEnabled)
				{
					StringClass debug;
					debug.Format("msg %s: WARNING: Unable to find a suitable MCT/weakpoint objective location for bots", Commands->Get_Preset_Name(obj));
					M_Console_Input(debug.Peek_Buffer());
				}
				isReachableOnGround = false;
			}

			mctObjectiveLocation = testPos;
			mctTargetLocation = MCTPos;
		}


		// Generate building exterior target markers, better perf than calling find closest poly/ lots of raycasts on every bot constantly
		Generate_Targets(obj, boundingBox);

		StringClass params;
		// Add unique central targets which will be tested first always by bots, it reduces the number of raycasts in a lot of cases
		centralTarget = Commands->Create_Object("Invisible_Object", boundingBox.Center);
		auto skinName = ArmorWarheadManager::Get_Armor_Name(obj->As_DamageableGameObj()->Get_Defense_Object()->Get_Skin());
		params.Format("Blamo,%s,0", skinName);
		Commands->Attach_Script(centralTarget, "MS_Switch_Skin_And_Shield_Timer", params.Peek_Buffer());
		Commands->Attach_Script(centralTarget, "dp88_AI_Marker_Building", "");
		Set_Object_Type(centralTarget, Get_Object_Type(obj));
		Set_Max_Shield_Strength(centralTarget, Commands->Get_Max_Health(obj));

		centralTargetGround = Commands->Create_Object("Invisible_Object", Vector3(boundingBox.Center.X, boundingBox.Center.Y, averageGroundHeight));
		Commands->Attach_Script(centralTargetGround, "MS_Switch_Skin_And_Shield_Timer", params.Peek_Buffer());
		Commands->Attach_Script(centralTargetGround, "dp88_AI_Marker_Building", "");
		Set_Object_Type(centralTargetGround, Get_Object_Type(obj));
		Set_Max_Shield_Strength(centralTargetGround, Commands->Get_Max_Health(obj));

		// Also create target marker at the weakpoint for soldiers
		if (isReachableOnGround)
		{
			auto targetObj = Commands->Create_Object("Invisible_Object", mctTargetLocation);
			if (BuildingDebugLocationsDisplayEnabled)
				Commands->Set_Model(targetObj, BuildingDebugTargetModel.Peek_Buffer());
			Commands->Attach_Script(targetObj, "MS_Switch_Skin_And_Shield_Timer", params.Peek_Buffer());
			Commands->Attach_Script(targetObj, "dp88_AI_Marker_Building", "");
			Set_Object_Type(targetObj, Get_Object_Type(obj));
			Set_Max_Shield_Strength(targetObj, Commands->Get_Max_Health(obj));
			objectiveMCTAttackTargetObj = targetObj;
		}

		// Next, generate go-to locations
		// If weakpoint is reachable, just use its already calculated location
		Vector3 soldierAttackObjectiveLocation = mctObjectiveLocation;
		// Otherwise, more calculations are needed to make sure the building is attackable (closest pathfind spot is not guaranteed to hit the building from, eg. RA_Fissure top silos, rocks cover it)
		if (!isReachableOnGround)
		{
			Get_Closest_Pathfind_Spot_Size(boundingBox.Center, bldRadius + 50.0f, &soldierAttackObjectiveLocation, 0.5f);
			Vector3 helper;
			int count = 0;
			while (count < 50 &&
				!M_Test_Raycast_Collision(soldierAttackObjectiveLocation, boundingBox.Center, false, &helper, obj) &&
				!M_Test_Raycast_Collision(soldierAttackObjectiveLocation, boundingBox.Center + Vector3(0, 0, boundingBox.Extent.Z), false, &helper, obj))
			{
				count++;
				helper = soldierAttackObjectiveLocation - (boundingBox.Center - soldierAttackObjectiveLocation).Normalized() * (float)count;
				Get_Closest_Pathfind_Spot_Size(helper, 5.0f, &soldierAttackObjectiveLocation, 0.5f);
			}
			// Place a target here to make sure the soldiers actually see a target to shoot from here (is not always certain, e.g. on RA_Fissure Soviet Silo objective)
			Validate_New_Target(obj, helper, soldierAttackObjectiveLocation);
			//StringClass msg;
			//msg.Format("msg %s: Recalced a proper position %d times", Commands->Get_Preset_Name(obj), count);
			//M_Console_Input(msg);
		}

		// One objective for soldiers to run to the MCT/attack location when no weakpoint present
		auto attackObjectiveObj = Commands->Create_Object("Invisible_Object", soldierAttackObjectiveLocation);
		if (BuildingDebugLocationsDisplayEnabled)
			Commands->Set_Model(attackObjectiveObj, BuildingDebugObjectiveModel.Peek_Buffer());
		Commands->Attach_Script(attackObjectiveObj, "MS_Switch_Skin_And_Shield_Timer", "Blamo,Blamo,0");
		params.Format("%d,%d,%d,%d,%d,%d,%d,%s", Get_Object_Type(obj) ? 0 : 1, dp88_AI_Objective::TYPE_OFFENSIVE, 1, 5, 0, 0, 0, Commands->Get_Preset_Name(obj));
		Commands->Attach_Script(attackObjectiveObj, "dp88_AI_Objective", params.Peek_Buffer());
		objectiveMCTAttackLocationObj = attackObjectiveObj;

		// Objectives for vehicles to go to the building
		const float offsetAmount = 5.0f;
		Vector3 offsetVectors[4];
		offsetVectors[0] = Vector3(+boundingBox.Extent.X + offsetAmount, 0, 0);
		offsetVectors[1] = Vector3(-boundingBox.Extent.X - offsetAmount, 0, 0);
		offsetVectors[2] = Vector3(0, +boundingBox.Extent.Y + offsetAmount, 0);
		offsetVectors[3] = Vector3(0, -boundingBox.Extent.Y - offsetAmount, 0);
		Vector3 objectivePos;
		int vehObjectivesCount = 0;
		for (int i = 0; i < 4; i++)
		{
			if (Get_Closest_Pathfind_Spot_Size(Vector3(boundingBox.Center.X, boundingBox.Center.Y, averageGroundHeight) + offsetVectors[i], offsetAmount - 1.0f, &objectivePos, 2.5f))
			{
				// Check for weird situations where pathfind sectors are epic tall and points would not be anywhere near the ground
				Vector3 checkPoint = objectivePos + Vector3(0, 0, 2 * -boundingBox.Extent.Z - 5.0f);
				Vector3 helper;
				if (M_Test_Raycast_Collision(objectivePos, checkPoint, false, &helper) && abs(helper.Z - objectivePos.Z) > 2.0f)
					objectivePos.Z = helper.Z + 2.0f;

				auto vehAttackObjectiveObj = Commands->Create_Object("Invisible_Object", objectivePos);
				if (BuildingDebugLocationsDisplayEnabled)
					Commands->Set_Model(vehAttackObjectiveObj, BuildingDebugObjectiveModel.Peek_Buffer());
				Commands->Attach_Script(vehAttackObjectiveObj, "MS_Switch_Skin_And_Shield_Timer", "Blamo,Blamo,0");
				params.Format("%d,%d,%d,%d,%d,%d,%d,%s", Get_Object_Type(obj) ? 0 : 1, dp88_AI_Objective::TYPE_OFFENSIVE, 10, 0, 5, 5, 5, Commands->Get_Preset_Name(obj));
				Commands->Attach_Script(vehAttackObjectiveObj, "dp88_AI_Objective", params.Peek_Buffer());
				objectiveVehicleAttackLocations.Add(vehAttackObjectiveObj);
				vehObjectivesCount++;
			}
		}

		// Fallback when no locations found, uncomment when needed
		/*if (!count && Get_Closest_Pathfind_Spot_Size(Vector3(boundingBox.Center.X, boundingBox.Center.Y, averageGroundHeight), bldRadius + 50.0f, &objectivePos, 2.0f))
		{
			auto vehAttackObjectiveObj = Commands->Create_Object("Invisible_Object", objectivePos); Commands->Set_Model(vehAttackObjectiveObj, "o_davesarrow");
			Commands->Attach_Script(vehAttackObjectiveObj, "MS_Switch_Skin_And_Shield_Timer", "Blamo,Blamo,0");
			params.Format("%d,%d,%d,%d,%d,%d,%d", Get_Object_Type(obj) ? 0 : 1, dp88_AI_Objective::TYPE_OFFENSIVE, (2 * bldRadius), 0, 5, 5, 5);
			Commands->Attach_Script(vehAttackObjectiveObj, "dp88_AI_Objective", params.Peek_Buffer());
			objectiveVehicleAttackLocations.Add(vehAttackObjectiveObj);
			vehObjectivesCount++;
		}*/

		// Finally, add a marker if it's not a building logic type, otherwise they will be treated as light vehicles
		if (!obj->As_BuildingGameObj())
			Commands->Attach_Script(obj, "dp88_AI_Marker_Building", "");

		
		if (BuildingDebugLocationsDisplayEnabled)
		{
			StringClass debug;
			debug.Format("msg %s targets: %d, veh objectives: %d (bounding box: %.2f/%.2f/%.2f)", Commands->Get_Preset_Name(obj), exteriorTargets.Count(), vehObjectivesCount, boundingBox.Extent.X, boundingBox.Extent.Y, boundingBox.Extent.Z);
			M_Console_Input(debug);
		}

		Commands->Start_Timer(obj, this, 1.0f, 1);

		isDeleted = false;
		AIBuildings.Add(obj);

		/*const char* presetName = "";
		auto list = Combine_Lists(Get_Enlisted_Purchase_Items(1), Get_Purchase_Items(1, PurchaseSettingsDefClass::TYPE_CLASSES));
		for (int i = 0; i < list.Count(); i++)
		{
			presetName = DefinitionMgrClass::Find_Definition(list[i], false)->Get_Name();
			if (Is_Valid_Preset(presetName))
				break;
		}

		auto testSold = ObjectLibraryManager::Create_Object(presetName);
		Set_Object_Type(testSold, 2);
		testSold->As_SoldierGameObj()->Set_Grav_Scale(0);
		Commands->Set_Position(testSold, mctObjectiveLocation);
		Commands->Set_Facing(testSold, (float)RAD2DEG(facing));

		auto testSold2 = ObjectLibraryManager::Create_Object(presetName);
		Set_Object_Type(testSold2, 2);
		testSold2->As_SoldierGameObj()->Set_Grav_Scale(0);
		Commands->Set_Position(testSold2, mctTargetLocation);
		Commands->Set_Facing(testSold2, (float)RAD2DEG(facing));*/

	}

	void Generate_Targets(GameObject *obj, const AABoxClass &boundingBox)
	{
		// Add full Z extent to make sure we start checking above the ground, even if the building's underground interior is bigger than above ground
		const float heightStart = obj->As_BuildingGameObj() ? (boundingBox.Extent.Z + 2.0f) : 0;
		const float sideOffset = 2.0f;
		Vector3 sg1 = boundingBox.Center + Vector3(boundingBox.Extent.X + sideOffset, 0, heightStart);
		Vector3 sg2 = boundingBox.Center + Vector3(-boundingBox.Extent.X - sideOffset, 0, heightStart);
		Vector3 sg3 = boundingBox.Center + Vector3(0, boundingBox.Extent.Y + sideOffset, heightStart);
		Vector3 sg4 = boundingBox.Center + Vector3(0, -boundingBox.Extent.Y - sideOffset, heightStart);

		// Corners next
		Vector3 sg5 = boundingBox.Center + Vector3( boundingBox.Extent.X + sideOffset,  boundingBox.Extent.Y + sideOffset, heightStart);
		Vector3 sg6 = boundingBox.Center + Vector3(-boundingBox.Extent.X - sideOffset, -boundingBox.Extent.Y - sideOffset, heightStart);
		Vector3 sg7 = boundingBox.Center + Vector3( boundingBox.Extent.X + sideOffset, -boundingBox.Extent.Y - sideOffset, heightStart);
		Vector3 sg8 = boundingBox.Center + Vector3(-boundingBox.Extent.X - sideOffset,  boundingBox.Extent.Y + sideOffset, heightStart);

		Place_Target_Pair(obj, sg1, sg2, boundingBox);
		Place_Target_Pair(obj, sg3, sg4, boundingBox);
		Place_Target_Pair(obj, sg5, sg6, boundingBox);
		Place_Target_Pair(obj, sg7, sg8, boundingBox);

		averageGroundHeight = (sg1.Z + sg2.Z + sg3.Z + sg4.Z + sg5.Z + sg6.Z + sg7.Z + sg8.Z) / 8;

		// Place more targets if the building is wide 
		float step = boundingBox.Extent.Y > 28 ? 14.0f : 8.0f;
		for (float i = step; i < boundingBox.Extent.Y - step; i += step)
		{
			Vector3 p1 = boundingBox.Center + Vector3(boundingBox.Extent.X + sideOffset, i, heightStart);
			Vector3 p2 = boundingBox.Center + Vector3(-boundingBox.Extent.X - sideOffset, i, heightStart);
			Vector3 p3 = boundingBox.Center + Vector3(boundingBox.Extent.X + sideOffset, -i, heightStart);
			Vector3 p4 = boundingBox.Center + Vector3(-boundingBox.Extent.X - sideOffset, -i, heightStart);
			Place_Target_Pair(obj, p1, p2, boundingBox);
			Place_Target_Pair(obj, p3, p4, boundingBox);
		}

		step = boundingBox.Extent.X > 28 ? 14.0f : 8.0f;
		for (float i = step; i < boundingBox.Extent.X - step; i += step)
		{
			Vector3 p1 = boundingBox.Center + Vector3(i, boundingBox.Extent.Y + sideOffset, heightStart);
			Vector3 p2 = boundingBox.Center + Vector3(i, -boundingBox.Extent.Y - sideOffset, heightStart);
			Vector3 p3 = boundingBox.Center + Vector3(-i, boundingBox.Extent.Y + sideOffset, heightStart);
			Vector3 p4 = boundingBox.Center + Vector3(-i, -boundingBox.Extent.Y - sideOffset, heightStart);
			Place_Target_Pair(obj, p1, p2, boundingBox);
			Place_Target_Pair(obj, p3, p4, boundingBox);
		}

		// Place targets for bombers / units that are looking at the building from up high
		const float stepX = boundingBox.Extent.X > 14 ? 12.0f : 6.0f;
		const float stepY = boundingBox.Extent.Y > 14 ? 12.0f : 6.0f;
		for (float i = -boundingBox.Extent.X; i < boundingBox.Extent.X; i += stepX)
		{
			for (float j = -boundingBox.Extent.Y; j < boundingBox.Extent.Y; j += stepY)
			{
				Vector3 p1 = boundingBox.Center + Vector3(i, j, +(boundingBox.Extent.Z + 2.0f));
				Vector3 p2 = boundingBox.Center + Vector3(i, j, -(boundingBox.Extent.Z + 2.0f));
				Vector3 contactPoint;
				if (M_Test_Raycast_Collision(p1, p2, false, &contactPoint, obj))
				{
					Validate_New_Target(obj, contactPoint, p1);
				}
			}
		}

		// Place one extra topdown center point
		Vector3 contactPoint;
		Vector3 p1 = boundingBox.Center + Vector3(0, 0, +(boundingBox.Extent.Z + 2.0f));
		Vector3 p2 = boundingBox.Center + Vector3(0, 0, -(boundingBox.Extent.Z + 2.0f));

		if (!obj->As_BuildingGameObj())
		{
			int boneIdx = obj->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Index("target");
			if (!boneIdx)
				boneIdx = obj->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Index("turret");
			if (!boneIdx)
				boneIdx = obj->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Index("ROOTTRANSFORM");
			if (boneIdx)
			{
				auto transform = obj->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Transform(boneIdx);
				p2 = transform.Get_Translation();
				p2 = p2 - (p1 - p2).Normalized() * Vector3::Distance(p1, p2);
			}
		}
		if (M_Test_Raycast_Collision(p1, p2, !obj->As_BuildingGameObj(), &contactPoint, obj))
		{
			Validate_New_Target(obj, contactPoint, p1);
		}
	}

	// TODO: rename
	void Place_Target_Pair(GameObject *obj, Vector3 &sg1, Vector3 &sg2, const AABoxClass &boundingBox)
	{
		Vector3 closestPoly1;
		Vector3 closestPoly2;
		for (int i = 0; i < 2; i++)
		{
			Vector3 &point = i == 0 ? sg1 : sg2;
			Vector3 originalPoint = point;
			Vector3 helper;
			int limit = 100;
			while (limit && !Get_Closest_Pathfind_Spot(point, 1.7f, &helper))
			{
				point.Z -= 1.5f;
				limit--;
				//auto g1 = Commands->Create_Object("Invisible_Object", point); Commands->Set_Model(g1, "o_davesarrow");
			}

			// Check for weird situations where pathfind sectors are epic tall and points would not be anywhere near the ground (e.g. APB Beta Bonzai, Allied Ref)
			// Also, buildings offmap with no pathfind right next to them need extra help raycast downwards to detect the ground location before placing targets
			Vector3 &startingPoint = limit ? point : originalPoint;
			Vector3 checkPoint = Vector3(point.X, point.Y, boundingBox.Center.Z) + Vector3(0, 0, 2 * -boundingBox.Extent.Z + 2.0f);

			if (M_Test_Raycast_Collision(startingPoint, checkPoint, false, &helper) && abs(helper.Z - startingPoint.Z) > 3.0f)
			{
				point.Z = helper.Z + 3.0f;
			}
			else if (!limit)
			{
				//StringClass msg;
				//msg.Format("msg %s: point failed to find a proper spot", Commands->Get_Preset_Name(obj));
				//M_Console_Input(msg);
				// No ground around the building?! Oookay, then allow to shoot its underground interior parts too.
				point.Z = boundingBox.Center.Z + (boundingBox.Extent.Z > 6.0f ? -boundingBox.Extent.Z + 1.0f : 0);
				// Return, otherwise this point can end up deep underground firing rays upwards, creating underground interior targets, which is not wanted
				// return;
			}

			//auto g1 = Commands->Create_Object("Invisible_Object", point); Commands->Set_Model(g1, "o_davesarrow");

			Vector3 &closestPoly = i == 0 ? closestPoly1 : closestPoly2;
			if (obj->As_BuildingGameObj())
			{
				Find_Closest_Poly_Position(obj->As_BuildingGameObj(), point, &closestPoly);
				// Do not place point to closest poly, this is unreliable and a bad source of bad target locations (in the interior)
			}
			else
			{
				int boneIdx = obj->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Index("target");
				if (!boneIdx)
					boneIdx = obj->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Index("turret");
				if (!boneIdx)
					boneIdx = obj->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Index("ROOTTRANSFORM");
				if (boneIdx)
				{
					auto transform = obj->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Transform(boneIdx);
					closestPoly = transform.Get_Translation();
				}
			}
		}

		Raycast_Target_Pair(obj, sg1, sg2, closestPoly1, closestPoly2);

		// Tall buildings
		const float heightAvg = (sg1.Z + sg2.Z) / 2;
		int i = 1;
		while (boundingBox.Center.Z + boundingBox.Extent.Z > heightAvg + 10 * i)
		{
			Vector3 higherPoint1 = sg1 + Vector3(0, 0, 10.0f * i);
			Vector3 higherPoint2 = sg2 + Vector3(0, 0, 10.0f * i);
			if (obj->As_BuildingGameObj())
			{
				Find_Closest_Poly_Position(obj->As_BuildingGameObj(), higherPoint1, &closestPoly1);
				Find_Closest_Poly_Position(obj->As_BuildingGameObj(), higherPoint2, &closestPoly2);
			}
			Raycast_Target_Pair(obj, higherPoint1, higherPoint2, closestPoly1, closestPoly2);
			i++;
		}
	}

	// TODO: rename
	void Raycast_Target_Pair(GameObject *obj, Vector3 &sg1, Vector3 &sg2, const Vector3 &closestPoly1, const Vector3 &closestPoly2)
	{
		for (int i = 0; i < 2; i++)
		{
			Vector3 &p1 = i == 0 ? sg1 : sg2;
			Vector3 &p2 = i == 0 ? sg2 : sg1;
			Vector3 contactPoint;
			if (M_Test_Raycast_Collision(p1, p2, !obj->As_BuildingGameObj(), &contactPoint, obj))
			{
				Validate_New_Target(obj, contactPoint, p1);
			}
			else
			{
				Vector3 closestPoly = i == 0 ? closestPoly1 : closestPoly2;
				Vector3 buildingCtrlPos = Commands->Get_Position(obj);
				Vector3 otherPoint1 = closestPoly - (p1 - closestPoly).Normalized() * Vector3::Distance(p1, closestPoly);
				Vector3 otherPoint2 = buildingCtrlPos - (p1 - buildingCtrlPos).Normalized() * Vector3::Distance(p1, buildingCtrlPos);

				// Closest poly is unreliable, proven can be on an invisible physical only blocker, do not use it for building target position
				// Closest poly, ESPECIALLY stepped in any direction, is unreliable to start raycasting from. Only raycasting towards it should be used.
				// Vector3 steppedClosestPoly = closestPoly + (p1 - closestPoly).Normalized() * 1.5f;
				if (M_Test_Raycast_Collision(p1, otherPoint1, !obj->As_BuildingGameObj(), &contactPoint, obj))
				{
					Validate_New_Target(obj, contactPoint, p1);
				}
				else if (M_Test_Raycast_Collision(p1, otherPoint2, !obj->As_BuildingGameObj(), &contactPoint, obj))
				{
					Validate_New_Target(obj, contactPoint, p1);
				}
			}
		}
	}

	void Validate_New_Target(GameObject *obj, const Vector3& newTargetLocation, const Vector3& raycastedFromLocation)
	{
		float min_dist = (obj->As_BuildingGameObj() ? (exteriorTargets.Count() >= 8 ? 3.0f : 1.0f) : (exteriorTargets.Count() >= 4 ? 1.0f : 0.5f));
		float min_dist_sq = min_dist * min_dist;
		for (int i = 0; i < exteriorTargets.Count(); i++)
		{
			auto target = exteriorTargets[i];
			if (Vector3::Distance_Squared(Commands->Get_Position(target), newTargetLocation) < min_dist_sq)
				return;
		}
		auto extTarget = Commands->Create_Object("Invisible_Object", newTargetLocation);
		if (BuildingDebugLocationsDisplayEnabled)
			Commands->Set_Model(extTarget, BuildingDebugTargetModel.Peek_Buffer());
		auto skinName = ArmorWarheadManager::Get_Armor_Name(obj->As_DamageableGameObj()->Get_Defense_Object()->Get_Skin());
		StringClass params;
		params.Format("Blamo,%s,0", skinName);
		Commands->Attach_Script(extTarget, "MS_Switch_Skin_And_Shield_Timer", params.Peek_Buffer());
		Commands->Attach_Script(extTarget, "dp88_AI_Marker_Building", "");
		Set_Object_Type(extTarget, Get_Object_Type(obj));
		Set_Max_Shield_Strength(extTarget, Commands->Get_Max_Health(obj));
		Vector3 exteriorTargetLocation = Commands->Get_Position(extTarget);
		Vector3 targetWatchLocation = exteriorTargetLocation + (raycastedFromLocation - exteriorTargetLocation).Normalized() * 0.2f;
		exteriorTargets.Add(extTarget);
		exteriorTargetWatchpoints.Add(targetWatchLocation);
	}

	void Timer_Expired(GameObject *obj, int number) override
	{
		if (number == 1)
		{
			// Sync health of targets with building hp so bots prioritize between buildings/other units correctly
			if (centralTarget && Commands->Get_Shield_Strength(centralTarget) != Commands->Get_Health(obj))
			{
				const float newHealth = Commands->Get_Health(obj);
				Commands->Set_Shield_Strength(centralTarget, newHealth);
				Commands->Set_Shield_Strength(centralTargetGround, newHealth);
				if (objectiveMCTAttackTargetObj)
					Commands->Set_Shield_Strength(objectiveMCTAttackTargetObj, newHealth);

				for (int i = 0; i < exteriorTargets.Count(); i++)
				{
					auto target = exteriorTargets[i];
					Commands->Set_Shield_Strength(target, newHealth);
				}
			}
			Commands->Start_Timer(obj, this, 4.0f, 1);
		}
	}

	void Custom(GameObject* obj, int type, int param, GameObject* sender) override
	{
		if (type == CUSTOM_AI_REPAIR_NEEDED)
		{
			if (!objectiveRepairLocationObj && isReachableOnGround)
			{
				auto objectiveObj = Commands->Create_Object("Invisible_Object", mctObjectiveLocation);
				Commands->Attach_Script(objectiveObj, "MS_Switch_Skin_And_Shield_Timer", "Blamo,Blamo,0");
				objectiveRepairLocationObj = objectiveObj;
				StringClass params;
				params.Format("%d,%d,%d,%d,%d,%d,%d,%s", Get_Object_Type(obj), dp88_AI_Objective::TYPE_ENGINEERING, 1, 5, 0, 0, 0, Commands->Get_Preset_Name(obj));
				Commands->Attach_Script(objectiveObj, "dp88_AI_Objective", params.Peek_Buffer());

				// Also create target marker at the mct
				auto targetObj = Commands->Create_Object("Invisible_Object", mctTargetLocation);
				auto skinName = ArmorWarheadManager::Get_Armor_Name(obj->As_DamageableGameObj()->Get_Defense_Object()->Get_Skin());
				params.Format("Blamo,%s,0", skinName);
				Commands->Attach_Script(targetObj, "MS_Switch_Skin_And_Shield_Timer", params.Peek_Buffer());
				objectiveRepairTargetObj = targetObj;
				// Make it teamed so engineers will "shoot" it with the updated Enemy_Seen calls
				Set_Object_Type(targetObj, Get_Object_Type(obj));
				StringClass repParams;
				repParams.Format("%.2f", dp88_customAI::getDistance(objectiveObj, targetObj));
				Commands->Attach_Script(targetObj, "dp88_AI_Marker_Repairable", repParams.Peek_Buffer());

				if (BuildingDebugLocationsDisplayEnabled)
				{
					StringClass debug;
					debug.Format("msg %s: repair needed!", Commands->Get_Preset_Name(obj));
					M_Console_Input(debug.Peek_Buffer());
				}
			}
		}
		else if (type == CUSTOM_AI_REPAIR_COMPLETE)
		{
			if (objectiveRepairLocationObj && isReachableOnGround)
			{
				Commands->Destroy_Object(objectiveRepairLocationObj);
				Commands->Destroy_Object(objectiveRepairTargetObj);
				objectiveRepairLocationObj = nullptr;
				objectiveRepairTargetObj = nullptr;

				if (BuildingDebugLocationsDisplayEnabled)
				{
					StringClass debug;
					debug.Format("msg %s: repair complete!", Commands->Get_Preset_Name(obj));
					M_Console_Input(debug.Peek_Buffer());
				}
			}
		}
		else if (type == CUSTOM_EVENT_BUILDING_REVIVED)
		{
			Init(obj);
		}
	}

	void Damaged(GameObject* obj, GameObject* damager, float amount) override
	{
		// Suppress damage "spam"
		if (M_Has_Timer(obj, this, 2) || amount == 0)
			return;

		Commands->Start_Timer(obj, this, 5.0f, 2);
		if (!damager || (!damager->As_SoldierGameObj() && !damager->As_VehicleGameObj()))
			return;

		Vector3 targetPos = Vector3();
		bool isWithinPathfindGrid = Get_Closest_Pathfind_Spot_Size(Commands->Get_Position(damager), 2.0f, &targetPos, 2.0f);
		if (!isWithinPathfindGrid)
		{
			dp88_customAI::TargetType targetType = dp88_customAI::GetTargetType(damager);
			if (targetType != dp88_customAI::FLYING && !Get_Closest_Pathfind_Spot_Size(Commands->Get_Position(damager), 20.0f, &targetPos, 2.0f))
			{
				return;
			}
			// If attacker is flying, find ground location under it
			else if (targetType == dp88_customAI::FLYING)
			{
				Vector3 enemyLocation = Commands->Get_Position(damager);
				Vector3 helper;
				if (!M_Test_Raycast_Collision(enemyLocation, enemyLocation + Vector3(0, 0, -100), false, &helper) ||
					// Assume the raycast landed within a pathfind grid, run a smaller radius check, and a bigger one only if that fails
					(!Get_Closest_Pathfind_Spot_Size(helper, 10.0f, &targetPos, 2.0f) && !Get_Closest_Pathfind_Spot_Size(helper, 50.0f, &targetPos, 2.0f)))
				{
					return;
				}
			}
			else
			{
				// Failed to find suitable location to go closer to target enemy, better not place than cause massive pathfind fail suiciding
				return;
			}
		}
		else
		{
			// Place exactly at enemy if within pathfind grid, not around in the pathfind sector
			targetPos = Commands->Get_Position(damager);
		}
		Create_Timed_Objective(Get_Object_Type(obj), 8.0f, targetPos, 2, dp88_AI_Objective::TYPE_DEFENSIVE);

	}

	void Cleanup(GameObject* obj)
	{
		if (!isDeleted)
		{
			Commands->Destroy_Object(objectiveMCTAttackLocationObj);
			Commands->Destroy_Object(objectiveMCTAttackTargetObj);
			Commands->Destroy_Object(objectiveRepairLocationObj);
			Commands->Destroy_Object(objectiveRepairTargetObj);
			Commands->Destroy_Object(centralTarget);
			Commands->Destroy_Object(centralTargetGround);
			for (int i = 0; i < exteriorTargets.Count(); i++)
				Commands->Destroy_Object(exteriorTargets[i]);

			for (int i = 0; i < objectiveVehicleAttackLocations.Count(); i++)
				Commands->Destroy_Object(objectiveVehicleAttackLocations[i]);

			// TODO: keep the locations saved so no need to recalculate after reviving, just place them
			exteriorTargets.Clear();
			exteriorTargetWatchpoints.Clear();
			if (M_Has_Timer(obj, this, 1))
				M_Stop_Timer2(obj, this, 1);

			AIBuildings.DeleteObj(obj);
			isDeleted = true;
		}
	}

	void Killed(GameObject* obj, GameObject* killer) override
	{
		Cleanup(obj);
	}

	void Detach(GameObject* obj) override
	{
		if (Exe != EXE_LEVELEDIT)
		{
			Cleanup(obj);
		}
		ScriptImpClass::Detach(obj);
	}
};
REGISTER_SCRIPT(MS_AI_Building, "DamagePercentToRepair=10:int,CustomWeakpointPresetName:string");


/*------------------------
AI Scripts
--------------------------*/

static bool DebugPathfindFailInited = false;

void Init_Pathfind_Debug()
{
	if (DebugPathfindFailInited)
		return;

	INIClass* config = Get_INI("bot_config.cfg");
	if (config)
	{
		DebugPathfindFailMessagesLiveEnabled = config->Get_Bool("Debug", "EnablePathfindFailLiveMessages", false);
		DebugPathfindFailMessagesFileEnabled = config->Get_Bool("Debug", "EnablePathfindFailFileMessages", false);
		Release_INI(config);
		DebugPathfindFailInited = true;
	}
}

/*!
* \brief Extended AI Behaviour
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* Copy of Danpaul88's Offensive AI, but it knows if it can deploy and can be defensive/engineer too.
* These AI units will strafe around in battle, trying to evade enemy shots.
* They can also properly use flying units and vehicles with limited turret rotation.
* The script also helps the AI a bit when colliding into objects they cannot see in the pathfind grid.
*/
class MS_AI_Behaviour : public dp88_AI_Tank_Offensive, public Pathfind_Help_Implementer
{
	template<typename> 
	friend class ScriptRegistrant;

	enum State {
		UNKNOWN,
		GOING_TO_VEHICLE,
		GOING_TO_REFILL,
		GOING_TO_SWITCH_OBJECTIVE,
		GOING_TO_RELOAD,
		GOING_TO_EXIT_VEHICLE_POINT,
		GOING_TO_RALLY_POINT,
		PATROLLING,
		DEAD,
	};

	State state;

	unsigned int objectiveType;
	JFW_Key_Hook_Base *deployScript;
	bool isDeployable;
	bool isDeployed;
	bool isDeployedStationary;
	bool isVTOL;
	bool isBomber;
	bool isJumpAllowed;
	bool isLimitedAmmoUnit;
	bool isAmmoEmptyNoteSent;
	// Prevent damage detection event spam
	bool isDamageDetectionEnabled;
	int totalAmmo;

	bool deploy_vs_infantry;
	bool deploy_vs_lightVehicle;
	bool deploy_vs_heavyVehicle;
	bool deploy_vs_VTOL;
	bool deploy_vs_building;

	float attackError;
	float flyingHeight;

	// Data from pathfind helper
	bool gettingAround;
	int pathfindFailCount;
	int objectiveNotFoundCount;

	int thinkCount;

	// Limiting wandering
	int wanderThinkCount;

	// Limiting evading
	int evadeThinkCount;
	Vector3 evadePosition;

	// Squishing!
	bool isSquisherUnit;
	bool isSquishableUnit;
	bool isSquishEvading;
	
	// Helping limited turret rotation vehicles
	bool isPrimaryAmmoTurning;
	bool isSecondaryAmmoTurning;
	bool isPrimaryAmmoGravityAffected;
	bool isSecondaryAmmoGravityAffected;
	bool isTurretRotationLimited;
	float turretTiltMin;
	float turretTiltMax;
	float turretTurnMin;
	float turretTurnMax;

	//int debugObjID = 0;

	// Ignoring primary objectives when doing other stuff
	ReferencerClass overridingTask;
	//int overridingTaskID;
	bool arrivedToOverridingTask;
	// Types: 0 = stationary | 1 = follow | 2 = cover | 7 = blindless attack
	int overridingTaskType;

	ReferencerClass patrolTask;
	//int patrolTaskID;

	BotPersonality *personality;

	DynamicVectorClass<int> ignoredObjectives;

	MS_AI_Behaviour()
		: personality(nullptr)
	{

	}

	~MS_AI_Behaviour()
	{
		REF_PTR_RELEASE(personality);
	}

	void Created(GameObject *obj) override
	{
		state = UNKNOWN;

		objectiveType = 1;
		deployScript = nullptr;
		isDeployable = false;
		isDeployed = false;
		isDeployedStationary = false;
		isVTOL = false;
		isBomber = false;
		isJumpAllowed = false;
		isLimitedAmmoUnit = false;
		isAmmoEmptyNoteSent = false;
		isDamageDetectionEnabled = true;
		totalAmmo = 0;

		deploy_vs_infantry = false;
		deploy_vs_lightVehicle = false;
		deploy_vs_heavyVehicle = false;
		deploy_vs_VTOL = false;
		deploy_vs_building = false;

		attackError = 0;
		flyingHeight = 25.0f;	

		gettingAround = false;
		pathfindFailCount = 0;
		objectiveNotFoundCount = 0;

		thinkCount = 0;

		wanderThinkCount = 0;

		evadeThinkCount = 0;
		evadePosition = Vector3();

		isSquisherUnit = false;
		isSquishableUnit = false;
		isSquishEvading = false;

		isPrimaryAmmoTurning = false;
		isSecondaryAmmoTurning = false;
		isPrimaryAmmoGravityAffected = false;
		isSecondaryAmmoGravityAffected = false;
		isTurretRotationLimited = false;
		turretTiltMin = 0.0f;
		turretTiltMax = 0.0f;
		turretTurnMin = 0.0f;
		turretTurnMax = 0.0f;

		overridingTask = nullptr;
		arrivedToOverridingTask = true;
		overridingTaskType = 0;

		patrolTask = nullptr;

		personality = nullptr;

		Remove_Stock_AI_Behaviour(obj);
		Init_Pathfind_Debug();

		objectiveType = (unsigned int)Get_Int_Parameter("ObjectiveType");
		if (objectiveType != dp88_AI_Objective::TYPE_OFFENSIVE && objectiveType != dp88_AI_Objective::TYPE_DEFENSIVE &&
			objectiveType != dp88_AI_Objective::TYPE_ENGINEERING)
		{
			objectiveType = dp88_AI_Objective::TYPE_OFFENSIVE;
		}

		if (GetTargetType(obj) == FLYING)
			isVTOL = true;

		attackError = obj->As_SmartGameObj()->Get_Weapon_Error();

		if (obj->As_VehicleGameObj())
		{
			isSquisherUnit = obj->As_VehicleGameObj()->Get_Squish_Velocity() < 10;
			turretTurnMin = obj->As_VehicleGameObj()->As_ArmedGameObj()->Get_Weapon_Turn_Min();
			turretTurnMax = obj->As_VehicleGameObj()->As_ArmedGameObj()->Get_Weapon_Turn_Max();

			if (turretTurnMin > DEG2RAD(-180) || turretTurnMax < DEG2RAD(180))
				isTurretRotationLimited = true;

			turretTiltMin = obj->As_VehicleGameObj()->As_ArmedGameObj()->Get_Weapon_Tilt_Min();
			turretTiltMax = obj->As_VehicleGameObj()->As_ArmedGameObj()->Get_Weapon_Tilt_Max();

			WeaponClass* weapon = obj->As_VehicleGameObj()->Get_Weapon();
			if (weapon)
			{
				if (auto ammoDef = weapon->PrimaryAmmoDefinition)
				{
					isPrimaryAmmoTurning = ammoDef->TurnRate > 0;
					isPrimaryAmmoGravityAffected = ammoDef->Gravity != 0;
				}
				if (auto ammoDef = weapon->SecondaryAmmoDefinition)
				{
					isSecondaryAmmoTurning = ammoDef->TurnRate > 0;
					isSecondaryAmmoGravityAffected = ammoDef->Gravity != 0;
				}
			}
		}
		else if (obj->As_SoldierGameObj()) {
			isSquishableUnit = !Is_Unsquishable(obj);
			obj->As_SoldierGameObj()->Set_Override_Muzzle_Direction(true);
		}

		// Overridden by MS_AI_Player_Controller logic if present
		personality = new BotPersonality(FreeRandom.Get_Int(3)); // Ref count 1

		Commands->Start_Timer(obj, this, 0.1f, TIMER_AI_POST_CREATED);
		dp88_AI_Tank_Offensive::Created(obj);
	}

	// -------------------------------------------------------------------------------------------------

	void Post_Created(GameObject *obj)
	{
		// Enable transitions for player-logic bot driven vehicles (but not for vehicle bots with no drivers inside)
		if (obj->As_VehicleGameObj() && obj->As_VehicleGameObj()->Get_Occupant_Count())
		{
			// Fix disabled transition set in dp88_customAI::Init 
			Commands->Enable_Vehicle_Transitions(obj, true);
		}
		if (auto deployParamsScript = (MS_AI_Marker_Deployable *)(Find_Script_On_Object(obj, "MS_AI_Marker_Deployable")))
		{
			deployScript = (JFW_Key_Hook_Base *)(Find_Script_On_Object(obj, deployParamsScript->Get_Deploy_Script_Name()));
			isDeployable = deployScript ? true : false;
			if (isDeployable)
			{
				// Init should deploy booleans
				deploy_vs_infantry = deployParamsScript->Get_Deploy_VS_Infantry();
				deploy_vs_lightVehicle = deployParamsScript->Get_Deploy_VS_Light_Vehicle();
				deploy_vs_heavyVehicle = deployParamsScript->Get_Deploy_VS_Heavy_Vehicle();
				deploy_vs_VTOL = deployParamsScript->Get_Deploy_VS_VTOL();
				deploy_vs_building = deployParamsScript->Get_Deploy_VS_Building();
				isDeployedStationary = deployParamsScript->Get_Deploy_Is_Stationary();
			}
		}

		isBomber = Is_Script_Attached(obj, "MS_AI_Marker_Bomber_Aircraft");
		isJumpAllowed = !Is_Script_Attached(obj, "MS_AI_Marker_No_Jump");
		isLimitedAmmoUnit = Is_Script_Attached(obj, "MS_AI_Marker_Refiller_Vehicle");

		// Fix bad AI definition settings
		const int weaponMaxRange = max(primary_maxRange, secondary_maxRange);
		const float defSightRange = obj->As_SmartGameObj()->Get_Definition().Get_Sight_Range();
		const float neededSightRange = max(max(weaponMaxRange + 5, defSightRange), 100); // +5: Sight range needs to be larger than the weapon max range
		if (defSightRange < neededSightRange)
			const_cast<SmartGameObjDef&>(obj->As_SmartGameObj()->Get_Definition()).Set_Sight_Range(neededSightRange);

		const float neededArc = obj->As_SoldierGameObj() ? 135.0f : 360.0f;
		const float currentArc = RAD2DEG(obj->As_SmartGameObj()->Get_Definition().Get_Sight_Arc());
		if (currentArc < neededArc)
			const_cast<SmartGameObjDef&>(obj->As_SmartGameObj()->Get_Definition()).Set_Sight_Arc(DEG2RAD(neededArc));

		Commands->Attach_Script(obj, "MS_AI_Pathfind_Helper", "");
	}

	// -------------------------------------------------------------------------------------------------

	// Engineers do react and shoot repairable target types, while others don't.
	float getBasePriority(GameObject *target) override
	{
		if (GetTargetType(target) == REPAIRABLE && objectiveType == dp88_AI_Objective::TYPE_ENGINEERING)
			return 1000000.0;
		else
			return dp88_customAI::getBasePriority(target);
	}

	// -------------------------------------------------------------------------------------------------

	float getPriority(GameObject *obj, GameObject* target) override
	{
		// Very high priority if blindless attack mode
		if (overridingTaskType == 7 && target && target == overridingTask)
			return 99999;

		return dp88_customAI::getPriority(obj, target);
	}

	// -------------------------------------------------------------------------------------------------

	// Return "should deploy" setting for current target type
	bool Should_Deploy(GameObject *target)
	{
		if (!isDeployable)
			return false;

		switch (GetTargetType(target))
		{
		case SOLDIER:
			return deploy_vs_infantry;
		case LIGHT_VEHICLE:
			return deploy_vs_lightVehicle;
		case HEAVY_VEHICLE:
			return deploy_vs_heavyVehicle;
		case FLYING:
			return deploy_vs_VTOL;
		case BUILDING:
			return deploy_vs_building;
		}
		return false;
	}

	// -------------------------------------------------------------------------------------------------

	bool Can_Tilt_Towards_Target(const bool isAimFree, const Matrix3D &turretTransform, GameObject* target)
	{
		if (isAimFree)
			return true;

		Vector3 targetPos = Commands->Get_Position(target);
		Vector3 adjustedTargetPos;
		Matrix3D::Inverse_Transform_Vector(turretTransform, targetPos, &adjustedTargetPos);
		adjustedTargetPos.X = Vector2(adjustedTargetPos.X, adjustedTargetPos.Y).Length();
		adjustedTargetPos.Y = 0;
		adjustedTargetPos.Normalize();
		const float angle = acos(adjustedTargetPos.X);
		const float signedAngle = adjustedTargetPos.Z >= 0 ? angle : -angle;
		const float finalAngle = signedAngle;

		return finalAngle < turretTiltMax && finalAngle > turretTiltMin;
	}

	// -------------------------------------------------------------------------------------------------

	// Air units: params.usePathfind must be false on VTOL units (or they use ground) + generate air pathfind on all maps! - Jerad
	void Enemy_Seen(GameObject *obj, GameObject *enemy) override
	{
		// Ignore all enemies if out of ammo or while going through doors
		if ((isLimitedAmmoUnit && !totalAmmo) || (obj->As_SoldierGameObj() && Is_Soldier_Busy(obj->As_SoldierGameObj())))
		{
			return;
		}

		// Attack immediately if this is the objective but not yet the target
		/*
		if (overridingTaskType == 7 && Commands->Get_ID(enemy) == overridingTaskID && Commands->Get_ID(enemy) != targetID)
		{
			targetID = Commands->Get_ID(enemy);
			AttackTarget(obj, enemy);
		}
		else
		*/
			dp88_AI_Tank_Offensive::Enemy_Seen(obj, enemy);
	}

	// -------------------------------------------------------------------------------------------------

	bool IsValidTarget(GameObject* obj, GameObject *target) override
	{
		if (obj->As_VehicleGameObj())
		{
			// Check if needed tilting angle is still available
			Matrix3D turretTransform;
			int boneIdx = obj->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Index("turret");
			if (boneIdx)
				turretTransform = obj->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Transform(boneIdx);
			else
				turretTransform = obj->As_PhysicalGameObj()->Peek_Model()->Get_Transform();

			const bool isAimFree = getPrimary(target) ? (isPrimaryAmmoTurning || isPrimaryAmmoGravityAffected) : (isSecondaryAmmoTurning || isSecondaryAmmoGravityAffected);
			if (!Can_Tilt_Towards_Target(isAimFree, turretTransform, target))
				return false;
		}

		return dp88_AI_Tank_Offensive::IsValidTarget(obj, target);
	}

	// -------------------------------------------------------------------------------------------------

	// Air units: params.usePathfind must be FALSE on VTOL units (or they use ground) + generate air pathfind on all maps! - Jerad
	void AttackTarget(GameObject *obj, GameObject *target) override
	{
		if (state == PATROLLING)
		{
			state = UNKNOWN;
		}

		ActionParamsStruct params;
		bool bPersueTarget = ShouldPursueTarget(obj, target);
		bool bModifyAction = false;

		// If we are going to persue the target then clear any existing actions
		if (bPersueTarget || !obj->As_SmartGameObj()->Get_Action()->Is_Acting() || 
			evadePosition != Vector3() || Commands->Get_Action_ID(obj) != ACTION_ID_ATTACK_TARGET)
		{
			// COPIED ORIGINAL FUNCTION, ONLY RESET IF ACTION DOESNT MATCH
			if (Commands->Get_Action_ID(obj) != ACTION_ID_ATTACK_TARGET)
				ResetAllActions(obj);
			params.Set_Basic(this, 90, ACTION_ID_ATTACK_TARGET);
		}
		// Modify only works for changing target while already attacking something.
		// Does not work for: 1. Modify movement and keep current target OR 2. Keep moving and ADD target while had none before
		else
		{
			Commands->Get_Action_Params(obj, params);
			bModifyAction = true;
		}

		// Setup parameters to track state
		// COPIED ORIGINAL FUNCTION, REMOVED VALUE SETTINGS HERE, Enemy_Seen HANDLES THEM
		//targetID = Commands->Get_ID(target);
		m_bTargetPrimaryFire = getPrimary(target);
		targetPriority = getPriority(obj, target);
		//targetLastSeen = (int)time(0);

		TargetType targetType = GetTargetType(target);

		// Setup the attack parameters
		// COPIED ORIGINAL FUNCTION, MODIFIED ATTACK ERROR FROM 0.0
		int maxRange = m_bTargetPrimaryFire ? primary_maxRange : secondary_maxRange;
		WeaponClass* weapon = obj->As_SmartGameObj()->Get_Weapon();
		// Use a worse attack error when throwing C4 so they dont end up all on top of eachother, better attack error vs building exteriors (helps longrange a lot)
		const float tempAttackError = (targetType == BUILDING && weapon) ? (weapon->Get_Style() == 0 ? DEG2RAD(200) : attackError / 5) : (attackError * personality->Get_Random(BotPersonality::ATTACK_ERROR_MULTIPLIER));
		// If C4 type, attack center of the c4, because the origin point is the lowlow part and shooting it would miss
		if (target->As_PhysicalGameObj() && (target->As_PhysicalGameObj()->As_C4GameObj() || target->As_PhysicalGameObj()->As_BeaconGameObj()))
		{
			auto &boundingBox = target->As_PhysicalGameObj()->Peek_Model()->Get_Bounding_Box();
			params.Set_Attack(boundingBox.Center, (float)(maxRange), tempAttackError, m_bTargetPrimaryFire);
		}
		else
			params.Set_Attack(target, (float)(maxRange), tempAttackError, m_bTargetPrimaryFire);

		
		params.AttackCheckBlocked = false;

		const Vector3 objPos = Commands->Get_Position(obj);
		float currentDistance2 = Vector3::Distance(objPos, Commands->Get_Position(target));
		// This makes units like Renegade MRLS/Stealth Tank work, their rockets turn after firing but the barrel cant point at their target
		params.AttackForceFire = currentDistance2 < maxRange ? (m_bTargetPrimaryFire ? isPrimaryAmmoTurning : isSecondaryAmmoTurning) : false;
		
		// COPIED ORIGINAL FUNCTION, MODIFIED FROM HERE
		// Soldiers crouch with some probability
		bool moveCrouched = false;
		// Ignore crouching during squishevasion, makes the soldier too slow to succeed
		if (bPersueTarget && obj->As_SoldierGameObj() && !isSquishEvading)
		{
			if (FreeRandom.Get_Float() < 0.1f && !isDeployable)
				moveCrouched = true;

			if (FreeRandom.Get_Float() < 0.5f)
				params.AttackCrouched = true;
		}

		int preferredRange = GetPreferredAttackRange(obj, target);

		// Evade-moving around
		if (evadePosition != Vector3())
		{
			params.Set_Movement(evadePosition, 1.0f, obj->As_SoldierGameObj() ? 0.5f : 2.0f, moveCrouched);
			if (obj->As_VehicleGameObj())
			{
				params.MoveBackup = Is_Behind_Cone(Get_Transform(obj), evadePosition, 0);
			}
		}
		// COPIED ORIGINAL FUNCTION, MODIFIED UNTIL HERE
		// Setup the movement parameters if we want to persue the target
		else if (bPersueTarget)
		{
			bool canSquishTarget = isSquisherUnit && target->As_SoldierGameObj() && !Is_Unsquishable(target);
			m_bMovingToTarget = (preferredRange < getDistance(obj, target) || canSquishTarget) && (!isDeployed || !isDeployedStationary);
			// Check if it's even possible to go there before pursuing, units might not be able to pathfind to target, and be stuck
			Vector3 targetPos = Commands->Get_Position(target);
			bool isTargetFlying = targetType == FLYING;
			// No corrections when target is flying or repairable
			if (!isTargetFlying)
			{
				Vector3 originalTargetPos = targetPos;
				bool isWithinPathfindGrid = Get_Closest_Pathfind_Spot_Size(originalTargetPos, 1.0f, &targetPos, 2.0f);
				if (!isWithinPathfindGrid)
				{
					if (!Get_Closest_Pathfind_Spot_Size(originalTargetPos, 15.0f, &targetPos, 2.0f))
					{
						// Default to not trying to move
						m_bMovingToTarget = false;
					}
					// Try to reach it without pathfind if out of range
					else if (targetType == REPAIRABLE && getDistance(obj, target) >= maxRange)
					{
						params.MovePathfind = false;
					}
				}
				else
				{
					// If pathfind sector is big, targetPos can be in a bad location, e.g. huge sector's center, further than the obj
					if (getDistance(obj, target) < Vector3::Distance(originalTargetPos, targetPos))
					{
						// Stupid result, defaulting to target location
						targetPos = originalTargetPos;
					}
				}
			}

			bool needsRotating = false;
			if (isTurretRotationLimited)
			{
				const float angle = Calculate_Relative_Facing_Diff(obj, targetPos);
				// Only need correction if the turret cannot rotate there, unless the ammo is turning, in which case a little offness is acceptable since the ammo turn corrects it after some distance
				auto &boundingBox = obj->As_PhysicalGameObj()->Peek_Model()->Get_Bounding_Box();
				const int radius = (int)(max(boundingBox.Extent.X, boundingBox.Extent.Y));
				if ((angle < turretTurnMin || angle > turretTurnMax) && (!(m_bTargetPrimaryFire ? isPrimaryAmmoTurning : isSecondaryAmmoTurning) || (abs(angle) > DEG2RAD(20) && Vector3::Distance(objPos, targetPos) > radius + 10.0f)))
					needsRotating = true;
			}

			if (targetType == REPAIRABLE)
			{
				params.AttackForceFire = true;
				if (auto objectiveObj = m_pCurrentObjective ? m_pCurrentObjective->GetGameObject() : nullptr)
					params.Set_Movement(Commands->Get_Position(objectiveObj), 1.0f, 1.25f);

				if (getDistance(obj, target) >= maxRange)
				{
					// targetPos = Commands->Get_Position(target);
					params.AttackRange = (float)(getDistance(obj, target) + 1);
				}
			}
			// COPIED ORIGINAL FUNCTION, ADDED REACHABLE CHECK
			else if (!needsRotating && (m_bMovingToTarget || pathfindFailCount > 4) && !isTargetFlying)
			{
				// COPIED ORIGINAL FUNCTION, MODIFIED FROM HERE
				// Choose random position around the position between me and target within preferred range
				// Get current distance and wanted distance to get the required ratio
				float ratio = preferredRange / currentDistance2;
				Vector3 pos = Lerp(targetPos, Commands->Get_Position(obj), ratio);
				// If building target type, make sure this pos is not something stupid unreachable (BAD_DEST when big vehicles get a position inside a building interior for example)
				if (targetType == BUILDING)
				{
					Get_Closest_Pathfind_Spot_Size(pos, min(20.0f, preferredRange), &pos, obj->As_SoldierGameObj() ? 0.5f : 5.0f);
					params.Set_Movement(pos, 1.0f, 1.0f, moveCrouched);
				}
				else
				{
					if (canSquishTarget) {
						params.Set_Movement(target, 1.0f, 0);
					}
					else if (!Get_Random_Pathfind_Spot(pos, (preferredRange / 2 - 2.0f), &pos) || !Can_Move_To(obj, pos) || preferredRange == 0 || isTurretRotationLimited || pathfindFailCount > 4)
					{
						params.Set_Movement(targetPos, 1.0f, abs((float)preferredRange - 1.5f), moveCrouched);
					}
					else
						params.Set_Movement(pos, 1.0f, 1.0f, moveCrouched);
				}
				// COPIED ORIGINAL FUNCTION, MODIFIED UNTIL HERE
			}
			// COPIED ORIGINAL FUNCTION, MODIFIED FROM HERE
			// "Turn towards" enemy if turret cannot turn on the enemy by moving closer to target
			else if (needsRotating && !isTargetFlying)
			{
				targetPos = Commands->Get_Position(target);

				// "Turn" towards target by actually going towards it
				const float currentDistance = Vector3::Distance(objPos, targetPos);
				float direction;
				if (currentDistance > preferredRange)
				{
					direction = 1.0f;
				}
				else
				{
					direction = -1.0f;
					params.MoveBackup = true;
				}
				Vector3 pos = objPos + (targetPos - objPos).Normalized() * direction * 1.0f;
				params.Set_Movement(pos, 1.0f, 0.5f);
			}
			// Change position around target randomly
			else if (obj->As_SoldierGameObj() && (!isDeployable || !isDeployedStationary))
			{
				Vector3 pos = Commands->Get_Position(target);
				if (Get_Random_Pathfind_Spot_Around_Position(obj, pos, 0, (float)preferredRange / 2, &pos))
					params.Set_Movement(pos, 1.0f, 5.0f);
				// Can't reach: continue to move towards objective (if one exists and not there yet)
				else if (nullptr != m_pCurrentObjective && m_pCurrentObjective->GetRange() < getDistance(obj, m_pCurrentObjective->GetGameObject()))
					params.Set_Movement(Commands->Get_Position(m_pCurrentObjective->GetGameObject()), 1.0f, (float)(m_pCurrentObjective->GetRange()));
			}
			// COPIED ORIGINAL FUNCTION, MODIFIED UNTIL HERE
		}
		// COPIED ORIGINAL FUNCTION, MODIFIED FROM HERE
		// Continue towards current objective, but shoot target
		else if (nullptr != m_pCurrentObjective)
		{
			// Setup the movement parameters
			int arrivalRange = m_pCurrentObjective->GetRange();
			m_bMovingToObjective = (arrivalRange + 1) < getDistance(obj, m_pCurrentObjective->GetGameObject());
			if (m_bMovingToObjective)
				params.Set_Movement(Commands->Get_Position(m_pCurrentObjective->GetGameObject()), 1.0f, (float)arrivalRange);
		}

		// VTOLs should never use ground pathfind
		if (isVTOL)
		{
			params.MovePathfind = false;
			params.MoveLocation.Z += flyingHeight;
			auto helperPos = objPos;
			helperPos.Z -= 15;
			if (isBomber && Vector3::Distance(helperPos, Commands->Get_Position(target)) <= maxRange)
			{
				params.AttackForceFire = true;
			}
		}
		// COPIED ORIGINAL FUNCTION, MODIFIED UNTIL HERE

		// Commence the action
		if (bModifyAction)
			Commands->Modify_Action(obj, params.ActionID, params, m_bMovingToTarget, true);
		else
		{
			/*
			if (m_bSplashInfantry && targetType == SOLDIER && target->As_SoldierGameObj()->Get_State() != HumanStateClass::SWIM)
			{
				Trace(TT::Tracing::TRACE_INFO, "Using splash damage against this target");
				Commands->Action_Attack_Splash(obj, params);
			}
			else
			*/
				Commands->Action_Attack(obj, params);
		}
	}

	// -------------------------------------------------------------------------------------------------

	dp88_AI_Objective* ChooseNewObjective(GameObject* obj) override
	{
		unsigned int objType = overridingTask ? (unsigned int)overridingTask->Get_ID() : objectiveType;
		dp88_AI_Objective* pObjective = dp88_AI_Objective::GetBestObjective(obj, objType, modifier_distance, ignoredObjectives);

		// Override patrolling with defensive objective
		if (pObjective && state == PATROLLING)
			state = UNKNOWN;

		return pObjective;
	}

	// -------------------------------------------------------------------------------------------------

	void GoToObjective(GameObject *obj, float speed = 1.0f) override
	{
		if (nullptr == m_pCurrentObjective)
			return;

		if (Go_To_Near_Objective(obj, m_pCurrentObjective))
			m_bMovingToObjective = true;

		// Override
		if (!m_bMovingToObjective || !overridingTask || overridingTaskType == 1 || arrivedToOverridingTask)
		{
			dp88_AI_Unit::GoToObjective(obj, Get_Movement_Speed(obj));
		}
	}

	// -------------------------------------------------------------------------------------------------

	void GoToPatrolPoint(GameObject *obj)
	{
		if (auto patrolObj = patrolTask.Get_Ptr())
		{
			auto objective = (dp88_AI_Objective *)(Find_Script_On_Object(patrolObj, "dp88_AI_Objective"));
			if (!objective)
				return;

			if (Go_To_Near_Objective(obj, objective))
				state = PATROLLING;
		}
	}

	// -------------------------------------------------------------------------------------------------

	/* Used by GoToObjective and GoToPatrolPoint */
	bool Go_To_Near_Objective(GameObject *obj, dp88_AI_Objective *objective)
	{
		if (isDeployable)
		{
			if (isDeployed && isDeployedStationary)
			{
				deployScript->KeyHook();
				//M_Console_Input("msg GoToObjective, undeploying");
			}
		}

		Vector3 pos;
		Vector3 objectivePos = Commands->Get_Position(objective->GetGameObject());
		float minDist = obj->As_SoldierGameObj() ? 1.0f : 2.0f;
		if (minDist > objective->GetRange())
			minDist = (float)objective->GetRange();

		if (Get_Random_Pathfind_Spot_Around_Position(obj, objectivePos, minDist, (float)objective->GetRange(), &pos))
		{
			ActionParamsStruct params;
			Commands->Get_Action_Params(obj, params);
			// Stop normal behaviour
			params.MoveObject = nullptr;
			params.WaypathID = 0;
			Commands->Action_Reset(obj, 101.0f);

			params.Set_Basic(this, 80, ACTION_ID_MOVE_TO_OBJECTIVE);
			if (isVTOL)
			{
				params.MovePathfind = false;
				pos.Z += flyingHeight;
			}
			params.Set_Movement(pos, Get_Movement_Speed(obj), 1.0f);
			Commands->Action_Goto(obj, params);
			return true;
		}
		return false;
	}

	// -------------------------------------------------------------------------------------------------

	/* Returns the speed that should be used while going to an objective */
	float Get_Movement_Speed(GameObject *obj)
	{
		float speed = 1.0f;
		// Fixes 4.x vehicle bots not driving at full speed potential
		if (obj->As_VehicleGameObj())
			speed = 100.0f;
		// Sprinting possibility for soldiers
		/*if (obj->As_SoldierGameObj() && personality->Get_Random(BotPersonality::SPRINT_CHANCE))
			speed = Commands->Get_Soldier_Sprint_Speed(obj);*/
		return speed;
	}

	// -------------------------------------------------------------------------------------------------

	bool ShouldPursueTarget(GameObject *obj, GameObject *target) override
	{
		// Don't chase if there's an objective AND objective is not this target
		if (overridingTask)
			return overridingTask == target;

		return dp88_AI_Unit::ShouldPursueTarget(obj, target);
	}

	// -------------------------------------------------------------------------------------------------

	void Custom(GameObject *obj, int type, int param, GameObject *sender) override
	{
		if (type == CUSTOM_DEPLOY_STATE_CHANGED && isDeployable)
		{
			isDeployed = param == 1;
		}
		else if (type == CUSTOM_AI_GO_TO_PATROL_POINT)
		{
			patrolTask = Commands->Find_Object(param);
			GoToPatrolPoint(obj);
		}
		else if (type == CUSTOM_AI_COOP_NEXT_RALLYPOINT || type == CUSTOM_AI_COOP_PLAYER_FOLLOW ||
			type == CUSTOM_AI_COOP_PLAYER_FOLLOW_COVER || type == CUSTOM_AI_COOP_PLAYER_DEFEND_BASE ||
			type == CUSTOM_AI_ORDER_MOVE || type == CUSTOM_AI_ORDER_ATTACK ||
			type == CUSTOM_AI_ORDER_DEFEND || type == CUSTOM_AI_GO_TO_REFILL || type == CUSTOM_AI_GO_SWITCH_OBJECTIVE ||
			type == CUSTOM_AI_GO_TO_RELOAD || type == CUSTOM_AI_GO_TO_EXIT_VEHICLE ||
			type == CUSTOM_AI_GO_TO_VEHICLE)
		{
			overridingTask = Commands->Find_Object(param);
			arrivedToOverridingTask = param == 0;

			if (type == CUSTOM_AI_GO_TO_VEHICLE)
				state = GOING_TO_VEHICLE;

			else if (type == CUSTOM_AI_GO_TO_REFILL)
				state = GOING_TO_REFILL;

			else if (type == CUSTOM_AI_GO_SWITCH_OBJECTIVE)
				state = GOING_TO_SWITCH_OBJECTIVE;

			else if (type == CUSTOM_AI_GO_TO_RELOAD)
				state = GOING_TO_RELOAD;

			else if (type == CUSTOM_AI_GO_TO_EXIT_VEHICLE)
				state = GOING_TO_EXIT_VEHICLE_POINT;

			// If task is 0, timer will catch the proper objective?
			else if (type == CUSTOM_AI_COOP_NEXT_RALLYPOINT && overridingTask)
				state = GOING_TO_RALLY_POINT;
			
			switch (type) {
			case CUSTOM_AI_COOP_NEXT_RALLYPOINT:
			case CUSTOM_AI_ORDER_MOVE:
			{
				//M_Console_Input("msg AI Beh: MOVE rcvd");
				overridingTaskType = 0;
				break;
			}
			case CUSTOM_AI_COOP_PLAYER_FOLLOW:
			case CUSTOM_AI_ORDER_DEFEND:
			case CUSTOM_AI_GO_TO_REFILL:
			case CUSTOM_AI_GO_SWITCH_OBJECTIVE:
			case CUSTOM_AI_GO_TO_RELOAD:
			case CUSTOM_AI_GO_TO_VEHICLE:
			case CUSTOM_AI_GO_TO_EXIT_VEHICLE:
			{
				//M_Console_Input("msg AI Beh: DEFEND rcvd");
				overridingTaskType = 1;
				break;
			}
			case CUSTOM_AI_COOP_PLAYER_FOLLOW_COVER:
			case CUSTOM_AI_COOP_PLAYER_DEFEND_BASE:
			{
				overridingTaskType = 2;
				break;
			}
			case CUSTOM_AI_ORDER_ATTACK:
			{
				if (auto taskTarget = overridingTask.Get_Ptr())
				{
					overridingTaskType = 7;
					state = UNKNOWN;
					// This makes sure infantry turn towards and attack the target if it's close by
					if (!M_Test_Raycast_Collision(Commands->Get_Position(obj) + Vector3(0, 0, 1.5f), Commands->Get_Position(taskTarget) + Vector3(0, 0, 1.5f), false))
						Enemy_Seen(obj, taskTarget);
				}
				break;
			}
			}
			ResetAllActions(obj);
			// Faster acting
			m_pCurrentObjective = ChooseNewObjective(obj);
			if (nullptr != m_pCurrentObjective)
				GoToObjective(obj);
		}
		else if (type == CUSTOM_AI_COOP_PLAYER_FOLLOW_STOP || type == CUSTOM_AI_COOP_STOP_SECONDARY_OBJECTIVE)
		{
			overridingTask = nullptr;
			m_target = nullptr;
			ResetAllActions(obj);
			m_pCurrentObjective = nullptr;
			state = UNKNOWN;
		}
		else
			dp88_AI_Tank_Offensive::Custom(obj, type, param, sender);
	}

	// -------------------------------------------------------------------------------------------------

	void Timer_Expired(GameObject* obj, int number) override
	{
		if (state == DEAD)
			return;
		
		if (number == TIMER_AI_POST_CREATED)
		{
			Post_Created(obj);
		}
		else if (number == TIMER_AI_THINK)
		{
			thinkTime = personality->Get_Random(BotPersonality::THINK_TIME);
			dp88_AI_Tank_Offensive::Timer_Expired(obj, number);

			thinkCount++;
			if (thinkCount > 9999)
				thinkCount = 0;
			else if (thinkCount % 30 == 0) {
				ignoredObjectives.Clear();
			}

			isDamageDetectionEnabled = true;

			if (state != GOING_TO_RELOAD && state != GOING_TO_REFILL)
			{
				Check_Repairables(obj);
				if (thinkCount % 2 == 0)
					Check_Enemy_Buildings(obj);
			}
			Handle_Range_Checks(obj);
			Handle_Refill_Checks(obj);

			if (!obj->As_SmartGameObj()->Get_Action()->Is_Acting() && state != GOING_TO_RELOAD && state != GOING_TO_REFILL && state != GOING_TO_SWITCH_OBJECTIVE)
				Handle_Idle_State(obj);
			else
				wanderThinkCount = 0;

			// Send check for alternate objectives request
			if (objectiveType != dp88_AI_Objective::TYPE_OFFENSIVE && !m_pCurrentObjective && thinkCount % 20 == 0)
			{
				Commands->Send_Custom_Event(obj, obj, CUSTOM_AI_CHECK_GO_HOME, 0, 0.0f);
				return;
			}

			/*if (obj->As_VehicleGameObj() && m_bMovingToObjective)
			{
			// TODOx: random side movements to break up the congaline
			}*/

			if (m_target)
				Handle_Target_Reaction_Think(obj, m_target);

			// Jumping
			/*if (Should_Jump(obj))
				Commands->Action_Jump(obj);*/
		}
		else
		{
			dp88_AI_Tank_Offensive::Timer_Expired(obj, number);
		}
	}

	// -------------------------------------------------------------------------------------------------

	void Damaged(GameObject *obj, GameObject *damager, float amount) override
	{
		// Trigger enemy seen if there is no blocking inbetween
		if (damager && isDamageDetectionEnabled && 
			!M_Test_Raycast_Collision(Commands->Get_Position(obj) + Vector3(0,0,1.5f), Commands->Get_Position(damager) + Vector3(0, 0, 1.5f), false))
		{ 
			Enemy_Seen(obj, damager);
			Notify_Neighbors_Enemy_Seen(obj, damager);
			isDamageDetectionEnabled = false;
		}

		// Trigger defensive evasion/jumping
		if (amount > 0 && !M_Has_Timer(obj, this, TIMER_AI_UNDER_ATTACK))
			Commands->Start_Timer(obj, this, 5.0f, TIMER_AI_UNDER_ATTACK);

		dp88_AI_Tank_Offensive::Damaged(obj, damager, amount);
	}

	// -------------------------------------------------------------------------------------------------

	void Handle_Idle_State(GameObject *obj)
	{
		// Patrolling for defensive bots: if nothing to do and not yet patrolling, do patrolling!
		if (nullptr == m_pCurrentObjective && (objectiveType == dp88_AI_Objective::TYPE_DEFENSIVE || objectiveType == dp88_AI_Objective::TYPE_OFFENSIVE) && 
			state != PATROLLING && wanderThinkCount > 0)
		{
			if (patrolTask)
				GoToPatrolPoint(obj);
			else
				Commands->Send_Custom_Event(obj, obj, CUSTOM_AI_REQUEST_PATROL_POINT, 0, 0.0f);
		}
		else if (state == PATROLLING)
			state = UNKNOWN;

		wanderThinkCount++;

		// Nothing happening? Do random walking/driving!
		if (wanderThinkCount > 5)
		{
			if (isDeployed && isDeployedStationary)
			{
				deployScript->KeyHook();
				//M_Console_Input("msg Wandering time, undeploying");
			}

			wanderThinkCount = 0;
			Vector3 pos = Commands->Get_Position(obj);
			float minDist = obj->As_SoldierGameObj() ? 3.0f : 8.0f;
			float maxDist = obj->As_SoldierGameObj() ? 9.0f : 35.0f;
			if (Get_Random_Pathfind_Spot_Around_Self(obj, minDist, maxDist, &pos))
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 1, ACTION_AI_RANDOM_MOVING);
				if (isVTOL)
				{
					params.MovePathfind = false;
					pos.Z += flyingHeight;
				}

				bool moveCrouched = false;
				float speed = 0.3f;
				if (obj->As_SoldierGameObj() && personality->Get_Random(BotPersonality::CROUCH_CHANCE_CALM))
				{
					moveCrouched = true;
					speed = 1.0f;
				}
				params.Set_Movement(pos, speed, 1.0f, moveCrouched);
				Commands->Action_Goto(obj, params);
			}
		}
	}

	// -------------------------------------------------------------------------------------------------

	void Handle_Target_Reaction_Think(GameObject *obj, GameObject *target)
	{
		// Deal with deployment activation and for stationary deployers, moving closer if needed
		bool deployOrderOverride = false;
		if (isDeployable)
		{
			int preferredRange = GetPreferredAttackRange(obj, target);
			float currentDistance = getDistance(obj, target);
			float maxRange = (float)((m_bTargetPrimaryFire) ? primary_maxRange : secondary_maxRange);

			// Deploy if deploy vs target is true, AND (deploy doesn't make it unmovable OR when it does, it must be within preferred range)
			bool shouldDeploy = Should_Deploy(target);
			bool isInPreferredDeployRange = !isDeployedStationary || currentDistance <= preferredRange;
			bool inRange = maxRange > currentDistance;

			if (!isDeployed && shouldDeploy)
			{
				if (isInPreferredDeployRange)
				{
					// Default
					evadePosition = Vector3();
					// Don't deploy when busy, retry later
					if (!(obj->As_SoldierGameObj() && Is_Soldier_Busy(obj->As_SoldierGameObj())))
						deployScript->KeyHook();
					// Commands->Start_Timer(obj, this, 0.8f, TIMER_AI_REQUEST_DEPLOY);
				}
				else
				{
					// Go closer, randomize location so it's not an easy botkill due to straight line movement
					Vector3 pos;
					float minDist = 0.5f;
					float maxDist = 10.0f;
					auto targetPos = Commands->Get_Position(target);
					if (Get_Random_Pathfind_Spot_Around_Position(obj, targetPos, minDist, maxDist, &pos))
						evadePosition = pos;
					else
						evadePosition = targetPos;
				}
				deployOrderOverride = true;
				AttackTarget(obj, target);
			}
			else if (isDeployed && (!shouldDeploy || !inRange))
			{
				deployScript->KeyHook();
				//M_Console_Input("msg Target not vs deployed mode, undeploying");
			}
		}

		TargetType targetType = GetTargetType(target);

		// Check weapon effectiveness/out of ammo
		if (obj->As_SoldierGameObj() && obj->As_SoldierGameObj()->Get_Weapon() && obj->As_SoldierGameObj()->Get_Weapon_Bag()->Get_Count() > 2)
		{
			WeaponClass* weapon = obj->As_SoldierGameObj()->Get_Weapon();
			if (!weapon->Get_Total_Rounds() || objectiveType == dp88_AI_Objective::TYPE_ENGINEERING || thinkCount % 2 == 0)
			{
				float scaledDamage = Calculate_Average_Scaled_Damage(weapon, target);

				bool shouldChangeWeapon = !weapon->Get_Total_Rounds() || (targetType != REPAIRABLE && scaledDamage <= 0) || (targetType == REPAIRABLE && (scaledDamage) >= 0);
				// Forced/occasional best DPS vs current target weapon checks
				if (shouldChangeWeapon || thinkCount % 4 == 0)
				{
					float distFromTarget = getDistance(obj, target);
					// Pure DPS of this weapon's primary ammo vs the target
					float damagePerSec = weapon->Get_Total_Rounds() ? Calculate_Weapon_DPS(weapon, scaledDamage) : 0;

					// Make the dps invalid to ensure a weapon switch when currently carrying C4 but target is not fitting
					// TODO: do not allow proximity type c4!
					bool isC4UsageAllowed = (targetType == BUILDING && m_pCurrentObjective && (m_pCurrentObjective->GetRange() + 1) > getDistance(obj, m_pCurrentObjective->GetGameObject()) && distFromTarget < 4) ||
											(targetType != BUILDING && targetType != SOLDIER && distFromTarget < 12);
					if (weapon->Get_Style() == 0 && !isC4UsageAllowed)
						damagePerSec = 0;

					WeaponClass* originalWeapon = weapon;
					WeaponClass* nextWeapon = weapon;

					auto weapon_bag = obj->As_SoldierGameObj()->Get_Weapon_Bag();
					int weapon_count = weapon_bag->Get_Count();
					for (int index = 0; index < weapon_count; index++)
					{
						weapon = weapon_bag->Peek_Weapon(index);
						if (!weapon || !weapon->Get_Total_Rounds() || (weapon->Get_Style() == 0 && !isC4UsageAllowed) || (weapon->PrimaryAmmoDefinition->Range <= 15 && distFromTarget >= 20))
							continue;

						scaledDamage = Calculate_Average_Scaled_Damage(weapon, target);

						float tempDamagePerSec = Calculate_Weapon_DPS(weapon, scaledDamage);
						if (targetType != REPAIRABLE || Get_Object_Type(target) != Get_Object_Type(obj) ? (tempDamagePerSec > damagePerSec) : (tempDamagePerSec < damagePerSec))
						{
							damagePerSec = tempDamagePerSec;
							nextWeapon = weapon;
						}
					}

					if (originalWeapon != nextWeapon)
					{
						Commands->Select_Weapon(obj, nextWeapon->Get_Name());
						// Recalculate ranges
						primary_maxRange = (int)nextWeapon->PrimaryAmmoDefinition->Range();
						if (nextWeapon->Get_Style() == 0 && target->As_VehicleGameObj())
						{
							auto model = target->As_VehicleGameObj()->Peek_Model();
							auto &obj_box = model->Get_Bounding_Box();
							// Expecting most vehicles to be square-ish sized, epic long vehicles will be problematic here.
							primary_maxRange += (int)(obj_box.Extent.X / 2 + obj_box.Extent.Y / 2) / 2;
						}
						m_primary_prefRange = primary_maxRange < 120 ? (int)(primary_maxRange / 2) : primary_maxRange - 10;
						if (primary_minRange > m_primary_prefRange)
							primary_minRange = m_primary_prefRange - 1;
						if (primary_minRange < 0)
							primary_minRange = 0;

						AttackTarget(obj, target);
						return;
					}
				}
			}
		}

		// Move around randomly every second (x second for vehs) during battle
		if (!gettingAround && !deployOrderOverride)
		{
			const float distFromTarget = getDistance(obj, target);
			const float maxRange = (float)((m_bTargetPrimaryFire) ? primary_maxRange : secondary_maxRange);
			const bool inRange = maxRange > distFromTarget;
			const bool inPrefRange = GetPreferredAttackRange(obj, target) >= distFromTarget;
			bool hasWeapon = false;

			if (target->As_SmartGameObj())
				hasWeapon = target->As_SmartGameObj()->Get_Weapon() ? true : false;

			// Set moving to false so the parent timer catches it and moves deploying units closer
			if (!inRange)
				m_bMovingToTarget = false;

			// Reset
			evadePosition = Vector3();
			if (obj->As_SoldierGameObj())
				isSquishEvading = false;

			const bool canSquishTarget = isSquisherUnit && target->As_SoldierGameObj() && !Is_Unsquishable(target);
			const bool targetCanSquishMe = isSquishableUnit && target->As_VehicleGameObj() && target->As_VehicleGameObj()->Get_Squish_Velocity() < 10;
			
			// Cancel evading in these cases
			if (!IsValidTarget(obj, target) || 
				// When not having to pursue the target, the evasion movement will not return but move the bot towards the objective.
				(ShouldPursueTarget(obj, target) && 
					((isDeployed && isDeployedStationary) || Should_Deploy(target) || canSquishTarget || 
						// Must check turret limit, otherwise e.g. the Renegade GDI MRLS does not rotate towards the target building and can keep hitting the wall/other dead buildings in the way
						(!M_Has_Timer(obj, this, TIMER_AI_UNDER_ATTACK) && !isTurretRotationLimited && (!hasWeapon || targetType == BUILDING || targetType == REPAIRABLE))
					)
				)
			)
				return;

			evadeThinkCount++;
			// Vehicles evade slower, limited turret vehicles correct their aim-position a bit faster
			if (ShouldPursueTarget(obj, target) &&
				(obj->As_VehicleGameObj() && evadeThinkCount < 5 && !(isTurretRotationLimited && evadeThinkCount > 2)))
				return;

			evadeThinkCount = 0;
			Vector3 pos;
			float minDist = obj->As_SoldierGameObj() ? 3.0f : 8.0f;
			float maxDist = obj->As_SoldierGameObj() ? 9.0f : 20.0f;

			// Limited turret rotation vehicles will calculate their next goto position inside AttackTarget instead of here, unless they have an overriding task (e.g. Follow a player, grouprush)
			if (obj->As_SoldierGameObj() || (state != GOING_TO_REFILL && state != GOING_TO_SWITCH_OBJECTIVE && (!isTurretRotationLimited || overridingTask))) {
				GameObject* objectiveObj = nullptr;
				if (m_pCurrentObjective != nullptr)
					objectiveObj = m_pCurrentObjective->GetGameObject();

				if (targetCanSquishMe && personality->Get_Random(BotPersonality::SQUISH_EVADE_CHANCE)) {
					const auto targetVeh = target->As_VehicleGameObj();

					const auto model = targetVeh->Peek_Model();
					const auto& obj_box = model->Get_Bounding_Box();
					// Expecting most vehicles to be square-ish sized, epic long ships will be problematic here.
					const float biggerSideHalf = max(obj_box.Extent.X, obj_box.Extent.Y);
					const float minEvadeDistance = biggerSideHalf + personality->Get_Random(BotPersonality::SQUISH_EVADE_DISTANCE);

					// Find an evade position around the enemy vehicle
					if (minEvadeDistance > distFromTarget)
					{

						// TODO: move to side of vehicle ASAP if not there yet, move further away if already there as far as preferred range is, but keep that to the vehicle's side too
						/*
						* If Enemy can squish me:
						*	1. Check if I'm epic close:
						*		- If yes, do evade and put objective on hold until we're safe or new target is around and done.
						*	2. If not close:
						*		- Check if target should be pursued (group rushing and negative priorities are all NO pursue):
						*			- If yes, move to preferred distance away from the enemy's closest side.
						*			- If not, move towards objective, aka do nothing.
						*/

						const Vector3 objPos = Commands->Get_Position(obj);
						const auto facing = DEG2RAD(Commands->Get_Facing(target));
						const bool isTooClose = distFromTarget < (obj_box.Extent.Y * 2);
						// const float XOffset = 0;//  distFromTarget < 3 * obj_box.Extent.Y ? 0 : (obj_box.Extent.Y * (Is_Behind_Cone(Get_Transform(target), objPos, 0) ? -1 : 1));
						// Get out of the way if too close, otherwise go out of dangerous proximity
						const float YOffset = obj_box.Extent.X + (isTooClose ? obj_box.Extent.Y * 3.0f : minEvadeDistance) + 3;

						Vector3 helpPos1 = Commands->Get_Position(target);
						Vector3 helpPos2 = Commands->Get_Position(target);
						const float facingCos = cos(facing);
						const float facingSin = sin(facing);
						//const float XOffsetCos = XOffset * facingCos;
						//const float XOffsetSin = XOffset * facingSin;
						const float YOffsetCos = YOffset * facingCos;
						const float YOffsetSin = YOffset * facingSin;
						//helpPos1.X += XOffsetCos;
						//helpPos1.Y += XOffsetSin;
						helpPos1.Y += YOffsetCos;
						helpPos1.X -= YOffsetSin;

						//helpPos2.X += XOffsetCos;
						//helpPos2.Y += XOffsetSin;
						helpPos2.Y += -YOffsetCos;
						helpPos2.X -= -YOffsetSin;

						const bool isWithinPathfindGrid1 = Get_Closest_Pathfind_Spot_Size(Vector3(helpPos1), 1.0f, &helpPos1, 2.0f);
						const bool isWithinPathfindGrid2 = Get_Closest_Pathfind_Spot_Size(Vector3(helpPos2), 1.0f, &helpPos2, 2.0f);
						if (isWithinPathfindGrid1 && isWithinPathfindGrid2)
						{
							// Choose the closer one
							evadePosition = Vector3::Distance_Squared(objPos, helpPos1) < Vector3::Distance_Squared(objPos, helpPos2) ? helpPos1 : helpPos2;
						}
						else
						{
							evadePosition = isWithinPathfindGrid1 ? helpPos1 : helpPos2;
						}
						if (isWithinPathfindGrid1 || isWithinPathfindGrid2)
						{
							isSquishEvading = true;
							Get_Random_Pathfind_Spot_Around_Position(obj, evadePosition, 0.5f, 2.0f, &evadePosition);

							//Commands->Destroy_Object(Commands->Find_Object(debugObjID));
							//const auto targetObj = Commands->Create_Object("Invisible_Object", evadePosition);
							//Commands->Set_Model(targetObj, isTooClose ? "o_davesarrow" : "o_flagatt");
							//debugObjID = targetObj->Get_ID();
						}
					}
				}

				if (!isSquishEvading)
				{
					if ((ShouldPursueTarget(obj, target) || M_Has_Timer(obj, this, TIMER_AI_UNDER_ATTACK)) && (targetType != BUILDING ? inPrefRange : inRange) && Get_Random_Pathfind_Spot_Around_Self(obj, minDist, maxDist, &pos, targetType != BUILDING ? target : nullptr))
						evadePosition = pos;
					else if (objectiveObj && (m_pCurrentObjective->GetRange() + 1) < getDistance(obj, m_pCurrentObjective->GetGameObject()))
					{
						minDist = 0.5f;
						maxDist = (float)m_pCurrentObjective->GetRange();
						const auto objectivePos = Commands->Get_Position(objectiveObj);
						if (Get_Random_Pathfind_Spot_Around_Position(obj, objectivePos, minDist, maxDist, &pos))
							evadePosition = pos;
					}
				}
			}

			AttackTarget(obj, target);
		}
	}

	// -------------------------------------------------------------------------------------------------

	void Handle_Range_Checks(GameObject *obj)
	{
		if (overridingTask)
		{
			auto taskObj = overridingTask.Get_Ptr();
			// Check the current objective, if any, is still valid
			if (nullptr == m_pCurrentObjective || (overridingTaskType == 7 && taskObj && (
					// Target soldier entered a vehicle
					(taskObj->As_SoldierGameObj() && Get_Vehicle(taskObj)) ||
					// OR attacking a vehicle but its driver has left it and it's not the harvester!
					(taskObj->As_VehicleGameObj() && !taskObj->As_VehicleGameObj()->Get_Occupant_Count() && !Is_Harvester(taskObj)))
			))
			{
				objectiveNotFoundCount++;
				//M_Console_Input("msg Objective not found!");
				if (objectiveNotFoundCount >= FreeRandom.Get_Int(1,3))
				{
					//M_Console_Input("msg Objective is invalid!");
					Commands->Send_Custom_Event(obj, obj, CUSTOM_AI_COOP_STOP_SECONDARY_OBJECTIVE, 0, 0.0f);
				}
				return;
			}
			else
				objectiveNotFoundCount = 0;

			if (state == GOING_TO_VEHICLE)
			{
				if (auto objectiveGO = m_pCurrentObjective->GetGameObject())
				{
					if (auto vehicle = objectiveGO->As_VehicleGameObj())
					{
						auto model = vehicle->Peek_Model();
						auto &obj_box = model->Get_Bounding_Box();
						// Expecting most vehicles to be square-ish sized, epic long ships will be problematic here.
						float distance = (obj_box.Extent.X / 2 + obj_box.Extent.Y / 2) / 2 + 5;

						if (distance > getDistance(obj, objectiveGO))
						{
							Commands->Send_Custom_Event(obj, obj, CUSTOM_AI_ARRIVED_TO_VEHICLE, 1, 0.0f);
						}
					}
				}
			}
			else if (state == GOING_TO_REFILL || state == GOING_TO_SWITCH_OBJECTIVE || state == GOING_TO_EXIT_VEHICLE_POINT)
			{
				if (auto objectiveGO = m_pCurrentObjective->GetGameObject())
				{
					if (m_pCurrentObjective->GetRange() + 1 >= getDistance(obj, objectiveGO))
					{
						auto custom = state == GOING_TO_REFILL || state == GOING_TO_SWITCH_OBJECTIVE ? CUSTOM_AI_ARRIVED_TO_REFILL : CUSTOM_AI_ARRIVED_TO_EXIT_VEHICLE;
						Commands->Send_Custom_Event(obj, obj, custom, 0, 0.0f);
					}
				}
			}
			else if (state == GOING_TO_RALLY_POINT && !arrivedToOverridingTask)
			{
				if (auto objectiveGO = m_pCurrentObjective->GetGameObject())
				{
					if (m_pCurrentObjective->GetRange() + 10 + (isVTOL ? flyingHeight : 0) >= getDistance(obj, objectiveGO))
					{
						arrivedToOverridingTask = true;
						Commands->Send_Custom_Event(obj, obj, CUSTOM_AI_COOP_ARRIVED_TO_RALLYPOINT, 0, 0.0f);
					}
				}
			}
		}
		// Check if arrived to next patrol point and ask for next
		else if (state == PATROLLING)
		{
			if (auto patrolObj = patrolTask.Get_Ptr())
			{
				auto objective = (dp88_AI_Objective *)(Find_Script_On_Object(patrolObj, "dp88_AI_Objective"));
				if (objective && objective->GetRange() + 4 + (isVTOL ? flyingHeight : 0) >= getDistance(obj, patrolObj))
				{
					Commands->Send_Custom_Event(obj, obj, CUSTOM_AI_REQUEST_PATROL_POINT, patrolTask->Get_ID(), 0.0f);
				}
			}
		}
	}

	// -------------------------------------------------------------------------------------------------

	void Handle_Refill_Checks(GameObject *obj)
	{
		if (thinkCount % 4 == 0)
		{
			if (!overridingTask)
			{
				Commands->Send_Custom_Event(obj, obj, CUSTOM_AI_CHECK_REPAIR_NEEDED, 0, 0.0f);
			}
			else if (state == GOING_TO_REFILL)
			{
				// No money
				if (obj->As_VehicleGameObj() &&
					MS_AccessHelper::Get_Money(obj) < Get_Team_Cost(Commands->Get_Preset_ID(obj), Get_Object_Type(obj)) * 0.25f)
				{
					Commands->Send_Custom_Event(obj, obj, CUSTOM_AI_COOP_STOP_SECONDARY_OBJECTIVE, 0, 0.0f);
				}
				else
				{
					const float currentHitp = Commands->Get_Health(obj) + Commands->Get_Shield_Strength(obj);
					const float maxHitp = Commands->Get_Max_Health(obj) + Commands->Get_Max_Shield_Strength(obj);
					if (currentHitp == maxHitp)
					{
						Commands->Send_Custom_Event(obj, obj, CUSTOM_AI_REFILL_COMPLETE, 0, 0.0f);
						Commands->Send_Custom_Event(obj, obj, CUSTOM_AI_COOP_STOP_SECONDARY_OBJECTIVE, 0, 0.0f);
					}
				}
			}
		}

		// GOING_TO_REFILL is checked here so it doesnt override a refilling order! +Check vehicle/weapon, empty falling longbows can crash the game
		if (isLimitedAmmoUnit && state != GOING_TO_REFILL && obj->As_VehicleGameObj() && obj->As_VehicleGameObj()->Get_Weapon())
		{
			totalAmmo = obj->As_VehicleGameObj()->Get_Weapon()->Get_Total_Rounds();
			bool isAmmoFull = obj->As_VehicleGameObj()->Get_Weapon()->Is_Ammo_Maxed();

			if (!isAmmoEmptyNoteSent && totalAmmo == 0)
			{
				Commands->Send_Custom_Event(obj, obj, CUSTOM_AI_VEHICLE_AMMO_EMPTY, obj->Get_ID(), 0.0f);
				isAmmoEmptyNoteSent = true;
			}
			else if (isAmmoEmptyNoteSent && isAmmoFull)
			{
				Commands->Send_Custom_Event(obj, obj, CUSTOM_AI_REFILL_COMPLETE, 0, 0.0f);
				Commands->Send_Custom_Event(obj, obj, CUSTOM_AI_COOP_STOP_SECONDARY_OBJECTIVE, 0, 0.0f);
				isAmmoEmptyNoteSent = false;
			}
		}
	}

	// -------------------------------------------------------------------------------------------------

	void Check_Repairables(GameObject* obj)
	{
		if (objectiveType != dp88_AI_Objective::TYPE_ENGINEERING || !obj->As_SoldierGameObj())
			return;

		// M_Console_Input("msg Checking repairables...");

		// Find Beacons
		for (auto node = GameObjManager::BeaconGameObjList.Head(); node; node = node->Next())
		{
			auto target = node->Data();
			// Skip friendly beacons and ones that don't have an objective on them (not a threat)
			if (Get_Object_Type(obj) == Get_Object_Type(target) || !Is_Script_Attached(target, "dp88_AI_Marker_Repairable"))
				continue;
			if (obj->As_SmartGameObj()->Is_Obj_Visible(target))
			{
				Enemy_Seen(obj, target);
				// Ignore all other targets
				return;
			}
		}

		// Find C4s
		for (auto node = GameObjManager::C4GameObjList.Head(); node; node = node->Next())
		{
			auto target = node->Data();
			// Skip friendly C4s and ones that don't have an objective on them (not a threat)
			if (Get_Object_Type(obj) == Get_Object_Type(target) || !Is_Script_Attached(target, "dp88_AI_Marker_Repairable"))
				continue;
			if (obj->As_SmartGameObj()->Is_Obj_Visible(target))
				Enemy_Seen(obj, target);
		}

		for (int index = 0; index < AIBuildings.Count(); index++)
		{
			auto building = AIBuildings[index];
			auto bldScript = (MS_AI_Building *)(Find_Script_On_Object(building, "MS_AI_Building"));

			if (!bldScript || (building->As_BuildingGameObj() && building->As_BuildingGameObj()->Is_Destroyed()))
				continue;

			auto target = bldScript->objectiveRepairTargetObj;

			if (target && GetTargetType(target) == REPAIRABLE && Get_Object_Type(obj) == Get_Object_Type(target))
			{
				// Only check friendly repairables
				/*if (strcmp("Invisible_Object", Commands->Get_Preset_Name(target)) == 0)
					M_Console_Input("msg Found my boy");*/

				auto script = (dp88_AI_Marker_Repairable*)(Find_Script_On_Object(target, "dp88_AI_Marker_Repairable"));
				float extraRange = GetPreferredAttackRange(obj, target) > 2.0f ? -1.0f : 1.0f;
				/*if (script)
				{
					StringClass buff;
					buff.Format("me from target: %.2f, must be smallerthan: %.2f, target from pathfind: %.2f, prefrange: %d, and +/- 1.0 by moonsense", getDistance(obj, target), script->Get_Distance_From_Pathfind() + GetPreferredAttackRange(obj, target) + extraRange,
						script->Get_Distance_From_Pathfind(), GetPreferredAttackRange(obj, target));
					Send_Message(MS_RGB_DEBUG, buff.Peek_Buffer());
				}*/

				if (script && getDistance(obj, target) < script->Get_Distance_From_Pathfind() + GetPreferredAttackRange(obj, target) + extraRange)
				{
					// M_Console_Input("msg Found one, calling enemy seen");
					Enemy_Seen(obj, target);
				}
			}
		}
	}

	// -------------------------------------------------------------------------------------------------

	void Check_Enemy_Buildings(GameObject* obj)
	{
		if (objectiveType == dp88_AI_Objective::TYPE_ENGINEERING)
			return;

		// M_Console_Input("msg Checking enemy buildings...");

		auto weapon = obj->As_SmartGameObj()->Get_Weapon();
		if (!weapon)
			return;

		bool isLimitedAmmoInfantry = obj->As_SoldierGameObj() && weapon->Get_Total_Rounds() != -1;
		Vector3 objPos = obj->As_SmartGameObj()->Get_Bullseye_Position() + Vector3(0, 0, 0.3f);
		Vector3 muzzlePos = objPos;
		Matrix3D turretTransform;
		// Raycast from the muzzlebone if this is a vehicle to make sure raycasting and actual firing matches better
		if (obj->As_VehicleGameObj())
		{
			int boneIdx = obj->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Index("muzzlea0");
			if (boneIdx)
				muzzlePos = obj->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Transform(boneIdx).Get_Translation();

			boneIdx = obj->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Index("turret");
			if (boneIdx)
			{
				turretTransform = obj->As_PhysicalGameObj()->Peek_Model()->Get_Bone_Transform(boneIdx);
				objPos = turretTransform.Get_Translation();
			}
			else
			{
				turretTransform = obj->As_PhysicalGameObj()->Peek_Model()->Get_Transform();
				objPos = obj->As_SmartGameObj()->Get_Bullseye_Position();
			}
		}

		for (int index = 0; index < AIBuildings.Count(); index++)
		{
			auto target = AIBuildings[index];
			if (target && Get_Object_Type(obj) != Get_Object_Type(target) && GetTargetType(target) == BUILDING)
			{
				auto bldScript = (MS_AI_Building *)(Find_Script_On_Object(target, "MS_AI_Building"));

				// Buildings with no mesh, only bld controller and destroyed buildings: should be skipped
				if (!bldScript || (target->As_BuildingGameObj() && target->As_BuildingGameObj()->Is_Destroyed()))
					continue;

				// StringClass msg;

				auto &boundingBox = target->As_BuildingGameObj() ? target->As_BuildingGameObj()->Get_Bounds() : target->As_PhysicalGameObj()->Peek_Model()->Get_Bounding_Box();
				const int radius = (int)(max(boundingBox.Extent.X, boundingBox.Extent.Y));
				const float weaponRange = float(getPrimary(target) ? primary_maxRange : secondary_maxRange);

				// Buildings too far away: should be skipped
				float distFromBuildingEdge = Vector3::Distance(objPos, boundingBox.Center) - radius + 2.0f;
				if (distFromBuildingEdge > weaponRange)
					continue;

				// Don't do tilting calculations for infantry and vehicles that use ammo with gravity or turning
				const bool isAimFree = obj->As_VehicleGameObj() ? (getPrimary(target) ? (isPrimaryAmmoTurning || isPrimaryAmmoGravityAffected) : (isSecondaryAmmoTurning || isSecondaryAmmoGravityAffected)) : true;

				if (!target->As_BuildingGameObj() && obj->As_SmartGameObj()->Is_Obj_Visible(target->As_PhysicalGameObj()) && Can_Tilt_Towards_Target(isAimFree, turretTransform, target))
				{
					// msg.Format("msg %s: Can naturally be seen, no extra checks.", Commands->Get_Preset_Name(target));
					// M_Console_Input(msg);
					Enemy_Seen(obj, target);
					continue;
				}

				// We are in range, check the targets

				// Check if MCT is in sight range (if exists) and stop any further checks if yes as it's the best target to attack
				if (obj->As_SoldierGameObj() && bldScript->objectiveMCTAttackTargetObj)
				{
					if (!M_Test_Raycast_Collision(objPos, bldScript->mctObjectiveLocation, false) && (
						(isLimitedAmmoInfantry && m_pCurrentObjective && getDistance(obj, m_pCurrentObjective->GetGameObject()) < 3) ||
						(!isLimitedAmmoInfantry && getDistance(obj, target) < weaponRange)
					))
					{
						// msg.Format("msg %s: Found weakpoint, calling enemy seen", Commands->Get_Preset_Name(target), bldScript->exteriorTargetWatchpoints.Count());
						// M_Console_Input(msg);
						Enemy_Seen(obj, bldScript->objectiveMCTAttackTargetObj);
						return;
					}
				}

				// Limited ammo infantry should only shoot the MCT, others are fine to attack from outside too
				if (isLimitedAmmoInfantry)
					continue;

				// Do 1 simple raycast first towards the building boundary center on same Z level as the bot first, and stop any further checks if it succeeds as attacking here should be safe (reduce the number of raycasts)
				Vector3 centralTargetPos = Commands->Get_Position(bldScript->centralTargetGround);
				Vector3 checkHitPos = (Vector3::Distance_Squared(objPos, centralTargetPos) < weaponRange * weaponRange) ? centralTargetPos : (objPos + (centralTargetPos - objPos).Normalized() * (float)weaponRange);
				if (M_Test_Raycast_Collision(objPos, checkHitPos, false, &checkHitPos, target) && Can_Tilt_Towards_Target(isAimFree, turretTransform, bldScript->centralTargetGround))
				{
					// msg.Format("msg %s: Raycast found lower center, calling enemy seen", Commands->Get_Preset_Name(target), bldScript->exteriorTargetWatchpoints.Count());
					// M_Console_Input(msg);
					Enemy_Seen(obj, bldScript->centralTargetGround);
					continue;
				}
				centralTargetPos = Commands->Get_Position(bldScript->centralTarget);
				// Do 1 simple raycast then towards the building boundary center, and stop any further checks if it succeeds as attacking here should be safe (reduce the number of raycasts)
				checkHitPos = (Vector3::Distance_Squared(objPos, centralTargetPos) < weaponRange * weaponRange) ? centralTargetPos : (objPos + (centralTargetPos - objPos).Normalized() * (float)weaponRange);
				if (M_Test_Raycast_Collision(objPos, checkHitPos, false, &checkHitPos, target) && Can_Tilt_Towards_Target(isAimFree, turretTransform, bldScript->centralTarget))
				{
					// msg.Format("msg %s: Raycast found center, calling enemy seen", Commands->Get_Preset_Name(target), bldScript->exteriorTargetWatchpoints.Count());
					// M_Console_Input(msg);
					Enemy_Seen(obj, bldScript->centralTarget);
					continue;
				}

				// Long range units: do 8 raycasts and stop trying if all failed, prevents epic amount of unnecessary raycastings
				if (distFromBuildingEdge > 100 && bldScript->exteriorTargetWatchpoints.Count() > 12)
				{
					const float sideOffset = 2.0f;
					DynamicVectorClass<Vector3> bldCorners;
					bldCorners.Add(boundingBox.Center + Vector3( boundingBox.Extent.X + sideOffset, 0, 0));
					bldCorners.Add(boundingBox.Center + Vector3(-boundingBox.Extent.X - sideOffset, 0, 0));
					bldCorners.Add(boundingBox.Center + Vector3( 0, -boundingBox.Extent.Y - sideOffset, 0));
					bldCorners.Add(boundingBox.Center + Vector3( 0,  boundingBox.Extent.Y + sideOffset, 0));
					bldCorners.Add(boundingBox.Center + Vector3( boundingBox.Extent.X + sideOffset,  boundingBox.Extent.Y + sideOffset, 0));
					bldCorners.Add(boundingBox.Center + Vector3(-boundingBox.Extent.X - sideOffset, -boundingBox.Extent.Y - sideOffset, 0));
					bldCorners.Add(boundingBox.Center + Vector3( boundingBox.Extent.X + sideOffset, -boundingBox.Extent.Y - sideOffset, 0));
					bldCorners.Add(boundingBox.Center + Vector3(-boundingBox.Extent.X - sideOffset,  boundingBox.Extent.Y + sideOffset, 0));
					bool edgesVisible = false;
					for (int i = 0; i < bldCorners.Count(); i++)
					{
						Vector3 &point = bldCorners[i];
						if (!M_Test_Raycast_Collision(objPos, point, false))
						{
							edgesVisible = true;
							// msg.Format("msg %s: Long range: a corner is visible, checking targets...", Commands->Get_Preset_Name(target), bldScript->exteriorTargetWatchpoints.Count());
							// M_Console_Input(msg);
							break;
						}
					}
					if (!edgesVisible)
					{
						// msg.Format("msg %s: Long range: NO corner is visible, aborting checks...", Commands->Get_Preset_Name(target), bldScript->exteriorTargetWatchpoints.Count());
						// M_Console_Input(msg);
						continue;
					}
				}

				// Order targets by distance first to prevent a lot of unnecessary raycasts
				DynamicVectorClass<Vector3> watchpointDistanceOrder = Order_Points_By_Distance(bldScript->exteriorTargetWatchpoints, objPos, weaponRange - 1);

				// int raycastCount = 0;
				// bool found = false;
				for (int i = 0; i < watchpointDistanceOrder.Count(); i++)
				{
					// raycastCount++;
					Vector3 &point = watchpointDistanceOrder[i];
					if (!M_Test_Raycast_Collision(muzzlePos, point, false))
					{
						const int targetArrayIndex = bldScript->exteriorTargetWatchpoints.ID(point);
						ReferencerClass &extTarget = bldScript->exteriorTargets[targetArrayIndex];
						if (!Can_Tilt_Towards_Target(isAimFree, turretTransform, extTarget))
							continue;
						// msg.Format("msg %s: Cheap attempts failed, FOUND a target, calling enemy seen (raycasts: %d / %d) (autoindex: %d)", Commands->Get_Preset_Name(target), raycastCount, bldScript->exteriorTargetWatchpoints.Count(), targetArrayIndex);
						// M_Console_Input(msg);
						Enemy_Seen(obj, extTarget);
						// found = true;
						break;
					}
				}
			}
		}
	}

	// -------------------------------------------------------------------------------------------------

	bool Should_Jump(GameObject *obj)
	{
		if (!obj->As_SoldierGameObj() || !isJumpAllowed || Is_Soldier_Busy(obj->As_SoldierGameObj()))
		{
			return false;
		}

		if (m_target && personality->Get_Random(BotPersonality::JUMP_CHANCE_COMBAT))
		{
			// Check target type here, engineers should not jump while "battling" repair targets...
			if (m_target && GetTargetType(m_target) != REPAIRABLE)
			{
				return true;
			}
		}
		else if (!m_target && m_bMovingToObjective && personality->Get_Random(BotPersonality::JUMP_CHANCE_REGULAR))
		{
			return true;
		}
		
		return false;
	}

	// -------------------------------------------------------------------------------------------------

	void Notify_Neighbors_Enemy_Seen(GameObject *obj, GameObject *enemy)
	{
		SList<GameObject> list;
		Find_All_Objects_With_Script_By_Distance("MS_AI_Behaviour", list, Commands->Get_Position(obj));
		for (auto x = list.Head(); x != nullptr; x = x->Next())
		{
			GameObject* neighbor = x->Data();
			// Neighbors already ordered by distance, can stop the cycle here
			if (getDistance(obj, neighbor) > 20)
				return;

			// Notify friends only, skip self
			if (Get_Object_Type(obj) != Get_Object_Type(neighbor) || Commands->Get_ID(obj) == Commands->Get_ID(neighbor))
				continue;

			auto behaviourScript = (MS_AI_Behaviour *)(Find_Script_On_Object(neighbor, "MS_AI_Behaviour"));
			if (behaviourScript)
				behaviourScript->On_Notification_Enemy_Seen(neighbor, enemy);
		}
	}

	// -------------------------------------------------------------------------------------------------

	void On_Notification_Enemy_Seen(GameObject *obj, GameObject *enemy)
	{
		// Trigger enemy seen if there is no blocking inbetween
		if (enemy && 
			!M_Test_Raycast_Collision(Commands->Get_Position(obj) + Vector3(0, 0, 1.5f), Commands->Get_Position(enemy) + Vector3(0, 0, 1.5f), false))
		{
			Enemy_Seen(obj, enemy);
		}
	}

	// -------------------------------------------------------------------------------------------------

	void Detach(GameObject* obj) override
	{
		if (Exe != EXE_LEVELEDIT)
		{
			// This should help make it possible to activate/deactivate bot behaviour whenever wanted
			ResetAllActions(obj);
			Remove_Script(obj, "MS_AI_Pathfind_Helper");
		}
		dp88_AI_Tank_Offensive::Detach(obj);
	}

	// -------------------------------------------------------------------------------------------------
	
	void Killed(GameObject* obj, GameObject* killer) override
	{
		state = DEAD;
	}

	// -------------------------------------------------------------------------------------------------

	void Destroyed(GameObject* obj) override
	{
		state = DEAD;
		//Commands->Destroy_Object(Commands->Find_Object(debugObjID));
	}

	// -------------------------------------------------------------------------------------------------

	void Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason) override
	{
		if (action_id == ACTION_ID_MOVE_TO_OBJECTIVE)
		{
			if (obj->As_SoldierGameObj() && state != GOING_TO_VEHICLE && 
				(complete_reason == ACTION_COMPLETE_PATH_BAD_DEST || complete_reason == ACTION_COMPLETE_PATH_BAD_START) && m_pCurrentObjective && m_pCurrentObjective->GetGameObject())
			{
				GameObject *objectiveObj = m_pCurrentObjective->GetGameObject();
				Vector3 objectiveLocation = Commands->Get_Position(objectiveObj);
				StringClass botName = obj->As_SoldierGameObj()->Get_Bot_Tag();

				if (overridingTask) {
					overridingTask = nullptr;
					if (DebugPathfindFailMessagesLiveEnabled || DebugPathfindFailMessagesFileEnabled) {
						StringClass debug;
						debug.Format("%s(%d): cannot reach overriding task: %s (%s-%d) at location: %.2f / %.2f / %.2f (%.2f dist, %.5f prio)", botName.Peek_Buffer(), Get_Object_Type(obj), m_pCurrentObjective->GetDebugTag(), Commands->Get_Preset_Name(objectiveObj), objectiveObj->Get_ID(), objectiveLocation.X, objectiveLocation.Y, objectiveLocation.Z, getDistance(obj, objectiveObj), m_pCurrentObjective->GetPriority(obj, modifier_distance));
						Print_Debug(debug);
					}
				}
				else {
					ignoredObjectives.Add(objectiveObj->Get_ID());
					auto objectiveCount = dp88_AI_Objective::CountUnitObjectives(Get_Object_Type(obj), objectiveType, obj, modifier_distance);
					if (DebugPathfindFailMessagesLiveEnabled || DebugPathfindFailMessagesFileEnabled) {
						StringClass debug;
						debug.Format("%s(%d): cannot reach: %s (%s-%d) at location: %.2f / %.2f / %.2f  (%.2f dist, %.5f prio) (%d/%d)", botName.Peek_Buffer(), Get_Object_Type(obj), m_pCurrentObjective->GetDebugTag(), Commands->Get_Preset_Name(objectiveObj), objectiveObj->Get_ID(), objectiveLocation.X, objectiveLocation.Y, objectiveLocation.Z, getDistance(obj, objectiveObj), m_pCurrentObjective->GetPriority(obj, modifier_distance), ignoredObjectives.Count(), objectiveCount);
						Print_Debug(debug);
					}

					// Out of objectives, could not reach any, suicide now and hope for a better spawn location next time.
					if (objectiveCount && objectiveCount == ignoredObjectives.Count()) {
						Vector3 botLocation = Commands->Get_Position(obj);
						if (DebugPathfindFailMessagesLiveEnabled || DebugPathfindFailMessagesFileEnabled) {
							StringClass debug;
							debug.Format("%s: out of objectives (%d), suiciding now at location: %.2f / %.2f / %.2f ", botName.Peek_Buffer(), objectiveCount, botLocation.X, botLocation.Y, botLocation.Z);
							Print_Debug(debug);
						}

						Commands->Send_Custom_Event(obj, obj, CUSTOM_AI_REQUEST_REFUND, 0, 0.0f);
						Commands->Apply_Damage(obj, 9999.0f, "Death", nullptr);
						return;
					}
				}
				m_target = nullptr;
				ResetAllActions(obj);
				state = UNKNOWN;
				// Faster acting
				m_pCurrentObjective = ChooseNewObjective(obj);
				if (nullptr != m_pCurrentObjective) {
					GoToObjective(obj);
					return;
				}
			}
			// Transport arrived to actual objective
			else if (obj->As_VehicleGameObj() && obj->As_VehicleGameObj()->Get_Occupant_Count() > 1
				&& m_pCurrentObjective && m_pCurrentObjective->GetGameObject()
				&& getDistance(obj, m_pCurrentObjective->GetGameObject()) < m_pCurrentObjective->GetRange()
				)
			{
				Commands->Send_Custom_Event(obj, obj, CUSTOM_AI_TRANSPORT_ARRIVED, 0, 0.0f);
			}
			// This action needs to call the parent
			dp88_AI_Tank_Offensive::Action_Complete(obj, action_id, complete_reason);
		}
		else if (action_id == ACTION_ID_ATTACK_TARGET)
		{
			if (complete_reason == ACTION_COMPLETE_PATH_BAD_DEST || complete_reason == ACTION_COMPLETE_PATH_BAD_START)
			{
				pathfindFailCount++;
			}
			else
			{
				if (pathfindFailCount > 0)
					pathfindFailCount--;
				// Do not reset actions when move-location is changed!
				if (complete_reason == ACTION_COMPLETE_LOW_PRIORITY)
					return;
				else
					dp88_AI_Tank_Offensive::Action_Complete(obj, action_id, complete_reason);
			}
		}
		else if (action_id == ACTION_AI_GET_AROUND_REVERSE || action_id == ACTION_AI_GET_AROUND_FORWARD)
			return;
		else
			dp88_AI_Tank_Offensive::Action_Complete(obj, action_id, complete_reason);
	}

	// -------------------------------------------------------------------------------------------------

public:
	int Get_Objective_Type()
	{
		return (int)objectiveType;
	}

	int Get_State()
	{
		return state;
	}

	GameObject *Get_Override_Task()
	{
		return overridingTask;
	}

	int Get_Override_Task_Type()
	{
		return overridingTaskType;
	}

	GameObject *Get_Current_Objective()
	{
		return m_pCurrentObjective ? m_pCurrentObjective->GetGameObject() : nullptr;
	}

	void Set_Personality(BotPersonality *newPersonality)
	{
		REF_PTR_SET(personality, newPersonality);
	}

	bool Should_Chase_Radar_Target(GameObject *enemy)
	{
		if (!Owner() || objectiveType == dp88_AI_Objective::TYPE_ENGINEERING || m_target)
			return false;

		return ShouldPursueTarget(Owner(), enemy);
	}

	bool Should_Ignore_Blocked_Checking(GameObject *obj)
	{
		if (isDeployed)
			return true;

		if (m_target)
		{
			TargetType targetType = GetTargetType(m_target);

			// Is Repairing
			if (targetType == REPAIRABLE && getDistance(obj, m_target) <= GetPreferredAttackRange(obj, m_target))
				return true;
			//M_Console_Input("msg Target is repairable and within range, I don't care about 'being stuck'");

			bool hasWeapon = m_target->As_SmartGameObj() && m_target->As_SmartGameObj()->Get_Weapon() != nullptr;
			// DO check for stuck if moving, even if not evade-moving!
			if (!m_bMovingToObjective && 
				// Bot will not keep evading in these cases and would think it's stuck
				(!IsValidTarget(obj, m_target) || !ShouldPursueTarget(obj, m_target) || !hasWeapon || targetType == BUILDING))
				return true;
		}
		return false;
	}

	void Prepare_Getting_Around(GameObject *obj)
	{
		ResetAllActions(obj);
		gettingAround = true;
		m_bMovingToObjective = true;
	}

	void Stop_Getting_Around(GameObject *obj)
	{
		ResetAllActions(obj);
		gettingAround = false;
		m_target = nullptr;
	}

	void On_Unable_To_Get_Unstuck(GameObject *obj)
	{
		if (obj->As_SoldierGameObj())
		{
			Commands->Send_Custom_Event(obj, obj, CUSTOM_AI_REQUEST_REFUND, 0, 0.0f);
			Commands->Apply_Damage(obj, 9999.0f, "Death", nullptr);
		}
		else if (obj->As_VehicleGameObj())
		{
			Commands->Apply_Damage(obj, 9999.0f, "Death", nullptr);
		}
	}

	ActionParamsStruct Get_Param_Attack_Modifications(ActionParamsStruct params) override
	{
		// Return attack params if there is a target
		if (m_target)
		{
			// Setup the attack parameters
			m_bTargetPrimaryFire = getPrimary(m_target);
			params.Set_Attack(m_target, (float)((m_bTargetPrimaryFire) ? primary_maxRange : secondary_maxRange), attackError, m_bTargetPrimaryFire);
		}

		return params;
	}
};
REGISTER_SCRIPT(MS_AI_Behaviour,
	"Priority_Infantry=1.0:float,"
	"Weapon_Infantry=1:int,"
	"Splash_Infantry=0:int,"
	"Priority_Light_Vehicle=5.0:float,"
	"Weapon_Light_Vehicle=1:int,"
	"Priority_Heavy_Vehicle=7.0:float,"
	"Weapon_Heavy_Vehicle=1:int,"
	"Priority_VTOL=5.0:float,"
	"Weapon_VTOL=1:int,"
	"Priority_Building=3.0:float,"
	"Weapon_Building=1:int,"
	"Max_Attack_Range=150:int,"
	"Min_Attack_Range=0:int,"
	"Preferred_Attack_Range=60:int,"
	"Max_Attack_Range_Secondary=150:int,"
	"Min_Attack_Range_Secondary=0:int,"
	"Preferred_Attack_Range_Secondary=60:int,"
	"Modifier_Distance=0.25:float,"
	"Modifier_Target_Damage=0.1:float,"
	"Modifier_Target_Value=0.05:float,"
	"DebugDeprecated=0:int,"
	"Detects_Stealth=0:int,"
	"ObjectiveType=1:int"
);


/*!
* \brief AI Preferences
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* AI Preferences, used by MS_AI_Player_Count_Controller. Attach this to all presets you want
* to be able to become bots, without the need to duplicate a bot/player version of the preset.
*/
class MS_AI_Preferences : public ScriptImpClass
{
	void Created(GameObject *obj) override
	{
		/*if (obj->As_SmartGameObj()->Is_Human_Controlled())
		{
			Commands->Attach_Script(obj, "MS_Target_Painter_Host", "");
		}*/
	}

	void Killed(GameObject *obj, GameObject *killer) override
	{
		if (obj && obj->As_SmartGameObj() && obj->As_SmartGameObj()->Is_Human_Controlled())
		{
			Commands->Send_Custom_Event(obj, killer, CUSTOM_AI_ENEMY_KILLED, 0, 0.0f);
		}
	}
};
REGISTER_SCRIPT(MS_AI_Preferences,
	"Priority_Infantry=1.0:float,"
	"Weapon_Infantry=1:int,"
	"Splash_Infantry=0:int,"
	"Priority_Light_Vehicle=5.0:float,"
	"Weapon_Light_Vehicle=1:int,"
	"Priority_Heavy_Vehicle=7.0:float,"
	"Weapon_Heavy_Vehicle=1:int,"
	"Priority_VTOL=5.0:float,"
	"Weapon_VTOL=1:int,"
	"Priority_Building=3.0:float,"
	"Weapon_Building=1:int,"
	"Max_Attack_Range=150:int,"
	"Min_Attack_Range=0:int,"
	"Preferred_Attack_Range=60:int,"
	"Max_Attack_Range_Secondary=150:int,"
	"Min_Attack_Range_Secondary=0:int,"
	"Preferred_Attack_Range_Secondary=60:int,"
	"Modifier_Distance=0.25:float,"
	"Modifier_Target_Damage=0.1:float,"
	"Modifier_Target_Value=0.05:float,"
	"DebugDeprecated=0:int,"
	"Detects_Stealth=0:int"
);


/*!
* \brief AI Pathfind Helper
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* The script helps the AI a bit when colliding into objects they cannot see in the pathfind grid.
*/
class MS_AI_Pathfind_Helper : public ScriptImpClass
{
	// Improving pathfinding ability (getting stuck on objects/trees)
	float lastRotation;
	Vector3 lastPosition;
	Vector3 getAroundReversePos;
	Vector3 getAroundSidePos;
	bool gettingAround;
	int stuckCount;
	int consecutiveStuckCount;
	Pathfind_Help_Implementer *implScript;

	void Created(GameObject *obj)
	{
		lastPosition = Vector3();
		getAroundReversePos = Vector3();
		getAroundSidePos = Vector3();
		gettingAround = false;
		stuckCount = 0;
		consecutiveStuckCount = 0;
		implScript = nullptr;

		// Check for being blocked
		lastRotation = Commands->Get_Facing(obj);
		lastPosition = Commands->Get_Position(obj);
		Commands->Start_Timer(obj, this, 5.0f, TIMER_AI_CHECK_FOR_BLOCK);

		Commands->Start_Timer(obj, this, 0.1f, TIMER_AI_POST_CREATED);
	}

	void Post_Created(GameObject *obj)
	{
		if (Is_Script_Attached(obj, "MS_AI_Behaviour"))
		{
			implScript = (MS_AI_Behaviour *)(Find_Script_On_Object(obj, "MS_AI_Behaviour"));
		}
		/*else if (Is_Script_Attached(obj, "dp88_Ore_Miner"))
		{
			implScript = (dp88_Ore_Miner *)(Find_Script_On_Object(obj, "dp88_Ore_Miner"));
		}
		else if (Is_Script_Attached(obj, "dp88_AR_Chrono_Miner"))
		{
			implScript = (dp88_AR_Chrono_Miner *)(Find_Script_On_Object(obj, "dp88_AR_Chrono_Miner"));
		}*/
	}

	void Timer_Expired(GameObject* obj, int number) override
	{
		if (number == TIMER_AI_POST_CREATED)
		{
			Post_Created(obj);
		}
		else if (number == TIMER_AI_CHECK_FOR_BLOCK)
		{
			Check_For_Block(obj);
			// Keep running this check
			Commands->Start_Timer(obj, this, 1.0f, TIMER_AI_CHECK_FOR_BLOCK);
		}
		else if (number == TIMER_AI_CHECK_UNSTUCK_TIMEOUT)
		{
			if (!stuckCount && !gettingAround)
			{
				consecutiveStuckCount = 0;
			}
		}
		else if (number == TIMER_AI_CHECK_GET_AROUND_TIMEOUT)
		{
			if (!M_Has_Timer(obj, this, TIMER_AI_CHECK_UNSTUCK_TIMEOUT))
				Commands->Start_Timer(obj, this, 12, TIMER_AI_CHECK_UNSTUCK_TIMEOUT);
			
			stuckCount = 1;
			consecutiveStuckCount++;
			if (consecutiveStuckCount >= 3 && implScript)
			{
				implScript->On_Unable_To_Get_Unstuck(obj);
			}
			gettingAround = false;
			if (implScript)
			{
				implScript->Stop_Getting_Around(obj);
			}
		}
		else if (number == TIMER_AI_CHECK_DISTANCE)
		{
			bool isReversing = Commands->Get_Action_ID(obj) == ACTION_AI_GET_AROUND_REVERSE;
			auto checkPos = isReversing ? getAroundReversePos : getAroundSidePos;
			auto isInRange = Vector3::Distance(Commands->Get_Position(obj), checkPos) < 1.5f;

			if (isInRange)
			{
				if (isReversing)
					Reverse_Movement_Complete(obj);
				else if (!isReversing)
					Forward_Movement_Complete(obj);
			}
			else if (gettingAround)
			{
				Commands->Start_Timer(obj, this, 1, TIMER_AI_CHECK_DISTANCE);
			}
		}
	}

	void Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason) override
	{
		if (complete_reason == ACTION_COMPLETE_LOW_PRIORITY)
		{
			return;
		}

		if (action_id == ACTION_AI_GET_AROUND_REVERSE)
		{
			Reverse_Movement_Complete(obj);
		}
		else if (action_id == ACTION_AI_GET_AROUND_FORWARD)
		{
			Forward_Movement_Complete(obj);
		}
	}

	void Check_For_Block(GameObject *obj)
	{
		if (Should_Ignore_Blocked_Checking(obj))
		{
			stuckCount = 0;
			return;
		}

		Vector3 pos = Commands->Get_Position(obj);
		float minDist = obj->As_SoldierGameObj() ? 1.5f : 3.0f;
		float rot = Commands->Get_Facing(obj);

		// This distance should depend on the speed... calculate average speed?
		if (Vector3::Distance(pos, lastPosition) < minDist && (obj->As_SoldierGameObj() || abs(rot - lastRotation) < 5))
			stuckCount++;
		else if (stuckCount > 0) // Don't go into negative
			stuckCount--;

		if (stuckCount > 5)
		{
			SimpleDynVecClass<Vector3> availableListB;
			SimpleDynVecClass<Vector3> availableListS;

			int distance = 12;
			// Right
			Vector3 offsetVectorRight;
			offsetVectorRight.X -= distance / 2;
			offsetVectorRight.Y -= distance;
			Vector3 backRight = Commands->Get_Position(obj) + Get_Transform(obj).Rotate_Vector(offsetVectorRight);
			offsetVectorRight.X += 3 * distance / 2;
			Vector3 right = Commands->Get_Position(obj) + Get_Transform(obj).Rotate_Vector(offsetVectorRight);

			float zCorrection = 0.5f;
			int count = 0;
			// These positions could be underground, raise
			while (!Get_Random_Pathfind_Spot_Around_Position(obj, backRight, 0, 5, &backRight))
			{
				count++;
				if (count >= 5)
				{
					backRight.Z -= count * zCorrection;
					break;
				}
				backRight.Z += zCorrection;
			}
			count = 0;
			while (!Get_Random_Pathfind_Spot_Around_Position(obj, right, 0, 5, &right))
			{
				count++;
				if (count >= 5)
				{
					right.Z -= count * zCorrection;
					break;
				}
				right.Z += zCorrection;
			}

			// Left
			Vector3 offsetVectorLeft;
			offsetVectorLeft.X -= distance / 2;
			offsetVectorLeft.Y += distance;
			Vector3 backLeft = Commands->Get_Position(obj) + Get_Transform(obj).Rotate_Vector(offsetVectorLeft);
			offsetVectorLeft.X += 3 * distance / 2;
			Vector3 left = Commands->Get_Position(obj) + Get_Transform(obj).Rotate_Vector(offsetVectorLeft);

			count = 0;
			// These positions could be underground, raise
			while (!Get_Random_Pathfind_Spot_Around_Position(obj, backLeft, 0, 5, &backLeft))
			{
				count++;
				if (count >= 5)
				{
					backLeft.Z -= count * zCorrection;
					break;
				}
				backLeft.Z += zCorrection;
			}
			count = 0;
			while (!Get_Random_Pathfind_Spot_Around_Position(obj, left, 0, 5, &left))
			{
				count++;
				if (count >= 5)
				{
					left.Z -= count * zCorrection;
					break;
				}
				left.Z += zCorrection;
			}

			if (Get_Random_Pathfind_Spot_Around_Position(obj, backRight, 0, 6, &backRight) && Get_Random_Pathfind_Spot_Around_Position(obj, right, 0, 6, &right))
			{
				availableListB.Add(backRight);
				availableListS.Add(right);
			}
			if (Get_Random_Pathfind_Spot_Around_Position(obj, backLeft, 0, 6, &backLeft) && Get_Random_Pathfind_Spot_Around_Position(obj, left, 0, 6, &left))
			{
				availableListB.Add(backLeft);
				availableListS.Add(left);
			}

			// Choose one side randomly
			if (availableListB.Count() > 0)
			{
				int random = FreeRandom.Get_Int(availableListB.Count());
				getAroundReversePos = availableListB[random];
				getAroundSidePos = availableListS[random];

				//M_Console_Input("msg STUCK: Attempting getting unstuck...");
			}
			else
			{
				//M_Console_Input("msg STUCK: There is no escape!");
				stuckCount = 0;
				Commands->Start_Timer(obj, this, 0, TIMER_AI_CHECK_GET_AROUND_TIMEOUT);
				return;
			}
			if (implScript)
			{
				implScript->Prepare_Getting_Around(obj);
			}
			else
			{
				Commands->Action_Reset(obj, 101.0f);
			}
			gettingAround = true;

			ActionParamsStruct params;
			params.Set_Basic(this, 95, ACTION_AI_GET_AROUND_REVERSE);
			params.Set_Movement(getAroundReversePos, 1.0f, 1.0f);
			params.MoveBackup = true;
			Handle_Movement_Action(obj, params);
		}

		lastRotation = rot;
		lastPosition = pos;
	}

	void Handle_Movement_Action(GameObject *obj, ActionParamsStruct params)
	{
		Commands->Start_Timer(obj, this, 9, TIMER_AI_CHECK_GET_AROUND_TIMEOUT);
		if (implScript)
		{
			params = implScript->Get_Param_Attack_Modifications(params);
			if (params.AttackObject)
			{
				Commands->Action_Attack(obj, params);
				// Attack action is not caught by Action_Complete function like Action_Goto, so we must check if arrived manually
				Commands->Start_Timer(obj, this, 1, TIMER_AI_CHECK_DISTANCE);
				return;
			}
		}
		Commands->Action_Goto(obj, params);
	}

	void Reverse_Movement_Complete(GameObject *obj)
	{
		if (M_Has_Timer(obj, this, TIMER_AI_CHECK_GET_AROUND_TIMEOUT))
			M_Stop_Timer2(obj, this, TIMER_AI_CHECK_GET_AROUND_TIMEOUT);

		ActionParamsStruct params;
		params.Set_Basic(this, 95, ACTION_AI_GET_AROUND_FORWARD);
		params.Set_Movement(getAroundSidePos, 1.0f, 1.0f);
		params.MovePathfind = false;
		params.MoveBackup = false;
		Handle_Movement_Action(obj, params);
	}

	void Forward_Movement_Complete(GameObject *obj)
	{
		if (M_Has_Timer(obj, this, TIMER_AI_CHECK_GET_AROUND_TIMEOUT))
			M_Stop_Timer2(obj, this, TIMER_AI_CHECK_GET_AROUND_TIMEOUT);

		if (!M_Has_Timer(obj, this, TIMER_AI_CHECK_UNSTUCK_TIMEOUT))
			Commands->Start_Timer(obj, this, 12, TIMER_AI_CHECK_UNSTUCK_TIMEOUT);

		stuckCount = 0;
		consecutiveStuckCount = 0;
		gettingAround = false;
		if (implScript)
		{
			implScript->Stop_Getting_Around(obj);
		}
	}

	bool Should_Ignore_Blocked_Checking(GameObject *obj)
	{
		return gettingAround || !obj->As_SmartGameObj()->Get_Action()->Is_Acting() || (implScript && implScript->Should_Ignore_Blocked_Checking(obj));
	}
};
REGISTER_SCRIPT(MS_AI_Pathfind_Helper, "");



/*------------------------
AI script helpers with player-logic elements
--------------------------*/


/* Bot control helper struct */
struct MS_AI_Global_Controller
{
private:
	static MS_AI_Global_Controller ms_instance;
	DynamicVectorClass<StringClass> availableBotNames;
	DynamicVectorClass<StringClass> killedResponses;
	DynamicVectorClass<StringClass> killerResponses;
	bool isInited;
	bool isMapInited;

	int rallyPointNodeGroupTeam[2];
	int rallyPointNodeNumTeam[2];
	int rallyPointChosenNodeIDTeam[2];
	bool rallyPointRegistrationOpenForTeam[2];
	DynamicVectorClass<int> rallyPointRegistrationsTeam[2];
	DynamicVectorClass<int> rallyPointUnitsNotArrivedTeam[2];
	int baseDefObjectiveIDTeam[2];

public:
	float radarRadius;
	GameObject* teamControllers[2];
	DynamicVectorClass<GameObject*> botControllersTeam[2];
	DynamicVectorClass<MS_AI_Rally_Point*> rallyPointsTeam[2];
	DynamicVectorClass<MS_AI_Patrol_Point*> patrolPointsTeam[2];
	DynamicVectorClass<Vector3> spawnPositions[2];
	DynamicVectorClass<int> playerList;
	DynamicVectorClass<const char*> engineerUnitList;
	int transportVehicleSeatCount;
	float grantCreditsMultiplier;
	bool chattingEnabled;
	bool radioCommandsEnabled;
	bool killedRespEnabled;
	bool killerRespEnabled;
	bool debugBuyMessagesEnabled;
	bool debugAcquireVehicleMessagesEnabled;
	bool debugGrouprushMessagesEnabled;

	MS_AI_Global_Controller() : isInited(false)
	{
		Reset();
	}

	~MS_AI_Global_Controller()
	{
		Reset();
	}

	void Reset()
	{
		isInited = false;
		availableBotNames.Delete_All();
		killedResponses.Delete_All();
		killerResponses.Delete_All();
	}

	static MS_AI_Global_Controller& GetInstance()
	{
		if (!ms_instance.isInited)
		{
			ms_instance.Init();
		}
		if (!ms_instance.isMapInited)
		{
			ms_instance.Create_Helper();
		}
		return ms_instance;
	}

	static bool GetMapInited()
	{
		return ms_instance.isMapInited;
	}

	void Init()
	{
		rallyPointNodeGroupTeam[0] = -1;
		rallyPointNodeGroupTeam[1] = -1;
		rallyPointNodeNumTeam[0] = -1;
		rallyPointNodeNumTeam[1] = -1;
		rallyPointChosenNodeIDTeam[0] = 0;
		rallyPointChosenNodeIDTeam[1] = 0;
		rallyPointRegistrationOpenForTeam[0] = true;
		rallyPointRegistrationOpenForTeam[1] = true;
		teamControllers[0] = nullptr;
		teamControllers[1] = nullptr;
		baseDefObjectiveIDTeam[0] = 0;
		baseDefObjectiveIDTeam[1] = 0;
		radarRadius = 0;
		transportVehicleSeatCount = 0;
		chattingEnabled = true;
		radioCommandsEnabled = true;
		killedRespEnabled = false;
		killerRespEnabled = false;
		debugBuyMessagesEnabled = false;
		debugAcquireVehicleMessagesEnabled = false;
		debugGrouprushMessagesEnabled = false;

		Init_Bot_Strings();
		Init_Bot_Config();
		AddRadioHook(RadioHook);
		AddPlayerJoinHook(PlayerJoinHook);
		AddPlayerLeaveHook(PlayerLeaveHook);
		isInited = true;
	}

	void Init_New_Map()
	{
		isMapInited = true;
		Refresh_Player_List();
		spawnPositions[0].Delete_All();
		spawnPositions[1].Delete_All();
		rallyPointsTeam[0].Delete_All();
		rallyPointsTeam[1].Delete_All();
		patrolPointsTeam[0].Delete_All();
		patrolPointsTeam[1].Delete_All();
		engineerUnitList.Delete_All();

		// TODO: THIS IS A SMALL PROBLEM
		radarRadius = 50.0f; //Commands->Get_Radar_Radius();

		IsFirstDebugMessageOnMapSent = false;

		int radarMode = RADAR_NOBODY;
		if (The_Game())
			radarMode = The_Game()->Get_Radar_Mode();

		if (radarMode == RADAR_ALL)
			Start_Timer_Fake(5.0f, TIMER_AI_CHECK_RADAR);

		Start_Timer_Fake(8.0f, TIMER_AI_CHECK_ALLIES);

		// Automatic building setups for bots
		// Get default parameters
		StringClass bldParams = "";
		DynamicVectorClass<ScriptParameter> parameters;
		Get_Script_Parameters("MS_AI_Building", parameters);
		for (int i = 0; i < parameters.Count(); i++)
		{
			auto parameter = parameters[i];
			bldParams.Format((i == 0 ? "%s%s" : "%s,%s"), bldParams.Peek_Buffer(), parameter.value.Peek_Buffer());
		}
		for (auto x = GameObjManager::BuildingGameObjList.Head(); x != nullptr; x = x->Next())
		{
			BuildingGameObj* object = x->Data();
			if (!Is_Script_Attached(object, "MS_AI_Building") && strcmp(Get_Skin(object), "Blamo") != 0)
				Commands->Attach_Script(object, "MS_AI_Building", bldParams.Peek_Buffer());
			// Find stock refineries, check their trickle/unload timers and hook into them to send money to bots
			if (object->As_RefineryGameObj() && !Is_Script_Attached(object, "MS_AI_Refinery"))
				Commands->Attach_Script(object, "MS_AI_Refinery", "");
		}

		DynamicVectorClass<StringClass> extraTargetList;
		INIClass* config = Get_INI("bot_config.cfg");
		if (config)
		{
			if (auto bldSection = config->Find_Section("BotTargets"))
			{
				int i = 0;
				StringClass entry = "Target0";
				while (config->Find_Entry("BotTargets", entry.Peek_Buffer()))
				{
					StringClass preset;
					config->Get_String(preset, "BotTargets", entry.Peek_Buffer(), "");
					extraTargetList.Add(preset);
					i++;
					entry.Format("Target%d", i);
				}
			}
			Release_INI(config);
		}

		if (extraTargetList.Count() > 0)
		{
			for (auto x = GameObjManager::GameObjList.Head(); x != nullptr; x = x->Next())
			{
				auto data = x->Data();
				if (!data->As_ScriptableGameObj() || extraTargetList.ID(Commands->Get_Preset_Name(data->As_ScriptableGameObj())) == -1)
					continue;

				auto object = data->As_ScriptableGameObj();
				if (!Is_Script_Attached(object, "MS_AI_Building") && strcmp(Get_Skin(object), "Blamo") != 0)
						Commands->Attach_Script(object, "MS_AI_Building", bldParams);
			}
		}

		Create_Engineer_Unit_List();
	}

	void End_Map()
	{
		isMapInited = false;
	}

	void Create_Helper()
	{
		if (!Find_Object_With_Script("MS_AI_Global_Controller_Helper"))
		{
			auto obj = Commands->Create_Object("Invisible_Object", Vector3());
			Commands->Attach_Script(obj, "MS_AI_Global_Controller_Helper", "");
		}
	}

	void Refresh_Player_List()
	{
		playerList.Delete_All();
		for (auto PlayerIter = Get_Player_List()->Head(); (PlayerIter != nullptr); PlayerIter = PlayerIter->Next())
		{
			cPlayer* p = PlayerIter->Data();
			// Check if ID is in the list already
			if (playerList.ID(p->Get_Id()) == -1)
			{
				playerList.Add(p->Get_Id());
			}
		}
	}

	void Init_Bot_Strings()
	{
		INIClass *config = Get_INI("bot_names.cfg");
		if (config)
		{
			int i = 0;
			StringClass entry = "BotName0";
			while (config->Find_Entry("BotNames", entry.Peek_Buffer()))
			{
				StringClass name;
				config->Get_String(name, "BotNames", entry.Peek_Buffer(), "");
				availableBotNames.Add(name);
				i++;
				entry.Format("BotName%d", i);
			}
			Release_INI(config);
		}
	}

	StringClass Get_Free_Bot_Name()
	{
		StringClass botName = DEFAULT_BOT_NAME;

		int count = availableBotNames.Count();
		// Get a random one
		if (count)
		{
			StringClass bot = availableBotNames[FreeRandom.Get_Int(count)];
			botName = bot;
			availableBotNames.DeleteObj(bot);
		}

		return botName;
	}

	void Give_Back_Bot_Name(const StringClass& name)
	{
		availableBotNames.Add(name);
	}

	StringClass Get_Killed_Response()
	{
		return Get_Chat_Response(killedResponses);
	}

	StringClass Get_Killer_Response()
	{
		return Get_Chat_Response(killerResponses);
	}

	StringClass Get_Chat_Response(const DynamicVectorClass<StringClass>& strArray)
	{
		StringClass resp = ":o";

		int count = strArray.Count();
		// Get a random one
		if (count)
		{
			resp = strArray[FreeRandom.Get_Int(count)];
		}

		if (FreeRandom.Get_Float() < 0.3f)
			resp = resp.AsUpper();
		else if (FreeRandom.Get_Float() < 0.3f)
			resp = resp.AsLower();

		if (FreeRandom.Get_Float() < 0.5f)
			resp += "!";

		return resp;
	}

	void Init_Bot_Config()
	{
		// 0 (building needs repair)
		// 1 (get in the veh), 2 (get out of the veh)
		// 6 (yes), 7 (no), 12 (move out), 13 (follow me), 14 (hold position), 15 (cover me)

		Radio_Message_Type::BUILDING_NEEDS_REPAIRS = 0;
		Radio_Message_Type::GET_IN_THE_VEHICLE = 1;
		Radio_Message_Type::GET_OUT_OF_THE_VEHICLE = 2;
		Radio_Message_Type::DESTROY_THAT_VEHICLE = 3;
		Radio_Message_Type::DONT_GET_IN_MY_WAY = 5;
		Radio_Message_Type::AFFIRMATIVE = 6;
		Radio_Message_Type::NEGATIVE = 7;

		Radio_Message_Type::TAKE_THE_POINT = 11;
		Radio_Message_Type::MOVE_OUT = 12;
		Radio_Message_Type::FOLLOW_ME = 13;
		Radio_Message_Type::COVER_ME = 15;
		Radio_Message_Type::DESTROY_IT_NOW = 19;

		Radio_Message_Type::ATTACK_THE_HARVESTER = 21;
		Radio_Message_Type::ATTACK_THAT_STRUCTURE = 22;
		Radio_Message_Type::DEFEND_THE_BASE = 25;
		Radio_Message_Type::DEFEND_THE_HARVESTER = 26;
		Radio_Message_Type::DEFEND_THAT_STRUCTURE = 27;

		// read from ini file?

		INIClass* config = Get_INI("bot_config.cfg");

		if (config)
		{
			killedRespEnabled = config->Get_Bool("EventKilled", "Enabled", false);
			int i = 0;
			StringClass entry = "Resp0";
			while (config->Find_Entry("EventKilled", entry.Peek_Buffer()))
			{
				StringClass name;
				config->Get_String(name, "EventKilled", entry.Peek_Buffer(), "");
				killedResponses.Add(name);
				i++;
				entry.Format("Resp%d", i);
			}

			killerRespEnabled = config->Get_Bool("EventKiller", "Enabled", false);
			i = 0;
			entry.Format("Resp0");
			while (config->Find_Entry("EventKiller", entry.Peek_Buffer()))
			{
				StringClass name;
				config->Get_String(name, "EventKiller", entry.Peek_Buffer(), "");
				killerResponses.Add(name);
				i++;
				entry.Format("Resp%d", i);
			}

			if (config->Get_Bool("RadioCommands", "Override", false))
			{
				Radio_Message_Type::BUILDING_NEEDS_REPAIRS = config->Get_Int("RadioCommands", "BUILDING_NEEDS_REPAIRS", Radio_Message_Type::BUILDING_NEEDS_REPAIRS);
				Radio_Message_Type::GET_IN_THE_VEHICLE = config->Get_Int("RadioCommands", "GET_IN_THE_VEHICLE", Radio_Message_Type::GET_IN_THE_VEHICLE);
				Radio_Message_Type::GET_OUT_OF_THE_VEHICLE = config->Get_Int("RadioCommands", "GET_OUT_OF_THE_VEHICLE", Radio_Message_Type::GET_OUT_OF_THE_VEHICLE);
				Radio_Message_Type::DESTROY_THAT_VEHICLE = config->Get_Int("RadioCommands", "DESTROY_THAT_VEHICLE", Radio_Message_Type::DESTROY_THAT_VEHICLE);
				Radio_Message_Type::DONT_GET_IN_MY_WAY = config->Get_Int("RadioCommands", "DONT_GET_IN_MY_WAY", Radio_Message_Type::DONT_GET_IN_MY_WAY);
				Radio_Message_Type::AFFIRMATIVE = config->Get_Int("RadioCommands", "AFFIRMATIVE", Radio_Message_Type::AFFIRMATIVE);
				Radio_Message_Type::NEGATIVE = config->Get_Int("RadioCommands", "NEGATIVE", Radio_Message_Type::NEGATIVE);

				Radio_Message_Type::TAKE_THE_POINT = config->Get_Int("RadioCommands", "TAKE_THE_POINT", Radio_Message_Type::TAKE_THE_POINT);
				Radio_Message_Type::MOVE_OUT = config->Get_Int("RadioCommands", "MOVE_OUT", Radio_Message_Type::MOVE_OUT);
				Radio_Message_Type::FOLLOW_ME = config->Get_Int("RadioCommands", "FOLLOW_ME", Radio_Message_Type::FOLLOW_ME);
				Radio_Message_Type::COVER_ME = config->Get_Int("RadioCommands", "COVER_ME", Radio_Message_Type::COVER_ME);
				Radio_Message_Type::DESTROY_IT_NOW = config->Get_Int("RadioCommands", "DESTROY_IT_NOW", Radio_Message_Type::DESTROY_IT_NOW);

				Radio_Message_Type::ATTACK_THE_HARVESTER = config->Get_Int("RadioCommands", "ATTACK_THE_HARVESTER", Radio_Message_Type::ATTACK_THE_HARVESTER);
				Radio_Message_Type::ATTACK_THAT_STRUCTURE = config->Get_Int("RadioCommands", "ATTACK_THAT_STRUCTURE", Radio_Message_Type::ATTACK_THAT_STRUCTURE);
				Radio_Message_Type::DEFEND_THE_BASE = config->Get_Int("RadioCommands", "DEFEND_THE_BASE", Radio_Message_Type::DEFEND_THE_BASE);
				Radio_Message_Type::DEFEND_THE_HARVESTER = config->Get_Int("RadioCommands", "DEFEND_THE_HARVESTER", Radio_Message_Type::DEFEND_THE_HARVESTER);
				Radio_Message_Type::DEFEND_THAT_STRUCTURE = config->Get_Int("RadioCommands", "DEFEND_THAT_STRUCTURE", Radio_Message_Type::DEFEND_THAT_STRUCTURE);
			}

			chattingEnabled = config->Get_Bool("General", "EnableChat", true);
			radioCommandsEnabled = config->Get_Bool("General", "EnableRadioCommands", true);
			transportVehicleSeatCount = config->Get_Int("General", "TransportVehicleMinimumSeatCount", 3);
			grantCreditsMultiplier = config->Get_Float("General", "GrantCreditsMultiplier", 1.0f);
			debugBuyMessagesEnabled = config->Get_Bool("Debug", "EnableBuyMessages", false);
			debugAcquireVehicleMessagesEnabled = config->Get_Bool("Debug", "EnableAcquireVehicleMessages", false);
			debugGrouprushMessagesEnabled = config->Get_Bool("Debug", "EnableGrouprushMessages", false);

			Release_INI(config);
		}

	}

	static int Get_Radio_Command_Index(int stringID, int team)
	{
		for (int i = 0; i <= 29; i++)
		{
			if (Get_Radio_Command_String(i) == stringID)
				return i;
		}
		return -1;
	}

	static void Receive_Radio_Command(GameObject *sender, int AnnouncementID, int orderType = 0, int orderObjID = 0)
	{
		int team = Get_Object_Type(sender);
		if (team != 0 && team != 1)
			return;

		int radioIndex = Get_Radio_Command_Index(AnnouncementID, team);
		if (radioIndex == Radio_Message_Type::DEFEND_THE_BASE)
		{
			MS_AI_Global_Controller::GetInstance().Create_Base_Defense_Objective(team);
			MS_AccessHelper::Activate_Defense_Protocol(team);
		}

		else if (radioIndex == Radio_Message_Type::BUILDING_NEEDS_REPAIRS)
			MS_AccessHelper::Activate_Defense_Protocol(team);

		else if (radioIndex == Radio_Message_Type::ATTACK_THE_HARVESTER)
		{
			if (auto target = Find_Harvester(team == 1 ? 0 : 1))
			{
				const int targetID = target->Get_ID();
				Create_Timed_Objective(team, 45.0f, Commands->Get_Position(target), 15, targetID, targetID);
			}
		}

		auto &botControllers = MS_AI_Global_Controller::GetInstance().botControllersTeam[team];
		// Send custom to all AI controllers
		for (int i = 0; i < botControllers.Count(); i++)
		{
			MS_AccessHelper::Send_Radio_Order_To_Bot(sender, botControllers[i], radioIndex, orderType, orderObjID);
		}
	}

	// Listen to Radio Commands
	static bool RadioHook(int PlayerType, int PlayerID, int AnnouncementID, int IconID, AnnouncementEnum AnnouncementType)
	{
		if (!MS_AI_Global_Controller::GetMapInited())
			return true;
		Receive_Radio_Command(Get_GameObj(PlayerID), AnnouncementID);
		return true;
	}

	static void PlayerJoinHook(int PlayerID, const char *PlayerName)
	{
		if (!MS_AI_Global_Controller::GetMapInited())
			return;
		// Check if ID is in the list already
		if (MS_AI_Global_Controller::GetInstance().playerList.ID(PlayerID) == -1)
			MS_AI_Global_Controller::GetInstance().playerList.Add(PlayerID);
	}

	static void PlayerLeaveHook(int PlayerID)
	{
		if (!MS_AI_Global_Controller::GetMapInited())
			return;
		MS_AI_Global_Controller::GetInstance().playerList.DeleteObj(PlayerID);
	}

	void Start_Timer_Fake(float duration, int timerNum, int param = -1)
	{
		if (auto helper = Find_Object_With_Script("MS_AI_Global_Controller_Helper"))
		{
			if (auto script = Find_Script_On_Object(helper, "MS_AI_Global_Controller_Helper"))
			{
				if (param == -1)
					Commands->Start_Timer(helper, script, duration, timerNum);
				else
					Commands->Send_Custom_Event(nullptr, helper, timerNum, param, duration);
			}
		}
	}

	void Stop_Timer_Fake(int number)
	{
		if (auto helper = Find_Object_With_Script("MS_AI_Global_Controller_Helper"))
		{
			if (auto script = Find_Script_On_Object(helper, "MS_AI_Global_Controller_Helper"))
			{
				if (M_Has_Timer(helper, script, number))
					M_Stop_Timer2(helper, script, number);
			}
		}
	}

	void Timer_Expired_Fake(int number, int param = -1)
	{
		if (number == TIMER_AI_COOP_GROUP_WAIT_TEAM_0 || number == TIMER_AI_COOP_GROUP_WAIT_TEAM_1)
		{
			int team = number == TIMER_AI_COOP_GROUP_WAIT_TEAM_0 ? 0 : 1;

			// Close registrations if this was the wait timer for the first node
			int &nodeNum = rallyPointNodeNumTeam[team];
			if (nodeNum == 0)
			{
				bool &regsOpen = rallyPointRegistrationOpenForTeam[team];
				regsOpen = false;
			}
			//M_Console_Input("msg Kicking unarrived registrants by timeout!");
			Kick_Missing_GroupRush_Registrants(team);
		}
		else if (number == CUSTOM_AI_COOP_PLAYER_DESTROY_OBJECTIVE)
		{
			int &id = baseDefObjectiveIDTeam[param];
			id = 0;
		}
		else if (number == TIMER_AI_CHECK_RADAR)
		{
			DynamicVectorClass<GameObject*> visibleObjects;
			// Go through all objects, save their position, team and visibility for later checks.
			for (auto x = GameObjManager::SmartGameObjList.Head(); x != nullptr; x = x->Next())
			{
				GameObject* object = x->Data();
				PhysicalGameObj* phys = object->As_PhysicalGameObj();

				// Check if radar blip is not None and make sure it's not a sneaking soldier
				if (phys->Get_Radar_Blip_Shape_Type() && object->As_SmartGameObj()->Is_Visible() &&
					(!object->As_SoldierGameObj() || (!Get_Vehicle(object)/* && (!Commands->Get_Soldiers_Can_Sneak() || !object->As_SoldierGameObj()->Is_Sneaking())*/)
					)
				)
				{
					visibleObjects.Add(object);
				}
			}

			Start_Timer_Fake(5.0f, TIMER_AI_CHECK_RADAR);

			bool team0RadarEnabled = Is_Radar_Enabled(0);
			bool team1RadarEnabled = Is_Radar_Enabled(1);

			auto botControllers = Combine_Lists(botControllersTeam[0], botControllersTeam[1]);
			for (int i = 0; i < botControllers.Count(); i++)
			{
				auto botController = botControllers[i];

				// Always reset first
				Commands->Send_Custom_Event(nullptr, botController, CUSTOM_AI_ADD_RADAR_TARGET, 0, 0.0f);

				// Radar off? Stop checking
				if (!(Get_Object_Type(botController) ? team1RadarEnabled : team0RadarEnabled))
					continue;

				// Send all visible enemies
				for (int j = 0; j < visibleObjects.Count(); j++)
				{
					auto visibleObject = visibleObjects[j];
					if (Get_Object_Type(visibleObject) != Get_Object_Type(botController) && Get_Object_Type(visibleObject) != SCRIPT_PLAYERTYPE_NEUTRAL)
					{
						Commands->Send_Custom_Event(nullptr, botController, CUSTOM_AI_ADD_RADAR_TARGET, visibleObject->Get_ID(), 0.1f);
					}
				}
			}
		}
		else if (number == TIMER_AI_CHECK_ALLIES)
		{
			DynamicVectorClass<GameObject*> playerObjects;
			for (int i = 0; i < playerList.Count(); i++)
			{
				auto playerID = playerList[i];
				if (auto playerObject = Get_GameObj(playerID))
				{ 
					playerObjects.Add(playerObject);
				}
			}

			Start_Timer_Fake(8.0f, TIMER_AI_CHECK_ALLIES);

			auto botControllers = Combine_Lists(botControllersTeam[0], botControllersTeam[1]);
			for (int i = 0; i < botControllers.Count(); i++)
			{
				auto botController = botControllers[i];

				Commands->Send_Custom_Event(nullptr, botController, CUSTOM_AI_ADD_ALLY_TARGET, 0, 0.0f);

				// Send all friendly players
				for (int j = 0; j < playerObjects.Count(); j++)
				{
					auto playerObject = playerObjects[j];
					if (Get_Object_Type(playerObject) == Get_Object_Type(botController) && Get_Object_Type(playerObject) != -2)
					{
						Commands->Send_Custom_Event(nullptr, botController, CUSTOM_AI_ADD_ALLY_TARGET, playerObject->Get_ID(), 0.1f);
					}
				}
			}
		}
	}

	void Change_Bot_Count(int count)
	{
		if (auto countController = Find_Object_With_Script("MS_AI_Player_Count_Controller"))
		{
			Commands->Send_Custom_Event(nullptr, countController, CUSTOM_AI_CHANGE_BOTCOUNT, count, 0);
		}
		// Create a count controller otherwise!
		else if (count != 0)
		{
			auto obj = Commands->Create_Object("Invisible_Object", Vector3());
			StringClass params;
			// Get default parameters
			DynamicVectorClass<ScriptParameter> parameters;
			Get_Script_Parameters("MS_AI_Player_Count_Controller", parameters);
			for (int i = 0; i < parameters.Count(); i++)
			{
				auto parameter = parameters[i];
				if (i == 0)
					params.Format("%d", count);
				else
					params.Format("%s,%s", params.Peek_Buffer(), parameter.value.Peek_Buffer());
			}
			Commands->Attach_Script(obj, "MS_AI_Player_Count_Controller", params.Peek_Buffer());
			M_Console_Input("msg WARNING: Bots are not configured for this map and may act strange!");
			if (!Is_Pathfind_Generated())
			{
				return;
			}

			if (!Find_Object_With_Script("dp88_AI_Objective"))
			{
				Create_Base_Attack_Objective(0);
				Create_Base_Attack_Objective(1);
			}
		}
	}

	void Change_Bot_Count_Team(int team)
	{
		if (auto countController = Find_Object_With_Script("MS_AI_Player_Count_Controller"))
		{
			Commands->Send_Custom_Event(nullptr, countController, CUSTOM_AI_CHANGE_BOTCOUNTTEAM, team, 0);
		}
	}

	void Create_Engineer_Unit_List()
	{
		// Find character presets that can repair a building
		if (auto sampleBuilding = Find_Building_By_Team(1) ? Find_Building_By_Team(1) : Find_Building_By_Team(0))
		{
			auto bldSkin = sampleBuilding->As_BuildingGameObj()->Get_Defense_Object()->Get_Skin();
			// auto bldSkinName = ArmorWarheadManager::Get_Armor_Name(bldSkin);
			/*StringClass params;
			params.Format("msg Sample building (%s): %s", Get_Translated_Definition_Name(Commands->Get_Preset_Name(sampleBuilding)), bldSkinName);
			M_Console_Input(params.Peek_Buffer());*/

			DynamicVectorClass<int> characterList;
			characterList = Combine_Lists(characterList, Get_Enlisted_Purchase_Items(1));
			characterList = Combine_Lists(characterList, Get_Enlisted_Purchase_Items(0));
			characterList = Combine_Lists(characterList, Get_Purchase_Items(1, PurchaseSettingsDefClass::TYPE_CLASSES));
			characterList = Combine_Lists(characterList, Get_Purchase_Items(0, PurchaseSettingsDefClass::TYPE_CLASSES));
			characterList = Combine_Lists(characterList, Get_Purchase_Items(1, PurchaseSettingsDefClass::TYPE_SECRET_CLASSES));
			characterList = Combine_Lists(characterList, Get_Purchase_Items(0, PurchaseSettingsDefClass::TYPE_SECRET_CLASSES));
			for (int i = 0; i < characterList.Count(); i++)
			{
				const char* presetName = DefinitionMgrClass::Find_Definition(characterList[i], false)->Get_Name();
				if (auto soldierDef = (SoldierGameObjDef*)Find_Named_Definition(presetName))
				{

					DynamicVectorClass<int> weaponList;
					weaponList.Add(soldierDef->Get_Weapon_Def_ID());
					weaponList.Add(soldierDef->Get_Secondary_Weapon_Def_ID());
					auto scriptList = soldierDef->ScriptNameList;
					auto paramList = soldierDef->ScriptParameterList;
					for (int j = 0; j < scriptList.Count(); j++)
					{
						// Check weapon granting scripts, there are multiple?
						if (strcmp(scriptList[j], "M00_GrantPowerup_Created") == 0)
						{
							if (auto powerupDef = (PowerUpGameObjDef*)Find_Named_Definition(paramList[j].Peek_Buffer()))
								weaponList.Add(powerupDef->Get_Grant_Weapon_ID());
						}
					}

					for (int j = 0; j < weaponList.Count(); j++)
					{
						if (auto weaponDef = (WeaponDefinitionClass*)Find_Definition(weaponList[j]))
						{
							if (auto ammoDef = (AmmoDefinitionClass*)Find_Definition(weaponDef->PrimaryAmmoDefID))
							{
								//auto whName = ArmorWarheadManager::Get_Warhead_Name(ammoDef->Warhead());
								auto multiplier = ArmorWarheadManager::Get_Damage_Multiplier(bldSkin, ammoDef->Warhead());
								auto damage = ammoDef->Damage();
								// Repairs!
								if (multiplier * damage < 0)
								{
									/*StringClass params;
									params.Format("msg Preset can rep: '%s' Weapon: %s, ammo warhead: %s, bld damage: %.2f / multiplier: %.2f, res: %.2f", presetName, weaponDef->Get_Name(), whName, damage, multiplier, multiplier*damage);// Get_Translated_Definition_Name(presetName));
									M_Console_Input(params.Peek_Buffer());*/
									engineerUnitList.Add(presetName);
									break;
								}
							}
						}
					}
				}
			}
		}
	}

	int Get_Temporary_Objective_ID(int team)
	{
		return baseDefObjectiveIDTeam[team];
	}

	void Create_Base_Attack_Objective(int team)
	{
		// Create objective in enemy base
		Vector3 center = Get_Base_Center((team == 1 ? 0 : 1));
		Vector3 pos;
		Get_Closest_Pathfind_Spot_Size(center, 50.0f, &pos, 3.0f);

		Create_Timed_Objective(team, 0, pos, 40, dp88_AI_Objective::TYPE_OFFENSIVE);
	}

	void Create_Base_Defense_Objective(int team)
	{
		//M_Console_Input("msg Creating base defense objective...");
		Vector3 center = Get_Base_Center(team);
		Vector3 pos;
		Get_Closest_Pathfind_Spot_Size(center, 50.0f, &pos, 3.0f);

		int &id = baseDefObjectiveIDTeam[team];
		if (!id)
		{
			id = Create_Timed_Objective(team, 60, pos, 40);
			Start_Timer_Fake(60, CUSTOM_AI_COOP_PLAYER_DESTROY_OBJECTIVE, team);
		}
	}

	Vector3 Get_Closest_Spawn_Point(GameObject *obj, int team)
	{
		Vector3 returnPos = Vector3();
		float dist = FLT_MAX;
		for (int i = 0; i < spawnPositions[team].Count(); i++)
		{
			auto position = spawnPositions[team][i];
			float newDistance = Vector3::Distance_Squared(Commands->Get_Position(obj), position);
			if (newDistance < dist)
			{
				returnPos = position;
				dist = newDistance;
			}
		}
		return returnPos;
	}

	int Get_New_Patrol_Point_ID(int team, int previousID, bool isForVehicle)
	{
		auto &patrolPoints = patrolPointsTeam[team];
		if (patrolPoints.Count())
		{
			if (patrolPoints.Count() == 1)
				return patrolPoints[0]->Get_Object()->Get_ID();
			
			auto copiedPoints = patrolPoints;
			DynamicVectorClass<MS_AI_Patrol_Point*> nodes;
			if (auto previousPoint = Commands->Find_Object(previousID))
			{
				auto patrolScript = (MS_AI_Patrol_Point *)(Find_Script_On_Object(previousPoint, "MS_AI_Patrol_Point"));
				copiedPoints.DeleteObj(patrolScript);

				// Collect all patrol points with the next num, return a random one
				for (int i = 0; i < copiedPoints.Count(); i++)
				{
					// Skip other groups
					if (copiedPoints[i]->Get_NodeGroup() != patrolScript->Get_NodeGroup())
						continue;

					if (copiedPoints[i]->Get_NodeNum() == patrolScript->Get_NodeNum() + 1)
						nodes.Add(copiedPoints[i]);
				}
				// Choose one of the same nums randomly
				if (nodes.Count() > 0)
					return nodes[FreeRandom.Get_Int(nodes.Count())]->Get_Object()->Get_ID();
				// None found, get num 0 of another group
				else
				{
					for (int i = 0; i < copiedPoints.Count(); i++)
					{
						// Skip same group or if not fit (inf only group for a vehicle)
						if (copiedPoints[i]->Get_NodeGroup() == patrolScript->Get_NodeGroup() || (copiedPoints[i]->Get_IsInfantryOnly() && isForVehicle))
							continue;

						if (copiedPoints[i]->Get_NodeNum() == 0)
							nodes.Add(copiedPoints[i]);
					}
					// Choose one of the same nums randomly
					if (nodes.Count() > 0)
						return nodes[FreeRandom.Get_Int(nodes.Count())]->Get_Object()->Get_ID();
				}
			}
			nodes.Delete_All();
			// Get num 0 of any group
			for (int i = 0; i < copiedPoints.Count(); i++)
			{
				// Skip if not fit (inf only group for a vehicle)
				if (copiedPoints[i]->Get_IsInfantryOnly() && isForVehicle)
					continue;

				if (copiedPoints[i]->Get_NodeNum() == 0)
					nodes.Add(copiedPoints[i]);
			}
			// Choose one of the same nums randomly
			if (nodes.Count() > 0)
				return nodes[FreeRandom.Get_Int(nodes.Count())]->Get_Object()->Get_ID();
		}
		return 0;
	}

	void Reset_GroupRush(int team)
	{
		//M_Console_Input("msg Reset_GroupRush.");

		int &nodeGroup = rallyPointNodeGroupTeam[team];
		nodeGroup = -1;

		bool &regsOpen = rallyPointRegistrationOpenForTeam[team];
		regsOpen = true;

		DynamicVectorClass<int> &rallyPointUnitsNotArrived = rallyPointUnitsNotArrivedTeam[team];
		DynamicVectorClass<int> &rallyPointRegs = rallyPointRegistrationsTeam[team];

		for (int i = 0; i < rallyPointRegs.Count(); i++)
		{
			Commands->Send_Custom_Event(nullptr, Commands->Find_Object(rallyPointRegs[i]), CUSTOM_AI_COOP_GROUPRUSH_COMPLETE, 0, 0.0f);
		}

		rallyPointUnitsNotArrived.Clear();
		rallyPointRegs.Clear();
	}

	/**
	* 0 = closed
	* 1 = infantry only
	* 2 = infantry + vehicles allowed
	*/
	int Is_Rally_Point_Registration_Open(int team)
	{
		// No rally points, no registration
		DynamicVectorClass<MS_AI_Rally_Point*> &rallyPoints = rallyPointsTeam[team];
		if (rallyPoints.Count() == 0)
			return 0;

		bool isOpen = rallyPointRegistrationOpenForTeam[team];
		if (!isOpen)
		{
			return 0;
		}

		if (auto node = Commands->Find_Object(Get_Rally_Point_ID(team)))
		{
			if (auto nodeScript = Find_Script_On_Object(node, "MS_AI_Rally_Point"))
			{
				return ((MS_AI_Rally_Point *)nodeScript)->Get_IsInfantryOnly() ? 1 : 2;
			}
		}

		return 0;
	}

	int Get_Rally_Point_ID(int team)
	{
		// Correct set of rallyPoints
		DynamicVectorClass<MS_AI_Rally_Point*> &rallyPoints = rallyPointsTeam[team];
		// Current node group
		int &nodeGroup = rallyPointNodeGroupTeam[team];
		// Current node num
		int &nodeNum = rallyPointNodeNumTeam[team];
		// Chosen final node
		int &nodeID = rallyPointChosenNodeIDTeam[team];

		// Set random node group if not yet set
		if (nodeGroup == -1)
		{
			DynamicVectorClass<int> nodeGroups;
			for (int i = 0; i < rallyPoints.Count(); i++)
			{
				// Only add a group once
				if (nodeGroups.ID(rallyPoints[i]->Get_NodeGroup()) == -1)
					nodeGroups.Add(rallyPoints[i]->Get_NodeGroup());
			}
			if (nodeGroups.Count() > 0)
			{
				nodeGroup = nodeGroups[FreeRandom.Get_Int(nodeGroups.Count())];
				// Set node num to first (0)
				nodeNum = 0;
				nodeID = 0;
			}
		}

		// Pick random node's id with matching num if not yet set
		if (nodeID == 0)
		{
			DynamicVectorClass<MS_AI_Rally_Point*> nodes;
			for (int i = 0; i < rallyPoints.Count(); i++)
			{
				// Skip other groups
				if (rallyPoints[i]->Get_NodeGroup() != nodeGroup)
					continue;

				if (rallyPoints[i]->Get_NodeNum() == nodeNum)
					nodes.Add(rallyPoints[i]);
			}
			// Choose one of the same nums randomly
			if (nodes.Count() > 0)
				nodeID = nodes[FreeRandom.Get_Int(nodes.Count())]->Get_Object()->Get_ID();
		}

		/*StringClass buff;
		buff.Format("msg Returning: (team: %d), group: %d, num: %d, ID: %d ", team, nodeGroup, nodeNum, nodeID);
		M_Console_Input(buff.Peek_Buffer());*/

		return nodeID;
	}

	void Check_Group_Arrived(int team)
	{
		DynamicVectorClass<int> &rallyPointUnitsNotArrived = rallyPointUnitsNotArrivedTeam[team];
		DynamicVectorClass<int> &rallyPointRegs = rallyPointRegistrationsTeam[team];

		bool &regsOpen = rallyPointRegistrationOpenForTeam[team];

		/*StringClass buff;
		buff.Format("msg Group arrived: %d/%d (team:%d)", rallyPointRegs.Count() - rallyPointUnitsNotArrived.Count(), rallyPointRegs.Count(), team);
		M_Console_Input(buff.Peek_Buffer());*/

		// All died? Reset
		if (rallyPointRegs.Count() == 0)
		{
			Reset_GroupRush(team);
			return;
		}


		// Print out who has not arrived yet
		/*for (int i = 0; i < rallyPointUnitsNotArrived.Count(); i++)
		{
		StringClass buff2;
		buff2.Format("msg Not arrived yet: %d (team:%d)", rallyPointUnitsNotArrived[i], team);
		M_Console_Input(buff2.Peek_Buffer());
		}*/

		// All arrived
		if (rallyPointUnitsNotArrived.Count() == 0 && !regsOpen)
		{
			// Stop kick timer!
			int timerID = team == 0 ? TIMER_AI_COOP_GROUP_WAIT_TEAM_0 : TIMER_AI_COOP_GROUP_WAIT_TEAM_1;
			Stop_Timer_Fake(timerID);
			//M_Console_Input("msg Stopping existing timer (WAIT)");

			int &nodeNum = rallyPointNodeNumTeam[team];
			int &nodeID = rallyPointChosenNodeIDTeam[team];

			nodeID = 0;
			nodeNum = nodeNum + 1;
			int nextID = Get_Rally_Point_ID(team);

			for (int i = 0; i < rallyPointRegs.Count(); i++)
			{
				Commands->Send_Custom_Event(nullptr, Commands->Find_Object(rallyPointRegs[i]), CUSTOM_AI_COOP_NEXT_RALLYPOINT, nextID, 0.0f);
			}
			rallyPointUnitsNotArrived.Clear();
			rallyPointUnitsNotArrived = rallyPointRegistrationsTeam[team];

			// None found -> grouprush reached last location (or other problem). Reset
			if (nextID == 0)
			{
				//M_Console_Input("msg Last location reached.");
				Reset_GroupRush(team);
			}
		}
	}

	void Object_Arrived(int team, int objID)
	{
		DynamicVectorClass<int> &rallyPointUnitsNotArrived = rallyPointUnitsNotArrivedTeam[team];

		// Check if ID is gone from the list already
		if (rallyPointUnitsNotArrived.ID(objID) == -1)
		{
			// This number is not in the array
			return;
		}

		int &nodeNum = rallyPointNodeNumTeam[team];
		// First unit arrived to not first node, start timeout
		if (nodeNum != 0 && rallyPointUnitsNotArrived.Count() == rallyPointRegistrationsTeam[team].Count())
		{
			int timerID = team == 0 ? TIMER_AI_COOP_GROUP_WAIT_TEAM_0 : TIMER_AI_COOP_GROUP_WAIT_TEAM_1;
			Start_Timer_Fake(80, timerID);
			//Commands->Start_Timer(Owner(), this, 90.0f, timerID);
		}

		rallyPointUnitsNotArrived.DeleteObj(objID);
		Check_Group_Arrived(team);
	}

	void Register_For_GroupRush(int team, int objID)
	{
		DynamicVectorClass<int> &rallyPointRegs = rallyPointRegistrationsTeam[team];
		DynamicVectorClass<int> &rallyPointUnitsNotArrived = rallyPointUnitsNotArrivedTeam[team];

		// Start timer for registration
		if (rallyPointRegs.Count() == 0)
		{
			int timerID = team == 0 ? TIMER_AI_COOP_GROUP_WAIT_TEAM_0 : TIMER_AI_COOP_GROUP_WAIT_TEAM_1;
			Start_Timer_Fake(130, timerID);
			//Commands->Start_Timer(Owner(), this, 140.0f, timerID);
		}
		// Check if ID is in the list already
		if (rallyPointRegs.ID(objID) != -1)
		{
			//M_Console_Input("msg Trying to add an ID twice to the rallyPointRegs list!");
			return;
		}

		/*StringClass buff;
		buff.Format("msg Adding: %d to grouprush! (team: %d)", objID, team);
		M_Console_Input(buff.Peek_Buffer());*/

		rallyPointRegs.Add(objID);
		rallyPointUnitsNotArrived.Add(objID);

		// Close group rush if enough/too many bots registered already
		if (rallyPointRegs.Count() >= 8 || rallyPointRegs.Count() >= botControllersTeam[team].Count() / 2)
		{
			bool &regsOpen = rallyPointRegistrationOpenForTeam[team];
			regsOpen = false;
		}
	}

	void Unregister_From_GroupRush(int team, int objID)
	{
		DynamicVectorClass<int> &rallyPointRegs = rallyPointRegistrationsTeam[team];
		DynamicVectorClass<int> &rallyPointUnitsNotArrived = rallyPointUnitsNotArrivedTeam[team];

		/*StringClass buff;
		buff.Format("msg Deleting: %d from grouprush! (team: %d)", objID, team);
		M_Console_Input(buff.Peek_Buffer());*/

		rallyPointRegs.DeleteObj(objID);
		rallyPointUnitsNotArrived.DeleteObj(objID);
		// Send 0 as next node and grouprush as complete
		GameObject* obj = Commands->Find_Object(objID);
		Commands->Send_Custom_Event(nullptr, obj, CUSTOM_AI_COOP_NEXT_RALLYPOINT, 0, 0.0f);
		Commands->Send_Custom_Event(nullptr, obj, CUSTOM_AI_COOP_GROUPRUSH_COMPLETE, 0, 0.0f);
		Check_Group_Arrived(team);
	}

	void Kick_Missing_GroupRush_Registrants(int team)
	{
		DynamicVectorClass<int> notArrivedList = rallyPointUnitsNotArrivedTeam[team];

		if (notArrivedList.Count() == 0)
		{
			Check_Group_Arrived(team);
		}
		else
		{
			for (int i = 0; i < notArrivedList.Count(); i++)
			{
				Unregister_From_GroupRush(team, notArrivedList[i]);
			}
		}
	}
};
MS_AI_Global_Controller MS_AI_Global_Controller::ms_instance;



/*!
* \brief Helper for MS_AI_Global_Controller
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* Subject to change. DO NOT PLACE THIS MANUALLY!
*
* \param Nothing
*   Nothing
*/
class MS_AI_Global_Controller_Helper : public ScriptImpClass
{
	void Created(GameObject *obj) override
	{
		// Making sure the controller exists
		MS_AI_Global_Controller::GetInstance().Init_New_Map();
	}

	void Custom(GameObject *obj, int type, int param, GameObject *sender) override
	{
		if (type == CUSTOM_AI_CHANGE_BOTCOUNT)
		{
			MS_AI_Global_Controller::GetInstance().Change_Bot_Count(param);
		}
		else if (type == CUSTOM_AI_CHANGE_BOTCOUNTTEAM)
		{
			MS_AI_Global_Controller::GetInstance().Change_Bot_Count_Team(param);
		}
		else
			MS_AI_Global_Controller::GetInstance().Timer_Expired_Fake(type, param);
	}

	void Timer_Expired(GameObject *obj, int number) override
	{
		MS_AI_Global_Controller::GetInstance().Timer_Expired_Fake(number);
	}

	void Detach(GameObject *obj) override
	{
		if (Exe != EXE_LEVELEDIT)
			MS_AI_Global_Controller::GetInstance().End_Map();

		ScriptImpClass::Detach(obj);
	}
};
REGISTER_SCRIPT(MS_AI_Global_Controller_Helper, "");


/*!
* \brief AI units gather at these points before attacks
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* Subject to change
*
*/
void MS_AI_Rally_Point::Created(GameObject *obj)
{
	StringClass params;
	params.Format("%d,%d,%d,%d,%d,%d,%d", Get_Team(), obj->Get_ID(), (Get_IsInfantryOnly() ? 12:20), 1, 1, 1, 1);
	Commands->Attach_Script(obj, "dp88_AI_Objective", params.Peek_Buffer());
	MS_AI_Global_Controller::GetInstance().rallyPointsTeam[Get_Team()].Add(this);

	/*
	WideStringClass str;
	str.Format(L"msg RALLY POINT Team: %d, NodeGroup: %d, NodeNum: %d!", Get_Team(), Get_NodeGroup(), Get_NodeNum());
	M_Console_Input(str);
	*/
}

void MS_AI_Rally_Point::Destroyed(GameObject *obj)
{
	MS_AI_Global_Controller::GetInstance().rallyPointsTeam[Get_Team()].DeleteObj(this);
}


int MS_AI_Rally_Point::Get_Team()
{
	return Get_Int_Parameter("Team") == 1 ? 1 : 0;
}

int MS_AI_Rally_Point::Get_NodeGroup()
{
	return Get_Int_Parameter("NodeGroup");
}

int MS_AI_Rally_Point::Get_NodeNum()
{
	return Get_Int_Parameter("NodeNum");
}

bool MS_AI_Rally_Point::Get_IsInfantryOnly()
{
	return Get_Int_Parameter("IsInfantryOnly") == 1;
}

GameObject *MS_AI_Rally_Point::Get_Object()
{
	return Owner();
}

REGISTER_SCRIPT(MS_AI_Rally_Point,
	"Team=0:int,"
	"NodeGroup=0:int,"
	"NodeNum=0:int,"
	"IsInfantryOnly=0:int"
);


/*!
* \brief AI defensive bots use these points to patrol/defend the base
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* Subject to change
*
*/
void MS_AI_Patrol_Point::Created(GameObject *obj)
{
	StringClass params;
	params.Format("%d,%d,%d,%d,%d,%d,%d", Get_Team(), obj->Get_ID(), (Get_IsInfantryOnly() ? 3 : 6), 1, 1, 1, 1);
	Commands->Attach_Script(obj, "dp88_AI_Objective", params.Peek_Buffer());
	MS_AI_Global_Controller::GetInstance().patrolPointsTeam[Get_Team()].Add(this);

	/*
	WideStringClass str;
	str.Format(L"msg PATROL POINT Team: %d, NodeGroup: %d, NodeNum: %d!", Get_Team(), Get_NodeGroup(), Get_NodeNum());
	M_Console_Input(str);
	*/
}

void MS_AI_Patrol_Point::Destroyed(GameObject *obj)
{
	MS_AI_Global_Controller::GetInstance().patrolPointsTeam[Get_Team()].DeleteObj(this);
}

REGISTER_SCRIPT(MS_AI_Patrol_Point,
	"Team=0:int,"
	"NodeGroup=0:int,"
	"NodeNum=0:int,"
	"IsInfantryOnly=0:int"
);


/*!
* \brief AI team controller
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* Subject to change. DO NOT PLACE THIS MANUALLY!
*
* \param Nothing
*   Nothing
*/
class MS_AI_Team_Controller : public ScriptImpClass
{
	int team;
	int nextObjectiveType;
	DynamicVectorClass<GameObject*> engineers;
	DynamicVectorClass<GameObject*> defenders;
	bool beaconPlacementVictoryAllowed;

	void Created(GameObject *obj) override
	{
		nextObjectiveType = dp88_AI_Objective::TYPE_OFFENSIVE;
		team = Get_Int_Parameter("Team");
		// Neutral is bad, actual team is needed by some scripts
		Set_Object_Type(obj, team);

		// Let Global Controller know we exist
		MS_AI_Global_Controller::GetInstance().teamControllers[team] = obj;

		beaconPlacementVictoryAllowed = false;
		if (The_Cnc_Game())
			beaconPlacementVictoryAllowed = The_Cnc_Game()->Beacon_Placement_Ends_Game();
		else if (The_Skirmish_Game())
			beaconPlacementVictoryAllowed = The_Skirmish_Game()->Beacon_Placement_Ends_Game();

		Commands->Start_Timer(obj, this, 0.1f, TIMER_AI_THINK);
	}

	void Destroyed(GameObject *obj) override
	{
		// Let Global Controller know we are gone
		MS_AI_Global_Controller::GetInstance().teamControllers[team] = nullptr;
	}

	void Timer_Expired(GameObject *obj, int number) override
	{
		if (The_Game()->Is_Game_Over())
			return;

		if (number == TIMER_AI_THINK)
		{
			Check_Explosives();

			nextObjectiveType = dp88_AI_Objective::TYPE_OFFENSIVE;

			// Search for engineering objectives
			if (Are_Engineers_Needed(obj))
				nextObjectiveType = dp88_AI_Objective::TYPE_ENGINEERING;
			// Search for defensive objectives if no engineering is present
			else if (Are_Defenders_Needed(obj))
				nextObjectiveType = dp88_AI_Objective::TYPE_DEFENSIVE;

			Commands->Start_Timer(obj, this, 5.0f, TIMER_AI_THINK);
		}
	}

	bool Are_Engineers_Needed(GameObject *obj)
	{
		auto objectives = dp88_AI_Objective::CountObjectives(team, dp88_AI_Objective::TYPE_ENGINEERING);
		auto engisNeeded = (objectives ? 1 : 0) + (M_Has_Timer(obj, this, TIMER_AI_DEFEND) ? 1 : 0);
		return engineers.Count() < engisNeeded;
	}

	bool Are_Defenders_Needed(GameObject *obj)
	{
		auto objectives = dp88_AI_Objective::CountObjectives(team, dp88_AI_Objective::TYPE_DEFENSIVE);
		auto defendersNeeded = (objectives ? 2 : 0) + (M_Has_Timer(obj, this, TIMER_AI_DEFEND) ? 2 : 0);
		return defenders.Count() < defendersNeeded;
	}

	GameObject *Find_Closest_AI_Building(const Vector3& position)
	{
		GameObject* bestBuilding = nullptr;
		float bestDistance = FLT_MAX;
		for (int index = 0; index < AIBuildings.Count(); index++)
		{
			auto building = AIBuildings[index];
			if (Get_Object_Type(building) != team)
				continue;
			Vector3 offset = Commands->Get_Position(building) - position;
			float distance = offset.Length2();
			if (distance < bestDistance)
			{
				bestBuilding = building;
				bestDistance = distance;
			}
		}
		return bestBuilding;
	}

	void Check_Explosives()
	{
		if (!AIBuildings.Count() && !beaconPlacementVictoryAllowed)
			return;

		// Find beacons around friendly buildings, attach objectives to them
		for (auto node = GameObjManager::BeaconGameObjList.Head(); node; node = node->Next())
		{
			auto object = node->Data();
			// Skip friendly beacons, undeployed ones and ones that already have a marker on them
			if (Get_Object_Type(object) == team || Is_Script_Attached(object, "dp88_AI_Marker_Repairable"))
				continue;

			bool isValidTarget = Is_On_Enemy_Pedestal(object) && beaconPlacementVictoryAllowed;

			// Only check further if it's not a pedestal beacon and valid AI buildings exist
			if (!isValidTarget && !AIBuildings.Count())
				continue;

			bool isVehicleTarget = false;
			if (!isValidTarget)
			{
				Vector3 objPos = Commands->Get_Position(object);
				auto bld = Find_Closest_AI_Building(objPos);
				// This check is necessary as it can happen that only the other team has buildings, and this team does not
				if (!bld)
					continue;

				ExplosionDefinitionClass *explosion = (ExplosionDefinitionClass *)DefinitionMgrClass::Find_Definition(object->Get_Definition().Get_Explosion_Obj_Def(), true);
				if (!explosion)
					continue;

				const float damageRadius = explosion->DamageRadius + 1.0f;
				// Attach objective if is close to the building
				if (bld->As_BuildingGameObj())
				{
					Vector3 closestPoly;
					Find_Closest_Poly_Position(bld->As_BuildingGameObj(), objPos, &closestPoly);
					isValidTarget = Vector3::Distance(objPos, closestPoly) < damageRadius;
				}
				else
				{
					auto &boundingBox = bld->As_PhysicalGameObj()->Peek_Model()->Get_Bounding_Box();
					isValidTarget = Vector3::Distance(objPos, boundingBox.Center) < damageRadius;
					isVehicleTarget = !Is_Inside_AABox(boundingBox, objPos);
				}
			}
			if (isValidTarget)
			{
				StringClass params;
				// Send entire team to valid beacon to disarm it and eliminate the planter/defenders
				params.Format("%d,%d,%d,%d,%d,%d,%d", team, dp88_AI_Objective::TYPE_ENGINEERING, 1, 1000, 0, 0, 0);
				Commands->Attach_Script(object, "dp88_AI_Objective", params.Peek_Buffer());
				params.Format("%d,%d,%d,%d,%d,%d,%d", team, dp88_AI_Objective::TYPE_OFFENSIVE, 4, 1000, 0, 0, 0);
				Commands->Attach_Script(object, "dp88_AI_Objective", params.Peek_Buffer());
				params.Format("%d,%d,%d,%d,%d,%d,%d", team, dp88_AI_Objective::TYPE_DEFENSIVE, 4, 1000, 0, 0, 0);
				Commands->Attach_Script(object, "dp88_AI_Objective", params.Peek_Buffer());
				Commands->Attach_Script(object, "dp88_AI_Marker_Repairable", "5.0");
				// Also send vehicles if it's placed outside
				if (isVehicleTarget)
				{
					params.Format("%d,%d,%d,%d,%d,%d,%d", team, dp88_AI_Objective::TYPE_OFFENSIVE, 10, 0, 1000, 1000, 1000);
					Commands->Attach_Script(object, "dp88_AI_Objective", params.Peek_Buffer());
				}
				if (BuildingDebugLocationsDisplayEnabled)
				{
					StringClass debug;
					debug.Format("msg %s: instance now marked as a target for bots", Commands->Get_Preset_Name(object));
					M_Console_Input(debug.Peek_Buffer());
				}
			}
		}

		if (!AIBuildings.Count())
			return;

		// Find C4s around friendly buildings, attach objectives to them
		for (auto node = GameObjManager::C4GameObjList.Head(); node; node = node->Next())
		{
			auto object = node->Data();
			// Skip friendly C4s and ones that already have a marker on them
			if (Get_Object_Type(object) == team || Is_Script_Attached(object, "dp88_AI_Marker_Repairable"))
				continue;

			Vector3 objPos = Commands->Get_Position(object);
			auto bld = Find_Closest_AI_Building(objPos);
			// This check is necessary as it can happen that only the other team has buildings, and this team does not
			if (!bld)
				continue;
			auto &boundingBox = bld->As_BuildingGameObj() ? bld->As_BuildingGameObj()->Get_Bounds() : bld->As_PhysicalGameObj()->Peek_Model()->Get_Bounding_Box();
			// Attach objective if is inside the building
			if (Is_Inside_AABox(boundingBox, objPos))
			{
				StringClass params;
				params.Format("%d,%d,%d,%d,%d,%d,%d", team, dp88_AI_Objective::TYPE_ENGINEERING, 1, 5, 0, 0, 0);
				Commands->Attach_Script(object, "dp88_AI_Objective", params.Peek_Buffer());
				Commands->Attach_Script(object, "dp88_AI_Marker_Repairable", "5.0");

				if (BuildingDebugLocationsDisplayEnabled)
				{
					StringClass debug;
					debug.Format("msg %s: instance now marked as a target for bots", Commands->Get_Preset_Name(object));
					M_Console_Input(debug.Peek_Buffer());
				}
			}
		}
	}


public:
	int Signup_For_Next_Objective_Type(GameObject *botCtrl)
	{
		if (nextObjectiveType == dp88_AI_Objective::TYPE_ENGINEERING)
			engineers.Add(botCtrl);
		else if (engineers.ID(botCtrl) != -1)
			engineers.DeleteObj(botCtrl);

		if (nextObjectiveType == dp88_AI_Objective::TYPE_DEFENSIVE)
			defenders.Add(botCtrl);
		else if (defenders.ID(botCtrl) != -1)
			defenders.DeleteObj(botCtrl);


		return nextObjectiveType;
	}

	const char *Get_Infantry_List()
	{
		if (nextObjectiveType == dp88_AI_Objective::TYPE_ENGINEERING)
			return Get_Parameter("InfListEngineering");
		else if (nextObjectiveType == dp88_AI_Objective::TYPE_DEFENSIVE)
			return Get_Parameter("InfListDefensive");
		
		return Get_Parameter("InfListOffensive");
	}

	void Report_Bot_Died(GameObject *botCtrl)
	{
		if (engineers.ID(botCtrl) != -1)
			engineers.DeleteObj(botCtrl);

		if (defenders.ID(botCtrl) != -1)
			defenders.DeleteObj(botCtrl);
	}

	void Activate_Defense_Protocol(GameObject *obj)
	{
		if (M_Has_Timer(obj, this, TIMER_AI_DEFEND))
		{
			M_Stop_Timer2(obj, this, TIMER_AI_DEFEND);
		}
		Commands->Start_Timer(obj, this, 60.0f, TIMER_AI_DEFEND);
	}
};
REGISTER_SCRIPT(MS_AI_Team_Controller, 
	"Team=0:int,"
	"InfListOffensive:string,"
	"InfListDefensive:string,"
	"InfListEngineering:string"
);


/*!
* \brief AI player count controller
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* Subject to change
*
* \param Nothing
*   Nothing
*/
class MS_AI_Player_Count_Controller : public ScriptImpClass
{
	int maxBotCount;
	int spawnTeam;
	const char *spawnPresetTeam0;
	const char *spawnPresetTeam1;

	DynamicVectorClass<GameObject*> spawnedControllersTeam0;
	DynamicVectorClass<GameObject*> spawnedControllersTeam1;

	void Created(GameObject *obj) override
	{
		if (!Init_Parameters(obj))
			return;

		// Create the AI team brains
		StringClass params;

		auto team0TeamCtrl = Commands->Create_Object("Invisible_Object", Vector3());
		params.Format("0,%s,%s,%s", 
			Get_Parameter("InfListOffensiveTeam0"), 
			Get_Parameter("InfListDefensiveTeam0"),
			Get_Parameter("InfListEngineeringTeam0")
		);
		Commands->Attach_Script(team0TeamCtrl, "MS_AI_Team_Controller", params.Peek_Buffer());

		auto team1TeamCtrl = Commands->Create_Object("Invisible_Object", Vector3());
		params.Format("1,%s,%s,%s", 
			Get_Parameter("InfListOffensiveTeam1"),
			Get_Parameter("InfListDefensiveTeam1"),
			Get_Parameter("InfListEngineeringTeam1")
		);
		Commands->Attach_Script(team1TeamCtrl, "MS_AI_Team_Controller", params.Peek_Buffer());

		if (!Is_Pathfind_Generated())
		{
			M_Console_Input("msg WARNING: Pathfind sectors were not generated!");
		}

		Commands->Start_Timer(obj, this, 2.0f, TIMER_AI_CONTROLLER_PLAYERCHECK);
	}

	void Timer_Expired(GameObject *obj, int number) override
	{
		if (number == TIMER_AI_CONTROLLER_PLAYERCHECK)
		{
			// Correction if count is odd
			int correction = spawnTeam != 2 ? 0 : (maxBotCount % 2 == 1 ? 1 : 0);

			int botCountDiffTeam0 = spawnTeam == 1 ? -spawnedControllersTeam0.Count() : (spawnTeam == 2 ? maxBotCount / 2 : maxBotCount) + correction - Get_Team_Player_Count(0) - spawnedControllersTeam0.Count();
			int botCountDiffTeam1 = spawnTeam == 0 ? -spawnedControllersTeam1.Count() : (spawnTeam == 2 ? maxBotCount / 2 : maxBotCount) + correction - Get_Team_Player_Count(1) - spawnedControllersTeam1.Count();

			// Fixes to team 0
			if (botCountDiffTeam0 > 0)
			{
				for (int i = 0; i < botCountDiffTeam0; i++)
					Create_Player_Controller(obj, 0);
			}
			else if (botCountDiffTeam0 < 0)
			{
				for (int i = 0; i < abs(botCountDiffTeam0); i++)
				{
					if (spawnedControllersTeam0.Count() == 0)
						break;

					GameObject *controller = spawnedControllersTeam0[spawnedControllersTeam0.Count() - 1];
					spawnedControllersTeam0.DeleteObj(controller);
					Commands->Destroy_Object(controller);
				}
			}

			// Fixes to team 1
			if (botCountDiffTeam1 > 0)
			{
				for (int i = 0; i < botCountDiffTeam1; i++)
					Create_Player_Controller(obj, 1);
			}
			else if (botCountDiffTeam1 < 0)
			{
				for (int i = 0; i < abs(botCountDiffTeam1); i++)
				{
					if (spawnedControllersTeam1.Count() == 0)
						break;

					GameObject *controller = spawnedControllersTeam1[spawnedControllersTeam1.Count() - 1];
					spawnedControllersTeam1.DeleteObj(controller);
					Commands->Destroy_Object(controller);
				}
			}
			Commands->Start_Timer(obj, this, 5.0f, TIMER_AI_CONTROLLER_PLAYERCHECK);
		}
	}

	void Custom(GameObject *obj, int type, int param, GameObject *sender) override
	{
		if (type == CUSTOM_AI_CHANGE_BOTCOUNT)
		{
			maxBotCount = param;

			// For safety, set max and min limit
			if (maxBotCount >= 300)
				maxBotCount = 300;

			else if (maxBotCount < 0)
				maxBotCount = 0;

			// TODO: THIS IS A SMALL PROBLEM
			// Commands->Notify_Bot_Count_Listeners(maxBotCount);
		}
		else if (type == CUSTOM_AI_CHANGE_BOTCOUNTTEAM)
		{
			spawnTeam = param;
		}
	}

	void Create_Player_Controller(GameObject *obj, int team)
	{
		auto &spawnedControllers = team == 0 ? spawnedControllersTeam0 : spawnedControllersTeam1;

		int objectiveType = 1;
		// TODO: SORT OUT LATER
		/*
		int objectiveType = 3;

		// 1 engineer bots max for both teams
		if (spawnedControllers.Count() >= 1)
			objectiveType = 2;

		// 2 defensive bots max for both teams
		if (spawnedControllers.Count() >= 3)
			objectiveType = 1;
		*/

		const char *spawnerName;
		const char *infPresetList;
		const char *vehPresetList;
		if (team)
		{
			spawnerName = spawnPresetTeam1;
			if (objectiveType == 3)
				infPresetList = Get_Parameter("InfListEngineeringTeam1");
			else if (objectiveType == 2)
				infPresetList = Get_Parameter("InfListDefensiveTeam1");
			else
				infPresetList = Get_Parameter("InfListOffensiveTeam1");

			vehPresetList = Get_Parameter("VehListTeam1");
		}
		else
		{
			spawnerName = spawnPresetTeam0;
			if (objectiveType == 3)
				infPresetList = Get_Parameter("InfListEngineeringTeam0");
			else if (objectiveType == 2)
				infPresetList = Get_Parameter("InfListDefensiveTeam0");
			else
				infPresetList = Get_Parameter("InfListOffensiveTeam0");

			vehPresetList = Get_Parameter("VehListTeam0");
		}

		Vector3 createPos = Commands->Get_Position(obj);
		GameObject *spawnedObject = Commands->Create_Object("Invisible_Object", createPos);
		spawnedControllers.Add(spawnedObject);

		// Attach script to spawned object so the controller and child can communicate
		StringClass params;
		params.Format("%d,%s,%s,%s,%d", team, spawnerName, infPresetList, vehPresetList, objectiveType);
		Commands->Attach_Script(spawnedObject, "MS_AI_Player_Controller", params.Peek_Buffer());
	}

	StringClass Validate_Twiddler_Param(const char *paramName)
	{
		auto twiddlerName = Get_Parameter(paramName);
		if (strcmp(twiddlerName, "USEPLAYERPURCHASELISTS") != 0 &&
			strcmp(twiddlerName, "AUTOENGINEER") != 0 &&
			strcmp(twiddlerName, "") != 0 && strcmp(twiddlerName, "null") != 0 &&
			!Is_Valid_Preset(twiddlerName))
		{
			StringClass errorMessage;
			errorMessage.Format("The script has an invalid %s parameter: %s", paramName, twiddlerName);
			return errorMessage;
		}
		return "";
	}

	bool Init_Parameters(GameObject *obj)
	{
		bool valid = true;
		StringClass errorMessage;
		StringClass checkedError;

		maxBotCount = Get_Int_Parameter("Count");
		if (valid && maxBotCount < 0)
		{
			errorMessage.Format("The script has the Count parameter as negative: %d", maxBotCount);
			valid = false;
		}

		if (valid)
			checkedError = Validate_Twiddler_Param("InfListOffensiveTeam0");
	
		if (checkedError.Is_Empty())
			checkedError = Validate_Twiddler_Param("InfListOffensiveTeam1");
		
		if (checkedError.Is_Empty())
			checkedError = Validate_Twiddler_Param("InfListDefensiveTeam0");
		
		if (checkedError.Is_Empty())
			checkedError = Validate_Twiddler_Param("InfListDefensiveTeam1");

		if (checkedError.Is_Empty())
			checkedError = Validate_Twiddler_Param("InfListEngineeringTeam0");

		if (checkedError.Is_Empty())
			checkedError = Validate_Twiddler_Param("InfListEngineeringTeam1");

		if (checkedError.Is_Empty())
			checkedError = Validate_Twiddler_Param("VehListTeam0");

		if (checkedError.Is_Empty())
			checkedError = Validate_Twiddler_Param("VehListTeam1");

		if (!checkedError.Is_Empty())
		{
			errorMessage = checkedError;
			valid = false;
		}

		spawnPresetTeam0 = Get_Parameter("Team0SpawnAtPreset");
		if (valid && strcmp(spawnPresetTeam0, "USEPLAYERSPAWNERS") != 0 && !Is_Valid_Preset(spawnPresetTeam0))
		{
			errorMessage.Format("The script has an invalid Team0SpawnAtPreset parameter: %s", spawnPresetTeam0);
			valid = false;
		}
		spawnPresetTeam1 = Get_Parameter("Team1SpawnAtPreset");
		if (valid && strcmp(spawnPresetTeam1, "USEPLAYERSPAWNERS") != 0 && !Is_Valid_Preset(spawnPresetTeam1))
		{
			errorMessage.Format("The script has an invalid Team1SpawnAtPreset parameter: %s", spawnPresetTeam1);
			valid = false;
		}
		spawnTeam = Get_Int_Parameter("SpawnTeam");
		if (valid && (spawnTeam < 0 || spawnTeam > 2))
		{
			errorMessage.Format("The script has the SpawnTeam parameter not as 0, 1 or 2: %d", maxBotCount);
			valid = false;
		}

		if (!valid)
		{
			StringClass msg;
			msg.Format("[%d:%hs:%hs] Critical Error: %hs. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), errorMessage.Peek_Buffer());
			M_Console_Output(msg.Peek_Buffer());
			Destroy_Script();
		}
		return valid;
	}
};
REGISTER_SCRIPT(MS_AI_Player_Count_Controller,
	"Count=0:int,"
	"InfListOffensiveTeam1=USEPLAYERPURCHASELISTS:string,"
	"InfListDefensiveTeam1=USEPLAYERPURCHASELISTS:string,"
	"InfListEngineeringTeam1=AUTOENGINEER:string,"
	"VehListTeam1=USEPLAYERPURCHASELISTS:string,"
	"InfListOffensiveTeam0=USEPLAYERPURCHASELISTS:string,"
	"InfListDefensiveTeam0=USEPLAYERPURCHASELISTS:string,"
	"InfListEngineeringTeam0=AUTOENGINEER:string,"
	"VehListTeam0=USEPLAYERPURCHASELISTS:string,"
	"Team1SpawnAtPreset=USEPLAYERSPAWNERS:string,"
	"Team0SpawnAtPreset=USEPLAYERSPAWNERS:string,"
	"SpawnTeam=2:int"
);


/*!
* \brief AI player controller
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* A controller script for creating/respawning one AI unit, tracking credits and purchasable unit lists.
* Also implements player-like thinking and more complicated AI decisions, e.g. cooperating with players.
*
* \param Nothing
*   Nothing
*/
class MS_AI_Player_Controller : public ScriptImpClass
{
	template<typename>
	friend class ScriptRegistrant;

	int team;
	int objectiveType;
	StringClass botName;
	const char *teamSpawnPreset;
	const char *spawnAtPresetName;
	const char *soldierPresetList;
	const char *vehPresetList;

	ReferencerClass soldierObject;
	ReferencerClass vehicleObject;
	//int soldierObjectID;
	//int vehicleObjectID;
	// This can be either the soldier or the vehicle
	ReferencerClass coopObject;
	//int coopObjectID;
	int vehicleConstructWaitCount;
	/* States:
	0 = not cooping
	1 = group rushing
	2 = following player
	3 = covering player
	4 = defending base
	5 = moving to ordered position
	6 = attacking ordered target
	7 = defending ordered target
	8 = unavailable, going to refill or other stuff
	*/
	int coopState;
	ReferencerClass coopTarget;
	//int coopTargetID;
	long coopFriendPlayerID;
	Vector3 coopFriendPrevPos;
	bool enteredVeh;
	// ID of the vehicle the soldier exited last, should not be reentered automatically
	ReferencerClass bannedVehicle;
	//int bannedVehicleID;
	float money;
	float expenses;

	ReferencerClass tempObjective;
	//int tempObjectiveID;

	// Help prevent refill spamming
	bool isRefillCooldownInProgress;

	// This is used so Detach and Destroy don't both try to clean up
	bool isDeleted;


	// Helpers for timed reaction ability
	bool isProcessingRadioMessage;
	int radioMessageID;
	int taskType;
	int taskObjID;

	BotPersonality *personality;

	PlayerRelationData playerRelationData;

	DynamicVectorClass<int> enemiesOnRadar;
	DynamicVectorClass<int> allyPlayersAround;

	MS_AI_Player_Controller()
		: personality(nullptr)
	{

	}

	~MS_AI_Player_Controller()
	{
		REF_PTR_RELEASE(personality);
	}

	void Created(GameObject* obj) override
	{
		team = 0;
		objectiveType = 1;
		teamSpawnPreset = nullptr;
		spawnAtPresetName = nullptr;
		soldierPresetList = nullptr;
		vehPresetList = nullptr;
		soldierObject = nullptr;
		vehicleObject = nullptr;
		coopObject = nullptr;
		vehicleConstructWaitCount = 0;
		coopState = 0;	
		coopTarget = nullptr;
		coopFriendPlayerID = 0;
		coopFriendPrevPos = Vector3();
		enteredVeh = false;
		bannedVehicle = nullptr;
		money = 0;
		expenses = 0;
		tempObjective = nullptr;
		isRefillCooldownInProgress = false;
		isDeleted = false;
		isProcessingRadioMessage = false;
		radioMessageID = 0;
		taskType = 0;
		taskObjID = 0;
		personality = nullptr;

		if (!Init_Parameters(obj))
			return;

		// Let Global Controller know we exist
		MS_AI_Global_Controller::GetInstance().botControllersTeam[team].Add(obj);

		// Bot name will be default if this script is created before the count controller is ready (needs a second to read ini)
		botName = MS_AI_Global_Controller::GetInstance().Get_Free_Bot_Name();
		// Personality will be tied to bot name later
		// Ref count 1
		personality = new BotPersonality(FreeRandom.Get_Int(3)); // 0, 1, 2

		StringClass buff;
		buff.Format("%s %s", botName.Peek_Buffer(), Get_Translated_String(Get_String_ID_By_Desc("IDS_MP_RENEGADE_JOINS")));
		Send_Message(MS_RGB_JOINLEFT_MSG, buff.Peek_Buffer());

		// Set team so when sending customs, this object's team can be checked
		Set_Object_Type(obj, team);

		if (The_Cnc_Game())
			money = (float)The_Cnc_Game()->Get_Starting_Credits();
		else if (The_Skirmish_Game())
			money = (float)The_Skirmish_Game()->Get_Starting_Credits();

		teamSpawnPreset = team ? Get_GDI_Soldier_Name() : Get_Nod_Soldier_Name();
		
		Spawn_Object(obj);
	}
	
	void Custom(GameObject *obj, int type, int param, GameObject *sender) override
	{
		// Respawn AI player if it died
		if (type == CUSTOM_AI_SOLDIER_DIED)
		{
			if (tempObjective) {
				Commands->Destroy_Object(tempObjective);
				tempObjective = nullptr;
			}
			if (coopState)
			{
				coopState = 0;
				if (coopObject == soldierObject)
					coopObject = nullptr;

				MS_AI_Global_Controller::GetInstance().Unregister_From_GroupRush(team, obj->Get_ID());
			}

			if (auto teamCtrlScript = Get_Team_Controller())
			{
				teamCtrlScript->Report_Bot_Died(obj);
			}

			soldierObject = nullptr;
			bannedVehicle = nullptr;
			// Cleanup after special cases (e.g. cyborg reaper double death +  removes all scripts from passenger, problems.)
			enteredVeh = false;

			if (isDeleted)
				return;

			Commands->Start_Timer(obj, this, 5.0f, TIMER_AI_OBJECT_SPAWN_COUNTDOWN);
			Commands->Send_Custom_Event(obj, obj, CUSTOM_AI_COOP_PLAYER_FOLLOW_STOP, 0, 0.0f);

		}
		else if (type == CUSTOM_AI_VEHICLE_DIED || type == CUSTOM_AI_VEHICLE_EXITED)
		{
			if (tempObjective) {
				Commands->Destroy_Object(tempObjective);
				tempObjective = nullptr;
			}
			// If it was the vehicle, activate AI on the soldier
			if (enteredVeh)
			{
				if (coopState)
				{
					coopState = 0;
					if (coopObject == vehicleObject)
						coopObject = nullptr;

					MS_AI_Global_Controller::GetInstance().Unregister_From_GroupRush(team, obj->Get_ID());
				}

				if (vehicleObject)
				{
					bannedVehicle = vehicleObject;
					vehicleObject = nullptr;
				}
				enteredVeh = false;

				if (isDeleted)
					return;

				if (soldierObject)
				{
					// Look for another vehicle, otherwise activate AI
					if (!Search_For_Vehicle(obj, 30))
						Activate_AI_Behaviour(obj, soldierObject);
				}
			}
		}
		else if (type == CUSTOM_AI_REQUEST_REFUND)
		{
			money += expenses;
			expenses = 0;
		}
		else if (type == CUSTOM_AI_SEND_PUBLIC_MESSAGE)
		{
			if (param == 0)
			{
				if (MS_AI_Global_Controller::GetInstance().killedRespEnabled && personality->Get_Random(BotPersonality::CHAT_CHANCE))
				{
					StringClass response = MS_AI_Global_Controller::GetInstance().Get_Killed_Response();
					Send_Public_Message_String(response.Peek_Buffer());
				}
			}
			else if (param == 1)
			{
				if (MS_AI_Global_Controller::GetInstance().killerRespEnabled && personality->Get_Random(BotPersonality::CHAT_CHANCE))
				{
					StringClass response = MS_AI_Global_Controller::GetInstance().Get_Killer_Response();
					Send_Public_Message_String(response.Peek_Buffer());
				}
			}
		}
		else if (type == CUSTOM_AI_COOP_PLAYER_RADIO_MESSAGE)
		{
			React_To_Radio_Message(sender, radioMessageID, taskType, taskObjID);
			radioMessageID = 0;
			taskType = 0;
			taskObjID = 0;
			isProcessingRadioMessage = false;
		}
		else if (type == CUSTOM_AI_COOP_ARRIVED_TO_RALLYPOINT)
		{
			MS_AI_Global_Controller::GetInstance().Object_Arrived(team, obj->Get_ID());
			if (Search_For_Vehicle(obj, 20))
				MS_AI_Global_Controller::GetInstance().Unregister_From_GroupRush(team, obj->Get_ID());
		}
		else if (type == CUSTOM_AI_COOP_NEXT_RALLYPOINT)
		{
			Commands->Send_Custom_Event(obj, coopObject, CUSTOM_AI_COOP_NEXT_RALLYPOINT, param, 0.0);
		}
		else if (type == CUSTOM_AI_COOP_GROUPRUSH_COMPLETE || type == CUSTOM_AI_COOP_STOP_SECONDARY_OBJECTIVE)
		{
			coopState = 0;
		}
		else if (type == CUSTOM_AI_COOP_PLAYER_FOLLOW_STOP)
		{
			coopState = 0;
			coopFriendPlayerID = 0;
		}
		else if (type == CUSTOM_AI_SEARCH_FOR_VEHICLE)
		{
			if (!coopState)
				Search_For_Vehicle(obj, 15);
		}
		else if (type == CUSTOM_AI_ADD_RADAR_TARGET)
		{
			if (param == 0)
				enemiesOnRadar.Clear();
			else {
				auto enemyObj = Commands->Find_Object(param);
				float checkRange = MS_AI_Global_Controller::GetInstance().radarRadius;

				// Check range
				if (soldierObject && enemyObj &&
					Vector3::Distance_Squared(Commands->Get_Position(soldierObject), Commands->Get_Position(enemyObj)) < checkRange * checkRange)
				{
					enemiesOnRadar.Add(param);
					/*StringClass buff;
					buff.Format("msg %s: Enemy on radar: %s", botName.Peek_Buffer(), Commands->Get_Preset_Name(enemyObj));
					M_Console_Input(buff.Peek_Buffer());*/
				}
			}
		}
		else if (type == CUSTOM_AI_ADD_ALLY_TARGET)
		{
			if (param == 0)
				allyPlayersAround.Clear();
			else {
				auto otherObj = Commands->Find_Object(param);

				float checkRange = Get_Vehicle(soldierObject) ? 35.0f : 15.0f;
				// Check range
				if (soldierObject && otherObj &&
					Vector3::Distance_Squared(Commands->Get_Position(soldierObject), Commands->Get_Position(otherObj)) < checkRange * checkRange)
				{
					allyPlayersAround.Add(param);
				}
			}
		}
		else if (type == CUSTOM_AI_CHECK_RADAR_TARGETS)
		{
			// Update sympathy and with a chance, stop following/covering idle players
			if (coopState == 2 || coopState == 3)
			{
				auto playerObj = Get_GameObj(coopFriendPlayerID);
				if (playerObj && coopObject)
				{
					auto name = Get_Player_Name_By_ID(coopFriendPlayerID);
					// Find sympathy of this guy
					auto sympathyData = playerRelationData.GetPlayerData(name);
					if (!sympathyData)
						return;

					float checkRange = coopObject->As_VehicleGameObj() ? 35.0f : 15.0f;
					// Update sympathy
					if (Vector3::Distance_Squared(Commands->Get_Position(playerObj), coopFriendPrevPos) < 2*2 && 
						Vector3::Distance_Squared(Commands->Get_Position(playerObj), Commands->Get_Position(coopObject)) < checkRange * checkRange)
					{
						if (sympathyData->sympathy > 0.1f)
							sympathyData->sympathy -= personality->Get_Random(BotPersonality::SYMPATHY_VARIABLE) / 2;
						else if (sympathyData->sympathy < 0.1f && FreeRandom.Get_Float() > sympathyData->sympathy)
							Commands->Send_Custom_Event(obj, coopObject, CUSTOM_AI_COOP_PLAYER_FOLLOW_STOP, 0, 0.0f);
					}
					else 
					{
						if (sympathyData->sympathy < 1.0f)
							sympathyData->sympathy += personality->Get_Random(BotPersonality::SYMPATHY_VARIABLE) / 4;
					}

					/*WideStringClass str;
					str.Format(L"msg %S: Updated sympathy for %S: %.3f", botName.Peek_Buffer(), name.Peek_Buffer(), sympathyData->sympathy);
					M_Console_Input(str);*/

					coopFriendPrevPos = Commands->Get_Position(playerObj);
				}
			}

			if (coopState != 0)
				return;
			
			auto soldierObj = soldierObject.Get_Ptr();
			if (!soldierObj)
				return;

			auto aiObj = soldierObj;

			if (enteredVeh)
			{
				auto vehObj = vehicleObject.Get_Ptr();
				if (vehObj && vehObj->As_VehicleGameObj() && vehObj->As_VehicleGameObj()->Get_Driver() == soldierObj)
					aiObj = vehObj;
				// Soldier inside a vehicle he's not driving: ignore radar targets.
				else if (Get_Vehicle(soldierObj))
					return;
			}

			auto behaviourScript = (MS_AI_Behaviour*)(Find_Script_On_Object(aiObj, "MS_AI_Behaviour"));
			if (!behaviourScript)
				return;

			if (personality->Get_Random(BotPersonality::INVESTIGATE_CHANCE))
			{
				// Find closest enemy
				float min_dist_sq = FLT_MAX;
				GameObject *chosenEnemy = nullptr;
				for (int i = 0; i < enemiesOnRadar.Count(); i++)
				{
					auto enemy = Commands->Find_Object(enemiesOnRadar[i]);
					// Also check if the bot cares about it (e.g. conscript ignores vehicles (-1 prio) unless it has no objectives)
					if (!enemy || Get_Vehicle(enemy) || Commands->Get_Health(enemy) <= 1.0f ||
						!behaviourScript->Should_Chase_Radar_Target(enemy) )
						continue;

					float dist_sq = Vector3::Distance_Squared(Commands->Get_Position(aiObj), Commands->Get_Position(enemy));
					if (dist_sq < min_dist_sq)
					{
						chosenEnemy = enemy;
						min_dist_sq = dist_sq;
					}
				}

				if (chosenEnemy)
				{
					int enemyID = chosenEnemy->Get_ID();
					/*StringClass buff;
					buff.Format("msg %s: Enemy chosen: %s", botName.Peek_Buffer(), Commands->Get_Preset_Name(chosenEnemy));
					M_Console_Input(buff.Peek_Buffer());*/

					Create_Timed_Objective(team, 30.0f, Commands->Get_Position(chosenEnemy), 1, enemyID, enemyID);

					coopState = 6;
					coopTarget = chosenEnemy;
					Commands->Start_Timer(obj, this, 0.0f, TIMER_AI_COOP_RETRY_REQUEST);
					return;
				}
			}

			if (objectiveType != 1)
				return;

			// Nothing? Then look around for friendlies and cooperate with them
			if (!M_Has_Timer(obj, this, TIMER_AI_COOP_PAUSE_INITIATION) && personality->Get_Random(BotPersonality::TEAMWORK_CHANCE))
			{
				// Find closest, sympathic ally
				float min_dist_sq = FLT_MAX;
				GameObject *chosenTarget = nullptr;
				for (int i = 0; i < allyPlayersAround.Count(); i++)
				{
					auto ally = Commands->Find_Object(allyPlayersAround[i]);

					// Also check if the bot cares about it
					if (!ally || Commands->Get_Health(ally) <= 1.0f)
						continue;

					if (ally->As_VehicleGameObj() && ally->As_VehicleGameObj()->Get_Driver())
						ally = ally->As_VehicleGameObj()->Get_Driver();

					// Decreased chance of teamwork if ally is not the same gameobject type (soldier-soldier, vehicle-vehicle)
					if ((!aiObj->As_VehicleGameObj() && Get_Vehicle(ally)) || (aiObj->As_VehicleGameObj() && !Get_Vehicle(ally)) && FreeRandom.Get_Float() < 0.5f)
						continue;

					// Get name of this guy
					auto name = Get_Player_Name(ally);
					// Find sympathy of this guy
					auto sympathyData = playerRelationData.GetPlayerData(name);
					if (!sympathyData)
						continue;

					// Update sympathy
					if (sympathyData->sympathy < 1.0f)
						sympathyData->sympathy += personality->Get_Random(BotPersonality::SYMPATHY_VARIABLE);

					/*WideStringClass str;
					str.Format(L"msg %S: Updated sympathy for %S: %.3f", botName.Peek_Buffer(), Get_Player_Name(ally).Peek_Buffer(), sympathyData->sympathy);
					M_Console_Input(str);*/

					if (FreeRandom.Get_Float() < sympathyData->sympathy)
						continue;

					float dist_sq = Vector3::Distance_Squared(Commands->Get_Position(aiObj), Commands->Get_Position(ally));
					if (dist_sq < min_dist_sq)
					{
						chosenTarget = ally;
						min_dist_sq = dist_sq;
					}
				}

				if (chosenTarget)
				{
					StringClass chosenName = Get_Player_Name(chosenTarget);
					// Who should follow who? Less sympathy: higher chance the bot asks the player to follow and not the other way around.
					auto sympathyData = playerRelationData.GetPlayerData(chosenName);
					if (FreeRandom.Get_Float() < sympathyData->sympathy) {
						coopFriendPlayerID = Get_Player_ID(chosenTarget);

						coopState = 3;
						Commands->Start_Timer(obj, this, 0.0f, TIMER_AI_COOP_RETRY_REQUEST);
						Play_Radio_Message(Radio_Message_Type::MOVE_OUT);
					}
					else
					{
						Commands->Start_Timer(obj, this, 60.0f, TIMER_AI_COOP_PAUSE_INITIATION);
						if (personality->Get_Random(BotPersonality::CHAT_CHANCE))
						{
							Play_Radio_Message(Radio_Message_Type::FOLLOW_ME);
						}
						else if (personality->Get_Random(BotPersonality::CHAT_CHANCE))
						{
							DynamicVectorClass<StringClass> followMeMsgs;
							followMeMsgs.Add("Let's go");
							followMeMsgs.Add("Come on");
							followMeMsgs.Add("Come with me");
							followMeMsgs.Add("Follow me");
							StringClass response = MS_AI_Global_Controller::GetInstance().Get_Chat_Response(followMeMsgs);
							Send_Private_Message_String(response.Peek_Buffer(), chosenTarget);
						}
					}
				}
			}
		}
		else if (type == CUSTOM_AI_REQUEST_PATROL_POINT)
		{
			auto patrolPointID = MS_AI_Global_Controller::GetInstance().Get_New_Patrol_Point_ID(team, param, sender->As_VehicleGameObj() ? true : false);
			if (patrolPointID)
				Commands->Send_Custom_Event(obj, sender, CUSTOM_AI_GO_TO_PATROL_POINT, patrolPointID, 0.0f);
		}
		else if (type == CUSTOM_AI_VEHICLE_AMMO_EMPTY)
		{
			auto botVeh = Commands->Find_Object(param);
			if (!botVeh)
				return;

			auto markerScript = Find_Script_On_Object(botVeh, "MS_AI_Marker_Refiller_Vehicle");
			if (!markerScript)
				return;

			auto presetName = ((MS_AI_Marker_Refiller_Vehicle *)markerScript)->Get_Zone_Preset_Name();
			auto exitZonePresetName = ((MS_AI_Marker_Refiller_Vehicle *)markerScript)->Get_Exit_Zone_Preset_Name();

			if (auto zoneObj = Find_Nearest_Preset(Commands->Get_Position(botVeh), presetName))
			{
				Commands->Destroy_Object(tempObjective);
				auto refillObjective = Commands->Create_Object("Invisible_Object", Commands->Get_Position(zoneObj));
				tempObjective = refillObjective;

				// Create objective for the bot
				StringClass params;
				params.Format("%d,%d,%d,%d,%d,%d,%d", team, tempObjective->Get_ID(), 5, 1000, 1000, 1000, 1000);
				Commands->Attach_Script(refillObjective, "dp88_AI_Objective", params.Peek_Buffer());

				Commands->Send_Custom_Event(obj, botVeh, CUSTOM_AI_GO_TO_RELOAD, tempObjective->Get_ID(), 0.0f);
			}
			else if (auto zoneObj2 = exitZonePresetName ? Find_Nearest_Preset(Commands->Get_Position(botVeh), exitZonePresetName) : nullptr)
			{
				Commands->Destroy_Object(tempObjective);
				auto exitObjective = Commands->Create_Object("Invisible_Object", Commands->Get_Position(zoneObj2));
				tempObjective = exitObjective;

				// Create objective for the bot
				StringClass params;
				params.Format("%d,%d,%d,%d,%d,%d,%d", team, tempObjective->Get_ID(), 5, 1000, 1000, 1000, 1000);
				Commands->Attach_Script(exitObjective, "dp88_AI_Objective", params.Peek_Buffer());

				Commands->Send_Custom_Event(obj, botVeh, CUSTOM_AI_GO_TO_EXIT_VEHICLE, tempObjective->Get_ID(), 0.0f);
			}
			// No zones present, leave the vehicle
			else if (soldierObject)
			{
				VehicleGameObj *vObj = botVeh->As_VehicleGameObj();
				Commands->Enable_Vehicle_Transitions(vObj, true);
				Force_Occupants_Exit(vObj);

				Commands->Send_Custom_Event(obj, botVeh, CUSTOM_AI_DEACTIVATE_AI, 0, 0.4f);
			}
		}
		else if (type == CUSTOM_AI_REFILL_COMPLETE)
		{
			Commands->Destroy_Object(tempObjective);
			tempObjective = nullptr;
			coopState = 0;
		}
		else if (type == CUSTOM_AI_ARRIVED_TO_REFILL)
		{
			if (sender && sender->As_SoldierGameObj() && Commands->Get_Health(sender) >= 1.0f)
			{
				/*if (sender->As_SoldierGameObj()->Can_Refill())
				{*/
					Grant_Refill(sender);
					isRefillCooldownInProgress = true;
					Commands->Start_Timer(obj, this, 8.0f, TIMER_AI_REFILL_COOLDOWN);
				//}
				// CUSTOM_AI_REFILL_COMPLETE will never arrive, because we remove MS_AI_Behaviour here!
				Commands->Destroy_Object(tempObjective);
				tempObjective = nullptr;
				coopState = 0;
				// Spawn protection and waiting before purchasing items
				Commands->Attach_Script(sender, "MS_Switch_Skin_And_Shield_Timer", "Blamo,Blamo,0");
				Remove_Script(sender, "MS_AI_Behaviour");
				Remove_Script(sender, "MS_AI_Player_Controller_Spawned_Object");
				Commands->Start_Timer(obj, this, 0.1f, TIMER_AI_OBJECT_ACTIVATE_COUNTDOWN);
			}
		}
		else if (type == CUSTOM_AI_ARRIVED_TO_EXIT_VEHICLE)
		{
			if (soldierObject)
			{
				VehicleGameObj *vObj = sender->As_VehicleGameObj();
				Commands->Enable_Vehicle_Transitions(vObj, true);
				Force_Occupants_Exit(vObj);

				Commands->Send_Custom_Event(obj, sender, CUSTOM_AI_DEACTIVATE_AI, 0, 0.4f);
			}
		}
		else if (type == CUSTOM_AI_CHECK_REPAIR_NEEDED || type == CUSTOM_AI_CHECK_GO_HOME)
		{
			// The two things do the same, one only checks HP, other wants to refill if not far away
			bool isRepairCheck = type == CUSTOM_AI_CHECK_REPAIR_NEEDED;

			if (!sender || (isRepairCheck && isRefillCooldownInProgress))
				return;

			if (sender->As_SoldierGameObj())
			{
				auto point = MS_AI_Global_Controller::GetInstance().Get_Closest_Spawn_Point(sender, team);
				if (point == Vector3())
					return;

				const float dist = Vector3::Distance(point, Commands->Get_Position(sender));
				if (isRepairCheck && !personality->Wants_To_Repair(sender, dist))
					return;

				if (!isRepairCheck && objectiveType == dp88_AI_Objective::TYPE_OFFENSIVE && dist > 100)
					return;

				// Create objective, send bot here
				Commands->Destroy_Object(tempObjective);
				auto refillObjective = Commands->Create_Object("Invisible_Object", point);
				tempObjective = refillObjective;

				// Create objective for the bot
				StringClass params;
				params.Format("%d,%d,%d,%d,%d,%d,%d", team, tempObjective->Get_ID(), 1, 1000, 1000, 1000, 1000);
				Commands->Attach_Script(refillObjective, "dp88_AI_Objective", params.Peek_Buffer());

				Commands->Send_Custom_Event(obj, sender, isRepairCheck ? CUSTOM_AI_GO_TO_REFILL : CUSTOM_AI_GO_SWITCH_OBJECTIVE, tempObjective->Get_ID(), 0.0f);
				coopState = 8;
			}
			// TODO: Objective changing not yet supported for vehicles
			else if (isRepairCheck && sender->As_VehicleGameObj())
			{
				// No money
				if (Get_Money() < Get_Team_Cost(Commands->Get_Preset_ID(sender), team) * 0.5f)
					return;

				SList<GameObject> list;
				Find_All_Objects_With_Script_By_Distance("MS_AI_Marker_Vehicle_Repair_Zone", list, Commands->Get_Position(sender));
				for (auto x = list.Head(); x != nullptr; x = x->Next())
				{
					GameObject* zoneObj = x->Data();
					auto zoneScript = (MS_AI_Marker_Vehicle_Repair_Zone *)(Find_Script_On_Object(zoneObj, "MS_AI_Marker_Vehicle_Repair_Zone"));

					// TODO: ignore occupied ones
					// Ignore if enemy team
					if (zoneScript->Get_Team() != team)
						continue;

					const float dist = Vector3::Distance(Commands->Get_Position(zoneObj), Commands->Get_Position(sender));

					if (!personality->Wants_To_Repair(sender, dist))
						return;

					const int repairObjectiveID = zoneObj->Get_ID();

					// Create objective for the bot
					if (!Is_Script_Attached(zoneObj, "dp88_AI_Objective"))
					{ 
						StringClass params;
						params.Format("%d,%d,%d,%d,%d,%d,%d", team, repairObjectiveID, 3, 1000, 1000, 1000, 1000);
						Commands->Attach_Script(zoneObj, "dp88_AI_Objective", params.Peek_Buffer());
					}

					Commands->Send_Custom_Event(obj, sender, CUSTOM_AI_GO_TO_REFILL, repairObjectiveID, 0.0f);
					coopState = 8;
					break;
				}
			}
		}
		else if (type == CUSTOM_EVENT_OWNED_VEHICLE)
		{
			// Get the vehicle object
			vehicleObject = Commands->Find_Object(param);
			Commands->Start_Timer(obj, this, 2.0f, TIMER_AI_WAIT_FOR_VEHICLE_CONSTRUCT);
		}
		else if (type == CUSTOM_AI_ARRIVED_TO_VEHICLE)
		{
			if (!soldierObject || enteredVeh)
				return;

			// If vehicle reached and is still alive
			if (param == 1 && vehicleObject)
			{
				VehicleGameObj *vehicle = vehicleObject->As_VehicleGameObj();
				bool isSoldierAlreadyInVehicle = false;
				// Check if already inside, e.g. remote controlled vehicles, cyborg reaper
				if (vehicle && vehicle->Get_Occupant_Count() && Get_Vehicle_Driver(vehicle) == soldierObject)
				{
					isSoldierAlreadyInVehicle = true;
					// Cancel any existing timeout since we successfully entered the vehicle
					if (auto spawnedScript = Find_Script_On_Object(soldierObject, "MS_AI_Player_Controller_Spawned_Object"))
					{
						if (M_Has_Timer(soldierObject, spawnedScript, TIMER_AI_GO_TO_VEHICLE_TIMEOUT))
							M_Stop_Timer2(soldierObject, spawnedScript, TIMER_AI_GO_TO_VEHICLE_TIMEOUT);
					}
				}
				// If vehicle is not full
				if ((vehicle && vehicle->Get_Occupant_Count() < vehicle->Get_Definition().Get_Seat_Count() &&
					// Must also be either empty, or friendly
					(vehicle->Get_Occupant_Count() == 0 || Get_Object_Type(vehicle) == team)/* &&
					// Attached turret case (e.g. Battle Fortress): parent must be friendly
					(!vehicle->Get_Host_Object() || Get_Object_Type(vehicle->Get_Host_Object()) == team)*/) || isSoldierAlreadyInVehicle)
				{
					// Don't take over vehicle with behaviour script
					if (Is_Script_Attached(soldierObject, "MS_AI_Behaviour"))
						Remove_Script(soldierObject, "MS_AI_Behaviour");

					// It might have become unteamed, must be teamed for bot to function properly
					Set_Object_Type(vehicle, team);
					enteredVeh = true;

					// Activate AI on vehicle if no drivers were previously in
					if (!vehicle->Get_Driver() || (isSoldierAlreadyInVehicle && !(Is_Script_Attached(vehicle, "MS_AI_Behaviour"))))
						Activate_AI_Behaviour(obj, vehicle);

					// Attach script ONCE to vehicle object so it can send custom to passengers
					if (!Is_Script_Attached(vehicle, "MS_AI_Player_Controller_Spawned_Object"))
					{
						StringClass params;
						params.Format("%d,%d", Commands->Get_ID(obj), 1);
						Commands->Attach_Script(vehicle, "MS_AI_Player_Controller_Spawned_Object", params.Peek_Buffer());
					}

					Commands->Action_Reset(soldierObject, 999.0f);

					// Put soldier in
					if (!isSoldierAlreadyInVehicle)
						Commands->Send_Custom_Event(obj, vehicle, CUSTOM_AI_VEHICLE_REQUEST_ENTER, soldierObject->Get_ID(), 0.0f);

					return;
				}
				// Else do not try re-running for it
				//M_Console_Input("msg Vehicle is full!");
			}
			// Otherwise param is 0 OR vehicle could not be entered, attach behaviour is needed or stop the overriding objective
			vehicleObject = nullptr;

			if (!Is_Script_Attached(soldierObject, "MS_AI_Behaviour"))
				Activate_AI_Behaviour(obj, soldierObject);
			else
			{
				Commands->Send_Custom_Event(obj, soldierObject, CUSTOM_AI_COOP_STOP_SECONDARY_OBJECTIVE, 0, 0.0f);
				Search_For_Vehicle(obj, 30);
			}
		}
	}

	void Timer_Expired(GameObject* obj, int number) override
	{
		if (number == TIMER_AI_OBJECT_SPAWN_COUNTDOWN)
		{
			Spawn_Object(obj);
		}
		else if (number == TIMER_AI_OBJECT_SPAWN_GHOSTING_COUNTDOWN)
		{
			if (soldierObject)
			{
				soldierObject->As_PhysicalGameObj()->Peek_Physical_Object()->Set_Collision_Group(Collision_Group_Type::SOLDIER_GHOST_COLLISION_GROUP);
			}
		}
		else if (number == TIMER_AI_OBJECT_ACTIVATE_COUNTDOWN)
		{
			if (soldierObject)
			{
				Remove_Script(soldierObject, "MS_Switch_Skin_And_Shield_Timer");
				Purchase_Presets(obj);
			}
			else
			{
				// This will happen if the soldier fell into the abyss at 0,0,0 or got killed in the 0.1 time.
				Commands->Send_Custom_Event(obj, obj, CUSTOM_AI_SOLDIER_DIED, 0, 0.0f);
			}
		}
		else if (number == TIMER_AI_WAIT_FOR_VEHICLE_CONSTRUCT)
		{
			auto vehicle = vehicleObject.Get_Ptr();
			// Always check if it still exists
			if (!vehicle || vehicleConstructWaitCount >= 20)
			{
				vehicleConstructWaitCount = 0;
				Commands->Send_Custom_Event(obj, obj, CUSTOM_AI_ARRIVED_TO_VEHICLE, 0, 0.0f);
				return;
			}

			Vector3 runLocation = Vector3();
			bool isWithinPathfindGrid = Get_Closest_Pathfind_Spot_Size(Commands->Get_Position(vehicle), 1.0f, &runLocation, 2.0f);
			if (isWithinPathfindGrid)
			{
				// If the created vehicle dies before the timer runs out, the go to action will be automatically complete.
				vehicleConstructWaitCount = 0;
				Go_To_Vehicle(obj, vehicleObject);
			}
			else
			{
				// Periodically check if it is reachable
				vehicleConstructWaitCount++;
				Commands->Start_Timer(obj, this, 2.0f, TIMER_AI_WAIT_FOR_VEHICLE_CONSTRUCT);
			}
		}
		else if (number == TIMER_AI_REFILL_COOLDOWN)
		{
			isRefillCooldownInProgress = false;
		}
		else if (number == TIMER_AI_COOP_RETRY_REQUEST)
		{
			// This should deal with cases where the joiner got kicked from the grouprush before this request applied
			if (!coopState)
				return;

			GameObject* soldier = soldierObject;
			GameObject* vehicle = vehicleObject;
			bool soldier_has_ai_behavior = soldier && Is_Script_Attached(soldier, "MS_AI_Behaviour");
			bool vehicle_has_ai_behavior = vehicle && Is_Script_Attached(vehicle, "MS_AI_Behaviour");

			if (soldier_has_ai_behavior || (vehicle_has_ai_behavior && enteredVeh))
			{
				coopObject = soldier_has_ai_behavior ? soldier : vehicle;

				// Send object to first rally point
				if (coopState == 1)
				{
					// Soldier wanting to group rush, but haven't entered vehicle yet
					if (vehicle && !enteredVeh)
					{
						Commands->Start_Timer(obj, this, 1.0f, TIMER_AI_COOP_RETRY_REQUEST);
					}
					else
					{
						Commands->Send_Custom_Event(obj, coopObject, CUSTOM_AI_COOP_NEXT_RALLYPOINT, MS_AI_Global_Controller::GetInstance().Get_Rally_Point_ID(team), 0.0);
						if (MS_AI_Global_Controller::GetInstance().debugGrouprushMessagesEnabled)
						{
							Vector3 rgb = Get_Color_For_Team(team);
							StringClass msg;
							const char *unitName = Get_Translated_Definition_Name(Commands->Get_Preset_Name(coopObject));
							msg.Format("%s: Joining group rush! (%s)", botName.Peek_Buffer(), unitName);
							Send_Message_Team(team, (int)(rgb.X * 255), (int)(rgb.Y * 255), (int)(rgb.Z * 255), msg.Peek_Buffer());
							delete[] unitName;
						}
					}
				}
				else if (coopState == 4)
				{
					int objectiveObjID = MS_AI_Global_Controller::GetInstance().Get_Temporary_Objective_ID(team);
					Commands->Send_Custom_Event(obj, coopObject, CUSTOM_AI_COOP_PLAYER_DEFEND_BASE, objectiveObjID, 0.0);
				}
				else if (coopState == 2 || coopState == 3)
				{
					auto playerObj = Get_GameObj(coopFriendPlayerID);
					int playerObjID = playerObj->Get_ID();

					if (!(Find_Script_On_Object(playerObj, "dp88_AI_Objective")))
					{
						int range = Get_Vehicle(playerObj) ? 20 : 10;
						StringClass params;
						params.Format("%d,%d,%d,%d,%d,%d,%d", team, playerObjID, range, 1000, 1000, 1000, 1000);
						Commands->Attach_Script(playerObj, "dp88_AI_Objective", params.Peek_Buffer());
					}

					if (personality->Get_Random(BotPersonality::CHAT_CHANCE))
					{
						char* text = coopState == 2 ? "I'm following you" : "I'm covering you";
						StringClass message;
						message.Format("%s, %s!", Get_Player_Name_By_ID(coopFriendPlayerID), text);
						Send_Private_Message_String(message.Peek_Buffer(), playerObj);
					}

					long custom = coopState == 2 ? CUSTOM_AI_COOP_PLAYER_FOLLOW : CUSTOM_AI_COOP_PLAYER_FOLLOW_COVER;

					Commands->Send_Custom_Event(obj, coopObject, custom, playerObjID, 0.0);
				}
				else if (coopState == 5 && coopTarget)
				{
					Commands->Send_Custom_Event(obj, coopObject, CUSTOM_AI_ORDER_MOVE, coopTarget->Get_ID(), 0.0);
				}
				else if (coopState == 6 && coopTarget)
				{
					Commands->Send_Custom_Event(obj, coopObject, CUSTOM_AI_ORDER_ATTACK, coopTarget->Get_ID(), 0.0);
				}
				else if (coopState == 7 && coopTarget)
				{
					Commands->Send_Custom_Event(obj, coopObject, CUSTOM_AI_ORDER_DEFEND, coopTarget->Get_ID(), 0.0);
				}
			}
			else
				Commands->Start_Timer(obj, this, 1.0f, TIMER_AI_COOP_RETRY_REQUEST);
		}
	}

	void Destroyed(GameObject *obj)
	{
		Controller_Gone(obj);
		Destroy_Script();
	}

	void Detach(GameObject *obj)
	{
		if (Exe != EXE_LEVELEDIT)
		{
			Controller_Gone(obj);
		}
		ScriptImpClass::Detach(obj);
	}

	void Controller_Gone(GameObject *obj)
	{
		if (isDeleted)
			return;

		isDeleted = true;

		StringClass buff;
		buff.Format("%s %s", botName.Peek_Buffer(), Get_Translated_String(Get_String_ID_By_Desc("IDS_MP_PLAYER_LEFT_GAME")));
		Send_Message(MS_RGB_JOINLEFT_MSG, buff.Peek_Buffer());

		if (strcmp(botName, DEFAULT_BOT_NAME) != 0)
			MS_AI_Global_Controller::GetInstance().Give_Back_Bot_Name(botName);

		// Let Global Controller know we are gone
		MS_AI_Global_Controller::GetInstance().botControllersTeam[team].DeleteObj(obj);
		MS_AI_Global_Controller::GetInstance().Unregister_From_GroupRush(team, obj->Get_ID());
		if (auto teamCtrlScript = Get_Team_Controller())
		{
			teamCtrlScript->Report_Bot_Died(obj);
		}

		auto soldier = soldierObject.Get_Ptr();
		if (!soldier)
			return;

		bool shouldDestroyVehicle = false;
		if (auto vehicle = vehicleObject.Get_Ptr())
		{
			if (vehicle->As_VehicleGameObj()->Get_Driver() == soldier)
			{
				// Only destroy the bot vehicle if this bot soldier was its only occupant
				if (vehicle->As_VehicleGameObj()->Get_Occupant_Count() == 1 && !vehicle->As_VehicleGameObj()->Is_Attached_To_An_Object())
					shouldDestroyVehicle = true;
				else
				{
					Commands->Enable_Vehicle_Transitions(vehicle, true);
					Remove_Script(vehicle, "MS_AI_Behaviour");
				}
			}
		}
		Commands->Destroy_Object(soldier);
		if (shouldDestroyVehicle)
		{
			if (auto vehicle = vehicleObject.Get_Ptr())
			{
				Commands->Enable_Vehicle_Transitions(vehicle, false);
				Commands->Attach_Script(vehicle, "JFW_Destroy_Self_Timer", "1.0,0");
			}
		}
		Commands->Destroy_Object(tempObjective);
	}

	void Spawn_Object(GameObject *obj)
	{
		// Spawn first as the particular team's spawn character
		GameObject *soldier = ObjectLibraryManager::Create_Object(teamSpawnPreset);

		/*
		Where to spawn:
		if USEPLAYERSPAWNERS is set, use that.
		else, check objects in a similar way how Get_Multiplayer_Spawn_Location does it
		if cannot teleport to any, use 0,0,0
		*/
		if (strcmp(spawnAtPresetName, "USEPLAYERSPAWNERS") == 0)
		{
			//M_Console_Input("msg USEPLAYERSPAWNERS true");
			auto soldierCasted = soldier->As_SoldierGameObj();
			Matrix3D matrix;
			Get_Multiplayer_Spawn_Location(team, soldierCasted, matrix);
			soldierCasted->Set_Transform(matrix);
		}
		else
		{
			//M_Console_Input("msg Original spawning.");

			// Collect all positions and try them all
			DynamicVectorClass<GameObject *> availableList;
			SimpleDynVecClass<GameObject *> fullList = *Get_All_Objects_By_Preset(2, spawnAtPresetName);

			// Check good positions
			for (int i = 0; i < fullList.Count(); i++)
			{
				Vector3 createPos = Commands->Get_Position(fullList[i]);
				MoveablePhysClass* phys = soldier->As_PhysicalGameObj() ? soldier->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : nullptr;
				if (phys && phys->Can_Teleport(Matrix3D(createPos)))
					availableList.Add(fullList[i]);
			}

			if (availableList.Count() > 0)
			{
				GameObject *chosenObj = availableList[FreeRandom.Get_Int(availableList.Count())];
				Commands->Set_Position(soldier, Commands->Get_Position(chosenObj));
				Commands->Set_Facing(soldier, Commands->Get_Facing(chosenObj));
			}
		}

		soldier->As_SoldierGameObj()->Set_Bot_Tag(botName.Peek_Buffer());
		soldier->As_SoldierGameObj()->Set_Player_Type(team);
		soldier->As_SoldierGameObj()->Start_Observers();
		Remove_Stock_AI_Behaviour(soldier);
		soldierObject = soldier;


		if (objectiveType != 3 && MS_AI_Global_Controller::GetInstance().spawnPositions[team].ID(Commands->Get_Position(soldier)) == -1)
			MS_AI_Global_Controller::GetInstance().spawnPositions[team].Add(Commands->Get_Position(soldier));

		// Spawn protection and waiting before purchasing items
		Commands->Attach_Script(soldier, "MS_Switch_Skin_And_Shield_Timer", "Blamo,Blamo,0");
		// Randomize waiting time a bit
		const float time = personality->Get_Random(BotPersonality::SPAWN_TIME);
		Commands->Start_Timer(obj, this, 0.1f, TIMER_AI_OBJECT_SPAWN_GHOSTING_COUNTDOWN);
		Commands->Start_Timer(obj, this, time, TIMER_AI_OBJECT_ACTIVATE_COUNTDOWN);

		expenses = 0;
	}

	void Purchase_Presets(GameObject *obj)
	{
		const int previousObjType = objectiveType;
		// Check what objective type is needed if the team controller can tell it
		if (auto teamCtrlScript = Get_Team_Controller())
		{
			objectiveType = teamCtrlScript->Signup_For_Next_Objective_Type(obj);
			soldierPresetList = teamCtrlScript->Get_Infantry_List();
		}

		// Non-offensive: clear earlier vehicle stuff
		if (objectiveType != dp88_AI_Objective::TYPE_OFFENSIVE)
			vehicleObject = nullptr;

		// TODO later: Current inf list contains current character or not?
		bool mustChangeCharacter = objectiveType != previousObjType; 

		int groupRushOpen = MS_AI_Global_Controller::GetInstance().Is_Rally_Point_Registration_Open(team);
		// Try group rush
		if (!coopState && groupRushOpen && objectiveType == dp88_AI_Objective::TYPE_OFFENSIVE)
		{
			MS_AI_Global_Controller::GetInstance().Register_For_GroupRush(team, obj->Get_ID());
			coopState = 1;
			Commands->Start_Timer(obj, this, 1.0f, TIMER_AI_COOP_RETRY_REQUEST);
		}

		const char* chosenPresetName;
		// Let's look at the available infantry first
		// Engineering AI always try to buy from their list!
		bool buyInf = mustChangeCharacter || objectiveType == dp88_AI_Objective::TYPE_ENGINEERING || personality->Get_Random(BotPersonality::BUYINF_CHANCE) || money > 10000;

		if (buyInf)
		{
			chosenPresetName = Choose_Unit(soldierPresetList, true);
			if (chosenPresetName)
			{
				Change_Character(soldierObject, chosenPresetName);
				if (MS_AI_Global_Controller::GetInstance().debugBuyMessagesEnabled)
				{
					int cost = Get_Team_Cost(chosenPresetName, team);
					Vector3 rgb = Get_Color_For_Team(team);
					StringClass msg;
					const char *unitName = Get_Translated_Definition_Name(chosenPresetName);
					msg.Format("%s: Purchasing: %s (%d)", botName.Peek_Buffer(), unitName, cost);
					Send_Message_Team(team, (int)(rgb.X * 255), (int)(rgb.Y * 255), (int)(rgb.Z * 255), msg.Peek_Buffer());
					delete[] unitName;
				}
			}
			else if (mustChangeCharacter)
			{
				// If buying failed, switch to spawn character
				Change_Character(soldierObject, teamSpawnPreset);
			}
			if (chosenPresetName || mustChangeCharacter)
				soldierObject->As_PhysicalGameObj()->Peek_Physical_Object()->Set_Collision_Group(Collision_Group_Type::SOLDIER_GHOST_COLLISION_GROUP);
		}

		// Attach script to soldier object so it can listen to the vehicle owned custom and everything
		StringClass params;
		params.Format("%d,%d", Commands->Get_ID(obj), 0);
		Commands->Attach_Script(soldierObject, "MS_AI_Player_Controller_Spawned_Object", params.Peek_Buffer());

		// If bot purchased a vehicle but was killed before reaching it, retry running to it
		if (vehicleObject)
		{
			if (GameObject* veh = vehicleObject)
			{
				// If vehicle is still alive and noone stole it
				VehicleGameObj* vObject = veh->As_VehicleGameObj();
				if (vObject && vObject->Get_Occupant_Count() < vObject->Get_Definition().Get_Seat_Count() && !Is_Script_Attached(vObject, "MS_AI_Player_Controller_Spawned_Object"))
				{
					//M_Console_Input("msg Retry running for vehicle...");
					Go_To_Vehicle(obj, vehicleObject);
					return;
				}
			}
			vehicleObject = nullptr;
		}

		if (!coopState && Search_For_Vehicle(obj, 100))
			return;

		// Try to buy a vehicle or not? Engineering AI must not buy
		// TODO: NO VEHICLE FOR DEFENDERS UNTIL THEY CAN SWITCH OBJECTIVE AND ACTUALLY GO AFTER ATTACKERS PROPERLY
		bool buyVeh = objectiveType == dp88_AI_Objective::TYPE_OFFENSIVE && (personality->Get_Random(BotPersonality::BUYVEH_CHANCE) || money > 10000);
		// No vehicle if chosen group rush doesn't allow it
		if (coopState == 1 && groupRushOpen == 1)
		{
			buyVeh = false;
		}

		if (buyVeh)
		{
			chosenPresetName = Choose_Unit(vehPresetList, false);
			if (chosenPresetName)
			{
				if (Create_Vehicle(chosenPresetName, 0.0, soldierObject, team/*, false*/))
				{
					if (MS_AI_Global_Controller::GetInstance().debugBuyMessagesEnabled)
					{
						int cost = Get_Team_Cost(chosenPresetName, team);
						Vector3 rgb = Get_Color_For_Team(team);
						StringClass msg;
						const char *unitName = Get_Translated_Definition_Name(chosenPresetName);
						msg.Format("%s: Purchasing: %s (%d)", botName.Peek_Buffer(), unitName, cost);
						Send_Message_Team(team, (int)(rgb.X * 255), (int)(rgb.Y * 255), (int)(rgb.Z * 255), msg.Peek_Buffer());
						delete[] unitName;
					}
					// Nothing else to do, created soldier is waiting for custom from created vehicle
					return;
				}
				else
				{
					// Refund, choose unit deducted cash but Create_Vehicle can be false!
					money += Get_Team_Cost(chosenPresetName, team);
				}
			}
		}

		// Activate AI on the new character (if no vehicles considered) or the spawn char (if no character was bought)
		if (soldierObject)
			Activate_AI_Behaviour(obj, soldierObject);
	}

	const char* Choose_Unit(const char *checkPresetList, bool isInfantry)
	{
		if (!checkPresetList)
			return nullptr;

		DynamicVectorClass<const char*> namedList;

		if (strcmp(checkPresetList, "AUTOENGINEER") == 0)
		{
			namedList = MS_AI_Global_Controller::GetInstance().engineerUnitList;
		}
		else
		{
			DynamicVectorClass<int> fullList;

			if (strcmp(checkPresetList, "USEPLAYERPURCHASELISTS") == 0)
			{
				if (isInfantry)
				{
					fullList = Combine_Lists(fullList, Get_Enlisted_Purchase_Items(team));
					fullList = Combine_Lists(fullList, Get_Purchase_Items(team, PurchaseSettingsDefClass::TYPE_CLASSES));
					if (Is_Extras_Enabled() || Is_Sidebar_Enabled())
						fullList = Combine_Lists(fullList, Get_Purchase_Items(team, PurchaseSettingsDefClass::TYPE_SECRET_CLASSES));
				}
				else
				{
					fullList = Combine_Lists(fullList, Get_Purchase_Items(team, PurchaseSettingsDefClass::TYPE_VEHICLES));
					if (Is_Extras_Enabled() || Is_Sidebar_Enabled())
						fullList = Combine_Lists(fullList, Get_Purchase_Items(team, PurchaseSettingsDefClass::TYPE_SECRET_VEHICLES));

					fullList = Combine_Lists(fullList, Get_Purchase_Items(team, PurchaseSettingsDefClass::TYPE_AIR));
					fullList = Combine_Lists(fullList, Get_Purchase_Items(team, PurchaseSettingsDefClass::TYPE_NAVAL));
				}
			}
			else
			{
				TwiddlerClass* twiddler = (TwiddlerClass*)DefinitionMgrClass::Find_Named_Definition(checkPresetList, false);
				if (twiddler)
					fullList = twiddler->Get_Definition_List();
			}
			for (int i = 0; i < fullList.Count(); i++)
			{
				if (auto* def = DefinitionMgrClass::Find_Definition(fullList[i], false))
				{
					namedList.Add(def->Get_Name());
				}
			}
		}

		DynamicVectorClass<const char*> finalList;

		for (int i = 0; i < namedList.Count(); i++)
		{
			const char* presetName = namedList[i];

			// Don't "rebuy" the spawning character
			if (isInfantry && strcmp(presetName, teamSpawnPreset) == 0)
				continue;

			bool available = isInfantry ? Is_Infantry_Purchaseable(presetName, team) : Is_Vehicle_Purchaseable(presetName, team);

			if (Get_Team_Cost(presetName, team) <= money && available)
				finalList.Add(presetName);
		}

		// Get one out of the list if possible
		if (finalList.Count() > 0)
		{
			const char* chosenPresetName = finalList[FreeRandom.Get_Int(finalList.Count())];
			int cost = Get_Team_Cost(chosenPresetName, team);
			money -= cost;

			// Needed if bot kills itself due to being stuck
			if (isInfantry)
				expenses += cost;

			return chosenPresetName;
		}
		return nullptr;
	}

	void Activate_AI_Behaviour(GameObject *obj, GameObject *activateOnObj)
	{
		// Safety check
		if (!activateOnObj)
		{
			StringClass msg;
			msg.Format("[%d:%hs:%hs] Critical Error: Trying to activate AI on a nullpointer object (%hs). Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Commands->Get_Preset_Name(activateOnObj));
			M_Console_Output(msg.Peek_Buffer());
			Commands->Destroy_Object(activateOnObj);
			Destroy_Script();
			return;
		}

		int isVehicle = activateOnObj->As_VehicleGameObj() ? 1 : 0;

		// Attach script to spawned object so the controller and child can communicate
		// Make sure script is only attached once
		if (!Is_Script_Attached(activateOnObj, "MS_AI_Player_Controller_Spawned_Object"))
		{
			StringClass params;
			params.Format("%d,%d", Commands->Get_ID(obj), isVehicle);
			Commands->Attach_Script(activateOnObj, "MS_AI_Player_Controller_Spawned_Object", params.Peek_Buffer());
		}

		// Get AI Preferences
		StringClass behaviourScriptParams;
		auto preferencesScript = (MS_AI_Preferences *)(Find_Script_On_Object(activateOnObj, "MS_AI_Preferences"));
		if (!preferencesScript)
		{
			int maxAttackRange = 0;
			int preferredAttackRange = 0;
			if (activateOnObj->As_PhysicalGameObj() && activateOnObj->As_PhysicalGameObj()->As_ArmedGameObj())
			{
				auto armedObj = activateOnObj->As_PhysicalGameObj()->As_ArmedGameObj();
				if (auto weapon = armedObj->Get_Weapon())
				{
					maxAttackRange = (int)(weapon->PrimaryAmmoDefinition ? weapon->PrimaryAmmoDefinition->Range() : maxAttackRange);
					preferredAttackRange = (int)(weapon->PrimaryAmmoDefinition ? weapon->PrimaryAmmoDefinition->EffectiveRange(): preferredAttackRange);
					// Make sure it's not zero or bigger than max 
					if (preferredAttackRange == 0 || preferredAttackRange > maxAttackRange)
						preferredAttackRange = maxAttackRange;
					// Long range units: snipers/artilleries, should not try to come as close
					if (maxAttackRange >= 120)
						preferredAttackRange = maxAttackRange - 10;
				}
			}

			// Get default parameters
			DynamicVectorClass<ScriptParameter> parameters;
			Get_Script_Parameters("MS_AI_Preferences", parameters);
			for (int i = 0; i < parameters.Count(); i++)
			{
				auto parameter = parameters[i];
				// Priority_Infantry is the first param
				if (i == 0)
				{
					// Use negative value for engineering units, they should never actively engage enemies, but run to repair instead
					if (objectiveType == dp88_AI_Objective::TYPE_ENGINEERING)
						behaviourScriptParams.Format("%.2f", -0.10f);
					else
						behaviourScriptParams.Format("%s", parameter.value.Peek_Buffer());
				}
				// Use the equipped weapon's ranges
				else if (strcmp(parameter.name, "Max_Attack_Range") == 0)
				{
					if (maxAttackRange)
						behaviourScriptParams.Format("%s,%d", behaviourScriptParams.Peek_Buffer(), maxAttackRange);
					else
						behaviourScriptParams.Format("%s,%s", behaviourScriptParams.Peek_Buffer(), parameter.value.Peek_Buffer());
				}
				else if (strcmp(parameter.name, "Preferred_Attack_Range") == 0)
				{
					if (preferredAttackRange)
						behaviourScriptParams.Format("%s,%d", behaviourScriptParams.Peek_Buffer(), preferredAttackRange);
					else
						behaviourScriptParams.Format("%s,%s", behaviourScriptParams.Peek_Buffer(), parameter.value.Peek_Buffer());
				}
				// Use 0 values for engineering units, they should never actively engage enemies, but run to repair instead
				else if ((strcmp(parameter.name, "Priority_Light_Vehicle") == 0 || strcmp(parameter.name, "Priority_Heavy_Vehicle") == 0 || strcmp(parameter.name, "Priority_VTOL") == 0) && objectiveType == dp88_AI_Objective::TYPE_ENGINEERING)
				{
					behaviourScriptParams.Format("%s,%d", behaviourScriptParams.Peek_Buffer(), 0);
				}
				else if (strcmp(parameter.name, "Priority_Building") == 0 && activateOnObj->As_SoldierGameObj())
				{
					behaviourScriptParams.Format("%s,%.2f", behaviourScriptParams.Peek_Buffer(), -0.01f);
				}
				else
					behaviourScriptParams.Format("%s,%s", behaviourScriptParams.Peek_Buffer(), parameter.value.Peek_Buffer());
			}
		}
		else
		{
			char str[256];
			preferencesScript->Get_Parameters_String(str, 256);
			behaviourScriptParams.Format("%s", str);
		}

		behaviourScriptParams.Format("%s,%d", behaviourScriptParams.Peek_Buffer(), objectiveType);
		Commands->Attach_Script(activateOnObj, "MS_AI_Behaviour", behaviourScriptParams.Peek_Buffer());

		auto behaviourScript = (MS_AI_Behaviour *)(Find_Script_On_Object(activateOnObj, "MS_AI_Behaviour"));
		if (behaviourScript)
			behaviourScript->Set_Personality(personality);
	}

	bool Search_For_Vehicle(GameObject *obj, float distanceLimit)
	{
		// Soldier not found or soldier is in a vehicle: don't search
		if (!soldierObject || (vehicleObject && enteredVeh))
			return false;

		// Only offensive AI
		if (objectiveType == 1)
		{
			// Search for transports (units with more available seats than occupants) and go for one
			SList<GameObject> list;
			Find_All_Vehicles_By_Distance(list, Commands->Get_Position(soldierObject));
			
			for (auto x = list.Head(); x != nullptr; x = x->Next())
			{
				GameObject* object = x->Data();
				// Check for being a vehicle
				if (!object->As_VehicleGameObj())
					continue;

				VehicleGameObj *vehicle = object->As_VehicleGameObj();
				// Skip vehicles that have no driver but have owner (e.g. freshly bought vehicles)
				if ((vehicle->Get_Occupant_Count() == 0 && vehicle->Is_Locked()) ||
					// Skip if this is the banned vehicle
					vehicle == bannedVehicle ||
					// Skip enemy vehicles with players inside
					(vehicle->Get_Occupant_Count() > 0 && Get_Object_Type(object) != team) ||
					// Attached turret case (e.g. Battle Fortress): parent must be friendly
					//(vehicle->Get_Host_Object() && Get_Object_Type(vehicle->Get_Host_Object()) != team) ||
					// 33% chance of ignoring ownerless vehicles with max 1 seat
					//(vehicle->Get_Definition().Get_Seat_Count() <= 1 && !vehicle->Is_Locked() && FreeRandom.Get_Float() < 0.33f) ||
					!vehicle->Get_Definition().Get_Seat_Count() ||
					!vehicle->Get_Transitions_Enabled() ||
					// Don't enter turret type vehicles
					vehicle->Is_Turret() ||
					// Skip if full
					vehicle->Get_Occupant_Count() >= vehicle->Get_Definition().Get_Seat_Count() ||
					// Skip if occupied and isn't a transport vehicle
					(vehicle->Get_Occupant_Count() >= 1 && vehicle->Get_Definition().Get_Seat_Count() < MS_AI_Global_Controller::GetInstance().transportVehicleSeatCount) && !Is_Script_Attached(vehicle, "MS_AI_Marker_Transport_Vehicle_Override"))
					continue;

				// If too far away, don't bother
				if (Vector3::Distance_Squared(Commands->Get_Position(soldierObject), Commands->Get_Position(vehicle)) > distanceLimit * distanceLimit)
					continue;

				// Skip vehicles marked to be ignored
				if (auto ignoreScript = (MS_AI_Marker_Vehicle_Search_Ignore*)(Find_Script_On_Object(vehicle, "MS_AI_Marker_Vehicle_Search_Ignore")))
				{
					int ignorerTeam = ignoreScript->Get_Ignorer_Team();
					if (ignorerTeam == 2 || ignorerTeam == team)
						continue;
				}

				// 33% chance of ignoring anyway
				/*if (FreeRandom.Get_Float() < 0.33f)
				continue;*/

				if (MS_AI_Global_Controller::GetInstance().debugAcquireVehicleMessagesEnabled)
				{
					Vector3 rgb = Get_Color_For_Team(team);
					StringClass msg;
					const char *unitName = Get_Translated_Definition_Name(Commands->Get_Preset_Name(vehicle));
					msg.Format("%s: Running for %s's vehicle (%s)!", botName.Peek_Buffer(), Get_Player_Name(vehicle->Get_Driver()), unitName);
					Send_Message_Team(team, (int)(rgb.X * 255), (int)(rgb.Y * 255), (int)(rgb.Z * 255), msg.Peek_Buffer());
					if (strcmp(unitName, "ERROR: String Not Found") == 0)
					{
						msg.Format("%s: Selected vehicle (%s) is probably wrong, has no string!", botName.Peek_Buffer(), Commands->Get_Preset_Name(vehicle));
						Send_Message_Team(team, (int)(rgb.X * 255), (int)(rgb.Y * 255), (int)(rgb.Z * 255), msg.Peek_Buffer());
					}
					delete[] unitName;
				}

				if (Go_To_Vehicle(obj, vehicle))
				{
					return true;
				}
			}
		}
		return false;
	}

	bool Go_To_Vehicle(GameObject *controllerObj, GameObject* vehicle)
	{
		// Soldier is alive and vehicle exists
		if (soldierObject && vehicle)
		{
			if (!(Is_Script_Attached(vehicle, "dp88_AI_Objective")))
			{
				StringClass params;
				params.Format("%d,%d,%d,%d,%d,%d,%d", 2, vehicle->Get_ID(), 1, 1000, 1000, 1000, 1000);
				Commands->Attach_Script(vehicle, "dp88_AI_Objective", params.Peek_Buffer());
			}
			if (!(Is_Script_Attached(soldierObject, "MS_AI_Behaviour")))
			{
				Activate_AI_Behaviour(controllerObj, soldierObject);
			}
			else if (!Is_Script_Attached(soldierObject, "MS_AI_Player_Controller_Spawned_Object"))
			{
				StringClass params;
				params.Format("%d,%d", Commands->Get_ID(controllerObj), false);
				Commands->Attach_Script(soldierObject, "MS_AI_Player_Controller_Spawned_Object", params.Peek_Buffer());
			}
			vehicleObject = vehicle;
			Commands->Send_Custom_Event(controllerObj, soldierObject, CUSTOM_AI_GO_TO_VEHICLE, vehicleObject->Get_ID(), 1.5f);
			return true;
		}
		return false;
	}

	void Play_Radio_Message(int stringID)
	{
		// Send_Client_Announcement is not working properly for non-players and cannot be modified in 4.x, reimplementing here.
		//Send_Client_Announcement(team, soldierObject->Get_ID(), Get_Radio_Command_String(stringID), ANNOUNCE_TEAM, stringID, true, true);

		auto obj = soldierObject.Get_Ptr();

		if (!obj)
			return;

		if (auto soldier = obj->As_SoldierGameObj())
			Set_Emot_Icon(soldier, stringID);

		if (TDBObjClass *tdbobj = TranslateDBClass::Find_Object(Get_Radio_Command_String(stringID)))
		{
			const char *unit = Get_Vehicle(obj) ? Get_Translated_Definition_Name(Commands->Get_Preset_Name(Get_Vehicle(obj))) : Get_Translated_Definition_Name(Commands->Get_Preset_Name(obj));
			Vector3 rgb = Get_Color_For_Team(team);
			StringClass msg;
			msg.Format("%s: [%s] %S", botName.Peek_Buffer(), unit, tdbobj->Get_String());
			Send_Message_Team(team, (int)(rgb.X * 255), (int)(rgb.Y * 255), (int)(rgb.Z * 255), msg.Peek_Buffer());
			delete[] unit;

			int soundID = tdbobj->Get_Sound_ID();
			if (auto soundDef = Find_Definition(soundID))
			{
				Create_2D_Sound_Team(soundDef->Get_Name(), team);
			}
		}
	}

	void Send_Private_Message_String(char *msg, GameObject *recipient)
	{
		if (!MS_AI_Global_Controller::GetInstance().chattingEnabled) {
			return;
		}
		
		unsigned int red, green, blue;
		Get_Private_Message_Color(&red, &green, &blue);

		StringClass message;
		message.Format("%s: %s", botName.Peek_Buffer(), msg);
		Send_Message_Player(recipient, red, green, blue, message.Peek_Buffer());
		Create_2D_Sound_Player(recipient, "Private_Message");
	}

	void Send_Public_Message_String(char *msg)
	{
		unsigned int red, green, blue;
		Get_Public_Message_Color(&red, &green, &blue);

		StringClass message;
		message.Format("%s: %s", botName.Peek_Buffer(), msg);
		Send_Message(red, green, blue, message.Peek_Buffer());
		Commands->Create_2D_Sound("Public_Message");
	}

	MS_AI_Team_Controller *Get_Team_Controller()
	{
		if (auto teamCtrlObj = MS_AI_Global_Controller::GetInstance().teamControllers[team])
		{
			if (auto teamCtrlScript = (MS_AI_Team_Controller *)(Find_Script_On_Object(teamCtrlObj, "MS_AI_Team_Controller")))
			{
				return teamCtrlScript;
			}
		}
		return nullptr;
	}
	

public:
	void Increment_Money(float amount)
	{
		money += amount;
	}

	float Get_Money()
	{
		return money;
	}

	int Get_Bot_ID()
	{
		return soldierObject ? soldierObject->Get_ID() : 0;
	}

	const StringClass& Get_Bot_Name()
	{
		return botName;
	}

	int Get_Coop_State()
	{
		return coopState;
	}

	void Receive_Radio_Message(GameObject *sender, int messageNumber, int orderType = 0, int orderObjID = 0)
	{
		if (isProcessingRadioMessage)
			return;

		isProcessingRadioMessage = true;

		// Save variables, will use them again when timer ran out
		radioMessageID = messageNumber;
		taskType = orderType;
		taskObjID = orderObjID;
		const float delay = personality->Get_Random(BotPersonality::RADIO_TIME);
		Commands->Send_Custom_Event(sender, Owner(), CUSTOM_AI_COOP_PLAYER_RADIO_MESSAGE, 0, delay);
	}

	void React_To_Radio_Message(GameObject *sender, int messageNumber, int orderType = 0, int orderObjID = 0)
	{
		GameObject *obj = Owner();
		GameObject *soldier = soldierObject;
		// Soldier dead or group rushing: no further checks
		if (!soldier || coopState == 1 || !sender || !Commands->Get_Preset_Name(sender))
			return;

		float sympathy = 0;
		auto playerName = Get_Player_Name(sender);
		if (auto sympathyData = playerRelationData.GetPlayerData(playerName))
			sympathy = sympathyData->sympathy;

		if (orderType != 0)
		{
			coopTarget = Commands->Find_Object(orderObjID);
			coopFriendPlayerID = Get_Player_ID(sender);
		}

		float distanceFromSender = Vector3::Distance(Commands->Get_Position(soldier), Commands->Get_Position(sender));


		if (messageNumber == Radio_Message_Type::BUILDING_NEEDS_REPAIRS)
		{
			if (objectiveType == dp88_AI_Objective::TYPE_ENGINEERING)
				Play_Radio_Message(Radio_Message_Type::AFFIRMATIVE);
			else if (personality->Get_Random(BotPersonality::CHAT_CHANCE) && FreeRandom.Get_Float() < 0.1f)
				Play_Radio_Message(Radio_Message_Type::NEGATIVE);
		}

		// not offensive: no further checks
		if (objectiveType != dp88_AI_Objective::TYPE_OFFENSIVE)
			return;

		if (messageNumber == Radio_Message_Type::GET_IN_THE_VEHICLE)
		{
			GameObject *senderVehicle = Get_Vehicle(sender);

			bool moreSoldiersCanFit = false;
			if (senderVehicle)
			{
				auto vehicleObj = senderVehicle->As_VehicleGameObj();
				moreSoldiersCanFit = (vehicleObj->Get_Occupant_Count() < vehicleObj->Get_Definition().Get_Seat_Count());
			}

			// No assigned vehicle yet and there are empty seats in sender's vehicle
			if (!vehicleObject && moreSoldiersCanFit && distanceFromSender < 30)
			{
				Go_To_Vehicle(obj, senderVehicle);
				Play_Radio_Message(Radio_Message_Type::AFFIRMATIVE);
			}
		}
		else if (messageNumber == Radio_Message_Type::GET_OUT_OF_THE_VEHICLE)
		{
			GameObject *vehicleObj = Get_Vehicle(soldier);

			// Is in vehicle driven by message sender
			if (vehicleObject && vehicleObj && vehicleObj->As_VehicleGameObj()->Get_Driver() == sender)
			{
				int seatNum = vehicleObj->As_VehicleGameObj()->Find_Seat(soldier->As_SoldierGameObj());
				Force_Occupant_Exit(vehicleObj, seatNum);
				Play_Radio_Message(Radio_Message_Type::AFFIRMATIVE);
			}
		}
		else if (messageNumber == Radio_Message_Type::DESTROY_THAT_VEHICLE ||
			messageNumber == Radio_Message_Type::DESTROY_IT_NOW ||
			messageNumber == Radio_Message_Type::ATTACK_THE_HARVESTER ||
			messageNumber == Radio_Message_Type::ATTACK_THAT_STRUCTURE)
		{
			if (sender == Get_GameObj(coopFriendPlayerID) && orderType == CUSTOM_AI_ORDER_ATTACK)
			{
				coopState = 6;
				Commands->Start_Timer(obj, this, 0.0f, TIMER_AI_COOP_RETRY_REQUEST);
				Play_Radio_Message(Radio_Message_Type::DESTROY_IT_NOW);
			}
			// Try to find the enemy harvester
			else if (messageNumber == Radio_Message_Type::ATTACK_THE_HARVESTER)
			{
				if (auto harvester = Find_Harvester(team == 1 ? 0 : 1))
				{
					// Bots will agree easier if the harvester is close
					float distFromTarget = Vector3::Distance(Commands->Get_Position(soldier), Commands->Get_Position(harvester));
					if (distFromTarget < 40 || (distFromTarget < 120 && personality->Get_Random(BotPersonality::TEAMWORK_CHANCE)) || FreeRandom.Get_Float() < sympathy)
					{ 
						coopTarget = harvester;
						coopState = 6;
						Commands->Start_Timer(obj, this, 0.0f, TIMER_AI_COOP_RETRY_REQUEST);
						Play_Radio_Message(Radio_Message_Type::AFFIRMATIVE);
					}
				}
			}
		}
		else if (messageNumber == Radio_Message_Type::TAKE_THE_POINT)
		{
			if (sender == Get_GameObj(coopFriendPlayerID))
			{
				// Moving order instead of releasing
				if (orderType == CUSTOM_AI_ORDER_MOVE)
				{
					coopState = 5;
					Commands->Start_Timer(obj, this, 0.0f, TIMER_AI_COOP_RETRY_REQUEST);
					Play_Radio_Message(Radio_Message_Type::COVER_ME);
				}
			}
		}
		else if (messageNumber == Radio_Message_Type::DONT_GET_IN_MY_WAY)
		{
			if (sender == Get_GameObj(coopFriendPlayerID))
			{
				// Soldier sends the message back to this controller, so no need to also send this custom to self
				Commands->Send_Custom_Event(obj, coopObject, CUSTOM_AI_COOP_PLAYER_FOLLOW_STOP, 0, 0.0f);
				Play_Radio_Message(Radio_Message_Type::AFFIRMATIVE);
			}
		}
		else if (messageNumber == Radio_Message_Type::FOLLOW_ME)
		{
			bool coveringSamePlayer = (coopState == 3 && sender == Get_GameObj(coopFriendPlayerID));

			// Join if not following anyone yet or already going with player but not with follow command
			if ((coopState == 0 && distanceFromSender < 80 && personality->Get_Random(BotPersonality::TEAMWORK_CHANCE)) || coveringSamePlayer)
			{
				if (long id = Get_Player_ID(sender))
					coopFriendPlayerID = id;

				coopState = 2;
				Commands->Start_Timer(obj, this, 0.0f, TIMER_AI_COOP_RETRY_REQUEST);
				Play_Radio_Message(Radio_Message_Type::AFFIRMATIVE);
			}
		}
		else if (messageNumber == Radio_Message_Type::COVER_ME)
		{
			bool followingSamePlayer = (coopState == 2 && sender == Get_GameObj(coopFriendPlayerID));

			if ((coopState == 0 && distanceFromSender < 80 && personality->Get_Random(BotPersonality::TEAMWORK_CHANCE)) || followingSamePlayer)
			{
				if (long id = Get_Player_ID(sender))
					coopFriendPlayerID = id;

				coopState = 3;
				Commands->Start_Timer(obj, this, 0.0f, TIMER_AI_COOP_RETRY_REQUEST);
				Play_Radio_Message(Radio_Message_Type::AFFIRMATIVE);
			}
		}
		else if (messageNumber == Radio_Message_Type::DEFEND_THE_BASE)
		{
			int objectiveObjID = MS_AI_Global_Controller::GetInstance().Get_Temporary_Objective_ID(team);
			float distanceFromBase = Vector3::Distance(Commands->Get_Position(soldier), Commands->Get_Position(Commands->Find_Object(objectiveObjID)));

			if (coopState != 4 && distanceFromBase < 200 && personality->Get_Random(BotPersonality::TEAMWORK_CHANCE))
			{
				coopState = 4;
				Commands->Start_Timer(obj, this, 0.0f, TIMER_AI_COOP_RETRY_REQUEST);
				Play_Radio_Message(Radio_Message_Type::AFFIRMATIVE);
			}
		}
		else if (messageNumber == Radio_Message_Type::DEFEND_THAT_STRUCTURE ||
			messageNumber == Radio_Message_Type::DEFEND_THE_HARVESTER)
		{
			if (orderType == CUSTOM_AI_ORDER_DEFEND)
			{
				coopState = 7;
				Commands->Start_Timer(obj, this, 0.0f, TIMER_AI_COOP_RETRY_REQUEST);
				Play_Radio_Message(Radio_Message_Type::AFFIRMATIVE);
			}
		}
	}

private:
	bool Init_Parameters(GameObject *obj)
	{
		bool valid = true;
		StringClass errorMessage;

		team = Get_Int_Parameter("Team");
		if (team != 0 && team != 1)
		{
			errorMessage.Format("The script has an invalid Team parameter: %d, this must be 0 or 1", team);
			valid = false;
		}
		spawnAtPresetName = Get_Parameter("SpawnAtPreset");
		if (valid && strcmp(spawnAtPresetName, "USEPLAYERSPAWNERS") != 0 && !Is_Valid_Preset(spawnAtPresetName))
		{
			errorMessage.Format("The script has an invalid SpawnAtPreset parameter: %s", spawnAtPresetName);
			valid = false;
		}

		soldierPresetList = Get_Parameter("SoldierTwiddlerPreset");
		if (strcmp(soldierPresetList, "") == 0 || strcmp(soldierPresetList, "null") == 0)
			soldierPresetList = nullptr;
		else if (valid && strcmp(soldierPresetList, "USEPLAYERPURCHASELISTS") != 0 && !Is_Valid_Preset(soldierPresetList))
		{
			errorMessage.Format("The script has an invalid SoldierTwiddlerPreset parameter: %s", soldierPresetList);
			valid = false;
		}

		vehPresetList = Get_Parameter("VehicleTwiddlerPreset");
		if (strcmp(vehPresetList, "") == 0 || strcmp(vehPresetList, "null") == 0)
			vehPresetList = nullptr;
		else if (valid && strcmp(vehPresetList, "USEPLAYERPURCHASELISTS") != 0 && !Is_Valid_Preset(vehPresetList))
		{
			errorMessage.Format("The script has an invalid VehicleTwiddlerPreset parameter: %s", vehPresetList);
			valid = false;
		}

		objectiveType = Get_Int_Parameter("ObjectiveType");
		if (valid && (objectiveType < 1 || objectiveType > 3))
		{
			errorMessage.Format("The script has an invalid ObjectiveType parameter: %d, this must be 1, 2 or 3", objectiveType);
			valid = false;
		}

		if (!valid)
		{
			StringClass msg;
			msg.Format("[%d:%hs:%hs] Critical Error: %hs. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), errorMessage.Peek_Buffer());
			M_Console_Output(msg.Peek_Buffer());
			Destroy_Script();
		}
		return valid;
	}

};
REGISTER_SCRIPT(MS_AI_Player_Controller,
	"Team=0:int,"
	"SpawnAtPreset=USEPLAYERSPAWNERS:string,"
	"SoldierTwiddlerPreset=USEPLAYERPURCHASELISTS:string,"
	"VehicleTwiddlerPreset=USEPLAYERPURCHASELISTS:string,"
	"ObjectiveType=1:int"
);


/*!
* \brief AI player controller's spawned object
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* Subject to change. DO NOT PLACE THIS MANUALLY!
*
* \param Nothing
*   Nothing
*/
class MS_AI_Player_Controller_Spawned_Object : public ScriptImpClass
{
	int controllerID;
	bool isAlive;

	// If this is a vehicle, it has no controller object
	bool isVehicle;
	DynamicVectorClass<int> passengerControllerIDs;
	int lastEnteredPassengerControllerID;
	StringClass botName;

	void Created(GameObject* obj) override
	{
		controllerID = 0;
		isAlive = true;
		isVehicle = false;
		lastEnteredPassengerControllerID = 0;
		botName = DEFAULT_BOT_NAME;

		controllerID = Get_Int_Parameter("ControllerId");
		isVehicle = Get_Int_Parameter("IsVehicle") > 0 ? true : false;

		if (auto ctrl = Get_Controller())
		{
			auto ctrlScript = (MS_AI_Player_Controller *)(Find_Script_On_Object(ctrl, "MS_AI_Player_Controller"));
			if (ctrlScript)
				botName = ctrlScript->Get_Bot_Name();
		}

		// Safety check for controller object existence
		if (!Commands->Find_Object(controllerID) && !isVehicle)
		{
			StringClass msg;
			msg.Format("[%d:%hs:%hs] Critical Error: The script's controller object can't be found. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name());
			M_Console_Output(msg.Peek_Buffer());
			Destroy_Script();
			return;
		}
		// Safety check for being a vehicle for real when it should be
		if (isVehicle && !obj->As_VehicleGameObj())
		{
			StringClass msg;
			msg.Format("[%d:%hs:%hs] Critical Error: The script's not on a vehicle when it should be, IsVehicle:%d  Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), isVehicle);
			M_Console_Output(msg.Peek_Buffer());
			Destroy_Script();
			return;
		}

		// Init passenger "Array"
		if (isVehicle)
		{
			auto vObj = obj->As_VehicleGameObj();
			for (int i = 0; i < vObj->Get_Definition().Get_Seat_Count(); i++)
			{
				// Check for bots already sitting at seats
				if (auto passenger = vObj->Get_Occupant(i))
				{
					if (auto spawnedScript = (MS_AI_Player_Controller_Spawned_Object *)(Find_Script_On_Object(passenger, "MS_AI_Player_Controller_Spawned_Object")))
					{
						if (auto botCtrl = spawnedScript->Get_Controller())
						{
							passengerControllerIDs.Add(botCtrl->Get_ID());
							continue;
						}
					}
				}
				// Otherwise init it as empty
				passengerControllerIDs.Add(0);
			}
		}
		else
		{
			Commands->Start_Timer(obj, this, 20.0f, TIMER_AI_SEARCH_FOR_VEHICLE);
		}

		Commands->Start_Timer(obj, this, 2.0f, TIMER_AI_THINK);
	}

	void Custom(GameObject *obj, int type, int param, GameObject *sender) override
	{
		if (type == CUSTOM_EVENT_OWNED_VEHICLE)
		{
			Commands->Send_Custom_Event(sender, Get_Controller(), CUSTOM_EVENT_OWNED_VEHICLE, param, 0.0f);
		}
		else if (type == CUSTOM_AI_GO_TO_VEHICLE)
		{
			// Timeout, vehicle could have been taken far away, bot should not try to run to it forever
			Commands->Start_Timer(obj, this, 40.0f, TIMER_AI_GO_TO_VEHICLE_TIMEOUT);
		}
		else if (type == CUSTOM_AI_ARRIVED_TO_VEHICLE)
		{
			Commands->Send_Custom_Event(obj, Get_Controller(), CUSTOM_AI_ARRIVED_TO_VEHICLE, param, 0.0f);
		}
		else if (type == CUSTOM_AI_VEHICLE_REQUEST_ENTER)
		{
			SoldierGameObj *soldier = Commands->Find_Object(param)->As_SoldierGameObj();
			// Check entry permitted before placing soldier in
			if (isVehicle)
			{
				lastEnteredPassengerControllerID = sender->Get_ID();
				Force_Vehicle_Entry(soldier, obj);

				// Cancel any existing timeout since we successfully entered the vehicle
				if (M_Has_Timer(obj, this, TIMER_AI_GO_TO_VEHICLE_TIMEOUT))
					M_Stop_Timer2(obj, this, TIMER_AI_GO_TO_VEHICLE_TIMEOUT);

				// Can transport more infantry? Enable transitions!
				if (obj->As_VehicleGameObj()->Get_Occupant_Count() < obj->As_VehicleGameObj()->Get_Definition().Get_Seat_Count())
					Commands->Enable_Vehicle_Transitions(obj, true);
			}
			// Otherwise param object would be trying to enter a soldier
		}
		else if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
		{
			if (lastEnteredPassengerControllerID != 0)
			{
				passengerControllerIDs[param] = lastEnteredPassengerControllerID;
				lastEnteredPassengerControllerID = 0;
			}
			// Players entering don't send controller id :)
		}
		else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
		{
			Commands->Send_Custom_Event(obj, Commands->Find_Object(passengerControllerIDs[param]), CUSTOM_AI_VEHICLE_EXITED, 1, 0.1f);
			passengerControllerIDs[param] = 0;
		}
		else if (type == CUSTOM_AI_REQUEST_REFUND)
		{
			Commands->Send_Custom_Event(obj, Get_Controller(), type, 0, 0.0f);
		}
		else if (type == CUSTOM_AI_COOP_ARRIVED_TO_RALLYPOINT)
		{
			Commands->Send_Custom_Event(sender, Get_Controller(), type, 0, 0.0f);
		}
		else if (type == CUSTOM_AI_TRANSPORT_ARRIVED)
		{
			int coopState = 0;
			if (auto ctrl = Get_Controller())
			{
				auto ctrlScript = (MS_AI_Player_Controller *)(Find_Script_On_Object(ctrl, "MS_AI_Player_Controller"));
				if (ctrlScript)
					coopState = ctrlScript->Get_Coop_State();
			}

			if (!isVehicle || coopState == 1)
				return;

			VehicleGameObj *vObj = obj->As_VehicleGameObj();
			// Get passengers out
			for (int i = 0; i < vObj->Get_Occupant_Count(); i++)
			{
				SoldierGameObj *passenger = vObj->Get_Occupant(i);
				// Skip driver
				if (passenger && i != 0)
				{
					Force_Occupant_Exit(vObj, i);
				}
			}
		}
		else if (type == CUSTOM_AI_COOP_PLAYER_FOLLOW_STOP)
		{
			Commands->Send_Custom_Event(sender, Get_Controller(), type, 0, 0.0f);
		}
		else if (type == CUSTOM_AI_VEHICLE_AMMO_EMPTY)
		{
			Commands->Send_Custom_Event(sender, Get_Controller(), type, param, 0.0f);
		}
		else if (type == CUSTOM_AI_REFILL_COMPLETE || type == CUSTOM_AI_CHECK_REPAIR_NEEDED || type == CUSTOM_AI_ARRIVED_TO_REFILL ||
				 type == CUSTOM_AI_ARRIVED_TO_EXIT_VEHICLE || type == CUSTOM_AI_REQUEST_PATROL_POINT || type == CUSTOM_AI_CHECK_GO_HOME ||
				 type == CUSTOM_AI_COOP_STOP_SECONDARY_OBJECTIVE)
		{
			Commands->Send_Custom_Event(sender, Get_Controller(), type, param, 0.0f);
		}
		else if (type == CUSTOM_AI_ENEMY_KILLED)
		{
			Commands->Send_Custom_Event(obj, Get_Controller(), CUSTOM_AI_SEND_PUBLIC_MESSAGE, 1, (float)FreeRandom.Get_Int(1, 4));
		}
		else if (type == CUSTOM_AI_DEACTIVATE_AI)
		{
			Remove_Script(obj, "MS_AI_Behaviour");
			Destroy_Script();
		}
	}

	void Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason) override
	{
	}

	void Timer_Expired(GameObject* obj, int number) override
	{
		if (number == TIMER_AI_SEARCH_FOR_VEHICLE)
		{
			if (isAlive && !Get_Vehicle(obj))
			{
				Commands->Send_Custom_Event(obj, Get_Controller(), CUSTOM_AI_SEARCH_FOR_VEHICLE, 0, 0.0f);
			}
			Commands->Start_Timer(obj, this, 20.0f, TIMER_AI_SEARCH_FOR_VEHICLE);
		}
		else if (number == TIMER_AI_GO_TO_VEHICLE_TIMEOUT)
		{
			if (!Get_Vehicle(obj))
				Commands->Send_Custom_Event(obj, Get_Controller(), CUSTOM_AI_ARRIVED_TO_VEHICLE, 0, 0.0f);
		}
		else if (number == TIMER_AI_THINK)
		{
			Commands->Send_Custom_Event(obj, Get_Controller(), CUSTOM_AI_CHECK_RADAR_TARGETS, 0, 0.0f);
			Commands->Start_Timer(obj, this, 2.0f, TIMER_AI_THINK);
		}
	}

	void Killed(GameObject *obj, GameObject *killer) override
	{
		// This fixes problems with healthlinker scripts killing the object, then healing then killing again (kill 2x)
		if (!isAlive)
			return;

		isAlive = false;
		// Vehicle died
		if (isVehicle)
		{
			// Send dead custom to all soldiers' controllers
			for (int i = 0; i < passengerControllerIDs.Count(); i++)
			{
				if (passengerControllerIDs[i] != 0)
					Commands->Send_Custom_Event(obj, Commands->Find_Object(passengerControllerIDs[i]), CUSTOM_AI_VEHICLE_DIED, 0, 0.1f);
			}
		}
		else
		{
			Commands->Send_Custom_Event(obj, Get_Controller(), CUSTOM_AI_SOLDIER_DIED, 0, 0.1f);
			if (killer && killer->As_SmartGameObj() && killer->As_SmartGameObj()->Is_Human_Controlled())
			{
				Create_2D_Sound_Player(killer, "My_Kill");
				// Chat message from bot
				// TODO: replace random with time to type it in, depending on chosen message character count
				Commands->Send_Custom_Event(obj, Get_Controller(), CUSTOM_AI_SEND_PUBLIC_MESSAGE, 0, (float)FreeRandom.Get_Int(1, 4));

				if (Kill_Messages_Disabled())
					return;

				//Kill messages are handled by DA.
				/*int killStringID = Get_Object_Type(obj) == Get_Object_Type(killer) ? Get_String_ID_By_Desc("IDS_MP_TREASON_PHRASE") : Get_String_ID_By_Desc("IDS_MP_DEFAULT_KILL_PHRASE");

				Vector3 rgb = Get_Color_For_Team(Get_Object_Type(killer));
				StringClass msg;
				msg.Format("%s %s %s", Get_Player_Name(killer), Get_Translated_String(killStringID), botName.Peek_Buffer());
				Send_Message((int)(rgb.X * 255), (int)(rgb.Y * 255), (int)(rgb.Z * 255), msg.Peek_Buffer());*/
			}
		}
	}

	void Destroyed(GameObject *obj) override
	{
		if (!isAlive)
			return;

		isAlive = false;
		// Vehicle died
		if (isVehicle)
		{
			// Send dead custom to all soldiers' controllers
			for (int i = 0; i < passengerControllerIDs.Count(); i++)
			{
				Commands->Send_Custom_Event(obj, Commands->Find_Object(passengerControllerIDs[i]), CUSTOM_AI_VEHICLE_DIED, 0, 0.1f);
			}
		}
		else
		{
			Commands->Send_Custom_Event(obj, Get_Controller(), CUSTOM_AI_SOLDIER_DIED, 0, 0.1f);
		}
	}
public:
	GameObject *Get_Controller()
	{
		GameObject *controller = Commands->Find_Object(controllerID);
		if (!controller)
		{
			Destroy_Script();
			return nullptr;
		}
		return controller;
	}

};
REGISTER_SCRIPT(MS_AI_Player_Controller_Spawned_Object, "ControllerId=0:int,IsVehicle=0:int");


/*!
* \brief Replace weapon when the unit is AI controlled
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* This script replaces one of the object's weapons the script is attached to with another one
* if the object is AI controlled. It helps to use the same preset for bot units that need a
* different weapon than the players.
*
* \param OriginalWeapon
*   The name of the weapon preset to replace.
* \param NewWeapon
*   The name of the weapon preset to grant.
* \param SelectOnGrant
*   Select the granted weapon (1) or not (0).
*/
class MS_AI_Replace_Weapon : public ScriptImpClass {
	void Created(GameObject *obj) override
	{
		if (!obj->As_VehicleGameObj())
			Activate_Script(obj, obj);
	}

	void Custom(GameObject *obj, int type, int param, GameObject *sender) override
	{
		if (type == CUSTOM_EVENT_VEHICLE_OWNER)
		{
			GameObject *driver = Commands->Find_Object(param);
			Activate_Script(obj, driver);
		}
	}

	void Activate_Script(GameObject *obj, GameObject *checkIfAI)
	{
		if (!checkIfAI->As_SmartGameObj()->Is_Human_Controlled())
		{
			Remove_Weapon(obj, Get_Parameter("OriginalWeapon"));
			Grant_Weapon(obj, Get_Parameter("NewWeapon"), true, 999999, false);
			if (Get_Int_Parameter("SelectOnGrant") > 0)
				Commands->Select_Weapon(obj, Get_Parameter("NewWeapon"));
		}
		Destroy_Script();
	}
};
REGISTER_SCRIPT(MS_AI_Replace_Weapon,
	"OriginalWeapon=null:string,"
	"NewWeapon=null:string,"
	"SelectOnGrant=0:int"
);


/*!
* \brief AI Bomb Specialist, helps AI use C4 weapons by faking the placement
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* Place this script on presets that carry bombs/c4/etc. Give them a fake bomb count, and when
* they enter a MS_AI_Bomb_Specialist_Zone, it will deduct a fake bomb from them. This ensures
* that the amount of bombs they can place is limited to the specificed count.
* The script helps to use the same preset for bot units that trigger customs in bomb specalist
* zones.
*
* \param BombCount
*   The count of bombs this unit can carry/place.
*/
class MS_AI_Bomb_Specialist : public ScriptImpClass
{
	int bombCount;

	void Created(GameObject* obj) override
	{
		bombCount = 0;
		if (!obj->As_SoldierGameObj())
		{
			StringClass msg;
			msg.Format("[%d:%hs:%hs] Critical Error: Only objects of type SoldierGameObj can use the MS_AI_Bomb_Specialist script. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name());
			M_Console_Output(msg.Peek_Buffer());
			Destroy_Script();
			return;
		}

		// Only for AI
		if (obj->As_SmartGameObj()->Is_Human_Controlled())
		{
			Destroy_Script();
			return;
		}

		bombCount = Get_Int_Parameter("BombCount");
		if (bombCount < 0)
			bombCount = 0;
	}

public:
	bool UseBomb()
	{
		if (bombCount > 0)
		{
			bombCount--;
			return true;
		}
		return false;
	}
};
REGISTER_SCRIPT(MS_AI_Bomb_Specialist,
	"BombCount=1:int"
);


/*!
* \brief AI Bomb Specialist C4 fake placement trigger zone
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* Place this script on script zones that when entered by a bomb specialist unit (with more than
* 0 bombs), should send a custom to a specific preset. Then anything can happen on that preset.
* In AR for example the presets attach a bomb to themselves when receiving the custom.
* Secondly, it also sends the same custom back to enterer bomb specialist unit.
*
* \param Team
*   The team of the entering bomb specialist to trigger on.
* \param SendCustom
*   The custom to send to the specified preset and also the enterer. The custom is only sent if
*   the bomb specialist unit has more than 0 bombs left.
* \param SendToPreset
*   The preset to send the custom to. If multiple objects of the same preset name are present,
*   they will all receive it.
*/
class MS_AI_Bomb_Specialist_Zone : public ScriptImpClass
{
	int team;
	const char *presetName;

	void Created(GameObject* obj)
	{
		team = Get_Int_Parameter("Team");
		if (team < 0 || team > 1)
			team = 0;

		presetName = Get_Parameter("SendToPreset");
		if (!Is_Valid_Preset(presetName))
		{
			StringClass msg;
			msg.Format("[%d:%hs:%hs] Critical Error: SendToPreset: %hs is not a valid preset name. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), presetName);
			M_Console_Output(msg.Peek_Buffer());
			Destroy_Script();
			return;
		}
	}

	void Entered(GameObject *obj, GameObject *enterer) override
	{
		// Soldiers only
		if (enterer->As_SoldierGameObj() &&
			Get_Object_Type(enterer) == team && Is_Script_Attached(enterer, "MS_AI_Bomb_Specialist"))
		{
			// Get script from enterer
			MS_AI_Bomb_Specialist* bsScript = (MS_AI_Bomb_Specialist *)(Find_Script_On_Object(enterer, "MS_AI_Bomb_Specialist"));
			if (bsScript && bsScript->UseBomb())
			{
				// Send custom to bombable object by preset name
				Send_Custom_Event_To_Object(obj, presetName, Get_Int_Parameter("SendCustom"), 0, 0.0f);
				Commands->Send_Custom_Event(obj, enterer, Get_Int_Parameter("SendCustom"), 0, 0.0f);
			}
		}
	}
};
REGISTER_SCRIPT(MS_AI_Bomb_Specialist_Zone,
	"Team=0:int,"
	"SendCustom=0:int,"
	"SendToPreset=null:string"
);


/*!
* \brief AI Refinery money distributor
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* Subject to change. DO NOT PLACE THIS MANUALLY!
*
* \param None
*   None
*/
class MS_AI_Refinery : public ScriptImpClass
{
	float moneyPerSec;
	float moneyFromDump;
	bool harvDocked;

	void Created(GameObject* obj) override
	{
		if (!obj->As_BuildingGameObj() || !obj->As_BuildingGameObj()->As_RefineryGameObj())
		{
			StringClass msg;
			msg.Format("[%d:%hs:%hs] Critical Error: Only objects of type RefineryGameObj can use the MS_AI_Refinery script. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name());
			M_Console_Output(msg.Peek_Buffer());
			Destroy_Script();
			return;
		}

		auto ref = obj->As_BuildingGameObj()->As_RefineryGameObj();
		moneyPerSec = ref->Get_Definition().Get_Funds_Distributed_Per_Sec();
		moneyFromDump = ref->Get_Definition().Get_Funds_Gathered();
		harvDocked = false;

		Commands->Start_Timer(obj, this, 1.0f, 0);
	}

	void Timer_Expired(GameObject* obj, int number) override
	{
		Commands->Start_Timer(obj, this, 1.0f, 0);
		if (moneyPerSec != 0.0f)
			MS_AccessHelper::Give_Bot_Credits(Get_Object_Type(obj), moneyPerSec);

		if (auto ref = obj->As_BuildingGameObj()->As_RefineryGameObj())
		{
			if (harvDocked != ref->Get_Is_Harvester_Docked())
			{
				harvDocked = ref->Get_Is_Harvester_Docked();
				// This will trigger the money giving on every undocking, unless undocking happened due to harvester dying (killed during dump)
				if (!harvDocked && ref->Get_Harvester_Vehicle())
					MS_AccessHelper::Give_Bot_Credits(Get_Object_Type(obj), moneyFromDump);
			}
		}
	}
};
REGISTER_SCRIPT(MS_AI_Refinery, "");


/*!
* \brief AI spectator, able to spectate bots
* \author Daniel Szabo (daniel.szabo.715@gmail.com)
* \ingroup scripts_ai
*
* Subject to change.
*
* \param None
*   None
*/
class MS_AI_Spectator : public JFW_Key_Hook_Base
{
	DynamicVectorClass<int> bots;
	bool spectateNext;
	bool isSpectating;
	int currentBotIndex;
	int currentBotCtrlID;
	int currentBotID;
	const char *currentCamera;

	void Created(GameObject* obj) override
	{
		spectateNext = true;
		isSpectating = true;
		currentBotIndex = 0;	
		currentBotCtrlID = 0;	
		currentBotID = 0;	
		currentCamera = nullptr;

		if (!obj->As_SoldierGameObj())
		{
			StringClass msg;
			msg.Format("[%d:%hs:%hs] Critical Error: Only objects of type SoldierGameObj can use the MS_AI_Spectator script. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name());
			M_Console_Output(msg.Peek_Buffer());
			Destroy_Script();
			return;
		}

		currentCamera = "Normal";

		InstallHook(Get_Parameter("SwitchKeyhook"), obj);
		Refresh_Bot_List();
		Toggle_Fly_Mode(obj);
		Commands->Set_Model(obj, "NULL");
		Commands->Set_Is_Visible(obj, false);
		PhysicalGameObj* physical_obj = obj->As_PhysicalGameObj();
		if (physical_obj)
		{
			PhysClass* phys = physical_obj->Peek_Physical_Object();
			if (phys)
			{
				phys->Set_Collision_Group(UNCOLLIDEABLE_GROUP);
			}
		}

		KeyHook();
		Commands->Start_Timer(obj, this, 1.5f, 0);
	}

	void KeyHook()
	{
		GameObject *obj = Owner();
		if (!obj)
			return;

		if (bots.Count() == 0)
		{
			Refresh_Bot_List();
			if (bots.Count() == 0)
			{ 
				Send_Message_Player(obj, DP88_RGB_GENERAL_MSG, "[AI Spectator]: There are no bots to spectate");
				isSpectating = false;
				return;
			}
		}

		if (!spectateNext)
		{
			Commands->Attach_To_Object_Bone(obj, nullptr, "");
			spectateNext = true;
			isSpectating = false;
			Send_Message_Player(obj, DP88_RGB_GENERAL_MSG, "[AI Spectator]: Free flying mode active");
			return;
		}
		else
			isSpectating = true;

		if (GameObject* cur_bot = Commands->Find_Object(bots[currentBotIndex]))
		{
			currentBotCtrlID = bots[currentBotIndex];
			Spectate_Current();

			if (auto playerControllerScript = (MS_AI_Player_Controller *)(Find_Script_On_Object(cur_bot, "MS_AI_Player_Controller")))
			{
				StringClass message;
				message.Format("[AI Spectator]: Now Spectating %s", playerControllerScript->Get_Bot_Name().Peek_Buffer());
				Send_Message_Player(obj, DP88_RGB_GENERAL_MSG, message);
			}

			currentBotIndex++;
			if (currentBotIndex >= bots.Count())
			{
				currentBotIndex = 0;
				spectateNext = false;

				Refresh_Bot_List();
			}
		}
		else
		{
			if (currentBotIndex < bots.Count())
				currentBotIndex++;
			else
			{
				currentBotIndex = 0;
				Refresh_Bot_List();
			}

			if (bots.Count() > 0)
				KeyHook();
		}
	}

	void Timer_Expired(GameObject* obj, int number) override
	{
		if (number == 0)
		{ 
			Commands->Start_Timer(obj, this, 1.0f, 0);
			if (!isSpectating)
				return;

			if (!Commands->Find_Object(currentBotCtrlID))
				KeyHook();

			else if (!Commands->Find_Object(currentBotID))
				Spectate_Current();
			
			else
				Check_Camera(obj);
		}
	}

	void Spectate_Current()
	{
		GameObject *obj = Owner();
		if (!obj)
			return;

		currentBotID = 0;

		if (auto botCtrlObj = Commands->Find_Object(currentBotCtrlID))
		{ 
			auto playerControllerScript = (MS_AI_Player_Controller *)(Find_Script_On_Object(botCtrlObj, "MS_AI_Player_Controller"));
			if (!playerControllerScript)
				return;

			auto botObj = Commands->Find_Object(playerControllerScript->Get_Bot_ID());

			if (!botObj)
			{
				StringClass message;
				message.Format("[AI Spectator]: %s: waiting to spawn...", playerControllerScript->Get_Bot_Name().Peek_Buffer());
				Send_Message_Player(obj, DP88_RGB_GENERAL_MSG, message);
				return;
			}

			currentBotID = playerControllerScript->Get_Bot_ID();

			// TODO: attach to vehicle and not soldier obj if it's in a vehicle?
			Commands->Attach_To_Object_Bone(obj, botObj, "");
			Check_Camera(obj);
		}
	}

	void Check_Camera(GameObject* obj)
	{
		if (auto botObj = Commands->Find_Object(currentBotID))
		{
			// Check if camera obj is the soldier or vehicle (might not be needed?)
			auto cameraObj = Get_Vehicle(botObj) ? Get_Vehicle(botObj) : botObj;
			// Get camera
			const char *cameraProfile = "";
			if (auto soldier = cameraObj->As_SoldierGameObj()) {
				cameraProfile = "Normal";
			}
			else if (auto vehicle = cameraObj->As_VehicleGameObj()) {
				cameraProfile = vehicle->Get_Profile();
			}

			// Update camera if needed
			if (strcmp(cameraProfile, currentCamera) != 0 && strcmp(cameraProfile, "") != 0)
			{
				currentCamera = cameraProfile;
				Set_Camera_Player(obj, cameraProfile);
			}

			auto logObj = Get_Vehicle(botObj) && Get_Vehicle(botObj)->As_VehicleGameObj()->Get_Driver() == botObj ? cameraObj : botObj;
			if (auto behaviourScript = (MS_AI_Behaviour *)(Find_Script_On_Object(logObj, "MS_AI_Behaviour")))
			{
				StringClass buf;
				buf.Format("Objective Type: %d", behaviourScript->Get_Objective_Type());
				Send_Message_Player(obj, 0, 255, 0, buf.Peek_Buffer());

				if (behaviourScript->Get_State())
				{
					StringClass buff;
					buff.Format("State: %d", behaviourScript->Get_State());
					Send_Message_Player(obj, 0, 255, 0, buff.Peek_Buffer());
				}

				if (auto overrideTask = behaviourScript->Get_Override_Task())
				{
					StringClass buff;
					buff.Format("Override task ID: %d, Type: %d", overrideTask->Get_ID(), behaviourScript->Get_Override_Task_Type());
					Send_Message_Player(obj, 0, 255, 0, buff.Peek_Buffer());

					if (auto target = overrideTask)
					{
						StringClass buff2;
						buff2.Format("Target is: %s", Commands->Get_Preset_Name(target));
						Send_Message_Player(obj, 0, 255, 0, buff2.Peek_Buffer());
						/*if (target->As_SmartGameObj() && target->As_SmartGameObj()->Get_Host_Object())
						{
							auto hostObj = target->As_SmartGameObj()->Get_Host_Object();
							StringClass buff3;
							buff3.Format("Attached to a: %s", Commands->Get_Preset_Name(hostObj));
							Send_Message_Player(obj, 0, 255, 0, buff3.Peek_Buffer());
						}*/
						
					}
				}
				if (auto objective = behaviourScript->Get_Current_Objective())
				{
					StringClass buff;
					buff.Format("Objective dist: %d", (int)Vector3::Distance(Commands->Get_Position(objective), Commands->Get_Position(logObj)));
					Send_Message_Player(obj, 0, 255, 0, buff.Peek_Buffer());
				}
			}
		}
	}

	void Refresh_Bot_List()
	{
		bots.Delete_All();

		DynamicVectorClass<int> botsTeam1;
		DynamicVectorClass<int> botsTeam0;
		auto botObjects = Combine_Lists(MS_AI_Global_Controller::GetInstance().botControllersTeam[1], MS_AI_Global_Controller::GetInstance().botControllersTeam[0]);
		for (int i = 0; i < botObjects.Count(); i++)
		{
			auto botObject = botObjects[i];
			if (Get_Object_Type(botObject) == 1)
				botsTeam1.Add(Commands->Get_ID(botObject));
			else
				botsTeam0.Add(Commands->Get_ID(botObject));
		}
		// Bots are now ordered: first the team1 bots, then team0 bots
		bots = Combine_Lists(botsTeam1, botsTeam0);
	}
};
REGISTER_SCRIPT(MS_AI_Spectator,"SwitchKeyhook=IDeploy:string");



/*------------------------
Helpers to avoid moving scripts to header
--------------------------*/


void MS_AccessHelper::Init_AI_Global_Controller()
{
	MS_AI_Global_Controller::GetInstance();
}

int MS_AccessHelper::Get_AI_Player_Controllers_Count(int team)
{
	return MS_AI_Global_Controller::GetInstance().botControllersTeam[team].Count();
}

void MS_AccessHelper::Give_Bot_Credits(int team, float amount)
{
	if (!MS_AI_Global_Controller::GetMapInited())
		return;

	auto controllers = MS_AI_Global_Controller::GetInstance().botControllersTeam[team];
	for (int i = 0; i < controllers.Count(); i++)
	{
		MS_AI_Player_Controller *playerControllerScript = (MS_AI_Player_Controller *)(Find_Script_On_Object(controllers[i], "MS_AI_Player_Controller"));
		if (playerControllerScript)
			playerControllerScript->Increment_Money(amount * MS_AI_Global_Controller::GetInstance().grantCreditsMultiplier);
	}
}

float MS_AccessHelper::Steal_Bot_Credits(int team, float percentage)
{
	if (!MS_AI_Global_Controller::GetMapInited())
		return 0;

	float theft = 0;
	auto controllers = MS_AI_Global_Controller::GetInstance().botControllersTeam[team];
	for (int i = 0; i < controllers.Count(); i++)
	{
		MS_AI_Player_Controller *playerControllerScript = (MS_AI_Player_Controller *)(Find_Script_On_Object(controllers[i], "MS_AI_Player_Controller"));
		if (playerControllerScript)
		{
			float credits = playerControllerScript->Get_Money();
			credits = credits * percentage;
			theft += credits;
			playerControllerScript->Increment_Money(-credits);
		}
	}
	return theft;
}

void MS_AccessHelper::Receive_Radio_Command(GameObject *sender, int AnnouncementID, int orderType, int orderObjID)
{
	if (!MS_AI_Global_Controller::GetMapInited())
		return;

	MS_AI_Global_Controller::GetInstance().Receive_Radio_Command(sender, AnnouncementID, orderType, orderObjID);
}

void MS_AccessHelper::Send_Radio_Order_To_Bot(GameObject *sender, GameObject *receiver, int radioIndex, int orderType, int orderObjID)
{
	if (receiver)
	{
		if (auto playerControllerScript = (MS_AI_Player_Controller *)(Find_Script_On_Object(receiver, "MS_AI_Player_Controller")))
		{
			playerControllerScript->Receive_Radio_Message(sender, radioIndex, orderType, orderObjID);
		}
	}
}

float MS_AccessHelper::Get_Money(GameObject *object)
{
	if (auto botScript = (MS_AI_Player_Controller_Spawned_Object *)(Find_Script_On_Object(object, "MS_AI_Player_Controller_Spawned_Object")))
	{
		if (auto botControllerObj = botScript->Get_Controller())
		{
			if (auto botControllerScript = (MS_AI_Player_Controller *)(Find_Script_On_Object(botControllerObj, "MS_AI_Player_Controller")))
			{
				return botControllerScript->Get_Money();
			}
		}
	}
	return 0;
}

void MS_AccessHelper::Give_Money(GameObject *object, float money)
{
	if (auto botScript = (MS_AI_Player_Controller_Spawned_Object *)(Find_Script_On_Object(object, "MS_AI_Player_Controller_Spawned_Object")))
	{
		if (auto botControllerObj = botScript->Get_Controller())
		{
			if (auto botControllerScript = (MS_AI_Player_Controller *)(Find_Script_On_Object(botControllerObj, "MS_AI_Player_Controller")))
			{
				botControllerScript->Increment_Money(money);
			}
		}
	}
}

void MS_AccessHelper::Activate_Defense_Protocol(int team)
{
	if (auto teamCtrlObj = MS_AI_Global_Controller::GetInstance().teamControllers[team])
	{
		if (auto teamCtrlScript = (MS_AI_Team_Controller *)(Find_Script_On_Object(teamCtrlObj, "MS_AI_Team_Controller")))
		{
			teamCtrlScript->Activate_Defense_Protocol(teamCtrlObj);
		}
	}
}
