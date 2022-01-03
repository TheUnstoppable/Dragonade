/*	Renegade Scripts.dll
	Copyright 2013 Tiberian Technologies

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
#include "slist.h"
#include "GameObjManager.h"
#include "engine_tt.h"
#include "engine_game.h"
#include "engine_obj.h"
#include "engine_def.h"
#include "engine_player.h"
#include "engine_obj2.h"
#include "engine_script.h"
#include "engine_tdb.h"
#include "SmartGameObjDef.h"
#include "VehicleGameObj.h"
#include "SoldierGameObj.h"
#include "SoldierGameObjDef.h"
#include "VehicleFactoryGameObj.h"
#include "VehicleFactoryGameObjDef.h"
#include "AirFactoryGameObj.h"
#include "Iterator.h"
#include "NavalFactoryGameObj.h"
#include "SCAnnouncement.h"
#include "cScTextObj.h"
#include "SuperweaponGameObj.h"
#include "DefinitionMgrClass.h"
#include "PhysicsSceneClass.h"
#include "C4GameObj.h"
#include "TeamPurchaseSettingsDefClass.h"
#include "PurchaseSettingsDefClass.h"
#include "PowerUpGameObjDef.h"
class WideStringClass;


SCRIPTS_API gpl Get_Player_List;
SCRIPTS_API gcmi Get_Current_Map_Index;
SCRIPTS_API gm Get_Map;
SCRIPTS_API gmc Get_Map_Count;
SCRIPTS_API smx Set_Map;
SCRIPTS_API ggd Get_Game_Definitions;
SCRIPTS_API _Enable_Stealth_Player Enable_Stealth_Player;
SCRIPTS_API _Set_Fog_Enable_Player Set_Fog_Enable_Player;
SCRIPTS_API _Set_Fog_Range_Player Set_Fog_Range_Player;
SCRIPTS_API _Set_Background_Music_Player Set_Background_Music_Player;
SCRIPTS_API _Fade_Background_Music_Player Fade_Background_Music_Player;
SCRIPTS_API _Stop_Background_Music_Player Stop_Background_Music_Player;
SCRIPTS_API _Enable_Radar_Player Enable_Radar_Player;
SCRIPTS_API _Display_GDI_Player_Terminal_Player Display_GDI_Player_Terminal_Player;
SCRIPTS_API _Display_NOD_Player_Terminal_Player Display_NOD_Player_Terminal_Player;
SCRIPTS_API _Set_Screen_Fade_Color_Player Set_Screen_Fade_Color_Player;
SCRIPTS_API _Set_Screen_Fade_Opacity_Player Set_Screen_Fade_Opacity_Player;
SCRIPTS_API _Force_Camera_Look_Player Force_Camera_Look_Player;
SCRIPTS_API _Enable_HUD_Player Enable_HUD_Player;
SCRIPTS_API _Create_Sound_Player Create_Sound_Player;
SCRIPTS_API _Create_2D_Sound_Player Create_2D_Sound_Player;
SCRIPTS_API _Create_2D_WAV_Sound_Player Create_2D_WAV_Sound_Player;
SCRIPTS_API _Create_3D_WAV_Sound_At_Bone_Player Create_3D_WAV_Sound_At_Bone_Player;
SCRIPTS_API _Create_3D_Sound_At_Bone_Player Create_3D_Sound_At_Bone_Player;
SCRIPTS_API aoch AddObjectCreateHook;
SCRIPTS_API roch RemoveObjectCreateHook;
SCRIPTS_API asn AddShaderNotify;
SCRIPTS_API rsn RemoveShaderNotify;
SCRIPTS_API akh AddKeyHook;
SCRIPTS_API rkh RemoveKeyHook;
SCRIPTS_API ach AddChatHook;
SCRIPTS_API ahh AddHostHook;
SCRIPTS_API arh AddRefillHook;
SCRIPTS_API allh AddLoadLevelHook;
SCRIPTS_API allh AddPreLoadLevelHook;
SCRIPTS_API allh AddGameOverHook;
SCRIPTS_API allh AddThinkHook;
SCRIPTS_API apjh AddPlayerJoinHook;
SCRIPTS_API arch AddRadioHook;
SCRIPTS_API asdh AddStockDamageHook;
SCRIPTS_API atdh AddTtDamageHook;
SCRIPTS_API aplh AddPlayerLeaveHook;
SCRIPTS_API aph AddPowerupPurchaseHook;
SCRIPTS_API aph AddVehiclePurchaseHook;
SCRIPTS_API aph AddCharacterPurchaseHook;
SCRIPTS_API apmh AddPowerupPurchaseMonHook;
SCRIPTS_API apmh AddVehiclePurchaseMonHook;
SCRIPTS_API apmh AddCharacterPurchaseMonHook;
SCRIPTS_API rph RemovePowerupPurchaseHook;
SCRIPTS_API rph RemoveVehiclePurchaseHook;
SCRIPTS_API rph RemoveCharacterPurchaseHook;
SCRIPTS_API rph RemovePowerupPurchaseMonHook;
SCRIPTS_API rph RemoveVehiclePurchaseMonHook;
SCRIPTS_API rph RemoveCharacterPurchaseMonHook;
SCRIPTS_API gcmt GetCurrentMusicTrack;
SCRIPTS_API sit Set_Info_Texture;
SCRIPTS_API cit Clear_Info_Texture;
SCRIPTS_API svl Set_Vehicle_Limit;
SCRIPTS_API gvl Get_Vehicle_Limit;
SCRIPTS_API svl Set_Air_Vehicle_Limit;
SCRIPTS_API gvl Get_Air_Vehicle_Limit;
SCRIPTS_API svl Set_Naval_Vehicle_Limit;
SCRIPTS_API gvl Get_Naval_Vehicle_Limit;
SCRIPTS_API sm Send_Message;
SCRIPTS_API smp Send_Message_Player;
SCRIPTS_API lnhi Load_New_HUD_INI;
SCRIPTS_API rw Remove_Weapon;
SCRIPTS_API crm Change_Radar_Map;
SCRIPTS_API gbm Get_Build_Time_Multiplier;
SCRIPTS_API scb Set_Currently_Building;
SCRIPTS_API icb Is_Currently_Building;
SCRIPTS_API acoh AddConsoleOutputHook;
SCRIPTS_API acoh AddLogFileHook;
SCRIPTS_API sfc Set_Fog_Color;
SCRIPTS_API sfcp Set_Fog_Color_Player;
SCRIPTS_API sfm Set_Fog_Mode;
SCRIPTS_API sfmp Set_Fog_Mode_Player;
SCRIPTS_API shn Send_HUD_Number;
SCRIPTS_API sfd Set_Fog_Density;
SCRIPTS_API sfdp Set_Fog_Density_Player;
SCRIPTS_API ctr Change_Time_Remaining;
SCRIPTS_API ctl Change_Time_Limit;
SCRIPTS_API dps Display_Security_Dialog;
SCRIPTS_API geo GetExplosionObj;
SCRIPTS_API seo SetExplosionObj;
SCRIPTS_API uno Update_Network_Object;
SCRIPTS_API unop Update_Network_Object_Player;
SCRIPTS_API sct Send_Client_Text;
SCRIPTS_API sca Send_Client_Announcement;
SCRIPTS_API spkm Send_Player_Kill_Message;
SCRIPTS_API spr Send_Purchase_Response;
SCRIPTS_API dod Do_Objectives_Dlg;
SCRIPTS_API sl Set_Player_Limit;
SCRIPTS_API gl Get_Player_Limit;
SCRIPTS_API _Get_Fog_Range Get_Fog_Range;
SCRIPTS_API sspp Send_Shader_Param_Player;
SCRIPTS_API ssp Send_Shader_Param;
SCRIPTS_API sspop Send_Shader_Param_Obj_Player;
SCRIPTS_API sspo Send_Shader_Param_Obj;
SCRIPTS_API ssn Set_GDI_Soldier_Name;
SCRIPTS_API ssn Set_Nod_Soldier_Name;
SCRIPTS_API gsnx Get_GDI_Soldier_Name;
SCRIPTS_API gsnx Get_Nod_Soldier_Name;
SCRIPTS_API Get_IP_Addressx Get_IP_Address;
SCRIPTS_API Get_IP_Portx Get_IP_Port;
SCRIPTS_API Get_IPx Get_IP;
SCRIPTS_API Get_Bandwidthx Get_Bandwidth;
SCRIPTS_API Get_Pingx Get_Ping;
SCRIPTS_API Get_Kbitsx Get_Kbits;
SCRIPTS_API sfop Set_Fog_Override_Player;
SCRIPTS_API cfop Clear_Fog_Override_Player;
SCRIPTS_API dgh Display_Game_Hint;
SCRIPTS_API dghi Display_Game_Hint_Image;
SCRIPTS_API sclb Set_Current_Bullets;
SCRIPTS_API sclb Set_Current_Clip_Bullets;
SCRIPTS_API spb Set_Position_Bullets;
SCRIPTS_API spb Set_Position_Clip_Bullets;
SCRIPTS_API sb Set_Bullets;
SCRIPTS_API sb Set_Clip_Bullets;
SCRIPTS_API smie Set_Moon_Is_Earth;
SCRIPTS_API smie Set_Global_Stealth_Disable;
SCRIPTS_API gml Get_Mine_Limit;
SCRIPTS_API sbd Set_Special_Base_Destruction;
SCRIPTS_API gpv Get_Client_Version;
SCRIPTS_API gpr Get_Client_Revision;
SCRIPTS_API gpr Get_Revision;
SCRIPTS_API gdw Get_Damage_Warhead;
SCRIPTS_API gdie Get_Damage_Is_Explosion;
SCRIPTS_API ggobpn Get_GameObj_By_Player_Name;
SCRIPTS_API scap Send_Custom_All_Players;
SCRIPTS_API stc Steal_Team_Credits;
SCRIPTS_API gtc Get_Team_Credits;
SCRIPTS_API addConnectionAcceptanceFilterType addConnectionAcceptanceFilter;
SCRIPTS_API removeConnectionAcceptanceFilterType removeConnectionAcceptanceFilter;
SCRIPTS_API iwu Is_WOL_User;
SCRIPTS_API ec Evict_Client;
SCRIPTS_API gcsh Get_Client_Serial_Hash;
SCRIPTS_API gml Get_Tech_Level;
SCRIPTS_API sw Set_Tech_Level;
SCRIPTS_API cee Create_Explosion_Extended;
SCRIPTS_API rwpa Retrieve_Waypaths;
SCRIPTS_API rwpo Retrieve_Waypoints;
SCRIPTS_API gwp Get_Waypoint_Position;
SCRIPTS_API trc Test_Raycast_Collision;
SCRIPTS_API iib Is_Inside_AABox;
SCRIPTS_API cgst Can_Generic_Soldier_Teleport;
SCRIPTS_API cl Create_Lightning;
SCRIPTS_API gc Get_Clouds;
SCRIPTS_API gli Get_Lightning;
SCRIPTS_API gw Get_Wind;
SCRIPTS_API gp Get_Rain;
SCRIPTS_API gp Get_Snow;
SCRIPTS_API gp Get_Ash;
SCRIPTS_API ugo Update_Game_Options;
SCRIPTS_API gttv GetTTVersion;
SCRIPTS_API gttr GetTTRevision;
SCRIPTS_API htp Set_HUD_Help_Text_Player;
SCRIPTS_API csp Change_String_Player;
SCRIPTS_API fpu Force_Position_Update;
SCRIPTS_API grps Get_Random_Pathfind_Spot;
SCRIPTS_API htpt Set_HUD_Help_Text_Player_Text;
SCRIPTS_API grw Grant_Weapon;
SCRIPTS_API sovfp Set_Object_Visibility_For_Player;
SCRIPTS_API sov Set_Object_Visibility;
SCRIPTS_API lscg Lock_Soldier_Collision_Group;
SCRIPTS_API ulscg Unlock_Soldier_Collision_Group;
SCRIPTS_API iea Is_Engine_Enabled;
SCRIPTS_API ss Stop_Timer;
SCRIPTS_API ss2 Stop_Timer2;
SCRIPTS_API htm Has_Timer;
SCRIPTS_API cwsdp Create_2D_Wave_Sound_Dialog_Player;
SCRIPTS_API fpup Force_Position_Update_Player;
SCRIPTS_API sbgmo Set_Background_Music_Player_Offset;
SCRIPTS_API setcam Set_Camera_Player;
SCRIPTS_API settran Set_Definition_TranslationID_Player;
SCRIPTS_API setnur Set_Net_Update_Rate_Player;
SCRIPTS_API fou Force_Orientation_Update;
SCRIPTS_API foup Force_Orientation_Update_Player;
SCRIPTS_API cwscp Create_2D_Wave_Sound_Cinematic_Player;
SCRIPTS_API ftu Force_Turret_Update;
SCRIPTS_API ftup Force_Turret_Update_Player;
SCRIPTS_API fvu Force_Velocity_Update;
SCRIPTS_API fvup Force_Velocity_Update_Player;
SCRIPTS_API schn Set_Camera_Host_Network;
SCRIPTS_API gpd Get_Pathfind_Distance;
SCRIPTS_API gpda Get_Pathfind_Distance_Async;
SCRIPTS_API cgpd Cancel_Get_Pathfind_Distance;
SCRIPTS_API gpdb Get_Pathfind_Distance_Blocking;
SCRIPTS_API gmsl Get_Multiplayer_Spawn_Location;
SCRIPTS_API esbn Enable_Spawners_By_Name;
SCRIPTS_API ipg Is_Pathfind_Generated;
SCRIPTS_API gcps Get_Closest_Pathfind_Spot;
SCRIPTS_API gcpss Get_Closest_Pathfind_Spot_Size;
SCRIPTS_API grcs Get_Radio_Command_String;
SCRIPTS_API sei Set_Emot_Icon;
SCRIPTS_API kmd Kill_Messages_Disabled;
SCRIPTS_API ise Is_Sidebar_Enabled;
SCRIPTS_API iee Is_Extras_Enabled;
SCRIPTS_API cbg Can_Build_Ground;
SCRIPTS_API cba Can_Build_Air;
SCRIPTS_API cbn Can_Build_Naval;
SCRIPTS_API isb Is_Soldier_Busy;
SCRIPTS_API ioep Is_On_Enemy_Pedestal;
SCRIPTS_API fcpp Find_Closest_Poly_Position;
SCRIPTS_API sdd Say_Dynamic_Dialogue;
SCRIPTS_API sddp Say_Dynamic_Dialogue_Player;
SCRIPTS_API sddt Say_Dynamic_Dialogue_Team;
SCRIPTS_API elp Enable_Letterbox_Player;
SCRIPTS_API cst Create_Sound_Team;
SCRIPTS_API c2dst Create_2D_Sound_Team;
SCRIPTS_API c2dwst Create_2D_WAV_Sound_Team;
SCRIPTS_API c2dwstd Create_2D_WAV_Sound_Team_Dialog;
SCRIPTS_API c2dwstc Create_2D_WAV_Sound_Team_Cinematic;
SCRIPTS_API c3dwsbt Create_3D_WAV_Sound_At_Bone_Team;
SCRIPTS_API c3dsbt Create_3D_Sound_At_Bone_Team;
SCRIPTS_API _Stop_Sound_Player Stop_Sound_Player;
SCRIPTS_API _Stop_Sound_Team Stop_Sound_Team;
SCRIPTS_API ssa Set_Subobject_Animation;
SCRIPTS_API sts Set_Time_Scale;
SCRIPTS_API ssap Set_Subobject_Animation_Player;
SCRIPTS_API wfa Write_File_Async;

SCRIPTS_API bool Can_Team_Build_Vehicle(int Team)
{
	if (Is_Currently_Building(Team))
	{
		return false;
	}
	GameObject *factory = Find_Vehicle_Factory(Team);
	if (factory)
	{
		return Is_Available_For_Purchase(factory);
	}
	return true;
}

SCRIPTS_API DynamicVectorClass<int> Get_Enlisted_Purchase_Items(int team)
{
	DynamicVectorClass<int> list;
	TeamPurchaseSettingsDefClass *t = TeamPurchaseSettingsDefClass::Get_Definition((TeamPurchaseSettingsDefClass::TEAM)PTTEAM(team));
	for (int i = 0; i < 4; i++)
	{
		if (t->Get_Enlisted_Definition(i) != 0)
			list.Add(t->Get_Enlisted_Definition(i));
	}
	return list;
}

SCRIPTS_API DynamicVectorClass<int> Get_Purchase_Items(int team, int defType)
{
	DynamicVectorClass<int> list;
	PurchaseSettingsDefClass *purchaseSettings = PurchaseSettingsDefClass::Find_Definition((PurchaseSettingsDefClass::TYPE)defType, (PurchaseSettingsDefClass::TEAM)PTTEAM(team));
	if (purchaseSettings)
	{
		for (int i = 0; i < 10; i++)
		{
			if (purchaseSettings->Get_Definition(i) != 0)
				list.Add(purchaseSettings->Get_Definition(i));
		}
	}
	return list;
}

SCRIPTS_API bool Is_Preset_Purchasable_In_List(int id, int team, PurchaseSettingsDefClass::TYPE def)
{
	PurchaseSettingsDefClass *purchaseSettings = PurchaseSettingsDefClass::Find_Definition(def, (PurchaseSettingsDefClass::TEAM)PTTEAM(team));
	if (purchaseSettings && !purchaseSettings->Get_Page_Hidden() && !purchaseSettings->Get_Page_Busy() && !purchaseSettings->Get_Page_Disabled())
	{
		for (int i = 0; i < 10; i++)
		{
			if ((purchaseSettings->Get_Definition(i) == id) ||
				(purchaseSettings->Get_Alt_Definition(i, 0) == id) || (purchaseSettings->Get_Alt_Definition(i, 1) == id) || (purchaseSettings->Get_Alt_Definition(i, 2) == id))
			{
				// No unit limit in 4.x?
				/*if (int unitlimit = purchaseSettings->Get_Unit_Limit(i))
				{
					int count = Count_Objects_Of_Preset(2, purchaseSettings->Get_Definition(i));
					count += Count_Objects_Of_Preset(2, purchaseSettings->Get_Alt_Definition(i, 0));
					count += Count_Objects_Of_Preset(2, purchaseSettings->Get_Alt_Definition(i, 1));
					count += Count_Objects_Of_Preset(2, purchaseSettings->Get_Alt_Definition(i, 2));
					if (unitlimit <= count)
					{
						return false;
					}
				}*/
				return !purchaseSettings->Get_Disabled(i) && !purchaseSettings->Get_Busy(i) && !purchaseSettings->Get_Hidden(i);
			}
		}
	}
	return false;
}

SCRIPTS_API bool Is_Infantry_Purchaseable(const char *preset,int team)
{
	int id = Get_Definition_ID(preset);
	if (!id)
	{
		return false;
	}

	TeamPurchaseSettingsDefClass *t = TeamPurchaseSettingsDefClass::Get_Definition((TeamPurchaseSettingsDefClass::TEAM)PTTEAM(team));
	
	for (int i = 0; i < 4; i++)
	{
		if (t->Get_Enlisted_Definition(i) == id)
		{
			// No unit limit in 4.x?
			/*int unitlimit = t->Get_Unit_Limit(i);
			if (unitlimit)
			{
				if (unitlimit <= Count_Objects_Of_Preset(2, t->Get_Enlisted_Definition(i)))
				{
					return false;
				}
			}*/
			return !t->Get_Disabled(i) && !t->Get_Busy(i) && !t->Get_Hidden(i);
		}
	}

	BaseControllerClass *baseController = BaseControllerClass::Find_Base(team); // This does not require a PurchaseSettingsDefClass team

	if (!baseController || baseController->Can_Generate_Soldiers())
	{
		if (Is_Preset_Purchasable_In_List(id, team, PurchaseSettingsDefClass::TYPE_CLASSES))
			return true;

		if (Is_Preset_Purchasable_In_List(id, team, PurchaseSettingsDefClass::TYPE_SECRET_CLASSES))
			return true;
	}

	return false;
}

SCRIPTS_API bool Is_Vehicle_Purchaseable(const char *preset,int team)
{
	int id = Get_Definition_ID(preset);
	if (!id)
	{
		return false;
	}

	BaseControllerClass *baseController = BaseControllerClass::Find_Base(team); // This does not require a PurchaseSettingsDefClass team
	
	if (!baseController || (baseController->Can_Generate_Vehicles() && Can_Build_Ground(team)))
	{
		if (Is_Preset_Purchasable_In_List(id, team, PurchaseSettingsDefClass::TYPE_VEHICLES) && !(Get_Vehicle_Definition_Mode(preset) == VEHICLE_TYPE_FLYING && !Is_Map_Flying()))
			return true;

		if (Is_Preset_Purchasable_In_List(id, team, PurchaseSettingsDefClass::TYPE_SECRET_VEHICLES))
			return true;
	}

	if (!baseController || Can_Build_Air(team))
	{
		if (Is_Preset_Purchasable_In_List(id, team, PurchaseSettingsDefClass::TYPE_AIR))
			return true;

		// Air vehicles can be present on the ground vehicle lists!
		if (Is_Preset_Purchasable_In_List(id, team, PurchaseSettingsDefClass::TYPE_VEHICLES) && !(Get_Vehicle_Definition_Mode(preset) == VEHICLE_TYPE_FLYING && !Is_Map_Flying()))
			return true;

		if (Is_Preset_Purchasable_In_List(id, team, PurchaseSettingsDefClass::TYPE_SECRET_VEHICLES))
			return true;
	}

	if (!baseController || Can_Build_Naval(team))
	{
		if (Is_Preset_Purchasable_In_List(id, team, PurchaseSettingsDefClass::TYPE_NAVAL))
			return true;
	}

	return false;
}

SCRIPTS_API void Send_Message_With_Team_Color(int Team,const char *Msg)
{
	unsigned int Red = 0,Blue = 0,Green = 0;
	Get_Team_Color(Team,&Red,&Green,&Blue);
	Send_Message(Red,Green,Blue,Msg);
}

SCRIPTS_API void Send_Message_With_Obj_Color(GameObject *obj,const char *Msg)
{
	unsigned int Red = 0,Blue = 0,Green = 0;
	Get_Object_Color(obj,&Red,&Green,&Blue);
	Send_Message(Red,Green,Blue,Msg);
}

SCRIPTS_API void Send_Message_Team(int team,unsigned int red,unsigned int green,unsigned int blue,const char *msg)
{
	SLNode<SoldierGameObj> *x = GameObjManager::StarGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			if ((Get_Object_Type(o) == team) || (team == 2))
			{
				Send_Message_Player(o,red,green,blue,msg);
			}
		}
		x = x->Next();
	}
}

// -------------------------------------------------------------------------------------------------

SCRIPTS_API void Send_Translated_Message_Team ( unsigned long ID, int team, int red, int green, int blue, bool bPlaySound )
{
  if ( Is_Valid_String_ID(ID) )
  {
    const char *str = Get_Translated_String(ID);
    Send_Message_Team(team,red,green,blue,str);
    delete[] str;

    if ( bPlaySound )
    {
      int soundId = Get_String_Sound_ID(ID);
      if (soundId && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000)
        Create_2D_Sound_Team(Get_Definition_Name(soundId),team);
    }
  }
}

// -------------------------------------------------------------------------------------------------

SCRIPTS_API void Send_Translated_Message_Team ( unsigned long ID, int team, Vector3 rgb, bool bPlaySound )
{
  int red = (int)(rgb.X * 255);
  int green = (int)(rgb.Y * 255);
  int blue = (int)(rgb.Z * 255);

  Send_Translated_Message_Team(ID, team, red, green, blue, bPlaySound);
}

// -------------------------------------------------------------------------------------------------

SCRIPTS_API void Set_Occupants_Fade(GameObject *obj,float red,float green,float blue,float opacity)
{
	if (!obj)
	{
		return;
	}
	if (!obj->As_VehicleGameObj())
	{
		return;
	}
	VehicleGameObj *o = obj->As_VehicleGameObj();
	int x = o->Get_Definition().Get_Seat_Count();
	for (int i = 0;i < x;i++)
	{
		if (o->Get_Occupant(i))
		{
			Set_Screen_Fade_Color_Player(o->Get_Occupant(i),red,green,blue,0);
			Set_Screen_Fade_Opacity_Player(o->Get_Occupant(i),opacity,0);
		}
	}
}

SCRIPTS_API void Enable_Team_Radar(int Team,bool Enable)
{
	SLNode<SoldierGameObj> *x = GameObjManager::StarGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			if ((Get_Object_Type(o) == Team) || (Team == 2))
			{
				Enable_Radar_Player(o,Enable);
			}
		}
		x = x->Next();
	}
}

SCRIPTS_API void Ranged_Stealth_On_Team(Gap_ListNode* FirstNode)
{
	SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();
	if (FirstNode)
	{
		while (x)
		{
			VehicleGameObj *o = x->Data();
			if (o)
			{
				bool stealth = false;
				Gap_ListNode *current = FirstNode;
				int test = Commands->Get_Player_Type(o);
				if (o->Get_Lock_Team() != 2)
				{
					test = o->Get_Lock_Team();
				}
				const VehicleGameObjDef *d = &o->Get_Definition();
				if (!d->Is_Stealthed() && !Is_Script_Attached(o, "Stealth_Powerup") && !Is_Script_Attached(o, "Stealth_Stationary") && !o->Peek_Model()->Is_Hidden())
				{
					while (current)
					{
						if (current->active)
						{
							if (test == current->teamID)
							{
								Vector3 GenPos = Commands->Get_Position(Commands->Find_Object(current->unitID));
								Vector3 TargetPos = Commands->Get_Position(o);
								float distance2 = (GenPos - TargetPos).Length2();
								if (distance2 <= current->range * current->range)
								{
									stealth = true;
								}
							}
						}
						current = current->Next;
					}
					Commands->Enable_Stealth(o, stealth);
					current = NULL;
				}
			}
			x = x->Next();
		}
	}
	else
	{
		while (x)
		{
			VehicleGameObj *o = x->Data();
			if (o)
			{
				const VehicleGameObjDef *d = &o->Get_Definition();
				if (!d->Is_Stealthed() && !Is_Script_Attached(o, "Stealth_Powerup") && !Is_Script_Attached(o, "Stealth_Stationary") && !o->Peek_Model()->Is_Hidden())
				{
					Commands->Enable_Stealth(o, false);
				}
			}
			x = x->Next();
		}
	}

	SLNode<SoldierGameObj> *x2 = GameObjManager::SoldierGameObjList.Head();
	if (FirstNode)
	{
		while (x2)
		{
			SoldierGameObj *o = x2->Data();
			if (o)
			{
				bool stealth = false;
				Gap_ListNode *current = FirstNode;
				int test = Commands->Get_Player_Type(o);
				const SoldierGameObjDef *d = &o->Get_Definition();
				if (!d->Is_Stealthed() && !Is_Script_Attached(o, "Stealth_Powerup") && !Is_Script_Attached(o, "Stealth_Stationary") && !o->Peek_Model()->Is_Hidden())
				{
					while (current)
					{
						if (current->active)
						{
							if (test == current->teamID)
							{
								Vector3 GenPos = Commands->Get_Position(Commands->Find_Object(current->unitID));
								Vector3 TargetPos = Commands->Get_Position(o);
								float distance2 = (GenPos - TargetPos).Length2();
								if (distance2 <= current->range * current->range)
								{
									stealth = true;
								}
							}
						}
						current = current->Next;
					}
					Commands->Enable_Stealth(o, stealth);
					current = NULL;
				}
			}
			x2 = x2->Next();
		}
	}
	else
	{
		while (x2)
		{
			SoldierGameObj *o = x2->Data();
			if (o)
			{
				const SoldierGameObjDef *d = &o->Get_Definition();
				if (!d->Is_Stealthed() && !Is_Script_Attached(o, "Stealth_Powerup") && !Is_Script_Attached(o, "Stealth_Stationary") && !o->Peek_Model()->Is_Hidden())
				{
					Commands->Enable_Stealth(o, false);
				}
			}
			x2 = x2->Next();
		}
	}
}

SCRIPTS_API void Ranged_Gap_Effect(Gap_ListNode* FirstNode)
{
	SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();
	if (FirstNode)
	{
		while (x)
		{
			VehicleGameObj *o = x->Data();
			if (o)
			{
				bool stealth = false;
				Gap_ListNode *current = FirstNode;
				int test = Commands->Get_Player_Type(o);
				const VehicleGameObjDef *d = &o->Get_Definition();
				if (!d->Is_Stealthed() && d->Get_Type() != VEHICLE_TYPE_SUB && !Is_Script_Attached(o, "Stealth_Powerup") && !Is_Script_Attached(o, "Stealth_Stationary") && !o->Peek_Model()->Is_Hidden())
				{
					while (current)
					{
						if (current->active)
						{
							Vector3 GenPos = Commands->Get_Position(Commands->Find_Object(current->unitID));
							Vector3 TargetPos = Commands->Get_Position(o);
							float distance2 = (GenPos - TargetPos).Length2();
							if (distance2 <= current->range * current->range)
							{
								if (test == current->teamID)
								{
									stealth = true;
								}
							}
						}
						current = current->Next;
					}
					Commands->Enable_Stealth(o, stealth);
					current = NULL;
				}
			}
			x = x->Next();
		}
	}
	else
	{
		while (x)
		{
			VehicleGameObj *o = x->Data();
			if (o)
			{
				const VehicleGameObjDef *d = &o->Get_Definition();
				if (!d->Is_Stealthed() && !Is_Script_Attached(o, "Stealth_Powerup") && !Is_Script_Attached(o, "Stealth_Stationary") && !o->Peek_Model()->Is_Hidden())
				{
					Commands->Enable_Stealth(o, false);
				}
			}
			x = x->Next();
		}
	}

	SLNode<SoldierGameObj> *x2 = GameObjManager::SoldierGameObjList.Head();
	if (FirstNode)
	{
		while (x2)
		{
			SoldierGameObj *o = x2->Data();
			if (o)
			{
				bool stealth = false;
				bool blackout = false;
				Gap_ListNode *current = FirstNode;
				int test = Commands->Get_Player_Type(o);
				const SoldierGameObjDef *d = &o->Get_Definition();
				if (!d->Is_Stealthed() && !Is_Script_Attached(o, "Stealth_Powerup") && !Is_Script_Attached(o, "Stealth_Stationary") && !o->Peek_Model()->Is_Hidden())
				{
					while (current)
					{
						if (current->active)
						{
							Vector3 GenPos = Commands->Get_Position(Commands->Find_Object(current->unitID));
							Vector3 TargetPos = Commands->Get_Position(o);
							float distance2 = (GenPos - TargetPos).Length2();
							if (distance2 <= current->range * current->range)
							{
								if (test == current->teamID)
								{
									stealth = true;
								}
								else
								{
									blackout = true;
								}
							}
						}
						current = current->Next;
					}
					Commands->Enable_Stealth(o, stealth);
					if (Commands->Is_A_Star(o))
					{
						if (blackout)
						{
							Commands->Send_Custom_Event(0,o,325523,1,0);
						}
						else
						{
							Commands->Send_Custom_Event(0,o,325523,0,0);
						}
					}
					current = NULL;
				}
			}
			x2 = x2->Next();
		}
	}
	else
	{
		while (x2)
		{
			SoldierGameObj *o = x2->Data();
			if (o)
			{
				const SoldierGameObjDef *d = &o->Get_Definition();
				if (!d->Is_Stealthed() && !Is_Script_Attached(o, "Stealth_Powerup") && !Is_Script_Attached(o, "Stealth_Stationary") && !o->Peek_Model()->Is_Hidden())
				{
					Commands->Enable_Stealth(o, false);
					if (Commands->Is_A_Star(o))
					{
						Commands->Send_Custom_Event(0,o,325523,0,0);
					}
				}
			}
			x2 = x2->Next();
		}
	}
}

SCRIPTS_API void SendShaderParam(const char* parameter, const char* value)
{
	Send_Shader_Param(parameter, value);
};

SCRIPTS_API void SendShaderParam(const char* parameter, int value) 
{
	char temp[256];
	sprintf(temp, "%d", value);
	Send_Shader_Param(parameter, temp);
};

SCRIPTS_API void SendShaderParam(const char* parameter, unsigned int value) 
{
	char temp[256];
	sprintf(temp, "%u", value);
	Send_Shader_Param(parameter, temp);
};

SCRIPTS_API void SendShaderParam(const char* parameter, float value) 
{
	char temp[256];
	sprintf(temp, "%f", value);
	Send_Shader_Param(parameter, temp);
};

SCRIPTS_API void SendShaderParam(const char* parameter, Vector2 value) 
{
	char temp[256];
	sprintf(temp, "%f|%f", value.X, value.Y);
	Send_Shader_Param(parameter, temp);
};

SCRIPTS_API void SendShaderParam(const char* parameter, Vector3 value) 
{
	char temp[256];
	sprintf(temp, "%f|%f|%f", value.X, value.Y, value.Z);
	Send_Shader_Param(parameter, temp);
};

SCRIPTS_API void SendShaderParam(const char* parameter, Vector4 value) 
{
	char temp[256];
	sprintf(temp, "%f|%f|%f|%f", value.X, value.Y, value.Z, value.W);
	Send_Shader_Param(parameter, temp);
};

SCRIPTS_API void SendShaderParamPlayer(GameObject* player, const char* parameter, const char* value)
{
	Send_Shader_Param_Player(player,parameter, value);
};

SCRIPTS_API void SendShaderParamPlayer(GameObject* player, const char* parameter, int value) 
{
	char temp[256];
	sprintf(temp, "%d", value);
	Send_Shader_Param_Player(player, parameter, temp);
};

SCRIPTS_API void SendShaderParamPlayer(GameObject* player, const char* parameter, unsigned int value) 
{
	char temp[256];
	sprintf(temp, "%u", value);
	Send_Shader_Param_Player(player, parameter, temp);
};

SCRIPTS_API void SendShaderParamPlayer(GameObject* player, const char* parameter, float value) 
{
	char temp[256];
	sprintf(temp, "%f", value);
	Send_Shader_Param_Player(player, parameter, temp);
};

SCRIPTS_API void SendShaderParamPlayer(GameObject* player, const char* parameter, Vector2 value) 
{
	char temp[256];
	sprintf(temp, "%f|%f", value.X, value.Y);
	Send_Shader_Param_Player(player, parameter, temp);
};

SCRIPTS_API void SendShaderParamPlayer(GameObject* player, const char* parameter, Vector3 value) 
{
	char temp[256];
	sprintf(temp, "%f|%f|%f", value.X, value.Y, value.Z);
	Send_Shader_Param_Player(player, parameter, temp);
};

SCRIPTS_API void SendShaderParamPlayer(GameObject* player, const char* parameter, Vector4 value) 
{
	char temp[256];
	sprintf(temp, "%f|%f|%f|%f", value.X, value.Y, value.Z, value.W);
	Send_Shader_Param_Player(player, parameter, temp);
};

SCRIPTS_API void SendShaderParamObj(const char* parameter, const char* value, GameObject* obj)
{
	Send_Shader_Param_Obj(parameter, value, obj);
};

SCRIPTS_API void SendShaderParamObj(const char* parameter, int value, GameObject* obj) 
{
	char temp[256];
	sprintf(temp, "%d", value);
	Send_Shader_Param_Obj(parameter, temp, obj);
};

SCRIPTS_API void SendShaderParamObj(const char* parameter, unsigned int value, GameObject* obj) 
{
	char temp[256];
	sprintf(temp, "%u", value);
	Send_Shader_Param_Obj(parameter, temp, obj);
};

SCRIPTS_API void SendShaderParamObj(const char* parameter, float value, GameObject* obj) 
{
	char temp[256];
	sprintf(temp, "%f", value);
	Send_Shader_Param_Obj(parameter, temp, obj);
};

SCRIPTS_API void SendShaderParamObj(const char* parameter, Vector2 value, GameObject* obj) 
{
	char temp[256];
	sprintf(temp, "%f|%f", value.X, value.Y);
	Send_Shader_Param_Obj(parameter, temp, obj);
};

SCRIPTS_API void SendShaderParamObj(const char* parameter, Vector3 value, GameObject* obj) 
{
	char temp[256];
	sprintf(temp, "%f|%f|%f", value.X, value.Y, value.Z);
	Send_Shader_Param_Obj(parameter, temp, obj);
};

SCRIPTS_API void SendShaderParamObj(const char* parameter, Vector4 value, GameObject* obj) 
{
	char temp[256];
	sprintf(temp, "%f|%f|%f|%f", value.X, value.Y, value.Z, value.W);
	Send_Shader_Param_Obj(parameter, temp, obj);
};

SCRIPTS_API void SendShaderParamObjPlayer(GameObject* player, const char* parameter, const char* value, GameObject* obj)
{
	Send_Shader_Param_Obj_Player(player,parameter, value, obj);
};

SCRIPTS_API void SendShaderParamObjPlayer(GameObject* player, const char* parameter, int value, GameObject* obj) 
{
	char temp[256];
	sprintf(temp, "%d", value);
	Send_Shader_Param_Obj_Player(player, parameter, temp, obj);
};

SCRIPTS_API void SendShaderParamObjPlayer(GameObject* player, const char* parameter, unsigned int value, GameObject* obj) 
{
	char temp[256];
	sprintf(temp, "%u", value);
	Send_Shader_Param_Obj_Player(player, parameter, temp, obj);
};

SCRIPTS_API void SendShaderParamObjPlayer(GameObject* player, const char* parameter, float value, GameObject* obj) 
{
	char temp[256];
	sprintf(temp, "%f", value);
	Send_Shader_Param_Obj_Player(player, parameter, temp, obj);
};

SCRIPTS_API void SendShaderParamObjPlayer(GameObject* player, const char* parameter, Vector2 value, GameObject* obj) 
{
	char temp[256];
	sprintf(temp, "%f|%f", value.X, value.Y);
	Send_Shader_Param_Obj_Player(player, parameter, temp, obj);
};

SCRIPTS_API void SendShaderParamObjPlayer(GameObject* player, const char* parameter, Vector3 value, GameObject* obj) 
{
	char temp[256];
	sprintf(temp, "%f|%f|%f", value.X, value.Y, value.Z);
	Send_Shader_Param_Obj_Player(player, parameter, temp, obj);
};

SCRIPTS_API void SendShaderParamObjPlayer(GameObject* player, const char* parameter, Vector4 value, GameObject* obj) 
{
	char temp[256];
	sprintf(temp, "%f|%f|%f|%f", value.X, value.Y, value.Z, value.W);
	Send_Shader_Param_Obj_Player(player, parameter, temp, obj);
};
RENEGADE_FUNCTION
void ScriptableGameObj::Start_Observers()
AT2(0x006B6C40,0x006B64E0);

SCRIPTS_API void Kill_All_Buildings_By_Team(int Team)
{
	BaseControllerClass::Find_Base(Team)->Set_Base_Destroyed(true);
	Set_Special_Base_Destruction();
}

bool SCRIPTS_API Create_Vehicle(const char *Preset_Name,float Delay,GameObject *Owner,int Team)
{
	if (Owner && Get_Vehicle(Owner))
	{
		return false;
	}
	int preset = Get_Definition_ID(Preset_Name);
	BaseControllerClass *base = BaseControllerClass::Find_Base(Team);
	DefinitionClass *vehicle = Find_Definition(preset);
	if (!vehicle)
	{
		return false;
	}
	BuildingGameObj *bg2 = ((BuildingGameObj *)base->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY));
	VehicleFactoryGameObj *vf = 0;
	if (bg2)
	{
		vf = bg2->As_VehicleFactoryGameObj();
	}
	if (!vf && !Vehicle_Preset_Is_Air(preset) && !Vehicle_Preset_Is_Naval(preset))
	{
		return false;
	}
	if (vf && ((!vf->Is_Available())))
	{
		if (((!Vehicle_Preset_Is_Air(preset)) || (!vf->Get_Definition().Get_Build_Ground_Only())) && !Vehicle_Preset_Is_Naval(preset))
		{
			return false;
		}
	}
	if (vf && (!Vehicle_Preset_Is_Air(preset) || !vf->Get_Definition().Get_Build_Ground_Only()) && !Vehicle_Preset_Is_Naval(preset))
	{
		float delay = 5.0;
		if (!base->Is_Base_Powered())
		{
			delay *= Get_Build_Time_Multiplier(Team);
		}
		vf->Request_Vehicle(preset,delay,Owner ? Owner->As_SoldierGameObj() : 0);
	}
	else if (Vehicle_Preset_Is_Air(preset))
	{
		BuildingGameObj *bg = ((BuildingGameObj *)base->Find_Building(BuildingConstants::TYPE_HELIPAD));
		AirFactoryGameObj *af = 0;
		if (bg)
		{
			af = bg->As_AirFactoryGameObj();
		}
		if (af && af->Is_Available() && Vehicle_Preset_Is_Air(preset))
		{
			af->Create_Vehicle(preset,Owner ? Owner->As_SoldierGameObj() : 0);
		}
		else
		{
			return false;
		}
	}
	else
	{
		BuildingGameObj *bg = ((BuildingGameObj *)base->Find_Building(BuildingConstants::TYPE_NAVAL_FACTORY));
		NavalFactoryGameObj *af = 0;
		if (bg)
		{
			af = bg->As_NavalFactoryGameObj();
		}
		if (!af || !af->Is_Available() || !af->Can_Spawn(preset) || !Vehicle_Preset_Is_Naval(preset))
		{
			return false;
		}
		else
		{
			af->Create_Vehicle(preset,Owner ? Owner->As_SoldierGameObj() : 0);
		}
	}
	return true;
}

SCRIPTS_API REF_DEF2(DynamicVectorClass<ConsoleFunctionClass *>, ConsoleFunctionList, 0x0081DEB8, 0x0081D098);

SCRIPTS_API void Delete_Console_Function(const char *name)
{
	const DynamicVectorClass<ConsoleFunctionClass*> consoleFunctions(ConsoleFunctionList);
	TT_FOREACH(consoleFunction, consoleFunctions)
	{
		const char* fname = (*consoleFunction)->Get_Name();
		if (fname)
		{
			if (strcmp(fname,name) == 0)
			{
				ConsoleFunctionList.DeleteObj(*consoleFunction);
			}
		}
	}
}

SCRIPTS_API void cScTextObj::Set_Dirty_Bit_For_Team(DIRTY_BIT value, int teamId)
{
	TT_FOREACH(player, *Get_Player_List())
	{
		if (player->Is_Active() && player->Get_Player_Type() == teamId)
			Set_Object_Dirty_Bit(player->Get_Id(), value, true);
	}
}

SCRIPTS_API void cNetEvent::Set_Object_Dirty_Bit_For_Revision(uint revision,DIRTY_BIT dirty_bit, bool onoff)
{
	for (int clientId = 1; clientId < 128; ++clientId)
	{
		if (Get_Client_Revision(clientId) >= revision)
		{
			Set_Object_Dirty_Bit(clientId,dirty_bit,onoff);
		}
	}
}

SCRIPTS_API void SCAnnouncement::Set_Dirty_Bit_For_Team(DIRTY_BIT value, int teamId)
{
	TT_FOREACH(player, *Get_Player_List())
	{
		if (player->Is_Active() && player->Get_Player_Type() == teamId)
			Set_Object_Dirty_Bit(player->Get_Id(), value, true);
	}
}

SCRIPTS_API void Force_Vehicle_Entry(GameObject *soldier,GameObject *vehicle)
{
	Commands->Set_Position(soldier,Commands->Get_Position(vehicle));
	vehicle->As_VehicleGameObj()->Add_Occupant(soldier->As_SoldierGameObj());
}

RENEGADE_FUNCTION
SCRIPTS_API void Sort_Function_List()
AT2(0x004282F0,0x00428470);

RENEGADE_FUNCTION
SCRIPTS_API void Verbose_Help_File()
AT2(0x004284F0,0x00428670);

RENEGADE_FUNCTION
SCRIPTS_API void ScriptableGameObj::Add_Observer( GameObjObserverClass * observer )
AT2(0x006B6C70,0x006B6510);

RENEGADE_FUNCTION
SCRIPTS_API void VehicleGameObj::Add_Occupant(SoldierGameObj * occupant )
AT2(0x0067B280,0x0067AB20);

RENEGADE_FUNCTION
SCRIPTS_API void SuperweaponGameObj::Launch()
AT2(0x00740D50,0x007405F0);

RENEGADE_FUNCTION
SCRIPTS_API bool PhysicsSceneClass::Cast_Ray(PhysRayCollisionTestClass & raytest,bool use_collision_region)
AT3(0x00631460,0x00630D00,0x006B0100);

RENEGADE_FUNCTION
SCRIPTS_API bool PhysicsSceneClass::Cast_AABox(class PhysAABoxCollisionTestClass &,bool)
AT3(0x00631530,0x00630DD0,0x006B0200);

RENEGADE_FUNCTION
SCRIPTS_API bool PhysicsSceneClass::Cast_OBBox(class PhysOBBoxCollisionTestClass &,bool)
AT3(0x00631600,0x00630EA0,0x006B0350);

RENEGADE_FUNCTION
SCRIPTS_API void PhysicsSceneClass::Get_Level_Extents(Vector3 &min, Vector3 &max)
AT3(0x00623C80,0x00623520,0x0069FAB0);

RENEGADE_FUNCTION
SCRIPTS_API void PhysicsSceneClass::Collect_Objects(const OBBoxClass & box,bool static_objs,bool dynamic_objs,MultiListClass<PhysClass> * list)
AT2(0x00631E30,0x006316D0);

SCRIPTS_API REF_DEF3(PhysicsSceneClass*, PhysicsSceneClass::TheScene, 0x00855444, 0x0085462C, 0x00872BCC);

RENEGADE_FUNCTION
void C4GameObj::Defuse()
AT2(0x0070D610,0x0070CBD0);

RENEGADE_FUNCTION
TeamPurchaseSettingsDefClass *TeamPurchaseSettingsDefClass::Get_Definition(TeamPurchaseSettingsDefClass::TEAM team)
AT2(0x006F3430,0x006F29F0);
RENEGADE_FUNCTION
PurchaseSettingsDefClass *PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE type,PurchaseSettingsDefClass::TEAM team)
AT2(0x006F2BD0,0x006F2190);

RENEGADE_FUNCTION
void BuildingGameObj::Collect_Building_Components()
AT2(0x006843E0,0x00683C80);

RENEGADE_FUNCTION
void BuildingGameObj::Find_Closest_Poly(const Vector3 &pos, float *distance2)
AT2(0x00685630,0x00684ED0);

RENEGADE_FUNCTION
void BaseControllerClass::Enable_Radar(bool Enable)
AT3(0x006EFD00,0x006EF2C0,0x00558ED0);

RENEGADE_FUNCTION
bool PowerUpGameObjDef::Grant(SmartGameObj *, PowerUpGameObj *, bool) const
AT2(0x006F09F0,0x006EFFB0);

RENEGADE_FUNCTION
void SoldierGameObj::Toggle_Fly_Mode()
AT2(0x006CFC80,0x006CF520);
RENEGADE_FUNCTION
void AirFactoryGameObj::Create_Vehicle(int defintion_id,SoldierGameObj * player)
AT2(0x006EDC80,0x006ED240);
RENEGADE_FUNCTION
int AirFactoryGameObj::Get_Team_Vehicle_Count() const
AT2(0x006F16B0,0x006F0C70);
RENEGADE_FUNCTION
bool AirFactoryGameObj::Is_Available_For_Purchase() const
AT2(0x0070B6F0,0x0070ACB0);
RENEGADE_FUNCTION
SCRIPTS_API bool SoldierGameObj::Is_In_Elevator()
AT2(0x006C9610,0x006C8EB0);
RENEGADE_FUNCTION
void NavalFactoryGameObj::Create_Vehicle(int defintion_id,SoldierGameObj * player)
AT2(0x0073EE30,0x0073E6D0);
RENEGADE_FUNCTION
int NavalFactoryGameObj::Get_Team_Vehicle_Count() const
AT2(0x0073FB80,0x0073F420);
RENEGADE_FUNCTION
bool NavalFactoryGameObj::Is_Available_For_Purchase() const
AT2(0x007408E0,0x00740180);
RENEGADE_FUNCTION
bool NavalFactoryGameObj::Can_Spawn(int definition_id)
AT2(0x007425B0,0x00741E50);

RENEGADE_FUNCTION
bool VehicleFactoryGameObj::Request_Vehicle(int defintion_id, float generation_time,SoldierGameObj * player)
AT2(0x006EE1A0,0x006ED760);
RENEGADE_FUNCTION
int VehicleFactoryGameObj::Get_Team_Vehicle_Count() const
AT2(0x006EE060,0x006ED620);
RENEGADE_FUNCTION
bool VehicleFactoryGameObj::Is_Available_For_Purchase() const
AT2(0x006EE010,0x006ED5D0);
RENEGADE_FUNCTION
void SoldierGameObj::Re_Init(const SoldierGameObjDef &)
AT2(0x006C7410,0x006C6CB0);
RENEGADE_FUNCTION
void SCRIPTS_API cPlayer::Set_Player_Type(int type)
AT2(0x0040D600,0x0040D600);

RENEGADE_FUNCTION
Vector3 SCRIPTS_API Get_Color_For_Team(int teamId)
AT2(0x006D99E0,0x006D9280);
Vector3 DamageableGameObj::Get_Team_Color(void)
{
	return Get_Color_For_Team(PlayerType);
}

void SCRIPTS_API Get_Object_Color(GameObject *obj, unsigned int *red, unsigned int *green, unsigned int *blue)
{
	if (!obj)
	{
		return;
	}
	DamageableGameObj *o = obj->As_DamageableGameObj();
	if (!o)
	{
		return;
	}
	Vector3 v = o->Get_Team_Color();
	*red = (unsigned int)(v.X*255);
	*green = (unsigned int)(v.Y*255);
	*blue = (unsigned int)(v.Z*255);
}

void SCRIPTS_API Grant_Powerup(GameObject *obj,const char *Preset_Name)
{
	if (!obj)
	{
		return;
	}
	SmartGameObj *o = obj->As_SmartGameObj();
	if (o)
	{
		PowerUpGameObjDef *def = (PowerUpGameObjDef *)Find_Named_Definition(Preset_Name);
		def->Grant(o,0,true);
	}
}

