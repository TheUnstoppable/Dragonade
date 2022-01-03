/*	Renegade Scripts.dll
    Dragonade Node Manager Game Mode Framework
	Copyright 2017 Whitedragon, Tiberian Technologies

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
#include "engine_DA.h"
#include "da.h"
#include "da_nodemanager.h"
#include "da_log.h"
#include "GameObjManager.h"
#include "PhysicsSceneClass.h"

DA_API DANodeManagerClass *DASingleton<DANodeManagerClass>::Instance;

static char Icons[3][20] = {
	"p_doubdam",
	"p_ammregen",
	"p_mobsheild"
};

void DABaseNodeClass::Init(const INIClass *INI,const StringClass &Header) {
	Group = Header;
	INI->Get_String(Preset,The_Game()->MapName,StringFormat("%s_NodePreset",Header),Preset);
	INI->Get_String(Model,The_Game()->MapName,StringFormat("%s_NodeModel",Header),Model);
	INI->Get_String(Animation,The_Game()->MapName,StringFormat("%s_NodeAnimation",Header),Animation);

	AttackDefendDistance = INI->Get_Float(The_Game()->MapName,StringFormat("%s_NodeAttackDefendDistance",Header),DANodeManager->Get_Attack_Defend_Distance());
	IgnoreLOS = INI->Get_Bool(The_Game()->MapName,StringFormat("%s_NodeIgnoreLOS",Header),false);
	IsCapturable = INI->Get_Bool(The_Game()->MapName,StringFormat("%s_NodeCapturable",Header),true);
	CaptureOnce = INI->Get_Bool(The_Game()->MapName,StringFormat("%s_NodeCaptureOnce",Header),false);

	Position.X = INI->Get_Float(The_Game()->MapName,StringFormat("%s_Node_X",Header),0.0f);
	Position.Y = INI->Get_Float(The_Game()->MapName,StringFormat("%s_Node_Y",Header),0.0f);
	Position.Z = INI->Get_Float(The_Game()->MapName,StringFormat("%s_Node_Z",Header),0.0f);

	Object = Commands->Create_Object(Preset,Position)->As_PhysicalGameObj();
	Position.Z += 0.5f;
	Commands->Set_Model(Object,Model);
	if (!Animation.Is_Empty()) {
		Commands->Set_Animation(Object,Animation,true,0,0,-1.0f,false);
	}
	Set_Max_Health(Object,Commands->Get_Max_Health(Object)+Commands->Get_Max_Shield_Strength(Object));
	Set_Max_Shield_Strength(Object,0.0f);
	Commands->Set_Health(Object,1.0f);
	Object->Set_Collision_Group(TERRAIN_COLLISION_GROUP);
	Object->Get_Defense_Object()->Set_Can_Object_Die(false);

	Vector3 IconPos = Position;
	IconPos.Z += 1.0f;
	Icon = Create_Object("Generic_Cinematic",IconPos);

	Radar[0] = Create_Object("Invisible_Object",Position);
	Radar[1] = Create_Object("Invisible_Object",Position);
	Radar[0]->Set_Player_Type(0);
	Radar[1]->Set_Player_Type(1);
	Radar[0]->Set_Radar_Blip_Shape_Type(RADAR_BLIP_SHAPE_OBJECTIVE);
	Radar[1]->Set_Radar_Blip_Shape_Type(RADAR_BLIP_SHAPE_OBJECTIVE);

	int StartingTeam = INI->Get_Int(The_Game()->MapName,StringFormat("%s_Team",Header),2);
	if (StartingTeam == 0 || StartingTeam == 1) {
		Captured(StartingTeam);
		if (CaptureOnce) {
			IsCapturable = true;
		}
	}
	else {
		Set_Object_Type(Object,2);
		Team = 2;
		Update_Icon_And_Radar();
	}

	INI->Get_String(Name,The_Game()->MapName,StringFormat("%s_Name",Header));
	
	SentAttackMessage = false;
	SentDefendMessage[0] = false;
	SentDefendMessage[1] = false;
	LastAttackTick = 0;
	LastDefendTick = 0;
	LastAttackTeam = 2;
	PlayerCheck[0] = 0;
	PlayerCheck[1] = 0;
	
	Register_Event(DAEvent::THINK);
}

void DABaseNodeClass::Think() {
	if (IsCapturable) {
		int LastPlayerCheck[2];

		LastPlayerCheck[0] = PlayerCheck[0];
		LastPlayerCheck[1] = PlayerCheck[1];
	
		PlayerCheck[0] = 0;
		PlayerCheck[1] = 0;
		for (SLNode<SoldierGameObj> *z = GameObjManager::SoldierGameObjList.Head();z;z = z->Next()) {
			SoldierGameObj *obj = z->Data();
			if (Is_Player_In_Range(obj)) {
				if (Get_Object_Type(obj) == 0) {
					PlayerCheck[0]++;
				}
				else if (Get_Object_Type(obj) == 1) {
					PlayerCheck[1]++;
				}
			}
		}
		if (Team == 2) {
			int AttackTeam = 2;
			if (!PlayerCheck[0] && !PlayerCheck[1]) {
				if (GetTickCount()-LastDefendTick >= DANodeManager->Get_Attack_Defend_Tick_Time()) {
					if (Commands->Get_Health(Object)-DANodeManager->Get_Attack_Defend_Tick_Amount() < 1.0f) {
						Commands->Set_Health(Object,1.0f);
					}
					else {
						Commands->Set_Health(Object,Commands->Get_Health(Object)-DANodeManager->Get_Attack_Defend_Tick_Amount());
					}
					LastDefendTick = GetTickCount();
				}
			}
			else if (PlayerCheck[0] > PlayerCheck[1]) {
				AttackTeam = 0;
			}
			else if (PlayerCheck[1] > PlayerCheck[0]) {
				AttackTeam = 1;
			}
			if (AttackTeam != 2) {
				if (AttackTeam != LastAttackTeam && LastAttackTeam != 2) {
					Commands->Set_Health(Object,1.0f);
					SentAttackMessage = false;
				}
				LastAttackTeam = AttackTeam;
				if (GetTickCount()-LastAttackTick >= DANodeManager->Get_Attack_Defend_Tick_Time()) {
					LastDefendTick = GetTickCount();
					LastAttackTick = GetTickCount();
					Attack_Tick(AttackTeam);
					LastAttackMessage = GetTickCount();
					if (!SentAttackMessage && Commands->Get_Health(Object) > DANodeManager->Get_Attack_Defend_Tick_Amount()*2.0f) {
						if (!Get_Name().Is_Empty()) {
							StringClass Message;
							Message.Format("%ls is attacking the %s %s Node!",Get_Wide_Team_Name(AttackTeam),Get_Name(),Get_Type());
							DA::Color_Message_With_Team_Color(AttackTeam,"%s",Message);
							DALogManager::Write_Log("_NODE","%d %s",AttackTeam,Message);
						}
						SentAttackMessage = true;
					}
				}
			}
		}
		else {
			int OtherTeam = Team?0:1;
			if (PlayerCheck[OtherTeam] > PlayerCheck[Team]) {
				if (GetTickCount()-LastAttackTick >= DANodeManager->Get_Attack_Defend_Tick_Time()) {
					LastAttackTick = GetTickCount();
					Attack_Tick(OtherTeam);
					LastAttackMessage = GetTickCount();
					if (!SentAttackMessage && Commands->Get_Health(Object) > DANodeManager->Get_Attack_Defend_Tick_Amount()*2.0f) {
						if (Team == 0) {
							DA::Create_2D_Sound_Team(0,"M00EVAN_DSGN0001I1EVAN_snd.wav");
						}
						else {
							DA::Create_2D_Sound_Team(1,"M00EVAG_DSGN0002I1EVAG_snd.wav");
						}
						if (!Get_Name().Is_Empty()) {
							StringClass Message;
							Message.Format("%ls is attacking the %s %s Node!",Get_Wide_Team_Name(OtherTeam),Get_Name(),Get_Type());
							DA::Color_Message_With_Team_Color(OtherTeam,"%s",Message);
							DALogManager::Write_Log("_NODE","%d %s",OtherTeam,Message);
						}
						SentAttackMessage = true;
						SentDefendMessage[0] = false;
						SentDefendMessage[1] = false;
					}
				}
			}
			else if (PlayerCheck[Team] > PlayerCheck[OtherTeam]) {
				if (GetTickCount()-LastDefendTick >= DANodeManager->Get_Attack_Defend_Tick_Time() && Commands->Get_Health(Object) != 1.0f) {
					LastDefendTick = GetTickCount();
					Defend_Tick();
					if ((!PlayerCheck[OtherTeam] || Commands->Get_Health(Object) == 1.0f) && (SentAttackMessage || SentDefendMessage[0])) { 
						if (!SentDefendMessage[1]) {
							if (!Get_Name().Is_Empty()) {
								StringClass Message;
								Message.Format("%ls has defended the %s %s Node!",Get_Wide_Team_Name(Team),Get_Name(),Get_Type());
								DA::Color_Message_With_Team_Color(Team,"%s",Message);
								DALogManager::Write_Log("_NODE","%d %s",Team,Message);
							}
							SentDefendMessage[1] = true;
						}
					}
					else if (!SentDefendMessage[0] && SentAttackMessage) {
						if (!Get_Name().Is_Empty()) {
							StringClass Message;
							Message.Format("%ls is defending the %s %s Node!",Get_Wide_Team_Name(Team),Get_Name(),Get_Type());
							DA::Color_Message_With_Team_Color(Team,"%s",Message);
							DALogManager::Write_Log("_NODE","%d %s",Team,Message);
						}
						SentDefendMessage[0] = true;
						SentDefendMessage[1] = false;
					}
				}
			}
			else if (PlayerCheck[Team] == PlayerCheck[OtherTeam]) {
				if (PlayerCheck[Team]) {
					if (LastPlayerCheck[OtherTeam] > LastPlayerCheck[Team] && SentAttackMessage && !SentDefendMessage[0]) {
						if (!Get_Name().Is_Empty()) {
							StringClass Message;
							Message.Format("%ls is defending the %s %s Node!",Get_Wide_Team_Name(Team),Get_Name(),Get_Type());
							DA::Color_Message_With_Team_Color(Team,"%s",Message);
							DALogManager::Write_Log("_NODE","%d %s",Team,Message);
						}
						SentDefendMessage[0] = true;
						SentDefendMessage[1] = false;
					}
					else if (LastPlayerCheck[Team] > LastPlayerCheck[OtherTeam] && !SentAttackMessage) {
						if (Team == 0) {
							DA::Create_2D_Sound_Team(0,"M00EVAN_DSGN0001I1EVAN_snd.wav");
						}
						else {
							DA::Create_2D_Sound_Team(1,"M00EVAG_DSGN0002I1EVAG_snd.wav");
						}
						if (!Get_Name().Is_Empty()) {
							StringClass Message;
							Message.Format("%ls is attacking the %s %s Node!",Get_Wide_Team_Name(OtherTeam),Get_Name(),Get_Type());
							DA::Color_Message_With_Team_Color(OtherTeam,"%s",Message);
							DALogManager::Write_Log("_NODE","%d %s",OtherTeam,Message);
						}
						LastAttackTick = GetTickCount();
						SentAttackMessage = true;
						SentDefendMessage[0] = false;
						SentDefendMessage[1] = false;
						LastAttackMessage = GetTickCount();
					}
				}
				else if (GetTickCount()-LastDefendTick >= DANodeManager->Get_Attack_Defend_Tick_Time() && Commands->Get_Health(Object) != 1.0f) {
					LastDefendTick = GetTickCount();
					Defend_Tick();
				}
			}
		}
	}
	if (GetTickCount()-LastAttackMessage >= 10000) {
		SentAttackMessage = false;
	}
}

void DABaseNodeClass::Captured(int CaptureTeam) {
	Team = CaptureTeam;
	if (!Get_Name().Is_Empty()) {
		StringClass Message;
		Message.Format("%ls has captured the %s %s Node!",Get_Wide_Team_Name(Team),Get_Name(),Get_Type());
		DA::Color_Message_With_Team_Color(Team,"%s",Message);
		DALogManager::Write_Log("_NODE","%d %s",Team,Message);
	}
	Commands->Set_Health(Object,Commands->Get_Max_Health(Object));
	Set_Object_Type(Object,Team);
	Commands->Set_Health(Object,1.0f);
	Update_Icon_And_Radar();
	Give_Capture_Points();
	SentAttackMessage = false;
	SentDefendMessage[0] = false;
	SentDefendMessage[1] = false;
	if (CaptureOnce) {
		IsCapturable = false;
	}
	Capture_Event();
	Set_Contested(false);
}

void DABaseNodeClass::Attack_Tick(int AttackTeam) {
	int OtherTeam = AttackTeam?0:1;
	float Amount = DANodeManager->Get_Attack_Defend_Tick_Amount()+(DANodeManager->Get_Attack_Defend_Tick_Amount()/2*(PlayerCheck[AttackTeam]-PlayerCheck[OtherTeam]-1));
	if (Amount > 0) {
		Set_Contested(true);
		Give_Attack_Defend_Tick_Points(AttackTeam);
		Commands->Set_Health(Object,Commands->Get_Health(Object)+Amount);
		if (Commands->Get_Health(Object) == Commands->Get_Max_Health(Object)) {
			Captured(AttackTeam);
		}
	}
}

void DABaseNodeClass::Defend_Tick() {
	int OtherTeam = Team?0:1;
	float Amount = DANodeManager->Get_Attack_Defend_Tick_Amount()+(DANodeManager->Get_Attack_Defend_Tick_Amount()/2*(PlayerCheck[Team]?PlayerCheck[Team]-1:0-PlayerCheck[OtherTeam]));
	if (Amount > 0) {
		Set_Contested(false);
		Give_Attack_Defend_Tick_Points(Team);
		if (Commands->Get_Health(Object)-Amount < 1.0f) {
			Commands->Set_Health(Object,1.0f);
		}
		else {
			Commands->Set_Health(Object,Commands->Get_Health(Object)-Amount);
		}
	}
}

void DABaseNodeClass::Update_Icon_And_Radar() {
	Commands->Set_Model(Icon,Icons[Team]);
	Commands->Set_Animation(Icon,StringFormat("%s.%s",Icons[Team],Icons[Team]),true,0,0,-1.0f,false);
	Radar[0]->Set_Radar_Blip_Color_Type(Team);
	Radar[1]->Set_Radar_Blip_Color_Type(Team);
}

bool DABaseNodeClass::Is_Player_In_Range(SoldierGameObj *Player) {
	Vector3 PlayerPosition;
	Player->Get_Position(&PlayerPosition);
	
	if (Commands->Get_Distance(PlayerPosition,Position) > AttackDefendDistance || Commands->Get_Health(Player) == 0.0f || Diff(PlayerPosition.Z,Position.Z) > 4.0f) {
		return false;
	}

	if (IgnoreLOS) {
		return true;
	}
	
	PlayerPosition.Z += 1.3f;
	
	CastResultStruct CastResult;
	PhysRayCollisionTestClass CollisionTest(LineSegClass(PlayerPosition,Position),&CastResult,TERRAIN_ONLY_COLLISION_GROUP);
	
	PhysicsSceneClass::Get_Instance()->Cast_Ray(CollisionTest,false);
	
	return (CollisionTest.CollidedPhysObj == Object->Peek_Physical_Object());
}

void DABaseNodeClass::Give_Attack_Defend_Tick_Points(int team) {
	for (SLNode<SoldierGameObj> *z = GameObjManager::StarGameObjList.Head();z;z = z->Next()) {
		SoldierGameObj *obj = z->Data();
		if (Get_Object_Type(obj) == team && Is_Player_In_Range(obj)) {
			cPlayer *Player = (cPlayer*)obj->Get_Player_Data();
			Player->Score += DANodeManager->Get_Attack_Defend_Tick_Points();
			Player->Money += DANodeManager->Get_Attack_Defend_Tick_Points();
			Player->Mark_As_Modified();
			if (DANodeManager->Get_Attack_Defend_Tick_Veteran_Points()) {
				Commands->Send_Custom_Event(Object,obj,558222,DANodeManager->Get_Attack_Defend_Tick_Veteran_Points(),0);
			}
		}
	}
}

void DABaseNodeClass::Give_Capture_Points() {
	for (SLNode<SoldierGameObj> *z = GameObjManager::StarGameObjList.Head();z;z = z->Next()) {
		SoldierGameObj *obj = z->Data();
		if (Get_Object_Type(obj) == Team && Is_Player_In_Range(obj)) {
			Grant_Refill(obj);
			cPlayer *Player = (cPlayer*)obj->Get_Player_Data();
			Player->Score += DANodeManager->Get_Capture_Points();
			Player->Money += DANodeManager->Get_Capture_Points();
			Player->Mark_As_Modified();
			if (DANodeManager->Get_Capture_Veteran_Points()) {
				Commands->Send_Custom_Event(Object,obj,558222,DANodeManager->Get_Capture_Veteran_Points(),0);
			}
		}
	}
}

void DASpawnNodeClass::Init(const INIClass *INI,const StringClass &Header) {
	DABaseNodeClass::Init(INI,Header);
	if (DASpawnManager) {
		SpawnPoint = DASpawnManager->Create_Spawn_Point(INI,Header);
		SpawnPoint->Set_Center_Point(Position);
		if (Team == 2) {
			SpawnPoint->Set_Enabled(false);
			SpawnPoint->Set_Team(2);
		}
	}
	else {
		SpawnPoint = 0; //Presumably an MP map.
	}
}

void DASpawnNodeClass::Contested_Event() {
	if (SpawnPoint) {
		if (Is_Contested()) {
			SpawnPoint->Set_Spawn_Time(DANodeManager->Get_Contested_Spawn_Time());
		}
		else {
			SpawnPoint->Set_Spawn_Time(SpawnPoint->Get_Base_Spawn_Time());
		}
	}
}

void DASpawnNodeClass::Capture_Event() {
	if (SpawnPoint) {
		SpawnPoint->Set_Enabled(true);
		SpawnPoint->Set_Team(Team);
		SpawnPoint->Clear_Spawn_List_Find_New_Spawn();
		SpawnPoint->Set_Spawn_Time(SpawnPoint->Get_Base_Spawn_Time());
	}
}

void DANodeManagerClass::Init(const INIClass *INI) {
	AttackDefendDistance = INI->Get_Float(The_Game()->MapName,"NodeAttackDefendDistance",INI->Get_Float("General","NodeAttackDefendDistance",15.0f));
	AttackDefendTickTime = (unsigned int)(INI->Get_Float(The_Game()->MapName,"NodeAttackDefendTickTime",INI->Get_Float("General","NodeAttackDefendTickTime",5.0f))*1000);
	AttackDefendTickAmount = INI->Get_Float(The_Game()->MapName,"NodeAttackDefendTickAmount",INI->Get_Float("General","NodeAttackDefendTickAmount",20.0f));
	AttackDefendTickPoints = INI->Get_Float(The_Game()->MapName,"NodeAttackDefendTickPoints",INI->Get_Float("General","NodeAttackDefendTickPoints",10.0f));
	AttackDefendTickVeteranPoints = INI->Get_Int(The_Game()->MapName,"NodeAttackDefendTickVeteranPoints",INI->Get_Int("General","NodeAttackDefendTickVeteranPoints",0));
	CapturePoints = INI->Get_Float(The_Game()->MapName,"NodeCapturePoints",INI->Get_Float("General","NodeCapturePoints",50.0f));
	CaptureVeteranPoints = INI->Get_Int(The_Game()->MapName,"NodeCaptureVeteranPoints",INI->Get_Int("General","NodeCaptureVeteranPoints",3));
	ContestedSpawnTime = (unsigned int)(INI->Get_Float(The_Game()->MapName,"NodeContestedSpawnTime",INI->Get_Float("General","NodeContestedSpawnTime",5.0f))*1000);
}
