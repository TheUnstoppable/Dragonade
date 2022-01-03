/*	Renegade Scripts.dll
    Dragonade Example Plugin
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
#include "engine.h"
#include "engine_da.h"
#include "da.h"
#include "da_crate.h"
#include "da_example_gameobjobserver.h"

/*
Like most things in DA, the crates use a factory/registrant system so you can add new ones without having to modify the crate manager code.
*/
class DAExampleCrateClass : public DACrateClass { //Inherit from DACrateClass. You can also inherit from DAEventClass for events, many of the built in crates do this.
	virtual void Init() { //This is called when the crate is loaded.
		DACrateClass::Init(); //Make sure to call this if you're overloading this function.
	}
	~DAExampleCrateClass() { //If you need to do something when the crate is unloaded.
	}
	virtual void Settings_Loaded() { //This is called when settings are reloaded.
		DACrateClass::Settings_Loaded(); //Make sure to call this if you're overloading this function.
	}
	virtual bool Can_Activate(cPlayer *Player) { //This is called whenever the crate odds are calculated to see if the player can pick up the crate.
		//You can check if the player already has this crate's effect.
		if (Player->Get_GameObj()->Get_Vehicle()?Player->Get_GameObj()->Get_Vehicle()->Find_Observer("DAExampleGameObjObserverClass"):Player->Get_GameObj()->Find_Observer("DAExampleGameObjObserverClass")) {
			return false;
		}
		if (Player->Get_GameObj()->Is_Stealth_Enabled()) { //Check if the player is something you don't want to get this crate.
			return false;
		}
		if (!Find_Base_Defense(!Player->Get_Player_Type())) { //Or check for any other scenario you wouldn't want the crate to trigger in.
			return false;
		}
		return true;
	}
	virtual void Activate(cPlayer *Player) { //Called when the crate is selected and given to a player.
		if (Player->Get_GameObj()->Get_Vehicle()) { //If the crate can be picked up by both infantry and vehicles remember to check if they're in a vehicle and apply the effect appropriately.
			Player->Get_GameObj()->Get_Vehicle()->Add_Observer(new DAExampleGameObjObserverClass);
			DA::Page_Player(Player,"Your vehicle is now an example!");
		}
		else {
			Player->Get_GameObj()->Add_Observer(new DAExampleGameObjObserverClass);
			DA::Page_Player(Player,"Your character is now an example!");
		}
	}
};
/*
Register the crate. 
The first parameter is the name of the crate. The settings for the crate will be loaded from a section called "[<Name>_Crate]".
The second parameter is what can pick up the crate. Either infantry, vehicles, or both.
*/
Register_Crate(DAExampleCrateClass,"Example",DACrateType::INFANTRY | DACrateType::VEHICLE);
/*
Register_Crate is a macro that produces the following code:
DACrateRegistrant<DAExampleCrateClass> DAExampleCrateClassRegistrant("Example",DACrateType::INFANTRY | DACrateType::VEHICLE);

It's possible to overwrite existing crates by simply registering your crate with the same name.
The following would replace the veteran crate with our example crate.
Register_Crate(DAExampleCrateClass,"Veteran",DACrateType::INFANTRY | DACrateType::VEHICLE);
*/


/*
Next we have crate modifiers. These allow you to apply modifiers to a crate which will change its odds under certain circumstances.
*/
class DAExampleCrateModifierClass : public DACrateModifierClass { //Inherit from DACrateModifierClass.
	virtual void Init(const char *Parameters) { //This is called when the modifier is loaded. You only need to overload it if your modifier has custom settings.
		DATokenParserClass Parser(Parameters,'|');
		DACrateModifierClass::Init(Parser.Get_String()); //The first part of the setting should be the operator and value. Pass it to the default Init.
		Parser.Get_Int(Players); //Store our custom setting.
	}

	virtual void Calculate_Odds(float &Odds,cPlayer *Player) { //This is called when the crate odds are calculated.
		if (The_Game()->Get_Current_Players() >= Players) { //Check if our modifier should be applied.
			Apply_Modifier(Odds); //Apply it. If you want you can manually modify Odds, but DACrateModifierClass will handle the default type of operation.
		}
	}

	int Players;
};
/*
Register the modifier.
The only parameter is the name. It is used in the settings to add it to a crate.
*/
Register_Crate_Modifier(DAExampleCrateModifierClass,"ExampleModifier");
/*
Register_Crate_Modifier is a macro that produces the following code:
DACrateModifierRegistrant<DAExampleCrateModifierClass> DAExampleCrateModifierClassRegistrant("ExampleModifier");

It's possible to overwrite existing modifiers by simply registering your modifier with the same name.
The following would replace the TimeAfter modifier with our example modifier.
Register_Crate_Modifier(DAExampleCrateModifierClass,"TimeAfter");
*/
