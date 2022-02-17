/*	Renegade Scripts.dll
	Renegade Role Play 2 Scripts
	Copyright 2012 Jerad Gray

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
#include "SoldierGameObj.h"
#include "WeaponBagClass.h"
#include "WeaponMgr.h"
#include "GameObjManager.h"
#include "MoveablePhysClass.h"
#include "engine_tt.h"
#include "PhysicsSceneClass.h"
#include "VehicleGameObj.h"

#include "jmgUtility.h"
#include "jmgMetroid.h"

void MetroidGame::WinMap()
{
	Commands->Enable_Spawner(407702,true);
	MainElevatorDown = false;
	BossDefeated = true;
	GameObject *ElevatorControl = Commands->Find_Object(403583);
	if (ElevatorControl)
		Commands->Send_Custom_Event(ElevatorControl,ElevatorControl,403025,0,0);
	GameObject *ShieldWall = Commands->Find_Object(FinalCheckpointShieldWallID);
	if (ShieldWall)
		Commands->Destroy_Object(ShieldWall);
	ShieldWall = Commands->Create_Object("Simple_Sydney_SandM_Wall",Vector3(344.817f,-435.808f,-0.887f));
	FinalCheckpointShieldWallID = Commands->Get_ID(ShieldWall);
	MetroidSabotagedCrateSystemControl.RecreateCrates();
	Commands->Static_Anim_Phys_Goto_Frame(456473,0.0f,"MineElevator.MineElevator");
	Commands->Static_Anim_Phys_Goto_Frame(458415,0.0f,"MineElevatorKZ.MineElevatorKZ");
	NumberOfDeactivatedTerminals = 0;
	BossID = 0;
	BossMode = 0;
	if (!winMusicId)
	{
		GameObject *winMusic = Commands->Create_Object("Daves Arrow",Vector3(353.3f,-406.2f,2.17f));
		Commands->Set_Model(winMusic,"s_victory");
		winMusicId = Commands->Get_ID(winMusic);
	}
	for (int x = 0;x < 3;x++)
	{
		GameObject *Terminal = Commands->Find_Object(MineTerminalID[x]);
		if (Terminal)
		{
			Commands->Enable_HUD_Pokable_Indicator(Terminal,true);
			Commands->Set_Animation(Terminal,"jg_computer.jg_computer",false,0,0.0f,0.0f,false);
		}
		MineTerminalDeactivated[x] = false;
		MineTerminalReset[x] = 0;
	}
	for (int x = 0;x < METROIDOBJECTIVECOUNT;x++)
		DisplayedObjective[x] = false;
	for (int x = 1;x < 128;x++)
	{
		GameObject *Player = Get_GameObj(x);
		if (!Player)
			continue;
		if (JmgUtility::SimpleDistance(Commands->Get_Position(Player),Vector3(-1750.0f,-350.0f,0.0f)) <= 62500.0f)
		{// Move player out of snow area
			DisplayDialogMessage(Player,20);
			Vector3 TargetPos = Vector3(360.0f,-410.0f,8.5f);
			MoveablePhysClass *mphys = Player->As_PhysicalGameObj() ? Player->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
			if (mphys && mphys->Find_Teleport_Location(TargetPos,10.0f,&TargetPos))
				Commands->Set_Position(Player,TargetPos);
			continue;
		}
		if (JmgUtility::SimpleDistance(Commands->Get_Position(Player),Vector3(1000.0f,500.0f,0.0f)) > 360000.0f)
			continue;
		PerkSystemIncreasePlayerPerkUnlockAmount(Player,73,1);
		MetroidPSSControl.PlayerSettings[x]->ChapterCompleted(Player,8);
		DisplayDialogMessage(Player,19);
		Vector3 TargetPos = Vector3(360.0f,-410.0f,8.5f);
		MoveablePhysClass *mphys = Player->As_PhysicalGameObj() ? Player->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
		if (mphys && mphys->Find_Teleport_Location(TargetPos,10.0f,&TargetPos))
			Commands->Set_Position(Player,TargetPos);
		SavePlayerPerkData();
		GameObject *BossMusic = Commands->Find_Object(MetroidGameControl.BossDefeatedMusicID);
		if (BossMusic)
			Commands->Destroy_Object(BossMusic);
		for (int y = 0;y < 128;y++)
			DefenseModeSystemControl.DefensePlayerDataNodes[y].TelepadID = 0;
		DefenseModeSystemControl.RemoveAllButSpecial();
		MetroidRoomObjectives.Set_Objective_Status(7,NewObjectiveSystem::Accomplished);
	}
	if (!JMG_Metroid_Game_Control::gameRemainingTime)
		JMG_Metroid_Game_Control::gameRemainingTime = 910;
}
void JMG_Metroid_Hide_On_Enter::Entered(GameObject *obj,GameObject *enter)
{
	Commands->Set_Model(enter,"RP2InvisSpawn");
}
void JMG_Metroid_Gibs_On_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (!enter->As_SoldierGameObj())
		return;
	if (!_stricmp(Get_Model(enter),"RP2InvisSpawn"))
		return;
	char splatter[32];
	sprintf(splatter,"fgibs%d",Commands->Get_Random_Int(0,6));
	Commands->Create_Explosion(splatter,Commands->Get_Position(enter),0);
	Commands->Set_Model(enter,"RP2InvisSpawn");
	if (Commands->Is_A_Star(enter))
	{
		char DeathMSG[512];
		sprintf(DeathMSG,"%s killed %s by %s.",Get_Player_Name(enter),JmgUtility::JMG_Get_Units_Sex(enter),Get_Parameter("DeathMessage"));
		JmgUtility::JMG_Player_Death_Message(enter,NULL,DeathMSG);
		Set_Screen_Fade_Color_Player(enter,0.75,0,0,0.1f);
		Set_Screen_Fade_Opacity_Player(enter,0.75f,0.1f);
		if (Get_Int_Parameter("PerkID"))
			PerkSystemIncreasePlayerPerkUnlockAmount(enter,Get_Int_Parameter("PerkID"),1);
	}
	Commands->Apply_Damage(enter,9999.9f,"Blamokiller",0);
}
void JMG_Metroid_Increase_Perk_Unlock_On_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (!Commands->Is_A_Star(enter) || !Get_Int_Parameter("PerkID"))
		return;
	PerkSystemIncreasePlayerPerkUnlockAmount(enter,Get_Int_Parameter("PerkID"),1);
}
void JMG_Metroid_HUD_Message_On_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (MetroidGameControl.DisplayedObjective[Get_Int_Parameter("StringID")])
		return;
	MetroidGameControl.DisplayedObjective[Get_Int_Parameter("StringID")] = true;
	if (Get_Float_Parameter("Delay") <= 0.0f)
		MetroidGameControl.DisplayHUDMessage(Get_Int_Parameter("StringID"));
	else
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
}
void JMG_Metroid_HUD_Message_On_Enter::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		MetroidGameControl.DisplayHUDMessage(Get_Int_Parameter("StringID"));
	}
}
void JMG_Metroid_Game_Initilization::Created(GameObject *obj)
{
	FILE *file = fopen("Data/Rp2ServerSettings.ini","r");
	if (file)
	{
		JMG_Metroid_Game_Initilization::IsRp2Hosted = true;
		fclose(file);
	}
	else
		JMG_Metroid_Game_Initilization::IsRp2Hosted = false;
	for (int x = 0;x < 128;x++)
		SupportModeSystemControl[x] = SupportModeSystem();
}
void JMG_Metroid_Game_Initilization::Destroyed(GameObject *obj)
{
	MetroidEquipmentSystemControl.EmptyList();
	AmbushPointSystemControl.EmptyList();
	MetroidBossPathControlSystem.EmptyList();
	MetroidPowerupSystemControl.EmptyList();
	MetroidPSSControl.EmptyList();
	MetroidSabotagedCrateSystemControl.EmptyList();
	DefenseModeSystemControl.EmptyList();
	for (int x = 0;x < 128;x++)
	{
		SupportModeSystemControl[x].EmptyList();
		SupplyModeSystemControl[x].EmptyList();
	}
	for (int x = 0;x < LOCKDOWNZONECOUNT;x++)
		RoomLockdownControl[x].EmptyList();
	MetroidRandomSpawnControl.EmptyList();
	MetroidGameControl = MetroidGame();
	MetroidScoreControl.Cleanup();
	MetroidObjectiveSystemControl.~NewObjectiveSystem();
	MetroidRoomObjectives.~NewObjectiveSystem();
}
JMG_Metroid_Game_Control::JMG_Metroid_Game_Control()
{
	for (int x = 0;x < 128;x++)
	{
		briefingText[x] = NULL;
		currentNode[x] = NULL;
	}
	mPlayerData = MetroidTempPlayerData();
	MetroidPSSControl = MetroidPlayerSettingsSystem();
}
void JMG_Metroid_Game_Control::Created(GameObject *obj)
{
	JMG_Metroid_Game_Control::gameRemainingTime = 0;
	MetroidObjectiveSystemControl.Add_Objective(1,NewObjectiveSystem::Primary,NewObjectiveSystem::Pending,12781,"",12781,Commands->Get_Position(Commands->Find_Object(100126)));
	MetroidRoomObjectives.Add_Objective(1,NewObjectiveSystem::Primary,NewObjectiveSystem::Pending,0,"",0,Commands->Get_Position(Commands->Find_Object(100126)));
	JMG_Metroid_Game_Control::currentBriefingId = 12766;
	for (int x = 1;x < 128;x++)
	{
		if (!Get_GameObj(x))
			continue;
		JMG_Metroid_Game_Control::ParseBriefingString(Get_Translated_String(JMG_Metroid_Game_Control::currentBriefingId),x);
	}
	MetroidScoreControl.LoadData();
	lastPlayerCount = 0;
	AddPerkTypes();
	Commands->Attach_Script(obj,"JMG_Metroid_Game_Initilization","");
	Attach_Script_Once(obj,"JMG_Utility_Sync_System_Controller","1.0");
	GameObject *ShieldWall = Commands->Create_Object("Simple_Sydney_SandM_Wall",Vector3(344.817f,-435.808f,-0.887f));
	MetroidGameControl.FinalCheckpointShieldWallID = Commands->Get_ID(ShieldWall);
	Commands->Start_Timer(obj,this,0.25f,42352355);
	Commands->Start_Timer(obj,this,10.0f,42352356);
	Commands->Start_Timer(obj,this,10.0f,42352357);
	Commands->Start_Timer(obj,this,300.0f,42352358);
	Commands->Start_Timer(obj,this,1.0f,1);
	Commands->Start_Timer(obj,this,6.0f,2);
}
void JMG_Metroid_Game_Control::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		int tempPlayerCount = Get_Player_Count();
		if (lastPlayerCount != tempPlayerCount)
		{
			lastPlayerCount = tempPlayerCount;
			JMG_Metroid_Game_Control::EnemyShieldRegenStartTime = (int)JmgUtility::MathClamp(5.5f-tempPlayerCount*0.5f,1.0f,5.0f);
		}
		IncreasePlayerTimeScores();
		if (gameRemainingTime)
		{
			gameRemainingTime--;
			char timeMsg[220];
			if ((gameRemainingTime >= 300 && !(gameRemainingTime % 300)) || (!(gameRemainingTime % 60) && gameRemainingTime > 0 && gameRemainingTime < 300))
			{
				JmgUtility::MessageAllPlayers(127,127,255,"===============================================");
				sprintf(timeMsg,"The map has been won! The round will end in %d minute%s",gameRemainingTime/60,gameRemainingTime/60 == 1 ? "!" : "s!");
				JmgUtility::MessageAllPlayers(127,127,255,timeMsg);
				JmgUtility::MessageAllPlayers(127,127,255,"===============================================");
			}
			if (gameRemainingTime == 30 || gameRemainingTime == 15 || gameRemainingTime == 10)
			{
				JmgUtility::MessageAllPlayers(127,127,255,"===============================================");
				sprintf(timeMsg,"The map has been won! The round will end in %d seconds!",gameRemainingTime);
				JmgUtility::MessageAllPlayers(127,127,255,timeMsg);
				JmgUtility::MessageAllPlayers(127,127,255,"===============================================");
			}
			if (gameRemainingTime == 5)
				JmgUtility::MessageAllPlayers(127,127,255,"===============================================");
			if (gameRemainingTime && gameRemainingTime <= 5)
			{
				sprintf(timeMsg,"%d",gameRemainingTime);
				JmgUtility::MessageAllPlayers(127,127,255,timeMsg);
			}
			if (!gameRemainingTime)
				Console_Input("win 1");
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
	if (number == 2)
	{
		for (int x = 1;x < 128;x++)
		{
			if (!currentNode[x])
				if (briefingText[x])
					currentNode[x] = briefingText[x];
				else
					continue;
			if (currentNode[x]->Text[0] != '\0')
			{
				GameObject *Player = Get_GameObj(x);
				if (Player)
					JmgUtility::DisplayChatMessage(Player,0,200,255,currentNode[x]->Text);
			}
			currentNode[x] = currentNode[x]->next;
			if (!currentNode[x])
				RemoveTextNodes(x);
		}
		Commands->Start_Timer(obj,this,6.0f,2);
	}
	if (42352355 == number)
	{
		if (The_Game()->Is_Game_Over())
		{
			SavePlayerPerkData();
			SaveMetroidHighScores();
			Commands->Destroy_Object(obj);
			return;
		}
		if (MetroidPSSControl.PlayerSettings[0]->IsInTeleport)
			MetroidPSSControl.PlayerSettings[0]->IsInTeleport--;
		Commands->Start_Timer(obj,this,0.25f,42352355);
	}
	if (number == 42352356)
	{
		AmbushPointSystemControl.CheckForErrors();
	}
	if (42352357 == number)
	{
		MetroidScoreControl.StateHighScore();
		Commands->Start_Timer(obj,this,60.0f,42352357);
	}
	if (42352358 == number)
	{
		SaveMetroidHighScores();
		Commands->Start_Timer(obj,this,300.0f,42352358);
	}
}
void JMG_Metroid_Game_Control::Destroyed(GameObject *obj)
{
	for (int x = 0;x < 128;x++)
		RemoveTextNodes(x);
}
void JMG_Metroid_Game_Control::IncreasePlayerTimeScores()
{
	JmgUtility::GenericDateTime currentTime = JmgUtility::GenericDateTime();
	for (int y = 1;y < 128;y++)
	{
		GameObject *Player = Get_GameObj(y);
		if (!Player)
			continue;
		MetroidScoreSystem::MetroidHighScoresNode *pobj = MetroidScoreControl.MetroidPlayerHighScoreNodes[y];	
		if (!pobj)
			continue;
		Vector3 Pos = Commands->Get_Position(Player);
		float Facing = Commands->Get_Facing(Player);
		bool IsMoving = true,IsTurning = true;
		if (!JmgUtility::SimpleDistance(Pos,pobj->LastPos))
			IsMoving = false;
		pobj->LastPos = Pos;
		if (abs(Facing - pobj->LastFacing) < 1.0f)
			IsTurning = false;
		pobj->LastFacing = Facing;
		pobj->LastPlayTime = currentTime;
		if (pobj->IsMoving == IsMoving && pobj->IsTurning == IsTurning)
			if (pobj->IdleDelay > 60)
			{
				pobj->IdleTime++;
				continue;
			}
			else
				pobj->IdleDelay++;
		else
			pobj->IdleDelay = 0;
		pobj->PlayTime++;
		pobj->IsMoving = IsMoving;
		pobj->IsTurning = IsTurning;
		if (MetroidPSSControl.PlayerSettings[y]->InTrainingRoom)
		{
			pobj->PlayTimeInSpawnRoom++;
			pobj->PlayTimeAsClass11++;
		}
		else
		{
			pobj->playTimeThisRound++;
			switch (MetroidPSSControl.PlayerSettings[y]->ArmorMode)
			{
			case 0:pobj->PlayTimeAsClass1++;break;
			case 1:pobj->PlayTimeAsClass2++;break;
			case 2:pobj->PlayTimeAsClass3++;break;
			case 3:pobj->PlayTimeAsClass4++;break;
			case 4:pobj->PlayTimeAsClass5++;break;
			case 5:pobj->PlayTimeAsClass6++;break;
			case 6:pobj->PlayTimeAsClass7++;break;
			case 7:pobj->PlayTimeAsClass8++;break;
			case 8:pobj->PlayTimeAsClass9++;break;
			case 9:pobj->PlayTimeAsClass10++;break;
			}
		}
	}
}
void JMG_Metroid_Game_Control::ParseBriefingString(const char *text,int playerId)
{
	RemoveTextNodes(playerId);
	AddNewTextNode(playerId);
	int length = strlen(text);
	char TextString[16384];
	sprintf(TextString,"%s",text);
	int Count = 0;
	for (int x = 0;x < length;x++)
	{
		if ((Count > 150 && TextString[x] == ' ') || Count == 220)
		{// String getting too long, start a new one.
			currentNode[playerId]->Text[Count] = '\0';
			AddNewTextNode(playerId);
			Count = 0;
		}
		else if (TextString[x] == '\n' || TextString[x] == '\0')
		{
			currentNode[playerId]->Text[Count] = '\0';
			break;// end of line your done
		}
		else if (TextString[x] == '\f')
		{
			currentNode[playerId]->Text[Count] = '\0';
			AddNewTextNode(playerId);
			Count = 0;
		}
		else
		{
			currentNode[playerId]->Text[Count] = TextString[x];
			Count++;
			currentNode[playerId]->Text[Count] = '\0';
		}
	}
	TextString[0] = '\0';
	currentNode[playerId] = briefingText[playerId];
}
void JMG_Metroid_Game_Control::AddNewTextNode(int playerId)
{
	if (!briefingText[playerId])
	{
		briefingText[playerId] = new BriefingTextNode;
		sprintf(briefingText[playerId]->Text,"\0");
		briefingText[playerId]->next = NULL;
		currentNode[playerId] = briefingText[playerId];
		return;
	}
	BriefingTextNode *Current = briefingText[playerId];
	while (Current)
	{
		if (!Current->next)
		{
			Current->next = new BriefingTextNode;
			Current = Current->next;
			sprintf(Current->Text,"\0");
			Current->next = NULL;
			currentNode[playerId] = Current;
			return;
		}
		Current = Current->next;
	}
}
void JMG_Metroid_Game_Control::RemoveTextNodes(int playerId)
{
	BriefingTextNode *Current = briefingText[playerId],*Temp;
	while (Current)
	{
		Temp = Current;
		Current = Current->next;
		delete Temp;
	}
	briefingText[playerId] = NULL;
	currentNode[playerId] = NULL;
}
void JMG_Metroid_Character_Control::Created(GameObject *obj)
{
	PlayerID = JmgUtility::JMG_Get_Player_ID(obj);
	if (!Commands->Is_A_Star(obj))
		return;
	MetroidScoreControl.Get_Current_Player_Score_Node(PlayerID);
	//Commands->Set_Position(Get_GameObj(1),Vector3(-1295.915f,-959.524f,12.195f));
	Commands->Attach_Script(obj,"JMG_Metroid_ZeroBug_Tracker","");
	MetroidGameControl.CheckForPlayerCountUpdate();
	mPlayerData.getPlayerNode(obj);
	if (!mPlayerData.players[PlayerID]->destroyTime || difftime(clock(),mPlayerData.players[PlayerID]->destroyTime) > 500)
	{
		if (Commands->Get_Preset_ID(obj) == 81930243 || Commands->Get_Preset_ID(obj) == 81930257)
		{
			JMG_Metroid_Game_Control::ParseBriefingString(Get_Translated_String(JMG_Metroid_Game_Control::currentBriefingId),PlayerID);
			MetroidObjectiveSystemControl.Display_All_Objectives(obj);
			int objectiveId = MetroidObjectiveSystemControl.Get_First_Pending_Objective_Of_Priority(NewObjectiveSystem::Primary);
			int currentObjectiveText = MetroidObjectiveSystemControl.Get_Mission_Text_Id(objectiveId);
			if (currentObjectiveText)
				Set_HUD_Help_Text_Player(obj,currentObjectiveText,Vector3(0,1,0));
		}
	}
	MetroidPSSControl.PlayerSettings[PlayerID]->ScreenColorZone = 0;
	Set_Screen_Fade_Color_Player(obj,0,0,0,0);
	Set_Screen_Fade_Opacity_Player(obj,0.0f,0);
	MetroidPSSControl.PlayerSettings[PlayerID]->ScreenColor = Vector3(0.0f,0.0f,0.0f);
	MetroidPSSControl.PlayerSettings[PlayerID]->ScreenOpacity = 0.0f;
	MetroidPSSControl.SpawnPlayer(PlayerID);
	MetroidPSSControl.PlayerSettings[PlayerID]->InStartRoom = true;
	MetroidPSSControl.PlayerSettings[PlayerID]->InTrainingRoom = false;
	JmgUtility::hasStatedDeathMessage[PlayerID] = false;
	Set_Screen_Fade_Color_Player(obj,0.0,0,0,0.1f);
	Set_Screen_Fade_Opacity_Player(obj,0.0f,0.1f);
	Commands->Attach_Script(obj,"JMG_Metroid_Screen_Fade_Red_On_Damage","");	
	FindPlayerPerkList(obj,false);
	Commands->Set_Player_Type(obj,1);
	char KeySetup[128];
	sprintf(KeySetup,"Vote_Up,%d",4073491);
	Commands->Attach_Script(obj,"JMG_Keyboard_Input_Custom",KeySetup);	
	sprintf(KeySetup,"Vote_Down,%d",4073490);
	Commands->Attach_Script(obj,"JMG_Keyboard_Input_Custom",KeySetup);
	if (PlayerID)
		switch (Commands->Get_Preset_ID(obj))
		{
			case 81930090:MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode = 0;break;
			case 81930081:MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode = 1;break;
			case 81930091:MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode = 2;break;
			case 81930092:MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode = 3;break;
			case 81930099:MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode = 4;break;
			case 81930106:MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode = 5;break;
			case 81930082:MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode = 9;break;
			case 81930243:case 81930257:case 81930103:break;
			default:
				sprintf(KeySetup,"msg JMG_Metroid_Character_Control ERROR: Undefined soldier type %d!",Commands->Get_Preset_ID(obj));
				Console_Input(KeySetup);
				break;	
		}
	Commands->Start_Timer(obj,this,0.25f,1);
	Commands->Start_Timer(obj,this,0.25f,2);
	Commands->Start_Timer(obj,this,0.1f,3);
	Commands->Start_Timer(obj,this,0.25f,4);
}
void JMG_Metroid_Character_Control::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		ClearBeacons(obj,MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode);
		Commands->Attach_Script(obj,"JMG_Keyboard_Input_Custom","Numpad0,940860500");
		Commands->Attach_Script(obj,"JMG_Keyboard_Input_Custom","Numpad1,940860501");
		Commands->Attach_Script(obj,"JMG_Keyboard_Input_Custom","Numpad2,940860502");
		Commands->Attach_Script(obj,"JMG_Keyboard_Input_Custom","Numpad3,940860503");
		Commands->Attach_Script(obj,"JMG_Keyboard_Input_Custom","Numpad4,940860504");
		Commands->Attach_Script(obj,"JMG_Keyboard_Input_Custom","Numpad5,940860505");
		Commands->Attach_Script(obj,"JMG_Keyboard_Input_Custom","Numpad6,940860506");
		Commands->Attach_Script(obj,"JMG_Keyboard_Input_Custom","Numpad7,940860507");
		Commands->Attach_Script(obj,"JMG_Keyboard_Input_Custom","Numpad8,940860508");
		Commands->Attach_Script(obj,"JMG_Keyboard_Input_Custom","Numpad9,940860509");
		Commands->Attach_Script(obj,"JMG_Keyboard_Input_Custom","FlashLight,7745471");	
	}
	if (2 == number)
	{
		if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),Vector3(350.0f,-425.0f,0.0f)) > 22500.0f)
			MetroidPSSControl.PlayerSettings[PlayerID]->InStartRoom = false;
		else
			Commands->Start_Timer(obj,this,0.25f,2);
	}
	if (3 == number)
	{
		if (SupportModeSystemControl[PlayerID].LastDamaged)
			SupportModeSystemControl[PlayerID].LastDamaged--;
		if (SupportModeSystemControl[PlayerID].LastFired)
			SupportModeSystemControl[PlayerID].LastFired--;
		int WeaponID = JmgUtility::JMG_Get_Current_Weapon_ID(obj);
		int CurrentBullets = Get_Current_Total_Bullets(obj);
		if (WeaponID == LastWeaponID && CurrentBullets < LastAmmoCount)
			SupportModeSystemControl[PlayerID].LastFired = 50;
		else if (WeaponID != LastWeaponID)
			LastWeaponID = WeaponID;
		LastAmmoCount = CurrentBullets;
		if (MetroidPSSControl.PlayerSettings[PlayerID]->IsInTeleport)
			MetroidPSSControl.PlayerSettings[PlayerID]->IsInTeleport--;
		Commands->Start_Timer(obj,this,0.1f,3);
	}
	if (4 == number)
	{
		if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),Vector3(550.0f,-425.0f,7.5f)) > 22500.0f)
		{
			if (MetroidPSSControl.PlayerSettings[PlayerID]->InTrainingRoom)
			{
				Commands->Send_Custom_Event(obj,obj,2342346,0,0);
				MetroidPSSControl.PlayerSettings[PlayerID]->InTrainingRoom = false;
				switch (MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode)
				{
				case 0:MetroidScoreControl.MetroidPlayerHighScoreNodes[PlayerID]->SpawnedAsCharacter0++;break;
				case 1:MetroidScoreControl.MetroidPlayerHighScoreNodes[PlayerID]->SpawnedAsCharacter1++;break;
				case 2:MetroidScoreControl.MetroidPlayerHighScoreNodes[PlayerID]->SpawnedAsCharacter2++;break;
				case 3:MetroidScoreControl.MetroidPlayerHighScoreNodes[PlayerID]->SpawnedAsCharacter3++;break;
				case 4:MetroidScoreControl.MetroidPlayerHighScoreNodes[PlayerID]->SpawnedAsCharacter4++;break;
				case 5:MetroidScoreControl.MetroidPlayerHighScoreNodes[PlayerID]->SpawnedAsCharacter5++;break;
				case 6:MetroidScoreControl.MetroidPlayerHighScoreNodes[PlayerID]->SpawnedAsCharacter6++;break;
				case 7:MetroidScoreControl.MetroidPlayerHighScoreNodes[PlayerID]->SpawnedAsCharacter7++;break;
				case 8:MetroidScoreControl.MetroidPlayerHighScoreNodes[PlayerID]->SpawnedAsCharacter8++;break;
				case 9:MetroidScoreControl.MetroidPlayerHighScoreNodes[PlayerID]->SpawnedAsCharacter9++;break;
				}
			}
		}
		else if (!MetroidPSSControl.PlayerSettings[PlayerID]->InTrainingRoom)
		{
			ClearBeacons(obj,-2);
			Change_Character(obj,"Training_Mode");
			MetroidPSSControl.PlayerSettings[PlayerID]->InTrainingRoom = true;
		}
		Commands->Start_Timer(obj,this,0.25f,4);
	}
}
void JMG_Metroid_Character_Control::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 7745471)
	{
		MetroidGameControl.WinMap();
	}
	if (message == 2342346)
	{
		ClearBeacons(obj,MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode);
		//if (MetroidPSSControl.PlayerSettings[PlayerID]->InStartRoom)
		//	JmgUtility::DisplayChatMessage(obj,127,255,255,"Press numpad 1-6 to change your player mode.");
		if (!MetroidPSSControl.PlayerSettings[PlayerID]->InStartRoom)
			Commands->Start_Timer(obj,this,0.25f,2);
		MetroidPSSControl.PlayerSettings[PlayerID]->InStartRoom = true;
		if (MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode == -1)
			MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode = Get_Team(PlayerID);
		switch (MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode)
		{
		case 0:Change_Character(obj,"Marine_super");break;
		case 1:Change_Character(obj,"Ally_soldier");break;
		case 2:Change_Character(obj,"FirePower_Mode");break;
		case 3:Change_Character(obj,"Support_Mode");break;
		case 4:Change_Character(obj,"Defense_Mode");break;
		case 5:Change_Character(obj,"Supply_Mode");break;
		case 9:Change_Character(obj,"Cheat_Soldier");break;
		}
		//Change_Character(obj,"Cheat_Soldier");//TODO: remove
	}
	if (message == 940860501 && MetroidPSSControl.PlayerSettings[PlayerID]->InStartRoom && MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode != 0)
	{
		MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode = 0;
		Change_Character(obj,"Marine_super");
		JmgUtility::DisplayChatMessage(obj,127,255,255,"Armor mode selected.");
		Commands->Set_Player_Type(obj,1);
	}
	if (message == 940860502 && MetroidPSSControl.PlayerSettings[PlayerID]->InStartRoom && MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode != 1)
	{
		MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode = 1;
		Change_Character(obj,"Ally_soldier");
		JmgUtility::DisplayChatMessage(obj,127,255,255,"Speed mode selected.");
		Commands->Set_Player_Type(obj,1);
	}
	if (message == 940860503 && MetroidPSSControl.PlayerSettings[PlayerID]->InStartRoom && MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode != 2)
	{
		MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode = 2;
		Change_Character(obj,"FirePower_Mode");
		JmgUtility::DisplayChatMessage(obj,127,255,255,"Firepower mode selected.");
		Commands->Set_Player_Type(obj,1);
	}
	if (message == 940860504 && MetroidPSSControl.PlayerSettings[PlayerID]->InStartRoom && MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode != 3)
	{
		MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode = 3;
		Change_Character(obj,"Support_Mode");
		JmgUtility::DisplayChatMessage(obj,127,255,255,"Support mode selected.");
		Commands->Set_Player_Type(obj,1);
	}
	if (message == 940860505 && MetroidPSSControl.PlayerSettings[PlayerID]->InStartRoom && MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode != 4)
	{
		MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode = 4;
		Change_Character(obj,"Defense_Mode");
		JmgUtility::DisplayChatMessage(obj,127,255,255,"Defense mode selected.");
		Commands->Set_Player_Type(obj,1);
	}
	if (message == 940860506 && MetroidPSSControl.PlayerSettings[PlayerID]->InStartRoom && MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode != 5)
	{
		MetroidPSSControl.PlayerSettings[PlayerID]->ArmorMode = 5;
		Change_Character(obj,"Supply_Mode");
		JmgUtility::DisplayChatMessage(obj,127,255,255,"Supply mode selected.");
		Commands->Set_Player_Type(obj,1);
	}
}
void JMG_Metroid_Character_Control::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage > 0)
	{
		SupportModeSystemControl[PlayerID].LastDamaged = 50;
		if (damage != IgnoreDamage && damager && damager != obj)
		{
			IgnoreDamage = MetroidGameControl.DamageMultiplier*damage;
			Commands->Apply_Damage(obj,IgnoreDamage,"None",damager);
		}
	}
}
void JMG_Metroid_Character_Control::Killed(GameObject *obj, GameObject *damager)
{
	MetroidScoreSystem::MetroidHighScoresNode *pobj = MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(obj)];
	pobj->Deaths++;
	if (Commands->Is_A_Star(damager) && damager != obj)
	{
		MetroidScoreSystem::MetroidHighScoresNode *dobj = MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(damager)];
		dobj->KilledPlayers++;
		dobj->Kills++;
	}
	if (obj == damager && damager)
		pobj->Suicides++;
	else
		switch (Commands->Get_Preset_ID(damager))
		{
			case 81930081:case 81930082:case 81930099:case 81930091:case 81930090:case 81930106:case 81930092:case 81930103: 
				pobj->DeathFromPlayers++;break;
			case 1000000808:
				pobj->DeathFromMiniBoss++;break;
			case 82080053:
				pobj->DeathFromDefenseCameras++;break;
			case 81930093:case 1000000107:case 1000000399:case 81930100:case 81930101:case 81930104:case 1000000134:case 81930102:case 81930105:case 1000000404:case 1000000123: 
				pobj->DeathFromAliens++;break;
			case 81930084:case 1000000245:case 81930087:case 81930085:case 81930089:case 81930086:case 81930095:case 81930096:case 81930088:case 1000000249: 
				pobj->DeathFromRobots++;break;
			case 81930094:case 81930097:case 81930098: 
				pobj->DeathFromTraitors++;break;
			case 82080078:case 82080079:
				pobj->DeathFromAttackCameras++;break;
			case 82080037:case 82080129:case 82080141:case 82080131:case 82080124:case 82080128:
				pobj->DeathFromGunEmplacements++;break;
			case 82080097:
				pobj->DeathFromShredderTurret++;break;
			case 82080095:
				pobj->DeathFromBansheeBoss++;break;
			case 82080048:
				pobj->DeathFromMineBoss++;break;
			case 82080043:
				pobj->DeathFromMineBossSquish++;break;
		}
	JmgUtility::JMG_Player_Death_Message(obj,damager,NULL);
	DefenseModeSystem::TelepadNode *node = DefenseModeSystemControl.FindPlayerTelepad(PlayerID);
	if (node)
	{
		node->playerDeathCount++;
		if (node->playerDeathCount >= 2)
		{
			GameObject *Telepad = Commands->Find_Object(node->ID);
			if (Telepad)
				Commands->Apply_Damage(Telepad,9999.9f,"None",0);
		}
	}
}
void JMG_Metroid_Character_Control::Destroyed(GameObject *obj)
{
	mPlayerData.players[PlayerID]->destroyTime = clock();
	MetroidGameControl.CheckForPlayerCountUpdate();
}
void JMG_Metroid_Character_Control::ClearBeacons(GameObject *obj,int ArmorMode)
{
	if (ArmorMode != 9)
	{
		if (ArmorMode != 3) 
		{ 
			if (Has_Weapon(obj,"Weapon_Medical_Kit_Beacon"))
			{
				Set_Clip_Bullets(obj,"Weapon_Medical_Kit_Beacon",0);
				Set_Bullets(obj,"Weapon_Medical_Kit_Beacon",0);
			}
			if (Has_Weapon(obj,"Weapon_Generator_Beacon")) 
			{
				Set_Clip_Bullets(obj,"Weapon_Generator_Beacon",0);
				Set_Bullets(obj,"Weapon_Generator_Beacon",0);
			}
		} 
		if (ArmorMode != 4) 
		{ 
			if (Has_Weapon(obj,"Weapon_Defense_Mode_Gun_Beacon"))
			{
				Set_Clip_Bullets(obj,"Weapon_Defense_Mode_Gun_Beacon",0);
				Set_Bullets(obj,"Weapon_Defense_Mode_Gun_Beacon",0);
			}
			if (Has_Weapon(obj,"Weapon_Defense_Mode_Telepad")) 
			{
				Set_Clip_Bullets(obj,"Weapon_Defense_Mode_Telepad",0);
				Set_Bullets(obj,"Weapon_Defense_Mode_Telepad",0);
			}
		}
		if (ArmorMode != 5) 
		{
			if (Has_Weapon(obj,"Weapon_Shield_Upgrade_Normal")) 
			{
				Set_Clip_Bullets(obj,"Weapon_Shield_Upgrade_Normal",0);
				Set_Bullets(obj,"Weapon_Shield_Upgrade_Normal",0);
				Set_Clip_Bullets(obj,"Weapon_Shield_Upgrade_Energy",0);
				Set_Bullets(obj,"Weapon_Shield_Upgrade_Energy",0);
				Set_Clip_Bullets(obj,"Weapon_Shield_Upgrade_Heat",0);
				Set_Bullets(obj,"Weapon_Shield_Upgrade_Heat",0);
				Set_Clip_Bullets(obj,"Weapon_Shield_Upgrade_Kinetic",0);
				Set_Bullets(obj,"Weapon_Shield_Upgrade_Kinetic",0);
				Set_Clip_Bullets(obj,"Weapon_Shield_Upgrade_Rad",0);
				Set_Bullets(obj,"Weapon_Shield_Upgrade_Rad",0);
			}
			if (Has_Weapon(obj,"Weapon_Weapons_Spawner")) 
			{
				Set_Clip_Bullets(obj,"Weapon_Weapons_Spawner",0);
				Set_Bullets(obj,"Weapon_Weapons_Spawner",0);
			}
			JMG_Metroid_Supply_Mode::ClearUpgrades(JmgUtility::JMG_Get_Player_ID(obj),Get_Player_Name(obj));
		}
	}
}
void JMG_Metroid_Hide_Object_On_Death::Killed(GameObject *obj, GameObject *damager)
{
	Commands->Set_Model(obj,"RP2InvisSpawn");
}
void JMG_Metroid_New_Ambush_Cover_Point::Created(GameObject *obj)
{
	AmbushPointSystemControl.NewCoverPoint(obj,Get_Int_Parameter("ID"),Get_Int_Parameter("GroupID"),Get_Int_Parameter("Crouch") ? true : false,Get_Int_Parameter("ExcellentHidingSpot") ? true : false);
}
void JMG_Metroid_New_Ambush_Attack_Point::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,5.0f,955551);
}
void JMG_Metroid_New_Ambush_Attack_Point::Timer_Expired(GameObject *obj,int number)
{
	if (955551 == number)
	{
		AmbushPointSystemControl.AddAmbushPoint(obj,Get_Int_Parameter("ID"));
	}
}
void JMG_Metroid_Move_To_Random_Ambush_Spot::Created(GameObject *obj)
{
	faceReset = 0;
	currentEnemyID = 0;
	MyLastPos = Commands->Get_Position(obj);
	LastEnemyID = 0;
	LastMoveToLocation = Vector3(0.0f,0.0f,0.0f);
	AmbushPoint = NULL;
	ImAtPoint = false;
	Commands->Innate_Disable(obj);
	InnateEnabled = false;
	StartPos = Commands->Get_Position(obj);
	EnemySeen = 5;
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,0.25f,1);
	if (Get_Int_Parameter("FindHidingSpots"))
		Commands->Start_Timer(obj,this,1.0f,3);
	if (Get_Int_Parameter("BeginOnSpawn"))
		Commands->Start_Timer(obj,this,10.0f,2);
	else
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay")+(Get_Float_Parameter("RandomDelay") ? Commands->Get_Random(-Get_Float_Parameter("RandomDelay"),Get_Float_Parameter("RandomDelay")) : 0.0f),2);
	Commands->Start_Timer(obj,this,60.0f,4);
	Commands->Start_Timer(obj,this,Commands->Get_Random(15.0f,60.0f),5);
}
void JMG_Metroid_Move_To_Random_Ambush_Spot::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (!Commands->Get_Health(seen))
		return;
	if (!EnemySeen)
		currentEnemyID = Commands->Get_ID(seen);
	EnemySeen = 5;
	if (AmbushPoint && !Get_Int_Parameter("FindVantagePoint"))
		QuitAmbushMove(obj);
	if (!InnateEnabled && Get_Int_Parameter("FindVantagePoint") != 2)
	{
		InnateEnabled = true;
		Commands->Innate_Enable(obj);
	}
}
void JMG_Metroid_Move_To_Random_Ambush_Spot::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		if (faceReset)
			faceReset--;
		if (EnemySeen)
			EnemySeen--;
		else if (currentEnemyID)
		{
			LastEnemyID = 0;
			currentEnemyID = 0;
			Commands->Action_Reset(obj,100.0f);
		}
		if (Get_Int_Parameter("FindVantagePoint") == 2 && EnemySeen && LastEnemyID != currentEnemyID && AmbushPoint && !AmbushPoint->Crouch && JmgUtility::SimpleDistance(AmbushPoint->coverPos,Commands->Get_Position(obj)) < 9.0f)
		{
			LastEnemyID = currentEnemyID;
			GameObject *enemy = Commands->Find_Object(currentEnemyID);
			if (enemy)
			{
				if (InnateEnabled)
				{
					InnateEnabled = false;
					Commands->Innate_Disable(obj);
				}
				ActionParamsStruct params;
				params.Set_Basic(this,10,100);
				params.Set_Attack(enemy,300.0f,0,true);
				params.AttackCrouched = false;
				params.AttackCheckBlocked = false;
				params.AttackErrorOverride = 1.0f;
				Commands->Action_Attack(obj,params);
			}
			else if (currentEnemyID)
			{
				LastEnemyID = 0;
				currentEnemyID = 0;
				Commands->Action_Reset(obj,100.0f);
			}
		}
		Commands->Start_Timer(obj,this,0.25f,1);
	}
	if (2 == number)
	{
		if ((!AmbushPoint || Get_Int_Parameter("FindVantagePoint")) && !EnemySeen)
			if (!SelectRandomAmbushSpot(obj))
			{
				Commands->Start_Timer(obj,this,Commands->Get_Random(5.0f,15.0f),2);
				return;
			}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay")+(Get_Float_Parameter("RandomDelay") ? Commands->Get_Random(-Get_Float_Parameter("RandomDelay"),Get_Float_Parameter("RandomDelay")) : 0.0f),2);
	}
	if (3 == number)
	{
		if (ImAtPoint)
		{
			Vector3 Pos = Commands->Get_Position(obj);
			GameObject *AStar = Commands->Get_A_Star(Pos);
			if (AStar && JmgUtility::SimpleDistance(Pos,Commands->Get_Position(AStar)) < 225.0f)
			{
				/*Vector3 EnemyPos = Commands->Get_Position(AStar);
				int EnemyID = Commands->Get_ID(AStar);
				if (LastEnemyID != EnemyID || JmgUtility::SimpleDistance(LastMoveToLocation,EnemyPos) > 5.0f)
				{
					LastEnemyID = EnemyID;
					LastMoveToLocation = EnemyPos;*/
					Commands->Action_Reset(obj,1);
					Commands->Action_Reset(obj,10);
					Commands->Action_Reset(obj,100);
					Commands->Action_Reset(obj,1337);
					ImAtPoint = false;
					Commands->Innate_Disable(obj);
					InnateEnabled = false;
					ActionParamsStruct params;
					params.Set_Movement(Commands->Get_Position(AStar),1.0f,1.0f,false);
					params.MovePathfind = true;
					Commands->Action_Goto(obj,params);
					params.Set_Basic(this,999,10);
					params.Set_Attack(AStar,300.0f,0,true);
					Commands->Action_Attack(obj,params);
				//}
			}
		}
		Commands->Start_Timer(obj,this,0.1f,3);
	}
	if (4 == number)
	{
		Vector3 Pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(Pos,MyLastPos) < 1.0f)
			Commands->Set_Animation(obj,"h_a_human.h_a_422a",false,0,0.0f,0.0f,false);
		MyLastPos = Pos;
		Commands->Start_Timer(obj,this,60.0f,4);
	}
	if (5 == number && Commands->Get_Health(obj))
	{
		FaceAmbushLocation(obj);
		Commands->Start_Timer(obj,this,Commands->Get_Random(15.0f,45.0f),5);
	}
}
void JMG_Metroid_Move_To_Random_Ambush_Spot::Action_Complete(GameObject *obj,int action,ActionCompleteReason reason)
{
	LastEnemyID = 0;
	Commands->Action_Reset(obj,100.0f);
	if (!ImAtPoint && Commands->Get_Health(obj) && !faceReset)
		FaceAmbushLocation(obj);
	ImAtPoint = true;
	if (!(Get_Int_Parameter("FindVantagePoint") || Get_Int_Parameter("FindHidingSpots")))
		QuitAmbushMove(obj);
}
void JMG_Metroid_Move_To_Random_Ambush_Spot::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damager && damage)
	{
		if (AmbushPoint && !Get_Int_Parameter("FindVantagePoint"))
			QuitAmbushMove(obj);
		Commands->Action_Reset(obj,100.0f);
		ActionParamsStruct params;
		params.Set_Basic(this,10,1);
		params.Set_Face_Location(Commands->Get_Position(damager),10.0f);
		Commands->Action_Face_Location(obj,params);
		if (!InnateEnabled)
		{
			InnateEnabled = true;
			Commands->Innate_Enable(obj);
		}
	}
}
void JMG_Metroid_Move_To_Random_Ambush_Spot::Destroyed(GameObject *obj)
{
	QuitAmbushMove(obj);
}
bool JMG_Metroid_Move_To_Random_Ambush_Spot::SelectRandomAmbushSpot(GameObject *obj)
{
	if (!Commands->Get_Health(obj))
		return false;
	AmbushPointSystem::CoverPointNode *Temp = NULL;
	if (Get_Int_Parameter("FindHidingSpots"))
		Temp = AmbushPointSystemControl.GetGoodHidingSpot(&FaceLocation,Get_Int_Parameter("GroupID"),Get_Int_Parameter("MaxGuards"));
	else if (Get_Int_Parameter("FindVantagePoint"))
	{
		Vector3 MyPos = Commands->Get_Position(obj);
		GameObject *AStar = Commands->Get_A_Star(MyPos);
		FaceLocation = Commands->Get_Position(AStar);
		if (!AStar)
			return false;
		Temp = AmbushPointSystemControl.FindBestCoverPoint(MyPos,&FaceLocation,Get_Int_Parameter("GroupID"),Get_Float_Parameter("MaxDistance"),Get_Float_Parameter("MaxTargetDistance"),Get_Int_Parameter("MaxGuards"));
	}
	else
		Temp = AmbushPointSystemControl.GetRandomCoverPoint(&FaceLocation,StartPos,Get_Int_Parameter("GroupID"),Get_Int_Parameter("MaxGuards"),Get_Float_Parameter("MaxDistance"));
	if (Temp == AmbushPoint)
		return false;
	if (!Temp)
		return false;
	QuitAmbushMove(obj);
	ImAtPoint = false;
	AmbushPoint = Temp;
	CurrentAmbushPoint = Temp;
	AmbushPoint->GuardCount++;
	Commands->Innate_Disable(obj);
	InnateEnabled = false;
	Commands->Action_Reset(obj,100.0f);
	Vector3 movePos = AmbushPoint ? AmbushPoint->coverPos : Vector3();
	ActionParamsStruct params;
	params.Set_Basic(this,10,1337);
	params.Set_Movement(movePos,(Get_Float_Parameter("SpeedMultiplier") ? Get_Float_Parameter("SpeedMultiplier") : 1.0f),Get_Int_Parameter("MaxGuards") == 1 ? 0.0f : 1.0f,false);
	params.MovePathfind = true;
	params.ObserverID = this->Get_ID();
	Commands->Action_Goto(obj,params);
	return true;
}
void JMG_Metroid_Move_To_Random_Ambush_Spot::QuitAmbushMove(GameObject *obj)
{
	if (AmbushPoint)
		if (AmbushPoint->GuardCount)
			AmbushPoint->GuardCount--;
		else
			Console_Input("msg Tried to remove more guards than possibile");
	AmbushPoint = NULL;
}
void JMG_Metroid_Move_To_Random_Ambush_Spot::FaceAmbushLocation(GameObject *obj)
{
	if (CurrentAmbushPoint && !EnemySeen && JmgUtility::SimpleDistance(CurrentAmbushPoint->coverPos,Commands->Get_Position(obj)) < 4.0f)
	{
		faceReset = 5;
		Commands->Action_Reset(obj,100.0f);
		ActionParamsStruct params;
		params.Set_Basic(this,10,1);
		params.Set_Face_Location(CurrentAmbushPoint ? CurrentAmbushPoint->GetRandomAmbushPoint() : Vector3(0.0f,0.0f,0.0f),10.0f);
		Commands->Action_Face_Location(obj,params);
		/*ActionParamsStruct params;
		Vector3 faceLocation = (CurrentAmbushPoint ? CurrentAmbushPoint->GetRandomAmbushPoint() : Vector3(0.0f,0.0f,0.0f)),pos = Commands->Get_Position(obj);
		faceLocation.X -= pos.X;
		faceLocation.Y -= pos.Y;
		Commands->Set_Facing(obj,atan2(faceLocation.Y,faceLocation.X)*180/PI);*/
	}
}
void JMG_Metroid_Mine_Tower_Control::Created(GameObject *obj)
{
	Commands->Enable_Spawner(407702,true);
	GameObject *Lockdown0 = Commands->Create_Object("FakeObject",Vector3(1000.0f,500.0f,-100.0f));
	Commands->Set_Model(Lockdown0,"LckDwnScrn00");
	Commands->Set_Animation(Lockdown0,"LckDwnScrn00.LckDwnScrn00",false,0,0.0f,0.0f,false);
	LockDownSignIDs[0] = Commands->Get_ID(Lockdown0);
	GameObject *Lockdown1 = Commands->Create_Object("FakeObject",Vector3(1000.0f,500.0f,-100.0f));
	Commands->Set_Model(Lockdown1,"LckDwnScrn01");
	Commands->Set_Animation(Lockdown1,"LckDwnScrn01.LckDwnScrn01",false,0,0.0f,0.0f,false);
	LockDownSignIDs[1] = Commands->Get_ID(Lockdown1);
	GameObject *Lockdown2 = Commands->Create_Object("FakeObject",Vector3(1000.0f,500.0f,-100.0f));
	Commands->Set_Model(Lockdown2,"LckDwnScrn02");
	Commands->Set_Animation(Lockdown2,"LckDwnScrn02.LckDwnScrn02",false,0,0.0f,0.0f,false);
	LockDownSignIDs[2] = Commands->Get_ID(Lockdown2);


	char param[256];
	sprintf(param,"%d,123424,0",Commands->Get_ID(obj));
	GameObject *Button0 = Commands->Create_Object("Security_Lockout",Vector3(1000.0f,500.0f,0.0f));
	Commands->Set_Animation(Button0,"JG_Button.JG_Button",false,0,0.0f,1.0f,true);
	Commands->Attach_To_Object_Bone(Button0,Lockdown0,"Button");
	Commands->Attach_Script(Button0,"JMG_Metroid_Send_Custom_On_Poke",param);
	LockDownButtonIDs[0] = Commands->Get_ID(Button0);
	GameObject *Button1 = Commands->Create_Object("Security_Lockout",Vector3(1000.0f,500.0f,0.0f));
	Commands->Set_Animation(Button1,"JG_Button.JG_Button",false,0,0.0f,1.0f,true);
	Commands->Attach_To_Object_Bone(Button1,Lockdown1,"Button");
	Commands->Attach_Script(Button1,"JMG_Metroid_Send_Custom_On_Poke",param);
	LockDownButtonIDs[1] = Commands->Get_ID(Button1);
	GameObject *Button2 = Commands->Create_Object("Security_Lockout",Vector3(1000.0f,500.0f,0.0f));
	Commands->Set_Animation(Button2,"JG_Button.JG_Button",false,0,0.0f,1.0f,true);
	Commands->Attach_To_Object_Bone(Button2,Lockdown2,"Button");
	Commands->Attach_Script(Button2,"JMG_Metroid_Send_Custom_On_Poke",param);
	LockDownButtonIDs[2] = Commands->Get_ID(Button2);

	for (int x = 0;x < 3;x++)
		Activated[x] = false;
	PendingActivation = 0;

	Commands->Start_Timer(obj,this,60.0f,1);
}
void JMG_Metroid_Mine_Tower_Control::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Static_Anim_Phys_Goto_Frame(456473,PendingActivation == 3 ? 999.0f : 0.0f,"MineElevator.MineElevator");
		Commands->Static_Anim_Phys_Goto_Frame(458415,PendingActivation == 3 ? 999.0f : 0.0f,"MineElevatorKZ.MineElevatorKZ");
		Commands->Start_Timer(obj,this,60.0f,1);
	}
}
void JMG_Metroid_Mine_Tower_Control::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 223424)
	{
		for (int x = 0;x < 3;x++)
		{
			if (MetroidObjectiveSystemControl.Get_Objective_Status(30+x) == NewObjectiveSystem::NotDefined)
			{
				MetroidObjectiveSystemControl.Add_Objective(30+x,NewObjectiveSystem::Primary,NewObjectiveSystem::Pending,12810+x,"",12810+x,Commands->Get_Position(Commands->Find_Object(LockDownButtonIDs[x])));
				JmgUtility::SetHUDHelpText(12810,Vector3(0,1,0));
			}
		}
	}
	if (message == 403025)
	{
		char animation[32];
		for (int x = 0;x < 3;x++)
		{
			Activated[x] = false;
			GameObject *Button = Commands->Find_Object(LockDownButtonIDs[x]);
			if (Button)
				Commands->Set_Animation(Button,"JG_Button.JG_Button",false,0,0.0f,1.0f,true);
			sprintf(animation,"LckDwnScrn0%d.LckDwnScrn0%d",x,x);
			GameObject *LockdownScreen = Commands->Find_Object(LockDownSignIDs[x]);
			if (LockdownScreen)
				Commands->Set_Animation(LockdownScreen,animation,false,0,0.0f,0.0f,false);
		}
		PendingActivation = 0;
	}
	if (message == 123424)
	{
		if (PendingActivation == 3)
			return;
		Commands->Enable_Spawner(407702,false);
		int Number = 0;
		int SenderID = Commands->Get_ID(sender);
		for (;Number < 3;Number++)
			if (LockDownButtonIDs[Number] == SenderID)
				break;
		if (Activated[Number])
			return;
		Activated[Number] = true;
		MetroidObjectiveSystemControl.Set_Objective_Status(30+Number,NewObjectiveSystem::Accomplished);
		Commands->Set_Animation(sender,"JG_Button.JG_Button",false,0,6.0f,7.0f,true);
		GameObject *Sign = Commands->Find_Object(LockDownSignIDs[Number]);
		if (!Sign)
			return;
		char Animation[32];
		sprintf(Animation,"%s.%s",Get_Model(Sign),Get_Model(Sign));
		Commands->Set_Animation(Sign,Animation,false,0,1.0f,1.0f,false);
		PendingActivation++;
		MetroidGameControl.DisplayDynamicHUDMessage(0,(float)PendingActivation);
		MetroidScoreControl.MetroidPlayerHighScoreNodes[param]->ElevatorButtonsPressed++;
		if (PendingActivation == 3)
		{
			MetroidSabotagedCrateSystemControl.TriggerExplosives();
			Commands->Static_Anim_Phys_Goto_Frame(456473,999.0f,"MineElevator.MineElevator");
			Commands->Static_Anim_Phys_Goto_Frame(458415,999.0f,"MineElevatorKZ.MineElevatorKZ");
			Commands->Create_Object("Mining_Equipment_Boss",Vector3(1032.995f,437.589f,-30.431f));
			Commands->Send_Custom_Event(obj,obj,123425,0,0);
			JMG_Metroid_Game_Control::currentBriefingId = 12776;
			for (int x = 1;x < 128;x++)
			{
				if (!Get_GameObj(x))
					continue;
				JMG_Metroid_Game_Control::ParseBriefingString(Get_Translated_String(JMG_Metroid_Game_Control::currentBriefingId),x);
			}
			for (int x = 1;x < 128;x++)
			{
				GameObject *Player = Get_GameObj(x);
				if (!Player)
					continue;
				if (JmgUtility::SimpleDistance(Commands->Get_Position(Player),Vector3(1000.0f,500.0f,0.0f)) > 360000.0f)
					continue;
				MetroidPSSControl.PlayerSettings[x]->ChapterCompleted(Player,7);
			}
		}
	}
	if (message == 123425)
	{
		if (MetroidObjectiveSystemControl.Get_Objective_Status(32) == NewObjectiveSystem::NotDefined)
		{
			MetroidObjectiveSystemControl.Add_Objective(33,NewObjectiveSystem::Primary,NewObjectiveSystem::Pending,12813,"",12813,Commands->Get_Position(Commands->Find_Object(406329)));
			JmgUtility::SetHUDHelpText(12813,Vector3(0,1,0));
		}
		MetroidGameControl.MainElevatorDown = true;
	}
	if (message == 406919)
	{
		GameObject *ShieldWall = Commands->Find_Object(MetroidGameControl.FinalCheckpointShieldWallID);
		if (ShieldWall)
			Commands->Destroy_Object(ShieldWall);
	}
}
void JMG_Metroid_Mine_Tower_Control::Destroyed(GameObject *obj)
{
	for (int x = 0;x < 3;x++)
	{
		GameObject *Object = Commands->Find_Object(LockDownSignIDs[x]);
		if (Object)
			Commands->Destroy_Object(Object);
		Object = Commands->Find_Object(LockDownButtonIDs[x]);
		if (Object)
			Commands->Destroy_Object(Object);
	}
}
void JMG_Metroid_Send_Custom_On_Poke::Poked(GameObject *obj,GameObject *poker)
{
	GameObject *Target = Commands->Find_Object(Get_Int_Parameter("ID"));
	if (!Target)
		return;
	Commands->Send_Custom_Event(obj,Target,Get_Int_Parameter("Message"),Get_Int_Parameter("Param") ? Get_Int_Parameter("Param") : JmgUtility::JMG_Get_Player_ID(poker),0);
}
void JMG_Metroid_AI_Path_Node::Created(GameObject *obj)
{
	MyNode = MetroidBossPathControlSystem.AddNode(obj,Get_Parameter("NodeName"),Get_Float_Parameter("MaxDeviation"));
	Commands->Start_Timer(obj,this,1.0f,847734);
}
void JMG_Metroid_AI_Path_Node::Timer_Expired(GameObject *obj,int number)
{
	if (number == 847734)
	{
		if (!MyNode)
		{
			Commands->Start_Timer(obj,this,1.0f,847734);
			return;
		}
		int TempSize = 0;
		char Temp[256];
		int Astrlen = strlen(Get_Parameter("LinkedNodes"));
		for (int x = 0;x < Astrlen;x++)
		{// Break the node list up
			if (Get_Parameter("LinkedNodes")[x] == Get_Parameter("Delim")[0])
			{// Check for the delims
				if (TempSize)
				{// Make sure there is a name to convert
					Temp[TempSize] = '\0';
					MyNode->AddLinkNode(MetroidBossPathControlSystem.Find_AIPathNode_Node(Temp));
					TempSize = 0;
					MyNode->NumberOfLinks++;
				}
			}
			else
			{
				Temp[TempSize] = Get_Parameter("LinkedNodes")[x];
				TempSize++;
			}
		}
		if (TempSize)
		{// Catch any leftovers
			Temp[TempSize] = '\0';
			MyNode->AddLinkNode(MetroidBossPathControlSystem.Find_AIPathNode_Node(Temp));
			MyNode->NumberOfLinks++;
		}
		Commands->Destroy_Object(obj);
		return;
	}
}
void JMG_Metroid_Mine_Boss::Created(GameObject *obj)
{
	GameObject *BossMusic = Commands->Find_Object(MetroidGameControl.BossDefeatedMusicID);
	if (BossMusic)
		Commands->Destroy_Object(BossMusic);
	LastHealth = 2000.0f;
	LastArmor = 2000.0f;
	SoundID = 0;
	MetroidGameControl.BossID = Commands->Get_ID(obj);
	MetroidGameControl.BossMode = 0;
	MetroidGameControl.BossDefeated = false;
	SuperStuckCount = StuckCount = 0;
	CreationSpot = LastSpot = Commands->Get_Position(obj);
	CreationSpot.Z += 5.0f;
	ChasingID = EnemyID = 0;
	SetupAZoneBox(obj);
	Commands->Set_Animation(obj,"MineBoss.MineBoss",true,0,0.0f,-1,false);

	GameObject *Turret = Commands->Create_Object("Mining_Equipment_Boss_Turret",Commands->Get_Position(obj));
	Commands->Attach_To_Object_Bone(Turret,obj,"TurretSpot");
	Commands->Attach_Script(Turret,"JMG_Metroid_Boss_Turret","0.0,300.0,11,1.0 0.0 0.0,90");
	TurretID = Commands->Get_ID(Turret);

	GameObject *WarningLight = Commands->Create_Object("Warning_Light",Vector3(987.63f,516.99f,-25.67f));
	Commands->Set_Model(WarningLight,"WarningLight");
	Commands->Set_Animation(WarningLight,"WarningLight.WarningLight",true,0,0.0f,-1,false);
	Commands->Attach_To_Object_Bone(WarningLight,obj,"WarningLight");
	WarningLightID = Commands->Get_ID(WarningLight);

	char bonename[32];
	for (int x = 0;x < 4;x++)
	{
		GameObject *Light = Commands->Create_Object("Warning_Light_Effect",Commands->Get_Position(WarningLight));
		sprintf(bonename,"LightPos%d",x+1);
		Commands->Attach_To_Object_Bone(Light,WarningLight,bonename);
		LightID[x] = Commands->Get_ID(Light);
	}
	RoadPoint = NULL;
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,2.25f,4732546);
	Commands->Start_Timer(obj,this,0.1f,4732547);
	Commands->Start_Timer(obj,this,1.0f,4732548);
}
void JMG_Metroid_Mine_Boss::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (!Commands->Get_Health(seen) || Is_Script_Attached(seen,"JMG_Metroid_Recover_Fall_Damage_Once"))
		return;
	if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) < 225)
		return;
	int SeenID = Commands->Get_ID(seen);
	if (!EnemyID || EnemyID == SeenID)
	{
		EnemyID = SeenID;
		ClearTarget = 11;
		ActionParamsStruct params;
		Commands->Get_Action_Params(obj,params);
		params.MoveLocation = Commands->Get_Position(seen);
		Commands->Action_Reset(obj,10);
		Commands->Action_Reset(obj,100);
	}
}
void JMG_Metroid_Mine_Boss::Timer_Expired(GameObject *obj,int number)
{
	if (number == 4732548)
	{
		if (!SoundID)
		{
			GameObject *AStar = Commands->Get_A_Star(Vector3(950.0f,500.0f,-1501.0f));
			if (AStar && JmgUtility::SimpleDistance(Vector3(950.0f,500.0f,-1501.0f),Commands->Get_Position(AStar)) < 2250000.0f)
			{
				GameObject *Sound = Commands->Create_Object("Daves Arrow",Vector3(950.0f,500.0f,0.0f));
				Commands->Set_Model(Sound,!MetroidGameControl.BossMode ? "BossMusic1" : "BossMusic2");
				SoundID = Commands->Get_ID(Sound);
			}
		}
		Vector3 CurrentPos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(CurrentPos,LastSpot) < 225)
		{
			StuckCount++;
			if (StuckCount > 2)
			{
				RoadPoint = NULL;
				Commands->Action_Reset(obj,10);
				Commands->Action_Reset(obj,100);
				EnemyID = 0;
				StuckCount = 0;
				SuperStuckCount++;
				if (SuperStuckCount > 2)
				{
					SuperStuckCount = 0;
					Commands->Set_Position(obj,CreationSpot);
				}
			}
		}
		else
		{
			SuperStuckCount = 0;
			StuckCount = 0;
			LastSpot = CurrentPos;
		}
		Commands->Start_Timer(obj,this,1.0f,4732548);
	}
	if (number == 4732546)
	{
		RoadPoint = MetroidBossPathControlSystem.GetNearestRoadNode(Commands->Get_Position(obj));
		RoadPointPos = RoadPoint->Position;
		Goto_Location(obj,RoadPointPos,true,5.0,false,true,2.0f);
	}
	if (number == 4732547)
	{
		Vector3 TargetSpot = Commands->Get_Bone_Position(obj,The_Cnc_Game()->Is_Dedicated() ? "DedicatedZone" : "ZonePos");
		GameObject *TheZone = Commands->Find_Object(ZoneID);
		if (TheZone)
		{
			Matrix3 rotation(true);
			rotation.Rotate_Z(Commands->Get_Facing(obj));
			OBBoxClass zoneBoundingBox(TargetSpot,DrillZone->Extent,rotation);
			Set_Zone_Box(TheZone,zoneBoundingBox);
		}
		if (ClearTarget)
		{
			ClearTarget--;
			if (!ClearTarget)
			{
				EnemyID = 0;
				ChasingID = 0;
				GameObject *Turret = Commands->Find_Object(TurretID);
				if (Turret)
					Commands->Send_Custom_Event(obj,Turret,9119112,0,0);
			}
		}
		int equipmentId = 0;
		Vector3 myPos = Commands->Get_Position(obj);
		if (MetroidEquipmentSystemControl.FindNearestEquipment(&myPos,&equipmentId) < 25.0f && equipmentId)
		{
			GameObject *equipment = Commands->Find_Object(equipmentId);
			if (equipment)
				Commands->Apply_Damage(equipment,9999.9f,"Blamokiller",obj);
		}
		Commands->Start_Timer(obj,this,0.1f,4732547);
	}
}
void JMG_Metroid_Mine_Boss::Action_Complete(GameObject *obj,int action,ActionCompleteReason reason)
{
	if (!EnemyID && Check_Road_Update(Commands->Get_Position(obj)))
		Goto_Location(obj,RoadPointPos,false,5.0,false,true,2.0f);
	if (EnemyID)
	{
		GameObject *Enemy = Commands->Find_Object(EnemyID);
		Vector3 EnemyPos = Commands->Get_Position(Enemy);
		if (Enemy)
		{
			RoadPoint = NULL;
			ChasingID = EnemyID;
			Goto_Location(obj,EnemyPos,false,5.0,false,true,JmgUtility::MathClamp(Commands->Get_Distance(Commands->Get_Position(obj),EnemyPos)/250.0f,1.5f,5));
			GameObject *Turret = Commands->Find_Object(TurretID);
			if (Turret)
				Commands->Send_Custom_Event(obj,Turret,9119112,MetroidGameControl.BossMode ? 0 : ChasingID,0);
		}
		else if (Check_Road_Update(Commands->Get_Position(obj)))
			Goto_Location(obj,RoadPointPos,false,5.0,false,true,2.0f);
	}
}
void JMG_Metroid_Mine_Boss::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damager && damage > 0 && Commands->Get_Player_Type(damager) != 1)
	{
		Commands->Apply_Damage(obj,-damage,"None",damager);
		return;
	}
	float MyHealth = Commands->Get_Health(obj);
	float MyArmor = Commands->Get_Shield_Strength(obj);
	if (MyHealth)
	{
		LastHealth = MyHealth;
		LastArmor = MyArmor;
	}
	if (!MetroidGameControl.BossMode && MyHealth < 2000.0f)
	{
		MetroidGameControl.BossMode = 1;
		GameObject *Sound = Commands->Find_Object(SoundID);
		if (Sound)
			Commands->Destroy_Object(Sound);
		SoundID = 0;
	}
	if (MetroidGameControl.BossMode && MyArmor >= 2000.0f)
	{
		MetroidGameControl.BossMode = 0;
		GameObject *Sound = Commands->Find_Object(SoundID);
		if (Sound)
			Commands->Destroy_Object(Sound);
		SoundID = 0;
	}
	if (damager && damage > 0)
		MetroidPSSControl.PlayerSettings[JmgUtility::JMG_Get_Player_ID(damager)]->BossDamage += damage;
	int SeenID = Commands->Get_ID(damager);
	if (MetroidGameControl.BossMode && damage > 0 && damager && (!EnemyID || EnemyID == SeenID))
	{
		EnemyID = SeenID;
		ClearTarget = 11;
		ActionParamsStruct params;
		Commands->Get_Action_Params(obj,params);
		params.MoveLocation = Commands->Get_Position(damager);
		Commands->Action_Reset(obj,10);
		Commands->Action_Reset(obj,100);
	}
}
void JMG_Metroid_Mine_Boss::Killed(GameObject *obj, GameObject *damager)
{
	if ((damager == obj || damager == NULL || Commands->Get_Player_Type(damager) != 1) && LastHealth)
	{
		GameObject *Boss = Commands->Create_Object(Commands->Get_Preset_Name(obj),CreationSpot);
		Commands->Set_Health(Boss,LastHealth);
		Commands->Set_Shield_Strength(Boss,LastArmor);
		return;
	}
	MetroidObjectiveSystemControl.Set_Objective_Status(41,NewObjectiveSystem::Accomplished);
	PerkSystemIncreasePlayerPerkUnlockAmount(damager,79,1);
	MetroidPSSControl.BossKilled();
	MetroidGameControl.BossDefeated = true;
	GameObject *Sound = Commands->Create_Object("Daves Arrow",Vector3(950.0f,500.0f,0.0f));
	Commands->Set_Model(Sound,"BossMusic5");
	MetroidGameControl.BossDefeatedMusicID = Commands->Get_ID(Sound);
}
void JMG_Metroid_Mine_Boss::Destroyed(GameObject *obj)
{
	GameObject *Object = Commands->Find_Object(TurretID);
	if (Object)
		Commands->Destroy_Object(Object);
	Object = Commands->Find_Object(ZoneID);
	if (Object)
		Commands->Destroy_Object(Object);
	Object = Commands->Find_Object(WarningLightID);
	if (Object)
		Commands->Destroy_Object(Object);
	for (int x = 0;x < 4;x++)
	{
		Object = Commands->Find_Object(LightID[x]);
		if (Object)
			Commands->Destroy_Object(Object);
	}
	GameObject *Sound = Commands->Find_Object(SoundID);
	if (Sound)
		Commands->Destroy_Object(Sound);
	SoundID = 0;
	if (Commands->Get_Health(obj) > 0.0f && !MetroidGameControl.BossDefeated)
	{
		GameObject *Boss = Commands->Create_Object(Commands->Get_Preset_Name(obj),CreationSpot);
		Commands->Set_Health(Boss,Commands->Get_Health(obj));
		Commands->Set_Shield_Strength(Boss,Commands->Get_Shield_Strength(obj));
	}
}
bool JMG_Metroid_Mine_Boss::Check_Road_Update(const Vector3 &Pos)
{
	if (!RoadPoint)
		RoadPoint = MetroidBossPathControlSystem.GetNearestRoadNode(Pos);
	if (!RoadPoint)
		return false;
	MetroidBossPathControl::AIPathNode *Temp = RoadPoint->GetNextRoadPoint();
	if (Temp)
	{
		RoadPoint = Temp;
		RoadPointPos = RoadPoint->Position;
		return true;
	}
	return false;
}
void JMG_Metroid_Mine_Boss::Goto_Location(GameObject *obj,const Vector3 &TargetLocation,bool Pathfind,float ArriveDistance,bool Backward,bool Monitor,float Speed)
{
	Commands->Action_Reset(obj,10);
	ActionParamsStruct params;
	params.Set_Basic(this,100,10);
	params.MoveFollow=false;
	params.MoveBackup=Backward;
	params.ShutdownEngineOnArrival = true;
	if (Monitor)
		params.ObserverID = this->Get_ID();
	params.MovePathfind=Pathfind;
	params.Set_Movement(TargetLocation,Speed*(!MetroidGameControl.BossMode ? 0.33f : 2.0f),ArriveDistance,false);
	Commands->Action_Goto(obj,params);
}
void JMG_Metroid_Mine_Boss::SetupAZoneBox(GameObject *obj)
{
	OBBoxClass *TheBox = new OBBoxClass;
	JmgUtility::RotateZoneBox(Commands->Get_Facing(obj),TheBox->Basis);
	TheBox->Center = Commands->Get_Bone_Position(obj,The_Cnc_Game()->Is_Dedicated() ? "DedicatedZone" : "ZonePos");
	TheBox->Extent = Vector3(5.25f,1.83f,The_Cnc_Game()->Is_Dedicated() ? 10.0f : 1.83f);
	GameObject *ReEnterZone = Create_Zone("Script_Zone_All",*TheBox);
	Commands->Attach_Script(ReEnterZone,"JMG_Metroid_Boss_Grinder_Kill_Zone","");
	ZoneID = Commands->Get_ID(ReEnterZone);
	DrillZone = TheBox;
}
void JMG_Metroid_Boss_Turret::Created(GameObject *obj)
{
	IgnoreID = 0;
	MinDistance = Get_Float_Parameter("MinAttackDistance")*Get_Float_Parameter("MinAttackDistance");
	MaxDistance = Get_Float_Parameter("MaxAttackDistance")*Get_Float_Parameter("MaxAttackDistance");
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	ClearTarget = 0;
	SeenID = 0;
	RotateDirection = 1;
	if (Get_Int_Parameter("SwivelTurret") == 1)
		Commands->Start_Timer(obj,this,1,1);
	else if (!Get_Int_Parameter("SwivelTurret"))
		Commands->Start_Timer(obj,this,1,0);
	else if (Get_Int_Parameter("SwivelTurret") > 0)
		Commands->Start_Timer(obj,this,10.0f,3);
	Commands->Start_Timer(obj,this,10.0f,10);
}
void JMG_Metroid_Boss_Turret::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (!Commands->Get_Health(seen))
		return;
	if (seen->As_VehicleGameObj())
	{
		int SeatCount = Get_Vehicle_Seat_Count(seen);
		if (!SeatCount)
			return;
		if (SeatCount && !Get_Vehicle_Occupant_Count(seen))
			return;
	}
	GameObject *o = Get_Vehicle(seen);
	if (o)
		seen = o;
	if (seen->As_SoldierGameObj() && !Get_Vector3_Parameter("CanTarget[Infantry|Vehicles|Aircraft]").X)
		return;
	else if (seen->As_VehicleGameObj())
		if (Is_VTOL(seen) && !Get_Vector3_Parameter("CanTarget[Infantry|Vehicles|Aircraft]").Z)
			return;
		else if (!Is_VTOL(seen) && !Get_Vector3_Parameter("CanTarget[Infantry|Vehicles|Aircraft]").Y)
			return;
	int TargetID = Commands->Get_ID(seen);
	if (TargetID == IgnoreID)
		return;
	float TempDist = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen));
	if (TempDist >= MinDistance && TempDist <= MaxDistance && (!SeenID || SeenID == TargetID || TempDist < TargetDistance))
	{
		TargetDistance = TempDist;
		ClearTarget = Get_Int_Parameter("AttackTimer");
		if (SeenID != TargetID)
		{
			SeenID = TargetID;
			ActionParamsStruct params;
			params.Set_Basic(this,100,2);
			params.Set_Attack(seen,Get_Float_Parameter("MaxAttackDistance"),0.0,MetroidGameControl.BossMode ? false : true);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
		}
	}
}
void JMG_Metroid_Boss_Turret::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 9119112)
	{
		IgnoreID = param;
	}
}
void JMG_Metroid_Boss_Turret::Action_Complete(GameObject *obj,int action,ActionCompleteReason reason)
{
	if (action == 2)
		Commands->Action_Reset(obj,100);
}
void JMG_Metroid_Boss_Turret::Timer_Expired(GameObject *obj,int number)
{
	if (number == 10)
	{
 		if (ClearTarget)
		{
			ClearTarget--;
			if (!ClearTarget)
			{
				Commands->Action_Reset(obj,100.0f);
				Commands->Action_Reset(obj,2.0f);
				SeenID = 0;
			}
		}
		Commands->Start_Timer(obj,this,0.1f,10);
	}
	if (number == 0)
	{
		if (SeenID)
		{
			Commands->Start_Timer(obj,this,1.0f,0);
			return;
		}
		Commands->Action_Reset(obj,70.0f);
		Vector3 Pos = Commands->Get_Position(obj);
		float Facing = Commands->Get_Facing(obj)*PI180;
		Pos.X += cos(Facing)*1000.0f;
		Pos.Y += sin(Facing)*1000.0f;
		Pos.Z = Commands->Get_Bone_Position(obj,"MuzzleA0").Z;
		ActionParamsStruct params;
		params.Set_Basic(this,70,1);
		params.Set_Attack(Pos,0.0f,0.0f,true);
		Commands->Action_Attack(obj,params);
		Commands->Start_Timer(obj,this,0.25f,0);
	}
	if (number == 1)
	{
		if (SeenID)
		{
			Commands->Start_Timer(obj,this,10.0f,1);
			return;
		}
		ActionParamsStruct params;
		params.Set_Basic(this,70,1);
		params.Set_Attack(Vector3(10.0f*Commands->Get_Random_Int(-1,2),10.0f*Commands->Get_Random_Int(-1,2),2.0f),0.0f,0.0f,true);
		Commands->Action_Attack(obj,params);
		Commands->Start_Timer(obj,this,10.0f,1);
	}
	if (number == 3)
	{
		if (SeenID)
		{
			Commands->Start_Timer(obj,this,1.0f,3);
			return;
		}
		Commands->Action_Reset(obj,70.0f);
		RotateDirection *= -1.0f;
		Vector3 Pos = Commands->Get_Position(obj);
		float Rotation = JmgUtility::MathClampDegrees(Commands->Get_Facing(obj)+RotateDirection*Get_Int_Parameter("SwivelTurret"))*PI180;
		Pos.X += cos(Rotation)*1000.0f;
		Pos.Y += sin(Rotation)*1000.0f;
		Pos.Z = Commands->Get_Bone_Position(obj,"MuzzleA0").Z;
		ActionParamsStruct params;
		params.Set_Basic(this,70,1);
		params.Set_Attack(Pos,0.0f,0.0f,true);
		Commands->Action_Attack(obj,params);
		Commands->Start_Timer(obj,this,0.05f*Get_Int_Parameter("SwivelTurret"),3);
	}
}
void JMG_Metroid_Boss_Turret::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage > 0 && !damager && obj != damager && !SeenID)
	{
		SeenID = Commands->Get_ID(damager);
		ActionParamsStruct params;
		params.Set_Basic(this,100,2);
		params.Set_Attack(damager,Get_Float_Parameter("MaxAttackDistance"),1.0,MetroidGameControl.BossMode ? false : true);
		params.AttackCheckBlocked = false;
		params.AttackForceFire = true;
		ClearTarget = Get_Int_Parameter("AttackTimer");
		Commands->Action_Attack(obj,params);
	}
}
void JMG_Metroid_Boss_Grinder_Kill_Zone::Entered(GameObject *obj,GameObject *enter)
{
	int PID = Commands->Get_Preset_ID(enter);
	if (PID == 82080051 || PID == 82080029 || PID == 82080055 || PID == 82080056 || PID == 82080053)
	{// Crates or player equipment
		GameObject *Boss = Commands->Find_Object(MetroidGameControl.BossID);
		if (Boss)
			Commands->Apply_Damage(Boss,-(5.0f+Get_Player_Count()),"None",enter);
		Commands->Apply_Damage(enter,9999.9f,"Blamokiller",0);
		return;
	}
	if (!enter->As_SoldierGameObj() || !Commands->Get_Health(enter))
		return;
	GameObject *Boss = Commands->Find_Object(MetroidGameControl.BossID);
	if (Boss)
		Commands->Apply_Damage(Boss,-(Commands->Get_Health(enter)+Commands->Get_Shield_Strength(enter)+(Get_Player_Count()*2.5f)),"None",enter);
	char splatter[32];
	sprintf(splatter,"gibs%d",Commands->Get_Random_Int(0,6));
	Commands->Create_Explosion(splatter,Commands->Get_Position(enter),0);
	Commands->Set_Model(enter,"RP2InvisSpawn");
	if (Commands->Is_A_Star(enter))
	{
		char DeathMSG[512];
		sprintf(DeathMSG,"%s was turned into resources by the Mining Equipment.",Get_Player_Name(enter));
		JmgUtility::JMG_Player_Death_Message(enter,NULL,DeathMSG);
		Set_Screen_Fade_Color_Player(enter,0.75,0,0,0.1f);
		Set_Screen_Fade_Opacity_Player(enter,0.75f,0.1f);
		MetroidPSSControl.PlayerSettings[JmgUtility::JMG_Get_Player_ID(enter)]->FeedBoss++;
		if (MetroidPSSControl.PlayerSettings[JmgUtility::JMG_Get_Player_ID(enter)]->hasDied)
		{
			MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(enter)]->DeathFromMineBossResources++;
			MetroidPSSControl.PlayerSettings[JmgUtility::JMG_Get_Player_ID(enter)]->hasDied = true;
		}
	}
	Commands->Apply_Damage(enter,9999.9f,"Blamokiller",0);
}
void JMG_Metroid_Dynamic_Powerup_System::Created(GameObject *obj)
{
	MetroidPowerupSystemControl += obj;
}
void JMG_Metroid_Dynamic_Powerup_System::Destroyed(GameObject *obj)
{
	MetroidPowerupSystemControl -= obj;
}
void JMG_Metroid_Crate_Random_Powerup::Killed(GameObject *obj, GameObject *damager)
{
	if (Commands->Get_Random(0.0f,1.0f) < 0.5f)
		return;
	if (Commands->Get_Random(0.0f,1.0f) < 0.2f)
	{
		GameObject *Powerup = Commands->Create_Object(Commands->Get_Random(0.0f,1.0f) < 0.5 ? "POW_Health_050" : "CnC_Ammo_Crate",Commands->Get_Position(obj));
		Commands->Attach_Script(Powerup,"JMG_Metroid_Dynamic_Powerup_System","");
		return;
	}
	float Random = Commands->Get_Random(0.0f,1.0f);
	const char *PowerupName;
	if (Random < 0.2)//.3
		PowerupName = "POW_Imperialist";
	else if (Random < 0.4)//.55
		PowerupName = "POW_Machinegun";
	//else if (Random < 0.75)
	//	PowerupName = "POW_Lazershotgun";
	else if (Random < 0.6)
		PowerupName = "POW_Shock_coil";
	else if (Random < 0.8)
		PowerupName = "POW_Flame_Upgrade";
	else
		PowerupName = "POW_Supergrenade";
	GameObject *Powerup = Commands->Create_Object(PowerupName,Commands->Get_Position(obj));
	Commands->Attach_Script(Powerup,"JMG_Metroid_Dynamic_Powerup_System","");
}
void JMG_Metroid_Mine_Tower_Elevator::Created(GameObject *obj)
{
	char param[256];
	sprintf(param,"%d,223424,0",Commands->Get_ID(obj));
	GameObject *LockDownLasers = Commands->Create_Object("Metroid_Lockdown_Lazer_Wall",Vector3(1000.0f,524.157f,5.011f));
	LockdownMode = Commands->Get_ID(LockDownLasers);
	GameObject *LockDownButton = Commands->Create_Object("Lockdown_Button",Vector3(1000.0f,474.592f,1.5f));
	Commands->Set_Facing(LockDownButton,90.0f);
	Commands->Set_Animation(LockDownButton,"JG_Button.JG_Button",false,0,8.0f,8.0f,true);
	Commands->Attach_Script(LockDownButton,"JMG_Metroid_Send_Custom_On_Poke",param);
	LockDownButtonID = Commands->Get_ID(LockDownButton);

	ElevatorCurrentFloor = 0;
	ElevatorInMotion = false;
	GameObject *ButtonPositions = Commands->Create_Object("Daves Arrow",Vector3(1000.0f,500.0f,0.0f));
	Commands->Set_Model(ButtonPositions,"elevatorbuttons");
	sprintf(param,"%d,223425,0",Commands->Get_ID(obj));
	char ButtonPreset[256];
	for (int x = 0;x < 4;x++)
		for (int y = 0;y < 5;y++)
		{
			if (y != 4)
				if (x == y)
				{
					ElevatorButtonIDs[x][y] = 0;
					continue;
				}
				else
					sprintf(ButtonPreset,"Metroid_Elevator_Button_L%d",y);
			else
				sprintf(ButtonPreset,"Metroid_Elevator_Button_Call");
			GameObject *Button = Commands->Create_Object(ButtonPreset,Vector3(1000.0f,500.0f,0.0f));
			sprintf(ButtonPreset,"Button%d%d",x,y);
			Commands->Set_Position(Button,Commands->Get_Bone_Position(ButtonPositions,ButtonPreset));
			Commands->Set_Facing(Button,-90.0f);
			Commands->Attach_Script(Button,"JMG_Metroid_Send_Custom_On_Poke",param);
			ElevatorButtonIDs[x][y] = Commands->Get_ID(Button);
		}
	Commands->Destroy_Object(ButtonPositions);
	UpdateElevatorButtons();
	Commands->Static_Anim_Phys_Goto_Frame(456474,0.0f,"jg_elevator.jg_elevator");
}
void JMG_Metroid_Mine_Tower_Elevator::Timer_Expired(GameObject *obj,int number)
{
	if (42352355 == number)
	{
		ElevatorInMotion = false;
		UpdateElevatorButtons();
	}
}
void JMG_Metroid_Mine_Tower_Elevator::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 403025)
	{//TODO: reset
		GameObject *Boss = Commands->Find_Object(MetroidGameControl.BossID);
		if (Boss)
			Commands->Destroy_Object(Boss);
		GameObject *LockDownButton = Commands->Find_Object(LockDownButtonID);
		Commands->Set_Animation(LockDownButton,"JG_Button.JG_Button",false,0,8.0f,8.0f,true);
		GameObject *LaserWall = Commands->Find_Object(LockdownMode);
		if (LaserWall)
			Commands->Destroy_Object(LaserWall);
		GameObject *LockDownLasers = Commands->Create_Object("Metroid_Lockdown_Lazer_Wall",Vector3(1000.0f,524.157f,5.011f));
		LockdownMode = Commands->Get_ID(LockDownLasers);
		ElevatorCurrentFloor = 0;
		ElevatorInMotion = false;
		UpdateElevatorButtons();
		Commands->Static_Anim_Phys_Goto_Frame(456474,0.0f,"jg_elevator.jg_elevator");
		MetroidScoreControl.MetroidPlayerHighScoreNodes[param]->ElevatorLockdownButton++;
	}
	if (message == 223424)
	{
		GameObject *LazerWall = Commands->Find_Object(LockdownMode);
		if (!LazerWall)
			return;
		MetroidGameControl.DisplayHUDMessage(6);
		MetroidObjectiveSystemControl.Set_Objective_Status(29,NewObjectiveSystem::Accomplished);
		Commands->Apply_Damage(LazerWall,9999.9f,"BlamoKiller",0);
		Commands->Set_Animation(sender,"JG_Button.JG_Button",false,0,2.0f,3.0f,true);
		LockdownMode = 0;
		UpdateElevatorButtons();
	}
	if (message == 223425)
	{
		if (LockdownMode)
		{
			MetroidGameControl.DisplayDialogMessage(Get_GameObj(param),18);
			Commands->Create_3D_WAV_Sound_At_Bone("interface_emblem.wav",sender,"origin");
			return;
		}
		int senderID = Commands->Get_ID(sender);
		int Floor = 0;
		int ButtonFloor = 0;
		for (int x = 0;x < 4 && !Floor;x++)
			for (int y = 0;y < 5 && !Floor;y++)
				if (ElevatorButtonIDs[x][y] == senderID)
				{
					Floor = y;
					ButtonFloor = x;
				}
		if (ElevatorInMotion || (ElevatorCurrentFloor != ButtonFloor && Floor != 4) || (ElevatorCurrentFloor == ButtonFloor && Floor == 4))
		{
			Commands->Create_3D_WAV_Sound_At_Bone("interface_emblem.wav",sender,"origin");
			return;
		}
		Commands->Create_3D_WAV_Sound_At_Bone("interface_mouseclick.wav",sender,"origin");
		float FloorFrams[4] = {0.0f,59.0f,245.5f,449.0f};
		float MoveTime = abs(FloorFrams[ElevatorCurrentFloor]-FloorFrams[Floor == 4 ? ButtonFloor : Floor])/30.0f+2.0f;
		ElevatorInMotion = true;
		Commands->Static_Anim_Phys_Goto_Frame(456474,FloorFrams[Floor == 4 ? ButtonFloor : Floor],"jg_elevator.jg_elevator");
		Commands->Start_Timer(obj,this,MoveTime,42352355);
		ElevatorCurrentFloor = Floor == 4 ? ButtonFloor : Floor;
		UpdateElevatorButtons();
		Commands->Set_Animation(sender,"JG_Button.JG_Button",false,0,3.0,2.0,false);
	}
	if (message == 406919)
	{
		if (MetroidObjectiveSystemControl.Get_Objective_Status(29) == NewObjectiveSystem::NotDefined)
		{
			MetroidObjectiveSystemControl.Add_Objective(29,NewObjectiveSystem::Primary,NewObjectiveSystem::Pending,12813,"",12813,Commands->Get_Position(Commands->Find_Object(LockDownButtonID)));
			JmgUtility::SetHUDHelpText(12813,Vector3(0,1,0));
		}
	}
	if (message == 406920)
	{
		for (int x = 0;x < 3;x++)
		{
			if (MetroidObjectiveSystemControl.Get_Objective_Status(34+x) == NewObjectiveSystem::NotDefined)
			{
				MetroidObjectiveSystemControl.Add_Objective(34+x,NewObjectiveSystem::Primary,NewObjectiveSystem::Pending,12814+x,"",12814+x,Commands->Get_Position(Commands->Find_Object(MetroidGameControl.MineTerminalID[x])));
				JmgUtility::SetHUDHelpText(12814+x,Vector3(0,1,0));
			}
		}
	}
}
void JMG_Metroid_Mine_Tower_Elevator::Destroyed(GameObject *obj)
{
	GameObject *Object = Commands->Find_Object(LockDownButtonID);
	if (Object)
		Commands->Destroy_Object(Object);
	for (int x = 0;x < 4;x++)
		for (int y = 0;y < 5;y++)
		{
			Object = Commands->Find_Object(ElevatorButtonIDs[x][y]);
			if (Object)
				Commands->Destroy_Object(Object);
		}
}
void JMG_Metroid_Mine_Tower_Elevator::UpdateElevatorButtons()
{
	for (int x = 0;x < 4;x++)
		for (int y = 0;y < 5;y++)
			if (ElevatorButtonIDs[x][y])
			{
				GameObject *Button = Commands->Find_Object(ElevatorButtonIDs[x][y]);
				if (y != 4)
				{
					if (ElevatorInMotion)
						if (x != ElevatorCurrentFloor)
							Commands->Set_Animation(Button,"JG_Button.JG_Button",false,0,0.0,0.0,false);
						else
							Commands->Set_Animation(Button,"JG_Button.JG_Button",false,0,2.0,2.0,false);
					else
						if (LockdownMode)
							Commands->Set_Animation(Button,"JG_Button.JG_Button",false,0,4.0,4.0,false);
						else if (x != ElevatorCurrentFloor)
							Commands->Set_Animation(Button,"JG_Button.JG_Button",false,0,0.0,0.0,false);
						else
							Commands->Set_Animation(Button,"JG_Button.JG_Button",false,0,6.0,6.0,false);
				}
				else
				{
					if (LockdownMode)
						Commands->Set_Animation(Button,"JG_Button.JG_Button",false,0,4.0,4.0,false);
					else if (ElevatorInMotion || x == ElevatorCurrentFloor)
						Commands->Set_Animation(Button,"JG_Button.JG_Button",false,0,0.0,0.0,false);
					else if (ElevatorInMotion)
						Commands->Set_Animation(Button,"JG_Button.JG_Button",false,0,2.0,2.0,false);
					else if (x != ElevatorCurrentFloor)
						Commands->Set_Animation(Button,"JG_Button.JG_Button",false,0,6.0,6.0,false);
				}
			}
}
void JMG_Metroid_Text_Dialog::Entered(GameObject *obj,GameObject *enter)
{
	if (!Commands->Is_A_Star(enter))
		return;
	int PlayerID = JmgUtility::JMG_Get_Player_ID(enter);
	if (SupportModeSystemControl[PlayerID].LastDamaged > 40 || SupportModeSystemControl[PlayerID].LastFired > 40)
		return;
	MetroidGameControl.DisplayDialogMessage(enter,Get_Int_Parameter("StringID"));
}
void JMG_Metroid_Mine_Computer_Console::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	Commands->Set_Animation(obj,"jg_computer.jg_computer",false,0,0.0f,0.0f,false);
	for (int x = 0;x < 3;x++)
		if (!MetroidGameControl.MineTerminalID[x])
		{
			TerminalNumber = x;
			MetroidGameControl.MineTerminalID[x] = Commands->Get_ID(obj);
			break;
		}
	MetroidGameControl.MineTerminalReset[TerminalNumber] = 0;
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Metroid_Mine_Computer_Console::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (MetroidGameControl.MineTerminalReset[TerminalNumber])
		{
			MetroidGameControl.MineTerminalReset[TerminalNumber]--;
			if (!MetroidGameControl.MineTerminalReset[TerminalNumber])
			{
				Commands->Enable_HUD_Pokable_Indicator(obj,true);
				MetroidGameControl.NumberOfDeactivatedTerminals--;
				MetroidGameControl.MineTerminalDeactivated[TerminalNumber] = false;
				Commands->Set_Animation(obj,"jg_computer.jg_computer",false,0,0.0f,0.0f,false);
				for (int x = 1;x < 128;x++)
				{
					GameObject *Player = Get_GameObj(x);
					if (!Player)
						continue;
					Set_Shader_Number_Vector(Player,998,Vector4(4920037.0f,0.0f,0.0f,0.0f));
				}
			}
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Metroid_Mine_Computer_Console::Poked(GameObject *obj,GameObject *poker)
{
	int id = Commands->Get_ID(obj);
	for (int x = 0;x < 3;x++)
		if (id == MetroidGameControl.MineTerminalID[x])
			if (MetroidGameControl.MineTerminalDeactivated[x])
			{
				MetroidGameControl.DisplayDialogMessage(poker,21);
				return;
			}
	char params[512];
	sprintf(params,"%d",Commands->Get_ID(obj));
	Attach_Script_Once(poker,"JMG_Metroid_Mine_Computer_Console_Disarm_Script",params);
}
void JMG_Metroid_Mine_Computer_Console_Disarm_Script::Created(GameObject *obj)
{
	LastSelection = -1;
	CompletedCombos = 0;
	SelectRandomKey(obj);
	Commands->Start_Timer(obj,this,1.0f,4732545);
	Commands->Start_Timer(obj,this,10.0f,4732546);
}
void JMG_Metroid_Mine_Computer_Console_Disarm_Script::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 9870923)
	{// Disarm another one
		if (Get_Int_Parameter("ID") != param)
			return;
		Killed(obj,NULL);
	}
	switch (message)
	{
	case 940860500:
		break;
	case 940860501:
		break;
	case 940860502:
		break;
	case 940860503:
		break;
	case 940860504:
		break;
	case 940860505:
		break;
	case 940860506:
		break;
	case 940860507:
		break;
	case 940860508:
		break;
	case 940860509:
		break;
	default:
		return;
	}
	PressKey(obj,message);
}
void JMG_Metroid_Mine_Computer_Console_Disarm_Script::Timer_Expired(GameObject *obj,int number)
{
	if (number == 4732545)
	{
		GameObject *ControlPanel = Commands->Find_Object(Get_Int_Parameter("ID"));
		if (!ControlPanel)
		{
			Killed(obj,NULL);
			return;
		}
		if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(ControlPanel)) > 25.0f)
		{
			Set_Shader_Number_Vector(obj,997,Vector4(4920037.0f,0.0f,0.0f,0.0f));
			Killed(obj,NULL);
			return;
		}
		Commands->Start_Timer(obj,this,1.0f,4732545);
	}
	if (number == 4732546)
	{
		GameObject *ControlPanel = Commands->Find_Object(Get_Int_Parameter("ID"));
		Set_Shader_Number_Vector(obj,998,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		Commands->Apply_Damage(obj,25.0f,"Electric",ControlPanel);
		Killed(obj,NULL);
	}
}
void JMG_Metroid_Mine_Computer_Console_Disarm_Script::Killed(GameObject *obj,GameObject *killer)
{
	if (Is_Script_Attached(obj,"JMG_Metroid_Mine_Computer_Console_Disarm_Script"))
		Remove_Script(obj,"JMG_Metroid_Mine_Computer_Console_Disarm_Script");
}
void JMG_Metroid_Mine_Computer_Console_Disarm_Script::SelectRandomKey(GameObject *obj)
{
	int Temp = Commands->Get_Random_Int(0,10);
	while (LastSelection == (Temp = Commands->Get_Random_Int(0,10)));
	LastSelection = Temp;
	switch (LastSelection)
	{
	case 0:
		Set_Shader_Number_Vector(obj,0,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860500;
		break;
	case 1:
		Set_Shader_Number_Vector(obj,1,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860501;
		break;
	case 2:
		Set_Shader_Number_Vector(obj,2,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860502;
		break;
	case 3:
		Set_Shader_Number_Vector(obj,3,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860503;
		break;
	case 4:
		Set_Shader_Number_Vector(obj,4,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860504;
		break;
	case 5:
		Set_Shader_Number_Vector(obj,5,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860505;
		break;
	case 6:
		Set_Shader_Number_Vector(obj,6,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860506;
		break;
	case 7:
		Set_Shader_Number_Vector(obj,7,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860507;
		break;
	case 8:
		Set_Shader_Number_Vector(obj,8,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860508;
		break;
	case 9:
		Set_Shader_Number_Vector(obj,9,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860509;
		break;
	}
}
void JMG_Metroid_Mine_Computer_Console_Disarm_Script::PressKey(GameObject *obj,int message)
{
	if (message == NextExpectedKey)
	{
		CompletedCombos++;
		if (CompletedCombos >= 10)
		{
			MetroidGameControl.DeactivateTerminal(obj,Get_Int_Parameter("ID"));
			Killed(obj,NULL);
			return;
		}
		SelectRandomKey(obj);
		return;
	}
	GameObject *ControlPanel = Commands->Find_Object(Get_Int_Parameter("ID"));
	if (!ControlPanel)
	{
		Killed(obj,NULL);
		return;
	}
	Set_Shader_Number_Vector(obj,999,Vector4(4920037.0f,0.0f,0.0f,0.0f));
	Commands->Apply_Damage(obj,25.0f,"Electric",ControlPanel);
	Killed(obj,NULL);
}
void JMG_Metroid_Reset_Fall_Distance_On_Enter::Entered(GameObject *obj,GameObject *enter)
{
	Toggle_Fly_Mode(enter);
	Toggle_Fly_Mode(enter);
	Attach_Script_Once(enter,"JMG_Metroid_Recover_Fall_Damage_Once","");
}
void JMG_Metroid_Recover_Fall_Damage_Once::Created(GameObject *obj)
{
	Fall = false;
	Commands->Start_Timer(obj,this,5.0f,1);
}
void JMG_Metroid_Recover_Fall_Damage_Once::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage > 0 && !damager && !Fall)
	{
		Fall = true;
		Commands->Apply_Damage(obj,-damage,"Earth",0);
		if (Is_Script_Attached(obj,"JMG_Metroid_Recover_Fall_Damage_Once"))
			Remove_Script(obj,"JMG_Metroid_Recover_Fall_Damage_Once");
	}
}
void JMG_Metroid_Recover_Fall_Damage_Once::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Fall = true;
		if (Is_Script_Attached(obj,"JMG_Metroid_Recover_Fall_Damage_Once"))
			Remove_Script(obj,"JMG_Metroid_Recover_Fall_Damage_Once");
	}
}
void JMG_Metroid_Teleport_Zone::Created(GameObject *obj)
{
	enabled = true;
}
void JMG_Metroid_Teleport_Zone::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 2142)
	{
		enabled = true;
	}
	if (message == 2140)
	{
		enabled = false;
	}
}
void JMG_Metroid_Teleport_Zone::Entered(GameObject *obj,GameObject *enter)
{
	if (!JmgUtility::IsTruePhysicalObject(enter) || !enabled)
		return;
	GameObject *Object = Commands->Find_Object(Get_Int_Parameter("ObjectID"));
	Vector3 TargetPos = Get_Vector3_Parameter("Position");
	if (Object)
		TargetPos = Commands->Get_Position(Object);
	MoveablePhysClass *mphys = enter->As_PhysicalGameObj() ? enter->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	if (mphys && mphys->Find_Teleport_Location(TargetPos,Get_Float_Parameter("MaxDistance"),&TargetPos))
		Commands->Set_Position(enter,TargetPos);
}
void JMG_Metroid_Tower_Put_AI_Down_Zone::Entered(GameObject *obj,GameObject *enter)
{
	if (Commands->Is_A_Star(enter) || !enter->As_SoldierGameObj() || !MetroidGameControl.MainElevatorDown)
		return;
	Vector3 Pos = Commands->Get_Position(enter);
	Pos.Z = 9.87f;
	Toggle_Fly_Mode(enter);
	MoveablePhysClass *mphys = enter->As_PhysicalGameObj() ? enter->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	if (mphys && mphys->Find_Teleport_Location(Pos,1.5f,&Pos))
		Commands->Set_Position(enter,Pos);
	Toggle_Fly_Mode(enter);
}
void JMG_Metroid_Electrified_Water::Entered(GameObject *obj,GameObject *enter)
{
	if (!enter->As_SoldierGameObj())
		return;
	if (Commands->Is_A_Star(enter))
	{
		char DeathMSG[512];
		sprintf(DeathMSG,"%s died by falling in electrified water.",Get_Player_Name(enter));
		JmgUtility::JMG_Player_Death_Message(enter,NULL,DeathMSG);
		Set_Screen_Fade_Color_Player(enter,0,0.75,1,0.1f);
		Set_Screen_Fade_Opacity_Player(enter,0.75f,0.1f);
		MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(enter)]->DeathFromElectrifiedWater++;
	}
	Commands->Create_Sound("Electric_Ricochet_Sounds_Twiddler",Commands->Get_Position(enter),enter);
	Commands->Apply_Damage(enter,9999.9f,"Electric",0);
	Commands->Apply_Damage(enter,9999.9f,"BlamoKiller",0);
}
void JMG_Metroid_Set_Screen_Color_While_In_Zone::Entered(GameObject *obj,GameObject *enter)
{
	if (!Commands->Is_A_Star(enter))
		return;
	int PlayerID = JmgUtility::JMG_Get_Player_ID(enter);
	MetroidPSSControl.PlayerSettings[PlayerID]->ScreenColorZone++;
	Set_Screen_Fade_Color_Player(enter,Get_Vector3_Parameter("ScreenColor").X,Get_Vector3_Parameter("ScreenColor").Y,Get_Vector3_Parameter("ScreenColor").Z,0);
	Set_Screen_Fade_Opacity_Player(enter,Get_Float_Parameter("Opacity"),0);
	MetroidPSSControl.PlayerSettings[PlayerID]->ScreenColor = Get_Vector3_Parameter("ScreenColor");
	MetroidPSSControl.PlayerSettings[PlayerID]->ScreenOpacity = Get_Float_Parameter("Opacity");
}
void JMG_Metroid_Set_Screen_Color_While_In_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (!Commands->Is_A_Star(exiter))
		return;
	int PlayerID = JmgUtility::JMG_Get_Player_ID(exiter);
	if (MetroidPSSControl.PlayerSettings[PlayerID]->ScreenColorZone)
		MetroidPSSControl.PlayerSettings[PlayerID]->ScreenColorZone--;
	if (MetroidPSSControl.PlayerSettings[PlayerID]->ScreenColorZone)
		return;
	Set_Screen_Fade_Color_Player(exiter,0.0f,0.0f,0.0f,0);
	Set_Screen_Fade_Opacity_Player(exiter,0.0f,0);
	MetroidPSSControl.PlayerSettings[PlayerID]->ScreenColor = Vector3(0.0f,0.0f,0.0f);
	MetroidPSSControl.PlayerSettings[PlayerID]->ScreenOpacity = 0.0f;
}
void JMG_Metroid_Medic::Created(GameObject *obj)
{
	LastMedicalKitAmmo = Get_Total_Bullets(obj,"Weapon_Medical_Kit_Beacon");
	BlockMedicalKitRegen = 0;
	Commands->Give_PowerUp(obj,"POW_Weapon_Shield_Recharge",false);
	LastGeneratorAmmo = 0;
	BlockGeneratorRegen = 60;
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Metroid_Medic::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		int CurrentAmmo = Get_Total_Bullets(obj,"Weapon_Medical_Kit_Beacon");
		if (CurrentAmmo < LastMedicalKitAmmo)
			BlockMedicalKitRegen = 60;
		LastMedicalKitAmmo = CurrentAmmo;
		if (BlockMedicalKitRegen)
			BlockMedicalKitRegen--;
		if (!BlockMedicalKitRegen && CurrentAmmo < Get_Max_Total_Bullets(obj,"Weapon_Medical_Kit_Beacon"))
		{
			Set_Clip_Bullets(obj,"Weapon_Medical_Kit_Beacon",Get_Total_Bullets(obj,"Weapon_Medical_Kit_Beacon")+1);
			LastMedicalKitAmmo = Get_Total_Bullets(obj,"Weapon_Medical_Kit_Beacon");
			BlockMedicalKitRegen = 15;
		}
		
		CurrentAmmo = Get_Total_Bullets(obj,"Weapon_Generator_Beacon");
		if (CurrentAmmo < LastGeneratorAmmo)
			BlockGeneratorRegen = 300;
		LastGeneratorAmmo = CurrentAmmo;
		if (BlockGeneratorRegen && !MetroidPSSControl.PlayerSettings[JmgUtility::JMG_Get_Player_ID(obj)]->InStartRoom)
			BlockGeneratorRegen--;
		if (!BlockGeneratorRegen && CurrentAmmo < Get_Max_Total_Bullets(obj,"Weapon_Generator_Beacon"))
		{
			Set_Bullets(obj,"Weapon_Generator_Beacon",Get_Clip_Bullets(obj,"Weapon_Generator_Beacon")+1);
			LastGeneratorAmmo = Get_Total_Bullets(obj,"Weapon_Generator_Beacon");
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Metroid_Support_Health_Powerup_Beacon::Created(GameObject *obj)
{
	GameObject *Player = NULL;
	Player = Commands->Get_A_Star(Commands->Get_Position(obj));
	if (!Player)
	{
		Commands->Destroy_Object(obj);
		return;
	}
	float Facing = Commands->Get_Facing(Player)*PI180;
	Vector3 PlacePosition = Commands->Get_Position(Player);
	PlacePosition.X += cos(Facing)*1.5f;
	PlacePosition.Y += sin(Facing)*1.5f;
	PlacePosition.Z += 1.0f;
	//GameObject *Medpack = Commands->Create_Object(Commands->Get_Random(0.0f,1.0f) < 0.75 ? "NSE_POW_Health_025" : (Commands->Get_Random(0.0f,1.0f) < 0.75 ? "NSE_POW_Health_050" : "NSE_POW_Health_100"),PlacePosition);
	GameObject *Medpack = Commands->Create_Object("NSE_POW_Health_025",PlacePosition);
	Commands->Set_Facing(Medpack,Commands->Get_Facing(Player));
	PlayerID = JmgUtility::JMG_Get_Player_ID(Player);
	char params[256];
	sprintf(params,"%d",PlayerID);
	Commands->Attach_Script(Medpack,"JMG_Metroid_Support_Health_Powerup",params);
	Commands->Attach_Script(Medpack,"JMG_Metroid_Weapon_Pickup_Score_Monitor","14");
	MetroidScoreControl.MetroidPlayerHighScoreNodes[PlayerID]->PlacedMedPacks++;
	Commands->Destroy_Object(obj);
}
void JMG_Metroid_Support_Health_Powerup::Created(GameObject *obj)
{
	SupportModeSystemControl[Get_Int_Parameter("PlayerID")].AddMedicalPack(obj);
}
void JMG_Metroid_Support_Health_Powerup::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		GameObject *Owner = Get_GameObj(Get_Int_Parameter("PlayerID"));
		if (!Owner || Owner == sender)
			return;
		JMGGivePoints(Owner,25.0f,Get_Int_Parameter("PlayerID"));
		MetroidScoreControl.MetroidPlayerHighScoreNodes[Get_Int_Parameter("PlayerID")]->HealthPackPoints += 25;
		MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(sender)]->PickedUpPlayerPlacedHealthPack++;
		PerkSystemIncreasePlayerPerkUnlockAmount(Owner,80,1);
	}
}
void JMG_Metroid_Support_Health_Powerup::Destroyed(GameObject *obj)
{
	SupportModeSystemControl[Get_Int_Parameter("PlayerID")].RemoveMedicalPack(obj);
}
void JMG_Metroid_Support_Generator_Powerup_Beacon::Created(GameObject *obj)
{
	GameObject *Player = NULL;
	Player = Commands->Get_A_Star(Commands->Get_Position(obj));
	if (!Player)
	{
		Commands->Destroy_Object(obj);
		return;
	}
	Vector3 PlacePosition = Commands->Get_Position(Player);
	PlacePosition.Z += 0.392f;
	GameObject *Generator = Commands->Create_Object("Support_Mode_Generator",PlacePosition);
	Commands->Set_Model(Generator,"null");
	Commands->Set_Facing(Generator,Commands->Get_Facing(Player));
	char params[256];
	sprintf(params,"%d",Commands->Get_ID(obj));
	Commands->Attach_Script(Generator,"JMG_Metroid_Support_Generator_Powerup",params);
}
void JMG_Metroid_Support_Generator_Powerup::Created(GameObject *obj)
{
	GameObject *Player = NULL;
	Player = Commands->Get_A_Star(Commands->Get_Position(obj));
	if (!Player)
	{
		Commands->Destroy_Object(obj);
		return;
	}
	vampireShield = 0;
	PlayerID = JmgUtility::JMG_Get_Player_ID(Player);
	GameObject *Generator = Commands->Find_Object(SupportModeSystemControl[PlayerID].GeneratorID);
	if (Generator)
		Commands->Destroy_Object(Generator);
	currentCount = 1;
	MetroidEquipmentSystemControl += obj;
	Commands->Start_Timer(obj,this,5.0f,3);
	Commands->Start_Timer(obj,this,4.9f,4);
}
void JMG_Metroid_Support_Generator_Powerup::Timer_Expired(GameObject *obj,int number)
{
	if (number == 3)
	{
		SupportModeSystemControl[PlayerID].GeneratorID = Commands->Get_ID(obj);
		Commands->Set_Model(obj,"jGenerator");
		GameObject *aBeacon = Commands->Find_Object(Get_Int_Parameter("BeaconID"));
		if (aBeacon)
			Commands->Destroy_Object(aBeacon);
		MetroidScoreControl.MetroidPlayerHighScoreNodes[PlayerID]->PlacedRechargers++;
		Commands->Start_Timer(obj,this,0.1f,1);
		Commands->Start_Timer(obj,this,2.5f,2);
	}
	if (number == 4)
	{
		GameObject *aBeacon = Commands->Find_Object(Get_Int_Parameter("BeaconID"));
		if (!aBeacon || !Commands->Get_Health(aBeacon))
		{
			Commands->Destroy_Object(obj);
			return;
		}
	}
	if (number == 1)
	{
		currentCount++;
		if (currentCount > 10000)
			currentCount = 1;
		GameObject *Owner = Get_GameObj(PlayerID);
		Vector3 Pos = Commands->Get_Position(obj);
		for (int x = 1;x < 128;x++)
		{
			GameObject *Player = Get_GameObj(x);
			if (!Player || JmgUtility::SimpleDistance(Pos,Commands->Get_Position(Player)) > 56.25f)
				continue;
			if (SupportModeSystemControl[x].LastDamaged < 40)
			{
				float ShieldStrength = Commands->Get_Shield_Strength(Player);
				float MaxShieldStrength = Commands->Get_Max_Shield_Strength(Player);
				if (ShieldStrength < MaxShieldStrength)
				{
					float tVampShield = vampireShield ? JmgUtility::MathClamp(vampireShield,0,10) : 0;
					if (tVampShield)
						vampireShield -= tVampShield;
					Commands->Set_Shield_Strength(Player,ShieldStrength+0.5f+tVampShield);
					if (Owner != Player)
					{
						MetroidScoreControl.MetroidPlayerHighScoreNodes[PlayerID]->RechargePoints += 0.1f;
						JMGGivePoints(Owner,0.1f,PlayerID);
						PerkSystemIncreasePlayerPerkUnlockAmount(Owner,80,5);
					}
				}
				if (ShieldStrength > MaxShieldStrength)
					Commands->Set_Shield_Strength(Player,MaxShieldStrength);
			}
			if (!SupportModeSystemControl[x].LastFired)
			{
				const char *Weapon = Get_Current_Weapon(Player);
				if (!Weapon)
					continue;
				if (!_stricmp(Weapon,"Weapon_Generator_Beacon") || !_stricmp(Weapon,"Weapon_Medical_Kit_Beacon") || !_stricmp(Weapon,"Weapon_Defense_Mode_Gun_Beacon") || !_stricmp(Weapon,"Weapon_Defense_Mode_Telepad") || !_stricmp(Weapon,"Weapon_Shield_Upgrade_Normal") || !_stricmp(Weapon,"Weapon_Shield_Upgrade_Energy") || !_stricmp(Weapon,"Weapon_Shield_Upgrade_Heat") || !_stricmp(Weapon,"Weapon_Shield_Upgrade_Kinetic") || !_stricmp(Weapon,"Weapon_Shield_Upgrade_Rad") || !_stricmp(Weapon,"Weapon_Weapons_Spawner"))
					continue;
				int CurrentClipBullets = Get_Current_Clip_Bullets(Player);
				int MaxClipBullets = Get_Current_Clip_Max_Bullets(Player);
				if (CurrentClipBullets < MaxClipBullets && currentCount%((int)((100.00/(double)MaxClipBullets)*100)/10 | 1) == 0)
				{
					Set_Current_Bullets(Player,CurrentClipBullets+1);
					if (Owner != Player)
					{
						MetroidScoreControl.MetroidPlayerHighScoreNodes[PlayerID]->RechargePoints += 1.0f;
						JMGGivePoints(Owner,1.0f,PlayerID);
						PerkSystemIncreasePlayerPerkUnlockAmount(Owner,80,1);
					}
				}
			}
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
	if (number == 2)
	{
		Vector3 mypos = Commands->Get_Position(obj);
		SLNode<SmartGameObj> *Current = GameObjManager::SmartGameObjList.Head();
		while (Current) 
		{
			GameObject *o = ((GameObject *)Current->Data())->As_SmartGameObj();
			if (o && obj != o && vampireShield < 500.0f)
			{
				int team = Commands->Get_Player_Type(o);
				float shield = Commands->Get_Shield_Strength(o);
				if (team != 1 && team != -1 && shield && JmgUtility::SimpleDistance(Commands->Get_Position(o),mypos) < 225)
				{
					float takeShield = JmgUtility::MathClamp(Commands->Get_Max_Shield_Strength(o)*0.1f,0.0f,shield);
					vampireShield += takeShield*0.1f;
					if (vampireShield > 500.0f)
					{
						takeShield -= (vampireShield-500.0f);
						vampireShield = 500.0f;
					}
					GameObject *Owner = Get_GameObj(PlayerID);
					if (Owner)
					{
						MetroidScoreControl.MetroidPlayerHighScoreNodes[PlayerID]->RechargePoints += takeShield;
						if (takeShield)
							JMGGivePoints(Owner,takeShield,PlayerID);
						PerkSystemIncreasePlayerPerkUnlockAmount(Owner,80,1);
					}
					Commands->Apply_Damage(o,takeShield,"None",obj);
				}
			}
			Current = Current->Next();
		}
		Commands->Start_Timer(obj,this,1.0f,2);
	}
}
void JMG_Metroid_Support_Generator_Powerup::Destroyed(GameObject *obj)
{
	MetroidEquipmentSystemControl -= obj;
	SupportModeSystemControl[PlayerID].GeneratorID = 0;
}
void JMG_Metroid_AI_Hunt_Attack::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);
	MyLastPos = Commands->Get_Position(obj);
	LastEnemyID = 0;
	LastMoveToLocation = Vector3(0.0f,0.0f,0.0f);
	StarInRange = false;
	InnateEnabled = false;
	StartPosition = MoveToLocation = Commands->Get_Position(obj);
	MaxRange = Get_Float_Parameter("MaxStarSeekDistance")*Get_Float_Parameter("MaxStarSeekDistance");
	EnemySeen = 5;
	EnemyID = 0;
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,0.25f,1);
	Commands->Start_Timer(obj,this,1.0f,2);
	Commands->Start_Timer(obj,this,1.0f,3);
}
void JMG_Metroid_AI_Hunt_Attack::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (!Commands->Get_Health(seen))
		return;
	int seenID = Commands->Get_ID(seen);
	if (!EnemyID || seenID == EnemyID)
	{
		EnemySeen = 5;
		EnemyID = seenID;
	}
}
void JMG_Metroid_AI_Hunt_Attack::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		if (EnemySeen)
		{
			EnemySeen--;
			if (!EnemySeen)
				EnemyID = 0;
		}
		Commands->Start_Timer(obj,this,0.25f,1);
	}
	if (2 == number)
	{
		GetClosestStar(obj);
		Update(obj);
		Commands->Start_Timer(obj,this,0.1f,2);
	}
	if (3 == number)
	{
		Vector3 Pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(Pos,MyLastPos) < 1.0f)
			Commands->Set_Animation(obj,"h_a_human.h_a_422a",false,0,0.0f,0.0f,false);
		MyLastPos = Pos;
		Commands->Start_Timer(obj,this,60.0f,3);
	}
}
void JMG_Metroid_AI_Hunt_Attack::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damager && damage && !EnemyID && Commands->Get_Player_Type(damager) != Commands->Get_Player_Type(obj))
		EnemyID = Commands->Get_ID(damager);
}
void JMG_Metroid_AI_Hunt_Attack::Action_Complete(GameObject *obj,int action,ActionCompleteReason reason)
{
	if (Get_Int_Parameter("ReturnToOriginalPosition") && JmgUtility::SimpleDistance(Commands->Get_Position(obj),StartPosition) > 100.0f && !StarInRange)
	{
		MoveToLocation = StartPosition;
	}
	else if (!InnateEnabled)
	{
		InnateEnabled = true;
		if (Get_Int_Parameter("AllowInnate"))
			Commands->Innate_Enable(obj);
	}
}
void JMG_Metroid_AI_Hunt_Attack::Update(GameObject *obj)
{
	if (InnateEnabled)
		return;
	if (EnemyID || JmgUtility::SimpleDistance(LastMoveToLocation,MoveToLocation) > 15.0f)
	{
		LastEnemyID = EnemyID;
		LastMoveToLocation = MoveToLocation;
	}
	else
		return;
	ActionParamsStruct params;
	Commands->Get_Action_Params(obj,params);
	params.Set_Basic(this,10,100);
	params.Set_Movement(MoveToLocation,1.0f,5.0f,false);
	params.MovePathfind = true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,100,10);
	params.ObserverID = this->Get_ID();
	if (EnemyID)
	{
		GameObject *Enemy = Commands->Find_Object(EnemyID);
		params.Set_Attack(Enemy,300.0f,0,true);
		Commands->Action_Attack(obj,params);
	}
}
void JMG_Metroid_AI_Hunt_Attack::GetClosestStar(GameObject *obj)
{
	Vector3 Pos = Commands->Get_Position(obj);
	GameObject *AStar = Commands->Get_A_Star(Pos);
	if (AStar)
	{
		Vector3 StarPos = Commands->Get_Position(AStar);
		float TempDist = JmgUtility::SimpleDistance(StarPos,Pos);
		StarInRange = false;
		if ((MaxRange <= 0 || TempDist < MaxRange) && MaxRange > 100.0f)
		{
			MoveToLocation = StarPos;
			StarInRange = true;
			if (InnateEnabled)
			{
				InnateEnabled = false;
				Commands->Innate_Disable(obj);
			}
			return;
		}
		else if (Get_Int_Parameter("ReturnToOriginalPosition") && JmgUtility::SimpleDistance(Pos,StartPosition) > 100.0f)
		{
			MoveToLocation = StartPosition;
			if (InnateEnabled)
			{
				InnateEnabled = false;
				Commands->Innate_Disable(obj);
			}
		}
		else
		{
			MoveToLocation = Pos;
			if (!InnateEnabled)
			{
				InnateEnabled = true;
				if (Get_Int_Parameter("AllowInnate"))
					Commands->Innate_Enable(obj);
			}
		}
	}
}
void JMG_Metroid_Powered_Teleport_Zone::Created(GameObject *obj)
{
	Enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	GameObject *VisibleTeleporter = Commands->Find_Object(Get_Int_Parameter("VisibleTeleporterID"));
	if (!VisibleTeleporter)
		return;
	char animation[32];
	sprintf(animation,"%s.%s",Get_Model(VisibleTeleporter),Get_Model(VisibleTeleporter));
	Commands->Set_Animation(VisibleTeleporter,animation,false,0,Enabled,Enabled,false);
}
void JMG_Metroid_Powered_Teleport_Zone::Entered(GameObject *obj,GameObject *enter)
{
	if (!JmgUtility::IsTruePhysicalObject(enter) || !Enabled || MetroidPSSControl.PlayerSettings[JmgUtility::JMG_Get_Player_ID(enter)]->IsInTeleport)
		return;
	GameObject *Object = Commands->Find_Object(Get_Int_Parameter("ObjectID"));
	Vector3 TargetPos = Vector3(0.0f,0.0f,0.0f);
	if (Object)
		TargetPos = Commands->Get_Position(Object);
	MetroidPSSControl.PlayerSettings[JmgUtility::JMG_Get_Player_ID(enter)]->IsInTeleport = 2;
	MoveablePhysClass *mphys = enter->As_PhysicalGameObj() ? enter->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	if (mphys && mphys->Find_Teleport_Location(TargetPos,2.5f,&TargetPos))
		Commands->Set_Position(enter,TargetPos);
}
void JMG_Metroid_Powered_Teleport_Zone::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("PowerMessage"))
	{
		Enabled = param ? true : false;
		GameObject *VisibleTeleporter = Commands->Find_Object(Get_Int_Parameter("VisibleTeleporterID"));
		if (!VisibleTeleporter)
			return;
		char animation[32];
		sprintf(animation,"%s.%s",Get_Model(VisibleTeleporter),Get_Model(VisibleTeleporter));
		Commands->Set_Animation(VisibleTeleporter,animation,false,0,Enabled,Enabled,false);
	}
}
void JMG_Metroid_Send_Custom_On_Zone_Enter::Entered(GameObject *obj,GameObject *enter)
{
	GameObject *Target = Commands->Find_Object(Get_Int_Parameter("ID"));
	if (!Target)
		return;
	Commands->Send_Custom_Event(obj,Target,Get_Int_Parameter("Message"),Get_Int_Parameter("Param"),0.0f);
}
void JMG_Metroid_Sabotaged_Crate::Created(GameObject *obj)
{
	MetroidSabotagedCrateSystemControl += obj;
}
void JMG_Metroid_Enable_Spawners_On_Death::Killed(GameObject *obj, GameObject *damager)
{
	int end = Get_Int_Parameter("EndID");
	for (int x = Get_Int_Parameter("StartID");x <= end;x++)
		Commands->Enable_Spawner(x,true);
}
void JMG_Metroid_Lockdown_Controller::Created(GameObject *obj)
{
	RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].SetSpeakerLocation(Commands->Get_Position(obj));
	RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].HUDMessageLockdownInitiated = Get_Int_Parameter("HUDMsgLockdownStart");
	RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].HUDMessageLockdownAverted = Get_Int_Parameter("HUDMsgLockdownEnd");
	RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].LockDownInitiatedStringID = Get_Int_Parameter("LockDownStartMsg");
	RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].LockDownAvertedStringID = Get_Int_Parameter("LockDownEndMsg");
	sprintf(RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].LockDownInitiatedSound,"%s",Get_Parameter("LockDownStartMsg"));
	sprintf(RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].LockDownAvertedSound,"%s",Get_Parameter("LockDownEndMsg"));
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Metroid_Lockdown_Controller::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].Update();
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Metroid_Lockdown_Lockdown_Object::Created(GameObject *obj)
{
	RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].AddLockdownBlocker(obj);
}
void JMG_Metroid_Lockdown_Post_Lockdown_Object::Created(GameObject *obj)
{
	RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].AddLockdownPostBlocker(obj);
}
void JMG_Metroid_Lockdown_Pre_Lockdown_Object::Created(GameObject *obj)
{
	RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].AddLockdownPreBlocker(obj);
}
void JMG_Metroid_Lockdown_Lockdown_Send_Custom_Object::Created(GameObject *obj)
{
	RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].AddLockdownCustomObject(obj,Get_Int_Parameter("LockdownStartCustom"),Get_Int_Parameter("PreLockdownStartCustom"),Get_Int_Parameter("PostLockdownStartCustom"));
}
void JMG_Metroid_Lockdown_Lockdown_Send_Custom_Object::Destroyed(GameObject *obj)
{
	RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].RemoveLockdownCustomObject(obj);
}
void JMG_Metroid_Lockdown_Unit_Spawner::Created(GameObject *obj)
{
	RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].AddSpawnPoint(obj,Get_Parameter("PresetName"),Get_Int_Parameter("LockDownZoneID"),Get_Int_Parameter("MaxSpawnedAtOnce"),Get_Int_Parameter("MaxTotalSpawned"),Get_Int_Parameter("SpawnFequency"),Get_Int_Parameter("RespawnDelay"),Get_Int_Parameter("RandomRespawnDelay"));
}
void JMG_Metroid_Lockdown_Light::Created(GameObject *obj)
{
	RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].AddLockdownLight(obj);
}
void JMG_Metroid_Lockdown_zSpawned_Unit::Killed(GameObject *obj, GameObject *damager)
{
	RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].SpawnedUnitKilled(obj);
}
void JMG_Metroid_Lockdown_Trigger_Zone::Entered(GameObject *obj,GameObject *enter)
{
	if (RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].BeginLockDown(Get_Int_Parameter("EnemyArriveDelay"),Get_Int_Parameter("LockDownZoneID")))
		switch (Get_Int_Parameter("LockDownZoneID"))
		{
			case 0:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(enter)]->TriggeredLockdown0++;break;
			case 1:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(enter)]->TriggeredLockdown1++;break;
			case 2:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(enter)]->TriggeredLockdown2++;break;
			case 3:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(enter)]->TriggeredLockdown3++;break;
			case 4:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(enter)]->TriggeredLockdown4++;break;
			case 5:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(enter)]->TriggeredLockdown5++;break;
			case 6:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(enter)]->TriggeredLockdown6++;break;
			case 7:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(enter)]->TriggeredLockdown7++;break;
			case 8:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(enter)]->TriggeredLockdown8++;break;
			case 9:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(enter)]->TriggeredLockdown9++;break;
		}
}
void JMG_Metroid_Lockdown_Disable_Lockdown_Terminal::Created(GameObject *obj)
{
	RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].SetLockdownTerminal(obj);
}
void JMG_Metroid_Lockdown_zComputer_Console::Poked(GameObject *obj,GameObject *poker)
{
	char params[512];
	sprintf(params,"%d,%d",Commands->Get_ID(obj),Get_Int_Parameter("LockDownZoneID"));
	Attach_Script_Once(poker,"JMG_Metroid_Lockdown_zComputer_Console_Disarm_Script",params);
}
void JMG_Metroid_Lockdown_zComputer_Console_Disarm_Script::Created(GameObject *obj)
{
	LastSelection = -1;
	CompletedCombos = 0;
	SelectRandomKey(obj);
	Commands->Start_Timer(obj,this,1.0f,4732545);
	Commands->Start_Timer(obj,this,10.0f,4732546);
}
void JMG_Metroid_Lockdown_zComputer_Console_Disarm_Script::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 9870924)
	{// Disarm another one
		Killed(obj,NULL);
	}
	switch (message)
	{
	case 940860500:
		break;
	case 940860501:
		break;
	case 940860502:
		break;
	case 940860503:
		break;
	case 940860504:
		break;
	case 940860505:
		break;
	case 940860506:
		break;
	case 940860507:
		break;
	case 940860508:
		break;
	case 940860509:
		break;
	default:
		return;
	}
	PressKey(obj,message);
}
void JMG_Metroid_Lockdown_zComputer_Console_Disarm_Script::Timer_Expired(GameObject *obj,int number)
{
	if (number == 4732545)
	{
		GameObject *ControlPanel = Commands->Find_Object(Get_Int_Parameter("ID"));
		if (!ControlPanel)
		{
			Killed(obj,NULL);
			return;
		}
		if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(ControlPanel)) > 25.0f)
		{
			Set_Shader_Number_Vector(obj,997,Vector4(4920037.0f,0.0f,0.0f,0.0f));
			Killed(obj,NULL);
			return;
		}
		Commands->Start_Timer(obj,this,1.0f,4732545);
	}
	if (number == 4732546)
	{
		GameObject *ControlPanel = Commands->Find_Object(Get_Int_Parameter("ID"));
		Set_Shader_Number_Vector(obj,998,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		Commands->Apply_Damage(obj,25.0f,"Electric",ControlPanel);
		Killed(obj,NULL);
	}
}
void JMG_Metroid_Lockdown_zComputer_Console_Disarm_Script::Killed(GameObject *obj,GameObject *killer)
{
	if (Is_Script_Attached(obj,"JMG_Metroid_Lockdown_zComputer_Console_Disarm_Script"))
		Remove_Script(obj,"JMG_Metroid_Lockdown_zComputer_Console_Disarm_Script");
}
void JMG_Metroid_Lockdown_zComputer_Console_Disarm_Script::SelectRandomKey(GameObject *obj)
{
	int Temp = Commands->Get_Random_Int(0,10);
	while (LastSelection == (Temp = Commands->Get_Random_Int(0,10)));
	LastSelection = Temp;
	switch (LastSelection)
	{
	case 0:
		Set_Shader_Number_Vector(obj,0,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860500;
		break;
	case 1:
		Set_Shader_Number_Vector(obj,1,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860501;
		break;
	case 2:
		Set_Shader_Number_Vector(obj,2,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860502;
		break;
	case 3:
		Set_Shader_Number_Vector(obj,3,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860503;
		break;
	case 4:
		Set_Shader_Number_Vector(obj,4,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860504;
		break;
	case 5:
		Set_Shader_Number_Vector(obj,5,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860505;
		break;
	case 6:
		Set_Shader_Number_Vector(obj,6,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860506;
		break;
	case 7:
		Set_Shader_Number_Vector(obj,7,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860507;
		break;
	case 8:
		Set_Shader_Number_Vector(obj,8,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860508;
		break;
	case 9:
		Set_Shader_Number_Vector(obj,9,Vector4(4920037.0f,0.0f,0.0f,0.0f));
		NextExpectedKey = 940860509;
		break;
	}
}
void JMG_Metroid_Lockdown_zComputer_Console_Disarm_Script::PressKey(GameObject *obj,int message)
{
	if (message == NextExpectedKey)
	{
		CompletedCombos++;
		if (CompletedCombos >= 10)
		{
			RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].EndLockDown(true,300);
			Killed(obj,NULL);
			return;
		}
		SelectRandomKey(obj);
		return;
	}
	GameObject *ControlPanel = Commands->Find_Object(Get_Int_Parameter("ID"));
	if (!ControlPanel)
	{
		Killed(obj,NULL);
		return;
	}
	Set_Shader_Number_Vector(obj,999,Vector4(4920037.0f,0.0f,0.0f,0.0f));
	Commands->Apply_Damage(obj,25.0f,"Electric",ControlPanel);
	Killed(obj,NULL);
}
void JMG_Metroid_Lockdown_Disable_Lockdown_On_Custom_Count::Created(GameObject *obj)
{
	MessageCount = 0;
}
void JMG_Metroid_Lockdown_Disable_Lockdown_On_Custom_Count::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Message"))
	{
		MessageCount++;
		if (Get_Int_Parameter("Count") <= MessageCount)
		{
			RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].EndLockDown(Get_Int_Parameter("ResetSpawnersOnComplete") ? true : false,Get_Int_Parameter("LockoutLockdownTime"));
			MessageCount = 0;
		}
	}
}
void JMG_Metroid_Lockdown_Disable_Lockdown_On_Death::Killed(GameObject *obj,GameObject *killer)
{
	RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].EndLockDown(Get_Int_Parameter("ResetSpawnersOnComplete") ? true : false,Get_Int_Parameter("LockoutLockdownTime"));
}
void JMG_Metroid_Lockdown_Trigger_On_Message::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Message"))
	{
		if (RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].BeginLockDown(Get_Int_Parameter("EnemyArrivalDelay"),Get_Int_Parameter("LockDownZoneID")))
		{
			GameObject *player = Commands->Find_Object(param);
			if (player)	
				switch (Get_Int_Parameter("LockDownZoneID"))
				{
					case 0:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(player)]->TriggeredLockdown0++;break;
					case 1:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(player)]->TriggeredLockdown1++;break;
					case 2:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(player)]->TriggeredLockdown2++;break;
					case 3:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(player)]->TriggeredLockdown3++;break;
					case 4:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(player)]->TriggeredLockdown4++;break;
					case 5:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(player)]->TriggeredLockdown5++;break;
					case 6:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(player)]->TriggeredLockdown6++;break;
					case 7:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(player)]->TriggeredLockdown7++;break;
					case 8:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(player)]->TriggeredLockdown8++;break;
					case 9:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(player)]->TriggeredLockdown9++;break;
				}
		}
	}
}
void JMG_Metroid_Lockdown_Disable_Lockdown_On_Time_Without_Message::Created(GameObject *obj)
{
	Count = 0;
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Metroid_Lockdown_Disable_Lockdown_On_Time_Without_Message::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("ArmMessage"))
	{
		Count = Get_Int_Parameter("DisarmTime");
	}
}
void JMG_Metroid_Lockdown_Disable_Lockdown_On_Time_Without_Message::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (Count)
		{
			Count--;
			if (!Count)
				RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].EndLockDown(Get_Int_Parameter("ResetSpawnersOnComplete") ? true : false,Get_Int_Parameter("LockoutLockdownTime"));
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Metroid_Complete_Chapter::Created(GameObject *obj)
{
	enabled = true;
}
void JMG_Metroid_Complete_Chapter::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 2142)
	{
		enabled = true;
	}
	if (message == 2140)
	{
		enabled = false;
	}
}
void JMG_Metroid_Complete_Chapter::Entered(GameObject *obj,GameObject *enter)
{
	if (!Commands->Is_A_Star(enter) || !enabled)
		return;
	int PlayerID = JmgUtility::JMG_Get_Player_ID(enter);
	MetroidPSSControl.PlayerSettings[PlayerID]->ChapterCompleted(enter,Get_Int_Parameter("ChapterID"));
}
void JMG_Metroid_Defense_Mode::Created(GameObject *obj)
{
	LastGunAmmo = Get_Total_Bullets(obj,"Weapon_Defense_Mode_Gun_Beacon");
	BlockSupportGunRegen = 0;
	Commands->Give_PowerUp(obj,"POW_Weapon_Telepad",false);
	LastTelepadAmmo = 0;
	BlockTelepadRegen = 60;
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Metroid_Defense_Mode::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		int CurrentAmmo = Get_Total_Bullets(obj,"Weapon_Defense_Mode_Gun_Beacon");
		if (CurrentAmmo < LastGunAmmo)
			BlockSupportGunRegen = 450;
		LastGunAmmo = CurrentAmmo;
		if (BlockSupportGunRegen)
			BlockSupportGunRegen--;
		if (!BlockSupportGunRegen && CurrentAmmo < Get_Max_Total_Bullets(obj,"Weapon_Defense_Mode_Gun_Beacon"))
		{
			Set_Clip_Bullets(obj,"Weapon_Defense_Mode_Gun_Beacon",Get_Total_Bullets(obj,"Weapon_Defense_Mode_Gun_Beacon")+1);
			LastGunAmmo = Get_Total_Bullets(obj,"Weapon_Defense_Mode_Gun_Beacon");
		}
		
		CurrentAmmo = Get_Total_Bullets(obj,"Weapon_Defense_Mode_Telepad");
		if (CurrentAmmo < LastTelepadAmmo)
			BlockTelepadRegen = 300;
		LastTelepadAmmo = CurrentAmmo;
		if (BlockTelepadRegen && !MetroidPSSControl.PlayerSettings[JmgUtility::JMG_Get_Player_ID(obj)]->InStartRoom)
			BlockTelepadRegen--;
		if (!BlockTelepadRegen && CurrentAmmo < Get_Max_Total_Bullets(obj,"Weapon_Defense_Mode_Telepad"))
		{
			Set_Clip_Bullets(obj,"Weapon_Defense_Mode_Telepad",Get_Total_Bullets(obj,"Weapon_Defense_Mode_Telepad")+1);
			LastTelepadAmmo = Get_Total_Bullets(obj,"Weapon_Defense_Mode_Telepad");
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Metroid_Defense_Mode::Destroyed(GameObject *obj)
{
}
void JMG_Metroid_Defense_Gun_Beacon::Created(GameObject *obj)
{
	GameObject *Player = NULL;
	Player = Commands->Get_A_Star(Commands->Get_Position(obj));
	if (!Player)
	{
		Commands->Destroy_Object(obj);
		return;
	}
	Vector3 PlacePosition = Commands->Get_Position(Player);
	PlacePosition.Z += 0.75f;
	GameObject *DefGun = Commands->Create_Object("Defense_Mode_Gun",PlacePosition);
	Commands->Set_Facing(DefGun,Commands->Get_Facing(Player));
	char params[256];
	sprintf(params,"%d,%d",Commands->Get_ID(obj),JmgUtility::JMG_Get_Player_ID(Player));
	Commands->Attach_Script(DefGun,"JMG_Metroid_Defense_Gun_Powerup",params);
}
void JMG_Metroid_Defense_Gun_Powerup::Created(GameObject *obj)
{
	Count = 0;
	GameObject *Player = NULL;
	Player = Commands->Get_A_Star(Commands->Get_Position(obj));
	if (!Player)
	{
		Commands->Destroy_Object(obj);
		return;
	}
	PlayerID = JmgUtility::JMG_Get_Player_ID(Player);
	GameObject *DefGun = Commands->Find_Object(DefenseModeSystemControl.DefensePlayerDataNodes[PlayerID].DefGunID);
	if (DefGun)
		Commands->Destroy_Object(DefGun);
	MetroidEquipmentSystemControl += obj;
	Commands->Start_Timer(obj,this,0.1f,4);
}
void JMG_Metroid_Defense_Gun_Powerup::Timer_Expired(GameObject *obj,int number)
{
	if (number == 4)
	{
		Count++;
		if (Count >= 74)
		{
			DefenseModeSystemControl.DefensePlayerDataNodes[PlayerID].DefGunID = Commands->Get_ID(obj);
			GameObject *aBeacon = Commands->Find_Object(Get_Int_Parameter("BeaconID"));
			if (aBeacon)
				Commands->Destroy_Object(aBeacon);
			Commands->Attach_Script(obj,"JMG_Metroid_Camera_Behavior","90.0,0,1,0.0,0,Beep,0.5");
			MetroidScoreControl.MetroidPlayerHighScoreNodes[PlayerID]->PlacedTurrets++;
			return;
		}
		GameObject *aBeacon = Commands->Find_Object(Get_Int_Parameter("BeaconID"));
		if (!aBeacon || !Commands->Get_Health(aBeacon))
		{
			Commands->Destroy_Object(obj);
			return;
		}
		Commands->Start_Timer(obj,this,0.1f,4);
	}
}
void JMG_Metroid_Defense_Gun_Powerup::Destroyed(GameObject *obj)
{
	GameObject *aBeacon = Commands->Find_Object(Get_Int_Parameter("BeaconID"));
	if (aBeacon)
		Commands->Destroy_Object(aBeacon);
	DefenseModeSystemControl.DefensePlayerDataNodes[PlayerID].DefGunID = 0;
	MetroidEquipmentSystemControl -= obj;
}
void JMG_Metroid_Defense_Telepad_Beacon::Created(GameObject *obj)
{
	GameObject *Player = NULL;
	Player = Commands->Get_A_Star(Commands->Get_Position(obj));
	if (!Player)
	{
		Commands->Destroy_Object(obj);
		return;
	}
	GameObject *Telepad = Commands->Create_Object("Defense_Mode_Telepad",Commands->Get_Position(Player));
	Commands->Set_Animation(Telepad,"DefModeTelepad.DefModeTelepad",false,0,0,0,false);
	Commands->Set_Facing(Telepad,Commands->Get_Facing(Player));
	char params[256];
	sprintf(params,"%d,%d",Commands->Get_ID(obj),JmgUtility::JMG_Get_Player_ID(Player));
	Commands->Attach_Script(Telepad,"JMG_Metroid_Defense_Telepad_Powerup",params);
}
void JMG_Metroid_Defense_Telepad_Powerup::Created(GameObject *obj)
{
	dieTime = 300;
	Count = 0;
	GameObject *Player = NULL;
	Player = Commands->Get_A_Star(Commands->Get_Position(obj));
	if (!Player)
	{
		Commands->Destroy_Object(obj);
		return;
	}
	PlayerID = JmgUtility::JMG_Get_Player_ID(Player);
	MetroidEquipmentSystemControl += obj;
	Commands->Start_Timer(obj,this,0.1f,4);
}
void JMG_Metroid_Defense_Telepad_Powerup::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (dieTime && !Get_GameObj(PlayerID))
		{
			dieTime--;
			if (!dieTime)
				Commands->Apply_Damage(obj,9999.9f,"BlamoKiller",0);
		}
		else
			dieTime = 300;
		Commands->Start_Timer(obj,this,1.0f,1);
	}
	if (number == 4)
	{
		Count++;
		if (Count >= 99)
		{
			DefenseModeSystemControl.AddTelepad(obj,PlayerID);
			DefenseModeSystemControl.DefensePlayerDataNodes[PlayerID].TelepadID = Commands->Get_ID(obj);
			Commands->Set_Animation(obj,"DefModeTelepad.DefModeTelepad",false,0,1,1,false);
			MetroidScoreControl.MetroidPlayerHighScoreNodes[PlayerID]->PlacedTeleports++;
			GameObject *aBeacon = Commands->Find_Object(Get_Int_Parameter("BeaconID"));
			if (aBeacon)
				Commands->Destroy_Object(aBeacon);
			Commands->Start_Timer(obj,this,1.0f,1);
			return;
		}
		GameObject *aBeacon = Commands->Find_Object(Get_Int_Parameter("BeaconID"));
		if (!aBeacon || !Commands->Get_Health(aBeacon))
		{
			Commands->Destroy_Object(obj);
			return;
		}
		Commands->Start_Timer(obj,this,0.1f,4);
	}
}
void JMG_Metroid_Defense_Telepad_Powerup::Destroyed(GameObject *obj)
{
	GameObject *aBeacon = Commands->Find_Object(Get_Int_Parameter("BeaconID"));
	if (aBeacon)
		Commands->Destroy_Object(aBeacon);
	else
	{
		DefenseModeSystemControl.RemoveTelepad(obj);
		DefenseModeSystemControl.DefensePlayerDataNodes[PlayerID].TelepadID = 0;
	}
	MetroidEquipmentSystemControl -= obj;
}
void JMG_Metroid_Defense_Telepad_Zone::Entered(GameObject *obj,GameObject *enter)
{
	if (!enter->As_SoldierGameObj() || MetroidPSSControl.PlayerSettings[JmgUtility::JMG_Get_Player_ID(enter)]->IsInTeleport)
		return;
	DefenseModeSystem::TelepadNode *Node = DefenseModeSystemControl.FindNextTelepad(Get_Int_Parameter("PadID"));
	if (!Node)
		return;
	Vector3 TargetPos = Node->Pos;
	MetroidPSSControl.PlayerSettings[JmgUtility::JMG_Get_Player_ID(enter)]->IsInTeleport = 2;
	MoveablePhysClass *mphys = enter->As_PhysicalGameObj() ? enter->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	if (mphys && mphys->Find_Teleport_Location(TargetPos,0.5f,&TargetPos))
		Commands->Set_Position(enter,TargetPos);
	GameObject *Owner = Get_GameObj(Get_Int_Parameter("PlayerID"));
	Commands->Set_Shield_Strength(enter,0.0f);
	Commands->Apply_Damage(enter,0.1f,"None",0);
	Commands->Apply_Damage(enter,-0.1f,"None",0);
	MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(enter)]->UsedPlayerPlacedTeleport++;
	if (!Owner || Owner == enter)
		return;
	JMGGivePoints(Owner,25.0f,Get_Int_Parameter("PlayerID"));
	MetroidScoreControl.MetroidPlayerHighScoreNodes[Get_Int_Parameter("PlayerID")]->TeleportPoints += 25.0f;
}
void JMG_Metroid_Preplaced_Player_Telepad::Created(GameObject *obj)
{
	DefenseModeSystemControl.AddTelepad(obj,-1);
	Commands->Set_Animation(obj,"DefModeTelepad.DefModeTelepad",false,0,0,0,false);
}
void JMG_Metroid_Player_Turret_Damage::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (Commands->Get_Preset_ID(damager) == 82080053)
	{
		int DamagerID = Commands->Get_ID(damager);
		for (int x = 1;x < 128;x++)
			if (DefenseModeSystemControl.DefensePlayerDataNodes[x].DefGunID == DamagerID)
			{
				GameObject *Player = Get_GameObj(x);
				if (!Player)
					return;
				float DP = (Get_Player_Type(obj) == 1 ? -1 : 1) * Get_Damage_Points(obj)*0.5f*damage;
				if (DP)
					JMGGivePoints(Player,DP,x);
				MetroidScoreControl.MetroidPlayerHighScoreNodes[x]->DefenseGunPoints += DP;
				PerkSystemIncreasePlayerPerkUnlockAmount(Player,81,(int)DP);
				return;
			}
	}
}
void JMG_Metroid_Player_Turret_Damage::Killed(GameObject *obj, GameObject *damager)
{
	if (Commands->Get_Preset_ID(damager) == 82080053)
	{
		int DamagerID = Commands->Get_ID(damager);
		for (int x = 1;x < 128;x++)
			if (DefenseModeSystemControl.DefensePlayerDataNodes[x].DefGunID == DamagerID)
			{
				GameObject *Player = Get_GameObj(x);
				if (!Player)
					return;
				float DP = (Get_Player_Type(obj) == 1 ? -1 : 1) * Get_Death_Points(obj)*0.5f;
				if (DP)
					JMGGivePoints(Player,DP,x);
				MetroidScoreControl.MetroidPlayerHighScoreNodes[x]->DefenseGunPoints += DP;
				PerkSystemIncreasePlayerPerkUnlockAmount(Player,81,(int)DP);
				cPlayer *thePlayer = Find_Player(x);
				if (thePlayer)
					thePlayer->Set_Kills(thePlayer->Get_Kills()+1);
				return;
			}
	}
}
void JMG_Metroid_Screen_Fade_Red_On_Damage::Created(GameObject *obj)
{
	InjuryRatio = 0.0f;
	ScreenOpacicity = 0.0f;
	Commands->Start_Timer(obj,this,0.1f,98765432);
}
void JMG_Metroid_Screen_Fade_Red_On_Damage::Timer_Expired(GameObject *obj, int number)
{
	if (number == 98765432 && Commands->Get_Health(obj) > 0)
	{
		if (InjuryRatio > 0)
			InjuryRatio -= 0.01f;
		else
			InjuryRatio = 0.0f;
		if (ScreenOpacicity > 0)
			ScreenOpacicity -= 0.01f;
		else
			ScreenOpacicity = 0.0f;
		ScreenFade(obj);
		Commands->Start_Timer(obj,this,0.1f,98765432);
	}
}
void JMG_Metroid_Screen_Fade_Red_On_Damage::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage > 1.0f)
	{
		float TotalHP = Commands->Get_Max_Health(obj) + Commands->Get_Max_Shield_Strength(obj);
		float Temp = damage/TotalHP;
		if (ScreenOpacicity + Temp < 0.85)
			ScreenOpacicity += Temp;
		else
			ScreenOpacicity = 0.85f;
		if (InjuryRatio + Temp < 1.0f)
			InjuryRatio += Temp;
		else
			InjuryRatio = 1.0f;
	}
}
void JMG_Metroid_Screen_Fade_Red_On_Damage::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 596911)
	{
		float TempParam = (float)param / 100.0f;
		if (TempParam > 1.0f)
		{
			float TotalHP = Commands->Get_Max_Health(obj) + Commands->Get_Max_Shield_Strength(obj);
			float Temp = TempParam/TotalHP;
			if (ScreenOpacicity + Temp < 0.85)
				ScreenOpacicity += Temp;
			else
				ScreenOpacicity = 0.85f;
			if (InjuryRatio + Temp < 1.0f)
				InjuryRatio += Temp;
			else
				InjuryRatio = 1.0f;
		}
	}
}
void JMG_Metroid_Screen_Fade_Red_On_Damage::ScreenFade(GameObject *obj)
{
	if (!Commands->Is_A_Star(obj))
		return;
	int PlayerID = JmgUtility::JMG_Get_Player_ID(obj);
	if (MetroidPSSControl.PlayerSettings[PlayerID]->ScreenOpacity)
	{
		float Temp = 1.0f-InjuryRatio;
		Set_Screen_Fade_Color_Player(obj,(MetroidPSSControl.PlayerSettings[PlayerID]->ScreenColor.X*Temp+InjuryRatio*0.75f),MetroidPSSControl.PlayerSettings[PlayerID]->ScreenColor.Y*Temp,MetroidPSSControl.PlayerSettings[PlayerID]->ScreenColor.Z*Temp,0.1f);
		Set_Screen_Fade_Opacity_Player(obj,JmgUtility::MathClamp(ScreenOpacicity+MetroidPSSControl.PlayerSettings[PlayerID]->ScreenOpacity,0.0f,0.85f),0.1f);
	}
	else
	{
		Set_Screen_Fade_Color_Player(obj,0.75f,0.0f,0.0f,0.1f);
		Set_Screen_Fade_Opacity_Player(obj,ScreenOpacicity,0.1f);
	}
}
void JMG_Metroid_Kill_On_Damage_Amounts::Created(GameObject *obj)
{
	Enabled = true;
	char Delim = Get_Parameter("Delim")[0];
	int StringLength = strlen(Get_Parameter("Values"));
	char Value[2000];
	sprintf(Value,"\0");
	int strPos = 0;
	int DamagePos = 0;
	for (int x = 0;x < 256;x++)
		DamageAmounts[x] = 0.0f;
	for (int x = 0;x < StringLength;x++)
	{
		if (Get_Parameter("Values")[x] == Delim)
		{
			DamageAmounts[DamagePos] = (float)atof(Value);
			DamagePos++;
			strPos = 0;
		}
		else
		{
			Value[strPos] = Get_Parameter("Values")[x];
			strPos++;
		}
		Value[strPos] = '\0';
	}
	DamageAmounts[DamagePos] = (float)atof(Value);
}
void JMG_Metroid_Kill_On_Damage_Amounts::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!Enabled)
		return;
	for (int x = 0;x < 256;x++)
		if (!DamageAmounts[x])
		{
			Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
			Commands->Set_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj));
			if (damage && damager)
				JMGGivePoints(damager,-damage*Get_Damage_Points(obj),JmgUtility::JMG_Get_Player_ID(damager));
			return;
		}
		else if (abs(damage-DamageAmounts[x]) < 0.01f)
		{
			Enabled = false;
			Commands->Apply_Damage(obj,99999.9f,"BlamoKiller",damager);
			return;
		}
}
void JMG_Metroid_Grant_Keycard::Entered(GameObject *obj,GameObject *enter)
{
	Commands->Grant_Key(enter,Get_Int_Parameter("Key"),Get_Int_Parameter("Grant") ? true : false);
}
void JMG_Metroid_AI_Move_To_Position::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Metroid_AI_Move_To_Position::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		ActionParamsStruct params;
		Commands->Get_Action_Params(obj,params);
		params.Set_Basic(this,10,100);
		params.Set_Movement(Get_Vector3_Parameter("TargetSpot"),Get_Float_Parameter("Speed"),Get_Float_Parameter("ArriveDistance"),false);
		params.MovePathfind = true;
		Commands->Action_Goto(obj,params);
		params.Set_Basic(this,100,10);
	}
}
void JMG_Metroid_Reset_Screen_Color_On_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (!Commands->Is_A_Star(enter))
		return;
	int PlayerID = JmgUtility::JMG_Get_Player_ID(enter);
	MetroidPSSControl.PlayerSettings[PlayerID]->ScreenColorZone = 0;
	Set_Screen_Fade_Color_Player(enter,0.0f,0.0f,0.0f,0);
	Set_Screen_Fade_Opacity_Player(enter,0.0f,0);
	MetroidPSSControl.PlayerSettings[PlayerID]->ScreenColor = Vector3(0.0f,0.0f,0.0f);
	MetroidPSSControl.PlayerSettings[PlayerID]->ScreenOpacity = 0.0f;
}
void JMG_Metroid_AI_Hunt_Equipment::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);
	StandingStill = false;
	MyLastPos = Commands->Get_Position(obj);
	LastEnemyID = 0;
	LastMoveToLocation = Vector3(0.0f,0.0f,0.0f);
	StarInRange = false;
	InnateEnabled = false;
	StartPosition = MoveToLocation = Commands->Get_Position(obj);
	MaxRange = Get_Float_Parameter("MaxEquipmentSeekDistance")*Get_Float_Parameter("MaxEquipmentSeekDistance");
	EnemySeen = 5;
	EnemyID = 0;
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,0.25f,1);
	Commands->Start_Timer(obj,this,1.0f,2);
	Commands->Start_Timer(obj,this,1.0f,3);
}
void JMG_Metroid_AI_Hunt_Equipment::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (!Commands->Get_Health(seen))
		return;
	int seenID = Commands->Get_ID(seen);
	if (!EnemyID || seenID == EnemyID)
	{
		EnemySeen = 5;
		EnemyID = seenID;
	}
}
void JMG_Metroid_AI_Hunt_Equipment::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		if (EnemySeen)
		{
			EnemySeen--;
			if (!EnemySeen)
				EnemyID = 0;
		}
		Commands->Start_Timer(obj,this,0.25f,1);
	}
	if (2 == number)
	{
		GetClosestStar(obj);
		Update(obj);
		Commands->Start_Timer(obj,this,0.1f,2);
	}
	if (3 == number)
	{
		Vector3 Pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(Pos,MyLastPos) < 1.0f)
		{
			StandingStill = true;
			Commands->Set_Animation(obj,"h_a_human.h_a_422a",false,0,0.0f,0.0f,false);
		}
		else
			StandingStill = false;
		MyLastPos = Pos;
		Commands->Start_Timer(obj,this,10.0f,3);
	}
}
void JMG_Metroid_AI_Hunt_Equipment::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damager && damage && !EnemyID && Commands->Get_Player_Type(damager) != Commands->Get_Player_Type(obj))
		EnemyID = Commands->Get_ID(damager);
}
void JMG_Metroid_AI_Hunt_Equipment::Action_Complete(GameObject *obj,int action,ActionCompleteReason reason)
{
	if (Get_Int_Parameter("ReturnToOriginalPosition") && JmgUtility::SimpleDistance(Commands->Get_Position(obj),StartPosition) > 100.0f && !StarInRange)
	{
		MoveToLocation = StartPosition;
	}
	else if (!InnateEnabled)
	{
		InnateEnabled = true;
		if (Get_Int_Parameter("AllowInnate"))
			Commands->Innate_Enable(obj);
	}
}
void JMG_Metroid_AI_Hunt_Equipment::Update(GameObject *obj)
{
	if (InnateEnabled)
		return;
	if (EnemyID || JmgUtility::SimpleDistance(LastMoveToLocation,MoveToLocation) > 15.0f)
	{
		LastEnemyID = EnemyID;
		LastMoveToLocation = MoveToLocation;
	}
	else
		return;
	ActionParamsStruct params;
	Commands->Get_Action_Params(obj,params);
	params.Set_Basic(this,10,100);
	params.Set_Movement(MoveToLocation,1.0f,5.0f,false);
	params.MovePathfind = true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,100,10);
	params.ObserverID = this->Get_ID();
	if (EnemyID)
	{
		GameObject *Enemy = Commands->Find_Object(EnemyID);
		params.Set_Attack(Enemy,300.0f,0,true);
		Commands->Action_Attack(obj,params);
	}
}
void JMG_Metroid_AI_Hunt_Equipment::GetClosestStar(GameObject *obj)
{
	Vector3 Pos = Commands->Get_Position(obj);
	GameObject *AStar = Commands->Get_A_Star(Pos);
	if (AStar)
	{
		Vector3 StarPos = Commands->Get_Position(AStar);
		TargetEquipmentID = StandingStill ? TargetEquipmentID : 0;
		float TempDist = MetroidEquipmentSystemControl.FindNearestEquipment(&StarPos,&TargetEquipmentID);
		StarInRange = false;
		if (TempDist >= 0 && (MaxRange <= 0 || TempDist < MaxRange) && MaxRange > 100.0f)
		{
			MoveToLocation = StarPos;
			StarInRange = true;
			if (InnateEnabled)
			{
				InnateEnabled = false;
				Commands->Innate_Disable(obj);
			}
			return;
		}
		else if (Get_Int_Parameter("ReturnToOriginalPosition") && JmgUtility::SimpleDistance(Pos,StartPosition) > 100.0f)
		{
			MoveToLocation = StartPosition;
			if (InnateEnabled)
			{
				InnateEnabled = false;
				Commands->Innate_Disable(obj);
			}
		}
		else
		{
			MoveToLocation = Pos;
			if (!InnateEnabled)
			{
				InnateEnabled = true;
				if (Get_Int_Parameter("AllowInnate"))
					Commands->Innate_Enable(obj);
			}
		}
	}
}
void JMG_Metroid_Base_Defense::Created(GameObject *obj)
{
	resetTime = 0;
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	enemyID = 0;
	MinDist = Get_Float_Parameter("MinAttackDistance")*Get_Float_Parameter("MinAttackDistance");
	MaxDist = Get_Float_Parameter("MaxAttackDistance")*Get_Float_Parameter("MaxAttackDistance");
	Commands->Start_Timer(obj,this,0.1f,2);
}
void JMG_Metroid_Base_Defense::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	GameObject *vehicle = Get_Vehicle(seen);
	if (vehicle)
		seen = vehicle;
	if (!Commands->Get_Health(seen))
		return;
	float distance = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen));
	int seenId = Commands->Get_ID(seen);
	if (!enemyID && distance >= MinDist && distance <= MaxDist)
	{
		enemyID = seenId;
		ActionParamsStruct params;
		params.Set_Basic(this,100,2);
		params.Set_Attack(seen,Get_Float_Parameter("MaxAttackDistance"),0.0,true);
		Commands->Action_Attack(obj,params);
		params.AttackCheckBlocked = false;
	}
	if (enemyID == seenId)
		resetTime = Get_Float_Parameter("AttackTimer");
}
void JMG_Metroid_Base_Defense::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (enemyID)
		return;
	float distance = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(damager));
	if (distance >= MinDist && distance <= MaxDist)
	{
		enemyID = Commands->Get_ID(damager);
		ActionParamsStruct params;
		params.Set_Basic(this,100,2);
		params.Set_Attack(damager,Get_Float_Parameter("MaxAttackDistance"),1.5,true);
		params.AttackCheckBlocked = false;
		params.AttackForceFire = true;
		Commands->Action_Attack(obj,params);
		resetTime = Get_Float_Parameter("AttackTimer");
	}
}
void JMG_Metroid_Base_Defense::Timer_Expired(GameObject *obj,int number)
{
	if (number == 2)
	{
		if (enemyID)
		{
			GameObject *enemy = Commands->Find_Object(enemyID);
			if (!enemy || !Commands->Get_Health(enemy))
				enemyID = 0;
		}
		if (resetTime)
		{
			resetTime -= 0.1f;
			if (resetTime <= 0.0f)
			{
				resetTime = 0.0f;
				enemyID = 0;
				Commands->Action_Reset(obj,100);
			}
		}
		Commands->Start_Timer(obj,this,0.1f,2);
	}
}
void JMG_Metroid_Camera_Behavior::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Metroid_Camera_Behavior::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (!Commands->Get_Health(seen))
		return;
	int seenID = Commands->Get_ID(seen);
	if (!EnemyID)
	{
		Commands->Create_Sound(Get_Parameter("Alarm_Sound"),Commands->Get_Position(obj),obj);
		SeenTime = 10;
		EnemyID = seenID;
		ActionParamsStruct params;
		params.Set_Basic(this,70,1);
		if (Get_Int_Parameter("Is_Gun"))
			params.Set_Attack(seen,Get_Float_Parameter("Delay") ? 0.0f : 9999.0f,0,true);
		else
			params.Set_Attack(seen,0.0f,0,true);
		Commands->Action_Attack(obj,params);
		if (!Get_Float_Parameter("Delay"))
		{
			if (Get_Int_Parameter("Alarm_ID") && Get_Int_Parameter("Alarm_Message"))
			{
				GameObject *target = Commands->Find_Object(Get_Int_Parameter("Alarm_ID"));
				Commands->Send_Custom_Event(obj,target,Get_Int_Parameter("Alarm_Message"),EnemyID,0);
			}
		}
		else
			Commands->Send_Custom_Event(obj,obj,47498612,EnemyID,Get_Float_Parameter("Delay"));
	}
	else if (seenID == EnemyID)
		SeenTime = 10;
}
void JMG_Metroid_Camera_Behavior::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 47498612 && EnemyID == param)
	{
		if (Get_Int_Parameter("Is_Gun"))
		{
			GameObject *seen = Commands->Find_Object(EnemyID);
			ActionParamsStruct params;
			params.Set_Basic(this,70,1);
			params.Set_Attack(seen,9999.0f,0,true);
			Commands->Action_Attack(obj,params);
		}
		if (Get_Int_Parameter("Alarm_ID") && Get_Int_Parameter("Alarm_Message"))
		{
			GameObject *target = Commands->Find_Object(Get_Int_Parameter("Alarm_ID"));
			Commands->Send_Custom_Event(obj,target,Get_Int_Parameter("Alarm_Message"),EnemyID,0);
			Commands->Create_Sound(Get_Parameter("Alarm_Sound"),Commands->Get_Position(obj),obj);
			Commands->Send_Custom_Event(obj,obj,47498613,EnemyID,Get_Float_Parameter("Delay"));
		}
	}
	if (message == 47498613 && EnemyID == param)
	{
		if (Get_Int_Parameter("Alarm_ID") && Get_Int_Parameter("Alarm_Message"))
		{
			GameObject *target = Commands->Find_Object(Get_Int_Parameter("Alarm_ID"));
			Commands->Send_Custom_Event(obj,target,Get_Int_Parameter("Alarm_Message"),EnemyID,0);
			Commands->Create_Sound(Get_Parameter("Alarm_Sound"),Commands->Get_Position(obj),obj);
			Commands->Send_Custom_Event(obj,obj,47498613,EnemyID,Get_Float_Parameter("Delay"));
		}
	}
}
void JMG_Metroid_Camera_Behavior::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Enable_Hibernation(obj,false);
		Commands->Innate_Enable(obj);
		Commands->Enable_Enemy_Seen(obj,true);
		EnemyID = 0;
		Vector3 Pos = Commands->Get_Position(obj);
		float Angle = Get_Float_Parameter("Angle")/4;
		float Facing = Commands->Get_Facing(obj);
		CameraFacingCount = 0;
		IncreaseOrDecreaseCount = 1;
		Facing = Facing-(Angle*2);
		float ZPos = Commands->Get_Bone_Position(obj,"MuzzleA0").Z;
		float TempFacing = Facing*PI180;
		CameraFacingLocation[0] = Pos;
		CameraFacingLocation[0].X += cos(TempFacing)*1000.0f;
		CameraFacingLocation[0].Y += sin(TempFacing)*1000.0f;
		CameraFacingLocation[0].Z = ZPos;
		Facing = Facing+Angle;
		TempFacing = Facing*PI180;
		CameraFacingLocation[1] = Pos;
		CameraFacingLocation[1].X += cos(TempFacing)*1000.0f;
		CameraFacingLocation[1].Y += sin(TempFacing)*1000.0f;
		CameraFacingLocation[1].Z = ZPos;
		Facing = Facing+Angle;
		TempFacing = Facing*PI180;
		CameraFacingLocation[2] = Pos;
		CameraFacingLocation[2].X += cos(TempFacing)*1000.0f;
		CameraFacingLocation[2].Y += sin(TempFacing)*1000.0f;
		CameraFacingLocation[2].Z = ZPos;
		Facing = Facing+Angle;
		TempFacing = Facing*PI180;
		CameraFacingLocation[3] = Pos;
		CameraFacingLocation[3].X += cos(TempFacing)*1000.0f;
		CameraFacingLocation[3].Y += sin(TempFacing)*1000.0f;
		CameraFacingLocation[3].Z = ZPos;
		Facing = Facing+Angle;
		TempFacing = Facing*PI180;
		CameraFacingLocation[4] = Pos;
		CameraFacingLocation[4].X += cos(TempFacing)*1000.0f;
		CameraFacingLocation[4].Y += sin(TempFacing)*1000.0f;
		CameraFacingLocation[4].Z = ZPos;
		CameraFacingUpdateTime = Get_Float_Parameter("Angle")*0.0085f;
		if (CameraFacingUpdateTime < 0.01f)
			CameraFacingUpdateTime = 0.1f;
		Commands->Start_Timer(obj,this,0.0f,2);
		Commands->Start_Timer(obj,this,0.1f,3);
	}
	if (number == 2)
	{
		if (!EnemyID)
		{
			CameraFacingCount += IncreaseOrDecreaseCount;
			if (CameraFacingCount == 0 || CameraFacingCount == 4)
				IncreaseOrDecreaseCount = IncreaseOrDecreaseCount == -1 ? 1 : -1;
			ActionParamsStruct params;
			params.Set_Basic(this,70,1);
			params.Set_Attack(CameraFacingLocation[CameraFacingCount],0,0,true);
			Commands->Action_Attack(obj,params);
		}
		Commands->Start_Timer(obj,this,CameraFacingUpdateTime*Get_Float_Parameter("UpdateRateMultiplier"),2);
	}
	if (number == 3)
	{
		if (SeenTime)
		{
			SeenTime--;
			if (!SeenTime)
				EnemyID = 0;
		}
		Commands->Start_Timer(obj,this,0.1f,3);
	}
}
void JMG_Metroid_Zone_Create_Explosion::Entered(GameObject *obj,GameObject *enter)
{
	Commands->Create_Explosion(Get_Parameter("Explosion"),Commands->Get_Position(enter),enter);
}
void JMG_Metroid_Create_Object_Once_On_Enter::Created(GameObject *obj)
{
	wasCreated = false;
}
void JMG_Metroid_Create_Object_Once_On_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (wasCreated)
		return;
	wasCreated = true;
	GameObject *newObj = Commands->Create_Object(Get_Parameter("PresetName"),Get_Vector3_Parameter("Position"));
	Commands->Set_Facing(newObj,Get_Float_Parameter("Facing"));
}
void JMG_Metroid_Remove_Object_On_Enter::Entered(GameObject *obj,GameObject *enter)
{
	GameObject *object = Commands->Find_Object(Get_Int_Parameter("ID"));
	if (object)
		Commands->Destroy_Object(object);
}
void JMG_Metroid_AI_Snow_Mini_Boss::Created(GameObject *obj)
{
	LastSeen = 0;
	currentTargetID = 0;
	huntorattack = 0;
	waitcount = 0;
	speed = 2.0f;
	homelocation = Commands->Get_Position(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,0.1f,92562343);
}
void JMG_Metroid_AI_Snow_Mini_Boss::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	if (!Commands->Get_Health(seen))
		return;
	int SeenID = Commands->Get_ID(seen);
	if (!currentTargetID || !LastSeen)
	{
		LastSeen = 30;
		huntorattack = 0;
		currentTargetID = SeenID;
	}
	else if (currentTargetID == SeenID)
	{
		LastSeen = 30;
	}
}
void JMG_Metroid_AI_Snow_Mini_Boss::Timer_Expired(GameObject *obj,int number)
{
	if (92562343 == number)
	{
		if (LastSeen)
		{
			LastSeen--;
			if (!LastSeen)
				currentTargetID = 0;
		}
		if (currentTargetID)
		{
			GameObject *target = Commands->Find_Object(currentTargetID);
			if (!target || Commands->Get_Health(target) == 0.0f)
			{
				waitcount = 0;
				currentTargetID = 0;
				Commands->Action_Reset(obj,10);
				ActionParamsStruct params;
				params.Set_Basic(this,999,10);
				params.MovePathfind = false;
				params.Set_Movement(homelocation,speed,0,false);
				Commands->Action_Goto(obj,params);
			}
			else if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target)) > 40000.0f)
			{
				currentTargetID = 0;
				waitcount = 0;
				if (huntorattack == 1)
					Commands->Action_Reset(obj,10);
			}
			else
			{
				waitcount++;
				if (waitcount > 2)
				{
					waitcount = 0;
					ActionParamsStruct params;
					params.Set_Movement(target,speed,0,false);
					params.MovePathfind = false;
					Commands->Action_Goto(obj,params);
					params.Set_Basic(this,999,10);
					params.Set_Attack(target,3.0f,1,true);
					Commands->Action_Attack(obj,params);
				}
			}
		}
		Commands->Start_Timer(obj,this,0.1f,92562343);
	}
}
void JMG_Metroid_AI_Snow_Mini_Boss::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damager || Commands->Get_Player_Type(damager) != 1 || currentTargetID)
		return;
	GameObject *target = Commands->Find_Object(currentTargetID);
	if (target)
	{
		float targetDistance = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target));
		float damagerDistance = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(damager));
		if (targetDistance < damagerDistance)
			return;
	}
	LastSeen = 30;
	huntorattack = 0;
	waitcount = 0;
	currentTargetID = Commands->Get_ID(damager);
}
void JMG_Metroid_Banshee_Strike_Craft::Created(GameObject *obj)
{
	lastAnimation = -1;
	Commands->Set_Model(obj,"bansheeAtkModel");
	Commands->Set_Position(obj,Vector3(-1295.915f,-959.524f,2.195f));
	GameObject *Banshee = Commands->Create_Object("Alien_Strike_Craft",Vector3(0.0f,0.0f,0.0f));
	Commands->Attach_To_Object_Bone(Banshee,obj,"Banshee");
	Commands->Attach_Script(Banshee,"M00_Base_Defense","0.0,300.0,3");
	Commands->Attach_Script(Banshee,"JMG_Utility_Timer_Trigger_Enemy_Seen","0.1");
	BansheeID = Commands->Get_ID(Banshee);
	Animation_Complete(obj,"");
}
void JMG_Metroid_Banshee_Strike_Craft::Animation_Complete(GameObject *obj,const char *anim)
{
	int animation = Commands->Get_Random_Int(0,5);
	while (animation == lastAnimation)
		animation = Commands->Get_Random_Int(0,5);
	float animationend[5] = {899.0f,600.0f,650.0f,700.0f,950.0f};
	char animationName[32];
	sprintf(animationName,"bansheeAtkModel.bansheeAtkAnim%d",animation);
	Commands->Set_Animation(obj,animationName,false,0,0,animationend[animation],true);
	lastAnimation = animation;
}
void JMG_Metroid_Banshee_Strike_Craft::Destroyed(GameObject *obj)
{
	GameObject *Banshee = Commands->Find_Object(BansheeID);
	if (Banshee)
		Commands->Apply_Damage(Banshee,9999.9f,"BlamoKiller",0);
}
void JMG_Metroid_Cheat_Mode_Death_Counter::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (Commands->Get_Shield_Strength(obj)-damage <= 0.0f)
	{
		Commands->Set_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj));
		int playerId = JmgUtility::JMG_Get_Player_ID(obj);
		if (playerId)
		{
			cPlayer *thePlayer = Find_Player(playerId);
			thePlayer->Set_Deaths(thePlayer->Get_Deaths()+1);
		}
	}
}
void JMG_Metroid_ZeroBug_Tracker::Created(GameObject *obj)
{
	PlayerID = JmgUtility::JMG_Get_Player_ID(obj);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Metroid_ZeroBug_Tracker::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!MetroidPSSControl.PlayerSettings[PlayerID]->hasZeroBug && !Get_Money(PlayerID) && !Get_Score(PlayerID))
		{
			MetroidPSSControl.PlayerSettings[PlayerID]->hasZeroBug = true;
			char zeroBug[512];
			Vector3 Pos = Commands->Get_Position(obj);
			sprintf(zeroBug,"msg %s has contracted the ZeroBug! (%.2f %.2f %.2f)",Get_Player_Name(obj),Pos.X,Pos.Y,Pos.Z);
			Console_Input(zeroBug);
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Metroid_Zone_Damage_Object_While_In_Zone::Entered(GameObject *obj,GameObject *enter)
{
	char params[128];
	sprintf(params,"%.3f,%s,%.2f,%d",Get_Float_Parameter("DamageRate"),Get_Parameter("Warhead"),Get_Float_Parameter("Delay"),Get_Int_Parameter("Death_Zone_Type"));
	Commands->Attach_Script(enter,"JMG_Metroid_Attached_Zone_Damage_Object_While_In_Zone",params);
}
void JMG_Metroid_Zone_Damage_Object_While_In_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (Is_Script_Attached(exiter,"JMG_Metroid_Attached_Zone_Damage_Object_While_In_Zone"))
		Remove_Script(exiter,"JMG_Metroid_Attached_Zone_Damage_Object_While_In_Zone");
}
void JMG_Metroid_Attached_Zone_Damage_Object_While_In_Zone::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.0f,1);
}
void JMG_Metroid_Attached_Zone_Damage_Object_While_In_Zone::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Apply_Damage(obj,Get_Float_Parameter("DamageRate"),Get_Parameter("Warhead"),0);
		if (Get_Int_Parameter("Death_Zone_Type") && Commands->Get_Health(obj) && MetroidPSSControl.PlayerSettings[JmgUtility::JMG_Get_Player_ID(obj)]->hasDied)
		{
			switch (Get_Int_Parameter("Death_Zone_Type"))
			{
			case 1:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(obj)]->DeathFromSpikePit++;break;
			case 2:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(obj)]->DeathFromFallingOffTheTrain++;break;
			case 3:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(obj)]->DeathInLava++;break;
			case 4:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(obj)]->DeathInRadioactivePit++;break;
			case 5:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(obj)]->DeathFromReactorRadiation++;break;
			case 6:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(obj)]->DeathInFire++;break;
			case 7:MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(obj)]->DeathFromMineBossResources++;break;
			}
			MetroidPSSControl.PlayerSettings[JmgUtility::JMG_Get_Player_ID(obj)]->hasDied = true;
		}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
	}
}
void JMG_Metroid_Random_Spawn_Point_Node::Created(GameObject *obj)
{
	MetroidRandomSpawnControl.AddNode(obj,Get_Int_Parameter("SpawnGroup"));
}
void JMG_Metroid_Spawn_At_Random_Spawn_Point::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Metroid_Spawn_At_Random_Spawn_Point::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 pos = MetroidRandomSpawnControl.SelectFurthestSpawnPoint(Commands->Get_Position(obj),Get_Int_Parameter("SpawnGroup"));
		MoveablePhysClass *mphys = obj->As_PhysicalGameObj() ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
		if (mphys && mphys->Find_Teleport_Location(pos,1.0f,&pos))
			Commands->Set_Position(obj,pos);
	}
}
void JMG_Metroid_Granted_Shield_Powerup::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		int playerId = JmgUtility::JMG_Get_Player_ID(sender);
		if (!playerId)
			return;
		JMG_Metroid_Supply_Mode::ShieldUpgradeGrantedBy[playerId] = Get_Int_Parameter("GranterPlayerID");
	}
}
void JMG_Metroid_Supply_Mode::Created(GameObject *obj)
{
	Commands->Give_PowerUp(obj,"POW_Weapon_Shield_Normal_Powerup",false);
	Commands->Give_PowerUp(obj,"POW_Weapon_Shield_Energy_Powerup",false);
	Commands->Give_PowerUp(obj,"POW_Weapon_Shield_Heat_Powerup",false);
	Commands->Give_PowerUp(obj,"POW_Weapon_Shield_Kinetic_Powerup",false);
	Commands->Give_PowerUp(obj,"POW_Weapon_Shield_Rad_Powerup",false);
	LastArmorUpgradeAmmo = Get_Total_Bullets(obj,"POW_Weapon_Shield_Normal_Powerup");
	BlockArmorUpgradeRegen = 0;
	BlockWeaponSpawnRegen = 60;
	LastWeaponSpawnAmmo = 0;
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Metroid_Supply_Mode::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		int CurrentAmmo = Get_Total_Bullets(obj,"Weapon_Shield_Upgrade_Normal");
		if (CurrentAmmo > Get_Total_Bullets(obj,"Weapon_Shield_Upgrade_Energy"))
			CurrentAmmo = Get_Total_Bullets(obj,"Weapon_Shield_Upgrade_Energy");
		if (CurrentAmmo > Get_Total_Bullets(obj,"Weapon_Shield_Upgrade_Heat"))
			CurrentAmmo = Get_Total_Bullets(obj,"Weapon_Shield_Upgrade_Heat");
		if (CurrentAmmo > Get_Total_Bullets(obj,"Weapon_Shield_Upgrade_Kinetic"))
			CurrentAmmo = Get_Total_Bullets(obj,"Weapon_Shield_Upgrade_Kinetic");
		if (CurrentAmmo > Get_Total_Bullets(obj,"Weapon_Shield_Upgrade_Rad"))
			CurrentAmmo = Get_Total_Bullets(obj,"Weapon_Shield_Upgrade_Rad");
		if (CurrentAmmo < LastArmorUpgradeAmmo)
		{
			BlockArmorUpgradeRegen = 60;
			Set_Bullets(obj,"Weapon_Shield_Upgrade_Normal",CurrentAmmo);
			Set_Bullets(obj,"Weapon_Shield_Upgrade_Energy",CurrentAmmo);
			Set_Bullets(obj,"Weapon_Shield_Upgrade_Heat",CurrentAmmo);
			Set_Bullets(obj,"Weapon_Shield_Upgrade_Kinetic",CurrentAmmo);
			Set_Bullets(obj,"Weapon_Shield_Upgrade_Rad",CurrentAmmo);
			LastArmorUpgradeAmmo = CurrentAmmo;
		}
		if (BlockArmorUpgradeRegen)
			BlockArmorUpgradeRegen--;
		if (!BlockArmorUpgradeRegen && CurrentAmmo < Get_Max_Total_Bullets(obj,"Weapon_Shield_Upgrade_Normal"))
		{
			Set_Bullets(obj,"Weapon_Shield_Upgrade_Normal",Get_Bullets(obj,"Weapon_Shield_Upgrade_Normal")+1);
			Set_Bullets(obj,"Weapon_Shield_Upgrade_Energy",Get_Bullets(obj,"Weapon_Shield_Upgrade_Energy")+1);
			Set_Bullets(obj,"Weapon_Shield_Upgrade_Heat",Get_Bullets(obj,"Weapon_Shield_Upgrade_Heat")+1);
			Set_Bullets(obj,"Weapon_Shield_Upgrade_Kinetic",Get_Bullets(obj,"Weapon_Shield_Upgrade_Kinetic")+1);
			Set_Bullets(obj,"Weapon_Shield_Upgrade_Rad",Get_Bullets(obj,"Weapon_Shield_Upgrade_Rad")+1);
			LastArmorUpgradeAmmo = Get_Total_Bullets(obj,"Weapon_Shield_Upgrade_Normal");
			BlockArmorUpgradeRegen = 15;
		}
		CurrentAmmo = Get_Total_Bullets(obj,"Weapon_Weapons_Spawner");
		if (CurrentAmmo < LastWeaponSpawnAmmo)
			BlockWeaponSpawnRegen = 300;
		LastWeaponSpawnAmmo = CurrentAmmo;
		if (BlockWeaponSpawnRegen && !MetroidPSSControl.PlayerSettings[JmgUtility::JMG_Get_Player_ID(obj)]->InStartRoom)
			BlockWeaponSpawnRegen--;
		if (!BlockWeaponSpawnRegen && CurrentAmmo < Get_Max_Total_Bullets(obj,"Weapon_Weapons_Spawner"))
		{
			Set_Clip_Bullets(obj,"Weapon_Weapons_Spawner",Get_Clip_Bullets(obj,"Weapon_Weapons_Spawner")+1);
			LastWeaponSpawnAmmo = Get_Total_Bullets(obj,"Weapon_Weapons_Spawner");
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Metroid_Supply_Mode::Destroyed(GameObject *obj)
{
	ClearUpgrades(JmgUtility::JMG_Get_Player_ID(obj),Get_Player_Name(obj));
}
void JMG_Metroid_Supply_Mode::ClearUpgrades(int playerId,const char *playerName)
{
	SupplyModeSystemControl[playerId].DestroyList();
	GameObject *weaponSpawner = Commands->Find_Object(SupplyModeSystemControl[playerId].WeapoonSpawnerID);
	if (weaponSpawner)
		Commands->Destroy_Object(weaponSpawner);
	SupplyModeSystemControl[playerId].WeapoonSpawnerID = 0;
	for (int x = 1;x < 128;x++)
	{
		if (!JMG_Metroid_Supply_Mode::ShieldUpgradeGrantedBy[x] || JMG_Metroid_Supply_Mode::ShieldUpgradeGrantedBy[x] != playerId || (x == playerId && JMG_Metroid_Supply_Mode::ShieldUpgradeGrantedBy[playerId] == playerId))
			continue;
		GameObject *player = Get_GameObj(x);
		if (!player)
			continue;
		Commands->Set_Shield_Type(player,"ShieldKevlar");
		char lostmsg[220];
		sprintf(lostmsg,"Your shield upgrade was lost because %s died.",playerName);
		JmgUtility::DisplayChatMessage(player,127,255,255,lostmsg);
	}
}
void JMG_Metroid_Supply_Shield_Powerup_Beacon::Created(GameObject *obj)
{
	GameObject *Player = Commands->Get_A_Star(Commands->Get_Position(obj));
	if (!Player)
	{
		Commands->Destroy_Object(obj);
		return;
	}
	float Facing = Commands->Get_Facing(Player)*PI180;
	Vector3 PlacePosition = Commands->Get_Position(Player);
	PlacePosition.X += cos(Facing)*1.5f;
	PlacePosition.Y += sin(Facing)*1.5f;
	PlacePosition.Z += 1.0f;
	GameObject *armorUpgrade = NULL;
	const char *weap = Get_Current_Weapon(Player);
	if (!weap)
	{
		Commands->Destroy_Object(obj);
		return;
	}
	if (!_stricmp("Weapon_Shield_Upgrade_Normal",weap))
		armorUpgrade = Commands->Create_Object("POW_Normal_Shield",PlacePosition);
	else if (!_stricmp("Weapon_Shield_Upgrade_Energy",weap))
		armorUpgrade = Commands->Create_Object("POW_Energy_Shield",PlacePosition);
	else if (!_stricmp("Weapon_Shield_Upgrade_Heat",weap))
		armorUpgrade = Commands->Create_Object("POW_Heat_Shield",PlacePosition);
	else if (!_stricmp("Weapon_Shield_Upgrade_Kinetic",weap))
		armorUpgrade = Commands->Create_Object("POW_Kinetic_Shield",PlacePosition);
	else if (!_stricmp("Weapon_Shield_Upgrade_Rad",weap))
		armorUpgrade = Commands->Create_Object("POW_Radiation_Shield",PlacePosition);
	else
	{
		Commands->Destroy_Object(obj);
		return;
	}
	Commands->Set_Player_Type(armorUpgrade,1);
	Commands->Set_Facing(armorUpgrade,Commands->Get_Facing(Player));
	char params[256];
	sprintf(params,"%d",JmgUtility::JMG_Get_Player_ID(Player));
	Commands->Attach_Script(armorUpgrade,"JMG_Metroid_Supply_Shield_Powerup",params);
	Commands->Destroy_Object(obj);
}
void JMG_Metroid_Supply_Shield_Powerup::Created(GameObject *obj)
{
	MetroidScoreControl.MetroidPlayerHighScoreNodes[Get_Int_Parameter("PlayerID")]->PlacedShieldUpgrades++;
	SupplyModeSystemControl[Get_Int_Parameter("PlayerID")].AddUpgradePowerup(obj);
}
void JMG_Metroid_Supply_Shield_Powerup::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		GameObject *Owner = Get_GameObj(Get_Int_Parameter("PlayerID"));
		if (!Owner || Owner == sender)
			return;
		JMGGivePoints(Owner,10.0f,Get_Int_Parameter("PlayerID"));
		PerkSystemIncreasePlayerPerkUnlockAmount(Owner,114,10);
		MetroidScoreControl.MetroidPlayerHighScoreNodes[Get_Int_Parameter("PlayerID")]->ShieldUpgradePoints += 10.0f;
		MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(sender)]->PickedUpPlayerShieldUpgrade++;
	}
}
void JMG_Metroid_Supply_Shield_Powerup::Destroyed(GameObject *obj)
{
	SupplyModeSystemControl[Get_Int_Parameter("PlayerID")].RemoveUpgradePowerup(obj);
}
void JMG_Metroid_Supply_Weapon_Spawner_Beacon::Created(GameObject *obj)
{
	GameObject *Player = Commands->Get_A_Star(Commands->Get_Position(obj));
	if (!Player)
	{
		Commands->Destroy_Object(obj);
		return;
	}
	int playerID = JmgUtility::JMG_Get_Player_ID(Player);
	GameObject *oldWeapon = Commands->Find_Object(SupplyModeSystemControl[playerID].WeapoonSpawnerID);
	if (oldWeapon)
		Commands->Destroy_Object(oldWeapon);
	float random = Commands->Get_Random(0.0f,1.0f);
	MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->PlacedWeaponSpawners++;
	char weapon[256];
	if (random < 0.3)
		if (Commands->Get_Random(0.0f,1.0f) < 0.75f)
			sprintf(weapon,"POW_Chicken");
		else
			sprintf(weapon,"POW_Robotic_Chicken");
	else if (random < 0.475)
		if (Commands->Get_Random(0.0f,1.0f) < 0.75f)
			sprintf(weapon,"POW_Lazershotgun");
		else
			sprintf(weapon,"POW_Robotic_Laser_Shotgun");
	else if (random < 0.625)
		if (Commands->Get_Random(0.0f,1.0f) < 0.75f)
			sprintf(weapon,"POW_Flame_Upgrade");
		else
			sprintf(weapon,"POW_Robotic_Flame_Upgrade");
	else if (random < 0.75)
		if (Commands->Get_Random(0.0f,1.0f) < 0.75f)
			sprintf(weapon,"POW_Shock_coil");
		else
			sprintf(weapon,"POW_Robotic_Shock_Coil");
	else if (random < 0.85)
		if (Commands->Get_Random(0.0f,1.0f) < 0.75f)
			sprintf(weapon,"POW_Imperialist");
		else
			sprintf(weapon,"POW_Robotic_Imperialist");
	else if (random < 0.925)
		if (Commands->Get_Random(0.0f,1.0f) < 0.75f)
			sprintf(weapon,"POW_Machinegun");
		else
			sprintf(weapon,"POW_Robotic_MachineGun");
	else
		sprintf(weapon,"POW_Supergrenade");

	GameObject *newObj = Commands->Create_Object("Daves Arrow",Commands->Get_Position(Player));
	SupplyModeSystemControl[playerID].WeapoonSpawnerID = Commands->Get_ID(newObj);
	Commands->Set_Facing(newObj,Commands->Get_Facing(Player));
	Commands->Set_Model(newObj,"null");
	char params[220];
	sprintf(params,"%d,%s",playerID,weapon);
	Commands->Attach_Script(newObj,"JMG_Metroid_Supply_Weapon_Spawner_Decoy",params);
	Commands->Destroy_Object(obj);
}
void JMG_Metroid_Supply_Weapon_Spawner_Decoy::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,2.5f,1);
}
void JMG_Metroid_Supply_Weapon_Spawner_Decoy::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(pos,Commands->Get_Position(Commands->Get_A_Star(pos))) < 2)
		{
			Commands->Start_Timer(obj,this,0.1f,1);
			return;
		}
		GameObject *weaponSpawner = Commands->Create_Object(Get_Parameter("PowerupPreset"),pos);
		SupplyModeSystemControl[Get_Int_Parameter("PlayerID")].WeapoonSpawnerID = Commands->Get_ID(weaponSpawner);
		Commands->Set_Facing(weaponSpawner,Commands->Get_Facing(obj));
		char params[220];
		sprintf(params,"%d,%s",Get_Int_Parameter("PlayerID"),Get_Parameter("PowerupPreset"));
		Commands->Attach_Script(weaponSpawner,"JMG_Metroid_Supply_Weapon_Spawner_Weapon",params);
		Commands->Set_Shield_Type(weaponSpawner,"None");
		Set_Skin(weaponSpawner,"None");
		Commands->Set_Player_Type(weaponSpawner,1);
		Commands->Destroy_Object(obj);
	}
}
void JMG_Metroid_Supply_Weapon_Spawner_Weapon::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		GameObject *Owner = Get_GameObj(Get_Int_Parameter("PlayerID"));
		if (Owner && Owner != sender)
		{
			JMGGivePoints(Owner,10.0f,Get_Int_Parameter("PlayerID"));
			PerkSystemIncreasePlayerPerkUnlockAmount(Owner,114,1);
			MetroidScoreControl.MetroidPlayerHighScoreNodes[Get_Int_Parameter("PlayerID")]->WeaponSpawnPoints += 10.0;
		}
		MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(sender)]->PickedUpPlayerPlacedWeapon++;
		GameObject *newObj = Commands->Create_Object("Daves Arrow",Commands->Get_Position(obj));
		SupplyModeSystemControl[Get_Int_Parameter("PlayerID")].WeapoonSpawnerID = Commands->Get_ID(newObj);
		Commands->Set_Facing(newObj,Commands->Get_Facing(obj));
		Commands->Set_Model(newObj,"null");
		char params[220];
		sprintf(params,"%d,%s",Get_Int_Parameter("PlayerID"),Get_Parameter("PowerupPreset"));
		Commands->Attach_Script(newObj,"JMG_Metroid_Supply_Weapon_Spawner_Decoy",params);
		Commands->Destroy_Object(obj);
	}
}
void JMG_Metroid_Display_Message_On_Pickup::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		JmgUtility::DisplayChatMessage(sender,(int)Get_Vector3_Parameter("Color[R|G|B]").X,(int)Get_Vector3_Parameter("Color[R|G|B]").Y,(int)Get_Vector3_Parameter("Color[R|G|B]").Z,Get_Parameter("PickupMessage"));
	}
}
void JMG_Metroid_Tutorial_Teleport::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 911)
	{
		GameObject *target = Commands->Find_Object(param);
		if (!target)
			return;
		Commands->Set_Position(target,Vector3(550.0f,-410.0f,0.25f));
		MetroidGameControl.DisplayDialogMessage(target,20);
	}
}
void JMG_Metroid_Shield_Regen::Created(GameObject *obj)
{
	playerId = JmgUtility::JMG_Get_Player_ID(obj);
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
}
void JMG_Metroid_Shield_Regen::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (SupportModeSystemControl[playerId].LastDamaged < 20)
		{
			float shield = Commands->Get_Shield_Strength(obj)+Get_Float_Parameter("Points");
			if (shield > Commands->Get_Max_Shield_Strength(obj))
				shield = Commands->Get_Max_Shield_Strength(obj);
			Commands->Set_Shield_Strength(obj,shield);
		}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
	}
}
void JMG_Metroid_Transform_Into_Cinematic_Object::Created(GameObject *obj)
{
	Commands->Set_Model(obj,"null");
	Commands->Attach_Script(obj,"Test_Cinematic",Get_Parameter("ControlFileName"));
}
void JMG_Metroid_Grant_Phone_If_Pkg_Is_Rp2::Created(GameObject *obj)
{
	if (!JMG_Metroid_Game_Initilization::IsRp2Hosted)
		return;
	Attach_Script_Once(obj,"JMG_RP2_Player_Phone_Control_Script","");
	Commands->Give_PowerUp(obj,"POW_Phone",false);
	Commands->Attach_Script(obj,"JMG_Keyboard_Input_Custom","Primary_Fire,21511235");	
	Commands->Attach_Script(obj,"JMG_Keyboard_Input_Custom","Secondary_Fire,21511236");
	Commands->Attach_Script(obj,"JMG_Keyboard_Input_Custom","Tertiary_Fire,21511237");
	Commands->Attach_Script(obj,"JMG_Keyboard_Input_Custom","Phone,331194043");	
}
void JMG_Metroid_Boss_HP_Modifier::Created(GameObject *obj)
{
	maxShield = JmgUtility::MathClamp((float)Get_Player_Count()/10.0f,0.0f,1.0f)*Commands->Get_Max_Shield_Strength(obj);
	Commands->Set_Shield_Strength(obj,maxShield);
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Metroid_Boss_HP_Modifier::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (Commands->Get_Health(obj) < Commands->Get_Max_Health(obj))
		{
			float health = Commands->Get_Health(obj)+JmgUtility::MathClamp((float)Get_Player_Count()/10.0f,0.0f,1.0f)*Get_Float_Parameter("Points");
			if (health > Commands->Get_Max_Health(obj))
				health = Commands->Get_Max_Health(obj);
			Commands->Set_Health(obj,health);
		}
		else if (Commands->Get_Shield_Strength(obj) < Commands->Get_Max_Shield_Strength(obj))
		{
			float shield = Commands->Get_Shield_Strength(obj)+JmgUtility::MathClamp((float)Get_Player_Count()/10.0f,0.0f,1.0f)*Get_Float_Parameter("Points");
			if (shield > Commands->Get_Max_Shield_Strength(obj))
				shield = Commands->Get_Max_Shield_Strength(obj);
			Commands->Set_Shield_Strength(obj,shield);
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Metroid_Set_Is_Rendered_On_Custom::Created(GameObject *obj)
{
	Commands->Set_Is_Rendered(obj,Get_Int_Parameter("RenderedOnCreate") ? true : false);
}
void JMG_Metroid_Set_Is_Rendered_On_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("ShowMessage") && Get_Int_Parameter("ShowMessage") != -1)
	{
		Commands->Set_Is_Rendered(obj,true);
	}
	if (message == Get_Int_Parameter("HideMessage") && Get_Int_Parameter("HideMessage") != -1)
	{
		Commands->Set_Is_Rendered(obj,false);
	}
}
void JMG_Metroid_AI_Shield_Regen::Created(GameObject *obj)
{
	lastDamaged = 0;
	Commands->Start_Timer(obj,this,1.0,1);
}
void JMG_Metroid_AI_Shield_Regen::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (lastDamaged)
			lastDamaged--;
		if (!lastDamaged)
			if (Commands->Get_Health(obj) < Commands->Get_Max_Health(obj))
			{
				float health = Commands->Get_Health(obj)+10;
				if (health > Commands->Get_Max_Health(obj))
					health = Commands->Get_Max_Health(obj);
				Commands->Set_Health(obj,health);
			}
			else if (Commands->Get_Shield_Strength(obj) < Commands->Get_Max_Shield_Strength(obj))
			{
				float shield = Commands->Get_Shield_Strength(obj)+10;
				if (shield > Commands->Get_Max_Shield_Strength(obj))
					shield = Commands->Get_Max_Shield_Strength(obj);
				Commands->Set_Shield_Strength(obj,shield);
			}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Metroid_AI_Shield_Regen::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage > 0)
		lastDamaged = JMG_Metroid_Game_Control::EnemyShieldRegenStartTime;
}
void JMG_Metroid_Invincible_Object::Created(GameObject *obj)
{
	Set_Skin(obj,"Blamo");
	Commands->Set_Shield_Type(obj,"Blamo");
}
void JMG_Metroid_Damage_Score_Monitor::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0 || !damager || !Commands->Is_A_Star(damager))
		return;
	int playerId = JmgUtility::JMG_Get_Player_ID(damager);
	float points = (Get_Player_Type(obj) == 1 ? -1 : 1) * Get_Damage_Points(obj)*damage;
	MetroidScoreControl.MetroidPlayerHighScoreNodes[playerId]->pointsThisRound += points;
	MetroidScoreControl.MetroidPlayerHighScoreNodes[playerId]->Points += points;
}
void JMG_Metroid_Damage_Score_Monitor::Killed(GameObject *obj, GameObject *damager)
{
	if (!damager || !Commands->Is_A_Star(damager))
		return;
	MetroidScoreSystem::MetroidHighScoresNode *pobj = MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(damager)];
	switch (Commands->Get_Preset_ID(obj))
	{
		case 82080082: 
			pobj->KilledSecurityCameras++;break;
		case 1000000808:
			pobj->KilledMiniBoss++;break;
		case 82080053:
			pobj->KilledDefenseCameras++;break;
		case 81930093:case 1000000107:case 1000000399:case 81930100:case 81930101:case 81930104:case 1000000134:case 81930102:case 81930105:case 1000000404:case 1000000123: 
			pobj->KilledAliens++;break;
		case 81930084:case 1000000245:case 81930087:case 81930085:case 81930089:case 81930086:case 81930095:case 81930096:case 81930088:case 1000000249: 
			pobj->KilledRobots++;break;
		case 81930094:case 81930097:case 81930098: 
			pobj->KilledTraitors++;break;
		case 82080078:case 82080079:
			pobj->KilledAttackCameras++;break;
		case 82080037:case 82080129:case 82080141:case 82080131:case 82080124:case 82080128:
			pobj->KilledGunEmplacements++;break;
		case 82080097:
			pobj->KilledTurretBoss++;break;
		case 82080095:
			pobj->KilledBansheeBoss++;break;
		case 82080043:
			pobj->KilledMineBoss++;break;
		case 82080051:case 82080029:case 81960277:case 81960291:
			pobj->KilledCrates++;break;
	}
	switch (JmgUtility::JMG_Get_Current_Weapon_ID(damager))
	{
		case 409610067:pobj->KillsWithDefenseGun++;break;
		case 1000000285:case 409610130:pobj->KillsWithCheatGun++;break;
		case 409610048:pobj->KillsWithFlameUpgrade++;break;
		case 409610022:pobj->KillsWithRoboticFlameUpgrade++;break;
		case 1000000416:pobj->KillsWithGlitchGun++;break;
		case 409610017:pobj->KillsWithImperialist++;break;
		case 409610020:pobj->KillsWithRoboticImperialist++;break;
		case 409610021:pobj->KillsWithLaserShotgun++;break;
		case 409610023:pobj->KillsWithRoboticLaserShotgun++;break;
		case 409610025:pobj->KillsWithMachineGun++;break;
		case 409610089:pobj->KillsWithRoboticMachineGun++;break;
		case 409610016:pobj->KillsWithShockCoil++;break;
		case 409610024:pobj->KillsWithRoboticShockCoil++;break;
		case 409610033:pobj->KillsWithLaser++;break;
		case 409610031:pobj->KillsWithRoboticLaser++;break;
		case 409610026:pobj->KillsWithHyperdeath++;break;
		case 409610029:pobj->KillsWithSuperHyperdeath++;break;
		case 409610119:pobj->KillsWithKnife++;break;
		case 409610133:pobj->KillsWithGrenade++;break;
		case 409610134:pobj->KillsWithRoboticGrenade++;break;
		default:pobj->KillsWithUnknownWeapon++;break;
	}
	float points = (Get_Player_Type(obj) == 1 ? -1 : 1) * Get_Death_Points(obj);
	pobj->pointsThisRound += points;
	pobj->Points += points;
	pobj->Kills++;
}
void JMG_Metroid_Death_Zone_Score_Monitor::Entered(GameObject *obj,GameObject *enter)
{
	if (!Commands->Is_A_Star(enter))
		return;
	int playerID = JmgUtility::JMG_Get_Player_ID(enter);
	if (MetroidPSSControl.PlayerSettings[playerID]->hasDied)
		return;
	MetroidPSSControl.PlayerSettings[playerID]->hasDied = true;
	switch (Get_Int_Parameter("Death_Zone_Type"))
	{
	case 1:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->DeathFromSpikePit++;break;
	case 2:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->DeathFromFallingOffTheTrain++;break;
	case 3:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->DeathInLava++;break;
	case 4:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->DeathInRadioactivePit++;break;
	case 5:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->DeathFromReactorRadiation++;break;
	case 6:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->DeathInFire++;break;
	case 7:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->DeathFromMineBossResources++;break;
	}
}
void JMG_Metroid_Weapon_Pickup_Score_Monitor::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		if (!Commands->Is_A_Star(sender))
			return;
		int playerID = JmgUtility::JMG_Get_Player_ID(sender);
		int pt = Get_Int_Parameter("PowerUp_Type");
		switch (pt)
		{
		case 0:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->PickedUpPlayerPlacedWeapon++;break;
		case 1:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->PickedUpLaser++;break;
		case 2:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->PickedUpShotgun++;break;
		case 3:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->PickedUpSniper++;break;
		case 4:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->PickedUpFlame++;break;
		case 5:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->PickedUpShockCoil++;break;
		case 6:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->PickedUpMachineGun++;break;
		case 7:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->PickedUpRoboticLaser++;break;
		case 8:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->PickedUpRoboticShotgun++;break;
		case 9:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->PickedUpRoboticSniper++;break;
		case 10:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->PickedUpRoboticFlame++;break;
		case 11:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->PickedUpRoboticShockCoil++;break;
		case 12:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->PickedUpRoboticMachineGun++;break;
		case 13:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->PickedUpGlitchGun++;break;
		case 14:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->PickedUpHealthPack++;break;
		case 15:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->PickedUpShieldUpgrade++;break;
		case 16:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->PickedUpPlayerShieldUpgrade++;break;
		case 17:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->PickedUpGrenadeLaunchers++;break;
		case 18:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->FoundCheatGun++;break;
		case 19:if (!MetroidPSSControl.PlayerSettings[playerID]->hasKeycard)
		   {
			   MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->GotKeycard++;
			   MetroidPSSControl.PlayerSettings[playerID]->hasKeycard = true;
		   }break;
		}
	}
}
void JMG_Metroid_Count_Jammed_Door_Destruction::Killed(GameObject *obj, GameObject *damager)
{
	if (!damager || !Commands->Is_A_Star(damager))
		return;
	MetroidScoreControl.MetroidPlayerHighScoreNodes[JmgUtility::JMG_Get_Player_ID(damager)]->KilledBrokenDoors;
}
void JMG_Metroid_Zone_Score_Monitor::Entered(GameObject *obj,GameObject *enter)
{
	if (!Commands->Is_A_Star(enter))
		return;
	int playerID = JmgUtility::JMG_Get_Player_ID(enter);
	switch (Get_Int_Parameter("Zone_Type"))
	{
	case 1:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->GlitchModeFound++;break;
	case 2:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->GlitchModeCompleted++;break;
	case 3:if (!MetroidPSSControl.PlayerSettings[playerID]->openedSecurityDoor && Commands->Has_Key(enter,2))
		   {
			   MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->OpenedFirstDoor++;
			   MetroidPSSControl.PlayerSettings[playerID]->openedSecurityDoor = true;
		   }break;
	case 4:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->KilledByRapids++;break;
	case 5:MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->KilledByFall++;break;
	case 6:if (!MetroidGameControl.deactivatedLaserFence)
		   {
			   MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->TurnedOffLaserFence++;
			   MetroidGameControl.deactivatedLaserFence = true;
		   }break;
	case 7:if (!MetroidGameControl.backLaserFence)
		   {
			   MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->BackLaserFence++;
			   MetroidGameControl.backLaserFence = true;
		   }break;
	case 8:if (!MetroidGameControl.reactorPowerRestored)
		   {
			   MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->ReactorPowerRestored++;
			   MetroidGameControl.reactorPowerRestored = true;
		   }break;
	case 9:if (!MetroidGameControl.cityShieldLowered)
		   {
			   MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->CityShieldLowered++;
			   MetroidGameControl.cityShieldLowered = true;
		   }break;
	case 10:if (!MetroidGameControl.electrifiedWaterTurnedOff)
		   {
			   MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->ElectrifiedWaterTurnedOff++;
			   MetroidGameControl.electrifiedWaterTurnedOff = true;
		   }break;
	case 11:if (!MetroidGameControl.tunnelEntryLowered)
		   {
			   MetroidScoreControl.MetroidPlayerHighScoreNodes[playerID]->TunnelEntryLowered++;
			   MetroidGameControl.tunnelEntryLowered = true;
		   }break;
	}
}
void JMG_Metroid_Lockdown_zComputer_Console_Special_Disarm_Script::Created(GameObject *obj)
{
	lastPoke = clock();
	Timeout(obj);
	Commands->Set_Model(obj,"hackterminal");
	Commands->Set_Animation(obj,"hackterminal.hackterminal",false,0,0.0f,0.0f,false);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Metroid_Lockdown_zComputer_Console_Special_Disarm_Script::Poked(GameObject *obj,GameObject *poker)
{
	if (!RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].GetLockdownInProgress())
		return;
	if (difftime(clock(),lastPoke) < 50)
		return;
	lastPoke = clock();
	subtractionGrowth = 0;
	pokeAmount += 0.33f;
	if (pokeAmount >= 100.0f)
	{
		RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].EndLockDown(true,300);
		pokeAmount = 0;
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		Commands->Set_Animation(obj,"hackterminal.hackterminal",false,0,190.0f,190.0f,false);
		Killed(obj,NULL);
		return;
	}
	float frame = pokeAmount/100.0f*189.0f;
	Commands->Set_Animation(obj,"hackterminal.hackterminal",false,0,frame,frame,false);
	Set_Shader_Number_Vector(obj,999,Vector4(4920037.0f,0.0f,0.0f,0.0f));
}
void JMG_Metroid_Lockdown_zComputer_Console_Special_Disarm_Script::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].GetLockdownInProgress())
			return;
		if (RoomLockdownControl[Get_Int_Parameter("LockDownZoneID")].GetLockdownInProgress())
		{
			if (pokeAmount)
			{
				pokeAmount -= subtractionGrowth*0.02f;
				if (pokeAmount <= 0.0f)
					pokeAmount = 0.0f;
				float frame = pokeAmount/100.0f*189.0f;
				Commands->Set_Animation(obj,"hackterminal.hackterminal",false,0,frame,frame,false);
			}		
			if (subtractionGrowth < 500)
				subtractionGrowth++;		
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Metroid_Lockdown_zComputer_Console_Special_Disarm_Script::Killed(GameObject *obj,GameObject *killer)
{
	if (Is_Script_Attached(obj,"JMG_Metroid_Lockdown_zComputer_Console_Special_Disarm_Script"))
		Remove_Script(obj,"JMG_Metroid_Lockdown_zComputer_Console_Special_Disarm_Script");
}
void JMG_Metroid_Lockdown_zComputer_Console_Special_Disarm_Script::Timeout(GameObject *obj)
{
	pokeAmount = 0.0f;
	subtractionGrowth = 0;
	Commands->Set_Animation(obj,"hackterminal.hackterminal",false,0,0.0f,0.0f,false);
}
void JMG_Metroid_Mine_Computer_Console_Script_Special::Created(GameObject *obj)
{
	for (int x = 0;x < 3;x++)
		if (!MetroidGameControl.MineTerminalID[x])
		{
			TerminalNumber = x;
			MetroidGameControl.MineTerminalID[x] = Commands->Get_ID(obj);
			break;
		}
	lastPoke = clock();
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	MetroidGameControl.MineTerminalReset[TerminalNumber] = 0;
	Commands->Set_Model(obj,"hackterminal");
	Commands->Set_Animation(obj,"hackterminal.hackterminal",false,0,1.0f,1.0f,false);
	pokeAmount = 0.0f;
	subtractionGrowth = 0;
	Timeout(obj);
	Commands->Start_Timer(obj,this,0.1f,1);
	Commands->Start_Timer(obj,this,1.0f,2);
}
void JMG_Metroid_Mine_Computer_Console_Script_Special::Poked(GameObject *obj,GameObject *poker)
{
	if (MetroidGameControl.MineTerminalReset[TerminalNumber])
		return;
	if (difftime(clock(),lastPoke) < 50)
		return;
	lastPoke = clock();
	subtractionGrowth = 0;
	pokeAmount += 0.33f;
	if (pokeAmount >= 100.0f)
	{
		MetroidGameControl.DeactivateTerminal(obj,Commands->Get_ID(obj));
		pokeAmount = 0;
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		Commands->Set_Animation(obj,"hackterminal.hackterminal",false,0,190.0f,190.0f,false);
		return;
	}
	float frame = pokeAmount/100.0f*189.0f;
	Commands->Set_Animation(obj,"hackterminal.hackterminal",false,0,frame,frame,false);
	Set_Shader_Number_Vector(obj,999,Vector4(4920037.0f,0.0f,0.0f,0.0f));
}
void JMG_Metroid_Mine_Computer_Console_Script_Special::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!MetroidGameControl.MineTerminalReset[TerminalNumber])
		{
			if (pokeAmount)
			{
				pokeAmount -= subtractionGrowth*0.02f;
				if (pokeAmount <= 0.0f)
					pokeAmount = 0.0f;
				float frame = pokeAmount/100.0f*189.0f;
				Commands->Set_Animation(obj,"hackterminal.hackterminal",false,0,frame,frame,false);
			}			
			if (subtractionGrowth < 500)
				subtractionGrowth++;
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
	if (number == 2)
	{
		if (MetroidGameControl.MineTerminalReset[TerminalNumber])
		{
			MetroidGameControl.MineTerminalReset[TerminalNumber]--;
			if (!MetroidGameControl.MineTerminalReset[TerminalNumber])
			{
				if (MetroidObjectiveSystemControl.Get_Objective_Status(34+TerminalNumber) == NewObjectiveSystem::Accomplished)
				{
					MetroidObjectiveSystemControl.Remove_Objective(34+TerminalNumber);
					MetroidObjectiveSystemControl.Add_Objective(34+TerminalNumber,NewObjectiveSystem::Primary,NewObjectiveSystem::Pending,12814+TerminalNumber,"",12814+TerminalNumber,Commands->Get_Position(Commands->Find_Object(MetroidGameControl.MineTerminalID[TerminalNumber])));
				}
				Commands->Enable_HUD_Pokable_Indicator(obj,true);
				MetroidGameControl.NumberOfDeactivatedTerminals--;
				MetroidGameControl.MineTerminalDeactivated[TerminalNumber] = false;
				Timeout(obj);
				for (int x = 1;x < 128;x++)
				{
					GameObject *Player = Get_GameObj(x);
					if (!Player)
						continue;
					Set_Shader_Number_Vector(Player,998,Vector4(4920037.0f,0.0f,0.0f,0.0f));
				}
			}
		}
		Commands->Start_Timer(obj,this,1.0f,2);
	}
}
void JMG_Metroid_Mine_Computer_Console_Script_Special::Timeout(GameObject *obj)
{
	pokeAmount = 0.0f;
	subtractionGrowth = 0;
	Commands->Set_Animation(obj,"hackterminal.hackterminal",false,0,0.0f,0.0f,false);
}
void JMG_Metroid_Poke_Activate_Purchase_Terminal::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void JMG_Metroid_Poke_Activate_Purchase_Terminal::Poked(GameObject *obj,GameObject *poker)
{
	int PlayerID = JmgUtility::JMG_Get_Player_ID(poker);
	if (!PlayerID)
		return;
	if (!MetroidPSSControl.PlayerSettings[PlayerID]->InStartRoom)
	{
		char dialog[1000];
		sprintf(dialog,"pamsg %d Sorry, you can only access the PTs before first leaving the drop ship.",PlayerID);
		Console_Input(dialog);
		return;
	}
	Display_GDI_Player_Terminal_Player(poker);
}
void JMG_Metroid_Update_And_Display_Briefing_On_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (!Commands->Is_A_Star(enter))
		return;
	if (Get_Int_Parameter("BriefingId") <= JMG_Metroid_Game_Control::currentBriefingId)
		return;
	JMG_Metroid_Game_Control::currentBriefingId = Get_Int_Parameter("BriefingId");
	for (int x = 1;x < 128;x++)
	{
		if (!Get_GameObj(x))
			continue;
		JMG_Metroid_Game_Control::ParseBriefingString(Get_Translated_String(Get_Int_Parameter("BriefingId")),x);
	}
}
void JMG_Metroid_Objective_Update_On_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (!Commands->Is_A_Star(enter) || triggered)
		return;
	triggered = true;
	if (Get_Int_Parameter("CompleteObjectiveID"))
	{
		MetroidObjectiveSystemControl.Add_Objective(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
		MetroidObjectiveSystemControl.Set_Objective_Status(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Accomplished);
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
}
void JMG_Metroid_Objective_Update_On_Enter::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (Get_Int_Parameter("NewObjectiveID"))
		{
			GameObject *ObjectiveMarker = Commands->Find_Object(Get_Int_Parameter("ObjectiveMarkerObjectID"));
			if (!ObjectiveMarker && Get_Int_Parameter("ObjectiveMarkerObjectID"))
			{
				Console_Input("msg JMG_Metroid_Objective_Update_On_Enter ERROR: Could not find objective marker object!");
				return;
			}
			if (ObjectiveMarker)
				MetroidObjectiveSystemControl.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"),Commands->Get_Position(ObjectiveMarker));
			else
				MetroidObjectiveSystemControl.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"));
			JmgUtility::SetHUDHelpText(Get_Int_Parameter("NewObjectiveStringID"),Vector3(0,1,0));
		}
	}
}
void JMG_Metroid_Objective_Update_On_Pickup::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		if (!Commands->Is_A_Star(sender))
			return;
		if (Get_Int_Parameter("CompleteObjectiveID"))
		{
			MetroidObjectiveSystemControl.Add_Objective(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
			MetroidObjectiveSystemControl.Set_Objective_Status(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Accomplished);
		}
		GameObject *ObjectiveMarker = Commands->Find_Object(Get_Int_Parameter("ObjectiveMarkerObjectID"));
		if (!ObjectiveMarker && Get_Int_Parameter("ObjectiveMarkerObjectID"))
		{
			Console_Input("msg JMG_Metroid_Objective_Update_On_Enter ERROR: Could not find objective marker object!");
			return;
		}
		if (ObjectiveMarker)
			MetroidObjectiveSystemControl.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"),Commands->Get_Position(ObjectiveMarker));
		else
			MetroidObjectiveSystemControl.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"));
		JmgUtility::SetHUDHelpText(Get_Int_Parameter("NewObjectiveStringID"),Vector3(0,1,0));
	}
}
void JMG_Metroid_Objective_Update_On_Killed::Killed(GameObject *obj, GameObject *damager)
{
	if (The_Cnc_Game()->Get_Game_Duration_S() < 10)
		return;
	if (Get_Int_Parameter("CompleteObjectiveID"))
	{
		MetroidObjectiveSystemControl.Add_Objective(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
		MetroidObjectiveSystemControl.Set_Objective_Status(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Accomplished);
	}
	GameObject *ObjectiveMarker = Commands->Find_Object(Get_Int_Parameter("ObjectiveMarkerObjectID"));
	if (!ObjectiveMarker && Get_Int_Parameter("ObjectiveMarkerObjectID"))
	{
		Console_Input("msg JMG_Metroid_Objective_Update_On_Enter ERROR: Could not find objective marker object!");
		return;
	}
	if (ObjectiveMarker)
		MetroidObjectiveSystemControl.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"),Commands->Get_Position(ObjectiveMarker));
	else
		MetroidObjectiveSystemControl.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"));
	JmgUtility::SetHUDHelpText(Get_Int_Parameter("NewObjectiveStringID"),Vector3(0,1,0));
}
void JMG_Metroid_Objective_Update_On_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		if (Get_Int_Parameter("CompleteObjectiveID"))
		{
			MetroidObjectiveSystemControl.Add_Objective(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
			MetroidObjectiveSystemControl.Set_Objective_Status(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Accomplished);
		}
		GameObject *ObjectiveMarker = Commands->Find_Object(Get_Int_Parameter("ObjectiveMarkerObjectID"));
		if (!ObjectiveMarker && Get_Int_Parameter("ObjectiveMarkerObjectID"))
		{
			Console_Input("msg JMG_Metroid_Objective_Update_On_Enter ERROR: Could not find objective marker object!");
			return;
		}
		if (ObjectiveMarker)
			MetroidObjectiveSystemControl.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"),Commands->Get_Position(ObjectiveMarker));
		else
			MetroidObjectiveSystemControl.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"));
		JmgUtility::SetHUDHelpText(Get_Int_Parameter("NewObjectiveStringID"),Vector3(0,1,0));
	}
}
void JMG_AI_Artillery_Targeting_Fire_At_Custom::Created(GameObject *obj)
{
	if (obj->As_SoldierGameObj())
		obj->As_SoldierGameObj()->Set_Override_Muzzle_Direction(true);
	const char *weap = Get_Current_Weapon(obj);
	if (!weap)
		return;
	const AmmoDefinitionClass *primaryAmmo = Get_Weapon_Ammo_Definition(weap,1);
	if (!primaryAmmo)
		return;
	velocity = primaryAmmo->Velocity;
	gravity = primaryAmmo->Gravity;
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_AI_Artillery_Targeting_Fire_At_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (Get_Int_Parameter("TargetCustom") == message)
	{
		GameObject *seen = Commands->Find_Object(param);
		if (!seen)
			seen = sender;
		if (Get_Vehicle_Driver(seen))
			seen = Get_Vehicle_Driver(seen);
		Vector3 myPos = obj->As_SoldierGameObj() ? Commands->Get_Bone_Position(obj,"gunbone") : Commands->Get_Bone_Position(obj,"MuzzleA0"),enemyPos = Commands->Get_Position(seen);
		float targetDist = JmgUtility::SimpleDistance(myPos,enemyPos),targetFlatDistance = JmgUtility::SimpleFlatDistance(myPos,enemyPos),minDistanceSquared = Get_Float_Parameter("MinDistance")*Get_Float_Parameter("MinDistance");
		if (targetDist < minDistanceSquared)
			return;
		bool useHighAngle = true;
		if (targetDist <= Get_Float_Parameter("UseLowAngleMaxDistance")*Get_Float_Parameter("UseLowAngleMaxDistance"))
			useHighAngle = false;
		else if (Get_Int_Parameter("UseLowAngleWhenAboveMinDistance") && targetFlatDistance <= minDistanceSquared)
			useHighAngle = false;
		else if (Get_Float_Parameter("UseLowAngleTargetAboveHeight") && myPos.Z+Get_Float_Parameter("UseLowAngleTargetAboveHeight") < Commands->Get_Position(seen).Z)
			useHighAngle = false;
		double height = enemyPos.Z-myPos.Z,angle;
		myPos.Z = enemyPos.Z = 0.0f;
		if (!CalculateAngle(&angle,Commands->Get_Distance(enemyPos,myPos),height,useHighAngle))
			return;
		float TempRotation = atan2(enemyPos.Y-myPos.Y,enemyPos.X-myPos.X);
		Vector3 pos = obj->As_SoldierGameObj() ? Commands->Get_Bone_Position(obj,"gunbone") : Commands->Get_Bone_Position(obj,"MuzzleA0");
		pos.X += (float)cos(TempRotation)*10.0f;
		pos.Y += (float)sin(TempRotation)*10.0f;
		pos.Z += (float)tan(angle*PI/180)*10.0f;
		ActionParamsStruct params;
		params.Set_Attack(pos,100000.0f,0,1);
		params.AttackForceFire = false;
		params.AttackCheckBlocked = false;
		Commands->Action_Attack(obj,params);
	}
}
void JMG_AI_Artillery_Targeting_Fire_At_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!Get_Current_Bullets(obj))
			Commands->Action_Reset(obj,100);
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
bool JMG_AI_Artillery_Targeting_Fire_At_Custom::CalculateAngle(double *returnAngle,double distance,double height,bool highAngle)
{
	double velocitySquared = velocity*velocity;
	double calculatedGravity = gravity*9.8;
	if (distance <= 0.0)
		return false;
	double x = velocitySquared*velocitySquared-calculatedGravity*(calculatedGravity*(distance*distance)+2*height*velocitySquared);
	if (x < 0)
		return false;
	*returnAngle = atan((velocitySquared+(highAngle ? 1 : -1)*sqrt(x))/(calculatedGravity*distance))*180.0/PI;
	if (*returnAngle < Get_Float_Parameter("MinAngle") || *returnAngle > Get_Float_Parameter("MaxAngle"))
		return false;
	return true;
}
void JMG_Metroid_AI_Forest_Mini_Boss::Created(GameObject *obj)
{
	LastSeen = 0;
	currentTargetID = 0;
	huntorattack = 0;
	waitcount = 0;
	speed = 0.4f;
	primary = true;
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,7.5f,1);
	Commands->Start_Timer(obj,this,0.1f,92562343);
}
void JMG_Metroid_AI_Forest_Mini_Boss::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	if (!Commands->Get_Health(seen))
		return;
	int SeenID = Commands->Get_ID(seen);
	if (!currentTargetID || !LastSeen)
	{
		if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) <= 1.567504)
			return;
		LastSeen = 30;
		huntorattack = 0;
		currentTargetID = SeenID;
	}
	else if (currentTargetID == SeenID)
	{
		if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) <= 1.567504)
			LastSeen = 1;
		else
			LastSeen = 30;
	}
}
void JMG_Metroid_AI_Forest_Mini_Boss::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		if (!currentTargetID)
		{
			Commands->Action_Reset(obj,100);
			Commands->Action_Reset(obj,999);
			Hide(obj);
		}
	}
	if (92562343 == number)
	{
		if (LastSeen)
		{
			LastSeen--;
			if (!LastSeen)
			{
				currentTargetID = 0;
				Hide(obj);
			}
		}
		if (currentTargetID)
		{
			GameObject *target = Commands->Find_Object(currentTargetID);
			if (!target || Commands->Get_Health(target) == 0.0f)
				Hide(obj);
			else if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target)) > 40000.0f)
			{
				currentTargetID = 0;
				waitcount = 0;
				if (huntorattack == 1)
					Commands->Action_Reset(obj,10);
			}
			else
			{
				waitcount++;
				if (waitcount > 2)
				{
					Attack_Target(obj,target);
				}
			}
		}
		Commands->Start_Timer(obj,this,0.1f,92562343);
	}
}
void JMG_Metroid_AI_Forest_Mini_Boss::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damager || damager == obj || Commands->Get_Player_Type(damager) != 1)
	{
		if (damager > 0)
			Commands->Apply_Damage(obj,-damage,"None",0);
		return;
	}
	float health = Get_Hitpoints(obj)/Get_Max_Hitpoints(obj);
	if (health < 0.66 && speed != 1.0f)
	{
		speed = 1.0f;
		Attack_Target(obj,Commands->Find_Object(currentTargetID));
	}
	else if (health >= 0.66 && speed != 0.25f)
	{
		speed = 0.4f;
		Attack_Target(obj,Commands->Find_Object(currentTargetID));
	}
	if (health < 0.33 && primary)
	{
		primary = false;
		Commands->Select_Weapon(obj,"Weapon_Melee_AI_Deer_Flame");
		Attack_Target(obj,Commands->Find_Object(currentTargetID));
	}
	else if (health >= 0.33 && !primary)
	{
		primary = true;
		Commands->Select_Weapon(obj,"Weapon_Melee_AI_Deer");
		Attack_Target(obj,Commands->Find_Object(currentTargetID));
	}
	if (currentTargetID)
		return;
	GameObject *target = Commands->Find_Object(currentTargetID);
	if (target)
	{
		float targetDistance = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target));
		float damagerDistance = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(damager));
		if (targetDistance < damagerDistance)
			return;
	}
	LastSeen = 30;
	huntorattack = 0;
	waitcount = 0;
	currentTargetID = Commands->Get_ID(damager);
}
void JMG_Metroid_AI_Forest_Mini_Boss::Attack_Target(GameObject *obj,GameObject *target)
{
	if (!target)
		return;
	waitcount = 0;
	ActionParamsStruct params;
	params.Set_Movement(target,speed,primary ? 1.252f : 5.0f,false);
	params.MovePathfind = primary && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target)) <= 225 ? false : true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,999,10);
	params.Set_Attack(target,primary ? 3.0f : 40.0f,1,true);
	Commands->Action_Attack(obj,params);
}
void JMG_Metroid_AI_Forest_Mini_Boss::Hide(GameObject *obj)
{
	Commands->Action_Reset(obj,100);
	Vector3 pos = Commands->Get_Position(obj);
	AmbushPointSystem::CoverPointNode *Temp = AmbushPointSystemControl.GetGoodHidingSpot(&pos,8334,1);
	if (Temp)
	{
		waitcount = 0;
		currentTargetID = 0;
		Commands->Action_Reset(obj,10);
		ActionParamsStruct params;
		params.Set_Basic(this,999,10);
		params.MovePathfind = true;
		params.Set_Movement(Temp->coverPos,speed,1.0f,false);
		Commands->Action_Goto(obj,params);
	}
}
void JMG_Metroid_SpawnRoom_Objective_Update_On_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (!Commands->Is_A_Star(enter) || triggered)
		return;
	triggered = true;
	if (Get_Int_Parameter("CompleteObjectiveID"))
	{
		MetroidRoomObjectives.Add_Objective(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
		MetroidRoomObjectives.Set_Objective_Status(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Accomplished);
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
}
void JMG_Metroid_SpawnRoom_Objective_Update_On_Enter::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (Get_Int_Parameter("NewObjectiveID"))
		{
			GameObject *ObjectiveMarker = Commands->Find_Object(Get_Int_Parameter("ObjectiveMarkerObjectID"));
			if (!ObjectiveMarker && Get_Int_Parameter("ObjectiveMarkerObjectID"))
			{
				Console_Input("msg JMG_Metroid_Objective_Update_On_Enter ERROR: Could not find objective marker object!");
				return;
			}
			if (ObjectiveMarker)
				MetroidRoomObjectives.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,0,"",0,Commands->Get_Position(ObjectiveMarker));
			else
				MetroidRoomObjectives.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,0,"",0);
		}
	}
}
void JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_At_Custom::Created(GameObject *obj)
{
	reloadComplete = true;
	Commands->Disable_All_Collisions(obj);
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_At_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (Get_Int_Parameter("TargetCustom") == message && reloadComplete)
	{
		GameObject *seen = Commands->Find_Object(param);
		if (!seen)
			seen = sender;
		if (Get_Vehicle_Driver(seen))
			seen = Get_Vehicle_Driver(seen);
		Vector3 myPos = Commands->Get_Position(obj),enemyPos = Commands->Get_Position(seen);
		float MissAmount = Get_Float_Parameter("MissAmount");
		if (MissAmount)
		{
			myPos.X += Commands->Get_Random(-MissAmount,MissAmount);
			myPos.Y += Commands->Get_Random(-MissAmount,MissAmount);
			myPos.Z += Commands->Get_Random(-MissAmount,MissAmount);
		}
		float targetDist = JmgUtility::SimpleDistance(myPos,enemyPos),targetFlatDistance = JmgUtility::SimpleFlatDistance(myPos,enemyPos),minDistanceSquared = Get_Float_Parameter("MinDistance")*Get_Float_Parameter("MinDistance");
		if (targetDist < minDistanceSquared)
			return;
		bool useHighAngle = true;
		if (targetDist <= Get_Float_Parameter("UseLowAngleMaxDistance")*Get_Float_Parameter("UseLowAngleMaxDistance"))
			useHighAngle = false;
		else if (Get_Int_Parameter("UseLowAngleWhenAboveMinDistance") && targetFlatDistance <= minDistanceSquared)
			useHighAngle = false;
		else if (Get_Float_Parameter("UseLowAngleTargetAboveHeight") && Commands->Get_Position(obj).Z+Get_Float_Parameter("UseLowAngleTargetAboveHeight") < Commands->Get_Position(seen).Z)
			useHighAngle = false;
		double height = enemyPos.Z-myPos.Z,angle;
		myPos.Z = enemyPos.Z = 0.0f;
		if (!CalculateAngle(&angle,Commands->Get_Distance(enemyPos,myPos),height,useHighAngle))
			return;
		double zRotation = atan2(enemyPos.Y-myPos.Y,enemyPos.X-myPos.X);
		FireProjectile(obj,zRotation,angle);
		reloadComplete = false;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("ReloadTime"),1);
	}
}
void JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_At_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
		reloadComplete = true;
}
bool JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_At_Custom::CalculateAngle(double *returnAngle,double distance,double height,bool highAngle)
{
	double velocitySquared = Get_Float_Parameter("FireVelocity")*Get_Float_Parameter("FireVelocity");
	double calculatedGravity = Get_Float_Parameter("GravityScale")*9.8;
	if (distance <= 0.0)
		return false;
	double x = velocitySquared*velocitySquared-calculatedGravity*(calculatedGravity*(distance*distance)+2*height*velocitySquared);
	if (x < 0)
		return false;
	*returnAngle = atan((velocitySquared+(highAngle ? 1 : -1)*sqrt(x))/(calculatedGravity*distance));
	if (*returnAngle*180.0/PI < Get_Float_Parameter("MinAngle") || *returnAngle*180.0/PI > Get_Float_Parameter("MaxAngle"))
		return false;
	return true;
}
void JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_At_Custom::FireProjectile(GameObject *obj,double zAngle,double aimAngle)
{
	Commands->Create_3D_Sound_At_Bone(Get_Parameter("FireSound"),obj,"MuzzleA0");
	GameObject *projectile = Commands->Create_Object(Get_Parameter("VehicleProjectilePreset"),Commands->Get_Bone_Position(obj,"muzzleA0"));
	Commands->Create_Explosion(Get_Parameter("MuzzleFlashExplosion"),Commands->Get_Bone_Position(obj,"muzzleA0"),obj);
	char params[220];
	sprintf(params,"%d,%s",Commands->Get_ID(obj),Get_Parameter("ProjectileExplosion"));
	Commands->Attach_Script(projectile,"JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_Attach",params);
	Commands->Attach_Script(projectile,"JMG_Utility_Sync_Object_Periodically","0.1");
	Vector3 normalizedVector = Vector3((float)cos(zAngle),(float)sin(zAngle),(float)tan(aimAngle));
	normalizedVector.Normalize();
	normalizedVector.X *= Get_Float_Parameter("FireVelocity");
	normalizedVector.Y *= Get_Float_Parameter("FireVelocity");
	normalizedVector.Z *= Get_Float_Parameter("FireVelocity");
	Set_Velocity(projectile,normalizedVector);
}
void JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_Attach::Created(GameObject *obj)
{
	Force_Position_Update(obj);
	Force_Orientation_Update(obj);
	Force_Velocity_Update(obj);
	Update_Network_Object(obj);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_Attach::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Force_Position_Update(obj);
		Force_Orientation_Update(obj);
		Force_Velocity_Update(obj);
		Update_Network_Object(obj);
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_Attach::Destroyed(GameObject *obj)
{
	GameObject *launcher = Commands->Find_Object(Get_Int_Parameter("LauncherId"));
	if (launcher)
		Commands->Create_Explosion(Get_Parameter("ExplosionPreset"),Commands->Get_Position(obj),launcher);
	else
		Commands->Create_Explosion(Get_Parameter("ExplosionPreset"),Commands->Get_Position(obj),obj);
}
void JMG_Metroid_Miniboss_Turret::Created(GameObject *obj)
{
	Commands->Enable_Spawner(603561,true);
	Commands->Enable_Spawner(603567,true);
}
void JMG_Metroid_Miniboss_Turret::Destroyed(GameObject *obj)
{
	Commands->Enable_Spawner(603561,false);
	Commands->Enable_Spawner(603567,false);
	for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
	{
		SmartGameObj* o = current->Data();
		if (o && Commands->Get_Preset_ID(o) == 82080178)
			Commands->Apply_Damage(o,1000.0f,"None",o);
	}
}
void JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_Custom_Player::Created(GameObject *obj)
{
	reloadComplete = true;
	Commands->Disable_All_Collisions(obj);
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_Custom_Player::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (Get_Int_Parameter("TargetCustom") == message && reloadComplete)
	{
		GameObject *seen = Commands->Find_Object(param);
		if (!seen)
			seen = sender;
		if (Get_Vehicle_Driver(seen))
			seen = Get_Vehicle_Driver(seen);
		Vector3 myPos = Commands->Get_Position(obj),enemyPos = Commands->Get_Position(seen);
		float MissAmount = Get_Float_Parameter("MissAmount");
		if (MissAmount)
		{
			myPos.X += Commands->Get_Random(-MissAmount,MissAmount);
			myPos.Y += Commands->Get_Random(-MissAmount,MissAmount);
			myPos.Z += Commands->Get_Random(-MissAmount,MissAmount);
		}
		float targetDist = JmgUtility::SimpleDistance(myPos,enemyPos),targetFlatDistance = JmgUtility::SimpleFlatDistance(myPos,enemyPos),minDistanceSquared = Get_Float_Parameter("MinDistance")*Get_Float_Parameter("MinDistance");
		if (targetDist < minDistanceSquared)
			return;
		bool useHighAngle = true;
		if (targetDist <= Get_Float_Parameter("UseLowAngleMaxDistance")*Get_Float_Parameter("UseLowAngleMaxDistance"))
			useHighAngle = false;
		else if (Get_Int_Parameter("UseLowAngleWhenAboveMinDistance") && targetFlatDistance <= minDistanceSquared)
			useHighAngle = false;
		else if (Get_Float_Parameter("UseLowAngleTargetAboveHeight") && Commands->Get_Position(obj).Z+Get_Float_Parameter("UseLowAngleTargetAboveHeight") < Commands->Get_Position(seen).Z)
			useHighAngle = false;
		double height = enemyPos.Z-myPos.Z,angle;
		myPos.Z = enemyPos.Z = 0.0f;
		if (!CalculateAngle(&angle,Commands->Get_Distance(enemyPos,myPos),height,useHighAngle))
			return;
		double zRotation = atan2(enemyPos.Y-myPos.Y,enemyPos.X-myPos.X);
		FireProjectile(obj,zRotation,angle);
		reloadComplete = false;
		Commands->Start_Timer(obj,this,max(Get_Float_Parameter("ReloadTime")-(Get_Float_Parameter("SubtractReloadPerPlayer")*Get_Player_Count()),Get_Float_Parameter("MinReloadTime")),1);
	}
}
void JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_Custom_Player::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
		reloadComplete = true;
}
bool JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_Custom_Player::CalculateAngle(double *returnAngle,double distance,double height,bool highAngle)
{
	double velocitySquared = Get_Float_Parameter("FireVelocity")*Get_Float_Parameter("FireVelocity");
	double calculatedGravity = Get_Float_Parameter("GravityScale")*9.8;
	if (distance <= 0.0)
		return false;
	double x = velocitySquared*velocitySquared-calculatedGravity*(calculatedGravity*(distance*distance)+2*height*velocitySquared);
	if (x < 0)
		return false;
	*returnAngle = atan((velocitySquared+(highAngle ? 1 : -1)*sqrt(x))/(calculatedGravity*distance));
	if (*returnAngle*180.0/PI < Get_Float_Parameter("MinAngle") || *returnAngle*180.0/PI > Get_Float_Parameter("MaxAngle"))
		return false;
	return true;
}
void JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_Custom_Player::FireProjectile(GameObject *obj,double zAngle,double aimAngle)
{
	Commands->Create_3D_Sound_At_Bone(Get_Parameter("FireSound"),obj,"MuzzleA0");
	GameObject *projectile = Commands->Create_Object(Get_Parameter("VehicleProjectilePreset"),Commands->Get_Bone_Position(obj,"muzzleA0"));
	Commands->Create_Explosion(Get_Parameter("MuzzleFlashExplosion"),Commands->Get_Bone_Position(obj,"muzzleA0"),obj);
	char params[220];
	sprintf(params,"%d,%s",Commands->Get_ID(obj),Get_Parameter("ProjectileExplosion"));
	Commands->Attach_Script(projectile,"JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_Attach",params);
	Commands->Attach_Script(projectile,"JMG_Utility_Sync_Object_Periodically","0.1");
	Vector3 normalizedVector = Vector3((float)cos(zAngle),(float)sin(zAngle),(float)tan(aimAngle));
	normalizedVector.Normalize();
	normalizedVector.X *= Get_Float_Parameter("FireVelocity");
	normalizedVector.Y *= Get_Float_Parameter("FireVelocity");
	normalizedVector.Z *= Get_Float_Parameter("FireVelocity");
	Set_Velocity(projectile,normalizedVector);
}
ScriptRegistrant<JMG_Metroid_Hide_On_Enter> JMG_Metroid_Hide_On_Enter_Registrant("JMG_Metroid_Hide_On_Enter","");
ScriptRegistrant<JMG_Metroid_Gibs_On_Enter> JMG_Metroid_Gibs_On_Enter_Registrant("JMG_Metroid_Gibs_On_Enter","DeathMessage=falling into a spiky pit:string,PerkID=0:int");
ScriptRegistrant<JMG_Metroid_Increase_Perk_Unlock_On_Enter> JMG_Metroid_Increase_Perk_Unlock_On_Enter_Registrant("JMG_Metroid_Increase_Perk_Unlock_On_Enter","PerkID:int");
ScriptRegistrant<JMG_Metroid_HUD_Message_On_Enter> JMG_Metroid_HUD_Message_On_Enter_Registrant("JMG_Metroid_HUD_Message_On_Enter","StringID:int,Delay=0.0:float");
ScriptRegistrant<JMG_Metroid_Game_Initilization> JMG_Metroid_Game_Initilization_Registrant("JMG_Metroid_Game_Initilization","");
ScriptRegistrant<JMG_Metroid_Game_Control> JMG_Metroid_Game_Control_Registrant("JMG_Metroid_Game_Control","");
ScriptRegistrant<JMG_Metroid_Character_Control> JMG_Metroid_Character_Control_Registrant("JMG_Metroid_Character_Control","");
ScriptRegistrant<JMG_Metroid_Hide_Object_On_Death> JMG_Metroid_Hide_Object_On_Death_Registrant("JMG_Metroid_Hide_Object_On_Death","");
ScriptRegistrant<JMG_Metroid_New_Ambush_Cover_Point> JMG_Metroid_New_Ambush_Cover_Point_Registrant("JMG_Metroid_New_Ambush_Cover_Point","ID:int,GroupID=0:int,Crouch=0:int,ExcellentHidingSpot=0:int");
ScriptRegistrant<JMG_Metroid_New_Ambush_Attack_Point> JMG_Metroid_New_Ambush_Attack_Point_Registrant("JMG_Metroid_New_Ambush_Attack_Point","ID:int");
ScriptRegistrant<JMG_Metroid_Move_To_Random_Ambush_Spot> JMG_Metroid_Move_To_Random_Ambush_Spot_Registrant("JMG_Metroid_Move_To_Random_Ambush_Spot","GroupID:int,MaxGuards=0:int,Delay=30.0:float,RandomDelay=10.0:float,MaxDistance=0.0:float,BeginOnSpawn=0:int,FindVantagePoint=0:int,MaxTargetDistance=0.0:float,FindHidingSpots=0:int,SpeedMultiplier=1.0:float");
ScriptRegistrant<JMG_Metroid_Mine_Tower_Control> JMG_Metroid_Mine_Tower_Control_Registrant("JMG_Metroid_Mine_Tower_Control","");
ScriptRegistrant<JMG_Metroid_Send_Custom_On_Poke> JMG_Metroid_Send_Custom_On_Poke_Registrant("JMG_Metroid_Send_Custom_On_Poke","ID:int,Message:int,Param:int");
ScriptRegistrant<JMG_Metroid_AI_Path_Node> JMG_Metroid_AI_Path_Node_Registrant("JMG_Metroid_AI_Path_Node","NodeName:string,LinkedNodes:string,Delim=@:string,MaxDeviation=15.0:float");
ScriptRegistrant<JMG_Metroid_Mine_Boss> JMG_Metroid_Mine_Boss_Registrant("JMG_Metroid_Mine_Boss","");
ScriptRegistrant<JMG_Metroid_Boss_Turret> JMG_Metroid_Boss_Turret_Registrant("JMG_Metroid_Boss_Turret","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=11:int,CanTarget[Infantry|Vehicles|Aircraft]=1.0 1.0 1.0:Vector3,SwivelTurret=1:int");
ScriptRegistrant<JMG_Metroid_Boss_Grinder_Kill_Zone> JMG_Metroid_Boss_Grinder_Kill_Zone_Registrant("JMG_Metroid_Boss_Grinder_Kill_Zone","");
ScriptRegistrant<JMG_Metroid_Dynamic_Powerup_System> JMG_Metroid_Dynamic_Powerup_System_Registrant("JMG_Metroid_Dynamic_Powerup_System","");
ScriptRegistrant<JMG_Metroid_Crate_Random_Powerup> JMG_Metroid_Crate_Random_Powerup_Registrant("JMG_Metroid_Crate_Random_Powerup","");
ScriptRegistrant<JMG_Metroid_Mine_Tower_Elevator> JMG_Metroid_Mine_Tower_Elevator_Registrant("JMG_Metroid_Mine_Tower_Elevator","");
ScriptRegistrant<JMG_Metroid_Text_Dialog> JMG_Metroid_Text_Dialog_Registrant("JMG_Metroid_Text_Dialog","StringID:int");
ScriptRegistrant<JMG_Metroid_Mine_Computer_Console> JMG_Metroid_Mine_Computer_Console_Registrant("JMG_Metroid_Mine_Computer_Console","");
ScriptRegistrant<JMG_Metroid_Mine_Computer_Console_Disarm_Script> JMG_Metroid_Mine_Computer_Console_Disarm_Script_Registrant("JMG_Metroid_Mine_Computer_Console_Disarm_Script","ID:int");
ScriptRegistrant<JMG_Metroid_Reset_Fall_Distance_On_Enter> JMG_Metroid_Reset_Fall_Distance_On_Enter_Registrant("JMG_Metroid_Reset_Fall_Distance_On_Enter","");
ScriptRegistrant<JMG_Metroid_Recover_Fall_Damage_Once> JMG_Metroid_Recover_Fall_Damage_Once_Registrant("JMG_Metroid_Recover_Fall_Damage_Once","");
ScriptRegistrant<JMG_Metroid_Teleport_Zone> JMG_Metroid_Teleport_Zone_Registrant("JMG_Metroid_Teleport_Zone","ObjectID:int,Position=0.0 0.0 0.0:vector3,MaxDistance=1.5:float");
ScriptRegistrant<JMG_Metroid_Tower_Put_AI_Down_Zone> JMG_Metroid_Tower_Put_AI_Down_Zone_Registrant("JMG_Metroid_Tower_Put_AI_Down_Zone","");
ScriptRegistrant<JMG_Metroid_Electrified_Water> JMG_Metroid_Electrified_Water_Registrant("JMG_Metroid_Electrified_Water","");
ScriptRegistrant<JMG_Metroid_Set_Screen_Color_While_In_Zone> JMG_Metroid_Set_Screen_Color_While_In_Zone_Registrant("JMG_Metroid_Set_Screen_Color_While_In_Zone","ScreenColor=0.28 0.43 0.55:Vector3,Opacity=0.9:float");
ScriptRegistrant<JMG_Metroid_Medic> JMG_Metroid_Medic_Registrant("JMG_Metroid_Medic","");
ScriptRegistrant<JMG_Metroid_Support_Health_Powerup_Beacon> JMG_Metroid_Support_Health_Powerup_Beacon_Registrant("JMG_Metroid_Support_Health_Powerup_Beacon","");
ScriptRegistrant<JMG_Metroid_Support_Health_Powerup> JMG_Metroid_Support_Health_Powerup_Registrant("JMG_Metroid_Support_Health_Powerup","PlayerID:int");
ScriptRegistrant<JMG_Metroid_Support_Generator_Powerup_Beacon> JMG_Metroid_Support_Generator_Powerup_Beacon_Registrant("JMG_Metroid_Support_Generator_Powerup_Beacon","");
ScriptRegistrant<JMG_Metroid_Support_Generator_Powerup> JMG_Metroid_Support_Generator_Powerup_Registrant("JMG_Metroid_Support_Generator_Powerup","BeaconID:int");
ScriptRegistrant<JMG_Metroid_AI_Hunt_Attack> JMG_Metroid_AI_Hunt_Attack_Registrant("JMG_Metroid_AI_Hunt_Attack","MaxStarSeekDistance=100.0:float,ScanFromSpawnPosition=0:int,ReturnToOriginalPosition=0:int,AllowInnate=1:int");
ScriptRegistrant<JMG_Metroid_Powered_Teleport_Zone> JMG_Metroid_Powered_Teleport_Zone_Registrant("JMG_Metroid_Powered_Teleport_Zone","ObjectID:int,StartsEnabled=0:int,PowerMessage=0:int,VisibleTeleporterID=0:int");
ScriptRegistrant<JMG_Metroid_Send_Custom_On_Zone_Enter> JMG_Metroid_Send_Custom_On_Zone_Enter_Registrant("JMG_Metroid_Send_Custom_On_Zone_Enter","ID:int,Message:int,Param:int");
ScriptRegistrant<JMG_Metroid_Sabotaged_Crate> JMG_Metroid_Sabotaged_Crate_Registrant("JMG_Metroid_Sabotaged_Crate","");
ScriptRegistrant<JMG_Metroid_Enable_Spawners_On_Death> JMG_Metroid_Enable_Spawners_On_Death_Registrant("JMG_Metroid_Enable_Spawners_On_Death","StartID:int,EndID:int");
ScriptRegistrant<JMG_Metroid_Lockdown_Lockdown_Object> JMG_Metroid_Lockdown_Lockdown_Object_Registrant("JMG_Metroid_Lockdown_Lockdown_Object","LockDownZoneID=0:int");
ScriptRegistrant<JMG_Metroid_Lockdown_Post_Lockdown_Object> JMG_Metroid_Lockdown_Post_Lockdown_Object_Registrant("JMG_Metroid_Lockdown_Post_Lockdown_Object","LockDownZoneID=0:int");
ScriptRegistrant<JMG_Metroid_Lockdown_Pre_Lockdown_Object> JMG_Metroid_Lockdown_Pre_Lockdown_Object_Registrant("JMG_Metroid_Lockdown_Pre_Lockdown_Object","LockDownZoneID=0:int");
ScriptRegistrant<JMG_Metroid_Lockdown_Lockdown_Send_Custom_Object> JMG_Metroid_Lockdown_Lockdown_Send_Custom_Object_Registrant("JMG_Metroid_Lockdown_Lockdown_Send_Custom_Object","LockDownZoneID=0:int,PreLockdownStartCustom=0:int,LockdownStartCustom=0:int,PostLockdownStartCustom=0:int");
ScriptRegistrant<JMG_Metroid_Lockdown_Unit_Spawner> JMG_Metroid_Lockdown_Unit_Spawner_Registrant("JMG_Metroid_Lockdown_Unit_Spawner","LockDownZoneID=0:int,PresetName=null:string,MaxSpawnedAtOnce=5:int,MaxTotalSpawned=0:int,SpawnFequency=1:int,RespawnDelay=60:int,RandomRespawnDelay=10:int");
ScriptRegistrant<JMG_Metroid_Lockdown_Controller> JMG_Metroid_Lockdown_Controller_Registrant("JMG_Metroid_Lockdown_Controller","LockDownZoneID=0:int,HUDMsgLockdownStart=19:int,HUDMsgLockdownEnd=18:int,LockDownStartMsg=24:int,LockDownEndMsg=25:int,LockDownStartWavSound=LockDownInitiated.wav:string,LockDownEndWavSound=LockDownAverted.wav:string");
ScriptRegistrant<JMG_Metroid_Lockdown_Light> JMG_Metroid_Lockdown_Light_Registrant("JMG_Metroid_Lockdown_Light","LockDownZoneID=0:int");
ScriptRegistrant<JMG_Metroid_Lockdown_zSpawned_Unit> JMG_Metroid_Lockdown_zSpawned_Unit_Registrant("JMG_Metroid_Lockdown_zSpawned_Unit","LockDownZoneID=0:int");
ScriptRegistrant<JMG_Metroid_Lockdown_Trigger_Zone> JMG_Metroid_Lockdown_Trigger_Zone_Registrant("JMG_Metroid_Lockdown_Trigger_Zone","LockDownZoneID=0:int,EnemyArriveDelay=5:int");
ScriptRegistrant<JMG_Metroid_Lockdown_Disable_Lockdown_Terminal> JMG_Metroid_Lockdown_Disable_Lockdown_Terminal_Registrant("JMG_Metroid_Lockdown_Disable_Lockdown_Terminal","LockDownZoneID=0:int");
ScriptRegistrant<JMG_Metroid_Lockdown_zComputer_Console> JMG_Metroid_Lockdown_zComputer_Console_Registrant("JMG_Metroid_Lockdown_zComputer_Console","LockDownZoneID=0:int");
ScriptRegistrant<JMG_Metroid_Lockdown_zComputer_Console_Disarm_Script> JMG_Metroid_Lockdown_zComputer_Console_Disarm_Script_Registrant("JMG_Metroid_Lockdown_zComputer_Console_Disarm_Script","ID:int,LockDownZoneID:int");
ScriptRegistrant<JMG_Metroid_Lockdown_Disable_Lockdown_On_Custom_Count> JMG_Metroid_Lockdown_Disable_Lockdown_On_Custom_Count_Registrant("JMG_Metroid_Lockdown_Disable_Lockdown_On_Custom_Count","LockDownZoneID:int,Message:int,Count:int,ResetSpawnersOnComplete=1:int,LockoutLockdownTime=300:int");
ScriptRegistrant<JMG_Metroid_Lockdown_Disable_Lockdown_On_Death> JMG_Metroid_Lockdown_Disable_Lockdown_On_Death_Registrant("JMG_Metroid_Lockdown_Disable_Lockdown_On_Death","LockDownZoneID:int,ResetSpawnersOnComplete=1:int,LockoutLockdownTime=300:int");
ScriptRegistrant<JMG_Metroid_Lockdown_Trigger_On_Message> JMG_Metroid_Lockdown_Trigger_On_Message_Registrant("JMG_Metroid_Lockdown_Trigger_On_Message","LockDownZoneID=0:int,Message=0:int,EnemyArrivalDelay=0:int");
ScriptRegistrant<JMG_Metroid_Lockdown_Disable_Lockdown_On_Time_Without_Message> JMG_Metroid_Lockdown_Disable_Lockdown_On_Time_Without_Message_Registrant("JMG_Metroid_Lockdown_Disable_Lockdown_On_Time_Without_Message","LockDownZoneID=0:int,ArmMessage=0:int,DisarmTime=1:int,ResetSpawnersOnComplete=1:int,LockoutLockdownTime=300:int");
ScriptRegistrant<JMG_Metroid_Complete_Chapter> JMG_Metroid_Complete_Chapter_Registrant("JMG_Metroid_Complete_Chapter","ChapterID:int");
ScriptRegistrant<JMG_Metroid_Defense_Mode> JMG_Metroid_Defense_Mode_Registrant("JMG_Metroid_Defense_Mode","");
ScriptRegistrant<JMG_Metroid_Defense_Gun_Beacon> JMG_Metroid_Defense_Gun_Beacon_Registrant("JMG_Metroid_Defense_Gun_Beacon","");
ScriptRegistrant<JMG_Metroid_Defense_Gun_Powerup> JMG_Metroid_Defense_Gun_Powerup_Registrant("JMG_Metroid_Defense_Gun_Powerup","BeaconID:int,PlayerID:int");
ScriptRegistrant<JMG_Metroid_Defense_Telepad_Beacon> JMG_Metroid_Defense_Telepad_Beacon_Registrant("JMG_Metroid_Defense_Telepad_Beacon","");
ScriptRegistrant<JMG_Metroid_Defense_Telepad_Powerup> JMG_Metroid_Defense_Telepad_Powerup_Registrant("JMG_Metroid_Defense_Telepad_Powerup","BeaconID:int,PlayerID:int");
ScriptRegistrant<JMG_Metroid_Defense_Telepad_Zone> JMG_Metroid_Defense_Telepad_Zone_Registrant("JMG_Metroid_Defense_Telepad_Zone","PlayerID:int,PadID:int");
ScriptRegistrant<JMG_Metroid_Preplaced_Player_Telepad> JMG_Metroid_Preplaced_Player_Telepad_Registrant("JMG_Metroid_Preplaced_Player_Telepad","");
ScriptRegistrant<JMG_Metroid_Player_Turret_Damage> JMG_Metroid_Player_Turret_Damage_Registrant("JMG_Metroid_Player_Turret_Damage","");
ScriptRegistrant<JMG_Metroid_Screen_Fade_Red_On_Damage> JMG_Metroid_Screen_Fade_Red_On_Damage_Registrant("JMG_Metroid_Screen_Fade_Red_On_Damage","");
ScriptRegistrant<JMG_Metroid_Kill_On_Damage_Amounts> JMG_Metroid_Kill_On_Damage_Amounts_Registrant("JMG_Metroid_Kill_On_Damage_Amounts","Values:string,Delim=@:string");
ScriptRegistrant<JMG_Metroid_Grant_Keycard> JMG_Metroid_Grant_Keycard_Registrant("JMG_Metroid_Grant_Keycard","Key=0:int,Grant=1:int");
ScriptRegistrant<JMG_Metroid_AI_Move_To_Position> JMG_Metroid_AI_Move_To_Position_Registrant("JMG_Metroid_AI_Move_To_Position","TargetSpot:vector3,Speed=1.0:float,ArriveDistance=1.0:float");
ScriptRegistrant<JMG_Metroid_Reset_Screen_Color_On_Enter> JMG_Metroid_Reset_Screen_Color_On_Enter_Registrant("JMG_Metroid_Reset_Screen_Color_On_Enter","");
ScriptRegistrant<JMG_Metroid_AI_Hunt_Equipment> JMG_Metroid_AI_Hunt_Equipment_Registrant("JMG_Metroid_AI_Hunt_Equipment","MaxEquipmentSeekDistance=250.0:float,ScanFromSpawnPosition=0:int,ReturnToOriginalPosition=0:int,AllowInnate=1:int");
ScriptRegistrant<JMG_Metroid_Base_Defense> JMG_Metroid_Base_Defense_Registrant("JMG_Metroid_Base_Defense","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:float");
ScriptRegistrant<JMG_Metroid_Camera_Behavior> JMG_Metroid_Camera_Behavior_Registrant("JMG_Metroid_Camera_Behavior","Angle:float,Alarm_ID=0:int,Is_Gun=1:int,Delay=0:float,Alarm_Message=0:int,Alarm_Sound=Beep:string,UpdateRateMultiplier=1.0:float");
ScriptRegistrant<JMG_Metroid_Zone_Create_Explosion> JMG_Metroid_Zone_Create_Explosion_Registrant("JMG_Metroid_Zone_Create_Explosion","Explosion=null:string");
ScriptRegistrant<JMG_Metroid_Create_Object_Once_On_Enter> JMG_Metroid_Create_Object_Once_On_Enter_Registrant("JMG_Metroid_Create_Object_Once_On_Enter","PresetName=null:string,Position=0.0 0.0 0.0:vector3,Facing=0.0:float");
ScriptRegistrant<JMG_Metroid_Remove_Object_On_Enter> JMG_Metroid_Remove_Object_On_Enter_Registrant("JMG_Metroid_Remove_Object_On_Enter","ID:int");
ScriptRegistrant<JMG_Metroid_AI_Snow_Mini_Boss> JMG_Metroid_AI_Snow_Mini_Boss_Registrant("JMG_Metroid_AI_Snow_Mini_Boss","");
ScriptRegistrant<JMG_Metroid_Banshee_Strike_Craft> JMG_Metroid_Banshee_Strike_Craft_Registrant("JMG_Metroid_Banshee_Strike_Craft","");
ScriptRegistrant<JMG_Metroid_Cheat_Mode_Death_Counter> JMG_Metroid_Cheat_Mode_Death_Counter_Registrant("JMG_Metroid_Cheat_Mode_Death_Counter","");
ScriptRegistrant<JMG_Metroid_ZeroBug_Tracker> JMG_Metroid_ZeroBug_Tracker_Registrant("JMG_Metroid_ZeroBug_Tracker","");
ScriptRegistrant<JMG_Metroid_Zone_Damage_Object_While_In_Zone> JMG_Metroid_Zone_Damage_Object_While_In_Zone_Registrant("JMG_Metroid_Zone_Damage_Object_While_In_Zone","DamageRate:float,Warhead:string,Delay=0.1:float,Death_Zone_Type=0:int");
ScriptRegistrant<JMG_Metroid_Attached_Zone_Damage_Object_While_In_Zone> JMG_Metroid_Attached_Zone_Damage_Object_While_In_Zone_Registrant("JMG_Metroid_Attached_Zone_Damage_Object_While_In_Zone","DamageRate:float,Warhead:string,Delay:float,Death_Zone_Type:int");
ScriptRegistrant<JMG_Metroid_Random_Spawn_Point_Node> JMG_Metroid_Random_Spawn_Point_Node_Registrant("JMG_Metroid_Random_Spawn_Point_Node","SpawnGroup:int");
ScriptRegistrant<JMG_Metroid_Spawn_At_Random_Spawn_Point> JMG_Metroid_Spawn_At_Random_Spawn_Point_Registrant("JMG_Metroid_Spawn_At_Random_Spawn_Point","SpawnGroup:int");
ScriptRegistrant<JMG_Metroid_Granted_Shield_Powerup> JMG_Metroid_Granted_Shield_Powerup_Registrant("JMG_Metroid_Granted_Shield_Powerup","GranterPlayerID:int");
ScriptRegistrant<JMG_Metroid_Supply_Mode> JMG_Metroid_Supply_Mode_Registrant("JMG_Metroid_Supply_Mode","");
ScriptRegistrant<JMG_Metroid_Supply_Shield_Powerup_Beacon> JMG_Metroid_Supply_Shield_Powerup_Beacon_Registrant("JMG_Metroid_Supply_Shield_Powerup_Beacon","");
ScriptRegistrant<JMG_Metroid_Supply_Shield_Powerup> JMG_Metroid_Supply_Shield_Powerup_Registrant("JMG_Metroid_Supply_Shield_Powerup","PlayerID:int");
ScriptRegistrant<JMG_Metroid_Supply_Weapon_Spawner_Beacon> JMG_Metroid_Supply_Weapon_Spawner_Beacon_Registrant("JMG_Metroid_Supply_Weapon_Spawner_Beacon","");
ScriptRegistrant<JMG_Metroid_Supply_Weapon_Spawner_Decoy> JMG_Metroid_Supply_Weapon_Spawner_Decoy_Registrant("JMG_Metroid_Supply_Weapon_Spawner_Decoy","PlayerID:int,PowerupPreset:string");
ScriptRegistrant<JMG_Metroid_Supply_Weapon_Spawner_Weapon> JMG_Metroid_Supply_Weapon_Spawner_Weapon_Registrant("JMG_Metroid_Supply_Weapon_Spawner_Weapon","PlayerID:int,PowerupPreset:string");
ScriptRegistrant<JMG_Metroid_Display_Message_On_Pickup> JMG_Metroid_Display_Message_On_Pickup_Registrant("JMG_Metroid_Display_Message_On_Pickup","PickupMessage=null:string,Color[R|G|B]=255 255 255:vector3");
ScriptRegistrant<JMG_Metroid_Tutorial_Teleport> JMG_Metroid_Tutorial_Teleport_Registrant("JMG_Metroid_Tutorial_Teleport","");
ScriptRegistrant<JMG_Metroid_Shield_Regen> JMG_Metroid_Shield_Regen_Registrant("JMG_Metroid_Shield_Regen","Points:float,Delay:float");
ScriptRegistrant<JMG_Metroid_Transform_Into_Cinematic_Object> JMG_Metroid_Transform_Into_Cinematic_Object_Registrant("JMG_Metroid_Transform_Into_Cinematic_Object","ControlFileName:String");
ScriptRegistrant<JMG_Metroid_Grant_Phone_If_Pkg_Is_Rp2> JMG_Metroid_Grant_Phone_If_Pkg_Is_Rp2_Registrant("JMG_Metroid_Grant_Phone_If_Pkg_Is_Rp2","");
ScriptRegistrant<JMG_Metroid_Boss_HP_Modifier> JMG_Metroid_Boss_HP_Modifier_Registrant("JMG_Metroid_Boss_HP_Modifier","Points:float");
ScriptRegistrant<JMG_Metroid_Set_Is_Rendered_On_Custom> JMG_Metroid_Set_Is_Rendered_On_Custom_Registrant("JMG_Metroid_Set_Is_Rendered_On_Custom","ShowMessage=-1:int,HideMessage=-1:int,RenderedOnCreate=1:int");
ScriptRegistrant<JMG_Metroid_AI_Shield_Regen> JMG_Metroid_AI_Shield_Regen_Registrant("JMG_Metroid_AI_Shield_Regen","");
ScriptRegistrant<JMG_Metroid_Invincible_Object> JMG_Metroid_Invincible_Object_Registrant("JMG_Metroid_Invincible_Object","");
ScriptRegistrant<JMG_Metroid_Damage_Score_Monitor> JMG_Metroid_Damage_Score_Monitor_Registrant("JMG_Metroid_Damage_Score_Monitor","");
ScriptRegistrant<JMG_Metroid_Death_Zone_Score_Monitor> JMG_Metroid_Death_Zone_Score_Monitor_Registrant("JMG_Metroid_Death_Zone_Score_Monitor","Death_Zone_Type:int");
ScriptRegistrant<JMG_Metroid_Weapon_Pickup_Score_Monitor> JMG_Metroid_Weapon_Pickup_Score_Monitor_Registrant("JMG_Metroid_Weapon_Pickup_Score_Monitor","PowerUp_Type:int");
ScriptRegistrant<JMG_Metroid_Count_Jammed_Door_Destruction> JMG_Metroid_Count_Jammed_Door_Destruction_Registrant("JMG_Metroid_Count_Jammed_Door_Destruction","");
ScriptRegistrant<JMG_Metroid_Zone_Score_Monitor> JMG_Metroid_Zone_Score_Monitor_Registrant("JMG_Metroid_Zone_Score_Monitor","Zone_Type:int");
ScriptRegistrant<JMG_Metroid_Lockdown_zComputer_Console_Special_Disarm_Script> JMG_Metroid_Lockdown_zComputer_Console_Special_Disarm_Script_Registrant("JMG_Metroid_Lockdown_zComputer_Console_Special_Disarm_Script","LockDownZoneID:int");
ScriptRegistrant<JMG_Metroid_Mine_Computer_Console_Script_Special> JMG_Metroid_Mine_Computer_Console_Script_Special_Registrant("JMG_Metroid_Mine_Computer_Console_Script_Special","");
ScriptRegistrant<JMG_Metroid_Poke_Activate_Purchase_Terminal> JMG_Metroid_Poke_Activate_Purchase_Terminal_Registrant("JMG_Metroid_Poke_Activate_Purchase_Terminal","");
ScriptRegistrant<JMG_Metroid_Update_And_Display_Briefing_On_Enter> JMG_Metroid_Update_And_Display_Briefing_On_Enter_Registrant("JMG_Metroid_Update_And_Display_Briefing_On_Enter","BriefingId:int");
ScriptRegistrant<JMG_Metroid_Objective_Update_On_Enter> JMG_Metroid_Objective_Update_On_Enter_Registrant("JMG_Metroid_Objective_Update_On_Enter","NewObjectiveID:int,NewObjectiveStringID:int,ObjectiveMarkerObjectID:int,CompleteObjectiveID:int,Delay:float,NewObjectivePriority=1:int");
ScriptRegistrant<JMG_Metroid_Objective_Update_On_Pickup> JMG_Metroid_Objective_Update_On_Pickup_Registrant("JMG_Metroid_Objective_Update_On_Pickup","NewObjectiveID:int,NewObjectiveStringID:int,ObjectiveMarkerObjectID:int,CompleteObjectiveID:int,Delay:float,NewObjectivePriority=1:int");
ScriptRegistrant<JMG_Metroid_Objective_Update_On_Killed> JMG_Metroid_Objective_Update_On_Killed_Registrant("JMG_Metroid_Objective_Update_On_Killed","NewObjectiveID:int,NewObjectiveStringID:int,ObjectiveMarkerObjectID:int,CompleteObjectiveID:int,Delay:float,NewObjectivePriority=1:int");
ScriptRegistrant<JMG_Metroid_Objective_Update_On_Custom> JMG_Metroid_Objective_Update_On_Custom_Registrant("JMG_Metroid_Objective_Update_On_Custom","Custom:int,NewObjectiveID:int,NewObjectiveStringID:int,ObjectiveMarkerObjectID:int,CompleteObjectiveID:int,Delay:float,NewObjectivePriority=1:int");
ScriptRegistrant<JMG_AI_Artillery_Targeting_Fire_At_Custom> JMG_AI_Artillery_Targeting_Fire_At_Custom_Registrant("JMG_AI_Artillery_Targeting_Fire_At_Custom","TargetCustom:int,MinDistance=0.0:float,MaxDistance=999999.0:float,MinAngle=-90.0:float,MaxAngle=90.0:float,UseLowAngleMaxDistance=0.0:float,UseLowAngleWhenAboveMinDistance=1:int,UseLowAngleTargetAboveHeight=9999.9:float");
ScriptRegistrant<JMG_Metroid_AI_Forest_Mini_Boss> JMG_Metroid_AI_Forest_Mini_Boss_Registrant("JMG_Metroid_AI_Forest_Mini_Boss","");
ScriptRegistrant<JMG_Metroid_SpawnRoom_Objective_Update_On_Enter> JMG_Metroid_SpawnRoom_Objective_Update_On_Enter_Registrant("JMG_Metroid_SpawnRoom_Objective_Update_On_Enter","NewObjectiveID:int,NewObjectiveStringID:int,ObjectiveMarkerObjectID:int,CompleteObjectiveID:int,Delay:float,NewObjectivePriority=1:int");
ScriptRegistrant<JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_At_Custom> JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_At_Custom_Registrant("JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_At_Custom","TargetCustom:int,MinDistance=0.0:float,MaxDistance=999999.0:float,MinAngle=-90.0:float,MaxAngle=90.0:float,UseLowAngleMaxDistance=0.0:float,UseLowAngleWhenAboveMinDistance=1:int,UseLowAngleTargetAboveHeight=9999.9:float,VehicleProjectilePreset:string,FireVelocity=1.0:float,GravityScale=1.0:float,FireSound=null:string,ProjectileExplosion=null:string,ReloadTime=1.0:float,MuzzleFlashExplosion=null:string,MissAmount=0.0:float");
ScriptRegistrant<JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_Attach> JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_Attach_Registrant("JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_Attach","LauncherId:int,ExplosionPreset:string");
ScriptRegistrant<JMG_Metroid_Miniboss_Turret> JMG_Metroid_Miniboss_Turret_Registrant("JMG_Metroid_Miniboss_Turret","");
ScriptRegistrant<JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_Custom_Player> JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_Custom_Player_Registrant("JMG_AI_Artillery_Targeting_Fire_Vehicle_Projectile_Custom_Player","TargetCustom:int,MinDistance=0.0:float,MaxDistance=999999.0:float,MinAngle=-90.0:float,MaxAngle=90.0:float,UseLowAngleMaxDistance=0.0:float,UseLowAngleWhenAboveMinDistance=1:int,UseLowAngleTargetAboveHeight=9999.9:float,VehicleProjectilePreset:string,FireVelocity=1.0:float,GravityScale=1.0:float,FireSound=null:string,ProjectileExplosion=null:string,ReloadTime=1.0:float,MuzzleFlashExplosion=null:string,MissAmount=0.0:float,SubtractReloadPerPlayer=0.0:float,MinReloadTime=0.0:float");











