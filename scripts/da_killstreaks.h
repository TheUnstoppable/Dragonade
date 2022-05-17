#pragma once

#include "da_event.h"
#include "da_gamefeature.h"
#include "da_player.h"

class DA_API DAKillStreaksPlayerDataClass : public DAPlayerDataClass {
	friend class DAKillStreaksGameFeatureClass;

public:
	virtual const char* Get_Observer_Name() {
		return "DAKillStreaksObserverClass";
	}

	int Increment_Streak() { return ++Streak; }
	int Get_Streak() { return Streak; }
	void Reset_Streak() { Streak = 0; }

	virtual void Init() { Reset_Streak(); }
	virtual void Clear_Level() { Reset_Streak(); }
	virtual void Clear_Session()  { Reset_Streak(); }
private:
	int Streak;
};

class DA_API DAKillStreaksGameFeatureClass : public DAEventClass, public DAGameFeatureClass, public DAPlayerDataManagerClass<DAKillStreaksPlayerDataClass> {
public:
	virtual void Init();
	virtual ~DAKillStreaksGameFeatureClass();
	virtual void Settings_Loaded_Event();
	virtual void Player_Leave_Event(cPlayer* Player);
	virtual void Timer_Expired(int Number, unsigned int Data);
	virtual void Kill_Event(DamageableGameObj* Victim, ArmedGameObj* Killer, float Damage, unsigned int Warhead, float Scale, DADamageType::Type Type);

	void Add_Streak(int KillCount, StringClass Sound, unsigned int Red, unsigned int Green, unsigned int Blue, StringClass Message);
	bool Streak_Exists(int KillCount);
	void Delete_Streak(int KillCount);
	void Clear_Streaks();

private:
	struct DAKillStreak {
		int Count;
		StringClass Sound;
		int Red;
		int Green;
		int Blue;
		StringClass Message;
	};

	SList<DAKillStreak> Streaks;
	bool DeathResetsStreak;
	bool KillRestartsTimer;
	float StreakResetTime;
};