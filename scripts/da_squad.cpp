/*	Renegade Scripts.dll
    Dragonade Squads Game Feature
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
#include "da_squad.h"
#include "da_settings.h"
#include "da_translation.h"
#include "da_log.h"
#include "da_gameobj.h"

void DASquadMemberClass::Init() {
	Register_Chat_Command((DAPOCC)&DASquadMemberClass::Disband_Chat_Command,"!disband");
	Register_Chat_Command((DAPOCC)&DASquadMemberClass::Promote_Chat_Command,"!promote",1);
	Register_Chat_Command((DAPOCC)&DASquadMemberClass::Info_Chat_Command,"!sinfo");
	Register_Chat_Command((DAPOCC)&DASquadMemberClass::SKick_Chat_Command,"!skick",1);
	Register_Chat_Command((DAPOCC)&DASquadMemberClass::Msg_Chat_Command,"!sm|!ms|!sc|!cs",1);
	if (Get_GameObj()) {
		Create_Radar();
	}
}

DASquadMemberClass::~DASquadMemberClass() {
	if (DASquadManager) {
		Squad->Internal_Remove(this);
		for (int i = 0;i < Squad->Size();i++) {
			Squad->Get_Member(i)->Remove_Radar(this);
		}
	}
	if (Radar) {
		Radar->Set_Delete_Pending();
	}
}

void DASquadMemberClass::Team_Change() {
	if (Get_Team() != Squad->Get_Team()) {
		Leave_Squad();
	}
	else if (Radar) {
		((PhysicalGameObj*)Radar.Get_Ptr())->Set_Player_Type(Get_Team());
	}
}

void DASquadMemberClass::Created() {
	Update_Radar();
	Commands->Attach_To_Object_Bone(Radar,Get_GameObj(),"Origin");
}

void DASquadMemberClass::Player_Loaded() {
	Create_Radar();
}

void DASquadMemberClass::Vehicle_Enter(VehicleGameObj *Vehicle,int Seat) {
	Update_Radar();
}

void DASquadMemberClass::Vehicle_Exit(VehicleGameObj *Vehicle,int Seat) {
	Update_Radar();
}

void DASquadMemberClass::Character_Purchase(float Cost,const SoldierGameObjDef *Item) {
	Squad->Squad_Message_Except(Get_Owner(),"Purchase: %ls - %s",Get_Name(),DATranslationManager::Translate(Item));
}

void DASquadMemberClass::Vehicle_Purchase(float Cost,const VehicleGameObjDef *Item) {
	Squad->Squad_Message_Except(Get_Owner(),"Purchase: %ls - %s",Get_Name(),DATranslationManager::Translate(Item));
}

void DASquadMemberClass::PowerUp_Purchase(float Cost,const PowerUpGameObjDef *Item) {
	Squad->Squad_Message_Except(Get_Owner(),"Purchase: %ls - %s",Get_Name(),DATranslationManager::Translate(Item));
}

bool DASquadMemberClass::Disband_Chat_Command(const DATokenClass &Text,TextMessageEnum ChatType) {
	if (!Is_Leader()) {
		DA::Private_Color_Message(Get_ID(),SQUADCOLOR,"You must be the squad leader to use this command.");
	}
	else {
		Squad->Disband();
	}
	return false;
}

bool DASquadMemberClass::Promote_Chat_Command(const DATokenClass &Text,TextMessageEnum ChatType) {
	if (!Is_Leader()) {
		DA::Private_Color_Message(Get_ID(),SQUADCOLOR,"You must be the squad leader to use this command.");
	}
	else {
		cPlayer *MatchPlayer = Match_Player(Get_Owner(),Text[1],false);
		if (MatchPlayer) {
			if (!Squad->Is_Member(MatchPlayer)) {
				DA::Private_Color_Message(Get_ID(),SQUADCOLOR,"%ls is not in your squad.",MatchPlayer->Get_Name());
			}
			else {
				Squad->Set_Leader(MatchPlayer);
			}
		}
	}
	return false;
}

bool DASquadMemberClass::Info_Chat_Command(const DATokenClass &Text,TextMessageEnum ChatType) {
	StringClass Str = "Leader: " + StringClass(Squad->Get_Leader()->Get_Name()) + "(" + DATranslationManager::Translate_Soldier(Squad->Get_Leader()->Get_GameObj()) + ")";
	Str += " - Members: ";
	for (int i = 1;i < Squad->Size();i++) {
		if (i > 1) {
			Str += ", " + StringClass(Squad->Get_Member(i)->Get_Name()) + "(" + DATranslationManager::Translate_Soldier(Squad->Get_Member(i)->Get_GameObj()) + ")";
		}
		else {
			Str += StringClass(Squad->Get_Member(i)->Get_Name()) + "(" + DATranslationManager::Translate_Soldier(Squad->Get_Member(i)->Get_GameObj()) + ")";
		}
	}
	DA::Private_Color_Message(Get_ID(),SQUADCOLOR,"%s",Str);
	return false;
}

bool DASquadMemberClass::SKick_Chat_Command(const DATokenClass &Text,TextMessageEnum ChatType) {
	if (!Is_Leader()) {
		DA::Private_Color_Message(Get_ID(),SQUADCOLOR,"You must be the squad leader to use this command.");
	}
	else {
		for (int i = 1;i <= Text.Size();i++) {
			cPlayer *MatchPlayer = Match_Player(Get_Owner(),Text[i],false);
			if (MatchPlayer) {
				if (Squad->Remove(MatchPlayer)) {
					DA::Private_Color_Message(MatchPlayer,SQUADCOLOR,"%ls has removed you from the squad.",Get_Name());
					Squad->Squad_Message_Except(MatchPlayer,"%ls was removed from the squad.",MatchPlayer->Get_Name());
				}
				else if (DASquadManager->Remove_WaitList(MatchPlayer,Get_Owner())) {
					DA::Private_Color_Message(MatchPlayer,SQUADCOLOR,"%ls has removed you from the wait list.",Get_Name());
					Squad->Leader_Message("%ls was removed from the wait list.",MatchPlayer->Get_Name());
				}
				else if (DASquadManager->Remove_Invite(MatchPlayer,Get_Owner())) {
					DA::Private_Color_Message(MatchPlayer,SQUADCOLOR,"%ls has rescinded your squad invitation.",Get_Name());
					Squad->Leader_Message("%ls squad invitation was rescinded.",Make_Possessive(MatchPlayer->Get_Name()));
				}
			}
		}
	}
	return false;
}

bool DASquadMemberClass::Msg_Chat_Command(const DATokenClass &Text,TextMessageEnum ChatType) {
	Squad->Squad_Chat(Get_Owner(),"%s",Text[0]);
	return false;
}

void DASquadMemberClass::Create_Radar() {
	Radar = Create_Object_Attach_To_Object(Get_GameObj(),"Invisible_Object","Origin");
	((PhysicalGameObj*)Radar.Get_Ptr())->Set_Radar_Blip_Shape_Type(RADAR_BLIP_SHAPE_OBJECTIVE);
	((PhysicalGameObj*)Radar.Get_Ptr())->Set_Player_Type(Get_Team());
	DAGameObjManager::Set_GameObj_Invisible_No_Delete(Radar);
	Update_Radar();
	for (int i = 0;i < Squad->Size();i++) {
		if (Squad->Get_Member(i) != this) {
			Add_Radar(Squad->Get_Member(i));
			Squad->Get_Member(i)->Add_Radar(this);
		}
	}
}

void DASquadMemberClass::Update_Radar() {
	if (Radar) {
		SoldierGameObj *Soldier = Get_GameObj();
		if (Soldier && Soldier->Get_Vehicle()) {
			((PhysicalGameObj*)Radar.Get_Ptr())->Set_Radar_Blip_Color_Type(RADAR_BLIP_COLOR_SECONDARY_OBJECTIVE);
		}
		else {
			((PhysicalGameObj*)Radar.Get_Ptr())->Set_Radar_Blip_Color_Type(RADAR_BLIP_COLOR_PRIMARY_OBJECTIVE);
		}
	}
}

void DASquadMemberClass::Add_Radar(DASquadMemberClass *Member) {
	if (Radar) {
		Radar->Set_Object_Dirty_Bit(Member->Get_ID(),NetworkObjectClass::BIT_CREATION,true);
	}
}

void DASquadMemberClass::Remove_Radar(DASquadMemberClass *Member) {
	if (Radar && !Radar->Is_Delete_Pending() && Member->Get_Owner()->Is_Active()) {
		Radar->Set_Is_Delete_Pending(true);
		Update_Network_Object_Player(Radar,Member->Get_ID());
		Radar->Set_Is_Delete_Pending(false);
	}
}

bool DASquadMemberClass::Is_Leader() {
	return Squad->Get_Leader() == this;
}

void DASquadMemberClass::Leave_Squad() {
	Set_Delete_Pending();
}




DASquadClass::DASquadClass(cPlayer *Player) {
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		if (z->Data()->Is_Active() && z->Data() != Player) {
			DA::Private_Color_Message(z->Data(),SQUADCOLOR,"%ls has created a squad.",Player->Get_Name());
		}
	}
	DALogManager::Write_Log("_SQUAD","%ls has created a squad.",Player->Get_Name());
	DA::Private_Color_Message(Player,SQUADCOLOR,"You have created a squad.");
	DASquadMemberClass *NewMember = new DASquadMemberClass(this);
	Player->Get_DA_Player()->Add_Observer(NewMember);
	Members.Add(NewMember);
	Team = Player->Get_Player_Type();
}

void DASquadClass::Add(cPlayer *Player) {
	Squad_Message("%ls has joined the squad.",Player->Get_Name());
	DA::Private_Color_Message(Player,SQUADCOLOR,"You have joined %ls squad.",Make_Possessive(Get_Leader()->Get_Name()));
	DASquadMemberClass *NewMember = new DASquadMemberClass(this);
	Player->Get_DA_Player()->Add_Observer(NewMember);
	Members.Add(NewMember);
	Disbanded = false;
}

bool DASquadClass::Remove(cPlayer *Player) {
	for (int i = 0;i < Members.Count();i++) {
		if (Members[i]->Get_Owner() == Player) {
			Members[i]->Leave_Squad();
			return true;
		}
	}
	return false;
}

void DASquadClass::Internal_Remove(DASquadMemberClass *Member) {
	for (int i = 0;i < Members.Count();i++) {
		if (Members[i] == Member) {
			Members.Delete(i);
			if (!Disbanded) {
				Squad_Message("%ls has left the squad.",Member->Get_Name());
				if (Member->Get_Owner()->Is_Active()) {
					DA::Private_Color_Message(Member->Get_ID(),SQUADCOLOR,"You have left the squad.");
				}
				if (i == 0 && Members.Count() > 1) {
					Squad_Message_Except(Get_Leader()->Get_Owner(),"%ls is now the squad leader.",Get_Leader()->Get_Name());
					Leader_Message("You are now the squad leader.");
				}
				if (Size() <= 1) {
					Disband();
				}
			}
			break;
		}
	}
}

void DASquadClass::Disband() {
	Squad_Message("Your squad has been disbanded.");
	for (int i = 0;i < Members.Count();i++) {
		Members[i]->Leave_Squad();
	}
	Disbanded = true;
}

void DASquadClass::Squad_Chat(cPlayer *Player,const char *Format,...) {
	char Message[512];
	Format_String(Message);
	for (int i = 0;i < Members.Count();i++) {
		if (Members[i]->Get_Owner()->Is_Active()) {
			Create_2D_WAV_Sound_Player_By_ID(Members[i]->Get_ID(),"yo1.wav");
			DA::Private_Color_Message(Members[i]->Get_ID(),SQUADCOLOR,"%ls: %s",Player->Get_Name(),Message);
		}
	}
}

void DASquadClass::Squad_Message(const char *Format,...) {
	char Message[512];
	Format_String(Message);
	for (int i = 0;i < Members.Count();i++) {
		if (Members[i]->Get_Owner()->Is_Active()) {
			DA::Private_Color_Message(Members[i]->Get_ID(),SQUADCOLOR,"%s",Message);
		}
	}
}

void DASquadClass::Squad_Message_Except(cPlayer *Player,const char *Format,...) {
	char Message[512];
	Format_String(Message);
	for (int i = 0;i < Members.Count();i++) {
		if (Members[i]->Get_Owner() != Player && Members[i]->Get_Owner()->Is_Active()) {
			DA::Private_Color_Message(Members[i]->Get_ID(),SQUADCOLOR,"%s",Message);
		}
	}
}

void DASquadClass::Leader_Message(const char *Format,...) {
	if (Get_Leader() && Get_Leader()->Get_Owner()->Is_Active()) {
		char Message[512];
		Format_String(Message);
		DA::Private_Color_Message(Get_Leader()->Get_ID(),SQUADCOLOR,"%s",Message);
	}
}

void DASquadClass::Set_Leader(cPlayer *Player) {
	for (int i = 0;i < Members.Count();i++) {
		if (Members[i]->Get_Owner() == Player) {
			DASquadMemberClass *Temp = Members[i];
			Members.Delete(i);
			Members.Add_Head(Temp);
			Squad_Message("%ls has been promoted to squad leader.",Temp->Get_Name());
			break;
		}
	}
}

void DASquadClass::Set_Team(int Team) {
	this->Team = Team;
	for (int i = Size()-1;i >= 0;i--) {
		if (!Members[i]->Is_Delete_Pending()) {
			Change_Team_4(Members[i]->Get_Owner(),Team);
			Members[i]->Get_Owner()->Get_DA_Player()->Set_Needs_Team(false);
		}
	}
}

void DASquadClass::Check_Team() {
	int TeamCount[2] = {0,0};
	for (int i = 0;i < Size();i++) {
		if (!Members[i]->Is_Delete_Pending()) {
			if (Members[i]->Get_Team() == 0) {
				TeamCount[0]++;
			}
			else if (Members[i]->Get_Team() == 1) {
				TeamCount[1]++;
			}
		}
	}
	if (TeamCount[0] == 1 && TeamCount[1] == 1) {
		if (Members[0]->Get_Team() != Get_Team()) {
			Members[0]->Leave_Squad();
		}
		else {
			Members[1]->Leave_Squad();
		}
	}
	else {
		if (TeamCount[0] > TeamCount[1]) {
			Team = 0;
		}
		else if (TeamCount[1] > TeamCount[0]) {
			Team = 1;
		}
		else {
			Team = Get_Leader()->Get_Team();
		}
		for (int i = Size()-1;i >= 0;i--) {
			if (Members[i]->Get_Team() != Team) {
				Members[i]->Leave_Squad();
			}
		}
	}
}

bool DASquadClass::Is_Full() {
	return Size() >= DASquadManager->Get_Max_Squad_Size();
}

int DASquadClass::Active_Size() {
	int Ret = 0;
	for (int i = 0;i < Size();i++) {
		if (!Members[i]->Is_Delete_Pending()) {
			Ret++;
		}
	}
	return Ret;
}




void DASquadManagerClass::Init() {
	Register_Event(DAEvent::LEVELLOADED);
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Event(DAEvent::REMIX);
	Register_Event(DAEvent::REBALANCE);
	Register_Event(DAEvent::SWAP);
	Register_Event(DAEvent::PLAYERLEAVE);
	Register_Event(DAEvent::TEAMCHANGE);
	Register_Event(DAEvent::THINK);
	Register_Event(DAEvent::CHAT);
	Register_Chat_Command((DAECC)&DASquadManagerClass::List_Chat_Command,"!squads|!squad");
	Register_Chat_Command((DAECC)&DASquadManagerClass::Join_Chat_Command,"!join",1);
	Register_Chat_Command((DAECC)&DASquadManagerClass::Invite_Chat_Command,"!invite",1);
	Register_Chat_Command((DAECC)&DASquadManagerClass::Accept_Chat_Command,"!accept");
	Register_Chat_Command((DAECC)&DASquadManagerClass::Decline_Chat_Command,"!decline");
	Register_Chat_Command((DAECC)&DASquadManagerClass::Leave_Chat_Command,"!leave");
}

DASquadManagerClass::~DASquadManagerClass() {
	for (int i = 0;i < SquadList.Count();i++) {
		SquadList[i]->Disband();
		delete SquadList[i];
	}
	SquadList.Delete_All();
}

void DASquadManagerClass::Level_Loaded_Event() {
	Check_Teams();
	Check_Size();
	Invites.Delete_All();
	Joins.Delete_All();
	WaitList.Delete_All();
}

void DASquadManagerClass::Settings_Loaded_Event() {
	RemixSquads = DASettingsManager::Get_Bool("RemixSquads",true);
	MaxSquadSize = WWMath::Clamp_Int(DASettingsManager::Get_Int("MaxSquadSize",5),2,INT_MAX);
}

void DASquadManagerClass::Player_Leave_Event(cPlayer *Player) {
	Clear_Lists(Player);
}

void DASquadManagerClass::Team_Change_Event(cPlayer *Player) {
	if (RemixSquads) {
		for (int i = 0;i < WaitList.Count();i++) {
			if (WaitList[i].Player == Player) {
				DASquadClass *Squad = Find_Squad(WaitList[i].Leader);
				if (Squad) {
					if (Squad->Get_Team() == Player->Get_Player_Type() && Squad->Is_Leader(WaitList[i].Leader) && !Squad->Is_Full()) {
						Squad->Add(Player);
					}
				}
				else if (WaitList[i].Leader->Get_Player_Type() == Player->Get_Player_Type() && Can_Create_Squads()) {
					DASquadClass *NewSquad = Create_Squad(WaitList[i].Leader);
					NewSquad->Add(Player);
				}
				WaitList.Delete(i);
				break;
			}
			else if (WaitList[i].Leader == Player) {
				DASquadClass *Squad = Find_Squad(WaitList[i].Leader);
				if (Squad) {
					if (Squad->Get_Team() == WaitList[i].Player->Get_Player_Type() && Squad->Is_Leader(Player) && !Squad->Is_Full()) {
						Squad->Add(WaitList[i].Player);
					}
				}
				else if (WaitList[i].Leader->Get_Player_Type() == Player->Get_Player_Type() && Can_Create_Squads()) {
					DASquadClass *NewSquad = Create_Squad(Player);
					NewSquad->Add(WaitList[i].Player);
				}
				WaitList.Delete(i);
				i--;
			}
		}
	}
	Clear_Lists(Player);
}

void DASquadManagerClass::Remix_Event() {
	if (RemixSquads) {
		Check_Size();
		
		for (int i = 0;i < WaitList.Count();i++) {
			DASquadClass *Squad = Find_Squad(WaitList[i].Leader);
			if (!Squad && Can_Create_Squads()) {
				DASquadClass *NewSquad = Create_Squad(WaitList[i].Leader);
				NewSquad->Add(WaitList[i].Player);
			}
			else if (Squad->Is_Leader(WaitList[i].Leader) && !Squad->Is_Full()) {
				Squad->Add(WaitList[i].Player);
			}
		}
		WaitList.Delete_All();

		if (SquadList.Count() == 1) { //If theres only one squad just put it on a random team.
			SquadList[0]->Set_Team(Get_Random_Bool());	
		}
		else if (SquadList.Count() == 2) { //If there are two squads they go on opposite teams.
			int RandTeam = Get_Random_Bool();
			SquadList[0]->Set_Team(RandTeam);
			SquadList[1]->Set_Team(!RandTeam);
		}
		else if (SquadList.Count() >= 3) { //If there are three or more we make many possible team combinations and choose the one that puts the most balanced amount of squad members on each team.
			int LeastDiff = 127;
			DynamicVectorClass<DASquadRemixStruct> LeastDiffSquadTeams;
			for (int loops = 0;loops < 100;loops++) {
				int LoopTeamCount[2] = {0,0};
				DynamicVectorClass<DASquadRemixStruct> SquadTeams;
				for (int i = 0;i < SquadList.Count();i++) {
					if (SquadList[i]->Active_Size()) {
						int RandTeam = Get_Random_Bool();
						LoopTeamCount[RandTeam] += SquadList[i]->Active_Size();
						SquadTeams.Add(DASquadRemixStruct(SquadList[i],RandTeam));
					}
				}
				if (Diff(LoopTeamCount[0],LoopTeamCount[1]) < 2) { //Not going to get any better than this.
					LeastDiffSquadTeams = SquadTeams;
					LeastDiff = Diff(LoopTeamCount[0],LoopTeamCount[1]);
					break;
				}
				else if (Diff(LoopTeamCount[0],LoopTeamCount[1]) < LeastDiff) {
					LeastDiffSquadTeams = SquadTeams;
					LeastDiff = Diff(LoopTeamCount[0],LoopTeamCount[1]);
				}
			}
			for (int i = 0;i < LeastDiffSquadTeams.Count();i++) {
				LeastDiffSquadTeams[i].Squad->Set_Team(LeastDiffSquadTeams[i].Team);
			}
		}
	}
}

void DASquadManagerClass::Rebalance_Event() {
	int TeamCount[2] = {0,0};
	int NoSquadTeamCount[2] = {0,0};
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) { 
		cPlayer *Player = z->Data(); 
		if (Player->Is_Active()) { 
			if (Player->Get_Player_Type() == 0) { 
				TeamCount[0]++;
				if (!Find_Squad(Player)) {
					NoSquadTeamCount[0]++;
				}
			} 
			else if (Player->Get_Player_Type() == 1) {
				TeamCount[1]++;
				if (!Find_Squad(Player)) {
					NoSquadTeamCount[1]++;
				}
			} 
		}
	}

	int OldTeam = -1;
	int NewTeam = -1;
	if (TeamCount[0] > TeamCount[1]) {
		OldTeam = 0;
		NewTeam = 1;
	}
	else {
		OldTeam = 1;
		NewTeam = 0;
	}
	//Teams are uneven, try to balance them without touching the squads.
	while ((TeamCount[OldTeam]-TeamCount[NewTeam]) > 1 && NoSquadTeamCount[OldTeam]) { //Go until teams are balanced or we run out of players not in a squad.
		int Rand = Get_Random_Int(0,NoSquadTeamCount[OldTeam]); //Select random player on team to change.
		int Count = 0;
		for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
			cPlayer *Player = z->Data();
			if (Player->Is_Active() && Player->Get_Player_Type() == OldTeam && !Find_Squad(Player)) {
				if (Count == Rand) { //Loop until we find that player.
					Change_Team_3(Player,NewTeam);
					TeamCount[OldTeam]--;
					TeamCount[NewTeam]++;
					NoSquadTeamCount[OldTeam]--;
					NoSquadTeamCount[NewTeam]++;
					break;
				}
				Count++;
			}
		}
	}
	
	if ((TeamCount[OldTeam]-TeamCount[NewTeam]) > 1) { //Teams are still uneven, going to have to start changing people in squads.
		for (int i = SquadList.Count()-1;i >= 0 && TeamCount[OldTeam]-TeamCount[NewTeam] > 1;i--) { //Go until the teams are even or we run out of squads.
			DASquadClass *Squad = SquadList[i];
			for (int j = Squad->Size()-1;j >= 0 && TeamCount[OldTeam]-TeamCount[NewTeam] > 1;j--) { //Start with the last member of the last squad and work backwards.
				if (Squad->Get_Team() == OldTeam) {
					DASquadMemberClass *Member = Squad->Get_Member(j);
					Change_Team_3(Member->Get_Owner(),NewTeam);
					TeamCount[OldTeam]--;
					TeamCount[NewTeam]++;
				}
			}
		}
	}
	
	Check_Teams();
}

void DASquadManagerClass::Swap_Event() {
	for (int i = 0;i < SquadList.Count();i++) {
		SquadList[i]->Set_Team(!SquadList[i]->Get_Team());
	}
}

void DASquadManagerClass::Think() {
	for (int i = SquadList.Count()-1;i >= 0;i--) {
		if (!SquadList[i]->Size()) {
			delete SquadList[i];
			SquadList.Delete(i);
		}
	}

	for (int i = Invites.Count()-1;i >= 0;i--) {
		if (The_Game()->Get_Game_Duration_S()-Invites[i].Timeout >= 30) {
			Invites.Delete(i);
		}
	}

	for (int i = Joins.Count()-1;i >= 0;i--) {
		if (The_Game()->Get_Game_Duration_S()-Joins[i].Timeout >= 30) {
			Joins.Delete(i);
		}
	}
}

bool DASquadManagerClass::Chat_Event(cPlayer *Player,TextMessageEnum Type,const wchar_t *Message,int ReceiverID) {
	if (Type == TEXT_MESSAGE_PRIVATE && Player->Get_Id() == ReceiverID) {
		DASquadClass *Squad = Find_Squad(Player);
		if (Squad) {
			Squad->Squad_Chat(Player,"%ls",Message);
			return false;
		}
	}
	return true;
}

bool DASquadManagerClass::List_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	if (SquadList.Count()) {
		for (int i = 0;i < SquadList.Count();i++) {
			DASquadClass *Squad = SquadList[i];
			if (Squad->Active_Size() && Squad->Get_Team() == 0) {
				StringClass Str;
				Str.Format("Leader: %ls - Members: ",Squad->Get_Leader()->Get_Name());
				for (int x = 1;x < Squad->Size();x++) {
					Str += StringClass(Squad->Get_Member(x)->Get_Name()) + ", ";
				}
				Str[Str.Get_Length()-2] = '\0';
				DA::Private_Color_Message_With_Team_Color(Player,0,"%s",Str);
			}
		}

		for (int i = 0;i < SquadList.Count();i++) {
			DASquadClass *Squad = SquadList[i];
			if (Squad->Active_Size() && Squad->Get_Team() == 1) {
				StringClass Str;
				Str.Format("Leader: %ls - Members: ",Squad->Get_Leader()->Get_Name());
				for (int x = 1;x < Squad->Size();x++) {
					Str += StringClass(Squad->Get_Member(x)->Get_Name()) + ", ";
				}
				Str[Str.Get_Length()-2] = '\0';
				DA::Private_Color_Message_With_Team_Color(Player,1,"%s",Str);
			}
		}
		if (Can_Create_Squads()) {
			DA::Private_Color_Message(Player,SQUADCOLOR,"You can request to join one of these squads by typing \"!join <player name>\" in chat. Or create your own with \"!invite <player name>\". The current maxiumum size for a squad is %d(%d) players.",Get_Max_Squad_Size(),MaxSquadSize);
		}
		else {
			DA::Private_Color_Message(Player,SQUADCOLOR,"There are currently not enough players to create a squad. When there are, you can create one by typing \"!invite <player name>\".");
		}
	}
	else if (Can_Create_Squads()) {
		DA::Private_Color_Message(Player,SQUADCOLOR,"There are no squads. You can create one by typing \"!invite <player name>\" in chat. The current maxiumum size for a squad is %d(%d) players.",Get_Max_Squad_Size(),MaxSquadSize);
	}
	else {
		DA::Private_Color_Message(Player,SQUADCOLOR,"There are currently not enough players to create a squad. When there are, you can create one by typing \"!invite <player name>\".");
	}
	
	/*Console_InputF("msg squad max size %d squads %d",Get_Max_Squad_Size(),SquadList.Count());
	for (int i = 0;i < SquadList.Count();i++) {
	DASquadClass *Squad = SquadList[i];
	Console_InputF("msg squad");
		for (int i = 0;i < Squad->Size();i++) {
			Console_InputF("msg member %ls",Squad->Get_Member(i)->Get_Name());
		}
	}
	for (int i = 0;i < Joins.Count();i++) {
		Console_InputF("msg join %ls %ls %d",Joins[i].Player->Get_Name(),Joins[i].Leader->Get_Name(),Joins[i].Timeout);
	}
	for (int i = 0;i < Invites.Count();i++) {
		Console_InputF("msg invite %ls %ls %d",Invites[i].Player->Get_Name(),Invites[i].Leader->Get_Name(),Invites[i].Timeout);
	}
	for (int i = 0;i < WaitList.Count();i++) {
		Console_InputF("msg join %ls %ls %d",WaitList[i].Player->Get_Name(),WaitList[i].Leader->Get_Name(),WaitList[i].Timeout);
	}*/
	return true;
}

bool DASquadManagerClass::Join_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	if (Find_Squad(Player)) {
		DA::Private_Color_Message(Player,SQUADCOLOR,"You are already in a squad.");
	}
	else {
		cPlayer *MatchPlayer = Match_Player(Player,Text[1],false);
		if (MatchPlayer) {
			DASquadClass *Squad = Find_Squad(MatchPlayer);
			if (!Squad) {
				for (int i = 0;i < Invites.Count();i++) {
					if (Invites[i].Player == MatchPlayer && Invites[i].Leader == Player) {
						Invite_Accepted(i);
						return false;
					}
				}
				DA::Private_Color_Message(Player,SQUADCOLOR,"%ls is not in a squad.",MatchPlayer->Get_Name());
			}
			else if (Squad->Is_Full()) {
				DA::Private_Color_Message(Player,SQUADCOLOR,"%ls squad is full.",Make_Possessive(Squad->Get_Leader()->Get_Name()));
			}
			else if (Is_Join_Pending(Player,Squad->Get_Leader()->Get_Owner())) {
				DA::Private_Color_Message(Player,SQUADCOLOR,"You have already requested to join %ls squad.",Make_Possessive(Squad->Get_Leader()->Get_Name()));
			}
			else if (!RemixSquads && Squad->Get_Team() != Player->Get_Player_Type()) {
				DA::Private_Color_Message(Player,SQUADCOLOR,"You cannot join squads on the other team.");
			}
			else {
				for (int i = 0;i < Invites.Count();i++) {
					if (Invites[i].Player == Player && Squad->Is_Leader(Invites[i].Leader)) {
						Invite_Accepted(i);
						return false;
					}
				}
				Clear_Lists(Player);
				DASquadInviteStruct Struct(Squad->Get_Leader()->Get_Owner(),Player);
				Joins.Add(Struct);
				DA::Private_Color_Message(Player,SQUADCOLOR,"You have requested to join %ls squad.",Make_Possessive(Squad->Get_Leader()->Get_Name()));
				Squad->Leader_Message("%ls has requested to join your squad. Type \"!accept\" to accept, or \"!decline\" to decline.",Player->Get_Name());
			}
		}
	}
	return false;
}

bool DASquadManagerClass::Invite_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	DASquadClass *Squad = Find_Squad(Player);
	if (Squad) {
		if (!Squad->Is_Leader(Player)) {
			DA::Private_Color_Message(Player,SQUADCOLOR,"You must be the squad leader to use this command.");
		}
		else if (Squad->Is_Full()) {
			DA::Private_Color_Message(Player,SQUADCOLOR,"Your squad is full.");
		}
		else {
			for (int j = 1;j <= Text.Size();j++) {
				cPlayer *MatchPlayer = Match_Player(Player,Text[j],false);
				if (MatchPlayer) {
					for (int i = 0;i < Joins.Count();i++) {
						if (Joins[i].Leader == Player && Joins[i].Player == MatchPlayer) {
							Join_Accepted(i);
							return false;
						}
					}
					if (Is_Invite_Pending(MatchPlayer) || Is_WaitList_Pending(MatchPlayer)) {
						DA::Private_Color_Message(Player,SQUADCOLOR,"%ls has already been invited to a squad.",MatchPlayer->Get_Name());
					}
					else if (Find_Squad(MatchPlayer)) {
						DA::Private_Color_Message(Player,SQUADCOLOR,"%ls is already in a squad.",MatchPlayer->Get_Name());
					}
					else if (!RemixSquads && Squad->Get_Team() != MatchPlayer->Get_Player_Type()) {
						DA::Private_Color_Message(Player,SQUADCOLOR,"You cannot invite players on the other team.");
					}
					else {
						Invite(MatchPlayer,Player);
					}
				}
			}
		}
	}
	else if (!Can_Create_Squads()) {
		DA::Private_Color_Message(Player,SQUADCOLOR,"There are not enough players to create a squad.");
	}
	else {
		for (int i = 1;i <= Text.Size();i++) {
			cPlayer *MatchPlayer = Match_Player(Player,Text[i],false);
			if (MatchPlayer) {
				if (Is_Invite_Pending(MatchPlayer) || Is_WaitList_Pending(MatchPlayer)) {
					DA::Private_Color_Message(Player,SQUADCOLOR,"%ls has already been invited to a squad.",MatchPlayer->Get_Name());
				}
				else if (Find_Squad(MatchPlayer)) {
					DA::Private_Color_Message(Player,SQUADCOLOR,"%ls is already in a squad.",MatchPlayer->Get_Name());
				}
				else if (!RemixSquads && Player->Get_Player_Type() != MatchPlayer->Get_Player_Type()) {
					DA::Private_Color_Message(Player,SQUADCOLOR,"You cannot invite players on the other team.");
				}
				else {
					Invite(MatchPlayer,Player);
				}
			}
		}
	}
	return false;
}

bool DASquadManagerClass::Accept_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	DASquadClass *Squad = Find_Squad(Player);
	if (Squad) {
		if (Squad->Is_Leader(Player)) {
			for (int i = 0;i < Joins.Count();i++) {
				if (Joins[i].Leader == Player) {
					Join_Accepted(i);
					break;
				}
			}
		}
	}
	else {
		for (int i = 0;i < Invites.Count();i++) {
			if (Invites[i].Player == Player) {
				Invite_Accepted(i);
				break;
			}
		}
	}
	return false;
}

bool DASquadManagerClass::Decline_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	DASquadClass *Squad = Find_Squad(Player);
	if (Squad) { 
		if (Squad->Is_Leader(Player)) {
			for (int i = 0;i < Joins.Count();i++) {
				if (Joins[i].Leader == Player) {
					DA::Private_Color_Message(Player,SQUADCOLOR,"You have refused %ls join request.",Make_Possessive(Joins[i].Leader->Get_Name()));
					DA::Private_Color_Message(Joins[i].Leader,SQUADCOLOR,"%ls has refused your join request.",Player->Get_Name());
					Joins.Delete(i);
					break;
				}
			}
		}
	}
	else {
		for (int i = 0;i < Invites.Count();i++) {
			if (Invites[i].Player == Player) {
				DA::Private_Color_Message(Player,SQUADCOLOR,"You have declined %ls squad invitation.",Make_Possessive(Invites[i].Leader->Get_Name()));
				DA::Private_Color_Message(Invites[i].Leader,SQUADCOLOR,"%ls has declined your squad invitation.",Player->Get_Name());
				Invites.Delete(i);
				break;
			}
		}
	}
	return false;
}

bool DASquadManagerClass::Leave_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	DASquadClass *Squad = Find_Squad(Player);
	if (Squad) {
		Squad->Remove(Player);
	}
	else if (Remove_WaitList(Player)) {
		DA::Private_Color_Message(Player,SQUADCOLOR,"You have left the squad wait list.");
	}
	else if (Remove_Join(Player)) {
		DA::Private_Color_Message(Player,SQUADCOLOR,"You have cancelled your squad join request.");
	}
	else if (Clear_Invites(Player)) {
		DA::Private_Color_Message(Player,SQUADCOLOR,"You have rescinded your squad invitations.");
	}
	return false;
}

DASquadClass *DASquadManagerClass::Create_Squad(cPlayer *Player) {
	DASquadClass *Squad = new DASquadClass(Player);
	SquadList.Add(Squad);
	return Squad;
}

void DASquadManagerClass::Invite(cPlayer *Player,cPlayer *Leader) {
	DASquadInviteStruct Struct(Leader,Player);
	Invites.Add(Struct);
	DA::Private_Color_Message(Player,SQUADCOLOR,"%ls has invited you to join their squad. Type \"!accept\" to accept, or \"!decline\" to decline.",Leader->Get_Name());
	DA::Private_Color_Message(Leader,SQUADCOLOR,"You have invited %ls to join your squad.",Player->Get_Name());
}

void DASquadManagerClass::Join_Accepted(int JoinIndex) {
	cPlayer *Player = Joins[JoinIndex].Player;
	DASquadClass *Squad = Find_Squad(Joins[JoinIndex].Leader);
	if (Squad && Squad->Is_Leader(Joins[JoinIndex].Leader) && !Find_Squad(Player)) {
		if (Squad->Is_Full()) {
			DA::Private_Color_Message(Player,SQUADCOLOR,"%ls squad is full.",Make_Possessive(Squad->Get_Leader()->Get_Name()));
			Squad->Leader_Message("Your squad is full.");
		}
		else if (Squad->Get_Team() != Player->Get_Player_Type()) {
			DA::Private_Color_Message(Player,SQUADCOLOR,"%ls squad is on the other team. You have been added to the wait list and will be teamed with them next game.",Make_Possessive(Squad->Get_Leader()->Get_Name()));
			Squad->Leader_Message("%ls is on the other team. They have been added to the wait list and will be teamed with you next game.",Player->Get_Name());
			WaitList.Add(Joins[JoinIndex]);
		}
		else {
			Squad->Add(Player);
		}
	}
	Joins.Delete(JoinIndex);
	Clear_Invites(Player);
	Clear_WaitList(Player);
}

void DASquadManagerClass::Invite_Accepted(int InviteIndex) {
	cPlayer *Player = Invites[InviteIndex].Player;
	DASquadClass *Squad = Find_Squad(Invites[InviteIndex].Leader);
	if (Squad) {
		if (Squad->Is_Full()) {
			DA::Private_Color_Message(Player,SQUADCOLOR,"%ls squad is full.",Make_Possessive(Squad->Get_Leader()->Get_Name()));
			Squad->Leader_Message("Your squad is full.");
		}
		else if (Squad->Get_Team() != Player->Get_Player_Type()) {
			DA::Private_Color_Message(Player,SQUADCOLOR,"%ls squad is on the other team. You have been added to the wait list and will be teamed with them next game.",Make_Possessive(Squad->Get_Leader()->Get_Name()));
			Squad->Leader_Message("%ls is on the other team. They have been added to the wait list and will be teamed with you next game.",Player->Get_Name());
			WaitList.Add(Invites[InviteIndex]);
		}
		else { 
			Squad->Add(Player);
		}
	}
	else if (Invites[InviteIndex].Leader->Get_Player_Type() != Player->Get_Player_Type()) {
		DA::Private_Color_Message(Player,SQUADCOLOR,"%ls is on the other team. You have been added to the wait list and will be teamed with them next game.",Invites[InviteIndex].Leader->Get_Name());
		DA::Private_Color_Message(Invites[InviteIndex].Leader,SQUADCOLOR,"%ls is on the other team. They have been added to the wait list and will be teamed with you next game.",Player->Get_Name());
		WaitList.Add(Invites[InviteIndex]);
	}
	else if (Can_Create_Squads()) {
		DASquadClass *NewSquad = Create_Squad(Invites[InviteIndex].Leader);
		NewSquad->Add(Player);
	}
	Invites.Delete(InviteIndex);
	Clear_Invites(Player);
	Clear_WaitList(Player);
}

bool DASquadManagerClass::Is_Join_Pending(cPlayer *Player,cPlayer *Leader) {
	for (int i = 0;i < Joins.Count();i++) {
		if (Joins[i].Player == Player && (!Leader || Joins[i].Leader == Leader)) {
			return true;
		}
	}
	return false;
}

bool DASquadManagerClass::Is_Invite_Pending(cPlayer *Player,cPlayer *Leader) {
	for (int i = 0;i < Invites.Count();i++) {
		if (Invites[i].Player == Player && (!Leader || Invites[i].Leader == Leader)) {
			return true;
		}
	}
	return false;
}

bool DASquadManagerClass::Is_WaitList_Pending(cPlayer *Player,cPlayer *Leader) {
	for (int i = 0;i < WaitList.Count();i++) {
		if (WaitList[i].Player == Player && (!Leader || WaitList[i].Leader == Leader)) {
			return true;
		}
	}
	return false;
}

bool DASquadManagerClass::Remove_Join(cPlayer *Player,cPlayer *Leader) {
	for (int i = 0;i < Joins.Count();i++) {
		if (Joins[i].Player == Player && (!Leader || Joins[i].Leader == Leader)) {
			Joins.Delete(i);
			return true;
		}
	}
	return false;
}

bool DASquadManagerClass::Remove_Invite(cPlayer *Player,cPlayer *Leader) {
	for (int i = 0;i < Invites.Count();i++) {
		if (Invites[i].Player == Player && (!Leader || Invites[i].Leader == Leader)) {
			Invites.Delete(i);
			return true;
		}
	}
	return false;
}

bool DASquadManagerClass::Remove_WaitList(cPlayer *Player,cPlayer *Leader) {
	for (int i = 0;i < WaitList.Count();i++) {
		if (WaitList[i].Player == Player && (!Leader || WaitList[i].Leader == Leader)) {
			WaitList.Delete(i);
			return true;
		}
	}
	return false;
}

bool DASquadManagerClass::Clear_Joins(cPlayer *Player) {
	bool Return = false;
	for (int i = Joins.Count()-1;i >= 0;i--) {
		if (Joins[i].Leader == Player) {
			Joins.Delete(i);
			Return = true;
		}
	}
	return Return;
}

bool DASquadManagerClass::Clear_Invites(cPlayer *Player) {
	bool Return = false;
	for (int i = Invites.Count()-1;i >= 0;i--) {
		if (Invites[i].Leader == Player) {
			Invites.Delete(i);
			Return = true;
		}
	}
	return Return;
}

bool DASquadManagerClass::Clear_WaitList(cPlayer *Player) {
	bool Return = false;
	for (int i = WaitList.Count()-1;i >= 0;i--) {
		if (WaitList[i].Leader == Player) {
			WaitList.Delete(i);
			Return = true;
		}
	}
	return Return;
}

void DASquadManagerClass::Clear_Lists(cPlayer *Player) {
	Remove_Join(Player);
	Remove_Invite(Player);
	Remove_WaitList(Player);
	Clear_Joins(Player);
	Clear_Invites(Player);
	Clear_WaitList(Player);
}

DASquadClass *DASquadManagerClass::Find_Squad(cPlayer *Player) {
	for (int i = 0;i < SquadList.Count();i++) {
		if (SquadList[i]->Is_Member(Player)) {
			return SquadList[i];
		}
	}
	return 0;
}

int DASquadManagerClass::Get_Max_Squad_Size() {
	int Size = (The_Game()->Get_Current_Players()/6)+1;
	if (Size < MaxSquadSize) {
		return Size;
	}
	return MaxSquadSize;
}

bool DASquadManagerClass::Can_Create_Squads() {
	return Get_Max_Squad_Size() >= 2;
}

void DASquadManagerClass::Check_Teams() {
	for (int i = SquadList.Count()-1;i >= 0;i--) {
		SquadList[i]->Check_Team();
	}
}

void DASquadManagerClass::Check_Size() {
	for (int i = SquadList.Count()-1;i >= 0;i--) {
		DASquadClass *Squad = SquadList[i];
		for (int x = Squad->Size()-1;x >= 0 && Squad->Active_Size() > Get_Max_Squad_Size();x--) {
			Squad->Get_Member(x)->Leave_Squad();
		}
	}
}

DA_API Register_Game_Feature(DASquadManagerClass,"Squad System","EnableSquads",0);

