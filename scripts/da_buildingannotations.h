/*	Renegade Scripts.dll
	Dragonade Building Annotations Game Feature
	Copyright 2022 Unstoppable, Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DABUILDINGWARNINGS
#define INCLUDE_DABUILDINGWARNINGS

#include "da_gamefeature.h"
#include "da_event.h"

class DABuildingAnnotationsGameFeatureClass : public DAEventClass, public DAGameFeatureClass {
protected:
	void Init() override;
	void Settings_Loaded_Event() override;
	void Damage_Event(DamageableGameObj* Victim, ArmedGameObj* Damager, float Damage, unsigned int Warhead, float Scale, DADamageType::Type Type) override;
	void Timer_Expired(int Number, unsigned int Data) override;
private:
	static const char Sounds[4][2][50];

	DynamicVectorClass<StringClass> IncludeList;
	DynamicVectorClass<StringClass> ExcludeList;
	bool Enable75;
	bool Enable50;
	bool Enable25;
	bool EnableRepairing;

	bool Announce75[2];
	bool Announce50[2];
	bool Announce25[2];
	bool AnnounceRepairing[2];
};

#endif