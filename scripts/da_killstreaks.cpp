#include "General.h"
#include "engine.h"
#include "engine_da.h"
#include "da.h"
#include "da_settings.h"
#include "da_killstreaks.h"

void DAKillStreaksGameFeatureClass::Init() {
	DAPlayerDataManagerClass<DAKillStreaksPlayerDataClass>::Init();
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Event(DAEvent::PLAYERLEAVE);
	Register_Object_Event(DAObjectEvent::KILLRECEIVED, DAObjectEvent::PLAYER);
}

DAKillStreaksGameFeatureClass::~DAKillStreaksGameFeatureClass() {
	Clear_Streaks();
}

void DAKillStreaksGameFeatureClass::Settings_Loaded_Event() {
	Clear_Streaks();
	DeathResetsStreak = DASettingsManager::Get_Bool("DeathResetsStreak", true);
	KillRestartsTimer = DASettingsManager::Get_Bool("KillRestartsTimer", true);
	StreakResetTime = DASettingsManager::Get_Float("StreakResetTime", 20.f);
	INISection* StreaksSection = DASettingsManager::Get_Section("KillStreaks");
	if (StreaksSection) {
		for (INIEntry* Entry = StreaksSection->EntryList.First(); Entry && Entry->Is_Valid(); Entry = Entry->Next()) {
			DAKillStreak* Streak = new DAKillStreak;
			DATokenParserClass Token(Entry->Value, '|');
			Streak->Count = atoi(Entry->Entry);
			Streak->Sound = Token.Get_String();
			DATokenParserClass ColorTok(Token.Get_String(), ',');
			if (!(ColorTok.Get_Int(Streak->Red) && ColorTok.Get_Int(Streak->Green) && ColorTok.Get_Int(Streak->Blue))) {
				continue;
			}
			Streak->Message = Token.Get_Remaining_String();
			Streaks.Add_Tail(Streak);
		}
	}
}

void DAKillStreaksGameFeatureClass::Player_Leave_Event(cPlayer* Player) {
	Stop_Timer(121, Player->Get_Id());
}

void DAKillStreaksGameFeatureClass::Timer_Expired(int Number, unsigned int Data) {
	if (Number == 121) {
		DAKillStreaksPlayerDataClass* PlayerData = Get_Player_Data(Data);
		PlayerData->Reset_Streak();
	}
}

void DAKillStreaksGameFeatureClass::Kill_Event(DamageableGameObj* Victim, ArmedGameObj* Killer, float Damage, unsigned int Warhead, float Scale, DADamageType::Type Type) {
	if (Killer == Victim) {
		return;
	}
	if (!Commands->Is_A_Star(Killer)) {
		return;
	}
	DAKillStreaksPlayerDataClass* Data = 0;
	if (Killer->As_VehicleGameObj()) {
		if (SoldierGameObj* Gunner = ((VehicleGameObj*)Killer)->Get_Gunner()) {
			Data = Get_Player_Data(Gunner);
		}
		else if (SoldierGameObj* Driver = ((VehicleGameObj*)Killer)->Get_Driver()) {
			Data = Get_Player_Data(Driver);
		}
	}
	else if (Killer->As_SoldierGameObj()) {
		Data = Get_Player_Data(Killer);
	}
	if (Data) {
		int streakCount = Data->Increment_Streak();
		for (SLNode<DAKillStreak>* z = Streaks.Head(); z; z = z->Next()) {
			DAKillStreak* streak = z->Data();
			if (streak->Count == streakCount) {
				StringClass string(streak->Message);
				string.Replace("{PLAYER}", StringClass(Data->Get_Name()));
				DA::Create_2D_Sound(streak->Sound);
				DA::Color_Message(streak->Red, streak->Green, streak->Blue, string);
				break;
			}
		}
		if (KillRestartsTimer) {
			Stop_Timer(121, Data->Get_ID());
			Start_Timer(121, StreakResetTime, false, Data->Get_ID());
		}
		else if (!Is_Timer(121, Data->Get_ID())) {
			Start_Timer(121, StreakResetTime, false, Data->Get_ID());
		}
	}
	if (DeathResetsStreak) {
		DAKillStreaksPlayerDataClass* Data = Get_Player_Data(Victim);
		Data->Reset_Streak();
		Stop_Timer(121, Data->Get_ID());
	}
}

void DAKillStreaksGameFeatureClass::Add_Streak(int KillCount, StringClass Sound, unsigned int Red, unsigned int Green, unsigned int Blue, StringClass Message) {
	Delete_Streak(KillCount);
	DAKillStreak* Streak = new DAKillStreak;
	Streak->Count = KillCount;
	Streak->Sound = Sound;
	Streak->Red = Red;
	Streak->Green = Green;
	Streak->Blue = Blue;
	Streak->Message = Message;
	Streaks.Add_Tail(Streak);
}

bool DAKillStreaksGameFeatureClass::Streak_Exists(int KillCount) {
	for (SLNode<DAKillStreak>* z = Streaks.Head(); z; z = z->Next()) {
		if (z->Data()->Count == KillCount) {
			return true;
		}
	}
	return false;
}

void DAKillStreaksGameFeatureClass::Delete_Streak(int KillCount) {
	for (SLNode<DAKillStreak>* z = Streaks.Head(); z; z = z->Next()) {
		if (z->Data()->Count == KillCount) {
			Streaks.Remove(z->Data());
			delete z->Data();
			return;
		}
	}
}

void DAKillStreaksGameFeatureClass::Clear_Streaks() {
	for (SLNode<DAKillStreak>* z = Streaks.Head(); z; z = z->Next()) {
		delete z->Data();
	}
	Streaks.Remove_All();
}

Register_Game_Feature(DAKillStreaksGameFeatureClass, "Kill Streaks", "EnableKillStreaks", 0);