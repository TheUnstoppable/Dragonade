//MasterCan's Scripts for Public TT Release - v3
//Made by The Unstoppable (MasterCan)
//This version includes 40 scripts.

//39 Working
//1 Buggy
//0 Not Working
//0 Not Tested

#include "General.h"
#include "engine.h"
#include "engine_common.h"
#include "engine_def.h"
#include "engine_game.h"
#include "engine_player.h"
#include "engine_tt.h"
#include "engine_script.h"
#include "engine_player.h"
#include "engine_io.h"
#include "engine_obj.h"
#include "WeaponClass.h"
#include "SoldierGameObj.h"
#include "WeaponBagClass.h"
#include "VehicleGameObj.h"
#include "SimpleGameObj.h"
#include "BuildingGameObj.h"
#include "PowerupGameObj.h"
#include "cPlayer.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

class MC_Teleport : public ScriptImpClass
{
	void Entered(GameObject *obj, GameObject *enterer)
	{
		//BxoID: Script Zone ID
		//ArrowID: Daves Arrow (to teleport) ID
		//TeleportSound: The sound that will be played after teleportation done.
		//Changelog: 1.0 - First Release
		//           1.1 - Gonna Make This Two-Way Teleportation
		if (enterer->As_SoldierGameObj() || enterer->As_VehicleGameObj())
		{
			Commands->Set_Position(enterer, Commands->Get_Position(Commands->Find_Object(Get_Int_Parameter("ArrowID"))));
			if (Get_Parameter("TeleportSound"))
			{
				Create_2D_WAV_Sound_Player(enterer, Get_Parameter("TeleportSound"));
			}
		}
	}
};


class MC_HUD_Message_Zone : public ScriptImpClass
{
	void Entered(GameObject *obj, GameObject *enterer)
	{
		//BoxID: Script Zone ID
		//Message: The message that will be shown on HUD.
		//Sound: The sound that will be played.
		//Changelog: 1.0 - First Release
		//           1.1 - No plans right now.

		if (Commands->Find_Object(Get_Int_Parameter("BoxID")))
		{
			if (enterer->As_SoldierGameObj())
			{
				SoldierGameObj *Soldier = enterer->As_SoldierGameObj();
				if (Get_Parameter("Message"))
				{
					Set_HUD_Help_Text_Player_Text(Soldier, 8269, Get_Parameter("Message"), Vector3(Get_Float_Parameter("Red"), Get_Float_Parameter("Green"), Get_Float_Parameter("Blue")));
				}

				if (Get_Parameter("Sound"))
				{
					Create_2D_WAV_Sound_Player(Soldier, Get_Parameter("Sound"));
				}
			}
		}
	}
};

class MC_Stop_Vehicle_Engine : public ScriptImpClass
{
	GameObject *Vehicle;
	bool Disabled;
	void Created(GameObject *obj)
	{
		//Duration: The amount of time
		//Changelog: 1.0 - First release
		//           1.1 - Planning to spawn a ciematic (like how EMP has) when script activated.
		Vehicle = obj;
		if (Vehicle->As_VehicleGameObj())
		{
			Disabled = true;
			Commands->Start_Timer(Vehicle, this, Get_Float_Parameter("Duration"), 1);
			Commands->Enable_Engine(Vehicle, false);
			Vehicle->As_VehicleGameObj()->Set_Immovable(true);
		}
		else
		{
			Destroy_Script();
		}
	}

	void Custom(GameObject *obj, int type, int param, GameObject *sender)
	{
		if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
		{
			if (Disabled)
			{
				Commands->Enable_Engine(Vehicle, false);
				Vehicle->As_VehicleGameObj()->Set_Immovable(true);
			}
		}
	}

	void Timer_Expired(GameObject *obj, int number)
	{
		Disabled = false;
		Commands->Enable_Engine(obj, true);
		Vehicle->As_VehicleGameObj()->Set_Immovable(false);
		Destroy_Script();
	}
};

class MC_Change_Vehicle_Weapon : public ScriptImpClass
{
	void Created(GameObject *obj)
	{
		//Preset: The preset which will be changed.
		//Rounds: The amount of ammo which will be granted.
		if (obj->As_VehicleGameObj())
		{
			Commands->Clear_Weapons(obj);
			Grant_Weapon(obj, Get_Parameter("Preset"), true, Get_Int_Parameter("Rounds"), false);
			Commands->Select_Weapon(obj, Get_Parameter("Preset"));
		}

		Destroy_Script();
	}
};

class MC_MCTMainSyncObject : public ScriptImpClass
{
	GameObject *MCTObj;
	void Created(GameObject *obj)
	{
		MCTObj = Commands->Find_Object(Get_Int_Parameter("MCT_ObjectID"));

		float MaxS = Commands->Get_Max_Shield_Strength(obj);
		float MaxH = Commands->Get_Max_Health(obj);
		Set_Max_Shield_Strength(MCTObj, MaxS);
		Set_Max_Health(MCTObj, MaxH);
	}

	void Damaged(GameObject *obj, GameObject *damager, float amount)
	{
		Set_Max_Shield_Strength(MCTObj, Commands->Get_Max_Shield_Strength(obj));
		Set_Max_Health(MCTObj, Commands->Get_Max_Health(obj));
		Commands->Apply_Damage(MCTObj, amount, "None", 0);
	}
};

class MC_MasterControlTerminal_Object : public ScriptImpClass
{
	GameObject *MainStructure;
	float Multiplier;
	bool SameHealth;
	DefenseObjectClass *MCT;
	DefenseObjectClass *Main;
	//Object: The object which will be main structure.
	//Multiply: The multiplier of amount which will be applied.
	//SameHealthWithMain: Makes MCT health same with main object.
	void Created(GameObject *obj)
	{
		MainStructure = Commands->Find_Object(Get_Int_Parameter("Object"));
		Multiplier = Get_Float_Parameter("Multiply");
		Commands->Start_Timer(obj, this, 1.0f, 1);
	}

	void Damaged(GameObject *obj, GameObject *damager, float amount)
	{
		Commands->Apply_Damage(MainStructure, amount * Multiplier, "None", damager);
	}

	void Timer_Expired(GameObject *obj, int number)
	{
		if (number == 1)
		{
			Change_Team_2(obj, Commands->Get_Player_Type(MainStructure));
			Commands->Start_Timer(obj, this, 1.0f, 1);
		}
	}
};

class MC_Change_Vehicle_Weapon_Zone : public ScriptImpClass
{
	void Entered(GameObject *obj, GameObject *enterer)
	{
		//Preset: The preset which will be changed.
		//Rounds: The amount of ammo which will be granted.
		if (enterer->As_VehicleGameObj())
		{
			Commands->Clear_Weapons(enterer);
			Grant_Weapon(enterer, Get_Parameter("Preset"), true, Get_Int_Parameter("Rounds"), false);
			Commands->Select_Weapon(enterer, Get_Parameter("Preset"));
		}
	}
};

class MC_Grant_Weapon : public ScriptImpClass
{
	void Created(GameObject *obj)
	{
		//Preset: The preset which will be granted.
		//Rounds: The amount of ammo which will be granted.
		Grant_Weapon(obj, Get_Parameter("Preset"), true, Get_Int_Parameter("Rounds"), false);
		Destroy_Script();
	}
};

class MC_Grant_Weapon_Zone : public ScriptImpClass
{
	void Entered(GameObject *obj, GameObject *enterer)
	{
		//Preset: The preset which will be granted.
		//Rounds: The amount of ammo which will be granted.
		Grant_Weapon(enterer, Get_Parameter("Preset"), true, Get_Int_Parameter("Rounds"), false);
	}
};

class MC_Spawn_When_Death : public ScriptImpClass
{
	void Killed(GameObject *obj, GameObject *killer)
	{
		//Preset: The preset which will be spawned when object is killed.
		GameObject *Spawned = Commands->Create_Object(Get_Parameter("Preset"), Commands->Get_Position(obj));
		Commands->Set_Facing(Spawned, Commands->Get_Facing(obj));
		Destroy_Script();
	}
};


class MC_GameOver_When_Kill : public ScriptImpClass
{
	void Killed(GameObject *obj, GameObject *killer)
	{
		Console_Input("gameover");
		Destroy_Script();
	}
};

class MC_Grant_Character : public ScriptImpClass
{
	void Created(GameObject *obj)
	{
		//Preset: The character preset which will be granted.
		if (obj->As_SoldierGameObj())
		{
			Change_Character(obj, Get_Parameter("Preset"));
		}
		Destroy_Script();
	}
};

class MC_Grant_Character_Zone : public ScriptImpClass
{
	void Entered(GameObject *obj, GameObject *enterer)
	{
		//Preset: The character preset which will be granted.
		if (enterer->As_SoldierGameObj())
		{
			Change_Character(enterer, Get_Parameter("Preset"));
		}
	}
};

class MC_Spawn_When_Fully_Repaired : public ScriptImpClass
{
	void Damaged(GameObject *obj, GameObject *damager, float amount)
	{
		//Preset: The preset which will be spawned when the object is fully repaired.
		if (amount < 0)
		{
			float CurrentHealth = Commands->Get_Health(obj) + Commands->Get_Shield_Strength(obj);
			const float MaxHealth = Commands->Get_Max_Health(obj) + Commands->Get_Max_Shield_Strength(obj);
			if (CurrentHealth == MaxHealth)
			{
				Commands->Create_Object(Get_Parameter("Preset"), Commands->Get_Position(obj));
				Commands->Destroy_Object(obj);
				Destroy_Script();
			}
		}
	}

	void Killed(GameObject *obj, GameObject *damager)
	{
		Destroy_Script();
	}
};


class MC_Refinery_Object : public ScriptImpClass
{
	int Team;
	void Created(GameObject *obj)
	{
		//Interval: The interval of the grant credits.
		//Amount: The amount of credits will be granted.
		Team = Get_Int_Parameter("Team");
		Commands->Set_Player_Type(obj, Team);
		Commands->Start_Timer(obj, this, Get_Float_Parameter("Interval"), 5);
	}

	void Timer_Expired(GameObject *obj, int number)
	{
		for (SLNode<cPlayer> *z = Get_Player_List()->Head(); z; z = z->Next())
		{
			cPlayer *Player = z->Data();
			GameObject *PlayerObject = Player->Get_GameObj();
			if (Commands->Get_Player_Type(PlayerObject) == Team)
			{
				Commands->Give_Money(PlayerObject, Get_Float_Parameter("Amount"), false);
			}
		}
	}
};

class MC_Vehicle_Purchase_Terminal : public ScriptImpClass
{
	bool Pokeable;
	Vector3 SpawnLocation;
	float Facing;
	GameObject *Terminal;
	const char *Preset;
	int Amount;
	void Created(GameObject *obj)
	{
		//Team: The team of the PT.
		//VehiclePreset: The tank preset.
		//SpawnerID: Location that the object will be spawned.
		//Credits: The amount of credits that needed to purchase this object.
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
		Preset = Get_Parameter("VehiclePreset");
		SpawnLocation = Commands->Get_Position(Commands->Find_Object(Get_Int_Parameter("SpawnerID")));
		Facing = Commands->Get_Facing(Commands->Find_Object(Get_Int_Parameter("SpawnerID")));
		Terminal = obj;
		Pokeable = true;
		Amount = Get_Int_Parameter("Credits");
	}

	void Poked(GameObject *obj, GameObject *poker)
	{
		if (Pokeable)
		{
			if (poker->As_SoldierGameObj())
			{
				SoldierGameObj *Player = poker->As_SoldierGameObj();
				if (Commands->Get_Player_Type(Player) == Get_Int_Parameter("Team"))
				{
					if (Commands->Get_Money(Player) >= Amount)
					{
						GameObject *Vehicle = Commands->Create_Object(Preset, SpawnLocation);
						Commands->Set_Facing(Vehicle, Facing);
						Send_Message_Player(Player, 255, 255, 255, "Purchase request granted!");
						Create_2D_WAV_Sound_Player(Player, "M00GBMG_SECX0002I1GBMG_snd.wav");
						Commands->Give_Money(Player, Amount * -1.0f, false);
						Pokeable = false;
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
					Send_Message_Player(Player, 255, 255, 255, StringClass::getFormattedString("Access Denied! You aren't in team %s", Get_Team_Name(Get_Int_Parameter("Team"))));
				}
			}
		}
	}

	void Timer_Expired(GameObject *obj, int number)
	{
		if (number == 1)
		{
			Pokeable = true;
			Commands->Enable_HUD_Pokable_Indicator(obj, true);
		}
	}
};

class MC_Character_Purchase_Terminal : public ScriptImpClass
{
	bool Pokeable;
	GameObject *Terminal;
	const char *Preset;
	int Amount;
	void Created(GameObject *obj)
	{
		//Team: The team of the PT.
		//VehiclePreset: The tank preset.
		//SpawnerID: Location that the object will be spawned.
		//Credits: The amount of credits that needed to purchase this object.
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
		Preset = Get_Parameter("CharacterPreset");
		Terminal = obj;
		Pokeable = true;
		Amount = Get_Int_Parameter("Credits");
	}

	void Poked(GameObject *obj, GameObject *poker)
	{
		if (Pokeable)
		{
			if (poker->As_SoldierGameObj())
			{
				SoldierGameObj *Player = poker->As_SoldierGameObj();
				if (Commands->Get_Player_Type(Player) == Get_Int_Parameter("Team"))
				{
					if (Commands->Get_Money(poker) >= Amount)
					{
						Change_Character(poker, Preset);
						Send_Message_Player(Player, 255, 255, 255, "Purchase request granted!");
						Create_2D_WAV_Sound_Player(Player, "M00GBMG_SECX0002I1GBMG_snd.wav");
						Commands->Give_Money(Player, Amount * -1.0f, false);
						Pokeable = false;
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
					Send_Message_Player(Player, 255, 255, 255, StringClass::getFormattedString("Access Denied! You aren't in team %s", Get_Team_Name(Get_Int_Parameter("Team"))));
				}
			}
		}
	}

	void Timer_Expired(GameObject *obj, int number)
	{
		if (number == 1)
		{
			Pokeable = true;
			Commands->Enable_HUD_Pokable_Indicator(obj, true);
		}
	}
};

class MC_Set_Team_When_Exit : public ScriptImpClass
{
	int Team;
	void Created(GameObject *obj)
	{
		Team = Get_Int_Parameter("Team");
	}

	void Custom(GameObject *obj, int type, int param, GameObject *sender)
	{
		if (type == CUSTOM_EVENT_VEHICLE_EXITED)
		{
			Set_Object_Type(obj, Team);
			Commands->Start_Timer(obj, this, 0.2f, 1);
		}
	}

	void Timer_Expired(GameObject *obj, int number)
	{
		Set_Object_Type(obj, Team);
	}
};

class MC_IonExplosion_When_Death : public ScriptImpClass
{
	void Killed(GameObject *obj, GameObject *killer)
	{
		Vector3 Pos = Commands->Get_Position(obj);
		Create_Explosion_Extended("Explosion_IonCannonBeacon", Pos, obj);
		Commands->Create_Object("Beacon_Ion_Cannon_Anim_Post", Commands->Get_Position(obj));
		Destroy_Script();
	}
};

class MC_Capturable_Object : public ScriptImpClass
{
	GameObject *Building;
	const char *Repair_Sound;
	const char *Destroy_Sound;
	unsigned int Red;
	unsigned int Green;
	unsigned int Blue;
	const char *Repair_Message;
	const char *Destroy_Message;
	bool Killed;
	void Created(GameObject *obj)
	{
		Building = obj;
		Red = Get_Int_Parameter("Red");
		Green = Get_Int_Parameter("Green");
		Blue = Get_Int_Parameter("Blue");
		Repair_Sound = Get_Parameter("RepairSound");
		Destroy_Sound = Get_Parameter("DestroySound");
		Repair_Message = Get_Parameter("RepairMessage");
		Destroy_Message = Get_Parameter("DestroyMessage");
		Set_Object_Type(Building, Get_Int_Parameter("InitialTeam"));
		if (Get_Int_Parameter("InitialTeam") == -2)
		{
			Killed = true;
		}
		else
		{
			Killed = false;
		}
	}

	void Damaged(GameObject *obj, GameObject *damager, float amount)
	{
		if (amount < 0.0f) //Repair
		{
			if (Commands->Get_Player_Type(Building) == -2 && Commands->Get_Health(Building) + Commands->Get_Shield_Strength(Building) == Commands->Get_Max_Health(Building) + Commands->Get_Max_Shield_Strength(Building) && Commands->Get_Player_Type(damager) != -2)
			{
				Set_Object_Type(Building, Commands->Get_Player_Type(damager));
				Send_Message_Team(Commands->Get_Player_Type(damager), Red, Green, Blue, Repair_Message);
				Create_2D_WAV_Sound_Team(Repair_Sound, Commands->Get_Player_Type(damager));
				Killed = false;
			}
		}
		else if (amount > Commands->Get_Health(Building))
		{
			if (!Killed)
			{
				Send_Message(Red, Green, Blue, Destroy_Message);
				Create_2D_WAV_Sound_Team(Repair_Sound, 0);
				Create_2D_WAV_Sound_Team(Repair_Sound, 1);
				Commands->Set_Health(Building, 1.0f);
				Set_Object_Type(Building, -2);
			}
			else
			{
				Commands->Set_Health(Building, 1.0f);
				Set_Object_Type(Building, -2);
			}
		}
	}
};

class MC_FlyMode_On : public ScriptImpClass
{
	void Created(GameObject *obj)
	{
		if (!Get_Fly_Mode(obj))
		{
			Toggle_Fly_Mode(obj);
		}
		Destroy_Script();
	}
};

class MC_FlyMode_Off : public ScriptImpClass
{
	void Created(GameObject *obj)
	{
		if (Get_Fly_Mode(obj))
		{
			Toggle_Fly_Mode(obj);
		}
		Destroy_Script();
	}
};

class MC_FlyModeZone_On : public ScriptImpClass
{
	void Entered(GameObject *obj, GameObject *enterer)
	{
		if (!Get_Fly_Mode(enterer))
		{
			Toggle_Fly_Mode(enterer);
		}
	}
};

class MC_FlyModeZone_Off : public ScriptImpClass
{
	void Entered(GameObject *obj, GameObject *enterer)
	{
		if (Get_Fly_Mode(enterer))
		{
			Toggle_Fly_Mode(enterer);
		}
	}
};

class MC_Spectate : public ScriptImpClass
{
	void Created(GameObject *obj)
	{
		SoldierGameObj *Soldier = obj->As_SoldierGameObj();
		int Team = Soldier->Get_Player_Type();
		if (Team != -4)
		{
			if (Soldier->Get_Vehicle())
			{
				Destroy_Script();
			}
			if (!Get_Fly_Mode(Soldier))
			{
				Toggle_Fly_Mode(Soldier);
			}
			Soldier->Set_Player_Type(-4);
			Commands->Set_Model(Soldier, "NULL");
			Commands->Clear_Weapons(Soldier);
			Commands->Disable_All_Collisions(Soldier);
			Commands->Set_Is_Visible(Soldier, false);
			Commands->Set_Is_Rendered(Soldier, false);
			Commands->Set_Shield_Type(Soldier, "Blamo");
			Disarm_All_C4_Beacons(Get_Player_ID(Soldier));
			for (SLNode<cPlayer>* PlayerIter = Get_Player_List()->Head(); (PlayerIter != NULL); PlayerIter = PlayerIter->Next())
			{
				cPlayer *p = PlayerIter->Data();
				if (!p->Is_Active()) continue;
				if (p->Get_Id() == Get_Player_ID(Soldier))
				{
					Update_Network_Object_Player(Soldier, p->Get_Id());
				}
				else
				{
					bool pending = Soldier->Is_Delete_Pending();
					Soldier->Set_Is_Delete_Pending(true);
					Update_Network_Object_Player(Soldier, p->Get_Id());
					Soldier->Set_Is_Delete_Pending(pending);
				}
			}
			Soldier->Clear_Object_Dirty_Bits();
		}
		Destroy_Script();
	}
};

class MC_Spectate_Zone : public ScriptImpClass
{
	void Entered(GameObject *obj, GameObject *enterer)
	{
		SoldierGameObj *Soldier = enterer->As_SoldierGameObj();
		int Team = Soldier->Get_Player_Type();
		if (Team != -4)
		{
			if (Soldier->Get_Vehicle())
			{
				return;
			}
			if (!Get_Fly_Mode(Soldier))
			{
				Toggle_Fly_Mode(Soldier);
			}
			Soldier->Set_Player_Type(-4);
			Commands->Set_Model(Soldier, "NULL");
			Commands->Clear_Weapons(Soldier);
			Commands->Disable_All_Collisions(Soldier);
			Commands->Set_Is_Visible(Soldier, false);
			Commands->Set_Is_Rendered(Soldier, false);
			Commands->Set_Shield_Type(Soldier, "Blamo");
			Disarm_All_C4_Beacons(Get_Player_ID(Soldier));
			for (SLNode<cPlayer>* PlayerIter = Get_Player_List()->Head(); (PlayerIter != NULL); PlayerIter = PlayerIter->Next())
			{
				cPlayer *p = PlayerIter->Data();
				if (!p->Is_Active()) continue;
				if (p->Get_Id() == Get_Player_ID(Soldier))
				{
					Update_Network_Object_Player(Soldier, p->Get_Id());
				}
				else
				{
					bool pending = Soldier->Is_Delete_Pending();
					Soldier->Set_Is_Delete_Pending(true);
					Update_Network_Object_Player(Soldier, p->Get_Id());
					Soldier->Set_Is_Delete_Pending(pending);
				}
			}
			Soldier->Clear_Object_Dirty_Bits();
		}
	}
};

class MC_Beacon_Block_Zone : public ScriptImpClass
{
	//const char *BeaconPresets[6];
	void Created(GameObject *obj)
	{

	}

	void Entered(GameObject *obj, GameObject *enterer)
	{
		SoldierGameObj *Soldier;
		if (enterer->As_VehicleGameObj())
		{
			Soldier = enterer->As_VehicleGameObj()->Get_Driver();
		}
		else
		{
			Soldier = enterer->As_SoldierGameObj();
		}
		Commands->Start_Timer(obj, this, 0.2f, Commands->Get_ID(Soldier));
	}

	void Exited(GameObject *obj, GameObject *exiter)
	{
		//Reserved for next update :)))
	}

	void Timer_Expired(GameObject *obj, int number)
	{
		const char *BeaconPresets[6] = { "CnC_Weapon_IonCannonBeacon_Player", "Weapon_IonCannonBeacon_Player", "Weapon_IonCannonBeacon_Ai", "CnC_Weapon_IonCannonBeacon_Player", "CnC_Weapon_NukeBeacon_Player", "Weapon_NukeBeacon_Ai" };
		if (IsInsideZone(obj, Commands->Find_Object(number)))
		{
			SoldierGameObj *Soldier = Commands->Find_Object(number)->As_SoldierGameObj();
			unsigned int Red = Get_Int_Parameter("Red");
			unsigned int Green = Get_Int_Parameter("Green");
			unsigned int Blue = Get_Int_Parameter("Blue");
			for (size_t i = 0; i < 6; i++)
			{
				if (Get_Current_Weapon(Soldier) == BeaconPresets[i])
				{
					Commands->Select_Weapon(Soldier, "");
					Send_Message_Player(Soldier, Red, Green, Blue, Get_Parameter("BlockMessage"));
				}
			}
			Commands->Start_Timer(obj, this, 0.2f, number);
		}
	}
};

class MC_Open_Mutant_PT : public ScriptImpClass
{
	void Created(GameObject *obj)
	{
		if (obj->As_SoldierGameObj())
		{
			Commands->Display_Mutant_Player_Terminal();
		}
		Destroy_Script();
	}
};

class MC_Open_GDI_PT : public ScriptImpClass
{
	void Created(GameObject *obj)
	{
		if (obj->As_SoldierGameObj())
		{
			Display_GDI_Player_Terminal_Player(obj);
		}
		Destroy_Script();
	}
};

class MC_Open_Nod_PT : public ScriptImpClass
{
	void Created(GameObject *obj)
	{
		if (obj->As_SoldierGameObj())
		{
			Display_NOD_Player_Terminal_Player(obj);
		}
		Destroy_Script();
	}
};


class MC_Objective_Message_Sender : public ScriptImpClass
{
	static StringClass Underscore_To_Space(StringClass strtext)
	{
		std::string text = std::string(strtext);
		int length = text.size();
		int x = 0;
		std::string fixed;
		while (x != length) {
			std::string letter = text.substr(x, 1);
			if (letter == "_") {
				letter = " ";
			}
			fixed = fixed + letter;
			x = x + 1;
		}
		return StringClass(fixed.c_str());
	}

	bool Pokeable;
	StringClass Message;
	void Created(GameObject *obj)
	{
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
		Message = StringClass(Underscore_To_Space(Get_Parameter("Message")));
		Send_Message(90, 231, 114, StringClass::getFormattedString("New Objective: %s", Message));
		WideStringClass Send;
		Send.Format(L"j\n90\n8269\n%hs\n%f\n%f\n%f\n,", "New Objective Available", 214 / 255.0f, 0 / 255.0f, 255 / 255.0f);
		Send_Client_Text(Send, TEXT_MESSAGE_PUBLIC, false, -2, -1, true, true);
		Create_2D_WAV_Sound_Team("paging_attention_1.wav", 0);
		Create_2D_WAV_Sound_Team("paging_attention_1.wav", 1);
		Commands->Start_Timer(obj, this, 3.0f, 2);
		Pokeable = true;
	}

	void Poked(GameObject *obj, GameObject *poker)
	{
		if (Pokeable)
		{
			if (Commands->Is_A_Star(poker))
			{
				Send_Message_Player(poker, 90, 231, 114, StringClass::getFormattedString("Current Objective: %s", Message));
				Pokeable = false;
				Commands->Enable_HUD_Pokable_Indicator(obj, false);
				Commands->Start_Timer(obj, this, 3.0f, 1);
			}
		}
	}

	void Timer_Expired(GameObject *obj, int number)
	{
		if (number == 1)
		{
			Pokeable = true;
			Commands->Enable_HUD_Pokable_Indicator(obj, true);
		}
		if (number == 2)
		{
			Commands->Start_Timer(obj, this, 90.0f, 2);
			WideStringClass Send;
			Send.Format(L"j\n90\n8269\n%hs\n%f\n%f\n%f\n,", Message, 214 / 255.0f, 0 / 255.0f, 255 / 255.0f);
			Send_Client_Text(Send, TEXT_MESSAGE_PUBLIC, false, -2, -1, true, true);
		}
	}

	void Custom(GameObject *obj, int type, int param, GameObject *sender)
	{
		if (type == 734638724)
		{
			Destroy_Script();
		}
	}
};

class MC_Objective : public ScriptImpClass
{
public:
	StringClass Message;
	int Order;
	const char *OverType; //Valid: KILLOBJECT, ENTER, POKE, NONE
	GameObject *ParameterObject;
	GameObject *Listener;
	GameObject *This;
	int StarTrigger;
	int CustomTriggerID;

	void Send_Debug_Message(const char *String)
	{
		if (Get_Int_Parameter("Debug") == 1)
		{
			Send_Message(255, 255, 255, StringClass::getFormattedString("[MCDBG] %s", String));
			Console_Output(StringClass::getFormattedString("[MCDBG] %s\n", String));
		}
	}

	void Created(GameObject *obj)
	{

		CustomTriggerID = 7236527;
		Message = StringClass::getFormattedString("%s", Get_Parameter("Message"));
		Order = Get_Int_Parameter("ObjectiveOrder");
		OverType = Get_Parameter("ObjectiveCompleteType");
		StarTrigger = Get_Int_Parameter("StarTrigger");
		Listener = Commands->Find_Object(Get_Int_Parameter("Listener"));
		This = obj;
		Send_Debug_Message(StringClass::getFormattedString("Over Type is %s", OverType));
		if (!strstr(OverType, "NONE"))
		{
			ParameterObject = Commands->Find_Object(Get_Int_Parameter("ParameterObject"));
			if (ParameterObject)
			{
				if (std::string(OverType).find("KILLOBJECT") != std::string::npos)
				{
					Send_Debug_Message("Created listener with type KILLOBJECT.");
					Attach_Script_Once(ParameterObject, "MC_Objective_Listener", StringClass::getFormattedString("%i,%s,%i,%i", Commands->Get_ID(obj), OverType, CustomTriggerID, StarTrigger));
				}
				else if (std::string(OverType).find("ENTER") != std::string::npos)
				{
					Send_Debug_Message("Created listener with type ENTER.");
					Attach_Script_Once(ParameterObject, "MC_Objective_Listener", StringClass::getFormattedString("%i,%s,%i,%i", Commands->Get_ID(obj), OverType, CustomTriggerID, StarTrigger));
				}
				else if (std::string(OverType).find("POKE") != std::string::npos)
				{
					Send_Debug_Message("Created listener with type POKE.");
					Attach_Script_Once(ParameterObject, "MC_Objective_Listener", StringClass::getFormattedString("%i,%s,%i,%i", Commands->Get_ID(obj), OverType, CustomTriggerID, StarTrigger));
				}
				else
				{
					Send_Debug_Message("Can't create listener. Event is unknown.");
				}
			}
		}
		else
		{
			Send_Debug_Message("Created listener with type NONE.");
		}
		//MC_Objective_Controller::Scripts->Add_Head(this); //FIX
		Commands->Start_Timer(obj, this, 2.0f, 129);
	}

	void Custom(GameObject *obj, int type, int param, GameObject *sender)
	{
		if (ParameterObject)
		{
			if (sender == ParameterObject)
			{
				if (type == CustomTriggerID)
				{
					Commands->Send_Custom_Event(obj, Listener, 777888994, Order, 0.0f);
					Destroy_Script();
				}
			}
		}
	}
};
class MC_Objective_Controller : public ScriptImpClass
{
	GameObject *MC_Objective_Controller::ObjectiveMessageSender; //nvm
	DynamicVectorClass<GameObject*> MC_Objective_Controller::Objectives;
	SList<MC_Objective> MC_Objective_Controller::Scripts;
	int Current_Objective;

public:

	void Send_Debug_Message(const char *String)
	{
		if (Get_Int_Parameter("Debug") == 1)
		{
			Send_Message(255, 255, 255, StringClass::getFormattedString("[MCDBG] %s", String));
			Console_Output(StringClass::getFormattedString("[MCDBG] %s\n", String));
		}
	}

	void Created(GameObject *obj)
	{
		Send_Debug_Message("Controller created.");
		ObjectiveMessageSender = Commands->Find_Object(Get_Int_Parameter("MessageObject"));
		//Objectives = /*&std::vector<GameObject*>();*/ &DynamicVectorClass<GameObject*>();
		Scripts = (SList<MC_Objective>());
		Current_Objective = 0;
		const char *ObjectiveList = Get_Parameter("Objectives");
		std::vector<std::string> Objects = split(std::string(ObjectiveList), '-');
		for (size_t i = 0; i < Objects.size(); i++)
		{
			int ObjectID = stoi(Objects[i]);
			GameObject *obj = Commands->Find_Object(ObjectID);
			Objectives.Insert(0, obj);
		}
		Send_Debug_Message(StringClass::getFormattedString("%i objectives found and included.", Objects.size()));
		//Starting first objective.
		CurrentObjective = Get_Objective(0);
		Send_Debug_Message("Started first objective.");
		if (CurrentObjective)
		{
			Attach_Script_V(ObjectiveMessageSender, "MC_Objective_Message_Sender", CurrentObjective->Message);
			Send_Debug_Message("Attached message sender.");
		}
		else
		{
			Commands->Start_Timer(obj, this, 0.2f, 2);
		}
	}

	MC_Objective *CurrentObjective;
	void Custom(GameObject *obj, int type, int param, GameObject *sender)
	{
		if (type == 777888994)
		{
			if (Current_Objective == param)
			{
				Send_Debug_Message("Received finish objective custom from listener.");
				CurrentObjective = Get_Objective(Current_Objective + 1);
				if (CurrentObjective)
				{
					Current_Objective++;
					Send_Debug_Message("Found a new objective to start.");
					Commands->Send_Custom_Event(obj, ObjectiveMessageSender, 734638724, 0, 0.0f);
					Commands->Start_Timer(obj, this, 0.2f, 1);
				}
				else
				{
					Send_Debug_Message("No new objectives detected.");
					Commands->Send_Custom_Event(obj, ObjectiveMessageSender, 734638724, 0, 0.0f);
					Commands->Start_Timer(obj, this, 0.2f, 2);
				}
			}
		}
	}

	void Timer_Expired(GameObject *obj, int number)
	{
		if (number != 2)
		{
			if (!Is_Script_Attached(ObjectiveMessageSender, "MC_Objective_Message_Sender"))
			{
				Attach_Script_V(ObjectiveMessageSender, "MC_Objective_Message_Sender", StringClass::getFormattedString("%s", CurrentObjective->Message));
				Send_Debug_Message("Started a new objective.");
			}
			else
			{
				Commands->Start_Timer(obj, this, 0.2f, 1);
			}
		}
		else
		{
			if (!Is_Script_Attached(ObjectiveMessageSender, "MC_Objective_Message_Sender"))
			{
				GameObject *NoObjective = Commands->Create_Object("Daves Arrow", Vector3(0.0f, 0.0f, 0.0f));
				Commands->Set_Model(NoObjective, "NULL");
				Attach_Script_V(NoObjective, "MC_Objective", "There_are_no_new_objectives_detected._Game_might_be_ended.,%i,NONE,0,%i,0", Current_Objective, Commands->Get_ID(obj));
				Objectives.Insert(0, NoObjective);
				Attach_Script_V(ObjectiveMessageSender, "MC_Objective_Message_Sender", StringClass::getFormattedString("There_are_no_new_objectives_detected._Game_might_be_ended."));
				Send_Debug_Message("Attached no objectives message.");
			}
			else
			{
				Commands->Start_Timer(obj, this, 0.2f, 2);
			}
		}
	}

	std::vector<std::string> split(std::string strToSplit, char delimeter)
	{
		std::stringstream ss(strToSplit);
		std::string item;
		std::vector<std::string> splittedStrings;
		while (std::getline(ss, item, delimeter))
		{
			splittedStrings.push_back(item);
		}
		return splittedStrings;
	}
	MC_Objective *Get_Objective(int Index)
	{
		//int ind = 0;
		for (int i = 0; i < Objectives.Count(); i++) //Unreachable code
		{
			if (Objectives[i])
			{
				MC_Objective *Objective = (MC_Objective*)Find_Script_On_Object(Objectives[i], "MC_Objective"); //this was nullptr.
				if (Objective)
				{
					int obj_order = Objective->Order;
					if (Index == obj_order)
					{
						return Objective;
					}
				}
			}
			else
			{

			}
		}
		return 0;
	}

	MC_Objective *Get_Objective_Script(GameObject *obj)
	{
		for (SLNode<MC_Objective> *z = Scripts.Head(); z; z = z->Next())
		{
			if (z->Data())
			{
				if (z->Data()->This == obj)
				{
					return (z->Data());
				}
			}
			else
			{

			}
		}
		return 0;
		/*SimpleDynVecClass<GameObjObserverClass*> Observers = obj->Get_Observers();
		for (size_t i = 0; i < Observers.Count; i++)
		{
		if (Observers[i]->Get_Name() == "MC_Objective")
		{
		return Observers[i];
		}
		}*/
	}

};

class MC_Objective_Listener : public ScriptImpClass
{
	GameObject *Objective;
	const char *TriggerType;
	int CustomToSend;
	int OnlyStarTriggered;
	void Send_Debug_Message(const char *String)
	{
		if (Get_Int_Parameter("Debug") == 1)
		{
			Send_Message(255, 255, 255, StringClass::getFormattedString("[MCDBG] %s", String));
			Console_Output(StringClass::getFormattedString("[MCDBG] %s\n", String));
		}
	}

	void Created(GameObject *obj)
	{
		Send_Debug_Message(StringClass::getFormattedString("LISTENER: Created listener at %i...", Commands->Get_ID(obj)));
		Objective = Commands->Find_Object(Get_Int_Parameter("Objective"));
		TriggerType = Get_Parameter("Trigger");
		CustomToSend = Get_Int_Parameter("CustomID");
		OnlyStarTriggered = Get_Int_Parameter("OnlyStarCanTrigger");
	}

	void Killed(GameObject *obj, GameObject *killer)
	{
		if (std::string(TriggerType).find("KILLOBJECT") != std::string::npos)
		{
			Commands->Send_Custom_Event(obj, Objective, CustomToSend, 0, 0.0f);
			Destroy_Script();
		}
	}

	void Destroyed(GameObject *obj)
	{
		if (std::string(TriggerType).find("KILLOBJECT") != std::string::npos)
		{
			Commands->Send_Custom_Event(obj, Objective, CustomToSend, 0, 0.0f);
			Destroy_Script();
		}
	}

	void Poked(GameObject *obj, GameObject *poker)
	{
		if (std::string(TriggerType).find("POKE") != std::string::npos)
		{
			if (OnlyStarTriggered == 1)
			{
				if (Commands->Is_A_Star(poker))
				{
					Commands->Send_Custom_Event(obj, Objective, CustomToSend, 0, 0.0f);
					Destroy_Script();
				}
			}
			else
			{
				Commands->Send_Custom_Event(obj, Objective, CustomToSend, 0, 0.0f);
				Destroy_Script();
			}
		}
	}

	void Entered(GameObject *obj, GameObject *enterer)
	{
		if (std::string(TriggerType).find("ENTER") != std::string::npos)
		{
			if (OnlyStarTriggered == 1)
			{
				if (Commands->Is_A_Star(enterer))
				{
					Commands->Send_Custom_Event(obj, Objective, CustomToSend, 0, 0.0f);
					Destroy_Script();
				}
			}
			else
			{
				Commands->Send_Custom_Event(obj, Objective, CustomToSend, 0, 0.0f);
				Destroy_Script();
			}
		}
	}

	void Custom(GameObject *obj, int type, int param, GameObject *sender)
	{
		if (sender)
		{
			if (sender == Objective)
			{
				if (type == 4222957) //Destroy listener if objective cancelled.
				{
					Send_Debug_Message(StringClass::getFormattedString("LISTENER: Received end objective event from Controller. Destroying listener at %i...", Commands->Get_ID(obj)));
					Destroy_Script();
				}
			}
		}
	}
};


class MC_Destroy_When_Objective_Completed : public ScriptImpClass
{
	GameObject *ObjectiveObject;

	void Created(GameObject *obj)
	{
		ObjectiveObject = Commands->Find_Object(Get_Int_Parameter("ObjectiveArrow"));
		Commands->Start_Timer(obj, this, 2.0f, 1);
	}

	void Timer_Expired(GameObject *obj, int number)
	{
		if (Is_Script_Attached(ObjectiveObject, "MC_Objective"))
		{

		}
		else
		{
			obj->Set_Delete_Pending();
			Destroy_Script();
		}
		Commands->Start_Timer(obj, this, 2.0f, 1);
	}
};

class MC_Select_Weapon_On_Zone_Enter : public ScriptImpClass
{
	void Entered(GameObject *obj, GameObject *enterer)
	{
		Commands->Select_Weapon(enterer, Get_Parameter("WeaponPreset"));
	}
};

class MC_Select_Weapon_On_Zone_Exit : public ScriptImpClass
{
	void Exited(GameObject *obj, GameObject *exiter)
	{
		Commands->Select_Weapon(exiter, Get_Parameter("WeaponPreset"));
	}
};

class MC_Select_Weapon_On_Zone : public ScriptImpClass
{
	void Entered(GameObject *obj, GameObject *enterer)
	{
		Commands->Select_Weapon(enterer, Get_Parameter("EnterWeaponPreset"));
	}

	void Exited(GameObject *obj, GameObject *exiter)
	{
		Commands->Select_Weapon(exiter, Get_Parameter("ExitWeaponPreset"));
	}
};

class MC_Chinook_Reinforcements_Paradrop : public ScriptImpClass
{
	//Taken from Single Player scriot M03_Chinook_ParaDrop and modified to drop 3 different soldiers.
public:
	void Register_Auto_Save_Variables()
	{
		Auto_Save_Variable(&this->nodTransportHelicopterObjId, sizeof(this->nodTransportHelicopterObjId), 1);
		Auto_Save_Variable(&this->chinookKilled, sizeof(this->chinookKilled), 2);
		Auto_Save_Variable(&this->paratrooperIndex, sizeof(this->paratrooperIndex), 3);
	}

private:
	int nodTransportHelicopterObjId;
	bool chinookKilled;
	int paratrooperIndex;
	GameObject *presetObj1;
	GameObject *presetObj2;
	GameObject *presetObj3;
	GameObject *nodTransportHelicopterObj;
	GameObject *MaintrajectoryObj;
	int SoundObj;

	void Send_Debug_Message(const char *String, ...)
	{
		if (Get_Int_Parameter("Debug") == 1)
		{
			Send_Message(255, 255, 255, StringClass::getFormattedString("[MCDBG] %s", String));
			Console_Output(StringClass::getFormattedString("[MCDBG] %s\n", String));
		}
	}

	bool Check_Object(GameObject *obj)
	{
		if (obj)
		{
			if (obj->As_SmartGameObj())
			{
				if (obj->As_SoldierGameObj())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	void Created(GameObject *obj)
	{
		presetObj1 = 0;
		presetObj2 = 0;
		presetObj3 = 0;
		Vector3 pos = Commands->Get_Position(obj);
		float facing = Commands->Get_Facing(obj);
		Send_Debug_Message("Chinook Drop script is now initializing...");
		GameObject *MaintrajectoryObj = Commands->Create_Object("Generic_Cinematic", pos);
		Commands->Set_Model(MaintrajectoryObj, "X5D_Chinookfly");
		Commands->Set_Facing(MaintrajectoryObj, facing);
		Commands->Set_Animation(MaintrajectoryObj, "X5D_Chinookfly.X5D_Chinookfly", false, NULL, 0.0f, -1.0f, false);
		Send_Debug_Message("Created main Trajectory object.");
		if (Get_Object_Type(obj) == 0)
		{

			Send_Debug_Message("Creating Nod Chinook...");
			nodTransportHelicopterObj = Commands->Create_Object("Nod_Transport_Helicopter_Flyover", pos);
			Set_Max_Health(nodTransportHelicopterObj, 100000.0f);
			Set_Max_Shield_Strength(nodTransportHelicopterObj, 100000.0f);
			Commands->Set_Health(nodTransportHelicopterObj, 100000.0f);
			Commands->Set_Shield_Strength(nodTransportHelicopterObj, 100000.0f);
			Commands->Set_Facing(nodTransportHelicopterObj, facing);
			Commands->Disable_Physical_Collisions(nodTransportHelicopterObj);
			Commands->Set_Animation(nodTransportHelicopterObj, "v_Nod_trnspt.v_Nod_trnspt", true, NULL, 0.0f, -1.0f, false);
			Commands->Attach_To_Object_Bone(nodTransportHelicopterObj, MaintrajectoryObj, "BN_Chinook_1");
			Send_Debug_Message("Chinook creation succesful.");
		}
		else
		{
			Send_Debug_Message("Creating GDI Chinook...");
			nodTransportHelicopterObj = Commands->Create_Object("GDI_Transport_Helicopter_Flyover", pos);
			Set_Max_Health(nodTransportHelicopterObj, 100000.0f);
			Set_Max_Shield_Strength(nodTransportHelicopterObj, 100000.0f);
			Commands->Set_Health(nodTransportHelicopterObj, 100000.0f);
			Commands->Set_Shield_Strength(nodTransportHelicopterObj, 100000.0f);
			Commands->Set_Facing(nodTransportHelicopterObj, facing);
			Commands->Disable_Physical_Collisions(nodTransportHelicopterObj);
			Commands->Set_Animation(nodTransportHelicopterObj, "v_GDI_trnspt.v_GDI_trnspt", true, NULL, 0.0f, -1.0f, false);
			Commands->Attach_To_Object_Bone(nodTransportHelicopterObj, MaintrajectoryObj, "BN_Chinook_1");
			Send_Debug_Message("Chinook creation succesful.");
		}
		Send_Debug_Message("Finishing initialization...");
		this->chinookKilled = false;
		this->paratrooperIndex = 0;

		int objId = Commands->Get_ID(obj);
		char buffer[16];
		sprintf(buffer, "%d", objId);
		Commands->Attach_Script(nodTransportHelicopterObj, "M03_Reinforcement_Chinook", buffer);

		this->nodTransportHelicopterObjId = Commands->Get_ID(nodTransportHelicopterObj);
		Send_Debug_Message("Starting timers...");
		Commands->Start_Timer(obj, this, 300 / 30.0f, 0);
		Commands->Start_Timer(obj, this, 169 / 30.0f, 1);
		Commands->Start_Timer(obj, this, 179 / 30.0f, 2);
		Commands->Start_Timer(obj, this, 198 / 30.0f, 3);
		Commands->Start_Timer(obj, this, 145 / 30.0f, 4);
		Commands->Start_Timer(obj, this, 155 / 30.0f, 5);
		Commands->Start_Timer(obj, this, 165 / 30.0f, 6);
		Commands->Start_Timer(obj, this, 25 / 30.0f, 7);
		Commands->Start_Timer(obj, this, 280 / 30.0f, 8);
		Send_Debug_Message("Script is now enabled and playing cinematic...");
	}

	void Custom(GameObject *obj, int type, int param, GameObject *sender)
	{
		if (type == 23000 && param == 23000)
		{
			this->chinookKilled = true;
		}

		if (type == 15730)
		{
			GameObject *Attach = Commands->Find_Object(param);
			if (Attach)
			{
				presetObj1 = Attach;
				Send_Debug_Message(StringClass::getFormattedString("Object at Slot 1 has changed to object ID %i by a custom.", param));
			}
			else
			{
				Send_Debug_Message(StringClass::getFormattedString("Object at Slot 1 was tried to change with object %i but object was unable to found.", param));
			}
		}
		if (type == 15731)
		{
			GameObject *Attach = Commands->Find_Object(param);
			if (Attach)
			{
				presetObj2 = Attach;
				Send_Debug_Message(StringClass::getFormattedString("Object at Slot 2 has changed to object ID %i by a custom.", param));
			}
			else
			{
				Send_Debug_Message(StringClass::getFormattedString("Object at Slot 2 was tried to change with object %i but object was unable to found.", param));
			}
		}
		if (type == 15732)
		{
			GameObject *Attach = Commands->Find_Object(param);
			if (Attach)
			{
				presetObj3 = Attach;
				Send_Debug_Message(StringClass::getFormattedString("Object at Slot 3 has changed to object ID %i by a custom.", param));
			}
			else
			{
				Send_Debug_Message(StringClass::getFormattedString("Object at Slot 3 was tried to change with object %i but object was unable to found.", param));
			}
		}
	}

	void Timer_Expired(GameObject *obj, int number)
	{
		Vector3 pos = Commands->Get_Position(obj);
		const char *preset1 = Get_Parameter("FirstPreset");
		const char *preset2 = Get_Parameter("SecondPreset");
		const char *preset3 = Get_Parameter("ThirdPreset");
		float facing = Commands->Get_Facing(obj);

		if (number == 0)
		{
			Send_Debug_Message("Cinematic ended and now scripts is being detached...");
			Commands->Send_Custom_Event(0, presetObj1, 1248753, 0, 0.0f);
			Commands->Send_Custom_Event(0, presetObj2, 1248753, 0, 0.0f);
			Commands->Send_Custom_Event(0, presetObj3, 1248753, 0, 0.0f);
			Send_Debug_Message("Cinematic played successfully! Detaching script...");
			Destroy_Script();
		}

		else if (number == 8)
		{
			GameObject *nodTransportHelicopterObj = Commands->Find_Object(this->nodTransportHelicopterObjId);
			if (nodTransportHelicopterObj)
			{
				Commands->Destroy_Object(nodTransportHelicopterObj);
				Send_Debug_Message("Destroyed Chinook.");
			}
			else
			{
				Send_Debug_Message("Couldn't destroy Chinook: Object did not found.");
			}
		}

		else if (number == 7)
		{
			if (nodTransportHelicopterObj)
			{
				Commands->Enable_Engine(nodTransportHelicopterObj, true);
				Send_Debug_Message("Enabled Chinook engine.");
			}
			else
			{
				Send_Debug_Message("Couldn't enable Chinook engine: Object did not found.");
			}
		}

		else if (number == 1)
		{
			if (this->paratrooperIndex > 0)
			{
				if (presetObj1)
				{
					GameObject *cinObj = Commands->Create_Object("Generic_Cinematic", pos);
					Commands->Set_Facing(cinObj, facing);
					Commands->Set_Model(cinObj, "X5D_Parachute");
					Commands->Set_Animation(cinObj, "X5D_Parachute.X5D_ParaC_1", false, NULL, 0.0f, -1.0f, false);
					Commands->Create_3D_Sound_At_Bone("parachute_open", cinObj, "ROOTTRANSFORM");
					Commands->Attach_Script(cinObj, "M03_No_More_Parachute", "");
					Send_Debug_Message("Enabled chute for the object at slot 1.");
				}
				else
				{
					Send_Debug_Message("Couldn't enabled chute for the object at slot 1: Object did not found.");
				}
			}
		}

		else if (number == 2)
		{
			if (this->paratrooperIndex > 1)
			{
				if (presetObj2)
				{
					GameObject *cinObj = Commands->Create_Object("Generic_Cinematic", pos);
					Commands->Set_Facing(cinObj, facing);
					Commands->Set_Model(cinObj, "X5D_Parachute");
					Commands->Set_Animation(cinObj, "X5D_Parachute.X5D_ParaC_2", false, NULL, 0.0f, -1.0f, false);
					Commands->Create_3D_Sound_At_Bone("parachute_open", cinObj, "ROOTTRANSFORM");
					Commands->Attach_Script(cinObj, "M03_No_More_Parachute", "");
					Send_Debug_Message("Enabled chute for the object at slot 2.");
				}
				else
				{
					Send_Debug_Message("Couldn't enabled chute for the object at slot 2: Object did not found.");
				}
			}
		}

		else if (number == 3)
		{
			if (this->paratrooperIndex == 3)
			{
				if (presetObj3)
				{
					GameObject *cinObj = Commands->Create_Object("Generic_Cinematic", pos);
					Commands->Set_Facing(cinObj, facing);
					Commands->Set_Model(cinObj, "X5D_Parachute");
					Commands->Set_Animation(cinObj, "X5D_Parachute.X5D_ParaC_3", false, NULL, 0.0f, -1.0f, false);
					Commands->Create_3D_Sound_At_Bone("parachute_open", cinObj, "ROOTTRANSFORM");
					Commands->Attach_Script(cinObj, "M03_No_More_Parachute", "");
					Send_Debug_Message("Enabled chute for the object at slot 3.");
				}
				else
				{
					Send_Debug_Message("Couldn't enabled chute for the object at slot 3: Object did not found.");
				}
			}
		}

		else if (number == 4)
		{
			if (!this->chinookKilled)
			{
				if (Is_Valid_Preset(preset1) || presetObj1)
				{
					Send_Debug_Message("Initializing drop animation for object at slot 1...");
					GameObject *trajectoryObj = Commands->Create_Object("Generic_Cinematic", pos);
					Commands->Set_Model(trajectoryObj, "X5D_Box01");
					Commands->Set_Facing(trajectoryObj, facing);
					Commands->Set_Animation(trajectoryObj, "X5D_Box01.X5D_Box01", false, NULL, 0.0f, -1.0f, false);
					Send_Debug_Message("<Slot 1> Animation created! Attaching object...");
					if (!presetObj1)
					{
						presetObj1 = Commands->Create_Object_At_Bone(trajectoryObj, preset1, "Box01");
						Send_Debug_Message("<Slot 1> The object have been created.");
					}
					else
					{
						Send_Debug_Message("<Slot 1> Object is already attached by a custom event!");
					}

					if (presetObj1)
					{
						if (Check_Object(presetObj1))
						{
							Commands->Set_Facing(presetObj1, facing);
							Send_Debug_Message("<Slot 1> Facing adjusted.");
							Attach_Script_Once(presetObj1, "DB_Innate_Soldier", "0.5,0,0,0");
							Attach_Script_Once(presetObj1, "MC_Unkillable_Until_Custom", "1248753");
							Commands->Attach_Script(presetObj1, "RMV_Trigger_Killed", "1144444, 1000, 1000");
							Commands->Attach_Script(presetObj1, "M03_Killed_Sound", "");
							Commands->Attach_Script(presetObj1, "DLS_Volcano_Stumble", "");
							Send_Debug_Message("<Slot 1> Scripts attached.");
							Commands->Set_Animation(presetObj1, "s_a_human.H_A_X5D_ParaT_1", false, NULL, 0.0f, -1.0f, false);
							Send_Debug_Message("<Slot 1> Animation set.");
						}
						Commands->Attach_To_Object_Bone(presetObj1, trajectoryObj, "Box01");
						Send_Debug_Message("<Slot 1> Object attached to trajectory.");
					}
					Send_Debug_Message("<Slot 1> This slot has completed it's initialization.");
				}
				else
				{
					Send_Debug_Message("Initialization of slot 1 has cancelled: Invalid preset and object is nothing, object cannot be useable.");
				}
				this->paratrooperIndex++;
			}
		}

		else if (number == 5)
		{
			if (!this->chinookKilled)
			{
				if (Is_Valid_Preset(preset2) || presetObj2)
				{
					Send_Debug_Message("Initializing drop animation for object at slot 2...");
					GameObject *trajectoryObj = Commands->Create_Object("Generic_Cinematic", pos);
					Commands->Set_Model(trajectoryObj, "X5D_Box02");
					Commands->Set_Facing(trajectoryObj, facing);
					Commands->Set_Animation(trajectoryObj, "X5D_Box02.X5D_Box02", false, NULL, 0.0f, -1.0f, false);
					Send_Debug_Message("<Slot 2> Animation created! Attaching object...");
					if (!presetObj2)
					{
						presetObj2 = Commands->Create_Object_At_Bone(trajectoryObj, preset2, "Box02");
						Send_Debug_Message("<Slot 2> The object have been created.");
					}
					else
					{
						Send_Debug_Message("<Slot 2> Object is already attached by a custom event!");
					}

					if (presetObj2)
					{
						if (Check_Object(presetObj2))
						{
							Commands->Set_Facing(presetObj2, facing);
							Send_Debug_Message("<Slot 2> Facing adjusted.");
							Attach_Script_Once(presetObj2, "DB_Innate_Soldier", "0.5,0,0,0");
							Attach_Script_Once(presetObj2, "MC_Unkillable_Until_Custom", "1248753");
							Commands->Attach_Script(presetObj2, "RMV_Trigger_Killed", "1144444, 1000, 1000");
							Commands->Attach_Script(presetObj2, "M03_Killed_Sound", "");
							Commands->Attach_Script(presetObj2, "DLS_Volcano_Stumble", "");
							Send_Debug_Message("<Slot 2> Scripts attached.");
							Commands->Set_Animation(presetObj2, "s_a_human.H_A_X5D_ParaT_2", false, NULL, 0.0f, -1.0f, false);
							Send_Debug_Message("<Slot 2> Animation set.");
						}
						Commands->Attach_To_Object_Bone(presetObj2, trajectoryObj, "Box02");
						Send_Debug_Message("<Slot 2> Object attached to trajectory.");
					}
					Send_Debug_Message("<Slot 2> This slot has completed it's initialization.");
				}
				else
				{
					Send_Debug_Message("Initialization of slot 2 has cancelled: Invalid preset and object is nothing, object cannot be useable.");
				}
				this->paratrooperIndex++;
			}
		}

		else if (number == 6)
		{
			if (!this->chinookKilled)
			{
				if (Is_Valid_Preset(preset3) || presetObj3)
				{
					Send_Debug_Message("Initializing drop animation for object at slot 3...");
					GameObject *trajectoryObj = Commands->Create_Object("Generic_Cinematic", pos);
					Commands->Set_Model(trajectoryObj, "X5D_Box03");
					Commands->Set_Facing(trajectoryObj, facing);
					Commands->Set_Animation(trajectoryObj, "X5D_Box03.X5D_Box03", false, NULL, 0.0f, -1.0f, false);
					Send_Debug_Message("<Slot 3> Animation created! Attaching object...");

					if (!presetObj3)
					{
						presetObj3 = Commands->Create_Object_At_Bone(trajectoryObj, preset3, "Box03");
						Send_Debug_Message("<Slot 3> The object have been created.");
					}
					else
					{
						Send_Debug_Message("<Slot 3> Object is already attached by a custom event!");
					}

					if (presetObj3)
					{
						if (Check_Object(presetObj3))
						{
							Commands->Set_Facing(presetObj3, facing);
							Send_Debug_Message("<Slot 3> Facing adjusted.");
							Attach_Script_Once(presetObj3, "DB_Innate_Soldier", "0.5,0,0,0");
							Attach_Script_Once(presetObj3, "MC_Unkillable_Until_Custom", "1248753");
							Commands->Attach_Script(presetObj3, "RMV_Trigger_Killed", "1144444, 1000, 1000");
							Commands->Attach_Script(presetObj3, "M03_Killed_Sound", "");
							Commands->Attach_Script(presetObj3, "DLS_Volcano_Stumble", "");
							Send_Debug_Message("<Slot 3> Scripts attached.");
							Commands->Set_Animation(presetObj3, "s_a_human.H_A_X5D_ParaT_3", false, NULL, 0.0f, -1.0f, false);
							Send_Debug_Message("<Slot 3> Animation set.");
						}
						Commands->Attach_To_Object_Bone(presetObj3, trajectoryObj, "Box03");
						Send_Debug_Message("<Slot 3> Object attached to trajectory.");
					}
					Send_Debug_Message("<Slot 3> This slot has completed it's initialization.");
				}
				else
				{
					Send_Debug_Message("Initialization of slot 3 has cancelled: Invalid preset and object is nothing, object cannot be useable.");
				}
				this->paratrooperIndex++;
			}
		}
	}
};

class MC_Unkillable_Until_Custom : public ScriptImpClass
{
	void Custom(GameObject *obj, int type, int param, GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			Destroy_Script();
		}
	}

	void Damaged(GameObject *obj, GameObject *damager, float amount)
	{
		float maxHealth = Commands->Get_Max_Health(obj);
		Commands->Set_Health(obj, maxHealth);
	}
};

class MC_AttachScript_On_Kill : public ScriptImpClass
{
	void Killed(GameObject *obj, GameObject *killer)
	{
		Attach_Script_Once(Commands->Find_Object(Get_Int_Parameter("ObjectID")), Get_Parameter("Script"), Get_Parameter("Parameters"));
		Destroy_Script();
	}

	void Destroyed(GameObject *obj)
	{
		Attach_Script_Once(Commands->Find_Object(Get_Int_Parameter("ObjectID")), Get_Parameter("Script"), Get_Parameter("Parameters"));
		Destroy_Script();
	}
};

class MC_AttachScript_Interval_Self : public ScriptImpClass
{
	const char *Script;
	const char *Parameters;
	float Interval;
	int LoopCount;
	int Looped;
	void Created(GameObject *obj)
	{
		Script = Get_Parameter("Script");
		Parameters = Get_Parameter("Parameters");
		Commands->Start_Timer(obj, this, Interval, 234245);
		LoopCount = Get_Int_Parameter("LoopCount");
		Looped = 0;
	}

	void Timer_Expired(GameObject *obj, int number)
	{
		if (number == 234245)
		{
			Attach_Script_Once(obj, Script, Parameters);
			if (LoopCount != -1)
			{
				if (Looped >= LoopCount)
				{
					Destroy_Script();
				}
				else
				{
					Commands->Start_Timer(obj, this, Interval, 234245);
				}
			}
			else
			{
				Commands->Start_Timer(obj, this, Interval, 234245);
			}
		}
	}
};

class MC_AttachScript_Interval : public ScriptImpClass
{
	const char *Script;
	const char *Parameters;
	GameObject *Target;
	float Interval;
	int LoopCount;
	int Looped;
	void Created(GameObject *obj)
	{
		Target = Commands->Find_Object(Get_Int_Parameter("ObjectID"));
		Script = Get_Parameter("Script");
		Parameters = Get_Parameter("Parameters");
		Commands->Start_Timer(obj, this, Interval, 234244);
		LoopCount = Get_Int_Parameter("LoopCount");
		Looped = 0;

	}

	void Timer_Expired(GameObject *obj, int number)
	{
		if (number == 234244)
		{
			Attach_Script_Once(Target, Script, Parameters);
			if (LoopCount != -1)
			{
				if (Looped >= LoopCount)
				{
					Destroy_Script();
				}
				else
				{
					Commands->Start_Timer(obj, this, Interval, 234244);
				}
			}
			else
			{
				Commands->Start_Timer(obj, this, Interval, 234244);
			}
		}
	}
};

class MC_Object_Follower_AI : public ScriptImpClass
{
	int TargetChangeCustom;
	int DestroyScriptCustom;
	GameObject *TargetObject;

	void Created(GameObject *obj)
	{
		TargetChangeCustom = Get_Int_Parameter("TargetChangeCustomID");
		DestroyScriptCustom = Get_Int_Parameter("DestroyScriptCustomID");
	}

	void Custom(GameObject *obj, int type, int param, GameObject *sender)
	{
		if (type == TargetChangeCustom)
		{
			GameObject *Target = Commands->Find_Object(param);
			if (Target)
			{
				TargetObject = Target;
				ActionParamsStruct Parameters;
				Parameters.Set_Basic(this, 100.0f, 20000);
				Parameters.Set_Movement(Target, 0.8f, 2.5f);
			}
		}
		else if (type == DestroyScriptCustom)
		{
			Destroy_Script();
		}
	}

	void Timer_Expired(GameObject *obj, int number)
	{
		if (number == 12314178)
		{
			if (TargetObject)
			{
				ActionParamsStruct Parameters;
				Parameters.Set_Basic(this, 100.0f, 20000);
				Parameters.Set_Movement(TargetObject, 0.8f, 2.5f);
			}
		}
	}
};

class MC_Private_Color_Message_On_Poke : public ScriptImpClass
{
	bool Pokeable;
	void Created(GameObject *obj)
	{
		Pokeable = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}

	void Timer_Expired(GameObject *obj, int number)
	{
		if (number == 1)
		{
			Pokeable = true;
			Commands->Enable_HUD_Pokable_Indicator(obj, true);
		}
	}

	void Poked(GameObject *obj, GameObject *poker)
	{
		if (Pokeable)
		{
			Send_Message_Player(poker, Get_Int_Parameter("Red"), Get_Int_Parameter("Green"), Get_Int_Parameter("Blue"), Get_Parameter("Message"));
			Pokeable = false;
			Commands->Enable_HUD_Pokable_Indicator(obj, false);
			Commands->Start_Timer(obj, this, 5.0f, 1);
		}
	}
};

/* NotTest: Not tested yet */
/* Working: Completely working */
/* HasBugs: Has at least 1 bug */
/* NotWork: Not working. Going to be fixed */
/* Working */ ScriptRegistrant<MC_Stop_Vehicle_Engine> MC_Stop_Vehicle_Engine_Registrant("MC_Stop_Vehicle_Engine", "Duration=0:float");
/* Working */ ScriptRegistrant<MC_Vehicle_Purchase_Terminal> MC_Vehicle_Purchase_Terminal_Registrant("MC_Vehicle_Purchase_Terminal", "Team=0:int,SpawnerID=0:int,VehiclePreset:string,Credits=400:int");
/* Working */ ScriptRegistrant<MC_Character_Purchase_Terminal> MC_Character_Purchase_Terminal_Registrant("MC_Character_Purchase_Terminal", "Team=0:int,CharacterPreset:string,Credits=400:int");
/* Working */ ScriptRegistrant<MC_HUD_Message_Zone> MC_HUD_Message_Zone_Registrant("MC_HUD_Message_Zone", "Message=YourMessage:string,Sound=SoundName.wav:string,Red=16.0:float,Green=255.0:float,Blue=16.0:float");
/* Working */ ScriptRegistrant<MC_Teleport> MC_Teleport_Registrant("MC_Teleport", "ArrowID=0:int,TeleportSound=SoundName.wav:string");
/* Working */ ScriptRegistrant<MC_Change_Vehicle_Weapon> MC_Change_Vehicle_Weapon_Registrant("MC_Change_Vehicle_Weapon", "Preset=Weapon:string,Rounds=1:int");
/* Working */ ScriptRegistrant<MC_Change_Vehicle_Weapon_Zone> MC_Change_Vehicle_Weapon_Zone_Registrant("MC_Change_Vehicle_Weapon_Zone", "Preset=Weapon:string,Rounds=1:int");
/* Working */ ScriptRegistrant<MC_Grant_Weapon> MC_Grant_Weapon_Registrant("MC_Grant_Weapon", "Preset=Weapon:string,Rounds=1:int");
/* Working */ ScriptRegistrant<MC_Grant_Weapon_Zone> MC_Grant_Weapon_Zone_Registrant("MC_Grant_Weapon_Zone", "Preset=Weapon:string,Rounds=1:int");
/* Working */ ScriptRegistrant<MC_Grant_Character> MC_Grant_Character_Registrant("MC_Grant_Character", "Preset=Character:string");
/* Working */ ScriptRegistrant<MC_Grant_Character_Zone> MC_Grant_Character_Zone_Registrant("MC_Grant_Character_Zone", "Preset=Character:string");
/* Working */ ScriptRegistrant<MC_Spawn_When_Fully_Repaired> MC_Spawn_When_Fully_Repaired_Registrant("MC_Spawn_When_Fully_Repaired", "Preset=Weapon:string");
/* Working */ ScriptRegistrant<MC_Refinery_Object> MC_Refinery_Object_Registrant("MC_Refinery_Object", "Team=0:int,Interval=1.0:float,Amount=2.0:float");
/* Working */ ScriptRegistrant<MC_GameOver_When_Kill> MC_GameOver_When_Kill_Registrant("MC_GameOver_When_Kill", "");
/* Working */ ScriptRegistrant<MC_Spawn_When_Death> MC_Spawn_When_Death_Registrant("MC_Spawn_When_Death", "Preset=Object:string");
/* Working */ ScriptRegistrant<MC_MasterControlTerminal_Object> MC_MasterControlTerminal_Object_Registrant("MC_MasterControlTerminal_Object", "Object:int,Multiply=1.00:int");
/* Working */ ScriptRegistrant<MC_MCTMainSyncObject> MC_MCTMainSyncObject_Registrant("MC_MCTMainSyncObject", "MCT_ObjectID:int");
/* Working */ ScriptRegistrant<MC_Set_Team_When_Exit> MC_Set_Team_When_Exit_Registrant("MC_Set_Team_When_Exit", "Team=-1:int");
/* Working */ ScriptRegistrant<MC_IonExplosion_When_Death> MC_IonExplosion_When_Death_Registrant("MC_IonExplosion_When_Death", "");
/* Working */ ScriptRegistrant<MC_Capturable_Object> MC_Capturable_Object_Registrant("MC_Capturable_Object", "Red=16:int,Green=255:int,Blue=16:int,RepairSound:string,DestroySound:string,RepairMessage:string,DestroyMessage:string,InitialTeam=-2:int");
/* Working */ ScriptRegistrant<MC_FlyMode_On> MC_FlyMode_On_Registrant("MC_FlyMode_On", "");
/* Working */ ScriptRegistrant<MC_FlyMode_Off> MC_FlyMode_Off_Registrant("MC_FlyMode_Off", "");
/* Working */ ScriptRegistrant<MC_FlyModeZone_On> MC_FlyModeZone_On_Registrant("MC_FlyModeZone_On", "");
/* Working */ ScriptRegistrant<MC_FlyModeZone_Off> MC_FlyModeZone_Off_Registrant("MC_FlyModeZone_Off", "");
/* Working */ ScriptRegistrant<MC_Spectate> MC_Spectate_Registrant("MC_Spectate", "");
/* Working */ ScriptRegistrant<MC_Spectate_Zone> MC_Spectate_Zone_Registrant("MC_Spectate_Zone", "");
/* HasBugs */ ScriptRegistrant<MC_Beacon_Block_Zone> MC_Beacon_Block_Zone_Registrant("MC_Beacon_Block_Zone", "Red=16:int,Green=255:int,Blue=16:int,BlockMessage:string");
/* Working */ ScriptRegistrant<MC_Objective_Message_Sender> MC_Objective_Message_Sender_Registrant("MC_Objective_Message_Sender", "Message=Use_Underscore_For_Space:string");
/* Working */ ScriptRegistrant<MC_Objective_Controller> MC_Objective_Controller_Registrant("MC_Objective_Controller", "Objectives=Objective-Objects-IDs-Here-Minus-Is-Delimeter:string,MessageObject=100000:int,Debug=1:int");
/* Working */ ScriptRegistrant<MC_Objective_Listener> MC_Objective_Listener_Registrant("MC_Objective_Listener", "Objective=100000:int,Trigger:string,CustomID:int,OnlyStarCanTrigger:int,Debug=1:int");
/* Working */ ScriptRegistrant<MC_Objective> MC_Objective_Registrant("MC_Objective", "Message=Use_Underscore_For_Space:string,ObjectiveOrder=0:int,ObjectiveCompleteType=Must_be_KILLOBJECT_ENTER_POKE_or_NONE:string,StarTrigger=0:int,Listener=100000:int,ParameterObject=100000:int,Debug=1:int");
/* Working */ ScriptRegistrant<MC_Destroy_When_Objective_Completed> MC_Destroy_When_Objective_Completed_Registrant("MC_Destroy_When_Objective_Completed", "ObjectiveArrow=100000:int");
/* Working */ ScriptRegistrant<MC_Select_Weapon_On_Zone> MC_Select_Weapon_On_Zone_Registrant("MC_Select_Weapon_On_Zone", "EnterWeaponPreset:string,ExitWeaponPreset:string");
/* Working */ ScriptRegistrant<MC_Select_Weapon_On_Zone_Enter> MC_Select_Weapon_On_Zone_Enter_Registrant("MC_Select_Weapon_On_Zone_Enter", "WeaponPreset:string");
/* Working */ ScriptRegistrant<MC_Select_Weapon_On_Zone_Exit> MC_Select_Weapon_On_Zone_Exit_Registrant("MC_Select_Weapon_On_Zone_Exit", "WeaponPreset:string");
/* Working */ ScriptRegistrant<MC_Chinook_Reinforcements_Paradrop> MC_Chinook_Reinforcements_Paradrop_Registrant("MC_Chinook_Reinforcements_Paradrop", "FirstPreset:string,SecondPreset:string,ThirdPreset:string,Debug=1:int");
/* Working */ ScriptRegistrant<MC_Unkillable_Until_Custom> MC_Unkillable_Until_Custom_Registrant("MC_Unkillable_Until_Custom", "Message:int");
/* Working */ ScriptRegistrant<MC_AttachScript_On_Kill> MC_AttachScript_On_Kill_Registrant("MC_AttachScript_On_Kill", "ObjectID:int,Script:string,Parameters:string");
/* Working */ ScriptRegistrant<MC_AttachScript_Interval_Self> MC_AttachScript_Interval_Self_Registrant("MC_AttachScript_Interval_Self", "Script:string,Parameters:string,LoopCount=-1:int");
/* Working */ ScriptRegistrant<MC_AttachScript_Interval> MC_AttachScript_Interval_Registrant("MC_AttachScript_Interval", "ObjectID:int,Script:string,Parameters:string,LoopCount=-1:int");
/* Working */ ScriptRegistrant<MC_Private_Color_Message_On_Poke> MC_Private_Color_Message_On_Poke_Registrant("MC_Private_Color_Message_On_Poke", "Red=255:int,Green=255:int,Blue=255:int,Message=Message:string");