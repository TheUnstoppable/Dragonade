/*	Renegade Scripts.dll
    Dragonade Crate Classes
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
#include "da.h"
#include "da_crate.h"
#include "da_cratemanager.h"
#include "da_settings.h"
#include "engine_da.h"
#include "SoldierGameObj.h"

/********************************************************************************************************************************/

DACrateModifierFactoryClass::DACrateModifierFactoryClass(const char *Name) {
	this->Name = Name;
	DACrateManager::Add_Crate_Modifier(this);
}

void DACrateModifierClass::Init(const char *Parameters) {
	if (Parameters[0] == '+') {
		Operator = DACrateModifierOperator::ADD;
	}
	else if (Parameters[0] == '-') {
		Operator = DACrateModifierOperator::SUBTRACT;
	}
	else if (Parameters[0] == '*') {
		Operator = DACrateModifierOperator::MULTIPLY;
	}
	else if (Parameters[0] == '/') {
		Operator = DACrateModifierOperator::DIVIDE;
	}
	else {
		Operator = DACrateModifierOperator::NONE;
	}
	Value = (float)atof(Parameters+1);
}

void DACrateModifierClass::Apply_Modifier(float &Odds) {
	if (Operator == DACrateModifierOperator::ADD) {
		Odds += Value;
	}
	else if (Operator == DACrateModifierOperator::SUBTRACT) {
		Odds -= Value;
	}
	else if (Operator == DACrateModifierOperator::MULTIPLY) {
		Odds *= Value;
	}
	else if (Operator == DACrateModifierOperator::DIVIDE) {
		Odds /= Value;
	}
}

bool DACrateModifierClass::Is_Multiplicative() {
	return (Operator == DACrateModifierOperator::MULTIPLY || Operator == DACrateModifierOperator::DIVIDE);
}

bool DACrateModifierClass::Is_Positive() {
	return ((Operator == DACrateModifierOperator::MULTIPLY && Value > 1) || (Operator == DACrateModifierOperator::ADD && Value > 0) || (Operator == DACrateModifierOperator::DIVIDE && Value < 1 && Value > 0) || (Operator == DACrateModifierOperator::SUBTRACT && Value < 0));
}

/********************************************************************************************************************************/

DACrateFactoryClass::DACrateFactoryClass(const char *Name,DACrateType::Type Type) {
	this->Name = Name;
	this->Type = Type;
	Section.Format("%s_Crate",Name);
	for (int i = 0;i < Section.Get_Length();i++) {
		if (Section[i] == ' ') {
			Section[i] = '_';
		}
	}
	Instance = 0;
	DACrateManager::Add_Crate(this);
}

void DACrateFactoryClass::Destroy_Instance() {
	delete Instance;
	Instance = 0;
}

bool DACrateFactoryClass::Check_Enabled() const {
	return (DASettingsManager::Get_Float(Section,"Odds",0) > 0 && Get_Enabled_Type());
}

bool DACrateFactoryClass::Check_Type(DACrateType::Type Type) const {
	return (this->Type & Type) == Type;
}

bool DACrateFactoryClass::Check_Enabled_Type(DACrateType::Type Type) const {
	return (Get_Enabled_Type() & Type) == Type;
}

DACrateType::Type DACrateFactoryClass::Get_Enabled_Type() const { //Modifies and returns the type based on settings.
	DACrateType::Type Return = Type;
	if (Check_Type(DACrateType::INFANTRY) && (!DASettingsManager::Get_Bool("EnableInfantryCrates",true) || DASettingsManager::Get_Bool(Get_Section(),"DisableInfantry",false))) {
		Return &= ~DACrateType::INFANTRY;
	}
	if (Check_Type(DACrateType::VEHICLE) && (!DASettingsManager::Get_Bool("EnableVehicleCrates",true) || DASettingsManager::Get_Bool(Get_Section(),"DisableVehicle",false))) {
		Return &= ~DACrateType::VEHICLE;
	}
	return Return;
}

void DACrateClass::Init() {
	BaseOdds = 0;
	ModifiedOdds = 0;
	FinalOdds = 0;
	Type = (DACrateType::Type)0;
}

void DACrateClass::Settings_Loaded() {
	//Odds
	CapOdds = DASettingsManager::Get_Bool(Get_Section(),"CapOdds",false);
	BaseOdds = DASettingsManager::Get_Float(Get_Section(),"Odds",0);
	ModifiedOdds = 0;
	FinalOdds = 0;
	
	//Modifiers
	for (int i = 0;i < Modifiers.Count();i++) {
		delete Modifiers[i];
	}
	Modifiers.Delete_All();
	INISection *Section = DASettingsManager::Get_Section(Get_Section());
	if (Section) {
		for (INIEntry *i = Section->EntryList.First();i && i->Is_Valid();i = i->Next()) {
			if (_stricmp(i->Entry,"Enable") && _stricmp(i->Entry,"Odds")) {
				DACrateModifierClass *Mod = DACrateManager::Create_Crate_Modifier(i->Entry,i->Value);
				if (Mod) {
					if (Mod->Is_Multiplicative()) { //Apply additive modifiers before multiplicative ones.
						Modifiers.Add(Mod);
					}
					else {
						Modifiers.Add_Head(Mod);
					}
				}
			}
		}
	}

	//Type
	Type = Factory->Get_Enabled_Type();
}

void DACrateClass::Calculate_Odds(cPlayer *Player) {
	if (Can_Activate(Player)) {
		ModifiedOdds = BaseOdds;
		for (int i = 0;i < Modifiers.Count();i++) {
			if (!CapOdds || !Modifiers[i]->Is_Positive()) {
				Modifiers[i]->Calculate_Odds(ModifiedOdds,Player);
			}
		}
		if (ModifiedOdds < 0) {
			ModifiedOdds = 0;
		}
		FinalOdds = ModifiedOdds;
	}
	else {
		FinalOdds = ModifiedOdds = 0;
	}
}

bool DACrateClass::Check_Type(cPlayer *Player) const {
	if (Player->Get_GameObj()->Get_Vehicle()) {
		return Check_Type(DACrateType::VEHICLE);
	}
	return Check_Type(DACrateType::INFANTRY);
}

bool DACrateClass::Check_Type(DACrateType::Type Type) const {
	return (this->Type & Type) == Type;
}

void DACrateClass::Adjust_Odds(float Odds) {
	FinalOdds += Odds;
	if (FinalOdds < 0) {
		FinalOdds = 0;
	}
}
