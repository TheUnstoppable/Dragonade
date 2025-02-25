/*	Renegade Scripts.dll
	Copyright 2022 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#include "General.h"
#include "engine.h"
#include "engine_common.h"
#include "engine_def.h"
#include "engine_player.h"
#include "engine_tt.h"
#include "engine_script.h"
#include "engine_obj.h"
#include "WeaponClass.h"
#include "SoldierGameObj.h"
#include "VehicleGameObj.h"
#include "BuildingGameObj.h"
#include "SoldierGameObjDef.h"
#include "cPlayer.h"
#include "unstoppable.h"

void UP_Teleport::Entered(GameObject* obj, GameObject* enterer)
{
	if (enterer->As_SoldierGameObj() || enterer->As_VehicleGameObj())
	{
		Commands->Set_Position(enterer, Commands->Get_Position(Commands->Find_Object(Get_Int_Parameter("ArrowID"))));
		if (Get_Parameter("TeleportSound"))
		{
			Create_2D_WAV_Sound_Player(enterer, Get_Parameter("TeleportSound"));
		}
	}
}

ScriptRegistrant<UP_Teleport> UP_Teleport_Registrant("UP_Teleport", "ArrowID=0:int,TeleportSound=SoundName.wav:string");

/******************************************************************************************************/

void UP_HUD_Message_Zone_Enter::Entered(GameObject* obj, GameObject* enterer)
{
	if (Commands->Is_A_Star(enterer))
	{
		if (Get_Parameter("Message"))
		{
			Set_HUD_Help_Text_Player_Text(enterer, 7811, Get_Parameter("Message"), Vector3(Get_Int_Parameter("Red") / 255.f, Get_Int_Parameter("Green") / 255.f, Get_Int_Parameter("Blue") / 255.f));
		}

		if (Get_Parameter("Sound"))
		{
			Create_2D_WAV_Sound_Player(enterer, Get_Parameter("Sound"));
		}
	}
}

ScriptRegistrant<UP_HUD_Message_Zone_Enter> UP_HUD_Message_Zone_Enter_Registrant("UP_HUD_Message_Zone_Enter", "Message=Your Message:string,Sound=SoundName.wav:string,Red=255:int,Green=255:int,Blue=255:int");

/******************************************************************************************************/

void UP_HUD_Message_Zone_Exit::Exited(GameObject* obj, GameObject* exiter)
{
	if (Commands->Is_A_Star(exiter))
	{
		if (Get_Parameter("Message"))
		{
			Set_HUD_Help_Text_Player_Text(exiter, 7811, Get_Parameter("Message"), Vector3(Get_Int_Parameter("Red") / 255.f, Get_Int_Parameter("Green") / 255.f, Get_Int_Parameter("Blue") / 255.f));
		}

		if (Get_Parameter("Sound"))
		{
			Create_2D_WAV_Sound_Player(exiter, Get_Parameter("Sound"));
		}
	}
}

ScriptRegistrant<UP_HUD_Message_Zone_Exit> UP_HUD_Message_Zone_Exit_Registrant("UP_HUD_Message_Zone_Exit", "Message=Your Message:string,Sound=SoundName.wav:string,Red=255:int,Green=255:int,Blue=255:int");

/******************************************************************************************************/

void UP_Stop_Vehicle_Engine::Created(GameObject* obj)
{
	Vehicle = obj;
	if (Vehicle->As_VehicleGameObj())
	{
		Commands->Start_Timer(Vehicle, this, Get_Float_Parameter("Duration"), 1);
		Commands->Enable_Engine(Vehicle, false);
		Vehicle->As_VehicleGameObj()->Set_Immovable(true);
	}
	else
	{
		Destroy_Script();
	}
}

void UP_Stop_Vehicle_Engine::Custom(GameObject* obj, int type, int param, GameObject* sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		Commands->Enable_Engine(Vehicle, false);
		Vehicle->As_VehicleGameObj()->Set_Immovable(true);
	}
}

void UP_Stop_Vehicle_Engine::Timer_Expired(GameObject* obj, int number)
{
	Commands->Enable_Engine(obj, true);
	Vehicle->As_VehicleGameObj()->Set_Immovable(false);
	Destroy_Script();
}

ScriptRegistrant<UP_Stop_Vehicle_Engine> UP_Stop_Vehicle_Engine_Registrant("UP_Stop_Vehicle_Engine", "Duration=0:float");

/******************************************************************************************************/

void UP_Change_Vehicle_Weapon::Created(GameObject* obj)
{
	if (obj->As_VehicleGameObj())
	{
		Commands->Clear_Weapons(obj);
		Grant_Weapon(obj, Get_Parameter("Preset"), true, Get_Int_Parameter("Rounds"), false);
		Commands->Select_Weapon(obj, Get_Parameter("Preset"));
	}

	Destroy_Script();
}

ScriptRegistrant<UP_Change_Vehicle_Weapon> UP_Change_Vehicle_Weapon_Registrant("UP_Change_Vehicle_Weapon", "Preset=Weapon:string,Rounds=1:int");

/******************************************************************************************************/

void UP_MCT_Building::Created(GameObject* obj)
{
	MCTObj = Commands->Find_Object(Get_Int_Parameter("MCTID"));
	if (MCTObj)
	{
		if (DamageableGameObj* damageable = MCTObj->As_DamageableGameObj())
		{
			if (DamageableGameObj* damageable2 = obj->As_DamageableGameObj())
			{
				float maxShield = Commands->Get_Max_Shield_Strength(damageable2);
				float maxHealth = Commands->Get_Max_Health(damageable2);
				StringClass healthSkin = ArmorWarheadManager::Get_Armor_Name(damageable2->Get_Defense_Object()->Get_Skin());
				StringClass shieldSkin = ArmorWarheadManager::Get_Armor_Name(damageable2->Get_Defense_Object()->Get_Shield_Type());
				Set_Max_Shield_Strength(damageable, maxShield);
				Set_Max_Health(damageable, maxHealth);
				damageable->Get_Defense_Object()->Set_Skin(ArmorWarheadManager::Get_Armor_Type(healthSkin));
				damageable->Get_Defense_Object()->Set_Shield_Type(ArmorWarheadManager::Get_Armor_Type(shieldSkin));
				damageable->Get_Defense_Object()->Mark_Owner_Dirty();

				Attach_Script_V(MCTObj, "UP_Master_Control_Terminal", "%d,%f", Commands->Get_ID(damageable2), Get_Float_Parameter("Multiplier"));
			}
			else
			{
				Destroy_Script();
			}
		}
		else
		{
			Destroy_Script();
		}
	}
	else
	{
		Destroy_Script();
	}
}

void UP_MCT_Building::Damaged(GameObject* obj, GameObject* damager, float amount)
{
	Commands->Set_Health(MCTObj, Commands->Get_Health(obj));
	Commands->Set_Shield_Strength(MCTObj, Commands->Get_Shield_Strength(obj));
}

ScriptRegistrant<UP_MCT_Building> UP_MCT_Building_Registrant("UP_MCT_Building", "MCTID:int,Multiplier:float");

/******************************************************************************************************/

void UP_Master_Control_Terminal::Created(GameObject* obj)
{
	MainBuilding = Commands->Find_Object(Get_Int_Parameter("Object"));
	Multiplier = Get_Float_Parameter("Multiplier");
	Set_Object_Type(obj, Get_Object_Type(MainBuilding));
}

void UP_Master_Control_Terminal::Damaged(GameObject* obj, GameObject* damager, float amount)
{
	Commands->Set_Health(obj, Commands->Get_Max_Health(obj));
	Commands->Set_Shield_Strength(obj, Commands->Get_Max_Shield_Strength(obj));
	Commands->Apply_Damage(MainBuilding, amount * Multiplier, "None", damager);
}

ScriptRegistrant<UP_Master_Control_Terminal> UP_Master_Control_Terminal_Registrant("UP_Master_Control_Terminal", "Object:int,Multiplier=1.00:float");

/******************************************************************************************************/

void UP_Grant_Weapon::Created(GameObject* obj)
{
	if (DefinitionClass* Def = Find_Named_Definition(Get_Parameter("Preset")))
	{
		if (Def->Get_Class_ID() == CID_Weapon)
		{
			Grant_Weapon(obj, Def->Get_Name(), true, 0, true);
			Set_Clip_Bullets(obj, Def->Get_Name(), Get_Int_Parameter("ClipRounds"));
			Set_Bullets(obj, Def->Get_Name(), Get_Int_Parameter("InventoryRounds"));
			if (Get_Int_Parameter("AutoSelect"))
			{
				Commands->Select_Weapon(obj, Def->Get_Name());
			}
		}
	}
	Destroy_Script();
}

ScriptRegistrant<UP_Grant_Weapon> UP_Grant_Weapon_Registrant("UP_Grant_Weapon", "Preset=Weapon:string,ClipRounds=1:int,InventoryRounds=1:int,AutoSelect=1:int");

/******************************************************************************************************/

void UP_Grant_Weapon_Zone_Enter::Entered(GameObject* obj, GameObject* enterer)
{
	if (DefinitionClass* Def = Find_Named_Definition(Get_Parameter("Preset")))
	{
		if (Def->Get_Class_ID() == CID_Weapon)
		{
			Grant_Weapon(enterer, Def->Get_Name(), true, 0, true);
			Set_Clip_Bullets(enterer, Def->Get_Name(), Get_Int_Parameter("ClipRounds"));
			Set_Bullets(enterer, Def->Get_Name(), Get_Int_Parameter("InventoryRounds"));
			if (Get_Int_Parameter("AutoSelect"))
			{
				Commands->Select_Weapon(enterer, Def->Get_Name());
			}
		}
	}
}

ScriptRegistrant<UP_Grant_Weapon_Zone_Enter> UP_Grant_Weapon_Zone_Enter_Registrant("UP_Grant_Weapon_Zone_Enter", "Preset=Weapon:string,ClipRounds=1:int,InventoryRounds=1:int,AutoSelect=1:int");

/******************************************************************************************************/

void UP_Grant_Weapon_Zone_Exit::Exited(GameObject* obj, GameObject* exiter)
{
	if (DefinitionClass* Def = Find_Named_Definition(Get_Parameter("Preset")))
	{
		if (Def->Get_Class_ID() == CID_Weapon)
		{
			Grant_Weapon(exiter, Def->Get_Name(), true, 0, true);
			Set_Clip_Bullets(exiter, Def->Get_Name(), Get_Int_Parameter("ClipRounds"));
			Set_Bullets(exiter, Def->Get_Name(), Get_Int_Parameter("InventoryRounds"));
			if (Get_Int_Parameter("AutoSelect"))
			{
				Commands->Select_Weapon(exiter, Def->Get_Name());
			}
		}
	}
}

ScriptRegistrant<UP_Grant_Weapon_Zone_Exit> UP_Grant_Weapon_Zone_Exit_Registrant("UP_Grant_Weapon_Zone_Exit", "Preset=Weapon:string,ClipRounds=1:int,InventoryRounds=1:int,AutoSelect=1:int");

/******************************************************************************************************/

void UP_Spawn_When_Death::Killed(GameObject* obj, GameObject* killer)
{
	GameObject* spawnedObj = Commands->Create_Object(Get_Parameter("Preset"), Commands->Get_Position(obj));
	Commands->Set_Facing(spawnedObj, Commands->Get_Facing(obj));
}

ScriptRegistrant<UP_Spawn_When_Death> UP_Spawn_When_Death_Registrant("UP_Spawn_When_Death", "Preset=Object:string");

/******************************************************************************************************/

void UP_GameOver_When_Death::Killed(GameObject* obj, GameObject* killer)
{
	Console_Input("gameover");
}

ScriptRegistrant<UP_GameOver_When_Death> UP_GameOver_When_Death_Registrant("UP_GameOver_When_Death", "");

/******************************************************************************************************/

void UP_Change_Character::Created(GameObject* obj)
{
	if (obj->As_SoldierGameObj())
	{
		if (DefinitionClass* Def = Find_Named_Definition(Get_Parameter("Preset")))
		{
			if (SoldierGameObjDef* SoldierDef = (SoldierGameObjDef*)Def)
			{
				obj->As_SoldierGameObj()->Re_Init(*SoldierDef);
				obj->As_SoldierGameObj()->Post_Re_Init();
			}
		}
	}
	Destroy_Script();
}

ScriptRegistrant<UP_Change_Character> UP_Change_Character_Registrant("UP_Change_Character", "Preset=Character:string");

/******************************************************************************************************/

void UP_Grant_Character_Zone_Enter::Entered(GameObject* obj, GameObject* enterer)
{
	if (enterer->As_SoldierGameObj())
	{
		if (DefinitionClass* Def = Find_Named_Definition(Get_Parameter("Preset")))
		{
			if (SoldierGameObjDef* SoldierDef = (SoldierGameObjDef*)Def)
			{
				enterer->As_SoldierGameObj()->Re_Init(*SoldierDef);
				enterer->As_SoldierGameObj()->Post_Re_Init();
			}
		}
	}
}

ScriptRegistrant<UP_Grant_Character_Zone_Enter> UP_Grant_Character_Zone_Enter_Registrant("UP_Grant_Character_Zone_Enter", "Preset=Character:string");

/******************************************************************************************************/

void UP_Grant_Character_Zone_Exit::Exited(GameObject* obj, GameObject* exiter)
{
	if (exiter->As_SoldierGameObj())
	{
		if (DefinitionClass* Def = Find_Named_Definition(Get_Parameter("Preset")))
		{
			if (SoldierGameObjDef* SoldierDef = (SoldierGameObjDef*)Def)
			{
				exiter->As_SoldierGameObj()->Re_Init(*SoldierDef);
				exiter->As_SoldierGameObj()->Post_Re_Init();
			}
		}
	}
}

ScriptRegistrant<UP_Grant_Character_Zone_Exit> UP_Grant_Character_Zone_Exit_Registrant("UP_Grant_Character_Zone_Exit", "Preset=Character:string");

/******************************************************************************************************/

void UP_Spawn_And_Destroy_When_Fully_Repaired::Damaged(GameObject* obj, GameObject* damager, float amount)
{
	if (amount < 0)
	{
		float current = Commands->Get_Health(obj) + Commands->Get_Shield_Strength(obj);
		float max = Commands->Get_Max_Health(obj) + Commands->Get_Max_Shield_Strength(obj);
		if (current == max)
		{
			GameObject* newObject = Commands->Create_Object(Get_Parameter("Preset"), Commands->Get_Position(obj));
			Commands->Set_Facing(newObject, Commands->Get_Facing(obj));
			obj->Set_Delete_Pending();
			Destroy_Script();
		}
	}
}

ScriptRegistrant<UP_Spawn_And_Destroy_When_Fully_Repaired> UP_Spawn_And_Destroy_When_Fully_Repaired_Registrant("UP_Spawn_And_Destroy_When_Fully_Repaired", "Preset=Object:string");

/******************************************************************************************************/

void UP_Refinery_Object::Created(GameObject* obj)
{
	Team = Get_Int_Parameter("Team");
	Amount = Get_Float_Parameter("Amount");
	Set_Object_Type(obj, Team);
	Commands->Start_Timer(obj, this, Get_Float_Parameter("Interval"), 5);
}

void UP_Refinery_Object::Timer_Expired(GameObject* obj, int number)
{
	for (SLNode<cPlayer>* z = Get_Player_List()->Head(); z; z = z->Next())
	{
		cPlayer* player = z->Data();
		if (player->Get_Is_In_Game() && player->Get_Player_Type() == Team)
		{
			player->Set_Money(player->Get_Money() + Amount);
		}
	}
}

ScriptRegistrant<UP_Refinery_Object> UP_Refinery_Object_Registrant("UP_Refinery_Object", "Team=0:int,Interval=1.0:float,Amount=2.0:float");

/******************************************************************************************************/

void UP_Vehicle_Purchase_Terminal::Created(GameObject* obj)
{
	Preset = Get_Parameter("VehiclePreset");
	SpawnLocation = Commands->Get_Position(Commands->Find_Object(Get_Int_Parameter("SpawnerID")));
	Facing = Commands->Get_Facing(Commands->Find_Object(Get_Int_Parameter("SpawnerID")));
	Terminal = obj;
	Pokable = true;
	Amount = Get_Int_Parameter("Credits");

	if (DefinitionClass* Def = Find_Named_Definition(Preset))
	{
		if (Def->Get_Class_ID() != CID_Vehicle)
		{
			Destroy_Script();
		}
	}
	else
	{
		Destroy_Script();
	}

	Commands->Enable_HUD_Pokable_Indicator(obj, true);
}

void UP_Vehicle_Purchase_Terminal::Poked(GameObject* obj, GameObject* poker)
{
	if (Pokable)
	{
		SoldierGameObj* playerObj = poker->As_SoldierGameObj();
		if (Commands->Get_Player_Type(playerObj) == Get_Int_Parameter("Team"))
		{
			if (Commands->Get_Money(playerObj) >= Amount)
			{
				GameObject* Vehicle = Commands->Create_Object(Preset, SpawnLocation);
				Commands->Set_Facing(Vehicle, Facing);
				Send_Message_Player(playerObj, 255, 255, 255, "Purchase request granted!");
				Create_2D_WAV_Sound_Player(playerObj, "M00GBMG_SECX0002I1GBMG_snd.wav");
				Commands->Give_Money(playerObj, Amount * -1.0f, false);
				Pokable = false;
				Commands->Enable_HUD_Pokable_Indicator(obj, false);
				Commands->Start_Timer(obj, this, 5.0f, 1);
			}
			else
			{
				Send_Message_Player(playerObj, 255, 255, 255, "Insufficient funds!");

				if (Get_Player_Type(poker) == 0)
				{
					Create_2D_WAV_Sound_Player(poker, "m00evan_dsgn0024i1evan_snd.wav");
				}
				if (Get_Player_Type(poker) == 1)
				{
					Create_2D_WAV_Sound_Player(poker, "m00evag_dsgn0028i1evag_snd.wav");
				}
			}
		}
		else
		{
			Send_Message_Player(playerObj, 255, 255, 255, StringClass::getFormattedString("Access Denied! You aren't in team %s.", Get_Team_Name(Get_Int_Parameter("Team"))));
		}
	}
}

void UP_Vehicle_Purchase_Terminal::Timer_Expired(GameObject* obj, int number)
{
	if (number == 1)
	{
		Pokable = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}

ScriptRegistrant<UP_Vehicle_Purchase_Terminal> UP_Vehicle_Purchase_Terminal_Registrant("UP_Vehicle_Purchase_Terminal", "Team=0:int,SpawnerID=0:int,VehiclePreset:string,Credits=400:int");

/******************************************************************************************************/

void UP_Character_Purchase_Terminal::Created(GameObject* obj)
{
	Preset = Get_Parameter("CharacterPreset");
	Terminal = obj;
	Pokable = true;
	Amount = Get_Int_Parameter("Credits");

	if (DefinitionClass* Def = Find_Named_Definition(Preset))
	{
		if (Def->Get_Class_ID() != CID_Soldier)
		{
			Destroy_Script();
		}
	}
	else
	{
		Destroy_Script();
	}

	Commands->Enable_HUD_Pokable_Indicator(obj, true);
}

void UP_Character_Purchase_Terminal::Poked(GameObject* obj, GameObject* poker)
{
	if (Pokable)
	{
		SoldierGameObj* Player = poker->As_SoldierGameObj();
		if (Commands->Get_Player_Type(Player) == Get_Int_Parameter("Team"))
		{
			if (Commands->Get_Money(poker) >= Amount)
			{
				Player->Re_Init(*(SoldierGameObjDef*)Find_Named_Definition(Preset));
				Player->Post_Re_Init();
				Send_Message_Player(Player, 255, 255, 255, "Purchase request granted!");
				Create_2D_WAV_Sound_Player(Player, "M00GBMG_SECX0002I1GBMG_snd.wav");
				Commands->Give_Money(Player, Amount * -1.0f, false);
				Pokable = false;
				Commands->Enable_HUD_Pokable_Indicator(obj, false);
				Commands->Start_Timer(obj, this, 5.0f, 1);
			}
			else
			{
				Send_Message_Player(Player, 255, 255, 255, "Insufficient funds!");

				if (Get_Player_Type(poker) == 0)
				{
					Create_2D_WAV_Sound_Player(poker, "m00evan_dsgn0024i1evan_snd.wav");
				}
				if (Get_Player_Type(poker) == 1)
				{
					Create_2D_WAV_Sound_Player(poker, "m00evag_dsgn0028i1evag_snd.wav");
				}
			}
		}
		else
		{
			Send_Message_Player(Player, 255, 255, 255, StringClass::getFormattedString("Access Denied! You aren't in team %s.", Get_Team_Name(Get_Int_Parameter("Team"))));
		}
	}
}

void UP_Character_Purchase_Terminal::Timer_Expired(GameObject* obj, int number)
{
	if (number == 1)
	{
		Pokable = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}

ScriptRegistrant<UP_Character_Purchase_Terminal> UP_Character_Purchase_Terminal_Registrant("UP_Character_Purchase_Terminal", "Team=0:int,CharacterPreset:string,Credits=400:int");

/******************************************************************************************************/

void UP_Set_Team_When_Exit::Custom(GameObject* obj, int type, int param, GameObject* sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		Set_Object_Type(obj, Get_Int_Parameter("Team"));
		Commands->Start_Timer(obj, this, 0.1f, 1);
	}
}

void UP_Set_Team_When_Exit::Timer_Expired(GameObject* obj, int number)
{
	Set_Object_Type(obj, Get_Int_Parameter("Team"));
}

ScriptRegistrant<UP_Set_Team_When_Exit> UP_Set_Team_When_Exit_Registrant("UP_Set_Team_When_Exit", "Team=-1:int");

/******************************************************************************************************/

void UP_Ion_Explosion_When_Death::Killed(GameObject* obj, GameObject* killer)
{
	Vector3 Pos = Commands->Get_Position(obj);
	Create_Explosion_Extended("Explosion_IonCannonBeacon", Pos, obj);
	Commands->Create_Object("Beacon_Ion_Cannon_Anim_Post", Commands->Get_Position(obj));
	Destroy_Script();
}

ScriptRegistrant<UP_Ion_Explosion_When_Death> UP_Ion_Explosion_When_Death_Registrant("UP_Ion_Explosion_When_Death", "");

/******************************************************************************************************/

void UP_FlyMode_Enable::Created(GameObject* obj)
{
	if (!Get_Fly_Mode(obj))
	{
		Toggle_Fly_Mode(obj);
	}

	Destroy_Script();
}

ScriptRegistrant<UP_FlyMode_Enable> UP_FlyMode_Enable_Registrant("UP_FlyMode_Enable", "");

/******************************************************************************************************/

void UP_FlyMode_Disable::Created(GameObject* obj)
{
	if (Get_Fly_Mode(obj))
	{
		Toggle_Fly_Mode(obj);
	}

	Destroy_Script();
}

ScriptRegistrant<UP_FlyMode_Disable> UP_FlyMode_Disable_Registrant("UP_FlyMode_Disable", "");

/******************************************************************************************************/

void UP_Spectate::Created(GameObject* obj)
{
	if (Commands->Is_A_Star(obj))
	{
		Console_Input(StringClass::getFormattedString("spectate %d", Get_Player_ID(obj)));
	}

	Destroy_Script();
}

ScriptRegistrant<UP_Spectate> UP_Spectate_Registrant("UP_Spectate", "");

/******************************************************************************************************/

void UP_Spectate_Zone_Enter::Entered(GameObject* obj, GameObject* enterer)
{
	if (Commands->Is_A_Star(enterer))
	{
		Console_Input(StringClass::getFormattedString("spectate %d", Get_Player_ID(enterer)));
	}
}

ScriptRegistrant<UP_Spectate_Zone_Enter> UP_Spectate_Zone_Enter_Registrant("UP_Spectate_Zone_Enter", "");

/******************************************************************************************************/

void UP_Spectate_Zone_Exit::Exited(GameObject* obj, GameObject* exiter)
{
	if (Commands->Is_A_Star(exiter))
	{
		Console_Input(StringClass::getFormattedString("spectate %d", Get_Player_ID(exiter)));
	}
}

ScriptRegistrant<UP_Spectate_Zone_Exit> UP_Spectate_Zone_Exit_Registrant("UP_Spectate_Zone_Exit", "");

/******************************************************************************************************/

void UP_Select_Weapon::Created(GameObject* obj)
{
	Commands->Select_Weapon(obj, Get_Parameter("WeaponPreset"));
}

ScriptRegistrant<UP_Select_Weapon> UP_Select_Weapon_Registrant("UP_Select_Weapon", "WeaponPreset:string");

/******************************************************************************************************/

void UP_Select_Weapon_On_Zone_Enter::Entered(GameObject* obj, GameObject* enterer)
{
	Commands->Select_Weapon(enterer, Get_Parameter("WeaponPreset"));
}

ScriptRegistrant<UP_Select_Weapon_On_Zone_Enter> UP_Select_Weapon_On_Zone_Enter_Registrant("UP_Select_Weapon_On_Zone_Enter", "WeaponPreset:string");

/******************************************************************************************************/

void UP_Select_Weapon_On_Zone_Exit::Exited(GameObject* obj, GameObject* exiter)
{
	Commands->Select_Weapon(exiter, Get_Parameter("WeaponPreset"));
}

ScriptRegistrant<UP_Select_Weapon_On_Zone_Exit> UP_Select_Weapon_On_Zone_Exit_Registrant("UP_Select_Weapon_On_Zone_Exit", "WeaponPreset:string");

/******************************************************************************************************/

void UP_Chinook_Reinforcements_Paradrop::Created(GameObject* obj)
{
	presetObj1 = 0;
	presetObj2 = 0;
	presetObj3 = 0;

	Vector3 pos = Commands->Get_Position(obj);
	float facing = Commands->Get_Facing(obj);
	trajectory = Commands->Create_Object("Generic_Cinematic", pos);
	Commands->Set_Model(trajectory, "X5D_Chinookfly");
	Commands->Set_Facing(trajectory, facing);
	Commands->Set_Animation(trajectory, "X5D_Chinookfly.X5D_Chinookfly", false, NULL, 0.0f, -1.0f, false);

	if (Get_Object_Type(obj) == 0)
	{
		helicopter = Commands->Create_Object("Nod_Transport_Helicopter_Flyover", pos);
		Commands->Set_Animation(helicopter, "v_Nod_trnspt.v_Nod_trnspt", true, NULL, 0.0f, -1.0f, false);
	}
	else
	{
		helicopter = Commands->Create_Object("GDI_Transport_Helicopter_Flyover", pos);
		Commands->Set_Animation(helicopter, "v_GDI_trnspt.v_GDI_trnspt", true, NULL, 0.0f, -1.0f, false);
	}

	Commands->Set_Facing(helicopter, facing);
	Commands->Attach_Script(helicopter, "UP_Unkillable_Until_Custom", "1496001");
	Commands->Set_Is_Visible(helicopter, false);
	Commands->Disable_All_Collisions(helicopter);
	Commands->Attach_To_Object_Bone(helicopter, trajectory, "BN_Chinook_1");

	chinookKilled = false;
	paratrooperIndex = 0;

	Attach_Script_V(helicopter, "M03_Reinforcement_Chinook", "%d", Commands->Get_ID(obj));

	Commands->Start_Timer(obj, this, 300 / 30.0f, 0);
	Commands->Start_Timer(obj, this, 169 / 30.0f, 1);
	Commands->Start_Timer(obj, this, 179 / 30.0f, 2);
	Commands->Start_Timer(obj, this, 198 / 30.0f, 3);
	Commands->Start_Timer(obj, this, 145 / 30.0f, 4);
	Commands->Start_Timer(obj, this, 155 / 30.0f, 5);
	Commands->Start_Timer(obj, this, 165 / 30.0f, 6);
	Commands->Start_Timer(obj, this, 25 / 30.0f, 7);
	Commands->Start_Timer(obj, this, 280 / 30.0f, 8);
}

void UP_Chinook_Reinforcements_Paradrop::Custom(GameObject* obj, int type, int param, GameObject* sender)
{
	if (type == 23000 && param == 23000)
	{
		chinookKilled = true;
	}
	else if (type == 15730)
	{
		if (GameObject* Attach = Commands->Find_Object(param))
		{
			presetObj1 = Attach;
		}
	}
	else if (type == 15731)
	{
		if (GameObject* Attach = Commands->Find_Object(param))
		{
			presetObj2 = Attach;
		}
	}
	else if (type == 15732)
	{
		if (GameObject* Attach = Commands->Find_Object(param))
		{
			presetObj3 = Attach;
		}
	}
}

void UP_Chinook_Reinforcements_Paradrop::Timer_Expired(GameObject* obj, int number)
{
	Vector3 pos = Commands->Get_Position(obj);
	const char* preset1 = Get_Parameter("FirstPreset");
	const char* preset2 = Get_Parameter("SecondPreset");
	const char* preset3 = Get_Parameter("ThirdPreset");
	float facing = Commands->Get_Facing(obj);

	if (number == 0)
	{
		Commands->Send_Custom_Event(0, presetObj1, 1248753, 0, 0.0f);
		Commands->Send_Custom_Event(0, presetObj2, 1248753, 0, 0.0f);
		Commands->Send_Custom_Event(0, presetObj3, 1248753, 0, 0.0f);
		Destroy_Script();
	}

	else if (number == 8)
	{
		if (helicopter)
		{
			Commands->Destroy_Object(helicopter);
		}
	}

	else if (number == 7)
	{
		if (helicopter)
		{
			Commands->Enable_Engine(helicopter, true);
		}
	}

	else if (number == 1)
	{
		if (this->paratrooperIndex > 0)
		{
			if (presetObj1)
			{
				GameObject* cinObj = Commands->Create_Object("Generic_Cinematic", pos);
				Commands->Set_Facing(cinObj, facing);
				Commands->Set_Model(cinObj, "X5D_Parachute");
				Commands->Set_Animation(cinObj, "X5D_Parachute.X5D_ParaC_1", false, NULL, 0.0f, -1.0f, false);
				Commands->Create_3D_Sound_At_Bone("parachute_open", cinObj, "ROOTTRANSFORM");
				Commands->Attach_Script(cinObj, "M03_No_More_Parachute", "");
			}
		}
	}

	else if (number == 2)
	{
		if (this->paratrooperIndex > 1)
		{
			if (presetObj2)
			{
				GameObject* cinObj = Commands->Create_Object("Generic_Cinematic", pos);
				Commands->Set_Facing(cinObj, facing);
				Commands->Set_Model(cinObj, "X5D_Parachute");
				Commands->Set_Animation(cinObj, "X5D_Parachute.X5D_ParaC_2", false, NULL, 0.0f, -1.0f, false);
				Commands->Create_3D_Sound_At_Bone("parachute_open", cinObj, "ROOTTRANSFORM");
				Commands->Attach_Script(cinObj, "M03_No_More_Parachute", "");
			}
		}
	}

	else if (number == 3)
	{
		if (this->paratrooperIndex == 3)
		{
			if (presetObj3)
			{
				GameObject* cinObj = Commands->Create_Object("Generic_Cinematic", pos);
				Commands->Set_Facing(cinObj, facing);
				Commands->Set_Model(cinObj, "X5D_Parachute");
				Commands->Set_Animation(cinObj, "X5D_Parachute.X5D_ParaC_3", false, NULL, 0.0f, -1.0f, false);
				Commands->Create_3D_Sound_At_Bone("parachute_open", cinObj, "ROOTTRANSFORM");
				Commands->Attach_Script(cinObj, "M03_No_More_Parachute", "");
			}
		}
	}

	else if (number == 4)
	{
		if (!chinookKilled)
		{
			if (Is_Valid_Preset(preset1) || presetObj1)
			{
				GameObject* trajectoryObj = Commands->Create_Object("Generic_Cinematic", pos);
				Commands->Set_Model(trajectoryObj, "X5D_Box01");
				Commands->Set_Facing(trajectoryObj, facing);
				Commands->Set_Animation(trajectoryObj, "X5D_Box01.X5D_Box01", false, NULL, 0.0f, -1.0f, false);
				if (!presetObj1)
				{
					presetObj1 = Commands->Create_Object_At_Bone(trajectoryObj, preset1, "Box01");
				}

				if (presetObj1)
				{
					if (Check_Object(presetObj1))
					{
						Commands->Set_Facing(presetObj1, facing);
						Attach_Script_Once(presetObj1, "UP_Unkillable_Until_Custom", "1248753");
						Commands->Attach_Script(presetObj1, "RMV_Trigger_Killed", "1144444, 1000, 1000");
						Commands->Attach_Script(presetObj1, "M03_Killed_Sound", "");
						Commands->Attach_Script(presetObj1, "DLS_Volcano_Stumble", "");
						Commands->Set_Animation(presetObj1, "s_a_human.H_A_X5D_ParaT_1", false, NULL, 0.0f, -1.0f, false);
					}
					Commands->Attach_To_Object_Bone(presetObj1, trajectoryObj, "Box01");
				}
			}

			paratrooperIndex++;
		}
	}

	else if (number == 5)
	{
		if (!chinookKilled)
		{
			if (Is_Valid_Preset(preset2) || presetObj2)
			{
				GameObject* trajectoryObj = Commands->Create_Object("Generic_Cinematic", pos);
				Commands->Set_Model(trajectoryObj, "X5D_Box02");
				Commands->Set_Facing(trajectoryObj, facing);
				Commands->Set_Animation(trajectoryObj, "X5D_Box02.X5D_Box02", false, NULL, 0.0f, -1.0f, false);
				if (!presetObj2)
				{
					presetObj2 = Commands->Create_Object_At_Bone(trajectoryObj, preset2, "Box02");
				}

				if (presetObj2)
				{
					if (Check_Object(presetObj2))
					{
						Commands->Set_Facing(presetObj2, facing);
						Attach_Script_Once(presetObj2, "UP_Unkillable_Until_Custom", "1248753");
						Commands->Attach_Script(presetObj2, "RMV_Trigger_Killed", "1144444, 1000, 1000");
						Commands->Attach_Script(presetObj2, "M03_Killed_Sound", "");
						Commands->Attach_Script(presetObj2, "DLS_Volcano_Stumble", "");
						Commands->Set_Animation(presetObj2, "s_a_human.H_A_X5D_ParaT_2", false, NULL, 0.0f, -1.0f, false);
					}
					Commands->Attach_To_Object_Bone(presetObj2, trajectoryObj, "Box02");
				}
			}

			paratrooperIndex++;
		}
	}

	else if (number == 6)
	{
		if (!chinookKilled)
		{
			if (Is_Valid_Preset(preset3) || presetObj3)
			{
				GameObject* trajectoryObj = Commands->Create_Object("Generic_Cinematic", pos);
				Commands->Set_Model(trajectoryObj, "X5D_Box03");
				Commands->Set_Facing(trajectoryObj, facing);
				Commands->Set_Animation(trajectoryObj, "X5D_Box03.X5D_Box03", false, NULL, 0.0f, -1.0f, false);

				if (!presetObj3)
				{
					presetObj3 = Commands->Create_Object_At_Bone(trajectoryObj, preset3, "Box03");
				}

				if (presetObj3)
				{
					if (Check_Object(presetObj3))
					{
						Commands->Set_Facing(presetObj3, facing);
						Attach_Script_Once(presetObj3, "UP_Unkillable_Until_Custom", "1248753");
						Commands->Attach_Script(presetObj3, "RMV_Trigger_Killed", "1144444, 1000, 1000");
						Commands->Attach_Script(presetObj3, "M03_Killed_Sound", "");
						Commands->Attach_Script(presetObj3, "DLS_Volcano_Stumble", "");
						Commands->Set_Animation(presetObj3, "s_a_human.H_A_X5D_ParaT_3", false, NULL, 0.0f, -1.0f, false);
					}
					Commands->Attach_To_Object_Bone(presetObj3, trajectoryObj, "Box03");
				}
			}
			
			paratrooperIndex++;
		}
	}
}

ScriptRegistrant<UP_Chinook_Reinforcements_Paradrop> UP_Chinook_Reinforcements_Paradrop_Registrant("UP_Chinook_Reinforcements_Paradrop", "FirstPreset:string,SecondPreset:string,ThirdPreset:string");

/******************************************************************************************************/

void UP_Unkillable_Until_Custom::Custom(GameObject* obj, int type, int param, GameObject* sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Destroy_Script();
	}
}

void UP_Unkillable_Until_Custom::Damaged(GameObject* obj, GameObject* damager, float amount)
{
	float maxHealth = Commands->Get_Max_Health(obj);
	Commands->Set_Health(obj, maxHealth);

	float maxShield = Commands->Get_Max_Shield_Strength(obj);
	Commands->Set_Shield_Strength(obj, maxShield);
}

ScriptRegistrant<UP_Unkillable_Until_Custom> UP_Unkillable_Until_Custom_Registrant("UP_Unkillable_Until_Custom", "Message:int");

/******************************************************************************************************/

void UP_AttachScript_Interval_Self::Created(GameObject* obj)
{
	Script = Get_Parameter("Script");
	Parameters = Get_Parameter("Parameters");
	Interval = Get_Float_Parameter("Interval");
	LoopCount = Get_Int_Parameter("LoopCount");
	Looped = 0;

	Parameters.Replace("|", ",");

	Commands->Start_Timer(obj, this, Interval, 234245);
}

void UP_AttachScript_Interval_Self::Timer_Expired(GameObject* obj, int number)
{
	if (number == 234245)
	{
		Commands->Attach_Script(obj, Script, Parameters);
		if (LoopCount != -1)
		{
			if (Looped >= LoopCount)
			{
				Destroy_Script();
			}
			else
			{
				++Looped;
				Commands->Start_Timer(obj, this, Interval, 234245);
			}
		}
		else
		{
			Commands->Start_Timer(obj, this, Interval, 234245);
		}
	}
}

ScriptRegistrant<UP_AttachScript_Interval_Self> UP_AttachScript_Interval_Self_Registrant("UP_AttachScript_Interval_Self", "Script:string,Parameters:string,LoopCount=-1:int,Interval=1:float");

/******************************************************************************************************/

void UP_AttachScript_Interval::Created(GameObject* obj)
{
	Target = Commands->Find_Object(Get_Int_Parameter("ObjectID"));
	Script = Get_Parameter("Script");
	Parameters = Get_Parameter("Parameters");
	Interval = Get_Float_Parameter("Interval");
	LoopCount = Get_Int_Parameter("LoopCount");
	Looped = 0;

	Parameters.Replace("|", ",");

	Commands->Start_Timer(obj, this, Interval, 234244);
}

void UP_AttachScript_Interval::Timer_Expired(GameObject* obj, int number)
{
	if (number == 234244)
	{
		Commands->Attach_Script(Target, Script, Parameters);
		if (LoopCount != -1)
		{
			if (Looped >= LoopCount)
			{
				Destroy_Script();
			}
			else
			{
				++Looped;
				Commands->Start_Timer(obj, this, Interval, 234244);
			}
		}
		else
		{
			Commands->Start_Timer(obj, this, Interval, 234244);
		}
	}
}

ScriptRegistrant<UP_AttachScript_Interval> UP_AttachScript_Interval_Registrant("UP_AttachScript_Interval", "ObjectID:int,Script:string,Parameters:string,LoopCount=-1:int,Interval=1:float");

/******************************************************************************************************/

void UP_Object_Follower_AI::Created(GameObject* obj)
{
	TargetChangeCustom = Get_Int_Parameter("TargetChangeCustomID");
	DestroyScriptCustom = Get_Int_Parameter("DestroyScriptCustomID");
}

void UP_Object_Follower_AI::Custom(GameObject* obj, int type, int param, GameObject* sender)
{
	if (type == TargetChangeCustom)
	{
		if(param == 0)
		{
			TargetObject = 0;
			Commands->Action_Reset(TargetObject, FLT_MAX);
			if (Has_Timer(obj, this, 12314178))
			{
				Stop_Timer2(obj, this, 12314178);
			}
		}
		else
		{
			if (GameObject* Target = Commands->Find_Object(param))
			{
				TargetObject = Target;
				Timer_Expired(obj, 12314178);

				if (!Has_Timer(obj, this, 12314178))
				{
					Commands->Start_Timer(obj, this, 5.0f, 12314178);
				}
			}
		}
	}
	else if (type == DestroyScriptCustom)
	{
		Destroy_Script();
	}
}

void UP_Object_Follower_AI::Timer_Expired(GameObject* obj, int number)
{
	if (number == 12314178)
	{
		if (TargetObject)
		{
			ActionParamsStruct Parameters;
			Parameters.Set_Basic(this, 100.0f, 20000);
			Parameters.Set_Movement(TargetObject, 0.8f, 2.5f);
			Commands->Action_Goto(TargetObject, Parameters);
		}
	}
}

ScriptRegistrant<UP_Object_Follower_AI> UP_Object_Follower_AI_Registrant("UP_Object_Follower_AI", "TargetChangeCustomID=123:int,DestroyScriptCustomID=124:int");

/******************************************************************************************************/

void UP_Private_Color_Message_On_Poke::Created(GameObject* obj)
{
	Pokable = true;
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
}

void UP_Private_Color_Message_On_Poke::Timer_Expired(GameObject* obj, int number)
{
	if (number == 1)
	{
		Pokable = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}

void UP_Private_Color_Message_On_Poke::Poked(GameObject* obj, GameObject* poker)
{
	if (Pokable)
	{
		Send_Message_Player(poker, Get_Int_Parameter("Red"), Get_Int_Parameter("Green"), Get_Int_Parameter("Blue"), Get_Parameter("Message"));
		Pokable = false;
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
		Commands->Start_Timer(obj, this, 5.0f, 1);
	}
}

ScriptRegistrant<UP_Private_Color_Message_On_Poke> UP_Private_Color_Message_On_Poke_Registrant("UP_Private_Color_Message_On_Poke", "Red=255:int,Green=255:int,Blue=255:int,Message=Message:string");

/******************************************************************************************************/

void UP_Seize_Weapon_Enter::Entered(GameObject* obj, GameObject* enterer)
{
	const char* weaponName = Get_Parameter("Weapon");
	if (Has_Weapon(enterer, weaponName))
	{
		int clipAmmo = Get_Clip_Bullets(enterer, weaponName);
		int invAmmo = Get_Bullets(enterer, weaponName);
		Attach_Script_V(enterer, "UP_Grant_Weapon_Custom_Then_Detach", "%s,%d,%d,0,%d", weaponName, clipAmmo, invAmmo, Get_Int_Parameter("CustomID"));
		Remove_Weapon(enterer, weaponName);
	}
}

void UP_Seize_Weapon_Enter::Exited(GameObject* obj, GameObject* exiter)
{
	Commands->Send_Custom_Event(obj, exiter, Get_Int_Parameter("CustomID"), 0, .0f);
}

ScriptRegistrant<UP_Seize_Weapon_Enter> UP_Seize_Weapon_Enter_Registrant("UP_Seize_Weapon_Enter", "Weapon:string,CustomID:int");

/******************************************************************************************************/

void UP_Seize_Weapon_Exit::Entered(GameObject* obj, GameObject* enterer)
{
	Commands->Send_Custom_Event(obj, enterer, Get_Int_Parameter("CustomID"), 0, .0f);
}

void UP_Seize_Weapon_Exit::Exited(GameObject* obj, GameObject* exiter)
{
	const char* weaponName = Get_Parameter("Weapon");
	if (Has_Weapon(exiter, weaponName))
	{
		int clipAmmo = Get_Clip_Bullets(exiter, weaponName);
		int invAmmo = Get_Bullets(exiter, weaponName);
		Attach_Script_V(exiter, "UP_Grant_Weapon_Custom_Then_Detach", "%s,%d,%d,0,%d", weaponName, clipAmmo, invAmmo, Get_Int_Parameter("CustomID"));
		Remove_Weapon(exiter, weaponName);
	}
}

ScriptRegistrant<UP_Seize_Weapon_Exit> UP_Seize_Weapon_Exit_Registrant("UP_Seize_Weapon_Exit", "Weapon:string,CustomID:int");

/******************************************************************************************************/

void UP_Seize_All_Weapons_Enter::Created(GameObject* obj)
{
	char* weaponList = newstr(Get_Parameter("WeaponsExcept"));
	char* curPos = weaponList;
	char* lastPos = weaponList;
	
	for(;;)
	{
		if(curPos[0] == '|')
		{
			curPos[0] = '\0';
			Weapons.Add(lastPos);
			curPos[0] = '|';

			curPos++;
			lastPos = curPos;
		}
		else if (curPos[0] == '\0')
		{
			if (curPos > lastPos)
			{
				Weapons.Add(lastPos);
			}
			break;
		}

		curPos++;
	}

	delete[] weaponList;
}

void UP_Seize_All_Weapons_Enter::Entered(GameObject* obj, GameObject* enterer)
{
	for (int i = 1; i < Get_Weapon_Count(enterer); i++)
	{
		if (const char* weaponName = Get_Weapon(enterer, i))
		{
			if (!strcmp(weaponName, "None"))
				continue;

			if (Weapons.ID(weaponName) == -1)
			{
				int clipAmmo = Get_Clip_Bullets(enterer, weaponName);
				int invAmmo = Get_Bullets(enterer, weaponName);
				Attach_Script_V(enterer, "UP_Grant_Weapon_Custom_Then_Detach", "%s,%d,%d,0,%d", weaponName, clipAmmo, invAmmo, Get_Int_Parameter("CustomID"));
				Remove_Weapon(enterer, weaponName);
				i--;
			}
		}
	}
}

void UP_Seize_All_Weapons_Enter::Exited(GameObject* obj, GameObject* exiter)
{
	Commands->Send_Custom_Event(obj, exiter, Get_Int_Parameter("CustomID"), 0, .0f);
}

ScriptRegistrant<UP_Seize_All_Weapons_Enter> UP_Seize_All_Weapons_Enter_Registrant("UP_Seize_All_Weapons_Enter", "WeaponsExcept:string,CustomID:int");

/******************************************************************************************************/

void UP_Seize_All_Weapons_Exit::Created(GameObject* obj)
{
	char* weaponList = newstr(Get_Parameter("WeaponsExcept"));
	char* curPos = weaponList;
	char* lastPos = weaponList;

	for (;;)
	{
		if (curPos[0] == '|')
		{
			curPos[0] = '\0';
			Weapons.Add(lastPos);
			curPos[0] = '|';

			curPos++;
			lastPos = curPos;
			continue;
		}
		else if (curPos[0] == '\0')
		{
			if (curPos > lastPos)
			{
				Weapons.Add(lastPos);
			}
			break;
		}

		curPos++;
	}

	delete[] weaponList;
}

void UP_Seize_All_Weapons_Exit::Entered(GameObject* obj, GameObject* enterer)
{
	Commands->Send_Custom_Event(obj, enterer, Get_Int_Parameter("CustomID"), 0, .0f);
}

void UP_Seize_All_Weapons_Exit::Exited(GameObject* obj, GameObject* exiter)
{
	for (int i = 1; i < Get_Weapon_Count(exiter); i++)
	{
		if (const char* weaponName = Get_Weapon(exiter, i))
		{
			if (!strcmp(weaponName, "None"))
				continue;

			if (Weapons.ID(weaponName) == -1)
			{
				int clipAmmo = Get_Clip_Bullets(exiter, weaponName);
				int invAmmo = Get_Bullets(exiter, weaponName);
				Attach_Script_V(exiter, "UP_Grant_Weapon_Custom_Then_Detach", "%s,%d,%d,0,%d", weaponName, clipAmmo, invAmmo, Get_Int_Parameter("CustomID"));
				Remove_Weapon(exiter, weaponName);
				i--;
			}
		}
	}
}

ScriptRegistrant<UP_Seize_All_Weapons_Exit> UP_Seize_All_Weapons_Exit_Registrant("UP_Seize_All_Weapons_Exit", "WeaponsExcept:string,CustomID:int");

/******************************************************************************************************/

void UP_Grant_Weapon_Custom_Then_Detach::Custom(GameObject* obj, int type, int param, GameObject* sender)
{
	if (type == Get_Int_Parameter("CustomID"))
	{
		Attach_Script_V(obj, "UP_Grant_Weapon", "%s,%d,%d,%d", Get_Parameter("Preset"), Get_Int_Parameter("ClipRounds"), Get_Int_Parameter("InventoryRounds"), Get_Int_Parameter("AutoSelect"));
		Destroy_Script();
	}
}

ScriptRegistrant<UP_Grant_Weapon_Custom_Then_Detach> UP_Grant_Weapon_Custom_Then_Detach_Registrant("UP_Grant_Weapon_Custom_Then_Detach", "Preset=Weapon:string,ClipRounds=1:int,InventoryRounds=1:int,AutoSelect=1:int,CustomID=10123:int");

/******************************************************************************************************/

void UP_Disable_Weapon_For_Driver::Created(GameObject* obj)
{
	if (!obj->As_VehicleGameObj())
		Destroy_Script();

	if (Get_Vehicle_Seat_Count(obj) < 2)
		Destroy_Script();

	HasWeapon = true;
	VehicleGameObj* Vehicle = obj->As_VehicleGameObj();
	if (!Vehicle->Get_Occupant(1))
	{
		Weapon = Get_Current_Weapon(Vehicle);
		ClipAmmo = Get_Current_Clip_Bullets(Vehicle);
		InventoryAmmo = Get_Current_Bullets(Vehicle);
		Remove_Weapon(Vehicle, Weapon);
		HasWeapon = false;
	}
}

void UP_Disable_Weapon_For_Driver::Custom(GameObject* obj, int type, int param, GameObject* sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		VehicleGameObj* Vehicle = obj->As_VehicleGameObj();
		if (Vehicle->Get_Occupant(1) && !HasWeapon)
		{
			Grant_Weapon(Vehicle, Weapon, true, 0, true);
			Set_Clip_Bullets(Vehicle, Weapon, ClipAmmo);
			Set_Bullets(Vehicle, Weapon, InventoryAmmo);
			Commands->Select_Weapon(Vehicle, Weapon);

			Weapon = "";
			ClipAmmo = 0;
			InventoryAmmo = 0;
			HasWeapon = true;
		}
	}
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		VehicleGameObj* Vehicle = obj->As_VehicleGameObj();
		if (!Vehicle->Get_Occupant(1) && HasWeapon)
		{
			Weapon = Get_Current_Weapon(Vehicle);
			ClipAmmo = Get_Current_Clip_Bullets(Vehicle);
			InventoryAmmo = Get_Current_Bullets(Vehicle);
			Remove_Weapon(Vehicle, Weapon);
			HasWeapon = false;
		}
	}
}

ScriptRegistrant<UP_Disable_Weapon_For_Driver> UP_Disable_Weapon_For_Driver_Registrant("UP_Disable_Weapon_For_Driver", "");

/******************************************************************************************************/

void UP_Set_Bot_Tag::Created(GameObject* obj)
{
	if (obj->As_SoldierGameObj())
	{
		obj->As_SoldierGameObj()->Set_Bot_Tag(Get_Parameter("Tag"));
	}

	Destroy_Script();
}

ScriptRegistrant<UP_Set_Bot_Tag> UP_Set_Bot_Tag_Registrant("UP_Set_Bot_Tag", "Tag:string");

/******************************************************************************************************/

void UP_Set_Bot_Tag_MultiLine::Created(GameObject* obj)
{
	if (obj->As_SoldierGameObj())
	{
		StringClass tag;
		tag.Format("%s\n%s\n%s\n%s\n%s", Get_Parameter("Tag1"), Get_Parameter("Tag2"), Get_Parameter("Tag3"), Get_Parameter("Tag4"), Get_Parameter("Tag5"));
		obj->As_SoldierGameObj()->Set_Bot_Tag(tag);
	}

	Destroy_Script();
}

ScriptRegistrant<UP_Set_Bot_Tag_MultiLine> UP_Set_Bot_Tag_MultiLine_Registrant("UP_Set_Bot_Tag_MultiLine", "Tag1:string,Tag2:string,Tag3:string,Tag4:string,Tag5:string");

/******************************************************************************************************/

void UP_Remove_Bot_Tag::Created(GameObject* obj)
{
	if (obj->As_SoldierGameObj())
	{
		obj->As_SoldierGameObj()->Set_Bot_Tag("\0");
	}

	Destroy_Script();
}

ScriptRegistrant<UP_Remove_Bot_Tag> UP_Remove_Bot_Tag_Registrant("UP_Remove_Bot_Tag", "");

/******************************************************************************************************/

void UP_Stop_Sound::Created(GameObject* obj)
{
	Commands->Stop_Sound(Get_Int_Parameter("SoundID"), Get_Int_Parameter("Destroy") != 0);
	Destroy_Script();
}

ScriptRegistrant<UP_Stop_Sound> UP_Stop_Sound_Registrant("UP_Stop_Sound", "SoundID:int,Destroy:int");

/******************************************************************************************************/

void UP_Sound_Controller_2D::Created(GameObject* obj)
{
	StartCustomType = Get_Int_Parameter("StartCustomType");
	StopCustomType = Get_Int_Parameter("StopCustomType");
	SoundDef = Get_Parameter("SoundDef");

	SoundID = Commands->Create_2D_Sound(SoundDef);
	if (!SoundID)
	{
		Destroy_Script();
	}
}

void UP_Sound_Controller_2D::Destroyed(GameObject* obj)
{
	Commands->Stop_Sound(SoundID, true);
}

void UP_Sound_Controller_2D::Custom(GameObject* obj, int type, int param, GameObject* sender)
{
	if (type == StartCustomType)
	{
		if (!SoundID)
		{
			SoundID = Commands->Create_2D_Sound(SoundDef);
		}
	}
	else if (type == StopCustomType)
	{
		Commands->Stop_Sound(SoundID, true);
		SoundID = 0;
	}
}

ScriptRegistrant<UP_Sound_Controller_2D> UP_Sound_Controller_2D_Registrant("UP_Sound_Controller_2D", "SoundDef:string,StartCustomType=12300:int,StopCustomType=12301:int");

/******************************************************************************************************/

void UP_Sound_Controller_3D::Created(GameObject* obj)
{
	StartCustomType = Get_Int_Parameter("StartCustomType");
	StopCustomType = Get_Int_Parameter("StopCustomType");
	SoundDef = Get_Parameter("SoundDef");

	SoundID = Commands->Create_Sound(SoundDef, Commands->Get_Position(obj), obj);
	if (!SoundID)
	{
		Destroy_Script();
	}
}

void UP_Sound_Controller_3D::Destroyed(GameObject* obj)
{
	Commands->Stop_Sound(SoundID, true);
}

void UP_Sound_Controller_3D::Custom(GameObject* obj, int type, int param, GameObject* sender)
{
	if (type == StartCustomType)
	{
		if (!SoundID)
		{
			SoundID = Commands->Create_Sound(SoundDef, Commands->Get_Position(obj), obj);
		}
	}
	else if (type == StopCustomType)
	{
		Commands->Stop_Sound(SoundID, true);
		SoundID = 0;
	}
}

ScriptRegistrant<UP_Sound_Controller_3D> UP_Sound_Controller_3D_Registrant("UP_Sound_Controller_3D", "SoundDef:string,StartCustomType=12300:int,StopCustomType=12301:int");

/******************************************************************************************************/

void UP_Teleport_Killer_On_Death::Created(GameObject* obj)
{
	TeleportObj = Get_Int_Parameter("TeleportObj");
}

void UP_Teleport_Killer_On_Death::Killed(GameObject* obj, GameObject* killer)
{
	if (!killer) return;

	if (GameObject* target = Commands->Find_Object(TeleportObj))
	{
		if (!Find_Script_On_Object(target, "UP_Teleport_Killer_Immunity"))
		{
			Commands->Set_Position(killer, Commands->Get_Position(target));
		}
	}
}

ScriptRegistrant<UP_Teleport_Killer_On_Death> UP_Teleport_Killer_On_Death_Registrant("UP_Teleport_Killer_On_Death", "TeleportObj:int");

/******************************************************************************************************/

void UP_Teleport_Killer_Immunity::Created(GameObject* obj)
{
	Expire = Get_Float_Parameter("Expire");

	if (Expire > WWMATH_EPSILON)
	{
		Commands->Start_Timer(obj, this, Expire, 123);
	}
}

void UP_Teleport_Killer_Immunity::Timer_Expired(GameObject* obj, int number)
{
	if (number == 123)
	{
		Destroy_Script();
	}
}

ScriptRegistrant<UP_Teleport_Killer_Immunity> UP_Teleport_Killer_Immunity_Registrant("UP_Teleport_Killer_Immunity", "Expire=0:float");

/******************************************************************************************************/

void UP_Teleport_On_Zone_Kill::Created(GameObject* obj)
{
	TeleportObj = Get_Int_Parameter("TeleportObj");
}

void UP_Teleport_On_Zone_Kill::Entered(GameObject* obj, GameObject* enterer)
{
	Attach_Script_V(enterer, "UP_Teleport_On_Death", "%d", TeleportObj);
}

void UP_Teleport_On_Zone_Kill::Exited(GameObject* obj, GameObject* exiter)
{
	if (ScriptImpClass* Script = Find_Script_On_Object(exiter, "UP_Teleport_On_Death"))
	{
		Script->Destroy_Script();
	}
}

ScriptRegistrant<UP_Teleport_On_Zone_Kill> UP_Teleport_On_Zone_Kill_Registrant("UP_Teleport_On_Zone_Kill", "TeleportObj:int");

/******************************************************************************************************/

void UP_Teleport_Killer_Immunity_Zone::Created(GameObject* obj)
{
	Expire = Get_Float_Parameter("Expire");
}

void UP_Teleport_Killer_Immunity_Zone::Entered(GameObject* obj, GameObject* enterer)
{
	Attach_Script_V(enterer, "UP_Teleport_Killer_Immunity", "%f", Expire);
}

void UP_Teleport_Killer_Immunity_Zone::Exited(GameObject* obj, GameObject* exiter)
{
	if (ScriptImpClass* Script = Find_Script_On_Object(exiter, "UP_Teleport_Killer_Immunity"))
	{
		Script->Destroy_Script();
	}
}

ScriptRegistrant<UP_Teleport_Killer_Immunity_Zone> UP_Teleport_Killer_Immunity_Zone_Registrant("UP_Teleport_Killer_Immunity_Zone", "Expire=0:float");

/******************************************************************************************************/

void UP_Gravity_Zone::Created(GameObject* obj)
{
	Multiplier = Get_Float_Parameter("Multiplier");
}

void UP_Gravity_Zone::Entered(GameObject* obj, GameObject* enterer)
{
	Set_Gravity_Multiplier(enterer, Multiplier);
}

void UP_Gravity_Zone::Exited(GameObject* obj, GameObject* exiter)
{
	Set_Gravity_Multiplier(exiter, 1.0f);
}

ScriptRegistrant<UP_Gravity_Zone> UP_Gravity_Zone_Registrant("UP_Gravity_Zone", "Multiplier=0.5:float");

/******************************************************************************************************/

void UP_No_Targeting_Zone::Entered(GameObject* obj, GameObject* enterer)
{
	Enable_Global_Targeting_Player(enterer, false);
}

void UP_No_Targeting_Zone::Exited(GameObject* obj, GameObject* exiter)
{
	Enable_Global_Targeting_Player(exiter, true);
}

ScriptRegistrant<UP_No_Targeting_Zone> UP_No_Targeting_Zone_Registrant("UP_No_Targeting_Zone", "");

/******************************************************************************************************/

void UP_Send_Message_Self_On_Custom::Created(GameObject* obj)
{
	if (!Commands->Is_A_Star(obj))
	{
		Destroy_Script();
	}
}

void UP_Send_Message_Self_On_Custom::Custom(GameObject* obj, int type, int param, GameObject* sender)
{
	if (type == Get_Int_Parameter("ID") && param == Get_Int_Parameter("Param"))
	{
		Send_Message_Player(obj, Get_Int_Parameter("Red"), Get_Int_Parameter("Green"), Get_Int_Parameter("Blue"), Get_Parameter("Message"));

		if (Get_Int_Parameter("DestroyAfterSend"))
		{
			Destroy_Script();
		}
	}
}

ScriptRegistrant<UP_Send_Message_Self_On_Custom> UP_Send_Message_Self_On_Custom_Registrant("UP_Send_Message_Self_On_Custom", "ID:int,Param:int,Red=255:int,Green=255:int,Blue=255:int,Message:string,DestroyAfterSend=0:int");

/******************************************************************************************************/

void UP_Send_Message_Sender_On_Custom::Custom(GameObject* obj, int type, int param, GameObject* sender)
{
	if (type == Get_Int_Parameter("ID") && param == Get_Int_Parameter("Param"))
	{
		if (sender && Commands->Is_A_Star(sender))
		{
			Send_Message_Player(sender, Get_Int_Parameter("Red"), Get_Int_Parameter("Green"), Get_Int_Parameter("Blue"), Get_Parameter("Message"));

			if (Get_Int_Parameter("DestroyAfterSend"))
			{
				Destroy_Script();
			}
		}
	}
}

ScriptRegistrant<UP_Send_Message_Sender_On_Custom> UP_Send_Message_Sender_On_Custom_Registrant("UP_Send_Message_Sender_On_Custom", "ID:int,Param:int,Red=255:int,Green=255:int,Blue=255:int,Message:string,DestroyAfterSend=0:int");

/******************************************************************************************************/

void UP_Print_Console_Self_On_Custom::Created(GameObject* obj)
{
	if (!Commands->Is_A_Star(obj))
	{
		Destroy_Script();
	}
}

void UP_Print_Console_Self_On_Custom::Custom(GameObject* obj, int type, int param, GameObject* sender)
{
	if (type == Get_Int_Parameter("ID") && param == Get_Int_Parameter("Param"))
	{
		Print_Client_Console_Player(obj, Get_Parameter("Message"), Vector4(Get_Int_Parameter("Alpha") / 255.f, Get_Int_Parameter("Red") / 255.f, Get_Int_Parameter("Green") / 255.f, Get_Int_Parameter("Blue") / 255.f));

		if (Get_Int_Parameter("DestroyAfterSend"))
		{
			Destroy_Script();
		}
	}
}

ScriptRegistrant<UP_Print_Console_Self_On_Custom> UP_Print_Console_Self_On_Custom_Registrant("UP_Print_Console_Self_On_Custom", "ID:int,Param:int,Alpha=255:int,Red=255:int,Green=255:int,Blue=255:int,Message:string,DestroyAfterSend=0:int");

/******************************************************************************************************/

void UP_Print_Console_Sender_On_Custom::Custom(GameObject* obj, int type, int param, GameObject* sender)
{
	if (type == Get_Int_Parameter("ID") && param == Get_Int_Parameter("Param"))
	{
		if (sender && Commands->Is_A_Star(sender))
		{
			Print_Client_Console_Player(sender, Get_Parameter("Message"), Vector4(Get_Int_Parameter("Alpha") / 255.f, Get_Int_Parameter("Red") / 255.f, Get_Int_Parameter("Green") / 255.f, Get_Int_Parameter("Blue") / 255.f));

			if (Get_Int_Parameter("DestroyAfterSend"))
			{
				Destroy_Script();
			}
		}
	}
}

ScriptRegistrant<UP_Print_Console_Sender_On_Custom> UP_Print_Console_Sender_On_Custom_Registrant("UP_Print_Console_Sender_On_Custom", "ID:int,Param:int,Alpha=255:int,Red=255:int,Green=255:int,Blue=255:int,Message:string,DestroyAfterSend=0:int");

/******************************************************************************************************/

void UP_Add_Chat_History_Self_On_Custom::Created(GameObject* obj)
{
	if (!Commands->Is_A_Star(obj))
	{
		Destroy_Script();
	}
}

void UP_Add_Chat_History_Self_On_Custom::Custom(GameObject* obj, int type, int param, GameObject* sender)
{
	if (type == Get_Int_Parameter("ID") && param == Get_Int_Parameter("Param"))
	{
		Add_Shadow_Message(Get_Player_ID(obj), Vector3(Get_Int_Parameter("Red") / 255.f, Get_Int_Parameter("Green") / 255.f, Get_Int_Parameter("Blue") / 255.f), WideStringClass(Get_Parameter("Message")));

		if (Get_Int_Parameter("DestroyAfterSend"))
		{
			Destroy_Script();
		}
	}
}

ScriptRegistrant<UP_Add_Chat_History_Self_On_Custom> UP_Add_Chat_History_Self_On_Custom_Registrant("UP_Add_Chat_History_Self_On_Custom", "ID:int,Param:int,Red=255:int,Green=255:int,Blue=255:int,Message:string,DestroyAfterSend=0:int");

/******************************************************************************************************/

void UP_Add_Chat_History_Sender_On_Custom::Custom(GameObject* obj, int type, int param, GameObject* sender)
{
	if (type == Get_Int_Parameter("ID") && param == Get_Int_Parameter("Param"))
	{
		if (sender && Commands->Is_A_Star(sender))
		{
			Add_Shadow_Message(Get_Player_ID(sender), Vector3(Get_Int_Parameter("Red") / 255.f, Get_Int_Parameter("Green") / 255.f, Get_Int_Parameter("Blue") / 255.f), WideStringClass(Get_Parameter("Message")));

			if (Get_Int_Parameter("DestroyAfterSend"))
			{
				Destroy_Script();
			}
		}
	}
}

ScriptRegistrant<UP_Add_Chat_History_Sender_On_Custom> UP_Add_Chat_History_Sender_On_Custom_Registrant("UP_Add_Chat_History_Sender_On_Custom", "ID:int,Param:int,Red=255:int,Green=255:int,Blue=255:int,Message:string,DestroyAfterSend=0:int");

/******************************************************************************************************/

void UP_Grant_Credits_Self_On_Custom::Created(GameObject* obj)
{
	if (!Commands->Is_A_Star(obj))
	{
		Destroy_Script();
	}
}

void UP_Grant_Credits_Self_On_Custom::Custom(GameObject* obj, int type, int param, GameObject* sender)
{
	if (type == Get_Int_Parameter("ID"))
	{
		Commands->Give_Money(obj, (float)param, false);

		if (Get_Int_Parameter("DestroyAfterSend"))
		{
			Destroy_Script();
		}
	}
}

ScriptRegistrant<UP_Grant_Credits_Self_On_Custom> UP_Grant_Credits_Self_On_Custom_Registrant("UP_Grant_Credits_Self_On_Custom", "ID:int,DestroyAfterSend=0:int");

/******************************************************************************************************/

void UP_Grant_Credits_Sender_On_Custom::Custom(GameObject* obj, int type, int param, GameObject* sender)
{
	if (type == Get_Int_Parameter("ID"))
	{
		if (sender && Commands->Is_A_Star(sender))
		{
			Commands->Give_Money(sender, (float)param, false);

			if (Get_Int_Parameter("DestroyAfterSend"))
			{
				Destroy_Script();
			}
		}
	}
}

ScriptRegistrant<UP_Grant_Credits_Sender_On_Custom> UP_Grant_Credits_Sender_On_Custom_Registrant("UP_Grant_Credits_Sender_On_Custom", "ID:int,DestroyAfterSend=0:int");

/******************************************************************************************************/

void UP_Grant_Credits_Team_On_Custom::Custom(GameObject* obj, int type, int param, GameObject* sender)
{
	if (type == Get_Int_Parameter("ID"))
	{
		if (sender && Commands->Is_A_Star(sender))
		{
			int team = Get_Int_Parameter("Team");

			for(SLNode<cPlayer>* n = Get_Player_List()->Head(); n; n = n->Next())
			{
				if (n->Data()->Get_Player_Type() == team)
				{
					n->Data()->Increment_Money((float)param);
				}
			}

			if (Get_Int_Parameter("DestroyAfterSend"))
			{
				Destroy_Script();
			}
		}
	}
}

ScriptRegistrant<UP_Grant_Credits_Team_On_Custom> UP_Grant_Credits_Team_On_Custom_Registrant("UP_Grant_Credits_Sender_On_Custom", "Team:int,ID:int,DestroyAfterSend=0:int");

/******************************************************************************************************/

void UP_Steal_Credits_On_Custom::Custom(GameObject* obj, int type, int param, GameObject* sender)
{
	if (type == Get_Int_Parameter("ID"))
	{
		int team = Get_Int_Parameter("Team");
		switch (team)
		{
			case -1:
				if (sender) team = Get_Object_Type(sender);
				break;

			case -2:
				team = Get_Object_Type(obj);
				break;

			case -3:
				if (sender) team = Get_Object_Type(sender) ? 0 : 1;
				break;

			case -4:
				team = Get_Object_Type(obj) ? 0 : 1;
				break;
		}

		float percentage = Get_Float_Parameter("Percentage");
		if (percentage < 0)
		{
			percentage = param / 100.f;
		}

		int otherTeam = team ? 0 : 1;
		float stolenCredits = Steal_Team_Credits(percentage, team);
		float distributedCredits = stolenCredits / Get_Team_Player_Count(otherTeam);

		for (SLNode<cPlayer>* n = Get_Player_List()->Head(); n; n = n->Next())
		{
			if (n->Data()->Get_Player_Type() == otherTeam)
			{
				n->Data()->Increment_Money(distributedCredits);
			}
		}

		if (Get_Int_Parameter("DestroyAfterSteal"))
		{
			Destroy_Script();
		}
	}
}

ScriptRegistrant<UP_Steal_Credits_On_Custom> UP_Steal_Credits_On_Custom_Registrant("UP_Steal_Credits_On_Custom", "Team=-2:int,Percentage:int,ID:int,DestroyAfterSteal=0:int");

/******************************************************************************************************/

bool UP_No_Refill::RefillHookInstalled = false;
int UP_No_Refill::AliveScriptCount = 0;

void UP_No_Refill::Created(GameObject* obj)
{
	if (!RefillHookInstalled)
	{
		AddRefillHook(Refill_Hook);
		RefillHookInstalled = true;
	}
	AliveScriptCount++;
}

void UP_No_Refill::Destroyed(GameObject* obj)
{
	AliveScriptCount--;
}

bool UP_No_Refill::Refill_Hook(GameObject* purchaser)
{
	return !AliveScriptCount;
}

ScriptRegistrant<UP_No_Refill> UP_No_Refill_Registrant("UP_No_Refill", "");

/******************************************************************************************************/