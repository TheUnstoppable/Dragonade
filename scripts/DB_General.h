/*	Renegade Scripts.dll
	Copyright 2016 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#pragma once

static HashTemplateClass<uint,uint> UpgradeComplete;

enum Research_Technology
{
	UPGRADE_INFANTRY_ARMOR_TIMER_NUMBER = 600,
	UPGRADE_INFANTRY_ARMOR_TIME = 10,
	UPGRADE_INFANTRY_ARMOR_COST = 5000,
	UPGRADE_INFANTRY_ARMOR_AMOUNT = 10,
	UPGRADE_INFANTRY_ARMOR_CUSTOM = 70008001,
};

#define REATTACH_BONES_CUSTOM 9320943
#define DEPLOY_BEGIN_CUSTOM 9320944
#define DEPLOY_COMPLETE_CUSTOM 9320945
#define UNDEPLOY_BEGIN_CUSTOM 9320946
#define UNDEPLOY_COMPLETE_CUSTOM 9320947
#define BOMBARD_TOGGLE 9320948
