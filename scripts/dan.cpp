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
#include "engine.h"
#include "dan.h"

void DAN_CnC_Crate::Created(GameObject *obj)
{
	// Create the vehicle blocker and save it as a GameObject Variable,so I can destroy 
	// it when the crate is picked up! 
	Commands->Set_Model(obj,"vehcol2m");
	VehBlock = Commands->Get_ID(Commands->Create_Object("Vehicle_Blocker",Commands->Get_Position(obj)));
}

void DAN_CnC_Crate::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Commands->Destroy_Object(Commands->Find_Object(VehBlock));
		// Holds whether or not each powerup is enabled!
		const int Params[] = {Get_Int_Parameter("Points"),			// 0<- 'ID' of powerup
								Get_Int_Parameter("Death_Crate"),	// 1
								Get_Int_Parameter("Stealth_Suit"),	// 2
								Get_Int_Parameter("DeHealth"),		// 3
								Get_Int_Parameter("DeArmour"),		// 4
								Get_Int_Parameter("Full_Health"),	// 5
								Get_Int_Parameter("Money"),			// 6
								Get_Int_Parameter("Spy_Crate"),		// 7
								Get_Int_Parameter("Random_Weapon"),	// 8
								Get_Int_Parameter("Theif")};		// 9
		// The chance of each Powerup occuring. The higher,the more likely
		const int Chance[] = {4,			// 0
							 4,				// 1
							 2,				// 2
							 5,				// 3
							 5,				// 4
							 4,				// 5
							 4,				// 6
							 1,				// 7
							 5,				// 8
							 4};			// 9
		int y = 0;
		int Total = 0;
		int Number[49]; // This number MUST be the sum of what Chance[] holds.
		// This loop cycles through Params[] and looks for enabled Powerups
		// It then puts the powerup 'ID' in Number as many times as the number in Chance[]
		memset(Number,0,sizeof(Number));
		for (int x = 0; x < 9; x++)
		{
			if (Params[x] >= 1)
			{
				for (int z = 0; z <= Chance[x]; z++)
				{
					Number[y] = x;
					y++;
					Total++;
				}
			}
		}
		int RandNum = Commands->Get_Random_Int(1,(Total + 1));
		const int RandParam = Commands->Get_Random_Int(1,3);
		// Just add the function on the end when you want to add a new powerup!
		switch (Number[RandNum])
		{
			case 0:
				Points(sender,RandParam);
				break;
			case 1:
				DeathCrate(obj);
				break;
			case 2:
				StealthSuit(sender);
				break;
			case 3:
				DeHealth(sender);
				break;
			case 4:
				DeArmour(sender);
				break;
			case 5:
				FullHealth(sender);
				break;
			case 6:
				Money(sender,RandParam);
				break;
			case 7:
				SpyCrate(sender);
				break;
			case 8:
				RandomWeapon(sender);
				break;
			case 9:
				Theif(sender);
				break;
			default:
				Console_Output("[Crate Script] Error: Non-Existant Function called!");
				break;
		}
	}
}

void DAN_CnC_Crate::Points(GameObject *obj,int RandNum)
{
	char *Team = 0;
	if (!Commands->Get_Player_Type(obj))
	{
		Team = "Nod";
	}
	else if (Commands->Get_Player_Type(obj) == 1)
	{
		Team = "GDI";
	}
	char Text[512];
	int Num = Commands->Get_Random_Int(1,7);
	switch (RandNum)
	{
		case 1:// Give points
			if ((1 <= Num) && (Num < 4))
			{
				Commands->Give_Points(obj,100,true);
				sprintf(Text,"MESSAGE [Crate] Enjoy those 100 Points, %s",Team);
				Console_Input(Text);
			}
			if ((4 <= Num) && (Num < 6))
			{
				Commands->Give_Points(obj,500,true);
				sprintf(Text,"MESSAGE [Crate] Enjoy those 500 points, %s",Team);
				Console_Input(Text);
			}
			if (Num == 6)
			{
				Commands->Give_Points(obj,1000,true);
				sprintf(Text,"MESSAGE [Crate] Enjoy those 1000 points, %s",Team);
				Console_Input(Text);
			}
			break;
		case 2:// Take Points
			if ((1 <= Num) && (Num < 4))
			{
				Commands->Give_Points(obj,-100,true);
				sprintf(Text,"MESSAGE [Crate] Hope you dont miss those 100 points, %s",Team);
				Console_Input(Text);
			}
			if ((4 <= Num) && (Num < 6))
			{
				Commands->Give_Points(obj,-500,true);
				sprintf(Text,"MESSAGE [Crate] Hope you dont miss those 500 points, %s",Team);
				Console_Input(Text);
			}
			if (Num == 6)
			{
				Commands->Give_Points(obj,-1000,true);
				sprintf(Text,"MESSAGE [Crate] Hope you dont miss those 1000 points, %s",Team);
				Console_Input(Text);
			}
			break;
	}
}

// The following 4 scripts are pretty much self-explanatory.
void DAN_CnC_Crate::DeathCrate(GameObject *obj)
{
	char *Team = 0;
	if (!Commands->Get_Player_Type(obj))
	{
		Team = "Nod";
	}
	else if (Commands->Get_Player_Type(obj) == 1)
	{
		Team = "GDI";
	}
	char Text[512];
	Commands->Create_Object("Beacon_Ion_Cannon_Anim_Post",Commands->Get_Position(obj));
	Commands->Apply_Damage(obj,1000,"Death",0);
	sprintf(Text,"MESSAGE [Crate] Some poor %s guy just got owned by the Death Crate!",Team);
	Console_Input(Text);
}

void DAN_CnC_Crate::DeArmour(GameObject *obj)
{
	char Text[512];
	int PlayerID;
	PlayerID = Get_Player_ID(obj);
	if (Commands->Get_Shield_Strength(obj) > 1)
	{
		Commands->Set_Shield_Strength(obj,1);
		sprintf(Text,"PPAGE %d [Crate] You have got the DeArmour Crate!",PlayerID);
		Console_Input(Text);
		sprintf(Text,"PPAGE %d [Crate] Your Armour has been set to 1!",PlayerID);
		Console_Input(Text);
	}
	else
		DeHealth(obj);
}

void DAN_CnC_Crate::DeHealth(GameObject *obj)
{
	char Text[512];
	int PlayerID;
	PlayerID = Get_Player_ID(obj);
	Commands->Set_Health(obj,1);
	sprintf(Text,"PPAGE %d [Crate] You have got the DeHealth Crate!",PlayerID);
	Console_Input(Text);
	sprintf(Text,"PPAGE %d [Crate] Your Health has been set to 1!",PlayerID);
	Console_Input(Text);
}

void DAN_CnC_Crate::FullHealth(GameObject *obj)
{
	char Text[512];
	int PlayerID;
	PlayerID = Get_Player_ID(obj);
	Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
	Commands->Set_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj));
	sprintf(Text,"PPAGE %d [Crate] You have got the Full Health Crate!",PlayerID);
	Console_Input(Text);
	sprintf(Text,"PPAGE %d [Crate] Your Health and Armour has been replenished!",PlayerID);
	Console_Input(Text);
}

void DAN_CnC_Crate::Money(GameObject *obj,int RandNum)
{
	int PlayerID;
	PlayerID = Get_Player_ID(obj);
	char Text[512];
	const int Num = Commands->Get_Random_Int(1,7);
	switch (RandNum)
	{
		case 1:
			if ((1 <= Num) && (Num < 4))
			{
				Commands->Give_Money(obj,100,false);
				sprintf(Text,"PPAGE %d [Crate] You have got 100 Credits from the MoneyCrate!",PlayerID);
				Console_Input(Text);
			}
			if ((4 <= Num) && (Num < 6))
			{
				Commands->Give_Money(obj,500,false);
				sprintf(Text,"PPAGE %d [Crate] You have got 500 Credits from the MoneyCrate!",PlayerID);
				Console_Input(Text);
			}
			if (Num == 6)
			{
				Commands->Give_Money(obj,1000,false);
				sprintf(Text,"PPAGE %d [Crate] You have got 1000 Credits from the MoneyCrate!",PlayerID);
				Console_Input(Text);
			}
			break;
		case 2:
			if ((1 <= Num) && (Num < 4))
			{
				Commands->Give_Money(obj,-100,false);
				sprintf(Text,"PPAGE %d [Crate] You have lost 100 Credits from the MoneyCrate!",PlayerID);
				Console_Input(Text);
			}
			if ((4 <= Num) && (Num < 6))
			{
				Commands->Give_Money(obj,-500,false);
				sprintf(Text,"PPAGE %d [Crate] You have lost 500 Credits from the MoneyCrate!",PlayerID);
				Console_Input(Text);
			}
			if (Num == 6)
			{
				Commands->Give_Money(obj,-1000,false);
				sprintf(Text,"PPAGE %d [Crate] You have lost 1000 Credits from the MoneyCrate!",PlayerID);
				Console_Input(Text);
			}
			break;
	}
}

void DAN_CnC_Crate::RandomWeapon(GameObject *obj)
{
	int PlayerID;
	PlayerID = Get_Player_ID(obj);
	char Text[512];
	int RandNum = Commands->Get_Random_Int(1,19);
	char *Weapon;
	Weapon = "Random Weapon";
	switch (RandNum)
	{
		case 1:
			Grant_Powerup(obj,"CnC_POW_AutoRifle_Player_GDI");
			Weapon = "AutoRifle";
			break;
		case 2:
			Grant_Powerup(obj,"POW_Chaingun_Player");
			Weapon = "ChainGun";
			break;
		case 3:
			Grant_Powerup(obj,"POW_ChemSprayer_Player");
			Weapon = "Chem Sprayer";
			break;
		case 4:
			Grant_Powerup(obj,"POW_Flamethrower_Player");
			Weapon = "FlameThrower";
			break;
		case 5:
			Grant_Powerup(obj,"POW_GrenadeLauncher_Player");
			Weapon = "Grenade Launcher";
			break;
		case 6:
			Grant_Powerup(obj,"POW_LaserChaingun_Player");
			Weapon = "Laser Chaingun";
			break;
		case 7:
			Grant_Powerup(obj,"POW_LaserRifle_Player");
			Weapon = "Laser Rifle";
			break;
		case 8:
			Grant_Powerup(obj,"POW_MineProximity_Player");
			Weapon = "Proximity Mine";
			break;
		case 9:
			Grant_Powerup(obj,"POW_MineRemote_Player");
			Weapon = "Remote Mine";
			break;
		case 10:
			Grant_Powerup(obj,"POW_MineTimed_Player");
			Weapon = "Timed C4";
			break;
		case 11:
			Grant_Powerup(obj,"POW_PersonalIonCannon_Player");
			Weapon = "Personal Ion Cannon";
			break;
		case 12:
			Grant_Powerup(obj,"POW_Railgun_Player");
			Weapon = "RailGun";
			break;
		case 13:
			Grant_Powerup(obj,"POW_RamjetRifle_Player");
			Weapon = "Ramjet Rifle";
			break;
		case 14:
			Grant_Powerup(obj,"POW_RocketLauncher_Player");
			Weapon = "RocketLauncher";
			break;
		case 15:
			Grant_Powerup(obj,"POW_Shotgun_Player");
			Weapon = "Shotgun";
			break;
		case 16:
			Grant_Powerup(obj,"POW_SniperRifle_Player");
			Weapon = "Sniper Rifle";
			break;
		case 17:
			Grant_Powerup(obj,"POW_TiberiumAutoRifle_Player");
			Weapon = "Tiberium AutoRifle";
			break;
		case 18:
			Grant_Powerup(obj,"POW_TiberiumFlechetteGun_Player");
			Weapon = "Tiberium Flechette Gun";
			break;
	}
	sprintf(Text,"PPAGE %d [Crate] You have been given a %s from the Weapons Locker!",PlayerID,Weapon);
	Console_Input(Text);
}

void DAN_CnC_Crate::SpyCrate(GameObject *obj)
{
	char *Team = 0;
	char *OtherTeam = 0;
	if (!Commands->Get_Player_Type(obj))
	{
		Team = "Nod";
		OtherTeam = "GDI";
	}
	else if (Commands->Get_Player_Type(obj) == 1)
	{
		Team = "GDI";
		OtherTeam = "Nod";
	}
	char Text[512];
	Commands->Set_Is_Visible(obj,false);
	sprintf(Text,"MESSAGE [Crate] Some %s guy got the Spy Crate! Better watch your base, %s",Team,OtherTeam);
	Console_Input(Text);
}

void DAN_CnC_Crate::StealthSuit(GameObject *obj)
{
	char *Team = 0;
	if (!Commands->Get_Player_Type(obj))
	{
		Team = "Nod";
	}
	else if (Commands->Get_Player_Type(obj) == 1)
	{
		Team = "GDI";
	}
	char Text[512];
	Commands->Enable_Stealth(obj,true);
	sprintf(Text,"MESSAGE [Crate] Some %s guy found the Stealth Suit!",Team);
	Console_Input(Text);
}

void DAN_CnC_Crate::Theif (GameObject *obj)
{
	char *Team = 0;
	if (!Commands->Get_Player_Type(obj))
	{
		Team = "Nod";
	}
	else if (Commands->Get_Player_Type(obj) == 1)
	{
		Team = "GDI";
	}
	char Text[512];
	Commands->Give_Money(obj,(Commands->Get_Money(obj)*-1),false);
	sprintf(Text,"MESSAGE [Crate] Awwww, some poor %s guy lost all his money! Pwnt!",Team);
	Console_Input(Text);
}

void DAN_CnC_Crate::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&VehBlock,4,1);
}

void DAN_Drop_Wreckage_On_Death::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *CurTank;
	CurTank = Commands->Create_Object(Get_Parameter("Wreckage_Preset"),Commands->Get_Position(obj));
	Commands->Set_Facing(CurTank,Commands->Get_Facing(obj));
	Commands->Attach_Script(CurTank,"DAN_Wreckage_Rebuildable",Commands->Get_Preset_Name(obj));
}

void DAN_Wreckage_Rebuildable::Created(GameObject *obj)
{
	Commands->Set_Player_Type(obj,-2);
	Commands->Set_Health(obj,50);
	Commands->Set_Shield_Strength(obj,0);
}

void DAN_Wreckage_Rebuildable::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (amount < 0)
	{
		float TotalHealth = Commands->Get_Health(obj) + Commands->Get_Shield_Strength(obj);
		const float MaxHealth = Commands->Get_Max_Health(obj) + Commands->Get_Max_Shield_Strength(obj);
		Vector3 CurPosition = Commands->Get_Position(obj);
		if (TotalHealth == MaxHealth)
		{
			GameObject *CurTank = Commands->Create_Object(Get_Parameter("Vehicle_Preset"),Commands->Get_Position(obj));
			Commands->Set_Facing(CurTank,Commands->Get_Facing(obj));
			Commands->Set_Health(CurTank,5);
			Commands->Set_Shield_Strength(CurTank,0);
			Commands->Set_Player_Type(CurTank,Commands->Get_Player_Type(damager));
			CurPosition.Z += 1;
			Commands->Set_Position(CurTank,CurPosition);
			Commands->Destroy_Object(obj); 
		}
	}
}

ScriptRegistrant<DAN_CnC_Crate> DAN_CnC_Crate_Registrant("DAN_CnC_Crate","Death_Crate:int,Stealth_Suit:int,DeHealth:int,DeArmour:int,Full_Health:int,Money:int,Spy_Crate:int,Random_Weapon:int,Points:int,Theif:int");
ScriptRegistrant<DAN_Drop_Wreckage_On_Death> DAN_Drop_Wreckage_On_Death_Registrant("DAN_Drop_Wreckage_On_Death","Wreckage_Preset:string");
ScriptRegistrant<DAN_Wreckage_Rebuildable> DAN_Wreckage_Rebuildable_Registrant("DAN_Wreckage_Rebuildable","Vehicle_Preset:string");
