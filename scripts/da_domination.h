/*	Renegade Scripts.dll
    Dragonade Domination Game Mode
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DADOMINATION
#define INCLUDE_DADOMINATION

#include "da_event.h"
#include "da_gamemode.h"
#include "da_spawnsystem.h"
#include "da_nodemanager.h"
#include "da_vehiclespawn.h"
#include "da_teleporter.h"

#define ControlNodeType "Control"
#define TiberiumNodeType "Tiberium"

class DAControlNodeClass : public DASpawnNodeClass {
public:
	virtual void Timer_Expired(int Number,unsigned int Data);
	virtual void Capture_Event();
	virtual void Contested_Event();
	virtual const char *Get_Type() const {
		return ControlNodeType;
	}
protected:
	virtual void Init(const INIClass *INI,const StringClass &Header);
};

class DATiberiumNodeClass : public DABaseNodeClass {
public:
	virtual void Timer_Expired(int Number,unsigned int Data);
	virtual void Capture_Event();
	virtual void Contested_Event();
	virtual const char *Get_Type() const {
		return TiberiumNodeType;
	}
protected:
	virtual void Init(const INIClass *INI,const StringClass &Header);
};

class DADominationManagerClass : public DAEventClass, public DAGameModeClass {
public:
	inline void Increment_Score(int Team) {
		Score[Team] += PointTickAmount;
		Update_Score(Team);

		if (Score[Team] >= WinningPoints) {
			Set_Winner(Team);
		}
	}
	inline void Increment_Credits(int Team) {
		Give_Credits_Team(Team,CreditTickAmount);
	}
	void Set_Winner(int Winner);
	inline const StringClass &Get_Control_Node_Preset() const {
		return ControlNodePreset;
	}
	inline const StringClass &Get_Control_Node_Model() const {
		return ControlNodeModel;
	}
	inline const StringClass &Get_Control_Node_Animation() const {
		return ControlNodeAnimation;
	}
	inline const StringClass &Get_Tiberium_Node_Preset() const {
		return TiberiumNodePreset;
	}
	inline const StringClass &Get_Tiberium_Node_Model() const {
		return TiberiumNodeModel;
	}
	inline const StringClass &Get_Tiberium_Node_Animation() const {
		return TiberiumNodeAnimation;
	}
	inline float Get_Point_Tick_Amount() const {
		return PointTickAmount;
	}
	inline float Get_Point_Tick_Time() const {
		return PointTickTime;
	}
	inline float Get_Credit_Tick_Amount() const {
		return CreditTickAmount;
	}
	inline float Get_Credit_Tick_Time() const {
		return CreditTickTime;
	}
	void Update_Node_Count();
	inline void Update_Score(int Team) { 
		Find_Team(Team)->Set_Score(Score[Team]);
	}

private:
	virtual void Init();
	virtual void Settings_Loaded_Event();
	virtual void Player_Loaded_Event(cPlayer *Player);
	virtual void Object_Created_Event(GameObject *obj);
	virtual void Timer_Expired(int Number,unsigned int Data);
	void Send_Info_Message(int ID);
	bool Nodes_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);

	virtual ~DADominationManagerClass() {
		DANodeManager->Destroy_Instance();
		DATeleporterManager->Destroy_Instance();
		DAVehicleSpawnManager->Destroy_Instance();
		DASpawnManager->Destroy_Instance();
	}

	float Score[2];

	//Settings
	StringClass ControlNodePreset;
	StringClass ControlNodeModel;
	StringClass ControlNodeAnimation;
	StringClass TiberiumNodePreset;
	StringClass TiberiumNodeModel;
	StringClass TiberiumNodeAnimation;
	float PointTickAmount;
	float PointTickTime;
	float CreditTickAmount;
	float CreditTickTime;
	float WinningPoints;
};

extern DAGameModeRegistrant<DADominationManagerClass> DADominationManagerClassRegistrant;
#define DADominationManager ((DADominationManagerClass*)DADominationManagerClassRegistrant.Get_Instance())
#endif