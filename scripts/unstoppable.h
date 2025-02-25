/*	Renegade Scripts.dll
	Copyright 2022 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#pragma once
#include "scripts.h"

/*!
 * \brief Teleport
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Teleports enterer to specified object.
 *
 * \param ArrowID
 *	 Object to teleport enterer.
 * \param SoundName
 *   Name of sound file to play after teleportation.
 */
class UP_Teleport : public ScriptImpClass
{
public:
	void Entered(GameObject* obj, GameObject* enterer);
};

/*!
 * \brief HUD Message on Zone Enter
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Displays an HUD message when a player enters the zone.
 *
 * \param Message
 *   Message to send to enterer player.
 * \param SoundName
 *   Name of sound file to play to player.
 * \param Red
 *   Red tone of the message.
 * \param Green
 *   Green tone of the message.
 * \param Blue
 *   Blue tone of the message.
 */
class UP_HUD_Message_Zone_Enter : public ScriptImpClass
{
public:
	void Entered(GameObject* obj, GameObject* enterer) override;
};


/*!
 * \brief HUD Message on Zone Exit
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Displays an HUD message when a player exits the zone.
 *
 * \param Message
 *   Message to send to exiter player.
 * \param SoundName
 *   Name of sound file to play to player.
 * \param Red
 *   Red tone of the message.
 * \param Green
 *   Green tone of the message.
 * \param Blue
 *   Blue tone of the message.
 */
class UP_HUD_Message_Zone_Exit : public ScriptImpClass
{
public:
	void Exited(GameObject* obj, GameObject* exiter) override;
};

/*!
 * \brief Stop Vehicle Engine
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Freezes and disables vehicle engine for a duration.
 *
 * \param Duration
 *   Amount of time to keep vehicle engine disabled.
 */
class UP_Stop_Vehicle_Engine : public ScriptImpClass
{
public:
	void Created(GameObject* obj);
	void Custom(GameObject* obj, int type, int param, GameObject* sender);
	void Timer_Expired(GameObject* obj, int number);

private:
	ReferencerClass Vehicle;
};

/*!
 * \brief Change Vehicle Weapon
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Changes vehicle weapon, and detaches.
 *
 * \param Preset
 *   Name of weapon preset to give.
 * \param Rounds
 *   Amount of rounds to give.
 */
class UP_Change_Vehicle_Weapon : public ScriptImpClass
{
public:
	void Created(GameObject* obj);
};

/*!
 * \brief MCT Controlled Building
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Configures specified object to behave like attached object's MCT.
 *
 * \param MCTID
 *   Object ID of the MCT.
 * \param Multiplier
 *   Damage multiplier of MCT.
 */
class UP_MCT_Building : public ScriptImpClass
{
public:
	void Created(GameObject* obj);
	void Damaged(GameObject* obj, GameObject* damager, float amount);

private:
	ReferencerClass MCTObj;
};

/*!
 * \brief Master Control Terminal
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Automatically attached by UP_MCT_Building, forwards damage requests from this object to building that attached this script.
 *
 * \param Object
 *   Main building object of this MCT.
 * \param Multiplier
 *   Damage multiplier of this MCT.
 */
class UP_Master_Control_Terminal : public ScriptImpClass
{
private:
	ReferencerClass MainBuilding;
	float Multiplier;

public:
	void Created(GameObject* obj);
	void Damaged(GameObject* obj, GameObject* damager, float amount);
};

/*!
 * \brief Grant Weapon
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Grants a weapon to the attached object with specified ammunition, and detaches.
 *
 * \param Preset
 *   Name of weapon preset to give.
 * \param ClipRounds
 *   Amount of clip rounds to give.
 * \param InventoryRounds
 *   Amount of inventory rounds to give.
 * \param AutoSelect
 *   Automatically select the weapon after giving.
 */
class UP_Grant_Weapon : public ScriptImpClass
{
	void Created(GameObject* obj);
};

/*!
 * \brief Grant Weapon on Zone Enter
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Grants a weapon to the enterer objects with specified ammunition.
 *
 * \param Preset
 *   Name of weapon preset to give.
 * \param ClipRounds
 *   Amount of clip rounds to give.
 * \param InventoryRounds
 *   Amount of inventory rounds to give.
 * \param AutoSelect
 *   Automatically select the weapon after giving.
 */
class UP_Grant_Weapon_Zone_Enter : public ScriptImpClass
{
	void Entered(GameObject* obj, GameObject* enterer);
};

/*!
 * \brief Grant Weapon on Zone Exit
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Grants a weapon to the exiter objects with specified ammunition.
 *
 * \param Preset
 *   Name of weapon preset to give.
 * \param ClipRounds
 *   Amount of clip rounds to give.
 * \param InventoryRounds
 *   Amount of inventory rounds to give.
 * \param AutoSelect
 *   Automatically select the weapon after giving.
 */
class UP_Grant_Weapon_Zone_Exit : public ScriptImpClass
{
	void Exited(GameObject* obj, GameObject* exiter);
};

/*!
 * \brief Spawn Object on Death
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Spawns a preset when attached object dies.
 *
 * \param Preset
 *   Name of the preset to spawn.
 */
class UP_Spawn_When_Death : public ScriptImpClass
{
	void Killed(GameObject* obj, GameObject* killer);
};

/*!
 * \brief Game Over on Death
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Ends the game when attached object dies.
 */
class UP_GameOver_When_Death : public ScriptImpClass
{
	void Killed(GameObject* obj, GameObject* killer);
};

/*!
 * \brief Change Character
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Changes character of attached object, and detaches.
 *
 * \param Preset
 *   Name of the soldier preset to change.
 */
class UP_Change_Character : public ScriptImpClass
{
	void Created(GameObject* obj);
};

/*!
 * \brief Change Character on Zone Enter
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Changes character of enterer objects.
 *
 * \param Preset
 *   Name of the soldier preset to change.
 */
class UP_Grant_Character_Zone_Enter : public ScriptImpClass
{
	void Entered(GameObject* obj, GameObject* enterer);
};

/*!
 * \brief Change Character on Zone Exit
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Changes character of exiter objects.
 *
 * \param Preset
 *   Name of the soldier preset to change.
 */
class UP_Grant_Character_Zone_Exit : public ScriptImpClass
{
	void Exited(GameObject* obj, GameObject* exiter);
};

/*!
 * \brief Spawn Object and Destroy Itself When Fully Repaired
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Spawns a specified preset, and destroys itself when attached object is fully repaired.
 *
 * \param Preset
 *   Name of the preset to spawn.
 */
class UP_Spawn_And_Destroy_When_Fully_Repaired : public ScriptImpClass
{
	void Damaged(GameObject* obj, GameObject* damager, float amount);
};

/*!
 * \brief Refinery
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Gives specified amount of money with a specified interval to specified team.
 *
 * \param Team
 *   ID of the team to give money.
 * \param Interval
 *   Delay between giving money.
 * \param Amount
 *   Amount of money to give.
 */
class UP_Refinery_Object : public ScriptImpClass
{
private:
	int Team;
	float Amount;

public:
	void Created(GameObject* obj);
	void Timer_Expired(GameObject* obj, int number);
};

/*!
 * \brief Vehicle PT
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Creates a vehicle purchase terminal with custom spawn location for specified team.
 *
 * \param Team
 *   ID of the team to allow purchases to.
 * \param SpawnerID
 *   Object to spawn new vehicles.
 * \param VehiclePreset
 *   Name of the vehicle preset to spawn.
 * \param Credits
 *   Money required to spawn the preset.
 */
class UP_Vehicle_Purchase_Terminal : public ScriptImpClass
{
private:
	bool Pokable;
	Vector3 SpawnLocation;
	float Facing;
	ReferencerClass Terminal;
	StringClass Preset;
	int Amount;

public:
	void Created(GameObject* obj);
	void Poked(GameObject* obj, GameObject* poker);
	void Timer_Expired(GameObject* obj, int number);
};

/*!
 * \brief Infantry PT
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Creates an infantry purchase terminal for specified team.
 *
 * \param Team
 *   ID of the team to allow purchases to.
 * \param CharacterPreset
 *   Name of the infantry preset to spawn.
 * \param Credits
 *   Money required to buy and switch to the infantry preset.
 */
class UP_Character_Purchase_Terminal : public ScriptImpClass
{
private:
	bool Pokable;
	ReferencerClass Terminal;
	StringClass Preset;
	int Amount;

public:
	void Created(GameObject* obj);
	void Poked(GameObject* obj, GameObject* poker);
	void Timer_Expired(GameObject* obj, int number);
};

/*!
 * \brief Set Team on Vehicle Exit
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Sets the team of vehicle when someone leaves the vehicle.
 *
 * \param Team
 *   ID of team to change after leaving the vehicle.
 */
class UP_Set_Team_When_Exit : public ScriptImpClass
{
	void Custom(GameObject* obj, int type, int param, GameObject* sender);
	void Timer_Expired(GameObject* obj, int number);
};

/*!
 * \brief Ion Explosion on Death
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Creates an Ion Cannon explosion at the attached object's location when it dies.
 */
class UP_Ion_Explosion_When_Death : public ScriptImpClass
{
	void Killed(GameObject* obj, GameObject* killer);
};

/*!
 * \brief Enable Fly Mode
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Enables fly mode for attached object, and detaches.
 */
class UP_FlyMode_Enable : public ScriptImpClass
{
	void Created(GameObject* obj);
};

/*!
 * \brief Disable Fly Mode
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Disables fly mode for attached object, and detaches.
 */
class UP_FlyMode_Disable : public ScriptImpClass
{
	void Created(GameObject* obj);
};

/*!
 * \brief Toggle Spectate Mode
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Enables or disables spectator mode for attached player, and detaches.
 */
class UP_Spectate : public ScriptImpClass
{
	void Created(GameObject* obj);
};

/*!
 * \brief Toggle Spectate Mode on Zone Enter
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Enables or disables spectator mode for enterer players.
 */
class UP_Spectate_Zone_Enter : public ScriptImpClass
{
	void Entered(GameObject* obj, GameObject* enterer);
};

/*!
 * \brief Toggle Spectate Mode on Zone Exit
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Enables or disables spectator mode for exiter players.
 */
class UP_Spectate_Zone_Exit : public ScriptImpClass
{
	void Exited(GameObject* obj, GameObject* exiter);
};

/*!
 * \brief Select Weapon
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Selects specified weapon for attached object, and detaches.
 *
 * \param WeaponPreset
 *   Name of the weapon preset to select. Nothing happens if player does not have the weapon or is an invalid preset.
 */
class UP_Select_Weapon : public ScriptImpClass
{
	void Created(GameObject* obj);
};

/*!
 * \brief Select Weapon on Zone Enter
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Selects specified weapon for enterer objects.
 *
 * \param WeaponPreset
 *   Name of the weapon preset to select. Nothing happens if player does not have the weapon or is an invalid preset.
 */
class UP_Select_Weapon_On_Zone_Enter : public ScriptImpClass
{
	void Entered(GameObject* obj, GameObject* enterer);
};

/*!
 * \brief Select Weapon on Zone Exit
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Selects specified weapon for exiter objects.
 *
 * \param WeaponPreset
 *   Name of the weapon preset to select. Nothing happens if player does not have the weapon or is an invalid preset.
 */
class UP_Select_Weapon_On_Zone_Exit : public ScriptImpClass
{
	void Exited(GameObject* obj, GameObject* exiter);
};

/*!
 * \brief Paradrop
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Creates a para drop cinematic which was taken from 3rd mission scripts, and drops up to 3 presets.
 * Send a custom event with type "15730" and object ID as parameter to attach a custom object to slot 1, overriding the script parameters.
 * Send a custom event with type "15731" and object ID as parameter to attach a custom object to slot 2, overriding the script parameters.
 * Send a custom event with type "15732" and object ID as parameter to attach a custom object to slot 3, overriding the script parameters.
 *
 * \param FirstPreset
 *   Name of the preset to spawn in first slot.
 * \param SecondPreset
 *   Name of the preset to spawn in second slot.
 * \param ThirdPreset
 *   Name of the preset to spawn in third slot.
 */
class UP_Chinook_Reinforcements_Paradrop : public ScriptImpClass
{
private:
	bool chinookKilled;
	int paratrooperIndex;
	ReferencerClass presetObj1;
	ReferencerClass presetObj2;
	ReferencerClass presetObj3;
	ReferencerClass helicopter;
	ReferencerClass trajectory;

	bool Check_Object(GameObject* obj)
	{
		if (obj)
		{
			if (obj->As_SmartGameObj())
			{
				if (obj->As_SoldierGameObj())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

public:
	void Created(GameObject* obj);
	void Custom(GameObject* obj, int type, int param, GameObject* sender);
	void Timer_Expired(GameObject* obj, int number);
};

/*!
 * \brief Invincible Until Custom Event
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Disables damages for attached object until a custom is received.
 *
 * \param Message
 *   Type of the custom event to disable invincibility.
 */
class UP_Unkillable_Until_Custom : public ScriptImpClass
{
	void Custom(GameObject* obj, int type, int param, GameObject* sender);
	void Damaged(GameObject* obj, GameObject* damager, float amount);
};

/*!
 * \brief Attach Script to Itself with Interval
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Attaches a script to itself after every specified time.
 *
 * \param Script
 *   Name of the script to attach.
 * \param Parameters
 *   Parameters of the script. Use '|' as the delimiter.
 * \param LoopCount
 *   Total loop count. Set to -1 for infinite loop.
 * \param Interval
 *   Delay between each attach.
 */
class UP_AttachScript_Interval_Self : public ScriptImpClass
{
private:
	StringClass Script;
	StringClass Parameters;
	float Interval;
	int LoopCount;
	int Looped;

public:
	void Created(GameObject* obj);
	void Timer_Expired(GameObject* obj, int number);
};

/*!
 * \brief Attach Script to Target with Interval
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Attaches a script to target object after every specified time.
 *
 * \param ObjectID
 *   ID of object to attach the script.
 * \param Script
 *   Name of the script to attach.
 * \param Parameters
 *   Parameters of the script. Use '|' as the delimiter.
 * \param LoopCount
 *   Total loop count. Set to -1 for infinite loop.
 * \param Interval
 *   Delay between each attach.
 */
class UP_AttachScript_Interval : public ScriptImpClass
{
private:
	StringClass Script;
	StringClass Parameters;
	ReferencerClass Target;
	float Interval;
	int LoopCount;
	int Looped;

public:
	void Created(GameObject* obj);
	void Timer_Expired(GameObject* obj, int number);
};

/*!
 * \brief Object Follower
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Adds object following capability by customs.
 *
 * \param TargetChangeCustomID
 *   ID to listen to change target object. Set to "0" to reset following.
 * \param DestroyScriptCustomID
 *   ID to listen to detach this script.
 */
class UP_Object_Follower_AI : public ScriptImpClass
{
private:
	int TargetChangeCustom;
	int DestroyScriptCustom;
	ReferencerClass TargetObject;

public:
	void Created(GameObject* obj);
	void Custom(GameObject* obj, int type, int param, GameObject* sender);
	void Timer_Expired(GameObject* obj, int number);
};

/*!
 * \brief Private Color Message on Poke
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Sends a private color message to poker when poked.
 *
 * \param Message
 *   Message to send to exiter player.
 * \param Red
 *   Red tone of the message.
 * \param Green
 *   Green tone of the message.
 * \param Blue
 *   Blue tone of the message.
 */
class UP_Private_Color_Message_On_Poke : public ScriptImpClass
{
private:
	bool Pokable;

public:
	void Created(GameObject* obj);
	void Timer_Expired(GameObject* obj, int number);
	void Poked(GameObject* obj, GameObject* poker);
};

/*!
 * \brief Seize Weapon on Zone Enter
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Removes the weapon from enterer object, and gives weapon back when it leaves the zone.
 *
 * \param Weapon
 *   Name of the weapon preset to seize.
 * \param CustomID
 *   Unique custom type for this script to grant weapons back to player.
 */
class UP_Seize_Weapon_Enter : public ScriptImpClass
{
public:
	void Entered(GameObject* obj, GameObject* enterer);
	void Exited(GameObject* obj, GameObject* exiter);
};

/*!
 * \brief Seize Weapon on Zone Exit
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Removes the weapon from exiter object, and gives weapon back when it enters the zone.
 *
 * \param Weapon
 *   Name of the weapon preset to seize.
 * \param CustomID
 *   Unique custom type for this script to grant weapons back to player.
 */
class UP_Seize_Weapon_Exit : public ScriptImpClass
{
public:
	void Entered(GameObject* obj, GameObject* enterer);
	void Exited(GameObject* obj, GameObject* exiter);
};

/*!
 * \brief Seize All Weapons on Zone Enter
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Removes all weapons except for specified ones from enterer object, and gives everything back when it leaves the zone.
 *
 * \param Weapons
 *   Name of the weapon presets to exclude from seize.
 * \param CustomID
 *   Unique custom type for this script to grant weapons back to player.
 */
class UP_Seize_All_Weapons_Enter : public ScriptImpClass
{
private:
	DynamicVectorClass<StringClass> Weapons;

public:
	void Created(GameObject* obj);
	void Entered(GameObject* obj, GameObject* enterer);
	void Exited(GameObject* obj, GameObject* exiter);
};

/*!
 * \brief Seize All Weapons on Zone Exit
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Removes all weapons except for specified ones from exiter object, and gives everything back when it enters the zone.
 *
 * \param Weapons
 *   Name of the weapon presets to exclude from seize.
 * \param CustomID
 *   Unique custom type for this script to grant weapons back to player.
 */
class UP_Seize_All_Weapons_Exit : public ScriptImpClass
{
private:
	DynamicVectorClass<StringClass> Weapons;

public:
	void Created(GameObject* obj);
	void Entered(GameObject* obj, GameObject* enterer);
	void Exited(GameObject* obj, GameObject* exiter);
};

/*!
 * \brief Grant Weapon and Detach on Custom Event 
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Grants specified weapon to attached object with specified ammunition on a custom event, and detaches.
 *
 * \param Preset
 *   Name of weapon preset to give.
 * \param ClipRounds
 *   Amount of clip rounds to give.
 * \param InventoryRounds
 *   Amount of inventory rounds to give.
 * \param AutoSelect
 *   Automatically select the weapon after giving.
 * \param CustomID
 *   Type of the custom event to grant weapon and detach.
 */
class UP_Grant_Weapon_Custom_Then_Detach : public ScriptImpClass
{
public:
	void Custom(GameObject* obj, int type, int param, GameObject* sender);
};

/*!
 * \brief Disable Weapon for Driver
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Removes weapon from vehicle if driver is gunner, and gives back when a gunner enters. Designed to work with DriverIsAlwaysGunner turned off.
 */
class UP_Disable_Weapon_For_Driver : public ScriptImpClass
{
private:
	bool HasWeapon;
	StringClass Weapon;
	int ClipAmmo;
	int InventoryAmmo;

public:
	void Created(GameObject* obj);
	void Custom(GameObject* obj, int type, int param, GameObject* sender);
};

/*!
 * \brief Set Bot Tag
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Sets the bot tag of attached soldier object, then detaches.
 *
 * \param Tag
 *   Tag to set.
 */
class UP_Set_Bot_Tag : public ScriptImpClass
{
public:
	void Created(GameObject* obj);
};

/*!
 * \brief Set Bot Tag Multiple Lines
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Sets the bot tag of attached soldier object with multiple lines, then detaches.
 *
 * \param Tag1
 *   First line for the tag.
 * \param Tag2
 *   Second line for the tag.
 * \param Tag3
 *   Third line for the tag.
 * \param Tag4
 *   Fourth line for the tag.
 * \param Tag5
 *   Fifth line for the tag.
 */
class UP_Set_Bot_Tag_MultiLine : public ScriptImpClass
{
public:
	void Created(GameObject* obj);
};

/*!
 * \brief Remove Bot Tag
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Removes the bot tag of attached soldier object, then detaches.
 */
class UP_Remove_Bot_Tag : public ScriptImpClass
{
public:
	void Created(GameObject* obj);
};

/*!
 * \brief Stop Sound
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Stops the sound specified by its ID, then detaches.
 *
 * \param SoundID
 *   ID of the sound to stop.
 * \param Destroy
 *   Whether or not to destroy the sound after stopping.
 */
class UP_Stop_Sound : public ScriptImpClass
{
public:
	void Created(GameObject* obj);
};

/*!
 * \brief 2D Sound Controller
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Creates a 2D sound with specified definition name, and starts/stops it with customs.
 * The created sound is destroyed when the script detaches or the attached object gets destroyed.
 *
 * \param SoundDef
 *   Definition name of the sound to create.
 * \param StartCustomType
 *   Expected custom type to start the sound.
 * \param StopCustomType
 *   Expected custom type to stop the sound.
 */
class UP_Sound_Controller_2D : public ScriptImpClass
{
public:
	void Created(GameObject* obj);
	void Destroyed(GameObject* obj);
	void Custom(GameObject* obj, int type, int param, GameObject* sender);

private:
	StringClass SoundDef;
	int SoundID;
	int StartCustomType;
	int StopCustomType;
};

/*!
 * \brief 3D Sound Controller
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Creates a 3D sound with specified definition name and at the position of the attached object, and starts/stops it with customs.
 * The created sound is destroyed when the script detaches or the attached object gets destroyed.
 *
 * \param SoundDef
 *   Definition name of the sound to create.
 * \param StartCustomType
 *   Expected custom type to start the sound.
 * \param StopCustomType
 *   Expected custom type to stop the sound.
 */
class UP_Sound_Controller_3D : public ScriptImpClass
{
public:
	void Created(GameObject* obj);
	void Destroyed(GameObject* obj);
	void Custom(GameObject* obj, int type, int param, GameObject* sender);

private:
	StringClass SoundDef;
	int SoundID;
	int StartCustomType;
	int StopCustomType;
};

/*!
 * \brief Teleport Killer on Death
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Teleports the player who killed this object to the specified object.
 *
 * \param TeleportObj
 *   ID of the object to teleport to.
 */
class UP_Teleport_Killer_On_Death : public ScriptImpClass
{
public:
	void Created(GameObject* obj);
	void Killed(GameObject* obj, GameObject* killer);

private:
	int TeleportObj;
};

/*!
 * \brief Immunity for Teleport Killer on Death
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * This is a flag that hints \b UP_Teleport_Killer_On_Death to not teleport the killer.
 *
 * \param Expire
 *	 Duration in seconds of this immunity. 0 means infinite immunity.
 */
class UP_Teleport_Killer_Immunity : public ScriptImpClass
{
public:
	void Created(GameObject* obj);
	void Timer_Expired(GameObject* obj, int number);

private:
	float Expire;
};

/*!
 * \brief Teleport on Kill inside Zone
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 * Teleports the killer to specified object when killer kills someone who's inside this zone.
 *
 * \param TeleportObj
 *   ID of the object to teleport to.
 */
class UP_Teleport_On_Zone_Kill : public ScriptImpClass
{
public:
	void Created(GameObject* obj);
	void Entered(GameObject* obj, GameObject* enterer);
	void Exited(GameObject* obj, GameObject* exiter);

private:
	int TeleportObj;
};

/*!
 * \brief Teleport on Kill Immunity Zone
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 *	Makes the objects who enter this zone immune to the UP_Teleport_Killer_On_Death script.
 *
 * \param Expire
 *	 Duration in seconds of this immunity. 0 means infinite immunity.
 */
class UP_Teleport_Killer_Immunity_Zone : public ScriptImpClass
{
public:
	void Created(GameObject* obj);
	void Entered(GameObject* obj, GameObject* enterer);
	void Exited(GameObject* obj, GameObject* exiter);

private:
	float Expire;
};

/*!
 * \brief Gravity Zone
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 *	Changes the gravity multiplier of objects in the zone.
 *
 * \param Multiplier
 *   The gravity multiplier to set.
 */
class UP_Gravity_Zone : public ScriptImpClass
{
public:
	void Created(GameObject* obj);
	void Entered(GameObject* obj, GameObject* enterer);
	void Exited(GameObject* obj, GameObject* exiter);

private:
	float Multiplier;
};

/*!
 * \brief No Targeting Zone
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 *	Disables the global targeting of players in the zone. (Except for pokeable objects and buildings)
 */
class UP_No_Targeting_Zone : public ScriptImpClass
{
public:
	void Entered(GameObject* obj, GameObject* enterer);
	void Exited(GameObject* obj, GameObject* exiter);
};

/*!
 * \brief Send Message to Self On Custom
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 *	Sends a chat message to the attached object when a custom event is received. If the attached object is not a player, then script destroys itself.
 *
 *	\param ID
 *	 The custom message ID.
 *	\param Param
 *	 The custom message parameter.
 *	\param Red
 *	 Red tone of the message.
 *	\param Green
 *	 Green tone of the message.
 *	\param Blue
 *	 Blue tone of the message.
 *	\param Message
 *	 The message to be displayed.
 *	\param DestroyAfterSend
 *	 Destroys the script after sending the message if set to 1.
 */
class UP_Send_Message_Self_On_Custom : public ScriptImpClass
{
public:
	void Created(GameObject* obj);
	void Custom(GameObject* obj, int type, int param, GameObject* sender);
};

/*!
 * \brief Send Message to Sender On Custom
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 *	Sends a chat message to the sender of a custom event.
 *
 *	\param ID
 *	 The custom message ID.
 *	\param Param
 *	 The custom message parameter.
 *	\param Red
 *	 Red tone of the message.
 *	\param Green
 *	 Green tone of the message.
 *	\param Blue
 *	 Blue tone of the message.
 *	\param Message
 *	 The message to be displayed.
 *	\param DestroyAfterSend
 *	 Destroys the script after sending the message if set to 1.
 */
class UP_Send_Message_Sender_On_Custom : public ScriptImpClass
{
public:
	void Custom(GameObject* obj, int type, int param, GameObject* sender);
};

/*!
 * \brief Print Console of Self On Custom
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 *	Prints a message to the console of the attached object when a custom event is received. If the attached object is not a player, then script destroys itself.
 *
 *	\param ID
 *	 The custom message ID.
 *	\param Param
 *	 The custom message parameter.
 *	\param Alpha
 *	 Alpha (transparency) tone of the message.
 *	\param Red
 *	 Red tone of the message.
 *	\param Green
 *	 Green tone of the message.
 *	\param Blue
 *	 Blue tone of the message.
 *	\param Message
 *	 The message to be displayed.
 *	\param DestroyAfterSend
 *	 Destroys the script after printing the message if set to 1.
 */
class UP_Print_Console_Self_On_Custom : public ScriptImpClass
{
public:
	void Created(GameObject* obj);
	void Custom(GameObject* obj, int type, int param, GameObject* sender);
};

/*!
 * \brief Print Console of Sender On Custom
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 *	Prints a message to the console of the sender of a custom event.
 *
 *	\param ID
 *	 The custom message ID.
 *	\param Param
 *	 The custom message parameter.
 *	\param Alpha
 *	 Alpha (transparency) tone of the message.
 *	\param Red
 *	 Red tone of the message.
 *	\param Green
 *	 Green tone of the message.
 *	\param Blue
 *	 Blue tone of the message.
 *	\param Message
 *	 The message to be displayed.
 *	\param DestroyAfterSend
 *	 Destroys the script after printing the message if set to 1.
 */
class UP_Print_Console_Sender_On_Custom : public ScriptImpClass
{
public:
	void Custom(GameObject* obj, int type, int param, GameObject* sender);
};

/*!
 * \brief Add Chat History to Self On Custom
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 *	Adds a message to the chat history (without displaying in chat) of attached object when a custom event is received. If the attached object is not a player, then script destroys itself.
 *
 *	\param ID
 *	 The custom message ID.
 *	\param Param
 *	 The custom message parameter.
 *	\param Red
 *	 Red tone of the message.
 *	\param Green
 *	 Green tone of the message.
 *	\param Blue
 *	 Blue tone of the message.
 *	\param Message
 *	 The message to be displayed.
 *	\param DestroyAfterSend
 *	 Destroys the script after sending the message if set to 1.
 */
class UP_Add_Chat_History_Self_On_Custom : public ScriptImpClass
{
public:
	void Created(GameObject* obj);
	void Custom(GameObject* obj, int type, int param, GameObject* sender);
};

/*!
 * \brief Add Chat History to Sender On Custom
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 *	Sends a chat message to the sender of a custom event.
 *
 *	\param ID
 *	 The custom message ID.
 *	\param Param
 *	 The custom message parameter.
 *	\param Red
 *	 Red tone of the message.
 *	\param Green
 *	 Green tone of the message.
 *	\param Blue
 *	 Blue tone of the message.
 *	\param Message
 *	 The message to be displayed.
 *	\param DestroyAfterSend
 *	 Destroys the script after sending the message if set to 1.
 */
class UP_Add_Chat_History_Sender_On_Custom : public ScriptImpClass
{
public:
	void Custom(GameObject* obj, int type, int param, GameObject* sender);
};

/*!
 * \brief Grant Credits to Self On Custom
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 *	Grants credits to attached object when a custom event is received. The amount is determined by the custom message's parameter. If the attached object is not a player, then script destroys itself.
 *
 *	\param ID
 *	 The custom message ID.
 *	\param DestroyAfterGrant
 *	 Destroys the script after sending the message if set to 1.
 */
class UP_Grant_Credits_Self_On_Custom : public ScriptImpClass
{
public:
	void Created(GameObject* obj);
	void Custom(GameObject* obj, int type, int param, GameObject* sender);
};

/*!
 * \brief Grant Credits to Sender On Custom
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 *	Grants credits to the sender of a custom event. The amount is determined by the custom message's parameter.
 *
 *	\param ID
 *	 The custom message ID.
 *	\param DestroyAfterGrant
 *	 Destroys the script after sending the message if set to 1.
 */
class UP_Grant_Credits_Sender_On_Custom : public ScriptImpClass
{
public:
	void Custom(GameObject* obj, int type, int param, GameObject* sender);
};

/*!
 * \brief Grant Credits to Team On Custom
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 *	Grants credits to the entire team on a custom event. The amount is determined by the custom message's parameter.
 *
 *	\param Team
 *	 The team which receives the credits.
 *	\param ID
 *	 The custom message ID.
 *	\param DestroyAfterGrant
 *	 Destroys the script after sending the message if set to 1.
 */
class UP_Grant_Credits_Team_On_Custom : public ScriptImpClass
{
public:
	void Custom(GameObject* obj, int type, int param, GameObject* sender);
};

/*!
 * \brief Steal Credits On Custom
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 *	Steal credits from a team and evenly distributes the stolen credits to the opponent team on a custom event.
 *
 *	\param Team
 *	 The team which loses the credits.
 *	\param Percentage
 *	 The percentage of credits to be stolen from the team. Setting to a negative value (such as -1) will use the custom message's parameter value as the percentage.
 *	\param ID
 *	 The custom message ID.
 *	\param DestroyAfterSteal
 *	 Destroys the script after stealing the credits if set to 1.
 *
 *	\remark
 *	 The "Team" parameter has 4 special values.
 *	 -1 = Steal credits from the team of the sender object.
 *	 -2 = Steal credits from the team of the attached object.
 *	 -3 = Steal credits from the opponent team of the sender object.
 *	 -4 = Steal credits from the opponent team of the attached object.
 *
 *	\remark
 *	 Normally, the range of "Percentage" parameter is [0, 1]. However, if the "Percentage" parameter is negative, then the custom message's parameter value's range is [0, 100].
 */
class UP_Steal_Credits_On_Custom : public ScriptImpClass
{
public:
	void Custom(GameObject* obj, int type, int param, GameObject* sender);
};

/*!
 * \brief No Refill
 * \author Unstoppable
 * \ingroup UnstoppableScripts
 *
 *	Disables refilling as long as there is at least 1 instance of the script.
 */
class UP_No_Refill : public ScriptImpClass
{
public:
	void Created(GameObject* obj);
	void Destroyed(GameObject* obj);

protected:
	static bool Refill_Hook(GameObject* purchaser);

private:
	static bool RefillHookInstalled;
	static int AliveScriptCount;
};