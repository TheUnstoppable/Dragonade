Dragonade 1.10.7
Created by Whitedragon and Black-Cell.net

Special thanks to:
Vloktboky, original auther of DA and creator of server-side modding in general.
Jonwil, creating scripts.dll and tt.dll.
Ma1kel, input and ideas on various things.
ExEric3, providing hosting and test servers.
dblaney1, misc. code and fixes.
...and everyone that supported BC over the years.


--------------------------------------------------------------------------------------------------------------------------------------------------------------


Installation:

1. Download and install the latest server version of TT from http://www.tiberiantechnologies.org/downloads. 
   Note that DA was designed to work with TT 4.7. It probably won't work with other versions.
2. Copy the contents of the server folder included in this zip to the server folder of your FDS, overwriting any existing files.
3. Configure da.ini, da_crates.ini, and the relevant gamemode.ini to your liking.


--------------------------------------------------------------------------------------------------------------------------------------------------------------


Update History:

Version 1.10.7
  Additions:
   - Updated to TT 4.8.

  Changes:
   - Tweaks to the chat hook of Dragonade Core.
   - Use the vehicle owner for enemy checking of vehicles for Veteran System.
   - Partially standardize GameSpy broadcaster for the new TT system.

  Bug Fixes:
   - Fix Veteran System is not granting veteran points for repairing.

Version 1.10.6
  Additions:
   - Updated to TT 4.7.4.

Version 1.10.5 Hotfix #2
  Additions:
   - Added Is_Smart_Bot DA engine function.

  Changes:
   - Add C4/Beacon detonate and disarm messages for smart bots.
   - Improve smart bot detection in kill messages and C4/Beacon pokes.
   - Add Dialog event in Player Observer.
   - Armor, health, weapon and keycard powerup pickups are now displayed in HUD.
   - Remove HUD grant display from DA's Loot PowerUps as this is now handled by Dragonade's core.
   - Private chats now trigger plugin events.

  Bug Fixes:
   - Fix when a vehicle with 0-seat and VehicleType not set to Turret crashes server when it kills a vehicle.
   - Fix base power off-line messages to log server when there are more than 1 Power Plants in the map.
   - Fix so kill streak won't increment when player suicides.

Version 1.10.5 Hotfix #1
  Changes:
   - Add HUD weapon grant display to Backpacks.

  Bug Fixes:
   - Fix a crash occurring when a weapon power-up is granted to a player created by Dragonade Loot System.
   - Fix BlockFakeBeacons not working properly.

Version 1.10.5
  Additions:
   - Updated to TT 4.7.3.
   - Added Kill Streaks game feature.
   - Added dialog event.

  Changes:
   - Game data now updates after server title changes to display it correctly in Discord Rich Presence.
   - When a C4/Beacon is poked, display bot tag or preset translation if it's owner is not a player.
   - Display weapon/ammo grants in HUD when a backpack is picked up.

Version 1.10.4
  Additions:
   - Updated to TT 4.7.2.

  Bug Fixes:
   - Fix a bug in Veteran System when the vehicle that player is currently in gets destroyed confuses Veteran System, causing a crash.

Version 1.10.3 Hotfix #1
  Changes:
   - Improvements for kill messages.
   - Move veteran system configuration to a new file "da_veteran.ini".

  Bug Fixes:
   - Fix a bug in Veteran System when an object is killed by non-player would not give player veteran points even if player damaged the victim before.

Version 1.10.3
  Additions:
   - Updated to TT 4.7.1.
   - Add more kill/destroy/suicide messages for moonsense715's bots.
   - Add Veteran game feature.
   - Add Building Annotations game feature.

  Changes:
   - Modify Veteran Crate to grant veteran points between 1-30 when picked up with Veteran game feature.

  Bug Fixes:
   - Fix a bug where tagging an enemy building or dead enemy unit then using Radio6 command would report "Enemy spotted!" to enemy.
   - Fix an exploit where tagging a stealth enemy vehicle then using Radio5 or Radio6 would reveal stealthed enemy with a small icon on top of it.
   - Fix so using Taunts with a non-Human model no longer crashes server.

Version 1.10.2
  Additions:
   - Updated to TT 4.7.
   - Renamed DA function "Set_Emot_Icon" to "Set_Emote_Icon" to fix redefinition with engine call "Set_Emot_Icon" (sei).
   - Added private chat messages to show up in events.

Version 1.10.1
  Additions:
   - Added DefaultConnectionAcceptanceFilter.

  Bug Fixes:
    - Fixed typo in HUDMSG help text.

Version 1.10
  Additions:
   - Updated to TT 4.6.9.

  Changes:
   - Swapped Radio8 key hook with Radio12 key hook.
   - Improved stealth checking in some engine calls.

  Bug Fixes:
   - Fixed beacon owner checking in DAC4BeaconManager::Beacon_Deploy_Event
   - Fixed memory leaks by introducing a virtual destructor in several Dragonade classes.
   - Fixed GameSpy default master servers.
   - Fixed GameSpy server not listening to the 'info' command.
   - Fixed vehicle shell physics. 
   - Fixed PresetMgrClass static declaration

Version 1.9:
  Additions:
   - Updated to TT 4.3.
   - Updated to Visual Studio 2015.
   - Added DA log message for building revival.
   - Added HUD message console commands and functions that display a message in the middle of the HUD.
   - Crates and loot now display a HUD message when picked up.
   - New game feature "Chat Sounds" which allows you to setup sounds to play for certain chat messages.
   - New game feature "Purchasable Weapons" which allows players to purchase weapons with chat commands.

  Changes:
   - Changed maximum squad size from (current players/3) to ((current_players/6)+1).

  Bug Fixes:
   - Fixed rare crash when deleting a DAGameObjObserverClass.
   - Fixed blank game title when enabling GameSpy midgame.
   - Mutants can once again heal themselves with splash damage.
   - Fixed invisible vehicle death explosions for 4.3 clients.
   - Fixed some bugs caused by C4/beacons with no owner.



Version 1.8.1:
  Additions:
   - Added setting "IntermissionTime" which sets how long the score screen is displayed at the end of a game.

  Changes:
   - Kill messages for players destroying their own vehicles now only display if an enemy has recently damaged the vehicle.

  Bug Fixes:
   - Added some missing functions from the final release of TT 4.2.
   - The "!stuck" command should now work in more situations.
   - Fixed the object event type "ARMED".
   - The "!killme" command can now kill you while in a vehicle.



Version 1.8:
  Additions:
   - Added a message when squads are created.
   - The beacon deployment team message now works with buildings that aren't BuildingGameObjs.
   - Added an indication when the Second Wind Crate triggers.
   - You can now talk in squad chat by paging yourself.

  Changes:
   - Squad radar has been re-enabled now that changing radar works again.
   - Maximum squad size is now capped at a third of the current player count.

  Bug Fixes:
   - Added some missing functions/exports.
   - Fixed the ban system not reading or writing to the ban database.
   - Fixed various bugs in the squad system.
   - Damage events doing 0 damage will once again call GameObjObserverClass::Damaged. (For real)
   - Reduced weapon limit to 28 to prevent a crash/gameplay pending issue.
   - Fixed pages to the host being logged twice.
   - Fixed team rebalancing sometimes not occuring when it should have.
   - Fixed Get_Damage_Warhead.
   - Fixed a crash in the disable list if an invalid preset was used.



Version 1.7:
  Additions:
   - Updated to TT 4.2.
  
  Changes:
   - Removed "MessageNick" setting.

  Bug Fixes:
   - Damage events doing 0 damage will once again call GameObjObserverClass::Damaged.



Version 1.6.2:
  Additions:
   - Added "RebalanceScoreLimit" setting.

  Changes:
   - Players on teams other than Nod/GDI will no longer be affected by team remixing/rebalancing/swapping.
   - The icons over Loot powerups now work for weapons with no powerup model.
   - The vehicle limit reached message will no longer play on maps without a vehicle factory.
  
  Bug Fixes:
   - Fixed a client crash.



Version 1.6.1:
  Changes:
   - The UAV Crate will now temporarily reactivate a team's radar if it is offline.
   - Both teams can now have the UAV Crate active at the same time.

  Bug Fixes:
   - Partially fixed map names not showing up in the server info menu. (Full fix will be in TT 4.2 client)
   - Fixed parachutes getting stuck on uneven terrain.
   - The Vehicle Queue will now correctly respawn the Harvester when the Refinery is revived.
   - Added a per-player weapon limit of 30 to prevent a crash.



Version 1.6:
  Additions:
   - Added support for broadcasting to multiple GameSpy master servers.
   - Added settings "EnableMultiWeaponDrop" and "EnableWeaponCarryOver" to the Loot Game Feature.
   - Added vehicle drop off positions for more fan maps.
   
  Changes:
   - Players can now repickup weapons they've dropped with the "!drop" command after a short delay.
   - The Power Crate will now affect both teams instead of just one.
   - The Stealth Crate can now be picked up by vehicles on servers that are TT only.
   - Tweaked the Exclusive and Remove Weapon systems so they work better on servers without infinite ammo.
   - The Random Vehicle and Overhaul Crates can now grant vehicles from the alternate skin slots of the PT.
   - Moved "GameSpyQueryPort" setting to da.ini.

  Bug Fixes:
   - Fixed possible crash on building death.
   - Character Refunds now work again.
   - Fixed Power Crate not working correctly on maps with multiple base defenses.
   - Fixed vehicles unlocking when their owner switches characters.
   - Fixed crate spawners not working on some fan maps.



Version 1.5:
  Additions:
   - Updated to TT 4.1.
   - Updated to Visual Studio 2012.

  Changes:
   - Players can now donate all their credits to a teammate by using "!donate <player>", i.e. without specifying an amount.
  
  Bug Fixes:
   - Fixed the da.ini spawn character settings breaking the spawn character settings in leveledit and tt.ini.
   - Visceroids now have a firing animation.
   - Fixed the invisible blockers in Domination mode sometimes being visible.



Version 1.4:
  Additions:
   - A new ban manager has been added. It supports bans, ban exceptions, forceTTs, and forceTT exceptions.
     Console functions are included to kick/ban/forceTT players, or add/remove/list bans, ban exceptions, forceTTs, and forceTT exceptions.
     Log messages will be under the "_BAN" header.
   - Settings sections "[Exclusive_Weapons]", "[Replace_Weapons]", and "[Remove_Weapons]" have been added. Together these can be used to prevent players
     from having multiple versions of the same weapon.
   - Added per-crate settings "DisableInfantry" and "DisableVehicle" which can be used to turn a generic crate into a vehicle or infantry only crate.
   - Added crate odds modifier "Stealth" which will be applied if the player or their vehicle is stealthed.
   - Added crate odds modifiers "Vehicle" and "Infantry" which will be applied if the player is or is not in a vehicle.
   - Added a warning message to players when they try to deploy a beacon on the pedestal when pedestal beacons are disabled.
   - Settings "DisableDamagePoints", "DisableDeathPoints", and "DisableTeamScoreCounter" have been added.
   - New setting "CreditsMultiplier" to control how many credits are granted per-point.
   - New settings "NodRefineryDumpAmount", "GDIRefineryDumpAmount", "NodRefineryTickAmount", and "GDIRefineryTickAmount" to control how many credits Refineries grant.
   - A BRenBot plugin is now included that will display DA's log messages on IRC.
   - Settings "C4", "Beacons", "EnterVehicles", "DriveVehicles", and "StealVehicles" have been added to the per-character disable list.
   - Added vehicle theft log messages under the "_VEHICLE" header.
   - New console function "mapch" to check if a scripts 3.4 client has a given file.
   - Added support for the cheat warnings sent by RR clients. 
   - Added "!vkills" chat command to get how many enemy vehicles a player has killed.
   - Added "!killme" chat command that will kill a player after 10 seconds.
  
  Changes:
   - The formats for the damage and kill events have been changed. If your plugin uses these you will need to update them and recompile.
   - The owner of a multi-seat vehicle will no longer always be put into the driver seat when entering their vehicle.
   - The "!lock" and "!vkick" commands will now move the owner of the vehicle to the driver seat if they remove the current driver.
   - Vehicles that have been put up for grabs with the "!free" command will now auto-bind when a player enters them.
   - The UAV Crate will now only mark stealth units when they're decloaked.
   - Players will now remain in their vehicle when picking up the Overhaul Crate.
   - Renamed "EnableStockKillMessages" setting to "DisableKillMessages".

  Bug Fixes:
   - The UAV Crate will now mark beacons.
   - Fixed a bug where beacons placed on the pedestal could be incorrectly considered as fake.
   - Fixed some bugs in the damage and kill events and removed the ugly workarounds that were previously used.
   - The "VehiclesDestroyed" player stat now only counts enemy player vehicles.
   - The "Squishes" player stat now works.
   - Fixed responses to chat commands appearing in the chat window before the command itself.



Version 1.3:
  Additions:
   - Crates have been added. See da_crates.ini.
   - The settings manager has been overhauled and can now support multiple settings files beyond just da.ini and gamemode.ini. This is used to load
     da_crates.ini, but can just as easily be used in your own plugins.
   - Added setting "EnableStockKillMessages".
   - Added settings "DisableKillCounter", "DisableTeamKillCounter", "DisableDeathCounter", and "DisableTeamDeathCounter".
   - Added some crate and settings examples to the example plugin.
   - Added better protection against sending long messages which could cause clients to crash.

  Changes:
   - Switched to a (hopefully) better random number generator.
   - The Vehicle Queue will now gray out the vehicle menu for a player when their vehicle is building.

  Bug Fixes:
   - Fixed a bug where vehicle shells could sometimes not be seen by players that joined the game after the shell was created.
   - Settings and section names are no longer case sensitive.
   - Points Distribution will no longer give points for killing neutral vehicles that never had a player in them.
   - Fixed team swapping not working correctly if "SwapChance" was set to anything besides 50%.
   - Fixed a rare crash on map end.
   - Players will no longer get a death when switching teams.
   - Fixed an exploit in stock Renegade where a player could avoid a death by leaving the game after they died but before they respawned.



Version 1.2:
  Additions:
   - Added setting "ForceTT". This can be used in three ways:
     0: Allow non-TT clients to join the server.
     1: Allow non-TT clients to join the server and use server damage extrapolation for them. This means they will be unable to use any kind of damage modifying cheat, but their game will appear laggier.
     2: Allow only TT clients to join the server.
   - New Console functions to toggle server damage extrapolation for a player.
   - Added setting "TTRevision". This should be set to the latest publicly released TT revision to ensure all TT players are running that revision.
   - Added setting "DropCommandExpireTime" to the Loot Game Feature.
   - Parachutes now have sounds.
   - Headshot kill messages will now work when the killer is AI controlled.
   - Added "!unstuck" chat command that will attempt to move a player if they are stuck.
   - Added version of the "!vlimit" chat command that works when the Vehicle Queue is disabled.

  Changes:
   - Team remixing should now be more random.
   - Team rebalancing has been improved.
   - All beacon log messages now include the owner of the beacon.
   - Headshot kill messages will now be more accurate when the killer is a non-TT client.
   - The functions relating to fixing stuck objects have been improved.
   - Updated some out of date scripts files.

  Bug Fixes:
   - Fixed a bug in the partial nick matching used in various commands.
   - GameSpy should now display the correct next map.
   - Fixed some bugs in the chat command system.
   - Fixed page sound not playing for the player sending the page.
   - Beacon disarm log messages now trigger when the beacon is disarmed by means other than damage.
   - Fixed possible crash in RTC Game Feature when a player is on the neutral team.
   - Fixed some bugs in the Squad System.



Version 1.1:
  Additions:
   - The Drop Weapons Game Feature is now the Loot Game Feature and includes support for dropping backpacks, DNA powerups, and generic powerups(health/armor/weapons).
     The odds for what type of powerup will drop, and many other things, can be configured in the settings file.
   - Backpacks, and the newly added DNA poweups, will now have an icon above them that shows what weapons or character they contain.
   - The Parachutes Game Feature has been added. If enabled parachutes will automatically deploy when a player exits a flying vehicle above a certain height.
   - Messages have been added for when a player steals an enemy vehicle.
   - Setting "BlockFakeBeacons" has been added and will prevent players from deploying beacons that would not damage an enemy building.
   - Settings "EnablePlayerKillMessages", "EnableVehicleKillMessages", "EnableBuildingKillMessages", and "EnableBotKillMessages" have been added to the Advanced Kill Messages Game Feature.
   - Support for the vote keyboard shortcuts has been added.
   - Keyboard shortcut "VehKick" has been added to trigger "!vkick all". Clients will need to add and bind this key in the extended options menu to use it.
   - New console function "teammessage"/"thmsg" to send a host message to a specific team.
   - Some new documentation and examples have been added to the example plugin.
   - Added macros for simpler registering of global chat commands and key hooks.
   - Added macro to register console functions.
   - Events PowerUp_Grant, PowerUp_Grant_Request, Add_Weapon_Request, Add_Weapon, Remove_Weapon, and Clear_Weapons have been added to DAGameObjObserverClass.
   - Chat commands that are triggered by key hooks will now be passed TEXT_MESSAGE_KEYHOOK as the chat type.
  
  Changes:
   - All player based events will now be passed the player's cPlayer instead of their SoldierGameObj.
   - Vehicle shells will no longer spawn from AI controlled vehicles.
   - The _VEHKILL log messages have been split into _VEHKILL(player controllable vehicles), _HARVKILL(harvesters), _BUILDINGKILL(turrets/guard towers), and _BOTKILL(any other AI controlled vehicle).
   - The team messages generated by the Extra Radio Commands Game Feature and beacon deployments will no longer show up on IRC.

  Bug Fixes:
   - Fixed some memory leaks.
   - "CanRepairBuildings", "IsFriendlyFirePermitted", and "BeaconPlacementEndsGame" now work correctly when used in da.ini or gamemode.ini.
   - Fixed some color messages not working for players with scripts 2.6-3.4.
   - Fixed not being able to compile in debug mode.
   - Black Hand Rocket Soldiers in Domination mode will now work correctly with the Loot Game Feature.



Version 1.0.1:
  Bug Fixes:
   - Added missing cPlayer exports that would cause some plugins to not load correctly.
   - Fixed crash that occurred on certain fan maps with Tiberium Silos.
   - Fixed one of the examples in the example plugin not working.



Version 1.0:
   - Initial release


