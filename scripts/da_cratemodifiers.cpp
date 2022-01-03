/*	Renegade Scripts.dll
    Dragonade Crate Modifiers
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
#include "engine_da.h"
#include "da.h"
#include "da_crate.h"
#include "SoldierGameObj.h"
#include "VehicleGameObj.h"
#include "GameObjManager.h"

/********************************************************************************************************************************/

class DAWinningCrateModifierClass : public DACrateModifierClass {
	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		if (Get_Building_Count(Player->Get_Player_Type(),false) > Get_Building_Count(Player->Get_Player_Type()?0:1,false)) { //Apply if player's team has more living buildings.
			Apply_Modifier(Odds);
		}
	}
};

Register_Crate_Modifier(DAWinningCrateModifierClass,"Winning");

/********************************************************************************************************************************/

class DALosingCrateModifierClass : public DACrateModifierClass {
	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		if (Get_Building_Count(Player->Get_Player_Type(),false) < Get_Building_Count(Player->Get_Player_Type()?0:1,false)) { //Apply if player's team has less living buildings.
			Apply_Modifier(Odds);
		}
	}
};

Register_Crate_Modifier(DALosingCrateModifierClass,"Losing");

/********************************************************************************************************************************/

class DATimeAfterCrateModifierClass : public DACrateModifierClass {
	virtual void Init(const char *Parameters) {
		DATokenParserClass Parser(Parameters,'|');
		DACrateModifierClass::Init(Parser.Get_String()); //Store operator and value.
		Parser.Get_UInt(Time); //Store time.
		Time *= 60;
	}

	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		if (The_Game()->Get_Game_Duration_S() >= Time) { //Apply once if more than Time seconds have elapsed.
			Apply_Modifier(Odds);
		}
	}

	unsigned int Time;
};

Register_Crate_Modifier(DATimeAfterCrateModifierClass,"TimeAfter");

/********************************************************************************************************************************/

class DATimeBeforeCrateModifierClass : public DACrateModifierClass {
	virtual void Init(const char *Parameters) {
		DATokenParserClass Parser(Parameters,'|');
		DACrateModifierClass::Init(Parser.Get_String()); //Store operator and value.
		Parser.Get_UInt(Time); //Store time.
		Time *= 60;
	}

	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		if (The_Game()->Get_Game_Duration_S() < Time) { //Apply once if less than Time seconds have elapsed.
			Apply_Modifier(Odds);
		}
	}

	unsigned int Time;
};

Register_Crate_Modifier(DATimeBeforeCrateModifierClass,"TimeBefore");

/********************************************************************************************************************************/

class DATimeEveryCrateModifierClass : public DACrateModifierClass {
	virtual void Init(const char *Parameters) {
		DATokenParserClass Parser(Parameters,'|');
		DACrateModifierClass::Init(Parser.Get_String()); //Store operator and value.
		Parser.Get_UInt(Time); //Store time.
		Time *= 60;
	}

	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		unsigned int Times = The_Game()->Get_Game_Duration_S()/Time; //Apply once for every Time seconds that have elapsed.
		for (unsigned int i = 0;i < Times;i++) {
			Apply_Modifier(Odds);
		}
	}

	unsigned int Time;
};

Register_Crate_Modifier(DATimeEveryCrateModifierClass,"TimeEvery");

/********************************************************************************************************************************/

class DAScoreGreaterCrateModifierClass : public DACrateModifierClass {
	virtual void Init(const char *Parameters) {
		DATokenParserClass Parser(Parameters,'|');
		DACrateModifierClass::Init(Parser.Get_String()); //Store operator and value.
		Parser.Get_Float(Score); //Store score.
	}

	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		if (Player->Get_Score() >= Score) { //Apply if player's score is greater than or equal to.
			Apply_Modifier(Odds);
		}
	}

	float Score;
};

Register_Crate_Modifier(DAScoreGreaterCrateModifierClass,"ScoreGreater");

/********************************************************************************************************************************/

class DAScoreLesserCrateModifierClass : public DACrateModifierClass {
	virtual void Init(const char *Parameters) {
		DATokenParserClass Parser(Parameters,'|');
		DACrateModifierClass::Init(Parser.Get_String()); //Store operator and value.
		Parser.Get_Float(Score); //Store score.
	}

	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		if (Player->Get_Score() < Score) { //Apply if player's score is lesser than.
			Apply_Modifier(Odds);
		}
	}

	float Score;
};

Register_Crate_Modifier(DAScoreLesserCrateModifierClass,"ScoreLesser");

/********************************************************************************************************************************/

class DACreditsGreaterCrateModifierClass : public DACrateModifierClass {
	virtual void Init(const char *Parameters) {
		DATokenParserClass Parser(Parameters,'|');
		DACrateModifierClass::Init(Parser.Get_String()); //Store operator and value.
		Parser.Get_Float(Credits); //Store credits.
	}

	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		if (Player->Get_Money() >= Credits) { //Apply if player has more or equal credits.
			Apply_Modifier(Odds);
		}
	}

	float Credits;
};

Register_Crate_Modifier(DACreditsGreaterCrateModifierClass,"CreditsGreater");

/********************************************************************************************************************************/

class DACreditsLesserCrateModifierClass : public DACrateModifierClass {
	virtual void Init(const char *Parameters) {
		DATokenParserClass Parser(Parameters,'|');
		DACrateModifierClass::Init(Parser.Get_String()); //Store operator and value.
		Parser.Get_Float(Credits); //Store credits.
	}

	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		if (Player->Get_Money() < Credits) { //Apply if player has less credits.
			Apply_Modifier(Odds);
		}
	}

	float Credits;
};

Register_Crate_Modifier(DACreditsLesserCrateModifierClass,"CreditsLesser");

/********************************************************************************************************************************/

class DABaseDefenseDestroyedCrateModifierClass : public DACrateModifierClass {
	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		BuildingGameObj *Building = (BuildingGameObj*)Find_Base_Defense(Player->Get_Player_Type());
		if (Building && Building->Is_Destroyed()) { //Apply if base defense is dead.
			Apply_Modifier(Odds);
		}
	}
};

Register_Crate_Modifier(DABaseDefenseDestroyedCrateModifierClass,"BaseDefenseDestroyed");

/********************************************************************************************************************************/

class DAPowerPlantDestroyedCrateModifierClass : public DACrateModifierClass {
	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		BuildingGameObj *Building = (BuildingGameObj*)Find_Power_Plant(Player->Get_Player_Type());
		if (Building && Building->Is_Destroyed()) { //Apply if power plant is dead.
			Apply_Modifier(Odds);
		}
	}
};

Register_Crate_Modifier(DAPowerPlantDestroyedCrateModifierClass,"PowerPlantDestroyed");

/********************************************************************************************************************************/

class DARefineryDestroyedCrateModifierClass : public DACrateModifierClass {
	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		BuildingGameObj *Building = (BuildingGameObj*)Find_Refinery(Player->Get_Player_Type());
		if (Building && Building->Is_Destroyed()) { //Apply if refinery is dead.
			Apply_Modifier(Odds);
		}
	}
};

Register_Crate_Modifier(DARefineryDestroyedCrateModifierClass,"RefineryDestroyed");

/********************************************************************************************************************************/

class DASoldierFactoryDestroyedCrateModifierClass : public DACrateModifierClass {
	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		BuildingGameObj *Building = (BuildingGameObj*)Find_Soldier_Factory(Player->Get_Player_Type());
		if (Building && Building->Is_Destroyed()) { //Apply if soldier factory is dead.
			Apply_Modifier(Odds);
		}
	}
};

Register_Crate_Modifier(DASoldierFactoryDestroyedCrateModifierClass,"SoldierFactoryDestroyed");

/********************************************************************************************************************************/

class DAVehicleFactoryDestroyedCrateModifierClass : public DACrateModifierClass {
	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		BuildingGameObj *Building = (BuildingGameObj*)Find_Vehicle_Factory(Player->Get_Player_Type());
		if (Building && Building->Is_Destroyed()) { //Apply if vehicle factory is dead.
			Apply_Modifier(Odds);
		}
	}
};

Register_Crate_Modifier(DAVehicleFactoryDestroyedCrateModifierClass,"VehicleFactoryDestroyed");

/********************************************************************************************************************************/

class DAAirFactoryDestroyedCrateModifierClass : public DACrateModifierClass {
	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		BuildingGameObj *Building = (BuildingGameObj*)Find_Helipad(Player->Get_Player_Type());
		if (Building && Building->Is_Destroyed()) { //Apply if air factory is dead.
			Apply_Modifier(Odds);
		}
	}
};

Register_Crate_Modifier(DAAirFactoryDestroyedCrateModifierClass,"AirFactoryDestroyed");

/********************************************************************************************************************************/

class DANavalFactoryDestroyedCrateModifierClass : public DACrateModifierClass {
	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		BuildingGameObj *Building = (BuildingGameObj*)Find_Naval_Factory(Player->Get_Player_Type());
		if (Building && Building->Is_Destroyed()) { //Apply if vehicle factory is dead.
			Apply_Modifier(Odds);
		}
	}
};

Register_Crate_Modifier(DANavalFactoryDestroyedCrateModifierClass,"NavalFactoryDestroyed");

/********************************************************************************************************************************/

class DARepairBayDestroyedCrateModifierClass : public DACrateModifierClass {
	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		BuildingGameObj *Building = (BuildingGameObj*)Find_Repair_Bay(Player->Get_Player_Type());
		if (Building && Building->Is_Destroyed()) { //Apply if repair bay is dead.
			Apply_Modifier(Odds);
		}
	}
};

Register_Crate_Modifier(DARepairBayDestroyedCrateModifierClass,"RepairBayDestroyed");

/********************************************************************************************************************************/

class DACommCenterDestroyedCrateModifierClass : public DACrateModifierClass {
	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		BuildingGameObj *Building = (BuildingGameObj*)Find_Com_Center(Player->Get_Player_Type());
		if (Building && Building->Is_Destroyed()) { //Apply if repair bay is dead.
			Apply_Modifier(Odds);
		}
	}
};

Register_Crate_Modifier(DACommCenterDestroyedCrateModifierClass,"CommCenterDestroyed");

/********************************************************************************************************************************/

class DAFactoriesDestroyedCrateModifierClass : public DACrateModifierClass {
	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		for (SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();x;x = x->Next()) {
			BuildingGameObj *Building = x->Data();
			if (Building->Get_Player_Type() == Player->Get_Player_Type() && !Building->Is_Destroyed()) { //On their team and alive.
				if (Building->As_SoldierFactoryGameObj() || Building->As_VehicleFactoryGameObj() || Building->As_NavalFactoryGameObj() || Building->As_AirFactoryGameObj()) {
					return;
				}
			}
		}
		Apply_Modifier(Odds); //Apply modifier if no production buildings are alive.
	}
};

Register_Crate_Modifier(DAFactoriesDestroyedCrateModifierClass,"FactoriesDestroyed");

/********************************************************************************************************************************/

class DAStealthCrateModifierClass : public DACrateModifierClass {
	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		if (Player->Get_GameObj()->Get_Vehicle()) {
			if (Player->Get_GameObj()->Get_Vehicle()->Is_Stealth_Enabled()) {
				Apply_Modifier(Odds); //Apply modifier if vehicle is stealth.
			}
		}
		else if (Player->Get_GameObj()->Is_Stealth_Enabled()) {
			Apply_Modifier(Odds); //Apply modifier if character is stealth.
		}
	}
};

Register_Crate_Modifier(DAStealthCrateModifierClass,"Stealth");

/********************************************************************************************************************************/

class DAInfantryCrateModifierClass : public DACrateModifierClass {
	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		if (!Player->Get_GameObj()->Get_Vehicle()) {
			Apply_Modifier(Odds); //Apply modifier if infantry.
		}
	}
};

Register_Crate_Modifier(DAInfantryCrateModifierClass,"Infantry");

/********************************************************************************************************************************/

class DAVehicleCrateModifierClass : public DACrateModifierClass {
	virtual void Calculate_Odds(float &Odds,cPlayer *Player) {
		if (Player->Get_GameObj()->Get_Vehicle()) {
			Apply_Modifier(Odds); //Apply modifier if in a vehicle.
		}
	}
};

Register_Crate_Modifier(DAVehicleCrateModifierClass,"Vehicle");

/********************************************************************************************************************************/
